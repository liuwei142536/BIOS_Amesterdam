/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  2007 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  HeciInit.c

Abstract:

  HECI Initialization DXE Driver

--*/

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include <Token.h>
// AptioV server override: Including Token.h here so that token.h gets included first and redefintion errors are avoided when including token.h
#include "HeciInit.h"
#include <Protocol/HeciProtocol.h>
#include <PchAccess.h>
#include <Guid/SetupVariable.h>
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include <Protocol/MeRcInfo.h>
#include <Guid/EventGroup.h>
#include <PchRegs.h>
#include "MkhiMsgs.h"
#include "Protocol/MeBiosPayloadData.h"
#include "MeLib.h"
#include "MeChipset.h"
#include "CommonIncludes.h"

//Aptio Server override: Defining gEfiShellEnvironment2Guid value here.
//extern EFI_GUID gEfiShellEnvironment2Guid;
static EFI_GUID gEfiShellEnvironment2Guid = {0x47c7b221, 0xc42a, 0x11d2, {0x8e, 0x57, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
#endif // AMT_SUPPORT

#include "Library/MeTypeLib.h"
#include "Guid/HiiSetupEnter.h"
#include "MeAccess.h"
#include "Library/PciLib.h"
#include "IndustryStandard/Pci.h"

#include "Hecidrv.h"

/*****************************************************************************
 * Local function prototypes.
 *****************************************************************************/
#ifndef NON_BLOCKING
#define NON_BLOCKING  0
#define BLOCKING      1
#endif

/*****************************************************************************
 * Variables.
 *****************************************************************************/
//
// Global driver data
//
HECI_INSTANCE *mHeciContext = NULL;

HECI_DEVICE   mHeciDev = {HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_HIDM_MSI, HECI1_MBAR_DEFAULT};

static HECI_MSG_HEADER mHeciDxeMsgBuffer[HECI_MSG_MAXLEN / sizeof(HECI_MSG_HEADER)];
BOOLEAN  gHeciSmmExecution; //AptioV Server override: Fix for tpl assert issue with AFUDOS ME update.

/*****************************************************************************
 * Public functions.
 *****************************************************************************/
VOID
ExecuteOnSetupEnter (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  MeTypeSet();

  return;
}

#if defined(AMT_SUPPORT) && AMT_SUPPORT
EFI_STATUS
HeciInstallForME9 (
  IN EFI_HANDLE ImageHandle
  )
{
  EFI_STATUS              Status = EFI_SUCCESS;

  if (MeTypeIsAmt()) {
    VOID                    *Registration;
    UINT16                  VendorID;
    UINT16                  DeviceID;
    DXE_MBP_DATA_PROTOCOL   *MbpData;

    if (mHeciContext == NULL) {
      DEBUG ((EFI_D_ERROR, "HECI: mHeciContext is NULL in HeciInstallForME9()\n"));
      return EFI_INVALID_PARAMETER;
    }
    //
    // Install the MBP information
    //
    Status = InstallMbpDataProtocol ();
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->LocateProtocol (
                  &gMeBiosPayloadDataProtocolGuid,
                  NULL,
                  &MbpData
                  );
    if (!EFI_ERROR (Status)) {
      UINT32 MeStatus;
      UINT32 MeMode;

      HeciGetMeStatus (&MeStatus);
      HeciGetMeMode (&MeMode);

      if (MbpData->MeBiosPayload.FwCapsSku.Available == 0) {
        if ((MeMode == ME_MODE_NORMAL) &&
            ((ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_IN_RECOVERY_MODE) ||
             (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY))) {
          MEFWCAPS_SKU FwCapsSku;

          Status = HeciGetFwCapsSku (&FwCapsSku);
          if (!EFI_ERROR (Status)) {
            MbpData->MeBiosPayload.FwCapsSku.FwCapabilities.Data = FwCapsSku.Data;
          }
        }
      }

      if (MbpData->MeBiosPayload.FwPlatType.Available == 0) {
        if ((MeMode == ME_MODE_NORMAL) &&
            ((ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_IN_RECOVERY_MODE) ||
             (ME_STATUS_ME_STATE_ONLY (MeStatus) == ME_READY))) {
          PLATFORM_TYPE_RULE_DATA RuleData;

          Status = HeciGetPlatformType (&RuleData);
          if (!EFI_ERROR (Status)) {
            MbpData->MeBiosPayload.FwPlatType.RuleData.Data = RuleData.Data;
          }
        }
      }
      //
      // Dxe Mbp data is gone after ExitPmAuth, so we keep MeFwImageType for the inspection after ExitPmAuth
      //
      mHeciContext->MeFwImageType = (UINT8) MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType;
    }
    //
    // Initialize the Me Reference Code Information
    //
    mHeciContext->MeRcInfo.Revision   = ME_RC_INFO_PROTOCOL_REVISION_1;
    mHeciContext->MeRcInfo.RCVersion  = ME_RC_VERSION;

    //
    // Install the Me Reference Code Information
    //
    Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiMeRcInfoProtocolGuid,
                  &mHeciContext->MeRcInfo,
                  NULL
                  );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "HECI: Can't install gEfiMeRcInfoProtocolGuid. Not critical, continue.\n"));
    }
    //
    // ME BWG 1.0 5.3.1.1: IDER Workaround, perform this only when IDER device is present.
    //
    VendorID = PciRead16 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x00));
    DeviceID = PciRead16 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, IDER_FUNCTION_NUMBER, 0x02));
    if ((VendorID == V_ME_IDER_VENDOR_ID) && IS_PCH_LPT_IDER_DEVICE_ID(DeviceID)) {
      PerformIderWorkaround (ImageHandle);
    }
    //
    // Create an ExitPmAuth protocol call back event.
    //
    mHeciContext->EfiPmEvent = EfiCreateProtocolNotifyEvent (
                          &gExitPmAuthProtocolGuid,
                          TPL_CALLBACK,
                          MeScriptSaveEventFunction,
                          NULL,
                          &Registration
                         );
    ASSERT (mHeciContext->EfiPmEvent);
    if (!mHeciContext->EfiPmEvent) {
      return EFI_NOT_STARTED;
    }

    //
    // Create events to send EOP message
    // 1. on ExitBootServices event
    // 2. on LegacyBoot event
    // 2. on enter EfiShell using gEfiShellEnvironment2Guid
    //    /ExitBootServices is not called on EfiShell enter/
    //
    DEBUG ((EFI_D_ERROR, "HECI install ReadyToBoot handler\n"));
    Status = EfiCreateEventReadyToBootEx (
                                          TPL_CALLBACK,
                                          MeReadyToBootEventFunction,
                                          (VOID *) &ImageHandle,
                                          &mHeciContext->ReadyToBootEvent
                                         );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return Status;
}
#endif // AMT_SUPPORT

