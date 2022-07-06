//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code for Haswell CPU Miscellenous MSRs programming Interface Feature

  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  SktScopeMSRInit.c

**/

#include "Cpu.h"
#include "MpService.h"
#include "Protocol\CpuCsrAccess.h"
#include "UBOX_CFG.h"
#include "PCU_FUN3.h"

extern  MP_SYSTEM_DATA                      mMPSystemData;
extern  CPU_CONFIG_CONTEXT_BUFFER           mCpuConfigContextBuffer;
extern  EFI_CPU_CSR_ACCESS_PROTOCOL        *mCpuCsrAccess;

extern  UINT64                              mCpuIioLlcWaysBitMask;
extern  UINT64                              mCpuQlruCfgBitMask;
extern  UINT8                               mCpuDebugInterface;
extern  BOOLEAN                             mCpuHaswellFamilyFlag;


/*++
	This function returns TRUE if current CPU is HSX server CPU, FALSE otherwise
++*/
BOOLEAN  IsHsxCpu()
{

	EFI_CPUID_REGISTER	  CpuidRegisters;
	UINT32	              FamilyId;
	UINT32	              ModelId;
    UINT32                ExtendedFamilyId = 0;
    UINT32                ExtendedModelId = 0;

	/*
	 This simiple version does not work because  CPU_FAMILY_HSX is defined in a file that should not be included by this file
		AsmCpuid (EFI_CPUID_VERSION_INFO, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
	
		if ((CpuidRegisters.RegEax >> 4) == CPU_FAMILY_HSX) {  
			return TRUE;
		}
		else {
			return FALSE;
		}
	*/
	
	AsmCpuid (EFI_CPUID_VERSION_INFO, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
	//
	// The Extended Family ID needs to be examined only when the Family ID is 0FH.
	// If Family ID is 0FH, Family ID = Family ID + Extended Family ID.
	//
	FamilyId = BitFieldRead32 (CpuidRegisters.RegEax, 8, 11);
    if (FamilyId == 0x0f) {
        ExtendedFamilyId = BitFieldRead32 (CpuidRegisters.RegEax, 20, 27);
		//FamilyId += ExtendedFamilyId;
    }
	
	//
	// The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
	// If Family ID is 06H or 0FH, Model ID = Model ID + (Extended Model ID << 4).
	//
    ModelId = BitFieldRead32 (CpuidRegisters.RegEax, 4, 7);
	if (FamilyId == 0x06 || FamilyId == 0x0f) {
		ExtendedModelId = BitFieldRead32 (CpuidRegisters.RegEax, 16, 19);
		ModelId += (ExtendedModelId << 4);
    }

	FamilyId += ExtendedFamilyId;

    if (mCpuHaswellFamilyFlag && IS_HASWELL_SERVER_PROC (FamilyId, ModelId)) {
		return TRUE;
    }
    else {
        return FALSE;
	}

}

//
/*++
	This function returns TRUE if current CPU is BDX server CPU, FALSE otherwise
++*/
BOOLEAN  IsBdxDECpu()
{

	EFI_CPUID_REGISTER	  CpuidRegisters;
	UINT32	              FamilyId;
	UINT32	              ModelId;
    UINT32                ExtendedFamilyId = 0;
    UINT32                ExtendedModelId = 0;


	
	AsmCpuid (EFI_CPUID_VERSION_INFO, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
	//
	// The Extended Family ID needs to be examined only when the Family ID is 0FH.
	// If Family ID is 0FH, Family ID = Family ID + Extended Family ID.
	//
	FamilyId = BitFieldRead32 (CpuidRegisters.RegEax, 8, 11);
    if (FamilyId == 0x0f) {
        ExtendedFamilyId = BitFieldRead32 (CpuidRegisters.RegEax, 20, 27);
		//FamilyId += ExtendedFamilyId;
    }
	
	//
	// The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
	// If Family ID is 06H or 0FH, Model ID = Model ID + (Extended Model ID << 4).
	//BDX-model if is 0x56
	//
    ModelId = BitFieldRead32 (CpuidRegisters.RegEax, 4, 7);
	if (FamilyId == 0x06 || FamilyId == 0x0f) {
		ExtendedModelId = BitFieldRead32 (CpuidRegisters.RegEax, 16, 19);
		ModelId += (ExtendedModelId << 4);
    }

	FamilyId += ExtendedFamilyId;

    if( FeaturePcdGet (PcdCpuBroadwellDEFamilyFlag) && IS_BROADWELL_DE_PROC (FamilyId, ModelId)) {
		return TRUE;
    }
    else {
        return FALSE;
	}

}

//

VOID
ProgramIioLlcWaysMsr (
         UINTN ProcessorNumber, 
         UINT8 SocketId
		 )
		 
/*++
	This function programs package-scope MSR CBO_SLICE0_CR_IIO_LLC_WAYS
++*/

{
   UINT32    Data2;
   UINT64    MsrQword;

   //
   // Check if the MSR is supported, i.e.,    (capid6.iio_llcconfig_en) 
   //
   Data2 = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, CAPID6_PCU_FUN3_REG);
   if (Data2 & BIT30) {
      //
      // Proggram MSR CBO_SLICE0_CR_IIO_LLC_WAYS based on PCD bitmask data
      //
      if (mCpuIioLlcWaysBitMask) {
          MsrQword = AsmReadMsr64 (MSR_CBO_SLICE0_CR_IIO_LLC_WAYS);
          MsrQword &= ~MASK_CBO_SLICE0_CR_IIO_LLC_WAYS;
	      MsrQword |= mCpuIioLlcWaysBitMask & MASK_CBO_SLICE0_CR_IIO_LLC_WAYS;
	      AsmWriteMsr64 (MSR_CBO_SLICE0_CR_IIO_LLC_WAYS, MsrQword);
      }
   	}
}




VOID
ProgramQlruMsr (
         UINTN ProcessorNumber, 
         UINT8 SocketId
		 )
		 
/*++
	This function programs package-scope MSR VIRTUAL_MSR_CR_QLRU_CONFIG
++*/

{
	UINT32	  Data32;
	UINT64	  MsrQword;

   //
   // Check if the MSRs is supported, i.e.,   capid6.iio_llcconfig_en  = 1
   //
   Data32 = mCpuCsrAccess->ReadCpuCsr(SocketId, 0, CAPID6_PCU_FUN3_REG);
   if (Data32 & BIT30) {
      //
      // Proggram MSR VIRTUAL_MSR_CR_QLRU_CONFIG based on PCD bitmask data
      //
      if (mCpuQlruCfgBitMask) {
          MsrQword = AsmReadMsr64 (MSR_VIRTUAL_MSR_CR_QLRU_CONFIG);
          MsrQword &= ~MASK_VIRTUAL_MSR_CR_QLRU_CONFIG;
          MsrQword |= mCpuQlruCfgBitMask & MASK_VIRTUAL_MSR_CR_QLRU_CONFIG;
          AsmWriteMsr64 (MSR_VIRTUAL_MSR_CR_QLRU_CONFIG, MsrQword);

      }
   }
}






VOID
EarlyMpInitPbsp (
		 UINTN ProcessorNumber
		 )
 /*++
 
		 Routine Description:
 
		 This function is called on the package BSP thread to perform package-scope programming.

	        Assumptions:
	            *mCpuCsrAccess has been initialized to point to the CpuCsrAccess protocol structure
	             mCpuConfigConextBuffer has been initialized with proper data
	              
		 Arguments:
 
		       ProcessorNumber:   Current processor number
 
		 Returns:
 
		       None
 
 --*/
 {
 
   UINT8    SocketId;

   SocketId = (UINT8) mCpuConfigContextBuffer.CollectedDataBuffer[ProcessorNumber].ProcessorLocation.Package;

   ProgramIioLlcWaysMsr(ProcessorNumber, SocketId);

   ProgramQlruMsr(ProcessorNumber, SocketId);


   return;
 }



 
 

