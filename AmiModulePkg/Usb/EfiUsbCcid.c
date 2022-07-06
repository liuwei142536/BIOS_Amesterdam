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

/** @file EfiUsbCcid.c
    USB CCID EFI driver implementation

**/

#include "Uhcd.h"
#include "ComponentName.h"
#include "UsbBus.h"
#include <AmiUsbRtCcid.h>
#include "AmiUsbSmartCardReader.h"

extern USB_GLOBAL_DATA *gUsbData;
extern URP_STRUC       *gParameters;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

// Below gEfiSmartCardReaderProtocolGuid definiion will be removed once
// it is added into MdePkg
EFI_GUID gEfiSmartCardReaderProtocolGuid = EFI_SMART_CARD_READER_PROTOCOL_GUID;

EFI_STATUS
EFIAPI
SupportedUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath
);

EFI_STATUS
EFIAPI
StartUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath
);

EFI_STATUS
EFIAPI
StopUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    UINTN                       NumberOfChildren,
    EFI_HANDLE                  *Children
);

EFI_STATUS
GetReturnValue(
    UINT8    bRetValue
);

ICC_DEVICE*
GetICCDevice(
    DEV_INFO        *FpDevInfo, 
    UINT8            Slot
);

EFI_STATUS
EFIAPI
USBCCIDAPISmartClassDescriptor(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *ResponseBuffer
);

EFI_STATUS
EFIAPI
USBCCIDAPIGetAtr(
    IN AMI_CCID_IO_PROTOCOL *This,
    UINT8 Slot,
    UINT8 *ATRData
);

EFI_STATUS
EFIAPI
USBCCIDAPIPowerupSlot(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *bStatus,
    OUT UINT8                           *bError,
    OUT UINT8                           *ATRData
);

EFI_STATUS
EFIAPI
USBCCIDAPIPowerDownSlot(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *bStatus,
    OUT UINT8                           *bError
);

EFI_STATUS
EFIAPI
USBCCIDAPIGetSlotStatus(
    IN AMI_CCID_IO_PROTOCOL              *This,
    OUT UINT8                            *bStatus,
    OUT UINT8                            *bError,
    OUT UINT8                            *bClockStatus

);

EFI_STATUS
EFIAPI
USBCCIDAPIXfrBlock(
    IN AMI_CCID_IO_PROTOCOL             *This,
    IN UINTN                            CmdLength,
    IN UINT8                            *CmdBuffer,
    IN UINT8                            ISBlock,
    OUT UINT8                           *bStatus,
    OUT UINT8                           *bError,
    IN OUT UINTN                        *ResponseLength,
    OUT UINT8                           *ResponseBuffer
);

EFI_STATUS
EFIAPI
USBCCIDAPIGetParameters(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *bStatus,
    OUT UINT8                           *bError,
    IN OUT UINTN                        *ResponseLength,
    OUT UINT8                           *ResponseBuffer
);

EFI_GUID gAmiCCIDIoProtocolGuid = AMI_CCID_IO_PROTOCOL_GUID;
EFI_GUID gAmiCCIDPresenceGuid = AMI_CCID_PRESENCE_GUID;

EFI_EVENT       gEvICCEnumTimer = 0;
UINTN           gCounterCCIDEnumTimer = 0;
UINTN           gICCLock = 0;

#define    USBCCID_DRIVER_VERSION        1

EFI_DRIVER_BINDING_PROTOCOL CcidBindingProtocol = {
        SupportedUsbCcid,
        StartUsbCcid,
        StopUsbCcid,
        USBCCID_DRIVER_VERSION,
        NULL,
        NULL 
};

/**
    Function installs EFI_SMART_CARD_READER_PROTOCOL_GUID for Smart card reader's slot

    @param CCIDHandle     Ccid handle
    @param fpCCIDDevice   Ptr to Ccid device info
    @param fpICCDevice    Ptr to Ccid device 

    @retval EFI_SUCCESS             Success to install Usb card reader protocol
    @retval EFI_INVALID_PARAMETER   Input is not valid.

**/

EFI_STATUS
InstallUSBSCardReaderProtocolOnSlot(
    EFI_HANDLE      CCIDHandle,
    DEV_INFO        *fpCCIDDevice,
    ICC_DEVICE      *fpICCDevice
)
{

    EFI_STATUS             Status = EFI_INVALID_PARAMETER;
    EFI_USB_IO_PROTOCOL    *UsbIo = NULL;
    USB_SCARD_DEV          *SCardDev=NULL;
    CCID_DEV_INFO          *CcidDevData;
    
    CcidDevData =  (CCID_DEV_INFO*)fpCCIDDevice->SpecificDevData;

    // Install Protocol for the SCard Reader Slot
    if(!fpCCIDDevice || !CcidDevData || !((SMARTCLASS_DESC*)(CcidDevData->CcidDescriptor)) ) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->AllocatePool(EfiBootServicesData, sizeof(USB_SCARD_DEV), (VOID**)&SCardDev);
    SCardDev->ChildHandle = 0;
    SCardDev->ControllerHandle = CCIDHandle;
    SCardDev->Slot        = fpICCDevice->Slot;
    SCardDev->DevInfo     = fpCCIDDevice;

    SCardDev->EfiSmartCardReaderProtocol.SCardConnect    = USBSCardReaderAPIConnect;
    SCardDev->EfiSmartCardReaderProtocol.SCardStatus     = USBSCardReaderAPIStatus;
    SCardDev->EfiSmartCardReaderProtocol.SCardDisconnect = USBSCardReaderAPIDisconnect;
    SCardDev->EfiSmartCardReaderProtocol.SCardTransmit   = USBSCardReaderAPITransmit;
    SCardDev->EfiSmartCardReaderProtocol.SCardControl    = USBSCardReaderAPIControl;
    SCardDev->EfiSmartCardReaderProtocol.SCardGetAttrib  = USBSCardReaderAPIGetAttrib;

    Status = gBS->InstallProtocolInterface(
                        &SCardDev->ChildHandle,
                        &gEfiSmartCardReaderProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        &SCardDev->EfiSmartCardReaderProtocol
                        );
    ASSERT_EFI_ERROR(Status);

    fpICCDevice->SCardChildHandle = SCardDev->ChildHandle;

    Status = gBS->OpenProtocol (
                        CCIDHandle,
                        &gEfiUsbIoProtocolGuid,
                        (VOID**)&UsbIo,
                        CcidBindingProtocol.DriverBindingHandle,
                        SCardDev->ChildHandle,
                        EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER );

    ASSERT_EFI_ERROR(Status);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Install SCard on %lx status = %x SCardDev->ChildHandle %x \n", CCIDHandle, Status, SCardDev->ChildHandle);

    return Status;
}

