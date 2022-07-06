//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

/** @file IdeBus.c
    Provides IDE Block IO protocol

**/

//---------------------------------------------------------------------------

#include "IdeBus.h"

//---------------------------------------------------------------------------

#if ( defined(BOOT_SECTOR_WRITE_PROTECT) && (BOOT_SECTOR_WRITE_PROTECT != 0) )
#include <Protocol/AmiBlockIoWriteProtection.h>
AMI_BLOCKIO_WRITE_PROTECTION_PROTOCOL   *AmiBlkWriteProtection = NULL;
#endif

#if defined(ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT) && (ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT != 0)
#include <Protocol/AcousticProtocol.h>
EFI_GUID gHddAcousticInitProtocolGuid = AMI_HDD_ACOUSTIC_INIT_PROTOCOL_GUID;
AMI_HDD_ACOUSTIC_INIT_PROTOCOL *gHddAcousticInitProtocol = NULL;
#endif

static EFI_GUID    gIdeSetupProtocolguid           = IDE_SETUP_PROTOCOL_GUID;


EFI_EVENT                               gIDEBusEvtBootScript      = NULL;
EFI_EVENT                               gIDEBusEvtMiscSmmFeatures = NULL;
AMI_SCSI_PASS_THRU_INIT_PROTOCOL        *gScsiPassThruInitProtocol; 
AMI_ATA_PASS_THRU_INIT_PROTOCOL         *AtaPassThruInitProtocol;  
AMI_HDD_OPAL_SEC_INIT_PROTOCOL          *OpalSecInitProtocol = NULL;
AMI_HDD_SECURITY_INIT_PROTOCOL          *HddSecurityInitProtocol = NULL;
AMI_HDD_SMART_INIT_PROTOCOL             *HddSmartInitProtocol = NULL;
AMI_IDE_SETUP_PROTOCOL                  *gIdeSetupProtocol;
PLATFORM_IDE_PROTOCOL                   *gPlatformIdeProtocol;

extern EFI_COMPONENT_NAME_PROTOCOL gIdeBusControllerDriverName;

//
// Instantiate PLATFORM_IDE_PROTOCOL with default SDL Token values 
//

PLATFORM_IDE_PROTOCOL        gPlatformIdeProtocolInstance = {
    3,                                       //Protocol Revision
    1,                                       //MasterSlaveEnumeration
    1,                                       //IdeBusMasterSupport
    1,                                       //AtapiBusMasterSupport
    1,                                       //AcousticManagementSupport
    0,                                       //IdePowerManagementSupport
    5,                                       //StandbyTimeout
    0,                                       //AdvPowerManagementSupport
    1,                                       //AdvPowerManagementLevel
    0,                                       //PowerupInStandbySupport
    0,                                       //PowerupInStandbyMode
    0,                                       //IdePwrManagementInterfaceSupport
    0,                                       //HostProtectedAreaSupport
    0,                                       //IdeHPSupport
    1,                                       //EfiIdeProtocol
    0,                                       //AhciCompatibleMode
    0,                                       //SBIdeSupport
    1,                                       //HddPowerLossInS3
    0,                                       //DiPMSupport
    0,                                       //DisableSoftSetPrev
    0,                                       //ForceHddPasswordPrompt
    EFI_IDE_CONTROLLER_INIT_PROTOCOL_GUID,   //gIdeControllerProtocolGuid
    EFI_COMPONENT_NAME_PROTOCOL_GUID,        //gComponentNameProtocolGuid
    10000,                                   //S3BusyClearTimeout
    5000,                                    //DmaAtaCompleteCommandTimeout
    16000,                                   //DmaAtaPiCompleteCommandTimeout
    5000,                                    //AtaPiResetCommandTimeout
    16000,                                   //AtaPiBusyClearTimeout
    10000,                                   //PoweonBusyClearTimeout 
    0                                        //CommandTimeout 
};


EFI_DRIVER_BINDING_PROTOCOL        gIdeBusDriverBinding = {
    IdeBusSupported,
    IdeBusStart,
    IdeBusStop,
    IDE_BUS_DRIVER_VERSION,  // version
    NULL,                    // ImageHandle
    NULL                     // DriverBindingHandle
};

// Global Buffer pointer used for Bus Mastering
VOID                               *gDescriptorBuffer = NULL;

VOID
InitMiscConfig (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

VOID
SetIdePortSpeed (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
);

/**
    Installs gIdeBusDriverBinding protocol

        
    @param ImageHandle 
    @param SystemTable 

    @retval EFI_STATUS

    @note  
      Here is the control flow of this function:
      1. Initialize Ami Lib.
      2. Install Driver Binding Protocol
      3. Return EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI IdeBusEntryPoint (
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS  Status;

    gIdeBusDriverBinding.DriverBindingHandle = NULL;
    gIdeBusDriverBinding.ImageHandle         = ImageHandle;

    InitAmiLib( ImageHandle, SystemTable );
   
    Status = pBS->LocateProtocol(
                                &gAmiPlatformIdeProtocolGuid,
                                NULL,
                                (void**)&gPlatformIdeProtocol
                                );
    if (EFI_ERROR (Status)) {
        //
        // if EFI_ERROR then Assign Default values Instance to gPlatformIdeProtocol and 
        // install the PLATFORM_IDE_PROTOCOL
        //
        gPlatformIdeProtocol = (PLATFORM_IDE_PROTOCOL *)&gPlatformIdeProtocolInstance;
        
        Status = pBS->InstallProtocolInterface( &ImageHandle,
                                                &gAmiPlatformIdeProtocolGuid,
                                                EFI_NATIVE_INTERFACE,
                                                &gPlatformIdeProtocol
                                              );
        ASSERT_EFI_ERROR(Status);
    }
    //
    // Assert when PLATFORM_IDE_PROTOCOL's revision is not supported 
    //
    ASSERT(!(gPlatformIdeProtocol->Revision > PLATFORM_IDE_PROTOCOL_SUPPORTED_REVISION));

    Status = pBS->InstallMultipleProtocolInterfaces(
        &gIdeBusDriverBinding.DriverBindingHandle,
        &gEfiDriverBindingProtocolGuid, &gIdeBusDriverBinding,
        &(gPlatformIdeProtocol->gComponentNameProtocolGuid), &gIdeBusControllerDriverName,
        NULL
        );

#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "IdeBusEntryPoint Exit Status %x\n", Status ));
#endif

    return Status;
}

/**
    Checks whether IDE_PROTOCOL_INTERFACE is installed on the controller.
    If 'yes', return SUCCESS else ERROR

        
    @param This 
    @param Controller 
    @param RemainingDevicePath 

    @retval EFI_STATUS

    @note  
     Here is the control flow of this function:
     1. If Devicepath is NULL, check "gEfiIdeControllerProtocolGuid" is installed by IdeController device driver,
     if yes, it is the IDE controller that this Bus will manage. Then return Success.
     2. If Devicepath is valid, check if "gAmiIdeBusInitProtocolGuid" is already installed by this BUS driver,
     if yes, check whether it is OK to configure this device. if not installed goto step 1.

**/
EFI_STATUS
IdeBusSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
)
{
    EFI_STATUS                      Status;
    VOID                            *IdeControllerInterface = NULL ;
    ATAPI_DEVICE_PATH               *AtapiRemainingDevicePath = (ATAPI_DEVICE_PATH*) RemainingDevicePath;
    AMI_IDE_BUS_INIT_PROTOCOL       *IdeBusInitInterface;
    EFI_PCI_IO_PROTOCOL             *PciIO;
    UINT8 PciConfig[256];

#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "IdeBusSupported Entry\n" ));
#endif


    //
    //Check for Valid ATAPI Device Path. If no return UNSUPPORTED

        //
        // RemainingDevicePath is not NULL and begins with the End of Device Path node, 
        // then the driver must not enumerate any of the children of Controller nor create any child device handle. 
        // Only the controller initialization should be performed. UEFI specification V 2.3.1, P. 10.1
        //
    //
    if ( !(AtapiRemainingDevicePath == NULL) && !(isEndNode(RemainingDevicePath))) {

        //
        // Check if the AtapiRemainingDevicePath is valid 8.3.4.1
        //
        if ( AtapiRemainingDevicePath->Header.Type != MESSAGING_DEVICE_PATH
             || AtapiRemainingDevicePath->Header.SubType != MSG_ATAPI_DP
             && NODE_LENGTH( &AtapiRemainingDevicePath->Header ) != ATAPI_DEVICE_PATH_LENGTH ) {
#if IDEBUS_DEBUG_PRINT == 1
            TRACE(( -1, "AtapiRemainingDevicePath not Valid\n" ));
#endif
            return EFI_UNSUPPORTED;
        }

        //
        //Now check whether it is OK to enumerate the specified device.
        //
        Status = pBS->OpenProtocol( Controller,
                                    &gAmiIdeBusInitProtocolGuid,
                                    (VOID**)&IdeBusInitInterface,
                                    This->DriverBindingHandle,
                                    Controller,
                                    EFI_OPEN_PROTOCOL_BY_DRIVER );

        if ( Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED ) {
            pBS->CloseProtocol(
                                Controller,
                                &gAmiIdeBusInitProtocolGuid,
                                This->DriverBindingHandle,
                                Controller
                                );

            if ( AtapiRemainingDevicePath->PrimarySecondary == PRIMARY_CHANNEL && AtapiRemainingDevicePath->SlaveMaster == MASTER_DRIVE ) {
                if ( IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][MASTER_DRIVE] >= DEVICE_DETECTION_FAILED ) {
                    return EFI_ALREADY_STARTED;
                }
            }

            if ( AtapiRemainingDevicePath->PrimarySecondary == PRIMARY_CHANNEL && AtapiRemainingDevicePath->SlaveMaster == SLAVE_DRIVE ) {
                if ( IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][SLAVE_DRIVE] >= DEVICE_DETECTION_FAILED ) {
                    return EFI_ALREADY_STARTED;
                }
            }

            if ( AtapiRemainingDevicePath->PrimarySecondary == SECONDARY_CHANNEL && AtapiRemainingDevicePath->SlaveMaster == MASTER_DRIVE ) {
                if ( IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][MASTER_DRIVE] >= DEVICE_DETECTION_FAILED ) {
                    return EFI_ALREADY_STARTED;
                }
            }

            if ( AtapiRemainingDevicePath->PrimarySecondary == SECONDARY_CHANNEL && AtapiRemainingDevicePath->SlaveMaster == SLAVE_DRIVE ) {
                if ( IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][SLAVE_DRIVE] >= DEVICE_DETECTION_FAILED ) {
                    return EFI_ALREADY_STARTED;
                }
            }
        }
    }

    //
    //Check whether IDE_CONTROLLER_PROTOCOL  has been installed on this controller
    //
    Status = pBS->OpenProtocol( Controller,
                                &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                (VOID**)&IdeControllerInterface,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_BY_DRIVER );

#if IDEBUS_DEBUG_PRINT == 1

    if ( EFI_ERROR( Status )) {
        if ( gPlatformIdeProtocol->EfiIdeProtocol )
            TRACE(( -1, "Error opening gEfiIdeControllerInitProtocolGuid\n" ));
    }
