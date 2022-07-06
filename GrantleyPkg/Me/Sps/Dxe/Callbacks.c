/*++

Copyright (c) 1996 - 2021, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  CallBacks.c

Abstract:
   This file contains callbacks of events that SPS is interested in.
   
--*/
#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>

#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/MePolicyLib.h>

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>

#include <Guid/HobList.h>
#include <Guid/EventLegacyBios.h>
// AptioV server override start: Commented to be included as the last include file to avoid redefintion error when including token.h
//#include <Guid/SpsInfoHobGuid.h>
//#include <Guid/MeFwModesGuid.h>
// AptioV server override end: Commented to be included as the last include file to avoid redefintion error when including token.h

#include <Protocol/MpService.h>
#include <Protocol/ExitPmAuth.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>

#include <Protocol/CpuIo.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/PciIo.h>
#include <Protocol/SpsSiEn.h>
#include <Protocol/SpsNm.h>

#include <Sps.h>
#include <Platform.h>
#include <PchAccess.h>
#include <PchRegs/PchRegsLpc.h>
#include <PchRegs/PchRegsRcrb.h>
#include <SpsMisc.h>
#include <HeciRegs.h>
#include <MeAccess.h>

// AptioV server override start: included here to avoid redefintion errors when including token.h and defining ME_SKU before including Callbacks.h
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>
#include "SpsDxe.h"
// AptioV server override end: included here to avoid redefintion errors when including token.h and defining ME_SKU before including Callbacks.h



/******************************************************************************
 * Functions
 */
