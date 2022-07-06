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

/** @file EfiUsbPoint.c
    EFI USB Absolute pointer Driver

**/

#include "Uhcd.h"
#include "ComponentName.h"
#include "UsbBus.h"
#include <Library/AmiUsbHcdLib.h>
#include <Library/PrintLib.h>

#include <Protocol/AbsolutePointer.h>
#define USB_ABSOLUTE_MOUSE_DRIVER_VERSION 1

#define USB_ABSOLUTE_MOUSE_DEV_SIGNATURE   EFI_SIGNATURE_32('u','a','b','s')
#define USB_ABSOLUTE_MOUSE_DEV_FROM_ABSOLUTE_PROTOCOL(a) \
    CR(a, USB_ABSOLUTE_MOUSE_DEV, AbsolutePointerProtocol, USB_ABSOLUTE_MOUSE_DEV_SIGNATURE)

typedef struct
{
    UINTN                           Signature;
    EFI_ABSOLUTE_POINTER_PROTOCOL   AbsolutePointerProtocol;
    EFI_ABSOLUTE_POINTER_STATE      State;
    EFI_ABSOLUTE_POINTER_MODE       Mode;
    BOOLEAN                         StateChanged;
    VOID                            *DevInfo;
} USB_ABSOLUTE_MOUSE_DEV;

VOID EFIAPI UsbAbsMouseWaitForInput (EFI_EVENT, VOID*);
EFI_STATUS UpdateUsbAbsMouseData(USB_ABSOLUTE_MOUSE_DEV      *,ABS_MOUSE*);

//
// ABS Protocol
//
EFI_STATUS
EFIAPI
GetAbsMouseState(
  IN   EFI_ABSOLUTE_POINTER_PROTOCOL  *This,
  OUT  EFI_ABSOLUTE_POINTER_STATE     *AbsState
);

EFI_STATUS
EFIAPI
UsbAbsMouseReset(
  IN EFI_ABSOLUTE_POINTER_PROTOCOL      *This,
  IN BOOLEAN                            ExtendedVerification
  );

extern USB_GLOBAL_DATA *gUsbData;

/**
    Installs ABSOLUTEPointerProtocol interface on a given controller.

    @param Controller    controller handle to install interface on.
    @param DevInfo       Ptr to DEV_INFO

    @retval EFI_SUCCESS   Success to install point devcie

**/

EFI_STATUS
InstallUSBAbsMouse(
    EFI_HANDLE      Controller,
    DEV_INFO        *DevInfo
)
{
    EFI_STATUS                Status;
    USB_ABSOLUTE_MOUSE_DEV    *UsbAbsMs;
    CHAR8                     UsbAbsToken[USB_TOKEN_NAME_SIZE];

    Status = gBS->AllocatePool(EfiBootServicesData,
                    sizeof(USB_ABSOLUTE_MOUSE_DEV),
                    (VOID**)&UsbAbsMs);
        
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    SetMem(UsbAbsMs, sizeof(USB_ABSOLUTE_MOUSE_DEV), 0);

    //
    // Initialize UsbABSDevice
    //
    UsbAbsMs->Signature = USB_ABSOLUTE_MOUSE_DEV_SIGNATURE;

    UsbAbsMs->AbsolutePointerProtocol.GetState = GetAbsMouseState;
    UsbAbsMs->AbsolutePointerProtocol.Reset = UsbAbsMouseReset;
    UsbAbsMs->AbsolutePointerProtocol.Mode = &UsbAbsMs->Mode;
    UsbAbsMs->Mode.Attributes = EFI_ABSP_SupportsPressureAsZ;

    UsbAbsMs->Mode.AbsoluteMinX = 0;
    UsbAbsMs->Mode.AbsoluteMinY = 0;
    UsbAbsMs->Mode.AbsoluteMinZ = 0;
    UsbAbsMs->Mode.AbsoluteMaxX = DevInfo->HidReport.AbsMaxX;
    UsbAbsMs->Mode.AbsoluteMaxY = DevInfo->HidReport.AbsMaxY;
    UsbAbsMs->Mode.AbsoluteMaxZ = 0;

    UsbAbsMs->DevInfo = DevInfo;

    SetMem(&UsbAbsMs->State, sizeof(EFI_ABSOLUTE_POINTER_STATE), 0);
    SetMem(&DevInfo->AbsMouseData, sizeof(ABS_MOUSE), 0);

    UsbAbsMs->StateChanged = FALSE;
    
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_WAIT,
                    TPL_NOTIFY,
                    UsbAbsMouseWaitForInput,
                    UsbAbsMs,
                    &((UsbAbsMs->AbsolutePointerProtocol).WaitForInput)
                    );
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "ABS event is created, status = %r\n", Status);
        
    ASSERT(Status == EFI_SUCCESS);
    //
    // Install protocol interfaces for the USB ABS device
    //
    Status = gBS->InstallProtocolInterface(
                    &Controller,
                    &gEfiAbsolutePointerProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &UsbAbsMs->AbsolutePointerProtocol);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "ABS protocol is installed, status = %r\n", Status);

    // Init Detect Usb Abs token
    AsciiSPrintUnicodeFormat(UsbAbsToken, USB_TOKEN_NAME_SIZE, L"DetectUsbAbsMs:%X/%X", DevInfo->VendorId, DevInfo->DeviceId);

    // PERF_START
    AmiUsbPerfStart(NULL, UsbAbsToken, "AmiUsb", DevInfo->DetectStartTime);

    // PERF_END
    AmiUsbPerfEnd(NULL, UsbAbsToken, "AmiUsb", DevInfo->DetectEndTime);

    // Init Config Usb Abs token
    AsciiSPrintUnicodeFormat(UsbAbsToken, USB_TOKEN_NAME_SIZE, L"ConfigUsbAbsMs:%X/%X", DevInfo->VendorId, DevInfo->DeviceId);

    // PERF_START
    AmiUsbPerfStart(NULL, UsbAbsToken, "AmiUsb", DevInfo->ConfigStartTime);

    // PERF_END
    AmiUsbPerfEnd(NULL, UsbAbsToken, "AmiUsb", 0);

   
    ASSERT(Status == EFI_SUCCESS);
        
    return Status;
}


