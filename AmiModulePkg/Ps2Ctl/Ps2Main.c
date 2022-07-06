//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Ps2Main.c
    PS/2 Controller DXE driver
    This driver supports PS/2 keyboard and/or PS/2 mouse depending on the
    switches in SDL tokens.

**/

//----------------------------------------------------------------------

#include "Efi.h"
#include <Protocol/ComponentName.h>
#include <Protocol/LegacyBiosExt.h>
#include "Ps2Ctl.h"
#include "Kbc.h"

//----------------------------------------------------------------------

UINT8                               gDriverStartCounter;
VOID                                *gAllDriverConnectedNotifyRegistration;
EFI_EVENT                           gAllDriverConnectedEvent;
extern BOOLEAN                      Ps2MouseSupport;
extern BOOLEAN                      Ps2KbdSupport;
extern BOOLEAN                      KbcBasicAssuranceTest;
extern EFI_COMPONENT_NAME_PROTOCOL  gPS2CtlDriverName;
extern EFI_LEGACY_8259_PROTOCOL     *mLegacy8259;
extern BOOLEAN                      KbRdBeforeInstall; 
EFI_SYSTEM_TABLE                    *gSysTable = NULL;
extern UINT8                        gKeyboardIrqInstall;
extern  UINT32                      IbFreeTimeoutValue;

  

//==================================================================================
// Function Prototypes for Driver Binding Protocol Interface
//==================================================================================
EFI_STATUS 
EFIAPI 
PS2CtlSupported (
        EFI_DRIVER_BINDING_PROTOCOL *This,
        EFI_HANDLE                  Controller,
        EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath);

EFI_STATUS 
EFIAPI 
PS2CtlStart (
        EFI_DRIVER_BINDING_PROTOCOL *This,
        EFI_HANDLE                  Controller,
        EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath);

EFI_STATUS 
EFIAPI 
PS2CtlStop (
        EFI_DRIVER_BINDING_PROTOCOL *This,
        EFI_HANDLE                  Controller,
        UINTN                       NumberOfChildren,
        EFI_HANDLE                  *ChildHandleBuffer);

//==================================================================================
// Driver binding protocol instance for PS2Ctl Driver
//==================================================================================
EFI_DRIVER_BINDING_PROTOCOL gPS2CtlDriverBinding = {
    PS2CtlSupported,
    PS2CtlStart,
    PS2CtlStop,
    PS2_DRIVER_VER,     // Driver version
    NULL,               // Image Handle
    NULL                // DriverBindingHandle
};

//==================================================================================
// Supported PS2 devices table
//==================================================================================
CHAR16 *gPS2ControllerName = L"PS/2 Controller";

PS2DEV_TABLE    supportedDevices[] = {
    {EISA_PNP_ID(0xF03), 0, DEVICETYPE_MOUSE, StartMouse, StopMouse, L"Microsoft PS/2 Mouse"},
    {EISA_PNP_ID(0xF12), 0, DEVICETYPE_MOUSE, StartMouse, StopMouse, L"Logitech PS/2 Mouse"},
    {EISA_PNP_ID(0xF13), 0, DEVICETYPE_MOUSE, StartMouse, StopMouse, L"Generic PS/2 Mouse"},
    {EISA_PNP_ID(0x303), 1, DEVICETYPE_MOUSE, StartMouse, StopMouse, L"IBM Keyboard, PS/2 Mouse Support"},
    {EISA_PNP_ID(0x303), 0, DEVICETYPE_KEYBOARD, StartKeyboard, StopKeyboard, L"Generic PS/2 Keyboard"},
    {0} // End of table
};


/**
    PS/2 Controller Driver Entry Point
    This function is a part of DriverBinfing protocol

    @param  ImageHandle  - Image handle for this driver image
            *SystemTable - pointer to the EFI system table

    @retval    EFI_STATUS Status of the operation

**/

EFI_STATUS 
EFIAPI 
PS2CtlEntryPoint (
    EFI_HANDLE          ImageHandle,
    EFI_SYSTEM_TABLE    *SystemTable )
{
    EFI_STATUS         Status;

    InitAmiLib(ImageHandle,SystemTable);

    gSysTable = SystemTable;
    gPS2CtlDriverBinding.DriverBindingHandle = NULL;
    gPS2CtlDriverBinding.ImageHandle = ImageHandle;

    Status = gSysTable->BootServices->InstallMultipleProtocolInterfaces(
                &gPS2CtlDriverBinding.DriverBindingHandle,
                &gEfiDriverBindingProtocolGuid, &gPS2CtlDriverBinding,
#ifndef EFI_COMPONENT_NAME2_PROTOCOL_GUID //old Core
                &gEfiComponentNameProtocolGuid, &gPS2CtlDriverName,
#else
                &gEfiComponentName2ProtocolGuid, &gPS2CtlDriverName,
#endif
                NULL);
    if (EFI_ERROR(Status)) {
       return Status;
    }
	
    gDriverStartCounter = 0;
    
#if defined(GenericSio_SUPPORT) && (GenericSio_SUPPORT != 0)
    //
    // Update the SIO variable in the ACPI name space depend on the 
    // Ps2keyboard and Mouse Present state.
    //
    Status = RegisterProtocolCallback(
                    &gBdsAllDriversConnectedProtocolGuid,
                    UpdateSioVariableForKeyboardMouse,
                    NULL,   
                    &gAllDriverConnectedEvent,
                    &gAllDriverConnectedNotifyRegistration
    );
    ASSERT_EFI_ERROR(Status);
#endif
    //
    // Initialized Keyboard irq if keys are to be read before starting driver
    // IRQ handler will save the data in temp buffer and once the Keyboard
    // Driver started , the temp buffer data is pushed into Keyboard driver.
    // So that the key's are pressed in post also taken by Ps2 driver
    //
    if(KbRdBeforeInstall){
        if(IoRead8(KBC_CMDSTS_PORT) != 0xFF) { 
            //
            // Enable the Keyboard and Keyboard Interrupt  
            //
            Write8042CommandByte(0x65);
            InitKeyboardIrq();
        }
    }

    return EFI_SUCCESS;
}


