/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  2014-2017 Intel Corporation. All rights reserved

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
  PlatformCpuPolicy.c

Abstract: 
  EPG specific CPU Policy Platform Driver. Formely known as PlatformMicrocode.

--*/

#include <Token.h> // AptioV server override

#include <PiDxe.h>

#include <Protocol/IntelCpuPcdsSetDone.h>
#include <Protocol/PlatformType.h>
#include <Protocol/ExitPmAuth.h>
#include <Guid/SetupVariable.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/CpuPpmLib.h>

#include <Platform.h>
#include <Setup/IioUniversalData.h>
#include <Protocol\CpuCsrAccess.h>
#include <UBOX_MISC.h>
#include "PlatformHost.h"
#include "PfatDefinitions.h"
#define PCD_CPU_PCIE_DOWNSTREAMPECIWRITE_BIT     0x00001000

VOID
InitializePfatToolsIntCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );


CHAR16 mCpuSocketStr[4][5] = {L"CPU0", L"CPU1", L"CPU2", L"CPU3"};
CHAR16 mCpuAssetTagStr[] = L"UNKNOWN";
EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;

/**
  Set platform CPU data that related to SMBIOS.
**/
VOID
PlatformCpuSmbiosData (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_PLATFORM_TYPE_PROTOCOL    *PlatformType;
  UINT32                        CpuSocketCount;
  UINTN                         Index;
  CHAR16                        **CpuSocketNames;
  CHAR16                        **CpuAssetTags;

  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &PlatformType
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Set the count of CPU sockets on the board.
  //
  
   
    switch (PlatformType->BoardId) {
#ifdef GRANGEVILLE_FLAG
    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
    case TypeTrabucoCanyon:
    case TypeCamelbackMountain:
    case TypeTlaquepaque:
    case TypePiestewaPeak:
	case TypeEchoCanyon:
      CpuSocketCount = 1;
      break;
#else
    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
      CpuSocketCount = 1;
      break;

    case TypeIncaCityCRB:
    case TypeIncaCitySRP:

      CpuSocketCount = 4;
      break;

    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
    case TypeWildcatPass:
    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeHalfWidthCitySRP:
    case TypeEmeraldPoint: 

#endif
     default:
      CpuSocketCount = 2;
      break;
  }

 

  PcdSet32 (PcdPlatformCpuSocketCount, CpuSocketCount);

  CpuSocketNames = AllocatePool (CpuSocketCount * sizeof (UINTN));
  CpuAssetTags = AllocatePool (CpuSocketCount * sizeof (UINTN));

  if((CpuSocketNames !=NULL) && (CpuAssetTags != NULL)) {
    for (Index = 0; Index < CpuSocketCount; Index++) {
      CpuSocketNames[Index] = mCpuSocketStr[Index];
      CpuAssetTags[Index] = mCpuAssetTagStr;
    }

  PcdSet64 (PcdPlatformCpuSocketNames, (UINT64) (UINTN) CpuSocketNames);
  PcdSet64 (PcdPlatformCpuAssetTags, (UINT64) (UINTN) CpuAssetTags);
  }
}

/**
  Check and allocate memory for IED trace, aslo report
  base and size (in 1M boundary) to scrtachpad register
  BIOSNONSTICKYSCRATCHPAD3 (Base address) and 
  BIOSNONSTICKYSCRATCHPAD3 (size)
**/
VOID
CheckAndPopulateIedTraceMemory(
  UINTN       IedTraceSize
  )
{
  EFI_STATUS          Status;
  IIO_UDS             *IioUds;
  EFI_GUID            UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;  
  EFI_HOB_GUID_TYPE   *GuidHob;
  UINTN               NumPages;
  UINTN               Addr;
  UINTN               AllocateSize;
  UINT8               i;
  UINT32              RegData;


  AllocateSize = IedTraceSize;
    
  //valid Ied trace size 4M to 1GB
  ASSERT_EFI_ERROR((IedTraceSize < 0x400000) || (IedTraceSize > 0x40000000));

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  IioUds = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (IioUds);
  
  while(1) {
    NumPages = EFI_SIZE_TO_PAGES (AllocateSize + 0x200000); //IED need to start at even 1M boundary
    Status = gBS->AllocatePages (
                          AllocateAnyPages, 
                          EfiReservedMemoryType,
                          NumPages,
                          &Addr
                          );
    if(Status == EFI_SUCCESS) {
      //make IedTrace start at even 1M boundary
      if(Addr & 0x100000) { //Addr starts somewhere inside odd Meg address
        Addr = (Addr & 0xFFF00000) + 0x100000; //make it at even 1M boundary
      } else { //Addr starts somehwere in an even Meg
        Addr = (Addr & 0xFFF00000) + 0x200000;
      }
      
      gBS->SetMem((UINT8 *)Addr, AllocateSize, 0);
      //set "INTEL RSVD" signature on begging address of IedTrace memory
      gBS->CopyMem((UINT8 *)Addr, "INTEL RSVD", 10);      
      break;
    } else {
      AllocateSize /= 2;
      if(AllocateSize < 0x400000) { //min 4M
        DEBUG ((EFI_D_ERROR, "Error(%x): Failed to allocate IedTrace memory\n", Status));
        Addr = 0;
        AllocateSize = 0;
        break;
      }
    }

  }

  if(AllocateSize != IedTraceSize) {
    DEBUG ((EFI_D_ERROR, "Warning: Request Iedtrace Size: 0x%x, only allocate 0x%x != \n", IedTraceSize, AllocateSize));
  } else {
    DEBUG ((EFI_D_ERROR, "Allocate 0x%x byte IedTrace Memory @0x%x\n", AllocateSize, Addr));
  }

  //now store info into scratchpad
  for(i = 0; i < MAX_SOCKET; i++) {
    if(IioUds->PlatformData.CpuQpiInfo[i].Valid) {
      // populate IEDTrace Base (MB bounded) to NonSticky Scratchpad10 
      RegData = (UINT32)(UINTN)(Addr >> 20);
      mCpuCsrAccess->WriteCpuCsr(i, 0, BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG, RegData);

      // populate IEDTrace Size in MB to NonSticky Scratchpad11
      RegData = (UINT32)(UINTN)(AllocateSize >> 20);      
      mCpuCsrAccess->WriteCpuCsr(i, 0, BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG, RegData);

    }
  }
}


