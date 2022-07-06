//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#include <Token.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>

#include <SecureBoot.h>
#include <AmiRedFishApi.h>
#include <AmiSecureBootLib.h>

//----------------------------------------------------------------------------
// Function forward declaration
//----------------------------------------------------------------------------

// Delete the content of all UEFI Secure Boot key databases (PK, KEK, DB, DBX). This puts the system in Setup Mode
EFI_STATUS EFIAPI DeleteAllKeys (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
// Reset the content of all UEFI Secure Boot key databases (PK, KEK, DB, DBX,..) to factory default values
EFI_STATUS EFIAPI ResetAllKeysToDefault (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
// Delete the content of the PK UEFI Secure Boot database. This puts the system in Setup Mode.
EFI_STATUS EFIAPI DeletePK (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
// Current Secure Boot Mode.
EFI_STATUS EFIAPI SecureBootMode(
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN OUT UINT8 *SecureBootModeType
);
//Secure Boot state during the current boot cycle.
EFI_STATUS EFIAPI SecureBootCurrentBoot (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN OUT UINT8 *SecureBootState
);
// Enable or disable UEFI Secure Boot (takes effect on next boot).
// Setting this property to true enables UEFI Secure Boot, and 
// setting it to false disables it. This property can be enabled only in UEFI boot mode.
EFI_STATUS EFIAPI SecureBootEnable (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN BOOLEAN Enable
);    
//Return Secure Boot Setup enable status
EFI_STATUS EFIAPI GetSecureBootEnable (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN OUT UINT8 *SecureBootEnable
);    
//Check if all UEFI Secure Boot key databases (PK, KEK, DB, DBX,..) are set to factory default values
// EFI_SUCCESS    Only default keys are installed
// EFI_NOT_FOUND  Not all keys are Default
// EFI_NOT_READY  No keys are installed
EFI_STATUS EFIAPI
TestAllKeysToDefault (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
);
//----------------------------------------------------------------------------
// Protocol Identifiers
//----------------------------------------------------------------------------
//EFI_GUID gAmiRedFishSecBootApiGuid = AMI_REDFISH_SECBOOT_PROTOCOL_GUID;

AMI_REDFISH_SECBOOT_PROTOCOL mRedfishSecbootAPI = {
  0x10040201,
  DeleteAllKeys,
  ResetAllKeysToDefault,
  DeletePK,
  SecureBootMode,
  SecureBootCurrentBoot,
  SecureBootEnable,
  // added in v0101
  GetSecureBootEnable,
  // added in v0201
  TestAllKeysToDefault
};
//----------------------------------------------------------------------------

/**
  Provisioning of factory default Secure Boot policy Variables
  Install EFI Variables: PK, KEK, db, dbx, ...
  
  @param[in]  InstallVars Supported Types: RESET_NV_KEYS & SET_NV_DEFAULT_KEYS

  @retval     Status
**/
EFI_STATUS
InstallSecureVariables (
    UINT16    InstallVars
    )
{
    return AmiInstallSecureBootDefaults (InstallVars);
}

/**
  Enable or disable UEFI Secure Boot (takes effect on next boot).

  @param[in]  Enable 

  @retval     Status
**/
EFI_STATUS EFIAPI
SecureBootEnable(
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN BOOLEAN Enable
    )
{
    EFI_STATUS  Status;
    UINTN       Size;
    UINT32      Attributes;
    SECURE_BOOT_SETUP_VAR SecureBootSetup;
    static EFI_GUID guidSecurity = SECURITY_FORM_SET_GUID;

    Size = sizeof(SECURE_BOOT_SETUP_VAR);
    Status = pRS->GetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity, &Attributes, &Size, &SecureBootSetup);
    if(EFI_ERROR(Status))
        return EFI_UNSUPPORTED;
    SecureBootSetup.SecureBootSupport = Enable;
    Status = pRS->SetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity, Attributes, Size, &SecureBootSetup);
//    DEBUG((SecureBoot_DEBUG_LEVEL,"Update %s Var %r\n", AMI_SECURE_BOOT_SETUP_VAR, Status));

    return Status;
}
/**
  Secure Boot state during the current boot cycle.

  @retval     SecureBootState
  @retval     Status
**/
EFI_STATUS EFIAPI
GetSecureBootEnable( 
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    UINT8 *SecureBootEnable
    )
{
    EFI_STATUS  Status;
    UINTN       Size;
    SECURE_BOOT_SETUP_VAR SecureBootSetup;
    static EFI_GUID guidSecurity = SECURITY_FORM_SET_GUID;

    if(SecureBootEnable==NULL)
        return EFI_INVALID_PARAMETER;

    Size = sizeof(SECURE_BOOT_SETUP_VAR);
    Status = pRS->GetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity, NULL, &Size, &SecureBootSetup);
    if(!EFI_ERROR(Status)) {
        *SecureBootEnable = SecureBootSetup.SecureBootSupport;
    }

    return Status;
}

