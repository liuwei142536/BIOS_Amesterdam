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

/** @file EfiUsbMs.c
    EFI USB Mouse Driver

**/

#include "Uhcd.h"
#include "ComponentName.h"
#include "UsbBus.h"
#include <Library/AmiUsbHcdLib.h>
#include <Library/PrintLib.h>

#define USBMS_DRIVER_VERSION 2

#define USB_MOUSE_DEV_SIGNATURE   EFI_SIGNATURE_32('u','m','o','u')

#define USB_MOUSE_DEV_FROM_MOUSE_PROTOCOL(a,b) \
    CR(a, USB_MOUSE_DEV, b, USB_MOUSE_DEV_SIGNATURE)

typedef struct {
    UINTN                           Signature;
    UINT8                           NumberOfButtons;
    INT32                           XLogicMax;
    INT32                           XLogicMin;
    INT32                           YLogicMax;
    INT32                           YLogicMin;
    EFI_SIMPLE_POINTER_PROTOCOL     SimplePointerProtocol;
    EFI_SIMPLE_POINTER_MODE         Mode;
    UINT8                           Endpoint;
    EFI_USB_IO_PROTOCOL             *UsbIo;
} USB_MOUSE_DEV;

VOID
EFIAPI
UsbMouseWaitForInput (
  IN  EFI_EVENT               Event,
  IN  VOID                    *Context
  );

EFI_STATUS
UpdateUsbMouseData (
    EFI_SIMPLE_POINTER_PROTOCOL  *This, 
        EFI_SIMPLE_POINTER_STATE        *State
  );

//
// Mouse Protocol
//
EFI_STATUS
EFIAPI
GetMouseState(
  IN   EFI_SIMPLE_POINTER_PROTOCOL  *This,
  OUT  EFI_SIMPLE_POINTER_STATE     *MouseState
);

EFI_STATUS
EFIAPI
UsbMouseReset(
  IN EFI_SIMPLE_POINTER_PROTOCOL    *This,
  IN BOOLEAN                        ExtendedVerification
  );

extern USB_GLOBAL_DATA *gUsbData;

BOOLEAN                         StateChanged; 
UINT8                           ButtonsState; 
EFI_SIMPLE_POINTER_STATE        MsState;



/**
    Initialize USB mouse device and all private data structures.

    @retval EFI_SUCCESS   Success to init USB mouse data.

**/

EFI_STATUS
InitUSBMouse()
{
    SetMem (&MsState, sizeof(EFI_SIMPLE_POINTER_STATE), 0);
    ButtonsState = 0;
    StateChanged = FALSE; 
    return EFI_SUCCESS;
}  

/**
    Installs SimplePointerProtocol interface on a given controller.

    @param Controller   controller handle to install interface on.
    @param DevInfo      Ptr to UsbIo
    @param DevInfo      Ptr to DEV_INFO

**/
VOID
InstallUSBMouse(
    EFI_HANDLE          Controller,
    EFI_USB_IO_PROTOCOL *UsbIo,
    DEV_INFO            *DevInfo
)
{
    USB_MOUSE_DEV   *UsbMouse; 
    EFI_STATUS      Status;
    CHAR8           UsbMsToken[USB_TOKEN_NAME_SIZE];

    Status = gBS->AllocatePool(EfiBootServicesData, sizeof(USB_MOUSE_DEV),
                    (VOID**)&UsbMouse);
    
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return;
    }

    REPORT_STATUS_CODE(EFI_PROGRESS_CODE,(EFI_PERIPHERAL_MOUSE | EFI_P_PC_INIT));
    SetMem(UsbMouse, sizeof(USB_MOUSE_DEV), 0);

    //
    // Initialize UsbMouseDevice
    //
    UsbMouse->Signature = USB_MOUSE_DEV_SIGNATURE;
    UsbMouse->SimplePointerProtocol.GetState = GetMouseState;
    UsbMouse->SimplePointerProtocol.Reset = UsbMouseReset;
    UsbMouse->SimplePointerProtocol.Mode = &UsbMouse->Mode;

    UsbMouse->NumberOfButtons = 2;
    UsbMouse->XLogicMax = UsbMouse->YLogicMax = 127;
    UsbMouse->XLogicMin = UsbMouse->YLogicMin = -127;

    UsbMouse->Mode.LeftButton = TRUE;
    UsbMouse->Mode.RightButton = TRUE;
    UsbMouse->Mode.ResolutionX = 8;
    UsbMouse->Mode.ResolutionY = 8;
    UsbMouse->Mode.ResolutionZ = 1; 

    UsbMouse->UsbIo = UsbIo;
    UsbMouse->Endpoint = DevInfo->IntInEndpoint;

    UsbMouseReset(NULL, FALSE);

    REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_PERIPHERAL_MOUSE | EFI_P_PC_PRESENCE_DETECT));
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_WAIT,
                    TPL_NOTIFY,
                    UsbMouseWaitForInput,
                    UsbMouse,
                    &((UsbMouse->SimplePointerProtocol).WaitForInput)
                    );
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "Mouse event is created, status = %r\n", Status);
    
    ASSERT(Status == EFI_SUCCESS);

    //
    // Install protocol interfaces for the USB mouse device
    //
    Status = gBS->InstallProtocolInterface(
                    &Controller,
                    &gEfiSimplePointerProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &UsbMouse->SimplePointerProtocol);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "Mouse protocol is installed, status = %r\n", Status);
    
    ASSERT(Status == EFI_SUCCESS);

    // Init Detect Usb Mouse token
    AsciiSPrintUnicodeFormat(UsbMsToken, USB_TOKEN_NAME_SIZE, L"DetectUsbMs:%X/%X", DevInfo->VendorId, DevInfo->DeviceId);

    // PERF_START
    AmiUsbPerfStart(NULL, UsbMsToken, "AmiUsb", DevInfo->DetectStartTime);

    // PERF_END
    AmiUsbPerfEnd(NULL, UsbMsToken, "AmiUsb", DevInfo->DetectEndTime);

    // Init Config Usb Mouse token
    AsciiSPrintUnicodeFormat(UsbMsToken, USB_TOKEN_NAME_SIZE, L"ConfigUsbMs:%X/%X", DevInfo->VendorId, DevInfo->DeviceId);

    // PERF_START
    AmiUsbPerfStart(NULL, UsbMsToken, "AmiUsb", DevInfo->ConfigStartTime);

    // PERF_END
    AmiUsbPerfEnd(NULL, UsbMsToken, "AmiUsb", 0);


  
    REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_PERIPHERAL_MOUSE | EFI_P_PC_ENABLE));
}

