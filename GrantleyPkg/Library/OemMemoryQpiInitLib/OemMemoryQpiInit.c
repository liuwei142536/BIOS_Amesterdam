/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2005-2021 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  OemMemoryQpiInit.c

Abstract:


--*/

#pragma warning(disable : 4100)

#include <Token.h> // AptioV server override

#include "EdkMemoryQpiInit.h"
#include <Library/OemMemoryQpiInit.h>
#include <Platform.h>
#include <Guid\MemoryOverwriteControl.h>
//#include <Ppi\TpmInitialized.h>
//#include <Ppi\TpmSaveState.h>
#include <Guid\PprVariable.h>

#if ME_SUPPORT_FLAG
#include <HeciRegs.h>
#include <MeAccess.h>
#include <MeUma.h>
#if SPS_SUPPORT
#include <Sps.h>
#include <Library/TimerLib.h>
#include <Ppi/Stall.h>
#endif
#endif // ME_SUPPORT_FLAG
#ifdef BTG_FLAG
#include "BootGuardLibrary.h"
#endif

#include "MeState.h"

#include <Library/UefiDecompressLib.h>

#pragma pack(1)
typedef struct {
  UINT8   TpmPresent;
  BOOLEAN HideTpm;
  UINT8   TpmCurrentState;
  BOOLEAN MorState;
} TCG_SETUP;
#pragma pack()

#include <Setup.h> // AptioV Server Override: Include header file to get SETUP_DATA definition

#if CRB_OUT_OF_RESOURCE_SUPPORT
#include <Protocol\IioUds.h>  // Aptio V Server Override : Changes done for the CRB defined OutOfResource (MMIO/IO) Support
EFI_GUID gEfiAutoMmioIOVariableGuid     = AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA_GUID;
#endif

//Aptio V Server Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
#include <AmiCRBPkg\XtuCrbTiming\NB\Pei\XtuSaTimingPeiLib.h>
#include <AmiCRBPkg/XtuCrbTiming/Cpu/Pei/Library/CpuPerfTunePeiLib.h>
#endif  //PERF_TUNE_SUPPORT
//Aptio V Server Override for XTU - END

extern EFI_GUID gEfiMemoryConfigDataGuid;
extern EFI_GUID gEfiOcDataGuid;
EFI_GUID gEfiAfterMrcGuid = EFI_AFTER_MRC_GUID;
EFI_GUID gEfiAfterQpircGuid = EFI_AFTER_QPIRC_GUID;
//EFI_GUID gPeiTpmInitializedPpiGuid = PEI_TPM_INITIALIZED_PPI_GUID;

static EFI_PEI_PPI_DESCRIPTOR mAfterQpircPpi[] =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiAfterQpircGuid,
    NULL
};

static EFI_PEI_PPI_DESCRIPTOR mAfterMrcPpi[] =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiAfterMrcGuid,
    NULL
};

// AptioV Server Override start EIP214891: Installs PPI to signal the TCG module to skip TPM Startup if
//                          the TXT policy specified in the FIT (CMOS BITS) is set to enabled.

#define AMI_SKIP_TPM_STARTUP_GUID \
    { 0x6ee1b483, 0xa9b8, 0x4eaf, 0x9a, 0xe1, 0x3b, 0x28, 0xc5, 0xcf, 0xf3,\
      0x6b}

static EFI_GUID gAmiSkipTpmStartupGuid = AMI_SKIP_TPM_STARTUP_GUID;

static EFI_PEI_PPI_DESCRIPTOR gPpiList[] = {
    EFI_PEI_PPI_DESCRIPTOR_PPI
    | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gAmiSkipTpmStartupGuid,
    NULL
};

// AptioV Server Override end EIP214891: Installs PPI to signal the TCG module to skip TPM Startup if
//                          the TXT policy specified in the FIT (CMOS BITS) is set to enabled.
//
// Function Implementations
//

// Aptio V Server Override - Start : Get AcpiSleepState Setup Variable
UINT16
GetAcpiSleepStateSetupVariable (
  struct sysHost             *host )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  EFI_STATUS                            Status;
  EFI_PEI_SERVICES                      **PeiServices;
  SETUP_DATA                            SetupData;

  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );
  if ( EFI_ERROR (Status) ) {
      return 0;
  }
  
  //
  // Get Variable 
  //
  VariableSize = sizeof (SETUP_DATA);
  Status = PeiVariable->GetVariable (
                        PeiVariable,
                        L"Setup",
                        &gEfiSetupVariableGuid,
                        NULL,
                        &VariableSize,
                        &SetupData
                        );
  if ( EFI_ERROR (Status) ) {
      return 0;
  }

  return SetupData.AcpiSleepState;
}
// Aptio V Server Override - End : Get AcpiSleepState Setup Variable

// Aptio V Server override Start: Fix for the MMIOL and IO Ratio Setup option issue
VOID
OemEarlyUncoreInit (
  struct sysHost             *Host
  )
/*++

Routine Description:

  Generic Hook Point prior to any Uncore Initialization

Arguments:

  **PeiServices     General purpose services available to every PEIM
  *IohUds           Universal Data Structure initialized to { 0 }

Returns:

  VOID

--*/
{
	EFI_STATUS					Status;
	EFI_PEI_SERVICES			**PeiServices;
	UINTN						VariableSize;
	struct sysSetup				*SysSetup;
	EFI_PEI_READ_ONLY_VARIABLE2_PPI		*PeiVariable;
	SYSTEM_CONFIGURATION        		SystemConfiguration;
	UINT8					Index;
// Aptio V Server Override Start : Changes done for the CRB defined OutOfResource (MMIO/IO) Support	
#if CRB_OUT_OF_RESOURCE_SUPPORT	
	AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA	AutoAdjustMmioIoVariable;	
#endif
// Aptio V Server Override End : Changes done for the CRB defined OutOfResource (MMIO/IO) Support		
	

	PeiServices  = (EFI_PEI_SERVICES **) Host->var.common.oemVariable;

	SysSetup = (struct sysSetup*)&Host->setup;
	//
	// Locate Variable PPI
	//
	(**PeiServices).LocatePpi (
	              PeiServices,
	              &gEfiPeiReadOnlyVariable2PpiGuid,
	              0,
	              NULL,
	              &PeiVariable
	              );

	VariableSize = sizeof (SYSTEM_CONFIGURATION);

	Status = PeiVariable->GetVariable (
			PeiVariable,
			L"IntelSetup",
			&gEfiSetupVariableGuid,
			NULL,
			&VariableSize,
			&SystemConfiguration
		  );

	if (EFI_ERROR (Status))
	{
		// Default values
		SystemConfiguration.CsiSocketResourceAutoAdjust = 1; // Aptio V Server Override Start : Changes done for the CRB defined OutOfResource (MMIO/IO) Support		
		for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index)
		{
			SystemConfiguration.MmiolRatio[Index] = 1;
			SystemConfiguration.IoRatio[Index] = 1;
		}
	}
	// Aptio V Server Override Start : Changes done for the CRB defined OutOfResource (MMIO/IO) Support	
	#if CRB_OUT_OF_RESOURCE_SUPPORT	
		//
		//  Check the AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA Variable is Created.
		//  The Variable will be created in AmiPciAccessCspBaseLib\AmiPciAccessCspBaseLib.c only when OutOfResource Condition Occurred

		VariableSize = sizeof (AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA);
		Status = PeiVariable->GetVariable(
						PeiVariable,
						L"AutoAdjustMMIOIO",
						&gEfiAutoMmioIOVariableGuid,
						NULL,
						&VariableSize,
						&AutoAdjustMmioIoVariable
						);

		//
		// Check AutoRatio Setup Question is selected as Auto or Manual
		//
		if ((Status == EFI_SUCCESS) && (SystemConfiguration.CsiSocketResourceAutoAdjust)) {
			DEBUG((DEBUG_ERROR, "MMIO and IO Ratio EARLY OEMUNCOREINIT : AutoRatio Setup Option is selected &  AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA Variable is Available\n"));
			for(Index=0;Index<MAX_CPU_SOCKETS;Index++)
			{
				SysSetup->qpi.IoRatio[Index] = AutoAdjustMmioIoVariable.IoRatio[Index];
				SysSetup->qpi.MmiolRatio[Index] = AutoAdjustMmioIoVariable.MmiolRatio[Index];
				DEBUG((DEBUG_ERROR, "IOH=%x MMIO Ratio=%x IO Ratio=%x\n",Index, SysSetup->qpi.MmiolRatio[Index], SysSetup->qpi.IoRatio[Index]));
			}
		}

		//
		//  AUTO_ADJUST_MMMIO_IO_VARIABLE_DATA Variable is Not Created or Setup option is not selected as Auto
		//
	#endif

	else
	{
		for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index)
		{
			SysSetup->qpi.MmiolRatio[Index] = SystemConfiguration.MmiolRatio[Index];
			SysSetup->qpi.IoRatio[Index] = SystemConfiguration.IoRatio[Index];
			DEBUG((DEBUG_ERROR, "IOH=%x MMIO Ratio=%x IO Ratio=%x\n",Index, SysSetup->qpi.MmiolRatio[Index], SysSetup->qpi.IoRatio[Index]));
		}
	}
}
// Aptio V Server override End: Fix for the MMIOL and IO Ratio Setup option issue

// EmeraldPoint specific code for C2F support
void setCPLDClk(UINT16 gpioBase, UINT8 level)
{
#ifdef GRANGEVILLE_FLAG
  // CPLD_CLK
  if (level)
    IoWrite32((gpioBase + 0x38), (IoRead32((gpioBase + 0x38)) | (1 << 9)));
  else
    IoWrite32((gpioBase + 0x38), (IoRead32((gpioBase + 0x38)) & (~(1 << 9))));
#else
  // WBG_CPLD_CNTL0
  if (level)
    IoWrite32((gpioBase + 0x0C), (IoRead32((gpioBase + 0x0C)) | (1 << 14)));
  else
    IoWrite32((gpioBase + 0x0C), (IoRead32((gpioBase + 0x0C)) & (~(1 << 14))));

#endif
}

void setCPLDData(UINT16 gpioBase, UINT8 level)
{
#ifdef GRANGEVILLE_FLAG
  // CPLD_DATA
  if (level)
    IoWrite32((gpioBase + 0x38), (IoRead32((gpioBase + 0x38)) | (1 << 10)));
  else
    IoWrite32((gpioBase + 0x38), (IoRead32((gpioBase + 0x38)) & (~(1 << 10))));
#else
  // WBG_CPLD_CNTL1
  if (level)
    IoWrite32((gpioBase+0x0C),(IoRead32((gpioBase+0x0C)) | (1 << 15)));
  else
    IoWrite32((gpioBase+0x0C),(IoRead32((gpioBase+0x0C)) & (~(1 << 15))));
#endif

}

void spinloop(UINT32 count)
{
  volatile UINT32 i;
  for (i = 0; i < count; i++);
}


void sendCPLDByte(UINT16 gpioBase, UINT8 data)
{
  int i;

  for (i=0; i<8; i++) {
    setCPLDClk(gpioBase,0);
    spinloop(50000);

    setCPLDData(gpioBase, ((data & 0x80) ? 1 : 0));
    spinloop(50000);

    setCPLDClk(gpioBase,1);
    data = data << 1;
    spinloop(50000);
  }
}

VOID
OemGetPlatformSetupValueforIIO(
  struct sysHost             *host,
  SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
  )
{
  if ((TotalSystemConfigurationPtr->IioLinkTrainPhase == 0) || (host->var.common.cpuType == CPU_HSX))
     host->var.iio.IioVData.IioLinkTrainPhase = 0;  // Link training in DXE phase
   else
     host->var.iio.IioVData.IioLinkTrainPhase = 1;  // Link training in PEI phase
  return;
}

VOID
OemGetPlatformSetupValueforMRC(
  struct sysHost             *host,
  SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr,
  PPR_ADDR_VARIABLE          *pprAddrVariable
  )
/*++
--*/
{
  struct sysSetup       *setup;
  UINT8                 socket;
  UINT8                 ch;
  UINT8                 pagePolicy;
  UINT8                 phyRank;
  UINT8                 rank;
  UINT8                 dimm;
  UINT8                 dimmRank;
  UINT8                 i;
  EFI_HOB_GUID_TYPE     *OcGuidHob;
  OC_DATA_HOB           *OcData;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT  scratchPad1Reg;
  
  setup = (struct sysSetup *)(&host->setup);
  //  ddrFreqLimit
  //    0:Auto
  //    1:Force DDR-800
  //    3:Force DDR-1066
  //    5:Force DDR-1333
  //    7:Force DDR-1600
  //    9:Force DDR-1867
  //    11:Force DDR-2133
  //    13:Force DDR-2400
  //
  if (TotalSystemConfigurationPtr->DdrFreqLimit == DDR3_FREQ_AUTO) {
    setup->mem.ddrFreqLimit = DDR3_FREQ_DEFAULT;
  } else {
    setup->mem.ddrFreqLimit = TotalSystemConfigurationPtr->DdrFreqLimit;

    //
    //5003070: Switch to POR frequency if 100 MHz bclk based frequency is not supported.
    //
    scratchPad1Reg.Data = ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    if ((scratchPad1Reg.Data & (BIT11)) != 0) {
      setup->mem.ddrFreqLimit = DDR3_FREQ_DEFAULT;
    }
  }

  if (TotalSystemConfigurationPtr->EnforcePOR == ENFORCE_POR_AUTO) {
    setup->mem.enforcePOR = ENFORCE_POR_DEFAULT;
  } else {
    setup->mem.enforcePOR = TotalSystemConfigurationPtr->EnforcePOR;
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    setup->mem.socket[socket].ddrVddLimit = TotalSystemConfigurationPtr->DdrVoltage; // 0:Auto; 1:Force 1.5v; 2:Force 1.35v
  }

  //setup->mem.options = 0;    // Zero out all options to start



  //
  // PPR options
  //
  setup->mem.pprType        = TotalSystemConfigurationPtr->pprType;
  setup->mem.pprErrInjTest  = TotalSystemConfigurationPtr->pprErrInjTest;
  if (pprAddrVariable != NULL) {
    for (i = 0; i < MAX_PPR_ADDR_ENTRIES; i++) {
      setup->mem.pprAddrSetup[i].pprAddrStatus    = pprAddrVariable->pprAddrSetup[i].pprAddrStatus;
      setup->mem.pprAddrSetup[i].socket           = pprAddrVariable->pprAddrSetup[i].socket;
      setup->mem.pprAddrSetup[i].mc               = pprAddrVariable->pprAddrSetup[i].mc;
      setup->mem.pprAddrSetup[i].ch               = pprAddrVariable->pprAddrSetup[i].ch;
      setup->mem.pprAddrSetup[i].pprAddr.dimm     = pprAddrVariable->pprAddrSetup[i].dimm;
      setup->mem.pprAddrSetup[i].pprAddr.rank     = pprAddrVariable->pprAddrSetup[i].rank;
      setup->mem.pprAddrSetup[i].pprAddr.subRank  = pprAddrVariable->pprAddrSetup[i].subRank;
      setup->mem.pprAddrSetup[i].pprAddr.bank     = pprAddrVariable->pprAddrSetup[i].bank;
      setup->mem.pprAddrSetup[i].pprAddr.row      = pprAddrVariable->pprAddrSetup[i].row;
      setup->mem.pprAddrSetup[i].pprAddr.dramMask = pprAddrVariable->pprAddrSetup[i].dramMask;
    }
  }
  //CECC WA Control Knob
  if (TotalSystemConfigurationPtr->EnCECCWA == CECCWA_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) { //BDX
      if (CECCWA_DEFAULT == CECCWA_AUTO) {
        if ((IsBDXEP(host) && ((IsMccChop(host) && CheckSteppingLessThan(host, CPU_BDX, B0_REV_BDX))))// L0
            || (IsBDXEP(host) && CheckSteppingGreaterThan(host, CPU_BDX, A0_REV_BDX)) //B0 and above
            || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, C0_REV_BDX))) { //DE D0 and above
          setup->mem.optionsExt &= ~CECC_WA; // Disable CECC WA
        } else {
          setup->mem.optionsExt |= CECC_WA;// Enable CECC WA
        }
      } else if (CECCWA_DEFAULT == CECCWA_ENABLE) {
        setup->mem.optionsExt |= CECC_WA;
      } else {
        setup->mem.optionsExt &= ~CECC_WA; 
      }
    } else {// HSX
      setup->mem.optionsExt &= ~CECC_WA;
    }
  } else if (TotalSystemConfigurationPtr->EnCECCWA == CECCWA_ENABLE) {
    if (host->var.common.cpuType == CPU_BDX) { //BDX
      setup->mem.optionsExt |= CECC_WA;
    } else {
      setup->mem.optionsExt &= ~CECC_WA;
    }
  } else {
    setup->mem.optionsExt &= ~CECC_WA;
  }

  if ((TotalSystemConfigurationPtr->CapErrFlow == CAP_ERR_FLOW_AUTO) && (CAP_ERR_FLOW_DEFAULT == CAP_ERR_FLOW_ENABLE)) {
    setup->mem.optionsExt |= CAP_ERR_FLOW;
  } else if (TotalSystemConfigurationPtr->CapErrFlow == CAP_ERR_FLOW_ENABLE) {
    setup->mem.optionsExt |= CAP_ERR_FLOW;
  } else {
    setup->mem.optionsExt &= ~CAP_ERR_FLOW;
  }

#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
  if ((TotalSystemConfigurationPtr->WrCRC == WR_CRC_AUTO) && (WR_CRC_DEFAULT == WR_CRC_ENABLE)) {
   setup->mem.optionsExt |= WR_CRC;
  } else if (TotalSystemConfigurationPtr->WrCRC == WR_CRC_ENABLE) {
    setup->mem.optionsExt |= WR_CRC;
  } else {
    setup->mem.optionsExt &= ~WR_CRC;
  }
#endif

  if ((TotalSystemConfigurationPtr->DDR4Platform == DDR4_PLATFORM_AUTO) && (DDR4_PLATFORM_DEFAULT == DDR4_PLATFORM_ENABLE)) {
    setup->mem.optionsExt |= DDR4_PLATFORM;
  } else if (TotalSystemConfigurationPtr->DDR4Platform == DDR4_PLATFORM_ENABLE) {
    setup->mem.optionsExt |= DDR4_PLATFORM;
  } else {
    setup->mem.optionsExt &= ~DDR4_PLATFORM;
  }

  // WMM Opportunistic read disable
  if ((TotalSystemConfigurationPtr->oppReadInWmm == OPP_RD_WMM_AUTO) && (OPP_RD_WMM_DEFAULT == OPP_RD_WMM_EN)) {
    setup->mem.options &= ~DISABLE_WMM_OPP_READ;
  } else if (TotalSystemConfigurationPtr->oppReadInWmm == OPP_RD_WMM_EN){
    setup->mem.options &= ~DISABLE_WMM_OPP_READ;
  } else {
    setup->mem.options |= DISABLE_WMM_OPP_READ;
  }

  // MCODT_50/100 ohms
  setup->mem.setMCODT = TotalSystemConfigurationPtr->mcODTOverride;

  // ECC Support
  if ((TotalSystemConfigurationPtr->EccSupport == ECC_AUTO) && (ECC_DEFAULT == ECC_EN)) {
    setup->mem.options |= ECC_CHECK_EN;
  } else if (TotalSystemConfigurationPtr->EccSupport == ECC_EN) {
    setup->mem.options |= ECC_CHECK_EN;
  } else {
    setup->mem.options &= ~ECC_CHECK_EN;
  }

  // 3 month timeout for re-training of memory: s4988567
  if ((TotalSystemConfigurationPtr->EnforceThreeMonthTimeout == ENFORCE_TIMEOUT_AUTO) && (ENFORCE_TIMEOUT_DEFAULT == ENFORCE_TIMEOUT_EN)) {
    setup->mem.enforceThreeMonthTimeout = ENFORCE_TIMEOUT_EN;
  } else if (TotalSystemConfigurationPtr->EnforceThreeMonthTimeout == ENFORCE_TIMEOUT_EN) {
    setup->mem.enforceThreeMonthTimeout = ENFORCE_TIMEOUT_EN;
  } else {
    setup->mem.enforceThreeMonthTimeout = ENFORCE_TIMEOUT_DIS;
  }

