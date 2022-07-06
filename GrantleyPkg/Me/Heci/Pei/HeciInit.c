/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
 **/
/**

Copyright (c) 2008-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  HeciInit.c

@brief:

 HECI PEIM, implements HECI PPI.

**/
#include <PiPei.h>
#include <Uefi.h>
    
#include <Ppi/Heci.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PciCfg.h>
#include <Ppi/EndOfPeiPhase.h>

//#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/PciCfg.h>
#include <Ppi/EndOfPeiPhase.h>

#include <Library/BaseLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/HobLib.h>
#include <library/PcdLib.h>
#include <Library/HeciCoreLib.h>
#include <Library/MeTypeLib.h>

#include <HeciRegs.h>
#include <MeAccess.h>
#include <MeChipset.h>


/*****************************************************************************
 * Local definitions
 *****************************************************************************/
typedef struct _HECI1_PPI_EXT_
{
  PEI_HECI_PPI    Ppi;
  HECI_DEVICE     Heci;
  HECI_MSG_HEADER MsgBuf[HECI_MSG_MAXLEN / sizeof(HECI_MSG_HEADER)];
} HECI1_PPI_EXT;

typedef struct _HECI2_PPI_EXT_
{
  PEI_HECI2_PPI   Ppi;
  HECI_DEVICE     Heci;
} HECI2_PPI_EXT;


/*****************************************************************************
 * Local function prototypes.
 *****************************************************************************/
EFI_STATUS HeciSendwAck(
  IN     PEI_HECI_PPI                 *This,
  IN OUT UINT32                       *Message,
  IN     UINT32                       HeciMemBar,
  IN OUT UINT32                       *Length,
  IN     UINT8                        HostAddress,
  IN     UINT8                        MeAddress);
EFI_STATUS HeciReadMsg(
  IN     PEI_HECI_PPI                 *This,
  IN     UINT32                       Blocking,
  IN     UINT32                       HeciMemBar,
  IN     UINT32                       *MessageBody,
  IN OUT UINT32                       *Length);
EFI_STATUS HeciSendMsg(
  IN     PEI_HECI_PPI                 *This,
  IN     UINT32                       *Message,
  IN     UINT32                       HeciMemBar,
  IN     UINT32                       Length,
  IN     UINT8                        HostAddress,
  IN     UINT8                        MeAddress);
EFI_STATUS HeciInitialize(
  IN     PEI_HECI_PPI                 *This,
  IN OUT UINT32                       *HeciMemBar);
EFI_STATUS HeciGetMeStatus(
  IN     UINT32                 *MeStatus);
EFI_STATUS HeciGetMeMode(
  IN UINT32                     *MeMode);
EFI_STATUS Heci2GetMeStatus(
  IN UINT32                 *pNmFs);
EFI_STATUS Heci2Initialize(
  IN PEI_HECI2_PPI          *This,
  IN OUT  UINT32            *HeciMemBar);


/*****************************************************************************
 * Variables
 *****************************************************************************/