#endif

    //
    //IDE_CONTROLLER_PROTOCOL will be opened by each device. So EFI_ALREADY_STARTED is not an error
    //
    if  ( !(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        return EFI_UNSUPPORTED;
    }

    //
    //Close IDE_CONTROLLER_PROTOCOL
    //
    pBS->CloseProtocol(
                        Controller,
                        &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                        This->DriverBindingHandle,
                        Controller
                        );

#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "IdeBusSupported Exit Success\n" ));
#endif
    //
    // Check if Controller is in AHCI mode or not?
    //
    Status = pBS->OpenProtocol( Controller,
                                &gEfiPciIoProtocolGuid,
                                (VOID**)&PciIO,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL );

    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }

    Status = PciIO->Pci.Read( PciIO,
                              EfiPciIoWidthUint8,
                              0,
                              sizeof (PciConfig),
                              PciConfig
                              );
                              
    ASSERT_EFI_ERROR(Status);

    if ((PciConfig [IDE_SUB_CLASS_CODE] == SCC_AHCI_CONTROLLER) || (PciConfig [IDE_SUB_CLASS_CODE] == SCC_RAID_CONTROLLER)) {
        if ( !(gPlatformIdeProtocol->AhciCompatibleMode) )
            return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    Installs IDE Block IO Protocol

        
    @param This 
    @param Controller 
    @param RemainingDevicePath 

    @retval EFI_STATUS

    @note  
      Here is the control flow of this function:
     1.  Check if "gAmiIdeBusInitProtocolGuid" is already installed. If not, install it "InstallBusInitProtocol".
     2.  If Devicepath is valid, initialize so that only particular device will be detected and
         Configured, else all devices will be detected and configured.
     3.  check whether the device has not been already detected. If yes, skip it and go for next device
     4.  IdeBusInterface is initialized for this device. Update Base address.
     5.  Do the detection "DetectIdeDevice". Update the status in "IdeBusInitProtocol". If failure undo the process "IdeBusInterface".
     6.  Repeat for all devices from step 3.
     7.  Start the configuration process for devices which are detected successfully "DEVICE_DETECTED_SUCCESSFULLY".
     8.  "ConfigureIdeDeviceAndController" will do the configuration.
     9.  If above step is success, build Devicepath "CreateIdeDevicePath"
     10. Create the Block_io_Protocol"InitIdeBlockIO". Install it only if Device is not Password protected.
     11. Create the DISK_INFO_PROTOCOL "InitIdeDiskInfo". Install it only if Device is not Password protected.
     12. Install a child device with the above three protocols.
     13. Open "gEfiIdeControllerProtocolGuid" with the child handle.
     14. Repeat from step 8 for the remaining devices.

**/
EFI_STATUS
IdeBusStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath
)
{
    EFI_STATUS                       Status;
    EFI_PCI_IO_PROTOCOL              *PciIO;

    VOID                            *IdeControllerInterface = NULL ;
    AMI_IDE_BUS_INIT_PROTOCOL       *IdeBusInitInterface;
    AMI_IDE_BUS_PROTOCOL            *IdeBusInterface;
    VOID                            *TempProtocolPtr;
    UINT8                           Enumeration_Process = ENUMERATE_ALL;
    ATAPI_DEVICE_PATH               *AtapiRemainingDevicePath = (ATAPI_DEVICE_PATH*)RemainingDevicePath;

    UINT8                           Start_Channel = PRIMARY_CHANNEL;
    UINT8                           End_Channel   = SECONDARY_CHANNEL;
    UINT8                           Start_Device  = MASTER_DRIVE;
    UINT8                           End_Device    = SLAVE_DRIVE;
    INT8                            TotalDevice = 0;
    UINT8                           Current_Channel;
    UINT8                           Current_Device;
    

#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "IdeBusStart Entry\n" ));
#endif


    PROGRESS_CODE( DXE_IDE_BEGIN );
    
#if ( defined(BOOT_SECTOR_WRITE_PROTECT) && (BOOT_SECTOR_WRITE_PROTECT != 0) )
    if(AmiBlkWriteProtection == NULL) {
        Status = pBS->LocateProtocol( &gAmiBlockIoWriteProtectionProtocolGuid, 
                                      NULL, 
                                      &AmiBlkWriteProtection ); 
        if(EFI_ERROR(Status)) {
            AmiBlkWriteProtection = NULL;
        }
    }
#endif

    //
    //Open IDE_CONTROLLER_PROTOCOL. If success or Already opened, It is OK to proceed.
    //
    Status = pBS->OpenProtocol( Controller,
                                &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                (VOID**)&IdeControllerInterface,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_BY_DRIVER );

    if  ( !(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Get the PciIO interface
    //
    Status = pBS->OpenProtocol( Controller,
                                &gEfiPciIoProtocolGuid,
                                (VOID**)&PciIO,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL );

    ASSERT_EFI_ERROR(Status);
    //
    //Check if AMI_IDE_BUS_INIT_PROTOCOL installed.
    //
    Status = pBS->OpenProtocol( Controller,
                                &gAmiIdeBusInitProtocolGuid,
                                (VOID**)&IdeBusInitInterface,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_BY_DRIVER );


    if  ( !(Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED)) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(AMI_IDE_BUS_INIT_PROTOCOL),
                                    (VOID**)&IdeBusInitInterface
                                    );

        if ( EFI_ERROR( Status )) {
            return EFI_OUT_OF_RESOURCES; 
        }
        Status = InstallBusInitProtocol( Controller, IdeBusInitInterface, IdeControllerInterface );

        if ( EFI_ERROR( Status )) {
            if ( Status == EFI_OUT_OF_RESOURCES) {
                return EFI_OUT_OF_RESOURCES;
            } else {
                return EFI_DEVICE_ERROR;
            }
        }
    }

    //
    //	Check if AtapiRemainingDevicePath is valid or not
    //
    if ( !(AtapiRemainingDevicePath == NULL)) {

        //
        // RemainingDevicePath is not NULL and begins with the End of Device Path node, 
        // then the driver must not enumerate any of the children of Controller nor create any child device handle. 
        // Only the controller initialization should be performed. UEFI specification V 2.3.1, P. 10.1
        //
        if(isEndNode(RemainingDevicePath)) {
            return EFI_SUCCESS;
        }

        //
        // Check if the AtapiRemainingDevicePath is valid 8.3.4.1
        //
        if ( AtapiRemainingDevicePath->Header.Type != MESSAGING_DEVICE_PATH
             || AtapiRemainingDevicePath->Header.SubType != MSG_ATAPI_DP
             && NODE_LENGTH( &AtapiRemainingDevicePath->Header ) != ATAPI_DEVICE_PATH_LENGTH ) {
#if IDEBUS_DEBUG_PRINT == 1
            TRACE(( -1, "AtapiRemainingDevicePath not Valid\n" ));
#endif
            return EFI_DEVICE_ERROR;
        }

        if ( AtapiRemainingDevicePath->PrimarySecondary == PRIMARY_CHANNEL
             && AtapiRemainingDevicePath->SlaveMaster == MASTER_DRIVE ) {
            Enumeration_Process = ENUMERATE_PRIMARY_MASTER;
        }

        if ( AtapiRemainingDevicePath->PrimarySecondary == PRIMARY_CHANNEL
             && AtapiRemainingDevicePath->SlaveMaster == SLAVE_DRIVE ) {
            Enumeration_Process = ENUMERATE_PRIMARY_SLAVE;
        }

        if ( AtapiRemainingDevicePath->PrimarySecondary == SECONDARY_CHANNEL
             && AtapiRemainingDevicePath->SlaveMaster == MASTER_DRIVE ) {
            Enumeration_Process = ENUMERATE_SECONDARY_MASTER;
        }

        if ( AtapiRemainingDevicePath->PrimarySecondary == SECONDARY_CHANNEL
             && AtapiRemainingDevicePath->SlaveMaster == SLAVE_DRIVE ) {
            Enumeration_Process = ENUMERATE_SECONDARY_SLAVE;
        }

        switch ( Enumeration_Process )
        {
        case ENUMERATE_PRIMARY_MASTER:
            Start_Channel = PRIMARY_CHANNEL;
            End_Channel   = PRIMARY_CHANNEL;
            Start_Device  = MASTER_DRIVE;
            End_Device    = MASTER_DRIVE;
            break;
        case ENUMERATE_PRIMARY_SLAVE:
            Start_Channel = PRIMARY_CHANNEL;
            End_Channel   = PRIMARY_CHANNEL;
            Start_Device  = SLAVE_DRIVE;
            End_Device    = SLAVE_DRIVE;
            break;
        case ENUMERATE_SECONDARY_MASTER:
            Start_Channel = SECONDARY_CHANNEL;
            End_Channel   = SECONDARY_CHANNEL;
            Start_Device  = MASTER_DRIVE;
            End_Device    = MASTER_DRIVE;
            break;
        case ENUMERATE_SECONDARY_SLAVE:
            Start_Channel = SECONDARY_CHANNEL;
            End_Channel   = SECONDARY_CHANNEL;
            Start_Device  = SLAVE_DRIVE;
            End_Device    = SLAVE_DRIVE;
            break;
        default:
            break;
        }
    }


#if IDEBUS_DEBUG_PRINT == 1
            TRACE(( -1, "Enumerate = %x\n", Enumeration_Process ));
#endif


    //---------------------------------------------------------------------------------------------------------
    //	Device Detection Begins
    //---------------------------------------------------------------------------------------------------------
    PROGRESS_CODE( DXE_IDE_DETECT );

    for ( Current_Channel = Start_Channel; Current_Channel <= End_Channel; Current_Channel++ )
    {

        if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
            ( (EFI_IDE_CONTROLLER_INIT_PROTOCOL*)IdeControllerInterface )->NotifyPhase( (EFI_IDE_CONTROLLER_INIT_PROTOCOL*)IdeControllerInterface, EfiIdeBeforeChannelEnumeration, Current_Channel );
        }
        //
        // Calculate the Number of Devices to be detected in single Channel 
        //
        TotalDevice = ( End_Device - Start_Device ) + 1;

        if ( gPlatformIdeProtocol->MasterSlaveEnumeration ) {
            Current_Device = Start_Device;
        } else {
            Current_Device = End_Device;
        }

        while ( TotalDevice ) 
        {
            //
            // Check whether the given device is disabled by USER OR already processed
            //
            if ( IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_DISABLED
                 || IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_DETECTED_SUCCESSFULLY
                 || IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_CONFIGURED_SUCCESSFULLY
                 || IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_DETECTION_FAILED
                 || IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_REMOVED ) {

                if ( gPlatformIdeProtocol->MasterSlaveEnumeration ) {
                    Current_Device++;
                } else {
                    Current_Device--;
                }
                TotalDevice--;
                continue;
            }

            //
            // Allocate memory for AMI_IDE_BUS_PROTOCOL
            //

            IdeBusInterface = MallocZ( sizeof(AMI_IDE_BUS_PROTOCOL));

            if ( !IdeBusInterface ) {
                return EFI_OUT_OF_RESOURCES;
            }

            //
            // Initialize the fields
            //

            if ( gPlatformIdeProtocol->EfiIdeProtocol ) { 
                IdeBusInterface->EfiIdeControllerInterface =(EFI_IDE_CONTROLLER_INIT_PROTOCOL*) IdeControllerInterface;
                IdeBusInterface->IdeControllerInterfaceHandle = Controller;
            }

            IdeBusInterface->IdeBusInitInterface    = IdeBusInitInterface;
            IdeBusInterface->PciIO                  = PciIO;
            IdeBusInterface->IdeDevice.PciIO        = PciIO;
            IdeBusInterface->IdeDevice.Channel      = Current_Channel;
            IdeBusInterface->IdeDevice.Device       = Current_Device;
            IdeBusInterface->IdeSoftReset           = IdeSoftReset;
            IdeBusInterface->AtaReadWritePio        = AtaReadWritePio;
            IdeBusInterface->AtaPioDataIn           = AtaPioDataIn;
            IdeBusInterface->AtaPioDataOut          = AtaPioDataOut;
            IdeBusInterface->AtaAtapiDmaDataCommand = AtaAtapiDmaDataCommand;
            IdeBusInterface->IdeNonDataCommand      = IdeNonDataCommandExp;
            IdeBusInterface->WaitForCmdCompletion   = WaitForCmdCompletion;
            IdeBusInterface->GeneralAtapiCommandAndData = GeneralAtapiCommandAndData;

            SetIdePortSpeed(IdeBusInterface);   
            
            Status = UpdateBaseAddress( IdeBusInterface );                   
             if ( Status == EFI_SUCCESS ) {
                Status = DetectIdeDevice( IdeBusInterface );
            }

            if ( EFI_ERROR( Status ) ) {
                //
                // Update IdeBusInitInterface
                //
                IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] = DEVICE_DETECTION_FAILED;
                pBS->FreePool( IdeBusInterface );
 
                if ( gPlatformIdeProtocol->MasterSlaveEnumeration ) {
                    Current_Device++;
                } else {
                    Current_Device--;
                }
                TotalDevice--;
                continue;
            } else {
                IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device]  = DEVICE_DETECTED_SUCCESSFULLY;
                IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device] = IdeBusInterface; // Save the Pointer
            }

            if ( gPlatformIdeProtocol->MasterSlaveEnumeration ) {
                Current_Device++;
            } else {
                Current_Device--;
            }
            TotalDevice--;
        }                                                               // Loop for Master/Slave
    }                                                                   // Loop for Primary/Secondary

    //---------------------------------------------------------------------------------------------------------
    //	Device Detection Ends
    //---------------------------------------------------------------------------------------------------------
    //	Configuration Starts
    //---------------------------------------------------------------------------------------------------------
    PROGRESS_CODE( DXE_IDE_ENABLE );

    for ( Current_Channel = Start_Channel; Current_Channel <= End_Channel; Current_Channel++ )
    {
        for ( Current_Device = Start_Device; Current_Device <= End_Device; Current_Device++ )
        {

            //  
            //Check whether the given device is detected successfully
            //
            if ( IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_DETECTED_SUCCESSFULLY ) {
                IdeBusInterface = IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device];

                Status = ConfigureIdeDeviceAndController( IdeBusInterface, IdeControllerInterface, IdeBusInitInterface );

                if ( EFI_ERROR( Status )) {
                    ERROR_CODE( DXE_IDE_DEVICE_FAILURE, EFI_ERROR_MAJOR );

                    pBS->FreePool( IdeBusInterface );

                    IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device] = NULL;
                    continue;
                }

                //
                //Device has been successfully detected and configured.
                //

                //
                //Create the Devicepath
                //  
                Status = CreateIdeDevicePath( This, Controller, IdeBusInitInterface, IdeBusInterface, RemainingDevicePath, Current_Channel, Current_Device );

                if ( EFI_ERROR( Status )) {
                    pBS->FreePool( IdeBusInterface );
                    continue;
                }

                //
                //Initialize Block_IO Protocol
                //
                Status = InitIdeBlockIO( IdeBusInterface );

                if ( EFI_ERROR( Status )) {
                    pBS->FreePool( IdeBusInterface );
                    continue;
                }

                //
                //Initialize IDE EFI_DISK_INFO_PROTOCOL
                //
                Status = InitIdeDiskInfo( IdeBusInterface );

                if ( EFI_ERROR( Status )) {
                    pBS->FreePool( IdeBusInterface );
                    continue;
                }

                //
                //Install Devicepath
                //
                Status = pBS->InstallMultipleProtocolInterfaces(
                                            &(IdeBusInterface->IdeDeviceHandle),
                                            &gEfiDevicePathProtocolGuid, IdeBusInterface->DevicePathProtocol,
                                            NULL );

                if ( EFI_ERROR( Status )) {
                    IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] = DEVICE_DETECTION_FAILED;
                    pBS->FreePool( IdeBusInterface );
                    continue;
                }

                //
                //Open IdeControllerProtocol
                //
                Status = pBS->OpenProtocol( Controller,
                                            &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                            (VOID**)&IdeControllerInterface,
                                            This->DriverBindingHandle,
                                            IdeBusInterface->IdeDeviceHandle,
                                            EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER );

                ASSERT_EFI_ERROR(Status);

                if ( gPlatformIdeProtocol->IdePwrManagementInterfaceSupport ) {
                    InstallIDEPowerMgmtInterface( IdeBusInterface );
                }

                if ( gPlatformIdeProtocol->HostProtectedAreaSupport ) {
                    InstallHpaInterface( IdeBusInterface );
                }