#ifndef GRANGEVILLE_FLAG
  // Forcing ECC off for TypeHedtxxx
  if ( (host->nvram.common.platformType == TypeHedtSKU1CRB) || (host->nvram.common.platformType == TypeHedtEv) ) {
    setup->mem.options &= ~ECC_CHECK_EN;
  }
#endif

  // Setup option for memory test
  if ((TotalSystemConfigurationPtr->HwMemTest == MEM_TEST_AUTO) && (MEM_TEST_DEFAULT == MEM_TEST_EN)) {
    setup->mem.options |= MEMORY_TEST_EN;
  } else if (TotalSystemConfigurationPtr->HwMemTest == MEM_TEST_EN) {
    setup->mem.options |= MEMORY_TEST_EN;
  } else {
    setup->mem.options &= ~MEMORY_TEST_EN;
  }

  //
  // Setup option for memory test loops
  //
  setup->mem.memTestLoops = TotalSystemConfigurationPtr->MemTestLoops;

  setup->mem.DramMaintenanceTest = TotalSystemConfigurationPtr->DramMaintenanceTest;
  setup->mem.DMTDir        = TotalSystemConfigurationPtr->DMTDir;
  setup->mem.DMTInv        = TotalSystemConfigurationPtr->DMTInv;
  setup->mem.DMTRep        = TotalSystemConfigurationPtr->DMTRep;
  setup->mem.DMTIter       = TotalSystemConfigurationPtr->DMTIter;
  setup->mem.DMTSwizzleEn  = TotalSystemConfigurationPtr->DMTSwizzleEn;
  setup->mem.DMRefreshEn   = TotalSystemConfigurationPtr->DMRefreshEn;

  //
  // DRAM Maintenance
  //
  setup->mem.dramMaint        = TotalSystemConfigurationPtr->DramMaint;
  setup->mem.dramMaintMode    = TotalSystemConfigurationPtr->DramMaintMode;
  setup->mem.dramMaintTRRMode = TotalSystemConfigurationPtr->DramMaintTRRMode;

  // HEDT BCLK RATIO
  
  OcGuidHob       = GetFirstGuidHob (&gEfiOcDataGuid);
  if(OcGuidHob != NULL){
    OcData          = GET_GUID_HOB_DATA (OcGuidHob);
    setup->mem.bclk = OcData->DmiPeg;
  }else{
    setup->mem.bclk = DEFAULT_BCLK;
  }
  

  if (TotalSystemConfigurationPtr->refreshMode)                       setup->mem.options |= ALLOW2XREF_EN;

  // Rank Margin Tool
  if ((TotalSystemConfigurationPtr->RankMargin == RMT_EN) ||
      ((TotalSystemConfigurationPtr->RankMargin == RMT_AUTO) && (RMT_DEFAULT == RMT_EN))) {
    setup->mem.options |= MARGIN_RANKS_EN;
  } else {
    setup->mem.options &= ~MARGIN_RANKS_EN;
  }

  // Scrambling
  if ((TotalSystemConfigurationPtr->ScrambleEn == SCRAMBLE_ENABLE) ||
      ((TotalSystemConfigurationPtr->ScrambleEn == SCRAMBLE_AUTO) && (SCRAMBLE_DEFAULT == SCRAMBLE_ENABLE))) {
    setup->mem.options |= SCRAMBLE_EN;
  } else {
    setup->mem.options &= ~SCRAMBLE_EN;
  }

  // Allow SBE during Memory training
  //
  // AptioV Server Override Start : Set the allowcorrectableerror to enabled when it is Auto and default is Enable.
  //
  if ((TotalSystemConfigurationPtr->allowCorrectableError ) ||
      ((TotalSystemConfigurationPtr->allowCorrectableError == ALLOW_CORRECTABLE_ERROR_AUTO) && 
       (ALLOW_CORRECTABLE_ERROR_DEFAULT == ALLOW_CORRECTABLE_ERROR_ENABLE))) {
  //
  // AptioV Server Override End : Set the allowcorrectableerror to enabled when it is Auto and default is Enable.
  //
    setup->mem.optionsExt |= ALLOW_CORRECTABLE_ERROR;
  } else {
    setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_ERROR;
  }

  // Fast boot
  if ((TotalSystemConfigurationPtr->AttemptFastBoot == FAST_BOOT_AUTO) && (FAST_BOOT_DEFAULT == FAST_BOOT_EN)) {
    setup->mem.options |= ATTEMPT_FAST_BOOT;
  } else if (TotalSystemConfigurationPtr->AttemptFastBoot == FAST_BOOT_EN) {
    setup->mem.options |= ATTEMPT_FAST_BOOT;
  } else {
    setup->mem.options &= ~ATTEMPT_FAST_BOOT;
  }

  // Fast cold boot
  if ((TotalSystemConfigurationPtr->AttemptFastBootCold == FAST_BOOT_COLD_AUTO) && (FAST_BOOT_COLD_DEFAULT == FAST_BOOT_COLD_EN)) {
    setup->mem.options |= ATTEMPT_FAST_BOOT_COLD;
  } else if (TotalSystemConfigurationPtr->AttemptFastBootCold == FAST_BOOT_COLD_EN) {
    setup->mem.options |= ATTEMPT_FAST_BOOT_COLD;
  } else {
    setup->mem.options &= ~ATTEMPT_FAST_BOOT_COLD;
  }

  // RMT on Fast cold boot
  if ((TotalSystemConfigurationPtr->RmtOnColdFastBoot == RMT_COLD_FAST_BOOT_AUTO) && (RMT_COLD_FAST_BOOT_DEFAULT == RMT_COLD_FAST_BOOT_EN)) {
    setup->mem.options |= RMT_COLD_FAST_BOOT;
  } else if (TotalSystemConfigurationPtr->RmtOnColdFastBoot == RMT_COLD_FAST_BOOT_EN) {
    setup->mem.options |= RMT_COLD_FAST_BOOT;
  } else {
    setup->mem.options &= ~RMT_COLD_FAST_BOOT;
  }

  // C/A Parity
  //
  // AptioV Server Override Start : Set the CA parity to enabled when it is Auto and default is Enable.
  //
  if ((TotalSystemConfigurationPtr->caParity) || 
     ((TotalSystemConfigurationPtr->caParity == CA_PARITY_AUTO) && (CA_PARITY_DEFAULT == CA_PARITY_ENABLE))) {
  //
  // AptioV Server Override End : Set the CA parity to enabled when it is Auto and default is Enable.
  //
    setup->mem.options |= CA_PARITY_EN;
  } else {
    setup->mem.options &= ~CA_PARITY_EN;
  }

#ifndef GRANGEVILLE_FLAG
  // Forcing C/A Parity off for TypeHedtSKU1CRB(REV1)-HEDT_FAB1 does not support Early CMD/CLK training: HSD s4987927
  if ((host->nvram.common.platformType == TypeHedtSKU1CRB) && (host->nvram.common.platformRevId == HEDT_FAB1)) {
    setup->mem.options &= ~CA_PARITY_EN;
  }
#endif

  if ((TotalSystemConfigurationPtr->MultiThreaded == MULTI_THREADED_EN)  ||      
      ((TotalSystemConfigurationPtr->MultiThreaded == MULTI_THREADED_AUTO) && (MULTI_THREADED_DEFAULT == MULTI_THREADED_EN) &&         
      !(TotalSystemConfigurationPtr->serialDebugMsgLvl  & (SDBG_MAX | SDBG_TRACE | SDBG_MEM_TRAIN | SDBG_TST | SDBG_CPGC | SDBG_RCWRITETAG)))) {
    setup->mem.options |= MULTI_THREAD_MRC_EN;
  } else {
    setup->mem.options &= ~MULTI_THREAD_MRC_EN;
  }

  DEBUG((EFI_D_ERROR, "ScrambleEn = 0x%x\n", TotalSystemConfigurationPtr->ScrambleEn));

#ifdef XMP_SUPPORT

//Aptio V Server Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
  // AMI Xtu Override.
  GetXtuSaTimingVariable((EFI_PEI_SERVICES **) host->var.common.oemVariable, TotalSystemConfigurationPtr);
#endif  //PERF_TUNE_SUPPORT
//Aptio V Server Override for XTU - END
  setup->mem.options &= ~(MEM_OVERRIDE_EN);
  if (TotalSystemConfigurationPtr->XMPMode != XMP_AUTO) {
    setup->mem.options |= MEM_OVERRIDE_EN;
    setup->mem.enforcePOR = ENFORCE_POR_DIS;
  }

  //
  // Verify that the values being set are within a valid range. They are UINTs so the
  // so the minimum value does not need to be checked.
  //
  //setup->mem.ddrFreqLimit = TotalSystemConfigurationPtr->DdrFreqLimit;
  setup->mem.inputMemTime.vdd = TotalSystemConfigurationPtr->Vdd;

  if (TotalSystemConfigurationPtr->tCAS > MAX_CAS_LATENCY){
    TotalSystemConfigurationPtr->tCAS = 0;  // If out of range set the default.
  }
  setup->mem.inputMemTime.nCL = TotalSystemConfigurationPtr->tCAS;

  if (TotalSystemConfigurationPtr->tRP > MAX_TRP_LATENCY){
    TotalSystemConfigurationPtr->tRP = 0;   // If out of range set the default.
  }
  setup->mem.inputMemTime.nRP = TotalSystemConfigurationPtr->tRP;

  if (TotalSystemConfigurationPtr->tRCD > MAX_TRCD_LATENCY){
    TotalSystemConfigurationPtr->tRCD = 0;  // If out of range set the default
  }
  setup->mem.inputMemTime.nRCD = TotalSystemConfigurationPtr->tRCD;

  if (TotalSystemConfigurationPtr->tRRD > MAX_TRRD_LATENCY){
    TotalSystemConfigurationPtr->tRRD = 0;   // If out of range set the default
  }
  setup->mem.inputMemTime.nRRD = TotalSystemConfigurationPtr->tRRD;

  if (TotalSystemConfigurationPtr->tWTR > MAX_TWTR_LATENCY) {
    TotalSystemConfigurationPtr->tWTR = 0;    // If out of range set the default
  }
  setup->mem.inputMemTime.nWTR = TotalSystemConfigurationPtr->tWTR;

  if (TotalSystemConfigurationPtr->tRAS > MAX_TRAS_LATENCY){
    TotalSystemConfigurationPtr->tRAS = 0;   // If out of range set the default
  }
  setup->mem.inputMemTime.nRAS = TotalSystemConfigurationPtr->tRAS;

  if (TotalSystemConfigurationPtr->tRTP > MAX_TRTP_LATENCY){
    TotalSystemConfigurationPtr->tRTP = 0;   // If out of range set the default
  }
   setup->mem.inputMemTime.nRTP = TotalSystemConfigurationPtr->tRTP;

  if (TotalSystemConfigurationPtr->tWR > MAX_TWR_LATENCY){
    TotalSystemConfigurationPtr->tWR = 0;     // If out of range set the default
  }
  setup->mem.inputMemTime.nWR = TotalSystemConfigurationPtr->tWR;

  if (TotalSystemConfigurationPtr->tFAW > MAX_TFAW_LATENCY){
    TotalSystemConfigurationPtr->tFAW = 0;   // If out of range set the default
  }
  setup->mem.inputMemTime.nFAW = TotalSystemConfigurationPtr->tFAW;

  if (TotalSystemConfigurationPtr->tCWL > MAX_TCWL_LATENCY){
    TotalSystemConfigurationPtr->tCWL = 0;    // If out of range set the default
  }
  setup->mem.inputMemTime.nCWL = TotalSystemConfigurationPtr->tCWL;

  if (TotalSystemConfigurationPtr->tRC > MAX_TRC_LATENCY){
    TotalSystemConfigurationPtr->tRC = 0;     // If out of range set the default
  }
  setup->mem.inputMemTime.nRC = TotalSystemConfigurationPtr->tRC;

  if (TotalSystemConfigurationPtr->tREFI > MAX_REFRESH_RATE){
    TotalSystemConfigurationPtr->tREFI = 0;  // If out of range set the default
  }
  setup->mem.inputMemTime.tREFI = TotalSystemConfigurationPtr->tREFI;

  if (TotalSystemConfigurationPtr->tRFC > MAX_TRFC_LATENCY){
    TotalSystemConfigurationPtr->tRFC = 0;    // If out of range set the default
  }
  setup->mem.inputMemTime.nRFC = TotalSystemConfigurationPtr->tRFC;
  setup->mem.inputMemTime.nCMDRate = TotalSystemConfigurationPtr->commandTiming;
//Aptio V Server Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
  setup->mem.ddrFreqLimit = TotalSystemConfigurationPtr->DdrFreqLimit; 
#endif  //PERF_TUNE_SUPPORT
//Aptio V Server Override for XTU - END