/**
    Function installs AMI_CCID_IO_PROTOCOL_GUID for the ICC card

    
    @param CCIDHandle      Ccid handle
    @param fpCCIDDevice    Ptr to Ccid device info
    @param fpICCDevice     Ptr to Ccid device

    @retval EFI_SUCCESS             Success to install Usb ccid device
    @retval EFI_INVALID_PARAMETER   Input is not valid.

**/

EFI_STATUS
InstallUSBCCID(
    EFI_HANDLE      CCIDHandle,
    DEV_INFO        *fpCCIDDevice, 
    ICC_DEVICE      *fpICCDevice
)
{

    EFI_STATUS                  Status = EFI_INVALID_PARAMETER;
    USB_ICC_DEV                 *ICCDev;
    EFI_USB_IO_PROTOCOL         *UsbIo;

    //
    // Install Protocol irrespective of device found or not. 
    // By checking ChildHandle here, avoid repeated protocol installation.
    //

    if (fpICCDevice && !fpICCDevice->ChildHandle) {
    
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "InstallUSBCCID ....\n" );

        gBS->AllocatePool(EfiBootServicesData, sizeof(USB_ICC_DEV), (VOID**)&ICCDev);
        ICCDev->ChildHandle = 0;
        ICCDev->ControllerHandle = 0;
        ICCDev->DevInfo    = fpCCIDDevice;
        ICCDev->Slot       = fpICCDevice->Slot;


        ICCDev->CCIDIoProtocol.USBCCIDAPISmartClassDescriptor = USBCCIDAPISmartClassDescriptor;
        ICCDev->CCIDIoProtocol.USBCCIDAPIGetAtr = USBCCIDAPIGetAtr;
        ICCDev->CCIDIoProtocol.USBCCIDAPIPowerupSlot = USBCCIDAPIPowerupSlot;
        ICCDev->CCIDIoProtocol.USBCCIDAPIPowerDownSlot = USBCCIDAPIPowerDownSlot;
        ICCDev->CCIDIoProtocol.USBCCIDAPIGetSlotStatus = USBCCIDAPIGetSlotStatus;
        ICCDev->CCIDIoProtocol.USBCCIDAPIXfrBlock = USBCCIDAPIXfrBlock;
        ICCDev->CCIDIoProtocol.USBCCIDAPIGetParameters = USBCCIDAPIGetParameters;
    
        Status = gBS->InstallProtocolInterface(
                        &ICCDev->ChildHandle,
                        &gAmiCCIDIoProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        &ICCDev->CCIDIoProtocol
                        );

        ASSERT_EFI_ERROR(Status);

        fpICCDevice->ChildHandle = ICCDev->ChildHandle;

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "ICCDev->ChildHandle = %x\n", ICCDev->ChildHandle);

        Status = gBS->OpenProtocol (
                        CCIDHandle,
                        &gEfiUsbIoProtocolGuid,
                        (VOID**)&UsbIo,
                        CcidBindingProtocol.DriverBindingHandle,
                        ICCDev->ChildHandle,
                        EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER );
    
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Install CCID on %x status = %r\n", CCIDHandle, Status);
        ASSERT_EFI_ERROR(Status);

    }

    return Status;

}

/**
    Uninstall EFI_SMART_CARD_READER_PROTOCOL for Smart card reader's slot

    @param Controller            Controller handle 
    @param ScardHandle           Scard handle
    @param DriverBindingHandle   Driver binding handle

    @retval EFI_SUCCESS             Success to uninstall Usb card reader protocol
    @retval EFI_INVALID_PARAMETER   Input is not valid.

**/

EFI_STATUS
UnInstallScardProtocol (
    EFI_HANDLE    Controller,
    EFI_HANDLE    ScardHandle,
    EFI_HANDLE    DriverBindingHandle
)
{

    EFI_STATUS                       Status;
    EFI_SMART_CARD_READER_PROTOCOL   *SmartCardReaderProtocol;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "UnInstallSCardProtocol ....ChildHandle : %lx", ScardHandle);

    Status = gBS->OpenProtocol ( ScardHandle,
                                &gEfiSmartCardReaderProtocolGuid,
                                (VOID**)&SmartCardReaderProtocol,
                                DriverBindingHandle,
                                ScardHandle,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    
    if(!EFI_ERROR(Status)) {

        Status = gBS->CloseProtocol (Controller, 
                                     &gEfiUsbIoProtocolGuid, 
                                     DriverBindingHandle, 
                                     ScardHandle);
        
        ASSERT_EFI_ERROR(Status);

        Status = gBS->UninstallProtocolInterface ( ScardHandle, 
                                                   &gEfiSmartCardReaderProtocolGuid, 
                                                   SmartCardReaderProtocol);

        ASSERT_EFI_ERROR(Status);

        Status = gBS->FreePool(SmartCardReaderProtocol);
        ASSERT_EFI_ERROR(Status);
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, " Status : %r\n", Status);
    return Status;
}

/**
    Uninstall AMI_CCID_IO_PROTOCOL_GUID for each slot

    @param  CCIDHandle    SmartCard Reader Handle
    @param  ChildHandle   Smart Card Handle

    @retval EFI_STATUS    Status of the operation
**/