///////////////////////////////////////////////////////////////////////////////
// Function Implementations
//
/*****************************************************************************
 @brief
  Internal function performing Heci platform PPIs init needed in PEI phase

 @param[in] PeiServices          General purpose services available to every PEIM.

 @retval EFI_SUCCESS             The function completed successfully.
 @retval EFI_DEVICE_ERROR        ME FPT is bad
**/
EFI_STATUS
PeiInstallHeciPpi (
  IN CONST EFI_PEI_SERVICES **PeiServices)
{
  EFI_STATUS  Status;
  struct _HECI_PPI_
  {
    EFI_PEI_PPI_DESCRIPTOR Desc[2];
    HECI1_PPI_EXT          Heci1PpiExt;
    HECI2_PPI_EXT          Heci2PpiExt;
  } *pHeciPpi;
  
  //
  // Initialize Heci platform PPIs
  // Do not use EdkIIGlueLib here because PeiService pointer used in GlueLib
  // is not updated after memory installed.
  //
  pHeciPpi = AllocatePool(sizeof(*pHeciPpi));
  ASSERT(pHeciPpi);
  pHeciPpi->Desc[0].Flags = EFI_PEI_PPI_DESCRIPTOR_PPI;
  pHeciPpi->Desc[0].Guid = &gPeiHeciPpiGuid;
  pHeciPpi->Desc[0].Ppi = &pHeciPpi->Heci1PpiExt;
  pHeciPpi->Heci1PpiExt.Ppi.SendwAck = HeciSendwAck;
  pHeciPpi->Heci1PpiExt.Ppi.ReadMsg = HeciReadMsg;
  pHeciPpi->Heci1PpiExt.Ppi.SendMsg = HeciSendMsg;
  pHeciPpi->Heci1PpiExt.Ppi.InitializeHeci = HeciInitialize;
  pHeciPpi->Heci1PpiExt.Ppi.GetMeStatus = HeciGetMeStatus;
  pHeciPpi->Heci1PpiExt.Ppi.GetMeMode = HeciGetMeMode;
  pHeciPpi->Heci1PpiExt.Heci.Bus = HECI1_BUS;
  pHeciPpi->Heci1PpiExt.Heci.Dev = HECI1_DEV;
  pHeciPpi->Heci1PpiExt.Heci.Fun = HECI1_FUN;
  pHeciPpi->Heci1PpiExt.Heci.Hidm = HECI_HIDM_MSI;
  pHeciPpi->Heci1PpiExt.Heci.Mbar = HECI1_MBAR_DEFAULT;
  
  pHeciPpi->Desc[1].Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  pHeciPpi->Desc[1].Guid = &gPeiHeci2PpiGuid;
  pHeciPpi->Desc[1].Ppi = &pHeciPpi->Heci2PpiExt;
  pHeciPpi->Heci2PpiExt.Ppi.Heci2Initialize = Heci2Initialize;
  pHeciPpi->Heci2PpiExt.Ppi.Heci2GetMeStatus = Heci2GetMeStatus;
  pHeciPpi->Heci2PpiExt.Heci.Bus = HECI2_BUS;
  pHeciPpi->Heci2PpiExt.Heci.Dev = HECI2_DEV;
  pHeciPpi->Heci2PpiExt.Heci.Fun = HECI2_FUN;
  pHeciPpi->Heci2PpiExt.Heci.Hidm = HECI_HIDM_SCI;
  pHeciPpi->Heci2PpiExt.Heci.Mbar = HECI2_MBAR_DEFAULT;
  
  Status = (**PeiServices).InstallPpi(PeiServices, &pHeciPpi->Desc[0]);
  ASSERT_EFI_ERROR(Status);
  
  return Status;
}


/**
 *  Internal function performing PM register initialization for ME
 *
 * @param[in] PeiServices          General purpose services available to every PEIM.
 *
 * @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
MePmInit(
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  UINT16  PmBase;
  UINT16  Pm1Cnt;

  //
  // Before system memory initialization, BIOS should check the WAK_STS bit in PM1_STS[15] (PCH register PMBASE+00h)
  // to determine if Intel Management Engine has reset the system while the host was in a sleep state. If WAK_STS is
  // not set, BIOS should ensure a non-sleep exit path is taken. One way to accomplish this is by overwriting
  // PM1_CNT[12:10] (PCH register PMBASE+04h) to 111b to force an S5 exit path by the BIOS.
  //
  PmBase = PciRead16(PCI_LIB_ADDRESS(PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_ACPI_BASE)) & B_PCH_LPC_ACPI_BASE_BAR;

  ASSERT(PmBase != 0);

  if (PmBase != 0) {
    if ((IoRead16 (PmBase + R_ACPI_PM1_STS) & B_WAK_STS) == 0) {
      Pm1Cnt = IoRead16 (PmBase + R_ACPI_PM1_CNT) | V_S5;
      IoWrite16 (PmBase + R_ACPI_PM1_CNT, Pm1Cnt);
      DEBUG ((EFI_D_ERROR, "Force an S5 exit path.\n"));
    }
  }

  return EFI_SUCCESS;
}


/*****************************************************************************
 @bref
  The entry point of the HECI PEIM. Installs HECI PPI interface.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services. 
  
  @retval EFI_SUCCESS   Indicates that HECI initialization completed successfully.
**/
EFI_STATUS
HeciPeimEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  //
  // Handle wake status
  //
  MePmInit(PeiServices);

  //
  // Install HECI PPI
  //
  Status = PeiInstallHeciPpi(PeiServices);
  ASSERT_EFI_ERROR (Status);
  
  return Status;
} // PeimHeciInit()


