//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include "NvramDxeCommon.h"
#include <Protocol/SmmVariable.h>
#include <Protocol/SmmReadyToLock.h>
#include <Guid/HobList.h>
#include "NvramSmi.h"

EFI_STATUS EFIAPI SmmSetVariableNoLock(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
);
EFI_STATUS EFIAPI SmmSetVariable(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
);
VOID IndexReset();

EFI_SMM_VARIABLE_PROTOCOL SmmVariable = {DxeGetVariableWrapper, DxeGetNextVariableNameWrapper, SmmSetVariableNoLock, QueryVariableInfo};
EFI_SMM_VARIABLE_PROTOCOL SmmVariableWithLockSupport = {DxeGetVariableWrapper, DxeGetNextVariableNameWrapper, SmmSetVariable, QueryVariableInfo};
BOOLEAN LockEnforcementIsActive = TRUE;

BOOLEAN     gMorLock = FALSE;
BOOLEAN     MorLockCheckIsActive = FALSE;
UINT64      MorKeyArray[2];
UINT64      *gMorKey = NULL;


static EFI_GUID gMemoryOverwriteRequestControlLockGuid = { 0xbb983ccf, 0x151d, 0x40e1, { 0xa0, 0x7b, 0x4a, 0x17, 0xbe, 0x16, 0x82, 0x92 } };
static EFI_GUID gMemoryOverwriteRequestControlGuid = {0xe20939be, 0x32d4, 0x41be, { 0xa1, 0x50, 0x89, 0x7f, 0x85, 0xd4, 0x98,0x29 } };

struct _NVRAMDXE_INFO{
    UPDATE_INFO *UpdateInfo;

    UINT8 *NvAddress;
    UINTN NvSize;
    UINT32 NvHeader;

    UINT8 *MemAddress;
    UINTN MemSize;
} NvramDxeInfo;
BOOLEAN NvramDxeUpdateShutDown = FALSE;

#if AuthVariable_SUPPORT
VOID AuthVariableServiceInitSMM (VOID);
#else
VOID AuthVariableServiceInitSMM (VOID) {};
#endif //#if AuthVariable_SUPPORT
extern EFI_GCD_MEMORY_SPACE_DESCRIPTOR  gDescriptor; 
EFI_STATUS
EFIAPI
NvramSmiEntry(
    IN EFI_HANDLE                ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
);

VOID* NvramAllocatePages(UINTN Size){
    EFI_PHYSICAL_ADDRESS Address;
    EFI_STATUS Status;

    Status = pSmst->SmmAllocatePages(AllocateAnyPages, EfiRuntimeServicesData, EFI_SIZE_TO_PAGES(Size), &Address);
    if (EFI_ERROR(Status)) return NULL;
    return (VOID*)(UINTN)Address;
}

VOID NvramFreePages(VOID *Address, UINTN Size){
    EFI_STATUS Status;

    Status =  pSmst->SmmFreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Address, EFI_SIZE_TO_PAGES(Size));
    ASSERT_EFI_ERROR (Status);
}

AMI_FLASH_PROTOCOL *LocateFlashProtocol(){
    typedef EFI_STATUS (EFIAPI *AMI_NVRAM_FLASH_INTERFACE_GET)(OUT AMI_FLASH_PROTOCOL **FlashInterface);

    typedef struct _AMI_NVRAM_FLASH_INTERFACE_PROTOCOL{
        AMI_NVRAM_FLASH_INTERFACE_GET GetNvramFlashInterface;
    } AMI_NVRAM_FLASH_INTERFACE_PROTOCOL;

    static EFI_GUID gAmiNvramFlashInterfaceProtocolGuid =
        { 0xfed7c6, 0xad32, 0x4793, { 0x81, 0x94, 0xd2, 0xd2, 0x41, 0xa3, 0x3f, 0x10 } };

    AMI_NVRAM_FLASH_INTERFACE_PROTOCOL *FlashInterface;
    EFI_STATUS Status;
    AMI_FLASH_PROTOCOL *Flash;

    Status = pSmst->SmmLocateProtocol(&gAmiNvramFlashInterfaceProtocolGuid, NULL, (VOID**)&FlashInterface);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return NULL;
    Status = FlashInterface->GetNvramFlashInterface(&Flash);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return NULL;
    return Flash;
}

