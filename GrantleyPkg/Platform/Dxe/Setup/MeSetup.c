/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/**
@file

Copyright (c) 2011 - 2014, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  Name:
    MeSetup.c
  
  Description:
    This file contains functions that update ME related strings in BIOS setup.
  
**/

#include "SetupPlatform.h"
#include <Library/PciLib.h>
#include <Protocol/HeciProtocol.h>
#include <Sps.h>
#include <Guid/MeFwModesGuid.h>
#include "Library/MeTypeLib.h"
#include "MeChipset.h" //AptioV server override: Me configuration in setup is showing wrong values when Setup is launched from Shell. 

#include <HeciRegs.h>

#include <MeState.h>

#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include <MePlatformPolicyUpdateDxe.h>
#include "AmtSetup.h"

#ifndef MmioRead32
#define MmioRead32(x) *(VOLATILE UINT32*)(x)
#endif

BOOLEAN  gAmtResetRequests = FALSE;
extern EFI_GUID gSignalBeforeEnterSetupGuid;
#ifdef SDP_FLAG
extern MKHI_MSG_GET_FW_VERSION_RSP      FwVerRspExt; 
#endif

VOID MeDummyFunction(IN EFI_EVENT Event, IN VOID *Context) {};

/*****************************************************************************
Routine Description:
  Signal menu changes
Arguments:
Returns:
  always SUCCESS
**/
EFI_STATUS
EFIAPI
SignalChange (
  EFI_GUID gSignaledGuid
)
{
  EFI_STATUS Status;
  EFI_EVENT  Event;

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                               TPL_CALLBACK,
                               MeDummyFunction,
                               NULL,
                               &gSignaledGuid,
                               &Event
                              );
  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    gBS->SignalEvent(Event);
    gBS->CloseEvent(Event);
  }
  return EFI_SUCCESS;
}

EFI_STATUS getMeInfoSetupData(ME_INFO_SETUP_DATA *pMeInfoSetupData)
{
  EFI_STATUS Status = EFI_INVALID_PARAMETER;

  DEBUG((EFI_D_INFO, "[ME] getMeInfoSetupData() enter.\n"));

  if (pMeInfoSetupData) {
    UINTN VariableSize;

    VariableSize = sizeof(ME_INFO_SETUP_DATA);
    Status = gRT->GetVariable(
               L"MeInfoSetup",
               &gMeSetupInfoGuid,
               NULL,
               &VariableSize,
               pMeInfoSetupData);
  }

  DEBUG((EFI_D_INFO, "[ME] getMeInfoSetupData() exit (Status = %r).\n", Status));

  return Status;
}

BOOLEAN IsAfterEndOfPost (void);

EFI_STATUS
EFIAPI
MeAdvancedRouteConfig (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  CONST EFI_STRING                       Configuration,
  OUT EFI_STRING                             *Progress
)
{
  HECI_MEFS1             Mefs1;
  EFI_STATUS             Status;
  DXE_ME_POLICY_PROTOCOL *MePlatformPolicy;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_SUCCESS;
  }
#endif //SPS_SUPPORT

  Mefs1.DWord = HeciPciReadMefs1();
  switch (Mefs1.Bits.CurrentState)
  {
    case ME_CURSTATE_RECOVERY:
    case ME_CURSTATE_NORMAL:
    case ME_CURSTATE_TRANSITION:
      break;
    default:
      //
      // Do not send any HECI requests, no chance ME responds.
      //
      return EFI_SUCCESS;
  }

  // signal events to apply changes to ME if anything was changed
  // changes will be sent to ME FW
  // if we are after EndOfPost instead sending changes, request a reset
  SignalChange(gMeSetupPolicyUpdate);
  SignalChange(gMeStateChgGuid);
  SignalChange(gSetupFwUpdChgGuid);
  SignalChange(gSetupMngChgGuid);

  if (IsAfterEndOfPost()) {
    gAmtResetRequests = TRUE;
  } else {
    gAmtResetRequests = FALSE;
    Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &MePlatformPolicy);
    if (EFI_ERROR(Status)) {
      // in case no gDxePlatformMePolicyGuid, request reset
      // any changes will be applied after reset
      gAmtResetRequests = TRUE;
    } else {
      if (MePlatformPolicy->AnythingChanged) {
        // There are changes after EndOfPost - request reset
        gAmtResetRequests = TRUE;
      }
    }
  }

  // call IfrUpdChg,
  // if anything changed /detected other notification handlers/
  // update strings
  SignalChange(gMeSetupInfoGuid);

  return EFI_SUCCESS;
}

