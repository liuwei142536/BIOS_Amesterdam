//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiSerial.c
    Driver that will install AMI_SERIAL_PROTOCOL for serial   
    device, the supported/Start/Stop function need to be
    ported for a non-generic serial device.

**/

//---------------------------------------------------------------------------
#include <AmiDxeLib.h>
#include <Token.h>
#include <GenericSio.h>
#include <Pci.h>
#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <AcpiRes.h>
#include <Protocol/TerminalAmiSerial.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
typedef struct _AMI_SERIAL_PARAMETERS{
    AMI_SERIAL_PROTOCOL AmiSerialProtocol;
    UINT64              BaseAddress;
    BOOLEAN             Pci;
    BOOLEAN             Mmio;
    UINT8               MmioWidth;
    UINTN               Bus;
    UINTN               Dev;
    UINTN               Func;
    UINT8               Port;
    EFI_HANDLE          Handle;
    DLINK               SerialDeviceLink;
    UINT8               SerialIRQ;
} AMI_SERIAL_PARAMETERS;

//---------------------------------------------------------------------------

VOID
EFIAPI
GetBaseAddress (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT64              *BaseAddress
);

VOID
EFIAPI
CheckPciMmio (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT BOOLEAN             *Pci,
    OUT BOOLEAN             *Mmio,
    OUT UINT8               *MmioWidth
);

VOID
EFIAPI
GetPciLocation (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINTN               *Bus,
    OUT UINTN               *Dev,
    OUT UINTN               *Func,
    OUT UINT8               *Port
);

VOID
EFIAPI
GetSerialIRQ (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT8               *SerialIRQ
);

VOID
EFIAPI
GetUID (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT8               *UID
);

VOID
EFIAPI
GetUartClock (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINTN              *Clock
);

EFI_STATUS
EFIAPI
AmiSerialGetDriverName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  CHAR8                        *Language,
    OUT CHAR16                      **DriverName
);

EFI_STATUS 
EFIAPI
AmiSerialSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS 
EFIAPI
AmiSerialStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
);

EFI_STATUS 
EFIAPI
AmiSerialStop (
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     ControllerHandle,
    IN  UINTN                          NumberOfChildren,
    IN  EFI_HANDLE                     *ChildHandleBuffer
);


DLIST   AmiSerialDeviceList;

#ifndef LANGUAGE_CODE_ENGLISH
CHAR8 SupportedLanguages[] = "eng";
#else
CHAR8 SupportedLanguages[] = LANGUAGE_CODE_ENGLISH;
#endif

CHAR16 *gAmiSerialDriverName = L"AMI Serial Driver";

EFI_COMPONENT_NAME2_PROTOCOL gAmiSerialCtlName = {
    AmiSerialGetDriverName,
    0,
    0
};

EFI_DRIVER_BINDING_PROTOCOL gAmiSerialDriverBinding = {
    AmiSerialSupported,
    AmiSerialStart,
    AmiSerialStop,
    0x10,
    NULL,
    NULL
};

/**
    This function will return the Driver name's address.

    @param This Pointer to Component name protocol
    @param Language Lang. of driver name that caller needs.
    @param DriverName Drivername's address

    @retval EFI_INVALID_PARAMETER if Language & DriverName pointer are NULL.
    @retval EFI_UNSUPPORTED if the Caller's Language is not supported.
    @retval EFI_SUCCESS Driver name is given successfully.

**/

EFI_STATUS
EFIAPI
AmiSerialGetDriverName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL    *This,
    IN  CHAR8                           *Language,
    OUT CHAR16                          **DriverName
)
{
    if ( !Language || !DriverName ) {
        return EFI_INVALID_PARAMETER;
    }

    if ( !LanguageCodesEqual( Language, This->SupportedLanguages) ) {
        return EFI_UNSUPPORTED;
    }

    *DriverName = gAmiSerialDriverName;
    return EFI_SUCCESS;
}

/**
    Entry point for AMI Serial Driver.It installs AMI Serial DriverBinding
    Protocol.

    @param  ImageHandle The image handle.
    @param  SystemTable A pointer to the EFI system table.

    @retval EFI_STATUS

**/