/**
 * Initialize HECI-1 for use in PEI phase.
 *
 * Determines if the HECI-1 device is present and, if present, initializes it for
 * use by the BIOS.
 *
 * param[in] PeiServices     General purpose services available to every PEIM.
 * param[in] This            The address of HECI PPI
 * param[in][out] HeciMemBar HECI Memory BAR
 *
 * retval EFI_SUCCESS        The function completed successfully.
 * retval EFI_DEVICE_ERROR   No HECI device
 */
EFI_STATUS
HeciInitialize (
  IN     PEI_HECI_PPI                 *This,
  IN OUT UINT32                       *HeciMemBar)
{
  UINT32 Timeout = PEI_HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  if (This == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  return HeciInit(&((HECI1_PPI_EXT*)This)->Heci, &Timeout);
} // HeciInitialize()


/**
 * Read a message from HECI queue.
 * 
 * @param[in] PeiServices       General purpose services available to every PEIM.
 * @param[in] This              The address of HECI PPI.
 * @param[in] HeciMemBar        HECI Memory BAR
 * @param[in] Blocking          Used to determine if the read is BLOCKING or NON_BLOCKING
 * @param[in] MessageBody       Pointer to a buffer used to receive a message.
 * @param[in][out] Length       Pointer to the length of the buffer on input and the length
 *                              of the message on return. (in bytes)
 *
 * @retval EFI_SUCCESS          One message packet read
 * @retval EFI_TIMEOUT          HECI is not ready for communication
 * @retval EFI_DEVICE_ERROR     Zero-length message packet read
 * @retval EFI_BUFFER_TOO_SMALL The caller's buffer was not large enough
 */
EFI_STATUS
HeciReadMsg (
  IN     PEI_HECI_PPI           *This,
  IN     UINT32                 Blocking,
  IN     UINT32                 HeciMemBar,
  IN     UINT32                 *MessageBody,
  IN OUT UINT32                 *Length)
{
  EFI_STATUS       Status;
  UINT32           i, Timeout, MsgLen;
  UINT32           FrgLen;
  UINT8           *pFrgBdy;
  HECI_MSG_HEADER *pFrgHdr;
  
  if (This == NULL || MessageBody == NULL || Length == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  Timeout = (Blocking) ? (PEI_HECI_READ_TIMEOUT / HECI_TIMEOUT_UNIT) : 0;
  MsgLen = 0;
  pFrgHdr = &((HECI1_PPI_EXT*)This)->MsgBuf[0];
  pFrgBdy = (UINT8*)&((HECI1_PPI_EXT*)This)->MsgBuf[1];
  do
  {
    FrgLen = sizeof(((HECI1_PPI_EXT*)This)->MsgBuf);
    Status = HeciMsgRecv(&((HECI1_PPI_EXT*)This)->Heci, &Timeout, pFrgHdr, &FrgLen);
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
        ((UINT8*)MessageBody)[MsgLen++] = pFrgBdy[i];
      }
    }
  }
  while (!pFrgHdr->Bits.MsgComplete);
  
  *Length = MsgLen;
  return Status;
} // HeciReadMsg()


