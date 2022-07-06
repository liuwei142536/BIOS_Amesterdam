/*++

Copyright (c) 2006 - 2015, Intel Corporation.

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

  SpsDxe.c

Abstract:

  This driver implements the DXE phase of SPS support as defined in
  SPS ME-BIOS Interface Specification.
  
--*/
#include <Token.h> // AptioV server override
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
#include <Library/CmosAccessLib.h>
#include <Library/PciLib.h>

#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

#include <Guid/GlobalVariable.h>
#include <Guid/SetupVariable.h>

#include <Guid/HobList.h>
#include <Guid/EventLegacyBios.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Guid/MeFwModesGuid.h>

#include <Protocol/MpService.h>
#include <Protocol/ExitPmAuth.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/CpuConfigLib.h>

#include <Protocol/CpuIo.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/PciIo.h>
#include <Protocol/SpsSiEn.h>
#include <Protocol/SpsNm.h>

#include <HeciRegs.h>
#include <Sps.h>
#include "SpsDxe.h"


/******************************************************************************
 * Function prototypes
 */
#ifdef DE_SKU
#define DEFAULT_PCH_TEMP_REPORTING_INTERVAL     250
#define DEFAULT_PCH_TEMP_MAX_LOW_POWER_INTERVAL 1000

EFI_STATUS SpsSendPchTempReportingCfg(VOID);
#endif


/******************************************************************************
 * Variables
 */
EFI_EVENT mSpsExitPmAuthEvent = NULL;
EFI_EVENT mSpsReadyToBootEvent = NULL;
SPS_DXE_CONTEXT *mpSpsContext = NULL;


/******************************************************************************
 * Functions
 */
/*++
  Routine Description:
    TBD.
  Arguments:
    pSpsMode - 
    pSystemTable - pointer to system table
    pHob - 
  Returns:
    EFI status is returned.
--*/
EFI_STATUS
GetSpsConfiguration (
  IN ME_FW_DISCOVERY_MODE   *pSpsMode,
  IN EFI_SYSTEM_TABLE       *SystemTable,
  EFI_HOB_TYPE_SPS_INFO     *pHob
  )
{

  VOID                  *HobList;
  EFI_GUID              TempGUID;
  EFI_STATUS            Status;
  EFI_HOB_GUID_TYPE     *GuidHob;
  EFI_HOB_TYPE_SPS_INFO *pInfoData;
  UINT8                 Index;

  DEBUG((EFI_D_INFO, "[SPS] Getting Info from PEI\n"));
  Status = EFI_ERROR (1);
  for (Index = 0; Index < SystemTable->NumberOfTableEntries; Index++)
  {
    CopyMem (&TempGUID, &(SystemTable->ConfigurationTable[Index].VendorGuid), sizeof (EFI_GUID));
    if (CompareGuid (&gEfiHobListGuid, &TempGUID))
    {
      HobList = SystemTable->ConfigurationTable[Index].VendorTable;
      Status  = EFI_SUCCESS;
      break;
    }
  }

  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate any HOB\n"));
    return EFI_UNSUPPORTED;
  }

  DEBUG((EFI_D_INFO, "[SPS] Looking for SPS HOB info from PEI\n"));
  
  GuidHob = GetFirstGuidHob (&gEfiSpsInfoHobGuid);
  pInfoData = GET_GUID_HOB_DATA (GuidHob);
  if (pInfoData == NULL)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate SPS HOB\n"));
    return EFI_UNSUPPORTED;
  }
  
  DEBUG((EFI_D_INFO, "[SPS] HOB: flow %d, feature set 0x%04X, pwr opt boot %d, cores2disable %d\n",
        pInfoData->WorkFlow, pInfoData->FeatureSet, pInfoData->PwrOptBoot, pInfoData->Cores2Disable));
  
  CopyMem(pHob, pInfoData, sizeof(*pHob));

  if (pInfoData->WorkFlow != ME_FUNCTIONAL)
  {
    pSpsMode->WorkFlow = pInfoData->WorkFlow;
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: ME is non functional (flow %d)\n", pInfoData->WorkFlow));
    Status = EFI_UNSUPPORTED;
    return Status;
  }
  pSpsMode->WorkFlow        = pInfoData->WorkFlow;
  pSpsMode->FeatureSet.Data = pInfoData->FeatureSet;

  if (!pSpsMode->FeatureSet.Bits.NodeManager && !pSpsMode->FeatureSet.Bits.DCMI)
  {
    pSpsMode->Mode = SIEN_MODE;
  }

  if (pSpsMode->FeatureSet.Bits.DCMI && !pSpsMode->FeatureSet.Bits.NodeManager) {
    pSpsMode->Mode = DM_MODE;
  }

  if (pSpsMode->FeatureSet.Bits.NodeManager && !pSpsMode->FeatureSet.Bits.DCMI) {
    pSpsMode->Mode = NM_MODE;
  }

  if (pSpsMode->FeatureSet.Bits.NodeManager && pSpsMode->FeatureSet.Bits.DCMI) {
    pSpsMode->Mode = DNM_MODE;
  }
  return EFI_SUCCESS;
} // GetSpsConfiguration()


