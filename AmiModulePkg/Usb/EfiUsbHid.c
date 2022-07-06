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

/** @file EfiUsbHid.c
    EFI USB HID device Driver

**/

#include "Uhcd.h"
#include "EfiUsbKb.h"
#include <UsbKbd.h>

#include "ComponentName.h"
#include "UsbBus.h"
#include <Protocol/AmiUsbHid.h>
#include <Library/TimerLib.h>

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

/**
    USB EFI keyboard driver driver protocol function that
    returns the keyboard controller name.

    @param Controller       The handle of a controller that the driver specified by
                            This is managing.  This handle specifies the controller
                            whose name is to be returned.
    @param Child            The handle of the child controller to retrieve the name
                            of.  This is an optional parameter that may be NULL.  It
                            will be NULL for device drivers.  It will also be NULL
                            for a bus drivers that wish to retrieve the name of the
                            bus controller.  It will not be NULL for a bus driver
                            that wishes to retrieve the name of a child controller.
                            Language         - A pointer to a three character ISO 639-2 language
                            identifier.  This is the language of the controller name
                            that that the caller is requesting, and it must match one
                            of the languages specified in SupportedLanguages.  The
                            number of languages supported by a driver is up to the
                            driver writer.
    @retval NULL            No controller name

**/

CHAR16*
UsbHidGetControllerName(
    EFI_HANDLE Controller,
    EFI_HANDLE Child
)
{
    return NULL;
}
 

EFI_DRIVER_BINDING_PROTOCOL gUsbHidDriverBinding = {
        SupportedUSBHid,
        InstallUSBHid,
        UninstallUSBHid,
        USBKB_DRIVER_VERSION,
        NULL,
        NULL };


/**
    HID EFI driver entry point
    
    @param ImageHandle      Image handle
    @param ServiceHandle    Binding handle

    @retval EFI_SUCCESS     Sucess to initial HID device
**/

EFI_STATUS
EFIAPI
UsbHidInit(
    EFI_HANDLE  ImageHandle,
    EFI_HANDLE  ServiceHandle
)
{
    EFI_STATUS        Status;

    gUsbHidDriverBinding.DriverBindingHandle = ServiceHandle;
    gUsbHidDriverBinding.ImageHandle = ImageHandle;

    InitUSBMouse(); 

    // Install driver binding protocol here
    Status = EfiLibInstallDriverBindingComponentName2 (
                 ImageHandle,
                 gST,
                 &gUsbHidDriverBinding,
                 gUsbHidDriverBinding.DriverBindingHandle,
                 NULL,
                 &gComponentNameUsbHid
                 );
    return Status;
}
 
/**
    Verifies if usb hid support can be installed on a device

    @param This                 Protocol instance pointer
    @param Controller           Handle of device to test
    @param DevicePath           Optional parameter use to pick a specific child
                                device to start.

    @retval EFI_SUCCESS         This driver supports this device.
    @retval EFI_UNSUPPORTED     This driver does not support this device.
**/

EFI_STATUS
EFIAPI
SupportedUSBHid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath)
{
    EFI_USB_INTERFACE_DESCRIPTOR    Desc;
    EFI_STATUS                      Status;
    EFI_USB_IO_PROTOCOL             *UsbIo;
    DEV_INFO                        *DevInfo;

    Status = gBS->OpenProtocol(Controller, &gEfiUsbIoProtocolGuid,
                    (VOID**)&UsbIo, This->DriverBindingHandle,
                    Controller, EFI_OPEN_PROTOCOL_BY_DRIVER);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gBS->CloseProtocol(Controller, &gEfiUsbIoProtocolGuid,
        This->DriverBindingHandle, Controller);

    DevInfo = FindDevStruc(Controller);

    if (DevInfo == NULL) {
        return EFI_UNSUPPORTED;
    }
    
    if (DevInfo->Flag & DEV_INFO_DEV_UNSUPPORTED) {
        return EFI_UNSUPPORTED;
    }

    Status = UsbIo->UsbGetInterfaceDescriptor(UsbIo, &Desc);
    
    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    if (Desc.InterfaceClass == BASE_CLASS_HID) {
        return EFI_SUCCESS;
    } else {
        return EFI_UNSUPPORTED;
    }
} 

