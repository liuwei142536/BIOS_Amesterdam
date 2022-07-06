//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file TcgStorageSec.c
        Provides TCG Storage OPAL security protocol support
**/

//---------------------------------------------------------------------------

#include "TcgStorageSec.h"
#if ENABLE_DEVICE_RESET_THRU_PSID
#include "AmiTcgStorageDeviceReset.h"
#endif
#include <Protocol/AmiHddSecurityInit.h>

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
ACTIVE_NAMESPACE_DATA*
GetActiveNameSpace (
    AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    UINT32                         NamespaceId
)
{
    LIST_ENTRY            *LinkData;
    ACTIVE_NAMESPACE_DATA *ActiveNameSpace = NULL;
    
    if(!NvmeController) {
        return NULL;
    }

    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {

        ActiveNameSpace = BASE_CR(LinkData, ACTIVE_NAMESPACE_DATA, Link);
        
        // If NamespaceId is 0xFFFFFFFF, then return the first valid ActiveNameSpace
        if( NamespaceId == 0xFFFFFFFF ) {
            return ActiveNameSpace;
        }

        if(ActiveNameSpace->ActiveNameSpaceID == NamespaceId) {
            return ActiveNameSpace;
        }
    }
    
    return NULL;
}
#endif
//---------------------------------------------------------------------------
// Global variables

#if SmmTcgStorageSec_SUPPORT
EFI_SMM_COMMUNICATION_PROTOCOL  *gSmmCommunication = NULL;

//Global Buffers used for SMM Communication
UINT8                       *gTcgStorageSecCommunicateBuffer = NULL;
UINT8                       *gTcgStorageSecCommunicateData = NULL;
UINTN                       gTcgStorageSecCommunicateBufferSize;

EFI_EVENT   gTcgReadyToBootSmmServiceEvent;
BOOLEAN     gTcgIsReadyToBootEventServiced;

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
BOOT_SCRIPT_NVME_REG_SAVE gPciRootBridgeRegistersSave[] = {
   PCI_ROOT_BRIDGE_BUS,   EfiBootScriptWidthUint32, 0xffffffff, // 0x18
   PCI_ROOT_BRIDGE_IO,    EfiBootScriptWidthUint16, 0xffff,     // 0x1C
   PCI_ROOT_BRIDGE_MEM,   EfiBootScriptWidthUint32, 0xffffffff, // 0x20
   PCI_ROOT_BRIDGE_PFMEM, EfiBootScriptWidthUint32, 0xffffffff, // 0x24
   PCI_ROOT_BRIDGE_CMD,   EfiBootScriptWidthUint8,  0xff,       // 0x04
};

BOOT_SCRIPT_NVME_REG_SAVE gNvmeControllerRegistersSave[] = {
   NVME_CONTROLLER_BAR0, EfiBootScriptWidthUint32, 0xffffffff, // 0x10
   NVME_CONTROLLER_BAR1, EfiBootScriptWidthUint32, 0xffffffff, // 0x14
   NVME_CONTROLLER_BAR2, EfiBootScriptWidthUint32, 0xffffffff, // 0x18
   NVME_CONTROLLER_BAR3, EfiBootScriptWidthUint32, 0xffffffff, // 0x1C
   NVME_CONTROLLER_BAR4, EfiBootScriptWidthUint32, 0xffffffff, // 0x20
   NVME_CONTROLLER_BAR5, EfiBootScriptWidthUint32, 0xffffffff, // 0x24
   NVME_CONTROLLER_CMD,  EfiBootScriptWidthUint8,  0xff,       // 0x04
};

// Total PCI register count for each device 
#define TOTAL_PCI_REGISTERS_COUNT (sizeof(gPciRootBridgeRegistersSave)/sizeof(BOOT_SCRIPT_NVME_REG_SAVE) + \
                               sizeof(gNvmeControllerRegistersSave)/sizeof(BOOT_SCRIPT_NVME_REG_SAVE))
                               
// Global buffers used for collecting NVMe controller and its PCI bridge configuration space 
// register data to save it in Boot script table. 
BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY    gNvmeSaveEntry[TOTAL_PCI_REGISTERS_COUNT * MAX_NVME_DEVICES];
EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL         *gPciRootBridgeIo = NULL;
UINT32                                  gNvmeSaveEntryCount = 0;
#endif
#endif

#if ENABLE_DEVICE_RESET_THRU_PSID
EFI_GUID gAmiTcgStorageDeviceResetGuid = AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL_GUID;
#endif
// TCG Session related variables

VOID            *gBuffer = NULL;
UINTN           gBufferLength;
SESSION_PARAMS  *gSessionParams = NULL;

#if SmmTcgStorageSec_SUPPORT
// Common Ready To Boot SMM service callback
VOID
EFIAPI
TcgReadyToBootSmmServiceCallback (
    IN  EFI_EVENT   Event,
    IN  VOID        *Context
);
#endif

#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
/** @internal
    Check whether Nvme device interface is handled by RST or not
    
    @ param Signature
    
    @retval  TRUE    Nvme device handled by RST driver
    @retval  FALSE   Nvme device not handled by RST driver
    @endinternal
**/
BOOLEAN
IsRstDevice (
   UINT32    Signature
) {
    if((UINT32)Signature == RST_PWD_SIGNATURE) {
        return TRUE;
    } else {
        return FALSE;
    }
}
#endif

/**
    This function is the entry point for TCG STORAGE SECURITY Driver.

    @param ImageHandle - Handle assigned to this driver
    @param SystemTable - EFI System table.

    @retval EFI_STATUS

    @note  
      Installs AmiTcgStorageSecurityInitProtocol

**/
EFI_STATUS
EFIAPI
TcgStorageSecEntryPoint (
    IN  EFI_HANDLE        ImageHandle,
    IN  EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_HANDLE          Handle = NULL;
    EFI_STATUS          Status;
    EFI_EVENT           EndOfDxeEvent = NULL;
    AMI_HDD_SECURITY_INIT_PROTOCOL  *TcgStorageSecInitProtocol = NULL;

    Status = gBS->AllocatePool( EfiBootServicesData,
                                sizeof(AMI_HDD_SECURITY_INIT_PROTOCOL),
                                (VOID**)&TcgStorageSecInitProtocol );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    TcgStorageSecInitProtocol->InstallSecurityInterface = TcgInstallSecurityInterface;
    TcgStorageSecInitProtocol->StopSecurityModeSupport  = TcgStopSecurityModeSupport;

    Status = gBS->InstallProtocolInterface(
                        &Handle,
                        &gAmiTcgStorageSecurityInitProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        TcgStorageSecInitProtocol
                        );
    
    ASSERT_EFI_ERROR( Status );
    if(EFI_ERROR(Status)) {
        return Status;
    }
#if SmmTcgStorageSec_SUPPORT
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    gTcgStorageSecCommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + TCG_STORAGE_SECURITY_PASSWORD_SIZE \
                            + sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY)*(TOTAL_PCI_REGISTERS_COUNT * MAX_NVME_DEVICES)+(2*OPAL_PASSWORD_LENGTH);
#else
    gTcgStorageSecCommunicateBufferSize  = (SMM_COMMUNICATE_HEADER_SIZE + TCG_STORAGE_SECURITY_PASSWORD_SIZE)+(2*OPAL_PASSWORD_LENGTH);
#endif
    //
    // Allocate Memory for Communication buffer.   
    //
    Status = gBS->AllocatePool( EfiRuntimeServicesData,
                                gTcgStorageSecCommunicateBufferSize,
                                (VOID**)&gTcgStorageSecCommunicateBuffer );
    if ( EFI_ERROR( Status )) {
        return Status;
    }
            
    Status = gBS->AllocatePool( EfiReservedMemoryType,
                                512,
                                (VOID**)&gTcgStorageSecCommunicateData );      
    if ( EFI_ERROR( Status )) {
        return Status;
    }
    
    Status = gBS->CreateEventEx (
                       EVT_NOTIFY_SIGNAL,
                       TPL_CALLBACK,
                       (EFI_EVENT_NOTIFY)TransferDmaBufferToSmram,
                       NULL,
                       &gEfiEndOfDxeEventGroupGuid,
                       &EndOfDxeEvent
                       );
    ASSERT_EFI_ERROR(Status);
#endif
    return Status;
}

/** @internal
    Checks if device supports TCG Security protocol or not.

    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval EFI_SUCCESS                      : Security Mode supported.
    @retval EFI_DEVICE_ERROR/EFI_UNSUPPORTED : Not supported.
    
    @endinternal
**/

EFI_STATUS
IsTcgStorageSecProtocolSupported (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
    
)
{
    EFI_STATUS                              Status;
    SP0_TRUSTED_RECEIVE_PARAMETER_DATA      QueryBuffer;
    UINTN                                   QueryBufferSize = sizeof(SP0_TRUSTED_RECEIVE_PARAMETER_DATA);
    UINTN                                   SpByte;
    EFI_STORAGE_SECURITY_COMMAND_PROTOCOL   *StorageSecurityProtocol = NULL;
    UINT16                                  ListLength = 0;
    UINT32                                  MediaId = 0;
    UINT64                                  CmdTimeout=0;     
    BOOLEAN                                 RstControlled = FALSE;
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )    
    AMI_NVME_CONTROLLER_PROTOCOL            *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
    ACTIVE_NAMESPACE_DATA                   *ActiveNameSpace = NULL;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        MediaId = ((SATA_DEVICE_INTERFACE*)BusInterface)->SataBlkIo->BlkIo.Media->MediaId;
        StorageSecurityProtocol = ((SATA_DEVICE_INTERFACE*)BusInterface)->StorageSecurityInterface;
        CmdTimeout=SATA_COMMAND_TIMEOUT;
    }
#endif
 
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
        MediaId = 0;
        CmdTimeout = NVME_COMMAND_TIMEOUT;
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled){
            StorageSecurityProtocol = (EFI_STORAGE_SECURITY_COMMAND_PROTOCOL *)NvmeRstInterface->EfiStorageSecInterface;
        }
#endif        
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32 );        
            StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
        }
#endif
    }
    if(NULL == StorageSecurityProtocol) {
        return EFI_NOT_FOUND;
    }

    gBS->SetMem(&QueryBuffer, QueryBufferSize, 0);
    // SPC Spec Section 7.7.1 has details on decoding the data
    Status = StorageSecurityProtocol->ReceiveData( StorageSecurityProtocol,
                                                   MediaId, // Media ID
                                                   MultU64x32(CmdTimeout, 10000000),
                                                   SECURITY_PROTOCOL_INFORMATION,   // Security Protocol ID
                                                   0x00,                            // SP specific id
                                                   0x200,                           // Transfer Length
                                                   &QueryBuffer,
                                                   &QueryBufferSize                 // Transferred Length
                                                   );
    DEBUG ((DEBUG_VERBOSE, "TcgStorageSecurity: GetSupportedProtocols Status : %r\n", Status));
    if (!EFI_ERROR(Status)) {

        // The List Length field indicates the total length, in bytes, of the supported security protocol list.
        ListLength = QueryBuffer.ListLengthHigh << 8 | QueryBuffer.ListLengthLow;

        if (ListLength == 0) {
            return EFI_UNSUPPORTED;
        }

        // Check Security supported Protocol List for Protocol ID 1
        for (SpByte = 0; SpByte < ListLength; SpByte++) {
            DEBUG ((DEBUG_VERBOSE, "TcgStorageSecurity: SpByte =: %x, SupportedProtocolList = %x\n", \
                    SpByte, QueryBuffer.SupportedSPList[SpByte]));      
            if (QueryBuffer.SupportedSPList[SpByte] == SECURITY_PROTOCOL_1) {
                return EFI_SUCCESS;
            }
        }
    }
    return EFI_UNSUPPORTED;
}

/** @internal
    Installs TcgStorageSecurityProtocol on input device handle

    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval EFI_SUCCESS                      : TcgStorageSecurityProtocol INSTALLED
    @retval EFI_DEVICE_ERROR/EFI_UNSUPPORTED : Not supported.
    
    @endinternal
**/

EFI_STATUS
EFIAPI
TcgInstallSecurityInterface (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
    EFI_STATUS                              Status;
    AMI_TCG_STORAGE_SECURITY_INTERFACE      *TcgSecurityInterface;
    BOOLEAN                                  RstControlled = FALSE;
#if ENABLE_DEVICE_RESET_THRU_PSID
    AMI_TCG_STORAGE_RESET_INTERFACE         *TcgStorageResetInterface;
#endif
    EFI_HANDLE                      DeviceHandle = NULL;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
    UINT16   OpalDeviceStatus;
#endif

    if ( BusInterfaceType == AmiStorageInterfaceIde ) {
        return EFI_UNSUPPORTED;
    }

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        DeviceHandle = SataDevInterface->IdeDeviceHandle;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled){
            DeviceHandle = NvmeRstInterface->ControllerHandle;
        }
#endif        
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32 );
            DeviceHandle = ActiveNameSpace->NvmeDeviceHandle;
        }
#endif
    }
    if(!RstControlled) {
        // Check for TRUSTED Send and Receive command Support
        Status = gBS->OpenProtocol ( DeviceHandle, 
                                 &gEfiStorageSecurityCommandProtocolGuid, 
                                 NULL, 
                                 NULL, 
                                 NULL, 
                                 EFI_OPEN_PROTOCOL_TEST_PROTOCOL );
    
        TCG_STORAGE_ERR_CHECK(Status);
    } 

    // Get supported protocol list and check if TCG security protocol supported
    Status = IsTcgStorageSecProtocolSupported(BusInterface, BusInterfaceType);

    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "TCG security protocol NOT supported \n"));
        return Status;
    }

    // Return error if gAmiTcgStorageSecurityProtocolGuid is already installed on the Device Handle
    Status = gBS->OpenProtocol ( DeviceHandle, 
                                 &gAmiTcgStorageSecurityProtocolGuid, 
                                 NULL, 
                                 NULL, 
                                 NULL, 
                                 EFI_OPEN_PROTOCOL_TEST_PROTOCOL );
    if(!EFI_ERROR(Status)) {
        return EFI_ALREADY_STARTED;
    }

    Status = gBS->AllocatePool( EfiBootServicesData,
                                sizeof(AMI_TCG_STORAGE_SECURITY_INTERFACE),
                                (VOID**)&TcgSecurityInterface );
    TCG_STORAGE_ERR_CHECK(Status);
#if ENABLE_DEVICE_RESET_THRU_PSID   
    Status = gBS->AllocatePool( EfiBootServicesData,
                                    sizeof(AMI_TCG_STORAGE_RESET_INTERFACE),
                                    (VOID**)&TcgStorageResetInterface );

   TCG_STORAGE_ERR_CHECK(Status);
#endif
   
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        SataDevInterface->IdeSecurityInterface = (AMI_HDD_SECURITY_PROTOCOL*) TcgSecurityInterface;
        TcgSecurityInterface->BusInterface     = SataDevInterface;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )
        if (RstControlled ) {
            NvmeRstInterface->SecurityInterface = TcgSecurityInterface;
            TcgSecurityInterface->BusInterface     = NvmeRstInterface; 
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if ( !RstControlled ) {
            NvmeDevInterface->SecurityInterface = TcgSecurityInterface;
            TcgSecurityInterface->BusInterface     = NvmeDevInterface;
        }
#endif
    }
    
    Status = InitializeOpalConfig(TcgSecurityInterface->BusInterface, BusInterfaceType);
    TCG_STORAGE_ERR_CHECK(Status)

    TcgSecurityInterface->ReturnSecurityStatus    = TcgReturnSecurityStatus;
    TcgSecurityInterface->SecuritySetPassword     = TcgSecuritySetPassword;
    TcgSecurityInterface->SecurityUnlockPassword  = TcgSecurityUnlockPassword;
    TcgSecurityInterface->SecurityDisablePassword = TcgSecurityDisablePassword;
    TcgSecurityInterface->SecurityFreezeLock      = TcgSecurityBlockSid;
    TcgSecurityInterface->SecurityEraseUnit       = TcgSecurityEraseUnit;
    TcgSecurityInterface->ReturnIdePasswordFlags  = TcgReturnIdePasswordFlags;

    // Set BusInterfaceType to FALSE if IdeBus else TRUE for Ahci Bus.
    TcgSecurityInterface->BusInterfaceType = BusInterfaceType;  // Ide Bus
#if ENABLE_DEVICE_RESET_THRU_PSID    
    TcgStorageResetInterface->PsidRevert   = TcgStoragePsidRevert;
    TcgStorageResetInterface->TcgStorageSecurityInterface = TcgSecurityInterface;
#endif
    //  Install the protocol on the device
    Status = gBS->InstallMultipleProtocolInterfaces( &DeviceHandle,
                                                     &gAmiTcgStorageSecurityProtocolGuid,
                                                     TcgSecurityInterface,
#if ENABLE_DEVICE_RESET_THRU_PSID
                                                     &gAmiTcgStorageDeviceResetGuid, 
                                                     TcgStorageResetInterface,
#endif
                                                     NULL );
    
    ASSERT_EFI_ERROR( Status );
    TCG_STORAGE_ERR_CHECK(Status)
#if SmmTcgStorageSec_SUPPORT
    // Not needed to execute following code if BUS drivers gets connected after ReadyToBootGuid Event
    if(gTcgIsReadyToBootEventServiced) {
        return Status;    
    }
#endif
    
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
    
    Status = TcgReturnSecurityStatus ( (AMI_HDD_SECURITY_PROTOCOL*)TcgSecurityInterface,
                                       &OpalDeviceStatus );
    ASSERT_EFI_ERROR( Status );
    
    // Issue Tper reset to force lock the device when
    // Locking enabled but the device is not locked (eg: warm reset)
    if( !EFI_ERROR(Status) && (OpalDeviceStatus & SecurityEnabledMask)  && !(OpalDeviceStatus & SecurityLockedMask) ) {
        IssueTperReset (TcgSecurityInterface->BusInterface, BusInterfaceType );
    }
#endif
    
#if SmmTcgStorageSec_SUPPORT
    
    // Send data to SMM.
    if( !gSmmCommunication ) {
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
        if (EFI_ERROR(Status)) {
            gSmmCommunication=NULL;
        }
    }
    
#if FREEZE_LOCK_OPAL
    // Save device information to send Block SID during S3 resume.

    Status = SmmTcgPasswordInterface( (AMI_HDD_SECURITY_PROTOCOL*) TcgSecurityInterface,
                                        0,  // Not applicable
                                        NULL,   // Not applicable
                                        SaveDeviceInfo ); 
#endif // #if FREEZE_LOCK_OPAL 

    // Install the Readytoboot event 
    if ( gTcgReadyToBootSmmServiceEvent == NULL ) {
        //
        // Create event for saving Boot Script information required to unlock the drive during S3.
        //
        Status = gBS->CreateEventEx(
                        EVT_NOTIFY_SIGNAL, 
                        TPL_CALLBACK, 
                        TcgReadyToBootSmmServiceCallback,
                        NULL,
                        &gEfiEventReadyToBootGuid,
                        &gTcgReadyToBootSmmServiceEvent);
         ASSERT_EFI_ERROR(Status);
    }
#endif // #if SmmTcgStorageSec_SUPPORT
    return Status;
}

#if SmmTcgStorageSec_SUPPORT
/** @internal
    Transfer BS buffer to SmmTcgStorageSec driver

    @param None

    @retval EFI_STATUS

    @endinternal
**/
EFI_STATUS
TransferDmaBufferToSmram (
    IN  EFI_EVENT   Event, 
    IN  VOID        *Context
)
{
    EFI_SMM_COMMUNICATE_HEADER  *SmmCommunicateHeader;
    EFI_STATUS                  Status;
    UINTN                       CommunicateBufferSize = 0;

    // Calculate Size of Communication buffer
    CommunicateBufferSize  = (((UINTN) &(((EFI_SMM_COMMUNICATE_HEADER *)0)->Data))) + sizeof (UINT8 *);

    gBS->SetMem( gTcgStorageSecCommunicateBuffer, CommunicateBufferSize, 0);

    // Copy Smm Communicate Header Here
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gTcgStorageSecCommunicateBuffer;
    gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgGetNonSmramDataTransferBufferGuid, sizeof( EFI_GUID ));
    SmmCommunicateHeader->MessageLength  = sizeof(UINT8 *);

    gBS->CopyMem( &SmmCommunicateHeader->Data, &gTcgStorageSecCommunicateData, sizeof(UINT8 *));
    
    if(gSmmCommunication == NULL) {
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            return Status;  
        }
    }

    // Send data to SMM.
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;  
    }
    gBS->CloseEvent(Event);
    return Status; 
}

