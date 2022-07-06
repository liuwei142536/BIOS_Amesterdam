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

/** @file OA3.c
    Microsoft OEM Activation 3.0

**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <Ppi/FwVersion.h>
#include <AmiHobs.h>
#include "OA3.h"
#if OEM_ACTIVATION_TABLE_LOCATION == 2
#include <OA3TableAddressElink.h>
#endif

#if OEM_ACTIVATION_TABLE_LOCATION == 2
// Type Definition(s)
typedef VOID (OA3_TABLE_ADDRESS_FUNC) (  
    OUT EFI_PHYSICAL_ADDRESS *OA3TableAddress);

// External Declaration(s)
extern OA3_TABLE_ADDRESS_FUNC OA3_TABLE_ADDRESS_ELINK EndOfList;

OA3_TABLE_ADDRESS_FUNC* OA3TableAddressElink[] = \
                                            {OA3_TABLE_ADDRESS_ELINK NULL};
#endif

//----------------------------------------------------------------------------



/**
    This function locate resource binary by GUID. 

    @param 
        pResourceGuid        - pointer to recourse GUID
        pResourceSectionGuid - pointer to recourse section GUID
        Address              - pointer to returning address of the resource

    @retval EFI_STATUS

**/

EFI_STATUS LocateSectionResource(
    IN  EFI_GUID *pResourceGuid,
    IN  EFI_GUID *pResourceSectionGuid,
    OUT UINT8 **Address )
{
    EFI_STATUS Status;
    EFI_HANDLE *HandleBuffer = NULL;
    UINTN  NumberOfHandles;
    UINTN  Index;
    UINT32 Authentication;
    UINTN  i;
    EFI_GUID *pGuid = NULL;
    UINTN  SectionSize;
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
                pResourceGuid,
                EFI_SECTION_FREEFORM_SUBTYPE_GUID,
                i++,
                &pGuid,
                &SectionSize,
                &Authentication
            );
            if (EFI_ERROR(Status))
                break;
            else if ( CompareGuid(pGuid, pResourceSectionGuid ) ) {
                *Address = (UINT8*)pGuid;
                goto LocateSectionResource_exit;
            }
            gBS->FreePool(pGuid);
            pGuid = NULL;
        } 
    }
    
LocateSectionResource_exit:
    gBS->FreePool(HandleBuffer);
    return Status;
}

#if OEM_ACTIVATION_TABLE_LOCATION == 0
/**
    This function locates the Product Key. 

    @param Address - pointer to returning address of the Product Key structure.

    @retval EFI_STATUS

**/

EFI_STATUS GetProductKey( 
    IN EFI_PHYSICAL_ADDRESS StartAddr, 
    IN EFI_PHYSICAL_ADDRESS EndAddr, 
    OUT UINT8 **Address )
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    EFI_GUID FidTableSectionGuid = FID_TABLE_SECTION_GUID;
    EFI_PHYSICAL_ADDRESS i;

    for ( i = StartAddr; i < EndAddr; i = i + 4 )
        if ( !CompareMem((UINT8*)(UINTN)i, &FidTableSectionGuid, sizeof(EFI_GUID)) ) {

            // skip the GUID
            i += sizeof(EFI_GUID);

            // Check version of the FID table
            if ( ((FW_VERSION*)(UINTN)i)->StructVersion < FID_SUPPORTED_VERSION ) {
                DEBUG ((DEBUG_ERROR, "OEM Activation: FID table version %d unsupported!!!\n",((FW_VERSION*)i)->StructVersion));
                Status = EFI_UNSUPPORTED;
                break;
            }

            // skip the ACPI header
            *Address = (UINT8*)((FW_VERSION*)(UINTN)i)->OemActivationKey;
            DEBUG ((DEBUG_INFO, "OEM Activation: Product Key Address %X\n",*Address));

            Status = EFI_SUCCESS;
            break;
        }
    return Status;
}
#endif

/**
    This function preserves the Product Key.

    @param VOID

    @retval EFI_STATUS

**/

EFI_STATUS PreserveProductKey(VOID)
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    RECOVERY_IMAGE_HOB *RecoveryHob; 
    EFI_GUID RecoveryHobGuid = AMI_RECOVERY_IMAGE_HOB_GUID;
    EFI_CONFIGURATION_TABLE *Table = gST->ConfigurationTable;
    UINTN  i;
