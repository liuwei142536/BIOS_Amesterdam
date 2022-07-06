/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c) 1999 - 2006 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    CachePeim.c
    
Abstract:

    EFI 2.0 PEIM to initialize the cache and load the BSP microcode

Revision History

--*/

#include "CpuPei.h"
#include <Cpu\CpuRegs.h>
#include <Cpu\CpuDataStruct.h>


#define  ALIGNED_SEED          0x01010101

INT8
CheckDirection (
  IN UINT64      Input
  );

UINT64
PeiPower2MaxMemory (
  IN UINT64     MemoryLength
  );

VOID
EfiDisableCacheMtrr (
  IN UINT64     *OldMtrr
  );

VOID
EfiRecoverCacheMtrr (
  IN BOOLEAN     EnableMtrr,
  IN UINT64      OldMtrr
  );

VOID
EfiProgramMtrr (
  IN  UINTN                     MtrrNumber,
  IN  EFI_PHYSICAL_ADDRESS      MemoryAddress,
  IN  UINT64                    MemoryLength,
  IN  EFI_MEMORY_CACHE_TYPE     MemoryCacheType,
  IN  UINT64                    ValidMtrrAddressMask
  );

EFI_STATUS
EFIAPI
PeiResetCacheAttributes (
  IN  EFI_PEI_SERVICES     **PeiServices,
  IN  PEI_CACHE_PPI        *This
  );

/**
  program the MTRR according to the given range and cache type.

  This function provides the PPI for PEI phase to set the memory attribute by program
  the MTRR according to the given range and cache type. Actually this function is a 
  wrapper of the MTRR libaray to suit the PEI_CACHE_PPI interface

  @param  PeiServices     General purpose services available to every PEIM.  
  @param  This            Current instance of Pei Cache PPI.                 
  @param  MemoryAddress   Base Address of Memory to program MTRR.            
  @param  MemoryLength    Length of Memory to program MTRR.                  
  @param  MemoryCacheType Cache Type.   
  @retval EFI_SUCCESS             Mtrr are set successfully.        
  @retval EFI_LOAD_ERROR          No empty MTRRs to use.            
  @retval EFI_INVALID_PARAMETER   The input parameter is not valid. 
  @retval others                  An error occurs when setting MTTR.
                                          
**/
EFI_STATUS
EFIAPI
PeiSetCacheAttributes (
  IN  EFI_PEI_SERVICES         **PeiServices,
  IN  PEI_CACHE_PPI            *This,
  IN  EFI_PHYSICAL_ADDRESS     MemoryAddress,
  IN  UINT64                   MemoryLength,
  IN  EFI_MEMORY_CACHE_TYPE    MemoryCacheType
  );

EFI_STATUS
SearchForExactMtrr (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_PHYSICAL_ADDRESS      MemoryAddress,
  IN  UINT64                    MemoryLength,
  IN  UINT64                    ValidMtrrAddressMask,
  OUT UINT32                    *UsedMsrNum,
  OUT EFI_MEMORY_CACHE_TYPE     *MemoryCacheType
  );

BOOLEAN
IsDefaultType (
  IN EFI_MEMORY_CACHE_TYPE     MemoryCacheType
  );

EFI_STATUS
DisableCacheAsRam (
  VOID
  );

typedef struct _ALIGNED_DWORD {
  UINT32 High;
  UINT32 Low;
} ALIGNED_DWORD;

typedef union _ALIGNED {
  UINT64        AlignedQword;
  ALIGNED_DWORD AlignedDword;
} ALIGNED;

typedef struct {
  UINT32  Msr;
  UINT32  BaseAddress;
  UINT32  Length;
} EFI_FIXED_MTRR;