#if IDEBUS_DEBUG_PRINT == 1
                PrintIdeDeviceInfo( IdeBusInterface );
#endif

            }                                               // DEVICE_CONFIGURED_SUCCESSFULLY

            //Before installing BlockIO and DiskInfo protocol, check whether the device has been
            //password protected. If yes, BlockIO and DiskInfo will not be installed.
            IdeBusInterface = IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device];

            if ( IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_CONFIGURED_SUCCESSFULLY ) {
                //
                //Check BlockIO has been installed or not.
                //
                Status = pBS->OpenProtocol( IdeBusInterface->IdeDeviceHandle,
                                            &gEfiBlockIoProtocolGuid,
                                            NULL,
                                            This->DriverBindingHandle,
                                            IdeBusInterface->IdeDeviceHandle,
                                            EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

                if ( EFI_ERROR( Status )) {
                    //
                    //BLOCKIO not installed and device has been configured successfully
                    //  
                    Status = pBS->InstallMultipleProtocolInterfaces(
                                        &(IdeBusInterface->IdeDeviceHandle),
                                        &gEfiBlockIoProtocolGuid, (EFI_BLOCK_IO_PROTOCOL*)(IdeBusInterface->IdeBlkIo),
                                        &gEfiDiskInfoProtocolGuid, (EFI_DISK_INFO_PROTOCOL*)(IdeBusInterface->IdeDiskInfo),
                                        NULL );
                }                                                       // If BlockIO not Installed
            }                                                           // End of installing BlockIO and DiskInfo
        }                                                               // Loop for Master/Slave
    }                                                                   // Loop for Primary/Secondary

    //---------------------------------------------------------------------------------------------------------
    //	Configuration Ends
    //---------------------------------------------------------------------------------------------------------

    Status = pBS->LocateProtocol (
                                &gAmiAtaPassThruInitProtocolGuid,
                                NULL,
                                (VOID **) &AtaPassThruInitProtocol
                                );

    if(!EFI_ERROR(Status)) {
        if(AtaPassThruInitProtocol != NULL) {
            AtaPassThruInitProtocol->InstallAtaPassThru(Controller, FALSE);
        }
    }

    // SCSIPassThruAtapi install
    Status = pBS->LocateProtocol (
                                &gAmiScsiPassThruInitProtocolGuid,
                                NULL,
                                (VOID **) &gScsiPassThruInitProtocol
                                );

    if(!EFI_ERROR(Status)) {
        if(gScsiPassThruInitProtocol != NULL) {
            gScsiPassThruInitProtocol->InstallScsiPassThruAtapi(Controller, FALSE);
        }
    }

    //Install other optional Protocols - HDD Security, Opal Security, HDD Smart etc.

    if ( HddSecurityInitProtocol == NULL) {
        pBS->LocateProtocol( &gAmiHddSecurityInitProtocolGuid,
                             NULL,
                             (VOID **) &HddSecurityInitProtocol);
    }

#if defined(ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT) && (ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT != 0)
    if ( gHddAcousticInitProtocol == NULL) {
        pBS->LocateProtocol( &gHddAcousticInitProtocolGuid,
                             NULL,
                             (VOID **) &gHddAcousticInitProtocol);
    }
#endif

    if(OpalSecInitProtocol == NULL) {
        pBS->LocateProtocol ( &gAmiHddOpalSecInitProtocolGuid,
                              NULL,
                              (VOID **)  &OpalSecInitProtocol);
    }

    if ( HddSmartInitProtocol == NULL ) {
        pBS->LocateProtocol( &gAmiHddSmartInitProtocolGuid,
                             NULL,
                             (VOID **) &HddSmartInitProtocol);
    }

    for ( Current_Channel = Start_Channel; Current_Channel <= End_Channel; Current_Channel++ ) {

        for ( Current_Device = Start_Device; Current_Device <= End_Device; Current_Device++ ) {

            if ( IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_CONFIGURED_SUCCESSFULLY ) {
                IdeBusInterface = IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device];

                if(IdeBusInterface->IsDeviceFeatureDone) {
                    continue;
                }
                IdeBusInterface->IsDeviceFeatureDone = TRUE;

                if ( HddSecurityInitProtocol != NULL) {
                    HddSecurityInitProtocol->InstallSecurityInterface( IdeBusInterface, AmiStorageInterfaceIde);
                } else {
                    // If Security Feature support is not enabled, always freeze lock the security feature
                    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_82 & 0x2 ) {
                        Status = IdeNonDataCommand( IdeBusInterface, 0, 0,
                                                    0, 0, 0,
                                                    IdeBusInterface->IdeDevice.Device << 4,
                                                    SECURITY_FREEZE_LOCK );
                        // if Device Configuration Overlay feature set supported then issue the
                        // Dev configure Free lock command.
                        if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x800 ) {
                            Status = IdeNonDataCommand( IdeBusInterface, DEV_CONFIG_FREEZE_LOCK_FEATURES, 0,
                                                        0, 0, 0,
                                                        IdeBusInterface->IdeDevice.Device << 4,
                                                        DEV_CONFIG_FREEZE_LOCK );
                        }
                        // Update the Identify device buffer
                        GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));
                    }
                }

#if defined(ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT) && (ACOUSTIC_MANAGEMENT_DRIVER_SUPPORT != 0)
                if(gHddAcousticInitProtocol != NULL) {
                    gHddAcousticInitProtocol->InitHddAcoustic(IdeBusInterface, AmiStorageInterfaceIde);
                }
#endif
                
                if(OpalSecInitProtocol != NULL) {
                    OpalSecInitProtocol->InstallOpalSecurityInterface(IdeBusInterface, AmiStorageInterfaceIde);
                }

                if ( HddSmartInitProtocol != NULL ) {

                    HddSmartInitProtocol->InitSmartSupport( IdeBusInterface, AmiStorageInterfaceIde);

                    //Get the updated IdentifyData
                    GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));

                    if ( HddSmartInitProtocol->SmartDiagonasticFlag ) {
                        HddSmartInitProtocol->InstallSmartInterface( IdeBusInterface, AmiStorageInterfaceIde);
                    }
                }

            } // end of if DEVICE_CONFIGURED_SUCCESSFULLY
        } // Loop for Master/Slave
    } // Loop for Primary/Secondary

    // Handle the OnBoard Raid controller Password Verification

    Status = pBS->HandleProtocol(Controller, 
                                 &gAmiHddSecurityEndProtocolGuid, 
                                 & TempProtocolPtr
                                 );

    if(Status == EFI_SUCCESS) {
    	//
        // Protocol already installed on the Controller handle.
        // UnInstall and Install back the protocol interface to Notify the Password verification 
        //
        Status = pBS->UninstallProtocolInterface(
                                Controller, 
                                &gAmiHddSecurityEndProtocolGuid, 
                                NULL
                                );
        
        ASSERT_EFI_ERROR(Status);
        
        Status = pBS->InstallProtocolInterface(
                                &Controller, 
                                &gAmiHddSecurityEndProtocolGuid, 
                                EFI_NATIVE_INTERFACE,
                                NULL
                                );
        
        ASSERT_EFI_ERROR(Status);
    } else {
        //
        // This will notify AMITSE to invoke the HDD password Screen
        //
        Status = pBS->InstallProtocolInterface(
                                &Controller, 
                                &gAmiHddSecurityEndProtocolGuid, 
                                EFI_NATIVE_INTERFACE,
                                NULL
                                );
        
        ASSERT_EFI_ERROR(Status);
    }


#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "IdeBusStart Exit Success\n" ));
#endif

    return EFI_SUCCESS;
}

