//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Ps2Tokens.c
    File contains Ps2Ctl driver global variables which replaces SDL tokens

**/

//----------------------------------------------------------------------
#include <Efi.h>
#include <Token.h>
#include "Ps2Ctl.h"

//----------------------------------------------------------------------
//Ps2Ctl driver global variables

/**
    Variable to replace KBD_MS_IRQ_SUPPORT token.

    @note    BOOLEAN

**/
BOOLEAN KbdMsIrqSupport = 
#ifdef KBD_MS_IRQ_SUPPORT
    KBD_MS_IRQ_SUPPORT
#else
    0
#endif
    ;
/**
    Variable to replace PS2MOUSE_SUPPORT token.

    @note    BOOLEAN

**/
BOOLEAN Ps2MouseSupport = 
#ifdef PS2MOUSE_SUPPORT
    PS2MOUSE_SUPPORT
#else
    0
#endif
    ;
/**
    Variable to replace PS2KBD_SUPPORT token.

    @note    BOOLEAN

**/
BOOLEAN Ps2KbdSupport = 
#ifdef PS2KBD_SUPPORT
PS2KBD_SUPPORT
#else
    0
#endif
    ;
/**
    Variable to replace KBD_READ_BEFORE_INSTALL token.

    @note    BOOLEAN

**/
BOOLEAN KbRdBeforeInstall = 
#ifdef KBD_READ_BEFORE_INSTALL
    KBD_READ_BEFORE_INSTALL
#else
    0
#endif
    ;
/**
    Variable to replace KBC_AUTODETECT_PORTS token.

    @note    BOOLEAN

**/
BOOLEAN KbcAutoDetectPorts = 
#ifdef KBC_AUTODETECT_PORTS
KBC_AUTODETECT_PORTS
#else
    0
#endif
    ;
/**
    Variable to replace BLOCK_KBC_PIN_22_23_BIT token.

    @note    BOOLEAN

**/
BOOLEAN BlockKbcPin2223Bit = 
#ifdef BLOCK_KBC_PIN_22_23_BIT
BLOCK_KBC_PIN_22_23_BIT
#else
    0
#endif
    ;
/**
    Variable to replace LEDS_AT_STARTUP token.

    @note    UINT8

**/
UINT8 LedsAtStartup = 
#ifdef LEDS_AT_STARTUP
LEDS_AT_STARTUP
#else
    0
#endif
    ;
/**
    Variable to replace KBC_BASIC_ASSURANCE_TEST token.

    @note    BOOLEAN

**/
BOOLEAN KbcBasicAssuranceTest = 
#ifdef KBC_BASIC_ASSURANCE_TEST
KBC_BASIC_ASSURANCE_TEST
#else
    0
#endif
    ;
/**
    Variable to replace DETECT_PS2_KEYBOARD token.

    @note    BOOLEAN

**/
BOOLEAN DetectPs2KeyboardValue = 
#ifdef DETECT_PS2_KEYBOARD
DETECT_PS2_KEYBOARD
#else
    0
#endif
    ;
/**
    Variable to replace DETECT_PS2_MOUSE token.

    @note    BOOLEAN

**/
BOOLEAN DetectPs2MouseValue = 
#ifdef DETECT_PS2_MOUSE
DETECT_PS2_MOUSE
#else
    0
#endif
    ;
/**
    Variable to replace INSTALL_KEYBOARD_MOUSE_ALWAYS token.

    @note    BOOLEAN

**/
BOOLEAN InstallKeyboardMouseAlways = 
#ifdef INSTALL_KEYBOARD_MOUSE_ALWAYS
INSTALL_KEYBOARD_MOUSE_ALWAYS
#else
    0
#endif
    ;
/**
    Variable to replace IBFREE_TIMEOUT token.

    @note    UINT32

**/
UINT32
IbFreeTimeoutValue = 
#ifdef IBFREE_TIMEOUT
IBFREE_TIMEOUT
#else
    0
#endif
    ;

/**
    Variable to replace KBC_AUTODETECT_PORTS_FUNCTION token.

    @note    AUTODETECT_KBD_MOUSE_PORTS

**/

#if KBC_AUTODETECT_PORTS
extern void KBC_AUTODETECT_PORTS_FUNCTION();
#endif
AUTODETECT_KBD_MOUSE_PORTS AutodetectKbdMousePortsPtr =
#if KBC_AUTODETECT_PORTS
        (AUTODETECT_KBD_MOUSE_PORTS)KBC_AUTODETECT_PORTS_FUNCTION
#else
    NULL
#endif
    ;

/**
    Variable to replace MAXIMUM_TIMEOUT_FOR_IBFREE token.

    @note    UINT32

**/
UINT32
IbFreeMaxTimeoutValue = 
#ifdef MAXIMUM_TIMEOUT_FOR_IBFREE
MAXIMUM_TIMEOUT_FOR_IBFREE
#else
    0
#endif
    ;

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
