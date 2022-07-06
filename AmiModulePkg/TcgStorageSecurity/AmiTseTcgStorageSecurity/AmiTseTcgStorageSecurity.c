//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiTseTcgStorageSecurity.c
    Provides OPAL password related AMITSE setup routines.

**/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/AMIScreenMgmt.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DiskInfo.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SmmControl2.h>
#include <AMIVfr.h>
#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
#include "Core/em/AMITSE/Inc/PwdLib.h"
#include "Core/em/AMITSE/Inc/variable.h"
#endif
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
  #include "Nvme/NvmeIncludes.h"
  #include "Nvme/NvmeController.h"
  #include "Protocol/AmiNvmeController.h"
  #include "Nvme/NvmePassthru.h"
#endif

#include "../TcgStorageSecurityCommon.h"
#include "AmiTseTcgStorageSecurity.h"


//---------------------------------------------------------------------------

// Remove below code when gAmiTcgStorageSecurityProtocolGuid 
// definition is added in AmiModulePkg
#ifndef AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID 
#define AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID \
        { 0xb396da3a, 0x52b2, 0x4cd6, { 0xa8, 0x9a, 0x13, 0xe7, 0xc4, 0xae, 0x97, 0x90 } }
#endif

EFI_GUID gAmiTcgStorageSecurityProtocolGuid = AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID;

EFI_GUID gTcgPasswordPromptEnterGuid = HDD_PASSWORD_PROMPT_ENTER_GUID;
EFI_GUID gTcgPasswordPromptExitGuid  = HDD_PASSWORD_PROMPT_EXIT_GUID;
#if ENABLE_DEVICE_RESET_THRU_PSID
EFI_GUID gAmiTcgStorageDeviceResetGuid = AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL_GUID;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
EFI_GUID    gRstPasswordDeviceProtocolGuid = NVME_RST_PASSWORD_PROTOCOL_GUID;
#endif
static EFI_HII_STRING_PROTOCOL *HiiString = NULL;
static CHAR8                   *SupportedLanguages=NULL;
TCG_STORAGE_SECURITY_INTERNAL_DATA     *TcgSecurityInternalData = NULL;
EFI_HANDLE                     gTcgSecEndHandle = NULL;
EFI_HANDLE                     TcgNotifyHandle;
static  EFI_HANDLE             *gHandleBuffer = NULL;
EFI_EVENT                      TcgNotifyEvent;
VOID                           *TcgNotifyRegistration;
#if FREEZE_LOCK_OPAL
EFI_EVENT                      TcgReadyToBootEvent;
#endif
#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
EFI_EVENT                      gTCGSetupSameSysPwEvent;
VOID                          *gTCGSetupSameSysPwRegistration = NULL;
#endif
BOOLEAN                        OpalFreeze      = FALSE;
UINT16                         gOpalSecurityCount = 0;
BOOLEAN                        TcgCheckAllController=FALSE;
AMI_POST_MANAGER_PROTOCOL   	*mAmiPostMgr = NULL;
AMI_TSE_SCREEN_MGMT_PROTOCOL	*mAmiTseScreenMgmt = NULL;

extern VOID MouseInit(VOID);
extern BOOLEAN IsMouseSupported(VOID);  
extern BOOLEAN IsSoftKbdSupported(VOID);
extern VOID SetPwdKeyboardLayout(VOID);
extern VOID TSEStringReadLoopEntryHook(VOID);
extern VOID TSEStringReadLoopExitHook(VOID);
extern VOID ResetPwdKeyboardLayout(VOID);


/** @internal
    Internal function that installs/uninstall protocol
    with a specified GUID and NULL interface.
    Such protocols can be used as event signaling mechanism.

    @param ProtocolGuid Pointer to the protocol GUID

    @retval VOID
    
    @endinternal
**/
VOID
TcgSecuritySignalProtocolEvent (
    IN  EFI_GUID    *ProtocolGuid
)
{

    EFI_HANDLE  Handle = NULL;
    EFI_STATUS   Status;

    Status = gBS->InstallProtocolInterface (
                                &Handle,
                                ProtocolGuid,
                                EFI_NATIVE_INTERFACE,
                                NULL );
    ASSERT_EFI_ERROR( Status );

    Status = gBS->UninstallProtocolInterface (
                                Handle,
                                ProtocolGuid,
                                NULL);
    ASSERT_EFI_ERROR( Status );
    
    return;
}

/** @internal
    Register the Protocol call back event

    @param VOID

    @retval VOID
    
    @endinternal
**/
BOOLEAN
RegisterSecurityEndNotification (
    VOID
)
{
    EFI_STATUS Status ;
    
#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    RegisterProtocolCallback( &gAmiTseNVRAMUpdateGuid,
                              TcgStorageSecSetupSameSysPw,
                              NULL,
                              &gTCGSetupSameSysPwEvent,
                              &gTCGSetupSameSysPwRegistration );
#endif
 
#if FREEZE_LOCK_OPAL
    // Create callback for ReadyToBoot event.
    EfiCreateEventReadyToBootEx( TPL_CALLBACK,
                                 TcgStorageReadyToBootNotification,
                                 NULL,
                                 &TcgReadyToBootEvent );
#endif    
    
    Status = gBS->CreateEvent(  EVT_NOTIFY_SIGNAL,
                                TPL_CALLBACK,
                                TcgNotificationFunction,
                                &TcgNotifyRegistration,
                                &TcgNotifyEvent);

    ASSERT_EFI_ERROR(Status);
    Status = gBS->RegisterProtocolNotify(   &gAmiHddSecurityEndProtocolGuid,
                                            TcgNotifyEvent,
                                            &TcgNotifyRegistration);
    ASSERT_EFI_ERROR(Status);

    //
    // get any of these events that have occurred in the past
    // When no con in devices are present this function is called later in BDS after InitConVars,
    // by that time if more than one controller present, Collect all controller and install gAmiHddPasswordVerifiedGuid
    TcgCheckAllController = TRUE;
    gBS->SignalEvent( TcgNotifyEvent );

    return FALSE;
}

