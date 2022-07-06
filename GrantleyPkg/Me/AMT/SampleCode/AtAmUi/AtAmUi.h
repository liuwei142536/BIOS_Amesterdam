/**

Copyright (c) 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file
  AtAmUi.h

@brief
 Header file.

**/

/**

 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement

**/
#ifndef _ATAMUI_H_
#define _ATAMUI_H_

#include "AtAmUiString.h"
#include "AtAm.h"
#include "AtHi.h"
#include "AtAmHelper.h"

// #include EFI_PROTOCOL_CONSUMER (AtAm)
// #include EFI_PROTOCOL_CONSUMER (AtPlatformPolicy)

#define ME_ALERT_AT_HANDLER_GUID \
  { \
    0xb441df87, 0x8d94, 0x4811, 0x85, 0xf7, 0xf, 0x9a, 0x7b, 0xf8, 0x9d, 0x2a \
  }

#define ATAMUI_SUSPEND_ATTEMPTS 3

EFI_STATUS
AtAmUiDisplayIsvStrings (
  VOID
  )
/**

  Function displaing Ivs strings.

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Unable to allocate necessary data structures.

**/
;

VOID
AtAmUiCallback (
  EFI_EVENT   Event,
  VOID        *ParentImageHandle
  )
/**

  ATAM UI callback.

  @param[in] Event
  @param[in] ParentImageHandle

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiEnterSuspendState (
  VOID
  )
/**

  Function handling entering suspend mode.

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Unable to allocate necessary data structures.

**/
;

EFI_STATUS
AtAmUiExitSuspendState (
  VOID
  )
/**

  Function handling exiting suspend mode.

  @retval EFI_SUCCESS                 The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to allocate memory.

**/
;

EFI_STATUS
AtAmUiGetPassword (
  OUT UINT8    *PasswordASCII,
  IN  INTN     MaxPasswordLength,
  OUT CHAR16   *PasswordUNICODE,
  IN  BOOLEAN  showPassword
  )
/**

  This GetRecoveryPassword() process the AT recovery password user input.

  @param[out] PasswordASCII          Pointer to an array of ASCII user input
  @param[in]  MaxPasswordLength      Integer value for password length
  @param[out] PasswordUNICODE        Pointer to an array of UNICODE user input
  @param[in]  showPassword           TRUE - password is shown, FALSE - pasword is hidden by *

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Do not have enough resources to allocate memory or password too long.

**/
;

EFI_STATUS
AtAmUiRecovery (
  VOID
  )
/**

  Function handling recovery proccess.

  @retval EFI_SUCCESS                 The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to allocate memory.

**/
;

#endif
