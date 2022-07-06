/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MePeiPlatformPolicyUpdate.c

Abstract:

  Do Platform Stage ME initialization.

--*/
#include <Guid/SetupVariable.h>
#include <Guid/ServerMgmtSetupVariable.h>
#include <Ppi/ReadOnlyVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Protocol/MePlatformPolicyPei.h>
#include "Library/DebugLib.h"
#include "Library/MeTypeLib.h"

EFI_STATUS
UpdatePeiMePlatformPolicy (
  IN      EFI_PEI_SERVICES            **PeiServices,
  IN OUT  PEI_ME_PLATFORM_POLICY_PPI  *PeiMePlatformPolicyPpi
  )
/*++

Routine Description:

  Install the Me Platform Policy Library

Arguments:

  PeiServices             General purpose services available to every PEIM.
  PeiMePlatformPolicyPpi  Me Platform Policy Ppi

Returns:

  EFI_SUCCESS           Initialization complete.
  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  EFI_NOT_READY         Not all required services are loaded. Try later.

--*/
{
  EFI_STATUS                  Status = EFI_SUCCESS;
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  SYSTEM_CONFIGURATION            SystemConfiguration;
  UINTN                           VarSize;

  Status = EFI_SUCCESS;
  //
  // Locate system configuration variable
  //
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
                            0,                                // INSTANCE
                            NULL,                             // EFI_PEI_PPI_DESCRIPTOR
                            &VariableServices                 // PPI
                            );
  ASSERT_EFI_ERROR (Status);

  if (Status != EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "Error: System configuration variable cannot be located\n"));
      return EFI_NOT_READY;
  }

  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status = VariableServices->GetVariable (
                              VariableServices,
                              PLATFORM_SETUP_NVDATA_NVNAME,
                              &gEfiSetupVariableGuid,
                              NULL,
                              &VarSize,
                              &SystemConfiguration
                              );
  if (Status == EFI_SUCCESS) {
    PeiMePlatformPolicyPpi->DidEnabled = SystemConfiguration.MeDidEnabled;
    PeiMePlatformPolicyPpi->DidTimeout = SystemConfiguration.MeDidTimeout;
    PeiMePlatformPolicyPpi->DidInitStat = SystemConfiguration.MeDidInitStat;
  }
#endif
  return Status;
}