EFI_STATUS
UnInstallUsbCcid (
    EFI_HANDLE    CcidHandle,
    EFI_HANDLE    ChildHandle
)
{

    EFI_STATUS                Status;
    AMI_CCID_IO_PROTOCOL      *CcidIoProtocol;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "UnInstallUSBCCID ....ChildHandle : %lx", ChildHandle );

    Status = gBS->OpenProtocol ( ChildHandle, 
                                &gAmiCCIDIoProtocolGuid, 
                                (VOID**)&CcidIoProtocol, 
                                CcidBindingProtocol.DriverBindingHandle, 
                                ChildHandle, 
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);

    //
    // CCID protocol not found on ChildHandle. return with error.
    //
    if(!EFI_ERROR(Status)) {
        // If CCID protocol Found, close the protocol and uninstall the protocol interface.

        Status = gBS->CloseProtocol (CcidHandle, 
                                     &gEfiUsbIoProtocolGuid, 
                                     CcidBindingProtocol.DriverBindingHandle, 
                                     ChildHandle);
        if (EFI_ERROR(Status)) {
            return Status;
        }
        Status = gBS->UninstallProtocolInterface ( ChildHandle, 
                                                   &gAmiCCIDIoProtocolGuid, 
                                                   CcidIoProtocol);
    
        if(!EFI_ERROR(Status)){
            gBS->FreePool(CcidIoProtocol);
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, " Status : %r\n", Status);
    return Status;
}

/**
    Generates a SW SMI to get the SMART Class Descriptor for the CCID device

        
    @param This            Ptr for AMI_CCID_IO_PROTOCOL
    @param ResponseBuffer  Ptr for response buffer.

    @retval EFI_STATUS     Status returns SMART Class Descriptor in ResponseBuffer

**/