EFI_STATUS 
EFIAPI
AmiSerialEntryPoint (
    IN  EFI_HANDLE           ImageHandle,
    IN  EFI_SYSTEM_TABLE     *SystemTable
  )
{

    EFI_STATUS  Status;

    InitAmiLib(ImageHandle,SystemTable);

    //
    // Initialize the ImageHandle and DriverBindingHandle
    //
    gAmiSerialDriverBinding.DriverBindingHandle = NULL;
    gAmiSerialDriverBinding.ImageHandle = ImageHandle;

    gAmiSerialCtlName.SupportedLanguages = SupportedLanguages;

    DListInit(&AmiSerialDeviceList);

    Status = pBS->InstallMultipleProtocolInterfaces(
                    &gAmiSerialDriverBinding.DriverBindingHandle,
                    &gEfiDriverBindingProtocolGuid, &gAmiSerialDriverBinding,
                    &gEfiComponentName2ProtocolGuid, &gAmiSerialCtlName,
                    NULL);

    return Status;

}

/**
    This checks for the presence of serial device. This function needs to be 
    Ported based on the Serial Device.

    @param This Pointer to driver binding protocol
    @param Controller Controller Handle
    @param RemainingDevicePath Pointer to
        RemainingDevicePath

    @retval EFI_UNSUPPORTED Driver doesn't support the controller.
    @retval EFI_ALREADY_STARTED Driver already handles Controller.   
    @retval EFI_ACCESS_DENIED Controller has been exclusively access by another.
    @retval EFI_SUCCES Driver Supports the controller

    @note  
      This function is written for generic PCI Serial device. OEM has to port
      this function for the non-generic Serial device.
      Open the EFI_PCI_IO_PROTOCOL with EFI_OPEN_PROTOCOL_BY_DRIVER |
      EFI_OPEN_PROTOCOL_EXCLUSIVE attribute so that AMI Serial driver handles
      this controller exclusively.
**/