/**
  Secure Boot state during the current boot cycle.

  @retval     SecureBootState
  @retval     Status
**/
EFI_STATUS EFIAPI
SecureBootCurrentBoot( 
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    UINT8 *SecureBootState
    )
{
    UINTN       Size;
    if(SecureBootState==NULL)
        return EFI_INVALID_PARAMETER;

    Size = sizeof(UINT8);
    return pRS->GetVariable(EFI_SECURE_BOOT_NAME, &gEfiGlobalVariableGuid, NULL, &Size, SecureBootState);
}

/**
  Return Current Secure Boot Mode.

  SetupMode    0 Secure Boot is currently in Setup Mode
  UserMode     1 Secure Boot is currently in User Mode
  AuditMode    2 Secure Boot is currently in Audit Mode
  DeployedMode 3 Secure Boot is currently in Deployed Mode

  @retval     SecureBootModeType
  @retval     Status
**/
EFI_STATUS EFIAPI
SecureBootMode( 
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This,
    IN OUT UINT8 *SecureBootModeType
    )
{
    EFI_STATUS  Status;
    UINTN       Size;
    UINT8       SetupMode, AuditMode, DeployedMode;

    if(SecureBootModeType==NULL)
        return EFI_INVALID_PARAMETER;

    Size = sizeof(UINT8);
    Status = pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &SetupMode);
    if(EFI_ERROR(Status))
        return EFI_UNSUPPORTED;
    // User=1/Setup=0
    *SecureBootModeType = (UINT8)((SetupMode==1)?AmiRfSetupMode:AmiRfUserMode);

    // Update Audit/Deployed Vars
    Size = sizeof(UINT8);
    if(EFI_ERROR(pRS->GetVariable(EFI_AUDIT_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &AuditMode)))
        return Status;
    Size = sizeof(UINT8);
    if(EFI_ERROR(pRS->GetVariable(EFI_DEPLOYED_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &DeployedMode)))
        return Status; // User/Setup

    if(AuditMode) {
        *SecureBootModeType = (UINT8)AmiRfAuditMode;
    }
    else
        if(DeployedMode) {
            *SecureBootModeType = (UINT8)AmiRfDeployedMode;
        }

    return Status;
}

/**
  Delete the content of the PK UEFI Secure Boot database. This puts the system in Setup Mode.

  @retval     Status
**/
EFI_STATUS EFIAPI
DeletePK(
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
    )
{
    EFI_STATUS Status;
    // try to erase. should succeed if system in pre-boot and physical user authenticated mode
    Status = AmiSetSecureBootVariable(SET_SECURE_VARIABLE_DEL, SecureVariableFileName[PkVar], NULL, &gEfiGlobalVariableGuid, NULL, 0 );
    DEBUG((SecureBoot_DEBUG_LEVEL,"Del Var %s, Status %r\n",SecureVariableFileName[PkVar], Status));
    return Status;
}

/**
  Delete the content of all UEFI Secure Boot key databases (PK, KEK, DB, DBX,..). This puts the system in Setup Mode

  @retval     Status
**/
EFI_STATUS EFIAPI
DeleteAllKeys(
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
    )
{
    return InstallSecureVariables(RESET_NV_KEYS);    // erase
}

/**
  Reset the content of all UEFI Secure Boot key databases (PK, KEK, DB, DBX,..) to factory default values

  @retval     Status
**/
EFI_STATUS EFIAPI
ResetAllKeysToDefault (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
    )
{
    InstallSecureVariables(RESET_NV_KEYS);
    return InstallSecureVariables(SET_NV_DEFAULT_KEYS);
}

