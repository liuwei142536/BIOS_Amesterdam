//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
#include <Uefi.h>
#include <Guid/CapsuleVendor.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

EFI_STATUS EFIAPI ProcessCapsuleImage(
    IN EFI_CAPSULE_HEADER *CapsuleHeader
){
    return EFI_UNSUPPORTED;
}

/// Delete Capsule Variables
STATIC VOID AmiDxeCapsuleLibPerformCapsuleVariableCleanup(){
    EFI_STATUS Status;
    CHAR16 CapsuleNameBuffer[30];
    UINTN Index = 0;

    // Clear any NVRAM variable of the format L"CapsuleUpdateData" and L"CapsuleUpdateDataN" where N is an integer
    UnicodeSPrint(CapsuleNameBuffer, sizeof(CapsuleNameBuffer), L"%s", EFI_CAPSULE_VARIABLE_NAME);
    do{
        Status = gRT->SetVariable(CapsuleNameBuffer, &gEfiCapsuleVendorGuid, 0, 0, NULL);
        if (EFI_ERROR(Status)) break;
        Index++;
        UnicodeSPrint(CapsuleNameBuffer, sizeof(CapsuleNameBuffer), L"%s%d", EFI_CAPSULE_VARIABLE_NAME, Index);
    }while(TRUE);
}

/// Publish capsules to UEFI system table
STATIC EFI_STATUS AmiDxeCapsuleLibPublishCpasulesToUefiSystemTable(){

    EFI_HOB_UEFI_CAPSULE *CapsuleHob, *CapsuleHob2;
    EFI_CAPSULE_HEADER *Capsule, *Capsule2;
    EFI_CAPSULE_TABLE *CapsuleTable;
    UINT32 CapsuleCounter;
    EFI_STATUS Status, Status2;

    Status = EFI_SUCCESS;

    // Iterate over all capsule HOBs
    for(   CapsuleHob = GetFirstHob(EFI_HOB_TYPE_UEFI_CAPSULE)
         ; CapsuleHob != NULL
         ; CapsuleHob = GetNextHob(EFI_HOB_TYPE_UEFI_CAPSULE, GET_NEXT_HOB (CapsuleHob))
    ){
        VOID *Dummy;

        Capsule = (EFI_CAPSULE_HEADER*)(UINTN)CapsuleHob->BaseAddress;
        // If capsule should not be published into UEFI system table, continue.
        if ( (Capsule->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) == 0 ) continue;
        // If configuration table with capsule GUID exists,
        // capsules with this GUID have already been processed by this function.
        if (!EFI_ERROR(EfiGetSystemConfigurationTable(&Capsule->CapsuleGuid, &Dummy))) continue;

        // Check if there are more capsules with the same GUID.
        CapsuleCounter = 0;
        for(  CapsuleHob2 = GetNextHob(EFI_HOB_TYPE_UEFI_CAPSULE, GET_NEXT_HOB (CapsuleHob))
            ; CapsuleHob2 != NULL
            ; CapsuleHob2 = GetNextHob(EFI_HOB_TYPE_UEFI_CAPSULE, GET_NEXT_HOB (CapsuleHob2))
        ){
            Capsule2 = (EFI_CAPSULE_HEADER*)(UINTN)CapsuleHob2->BaseAddress;
            if ( (Capsule2->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) == 0 ) continue;
            if (!CompareGuid(&Capsule->CapsuleGuid, &Capsule2->CapsuleGuid)) continue;
            CapsuleCounter++;
        }

        // There are CapsuleCounter+1 capsules with the same GUID.

        // EFI_CAPSULE_TABLE includes one pointer, adding space for additional CapsuleCounter pointers.
        CapsuleTable = AllocateRuntimePool(sizeof(EFI_CAPSULE_TABLE)+CapsuleCounter*sizeof(VOID*));
        ASSERT(CapsuleTable!=NULL);
        if (CapsuleTable==NULL) return EFI_OUT_OF_RESOURCES;
        CapsuleTable->CapsuleArrayNumber = CapsuleCounter + 1;
        CapsuleTable->CapsulePtr[0] = Capsule;

        CapsuleCounter = 1; // CapsuleTable->CapsulePtr[0] is already populated; start with index 1.
        for(  CapsuleHob2 = GetNextHob(EFI_HOB_TYPE_UEFI_CAPSULE, GET_NEXT_HOB (CapsuleHob))
            ; CapsuleHob2 != NULL && CapsuleCounter < CapsuleTable->CapsuleArrayNumber
            ; CapsuleHob2 = GetNextHob(EFI_HOB_TYPE_UEFI_CAPSULE, GET_NEXT_HOB (CapsuleHob2))
        ){
            Capsule2 = (EFI_CAPSULE_HEADER*)(UINTN)CapsuleHob2->BaseAddress;
            if ( (Capsule2->Flags & CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE) == 0 ) continue;
            if (!CompareGuid(&Capsule->CapsuleGuid, &Capsule2->CapsuleGuid)) continue;
            CapsuleTable->CapsulePtr[CapsuleCounter++] = Capsule2;
        }
        ASSERT(CapsuleCounter==CapsuleTable->CapsuleArrayNumber);
        Status2 = gBS->InstallConfigurationTable(&Capsule->CapsuleGuid, CapsuleTable);
        ASSERT_EFI_ERROR(Status2);
        if (EFI_ERROR(Status2)) Status = Status2;
    }
    return Status;
}

EFI_STATUS EFIAPI ProcessCapsules(VOID){
    STATIC BOOLEAN AlreadyProcessed = FALSE;
    if (AlreadyProcessed) return EFI_SUCCESS;
    AlreadyProcessed = TRUE;
    AmiDxeCapsuleLibPerformCapsuleVariableCleanup();
    return AmiDxeCapsuleLibPublishCpasulesToUefiSystemTable();
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
