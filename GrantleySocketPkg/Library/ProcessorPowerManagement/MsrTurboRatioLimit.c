/*++
This file contains an 'Intel Peripheral Driver' and is
licensed for Intel CPUs and chipsets under the terms of your
license agreement with Intel or your vendor. This file may
be modified by the user, subject to additional terms of the
license agreement
--*/
/** @file

Code for Enhanced Intel Speedstep Technology

Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name: MsrTurboRatioLimit.c

**/

#include "CpuPpmIncludes.h"

BOOLEAN 
RatioLimitCheck (
  UINT32 *RatioLimit,
  UINT8   CoreCount
  );

BOOLEAN 
RatioLimitCheck (
  UINT32 *RatioLimit,
  UINT8   CoreCount
  )
{
  BOOLEAN RatioLimitState = TRUE;
  UINT32   i;
    
  for(i = 1; i < CoreCount; i++) {
    RatioLimitState = (RatioLimit[0] >= RatioLimit[i]);
    if(!RatioLimitState) {
        break;
    }
  }

  return RatioLimitState;

} 

VOID
ProgramMsrTurboRatioLimit (
  EFI_CPU_PM_STRUCT *ppm
  )
  /*++

  Routine Description:

  Initializes XE OR Overclocking support in the processor.

  Arguments:

  PPMPolicy Pointer to PPM Policy protocol instance

  Returns:

  EFI_SUCCESS

  --*/
{
  MSR_REGISTER PlatformInfoMsr;
  MSR_REGISTER TurboRatioLimit;
  MSR_REGISTER TurboRatioLimit1;  
  MSR_REGISTER TurboRatioLimit2; 
  MSR_REGISTER TurboRatioLimit3; 
  MSR_REGISTER CoreThreadCount;
  MSR_REGISTER FlexRatioMsr;
  UINT8 CoreCount;
  UINT8 OverclockingBins;
  UINT8 MaxRatioOverride;
  UINT16 MaxBusRatio;
  UINT32 RatioLimit[MAX_CORE];
  BOOLEAN SemaphoreSet = FALSE;
  XE_STRUCT *XePtr = NULL;

  XePtr = (XE_STRUCT *)&(ppm->Setup->Xe);

  for(CoreCount = 0;  CoreCount < MAX_CORE; CoreCount++) {
    RatioLimit[CoreCount] = XePtr->RatioLimit[CoreCount];
  }

  //
  // Now initialize turbo ratio limit MSR
  // Find the number of active cores and initialize the ratio limits only if they are available.
  // Also program the VID value for the new max Turbo ratio by programming Flex Ratio MSR.
  //
  CoreThreadCount.Qword = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
  CoreCount = (UINT8) (CoreThreadCount.Dwords.Low >> CORE_COUNT_OFFSET);

  //if (!XePtr->Enable || (!(ppm->Info->CapId4 & B_OC_ENABLED))) {
    //
    // XE is disabled
    //
    //return; ;
 // }

  TurboRatioLimit.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  TurboRatioLimit1.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT1);
//start
 if(ppm->Info->CpuType == CPU_IVYTOWN) {

	SemaphoreSet = TurboRatioLimit1.Dwords.High && BIOS_PCU_SEMAPHORE;
  } else if (ppm->Info->CpuType == CPU_HASWELLSERVER) {
	TurboRatioLimit2.Qword =  AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT2);
	SemaphoreSet = TurboRatioLimit2.Dwords.High && BIOS_PCU_SEMAPHORE;
  } else if(ppm->Info->CpuType == CPU_BDX_DE_SERVER || ppm->Info->CpuType == CPU_BDX_EP_SERVER) {
    TurboRatioLimit2.Qword =  AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT2);
	TurboRatioLimit3.Qword =  AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT3);
	SemaphoreSet = TurboRatioLimit3.Dwords.High && BIOS_PCU_SEMAPHORE;
  }
  //return if semaphore is set,pcode owns TURBO Limit MSRs
  if(SemaphoreSet) {
	return;
  }  
  