/*++
  Routine Description:
    SPS DXE driver entry point.
  Arguments:
    ImageHandle - handle to SPS DXE image
    pSystemTable - pointer to system table
  Returns:
    Void.
--*/
EFI_STATUS
SpsDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *pSystemTable
  )
{
  EFI_STATUS            Status;
  UINTN                 SysCfgSize;
  UINT32                SysCfgAttr;
  EFI_HOB_TYPE_SPS_INFO HobInfo;
  VOID                  *pRegistration;
  
  DEBUG((EFI_D_ERROR | EFI_D_INFO, "[SPS] DXE PHASE \n"));

  Status = gBS->AllocatePool(EfiBootServicesData, sizeof(*mpSpsContext), (VOID**)&mpSpsContext);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Memory allocation failed (%r)\n", Status));
    return Status;
  }
  SetMem(mpSpsContext, sizeof(*mpSpsContext), 0);
  
  SysCfgSize = sizeof(mpSpsContext->SysCfg);
  // AptioV server override: Changed variable name to Intelsetup  
  Status = gRT->GetVariable(L"IntelSetup", &gEfiSetupVariableGuid, &SysCfgAttr,
                            &SysCfgSize, &mpSpsContext->SysCfg);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot get system configuration (%r)\n"
                        "      Using hardcoded defaults\n", Status));
    mpSpsContext->SysCfg.MeHmrfpoLockEnabled = TRUE;
    mpSpsContext->SysCfg.MeGrLockEnabled = TRUE;
    mpSpsContext->SysCfg.MeEndOfPostEnabled = TRUE;
    mpSpsContext->SysCfg.MeHeci1Enabled = 2; // auto
    mpSpsContext->SysCfg.MeHeci2Enabled = 2; // auto
    mpSpsContext->SysCfg.MeIderEnabled = 2; // auto
    mpSpsContext->SysCfg.MeKtEnabled = 2; // auto
    mpSpsContext->SysCfg.MeSiliconViewTechEnabled = 0; // disable
#ifdef DE_SKU
    mpSpsContext->SysCfg.UsePchTempFromME = 1;
#endif
  }
  
  Status = GetSpsConfiguration(&mpSpsContext->SpsMode, pSystemTable, &HobInfo);
  if (EFI_ERROR(Status))
  {
    return Status;
  }
  if (GetBootModeHob() != BOOT_ON_S4_RESUME &&
      (mpSpsContext->SysCfg.NmPwrOptBoot != HobInfo.PwrOptBoot ||
       mpSpsContext->SysCfg.NmCores2Disable != HobInfo.Cores2Disable))
  {
    if (!mpSpsContext->SysCfg.NmPwrOptBootOverride)
    {
      DEBUG((EFI_D_INFO, "[SPS] Updating NM boot mode %d->%d\n",
               mpSpsContext->SysCfg.NmPwrOptBoot, HobInfo.PwrOptBoot));
      mpSpsContext->SysCfg.NmPwrOptBoot = HobInfo.PwrOptBoot;
      Status = !EFI_SUCCESS;
    }
    if (!mpSpsContext->SysCfg.NmCores2DisableOverride)
    {
      DEBUG((EFI_D_INFO, "[SPS] Updating boot mode %d->%d, cores to disable %d -> %d\n",
               mpSpsContext->SysCfg.NmPwrOptBoot, HobInfo.PwrOptBoot,
               mpSpsContext->SysCfg.NmCores2Disable, HobInfo.Cores2Disable));
      mpSpsContext->SysCfg.NmCores2Disable = HobInfo.Cores2Disable;
      Status = !EFI_SUCCESS;
    }
    if (Status != EFI_SUCCESS)
    {
	// AptioV server override: Changed variable name to Intelsetup
      Status = gRT->SetVariable(L"IntelSetup", &gEfiSetupVariableGuid,
                                SysCfgAttr, SysCfgSize, &mpSpsContext->SysCfg);
      ASSERT_EFI_ERROR(Status);
    }
  }

#ifdef DE_SKU
  //
  // Send PCH temperature reporting configuration to ME
  // 
  if (mpSpsContext->SysCfg.UsePchTempFromME) // If not disabled in debug menu
  {
    SpsSendPchTempReportingCfg();
  }
