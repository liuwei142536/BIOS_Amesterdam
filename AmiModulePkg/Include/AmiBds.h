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
/** @file
  BDS definitions.
**/
#ifndef __AMI_BDS__H__
#define __AMI_BDS__H__
/// @name BDS Execution Policy Flags.
/// These values define BDS execution policy expressed via
/// AmiPcdBdsPolicyMask and AmiPcdBdsPolicyMaskOnRecoveryAndFlashUpdate PCD tokens.
///@{

/// Do not process DriverXXX and SysPrepXXX variables when secure boot is disabled
#define AMI_BDS_POLICY_VAR_XXX_ON_SECURE_BOOT_ONLY 0x1
/// Do not process DriverXXX and SysPrepXXX variables
#define AMI_BDS_POLICY_NO_VAR_XXX 0x3
#define AMI_BDS_POLICY_VAR_XXX_MASK 0x3

/// Do not launch third party option ROM when secure boot is disabled
#define AMI_BDS_POLICY_EXTERNAL_OPROM_ON_SECURE_BOOT_ONLY 0x4
/// Do not launch third party option ROM
#define AMI_BDS_POLICY_NO_EXTERNAL_OPROM 0xC
#define AMI_BDS_POLICY_EXTERNAL_OPROM_MASK 0xC

///@}
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