#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
/** @internal
    It uses EFI_SMM_COMMUNICATION_PROTOCOL API to transfer data from Non-SMM
    mode NVME BootScript Data to SMM mode. 
    
    @param pData - Pointer to data to be transfered
    @param DataSize - Size of the data
    @param pGuid  - Pointer to GUID identifier for the data
        
    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
TcgTransferNvmeBootScriptDataToSmm ( 
    IN  VOID      *Data, 
    IN  UINTN     DataSize, 
    IN  EFI_GUID  *Guid 
)
{
    EFI_SMM_COMMUNICATE_HEADER     *SmmCommunicateHeader;
    EFI_STATUS                     Status;
    UINTN                          CommunicateBufferSize;

    if ( Data == NULL || DataSize == 0 || Guid == NULL ) {
        return EFI_INVALID_PARAMETER;
    }

    // Calculate Size of Communication buffer
    CommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + DataSize; // Header size (without data) + data size
    
    gBS->SetMem( gTcgStorageSecCommunicateBuffer, CommunicateBufferSize, 0 );

    // Copy SMM Communicate Header Here
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gTcgStorageSecCommunicateBuffer;

    // Copy data GUID
    gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, Guid, sizeof( EFI_GUID ));

    // Updated data length
    SmmCommunicateHeader->MessageLength  = DataSize;

    // Copy Data Here
    gBS->CopyMem( &SmmCommunicateHeader->Data, Data, DataSize);

    // Locate EFI_SMM_COMMUNICATION_PROTOCOL
    if(gSmmCommunication == NULL) {
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            return Status;  
        }
    }

    // Send data to SMM using protocol API
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;  
    }
    
    return EFI_SUCCESS;
}

/** @internal
    Copies PciRegisters from NvmeController or PCI Root Bridge devices 
    to S3 Buffer Structure for saving Data in Bootscript for S3 Resume.

    @param PciAddress - Physical Pci Address of register to be stored
    @param Width - Width of the data to be stored
    @param Value  - Value to be saved for Bootscript
        
    @retval EFI_STATUS
    
    @endinternal

**/
EFI_STATUS
TcgCopyPciDataToS3BufferStructure (
  IN    UINTN                           PciAddr,
  IN    EFI_BOOT_SCRIPT_WIDTH           Width,
  IN    UINTN                           Value
){
    DEBUG((DEBUG_VERBOSE,"\n TcgCopyPciDataToS3BufferStructure - %x %x %x %x", gNvmeSaveEntryCount, PciAddr, Width, Value ));
    
    if(gNvmeSaveEntryCount >= (TOTAL_PCI_REGISTERS_COUNT * MAX_NVME_DEVICES)) {
        ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
        return EFI_OUT_OF_RESOURCES;
    }

    // Save the bootScript save entries to the array list
    gNvmeSaveEntry[gNvmeSaveEntryCount].PciAddress = PciAddr;
    gNvmeSaveEntry[gNvmeSaveEntryCount].Width = Width;
    gNvmeSaveEntry[gNvmeSaveEntryCount].Value = Value;
    gNvmeSaveEntryCount++;
    
    return EFI_SUCCESS;
}

/** @internal
    Function stores PCI bridge BAR,CMD,BUS PCI Configuration values

    @param BusNum
    @param DevNum
    @param FuncNum

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
StorePciRootBridgePciCfgRegs (
    UINTN             BusNum,
    UINTN             DevNum,
    UINTN             FuncNum
)
{
    EFI_STATUS    Status;
    UINTN         Data32;
    UINTN         Index;
    UINTN         RbPciAddress;
    
    EFI_GUID gPciRootBridgeIoProtocolGuid = EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_GUID;

    if( !gPciRootBridgeIo ) {
        Status = gBS->LocateProtocol(
                      &gPciRootBridgeIoProtocolGuid,
                      NULL,
                      (void**) &gPciRootBridgeIo
                      );

        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR (Status);
            return Status;
        }
    }

    for (Index = 0; Index < sizeof(gPciRootBridgeRegistersSave)/sizeof(BOOT_SCRIPT_NVME_REG_SAVE); Index++ ) {

        RbPciAddress = EFI_PCI_CFG_ADDRESS (BusNum, DevNum, FuncNum, gPciRootBridgeRegistersSave[Index].RegNum);


        gPciRootBridgeIo->Pci.Read(
                gPciRootBridgeIo,
                gPciRootBridgeRegistersSave[Index].Width,
                RbPciAddress,
                1,
                &Data32 );
        
        Data32 &= gPciRootBridgeRegistersSave[Index].Mask;
        
        // Copy Root Bridge Pci Data to be saved in BootScript to entry list one by one to transfer to Smm
        // in global buffer gNvmeSaveEntry.
        Status = TcgCopyPciDataToS3BufferStructure(RbPciAddress, gPciRootBridgeRegistersSave[Index].Width, Data32);
        
        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR (Status);
            return Status;
        }
    }
    return EFI_SUCCESS;
}

/** @internal
    This routine collects NVMe controller and its corresponding PCI bridge 
    register data to save it in Boot script table.During S3 resume PCI register 
    information stored in Boot Script will be programmed to PCI configuration space 
    registers.So that NVMe device can be unlocked and used during S3 resume.
    
    @param  VOID
    
    @retval VOID
    
    @note If TCG_STORAGE_SEC_SAVE_PCI_ROOTBRIDGE_REGS token is disabled only NVMe controller
          PCI configuration space registers will be saved.If this token is enabled PCI bridge
          register data under which NVMe controller is connected also will be saved to BootScript.
    
    @endinternal
**/

VOID
EFIAPI
StoreNvmePciConfigDataInBootScript (
)
{
    EFI_STATUS                              Status;
    UINTN                                   PciSeg, PciBus, PciDev, PciFunc;
    EFI_PCI_IO_PROTOCOL                     *PciIO;
    UINTN                                   Data32 = 0;
    UINT32                                  RegIndex;
    UINT32                                  Index;
    UINTN                                   HandleCount;
    EFI_HANDLE                              *HandleBuffer;
    UINTN                                   PciAddress;
    BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY    *TransferBuffer = NULL;
#if TCG_STORAGE_SEC_SAVE_PCI_ROOTBRIDGE_REGS
    EFI_HANDLE                              MatchedDevicePathHandle;
    EFI_DEVICE_PATH_PROTOCOL                *DevicePath;
    EFI_DEVICE_PATH_PROTOCOL                *DevicePathNode;
    EFI_DEVICE_PATH_PROTOCOL                *HeadDevicePath = NULL;
    EFI_DEVICE_PATH_PROTOCOL                *TempHeadDevicePath;
#endif
    AMI_NVME_CONTROLLER_PROTOCOL            *NvmeDevInterface;
    ACTIVE_NAMESPACE_DATA                   *ActiveNameSpace = NULL;

    Status = gBS->LocateHandleBuffer(
                  ByProtocol,
                  &gAmiNvmeControllerProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                 );

    if ( EFI_ERROR( Status )) {
        return;
    }
    
    for( Index = 0; Index < HandleCount; Index++ ) {
        
        Status = gBS->HandleProtocol(
                      HandleBuffer[Index],
                      &gAmiNvmeControllerProtocolGuid,
                      (VOID *)&NvmeDevInterface
                      );

        if(EFI_ERROR(Status)) {
            continue;
        }
        
        ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32 );         

        // Verify whether gAmiTcgStorageSecurityProtocolGuid is installed or not ?
        Status = gBS->OpenProtocol ( ActiveNameSpace->NvmeDeviceHandle, 
                                     &gAmiTcgStorageSecurityProtocolGuid, 
                                     NULL, 
                                     NULL, 
                                     NULL, 
                                     EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                                     );
        if(EFI_ERROR(Status)) {
            continue;
        }
#if TCG_STORAGE_SEC_SAVE_PCI_ROOTBRIDGE_REGS
        // Get the Device path Protocol interface
        Status = gBS->HandleProtocol( HandleBuffer[Index],
                                      &gEfiDevicePathProtocolGuid,
                                      (void**)&DevicePath
                                      );
    
        if(!EFI_ERROR(Status)) {
        
            DevicePathNode = DevicePath;
            // From DP obtain each PCI DP node, if its a PCI bridge node then gather PCI bridge's 
            // PCI configuration space register data to save in boot script table.
            for ( ;!IsDevicePathEnd(DevicePathNode); DevicePathNode = NextDevicePathNode (DevicePathNode)) {
    
                TempHeadDevicePath = HeadDevicePath;
                HeadDevicePath = AppendDevicePathNode(HeadDevicePath,DevicePathNode);
    
                if(TempHeadDevicePath != NULL) {
                    gBS->FreePool(TempHeadDevicePath);
                }
    
                // Check to identify the PCI device Path
                if ((DevicePathNode->Type == HARDWARE_DEVICE_PATH) &&
                            (DevicePathNode->SubType == HW_PCI_DP)) {
    
                    TempHeadDevicePath = HeadDevicePath;
    
                    Status = gBS->LocateDevicePath( &gEfiDevicePathProtocolGuid,
                                                    &TempHeadDevicePath,
                                                    &MatchedDevicePathHandle
                                                    );
                    if ( EFI_ERROR( Status )) {
                        ASSERT_EFI_ERROR (Status);
                        continue;
                    }
    
                    Status = gBS->HandleProtocol( MatchedDevicePathHandle,
                                                  &gEfiPciIoProtocolGuid,
                                                  (void **) &PciIO
                                                  );
                    if ( EFI_ERROR( Status )) {
                        ASSERT_EFI_ERROR (Status);
                        continue;
                    }
                    Status = PciIO->GetLocation(PciIO, &PciSeg, &PciBus, &PciDev, &PciFunc);
                    if ( EFI_ERROR( Status )) {
                        ASSERT_EFI_ERROR (Status);
                        continue;
                    }
                    
                    Status = PciIO->Pci.Read(PciIO, 
                                             EfiPciIoWidthUint32, 
                                             PCI_RID, 
                                             1, 
                                             &Data32);
                    if(EFI_ERROR(Status)) {
                        ASSERT_EFI_ERROR (Status);
                        continue;
                    }
    
                    // Check Class code to identify whether it is a PCI root bridge device or not
                    if((Data32>>24) == PCI_CL_BRIDGE) {
                        // Store PCI bridge device PCI configuration values
                        Status = StorePciRootBridgePciCfgRegs(PciBus,PciDev,PciFunc);

                        if ( EFI_ERROR( Status )) {
                            ASSERT_EFI_ERROR (Status);
                            return;
                        }
                    }
                }
            }
        } else {
            ASSERT_EFI_ERROR (Status);
            continue;
        }
#endif
        // PCI bridge register data has been stored above.Now gather NVMe controller's
        // PCI configuration space data to save in Boot Script.
        
        // Get the PCIIO protocol installed on the handle
        Status = gBS->HandleProtocol( HandleBuffer[Index],
                                      &gEfiPciIoProtocolGuid,
                                      (void**) &PciIO
                                      );
    
        if ( EFI_ERROR( Status )) {
            ASSERT_EFI_ERROR (Status);
            continue;
        }
    
        Status = PciIO->GetLocation(PciIO, &PciSeg, &PciBus, &PciDev, &PciFunc);
        if ( EFI_ERROR( Status )) {
            ASSERT_EFI_ERROR (Status);
            continue;
        }
        
        Status = PciIO->Pci.Read( \
                        PciIO, \
                        EfiBootScriptWidthUint32, \
                        PCI_VID, \
                        1, \
                        &Data32);
    
        if ( EFI_ERROR( Status )) {
            ASSERT_EFI_ERROR (Status);
            continue;
        }
        
        if (Data32 != 0xffffffff) {
            for (RegIndex = 0; RegIndex < sizeof(gNvmeControllerRegistersSave)/sizeof(BOOT_SCRIPT_NVME_REG_SAVE); RegIndex++) {
                Status = PciIO->Pci.Read( \
                         PciIO, \
                         gNvmeControllerRegistersSave[RegIndex].Width, \
                         gNvmeControllerRegistersSave[RegIndex].RegNum, \
                         1, \
                         &Data32);
                if ( EFI_ERROR( Status )) {
                    ASSERT_EFI_ERROR (Status);
                    continue;
                }
                
                Data32 &= gNvmeControllerRegistersSave[RegIndex].Mask;
                
                PciAddress =   EFI_PCI_CFG_ADDRESS( PciBus, PciDev, PciFunc, gNvmeControllerRegistersSave[RegIndex].RegNum);

                // Copy NVMe PCI data to be saved in BootScript to entry list one by one to transfer to SMM
                Status = TcgCopyPciDataToS3BufferStructure(PciAddress, gNvmeControllerRegistersSave[RegIndex].Width, Data32);

                if (EFI_ERROR(Status)) {
                    ASSERT_EFI_ERROR (Status);
                    return;
                }
            }
        }
    }

    // Allocate a buffer that will hold the entry list of BootScript saved along with the Nvme BootScript Signature
    // and Total bootscript entry count @ the end
    //          Buffer will be as of below
    
    //  ===============================     ======================================== 
    //          Buffer entries                              Size  
    //  ===============================     ========================================  
    //       NvmeSaveEntry[0]                   sizeof(BOOT_SCRIPT_NVME_REG_SAVE)
    //       NvmeSaveEntry[1]                   sizeof(BOOT_SCRIPT_NVME_REG_SAVE)
    //       NvmeSaveEntry[2]                   sizeof(BOOT_SCRIPT_NVME_REG_SAVE)
    //          .
    //          .
    //          .
    //       NvmeSaveEntry[n]                   sizeof(BOOT_SCRIPT_NVME_REG_SAVE)  where n is Total Entry Count
    //       NVME Boot Script Signature         UINT32   ('n','s','b','s')
    //       gNvmeSaveEntryCount                UINT32
    //
    Status = gBS->AllocatePool( EfiBootServicesData,
                                sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY)*(gNvmeSaveEntryCount) + 
                                2*sizeof(UINT32),
                                (VOID**)&TransferBuffer);
    
    if ( EFI_ERROR( Status )) {
        ASSERT_EFI_ERROR (Status);
        return;
    }

    // Copy the saved entry list to the buffer allocated
    gBS->CopyMem(TransferBuffer, gNvmeSaveEntry, sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY)*(gNvmeSaveEntryCount));

    // Append the Signature and Boot script Entry Count to the buffer.
    *(UINT32*)(&TransferBuffer[gNvmeSaveEntryCount]) = NVME_SECURITY_SAVE_SIGNATURE;
    *(UINT32*)(((UINT8*)&TransferBuffer[gNvmeSaveEntryCount]) + sizeof(UINT32)) = gNvmeSaveEntryCount;

    // Transfer NVMe boot script buffer with size to communication buffer to trasfer to SMM 
    TcgTransferNvmeBootScriptDataToSmm ( TransferBuffer,
                                         sizeof(BOOT_SCRIPT_NVME_SECURITY_SAVE_ENTRY)*(gNvmeSaveEntryCount) + 
                                         2*sizeof(UINT32), 
                                         &gTcgStorageSecNvmeBootScriptSaveTriggerGuid );

    // Free the unused memory
    gBS->FreePool(TransferBuffer);
    gBS->FreePool(HandleBuffer);
    
    return;
}
#endif
/** @internal
    Triggers SMM Communication event to save SwSmi port address and 
    TCG storage password Unlock SMI handler context in BootScript table.
    During S3 resume using this stored boot script data an SMI(UnlockOPALPassword) 
    will be generated that unlocks the TCG drive.
    
    @param  VOID
         
    @retval VOID

    @note  Unlock SMI handler will be called only if OPAL device loses power
    in S3 state.A SW SMI will be generated during S3 resume, to unlock the OPAL 
    device and also to put the TCG device in Freeze state.
    
    @endinternal
**/
VOID
EFIAPI
StoreUnlockSmiContextInBootScript(
) 
{
    EFI_SMM_COMMUNICATE_HEADER  *SmmCommunicateHeader=NULL;
    EFI_STATUS                  Status;
    UINTN                       CommunicateBufferSize;
    
    if (gSmmCommunication == NULL) {
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            return ;  
        }
    }
    // Calculate Size of Communication buffer
    CommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + sizeof (UINT8 *);

    gBS->SetMem( gTcgStorageSecCommunicateBuffer, CommunicateBufferSize, 0 );

    //
    // Copy Smm Communicate Header Here
    //
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gTcgStorageSecCommunicateBuffer;
    gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgStorageSecSmiBootScriptSaveTriggerGuid, sizeof( EFI_GUID ) );
    SmmCommunicateHeader->MessageLength  = sizeof(UINT8 *);

    //
    // Send data to SMM.
    //
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
    }
    return;
}
/** @internal
    Triggers SMM Communication event to UnRegister SaveOPALPassword and 
    RemoveOPALPassword SMI handlers.

    @param  VOID

    @retval VOID

    @endinternal
**/
VOID
EFIAPI
UnRegisterSaveRemovePwdSmiHandlers ()
{
    EFI_STATUS                  Status;
    EFI_SMM_COMMUNICATE_HEADER  *SmmCommunicateHeader=NULL;
    UINTN                       CommunicateBufferSize;
    UINT64                      TcgSmiUnRegSignature = TCG_SMI_UNREGISTER_SIGNATURE;

    // Calculate Size of Communication buffer
    CommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + sizeof (TcgSmiUnRegSignature);
    
    gBS->SetMem( gTcgStorageSecCommunicateBuffer, CommunicateBufferSize, 0 );
    
    //
    // Copy SMM Communicate Header Here
    //
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gTcgStorageSecCommunicateBuffer;
    
    //
    // UnRegister Save Password SMI handler
    //
    gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgStorageSecSavePwdGuid, sizeof( EFI_GUID ) );
    gBS->CopyMem( &SmmCommunicateHeader->Data, &TcgSmiUnRegSignature, sizeof( TcgSmiUnRegSignature ) );
    SmmCommunicateHeader->MessageLength  = sizeof(TcgSmiUnRegSignature);

    if(gSmmCommunication == NULL) {
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            return ;  
        }
    }
    //
    // Send data to SMM.
    //
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return ;  
    }
    //
    // Now UnRegister Remove Password SMI handler
    //
    gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgStorageSecRemovePwdGuid, sizeof( EFI_GUID));
    //
    // Send data to SMM.
    //
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);
    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
    }
    return;
}

/**
    This Ready to Boot service call back will perform below functions:
     
    1) StoreNvmePciConfigDataInBootScript() - Store NVMe Controller and
       PCI bridge register data in BootScript table.
    2) StoreUnlockSmiContextInBootScript() - Store Unlock SMI handler 
       context to BootScript.
    3) TransferPwDataFromNvramToSmram - If FORCE_TCG_OPAL_PASSWORD_PROMPT 
       support is disabled transfers the password data stored from NVRAM to SMRAM.
    4) UnRegisterSaveRemovePwdSmiHandlers - UnRegisters SMI handlers that Stores 
       and Removes password in SMRAM.
    5) Finally frees the global Runtime memory allocated for SMM communication
       operation.(This operation should be performed at last in this callback)
    6) Close this Event.

    @param Event    - Event that was triggered
    @param Context  - Point to calling context
         
    @retval VOID

    @note 1. Will be called only if OPAL device loses power in S3 state. A SW SMI will
        be created during S3 resume, to unlock the OPAL device and put it in
        Freeze state.
        2.The above mentioned function execution order should not be disturbed.

**/
VOID
EFIAPI
TcgReadyToBootSmmServiceCallback (
    IN  EFI_EVENT Event,
    IN  VOID      *Context
)
{
#if  !FORCE_TCG_OPAL_PASSWORD_PROMPT
    EFI_STATUS    Status = EFI_SUCCESS;
#endif
    gTcgIsReadyToBootEventServiced = TRUE;
    
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    // Store NVMe Controller and PCI bridge register data in BootScript table.
    StoreNvmePciConfigDataInBootScript();
#endif
    // Store TCG Unlock SMI Handler context in BootScript table
    StoreUnlockSmiContextInBootScript();
    
    // Transfer password data from NVRAM to SMRAM
#if  !FORCE_TCG_OPAL_PASSWORD_PROMPT
    Status = TcgTransferPwDataFromNvramToSmram();
    ASSERT_EFI_ERROR(Status);
#endif

    // Unregister password save and remove SMI Handlers.
    UnRegisterSaveRemovePwdSmiHandlers();
    //
    // Free the RT service memory buffer that is allocated for SMM communication operation.
    //
    if( gTcgStorageSecCommunicateBuffer != NULL ) {        

        gBS->FreePool(gTcgStorageSecCommunicateBuffer);
        gTcgStorageSecCommunicateBuffer = NULL;
    }
    //Close the Event
    gBS->CloseEvent( Event );
    return;
    
}
#endif // SmmTcgStorageSec_SUPPORT
/** 
    Stops the security mode support by un-installing the
    security interface.

    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval 
        EFI_SUCCESS         : Security Mode supported
        EFI_DEVICE_ERROR    : Not supported
**/