/**
    Stops USB mouse device
    
    @param This                Pointer to driver binding protocol
    @param Controller          Controller handle.
    @param NumberOfChildren    Number of children in driver binding
    @param Children            Pointer to children handle
            
    @retval EFI_STATUS         Status of the operation
**/

EFI_STATUS
UninstallUSBMouse (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    UINTN                       NumberOfChildren,
    EFI_HANDLE                  *Children
)
{
    EFI_STATUS                   Status;
    EFI_SIMPLE_POINTER_PROTOCOL  *SimplePoint; 
    USB_MOUSE_DEV                *UsbMouse = 0; 
 

    Status = gBS->OpenProtocol(Controller,
                    &gEfiSimplePointerProtocolGuid,
                    (VOID **)&SimplePoint,
                    This->DriverBindingHandle,
                    Controller,
                    EFI_OPEN_PROTOCOL_GET_PROTOCOL); 
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_PERIPHERAL_MOUSE | EFI_P_PC_DISABLE));
    
    UsbMouse = USB_MOUSE_DEV_FROM_MOUSE_PROTOCOL(SimplePoint,SimplePointerProtocol);
    
    Status = gBS->UninstallProtocolInterface(Controller, &gEfiSimplePointerProtocolGuid,
                    &UsbMouse->SimplePointerProtocol);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->CloseEvent(
                    (UsbMouse->SimplePointerProtocol).WaitForInput);
    
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    gBS->FreePool(UsbMouse);
    
    return Status;
} 


/************ SimplePointer Protocol implementation routines*************/

/**
    This routine is a part of SimplePointerProtocol implementation;
    it resets USB mouse.

    @param This                  A pointer to the EFI_SIMPLE_POINTER_PROTOCOL instance.
    @param ExtendedVerification  Indicates that the driver may perform
                                 a more exhaustive verification operation of the device during
                                 reset.

    @retval EFI_SUCCESS          Success to reset USb mouse.

**/

EFI_STATUS
EFIAPI
UsbMouseReset(
    IN EFI_SIMPLE_POINTER_PROTOCOL    *This,
    IN BOOLEAN                        ExtendedVerification
  )
{
    REPORT_STATUS_CODE(EFI_PROGRESS_CODE, (EFI_PERIPHERAL_MOUSE | EFI_P_PC_RESET));
    SetMem (&MsState, sizeof(EFI_SIMPLE_POINTER_STATE), 0);
    StateChanged = FALSE;
    
    SetMem (&gUsbData->MouseData, sizeof(MOUSE_DATA), 0);

    return EFI_SUCCESS;
}


/**
    This routine is a part of SimplePointerProtocol implementation;
    it retrieves the current state of a pointer device.

    @param This        A pointer to the EFI_SIMPLE_POINTER_PROTOCOL instance.
    @param MouseState  A pointer to the state information on the pointer
                       device.
 
    @retval EFI_SUCCESS    The state of the pointer device was returned
                           in MouseState.
    @retval EFI_NOT_READY  The state of the pointer device has not changed
                           since the last call to GetMouseState().
    @retval EFI_DEVICE_ERROR A device error occurred while attempting to
                           retrieve the current state of mouse.
**/

