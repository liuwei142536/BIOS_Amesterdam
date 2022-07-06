/**

Copyright (c) 2004 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtDxePolicyInit.h

@brief
  AtDxePolicyInit to check and set At Platform Policy.

**/
/**
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
**/

#ifndef _AT_PLATFORM_POLICY_H_
#define _AT_PLATFORM_POLICY_H_

// #include EFI_PROTOCOL_PRODUCER (MePlatformPolicy)
// #include EFI_PROTOCOL_PRODUCER (AtPlatformPolicy)
// #include EFI_GUID_DEFINITION (MeBiosExtensionSetup)
#include "MeLib.h"
#include "AtPlatformPolicyUpdateDxeLib.h"

EFI_STATUS
EFIAPI
AtDxePolicyInitEntryPoint (
  IN  EFI_HANDLE                  ImageHandle,
  IN  EFI_SYSTEM_TABLE            *SystemTable
  )
/**

  @brief
  Initilize Intel At DXE Platform Policy

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.

**/
;
#endif