/**
    Uninstall all devices installed in start procedure.

        
    @param This 
    @param Controller 
    @param RemainingDevicePath 

    @retval 
        EFI_STATUS

    @note  
     Here is the control flow of this function:
    1.  Check whether "gAmiIdeBusInitProtocolGuid" is installed on this controller. If not exit with error.
    2.  If "NumberOfChildren" is zero, check weather all child devices have been stopped. If not exit with error.
        if all child devices have been stopped, then close "gEfiIdeControllerProtocolGuid" and "gAmiIdeBusInitProtocolGuid",
        uninstall "gAmiIdeBusInitProtocolGuid" and then exit with success.
    3.  If "NumberOfChildren" is non-zero,	close "gEfiIdeControllerProtocolGuid" opened by the child device in start function.
        uninstall all protocols installed on this child device in start function,
        free up all resources allocated in start function. Repeat step	3 for all child devices and	return success at the end.
**/
EFI_STATUS
IdeBusStop (
    IN  EFI_DRIVER_BINDING_PROTOCOL *This,
    IN  EFI_HANDLE                  Controller,
    IN  UINTN                       NumberOfChildren,
    IN  EFI_HANDLE                  *ChildHandleBuffer
)
{
    AMI_IDE_BUS_INIT_PROTOCOL       *IdeBusInitInterface;
    AMI_IDE_BUS_PROTOCOL            *IdeBusInterface = NULL;
    EFI_STATUS                      Status;
    VOID                            *IdeControllerInterface = NULL ;
    UINT8                           Current_Channel = 0xff;
    UINT8                           Current_Device  = 0xff;
    UINT8                           Index   = 0;
    BOOLEAN                         Flag = TRUE;
    EFI_DEVICE_PATH_PROTOCOL        *DevicePath;

    //
    //Check if AMI_IDE_BUS_INIT_PROTOCOL is installed on the Controller.
    //
    Status = pBS->OpenProtocol( Controller,
                                &gAmiIdeBusInitProtocolGuid,
                                (VOID**)&IdeBusInitInterface,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Check if ChildHandleBuffer is valid
    //
    if ( NumberOfChildren ) {
        while ( NumberOfChildren )
        {
            Status = pBS->OpenProtocol(
                ChildHandleBuffer[Index],
                &gEfiDevicePathProtocolGuid,
                (VOID**)&DevicePath,
                This->DriverBindingHandle,
                Controller,
                EFI_OPEN_PROTOCOL_GET_PROTOCOL );

            //Lookout for ATAPI device path
            //ACPI_DEVICE path, PCI Device path and then ATAPI device path will be the sequence
            do
            {
                if ((DevicePath->Type == MESSAGING_DEVICE_PATH) && (DevicePath->SubType == MSG_ATAPI_DP)) {
                    Current_Channel = ((ATAPI_DEVICE_PATH*)DevicePath)->PrimarySecondary;
                    Current_Device  = ((ATAPI_DEVICE_PATH*)DevicePath)->SlaveMaster;
                    break;
                } else {
                    DevicePath = NEXT_NODE( DevicePath );
                }
            } while ( DevicePath->Type != END_DEVICE_PATH );

            if ( Current_Channel == 0xff || Current_Device == 0xff ) {
                return EFI_DEVICE_ERROR;
            }

            //
            //Get the pointer to AMI_IDE_BUS_PROTOCOL
            //
            IdeBusInterface = IdeBusInitInterface->pIdeBusProtocol[Current_Channel][Current_Device];

            //
            //Close all the protocols opened by this Child Device
            //
            pBS->CloseProtocol(
                        Controller,
                        &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                        This->DriverBindingHandle,
                        ChildHandleBuffer[Index] );

            //Before uninstall BLOCKIO check whether it is installed or not
            Status = pBS->OpenProtocol( ChildHandleBuffer[Index],
                                        &gEfiBlockIoProtocolGuid,
                                        NULL,
                                        This->DriverBindingHandle,
                                        ChildHandleBuffer[Index],
                                        EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

            if ( Status == EFI_SUCCESS ) {
                Status = pBS->UninstallMultipleProtocolInterfaces(
                                    ChildHandleBuffer[Index],
                                    &gEfiBlockIoProtocolGuid, (EFI_BLOCK_IO_PROTOCOL*)(IdeBusInterface->IdeBlkIo),
                                    &gEfiDiskInfoProtocolGuid, (EFI_DISK_INFO_PROTOCOL*)(IdeBusInterface->IdeDiskInfo),
                                    NULL );
            }

            Status = pBS->    UninstallMultipleProtocolInterfaces(
                ChildHandleBuffer[Index],
                &gEfiDevicePathProtocolGuid, IdeBusInterface->DevicePathProtocol,
                NULL );

            if ( EFI_ERROR( Status )) {
                pBS->OpenProtocol( Controller,
                                   &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                   (VOID**)&IdeControllerInterface,
                                   This->DriverBindingHandle,
                                   ChildHandleBuffer[Index],
                                   EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER );

                return EFI_DEVICE_ERROR;
            }  else {
                //
                //Now free up all resources allocated.
                //
                if ( IdeBusInterface->IdeDevice.AtapiDevice != NULL ) {
                    pBS->FreePool( IdeBusInterface->IdeDevice.AtapiDevice->PacketBuffer );
                    pBS->FreePool( IdeBusInterface->IdeDevice.AtapiDevice->InquiryData );
                    pBS->FreePool( IdeBusInterface->IdeDevice.AtapiDevice );
                }

                //
                //Free up resources allocated for component names
                //
                if ( IdeBusInterface->IdeDevice.UDeviceName != NULL ) {
                    pBS->FreePool( IdeBusInterface->IdeDevice.UDeviceName->Language );
                    pBS->FreePool( IdeBusInterface->IdeDevice.UDeviceName->UnicodeString );
                    pBS->FreePool( IdeBusInterface->IdeDevice.UDeviceName );
                }

                // Uninstall optional protocols

                //
                //Before uninstall HDD security check whether it is installed or not
                //
                Status = pBS->OpenProtocol( ChildHandleBuffer[Index],
                                            &gAmiHddSecurityProtocolGuid,
                                            NULL,
                                            This->DriverBindingHandle,
                                            ChildHandleBuffer[Index],
                                            EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

                if ( !EFI_ERROR( Status )) {
                    if ( HddSecurityInitProtocol != NULL ) {
                        HddSecurityInitProtocol->StopSecurityModeSupport( IdeBusInterface, AmiStorageInterfaceIde );
                    }
                }

                //
                //Before uninstall Hdd Smart check whether it is installed or not
                //
                Status = pBS->OpenProtocol( ChildHandleBuffer[Index],
                                            &gAmiHddSmartProtocolGuid,
                                            NULL,
                                            This->DriverBindingHandle,
                                            ChildHandleBuffer[Index],
                                            EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

                if ( !EFI_ERROR( Status )) {
                    if ( HddSmartInitProtocol != NULL ) {
                        if ( HddSmartInitProtocol->SmartDiagonasticFlag ) {
                            HddSmartInitProtocol->UnInstallSmartInterface( IdeBusInterface, AmiStorageInterfaceIde );
                        }
                    }
                }

                //
                //Before uninstall OPAL security Interface check whether it is installed or not.
                //
                Status = pBS->OpenProtocol( ChildHandleBuffer[Index],
                                            &gEfiStorageSecurityCommandProtocolGuid,
                                            NULL,
                                            This->DriverBindingHandle,
                                            ChildHandleBuffer[Index],
                                            EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

                if ( !EFI_ERROR( Status )) {
                    if ( OpalSecInitProtocol != NULL ) {
                        OpalSecInitProtocol->UnInstallOpalSecurityInterface( IdeBusInterface, AmiStorageInterfaceIde );
                    }
                }

                if ( gPlatformIdeProtocol->IdePwrManagementInterfaceSupport ) {
                    StopIDEPowerMgmtInterface( IdeBusInterface );
                }

                if ( gPlatformIdeProtocol->HostProtectedAreaSupport ) {
	                StopHpaInterface( IdeBusInterface );
                }

                pBS->FreePool( IdeBusInterface->IdeBlkIo->BlkIo.Media );
                pBS->FreePool( IdeBusInterface->IdeBlkIo );
                pBS->FreePool( IdeBusInterface->IdeDiskInfo );
                Current_Channel = IdeBusInterface->IdeDevice.Channel;
                Current_Device  = IdeBusInterface->IdeDevice.Device;
                IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] = DEVICE_IN_RESET_STATE;
                pBS->FreePool( IdeBusInterface->DevicePathProtocol );
                pBS->FreePool( IdeBusInterface );
            }
            NumberOfChildren--;
            Index++;
        }

        return EFI_SUCCESS;
    } else {
        //   
        //Check if AMI_IDE_BUS_INIT_PROTOCOL can be removed
        //
        for ( Current_Channel = PRIMARY_CHANNEL; Current_Channel <= SECONDARY_CHANNEL; Current_Channel++ )
        {
            for ( Current_Device = MASTER_DRIVE; Current_Device <= SLAVE_DRIVE; Current_Device++ )
            {
                if ( IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] == DEVICE_CONFIGURED_SUCCESSFULLY ) {
                    Flag = FALSE;
                }
            }
        }

        //
        // Can't uninstall AMI_IDE_BUS_INIT_PROTOCOL if any one of the IDE devices are in "CONFIGURED" state.
        //
        if ( Flag == TRUE ) {
            if ( gPlatformIdeProtocol->IdeHPSupport ) {

                //
                // Check whether already HP Event has been created and also HP is supported
                //    
                if ( IdeBusInitInterface->HPEvent != NULL ) {
                    pBS->CloseEvent( IdeBusInitInterface->HPEvent );
                }
            }
            //    
            //Close all the protocols opened in Start Function
            //
            Status = pBS->CloseProtocol( Controller,
                                         &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                         This->DriverBindingHandle,
                                         Controller );
            ASSERT_EFI_ERROR(Status);
            //
            // AtaPass Thru uninstall
            //
            Status = pBS->LocateProtocol (
                                &gAmiAtaPassThruInitProtocolGuid,
                                NULL,
                                (VOID**)&AtaPassThruInitProtocol
                        );

            if(!EFI_ERROR(Status)) {
                 if(AtaPassThruInitProtocol != NULL) {
                     AtaPassThruInitProtocol->StopAtaPassThruSupport(Controller, FALSE);
                 }
             }
            //
            // ScsiPassThruAtapi uninstall
            //
            Status = pBS->LocateProtocol (
                                &gAmiScsiPassThruInitProtocolGuid,
                                NULL,
                                (VOID**)&gScsiPassThruInitProtocol);

             if(!EFI_ERROR(Status)) {
                 if(gScsiPassThruInitProtocol != NULL) {
                     gScsiPassThruInitProtocol->StopScsiPassThruAtapiSupport(Controller, FALSE);
                 }
             }


            Status = pBS->CloseProtocol( Controller,
                                         &gAmiIdeBusInitProtocolGuid,
                                         This->DriverBindingHandle,
                                         Controller );
            ASSERT_EFI_ERROR(Status);												

            Status = pBS->UninstallProtocolInterface( Controller,
                                                      &gAmiIdeBusInitProtocolGuid,
                                                      IdeBusInitInterface );

            if ( EFI_ERROR( Status ))  {
                Status = pBS->OpenProtocol( Controller,
                                            &gAmiIdeBusInitProtocolGuid,
                                            (VOID**)&IdeBusInitInterface,
                                            This->DriverBindingHandle,
                                            Controller,
                                            EFI_OPEN_PROTOCOL_BY_DRIVER );
                ASSERT_EFI_ERROR(Status);

                Status = pBS->OpenProtocol( Controller,
                                            &(gPlatformIdeProtocol->gIdeControllerProtocolGuid),
                                            (VOID**)&IdeControllerInterface,
                                            This->DriverBindingHandle,
                                            Controller,
                                            EFI_OPEN_PROTOCOL_BY_DRIVER );
                ASSERT_EFI_ERROR(Status);

                return EFI_DEVICE_ERROR;
            }

            pBS->FreePool( IdeBusInterface );

            if ( gDescriptorBuffer != NULL ) {
                pBS->FreePool( gDescriptorBuffer );
            }
            gDescriptorBuffer = NULL;
            return EFI_SUCCESS;
        } else {
            return EFI_DEVICE_ERROR;
        }
    }
}

/**
    Installs BUS Init Protocol on the IDE controller Handle

        
    @param  Controller 
    @param  AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitInterface;
    @param  IDE_CONTROLLER_PROTOCOL     *IdeControllerInterface

    @retval EFI_STATUS

    @note  
    Here is the control flow of this function:
    1. Call "IdeGetControllerInfo", to get channel information from IdeController driver.
    2. Install "gAmiIdeBusInitProtocolGuid" on the IDE controller.

**/
EFI_STATUS
InstallBusInitProtocol (
    IN  EFI_HANDLE                       Controller,
    IN  OUT AMI_IDE_BUS_INIT_PROTOCOL    *IdeBusInitInterface,
    IN  VOID                             *IdeControllerInterfaceIn
)
{
    CONTROLLER_INFO                     *ControllerInfo;
    EFI_IDE_CONTROLLER_INIT_PROTOCOL    *EfiIdeControllerInterface = NULL ;
    EFI_STATUS                           Status;

    //
    // Initialize IdeControllerInterface
    //
    if ( gPlatformIdeProtocol->EfiIdeProtocol )
        EfiIdeControllerInterface = ( EFI_IDE_CONTROLLER_INIT_PROTOCOL *) IdeControllerInterfaceIn;
    //
    //Initialize the default Values
    //
    ZeroMemory( IdeBusInitInterface, sizeof(AMI_IDE_BUS_INIT_PROTOCOL));

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(CONTROLLER_INFO),
                                (VOID**)&ControllerInfo
                                );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    ZeroMemory( ControllerInfo, sizeof(CONTROLLER_INFO));

    if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
        ControllerInfo->Flags = 0; // ACOUSTIC_SUPPORT_DISABLE

        if ( gPlatformIdeProtocol->SBIdeSupport ) {
            Status = pBS->LocateProtocol(&gIdeSetupProtocolguid,
                                         NULL,
                                         (VOID**)&gIdeSetupProtocol);

            if ( gPlatformIdeProtocol->AcousticManagementSupport ) {  

                if (!EFI_ERROR(Status)) {

                    if(gIdeSetupProtocol->AcousticPwrMgmt)
                        ControllerInfo->Flags |= ACOUSTIC_SUPPORT_ENABLE;
                    else
                        ControllerInfo->Flags &= ~ACOUSTIC_SUPPORT_ENABLE;
                    ControllerInfo->Acoustic_Management_Level   = gIdeSetupProtocol->AcousticLevel;
                } else {
                    ControllerInfo->Flags &= ~ACOUSTIC_SUPPORT_ENABLE ;
                    ControllerInfo->Acoustic_Management_Level   = ACOUSTIC_LEVEL_BYPASS;
                }
            } //End of ACOUSTIC_MANAGEMENT_SUPPORT   
        } //End of SBIDE_SUPPORT

        if ( gPlatformIdeProtocol->HddPowerLossInS3 ) {
            ControllerInfo->Flags |= S3_RESUME_UNLOCK_HDD_PASSWORD;
        }

        ControllerInfo->PrimaryChannel        = PRIMARY_CHANNEL_ENABLE;
        ControllerInfo->PrimaryMasterDevice   = PRIMARY_MASTER_DRIVE_ENABLE;
        ControllerInfo->PrimarySlaveDevice    = PRIMARY_SLAVE_DRIVE_ENABLE;
        ControllerInfo->SecondaryChannel      = SECONDARY_CHANNEL_ENABLE;
        ControllerInfo->SecondaryMasterDevice = SECONDARY_MASTER_DRIVE_ENABLE;
        ControllerInfo->SecondarySlaveDevice  = SECONDARY_SLAVE_DRIVE_ENABLE;
        ControllerInfo->BusMasterEnable       = BUSMASTER_ENABLE;
        ControllerInfo->HPMask                = 0;           // Hot Plug Mask


    }

    if ( ControllerInfo->PrimaryChannel == DEVICE_DISABLED ) {
        IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][MASTER_DRIVE] = DEVICE_DISABLED;
        IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][SLAVE_DRIVE]  = DEVICE_DISABLED;
    } else {
        IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][MASTER_DRIVE] = ControllerInfo->PrimaryMasterDevice;
        IdeBusInitInterface->IdeBusInitData[PRIMARY_CHANNEL][SLAVE_DRIVE]  = ControllerInfo->PrimarySlaveDevice;
    }

    if ( ControllerInfo->SecondaryChannel == DEVICE_DISABLED ) {
        IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][MASTER_DRIVE] = DEVICE_DISABLED;
        IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][SLAVE_DRIVE]  = DEVICE_DISABLED;
    } else {
        IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][MASTER_DRIVE] = ControllerInfo->SecondaryMasterDevice;
        IdeBusInitInterface->IdeBusInitData[SECONDARY_CHANNEL][SLAVE_DRIVE]  = ControllerInfo->SecondarySlaveDevice;
    }

    IdeBusInitInterface->BusMasterSelection        = ControllerInfo->BusMasterEnable;
    IdeBusInitInterface->HPMask                    = ControllerInfo->HPMask;
    if ( gPlatformIdeProtocol->EfiIdeProtocol )
        IdeBusInitInterface->EfiIdeControllerInterface = EfiIdeControllerInterface;


    IdeBusInitInterface->Flags                     = ControllerInfo->Flags;
    IdeBusInitInterface->Acoustic_Management_Level = ControllerInfo->Acoustic_Management_Level;

    pBS->FreePool( ControllerInfo );

    // Initialize PrevPortNum and PrevPortMultiplierPortNum for AtaPassThru to 0xFFFF

    IdeBusInitInterface->PrevPortNum = 0xffff;
    IdeBusInitInterface->PrevPortMultiplierPortNum = 0xffff;

    Status = pBS->InstallProtocolInterface(
        &Controller,
        &gAmiIdeBusInitProtocolGuid,
        EFI_NATIVE_INTERFACE,
        IdeBusInitInterface );

    if ((IdeBusInitInterface->BusMasterSelection) &&  gDescriptorBuffer == NULL ) {
        //Since the PRD table need to be aligned at 64KB to create maximum # of tables
        //allocate 128KB and use the 64KB aligned address,
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    2 * 64 * 1024,
                                    (VOID**)&gDescriptorBuffer
                                    );

        if ( EFI_ERROR( Status )) {
            IdeBusInitInterface->BusMasterSelection = 0;
        }
    }

#if IDEBUS_DEBUG_PRINT == 1
    TRACE(( -1, "Installed AMI_IDE_BUS_INIT_PROTOCOL\n" ));
