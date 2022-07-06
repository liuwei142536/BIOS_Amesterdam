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
  MePolicyLib.h

@brief:
  Header file for Me Policy functionality

**/
#ifndef _ME_POLICY_LIB_H_
#define _ME_POLICY_LIB_H_

#include "MePlatformPolicy.h"

EFI_STATUS
MePolicyLibInit (
  VOID
  )
/**

  Check if Me is enabled.

  @param[in] None.

**/
;

BOOLEAN
MeEndOfPostEnabled (
  VOID
  )
/**

  @brief

  Check if End of Post Message is enabled in setup options.

  @param[in] None.

  @retval FALSE                   EndOfPost is disabled.
  @retval TRUE                    EndOfPost is enabled.

**/
;


VOID
MeReportError (
  ME_ERROR_MSG_ID MsgId
  )
/**

  @brief
  Show Me Error message.

  @param[in] MsgId                Me error message ID.


**/
;

BOOLEAN
MeFwDowngradeSupported (
  VOID
  )
/**

  @brief
  Check if MeFwDowngrade is enabled in setup options.

  @param[in] None.

  @retval FALSE                   MeFwDowngrade is disabled.
  @retval TRUE                    MeFwDowngrade is enabled.

**/
;

VOID
DxeMePolicyDebugDump (
  VOID
  )
/**

  Dump DXE ME Platform Policy

  @param[in] None.

**/
;

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
;

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
;
#endif
