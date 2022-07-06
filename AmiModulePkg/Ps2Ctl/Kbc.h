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

/** @file Kbc.h
    Keyboard Controller functions header

**/

#ifndef __PS2_KBC_SUPPORT_H__
#define __PS2_KBC_SUPPORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <Protocol/Cpu.h>
#include <Protocol/Legacy8259.h>
#include <KbcCommon.h>

#define         KEY_POLLING_INTERVAL    500000

typedef struct {
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL iSimpleIn;
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL iSimpleInEx;
    AMI_EFIKEYCODE_PROTOCOL iKeycodeInEx;
    AMI_EFI_KEY_DATA KeyData;
    BOOLEAN KeyIsReady;
    UINT8  ScannerState;
    AMI_EFI_KEY_DATA* pBufHead;
    AMI_EFI_KEY_DATA* pBufTail;
    AMI_EFI_KEY_DATA* pBufStart;
    AMI_EFI_KEY_DATA* pBufEnd;
    UINT8  Count;
    UINT8  Indicators;
    UINT8  LEDCommandState;
    UINT8  CommandResponded;           
} KEYBOARD;

typedef struct {
    UINT8   KbdBuffer[BUFFER_SIZE];
    UINT8   KbdIndex;
} KEYBOARD_IRQ_STORE;

EFI_STATUS KBCBatTest ();
UINT8 Read8042(UINT8 bCmd);
UINT8 ObFullRead();
void WriteKeyboardCommand(UINT8 bCmd);
void WriteKeyboardData(UINT8 bCmd);
void Write8042CommandByte(UINT8 bCCB);
void IbFree();
EFI_STATUS IbFreeTimeout (UINT32 TimeoutValue);
void IoDelay();
void AutodetectKbdMousePorts();
EFI_STATUS ReadDevice (UINT8 bCmd, UINT8 *Data, UINT8 Response);
BOOLEAN ObFullReadTimeout(UINT8* data,  UINT32 msec, BOOLEAN ONLYOBF);
UINT8 IssueCommand(UINT8 bCmd);
UINT8 AuxDeviceCommand(UINT8 bCmd);
EFI_STATUS OutToKb (KEYBOARD* kbd, UINT8 bCmd);
EFI_STATUS InsertKeyToBuffer (KEYBOARD* kbd, AMI_EFI_KEY_DATA *key);
EFI_STATUS GetKeyFromBuffer (KEYBOARD* kbd, VOID* key, UINT8 size);
BOOLEAN CheckKeyinBuffer (KEYBOARD* kbd);
void ReadAndProcessKey(void *Context);
void DisableKeyboard();
EFI_STATUS EnableKeyboard ();
void DisableAuxDevice();
void EnableAuxDevice();
void LEDsOnOff(KEYBOARD* kbd);
void CheckIssueLEDCmd (KEYBOARD *kbd);
void ProcessKBDResponse (KEYBOARD *kbd, UINT8 bData); 
EFI_STATUS DetectPS2Keyboard ();
EFI_STATUS DetectPS2KeyboardAndMouse ();
void HandleKBDData(KEYBOARD *Context, UINT8 data);
void ProcessKBDData (KEYBOARD *kbd, UINT8 data);
BOOLEAN ObFullReadMTimeout (UINT8* data, UINT32 msec);
EFI_STATUS ReadDeviceM (UINT8 bCmd, UINT8 *Data, UINT8 Response);
EFI_STATUS DetectPs2Mouse (BOOLEAN Flag);
EFI_STATUS GetMouseData ();

UINT8
MouseDummyGetData (
VOID *Context 
);

#if defined(GenericSio_SUPPORT) && (GenericSio_SUPPORT != 0)
#include "GenericSio.h"
VOID 
EFIAPI 
UpdateSioVariableForKeyboardMouse(
    EFI_EVENT   Event,
    VOID        *Context
);
#endif

VOID InitKeyboardIrq(VOID);     
void ProcessLEDCommandData(KEYBOARD* kbd);
EFI_STATUS ProcessMultiLanguage (
    IN OUT  AMI_EFI_KEY_DATA                *KeyData
    );
EFI_STATUS IsPs2MousePresent ();

EFI_STATUS 
EFIAPI 
KbdReset (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
    BOOLEAN                         ExtendedVerification);

EFI_STATUS 
EFIAPI 
KbdReadKey (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
    EFI_INPUT_KEY                   *key);

EFI_STATUS 
EFIAPI 
KbdResetEx (
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL   *This,
    BOOLEAN                             ExtendedVerification );

EFI_STATUS 
EFIAPI 
KbdReadKeyEx (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL    *This,
    OUT EFI_KEY_DATA                        *KeyData
);

EFI_STATUS 
EFIAPI 
KbdReadEfiKeyEx (
    IN AMI_EFIKEYCODE_PROTOCOL  *This,
    OUT AMI_EFI_KEY_DATA        *KeyData
);

EFI_STATUS 
EFIAPI 
SetState (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL    *This,
    IN EFI_KEY_TOGGLE_STATE                 *KeyToggleState
);

EFI_STATUS 
EFIAPI 
RegisterKeyNotify (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL    *This,
    IN EFI_KEY_DATA                         *KeyData,
    IN EFI_KEY_NOTIFY_FUNCTION              KeyNotificationFunction,
    OUT EFI_HANDLE                          *NotifyHandle
);

EFI_STATUS 
EFIAPI 
UnRegisterKeyNotify (
    IN EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL    *This,
    IN EFI_HANDLE                           NotificationHandle
);

void 
EFIAPI 
OnWaitingOnKey(
    EFI_EVENT   Event,
    void        *Context
);

VOID 
EFIAPI 
StartPollingKey(
    EFI_EVENT   Event,
    VOID        *Context
);

void ProcessByte(
    KEYBOARD    *Kbd,
    UINT8       data,
    BOOLEAN     fourth_byte
);

void ResetStateMachine(
    KEYBOARD    *Kbd
);

void LEDsOnOff(
    KEYBOARD* Kbd
);

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
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
