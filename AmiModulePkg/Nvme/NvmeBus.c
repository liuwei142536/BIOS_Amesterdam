//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file NvmeBus.c
    Provides Nvme Block IO protocol

**/

//---------------------------------------------------------------------------

#include "NvmeIncludes.h"
#include "NvmeBus.h"
#include "NvmePassthru.h"
#include "Protocol/AmiHddSecurityInit.h"
#include "Protocol/AmiHddSecurity.h"
#include "Protocol/AmiHddSmartInit.h"
#include "Protocol/AmiHddSmart.h"
#include "Protocol/LegacyBios.h"
#include <Guid/AmiResetSystemEvent.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/TrEEProtocol.h>
#if DO_NOT_LAUNCH_NVME_OPROM
#include <Protocol/AmiOptionRomPolicy.h>
#endif

//---------------------------------------------------------------------------
extern EFI_COMPONENT_NAME2_PROTOCOL gNvmeBusControllerDriverName;

// Below code will be removed when gAmiTcgStorageSecurityInitProtocolGuid definition is added in AmiModulePkg
#ifndef AMI_TCG_STORAGE_SECURITY_INIT_PROTOCOL_GUID 
#define AMI_TCG_STORAGE_SECURITY_INIT_PROTOCOL_GUID \
        { 0x734aa01d, 0x95ec, 0x45b7, { 0xa2, 0x3a, 0x2d, 0x86, 0xd8, 0xfd, 0xeb, 0xb6 } }
#endif

// Below code will be removed when gAmiTcgStorageSecurityProtocolGuid definition is added in AmiModulePkg
#ifndef AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID 
#define AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID \
        { 0xb396da3a, 0x52b2, 0x4cd6, { 0xa8, 0x9a, 0x13, 0xe7, 0xc4, 0xae, 0x97, 0x90 } }
#endif


#if DO_NOT_LAUNCH_NVME_OPROM
VOID        *NvmePciIoRegistration = NULL;
EFI_EVENT   NvmePciIoEvent = NULL;
#endif

#define BDS_CONNECT_DRIVERS_PROTOCOL_GUID \
    { 0x3aa83745, 0x9454, 0x4f7a, { 0xa7, 0xc0, 0x90, 0xdb, 0xd0, 0x2f, 0xab, 0x8e } }

EFI_GUID gAmiTcgStorageSecurityInitProtocolGuid = AMI_TCG_STORAGE_SECURITY_INIT_PROTOCOL_GUID;
EFI_GUID gAmiTcgStorageSecurityProtocolGuid = AMI_TCG_STORAGE_SECURITY_PROTOCOL_GUID;
EFI_GUID gBdsConnectDriversProtocolGuid = BDS_CONNECT_DRIVERS_PROTOCOL_GUID;

AMI_HDD_SECURITY_INIT_PROTOCOL      *gNvmeHddSecurityInitProtocol = NULL;
AMI_HDD_SECURITY_INIT_PROTOCOL      *gAmiTcgStorageSecurityInitProtocol = NULL;

EFI_EVENT                              NvmeShutdownEvent = NULL;
EFI_EVENT                              NvmeReadyToBootEvent = NULL;
AMI_HDD_SMART_INIT_PROTOCOL            *HddSmartInitProtocol = NULL;
AMI_BLOCKIO_WRITE_PROTECTION_PROTOCOL  *AmiBlkWriteProtection = NULL;

#if NVMe_SMM_SUPPORT
// Global Buffers used for SMM Communicate
UINT8                        *gNvmeCommunicateBuffer = NULL;
UINT8                        *gNvmeIdentifyControllerData = NULL;
UINT8                        *gNvmeActiveNsBlkMedia = NULL;

// Used by NVMe legacy boot support
UINT8       gInt13DeviceAddress = 1;
VOID        *gLegacyNvmeBuffer = NULL;
#endif

UINT8                        gMorValue = 0xFE;

EFI_DRIVER_BINDING_PROTOCOL gNvmeBusDriverBinding = {
    NvmeBusSupported, 
    NvmeBusStart,     
    NvmeBusStop,      
    NVME_BUS_DRIVER_VERSION,  // version
    NULL,                    // ImageHandle
    NULL                     // DriverBindingHandle
};

/**
    Installs gNvmeBusDriverBinding protocol

        
    @param ImageHandle 
    @param SystemTable 

    @retval 
        EFI_STATUS

    @note  
  Here is the control flow of this function:
  1. Initialize Ami Lib.
  2. Install Driver Binding Protocol
  3. Return EFI_SUCCESS.

**/

EFI_STATUS
EFIAPI
NvmeBusEntryPoint (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS            Status;
    EFI_GUID              MorGuid = MEMORY_ONLY_RESET_CONTROL_GUID;
    UINTN                 MorValueSize = sizeof (UINT8);
    EFI_EVENT             ReadyToBootEvent = NULL;
#if NVMe_SMM_SUPPORT
    EFI_EVENT             SwSmiHandlerEvent;
    VOID                  *Registration;
    UINTN                 NvmeCommunicateBufferSize = 0;
    EFI_EVENT             NvmeAllDriverConnectedEvent;
    VOID                  *NvmeAllDriverConnectedRegistration;

    // Buffers Allocated for SMM Communication As new core doesn't allow Buffers which are allocated after EndofDXE.
    // Maximum Size of Buffer allocated
    
    NvmeCommunicateBufferSize  = sizeof(EFI_GUID) + sizeof(UINTN) + sizeof(AMI_NVME_CONTROLLER_PROTOCOL) + \
                                  sizeof(ACTIVE_NAMESPACE_DATA) + sizeof(NVME_CONTROLLER_DATA_TO_SMM);
    
    // Allocate Memory for Communication buffer.   
    Status = gBS->AllocatePool( EfiRuntimeServicesData,
                                NvmeCommunicateBufferSize,
                                (VOID**)&gNvmeCommunicateBuffer );
    if ( EFI_ERROR( Status )) {
        return Status;
    }
    
    // Allocate memory for IdentifyControllerData
    Status = gBS->AllocatePool ( EfiRuntimeServicesData,
                                 sizeof (IDENTIFY_CONTROLLER_DATA),
                                 (VOID**)&gNvmeIdentifyControllerData);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Allocate memory for BlockIo Media
    Status = gBS->AllocatePool (EfiRuntimeServicesData,
                                sizeof (EFI_BLOCK_IO_MEDIA),
                                (VOID**)&gNvmeActiveNsBlkMedia );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // If buffer is not aligned for INT13 read/write, this memory will be used 
    Status = gBS->AllocatePages ( AllocateAnyPages,
                                  EfiReservedMemoryType,
                                  EFI_SIZE_TO_PAGES(512),  
                                  (EFI_PHYSICAL_ADDRESS*)&(gLegacyNvmeBuffer));
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Register a callback function for gBdsConnectDriversProtocolGuid
    // to register the NvmeSwSmiHandler, only when CSM is enabled.
    
    Status = gBS->CreateEvent(
                     EFI_EVENT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     InstallSwSmiHandlerForLegacy,
                     NULL,
                     &SwSmiHandlerEvent
                 );
    ASSERT_EFI_ERROR(Status);

    Status = gBS->RegisterProtocolNotify (&gBdsConnectDriversProtocolGuid,
                                          SwSmiHandlerEvent,
                                          &Registration);
    
    ASSERT_EFI_ERROR(Status);

    Status = gBS->CreateEvent( 
                            EFI_EVENT_NOTIFY_SIGNAL,
                            TPL_CALLBACK,
                            NvmeAllDriverConnectedCallback,
                            NULL,
                            &NvmeAllDriverConnectedEvent );
    ASSERT_EFI_ERROR(Status);
    Status = gBS->RegisterProtocolNotify(
                                &gBdsAllDriversConnectedProtocolGuid,
                                NvmeAllDriverConnectedEvent,
                                &NvmeAllDriverConnectedRegistration);
    ASSERT_EFI_ERROR (Status);
    

#endif
    
    gNvmeBusDriverBinding.DriverBindingHandle=NULL;
    gNvmeBusDriverBinding.ImageHandle=ImageHandle;

    Status = gBS->InstallMultipleProtocolInterfaces(
                    &gNvmeBusDriverBinding.DriverBindingHandle,
                    &gEfiDriverBindingProtocolGuid,&gNvmeBusDriverBinding,
                    &gEfiComponentName2ProtocolGuid, &gNvmeBusControllerDriverName,
                    NULL
                    );
    ASSERT_EFI_ERROR(Status);

    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    NvmeReadyToBootEventCallback,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &ReadyToBootEvent
                    );
    ASSERT_EFI_ERROR (Status);

#if DO_NOT_LAUNCH_NVME_OPROM
    Status = gBS->CreateEvent(
                     EFI_EVENT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     NvmeOpromPolicy,
                     NULL,
                     &NvmePciIoEvent
                     );
    ASSERT_EFI_ERROR(Status);
    if(!EFI_ERROR(Status)) {
        Status = gBS->RegisterProtocolNotify (&gEfiPciIoProtocolGuid,
                                              NvmePciIoEvent,
                                              &NvmePciIoRegistration);
        ASSERT_EFI_ERROR (Status);
    }
#endif
    // Read memory over write request Nvram variable value and store it globally
    gRT->GetVariable(L"MemoryOverwriteRequestControl",
                     &MorGuid,
                     NULL,
                     &MorValueSize,
                     &gMorValue 
                     );
    return Status;
    
}

/**
    Checks whether it is a Nvme controller or not. 
    If 'yes', return SUCCESS else ERROR

    @param This 
    @param Controller 
    @param RemainingDevicePath 

    @retval EFI_STATUS

**/

EFI_STATUS 
EFIAPI
NvmeBusSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
)
{

    EFI_STATUS                  Status;
    EFI_PCI_IO_PROTOCOL         *PciIO;
    PCI_TYPE00                  PciConfig;
    EFI_DEVICE_PATH_PROTOCOL    *ParentDevicePath;
    NVME_DEVICE_PATH                *NvmeRemainingDevicePath = NULL;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController;
    LIST_ENTRY                      *LinkData;
    
    // Check whether device path is valid
    if (RemainingDevicePath != NULL) {
    	
    	// Check if RemainingDevicePath is the End of Device Path Node,
        // if yes, go on checking other conditions
        if (!IsDevicePathEnd (RemainingDevicePath)) {
            // If RemainingDevicePath isn't the End of Device Path Node,
            // check its validation
            NvmeRemainingDevicePath = (NVME_DEVICE_PATH*)RemainingDevicePath;

            if ((NvmeRemainingDevicePath->Header.Type != MESSAGING_DEVICE_PATH) ||
                (NvmeRemainingDevicePath->Header.SubType != MSG_NVME_DP) ||
                (DevicePathNodeLength(NvmeRemainingDevicePath) != sizeof(NVME_DEVICE_PATH))) {
                
                return EFI_UNSUPPORTED;
            }
        } else {
            return EFI_UNSUPPORTED;
        }
            

        // Check if gAmiNvmeControllerProtocolGuid is installed on the controller
        Status = gBS->OpenProtocol( Controller,
                                &gAmiNvmeControllerProtocolGuid,
                                (VOID **)&NvmeController,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);

        if (Status == EFI_SUCCESS ) {
            for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
                    LinkData != &NvmeController->ActiveNameSpaceList; 
                     LinkData = LinkData->ForwardLink) {
    	                     
                ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);

                // Check if it is an already enumerated NameSpace
                if(ActiveNameSpace->ActiveNameSpaceID == NvmeRemainingDevicePath->Nsid ){
                    return EFI_ALREADY_STARTED;
                }
            }
            return EFI_SUCCESS;
        } 
    }
    // Check whether DevicePath Protocol has been installed on this controller
    Status = gBS->OpenProtocol( Controller,
                    &gEfiDevicePathProtocolGuid,
                    (VOID**)&ParentDevicePath,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_BY_DRIVER );

    if (!(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        return Status;
    }

    if(Status == EFI_SUCCESS){
       
        gBS->CloseProtocol (
                    Controller,
                    &gEfiDevicePathProtocolGuid,
                    This->DriverBindingHandle,
                    Controller);
    }

    // Check for Valid SATA Device Path. If no return UNSUPPORTED
    // Check if Controller is Nvme or not?
    Status = gBS->OpenProtocol( Controller,
                        &gEfiPciIoProtocolGuid,
                        (VOID **)&PciIO,
                        This->DriverBindingHandle,     
                        Controller,   
                        EFI_OPEN_PROTOCOL_BY_DRIVER);
    
     if (!(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
         return Status;
     }
     
     if(Status == EFI_SUCCESS){
         
         gBS->CloseProtocol( Controller,
                             &gEfiPciIoProtocolGuid,
                             This->DriverBindingHandle,
                             Controller );
     }

     Status = PciIO->Pci.Read ( PciIO,
                                EfiPciIoWidthUint8,
                                0,
                                sizeof (PCI_TYPE00),
                                &PciConfig
                                );
     
     if (EFI_ERROR(Status)) {
         return EFI_UNSUPPORTED;
     }

     // Check for MASS Storage controller, Non-Volatile and NVMHCI interface
     if (IS_CLASS3 (&PciConfig, 
                 PCI_CLASS_MASS_STORAGE, 
                 PCI_CLASS_MASS_STORAGE_SOLID_STATE, 
                 PCI_IF_MASS_STORAGE_SOLID_STATE_ENTERPRISE_NVMHCI)) {
   
         return EFI_SUCCESS;
     }
     
    return EFI_UNSUPPORTED; 
}

/**
    Installs Nvme Block IO Protocol

        
    @param This 
    @param Controller 
    @param RemainingDevicePath 

    @retval 
        EFI_STATUS

**/

EFI_STATUS 
EFIAPI
NvmeBusStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
)
{

    EFI_STATUS                         Status;
    EFI_STATUS                         NvmeControllerStatus; 
    EFI_PCI_IO_PROTOCOL                *PciIO;
    AMI_NVME_CONTROLLER_PROTOCOL       *NvmeController = NULL;
    IDENTIFY_CONTROLLER_DATA           *IdentifyData = NULL;
    UINT32                             *ActiveNameSpaceIDs = NULL;
    UINT32                             i;
    AMI_NVME_PASS_THRU_PROTOCOL        *AmiNvmePassThru;
    NVM_EXPRESS_PASS_THRU_INSTANCE     *NvmePassthruInstance = NULL;
    EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *EfiNvmExpressPassThru;
    VOID                               *AmiHddSecurityEndProtocol=NULL;
	EFI_DEVICE_PATH_PROTOCOL  		   *PciDevPath;
    ACTIVE_NAMESPACE_DATA              *ActiveNameSpace = NULL;
    LIST_ENTRY                         *LinkData;
	NVME_DEVICE_PATH                   *NvmeRemainingDevicePath = (NVME_DEVICE_PATH*)RemainingDevicePath;
    
    REPORT_STATUS_CODE(EFI_PROGRESS_CODE,EFI_IO_BUS_ATA_ATAPI | EFI_IOB_PC_INIT); 
    
    Status = gBS->OpenProtocol( Controller,
                                &gEfiDevicePathProtocolGuid,
                                (VOID**)&PciDevPath,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_BY_DRIVER );

    if (!(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        return EFI_DEVICE_ERROR;
    }

    // Get the PciIO interface
    Status = gBS->OpenProtocol( Controller,
                        &gEfiPciIoProtocolGuid,
                        (VOID **)&PciIO,
                        This->DriverBindingHandle,     
                        Controller,
                        EFI_OPEN_PROTOCOL_BY_DRIVER);

    if (!(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        return EFI_DEVICE_ERROR;
    } 

    // Check if AmiNvmeController protocol is installed.
    NvmeControllerStatus = gBS->OpenProtocol( Controller,
                                    &gAmiNvmeControllerProtocolGuid,
                                    (VOID **)&NvmeController,
                                    This->DriverBindingHandle,     
                                    Controller,   
                                    EFI_OPEN_PROTOCOL_GET_PROTOCOL);

    if ( !(NvmeControllerStatus == EFI_SUCCESS)) {
         
        DEBUG((DEBUG_INFO,"\nNVMe Driver Detection and Configuration starts\n"));  
        
        // Do Controller Init
        Status = InitializeNvmeController(Controller, This->DriverBindingHandle, &NvmeController);
        if (EFI_ERROR(Status)) {
            goto Error_Path;
        }
    
        // Get the IdentifyData
        Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        sizeof (IDENTIFY_CONTROLLER_DATA),
                        (VOID**)&IdentifyData
                        );
        
        if (EFI_ERROR(Status)) {
            goto Error_Path;
        }
        
        gBS->SetMem(IdentifyData, sizeof(IDENTIFY_CONTROLLER_DATA), 0);
        NvmeController->IdentifyControllerData = IdentifyData;
        
        // Get IdentifyController Data Structure    
        Status = GetIdentifyData (NvmeController, (UINT8 *)IdentifyData, IDENTIFY_CONTROLLER_CNS_VALUE, 0);
        if (EFI_ERROR(Status)) {
            goto Error_Path;
        }
    
        GetNvmeUnicodeStringName(NvmeController);
    
        // Get the list of Active Name space IDs
        Status = gBS->AllocatePages (
                            AllocateAnyPages,
                            EfiRuntimeServicesData,
                            EFI_SIZE_TO_PAGES(4096+4),
                            (EFI_PHYSICAL_ADDRESS*)&(ActiveNameSpaceIDs));
    
        if (EFI_ERROR(Status)) {
            goto Error_Path;
        }
        
        gBS->SetMem(ActiveNameSpaceIDs, EFI_SIZE_TO_PAGES(4096+4) * EFI_PAGE_SIZE , 0);
        
        NvmeController->ActiveNameSpaceIDs = ActiveNameSpaceIDs;
        
        // Check for NVMe spec. version compliance to get Active name space list
        // Controllers that support specification revision 1.1 or later shall support 
        Status = EFI_DEVICE_ERROR;
        if (NvmeController->ControllerVersion >= NVME_SPEC_VS_1_1) {
            Status = GetIdentifyData (NvmeController, (UINT8 *)(ActiveNameSpaceIDs + 1), NAMESPACE_LIST_CNS_VALUE, 0);
        }
        if(EFI_ERROR(Status)) {
            for (i=1; i < IdentifyData->NN + 1; i++ ) {
                ActiveNameSpaceIDs[i] = i;
            }    
        }
        // Create Submission and Completion Queue1
        NvmeController->NVMQueueNumber = 1;
        Status  = CreateAdditionalSubmissionCompletionQueue(NvmeController, 
                                                            NvmeController->NvmeCmdWrapper, 
                                                            NvmeController->NVMQueueNumber, 
                                                            NvmeController->MaxQueueEntrySupported >= QUEUE1_SIZE ? QUEUE1_SIZE : NvmeController->MaxQueueEntrySupported
                                                            );
        if (EFI_ERROR(Status)) {
            goto Error_Path_1;
        }
        
        // Allocate memory for NVM_EXPRESS_PASS_THRU_INSTANCE structure
        Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      sizeof(NVM_EXPRESS_PASS_THRU_INSTANCE),
                      (VOID**)&NvmePassthruInstance
                      );

        if( !EFI_ERROR(Status)) {

            Status = gBS->AllocatePool (
                          EfiBootServicesData,
                          sizeof(EFI_NVM_EXPRESS_PASS_THRU_MODE),
                          (VOID**)&NvmePassthruInstance->EfiNvmExpressPassThru.Mode
                          );
            if(EFI_ERROR(Status)) {
                gBS->FreePool(NvmePassthruInstance);
            }
        }

        if (!EFI_ERROR(Status)) {

            EfiNvmExpressPassThru = (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL*)&(NvmePassthruInstance->EfiNvmExpressPassThru);

            NvmePassthruInstance->NvmeControllerProtocol = NvmeController;
            NvmeController->EfiNvmExpressPassThru = EfiNvmExpressPassThru;
            NvmePassthruInstance->ControllerHandle = Controller;

            EfiNvmExpressPassThru->Mode->Attributes = EFI_NVM_EXPRESS_PASS_THRU_ATTRIBUTES_PHYSICAL |
                                                       EFI_NVM_EXPRESS_PASS_THRU_ATTRIBUTES_CMD_SET_NVM;
            EfiNvmExpressPassThru->Mode->IoAlign = 4; // DWORD aligned
            EfiNvmExpressPassThru->Mode->NvmeVersion = NvmeController->ControllerVersion;
            EfiNvmExpressPassThru->PassThru = NvmePassThru;
            EfiNvmExpressPassThru->GetNextNamespace = GetNextNamespace;
            EfiNvmExpressPassThru->BuildDevicePath = BuildDevicePath;
            EfiNvmExpressPassThru->GetNamespace = GetNamespace;

            Status = gBS->InstallMultipleProtocolInterfaces( &NvmePassthruInstance->ControllerHandle,
                                                             &gEfiNvmExpressPassThruProtocolGuid, 
                                                             &NvmePassthruInstance->EfiNvmExpressPassThru,
                                                             NULL
                                                             );
            ASSERT_EFI_ERROR(Status);
        }
   
        NvmeController->ActiveNameSpaceList.ForwardLink = &(NvmeController->ActiveNameSpaceList);
        NvmeController->ActiveNameSpaceList.BackLink = &(NvmeController->ActiveNameSpaceList);
    }
    
    // Check whether device path is valid
    if (NvmeRemainingDevicePath != NULL) {

        if ((NvmeRemainingDevicePath->Header.Type != MESSAGING_DEVICE_PATH) ||
                   (NvmeRemainingDevicePath->Header.SubType != MSG_NVME_DP) ||
                   (DevicePathNodeLength(NvmeRemainingDevicePath) != sizeof(NVME_DEVICE_PATH))) {
            return EFI_UNSUPPORTED;
        }
        
        // Enumerating the Namespace in remaining device path
        // No error check needed for NameSpace
        EnumerateActiveNameSpace (NvmeController,NvmeRemainingDevicePath->Nsid);
       
    } else {
        // Get Identify NameSpace Data for each Namespace 
        for (i = 1; (i <= 1024) && NvmeController->ActiveNameSpaceIDs[i]; i++) {
        	// No error check needed for NameSpaces
        	EnumerateActiveNameSpace (NvmeController,NvmeController->ActiveNameSpaceIDs[i]);
           
        	// First instance of zero marks the end of valid namespace IDs
            if (!NvmeController->ActiveNameSpaceIDs[i+1]) {
                break;
            }    
        }
    }
    
    // Is there any active name space discovered
    if(IsListEmpty(&NvmeController->ActiveNameSpaceList)) {
        goto    exit_NvmeBusStart;
    }
    
    // If memory overwrite request is set issue Tper reset
    if ((gMorValue & 01)!= 0) {
        Status = IssueLockOnResetCommand(NvmeController);
        DEBUG((DEBUG_BLKIO,"\n NvmeBus IssueLockOnResetCommand Status :%r ", Status));
    }
	// Install BlockIo instance after ActiveNameSpace Enumeration.    
	Status = InstallBlockIoDiskInfo(This, NvmeController);
    if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"\nInstallBlockIoDiskInfo Status %r\n", Status));
        ASSERT_EFI_ERROR(Status);
    }
	
    // Locate HddSmartInitProtocol to provide Nvme HddSmart support
    if ( HddSmartInitProtocol == NULL ) {
        gBS->LocateProtocol (&gAmiHddSmartInitProtocolGuid,
                             NULL,
                             (VOID **)&HddSmartInitProtocol);
    }
    
	 
    // Skip the driver Installing Smart Interface each time when it is called with remaining device path
    if( HddSmartInitProtocol != NULL && NvmeControllerStatus != EFI_SUCCESS) {
               
        // Routine will check SmartHealth status of Nvme device
        HddSmartInitProtocol->InitSmartSupport( NvmeController, AmiStorageInterfaceNvme );

        if(HddSmartInitProtocol->SmartDiagonasticFlag) {
            HddSmartInitProtocol->InstallSmartInterface( NvmeController, AmiStorageInterfaceNvme );
        }
    }

    Status = EFI_UNSUPPORTED;

    // Install HDDSecurity protocol for NVMe device that supports ATA security Protocol
    if(gNvmeHddSecurityInitProtocol == NULL) {
        
        gBS->LocateProtocol( &gAmiHddSecurityInitProtocolGuid,
                             NULL,
                             (VOID **) &gNvmeHddSecurityInitProtocol);
    }
    
	// Skip the driver Installing Hdd security protocol each time when it is called with remaining device path
    if( gNvmeHddSecurityInitProtocol != NULL && NvmeControllerStatus != EFI_SUCCESS) {
        Status = gNvmeHddSecurityInitProtocol->InstallSecurityInterface(NvmeController, AmiStorageInterfaceNvme );
    }
    // Install TcgStorageSecurity protocol for NVMe device that supports TCG security Protocol
    if (EFI_ERROR(Status)) {
        if(gAmiTcgStorageSecurityInitProtocol == NULL) {
            gBS->LocateProtocol( &gAmiTcgStorageSecurityInitProtocolGuid,
                                 NULL,
                                 (VOID **) &gAmiTcgStorageSecurityInitProtocol);
        }
        
        if( gAmiTcgStorageSecurityInitProtocol != NULL && NvmeControllerStatus != EFI_SUCCESS ) {
            Status = gAmiTcgStorageSecurityInitProtocol->InstallSecurityInterface(NvmeController, AmiStorageInterfaceNvme );
        }
    }

    // If No Security protocol installed then send freeze lock command to the device at ReadyToBoot event.
    if (EFI_ERROR(Status)) {
        if(NvmeReadyToBootEvent == NULL && NvmeControllerStatus != EFI_SUCCESS ) {
            gBS->CreateEventEx(
                EVT_NOTIFY_SIGNAL, TPL_CALLBACK, 
                NvmeFreezeLockDevice,
                NULL,
                &gEfiEventReadyToBootGuid,
                &NvmeReadyToBootEvent);
        }
    }

    if(NvmeControllerStatus != EFI_SUCCESS) {
		Status = gBS->HandleProtocol( Controller, 
                                      &gAmiHddSecurityEndProtocolGuid, 
                                      &AmiHddSecurityEndProtocol
                                      );
    
        if(!EFI_ERROR(Status)) {
            // Protocol already installed on the Controller handle.
            // UnInstall and Install back the protocol interface to Notify the Password verification 
            Status = gBS->UninstallProtocolInterface(
                                        Controller, 
                                        &gAmiHddSecurityEndProtocolGuid, 
                                        NULL
                                        );
            ASSERT_EFI_ERROR(Status);
        }
    
        // This will notify AMITSE to invoke the HDD password Screen
        Status = gBS->InstallProtocolInterface(
                                        &Controller, 
                                        &gAmiHddSecurityEndProtocolGuid, 
                                        EFI_NATIVE_INTERFACE,
                                        NULL
                                        );
        ASSERT_EFI_ERROR(Status);
    }
    
    // Locate AmiBlockIoWriteProtectionProtocol to provide BootSectorWriteProtection support
    if (AmiBlkWriteProtection == NULL && NvmeControllerStatus != EFI_SUCCESS) {
        gBS->LocateProtocol( &gAmiBlockIoWriteProtectionProtocolGuid, 
                             NULL, 
                             (VOID**)&AmiBlkWriteProtection ); 
    }


    // Install NvmePassThru protocol
    Status = gBS->OpenProtocol( 
                  Controller,
                  &gAmiNvmePassThruProtocolGuid,
                  (VOID **)&AmiNvmePassThru,
                  This->DriverBindingHandle,     
                  Controller,   
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
    
    if (EFI_ERROR(Status)) {
        Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      sizeof(AMI_NVME_PASS_THRU_PROTOCOL),
                      (VOID**)&AmiNvmePassThru
                      );
        if (!EFI_ERROR(Status)) {
            AmiNvmePassThru->ExecuteNvmeCmd = ExecuteNvmeCmd;
            gBS->InstallMultipleProtocolInterfaces(
                        &Controller,
                        &gAmiNvmePassThruProtocolGuid, 
                        AmiNvmePassThru,
                        NULL
                        );
        }
    }

    if(NvmeShutdownEvent == NULL) {
        Status = gBS->CreateEventEx(
                         EVT_NOTIFY_SIGNAL,
                         TPL_CALLBACK,
                         NvmeShutdown,
                         NULL,
                         &gAmiResetSystemEventGuid,
                         &NvmeShutdownEvent);
    }
    
