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

/** @file Ps2Mouse.h
    Ps2 mouse support header

**/

#ifndef __PS2_MOUSE_H__
#define __PS2_MOUSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <Protocol/SimplePointer.h>

typedef struct {
    EFI_SIMPLE_POINTER_PROTOCOL iSmplPtr;
    EFI_SIMPLE_POINTER_MODE mode;
    EFI_SIMPLE_POINTER_STATE state;
    UINT8 saved_flags;
} MOUSE;

EFI_STATUS 
EFIAPI 
MouseReset (
    MOUSE   *This,
    BOOLEAN ExtendedVerification
);

EFI_STATUS 
EFIAPI 
MouseGetState (
    MOUSE                       *This,
    EFI_SIMPLE_POINTER_STATE    *State
);

EFI_STATUS
IsPs2MousePresent ();

VOID 
EFIAPI 
OnWaitingOnMouse(
    EFI_EVENT   Event, 
    MOUSE       *Context
);

EFI_STATUS 
InitMOUSE ( 
    MOUSE** 
);

VOID
ReadAndProcessMouseData (
    VOID    *Context 
);

#define KBC_CMD_ENABLE_AUX  0x0A8
#define KBC_CMD_DISABLE_AUX 0x0A7
#define KBC_CMD_ENABLE_KBD  0x0AE
#define KBC_CMD_DISABLE_KBD 0x0AD
#define KBC_CMD_CHK_AUX     0x0A9
#define KBC_CMD_WRT_AUX     0x0D4


//----------------------------------------------------------------------------
// MOUSE  COMMANDS from  SYSTEM SET SAMPLING RATE & SET RESOLUTION
// Needs one more byte to be sent.
//----------------------------------------------------------------------------

#define RESET               0x0FF    // Reset.
#define DISABLE             0x0F5    // Disable.
#define ENABLE              0x0F4    // Enable.
#define SET_SAMPLING_RATE   0x0F3    // Set sampling rate.
#define READ_DEVICE_TYPE    0x0F2    // Read device type.
#define STATUS_REQUEST      0x0E9    // Status request.
#define SET_RESOLUTION      0x0E8    // Set resolution.
#define SET_SCALING_2X1     0x0E7    // Set scaling 2:1.
#define RESET_SCALING       0x0E6    // Set scaling 1:1.
#define REMOTE_MODE         0x0F0    // Set Remote Mode - mouse transfers 
                                     // packets by request
#define READ_DATA           0x0eb    // Forces the transfer of a mouse data packet
#define SET_STANDART        0x0f6    // Initialize PS/2 and set standard state
//---------------------------------------
//  MOUSE RESPONSE ON COMMAND COMPLETION    
//---------------------------------------

#define ACK         0x0fa   // ACKnowledge
#define RSND        0x0fe   // RESEND
#define ERROR       0x0fc   // two invalid inputs received

//---------------------------------------
//  CCB Flags
//---------------------------------------

#define CCB_PCCOMPAT            0x40    // PC Compatibility mode
#define CCB_PS2MS_DISABLE       0x20    // Disable PS/2 mouse interface
#define CCB_KBD_DISABLE         0x10    // Disable in succession by mouse
#define CCB_SYSTEM              0x04    // 
#define CCB_MS_INTRPT           0x02    // 
#define CCB_KBD_INTRPT          0x01    // 


typedef struct {
    UINT8 flags;
    UINT8 x,y,z;    
} StdMouseDataPacket;

#define BUTTON_MID      0x04 
#define BUTTON_RIGHT    0x02
#define BUTTON_LEFT     0x01
#define MOUSE_X_NEG     0x10 
#define MOUSE_Y_NEG     0x20 
#define MOUSE_X_OVF     0x40 
#define MOUSE_Y_OVF     0x80 

#define NON_WHEEL_REPORT_FORMAT     3
#define WHEEL_REPORT_FORMAT         4

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif  

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