EFI_STATUS
EFIAPI
TcgStopSecurityModeSupport (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
    EFI_STATUS                  Status;
    EFI_HANDLE                  DeviceHandle = NULL;
    AMI_TCG_STORAGE_SECURITY_INTERFACE  *TcgSecurityInterface = NULL;
    BOOLEAN                             RstControlled = FALSE;
    OPAL_DEVICE                         *OpalConfig = NULL;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE       *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif
    

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        TcgSecurityInterface = (AMI_TCG_STORAGE_SECURITY_INTERFACE*)SataDevInterface->IdeSecurityInterface;
        DeviceHandle = SataDevInterface->IdeDeviceHandle;
		OpalConfig = (OPAL_DEVICE*)SataDevInterface->OpalConfig;
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled){
            TcgSecurityInterface = (AMI_TCG_STORAGE_SECURITY_INTERFACE*)NvmeRstInterface->SecurityInterface;
            DeviceHandle = NvmeRstInterface->ControllerHandle;
			OpalConfig = (OPAL_DEVICE*)NvmeRstInterface->OpalConfig;
        }
#endif        
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32 );
            TcgSecurityInterface = (AMI_TCG_STORAGE_SECURITY_INTERFACE*)NvmeDevInterface->SecurityInterface;
            DeviceHandle = ActiveNameSpace->NvmeDeviceHandle;
			OpalConfig = (OPAL_DEVICE*)NvmeDevInterface->OpalConfig;
        }
#endif
    }
    // IDE BusInterfaces NOT supported
    if ( BusInterfaceType == AmiStorageInterfaceIde ) {
        return EFI_UNSUPPORTED;
    }

    if ( TcgSecurityInterface == NULL ) {
        return EFI_SUCCESS;
    }

    //  Install Devicepath and BLOCK_IO on a new handle
    Status = gBS->UninstallMultipleProtocolInterfaces(
                                DeviceHandle,
                                &gAmiTcgStorageSecurityProtocolGuid,
                                TcgSecurityInterface,
                                NULL 
                                );

    if ( Status == EFI_SUCCESS ) {
        gBS->FreePool( TcgSecurityInterface );
    }

    gBS->FreePool( OpalConfig );
    OpalConfig = NULL;

    return Status;
}

/**
    Returns whether the Device supports Security Mode or not.
    If supported, the current status of the device is returned.
            
    @param This - AMI_HDD_SECURITY_PROTOCOL instance
    @param SecurityStatus - Device security status

    @retval EFI_SUCCESS : Security Mode supported
    @retval EFI_UNSUPPORTED : Not supported

    @retval UINT16      *SecurityStatus
    @retval             Bit 0 : Security Supported
    @retval             Bit 1 : Security Enabled
    @retval             Bit 2 : Security Locked
    @retval             Bit 3 : Security Frozen
    @retval             Bit 4 : Security Count Expired
    @retval             Bit 5 : Enhanced Security Erase supported
    @retval             Bit 6 : Enhanced Security Erase supported
    @retval             Bit 8 : Security Level 0:High, 1:Maximum

**/

EFI_STATUS
EFIAPI
TcgReturnSecurityStatus (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN OUT UINT16                   *SecurityStatus
)
{
    EFI_STATUS       Status;
    VOID             *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;
    
	// Update opal device Level0 information
    Status = UpdateLevel0Data( BusInterface,  BusInterfaceType);
    if (EFI_ERROR(Status)) {
        return Status;
    }
	
    // Return device security status of the device
    *SecurityStatus = GetOpalSecurityStatus( BusInterface,  BusInterfaceType);
     
    DEBUG((DEBUG_INFO,"\n TcgReturnSecurityStatus = %x", *SecurityStatus));
    return EFI_SUCCESS;
}

/**
    Sets TCG OPAL Admin/User Password.
        
    @param This    - AMI_HDD_SECURITY_PROTOCOL instance
    @param Control - Bit 0 : 0/1 User/Admin Password
                     Bit 1 : Password Buffer has 
                             Old Admin(Master) Password and 
                             New User/Master Password

    @param Buffer (32 Bytes) - Buffer with password data
    @param RevisionCode - Ignored for OPAL

    @retval EFI_SUCCESS : Password set.
    @retval EFI_UNSUPPORTED : Not supported.

    @note  

**/
EFI_STATUS
EFIAPI
TcgSecuritySetPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer,
    IN  UINT16                      RevisionCode
)
{
    BOOLEAN          RstControlled = FALSE;
    EFI_STATUS       Status  = EFI_UNSUPPORTED;
    OPAL_DEVICE      *OpalDevice = NULL;
    UINT16           OpalDeviceStatus = 0;
    VOID             *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif
    
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }
	
    // Bit0 and Bit 8 are valid. 
    // Bit1 valid for OPAL 
    //            - when set password buffer contains both old and new password.
    //            - when cleared password buffer contains only new password.
    Control &= 0x0103;          
    
    // Set Admin Password
    if ( Control & BIT0 ) {            
        OpalDeviceStatus = GetOpalSecurityStatus( BusInterface,  BusInterfaceType);
            
        if( !(OpalDeviceStatus & BIT1) ) {
            Status = SetAdminPassword(OpalDevice, Buffer);
        } else {
            Status = ChangeAdminPassword( OpalDevice, Buffer );
        }

    } else {
        // Check if both passwords present if not return error
        if ( !(Control & BIT1)) {
            return EFI_INVALID_PARAMETER;
        }
        
        // Set User Password
        Status = SetUserPassword(OpalDevice, Buffer);

    }

    return Status;
}

/**
    Unlocks Opal SED with Admin or User password      
        
    @param This    - AMI_HDD_SECURITY_PROTOCOL instance
    @param Control - Bit 0 : 0/1 User/Admin Password
                     Bit 1 : Password Buffer has 
                             Old Admin(Master) Password and 
                             New User/Master Password

    @param Buffer (32 Bytes) - Buffer with password data

    @retval EFI_SUCCESS : Device unlocked.
    @retval EFI_UNSUPPORTED : Not supported

**/
EFI_STATUS
EFIAPI
TcgSecurityUnlockPassword (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer
)
{
    EFI_STATUS            Status  = EFI_UNSUPPORTED;
    OPAL_DEVICE           *OpalDevice = NULL;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;
#if (SmmTcgStorageSec_SUPPORT) && !FORCE_TCG_OPAL_PASSWORD_PROMPT
    UINT32                TcgDeviceNvramId  = 0;
#endif    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }

    Control &= 0x0101;          // Bit0 and Bit 8 are valid

    // Unlock  with Admin Password
    if (Control & BIT0) {
        Status = UnlockOpalDrive(OpalDevice, Buffer, TRUE);

    } else {
        // Unlock  with User Password
        Status = UnlockOpalDrive(OpalDevice, Buffer, FALSE);
    }
    
    if (!(EFI_ERROR(Status))) {
        
#if SmmTcgStorageSec_SUPPORT
        // Unlocked Successfully, save Password to use it during s3 resume.
        SmmTcgPasswordInterface( This,
                                 Control,
                                 Buffer,
                                 SavePassword );
#if !FORCE_TCG_OPAL_PASSWORD_PROMPT
        Status = GatherTcgDeviceNvramInfo( This, &TcgDeviceNvramId );
        ASSERT_EFI_ERROR( Status );

        Status = SetTcgDeviceDataInNvram( &TcgDeviceNvramId,
                                          Control,
                                          Buffer );
        ASSERT_EFI_ERROR( Status );
#endif
#endif

    }    
    return Status;
}

/**
    Clears OPAL Admin or User password.
        
    @param This    - AMI_HDD_SECURITY_PROTOCOL instance
    @param Control - Bit 0 : 0/1 User/Admin Password
                     Bit 1 : Password Buffer has 
                             Old Admin(Master) Password and 
                             New User/Master Password

    @param Buffer (32 Bytes) - Buffer with password data

    @retval EFI_SUCCESS     : Password cleared.
    @retval EFI_UNSUPPORTED : Not supported

**/
EFI_STATUS
EFIAPI
TcgSecurityDisablePassword (
    IN  AMI_HDD_SECURITY_PROTOCOL       *This,
    IN  UINT16                          Control,
    UINT8                               *Buffer
)
{
    EFI_STATUS            Status  = EFI_UNSUPPORTED;
    OPAL_DEVICE           *OpalDevice = NULL;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }

    // Enterprise device doesn't support pwd disable
    if IS_ENTERPRISE_DEVICE(OpalDevice){
        return EFI_DEVICE_ERROR;
    }
    
    // Disable Opal Admin(Master) Password
    if ( Control & BIT0 ) {
        Status = DisableOpalAdminPassword( OpalDevice, Buffer );          
    } else {  
    // Disable Opal Admin(Master) Password
        Status = DisableOpalUserPassword(OpalDevice, Buffer ); 
    }
       
    return Status;
}

/**
    Issues BlockSID command if the device ownership is not taken prior.
    (when C_PIN_SID is equal to C_PIN_MISD).
    Issuing Block SID command will block attempts to authenticate the 
    SID authority until a subsequent device power cycle occurs.
        
    @param This    - AMI_HDD_SECURITY_PROTOCOL instance

    @retval EFI_SUCCESS     : OPAL device kept in security blocked state successfully.
    @retval EFI_UNSUPPORTED : Not supported
    
    @note It can be used by BIOS/platform firmware to prevent a malicious entity 
    from taking ownership of a SID credential that is still set to its default value of MSID. 

**/
EFI_STATUS
EFIAPI
TcgSecurityBlockSid (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This
)
{
    EFI_STATUS            Status  = EFI_UNSUPPORTED;
    OPAL_DEVICE           *OpalDevice = NULL;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif
  
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }

    Status = EnableBlockSID(OpalDevice, gBuffer, gBufferLength);
    return Status;
}

/**
    Erases the Device content

    @param This    - AMI_HDD_SECURITY_PROTOCOL instance
        
    @retval EFI_UNSUPPORTED : Not supported

**/
EFI_STATUS
EFIAPI
TcgSecurityEraseUnit (
    IN AMI_HDD_SECURITY_PROTOCOL    *This,
    IN UINT16                       Control,
    IN UINT8                        *Buffer
)
{
    return EFI_UNSUPPORTED;
}


/**
    Returns OPAL device security status, Password Variables and Flags.

    @param This - AMI_HDD_SECURITY_PROTOCOL instance
    @param PasswordFlags - Pointer to Flag data

    @retval EFI_SUCCESS : Password Status Flag is returned in IdePasswordFlags.
    @retval EFI_ERROR   : Error while retrieving the SecurityStatus.

    @note PasswordFlags:
     **TBD Only Bit0, Bit03, Bit11, Bit16, Bit17 are Supported Currently

         BIT0                             User Password Support;
         BIT1                             Master Password Support;
         BIT2                             Display User Password;
         BIT3                             Display Master Password;
         BIT4                             Display User Password Status;
         BIT5                             Display Master Password Status;
         BIT6                             Freeze Lock device;
         BIT7                             Single User Password;
         BIT8                             Single Master Password;
         BIT9                             System User Password as device User Password;
         BIT10                            System Admin Password as device Master Password;
         BIT11                            OEM Default Master Password
         BIT12                            Prompt for User Password in Post
         BIT13                            Prompt for Master Password in Post
         BIT14                            Display Security Status in Setup
         BIT15                            DUMMY Security Token.
         BIT16                            MasterPassword Set Status.
         BIT17                            UserPassword Set Status.
**/

EFI_STATUS
EFIAPI
TcgReturnIdePasswordFlags (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN OUT UINT32                   *PasswordFlags
)
{
    UINT16                SecurityStatus = 0;
    UINT32                Temp           = 0;   
    OPAL_DEVICE           *OpalDevice = NULL;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }

    Temp |= BIT1;

    // Display Master Password - always enable it for OPAL.
    Temp |= BIT3; 

    SecurityStatus = GetOpalSecurityStatus( BusInterface,  BusInterfaceType);

    // BIT1 from Locking feature can't be used for Enterprise
    if (IS_ENTERPRISE_DEVICE(OpalDevice)){
        if (OpalDevice->EnterpriseAdminPwdSts == UserEnabled) {
            Temp |= BIT16;    
        }
    } else {
        // Check if Password is set for the device (LockingFeature:BIT01)
        if (SecurityStatus & BIT1 ) { 

            Temp |= BIT16;

            if (OpalDevice->UserEnabledStatus == UserEnabled) {
                Temp |= BIT17;
            } else if (OpalDevice->UserEnabledStatus == UserEnabledStatusUnkown) {
                // Not able to find whether UserEnabled or not from the Authority Table due to invalid Admin/ User credentials,
                // So Update the UserPassword status as Unknown 
                Temp |= BIT4;
            }
        }
    }
    
    *PasswordFlags = (UINT32)Temp;

    return EFI_SUCCESS;
}

#if ENABLE_DEVICE_RESET_THRU_PSID
/**
    Reset the device to the Original Factory State using 32 byte PSID value.
    Function written for TCG Enterprise device.

    @param OpalDevice
    @param SessionParams
    @param PsidBuffer - Buffer with Psid data

    @retval EFI_SUCCESS           : Device reset succeeds.
    @retval EFI_INVALID_PARAMETER : 
**/
EFI_STATUS
ERevertUsingPsid (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    VOID                *PsidBuffer
)
{
    
    EFI_STATUS          Status;

    DEBUG ((DEBUG_VERBOSE, "\n PSID : %a\n", PsidBuffer));

    //Open AdminSP and Authenticate
    Status = OpenSessionAndAuthenticate (OpalDevice, SessionParams, gBuffer, gBufferLength, SP_ADMIN_UID, \
            PSID_AUTH_UID, PsidBuffer, (UINT32)AsciiStrLen((CHAR8 *) PsidBuffer));
    TCG_STORAGE_ERR_CHECK(Status);

    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = SpObjectUidRevert (OpalDevice, SessionParams, gBuffer, gBufferLength, THISSP_UID, REVERTSP_UID, FALSE);
    if (EFI_ERROR(Status)){
        SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
        return Status;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, SessionParams);
    
    if (EFI_ERROR(Status)){
        SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    }  else {
        // After revert session gets closed automatically.
        gBS->SetMem(SessionParams, sizeof (SESSION_PARAMS), 0);
    }
    
    return Status;
}

/**
    Reset the device to the Original Factory State using 32 byte PSID value.

    @param This    - AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL instance
        
    @param Buffer (32 Bytes) - Buffer with Psid data

    @retval EFI_SUCCESS           : Device reset succeeds.
    @retval EFI_INVALID_PARAMETER : 
**/
EFI_STATUS
EFIAPI
TcgStoragePsidRevert (
    IN AMI_TCG_STORAGE_DEVICE_RESET_PROTOCOL    *This,    
    IN UINT8                                    *PsidBuffer
)
{
    EFI_STATUS            Status;
    OPAL_DEVICE           *OpalDevice = NULL;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface
        = ((AMI_TCG_STORAGE_RESET_INTERFACE* )This)->TcgStorageSecurityInterface->BusInterface;
    
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType
        = ((AMI_TCG_STORAGE_RESET_INTERFACE* )This)->TcgStorageSecurityInterface->BusInterfaceType;
    UINTN    Index, TempPsidBufferLength;
    UINT8    *TempPsidBuffer;
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)((SATA_DEVICE_INTERFACE*)BusInterface)->OpalConfig;
    }
#endif
    
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }

    TempPsidBufferLength = AsciiStrLen((CHAR8 *) PsidBuffer);
    
    Status = gBS->AllocatePool( EfiBootServicesData,
                                TempPsidBufferLength + 1,
                                (VOID**)&TempPsidBuffer );

    if ( EFI_ERROR( Status )) {
        ASSERT_EFI_ERROR(Status);
        return Status;
    }

    gBS->SetMem(TempPsidBuffer, TempPsidBufferLength + 1, 0);
    
    for (Index = 0; Index < TempPsidBufferLength; Index++) {
        TempPsidBuffer[Index] = PsidBuffer[TempPsidBufferLength-1-Index];
    }
    
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = ERevertUsingPsid(OpalDevice, gSessionParams, TempPsidBuffer);
    } else {
        Status = RevertUsingPsid(OpalDevice, gSessionParams, TempPsidBuffer);
    }
    
    gBS->FreePool (TempPsidBuffer);

    return Status; 
}

#endif
/** @internal
    Initializes OPAL device configuration details for TCG Security protocol 
    communication.
        
    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval EFI_SUCCESS     : OPAL config initialized successfully
    
    @endinternal
**/
EFI_STATUS
InitializeOpalConfig (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif 
    EFI_STATUS                     Status;
    UINT64                         TableObjUid;
    OPAL_DEVICE                    *OpalDevice = NULL;
    BOOLEAN                        RstControlled = FALSE; 
    
    Status = gBS->AllocatePool( EfiBootServicesData,
                                    sizeof(OPAL_DEVICE),
                                    (VOID**)&OpalDevice );
    
    gBS->SetMem( OpalDevice, sizeof (OPAL_DEVICE), 0);
    TCG_STORAGE_ERR_CHECK(Status);

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice->StorageSecurityProtocol = SataDevInterface->StorageSecurityInterface;
        OpalDevice->MediaId = SataDevInterface->SataBlkIo->BlkIo.Media->MediaId;
        OpalDevice->PortNumber = SataDevInterface->PortNumber;
        OpalDevice->PMPortNumber = SataDevInterface->PMPortNumber;
        OpalDevice->DeviceHandle = SataDevInterface->IdeDeviceHandle;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled) {
            OpalDevice->StorageSecurityProtocol = (EFI_STORAGE_SECURITY_COMMAND_PROTOCOL *)NvmeRstInterface->EfiStorageSecInterface;
            OpalDevice->MediaId = 0;  // Force Media ID to zero
            OpalDevice->DeviceHandle = NvmeRstInterface->ControllerHandle;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32 );
            OpalDevice->StorageSecurityProtocol = &(ActiveNameSpace->NvmeSecurityStorageCmd);
            OpalDevice->MediaId = 0;  // Force Media ID to zero
            OpalDevice->DeviceHandle = NvmeDevInterface->ControllerHandle;
        }
#endif
    }
    Status = GetLevel0DiscoveryData (OpalDevice);
    
    // Return EFI_UNSUPPORTED Error if Unsupported TCG OPAL drive is connected
    if((!OpalDevice->OpalFeature.FeatureCode) || EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "TCG Password feature is not installed as Unsupported TCG OPAL drive connected \n"));
        return EFI_UNSUPPORTED;
    }

    if (!gSessionParams) {
        Status = gBS->AllocatePool(EfiBootServicesData, sizeof (SESSION_PARAMS), (VOID**)&gSessionParams);
        TCG_STORAGE_ERR_CHECK(Status);
    }

    if (!gBuffer) {
        gBufferLength = 512;
        Status = gBS->AllocatePool(EfiBootServicesData, gBufferLength, (VOID**)&gBuffer);
        TCG_STORAGE_ERR_CHECK(Status);
    }

    gBS->SetMem(gSessionParams, sizeof (SESSION_PARAMS), 0);            

    //Get the properties from the TPer
    Status = GetTperProperties (OpalDevice, 
                                gSessionParams, 
                                gBuffer, 
                                gBufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
    
    // Get C_PIN_MSID
    DEBUG ((DEBUG_VERBOSE, "Get  C_PIN_MSID_UID\n"));
    TableObjUid = C_PIN_MSID_UID;
    Status = GetCpin ( OpalDevice, 
                                gSessionParams, 
                                TableObjUid, 
                                (VOID *)&(OpalDevice->CPinMsidLength), 
                                (VOID *)&(OpalDevice->pCPinMsid), 
                                gBuffer, gBufferLength);
     TCG_STORAGE_ERR_CHECK(Status);

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        SataDevInterface->OpalConfig = (VOID *)OpalDevice;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        if(RstControlled) {
            NvmeRstInterface->OpalConfig = (VOID *)OpalDevice;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            NvmeDevInterface->OpalConfig = (VOID *)OpalDevice;
        }
#endif
    }
    return Status;
}
/** @internal
    Updates OPAL device Level 0 data LEVEL0_DISCOVERY_DATA

    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval EFI_SUCCESS : Level 0 data updated successfully.
    @retval EFI_ERROR   : Failed to update Level 0 data.
    
    @endinternal
**/
EFI_STATUS
UpdateLevel0Data (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
    BOOLEAN            RstControlled = FALSE;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif  
    EFI_STATUS                     Status = EFI_SUCCESS;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        Status = GetLevel0DiscoveryData ( (OPAL_DEVICE*)(SataDevInterface->OpalConfig));
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled) {
            Status = GetLevel0DiscoveryData ( (OPAL_DEVICE*)(NvmeRstInterface->OpalConfig));
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            Status = GetLevel0DiscoveryData ( (OPAL_DEVICE*)(NvmeDevInterface->OpalConfig));
        }
#endif
    }
    
    ASSERT_EFI_ERROR(Status);
    return Status;
}