exit_NvmeBusStart:
    DEBUG((DEBUG_INFO,"\nNVMe Driver Detection and Configuration Ends with Status =  EFI_SUCCESS\n"));  
    
    return EFI_SUCCESS;
    
Error_Path_1:

    LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
    while ( LinkData != &NvmeController->ActiveNameSpaceList ) {
        ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
        
        // Uninstalls the Protocols and frees the allocated memory by all Nvme device 
        UninstallNvmeDeviceProtocols( This,
                                      ActiveNameSpace->NvmeDeviceHandle,
                                      NvmeController,
                                      ActiveNameSpace,
                                      &LinkData
                                      );        
    }
    
Error_Path:

    // Uninstalls the Protocols and frees the allocated memory by  Nvme Controller 
    if (NvmeController) {
        UninstallNvmeControllerProtocols (This,Controller,NvmeController);
    }

    DEBUG((EFI_D_ERROR,"\nNVMe Driver Detection and Configuration Ends with Status %r\n", Status));
    return Status;
}

/**
    Uninstall all devices installed in start procedure.

    @param This 
    @param Controller 
    @param NumberOfChildren 
    @param ChildHandleBuffer 

    @retval EFI_STATUS

**/

EFI_STATUS 
EFIAPI
NvmeBusStop (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  UINTN                       NumberOfChildren,
    IN  EFI_HANDLE                  *ChildHandleBuffer
 )
{

    EFI_STATUS                         Status;
    AMI_NVME_CONTROLLER_PROTOCOL       *NvmeController = NULL;
    ACTIVE_NAMESPACE_DATA              *ActiveNameSpace;
    UINTN                              Index = 0;
    EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
    NVME_DEVICE_PATH                   *NvmeDevicePath;
    LIST_ENTRY                         *LinkData;

    NVM_EXPRESS_PASS_THRU_INSTANCE     *NvmePassthruInstance = NULL;


    // Check if gAmiNvmeControllerProtocolGuid is installed on the device
    Status = gBS->OpenProtocol( Controller,
                        &gAmiNvmeControllerProtocolGuid,
                        (VOID **)&NvmeController,
                        This->DriverBindingHandle,
                        Controller,
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    
    if (EFI_ERROR(Status) && (NvmeController == NULL)) {
        return EFI_DEVICE_ERROR;
    }
    
    //
    // Check if ChildHandleBuffer is valid
    //
    if (NumberOfChildren) {
        while (NumberOfChildren) {
            // Does the child handle have the correct devicepath
            Status = gBS->OpenProtocol(
                            ChildHandleBuffer[Index],
                            &gEfiDevicePathProtocolGuid,
                            (VOID **)&DevicePath,
                            This->DriverBindingHandle,     
                            Controller,
                            EFI_OPEN_PROTOCOL_GET_PROTOCOL);
            
            if(!EFI_ERROR(Status) && (DevicePath != NULL)) {
                // Locate Nvme devicepath
                NvmeDevicePath = (NVME_DEVICE_PATH *) DevicePath;
                
                while (!IsDevicePathEnd ((VOID *)NvmeDevicePath)) {
                    
                    NvmeDevicePath = (NVME_DEVICE_PATH *)NextDevicePathNode (NvmeDevicePath);
                        if (NvmeDevicePath->Header.Type == MESSAGING_DEVICE_PATH && \
                        NvmeDevicePath->Header.SubType == MSG_NVME_DP){
    
                        for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; \
                             LinkData != &NvmeController->ActiveNameSpaceList; \
                             LinkData = LinkData->ForwardLink) {
                            
                            ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
    
                            // If ActiveNameSpace matches with devicepath, then uninstall the protocols
                            // and break the loop
                            if (ActiveNameSpace->IdentifyNamespaceData.EUI64 == NvmeDevicePath->EUI64 && \
                                ActiveNameSpace->ActiveNameSpaceID == NvmeDevicePath->Nsid ) {
    
                                Status = gBS->CloseProtocol ( 
                                               Controller,
                                               &gEfiPciIoProtocolGuid,
                                               This->DriverBindingHandle,
                                               ChildHandleBuffer[Index]);
                                
                                ASSERT_EFI_ERROR(Status);
                                if (EFI_ERROR(Status)) {
                                    break;
                                } 
    
                                // Uninstalls the Protocols and frees the allocated memory by all Nvme device 
                                Status = UninstallNvmeDeviceProtocols(
                                                              This,
                                                              ChildHandleBuffer[Index],
                                                              NvmeController,
                                                              ActiveNameSpace,
                                                              &LinkData
                                                              );
                                ASSERT_EFI_ERROR(Status);
                                break;
                            }
                        } // End of for loop
                        break;
                    }
                    NvmeDevicePath = (NVME_DEVICE_PATH *)NextDevicePathNode (NvmeDevicePath);
                } // End of while loop
            }
            Index++;
            NumberOfChildren --;
        }
    } else {
          // Check if all Active name space has been stopped
          if(!IsListEmpty(&NvmeController->ActiveNameSpaceList)) {
              return EFI_DEVICE_ERROR;
          } 
          
          // Uninstalls the Protocols and frees the allocated memory from Nvme Controller 
          UninstallNvmeControllerProtocols ( This, Controller, NvmeController);
    }
    
    return EFI_SUCCESS;
}

/** 
    This function uninstalls all protocols and frees the memory related to Nvme controller.
 
    @param This Pointer to the driver binding protocol
    @param Controller Handle of the controller to check if able to be managed by Nvme controller
    @param NvmeController Pointer to the Ami Nvme Controller protocol
    
    @retval NULL
**/

VOID
UninstallNvmeControllerProtocols (
    IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
    IN  EFI_HANDLE                    Controller,
    IN  AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController
)
{
    AMI_NVME_PASS_THRU_PROTOCOL        *AmiNvmePassThru;
    EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmExpressPassthruProtocol;
    NVM_EXPRESS_PASS_THRU_INSTANCE     *NvmePassthruInstance = NULL;
    EFI_STATUS                          Status;
    EFI_PCI_IO_PROTOCOL                 *PciIo;
    UINT64                              PciAttributes;
    UINT32                             Delay;
	
    // Before un-installing HDD security check whether it is installed or not
    Status = gBS->OpenProtocol( Controller,
                                &gAmiHddSecurityProtocolGuid,
                                NULL,
                                This->DriverBindingHandle,
                                Controller, 
                                EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                               );

    if ( Status == EFI_SUCCESS ) {
        if( gNvmeHddSecurityInitProtocol != NULL) {         
            gNvmeHddSecurityInitProtocol->StopSecurityModeSupport( 
                                            NvmeController,  
                                            AmiStorageInterfaceNvme
                                            );
        }
    }

    // Before un-installing TCGStoragesecurity check whether it is installed or not
    Status = gBS->OpenProtocol( Controller,
                                &gAmiTcgStorageSecurityProtocolGuid,
                                NULL,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_TEST_PROTOCOL);

    if ( Status == EFI_SUCCESS ) {
        if( gAmiTcgStorageSecurityInitProtocol != NULL) {
            gAmiTcgStorageSecurityInitProtocol->StopSecurityModeSupport( 
                                                  NvmeController, 
                                                  AmiStorageInterfaceNvme
                                                  );
        }
    }

    // Before un-installing HDDSmart check whether it is installed or not
    Status = gBS->OpenProtocol( Controller,
                                &gAmiHddSmartProtocolGuid,
                                NULL,
                                This->DriverBindingHandle,
                                Controller, 
                                EFI_OPEN_PROTOCOL_TEST_PROTOCOL);

    if (Status == EFI_SUCCESS) {
        if( HddSmartInitProtocol != NULL && HddSmartInitProtocol->SmartDiagonasticFlag ) {
            HddSmartInitProtocol->UnInstallSmartInterface( 
                                     NvmeController, 
                                     AmiStorageInterfaceNvme
                                     );
        }
    }
  
    // Before un-installing AmiNvmeController check whether it is installed or not
    Status = gBS->OpenProtocol( 
                         Controller,
                         &gAmiNvmeControllerProtocolGuid,
                         NULL,
                         This->DriverBindingHandle,
                         Controller,
                         EFI_OPEN_PROTOCOL_TEST_PROTOCOL);
    if (!EFI_ERROR(Status)) {
        
        // No child is active. 
        // Stop the controller. This will delete all the Queues.
          // No child is active. 
          // Stop the controller. This will delete all the Queues.
          // Check if the controller is already running. If yes stop it.
          Delay = NvmeController->TimeOut * 500;
          if (CONTROLLER_REG32(NvmeController, Offset_CC) & BIT0) {
              //  Disable Enable bit
              CONTROLLER_REG32_AND (NvmeController, Offset_CC, ~BIT0);
              do {
                  if (!(CONTROLLER_REG32(NvmeController, Offset_CSTS) & BIT0)) {
                      break;
                  }
                  gBS->Stall(1000); // 1msec delay
              }while (--Delay);
          }
          
          if (!Delay) {
              ASSERT (FALSE);
          }
          
        Status = gBS->UninstallMultipleProtocolInterfaces (
                Controller,
                &gAmiNvmeControllerProtocolGuid,
                NvmeController,
                NULL);
        ASSERT_EFI_ERROR(Status);
    }
    
    Status = gBS->OpenProtocol( Controller,
                                &gAmiNvmePassThruProtocolGuid,
                                (VOID **)&AmiNvmePassThru,
                                This->DriverBindingHandle,     
                                Controller,   
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);
               
    if (!EFI_ERROR(Status)) {
        Status = gBS->UninstallMultipleProtocolInterfaces(
                                 Controller,
                                 &gAmiNvmePassThruProtocolGuid, 
                                 AmiNvmePassThru,
                                 NULL
                                 );

        ASSERT_EFI_ERROR(Status);
    }
    
    Status = gBS->OpenProtocol( Controller,
                                &gEfiNvmExpressPassThruProtocolGuid,
                                (VOID **)&NvmExpressPassthruProtocol,
                                This->DriverBindingHandle,
                                Controller,   
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);
               
    if (!EFI_ERROR(Status)) {
        
        Status = gBS->UninstallMultipleProtocolInterfaces(
                                 Controller,
                                 &gEfiNvmExpressPassThruProtocolGuid, 
                                 NvmExpressPassthruProtocol,
                                 NULL
                                 );

        ASSERT_EFI_ERROR(Status);

        NvmePassthruInstance = BASE_CR( NvmExpressPassthruProtocol ,
                                        NVM_EXPRESS_PASS_THRU_INSTANCE, 
                                        EfiNvmExpressPassThru);
        gBS->FreePool (NvmePassthruInstance->EfiNvmExpressPassThru.Mode);
        gBS->FreePool (NvmePassthruInstance);
    }

    Status = gBS->OpenProtocol ( Controller,
                                 &gEfiPciIoProtocolGuid,
                                 (VOID**)&PciIo,
                                 This->DriverBindingHandle,
                                 Controller,
                                 EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );
    ASSERT_EFI_ERROR(Status);

    if (!EFI_ERROR(Status)) {
             
        Status = PciIo->Attributes (
                                 PciIo,
                                 EfiPciIoAttributeOperationSupported,
                                 0,
                                 &PciAttributes
                                );
        
        ASSERT_EFI_ERROR(Status);    

        // Shutdown the hardware
        PciIo->Attributes (
                                PciIo,
                                EfiPciIoAttributeOperationDisable,
                                PciAttributes & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE),
                                NULL
                                );
    }
    
    // Uninstall and close the protocols
    Status = gBS->CloseProtocol ( 
                            Controller,
                            &gEfiPciIoProtocolGuid,
                            This->DriverBindingHandle,
                            Controller);
           
    ASSERT_EFI_ERROR(Status);
             
    Status = gBS->CloseProtocol (
                          Controller,
                          &gEfiDevicePathProtocolGuid,
                          This->DriverBindingHandle,
                          Controller);
             
    ASSERT_EFI_ERROR(Status);

    // Free up all the memory allocated
    if (NvmeController->IdentifyControllerData) { 
        gBS->FreePool (NvmeController->IdentifyControllerData);
    }

    if (NvmeController->ActiveNameSpaceIDs) { 
        gBS->FreePages ((EFI_PHYSICAL_ADDRESS)NvmeController->ActiveNameSpaceIDs, 
                         EFI_SIZE_TO_PAGES(4096 + 4)
                       );
    }

    if (NvmeController->AdminSubmissionQueueUnMap) { 
        NvmeController->PciIO->Unmap (NvmeController->PciIO, 
                NvmeController->AdminSubmissionQueueUnMap
        );
    }
    if (NvmeController->AdminSubmissionUnAligned) { 
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
             EFI_SIZE_TO_PAGES(NvmeController->AdminSubmissionUnAlignedSize), \
             (VOID *)NvmeController->AdminSubmissionUnAligned
        );
    }

    if (NvmeController->AdminCompletionQueueUnMap) {
                NvmeController->PciIO->Unmap (NvmeController->PciIO, 
                NvmeController->AdminCompletionQueueUnMap
        );
    }

    if (NvmeController->AdminCompletionUnAligned) { 
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
                  EFI_SIZE_TO_PAGES(NvmeController->AdminCompletionUnAlignedSize), \
                  (VOID *)NvmeController->AdminCompletionUnAligned
        );
    }

    if (NvmeController->Queue1SubmissionQueueUnMap) { 
        NvmeController->PciIO->Unmap (NvmeController->PciIO, 
                NvmeController->Queue1SubmissionQueueUnMap
        );
    }
             
    if (NvmeController->Queue1SubmissionUnAligned) { 
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
                EFI_SIZE_TO_PAGES(NvmeController->Queue1SubmissionUnAlignedSize), \
                (VOID *)NvmeController->Queue1SubmissionUnAligned
        );
    }
             
    if (NvmeController->Queue1CompletionQueueUnMap) {  
        NvmeController->PciIO->Unmap (NvmeController->PciIO, 
                NvmeController->Queue1CompletionQueueUnMap
        ); 
    }
    if (NvmeController->Queue1CompletionUnAligned) { 
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
                EFI_SIZE_TO_PAGES(NvmeController->Queue1CompletionUnAlignedSize), \
                (VOID *)NvmeController->Queue1CompletionUnAligned
        );
    }
             
    if (NvmeController->NvmeCmdWrapper) { 
        gBS->FreePool (NvmeController->NvmeCmdWrapper); 
    }
             
    if (NvmeController->UDeviceName) {
        gBS->FreePool (NvmeController->UDeviceName->Language);
        gBS->FreePool (NvmeController->UDeviceName->UnicodeString);
        gBS->FreePool (NvmeController->UDeviceName);
    }
             
     gBS->FreePool (NvmeController);
             
    if(NvmeShutdownEvent != NULL) {
        gBS->CloseEvent(NvmeShutdownEvent);
        NvmeShutdownEvent = NULL;              
    }         
}