/**
    HDD notification function gets called when HddSecurityEnd Protocol get installed.

    @param EFI_EVENT Event - Event to signal
    @param void HddRegContext - Event specific context (pointer to NotifyRegisteration

    @retval VOID

**/
VOID
TcgNotificationFunction (
    EFI_EVENT   Event,
    VOID        *HddRegContext
)
{
    EFI_STATUS                  Status;
    EFI_HANDLE                  *HandleBuffer = NULL;
    TCG_STORAGE_SECURITY_INTERNAL_DATA  *DataPtr = NULL;
    UINTN                       NumHandles;
    UINTN                       Index = 0;
    EFI_HANDLE                  Handle = NULL;
    EFI_GUID                    AmiTseTcgNotifyGuid = AMI_TSE_TCG_NOTIFY_GUID;
    UINTN                       BufferSize = sizeof(UINTN);

    if ( HddRegContext == NULL ) {
        return;
    }

    //Locate the handle
    if(TcgCheckAllController) {
        // Locate all Handles that were installed previously.
    	TcgCheckAllController = FALSE;
        Status = gBS->LocateHandleBuffer(   ByProtocol,
                                            &gAmiHddSecurityEndProtocolGuid,
                                            NULL,
                                            &NumHandles,
                                            &HandleBuffer);
    } else {
        Status = gBS->LocateHandleBuffer(   ByRegisterNotify,
                                            NULL,
                                            *(VOID**)HddRegContext,
                                            &NumHandles,
                                            &HandleBuffer);
    }

    // If protocol not installed return
    if ( EFI_ERROR( Status ))
        return;

    //Locate the Security Protocols
    gOpalSecurityCount = InitTcgSecurityInternalDataPtr();

	if (!gOpalSecurityCount)
		return;
	
    for(Index=0; Index<gOpalSecurityCount; Index++) {
        //
        //Initialize the DataPtr
        //
        DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA *) TCGPasswordGetDataPtr(Index);

        //
        // Search for locked Hard disc and not password verification done
        //
        if(DataPtr->Locked && !DataPtr->Validated){
            break;
        }
    }

    // Validate the password only if HDD is locked
    if( (gOpalSecurityCount != 0 ) && (NULL != DataPtr ) &&
            (DataPtr->Locked ) && (!DataPtr->Validated ) ) {
    	TCGPasswordCheck();
    }
    
    // ConnectController once HDD is unlocked when no CONOUT present.
    for (Index=0; Index<gOpalSecurityCount; Index++) {
        //Initialize the DataPtr
        DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA *) TCGPasswordGetDataPtr(Index);

        gBS->ConnectController(DataPtr->DeviceHandle,
                                        NULL,
                                        NULL,
                                        TRUE);
    }
    
    // Install gAmiHddPasswordVerifiedGuid on specific Controller Handle.
    for(Index = 0; Index < NumHandles; Index++){

        gTcgSecEndHandle = HandleBuffer[Index];

        // Install the Unlocked Protocol to notify HDD has been unlocked
        // In case gAmiHddPasswordVerifiedGuid is already installed
        // on gTcgSecEndHandle, Error shall be ignored to proceed.
        if ( gTcgSecEndHandle != NULL ) {
            gBS->InstallProtocolInterface( &gTcgSecEndHandle,
                                           &gAmiHddPasswordVerifiedGuid,
                                           EFI_NATIVE_INTERFACE,
                                           NULL);
        }
    }

    gBS->FreePool(HandleBuffer);
    
    Status = gBS->LocateHandle ( ByProtocol,
                                  &AmiTseTcgNotifyGuid,
                                  NULL,
                                  &BufferSize,
                                  &Handle );
    if (Status == EFI_SUCCESS) {
        Status = gBS->ReinstallProtocolInterface(
                        Handle,
                        &AmiTseTcgNotifyGuid,
                        NULL,
                        NULL);
    } else {
        // Notify setup modules that AMITSE initialized TCG device data.
        Status = gBS->InstallProtocolInterface( &Handle, 
                                                &AmiTseTcgNotifyGuid, 
                                                EFI_NATIVE_INTERFACE,
                                                NULL );
    }
        
    return;
}

/** @internal
    Add the String to HII Database using HiiString Protocol

    @param HiiHandle
    @param String

    @retval VOID
    
    @endinternal
**/
EFI_STRING_ID
OpalPrivateHiiAddString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16 *        String
)
{
    EFI_STATUS      Status;
    CHAR8*          Languages = NULL;
    UINTN           LangSize = 0;
    CHAR8*          CurrentLanguage;
    BOOLEAN         LastLanguage = FALSE;
    EFI_STRING_ID   StringId = 0;

    if(HiiString == NULL) {
        Status = gBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status)) {
            return 0;
        }
    }

    if(SupportedLanguages == NULL) {
        Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = gBS->AllocatePool(EfiBootServicesData, LangSize, (VOID**)&Languages);
            if(EFI_ERROR(Status)) {
                //
                //not enough resources to allocate string
                //
                return 0;
            }
            Status = HiiString->GetLanguages(HiiString, HiiHandle, Languages, &LangSize);
            // Return if failed to get list of supported Languages
            if(EFI_ERROR(Status)) {
                return 0;
            }
        }

        if(Languages == NULL) {
            return 0;
        }

        SupportedLanguages=Languages;
    } else {
        Languages=SupportedLanguages;
    }

    while(!LastLanguage) {
        //
        //point CurrentLanguage to start of new language
        //
        CurrentLanguage = Languages;
        while(*Languages != ';' && *Languages != 0)
            Languages++;

        //
        //Last language in language list
        //
        if(*Languages == 0) {
            LastLanguage = TRUE;
            if(StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            }
            if(EFI_ERROR(Status)) {
                return 0;
            }
        } else {
            //
            //put null-terminator
            //
            *Languages = 0;
            if(StringId == 0) {
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            } else {
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            }
            *Languages = ';';       //restore original character
            Languages++;
            if(EFI_ERROR(Status)) {
                return 0;
            }
        }
    }
    return StringId;
}

/** @internal
    Add the String to HiiDatabase

    @param HiiHandle
    @param String

    @retval VOID

    @endinternal
**/
EFI_STRING_ID
OpalPasswordHiiAddString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  CHAR16          *String
)
{
    EFI_STRING_ID   StringId = 0;

    StringId=OpalPrivateHiiAddString( HiiHandle, String );

    //
    // Error in Adding String. Try with Default function that AMITSE has.
    //
    if(StringId == 0) {
        StringId= HiiAddString( HiiHandle, String );
    }

    return StringId;
}
/** @internal
    Check whether global range is locked or not for the device.

    @param Handle                - DeviceHandle
    @param HddSecurityProtocol   - Instance to hold HddSecurityProtocol instance
	
    @retval GlobalLbaRangeLocked  - TRUE, Global LBA range is locked
    
    @endinternal
**/
BOOLEAN
CheckForGlobalLbaRangeLock (
    EFI_HANDLE                  Handle,
    AMI_HDD_SECURITY_PROTOCOL   *HddSecurityProtocol,
    BOOLEAN                     *GlobalLbaRangeLocked
) {
    EFI_STATUS                               Status;
    EFI_BLOCK_IO_PROTOCOL                    *BlockIo = NULL;
    VOID                                     *Buffer = NULL;
    BOOLEAN                                  LockedStatus = FALSE;
    AMI_TCG_STORAGE_SECURITY_INTERFACE       *Security = NULL;
    
    *GlobalLbaRangeLocked = FALSE;  // Initialize default value to FALSE

    if (!CheckOpalSecurityStatus( HddSecurityProtocol, &LockedStatus, SecurityLockedMask ) )
        return FALSE;
    
    *GlobalLbaRangeLocked = LockedStatus; // Initialized locked status based on level 0 data
    
    // If Locked Bit is set in Level0data, then verify whether global ranged is locked or not 
    // by reading LBA0 data.
    if (LockedStatus) {
        Status = gBS->HandleProtocol(Handle, &gEfiBlockIoProtocolGuid, (VOID**)&BlockIo);
        // In RAID mode, BlockIo wont be installed on device handle, where security protocol is installed. 
        // So retrieved BlockIo interface from SATA_DEVICE_INTERFACE.
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0))
        if (EFI_ERROR(Status)){
            Security = (AMI_TCG_STORAGE_SECURITY_INTERFACE *)HddSecurityProtocol;
            if(Security->BusInterfaceType == AmiStorageInterfaceAhci) {
                BlockIo = &(((SATA_DEVICE_INTERFACE*)Security->BusInterface)->SataBlkIo->BlkIo);
            }
        }
#endif
        if (BlockIo != NULL ){
            Status = gBS->AllocatePool(EfiBootServicesData, BlockIo->Media->BlockSize, &Buffer);      
            ASSERT_EFI_ERROR(Status);
            Status = BlockIo->ReadBlocks(BlockIo,
                                         BlockIo->Media->MediaId,
                                         0,    // Lba 0
                                         BlockIo->Media->BlockSize,
                                         Buffer);
            // If LBA read is success , then update GlobalLbaRangeLocked flag to FALSE, indicating that whole LBA range is not locked.
            if (!EFI_ERROR(Status)) {
                *GlobalLbaRangeLocked = FALSE;
            }    
            gBS->FreePool(Buffer);
        } 
    } 
    return TRUE;
}

