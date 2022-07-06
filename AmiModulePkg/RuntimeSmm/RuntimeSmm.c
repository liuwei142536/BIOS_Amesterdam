//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-20119, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************


/** 
    @file RuntimeSmm.c
        Installs SMM copy of the Runtime Services Table
**/


//=============================================================================
// Includes
#include <Protocol/Runtime.h>
#include <Protocol/LoadedImage.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <AmiDxeLib.h>
#include <Library/BaseMemoryLib.h>


extern EFI_GUID SmmRsTableGuid;


/**
    This function is a Dummy function that is used as a default function
    for the SMM instance of the Runtime Services Table


    @return EFI_STATUS
    @retval EFI_UNSUPPORTED all the time
**/
EFI_STATUS DummySmmRtFunction(){ return EFI_UNSUPPORTED; }

/**
    This function is the entry point for the Runtime Driver.  It initializes
    the AMI Library and then it calls the Initialization functions for both
    the SMM and NON-SMM versions of the Runtime Driver.  It then calls the 
    InitParts2 function which calls the functions in the RuntimeSmmInitialize
    eLink.

    @param ImageHandle - The firmware allocated handle for this driver
    @param SystemTable - Pointer to the UEFI SystemTable
  
    @return EFI_STATUS 
    @retval Based on the return values of the InitSmmHandlerEx function
**/
EFI_STATUS EFIAPI RuntimeEntry(
    EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_RUNTIME_SERVICES 	*SmmRs;
    EFI_STATUS      Status;
    EFI_HANDLE      Handle = NULL;
    VOID **p;

    Status = gSmst->SmmAllocatePool(EfiRuntimeServicesData, sizeof(EFI_RUNTIME_SERVICES), (VOID**)&SmmRs);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;
        
    //copy header
    CopyMem ((VOID*)&(SmmRs->Hdr), (VOID*)&(gRT->Hdr), sizeof(EFI_TABLE_HEADER));

    //Init Runtime Services function pointers with our dummy function
    for(p = (VOID**)((UINT8*)SmmRs + sizeof(SmmRs->Hdr)); p < (VOID**)(SmmRs + 1); p++)
        *p = (VOID*)(UINTN)DummySmmRtFunction;

    // install the SMM version of the Runtime Services Table SMM Configuration Table
    Status = gSmst->SmmInstallConfigurationTable(
        gSmst, &SmmRsTableGuid, SmmRs, sizeof(EFI_RUNTIME_SERVICES) );
    ASSERT_EFI_ERROR(Status);
    
    Status = gBS->InstallMultipleProtocolInterfaces (
        &Handle,
        &SmmRsTableGuid,
        SmmRs,
        NULL
    );

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-20119, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
