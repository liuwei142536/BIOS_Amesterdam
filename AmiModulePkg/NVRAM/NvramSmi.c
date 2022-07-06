//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file
    NvramSmi helps secure a system by providing a set of variable service functions
    which route variable access through the SMM interface.
**/
#include <AmiDxeLib.h>
#include <Library/AmiBufferValidationLib.h>
#include "NvramSmi.h"
#include "NvramDxeCommon.h"

EFI_HANDLE  VarSmiHandle = NULL;

EFI_STATUS SmmSetVariable(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
);

EFI_STATUS GetVariableSmmHandler(
    SMI_GET_VARIABLE_BUFFER *GetVarBuffer, UINTN BufferSize
){
    CHAR16 *VariableName;

    // Validate input buffer before accessing it.
    if (BufferSize <= sizeof(SMI_GET_VARIABLE_BUFFER)) return EFI_INVALID_PARAMETER;
    BufferSize -= sizeof(SMI_GET_VARIABLE_BUFFER);
    // Variable name must contains at least one character (zero terminator). String size must be an even number
    if (   GetVarBuffer->VariableNameSize<sizeof(CHAR16)
        || (GetVarBuffer->VariableNameSize & 1) != 0
    ) return EFI_INVALID_PARAMETER;
    if (BufferSize < GetVarBuffer->VariableNameSize) return EFI_INVALID_PARAMETER;
    BufferSize -= (UINTN)GetVarBuffer->VariableNameSize;
    VariableName = (CHAR16*)(GetVarBuffer+1);
    // Make sure variable name is zero terminated.
    if (VariableName[(UINTN)GetVarBuffer->VariableNameSize/sizeof(CHAR16)-1]) return EFI_INVALID_PARAMETER;
    if (BufferSize != GetVarBuffer->DataSize) return EFI_INVALID_PARAMETER;

    return DxeGetVariableWrapper (
        VariableName, &GetVarBuffer->Guid,
        &GetVarBuffer->Attributes, (UINTN*)&GetVarBuffer->DataSize,
        (BufferSize==0) ? NULL : (VOID*)((UINT8*)(GetVarBuffer+1) + GetVarBuffer->VariableNameSize)
    );
}

EFI_STATUS GetNextVariableNameSmmHandler(
    SMI_GET_NEXT_VARIABLE_NAME_BUFFER *GetNextVarNameBuffer, UINTN BufferSize
){
    CHAR16 *VariableName;

    //Validate input buffer before accessing it.
    if (BufferSize <= sizeof(SMI_GET_NEXT_VARIABLE_NAME_BUFFER)) return EFI_INVALID_PARAMETER;
    BufferSize -= sizeof(SMI_GET_NEXT_VARIABLE_NAME_BUFFER);
    if (BufferSize != GetNextVarNameBuffer->DataSize) return EFI_INVALID_PARAMETER;
    // Input variable name must contains at least one character (zero terminator).
    if (GetNextVarNameBuffer->DataSize<sizeof(CHAR16)) return EFI_INVALID_PARAMETER;
    //Validating VariableName
    VariableName = (CHAR16*)(GetNextVarNameBuffer+1);
    if (GetVariableNameSize(VariableName, (UINTN)GetNextVarNameBuffer->DataSize) > GetNextVarNameBuffer->DataSize) return EFI_INVALID_PARAMETER;

    return DxeGetNextVariableNameWrapper(
        (UINTN*)&GetNextVarNameBuffer->DataSize,
        VariableName, &GetNextVarNameBuffer->Guid
    );
}