#endif


    return Status;
}

/**
    PIO and DMA mode programming both on the Controller as well as on the Device is done

    @param  AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitInterface
    @param  IDE_CONTROLLER_PROTOCOL     *IdeControllerInterface
    @param  AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitInterface

    @retval EFI_STATUS

    @note  
    Here is the control flow of this function:
     1. Get the Best PIO and DMA mode supported by the device from Identify Data
     2. Set the PIO and DMA mode in the IDE controller.
     3. Configure the IDE device with the PIO and DMA mode.
     4. Update the Identify Data.
     5. Based on the IDE device select the proper Read/Write commands.
     6. Construct a unicode string for the IDE device.

**/
EFI_STATUS
ConfigureIdeDeviceAndController (
    IN  AMI_IDE_BUS_PROTOCOL                *IdeBusInterface,
    IN  VOID                                *IdeControllerInterfaceIn,
    IN  AMI_IDE_BUS_INIT_PROTOCOL           *IdeBusInitInterface
)
{
    EFI_STATUS              Status;
    UINT8                   Current_Channel = IdeBusInterface->IdeDevice.Channel;
    UINT8                   Current_Device  = IdeBusInterface->IdeDevice.Device;
    EFI_ATA_COLLECTIVE_MODE *SupportedModes = NULL;
    EFI_IDE_CONTROLLER_INIT_PROTOCOL       *EfiIdeControllerInterface = NULL ;


    if ( gPlatformIdeProtocol->EfiIdeProtocol )
        EfiIdeControllerInterface = (EFI_IDE_CONTROLLER_INIT_PROTOCOL*) IdeControllerInterfaceIn;

    if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
        GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));
        Status = EfiIdeControllerInterface->SubmitData( EfiIdeControllerInterface, IdeBusInterface->IdeDevice.Channel,
                                                 IdeBusInterface->IdeDevice.Device, (EFI_IDENTIFY_DATA*)&(IdeBusInterface->IdeDevice.IdentifyData));

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        Status = EfiIdeControllerInterface->CalculateMode( EfiIdeControllerInterface, IdeBusInterface->IdeDevice.Channel,
                                                    IdeBusInterface->IdeDevice.Device, &SupportedModes );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

        Status = EfiIdeControllerInterface->SetTiming( EfiIdeControllerInterface, IdeBusInterface->IdeDevice.Channel,
                                                IdeBusInterface->IdeDevice.Device, SupportedModes );

        if ( EFI_ERROR( Status )) {
            return Status;
        }

        IdeBusInterface->IdeDevice.PIOMode = 0xff;
        IdeBusInterface->IdeDevice.SWDma   = 0xff;
        IdeBusInterface->IdeDevice.MWDma   = 0xff;
        IdeBusInterface->IdeDevice.UDma    = 0xff;

        if ( SupportedModes->PioMode.Valid ) {
            IdeBusInterface->IdeDevice.PIOMode = SupportedModes->PioMode.Mode;
        }

        if ( SupportedModes->SingleWordDmaMode.Valid ) {
            IdeBusInterface->IdeDevice.SWDma = SupportedModes->SingleWordDmaMode.Mode;
        }

        if ( SupportedModes->MultiWordDmaMode.Valid ) {
             IdeBusInterface->IdeDevice.MWDma = SupportedModes->MultiWordDmaMode.Mode;
        }

        if ( SupportedModes->UdmaMode.Valid ) {
            IdeBusInterface->IdeDevice.UDma = SupportedModes->UdmaMode.Mode;
        }

        IdeBusInterface->IdeDevice.IORdy = (UINT8)((((EFI_IDENTIFY_DATA*)&(IdeBusInterface->IdeDevice.IdentifyData))->AtaData.capabilities_49 & BIT11) >> 11);

    }

    //	Issue SET feature command to set the PIO and DMA mode
    Status = ConfigureIdeDevice( IdeBusInterface, IdeBusInitInterface );

    if ( EFI_ERROR( Status )) {
        return Status;
    } else {
        //
        //Get the updated IdentifyData
        //
        GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));

        //
        //Check if UDMA and MWDMA are programmed successfully
        //    
        if ( IdeBusInterface->IdeDevice.UDma != 0xff ) {
            if ( !(ReturnMSBset((IdeBusInterface->IdeDevice.IdentifyData.UDMA_Mode_88 >> 8)) == IdeBusInterface->IdeDevice.UDma)) {
                IdeBusInterface->IdeDevice.UDma = ReturnMSBset((IdeBusInterface->IdeDevice.IdentifyData.UDMA_Mode_88 >> 8));
                if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
                    SupportedModes->UdmaMode.Mode = IdeBusInterface->IdeDevice.UDma;
                    EfiIdeControllerInterface->SetTiming( EfiIdeControllerInterface, IdeBusInterface->IdeDevice.Channel,
                                                             IdeBusInterface->IdeDevice.Device, SupportedModes );
                }
            }
        } else {
            if ( IdeBusInterface->IdeDevice.MWDma != 0xff ) {
                if ( !(ReturnMSBset( IdeBusInterface->IdeDevice.IdentifyData.MultiWord_DMA_63 >> 8 ) == IdeBusInterface->IdeDevice.MWDma)) {
                    IdeBusInterface->IdeDevice.MWDma = ReturnMSBset( (IdeBusInterface->IdeDevice.IdentifyData.MultiWord_DMA_63 >> 8));
                    if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
                        SupportedModes->MultiWordDmaMode.Mode = IdeBusInterface->IdeDevice.MWDma;
                        EfiIdeControllerInterface-> SetTiming( EfiIdeControllerInterface, IdeBusInterface->IdeDevice.Channel,
                                                        IdeBusInterface->IdeDevice.Device, SupportedModes );
                    } 
                }
            }
        }

        //		Check for ATA
        if ( IdeBusInterface->IdeDevice.DeviceType == ATA ) {
            //			Update IDE Read/Write Command
            if  ((IdeBusInterface->IdeDevice.IdentifyData.Valid_Bits_59 & 0x100)
                 && (IdeBusInterface->IdeDevice.IdentifyData.Valid_Bits_59 & 0xff)) {  // Check if Multiple Read/Write
                if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
                    //					48Bit LBA supported
                    IdeBusInterface->IdeDevice.ReadCommand  = READ_MULTIPLE_EXT;
                    IdeBusInterface->IdeDevice.WriteCommand = WRITE_MULTIPLE_EXT;
                } else {
                    IdeBusInterface->IdeDevice.ReadCommand  = READ_MULTIPLE;
                    IdeBusInterface->IdeDevice.WriteCommand = WRITE_MULTIPLE;
                }
            } else {                      // 1 Block = 1 Sector
                if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
                    //					48Bit LBA supported
                    IdeBusInterface->IdeDevice.ReadCommand  = READ_SECTORS_EXT;
                    IdeBusInterface->IdeDevice.WriteCommand = WRITE_SECTORS_EXT;
                } else {
                    IdeBusInterface->IdeDevice.ReadCommand  = READ_SECTORS;
                    IdeBusInterface->IdeDevice.WriteCommand = WRITE_SECTORS;
                }
            }

            if ( DMACapable( IdeBusInterface )) {
                if ( gPlatformIdeProtocol->IdeBusMasterSupport ) {
                    IdeBusInterface->IdeDevice.ReadCommand  = READ_DMA;
                    IdeBusInterface->IdeDevice.WriteCommand = WRITE_DMA;

                    if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
                    //
                    // 48Bit LBA supported
                    //
                        IdeBusInterface->IdeDevice.ReadCommand  = READ_DMA_EXT;
                        IdeBusInterface->IdeDevice.WriteCommand = WRITE_DMA_EXT;
                    }
                }
            }
        }    
        IdeBusInitInterface->IdeBusInitData[Current_Channel][Current_Device] = DEVICE_CONFIGURED_SUCCESSFULLY;
    }


    return EFI_SUCCESS;
}

/**
    Initializes IDE Block IO interface

    @param  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,

    @retval EFI_STATUS

    @note  
    Here is the control flow of this function:
    1. Initialize EFI_BLOCK_IO_PROTOCOL Protocol.
    2. In case of Removable devices, detect Media presence.

**/

EFI_STATUS
InitIdeBlockIO( 
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS              Status;
    EFI_BLOCK_IO_PROTOCOL   *BlkIo;
    IDE_BLOCK_IO            *IdeBlkIo;
    EFI_BLOCK_IO_MEDIA      *BlkMedia;
    ATAPI_DEVICE            *AtapiDevice;
    UINT8                   *Data;
    UINT8                   *InquiryData;
    UINT16                  InquiryDataSize;
    UINT8                   Current_Channel;
    UINT8                   Current_Device;
    UINT8                   bTemp;
    UINT16                  OddType = 0;
    UINT8                   OddLoadingType =0xFF;
    UINT32                  SectorSize = ATA_SECTOR_BYTES;

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(IDE_BLOCK_IO),
                                (VOID**)&IdeBlkIo );

    if ( EFI_ERROR( Status )) {
        return Status;
    }


    BlkMedia = MallocZ( sizeof(EFI_BLOCK_IO_MEDIA));

    if ( !BlkMedia ) {
        pBS->FreePool( IdeBlkIo );
        return EFI_OUT_OF_RESOURCES;
    }

    //
    //Initialize the IdeBlkIo pointer in AMI_IDE_BUS_PROTOCOL (IdeBusInterface)
    //
    IdeBusInterface->IdeBlkIo = IdeBlkIo;

    //
    //Initialize the fields in IdeBlkIo (IDE_BLOCK_IO)
    //    
    IdeBlkIo->IdeBusInterface = IdeBusInterface;

    if ( IdeBusInterface->IdeDevice.DeviceType == ATA ) {
        //    
        //ATA
        //    
        BlkIo              = &(IdeBlkIo->BlkIo);
        //
        // UEFI2.3.1 Supports the BlockIo Revision 3
        //
        if(pST->Hdr.Revision >= 0x0002001F) {
            BlkIo->Revision    = EFI_BLOCK_IO_PROTOCOL_REVISION3;
        } else {
            BlkIo->Revision    = BLKIO_REVISION;
        }
        BlkIo->Media       = BlkMedia;
        BlkIo->Reset       = AtaReset;
        BlkIo->ReadBlocks  = AtaBlkRead;
        BlkIo->WriteBlocks = AtaBlkWrite;
        BlkIo->FlushBlocks = AtaBlkFlush;

        BlkMedia->MediaId        = 0;
        BlkMedia->RemovableMedia = (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x80) == 0x80 ? TRUE : FALSE;

        //Check if Hot plug supported for this device
        Current_Channel = IdeBusInterface->IdeDevice.Channel;
        Current_Device  = IdeBusInterface->IdeDevice.Device;
        bTemp           = Current_Channel == 0 ? 1 : 4;       // BIT MASK

        if ( Current_Device ) {
            bTemp <<= 1;
        }

        if ( bTemp & IdeBusInterface->IdeBusInitInterface->HPMask ) {
            BlkMedia->RemovableMedia = TRUE;
        }

        BlkMedia->MediaPresent     = TRUE;
        BlkMedia->LogicalPartition = FALSE;
        BlkMedia->ReadOnly         = FALSE;
        BlkMedia->WriteCaching     = FALSE;


        if((IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT14) && // WORD 106 valid? - BIT 14 - 1
           (!(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT15)) && // WORD 106 valid? - BIT 15 - 0
           (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & BIT12)) { // WORD 106 bit 12 - Sectorsize > 256 words
            // The sector size is in words 117-118.
            SectorSize = (UINT32)(IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[13] + \
                                  (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[14] << 16)) * 2;
        }

        BlkMedia->BlockSize = SectorSize;

        //If BusMaster Support is Enabled for the device, IO Alignment should be DWORD

        if ( DMACapable( IdeBusInterface )) {
            BlkMedia->IoAlign = 4;
        } else {
            BlkMedia->IoAlign = 0;
        }

        if ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x400 ) {
            BlkMedia->LastBlock = IdeBusInterface->IdeDevice.IdentifyData.LBA_48 - 1;
        } else {
            BlkMedia->LastBlock = IdeBusInterface->IdeDevice.IdentifyData.Addressable_Sector_60 - 1;
        }

        if(pST->Hdr.Revision >= 0x0002001F) {

            BlkMedia->OptimalTransferLengthGranularity=BlkMedia->BlockSize;
            //
            // bit 14 of word 106 is set to one and bit 15 of word 106 is cleared to zero, 
            // then the contents of word 106 contain valid information . 
            // Otherwise, information is not valid in this word.
            //
            if ( (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & 0xC000) == 0x4000 ) {
                //
                // If bit 13 of word 106 is set to one, then the device has more than one 
                // logical sector per physical sector and bits (3:0) of word 106 are valid
                //
                if ( IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & 0x2000 ) {
    
                    BlkMedia->LogicalBlocksPerPhysicalBlock = 1 << 
                                    (IdeBusInterface->IdeDevice.IdentifyData.Reserved_104_126[2] & 0xF);
    
                    //
                    // Bits 13:0 of word 209 indicate the Logical sector offset within the first physical 
                    // sector where the first logical sector is placed
                    //
                    BlkMedia->LowestAlignedLba =IdeBusInterface->IdeDevice.IdentifyData.Reserved_206_254[3] & 0x3FFF;
    
                } else {
                    //
                    // Default set the 1 logical blocks per PhysicalBlock
                    //
                    BlkMedia->LogicalBlocksPerPhysicalBlock=1;
    
                    //
                    // Default value set to 0 for Lowest Aligned LBA
                    //
                    BlkMedia->LowestAlignedLba=0;
                }
            } else {
                    //
                    // Default set the 1 logical blocks per PhysicalBlock
                    //
                    BlkMedia->LogicalBlocksPerPhysicalBlock=1;
                    //
                    // Default value set to 0 for Lowest Aligned LBA
                    //
                    BlkMedia->LowestAlignedLba=0;
            }
        }
    } else {
        //
        //If it is an ATAPI device, check whether it is a CDROM or not. Currently only CDROM/Direct access Devices are supported.
        //
        if ((IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x1f00) == (CDROM_DEVICE << 8)
            || (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x1f00) == (DIRECT_ACCESS_DEVICE << 8)
            || (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x1f00) == (OPTICAL_MEMORY_DEVICE << 8)) {
            AtapiDevice = MallocZ( sizeof (ATAPI_DEVICE));

            if ( !AtapiDevice ) {
                return EFI_OUT_OF_RESOURCES;
            }

            IdeBusInterface->IdeDevice.AtapiDevice = AtapiDevice;

            Status = pBS->AllocatePool( EfiBootServicesData,
                                        16,
                                        (VOID**)&Data
                                        );

            if ( EFI_ERROR( Status )) {
                return Status;
            }

            AtapiDevice->PacketBuffer = Data;
            AtapiDevice->DeviceType   = (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x1f00) >> 8;
            AtapiDevice->PacketSize   = (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 3) == 1 ? 16 : 12;

            BlkIo              = &(IdeBlkIo->BlkIo);
            //
            // UEFI2.3.1 Supports the BlockIo Revision 3
            //
            if(pST->Hdr.Revision >= 0x0002001F) {
                BlkIo->Revision    = EFI_BLOCK_IO_PROTOCOL_REVISION3;
            } else {
                BlkIo->Revision    = BLKIO_REVISION;
            }
            BlkIo->Media       = BlkMedia;
            BlkIo->Reset       = AtapiReset;
            BlkIo->ReadBlocks  = AtapiBlkRead;
            BlkIo->WriteBlocks = AtapiBlkWrite;
            BlkIo->FlushBlocks = AtapiBlkFlush;
            //
            //Initialize with default value. Later on it will be updated if needed.
            //
            BlkMedia->BlockSize = CDROM_BLOCK_SIZE;


            //
            //Update Inquiry Data
            //
            Status = pBS->AllocatePool( EfiBootServicesData,
                                        INQUIRY_DATA_LENGTH,
                                        (VOID**)&InquiryData );

            if ( EFI_ERROR( Status )) {
                return Status;
            }
            InquiryDataSize = INQUIRY_DATA_LENGTH;
            AtapiInquiryData( IdeBusInterface, InquiryData, &InquiryDataSize );
            AtapiDevice->InquiryData     = InquiryData;
            AtapiDevice->InquiryDataSize = InquiryDataSize;

            //
            // Get the ATAPI drive Type information and save it ATAPI_DEVICE
            //
            Status = GetOddType( IdeBusInterface, &OddType );

            if ( !EFI_ERROR( Status )) {
                AtapiDevice->OddType = GetEnumOddType( OddType );
            }

            //
            // Get the ATAPI drive Loading information and save it ATAPI_DEVICE
            //
            Status = GetOddLoadingType( IdeBusInterface, &OddLoadingType );

            if ( !EFI_ERROR( Status )) {
                AtapiDevice->OddLoadingType = (ODD_LOADING_TYPE)OddLoadingType;
            } else {
                AtapiDevice->OddLoadingType = (ODD_LOADING_TYPE)0xFF;
            }

            BlkMedia->RemovableMedia   = (IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 0x80) == 0x80 ? TRUE : FALSE;
            BlkMedia->LogicalPartition = FALSE;
            BlkMedia->WriteCaching     = FALSE;

            //
            //If BusMaster Support is Enabled for the device, IO Alignment should be DWORD
            //
            if ( DMACapable( IdeBusInterface )) {
                BlkMedia->IoAlign = 4;
            } else {
                BlkMedia->IoAlign = 0;
            }
        }
    }
    return EFI_SUCCESS;
}

