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

/** @file OA2.c
    Updated the XSDT with the SLIC OEM Activation ACPI Table.
 
**/

//---------------------------------------------------------------------------
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <AmiHobs.h>
#include "OA2.h"

//---------------------------------------------------------------------------


/**
    This fills the PubKey and Marker structures of SLIC table.

    @param 
        *FfsGuid    - Pointer to GUID of the FFS file to read
        *Address    - Address of the buffer to read data into
        Size        - Size of the buffer

         
    @retval EFI_SUCCESS Successful
    @retval EFI_NOT_FOUND Couldn't find the binaries

**/

EFI_STATUS LocateAndLoadRawData(
    IN EFI_GUID *FfsGuid,
    IN VOID *Address,
    IN UINTN Size )
{
    EFI_STATUS                   Status;
    EFI_HANDLE                   *HandleBuffer = NULL;
    UINTN                        NumberOfHandles;
    UINTN                        Index;
    UINT32                       Authentication;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVolumeProtocol = NULL;

    // Locate the Firmware volume protocol
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiFirmwareVolume2ProtocolGuid, 
        NULL,
        &NumberOfHandles,
        &HandleBuffer
    );
    if (EFI_ERROR(Status)) 
        return Status;
    
    // Find and read raw data
    for (Index = 0; Index < NumberOfHandles; Index++) {

        Status = gBS->HandleProtocol(
            HandleBuffer[Index],
            &gEfiFirmwareVolume2ProtocolGuid,
            &FwVolumeProtocol
        );
        if (EFI_ERROR(Status)) 
            continue;
        
        Status = FwVolumeProtocol->ReadSection(
            FwVolumeProtocol,
            FfsGuid,
            EFI_SECTION_RAW,
            0x0,
            &Address,
            &Size,
            &Authentication
        );
        if (Status == EFI_SUCCESS) 
            break;
    }
    
    gBS->FreePool(HandleBuffer);
    return Status;
}


/**
    This function preserves the Marker and PubKey binaries.

    @param VOID

    @retval EFI_STATUS based on result

**/