#endif // AMT_SUPPORT

/*****************************************************************************
  This function updates ME related strings in BIOS setup database.

  Some of the strings contain dummy values to be update with run-time date.
  These strings are updated by this function before any setup screen is
  printed.
  
  @param[in] HiiHandle - BIOS setup datebase handle
  @param[in] Class     - unused
  @return Void.
**/
VOID InitMeSetupStrings(void)
{
  EFI_STATUS             Status;
  EFI_HECI_PROTOCOL      *pHeciProtocol;
  SPS_MEFS1              Mefs1;
  SPS_MEFS2              Mefs2;
#if defined(SPS_SUPPORT) && SPS_SUPPORT  //AptioV server override: To fix build error when AMT_SUPPORT is enabled
  
  SPS_FEATURE_SET        FeatureSet;
#endif  //AptioV server override: To fix build error when AMT_SUPPORT is enabled
  UINT32                 RspLen;
  CHAR16                 *pStrBuf;
  UINTN                  StrSize;
  union
  {
    MKHI_MSG_GET_FW_VERSION_REQ      FwVerReq;
    MKHI_MSG_GET_FW_VERSION_RSP      FwVerRsp;
    SPS_MSG_GET_MEBIOS_INTERFACE_REQ MeBiosIfReq;
    SPS_MSG_GET_MEBIOS_INTERFACE_RSP MeBiosIfRsp;
  } HeciMsg;

  //AptioV server override start: Me configuration in setup is showing wrong values when Setup is launched from Shell. 
  BOOLEAN               HideMe = FALSE;
  
  if ( B_PCH_RCRB_FUNC_DIS2_MEF0D & MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_FUNC_DIS2) ) {
      MmioAnd32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_FUNC_DIS2, (UINT32)~B_PCH_RCRB_FUNC_DIS2_MEF0D);
      HideMe = TRUE;
  }
  //AptioV server override End: Me configuration in setup is showing wrong values when Setup is launched from Shell. 

  StrSize = 0x200;
  pStrBuf = AllocateZeroPool(StrSize);
  
  Mefs1.DWord = HeciPciReadMefs1();
  Mefs2.DWord = HeciPciReadMefs2();
  DEBUG((EFI_D_INFO, "[ME] MEFS1: 0x%08X, MEFS2: 0x%08X\n", Mefs1.DWord, Mefs2.DWord));
  
  UnicodeSPrint(pStrBuf, StrSize, L"0x%08X", Mefs1.DWord);
  HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWSTS1_VALUE), pStrBuf, NULL);  
    
  UnicodeSPrint(pStrBuf, StrSize, L"0x%08X", Mefs2.DWord);
  HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWSTS2_VALUE), pStrBuf, NULL);  
  //
  // Update ME Current State in setup strings database
  //
  switch (Mefs1.Bits.CurrentState)
  {
    case MEFS1_CURSTATE_RESET:
      UnicodeSPrint(pStrBuf, StrSize, L"Reset");
      break;
    case MEFS1_CURSTATE_INIT:
      UnicodeSPrint(pStrBuf, StrSize, L"Initializing");
      break;
    case MEFS1_CURSTATE_RECOVERY:
      UnicodeSPrint(pStrBuf, StrSize, L"Recovery");
      break;
    case MEFS1_CURSTATE_DISABLED:
      UnicodeSPrint(pStrBuf, StrSize, L"Disabled");
      break;
    case MEFS1_CURSTATE_NORMAL:
      UnicodeSPrint(pStrBuf, StrSize, L"Operational");
      break;
    case MEFS1_CURSTATE_TRANSITION:
      UnicodeSPrint(pStrBuf, StrSize, L"Transitioning");
      break;
    default:
      UnicodeSPrint(pStrBuf, StrSize, L"Unknown(%d)", Mefs1.Bits.CurrentState);
      break;
  }
  HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWCURSTE_VALUE), pStrBuf, NULL);  
  //
  // Update ME Error Code in setup strings database
  //
  switch (Mefs1.Bits.ErrorCode)
  {
    case MEFS1_ERRCODE_NOERROR:
      UnicodeSPrint(pStrBuf, StrSize, L"No Error");
      break;
    case MEFS1_ERRCODE_UNKNOWN:
      UnicodeSPrint(pStrBuf, StrSize, L"Uncategorized Failure");
      break;
    case MEFS1_ERRCODE_DISABLED:
      UnicodeSPrint(pStrBuf, StrSize, L"Disabled");
      break;
    case MEFS1_ERRCODE_IMAGEFAIL:
      UnicodeSPrint(pStrBuf, StrSize, L"Image Failure");
      break;
    default:
      UnicodeSPrint(pStrBuf, StrSize, L"Unknown(%d)", Mefs1.Bits.ErrorCode);
      break;
  }
  HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWERRCODE_VALUE), pStrBuf, NULL);
  
  switch (Mefs1.Bits.CurrentState)
  {
    case MEFS1_CURSTATE_RECOVERY:
    case MEFS1_CURSTATE_NORMAL:
    case MEFS1_CURSTATE_TRANSITION:
      break;
    default:
      //
      // Do not send any HECI requests, no chance ME responds.
      //
      //AptioV server override start: Me configuration in setup is showing wrong values when Setup is launched from Shell. 
      goto END;
      //return;
      //AptioV server override End: Me configuration in setup is showing wrong values when Setup is launched from Shell. 
  }
  //
  // Update the ME firmware version info in BIOS Setup strings database
  //
  Status = gBS->LocateProtocol(&gEfiHeciProtocolGuid, NULL, &pHeciProtocol);
  if (!EFI_ERROR(Status)) {
    UINT32 MeMode = ME_MODE_TEMP_DISABLED;

    Status = pHeciProtocol->GetMeMode (&MeMode);
    if (MeMode == ME_MODE_TEMP_DISABLED) {
      DEBUG((EFI_D_INFO, "[ME] ME is in TEMP DISABLED MODE (don't send MKHI_GET_FW_VERSION message).\n"));
      Status = EFI_UNSUPPORTED;
    }
  }
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[ME] ERROR: Cannot locate HECI protocol (status: %r)\n",
                        Status));
  }
  else
  { //
    // Send MKHI_GET_FW_VERSION request to ME
    //
    
    DEBUG((EFI_D_INFO, "[ME] Sending MKHI_GET_FW_VERSION request to ME\n"));
    HeciMsg.FwVerReq.Mkhi.Data = 0x000002FF;
    RspLen = sizeof(HeciMsg.FwVerRsp);
    Status = pHeciProtocol->SendwACK((UINT32*)&HeciMsg.FwVerReq, sizeof(HeciMsg.FwVerReq),
                                     &RspLen, SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
    if (EFI_ERROR(Status))
    {
      DEBUG((EFI_D_ERROR, "[ME] ERROR: Cannot send MKHI_GET_FW_VERSION (status: %r)\n",
                          Status));
    }
    else if (HeciMsg.FwVerRsp.Mkhi.Data != 0x000082FF)
    {
      DEBUG((EFI_D_ERROR, "[ME] ERROR: Invalid MKHI_GET_FW_VERSION response (MKHI: 0x%X)\n",
                          HeciMsg.FwVerRsp.Mkhi.Data));
    }
    else
    {
      CHAR16 *MeTypeString = HiiGetString(mAdvancedHiiHandle, STR_ME_FW_TYPE_UNKNOWN, NULL);
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#if defined(SPS_SUPPORT) && SPS_SUPPORT
      if (MeTypeIsAmt())
#endif // SPS_SUPPORT
      {
        ME_INFO_SETUP_DATA mMeInfoSetupData;

        if (getMeInfoSetupData(&mMeInfoSetupData) == EFI_SUCCESS)
        {
          CHAR16               *NewStringToken;
          static EFI_STRING_ID MeMode[] = {
                 STRING_TOKEN(STR_ME_NORMAL_MODE),
                 STRING_TOKEN(STR_ME_FAILED),
                 STRING_TOKEN(STR_ME_ALT_DISABLED),
                 STRING_TOKEN(STR_ME_TEMP_DISABLED),
                 STRING_TOKEN(STR_ME_SECOVER),
                 STRING_TOKEN(STR_ME_FAILED),
                 STRING_TOKEN(STR_ME_FAILED),
              };

          DEBUG((EFI_D_INFO, "[ME] ME filesystem corruption value = %x\n", mMeInfoSetupData.MeFileSystem));
          UnicodeSPrint(pStrBuf, StrSize, L"%X", mMeInfoSetupData.MeFileSystem);
          HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FILE_SYSTEM_CORRUPTION_VALUE), pStrBuf, NULL);
          // Set ME FW SKU
          switch (mMeInfoSetupData.MeImageType) {
            case ME_IMAGE_1_5MB_FW:
              UnicodeSPrint(pStrBuf, StrSize, L"%a", STR_ME_FW_SKU_1_5MB);
            break;
            case ME_IMAGE_5MB_FW:
              UnicodeSPrint(pStrBuf, StrSize, L"%a", STR_ME_FW_SKU_5MB);
            break;
            default:
              UnicodeSPrint(pStrBuf, StrSize, L"%a", STR_ME_FW_SKU_UNIDENTIFIED);
          }
          HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FW_SKU_VALUE), pStrBuf, NULL);
          // ME FW Info
          NewStringToken = HiiGetString(mAdvancedHiiHandle, MeMode[(mMeInfoSetupData.MeFirmwareInfo & ME_MODE_MASK)], NULL);
          if (NewStringToken) {
            HiiSetString(mAdvancedHiiHandle,STRING_TOKEN(STR_ME_FW_INFO_VALUE), NewStringToken, NULL); 
            FreePool (NewStringToken);
          }
        }
        // Set AMT ME Type
        if (MeTypeString) {
          FreePool (MeTypeString);
        }
        MeTypeString = HiiGetString(mAdvancedHiiHandle, STR_ME_FW_TYPE_AMT, NULL);