/** @internal
    Wrapper function to get OPAL device Level 0 data.
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE

    @retval EFI_SUCCESS - Success
            EFI_OUT_OF_RESOURCES - Buffer allocate failed
            
    @endinternal
**/
EFI_STATUS
GetLevel0DiscoveryData (
    IN  OPAL_DEVICE         *OpalDevice
)
{
    EFI_STATUS                  Status;
    LEVEL0_DISCOVERY_DATA       *QueryBuffer = NULL;
    UINTN                       QueryBufferSize = 0x200;
    
    Status = gBS->AllocatePool( EfiBootServicesData, QueryBufferSize, (VOID**)&QueryBuffer);
    if (EFI_ERROR(Status)) {
        return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_VERBOSE, "\nGetLevel0DiscoveryData QueryBuffer : %lx\n", QueryBuffer));
    gBS->SetMem(QueryBuffer, QueryBufferSize, 0);
    
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }
    
    Status = OpalDevice->StorageSecurityProtocol->ReceiveData( OpalDevice->StorageSecurityProtocol,
                        OpalDevice->MediaId,
                        OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme ? MultU64x32(NVME_COMMAND_TIMEOUT, 10000000) : MultU64x32(SATA_COMMAND_TIMEOUT, 10000000),
                        SECURITY_PROTOCOL_1,   // Security Protocol ID
                        ToBigLittleEndianWord(COMID_LEVEL0_DISCOVERY), // SP specific id
                        0x200,                 // TranferLength
                        QueryBuffer,
                        &QueryBufferSize
                        );
    DEBUG ((DEBUG_VERBOSE, "GetLevel0DiscoveryData Status : %r\n", Status));
    if (!EFI_ERROR(Status)) {
        UpdatePrintLevel0DiscoveryData(OpalDevice, QueryBuffer);
    }
   
    gBS->FreePool (QueryBuffer);
    return Status;
}

/** @internal
    Internal Wrapper function to update OPAL device Level 0 data into current 
    device structure.

    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param QueryBuffer - Pointer to LEVEL0_DISCOVERY_DATA buffer

    @retval EFI_SUCCESS 
    
    @endinternal
**/
VOID
UpdatePrintLevel0DiscoveryData(
    OPAL_DEVICE             *OpalDevice,
    LEVEL0_DISCOVERY_DATA   *QueryBuffer
)
{
    FEATURE_DESCRIPTOR               *FeatureDescriptor;
    UINT8                            Byte;
    UINT16                           FeatureCode;
    FEATURE_DESCRIPTOR_GEOMETRY      *Geometry;
    FEATURE_DESCRIPTOR_OPAL_SSC_V200 *OpalSSCV200=NULL;
    FEATURE_DESCRIPTOR_OPALITE_SSC   *OpaliteSSCFeature;
    
#if defined(ENTERPRISE_SSC_DEVICE_SUPPORT) && (ENTERPRISE_SSC_DEVICE_SUPPORT == 1)
    FEATURE_DESCRIPTOR_ENTERPRISE_SSC  *EnterpriseSSCFeature=NULL;
#endif
    FEATURE_DESCRIPTOR_BLOCK_SID     *BlockSid;  
    UINTN   TotalLength = ToBigLittleEndianDword(QueryBuffer->Level0DiscoveryHeader.Length) + 4;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
    DEBUG ((DEBUG_VERBOSE, "Level0 Discovery Data\n"));
#endif
    FeatureDescriptor = (FEATURE_DESCRIPTOR *)((UINT8 *)QueryBuffer + sizeof(LEVEL0_DISCOVERY_HEADER));
    do {

        FeatureCode = ToBigLittleEndianWord(FeatureDescriptor->FeatureCode);

        switch (FeatureCode) {

            case    TPer_Feature:
                Byte = FeatureDescriptor->Data[0];
                OpalDevice->TPerFeature = Byte;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)                 
                DEBUG ((DEBUG_VERBOSE, "TPer_Feature:\n"));
                DEBUG ((DEBUG_VERBOSE, "COMID Mgmt  Support  : %x\n", (Byte & 0x40) ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Streaming   Support  : %x\n", (Byte & 0x10) ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Buffer Mgmt Support  : %x\n", (Byte & 0x8) ? TRUE : FALSE));   
                DEBUG ((DEBUG_VERBOSE, "Ack/Nak Support      : %x\n", (Byte & 0x4)  ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Async   Support      : %x\n", (Byte & 0x2)  ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Sync   Support       : %x\n", (Byte & 0x1)  ? TRUE : FALSE));  
#endif                
                break;
            case    Locking_Feature:
                OpalDevice->LockingFeature = FeatureDescriptor->Data[0];
                Byte = FeatureDescriptor->Data[0];
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)                 
                DEBUG ((DEBUG_VERBOSE, "\nLocking_Feature:\n"));
                DEBUG ((DEBUG_VERBOSE, "MBR Shadow Absent    : %x\n", (Byte & 0x40) ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "MBR Done             : %x\n", (Byte & 0x20) ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "MBR Enabled          : %x\n", (Byte & 0x10) ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Media Encryption     : %x\n", (Byte & 0x8)  ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Locked               : %x\n", (Byte & 0x4)  ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Locking Enabled      : %x\n", (Byte & 0x2)  ? TRUE : FALSE));  
                DEBUG ((DEBUG_VERBOSE, "Locking Supported    : %x\n", (Byte & 0x1)  ? TRUE : FALSE));  
#endif                
                break;

            case    Geometry_Reporting_Feature:

                Geometry = (FEATURE_DESCRIPTOR_GEOMETRY *)FeatureDescriptor;
                gBS->CopyMem(&(OpalDevice->GeometryFeature), Geometry, sizeof(FEATURE_DESCRIPTOR_GEOMETRY));
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)                 
                DEBUG ((DEBUG_VERBOSE, "\nGeometry_Reporting : %2X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "Logical Block Size   : %8X\n", ToBigLittleEndianDword(Geometry->LogicalBlockSize)));
                DEBUG ((DEBUG_VERBOSE, "AlignmentGranularity : %16X\n", ToBigLittleEndianQword(Geometry->AlignmentGranularity)));
                DEBUG ((DEBUG_VERBOSE, "LowestAlignedLBA     : %16x\n", ToBigLittleEndianQword(Geometry->LowestAlignedLBA)));
#endif                
                break;

            case    OPAL_SSC_V200_Feature:

                OpalSSCV200 = (FEATURE_DESCRIPTOR_OPAL_SSC_V200 *) FeatureDescriptor;
                OpalDevice->OpalFeature.FeatureCode = ToBigLittleEndianWord(OpalSSCV200->FeatureCode);
                OpalDevice->OpalFeature.BaseComID = ToBigLittleEndianWord(OpalSSCV200->BaseComID);
                OpalDevice->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(OpalSSCV200->NumberofComIDs);
                OpalDevice->OpalFeature.Byte8 = OpalSSCV200->Byte8;
                OpalDevice->OpalFeature.SPAdminAuth = ToBigLittleEndianWord(OpalSSCV200->SPAdminAuth);
                OpalDevice->OpalFeature.SPUserAuth = ToBigLittleEndianWord(OpalSSCV200->SPUserAuth);
                OpalDevice->OpalFeature.CPinSid = OpalSSCV200->CPinSid;
                OpalDevice->OpalFeature.CPinSidRevert = OpalSSCV200->CPinSidRevert;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
                DEBUG ((DEBUG_VERBOSE, "\nOPAL_SSC_V200      : %4X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "BaseComID            : %4X\n", OpalDevice->OpalFeature.BaseComID));
                DEBUG ((DEBUG_VERBOSE, "NumberofComIDs       : %4X\n", OpalDevice->OpalFeature.NumberofComIDs));
                DEBUG ((DEBUG_VERBOSE, "RangeCross Over      : %2X\n", OpalDevice->OpalFeature.Byte8));
                DEBUG ((DEBUG_VERBOSE, "SPAdminAuth          : %4X\n", OpalDevice->OpalFeature.SPAdminAuth));
                DEBUG ((DEBUG_VERBOSE, "SPUserAuth           : %4X\n", OpalDevice->OpalFeature.SPUserAuth));
                DEBUG ((DEBUG_VERBOSE, "CPinSid              : %2X\n", OpalDevice->OpalFeature.CPinSid));
                DEBUG ((DEBUG_VERBOSE, "CPinSidRevert        : %2X\n", OpalDevice->OpalFeature.CPinSidRevert));
#endif                
                break;

            case    OPALITE_SSC_Feature:

                OpaliteSSCFeature = (FEATURE_DESCRIPTOR_OPALITE_SSC *) FeatureDescriptor;
                OpalDevice->OpalFeature.FeatureCode = ToBigLittleEndianWord(OpaliteSSCFeature->FeatureCode);
                OpalDevice->OpalFeature.BaseComID = ToBigLittleEndianWord(OpaliteSSCFeature->BaseComID);
                OpalDevice->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(OpaliteSSCFeature->NumberofComIDs);
                if(OpalSSCV200) {
                    OpalDevice->OpalFeature.CPinSid = OpalSSCV200->CPinSid;
                    OpalDevice->OpalFeature.CPinSidRevert = OpalSSCV200->CPinSidRevert;
                }
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)                 
                DEBUG ((DEBUG_VERBOSE, "\nOPAL_SSC_OPALITE   : %4X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "BaseComID            : %4X\n", OpalDevice->OpalFeature.BaseComID));
                DEBUG ((DEBUG_VERBOSE, "NumberofComIDs        : %4X\n", OpalDevice->OpalFeature.NumberofComIDs));
                DEBUG ((DEBUG_VERBOSE, "CPinSid              : %2X\n", OpalDevice->OpalFeature.CPinSid));
                DEBUG ((DEBUG_VERBOSE, "CPinSidRevert        : %2X\n", OpalDevice->OpalFeature.CPinSidRevert));
#endif                
                break;

            case    PYRITE_SSC_Feature:
            case    PYRITE_SSC_Feature_v2_0:
                OpaliteSSCFeature = (FEATURE_DESCRIPTOR_OPALITE_SSC *) FeatureDescriptor;
                OpalDevice->OpalFeature.FeatureCode = ToBigLittleEndianWord(OpaliteSSCFeature->FeatureCode);
                OpalDevice->OpalFeature.BaseComID = ToBigLittleEndianWord(OpaliteSSCFeature->BaseComID);
                OpalDevice->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(OpaliteSSCFeature->NumberofComIDs);
                if(OpalSSCV200) {
                    OpalDevice->OpalFeature.CPinSid = OpalSSCV200->CPinSid;
                    OpalDevice->OpalFeature.CPinSidRevert = OpalSSCV200->CPinSidRevert;
                }
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)                 
                DEBUG ((DEBUG_VERBOSE, "\nOPAL_SSC_PYRITE    : %4X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "BaseComID            : %4X\n", OpalDevice->OpalFeature.BaseComID));
                DEBUG ((DEBUG_VERBOSE, "NumberofComIDs        : %4X\n", OpalDevice->OpalFeature.NumberofComIDs));
                DEBUG ((DEBUG_VERBOSE, "CPinSid              : %2X\n", OpalDevice->OpalFeature.CPinSid));
                DEBUG ((DEBUG_VERBOSE, "CPinSidRevert        : %2X\n", OpalDevice->OpalFeature.CPinSidRevert));
#endif                
                break;

// If Enterprise SED device support not enabled, don't update the internal structure. This device won't be handled.
#if defined(ENTERPRISE_SSC_DEVICE_SUPPORT) && (ENTERPRISE_SSC_DEVICE_SUPPORT == 1)
            case    ENTERPRISE_SSC_Feature:
                
                EnterpriseSSCFeature = (FEATURE_DESCRIPTOR_ENTERPRISE_SSC *) FeatureDescriptor;
                OpalDevice->OpalFeature.FeatureCode = FeatureCode;
                OpalDevice->OpalFeature.BaseComID = ToBigLittleEndianWord(EnterpriseSSCFeature->BaseComID);
                OpalDevice->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(EnterpriseSSCFeature->NumberofComIDs);
                OpalDevice->OpalFeature.CPinSid = 0;
                OpalDevice->OpalFeature.CPinSidRevert = 0;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
                DEBUG ((DEBUG_VERBOSE, "\nENTERPRISE_SSC            : %4X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "BaseComID                   : %4X\n", OpalDevice->OpalFeature.BaseComID));
                DEBUG ((DEBUG_VERBOSE, "NumberofComIDs              : %4X\n", OpalDevice->OpalFeature.NumberofComIDs));
                DEBUG ((DEBUG_VERBOSE, "RangeCrossing               : %2X\n", EnterpriseSSCFeature->RangeCrossing));
#endif
                break;
#endif

                
// If Ruby SED device support is not enabled, don't update the internal structure. This device won't be handled.
#if defined(RUBY_SSC_DEVICE_SUPPORT) && (RUBY_SSC_DEVICE_SUPPORT == 1)
            case RUBY_SSC_Feature:
                OpalSSCV200 = (FEATURE_DESCRIPTOR_OPAL_SSC_V200 *) FeatureDescriptor;
                OpalDevice->OpalFeature.FeatureCode = ToBigLittleEndianWord(OpalSSCV200->FeatureCode);
                OpalDevice->OpalFeature.BaseComID = ToBigLittleEndianWord(OpalSSCV200->BaseComID);
                OpalDevice->OpalFeature.NumberofComIDs = ToBigLittleEndianWord(OpalSSCV200->NumberofComIDs);
                OpalDevice->OpalFeature.Byte8 = OpalSSCV200->Byte8;
                OpalDevice->OpalFeature.SPAdminAuth = ToBigLittleEndianWord(OpalSSCV200->SPAdminAuth);
                OpalDevice->OpalFeature.SPUserAuth = ToBigLittleEndianWord(OpalSSCV200->SPUserAuth);
                OpalDevice->OpalFeature.CPinSid = OpalSSCV200->CPinSid;
                OpalDevice->OpalFeature.CPinSidRevert = OpalSSCV200->CPinSidRevert;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
                DEBUG ((DEBUG_VERBOSE, "\nRUBY_SSC_V100      : %4X\n", FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "BaseComID            : %4X\n", OpalDevice->OpalFeature.BaseComID));
                DEBUG ((DEBUG_VERBOSE, "NumberofComIDs       : %4X\n", OpalDevice->OpalFeature.NumberofComIDs));
                DEBUG ((DEBUG_VERBOSE, "RangeCross Over      : %2X\n", OpalDevice->OpalFeature.Byte8));
                DEBUG ((DEBUG_VERBOSE, "SPAdminAuth          : %4X\n", OpalDevice->OpalFeature.SPAdminAuth));
                DEBUG ((DEBUG_VERBOSE, "SPUserAuth           : %4X\n", OpalDevice->OpalFeature.SPUserAuth));
                DEBUG ((DEBUG_VERBOSE, "CPinSid              : %2X\n", OpalDevice->OpalFeature.CPinSid));
                DEBUG ((DEBUG_VERBOSE, "CPinSidRevert        : %2X\n", OpalDevice->OpalFeature.CPinSidRevert));
#endif                
                break;
#endif

            case Block_SID_Authentification_Feature:
                BlockSid = (FEATURE_DESCRIPTOR_BLOCK_SID *) FeatureDescriptor;
                OpalDevice->BlockSid.FeatureCode = ToBigLittleEndianWord(BlockSid->FeatureCode);
                OpalDevice->BlockSid.SIDStateValue = BlockSid->SIDStateValue;
                OpalDevice->BlockSid.SIDBlockedState = BlockSid->SIDBlockedState;                   
                OpalDevice->BlockSid.HWReset = BlockSid->HWReset;
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
                DEBUG ((DEBUG_VERBOSE, "\n Block_SID_Authentification_Feature    : %4X\n", OpalDevice->BlockSid.FeatureCode));
                DEBUG ((DEBUG_VERBOSE, "SIDStateValue          : %2X\n", OpalDevice->BlockSid.SIDStateValue));
                DEBUG ((DEBUG_VERBOSE, "SIDBlockedState        : %2X\n", OpalDevice->BlockSid.SIDBlockedState));
                DEBUG ((DEBUG_VERBOSE, "HWReset                : %2X\n", OpalDevice->BlockSid.HWReset));
#endif
                break;

            default:
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)              
                DEBUG ((DEBUG_VERBOSE, "\nUnKnown Feature Code %2X\n", FeatureCode));
#endif
                break;
        }

        FeatureDescriptor = (FEATURE_DESCRIPTOR *)((UINT8 *)FeatureDescriptor + FeatureDescriptor->Length + 4);

    } while ((UINT8 *)FeatureDescriptor < (UINT8 *)((UINT8 *)QueryBuffer + TotalLength));
    
    return;
}

/** @internal
    Returns current security status of the device.
    Security status information will be retrieved from buffer
    updated by LEVEL 0 data.
        
    @param This - AMI_HDD_SECURITY_PROTOCOL instance
    @param UINT16 - Security status


    @retval UINT16      *SecurityStatus
    @retval             Bit 0 : Security Supported
    @retval             Bit 1 : Security Enabled
    @retval             Bit 2 : Security Locked
    @retval             Bit 3 : Security Frozen
    @retval             Bit 4 : Security Count Expired
    @retval             Bit 5 : Enhanced Security Erase supported
    @retval             Bit 6 : Enhanced Security Erase supported
    @retval             Bit 8 : Security Level 0:High, 1:Maximum
    @retval             Bit 9 : Enterrpise TCG device
    @endinternal
**/

UINT16
GetOpalSecurityStatus(
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE           *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif
    UINT16                         OpalSecurityStatus = 0;
    OPAL_DEVICE                    *OpalConfig = NULL;
    BOOLEAN                        RstControlled = FALSE;
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalConfig = (OPAL_DEVICE*)SataDevInterface->OpalConfig;
    }
#endif
    
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled) {
            OpalConfig = (OPAL_DEVICE*)NvmeRstInterface->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalConfig = (OPAL_DEVICE*)NvmeDevInterface->OpalConfig;
        }
#endif
    }
    
    if(OpalConfig == NULL) {
        return OpalSecurityStatus;
    }
    
    OpalSecurityStatus = OpalConfig->LockingFeature;
    OpalSecurityStatus &= OPAL_SECURITY_LOCKING_FEATURE_MASK;

    if (IS_ENTERPRISE_DEVICE(OpalConfig)){
        EnterpriseLockedStatus (OpalConfig);
        OpalSecurityStatus &= ~(BIT1 + BIT2);
        OpalSecurityStatus |= ((OpalConfig->LockingFeature & 4 ) ? 4 : 0); //Locked
        OpalSecurityStatus |= (OpalConfig->EnterpriseAdminPwdSts == UserEnabled ? 2 : 0); // If admin pwd installed, report security enabled.
        OpalSecurityStatus |= BIT9;  // Indicate Enterprise SED
        
    }
    // Update OPAL Security FROZEN state
    if (OpalConfig->BlockSid.SIDBlockedState) {
        OpalSecurityStatus |= BIT3;
    }
    DEBUG ((DEBUG_VERBOSE,"\n GetOpalSecurityStatus=%x", OpalSecurityStatus));
    return OpalSecurityStatus;
    
}

#if SmmTcgStorageSec_SUPPORT
/** @internal
    Manages OPAL Password in SMM area.

    @param This    - AMI_HDD_SECURITY_PROTOCOL instance
    @param Control - Bit 0 : 0/1 User/Master Password
                     Bit 1 : Password Buffer has 
                             Old Admin(Master) Password and 
                             New User/Master Password

    @param Buffer (32 Bytes) - Buffer with password data
    @param Action - value from TCG_STORAGE_SEC_PASSWORD_ACTION

    @retval EFI_SUCCESS      : Password saved which will be used to unlock the drive
                               during S3 resume.
    @retval EFI_NOT_FOUND : Action not valid

    @note  
      1. Buffer should be 32 bytes long (Mandatory). Even if Password is not
     32 bytes long, pad it with zeros to create a buffer of 32 bytes.
    
    @endinternal
**/

EFI_STATUS
SmmTcgPasswordInterface (
    IN  AMI_HDD_SECURITY_PROTOCOL   *This,
    IN  UINT16                      Control,
    IN  UINT8                       *Buffer,
    IN  TCG_STORAGE_SEC_PASSWORD_ACTION     Action
)
{
    EFI_STATUS                 Status;
    UINTN                      Index;
    EFI_SMM_COMMUNICATE_HEADER *SmmCommunicateHeader;
    UINTN                      CommunicateBufferSize;
    TCG_STORAGE_SECURITY_PASSWORD   *OpalPasswordBuffer;
    VOID                       *BusInterface   \
        = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE    BusInterfaceType   \
        = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )This)->BusInterfaceType;

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
    AMI_NVME_CONTROLLER_PROTOCOL    *NvmeDevInterface = (AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface;
    ACTIVE_NAMESPACE_DATA           *ActiveNameSpace = NULL;
#endif
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) )    
    NVME_RST_INTERFACE              *NvmeRstInterface = (NVME_RST_INTERFACE*)BusInterface;
#endif
    BOOLEAN             RstControlled = FALSE;
    EFI_PCI_IO_PROTOCOL *PciIo = NULL;
    UINTN               SegNum;
    UINTN               BusNum;
    UINTN               DevNum;
    UINTN               FuncNum;
	
    // Calculate Size of Communication buffer
    CommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + TCG_STORAGE_SECURITY_PASSWORD_SIZE + (2* OPAL_PASSWORD_LENGTH);

    gBS->SetMem( gTcgStorageSecCommunicateBuffer,CommunicateBufferSize, 0);
        
    // Copy Smm Communicate Header Here
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gTcgStorageSecCommunicateBuffer;

    if (Action == SavePassword || Action == SaveDeviceInfo) {
        gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgStorageSecSavePwdGuid, sizeof( EFI_GUID ));
            
    } else if (Action == RemovePassword) {
        gBS->CopyMem( &SmmCommunicateHeader->HeaderGuid, &gTcgStorageSecRemovePwdGuid, sizeof( EFI_GUID ) );
           
    } else {
        ASSERT(FALSE);
        return EFI_NOT_FOUND;
    }

    SmmCommunicateHeader->MessageLength  = TCG_STORAGE_SECURITY_PASSWORD_SIZE + (2* OPAL_PASSWORD_LENGTH);
    
    //Copy TCG_STORAGE_SECURITY_PASSWORD here
    OpalPasswordBuffer = (TCG_STORAGE_SECURITY_PASSWORD *) &SmmCommunicateHeader->Data;
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        PciIo = SataDevInterface->AhciBusInterface->PciIO;
        OpalPasswordBuffer->DevHandle = SataDevInterface->IdeDeviceHandle;
        OpalPasswordBuffer->PortNumber = SataDevInterface->PortNumber;
        OpalPasswordBuffer->PMPortNumber = SataDevInterface->PMPortNumber;
    }