EFI_FIXED_MTRR  mFixedMtrrTable[] = {
  {
    EFI_IA32_MTRR_FIX64K_00000,
    0,
    0x10000
  },
  {
    EFI_IA32_MTRR_FIX16K_80000,
    0x80000,
    0x4000
  },
  {
    EFI_IA32_MTRR_FIX16K_A0000,
    0xA0000,
    0x4000
  },
  {
    EFI_IA32_MTRR_FIX4K_C0000,
    0xC0000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_C8000,
    0xC8000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_D0000,
    0xD0000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_D8000,
    0xD8000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_E0000,
    0xE0000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_E8000,
    0xE8000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_F0000,
    0xF0000,
    0x1000
  },
  {
    EFI_IA32_MTRR_FIX4K_F8000,
    0xF8000,
    0x1000
  },
  {
    0,
    0x100000,
    0
  }
};

PEI_CACHE_PPI   mCachePpi = {
  PeiSetCacheAttributes,
  PeiResetCacheAttributes
};

VOID
EfiDisableCacheMtrr (
  OUT UINT64                   *OldMtrr
  )
/*++
  Routine Description:
    Disable cache and its mtrr
  Arguments:
    OldMtrr - To return the Old MTRR value
  Returns:
    None 
--*/
{
  UINT64  TempQword;

  AsmDisableCache ();
  //
  // Disable Cache MTRR
  //
  *OldMtrr  = AsmReadMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE);
  TempQword = (*OldMtrr) &~B_EFI_CACHE_MTRR_VALID &~B_EFI_CACHE_FIXED_MTRR_VALID;
  AsmWriteMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE, TempQword);

  return ;
}

VOID
EfiRecoverCacheMtrr (
  IN BOOLEAN                  EnableMtrr,
  IN UINT64                   OldMtrr
  )
/*++
  Routine Description:
    Recover cache MTRR
  Arguments:
    EnableMtrr - Whether to enable the MTRR
    OldMtrr    - The saved old MTRR value to restore when not to 
                 enable the MTRR
  Returns:
    None 
--*/
{
  UINT64  TempQword;

  TempQword = 0;
  //
  // Enable Cache MTRR
  //
  if (EnableMtrr) {
    TempQword = AsmReadMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE);
    TempQword |= (B_EFI_CACHE_MTRR_VALID | B_EFI_CACHE_FIXED_MTRR_VALID);
  } else {
    TempQword = OldMtrr;
  }

  AsmWriteMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE, TempQword);

  AsmEnableCache ();
  return ;
}

VOID
EfiProgramMtrr (
  IN UINTN                     MtrrNumber,
  IN EFI_PHYSICAL_ADDRESS      MemoryAddress,
  IN UINT64                    MemoryLength,
  IN EFI_MEMORY_CACHE_TYPE     MemoryCacheType,
  IN UINT64                    ValidMtrrAddressMask
  )
/*++

Routine Description:

  Programming MTRR according to Memory address, length, and type.

Arguments:

  MtrrNumber           - the variable MTRR index number
  MemoryAddress        - the address of target memory
  MemoryLength         - the length of target memory
  MemoryCacheType      - the cache type of target memory
  ValidMtrrAddressMask - the MTRR address mask

Returns:

  none

--*/
{
  UINT64  TempQword;
  UINT64  OldMtrr;

  EfiDisableCacheMtrr (&OldMtrr);

  //
  // MTRR Physical Base
  //
  TempQword = (MemoryAddress & ValidMtrrAddressMask) | MemoryCacheType;
  AsmWriteMsr64 (MtrrNumber, TempQword);

  //
  // MTRR Physical Mask
  //
  TempQword = ~(MemoryLength - 1);
  AsmWriteMsr64 (MtrrNumber + 1, (TempQword & ValidMtrrAddressMask) | B_EFI_CACHE_MTRR_VALID);

  EfiRecoverCacheMtrr (TRUE, OldMtrr);

  return ;
}

