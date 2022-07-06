//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************


/** @file OA1.c
    Microsoft OEM Activation 1.0

**/

#include "OA1.h"
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LegacyBiosExt.h>


/**
    This function locate resource binary by GUID. 

        
    @param ResourceGuid pointer to resource GUID
    @param ResourceSectionGuid pointer to resource section GUID
    @param Address pointer to returning address of the resource

    @retval EFI_STATUS

**/

static EFI_STATUS LocateSectionResource(
    IN  EFI_GUID *ResourceFileGuid,
    IN  EFI_GUID *ResourceSectionGuid,
    OUT UINT8 **Address )
{
    EFI_STATUS Status;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVolumeProtocol = NULL;
    EFI_HANDLE *HandleBuffer = NULL;
    UINTN  NumberOfHandles;
    UINTN  Index;
    UINT32 Authentication;
    UINTN  i;
    EFI_GUID *SectionGuid = NULL;
    UINTN  SectionSize;

    // Locate the Firmware volume protocol
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiFirmwareVolume2ProtocolGuid,
        NULL,
        &NumberOfHandles,
        &HandleBuffer
    );
    if (EFI_ERROR(Status)) 
        return EFI_NOT_FOUND;
    
    // Find and read raw data
    for (Index = 0; Index < NumberOfHandles; Index++) {

        Status = gBS->HandleProtocol(
            HandleBuffer[Index],
            &gEfiFirmwareVolume2ProtocolGuid,
            &FwVolumeProtocol
        );
        if (EFI_ERROR(Status)) 
            continue;
        
        i = 0;
        while(1) {
            SectionSize = 0;
            Status = FwVolumeProtocol->ReadSection(
                FwVolumeProtocol,
                ResourceFileGuid,
                EFI_SECTION_FREEFORM_SUBTYPE_GUID,
                i++,
                &SectionGuid,
                &SectionSize,
                &Authentication
            );
            if (EFI_ERROR(Status))
                break;
            else if ( CompareGuid(SectionGuid, ResourceSectionGuid) ) {
                *Address = (UINT8*)SectionGuid;
                goto LocateSectionResource_exit;
            }
            gBS->FreePool(SectionGuid);
            SectionGuid = NULL;
        } 
    }
    
LocateSectionResource_exit:
    gBS->FreePool(HandleBuffer);
    return Status;
}


/**
    This function is the entry point of the OA1 eModule.
               
    @param 
        ImageHandle - Image handle for this driver image.
        SystemTable - Pointer to the EFI system table.

    @retval EFI_STATUS
 
**/

EFI_STATUS OA1_EntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status;
    EFI_GUID EfiLegacyBiosExtProtocolGuid = EFI_LEGACY_BIOS_EXT_PROTOCOL_GUID;
    EFI_GUID BiosFeaturesSectionGuid = BIOS_FEATURES_SECTION_GUID;
    EFI_GUID Oa10FileGuid = OA10_FILE_GUID;
    EFI_LEGACY_BIOS_EXT_PROTOCOL *LegacyBiosExtProtocol = NULL;
    UINT8 *DataPtr = NULL;
    UINTN Address;

    // Get OEM data from APTIO FV
    Status = LocateSectionResource(
        &Oa10FileGuid, 
        &BiosFeaturesSectionGuid, 
        &DataPtr
    );
    if (EFI_ERROR(Status))
        return Status;

    // Locate the APTIO extended Legacy Protocol
    Status = gBS->LocateProtocol( 
        &EfiLegacyBiosExtProtocolGuid, 
        NULL, 
        &LegacyBiosExtProtocol
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    // Copy OEM Data to Legacy Table
    Address = LegacyBiosExtProtocol->CopyLegacyTable(
        ((BIOS_OEM_DATA*)DataPtr)->Data,    // Pointer to OEM data
        OEM_DATA_SPACE_SIZE,                // Size of the data
        1,                                  // Alignment
        OEM_DATA_LOCATION_BIT               // Location Attribute
    );
    ASSERT_EFI_ERROR(Address);
    DEBUG ((DEBUG_INFO, "OEM Activation 1.0 CopyLegacyTable Address=%X\n",Address));

    Status = gBS->FreePool(DataPtr);
    ASSERT_EFI_ERROR(Status);
    return Status;
}

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