#endif
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(NvmeRstInterface->Signature);
        if(RstControlled) {
            PciIo = NvmeRstInterface->PciIO;
            OpalPasswordBuffer->DevHandle = NvmeRstInterface->ControllerHandle;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            ActiveNameSpace = GetActiveNameSpace( NvmeDevInterface, MAX_UINT32);        
            PciIo = NvmeDevInterface->PciIO;
            OpalPasswordBuffer->DevHandle = ActiveNameSpace->NvmeDeviceHandle;
        }
        OpalPasswordBuffer->RstControlled = RstControlled;
#endif
    }

    if(!PciIo) {
        ASSERT(FALSE);
        return EFI_NOT_FOUND;
    }
    
    // Get PCI Bus/Device/Function number
    Status = PciIo->GetLocation ( PciIo,
                                  &SegNum,
                                  &BusNum,
                                  &DevNum,
                                  &FuncNum
                                  );
    ASSERT_EFI_ERROR (Status);

    // Save it to initialize the controller.
    OpalPasswordBuffer->BusNo=(UINT8)BusNum;
    OpalPasswordBuffer->DeviceNo=(UINT8)DevNum;
    OpalPasswordBuffer->FunctionNo=(UINT8)FuncNum;

    OpalPasswordBuffer->Signature = OPAL_PASSWORD_SIGNATURE; // 0x4C504F24; //
    OpalPasswordBuffer->Control   = Control;
    OpalPasswordBuffer->BusInterfaceType  = BusInterfaceType;
    
    // SaveDeviceInfo does not contain password information so skip below code
    if (Action == SavePassword) {
        if ( Control & 0x01 ) {
            // gTcgStorageSecCommunicateBuffer comprises SmmCommunicateHeader, TCG_STORAGE_SECURITY_PASSWORD data and User & Master password
            OpalPasswordBuffer->MasterPassword = (UINT8 *)&SmmCommunicateHeader->Data + TCG_STORAGE_SECURITY_PASSWORD_SIZE;
            for ( Index = 0; Index < OPAL_PASSWORD_LENGTH; Index++ ) {
                OpalPasswordBuffer->MasterPassword[Index] = Buffer[Index];
            } // end of for
        } else {
            // User password is located after Master password in gTcgStorageSecCommunicateBuffer
            OpalPasswordBuffer->UserPassword = (UINT8 *)&SmmCommunicateHeader->Data + TCG_STORAGE_SECURITY_PASSWORD_SIZE + OPAL_PASSWORD_LENGTH;
            for ( Index = 0; Index < OPAL_PASSWORD_LENGTH; Index++ ) {
                OpalPasswordBuffer->UserPassword[Index] = Buffer[Index];
            } // end of for
        }
    }

    if(gSmmCommunication == NULL) {
        
        Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);

        if (EFI_ERROR(Status)) {
            ASSERT_EFI_ERROR(Status);
            return Status;  
        }
    }

    // Send data to SMM.
    Status = gSmmCommunication->Communicate (gSmmCommunication, gTcgStorageSecCommunicateBuffer, &CommunicateBufferSize);

    if (EFI_ERROR(Status)) {
        ASSERT_EFI_ERROR(Status);
        return Status;  
    }
    // Clear the Password data stored in global buffer
    gBS->SetMem(gTcgStorageSecCommunicateBuffer,CommunicateBufferSize + (2*OPAL_PASSWORD_LENGTH),0);
    return EFI_SUCCESS;
}
#endif

/** @internal
    Sets OPAL Admin Password. 
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to buffer with Admin password

    @retval EFI_STATUS
    
    @note
    Once set successfully, device will be locked immediately.
    
    @endinternal
**/

EFI_STATUS
SetAdminPassword (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
)
{  
    EFI_STATUS          Status;
    
    // Initialize and take device ownership
    Status = InitAdminSPCPinSid(OpalDevice, NULL, AdminPassword);
    if (!EFI_ERROR(Status)) {
        Status = ActivateLockingSP (OpalDevice, AdminPassword);
        if (!EFI_ERROR(Status)) {
            // Initialize UserPwd Status as UserNotEnabled
            OpalDevice->UserEnabledStatus = UserNotEnabled;
            Status = EnableGlobalRange (OpalDevice, AdminPassword);
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
            if (!EFI_ERROR(Status)) {
                Status = EnableTperReset( OpalDevice, AdminPassword );
            }
#endif
        }
    }
    return Status;  
}

/** @internal
    Programs the Device reset behavior for Power cycles

    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to the Admin Password buffer

    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
EnableTperReset (
    IN   OPAL_DEVICE         *OpalDevice,
    IN   UINT8               *AdminPassword
)
{
    EFI_STATUS          Status;
    UINT8               ColumnValue;
    CHAR8               ColumnName8[] = "ProgrammaticResetEnable";

    DEBUG ((DEBUG_VERBOSE, "\n Enable ProgrammaticReset value of TPER_INFO table \n"));
    gBS->SetMem(gBuffer, gBufferLength, 0);

    // TPER Reset isn't working in Enterprise device
    if (IS_ENTERPRISE_DEVICE(OpalDevice)){
        return EFI_SUCCESS;
    }

    // Open session to Admin SP as  the SID authority using the MSID password
    gSessionParams->InvokingID = SM_UID;
    gSessionParams->MethodID = START_SESSION_UID;
    gSessionParams->HostSessionID = 1;
    gSessionParams->SPID = SP_ADMIN_UID;
    gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) AdminPassword);
    gSessionParams->HostChallenge = AdminPassword; //OpalDevice->pCPinSid;
    gSessionParams->HostSigningAuthority = SID_AUTH_UID;
    gSessionParams->Write = 1;

    Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    TCG_STORAGE_ERR_CHECK(Status);

    ColumnValue = 1;
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, TPERINFO_UID,\
                    8, ColumnName8, &ColumnValue,1, TRUE, ENUMERATION_TYPES);
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    DEBUG ((DEBUG_VERBOSE, "Status = %r \n", Status));
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);

CloseSession: 
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    return Status;
}


/** @internal
    Function issues Tper Reset command to the device

    @param BusInterface  - Pointer to current bus interface
    @param BusInterfaceType - Pointer to bus interface type AHCI/ID

    @retval EFI_SUCCESS                      : TPER Reset is success.
    @retval EFI_DEVICE_ERROR/EFI_UNSUPPORTED : TPER Reset is not success.

    @endinternal
**/
EFI_STATUS
IssueTperReset (
    IN  VOID                       *BusInterface,
    IN  AMI_STORAGE_BUS_INTERFACE  BusInterfaceType
)
{
    OPAL_DEVICE                     *OpalDevice = NULL;
    BOOLEAN                         RstControlled = FALSE;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE           *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif
    EFI_STATUS                      Status;

    if ( BusInterfaceType == AmiStorageInterfaceIde ) {
        return EFI_UNSUPPORTED;
    }
    
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        OpalDevice = (OPAL_DEVICE*)SataDevInterface->OpalConfig;
    }
#endif
    
    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((NVME_RST_INTERFACE*)BusInterface)->OpalConfig;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) {
            OpalDevice = (OPAL_DEVICE*)((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->OpalConfig;
        }
#endif
    }
    if(NULL == OpalDevice) {
        return EFI_NOT_FOUND;
    }
    
    // TPER Reset isn't working in Enterprise device
    gBS->SetMem(gBuffer, gBufferLength, 0);
    if (IS_ENTERPRISE_DEVICE(OpalDevice)){
        Status = EFI_SUCCESS;
    } else {
        Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_2, ToBigLittleEndianWord(TPER_RESET), gBuffer, gBufferLength);
    }

    DEBUG ((DEBUG_VERBOSE, "IssueTperReset Status = %r \n", Status ));
    return Status;
}

/** @internal
    Changes OPAL Locking SP Admin Password. 
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param OldPassword - Pointer to buffer with Old password
    @param NewPassword - Pointer to buffer with New password

    @retval EFI_STATUS
    
    @endinternal    
**/
EFI_STATUS
ChangeLockingSPAdminPassword(
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               *OldPassword,
    IN  UINT8               *NewPassword
)
{
    EFI_STATUS          Status;
    CHAR8               ColumnName3[]="PIN";
          
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    // Open session to Admin SP as  the SID authority using the old password
    gSessionParams->InvokingID = SM_UID;
    gSessionParams->MethodID = START_SESSION_UID;
    gSessionParams->HostSessionID = 1;
    gSessionParams->SPID = LOCKING_SP_UID;
    gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) OldPassword);//OpalDevice->LockingSPAdmin1Length;
    gSessionParams->HostChallenge = OldPassword; //OpalDevice->LockingSPAdmin1Pin;
    gSessionParams->HostSigningAuthority = LOCKING_SP_ADMIN1_UID;
    gSessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
    if (EFI_ERROR(Status)) {
        gSessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    }
#endif
    TCG_STORAGE_ERR_CHECK(Status);
    
    
    // Table 181 C_PIN_TABLE Description 
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, C_PIN_ADMIN1_UID, \
                    3, ColumnName3, NewPassword, AsciiStrLen((CHAR8 *) NewPassword), TRUE, SIMPLE_TYPES);
    
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    if (EFI_ERROR(Status)) {
        goto CloseSession;
    }
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    return Status;
}
/** @internal
    Changes OPAL Admin Password. 
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to buffer with Admin password

    @retval EFI_STATUS
    
    @endinternal    
**/
EFI_STATUS
ChangeAdminPassword(
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               *AdminPassword
)
{
    EFI_STATUS          Status;
    UINT8              *OldPassword = NULL;
    UINT8              *NewPassword = NULL;
    CHAR8               ColumnName3[]="PIN";
    
    Status = SplitPasswordBuffer( AdminPassword, &OldPassword, &NewPassword );
    
    if (EFI_ERROR(Status)){
        return Status;
    }
    // Change AdminSP Admin Password
    gBS->SetMem(gBuffer, gBufferLength, 0);
    gBS->SetMem(gSessionParams, sizeof (SESSION_PARAMS), 0);
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = InitAdminSPCPinSid(OpalDevice, OldPassword, NewPassword);
        gBS->FreePool( OldPassword );
        gBS->FreePool( NewPassword );
        return Status;
    }
    
    // Open session to Admin SP as  the SID authority using old password
    gSessionParams->InvokingID = SM_UID;
    gSessionParams->MethodID = START_SESSION_UID;
    gSessionParams->HostSessionID = 1;
    gSessionParams->SPID = SP_ADMIN_UID;
    gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) OldPassword); //OpalDevice->LockingSPAdmin1Length;
    gSessionParams->HostChallenge = OldPassword; //OpalDevice->LockingSPAdmin1Pin;
    gSessionParams->HostSigningAuthority = SID_AUTH_UID;
    gSessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
    if (EFI_ERROR(Status)) {
        gSessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    }
#endif
    TCG_STORAGE_ERR_CHECK(Status);
    
    
    // Table 181 C_PIN_TABLE Description 
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, C_PIN_SID_UID, \
                    3, ColumnName3, NewPassword, AsciiStrLen((CHAR8 *) NewPassword), TRUE, SIMPLE_TYPES);
    
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    if (!EFI_ERROR(Status)) {
        // Change LockingSP Admin Password
        Status = ChangeLockingSPAdminPassword(OpalDevice, OldPassword, NewPassword);
    }
    
    gBS->FreePool( OldPassword );
    gBS->FreePool( NewPassword );
    
    return Status;
}

/** @internal
    Function retrieves Security state of the TCG enterprise device
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE

    @retval EFI_STATUS
    
    @endinternal    
**/
EFI_STATUS
EnterpriseLockedStatus (
    IN   OPAL_DEVICE         *OpalDevice
)
{
    
    EFI_STATUS      Status;
    CELL_BLOCK      CellBlock;
    UINT32          Length = 1;
    UINT8           ReadLockEnabled = 0;
    UINT8           ReadLocked = 0;
    
    if (OpalDevice->pCPinMsid == NULL) {
        OpalDevice->EnterpriseAdminPwdSts = UserEnabledStatusUnkown;
        return EFI_SUCCESS;
    }
    
    DEBUG ((DEBUG_VERBOSE, "***EnterpriseLockedStatus*** %x\n", OpalDevice->EnterpriseAdminPwdSts));
    
    // Authenticating multiple times leeds to exceeding the limit and locks out. Instead use a global value
    if (OpalDevice->EnterpriseAdminPwdSts != UserEnabledStatusUnkown) {
        return EFI_SUCCESS;
    }

    OpalDevice->EnterpriseAdminPwdSts = UserNotEnabled;
    
    gBS->SetMem(gSessionParams, sizeof (SESSION_PARAMS), 0);    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    DEBUG ((DEBUG_VERBOSE, "***Issue cmd EnterpriseLockedStatus***\n"));
    
    //Open Locking SP  Authentification
    Status = OpenSessionAndAuthenticate(OpalDevice, gSessionParams, gBuffer, gBufferLength, \
                    LOCKING_SP_UID_ENYTERPRISE, BANDMASTER0_UID, OpalDevice->pCPinMsid, OpalDevice->CPinMsidLength);
    if (EFI_ERROR(Status)){
        // Device Locked
        OpalDevice->EnterpriseAdminPwdSts = UserEnabled;
        return EFI_SUCCESS;    
    }
    
    gBS->SetMem(&CellBlock, sizeof (CellBlock), 0);
    CellBlock.TableUID = (UINTN)-1;
    CellBlock.RowisUid = TRUE; /// It will skip both start and end row
    CellBlock.StartRow = (UINTN)-1;
    CellBlock.EndRow = (UINT8)-1;
    CellBlock.StartColumn = 5; 
    CellBlock.EndColumn = 8;
    gBS->CopyMem (CellBlock.StartColumnString, "ReadLockEnabled", sizeof("ReadLockEnabled")); 
    gBS->CopyMem (CellBlock.EndColumnString, "WriteLocked", sizeof("WriteLocked"));
    
    Status = TableUidGet (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID, &CellBlock);
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    if (!EFI_ERROR(Status)){
        
        Status = GetColumnValue(OpalDevice, gBuffer, gBufferLength, &CellBlock, &Length, &ReadLockEnabled);

        DEBUG ((DEBUG_VERBOSE, "ReadLockEnabled : %x\n", ReadLockEnabled));
        if(!EFI_ERROR(Status)) {
            gBS->SetMem(CellBlock.StartColumnString, sizeof (CellBlock.StartColumnString), 0);
            gBS->CopyMem (CellBlock.StartColumnString, "ReadLocked", sizeof("ReadLocked"));
            CellBlock.StartColumn = 7;
            Status = GetColumnValue(OpalDevice, gBuffer, gBufferLength, &CellBlock, &Length, &ReadLocked);

            DEBUG ((DEBUG_VERBOSE, "ReadLocked : %x\n", ReadLocked));
        }
    }

    // This should get called only once. 
    if (!EFI_ERROR(Status) && ReadLockEnabled == 1) {
        OpalDevice->EnterpriseAdminPwdSts = UserEnabled;
    }
    else {
        OpalDevice->EnterpriseAdminPwdSts = UserNotEnabled;
    }
 CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    return Status;
    
}

/** @internal
    Initializes AdminSP CPinSid to take device ownership
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to buffer with Admin password

    @retval EFI_STATUS
    
    @endinternal   
**/

// If the storage device ownership is not taken then pCPinMsid will be valid and used to open a session and 
// set new password for C_PIN_SID_UID. From this point onwards, C_PIN_MSID is not valid and C_PIN_SID should be used
// for all future startsessions.

EFI_STATUS
InitAdminSPCPinSid (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *OldPassword,
   IN   UINT8               *AdminPassword
)
{
    EFI_STATUS          Status;
    CHAR8               ColumnName3[]="PIN";
    UINT32              OldPasswordLength;

    if (OpalDevice->pCPinMsid == NULL) {
        DEBUG((DEBUG_VERBOSE,"\n C_PIN_MSID is not available"));
        return EFI_DEVICE_ERROR;
    }
    gBS->SetMem(gSessionParams, sizeof (SESSION_PARAMS), 0);    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    if (!OldPassword) {
        OldPassword = OpalDevice->pCPinMsid;
        OldPasswordLength = OpalDevice->CPinMsidLength;
    } else {
        OldPasswordLength = (UINT32)AsciiStrLen((CHAR8 *)OldPassword);     
    }
    
    if( IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = OpenSessionAndAuthenticate(OpalDevice, gSessionParams, gBuffer, gBufferLength,\
                      SP_ADMIN_UID, SID_AUTH_UID, OldPassword, OldPasswordLength);
        TCG_STORAGE_ERR_CHECK(Status);
    } else {
        // Open session to Admin SP as  the SID authority using the MSID password
        gSessionParams->InvokingID = SM_UID;
        gSessionParams->MethodID = START_SESSION_UID;
        gSessionParams->HostSessionID = 1;
        gSessionParams->SPID = SP_ADMIN_UID;
        gSessionParams->HostChallengeLength = OpalDevice->CPinMsidLength;
        gSessionParams->HostChallenge = OpalDevice->pCPinMsid;
        gSessionParams->HostSigningAuthority = SID_AUTH_UID;
        gSessionParams->Write = 1;
        
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
        TCG_STORAGE_ERR_CHECK(Status);
    }
    
    //OpalDevice->pCPinSid = AdminPassword;
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, C_PIN_SID_UID, \
                    3, ColumnName3, AdminPassword, AsciiStrLen((CHAR8 *) AdminPassword), TRUE, SIMPLE_TYPES);
    
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    DEBUG((DEBUG_VERBOSE,"\n InitAdminSP CPIN : %r\n", Status));
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);

    if( IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
        TCG_STORAGE_ERR_CHECK(Status);
        
        gBS->SetMem(gSessionParams, sizeof (SESSION_PARAMS), 0);
        
        Status = OpenSessionAndAuthenticate(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_SP_UID_ENYTERPRISE, \
                    BANDMASTER0_UID, OldPassword, OldPasswordLength);
        TCG_STORAGE_ERR_CHECK(Status);
       
        gBS->SetMem(gBuffer, gBufferLength, 0);
        
        // Set New Pin for BandMaster0
        //OpalDevice->pCPinSid = AdminPassword; Table 181
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, BANDMASTER0_PIN_UID, \
                        3, ColumnName3, AdminPassword,  AsciiStrLen((CHAR8 *)AdminPassword), TRUE, SIMPLE_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);

        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        DEBUG ((EFI_D_ERROR, "BasterMaster0 PIN : %r\n", Status));

        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);

        //Authenticate EraseMaster with MSID PIN
        Status = SpObjectUidAuthenticate (OpalDevice, gSessionParams, gBuffer, gBufferLength, ERASEMASTER_UID, \
                        OldPassword, OldPasswordLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        // Set New Pin for ERASEMASTER
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, ERASEMASTER_PIN_UID, \
                        3, ColumnName3, AdminPassword, AsciiStrLen((CHAR8 *)AdminPassword), TRUE, SIMPLE_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);

        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        DEBUG ((EFI_D_ERROR, "EraseMaster PIN : %r\n", Status));
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    }
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    
    return Status;
    
}

/** @internal
    Once device ownership is taken this routine is used to activate
    LockingSP
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param PasswordBuffer - Pointer to buffer with Admin password

    @retval EFI_STATUS
    
    @endinternal
**/

// This procedure is called as part of Admin account setup.
// It copies  C_PIN_SID from ADMIN SP to C_PIN_ADMIN1 in the Locking SP