EFI_STATUS 
EFIAPI
AmiSerialSupported (
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo=NULL;
    UINT8                   RevisionId[4];
    UINT64                  CommandReg=0;
    UINT64                  Supports=0;
    ASLR_QWORD_ASD          *Resources=NULL;
    UINT8                   i;
    EFI_DEVICE_PATH_PROTOCOL    *tmpdp=NULL;
    EFI_HANDLE              TempHandle;
    EFI_PCI_IO_PROTOCOL     *RootPciIo=NULL;
    EFI_DEVICE_PATH_PROTOCOL *TruncatedDevPath = NULL;
    EFI_DEVICE_PATH_PROTOCOL *TempDevPath = NULL;

    Status = pBS->OpenProtocol (Controller,
                                &gEfiDevicePathProtocolGuid,
                                (VOID **)&tmpdp, 
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );

    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }

    Status = pBS->OpenProtocol (Controller,
                                &gEfiPciIoProtocolGuid,
                                (VOID **)&PciIo,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL
                                );

    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }

    Status = PciIo->Pci.Read (PciIo,
                     EfiPciIoWidthUint32,
                     PCI_REV_ID_OFFSET,
                     1,
                     &RevisionId
                     );
    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }
    //----------------------------------------------------------------------//
    //                  ***** PORTING REQUIRED *****                        //
    //----------------------------------------------------------------------//
    //
    // Check for the correct PCI Serial Device, if found, Open the protocol 
    // with EFI_OPEN_PROTOCOL_BY_DRIVER|EFI_OPEN_PROTOCOL_EXCLUSIVE attribute
    // so that AMI Serial driver exclusively handles that PCI Serial Device. 
    // (Note: Below code is written for generic PCISerial device). The PCI
    // Serial device programming has to be ported. We need to port for the BAR
    // selection IO/MMIO.
    //

    if ((RevisionId[3] == PCI_CL_COMM) &&\
        ((RevisionId[2] == PCI_CL_COMM_CSL_SERIAL) ||\
            (RevisionId[2] == PCI_CL_COMM_CSL_OTHER))) {

        Status = pBS->OpenProtocol (Controller, 
                                    &gEfiPciIoProtocolGuid,
                                    (VOID **)&PciIo,
                                    This->DriverBindingHandle,
                                    Controller,
                                    EFI_OPEN_PROTOCOL_BY_DRIVER |\
                                    EFI_OPEN_PROTOCOL_EXCLUSIVE
                                    );

        if ( EFI_ERROR( Status )) {
            if( Status == EFI_ALREADY_STARTED || Status == EFI_ACCESS_DENIED ) {
                return Status;
            } 
            return EFI_UNSUPPORTED;
        }

    } else {
        return EFI_UNSUPPORTED;
    }

    Status = pBS->CloseProtocol (Controller,
                        &gEfiPciIoProtocolGuid,
                        This->DriverBindingHandle,
                        Controller
                        );
    if ( EFI_ERROR( Status )) {
        return EFI_UNSUPPORTED;
    }

    Supports = 1;

    for (i = 0; i < PCI_MAX_BAR_NO; i++) {
        Status = PciIo->GetBarAttributes (PciIo,
                                          i,
                                          &Supports,
                                          &Resources
                                          );
        if ( EFI_ERROR( Status )) {
            return EFI_UNSUPPORTED;
        }
        //
        // Find Serial device Resource type. Based on that Enable IO/Memory. 
        //
        if (Resources->Type == ASLRV_SPC_TYPE_IO) {
            CommandReg = EFI_PCI_IO_ATTRIBUTE_IO;
            pBS->FreePool(Resources);
            break;
        } else if(Resources->Type == ASLRV_SPC_TYPE_MEM) {
            CommandReg = EFI_PCI_IO_ATTRIBUTE_MEMORY;
            pBS->FreePool(Resources);
            break;
        }
    }

    if(CommandReg != 0) {
        Status = PciIo->Attributes (PciIo,
                                    EfiPciIoAttributeOperationEnable,
                                    CommandReg,
                                    NULL);
        if ( EFI_ERROR( Status )) {
            return EFI_UNSUPPORTED;
        }

        TruncatedDevPath=tmpdp;
        do {
            TruncatedDevPath = DPCut(TruncatedDevPath);
            if(TempDevPath) {
                pBS->FreePool(TempDevPath);
            }
            if(TruncatedDevPath == NULL ) {
                break;
            }

            //
            // Locate handle using device path
            //
            TempHandle= NULL;    
            TempDevPath=TruncatedDevPath;
            Status = pBS->LocateDevicePath(&gEfiPciIoProtocolGuid,
                                           &TempDevPath,
                                           &TempHandle);

            if(Status == EFI_SUCCESS) {
                
                TempDevPath=TruncatedDevPath;

                RootPciIo=NULL;
                Status = pBS->HandleProtocol (TempHandle,
                                              &gEfiPciIoProtocolGuid,
                                              (VOID **)&RootPciIo);

                if(Status == EFI_SUCCESS) {
                    Status = RootPciIo->Attributes (RootPciIo,
                                            EfiPciIoAttributeOperationEnable,
                                            CommandReg,
                                            NULL);

                }

            }

        }while(TruncatedDevPath != NULL);

    }
    return EFI_SUCCESS;
}

/**
    Installs AMI_SERIAL_PROTOCOL for the Serial device. 
    This Function needs to be ported based on the Serial Device.

    @param This Pointer to driver binding protocol
    @param Controller Controller Handle
    @param RemainingDevicePath Pointer to RemainingDevicePath

    @retval EFI_STATUS
    @retval EFI_DEVICE_ERROR
    @retval EFI_SUCCES

    @note 
    This function is written for generic PCI Serial device. OEM has to port
    this function for the non-generic Serial device.
    Open the EFI_PCI_IO_PROTOCOL with EFI_OPEN_PROTOCOL_BY_DRIVER |
    EFI_OPEN_PROTOCOL_EXCLUSIVE attribute so that AMI Serial driver handles
    this controller exclusively.
    The AMI_SERIAL_PROTOCOL has to be installed for each port in the cases
    where we have many ports in the same Serial Device for the redirection to
    happen in all the ports.
**/