/** @internal
    Function checks the TcgStorageSecurityProtocol handles are valid or not.

    @param HandleBuffer       Handle to the buffer
    @param Count              Number of HDD/NVMe connected.

    @retval VOID
    
    @note
    If any invalid handle is found, HandleBuffer and HanldeCount will be updated
    with valid handles and count.

    @endinternal
**/

VOID 
GetValidTcgStorageSecurityHandles (
    IN OUT EFI_HANDLE    *HandleBuffer,
    IN OUT UINTN         *HandleCount
)
{
    EFI_STATUS                  Status;
    UINTN                       Index;
    UINTN                       Index1;
    UINTN                       ValidHandleCount = *HandleCount;
    EFI_HANDLE                  ControllerHandle = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath;
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
    AMI_TCG_STORAGE_SECURITY_INTERFACE  *Security = NULL;
    VOID                        *NvmeRstInterface = NULL;
#endif  
    for( Index=0; Index < ValidHandleCount; Index++ ) {
        Status = gBS->OpenProtocol (
                         HandleBuffer[Index],
                         &gEfiDevicePathProtocolGuid,
                         (VOID**)&DevicePath,
                         NULL,
                         NULL,
                         EFI_OPEN_PROTOCOL_GET_PROTOCOL
                         );
        if(!EFI_ERROR(Status)) {
            // Find the PCI I/O handle corresponding to this device.
            Status = gBS->LocateDevicePath (
                                 &gEfiPciIoProtocolGuid,
                                 &DevicePath,
                                 &ControllerHandle
                                 );
             if(!EFI_ERROR(Status)) {
                 continue;
             }
        }
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )
        // Check For valid NvmeRst devices
        Status = gBS->HandleProtocol(HandleBuffer[Index], &gAmiTcgStorageSecurityProtocolGuid, (VOID**)&Security);
        if ( !EFI_ERROR(Status) && ( Security->BusInterfaceType == AmiStorageInterfaceNvme ) ) {
             if( ((NVME_RST_INTERFACE*)Security->BusInterface)->Signature == RST_PWD_SIGNATURE) {
               Status = gBS->OpenProtocol (
                                HandleBuffer[Index],
                                &gRstPasswordDeviceProtocolGuid,
                                (VOID**)&NvmeRstInterface,
                                NULL,
                                NULL,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );
               if(!EFI_ERROR(Status)) {
                   continue;
               }
           }
        }
#endif
        // Found an invalid device handle, remove this handle and reduce the 
        // valid handle count.
        ValidHandleCount--;
        for(Index1 = Index; Index1 < ValidHandleCount; Index1++ ) {
            HandleBuffer[Index1] = HandleBuffer[Index1+1];
        }
        HandleBuffer[Index1] = 0;
        Index--;
    }
    *HandleCount = ValidHandleCount;
    return;
}

/** @internal
    Function checks the locally stored TcgStorageSecurityProtocol handles are valid or not

    @param HandleBuffer       Handle to the buffer
    @param Count              Number of HDD connected.

    @retval BOOLEAN

    @endinternal
**/

BOOLEAN IsStoredTcgPasswordDevHandleValid (
    EFI_HANDLE    *HandleBuffer,
    UINTN          Count
)
{
    UINTN    i, j;
    BOOLEAN  AllTcgPwddDevHandleAreValid=TRUE;
    
    if (gOpalSecurityCount!=Count) {
        return FALSE;
    }
    
    for( i=0; i<gOpalSecurityCount; i++ ) {
        for( j=0; j<Count; j++ ) {
            if( gHandleBuffer[i] == HandleBuffer[j]) {
                break;
            }
        }
        if(j==Count){
            gHandleBuffer[i] = 0;
            AllTcgPwddDevHandleAreValid=FALSE;
        }
    }
    
    return AllTcgPwddDevHandleAreValid;
}

/** @internal
    Locate the Security Protocols and return the information

    @param VOID

    @retval VOID
    
    @endinternal
**/
UINT16
InitTcgSecurityInternalDataPtr (
)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      *HandleBuffer = NULL;
    UINT16                          Index, PrevHandleCount, HDDCount = 0;
    UINTN                           Count;
    CHAR16                          *Name;
    CHAR16                          Temp[60];
    AMI_HDD_SECURITY_PROTOCOL       *HddSecurityProtocol = NULL;
    TCG_STORAGE_SECURITY_INTERNAL_DATA      *DataPtr = NULL;
    EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
    EFI_DEVICE_PATH_PROTOCOL        *DevicePathNode;
    PCI_DEVICE_PATH                 *PciDevicePath;
    CHAR16                          *Buff=L"P";
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    UINT32                          PortNumber;
    UINT32                          PMPortNumber;
#endif
    AMI_TCG_STORAGE_SECURITY_INTERFACE      *Security = NULL;
#if ENABLE_DEVICE_RESET_THRU_PSID
    AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL    *TcgStorageDevReset = NULL;
