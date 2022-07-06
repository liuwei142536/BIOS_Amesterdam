//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  DxeAmiConnectPolicyLib instance.
**/

#include <Protocol/AmiBdsConnectPolicy.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SecurityManagementLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>

STATIC AMI_BDS_CONNECT_POLICY_PROTOCOL* AmiConnectPolicyLibAmiBdsConnectPolicy = NULL;

/**
  The function implements platform specific driver connect policy.

  The function uses an instance of AMI_BDS_CONNECT_POLICY_PROTOCOL to retrieve
  platform specific driver connect policy for a specified device path.
  The function then implements the retrieved policy by preventing driver binding to the device path and,
  when device path is a PCI OpROM device path, by blocking OpROM image loading.

  __NOTE: Even though the function uses Security Architectural Protocol infrastructure, it has no effect on
    platform security. It provides a way to implement platform specific device connect policy.

  @param[in]    AuthenticationStatus    Not used
  @param[in]    File                    A pointer to the device path being connected or device path of an image being loaded.
                                        In case of ConnectController handling, this is device path associated with
                                        ControllerHandle and RemainingDevicePath
                                        In case of LoadImage handling, this is device path of the image being loaded.
  @param[in]    FileBuffer              A pointer to the buffer with the UEFI file image. The function uses pointer value to detect
                                        if ConnectController or LoadImage validation has to be performed.
                                        FileBuffer is NULL when the handler is called by gBS->ConnectController.
                                        FileBuffer is not NULL when the handler is called by gBS->LoadImage.
  @param[in]    FileSize                Not used

  @retval EFI_SUCCESS             File is NULL or platform connect policy cannot be retrieved
  @retval EFI_SUCCESS             Platform policy allows to start
                                  UEFI device drivers on the device path specified by File.
  @retval EFI_SECURITY_VIOLATION  Platform policy does not allow to start
                                  UEFI device drivers on the device path specified by File.
**/
EFI_STATUS EFIAPI  AmiConnectPolicyHandler(
    IN  UINT32 AuthenticationStatus, IN CONST EFI_DEVICE_PATH_PROTOCOL *File,
    IN  VOID *FileBuffer, IN  UINTN FileSize, IN BOOLEAN BootPolicy
){
    AMI_BDS_CONNECT_POLICY Policy;
    EFI_STATUS Status;

    if ( File == NULL ) return EFI_SUCCESS;
    if ( AmiConnectPolicyLibAmiBdsConnectPolicy == NULL){
        Status = gBS->LocateProtocol(
            &gAmiBdsConnectPolicyProtocolGuid, NULL,
            (VOID**)&AmiConnectPolicyLibAmiBdsConnectPolicy
        );
        if (EFI_ERROR(Status)) return EFI_SUCCESS;
    }
    Status = AmiConnectPolicyLibAmiBdsConnectPolicy->GetDevicePathPolicy(
        AmiConnectPolicyLibAmiBdsConnectPolicy, File, &Policy
    );
    return (Status==EFI_SUCCESS && Policy==AmiBdsConnectPolicyDisable) ? EFI_SECURITY_VIOLATION : EFI_SUCCESS;
}

/**
  Library constructor. Called in Driver's entry point.
  @param ImageHandle     Image handle.
  @param SystemTable    Pointer to the EFI system table.

  @retval: EFI_SUCCESS
**/
EFI_STATUS EFIAPI AmiConnectPolicyLibConstructor(
    IN  EFI_HANDLE ImageHandle, IN  EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS Status;

    Status = RegisterSecurity2Handler(
        AmiConnectPolicyHandler,
        EFI_AUTH_OPERATION_CONNECT_POLICY | EFI_AUTH_OPERATION_VERIFY_IMAGE
    );
    ASSERT_EFI_ERROR (Status);
    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
