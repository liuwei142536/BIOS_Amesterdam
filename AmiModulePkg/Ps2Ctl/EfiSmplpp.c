//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file efismplpp.c
    PS/2 mouse implementation of simple pointer protocol

**/

//----------------------------------------------------------------------

#include <AmiLib.h>
#include "Ps2Ctl.h"
#include "Kbc.h"
#include "Ps2Mouse.h"

//----------------------------------------------------------------------

extern MOUSE gMouse;
extern STATEMACHINEPROC DrivePS2MouseMachine;

//----------------------------------------------------------------------


/**
    This routine is called from Driver Binding Start function.
    It starts the mouse support

    @param     *This      - Pointer to this instance of driver
                            binding protocol structure
               Controller - Handle for this driver

    @retval    EFI_STATUS Status of the operation

**/

EFI_STATUS 
EFIAPI 
StartMouse (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller )
{
    EFI_STATUS                  Status;
    EFI_DEVICE_PATH_PROTOCOL    *pDummyDevPath;
    MOUSE                       *pmouse = 0;

    if (EFI_ERROR(gSysTable->BootServices->OpenProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            (VOID**)&pDummyDevPath,
            This->DriverBindingHandle,
            Controller,
            EFI_OPEN_PROTOCOL_BY_DRIVER))) {
        return EFI_INVALID_PARAMETER;
    }

    PROGRESS_CODE(DXE_MOUSE_INIT);
    DetectPS2KeyboardAndMouse();

    if ( EFI_ERROR(InitMOUSE( &pmouse ))) {
        gSysTable->BootServices->CloseProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller);
        // Don't report EFI_ERROR_MAJOR as if mouse not present also error is reported
        return EFI_DEVICE_ERROR;
    }

    Status = gSysTable->BootServices->CreateEvent(
            EVT_NOTIFY_WAIT,
            TPL_NOTIFY, 
            (EFI_EVENT_NOTIFY)OnWaitingOnMouse,
            pmouse,
            &pmouse->iSmplPtr.WaitForInput);
    ASSERT_EFI_ERROR(Status);
    // Install protocol interfaces for the pointer device.
    Status = gSysTable->BootServices->InstallProtocolInterface (
        &Controller, &gEfiSimplePointerProtocolGuid, EFI_NATIVE_INTERFACE,
        &pmouse->iSmplPtr);

    if (EFI_ERROR(Status)) {
        gSysTable->BootServices->CloseProtocol(
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller);

        gSysTable->BootServices->CloseEvent(pmouse->iSmplPtr.WaitForInput);
		ERROR_CODE (EFI_PERIPHERAL_MOUSE|EFI_P_EC_CONTROLLER_ERROR, EFI_ERROR_MAJOR);
    }

    DrivePS2MouseMachine = ReadAndProcessMouseData;

    return Status;  
}


/**
    This routine is called from Driver Binding Start function.
    It stops the mouse support

    @param  *This      - Pointer to this instance of driver
                         binding protocol structure
            Controller - Handle for this driver

    @retval EFI_STATUS Status of the operation

**/

EFI_STATUS 
EFIAPI 
StopMouse (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller )
{
    EFI_STATUS Status;

    // Kill wait event
    Status = gSysTable->BootServices->CloseEvent(gMouse.iSmplPtr.WaitForInput);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    // Uninstall protocol interfaces from the Mouse device.
    Status = gSysTable->BootServices->UninstallMultipleProtocolInterfaces (
        Controller,
        &gEfiSimplePointerProtocolGuid, &gMouse.iSmplPtr,   
        NULL
    );

    if (EFI_ERROR(Status)) {
        return Status;
    }
    // Close protocols that is open during Start
    Status = gSysTable->BootServices->CloseProtocol(
        Controller,
        &gEfiDevicePathProtocolGuid,
        This->DriverBindingHandle,
        Controller);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    pBS->FreePool(gMouse.iSmplPtr.Mode);

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