/**
 * Function sends one message (of any length) through the HECI circular buffer.
 *
 * @param[in] PeiServices          General purpose services available to every PEIM.
 * @param[in] This                 The address of HECI PPI.
 * @param[in] HeciMemBar           HECI Memory BAR.
 * @param[in] Message              Pointer to the message data to be sent.
 * @param[in] Length               Length of the message in bytes.
 * @param[in] HostAddress          The address of the host processor.
 * @param[in] MeAddress            Address of the ME subsystem the message is being sent to.
 *
 * @retval EFI_SUCCESS             One message packet sent.
 * @retval EFI_DEVICE_ERROR        Failed to initialize HECI
 * @retval EFI_TIMEOUT             HECI is not ready for communication
 * @exception EFI_UNSUPPORTED      Current ME mode doesn't support send message through HEC
 */
EFI_STATUS
HeciSendMsg (
  IN      PEI_HECI_PPI            *This,
  IN      UINT32                  *Message,
  IN      UINT32                   HeciMemBar,
  IN      UINT32                   Length,
  IN      UINT8                    HostAddress,
  IN      UINT8                    MeAddress)
{
  EFI_STATUS      Status;
  UINT32          i, Timeout;
  UINT32          Len = Length;
  UINT8           *pMsg = (UINT8*)Message;
  UINT8           *pFrgBdy;
  HECI_MSG_HEADER *pFrgHdr;
  
  if (This == NULL || Message == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  Timeout = PEI_HECI_SEND_TIMEOUT / HECI_TIMEOUT_UNIT;
  //
  // Fragment the message into queue-sized packets and loop until completely sent
  //
  pFrgHdr = &((HECI1_PPI_EXT*)This)->MsgBuf[0];
  pFrgBdy = (UINT8*)&((HECI1_PPI_EXT*)This)->MsgBuf[1];
  pFrgHdr->DWord = 0;
  pFrgHdr->Bits.MeAddress = MeAddress;
  pFrgHdr->Bits.HostAddress = HostAddress;
  ASSERT(((HECI1_PPI_EXT*)This)->Heci.HMtu > 0); // make sure HECI is initialized when sending
  do
  {
    if (Len <= ((HECI1_PPI_EXT*)This)->Heci.HMtu)
    {
      pFrgHdr->Bits.Length = Len;
      pFrgHdr->Bits.MsgComplete = 1; // Set msg complete in last fragment
    }
    else
    {
      pFrgHdr->Bits.Length = ((HECI1_PPI_EXT*)This)->Heci.HMtu;
    }
    for (i = 0; i < pFrgHdr->Bits.Length; i++)
    {
      pFrgBdy[i] = pMsg[i];
    }
    pMsg += pFrgHdr->Bits.Length;
    Len -= pFrgHdr->Bits.Length;
    Status = HeciMsgSend(&((HECI1_PPI_EXT*)This)->Heci, &Timeout, pFrgHdr);
    if (EFI_ERROR(Status))
    {
      goto GetOut;
    }
  }
  while (Len > 0);
  
GetOut:
  return Status;
} // HeciSendMsg()


/**
 * Send HECI message and wait for response.
 *
 */
EFI_STATUS
HeciSendwAck(
  IN      PEI_HECI_PPI                 *This,
  IN OUT  UINT32                       *Message,
  IN      UINT32                       HeciMemBar,
  IN OUT  UINT32                       *Length,
  IN      UINT8                        HostAddress,
  IN      UINT8                        MeAddress)
{
  EFI_STATUS  Status;
  
  Status = HeciSendMsg(This, Message, HeciMemBar, *Length, HostAddress, MeAddress);
  if (!EFI_ERROR(Status))
  {
    Status = HeciReadMsg(This, TRUE, HeciMemBar, Message, Length);
  }
  return Status;
} // HeciSendwAck()


/**
 * Return ME Status
 *
 * @param[in] PeiServices          General purpose services available to every PEIM.
 * @param[in] MeStatus             Pointer for status report
 *
 * @retval EFI_SUCCESS             MeStatus copied
 * @retval EFI_INVALID_PARAMETER   Pointer of MeStatus is invalid
 */
EFI_STATUS
HeciGetMeStatus(
  IN UINT32                 *MeStatus)
{
  HECI_MEFS1 MeFs1;

  if (MeStatus == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  MeFs1.DWord = HeciPciReadMefs1();
  
  switch (MeFs1.Bits.CurrentState)
  {
  case MEFS1_CURSTATE_NORMAL:
    if (MeFs1.Bits.ErrorCode == 0)
    {
      *MeStatus = ME_READY;
    }
    break;
  case MEFS1_CURSTATE_RECOVERY:
    *MeStatus = ME_IN_RECOVERY_MODE;
    break;

  case MEFS1_CURSTATE_INIT:
    *MeStatus = ME_INITIALIZING;
    break;
  default:
    *MeStatus = ME_NOT_READY;
    break;
  }
  DEBUG((EFI_D_INFO, "[HECI] MEFS1: 0x%08X -> MeStatus 0x%X\n", MeFs1.DWord, *MeStatus));
  
  return EFI_SUCCESS;
} // HeciGetMeStatus()


/**
 * Return ME Mode
 *
 * @param[in] PeiServices          General purpose services available to every PEIM.
 * @param[in] MeMode               Pointer for ME Mode report
 *
 * @retval EFI_SUCCESS             MeMode copied
 * @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
 */
EFI_STATUS
HeciGetMeMode(
  IN UINT32                     *MeMode)
{
  HECI_MEFS1 MeFs1;
  
  if (MeMode == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  MeFs1.DWord = HeciPciReadMefs1();
  switch (MeFs1.Bits.OperatingMode)
  {
  case MEFS1_OPMODE_NORMAL:
  case MEFS1_OPMODE_SPS:
    *MeMode = ME_MODE_NORMAL;
    break;

  case MEFS1_OPMODE_DEBUG:
    *MeMode = ME_MODE_DEBUG;
    break;

  case MEFS1_OPMODE_TEMP_DISABLE:
    *MeMode = ME_MODE_TEMP_DISABLED;
    break;

  case MEFS1_OPMODE_SECOVR_JMPR:
  case MEFS1_OPMODE_SECOVR_MSG:
    *MeMode = ME_MODE_SECOVER;
    break;

  default:
    *MeMode = ME_MODE_FAILED;
  }
  DEBUG((EFI_D_INFO, "[HECI] MEFS1: 0x%08X -> MeMode %d\n", MeFs1.DWord, *MeMode));
  return EFI_SUCCESS;
} // HeciGetMeMode()


/*++

  Routine Description:
    Return ME Status

  Arguments:
    PeiServices - General purpose services available to every PEIM.
    MeStatus    - Pointer for status report

  Returns:
    EFI_STATUS

--*/
EFI_STATUS EFIAPI
Heci2GetMeStatus (
  IN  UINT32                 *pNmFs)
{
  if (pNmFs == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  *pNmFs = Heci2PciRead32(HECI_R_HFS);

  DEBUG((EFI_D_ERROR, "[HECI2] NMFS: %X\n", *pNmFs));

  return EFI_SUCCESS;
}


/**
 * Determines if the HECI-2 device is present and, if present, initializes it for
 * use by the BIOS.
 *
 * @param[in] PeiServices General purpose services available to every PEIM
 * @param[in] pHeciMbar  Memory base address register used by HECI
 *
 * @retval EFI_SUCCESS    The function completed successfully.
 * @retval EFI_DEVICE_ERROR  HECI device not present.
 */
EFI_STATUS
EFIAPI
Heci2Initialize (
  IN PEI_HECI2_PPI           *This,
  IN OUT   UINT32            *pHeciMbar)
{
  UINT32 Timeout = PEI_HECI_INIT_TIMEOUT / HECI_TIMEOUT_UNIT;
  
  if (This == NULL)
  {
    return EFI_INVALID_PARAMETER;
  }
  return HeciInit(&((HECI2_PPI_EXT*)This)->Heci, &Timeout);
}

