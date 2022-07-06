/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2002 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    DisableCacheAsRam.c
    
Abstract:

    Disable Cache As Ram

Revision History

--*/

#include "CpuPei.h"
#include "CpuPciAccess.h"
#include <Cpu\CpuRegs.h>
#include <Cpu\CpuDataStruct.h>
#include <Library/PciLib.h>

VOID
CacheInvd (
  VOID
  );

VOID
EFIAPI
GetCpuVersion (
  IN  OUT UINT16  *FamilyId,    OPTIONAL
  IN  OUT UINT8   *Model,       OPTIONAL
  IN  OUT UINT8   *SteppingId,  OPTIONAL
  IN  OUT UINT8   *Processor    OPTIONAL
  )
/*++

Routine Description:
  Extract CPU detail version infomation

Arguments:
  FamilyId   - FamilyId, including ExtendedFamilyId
  Model      - Model, including ExtendedModel
  SteppingId - SteppingId
  Processor  - Processor

--*/
{
  EFI_CPUID_REGISTER Register;
  UINT8              TempFamilyId;
  
  AsmCpuid (EFI_CPUID_VERSION_INFO, &Register.RegEax, &Register.RegEbx, &Register.RegEcx, &Register.RegEdx);
  
  if (SteppingId != NULL) {
    *SteppingId = (UINT8) (Register.RegEax & 0xF);
  }

  if (Processor != NULL) {
    *Processor = (UINT8) ((Register.RegEax >> 12) & 0x3);
  }

  if (Model != NULL || FamilyId != NULL) {
    TempFamilyId = (UINT8) ((Register.RegEax >> 8) & 0xF);
  
    if (Model != NULL) {
      *Model = (UINT8) ((Register.RegEax >> 4) & 0xF);
      if (TempFamilyId == 0x6 || TempFamilyId == 0xF) {
        *Model = (UINT8) (*Model  | ((Register.RegEax >> 12) & 0xF0));
      }
    }
  
    if (FamilyId != NULL) {
      *FamilyId = TempFamilyId;
      if (TempFamilyId == 0xF) {
        *FamilyId = (UINT8 ) (*FamilyId + (UINT16) ((Register.RegEax >> 20) & 0xFF));
      }
    }
  } 
}

BOOLEAN
IsNehalem (
  VOID
  )
{
  UINT16              FamilyId;
  UINT8               Model;

  //
  // Check Nehalem, Bloomfield MP/DP, Hillsdale
  //
  GetCpuVersion (&FamilyId, &Model, NULL, NULL);
  if (FamilyId == 0x06 && ((Model == 0x1A) || (Model == 0x2E))) {
    return TRUE;
  } else { 
    return FALSE;
  }
}


EFI_STATUS
DisableCacheAsRam (
  VOID
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  None

Returns:

  EFI_SUCCESS - GC_TODO: Add description for return value

--*/
{
  UINT64  CacheAsRamMsr;

  CacheInvd ();

  CacheAsRamMsr = AsmReadMsr64 (EFI_NO_EVICT_MODE);
  
  //
  // Step 3: Disable No-Eviction Mode Run State by clearing
  //         NO_EVICT_MODE MSR 2E0h bit [1] = 0
  CacheAsRamMsr &= ~B_EFI_NO_EVICT_MODE_RUN;
  AsmWriteMsr64 (EFI_NO_EVICT_MODE, CacheAsRamMsr);

  // Step 4: Disable No-Eviction Mode Setup State by clearing
  //         NO_EVICT_MODE MSR 2E0h bit [0] = 0
  CacheAsRamMsr &= ~B_EFI_NO_EVICT_MODE_SETUP;
  AsmWriteMsr64 (EFI_NO_EVICT_MODE, CacheAsRamMsr);

  return EFI_SUCCESS;
}