EFI_STATUS
PlatformCpuPolicyEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++
  
Routine Description:

  This is the EFI driver entry point for the CpuPolicy Driver. This
  driver is responsible for getting microcode patches from FV.

Arguments:

  ImageHandle     - Handle for the image of this driver.
  SystemTable     - Pointer to the EFI System Table.

Returns:

  EFI_SUCCESS     - Protocol installed sucessfully.

--*/
{
  EFI_STATUS                       Status;
  UINTN                            SysCfgSize;
  SYSTEM_CONFIGURATION             SysCfg;
  UINT32                           CpuPolicy;
  UINT32                           CpuPolicyEx1;
  EFI_HANDLE                       Handle;
  UINT32                           CsrSapmCtl;
  UINT32                           CsrPerfPlimitCtl;
  UINT8                            socket;
  UINT8                            ConfigTDPCtrl;
  UINT8                            PCPSOptions = 0;
  UINT32                           AdvPwrMgtCtl;

  UINT32                           MsrPowerCtlLow  = 0;
  UINT32                           MsrTurboPowerLimitHigh;
  UINT32                           MsrTurboPowerLimitLow;
  UINT32                           MsrPriPlaneCurrentCfgCtlHigh;
  UINT32                           MsrPriPlaneCurrentCfgCtlLow;
  UINT32                           CsrDynamicPerfPowerCtl;
  UINT32                           CsrPcieIltrOvrd;
  UINT32                           MsrPerfBiasConfig;
  UINT32                           TurboOverride = 0;


  EFI_PLATFORM_TYPE_PROTOCOL       *PlatformType;  
  UINT32                           CpuFamilyModelStepping;

  UINT64                           i;
  UINT64                           *Addr;
  EFI_PPM_STRUCT                   *ppm = NULL;
  UINT32                           *PkgCstEntryCriteriaMask = NULL;
  XE_STRUCT                        *XePtr = NULL;
  VOID                             *Registration;

  IIO_UDS                          *IioUds;
  EFI_GUID                         UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;  
  EFI_HOB_GUID_TYPE                *GuidHob;

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  IioUds = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (IioUds);

  AsmCpuid (1, &CpuFamilyModelStepping, NULL, NULL, NULL);

  PlatformCpuSmbiosData ();
  
  ///
  /// Create an ExitPmAuth protocol callback event for PFAT.
  ///
  EfiCreateProtocolNotifyEvent (
          &gExitPmAuthProtocolGuid,
          8,
          InitializePfatToolsIntCallback,
          NULL,
          &Registration
          );

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->AllocatePool (
                        EfiBootServicesData, 
                        sizeof(EFI_PPM_STRUCT),
                        &Addr
                        );
  if(Status != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "::Failed to allocate mem for PPM Struct\n"));
    ASSERT_EFI_ERROR (Status);      //may need to create a default
  } else {
    ZeroMem(Addr, sizeof(EFI_PPM_STRUCT));
    i = (UINT32)(*(UINT64 *)(&Addr));
    ppm = (EFI_PPM_STRUCT *)(Addr);
    PcdSet64 (PcdCpuPmStructAddr, i);
    DEBUG ((EFI_D_INFO, "::ppm mem allocate @ %x %X %X\n", i, PcdGet64(PcdCpuPmStructAddr), ppm));
    PkgCstEntryCriteriaMask = (UINT32 *)(((EFI_PPM_STRUCT *)Addr)->Cst.PkgCstEntryCriteriaMask);
    XePtr = (XE_STRUCT *)(&((EFI_PPM_STRUCT *)Addr)->Xe);
    DEBUG ((EFI_D_INFO, ":: PkgC @ %X XE @ %X\n", PkgCstEntryCriteriaMask, XePtr));


  }

  //
  // Read the current system configuration variable store.
  //
  SysCfgSize = sizeof (SysCfg);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  NULL, // Attributes
                  &SysCfgSize,
                  &SysCfg
                  );

  if (EFI_ERROR(Status)) {
    //
    // Safe default settings.
    //
    CpuPolicy = PCD_CPU_HT_BIT |
                PCD_CPU_C1E_BIT |
                PCD_CPU_EXECUTE_DISABLE_BIT |
                PCD_CPU_MACHINE_CHECK_BIT |
                PCD_CPU_DCU_PREFETCHER_BIT |
                PCD_CPU_IP_PREFETCHER_BIT |
                PCD_CPU_MONITOR_MWAIT_BIT |
                PCD_CPU_THERMAL_MANAGEMENT_BIT |
                PCD_CPU_EIST_BIT;

    CpuPolicyEx1 = PCD_CPU_MLC_STREAMER_PREFETCHER_BIT |
                   PCD_CPU_MLC_SPATIAL_PREFETCHER_BIT |
                   PCD_CPU_ENERGY_PERFORMANCE_BIAS_BIT |
                   PCD_CPU_TURBO_MODE_BIT |
                   PCD_CPU_C_STATE_BIT;

    PcdSet8 (PcdCpuDcuMode, 0);
    PcdSet8 (PcdCpuProcessorMsrLockCtrl, 0);

    PcdSet8 (PcdCpuDebugInterface, 0);
    PcdSet64(PcdCpuIioLlcWaysBitMask, 0);
    PcdSet64(PcdCpuQlruCfgBitMask, 0);
    SysCfg.IedTraceSize = 0;

    PcdSet8 (PcdSbspSelection, 0xFF);   //default to auto
    ppm->ConfigTDP = 0;
    ppm->PcpsCtrl = 0;
    ppm->Pl2SafetyNetEnable = 1;
    ppm->FastRaplDutyCycle = 0;

  } else {
    //
    // Verify that the value being set is within the valid range 0 to MAX_SOCKET - 1
    //
	if (SysCfg.BspSelection > MAX_SOCKET)
      SysCfg.BspSelection= 0xFF;
    PcdSet8 (PcdSbspSelection, SysCfg.BspSelection);

    //
    // Map CPU setup options to PcdCpuProcessorFeatureUserConfiguration
    //
    CpuPolicy = (SysCfg.ProcessorHyperThreadingDisable ? 0 : PCD_CPU_HT_BIT) |
                (SysCfg.ProcessorEistEnable ? PCD_CPU_EIST_BIT : 0) |
                (SysCfg.ProcessorC1eEnable ? PCD_CPU_C1E_BIT : 0) |
                (SysCfg.ProcessorVmxEnable ? PCD_CPU_VT_BIT : 0) |
                (SysCfg.ProcessorSmxEnable ? PCD_CPU_LT_BIT : 0) |
                (SysCfg.FastStringEnable ? PCD_CPU_FAST_STRING_BIT : 0) |
                (SysCfg.ExecuteDisableBit ? PCD_CPU_EXECUTE_DISABLE_BIT : 0) |
                (SysCfg.MachineCheckEnable ? PCD_CPU_MACHINE_CHECK_BIT : 0) |
                (SysCfg.DCUStreamerPrefetcherEnable ? PCD_CPU_DCU_PREFETCHER_BIT : 0) |
                (SysCfg.DCUIPPrefetcherEnable ? PCD_CPU_IP_PREFETCHER_BIT : 0) |
//MWAIT changes-start 
                (SysCfg.MonitorMwaitEnable ? PCD_CPU_MONITOR_MWAIT_BIT : 0) | 
//MWAIT changes-end

                (SysCfg.TStateEnable ? PCD_CPU_TSTATE_BIT : 0) |
                (SysCfg.TurboMode ? PCD_CPU_TURBO_MODE_BIT : 0) |
                (SysCfg.EnableThermalMonitor ? PCD_CPU_THERMAL_MANAGEMENT_BIT : 0);
                // (SysCfg.CpuidMaxValue ? PCD_CPU_MAX_CPUID_VALUE_LIMIT_BIT : 0);

    CpuPolicyEx1 = (SysCfg.ProcessorCcxEnable ? PCD_CPU_C_STATE_BIT : 0) |
                   (SysCfg.MlcStreamerPrefetcherEnable ? PCD_CPU_MLC_STREAMER_PREFETCHER_BIT : 0) |
                   (SysCfg.MlcSpatialPrefetcherEnable ? PCD_CPU_MLC_SPATIAL_PREFETCHER_BIT : 0) |
                   (SysCfg.ThreeStrikeTimer ? PCD_CPU_THREE_STRIKE_COUNTER_BIT : 0) |
                   PCD_CPU_ENERGY_PERFORMANCE_BIAS_BIT |
                   (SysCfg.DcaEnable ? PCD_CPU_DCA_BIT : 0) |
                   (SysCfg.ProcessorXapic ? PCD_CPU_X2APIC_BIT : 0) |
                   (SysCfg.AesEnable ? PCD_CPU_AES_BIT : 0) | 
				   (SysCfg.PCIeDownStreamPECIWrite ? PCD_CPU_PCIE_DOWNSTREAMPECIWRITE_BIT : 0);

    //AptioV server override
    PCPSOptions =  (SysCfg.ProcessorEistPsdFunc ? PCD_CPU_PCPS_SINGLEPCTL : 0) |
                   (SysCfg.ProcessorSPD ? PCD_CPU_PCPS_SPD : 0) |
                   (SysCfg.PStateDomain ? PCD_CPU_PCPS_PSTATEDOMAIN : 0) |
                   (UINT8) SysCfg.ProcessorEistPsdFunc;

    ppm->PcpsCtrl = PCPSOptions;
    ppm->Pl2SafetyNetEnable = SysCfg.Pl2SafetyNetEnable;

    ppm->OverclockingLock = SysCfg.OverclockingLock;

    if (SysCfg.DramRaplInit)    ppm->FastRaplDutyCycle = SysCfg.FastRaplDutyCycle;

    //
    // HSD 4168370 Add EVMode Support to the HSX EP BIOS
    //
    if(IioUds->SetupInfo.MiscConfig.EVMode)
      CpuPolicy &= ~PCD_CPU_LT_BIT;

    if (SysCfg.ProcessorEistEnable) {
      PcdSetBool (PcdCpuHwCoordination, SysCfg.ProcessorEistPsdFunc ? FALSE : TRUE);
    }

    //
    // Verify that the value being set is either of the valid options
    // ITURBO_MODE_TRADITIONAL = 0 or ITURBO_MODE_POWER_OPTIMIZED = 4 
    //
    if (SysCfg.iTurboModeValue != ITURBO_MODE_POWER_OPTIMIZED)
      SysCfg.iTurboModeValue = ITURBO_MODE_TRADITIONAL; // If value out of range use the SETUP default
    PcdSet8 (PcdCpuEnergyPolicy, SysCfg.iTurboModeValue);

    if (SysCfg.ProcessorCcxEnable) {
      PcdSet16 (PcdCpuAcpiLvl2Addr, PCH_ACPI_BASE_ADDRESS + R_ACPI_LV2);
      PcdSet8 (PcdCpuPackageCStateLimit, SysCfg.PackageCState);
    }

    if (SysCfg.TStateEnable) {
      PcdSet8 (PcdCpuClockModulationDutyCycle, SysCfg.OnDieThermalThrottling);
    }
    PcdSet8 (PcdCpuDcuMode, SysCfg.DCUModeSelection);

    //
    // HSD 4168370 Add EVMode Support to the HSX EP BIOS
    //
    if(IioUds->SetupInfo.MiscConfig.EVMode)
      PcdSet8 (PcdCpuProcessorMsrLockCtrl, 0);
    else
      PcdSet8 (PcdCpuProcessorMsrLockCtrl, SysCfg.ProcessorMsrLockControl);

//
//HWPM changes-start
//
#if defined (BDX_HOST)
	 //
	 //ProcessorHWPM-init as disabled.
	 //
      ppm->Hwpm.HWPMNative = 0;
	  ppm->Hwpm.HWPMOOB = 0;
	  
	  if(SysCfg.ProcessorHWPMEnable == 1){
	   ppm->Hwpm.HWPMNative = SysCfg.ProcessorHWPMEnable;
	  }else if (SysCfg.ProcessorHWPMEnable == 2){
	   ppm->Hwpm.HWPMOOB = SysCfg.ProcessorHWPMEnable;
	  }else if (SysCfg.ProcessorHWPMEnable == 0){
	   ppm->Hwpm.HWPMNative = 0;
	   ppm->Hwpm.HWPMOOB = 0;
	   }
	//
	//ProcessorAutonumousCstateEnable-init as disabled.
	//
	ppm->Hwpm.AutoCState = 0;
      if(SysCfg.ProcessorAutonumousCstateEnable == 1){
	  	ppm->Hwpm.AutoCState = 1;
      	}

#endif  // BDX_HOST

//
//HWPM changes-end
//
    // 4165364	Support for Debug Interface MSR
    PcdSet8 (PcdCpuDebugInterface, SysCfg.DebugInterface);
    PcdSet64(PcdCpuIioLlcWaysBitMask, SysCfg.IioLlcWaysMask);
    PcdSet64(PcdCpuQlruCfgBitMask, ((UINT64) SysCfg.QlruCfgMask_Hi << 32) | (UINT64)SysCfg.QlruCfgMask_Lo );

    //CSR SAPM CTL
    CsrSapmCtl = 0;
	CsrPerfPlimitCtl=0;

    CsrSapmCtl = ( SysCfg.Qpi2PkgcClkGateDis << QPI2_PKGC_CLOCK_GATE_DISABLE_SHIFT ) |
                 ( SysCfg.Qpi01PkgcClkGateDis << QPI01_PKGC_CLOCK_GATE_DISABLE_SHIFT ) |
                 ( SysCfg.IioPkgcClkGateDis << IIO_PKGC_CLOCK_GATE_DISABLE_SHIFT ) |
                 ( SysCfg.MdllOnDe << MDLL_ON_DE_SHIFT ) |
                 ( SysCfg.MpllOnEn << MPLL_ON_DE_SHIFT ) |
                 ( SysCfg.NswakeSrexit << NSWAKE_SREXIT_SHIFT ) |
                 ( SysCfg.ForcePpllOff << FORCE_PPLL_OFF_SHIFT ) |
                 ( SysCfg.PpllOffEna << PPLL_OFF_ENA_SHIFT );

    if (SysCfg.MpllOffEna == 1) {

        CsrSapmCtl |= MPLL_OFF_ENA;// SysCfg.MpllOffEna << MPLL_OFF_ENA_SHIFT;
        
		DEBUG ((EFI_D_INFO, " MPLL_OFF_ENA -enabling:: %X \n",CsrSapmCtl));
    } else if(SysCfg.MpllOffEna == 0){

        CsrSapmCtl &= ~(MPLL_OFF_ENA);
		DEBUG ((EFI_D_INFO, "- MPLL_OFF_ENA -disabling:: %X \n",CsrSapmCtl));
    }

    ppm->SapmCtl  = CsrSapmCtl;

    if ( ((CpuFamilyModelStepping & 0xFFFFFFF0 )>> 4) == CPU_HASWELLSERVER_CPUID ) {
		if((CpuFamilyModelStepping & 0x0000000F ) < C0_REV_HSX){//HSX < C0
			CsrPerfPlimitCtl= ( SysCfg.PerfPLmtThshld << PERF_P_LIMIT_THRESHOLD_SHIFT ) |
                      ( SysCfg.PerfPLimitClip << PERF_P_LIMIT_CLIP_SHIFT ) |
                      ( SysCfg.DisPerfPInput << DISABLE_PERF_P_INPUT_SHIFT ) |
                      ( SysCfg.ResolutionMode << RESOLUTION_MODE_SHIFT ) |
                      ( SysCfg.PerfPLimitEn << REPERF_P_LIMIT_EN_SHIFT );
		}else{//HSX >= C0
		   CsrPerfPlimitCtl= ( SysCfg.PerfPLmtThshld << PERF_PLIMIT_THRESHOLD_SHIFT ) |
                      ( SysCfg.PerfPLimitClipC << PERF_PLIMIT_CLIP_SHIFT ) |
                      ( SysCfg.PerfPlimitDifferential << PERF_PLIMIT_DIFFERENTIAL_SHIFT ) |
                      ( SysCfg.PerfPLimitEn << REPERF_PLIMIT_EN_SHIFT );
		  DEBUG ((EFI_D_INFO, "::HSX PerfLimitEn:: %X XE PerfLimitClipC:: %X\n", SysCfg.PerfPLimitEn,  SysCfg.PerfPLimitClipC));
	  }
    } else { //BDX
	  if((((CpuFamilyModelStepping & 0xFFFFFFF0) >>4 ) == CPU_BDX_EP_SERVER_CPUID) || (((CpuFamilyModelStepping & 0xFFFFFFF0) >> 4) == CPU_BDX_DE_SERVER_CPUID)){//BDX ML,BDX DE
		   CsrPerfPlimitCtl= ( SysCfg.PerfPLmtThshld << BDX_PERF_PLIMIT_THRESHOLD_SHIFT ) |
                      ( SysCfg.PerfPLimitClipC << BDX_PERF_PLIMIT_CLIP_SHIFT ) |
                      ( SysCfg.PerfPlimitDifferential << BDX_PERF_PLIMIT_DIFFERENTIAL_SHIFT ) |
                      ( SysCfg.PerfPLimitEn << BDX_REPERF_PLIMIT_EN_SHIFT );
		   DEBUG ((EFI_D_INFO, ":: BDX PerfLimitEn:: %X XE PerfLimitClipC:: %X\n", SysCfg.PerfPLimitEn,  SysCfg.PerfPLimitClipC));
	  }
    }
    ppm->PerPLimitCtl = CsrPerfPlimitCtl;

    ConfigTDPCtrl = ( SysCfg.ConfigTDPLevel << CONFIG_TDP_LEVEL_SHIFT ) |
                    ( SysCfg.ConfigTDP << CONFIG_TDP_SHIFT );

    ppm->ConfigTDP = ConfigTDPCtrl;

    for( socket = 0; socket < MAX_SOCKET; socket++) {
      PkgCstEntryCriteriaMask[socket] = ((SysCfg.Qpi2In[socket] & 0x02)? QPI_2_IN_L1 : 0) |
                                   ((SysCfg.Qpi1In[socket] & 0x02)? QPI_1_IN_L1 : 0) |
                                   ((SysCfg.Qpi0In[socket] & 0x02) ? QPI_0_IN_L1 : 0 ) |
                                   ((SysCfg.Qpi2In[socket] & 0x01) ? QPI_2_IN_L0S : 0 ) |
                                   ((SysCfg.Qpi1In[socket] & 0x01) ? QPI_1_IN_L0S : 0 ) |
                                   ((SysCfg.Qpi0In[socket] & 0x01) ? QPI_0_IN_L0S : 0 );

      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie0In[socket] & 0x2) << 10) + ((SysCfg.Pcie0In[socket] & 1) << 0x0));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie1In[socket] & 0x2) << 11) + ((SysCfg.Pcie1In[socket] & 1) << 0x1));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie2In[socket] & 0x2) << 12) + ((SysCfg.Pcie2In[socket] & 1) << 0x2));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie3In[socket] & 0x2) << 13) + ((SysCfg.Pcie3In[socket] & 1) << 0x3));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie4In[socket] & 0x2) << 14) + ((SysCfg.Pcie4In[socket] & 1) << 0x4));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie5In[socket] & 0x2) << 15) + ((SysCfg.Pcie5In[socket] & 1) << 0x5));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie6In[socket] & 0x2) << 16) + ((SysCfg.Pcie6In[socket] & 1) << 0x6));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie7In[socket] & 0x2) << 17) + ((SysCfg.Pcie7In[socket] & 1) << 0x7));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie8In[socket] & 0x2) << 18) + ((SysCfg.Pcie8In[socket] & 1) << 0x8));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie9In[socket] & 0x2) << 19) + ((SysCfg.Pcie9In[socket] & 1) << 0x9));
      PkgCstEntryCriteriaMask[socket] |= (((SysCfg.Pcie10In[socket] & 0x2) << 20) + ((SysCfg.Pcie10In[socket] & 1) << 0x10));


      AdvPwrMgtCtl = (SysCfg.PkgCstEntryValCtl? PCD_CPU_PKG_CST_ENTRY_VAL_CTL : 0) |
                  (SysCfg.SapmctlValCtl? PCD_CPU_SAPM_CTL_VAL_CTL : 0) |
                  (SysCfg.CpuPmTuning? PCD_CPU_PM_TUNING : 0) |
                  (SysCfg.SoftwareLtrOvrdValCtl? PCD_CPU_SW_LTR_OVRD_CTL : 0) |
                  (SysCfg.PriPlnCurCfgValCtl? PCD_CPU_PRI_PLN_CURR_CFG_CTL : 0) |
                  (SysCfg.CurrentConfig? PCD_CPU_CURRENT_CONFIG : 0) | 
                  (SysCfg.BootPState? PCU_CPU_EFFICIENT_BOOT : 0) |
                  (SysCfg.ProcessorMsrLockControl? CPU_MSR_LOCK : 0);

      if (SysCfg.MpllOffEna == 2) {
		
        AdvPwrMgtCtl |= MPLL_OFF_ENA_AUTO;
      }

      ppm->AdvPwrMgtCtlFlags = AdvPwrMgtCtl;

    }


    // MSR_POWER_CTL 0x1FC
    MsrPowerCtlLow = ( SysCfg.PkgCLatNeg << PCH_NEG_DISABLE_SHIFT ) |
                     ( SysCfg.LTRSwInput << LTR_SW_DISABLE_SHIFT ) |
                     ( SysCfg.PwrPerfTuning << PWR_PERF_TUNING_CFG_MODE_SHIFT ) |
                     ( SysCfg.PwrPerfSwitch << PWR_PERF_TUNING_ENABLE_DYN_SHIFT ) |
                     ( SysCfg.SAPMControl << PWR_PERF_TUNING_DISABLE_SAPM_SHIFT ) |
                     ( SysCfg.PholdSrDisable << PHOLD_SR_DISABLE_SHIFT ) |
                     ( SysCfg.PholdCstPreventionInit << PHOLD_CST_PREVENTION_INIT_SHIFT ) |
                     ( SysCfg.FASTBrkIntEn << FAST_BRK_INT_EN_SHIFT ) |
                     ( SysCfg.FASTBrkSnpEn << FAST_BRK_SNP_EN_SHIFT ) |
                     ( SysCfg.EETurboDisable << EE_TURBO_DISABLE_SHIFT ) |
                     ( SysCfg.ProchotResponse << PROCHOT_RESPONSE_SHIFT ) |
                     ( SysCfg.ProcessorC1eEnable << C1E_ENABLE_SHIFT ) |
                     ( (SysCfg.EnableProcHot & 0x1) << DIS_PROCHOT_OUT_SHIFT ) |
                     ( (SysCfg.EnableProcHot & 0x2) >> 1 );

    // 4166429	[HSX_A0_PO] CLONE from HSX: SAPM DLL: PCODE is not checking de-feature bit IO_FEATURE_DISABLE_ENABLE_SAPM_DLL
    if(SysCfg.UFSDisable){
        MsrPowerCtlLow |= ( 1 << PWR_PERF_TUNING_DISABLE_SAPM_SHIFT );
    }
	//BIOS HSD 5002584--BIOS needs to set bit 25 in MSR 0x1FC when enabling HWP autonomous out of band mode
	if(SysCfg.ProcessorHWPMEnable == 2){
		MsrPowerCtlLow |= ( 1 << PWR_PERF_TUNING_CFG_MODE_SHIFT );
	}
    ppm->PowerCtl.Dwords.Low = MsrPowerCtlLow;
    
    // PRIMARY_PLANE_CURRENT_CONFIG_CONTROL 0x601
    MsrPriPlaneCurrentCfgCtlHigh = ( SysCfg.Psi3Thshld << PSI3_THSHLD_SHIFT ) |
      ( SysCfg.Psi2Thshld << PSI2_THSHLD_SHIFT ) |
      ( SysCfg.Psi1Thshld << PSI1_THSHLD_SHIFT );

    MsrPriPlaneCurrentCfgCtlLow = ( SysCfg.PpcccLock << PPCCC_LOCK_SHIFT ) |
      ( SysCfg.CurrentLimit << CURRENT_LIMIT_SHIFT );

    ppm->PP0CurrentCfg.Dwords.High = MsrPriPlaneCurrentCfgCtlHigh;
    ppm->PP0CurrentCfg.Dwords.Low = MsrPriPlaneCurrentCfgCtlLow;

    // MSR_TURBO_POWER_LIMIT 0x610
    // CSR_TURBO_POWER_LIMIT 1:30:0:0xe8
    MsrTurboPowerLimitHigh = ( SysCfg.TurboPowerLimitLock << POWER_LIMIT_LOCK_SHIFT ) |
      ( SysCfg.PowerLimit2En << POWER_LIMIT_ENABLE_SHIFT ) |
      ( SysCfg.PkgClmpLim2 << PKG_CLMP_LIM_SHIFT ) |
      ( SysCfg.PowerLimit2Power );

    MsrTurboPowerLimitLow = ( SysCfg.PowerLimit1Time << POWER_LIMIT_1_TIME_SHIFT ) |
      ( SysCfg.PowerLimit1En << POWER_LIMIT_ENABLE_SHIFT ) |
      ( SysCfg.PkgClmpLim1 << PKG_CLMP_LIM_SHIFT ) |
      ( SysCfg.PowerLimit1Power );
    ppm->TurboPowerLimit.Dwords.Low = MsrTurboPowerLimitLow;
    ppm->TurboPowerLimit.Dwords.High = MsrTurboPowerLimitHigh;

    // DYNAMIC_PERF_POWER_CTL (CSR 1:30:2:0x64)
    CsrDynamicPerfPowerCtl = ( SysCfg.EepLOverride << EEP_L_OVERRIDE_SHIFT ) |
      ( SysCfg.EepLOverrideEn << EEP_L_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.ITurboOvrdEn << I_TURBO_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.CstDemotOvrdEN << CST_DEMOTION_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.TrboDemotOvrdEn << TURBO_DEMOTION_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.UncrPerfPlmtOvrdEn << UNCORE_PERF_PLIMIT_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.EetOverrideEn << EET_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.IoBwPlmtOvrdEn << IO_BW_PLIMIT_OVERRIDE_ENABLE_SHIFT ) |