#endif

  //
  // Memory type
  //
  setup->mem.dimmTypeSupport = TotalSystemConfigurationPtr->DdrMemoryType;

  // Interleave and NUMA

  if (TotalSystemConfigurationPtr->NumaEn == NUMA_EN) {
    setup->mem.options |= NUMA_AWARE;
    setup->mem.socketInter = SOCKET_1WAY;
  } else {
    setup->mem.options &= ~NUMA_AWARE;
    setup->mem.socketInter = SOCKET_2WAY;
  }

  if ((setup->mem.socketInter == SOCKET_1WAY) && TotalSystemConfigurationPtr->SocketInterleaveBelow4GB == SOCKET_INTLV_BELOW_4G_EN) {
    setup->mem.options |= SPLIT_BELOW_4GB_EN;
  } else {
    setup->mem.options &= ~SPLIT_BELOW_4GB_EN;
  }

  if (TotalSystemConfigurationPtr->ChannelInterleaving == CH_INTER_AUTO) {
    setup->mem.chInter = CH_INTER_DEFAULT;
  } else {
    setup->mem.chInter = TotalSystemConfigurationPtr->ChannelInterleaving;
  }

  if (TotalSystemConfigurationPtr->RankInterleaving == RANK_INTER_AUTO) {
    setup->mem.rankInter = RANK_INTER_DEFAULT;
  } else {
    setup->mem.rankInter = TotalSystemConfigurationPtr->RankInterleaving;
  }

  // Page mode
  if (TotalSystemConfigurationPtr->PagePolicy == CLOSED_PAGE_AUTO) {
    pagePolicy = CLOSED_PAGE_DEFAULT;
  } else {
    pagePolicy = TotalSystemConfigurationPtr->PagePolicy;
  }

  if (pagePolicy == CLOSED_PAGE_DIS) {
    // Open Fixed
    setup->mem.options &= ~PAGE_POLICY;
    setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else if (pagePolicy == CLOSED_PAGE_EN) {
    // Closed
    setup->mem.options |= PAGE_POLICY;
    setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else if (pagePolicy == OPEN_PAGE_ADAPTIVE) {
    // Open Adaptive
    setup->mem.options &= ~PAGE_POLICY;
    setup->mem.options |= ADAPTIVE_PAGE_EN;
  }

  //
  // CKE Throttling
  //
  if (TotalSystemConfigurationPtr->ckeThrottling == CKE_MODE_AUTO) {
    setup->mem.ckeThrottling = CKE_MODE_DEFAULT;
  } else {
    setup->mem.ckeThrottling = TotalSystemConfigurationPtr->ckeThrottling;
  }

  //
  // CK/CK# OFF for Self Refresh
  //
  setup->mem.pdwnCkMode = TotalSystemConfigurationPtr->pdwnCkMode;

  //
  // Opportunistic Self-Refresh
  //
  if (TotalSystemConfigurationPtr->oppSR == OPP_SR_EN) {
    setup->mem.options |= OPP_SELF_REF_EN;
  } else if ((TotalSystemConfigurationPtr->oppSR == OPP_SR_AUTO) && (OPP_SR_DEFAULT == OPP_SR_EN)) {
    setup->mem.options |= OPP_SELF_REF_EN;
  } else {
    setup->mem.options &= ~OPP_SELF_REF_EN;
  }

  // Setup option for memory test on fast boot
  if ((TotalSystemConfigurationPtr->MemTestOnFastBoot == MEM_TEST_FAST_BOOT_AUTO) && (MEM_TEST_FAST_BOOT_DEFAULT == MEM_TEST_FAST_BOOT_EN)) {
    setup->mem.options |= MEMORY_TEST_FAST_BOOT_EN;
  } else if (TotalSystemConfigurationPtr->MemTestOnFastBoot == MEM_TEST_FAST_BOOT_EN) {
    setup->mem.options |= MEMORY_TEST_FAST_BOOT_EN;
  } else {
    setup->mem.options &= ~MEMORY_TEST_FAST_BOOT_EN;
  }

  if (TotalSystemConfigurationPtr->DdrVrPhaseShedding == PHASE_SHEDDING_AUTO) {
    if ((PHASE_SHEDDING_DEFAULT == PHASE_SHEDDING_EN)
#ifndef GRANGEVILLE_FLAG
    && (!((OemGetPlatformType(host) == TypeArandasVVP4S) || (OemGetPlatformType(host) == TypeArandasVVP2S)))
#endif
    ) {

      setup->mem.phaseShedding = PHASE_SHEDDING_EN;
    } else {
      setup->mem.phaseShedding = PHASE_SHEDDING_DIS;
    }

  } else {
    setup->mem.phaseShedding = TotalSystemConfigurationPtr->DdrVrPhaseShedding;
  }

  setup->mem.dramraplen = TotalSystemConfigurationPtr->DramRaplInit;
  setup->mem.dramraplbwlimittf = TotalSystemConfigurationPtr->BwLimitTfOvrd;
  setup->mem.DramRaplExtendedRange = TotalSystemConfigurationPtr->DramRaplExtendedRange;

#ifdef BDAT_SUPPORT
  setup->common.bdatEn = TotalSystemConfigurationPtr->bdatEn;
#endif //BDAT_SUPPORT

  //
  // Verify that the values being set are within a valid range. They are UINTx so the
  // so the minimum value does not need to be checked.
  //
  if (TotalSystemConfigurationPtr->ScrambleSeedLow > MAX_SCRAMBLE_SEED_LOW){
    TotalSystemConfigurationPtr->ScrambleSeedLow = SCRAMBLE_SEED_LOW;   // If out of range set the default
  }
  setup->mem.scrambleSeedLow = TotalSystemConfigurationPtr->ScrambleSeedLow;

  if (TotalSystemConfigurationPtr->ScrambleSeedHigh > MAX_SCRAMBLE_SEED_HIGH){
    TotalSystemConfigurationPtr->ScrambleSeedHigh = SCRAMBLE_SEED_HIGH; // If out of range set the default
  }
  setup->mem.scrambleSeedHigh = TotalSystemConfigurationPtr->ScrambleSeedHigh;

  if (TotalSystemConfigurationPtr->mcBgfThreshold > MAX_MC_BGF_THRESHOLD){
    TotalSystemConfigurationPtr->mcBgfThreshold= 0;  // If out of range set the default
  }
  setup->mem.mcBgfThreshold = TotalSystemConfigurationPtr->mcBgfThreshold;

  setup->mem.dllResetTestLoops = TotalSystemConfigurationPtr->dllResetTestLoops;

  setup->mem.normOppIntvl = TotalSystemConfigurationPtr->normOppInterval;
  setup->mem.numSparTrans = TotalSystemConfigurationPtr->numSparingTransactions;

  if (TotalSystemConfigurationPtr->mdllSden == MDLL_SDEN_EN){
    setup->mem.options |= MDLL_SHUT_DOWN_EN;
  } else if((TotalSystemConfigurationPtr->mdllSden == MDLL_SDEN_AUTO) && (MDLL_SDEN_DEFAULT == MDLL_SDEN_EN)) {
    setup->mem.options |= MDLL_SHUT_DOWN_EN;
  } else {
    setup->mem.options &= ~MDLL_SHUT_DOWN_EN;
  }


  if (TotalSystemConfigurationPtr->earlyCmdTraining == EARLY_CMD_CLK_EN){
    setup->mem.optionsExt |= EARLY_CMD_CLK_TRAINING_EN;
  } else if((TotalSystemConfigurationPtr->earlyCmdTraining == EARLY_CMD_CLK_AUTO) && (EARLY_CMD_CLK_DEFAULT == EARLY_CMD_CLK_EN)) {
    setup->mem.optionsExt |= EARLY_CMD_CLK_TRAINING_EN;
  } else {
    setup->mem.optionsExt &= ~EARLY_CMD_CLK_TRAINING_EN;
  }

  if (TotalSystemConfigurationPtr->earlyCtlTraining == EARLY_CTL_CLK_EN){
    setup->mem.optionsExt |= EARLY_CTL_CLK_TRAINING_EN;
  } else if((TotalSystemConfigurationPtr->earlyCtlTraining == EARLY_CTL_CLK_AUTO) && (EARLY_CTL_CLK_DEFAULT == EARLY_CTL_CLK_EN)) {
    setup->mem.optionsExt |= EARLY_CTL_CLK_TRAINING_EN;
  } else {
    setup->mem.optionsExt &= ~EARLY_CTL_CLK_TRAINING_EN;
  }

  if (TotalSystemConfigurationPtr->cmdNormalize == CMD_NORMAL_ENABLE){
    setup->mem.optionsExt |= CMD_NORMAL_EN;
  } else if (TotalSystemConfigurationPtr->cmdNormalize == CMD_NORMAL_AUTO) {
    if (IsBDXEP(host)) {
      if (CMD_NORMAL_DEFAULT == CMD_NORMAL_ENABLE) {
        setup->mem.optionsExt |= CMD_NORMAL_EN;
      } else {
        setup->mem.optionsExt &= ~CMD_NORMAL_EN;
      }
    } else {
      setup->mem.optionsExt &= ~CMD_NORMAL_EN;
    }
  } else {
    setup->mem.optionsExt &= ~CMD_NORMAL_EN;
  }


  if (TotalSystemConfigurationPtr->txEqCalibration == TX_EQ_ENABLE){
    setup->mem.optionsExt |= TX_EQ_EN;
  } else if (TotalSystemConfigurationPtr->txEqCalibration == TX_EQ_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) { //BDX
      if (TX_EQ_DEFAULT== TX_EQ_ENABLE) {
        setup->mem.optionsExt |= TX_EQ_EN;
      } else {
        setup->mem.optionsExt &= ~TX_EQ_EN; 
      }
    } else { //HSX
      if (TX_EQ_DEFAULT_HSX == TX_EQ_ENABLE) {
        setup->mem.optionsExt |= TX_EQ_EN;
      } else {
        setup->mem.optionsExt &= ~TX_EQ_EN; 
      }
    }
  } else {
    setup->mem.optionsExt &= ~TX_EQ_EN;
  }

  if (TotalSystemConfigurationPtr->iModeTraining == IMODE_ENABLE){
    setup->mem.optionsExt |= IMODE_EN;
  } else if (TotalSystemConfigurationPtr->iModeTraining == IMODE_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) { //BDX
      if (IMODE_DEFAULT == IMODE_ENABLE) {
        setup->mem.optionsExt |= IMODE_EN;
      } else {
        setup->mem.optionsExt &= ~IMODE_EN;
      }
    } else { //HSX
	  setup->mem.optionsExt &= ~IMODE_EN;
    }
  } else {
    setup->mem.optionsExt &= ~IMODE_EN;
  }

  if (TotalSystemConfigurationPtr->trainrxctle == RX_CTLE_ENABLE){
    setup->mem.optionsExt |= RX_CTLE_TRN_EN;
  } else if (TotalSystemConfigurationPtr->trainrxctle == RX_CTLE_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) { //BDX
      if (RX_CTLE_DEFAULT == RX_CTLE_ENABLE) {
        setup->mem.optionsExt |= RX_CTLE_TRN_EN;
      } else {
        setup->mem.optionsExt &= ~RX_CTLE_TRN_EN;
      }
    } else { //HSX
      setup->mem.optionsExt &= ~RX_CTLE_TRN_EN;
    }
  } else {
    setup->mem.optionsExt &= ~RX_CTLE_TRN_EN;
  }

  if (TotalSystemConfigurationPtr->RoundTripLatency == ROUND_TRIP_LATENCY_ENABLE){
    setup->mem.optionsExt |= ROUND_TRIP_LATENCY_EN;
  } else if((TotalSystemConfigurationPtr->RoundTripLatency == ROUND_TRIP_LATENCY_AUTO) && (ROUND_TRIP_LATENCY_DEFAULT == ROUND_TRIP_LATENCY_ENABLE)) {
    setup->mem.optionsExt |= ROUND_TRIP_LATENCY_EN;
  } else {
    setup->mem.optionsExt &= ~ROUND_TRIP_LATENCY_EN;
  }
  
  //
  // AptioV Server Override Start : Set the DutyCycleTraining to enabled when it is Auto and default is Enable.
  //
  if ((TotalSystemConfigurationPtr->DutyCycleTraining == DUTY_CYCLE_ENABLE) ||\
      ((TotalSystemConfigurationPtr->DutyCycleTraining == DUTY_CYCLE_AUTO) && (DUTY_CYCLE_DEFAULT == DUTY_CYCLE_ENABLE ))) {
    setup->mem.optionsExt |= DUTY_CYCLE_EN;
  } else {
    setup->mem.optionsExt &= ~DUTY_CYCLE_EN;
  }
  //
  // AptioV Server Override End : Set the DutyCycleTraining to enabled when it is Auto and default is Enable.
  //

  if (TotalSystemConfigurationPtr->advancedClkTraining == CMD_CLK_EN){
    setup->mem.options |= CMD_CLK_TRAINING_EN;
  } else if((TotalSystemConfigurationPtr->advancedClkTraining == CMD_CLK_AUTO) && (CMD_CLK_DEFAULT == CMD_CLK_EN)) {
    setup->mem.options |= CMD_CLK_TRAINING_EN;
  } else {
    setup->mem.options &= ~CMD_CLK_TRAINING_EN;
  }

  //
  // Perbit Deskew Training
  //
  if (TotalSystemConfigurationPtr->perBitDeskew == BIT_DESKEW_DISABLE){
    setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
  } else if(TotalSystemConfigurationPtr->perBitDeskew == BIT_DESKEW_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (IsBDXEP(host)) { //ML
        if (BIT_DESKEW_DEFAULT_BDX == BIT_DESKEW_ENABLE) {
          setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
        } else {
          setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
        }
      } else {// DE - Enable for V0 and above
        if ((BIT_DESKEW_DEFAULT_DE == BIT_DESKEW_ENABLE) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX)) {
          setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
        } else {
          setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
        }
      }
    } else { //HSX
      if (BIT_DESKEW_DEFAULT == BIT_DESKEW_ENABLE) {
        setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
      } else {
        setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
      }
    }
  } else {
    setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
  }
  
  setup->mem.logParsing = TotalSystemConfigurationPtr->logParsing;
  if (TotalSystemConfigurationPtr->readVrefCenter == RX_VREF_ENABLE){
    setup->mem.optionsExt |= RD_VREF_EN;
  } else if((TotalSystemConfigurationPtr->readVrefCenter == RX_VREF_AUTO) && (RX_VREF_DEFAULT == RX_VREF_ENABLE)) {
    setup->mem.optionsExt |= RD_VREF_EN;
  } else {
    setup->mem.optionsExt &= ~RD_VREF_EN;
  }

  //
  // PDA
  //
  if ((TotalSystemConfigurationPtr->pda == PDA_ENABLE) ||
      ((TotalSystemConfigurationPtr->pda == PDA_AUTO) && (PDA_DEFAULT == PDA_ENABLE))) {
    setup->mem.optionsExt |= PDA_EN;
  } else {
    setup->mem.optionsExt &= ~PDA_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if ((TotalSystemConfigurationPtr->turnaroundOpt == TURNAROUND_ENABLE) ||
      ((TotalSystemConfigurationPtr->turnaroundOpt == TURNAROUND_AUTO) && (TURNAROUND_DEFAULT == TURNAROUND_ENABLE))) {
    setup->mem.optionsExt |= TURNAROUND_OPT_EN;
  } else {
    setup->mem.optionsExt &= ~TURNAROUND_OPT_EN;
  }

  //
  // IOT MEMORY BUFFER RESERVATION
  //
  setup->mem.iotMemBufferRsvtn = TotalSystemConfigurationPtr->IotMemBufferRsvtn;

#ifdef SSA_FLAG
  //
  // Enabling the BIOS SSA loader
  //
  if (TotalSystemConfigurationPtr->EnableBiosSsaLoader == BIOS_SSA_LOADER_ENABLE) {
    setup->mem.enableBiosSsaLoader = BIOS_SSA_LOADER_ENABLE;
  } else if ((TotalSystemConfigurationPtr->EnableBiosSsaLoader == BIOS_SSA_DEFAULT) && (BIOS_SSA_DEFAULT == BIOS_SSA_LOADER_ENABLE)) {
    setup->mem.enableBiosSsaLoader = BIOS_SSA_LOADER_ENABLE;
  } else {
    setup->mem.enableBiosSsaLoader = BIOS_SSA_LOADER_DISABLE;
  }
#endif  //SSA_FLAG

  //
  // Enabling the Training Result Offset Option
  //
  if (TotalSystemConfigurationPtr->TrainingResultOffsetFunctionEnable == TRNG_RESULT_OFFSET_ENABLE) {
    setup->mem.trainingResultOffsetFunctionEnable = TRNG_RESULT_OFFSET_ENABLE;
  } else if ((TotalSystemConfigurationPtr->TrainingResultOffsetFunctionEnable == TRNG_RESULT_OFFSET_DEFAULT) && (TRNG_RESULT_OFFSET_DEFAULT == TRNG_RESULT_OFFSET_ENABLE)) {
    setup->mem.trainingResultOffsetFunctionEnable = TRNG_RESULT_OFFSET_ENABLE;
  } else {
    setup->mem.trainingResultOffsetFunctionEnable = TRNG_RESULT_OFFSET_DISABLE;
  }

  setup->mem.offsetTxDq = TotalSystemConfigurationPtr->OffsetTxDq; //default   = 0
  setup->mem.offsetRxDq = TotalSystemConfigurationPtr->OffsetRxDq; //default   = 0
  setup->mem.offsetTxVref = TotalSystemConfigurationPtr->OffsetTxVref; //default   = 0
  setup->mem.offsetRxVref = TotalSystemConfigurationPtr->OffsetRxVref; //default   = 0
  setup->mem.offsetCmdAll = TotalSystemConfigurationPtr->OffsetCmdAll; //default   = 0
  setup->mem.offsetCmdVref = TotalSystemConfigurationPtr->OffsetCmdVref; //default   = 0
  setup->mem.offsetCtlAll = TotalSystemConfigurationPtr->OffsetCtlAll; //default   = 0

#ifdef LRDIMM_SUPPORT
  //
  // Enable BacksideRMT
  //
  if (TotalSystemConfigurationPtr->EnableBacksideRMT == BACKSIDE_RMT_ENABLE) {
    setup->mem.enableBacksideRMT = BACKSIDE_RMT_ENABLE;
  } else if((TotalSystemConfigurationPtr->EnableBacksideRMT == BACKSIDE_RMT_AUTO) && (BACKSIDE_RMT_DEFAULT == BACKSIDE_RMT_ENABLE)) {
    setup->mem.enableBacksideRMT = BACKSIDE_RMT_ENABLE;
  } else {
    setup->mem.enableBacksideRMT = BACKSIDE_RMT_DISABLE;
  }
#endif  //LRDIMM_SUPPORT

  if (TotalSystemConfigurationPtr->wrVrefCenter == TX_VREF_ENABLE){
    setup->mem.optionsExt |= WR_VREF_EN;
  } else if((TotalSystemConfigurationPtr->wrVrefCenter == TX_VREF_AUTO) && (TX_VREF_DEFAULT == TX_VREF_ENABLE)) {
    setup->mem.optionsExt |= WR_VREF_EN;
  } else {
    setup->mem.optionsExt &= ~WR_VREF_EN;
  }

  //
  // Command Vref Training(BDX Only)
  //
  if (TotalSystemConfigurationPtr->cmdVrefCenter == CMD_VREF_ENABLE) {
    setup->mem.optionsExt |= CMD_VREF_EN;
  } else if (TotalSystemConfigurationPtr->cmdVrefCenter == CMD_VREF_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (CMD_VREF_DEFAULT == CMD_VREF_ENABLE) {
        setup->mem.optionsExt |= CMD_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~CMD_VREF_EN;
      }
    } else { //HSX // Always disabled for HSX
      setup->mem.optionsExt &= ~CMD_VREF_EN;
    }
  } else {
    setup->mem.optionsExt &= ~CMD_VREF_EN;
  }

  //
  // LRDIMM Backside Vref Training
  //
  if (TotalSystemConfigurationPtr->lrdimmbacksideVref == LRDIMM_BACKSIDE_VREF_ENABLE) {
    setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
  } else if (TotalSystemConfigurationPtr->lrdimmbacksideVref == LRDIMM_BACKSIDE_VREF_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (IsBDXEP(host)) { //ML
        if (LRDIMM_BACKSIDE_VREF_DEFAULT_BDX == LRDIMM_BACKSIDE_VREF_ENABLE) {
          setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
        } else {
          setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
        }
      } else {// DE
        setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
      }
    } else { //HSX
      if (LRDIMM_BACKSIDE_VREF_DEFAULT == LRDIMM_BACKSIDE_VREF_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
      }    
    }
  } else {
    setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
  }

  //
  // LRDIMM WR VREF
  //
  if (TotalSystemConfigurationPtr->lrdimmWrVref == LRDIMM_WR_VREF_ENABLE) {
    setup->mem.optionsExt |= LRDIMM_WR_VREF_EN;
  } else if (TotalSystemConfigurationPtr->lrdimmWrVref == LRDIMM_WR_VREF_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (LRDIMM_WR_VREF_DEFAULT == LRDIMM_WR_VREF_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_WR_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_WR_VREF_EN;
      }
    } else { //HSX
      setup->mem.optionsExt &= ~LRDIMM_WR_VREF_EN;
    }
  } else {
    setup->mem.optionsExt &= ~LRDIMM_WR_VREF_EN;
  }
  
  //
  // LRDIMM RD VREF
  //
  if (TotalSystemConfigurationPtr->lrdimmRdVref == LRDIMM_RD_VREF_ENABLE) {
    setup->mem.optionsExt |= LRDIMM_RD_VREF_EN;
  } else if (TotalSystemConfigurationPtr->lrdimmRdVref == LRDIMM_RD_VREF_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (LRDIMM_RD_VREF_DEFAULT == LRDIMM_RD_VREF_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_RD_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_RD_VREF_EN;
      }
    } else { //HSX
      setup->mem.optionsExt &= ~LRDIMM_RD_VREF_EN;
    }
  } else {
    setup->mem.optionsExt &= ~LRDIMM_RD_VREF_EN;
  }

  //
  // LRDIMM RX DQ CENTERING
  //
  if (TotalSystemConfigurationPtr->lrdimmRxDqCentering == LRDIMM_RX_DQ_CENTERING_ENABLE) {
    setup->mem.optionsExt |= LRDIMM_RX_DQ_CENTERING;
  } else if (TotalSystemConfigurationPtr->lrdimmRxDqCentering == LRDIMM_RX_DQ_CENTERING_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (LRDIMM_RX_DQ_CENTERING_DEFAULT == LRDIMM_RX_DQ_CENTERING_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_RX_DQ_CENTERING;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_RX_DQ_CENTERING;
      }
    } else { //HSX
      setup->mem.optionsExt &= ~LRDIMM_RX_DQ_CENTERING;
    }
  } else {
    setup->mem.optionsExt &= ~LRDIMM_RX_DQ_CENTERING;
  }

  //
  // LRDIMM TX DQ CENTERING
  //
  if (TotalSystemConfigurationPtr->lrdimmTxDqCentering == LRDIMM_TX_DQ_CENTERING_ENABLE) {
    setup->mem.optionsExt |= LRDIMM_TX_DQ_CENTERING;
  } else if (TotalSystemConfigurationPtr->lrdimmTxDqCentering == LRDIMM_TX_DQ_CENTERING_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (LRDIMM_TX_DQ_CENTERING_DEFAULT == LRDIMM_TX_DQ_CENTERING_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_TX_DQ_CENTERING;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_TX_DQ_CENTERING;
      }
    } else { //HSX
      setup->mem.optionsExt &= ~LRDIMM_TX_DQ_CENTERING;
    }
  } else {
    setup->mem.optionsExt &= ~LRDIMM_TX_DQ_CENTERING;
  }

  //
  // Periodic RCOMP type
  //
  if (TotalSystemConfigurationPtr->rcompType == RCOMP_TYPE_HW) {
    setup->mem.optionsExt |= RCOMP_TYPE;
  } else if (TotalSystemConfigurationPtr->rcompType == RCOMP_TYPE_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {
      if ((RCOMP_TYPE_DEFAULT_BDX == RCOMP_TYPE_HW) && (!(IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX)))) {
        setup->mem.optionsExt |= RCOMP_TYPE;
      } else {
        setup->mem.optionsExt &= ~RCOMP_TYPE;
      }
    } else {
      if (RCOMP_TYPE_DEFAULT == RCOMP_TYPE_HW) {
        setup->mem.optionsExt |= RCOMP_TYPE;
      } else {
        setup->mem.optionsExt &= ~RCOMP_TYPE;
      }
    }
  } else {
    setup->mem.optionsExt &= ~RCOMP_TYPE;
  }