#if TESTMENU_FLAG || ME_TESTMENU_FLAG
        // update default settings for ME9+
        if (GetBootModeHob() == BOOT_WITH_DEFAULT_SETTINGS) {
          SYSTEM_CONFIGURATION mSystemDefaults;
          UINTN                BufferSize;

          Status  = gRT->GetVariable (
                      L"SetupDefault",
                      &gEfiSetupVariableGuid,
                      NULL,
                      &BufferSize,
                      &mSystemDefaults
                    );
          if (!EFI_ERROR(Status)) {
            // disable HECI2
            mSystemDefaults.MeHeci2Enabled = 0;

            // Save changed defaults
            Status  = gRT->SetVariable (
                      L"SetupDefault",
                      &gEfiSetupVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof(SYSTEM_CONFIGURATION),
                      &mSystemDefaults
                    );
            if (EFI_ERROR(Status)) {
              DEBUG((EFI_D_ERROR, "[ME] Can't update ME9+ defaults (%r).\n", Status));
            }
          } else {
            DEBUG((EFI_D_ERROR, "[ME] Can't open ME9+ defaults (%r).\n", Status));
          }
        }
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG
      }
#endif // AMT_SUPPORT
#if defined(SPS_SUPPORT) && SPS_SUPPORT
#if defined(AMT_SUPPORT) && AMT_SUPPORT
      if (MeTypeIsSps())