EFI_STATUS SetVariableSmmHandler(
    SMI_SET_VARIABLE_BUFFER *SetVarBuffer, UINTN BufferSize
){
    CHAR16 *VariableName;

    //Validate input buffer before accessing it.
    if (BufferSize <= sizeof(SMI_SET_VARIABLE_BUFFER)) return EFI_INVALID_PARAMETER;
    BufferSize -= sizeof(SMI_SET_VARIABLE_BUFFER);
    // Variable name must contains at least one character (zero terminator). String size must be an even number
    if (   SetVarBuffer->VariableNameSize<sizeof(CHAR16)
        || (SetVarBuffer->VariableNameSize & 1) != 0
    ) return EFI_INVALID_PARAMETER;
    if (BufferSize < SetVarBuffer->VariableNameSize) return EFI_INVALID_PARAMETER;
    BufferSize -= (UINTN)SetVarBuffer->VariableNameSize;
    VariableName = (CHAR16*)(SetVarBuffer+1);
    // Make sure variable name is zero terminated.
    if (VariableName[(UINTN)SetVarBuffer->VariableNameSize/sizeof(CHAR16)-1]) return EFI_INVALID_PARAMETER;
    if (BufferSize != SetVarBuffer->DataSize) return EFI_INVALID_PARAMETER;

    return SmmSetVariable (
        VariableName, &SetVarBuffer->Guid,
        SetVarBuffer->Attributes, (UINTN)SetVarBuffer->DataSize,
        (BufferSize==0) ? NULL : (VOID*)((UINT8*)(SetVarBuffer+1) + SetVarBuffer->VariableNameSize)
    );
}

EFI_STATUS QueryVariableInfoSmmHandler(
    SMI_QUERY_VARIABLE_INFO_BUFFER *QueryVarInfoBuffer, UINTN BufferSize
){
    //Validate input buffer before accessing it.
    if (BufferSize != sizeof(SMI_QUERY_VARIABLE_INFO_BUFFER)) return EFI_INVALID_PARAMETER;

    return QueryVariableInfo (
        QueryVarInfoBuffer->Attributes,
        &QueryVarInfoBuffer->MaximumVariableStorageSize,
        &QueryVarInfoBuffer->RemainingVariableStorageSize,
        &QueryVarInfoBuffer->MaximumVariableSize
    );
}

EFI_STATUS RequestToLockSmmHandler(
    SMI_REQUEST_TO_LOCK_BUFFER *RequestToLockBuffer, UINTN BufferSize
){
    CHAR16 *VariableName;

    //Validate input buffer before accessing it.
    if (BufferSize <= sizeof(SMI_REQUEST_TO_LOCK_BUFFER)) return EFI_INVALID_PARAMETER;
    BufferSize -= sizeof(SMI_REQUEST_TO_LOCK_BUFFER);
    if (BufferSize != RequestToLockBuffer->DataSize) return EFI_INVALID_PARAMETER;
    // Variable name must contains at least one character (zero terminator). String size must be an even number
    if (   RequestToLockBuffer->DataSize<sizeof(CHAR16)
        || (RequestToLockBuffer->DataSize & 1) != 0
    ) return EFI_INVALID_PARAMETER;
    VariableName = (CHAR16*)(RequestToLockBuffer+1);
    // Make sure variable name is zero terminated.
    if (VariableName[(UINTN)RequestToLockBuffer->DataSize/sizeof(CHAR16)-1]) return EFI_INVALID_PARAMETER;

    return DxeRequestToLock(
        NULL,VariableName,&RequestToLockBuffer->Guid
    );
}

EFI_STATUS ExitBootServicesSmmHandler(
    SMI_EXIT_BOOT_SERVICES_BUFFER *ExitBootServicesBuffer, UINTN BufferSize
){
    //Validate input buffer before accessing it.
    if (BufferSize != sizeof(SMI_EXIT_BOOT_SERVICES_BUFFER)) return EFI_INVALID_PARAMETER;
    SwitchToRuntime();
    return EFI_SUCCESS;
}

