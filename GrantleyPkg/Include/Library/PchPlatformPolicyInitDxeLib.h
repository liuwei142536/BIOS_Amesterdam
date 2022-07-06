/*++
  This file contains a 'Sample Driver' and is licensed as such  
  under the terms of your license agreement with Intel or your  
  vendor.  This file may be modified by the user, subject to    
  the additional terms of the license agreement                 
--*/
/*++

Copyright (c) 2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  PchPlatformPolicyUpdateDxeLib.h

Abstract:

  Header file for DXE PchPlatformPolicyUpdate Library. 
  
--*/
#ifndef _PCH_PLATFORM_POLICY_UPDATE_DXE_LIB_H_
#define _PCH_PLATFORM_POLICY_UPDATE_DXE_LIB_H_
#ifdef DYNAMIC_MMCFG_BASE_FLAG //AptioV server override: dynamic MMCFG base change support.
#include <PchAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#endif
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/PlatformType.h>
#include <Guid/SetupVariable.h>

EFI_STATUS
EFIAPI
InitDxePchPlatformPolicy (
  IN SYSTEM_CONFIGURATION                   *SetupVariables,  
  IN EFI_PLATFORM_TYPE_PROTOCOL             *mPlatformType
  )
/*++

  @brief 
  Initilize Intel PCH DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
;

EFI_STATUS
EFIAPI
UpdateDxePchPlatformPolicy (
  IN SYSTEM_CONFIGURATION                       *SetupVariables,  
  IN EFI_PLATFORM_TYPE_PROTOCOL                 *mPlatformType,
  IN OUT DXE_PCH_PLATFORM_POLICY_PROTOCOL       *DxePlatformPchPolicy
  )
/*++
 
Routine Description:
 
  This function updates Dxe Pch Platform Policy
  
Arguments:
 
  SetupVariables                 Setup configuration values 
  mPlatformType                  Platformtype Info
 
Returns:
 
  EFI_SUCCESS                     Initialization complete.
  EFI_UNSUPPORTED                 The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES            Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR                Device error, driver exits abnormally.
 
--*/
;
#endif