EFI_STATUS 
EFIAPI
AmiSerialStart (
    IN  EFI_DRIVER_BINDING_PROTOCOL    *This,
    IN  EFI_HANDLE                     Controller,
    IN  EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
)
{
    EFI_STATUS              Status;
    AMI_SERIAL_PARAMETERS   *AmiSerialParameters=NULL;
    AMI_SERIAL_PARAMETERS   *SerialParameters=NULL;
    UINTN                   Segment=0;
    UINTN                   Bus=0;
    UINTN                   Device=0;
    UINTN                   Function=0;
    EFI_PCI_IO_PROTOCOL     *PciIo=0;
    UINT8                   i;
    UINT64                  Supports=0;
    ASLR_QWORD_ASD          *Resources=0;
    DLINK                   *dlink;


    Status = pBS->OpenProtocol (Controller, 
                                &gEfiPciIoProtocolGuid,
                                (VOID **)&PciIo,
                                This->DriverBindingHandle,
                                Controller,
                                EFI_OPEN_PROTOCOL_BY_DRIVER\
                                | EFI_OPEN_PROTOCOL_EXCLUSIVE
                                );

    //----------------------------------------------------------------------//
    //                  ***** PORTING REQUIRED *****                        //
    //----------------------------------------------------------------------//

    //
    // The below code has been written for the generic PCI Serial Device.
    // Handle differently for the PCI Serial Device you need based on the
    // Controller Specification/data sheet.
    //
    if ( !(EFI_ERROR( Status ))) {
    
        AmiSerialParameters = MallocZ(sizeof(AMI_SERIAL_PARAMETERS));
        if ( !AmiSerialParameters ) {
            return EFI_OUT_OF_RESOURCES;
        }

        for (i = 0; i < PCI_MAX_BAR_NO; i++) {

            Status = PciIo->GetBarAttributes (PciIo,
                                              i,
                                              &Supports,
                                              &Resources
                                              );
            //
            // Get the BaseAddress and the Port/Device type IO/MMIO.
            //
            if (Resources->Type == ASLRV_SPC_TYPE_IO) {
                AmiSerialParameters->BaseAddress = (UINT64)Resources->_MIN;
                Status = EFI_SUCCESS;
                pBS->FreePool(Resources);
                break;
            }
            if (Resources->Type == ASLRV_SPC_TYPE_MEM) {
                AmiSerialParameters->Mmio=TRUE;
                // MMIO Width has to be ported based on the device's DataSheet.
                // Byte Width - 1, Word width - 2, Dword Width - 4 only supported.
                AmiSerialParameters->MmioWidth=1;
                AmiSerialParameters->BaseAddress = (UINT64)Resources->_MIN;
                Status = EFI_SUCCESS;
                pBS->FreePool(Resources);
                break;
            } 
            pBS->FreePool(Resources);
        }
    
        if(EFI_ERROR( Status )) {
            goto Error;
        }

        AmiSerialParameters->Pci=TRUE;

        //
        // Get the Serial Device Location.
        //
        Status = PciIo->GetLocation(PciIo, &Segment, &Bus, 
                                    &Device, &Function);
        if (EFI_ERROR(Status)) {
            goto Error;
        }

        //
        // Get the IRQ number for the Serial Device.
        //
        Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, PCI_INTLINE, 1, 
                                        &AmiSerialParameters->SerialIRQ);
        if (EFI_ERROR(Status)) {
            goto Error;
        }

        AmiSerialParameters->Bus = Bus;
        AmiSerialParameters->Dev = Device;
        AmiSerialParameters->Func= Function;

        for (dlink = AmiSerialDeviceList.pHead; dlink; dlink = dlink->pNext) {
            SerialParameters = OUTTER(dlink, SerialDeviceLink, AMI_SERIAL_PARAMETERS);
            if( (SerialParameters->Bus == Bus) \
                 && (SerialParameters->Dev == Device)\
                 && (SerialParameters->Func == Function) ) {
                AmiSerialParameters->Port = SerialParameters->Port + 1;
            }
        }

        DListAdd(&AmiSerialDeviceList, &(AmiSerialParameters->SerialDeviceLink));
         
        AmiSerialParameters->AmiSerialProtocol.GetBaseAddress = GetBaseAddress;
        AmiSerialParameters->AmiSerialProtocol.CheckPciMmio   = CheckPciMmio;
        AmiSerialParameters->AmiSerialProtocol.GetPciLocation = GetPciLocation;
        AmiSerialParameters->AmiSerialProtocol.GetSerialIRQ = GetSerialIRQ;
        AmiSerialParameters->AmiSerialProtocol.GetUID = GetUID;
        AmiSerialParameters->AmiSerialProtocol.GetUartClock = GetUartClock;

        //
        // Install AmiSerial protocol interfaces for the serial device.
        //
        Status = pBS->InstallMultipleProtocolInterfaces (
                                    &AmiSerialParameters->Handle,
                                    &gAmiSerialProtocolGuid,
                                    &AmiSerialParameters->AmiSerialProtocol,
                                    NULL);

        if(EFI_ERROR( Status )) {
            goto Error;
        }

        //
        // Open the PCIIO protocol as child for the AmiSerial.
        //
        Status = pBS->OpenProtocol (Controller,
                                    &gEfiPciIoProtocolGuid,
                                    (VOID **)&PciIo,
                                    This->DriverBindingHandle,
                                    AmiSerialParameters->Handle,
                                    EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                                    );

        if(EFI_ERROR( Status )) {
            pBS->CloseProtocol( Controller,
                                &gEfiPciIoProtocolGuid,
                                This->DriverBindingHandle,
                                Controller
                                );
            return EFI_DEVICE_ERROR;
        }

        return EFI_SUCCESS;
            
    } else {
        return EFI_DEVICE_ERROR;
    }