#endif // AMT_SUPPORT
      {
        // Set SPS ME Type
        if (MeTypeString) {
          FreePool (MeTypeString);
        }
        MeTypeString = HiiGetString(mAdvancedHiiHandle, STR_ME_FW_TYPE_SPS, NULL);
      }
#endif // SPS_SUPPORT
      // Update Hii with ME Type
      if (MeTypeString) {
        HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FW_TYPE_VALUE), MeTypeString, NULL);
        FreePool (MeTypeString);
      }
#ifdef SDP_FLAG	  
      FwVerRspExt =  HeciMsg.FwVerRsp;  
#endif
//AptioV server override start: As per ME SPS for Grantley, the “major” FW version will be interpreted as Bits [7:0] version number
      DEBUG((EFI_D_INFO, "[ME] Firmware version is: \n"
                         "             Operational: %02X:%d.%d.%d.%d\n"
                         "                Recovery: %02X:%d.%d.%d.%d\n",
                         HeciMsg.FwVerRsp.Act.ProdId,
                         (HeciMsg.FwVerRsp.Act.Major & 0xFF),
                         HeciMsg.FwVerRsp.Act.Minor,
                         HeciMsg.FwVerRsp.Act.Patch,
                         HeciMsg.FwVerRsp.Act.Build,
                         HeciMsg.FwVerRsp.Rcv.ProdId,
                         (HeciMsg.FwVerRsp.Rcv.Major & 0xFF),
                         HeciMsg.FwVerRsp.Rcv.Minor,
                         HeciMsg.FwVerRsp.Rcv.Patch,
                         HeciMsg.FwVerRsp.Rcv.Build));