void
HeciOnErrorCleanup (
  IN EFI_STATUS   Status,
  IN EFI_HANDLE   ImageHandle
  )
{
  DEBUG ((EFI_D_ERROR, "HECI: Error occurred (%r). Starting HECI cleaning sequence.\n", Status));

  if (mHeciContext) {
    EFI_HECI_PROTOCOL *HeciCtlr;

    // gEfiHeciProtocolGuid
    Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &HeciCtlr
                  );
    if (!EFI_ERROR(Status)) {
      Status = gBS->UninstallMultipleProtocolInterfaces (
              ImageHandle,
              &gEfiHeciProtocolGuid,
              HeciCtlr,
              NULL
           );
      DEBUG ((EFI_D_INFO, "\t Removing gEfiHeciProtocolGuid (%r)\n", Status));
    }

#if defined(AMT_SUPPORT) && AMT_SUPPORT
    if (MeTypeIsAmt()) {
      DXE_MBP_DATA_PROTOCOL   *MbpData;
      EFI_ME_RC_INFO_PROTOCOL *MeRcInfo;

      // gEfiMeRcInfoProtocolGuid
      Status = gBS->LocateProtocol (
                    &gEfiMeRcInfoProtocolGuid,
                    NULL,
                    &MeRcInfo
                    );
      if (!EFI_ERROR(Status)) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                      ImageHandle,
                      &gEfiMeRcInfoProtocolGuid,
                      MeRcInfo,
                      NULL
                      );
        DEBUG ((EFI_D_INFO, "\t Removing gEfiMeRcInfoProtocolGuid (%r)\n", Status));
      }

      // gMeBiosPayloadDataProtocolGuid
      Status = gBS->LocateProtocol (
                    &gMeBiosPayloadDataProtocolGuid,
                    NULL,
                    &MbpData
                    );
      if (!EFI_ERROR(Status)) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                      MbpData->Handle,
                      &gMeBiosPayloadDataProtocolGuid,
                      MbpData,
                      NULL
                      );
        DEBUG ((EFI_D_INFO, "\t Removing gMeBiosPayloadDataProtocolGuid (%r)\n", Status));
      }

      DEBUG ((EFI_D_INFO, "\t Removing HECI devices from PCI space\n"));
      DisableAllMEDevices ();
      //
      // Store the current value of DEVEN for S3 resume path
      //
      DeviceStatusSave ();

      // Events
      if (mHeciContext->ExitBootServicesEvent) {
        gBS->CloseEvent (mHeciContext->ExitBootServicesEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event ExitBootServicesEvent\n"));
      }
      if (mHeciContext->ReadyToBootEvent) {
        gBS->CloseEvent (mHeciContext->ReadyToBootEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event ReadyToBootEvent\n"));
      }
      if (mHeciContext->EfiShellEvent) {
        gBS->CloseEvent (mHeciContext->EfiShellEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event EfiShellEvent\n"));
      }
      if (mHeciContext->EfiPmEvent) {
        gBS->CloseEvent (mHeciContext->EfiPmEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event EfiPmEvent\n"));
      }
      if (mHeciContext->LegacyBootEvent) {
        gBS->CloseEvent (mHeciContext->LegacyBootEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event LegacyBootEvent\n"));
      }
      if (mHeciContext->MpbClearEvent) {
        gBS->CloseEvent (mHeciContext->MpbClearEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event MpbClearEvent\n"));
      }
    }
#endif //AMT_SUPPORT
      if (mHeciContext->SetupEnterEvent) {
        gBS->CloseEvent (mHeciContext->SetupEnterEvent);
        DEBUG ((EFI_D_INFO, "\t Closing Event SetupEnterEvent\n"));
      }
    DEBUG ((EFI_D_INFO, "\t Removing mHeciContext allocation\n"));
    FreePool(mHeciContext);
    mHeciContext = NULL;
  }
}

EFI_STATUS
HeciDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status = EFI_OUT_OF_RESOURCES;
  void                    *pSetupRegistration;

  gHeciSmmExecution = FALSE; //AptioV Server override: Fix for tpl assert issue with AFUDOS ME update.

  mHeciContext = AllocateZeroPool (sizeof (HECI_INSTANCE));
  if (mHeciContext == NULL) {
     return Status;
  }

  //
  // Initialize HECI protocol pointers
  //
  mHeciContext->HeciCtlr.ResetHeci    = HeciResetInterface;
  mHeciContext->HeciCtlr.SendwACK     = HeciSendwACK;
  mHeciContext->HeciCtlr.ReadMsg      = HeciReceive;
  mHeciContext->HeciCtlr.SendMsg      = HeciSend;
  mHeciContext->HeciCtlr.InitHeci     = HeciInitialize;
  mHeciContext->HeciCtlr.ReInitHeci   = HeciReInitialize;
  mHeciContext->HeciCtlr.MeResetWait  = HeciMeResetWait;
  mHeciContext->HeciCtlr.GetMeStatus  = HeciGetMeStatus;
  mHeciContext->HeciCtlr.GetMeMode    = HeciGetMeMode;
  mHeciContext->HeciCtlr.ReadAndFlush = HeciReadAndFlush;

  //
  // Initialize the HECI device
  //
  Status = HeciInitialize();
  if (EFI_ERROR(Status))
  {
    HeciOnErrorCleanup(Status, ImageHandle);
    return Status;
  }

  //
  // Install the HECI interface
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
              &ImageHandle,
              &gEfiHeciProtocolGuid,
              &mHeciContext->HeciCtlr,
              NULL
           );
  if (EFI_ERROR (Status)) {
    HeciOnErrorCleanup(Status, ImageHandle);
    return Status;
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ExecuteOnSetupEnter,
                  NULL,
                  &mHeciContext->SetupEnterEvent
                  );
  if (EFI_ERROR (Status)) {
    HeciOnErrorCleanup(Status, ImageHandle);
    return Status;
  }

  Status = gBS->RegisterProtocolNotify (
                &gEfiSetupEnterGuid,
                mHeciContext->SetupEnterEvent,
                &pSetupRegistration
              );
  if (EFI_ERROR (Status)) {
    HeciOnErrorCleanup(Status, ImageHandle);
    return Status;
  }
  DEBUG ((EFI_D_ERROR, "HECI Registered on Setup Enter procedure!\n"));

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  Status = HeciInstallForME9(ImageHandle);
  if (EFI_ERROR (Status)) {
    HeciOnErrorCleanup(Status, ImageHandle);
    return Status;
  }
#endif

  return Status;
}


/**
    Function forces a reinit of the heci interface by following the reset heci interface via host algorithm
    in HPS 0.90 doc 4-17-06 njy

  Arguments:
    none

  Returns:
    EFI_STATUS

  */
EFI_STATUS EFIAPI
HeciResetInterface(VOID)
{
  UINT32 Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  return HeciQueReset(&mHeciDev, &Timeout);
}


/**
 * Sends one message through the HECI and wait for the response message.
 *
 * This function sends one message to ME through HECI circular buffer and
 * waits for one message recived from ME. It does not guarantee that the
 * received message is response to the send message.
 * 
 * @param[in,out] Message     Pointer to the message buffer
 * @param[in]     SendLength  Length in bytes of the message to send
 * @param[in,out] RecvLength  Length of the buffer for received message
 * @param[in]     HostAddress Address of the sending entity
 * @param[in]     MeAddress   Address of the ME entity receiving the message
 *
 * @returns EFI_STATUS is returned.
 */
EFI_STATUS EFIAPI
HeciSendwACK(
  IN OUT  UINT32 *Message,
  IN      UINT32  SendLength,
  IN OUT  UINT32 *RecvLength,
  IN      UINT8   HostAddress,
  IN      UINT8   MeAddress)
{
  EFI_STATUS  Status;
  UINT16      RetryCount;
  UINT32      TmpLen = 0;
  
  //
  // Send the request message
  //
  Status = HeciSend(Message, SendLength, HostAddress, MeAddress);
  if (!EFI_ERROR(Status))
  {
    //
    // Wait for ACK message
    //
    for (RetryCount = 0; RetryCount < HECI_MAX_RETRY; RetryCount++)
    {
      TmpLen = *RecvLength;
      Status = HeciReceive(BLOCKING, Message, &TmpLen);
      if (!EFI_ERROR(Status))
      {
        break;
      }
    }
    //
    // Return read length and status
    //
    *RecvLength = TmpLen;
  }
  return Status;
} // HeciSendwACK()


/**
 * Read one message from HECI, wait for it if needed.
 *
 * @param[in]     Blocking    Whether the function should wait for message
 * @param[out]    Message     Buffer for the received message
 * @param[in,out] Length      Length of the buffer/message
 *
 * @returns EFI_STATUS is returned.
 */
EFI_STATUS EFIAPI
HeciReceive (
  IN      UINT32  Blocking,
  IN OUT  UINT32 *MessageBody,
  IN OUT  UINT32 *Length)
{
  EFI_STATUS      Status;
  UINT32          i, Timeout, FrgLen, MsgLen;
  
  if (MessageBody == NULL || Length == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  Timeout = (Blocking) ? (HECI_READ_TIMEOUT / HECI_TIMEOUT_UNIT) : 0;
  MsgLen = 0;
  do
  {
    FrgLen = sizeof(mHeciDxeMsgBuffer);
    Status = HeciMsgRecv(&mHeciDev, &Timeout,
                         &mHeciDxeMsgBuffer[0], &FrgLen);
    if (EFI_ERROR(Status))
    {
      break;
    }
    else
    {
      FrgLen -= sizeof(HECI_MSG_HEADER);
      if (MsgLen + FrgLen > *Length)
      {
        FrgLen = *Length - MsgLen;
      }
      for (i = 0; i < FrgLen; i++)
      {
        ((UINT8*)MessageBody)[MsgLen++] = ((UINT8*)&mHeciDxeMsgBuffer[1])[i];
      }
    }
  }
  while (!mHeciDxeMsgBuffer[0].Bits.MsgComplete);
  
  *Length = MsgLen;
  return Status;
} // HeciReceive()


/**
 * Function sends one message (of any length) through the HECI circular buffer.
 *
 * @param[in] Message              Pointer to the message data to be sent.
 * @param[in] Length               Length of the message in bytes.
 * @param[in] HostAddress          The address of the host processor.
 * @param[in] MeAddress            Address of the ME subsystem the message is being sent to.
 *
 * @return EFI_STATUS is returned.
 */
EFI_STATUS EFIAPI
HeciSend(
  IN     UINT32 *Message,
  IN     UINT32  Length,
  IN     UINT8   HostAddress,
  IN     UINT8   MeAddress)
{
  EFI_STATUS  Status;
  UINT32      i, Retry, Timeout;
  UINT8       *pMsg = (UINT8*)Message;
  
  if (Message == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  mHeciDxeMsgBuffer[0].DWord = 0;
  mHeciDxeMsgBuffer[0].Bits.MeAddress = MeAddress;
  mHeciDxeMsgBuffer[0].Bits.HostAddress = HostAddress;
  //
  // Fragment the message into queue-sized packets and loop until completely sent
  //
  ASSERT(mHeciDev.HMtu > 0); // make sure HECI is initialized when sending
  do
  {
    if (Length <= mHeciDev.HMtu)
    {
      mHeciDxeMsgBuffer[0].Bits.Length = Length;
      mHeciDxeMsgBuffer[0].Bits.MsgComplete = 1; // Set message complete in last fragment
    }
    else
    {
      mHeciDxeMsgBuffer[0].Bits.Length = mHeciDev.HMtu;
    }
    for (i = 0; i < mHeciDxeMsgBuffer[0].Bits.Length; i++)
    {
      ((UINT8*)&mHeciDxeMsgBuffer[1])[i] = pMsg[i];
    }
    pMsg += mHeciDxeMsgBuffer[0].Bits.Length;
    Length -= mHeciDxeMsgBuffer[0].Bits.Length;
    Retry = HECI_MAX_RETRY;
    do
    {
      Timeout = HECI_SEND_TIMEOUT / HECI_TIMEOUT_UNIT;
      Status = HeciMsgSend(&mHeciDev, &Timeout, &mHeciDxeMsgBuffer[0]);
    }
    while (EFI_ERROR(Status) && Retry-- > 0);
    if (EFI_ERROR(Status))
    {
      goto GetOut;
    }
  }
  while (Length > 0);
  
GetOut:
  return Status;
} // HeciSendMsg()


EFI_STATUS EFIAPI
HeciInitialize(VOID)
{
  UINT32 Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  return HeciInit(&mHeciDev, &Timeout);
}

EFI_STATUS EFIAPI
HeciReInitialize(VOID)
{
  UINT32 Timeout = HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  return HeciInit(&mHeciDev, &Timeout);
}


EFI_STATUS EFIAPI
HeciMeResetWait(
  IN  UINT32  Delay)
{
  return HeciQueReset(&mHeciDev, &Delay);
}


/**
 * Convert MEFS1 status into BIOS specific ME status
 *
 * @param[out] MeStatus   Pointer for status report
 *
 * @retval EFI_SUCCESS             MeMode copied
 * @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
 */
EFI_STATUS EFIAPI
HeciGetMeStatus(
     OUT UINT32     *MeStatus)
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeStatus == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }

  MeFirmwareStatus.ul = HeciPciReadMefs1();

  if (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL)
  {
    *MeStatus = ME_READY;
  }
  else if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY)
  {
    *MeStatus = ME_IN_RECOVERY_MODE;
  }
  else if (MeFirmwareStatus.r.CurrentState == ME_STATE_INIT)
  {
    *MeStatus = ME_INITIALIZING;
  }
  else if (MeFirmwareStatus.r.CurrentState == ME_STATE_DISABLE_WAIT)
  {
    *MeStatus = ME_DISABLE_WAIT;
  }
  else if (MeFirmwareStatus.r.CurrentState == ME_STATE_TRANSITION)
  {
    *MeStatus = ME_TRANSITION;
  }
  else
  {
    *MeStatus = ME_NOT_READY;
  }
  if (MeFirmwareStatus.r.FwUpdateInprogress)
  {
    *MeStatus |= ME_FW_UPDATES_IN_PROGRESS;
  }
  if (MeFirmwareStatus.r.FwInitComplete)
  {
    *MeStatus |= ME_FW_INIT_COMPLETE;
  }
  if (MeFirmwareStatus.r.MeBootOptionsPresent)
  {
    *MeStatus |= ME_FW_BOOT_OPTIONS_PRESENT;
  }

  DEBUG((EFI_D_INFO, "[HECI] MEFS1 %08X -> MeStatus %d\n", MeFirmwareStatus.ul, *MeStatus));

  return EFI_SUCCESS;
}


