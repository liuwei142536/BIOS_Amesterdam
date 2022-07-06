//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <AmiDxeLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>

#include <Setup.h>

#include <SecureBoot.h>
#include <AmiSecureBootLib.h>

//----------------------------------------------------------------------------
/**
  Entry point of Secure Module DXE driver
  
  @param[in]   EFI_HANDLE           ImageHandle,
  @param[in]   EFI_SYSTEM_TABLE     *SystemTable
  
  @retval EFI_SUCCESS
**/
EFI_STATUS EFIAPI 
SecureBootDXE_Init (
    IN EFI_HANDLE         ImageHandle,
    IN EFI_SYSTEM_TABLE   *SystemTable
)
{
    EFI_STATUS Status;
    UINTN      DataSize;
    UINT32     Attributes;
    UINT8      SetupMode;
    VOID      *pHobList;
    static EFI_GUID guidSecurity = SECURITY_FORM_SET_GUID;
    static EFI_GUID gHobListGuid  = HOB_LIST_GUID;
    static SECURE_BOOT_SETUP_VAR SecureBootSetup = {DEFAULT_SECURE_BOOT_ENABLE, DEFAULT_SECURE_BOOT_MODE, DEFAULT_PROVISION_SECURE_VARS, 0,0,0,0};
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT && DEFAULT_SECURE_BOOT_DEPLOYED_MODE
    UINT8      DeployedMode;
#endif

    InitAmiLib(ImageHandle, SystemTable);

    DataSize = sizeof(SECURE_BOOT_SETUP_VAR);
    Status = pRS->GetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity,&Attributes,&DataSize,&SecureBootSetup);

    DEBUG((SecureBoot_DEBUG_LEVEL,"SecureBootSetup (%r) Attrib=%x,\nSecBoot-%d, SecMode-%d, DefaultProvision-%d\n",
           Status, Attributes, SecureBootSetup.SecureBootSupport, SecureBootSetup.SecureBootMode, SecureBootSetup.DefaultKeyProvision));

    pHobList = GetEfiConfigurationTable(SystemTable, &gHobListGuid);
    if (pHobList &&
        ( ((EFI_HOB_HANDOFF_INFO_TABLE*)pHobList)->BootMode==BOOT_IN_RECOVERY_MODE ||
          ((EFI_HOB_HANDOFF_INFO_TABLE*)pHobList)->BootMode==BOOT_ON_FLASH_UPDATE )
    )
        return EFI_UNLOAD_IMAGE;

    // Provisioning of secure boot defaults
    if(SecureBootSetup.DefaultKeyProvision == 1) {
        DataSize = 1;
        Status = pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &DataSize, &SetupMode);
        if(!EFI_ERROR(Status) && SetupMode == SETUP_MODE) {
            AmiInstallSecureBootDefaults(RESET_NV_KEYS); // erase all secure boot NV variables before installing selected defaults
            AmiInstallSecureBootDefaults(SET_NV_DEFAULT_KEYS);
            // The last Var to be installed - PK, SetupMode should've changed to User
            pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &DataSize, &SetupMode);
            if(SetupMode == USER_MODE) {
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT && DEFAULT_SECURE_BOOT_DEPLOYED_MODE
                // Update DeployedmentModeNV variable with new Secure Boot mode: DeployedMode
                DeployedMode = 1;
                Status = pRS->SetVariable(EFI_DEPLOYED_MODE_NAME,&gEfiGlobalVariableGuid, 
                            EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, 1, &DeployedMode);
                DEBUG((SecureBoot_DEBUG_LEVEL,"Force '%s' %r\n",EFI_DEPLOYED_MODE_NAME,  Status));
#endif
                // Issue reset only if User Mode with SecureBootSupport setup switch is enabled
                // System must operate in Secure Boot mode after reset
                if(SecureBootSetup.SecureBootSupport==SECURE_BOOT_ENABLE) {
                     pRS->ResetSystem(EfiResetCold, Status, 0, NULL);
                }
            }
        }
    }
    // Always install xxDefault variables
    AmiInstallSecureBootDefaults (SET_RT_DEFAULT_KEYS);

    //
    // This driver does not produce any protocol services, so always unload it.
    //
    return EFI_UNLOAD_IMAGE;
}