#ifdef SDP_FLAG
      UnicodeSPrint(pStrBuf, StrSize, L"v0%d.0%d.0%d.00%d.0",
                    (HeciMsg.FwVerRsp.Act.Major & 0xFF),
                    HeciMsg.FwVerRsp.Act.Minor,
                    HeciMsg.FwVerRsp.Act.Patch,
                    HeciMsg.FwVerRsp.Act.Build);
      HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWVER_OPERATIONAL_VALUE),pStrBuf,NULL);
#else
      UnicodeSPrint(pStrBuf, StrSize, L"%02X:%d.%d.%d.%d",
                    HeciMsg.FwVerRsp.Act.ProdId,
                    (HeciMsg.FwVerRsp.Act.Major & 0xFF),
                    HeciMsg.FwVerRsp.Act.Minor,
                    HeciMsg.FwVerRsp.Act.Patch,
                    HeciMsg.FwVerRsp.Act.Build);
      HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWVER_OPERATIONAL_VALUE),pStrBuf,NULL);
#endif
      //
      // Update ME firmware info in platform information menu
      //
      //HiiSetString(mMainHiiHandle, STRING_TOKEN(STR_ME_REVISION_VALUE), pStrBuf, NULL); AptioV server override: Form not provided in AptioV

      UnicodeSPrint(pStrBuf, StrSize, L"%02X:%d.%d.%d.%d",
                    HeciMsg.FwVerRsp.Rcv.ProdId,
                    (HeciMsg.FwVerRsp.Rcv.Major & 0xFF),
                    HeciMsg.FwVerRsp.Rcv.Minor,
                    HeciMsg.FwVerRsp.Rcv.Patch,
                    HeciMsg.FwVerRsp.Rcv.Build);
      HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWVER_RECOVERY_VALUE), pStrBuf, NULL);
      //
      // ME in dual-image configuration provides the version of the backup image yet
      //
      if (RspLen > sizeof(HeciMsg.FwVerRsp) - sizeof(HeciMsg.FwVerRsp.Bkp))
      {
        DEBUG((EFI_D_INFO, "                  Backup: %02X:%d.%d.%d.%d\n",
                           HeciMsg.FwVerRsp.Bkp.ProdId,
                           (HeciMsg.FwVerRsp.Bkp.Major & 0xFF),
                           HeciMsg.FwVerRsp.Bkp.Minor,
                           HeciMsg.FwVerRsp.Bkp.Patch,
                           HeciMsg.FwVerRsp.Bkp.Build));
        UnicodeSPrint(pStrBuf, StrSize, L"%02X:%d.%d.%d.%d",
                      HeciMsg.FwVerRsp.Bkp.ProdId,
                      (HeciMsg.FwVerRsp.Bkp.Major & 0xFF),
                      HeciMsg.FwVerRsp.Bkp.Minor,
                      HeciMsg.FwVerRsp.Bkp.Patch,
                      HeciMsg.FwVerRsp.Bkp.Build);
//AptioV server override end: As per ME SPS for Grantley, the “major” FW version will be interpreted as Bits [7:0] version number        
        HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWVER_BACKUP_VALUE), pStrBuf, NULL);
      }
    }