/**
    Get the Enum value for ODD type found on profile

    @param  UINT16      Oddtype

    @retval DD_TYPE    EnumValue

**/
ODD_TYPE
GetEnumOddType (
    UINT16  OddType
)
{
    switch ( OddType )
    {
    case 1:
    case 3:
    case 4:
    case 5:
        return Obsolete;

    case 2:
        return Removabledisk;

    case 8:
        return CDROM;

    case 9:
        return CDR;

    case 0xa:
        return CDRW;

    case 0x10:
        return DVDROM;

    case 0x11:
        return DVDRSequentialrecording;

    case 0x12:
        return DVDRAM;

    case 0x13:
        return DVDRWRestrictedOverwrite;

    case 0x15:
        return DVDRWSequentialrecording;

    case 0x16:
        return DVDRDualLayerJumprecording;

    case 0x17:
        return DVDRWDualLayer;

    case 0x18:
        return DVDDownloaddiscrecording;

    case 0x1a:
        return DVDRW;

    case 0x1b:
        return DVDR;

    case 0x40:
        return BDROM;

    case 0x41:
        return BDRSequentialRecording;

    case 0x42:
        return BDRRandomRecordingMode;

    case 0x43:
        return BDRE;

    case 0x50:
        return HDDVDROM;

    case 0x51:
        return HDDVDR;

    case 0x52:
        return HDDVDRAM;

    case 0x53:
        return HDDVDRW;

    case 0x58:
        return HDDVDRDualLayer;

    case 0x5a:
        return HDDVDRWDualLayer;

    default:
        return UnknownType;
    }
}

/**
    Initializes IDE DiskInfo Interface

    @param  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface,

    @retval EFI_STATUS

**/

EFI_STATUS
InitIdeDiskInfo (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_STATUS      Status;
    IDE_DISK_INFO   *IdeDiskInfo;

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof(IDE_DISK_INFO),
                                (VOID**)&IdeDiskInfo );

    if ( EFI_ERROR( Status )) {
        return Status;
    }

    //
    //Initialize the IdeBlkIo pointer in AMI_IDE_BUS_PROTOCOL (IdeBusInterface)
    //
    IdeBusInterface->IdeDiskInfo = IdeDiskInfo;

    //
    //Initialize the fields in IdeDiskInfo (IDE_DISK_INFO)
    //
    IdeDiskInfo->IdeBusInterface = IdeBusInterface;

    pBS->CopyMem( &(IdeDiskInfo->DiskInfo.Interface), &gEfiDiskInfoIdeInterfaceGuid, sizeof (EFI_GUID));
    IdeDiskInfo->DiskInfo.Inquiry   = DiskInfoInquiry;
    IdeDiskInfo->DiskInfo.Identify  = DiskInfoIdentify;
    IdeDiskInfo->DiskInfo.SenseData = DiskInfoSenseData;
    IdeDiskInfo->DiskInfo.WhichIde  = DiskInfoWhichIDE;
    return EFI_SUCCESS;
}