/**
    This function sets Var with specific GUID, Name and attributes in SMM
    synchronizing Varstors before and after operation

    @param VariableName pointer to Var Name in Unicode
    @param VendorGuid pointer to Var GUID
    @param Attributes Attributes of the Var
    @param DataSize size of Var
    @param Data Pointer to memory where Var data is stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS SmmSetVariableInt(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
)
{
    EFI_STATUS Status;

    Status = DxeSetVariableWrapper(VariableName, VendorGuid, Attributes, DataSize, Data);
    if ( EFI_ERROR(Status) || NvramDxeUpdateShutDown) return Status;
    if (NvramDxeInfo.UpdateInfo->ShutDown){
        NvramDxeUpdateShutDown = TRUE;
        return Status;
    }
    if (   NvramDxeInfo.NvSize != VarStoreInfo.NvInfo->NvramSize
        || NvramDxeInfo.MemSize != VarStoreInfo.MemInfo->NvramSize
        || NvramDxeInfo.NvHeader != VarStoreInfo.NvInfo->HeaderLength
    ){
        // NVRAM configuration has changed.
        // Shutdown synchronization to prevent potential NVRAM image corruption.
        NvramDxeInfo.UpdateInfo->ShutDown = TRUE;
        NvramDxeUpdateShutDown = TRUE;
    }else {
        BOOLEAN UpdateNvStore = FALSE;
        BOOLEAN UpdateMemStore = FALSE;

        if ((!(Attributes & EFI_VARIABLE_APPEND_WRITE) && !DataSize) ||
             !(Attributes & EFI_VARIABLE_BOOTSERVICE_ACCESS)
        ){
            // When variable is deleted we don't know if it is a volatile or non-volatile variable.
            // Synchronize both variable storages.
            UpdateNvStore = TRUE;
            UpdateMemStore = TRUE;
        }else if ( (Attributes & EFI_VARIABLE_NON_VOLATILE) != 0 ){
            UpdateNvStore = TRUE;
        }else{
            UpdateMemStore = TRUE;
        }

        if (UpdateNvStore){
            NvramDxeInfo.UpdateInfo->StoreMask |= UPDATE_INFO_ENUMERATE_NV_STORE;
            MemCpy(NvramDxeInfo.NvAddress, VarStoreInfo.NvInfo->NvramAddress, NvramDxeInfo.NvSize);
        }
        if (UpdateMemStore){
            NvramDxeInfo.UpdateInfo->StoreMask |= UPDATE_INFO_ENUMERATE_MEM_STORE;
            MemCpy(NvramDxeInfo.MemAddress, VarStoreInfo.MemInfo->NvramAddress, NvramDxeInfo.MemSize);
        }
    }

    return Status;
}

VOID ResetMorVariables(){
    UINT8 Data = 0;
    EFI_STATUS Status;
    UINTN DataSize = 0;

    // Firmware shall initialize the variable with 0 prior to processing the Boot#### on every boot.
    // (BIOS Enabling Guide for Windows 10, Section#9 - Virtual Security Mode)
    Status = SmmSetVariableInt(
        L"MemoryOverwriteRequestControlLock",&gMemoryOverwriteRequestControlLockGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
        sizeof(Data), &Data
    );
    ASSERT_EFI_ERROR(Status);
    // If MemoryOverwriteRequestControl variable does not exist, create it.
    // If the variable exists, we are not reseting it back to zero.
    // Code that cleans the memory should reset variable back to zero.
    Status = DxeGetVariableWrapper(
        L"MemoryOverwriteRequestControl",&gMemoryOverwriteRequestControlGuid, NULL, &DataSize, NULL
    );
    if (Status == EFI_NOT_FOUND){
        Status = SmmSetVariableInt(
            L"MemoryOverwriteRequestControl",&gMemoryOverwriteRequestControlGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
            sizeof(Data), &Data
        );
        ASSERT_EFI_ERROR(Status);
    }
    MorLockCheckIsActive = TRUE;
}

/**
  This function locks the update of MOR variables once
  MemoryOverwriteRequestControlLock variable is locked-down by setting it to one.
  This function is called by SmmSetVariable.

 @param VariableName pointer to Var Name in Unicode
 @param VendorGuid pointer to Var GUID
 @param Attributes Attributes of the Var
 @param DataSize size of Var
 @param Data Pointer to memory where Var data is stored

 @retval EFI_UNSUPPORTED - Passed Variable can be set.
 @retval Any Status other than EFI_UNSUPPORTED - the error is propagated as a result of SmmSetVariable.

**/
EFI_STATUS CheckForMorVariables(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
){

    EFI_STATUS Status;
    UINT8 VarData;

    if (!MorLockCheckIsActive) return EFI_UNSUPPORTED;

    if (  !StrCmp (VariableName, L"MemoryOverwriteRequestControlLock")
        && !guidcmp (VendorGuid, &gMemoryOverwriteRequestControlLockGuid)
    ){
        if ( DataSize != 0 && Data == NULL )
            return EFI_INVALID_PARAMETER;
        if ( Attributes == 0 || DataSize == 0 || Data == NULL )
            return EFI_WRITE_PROTECTED;
        if ( Attributes != (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)
                || (DataSize != 1 && DataSize != 8))
            return EFI_INVALID_PARAMETER;

        if (gMorLock == FALSE){
            // MOR Variable Unlocked
            if (DataSize == 1){
                if (*(UINT8*)Data == 0)
                    return EFI_UNSUPPORTED;
                if (*(UINT8*)Data == 1){
                    gMorLock = TRUE;
                    return EFI_UNSUPPORTED;
                }
                return EFI_INVALID_PARAMETER;
            } //if (DataSize == 1)

            if (DataSize == 8){
                gMorLock = TRUE;
                *gMorKey = ReadUnaligned64((UINT64*)Data);
                VarData = 2;
                Status = SmmSetVariableInt(VariableName, VendorGuid, Attributes, 1, &VarData);
                return Status;
            }
        }else{//if (gMorLock == FALSE)
            // MOR Variable Locked
            if (DataSize !=8 || *gMorKey == 0)
                return EFI_ACCESS_DENIED;
            if (*gMorKey != ReadUnaligned64((UINT64*)Data)){
                *gMorKey = 0;
                return EFI_ACCESS_DENIED;
            }
            gMorLock = FALSE;
            *gMorKey = 0;
            VarData = 0;
            Status = SmmSetVariableInt(VariableName, VendorGuid, Attributes, 1, &VarData);
            return Status;
        }

    }
    if(  !StrCmp (VariableName, L"MemoryOverwriteRequestControl")
            && !guidcmp (VendorGuid, &gMemoryOverwriteRequestControlGuid)
    ){
        if ( Attributes != (EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE)
                || (DataSize != 1))
            return EFI_INVALID_PARAMETER;
        if ( Data != NULL && *(UINT8*)Data > 1 && *(UINT8*)Data != 0x11)
            return EFI_INVALID_PARAMETER;
        if (gMorLock == FALSE)
            return EFI_UNSUPPORTED;
        else
            return EFI_ACCESS_DENIED;
    }

    return EFI_UNSUPPORTED;
}