/**
    Installs SimpleTxtIn protocol on a given handle

    @param This                 Protocol instance pointer.
    @param Controller           Handle of device to test
    @param DevicePath           Optional parameter use to pick a specific child
                                device to start.

    @retval EFI_SUCCESS        This driver supports this device.
    @retval EFI_UNSUPPORTED    This driver does not support this device.
    @retval EFI_DEVICE_ERROR   This driver cannot be started due to device

**/

EFI_STATUS
EFIAPI
InstallUSBHid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePath
)
{
    EFI_STATUS                  Status;
    EFI_USB_IO_PROTOCOL         *UsbIo;
    USBDEV_T                    *HidDev;
    HC_STRUC                    *HcData;
    UINT8                       UsbStatus;
    AMI_USB_HID_PROTOCOL        *AmiUsbHidProtocol;
 
    USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL,
        "USB: InstallUSBHid: starting...\n");
    //
    // Open Protocols
    //
    Status = gBS->OpenProtocol ( Controller,  &gEfiUsbIoProtocolGuid,
                    (VOID**)&UsbIo, This->DriverBindingHandle,
                    Controller, EFI_OPEN_PROTOCOL_BY_DRIVER );
    if (EFI_ERROR(Status))
        return Status; 
        
    HidDev = UsbIo2Dev(UsbIo);      

    ASSERT(HidDev);

    // Get Performance counter
    if ((gUsbData->UsbFeature2 & USB_PERFORMANCE_INFORMATION) && (!(gUsbData->UsbStateFlag & USB_FLAG_BOOT_EVENT))){
        HidDev->dev_info->ConfigStartTime = GetPerformanceCounter();
    }
    
    HcData = gHcTable[HidDev->dev_info->HcNumber - 1];
    UsbStatus = UsbSmiReConfigDevice(HcData, HidDev->dev_info); 
    if(UsbStatus != USB_SUCCESS) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_USBHC_LEVEL,
            "InstallUSBHid: failed to Reconfigure Hid: %d\n", UsbStatus );
        gBS->CloseProtocol(
               Controller, &gEfiUsbIoProtocolGuid,
               This->DriverBindingHandle, Controller);
        return EFI_DEVICE_ERROR;
    }

    if (HidDev->dev_info->HidDevType == 0) {
        HidDev->dev_info->Flag |= DEV_INFO_DEV_UNSUPPORTED;
        gBS->CloseProtocol(Controller, &gEfiUsbIoProtocolGuid,
               This->DriverBindingHandle, Controller);
        return EFI_UNSUPPORTED;
    }

    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(AMI_USB_HID_PROTOCOL),
                    (VOID**)&AmiUsbHidProtocol);

    if (EFI_ERROR(Status)) {
        gBS->CloseProtocol(
               Controller, &gEfiUsbIoProtocolGuid,
               This->DriverBindingHandle, Controller);
        return Status;
    }

    AmiUsbHidProtocol->HidDevType = HidDev->dev_info->HidDevType;

    Status = gBS->InstallMultipleProtocolInterfaces (
                      &Controller,
                      &gAmiUsbHidProtocolGuid,
                      AmiUsbHidProtocol, 
                      NULL
                      );
    
    if (HidDev->dev_info->HidDevType & HID_DEV_TYPE_KEYBOARD) {
        InstallUsbKeyboard(This,Controller,DevicePath,HidDev->dev_info,UsbIo);
    }

    if (HidDev->dev_info->HidDevType & (HID_DEV_TYPE_MOUSE | HID_DEV_TYPE_POINT)) {
        if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_REPORT_PROTOCOL) {
            if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_RELATIVE_DATA) {
                InstallUSBMouse(Controller, UsbIo, HidDev->dev_info);
            }
            if ((gUsbData->UsbDevSupport & USB_POINT_DEV_SUPPORT) == USB_POINT_DEV_SUPPORT)
                if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_ABSOLUTE_DATA) {
                    InstallUSBAbsMouse(Controller, HidDev->dev_info);
            }
        } else {
            InstallUSBMouse(Controller, UsbIo, HidDev->dev_info);
        }
    }
    return Status;

} 

