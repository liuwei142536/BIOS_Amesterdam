/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

@copyright
  Copyright (c)  2006 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AmtPolicyLibPei.h

@brief
  Header file for PEI AMT Policy functionality

**/
#ifndef _PEI_AMT_POLICY_LIB_H_
#define _PEI_AMT_POLICY_LIB_H_

#include "AmtPlatformPolicyPei.h"

EFI_STATUS
PeiAmtPolicyLibInit (
  EFI_PEI_SERVICES             **PeiServices,
  PEI_AMT_PLATFORM_POLICY_PPI  **PeiAmtPlatformPolicy
  )
/**

  Check if Amt is enabled in setup options.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PeiAmtPlatformPolicy The AMT Platform Policy protocol instance

  @retval EFI_SUCCESS             AMT platform policy Ppi located
  @retval All other error conditions encountered result in an ASSERT.

**/
;

BOOLEAN
PeiAmtWatchDog (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  Check if AMT WatchDog is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   AMT WatchDog is disabled.
  @retval TRUE                    AMT WatchDog is enabled.

**/
;

UINT16
PeiAmtWatchTimerBiosGet (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  Get WatchDog BIOS Timmer.

  @param[in] PeiServices          Point to Pei Services structure

  @retval UINT16                  WatchDog BIOS Timer

**/
;

BOOLEAN
PeiAmtSupported (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  Check if AMT is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   ActiveManagement is disabled.
  @retval TRUE                    ActiveManagement is enabled.

**/
;

BOOLEAN
PeiAsfSupported (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  Check if ASF is enabled in setup options.

  @param[in] PeiServices          Point to Pei Services structure

  @retval FALSE                   ASF is disabled.
  @retval TRUE                    ASF is enabled.

**/
;

UINT8
ManageabilityModeSetting (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  Provide Manageability Mode setting from MEBx BIOS Sync Data

  @param[in] PeiServices          Point to Pei Services structure

  @retval UINT8                   Manageability Mode = MNT_AMT or MNT_ASF

**/
;

BOOLEAN
PeiFwProgressSupport (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**

  This will return progress event Option.
  True if the option is enabled.

  @param[in] PeiServices          Point to Pei Services structure

  @retval True                    progress event is enabled.
  @retval False                   progress event is disabled.

**/
;

#endif