#ifdef MARGIN_CHECK
  //
  // Perbit margin
  //
  if (TotalSystemConfigurationPtr->perbitmargin == PER_BIT_MARGIN_ENABLE){
    setup->mem.optionsExt |= PER_BIT_MARGINS;
  } else if(TotalSystemConfigurationPtr->perbitmargin == PER_BIT_MARGIN_AUTO) {
    if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (IsBDXEP(host)) { //ML
        if (PER_BIT_MARGIN_DEFAULT_BDX == PER_BIT_MARGIN_ENABLE) {
          setup->mem.optionsExt |= PER_BIT_MARGINS;
        } else {
          setup->mem.optionsExt &= ~PER_BIT_MARGINS;
        }
      } else {// DE - Enable for V0 and above
        if ((PER_BIT_MARGIN_DEFAULT_DE == PER_BIT_MARGIN_ENABLE) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX)) {
          setup->mem.optionsExt |= PER_BIT_MARGINS;
        } else {
          setup->mem.optionsExt &= ~PER_BIT_MARGINS;
        }
      }
    } else { //HSX
      if (PER_BIT_MARGIN_DEFAULT == PER_BIT_MARGIN_ENABLE) {
        setup->mem.optionsExt |= PER_BIT_MARGINS;
      } else {
        setup->mem.optionsExt &= ~PER_BIT_MARGINS;
      }
    }
  } else {
    setup->mem.optionsExt &= ~PER_BIT_MARGINS;
  }

  setup->mem.rmtPatternLength = TotalSystemConfigurationPtr->rmtPatternLength;
  setup->mem.rmtPatternLengthExt = TotalSystemConfigurationPtr->rmtPatternLengthExt;
#endif
  setup->mem.check_pm_sts = TotalSystemConfigurationPtr->check_pm_sts;
  setup->mem.check_platform_detect = TotalSystemConfigurationPtr->check_platform_detect;
#ifdef MEM_NVDIMM_EN
  setup->mem.eraseArmNVDIMMS = TotalSystemConfigurationPtr->eraseArmNVDIMMS;
  setup->mem.restoreNVDIMMS = TotalSystemConfigurationPtr->restoreNVDIMMS;
  if (TotalSystemConfigurationPtr->ADRDataSaveMode == ADR_NVDIMM) {
#ifndef GRANGEVILLE_FLAG
    setup->mem.check_pm_sts = 0;
#endif
    setup->mem.check_platform_detect = 0;
  }
#endif
#ifdef NVMEM_FEATURE_EN
  setup->mem.interNVDIMMS = TotalSystemConfigurationPtr->interNVDIMMS;
#endif

  //
  // common options
  //
  setup->common.options &= ~(PROMOTE_MRC_WARN_EN | HALT_ON_ERROR_EN | PROMOTE_WARN_EN);

  if (TotalSystemConfigurationPtr->promoteMrcWarnings)  setup->common.options |= PROMOTE_MRC_WARN_EN;
  if (TotalSystemConfigurationPtr->haltOnMemErr)        setup->common.options |= HALT_ON_ERROR_EN;
  if (TotalSystemConfigurationPtr->promoteWarnings)     setup->common.options |= PROMOTE_WARN_EN;

  //
  // thermal throttling options
  //
  setup->mem.thermalThrottlingOptions &= ~(OLTT_EN | CLTT_EN | MH_OUTPUT_EN | MH_SENSE_EN);

  if (TotalSystemConfigurationPtr->thermalthrottlingsupport == 2) {
    setup->mem.thermalThrottlingOptions |= CLTT_EN;
  } else if (TotalSystemConfigurationPtr->thermalthrottlingsupport == 1) {
    setup->mem.thermalThrottlingOptions |= OLTT_EN;
  }

  //AptioV server override start: OLTT support on DDR3 platform 
  //
  // OLTT Peak Bandwidth limit
  //
  if (setup->mem.thermalThrottlingOptions & OLTT_EN) {
//    setup->mem.olttPeakBWLIMITPercent = TotalSystemConfigurationPtr->OLTTPeakBWLimitPercent;
  }  
  //AptioV server override end: OLTT support on DDR3 platform 
  
  //
  // mem hot options
  //
  if (TotalSystemConfigurationPtr->memhotSupport == MEMHOT_OUTPUT_ONLY) {
    setup->mem.thermalThrottlingOptions |= MH_OUTPUT_EN;
  } else if (TotalSystemConfigurationPtr->memhotSupport == MEMHOT_INPUT_ONLY) {
    setup->mem.thermalThrottlingOptions |= MH_SENSE_EN;
  }

  //
  // Electrical throttling options
  //
  setup->mem.electricalThrottling = TotalSystemConfigurationPtr->ElectricalThrottlingMode;

  //
  // Force LRDIMM Multiplication
  //
  setup->mem.forceRankMult = TotalSystemConfigurationPtr->lrdimmRankMult;

  //
  // LRDIMM Module delay
  //
  setup->mem.lrdimmModuleDelay = TotalSystemConfigurationPtr->lrdimmModuleDelay;

  //
  // MEM_PWR_SAVE
  //
  //
  //BDX changes to disable APD,PPD when PERF mode enabled.
  //
  setup->mem.MemPwrSave = TotalSystemConfigurationPtr->MemPwrSave;
 
  if (host->var.common.cpuType == CPU_HSX) {
    //
    // PPD is not supported on HSX
    // The only valid options are disabled and User mode
    //
    if ((setup->mem.MemPwrSave == MEM_PWR_SAV_SLOW) || (setup->mem.MemPwrSave == MEM_PWR_SAV_FAST)) {
      //
      // Change to user mode if set to an invalid setting
      //
      setup->mem.MemPwrSave = MEM_PWR_SAV_APD;
    }
  }

  if (host->var.common.cpuType == CPU_BDX) {
	  if (TotalSystemConfigurationPtr->AltEngPerfBIAS == ENERGY_PERF_MODE) {
		  setup->mem.MemPwrSave = MEM_PWR_SAV_DISABLE; //disable PPD,APD when PERF mode is enabled.
	  }
  }
  //
  // DDR4 SMB CLK
  //
  if (TotalSystemConfigurationPtr->smbSpeed){
    setup->mem.SpdSmbSpeed = SMB_CLK_1M;
  } else {
    setup->mem.SpdSmbSpeed = SMB_CLK_400K;
  }

  //
  // Host RAS Modes
  //
  setup->mem.RASmode &= ~(CH_MLS);
  setup->mem.spareErrTh = TotalSystemConfigurationPtr->spareErrTh;
  if (TotalSystemConfigurationPtr->RASMode == LOCKSTEP_ENABLED)     setup->mem.RASmode |= CH_LOCKSTEP;

  if (TotalSystemConfigurationPtr->enableSparing == SPARING_ENABLED){
    setup->mem.RASmode |= RK_SPARE;
    setup->mem.spareRanks = TotalSystemConfigurationPtr->multiSparingRanks;
  } else if (TotalSystemConfigurationPtr->RASMode == MIRROR_ENABLED){
    setup->mem.RASmode |= CH_MIRROR;
  }

  //
  // Host Extended RAS Modes
  //
  if (TotalSystemConfigurationPtr->DemandScrubMode == DEMAND_SCRUB_DIS) {
    setup->mem.RASmodeEx &= ~DMNDSCRB_EN;
  } else {
    setup->mem.RASmodeEx |= DMNDSCRB_EN;
  }

  if (TotalSystemConfigurationPtr->PatrolScrub == PATROL_SCRUB_DIS){
    setup->mem.RASmodeEx &= ~PTRLSCRB_EN;
  } else {
    setup->mem.RASmodeEx |= PTRLSCRB_EN;
    setup->mem.patrolScrubDuration = TotalSystemConfigurationPtr->PatrolScrubDuration;
  }

  if (TotalSystemConfigurationPtr->a7Mode){
    setup->mem.RASmodeEx |= A7_MODE_EN;
  } else {
    setup->mem.RASmodeEx &= ~A7_MODE_EN;
  }

  if (TotalSystemConfigurationPtr->DeviceTaggingMode) {
    setup->mem.RASmodeEx |= DEVTAGGING_EN;
  } else {
    setup->mem.RASmodeEx &= ~DEVTAGGING_EN;
  }

  //
  // Leaky bucket configuration options
  //
  setup->mem.leakyBktHi = TotalSystemConfigurationPtr->leakyBktHi;
  setup->mem.leakyBktLo = TotalSystemConfigurationPtr->leakyBktLo;

  //
  // Lockstep x4 DIMMs Disable Option
  //
  if (TotalSystemConfigurationPtr->lockstepEnableX4){
    setup->mem.lockstepEnableX4 = LOCKSTEPX4_ENABLE;
  } else {
    setup->mem.lockstepEnableX4 = LOCKSTEPX4_DISABLE;
  }

  //
  // Rank Disable Mask
  //
  setup->mem.socket[0].ddrCh[0].rankmask = TotalSystemConfigurationPtr->sck0ch0;
  setup->mem.socket[0].ddrCh[1].rankmask = TotalSystemConfigurationPtr->sck0ch1;
  setup->mem.socket[0].ddrCh[2].rankmask = TotalSystemConfigurationPtr->sck0ch2;
  setup->mem.socket[0].ddrCh[3].rankmask = TotalSystemConfigurationPtr->sck0ch3;
  setup->mem.socket[1].ddrCh[0].rankmask = TotalSystemConfigurationPtr->sck1ch0;
  setup->mem.socket[1].ddrCh[1].rankmask = TotalSystemConfigurationPtr->sck1ch1;
  setup->mem.socket[1].ddrCh[2].rankmask = TotalSystemConfigurationPtr->sck1ch2;
  setup->mem.socket[1].ddrCh[3].rankmask = TotalSystemConfigurationPtr->sck1ch3;
  setup->mem.socket[2].ddrCh[0].rankmask = TotalSystemConfigurationPtr->sck2ch0;
  setup->mem.socket[2].ddrCh[1].rankmask = TotalSystemConfigurationPtr->sck2ch1;
  setup->mem.socket[2].ddrCh[2].rankmask = TotalSystemConfigurationPtr->sck2ch2;
  setup->mem.socket[2].ddrCh[3].rankmask = TotalSystemConfigurationPtr->sck2ch3;
  setup->mem.socket[3].ddrCh[0].rankmask = TotalSystemConfigurationPtr->sck3ch0;
  setup->mem.socket[3].ddrCh[1].rankmask = TotalSystemConfigurationPtr->sck3ch1;
  setup->mem.socket[3].ddrCh[2].rankmask = TotalSystemConfigurationPtr->sck3ch2;
  setup->mem.socket[3].ddrCh[3].rankmask = TotalSystemConfigurationPtr->sck3ch3;

  //
  // Update rank mapout data based on rankMask option
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      for (rank = 0; rank < MAX_RANK_CH; rank++) {
        //
        // Convert logical rank to Phy. Rank, DIMM Slot# and DIMM rank#
        // Log. Rank:       0/1/2/3/4/5/6/7
        // 2DPC Phy. Rank:  0/1/2/3/4/5/6/7
        // 3DPC Phy. Rank:  0/1/2/3/4/5/8/9
        //
        if (host->nvram.mem.socket[socket].channelList[ch].maxDimm == 3 && rank > 5) {
          phyRank = 2 + rank;
        } else {
          phyRank = rank;
        }
        dimmRank = phyRank % MAX_RANK_DIMM;
        dimm     = phyRank / MAX_RANK_DIMM;
        //DEBUG ((EFI_D_INFO, "   %d           %d         %d", phyRank, dimmRank, dimm));

        //
        // If rank is not enabled in rankMask, then  flag the rank as
        // mapped out
        //
        if (!(setup->mem.socket[socket].ddrCh[ch].rankmask & (1 << rank))) {
          setup->mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[dimmRank] = 1;
        } else {
          setup->mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[dimmRank] = 0;
        }
      } // rank
    } // ch
  } // socket



  // Custom Refresh Rate
  if (TotalSystemConfigurationPtr->CustomRefreshRateEn) {
    setup->mem.customRefreshRate = TotalSystemConfigurationPtr->CustomRefreshRate;
  } else {
    setup->mem.customRefreshRate = 0;
  }

  if (setup->mem.optionsExt & CECC_WA) {
    setup->mem.ceccWaChMask = TotalSystemConfigurationPtr->ceccWaChMask;
  } else {
    setup->mem.ceccWaChMask = 0;
  }
}

VOID
OemGetPlatformSetupValueforQPI(
  struct sysHost             *host,
  SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
  )
/*++
--*/
{
  EFI_PEI_SERVICES            **PeiServices;
  struct sysSetup             *setup;
  QPI_CPU_PHY_SETTING         *pQpiCpuPhy;
  QPI_CPU_LINK_SETTING        *pQpiCpuLink;
  UINT8                       *PerPortOption;
  UINT8                       Ctr1, Ctr2;
  BOOLEAN                     ForceTo1SConfigMode = FALSE;

  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  setup = (struct sysSetup *)(&host->setup);

  setup->qpi.LegacyVgaSoc     = TotalSystemConfigurationPtr->LegacyVgaSoc;
  setup->qpi.MmioP2pDis       = TotalSystemConfigurationPtr->MmioP2pDis;
  setup->qpi.IsocAzaliaVc1En  = TotalSystemConfigurationPtr->IsocAzaliaVc1En;
  setup->qpi.DebugPrintLevel  = TotalSystemConfigurationPtr->DebugPrintLevel;
  setup->qpi.QpiLinkSpeedMode = TotalSystemConfigurationPtr->QpiLinkSpeedMode;
  setup->qpi.DegradePrecedence= TotalSystemConfigurationPtr->DegradePrecedence;

  //
  // Wildcat Pass does not have a QPI_SLOW jumper.  GPIO33 is unused (stuff option),
  // and they forgot to add a pullup.  Skip the test on this platform.
  //
#ifndef GRANGEVILLE_FLAG

  switch (OemGetPlatformType(host)) {
    case TypeKahunaDVP2SPbg:
    case TypeKahunaDVP2S:
    case TypeWildcatPass:
     break;

  default:
      if (!(IoRead32(PCH_GPIO_BASE_ADDRESS + 0x38) & 0x00002)) {
        // When the QPI_SLOW_MODE jumper is set (this is actually FORCE_1S_MODE jumper), run as a 1S config.
        // Note: This jumper is straped to Vcc in Kahuna board with connections to PLD and is ignored.
        ForceTo1SConfigMode = TRUE;
        DEBUG((EFI_D_ERROR, "QPI_SLOW_MODE (FORCE_1S_MODE) jumper is set : Disabling all the QPI links to run as 1S configuration\n"));
      } else {
        ForceTo1SConfigMode = FALSE;
      } // IoRead32 check
      break;
  } // platform check
#endif

  setup->qpi.QpiLinkSpeed     = TotalSystemConfigurationPtr->QpiLinkSpeed;
  setup->qpi.QpiLinkL0pEn     = TotalSystemConfigurationPtr->QpiLinkL0pEn;
  setup->qpi.QpiLinkL1En      = TotalSystemConfigurationPtr->QpiLinkL1En;
  setup->qpi.QpiLinkL0rEn     = TotalSystemConfigurationPtr->QpiLinkL0rEn;
  setup->qpi.QpiLbEn          = TotalSystemConfigurationPtr->QpiLbEn;
  for (Ctr1=0; Ctr1<MAX_CPU_SOCKETS; ++Ctr1){
    setup->qpi.IioUniphyDisable[Ctr1] = TotalSystemConfigurationPtr->IioUniphyDisable[Ctr1];
    setup->qpi.BusRatio[Ctr1] = TotalSystemConfigurationPtr->BusRatio[Ctr1];
  }
  setup->qpi.IBPECIEn    = TotalSystemConfigurationPtr->IBPECIEn;
  setup->qpi.QpiCrcMode       = TotalSystemConfigurationPtr->QpiCrcMode;

  if (TotalSystemConfigurationPtr->EarlySnoopEn >= 2) { // 'Auto' treated as 'Enable' for HSX and BDX
    if ((host->var.common.cpuType == CPU_HSX) || (host->var.common.cpuType == CPU_BDX)) {
      setup->qpi.EarlySnoopEn = 1;  // Enable EarlySnoop for HSX and BDX
    } else {
      setup->qpi.EarlySnoopEn = 0;  // Disable EarlySnoop for non-HSX and non-BDX. This will never happen.
    }
  } else {
    setup->qpi.EarlySnoopEn = TotalSystemConfigurationPtr->EarlySnoopEn;
  }

  if (TotalSystemConfigurationPtr->ClusterOnDieEn >= 2) {
    setup->qpi.ClusterOnDieEn = 0;  // Disable ClusterOnDieEn by default
  } else {
    setup->qpi.ClusterOnDieEn = TotalSystemConfigurationPtr->ClusterOnDieEn;
  }

#ifndef GRANGEVILLE_FLAG
  if (TotalSystemConfigurationPtr->HomeDirWOSBEn >= 2) {
    if (host->var.common.cpuType == CPU_BDX) {
      if ((host->nvram.common.platformType == TypeAztecCityCRBSRP) ||
          (host->nvram.common.platformType == TypeAztecCitySTHI) ||
          (host->nvram.common.platformType == TypeHedtSKU1CRB) ||
          (host->nvram.common.platformType == TypeHedtEv) ||
          (host->nvram.common.platformType == TypeHedtDDR3)) {
        setup->qpi.HomeDirWOSBEn = 0;  // Disable Home Dir w/ IVT Style OSB for BDX- in Aztec and Hedt platforms
      } else {
        setup->qpi.HomeDirWOSBEn = 1;  // Enable Home Dir w/ IVT Style OSB for BDX- in others server platforms
      } // platform check
    } else { // Disable when HSX- CPUs are used
      setup->qpi.HomeDirWOSBEn = 0;
    } // cpuType check
  } else {
    setup->qpi.HomeDirWOSBEn = TotalSystemConfigurationPtr->HomeDirWOSBEn;
  } // HomeDirWOSBEn
#endif

  setup->qpi.E2EParityEn = TotalSystemConfigurationPtr->E2EParityEn;

  setup->qpi.QpiCpuSktHotPlugEn           = TotalSystemConfigurationPtr->QpiCpuSktHotPlugEn;
  setup->qpi.QpiCpuSktHotPlugTopology     = TotalSystemConfigurationPtr->QpiCpuSktHotPlugTopology;
  setup->qpi.QpiSkuMismatchCheck          = TotalSystemConfigurationPtr->QpiSkuMismatchCheck;
  //
  // System wide option
  //
  setup->qpi.QpiLinkCreditReduce          = TotalSystemConfigurationPtr->QpiLinkCreditReduce;

#ifdef SDP_FLAG
  setup->qpi.SvParm.SvIoDcEn                         = TotalSystemConfigurationPtr->SvIoDcEn;
  setup->qpi.SvParm.SvSnoopMode                      = TotalSystemConfigurationPtr->SvSnoopMode;
  setup->qpi.SvParm.SvHitMeEn                        = TotalSystemConfigurationPtr->SvHitMeEn;
#endif


  //
  // Per port option
  //
  PerPortOption = ((UINT8 *)(&TotalSystemConfigurationPtr->QpiCpuPerPortStartTag) + 1);
  for (Ctr1 = 0; Ctr1 < MAX_CPU_SOCKETS; ++Ctr1) {
    for (Ctr2 = 0; Ctr2 < MAX_QPI_PORTS; ++Ctr2) {

      pQpiCpuLink = (QPI_CPU_LINK_SETTING *) &(host->setup.qpi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2]);
      pQpiCpuPhy = (QPI_CPU_PHY_SETTING *) &(host->setup.qpi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2]);

      if (ForceTo1SConfigMode == TRUE) {
        pQpiCpuLink->QpiPortDisable = TRUE;                                                         // Disable ports of all sockets
      } else {
        pQpiCpuLink->QpiPortDisable = (*(PerPortOption++)) & 0x1;                                   // This option is per-port only
      }

      if (TotalSystemConfigurationPtr->QpiLinkCreditReduce == 2) {                                  // Implies "Per Link Setting"  2 = Per Link
        pQpiCpuLink->QpiLinkCreditReduce = (*(PerPortOption++)) & 0x1;                              // Use per port option
      } else {
        pQpiCpuLink->QpiLinkCreditReduce = TotalSystemConfigurationPtr->QpiLinkCreditReduce & 0x1;  // Use system wide option
        PerPortOption++;
      }

      if (TotalSystemConfigurationPtr->QpiLinkSpeed == FREQ_PER_LINK) {                   // Implies "Per Link Setting" 7 = FREQ_PER_LINK
        pQpiCpuPhy->QpiLinkSpeed = (*(PerPortOption++)) & 0x7;                                      // Use per port option
      } else {
        pQpiCpuPhy->QpiLinkSpeed =  TotalSystemConfigurationPtr->QpiLinkSpeed & 0x7;                // Use system wide option
        PerPortOption++;
      }

      pQpiCpuPhy->QpiProbeType = (*(PerPortOption++)) & 0x3;                                        // This option is per-port only

      if (TotalSystemConfigurationPtr->QpiConfigTxWci == QPI_LINK_WIDTH_PER_LINK) {                // Implies "Per Link Setting" 12 = QPI_LINK_WIDTH_PER_LINK
        pQpiCpuPhy->QpiConfigTxWci = (*(PerPortOption++)) & 0x0F;                                   // Use per port option
      } else {
        pQpiCpuPhy->QpiConfigTxWci = TotalSystemConfigurationPtr->QpiConfigTxWci & 0x0F;            // Use system wide option
        PerPortOption++;
      }
    }
  }
}