EFI_STATUS
ActivateLockingSP (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
)
{

    EFI_STATUS          Status;
    UINT8               LifeCycle = 0;
    UINT32              LifeCycleLength = 1;
    CELL_BLOCK          CellBlock;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
  
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        return EFI_SUCCESS;
    }
  
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    // Open session to Admin SP as  the SID authority using the MSID password
    gSessionParams->InvokingID = SM_UID;
    gSessionParams->MethodID = START_SESSION_UID;
    gSessionParams->HostSessionID = 1;
    gSessionParams->SPID = SP_ADMIN_UID;
    gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) AdminPassword); //OpalDevice->CPinSidLength;
    gSessionParams->HostChallenge = AdminPassword; //OpalDevice->pCPinSid;
    gSessionParams->HostSigningAuthority = SID_AUTH_UID;
    gSessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
    
    // Get LifeCycle state from SP table  (Table 215 Opal CORE Spec)
    gBS->SetMem(&CellBlock, sizeof(CellBlock), 0);
    CellBlock.TableUID = (UINTN)-1;
    CellBlock.RowisUid = TRUE; /// It will skip both start and end row
    CellBlock.StartRow = (UINTN)-1;
    CellBlock.EndRow = (UINT8)-1;
    CellBlock.StartColumn = 6; // LifeCycle column
    CellBlock.EndColumn = 6;
    
    Status = TableUidGet (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_SP_UID, &CellBlock);
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    if (!EFI_ERROR(Status)){
        Status = GetColumnValue(OpalDevice, gBuffer, gBufferLength, &CellBlock, &LifeCycleLength, &LifeCycle);
    }   
    DEBUG ((DEBUG_VERBOSE, "LifeCycle State : %x\n", LifeCycle));
    
    if (LifeCycle == LIFE_CYCLE_STATE_ISSUED_MANUF_INACTIVE) {
        Status = BuildBasicCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_SP_UID, ACTIVATE_UID);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, gBuffer, gBufferLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        DEBUG ((DEBUG_VERBOSE, "Locking SP Activate State Status : %r\n", Status)); 
        //If Success, PIN gets copied to LockSP Admin1
        //if (!EFI_ERROR(Status)){
        //    OpalDevice->LockingSPAdmin1Length = OpalDevice->CPinSidLength;
        //    OpalDevice->LockingSPAdmin1Pin = OpalDevice->pCPinSid;
        //}
    }

CloseSession:

    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    return Status;
    
}

/** @internal
    Once device LockingSP is activated. This routine is used to set and enable 
    locking range. GlobalRange would locks complete LBA range of the device.
        
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param PasswordBuffer - Pointer to buffer with Admin password

    @retval EFI_STATUS
    
    @endinternal 
**/

// Read/Write access to Admin and User1 is given for the entire storage device.
// ReadLockedEnabled and WriteLockedEnabled is set to TRUE which locks the drive.

EFI_STATUS
EnableGlobalRange (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *AdminPassword
)
{
    
    EFI_STATUS          Status;
    UINT8               ColumnValue;
    UINT64              Uid1 = UserMMMM + LShiftU64 (1,56);
    UINT64              Uid2 = UserMMMM + LShiftU64 (2,56);
    UINT8               BooleanOperator = 1;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID); 
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
    UINT8               ColumnValueArray[2];
    CHAR8               ColumnName9[] = "LockOnReset";
#endif
    CHAR8               ColumnName5[] = "ReadLockEnabled";
    CHAR8               ColumnName6[] = "WriteLockEnabled";
    CHAR8               ColumnName7[] = "ReadLocked";
    CHAR8               ColumnName8[] = "WriteLocked";

    DEBUG ((DEBUG_VERBOSE, "EnableGlobalRange %a\n", AdminPassword));
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {

        //Open Locking SP  Authentification
        Status = OpenSessionAndAuthenticate(OpalDevice, gSessionParams, gBuffer, gBufferLength, \
                        LOCKING_SP_UID_ENYTERPRISE, BANDMASTER0_UID, AdminPassword,  (UINT32)AsciiStrLen((CHAR8 *) AdminPassword));
        TCG_STORAGE_ERR_CHECK(Status);
    } else {
        // Open session to Admin SP as  the SID authority using the MSID password
        gSessionParams->InvokingID = SM_UID;
        gSessionParams->MethodID = START_SESSION_UID;
        gSessionParams->HostSessionID = 1;
        gSessionParams->SPID = LOCKING_SP_UID;
        gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) AdminPassword); //OpalDevice->LockingSPAdmin1Length;
        gSessionParams->HostChallenge = AdminPassword; //OpalDevice->LockingSPAdmin1Pin;
        gSessionParams->HostSigningAuthority = LOCKING_SP_ADMIN1_UID;
        gSessionParams->Write = 1;
    
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
        DEBUG ((DEBUG_VERBOSE, "Open Session with LockSP with Admin1 Credentials = %r\n", Status));
        TCG_STORAGE_ERR_CHECK(Status);
    }
    
    if (!IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = ObjectUidSetBooleanExpression (OpalDevice, gSessionParams, gBuffer, gBufferLength, ACE_Locking_GlobalRange_Set_RdLocked, \
            Uid1, Uid2, BooleanOperator);
    
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
        Status = ObjectUidSetBooleanExpression (OpalDevice, gSessionParams, gBuffer, gBufferLength, ACE_Locking_GlobalRange_Set_WrLocked, \
            Uid1, Uid2, BooleanOperator);
    
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    }
    
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        ColumnValue = 1;
        // ReadLockEnabled
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                          5, ColumnName5, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, TRUE, FALSE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        // WriteLockEnabled
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID, \
                          6, ColumnName6, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, FALSE, FALSE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        // ReadLocked
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                          7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, FALSE, FALSE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        // WriteLocked
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                          8, ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, FALSE, 
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
                         FALSE);
#else 
                         TRUE);
#endif						 
						  
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
        // LockOnReset
        ColumnValue = 1;
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                          9, ColumnName9, &ColumnValue, 1, TRUE, SET_VALUE_TYPES, FALSE, TRUE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
#endif
        Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, gBuffer, gBufferLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
       
        OpalDevice->EnterpriseAdminPwdSts = UserEnabled;

    } else {
    
        DEBUG ((DEBUG_VERBOSE, "\nSet ReadLockEnabled in Locking table\n"));
        // Set ReadLockedEnabled to TRUE in Locking Table (Table 226 CORE Spec)
        ColumnValue = 1;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                5,  ColumnName5, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        DEBUG ((DEBUG_VERBOSE, "\nSet WriteLockEnabled in Locking table\n"));
        // Set WriteLockedEnabled to TRUE in Locking Table
        ColumnValue = 1;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                6, ColumnName6, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);

        DEBUG ((DEBUG_VERBOSE, "\nEnable ReadLocked in Locking table\n"));
        // Set ReadLocked to TRUE in Locking Table (Table 226 CORE Spec)
        ColumnValue = 1;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        DEBUG ((DEBUG_VERBOSE, "\nEnable WriteLocked in Locking table\n"));
        // Set WriteLocked to TRUE in Locking Table
        ColumnValue = 1;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                8,ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
#if FORCE_TCG_OPAL_PASSWORD_PROMPT
        DEBUG ((DEBUG_VERBOSE, "\nProgram LockOnReset in Locking table\n"));
        ColumnValueArray[0] = 0;
        ColumnValueArray[1] = 3;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LOCKING_GLOBAL_RANGE_UID,\
                        9, ColumnName9, ColumnValueArray, 2, TRUE, SET_VALUE_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
#endif
    }
CloseSession: 
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    
    return Status;
}

/** @internal
    Sets OPAL User Password. 
    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param PasswordBuffer - Pointer to buffer with Admin and User password

    @retval EFI_STATUS
    
    @endinternal    
**/
EFI_STATUS
SetUserPassword (
   IN   OPAL_DEVICE         *OpalDevice,
   IN   UINT8               *PasswordBuffer
) 
{  
    EFI_STATUS          Status;
    UINT8               *OldPassword = NULL;
    UINT8               *NewPassword = NULL;
    BOOLEAN             IsUser1Active;
    
    // To create/change User password, Admin password is also required for authentication.
    // Input buffer contains both Admin and User password. So split it here.
    Status = SplitPasswordBuffer(PasswordBuffer, &OldPassword, &NewPassword);
    
    if (EFI_ERROR(Status)){
        return Status;
    }
    
    Status  = IsUserEnabled(1, OpalDevice, OldPassword, &IsUser1Active);
    
    if (EFI_ERROR(Status)){
        goto SetUserPasswordExit;
    }
    
    if (!IsUser1Active) { // If User1 does not exist, Create and activate User1
        Status = CreateandActivateUser(1, OpalDevice, OldPassword, NewPassword);
    } else {  // If User1 exist, update User password alone
        Status = ChangeUserPassword(1, OpalDevice, OldPassword, NewPassword);
    }
    DEBUG ((DEBUG_VERBOSE, "\n OPAL SetUserPassword =%r", Status));
    
SetUserPasswordExit:
    gBS->FreePool( OldPassword );
    gBS->FreePool( NewPassword );

    return Status;
}

/** @internal
    Splits Admin and User password from single buffer.
    
    @param PasswordBuffer  - Pointer to buffer with both Admin and User password.

    @retval Password1 - Splitted password 1 [ADMIN]
    @retval Password2 - Splitted password 1 [USER]
    
    @endinternal
**/

EFI_STATUS 
SplitPasswordBuffer(
    IN UINT8 *PasswordBuffer,
    IN UINT8 **Password1,
    IN UINT8 **Password2
)
{
    EFI_STATUS Status;
    UINT8      SplitSize = OPAL_PASSWORD_LENGTH+1;
    UINT8      DestIndex,Index;
    UINT8      *TempBuffer;

    Status = gBS->AllocatePool( EfiBootServicesData,
                                SplitSize,
                                (VOID**)Password1);
    if (EFI_ERROR(Status)){
        return Status;
    }

    gBS->SetMem(*Password1, SplitSize , 0 );

    Status = gBS->AllocatePool( EfiBootServicesData,
                                SplitSize,
                                (VOID**)Password2);
    if (EFI_ERROR(Status)){
        return Status;
    }
    
    gBS->SetMem(*Password2, SplitSize , 0 );
    
    TempBuffer = *Password1;
    
    for ( Index = 0; Index < OPAL_PASSWORD_LENGTH+1; Index++ ) {
        TempBuffer[Index] = PasswordBuffer[Index];
        if ( !PasswordBuffer[Index] ) {
            break;
        }
    }
    
    TempBuffer = *Password2;
    
    for ( DestIndex=0, Index = OPAL_PASSWORD_LENGTH+1 ; Index < SplitSize*2; Index++,DestIndex++ ) {
        TempBuffer[DestIndex] = PasswordBuffer[Index];
        if ( !PasswordBuffer[Index] ) {
            break;
        }
    }
    return EFI_SUCCESS;
    
}
/** @internal
    Function to create and activate User1

    @param User - User index to create. 
                  Current driver supports only User 1 creation.
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to Admin pwd buffer
    @param UserPassword  - Pointer to User pwd buffer   

    @retval EFI_SUCCESS - Success
    
    @endinternal
**/

// Open a session with Admin Password
// In Authority table, enable the user account
// Set the Password in C_PIN table for the user

EFI_STATUS
CreateandActivateUser (
   IN   UINT8              User,
   IN   OPAL_DEVICE        *OpalDevice,
   IN   UINT8              *AdminPassword,
   IN   UINT8              *UserPassword 
)
{
    
    EFI_STATUS          Status;
    UINT8               ColumnValue = 1;
    CHAR8               ColumnName5[] = "Enabled";
    CHAR8               ColumnName3[] = "PIN";
    
    /*
    // Check if USER1 is supported
    if (OpalDevice->OpalFeature.SPUserAuth < User){
        DEBUG ((DEBUG_VERBOSE, "User%x not supported\n", User));
        return EFI_NOT_FOUND;
    }
    */ 
    Status = StartLockingSPSession(OpalDevice, gSessionParams, AdminPassword, TRUE);
    DEBUG ((DEBUG_VERBOSE, "Open Session with LockSP with Admin1 Credentials = %r\n", Status));
    
    TCG_STORAGE_ERR_CHECK(Status);
    
    // Enable UserXXXX authority. Table 178 in Core Spec (Authority Table Description)
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, (UserMMMM | (LShiftU64 (User,56))), \
                    5, ColumnName5, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);  
    
    if (EFI_ERROR(Status)){
        DEBUG ((DEBUG_VERBOSE, "Authority Table Description enabling for User%x Failed\n", User));
        goto CloseSession;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams); 
    DEBUG ((DEBUG_VERBOSE, "Authority Table Enabled: %r\n", Status)); 
    if (EFI_ERROR(Status)) {
        goto CloseSession;
    }
    
    // Table 181 C_PIN_TABLE Description 
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, (C_PIN_USERMMMM | (LShiftU64 (User,56))), \
                    3, ColumnName3, UserPassword, AsciiStrLen((CHAR8 *) UserPassword), TRUE, SIMPLE_TYPES);
      
    if (EFI_ERROR(Status)){ 
        goto CloseSession;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    if (EFI_ERROR(Status)) {
        goto CloseSession;
    }
    OpalDevice->UserEnabledStatus = UserEnabled;
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    return Status;
    
}

/** @internal
    Changes OPAL User Password.
    
    @param User - User index to create. 
                  Current driver supports only User 1 creation.    
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to Admin pwd buffer
    @param UserPassword  - Pointer to User pwd buffer   

    @retval EFI_STATUS
    
    @endinternal    
**/
EFI_STATUS
ChangeUserPassword (
   IN   UINT8              User,        // One based
   IN   OPAL_DEVICE        *OpalDevice,
   IN   UINT8              *AdminPassword,
   IN   UINT8              *UserPassword 
)
{
    
    EFI_STATUS          Status;
    CHAR8               ColumnName3[] = "PIN";
    
    /*
    // Check if USER1 is supported
    if (OpalDevice->OpalFeature.SPUserAuth < User){
        DEBUG ((DEBUG_VERBOSE, "User%x not supported\n", User));
        return EFI_NOT_FOUND;
    }
    */
          
    Status = StartLockingSPSession(OpalDevice, gSessionParams, AdminPassword, TRUE);
    DEBUG ((DEBUG_VERBOSE, "Open Session with LockSP with Admin1 Credentials = %r\n", Status));
    TCG_STORAGE_ERR_CHECK(Status);
        
    // Table 181 C_PIN_TABLE Description 
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, (C_PIN_USERMMMM | (LShiftU64 (User,56))), \
                    3, ColumnName3, UserPassword, AsciiStrLen((CHAR8 *) UserPassword), TRUE, SIMPLE_TYPES);
      
    if (EFI_ERROR(Status)){ 
        goto CloseSession;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    if (EFI_ERROR(Status)) {
        goto CloseSession;
    }
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    return Status;
    
}

/** @internal
    Unlocks OPAL drive with either Admin or User password
     
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param PasswordBuffer - Admin or User password  
    @param AdminUser - denotes if input pwd is Admin or 
                       User password
                        1- Admin
                        0 - User
    
    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
UnlockOpalDrive (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               *PasswordBuffer,
    IN  BOOLEAN             AdminUser
)
{
    
    EFI_STATUS          Status;
    UINT8               ColumnValue;
    BOOLEAN             IsUserActive = FALSE;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID); 
    CHAR8               ColumnName7[] = "ReadLocked";
    CHAR8               ColumnName8[] = "WriteLocked";
    UINT64              LockingRangeUid = LOCKING_GLOBAL_RANGE_UID;
    UINT64              AuthObj1, AuthObj2;
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        AuthObj2 = AdminUser == 1 ? BANDMASTER0_UID : BANDMASTER1_UID;
        AuthObj1 = LOCKING_SP_UID_ENYTERPRISE;
        Status = OpenSessionAndAuthenticate(OpalDevice, gSessionParams, gBuffer, gBufferLength, AuthObj1, AuthObj2, \
                       PasswordBuffer, (UINT32)AsciiStrLen((CHAR8 *)PasswordBuffer));
        TCG_STORAGE_ERR_CHECK(Status);  
        LockingRangeUid = AdminUser == 1 ? LOCKING_GLOBAL_RANGE_UID : LOCKING_BAND1_UID_ENTERPRISE; 
    } else {
        // Open session to Admin SP as  the SID authority using the MSID password
        gSessionParams->InvokingID = SM_UID;
        gSessionParams->MethodID = START_SESSION_UID;
        gSessionParams->HostSessionID = 1;
        gSessionParams->SPID = LOCKING_SP_UID;
        gSessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) PasswordBuffer); //OpalDevice->LockingSPAdmin1Length;
        gSessionParams->HostChallenge = PasswordBuffer; //OpalDevice->LockingSPAdmin1Pin;
            gSessionParams->HostSigningAuthority = AdminUser ? LOCKING_SP_ADMIN1_UID : (UserMMMM + LShiftU64 (1,56));
        gSessionParams->Write = 1;
        
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);  
        DEBUG ((DEBUG_VERBOSE, "\n Open Session with LockSP with Admin1/User Credentials = %r\n", Status));
    }
  
    if (EFI_ERROR(Status)) {
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
        gSessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH;
        Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
        if (EFI_ERROR(Status)) {
#endif
            // Failed to unlock the device with Admin1/User Credentials, so update 
            // the status as Unknown
            OpalDevice->UserEnabledStatus = UserEnabledStatusUnkown;
            return Status;
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
        }
#endif
    }    
    // Set ReadLocked to FALSE in Locking Table (Table 226 CORE Spec)
    DEBUG ((DEBUG_VERBOSE, "\nDisable ReadLocked in Locking table\n"));
   
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        gBS->SetMem(gBuffer, gBufferLength, 0);
        ColumnValue = 0;
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LockingRangeUid,\
                         7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, TRUE, FALSE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        Status = BuildSetCmdBlock (OpalDevice, gSessionParams, gBuffer, gBufferLength, LockingRangeUid,\
                         8, ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES, FALSE, TRUE);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, gBuffer, gBufferLength);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
    } else {
        ColumnValue = 0;
        Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LockingRangeUid,\
                        7, ColumnName7, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        DEBUG ((DEBUG_VERBOSE, "\nDisable WriteLocked in Locking table\n"));
    
        // Set WriteLocked to FALSE in Locking Table
        ColumnValue = 0;
            Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, LockingRangeUid,\
                            8, ColumnName8, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
        TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
        
        gBS->SetMem(gBuffer, gBufferLength, 0);
        Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
        
    }
    
CloseSession:   
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
    
    if (!IS_ENTERPRISE_DEVICE(OpalDevice) && !EFI_ERROR(Status)) {
        if (AdminUser) {
            //If device is unlocked through AdminPassword, then update the UserEnabled status based on 
            //the Enabled Coloumn in Authority Table
            IsUserEnabled(1, OpalDevice, PasswordBuffer,&IsUserActive);
            if (IsUserActive) {
                OpalDevice->UserEnabledStatus = UserEnabled;
            } else {
                OpalDevice->UserEnabledStatus = UserNotEnabled;
            }
        } else {
            //Device unlocked successfully with user password
            OpalDevice->UserEnabledStatus = UserEnabled;
        }
    }       
    return Status;
    
} 

/** @internal
    Checks if User1 is already created or not
     
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param AdminPassword - Pointer to buffer with Admin password
    @param UserEnabled - denotes if User enabled or not
    
    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
IsUserEnabled (
    IN  UINT8              User,        
    IN  OPAL_DEVICE        *OpalDevice,
    IN  UINT8              *AdminPassword,
    IN  BOOLEAN            *UserEnabled
)
{
    EFI_STATUS          Status;
    CELL_BLOCK          CellBlock;
    UINT8               UserEnableValue;
    UINT32              UserEnabledLength = sizeof(BOOLEAN);    

    *UserEnabled = FALSE;
        
    Status = StartLockingSPSession(OpalDevice, gSessionParams, AdminPassword, TRUE);
    DEBUG ((DEBUG_VERBOSE, "Open Session with LockSP with Admin1 Credentials = %r \n", Status));    

    TCG_STORAGE_ERR_CHECK(Status);
    DEBUG ((DEBUG_VERBOSE, "\nGet User1 Authority \n")); 
    gBS->SetMem(&CellBlock, sizeof(CellBlock), 0);
    // Get User1 authority enabled value. Table 178 Core Spec Column 0x5
    CellBlock.TableUID = (UINTN)-1;
    CellBlock.RowisUid = TRUE; // It will skip both start and end row
    CellBlock.StartRow = (UINTN)-1;
    CellBlock.EndRow = (UINT8)-1;
    CellBlock.StartColumn = 0x5; // Authority Enabled
    CellBlock.EndColumn = 0x5;
    
    Status = TableUidGet (OpalDevice, gSessionParams, gBuffer, gBufferLength, UserMMMM|(LShiftU64 (User,56)), &CellBlock);
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);    

    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);
    
    if (!EFI_ERROR(Status)) {
        Status = GetColumnValue(OpalDevice, gBuffer, gBufferLength, &CellBlock, &UserEnabledLength, (UINT8 *)&UserEnableValue);
        if (!EFI_ERROR(Status)) {
            if (UserEnableValue)
                *UserEnabled = TRUE;
        }
    } else { 
        goto CloseSession;
    }       
    
CloseSession:
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength); 
    
    return Status;
}

/** @internal
    Wrapper function to invoke Security protocol command API

    @param  OpalDevice  - OPAL_DEVICE instance
    @param  SecurityProtocolId - Security protocol ID of the security protocol 
                                 command to be sent.
    @param  SPSpecificID - Security protocol specific portion of 
                           the security protocol command.
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.


    @retval EFI_STATUS
    
    @endinternal
**/

