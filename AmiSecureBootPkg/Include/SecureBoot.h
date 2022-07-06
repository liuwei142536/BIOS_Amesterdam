//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    SecureBoot.h: Common Secure Boot module definitions
**/

#ifndef _SECURE_BOOT_MODE_H_
#define _SECURE_BOOT_MODE_H_

//#include <Guid/GlobalVariable.h>

///
/// "SecureBootSetup" variable stores current secure boot Setup control settings.
/// The value type is SECURE_BOOT_SETUP_VAR.
///
#define AMI_SECURE_BOOT_SETUP_VAR  L"SecureBootSetup"
///
/// "SecureVarPresent" variable stores current state of secure boot secure boot variables
/// The value type is UINT8[numSecVars]
///
#define AMI_SECURE_VAR_PRESENT_VAR  L"SecureVarPresent"
///
/// "DeploymentModeNV" variable stores current secure boot deployment mode.
///
/// GUID: gAmiDeploymentModeNVGuid
/// The value type is SECURE_BOOT_MODE_TYPE.
///
#define AMI_DEPLOYMENT_MODE_VARIABLE_NAME  L"DeploymentModeNv"
///
///  "VendorKeysNv" variable to record the out of band secure boot keys modification.
///  This variable is a read-only NV variable that indicates whether someone other than
///  the platform vendor has used a mechanism not defined by the UEFI Specification to
///  transition the system to setup mode or to update secure boot keys.
///
///  GUID: gAmiVendorKeysNvGuid
///  The value type is UINT8
///
#ifndef EFI_VENDOR_KEYS_NV_VARIABLE_NAME
#define EFI_VENDOR_KEYS_NV_VARIABLE_NAME  L"VendorKeysNv"
#endif
//#define VENDOR_KEYS_VALID             1
//#define VENDOR_KEYS_MODIFIED          0
///
///
/// "SecureBootSetup" variable data structure.
/// The value type is SECURE_BOOT_SETUP_VAR.
///
#pragma pack(1)
typedef struct{
    UINT8 SecureBootSupport;   ///< Setup control
    UINT8 SecureBootMode;      ///< Setup control
    UINT8 DefaultKeyProvision; ///< Setup control
    UINT8 Reserved;            ///< reserved
    UINT8 Load_from_OROM;      ///< Setup control
    UINT8 Load_from_REMOVABLE_MEDIA; ///< Setup control
    UINT8 Load_from_FIXED_MEDIA; ///< Setup control
} SECURE_BOOT_SETUP_VAR;

typedef struct{
    UINT8 Value; ///< SetupMode var value
} SETUP_MODE_VAR;

typedef struct{
    UINT8 Value; ///< SecureBoot var value
} SECURE_BOOT_VAR;

typedef struct{
    UINT8 Value; ///< AuditMode var value
} AUDIT_MODE_VAR;

typedef struct{
    UINT8 Value; ///< DeployedMode var value
} DEPLOYED_MODE_VAR;

typedef struct{
    UINT8 Value; ///< VendorKey variable value
} SECURE_BOOT_VENDOR_KEY_VAR;
#pragma pack()

typedef struct{
    UINT8 DBX; ///< var presence Flag
    UINT8 DBT; ///< var presence Flag
    UINT8 DBR; ///< var presence Flag
    UINT8 DB;  ///< var presence Flag
    UINT8 KEK; ///< var presence Flag
    UINT8 PK;  ///< var presence Flag
} SECURE_VAR_INSTALL_VAR;

// SecureBoot Control /0-Disabled/1-Enabled
#define SECURE_BOOT_ENABLE           1

// SetupMode variable
#define USER_MODE                    0
#define SETUP_MODE                   1

///
/// Value definition for SetupMode/DeployedMode/AuditMode variable
///
#ifndef DEPLOYED_MODE_ENABLE
#define SETUP_MODE_ENABLE            1
#define SETUP_MODE_DISABLE           0
#define DEPLOYED_MODE_ENABLE         1
#define DEPLOYED_MODE_DISABLE        0
#define AUDIT_MODE_ENABLE            1
#define AUDIT_MODE_DISABLE           0
#endif

// SecureBoot Mode
#define SECURE_BOOT_MODE_STANDARD    0
#define SECURE_BOOT_MODE_CUSTOM      1

#ifndef EFI_SECURE_BOOT_NAME
#define EFI_SECURE_BOOT_NAME         L"SecureBoot"
#endif
// EFI_IMAGE_SECURITY_DATABASE_DEFAULT must be defined in GlobalVariable.h or ImageAuthentication.h
// UEFI 2.4: Install Factory defaults as Read-only volatile variables for key distribution.
#ifndef EFI_DB_DEFAULT_VARIABLE_NAME
#define EFI_PK_DEFAULT_VARIABLE_NAME  L"PKDefault"
#define EFI_KEK_DEFAULT_VARIABLE_NAME L"KEKDefault"
#define EFI_DB_DEFAULT_VARIABLE_NAME  L"dbDefault"
#define EFI_DBX_DEFAULT_VARIABLE_NAME L"dbxDefault"
#endif
#ifndef EFI_IMAGE_SECURITY_DATABASE2
#define EFI_IMAGE_SECURITY_DATABASE2 L"dbt"
#define EFI_DBT_DEFAULT_VARIABLE_NAME L"dbtDefault"
#endif
#ifndef EFI_VENDOR_KEYS_VARIABLE_NAME
#define EFI_VENDOR_KEYS_VARIABLE_NAME L"VendorKeys"
#endif
// end  UEFI 2.4
#ifndef EFI_IMAGE_SECURITY_DATABASE3
#define EFI_IMAGE_SECURITY_DATABASE3 L"dbr"
#define EFI_DBR_DEFAULT_VARIABLE_NAME L"dbrDefault"
#endif
#ifndef EFI_OS_RECOVERY_ORDER_VARIABLE_NAME
#define EFI_OS_RECOVERY_ORDER_VARIABLE_NAME L"OsRecoveryOrder"
#endif
#ifndef EFI_OS_RECOVERY_XXXX_VARIABLE_NAME
#define EFI_OS_RECOVERY_XXXX_VARIABLE_NAME L"OsRecovery"
#endif
#ifndef EFI_AUDIT_MODE_NAME
#define EFI_AUDIT_MODE_NAME L"AuditMode"
#endif
#ifndef EFI_DEPLOYED_MODE_NAME
#define EFI_DEPLOYED_MODE_NAME L"DeployedMode"
#endif
// UEFI 2.7
#ifndef EFI_WARN_RESET_REQUIRED
#define EFI_WARN_RESET_REQUIRED 7
#endif
#ifndef EFI_CERT_EXTERNAL_MANAGEMENT_GUID
#define EFI_CERT_EXTERNAL_MANAGEMENT_GUID \
  { \
    0x452e8ced, 0xdfff, 0x4b8c, { 0xae, 0x01, 0x51, 0x18, 0x86, 0x2e, 0x68, 0x2c } \
  }
#endif
// end UEFI 2.7

#define SIGSUPPORT_LIST EFI_CERT_X509_SHA256_GUID, EFI_CERT_X509_SHA384_GUID, EFI_CERT_X509_SHA512_GUID, EFI_CERT_SHA256_GUID, EFI_CERT_X509_GUID, EFI_CERT_RSA2048_GUID, EFI_CERT_RSA2048_SHA256_GUID, EFI_CERT_RSA2048_SHA1_GUID, EFI_CERT_EXTERNAL_MANAGEMENT_GUID
#define SIGSUPPORT_NUM 9

#endif //_SECURE_BOOT_MODE_H_
