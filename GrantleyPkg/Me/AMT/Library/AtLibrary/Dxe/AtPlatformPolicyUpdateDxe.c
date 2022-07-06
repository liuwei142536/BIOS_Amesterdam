/**
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/

/**

Copyright (c) 2012, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtPlatformPolicyUpdateDxe.c

@brief
  At platform policy update.

**/
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AtPlatformPolicyUpdateDxeLib.h>
#include <Guid/SetupVariable.h>
#include "MeLib.h"

EFI_STATUS
EFIAPI
UpdateDxeAtPlatformPolicy (
  IN OUT  DXE_AT_POLICY_PROTOCOL *DxeAtPlatformPolicy
  )
/**

  @brief
  Install DxeAtPlatformPolicy with setup values.

  @param[in] DxeAtPlatformPolicy  DXE At Platform Policy.

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
{
  EFI_STATUS                Status;
  UINT32                    MeMode;
  EFI_HECI_PROTOCOL         *Heci;
  SYSTEM_CONFIGURATION      SetupDataBuffer;
  SYSTEM_CONFIGURATION      *SetupData;
  UINTN                     SetupVariableSize;
  UINT32                    SetupVariableAttributes;

  SetupData = &SetupDataBuffer;

  DEBUG ((EFI_D_INFO, "Enter InstallDxeAtPlatformPolicy\n"));

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  ASSERT_EFI_ERROR (Status);

  Status = Heci->GetMeMode (&MeMode);
  ASSERT_EFI_ERROR (Status);

  SetupVariableSize = sizeof (SYSTEM_CONFIGURATION);

  //
  // Get Management configuration from Setup Data
  //
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  &SetupVariableAttributes,
                  &SetupVariableSize,
                  &SetupDataBuffer
                  );

  if (!EFI_ERROR (Status)) {
    DxeAtPlatformPolicy->At.AtAmBypass           = SetupData->AtAmBypass;
    DxeAtPlatformPolicy->At.AtEnterSuspendState  = SetupData->AtEnterSuspendState;
    DxeAtPlatformPolicy->At.AtRecoveryAttempt    = SetupData->AtRecoveryAttempt;

    if(SetupData->AtEnterSuspendState == 1) {
      //
      // Enter Suspend State BIOS Setup Option is only valid for one boot
      //
      SetupData->AtEnterSuspendState = 0;
      // AptioV Server Override: Changed variable name to IntelSetup
      Status = gRT->SetVariable (
                      L"IntelSetup",
                      &gEfiSetupVariableGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      SetupVariableSize,
                      &SetupDataBuffer
                      );
    }
  }
  return Status;
}