#endif
  
  if (!mpSpsContext->SpsMode.FeatureSet.Bits.NodeManager && !mpSpsContext->SpsMode.FeatureSet.Bits.DCMI)
  {
    EFI_SPS_SIEN_PROTOCOL *SiEnProtocol;
    
    Status = gBS->AllocatePool (
                      EfiReservedMemoryType,
                      sizeof (EFI_SPS_SIEN_PROTOCOL),
                      &SiEnProtocol
                      );
    ASSERT_EFI_ERROR (Status);
  
    Status = gBS->InstallProtocolInterface (
                      &ImageHandle,
                      &gEfiSpsSiEnProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      SiEnProtocol
                      );
    ASSERT_EFI_ERROR (Status);
    DEBUG((EFI_D_INFO, "[SPS] SiliconEnabling Mode\n"));
  } // if (!NodeManager && !DCMI)
  if (mpSpsContext->SpsMode.FeatureSet.Bits.NodeManager)
  {
    EFI_SPS_NM_PROTOCOL *pNmProtocol;

    GatherCPUInfoData(&HobInfo, &mpSpsContext->SysCfg);
    
    Status = gBS->AllocatePool (
                      EfiReservedMemoryType,
                      sizeof (EFI_SPS_NM_PROTOCOL),
                      &pNmProtocol
                      );
  
    Status = gBS->InstallProtocolInterface (
                      &ImageHandle,
                      &gEfiSpsNmProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      pNmProtocol
                      );
    DEBUG((EFI_D_INFO, "[SPS] NM Mode \n"));
  }

  mSpsExitPmAuthEvent = EfiCreateProtocolNotifyEvent(&gExitPmAuthProtocolGuid,
                                       TPL_CALLBACK, SpsExitPmAuthCallback,
                                       mpSpsContext, &pRegistration);
  ASSERT(mSpsExitPmAuthEvent != NULL);
  Status = EfiCreateEventReadyToBootEx(TPL_CALLBACK, SpsReadyToBootCallback,
                                       mpSpsContext, &mSpsReadyToBootEvent);
  ASSERT_EFI_ERROR(Status);
  
  return EFI_SUCCESS;
} // SpsDxeEntryPoint()


#ifdef DE_SKU
/*****************************************************************************
* @brief Send PCH temperature reporting configuration to ME.
*
*   Note that this functionality is limited to BDX-DE only.
*
*  @retval EFI status 
**/
EFI_STATUS 
SpsSendPchTempReportingCfg(VOID)
{
  EFI_STATUS         Status;
  SPS_MEFS1          Mefs1;
  UINT32             RspLen;
  union
  {
    HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_REQ Req;
    HECI_MSG_SET_PCH_TEMP_REPORTING_CFG_RSP Rsp;
  } HeciMsg;
  
  EFI_HECI_PROTOCOL *pHeci;
  
  Mefs1.DWord = HeciPciReadMefs1();
  if (Mefs1.Bits.CurrentState != ME_CURSTATE_NORMAL)
  {
    DEBUG((EFI_D_WARN, "[SPS] WARNING: Using PCH temperature from ME is enabled, but ME is not "
                       "operational! (MEFS1: %08X)\n", Mefs1.DWord));
    return EFI_NOT_READY;
  }
  
  Status = gBS->LocateProtocol(&gEfiHeciProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
    return Status;
  }
  
  SetMem(&HeciMsg, sizeof(HeciMsg), 0);
  HeciMsg.Req.Command = SPS_CMD_SET_PCH_TEMP_REPORTING_CFG_REQ;
  HeciMsg.Req.PchTempReportingInterval = DEFAULT_PCH_TEMP_REPORTING_INTERVAL;
  HeciMsg.Req.PchTempMaximumLowPowerInterval = DEFAULT_PCH_TEMP_MAX_LOW_POWER_INTERVAL;
  
  DEBUG((EFI_D_INFO, "[SPS] Sending PCH temperature reporting configuration to ME\n"));
  DEBUG((EFI_D_INFO, "[SPS]   PCH Temperatur Reporting Interval: 0x%04x\n", HeciMsg.Req.PchTempReportingInterval));
  DEBUG((EFI_D_INFO, "[SPS]   PCH Temperatur Maximum Low Power Interval: 0x%04x\n", HeciMsg.Req.PchTempMaximumLowPowerInterval));
  
  RspLen = sizeof(HeciMsg);
  Status = pHeci->SendwACK((UINT32*)&HeciMsg.Req, sizeof(HeciMsg.Req), &RspLen,
                           SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_SPS);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send PCH temperature reporting configuration to ME (%r)\n", Status));
  }
  else if (HeciMsg.Rsp.Result != 0)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PCH temperature reporting configuration NACK (result: %d)\n", HeciMsg.Rsp.Result));
  }
  return Status;
}
#endif // DE_SKU
