/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2006 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  AmtPolicyLibPei.c

@brief:

  Implementation file for AMT Policy functionality for PEIM

**/
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include "AmtStatusCode.h"
#include "AmtPlatformPolicyPei.h"
#include "AmtPolicyLibPei.h"

EFI_GUID  mPeiAmtStatusCodePpiGuid = PEI_AMT_STATUS_CODE_PPI_GUID;

EFI_STATUS
PeiAmtPolicyLibInit (
  EFI_PEI_SERVICES             **PeiServices,
  PEI_AMT_PLATFORM_POLICY_PPI  **PeiAmtPlatformPolicy
  )
/**

  @brief
  Check if Amt is enabled in setup options.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PeiAmtPlatformPolicy The AMT Platform Policy protocol instance

  @retval EFI_SUCCESS             AMT platform policy Ppi located
  @retval All other error conditions encountered result in an ASSERT.

**/
{
  EFI_STATUS  Status;

  ///
  /// Locate system configuration variable
  ///
  Status = PeiServicesLocatePpi (
            &gPeiAmtPlatformPolicyPpiGuid,  /// GUID
            0,                              /// INSTANCE
            NULL,                           /// EFI_PEI_PPI_DESCRIPTOR
            PeiAmtPlatformPolicy            /// PPI
            );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

BOOLEAN
PeiAmtWatchDog (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  Check if AMT WatchDog is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   AMT WatchDog is disabled.
  @retval TRUE                    AMT WatchDog is enabled.

**/
{
  EFI_STATUS                  Status;
  BOOLEAN                     Supported;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Supported = FALSE;
  if (ManageabilityModeSetting (PeiServices) != 0) {
    Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
    ASSERT_EFI_ERROR (Status);
    ///
    /// First check if AMT WatchDog is enabled in Setup.
    ///
    if (PeiAmtPlatformPolicy->WatchDog == 1) {
      Supported = TRUE;
    }
  }

  return Supported;
}

UINT16
PeiAmtWatchTimerBiosGet (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  Get WatchDog BIOS Timmer.

  @param[in] PeiServices          Point to Pei Services structure

  @retval UINT16                  WatchDog BIOS Timer

**/
{
  EFI_STATUS                  Status;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
  ASSERT_EFI_ERROR (Status);

  return PeiAmtPlatformPolicy->WatchDogTimerBios;
}

BOOLEAN
PeiAmtSupported (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  Check if AMT is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   ActiveManagement is disabled.
  @retval TRUE                    ActiveManagement is enabled.

**/
{
  EFI_STATUS                  Status;
  BOOLEAN                     Supported;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// First check if AMT support is enabled in Setup.
  ///
  if (PeiAmtPlatformPolicy->iAmtEnabled == 1) {
    Supported = TRUE;
  } else {
    Supported = FALSE;
  }

  return Supported;
}

BOOLEAN
PeiAsfSupported (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  Check if ASF is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   ASF is disabled.
  @retval TRUE                    ASF is enabled.

**/
{
  EFI_STATUS                  Status;
  BOOLEAN                     Supported;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
  ASSERT_EFI_ERROR (Status);

  if (PeiAmtPlatformPolicy->Revision < PEI_AMT_PLATFORM_POLICY_PPI_REVISION_2) {
    return FALSE;
  }
  ///
  /// First check if AMT support is enabled in Setup.
  ///
  if (PeiAmtPlatformPolicy->AsfEnabled == 1) {
    Supported = TRUE;
  } else {
    Supported = FALSE;
  }

  return Supported;
}

UINT8
ManageabilityModeSetting (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  Provide Manageability Mode setting from MEBx BIOS Sync Data

  @param[in] PeiServices          Point to Pei Services structure

  @retval UINT8                   Manageability Mode = MNT_AMT or MNT_ASF

**/
{
  EFI_STATUS                  Status;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
  ASSERT_EFI_ERROR (Status);

  if (PeiAmtPlatformPolicy->Revision < PEI_AMT_PLATFORM_POLICY_PPI_REVISION_2) {
    return 0;
  }

  return (UINT8) (PeiAmtPlatformPolicy->ManageabilityMode);
}

BOOLEAN
PeiFwProgressSupport (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  @brief
  This will return progress event Option.
  True if the option is enabled.

  @param[in] PeiServices          Point to Pei Services structure

  @retval True                    progress event is enabled.
  @retval False                   progress event is disabled.

**/
{
  EFI_STATUS                  Status;
  BOOLEAN                     Supported;
  PEI_AMT_PLATFORM_POLICY_PPI *PeiAmtPlatformPolicy;

  Status = PeiAmtPolicyLibInit (PeiServices, &PeiAmtPlatformPolicy);
  ASSERT_EFI_ERROR (Status);

  Supported = FALSE;

  if (PeiAmtPlatformPolicy->Revision >= PEI_AMT_PLATFORM_POLICY_PPI_REVISION_3) {
    ///
    /// Check if progress event is enabled in Setup.
    ///
    if (PeiAmtPlatformPolicy->FWProgress == 1) {
      Supported = TRUE;
    }
  }

  return Supported;
}