/**
     SMI handler for NVRAM SMM Communication API.

    @param DispatchHandle - Dispatch Handle
    @param Context - Pointer to the passed context
    @param CommBuffer - Pointer to the passed Communication Buffer
    @param CommBufferSize - Pointer to the Comm Buffer Size


    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI NvramSmmCommunicationHandler (
  IN EFI_HANDLE DispatchHandle, IN CONST VOID *Context,
  IN OUT VOID *CommBuffer, IN OUT UINTN *CommBufferSize
){
    EFI_STATUS  Status;
    UINT64 *Command;
    VOID *Tmp;
    UINTN BufferSize;

    // Validate the buffer
    BufferSize = *CommBufferSize;
    Status = AmiValidateMemoryBuffer(CommBuffer, BufferSize);
    // A list of SMI handler return codes in defined by PI specification.
    // We can't return arbitrary error here.
    if (EFI_ERROR(Status)) return EFI_SUCCESS;
    if (BufferSize < sizeof(*Command))  return EFI_SUCCESS;

    // The CommBuffer is a pointer passed to SMM Communicate incremented to strip the header (GUID and MessageSize).
    // Since message size is UINTN, the pointer will not be naturally aligned in 32-bit mode.
    // We're aligning pointer to guarantee natural alignment.
    // See NVRAM_COMMUNICATION_BUFFER definition in NvramSmiDxe.c (The structure is naturally aligned).
    Tmp = ALIGN_POINTER(CommBuffer,sizeof(*Command));
    BufferSize -= (UINTN)Tmp-(UINTN)CommBuffer;
    CommBuffer = Tmp;
    Command = (UINT64*)CommBuffer;

    // Perform basic buffer size validation.
    // A more advanced validation is performed by the command handling functions.
    if (BufferSize < sizeof(*Command))  return EFI_SUCCESS;
    switch (*Command){
        case NVRAM_SMM_COMMAND_GET_VARIABLE: //GetVariable
            Status = GetVariableSmmHandler((SMI_GET_VARIABLE_BUFFER*)CommBuffer, BufferSize);
            break;
        case NVRAM_SMM_COMMAND_GET_NEXT_VARIABLE_NAME: //GetNextVariableName
            Status = GetNextVariableNameSmmHandler((SMI_GET_NEXT_VARIABLE_NAME_BUFFER*)CommBuffer, BufferSize);
            break;
        case NVRAM_SMM_COMMAND_SET_VARIABLE: //SetVariable
            Status = SetVariableSmmHandler((SMI_SET_VARIABLE_BUFFER*)CommBuffer, BufferSize);
            break;
        case NVRAM_SMM_COMMAND_QUERY_VARIABLE_INFO: //QueryVariableInfo
            Status = QueryVariableInfoSmmHandler((SMI_QUERY_VARIABLE_INFO_BUFFER*)CommBuffer, BufferSize);
            break;
        case NVRAM_SMM_COMMAND_REQUEST_TO_LOCK:
            Status = RequestToLockSmmHandler((SMI_REQUEST_TO_LOCK_BUFFER*)CommBuffer, BufferSize);
            break;
        case NVRAM_SMM_COMMAND_EXIT_BOOT_SERVICES:
            Status = ExitBootServicesSmmHandler((SMI_EXIT_BOOT_SERVICES_BUFFER*)CommBuffer, BufferSize);
            break;
        default:
            Status = EFI_INVALID_PARAMETER;
            break;
    }
    *Command = EFI_STATUS_TO_NVRAM_SMM_STATUS(Status);

    return EFI_SUCCESS;
}

/**
    Entry point.

    @param  ImageHandle - Image Handle
    @param  SystemTable - Pointer to a System Table

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI NvramSmiEntry(
    IN EFI_HANDLE                ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
){
    EFI_STATUS Status;

    InitAmiSmmLib (ImageHandle, SystemTable);
    ASSERT (pSmst != NULL);
    Status = pSmst->SmiHandlerRegister(NvramSmmCommunicationHandler, &gAmiNvramSmmCommunicationGuid, &VarSmiHandle);
    ASSERT_EFI_ERROR (Status);

    return Status;
}

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