//
// Function Implementations
//
VOID
OemGetPlatformSetupValue(
  struct sysHost             *host,
  SYSTEM_CONFIGURATION       *SystemConfigurationPtr
  )
/*++
--*/
{
  EFI_STATUS                            Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr;
  IIO_UDS                               IioUds; // Aptio V Server Override : Fix for Nvram Corruption
  EFI_PEI_SERVICES                      **PeiServices;
  struct sysSetup                       *setup;
#ifdef   SERIAL_DBG_MSG
  UINT8                                 SerialDebugMsgLevelSetting;
#endif // SERIAL_DBG_MSG
  UINT8                           socket;
  UINTN                           Index;
  UINT64                          Alignment;
  UINT64                          Length;
  TCG_SETUP                       TcgSetup;
  UINT8                           MorControl;
  extern EFI_GUID                 gEfiDprRegsProgrammedGuid;
#ifdef   SERIAL_DBG_MSG
  UINT8                             serialDebugMsgLevel;
#endif
  PPR_ADDR_VARIABLE               pprAddrVariable;
  PPR_ADDR_VARIABLE               *pprAddrVariablePtr;
  UINT16                          pprCrc16;
  UINT16                          curCrc16;
#ifndef GRANGEVILLE_FLAG
#ifdef   SERIAL_DBG_MSG
  EFI_BOOT_MODE               BootMode;
  EFI_PEI_SERVICES            **gPeiServices;
#endif // SERIAL_DBG_MSG
#endif

  

  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  TotalSystemConfigurationPtr = SystemConfigurationPtr;
  Status = EFI_SUCCESS;
  if(!TotalSystemConfigurationPtr){
    Status = EFI_ERROR_MAJOR;
  }
  ASSERT_EFI_ERROR (Status);
  ZeroMem(TotalSystemConfigurationPtr, sizeof(SYSTEM_CONFIGURATION) );

  //
  // Locate Variable PPI
  //
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to IntelSetup
  Status = PeiVariable->GetVariable (
                        PeiVariable,
                        L"IntelSetup",
                        &gEfiSetupVariableGuid,
                        NULL,
                        &VariableSize,
                        TotalSystemConfigurationPtr
                        );
  // Aptio V Server Override - Start : Fix for Nvram Corruption
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "GetVariable is failed... so load default values...\n"));
          //
          // Get MRC Setup options
          //
    /* Aptio V Grangeville override
        (*PeiServices)->CopyMem (
                          &(IioUds.SetupInfo.Config),
                          &IioConfigDefault,
                          sizeof (IIO_CONFIG)
                          );
                          */

        (*PeiServices)->CopyMem (
                          &(IioUds.SetupInfo.CommonRcConfig),
                          &CommonRcConfigDefault,
                          sizeof (COMMONRC_CONFIG)
                          );

        (*PeiServices)->CopyMem (
                          &(IioUds.SetupInfo.CsiConfig),
                          &CsiConfigDefault,
                          sizeof (CSI_CONFIG)
                          );

        (*PeiServices)->CopyMem (
                          &(IioUds.SetupInfo.MemoryConfig),
                          &MemoryConfigDefault,
                          sizeof (MEMORY_CONFIG)
                          );
    
        (*PeiServices)->CopyMem (
                          &(IioUds.SetupInfo.MiscConfig),
                          &MiscConfigDefault,
                          sizeof (MISC_CONFIG)
                          );
    
        (*PeiServices)->CopyMem (
                      &(TotalSystemConfigurationPtr->VariableTag),
                      &(IioUds.SetupInfo),
                      sizeof (SETUP_INFO)
                      );

    #ifdef LT_FLAG
    TotalSystemConfigurationPtr->ProcessorSmxEnable = 1;
    TotalSystemConfigurationPtr->TpmEnable = 1;
    #endif   
       
    #ifdef   SERIAL_DBG_MSG
    TotalSystemConfigurationPtr->serialDebugMsgLvl = host->setup.common.serialDebugMsgLvl; // Default was already set elsewhere.
    #endif
  } else {
  // Aptio V Server Override - End : Fix for Nvram Corruption

  //ASSERT_EFI_ERROR(Status);
  //
  // Until the default values of HII VFR forms becomes available, we will keep the QPI RC defaults as from previous programs
  //
  OemGetPlatformSetupValueforQPI(host, (VOID *)TotalSystemConfigurationPtr);
  }   // Aptio V Server Override : Fix for Nvram Corruption  
    
  VariableSize = sizeof (PPR_ADDR_VARIABLE);
  Status = PeiVariable->GetVariable (
                        PeiVariable,
                        PPR_VARIABLE_NAME,
                        &gEfiPprVariableGuid,
                        NULL,
                        &VariableSize,
                        &pprAddrVariable
                        );

  pprCrc16 =  pprAddrVariable.crc16;
  pprAddrVariable.crc16 = 0;
  curCrc16 = Crc16 ((char*)&pprAddrVariable, sizeof(PPR_ADDR_VARIABLE));

  if (EFI_ERROR(Status) || (pprCrc16 != curCrc16)) {
    DEBUG((EFI_D_ERROR, "PPR Variable not found or CRC mismatch - Status: 0x%0x, Crc: 0x%0x, calc. Crc: 0x%0x!\n", Status, pprCrc16, curCrc16));
    pprAddrVariablePtr = NULL;
  } else {
    pprAddrVariablePtr = &pprAddrVariable;
  }
  OemGetPlatformSetupValueforMRC(host, (VOID *)TotalSystemConfigurationPtr, pprAddrVariablePtr);
  OemGetPlatformSetupValueforIIO(host, (VOID *)TotalSystemConfigurationPtr);

// Aptio V Server Override: AMI XTU Performance tuning for CPU support ++ >>
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
  XtuCpuUpdateSystemConfiguration((VOID**)PeiServices, (VOID *)TotalSystemConfigurationPtr);
#endif  
// Aptio V Server Override: tuning for CPU support ++ <<

  //
  // Get MCTP Enable option from setup options
  //
  host->var.common.mctpEn = TotalSystemConfigurationPtr->MctpEn;


  setup = (struct sysSetup *)(&host->setup);

#ifdef   SERIAL_DBG_MSG
  serialDebugMsgLevel = TotalSystemConfigurationPtr->serialDebugMsgLvl;


  //
  // Init input parameters here according to setup options
  //

  switch (serialDebugMsgLevel) {
  case 0:
    // Disabled
    SerialDebugMsgLevelSetting = 0;
    break;
  case 1:
    // Minimum
    SerialDebugMsgLevelSetting = SDBG_MIN;
    break;
  case 2:
    // Normal
    SerialDebugMsgLevelSetting = SDBG_MAX;
    break;
  case 3:
    // Maximum
    SerialDebugMsgLevelSetting = SDBG_MINMAX;
    break;
  default:
    // If the default was set to something else in CommonSetup, preserve it for first boot
    SerialDebugMsgLevelSetting = TotalSystemConfigurationPtr->serialDebugMsgLvl;
    break;
  }

#ifndef GRANGEVILLE_FLAG
  //
  // For AztecCitySTHI, disable all serial messages
  //
  if(OemGetPlatformType(host) == TypeAztecCitySTHI) {

    gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
    Status = (*gPeiServices)->GetBootMode(gPeiServices, &BootMode);
    ASSERT_EFI_ERROR(Status);

    if (BootMode == BOOT_WITH_DEFAULT_SETTINGS ) {
      SerialDebugMsgLevelSetting = 0;
      TotalSystemConfigurationPtr->serialDebugMsgLvlTrainResults = 0;
      TotalSystemConfigurationPtr->serialDebugTrace = 0;
    }
  }
#endif //GRANGEVILLE_FLAG

  setup->common.serialDebugMsgLvl = SerialDebugMsgLevelSetting;
  setup->common.serialDebugMsgLvl |= TotalSystemConfigurationPtr->serialDebugTrace;
  setup->common.serialDebugMsgLvl |= TotalSystemConfigurationPtr->serialDebugMsgLvlTrainResults;

#endif //SERIAL_DBG_MSG


  //
  // HSD 4168370 Add EVMode Support to the HSX EP BIOS
  //

  if (TotalSystemConfigurationPtr->DFXEnable) {
    DEBUG((EFI_D_ERROR, "TotalSystemConfigurationPtr->DFXEnable = %d\n",TotalSystemConfigurationPtr->DFXEnable));
    host->var.common.EVMode = 1;
    TotalSystemConfigurationPtr->ProcessorSmxEnable = 0;
  }

  switch(TotalSystemConfigurationPtr->MmcfgBase){
    case 1:
      setup->common.mmCfgBase = 0x40000000;
      break;
    case 2:
      setup->common.mmCfgBase = 0xC0000000;
      break;
    default:
      setup->common.mmCfgBase = MMCFG_BASE_ADDRESS;
      break;
  }
  switch(TotalSystemConfigurationPtr->MmcfgSize){
    case 1:
      setup->common.mmCfgSize = 128*1024*1024;
      break;
    case 2:
      setup->common.mmCfgSize = 64*1024*1024;
      break;
    default:
      setup->common.mmCfgSize = MMCFG_SIZE;
      break;
  }
  /*switch(TotalSystemConfigurationPtr->MmiolBase){
    case 1:
      setup->common.mmiolBase = 0x50000000;
      break;
    case 2:
      setup->common.mmiolBase = 0xD0000000;
      break;
    default:
      setup->common.mmiolBase = setup->common.mmCfgBase + MMCFG_SIZE;
      break;
  }
  switch(TotalSystemConfigurationPtr->MmiolSize){
    case 1:
      setup->common.mmiolSize = 0xAC000000;
      break;
    case 2:
      setup->common.mmiolSize = 0x2C000000;
      break;
    default:
      setup->common.mmiolSize = MMIOL_LIMIT - setup->common.mmiolBase;
      break;
  }*/
  setup->common.mmiolBase = setup->common.mmCfgBase + MMCFG_SIZE;
  setup->common.mmiolSize = MMIOL_LIMIT - setup->common.mmiolBase + 1;

  switch(TotalSystemConfigurationPtr->MmiohSize){
    case 1:
      setup->common.mmiohSize = 128;
      break;
    case 2:
      setup->common.mmiohSize = 512;
      break;
    case 3:
      setup->common.mmiohSize = 1024;
      break;
    default:
      setup->common.mmiohSize = MMIOH_SIZE;
      break;
  }
  switch(TotalSystemConfigurationPtr->MmiohBase){
    case 1:
      setup->common.mmiohBase = 0x2800;
      break;
    case 2:
      setup->common.mmiohBase = 0x1800;
      break;
// HSD: 4986861 SDP: PCI Devices Mapped above addressable limit (6000036274)
    case 3:   //16T
      setup->common.mmiohBase = 0x1000;
      break;
     //HSD : 5003135 [BDX-EP] System stuck at Memory test failure
     //when set mmioh base = 4T and mmioh size = 1024G <Amazon/Quanta>
    //Providing 12T and 2T to support EXSi5.5 and 5.1 boot respectively
    case 4:  //for 12T
      setup->common.mmiohBase = 0x0C00;
      break;
    case 5:  //4T
      setup->common.mmiohBase = 0x0400;
      break;
    case 6:  //for 2T
      setup->common.mmiohBase = 0x0200;
      break;
    case 7:
	  if (setup->common.mmiohSize > 256) {
	    setup->common.mmiohSize = 256;	// Restrict max size to 256G 
	  }
      setup->common.mmiohBase = 0x0100;
      break;
    default:
      setup->common.mmiohBase = MMIOH_BASE;
      break;
  }
  setup->common.lowGap = FOUR_GB_MEM - (setup->common.mmCfgBase >> 26);
  setup->common.highGap = HIGH_GAP;            // High MMIO gap = 256 MB
  setup->common.maxAddrMem = MAX_MEM_ADDR;     // Supports 46-bit addressing
  //PcdSet64 (PcdPciExpressBaseAddress, (UINT64) (UINTN) setup->common.mmCfgBase); //AptioV server overrride Dynamic mmcfg base address change 

  if ((TotalSystemConfigurationPtr->IioLinkTrainPhase == 0) || (host->var.common.cpuType == CPU_HSX))
    host->var.iio.IioVData.IioLinkTrainPhase = 0;  // Link training in DXE phase
  else
    host->var.iio.IioVData.IioLinkTrainPhase = 1;  // Link training in PEI phase



#ifndef GRANGEVILLE_FLAG
  if (TotalSystemConfigurationPtr->IsocEn >= 2) {  // For 'Auto
    if ((host->nvram.common.platformType == TypeAztecCityCRBSRP) ||
        (host->nvram.common.platformType == TypeAztecCitySTHI) ||
        (host->nvram.common.platformType == TypeHedtSKU1CRB) ||
        (host->nvram.common.platformType == TypeHedtEv) ||
        (host->nvram.common.platformType == TypeHedtDDR3)) {
      // For WS and HEDT platforms, isoc enabled by default
      setup->common.isocEn = 1;
    } else {
      // For server platforms, isoc disabled by default
      setup->common.isocEn = 0;
    } // platform check
  } else {
    setup->common.isocEn = TotalSystemConfigurationPtr->IsocEn;
  } // IsocEn

  if (TotalSystemConfigurationPtr->MeSegEn >= 2) {  // For 'Auto
    if ((host->nvram.common.platformType == TypeAztecCityCRBSRP) ||
        (host->nvram.common.platformType == TypeAztecCitySTHI) ||
        (host->nvram.common.platformType == TypeHedtSKU1CRB) ||
        (host->nvram.common.platformType == TypeHedtEv) ||
        (host->nvram.common.platformType == TypeHedtDDR3)) {
      // For WS and HEDT platforms, meseg enabled by default
      setup->common.mesegEn = 1;
    } else {
      // For server platforms, meseg disabled by default
      setup->common.mesegEn = 0;
    } // platform check
  } else {
    setup->common.mesegEn = TotalSystemConfigurationPtr->MeSegEn;
  } // MesegEn

  //
  // 4986610: ESCALATE from hexaii_hsx_mock:POR:  Isoc not supported on socket B3 (EN segment)
  //
  if (host->var.common.socketType == SOCKET_EN) {
    setup->common.isocEn = 0;
  }
#else
  if (TotalSystemConfigurationPtr->IsocEn >= 2)
    setup->common.isocEn = 0;
  else
    setup->common.isocEn = TotalSystemConfigurationPtr->IsocEn;

  if (TotalSystemConfigurationPtr->MeSegEn >= 2) {
    setup->common.mesegEn = 0;
	
  }	
  else
    setup->common.mesegEn = TotalSystemConfigurationPtr->MeSegEn;
