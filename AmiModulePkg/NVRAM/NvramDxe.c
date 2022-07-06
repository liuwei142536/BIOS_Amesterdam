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
#include "NvramDxeCommon.h"
#include <Protocol/Variable.h>
#include <Protocol/VariableWrite.h>
#include <Protocol/MonotonicCounter.h>
#include <Protocol/VariableLock.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/LegacyBiosExt.h>
#include <Library/DebugLib.h>
#include <Guid/AmiGlobalVariable.h>
#include <Guid/HobList.h>
#include <Setup.h>

BOOLEAN NvramSmmIsActive = FALSE;
BOOLEAN GetVariableSmmActive = FALSE;
UPDATE_INFO UpdateInfo;
BOOLEAN NoAuthWriteAccess = FALSE;

EFI_MEMORY_TYPE NvramMemoryType;

extern const BOOLEAN NvramMonotonicCounterSupport;
EFI_STATUS NvramInitialize();

//##### Security ######
#if AuthVariable_SUPPORT
VOID AuthVariableServiceInit (VOID);
VOID AuthServiceVirtualFixup(VOID);
#else
VOID AuthVariableServiceInit (VOID) {};
VOID AuthServiceVirtualFixup(VOID) {};
#endif //#if AuthVariable_SUPPORT

EFI_STATUS InitTmpVolatileStore(
    NVRAM_STORE_INFO *Info, UINTN Size,
    UINT32 HeaderLength, UINT8 Flags
);
EFI_STATUS NvResetConfiguration(
    NVRAM_STORE_INFO* Info,
    BOOLEAN ExternalDefaults, UINT32 ResetPropertyMask
);
EFI_STATUS InitHybridInterface(
        HYBRID_NVRAM_STORE_INTERFACE *This,
        UINT8 *Buffer, UINTN Size, UINT8* FlashAddress, UINT8* BackupAddress,
        AMI_FLASH_PROTOCOL *Flash, UINT32 *HeaderLength
);
VOID VarStoreDiscovery(NVRAM_STORE_INFO *NvInfo);

//NvramSmiDxe API
EFI_STATUS DxeGetVariableSmmWrapper(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    OUT UINT32 *Attributes OPTIONAL,
    IN OUT UINTN *DataSize, OUT VOID *Data
);

EFI_STATUS DxeGetNextVariableNameSmmWrapper(
    IN OUT UINTN *VariableNameSize,
    IN OUT CHAR16 *VariableName, IN OUT EFI_GUID *VendorGuid
);

EFI_STATUS DxeSetVariableSmmWrapper(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
);
EFI_STATUS QueryVariableInfoSmmWrapper (
    IN UINT32 Attributes,
    OUT UINT64 *MaximumVariableStorageSize,
    OUT UINT64 *RemainingVariableStorageSize,
    OUT UINT64 *MaximumVariableSize
);

EFI_STATUS RequestToLockSmmWrapper (
  IN CONST EDKII_VARIABLE_LOCK_PROTOCOL *This,
  IN       CHAR16                       *VariableName,
  IN       EFI_GUID                     *VendorGuid
  );

EFI_STATUS NvramSmiDxeEntry();
EFI_STATUS EFIAPI ActivateSmmWrapper();

VOID NvramSmmWrapperExitBootServicesHandler();
VOID NvramSmmWrapperVirtualAddressChangeHandler();

VOID IndexVirtualFixup();
VOID IndexReset();

//Hook to check NVRAM data compatibility with the firmware.
typedef BOOLEAN (*DXE_NVRAM_COMPATIBILITY_CHECKER_TYPE)(IN EFI_GET_VARIABLE GetVariable);
// DXE_NVRAM_COMPATIBILITY_CHECKER is a macro defined in Token.h
// Checker declaration
BOOLEAN DXE_NVRAM_COMPATIBILITY_CHECKER(IN EFI_GET_VARIABLE GetVariable);
//Checker variable
DXE_NVRAM_COMPATIBILITY_CHECKER_TYPE CompatibilityChecker = DXE_NVRAM_COMPATIBILITY_CHECKER;

