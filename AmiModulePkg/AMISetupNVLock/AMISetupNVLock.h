//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AMISetupNVLock.h
    This file contains AMISetupNVLock header

**/

#ifndef _AMISETUPNVLOCK_H_
#define	_AMISETUPNVLOCK_H_

#include <AmiDxeLib.h>
#include "Setup.h"

#define TSE_SETUP_VAR_NAME	    L"AMITSESetup"
#define SETUPPASS_VAR_NAME      L"$SETUPPASSWD"
#define SET_ADMINPASS_VAR_NAME  L"$SETADMINPASSWD"
#define SET_USERPASS_VAR_NAME   L"$SETUSERPASSWD"

#define SETUPNVLOCK_MAILBOX_ADDRESS_VARIABLE_GUID \
    { 0x504af431, 0x3025, 0x4d32, {0x9f, 0xbf, 0xe9, 0xf3, 0x18, 0x55, 0x55, 0x1d }}

#define SETUP_NV_CONTROL_PROTOCOL_GUID \
    { 0x1bd378d6, 0xd2d6, 0x417f, {0x87, 0xf6, 0x9e, 0x52, 0xf2, 0x8d, 0xcb, 0x2f }}

#define NULL_GUID \
    { 0x0, 0x0, 0x0, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }}

#define SETUPPASSWD_GUID \
	{ 0x5855CE1B, 0xFB8E, 0x47E4, {0xBC, 0x1A, 0x39, 0xEC, 0xAA, 0x0C, 0x96, 0xCF}}

#define	AMI_PASSWORD_NONE		0
#define	AMI_PASSWORD_USER		1
#define	AMI_PASSWORD_ADMIN		2
#define	AMI_INVALID_PASSWORD	3
#define SHA512_SIZE 32
#define SHA384_SIZE 24
#define SHA256_SIZE 20
#define EXTENDED_TSE_PWD 20

#if TSE_PWD_ENCRYPT_USING_SHA512 
#if SETUP_PASSWORD_LENGTH > SHA512_SIZE
#define TSE_PASSWORD_LENGTH SETUP_PASSWORD_LENGTH
#else 
#define TSE_PASSWORD_LENGTH SHA512_SIZE
#endif
#elif TSE_PWD_ENCRYPT_USING_SHA384
#if SETUP_PASSWORD_LENGTH > SHA384_SIZE
#define TSE_PASSWORD_LENGTH     SETUP_PASSWORD_LENGTH
#else 
#define TSE_PASSWORD_LENGTH     SHA384_SIZE
#endif
#elif TSE_PWD_ENCRYPT_USING_SHA256 && EXTENDED_TSE_PWD_ENCRYPT_SUPPORT
#if SETUP_PASSWORD_LENGTH > SHA256_SIZE
#define TSE_PASSWORD_LENGTH     SETUP_PASSWORD_LENGTH
#else 
#define TSE_PASSWORD_LENGTH     SHA256_SIZE
#endif
#elif EXTENDED_TSE_PWD_ENCRYPT_SUPPORT
#if SETUP_PASSWORD_LENGTH > EXTENDED_TSE_PWD
#define TSE_PASSWORD_LENGTH     SETUP_PASSWORD_LENGTH
#else 
#define TSE_PASSWORD_LENGTH     EXTENDED_TSE_PWD
#endif
#else
#define TSE_PASSWORD_LENGTH SETUP_PASSWORD_LENGTH
#endif

#if ENABLE_AMISETUP_NVLOCK_TRACES
#define DEBUG_SETUPNVLOCK(Arguments) { DEBUG(Arguments); }
#else
#define DEBUG_SETUPNVLOCK(Arguments)
#endif

//NvLockMailbox Variable used for communication b/w DXE and SMM
typedef struct {
    UINTN NvLockState;
} SETUPNVLOCK_MAILBOX;

typedef struct {
    CHAR8 VariableName[50];
    EFI_GUID VariableGuid;
} SETUP_VARIABLE_DATA;

//Support to allow Variable update through AFU
typedef EFI_STATUS (*TOGGLE_SMI_FLASH_FLAG)(
    BOOLEAN Start
);

typedef struct {
    TOGGLE_SMI_FLASH_FLAG ToggleSmiFlashUpdateFlag;
} AMI_SETUPNV_CONTROL_PROTOCOL;

typedef EFI_STATUS (HOOK_LOCK_UNLOCK_NV)( 
    IN CHAR16       *VariableName, 
    IN EFI_GUID     *VendorGuid,
    IN UINT32       Attributes, 
    IN UINTN        DataSize, 
    IN VOID         *Data 
);

EFI_STATUS 
ToggleSmiFlashUpdateFlag (
    BOOLEAN Start
);

EFI_STATUS 
InstallSetupNvControlProtocol();

UINT32 
AuthenticatePassword ( 
    CHAR16 *Password,  
    UINTN Passwordsize
);

EFI_STATUS 
DxeGetVariable (
    IN CHAR16   *VariableName, 
    IN EFI_GUID *VendorGuid, 
    OUT UINT32  *Attributes OPTIONAL,
    IN OUT UINTN *DataSize, 
    OUT VOID    *Data
);

EFI_STATUS 
DxeSetVariable (
    IN CHAR16   *VariableName, 
    IN EFI_GUID *VendorGuid,
    IN UINT32   Attributes, 
    IN UINTN    DataSize, 
    IN VOID     *Data
);

BOOLEAN 
IsSetupPasswordVariable (
    IN CHAR16       *VariableName,
    IN EFI_GUID     *VendorGuid 
);

VOID
DeletePasswordVariable();

VOID 
PasswordEncode ( 
    CHAR16 *Password, 
    UINTN MaxSize
);

EFI_STATUS 
LockNv();

EFI_STATUS 
UnlockNv();

EFI_STATUS 
SetPassword (
    CHAR16  *VariableName, 
    VOID    *Password, 
    UINTN   Passwordsize
);

EFI_STATUS 
ValidateAndSetSetupPwd (
    CHAR16      *VariableName, 
    EFI_GUID    *VendorGuid, 
    UINT32      Attributes, 
    UINTN       DataSize, 
    VOID        *Data
);

#endif /* _AMISETUPNVLOCK_H_ */
