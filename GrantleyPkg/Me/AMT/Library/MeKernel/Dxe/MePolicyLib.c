/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2006 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  MePolicyLib.c

@brief:
  Implementation file for Me Policy functionality

**/

#include "Library/UefiBootServicesTableLib.h"
#include "Library/DebugLib.h"
#include "Library/MePolicyLib.h"
#include "MeAccess.h"
#include "Library/MeTypeLib.h"

///
/// Global variables
///
DXE_ME_POLICY_PROTOCOL  *mDxePlatformMePolicy = NULL;

EFI_STATUS
MePolicyLibInit (
  VOID
  )
/**

  Check if Me is enabled.

  @param[in] None.

**/
{
  EFI_STATUS  Status;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif //SPS_SUPPORT

  if (mDxePlatformMePolicy != NULL) {
    return EFI_SUCCESS;
  }
  //
  // Get the desired platform setup policy.
  //
  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &mDxePlatformMePolicy);
  return Status;
}

BOOLEAN
MeEndOfPostEnabled (
  VOID
  )
/**

  Check if End of Post Message is enabled in setup options.

  @param[in] None.

  @retval FALSE                   EndOfPost is disabled.
  @retval TRUE                    EndOfPost is enabled.

**/
{
  BOOLEAN    Supported;
  EFI_STATUS Status;

  if (mDxePlatformMePolicy == NULL) {
    Status = MePolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  if (mDxePlatformMePolicy->MeConfig.EndOfPostEnabled != 1) {
    Supported = FALSE;
  } else {
    Supported = TRUE;
  }

  return Supported;
}

BOOLEAN
IsAfterEndOfPost (
  VOID
  )
/**

  Checks if EndOfPost event already happened

  @param[in] None
  @retval TRUE                    if end of post happened
  @retval FALSE                   if not yet

**/
{
  EFI_STATUS              Status;

  if (mDxePlatformMePolicy == NULL) {
    Status = MePolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }

  if (mDxePlatformMePolicy->MeConfig.EndOfPostDone == TRUE) {
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
SetAfterEndOfPost (
  BOOLEAN vEOP
  )
/**

  Sets EndOfPost event value. Introduced for ICC support

  @param[in]
  @retval TRUE                    if end of post happened
  @retval FALSE                   if not yet

**/
{
  EFI_STATUS              Status;

  if (mDxePlatformMePolicy == NULL) {
    Status = MePolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }

  mDxePlatformMePolicy->MeConfig.EndOfPostDone = vEOP;

  return IsAfterEndOfPost();
}


VOID
MeReportError (
  ME_ERROR_MSG_ID MsgId
  )
/**

  Show Me Error message.

  @param[in] MsgId                Me error message ID.


**/
{
  EFI_STATUS              Status;

  if (mDxePlatformMePolicy == NULL) {
    Status = MePolicyLibInit();
    if (EFI_ERROR(Status)) {
      return;
    }
  }
  mDxePlatformMePolicy->MeReportError (MsgId);

  return ;
}

BOOLEAN
MeFwDowngradeSupported (
  VOID
  )
/**

  Check if MeFwDowngrade is enabled in setup options.

  @param[in] None.

  @retval FALSE                   MeFwDowngrade is disabled.
  @retval TRUE                    MeFwDowngrade is enabled.

**/
{
  EFI_STATUS              Status;

  if (mDxePlatformMePolicy == NULL) {
    Status = MePolicyLibInit();
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
  }
  if (mDxePlatformMePolicy->MeConfig.MeFwDownGrade == 1) {
    return TRUE;
  }

  return FALSE;
}