CRITICAL_SECTION NvramCs = NULL;

/**
  This function determines if the current NVRAM data
  (mainly Setup-related variables) is compatible with the current firmware.

  @param GetVariable - pointer to EFI_GET_VARIABLE function. The pointer can be used to read existing NVRAM variables.
  @return
    BOOLEAN
      TRUE - the NVRAM data is compatible
      FALSE - the NVRAM data is not compatible
**/
BOOLEAN DxeNvramDataCompatiblityChecker(
    IN EFI_GET_VARIABLE GetVariable
){
    UINTN Size = 0;
    EFI_STATUS Status;
    static EFI_GUID SetupVariableGuid = SETUP_GUID;

    Status=GetVariable(
        L"Setup",
        &SetupVariableGuid, NULL,
        &Size, NULL
    );
    if (Status!=EFI_BUFFER_TOO_SMALL) return TRUE;
    return Size==sizeof(SETUP_DATA);
}



VOID* NvramAllocatePages(UINTN Size){
    EFI_PHYSICAL_ADDRESS Address;
    EFI_STATUS Status;

    if (IsNvramRuntime()) return NULL;
    Status = pBS->AllocatePages(AllocateAnyPages, NvramMemoryType, EFI_SIZE_TO_PAGES(Size), &Address);
    if (EFI_ERROR(Status)) return NULL;
    return (VOID*)(UINTN)Address;
}

VOID NvramFreePages(VOID *Address, UINTN Size){
    EFI_STATUS Status;

    if (IsNvramRuntime()) return ;
    Status = pBS->FreePages((EFI_PHYSICAL_ADDRESS)(UINTN)Address, EFI_SIZE_TO_PAGES(Size));
    ASSERT_EFI_ERROR (Status);
}

AMI_FLASH_PROTOCOL *LocateFlashProtocol(){
    EFI_STATUS Status;
    AMI_FLASH_PROTOCOL *Flash;

    Status = pBS->LocateProtocol(&gAmiFlashProtocolGuid, NULL, (VOID**)&Flash);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return NULL;
    return Flash;
}

VOID ReenumerateVarStore(){
    if (!NvramSmmIsActive || GetVariableSmmActive) return;
    if (UpdateInfo.ShutDown){
        GetVariableSmmActive = TRUE;
        return;
    }

    if ((UpdateInfo.StoreMask & UPDATE_INFO_ENUMERATE_NV_STORE)!=0){
        ResetVarStore(VarStoreInfo.NvInfo);
        VarStoreInfo.LastInfoIndex = 0;
        UpdateNestedVarstores();
    }
    if ((UpdateInfo.StoreMask & UPDATE_INFO_ENUMERATE_MEM_STORE)!=0){
        ResetVarStore(VarStoreInfo.MemInfo);
    }
    if (UpdateInfo.StoreMask != 0){
        UpdateInfo.StoreMask = 0;
        IndexReset();
    }
}