#if OEM_ACTIVATION_TABLE_LOCATION != 2
    UINT8  *ProductKey;
    UINT8  *ProductKeyPreserve;
#endif

    // Find the Product Key place in the recovery HOB
    for( i = 0; i < gST->NumberOfTableEntries; i++, Table++ ) {

        if ( !CompareMem(&Table->VendorGuid, &gEfiHobListGuid, sizeof(EFI_GUID)) ) {

            RecoveryHob = Table->VendorTable;
            RecoveryHob = GetNextGuidHob(&RecoveryHobGuid, RecoveryHob);
            if (RecoveryHob != NULL) {
                if((RecoveryHob->Status == EFI_SUCCESS) && (RecoveryHob->Address != 0)) {
#if OEM_ACTIVATION_TABLE_LOCATION == 0
                // Get the Product Key pointer in the recovery HOB
                    Status = GetProductKey(
                        RecoveryHob->Address + FLASH_SIZE - FV_BB_SIZE, 
                        RecoveryHob->Address + FLASH_SIZE - sizeof(EFI_GUID), 
                        &ProductKeyPreserve
                    );
                    if (!EFI_ERROR(Status)) {
                        // Get the Product Key pointer in the Flash memory
                        Status = GetProductKey(FV_BB_BASE, FV_BB_END_ADDR, &ProductKey);
                        if (EFI_ERROR(Status))
                            break;
                        CopyMem( ProductKeyPreserve, ProductKey, sizeof(EFI_ACPI_MSDM_TABLE) - sizeof(EFI_ACPI_DESCRIPTION_HEADER) );
                    }
#elif OEM_ACTIVATION_TABLE_LOCATION == 1
                    ProductKeyPreserve = (UINT8*)(RecoveryHob->Address + FLASH_SIZE - OEM_ACTIVATION_TABLE_OFFSET);
                    ProductKey = (UINT8*)OEM_ACTIVATION_TABLE_ADDRESS;
                    CopyMem( ProductKeyPreserve, ProductKey, sizeof(EFI_ACPI_MSDM_TABLE) - sizeof(EFI_ACPI_DESCRIPTION_HEADER) );
                    Status = EFI_SUCCESS;
#endif
                }
            }
        }
    }
    DEBUG ((DEBUG_INFO, "PreserveProductKey: Status %r\n",Status));
    return Status;
}


/**
    This function is the entry point of the eModule.

    @param 
        ImageHandle  - Image handle
        *SystemTable - Pointer to the system table

    @retval EFI_STATUS

**/

EFI_STATUS EFIAPI OA3_EntryPoint(
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable )
{
#if OEM_ACTIVATION_TABLE_LOCATION == 0 
    EFI_GUID FidTableGuid = FID_TABLE_GUID;
    EFI_GUID FidTableSectionGuid = FID_TABLE_SECTION_GUID;
    FW_VERSION *Fid;
    UINT8  *FidSection = NULL;
#elif OEM_ACTIVATION_TABLE_LOCATION == 2
	EFI_ACPI_MSDM_TABLE EmptyTable;
	EFI_PHYSICAL_ADDRESS OA3TableAddress;
#endif
    EFI_STATUS Status;
    EFI_ACPI_MSDM_TABLE *Msdm = NULL;
    EFI_OA3_MSDM_STRUCTURE MsdmVariable = {0};
    EFI_GUID AmiGlobalVariableGuid = AMI_GLOBAL_VARIABLE_GUID;
    UINTN  i;

    // Check if it's a Recovery Mode then preserve the Product Key
    if ((GetBootModeHob () == BOOT_IN_RECOVERY_MODE) || (GetBootModeHob () == BOOT_ON_FLASH_UPDATE)) {
#if OEM_ACTIVATION_TABLE_LOCATION == 2
		//since the table is saved in other device, don't need to preserve key.
		return EFI_SUCCESS;
#endif
        Status = PreserveProductKey();
        return Status;
    }
#if OEM_ACTIVATION_TABLE_LOCATION == 0    
    // Locate the $FID table
    Status = LocateSectionResource(
        &FidTableGuid, 
        &FidTableSectionGuid, 
        &FidSection
    );
    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "OEM Activation: FID table Not Found!!!\n"));
        return Status;
    }
    Fid = (FW_VERSION*)(FidSection + sizeof(EFI_GUID)); // skip the GUID

    // Check version of the FID table
    if ( Fid->StructVersion < FID_SUPPORTED_VERSION ) {
        DEBUG ((DEBUG_ERROR, "OEM Activation: FID table version %d is unsupported!!!\n",Fid->StructVersion));
        return EFI_UNSUPPORTED;
    }