#endif
    TCG_STORAGE_SECURITY_INTERNAL_DATA      *TempIDEPasswordSecurityData = NULL;

    Status = gBS->LocateHandleBuffer(   ByProtocol,
                                        &gAmiTcgStorageSecurityProtocolGuid,
                                        NULL,
                                        &Count,
                                        &HandleBuffer);
    // Handle the security feature only for the valid TCG storage security handles 
    // which are being handled by a parent PCI controller handle.
    GetValidTcgStorageSecurityHandles(HandleBuffer,&Count);
    // If already data has been found return with that information
    if ( gOpalSecurityCount != 0 && TcgSecurityInternalData != NULL ) {
        // Check the locally stored AmiTcgStorageSecurityProtocol handles are valid or not
        if(IsStoredTcgPasswordDevHandleValid(HandleBuffer,Count)) {
            return gOpalSecurityCount;     // TcgSecurityInternalData is valid
        }
    }
    //
    // New HDD device found. Need to validate the password for the new HDD
    // and skip the HDD that has been already validated.
    //
    TempIDEPasswordSecurityData = TcgSecurityInternalData;
    
    TcgSecurityInternalData = EfiLibAllocateZeroPool( Count * sizeof(TCG_STORAGE_SECURITY_INTERNAL_DATA));

    if( TcgSecurityInternalData == NULL ) {
        ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);        
        return 0;
    }
    
    gBS->SetMem( TcgSecurityInternalData, sizeof(TCG_STORAGE_SECURITY_INTERNAL_DATA) * Count , 0);
    
    DataPtr = TcgSecurityInternalData;

    for ( Index = 0; Index < Count; Index++ ) {

        // Check if already Validate or not. If already validate don't verify the password again.
        if ( gHandleBuffer != NULL && gOpalSecurityCount != 0 ) {
            PrevHandleCount = gOpalSecurityCount;

            do {
                if ( HandleBuffer[Index] == gHandleBuffer[PrevHandleCount - 1] ) {
                    break;
                }
                PrevHandleCount--;
            } while ( PrevHandleCount != 0 );

            if ( PrevHandleCount != 0 ) {
                MemCopy((TcgSecurityInternalData+Index),(TempIDEPasswordSecurityData+(PrevHandleCount-1)),sizeof(TCG_STORAGE_SECURITY_INTERNAL_DATA));
                HDDCount++;
                continue;
            }
        }

        DataPtr = TcgSecurityInternalData+Index;
        
        //
        // Get the PasswordSecurity Protocol
        //
        Status = gBS->OpenProtocol( HandleBuffer[Index],
                                    &gAmiTcgStorageSecurityProtocolGuid,
                                    (VOID**) &HddSecurityProtocol,
                                    NULL,
                                    HandleBuffer[Index],
                                    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        if ( EFI_ERROR( Status ))
            continue;
#if ENABLE_DEVICE_RESET_THRU_PSID
        // Get the TcgStorageDevice Reset Protocol
        Status = gBS->OpenProtocol( HandleBuffer[Index],
                                    &gAmiTcgStorageDeviceResetGuid,
                                    (VOID**)&TcgStorageDevReset,
                                    NULL,
                                    HandleBuffer[Index],
                                    EFI_OPEN_PROTOCOL_GET_PROTOCOL );
        if ( EFI_ERROR( Status ))
            continue;
#endif
        
        Security = (AMI_TCG_STORAGE_SECURITY_INTERFACE *)HddSecurityProtocol;
        
        if( Security->BusInterfaceType == AmiStorageInterfaceIde ) {
            
            // This condition should not have occurred as per the Design 
            continue;
        }

        Security->PostHddCount = (VOID*)&gOpalSecurityCount;
        Security->PostHddData = (VOID**)&TcgSecurityInternalData;

        if( Security->BusInterfaceType != AmiStorageInterfaceNvme ) {
            // Locate the device path Protocol
            Status = gBS->OpenProtocol( HandleBuffer[Index],
                                        &gEfiDevicePathProtocolGuid,
                                        (VOID**)&DevicePath,
                                        NULL,
                                        HandleBuffer[Index],
                                        EFI_OPEN_PROTOCOL_GET_PROTOCOL );
            if ( EFI_ERROR( Status )) {
                continue;
            }

            DevicePathNode = DevicePath;

            // Traverse the Device Path structure till we reach HARDWARE_DEVICE_PATH
            while (!IsDevicePathEnd (DevicePathNode)) {

                if ((DevicePathNode->Type == HARDWARE_DEVICE_PATH) &&
                    (DevicePathNode->SubType == HW_PCI_DP)){

                    PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
                    break;
                }

                DevicePathNode = NextDevicePathNode (DevicePathNode);
            }

            if (PciDevicePath == NULL) continue;
        }

        Security = (AMI_TCG_STORAGE_SECURITY_INTERFACE *)HddSecurityProtocol;

        if( Security->BusInterfaceType == AmiStorageInterfaceAhci ) {
            //
            //  Get the PMPort and Port number for AHCI opal device
            //
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
            PortNumber = ((SATA_DEVICE_INTERFACE*)Security->BusInterface)->PortNumber;
            PMPortNumber = ((SATA_DEVICE_INTERFACE*)Security->BusInterface)->PMPortNumber == 0xFF ? 0 : 
                                 ((SATA_DEVICE_INTERFACE*)Security->BusInterface)->PMPortNumber;
#endif
        }
        if (CheckForGlobalLbaRangeLock( HandleBuffer[Index], HddSecurityProtocol, &(DataPtr->Locked))){    
            DataPtr->TcgStorageSecProtocol = HddSecurityProtocol;
#if ENABLE_DEVICE_RESET_THRU_PSID
            DataPtr->TcgDeviceResetProtocol = TcgStorageDevReset;
#endif            
            if ( OpalPasswordGetDeviceName( HandleBuffer[Index], &Name )) {

                DataPtr->NameToken = OpalPasswordHiiAddString( gHiiHandle, Name );

                if ( Security->BusInterfaceType == AmiStorageInterfaceNvme ) {
                    // Display the the Port Number in Setup
                    SPrint( Temp, 60, L"%s",Name );
                    DataPtr->PromptToken = OpalPasswordHiiAddString( gHiiHandle, Temp );
                } else if ( Security->BusInterfaceType == AmiStorageInterfaceAhci ) {
                    //  Display the the Port Number in Setup
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
                    SPrint( Temp, 60, L"%s%d:%s", Buff, PortNumber, Name );
#endif
                    DataPtr->PromptToken = OpalPasswordHiiAddString( gHiiHandle, Temp );                  
                }
            }
            DataPtr->Validated = FALSE;
            DataPtr->PostHiiHandle = gHiiHandle;
            DataPtr->DeviceHandle = HandleBuffer[Index];
            HDDCount++;
         } // end if

    } // end of for

    if ( gHandleBuffer != NULL ) {
        MemFreePointer( (VOID **)&gHandleBuffer );
    }

    gHandleBuffer = EfiLibAllocateZeroPool( sizeof(EFI_HANDLE) * Count );
    MemCopy( gHandleBuffer, HandleBuffer, sizeof(EFI_HANDLE) * Count );

    MemFreePointer((VOID**)&HandleBuffer );
    MemFreePointer((VOID**)&TempIDEPasswordSecurityData);
    
    //
    //if no HDD is supported
    //
    if ( HDDCount == 0 ) {
        MemFreePointer((VOID**)&TcgSecurityInternalData );
        TcgSecurityInternalData=NULL;
    }

    if(HDDCount != Count) {
        DEBUG((DEBUG_INFO,"\n Unable to get proper POST HddSecurity Data "));
        ASSERT(FALSE);
    }
    
    return HDDCount;
}

/** @internal
    Return HDD Locked Information

    @param UINTN       Index

    @retval VOID
    
    @endinternal

**/
BOOLEAN
TCGPasswordGetLocked (
    UINTN   Index
)
{
    TCG_STORAGE_SECURITY_INTERNAL_DATA   *DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)TCGPasswordGetDataPtr( Index );

    if(DataPtr == NULL) {
        return 0;
    }
    return DataPtr->Locked;
}

/** @internal
    return the Security Status Information

    @param VOID

    @retval VOID
    
    @endinternal
**/
BOOLEAN
CheckOpalSecurityStatus (
    AMI_HDD_SECURITY_PROTOCOL   *HddSecurityProtocol,
    BOOLEAN                     *ReqStatus,
    UINT16                      Mask
)
{
    UINT16     SecurityStatus = 0;
    EFI_STATUS Status;

    //
    //get the security status of the device
    //
    Status = HddSecurityProtocol->ReturnSecurityStatus( HddSecurityProtocol, &SecurityStatus );

    if ( EFI_ERROR( Status ))
        return FALSE;

    *ReqStatus = (BOOLEAN)((SecurityStatus & Mask) ? TRUE : FALSE );
    return TRUE;
}