/** 
    This function uninstalls all protocols and frees the memory of related to Nvme Device.
 
    @param This Pointer to the driver binding protocol
    @param NvmeDeviceHandle Handle of the controller to check if able to be managed by Nvme Device
    @param NvmeController Pointer to the Ami Nvme Controller protocol
    @param ActiveNameSpace Pointer to the Active Name Space Data
    @param LinkData Pointer to the list entry
    
    @retval EFI_STATUS Status of the operation
**/

EFI_STATUS
UninstallNvmeDeviceProtocols (
    IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
    IN  EFI_HANDLE                    NvmeDeviceHandle,
    IN  AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController,
    IN  ACTIVE_NAMESPACE_DATA         *ActiveNameSpace,
    IN  LIST_ENTRY                    **LinkData
) 
{
    EFI_STATUS                          Status;

    // Check if BlockIO is installed
    Status = gBS->OpenProtocol( NvmeDeviceHandle,
                &gEfiBlockIoProtocolGuid,
                NULL,
                This->DriverBindingHandle,     
                NvmeDeviceHandle,   
                EFI_OPEN_PROTOCOL_TEST_PROTOCOL);
    
    if (Status == EFI_SUCCESS) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                    NvmeDeviceHandle,
                    &gEfiBlockIoProtocolGuid,
                    &(ActiveNameSpace->NvmeBlockIO),
                    NULL);
        
        ASSERT_EFI_ERROR(Status);
    }
    
    // Check if Storage Security protocol is installed
    Status = gBS->OpenProtocol( NvmeDeviceHandle,
                &gEfiStorageSecurityCommandProtocolGuid,
                NULL,
                This->DriverBindingHandle,     
                NvmeDeviceHandle,   
                EFI_OPEN_PROTOCOL_TEST_PROTOCOL);
    
    if (Status == EFI_SUCCESS) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                    NvmeDeviceHandle,
                    &gEfiStorageSecurityCommandProtocolGuid,
                    &(ActiveNameSpace->NvmeSecurityStorageCmd),
                    NULL);
        
        ASSERT_EFI_ERROR(Status);
    }
    
    // Check if DiskInfo is installed
    Status = gBS->OpenProtocol( NvmeDeviceHandle,
                    &gEfiDiskInfoProtocolGuid,
                    NULL,
                    This->DriverBindingHandle,
                    NvmeDeviceHandle,
                    EFI_OPEN_PROTOCOL_TEST_PROTOCOL );
    
    if (Status == EFI_SUCCESS) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                        NvmeDeviceHandle,
                        &gEfiDiskInfoProtocolGuid,
                        &(ActiveNameSpace->NvmeDiskInfo),
                        NULL);
        ASSERT_EFI_ERROR(Status);
    }
    
    // Check if DevicePath is installed
    Status = gBS->OpenProtocol( NvmeDeviceHandle,
                     &gEfiDevicePathProtocolGuid,
                     NULL,
                     This->DriverBindingHandle,
                     NvmeDeviceHandle,
                     EFI_OPEN_PROTOCOL_TEST_PROTOCOL );
    if (Status == EFI_SUCCESS) {
        Status = gBS->UninstallMultipleProtocolInterfaces (
                    NvmeDeviceHandle,
                    &gEfiDevicePathProtocolGuid,
                    ActiveNameSpace->EfiDevicePath,
                    NULL);
        
        // Status of UninstallMultipleProtocolInterfaces should be handled
        ASSERT_EFI_ERROR(Status);

    }
	
    // Free up all the memory 
    if (ActiveNameSpace->PRP2ListUnMap) {
        NvmeController->PciIO->Unmap( NvmeController->PciIO, 
                                      ActiveNameSpace->PRP2ListUnMap
                                     );
    }
        
    if (ActiveNameSpace->PRP2List) { 
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO,
                                  EFI_SIZE_TO_PAGES(NvmeController->MemoryPageSize),\
                                  ActiveNameSpace->PRP2List
                                  );
    }
       
    if(ActiveNameSpace->NvmeBlockIO.Media != NULL) {
        gBS->FreePool (ActiveNameSpace->NvmeBlockIO.Media);
    }

    if (ActiveNameSpace->EfiDevicePath) {
        gBS->FreePool (ActiveNameSpace->EfiDevicePath);
    }

    if (ActiveNameSpace->MetaDataBlock) {
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
                EFI_SIZE_TO_PAGES((UINTN)ActiveNameSpace->MetaDataBufferSize), \
                (VOID *)ActiveNameSpace->MetaDataBlock
        );
    }

    if (ActiveNameSpace->ExtendedLBA) {
        NvmeController->PciIO->FreeBuffer (NvmeController->PciIO, \
                EFI_SIZE_TO_PAGES((UINTN)ActiveNameSpace->ExtendedLBABufferSize), \
                (VOID *)ActiveNameSpace->ExtendedLBA
        );
    }

    *LinkData = RemoveEntryList (*LinkData);
    Status = gBS->FreePool (ActiveNameSpace);
    return Status;
}

/**
    Initialize Nvme controller and ADMIN submission and Completion 
    queues

        
    @param  Controller 
    @param  DriverBindingHandle 
    @param  NvmeControllerReturnAddress 

    @retval EFI_STATUS

**/

EFI_STATUS
InitializeNvmeController (
    IN  EFI_HANDLE                      Controller,
    IN  EFI_HANDLE                      DriverBindingHandle,
    OUT AMI_NVME_CONTROLLER_PROTOCOL    **NvmeControllerReturnAddress
)
{
    
    EFI_STATUS                      Status;
    EFI_PCI_IO_PROTOCOL             *PciIO;
    PCI_TYPE00                      PciConfig;
    UINT64                          ControllerCapabilities;
    UINT32                          Delay;
    UINT32                          ProgramCC = 0;
    EFI_PHYSICAL_ADDRESS            AdminQueueAddress = 0;
    UINTN                           AllocatePageSize;
    UINTN                           AdminQueueSize;
    UINTN                           OrgAdminQueueSize;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController = NULL;
    UINT64                          PciAttributes;
    NVME_DXE_SMM_DATA_AREA          *NvmeDxeSmmDataArea = 0;
    UINT32                          MpsValue = 0;

    Status = gBS->AllocatePool (EfiBootServicesData,
                    sizeof(AMI_NVME_CONTROLLER_PROTOCOL),
                    (VOID**)&NvmeController
                    );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    *NvmeControllerReturnAddress = NvmeController;
    
    // Clear memory
    gBS->SetMem(NvmeController, sizeof(AMI_NVME_CONTROLLER_PROTOCOL), 0);

    Status = gBS->AllocatePool ( EfiReservedMemoryType,
                                 sizeof(NVME_DXE_SMM_DATA_AREA),
                                 (VOID**)&NvmeDxeSmmDataArea
                                 );

    if ( EFI_ERROR(Status) ) {
        return Status;
    }

    // Clear memory
    gBS->SetMem( NvmeDxeSmmDataArea, sizeof(NVME_DXE_SMM_DATA_AREA), 0 );
    NvmeController->NvmeDxeSmmDataArea = NvmeDxeSmmDataArea;
    
    InitializeListHead (&NvmeController->ActiveNameSpaceList);
    
    // Get the PciIO interface
    Status = gBS->OpenProtocol( Controller,
                        &gEfiPciIoProtocolGuid,
                        (VOID **)&PciIO,
                        DriverBindingHandle,     
                        Controller,   
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // Enable PCI Command register
    Status = PciIO->Attributes(PciIO, EfiPciIoAttributeOperationSupported, 0, &PciAttributes);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    Status = PciIO->Attributes(PciIO, 
                                EfiPciIoAttributeOperationEnable, 
                                PciAttributes & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE), 
                                NULL
                                );
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    Status = PciIO->Pci.Read ( PciIO,
                               EfiPciIoWidthUint8,
                               0,
                               sizeof (PCI_TYPE00),
                               &PciConfig
                               );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }    
    NvmeController->Signature = NVME_CONTROLLER_SIG;
    NvmeController->NvmeBarOffset = LShiftU64(PciConfig.Device.Bar[1], 32) + (PciConfig.Device.Bar[0] & ~(0x07));
    NvmeController->PciIO = PciIO;
    NvmeController->ControllerHandle = Controller;
    
    ControllerCapabilities = LShiftU64(CONTROLLER_REG32(NvmeController, 4), 32) + 
                                        CONTROLLER_REG32(NvmeController, 0);
    
    // Initialize various capability values
    
    NvmeController->RawControllerCapabilities = ControllerCapabilities;
    NvmeController->MaxQueueEntrySupported = (ControllerCapabilities & 0xFFFF) + 1;
    NvmeController->ContiguousQueueRequired = (BOOLEAN)(RShiftU64((ControllerCapabilities & 0x10000), 16));
    NvmeController->ArbitrationMechanismSupport = (BOOLEAN)(RShiftU64((ControllerCapabilities & 0x60000), 17));
    NvmeController->TimeOut = (UINT8)RShiftU64((ControllerCapabilities & 0xFF000000), 24);  // 500msec units
    NvmeController->DoorBellStride = (UINT8)(RShiftU64((ControllerCapabilities & 0xF00000000), 32));
    NvmeController->NVMResetSupport = (BOOLEAN) (RShiftU64((ControllerCapabilities & 0x1000000000), 36));
    NvmeController->CmdSetsSupported = (UINT8)(RShiftU64((ControllerCapabilities & 0x1FE000000000), 37));
    NvmeController->MemoryPageSizeMin = (UINT32) LShiftU64(1, (UINTN)(RShiftU64((ControllerCapabilities & 0xF000000000000), 48) + 12));  // In Bytes
    NvmeController->MemoryPageSizeMax = (UINT32) LShiftU64(1, (UINTN)(RShiftU64((ControllerCapabilities & 0xF0000000000000), 52) + 12)); // In Bytes  
    NvmeController->ControllerVersion = CONTROLLER_REG32(NvmeController, Offset_Version);
	
    PrintNvmeCapability(NvmeController);
    
    // Is NVM command set supported
    if (!(NvmeController->CmdSetsSupported & 0x1)) {
        return EFI_UNSUPPORTED;
    }
    
    // Check if the controller is already running. If yes stop it.
    if (CONTROLLER_REG32(NvmeController, Offset_CC) & BIT0) {
        //  Disable Enable bit
        CONTROLLER_REG32_AND (NvmeController, Offset_CC, ~BIT0);
    }
    
    // If controller is not running, check CSTS.RDY cleared or not
    Delay = NvmeController->TimeOut * 500;
    if (!(CONTROLLER_REG32(NvmeController, Offset_CC) & BIT0)) {
        do {
            if (!(CONTROLLER_REG32(NvmeController, Offset_CSTS) & BIT0)) {
                break;
            }
            gBS->Stall(1000); // 1msec delay
        }while (--Delay);
    }
    
    if (!Delay) {
        return EFI_DEVICE_ERROR;
    }
    
    // Initialize Controller configuration register. 
    // Select Round Robin and NVM Command Set (both values are zero)
    // Max Page Size
    // NvmeController->MemoryPageSize = NvmeController->MemoryPageSizeMin;
    // ProgramCC |= (UINT32) LShiftU64((UINTN)(RShiftU64((ControllerCapabilities & 0xF000000000000), 48)), 7);
    NvmeController->MemoryPageSize = (NvmeController->MemoryPageSizeMax > MAX_MPS_ALLOWED) ? \
                                        MAX_MPS_ALLOWED : NvmeController->MemoryPageSizeMax;

    // Program the MPS in CC register as per the memory page size chosen. 
    // The memory page size is (2 ^ (12 + MpsValue)).
    for( MpsValue = 1 ; (NvmeController->MemoryPageSize >> MpsValue) > 1 ; MpsValue++);
    MpsValue -= 12; 
    ProgramCC |= (UINT32) (MpsValue << 7); 
	
    // Initialize with default value. Later it can be modified
    ProgramCC |= (6 << 16);     // I/O Submission Queue Entry Size
    ProgramCC |= (4 << 20);     // I/O Completion Queue Entry Size
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset_CC, ProgramCC);

    // Allocate memory for Admin Queue. Each entry is 64bytes long and queue should be aligned on MemoryPageSize
    OrgAdminQueueSize = AdminQueueSize = (NvmeController->MaxQueueEntrySupported >= ADMIN_QUEUE_SIZE ? 
                                              ADMIN_QUEUE_SIZE : NvmeController->MaxQueueEntrySupported) * sizeof (NVME_ADMIN_COMMAND);

    AllocatePageSize = NvmeController->MemoryPageSize + AdminQueueSize; 
    
    DEBUG ((DEBUG_BLKIO, "Value programmed in Control register     : %08X\n", ProgramCC));
    Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                             AllocateAnyPages,
                                             EfiRuntimeServicesData,
                                             EFI_SIZE_TO_PAGES(AllocatePageSize),
                                             (VOID **)&(AdminQueueAddress),
                                             EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                             );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    NvmeController->AdminSubmissionUnAligned = AdminQueueAddress;
    NvmeController->AdminSubmissionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
    NvmeController->AdminSubmissionQueue = (AdminQueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                            NvmeController->MemoryPageSize;

    Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                      EfiPciIoOperationBusMasterCommonBuffer,
                                      (VOID *)NvmeController->AdminSubmissionQueue,
                                      &AdminQueueSize,
                                      &NvmeController->AdminSubmissionQueueMappedAddr,
                                      &NvmeController->AdminSubmissionQueueUnMap
                                      );
    
    // Check PCIIO mapped memory is allocated for AdminSubmissionQueue
    if (EFI_ERROR(Status) || (AdminQueueSize < OrgAdminQueueSize) ) {
        return Status;
    }
    
    Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                              AllocateAnyPages,
                                              EfiRuntimeServicesData,
                                              EFI_SIZE_TO_PAGES(AllocatePageSize),
                                              (VOID **)&(AdminQueueAddress),
                                              EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                              );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    NvmeController->AdminCompletionUnAligned = AdminQueueAddress;
    NvmeController->AdminCompletionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
    NvmeController->AdminCompletionQueue = (AdminQueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                            NvmeController->MemoryPageSize;

    Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                       EfiPciIoOperationBusMasterCommonBuffer,
                                       (VOID *)NvmeController->AdminCompletionQueue,
                                       &AdminQueueSize,
                                       &NvmeController->AdminCompletionQueueMappedAddr,
                                       &NvmeController->AdminCompletionQueueUnMap
                                       );
    
    // Check PCIIO mapped memory is allocated for AdminCompletionQueue
    if ( EFI_ERROR(Status) || (AdminQueueSize < OrgAdminQueueSize) ) {
        return Status;
    }
    
    if ( NvmeController->MaxQueueEntrySupported >= ADMIN_QUEUE_SIZE ) {
        NvmeController->AdminSubmissionQueueSize =  NvmeController->AdminCompletionQueueSize = ADMIN_QUEUE_SIZE;
    } else {
        NvmeController->AdminSubmissionQueueSize =  NvmeController->AdminCompletionQueueSize = NvmeController->MaxQueueEntrySupported;
    }

    // Clear memory
    gBS->SetMem((VOID *)NvmeController->AdminSubmissionQueue, AdminQueueSize, 0);
    gBS->SetMem((VOID *)NvmeController->AdminCompletionQueue, AdminQueueSize, 0);
    
    // Program Admin Queue Size and Base Address
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Aqa, ((NvmeController->AdminCompletionQueueSize - 1) << 16) + 
                                                (NvmeController->AdminSubmissionQueueSize - 1) );
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Asq, 
                                                (UINT32) NvmeController->AdminSubmissionQueueMappedAddr);
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Asq + 4,  
                                                (UINT32) RShiftU64(NvmeController->AdminSubmissionQueueMappedAddr, 32) );
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Acq, 
                                                (UINT32)NvmeController->AdminCompletionQueueMappedAddr);
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Acq + 4, 
                                                (UINT32) RShiftU64(NvmeController->AdminCompletionQueueMappedAddr, 32) );

#if  NVME_VERBOSE_PRINT
    DEBUG ((DEBUG_BLKIO, "Admin Submission Queue Size              : %08X\n", NvmeController->AdminSubmissionQueueSize));
    DEBUG ((DEBUG_BLKIO, "Admin Completion Queue Size              : %08X\n", NvmeController->AdminCompletionQueueSize));
    DEBUG ((DEBUG_BLKIO, "Admin Submission Queue Offset            : %08X\n", NvmeController->AdminSubmissionQueue));
    DEBUG ((DEBUG_BLKIO, "Admin Completion Queue Offset            : %08X\n", NvmeController->AdminCompletionQueue));
#endif
    
    NvmeController->NvmeDxeSmmDataArea->AdminPhaseTag = FALSE;
    
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof(NVME_COMMAND_WRAPPER),
                    (VOID**)&(NvmeController->NvmeCmdWrapper)
                    );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // Enable Controller
    CONTROLLER_REG32_OR(NvmeController, Offset_CC, 0x1);
    
    // Wait for the controller to get ready
    // Check if the controller is already running. If yes stop it.
    Delay = NvmeController->TimeOut * 500;
    do {
        if ((CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0x1)) {
            break;
        }
        gBS->Stall(1000); // 1msec delay
    } while (--Delay);
        
    if (!Delay) {
        DEBUG ((EFI_D_ERROR, "Controller not ready     : %X\n", CONTROLLER_REG32(NvmeController, Offset_CSTS)));   
        return EFI_DEVICE_ERROR;
    }
 
    Status = SetNumberOfQueues ( NvmeController,
                                 NvmeController->NvmeCmdWrapper );
    if (EFI_ERROR(Status)) {
        return Status;
    }    
    
    // Install NvmeController
    Status = gBS->InstallMultipleProtocolInterfaces(
                            &Controller,
                            &gAmiNvmeControllerProtocolGuid, 
                            NvmeController,
                            NULL
                            );
    
    
    return Status;
    
}

/**
    Issue Nvme Admin Identify command

    @param NvmeController 
    @param IdentifyData 
    @param ControllerNameSpaceStructure 
    @param NameSpaceID 

    @retval EFI_STATUS

    @note  
    ControllerNameSpaceStructure can take 0/1/2 : 
    See Figure 81 NVM Express 1.1 Spec.
    NameSpaceID can be 0 or specific NameSpace ID. 
    See Figure 38 NVM Express 1.1 Spec.

**/