#endif // GRANGEVILLE_FLAG

  setup->common.numaEn = TotalSystemConfigurationPtr->NumaEn;
  //
  //dca enable
  // Default disable for A step, enable for Bstep
  //
  if (TotalSystemConfigurationPtr->DcaEnable == 2) {
    if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping == A0_REV_HSX)) {
      setup->common.dcaEn = 0;
    } else {
      setup->common.dcaEn = 1;
    }
  } else {
    setup->common.dcaEn = TotalSystemConfigurationPtr->DcaEnable;
  }

  //
  //psmienable
  //
  setup->common.PSMIEnabledSupport = TotalSystemConfigurationPtr->PSMISupport;

  //Then CPU
  setup->cpu.debugInterfaceEn = TotalSystemConfigurationPtr->DebugInterface;   // CPU DebugInterface
  setup->cpu.dcuModeSelect = TotalSystemConfigurationPtr->DCUModeSelection;    // CPU DCU Mode select
  setup->cpu.flexRatioEn = TotalSystemConfigurationPtr->ProcessorFlexibleRatioOverrideEnable; // FlexRatio enable
  //
  // Verify that the values being set are within a valid range, if not use default.
  //
  if(TotalSystemConfigurationPtr->ProcessorFlexibleRatio > MAX_PROCESSOR_CORE_RATIO){
    TotalSystemConfigurationPtr->ProcessorFlexibleRatio = DEFAULT_PROCESSOR_CORE_RATIO; // If out of range set the default
  }
  setup->cpu.flexRatioNext = TotalSystemConfigurationPtr->ProcessorFlexibleRatio;        // Common NewRatio for all socket

  setup->cpu.smtEnable = TotalSystemConfigurationPtr->ProcessorHyperThreadingDisable ? 0 : 1;
  setup->cpu.vtEnable = TotalSystemConfigurationPtr->ProcessorVmxEnable;
  setup->cpu.ltEnable = TotalSystemConfigurationPtr->ProcessorSmxEnable;
  setup->cpu.ltsxEnable = TotalSystemConfigurationPtr->ProcessorLtsxEnable;
  setup->cpu.PpinControl = TotalSystemConfigurationPtr->PpinControl;
  setup->cpu.EnableGv = TotalSystemConfigurationPtr->ProcessorEistEnable;
  setup->cpu.ConfigTDPLevel = TotalSystemConfigurationPtr->ConfigTDPLevel;
  setup->cpu.ConfigTDP = TotalSystemConfigurationPtr->ConfigTDP;
  //uncore ratio from setup-start
  setup->cpu.UncoreFreq = TotalSystemConfigurationPtr->UncoreFreq;
  setup->cpu.UncoreFreqOverride = TotalSystemConfigurationPtr->UncoreCLROverrideEnable;
  setup->cpu.CpuOCSupport = TotalSystemConfigurationPtr->OverclockingSupport;
  //uncore ratio from setup-end
 setup->cpu.CpuIOTGEnable = TotalSystemConfigurationPtr->ProcessorIOTGEnable;
  for (socket = 0; socket<MAX_SOCKET; socket++) {
    setup->cpu.CoresToEnable[socket] = TotalSystemConfigurationPtr->CoresToEnable[socket]; //Aptio V Server Override
    setup->cpu.CoreDisableMask[socket] = TotalSystemConfigurationPtr->CoreDisableMask[socket]; //Aptio V Server Override: Grangeville 0xfe
    setup->cpu.IotEn[socket] = (UINT8) TotalSystemConfigurationPtr->IotEnCboBitmap[socket];
    setup->cpu.OclaTorEntry[socket] = TotalSystemConfigurationPtr->OclaMaxTorEntry[socket];
    setup->cpu.OclaWay[socket] = TotalSystemConfigurationPtr->OclaMaxWay[socket];
    setup->cpu.UFSDisable = TotalSystemConfigurationPtr->UFSDisable;
  }

  setup->cpu.CheckCpuBist = TotalSystemConfigurationPtr->CheckCpuBist;

  for (Index = 0; Index < MAX_CPU_SOCKETS; Index ++ ) {
    //
    // Compute IO Ratio
    //
    Alignment = TotalSystemConfigurationPtr->IoAlignment[Index] > QPI_SOCKET_MAX_IO_ALIGNMENT ?
        QPI_SOCKET_MAX_IO_ALIGNMENT :
        TotalSystemConfigurationPtr->IoAlignment[Index];
    Alignment = LShiftU64 (1, (UINTN) Alignment);
    Length = TotalSystemConfigurationPtr->IoLength[Index] ;
    if (Alignment > QPI_SOCKET_IO_RATIO_UNIT) {
      //
      // If Alignment request is greater than IO Chunk size, increase the length by Alignment - 1
      //
      Length += Alignment - 1;
    }
    setup->qpi.IoRatio[Index] = (UINT8) DivU64x32 (Length + QPI_SOCKET_IO_RATIO_UNIT - 1, QPI_SOCKET_IO_RATIO_UNIT);

    //
    // Compute MMIO32 Ratio
    //
    Alignment = TotalSystemConfigurationPtr->MmiolAlignment[Index] > QPI_SOCKET_MAX_MMIOL_ALIGNMENT ?
        QPI_SOCKET_MAX_MMIOL_ALIGNMENT  :
        TotalSystemConfigurationPtr->MmiolAlignment[Index];
    Alignment = LShiftU64 (1, (UINTN) Alignment);
    Length = TotalSystemConfigurationPtr->MmiolLength[Index] ;
    if (Alignment > QPI_SOCKET_MMIOL_RATIO_UNIT) {
      //
      // If Alignment request is greater than MMIO32 Chunk size, increase the length by Alignment - 1
      //
      Length += Alignment - 1;
    }
    setup->qpi.MmiolRatio[Index] = (UINT8) DivU64x32 (Length + QPI_SOCKET_MMIOL_RATIO_UNIT - 1, QPI_SOCKET_MMIOL_RATIO_UNIT);

  }
  //
  // Check MOR to see if we need clear memory
  //
  TcgSetup.MorState = 0;
  VariableSize = sizeof (TCG_SETUP);
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"TcgSetup",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          &TcgSetup
                          );
  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "TcgSetup Variable Found - MOR setup = %r\n", TcgSetup.MorState));
    if (TcgSetup.MorState){
      //
      // Try to get the MOR settings from NVRAM variable
      //
      DEBUG((EFI_D_ERROR, "MEMORY_INIT: MOR Control setup Enabled!\n"));

      VariableSize   = sizeof (UINT8);
      Status = PeiVariable->GetVariable (
                          PeiVariable,
                          MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                          &gEfiMemoryOverwriteControlDataGuid,
                          NULL,
                          &VariableSize,
                          &MorControl
                          );

      if (MOR_CLEAR_MEMORY_VALUE (MorControl) == 0x1) {
        setup->mem.options |= MEMORY_TEST_EN;
        setup->mem.options |= MEMORY_TEST_FAST_BOOT_EN;

        DEBUG((EFI_D_ERROR, "MEMORY_INIT: MOR Control Clear Action requested!\n"));
        DEBUG((EFI_D_ERROR, "MEMORY_INIT: options |= (MEMORY_TEST_EN | MEMORY_TEST_FAST_BOOT_EN) \n"));
      }
    }

  }
}

// Aptio V Server Override - Start : Fix for OemGetPlatformVariableTagPtr Corruption
EFI_STATUS
OemGetPlatformVariableTagPtr(
  struct sysHost             *host,
  VOID                       **VariableTagPtr,
  SYSTEM_CONFIGURATION  *TotalSystemConfigurationPtr
  )
// Aptio V Server Override - End Fix for OemGetPlatformVariableTagPtr Corruption 
{
// Aptio V Server Override - Start : Fix for the OemGetPlatformVariableTagPtr Corruption 
//  SYSTEM_CONFIGURATION                  TotalSystemConfiguration;
//  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
// Aptio V Server Override - End Fix for OemGetPlatformVariableTagPtr Corruption
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  EFI_STATUS                            Status;
  EFI_PEI_SERVICES                      **PeiServices;

  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  //
  // Locate Variable PPI
  //
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to IntelSetup
  Status = PeiVariable->GetVariable (
                        PeiVariable,
                        L"IntelSetup",
                        &gEfiSetupVariableGuid,
                        NULL,
                        &VariableSize,
                        TotalSystemConfigurationPtr
                        );

  if (Status == EFI_SUCCESS){
    *VariableTagPtr = &(TotalSystemConfigurationPtr->VariableTag);

  } else {
    *VariableTagPtr = NULL;
  }

  return Status;
}


UINT32
OemMemRiserInfo (
    struct sysHost             *host
  )
{
  struct sysSetup       *setup;
  UINT8                 socket;
  UINT8                 ch;

  setup = (struct sysSetup *)(&host->setup);
  switch (host->nvram.common.platformType) {
#ifndef GRANGEVILLE_FLAG
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    case TypeMayanCityCRBDDR3:
    case TypeWildcatPass:
    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:

      //
      // 1 DIMM vref controller per 2 channels
      //
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {

          if ( (0 == socket) || (1 == socket) ) {
            if (ch & BIT0) {
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = NO_DEV;
              continue;
            }

            setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = DCP_ISL9072X;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.controller    = PROCESSOR;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.strapAddress  = 0x06;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.deviceType    = DTI_DCP_FIVE;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.busSegment = (ch & BIT1) >> 1;
          }
        }
      }
      break;

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:

      //
      // 1 DIMM vref controller per 3 channels
      //
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {

          if ( (0 == socket) || (1 == socket) ) {
            if ( (0 == ch) || (3 == ch) ) {
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = DCP_ISL9072X;
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.controller    = PROCESSOR;
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.strapAddress  = 0x06;
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.deviceType    = DTI_DCP_FIVE;
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.busSegment = (ch & BIT1) >> 1;
            } else {
              setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = NO_DEV;
            }
          }
        }
      }
      break;

    case TypeHalfWidthCitySRP:
    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:

      //
      // No info yet on these platforms
      //
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = NO_DEV;
        }
      }
      break;
  }

  switch (host->nvram.common.platformType) {

    case TypeMayanCityCRB:
    case TypeWildcatPass:
    case TypePlatformUnknown:
    default:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0-1
          //
          if ( (0 == socket) || (1 == socket) ) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 3;
          }
        } // ch loop
      } // socket loop
      break;
    case TypeMayanCityCRBDDR3:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
        } // ch loop
      } // socket loop
      break;
    case TypeIncaCityCRB:
    case TypeIncaCitySRP:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0-3
          //
          setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 3;
        } // ch loop
      } // socket loop
      break;

    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0
          //
          if (0 == socket) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 1;
          }
        } // ch loop
      } // socket loop
      break;

    case TypeMayanCitySRP:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0
          //
          if (0 == socket) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 3;
          }
          // Socket 1
          //
          if (1 == socket) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
          }
        } // ch loop
      } // socket loop
      break;
    case TypeMayanCitySRPDDR3:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
        } // ch loop
      } // socket loop
      break;

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0
          //
          if (0 == socket) {
            // Channels 1-3
            //
            if ( (1 == ch) || (2 == ch) || (3 == ch) ) {
              setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
            }
          }
          // Socket 1
          //
          if (1 == socket) {
            // Channels 1-3
            //
            if ( (1 == ch) || (2 == ch) || (3 == ch) ) {
              setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
            }
          }
        } // ch loop
      } // socket loop
      break;

    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch = 0; ch < MAX_CH; ch++) {
          // Socket 0-1
          //
          if ( (0 == socket) || (1 == socket) ) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
          }
        } // ch loop
      } // socket loop
      break;
    case TypeEmeraldPoint:
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        for (ch =0; ch < MAX_CH; ch++) {
          // Socket 0-1
          //
          if ( (0 == socket) || (1 == socket) ) {
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
          }
        }
      }
      break;
#else
    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
    case TypeTrabucoCanyon:
    case TypeCamelbackMountain:
    case TypeTlaquepaque:
    case TypePiestewaPeak:
    case TypeEchoCanyon:
    //
    //DE has only 1 socket and 2 channels
    //
      for (socket = 0; socket < MAX_SOCKET; socket++) {
        if (socket) break;
          for (ch = 0; ch < MAX_CH; ch++) {
// AptioV server override start
//            if (ch > 1) break;
//setup->mem.socket[socket].ddrCh[ch].numDimmSlots need re-initial default value from MAX_DIMM to 0.
//Then ch2~3 numDimmSlots value will not be set for TypeEchoCanyon platform.
            setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 3;
            if (ch > 1) continue;
// AptioV server override end
#ifdef GRANGEVILLE_FLAG
              if((host->nvram.common.platformType == TypeTlaquepaque)||(host->nvram.common.platformType == TypeEchoCanyon)||(host->nvram.common.platformType == TypeCamelbackMountain)) {
                setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 1;
              } else {
#endif
                setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
#ifdef GRANGEVILLE_FLAG
              }
#endif
              if (ch & BIT0) {
                setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = NO_DEV;
                continue;
            }
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = DCP_ISL9072X;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.controller = PROCESSOR;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.strapAddress  = 0x06;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.deviceType = DTI_DCP_FIVE;
            setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.busSegment = (ch & BIT1) >> 1;
          }
      }
    break;
    case TypePlatformUnknown:
    //
    // Platform Type Unknown. Hence thowing error msg and asserting
    //
    //DEBUG((EFI_D_ERROR, "Platform type unknown. FRU not programmed correctly\n"));
    //ASSERT(0);
    break;

    default:
    //
    // Platform Type not supported, hence asserting
    //
     // DEBUG((EFI_D_ERROR, "Platform type not supported\n"));
     // ASSERT(0);
      break;
#endif
  }
  return SUCCESS;
}



VOID EnableDisableBatteryBackedOptions(
  struct sysSetup       *setup,
  UINT8 Enable
)
{

  setup->mem.socket[0].ddrCh[0].batterybacked = Enable;
  setup->mem.socket[0].ddrCh[1].batterybacked = Enable;
#ifndef GRANGEVILLE_FLAG
  setup->mem.socket[0].ddrCh[2].batterybacked = Enable;
  setup->mem.socket[0].ddrCh[3].batterybacked = Enable;
#endif
}

VOID  SetupADRGPIOs (
  struct sysHost             *host
 )
{
#ifdef GRANGEVILLE_FLAG
  if (host->nvram.common.platformType == TypePiestewaPeak) {
    DEBUG((EFI_D_INFO, "Set GPIO37 ADR_STATUS_CL as GPIO output \n"));
    IoWrite32((host->var.common.gpioBase + 0x30), (IoRead32((host->var.common.gpioBase + 0x30)) | (1 << 6)));
    IoWrite32((host->var.common.gpioBase + 0x34), (IoRead32((host->var.common.gpioBase + 0x34)) & ~(1 << 6)));
  }
  if ((host->nvram.common.platformType == TypeTrabucoCanyon) ||
    (host->nvram.common.platformType == TypeCamelbackMountain) || (host->nvram.common.platformType == TypeEchoCanyon) ||
    (host->nvram.common.platformType == TypeTlaquepaque)) {
    //  Configure the CPLD for the current ADR config
    //Set GPIO41 (CPLD_CONFIG_CLK_R) to be a GPIO output low
    DEBUG((EFI_D_INFO, "Set GPIO41 (CPLD_CONFIG_CLK_R) to be a GPIO output low\n"));
    IoWrite32((host->var.common.gpioBase + 0x30), (IoRead32((host->var.common.gpioBase + 0x30)) | (1 << 9)));
    IoWrite32((host->var.common.gpioBase + 0x34), (IoRead32((host->var.common.gpioBase + 0x34)) & (~(1 << 9))));

    //Set GPIO42 (CPLD_CONFIG_DATA_R) to be a GPIO output low
    DEBUG((EFI_D_INFO, "Set GPIO42 (CPLD_CONFIG_DATA_R) to be a GPIO output low\n"));
    IoWrite32((host->var.common.gpioBase + 0x30), (IoRead32((host->var.common.gpioBase + 0x30)) | (1 << 10)));
    IoWrite32((host->var.common.gpioBase + 0x34), (IoRead32((host->var.common.gpioBase + 0x34)) & (~(1 << 10))));
  }
  DEBUG((EFI_D_INFO, "Set GPIO36 (ADR_STATUS_RD) as GPIO input and GPIO37 ADR_STATUS_CL as GPIO output\n"));
  IoWrite32((host->var.common.gpioBase + 0x30), (IoRead32((host->var.common.gpioBase + 0x30)) | (1 << 4)));
  IoWrite32((host->var.common.gpioBase + 0x34), (IoRead32((host->var.common.gpioBase + 0x34)) | (1 << 4)));

#else
  if (host->nvram.common.platformType == TypeEmeraldPoint) {
    //Set GPIO15 (WBG_CPLD_CNTL1) to be a GPIO output low
    DEBUG((EFI_D_INFO, "Set GPIO15 (WBG_CPLD_CNTL1) to be a GPIO output low\n"));
    IoWrite32((PCH_GPIO_BASE_ADDRESS + 0x00), (IoRead32((PCH_GPIO_BASE_ADDRESS + 0x00)) | (1 << 15)));
    IoWrite32((PCH_GPIO_BASE_ADDRESS + 0x04), (IoRead32((PCH_GPIO_BASE_ADDRESS + 0x04)) & (~(1 << 15))));

    //Set GPIO14 (WBG_CPLD_CNTL0) to be a GPIO output low
    DEBUG((EFI_D_INFO, "Set GPIO14 (WBG_CPLD_CNTL0) to be a GPIO output low\n"));
    IoWrite32((PCH_GPIO_BASE_ADDRESS + 0x00), (IoRead32((PCH_GPIO_BASE_ADDRESS + 0x00)) | (1 << 14)));
    IoWrite32((PCH_GPIO_BASE_ADDRESS + 0x04), (IoRead32((PCH_GPIO_BASE_ADDRESS + 0x04)) & (~(1 << 14))));
    //Set GPIO41 (ADR_STATUS) to be a GPIO input
    DEBUG((EFI_D_INFO, "Set GPIO41 (ADR_STATUS) to be a GPIO input\n"));
    IoWrite32((host->var.common.gpioBase + 0x30), (IoRead32((host->var.common.gpioBase + 0x30)) | (1 << 9)));
    IoWrite32((host->var.common.gpioBase + 0x34), (IoRead32((host->var.common.gpioBase + 0x34)) | (1 << 9)));
  }
#endif
}

VOID  SetPlatformCPLDMode (
  SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
)
{
  // Put the CPLD into config mode
  DEBUG((EFI_D_INFO, "Put the CPLD into config mode\n"));
  sendCPLDByte(PCH_GPIO_BASE_ADDRESS, 0xA6);


  if (TotalSystemConfigurationPtr->ADREn == ADR_HARDWARE_TRIGGER) {
    // Put the CPLD into pure ADR mode
    DEBUG((EFI_D_INFO, "Put the CPLD into hardware ADR mode\n"));
    sendCPLDByte(PCH_GPIO_BASE_ADDRESS, 0x40);
  }
#ifdef DE_SKU
  if (TotalSystemConfigurationPtr->ADREn == ADR_SOFTWARE_TRIGGER) {
    // Put the CPLD into soft ADR mode
    DEBUG((EFI_D_INFO, "Put the CPLD into soft ADR mode\n"));
    sendCPLDByte(PCH_GPIO_BASE_ADDRESS, 0x42);
  }
#endif
  if (TotalSystemConfigurationPtr->ADREn && TotalSystemConfigurationPtr->ADRDataSaveMode == ADR_C2F) { // C2F Mode enabled.
   // Put the CPLD into pure C2F mode
   DEBUG((EFI_D_INFO, "Enable C2F Mode\n"));
   sendCPLDByte(PCH_GPIO_BASE_ADDRESS, 0x41);
  }

  DEBUG((EFI_D_INFO, "\nRemove the CPLD from config mode\n"));
  // Remove the CPLD from config mode
  sendCPLDByte(PCH_GPIO_BASE_ADDRESS, 0x88);
}


/*++

Routine Description:

  OEM hook to enable ADR related platform specific features.
  This routine enables batterybacked feature on all the supported channels.
  This routine also sets up CPLD into one of two mode (battery backed mode or C2F mode)
  Note that CPLD setup code is specific to Emerald Point platform.
  This routine needs to be updated with any other board specific changes related to ADR.

Arguments:

  host    - Pointer to SysHost structure.
  TotalSystemConfigurationPtr - Pointer to setup options structure.


Returns:

  None

--*/

VOID OemInitStorageFeatures(
  struct sysHost             *host,
  SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr
  )
{
  struct sysSetup       *setup;


  setup = (struct sysSetup *)(&host->setup);

  if (TotalSystemConfigurationPtr->ADREn)
    setup->mem.ADREn = TotalSystemConfigurationPtr->ADRDataSaveMode;
  else
    setup->mem.ADREn = 0;

  setup->mem.check_pm_sts = TotalSystemConfigurationPtr->check_pm_sts;
  setup->mem.check_platform_detect = TotalSystemConfigurationPtr->check_platform_detect;
  if (TotalSystemConfigurationPtr->ADRDataSaveMode == ADR_NVDIMM) { // Disable checking for ADR if in NVDIMM mode. 
#ifndef GRANGEVILLE_FLAG
    setup->mem.check_pm_sts = 0;
#endif
    setup->mem.check_platform_detect = 0;
  }
#ifdef GRANGEVILLE_FLAG
  setup->mem.BlockScTrafficOnAdr = TotalSystemConfigurationPtr->BlockScTrafficOnAdr;
#endif
#ifdef GRANGEVILLE_FLAG
  if (host->nvram.common.platformType == TypeTlaquepaque) {
    setup->mem.MemDownConfig = 1;
  }
#endif
#ifdef MEM_NVDIMM_EN
  setup->mem.eraseArmNVDIMMS = TotalSystemConfigurationPtr->eraseArmNVDIMMS;
  setup->mem.restoreNVDIMMS = TotalSystemConfigurationPtr->restoreNVDIMMS;
#endif

  if (TotalSystemConfigurationPtr->ADREn) {
    SetupADRGPIOs(host);
    DEBUG((EFI_D_ERROR, "Adr Enabled in setup\n"));
    if (TotalSystemConfigurationPtr->ADRDataSaveMode != ADR_NVDIMM) {
      EnableDisableBatteryBackedOptions(setup, 1);
      SetPlatformCPLDMode(TotalSystemConfigurationPtr);
    } else {
      EnableDisableBatteryBackedOptions(setup, 0);
    }
  }
  // SG_CODE End
}


VOID
OemInitializePlatformData (
    struct sysHost             *host
  )