/**
    PS/2 Controller Driver Supported function
    This function is a part of DriverBinfing protocol

    @param	*This                - Pointer to this instance of the 
                                     driver binding protocol
            Controller           - Handle for this controller
            *RemainingDevicePath - Pointer to last node in device path

    @retval EFI_SUCCESS The device specified by ControllerHandle and
			RemainingDevicePath is supported by the driver specified by This.
			
	@retval	EFI_ALREADY_STARTED The device specified by ControllerHandle and
			RemainingDevicePath is already being managed by the driver specified by This.
			
	@retval	EFI_ACCESS_DENIED The device specified by ControllerHandle and
			RemainingDevicePath is already being managed by a different
			driver or an application that requires exclusive access.
			
	@retval	EFI_UNSUPPORTED The device specified by ControllerHandle and
			RemainingDevicePath is not supported by the driver specified byThis.
			
**/

EFI_STATUS 
EFIAPI 
PS2CtlSupported (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath)
{
    ACPI_HID_DEVICE_PATH    *acpiDP;
    EFI_STATUS				Status;

    // Checking if KBC is available at all
    if (IoRead8 (KBC_CMDSTS_PORT) == 0xFF){
        return EFI_UNSUPPORTED;
    }
    
    Status=GetPS2_DP(This, Controller, &acpiDP, EFI_OPEN_PROTOCOL_BY_DRIVER);
    
    if(EFI_ERROR(Status)) {
        return Status;
    }
    
    // Find the last device node in the device path and return "Supported" 
    // for mouse and/or keyboard depending on the SDL switches.
    if( LookupPs2Hid(supportedDevices, acpiDP->HID, acpiDP->UID, 0)){
        return EFI_SUCCESS;
    } 
 
    return EFI_UNSUPPORTED;

}

/**
    PS/2 Controller Driver Start function
    This function is a part of DriverBinfing protocol

    @param    *This                - Pointer to this instance of the driver binding protocol
              Controller           - Handle for this controller
              *RemainingDevicePath - Pointer to last node in device path

    @retval    EFI_STATUS Status of the operation

**/

EFI_STATUS 
EFIAPI 
PS2CtlStart (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath)
{
    ACPI_HID_DEVICE_PATH    *acpiDP;
    PS2DEV_TABLE            *ps2dev = 0;
    EFI_STATUS              Status;
    EFI_TPL                 OldTpl;

    // The following code performs the basic KBC initialization
    // It has to be executed only once, we use global variable
    // gDriverStartCounter to control this. Also, this code is
    // executed on a higher TPL to prevent re-entrance.
    OldTpl = gSysTable->BootServices->RaiseTPL(TPL_NOTIFY);
    if (OldTpl > TPL_NOTIFY) return EFI_DEVICE_ERROR;
    
    while (!gDriverStartCounter) {
        // Disable the Keyboard IRQ if it's enabled before 
        // Finish all the Keyboard Initialization and Re-enable the IRQ again 
        if(KbRdBeforeInstall){
            mLegacy8259->DisableIrq( mLegacy8259, SYSTEM_KEYBOARD_IRQ );
            gKeyboardIrqInstall=FALSE;
        }
        // Initialize KBC hardware
        if ( KbcBasicAssuranceTest ) {
            Status = KBCBatTest();      // Perform KBC Basic Assurance Test
            if (EFI_ERROR(Status)) {
                // Report the Error code if the BAT test failed
                ERROR_CODE (DXE_KEYBOARD_STUCK_KEY_ERROR, EFI_ERROR_MAJOR);
                break;
            }

            // Check for stuck keys
            Status = IbFreeTimeout(IbFreeTimeoutValue);
            if (EFI_ERROR(Status)) {
                // Report the Error Code.
                ERROR_CODE (DXE_KEYBOARD_STUCK_KEY_ERROR, EFI_ERROR_MAJOR);
                break;
            }
        }
        // Swap ports if needed
        if (AutodetectKbdMousePortsPtr!=NULL) AutodetectKbdMousePortsPtr();
    
        gDriverStartCounter++;
    }
    
    
    if(!EFI_ERROR(GetPS2_DP(This, Controller, &acpiDP, EFI_OPEN_PROTOCOL_BY_DRIVER)) &&
            LookupPs2Hid(supportedDevices, acpiDP->HID, acpiDP->UID, &ps2dev) ){
             Status = ps2dev->start(This, Controller);

            // End of critical section - restore TPL
            gSysTable->BootServices->RestoreTPL(OldTpl);

            if(EFI_ERROR(Status)) {
                return EFI_DEVICE_ERROR;
            }

            return EFI_SUCCESS;
    } 
        
    // End of critical section - restore TPL
    gSysTable->BootServices->RestoreTPL(OldTpl);

    // If control is here then something totally wrong happened:
    // if device is not supported then Start shouldn't be called.
    return EFI_DEVICE_ERROR;
}