//end

 

  //
  // Check if XE capable
  //
  PlatformInfoMsr.Qword = AsmReadMsr64 (MSR_PLATFORM_INFO);
  FlexRatioMsr.Qword = AsmReadMsr64 (MSR_FLEX_RATIO);

  MaxBusRatio = PlatformInfoMsr.Bytes.SecondByte;

  //
  // Overclocking available in C-step and beyond
  //
  OverclockingBins = (UINT8)((FlexRatioMsr.Dwords.Low & OVERCLOCKING_BINS) >> 17);

  MaxRatioOverride = (UINT8)(MaxBusRatio + OverclockingBins);
  //MaxBusRatio is the MaxTurboratio

  //
  // Check if processor turbo-ratio can be overriden:
  // SandyBridge BWG Section 14.14.2
  // If PLATFORM INFO MSR [28] == 1 and FLEX_RATIO MSR [19:17] != 0
  // Processor with capability to override turbo-ratio detected (either XE or Overclocking part detected)
  //
  if (((PlatformInfoMsr.Dwords.Low & PROG_RATIO_LIMITS) == 0) || (OverclockingBins == 0)){
    //
    // Not XE or Over clocking Capable processor.
    //
//    DEBUG ((EFI_D_ERROR, "ERROR: Trying to configure XE params on a non-XE processor\n"));
    return;
  }

  //
  // For Over-clocking parts, verify ratio overide is within the allowable limits
  //
  if (((PlatformInfoMsr.Dwords.Low & PROG_RATIO_LIMITS) == 1) && (OverclockingBins < MAX_OVERCLOCKING_BINS)) {
    if(RatioLimit[0] > MaxRatioOverride ) {
      RatioLimit[0] = MaxRatioOverride;
    } else if (RatioLimit[1] > MaxRatioOverride ) {
      RatioLimit[1] = MaxRatioOverride;
    }else if (RatioLimit[2] > MaxRatioOverride ) {
      RatioLimit[2] = MaxRatioOverride;
    }else if (RatioLimit[3] > MaxRatioOverride ) {
      RatioLimit[3] = MaxRatioOverride;
    }else if (RatioLimit[4] > MaxRatioOverride ) {
      RatioLimit[4] = MaxRatioOverride;
    }else if (RatioLimit[5] > MaxRatioOverride ) {
      RatioLimit[5] = MaxRatioOverride;
    }else if (RatioLimit[6] > MaxRatioOverride ) {
      RatioLimit[6] = MaxRatioOverride;
    }else if (RatioLimit[7] > MaxRatioOverride ) {
      RatioLimit[7] = MaxRatioOverride;
    }else if (RatioLimit[8] > MaxRatioOverride ) {
      RatioLimit[8] = MaxRatioOverride;
    }else if (RatioLimit[9] > MaxRatioOverride ) {
      RatioLimit[9] = MaxRatioOverride;
    }else if (RatioLimit[10] > MaxRatioOverride ) {
      RatioLimit[10] = MaxRatioOverride;
    }else if (RatioLimit[11] > MaxRatioOverride ) {
      RatioLimit[11] = MaxRatioOverride;
    }else if (RatioLimit[12] > MaxRatioOverride ) {
      RatioLimit[12] = MaxRatioOverride;
    }else if (RatioLimit[13] > MaxRatioOverride ) {
      RatioLimit[13] = MaxRatioOverride;
    }else if (RatioLimit[14] > MaxRatioOverride ) {
      RatioLimit[14] = MaxRatioOverride;
    } else if(ppm->Info->CpuType == CPU_HASWELLSERVER) {
        if (RatioLimit[15] > MaxRatioOverride ) {
          RatioLimit[15] = MaxRatioOverride;
        }else if (RatioLimit[16] > MaxRatioOverride ) {
          RatioLimit[16] = MaxRatioOverride;
        }else if (RatioLimit[17] > MaxRatioOverride ) {
          RatioLimit[17] = MaxRatioOverride;
        }
    }else if ((ppm->Info->CpuType == CPU_BDX_EP_SERVER)){
        if (RatioLimit[15] > MaxRatioOverride ) {
          RatioLimit[15] = MaxRatioOverride;
        }else if (RatioLimit[16] > MaxRatioOverride ) {
          RatioLimit[16] = MaxRatioOverride;
        }else if (RatioLimit[17] > MaxRatioOverride ) {
          RatioLimit[17] = MaxRatioOverride;
        }else if (RatioLimit[18] > MaxRatioOverride ) {
          RatioLimit[18] = MaxRatioOverride;
        }else if (RatioLimit[19] > MaxRatioOverride ) {
          RatioLimit[19] = MaxRatioOverride;
        }else if (RatioLimit[20] > MaxRatioOverride ) {
          RatioLimit[20] = MaxRatioOverride;
        }else if (RatioLimit[21] > MaxRatioOverride ) {
          RatioLimit[21] = MaxRatioOverride;
        }else if (RatioLimit[22] > MaxRatioOverride ) {
          RatioLimit[22] = MaxRatioOverride;
        }else if (RatioLimit[23] > MaxRatioOverride ) {
          RatioLimit[23] = MaxRatioOverride;
        }
      }//end CPU_BDX_EP_SERVER
    } //end else
  //}

  if ((PlatformInfoMsr.Dwords.Low & PROG_RATIO_LIMITS)) {
      //read MSR_TURBO_RATIO_LIMIT,MSR_TURBO_RATIO_LIMIT1,MSR_TURBO_RATIO_LIMIT2
	  //BIOS can write values into the TURBO_RATIO CRs if the semaphore = 0
	  //0 = BIOS owns TURBO_RATIO_LIMIT registers,1 = PCODE owns TURBO_RATIO_LIMIT registers
	  //
    if(ppm->Info->CpuType == CPU_IVYTOWN){
  	  TurboRatioLimit1.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT1);
	  if ((TurboRatioLimit1.Dwords.High & BIOS_PCU_SEMAPHORE)) {    
        while (TurboRatioLimit1.Dwords.High & BIOS_PCU_SEMAPHORE) {
       }
	 }
  	}else if (ppm->Info->CpuType == CPU_HASWELLSERVER){
  		TurboRatioLimit2.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT2);
	   if ((TurboRatioLimit2.Dwords.High & BIOS_PCU_SEMAPHORE)) {    
         while (TurboRatioLimit2.Dwords.High & BIOS_PCU_SEMAPHORE) {
      }
	 }
  	}else if (ppm->Info->CpuType == CPU_BDX_DE_SERVER || ppm->Info->CpuType == CPU_BDX_EP_SERVER ){
  		TurboRatioLimit3.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT3);
	    if ((TurboRatioLimit3.Dwords.High & BIOS_PCU_SEMAPHORE)) {    
          while (TurboRatioLimit3.Dwords.High & BIOS_PCU_SEMAPHORE) {
          //DEBUG ( (EFI_D_INFO, "."));
        }
      //DEBUG ( (EFI_D_INFO, ".\n"));
	 }
  	} 
	  //
	  //BIOS can write new turbo ratios.
	  //as 0 = BIOS owns TURBO_RATIO_LIMIT registers 
	  //
    if(RatioLimitCheck((UINT32 *)&RatioLimit[0], CoreCount)) {
      if (CoreCount >= 1 && RatioLimit[0] ) {

        TurboRatioLimit.Dwords.Low &= ~MAX_RATIO_LIMIT_1C_MASK;
        TurboRatioLimit.Dwords.Low |=RatioLimit[0];
        FlexRatioMsr.Qword = AsmReadMsr64 (MSR_FLEX_RATIO);
        if (FlexRatioMsr.Dwords.Low & OC_LOCK) { 
          //
          // Clear OC_LOCK if it is set
          //
          FlexRatioMsr.Dwords.Low &= ~OC_LOCK;
          AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Qword);
        }

        //
        // Program VID for the new Max Turbo Ratio
        //
        FlexRatioMsr.Qword = AsmReadMsr64 (MSR_FLEX_RATIO);
        FlexRatioMsr.Dwords.Low &= ~MAX_EXTRA_VOLTAGE;
        FlexRatioMsr.Dwords.Low |= XePtr->Voltage;
        AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Qword);
        //save for S3
        WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_FLEX_RATIO, 0, 64, FlexRatioMsr.Qword);
      }
      if (CoreCount >= 2 && RatioLimit[1] ) {
        TurboRatioLimit.Dwords.Low &= ~MAX_RATIO_LIMIT_2C_MASK;
        TurboRatioLimit.Dwords.Low |= (RatioLimit[1] << 8);
      }
      if (CoreCount >= 3 && RatioLimit[2]) {
        TurboRatioLimit.Dwords.Low &= ~MAX_RATIO_LIMIT_3C_MASK;
        TurboRatioLimit.Dwords.Low |= (RatioLimit[2] << 16);
      }
      if (CoreCount >= 4 && RatioLimit[3]) {
        TurboRatioLimit.Dwords.Low &= ~MAX_RATIO_LIMIT_4C_MASK;
        TurboRatioLimit.Dwords.Low |= (RatioLimit[3] << 24);
      }
      if (CoreCount >= 5 && RatioLimit[4]) {
        TurboRatioLimit.Dwords.High &= ~MAX_RATIO_LIMIT_5C_MASK;
        TurboRatioLimit.Dwords.High |=RatioLimit[4];
      }
      if (CoreCount >= 6 && RatioLimit[5]) {
        TurboRatioLimit.Dwords.High &= ~MAX_RATIO_LIMIT_6C_MASK;
        TurboRatioLimit.Dwords.High |= (RatioLimit[5] << 8);
      }
      if (CoreCount >= 7 && RatioLimit[6]) {
        TurboRatioLimit.Dwords.High &= ~MAX_RATIO_LIMIT_7C_MASK;
        TurboRatioLimit.Dwords.High |= (RatioLimit[6] << 16);
      }
      if (CoreCount >= 8 && RatioLimit[7]) {
        TurboRatioLimit.Dwords.High &= ~MAX_RATIO_LIMIT_8C_MASK;
        TurboRatioLimit.Dwords.High |= (RatioLimit[7] << 24);
      }
      if (CoreCount >= 9 && RatioLimit[8]) {
        TurboRatioLimit1.Dwords.Low &= ~MAX_RATIO_LIMIT_1C_MASK;
        TurboRatioLimit1.Dwords.Low |= RatioLimit[8];
      }
      if (CoreCount >= 10 && RatioLimit[9]) {
        TurboRatioLimit1.Dwords.Low &= ~MAX_RATIO_LIMIT_2C_MASK;
        TurboRatioLimit1.Dwords.Low |= (RatioLimit[9] << 8);
      }
      if (CoreCount >= 11 && RatioLimit[10]) {
        TurboRatioLimit1.Dwords.Low &= ~MAX_RATIO_LIMIT_3C_MASK;
        TurboRatioLimit1.Dwords.Low |= (RatioLimit[10] << 16);
      }
      if (CoreCount >= 12 && RatioLimit[11]) {
        TurboRatioLimit1.Dwords.Low &= ~MAX_RATIO_LIMIT_4C_MASK;
        TurboRatioLimit1.Dwords.Low |= (RatioLimit[11] << 24);
      }
      if (CoreCount >= 13 && RatioLimit[12]) {
        TurboRatioLimit1.Dwords.High &= ~MAX_RATIO_LIMIT_5C_MASK;
        TurboRatioLimit1.Dwords.High |= RatioLimit[12];
      }
      if (CoreCount >= 14 && RatioLimit[13]) {
        TurboRatioLimit1.Dwords.High &= ~MAX_RATIO_LIMIT_6C_MASK;
        TurboRatioLimit1.Dwords.High |= (RatioLimit[13] << 8);
      }
      if (CoreCount >= 15 && RatioLimit[14]) {
        TurboRatioLimit1.Dwords.High &= ~MAX_RATIO_LIMIT_7C_MASK;
        TurboRatioLimit1.Dwords.High |= (RatioLimit[14] << 16);
      }
      if (ppm->Info->CpuType == CPU_HASWELLSERVER) {
        if (CoreCount >= 16 && RatioLimit[15]) {
          TurboRatioLimit1.Dwords.High &= ~MAX_RATIO_LIMIT_8C_MASK;
          TurboRatioLimit1.Dwords.High |= (RatioLimit[15] << 16);
        }
        if (CoreCount >= 17 && RatioLimit[16] ) {
          TurboRatioLimit2.Dwords.Low &= ~MAX_RATIO_LIMIT_1C_MASK;
          TurboRatioLimit2.Dwords.Low |= (RatioLimit[16]);
        }
        if (CoreCount >= 18 && RatioLimit[17] ) {
          TurboRatioLimit2.Dwords.Low &= ~MAX_RATIO_LIMIT_2C_MASK;
          TurboRatioLimit2.Dwords.Low |= (RatioLimit[17] << 8);
        }
      }else if (ppm->Info->CpuType == CPU_BDX_EP_SERVER) {
        if (CoreCount >= 16 && RatioLimit[16]) {
          TurboRatioLimit1.Dwords.High &= ~MAX_RATIO_LIMIT_7C_MASK;
          TurboRatioLimit1.Dwords.High |= (RatioLimit[15] << 16);
        }
        if (CoreCount >= 17 && RatioLimit[17]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_1C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[16]);
        }
        if (CoreCount >= 18 && RatioLimit[18]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_2C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[17] << 8);
        }
		if (CoreCount >= 19 && RatioLimit[19]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_3C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[18] << 8);
        }
		if (CoreCount >= 20 && RatioLimit[20]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_4C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[19] << 8);
        }
		if (CoreCount >= 21 && RatioLimit[21]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_5C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[20] << 8);
        }
		if (CoreCount >= 22 && RatioLimit[22]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_6C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[21] << 8);
        }
	    if (CoreCount >= 23 && RatioLimit[23]) {
          TurboRatioLimit2.Dwords.High &= ~MAX_RATIO_LIMIT_7C_MASK;
          TurboRatioLimit2.Dwords.High |= (RatioLimit[22] << 8);
        }
			 
      }

