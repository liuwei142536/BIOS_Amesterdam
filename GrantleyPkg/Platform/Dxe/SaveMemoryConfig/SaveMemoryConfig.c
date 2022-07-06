/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2007 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  SaveMemoryConfig.c
  
Abstract: 
  This is the driver that locates the MemoryConfigurationData HOB, if it
  exists, and saves the data to nvRAM.  

Revision History

--*/

#include <Library\DebugLib.h>
#include <Library\HobLib.h>
#include <Library\MemoryAllocationLib.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\UefiRuntimeServicesTableLib.h>
#include <Library\BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>

#include <Guid/MemoryConfigData.h>
#include <Guid/DebugMask.h>

CHAR16    EfiMemoryConfigVariable[] = L"MemoryConfig";

EFI_STATUS
EFIAPI
SaveMemoryConfigEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++
  
  Routine Description:
    This is the standard EFI driver point that detects whether there is a
    MemoryConfigurationData HOB and, if so, saves its data to nvRAM.

  Arguments:
    ImageHandle   - Handle for the image of this driver
    SystemTable   - Pointer to the EFI System Table

  Returns:
    EFI_SUCCESS   - if the data is successfully saved or there was no data
    EFI_NOT_FOUND - if the HOB list could not be located.
    EFI_UNLOAD_IMAGE - It is not success
    
--*/
{
  EFI_STATUS  Status;
  VOID        *HobList;
  EFI_HOB_GUID_TYPE *GuidHob;
  VOID        *HobData;
  VOID        *VariableData;
  UINTN       DataSize;
  UINTN       BufferSize;

  DataSize      = 0;
  VariableData  = NULL;
  GuidHob = NULL;
  HobList = NULL;
  HobData = NULL;
  Status  = EFI_UNSUPPORTED;
  //
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  HobList = GetHobList ();
  ASSERT (HobList != NULL);

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = GetNextGuidHob (&gEfiMemoryConfigDataGuid, HobList);
  if (GuidHob != NULL) {
    HobData = GET_GUID_HOB_DATA (GuidHob);
    DataSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
    //
    // Use the HOB to save Memory Configuration Data
    //
    BufferSize = DataSize;
    VariableData = AllocatePool (BufferSize);
    ASSERT (VariableData != NULL); 
    Status = gRT->GetVariable (
                    EfiMemoryConfigVariable,
                    &gEfiMemoryConfigDataGuid,
                    NULL,
                    &BufferSize,
                    VariableData
                    );
    if (Status == EFI_BUFFER_TOO_SMALL) {
      (gBS->FreePool)(VariableData);
      VariableData = AllocatePool (BufferSize);
      ASSERT (VariableData != NULL);      
      Status = gRT->GetVariable (
                      EfiMemoryConfigVariable,
                      &gEfiMemoryConfigDataGuid,
                      NULL,
                      &BufferSize,
                      VariableData
                      );
    }

    if ( (EFI_ERROR(Status)) || BufferSize != DataSize || 0 != CompareMem (HobData, VariableData, DataSize)) {
      if (Status != EFI_SUCCESS){
        Status = gRT->SetVariable (
                        EfiMemoryConfigVariable,
                        &gEfiGenericVariableGuid,
                        (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                        DataSize,
                        HobData
                        );
      }
      ASSERT((Status == EFI_SUCCESS) || (Status == EFI_OUT_OF_RESOURCES));

      DEBUG((EFI_D_ERROR, "Restored Size is 0x%x\n", DataSize));
    }
    (gBS->FreePool)(VariableData);
  }
  //
  // This driver does not produce any protocol services, so always unload it.
  //
  return Status;
}
