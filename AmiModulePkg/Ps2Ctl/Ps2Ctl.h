//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Ps2Ctl.h
    Ps2 driver header file

**/

#ifndef __PS2_CTRL_H__
#define __PS2_CTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <Efi.h>
#include <Token.h>
#include <AmiDxeLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/AmiKeycode.h>
#include <Protocol/SimplePointer.h>

#define PS2_DRIVER_VER 0x10
#define DEVICETYPE_MOUSE     1
#define DEVICETYPE_KEYBOARD  2
extern EFI_SYSTEM_TABLE     *gSysTable;


EFI_STATUS 
EFIAPI 
StartMouse(
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller);

EFI_STATUS 
EFIAPI 
StartKeyboard(
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller);

EFI_STATUS 
EFIAPI 
StopMouse(
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller);

EFI_STATUS 
EFIAPI 
StopKeyboard(
    EFI_DRIVER_BINDING_PROTOCOL     *This,
    EFI_HANDLE                      Controller);

typedef 
EFI_STATUS 
(EFIAPI *STARTSTOPPROC)(
    EFI_DRIVER_BINDING_PROTOCOL     *This, 
    EFI_HANDLE                      Controller);

typedef struct PS2DEV_TABLE_tag {
    UINT32 hid;
    UINT32 uid;
    UINT8  DeviceType;
    STARTSTOPPROC start;
    STARTSTOPPROC stop;
    CHAR16* name;
} PS2DEV_TABLE;

typedef void (*STATEMACHINEPROC)(void*);
void PS2DataDispatcher(void*);
UINT8 KBCGetData();

BOOLEAN LookupPs2Hid(
    PS2DEV_TABLE*,
    UINT32,
    UINT32,
    PS2DEV_TABLE**);

EFI_STATUS GetPS2_DP(
    EFI_DRIVER_BINDING_PROTOCOL*,
    EFI_HANDLE,
    ACPI_HID_DEVICE_PATH**,
    UINT32);

typedef VOID (*AUTODETECT_KBD_MOUSE_PORTS)();
typedef EFI_STATUS (*DETECT_KBC_DEVICE)();
extern AUTODETECT_KBD_MOUSE_PORTS AutodetectKbdMousePortsPtr;
extern DETECT_KBC_DEVICE DetectKeyboardPtr;
extern DETECT_KBC_DEVICE DetectMousePtr;

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif  

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