EFI_STATUS  
GetIdentifyData (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController, 
    OUT UINT8                           *IdentifyData,
    IN  UINT8                           ControllerNameSpaceStructure, 
    IN  UINT32                          NameSpaceID
)
{
    
    EFI_STATUS              Status;
    NVME_COMMAND_WRAPPER    *NvmeCmdWrapper = NULL;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    COMPLETION_QUEUE_ENTRY  CompletionData;
    UINT64                  PRP1_MappedBaseAddr = 0;
    void                    *PRP1_MapedAddress = NULL;
    UINTN                   TempMemSize; 
    UINTN                   TransferSize;   


    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof(NVME_COMMAND_WRAPPER),
                    (VOID**)&(NvmeCmdWrapper)
                    );
    
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    // Clear memory
    gBS->SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
    
   
    if (ControllerNameSpaceStructure == 1 ) {
        
        TempMemSize   = sizeof(IDENTIFY_CONTROLLER_DATA);
    }
    
    if (ControllerNameSpaceStructure == NAMESPACE_LIST_CNS_VALUE ) {
        
        TempMemSize   = 4096;
        
    }
    
    if (ControllerNameSpaceStructure == 0 ) {
        
        TempMemSize   = sizeof(IDENTIFY_NAMESPACE_DATA);
    }
    
    // PRP1:
    Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                            EfiPciIoOperationBusMasterWrite,
                                            (VOID *)IdentifyData,
                                            &TempMemSize,
                                            (EFI_PHYSICAL_ADDRESS *)&PRP1_MappedBaseAddr,
                                            &PRP1_MapedAddress
                                            );
    if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
     }
    
    // Build NVME command 
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = IDENTIFY;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = NameSpaceID;

    ProgramPRPEntry(NvmeController,
                    NvmeCmdWrapper,
                    TempMemSize, 
                    (UINTN*)PRP1_MappedBaseAddr,
                    NULL,    
                    NULL,    //PRP2List & PRP2ListUnMap is not needed for GetIdentify data
                    NULL,
                    &TransferSize);
    
    NvmeCmdWrapper->NvmCmd.CDW10 = ControllerNameSpaceStructure;
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;           // Queue 0 for Admin cmds
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;       
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
    
    if (!EFI_ERROR(Status)) {
        PrintIdentifyDataStructure (IdentifyData, ControllerNameSpaceStructure);
    }
    

    NvmeController->PciIO->Unmap ( NvmeController->PciIO,PRP1_MapedAddress);

    gBS->FreePool(NvmeCmdWrapper);
    return Status;
}

/**
    Detects active Namespace and adds it into a linked list

    @param NvmeController 

    @retval EFI_STATUS

**/

EFI_STATUS
EnumerateActiveNameSpace (
    IN  OUT AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN      UINT32                           ActiveNameSpaceId
    
)
{
    EFI_STATUS                  Status;
    ACTIVE_NAMESPACE_DATA       *ActiveNameSpaceData = NULL;
    LIST_ENTRY                  *LinkData;
    EFI_BLOCK_IO_MEDIA          *BlkMedia;
    
    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
                        LinkData != &NvmeController->ActiveNameSpaceList; 
                         LinkData = LinkData->ForwardLink) {
                                 
        ActiveNameSpaceData = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
        
        // Check if it is an already enumerated NameSpace
        if(ActiveNameSpaceData->ActiveNameSpaceID == ActiveNameSpaceId ){
            DEBUG((DEBUG_INFO,"\nActiveNameSpaceId %x already enumerated",ActiveNameSpaceId));
            return EFI_ALREADY_STARTED;
        }
    }
    
    ActiveNameSpaceData = NULL;
    
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (ACTIVE_NAMESPACE_DATA),
                    (VOID**)&ActiveNameSpaceData
                    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->AllocatePool (EfiBootServicesData,
                                sizeof (EFI_BLOCK_IO_MEDIA),
                                (VOID**)&BlkMedia
                                );
                
    if (EFI_ERROR(Status)) {
        return Status;
    }
    

    gBS->SetMem(ActiveNameSpaceData, sizeof(ACTIVE_NAMESPACE_DATA), 0);
    gBS->SetMem(BlkMedia, sizeof(EFI_BLOCK_IO_MEDIA), 0);
    
    ActiveNameSpaceData->Link.ForwardLink = &(ActiveNameSpaceData->Link);
    ActiveNameSpaceData->Link.BackLink = &(ActiveNameSpaceData->Link);
              
    ActiveNameSpaceData->Signature = ACTIVE_NAME_SPACE_SIG;
    ActiveNameSpaceData->NvmeController = NvmeController;
    ActiveNameSpaceData->Configured = FALSE;
    ActiveNameSpaceData->ActiveNameSpaceID = ActiveNameSpaceId;
    ActiveNameSpaceData->NvmeBlockIO.Media = BlkMedia;

    ActiveNameSpaceData->NvmeBlockIO.Media->MediaId = 0;
    
    Status = DetectActiveNameSpace (NvmeController, ActiveNameSpaceData);
    if (!EFI_ERROR(Status)) {
        DEBUG((DEBUG_INFO,"NameSpace %02X detected %lx\n", ActiveNameSpaceData->ActiveNameSpaceID, ActiveNameSpaceData));
        InsertTailList (&NvmeController->ActiveNameSpaceList, &ActiveNameSpaceData->Link);
        
        // Check Nvme device support Security Send and Security Receive commands
        if (NvmeController->IdentifyControllerData->OACS & BIT0) {
            ActiveNameSpaceData->NvmeSecurityStorageCmd.SendData = NvmeSendData;
            ActiveNameSpaceData->NvmeSecurityStorageCmd.ReceiveData = NvmeReceiveData;                            
    
            // Install EFI_STORAGE_SECURITY_COMMAND_PROTOCOL on device handle
            Status = gBS->InstallMultipleProtocolInterfaces (
                                                 &(ActiveNameSpaceData->NvmeDeviceHandle),
                                                 &gEfiStorageSecurityCommandProtocolGuid, 
                                                 &(ActiveNameSpaceData->NvmeSecurityStorageCmd),
                                                 NULL);
    
            ASSERT_EFI_ERROR(Status);
            return Status;
        }
    } else {
        gBS->FreePool(ActiveNameSpaceData);
    }
    
    return EFI_SUCCESS;
}

/**
    Install BlockIo, DiskInfo and DevicePath protocol for Active namespace

    @param This 
    @param NvmeController 

    @retval EFI_STATUS

**/

EFI_STATUS
InstallBlockIoDiskInfo (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
)
{
    
    EFI_STATUS                  Status =  EFI_SUCCESS;
    ACTIVE_NAMESPACE_DATA       *ActiveNameSpace;
    LIST_ENTRY                  *LinkData;
    EFI_BLOCK_IO_MEDIA          *BlkMedia;
    EFI_PCI_IO_PROTOCOL         *PciIO;
    UINT32                      BlockSize;
    UINTN                       AllocatePageSize;
    UINT16                      MetaDataSize = 0;
    
    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {
        
        ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);

        // Skip the configured Namespace
        if(ActiveNameSpace->Configured == TRUE) {
            continue;
        }
        
        // PRP2List will be cleared when used
        AllocatePageSize = NvmeController->MemoryPageSize;
        Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                             AllocateAnyPages,
                                             EfiRuntimeServicesData,
                                             EFI_SIZE_TO_PAGES(AllocatePageSize),
                                             (VOID**)&ActiveNameSpace->PRP2List,
                                             EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                             );
        
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                        EfiPciIoOperationBusMasterCommonBuffer,
                                        (VOID *)ActiveNameSpace->PRP2List,
                                        &AllocatePageSize,
                                        &ActiveNameSpace->PRP2ListMappedAddr,
                                        &ActiveNameSpace->PRP2ListUnMap
                                        );
          
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        // DiskInfo Protocol 
        gBS->CopyMem (&(ActiveNameSpace->NvmeDiskInfo.Interface), &gEfiDiskInfoNvmeInterfaceGuid, sizeof (EFI_GUID));

        ActiveNameSpace->NvmeDiskInfo.Inquiry = NvmeDiskInfoInquiry;
        ActiveNameSpace->NvmeDiskInfo.Identify = NvmeDiskInfoIdentify;
        ActiveNameSpace->NvmeDiskInfo.SenseData = NvmeDiskInfoSenseData;
        ActiveNameSpace->NvmeDiskInfo.WhichIde = NvmeDiskInfoWhichIde;

        // Create DevicePath Protocol
        Status = CreateNvmeDevicePath (This, ActiveNameSpace);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        // Install Devicepath and Disk Info protocol
        Status = gBS->InstallMultipleProtocolInterfaces (
                        &(ActiveNameSpace->NvmeDeviceHandle),              
                        &gEfiDevicePathProtocolGuid, ActiveNameSpace->EfiDevicePath,
                        &gEfiDiskInfoProtocolGuid, &(ActiveNameSpace->NvmeDiskInfo),
                        NULL);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
		// If NameSpace size is zero, don't install BlockIo Protocol
        if(ActiveNameSpace->IdentifyNamespaceData.NSIZE) {
            
            if(!ActiveNameSpace->NvmeBlockIO.Media) {
                
              Status = gBS->AllocatePool (EfiBootServicesData,
                                          sizeof (EFI_BLOCK_IO_MEDIA),
                                          (VOID**)&BlkMedia
                                          );
                        
              if (EFI_ERROR(Status)) {
                continue;
              }
                        
              gBS->SetMem(BlkMedia, sizeof(EFI_BLOCK_IO_MEDIA), 0);
              ActiveNameSpace->NvmeBlockIO.Media = BlkMedia;
            }

            BlockSize =  ActiveNameSpace->IdentifyNamespaceData.LBAF[ActiveNameSpace->IdentifyNamespaceData.FLBAS & 0xF].LBADS;
            MetaDataSize = ActiveNameSpace->IdentifyNamespaceData.LBAF[ActiveNameSpace->IdentifyNamespaceData.FLBAS & 0xF].MS;
            ActiveNameSpace->NvmeBlockIO.Media->MediaId = 0;            
            ActiveNameSpace->NvmeBlockIO.Media->RemovableMedia = FALSE;            
            ActiveNameSpace->NvmeBlockIO.Media->MediaPresent = TRUE;            
            ActiveNameSpace->NvmeBlockIO.Media->LogicalPartition = FALSE;            
            ActiveNameSpace->NvmeBlockIO.Media->ReadOnly = FALSE;            
            ActiveNameSpace->NvmeBlockIO.Media->WriteCaching = FALSE;            
            ActiveNameSpace->NvmeBlockIO.Media->BlockSize = (UINT32) LShiftU64(1, BlockSize);              
            ActiveNameSpace->NvmeBlockIO.Media->IoAlign = 4;      // DWORD aligned PRP Entry register BITS 1:0 are reserved 
            // Name space Size
            ActiveNameSpace->NvmeBlockIO.Media->LastBlock = (EFI_LBA) (ActiveNameSpace->IdentifyNamespaceData.NSIZE - 1);
            ActiveNameSpace->NvmeBlockIO.Media->LowestAlignedLba = 0;
            ActiveNameSpace->NvmeBlockIO.Media->LogicalBlocksPerPhysicalBlock = 1;            
            ActiveNameSpace->NvmeBlockIO.Media->OptimalTransferLengthGranularity = (UINT32) ActiveNameSpace->NvmeBlockIO.Media->BlockSize;            
            ActiveNameSpace->NvmeBlockIO.Revision = EFI_BLOCK_IO_PROTOCOL_REVISION3;
            ActiveNameSpace->NvmeBlockIO.Reset = NvmeReset;
            ActiveNameSpace->NvmeBlockIO.ReadBlocks = NvmeReadBlocks;
            ActiveNameSpace->NvmeBlockIO.WriteBlocks = NvmeWriteBlocks;
            ActiveNameSpace->NvmeBlockIO.FlushBlocks = NvmeFlushBlocks;
	
            // Install Block IO protocol
            Status = gBS->InstallMultipleProtocolInterfaces (
                                    &(ActiveNameSpace->NvmeDeviceHandle),              
                                    &gEfiBlockIoProtocolGuid, &(ActiveNameSpace->NvmeBlockIO),
                                    NULL);
            if (EFI_ERROR(Status)) {
                continue;
            }
            // Handle MetaData
            if (MetaDataSize) {

                DEBUG ((DEBUG_BLKIO, "NameSpace has valid metadata size    : %d\n",MetaDataSize ));
                // Is MetaData part of LBA?
                if (!(ActiveNameSpace->IdentifyNamespaceData.FLBAS & 0xF0)) {

                    // Buffer needed only for MetaData. Use the User provided buffer for LBA
                    // If Metadata is 8 bytes and protection enabled no need for extra buffer.
                    if ((MetaDataSize != 8) || !(ActiveNameSpace->IdentifyNamespaceData.DPS & 0x07)) {
                        ActiveNameSpace->MetaDataBufferSize = SIZE_1MB * 2;
                        Status = NvmeController->PciIO->AllocateBuffer(NvmeController->PciIO,
                                                         AllocateAnyPages,
                                                         EfiRuntimeServicesData,
                                                         EFI_SIZE_TO_PAGES((UINTN)ActiveNameSpace->MetaDataBufferSize),  
                                                         (VOID**)&ActiveNameSpace->MetaDataBlock,
                                                         EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                                         );

                        DEBUG ((DEBUG_BLKIO, "PciIO->AllocateBuffer %r %lx \n", Status,  ActiveNameSpace->MetaDataBufferSize));
                        if (EFI_ERROR(Status)) {
                            continue;
                        }
                    }
                } else {
                    // If Metadata is 8 bytes and protection enabled no need for extra buffer.
                    if ((MetaDataSize != 8) || !(ActiveNameSpace->IdentifyNamespaceData.DPS & 0x07)) {
                        // Double the block size to handle MetaData and LBA data. (1MB Block of data + 1MB block of Meta Data)
                        ActiveNameSpace->ExtendedLBABufferSize = SIZE_1MB * 2;
                        Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                                         AllocateAnyPages,
                                                         EfiRuntimeServicesData,
                                                         EFI_SIZE_TO_PAGES((UINTN)ActiveNameSpace->ExtendedLBABufferSize),  
                                                         (VOID**)&ActiveNameSpace->ExtendedLBA,
                                                         EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                                         );
                        DEBUG ((DEBUG_BLKIO, "PciIO->AllocateBuffer %r %lx \n", Status, ActiveNameSpace->ExtendedLBABufferSize ));
                        if (EFI_ERROR(Status)) {
                            continue;
                        }
                    }
                }
            }
        }
        
        // Open PCI IO protocol by CHILD
        Status = gBS->OpenProtocol (
                    NvmeController->ControllerHandle,
                    &gEfiPciIoProtocolGuid,
                    (VOID **)&PciIO,
                    This->DriverBindingHandle,     
                    ActiveNameSpace->NvmeDeviceHandle,   
                    EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER);
        
        ASSERT_EFI_ERROR(Status);
        
        ActiveNameSpace->Configured = TRUE;
    }
	
   // If any ActiveNameSapce is not configured properly, 
   // then remove that corresponding ActiveNameSapce from the list.
    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList;
         ) {
        
        ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
       
        if (!ActiveNameSpace->Configured) {

            UninstallNvmeDeviceProtocols( This,
                                          ActiveNameSpace->NvmeDeviceHandle,
                                          NvmeController,
                                          ActiveNameSpace,
                                          &LinkData
                                         );    
        } else {
            LinkData = LinkData->ForwardLink;
        }
    }

    return Status;
}

/**
    Issues LockOnReset Command.

    @param This
    @param BusInterface 
    @param BusInterfaceType 

    @retval VOID

**/
EFI_STATUS 
IssueLockOnResetCommand (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
)
{
    EFI_STATUS                                   Status;
    UINT16                                       ListLength = 0;
    UINT16                                       SpByte = 0;
    void                                         *Buffer;
    UINTN                                        BufferSize = sizeof(NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA);
    UINT32                                       MediaId = 0;
    NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA      *QueryBuffer = NULL;
    ACTIVE_NAMESPACE_DATA                       *ActiveNameSpace = NULL;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL       *StorageSecurityInterface;
    
    // Check Nvme device supports Security Send and Security Receive commands
    if(! (NvmeController->IdentifyControllerData->OACS & BIT0)) {
        return EFI_UNSUPPORTED;
    }
    
    Status = gBS->AllocatePool(EfiBootServicesData, 
                               BufferSize, 
                               (VOID**)&Buffer);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return EFI_OUT_OF_RESOURCES;
    }
    gBS->SetMem(Buffer, BufferSize, 0);

    ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );
    
    StorageSecurityInterface = &(ActiveNameSpace->NvmeSecurityStorageCmd);
    Status = StorageSecurityInterface->ReceiveData( 
                                         StorageSecurityInterface,
                                         MediaId,                         // Media ID
                                         MultU64x32(NVME_COMMAND_TIMEOUT, 100000000), // 10sec timeout
                                         SECURITY_PROTOCOL_INFORMATION,   // Security Protocol ID
                                         0x00,                            // SP specific id
                                         0x200,                           // Transfer Length
                                         Buffer,
                                         &BufferSize                      // Transferred Length
                                         );
    if(EFI_ERROR(Status)) {
        return Status;
    }
    
    QueryBuffer =  (NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA*) Buffer;

    //
    // The List Length field indicates the total length, in bytes, of the supported security protocol list.
    //
    ListLength = (QueryBuffer->ListLengthHigh << 8) | QueryBuffer->ListLengthLow;
    if (ListLength == 0) {
        return EFI_UNSUPPORTED;
    }

    for (SpByte = 0; SpByte < ListLength; SpByte++) {
        if (QueryBuffer->SupportedSPList[SpByte] == SECURITY_PROTOCOL_2) {
            //
            // Found TCG Security Protocol Supported.Issue TCG TPer Reset
            //
            BufferSize = 0x200;
            gBS->SetMem(Buffer, BufferSize, 0);
            StorageSecurityInterface->SendData(
                                     StorageSecurityInterface,
                                     0,
                                     0,
                                     SECURITY_PROTOCOL_2,   // Security Protocol 2 ID
                                     0x0400,                // SP specific id
                                     BufferSize,            // TranferLength - Non zero Value
                                     &Buffer
                                     );
            break;
        }
    }
    
    gBS->FreePool(Buffer);
    return Status;
}

/**
    Allocate memory and create a NVMe devicepath

    @param This 
    @param NvmeController 

    @retval EFI_STATUS

**/

EFI_STATUS
CreateNvmeDevicePath (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  ACTIVE_NAMESPACE_DATA       *ActiveNameSpace
)
{

    EFI_STATUS                          Status;
    IN AMI_NVME_CONTROLLER_PROTOCOL     *NvmeController = ActiveNameSpace->NvmeController;
    NVME_DEVICE_PATH                    NewDevicePath;
    EFI_DEVICE_PATH_PROTOCOL            *TempDevicePath;

    NewDevicePath.Header.Type = MESSAGING_DEVICE_PATH;
    NewDevicePath.Header.SubType = MSG_NVME_DP;
    SetDevicePathNodeLength(&NewDevicePath, sizeof(NVME_DEVICE_PATH));
    NewDevicePath.Nsid = ActiveNameSpace->ActiveNameSpaceID;
    NewDevicePath.EUI64 = ActiveNameSpace->IdentifyNamespaceData.EUI64;
    
    //
    // Append the Device Path
    //
    Status = gBS->OpenProtocol ( 
                    NvmeController->ControllerHandle,
                    &gEfiDevicePathProtocolGuid,
                    (VOID **)&TempDevicePath,
                    This->DriverBindingHandle,     
                    NvmeController->ControllerHandle,   
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL);

    ActiveNameSpace->EfiDevicePath = AppendDevicePathNode(TempDevicePath, &NewDevicePath.Header);
    
    return Status;
}

/**
    Issue GetIdentifyData cmd to get the list of active name space. 

    @param NvmeController 
    @param ActiveNameSpaceData 

    @retval EFI_STATUS

**/

EFI_STATUS
DetectActiveNameSpace (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    OUT ACTIVE_NAMESPACE_DATA           *ActiveNameSpaceData
)
{

    EFI_STATUS  Status;
    
    // Get list of Active NameSpace Data
    Status = GetIdentifyData (NvmeController, 
                        (UINT8*)&(ActiveNameSpaceData->IdentifyNamespaceData),
                        IDENTIFY_NAMESPACE_CNS_VALUE, 
                        ActiveNameSpaceData->ActiveNameSpaceID
                        );
    return Status;

}


