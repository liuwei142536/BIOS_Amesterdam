//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MsrTurboPowerLimit.c

**/

#include "CpuPpmIncludes.h"

#define PCD_CPU_TURBO_MODE_BIT                   0x80000000

VOID
ProgramMsrTurboPowerLimit (
		EFI_CPU_PM_STRUCT *ppm
		)
/*++

		Routine Description:

		- Enables Turbo Mode at package level
		- Based on user configuration
		- Configures Turbo Mode TDP power level and TDP power level time window
		- Configures Dynamic Turbo Power level and Dynamic Turbo Power time window
		- Enables Critical Power Clamp 1 and Critical Power Clamp 2

		Arguments:

		PPMPolicy Pointer to policy protocol instance

		Returns:

		None

--*/
{
//  UINT64 Ia32MiscEnableMsr;
  MSR_REGISTER TurboPowerLimitMsr;
  MSR_REGISTER PlatformInfoMsr;
  UINT16 ConvertedPowerLimit1;
  UINT16 ConvertedPowerLimit1Time;
  UINT16 ConvertedPowerLimit2Time;
  UINT16 ConvertedShortDurationPowerLimit;
  UINT16 Index;
  UINT16 Mutliplier;
  UINT16 GetPcdTemp = 0;
  UINT16      PackageTdp;                           
  UINT8       ProcessorPowerUnit;                 

  ConvertedPowerLimit1Time = 0;
  Mutliplier = 0;
  PackageTdp          = ppm->Info->PackageTdp;
  ProcessorPowerUnit  = ppm->Info->ProcessorPowerUnit;

  //
  // Clear Turbo Mode disable bit in IA32 Misc Enable MSR
  //
  /* Ia32MiscEnableMsr = AsmReadMsr64 (MSR_IA32_MISC_ENABLES);
  Ia32MiscEnableMsr &= ~((UINT64) DISABLE_MASTER_TURBO_MODE << 38);
  AsmWriteMsr64 (MSR_IA32_MISC_ENABLES, Ia32MiscEnableMsr); */

  TurboPowerLimitMsr.Qword = AsmReadMsr64 (MSR_TURBO_POWER_LIMIT);
  
  PlatformInfoMsr.Qword = AsmReadMsr64 (MSR_PLATFORM_INFO);
  //
  // Check if TDP limits are programmable
  // Platform Info MSR (0xCE) [29]
  //
  if ((PlatformInfoMsr.Dwords.Low & PROG_TDP_LIMITS)) {
	  //
	  // Initialize the Power Limit 1 and Power Limit 1 enable bit
	  // Power Limit 1: Turbo Power Limit MSR [14:0]
	  // Power Limit 1 Enable: Turbo Power Limit MSR [15]
	  //
	  //
	  // By default, program Power Limit 1 to Package TDP limit
	  //
	  ConvertedPowerLimit1 = (PackageTdp * ProcessorPowerUnit);
	  //DEBUG ((EFI_D_ERROR, "mPackageTdp = %x, mProcessorPowerUnit = %x\n", PackageTdp, ProcessorPowerUnit));
	
	  GetPcdTemp = ppm->Setup->TurboPowerLimit.Dwords.Low & POWER_LIMIT_MASK;

	  if (GetPcdTemp) {
	    //
	    // PPMPolicy->pTurboSettings->PowerLimit1Power is in watts. We need to convert it to
	    // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
	    // Since we are converting from Watts to CPU power units, multiply by
	    // PACKAGE_POWER_SKU_UNIT_MSR[3:0]. New TDP limit cannot be higher than
	    // the fused TDP limit.
	    //
	    if (GetPcdTemp <= PackageTdp) {
		    ConvertedPowerLimit1 = (GetPcdTemp * ProcessorPowerUnit);
	    }

	    //
	    // OverClocking unit is detected
	    //
            if (ppm->Info->CapId4 & B_OC_ENABLED) {
		ConvertedPowerLimit1 = (GetPcdTemp * ProcessorPowerUnit);
	    }
	  }
	
	  TurboPowerLimitMsr.Dwords.Low &= ~POWER_LIMIT_MASK;
	  TurboPowerLimitMsr.Dwords.Low |= (UINT32) (ConvertedPowerLimit1);
	  //DEBUG ((EFI_D_ERROR, "New Power Limit 1 %d watt (%d in CPU power unit)\n", GetPcdTemp, ConvertedPowerLimit1));
	
	  //
	  // Force Power Limit 1 override to be enabled
	  //
	  TurboPowerLimitMsr.Dwords.Low |= POWER_LIMIT_ENABLE;

	  //
	  // Convert seconds to MSR value. Since not all values are programmable, we'll select
	  // the entry from mapping table which is either equal to the user selected value. OR to a value in the mapping table
	  // which is closest (but less than) to the user-selected value.
	  //
	  // HSD 36144077: BIOS to progral PL2 time window to a non 0 default value
	  ConvertedPowerLimit2Time = 0x3;

   	GetPcdTemp = (UINT16) ((ppm->Setup->TurboPowerLimit.Dwords.Low & POWER_LIMIT_1_TIME_MASK) >> POWER_LIMIT_1_TIME_SHIFT);
	
	  ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[0][1];
	  for(Index = 0; mSecondsToMsrValueMapTable[Index][0] != END_OF_TABLE; Index++) {

	    if (GetPcdTemp >= mSecondsToMsrValueMapTable[Index][0]) {
		    ConvertedPowerLimit1Time = mSecondsToMsrValueMapTable[Index][1];
	    }
	  }

	  //
	  // Configure Power Limit 1 (Long Duration Turbo) time windows: Turbo Power Limit MSR [23:17]
	  //
	  //DEBUG ((EFI_D_ERROR, "Package Power Limit 2 Time Window = %x, Package Power Limit 1 Time Window = %x\n", ConvertedPowerLimit2Time, ConvertedPowerLimit1Time));

	  TurboPowerLimitMsr.Dwords.Low &= ~POWER_LIMIT_1_TIME_MASK;
	  TurboPowerLimitMsr.Dwords.Low |= (UINT32)ConvertedPowerLimit1Time << 17;
	  TurboPowerLimitMsr.Dwords.High &= ~POWER_LIMIT_1_TIME_MASK;
	  TurboPowerLimitMsr.Dwords.High |= (UINT32)ConvertedPowerLimit2Time << 17;

	  //
	  // PPMPolicy->pTurboSettings->PowerLimit2Power value is in watts. We need to convert it to
	  // CPU Power unit, specified in PACKAGE_POWER_SKU_UNIT_MSR[3:0].
	  // Since we are converting from Watts to CPU power units, multiply by
	  // PACKAGE_POWER_SKU_UNIT_MSR[3:0]
	  //
	  //HSD 3615282: PPM registers incorrectly programmed
	  Mutliplier = 120;
	  ConvertedShortDurationPowerLimit = EFI_IDIV_ROUND ((Mutliplier * PackageTdp * ProcessorPowerUnit), 100);
	
	  GetPcdTemp = ppm->Setup->TurboPowerLimit.Dwords.High & POWER_LIMIT_MASK;
	
	  if ((GetPcdTemp > 0) & (GetPcdTemp < EFI_IDIV_ROUND ((Mutliplier * PackageTdp), 100))) {
            ConvertedShortDurationPowerLimit = (GetPcdTemp * ProcessorPowerUnit);
//            DEBUG ((EFI_D_ERROR, "::PM:: NON-OverClocking unit is detected, caculate ConvertedShortDurationPowerLimit = (GetPcdTemp * ProcessorPowerUnit) = %x\n", ConvertedShortDurationPowerLimit));
	  }

          //
          // OverClocking unit is detected
          //
          if (ppm->Info->CapId4 & B_OC_ENABLED) {
		    if(GetPcdTemp > 0)
			{
            ConvertedShortDurationPowerLimit = (GetPcdTemp * ProcessorPowerUnit);
           //DEBUG ((EFI_D_ERROR, "::PM:: OverClocking unit is detected, caculate ConvertedShortDurationPowerLimit = (GetPcdTemp * ProcessorPowerUnit) = %x\n", ConvertedShortDurationPowerLimit));
			}
          }

	  TurboPowerLimitMsr.Dwords.High &= ~POWER_LIMIT_MASK;
	
	  TurboPowerLimitMsr.Dwords.High |= (UINT32)ConvertedShortDurationPowerLimit;

	  TurboPowerLimitMsr.Dwords.Low &= ~PKG_CLMP_LIM;
          TurboPowerLimitMsr.Dwords.Low |= (ppm->Setup->TurboPowerLimit.Dwords.Low & PKG_CLMP_LIM);

	  if (ppm->Setup->TurboPowerLimit.Dwords.High & POWER_LIMIT_ENABLE) {
	  
	    TurboPowerLimitMsr.Dwords.High |= POWER_LIMIT_ENABLE;
	    //DEBUG ((EFI_D_ERROR, "Short duration Power limit enabled, %d watt (%d in CPU power unit)\n", GetPcdTemp, ConvertedShortDurationPowerLimit));

            TurboPowerLimitMsr.Dwords.High &= ~PKG_CLMP_LIM;
            TurboPowerLimitMsr.Dwords.High |= (ppm->Setup->TurboPowerLimit.Dwords.High & PKG_CLMP_LIM);
	  }
  }
  
/*  
  //
  // Checks whether user indicates to enable Turbo/EIST
  //
  if (!ppm->Setup->Pst.EistEnabled || !ppm->Info->EistCap ) {
      TurboPowerLimitMsr.Qword = AsmReadMsr64 (MSR_TURBO_POWER_LIMIT);
      ConvertedPowerLimit2Time = 0x3;
      TurboPowerLimitMsr.Dwords.High &= ~POWER_LIMIT_1_TIME_MASK;
      TurboPowerLimitMsr.Dwords.High |= (UINT32)ConvertedPowerLimit2Time << 17;
 //         DEBUG ((EFI_D_ERROR, "Force MSR_TURBO_POWER_LIMIT value as PO value = %x %x\n", (TurboPowerLimitMsr.Dwords.High), TurboPowerLimitMsr.Dwords.Low));
  }
*/
	AsmWriteMsr64 (MSR_TURBO_POWER_LIMIT, TurboPowerLimitMsr.Qword);
  //save for S3
  WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_POWER_LIMIT, 0, 64, TurboPowerLimitMsr.Qword);

//	  DEBUG ((EFI_D_ERROR, "Write MSR_TURBO_POWER_LIMIT[%x] = %x %x\n", MSR_TURBO_POWER_LIMIT, (TurboPowerLimitMsr.Dwords.High),  TurboPowerLimitMsr.Dwords.Low));

  return;
}