/** @internal
    Return the Drive String Name

    @param EFI_HANDLE Controller - the handle of the drive
    @param CHAR16 **wsName - returned pointer to the drive string

    @retval BOOLEAN TRUE - drive string has been found and is in wsName
        - FALSE - drive string has not been found

    @note  it is the caller's responsibility to deallocate the space used for
      wsName
      
    @endinternal
**/
BOOLEAN
OpalPasswordGetDeviceName (
    EFI_HANDLE  Controller,
    CHAR16      **wsName
)
{
    EFI_STATUS                  Status;
    AMI_TCG_STORAGE_SECURITY_INTERFACE  *Security = NULL;
    CHAR16      *DeviceName = NULL;
    BOOLEAN     ReturnStatus = FALSE;
    BOOLEAN     RstControlled = FALSE;
    // Get the AMI_TCG_STORAGE_SECURITY_INTERFACE (actually getting the AMI_HDD_SECURITY_PROTOCOL, but
    //  the AMI_TCG_STORAGE_SECURITY_INTERFACE is an extended version with more information)
    Status = gBS->HandleProtocol(Controller, &gAmiTcgStorageSecurityProtocolGuid, (VOID**)&Security);
    
    if ( !EFI_ERROR(Status) ) {
        
        // Check the SATA controller operating mode, and based on the mode, get the UnicodeString
        //  name of the device
        if ( Security->BusInterfaceType == AmiStorageInterfaceAhci ) {
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
            DeviceName = ((SATA_DEVICE_INTERFACE*)Security->BusInterface)->UDeviceName->UnicodeString;
#endif
        }  else if ( Security->BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )
            if( ((NVME_RST_INTERFACE*)Security->BusInterface)->Signature == RST_PWD_SIGNATURE) {
                DeviceName = \
                    ((NVME_RST_INTERFACE*)Security->BusInterface)->UDeviceName->UnicodeString;
                RstControlled = TRUE;
           }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
         if(!RstControlled) {
            DeviceName = \
                    ((AMI_NVME_CONTROLLER_PROTOCOL*)Security->BusInterface)->UDeviceName->UnicodeString;
         }
#endif
        } else {
            return FALSE;
        } 

        
        // Allocate space to copy the UNICODE device name string
        
        *wsName = EfiLibAllocateZeroPool( sizeof(CHAR16)*(EfiStrLen(DeviceName)+1));

        if ( *wsName!=NULL ) {     
            EfiStrCpy( *wsName, DeviceName);
            ReturnStatus = TRUE;
        }
    }

    // Return true to signify that a device name was discovered
    return ReturnStatus;
}

/** @internal
    Get the password and Validate the HDD password

    @param UINT16 PromptToken,
    @param VOID *DataPtr

    @retval VOID
    
    @endinternal
**/
VOID
AMI_CheckTCGPassword (
    UINT16  PromptToken,
    VOID    *DataPtr
)
{
    UINTN           Index;
    CHAR16          *PasswordEntered;
    EFI_STATUS      Status;
    EFI_STATUS      AuthenticationStatus = EFI_ACCESS_DENIED;  
    CHAR16          *MessageBoxTitle = NULL;
    CHAR16          *Message = NULL;
    UINT16          SetPasswordTitle = 0;
    TEXT_INPUT_TEMPLATE		SetPasswordPrompt[1] = {
                {STRING_TOKEN(STR_TCG_STORAGE_EMPTY), OPAL_PASSWORD_LENGTH, TSE_FILTER_KEY_NULL, TRUE, NULL}};
    UINT16                               SecurityStatus = 0;
    TCG_STORAGE_SECURITY_INTERNAL_DATA   *TempDataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA *)DataPtr;
        
    CheckForKeyHook((EFI_EVENT)NULL, NULL );
    gST->ConIn->Reset( gST->ConIn, FALSE );
    
    // Locate AmiPostManager Protocol
    Status = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL,(void**) &mAmiPostMgr);
    
    if (EFI_ERROR(Status)) {
        return;
    }
    
    // Locate AmiTseScreenMgmt Protocol
    Status = gBS->LocateProtocol(&gTSEScreenMgmtProtocolGuid, NULL,(void**) &mAmiTseScreenMgmt);
    
    if (EFI_ERROR(Status)) {
        return;
    }
		
    PasswordEntered = EfiLibAllocateZeroPool((OPAL_PASSWORD_LENGTH + 1) * sizeof(CHAR16));
    
	if (PasswordEntered == NULL) {
		return;
	}

    TcgSecuritySignalProtocolEvent(&gTcgPasswordPromptEnterGuid);  

#if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
    PromptToken = STRING_TOKEN( STR_TCG_STORAGE_SECURITY_PROMPT );
#endif

    mAmiTseScreenMgmt->SaveTseScreen();
    
    // Give PASSWORD_RETRY_ATTEMPTS chances to enter Admin password
    for ( Index = 0; Index < PASSWORD_RETRY_ATTEMPTS; Index++ ) {
    	
    	mAmiTseScreenMgmt->ClearScreen(EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY);
        
    	if(IsMouseSupported()&& IsSoftKbdSupported()){
            MouseInit(); //Updating Mouse Pointer data based on current Screen Resolution.
            SetPwdKeyboardLayout();
            TSEStringReadLoopEntryHook();//Activate SoftKbd
    	}
    	        
        // Clear password buffer here to display blank in prompt TextBox
        gBS->SetMem( PasswordEntered, (OPAL_PASSWORD_LENGTH + 1) * sizeof(CHAR16), 0);
    	
    	// Display "Enter Admin Password" prompt
        SetPasswordTitle = PromptToken;     
        SetPasswordPrompt[0].ItemToken = STRING_TOKEN(STR_TCG_STORAGE_ENTER_ADMIN);
        SetPasswordPrompt[0].Value = PasswordEntered;
        
        //Get Admin Password
        Status = mAmiPostMgr->DisplayTextBox (
        							gHiiHandle,
                                    SetPasswordTitle,
                                    SetPasswordPrompt,
                                    1,
                                    NULL
                                    );
        if(IsMouseSupported()&& IsSoftKbdSupported()){
            TSEStringReadLoopExitHook();
            ResetPwdKeyboardLayout();//DeActivate SoftKbd
        }
        if ( EFI_SUCCESS != Status ) {
        	break;
        }
    	
		// Authenticate Password if string not NULL
        if ( *(CHAR16*)(SetPasswordPrompt[0].Value) ) {
        	AuthenticationStatus = TCGPasswordAuthenticate( PasswordEntered, DataPtr, FALSE );
        } else {
        	AuthenticationStatus = EFI_INVALID_PARAMETER;
        }
  	
        if ( EFI_SUCCESS == AuthenticationStatus ) {
            break;
        } else {
        	// Display "Invalid Password" Error message
        	MessageBoxTitle = HiiGetString (gHiiHandle, PromptToken);
        	Message = HiiGetString (gHiiHandle, STRING_TOKEN(STR_ERROR_PSWD));

        	mAmiPostMgr->DisplayMsgBox(
                                        MessageBoxTitle,
                                        Message,
                                        MSGBOX_TYPE_OK,
                                        NULL
                                        );
        }        
    } // end of for

    Status = TempDataPtr->TcgStorageSecProtocol->ReturnSecurityStatus( TempDataPtr->TcgStorageSecProtocol, &SecurityStatus );

    // If not Enterprise SED then prompt for User pwd
    if ( !EFI_ERROR(Status) && (EFI_SUCCESS != AuthenticationStatus) && !(SecurityStatus & BIT9)) {

            //
            //Give remaining chances to enter User password
            //
            for (; Index < MAXIMUM_TCG_OPAL_PWD_UNLOCK_ATTEMPTS; Index++ ) {

            	mAmiTseScreenMgmt->ClearScreen(EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY);
            	
            	if(IsMouseSupported()&& IsSoftKbdSupported()){
                    SetPwdKeyboardLayout();
                    TSEStringReadLoopEntryHook();//Activate SoftKbd
                }
                // Clear password buffer here to display blank in prompt TextBox
                gBS->SetMem( PasswordEntered, (OPAL_PASSWORD_LENGTH + 1) * sizeof(CHAR16), 0);
                
                SetPasswordTitle = PromptToken;     
                SetPasswordPrompt[0].ItemToken = STRING_TOKEN(STR_TCG_STORAGE_ENTER_USER);
                SetPasswordPrompt[0].Value = PasswordEntered;
                
                //Get User Password
                Status = mAmiPostMgr->DisplayTextBox (
                							gHiiHandle,
                                            SetPasswordTitle,
                                            SetPasswordPrompt,
                                            1,
                                            NULL
                                            );
                if(IsMouseSupported()&& IsSoftKbdSupported()){
                    TSEStringReadLoopExitHook();//DeActivate SoftKbd
                    ResetPwdKeyboardLayout();
                }
                if ( EFI_SUCCESS != Status ) {
                	break;
                } // end if
            	
        		// Authenticate Password if string not NULL
                if ( *(CHAR16*)(SetPasswordPrompt[0].Value) ) {
                	AuthenticationStatus = TCGPasswordAuthenticate( PasswordEntered, DataPtr, TRUE );
                } else {
                	AuthenticationStatus = EFI_INVALID_PARAMETER;
                }

                if ( EFI_SUCCESS == AuthenticationStatus ) {
                    break;
                } else {
                    if ( (Index + 1) != MAXIMUM_TCG_OPAL_PWD_UNLOCK_ATTEMPTS ) {
                    	// Display "Invalid Password" Error message                  	
                    	MessageBoxTitle = HiiGetString (gHiiHandle, PromptToken);
                    	Message = HiiGetString (gHiiHandle, STRING_TOKEN(STR_ERROR_PSWD));

                    	mAmiPostMgr->DisplayMsgBox(
                                                   MessageBoxTitle,
                                                   Message,
                                                   MSGBOX_TYPE_OK,
                                                   NULL
                                                   );
                    }
                }
            }// end of for
   }// end if

    gBS->SetMem( PasswordEntered, (OPAL_PASSWORD_LENGTH + 1) * sizeof(CHAR16), 0);

    MemFreePointer((VOID**)&PasswordEntered );

    if ( EFI_SUCCESS != AuthenticationStatus ) {
        // Report Invalid password and device locked.     
    	MessageBoxTitle = HiiGetString (gHiiHandle, PromptToken);
    	Message = HiiGetString (gHiiHandle, STRING_TOKEN(STR_TCG_STORAGE_ERROR_PSWD));

        mAmiPostMgr->DisplayMsgBox(
                                    MessageBoxTitle,
                                    Message,
                                    MSGBOX_TYPE_OK,
                                    NULL
                                    );
        // Unlock failed.
        EfiLibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                DXE_INVALID_IDE_PASSWORD,
                                0,
                                NULL,
                                NULL );
    }
    // Restore back screen 
    mAmiTseScreenMgmt->RestoreTseScreen();
    
    TcgSecuritySignalProtocolEvent(&gTcgPasswordPromptExitGuid);

    return ;
}