/**
    This function searches for Var with specific GUID and Name
    beginning and ending critical section

    @param VariableName pointer to Var Name in Unicode
    @param VendorGuid pointer to Var GUID
    @param Attributes Pointer to memory where Attributes will be returned
    @param DataSize size of Var - if smaller than actual EFI_BUFFER_TOO_SMALL
        will be returned and DataSize will be set to actual size needed
    @param Data Pointer to memory where Var will be returned

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI DxeGetVariableSafe(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    OUT UINT32 *Attributes OPTIONAL,
    IN OUT UINTN *DataSize, OUT VOID *Data
)
{
    EFI_STATUS Status;

    BEGIN_CRITICAL_SECTION(NvramCs);
    ReenumerateVarStore();
    if (GetVariableSmmActive)
        Status = DxeGetVariableSmmWrapper(
                     VariableName,VendorGuid,Attributes,DataSize,Data
                );
    else
        Status = DxeGetVariableWrapper(
                     VariableName,VendorGuid,Attributes,DataSize,Data
                );
    END_CRITICAL_SECTION(NvramCs);
    return Status;
}

/**
    This function searches for Var following after Var with specific name and GUID
    and returns it's Name, beginning and ending critical section .

    @param VariableNameSize size of Varname - if smaller than actual EFI_BUFFER_TOO_SMALL
        will be returned and DataSize will be set to actual size needed
    @param VariableName pointer where Var Name in Unicode will be stored
    @param VendorGuid pointer to memory where Var GUID is stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI DxeGetNextVariableNameSafe(
    IN OUT UINTN *VariableNameSize,
    IN OUT CHAR16 *VariableName, IN OUT EFI_GUID *VendorGuid
)
{
    EFI_STATUS Status;

    BEGIN_CRITICAL_SECTION(NvramCs);
    ReenumerateVarStore();
    if (GetVariableSmmActive)
        Status = DxeGetNextVariableNameSmmWrapper(
                     VariableNameSize,VariableName,VendorGuid
                 );
    else
        Status = DxeGetNextVariableNameWrapper(
                     VariableNameSize,VariableName,VendorGuid
                 );
    END_CRITICAL_SECTION(NvramCs);
    return Status;
}

/**
    This function sets Var with specific GUID, Name and attributes
    beginning and ending critical section.

    @param VariableName pointer to Var Name in Unicode
    @param VendorGuid pointer to Var GUID
    @param Attributes Attributes of the Var
    @param DataSize size of Var
    @param Data Pointer to memory where Var data is stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI DxeSetVariableSafe(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    IN UINT32 Attributes, IN UINTN DataSize, IN VOID *Data
)
{
    EFI_STATUS Status;

    BEGIN_CRITICAL_SECTION(NvramCs);
    if (NvramSmmIsActive)
        Status = DxeSetVariableSmmWrapper(
                     VariableName,VendorGuid,Attributes,DataSize,Data
                 );
    else
        Status = DxeSetVariableWrapper(
                     VariableName,VendorGuid,Attributes,DataSize,Data
                 );
    END_CRITICAL_SECTION(NvramCs);
    return Status;
}

EFI_STATUS EFIAPI QueryVariableInfoWrapper (
    IN UINT32 Attributes,
    OUT UINT64 *MaximumVariableStorageSize,
    OUT UINT64 *RemainingVariableStorageSize,
    OUT UINT64 *MaximumVariableSize
){
    EFI_STATUS Status;
    if ((Attributes & EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS) && NoAuthWriteAccess) 
    {
        return EFI_UNSUPPORTED;
    }
    ReenumerateVarStore();
    if (GetVariableSmmActive)
        Status = QueryVariableInfoSmmWrapper(
            Attributes,MaximumVariableStorageSize,RemainingVariableStorageSize,MaximumVariableSize
        );
    else
        Status = QueryVariableInfo(
            Attributes,MaximumVariableStorageSize,RemainingVariableStorageSize,MaximumVariableSize
        );
    return Status;
}

EFI_STATUS EFIAPI RequestToLockWrapper (
  IN CONST EDKII_VARIABLE_LOCK_PROTOCOL *This,
  IN       CHAR16                       *VariableName,
  IN       EFI_GUID                     *VendorGuid
  )
{
    EFI_STATUS Status;
    if (FeaturePcdGet(AmiPcdNvramSmmSupport))
        Status = RequestToLockSmmWrapper(
            This, VariableName, VendorGuid
        );
    else
        Status = DxeRequestToLock(
            This, VariableName, VendorGuid
        );
    return Status;
}

////////////////////////////////////////////////////////////////////
UINT32 CounterLo=0, CounterHi=0;

/**
    This function gets Next High Monotonic Count

    @param HighCount pointer to where result will be stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI GetNextHighMonotonicCount(OUT UINT32 *HighCount)
{
    EFI_STATUS Status;
    UINTN Size = sizeof(UINT32);

    if (!HighCount) return EFI_INVALID_PARAMETER;

    if (!CounterHi)
    {
        Status = pRS->GetVariable(L"MonotonicCounter", &gAmiGlobalVariableGuid,
                                  NULL, &Size, &CounterHi
                                 );

        if (EFI_ERROR(Status) && Status != EFI_NOT_FOUND)
            return Status;
    }

    ++CounterHi;
    Status = pRS->SetVariable(
                 L"MonotonicCounter", &gAmiGlobalVariableGuid,
                 EFI_VARIABLE_NON_VOLATILE|EFI_VARIABLE_BOOTSERVICE_ACCESS|EFI_VARIABLE_RUNTIME_ACCESS,
                 Size, &CounterHi
             );

    if (EFI_ERROR(Status)) return Status;

    *HighCount = CounterHi;
    return EFI_SUCCESS;
}

/**
    This function gets Next low and high Monotonic Count

    @param Count pointer to where result will be stored

    @retval EFI_STATUS based on result

**/
EFI_STATUS EFIAPI GetNextMonotonicCount(OUT UINT64 *Count)
{
    if (!Count) return EFI_INVALID_PARAMETER;

    ((UINT32*)Count)[0] = ++CounterLo;
    //--- If low counter overflows - GetNextHighMonotonicCount
    if (!CounterLo) return  GetNextHighMonotonicCount(&((UINT32*)Count)[1]);
    //--- If not - get current HighMonotonicCount
    else ((UINT32*)Count)[1] = CounterHi;

    return EFI_SUCCESS;
}