/**

  @brief
  This message is sent by the BIOS when it wants to notify that the BIOS is starting
  to run 3rd party code.

  @exception EFI_UNSUPPORTED      Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
HeciCoreBiosDoneMsg (
  VOID
  )
{
  EFI_STATUS           Status;
  UINT32               Length;
  UINT32               RespLength;
  CORE_BIOS_DONE       CoreBiosDone;
  UINT32               MeMode;
  EFI_HECI_PROTOCOL    *Heci;

  DEBUG ((DEBUG_INFO, "[ME] %a.\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  (VOID**) &Heci
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] Unable to Locate Heci Protocol.- %r\n", Status));
    return Status;
  }

  Length = sizeof (CORE_BIOS_DONE);
  RespLength = sizeof (CORE_BIOS_DONE_ACK);

  Status = Heci->GetMeMode (&MeMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] HeciCoreBiosDoneMsg: unsupported! (status: %r - ME mode: %d)\n", Status, MeMode));
    return EFI_UNSUPPORTED;
  }

  CoreBiosDone.MkhiHeader.Data              = 0;
  CoreBiosDone.MkhiHeader.Fields.GroupId    = MKHI_MCA_GROUP_ID;
  CoreBiosDone.MkhiHeader.Fields.Command    = MCA_CORE_BIOS_DONE_CMD;
  CoreBiosDone.MkhiHeader.Fields.IsResponse = 0;

  Status = Heci->SendwACK (
                  (UINT32*) &CoreBiosDone,
                  Length,
                  &RespLength,
                  BIOS_FIXED_HOST_ADDR,
                  HECI_CORE_MESSAGE_ADDR
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] Core Bios Done Message: message failed! - %r\n", Status));
  } else if ((CoreBiosDone.MkhiHeader.Fields.Command != MCA_CORE_BIOS_DONE_CMD) ||
      (CoreBiosDone.MkhiHeader.Fields.GroupId != MKHI_MCA_GROUP_ID) ||
      (CoreBiosDone.MkhiHeader.Fields.IsResponse != 1) ||
      (CoreBiosDone.MkhiHeader.Fields.Result != 0)
      ) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: Invalid response received for CORE_BIOS_DONE (MKHI: %08X)\n", (CoreBiosDone.MkhiHeader.Data)));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "[ME] %a: exit (%r)\n", __FUNCTION__, Status));

  return Status;
}


/*++
  Routine Description:
    SPS callback for ExitPmAuth event.
    Performs actions required by SPS ME-BIOS Spec before OS is loaded.
  Arguments:
    Event    - the event, not used
    pContext - pointer to SPS firmware description
  Returns:
    Void.
--*/
VOID SpsExitPmAuthCallback(
  IN  EFI_EVENT     Event,
  IN  VOID          *pContext)
{
  EFI_STATUS        Status;
  UINT32            Data32[2];
  UINT8             Data8;
  UINT32            RspLen;
  EFI_HECI_PROTOCOL *pHeci;
  SPS_DXE_CONTEXT   *pSpsContext = (SPS_DXE_CONTEXT*)pContext;
  
  union
  {
    MKHI_MSG_HMRFPO_LOCK_REQ   HmrfpoLockReq;
    MKHI_MSG_HMRFPO_LOCK_RSP   HmrfpoLockRsp;
    MKHI_MSG_HMRFPO_ENABLE_REQ HmrfpoEnableReq;
    MKHI_MSG_HMRFPO_ENABLE_RSP HmrfpoEnableRsp;
    MKHI_MSG_ISVT_ENABLE_REQ   IsvtEnableReq;
    MKHI_MSG_ISVT_ENABLE_RSP   IsvtEnableRsp;
    MCTP_SET_BUS_OWNER_REQ     MctpSetBusOwnerReq;
    MCTP_SET_BUS_OWNER_RSP     MctpSetBusOwnerRsp;
  } HeciMsg;
  
  //
  // Check whether this is real ExitPmAuth notification, or just a SignalEvent
  //
  Status = gBS->LocateProtocol(&gExitPmAuthProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR(Status))
  {
    return;
  }

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol(&gEfiHeciProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
  }
  else
  {
    //
    //Send MCPT Bus Owner Proxy configuration if set
    //
    if (pSpsContext->SysCfg.SpsMctpBusOwner != 0 &&
        pSpsContext->SpsMode.FeatureSet.Bits.MctpProxy)
    {
      DEBUG((EFI_D_INFO, "[SPS] Sending MCTP_SET_BUS_OWNER with address 0x%X\n",
             pSpsContext->SysCfg.SpsMctpBusOwner));
      HeciMsg.MctpSetBusOwnerReq.Command = MCPT_CMD_SET_BUS_OWNER_REQ;
      HeciMsg.MctpSetBusOwnerReq.Reserved0[0] =
      HeciMsg.MctpSetBusOwnerReq.Reserved0[1] =
      HeciMsg.MctpSetBusOwnerReq.Reserved0[2] = 0;
      HeciMsg.MctpSetBusOwnerReq.PCIeAddress = pSpsContext->SysCfg.SpsMctpBusOwner;
      HeciMsg.MctpSetBusOwnerReq.Location = 0;
      HeciMsg.MctpSetBusOwnerReq.Reserved1 = 0;
      RspLen = sizeof(HeciMsg);
      Status = pHeci->SendwACK((UINT32*)&HeciMsg.MctpSetBusOwnerReq,
                               sizeof(HeciMsg.MctpSetBusOwnerReq), &RspLen,
                               SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MCTP);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send MCTP_SET_BUS_OWNER (%r)\n", Status));
      }
      else if (HeciMsg.MctpSetBusOwnerRsp.Result != MCPT_CMD_SET_BUS_OWNER_RSP_SUCCESS)
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: MCTP_SET_BUS_OWNER failure (cause: %d)\n",
               HeciMsg.MctpSetBusOwnerRsp.Result));
      }
    }
    //
    // Send PCH DEBUG CAPABILITIES enable message if it is set by ME debug option
    //
    if (pSpsContext->SysCfg.MeSiliconViewTechEnabled)
    {
      //
      // BIOS utilizes the PCH DEBUG CAPABILITIES ENABLE message to turn on ISVT (Intel Silicon View) 
      // capabilities when the system is in a post-manufactured state. The PCH_DEBUG_CAPABILITIES  
      // ENABLE message is not necessary when using pre-production silicon or if the system has not 
      // completed manufacturing (SPI descriptor region unlocked). The ME firmware will automatically  
      // disable all PCH debug capabilities if PCH DEBUG CAPABILITY ENABLE MEI message is not received  
      // before receiving END_OF_POST. In the case BIOS wants to keep the interface enabled, the BIOS  
      // should send PCH DEBUG CAPABILITIES ENABLE message during every boot.
      //
      DEBUG((EFI_D_INFO, "[SPS] Sending PCH_DEBUG_CAPABILITIES to ME\n"));
      HeciMsg.IsvtEnableReq.Mkhi.Data = 0x00000908;
      HeciMsg.IsvtEnableReq.EnableMask = ENABLE_MASK_PCH_SVT;
      RspLen = sizeof(HeciMsg.IsvtEnableRsp);
      Status = pHeci->SendwACK((UINT32*)&HeciMsg.IsvtEnableReq,
                               sizeof(HeciMsg.IsvtEnableReq), &RspLen,
                               SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send PCH_DEBUG_CAPABILITIES (%r)\n", Status));
      }
      else if (HeciMsg.IsvtEnableRsp.Mkhi.Data != 0x00008908)
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Invalid PCH_DEBUG_CAPABILITIES response (MKHI: 0x%X)\n",
                            HeciMsg.IsvtEnableRsp.Mkhi.Data));
      }
      else if (HeciMsg.IsvtEnableRsp.Result != PCH_SVT_RSP_SUCCESS)
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: PCH_DEBUG_CAPABILITIES enable failure (cause: %d)\n",
                            HeciMsg.IsvtEnableRsp.Result));
      }
    } // if (SystemCfgData.MeSiliconViewTechEnabled)

    //
    // Sending notification that BIOS is starting to run 3rd party code.
    //
    if (pSpsContext->SysCfg.CoreBiosDoneEnabled) {
      HeciCoreBiosDoneMsg ();
    }
  } // if (HECI protocol located)
  //
  // Now prepare value for RCBA FD2 register that will disable ME devices according to
  // SPS ME-BIOS chapter "3.2 ME PCI Functions Hiding and Disabling".
  // The register will be written at END_OF_POST, becasue HECI-1 is still needed, but value
  // must be prepared now to save in S3 boot script. S3 script cannot be writen later.
  //
  pSpsContext->RcbaFd2 = MmioRead32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2);
  pSpsContext->RcbaFd2 &= ~(B_PCH_RCRB_FD2_KTD | B_PCH_RCRB_FD2_IRERD |
                            B_PCH_RCRB_FD2_MEI2D | B_PCH_RCRB_FD2_MEI1D);
  //
  // ME function 3 - KT
  // Disable if required in setup or if not supported by ME.
  // Register FD2 bit 4 disables ME function 4
  //
  if (pSpsContext->SysCfg.MeKtEnabled == 0 ||
      pSpsContext->SysCfg.MeKtEnabled == 2 && !pSpsContext->SpsMode.FeatureSet.Bits.SOL)
  {
    pSpsContext->RcbaFd2 |= B_PCH_RCRB_FD2_KTD;
  }
  //
  // ME function 2 - IDEr
  // Disable if required in setup or if not supported by ME.
  // Register FD2 bit 3 disables ME function 3
  //
  if (pSpsContext->SysCfg.MeIderEnabled == 0 ||
      pSpsContext->SysCfg.MeIderEnabled == 2 && !pSpsContext->SpsMode.FeatureSet.Bits.IDER)
  {
    pSpsContext->RcbaFd2 |= B_PCH_RCRB_FD2_IRERD;
  }
  //
  // ME function 1 - HECI-2
  // Disable if required in setup or if neither DM nor NM is enabled.
  // Register FD2 bit 2 disables ME function 1
  //
  if (pSpsContext->SysCfg.MeHeci2Enabled == 0 ||
      pSpsContext->SysCfg.MeHeci2Enabled == 2 &&
                                              !pSpsContext->SpsMode.FeatureSet.Bits.NodeManager &&
                                              !pSpsContext->SpsMode.FeatureSet.Bits.DCMI)
  {
    pSpsContext->RcbaFd2 |= B_PCH_RCRB_FD2_MEI2D;
  }
  //
  // ME function 0 - HECI-1
  // Disable if required in setup or if neither DM nor NM is enabled.
  // Register FD2 bit 1 disables ME function 0
  //
  if (pSpsContext->SysCfg.MeHeci1Enabled == 0 ||
      pSpsContext->SysCfg.MeHeci1Enabled == 2 &&
                                              !pSpsContext->SpsMode.FeatureSet.Bits.NodeManager &&
                                              !pSpsContext->SpsMode.FeatureSet.Bits.DCMI)
  {
    pSpsContext->RcbaFd2 |= B_PCH_RCRB_FD2_MEI1D;
  }
  //
  // Update S3 resume script
  // S3BootScript library is using specific form of PCI address, let's define macro for it.
  //