#elif OEM_ACTIVATION_TABLE_LOCATION == 2
	//OA3TableAddress default will point to empty table.
	SetMem( &EmptyTable, sizeof(EmptyTable), 0xff );
	OA3TableAddress = (EFI_PHYSICAL_ADDRESS)&EmptyTable;
	
	for (i = 0; OA3TableAddressElink[i] != NULL; i++) {
		OA3TableAddressElink[i](&OA3TableAddress);
	}
#endif

    // Create MSDM table
    Status = gBS->AllocatePool( EfiACPIMemoryNVS, sizeof(EFI_ACPI_MSDM_TABLE), &Msdm );
    if (EFI_ERROR(Status))
        return Status;
    Msdm->Header.Signature  = MSDM_SIG;
    Msdm->Header.Length     = sizeof(EFI_ACPI_MSDM_TABLE);
    Msdm->Header.Revision   = MSDM_REVISION;
    Msdm->Header.OemRevision = ACPI_OEM_REV;
    Msdm->Header.CreatorId  = CREATOR_ID_AMI;
    Msdm->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

    // Fill OEM ID and OEM Table ID 
    CopyMem (Msdm->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Msdm->Header.OemId));
    Msdm->Header.OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

    // Copy OEM Activation Key
    CopyMem( 
        (UINT8*)&Msdm->Version,
#if OEM_ACTIVATION_TABLE_LOCATION == 0
        (UINT8*)&Fid->OemActivationKey,         // Copy Key from the FID structure
#elif OEM_ACTIVATION_TABLE_LOCATION == 1
        (UINT8*)OEM_ACTIVATION_TABLE_ADDRESS,   // Copy Key from the NCB
#elif OEM_ACTIVATION_TABLE_LOCATION == 2
		(UINT8*)OA3TableAddress,
#endif
        sizeof(EFI_ACPI_MSDM_TABLE) - sizeof(EFI_ACPI_DESCRIPTION_HEADER) 
    );
#if OEM_ACTIVATION_TABLE_LOCATION == 0
    gBS->FreePool(FidSection);
#endif

    // Check if there is empty Product Key 
    for ( i = 0; i < sizeof(Msdm->Data); i++ ) {

        if ( Msdm->Data[i] != 0xFF ) {

            EFI_ACPI_TABLE_PROTOCOL *AcpiTableProtocol = NULL;
            UINTN  TableKey = 0;

            // Locate the ACPI support protocol
            Status = gBS->LocateProtocol(
                &gEfiAcpiTableProtocolGuid, 
                NULL, 
                &AcpiTableProtocol
            );
            if (EFI_ERROR(Status))
                DEBUG ((DEBUG_ERROR, "OEM Activation: Unable to locate AcpiSupportProtocol!\n"));
                
            else {
                // Publish MSDM ACPI table
                Status = AcpiTableProtocol->InstallAcpiTable(
                    AcpiTableProtocol,
                    Msdm,
                    sizeof(EFI_ACPI_MSDM_TABLE),
                    &TableKey
                );
                if (EFI_ERROR(Status))
                    DEBUG ((DEBUG_ERROR, "OEM Activation: SetAcpiTable failed!\n"));
                else
                    DEBUG ((DEBUG_INFO, "OEM Activation: MSDM table has been published.\n"));
            }
            gBS->FreePool(Msdm);
            return Status;
        }
    }

    DEBUG ((DEBUG_INFO, "OEM Activation: Found empty Product Key.\n"));

    MsdmVariable.MsdmAddress = (EFI_PHYSICAL_ADDRESS)Msdm;
#if OEM_ACTIVATION_TABLE_LOCATION == 1
    MsdmVariable.ProductKeyAddress = (EFI_PHYSICAL_ADDRESS)OEM_ACTIVATION_TABLE_ADDRESS;
#endif

    Status = gRT->SetVariable(
        EFI_OA3_MSDM_VARIABLE,
        &AmiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof(EFI_OA3_MSDM_STRUCTURE),
        &MsdmVariable
    );
    ASSERT_EFI_ERROR(Status);
    DEBUG ((DEBUG_INFO, "OEM Activation: MsdmAddress=%X\n",MsdmVariable.MsdmAddress));

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