//      ( SysCfg.ImcApmOvrdEn << IMC_APM_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.IomApmOvrdEn << IOM_APM_OVERRIDE_ENABLE_SHIFT ) |
      ( SysCfg.QpiApmOvrdEn << QPI_APM_OVERRIDE_ENABLE_SHIFT );   //4986218: Remove both changes from 4168487

    ppm->DynamicPerPowerCtl= CsrDynamicPerfPowerCtl;

    // CSR_PCIE_ILTR_OVRD (CSR 1:30:1:78)
    // SW_LTR_OVRD (MSR 0xa02) -- not used
    CsrPcieIltrOvrd = ( SysCfg.SnpLatVld << SNOOP_LATENCY_VLD_SHIFT ) |
      ( SysCfg.SnpLatOvrd << FORCE_SNOOP_OVRD_SHIFT ) |
      ( SysCfg.SnpLatMult << SNOOP_LATENCY_MUL_SHIFT ) |
      ( SysCfg.SnpLatVal << SNOOP_LATENCY_Value_SHIFT ) |
      ( SysCfg.NonSnpLatVld << NON_SNOOP_LATENCY_VLD_SHIFT ) |
      ( SysCfg.NonSnpLatOvrd << FORCE_NON_SNOOP_OVRD_SHIFT ) |
      ( SysCfg.NonSnpLatMult << NON_SNOOP_LATENCY_MUL_SHIFT ) |
      ( SysCfg.NonSnpLatVal << NON_SNOOP_LATENCY_Value_SHIFT );

    ppm-> PcieIltrOvrd = CsrPcieIltrOvrd;

    //Turbo Ratio Override for cores 1-8 MSR Turbo ratio Limit
    for(i = 0; i < MAX_CORE; i++) {
      //XeRatioLimit[i] = SysCfg.XeRatioLimit[i];
      XePtr->RatioLimit[i] = SysCfg.XeRatioLimit[i];
    }
    XePtr->Voltage = SysCfg.XeVoltage;
    XePtr->Enable = (SysCfg.XeVoltage ? TRUE : FALSE);

    if (SysCfg.TurboMode && SysCfg.EnableXe) {
      TurboOverride = SysCfg.XeRatioLimit[0];
    }

    PcdSet32(PcdCpuTurboOverride, TurboOverride);

    MsrPerfBiasConfig = ( SysCfg.EngAvgTimeWdw1 << AVG_TIME_Window_SHIFT ) |
    ( SysCfg.P0TtlTimeLow1 << PO_TOTAL_TIME_THSHLD_LOW_SHIFT ) |
    ( SysCfg.P0TtlTimeHigh1 << PO_TOTAL_TIME_THSHLD_HIGH_SHIFT ) |
    ( SysCfg.AltEngPerfBIAS << ALT_ENERGY_PERF_BIAS_SHIFT) |
    ( SysCfg.WorkLdConfig << WORKLD_CONFIG_SHIFT );

    ppm->PerfBiasConfig.Dwords.Low = MsrPerfBiasConfig;


    
    if(SysCfg.C2C3TT) { //if option is not AUTO
      ppm->C2C3TT = (UINT32)SysCfg.C2C3TT;
    } else {
      ppm->C2C3TT = 0x10;
    }