/**
    PS/2 Controller Driver Stop function
    This function is a part of DriverBinfing protocol

    @param    *This              - Pointer to this instance of 
                                   the driver binding protocol
              Controller         - Handle for this controller
              NumberOfChildren   - Number of children of this controller
              *ChildHandleBuffer - Pointer to a buffer for child handles

    @retval    EFI_STATUS Status of the operation

**/

EFI_STATUS 
EFIAPI 
PS2CtlStop (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    UINTN                       NumberOfChildren,
    EFI_HANDLE                  *ChildHandleBuffer)
{
    ACPI_HID_DEVICE_PATH        *acpiDP;
    PS2DEV_TABLE                *ps2dev = 0;
    EFI_STATUS                  Status;

    if(!EFI_ERROR(GetPS2_DP(This, Controller, &acpiDP, EFI_OPEN_PROTOCOL_GET_PROTOCOL)) &&
            LookupPs2Hid(supportedDevices, acpiDP->HID, acpiDP->UID, &ps2dev) ){
        Status = ps2dev->stop(This, Controller);
        if(EFI_ERROR(Status)) {
            return EFI_DEVICE_ERROR;
        }
        return EFI_SUCCESS;
    } 

    // If control is here then device path was not found in the lookup table
    return EFI_DEVICE_ERROR;
}


/**
    This function searches the PS2 device in table that 
    matches given HID and UID

    @param    *devTable - Lookup table pointer
              hid       - HID to look for
              uid       - UID to look for
              **dev     - address of the matched table entry

    @retval    BOOLEAN TRUE if match is found, FALSE otherwise

**/

BOOLEAN
LookupPs2Hid (
    PS2DEV_TABLE    *devTable,
    UINT32          hid,
    UINT32          uid,
    PS2DEV_TABLE    **dev)
{
    for( ;devTable->hid;++devTable){
        if( devTable->hid == hid && devTable->uid==uid){
            if ( (devTable->DeviceType == DEVICETYPE_MOUSE && Ps2MouseSupport) ||
                 (devTable->DeviceType == DEVICETYPE_KEYBOARD && Ps2KbdSupport) ) {
                if(dev) *dev = devTable;
                return TRUE;
            }
        } 
    }
    return FALSE;
}


/**
    This function returns the last node in the device 
    path for the given controller.

    @param    *This      - Pointer to this instance of the driver binding protocol
              Controller - Handle for this controller ACPI_HID_DEVICE_PATH** ps2dp 
                           Pointer to ACPI HID device path
              Attributes - Attributes passed to driver binding protocol

    @retval    EFI_SUCCESS or EFI_UNSUPPORTED

**/

EFI_STATUS
GetPS2_DP (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    ACPI_HID_DEVICE_PATH        **ps2dp,
    UINT32                      Attributes)
{   
    EFI_STATUS Status;
    ACPI_HID_DEVICE_PATH        *acpiDP;
    EFI_DEVICE_PATH_PROTOCOL    * ps2DevPath, *pDP;

    // Get device path from Controller handle.
    Status = gSysTable->BootServices->OpenProtocol (
                Controller,
                &gEfiDevicePathProtocolGuid,
                (VOID**)&pDP,
                This->DriverBindingHandle,
                Controller,   
                Attributes
                );
  
    if (EFI_ERROR(Status)) {
        if( Status == EFI_ALREADY_STARTED || Status == EFI_ACCESS_DENIED ) {
            return Status;
        }
        return EFI_UNSUPPORTED;
    }

    // Process ps2DevPath - get the node which is before the EndNode
    ps2DevPath=DPGetLastNode(pDP);

    // ps2DevPath is now the last node
    acpiDP = *ps2dp = (ACPI_HID_DEVICE_PATH*)ps2DevPath;

    Status = (acpiDP->Header.Type == ACPI_DEVICE_PATH && 
        acpiDP->Header.SubType == ACPI_DP)? EFI_SUCCESS : EFI_UNSUPPORTED;

    if (Attributes!=EFI_OPEN_PROTOCOL_GET_PROTOCOL)
        gSysTable->BootServices->CloseProtocol (
            Controller,
            &gEfiDevicePathProtocolGuid,
            This->DriverBindingHandle,
            Controller
            );

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
