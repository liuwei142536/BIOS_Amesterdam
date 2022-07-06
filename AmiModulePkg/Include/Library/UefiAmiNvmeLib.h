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
//**********************************************************************l
/** @file DxeAmiNvmeLib.h
  @brief Defines DxeAmiNvmeLib library class.

**/
#ifndef __DXE_AMI_NVME_LIB__H__
#define __DXE_AMI_NVME_LIB__H__

#include <Protocol/NvmExpressPassthru.h>

/**
    Returns whether the NVMe device freeze locked or not.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] FreezeLocked           Flag for returning the device freeze lock status.

    @retval EFI_SUCCESS                 Device freeze lock state obtained successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol/BlockSid feature is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeFrozen(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *FreezeLocked
);

/**
    Returns whether the NVMe device locked or not.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] DeviceLocked           Flag for returning the device lock status.

    @retval EFI_SUCCESS                 Device lock state obtained successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeLocked(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *DeviceLocked
);

/**
    Returns whether BlockSID command supported by the NVMe device.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] BlockSIDFeatureSupport Flag for returning the status of Block Sid support.

    @retval EFI_SUCCESS                 Device supports Block SID command.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeBlockSidSupported(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *BlockSidFeatureSupport
);

/**
    Issues the Enable BlockSID command for the NVMe device.

    @param [in] EfiNvmExpressPassThru   Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.

    @retval EFI_SUCCESS                 Block SID command is sent successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiNvmeIssueBlockSid(
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru
);

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
