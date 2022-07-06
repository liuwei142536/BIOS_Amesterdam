/** @file
  Header file for Common PchPolicyInit Library

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _PCH_PLATFORM_POLICY_UPDATE_COMMON_H_
#define _PCH_PLATFORM_POLICY_UPDATE_COMMON_H_

#include "Platform.h"
#include "PchAccess.h"
#include "PchUsbConfig.h"

#include <Guid/SetupVariable.h>

#include <Library/PchPlatformLib.h>
#include <PlatformInfo.h>

/**
  This function performs PCH USB Platform Policy initialzation

  @param[in] UsbConfig                  Pointer to PCH_USB_CONFIG data buffer.

  @retval NONE
**/
VOID
InitPchUsbConfig (
  IN PCH_USB_CONFIG                     *PchUsbConfig
  );

/**
  This function performs PCH USB Platform Policy initialzation

  @param[in] UsbConfig                       Pointer to PCH_USB_CONFIG data buffer
  @param[in] SetupVariables                  Pointer to Setup variable
  @param[in] PlatformType                    PlatformType specified
  @param[in] PlatformFlavor                  PlatformFlavor specified
  @param[in] BoardType                       BoardType specified

  @retval    NONE

**/
VOID
UpdatePchUsbConfig (
  IN PCH_USB_CONFIG            *UsbConfig,
  IN SYSTEM_CONFIGURATION      *SetupVariables,
  IN UINT8                     BoardId,
  IN EFI_PLATFORM_INFO         *PlatformInfoHobData
  );
#endif