/*++

Routine Description:

  MRC wrapper code.
  Initialize host structure with OEM specific setup data

Arguments:
    host = pointer to sysHost structure

Returns:

--*/
{
  EFI_STATUS        Status;
  EFI_BOOT_MODE     BootMode;
  EFI_PEI_SERVICES  **gPeiServices;
  UINT32            VariableSize;
  UINTN             VarAttrib;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;

#ifndef SKIP_OEMPLATFORMSETUP
#ifdef LT_FLAG
  EFI_PEI_CPU_IO_PPI             *CpuIo;
  UINT8                          Data8;
  UINT8                          Data8_Save;
  UINT8                          ScleanRequired;
  PEI_TXT_SCLEAN_PPI             *ScleanPpi;
#endif
  SYSTEM_CONFIGURATION           TotalSystemConfiguration;
#endif
  CHAR16                         EfiMemoryConfigVariable[] = L"MemoryConfig0";
  UINT32                         RemainingSize;
  VOID                           *NvramPtr = NULL;
  UINTN                          *CompressTable = NULL;
  UINTN                          CompressTableSize;

  SCRATCH_DATA                   ScratchData;
  UINT32                         ScratchDataSize;
  UINT8* CompressedInBytes;

  struct sysSetup *setup ;

  //
  // create a pointer to sysSetup structure
  //
  setup = (struct sysSetup *)&host->setup;

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  host->nvram.common.platformType = OemGetPlatformType(host);
  host->nvram.common.platformRevId = OemGetPlatformRevId(host);

#ifndef SKIP_OEMPLATFORMSETUP
  OemGetPlatformSetupValue (host, &TotalSystemConfiguration);
#endif

  setup->cpu.MeCoreDisable = 0;		//Aptio V Override
  
  //
  // Determine boot mode
  //
  Status = (*gPeiServices)->GetBootMode(gPeiServices, &BootMode);
  ASSERT_EFI_ERROR(Status);

  if (BootMode == BOOT_ON_S3_RESUME) {
    host->var.common.bootMode = S3Resume;
  } else {
    host->var.common.bootMode = NormalBoot;
  }

#ifndef SKIP_OEMPLATFORMSETUP
#if SPS_SUPPORT
  //
  // If it is SPS firmware running in ME and Node Manager is enabled
  // apply NM boot time policy, i.e. performance vs power optimized boot path
  // and processor cores disabling.
  // Note that defaults for PwrOptBoot and Cores2Disable are neutral so
  // for non SPS firmware or ME error BIOS defaults are not changed.
  //
  {
    SPS_MEFS1      Mefs1;
    SPS_NMFS       Nmfs;
    UINT8          CoresDisabled, Socket;
    UINT32         Mask;
    UINT32         Timeout = 500;
    EFI_PEI_STALL_PPI *pStallPpi = NULL;
    BOOLEAN        PwrOptBoot = FALSE;
    UINT8          Cores2Disable = 0;

    if (BootMode == BOOT_ON_S3_RESUME || BootMode == BOOT_ON_S4_RESUME)
    {
       DEBUG((EFI_D_INFO, "[SPS] Skip cores reconfiguration on S3 or S4 resume\n"));
       PwrOptBoot = TotalSystemConfiguration.NmPwrOptBoot;
       Cores2Disable = TotalSystemConfiguration.NmCores2Disable;
	   //setup->cpu.MeCoreDisable = TotalSystemConfiguration.NmCores2Disable; //Aptio V Server Override
    }
    else
    {
      while (1)
      {
        Mefs1.DWord = MmPci32(0, HECI1_BUS, HECI1_DEV, HECI1_FUN, HECI_R_MEFS1);
        if (Mefs1.Bits.ErrorCode != 0)
        {
          DEBUG((EFI_D_ERROR, "[ME] ERROR: Invalid ME state (MEFS1: 0x%08X)\n", Mefs1.DWord));
          break;
        }
        Nmfs.DWord = MmPci32(0, HECI2_BUS, HECI2_DEV, HECI2_FUN, SPS_R_NMFS);
        if (Nmfs.Bits.NmEnabled)
        {
          if (Mefs1.Bits.OperatingMode == MEFS1_OPMODE_SPS)
          {
            if (TotalSystemConfiguration.NmPwrOptBootOverride)
            {
              PwrOptBoot = TotalSystemConfiguration.NmPwrOptBoot;
            }
            else
            {
              PwrOptBoot = !Nmfs.Bits.PerfOptBoot;
            }
            if (TotalSystemConfiguration.NmCores2DisableOverride)
            {
              Cores2Disable = TotalSystemConfiguration.NmCores2Disable;
            }
            else
            {
              Cores2Disable = (UINT8)Nmfs.Bits.Cores2Disable;
            }
            DEBUG((EFI_D_ERROR, "[SPS] NM firmware detected in ME, "
                                "boot mode: %d, cores to disable: %d (NMFS: 0x%08X)\n",
                                !PwrOptBoot, Cores2Disable, Nmfs.DWord));
          }
          else
          {
            DEBUG((EFI_D_ERROR,"[SPS] Non SPS firmware in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", Mefs1.DWord, Nmfs.DWord));
          }
          break;
        }
        if (Mefs1.Bits.CurrentState > ME_CURSTATE_RECOVERY)
        {
          DEBUG((EFI_D_ERROR,"[SPS] NM not enabled in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", Mefs1.DWord, Nmfs.DWord));
          break;
        }
        if (Timeout-- == 0)
        {
          DEBUG((EFI_D_ERROR,"[SPS] ERROR: Timeout waiting for ME (MEFS1: 0x%08X)\n",Mefs1.DWord));
          break;
        }
        if (pStallPpi != NULL)
        {
          pStallPpi->Stall(gPeiServices, pStallPpi, 1000);
        }
      }
    }
    //
    // If NM requests power optimized boot disable parallel memory initialization.
    //
    if (PwrOptBoot)
    {
      setup->mem.options &= ~MULTI_THREAD_MRC_EN;
    }
    //
    // If NM requests disabling cores compare it with BIOS settings,
    // for each socket disable max(BIOS settings, NM request) cores.
    //
    if (Cores2Disable > 0 && Cores2Disable < 8 * sizeof(setup->cpu.CoreDisableMask[0]))
    {
      for (Socket = 0;
           Socket < sizeof(setup->cpu.CoreDisableMask)/sizeof(setup->cpu.CoreDisableMask[0]);
           Socket++)
      {
        CoresDisabled = 0;
        Mask = setup->cpu.CoreDisableMask[Socket];
        while (Mask != 0)
        {
          if (Mask & 1)
          {
            CoresDisabled++;
          }
          Mask >>= 1;
        }
        Mask = 0x00000002; // start with bit 1, do not disable core zero
        while (CoresDisabled < Cores2Disable && Mask)
        {
          if (!(setup->cpu.CoreDisableMask[Socket] & Mask))
          {
            setup->cpu.CoreDisableMask[Socket] |= Mask;
            CoresDisabled++;
          }
          Mask <<= 1;
        }
      } // for (Socket...)
    }
  } // if (BootMode != BOOT_ON_S3_RESUME)
#endif // SPS_SUPPORT

#ifdef LT_FLAG
//Aptio V Server Override - Only need to execute this section of code if Ltsx Enabled.
if (TotalSystemConfiguration.ProcessorLtsxEnable) {
  CpuIo = (*gPeiServices)->CpuIo;
  CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x70, LTSX_TXT_CMOS_ADDRESS); //Aptio V Server Override: Using the SDL token value
  Data8 = CpuIo->IoRead8 (gPeiServices, CpuIo, 0x71);
  Data8_Save = Data8 ;
  Data8 &= (~ (BIT4 | BIT5));
  if ((TotalSystemConfiguration.ProcessorLtsxEnable)) {
      // AptioV Server Override: Eip 443149 changes - Start
      // System Lock should not be configurable in release BIOS
      PcdSetBool (PcdProcessorLtsxEnable, TRUE);
      // AptioV Server Override: Eip 443149 changes - End
      Data8 |= BIT4 | BIT5;
  }
  DEBUG((EFI_D_ERROR, "ProcessorLtsxEnable Enabled in Setup : CMOS_LTSX_OFFSET(%02x)= 0x%02x: for LT/TPM policy\n", LTSX_TXT_CMOS_ADDRESS, Data8));// Aptio V Server Override: Using the SDL token value
  if( (Data8_Save&(BIT4+BIT5)) != (Data8&(BIT4+BIT5)) ) {
      //Aptio V Server Override: Using the SDL token value
      CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x70, LTSX_TXT_CMOS_ADDRESS); // Added because DEBUG now checks CMOS and alters port 0x70
      CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x71, Data8);    // sync up FITEGN cmos with setup option


      //
      // Do this powergood reset only if we're not in HSX PO
      //  where a bootscript is needed to start the CPU
      //
      DEBUG((EFI_D_ERROR, "ProcessorLtsxEnable Enabled in Setup : Sync up LT Policy for : do powergood reset!\n"));
      (*gPeiServices)->ResetSystem(gPeiServices);          // do powergood reset

  }
  if (TotalSystemConfiguration.ProcessorLtsxEnable) {
#if 1
      ScleanRequired = (( (*(volatile UINT32 *)(UINTN)0xfed30008) & BIT6 ) == 0  ) ? 0 : 1;
      ScleanRequired |= ((( (*(volatile UINT32 *)(UINTN)0xfed308f0) & BIT1 ) == 0  ) ? 0 : 1);
#else // Used this for forcing SCLEAN to ALWAYS run (for quick debugging)
      ScleanRequired = 1;
#endif
      if( ScleanRequired )
      {
        DEBUG((EFI_D_ERROR, "MEMORY_INIT: Secrets found in Memory\n"));
        Status = (**gPeiServices).LocatePpi (
                                   gPeiServices,
                                   &gPeiTxtScleanPpiGuid,
                                   0,
                                   NULL,
                                   &ScleanPpi
                                   );
        if (!EFI_ERROR(Status)) {
          Status = ScleanPpi->RunSclean (
                                gPeiServices,
                                ScleanPpi
                                );
        }
       } //ScleanRequired

// AptioV Server Override start EIP214891: Installs PPI to signal the TCG module to skip TPM Startup if
//                                the TXT policy specified in the FIT (CMOS BITS) is set to enabled.  
      if (((*(volatile UINT32 *)(UINTN)LT_SPAD_EXIST)))
      {
        Status = (**gPeiServices).InstallPpi(gPeiServices, gPpiList);
        if (Status == EFI_SUCCESS)
        {
          DEBUG((EFI_D_ERROR, "Disable TPM Startup: %r\n", Status));
        }
        else
        {
	      DEBUG((EFI_D_ERROR, "Warning : Enable TPM Startup and TPM Startup command will be sent again from TCG module : %r\n", Status));
	      ASSERT_EFI_ERROR (Status);
        }
      }
// AptioV Server Override end EIP214891: Installs PPI to signal the TCG module to skip TPM Startup if
//                                the TXT policy specified in the FIT (CMOS BITS) is set to enabled.      
  } //(TotalSystemConfigurationPtr.ProcessorSmxEnable)
   
//AptioV server Override Start:	 
}
else
{
	  CpuIo = (*gPeiServices)->CpuIo;
	  CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x70, LTSX_TXT_CMOS_ADDRESS); // Aptio V Server Override: Using the SDL token value
	  Data8 = CpuIo->IoRead8 (gPeiServices, CpuIo, 0x71);  
	  Data8_Save = Data8 ;
	  Data8 &= (~ (BIT4 | BIT5));

	  DEBUG((EFI_D_ERROR, "ProcessorLtsxEnable Disabled in Setup : CMOS_LTSX_OFFSET(%02x)= 0x%02x: for LT/TPM policy\n", LTSX_TXT_CMOS_ADDRESS, Data8));// Aptio V Server Override: Using the SDL token value
	  if( (Data8_Save&(BIT4+BIT5)) != (Data8&(BIT4+BIT5)) ) {
	      CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x70, LTSX_TXT_CMOS_ADDRESS); // Aptio V Server Override: Using the SDL token value ....// Added because DEBUG now checks CMOS and alters port 0x70
	      CpuIo->IoWrite8 (gPeiServices, CpuIo, 0x71, Data8);    // sync up FITEGN cmos with setup option


	      //
	      // Do this powergood reset only if we're not in HSX PO
	      //  where a bootscript is needed to start the CPU
	      //
	      DEBUG((EFI_D_ERROR, "ProcessorLtsxEnable Disabled in Setup :Sync up LT Policy for : do powergood reset!\n"));
	      (*gPeiServices)->ResetSystem(gPeiServices);          // do powergood reset

	  }
}
//AptioV server Override End:	 
#endif //LT_FLAG
#endif //!SKIP_OEMPLATFORMSETUP

  if ((host->setup.mem.options & ATTEMPT_FAST_BOOT) || (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD) || (host->var.common.bootMode == S3Resume) || (TotalSystemConfiguration.ADREn)) {
    //
    // Obtain variable services
    //
    //
    // Locate Variable PPI
    //
    host->var.mem.nvramDecompressionFailed = 0;
    Status = (*gPeiServices)->LocatePpi (
                  gPeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );
    ASSERT_EFI_ERROR (Status);

    VarAttrib = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_RUNTIME_ALLOW_RECLAIM;
    RemainingSize = sizeof(host->nvram);
    NvramPtr = &(host->nvram);
    DEBUG((EFI_D_ERROR, "Filling NVRAM structure \n"));
    while(RemainingSize > 0  && !EFI_ERROR(Status)){
      VariableSize = RemainingSize;
      //
      // Reconstruct the s3 nvram variable
      //

      //
      //check  compressed table size
      //
      CompressTableSize = 0;
      Status = PeiVariable->GetVariable (
                  PeiVariable,
                  EfiMemoryConfigVariable,
                  &gEfiMemoryConfigDataGuid,
                  &(UINT32)VarAttrib,
                  &CompressTableSize,
                  NULL
                  );
      DEBUG((EFI_D_ERROR, "Status after GetVariable gEfiMemoryConfigDataGuid = %r\n", Status));

      //
      //Allocate buffer and get comressed table from NVRAM
      //
      if(Status == EFI_BUFFER_TOO_SMALL)
      {
        CompressTable = AllocatePool(CompressTableSize);
        if(CompressTable == NULL)
        {
            DEBUG((EFI_D_ERROR,"Compress Table is NULL\n"));
            ASSERT(FALSE);
            Status = EFI_OUT_OF_RESOURCES;
            continue;
        }

        Status = PeiVariable->GetVariable (
                  PeiVariable,
                  EfiMemoryConfigVariable,
                  &gEfiMemoryConfigDataGuid,
                  &(UINT32)VarAttrib,
                  &CompressTableSize,
                  CompressTable
                  );
        DEBUG((EFI_D_ERROR, "Status after GetVariable gEfiMemoryConfigDataGuid = %r\n", Status));
      }

      if(!EFI_ERROR (Status))
      {
        ScratchDataSize = sizeof(ScratchData);
        CompressedInBytes = (UINT8*) CompressTable;
        VariableSize = CompressedInBytes[4] + (CompressedInBytes[5] << 8) + (CompressedInBytes[6] << 16) + (CompressedInBytes[7] << 24);

        Status = UefiDecompress (CompressTable, NvramPtr, &ScratchData);
        if(Status) {
          host->var.mem.nvramDecompressionFailed = 1;
          DEBUG((EFI_D_ERROR, "Decompression failed: Variable#: %c, status: %d \n",  EfiMemoryConfigVariable[12], Status));
        }
      }

      if (CompressTable!= NULL) {
        FreePool(CompressTable);
        CompressTable = NULL;
      }
      //
      // Increment pointers, Variable Name, and remaining size
      //
      DEBUG((EFI_D_ERROR, "Setting NvramPtr variable\n"));
      EfiMemoryConfigVariable[12]++;  // Increment the number portion of the string
      NvramPtr = (VOID*)((UINT8*) NvramPtr + VariableSize);
      RemainingSize -= VariableSize;  // Decrement the actual size of the variable received
    }
  }

  OemMemRASHook(host, 0);
  OemMemRiserInfo(host);
  OemInitStorageFeatures(host,&TotalSystemConfiguration);
}

UINT32
OemCheckCpuChangeDetection (
  PSYSHOST host
  )
/*++

Routine Description:

  OemCheckCpuChangeDetection  - OEM hook to check if CPU changed

Arguments:

  host  - Pointer to sysHost

Returns:

--*/
{
#if ME_SUPPORT_FLAG
  EFI_PEI_SERVICES            **PeiServices;
  EFI_STATUS                  Status;
  UINT8                       DetectedReplacedCpu;
  ME_UMA_PPI                  *MeUma;


  DetectedReplacedCpu = 0;
  MeUma = NULL;
  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  //
  // Locate MeUma PPI.
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gMeUmaPpiGuid, 0, NULL, &MeUma);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_INFO, "UMA: PchUma couldn't be located. Set UMA to 0 and force cold boot.\n"));
    ((struct sysSetup *)&host->setup)->mem.meRequestedSize = 0;
    host->var.mem.previousBootError = 1;
    return FAILURE;
  }

  if (MeUma->isMeUmaEnabled(PeiServices)) {
    if (host->var.common.bootMode != S3Resume) {
    //
    // ME Stolen Size in MB units
    //
    ((struct sysSetup *)&host->setup)->mem.meRequestedSize = MeUma->MeSendUmaSize (PeiServices, NULL);
    }
    DEBUG ((EFI_D_INFO, "ME UMA size = %d MBytes\n", host->setup.mem.meRequestedSize * (1024 * 1024)));
  }
#endif // ME_SUPPORT_FLAG
  return SUCCESS;
}

UINT32
OemPreMemoryInit (
  PSYSHOST host
  )
/*++

Routine Description:

  OemPreMemoryInit  - OEM hook pre memory init

Arguments:

  host  - Pointer to sysHost

Returns:

--*/
{
  SYSTEM_CONFIGURATION                  TotalSystemConfiguration;
  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  EFI_STATUS                            Status;
  EFI_PEI_SERVICES                      **PeiServices;

  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  //
  // Locate Variable PPI
  //
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  VariableSize = sizeof (SYSTEM_CONFIGURATION);

  Status = PeiVariable->GetVariable (
                        PeiVariable,
                        L"IntelSetup",                 //AptioV server override
                        &gEfiSetupVariableGuid,
                        NULL,
                        &VariableSize,
                        TotalSystemConfigurationPtr
                        );

  //
  // BIOS Guard / Pfat Initialization
  //
  PfatInit (PeiServices, TotalSystemConfigurationPtr, host);
  //
  // OEM specific Cleanup and settings required before memory init
  //

  //
  // OEM specific handling Errors from previous boot
  //

  OemCheckCpuChangeDetection(host);

  return SUCCESS;
}

VOID
OemQpiInit (
    struct sysHost   *host,
    UINT32           QpiStatus
  )
/*++

Routine Description:

  QPI wrapper code.

Arguments:
    host = pointer to sysHost structure
    QpiStatus - Return status from MRC code execution

Returns:

    VOID

--*/
{
  EFI_PEI_SERVICES  **gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  //
  // After QPI init breakpoint callback
  //
  (*gPeiServices)->InstallPpi(gPeiServices, mAfterQpircPpi);
}

#ifdef SERIAL_DBG_MSG
VOID
OemInitSerialDebug (
    PSYSHOST host
    )
/*++

Routine Description:

  Oem Init Serial Debug

Arguments:
    host = pointer to sysHost structure
    QpiStatus - Return status from MRC code execution

Returns:

    VOID

--*/
{
    //
    // Initialize USB debug port if present
    //
    if (!DiscoverUsb2DebugPort(host))
        InitializeUsb2DebugPort(host);
}
#endif  //  SERIAL_DBG_MSG