EFI_STATUS
EFIAPI
USBCCIDAPISmartClassDescriptor(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *ResponseBuffer
)
{

    EFI_STATUS  Status;
//    URP_STRUC   Parameters = {0};

    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_SMARTCLASSDESCRIPTOR;

    gParameters->ApiData.CCIDSmartClassDescriptor.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDSmartClassDescriptor.Slot = ((USB_ICC_DEV *)This)->Slot;
    gParameters->ApiData.CCIDSmartClassDescriptor.ResponseBuffer = (UINTN)ResponseBuffer;
     
    InvokeUsbApi(gParameters);

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    Generates a SW SMI to get the ATR data

        
    @param This      Ptr for AMI_CCID_IO_PROTOCOL
    @param Slot      Ccid slot
    @param ATRData   Ptr to atrdata

    @retval EFI_SUCCESS    Status returns if available

    @note  
      ATRData buffer length should be 32 bytes long. Caller should allocate 
      memory for *ATRData. 

**/

EFI_STATUS
EFIAPI
USBCCIDAPIGetAtr(
    IN AMI_CCID_IO_PROTOCOL             *This,
    IN UINT8                            Slot,
    OUT UINT8                           *ATRData
)
{

    EFI_STATUS  Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_ATR;

    gParameters->ApiData.CCIDAtr.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDAtr.Slot = ((USB_ICC_DEV *)This)->Slot;
    gParameters->ApiData.CCIDAtr.ATRData = (UINTN)ATRData;
    
    InvokeUsbApi(gParameters);

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    Generates a SW SMI to power up the slot in CCID

        
    @param This      Ptr to AMI_CCID_IO_PROTOCOL
    @param bStatus   Ptr to Ccid status
    @param bError    Ptr to Ccid Error code
    @param ATRData   Ptr to Atrdata

    @retval EFI_SUCCESS    Status returns if Card powered up successfully.

    @note  
        ATRData buffer length should be 32 bytes long. Caller should allocate memory for *ATRData. 
        Presence/Absence of card can be determined from *bStatus and *bError.

**/

EFI_STATUS
EFIAPI
USBCCIDAPIPowerupSlot(
    IN AMI_CCID_IO_PROTOCOL                 *This,
    OUT UINT8                               *bStatus,
    OUT UINT8                               *bError,
    OUT UINT8                               *ATRData
)
{

    EFI_STATUS              Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_POWERUP_SLOT;

    gParameters->ApiData.CCIDPowerupSlot.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDPowerupSlot.Slot = ((USB_ICC_DEV *)This)->Slot;
    gParameters->ApiData.CCIDPowerupSlot.ATRData = (UINTN)ATRData;
    
    InvokeUsbApi(gParameters);

    *bStatus = gParameters->ApiData.CCIDPowerupSlot.bStatus; 
    *bError = gParameters->ApiData.CCIDPowerupSlot.bError;

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    Generates a SW SMI to power down the slot in CCID.

        
    @param This      Ptr to AMI_CCID_IO_PROTOCOL
    @param bStatus   Ptr to Ccid status
    @param bError    Ptr to Ccid Error code

    @retval EFI_SUCCESS    Status returns if Card powered down successfully.

**/

EFI_STATUS
EFIAPI
USBCCIDAPIPowerDownSlot(
    IN AMI_CCID_IO_PROTOCOL                 *This,
    OUT UINT8                               *bStatus,
    OUT UINT8                               *bError
)
{

    EFI_STATUS              Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_POWERDOWN_SLOT;

    gParameters->ApiData.CCIDPowerdownSlot.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDPowerdownSlot.Slot = ((USB_ICC_DEV *)This)->Slot;

    InvokeUsbApi(gParameters);

    *bStatus = gParameters->ApiData.CCIDPowerdownSlot.bStatus; 
    *bError = gParameters->ApiData.CCIDPowerdownSlot.bError;

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    This API returns data from RDR_to_PC_SlotStatus

        
    @param This          Ptr to AMI_CCID_IO_PROTOCOL
    @param bStatus       Ptr to Ccid status
    @param bError        Ptr to Ccid Error code
    @param bClockStatus  Ptr to Ccid clock status.

    @retval EFI_SUCCESS    Status returns if get slot status successfully.

**/

EFI_STATUS
EFIAPI
USBCCIDAPIGetSlotStatus(
    IN AMI_CCID_IO_PROTOCOL             *This,
    OUT UINT8                           *bStatus,
    OUT UINT8                           *bError,
    OUT UINT8                           *bClockStatus

)
{

    EFI_STATUS              Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_GET_SLOT_STATUS;

    gParameters->ApiData.CCIDGetSlotStatus.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDGetSlotStatus.Slot = ((USB_ICC_DEV *)This)->Slot;

    InvokeUsbApi(gParameters);

    *bStatus = gParameters->ApiData.CCIDGetSlotStatus.bStatus; 
    *bError = gParameters->ApiData.CCIDGetSlotStatus.bError;
    *bClockStatus = gParameters->ApiData.CCIDGetSlotStatus.bClockStatus;

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    This API generates a SWSMI to execute the USB_CCID_XFRBLOCK API.

        
    @param This           Ptr to AMI_CCID_IO_PROTOCOL
    @param CmdLength      Length of CmdBuffer
    @param CmdBuffer      Buffer prepared to be sent to ICC through PC_TO_RDR_XFRBLOCK cmd
    @param ISBlock        Valid only in T1 TDPU        
    @param bStatus        Ptr to Ccid status
    @param bError         Ptr to Ccid Error code 
    @param ResponseLength Lenght of the Responsebuffer
    @param ResponseBuffer Response buffer.
 
    @retval EFI_SUCCESS   Status returns if xfr block successfully.

    @note  
       ISBlock is valid only for T1. For updating IFS use S_IFS_REQUEST(0xC1).
       For WTX request use S_WTX_REQUEST (0xC3). For all others use I_BLOCK(0x0)
**/

EFI_STATUS
EFIAPI
USBCCIDAPIXfrBlock(
    IN AMI_CCID_IO_PROTOCOL                *This,
    IN UINTN                            CmdLength,
    IN UINT8                            *CmdBuffer,
    IN UINT8                             ISBlock,
    OUT UINT8                            *bStatus,
    OUT UINT8                            *bError,
    IN OUT UINTN                         *ResponseLength,
    OUT UINT8                            *ResponseBuffer
)
{

    EFI_STATUS              Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_XFRBLOCK;
    gParameters->ApiData.CCIDXfrBlock.CmdLength = CmdLength;
    gParameters->ApiData.CCIDXfrBlock.CmdBuffer = (UINT32)(UINTN)CmdBuffer;
    gParameters->ApiData.CCIDXfrBlock.ISBlock = I_BLOCK;
    gParameters->ApiData.CCIDXfrBlock.ResponseLength = *ResponseLength;
    gParameters->ApiData.CCIDXfrBlock.ResponseBuffer = (UINTN)ResponseBuffer;

    gParameters->ApiData.CCIDXfrBlock.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDXfrBlock.Slot = ((USB_ICC_DEV *)This)->Slot;
    
    InvokeUsbApi(gParameters);

    *bStatus = gParameters->ApiData.CCIDXfrBlock.bStatus ;
    *bError = gParameters->ApiData.CCIDXfrBlock.bError;
    *ResponseLength = gParameters->ApiData.CCIDXfrBlock.ResponseLength;

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    Returns data from PC_TO_RDR_GETPARAMETERS/RDR_to_PCParameters cmd

        
    @param This           Ptr to AMI_CCID_IO_PROTOCOL
    @param bStatus        Ptr to Ccid status
    @param bError         Ptr to Ccid Error code 
    @param ResponseLength Lenght of the Responsebuffer
    @param ResponseBuffer Response buffer. 

    @retval EFI_SUCCESS   Status returns if get parameter successfully.

**/

EFI_STATUS
EFIAPI
USBCCIDAPIGetParameters(
    IN AMI_CCID_IO_PROTOCOL                *This,
    OUT UINT8                            *bStatus,
    OUT UINT8                            *bError,
    IN OUT UINTN                        *ResponseLength,
    OUT UINT8                            *ResponseBuffer
)
{

    EFI_STATUS              Status;

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_CCID_GET_PARAMETERS;
    gParameters->ApiData.CCIDGetParameters.ResponseLength = *ResponseLength;
    gParameters->ApiData.CCIDGetParameters.ResponseBuffer = (UINT32)(UINTN)ResponseBuffer;

    gParameters->ApiData.CCIDGetParameters.FpDevInfo = (UINTN)((USB_ICC_DEV *)This)->DevInfo;
    gParameters->ApiData.CCIDGetParameters.Slot = ((USB_ICC_DEV *)This)->Slot;
    
    InvokeUsbApi(gParameters);

    *bStatus = gParameters->ApiData.CCIDGetParameters.bStatus ;
    *bError = gParameters->ApiData.CCIDGetParameters.bError;
    *ResponseLength = gParameters->ApiData.CCIDGetParameters.ResponseLength;

    Status = GetReturnValue(gParameters->bRetValue);

    return Status;

}

/**
    Function to connect the Smard Card reader/ICC card in order to access the Smart
         reader/ICC card

    @param This                Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param AccessMode          Access Mode
    @param CardAction          Card Action
    @param PreferredProtocols  Preferred Protocols
    @param ActiveProtocol      Ptr to Active Protocol

    @retval EFI_SUCCESS        Status returns if card reader connect successfully.


**/

EFI_STATUS
EFIAPI
USBSCardReaderAPIConnect(
    IN EFI_SMART_CARD_READER_PROTOCOL *This,
    IN UINT32                         AccessMode,
    IN UINT32                         CardAction,
    IN UINT32                         PreferredProtocols,
    OUT UINT32                        *ActiveProtocol
)
{
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIConnect AccessMode : %x CardAction : %x  PreferredProtocols : %x \n",\
        AccessMode, CardAction, PreferredProtocols);

    // Return error if AccessMode or CardAction input parameter is Invalid
    if( !((AccessMode==SCARD_AM_READER)||(AccessMode==SCARD_AM_CARD)) || 
            !(CardAction <= SCARD_CA_EJECT) ) {
        return EFI_INVALID_PARAMETER;
    }

    // Invalid Input parameter for ScardReader connect call
    if(AccessMode == SCARD_AM_READER) {
        // if AccessMode is set to SCARD_AM_READER,PreferredProtocol must be set to 
        // SCARD_PROTOCOL_UNDEFINED,CardAction to SCARD_CA_NORESET else error
        if( CardAction!=SCARD_CA_NORESET || PreferredProtocols!=SCARD_PROTOCOL_UNDEFINED ) {
            return EFI_INVALID_PARAMETER;
        }
    }

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_SMART_CARD_READER_CONNECT;
    
    gParameters->ApiData.SmartCardReaderConnect.FpDevInfo = (UINTN)((USB_SCARD_DEV *)This)->DevInfo;
    gParameters->ApiData.SmartCardReaderConnect.Slot = (UINTN)((USB_SCARD_DEV *)This)->Slot;
    gParameters->ApiData.SmartCardReaderConnect.AccessMode = AccessMode;
    gParameters->ApiData.SmartCardReaderConnect.CardAction = CardAction;
    gParameters->ApiData.SmartCardReaderConnect.PreferredProtocols = PreferredProtocols;
    gParameters->ApiData.SmartCardReaderConnect.ActiveProtocol = (UINTN)ActiveProtocol;
    
    InvokeUsbApi(gParameters);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIConnect Status : %r ActiveProtocol : %x\n", \
             gParameters->ApiData.SmartCardReaderConnect.EfiStatus, *ActiveProtocol);
    return gParameters->ApiData.SmartCardReaderConnect.EfiStatus;
}

/**
    Function disconnects Scard Reader/ICC card

    @param This        Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param CardAction  Card Action 

    @retval EFI_SUCCESS    Status returns if card reader disconnect successfully.

**/

EFI_STATUS
EFIAPI
USBSCardReaderAPIDisconnect(
    IN EFI_SMART_CARD_READER_PROTOCOL  *This,
    IN UINT32                          CardAction
)
{
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIDisconnect CardAction : %x \n", CardAction);

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_SMART_CARD_READER_DISCONNECT;

    gParameters->ApiData.SmartCardReaderDisconnect.FpDevInfo = (UINTN)((USB_SCARD_DEV *)This)->DevInfo;
    gParameters->ApiData.SmartCardReaderDisconnect.Slot = (UINTN)((USB_SCARD_DEV *)This)->Slot;
    gParameters->ApiData.SmartCardReaderDisconnect.CardAction = CardAction;
    gParameters->ApiData.SmartCardReaderDisconnect.EfiStatus = 0;

    InvokeUsbApi(gParameters);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIDisconnect Status : %r \n", \
    gParameters->ApiData.SmartCardReaderDisconnect.EfiStatus );

    return gParameters->ApiData.SmartCardReaderDisconnect.EfiStatus;
}

/**
    Function to get the status of the ICC card connected in Smart card reader

    @param This              Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param ReaderName        Ptr to Reader Name
    @param ReaderNameLength  Ptr to Reader Name Length
    @param State             Ptr to state
    @param CardProtocol      Ptr to card protocl
    @param Atr               Ptr to atr
    @param AtrLength         Ptr to atr length

    @retval EFI_SUCCESS      Status returns if get card reader status successfully.

**/

EFI_STATUS
EFIAPI
USBSCardReaderAPIStatus(
    IN EFI_SMART_CARD_READER_PROTOCOL *This,
    OUT CHAR16                        *ReaderName,
    IN OUT UINTN                      *ReaderNameLength,
    OUT UINT32                        *State,
    OUT UINT32                        *CardProtocol,
    OUT UINT8                         *Atr,
    IN OUT UINTN                      *AtrLength
)
{
    if( EFI_ERROR( GetSmartCardReaderName( ((USB_SCARD_DEV *)This)->ControllerHandle,
                            ReaderName, 
                            ReaderNameLength ) ) ) {
        return EFI_BUFFER_TOO_SMALL;
    }


    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIStatus ReaderName : %lx ReaderNameLength : %lx Atr : %lx AtrLength : %lx \n",\
        ReaderName, *ReaderNameLength, Atr, *AtrLength);
    
    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_SMART_CARD_READER_STATUS;
    
    gParameters->ApiData.SmartCardReaderStatus.FpDevInfo = (UINTN)((USB_SCARD_DEV *)This)->DevInfo;
    gParameters->ApiData.SmartCardReaderStatus.Slot = (UINTN)((USB_SCARD_DEV *)This)->Slot;
    gParameters->ApiData.SmartCardReaderStatus.ReaderName = (UINTN)ReaderName;
    gParameters->ApiData.SmartCardReaderStatus.ReaderNameLength = (UINTN)ReaderNameLength;
    gParameters->ApiData.SmartCardReaderStatus.State = (UINTN)State;
    gParameters->ApiData.SmartCardReaderStatus.CardProtocol = (UINTN)CardProtocol;
    gParameters->ApiData.SmartCardReaderStatus.Atr = (UINTN)Atr;
    gParameters->ApiData.SmartCardReaderStatus.AtrLength = (UINTN)AtrLength;
      
    InvokeUsbApi(gParameters);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPIStatus ReaderNameLength : %lx AtrLength : %lx  Status : %r\n",\
         *ReaderNameLength, *AtrLength, gParameters->ApiData.SmartCardReaderStatus.EfiStatus);
    
    return gParameters->ApiData.SmartCardReaderStatus.EfiStatus;
}

/**
    This function sends a command to the card or reader and returns its response.

    @param This           Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param CAPDU          Ptr to CAPDU
    @param CAPDULength    CAPDU length
    @param RAPDU          Ptr to RAPDU
    @param RAPDULength    Ptr to RAPDU length

    @retval EFI_SUCCESS   Status returns if card reader transmit successfully.


**/

EFI_STATUS
EFIAPI
USBSCardReaderAPITransmit(
    IN EFI_SMART_CARD_READER_PROTOCOL *This,
    IN UINT8                          *CAPDU,
    IN UINTN                          CAPDULength,
    OUT UINT8                         *RAPDU,
    IN OUT UINTN                      *RAPDULength
)
{
 
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPITransmit CAPDU : %lx CAPDULength : %lx RAPDU : %lx RAPDULength : %lx \n",\
          CAPDU, CAPDULength, RAPDU, *RAPDULength);

    gBS->SetMem(gParameters, sizeof(URP_STRUC), 0);
    gParameters->bFuncNumber = USB_API_CCID_DEVICE_REQUEST;
    gParameters->bSubFunc = USB_SMART_CARD_READER_TRANSMIT;
    
    gParameters->ApiData.SmartCardReaderTransmit.FpDevInfo = (UINTN)((USB_SCARD_DEV *)This)->DevInfo;
    gParameters->ApiData.SmartCardReaderTransmit.Slot = (UINTN)((USB_SCARD_DEV *)This)->Slot;
    gParameters->ApiData.SmartCardReaderTransmit.CAPDU = (UINTN)CAPDU;
    gParameters->ApiData.SmartCardReaderTransmit.CAPDULength = CAPDULength;
    gParameters->ApiData.SmartCardReaderTransmit.RAPDU = (UINTN)RAPDU;
    gParameters->ApiData.SmartCardReaderTransmit.RAPDULength = (UINTN)RAPDULength;
     
    InvokeUsbApi(gParameters);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBSCardReaderAPITransmit RAPDULength : %lx Status : %r\n",\
            *RAPDULength, gParameters->ApiData.SmartCardReaderTransmit.EfiStatus);

    return gParameters->ApiData.SmartCardReaderTransmit.EfiStatus;
}

/**
    This function is the API function of SMART CARD READER PROTOCOL

    @param This             Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param ControlCode      Control code
    @param InBuffer         Ptr to in buffer
    @param InBufferLength   In buffer length
    @param OutBuffer        Ptr to Out buffer 
    @param OutBufferLength  Ptr to out buffer length

    @retval EFI_UNSUPPORTED This protocol is not support.

**/

EFI_STATUS
EFIAPI
USBSCardReaderAPIControl(
  IN     EFI_SMART_CARD_READER_PROTOCOL    *This,
  IN     UINT32                            ControlCode,
  IN     UINT8                             *InBuffer OPTIONAL,
  IN     UINTN                             InBufferLength OPTIONAL,
     OUT UINT8                             *OutBuffer OPTIONAL,
  IN OUT UINTN                             *OutBufferLength OPTIONAL
)
{
    return EFI_UNSUPPORTED;
}

/**
    This function is the API function of SMART CARD READER PROTOCOL

    @param This             Ptr to EFI_SMART_CARD_READER_PROTOCOL
    @param Attrib           attrib
    @param OutBuffer        Ptr to out buffer
    @param OutBufferLength  Ptr to out buffer length

    @retval EFI_UNSUPPORTED This protocol is not support.

**/

EFI_STATUS
EFIAPI
USBSCardReaderAPIGetAttrib(
    IN EFI_SMART_CARD_READER_PROTOCOL *This,
    IN UINT32                         Attrib,
    OUT UINT8                         *OutBuffer,
    IN OUT UINTN                      *OutBufferLength
)
{
    return EFI_UNSUPPORTED;
}


/**
    This function to get SMART CARD reader name

    @param ControllerHandle  Controller handle
    @param ReaderName        Ptr to reader name
    @param ReaderNameLength  Ptr to reader name length

    @retval EFI_SUCCESS Status returns if get card reader name successfully.

**/
EFI_STATUS
GetSmartCardReaderName (
    EFI_HANDLE    ControllerHandle,
    CHAR16        *ReaderName,
    UINTN         *ReaderNameLength
)
{
    EFI_STATUS                 Status;
    EFI_USB_IO_PROTOCOL        *UsbIo;
    DEVGROUP_T                 *DevGroup;
    EFI_USB_STRING_DESCRIPTOR  *StringDesc;
    USBDEV_T                   *CcidDev;

    // Get UsbIo Interface pointer
    Status = gBS->HandleProtocol ( ControllerHandle,
                                   &gEfiUsbIoProtocolGuid,
                                   (VOID**)&UsbIo );
    if( EFI_ERROR(Status)) {
        return Status;
    }

    CcidDev = UsbIo2Dev(UsbIo);

    DevGroup = (DEVGROUP_T*)CcidDev->node.parent->data;

    StringDesc = DevGroup->ProductStrDesc;

    if( StringDesc->DescriptorType == DESC_TYPE_STRING && StringDesc->Length > 2 ) {

        if( *ReaderNameLength < StringDesc->Length ) {
            Status = EFI_BUFFER_TOO_SMALL;
        } else {
           CopyMem(ReaderName, StringDesc->String, StringDesc->Length-2 );
           ReaderName[((StringDesc->Length-2)>>1)] = L'\0';
        }

       *ReaderNameLength = StringDesc->Length;
    }

    return Status;
}

/**
    Search the linked list to find the ICC_DEVICE for the given slot

    @param *FpDevInfo      Ptr to device info
    @param Slot            slot number

    @retval Pointer        ICC_DEVICE Pointer

**/
ICC_DEVICE*
GetICCDevice(
    DEV_INFO        *FpDevInfo, 
    UINT8            Slot
)
{

    ICC_DEVICE        *fpICCDevice;
    LIST_ENTRY        *Link;
    CCID_DEV_INFO     *CcidDevData;
    
    CcidDevData =  (CCID_DEV_INFO*)FpDevInfo->SpecificDevData;
    if (!CcidDevData) return NULL;


    for (Link = CcidDevData->IccDeviceList.ForwardLink;
        Link != &CcidDevData->IccDeviceList; 
        Link = Link->ForwardLink ) {
        fpICCDevice = BASE_CR(Link, ICC_DEVICE, Link);
        //
        // Slot # matches
        //
        if (fpICCDevice->Slot == Slot) {
            return fpICCDevice;
        }      
          
    }

    //
    // No Device Found. Return with NULL pointer
    //
    return NULL;

}

/**
    Search the linked list to find the CCID Device for the given ICC

    @param fpICCDevice     ICC device

    @retval Pointer        Ptr to DEV_INFO

**/
DEV_INFO*
GetCCIDDevice(
    ICC_DEVICE        *fpICCDevice
)
{


    DEV_INFO        *fpCCIDDevice;
    ICC_DEVICE      *fpTempICCDevice;
    LIST_ENTRY      *Link;
    UINTN           Index;
    CCID_DEV_INFO     *CcidDevData;
   

    for (Index = 0; Index < gUsbData->MaxDevCount; Index++ ){

        fpCCIDDevice = &(gUsbDataList->DevInfoTable[Index]);
        
        //
        // If not a CCID device continue the loop
        //
        if (!(fpCCIDDevice->DeviceType == BIOS_DEV_TYPE_CCID)) continue;
        CcidDevData =  (CCID_DEV_INFO*)fpCCIDDevice->SpecificDevData;

            for (Link = CcidDevData->IccDeviceList.ForwardLink;
                Link != &CcidDevData->IccDeviceList; 
                Link = Link->ForwardLink ) {
                fpTempICCDevice = BASE_CR(Link, ICC_DEVICE, Link);

                //    
                // Match found?
                //
                if (fpICCDevice == fpTempICCDevice) {
                    return fpCCIDDevice;
                }      
          
            }

    }

    //
    // Device Not Found, Return with NULL
    //
    return NULL;
}

/**
    Convert CCID return Value to EFI_STATUS

    @param bRetValue     Return value

    @retval EFI_STATUS   Return the EFI Status

**/

EFI_STATUS
GetReturnValue(
    UINT8    bRetValue
)
{
    EFI_STATUS    Status;

    switch (bRetValue) {

            case USB_SUCCESS:
                Status = EFI_SUCCESS;
                break;

            default:
                Status = EFI_DEVICE_ERROR;
    }

    return Status;
}

/**
    Timer call-back routine that is used to monitor insertion/removal 
    of ICC(Smart card) in the smart card reader.

     
    @param EFI_EVENT   Event,
    @param VOID        *Context

**/

VOID
EFIAPI
IccOnTimer(
    EFI_EVENT   Event,
    VOID        *Context
)
{
    ICC_DEVICE        *IccDevice = NULL;
    DEV_INFO          *CcidDevice = NULL; 
    UINTN              Lock;
    EFI_HANDLE        CcidHandle;

    ATOMIC({Lock = gICCLock; gICCLock=1;});

    if( Lock ){
        USB_DEBUG(DEBUG_WARN, DEBUG_LEVEL_USBBUS, "ICCOnTimer::  is locked; return\n" );
        return;
    }

    do {

        ATOMIC({IccDevice = (ICC_DEVICE *)QueueGet(&gUsbDataList->ICCQueueCnnctDisc);});

        if (IccDevice == NULL) { 
            break;
        }

        CcidDevice = GetCCIDDevice(IccDevice);

        if (!CcidDevice) {
            continue;
        }

        CcidHandle = (EFI_HANDLE) *(UINTN*)CcidDevice->Handle;

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Controller %lx\n", CcidHandle);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Insert/Removal: FpDevInfo %lx IccDevice %lx IccDevice->ChildHandle %lx IccDevice->ConfiguredStatus %x\n", \
                CcidDevice, IccDevice, IccDevice->ChildHandle, IccDevice->ConfiguredStatus );

      if ((gUsbData->UsbFeature & USB_CCID_USE_INT_INS_REMOVAL) == USB_CCID_USE_INT_INS_REMOVAL){
        if (!IccDevice->ChildHandle && (IccDevice->ConfiguredStatus & ICCPRESENT)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "InstallUSBCCID ....\n" );
            //
            // Install EFI interface to communicate with Smart Card/CCID
            //
            InstallUSBCCID(CcidHandle, CcidDevice, IccDevice);
        }

        if (IccDevice->ConfiguredStatus & CARDREMOVED) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "UnInstallUSBCCID ....\n" );
            //
            // Device Removed. Uninstall the existing Device
            //
             UnInstallUsbCcid (CcidHandle, IccDevice->ChildHandle);
             // Indication to the SMI handler that the CARD has been uninstalled.
             // This Handle will be checked before device is inserted into the queue.
             IccDevice->ChildHandle = NULL;
             
        }
      }else{

        InstallUSBCCID(CcidHandle, CcidDevice, IccDevice);     
      }


        // When card is removed, ScardChildHandle will still be valid so it will not do anything.
        // If the handle is valid, it means the protocol is already installed
        if (!IccDevice->SCardChildHandle) {
            InstallUSBSCardReaderProtocolOnSlot(CcidHandle, CcidDevice, IccDevice);
        }
    } while ( 1 );

    gICCLock = 0;

    return;
}

/**
    Verifies if usb CCID support can be installed on a device

        
    @param This                 pointer to driver binding protocol
    @param ControllerHandle     controller handle to install driver on
    @param RemainingDevicePath  pointer to device path

         
    @retval EFI_SUCCESS driver supports given controller
    @retval EFI_UNSUPPORTED driver doesn't support given controller

**/

EFI_STATUS
EFIAPI
SupportedUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath
)
{
    EFI_USB_INTERFACE_DESCRIPTOR    Desc;
    EFI_STATUS                      Status;
    EFI_USB_IO_PROTOCOL             *UsbIo;
    DEV_INFO                        *DevInfo;

    Status = gBS->OpenProtocol (Controller, &gEfiUsbIoProtocolGuid,
                                (VOID**)&UsbIo, This->DriverBindingHandle,
                                Controller, EFI_OPEN_PROTOCOL_GET_PROTOCOL 
                                );

    if (EFI_ERROR(Status)) {
        return Status;
    }

    DevInfo = FindDevStruc(Controller);

    if (DevInfo == NULL) {
        return EFI_UNSUPPORTED;
    }

    Status = UsbIo->UsbGetInterfaceDescriptor(UsbIo, &Desc);

    if(EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    if ( Desc.InterfaceClass == BASE_CLASS_CCID_STORAGE &&
        Desc.InterfaceSubClass == SUB_CLASS_CCID &&
        Desc.InterfaceProtocol == PROTOCOL_CCID) {
        return EFI_SUCCESS;
    } else {
        return EFI_UNSUPPORTED;
    }

}

/**
    Installs CCID protocol on a given handle

           
    @param This                pointer to driver binding protocol
    @param ControllerHandle    controller handle to install driver on
    @param RemainingDevicePath pointer to device path

         
    @retval EFI_SUCCESS       driver started successfully
    @retval EFI_UNSUPPORTED   driver didn't start

**/

EFI_STATUS
EFIAPI
StartUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath
)
{
    EFI_STATUS              Status;
    EFI_USB_IO_PROTOCOL     *UsbIo;

    USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL,
        "USB: installUSBCCID: starting...Controller : %lx\n", Controller);

    //
    // Open Protocols
    //
    //ALREADY_STARTED is a normal condition
    Status = gBS->OpenProtocol ( Controller,  &gEfiUsbIoProtocolGuid,
                                (VOID**)&UsbIo, This->DriverBindingHandle,
                                Controller, EFI_OPEN_PROTOCOL_BY_DRIVER );
    if( EFI_ERROR(Status)) {
        return Status;
    }

    {
        USBDEV_T* CCIDDev = UsbIo2Dev(UsbIo);
        HC_STRUC* HcData;
        UINT8     UsbStatus;

        ASSERT(CCIDDev);
        HcData = gHcTable[CCIDDev->dev_info->HcNumber - 1];
        UsbStatus = UsbSmiReConfigDevice(HcData, CCIDDev->dev_info);

        if(UsbStatus != USB_SUCCESS) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_USBHC_LEVEL,
                "installUSBCCID: failed to Reconfigure CCID: %d\n", UsbStatus );

            return EFI_DEVICE_ERROR;
        }

        //
        // Install AMI CCID Presence protocol to identify SMART Card reader is detected
        //
        Status = gBS->InstallProtocolInterface(
                            &Controller,
                            &gAmiCCIDPresenceGuid,
                            EFI_NATIVE_INTERFACE,
                            NULL
                            );

        ASSERT_EFI_ERROR(Status);

        //
        // Setting up Timer to to detect ICC card insertion removal 
        //
        gCounterCCIDEnumTimer++;
        if (gEvICCEnumTimer == 0) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: Start: setup timer callback %x\n", &IccOnTimer );
            gBS->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL,
                                    TPL_CALLBACK, IccOnTimer,0,&gEvICCEnumTimer);
            gBS->SetTimer(gEvICCEnumTimer, TimerPeriodic, MILLISECOND);
        }

    }

    USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL,
        "USB: installCCID: done (%x).\n", Status);


    return Status;
}