/**
    Reads data from the given LBA address

        
    @param This 
    @param MediaId 
    @param LBA
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
)
{
    
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_THIS(This);
    
    return NvmeReadWriteBlocks (ActiveNameSpace, MediaId, LBA, BufferSize, Buffer, NULL, NVME_READ);
    
}


/**
    Write data from the given LBA address

    @param This 
    @param MediaId 
    @param LBA
    @param BufferSize 
    @param Buffer 

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer
)
{
   
    EFI_STATUS                 Status;
    ACTIVE_NAMESPACE_DATA      *ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_THIS(This);

    if (AmiBlkWriteProtection != NULL) {
        Status = AmiBlkWriteProtection->BlockIoWriteProtectionCheck ( 
                                                    AmiBlkWriteProtection,
                                                    &(ActiveNameSpace->NvmeBlockIO),
                                                    LBA,
                                                    BufferSize
                                                    );
        
        // Abort operation if denied
        if(Status == EFI_ACCESS_DENIED) {
            return Status;
        }
    }
    
    return NvmeReadWriteBlocks (ActiveNameSpace, MediaId, LBA, BufferSize, Buffer, NULL, NVME_WRITE);
    
}

/**
    Resets Nvme Controller

    @param This 
    @param ExtendedVerification 

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeReset (
    IN  EFI_BLOCK_IO_PROTOCOL   *This,
    IN  BOOLEAN                 ExtendedVerification
)
{
    
    EFI_STATUS  Status = EFI_SUCCESS;
    
    return Status;
}

/**
    Flushes the data

    @param This 

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
NvmeFlushBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL   *This
)
{
    
    EFI_STATUS                      Status;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_THIS(This);
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController = ActiveNameSpace->NvmeController;
    NVME_DXE_SMM_DATA_AREA          *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper = NvmeController->NvmeCmdWrapper;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    
    DEBUG((DEBUG_BLKIO,"NvmeFlushBlocks \n"));

    gBS->SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0); 
    
    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = NVME_FLUSH;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierQueue1;
    NvmeCmdWrapper->NvmCmd.NSID = ActiveNameSpace->ActiveNameSpaceID;
        
    NvmeCmdWrapper->AdminOrNVMCmdSet = FALSE;
    NvmeCmdWrapper->SQIdentifier = NvmeController->NVMQueueNumber;
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;      
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
    
    DEBUG((DEBUG_BLKIO,"NvmeFlushBlocks Status %r \n", Status));
    
    return Status;
    
}

/**
    Return Nvme device Inquiry data

    @param  This 
    @param  InquiryData 
    @param  InquiryDataSize 

    @retval EFI_STATUS /EFI_NOT_FOUND

**/

EFI_STATUS
EFIAPI
NvmeDiskInfoInquiry (
    IN EFI_DISK_INFO_PROTOCOL    *This,
    IN OUT VOID                  *InquiryData,
    IN OUT UINT32                *InquiryDataSize
)
{

    return EFI_NOT_FOUND;
}

/**
    Return Identify Data

        
    @param This,
    @param IdentifyData 
    @param IdentifyDataSize 

    @retval EFI_STATUS

    @note  
      1. Return the Nvme device Identify command data.

**/
EFI_STATUS
EFIAPI
NvmeDiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL    *This,
    IN OUT VOID               *IdentifyData,
    IN OUT UINT32             *IdentifyDataSize
)
{
    ACTIVE_NAMESPACE_DATA       *ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_THIS_FOR_DISKINFO(This);

    if (*IdentifyDataSize < sizeof (IDENTIFY_NAMESPACE_DATA)) {
        *IdentifyDataSize = sizeof (IDENTIFY_NAMESPACE_DATA);
        return EFI_BUFFER_TOO_SMALL;
    }

    gBS->CopyMem (IdentifyData, &(ActiveNameSpace->IdentifyNamespaceData), sizeof (IDENTIFY_NAMESPACE_DATA)); 
    *IdentifyDataSize =  sizeof (IDENTIFY_NAMESPACE_DATA);
    return EFI_SUCCESS;
}

/**
    Return InfoSenseData.

    @param  This,
    @param  SenseData,
    @param  SenseDataSize,
    @param  SenseDataNumber

    @retval EFI_STATUS

    @note  
      1. Return the Sense data for the Nvme device.

**/

EFI_STATUS
EFIAPI
NvmeDiskInfoSenseData (
    IN EFI_DISK_INFO_PROTOCOL    *This,
    OUT VOID                     *SenseData,
    OUT UINT32                   *SenseDataSize,
    OUT UINT8                    *SenseDataNumber
)
{
    return EFI_NOT_FOUND;
}

/**
    Returns info about where the device is connected.

    @param This 
    @param IdeChannel 
    @param IdeDevice 

    @retval EFI_STATUS

    @note  
  1. Return Port and PMPort

**/
EFI_STATUS
EFIAPI
NvmeDiskInfoWhichIde (
    IN EFI_DISK_INFO_PROTOCOL    *This,
    OUT UINT32                   *IdeChannel,
    OUT UINT32                   *IdeDevice
)
{

    return EFI_UNSUPPORTED;
}

/**
    Function coverts Nvme Model number into Unicodestring.

    @param NvmeController

    @retval EFI_STATUS

**/
EFI_STATUS
GetNvmeUnicodeStringName (
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
)
{
    EFI_STATUS                  Status=EFI_SUCCESS;
    UINT16                      DeviceName[41];
    CHAR8                       Language[] = LANGUAGE_CODE_ENGLISH;
    EFI_UNICODE_STRING_TABLE    *tempUnicodeTable;
    UINT32                      Index;

    gBS->SetMem(DeviceName, sizeof(DeviceName), 0);

    for (Index = 0; Index < 40; Index ++) {
        DeviceName[Index] = ((UINT8 *)NvmeController->IdentifyControllerData->ModelNumber)[Index];
    }

    DeviceName[40] = 0; // Word
    
    // Remove the spaces from the end of the device name
    for (Index = 39; Index > 0; Index-- ) {
        if (DeviceName[Index] != 0x20) { 
             break;
        }
        DeviceName[Index] = 0;
    }

    Status = gBS->AllocatePool ( EfiBootServicesData,
                                 sizeof (EFI_UNICODE_STRING_TABLE) * 2,
                                 (VOID**)&tempUnicodeTable
                                 );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    gBS->SetMem(tempUnicodeTable, sizeof(EFI_UNICODE_STRING_TABLE) * 2, 0);

    Status = gBS->AllocatePool ( EfiBootServicesData,
                                 sizeof (Language),
                                (VOID**)&tempUnicodeTable[0].Language
                                );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    Status = gBS->AllocatePool ( EfiBootServicesData,
                                 sizeof (DeviceName),
                                 (VOID**)&tempUnicodeTable[0].UnicodeString
                                 );

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    gBS->CopyMem(tempUnicodeTable[0].Language, &Language, sizeof(Language)); 
    gBS->CopyMem(tempUnicodeTable[0].UnicodeString, DeviceName, sizeof (DeviceName)); 
    tempUnicodeTable[1].Language = NULL;
    tempUnicodeTable[1].UnicodeString = NULL;
    NvmeController->UDeviceName = tempUnicodeTable;

    return Status;
}

/**
    Function converts Big endian dword to Little Endian Dword and Vice versa.

    @param EndianDword
    
    @retval UINT32
**/
UINT32
ToBigLittleEndianDword (
    UINT32 EndianDword
) 
{
    return (((EndianDword & 0xFF) << 24) + ((EndianDword & 0xFF00) << 8) + \
            ((EndianDword & 0xFF0000) >>8) + ((EndianDword & 0xFF000000) >>24));
}

/**
    Function converts Big endian Qword to Little Endian Qword and Vice versa.

    @param EndianDword
    
    @retval UINT64
**/
UINT16
ToBigLittleEndianWord (
    UINT16 EndianWord
) 
{
    return (((EndianWord >> 8) & 0xFF) + (EndianWord << 8));
}

/**
    Function sends freeze lock command to Nvme controller

    @param NvmeController - Pointer to AMI_NVME_CONTROLLER_PROTOCOL

    @retval EFI_SUCCESS - FreezeLock command executed successfully
            EFI_DEVICE_ERROR - FreezeLock command completed with error
            EFI_NOT_FOUND - Device already in frozen state

**/
VOID
NvmeSendSecurityFreezeLock (
    IN AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController
)
{
    EFI_STATUS                                 Status;
    NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA    *QueryBuffer;
    UINTN                                      SpByte;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL      *StorageSecurityProtocol = NULL;
    UINT16                                     ListLength = 0;
    NVME_OPAL_DEVICE                           *OpalDevice;
    VOID                                       *Buffer = NULL;
    UINTN                                      BufferLength = 512;
    NVME_DEVICE_SECURITY_STATUS                NvmeSecurityStatus;
	VOID                                       *TreeProtocol = NULL;
    EFI_PHYSICAL_ADDRESS                       NvsMemoryAddress;
    EFI_GUID                                   FlagsStatusguid = AMI_TCG_CONFIRMATION_FLAGS_GUID;
    AMI_ASL_PPI_NV_VAR                         *TpmAcpiNvsFlags;
    UINTN                                      Size = sizeof (EFI_PHYSICAL_ADDRESS);  
	BOOLEAN                                    IssueBlockSid = TRUE;
	EFI_SMM_CONTROL2_PROTOCOL                  *gSmmCtl = NULL;
#ifdef  PPI_OFFSET
    UINT8                                      PpiOffsetValue = PPI_OFFSET;
#else
    UINT8                                      PpiOffsetValue = 0x35;        // Default smi value for PpiOffset 
#endif
    ACTIVE_NAMESPACE_DATA                     *ActiveNameSpace = NULL;
    
    // Check Nvme device supports Security Send and Security Receive commands
    if(! (NvmeController->IdentifyControllerData->OACS & BIT0)) {
        return;
    }
    
    ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );
    
    StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
    
    // Call GetNvmeDeviceLockStatus() to get latest data
    Status = GetNvmeDeviceLockStatus(NvmeController, &NvmeSecurityStatus);
    ASSERT_EFI_ERROR(Status);
	
    if((NvmeController->SupportedSpList == NULL) || EFI_ERROR(Status)) {
        return;
    }
    
    QueryBuffer = (NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA *)NvmeController->SupportedSpList;
        
    // The List Length field indicates the total length, in bytes, of the supported security protocol list.
    ListLength = QueryBuffer->ListLengthHigh << 8 | QueryBuffer->ListLengthLow;

    if (ListLength == 0) {
        return;
    }
    
    Status = gBS->LocateProtocol(&gEfiTrEEProtocolGuid,
                          NULL,
                          &TreeProtocol);
    if(!EFI_ERROR(Status)) {    
    	// If the platform has TPM 2.0 device connected,
    	// TCG2 driver manages Block SID feature, so NVME driver
    	// shouldn't send Block SID command.
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
    		
    		//Trigger a software SMI with the PPI_OFFSET value, to get the response for the PPI Request
    		Status = gBS->LocateProtocol(&gEfiSmmControl2ProtocolGuid, NULL, (VOID**)&gSmmCtl);
			ASSERT_EFI_ERROR(Status);
    		if (!EFI_ERROR(Status)){
        	    gSmmCtl->Trigger(gSmmCtl, &PpiOffsetValue, NULL, FALSE, 0);
    	    }

	        // If RequestFuncResponse is a non zero value, then TCG2 driver will manage the BlockSid command
		    if(TpmAcpiNvsFlags->RequestFuncResponse ) {
		        IssueBlockSid = FALSE;
		    }
        }
    }
    for (SpByte = 0; SpByte < ListLength; SpByte++) {
        
        // Check Security supported Protocol List 
        // If gAmiTcgStorageSecurityProtocolGuid is not installed in the controller and
        // if HddSecurity module is not available then Nvme driver sends freeze lock command.
        if ((QueryBuffer->SupportedSPList[SpByte] == NVME_SECUIRTY_SECP) && (gNvmeHddSecurityInitProtocol == NULL)\
            && !IsSecurityProtocolInstalled(NvmeController->ControllerHandle, &gAmiTcgStorageSecurityProtocolGuid)) {
          
            // Freeze Lock command will not work with the Nvme in Locked State.
            // So return with Success if the Nvme is either in Locked State or Frozen state.
            if(NvmeSecurityStatus.SAT3DeviceStatus & (BIT2 | BIT3)) {
                continue;
            }
                           
            Status = StorageSecurityProtocol->SendData( StorageSecurityProtocol,
                                                        0,                                      // Media ID
                                                        NVME_COMMAND_TIMEOUT * 100000000,       // 10sec timeout
                                                        NVME_SECUIRTY_SECP,
                                                        ToBigLittleEndianWord(NVME_SECURITY_FREEZE_LOCK),
                                                        0,
                                                        0
                                                        );
            if(EFI_ERROR(Status)) {
                DEBUG((EFI_D_ERROR,"\nSending SAT3 Freeze lock command Status %r\n", Status));    
            }

        }   
        
        // Check Security supported Protocol List for Protocol ID 1 and IssueBlockSid value 
        // if gAmiHddSecurityProtocolGuid is not installed in the controller handle and
        // if TcgStorageSecurity module is not available then Nvme driver sends BlockSid command.
        if ((QueryBuffer->SupportedSPList[SpByte] == NVME_SECURITY_PROTOCOL_1)  && (IssueBlockSid) && (gAmiTcgStorageSecurityInitProtocol==NULL)\
            && !IsSecurityProtocolInstalled(NvmeController->ControllerHandle, &gAmiHddSecurityProtocolGuid)) {
            
            if(NvmeController->OpalConfig == NULL) {
                continue;
            }
            
            OpalDevice = (NVME_OPAL_DEVICE*)NvmeController->OpalConfig;
            
            // Check Block SID feature supported by the device
            if (OpalDevice->BlockSid.FeatureCode != NVME_Block_SID_Authentification_Feature) {
                continue;
            }
            
            // Check whether C_PIN_SID is equal to C_PIN_MISD or not
            if (OpalDevice->BlockSid.SIDStateValue == 1) {
                continue;
            }
           
            // Check whether SID already blocked or not
            if (OpalDevice->BlockSid.SIDBlockedState == 1) {
                continue;
            }
            
            // Check whether device already locked or not
            if(OpalDevice->LockingFeature & NVME_SECURITY_LOCK_MASK) {
                continue;
            }
            
            Status = gBS->AllocatePool(EfiBootServicesData, BufferLength, (VOID**)&Buffer);
            if(EFI_ERROR(Status)) {
                ASSERT_EFI_ERROR(Status);
                return;
            }
            
            gBS->SetMem(Buffer, BufferLength, 0);
            ((UINT8 *)Buffer)[0] = 1;
            
            Status = StorageSecurityProtocol->SendData( StorageSecurityProtocol,
                                                        0,                                  // Media ID
                                                        NVME_COMMAND_TIMEOUT * 100000000,   // 10sec timeout
                                                        NVME_SECURITY_PROTOCOL_2,
                                                        ToBigLittleEndianWord(NVME_BLOCK_SID_COMID),
                                                        BufferLength,
                                                        Buffer
                                                       );
            if(EFI_ERROR(Status)) {
                DEBUG((EFI_D_ERROR,"\nSending BlockSid command Status %r\n", Status));    
            }
			
            gBS->FreePool (Buffer);
        }
    }
    return;
}

/**
 Function to check whether passed security protocol is installed in the Controller.
 
     @param  DeviceHandle           - NvmeDeviceHandle
     @param  SecurityProtocolGuid   - SecurityProtocolGuid Need to be checked
     
     @retval BOOLEAN                 TRUE / FALSE
             TRUE - Security protocol is installed
             FALSE - Security protocol is not installed
*/
BOOLEAN
IsSecurityProtocolInstalled (
    IN  EFI_HANDLE ControllerHandle,
    IN  EFI_GUID  *SecurityProtocolGuid
)
{
    EFI_STATUS  Status;
    BOOLEAN     InstallSecurityStatus = TRUE;
    
    // Check if the passed security protocol
    // is installed in the Controller.
    Status = gBS->OpenProtocol( ControllerHandle,
                                SecurityProtocolGuid,
                                NULL,
                                NULL,
                                NULL, 
                                EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                                );
        
    if(EFI_ERROR(Status)) {
        InstallSecurityStatus = FALSE;
    }

    return InstallSecurityStatus;
}

/**
    Function to update OPAL device Level 0 data into current device structure.

    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param QueryBuffer - Pointer to NVME_LEVEL0_DISCOVERY_DATA buffer

    @retval VOID 
**/
VOID
NvmeUpdateLevel0DiscoveryData(
    OUT NVME_OPAL_DEVICE             *OpalDevice,
    IN NVME_LEVEL0_DISCOVERY_DATA    *QueryBuffer
)
{
    UINT16                                FeatureCode;
    NVME_FEATURE_DESCRIPTOR               *FeatureDescriptor;
    NVME_FEATURE_DESCRIPTOR_BLOCK_SID     *BlockSid;
    
    UINTN   TotalLength = ToBigLittleEndianDword(QueryBuffer->Level0DiscoveryHeader.Length) + 4;
    
    FeatureDescriptor = (NVME_FEATURE_DESCRIPTOR *)((UINT8 *)QueryBuffer + sizeof(NVME_LEVEL0_DISCOVERY_HEADER));
    do {

        FeatureCode = ToBigLittleEndianWord(FeatureDescriptor->FeatureCode);

        switch (FeatureCode) {

            case NVME_Locking_Feature:
                OpalDevice->LockingFeature = FeatureDescriptor->Data[0];
                break;
                
            case NVME_Block_SID_Authentification_Feature:
                BlockSid = (NVME_FEATURE_DESCRIPTOR_BLOCK_SID *) FeatureDescriptor;
                OpalDevice->BlockSid.FeatureCode = ToBigLittleEndianWord(BlockSid->FeatureCode);
                OpalDevice->BlockSid.SIDStateValue = BlockSid->SIDStateValue;
                OpalDevice->BlockSid.SIDBlockedState = BlockSid->SIDBlockedState;                   
                OpalDevice->BlockSid.HWReset = BlockSid->HWReset;
                break;

            default:
                break;
        }

        FeatureDescriptor = (NVME_FEATURE_DESCRIPTOR *)((UINT8 *)FeatureDescriptor + FeatureDescriptor->Length + 4);

    } while ((UINT8 *)FeatureDescriptor < (UINT8 *)((UINT8 *)QueryBuffer + TotalLength));
    
    return;
}

/**
    Function to get OPAL device Level 0 data.
    
    @param  NvmeController - Pointer to AMI_NVME_CONTROLLER_PROTOCOL
            OpalDevice     - Pointer to NVME_OPAL_DEVICE

    @retval EFI_SUCCESS - Success
            EFI_OUT_OF_RESOURCES - Buffer allocate failed
**/
EFI_STATUS
NvmeGetLevel0DiscoveryData (
    IN   AMI_NVME_CONTROLLER_PROTOCOL         *NvmeController,
    OUT  NVME_OPAL_DEVICE                     *OpalDevice
)
{
    EFI_STATUS                            Status;
    NVME_LEVEL0_DISCOVERY_DATA            *Buffer = NULL;
    UINTN                                 BufferLength = 512;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL *StorageSecurityProtocol = NULL;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
    
    // Check Nvme device supports Security Send and Security Receive commands
    if(! (NvmeController->IdentifyControllerData->OACS & BIT0)) {
        return EFI_UNSUPPORTED;
    }
    
    ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );
    
    StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
            
    if(OpalDevice == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    Status = gBS->AllocatePool( EfiBootServicesData, BufferLength, (VOID**)&Buffer);
    if (EFI_ERROR(Status)) {
        return EFI_OUT_OF_RESOURCES;
    }
    gBS->SetMem(Buffer, BufferLength, 0);

    Status = StorageSecurityProtocol->ReceiveData( StorageSecurityProtocol,
                                                   0,                                                  // Media ID
                                                   NVME_COMMAND_TIMEOUT * 100000000,                   // 10sec timeout
                                                   NVME_SECURITY_PROTOCOL_1,                           // Security Protocol ID
                                                   ToBigLittleEndianWord(NVME_COMID_LEVEL0_DISCOVERY), // SP specific id
                                                   0x200,                                              // TranferLength
                                                   Buffer,
                                                   &BufferLength
                                                   );
    
    if (!EFI_ERROR(Status)) {
        // Update Level 0 discovery data to OpalDevice structure
        NvmeUpdateLevel0DiscoveryData(OpalDevice, Buffer);
    }
   
    gBS->FreePool (Buffer);
    
    return Status;
}

/**
    Function to get supported security protocols list 

    @param  NvmeController - Pointer to AMI_NVME_CONTROLLER_PROTOCOL
    @param  QueryBuffer    - Pointer to NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA

    @retval EFI_STATUS
**/
EFI_STATUS
GetSupportedSecurityProtocolList (
    IN AMI_NVME_CONTROLLER_PROTOCOL                *NvmeController,
    IN OUT NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA *QueryBuffer
)
{
    EFI_STATUS                                 Status;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL      *StorageSecurityProtocol = NULL;
    UINTN                                      QueryBufferSize = sizeof(NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA);
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
    
    // Check Nvme device supports Security Send and Security Receive commands
    if(! (NvmeController->IdentifyControllerData->OACS & BIT0)) {
        return EFI_UNSUPPORTED;
    }
    
    ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );
    
    StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
    
    if(QueryBuffer == NULL) {
        return EFI_UNSUPPORTED;
    }

    gBS->SetMem(QueryBuffer, sizeof (NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA), 0);
    // SPC Spec Section 7.7.1 has details on decoding the data
    Status = StorageSecurityProtocol->ReceiveData( StorageSecurityProtocol,
                                                   0,                                   // Media ID
                                                   NVME_COMMAND_TIMEOUT * 100000000,    // 10sec timeout
                                                   NVME_SECURITY_PROTOCOL_INFORMATION,  // Security Protocol ID
                                                   0x00,                                // SP specific id
                                                   0x200,                               // Transfer Length
                                                   QueryBuffer,
                                                   &QueryBufferSize                     // Transferred Length
                                                   );
    return Status;
}