/**
  Check if all UEFI Secure Boot key databases (PK, KEK, DB, DBX,..) are set to factory default values

  @retval     Status
  EFI_SUCCESS    Only default keys are installed
  EFI_NOT_READY  Not all keys are Default
  EFI_NOT_FOUND  No keys are installed
**/
EFI_STATUS EFIAPI
TestAllKeysToDefault (
    IN CONST AMI_REDFISH_SECBOOT_PROTOCOL *This
    )
{
    EFI_STATUS Status;
    UINT8       Index;
    EFI_GUID    *EfiVarGuid;
    UINTN       Size1, Size2;
    UINT8       *Buf1, *Buf2;
    BOOLEAN     bVendor;     // Vendor keys

    Status = EFI_NOT_FOUND;
    Index = 0;
    bVendor = TRUE;
    while(bVendor == TRUE && SecureVariableFileName[Index] != NULL)
    {
        if(Index < SecureVariableSigDatabaseCount)
            EfiVarGuid = &gEfiImageSecurityDatabaseGuid;
        else
            EfiVarGuid = &gEfiGlobalVariableGuid;
        Size1 = 0;
        //get Default Key from Volatile key var (use NV Key size)
        Status = pRS->GetVariable( SecureVariableFileNameDefault[Index], &gEfiGlobalVariableGuid, NULL, &Size1, NULL);
        DEBUG((SecureBoot_DEBUG_LEVEL,"%d:%s(%Xh),  status=%r\n",  Index, SecureVariableFileNameDefault[Index], Size1, Status));
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Buf1 = NULL;
            Status = pBS->AllocatePool(EfiBootServicesData, Size1, (void**)&Buf1);
            ASSERT_EFI_ERROR (Status);
            Status = pRS->GetVariable( SecureVariableFileNameDefault[Index], &gEfiGlobalVariableGuid, NULL, &Size1, Buf1);
            DEBUG((SecureBoot_DEBUG_LEVEL,"  %s(%Xh),  status=%r\n", SecureVariableFileNameDefault[Index], Size1, Status));
            if(!EFI_ERROR(Status)) {
                Buf2 = NULL;
                Size2 = Size1;
                Status = pBS->AllocatePool(EfiBootServicesData, Size2, (void**)&Buf2);
                ASSERT_EFI_ERROR (Status);
                //get Key from NV
                Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &Size2, Buf2);
                DEBUG((SecureBoot_DEBUG_LEVEL,"  %s(%Xh),  status=%r\n\n", SecureVariableFileName[Index], Size2, Status));
                if(EFI_ERROR(Status) ||
                   Size1 != Size2 || 
                   MemCmp(Buf1, Buf2, Size1)
                   ) {
                    bVendor = FALSE;
                }
                if(Buf2) pBS->FreePool(Buf2);
            }
            if(Buf1) pBS->FreePool(Buf1);
        } else {
            // Default Key does not exist but NV key may be installed
            Size1 = 0;
            //get Key from NV
            Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &Size1, NULL);
            DEBUG((SecureBoot_DEBUG_LEVEL,"  %s(%Xh),  status=%r\n\n", SecureVariableFileName[Index], Size1, Status));
            if(Status == EFI_BUFFER_TOO_SMALL) {
                bVendor = FALSE;
            }
        }
        Index++;
   }
   // No Keys are installed
   DEBUG((SecureBoot_DEBUG_LEVEL,"bVendor %d, Index %d-%d, %r\n", bVendor, Index, MaxVarCount, Status));
   if(bVendor && Index == MaxVarCount && Status == EFI_NOT_FOUND) // last default
       return EFI_NOT_FOUND;

   return (bVendor)?EFI_SUCCESS:EFI_NOT_READY;
}
/**
  Entry point of RedFish Secure Boot DXE driver
  @param[in]  EFI_HANDLE 
  @param[in]  EFI_SYSTEM_TABLE 
  
  @retval     Status
**/
EFI_STATUS EFIAPI 
RedFishApi_Init (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
    )
{
    EFI_HANDLE Handle;

    InitAmiLib(ImageHandle, SystemTable);

    Handle = NULL;
    // 0-15 - Protocol version
    //      0- 7  Min version
    //      8-15  Major
    // 16-31 - compatible Redfish SecBoot resource spec version
    mRedfishSecbootAPI.Version = 0x01040201;
    return pBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    &gAmiRedFishSecBootApiGuid,
                    &mRedfishSecbootAPI,
                    NULL
                    );
}
