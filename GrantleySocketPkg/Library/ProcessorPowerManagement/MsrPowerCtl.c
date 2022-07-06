//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2012, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MsrPowerCtl.c

**/

#include "CpuPpmIncludes.h"

// PCU_CR_CAPID4_CFG bit Description (FUSE value)
#define B_UFS_DISABLE                 (1 << 24) // Bit 24


VOID
ProgramMsrPowerCtl (
		EFI_CPU_PM_STRUCT   *ppm
		)
/*++

		Routine Description:

		This routine is called to program Power_CTL not programed elsewhere. It will be call multiple time passing any the node to be programmed.

		Arguments:

		PPMPolicy Pointer to PPM Policy protocol instance

		Returns:

		EFI_SUCCESS

--*/
{
  MSR_REGISTER MsrValue;

  //
  // Program MSR_POWER_CTL
  //
  MsrValue.Qword = AsmReadMsr64 (MSR_POWER_CTL);
  
  MsrValue.Dwords.Low &= ~(PCH_NEG_DISABLE + LTR_SW_DISABLE + PWR_PERF_TUNING_CFG_MODE + \
				PWR_PERF_TUNING_ENABLE_DYN_SWITCHING + PWR_PERF_TUNING_DISABLE_EEP_CTRL + \
				PWR_PERF_TUNING_DISABLE_SAPM_CTRL + PHOLD_SR_DISABLE + PHOLD_CST_PREVENTION_INIT + \
				FAST_BRK_INT_EN + FAST_BRK_SNP_EN + DIS_PROCHOT_OUT + ENABLE_BIDIR_PROCHOT_EN + \
                                C1E_ENABLE + PROCHOT_RESPONSE);

  MsrValue.Dwords.Low |= ppm->Setup->PowerCtl.Dwords.Low; 

  //HSD 3615299: PCIE BW drops on processor when after using BIOS versions that contain patch 503 ONLY with Pkg C-States enabled
  //Step 1 -- Switch back to mode (2) during PC2
  //- sv.socket0.uncore0.power_ctl_cr.sapm_imc_c2_policy = 0
  //- MSR 0x1FC bit 2
  MsrValue.Dwords.Low &= ~SAPM_IMC_C2_POLICY_EN;
 
  // 4166429	[HSX_A0_PO] CLONE from HSX: SAPM DLL: PCODE is not checking de-feature bit IO_FEATURE_DISABLE_ENABLE_SAPM_DLL
  // Check UFS fuse value
  if (ppm->Info->CapId4 & B_UFS_DISABLE) {
    MsrValue.Dwords.Low |= ( 1 << PWR_PERF_TUNING_DISABLE_SAPM_SHIFT );
  }

  AsmWriteMsr64 (MSR_POWER_CTL, MsrValue.Qword);
  //save for S3
  WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_POWER_CTL, 0, 64, MsrValue.Qword);
  //DEBUG ( (EFI_D_ERROR, ":: MSR (0%x) = %08X%08X \n", MSR_POWER_CTL, MsrValue.Dwords.High, MsrValue.Dwords.Low) );

  //HSD 3615018: needs ability to override ENERGY_PERF_BIAS without loosing Turbo in Windows Balanced Mode
  if (ppm->Setup->PowerCtl.Dwords.Low & PWR_PERF_TUNING_CFG_MODE) {
	  MSR_REGISTER MsrData;

	  MsrData.Qword = AsmReadMsr64 (MSR_ENERGY_PERFORMANCE_BIAS);
	  MsrData.Dwords.Low = ALTENERGYPERFIAS_DEFAULT;
	  AsmWriteMsr64 (MSR_ENERGY_PERFORMANCE_BIAS, MsrData.Qword);
    //save for S3
    WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_ENERGY_PERFORMANCE_BIAS, 0, 64, MsrData.Qword);
  }

  return;
}