EFI_STATUS
ExecuteSendTcgCmd (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  UINT8               SecurityProtocolID,
    IN  UINT16              SPSpecificID,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength
)
{
    
    EFI_STATUS      Status;
    
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1) 
    PrintCmdBlock (Buffer);
#endif
        
    Status = OpalDevice->StorageSecurityProtocol->SendData( 
                                        OpalDevice->StorageSecurityProtocol,
                                        OpalDevice->MediaId,
                                        OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme ? MultU64x32(NVME_COMMAND_TIMEOUT, 10000000) : MultU64x32(SATA_COMMAND_TIMEOUT, 10000000),
                                        SecurityProtocolID,   
                                        SPSpecificID,           
                                        BufferLength,
                                        Buffer
                                        );
    
    return Status;
        
}

/** @internal
    Get values of selected table based on TCG Security specification
     
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.    
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.    
    @param  TableObjUid       - UID of the table    
    @param  CellBlock         - point to the row# and Column# from 
                                where the data needs to be read.
        
    @retval EFI_STATUS
    
    @endinternal    
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.3.3.6
// It is used to get the values from a selected table. Multiple column values can be read in a single call.
// CELL_BLOCK will point to the row# and Column# from where the data needs to be read.

EFI_STATUS
TableUidGet (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength,
    IN  UINT64              TableObjUid, 
    IN  CELL_BLOCK          *CellBlock        
)
{

    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID); 
    DEBUG ((DEBUG_VERBOSE, "\n TableUidGet  : %lx\n", Buffer));

    Status = BuildGetCmdBlock( OpalDevice, Buffer, BufferLength, SessionParams, TableObjUid, CellBlock);
    TCG_STORAGE_ERR_CHECK(Status);
    
    Status = ExecuteSendTcgCmd( OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);

    DEBUG ((DEBUG_VERBOSE, "\nTableUidGet  Status : %r\n", Status));   
    return Status;
    
}

/** @internal
    This routine can be used to check if the device ownership has been taken or not
     
    @param OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.      
    @param  TableObjUid       - UID of the table
    @param  CpinLength        - C_PIN data length
    @param  Cpin              - C_PIN value         
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer. 
        
    @retval EFI_STATUS
    
    @note
    GetCpin will fail if ownership is already taken and pCPinMsid will remain NULL.
    
    @endinternal
**/

// This routine can be used to check if the device ownership has been taken or not.
// GetCpin will fail if ownership is already taken and pCPinMsid will remain NULL.

EFI_STATUS
GetCpin (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  UINT64          TableObjUid,
    IN  UINT32          *CpinLength,
    IN  UINT8           **Cpin,
    IN  VOID            *Buffer,
    IN  UINTN           BufferLength
)
{
    EFI_STATUS      Status;
    CELL_BLOCK      CellBlock;
    UINT32          Index;
    UINT8           Temp,*CpinPtr;

    SessionParams->InvokingID = SM_UID;
    SessionParams->MethodID = START_SESSION_UID;
    SessionParams->HostSessionID = 1;
    SessionParams->SPID = SP_ADMIN_UID;
    SessionParams->Write = 1;
    
    gBS->SetMem(Buffer, BufferLength, 0);
    gBS->SetMem(&CellBlock, sizeof(CellBlock), 0);
   
    gBS->CopyMem (CellBlock.StartColumnString, "PIN", 3); // Used for Enterprise SSC
    gBS->CopyMem (CellBlock.EndColumnString, "PIN", 3);
    
    Status = SMUidStartSession (OpalDevice, SessionParams, Buffer, BufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
    
    gBS->SetMem(Buffer, BufferLength, 0);
    
    // Get MSID's Pin from C_PIN Table of Admin SP. Table 181 C_PIN of Core spec
    CellBlock.TableUID = (UINTN)-1;
    CellBlock.RowisUid = TRUE; /// It will skip both start and end row
    CellBlock.StartRow = (UINTN)-1;
    CellBlock.EndRow = (UINT8)-1;
    CellBlock.StartColumn = 3; // Points to PIN column
    CellBlock.EndColumn = 3;
    
    Status = TableUidGet (OpalDevice, SessionParams, Buffer, BufferLength, TableObjUid, &CellBlock);
    TCG_STORAGE_ERR_CHECK(Status);

    gBS->SetMem(Buffer, BufferLength, 0);
    Status = GetResponse (OpalDevice, Buffer, BufferLength, SessionParams);
    if (!EFI_ERROR(Status)){
        
       *CpinLength = 256;
        Status = gBS->AllocatePool(EfiBootServicesData, *CpinLength, (VOID**)Cpin);
        TCG_STORAGE_ERR_CHECK(Status);
        gBS->SetMem(*Cpin, *CpinLength, 0);
        Status = GetColumnValue(OpalDevice, Buffer, BufferLength, &CellBlock, CpinLength, *Cpin);

        if (!EFI_ERROR(Status)) {
            CpinPtr = *Cpin;
            for (Index = 0; Index < *CpinLength/2; Index++) {
                Temp = CpinPtr[Index];
                CpinPtr[Index] = CpinPtr[*CpinLength-1-Index];
                CpinPtr[*CpinLength-1-Index] = Temp;
            }
            
            DEBUG ((DEBUG_VERBOSE, "\n Cpin Value: "));
            
            for (Index = 0; Index < *CpinLength; Index++) {
                DEBUG ((DEBUG_VERBOSE, "%2X ", CpinPtr[Index]));
            }
        }
    }
    
    SMUidCloseSession (OpalDevice, SessionParams, Buffer, BufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
    
    return Status;
    
}


/** @internal
    This routine can be used to Get OPAL device Tper properties
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer. 
        
    @retval EFI_STATUS
    
    @note
    Currently from the returned data MaxCompacket size is used for MBR shadowing.
    
    @endinternal
**/

// TCG_Storage_Architechture Spec v2.01 Sec 5.2.2
// Currently from the returned data MaxCompacket size is used for MBR shadowing.

EFI_STATUS
GetTperProperties (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  VOID            *Buffer,
    IN  UINTN           BufferLength
)
{
    
    EFI_STATUS          Status;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    UINT8               String[] = "MaxComPacketSize";
    
    DEBUG ((DEBUG_VERBOSE, "GetTperProperties :"));
   
    Status = BuildBasicCmdBlock (OpalDevice, SessionParams, Buffer, BufferLength, SM_UID, PROPERTIES_UID);
    TCG_STORAGE_ERR_CHECK(Status);
    Status = ExecuteSendTcgCmd(OpalDevice, SECURITY_PROTOCOL_1, SPSpecificID, Buffer, BufferLength);
    TCG_STORAGE_ERR_CHECK(Status);
    gBS->SetMem(Buffer, BufferLength, 0);
    Status = GetResponse (OpalDevice, Buffer, BufferLength, SessionParams);
    DEBUG ((DEBUG_VERBOSE, "%r\n", Status));
   
    TCG_STORAGE_ERR_CHECK(Status);
    
    // Let default value be 1KB
    OpalDevice->MaxComPacketSize =  1024;
    Status = SearchProperties (Buffer, BufferLength, String, (VOID *)&(OpalDevice->MaxComPacketSize));    
    TCG_STORAGE_ERR_CHECK(Status);        
    
    //DEBUG ((DEBUG_VERBOSE, "MaxComPacketSize : %lx", OpalDevice->MaxComPacketSize));
    return Status;
    
}

/** @internal
    This routine starts TCG session for LockingSP
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  Password   - Admin or User password  
    @param  AdminUser  - denotes if input pwd is Admin or 
                         User password
                         1- Admin
                         0 - User
        
    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
StartLockingSPSession (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  SESSION_PARAMS  *SessionParams,
    IN  UINT8           *Password,
    IN  BOOLEAN         AdminUser
)
{
    EFI_STATUS          Status;
	
    gBS->SetMem(gBuffer, gBufferLength, 0);

    SessionParams->InvokingID = SM_UID;
    SessionParams->MethodID = START_SESSION_UID;
    SessionParams->HostSessionID = 1;
    SessionParams->SPID = LOCKING_SP_UID;
    SessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) Password); 
    SessionParams->HostChallenge = Password; 
    SessionParams->HostSigningAuthority = AdminUser ? LOCKING_SP_ADMIN1_UID : (UserMMMM + LShiftU64 (1,56));
    SessionParams->Write = 1;
   
    Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
    if(EFI_ERROR(Status)) {
        SessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH;
        Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    }
#endif
    return Status;
    
}

/** @internal
    Clears OPAL Admin password by reverting LockingSP
    
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE          
    @param  PwdBuffer   - Pointer to Admin password buffer

    @retval EFI_STATUS
    
    @note
    1. Unlock the drive 
    2. Revert LockingSp with KeepGlobalRangeKey set to TRUE.
    3. Revert AdminSp
    
    @endinternal
**/
EFI_STATUS
DisableOpalAdminPassword (
    IN  OPAL_DEVICE  *OpalDevice,
    UINT8            *PwdBuffer
)
{
    EFI_STATUS Status;
    
    //Unlock the OPAL drive
    Status = UnlockOpalDrive(OpalDevice, PwdBuffer, TRUE);
    ASSERT_EFI_ERROR(Status);
    
    // Revert Locking SP
    Status = RevertLockingSP( OpalDevice, gSessionParams, PwdBuffer ); 
    ASSERT_EFI_ERROR(Status);
    
    // Revert Admin SP
    Status = RevertAdminSP ( OpalDevice, gSessionParams, PwdBuffer );
    ASSERT_EFI_ERROR(Status);
    
    return Status;

}

/** @internal
    Clears OPAL User password
    
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE          
    @param  PwdBuffer   - Pointer to Admin password buffer

    @retval EFI_STATUS
    
    @endinternal
**/
EFI_STATUS
DisableOpalUserPassword (
    IN  OPAL_DEVICE     *OpalDevice,
    IN  UINT8           *PwdBuffer
)
{
    // Only One user is enabled for SED drive
    // Deactivate the User1
    return (DisableUserPwd( 1, OpalDevice, gSessionParams, PwdBuffer ));
}
/** @internal
    Reset the device to Original Factory State (OFS). 
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  PwdBuffer   - Pointer to Psid buffer
        
    @retval EFI_STATUS
    
    @endinternal   
**/
#if ENABLE_DEVICE_RESET_THRU_PSID
EFI_STATUS
RevertUsingPsid (
    OPAL_DEVICE         *OpalDevice,
    SESSION_PARAMS      *SessionParams,
    UINT8               *PsidBuffer
) {    
    EFI_STATUS       Status;
    UINTN            Size = 0;
    
    // Calculating the size of PsidBuffer
    while(PsidBuffer[Size] != '\0') {  
        Size++;
    }
    
    if (Size == 0) {
        return EFI_INVALID_PARAMETER;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    // Open session to Admin SP as  the SID authority using the MSID password
    SessionParams->InvokingID = SM_UID;
    SessionParams->MethodID = START_SESSION_UID;
    SessionParams->HostSessionID = 1;
    SessionParams->SPID = SP_ADMIN_UID;
    SessionParams->HostChallengeLength = Size;
    SessionParams->HostChallenge = PsidBuffer;
    SessionParams->HostSigningAuthority = PSID_AUTH_UID;
    SessionParams->Write = 1;
    DEBUG ((DEBUG_VERBOSE, "\nConverted PSID : %a\n", PsidBuffer));  

    Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    //PSID value is invalid
    if (EFI_ERROR(Status)) {
        return EFI_INVALID_PARAMETER;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = SpObjectUidRevert (OpalDevice, SessionParams, gBuffer, gBufferLength, SP_ADMIN_UID, REVERT_UID, FALSE);
    ASSERT_EFI_ERROR(Status);

    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, SessionParams);

    
    if (EFI_ERROR(Status)){
        SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    } else {
        // After revert, session gets closed automatically.
        gBS->SetMem(SessionParams, sizeof (SESSION_PARAMS), 0);
    }
    return Status;   
}
#endif
/** @internal
    Reverts Admin SP to Original Factory State (OFS). 
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  PwdBuffer   - Pointer to Admin password buffer
        
    @retval EFI_STATUS    
    @note
    C_PIN_SID will be copied with C_PIN_MSID after successful invocation of revert.
    It means, InitAdminSPCPinSid needs to be called to activate Admin user again.
    
    @endinternal  
**/
EFI_STATUS
RevertAdminSP (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  UINT8                *PwdBuffer
)
{
    EFI_STATUS Status;
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = OpenSessionAndAuthenticate(OpalDevice, SessionParams, gBuffer, gBufferLength, SP_ADMIN_UID,\
                        SID_AUTH_UID, PwdBuffer, (UINT32)AsciiStrLen((CHAR8 *) PwdBuffer));
        TCG_STORAGE_ERR_CHECK(Status);
    } else {
        // Open session to Admin SP as  the SID authority using the C_PIN Admin password
        SessionParams->InvokingID = SM_UID;
        SessionParams->MethodID = START_SESSION_UID;
        SessionParams->HostSessionID = 1;
        SessionParams->SPID = SP_ADMIN_UID;
        SessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) PwdBuffer);
        SessionParams->HostChallenge = PwdBuffer; 
        SessionParams->HostSigningAuthority = SID_AUTH_UID;
        SessionParams->Write = 1;
        
        Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
        if (EFI_ERROR(Status)) {
            SessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
            Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
        }
#endif
        TCG_STORAGE_ERR_CHECK(Status);
    }
    Status = SpObjectUidRevert (OpalDevice, SessionParams, gBuffer, gBufferLength, SP_ADMIN_UID, REVERT_UID, FALSE);
    
    if (EFI_ERROR(Status)) {
        //  We always receive ERROR status here even when revert operation succeeds
        //  So update Status to SUCCESS and return here now.
        gBS->SetMem(gBuffer, gBufferLength, 0);
        gBS->SetMem(SessionParams, sizeof (SESSION_PARAMS), 0);        
        return EFI_SUCCESS;
    }        
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, SessionParams);

    if (EFI_ERROR(Status)){
         SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    } else {
        // After revert session gets closed automatically.
        gBS->SetMem(SessionParams, sizeof (SESSION_PARAMS), 0);
    }  

    return Status;
}

/** @internal
    Reverts Locking SP 
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  PwdBuffer   - Pointer to Admin password buffer
        
    @retval EFI_STATUS    
    @note
    C_PIN_SID will be copied with C_PIN_MSID after successful invocation of revert.
    
    @endinternal
**/
EFI_STATUS
RevertLockingSP (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  SESSION_PARAMS      *SessionParams,
    IN  UINT8                *PwdBuffer
)
{
    EFI_STATUS Status;
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    if (IS_ENTERPRISE_DEVICE(OpalDevice)) {
        Status = OpenSessionAndAuthenticate(OpalDevice, SessionParams, gBuffer, gBufferLength,\
                    LOCKING_SP_UID_ENYTERPRISE, BANDMASTER0_UID, PwdBuffer, (UINT32)AsciiStrLen((CHAR8 *) PwdBuffer));
        TCG_STORAGE_ERR_CHECK(Status);  
    } else {
        // Open session to Admin SP as the SID authority using the C_PIN Admin password
        SessionParams->InvokingID = SM_UID;
        SessionParams->MethodID = START_SESSION_UID;
        SessionParams->HostSessionID = 1;
        SessionParams->SPID = LOCKING_SP_UID;
        SessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) PwdBuffer); 
        SessionParams->HostChallenge = PwdBuffer; 
        SessionParams->HostSigningAuthority = LOCKING_SP_ADMIN1_UID;
        SessionParams->Write = 1;
        
        Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
        if (EFI_ERROR(Status)) {
            SessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
            Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
        }
#endif
        TCG_STORAGE_ERR_CHECK(Status);
    }
    Status = SpObjectUidRevert (OpalDevice, SessionParams, gBuffer, gBufferLength, THISSP_UID, REVERTSP_UID, TRUE);    
    DEBUG ((DEBUG_VERBOSE,"LockingSP Revert: %r\n", Status));       
    TCG_STORAGE_ERR_CHECK(Status);
     
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, SessionParams);

    if (EFI_ERROR(Status)){
         SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    } else {
        // After revert session gets closed automatically.
        gBS->SetMem(SessionParams, sizeof (SESSION_PARAMS), 0);
    }  
    
    return Status;
}

/** @internal
    This routine resets Cpin SID with MSID value
     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  PwdBuffer   - Admin password  
        
    @retval EFI_STATUS
    
    @endinternal  
**/
EFI_STATUS
ChangeSIDtoMSID (
   IN  OPAL_DEVICE         *OpalDevice,
   IN  VOID                *PwdBuffer
)
{

    EFI_STATUS          Status;
    UINT8               *MsidBuffer = NULL;
    CHAR8               ColumnName3[]="PIN";
   
    if (OpalDevice->pCPinMsid == NULL) {   
        DEBUG ((DEBUG_VERBOSE, "C_PIN_MSID is not available\n"));      
        return EFI_DEVICE_ERROR;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    // Open session to Admin SP as  the SID authority using the Admin password
    gSessionParams->InvokingID = SM_UID;
    gSessionParams->MethodID = START_SESSION_UID;
    gSessionParams->HostSessionID = 1;
    gSessionParams->SPID = SP_ADMIN_UID;
    gSessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH;
    gSessionParams->HostChallenge = PwdBuffer;
    gSessionParams->HostSigningAuthority = SID_AUTH_UID;
    gSessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);

    TCG_STORAGE_ERR_CHECK(Status);
    
    Status = gBS->AllocatePool( EfiBootServicesData,
                                   OpalDevice->CPinMsidLength,
                                   (VOID**)&MsidBuffer );
    ASSERT_EFI_ERROR( Status );
    
    // Set MSID value as C_PIN_SID
    gBS->CopyMem(MsidBuffer, OpalDevice->pCPinMsid, OpalDevice->CPinMsidLength);
    
    Status = ObjectUidSet(OpalDevice, gSessionParams, gBuffer, gBufferLength, C_PIN_SID_UID, \
                3, ColumnName3, MsidBuffer, OpalDevice->CPinMsidLength, TRUE, SIMPLE_TYPES);
        
    TCG_STORAGE_ERR_CHECK_CLOSE_SESSION(Status);
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, gSessionParams);        
    DEBUG ((DEBUG_VERBOSE,"AdminSP CPIN set to MSID: %r\n", Status));  
    
CloseSession:
    // Clear MSID password buffer
    gBS->SetMem(MsidBuffer, OpalDevice->CPinMsidLength, 0);
    gBS->FreePool(MsidBuffer);
    SMUidCloseSession (OpalDevice, gSessionParams, gBuffer, gBufferLength);
        
    return Status;
    
}

/** @internal
    Clears User password

    @param User - User index to create. 
                  Current driver supports only User 1 creation.     
    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  SessionParams     - information on current session.            
    @param  PwdBuffer   - Pointer to Admin password buffer
        
    @retval EFI_STATUS
        
    @endinternal  
**/
// In Table 178 Authority Table description set Column 5 to zero to de-activate the user account.
EFI_STATUS
DisableUserPwd (
   IN   UINT8               User,
   IN   OPAL_DEVICE         *OpalDevice,
   IN   SESSION_PARAMS      *SessionParams,
   IN   UINT8                *PwdBuffer
)
{
    EFI_STATUS         Status;
    UINT8              ColumnValue = 0;
    CHAR8              ColumnName5[] = "Enabled";
    
    // Open session to Admin SP as  the SID authority
    SessionParams->InvokingID = SM_UID;
    SessionParams->MethodID = START_SESSION_UID;
    SessionParams->HostSessionID = 1;
    SessionParams->SPID = LOCKING_SP_UID;
    SessionParams->HostChallengeLength = AsciiStrLen((CHAR8 *) PwdBuffer); 
    SessionParams->HostChallenge = PwdBuffer; 
    SessionParams->HostSigningAuthority = LOCKING_SP_ADMIN1_UID;
    SessionParams->Write = 1;
    
    Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
#if ATTEMPT_PADDED_PASSWORD_FOR_UNLOCK
    if (EFI_ERROR(Status)) {
        SessionParams->HostChallengeLength = OPAL_PASSWORD_LENGTH; //OpalDevice->LockingSPAdmin1Length;
        Status = SMUidStartSession (OpalDevice, SessionParams, gBuffer, gBufferLength);
    }
#endif
    TCG_STORAGE_ERR_CHECK(Status);
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    
    // Disable UserXXXX authority. Table 178 in Core Spec (Authority Table Description)
    Status = ObjectUidSet(OpalDevice, SessionParams, gBuffer, gBufferLength, (UserMMMM | (LShiftU64 (User,56))), \
                    5, ColumnName5, &ColumnValue, 1, TRUE, ENUMERATION_TYPES);
    
    if (EFI_ERROR(Status)){
        DEBUG ((DEBUG_VERBOSE, "Authority Table Description Disable for User%x Failed\n", User));
        goto CloseSession;
    }
    
    gBS->SetMem(gBuffer, gBufferLength, 0);
    Status = GetResponse (OpalDevice, gBuffer, gBufferLength, SessionParams);
    DEBUG ((DEBUG_VERBOSE, "\nAuthority Table Description Disable for User%x ends\n", User));    
    OpalDevice->UserEnabledStatus = UserNotEnabled;
    CloseSession:
        SMUidCloseSession (OpalDevice, SessionParams, gBuffer, gBufferLength);

    return Status; 
}