//        DEBUG ( (EFI_D_ERROR, "::MSR RATIO LIMIT VALUE %x\n",TurboRatioLimit.Qword) );
//        DEBUG ( (EFI_D_ERROR, "::MSR RATIO LIMIT1 VALUE %x\n",TurboRatioLimit1.Qword) );     
      AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT, TurboRatioLimit.Qword);
      AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT1, TurboRatioLimit1.Qword);
        //save for S3
        WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT, 0, 64, TurboRatioLimit.Qword);
        WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT1, 0, 64, TurboRatioLimit1.Qword);

      if (ppm->Info->CpuType == CPU_HASWELLSERVER || ppm->Info->CpuType == CPU_BDX_EP_SERVER) {
     // DEBUG ( (EFI_D_ERROR, "::MSR RATIO LIMIT2 VALUE %x\n",TurboRatioLimit2.Qword) );
        AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT2, TurboRatioLimit2.Qword);
          //save for S3
          WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT2, 0, 64, TurboRatioLimit2.Qword);
        }
    }
  }

  return;
}

VOID
PpmSetTurboLimitPcuSemaphore (
  EFI_CPU_PM_STRUCT *ppm
  )
  /*++

  Routine Description:

  Set Overclocking unit BIOS_PCU_SEMAPHORE bit in the processor.

  Arguments:

  PPMPolicy Pointer to PPM Policy protocol instance

  Returns:

  EFI_SUCCESS

  --*/
{
  MSR_REGISTER TurboRatioLimit1;  
  MSR_REGISTER TurboRatioLimit2;  
  MSR_REGISTER TurboRatioLimit3;
  MSR_REGISTER PlatformInfoMsr;
  
  PlatformInfoMsr.Qword = AsmReadMsr64 (MSR_PLATFORM_INFO);
    
  if ( (!(PlatformInfoMsr.Dwords.Low & PROG_RATIO_LIMITS))|| (!(ppm->Info->CapId4 & B_OC_ENABLED))) {
    //
    // XE is disabled
    //
    return; 
  }

  TurboRatioLimit2.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  TurboRatioLimit1.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT1);

  if (ppm->Info->CpuType == CPU_HASWELLSERVER) {
    TurboRatioLimit2.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT2);
    TurboRatioLimit2.Dwords.High |= BIOS_PCU_SEMAPHORE; // Set the semaphore after programming the ratio limit MSR
    AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT2, TurboRatioLimit2.Qword);
    //save for S3
    WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT2, 0, 64, TurboRatioLimit2.Qword);
  } else if (ppm->Info->CpuType == CPU_BDX_DE_SERVER || ppm->Info->CpuType == CPU_BDX_EP_SERVER) {
   
    TurboRatioLimit3.Qword = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT3);
    TurboRatioLimit3.Dwords.High |= BIOS_PCU_SEMAPHORE; // Set the semaphore after programming the ratio limit MSR
    AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT3, TurboRatioLimit3.Qword);

    //save for S3
    WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT3, 0, 64, TurboRatioLimit3.Qword);
  }else {
    TurboRatioLimit1.Dwords.High |= BIOS_PCU_SEMAPHORE; // Set the semaphore after programming the ratio limit MSR
    AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT1, TurboRatioLimit1.Qword);
    //save for S3
    WriteRegisterTable(ppm->ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT1, 0, 64, TurboRatioLimit1.Qword);
  } 

  return; 
}