VOID
OemSendDramInitDoneCommand (
    struct sysHost   *host,
    UINT32           MrcStatus
  )
/*++

Routine Description:

  MRC wrapper code.
  Copy host structure to system memory buffer after MRC when memory becomes available

Arguments:
    host = pointer to sysHost structure
    MrcStatus - Return status from MRC code execution

Returns:

 VOID

--*/

{
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  // Call MeConfigDidReg when ME UMA is configured correctly
  // or MRC error occurred
  //
  if (!host->var.common.resetRequired) {
    ME_UMA_PPI        *MeUma = NULL;
    UINT8             InitStat = 0;
    EFI_STATUS        Status;
    EFI_PEI_SERVICES  **PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

    if (MrcStatus != SUCCESS) {
      InitStat = 0x01;
    } else {
      if ((host->var.mem.previousBootError == 1) ||
          ((host->var.mem.subBootMode != WarmBootFast) && (host->var.common.bootMode == NormalBoot))) {
        DEBUG ((EFI_D_ERROR, "UMA: Memory retrain occurred during warm reset. Force ME FW reload.\n"));
        // Set the flag to tell FW that memory was not maintained InitStat bits 3:0 = (3).
        InitStat = (InitStat & 0xF0) | 0x3;
      }
    }
    //
    // ME UMA Size outside of a 0MB-32MB range or if BDF 0:22:0 is not present, exit.
    //
    if ((host->var.mem.meStolenSize > 0x20) || ((host->var.qpi.OutMesegEn==0) && (host->var.qpi.OutIsocEn==0) && (host->var.mem.meStolenSize > 0x00)) || (PchD22PciCfg32 (0x10) == 0xffffffff) )
    {
      DEBUG((EFI_D_WARN, "UMA: WARNING: Invalid ME UMA Size requested (%d)\n", host->var.mem.meStolenSize));
      InitStat = 0x01;
    }

    //
    // Locate MeUma PPI.
    //
    Status = (*PeiServices)->LocatePpi (PeiServices, &gMeUmaPpiGuid, 0, NULL, &MeUma);
    ASSERT_EFI_ERROR (Status);

    if ((Status == EFI_SUCCESS) && (MeUma->isMeUmaEnabled(PeiServices))) {
      UINT8 nextStep = CBM_DIR_DO_NOTHING;

      Status = MeUma->MeConfigDidReg (PeiServices, NULL, InitStat, &nextStep);
      if (Status == EFI_SUCCESS) {
        DEBUG ((EFI_D_INFO, "MeDramInitDone Complete. Checking for reset...\n"));
      } else {
        DEBUG ((EFI_D_INFO, "MeDramInitDone FAILED (0x%x). Checking for reset...\n", Status));
      }
      if (nextStep == CBM_DIR_NON_PCR) {
        // for this reset use MRC reset mechanism
        host->var.common.resetRequired = POST_RESET_POWERGOOD;
      } else {
        Status = MeUma->HandleMeBiosAction(PeiServices, nextStep);
      }
    }
  }
#endif // ME_SUPPORT_FLAG
}


VOID
OemPostMemoryInit (
    struct sysHost   *host,
    UINT32           MrcStatus
  )
/*++

Routine Description:

  MRC wrapper code.
  Copy host structure to system memory buffer after MRC when memory becomes available

Arguments:
    host = pointer to sysHost structure
    MrcStatus - Return status from MRC code execution

Returns:

 VOID

--*/

{
  EFI_STATUS        Status;
#ifdef BTG_FLAG
#if TESTMENU_FLAG || ME_TESTMENU_FLAG
  SYSTEM_CONFIGURATION  TotalSystemConfiguration;
  SYSTEM_CONFIGURATION  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
  UINTN                 VarSize; 
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  EFI_PEI_SERVICES                      **PeiServices;  
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG
#endif // BTG_FLAG
  UINT8  socket;
  INT16  data16;
  UINT32 csrReg;
  char  *memNvRamPtr;


  //
  // Post-MRC Housekeeping
  //
  if (MrcStatus == SUCCESS && !host->var.common.resetRequired) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
        csrReg = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
        //
        // Set BIT1, indicating MRC is done and the next reset is a warm reset.
        //
        WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, (csrReg | BIT1));
      }
    } // socket
  }

  if (MrcStatus == SUCCESS && !host->var.common.resetRequired) {

    if (host->var.common.bootMode == S3Resume && host->var.mem.subBootMode != AdrResume) {
      //
      // Install S3 memory
      //
      Status = InstallS3Memory (host);
      ASSERT_EFI_ERROR (Status);
    } else {
        OemMemRASHook(host, 1);
        //
        // Set BIT1 for SSPAD0 to indicate the next reset is a warm reset.  If this bit
        // goes back to 0, it means the power was fully cycled and the next boot will be
        // a cold reset.
        //
        for (socket = 0; socket < MAX_SOCKET; socket++) {
          //
          // Save processor PPIN to nvram for cold fast boot if not already saved
          //
          if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){
            if ((host->nvram.mem.socket[socket].procPpin.hi != host->var.mem.socket[socket].procPpin.hi) ||
               (host->nvram.mem.socket[socket].procPpin.lo != host->var.mem.socket[socket].procPpin.lo)){
              host->nvram.mem.socket[socket].procPpin = host->var.mem.socket[socket].procPpin;
            }
#ifdef SERIAL_DBG_MSG
            if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
              rcPrintf((host,"nvram[%d].ppin.hi: 0x%x, var[%d].ppin.hi: 0x%x \n", socket, host->nvram.mem.socket[socket].procPpin.hi, socket, host->var.mem.socket[socket].procPpin.hi ));
              rcPrintf((host,"nvram[%d].ppin.lo: 0x%x, var[%d].ppin.lo: 0x%x \n", socket, host->nvram.mem.socket[socket].procPpin.lo, socket, host->var.mem.socket[socket].procPpin.lo ));
            }
#endif
          }
        }
        //
        // Save socket bit map to NVRAM for cold fast boot
        //
        if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)){
          host->nvram.mem.socketBitMap = host->var.common.socketPresentBitMap;
        }

        host->nvram.mem.crc16 = 0;
        memNvRamPtr = (char *)(&(host->nvram.mem));
        data16 = Crc16 (memNvRamPtr, sizeof(host->nvram.mem));
        host->nvram.mem.crc16 = data16;

        //
        // Install EFI memory HOBs
        //
        DEBUG ((EFI_D_ERROR, "Install EFI Memory\n"));
        Status = InstallEfiMemory (host);
        ASSERT_EFI_ERROR (Status);
        //
        // Save S3 restore Data into Hob
        //
        DEBUG((EFI_D_ERROR, "Save NVRAM restore data into Hob\n"));
        Status = MrcSaveS3RestoreDataToHob (host);
        ASSERT_EFI_ERROR (Status);
      }
    }
    DEBUG ((EFI_D_ERROR, "MRC status = %08x\n", MrcStatus));
  OemSendDramInitDoneCommand(host, MrcStatus);

#ifdef BTG_FLAG
#if TESTMENU_FLAG || ME_TESTMENU_FLAG
  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  //
  // Locate Variable PPI
  //
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );
  //
  // Disable PBET after MemInit before sending IPI to APs
  //  
  VarSize = sizeof(SYSTEM_CONFIGURATION);
  Status = PeiVariable->GetVariable (
                            PeiVariable,
                            L"IntelSetup", // AptioV server override
                            &gEfiSetupVariableGuid,
                            NULL,
                            &VarSize,
                            TotalSystemConfigurationPtr
                            );

  DEBUG((EFI_D_INFO, "SkipStopPbet: %02x\n", TotalSystemConfigurationPtr->SkipStopPbet));
  if (TotalSystemConfigurationPtr->SkipStopPbet == 0) {
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG
    DEBUG ( (EFI_D_INFO, "BTG Inside of StopPbeTimer ()\n") );
    StopPbeTimer ();
#if TESTMENU_FLAG || ME_TESTMENU_FLAG
  }
#endif // TESTMENU_FLAG || ME_TESTMENU_FLAG
#endif // BTG_FLAG
}

VOID
OemPreQpiInit (
    struct sysHost   *host
  )
/*++

Routine Description:

  QPI wrapper code.

Arguments:
    host = pointer to sysHost structure

Returns:

    VOID

--*/
{
}

VOID
OemPostQpiInit (
    struct sysHost   *host,
    UINT32           QpiStatus
  )
/*++

Routine Description:

  QPI wrapper code.

Arguments:
    host = pointer to sysHost structure
    QpiStatus - Return status from MRC code execution

Returns:

    VOID

--*/
{
}

VOID
OemCheckAndHandleResetRequests (
  struct sysHost             *host
  )
/*++

Routine Description:
  Executed by System BSP only.
  OEM hook before checking and triggering the proper type of reset.

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  VOID

--*/
{
//AptioV server overrride start: Dynamic mmcfg base address change
#ifdef DYNAMIC_MMCFG_BASE_FLAG

	 	UINT8	node = 0;
	 	UINT8	CpuBusUncore[MAX_SOCKET];
		UINT32	pciExbase;
		UINT32	SecpciExbase;
		UINT32	Data32_Bits_0_31;
	// ###############################################################
	//
	//	CHIPSET PORTING REQIRUED - TO READ THE MMCFG BASE
	//
	// ###############################################################
		UINT8	MMCFG_Rule_DevNum = 15;
		UINT8	MMCFG_Rule_FnNum = 5;
		UINT8	MMCFG_Rule_Reg = 0xC0;

		pciExbase = host->setup.common.mmCfgBase;	
		if (host->var.common.mmCfgBase  == pciExbase)  return;
	
		OutPort32 (host, 0xCF8, 0x80000000 | (CORE05_BUS_NUM << 16) + (CORE05_DEV_NUM << 11) + (CORE05_FUNC_NUM << 8) + R_IIO_MMCFG_B0);
		SecpciExbase = InPort32 (host, 0xCFC) & 0xFFFFFFF0;			
			
		if (host->var.common.mmCfgBase  == pciExbase)  return ;
			
		for (node = 0; node < MAX_SOCKET; node++)
		{
			if (host->var.common.socketPresentBitMap & (BIT0 << node))
			{
				CpuBusUncore[node] = host->var.common.busUncore[node]; // CPUBUSNO(1) for the corresponding socket/Ioh
				OutPort32 ( host, 0xCF8, 0x80000000 | (CpuBusUncore[node] << 16) + (MMCFG_Rule_DevNum << 11) + (MMCFG_Rule_FnNum << 8) + MMCFG_Rule_Reg);
				Data32_Bits_0_31 = InPort32(host, 0xCFC);
				DEBUG ((EFI_D_ERROR, "MMCFG RULE value %x\n",Data32_Bits_0_31));


				Data32_Bits_0_31 = Data32_Bits_0_31 & 0x000FFFF;
				Data32_Bits_0_31 = Data32_Bits_0_31 | pciExbase;

				OutPort32 ( host, 0xCF8, 0x80000000 | (CpuBusUncore[node] << 16) + (MMCFG_Rule_DevNum << 11) + (MMCFG_Rule_FnNum << 8) + MMCFG_Rule_Reg);
				OutPort32 ( host, 0xCFC +(MMCFG_Rule_Reg & 0x3), Data32_Bits_0_31);
				DEBUG ((EFI_D_ERROR, "MMCFG RULE value %x\n",Data32_Bits_0_31));				

			}
		}
    	

		OutPort32 (host, 0xCF8, 0x80000000 | (CORE05_BUS_NUM << 16) + (CORE05_DEV_NUM << 11) + (CORE05_FUNC_NUM << 8) + R_IIO_MMCFG_B0);
		SecpciExbase = InPort32 (host, 0xCFC) & 0xFFFFFFF0;
		host->var.common.mmCfgBase=pciExbase;
		if (host->var.common.resetRequired == 0)
		{
	  		host->var.common.resetRequired = POST_RESET_WARM;
	#ifdef SERIAL_DBG_MSG
// AptioV server overrride Start: Dynamic mmcfg base address change 
	    	rcPrintf ((host, "\n PCIEXBASE ADDRESS IS CHANGED.SYSTEM WILL DO A RESET !!!! \n \n \n"));
// AptioV server overrride End: Dynamic mmcfg base address change 
	#endif
		}
#endif
		return;
//AptioV server overrride end: Dynamic mmcfg base address change
	   // Perform OEM tasks before a reset might be triggered after control returns to the caller
	
  
}

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
VOID
MEBIOSCheckAndHandleResetRequests(
  struct sysHost             *host
  )
/*++

Routine Description:
  MEBIOS hook before checking and triggering the proper type of reset.

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  VOID

--*/
{
  //
  // Perform MEBIOS tasks before a reset is triggered after control returns to the caller
  //
  EFI_STATUS         Status = EFI_UNSUPPORTED;
  ME_UMA_PPI        *MeUma = NULL;
  EFI_PEI_SERVICES **PeiServices = NULL;

  //
  // Locate MeUma PPI.
  //
  PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  Status = (**PeiServices).LocatePpi (PeiServices, &gMeUmaPpiGuid, 0, NULL, &MeUma);

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Locating MeUma PPI failed!, Status = %r\nSending of HostResetWarning notification FAILED!\n", Status));
  } else {
    //
    // Send HostResetWarning notification to ME
    //
    DEBUG((EFI_D_ERROR, "\nSend HostResetWarning notification to ME.\n"));
    Status = MeUma->HostResetWarning (PeiServices, NULL);
    if (Status == EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, "HostResetWarning notification Complete.\n"));
    } else {
      DEBUG ((EFI_D_INFO, "HostResetWarning notification Complete FAILED (0x%x).\n", Status));
    }
  }
  return;
}
#endif // ME_SUPPORT_FLAG


UINT32
OemMemRASHook (
  struct sysHost             *host,
  UINT8  flag                                 // flag = 0 (Pre MRC RAS hook), flag = 1 (Post MRC RAS hook)
  )
{
  UINT32 status = SUCCESS;
  return status;
}

UINT8
OemGetPlatformType (
  struct sysHost             *host
  )
/*++

Routine Description:

  OEM hook to return platform type

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  UINT8 platformtype.

--*/

//
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PEI_SERVICES                      **gPeiServices;

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);
  return PlatformInfo->BoardId;
}

UINT32
OemGetPlatformRevId (
  struct sysHost             *host
  )
/*++

Routine Description:

  OEM hook to return platform revision ID

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  UINT8 PlatformRevId

--*/

//
{
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PEI_SERVICES                      **gPeiServices;

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);
  return PlatformInfo->TypeRevisionId;
}

VOID
OemPostCpuInit (
  struct sysHost             *host,
  UINT8                      socket
  )
/*++

Routine Description:

  OEM hook to do any platform specifc init

Arguments:

  host - pointer to sysHost structure on stack
  Node - memory controller number ( 0 based)


Returns:

  UINT8

--*/

//
{

}

VOID
OemPublishDataForPost (
  struct sysHost             *host
  )
/*++

Routine Description:

  Puts the host structure variables for POST

Arguments:

  host - pointer to sysHost structure on stack

Returns:

 VOID

--*/
{
  PublishHobData(host);
}


void
OemPlatformFatalError (
  PSYSHOST host,
  UINT8    majorCode,
  UINT8    minorCode,
  UINT8    *haltOnError
  )
{
  return;
}


VOID
OemTurnOffVrsForHedt (
  struct sysHost             *host
  )
/*++

Routine Description:

  Determine if VRs should be turned off on HEDT

Arguments:

  host - pointer to sysHost structure on stack

Returns:

 VOID

--*/
{
#ifndef GRANGEVILLE_FLAG
  EFI_STATUS                        Status;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct memNvram                   *nvramMem;
  UINT32                            VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PeiVariable;
  UINT8                             VrsOff;
  SYSTEM_CONFIGURATION              SystemConfiguration;
  SYSTEM_CONFIGURATION              *SystemConfigurationPtr = &SystemConfiguration;

  EFI_PEI_SERVICES  **PeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  //
  // If HEDT
  //
  if ( (host->nvram.common.platformType == TypeHedtSKU1CRB) ||
      (host->nvram.common.platformType == TypeHedtEv) ||
      (host->nvram.common.platformType == TypeHedtDDR3) ) {

    //
    // Locate Variable PPI
    //
    (**PeiServices).LocatePpi (
                    PeiServices,
                    &gEfiPeiReadOnlyVariable2PpiGuid,
                    0,
                    NULL,
                    &PeiVariable
                    );

    //
    // Attempt to read setup data
    //
    VariableSize = sizeof (SYSTEM_CONFIGURATION);

    Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"IntelSetup", //AptioV Grangeville Override
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          SystemConfigurationPtr
                          );

    //
    // Read setup data OK?
    //
    if (EFI_ERROR (Status)) {
      VrsOff = 1;                                                       // no, force unused VRs off
    } else {
      VrsOff = SystemConfigurationPtr->LOT26UnusedVrPowerDownEnable;    // yes, read setup choice
    }

    if (VrsOff) {
      //
      // Determine which GPIO pins to assert
      //  based on memory configuration
      // These pins turn off power rails to memory VRs
      //
      //
      // Check channel enables
      //
      nvramMem = &host->nvram.mem;
      channelNvList = &nvramMem->socket[0].channelList;

      if ( !((*channelNvList)[0].enabled) && !((*channelNvList)[1].enabled) ) {
        //
        // Channmel 0/1 is empty, so assert V_SM_C01_LOT26 (GPIO15)
        //
        IoWrite32((host->var.common.gpioBase + 0x0C),(IoRead32((host->var.common.gpioBase + 0x0C)) | (1 << 15)));
      }

      if ( !((*channelNvList)[2].enabled) && !((*channelNvList)[3].enabled) ) {
        //
        // Channmel 2/3 is empty, so assert V_SM_C23_LOT26 (GPIO28)
        //
        IoWrite32((host->var.common.gpioBase + 0x0C),(IoRead32((host->var.common.gpioBase + 0x0C)) | (1 << 28)));
      }
    }  // If setup = enable
  }    // If HEDT
#endif // GRANGEVILLE_FLAG
  return;
}

UINT32
OemPlatformSpecificEParam (
  struct sysHost             *host,
  UINT32                     *SizeOfTable,
  UINT32                     *per_lane_SizeOfTable,
  HSX_ALL_LANES_EPARAM_LINK_INFO **ptr,
  HSX_PER_LANE_EPARAM_LINK_INFO  **per_lane_ptr
  )
{
  return FALSE;
}

void
OemPlatformHookMst (
  PSYSHOST host,
  UINT8    socket,
  struct   smbDevice dev,
  UINT8    byteOffset,
  UINT8    *data
  )
/*++

Routine Description:

  OEM hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling

Arguments:

  host              - pointer to sysHost
  socket            - socket number
  smbDevice dev     - SMB Device
  byteOffset        - byte Offset
  data              - data

Returns:

--*/
{
  return;
}

/*++

Routine Description:

  OEM hook meant to be used by customers where they can use it to write their own jumper detection code
  and have it return FALSE when no jumper is present, thereby, blocking calling of the BSSA loader; else have it
  return TRUE if the concerned jumper is physically present. This check ensures that someone will have to be
  physically present by the platform to enable the BSSA Loader.

Arguments:
  host              - pointer to sysHost

Returns:
  This function returns TRUE by default as OEMs are meant to implement their own jumper detection code(DetectPhysicalPresence).

--*/
BOOLEAN
OemDetectPhysicalPresenceSSA (
  PSYSHOST         host
  )
{
  BOOLEAN retval = FALSE;
  return retval;
}