/**
    This function initialize pBS MonotonicCount services and installs
    corresponding protocols

    @param ImageHandle Image handle
    @param SystemTable pointer to System Table

    @retval VOID

**/
VOID MonotonicCounterInit(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_HANDLE Handle = NULL;
    UINT32 HighCount;
    InitAmiLib(ImageHandle,SystemTable);
    pRS->GetNextHighMonotonicCount = GetNextHighMonotonicCount;
    pBS->GetNextMonotonicCount = GetNextMonotonicCount;
    //Initialize High part of the monotonic counter
    GetNextHighMonotonicCount(&HighCount);
    pBS->InstallMultipleProtocolInterfaces(
        &Handle,
        &gEfiMonotonicCounterArchProtocolGuid, NULL,
        NULL
    );
}

VOID EFIAPI VarExitBS (IN EFI_EVENT Event, IN VOID *Context)
{
    SwitchToRuntime();
    NvramSmmWrapperExitBootServicesHandler();
}

/**
    This function will be invoked to convert
    runtime pointers to virtual address

    @param Info pointer to NVRAM_STORE_INFO structure to fix-up

    @retval VOID

**/
VOID VarStoreVirtualFixup(NVRAM_STORE_INFO *Info)
{
    pRS->ConvertPointer(0,(VOID**)&Info->pFirstVar);
    pRS->ConvertPointer(0,(VOID**)&Info->pEndOfVars);
    pRS->ConvertPointer(0,(VOID**)&Info->NvramGuidsAddress);
    pRS->ConvertPointer(0,(VOID**)&Info->NvramAddress);
    Info->pLastReturned=NULL;
    if (Info->Interface!=NULL){
        Info->Interface->VirtualFixup(Info->Interface,Info->NvramAddress);
        pRS->ConvertPointer(0,(VOID**)&Info->Interface);
    }
}

/**
    This function will be invoked to convert whole Var infrastructure
    and pRS runtime pointers to virtual address

    @param Event signaled event
    @param Context calling context

    @retval VOID

**/
VOID EFIAPI VarVirtAddressChange (IN EFI_EVENT Event, IN VOID *Context)
{
    UINTN i;

    pRS->ConvertPointer(0,(VOID**)&NvramCs);
    if (FeaturePcdGet(AmiPcdNvramSmmSupport))
        NvramSmmWrapperVirtualAddressChangeHandler();

    for (i=0; i< VarStoreInfo.InfoCount; i++)
        VarStoreVirtualFixup(&VarStoreInfo.NvramInfo[i]);

    pRS->ConvertPointer(0,(VOID**)&VarStoreInfo.NvInfo);
    pRS->ConvertPointer(0,(VOID**)&VarStoreInfo.MemInfo);
    if (VarStoreInfo.NvramDriverBuffer)
        pRS->ConvertPointer(0,(VOID**)&VarStoreInfo.NvramDriverBuffer);
    VarStoreInfo.LastInfoIndex = 0;
    LockBufferVirtualFixup();
    //pRS is converted by the library
    //(call to InitAmiRuntimeLib initiates the processes)
    AuthServiceVirtualFixup();
    IndexVirtualFixup();
}

