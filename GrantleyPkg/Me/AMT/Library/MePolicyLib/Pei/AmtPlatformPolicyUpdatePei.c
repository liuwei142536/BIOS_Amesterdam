/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AmtPlatformPolicyUpdatePei.c

Abstract:

  Do Platform Stage AMT initialization.

--*/
#include "token.h"    //AptioV server override 
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include "Guid/SetupVariable.h"
#include <Library/HobLib.h>
#include <Guid/AmtForcePushPetPolicy.h>
#include <Guid/SetupVariable.h>
#include <Guid/MeBiosExtensionSetup.h>
#include <Ppi/AmtPlatformPolicyPei.h>
#include <Guid/ServerMgmtSetupVariable.h>
#include <Ppi/ReadOnlyVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include "AMT/BiosExtension/Efi/BiosExtensionLoader/Dxe/BiosExtensionLoader.h"
#include "AlertStandardFormatCommon.h"
#include "Library/MeTypeLib.h"

CHAR16 gEfiMeBiosExtensionSetupName[]   = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;

EFI_FRAMEWORK_MESSAGE_TYPE  mAmtForcePushPetPolicy[] = {
  EfiAsfMessageChassisIntrusion,
  EfiAsfMessageUserAuthenticationInvalid,
  EfiAsfMessageHddAuthenticationInvalid,
};

EFI_STATUS
InstallAmtForcePushPetPolicy (
  IN EFI_PEI_SERVICES  **PeiServices
  )
/*++

Routine Description:

  Install the Active Management push PET policy

Arguments:

  PeiServices             General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
{
  //
  // Platform Policy
  //
  EFI_STATUS  Status = EFI_SUCCESS;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  //
  // Build HOB for setup memory information
  //
  BuildGuidDataHob (
            &gAmtForcePushPetPolicyGuid,
            &mAmtForcePushPetPolicy,
            sizeof (mAmtForcePushPetPolicy)
            );


  return Status;
}

EFI_STATUS
UpdatePeiAmtPlatformPolicy (
  IN      EFI_PEI_SERVICES            **PeiServices,
  IN OUT  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicyPpi
  )
/*++

Routine Description:

  Install the Active Management Policy Ppi Library

Arguments:

  PeiServices             General purpose services available to every PEIM.
  PeiAmtPlatformPolicyPpi PEI Amt PlatformPolicy

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

--*/
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  SYSTEM_CONFIGURATION            SetupData;
  ME_BIOS_EXTENSION_SETUP         MeBiosExtensionSetupData;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  Status = EFI_SUCCESS;
  //
  // Locate Variable Ppi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, &VariableServices);

  //
  // Make sure we have a PPI, if not, just return.
  //
  if (!VariableServices) {
    return EFI_SUCCESS;
  }
  //
  // Get Setup Variable
  //
  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  Status = VariableServices->GetVariable (
                              VariableServices,
                              PLATFORM_SETUP_NVDATA_NVNAME,
                              &gEfiSetupVariableGuid,
                              NULL,
                              &VariableSize,
                              &SetupData
                              );

  if (Status == EFI_SUCCESS) {
    PeiAmtPlatformPolicyPpi->iAmtEnabled        = SetupData.Amt;
    PeiAmtPlatformPolicyPpi->WatchDog           = SetupData.WatchDog;
    PeiAmtPlatformPolicyPpi->WatchDogTimerBios  = SetupData.WatchDogTimerBios;
    PeiAmtPlatformPolicyPpi->WatchDogTimerOs    = SetupData.WatchDogTimerOs;
    PeiAmtPlatformPolicyPpi->AsfEnabled         = SetupData.Asf;
    PeiAmtPlatformPolicyPpi->FWProgress       = SetupData.FWProgress;
  }

  //
  // Get BIOS Sync-up data from MEBx to AMT platform policy
  //
  VariableSize = sizeof (MeBiosExtensionSetupData);
  Status = VariableServices->GetVariable (
                              VariableServices,
                              gEfiMeBiosExtensionSetupName,
                              &gEfiMeBiosExtensionSetupGuid,
                              NULL,
                              &VariableSize,
                              &MeBiosExtensionSetupData
                              );
  if (Status != EFI_SUCCESS) {
    PeiAmtPlatformPolicyPpi->ManageabilityMode = 0;
  } else {
    PeiAmtPlatformPolicyPpi->ManageabilityMode = MeBiosExtensionSetupData.PlatformMngSel;
  }

  InstallAmtForcePushPetPolicy (PeiServices);

  return Status;
}
#endif
