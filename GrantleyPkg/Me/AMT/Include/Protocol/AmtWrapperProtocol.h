//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/

#ifndef _EFI_AMT_WRAPPER_PROTOCOL_H
#define _EFI_AMT_WRAPPER_PROTOCOL_H

#include "Protocol\SimpleTextIn.h"

#define EFI_AMT_WRAPPER_PROTOCOL_GUID  \
  	{ 0xd54f49f6, 0xdd4, 0x4276, 0xb0, 0x9c, 0xdf, 0xe1, 0xfd, 0x80, 0x85, 0xf0 }

//EFI_FORWARD_DECLARATION (AMT_WRAPPER_PROTOCOL);

typedef
EFI_STATUS
(EFIAPI *AMT_WRAPPER_INIT) (
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE *SystemTable
);

typedef
BOOLEAN
(EFIAPI *AMT_WRAPPER_GET) (
	UINTN Index,
	VOID *pVar
);

typedef
BOOLEAN
(EFIAPI *AMT_WRAPPER_SET) (
	UINTN Index,
	VOID *pVar
);

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_ENABLE_KVM) (
  IN  VOID
);


typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_ENABLE_SOL) (
	IN  VOID
);

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_ENABLE_IDER) (
  	IN  VOID
);

/*
typedef
EFI_STATUS
(EFIAPI *AMT_WRAPPER_INITIALIZATION) (
    IN VOID
);
*/

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_BOOT_OPTION_EXIST) (
    IN VOID
);

typedef
EFI_STATUS
(EFIAPI *BDS_BOOT_VIA_ASF) (
  	IN  VOID
);

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_PAUSE_BOOT) (
  	IN  VOID
);

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_ENTER_SETUP) (
  	IN  VOID
);

typedef
BOOLEAN
(EFIAPI *ACTIVE_MANAGEMENT_CONSOLE_LOCKED) (
  	IN  VOID
);

typedef
EFI_STATUS
(EFIAPI *BDSLIB_UPDATE_CONSOLE_VARIABLE) (
  	IN  CHAR16                    *ConVarName,
  	IN  EFI_DEVICE_PATH_PROTOCOL  *CustomizedConDevicePath,
  	IN  EFI_DEVICE_PATH_PROTOCOL  *ExclusiveDevicePath
);

/* AptioV server override Don't use Intel function. 
typedef
EFI_STATUS
(EFIAPI *GET_SETUP_PROMPT) (
  IN OUT  UINTN     *PromptIndex,
  OUT     CHAR16    **String
);

typedef
EFI_STATUS
(EFIAPI *DETERMINE_SETUP_HOTKEY) (
    IN  EFI_KEY_DATA              *KeyData
);

typedef
EFI_STATUS
(EFIAPI *GET_SUPPORTED_HOTKEYS) (
  OUT  UINTN             *KeyCnt,
  OUT  EFI_KEY_DATA      **KeyData
  );
AptioV server override Don't use Intel function.  */

typedef struct _AMT_WRAPPER_PROTOCOL {
  AMT_WRAPPER_INIT                        AmtWrapperInit;
  AMT_WRAPPER_GET                         AmtWrapperGet;
  AMT_WRAPPER_SET                         AmtWrapperSet;
  ACTIVE_MANAGEMENT_ENABLE_SOL            ActiveManagementEnableSol;
  ACTIVE_MANAGEMENT_ENABLE_IDER           ActiveManagementEnableIdeR;
  ACTIVE_MANAGEMENT_PAUSE_BOOT            ActiveManagementPauseBoot;
  ACTIVE_MANAGEMENT_ENTER_SETUP           ActiveManagementEnterSetup;
  //AMT_WRAPPER_INITIALIZATION            AsfSupportInit;
  ACTIVE_MANAGEMENT_BOOT_OPTION_EXIST     ActiveManagementBootOptionExist;
  BDS_BOOT_VIA_ASF                        BdsBootViaAsf;
  BDSLIB_UPDATE_CONSOLE_VARIABLE          BdsLibUpdateConsoleVariable;
  ACTIVE_MANAGEMENT_CONSOLE_LOCKED        ActiveManagementConsoleLocked;
  ACTIVE_MANAGEMENT_ENABLE_KVM            ActiveManagementEnableKvm;
/*AptioV server override Don't use Intel function.
  GET_SETUP_PROMPT                        GetSetupPrompt;
  DETERMINE_SETUP_HOTKEY                  DetermineSetupHotKey;
  GET_SUPPORTED_HOTKEYS                   GetSupportedHotkeys;
AptioV server override Don't use Intel function. */
} AMT_WRAPPER_PROTOCOL;

#define GET_SOL_DP      1
#define GET_TIME_OUT    2					// return UINT16
#define GET_BOOT_OPTION 3					// return UINT16
#define GET_IS_SOL      4					// return UINT16
#define GET_IS_IDER     5					// return UINT16

#define SET_BIOS_WDT_START  1
#define SET_WDT_STOP        2
#define SET_OS_SOL          3
#define SET_OS_WDT_START    4

extern EFI_GUID gEfiAmtWrapperProtocolGuid;

#endif //_EFI_AMT_WRAPPER_PROTOCOL_H