#define S3BOOTSCRIPT_PCIADDR(Bus, Dev, Fun, Reg) \
          (UINT64)(((Bus) & 0xFF) << 24 | ((Dev) & 0x1F) << 16 | ((Fun) & 0x07) << 8 | ((Reg) & 0xFF ))
  //
  // If HECI-1 (D22 F0) is SMI or SCI mode there is no regular OS driver and
  // we must save it's configuration.
  //
  Data8 = HeciPciRead8(HECI_R_HIDM);
  if (Data8 != HECI_HIDM_MSI)
  {
    DEBUG((EFI_D_INFO, "[SPS] Saving HECI-1 configuration to S3 boot script\n"));
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_HIDM),
                                1, &Data8);
    Data32[0] = HeciPciRead32(HECI_R_MBAR);
    Data32[1] = HeciPciRead32(HECI_R_MBAR + 4);
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint32,
                                S3BOOTSCRIPT_PCIADDR(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_MBAR),
                                2, &Data32[0]);
    Data8 = HeciPciRead8(HECI_R_IRQ);
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_IRQ),
                                1, &Data8);
    Data8 = HeciPciRead8(HECI_R_COMMAND);
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_COMMAND),
                                1, &Data8);
  }
  //
  // If HECI-2 (D22 F1) is SMI or SCI mode there is no regular OS driver and
  // we must save it's configuration.
  //
  Data8 = Heci2PciRead8(HECI_R_HIDM);
  if (Data8 != HECI_HIDM_MSI)
  {
    DEBUG((EFI_D_INFO, "[SPS] Saving HECI-2 configuration to S3 boot script\n"));
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI2_BUS, HECI2_DEV, HECI2_FUN, HECI_R_HIDM),
                                1, &Data8);
    Data32[0] = Heci2PciRead32(HECI_R_MBAR);
    Data32[1] = Heci2PciRead32(HECI_R_MBAR + 4);
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint32,
                                S3BOOTSCRIPT_PCIADDR(HECI2_BUS, HECI2_DEV, HECI2_FUN, HECI_R_MBAR),
                                2, &Data32[0]);
    Data8 = Heci2PciRead8(HECI_R_IRQ);
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI2_BUS, HECI2_DEV, HECI2_FUN, HECI_R_IRQ),
                                1, &Data8);
    Data8 = Heci2PciRead8(HECI_R_COMMAND);
    if (!(Data8 & HECI_CMD_MSE) && (Data32[0] != 0 || Data32[1] != 0))
    {
      Data8 = HECI_CMD_BME | HECI_CMD_MSE;
      Heci2PciWrite8(HECI_R_COMMAND, Data8);
    }
    S3BootScriptSavePciCfgWrite(S3BootScriptWidthUint8,
                                S3BOOTSCRIPT_PCIADDR(HECI2_BUS, HECI2_DEV, HECI2_FUN, HECI_R_COMMAND),
                                1, &Data8);
  }
  S3BootScriptSaveMemWrite(EfiBootScriptWidthUint32, (UINTN)(PCH_RCRB_BASE + R_PCH_RCRB_FD2),
                           1, &pSpsContext->RcbaFd2);
  return;
} // SpsExitPmAuthCallback()