EFI_STATUS PreserveSlicBinaries(VOID)
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    RECOVERY_IMAGE_HOB *RecoveryHob;
    EFI_GUID RecoveryHobGuid = AMI_RECOVERY_IMAGE_HOB_GUID;
    EFI_GUID PubKeyGuid = OEM_SLP_PUBLIC_KEY_GUID;
    EFI_GUID MarkerGuid = OEM_SLP_MARKER_GUID;
    EFI_CONFIGURATION_TABLE *Table = gST->ConfigurationTable;
    EFI_PHYSICAL_ADDRESS Addr;
    UINTN  i;
    UINT8  *FwPubKeyPtr = NULL;
    UINT8  *FwMarkerPtr = NULL;
    UINT8  *RecPubKeyPtr = NULL;
    UINT8  *RecMarkerPtr = NULL;

    // Find the Product Key place in the recovery HOB
    for( i = 0; i < gST->NumberOfTableEntries; i++, Table++ ) {

        if ( !CompareMem(&Table->VendorGuid, &gEfiHobListGuid, sizeof(EFI_GUID)) ) {

            RecoveryHob = Table->VendorTable;
            RecoveryHob = GetNextGuidHob(&RecoveryHobGuid, RecoveryHob);
            if (RecoveryHob != NULL) {
                if((RecoveryHob->Status == EFI_SUCCESS) && (RecoveryHob->Address != 0)) {

                    // Find Public Key and Marker binaries in the Recovery image
                    Status = EFI_NOT_FOUND;
    
                    for (Addr = RecoveryHob->Address + FV_MAIN_OFFSET; Addr < RecoveryHob->Address + RecoveryHob->ImageSize; Addr = Addr + 4 ) {
                        // Find the Public Key binary GUID
                        if ( !CompareMem((UINT8*)Addr, &PubKeyGuid, sizeof(EFI_GUID)) )
                            RecPubKeyPtr = (UINT8*)Addr;
    
                        // Find the Marker binary GUID
                        else if ( !CompareMem((UINT8*)Addr, &MarkerGuid, sizeof(EFI_GUID)) )
                            RecMarkerPtr = (UINT8*)Addr;
    
                        // if all binaries are found then find original places in the Firmware
                        if ( RecPubKeyPtr != NULL && RecMarkerPtr != NULL ) {
    
                            // Find Public Key and Marker binaries in the BIOS firmware
                            for ( Addr = FLASH_DEVICE_BASE_ADDRESS + FV_MAIN_OFFSET; Addr < FLASH_UPPER_ADDRESS; Addr = Addr + 4 ) { 
            
                                // Find the Public Key binary GUID
                                if ( !CompareMem((UINT8*)Addr, &PubKeyGuid, sizeof(EFI_GUID)) )
                                    FwPubKeyPtr = (UINT8*)Addr;
    
                                // Find the Marker binary GUID
                                else if ( !CompareMem((UINT8*)Addr, &MarkerGuid, sizeof(EFI_GUID)) )
                                    FwMarkerPtr = (UINT8*)Addr;
            
                                // Preserve the SLIC binaries
                                if ( FwPubKeyPtr != NULL && FwMarkerPtr != NULL ) {
    
                                    CopyMem( 
                                        RecPubKeyPtr, 
                                        FwPubKeyPtr, 
                                        sizeof(OEM_PUBLIC_KEY_STRUCTURE) + sizeof(EFI_FFS_FILE_HEADER) + sizeof(UINT16)
                                    );
    
                                    CopyMem( 
                                        RecMarkerPtr, 
                                        FwMarkerPtr, 
                                        sizeof(WINDOWS_MARKER_STRUCTURE) + sizeof(EFI_FFS_FILE_HEADER) + sizeof(UINT16)
                                    );
    
                                    Status = EFI_SUCCESS;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    DEBUG ((DEBUG_INFO, "PreserveSlicBinaries: Status = %r\n",Status));
    return Status;
}


/**
    This function publish SLIC table in the ACPI.

    @param VOID

    @retval EFI_SUCCESS - SLIC table has been published
	@retval EFI_NOT_FOUND - SLP Public Key Binaries or Marker or AcpiTableProtocol was Not Found
	@retval other EFI_ERROR - Unable to publish Acpi Table

**/

EFI_STATUS PublishSlicTable(VOID)
{
    EFI_STATUS Status;
    EFI_GUID PubKeyGuid = OEM_SLP_PUBLIC_KEY_GUID;
    EFI_GUID MarkerGuid = OEM_SLP_MARKER_GUID;
    EFI_ACPI_SLP SlpTable;
    EFI_ACPI_TABLE_PROTOCOL *AcpiTableProtocol = NULL;
    UINTN  TableKey = 0;

    SlpTable.Header.Signature = SLIC_SIG;
    SlpTable.Header.Length = sizeof(EFI_ACPI_SLP);
    SlpTable.Header.Revision = 0x01;
    SlpTable.Header.Checksum = 0;
    SetMem(& SlpTable.Header.OemId, sizeof (SlpTable.Header.OemId), 0);
    SlpTable.Header.OemTableId = 0;
    SlpTable.Header.OemRevision = ACPI_OEM_REV;
    SlpTable.Header.CreatorId = CREATOR_ID_AMI;
    SlpTable.Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
    SetMem(&SlpTable.PubKey, sizeof(OEM_PUBLIC_KEY_STRUCTURE), 0);
    SetMem(&SlpTable.WinMarker, sizeof(WINDOWS_MARKER_STRUCTURE), 0);
    
    // Locate SLP Public Key binary
    Status = LocateAndLoadRawData(
        &PubKeyGuid, 
        &SlpTable.PubKey, 
        sizeof(SlpTable.PubKey)
    );
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "SLP Public Key Binaries Not Found!\n"));
        return Status;
    }

    // Locate SLP Marker binary
    Status = LocateAndLoadRawData(
        &MarkerGuid, 
        &SlpTable.WinMarker, 
        sizeof(SlpTable.WinMarker)
    );
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "SLP Marker Binaries are Not Found!\n"));
        return Status;
    }

    // The dummy SLIC will be not published
    if ( SlpTable.WinMarker.StructType == 0xFFFFFFFF && SlpTable.PubKey.StructType == 0xFFFFFFFF ) {
        DEBUG ((DEBUG_ERROR, "OA2: Found dummy binaries. The SLIC will be not published!\n"));
        return EFI_ABORTED;
    }

    // Copy OEM ID and OEM Table ID from Marker's binary to SLIC header
    CopyMem( 
        (UINT8*)&SlpTable.Header.OemId,
        (UINT8*)&SlpTable.WinMarker.sOEMID,
        sizeof(SlpTable.WinMarker.sOEMID)
    );
    CopyMem( 
        (UINT8*)&SlpTable.Header.OemTableId,
        (UINT8*)&SlpTable.WinMarker.sOEMTABLEID,
        sizeof(SlpTable.WinMarker.sOEMTABLEID)
    );

    // Locate the ACPI table protocol
    Status = gBS->LocateProtocol(
        &gEfiAcpiTableProtocolGuid,
        NULL, 
        &AcpiTableProtocol
    );
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "Unable to locate AcpiTableProtocol!\n"));
        return Status;
    }

    // Publish SLIC to ACPI table
    Status = AcpiTableProtocol->InstallAcpiTable(
        AcpiTableProtocol,
        &SlpTable,
        sizeof(EFI_ACPI_SLP), 
        &TableKey
    );
    if (!EFI_ERROR(Status))
        DEBUG ((DEBUG_INFO, "SLIC table has been published.\n"));

    return Status;
}


/**
    This function is the entry point of the eModule.

    @param 
        ImageHandle  - Image handle
        *SystemTable - Pointer to the system table

    @retval EFI_SUCCESS - Slic Table was published (and preserved if we are in recovery mode)
	@retval EFI_ERROR - Slic table was not published for some reasons. 

**/

EFI_STATUS EFIAPI OA2_EntryPoint(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable )
{
    EFI_STATUS Status = EFI_SUCCESS;

    // Check if it's a Recovery Mode then preserve the Marker and PubKey binaries
    if ((GetBootModeHob () == BOOT_IN_RECOVERY_MODE) || (GetBootModeHob () == BOOT_ON_FLASH_UPDATE))
        Status = PreserveSlicBinaries();
    if (!EFI_ERROR(Status))
        Status = PublishSlicTable();

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
