//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Ps2KbcPei.h
    Ps2 driver header file for PEI driver

**/

#ifndef __AMI_PS2_KBD_PEI_H__
#define __AMI_PS2_KBD_PEI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "Token.h"
#include "Pei.h"
#include <AmiPeiLib.h>
#include "Ppi/AmiKeyCodePpi.h"

typedef struct {
    EFI_PEI_AMIKEYCODE_PPI      AmiKeyCodePpi;
    EFI_PEI_AMIKEYCODE_DATA     Ps2KeyData;
    UINT8                       ScannerState;
    UINT8                       ScannerCount;
} PEI_KEYBOARD;

EFI_STATUS
EFIAPI
Ps2PeiReadKey (
    IN  EFI_PEI_SERVICES        **PeiServices,
    IN  EFI_PEI_AMIKEYCODE_PPI  *This,
    OUT EFI_PEI_AMIKEYCODE_DATA *KeyData
);

EFI_STATUS 
EFIAPI
Ps2PeiSetLedState (
    IN    EFI_PEI_SERVICES        **PeiServices,
    IN    EFI_PEI_AMIKEYCODE_PPI  *This,
    IN    EFI_KEY_TOGGLE_STATE    KeyToggleState
);

EFI_STATUS
ProcessByte(
    EFI_PEI_AMIKEYCODE_DATA     *KeyData,
    UINT8                       Data, 
    BOOLEAN                     long_sequence
);

EFI_STATUS
Processkey(
    EFI_PEI_AMIKEYCODE_DATA     *KeyData
);

VOID 
Pei_Kbc_WriteKeyboardControllerData (
    IN    UINT8                   bCmd, 
    IN    UINT8                   bData
);

VOID 
Pei_Kbc_ClearOutputBuffer(
);

VOID 
Pei_Kbc_WriteCommandByte (
    UINT8 bCmd
);

VOID 
Pei_Kbc_WaitForInputBufferToBeFree(
);

VOID
PeiLEDsOnOff(
);

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
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
