//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Ami RedFish SecureBoot API: Common Protocol definitions
**/

#ifndef _AMI_REDFISH_API_H_
#define _AMI_REDFISH_API_H_

typedef enum {
  AmiRfSetupMode = 0,     ///< Secure Boot is currently in Setup Mode
  AmiRfUserMode,          ///< Secure Boot is currently in User Mode
  AmiRfAuditMode,         ///< Secure Boot is currently in Audit Mode
  AmiRfDeployedMode       ///< Secure Boot is currently in Deployed Mode
} AmiRedfish_SecureBootModeType;

typedef enum {
  AmiRfDisabled = 0,
  AmiRfEnabled,
} AmiRedfish_SecureBootState;

// B5E7C7AF-A3E7-4D3C-B217-04596E4C368F
#define AMI_REDFISH_SECBOOT_PROTOCOL_GUID \
    {0xB5E7C7AF, 0xA3E7, 0x4D3C, 0xB2, 0x17, 0x04, 0x59, 0x6E, 0x4C, 0x36, 0x8F}

typedef struct _AMI_REDFISH_SECBOOT_PROTOCOL AMI_REDFISH_SECBOOT_PROTOCOL;

typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_DEL_ALLKEYS) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_RESET_KEYS_TO_DEFAULT) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_DEL_PK) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_MODE) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
  IN OUT UINT8 *SecureBootModeType
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_CURRENT_STATE) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
  IN OUT UINT8 *SecureBootState
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_ENABLE) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
  IN BOOLEAN Enable
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_GET_ENABLE) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
  IN OUT UINT8 *SecureBootEnable
);
typedef 
EFI_STATUS
(EFIAPI *AMI_REDFISH_SECBOOT_TEST_ALL_KEYS_TO_DEFAULT) (
  IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);

struct _AMI_REDFISH_SECBOOT_PROTOCOL {
  UINT32                             Version;
  AMI_REDFISH_SECBOOT_DEL_ALLKEYS    DeleteAllKeys;
  AMI_REDFISH_SECBOOT_RESET_KEYS_TO_DEFAULT ResetAllKeysToDefault;
  AMI_REDFISH_SECBOOT_DEL_PK         DeletePK;
  AMI_REDFISH_SECBOOT_MODE           GetSecureBootMode;
  AMI_REDFISH_SECBOOT_CURRENT_STATE  GetSecureBootCurrentBoot;
  AMI_REDFISH_SECBOOT_ENABLE         SetSecureBootEnable;
// Added in Interface ver 01.01
  AMI_REDFISH_SECBOOT_GET_ENABLE     GetSecureBootEnable;
// Added in Interface ver 02.01
  AMI_REDFISH_SECBOOT_TEST_ALL_KEYS_TO_DEFAULT TestAllKeysToDefault;
};

#endif //_AMI_REDFISH_API_H_