Error:

    pBS->FreePool(AmiSerialParameters);
    pBS->CloseProtocol( Controller,
                        &gEfiPciIoProtocolGuid,
                        This->DriverBindingHandle,
                        Controller
                        );
    return EFI_DEVICE_ERROR;

}

/**
    Uninstall's, Close's the Protocols that were installed in Start function.
    Needs to be ported based on Start() Function.
 
    @param This Pointer to driver binding protocol
    @param Controller Controller Handle to stop.
    @param NumberOfChildren No. of children's.
    @param ChildHandleBuffer Pointer to child handles

    @retval EFI_STATUS
    @retval EFI_DEVICE_ERROR
    @retval EFI_SUCCES

    @note 
    This function is written for generic PCI Serial device. OEM has to port
    this function for the non-generic Serial device. It should be based on 
    Start().
**/

EFI_STATUS EFIAPI
AmiSerialStop (
    IN  EFI_DRIVER_BINDING_PROTOCOL     *This,
    IN  EFI_HANDLE                      Controller,
    IN  UINTN                           NumberOfChildren,
    IN  EFI_HANDLE                      *ChildHandleBuffer
)
{

    EFI_STATUS              Status;
    AMI_SERIAL_PROTOCOL     *AmiSerialProtocol=NULL;
    AMI_SERIAL_PARAMETERS   *AmiSerialParameters=NULL;

    //----------------------------------------------------------------------//
    //                  ***** PORTING REQUIRED *****                        //
    //----------------------------------------------------------------------//

    //
    // Porting is needed based on the implementation of AmiSerialStart().
    // Need to uninstall protocols&free memory that done in AmiSerialStart().
    //
    if(!NumberOfChildren) {
        Status = pBS->CloseProtocol(
                        Controller,
                        &gEfiPciIoProtocolGuid,
                        This->DriverBindingHandle,
                        Controller
                        );
        if(EFI_ERROR( Status )) {
            return EFI_DEVICE_ERROR;
        }
        return EFI_SUCCESS;
    }

    Status = pBS->OpenProtocol (
                    *ChildHandleBuffer,
                    &gAmiSerialProtocolGuid,
                    (VOID **)&AmiSerialProtocol,
                    This->DriverBindingHandle,
                    *ChildHandleBuffer,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL
                    );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    AmiSerialParameters = (AMI_SERIAL_PARAMETERS *)AmiSerialProtocol;

    DListDelete(&AmiSerialDeviceList, &(AmiSerialParameters->SerialDeviceLink));

    Status = pBS->CloseProtocol(Controller,
                                &gEfiPciIoProtocolGuid,
                                This->DriverBindingHandle,
                                AmiSerialParameters->Handle
                                );

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }
    //
    // Uninstall the AMI_SERIAL_PROTOCOL.
    //
    Status = pBS->UninstallMultipleProtocolInterfaces (
                                    AmiSerialParameters->Handle,
                                    &gAmiSerialProtocolGuid,
                                    &AmiSerialParameters->AmiSerialProtocol,
                                    NULL);
    
    pBS->FreePool(AmiSerialParameters);

    if ( EFI_ERROR( Status )) {
        return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
}