/**
    Creates a IDE device devicepath and adds it to IdeBusInterface

        
    @param This 
    @param Controller 
    @param AMI_IDE_BUS_INIT_PROTOCO *IdeBusInitInterface;
    @param RemainingDevicePath 
    @param UINT8    Current_Channel
    @param UINT8    Current_Device

    @retval EFI_STATUS

    @note  
    Here is the control flow of this function:
    1.  If Remaining Devicepath is not NULL, we have already verified that it is a
        valid Atapi device path in IdeBusStart. So nothing to do. Just exit.
	2.	Build a Atapi devicepath and a End device path.
	3.  Get the Devicepath for the IDE controller.
	3.  Append Atapi devicepath to  IDE controller devicepath.

**/
EFI_STATUS
CreateIdeDevicePath (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  EFI_HANDLE                      Controller,
    AMI_IDE_BUS_INIT_PROTOCOL           *IdeBusInitInterface,
    AMI_IDE_BUS_PROTOCOL                *IdeBusInterface,
    IN  OUT EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath,
    IN  UINT8                           Current_Channel,
    IN  UINT8                           Current_Device
)
{
    EFI_STATUS               Status;
    ATAPI_DEVICE_PATH        NewDevicePath;
    EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;

    NewDevicePath.Header.Type    = MESSAGING_DEVICE_PATH;
    NewDevicePath.Header.SubType = MSG_ATAPI_DP;
    SET_NODE_LENGTH( &NewDevicePath.Header, ATAPI_DEVICE_PATH_LENGTH );
    NewDevicePath.PrimarySecondary = Current_Channel;
    NewDevicePath.SlaveMaster      = Current_Device;
    NewDevicePath.Lun              = 0;

    //
    //Append the Device Path
    //
    Status = pBS->OpenProtocol( Controller,
                                &gEfiDevicePathProtocolGuid,
                                (VOID**)&TempDevicePath,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    ASSERT_EFI_ERROR(Status);

    IdeBusInterface->DevicePathProtocol = DPAddNode( TempDevicePath, &NewDevicePath.Header );
    return EFI_SUCCESS;
}

/**
    Generates the ComReset to Port 

    @param AMI_IDE_BUS_PROTOCOL *IdeBusInterface,

    @retval EFI_STATUS

**/
EFI_STATUS
GeneratePortReset (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT16      SIDPBA=0;
    EFI_PCI_IO_PROTOCOL              *PciIO=IdeBusInterface->PciIO;
    UINT32      Data32=0;
    UINT8       PortIndex=(IdeBusInterface->IdeDevice.Channel << 1) | IdeBusInterface->IdeDevice.Device;
    EFI_STATUS  Status;
    UINT8       IOSpace;
    UINT64      PortOffset;
    
    Status = PciIO->Pci.Read( PciIO,
                              EfiPciIoWidthUint16,
                              PCI_SIDPBA,
                              sizeof (UINT16),
                              &SIDPBA
                              );

    if(EFI_ERROR(Status)) {
        return EFI_NOT_FOUND;
    }
 
    if(SIDPBA == 0 || SIDPBA == 0xFFFF) {
        return EFI_NOT_FOUND;
    }

    // ABAR/SIDPBA Bit0 is Resource Type Indicator (RTE). 1 - IO Space 0 - MMIO
    IOSpace = SIDPBA & BIT00;
    
    if( IOSpace ) {
       
        SIDPBA &=0xFFFE;

        Data32= (UINT32)((PortIndex << 8) | 01 );

        PciIO->Io.Write(
                PciIO,
                EfiPciIoWidthFifoUint32,
                EFI_PCI_IO_PASS_THROUGH_BAR,
                SIDPBA,
                1,
                &Data32 );

        //Read the Serial ATA Control Register
        Status = PciIO->Io.Read(
                PciIO,
                EfiPciIoWidthFifoUint32,
                EFI_PCI_IO_PASS_THROUGH_BAR,
                SIDPBA+4,
                1,
                &Data32 );

        if(EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }
        
        // Perform interface communication initialization sequence to establish
        // communication. This is functionally equivalent to a hard reset and
        // results in the interface being reset and communications reinitialized
        Data32 |= 01;

        PciIO->Io.Write(
                PciIO,
                EfiPciIoWidthFifoUint32,
                EFI_PCI_IO_PASS_THROUGH_BAR,
                SIDPBA+4,
                1,
                &Data32 );

        //1ms Delay
        pBS->Stall (1000);

        // Reset the COMRESET bit
        Data32 &=0xFFFFFFFE;

        PciIO->Io.Write(
                PciIO,
                EfiPciIoWidthFifoUint32,
                EFI_PCI_IO_PASS_THROUGH_BAR,
                SIDPBA+4,
                1,
                &Data32 );
    } else {
        
        PortOffset = (UINT64)( PortIndex << 7) + HBA_PORTS_START; //Port offset = PortIndex * 80h + 100h

        //Read the Port Serial ATA Control Register
        Status = PciIO->Mem.Read(
                    PciIO,
                    EfiPciIoWidthFillUint32,
                    5,
                    PortOffset + HBA_PORTS_SCTL,
                    1,
                    &Data32 );
        if(EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }

        // Perform interface communication initialization sequence to establish
        // communication. This is functionally equivalent to a hard reset and
        // results in the interface being reset and communications reinitialized
        Data32 |= 01;

        PciIO->Mem.Write(
                    PciIO,
                    EfiPciIoWidthFillUint32,
                    5,
                    PortOffset+ HBA_PORTS_SCTL,
                    1,
                    &Data32 );

        //1ms Delay
        pBS->Stall (1000);

        // Reset the COMRESET bit
        Data32 &=0xFFFFFFFE;

        PciIO->Mem.Write(
                    PciIO,
                    EfiPciIoWidthFillUint32,
                    5,
                    PortOffset+ HBA_PORTS_SCTL,
                    1,
                    &Data32 );
        
    }

    return EFI_SUCCESS;

}

/**
    Issues SET FEATURE Command

    @param  AMI_IDE_BUS_PROTOCOL			*IdeBusInterface,
    @param  AMI_IDE_BUS_INIT_PROTOCOL		*IdeBusInitInterface

    @retval EFI_STATUS

    @note  
      Here is the control flow of this function:
      1. Issue Set feature commend to set PIO mode if needed.
      2. Set Multiple Mode command for ATA devices if needed.
      3. Issue Set feature commend to set UDMA/MWDMA. If it fails, disable BusMaster support.

**/

EFI_STATUS
ConfigureIdeDevice (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface,
    IN  AMI_IDE_BUS_INIT_PROTOCOL   *IdeBusInitInterface
)
{
    EFI_STATUS               Status= EFI_SUCCESS;
    UINT8                    DMACapability;
    UINT8                    Data8;
    IO_REGS                  Regs = IdeBusInterface->IdeDevice.Regs;
    UINT16                   DeviceName[41];
    UINT16                   Index;

    EFI_UNICODE_STRING_TABLE *tempUnicodeTable;
    CHAR8                    Language[] = "Eng";

    if ( !gPlatformIdeProtocol->DisableSoftSetPrev && gPlatformIdeProtocol->ForceHddPasswordPrompt ) {

        // Software settings preserved
        if ((IdeBusInterface->IdeDevice.IdentifyData.Reserved_76_79[0] != 0xFFFF)
            && (IdeBusInterface->IdeDevice.IdentifyData.Reserved_76_79[2] & 0x0040)) {

            // Disable the Software Preservation
            IdeSetFeatureCommand( IdeBusInterface, DISABLE_SATA2_SOFTPREV, 6 );

            //GeneratePort Reset
            GeneratePortReset(IdeBusInterface);

            // Enable the Software Preservation
            IdeSetFeatureCommand( IdeBusInterface, 0x10, 6 );

            // Get the updated IdentifyData
            GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));
        }
    }


    //
    //Check if Device need spin-up
    //
    if ((IdeBusInterface->IdeDevice.IdentifyData.General_Config_0 & 4)
        && (IdeBusInterface->IdeDevice.IdentifyData.Special_Config_2 == SPIN_UP_REQUIRED1
            || IdeBusInterface->IdeDevice.IdentifyData.Special_Config_2 == SPIN_UP_REQUIRED2)) {
        Status = IdeSetFeatureCommand( IdeBusInterface, SET_DEVICE_SPINUP, 0 );

        if ( EFI_ERROR( Status )) {    
            //
            //Some HDD may take a long time to spin up. Wait for additional time
            //
            Status = WaitforBitClear( IdeBusInterface->PciIO, 
                                    Regs.ControlBlock.AlternateStatusReg, 
                                    IDE_BSY | IDE_DRQ, 
                                    gPlatformIdeProtocol->PoweonBusyClearTimeout );
        }
        //
        //Get the Identify Command once more
        //
        GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));
    }

    //
    //	Check if PIO mode needs to be programmed
    //    
    if ((IdeBusInterface->IdeDevice.IdentifyData.Valid_Bits_53 & 0x2) && (IdeBusInterface->IdeDevice.PIOMode != 0xff)) {
        Status = IdeSetFeatureCommand( IdeBusInterface, SET_TRANSFER_MODE, PIO_FLOW_CONTROL | IdeBusInterface->IdeDevice.PIOMode );
    }

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Issue Set Multiple Mode Command only for ATA device
    //
    if  ( IdeBusInterface->IdeDevice.DeviceType == ATA ) {
        Data8 = IdeBusInterface->IdeDevice.IdentifyData.Maximum_Sector_Multiple_Command_47 & 0xff;

        if ( Data8 & 0x2 ) {
            Data8 = 2;
        }

        if ( Data8 & 0x4 ) {
            Data8 = 0x4;
        }

        if ( Data8 & 0x8 ) {
            Data8 = 0x8;
        }

        if ( Data8 & 0x10 ) {
            Data8 = 0x10;
        }

        if ( Data8 & 0x20 ) {
            Data8 = 0x20;
        }

        if ( Data8 & 0x40 ) {
            Data8 = 0x40;
        }

        if ( Data8 & 0x80 ) {
            Data8 = 0x80;
        }

        if ( Data8 > 1 ) {
            Status = IdeNonDataCommand(
                IdeBusInterface,
                0,              // Features
                Data8,          // Sector Count
                0,              // LBA Low
                0,              // LBA Mid
                0,              // LBA High
                IdeBusInterface->IdeDevice.Device << 4, // Device
                SET_MULTIPLE_MODE );                     // Command
        }
    }

    //Check if BusMaster Enabled
    //Status = EFI_NOT_FOUND;
    //Check if  UDMA is supported
    if  ( IdeBusInterface->IdeDevice.UDma != 0xff ) {
        Status =     IdeSetFeatureCommand( IdeBusInterface, SET_TRANSFER_MODE, UDMA_MODE | IdeBusInterface->IdeDevice.UDma );
    } else {
        if ( IdeBusInterface->IdeDevice.MWDma != 0xff ) {
            Status = IdeSetFeatureCommand( IdeBusInterface, SET_TRANSFER_MODE, MWDMA_MODE | IdeBusInterface->IdeDevice.MWDma );
        }
    }

    if ( EFI_ERROR( Status )) {
        //
        //No DMA support
        //
        IdeBusInterface->IdeDevice.UDma  = 0xff;
        IdeBusInterface->IdeDevice.MWDma = 0xff;
    } else {
        //
        //Enable DMA capable bit
        //
        DMACapability = 0x20;

        if ( IdeBusInterface->IdeDevice.Device ) {
            DMACapability = 0x40;
        }

        if ( DMACapable( IdeBusInterface )) {
            IdeReadByte( IdeBusInterface->PciIO,
                         IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                         &Data8 );
            Data8 |= DMACapability;
            IdeWriteByte( IdeBusInterface->PciIO,
                          IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister,
                          Data8 );
        }
    }

    //
    //Convert the Device string from English to Unicode
    //
    IdeBusInterface->IdeDevice.UDeviceName = NULL;
    if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
        IdeBusInterface->ControllerNameTable = IdeBusInterface->IdeDevice.UDeviceName;
    } 

    for ( Index = 0; Index < 40; Index += 2 ) {

            DeviceName[Index]     = ((UINT8*)IdeBusInterface->IdeDevice.IdentifyData.Model_Number_27)[Index + 1];
            DeviceName[Index + 1] = ((UINT8*)IdeBusInterface->IdeDevice.IdentifyData.Model_Number_27)[Index];
    }

    for ( Index = 39; Index > 0; Index-- ) {
        if ( DeviceName[Index] == 0x20 )
            continue;
        else 
            break;
    }
	// Terminate string
    if(DeviceName[Index] == 0x20) {
        DeviceName[Index] = 0;       
    } else {
        DeviceName[Index + 1] = 0;    
    }
    Index += 2;

    //
    //This is a patch to make it work for both in Aptio and Alaska
    //
    tempUnicodeTable = MallocZ( sizeof (EFI_UNICODE_STRING_TABLE) * 2 );

    Status = pBS->AllocatePool( EfiBootServicesData,
                                sizeof (Language),
                                (VOID**)&tempUnicodeTable[0].Language
                                );																
    ASSERT_EFI_ERROR(Status);
    Status = pBS->AllocatePool( EfiBootServicesData,
                                Index * (sizeof (UINT16)),
                                (VOID**)&tempUnicodeTable[0].UnicodeString
                                );

    ASSERT_EFI_ERROR(Status);
    pBS->CopyMem( tempUnicodeTable[0].Language, &Language,  sizeof(Language));
    pBS->CopyMem( tempUnicodeTable[0].UnicodeString, DeviceName, Index * (sizeof (UINT16)));
    tempUnicodeTable[1].Language           = NULL;
    tempUnicodeTable[1].UnicodeString      = NULL;
    IdeBusInterface->IdeDevice.UDeviceName = tempUnicodeTable;
    if ( gPlatformIdeProtocol->EfiIdeProtocol ) {
        IdeBusInterface->ControllerNameTable = IdeBusInterface->IdeDevice.UDeviceName;
    } 
    if ( gPlatformIdeProtocol->SBIdeSupport ) {
        InitMiscConfig(IdeBusInterface);
    }

    if ( gPlatformIdeProtocol->IdePowerManagementSupport ) {
        Status=InitIDEPowerManagement( IdeBusInterface );
        ASSERT_EFI_ERROR(Status);
    }

    if ( gPlatformIdeProtocol->DisableSoftSetPrev ) {

        // Software settings preserved
        if ((IdeBusInterface->IdeDevice.IdentifyData.Reserved_76_79[0] != 0xFFFF)
            && (IdeBusInterface->IdeDevice.IdentifyData.Reserved_76_79[2] & 0x0040)) {

            // Disable the Software Preservation
            IdeSetFeatureCommand( IdeBusInterface, DISABLE_SATA2_SOFTPREV, 6 );

            // Get the updated IdentifyData
            GetIdentifyData( IdeBusInterface, &(IdeBusInterface->IdeDevice));
        }
    }


    return EFI_SUCCESS;
}

/**
    Initialize misc IDE configurations.

    @param SataDevInterface 

    @retval VOID

**/

VOID
InitMiscConfig (
    IN  AMI_IDE_BUS_PROTOCOL        *IdeBusInterface
)
{
    if ( gPlatformIdeProtocol->DiPMSupport ) {

        if(IdeBusInterface->IdeDevice.IdentifyData.Reserved_76_79[2] & \
                IDENTIFY_DIPM_SUPPORT) { // DiPM supported?
            //
            // Always disable DiPM in IDE mode
            //
            IdeSetFeatureCommand (IdeBusInterface, DIPM_DISABLE, DIPM_SUB_COMMAND);
        }
    }
    if ( gPlatformIdeProtocol->AcousticManagementSupport ) {
        InitAcousticSupport (IdeBusInterface);
    }

}

/**
    Initializes Acoustic Management Support functionality

    @param AMI_IDE_BUS_PROTOCOL			*IdeBusInterface,

    @retval EFI_STATUS

    @note  
     1. Check if the device support Acoustic management.
     2. Check the desired state Vs the current state.
     3. If both are equal nothing to do exit else program the desired level

**/
EFI_STATUS
InitAcousticSupport (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8 Data8;

    //
    //	Check if the device supports Acoustic Management
    //
    if  ( IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Supported_83 & 0x200 ) {
        //
        //Check if Acoustic Level need to be enabled or Disabled
        //
        if ( IdeBusInterface->IdeBusInitInterface->Flags & ACOUSTIC_SUPPORT_ENABLE ) {
            Data8 = IdeBusInterface->IdeBusInitInterface->Acoustic_Management_Level;

            //
            // Do we need to program the recommended value
            //
            if ( Data8 == ACOUSTIC_LEVEL_BYPASS ) {
                //
                // Get the recommended value
                //
                Data8 = (UINT8)(IdeBusInterface->IdeDevice.IdentifyData.Acoustic_Level_94 >> 8);
            }

                IdeSetFeatureCommand (IdeBusInterface, ACOUSTIC_MANAGEMENT_ENABLE, Data8);
        } else {
                //
                // If already disabled, nothing to do
                //
            if	(IdeBusInterface->IdeDevice.IdentifyData.Command_Set_Enabled_86 & 0x200) 			
                IdeSetFeatureCommand (IdeBusInterface, ACOUSTIC_MANAGEMENT_DISABLE, 0);
        }
    }
    return EFI_SUCCESS;
}


/**
    Updates Command and Control register address.

    @param  AMI_IDE_BUS_PROTOCOL			*IdeBusInterface;

    @retval EFI_STATUS

    @note  
  Here is the control flow of this function:
  1. Using PCI_IO_PROTOCOL, update the Command, control and BusMaster register address.
    Make use of Channel number while updating. Also check whether controller is running in Legacy/Native mode

**/

EFI_STATUS
UpdateBaseAddress (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_PCI_IO_PROTOCOL *PciIO;
    UINT8               PciConfig[0x40];
    UINT16              CommandReg;
    UINT16              ControlReg;
    UINT16              BusMasterReg;


    PciIO = IdeBusInterface->PciIO;
    PciIO->Pci.Read(
        PciIO,
        EfiPciIoWidthUint8,
        0,
        sizeof (PciConfig),
        PciConfig );

    if ((PciConfig [PROGRAMMING_INTERFACE_OFFSET] & (IdeBusInterface->IdeDevice.Channel == 0 ? 1 : 4))
        || (PciConfig [IDE_SUB_CLASS_CODE] == SCC_AHCI_CONTROLLER)
        || (PciConfig [IDE_SUB_CLASS_CODE] == SCC_RAID_CONTROLLER)) {
        //   
        //Native Mode Secondary
        //    
        if ( IdeBusInterface->IdeDevice.Channel ) {
            CommandReg   =    (*(UINT16*)(PciConfig + SECONDARY_COMMAND_BLOCK_OFFSET)) & 0xfffe;
            ControlReg   =   ((*(UINT16*)(PciConfig + SECONDARY_CONTROL_BLOCK_OFFSET)) & 0xfffe) + 2;
            BusMasterReg = ((*(UINT16*)(PciConfig + LEGACY_BUS_MASTER_OFFSET)) & 0xfffe) + 8;
        } else {  
            //
            // Native mode Primary
            //   
            CommandReg   =   (*(UINT16*)(PciConfig + PRIMARY_COMMAND_BLOCK_OFFSET)) & 0xfffe;
            ControlReg   =  ((*(UINT16*)(PciConfig + PRIMARY_CONTROL_BLOCK_OFFSET)) & 0xfffe) + 2;
            BusMasterReg = (*(UINT16*)(PciConfig + LEGACY_BUS_MASTER_OFFSET)) & 0xfffe;
        }
    } else {
        //
        //Legacy Mode Secondary
        //
        if ( IdeBusInterface->IdeDevice.Channel ) {
            CommandReg   = SECONDARY_COMMAND_BLOCK;
            ControlReg   = SECONDARY_CONTROL_BLOCK;
            BusMasterReg = (*(UINT16*)(PciConfig + LEGACY_BUS_MASTER_OFFSET) & 0xfffe) + 8;
        } else {      
            //
            //Legacy mode Primary
            //
            CommandReg   = PRIMARY_COMMAND_BLOCK;
            ControlReg   = PRIMARY_CONTROL_BLOCK;
            BusMasterReg = (*(UINT16*)(PciConfig + LEGACY_BUS_MASTER_OFFSET) & 0xfffe);
        }
    }

    //
    //	Validate the Address
    //
    if ( CommandReg == 0 || ControlReg == 0 ) {
        return EFI_DEVICE_ERROR;
    }

    if ( CommandReg == 0xfffe || ControlReg == 0xfffe ) {
        return EFI_DEVICE_ERROR;
    }

    //
    //Update the Command block registers
    //
    IdeBusInterface->IdeDevice.Regs.CommandBlock.DataReg        = CommandReg;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.FeatureReg     = CommandReg + 1;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.ErrorReg       = CommandReg + 1;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.SectorCountReg = CommandReg + 2;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.LBALowReg      = CommandReg + 3;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAMidReg      = CommandReg + 4;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.LBAHighReg     = CommandReg + 5;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.DeviceReg      = CommandReg + 6;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.CommandReg     = CommandReg + 7;
    IdeBusInterface->IdeDevice.Regs.CommandBlock.StatusReg      = CommandReg + 7;

    //
    //Update the Control Block Register address
    //
    IdeBusInterface->IdeDevice.Regs.ControlBlock.DeviceControlReg   = ControlReg;
    IdeBusInterface->IdeDevice.Regs.ControlBlock.AlternateStatusReg = ControlReg;


    //
    //Update the Bus Master register address
    //
    IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMCommandRegister           = BusMasterReg;
    IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMStatusRegister            = BusMasterReg + 2;
    IdeBusInterface->IdeDevice.Regs.BusMasterBlock.BMDescriptorTablePointerReg = BusMasterReg + 4;

    return EFI_SUCCESS;
}