EFI_STATUS
EFIAPI
GetMouseState(
    EFI_SIMPLE_POINTER_PROTOCOL  *This,
    EFI_SIMPLE_POINTER_STATE     *MouseState
)
{
    EFI_TPL OldTpl;
    EFI_STATUS  Status;
    
    if (MouseState == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);
    
    Status = UpdateUsbMouseData(This,MouseState);

    gBS->RestoreTPL(OldTpl);

    return Status;
}


/**
    This routine updates current mouse data.

    @param Data   pointer to the data area to be updated.

    @retval EFI_SUCCESS  Success to update usb mouse data

**/

EFI_STATUS
UpdateUsbMouseData (
    EFI_SIMPLE_POINTER_PROTOCOL  *This,
    EFI_SIMPLE_POINTER_STATE     *State
)
{
    BOOLEAN LeftButton;
    BOOLEAN RightButton;
    INT32   DataX;
    INT32   DataY;
    INT32   DataZ;
    UINT8   Data;
    EFI_STATUS  Status;
    UINT8   MouseData[4];
    UINTN   DataLength;
    UINT32  UsbStatus;
    INT32   Coordinates;
    USB_MOUSE_DEV       *UsbMouse = 0; 
 
    if ((gUsbData->UsbStateFlag & USB_FLAG_EFIMS_DIRECT_ACCESS) && (This != NULL) ){
        UsbMouse = USB_MOUSE_DEV_FROM_MOUSE_PROTOCOL(This,SimplePointerProtocol); 
        // Get the data from mouse
        DataLength = 4;
    
        Status = UsbMouse->UsbIo->UsbSyncInterruptTransfer(
                                    UsbMouse->UsbIo,
                                    UsbMouse->Endpoint | 0x80,    // IN
                                    MouseData,
                                    &DataLength,
                                    0,  // Timeout
                                   &UsbStatus
                                   );

        if (EFI_ERROR(Status)) {
            return EFI_DEVICE_ERROR;
        }
    
        gUsbData->MouseData.ButtonStatus = MouseData[0];
    
        Coordinates = (INT8)MouseData[1];
        gUsbData->MouseData.MouseX += Coordinates;
        Coordinates = (INT8)MouseData[2];
        gUsbData->MouseData.MouseY += Coordinates;
     }

    Data = gUsbData->MouseData.ButtonStatus & 7;

    //
    // Check mouse Data
    //
    LeftButton=(BOOLEAN)(Data & 0x01)?TRUE:FALSE;
    RightButton=(BOOLEAN)(Data & 0x02)?TRUE:FALSE;
    
    DataX = gUsbData->MouseData.MouseX;
    DataY = gUsbData->MouseData.MouseY;
    DataZ = - (gUsbData->MouseData.MouseZ);
    
    if (StateChanged == FALSE) {
        if (DataX == 0 && DataY == 0 && DataZ == 0 && 
            Data == ButtonsState) {
            return EFI_NOT_READY;
        }
        StateChanged = TRUE;
    }

    gUsbData->MouseData.MouseX=0;
    gUsbData->MouseData.MouseY=0;
    gUsbData->MouseData.MouseZ=0;

    ButtonsState = Data;
    MsState.LeftButton = LeftButton;
    MsState.RightButton = RightButton;
    MsState.RelativeMovementX += DataX;
    MsState.RelativeMovementY += DataY;
    MsState.RelativeMovementZ += DataZ; 


    if (State != NULL) {
        CopyMem(State, &MsState, sizeof(EFI_SIMPLE_POINTER_STATE));
        //
        // Clear previous move state
        //
        MsState.RelativeMovementX = 0;
        MsState.RelativeMovementY = 0;
        MsState.RelativeMovementZ = 0;  
        StateChanged = FALSE;
    }

    return EFI_SUCCESS;
}


/**
    Event notification function for SIMPLE_POINTER.WaitForInput
    event. Signal the event if there is input from mouse.

    @param Event    event to signal in case of mouse activity
    @param Context  data to pass along with the event.

**/

VOID
EFIAPI
UsbMouseWaitForInput (
    EFI_EVENT   Event,
    VOID        *Context
    )
{
    EFI_STATUS    Status;
    USB_MOUSE_DEV *UsbMouse = (USB_MOUSE_DEV*)Context; 

    Status = UpdateUsbMouseData (&UsbMouse->SimplePointerProtocol,NULL);
    if (EFI_ERROR(Status)) {
        return;
    }

    //
    // Someone is waiting on the mouse event, if there's
    // input from mouse, signal the event
    //
    gBS->SignalEvent(Event);

}



/**
    Initialize USB Mouse driver

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
UsbMsGetControllerName(
    EFI_HANDLE Controller,
    EFI_HANDLE Child
)
{
    return 0;
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