/**
    Gets the base address for the Serial Port.
 
    @param This Pointer to AmiSerialProtocol
    @param BaseAddress Pointer to return Base Address  

    @retval VOID

**/

VOID
EFIAPI
GetBaseAddress (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT64              *BaseAddress
) 
{
    *BaseAddress = ((AMI_SERIAL_PARAMETERS *)This)->BaseAddress;
    return;
}

/**
    Gives information whether it is a Pci serial device, if yes,
    tells whether it is a MMIO device.
 
    @param This Pointer to AmiSerialProtocol
    @param Pci BOOLEAN to fill whether it is PCI device 
    @param Mmio BOOLEAN to fill whether it is MMIO access

    @retval VOID

**/

VOID
EFIAPI
CheckPciMmio (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT BOOLEAN             *Pci,
    OUT BOOLEAN             *Mmio,
    OUT UINT8               *MmioWidth
)
{
    *Pci = ((AMI_SERIAL_PARAMETERS *)This)->Pci;
    *Mmio= ((AMI_SERIAL_PARAMETERS *)This)->Mmio;
    *MmioWidth = ((AMI_SERIAL_PARAMETERS *)This)->MmioWidth;
    return;
}

/**
    This Function will give the Bus/Dev/func if it is a PCI Serial Device.
 
    @param This Pointer to AmiSerialProtocol
    @param Bus Pointer to return Bus No.
    @param Dev Pointer to return Device No.
    @param Func Pointer to return Function No.
    @param Port Pointer to return Port No.

    @retval VOID

**/

VOID
EFIAPI
GetPciLocation (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINTN               *Bus,
    OUT UINTN               *Dev,
    OUT UINTN               *Func,
    OUT UINT8               *Port
)
{
    *Bus = ((AMI_SERIAL_PARAMETERS *)This)->Bus;
    *Dev = ((AMI_SERIAL_PARAMETERS *)This)->Dev;
    *Func = ((AMI_SERIAL_PARAMETERS *)This)->Func;
    *Port = ((AMI_SERIAL_PARAMETERS *)This)->Port;
    return;
}

/**
    Gets the IRQ number for the Serial Port.
 
    @param This Pointer to AmiSerialProtocol
    @param SerialIRQ Pointer to return IRQ number.

    @retval VOID

**/

VOID
EFIAPI
GetSerialIRQ (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT8             *SerialIRQ
)
{
    *SerialIRQ = ((AMI_SERIAL_PARAMETERS *)This)->SerialIRQ;
    return;
}


/**
    Gets the UID for the Serial Port.
 
    @param This Pointer to AmiSerialProtocol
    @param Uid Pointer to return UID.

    @retval VOID
    
    @note This API added for non-generic serial ports other than PCI serial 
    ports.

**/

VOID
EFIAPI
GetUID (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINT8               *Uid
)
{
    /// PORTING Needed 
    /// If more than a COM port present, porting engineer should take
    /// care while assigning UID. UID of COM ports should not be the same.
    *Uid = 0;
    return;
}


/**
    Gets the UART Clock .
 
    @param This Pointer to AmiSerialProtocol
    @param Clock Pointer to return UART Clock.

    @retval VOID
    
    @note This API added for non-generic serial ports other than PCI serial 
    ports.

**/

VOID
EFIAPI
GetUartClock (
    IN  AMI_SERIAL_PROTOCOL *This,
    OUT UINTN              *Clock
)
{

    /// If UART clock is different for different Non-generic PCI devices, 
    /// porting engineer should take care of assigning proper Clock value.
    *Clock=1843200;
    return;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