EDKII_VARIABLE_LOCK_PROTOCOL   gVariableLock              = { RequestToLockWrapper };

VOID SwitchToSmmWrapper (){
    EFI_HANDLE Handle = NULL;
    EFI_STATUS Status;

    NvramSmmIsActive = TRUE;
    pBS->InstallMultipleProtocolInterfaces (
        &Handle, &gEdkiiVariableLockProtocolGuid, &gVariableLock, NULL
    );
    Status = RequestToLockWrapper(
                       &gVariableLock,L"HwErrRecSupport",&gEfiGlobalVariableGuid
                       );
    ASSERT_EFI_ERROR(Status);

}

VOID EFIAPI SmmReadyToLockCallback (
    IN EFI_EVENT    Event,
    IN VOID         *Context
){
    SwitchToBds();
    if (FeaturePcdGet(AmiPcdNvramSmmSupport)){
        if (!NvramSmmIsActive){
            // This should never happen under normal circumstances.
            NVRAM_TRACE((DEBUG_ERROR,"NVRAM: NVRAM SMM failed to start\n"));
            ASSERT(FALSE);
            // If NVRAM SMM didn't start, switch to SMM wrapper anyway, which will lead to all variable services returning EFI_UNSUPPORTED
            // (Communicate will be returning EFI_UNSUPPORTED because ActivateSmmWrapper was never called and SmmCommProtocol is NULL).
            // We are not using DXE API going forward due to security considerations.
            // The project is configured to use NVRAM SMM API, which means secure implementation is expected.
            // Since we can't provide secure implementation, we are disabling NVRAM functionality altogether.
            NvramSmmIsActive = TRUE;
        }
    }
}

