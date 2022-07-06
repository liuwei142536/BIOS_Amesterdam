/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2004 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  FlashMap.c
   
Abstract:

  Build GUIDed HOBs for platform specific flash map.

--*/
#include <FlashLayout.h>

#include <Ppi/FlashMap.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/FlashMapHob.h>
#include <Guid/SystemNvDataGuid.h>
#include <Guid/SystemNvDataHobGuid.h>
#include <Guid/FirmwareFileSystem2.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>

EFI_STATUS
EFIAPI
GetAreaInfo (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN PEI_FLASH_MAP_PPI            * This,
  IN  EFI_FLASH_AREA_TYPE         AreaType,
  IN  EFI_GUID                    *AreaTypeGuid,
  OUT UINT32                      *NumEntries,
  OUT EFI_FLASH_SUBAREA_ENTRY     **Entries
  );

EFI_GUID                            mFvBlockGuid      = EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID;

EFI_GUID                            mFfsGuid          = EFI_FIRMWARE_FILE_SYSTEM2_GUID;

EFI_GUID                            mSystemDataGuid   = EFI_SYSTEM_NV_DATA_HOB_GUID;

STATIC PEI_FLASH_MAP_PPI            mFlashMapPpi = { GetAreaInfo };

STATIC EFI_PEI_PPI_DESCRIPTOR       mPpiListFlashMap = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiFlashMapPpiGuid,
  &mFlashMapPpi
};

static EFI_FLASH_AREA_DATA          mFlashAreaData[]  = { EFI_FLASH_AREA_DATA_DEFINITION };

#define NUM_FLASH_AREA_DATA (sizeof (mFlashAreaData) / sizeof (mFlashAreaData[0]))

static EFI_HOB_FLASH_MAP_ENTRY_TYPE mFlashMapHobData[] = { EFI_HOB_FLASH_MAP_ENTRY_TYPE_DATA_DEFINITION };

#define NUM_HOB_FLASH_MAP_DATA  (sizeof (mFlashMapHobData) / sizeof (mFlashMapHobData[0]))

EFI_STATUS
PeimInstallFlashMapPpi (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  )
{
  EFI_STATUS                    Status;
  //
  // Install FlashMap PPI
  //
  Status = PeiServicesInstallPpi (&mPpiListFlashMap);
  ASSERT_EFI_ERROR (Status);  

  DEBUG ((EFI_D_INFO, "Flash Map PEIM Loaded\n"));
  return EFI_SUCCESS;
}  