/**
    Function to get NVMe device security status

    @param  NvmeController - Pointer to AMI_NVME_CONTROLLER_PROTOCOL
    @param  NvmeSecurityStatus - Pointer to NVME_DEVICE_SECURITY_STATUS
    @param  NvmeDeviceType

    @retval EFI_STATUS
    
    @note   NvmeSecurityStatus
                Bit 0 : Security Supported
                Bit 1 : Security Enabled
                Bit 2 : Security Locked
                Bit 3 : Security Frozen
**/
EFI_STATUS
GetNvmeDeviceLockStatus (
    IN OUT AMI_NVME_CONTROLLER_PROTOCOL         *NvmeController,
    OUT NVME_DEVICE_SECURITY_STATUS             *NvmeSecurityStatus
) 
{
    EFI_STATUS                                 Status = EFI_UNSUPPORTED;
    NVME_OPAL_DEVICE                           *OpalDevice = NULL;
    NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA    *QueryBuffer = NULL;
    UINTN                                      SpByte;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL      *StorageSecurityProtocol = NULL;
    UINT16                                     ListLength = 0;
    NVME_SECURITY_RECEIVE_BUFFER               SecReceiveBuffer;
    UINTN                                      SecReceiveBufferSize = sizeof(NVME_SECURITY_RECEIVE_BUFFER);
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
    
    // Check Nvme device supports Security Send and Security Receive commands
    if(! (NvmeController->IdentifyControllerData->OACS & BIT0)) {
        return EFI_UNSUPPORTED;
    }
    
    ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );
    
    StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
    
    if(NvmeSecurityStatus == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    // Initialize structure
    gBS->SetMem(NvmeSecurityStatus, sizeof(NVME_DEVICE_SECURITY_STATUS), 0);
    

    // Check Supported Security Protocol List available or not
    if(NvmeController->SupportedSpList == NULL) {
        Status = gBS->AllocatePool(EfiBootServicesData,
                                   sizeof(NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA),
                                   (VOID**)&(NvmeController->SupportedSpList));
        if (EFI_ERROR(Status)) {
            return Status;
        }
        
        Status = GetSupportedSecurityProtocolList(NvmeController, 
                                                 (NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA*)NvmeController->SupportedSpList);
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }
    
    QueryBuffer = (NVME_SP0_TRUSTED_RECEIVE_PARAMETER_DATA*)NvmeController->SupportedSpList;
        
    // The List Length field indicates the total length, in bytes, of the supported security protocol list.
    ListLength = QueryBuffer->ListLengthHigh << 8 | QueryBuffer->ListLengthLow;

    if (ListLength == 0) {
        return EFI_UNSUPPORTED;
    }

    for (SpByte = 0; SpByte < ListLength; SpByte++) {
        // Get the security status of the SAT3 supported device,
        // only if it is not handled by HddSecurity and TcgStorageSecurity driver.
        if ((QueryBuffer->SupportedSPList[SpByte] == NVME_SECUIRTY_SECP) && (gNvmeHddSecurityInitProtocol == NULL) \
            && !IsSecurityProtocolInstalled(NvmeController->ControllerHandle, &gAmiTcgStorageSecurityProtocolGuid)) {
            // SAT-3 supported device
            gBS->SetMem(&SecReceiveBuffer, sizeof (NVME_SECURITY_RECEIVE_BUFFER), 0);

            Status = StorageSecurityProtocol->ReceiveData( StorageSecurityProtocol,
                                                           0,                                     // Media ID
                                                           NVME_COMMAND_TIMEOUT * 100000000,      // 10sec timeout
                                                           NVME_SECUIRTY_SECP,                    // Security Protocol ID
                                                           0x00,                                  // SP specific id
                                                           sizeof(NVME_SECURITY_RECEIVE_BUFFER),  // Transfer Length
                                                           &SecReceiveBuffer,
                                                           &SecReceiveBufferSize                  // Transferred Length
                                                           );
            if(!EFI_ERROR(Status)) {
                // Swap bytes for common security status alignment
                NvmeSecurityStatus->SAT3DeviceStatus = (SecReceiveBuffer.NvmeSecStatus >> 8) | \
                                                       (SecReceiveBuffer.NvmeSecStatus << 8);
            }
        }
        
        // Check Security supported Protocol List for Protocol ID 1 and 
        // get the security status only if it is not handled by 
        // HddSecurity and TcgStorageSecurity driver.
        if ((QueryBuffer->SupportedSPList[SpByte] == NVME_SECURITY_PROTOCOL_1) && (gAmiTcgStorageSecurityInitProtocol==NULL)\
            && !IsSecurityProtocolInstalled(NvmeController->ControllerHandle, &gAmiHddSecurityProtocolGuid)) {
            // Check OpalConfig data available or not
            if(NvmeController->OpalConfig == NULL) {
                
                Status = gBS->AllocatePool(EfiBootServicesData,
                                           sizeof(NVME_OPAL_DEVICE),
                                           (VOID**)&(NvmeController->OpalConfig));
                if (EFI_ERROR(Status)) {
                    return Status;
                }
                
                OpalDevice = (NVME_OPAL_DEVICE*)NvmeController->OpalConfig;
                gBS->SetMem(OpalDevice, sizeof (NVME_OPAL_DEVICE), 0);
                OpalDevice->StorageSecurityProtocol = StorageSecurityProtocol;
                
            } else {
                OpalDevice = (NVME_OPAL_DEVICE*)NvmeController->OpalConfig;
            }
            
            // Get Level 0 Discovery data to check whether device already in blocked state
            Status = NvmeGetLevel0DiscoveryData(NvmeController, OpalDevice);
            if(!EFI_ERROR(Status)) {
                NvmeSecurityStatus->TcgDeviceStatus = OpalDevice->LockingFeature;
                
                // Update TCG NVMe device frozen state 
                if(OpalDevice->BlockSid.SIDBlockedState == 1) {
                    NvmeSecurityStatus->TcgDeviceStatus |= BIT3;
                }
            }
        }
    }
    return Status;
}
/**
    Send the Freeze Lock and device config Freeze Lock command to 
    all the NVMe Devices connected 

    @param Event   - signaled event
    @param Context - pointer to event context

    @retval VOID

**/
VOID
EFIAPI
NvmeFreezeLockDevice (
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
    UINTN                                  HandleCount;
    EFI_HANDLE                             *HandleBuffer;
    UINT8                                  i;
    EFI_STATUS                             Status;
    AMI_NVME_CONTROLLER_PROTOCOL           *NvmeController = NULL;
    
    Status = gBS->LocateHandleBuffer (
                                      ByProtocol,
                                      &gAmiNvmeControllerProtocolGuid,
                                      NULL,
                                      &HandleCount,
                                      &HandleBuffer
                                      );
    if(EFI_ERROR(Status) || HandleCount == 0 ) {
        return;
    }
    
    //
    // Issue Freeze Lock command all the Nvme Devices connected.
    //
    for (i = 0; i < HandleCount; i++) {
     
        Status = gBS->HandleProtocol (HandleBuffer[i], 
                                      &gAmiNvmeControllerProtocolGuid, 
                                      (VOID**)&NvmeController);
        if (EFI_ERROR(Status)) {
            continue;
        }
        NvmeSendSecurityFreezeLock(NvmeController);
    }

    // Freeze Lock commands send to the device. Close the event as no more it's needed 
    gBS->CloseEvent(Event);

    return;
}


/**
    Send a security protocol command to a device.

    @param  This - Indicates a pointer to the calling context. Type
        EFI_STORAGE_SECURITY_COMMAND_PROTOCOL is defined in the
        EFI_STORAGE_SECURITY_COMMAND_PROTOCOL description.
    @param  MediaId - ID of the medium to send data to.
    @param  Timeout - The timeout, in 100ns units, to use for the execution of the 
        security protocol command. A Timeout value of 0 means that this function 
        will wait indefinitely for the security protocol command to execute.
    @param  If Timeout is greater than zero, then this function will return 
        EFI_TIMEOUT if the time required to execute the receive data command 
        is greater than Timeout.
    @param  SecurityProtocolId - Security protocol ID of the security protocol 
        command to be sent.
    @param  SecurityProtocolSpecificData - Security protocol specific portion of 
        the security protocol command.
    @param  PayloadBufferSize    - Size in bytes of the payload data buffer.
    @param  PayloadBuffer - A pointer to a buffer containing the security protocol 
        command specific payload data for the security protocol command.

         
    @retval EFI_SUCCESS The security protocol command completed successfully.
    @retval EFI_UNSUPPORTED The given MediaId does not support security protocol 
        commands.
    @retval EFI_DEVICE_ERROR The security protocol command completed with an error.
    @retval EFI_INVALID_PARAMETER The PayloadBuffer or PayloadTransferSize is 
        NULL and PayloadBufferSize is non-zero.
    @retval EFI_MEDIA_CHANGED The MediaId is not for the current media. 

**/

EFI_STATUS 
EFIAPI
NvmeSendData (
    IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
    IN UINT32   MediaId,
    IN UINT64   Timeout,
    IN UINT8    SecurityProtocolId,
    IN UINT16   SecurityProtocolSpecificData,
    IN UINTN    PayloadBufferSize,
    IN VOID     *PayloadBuffer  
)
{
    EFI_STATUS                      Status;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper;
    NVME_DXE_SMM_DATA_AREA          *NvmeDxeSmmDataArea;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    UINTN                           MappedBufferSize = PayloadBufferSize;
    EFI_PHYSICAL_ADDRESS            MappedBuffer;
    VOID                            *BufferUnMap;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;   
    UINTN                           BufferSize = 0;
    
    // Validate the PayloadBuffer for not null, 
    // only if PayloadBufferSize is non zero.
    if (!This || (PayloadBufferSize && !PayloadBuffer) ) {
        return EFI_INVALID_PARAMETER;
    }

    ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_SECURITY_PROTOCOL(This);    
               
    if(ActiveNameSpace->Signature != ACTIVE_NAME_SPACE_SIG) ASSERT(FALSE);              
    
    if (ActiveNameSpace->NvmeBlockIO.Media->MediaId != MediaId) {
        return EFI_MEDIA_CHANGED;
    }
    
    NvmeController = ActiveNameSpace->NvmeController;

    NvmeCmdWrapper = NvmeController->NvmeCmdWrapper;
    NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;

//  Skip MediaID check since controller command
//  Check if Media ID matches
//  if (BlkMedia->MediaId != MediaId) {
//     return EFI_MEDIA_CHANGED;
//  }
    
    gBS->SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0); 
    
    if (PayloadBufferSize) {
        MappedBuffer = (EFI_PHYSICAL_ADDRESS) PayloadBuffer;
        Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                          EfiPciIoOperationBusMasterRead,
                                          PayloadBuffer,
                                          &MappedBufferSize,
                                          &MappedBuffer,
                                          &BufferUnMap
                                          );
        if (EFI_ERROR(Status)) {
            return Status;
        }

        Status = ProgramPRPEntry(NvmeController,
                                NvmeCmdWrapper,
                                MappedBufferSize, 
                                (VOID*)MappedBuffer,
                                ActiveNameSpace,
                                &(ActiveNameSpace->PRP2List),
                                NULL,
                                &BufferSize);
        if (EFI_ERROR(Status)) {
            return EFI_DEVICE_ERROR;
        }
    }
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_SEND;
    
    //Swap SecurityProtocolSpecificData bytes
    NvmeCmdWrapper->NvmCmd.CDW10 = (SecurityProtocolId << 24) + ((SecurityProtocolSpecificData & 0xFF) << 16) +\
                                    (SecurityProtocolSpecificData & 0xFF00);
    NvmeCmdWrapper->NvmCmd.CDW11 = (UINT32)BufferSize;
            
    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = 0; 
        
    NvmeCmdWrapper->SQIdentifier = 0;       // Admin Queue      
    if (Timeout) {
	    // If timeout is less than 1 millisecond, Assign Timeout value to 1 millisecond
        NvmeCmdWrapper->CmdTimeOut = (UINT32)(Timeout < 10000 ? 1 : DivU64x32 (Timeout, 10000));  
    }
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
    
    if(PayloadBufferSize) {
        NvmeController->PciIO->Unmap(NvmeController->PciIO, BufferUnMap);
    }
    
    return Status;
    
}

/**
    Send a security protocol command to a device that receives 
    data and/or the result of one or more commands sent by 
    SendData.

    @param  This -Indicates a pointer to the calling context. Type 
        EFI_STORAGE_SECURITY_COMMAND_PROTOCOL is defined in the
        EFI_STORAGE_SECURITY_COMMAND_PROTOCOL description.
    @param  MediaId - ID of the medium to receive data from. 
    @param  Timeout - The timeout, in 100ns units, to use for the execution of the
        security protocol command. A Timeout value of 0 means that this 
        function will wait indefinitely for the security protocol command
        to execute. If Timeout is greater than zero, then this function 
        will return.
    @param  SecurityProtocolId - Security protocol ID of the security protocol 
        command to be sent.
    @param  SecurityProtocolSpecificData - Security protocol specific portion of 
        the security protocol command.
    @param  PayloadBufferSize - Size in bytes of the payload data buffer.
    @param  PayloadBuffer - A pointer to a destination buffer to store the 
        security protocol command specific payload data for the 
        security protocol command. The caller is responsible 
        for either having implicit or explicit ownership of 
        the buffer.
    @param  PayloadTransferSize - A pointer to a buffer to store the size in bytes
        of the data written to the payload data buffer.

         
    @retval EFI_SUCCESS The security protocol command completed successfully.
    @retval EFI_UNSUPPORTED The given MediaId does not support security 
        protocol commands.
    @retval EFI_DEVICE_ERROR The security protocol command completed with an error.
    @retval EFI_INVALID_PARAMETER The PayloadBuffer or PayloadTransferSize is 
        NULL and PayloadBufferSize is non-zero.
    @retval EFI_MEDIA_CHANGED The MediaId is not for the current media. 
        
**/

EFI_STATUS
EFIAPI
NvmeReceiveData (
    IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
    IN UINT32    MediaId,
    IN UINT64   Timeout,
    IN UINT8     SecurityProtocolId,
    IN UINT16   SecurityProtocolSpecificData,
    IN UINTN    PayloadBufferSize,
    OUT VOID    *PayloadBuffer,
    OUT UINTN   *PayloadTransferSize
)
{
    EFI_STATUS                      Status;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper;
    NVME_DXE_SMM_DATA_AREA          *NvmeDxeSmmDataArea;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    UINTN                           MappedBufferSize = PayloadBufferSize;
    EFI_PHYSICAL_ADDRESS            MappedBuffer;
    VOID                            *BufferUnMap;  
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;    
    UINTN                           BufferSize = 0;
    
    // Validate the PayloadBuffer and PayloadTransferSize for not null,
    // only if PayloadBufferSize is non zero.
    if (!This || (PayloadBufferSize && (!PayloadBuffer || !PayloadTransferSize))) {
        return EFI_INVALID_PARAMETER;
    }

    ActiveNameSpace = ACTIVE_NAMESPACE_DATA_FROM_SECURITY_PROTOCOL(This);
      
    if(ActiveNameSpace->Signature != ACTIVE_NAME_SPACE_SIG) ASSERT(FALSE);    
    
    if (ActiveNameSpace->NvmeBlockIO.Media->MediaId != MediaId) {
        return EFI_MEDIA_CHANGED;
    }
    
    NvmeController = ActiveNameSpace->NvmeController;
    NvmeCmdWrapper = NvmeController->NvmeCmdWrapper;
    NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;

//  Skip MediaID check since controller command
//  Check if Media ID matches
//  if (BlkMedia->MediaId != MediaId) {
//     return EFI_MEDIA_CHANGED;
//  }

    gBS->SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0); 
    
    if(PayloadBuffer) {
        MappedBuffer = (EFI_PHYSICAL_ADDRESS) PayloadBuffer;
        Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                          EfiPciIoOperationBusMasterWrite,
                                          PayloadBuffer,
                                          &MappedBufferSize,
                                          &MappedBuffer,
                                          &BufferUnMap
                                          );
        if (EFI_ERROR(Status)) {
            return Status;
        }
    
        Status = ProgramPRPEntry(NvmeController,
                                NvmeCmdWrapper,
                                MappedBufferSize, 
                                (VOID*)MappedBuffer,
                                ActiveNameSpace,
                                &(ActiveNameSpace->PRP2List),
                                NULL,
                                &BufferSize);
        if (EFI_ERROR(Status)) {
            return EFI_DEVICE_ERROR;
        }
    }
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SECURITY_RECEIVE;
    
    //Swap SecurityProtocolSpecificData bytes
    NvmeCmdWrapper->NvmCmd.CDW10 = (SecurityProtocolId << 24) + ((SecurityProtocolSpecificData & 0xFF) << 16) +\
                                    (SecurityProtocolSpecificData & 0xFF00);
    NvmeCmdWrapper->NvmCmd.CDW11 = (UINT32)BufferSize;
            
    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = 0;  
        
    NvmeCmdWrapper->SQIdentifier = 0;       // Admin Queue
    if (Timeout) {
	    // If timeout is less than 1 millisecond, Assign Timeout value to 1 millisecond
        NvmeCmdWrapper->CmdTimeOut = (UINT32)(Timeout < 10000 ? 1 : DivU64x32 (Timeout, 10000));  
    }
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
    
    if(PayloadBuffer) {
        NvmeController->PciIO->Unmap(NvmeController->PciIO, BufferUnMap);
    }
    
    if (!EFI_ERROR(Status)) {
        *PayloadTransferSize = MappedBufferSize;
    }
    
    return Status;
    
}

/**
    Perform the Shutdown Initialization Sequence for the Nvme Controller 

    1. Stop submitting any new I/O commands to the controller and allow any outstanding commands to complete.
    2. The host should delete all I/O Submission Queues, using the Delete I/O Submission Queue command. 
        A result of the successful completion of the Delete I/O Submission Queue command is that any 
        remaining commands outstanding are aborted.
    3. The host should delete all I/O Completion Queues, using the Delete 
        I/O Completion Queue command.
    4. The host should set the Shutdown Notification (CC.SHN) field to 01b to indicate a normal shutdown operation. 
        The controller indicates when shutdown processing is completed by updating 
        the Shutdown Status (CSTS.SHST) field to 10b.
        
    @param Event   - signaled event
    @param Context - pointer to event context

    @retval EFI_STATUS

**/
VOID
EFIAPI
NvmeShutdown (
    IN EFI_EVENT    Event,
    IN VOID         *Context 
)
{
 
    UINTN                           NvmeHandle;
    UINTN                           Index=0;
    EFI_HANDLE                      *NvmeControllerHandleBuffer;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController = NULL;
    NVME_DXE_SMM_DATA_AREA          *NvmeDxeSmmDataArea = NULL;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper = NULL;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    EFI_STATUS                      Status;
    UINT32                          Delay = 0;

   
    // Locate all the Nvme Controller Protocol
    Status = gBS->LocateHandleBuffer(
                         ByProtocol,
                         &gAmiNvmeControllerProtocolGuid,
                         NULL,
                         &NvmeHandle,
                         &NvmeControllerHandleBuffer
                         );
    if(EFI_ERROR(Status) || NvmeHandle == 0 ) {
        return ;
    }
    
    Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof(NVME_COMMAND_WRAPPER),
                  (VOID**)&NvmeCmdWrapper
                  );

    ASSERT_EFI_ERROR(Status);
    if(EFI_ERROR(Status)) {
        return ;
    }
    

    for (Index = 0; Index < NvmeHandle; Index++) {
    
        // Get the Nvme Controller Protocol Interface
        Status = gBS->HandleProtocol(
                                NvmeControllerHandleBuffer[Index],
                                &gAmiNvmeControllerProtocolGuid,
                                (VOID *)&NvmeController
                                );
                
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) {
            continue;
        }

        NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;

        // Clear memory
        SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
        
        // Delete all I/O Submission Queues, using the Delete I/O Submission Queue command
        // Build NVME command to delete Submission queue
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = DELETE_IO_SUBMISSION_QUEUE;
        NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
        NvmeCmdWrapper->NvmCmd.CDW10 = NvmeController->NVMQueueNumber;
        
        NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
        NvmeCmdWrapper->SQIdentifier = 0;           // Queue 0 for Admin cmds
        NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
        
        Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
        if (EFI_ERROR(Status)) {
            continue;
        }

        // Clear memory
        SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);

        // Delete all I/O Completion Queues, using the Delete I/O Completion Queue command
        // Build NVME command to delete Completion queue
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = DELETE_IO_COMPLETION_QUEUE;
        NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
        NvmeCmdWrapper->NvmCmd.CDW10 = NvmeController->NVMQueueNumber;
        
        NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
        NvmeCmdWrapper->SQIdentifier = 0;           // Queue 0 for Admin cmds
        NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
        
        Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        // Set the Shutdown Notification (CC.SHN) field to 01b to indicate a normal shutdown operation
        CONTROLLER_REG32_OR(NvmeController, Offset_CC, 0x4000);
        
        if (NvmeController->IdentifyControllerData != NULL) {
            Delay = (NvmeController->IdentifyControllerData->RTD3E) / 1000;
            if (Delay != 0) {
                if (((NvmeController->IdentifyControllerData->RTD3E) % 1000) != 0 ) {
                    Delay ++;
                }
            }
        }
        if (Delay == 0) {
            Delay = NvmeController->TimeOut * 500;
        }
        
        // Check if the shutdown process is completed  
        do {
         
            if ((CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0xC) == 8) {
                break;
            }
            gBS->Stall(1000); // 1msec delay
            
        }while (--Delay);
        
        if (!Delay) {
            ASSERT (FALSE);
        }
    }
    
    gBS->FreePool(NvmeCmdWrapper);
    gBS->FreePool(NvmeControllerHandleBuffer);
    
    return ;
}