/**
    Uninstalls CCID protocol on a given handle

           
    @param This               pointer to driver binding protocol
    @param ControllerHandle   controller handle to install driver on
    @param NumberOfChildren   number of childs on this handle
    @param ChildHandleBuffer  pointer to child handles array

         
    @retval EFI_SUCCESS             driver stopped successfully
    @retval EFI_INVALID_PARAMETER   invalid values passed for NumberOfChildren or
                                    ChildHandleBuffer
**/

EFI_STATUS
EFIAPI
StopUsbCcid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    UINTN                       NumberOfChildren,
    EFI_HANDLE                  *Children
)
{

    EFI_STATUS Status;

    if (!NumberOfChildren) {

        // Closer the timer event when all CCID devices have been stopped
        if(--gCounterCCIDEnumTimer==0){

            gBS->SetTimer(gEvICCEnumTimer, TimerCancel, ONESECOND);
            gBS->CloseEvent(gEvICCEnumTimer);
            gEvICCEnumTimer=0;

            USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL, "USB: Timer Stopped\n");
        }

        //
        // uninstall gAmiCCIDPresenceGuid 
        //
        Status = gBS->UninstallProtocolInterface ( Controller, 
                                                &gAmiCCIDPresenceGuid, 
                                                NULL);
        if (EFI_ERROR(Status)) {
            return Status;
        }
        Status = gBS->CloseProtocol (Controller, 
                                    &gEfiUsbIoProtocolGuid, 
                                    This->DriverBindingHandle, 
                                    Controller);
        ASSERT_EFI_ERROR(Status);
        
        USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL, "USB: StopUSBCCID: done : %r gCounterCCIDEnumTimer : %x\n", \
                 Status,  gCounterCCIDEnumTimer);
    }

    while (NumberOfChildren){

        //
        // Uninstall AMI_CCID_IO_PROTOCOL for each slot
        //
        Status = UnInstallUsbCcid (Controller, Children[NumberOfChildren -  1]);

        if( EFI_ERROR(Status) ) {
            Status = UnInstallScardProtocol (Controller, Children[NumberOfChildren -  1], This->DriverBindingHandle);
        }
        ASSERT_EFI_ERROR(Status);

        NumberOfChildren--;

    }

    return Status;

}