/**
    Uninstalls protocol on a given handle

    @param This              Protocol instance pointer.
    @param Controller        Handle of device to stop driver on
    @param NumberOfChildren  Number of Children in the ChildHandleBuffer
    @param Children          List of handles for the children we
                             need to stop.
    @retval EFI_SUCCESS      This driver is removed ControllerHandle
    @retval other            This driver was not removed from this device

**/

EFI_STATUS
EFIAPI
UninstallUSBHid(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    UINTN                       NumberOfChildren,
    EFI_HANDLE                  *Children
)
{
    EFI_STATUS                                  Status;
    EFI_USB_IO_PROTOCOL         *UsbIo;
    AMI_USB_HID_PROTOCOL        *AmiUsbHidProtocol;
    UINT8                           UsbSatus;
    USBDEV_T*                       HidDev;
    HC_STRUC*                       HcData;
    
    USB_DEBUG(DEBUG_INFO, DEBUG_USBHC_LEVEL,
            "\n USB: UnInstallUSBHid: stoping...\n");
    
    Status = gBS->OpenProtocol (
                      Controller,
                      &gAmiUsbHidProtocolGuid,
                      (VOID**)&AmiUsbHidProtocol,
                      This->DriverBindingHandle,
                      Controller,
                      EFI_OPEN_PROTOCOL_GET_PROTOCOL
                      );
    
    if (EFI_ERROR(Status)) {
            return Status; 
    }
    //
    // Open Protocols
    //
    Status = gBS->OpenProtocol(Controller, &gEfiUsbIoProtocolGuid,
                    (VOID**)&UsbIo, This->DriverBindingHandle,
                    Controller, EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    if (EFI_ERROR(Status)) {
        return Status; 
    }
    
    HidDev = UsbIo2Dev(UsbIo);      
    HcData = gHcTable[HidDev->dev_info->HcNumber - 1];
    
    UsbSatus = UsbDevDriverDisconnect(HcData, HidDev->dev_info);
    ASSERT(UsbSatus == USB_SUCCESS);
    
    if (HidDev->dev_info->HidDevType & HID_DEV_TYPE_KEYBOARD) {
        Status = UninstallUsbKeyboard(This,Controller,NumberOfChildren,Children);
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    if (HidDev->dev_info->HidDevType & (HID_DEV_TYPE_MOUSE | HID_DEV_TYPE_POINT)) {
        if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_REPORT_PROTOCOL) {
            if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_RELATIVE_DATA) {
                Status = UninstallUSBMouse(This, Controller, NumberOfChildren, Children);
                if (EFI_ERROR(Status)) {
                    return Status;
                }
            }
    
            if ((gUsbData->UsbDevSupport & USB_POINT_DEV_SUPPORT) == USB_POINT_DEV_SUPPORT){
                if (HidDev->dev_info->HidReport.Flag & HID_REPORT_FLAG_ABSOLUTE_DATA) {
                    Status = UninstallUSBAbsMouse(This, Controller, NumberOfChildren, Children);
                    if (EFI_ERROR(Status)) {
                        return Status;
                    }
                }
            }
    
        } else {
        
            Status = UninstallUSBMouse(This, Controller, NumberOfChildren, Children);
            if (EFI_ERROR(Status)) {
                return Status;
            }
        }
    }
    
    Status = gBS->UninstallMultipleProtocolInterfaces (
                      Controller,
                      &gAmiUsbHidProtocolGuid,
                      AmiUsbHidProtocol, 
                      NULL
                      );
    
    if (EFI_ERROR(Status)) {
            return Status;
    }
    
    gBS->FreePool(AmiUsbHidProtocol);
    
    //Close usbio protocol
    Status = gBS->CloseProtocol(Controller, &gEfiUsbIoProtocolGuid,
                    This->DriverBindingHandle, Controller); 

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