/**
    Check if DMA is supported

    @param IdeBusInterface 

    @retval TRUE : DMA Capable

**/
BOOLEAN
DMACapable (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    if ( IdeBusInterface->IdeDevice.DeviceType == ATAPI ) {
        //
        // For Atapi Devices check Bit 8 in Word 49 =  DMA	Supported or not
        //
        if ((IdeBusInterface->IdeDevice.IdentifyData.Capabilities_49 & 0x100) == 0 ) {
            return FALSE;
        }
    }

    if ((IdeBusInterface->IdeDevice.UDma != 0xff)
        || (IdeBusInterface->IdeDevice.MWDma != 0xff)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**

    @param This 
    @param InquiryData 
    @param InquiryDataSize 

    @retval EFI_STATUS

    @note  
    1. Check for Atapi Device. If not exit
    2. COpy the Inquiry Data from AtapiDevice->InquiryData to the input pointer.

**/

EFI_STATUS
DiskInfoInquiry (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID                *InquiryData,
    IN  OUT UINT32              *InquiryDataSize
)
{
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;
    ATAPI_DEVICE         *AtapiDevice     = IdeBusInterface->IdeDevice.AtapiDevice;


    //
    //	Check for ATAPI device. If not return EFI_NOT_FOUND
    //
    if ( IdeBusInterface->IdeDevice.DeviceType == ATA ) {
        return EFI_NOT_FOUND;               
    } else {
        if ( *InquiryDataSize < AtapiDevice->InquiryDataSize ) {
            *InquiryDataSize = AtapiDevice->InquiryDataSize;
            return EFI_BUFFER_TOO_SMALL;
        }

        if ( AtapiDevice->InquiryData != NULL ) {
            pBS->CopyMem( InquiryData, AtapiDevice->InquiryData, AtapiDevice->InquiryDataSize );
            *InquiryDataSize = AtapiDevice->InquiryDataSize;
            return EFI_SUCCESS;
        } else {
            return EFI_NOT_FOUND;
        }
    }
}

/**
    Return Identify Data

        
    @param EFI_DISK_INFO_PROTOCOL			*This,
    @param IdentifyData 
    @param IdentifyDataSize 

    @retval EFI_STATUS

    @note  
	1. Return the Identify command data.

**/
EFI_STATUS
DiskInfoIdentify (
    EFI_DISK_INFO_PROTOCOL  *This,
    IN  OUT VOID            *IdentifyData,
    IN  OUT UINT32          *IdentifyDataSize
)
{
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;

    if ( *IdentifyDataSize < sizeof (IDENTIFY_DATA)) {
        *IdentifyDataSize = sizeof (IDENTIFY_DATA);
        return EFI_BUFFER_TOO_SMALL;
    }

    //
    // ATA devices identify data might be changed because of the SetFeature command, 
    // So read the data from the device again by sending identify command.
    //
    if ( IdeBusInterface->IdeDevice.DeviceType == ATA ) {
        GetIdentifyData(IdeBusInterface, &(IdeBusInterface->IdeDevice.IdentifyData));
    }

    pBS->CopyMem( IdentifyData, &(IdeBusInterface->IdeDevice.IdentifyData), sizeof (IDENTIFY_DATA));
    *IdentifyDataSize =  sizeof (IDENTIFY_DATA);
    return EFI_SUCCESS;
}

/**
    Return InfoSenseData.

    @param  EFI_DISK_INFO_PROTOCOL  *This,
    @param  VOID                    *SenseData,
    @param  UINT32                  *SenseDataSize,
    @param  UINT8                   *SenseDataNumber

    @retval EFI_STATUS

    @note  
    1. Return the Sense data for the Atapi device.

**/
EFI_STATUS
DiskInfoSenseData (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    OUT VOID                    *SenseData,
    OUT UINT32                  *SenseDataSize,
    OUT UINT8                   *SenseDataNumber
)
{
    return EFI_NOT_FOUND;
}

/**
    Returns whether the device is PM/PS/SM/SS

    @param This 
    @param IdeChannel 
    @param IdeDevice 

    @retval EFI_STATUS

    @note  
    1. Return information about the Primary/Secondary channel and Master/Slave information.

**/

EFI_STATUS
DiskInfoWhichIDE (
    IN  EFI_DISK_INFO_PROTOCOL  *This,
    OUT UINT32                  *IdeChannel,
    OUT UINT32                  *IdeDevice
)
{
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface = ((IDE_BLOCK_IO*)This)->IdeBusInterface;

    *IdeChannel = IdeBusInterface->IdeDevice.Channel;
    *IdeDevice  = IdeBusInterface->IdeDevice.Device;
    return EFI_SUCCESS;
}

/**
    A quick check to see if ports are still decoded.

    @param IdeBusInterface 

    @retval 
        EFI_STATUS

**/
EFI_STATUS
CheckHPControllerPresence (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    UINT8   Device = IdeBusInterface->IdeDevice.Device;
    IO_REGS Regs   = IdeBusInterface->IdeDevice.Regs;
    UINT8   Data8;

    //
    //Select the drive
    //
    IdeWriteByte( IdeBusInterface->PciIO, Regs.CommandBlock.DeviceReg, Device << 4 );

    //
    //Read the status Register
    //
    IdeReadByte( IdeBusInterface->PciIO, Regs.CommandBlock.StatusReg, &Data8 );

    if ( Data8 == 0xff ) {
        return EFI_NOT_FOUND;
    }

    if ( Data8 == 0x7f ) {
        return EFI_NOT_FOUND;
    }

    return EFI_SUCCESS;
}

#if IDEBUS_DEBUG_PRINT == 1
/**
    Prints Debug Level 3 Trace Messages

        
    @param IdeBusInterface 

    @retval 
        EFI_STATUS

**/
void PrintIdeDeviceInfo(
    AMI_IDE_BUS_PROTOCOL *IdeBusInterface )
{
    UINT16 Data8;
    UINT16 *pIdentifyData;

    //		Print the Channel and Device Number
    TRACE(( -1, "-----------------IDE Device Info Start-----------------\n" ));
    TRACE(( -1, "Channel   : %x   Device    : %x\n", IdeBusInterface->IdeDevice.Channel, IdeBusInterface->IdeDevice.Device ));
    TRACE(( -1, "PIOMode   : %x   UDMAMode  : %x\n", IdeBusInterface->IdeDevice.PIOMode, IdeBusInterface->IdeDevice.UDma ));
    TRACE(( -1, "SWDMAMode : %x   MWDMAMode : %x\n", IdeBusInterface->IdeDevice.SWDma,   IdeBusInterface->IdeDevice.MWDma ));
    TRACE(( -1, "IORDY		: %x   \n",                  IdeBusInterface->IdeDevice.IORdy ));
    pIdentifyData = (UINT16*) &(IdeBusInterface->IdeDevice.IdentifyData);

    for ( Data8 = 0; Data8 < 0xff; Data8 += 4 )
    {
        TRACE(( -1, "%X %X %X %X\n", pIdentifyData[Data8 + 0], pIdentifyData[Data8 + 1], pIdentifyData[Data8 + 2], pIdentifyData[Data8 + 3] ));
    }
        TRACE(( -1, "-----------------IDE Device Info End-------------------\n" ));
}

#endif          /* IDEBUS_DEBUG_PRINT == 1 */


/**
    Checks If the controller is in AHCI MODE.

    @param  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface;
    @param  BOOLEAN                 TRUE  - AHCI Mode

    @retval FALSE IDE Mode

**/
BOOLEAN
CheckAhciMode (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{
    EFI_PCI_IO_PROTOCOL *PciIO;
    UINT8               PciConfig[0x40];
    BOOLEAN             AhciFlag ;

    PciIO = IdeBusInterface->PciIO;
    PciIO->Pci.Read(
                    PciIO,
                    EfiPciIoWidthUint8,
                    0,
                    sizeof (PciConfig),
                    PciConfig );

    AhciFlag = (BOOLEAN)((PciConfig [IDE_SUB_CLASS_CODE] & SCC_AHCI_CONTROLLER) ? TRUE : FALSE );

    return AhciFlag;
}

/**
    Set SATA port under IDE mode.

    @param  AMI_IDE_BUS_PROTOCOL            *IdeBusInterface;

    @retval VOID

    @note  
      1. Get the Serial ATA Index Data Pair Base Address.
      2. Select the Port and read the current interface speed.
      3. Check Device presence and Physical communication established.
      4. If link established, compare Current Interface Speed and Speed Allowed.
      5. If CIS is greater than SA generate a COMREST.
**/
VOID
SetIdePortSpeed (
    IN  AMI_IDE_BUS_PROTOCOL    *IdeBusInterface
)
{

    UINT32                  CurrentSpeed;
    UINT32                  MaxDevSpeed;
    UINT32                  DeviceDet;
    UINT16                  SIDPBA=0;
    UINT32                  Data32=0;
    UINT8                   PortIndex;
    EFI_PCI_IO_PROTOCOL     *PciIO=IdeBusInterface->PciIO;

    PciIO->Pci.Read( 
                        PciIO,
                        EfiPciIoWidthUint16,
                        PCI_SIDPBA,
                        sizeof (UINT16),
                        &SIDPBA );

    SIDPBA &=0xFFFE;

    if (SIDPBA == 0 || SIDPBA == 0xFFFE) {
        return;
    }

    PortIndex = (IdeBusInterface->IdeDevice.Channel << 1) | IdeBusInterface->IdeDevice.Device;
    Data32= (UINT32)((PortIndex << 8) | 00 );
    PciIO->Io.Write(
                       PciIO,
                       EfiPciIoWidthFifoUint32,
                       EFI_PCI_IO_PASS_THROUGH_BAR,
                       SIDPBA,
                       1,
                       &Data32 );

    //Read the Serial ATA Status Register
    PciIO->Io.Read ( 
                       PciIO,
                       EfiPciIoWidthUint16,
                       EFI_PCI_IO_PASS_THROUGH_BAR,
                       SIDPBA+4,
                       1,
                       &CurrentSpeed );

    DeviceDet = CurrentSpeed & 0x0F;
    CurrentSpeed = (CurrentSpeed & 0xF0) >> 4;

    if (DeviceDet == SSTS_DET_PCE ) {
        Data32= (UINT32)((PortIndex << 8) | 01 );
         PciIO->Io.Write(
                           PciIO,
                           EfiPciIoWidthFifoUint32,
                           EFI_PCI_IO_PASS_THROUGH_BAR,
                           SIDPBA,
                           1,
                           &Data32 );

        // As the Link is already established, get the negotiated interface
        // communication speed
        PciIO->Io.Read ( 
                           PciIO,
                           EfiPciIoWidthUint8,
                           EFI_PCI_IO_PASS_THROUGH_BAR,
                           SIDPBA+4,
                           1,
                           &Data32 );

        MaxDevSpeed =  (Data32 & 0xF0) >> 4;
        if ( (CurrentSpeed > MaxDevSpeed) && (MaxDevSpeed != 0)) {
            //GeneratePort Reset if CurrentSpeed is greater than MaxDevSpeed 
            GeneratePortReset(IdeBusInterface);
        }
    }
    return;
}

//***********************************************************************
//***********************************************************************
//**                                                                   **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.          **
//**                                                                   **
//**                       All Rights Reserved.                        **
//**                                                                   **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093         **
//**                                                                   **
//**                       Phone: (770)-246-8600                       **
//**                                                                   **
//***********************************************************************
//***********************************************************************