/**
    This function is the entry point for this DXE Driver.
    This function initialize pRS Var, MonotonicCount services
    (if MONOTONIC_COUNT_IN_NVRAM token is set) and installs
    corresponding protocols and sets NvRamMailbox Var not in SMM

    @param ImageHandle Image handle
    @param SystemTable pointer to System Table

    @retval EFI_STATUS based on result
**/
EFI_STATUS EFIAPI NvRamInit(
    IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_HANDLE Handle = NULL;
    EFI_EVENT Event;
    VOID *Registration;
    EFI_STATUS Status;
    NVRAM_MAILBOX Mailbox;
    VOID *Address;
    UINT16 HwErrRecSupport = 1;

    //When AmiPcdNvramSmmSupport is enabled, DXE memory buffers are not used at runtime.
    //We're allocating them as EfiRuntimeServicesData because NVRAM SMM access then after EndOfDxe and
    //only RT buffers can be accessed from SMM after EndOfDxe.
    NvramMemoryType = EfiRuntimeServicesData;

    InitAmiRuntimeLib(
        ImageHandle, SystemTable, VarExitBS, VarVirtAddressChange
    );
    PROGRESS_CODE(DXE_NVRAM_INIT);
#ifdef EFI_2_70_SYSTEM_TABLE_REVISION   
    if (SystemTable->Hdr.Revision >= EFI_2_70_SYSTEM_TABLE_REVISION)
    {
        NoAuthWriteAccess = TRUE;
    }
#endif
    CreateCriticalSection(&NvramCs);

    Status = NvramInitialize();
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    if (!CompatibilityChecker(DxeGetVariableWrapper))
    {
        UINT32 ResetPropertyMask;
        NVRAM_TRACE((DEBUG_ERROR, "NVRAM DXE: Incompatible NVRAM detected\n"));
        ResetPropertyMask = PcdGet32(AmiPcdNvramResetPropertyMask) | NVRAM_RESET_KEEP_VARIABLES_WITHOUT_DEFAULTS;
        NvResetConfiguration(
            VarStoreInfo.NvInfo,
            TRUE, ResetPropertyMask
        );
    }
    AuthVariableServiceInit ();

    if (FeaturePcdGet(AmiPcdNvramSmmSupport)){
        Status = NvramSmiDxeEntry(ImageHandle,SystemTable);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;

        Mailbox.VarStoreInfoPtr = &VarStoreInfo;
        Mailbox.SwitchToSmmWrapper = ActivateSmmWrapper;
        Mailbox.UpdateInfo = &UpdateInfo;
        UpdateInfo.StoreMask = 0;
        UpdateInfo.ShutDown = FALSE;

        Status = DxeSetVariableWrapper(
            L"NvramMailBox",&gAmiNvramSmmCommunicationGuid,EFI_VARIABLE_BOOTSERVICE_ACCESS,sizeof(Mailbox),&Mailbox
        );
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;
    }

    // We need to know when to switch to the Runtime mode.
    // Switching to runtime happens on exit boot services or legacy boot event.
    // exit boot services callback is installed by the InitAmiRuntimeLib function.
    // Let's install legacy boot callback here.
    // We are using gAmiLegacyBootProtocolGuid instead of legacy boot event to
    // make sure other legacy boot event callbacks can access boot time variables.
    // gAmiLegacyBootProtocolGuid is installed by CSM after signaling legacy boot event.
    RegisterProtocolCallback(
        &gAmiLegacyBootProtocolGuid,
        VarExitBS,
        NULL, &Event, &Registration
    );

    RegisterProtocolCallback(
        &gEfiDxeSmmReadyToLockProtocolGuid,
        SmmReadyToLockCallback,
        NULL, &Event, &Registration
    );

    pRS->GetNextVariableName = DxeGetNextVariableNameSafe;
    pRS->GetVariable = DxeGetVariableSafe;
    pRS->SetVariable = DxeSetVariableSafe;
    pRS->QueryVariableInfo = QueryVariableInfoWrapper;

    pBS->InstallMultipleProtocolInterfaces(
        &Handle,
        &gEfiVariableArchProtocolGuid, NULL,
        &gEfiVariableWriteArchProtocolGuid, NULL,
        NULL
    );
    //Creating HwErrRecSupport Variable which will be locked in SwitchToSmmWrapper if AmiPcdNvramSmmSupport is enabled 
    //or later in this function otherwise.
    Status = DxeSetVariableWrapper(
                        L"HwErrRecSupport",&gEfiGlobalVariableGuid, 
                        EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        sizeof(UINT16),&HwErrRecSupport
                    );
    ASSERT_EFI_ERROR(Status);
    
    if (!FeaturePcdGet(AmiPcdNvramSmmSupport)){
        Status = pBS->AllocatePool ( EfiRuntimeServicesData, NVRAM_SIZE / 2, &Address);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;
        InitNvramLockBuffer(Address,NVRAM_SIZE/2);
        pBS->InstallMultipleProtocolInterfaces (
            &Handle,
            &gEdkiiVariableLockProtocolGuid, &gVariableLock,
            &gAmiNvramUpdateProtocolGuid, &AmiNvramUpdate,
            NULL
        );
        Status = DxeRequestToLock(
                   &gVariableLock,L"HwErrRecSupport",&gEfiGlobalVariableGuid
                   );
        ASSERT_EFI_ERROR(Status);

    }
    //--- Install MonotonicCount services (if NVRAM_MONOTONIC_COUNTER_SUPPORT token is set)
    if (NvramMonotonicCounterSupport) MonotonicCounterInit(ImageHandle, SystemTable);
    return EFI_SUCCESS;
}

BOOLEAN IsLockEnforcementActive (){
    return IsSmmLocked();
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
