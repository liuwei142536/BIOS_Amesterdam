/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2005 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemoryPeim.c
   
Abstract:

  Tiano PEIM to provide the platform support functionality on the Bridgeport.
  This file implements the Platform Memory Range PPI

--*/

#include "PlatformEarlyInit.h"

/* EDK2_TODO Use below code to boot on softSDV. Fix is work in progress.
EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  {
    EfiACPIReclaimMemory,
    0x20
  },
  {
    EfiACPIMemoryNVS,
    0x100
  },
  {
    EfiReservedMemoryType,
    0x50
  },
  {
    EfiMemoryMappedIO,
    0
  },
  {
    EfiMemoryMappedIOPortSpace,
    0
  },
  {
    EfiPalCode,
    0
  },
  {
    EfiRuntimeServicesCode,
    0x50
  },
  {
    EfiRuntimeServicesData,
    0x50
  },
  {
    EfiLoaderCode,
    0
  },
  {
    EfiLoaderData,
    0
  },
  {
    EfiBootServicesCode,
    0
  },
  {
    EfiBootServicesData,
    0
  },
  {
    EfiConventionalMemory,
    0
  },
  {
    EfiUnusableMemory,
    0
  },
  {
    EfiMaxMemoryType,
    0
  }
};
*/

EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  {
    EfiACPIReclaimMemory,
    0x100
  },
  {
    EfiACPIMemoryNVS,
    0xA30
  },
  {
    EfiReservedMemoryType,
    0x100
  },
  {
    EfiRuntimeServicesCode,
    0x100
  },
  {
    EfiRuntimeServicesData,
    0x100
  },
  {
    EfiMaxMemoryType,
    0
  }
};

EFI_STATUS
EFIAPI
GetPlatformMemorySize (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      PEI_PLATFORM_MEMORY_SIZE_PPI           *This,
  IN OUT  UINT64                                 *MemorySize
  )
/*++

Routine Description:

  Build Memory type information.

Arguments:

  PeiServices - Pointer to Pei Services.
  This        - GC_TODO: add argument description
  MemorySize  - GC_TODO: add argument description

Returns:

  EFI_SUCCESS  -  Success.
  Others       -  Errors have occurred.
--*/
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  UINTN                           DataSize=0, LastIndex=0;
  EFI_MEMORY_TYPE_INFORMATION     MemoryData[EfiMaxMemoryType + 1];
  UINTN                           Index;
  EFI_BOOT_MODE                   BootMode;
  UINTN                           IndexNumber;
  UINTN                           *HobPointer;

  *MemorySize = PEI_MIN_MEMORY_SIZE;
  Status = PeiServicesLocatePpi (
           &gEfiPeiReadOnlyVariable2PpiGuid,
            0,
            NULL,
            &Variable
            );

  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (MemoryData);

  Status = Variable->GetVariable (
                      Variable,
                      EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME,
                      &gEfiMemoryTypeInformationGuid,
                      NULL,
                      &DataSize,
                      &MemoryData
                      );

  IndexNumber = sizeof (mDefaultMemoryTypeInformation) / sizeof (EFI_MEMORY_TYPE_INFORMATION);

  //
  // Accumulate maximum amount of memory needed
  //
  if (DataSize) {
  	LastIndex = (DataSize/sizeof(EFI_MEMORY_TYPE_INFORMATION))-1;
  }
  
  // AptioV server override start: Resolved Build error when Optimization is disabled
  //  DEBUG((EFI_D_ERROR, "PEI_MIN_MEMORY_SIZE:%dKB \n", *MemorySize/1024));
    DEBUG((EFI_D_ERROR, "PEI_MIN_MEMORY_SIZE:%dKB \n", RShiftU64 (*MemorySize, 10)));
  // AptioV server override end: Resolved Build error when Optimization is disabled
  DEBUG((EFI_D_ERROR, "IndexNumber:%d MemoryDataNumber:%d LastIndex MemoryTypeValue:%d \n", IndexNumber,DataSize/ sizeof (EFI_MEMORY_TYPE_INFORMATION), MemoryData[LastIndex].Type));
  if (EFI_ERROR (Status) || MemoryData[LastIndex].Type != EfiMaxMemoryType) {
    //
    // Start with minimum memory, if no memory type information is found, or stored memory type information conflicts with latest MaxMemoryType
    //
    for (Index = 0; Index < IndexNumber; Index++) {
      DEBUG((EFI_D_ERROR, "Index[%d].Type = %d .NumberOfPages=0x%x\n", Index,mDefaultMemoryTypeInformation[Index].Type,mDefaultMemoryTypeInformation[Index].NumberOfPages));
      *MemorySize += mDefaultMemoryTypeInformation[Index].NumberOfPages * EFI_PAGE_SIZE;
    }
    // AptioV server override start: Resolved Build error when Optimization is disabled
    //    DEBUG((EFI_D_ERROR, "No memory type,  Total platform memory:%dKB \n", *MemorySize/1024));
         DEBUG((EFI_D_ERROR, "No memory type,  Total platform memory:%dKB \n", RShiftU64 (*MemorySize, 10)));
    // AptioV server override end: Resolved Build error when Optimization is disabled
    //
    // Build the GUID'd HOB for DXE
    //
    HobPointer = BuildGuidDataHob (
                  &gEfiMemoryTypeInformationGuid,
                  mDefaultMemoryTypeInformation,
                  sizeof (mDefaultMemoryTypeInformation)
                  );
  } else {
    //
    // Start with at least 0x200 pages of memory for the DXE Core and the DXE Stack
    //
    for (Index = 0; Index < IndexNumber; Index++) {
      DEBUG((EFI_D_ERROR, "Index[%d].Type = %d .NumberOfPages=0x%x\n", Index,MemoryData[Index].Type,MemoryData[Index].NumberOfPages));
      *MemorySize += MemoryData[Index].NumberOfPages * EFI_PAGE_SIZE;
      
    }
    // AptioV server override start: Resolved Build error when Optimization is disabled
    //    DEBUG((EFI_D_ERROR, "has memory type,  Total platform memory:%dKB \n", *MemorySize/1024));
    	DEBUG((EFI_D_ERROR, "has memory type,  Total platform memory:%dKB \n", RShiftU64 (*MemorySize, 10)));
    // AptioV server override end: Resolved Build error when Optimization is disabled
    //
    // Build the GUID'd HOB for DXE
    //
	// Aptio V Server override: WA to allow DXE Load code to construct this HOB.
	/*
    HobPointer = BuildGuidDataHob (
                  &gEfiMemoryTypeInformationGuid,
                  MemoryData,
                  DataSize
                  );
      */
  }

  ASSERT (HobPointer);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN  EFI_PEI_SERVICES                   **PeiServices,
  IN  PEI_BASE_MEMORY_TEST_PPI           *This,
  IN  EFI_PHYSICAL_ADDRESS               BeginAddress,
  IN  UINT64                             MemoryLength,
  IN  PEI_MEMORY_TEST_OP                 Operation,
  OUT EFI_PHYSICAL_ADDRESS               *ErrorAddress
  )