/** @internal
    Send Freeze command all the HDD

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TCGPasswordFreezeDevices (
)
{
    EFI_STATUS            Status;
    EFI_HANDLE            *HandleBuffer;
    UINT16                Index;
    UINTN                 Count;
    AMI_HDD_SECURITY_PROTOCOL *TcgStorageSecurityProtocol = NULL;

    Status = gBS->LocateHandleBuffer(
                                ByProtocol,
                                &gAmiTcgStorageSecurityProtocolGuid,
                                NULL,
                                &Count,
                                &HandleBuffer
                                );

    if ( EFI_ERROR( Status )) {
        return;
    }

    for ( Index = 0; Index < Count; Index++ ) {
        //
        // get password security protocol
        //
        Status = gBS->OpenProtocol(
                                HandleBuffer[Index],
                                &gAmiTcgStorageSecurityProtocolGuid,
                                (VOID**) &TcgStorageSecurityProtocol,
                                NULL,
                                HandleBuffer[Index],
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );

        if ( EFI_ERROR( Status )) {
            continue;
        }

        //
        //Send Freeze lock command
        //
        TcgStorageSecurityProtocol->SecurityFreezeLock( TcgStorageSecurityProtocol );
        
    }// end of for
    MemFreePointer((VOID**)&HandleBuffer );

    return;
}

/** @internal
    Updates the HDD password for the current HDD alone.

    @param UINT32 Index,
    @param CHAR16 *Password,
    @param BOOLEAN bCheckUser

    @retval VOID

    @endinternal
**/
BOOLEAN
TCGPasswordUpdateHdd (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    AMI_HDD_SECURITY_PROTOCOL *TcgStorageSecProtocol = NULL;
    UINT16                Control              = bCheckUser ? 0 : 1;
    UINT8                 RevisionCode         = 0;
    BOOLEAN               Locked;
    EFI_STATUS            Status = EFI_UNSUPPORTED;
    UINT8                 Buffer[OPAL_PASSWORD_LENGTH + 1];
    UINT8                 Selection;
    UINT16                SecurityStatus = 0;
	
    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN                 ii;
    #endif
    TCG_STORAGE_SECURITY_INTERNAL_DATA     *DataPtr;

    DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)TCGPasswordGetDataPtr( Index );

    if ( DataPtr == NULL ) {
        ShowPostMsgBox(
                NULL,
                HiiGetString(
                            gHiiHandle,
                            STRING_TOKEN(STR_TCG_STORAGE_SECURITY_UNSUPPORTED)
                            ),
                MSGBOX_TYPE_OK,
                &Selection
         ); // ShowPostMsgBox

        return FALSE;
    }
    TcgStorageSecProtocol = DataPtr->TcgStorageSecProtocol;
   
    //Get the security status of the device
    Status = TcgStorageSecProtocol->ReturnSecurityStatus( TcgStorageSecProtocol, &SecurityStatus );
    
    if ( EFI_ERROR( Status )) {
	    // Unable to get security status
        return FALSE; 
    }
	
    // Mask security status based on mask data
    Locked  = ((SecurityStatus & SecurityLockedMask) ? TRUE : FALSE );
    
    if ( !Locked ) {
        if ( Password[0] == 0 ) {
            //
            //empty string is entered -> disable password
            //
            Status = TcgStorageSecProtocol->SecurityDisablePassword(
                                            TcgStorageSecProtocol,
                                            Control,
                                            TcgSecurityInternalData[Index].PWD );
        } else {
            //
            //set new password
            //
            gBS->SetMem( &Buffer, OPAL_PASSWORD_LENGTH + 1, 0 );
            #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW

            for ( ii = 0; ii < OPAL_PASSWORD_LENGTH + 1; ii++ ) {
                Buffer[ii] = (UINT8)Password[ii];

                if ( Password[ii] == L'\0' ) {
                    break;
                }
            }// end of for
            #else
            MemCopy( Buffer, Password, OPAL_PASSWORD_LENGTH + 1 );
            #endif

            Status = TcgStorageSecProtocol->SecuritySetPassword(
                                                TcgStorageSecProtocol,
                                                Control,
                                                Buffer,
                                                RevisionCode );
        }
    }// end if(!Locked)

    if ( EFI_ERROR( Status )) {
        return FALSE;
    }

    return TRUE;
}