EFI_STATUS
PeimInitializeFlashMap (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Build GUID HOBs for platform specific flash map.
  
Arguments:

  FfsHeader     Pointer this FFS file header.
  PeiServices   General purpose services available to every PEIM.
    
Returns:

  EFI_SUCCESS   Guid HOBs for platform flash map is built.
  Otherwise     Failed to build the Guid HOB data.

--*/
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  EFI_FLASH_AREA_HOB_DATA       FlashHobData;
  EFI_HOB_FLASH_MAP_ENTRY_TYPE  *Hob;
  EFI_BOOT_MODE                 BootMode;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Build flash area entries as GUIDed HOBs.
  //
  for (Index = 0; Index < NUM_FLASH_AREA_DATA; Index++) {
    SetMem (&FlashHobData, sizeof (EFI_FLASH_AREA_HOB_DATA), 0);

    FlashHobData.AreaType               = mFlashAreaData[Index].AreaType;
    FlashHobData.NumberOfEntries        = 1;
    FlashHobData.SubAreaData.Attributes = mFlashAreaData[Index].Attributes;
    FlashHobData.SubAreaData.Base       = (EFI_PHYSICAL_ADDRESS) (UINTN) mFlashAreaData[Index].Base;
    FlashHobData.SubAreaData.Length     = (EFI_PHYSICAL_ADDRESS) (UINTN) mFlashAreaData[Index].Length;

    // when it's in Recovery mode, use Recovery MRC instead
//#ifdef SPLIT_RECOVERY_FV // AptioV Server Override: Split Recovery Support Not required
    if ((BootMode == BOOT_IN_RECOVERY_MODE)
        && (mFlashAreaData[Index].Base == FLASH_REGION_FV_MRC_NORMAL_BASE)) {
      FlashHobData.SubAreaData.Base     = (EFI_PHYSICAL_ADDRESS) (UINTN) FLASH_REGION_FV_MRC_RECOVERY_BASE;
      DEBUG ((EFI_D_ERROR, "Recovery Mode, Recovery MRC:\n Base:%x\n Size:%x\n", FLASH_REGION_FV_MRC_RECOVERY_BASE, FLASH_REGION_FV_MRC_RECOVERY_SIZE));
    }
//#endif // AptioV Server Override: Split Recovery Support Not required


    switch (FlashHobData.AreaType) {
    case EFI_FLASH_AREA_RECOVERY_BIOS:
    case EFI_FLASH_AREA_MAIN_BIOS:
      CopyMem (
        &FlashHobData.AreaTypeGuid,
        &mFfsGuid,
        sizeof (EFI_GUID)
        );
      CopyMem (
        &FlashHobData.SubAreaData.FileSystem,
        &mFvBlockGuid,
        sizeof (EFI_GUID)
        );
      break;

    case EFI_FLASH_AREA_GUID_DEFINED:
      CopyMem (
        &FlashHobData.AreaTypeGuid,
        &mSystemDataGuid,
        sizeof (EFI_GUID)
        );
      CopyMem (
        &FlashHobData.SubAreaData.FileSystem,
        &mFvBlockGuid,
        sizeof (EFI_GUID)
        );
      break;

    default:
      break;
    }

    Hob = BuildGuidDataHob (
            &gEfiFlashMapHobGuid,
            &FlashHobData,
            sizeof (EFI_FLASH_AREA_HOB_DATA)
            );
    ASSERT (Hob);
  }

  for (Index = 0; Index < NUM_HOB_FLASH_MAP_DATA; Index++) {
    Status = PeiServicesCreateHob (
              EFI_HOB_TYPE_GUID_EXTENSION,
              (UINT16) (sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE)),
              &Hob
              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    CopyMem (
      Hob,
      &mFlashMapHobData[Index],
      sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE)
      );
    if (mFlashMapHobData[Index].AreaType == EFI_FLASH_AREA_EFI_VARIABLES) {
      Hob->Entries[0].Base    = Hob->Entries[0].Base + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;
      Hob->Entries[0].Length  = Hob->Entries[0].Length - EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetAreaInfo (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN PEI_FLASH_MAP_PPI            * This,
  IN  EFI_FLASH_AREA_TYPE         AreaType,
  IN  EFI_GUID                    *AreaTypeGuid,
  OUT UINT32                      *NumEntries,
  OUT EFI_FLASH_SUBAREA_ENTRY     **Entries
  )
/*++

Routine Description:

  Get data from the platform specific flash area map.
  
Arguments:

  PeiServices   General purpose services available to every PEIM.
  AreaType      Flash map area type.
  AreaTypeGuid  Guid for the flash map area type.
  NumEntries    On return, filled with the number of sub-areas with the same type.
  Entries       On return, filled with entry pointer to the sub-areas.
    
Returns:

  EFI_SUCCESS   The type of area exists in the flash map and data is returned.
  EFI_NOT_FOUND The type of area does not exist in the flash map.

--*/
{
  EFI_PEI_HOB_POINTERS          Hob;
  EFI_HOB_FLASH_MAP_ENTRY_TYPE  *FlashMapEntry;

  Hob.Raw = GetHobList ();
  while (Hob.Raw != NULL) {
    Hob.Raw = GET_NEXT_HOB (Hob);
    Hob.Raw = GetNextHob (EFI_HOB_TYPE_GUID_EXTENSION, Hob.Raw);
    if (CompareGuid (&Hob.Guid->Name, &gEfiFlashMapHobGuid)) {
      FlashMapEntry = (EFI_HOB_FLASH_MAP_ENTRY_TYPE *) Hob.Raw;
      if ((AreaType != FlashMapEntry->AreaType) ||
          ((AreaType == EFI_FLASH_AREA_GUID_DEFINED) && !CompareGuid (AreaTypeGuid, &FlashMapEntry->AreaTypeGuid))
          ) {
        continue;
      }

      *NumEntries = FlashMapEntry->NumEntries;
      *Entries    = FlashMapEntry->Entries;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}