/** @internal
    Function to Get response data of sent security command sent.

    @param  OpalDevice  - Pointer to OpalConfig OPAL_DEVICE
    @param  Buffer            - Payload buffer
    @param  BufferLength      - Size in bytes of the payload data buffer.    
    @param  SessionParams     - information on current session.
    
    @retval EFI_STATUS
    
    @endinternal
**/
// TCG_Storage_Architechture Spec v2.01 Sec 5.1.5 has the status codes

EFI_STATUS
GetResponse (
    IN  OPAL_DEVICE         *OpalDevice,
    IN  VOID                *Buffer,
    IN  UINTN               BufferLength,
    IN  SESSION_PARAMS      *SessionParams
)
{
    
    EFI_STATUS          Status;
    UINT8               bData = 0xFF;
    UINT16              SPSpecificID = ToBigLittleEndianWord(OpalDevice->OpalFeature.BaseComID);
    COM_PACKET_FORMAT   *ComPacket = Buffer;
    UINT32              Retries = NUM_OF_RETRIES;
    DEBUG ((DEBUG_VERBOSE, "\nGetResponse Buffer : %lx\n", Buffer));
    do {
        Status = OpalDevice->StorageSecurityProtocol->ReceiveData( 
                                OpalDevice->StorageSecurityProtocol,
                                OpalDevice->MediaId,
                                OpalDevice->BusInterfaceType == AmiStorageInterfaceNvme ? MultU64x32(NVME_COMMAND_TIMEOUT, 10000000) : MultU64x32(SATA_COMMAND_TIMEOUT, 10000000),
                                SECURITY_PROTOCOL_1,   // Security Protocol ID
                                SPSpecificID,   
                                0x200,
                                Buffer,
                                &BufferLength
                                );
        DEBUG ((DEBUG_VERBOSE, "GetResponse Buffer : %r\n", Status)); 
#if defined(TCG_STORAGE_SEC_VERBOSE_PRINT) && (TCG_STORAGE_SEC_VERBOSE_PRINT == 1)     
        PrintCmdBlock (Buffer);
#endif
        
        if (EFI_ERROR(Status)) {
            return Status;
        }
        
        // Check if it is a CloseSession
        Status = CheckCloseSession (Buffer, BufferLength);
        if (Status == EFI_SUCCESS) {
            SessionParams->Tsn = 0;     // Indicate Session is closed
            return EFI_DEVICE_ERROR;
        }

        // Sec 3.2.3.2.1.4 Core spec
        if ((ComPacket->OutStandingData == 0) && ComPacket->Length) {
            break;
        } else {
            MicroSecondDelay (100000); // 100msec
            continue;
        }
        
    } while (--Retries);
    
    Status = GetStatusValue (Buffer, BufferLength, &bData);
    
    if (EFI_ERROR(Status)) {
        switch (bData) {
        case NOT_AUTHORIZED:
            Status = EFI_ACCESS_DENIED;
            break;
        default:
            break;          
        }
    } 
    DEBUG ((DEBUG_VERBOSE, "GetResponse Status : %r\n", Status));
    TCG_STORAGE_ERR_CHECK(Status);
    
    return Status;
    
}
/** @internal
    Function to allocate pool of a particular type.

    @param  PoolType               Type of pool to allocate
    @param  Size                   The amount of pool to allocate
    @param  Buffer                 Pointer to allocated buffer
  
    @retval EFI_STATUS
    
    @endinternal

**/
EFI_STATUS
TcgSecAllocatePool(
  IN  EFI_MEMORY_TYPE              PoolType,
  IN  UINTN                        Size,
  OUT VOID                         **Buffer
  )
{
    return gBS->AllocatePool(PoolType, Size, Buffer);
}

/** @internal
    Function to free allocated pool

    @param  Buffer            Buffer to free
  
    @return EFI_STATUS
  
    @endinternal
**/
EFI_STATUS
TcgSecFreePool(
  IN  VOID                         *Buffer
  )
{
    return gBS->FreePool (Buffer);
}

#if SmmTcgStorageSec_SUPPORT && !FORCE_TCG_OPAL_PASSWORD_PROMPT

/** @internal
    Converts Integer TcgDevice's Nvram DataId to String.

    @param DataId 
    @param String 
         
    @retval VOID
    
    @endinternal
**/
VOID
TcgConvertTcgDataIdToString (
    IN  UINT32  DataId,
    IN  UINTN   BufferSize,
    OUT CHAR16  *String
)
{
    UnicodeSPrint(String, BufferSize, L"%d",DataId);
    return;
}

/** @internal
    Encodes/Decodes input string

    @param InputString 
    @param OutputString 
    @param StringLen 

    @retval VOID
    
    @endinternal
**/
VOID
TcgEncodeDecodePassword (
    IN  UINT8   *InputString,
    OUT UINT8   *OutputString,
    IN  UINT32  StringLength
)
{
    UINT8  *Key;
    UINT32 StrIndex;
    UINT32 KeyIndex;
    UINT32 KeyLength;
    EFI_STATUS   Status;

    if ((InputString == NULL) || (StringLength == 0)) {
        return;
    }

    KeyLength = (UINT32)( StrLen((CHAR16*) TCG_PWD_ENCRYPTION_KEY ));
    Status = gBS->AllocatePool( EfiBootServicesData, KeyLength, (VOID**)&Key );
    ASSERT_EFI_ERROR(Status);
    gBS->SetMem( Key, KeyLength, 0 );
    gBS->CopyMem( Key, TCG_PWD_ENCRYPTION_KEY, KeyLength );
    ASSERT( Key != NULL );

    for ( StrIndex = 0, KeyIndex = 0; StrIndex < StringLength; StrIndex++, KeyIndex++ )
    {
        if ( KeyIndex == (KeyLength - 1))
        {
            KeyIndex = 0;
        }
        OutputString[StrIndex] = (UINT8) ( InputString[StrIndex] ^ Key[KeyIndex] );
    }

    gBS->FreePool( Key );
    return;
}

/** @internal
    Transfers Stored Data from NVRAM to SMRAM.

    @param None

    @retval EFI_SUCCESS      : Password saved in SMRAM.
    @retval EFI_DEVICE_ERROR : Password not saved in SMRAM.
    
    @endinternal
**/
EFI_STATUS
TcgTransferPwDataFromNvramToSmram (
)
{
    UINT32             TcgDeviceNvramId = 0;
    CHAR16             *TcgSecutiyData = NULL;
    TCGSEC_NVRAM_DATA  *TcgSecData;
    EFI_STATUS         Status;
    UINT16             Control;
    UINTN              Index, PwdIndex;
    UINTN              TcgSecDataSize = sizeof (TCGSEC_NVRAM_DATA);
    UINT8              CryptBuffer[OPAL_PASSWORD_LENGTH];
    UINT8              Buffer[OPAL_PASSWORD_LENGTH];
    UINT16             TcgSecurityStatus = 0;
    EFI_HANDLE         *HandleBuffer;
    UINTN              Count;
    AMI_HDD_SECURITY_PROTOCOL *TcgStorageSecurityProtocol = NULL;
    
    Status = gBS->LocateHandleBuffer(
                                ByProtocol,
                                &gAmiTcgStorageSecurityProtocolGuid,
                                NULL,
                                &Count,
                                &HandleBuffer
                                );

    if ( EFI_ERROR( Status )) {
        return Status;
    }
    
    Status = gBS->AllocatePool( EfiBootServicesData,
                                0x20,
                                (VOID**)&TcgSecutiyData );
    ASSERT_EFI_ERROR( Status );

    gBS->AllocatePool( EfiBootServicesData,
                       sizeof (TCGSEC_NVRAM_DATA),
                       (VOID**)&TcgSecData );
    ASSERT_EFI_ERROR( Status );

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

        Status = TcgStorageSecurityProtocol->ReturnSecurityStatus ( TcgStorageSecurityProtocol, &TcgSecurityStatus);
        if ( EFI_ERROR( Status )) {
            continue;
        }

        // Tcg Password Nvram Variable will not be available if the HDD is in locked state. 
        // So continue without copying the password to Smram as the password 
        // data is not available
        if ( (TcgSecurityStatus & SecurityLockedMask) || !(TcgSecurityStatus & SecurityEnabledMask)) {
            continue;
        }

        Status = GatherTcgDeviceNvramInfo( TcgStorageSecurityProtocol,
                                           &TcgDeviceNvramId );
        ASSERT_EFI_ERROR( Status );

        gBS->SetMem( TcgSecutiyData, 0x20, 0 );
        TcgConvertTcgDataIdToString( TcgDeviceNvramId,0x20,TcgSecutiyData );

        gBS->SetMem( TcgSecData, sizeof(TCGSEC_NVRAM_DATA),  0 );
        gBS->SetMem( Buffer, OPAL_PASSWORD_LENGTH, 0 );

        Status = gRT->GetVariable(
                         TcgSecutiyData,
                         &gTcgStorageSecurityPwNvguid,
                         NULL,
                         &TcgSecDataSize,
                         TcgSecData
                         );

        ASSERT_EFI_ERROR( Status );

        Control = TcgSecData->UserMaster;

        if ( Control & 0x01 ) {
            for ( PwdIndex = 0; PwdIndex < sizeof (TcgSecData->TcgMasterPassword); PwdIndex++ ) {
                CryptBuffer[PwdIndex] = TcgSecData->TcgMasterPassword[PwdIndex];
            }

            TcgEncodeDecodePassword( CryptBuffer, Buffer, TcgSecData->PasswordLength );
        } else {
            for ( PwdIndex = 0; PwdIndex < sizeof (TcgSecData->TcgUserPassword); PwdIndex++ ) {
                CryptBuffer[PwdIndex] = TcgSecData->TcgUserPassword[PwdIndex];
            }

            TcgEncodeDecodePassword( CryptBuffer, Buffer, TcgSecData->PasswordLength );
        }

        SmmTcgPasswordInterface( TcgStorageSecurityProtocol,
                                 Control,
                                 Buffer,
                                 SavePassword );
    }

    gBS->FreePool( HandleBuffer );
    gBS->FreePool( TcgSecData );
    gBS->FreePool( TcgSecutiyData );

    return Status;
}

/** @internal
    Return NvramId based on Tcg device's Bus, Device, Function number

    @param TcgStorageSecurityProtocol 
    @param TcgDeviceNvramId

    @retval EFI_SUCCESS : return NvramId successfully
    @retval EFI_ERROR   : Failed while attaining TCG devices info
    
    @endinternal
**/
EFI_STATUS
GatherTcgDeviceNvramInfo (
    AMI_HDD_SECURITY_PROTOCOL   *TcgStorageSecurityProtocol,
    OUT UINT32                  *TcgDeviceNvramId
)
{
    EFI_STATUS            Status;
    UINTN                 SegNum;
    UINTN                 BusNum;
    UINTN                 DevNum;
    UINTN                 FuncNum;
    UINT16                wBDF;
    UINT16                wPortPMModeFlag=0;
    UINT8                 ChannelorPort=0;
    UINT8                 DeviceorPMPort=0;
    EFI_PCI_IO_PROTOCOL   *PciIo;
    BOOLEAN               RstControlled = FALSE;
    VOID                  *BusInterface \
         = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )TcgStorageSecurityProtocol)->BusInterface;
    AMI_STORAGE_BUS_INTERFACE   BusInterfaceType \
         = ((AMI_TCG_STORAGE_SECURITY_INTERFACE* )TcgStorageSecurityProtocol)->BusInterfaceType;
#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    SATA_DEVICE_INTERFACE *SataDevInterface = (SATA_DEVICE_INTERFACE*)BusInterface;
#endif

#if ( defined(AHCI_SUPPORT) && (AHCI_SUPPORT != 0) )
    if ( BusInterfaceType == AmiStorageInterfaceAhci ) {
        PciIo          = SataDevInterface->AhciBusInterface->PciIO;
        ChannelorPort  = SataDevInterface->PortNumber;
        DeviceorPMPort =  SataDevInterface->PMPortNumber;
    }
#endif

    if ( BusInterfaceType == AmiStorageInterfaceNvme ) {
#if ( defined(NvmeRstPassword_SUPPORT) && (NvmeRstPassword_SUPPORT != 0) ) 
        RstControlled = IsRstDevice(((NVME_RST_INTERFACE*)BusInterface)->Signature);
        if(RstControlled) {
            PciIo = ((NVME_RST_INTERFACE*)BusInterface)->PciIO;
        }
#endif
#if ( defined(NVMe_SUPPORT) && (NVMe_SUPPORT != 0) )
        if(!RstControlled) { 
            PciIo = ((AMI_NVME_CONTROLLER_PROTOCOL*)BusInterface)->PciIO;
        }
#endif
    }
    Status = PciIo->GetLocation( PciIo, &SegNum, &BusNum, &DevNum, &FuncNum );

    if(EFI_ERROR (Status) ) {
        return Status;
    }

    wBDF = (UINT16)((BusNum << 8) | (DevNum << 3) | FuncNum );

    // wPortPMModeFlag=0 for Nvme
    if ( BusInterfaceType <= AmiStorageInterfaceAhci ) {
        wPortPMModeFlag = (UINT16)((ChannelorPort << 12)   \
                                   | (DeviceorPMPort << 4)   \
                                   | ((UINT8)BusInterfaceType));
    }

    *TcgDeviceNvramId = (UINT32)(((UINT16)wBDF << 16) + ((UINT16)wPortPMModeFlag));

    DEBUG ((DEBUG_VERBOSE, "TcgDeviceNvramId = %x \n", *TcgDeviceNvramId));

    return EFI_SUCCESS;
}

/** @internal
    Gets TcgStorage device security data stored in NVRAM Area.
  
    @param *TcgStorageSecurityProtocol
    @param *Buffer
  
    @retval EFI_STATUS
   
    @endinternal
**/
EFI_STATUS 
GetTcgSecDataFromNvram(
    AMI_HDD_SECURITY_PROTOCOL   *TcgStorageSecurityProtocol,
    UINT8                       *Buffer 
)
{

    UINT32             TcgDeviceNvramId = 0;
    CHAR16             *TcgSecutiyData = NULL;
    TCGSEC_NVRAM_DATA  *TcgSecData;
    EFI_STATUS         Status;
    UINTN              PwdIndex;
    UINTN              TcgSecDataSize = sizeof (TCGSEC_NVRAM_DATA);
    UINT8              CryptBuffer[OPAL_PASSWORD_LENGTH];
    UINT16             Control;

    Status = GatherTcgDeviceNvramInfo( TcgStorageSecurityProtocol,
                                       &TcgDeviceNvramId );
    ASSERT_EFI_ERROR( Status );

    Status = gBS->AllocatePool( EfiBootServicesData,
                       0x20,
                       &TcgSecutiyData );
   
    ASSERT_EFI_ERROR( Status );

    gBS->SetMem( TcgSecutiyData, 0x20, 0 );
    TcgConvertTcgDataIdToString( TcgDeviceNvramId,0x20,TcgSecutiyData );

    Status = gBS->AllocatePool( EfiBootServicesData, sizeof (TCGSEC_NVRAM_DATA), &TcgSecData );
    ASSERT_EFI_ERROR( Status );
    gBS->SetMem( TcgSecData, sizeof(TCGSEC_NVRAM_DATA),  0 );

    gBS->SetMem( Buffer,  OPAL_PASSWORD_LENGTH, 0 );

    Status = gRT->GetVariable ( TcgSecutiyData,
                                &gTcgStorageSecurityPwNvguid,
                                NULL,
                                &TcgSecDataSize,
                                TcgSecData
                                );

    ASSERT_EFI_ERROR( Status );
    if(EFI_ERROR(Status)) {
        gBS->FreePool( TcgSecData );
        gBS->FreePool( TcgSecutiyData );
        return Status;
    }

    Control = TcgSecData->UserMaster;

    if ( Control & 0x01 ) {
        for ( PwdIndex = 0; PwdIndex < sizeof (TcgSecData->TcgMasterPassword); PwdIndex++ ) {
            CryptBuffer[PwdIndex] = TcgSecData->TcgMasterPassword[PwdIndex];
        }

        TcgEncodeDecodePassword( CryptBuffer, Buffer, TcgSecData->PasswordLength );
    } else {
        for ( PwdIndex = 0; PwdIndex < sizeof (TcgSecData->TcgUserPassword); PwdIndex++ ) {
            CryptBuffer[PwdIndex] = TcgSecData->TcgUserPassword[PwdIndex];
        }

        TcgEncodeDecodePassword( CryptBuffer, Buffer, TcgSecData->PasswordLength );
    }

    gBS->FreePool( TcgSecData );
    gBS->FreePool( TcgSecutiyData );

    return Status;
}

/** @internal
    Sets TcgStorage device security data in NVRAM Area.

    @param TcgDeviceNvramId
    @param Control
    @param Buffer

    @retval EFI_SUCCESS : TcgPassword saved in NVram which later will be transfered to
        SMRAM.
    @retval EFI_ERROR   : Failed to save the HDD password in Nvram.
    
    @endinternal
**/
EFI_STATUS
SetTcgDeviceDataInNvram (
    UINT32      *TcgDeviceNvramId,
    UINT16      Control,
    UINT8       *Buffer
)
{
    EFI_STATUS         Status;
    TCGSEC_NVRAM_DATA  *TcgSecData;
    UINTN              Index;
    CHAR16             *TcgSecutiyData = NULL;
    UINT8              CryptBuffer[OPAL_PASSWORD_LENGTH];
    UINTN              TcgSecDataSize = sizeof (TCGSEC_NVRAM_DATA);
    UINT32             ParseIndex;
    UINT32             Attribute = 0;

    Status = gBS->AllocatePool( EfiBootServicesData,
                       sizeof (TCGSEC_NVRAM_DATA),
                       (VOID**)&TcgSecData );
    
    ASSERT_EFI_ERROR( Status );
    
    gBS->SetMem( TcgSecData, sizeof(TCGSEC_NVRAM_DATA), 0);
    
    Status = gBS->AllocatePool( EfiBootServicesData,
                                0x20,
                                (VOID**)&TcgSecutiyData );
    ASSERT_EFI_ERROR(Status);
    
    gBS->SetMem( TcgSecutiyData, 0x20, 0 );
    TcgConvertTcgDataIdToString( *TcgDeviceNvramId,0x20,TcgSecutiyData);

    //
    // Get the password variable so that other password will be
    // maintained in the nvram variable.
    //
    Status = gRT->GetVariable(
                     TcgSecutiyData,
                     &gTcgStorageSecurityPwNvguid,
                     &Attribute,
                     &TcgSecDataSize,
                     TcgSecData
                     );

    if ( EFI_ERROR( Status )) {
        gBS->SetMem( TcgSecData, sizeof(TCGSEC_NVRAM_DATA), 0 );
        if(Attribute == 0) {
            Attribute = (UINT32)(EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS);
        }
    }

    for ( ParseIndex = 0; ParseIndex < OPAL_PASSWORD_LENGTH; ParseIndex++ )
    {
        if ( Buffer[ParseIndex] == 0 )
        {
            break;
        }
    }

    TcgSecData->UserMaster     = Control;
    TcgSecData->PasswordLength = ParseIndex;

    if ( Control & 0x01 ) {
        TcgEncodeDecodePassword ( Buffer, CryptBuffer, ParseIndex );

        for ( Index = 0; Index < ParseIndex; Index++ ) {
            TcgSecData->TcgMasterPassword[Index] = CryptBuffer[Index];
        }

        for ( Index = ParseIndex; Index < sizeof (TcgSecData->TcgMasterPassword); Index++ ) {
            TcgSecData->TcgMasterPassword[Index] = 00;
        }
    } else {
        TcgEncodeDecodePassword( Buffer, CryptBuffer, ParseIndex );

        for ( Index = 0; Index < ParseIndex; Index++ ) {
            TcgSecData->TcgUserPassword[Index] = CryptBuffer[Index];
        }

        for ( Index = ParseIndex; Index < sizeof (TcgSecData->TcgUserPassword); Index++ ) {
            TcgSecData->TcgUserPassword[Index] = 00;
        }
    }

    Status = gRT->SetVariable(
                     TcgSecutiyData,
                     &gTcgStorageSecurityPwNvguid,
                     Attribute,
                     TcgSecDataSize,
                     TcgSecData
                     );

    ASSERT_EFI_ERROR( Status );
    gBS->FreePool( TcgSecData );
    gBS->FreePool( TcgSecutiyData );

    return Status;
}
#endif