/**
 *Convert MEFS1 status into BIOS specific ME mode
 *
 * @param[out] MeMode              Pointer for ME Mode report
 *
 * @retval EFI_SUCCESS             MeMode copied
 * @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
 */
EFI_STATUS EFIAPI
HeciGetMeMode(
     OUT UINT32     *MeMode)
{
  HECI_FWS_REGISTER MeFirmwareStatus;
  
  if (MeMode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  MeFirmwareStatus.ul = HeciPciReadMefs1();
  switch (MeFirmwareStatus.r.MeOperationMode)
  {
    case ME_OPERATION_MODE_NORMAL:
      *MeMode = ME_MODE_NORMAL;
      break;

    case ME_OPERATION_MODE_DEBUG:
      *MeMode = ME_MODE_DEBUG;
      break;

    case ME_OPERATION_MODE_SOFT_TEMP_DISABLE:
      *MeMode = ME_MODE_TEMP_DISABLED;
      break;

    case ME_OPERATION_MODE_SECOVR_JMPR:
    case ME_OPERATION_MODE_SECOVR_HECI_MSG:
      *MeMode = ME_MODE_SECOVER;
      break;

    default:
      *MeMode = ME_MODE_FAILED;
  }
  DEBUG((EFI_D_INFO, "[HECI] MEFS1 %08X -> MeMode %d\n", MeFirmwareStatus.ul, *MeMode));

  return EFI_SUCCESS;
}


/**
 * Read a message from HECI and ignore it.
 *
 * @param Blocking  Used to determine if the read is BLOCKING or NON_BLOCKING.
 *
 * @return EFI_STATUS is returned.
 */
EFI_STATUS EFIAPI
HeciReadAndFlush(
  IN      UINT32  Blocking)
{
  UINT32 Length;

  Length = sizeof(mHeciDxeMsgBuffer);
  return HeciReceive(Blocking, &mHeciDxeMsgBuffer[0].DWord, &Length);
}