#if defined(SPS_SUPPORT) && SPS_SUPPORT
#if defined(AMT_SUPPORT) && AMT_SUPPORT
    if (MeTypeIsSps())
    {  
#endif // AMT_SUPPORT
      HeciMsg.MeBiosIfReq.Command.Data = SPS_CMD_GET_MEBIOS_INTERFACE_REQ;
      RspLen = sizeof(HeciMsg.MeBiosIfRsp);
      Status = pHeciProtocol->SendwACK((UINT32*)&HeciMsg.MeBiosIfReq, sizeof(HeciMsg.MeBiosIfReq),
                                       &RspLen, SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_SPS);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[ME] ERROR: Cannot send SPS_GET_MEBIOS_INTERFACE (status: %r)\n",
                            Status));
      }
      else if (RspLen < sizeof(HeciMsg.MeBiosIfRsp))
      {
        DEBUG((EFI_D_ERROR, "[ME] ERROR: Invalid SPS_GET_MEBIOS_INTERFACE response (length %d)\n",
                            RspLen));
      }
      else
      { //
        // Fill the ME firmware SKU info in setup
        //
        FeatureSet.Data = HeciMsg.MeBiosIfRsp.FeatureSet;
        UnicodeSPrint(pStrBuf, StrSize, L"SiEn%s%s%s%s%s%s%s%s",
                      FeatureSet.Bits.NodeManager ? L"+NM" : L"",
                      FeatureSet.Bits.PeciProxy ?   L"+PECIProxy" : L"",
                      FeatureSet.Bits.ICC ?         L"+ICC" : L"",
                      FeatureSet.Bits.DCMI ?        L"+DCMI" : L"",
                      FeatureSet.Bits.IDER ?        L"+IDEr" : L"",
                      FeatureSet.Bits.SOL ?         L"+SOL" : L"",
                      FeatureSet.Bits.PmBusProxy ?  L"+PMBusProxy" : L"",
                      FeatureSet.Bits.MicProxy ?    L"+MICProxy" : L"",
                      FeatureSet.Bits.MctpProxy ?   L"+MCTPProxy" : L"",
                      FeatureSet.Bits.TRVA ?        L"+TRVA" : L"");
        HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FWSKU_VALUE), pStrBuf, NULL);
        HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ME_FW_SKU_VALUE), pStrBuf, NULL);
      }
#if defined(AMT_SUPPORT) && AMT_SUPPORT
    }
#endif // AMT_SUPPORT
#endif // SPS_SUPPORT
  } // if (gBS->LocateProtocol(..HECI..)
  FreePool(pStrBuf);

  //AptioV server override start: Me configuration in setup is showing wrong values when Setup is launched from Shell. 
END:
    if ( HideMe == TRUE ) {
        MmioOr32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_FUNC_DIS2, (UINT32)B_PCH_RCRB_FUNC_DIS2_MEF0D);
    }
    //AptioV server override End: Me configuration in setup is showing wrong values when Setup is launched from Shell. 

  return;
} // InitMeSetupStrings()