/**
    Prints Identify data structure

    @param IdentifyData, 
    @param ControllerNameSpaceStructure
    
    @retval VOID

    @note  The amount of data that will get printed can be controlled 
    using DEBUG_ERROR_LEVEL_MASK SDL token. 
    Make sure PcdDebugPrintErrorLevel is properly cloned and set to 
    PcdsPatchableInModule in the project.

**/

VOID
PrintIdentifyDataStructure (
    IN  UINT8   *IdentifyData, 
    IN  UINT8   ControllerNameSpaceStructure
)
{

#if  NVME_VERBOSE_PRINT
    
    IDENTIFY_CONTROLLER_DATA    *IdentifyControllerData = (IDENTIFY_CONTROLLER_DATA *)IdentifyData;
    IDENTIFY_NAMESPACE_DATA     *IdentifyNameSpaceData = (IDENTIFY_NAMESPACE_DATA *)IdentifyData;
    UINT32                      *ActiveNameSapceID = (UINT32 *)IdentifyData;
    UINT32                      i;
    UINT64                      FirmwareVersion;
    
    switch (ControllerNameSpaceStructure){
    
        case 1:
            
            FirmwareVersion = *(UINT64 *)(IdentifyControllerData->FirmwareRevision);
            DEBUG((DEBUG_BLKIO, "\nIdentify Controller Data Structure\n"));
            DEBUG((DEBUG_BLKIO, "Vendor ID                                 : %08X\n", IdentifyControllerData->VID));
            DEBUG((DEBUG_BLKIO, "SubSystem Vendor ID                       : %08X\n", IdentifyControllerData->SSVID));
            DEBUG((DEBUG_BLKIO, "Firmware Version                          : %16lX\n", FirmwareVersion));
            DEBUG((DEBUG_BLKIO, "NameSpace Sharing Capability              : %08X\n", IdentifyControllerData->CMIC));
            DEBUG((DEBUG_BLKIO, "Max. Data Transfer Size                   : %08X\n", IdentifyControllerData->MDTS));
            DEBUG((DEBUG_BLKIO, "Controller ID                             : %08X\n", IdentifyControllerData->ControllerID));
            DEBUG((DEBUG_BLKIO, "Optional Admin Cmd Support                : %08X\n", IdentifyControllerData->OACS));
            DEBUG((DEBUG_BLKIO, "Abort Command Limit                       : %08X\n", IdentifyControllerData->ACL));
            DEBUG((DEBUG_BLKIO, "Asyn. Event Request Limit                 : %08X\n", IdentifyControllerData->AERL));
            DEBUG((DEBUG_BLKIO, "Firmware Updates                          : %08X\n", IdentifyControllerData->FRMW));
            DEBUG((DEBUG_BLKIO, "Log Page Attribute                        : %08X\n", IdentifyControllerData->LPA));
            DEBUG((DEBUG_BLKIO, "# of Power state supported                : %08X\n", IdentifyControllerData->NPSS));
            DEBUG((DEBUG_BLKIO, "Admin Vendor Specific cmd                 : %08X\n", IdentifyControllerData->AVSCC));
            DEBUG((DEBUG_BLKIO, "Autonomous Power state attrib             : %08X\n", IdentifyControllerData->APSTA));
            DEBUG((DEBUG_BLKIO, "Submission queue Entry Size               : %08X\n", IdentifyControllerData->SQES));
            DEBUG((DEBUG_BLKIO, "Completion queue Entry Size               : %08X\n", IdentifyControllerData->CQES));
            DEBUG((DEBUG_BLKIO, "Number of NameSpace                       : %08X\n", IdentifyControllerData->NN));
            DEBUG((DEBUG_BLKIO, "Optional NVM Command Support              : %08X\n", IdentifyControllerData->ONCS));
            DEBUG((DEBUG_BLKIO, "Fused Operation Support                   : %08X\n", IdentifyControllerData->FUSES));
            DEBUG((DEBUG_BLKIO, "Format NVM Attribute                      : %08X\n", IdentifyControllerData->FNA));
            DEBUG((DEBUG_BLKIO, "Volatile Write Cache                      : %08X\n", IdentifyControllerData->VWC));
            DEBUG((DEBUG_BLKIO, "Atomic Write Unit Normal                  : %08X\n", IdentifyControllerData->AWUN));
            DEBUG((DEBUG_BLKIO, "Atomic Write Unit Power Fail              : %08X\n", IdentifyControllerData->AWUPF));
            DEBUG((DEBUG_BLKIO, "NVM VS CMD Config                         : %08X\n", IdentifyControllerData->NVSCC));
            DEBUG((DEBUG_BLKIO, "Atomic Compare & Write Unit               : %08X\n", IdentifyControllerData->ACWU));
            DEBUG((DEBUG_BLKIO, "SGL Support                               : %08X\n", IdentifyControllerData->SGLS));
            break;
            
        case 0:
            DEBUG((DEBUG_BLKIO, "\nIdentify NameSpace Data Structure\n"));
            DEBUG((DEBUG_BLKIO, "NameSpace Size                             : %08X\n", IdentifyNameSpaceData->NSIZE));
            DEBUG((DEBUG_BLKIO, "NameSpace Capacity                         : %08X\n", IdentifyNameSpaceData->NCAP));
            DEBUG((DEBUG_BLKIO, "NameSpace Utilization                      : %08X\n", IdentifyNameSpaceData->NUSE));
            DEBUG((DEBUG_BLKIO, "NameSpace Features                         : %08X\n", IdentifyNameSpaceData->NSFEAT));
            DEBUG((DEBUG_BLKIO, "No. of LBA Formats (0 Based)               : %08X\n", IdentifyNameSpaceData->NLBAF));
            DEBUG((DEBUG_BLKIO, "Formatted LBA Size                         : %08X\n", IdentifyNameSpaceData->FLBAS));
            DEBUG((DEBUG_BLKIO, "MetaData Capabilities                      : %08X\n", IdentifyNameSpaceData->MC));
            DEBUG((DEBUG_BLKIO, "End-to-end data Protection cap             : %08X\n", IdentifyNameSpaceData->DPC));
            DEBUG((DEBUG_BLKIO, "End-to-end data Protection settings        : %08X\n", IdentifyNameSpaceData->DPS));
            DEBUG((DEBUG_BLKIO, "NameSpace Sharing                          : %08X\n", IdentifyNameSpaceData->NMIC));
            DEBUG((DEBUG_BLKIO, "Reservation Capabilities                   : %08X\n", IdentifyNameSpaceData->RESCAP));
            DEBUG((DEBUG_BLKIO, "IEEE Extended Unique Identifier            : %016lx\n", IdentifyNameSpaceData->EUI64));
            for (i=0; i<16; i++){
                DEBUG((DEBUG_BLKIO, "LBA Format %02X Support                : %08X\n", i, *(UINT32 *)&(IdentifyNameSpaceData->LBAF[i])));
            }
            break;
            
        case 2:
            DEBUG((DEBUG_BLKIO,"\nActive NameSpace IDs\n"));
            for (i=0; i < 1024 && ActiveNameSapceID[i]; i++) {
                DEBUG((DEBUG_BLKIO, "    %08X\n",ActiveNameSapceID[i]));
            }
            
    }
#endif   
    return;
}

#if DO_NOT_LAUNCH_NVME_OPROM
/**
    Locates AMI_OPTION_ROM_POLICY_PROTOCOL and disables OptionRom launching
    
    @param Event   - Signaled event
    @param Context - Pointer to event context

    @retval VOID
**/
VOID
EFIAPI
NvmeOpromPolicy (
    EFI_EVENT  Event,
    VOID       *Context
)
{
    EFI_STATUS                     Status;
    EFI_HANDLE                     Handle;
    UINTN                          BufferSize = sizeof(Handle);
    AMI_OPTION_ROM_POLICY_PROTOCOL *OpRomPolicy = NULL;
    EFI_PCI_IO_PROTOCOL            *PciIo = NULL;
    PCI_TYPE00                     PciConfig;
    
    // Locate the handle for newly installed PciIo Protocol
    Status = gBS->LocateHandle( ByRegisterNotify, 
                                NULL, 
                                NvmePciIoRegistration, 
                                &BufferSize, 
                                &Handle
                               );
    if (EFI_ERROR(Status)) {
        return;
    }
    
    // Check whether PciIo protocol installed for NVMe controller
    Status = gBS->HandleProtocol( Handle, 
                                  &gEfiPciIoProtocolGuid, 
                                  &PciIo
                                 );
    if (EFI_ERROR(Status)) {
        return;
    }
    
    Status = PciIo->Pci.Read ( PciIo,
                               EfiPciIoWidthUint8,
                               0,
                               sizeof (PCI_TYPE00),
                               &PciConfig
                              );
    if (EFI_ERROR(Status)) {
        return;
    }

    // Check for MASS Storage controller, Non-Volatile and NVMHCI interface
    // Return if not a NVMe controller 
    if (!IS_CLASS3 (&PciConfig, 
                    PCI_CLASS_MASS_STORAGE, 
                    PCI_CLASS_MASS_STORAGE_SOLID_STATE, 
                    PCI_IF_MASS_STORAGE_SOLID_STATE_ENTERPRISE_NVMHCI)) {
        return;
    }
    
    Status = gBS->HandleProtocol( Handle, 
                                  &gAmiOptionRomPolicyProtocolGuid, 
                                  &OpRomPolicy
                                );
    if (EFI_ERROR(Status)) {
        return;
    }
    
    // Disable UEFI and Legacy OpRoms
    OpRomPolicy->SetOptionRomPolicy( OpRomPolicy, AMI_OPTION_ROM_POLICY_DISABLE );
    
    return;
}
#endif

#if NVMe_SMM_SUPPORT

/**
    Transfer the Nvme Controller information to SMRAM area that would be used
    by the Nvme SMM driver 

    @param CommunicateBufferSize 

    @retval None

**/

VOID
TransferNvmeDataToSmram (
    IN  UINTN    CommunicateBufferSize
)
{
    EFI_STATUS                          SmmStatus;
    EFI_SMM_COMMUNICATION_PROTOCOL     *SmmCommunication;
    EFI_SMM_COMMUNICATE_HEADER         *CommHeader;
    UINTN                              NvmeCommunicateBufferSize;

    NvmeCommunicateBufferSize  = sizeof(EFI_GUID) + sizeof(UINTN) + CommunicateBufferSize;
    
    // Get needed resource
    SmmStatus = gBS->LocateProtocol (
                    &gEfiSmmCommunicationProtocolGuid,
                    NULL,
                    (VOID **)&SmmCommunication
                    );
    if (EFI_ERROR (SmmStatus)) {
        return;
    }

    CommHeader = (EFI_SMM_COMMUNICATE_HEADER *)gNvmeCommunicateBuffer;
    
    gBS->CopyMem (&CommHeader->HeaderGuid, &gAmiSmmNvmeCommunicationGuid, sizeof(gAmiSmmNvmeCommunicationGuid));
    
    CommHeader->MessageLength = CommunicateBufferSize;
    
    SmmStatus = SmmCommunication->Communicate ( SmmCommunication,
                                                gNvmeCommunicateBuffer,
                                                &NvmeCommunicateBufferSize );
    ASSERT_EFI_ERROR(SmmStatus);
    return;

}

/**
    Add the NVMe active partition as a legacy device 

    @param NvmeController 

    @retval None

**/

VOID
InstallNvmeLegacyDevice (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  ACTIVE_NAMESPACE_DATA           *ActiveNameSpace
)
{
    EFI_STATUS                      Status;
    AMI_NVME_LEGACY_PROTOCOL        *AmiNvmeLegacyProtocol = NULL;
    NVME_LEGACY_MASS_DEVICE         *NvmeLegacyMassDevice = NULL;
    NVME_MASS_DEV_INFO              *NvmeMassDevInfo = NULL;
    CHAR8                           *NewString = NULL;
    UINTN                           PciSeg, PciBus, PciDev, PciFunc;
    
    //If block size is not 512 do install INT13
    if (ActiveNameSpace->NvmeBlockIO.Media->BlockSize != 0x200) {
        return;
    }
    
    Status = gBS->LocateProtocol(&gAmiNvmeLegacyProtocolGuid, NULL, (VOID**)&AmiNvmeLegacyProtocol);
    // Legacy Support disabled. 
    if(EFI_ERROR(Status) || AmiNvmeLegacyProtocol == NULL ) {
        return;
    }
        
    Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        sizeof (NVME_LEGACY_MASS_DEVICE),
                        (VOID**)&NvmeLegacyMassDevice
                        );

    if (EFI_ERROR(Status)) {
        goto InstallNvmeLegacyDevice_Error_Path;
    }

    Status = gBS->AllocatePool (
                        EfiBootServicesData,
                        sizeof (NVME_MASS_DEV_INFO),
                        (VOID**)&NvmeMassDevInfo
                        );

    if (EFI_ERROR(Status)) {
        goto InstallNvmeLegacyDevice_Error_Path;
    }

    Status = gBS->AllocatePool(
                      EfiBootServicesData, 
                      256,
                      (VOID *) &NewString);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR(Status)) {
        goto InstallNvmeLegacyDevice_Error_Path;
    }

    // Read the first sector of the device
    ActiveNameSpace->dMaxLBA = ActiveNameSpace->NvmeBlockIO.Media->LastBlock;
    GetNvmeGeometryData(ActiveNameSpace, NvmeMassDevInfo);

    NvmeMassDevInfo->wBlockSize=ActiveNameSpace->NvmeBlockIO.Media->BlockSize;
    NvmeMassDevInfo->dMaxLba=ActiveNameSpace->dMaxLBA;     
    NvmeMassDevInfo->bHeads=ActiveNameSpace->NumHeads;
    NvmeMassDevInfo->bSectors=ActiveNameSpace->NumSectors;
    NvmeMassDevInfo->wCylinders=ActiveNameSpace->NumCylinders;
    NvmeMassDevInfo->bNonLBAHeads=ActiveNameSpace->LBANumHeads;
    NvmeMassDevInfo->bNonLBASectors=ActiveNameSpace->LBANumSectors;
    NvmeMassDevInfo->wNonLBACylinders=ActiveNameSpace->LBANumCyls;

    PrintNvmeMassDevInfo (NvmeMassDevInfo);

    // Get the NVMe controller Bus,Dev and Fun
    NvmeController->PciIO->GetLocation (NvmeController->PciIO, &PciSeg, &PciBus, &PciDev, &PciFunc);
    UnicodeStrToAsciiStr (NvmeController->UDeviceName->UnicodeString, NewString);

    NvmeLegacyMassDevice->PciBDF=(UINT16)((PciBus << 8)+(PciDev << 3) + PciFunc);
    NvmeLegacyMassDevice->DevString=NewString;
    NvmeLegacyMassDevice->DevInfo=(VOID*)NvmeMassDevInfo;
    ActiveNameSpace->Int13DeviceAddress = gInt13DeviceAddress;
    NvmeLegacyMassDevice->LogicalAddress=ActiveNameSpace->Int13DeviceAddress;
    gInt13DeviceAddress++;
    NvmeLegacyMassDevice->StorageType = NVME_MASS_DEV_HDD;
    NvmeLegacyMassDevice->Handle=ActiveNameSpace->NvmeDeviceHandle;

    AmiNvmeLegacyProtocol->AddNvmeLegacyDevice(NvmeLegacyMassDevice);
    return;

InstallNvmeLegacyDevice_Error_Path:

    // Free the pre-allocated memory during error path 
    if(NewString) {
        gBS->FreePool(NewString);
    }
    if(NvmeMassDevInfo) {
        gBS->FreePool(NvmeMassDevInfo);
    }
    if(NvmeLegacyMassDevice) {
        gBS->FreePool(NvmeLegacyMassDevice);
    }   

    DEBUG((EFI_D_ERROR,"\n InstallNvmeLegacyDevice Ends with Status : %r \n",Status));
    return;
}

/**
    Get drive geometry data for INT13 support

    @param Buffer, 
    @param NvmeMassDevInfo
    
    @retval VOID

    @note  
    
**/

EFI_STATUS
GetNvmeGeometryData (
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  OUT NVME_MASS_DEV_INFO      *NvmeMassDevInfo
)
{

    EFI_STATUS  Status;
    UINT8       *ActPartAddr;
    UINT32      dHS;
    UINT8       *Buffer;
    
    ActiveNameSpace->bStorageType = NVME_MASS_DEV_HDD;
    ActiveNameSpace->bEmuType = 0;
    
    ActiveNameSpace->bHiddenSectors = 0;
    
    Status = gBS->AllocatePages (
                       AllocateAnyPages,
                       EfiBootServicesData,
                       EFI_SIZE_TO_PAGES(ActiveNameSpace->NvmeBlockIO.Media->BlockSize),  
                       (EFI_PHYSICAL_ADDRESS*)&(Buffer));
    if (EFI_ERROR(Status)) {
       return Status;
    }
    
    // Read LBA-0 data
    NvmeReadWriteBlocks (ActiveNameSpace, 
                          ActiveNameSpace->NvmeBlockIO.Media->MediaId, 
                          (EFI_LBA)0, 
                          ActiveNameSpace->NvmeBlockIO.Media->BlockSize,
                          Buffer, 
                          NULL, 
                          NVME_READ);
    
    //
    // Check for validity of Boot Record
    //
    if ( *(UINT16*)(Buffer + 0x1FE) != 0xAA55 ) {
        NvmeSetDefaultGeometry(ActiveNameSpace);
        gBS->FreePages((EFI_PHYSICAL_ADDRESS)Buffer, EFI_SIZE_TO_PAGES(ActiveNameSpace->NvmeBlockIO.Media->BlockSize));
        return EFI_NOT_FOUND;
    }

    //
    // Check for validity of the partition table
    //
    if ( NvmeValidatePartitionTable( Buffer, ActiveNameSpace->dMaxLBA, &ActPartAddr) == EFI_SUCCESS ) {

        //
        // Read boot sector, set the LBA number to boot record LBA number
        //
        dHS = *((UINT32*)(ActPartAddr + 8));
        ActiveNameSpace->bHiddenSectors = (UINT8)dHS; // Save hidden sector value

        Status = NvmeReadWriteBlocks (ActiveNameSpace, 
                                      ActiveNameSpace->NvmeBlockIO.Media->MediaId, 
                                      (EFI_LBA)dHS, 
                                      ActiveNameSpace->NvmeBlockIO.Media->BlockSize, 
                                      Buffer, 
                                      NULL, 
                                      NVME_READ);

        if ( EFI_ERROR(Status) ) {  
            gBS->FreePages((EFI_PHYSICAL_ADDRESS)Buffer, EFI_SIZE_TO_PAGES(ActiveNameSpace->NvmeBlockIO.Media->BlockSize));
            return EFI_NOT_FOUND;
        }

        if ( NvmeUpdateCHSFromBootRecord(ActiveNameSpace, Buffer) != EFI_SUCCESS) {
            NvmeSetDefaultGeometry(ActiveNameSpace);
            ActiveNameSpace->bHiddenSectors = 0;      // Reset hidden sector value         
        }
    }
    
    gBS->FreePages((EFI_PHYSICAL_ADDRESS)Buffer, EFI_SIZE_TO_PAGES(ActiveNameSpace->NvmeBlockIO.Media->BlockSize));
    return EFI_SUCCESS;
}
    
/**
    This procedure sets the  default geometry for mass storage devices

    @param ActiveNameSpace 

    @retval EFI_STATUS

    @note

**/