/** @internal
    Updates the HDD password for all the HDDs present.

    @param UINT32 Index,
    @param CHAR16 *Password,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
BOOLEAN
TCGPasswordUpdateAllHdd (
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    UINTN       Index;
    BOOLEAN     Status = FALSE;
    for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
        Status = TCGPasswordUpdateHdd( (UINT32)Index, Password, bCheckUser);
    }
    return Status;
}

/** @internal
    Hook function to update the password for the HDDs based
    on the token TCG_STORAGE_SEC_ALL_HDD_SAME_PW.

    @param  UINT32 Index,
    @param  CHAR16 *Password,
    @param  BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
BOOLEAN
TCGPasswordUpdate (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    #if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
     return TCGPasswordUpdateAllHdd(Password, bCheckUser);
    #else
     return TCGPasswordUpdateHdd( Index, Password, bCheckUser);
    #endif

}
/** @internal
    Unlock the HDD

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
UnlockOPALDrive (
    VOID
)
{
    #if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN             Size=0, Index;
    AMITSESETUP       *TSESetup;
    TCG_STORAGE_SECURITY_INTERNAL_DATA *DataPtr;
    EFI_STATUS        Status;
    EFI_GUID AmiTseSetupGuid = AMITSESETUP_GUID;

    //
    //Do not proceed if ADMIN password is not set
    //
    if ( !(AMI_PASSWORD_USER & PasswordCheckInstalled()) ) {
        return;
    }

    //
    //Get password from NVRAM
    //
    Size = 0;
    TSESetup = VarGetNvramName (L"AMITSESetup", &AmiTseSetupGuid, NULL, &Size);

    if ( Size ) {
        //
        //For all drives
        //
        DataPtr = TcgSecurityInternalData;

        for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
            if ( DataPtr->Locked ) {
                //
                //ask for the password if locked
                //
                Status = TCGPasswordAuthenticateHdd( TSESetup->UserPassword,
                                                  DataPtr,
                                                  FALSE);

                if ( EFI_SUCCESS != Status ) {
                    //
                    // Unlock failed.
                    //
                    EfiLibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                            DXE_INVALID_IDE_PASSWORD,
                                            0,
                                            NULL,
                                            NULL );
                }
            }
            DataPtr++;
        } // end of for
    } // end if

    MemFreePointer((VOID**) &TSESetup );
    return;
    #endif //#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
}

/** @internal
    Set the HDD password

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TcgStorageSecSetupSameSysPw (
    IN EFI_EVENT  Event,
    IN VOID       *Context
)
{
    #if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN       Size=0, Index;
    AMITSESETUP *TSESetup;
    BOOLEAN     Status;
    EFI_GUID AmiTseSetupGuid = AMITSESETUP_GUID;

    //
    //Get password from NVRAM
    //
    Size = 0;
    TSESetup = VarGetNvramName (L"AMITSESetup", &AmiTseSetupGuid, NULL, &Size);

    if ( Size ) {
        //
        //For all drives
        //
        for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
            Status = TCGPasswordUpdateHdd( (UINT32)Index, TSESetup->UserPassword, FALSE);
            ASSERT_EFI_ERROR (Status);
        }
    }

    MemFreePointer((VOID**) &TSESetup );
    #endif //#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
}

/** @internal
    Validate the HDD password

    @param VOID

    @retval VOID
    
    @endinternal
**/
VOID
TCGPasswordCheck (
)
{
#if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
    EFI_STATUS  Status = EFI_SUCCESS;
#endif
#if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    TCG_STORAGE_SECURITY_INTERNAL_DATA   *DataPtr;
    UINT16              Index;
#endif

    DEBUG((DEBUG_INFO,"\n TCGPasswordCheck "));
    
    // build IDESecurity data
    gOpalSecurityCount = InitTcgSecurityInternalDataPtr( );

    DEBUG((DEBUG_INFO,"\t OPAL device count =  %x", gOpalSecurityCount));
    
    if ( TcgSecurityInternalData == NULL || gOpalSecurityCount == 0
         || OpalFreeze ) {
        return;
    }

#if TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UnlockOPALDrive();
#else
    DataPtr   = TcgSecurityInternalData;
#if !TCG_STORAGE_SEC_ALL_HDD_SAME_PW
    for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
        if ( DataPtr->Locked && (!DataPtr->Validated)) {
            //ask for the password if locked
            DataPtr->Validated = TRUE;
            AMI_CheckTCGPassword( DataPtr->PromptToken, (VOID*)DataPtr );
        }
        DataPtr++;
    }// end of for
#else

    for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
        // Check if Password is validated for a Device, if yes, Use the same Password buffer
        if( DataPtr->Validated && !DataPtr->Locked ) {

            Status = TCGPasswordUpdateAllHddWithValidatedPsw (
                              DataPtr->PWD,
                              DataPtr,
                              DataPtr->LoggedInAsMaster? 0:1 );
            if(!EFI_ERROR(Status)) {
                break;
            }
        }
        DataPtr++;
    }// end of for
    // Ask Password from User
    if( Index == gOpalSecurityCount ) {
        DataPtr = TcgSecurityInternalData;
        if ( DataPtr->Locked && (!DataPtr->Validated)) {
            //ask for the password if locked
            DataPtr->Validated = TRUE;
            AMI_CheckTCGPassword( DataPtr->PromptToken, (VOID*)DataPtr );
        }
    }
#endif
#endif

    return;
}


/** @internal
    Function Unlock the Hdd with Valid Password

    @param VOID

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TCGPasswordUpdateAllHddWithValidatedPsw (
    UINT8   *Password,
    VOID    *Ptr,
    BOOLEAN bCheckUser
)
{
#if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
	TCG_STORAGE_SECURITY_INTERNAL_DATA *DataPtr;
    UINTN         Index;
    EFI_STATUS    Status=EFI_NOT_FOUND;
    CHAR16        PassWordBuffer[OPAL_PASSWORD_LENGTH + 1];

    //For all drives
    DataPtr = TcgSecurityInternalData;

    if(DataPtr == NULL) {
        return EFI_NOT_FOUND;
    }

    for ( Index = 0; Index < OPAL_PASSWORD_LENGTH + 1; Index++ ) {
        PassWordBuffer[Index] = Password[Index];

        if ( Password[Index] == '\0' ) {
            break;
        }
    }

    for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {

        if( DataPtr->Locked && !DataPtr->Validated ) {

            DataPtr->Validated = TRUE;
            Status = TCGPasswordAuthenticateHdd( PassWordBuffer,
                                                 DataPtr,
                                                 bCheckUser );
            if ( EFI_SUCCESS != Status ) {
                // Unlock failed.
                EfiLibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                        DXE_INVALID_IDE_PASSWORD,
                                        0,
                                        NULL,
                                        NULL );
            }            
        }
        DataPtr++;
    }
#endif
    return EFI_SUCCESS;
}

/** @internal
    Validates the password for the current HDD alone.

    @param CHAR16 *Password,
    @param VOID* Ptr,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TCGPasswordAuthenticateHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    UINT16            Control = 0;
    EFI_STATUS        Status;
    UINT8             Buffer[OPAL_PASSWORD_LENGTH + 1];

    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW
    UINTN             Index;
    #endif

    TCG_STORAGE_SECURITY_INTERNAL_DATA * DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)Ptr;

    gBS->SetMem( &Buffer, OPAL_PASSWORD_LENGTH + 1, 0 );

    #if !TCG_STORAGE_SEC_SETUP_SAME_SYS_PW

    for ( Index = 0; Index < OPAL_PASSWORD_LENGTH + 1; Index++ ) {
        Buffer[Index] = (UINT8)Password[Index];

        if ( Password[Index] == L'\0' ) {
            break;
        }
    }
    #else
    gBS->CopyMem( Buffer, Password, OPAL_PASSWORD_LENGTH + 1 );
    #endif

    Control = bCheckUser ? 0 : 1;

    Status = (DataPtr->TcgStorageSecProtocol)->SecurityUnlockPassword(
        DataPtr->TcgStorageSecProtocol,
        Control,
        Buffer );

    if ( EFI_ERROR( Status )) {
        return EFI_ACCESS_DENIED;
    }

    //
    //save password in case we need to disable it during the setup
    //
    gBS->CopyMem( &(DataPtr->PWD), &Buffer, OPAL_PASSWORD_LENGTH + 1 );
    DataPtr->Locked = FALSE;

    if ( !bCheckUser ) {
        DataPtr->LoggedInAsMaster = TRUE;
    }

    return EFI_SUCCESS;
}

/** @internal
    Validates the password for all the HDDs Present.

    @param CHAR16 *Password,
    @param VOID* Ptr,
    @param BOOLEAN bCheckUser

    @retval VOID
    
    @endinternal
**/
EFI_STATUS
TCGPasswordAuthenticateAllHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
	TCG_STORAGE_SECURITY_INTERNAL_DATA *DataPtr;
    UINTN Index;
    EFI_STATUS        Status=EFI_NOT_FOUND;

    //
    //For all drives
    //
    DataPtr = TcgSecurityInternalData;

    if(DataPtr == NULL) {
        return EFI_NOT_FOUND;
    }
    for ( Index = 0; Index < gOpalSecurityCount; Index++ ) {
        Status = TCGPasswordAuthenticateHdd( Password,
                                             DataPtr,
                                             bCheckUser );
        if ( EFI_SUCCESS != Status ) {
            //
            // Unlock failed.
            //
            EfiLibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                    DXE_INVALID_IDE_PASSWORD,
                                    0,
                                    NULL,
                                    NULL );
        }
        DataPtr++;
    }
    return Status;
}