#ifdef GRANGEVILLE_FLAG
#endif  //#ifdef GRANGEVILLE_FLAG

#ifdef DE_SKU
  //CSR, PCH_TEMP_PCU_FUN0_REG
  ppm->UsePchHot = SysCfg.UsePchHot;
  ppm->UsePchTempFromME = SysCfg.UsePchTempFromME;
  ppm->CpuToPchThrottle = SysCfg.CpuToPchThrottle;
#endif  //#ifdef DE_SKU
  } //end - else

  //WA 4165788
  if(((CpuFamilyModelStepping & 0xFFFFFFF0) == CPU_FAMILY_HSX) || ((CpuFamilyModelStepping & 0xFFFFFFF0) == CPU_FAMILY_BDX) || ((CpuFamilyModelStepping & 0xFFFFFFF0) == CPU_FAMILY_BDX_DE)) {
    MsrPowerCtlLow |= DIS_PROCHOT_OUT + ENABLE_BIDIR_PROCHOT_EN;
  }
  ppm->PowerCtl.Dwords.Low |= MsrPowerCtlLow;

  CpuPolicy |= PCD_CPU_L3_CACHE_BIT;

  //
  // If Emulation flag set by InitializeDefaultData in MemoryQpiInit.c
  //  force X2APIC
  //
  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &PlatformType
                  );
  ASSERT_EFI_ERROR (Status);

  if (PlatformType->Emulation == VP_FLAG) {
    CpuPolicyEx1 |= PCD_CPU_X2APIC_BIT;
  }

  PcdSet32 (PcdCpuProcessorFeatureUserConfiguration, CpuPolicy);
  PcdSet32 (PcdCpuProcessorFeatureUserConfigurationEx1, CpuPolicyEx1);

  if (SysCfg.McBankWarmBootClearError == 1)
     PcdSetBool (PcdIsPowerOnReset, TRUE);
  else
     PcdSetBool (PcdIsPowerOnReset, FALSE);

  // allocate memory for IedTrace 
  if((SysCfg.IedTraceSize != 0) && (SysCfg.IedSize != 0)) {
    CheckAndPopulateIedTraceMemory(0x400000 << (SysCfg.IedTraceSize - 1));
  }

  DEBUG ( (EFI_D_INFO, ":: Power Management flags    %x\n", ppm->AdvPwrMgtCtlFlags) );
  DEBUG ( (EFI_D_INFO, ":: Set PowerCtl MSR          %016x\n", ppm->PowerCtl.Qword) );
  DEBUG ( (EFI_D_INFO, ":: Set TurboPowerLimit MSR   %016x\n", ppm->TurboPowerLimit.Qword) );
  DEBUG ( (EFI_D_INFO, ":: Set PP0 Current Config MSR %016x\n", ppm->PP0CurrentCfg.Qword) );
  DEBUG ( (EFI_D_INFO, ":: Set Perf Bias Config  MSR %016x\n", ppm->PerfBiasConfig.Qword) );
  
  DEBUG ( (EFI_D_INFO, ":: Set SAPMCTL CSR                 %08x\n", ppm->SapmCtl) );
  DEBUG ( (EFI_D_INFO, ":: Set PERF LIMIT CTL              %08x\n", ppm->PerPLimitCtl) );
  DEBUG ( (EFI_D_INFO, ":: Set C2C3TT CSR                  %08x\n", ppm->C2C3TT) );
  DEBUG ( (EFI_D_INFO, ":: Set DYNAMIC PERF PWR CTL CSR    %08x\n", ppm->DynamicPerPowerCtl) );
  DEBUG ( (EFI_D_INFO, ":: Set PCIE LTR OVRD CSR           %08x\n", ppm->PcieIltrOvrd) );
  for( socket = 0; socket < MAX_SOCKET; socket++) {
    DEBUG ( (EFI_D_INFO, ":: Set PkgCstEntryCriteriaMaskSkt%d %08x\n", socket, ppm->Cst.PkgCstEntryCriteriaMask[socket]) );
  }
  DEBUG ( (EFI_D_INFO, ":: Set XE Enable  %x\n", ppm->Xe.Enable) );
  DEBUG ( (EFI_D_INFO, ":: Set XE Voltage %x\n", ppm->Xe.Voltage) );

  //
  // Cpu Driver could be dispatched after this protocol installed.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gIntelCpuPcdsSetDoneProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID
InitializePfatToolsIntCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
/*++
  
Routine Description:

   Drop into SMM to register IOTRAP for pfat tools interface

Arguments:

   Event     - A pointer to the Event that triggered the callback.
   Context   - A pointer to private data registered with the callback function.

Returns:

   None

--*/
{
  EFI_STATUS           Status;
  VOID                 *ProtocolPointer;
  EFI_PHYSICAL_ADDRESS BaseAddress;
  PFAT_HOB             *PfatHobPtr;
  EFI_GUID             PfatHobGuid = PFAT_HOB_GUID;

  Status = gBS->LocateProtocol (&gExitPmAuthProtocolGuid, NULL, &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  PfatHobPtr = GetFirstGuidHob (&PfatHobGuid);
  if (PfatHobPtr != NULL) {
    BaseAddress = (EFI_PHYSICAL_ADDRESS) PfatHobPtr->PfatToolsIntIoTrapAdd;
    ///
    /// IOTRAP TO SMM
    ///
    IoRead8 (BaseAddress);
  }
  return;
}