/*++
  Routine Description:
    SPS callback for ready-to-boot event.
    Performs actions required by SPS ME-BIOS Spec before OS is loaded.
  Arguments:
    Event    - the event, not used
    pContext - pointer to SPS firmware description
  Returns:
    Void.
--*/
VOID SpsReadyToBootCallback(
  IN EFI_EVENT      Event,
  IN VOID           *pContext)
{
  EFI_STATUS        Status;
  UINT32            RspLen;
  EFI_HECI_PROTOCOL *pHeci;
  SPS_DXE_CONTEXT   *pSpsContext = (SPS_DXE_CONTEXT*)pContext;
  union
  {
    MKHI_MSG_END_OF_POST_REQ EopReq;
    MKHI_MSG_END_OF_POST_RSP EopRsp;
  } HeciMsg;

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol(&gEfiHeciProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
  }
  else
  {
    //
    // Send END_OF_POST notification if not desibled in ME debug options
    //
    if (pSpsContext->SysCfg.MeEndOfPostEnabled)
    {
      DEBUG((EFI_D_INFO, "[SPS] Sending END_OF_POST to ME\n"));
      
      HeciMsg.EopReq.Mkhi.Data = 0x00000CFF;
      RspLen = sizeof(HeciMsg.EopRsp);
      Status = pHeci->SendwACK((UINT32*)&HeciMsg.EopReq,
                               sizeof(HeciMsg.EopReq), &RspLen,
                               SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
      if (EFI_ERROR(Status))
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send END_OF_POST (%r)\n", Status));
        MeReportError (MSG_EOP_ERROR);
      }
      else if (HeciMsg.EopRsp.Mkhi.Data != 0x00008CFF)
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: Invalid END_OF_POST response (MKHI: 0x%X)\n",
                            HeciMsg.EopRsp.Mkhi.Data));
      }
      else if (RspLen == sizeof(HeciMsg.EopRsp) && // if response contains Action and
               HeciMsg.EopRsp.Action == 1)         // global reset was requested
      {
        DEBUG((EFI_D_ERROR, "[SPS] ERROR: END_OF_POST response requests Global Reset!!!\n"));
        // TODO: perform GlobalReset
      }
    } // if (SystemCfgData.MeEndOfPostEnabled)
  } // if (HECI protocol located)
  //
  // Lock global reset in PMIR regiser
  //

  DEBUG((EFI_D_INFO, "[SPS] Disabling Global Reset capability\n"));
  PciAndThenOr32(PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC,
                                  PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_PMIR),
                  (UINT32)~B_PCH_LPC_PMIR_CF9GR, (UINT32)B_PCH_LPC_PMIR_CF9LOCK);

  DEBUG((EFI_D_INFO, "[SPS] Disabling ME functions:"));
  if (pSpsContext->RcbaFd2 & B_PCH_RCRB_FD2_MEI1D) DEBUG((EFI_D_INFO, " 0 (HECI-1)"));
  if (pSpsContext->RcbaFd2 & B_PCH_RCRB_FD2_MEI2D) DEBUG((EFI_D_INFO, " 1 (HECI-2)"));
  if (pSpsContext->RcbaFd2 & B_PCH_RCRB_FD2_IRERD) DEBUG((EFI_D_INFO, " 2 (IDE-R)"));
  if (pSpsContext->RcbaFd2 & B_PCH_RCRB_FD2_KTD)   DEBUG((EFI_D_INFO, " 3 (KT)"));
  DEBUG((EFI_D_INFO, "\n"));
  
  MmioWrite32(PCH_RCBA_ADDRESS + R_PCH_RCRB_FD2, pSpsContext->RcbaFd2);
  
  gBS->CloseEvent(Event); //AptioV server override: Close event so that is isn't called again.
  
  return;
} // SpsReadyToBootCallback()