/**
    CCID driver entry point

    @param  ImageHandle    The firmware allocated handle for the EFI image.  
    @param  SystemTable    A pointer to the EFI System Table.

         
    @retval EFI_STATUS     Efi Status.

**/

EFI_STATUS
UsbCCIDInit(
    EFI_HANDLE  ImageHandle,
    EFI_HANDLE  ServiceHandle
)
{
    EFI_STATUS    Status;
    CcidBindingProtocol.DriverBindingHandle = ServiceHandle;
    CcidBindingProtocol.ImageHandle = ImageHandle;


    gUsbDataList->ICCQueueCnnctDisc.data = gUsbDataList->ICCQueueData;
    gUsbDataList->ICCQueueCnnctDisc.maxsize = COUNTOF(gUsbDataList->ICCQueueData);
    gUsbDataList->ICCQueueCnnctDisc.head = 0;
    gUsbDataList->ICCQueueCnnctDisc.tail = 0;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB CCID binding:\n\t");
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "SupportedUSBCCID:%x\n", &SupportedUsbCcid );
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\tInstallUSBCCID:%x\n", &StartUsbCcid);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\tUninstallUSBCCID:%x\n", &StopUsbCcid );


    // Install driver binding protocol here
    Status = EfiLibInstallDriverBindingComponentName2 (
                 ImageHandle,
                 gST,
                 &CcidBindingProtocol,
                 CcidBindingProtocol.DriverBindingHandle,
                 NULL,
                 &gComponentNameUsbCcid
                 );
    return Status;

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
