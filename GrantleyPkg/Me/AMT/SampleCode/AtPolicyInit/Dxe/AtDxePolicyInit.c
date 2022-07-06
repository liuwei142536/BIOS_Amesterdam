/**

Copyright (c) 2012 -2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtPlatformPolicy.c

@brief
  AtPlatformPolicy to check and set At Platform Policy.

**/
/**
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
**/

#include <Library/UefiBootServicesTableLib.h>
#include "Library/AtPlatformPolicyUpdateDxeLib.h"
#include "AtDxePolicyInit.h"
#include "Library/MeTypeLib.h"

DXE_AT_POLICY_PROTOCOL mAtPlatformPolicyInstance  = { 0 };


EFI_STATUS
EFIAPI
AtDxePolicyInitEntryPoint (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
/**

  @brief
  Entry point for the AtPlatformPolicy Driver.

  @param[in] ImageHandle  Image handle of this driver.
  @param[in] SystemTable  Global system service table.

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
{
  EFI_STATUS  Status;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif //SPS_SUPPORT

  ///
  /// AT DXE Policy Init
  ///
  mAtPlatformPolicyInstance.Revision                   = DXE_PLATFORM_AT_POLICY_PROTOCOL_REVISION_1;

  ///
  /// Initialzie the AT Configuration
  ///
  mAtPlatformPolicyInstance.At.AtAmBypass            = 1;
  mAtPlatformPolicyInstance.At.AtEnterSuspendState   = 0;
  mAtPlatformPolicyInstance.At.AtRecoveryAttempt     = 3;

  UpdateDxeAtPlatformPolicy (&mAtPlatformPolicyInstance);

  ///
  /// Install the AT Platform Policy PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
               &ImageHandle,
               &gDxePlatformAtPolicyGuid,
               &mAtPlatformPolicyInstance,
               NULL
             );

  return Status;
}