/**
    Uninstalls ABSOLUTEPointerProtocol interface.

    @param This                Pointer to driver binding protocol
    @param Controller          Controller handle.
    @param NumberOfChildren    Number of children in driver binding
    @param Children            Pointer to children handle
            
    @retval EFI_STATUS         Status of the operation

**/

EFI_STATUS
UninstallUSBAbsMouse(
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE Controller,
    UINTN NumberOfChildren,
    EFI_HANDLE *Children
)
{
    USB_ABSOLUTE_MOUSE_DEV          *UsbAbsMs;
    EFI_ABSOLUTE_POINTER_PROTOCOL   *AbsPointer = NULL;
    EFI_STATUS Status;

    Status = gBS->OpenProtocol(Controller,
                    &gEfiAbsolutePointerProtocolGuid,
                    (VOID**)&AbsPointer,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    UsbAbsMs = USB_ABSOLUTE_MOUSE_DEV_FROM_ABSOLUTE_PROTOCOL(AbsPointer);

    Status = gBS->UninstallProtocolInterface(
                    Controller,
                    &gEfiAbsolutePointerProtocolGuid,
                    &UsbAbsMs->AbsolutePointerProtocol);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->CloseEvent(
                    (UsbAbsMs->AbsolutePointerProtocol).WaitForInput);
            
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    gBS->FreePool(UsbAbsMs);
    
    return Status;
}




/**
    This routine is a part of ABSOLUTEPointerProtocol implementation;
    it resets USB ABS.

    @param This                 A pointer to the EFI_ABSOLUTE_POINTER_PROTOCOL instance.
    @param ExtendedVerification Indicates that the driver may perform
                                a more exhaustive verification operation of the device during
                                reset.

    @retval EFI_SUCCESS         Success to reset point device

**/

EFI_STATUS
EFIAPI
UsbAbsMouseReset(
    IN EFI_ABSOLUTE_POINTER_PROTOCOL    *This,
    IN BOOLEAN                          ExtendedVerification
  )
{
    USB_ABSOLUTE_MOUSE_DEV          *UsbAbsMs;
    DEV_INFO                        *DevInfo;
    EFI_TPL                         OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    UsbAbsMs = USB_ABSOLUTE_MOUSE_DEV_FROM_ABSOLUTE_PROTOCOL(This);
    DevInfo = (DEV_INFO*)UsbAbsMs->DevInfo;

    if (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    SetMem(&UsbAbsMs->State, sizeof(EFI_ABSOLUTE_POINTER_STATE), 0);
    
    UsbAbsMs->StateChanged = FALSE;

    SetMem(&DevInfo->AbsMouseData, sizeof(ABS_MOUSE), 0);

    gBS->RestoreTPL(OldTpl);
    
    return EFI_SUCCESS;
}


/**
    This routine is a part of ABSOLUTEPointerProtocol implementation;
    it retrieves the current state of a pointer device.

    @param This      A pointer to the EFI_ABSOLUTE_POINTER_PROTOCOL instance.
    @param ABSState  A pointer to the state information on the pointer
                     device.

    @retval EFI_SUCCESS  The state of the pointer device was returned
                         in ABSState.
    @retval EFI_NOT_READY The state of the pointer device has not changed
                         since the last call to GetABSState().
    @retval EFI_DEVICE_ERROR A device error occurred while attempting to
                         retrieve the current state of absolute mouse
**/

EFI_STATUS
EFIAPI
GetAbsMouseState(
    EFI_ABSOLUTE_POINTER_PROTOCOL  *This,
    EFI_ABSOLUTE_POINTER_STATE     *AbsMouseState
)
{
    USB_ABSOLUTE_MOUSE_DEV          *UsbAbsMs;
    DEV_INFO                        *DevInfo;
    EFI_TPL                         OldTpl;

    if (AbsMouseState == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    UsbAbsMs = USB_ABSOLUTE_MOUSE_DEV_FROM_ABSOLUTE_PROTOCOL(This);
    DevInfo = (DEV_INFO*)UsbAbsMs->DevInfo;

    if (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    UpdateUsbAbsMouseData(UsbAbsMs, &DevInfo->AbsMouseData);

    if (UsbAbsMs->StateChanged == FALSE) {
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_READY;
    }

    CopyMem(AbsMouseState, &UsbAbsMs->State, sizeof(EFI_ABSOLUTE_POINTER_STATE));
    
    UsbAbsMs->StateChanged = FALSE;

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}


/**
    This routine updates current AbsMouse data.

    @param UsbAbsMs    pointer to USB_ABSOLUTE_MOUSE_DEV to be updated.
    @param AbsData     pointer to ABS_MOUSE to be updated.
    
    @retval EFI_SUCCESS

**/

EFI_STATUS
UpdateUsbAbsMouseData (
    USB_ABSOLUTE_MOUSE_DEV          *UsbAbsMs,
    ABS_MOUSE                       *AbsData
)
{
    if (UsbAbsMs->State.CurrentX != (UINT64)AbsData->Xcoordinate ||
        UsbAbsMs->State.CurrentY != (UINT64)AbsData->Ycoordinate ||
        UsbAbsMs->State.CurrentZ != (UINT64)AbsData->Pressure ||
        UsbAbsMs->State.ActiveButtons != (UINT32)AbsData->ButtonStauts) {
        UsbAbsMs->StateChanged=TRUE;
    } else {
        UsbAbsMs->StateChanged=FALSE;
    }

    if (UsbAbsMs->StateChanged) {
        UsbAbsMs->State.ActiveButtons = (UINT32)AbsData->ButtonStauts;
        UsbAbsMs->State.CurrentX = (UINT64)AbsData->Xcoordinate;
        UsbAbsMs->State.CurrentY = (UINT64)AbsData->Ycoordinate;
        UsbAbsMs->State.CurrentZ = (UINT64)AbsData->Pressure; 
        UsbAbsMs->Mode.AbsoluteMaxX= (UINT64)AbsData->Max_X; 
        UsbAbsMs->Mode.AbsoluteMaxY= (UINT64)AbsData->Max_Y;  
    }

    return EFI_SUCCESS;
}


/**
    Event notification function for AbsMouseOLUTE_POINTER.WaitForInput
    event. Signal the event if there is input from AbsMouse.

    @param Event    event to signal in case of AbsMouse activity
    @param Context  data to pass along with the event.

**/

VOID
EFIAPI
UsbAbsMouseWaitForInput (
    EFI_EVENT   Event,
    VOID        *Context
)
{
    USB_ABSOLUTE_MOUSE_DEV          *UsbAbsMs;
    DEV_INFO                        *DevInfo;
    EFI_TPL                         OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    UsbAbsMs = (USB_ABSOLUTE_MOUSE_DEV*)(Context);
    DevInfo = (DEV_INFO*)UsbAbsMs->DevInfo;

    if (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return;
    }
    
    //
    // Someone is waiting on the AbsMouse event, if there's
    // input from AbsMouse, signal the event
    //
    if (UsbAbsMs->State.CurrentX != (UINT64)DevInfo->AbsMouseData.Xcoordinate ||
       UsbAbsMs->State.CurrentY != (UINT64)DevInfo->AbsMouseData.Ycoordinate ||
       UsbAbsMs->State.CurrentZ != (UINT64)DevInfo->AbsMouseData.Pressure ||
       UsbAbsMs->State.ActiveButtons != (UINT32)DevInfo->AbsMouseData.ButtonStauts) {
        gBS->SignalEvent(Event);
    }

    gBS->RestoreTPL(OldTpl);

    return;
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