EFI_STATUS 
NvmeSetDefaultGeometry ( 
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace
)
{
    ActiveNameSpace->NumHeads = 0xFF;
    ActiveNameSpace->LBANumHeads = 0xFF;
    ActiveNameSpace->NumSectors = 0x3F;
    ActiveNameSpace->LBANumSectors = 0x3F;

    NvmeMassUpdateCylinderInfo(ActiveNameSpace);
    return EFI_SUCCESS;
}

/**
    This procedure updates cylinder parameter for device geometry.
    head and sector parameters are required before invoking this
    function.

    @param ActiveNameSpace 
    
    @retval VOID

    @note

**/

VOID
NvmeMassUpdateCylinderInfo (
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace
)
{
    UINT32 data = (UINT32)(DivU64x64Remainder(ActiveNameSpace->dMaxLBA,(ActiveNameSpace->NumSectors * ActiveNameSpace->NumHeads), NULL));

    if (data <= 1) data++;
    if (data > 0x3FF) data = 0x3FF;   // DOS workaround

    ActiveNameSpace->NumCylinders = (UINT16)data;
    ActiveNameSpace->LBANumCyls = (UINT16)data;
    return;
}

/**
    This procedure checks whether the partition table is valid

    @param Buffer 
    @param dMaxLBA 
    @param ActPartAddr 

    @retval VOID

    @note

**/

EFI_STATUS
NvmeValidatePartitionTable (
    IN  UINT8   *Buffer,
    IN  UINT64  dMaxLBA,
    OUT UINT8   **ActPartAddr
)
{

    UINT8   *PartPtr;
    UINT8   PartNo = 0;
    UINT8   *ActPart = NULL;
    UINT8   ValidEntryCount = 0;

    //
    // Drive has a partition table, start from 1st bootable partition
    //
    PartPtr = Buffer + 0x1BE;

    for (; PartNo<4; PartNo++, PartPtr+=0x10 ) {

        if (*PartPtr & 0x7F ) { 
            return EFI_NOT_FOUND; //BootFlag should be 0x0 or 0x80
        }
        //
        // Check whether beginning LBA is reasonable
        //
        if (*(UINT32*)(PartPtr + 8) > dMaxLBA) { 
            return EFI_NOT_FOUND;
        }
        
        ValidEntryCount++;                      // Update valid entry count
        //
        // Update active entry offset
        //
        if (!(*PartPtr & 0x80)) continue;
        if (ActPart) continue;
        ActPart = PartPtr;
    }

    if (ValidEntryCount < 1) return EFI_NOT_FOUND; // At least one valid partition is found
    //
    // If no active partition table entry found use first entry
    //
    if (ActPart == NULL) ActPart = Buffer + 0x1BE;

    *ActPartAddr = ActPart;

    return EFI_SUCCESS;

}

/**
    This function parses the boot record and extract the CHS
    information of the formatted media from the boot record.
    This routine checks for DOS & NTFS formats only

    @param IdentifyData, 
    @param ControllerNameSpaceStructure
    
    @retval VOID

    @note  The amount of data that will get printed can be controlled 
    using DEBUG_ERROR_LEVEL_MASK SDL token. 
    Make sure PcdDebugPrintErrorLevel is properly cloned and set to 
    PcdsPatchableInModule in the project.

**/

EFI_STATUS
NvmeUpdateCHSFromBootRecord (
    IN  OUT ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  UINT8*                      BootRecord
)
{
    UINT32      OemName;
    UINT8       Heads;
    UINT8       SecPerTrack;
    UINT16      SecTimesHeads;
    UINT16      TotalSect;

    if (*((UINT16*)(BootRecord + 0x1FE)) != 0xAA55) { 
        return EFI_NOT_FOUND;
    }

    //
    // Read succeeded so the drive is formatted
    // Check for valid MSDOS/MSWIN/NTFS boot record
    //
    OemName = *(UINT32*)(BootRecord + 3);
    
    if ((OemName != 0x4F44534D) &&   // 'ODSM' for MSDO
        (OemName != 0x4957534D) &&   // 'IWSM' for MSWI
        (OemName != 0x5346544E)) {   // 'SFTN' for NTFS
        //
        // Check for valid FAT,FAT16,FAT32 boot records
        //
        *(BootRecord + 0x36 + 3) = 0x20;              // Ignore the 4th byte and fill it with space
        if ((*(UINT32*)(BootRecord + 0x36) != 0x20544146) &&      // " TAF" for FATx
            (*(UINT32*)(BootRecord + 0x52) != 0x33544146)) {      // "3TAF" for FAT3
            //
            // Boot Record is invalid. Return with error
            //
            return EFI_NOT_FOUND;
        }
    }

    Heads = *(BootRecord + 0x1A);         // Number of heads
    SecPerTrack = *(BootRecord + 0x18);   // Sectors/track
    SecTimesHeads = Heads * SecPerTrack;

    // Zero check added to prevent invalid sector/head information in BPB
    if (SecTimesHeads == 0) {
        return EFI_NOT_FOUND;
    }

    TotalSect = *(UINT16*)(BootRecord + 0x13);
    if ( TotalSect ) {
        ActiveNameSpace->dMaxLBA = TotalSect;
    }

    ActiveNameSpace->NumHeads = Heads;
    ActiveNameSpace->LBANumHeads = Heads;
    ActiveNameSpace->NumSectors = SecPerTrack;
    ActiveNameSpace->LBANumSectors = SecPerTrack;


    NvmeMassUpdateCylinderInfo(ActiveNameSpace);

    return  EFI_SUCCESS;
}

/**
    Install the SW SMI handler for the Legacy Nvme Function only when the 
    BIOS supports the Legacy BIOS Mode 
    
    @param Event   - signaled event
    @param Context - pointer to event context

    @retval   VOID
**/

VOID
EFIAPI
InstallSwSmiHandlerForLegacy (
    EFI_EVENT  Event,
    VOID      *Context
)
{
 
    EFI_STATUS                          Status;
    EFI_SMM_COMMUNICATION_PROTOCOL      *SmmCommunication;
    EFI_SMM_COMMUNICATE_HEADER          *CommHeader;
    UINT64                              NvmeSwSmiSignature = SIGNATURE_64 ('S', 'W', 'S', 'M', 'I', 0, 0, 0);
    EFI_LEGACY_BIOS_PROTOCOL            *LegacyBios;
    UINTN                               NvmeCommunicateBufferSize;

    NvmeCommunicateBufferSize  = sizeof(EFI_GUID) + sizeof(UINTN) + sizeof(NvmeSwSmiSignature);
    
    gBS->SetMem( gNvmeCommunicateBuffer, NvmeCommunicateBufferSize, 0 );
    
    // Check if the CSM/Legacy Function supported in BIOS
    Status = gBS->LocateProtocol(&gEfiLegacyBiosProtocolGuid, NULL, (VOID**)&LegacyBios);
    if(EFI_ERROR(Status)) { 
        // Legacy Not supported. Doesn't need to register the SW SMI
        return;
    }
  
    // Get the SmmCommunication Protocol 
    Status = gBS->LocateProtocol (
                       &gEfiSmmCommunicationProtocolGuid,
                       NULL,
                       (VOID **)&SmmCommunication
                       );
    
    if (EFI_ERROR (Status)) {
       return;
    }
    
    CommHeader = (EFI_SMM_COMMUNICATE_HEADER *)gNvmeCommunicateBuffer;
	
    gBS->CopyMem (&CommHeader->HeaderGuid, &gAmiSmmNvmeCommunicationGuid, sizeof(gAmiSmmNvmeCommunicationGuid));
        
    // Copy the Signature to Identify to differentiate between different handlers 
    CommHeader->MessageLength = sizeof(NvmeSwSmiSignature);
    
    gBS->CopyMem (&(CommHeader->Data[0]), &NvmeSwSmiSignature, sizeof(NvmeSwSmiSignature));
        
    SmmCommunication->Communicate (
                        SmmCommunication,
                        gNvmeCommunicateBuffer,
                        &NvmeCommunicateBufferSize
                        );
    
    gBS->CloseEvent(Event);
    
    return;
}

/**
    Callback function to initialize the INT13 drive information and Nvme Int13 API.
    Also transfers the Nvme Controller information to SMRAM area that would be used
    by the Nvme SMM driver.

    
    @param Event   - signaled event
    @param Context - pointer to event context

    @retval   VOID
**/
VOID
EFIAPI
NvmeAllDriverConnectedCallback (
    EFI_EVENT  Event,
    VOID      *Context
)
{
    UINT32                          Index;
    UINTN                           HandleCount;
    EFI_HANDLE                      *HandleBuffer;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController=NULL;
    EFI_STATUS                      Status;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace;
    LIST_ENTRY                      *LinkData;
    ACTIVE_NAMESPACE_DATA           *pActiveNameSpace = NULL;
    AMI_NVME_CONTROLLER_PROTOCOL    *pNvmeController = NULL;
    NVME_CONTROLLER_DATA_TO_SMM	    *NvmeSmmData = NULL;
    UINT64                          DxeToSmmSignature = SIGNATURE_64 ('D', 'X', 'E', 'T', 'O', 'S', 'M', 'M');
    UINTN                           CommunicateBufferSize = 0;
    
    Status = gBS->LocateHandleBuffer(
                      ByProtocol,
                      &gAmiNvmeControllerProtocolGuid,
                      NULL,
                      &HandleCount,
                      &HandleBuffer
                     );
    if ( EFI_ERROR( Status )) {
        gBS->CloseEvent(Event);
        return;
    }
    
    // Get NVME_CONTROLLER_DATA_TO_SMM structure location from global communication buffer
    NvmeSmmData = (NVME_CONTROLLER_DATA_TO_SMM*)((UINT8*)gNvmeCommunicateBuffer + sizeof(EFI_GUID) + sizeof(UINTN));
    
    // Limit controller data transfer to SMM using NVMEDEVS_MAX_ENTRIES
    for(Index = 0; (Index < HandleCount) && (Index < NVMEDEVS_MAX_ENTRIES); Index++) {
        Status = gBS->HandleProtocol( HandleBuffer[Index],
                                      &gAmiNvmeControllerProtocolGuid,
                                      (VOID **) &NvmeController
                                     );
        if (EFI_ERROR(Status)) {
            continue;
        }
        
        pNvmeController = (AMI_NVME_CONTROLLER_PROTOCOL *)NvmeSmmData->Data;
        
        // Data size which is going to transfer to SMM
        CommunicateBufferSize = OFFSET_OF (NVME_CONTROLLER_DATA_TO_SMM, Data) + sizeof(AMI_NVME_CONTROLLER_PROTOCOL);
        gBS->SetMem( NvmeSmmData, CommunicateBufferSize, 0 );

        NvmeSmmData->Signature = DxeToSmmSignature;
        NvmeSmmData->ControllerNumber = Index;
        NvmeSmmData->NameSpaceId = 0;   // Used to denote controller data going to transfer
        NvmeSmmData->LastNameSpace = FALSE;
        
        // Copy NvmeController data directly to communicate buffer
        gBS->CopyMem(pNvmeController, NvmeController, sizeof(AMI_NVME_CONTROLLER_PROTOCOL));
        
        gBS->CopyMem(gNvmeIdentifyControllerData, NvmeController->IdentifyControllerData, sizeof(IDENTIFY_CONTROLLER_DATA));
        pNvmeController->IdentifyControllerData = (IDENTIFY_CONTROLLER_DATA *)gNvmeIdentifyControllerData;
        pNvmeController->LegacyNvmeBuffer = gLegacyNvmeBuffer;
        
        // Transfer Controller data to SMM
        TransferNvmeDataToSmram(CommunicateBufferSize);
        
        // Fill NameSpace Data
        pActiveNameSpace = (ACTIVE_NAMESPACE_DATA *)NvmeSmmData->Data;
        gBS->SetMem( pActiveNameSpace, sizeof(ACTIVE_NAMESPACE_DATA), 0 );
        
        CommunicateBufferSize = OFFSET_OF (NVME_CONTROLLER_DATA_TO_SMM, Data) + sizeof(ACTIVE_NAMESPACE_DATA);
        
        for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
               LinkData != &NvmeController->ActiveNameSpaceList; 
               LinkData = LinkData->ForwardLink) {
            
            ActiveNameSpace = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
            
            if (ActiveNameSpace->IdentifyNamespaceData.NSIZE) {
                // No error check for Int13 installation
                InstallNvmeLegacyDevice(NvmeController, ActiveNameSpace);
            }
            
            NvmeSmmData->NameSpaceId = ActiveNameSpace->ActiveNameSpaceID;
            
            // Copy ActiveNameSpace data directly to communicate buffer
            gBS->CopyMem(pActiveNameSpace, ActiveNameSpace, sizeof(ACTIVE_NAMESPACE_DATA));
            
            gBS->CopyMem(gNvmeActiveNsBlkMedia, ActiveNameSpace->NvmeBlockIO.Media, sizeof(EFI_BLOCK_IO_MEDIA));
            pActiveNameSpace->NvmeBlockIO.Media = (EFI_BLOCK_IO_MEDIA *)gNvmeActiveNsBlkMedia;
            
            // Check whether the current ActiveNameSpace is the last ActiveNameSpace in List
            if(LinkData->ForwardLink == &NvmeController->ActiveNameSpaceList) {
                NvmeSmmData->LastNameSpace = TRUE;
            }
            
            // Transfer ActiveNameSpace data to SMM
            TransferNvmeDataToSmram(CommunicateBufferSize);
        }
    }
        
    gBS->FreePool(HandleBuffer);
    gBS->CloseEvent(Event);
}

VOID
PrintNvmeMassDevInfo (
    NVME_MASS_DEV_INFO              *NvmeMassDevInfo
)
{
    
#if  NVME_VERBOSE_PRINT    
    DEBUG((DEBUG_BLKIO, "********** NvmeMassDevInfo **********\n"));
    DEBUG((DEBUG_BLKIO, "wBlockSize                                  : %08X\n", NvmeMassDevInfo->wBlockSize));
    DEBUG((DEBUG_BLKIO, "dMaxLba                                     : %lX\n",  NvmeMassDevInfo->dMaxLba));
    DEBUG((DEBUG_BLKIO, "bHeads                                      : %08X\n", NvmeMassDevInfo->bHeads));
    DEBUG((DEBUG_BLKIO, "bSectors                                    : %08X\n", NvmeMassDevInfo->bSectors));
    DEBUG((DEBUG_BLKIO, "wCylinders                                  : %08X\n", NvmeMassDevInfo->wCylinders));
    DEBUG((DEBUG_BLKIO, "bNonLBAHeads                                : %08X\n", NvmeMassDevInfo->bNonLBAHeads));
    DEBUG((DEBUG_BLKIO, "bNonLBASectors                              : %08X\n", NvmeMassDevInfo->bNonLBASectors));
    DEBUG((DEBUG_BLKIO, "wNonLBACylinders                            : %08X\n", NvmeMassDevInfo->wNonLBACylinders));
#endif  
}
#endif

/**@internal    
    Function to program Software Progress Marker Feature
    
    @param    VOID
    
    @retval   VOID
    @endinternal
**/
VOID
ProgramSoftwareProgressMarker ( 
) 
{    
    UINTN                           NvmeHandleCount;
    UINTN                           Index=0;
    EFI_HANDLE                      *NvmeControllerHandleBuffer;
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController = NULL;
    NVME_COMMAND_WRAPPER            *NvmeCmdWrapper = NULL;
    COMPLETION_QUEUE_ENTRY          CompletionData;
    UINT8                           PreBootLoadCount = 0;
    EFI_STATUS                      Status;
    
    // Locate all the Nvme Controller Protocol
    Status = gBS->LocateHandleBuffer(
                         ByProtocol,
                         &gAmiNvmeControllerProtocolGuid,
                         NULL,
                         &NvmeHandleCount,
                         &NvmeControllerHandleBuffer
                         );
    if(EFI_ERROR(Status) || NvmeHandleCount == 0 ) {
        return ;
    }
    
    Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof(NVME_COMMAND_WRAPPER),
                  (VOID**)&NvmeCmdWrapper
                  );

    if(EFI_ERROR(Status)) {
        gBS->FreePool(NvmeControllerHandleBuffer);
		ASSERT_EFI_ERROR(Status);
        return ;
    }  

    for (Index = 0; Index < NvmeHandleCount; Index++) {
        // Get the Nvme Controller Protocol Interface
        Status = gBS->HandleProtocol(
                                NvmeControllerHandleBuffer[Index],
                                &gAmiNvmeControllerProtocolGuid,
                                (VOID *)&NvmeController
                                );
                
        ASSERT_EFI_ERROR(Status);
        if(EFI_ERROR(Status)) {
            continue;
        }
        
        //Check whether SET FEATURE command is supported with SAVE field set to 1
        if (!(NvmeController->IdentifyControllerData->ONCS & BIT4)) {
            DEBUG((EFI_D_VERBOSE,"\n SET_FEATURE command with Save field set to non zero value is not supported\n")); 
            continue;
        }
        // Clear memory
        SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
		       
        // Build NVME command
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = GET_FEATURES;
        NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeController->NvmeDxeSmmDataArea->CommandIdentifierAdmin;        
        NvmeCmdWrapper->NvmCmd.NSID = 0; 
        NvmeCmdWrapper->NvmCmd.CDW10 = PROGRESS_MARKER_FID;
        NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
        NvmeCmdWrapper->SQIdentifier = 0;           // Queue 0 for Admin cmds
        NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
        
        Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData); 
        if (EFI_ERROR(Status)) {
            continue;
        }  
        
        // Load the Pre-boot Software Load Count value from Dword 0 of completion queue entry
        PreBootLoadCount = CompletionData.DW0;
		
        // If the Pre-boot Software Load Count is less then 255, then update the field to one more than the previous value.       
        if (PreBootLoadCount < 0xFF) {		            
            // Updated additonal command fields for SET_FEATURES
            NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SET_FEATURES;            
            NvmeCmdWrapper->NvmCmd.CDW10 =  BIT31 | PROGRESS_MARKER_FID;  // Progress Marker Feature Id with Save field set to 1
            NvmeCmdWrapper->NvmCmd.CDW11 = ++PreBootLoadCount;            // Previous PreBootLoadCount value + 1  
            
            ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
        }    
    }
    gBS->FreePool(NvmeControllerHandleBuffer);
    gBS->FreePool(NvmeCmdWrapper);
    return;
}

/**
    Callback function to free allocated memory

    @param Event   - signaled event
    @param Context - pointer to event context

    @retval   VOID
**/
VOID
EFIAPI
NvmeReadyToBootEventCallback (
    EFI_EVENT  Event,
    VOID      *Context
)
{
    ProgramSoftwareProgressMarker();
#if NVMe_SMM_SUPPORT
    gBS->FreePool(gNvmeCommunicateBuffer);
    gBS->FreePool(gNvmeIdentifyControllerData);
    gBS->FreePool(gNvmeActiveNsBlkMedia);
#endif
    gBS->CloseEvent(Event);
}
/**
    Prints Controller capabilities

        
    @param NvmeController 

    @retval VOID

    @note  The amount of data that will get printed can be controlled 
    using DEBUG_ERROR_LEVEL_MASK SDL token.
    Make sure PcdDebugPrintErrorLevel is properly cloned and set to 
    PcdsPatchableInModule in the project.

**/

VOID
PrintNvmeCapability (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
)
{
    
#if  NVME_VERBOSE_PRINT
    DEBUG ((DEBUG_BLKIO, "Controller Capabilities Reg Value       : %16lx\n", NvmeController->RawControllerCapabilities));
    DEBUG ((DEBUG_BLKIO, "Max. Queue Entrys Supported              : %08X\n", NvmeController->MaxQueueEntrySupported));
    DEBUG ((DEBUG_BLKIO, "Contiguous Queue Required                : %08X\n", NvmeController->ContiguousQueueRequired));
    DEBUG ((DEBUG_BLKIO, "Arbitration Mode Supported               : %08X\n", NvmeController->ArbitrationMechanismSupport));
    DEBUG ((DEBUG_BLKIO, "TimeOut in 500msec unit                  : %08X\n", NvmeController->TimeOut));
    DEBUG ((DEBUG_BLKIO, "Doorbell Stride                          : %08X\n", NvmeController->DoorBellStride));
    DEBUG ((DEBUG_BLKIO, "NVM Subsystem Reset Support              : %08X\n", NvmeController->NVMResetSupport));
    DEBUG ((DEBUG_BLKIO, "Command Sets Supported                   : %08X\n", NvmeController->CmdSetsSupported));
    DEBUG ((DEBUG_BLKIO, "Memory Page Size Min.in Bytes            : %08X\n", NvmeController->MemoryPageSizeMin));
    DEBUG ((DEBUG_BLKIO, "Memory Page Size Max.in Bytes            : %08X\n", NvmeController->MemoryPageSizeMax));
    DEBUG ((DEBUG_BLKIO, "Controller Version                       : %08X\n", NvmeController->ControllerVersion));
#endif

    return;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