UINT64
PeiPower2MaxMemory (
  IN UINT64     MemoryLength
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  MemoryLength  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
{
  UINT64  Result;

  if (RShiftU64 (MemoryLength, 32)) {
    Result = LShiftU64 ((UINT64) GetPowerOfTwo64 ((UINT32) RShiftU64 (MemoryLength, 32)), 32);
  } else {
    Result = (UINT64) GetPowerOfTwo64 ((UINT32) MemoryLength);
  }

  return Result;
}

EFI_STATUS
PeiProgramUnalignedMtrr (
  IN UINT64                    AlignedQword,
  IN UINTN                     MsrNum,
  IN UINT32                    UnalignedBase,
  IN UINT32                    UnalignedLimit
  )
/*++

Routine Description:

  Program the unaligned MTRR register.

Arguments:

  AlignedQword          - The aligned 64-bit cache type.
  MsrNum                - The index of current MTRR.
  UnalignedBase         - Base Address of the current unaligned MTRR.
  UnalignedLimit        - Limit Address of the current unaligned MTRR.

Returns:

  EFI_SUCCESS           - The unaligned MTRR is set successfully.
  EFI_DEVICE_ERROR      - The unaligned address is not the multiple of the basic length of MTRR.

--*/
{
  UINT32      UnalignedOffset;
  UINT64      TempQword;
  UINT64      Mask;
  UINT8       ByteShift;

  UnalignedOffset = UnalignedBase - mFixedMtrrTable[MsrNum].BaseAddress;
  if (UnalignedOffset % mFixedMtrrTable[MsrNum].Length != 0) {
    return EFI_DEVICE_ERROR;
  }
  ByteShift   = (UINT8) (UnalignedOffset / mFixedMtrrTable[MsrNum].Length);
  Mask        = ~(LShiftU64 (1, ByteShift * 8) - 1);

  if (UnalignedLimit < mFixedMtrrTable[MsrNum + 1].BaseAddress) {
    UnalignedOffset = UnalignedLimit - mFixedMtrrTable[MsrNum].BaseAddress;
    if (UnalignedOffset % mFixedMtrrTable[MsrNum].Length!= 0) {
      return EFI_DEVICE_ERROR;
    }
    ByteShift = (UINT8) (UnalignedOffset / mFixedMtrrTable[MsrNum].Length);
    Mask     &= LShiftU64 (1, ByteShift * 8) - 1;
  }

  TempQword   = AsmReadMsr64 (mFixedMtrrTable[MsrNum].Msr) & ~Mask;
  TempQword  |= AlignedQword & Mask;

  AsmWriteMsr64 (mFixedMtrrTable[MsrNum].Msr, TempQword);
  return EFI_SUCCESS;
}

EFI_STATUS
PeiProgramLowMemoryMtrr (
  IN EFI_MEMORY_CACHE_TYPE     MemoryCacheType,
  IN UINT32                    MemoryBase,
  IN UINT32                    MemoryLimit
  )
/*++

Routine Description:

  Given the low memory range ( <= 1MB) and cache type, program the MTRRs.

Arguments:

  MemoryCacheType       - Cache Type.
  MemoryBase            - Base Address of Memory to program MTRR.
  MemoryLimit           - Limit Address of Memory to program MTRR.

Returns:

  EFI_SUCCESS           - Low memory MTRR is set successfully.
  others                - An error occurs when setting Low memory MTRR.

--*/
{
  EFI_STATUS  Status;
  UINT64      OldMtrr;
  ALIGNED     Aligned;
  UINTN       MsrNum;

  EfiDisableCacheMtrr (&OldMtrr);
  Status = EFI_SUCCESS;

  Aligned.AlignedDword.High = MemoryCacheType * ALIGNED_SEED;
  Aligned.AlignedDword.Low  = Aligned.AlignedDword.High;

  for (MsrNum = 0; mFixedMtrrTable[MsrNum].BaseAddress < MemoryBase; MsrNum++) {
    ;
  }
  if (MemoryBase < mFixedMtrrTable[MsrNum].BaseAddress) {
    Status = PeiProgramUnalignedMtrr (Aligned.AlignedQword, MsrNum - 1, MemoryBase, MemoryLimit);
    if (EFI_ERROR (Status)) {
      goto Done;
    }
  }
  while (MsrNum < V_EFI_FIXED_MTRR_NUMBER && MemoryLimit >= mFixedMtrrTable[MsrNum + 1].BaseAddress) {
    //
    // Program aligned MTRR
    //
    AsmWriteMsr64 (mFixedMtrrTable[MsrNum].Msr, Aligned.AlignedQword);
    MsrNum++;
  }
  if (MemoryLimit > mFixedMtrrTable[MsrNum].BaseAddress) {
    Status = PeiProgramUnalignedMtrr (Aligned.AlignedQword, MsrNum,  mFixedMtrrTable[MsrNum].BaseAddress, MemoryLimit);
  }

Done:
  EfiRecoverCacheMtrr (TRUE, OldMtrr);
  return Status;
}

EFI_STATUS
EFIAPI
PeiSetCacheAttributes (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN PEI_CACHE_PPI             *This,
  IN EFI_PHYSICAL_ADDRESS      MemoryAddress,
  IN UINT64                    MemoryLength,
  IN EFI_MEMORY_CACHE_TYPE     MemoryCacheType
  )
/*++

Routine Description:

  Given the memory range and cache type, programs the MTRRs.

Arguments:

  PeiServices           - General purpose services available to every PEIM.
  This                  - Current instance of Pei Cache PPI.
  MemoryAddress         - Base Address of Memory to program MTRR.
  MemoryLength          - Length of Memory to program MTRR.
  MemoryCacheType       - Cache Type.

Returns:

  EFI_SUCCESS           - Mtrr are set successfully.
  EFI_LOAD_ERROR        - No empty MTRRs to use.
  EFI_INVALID_PARAMETER - The input parameter is not valid.
  others                - An error occurs when setting MTTR.

Note:

--*/
{
  EFI_STATUS            Status;
  UINT32                MsrNum;
  UINT64                TempQword;
  UINT32                UsedMsrNum;
  EFI_MEMORY_CACHE_TYPE UsedMemoryCacheType;
  UINT64                ValidMtrrAddressMask;
  EFI_CPUID_REGISTER    FeatureInfo;
  UINT64                Power2Length[8];
  UINT64                LengthArray[8];
  UINTN                 LengthSize;
  UINTN                 Index;
  UINTN                 Count;
  UINTN                 Remainder;
  UINT32                VariableMtrrLimit;

  VariableMtrrLimit = (UINT32) (AsmReadMsr64(EFI_IA32_MTRR_CAP) & B_EFI_IA32_MTRR_VARIABLE_SUPPORT);

  ValidMtrrAddressMask = LShiftU64((UINT64) 1, 36) & (~(UINT64)0x0FFF);

  AsmCpuid (EFI_CPUID_EXTENDED_FUNCTION, &FeatureInfo.RegEax, &FeatureInfo.RegEbx, &FeatureInfo.RegEcx, &FeatureInfo.RegEdx);
  if (FeatureInfo.RegEax >= EFI_CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (EFI_CPUID_VIR_PHY_ADDRESS_SIZE, &FeatureInfo.RegEax, &FeatureInfo.RegEbx, &FeatureInfo.RegEcx, &FeatureInfo.RegEdx);
    ValidMtrrAddressMask = (LShiftU64((UINT64) 1, FeatureInfo.RegEax & 0xFF) - 1) & (~(UINT64)0x0FFF);
  }

  //
  // Check for invalid parameter
  //
  if ((MemoryAddress & ~ValidMtrrAddressMask) != 0 || (MemoryLength & ~ValidMtrrAddressMask) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  switch (MemoryCacheType) {
  case EFI_CACHE_UNCACHEABLE:
  case EFI_CACHE_WRITECOMBINING:
  case EFI_CACHE_WRITETHROUGH:
  case EFI_CACHE_WRITEPROTECTED:
  case EFI_CACHE_WRITEBACK:
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }
  //
  // Check if Fixed MTRR
  //
  if ((MemoryAddress + MemoryLength) <= (1 << 20)) {
    Status = PeiProgramLowMemoryMtrr (
               MemoryCacheType,
               (UINT32) MemoryAddress, 
               (UINT32) (MemoryAddress + MemoryLength)
               );
    return Status;
  }
  
  //
  // Special case for 1 MB base address
  //
  if (MemoryAddress == 0x100000) {
    MemoryAddress = 0;
    MemoryLength += 0x100000;
  }
  
  //
  // Split MemoryLength into a sum of power of 2
  //
  ZeroMem (Power2Length, sizeof (Power2Length));
  LengthSize = 0;
  TempQword  = MemoryLength;
  do {
    Power2Length[LengthSize] = PeiPower2MaxMemory (TempQword);
    TempQword -= Power2Length[LengthSize];
    LengthSize++;
  } while (TempQword != 0 && LengthSize < 8);
  if (TempQword != 0) {
    return EFI_LOAD_ERROR;
  }
  
  //
  // Work out an order of splitted power of 2
  // so that Base and Length are suitable for MTRR
  // setting constraints.
  //
  Count     = 0;
  TempQword = MemoryAddress;
  do {
    for (Index = 0; Index < LengthSize; Index++) {
      if (Power2Length[Index] != 0) {
        if (Power2Length[Index] >= 0x100000000L) {
          Remainder = (UINT32) TempQword;
          if (Remainder == 0) {
            DivU64x32Remainder (
              RShiftU64 (TempQword, 32),
              (UINTN) RShiftU64 (Power2Length[Index], 32),
              &Remainder
              );
          }
        } else {
          DivU64x32Remainder (TempQword, (UINTN) Power2Length[Index], &Remainder);
        }
        if (Remainder == 0) {
          LengthArray[Count] = Power2Length[Index];
          TempQword += Power2Length[Index];
          Power2Length[Index] = 0;
          Count++;
          break;
        }
      }
    }
    if (Index == LengthSize) {
      return EFI_LOAD_ERROR;
    }
  } while (Count < LengthSize);

  //
  // Begin setting the MTRR according to the order
  //
  for (Index = 0; Index < LengthSize; Index++, MemoryAddress += MemoryLength) {
    MemoryLength = LengthArray[Index];
    //
    // Search if the range attribute has been set before
    //
    Status = SearchForExactMtrr (
               PeiServices,
               MemoryAddress,
               MemoryLength,
               ValidMtrrAddressMask,
               &UsedMsrNum,
               &UsedMemoryCacheType
               );

    if (!EFI_ERROR (Status)) {
      //
      // Compare if it has the same type as current setting
      //
      if (UsedMemoryCacheType != MemoryCacheType) {
        //
        // Different type
        //
        //
        // Check if the set type is the same as default type
        //
        if (IsDefaultType (MemoryCacheType)) {
          //
          // Clear the mtrr
          //
          AsmWriteMsr64 (UsedMsrNum, 0);
          AsmWriteMsr64 (UsedMsrNum + 1, 0);

        } else {
          //
          // Modify the mtrr type
          //
          EfiProgramMtrr (
            UsedMsrNum,
            MemoryAddress,
            MemoryLength,
            MemoryCacheType,
            ValidMtrrAddressMask
            );
        }
      }
      continue;
    }
    //
    // Find first unused MTRR
    //
    for (MsrNum = EFI_CACHE_VARIABLE_MTRR_BASE; MsrNum < (EFI_CACHE_VARIABLE_MTRR_BASE + VariableMtrrLimit*2 - 1); MsrNum += 2) {
      if ((AsmReadMsr64 (MsrNum + 1) & B_EFI_CACHE_MTRR_VALID) == 0) {
        break;
      }
    }
    //
    // Reserve 1 MTRR for OS.
    //
    if (MsrNum > (EFI_CACHE_VARIABLE_MTRR_BASE + VariableMtrrLimit*2 - 1)) {
      return EFI_LOAD_ERROR;
    }

    EfiProgramMtrr (
      MsrNum,
      MemoryAddress,
      MemoryLength,
      MemoryCacheType,
      ValidMtrrAddressMask
      );
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
PeiResetCacheAttributes (
  IN EFI_PEI_SERVICES          **PeiServices,
  IN PEI_CACHE_PPI             *This
  )
/*++

Routine Description:

    Reset all the MTRRs to a known state.

Arguments:

    PeiServices - General purpose services available to every PEIM.
    This        - Pointer to the instance of the PEI_CACHE_PPI.
    
Returns:

    EFI_SUCCESS - All MTRRs have been reset successfully.

--*/
{
  UINT32      MsrNum;
  UINT16      Index;
  UINT64      OldMtrr;
  UINT32      VariableMtrrLimit;

  VariableMtrrLimit = (UINT32) (AsmReadMsr64(EFI_IA32_MTRR_CAP) & B_EFI_IA32_MTRR_VARIABLE_SUPPORT);

  DisableCacheAsRam ();

  Index = 0;
  EfiDisableCacheMtrr (&OldMtrr);

  //
  // Reset Fixed Mtrrs
  //
  while (mFixedMtrrTable[Index].Msr != 0) {
    AsmWriteMsr64 (mFixedMtrrTable[Index].Msr, 0);
    Index++;
  }

  //
  // Reset Variable Mtrrs
  //
  for (MsrNum = EFI_CACHE_VARIABLE_MTRR_BASE; MsrNum < (EFI_CACHE_VARIABLE_MTRR_BASE + VariableMtrrLimit*2 - 1); MsrNum++) {
    AsmWriteMsr64 (MsrNum, 0);
  }

  EfiRecoverCacheMtrr (TRUE, OldMtrr);

  return EFI_SUCCESS;
}

EFI_STATUS
SearchForExactMtrr (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_PHYSICAL_ADDRESS      MemoryAddress,
  IN  UINT64                    MemoryLength,
  IN  UINT64                    ValidMtrrAddressMask,
  OUT UINT32                    *UsedMsrNum,
  OUT EFI_MEMORY_CACHE_TYPE     *UsedMemoryCacheType
  )
/*++

Routine Description:

  Search the memory cache type for specific memory from MTRR.

Arguments:

  PeiServices          - General purpose services available to every PEIM.
  MemoryAddress        - the address of target memory
  MemoryLength         - the length of target memory
  ValidMtrrAddressMask - the MTRR address mask
  UsedMsrNum           - the used MSR number
  UsedMemoryCacheType  - the cache type for the target memory

Returns:

  EFI_SUCCESS   - The memory is found in MTRR and cache type is returned
  EFI_NOT_FOUND - The memory is not found in MTRR

--*/
{
  UINT32  MsrNum;
  UINT64  TempQword;
  UINT32  VariableMtrrLimit;

  VariableMtrrLimit = (UINT32) (AsmReadMsr64(EFI_IA32_MTRR_CAP) & B_EFI_IA32_MTRR_VARIABLE_SUPPORT);

  for (MsrNum = EFI_CACHE_VARIABLE_MTRR_BASE; MsrNum < (EFI_CACHE_VARIABLE_MTRR_BASE + VariableMtrrLimit*2 - 1); MsrNum += 2) {

    TempQword = AsmReadMsr64 (MsrNum + 1);

    if ((TempQword & B_EFI_CACHE_MTRR_VALID) == 0) {
      continue;
    }

    if ((TempQword & ValidMtrrAddressMask) != ((~(MemoryLength - 1)) & ValidMtrrAddressMask)) {
      continue;
    }

    TempQword = AsmReadMsr64 (MsrNum);

    if ((TempQword & ValidMtrrAddressMask) != (MemoryAddress & ValidMtrrAddressMask)) {
      continue;
    }

    *UsedMemoryCacheType  = (EFI_MEMORY_CACHE_TYPE) (TempQword & 0xff);
    *UsedMsrNum           = MsrNum;

    return EFI_SUCCESS;

  }

  return EFI_NOT_FOUND;
}

BOOLEAN
IsDefaultType (
  IN  EFI_MEMORY_CACHE_TYPE     MemoryCacheType
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  MemoryCacheType - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
{

  if ((AsmReadMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE) & 0xff) != MemoryCacheType) {
    return FALSE;
  } else {
    return TRUE;
  }
}