EFI_STATUS EFIAPI SmmSetVariable(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
)
{
    EFI_STATUS Status;

    // Special handling of the Memory Override Request Variables
    Status = CheckForMorVariables (
        VariableName, VendorGuid,
        Attributes, DataSize, Data
    );
    if (Status != EFI_UNSUPPORTED){
        return Status;
    }

    return SmmSetVariableInt(VariableName, VendorGuid, Attributes, DataSize, Data);
}

/**
    This function sets Var with specific GUID, Name and attributes in SMM
    synchronizing Varstors before and after operation

    @param VariableName pointer to Var Name in Unicode
    @param VendorGuid pointer to Var GUID
    @param Attributes Attributes of the Var
    @param DataSize size of Var
    @param Data Pointer to memory where Var data is stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI SmmSetVariableNoLock(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
)
{
    EFI_STATUS Status;

    LockEnforcementIsActive = FALSE;
    Status = SmmSetVariable(VariableName, VendorGuid, Attributes, DataSize, Data);
    LockEnforcementIsActive = TRUE;

    return Status;
}

EFI_STATUS EFIAPI SmmReadyToLockCallback(
  IN CONST EFI_GUID *Protocol, IN VOID *Interface, IN EFI_HANDLE Handle
){
    SwitchToBds();
    ResetMorVariables();
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI NvramSmmInit(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS  Status;
    EFI_HANDLE  Handle = NULL;
    VOID *Registration;
    VOID *Address;
    NVRAM_MAILBOX Mailbox;
    UINTN Size=0;

    InitAmiSmmLib(ImageHandle,SystemTable);
    gDescriptor.Length = 1;
    Status = NvramInitialize();
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    AuthVariableServiceInitSMM();

    if (((UINTN)&MorKeyArray[0] & 0x3F) == 0) //Check 64 byte aligment to use unaligned address
        gMorKey = &MorKeyArray[1]; 
    else 
        gMorKey = &MorKeyArray[0];
    *gMorKey = 0;

    Size = sizeof(Mailbox);

    Status = pST->RuntimeServices->GetVariable(
        L"NvramMailBox",&gAmiNvramSmmCommunicationGuid,NULL,&Size, &Mailbox
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;

    NvramDxeInfo.UpdateInfo = Mailbox.UpdateInfo;
    NvramDxeInfo.NvAddress = Mailbox.VarStoreInfoPtr->NvInfo->NvramAddress;
    NvramDxeInfo.NvSize = Mailbox.VarStoreInfoPtr->NvInfo->NvramSize;
    NvramDxeInfo.MemAddress = Mailbox.VarStoreInfoPtr->MemInfo->NvramAddress;
    NvramDxeInfo.MemSize = Mailbox.VarStoreInfoPtr->MemInfo->NvramSize;
    NvramDxeInfo.NvHeader = Mailbox.VarStoreInfoPtr->NvInfo->HeaderLength;

    // Delete the NvramMailBox variable
    Status = pST->RuntimeServices->SetVariable(
        L"NvramMailBox",&gAmiNvramSmmCommunicationGuid,0,0, NULL
    );
    ASSERT_EFI_ERROR(Status);

    ASSERT(VarStoreInfo.MemInfo->NvramSize == Mailbox.VarStoreInfoPtr->MemInfo->NvramSize);
    if (VarStoreInfo.MemInfo->NvramSize != Mailbox.VarStoreInfoPtr->MemInfo->NvramSize) return EFI_INVALID_PARAMETER;
    MemCpy(VarStoreInfo.MemInfo->NvramAddress,Mailbox.VarStoreInfoPtr->MemInfo->NvramAddress,VarStoreInfo.MemInfo->NvramSize);
    ResetVarStore(VarStoreInfo.MemInfo);
    if ((VarStoreInfo.NvramMode & NVRAM_MODE_SIMULATION)!=0){
        // if we're in simulation mode, copy content from DXE in-memory NVARM image
        ASSERT(VarStoreInfo.NvInfo->NvramSize == Mailbox.VarStoreInfoPtr->NvInfo->NvramSize);
        if (VarStoreInfo.NvInfo->NvramSize != Mailbox.VarStoreInfoPtr->NvInfo->NvramSize) return EFI_INVALID_PARAMETER;
        MemCpy(VarStoreInfo.NvInfo->NvramAddress,Mailbox.VarStoreInfoPtr->NvInfo->NvramAddress,VarStoreInfo.NvInfo->NvramSize);
        ResetVarStore(VarStoreInfo.NvInfo);
        UpdateNestedVarstores();
    }
    IndexReset();
    Status = pSmst->SmmAllocatePool ( EfiRuntimeServicesData, NVRAM_SIZE / 2, &Address);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    InitNvramLockBuffer(Address,NVRAM_SIZE/2);

    Status = NvramSmiEntry(ImageHandle, SystemTable);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;

    Status = Mailbox.SwitchToSmmWrapper();
    if (EFI_ERROR(Status)) return Status;

    pSmst->SmmRegisterProtocolNotify(&gEfiSmmReadyToLockProtocolGuid,SmmReadyToLockCallback,&Registration);
    //Make sure pRS is replaced with the in-SMM version by the library (AmiDxeLib)
    ASSERT(pRS!=pST->RuntimeServices);
    pRS->GetNextVariableName = DxeGetNextVariableNameWrapper;
    pRS->GetVariable = DxeGetVariableWrapper;
    pRS->SetVariable = SmmSetVariable;
    pRS->QueryVariableInfo = QueryVariableInfo;

    Status = pSmst->SmmInstallProtocolInterface(
        &Handle, &gEfiSmmVariableProtocolGuid, EFI_NATIVE_INTERFACE, &SmmVariable
    );
    ASSERT_EFI_ERROR (Status);
    Status = pSmst->SmmInstallProtocolInterface(
        &Handle, &gAmiNvramSmmCommunicationGuid, EFI_NATIVE_INTERFACE, &SmmVariableWithLockSupport
    );
    ASSERT_EFI_ERROR (Status);
    Status = pSmst->SmmInstallProtocolInterface(
        &Handle, &gAmiSmmNvramUpdateProtocolGuid, EFI_NATIVE_INTERFACE, &AmiNvramUpdate
    );
    ASSERT_EFI_ERROR (Status);

    return Status;
}

BOOLEAN IsLockEnforcementActive (){
    return IsSmmLocked() && LockEnforcementIsActive;
}

//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
