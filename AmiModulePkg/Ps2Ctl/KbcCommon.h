//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file KbcCommon.h
    Keyboard Controller Common definition 

**/

#ifndef __PS2_KBC_COMMON_H__
#define __PS2_KBC_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// KBC command/status/data IO ports
//
#define KBC_CMDSTS_PORT 0x64
#define KBC_DATA_PORT   0x60

// IO Delay Port
#define IO_DELAY_PORT   0xED

//
// KBC status bits definition
//
#define KBC_OBF         0x01
#define KBC_IBF         0x02 
#define KBC_SYSFLAG     0x04 
#define KBC_CMD_DATA    0x08
#define KBC_INHIBIT_SW  0x10
#define KBC_AUX_OBF     0x20
#define KBC_TIMEOUT_ERR 0x40
#define KBC_PARITY_ERR  0x80

//
//    COMMANDS from KEYBOARD to SYSTEM
//
#define KB_ACK_COM          0xFA    // ACKNOWLEDGE command
#define KB_RSND_COM         0xFE    // RESEND command
#define KB_OVRN_COM         0xFF    // OVERRUN command
#define KB_DIAG_FAIL_COM    0xFD    // DIAGNOSTIC FAILURE command

#define KBD_ENABLE_SCANNING     0xF4    
#define KBD_DISABLE_SCANNING    0xF5   
#define KBD_RESET               0xFF    

#define rKeyboardID         0xF2
#define rMouseID            0xF2

//
// Keyboard scanner states
//
#define KBST_READY      0
#define KBST_E0         1
#define KBST_E1         2

#define BUFFER_SIZE         32
#define SYSTEM_KEYBOARD_IRQ 0x01
#define SYSTEM_MOUSE_IRQ    0x0C
#define SLAVE_IRQ           0X02

// LED inter command state
#define ED_COMMAND_ISSUED       0x01
#define ED_DATA_ISSUED          0x02

typedef struct {
    UINT8 makecode;
    UINT8 efi_scancode;
    UINT8 efi_key;
} EFI_EXTKEY;

static EFI_EXTKEY ScanCode_Table[] = {
    0x3B, EFI_SCAN_F1, EfiKeyF1,
    0x3C, EFI_SCAN_F2, EfiKeyF2,
    0x3D, EFI_SCAN_F3, EfiKeyF3,
    0x3E, EFI_SCAN_F4, EfiKeyF4,
    0x3F, EFI_SCAN_F5, EfiKeyF5,
    0x40, EFI_SCAN_F6, EfiKeyF6,
    0x41, EFI_SCAN_F7, EfiKeyF7,
    0x42, EFI_SCAN_F8, EfiKeyF8,
    0x43, EFI_SCAN_F9, EfiKeyF9,
    0x44, EFI_SCAN_F10, EfiKeyF10,
    0x57, EFI_SCAN_F11, EfiKeyF11,
    0x58, EFI_SCAN_F12, EfiKeyF12,
    0x47, EFI_SCAN_HOME, EfiKeyHome,
    0x48, EFI_SCAN_UP, EfiKeyUpArrow,
    0x49, EFI_SCAN_PGUP, EfiKeyPgUp,
    0x4B, EFI_SCAN_LEFT, EfiKeyLeftArrow,
    0x4D, EFI_SCAN_RIGHT, EfiKeyRightArrow,
    0x4F, EFI_SCAN_END, EfiKeyEnd,
    0x50, EFI_SCAN_DN, EfiKeyDownArrow,
    0x51, EFI_SCAN_PGDN, EfiKeyPgDn,
    0x52, EFI_SCAN_INS, EfiKeyIns,
    0x53, EFI_SCAN_DEL, EfiKeyDel,
    0xFF, 0xFF, 0xFF  // End of table
};

// Lower case keys
static UINT8 code_table[59] = { 
0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8,
9, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13,
0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'','`',
0,'\\','z','x','c','v','b','n','m',',','.','/',0,
'*', 0, ' ', 0
};

// Upper case keys
static UINT8 Code_Table[59] = { 
0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8,
9, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 13,
0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
'*', 0, ' ',0
};

// EFI keys (UEFI Spec 2.1, Ch.28.4, Pg.1325)
static UINT8 ScancodeToEfi_table[59] = { 
0, EfiKeyEsc, EfiKeyE1, EfiKeyE2, EfiKeyE3,EfiKeyE4, EfiKeyE5, EfiKeyE6,
EfiKeyE7, EfiKeyE8, EfiKeyE9, EfiKeyE10, EfiKeyE11, EfiKeyE12, EfiKeyBackSpace,
EfiKeyTab, EfiKeyD1, EfiKeyD2, EfiKeyD3, EfiKeyD4, EfiKeyD5, EfiKeyD6, EfiKeyD7,
EfiKeyD8, EfiKeyD9, EfiKeyD10, EfiKeyD11, EfiKeyD12, EfiKeyEnter,
EfiKeyCapsLock, EfiKeyC1, EfiKeyC2, EfiKeyC3, EfiKeyC4, EfiKeyC5, EfiKeyC6, EfiKeyC7,
EfiKeyC8, EfiKeyC9, EfiKeyC10, EfiKeyC11, EfiKeyE0,
EfiKeyLShift, EfiKeyD13, EfiKeyB1, EfiKeyB2, EfiKeyB3, EfiKeyB4, EfiKeyB5, EfiKeyB6,
EfiKeyB7, EfiKeyB8, EfiKeyB9, EfiKeyB10, EfiKeyRshift,
EfiKeyAsterisk, 0, EfiKeySpaceBar, 0
};

static UINT8 KeyPad_Table[] = {
    '7','8','9','-','4','5','6','+','1','2','3','0','.'
};

static UINT8 KeyPadEfiCode_Table[] = {
    EfiKeySeven, EfiKeyEight, EfiKeyNine, EfiKeyMinus,
    EfiKeyFour, EfiKeyFive, EfiKeySix, EfiKeyPlus,
    EfiKeyOne, EfiKeyTwo, EfiKeyThree,
    EfiKeyZero, EfiKeyPeriod
};

static UINT8 E0SeqA[4]  = {0x2A, 0xAA, 0x46, 0xB6};
static UINT8 E0SeqB[11] = {0xD3, 0xD0, 0xCF, 0xC7, 0xD2, 0xCB, 0xD1, 0xC9, 0xCD, 0xC8, 0xB5};
static UINT8 E1Seq[5]   = {0x1D, 0x45, 0xE1, 0x9D, 0xC5};
static UINT8 E0EnhancedKeys[] = {0x20,0x30,0x2E};  // only mute, volume-up, down


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif  

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