/** @internal
    Hook function to validate Password for the HDDs based on
    the token TCG_STORAGE_SEC_ALL_HDD_SAME_PW.
    
    @param   *Password
    @param   Ptr
    @param   bCheckUser

    @retval VOID
    
    @endinternal

**/
EFI_STATUS
TCGPasswordAuthenticate (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    #if TCG_STORAGE_SEC_ALL_HDD_SAME_PW
     return TCGPasswordAuthenticateAllHdd( Password, Ptr, bCheckUser);
    #else
     return TCGPasswordAuthenticateHdd( Password, Ptr, bCheckUser);
    #endif

}

/** @internal
    Get the internal Data pointer

    @param Index - Index of HDD

    @retval VOID
    
    @endinternal
**/
VOID*
TCGPasswordGetDataPtr (
    UINTN   Index
)
{
	TCG_STORAGE_SECURITY_INTERNAL_DATA *DataPtr;

    if ( gOpalSecurityCount == 0 || TcgSecurityInternalData == NULL ) {
        //
        //try to initialize, if not initialized
        //
        gOpalSecurityCount = InitTcgSecurityInternalDataPtr( );
    }

    if ( gOpalSecurityCount == 0 || TcgSecurityInternalData == NULL || Index >=
         gOpalSecurityCount ) {
        return NULL;
    }

    DataPtr = (TCG_STORAGE_SECURITY_INTERNAL_DATA*)TcgSecurityInternalData;

    if(DataPtr == NULL) {
        return 0;
    }

    return (VOID*)&DataPtr[Index];
}

extern BOOLEAN gBrowserCallbackEnabled;
EFI_GUID       gTcgSecurityLoadSetupDefaultsGuid = AMI_TCG_STORAGE_SEC_LOAD_SETUP_DEFAULTS_GUID;
EFI_EVENT      gTcgSecurityLoadSetupDefaultsEvent = NULL;

VOID
EFIAPI
TcgSecurityLoadSetupDefaultsFunc (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
)
{
    //gBS->CloseEvent(Event);
}

VOID
TcgSecurityLoadSetupDefaults (
  VOID   *defaults,
  UINTN  data 
)
{
    EFI_STATUS          Status;
    if(gTcgSecurityLoadSetupDefaultsEvent == NULL) {
        Status = gBS->CreateEventEx(
                     EVT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     TcgSecurityLoadSetupDefaultsFunc,
                     NULL,
                     &gTcgSecurityLoadSetupDefaultsGuid,
                     &gTcgSecurityLoadSetupDefaultsEvent);
        ASSERT_EFI_ERROR(Status);
    }
    gBrowserCallbackEnabled = TRUE;
    gBS->SignalEvent(gTcgSecurityLoadSetupDefaultsEvent);
    gBrowserCallbackEnabled = FALSE;
}

#if FREEZE_LOCK_OPAL
/** @internal
    Notification function gets called when ReadyToBoot event is triggered.

    @param EFI_EVENT Event - Event to signal
    @param void Context - Event specific context

    @retval VOID
    
    @endinternal
**/
VOID
TcgStorageReadyToBootNotification (
    EFI_EVENT   Event,
    VOID        *Context
)
{
    EFI_STATUS                   Status;
    EFI_PHYSICAL_ADDRESS         NvsMemoryAddress;
    EFI_GUID                     FlagsStatusguid = AMI_TCG_CONFIRMATION_FLAGS_GUID;
    AMI_ASL_PPI_NV_VAR           *TpmAcpiNvsFlags;
    UINTN                        Size = sizeof (EFI_PHYSICAL_ADDRESS);  
    EFI_SMM_CONTROL2_PROTOCOL    *gSmmCtl = NULL;
    void                         *TreeProtocol = NULL;
#ifdef  PPI_OFFSET
     UINT8                       SwSmiValue = PPI_OFFSET;
#else
     UINT8                       SwSmiValue = 0x35;   // Default smi value for PpiOffset 
#endif
    EFI_HANDLE                   Handle=NULL;
    EFI_GUID                     AmiTseTcgFreezeStateNotifyGuid = AMI_TSE_TCG_FREEZE_STATE_NOTIFY_GUID;

    OpalFreeze = TRUE;
    gBS->CloseEvent(Event);

    // Notify setup modules that FreezeLock commands are sent to the Devices during ReadyToBootEvent
    gBS->InstallProtocolInterface( &Handle, 
                                   &AmiTseTcgFreezeStateNotifyGuid, 
                                   EFI_NATIVE_INTERFACE,
                                   NULL );

    Status = gBS->LocateProtocol(&gEfiTrEEProtocolGuid,
                                 NULL,
                                 &TreeProtocol);
    
    if(!EFI_ERROR(Status)) {
        Status = gRT->GetVariable(L"TpmServFlags",
                                  &FlagsStatusguid,
                                  NULL,
                                  &Size,
                                  &NvsMemoryAddress);
    
        if(!EFI_ERROR(Status)) {
            TpmAcpiNvsFlags =  ((AMI_ASL_PPI_NV_VAR *)(UINTN)NvsMemoryAddress);
            TpmAcpiNvsFlags->Flag = ACPI_FUNCTION_GET_USER_CONFIRMATION_STATUS_FOR_REQUEST;
            TpmAcpiNvsFlags->RQST = TCPA_PPIOP_ENABLE_BLOCK_SID_FUNC;
            TpmAcpiNvsFlags->RequestFuncResponse = 0; 
            
            Status = gBS->LocateProtocol(&gEfiSmmControl2ProtocolGuid, NULL, (VOID **)&gSmmCtl);
            ASSERT_EFI_ERROR(Status);
            if (!EFI_ERROR(Status)){
                //Trigger a software SMI with the PPI_OFFSET value, to get the response for the PPI Request
                gSmmCtl->Trigger(gSmmCtl, &SwSmiValue, NULL, FALSE, 0);
            }

            // If RequestFuncResponse is a non zero value, then TCG2 driver will manage the BlockSid command
            if(TpmAcpiNvsFlags->RequestFuncResponse ) {
                return;
            }
        }
    }	
    TCGPasswordFreezeDevices();
    return;
}
#endif