/*++

Routine Description:

  This function checks the memory range in PEI. 

Arguments:

  PeiServices     Pointer to PEI Services.
  This            Pei memory test PPI pointer.
  BeginAddress    Beginning of the memory address to be checked.
  MemoryLength    Bytes of memory range to be checked.
  Operation       Type of memory check operation to be performed.
  ErrorAddress    Return the address of the error memory address.
    
Returns:
  
  EFI_SUCCESS         The operation completed successfully.
  EFI_DEVICE_ERROR    Memory test failed. It's not safe to use this range of memory.

--*/
{
  UINT32                TestPattern;
  UINT32                TestMask;
  UINT32                SpanSize;
  EFI_PHYSICAL_ADDRESS  TempAddress;

  ReportStatusCode (
    EFI_PROGRESS_CODE,
    EFI_COMPUTING_UNIT_MEMORY + EFI_CU_MEMORY_PC_TEST
    );

  TestPattern = MEMORY_TEST_PATTERN;
  TestMask    = 0;
  SpanSize    = 0;

  //
  // Make sure we don't try and test anything above the max physical address range
  //
  ASSERT (BeginAddress + MemoryLength < EFI_MAX_ADDRESS);

  switch (Operation) {
  case Extensive:
    SpanSize = 0x4;
    break;

  case Sparse:
  case Quick:
    SpanSize = MEMORY_TEST_COVER_SPAN;
    break;

  case Ignore:
    goto Done;
    break;
  }
  //
  // Write the test pattern into memory range
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    (*(UINT32 *) (UINTN) TempAddress) = TestPattern;
    TempAddress += SpanSize;
  }
  //
  // Read pattern from memory and compare it
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    if ((*(UINT32 *) (UINTN) TempAddress) != TestPattern) {
      *ErrorAddress = TempAddress;
      ReportStatusCode (
        EFI_ERROR_CODE + EFI_ERROR_UNRECOVERED,
        EFI_COMPUTING_UNIT_MEMORY + EFI_CU_MEMORY_EC_UNCORRECTABLE
        );
      return EFI_DEVICE_ERROR;
    }

    TempAddress += SpanSize;
  }

Done:

  return EFI_SUCCESS;
}
