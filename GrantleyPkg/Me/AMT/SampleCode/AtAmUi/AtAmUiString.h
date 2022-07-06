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
  AtAmUiString.h

@brief
  Heder file.

**/

/**

 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement

**/
#ifndef _ATAMUISTRING_H_
#define _ATAMUISTRING_H_

typedef enum _AT_AM_UI_STRING_ID {
  STR_ATAMUI_LOCKED_TEXT,
  STR_ATAMUI_PLATFORM_ID,
  STR_ATAMUI_PROVIDER_ID,
  STR_ATAMUI_LOCK_DUE_TO,
  STR_ATAMUI_SELECT_PASS,
  STR_ATAMUI_TIME_LEFT,
  STR_ATAMUI_USER_PASS,
//Grangeville AptioV server override - CPP Check
  STR_ATAMUI_RECO_ENTER_PASS,
  STR_ATAMUI_SERV_BASED_RECOV,
  STR_ATAMUI_INVALID_SELECT,
  STR_ATAMUI_LOCK_TIME,
  STR_ATAMUI_LOCK_STOLEN,
  STR_ATAMUI_LOCK_THRESHOLD,
  STR_ATAMUI_LOCK_ATTACK,
  STR_ATAMUI_CHECKING_PASS,
  STR_ATAMUI_RECO_TRY_AGAIN,
  STR_ATAMUI_RECO_FAILED,
  STR_ATAMUI_RECO_SUCCESS,
  STR_ATAMUI_SUS_ENTER_INFO,
  STR_ATAMUI_SUS_REFER,
  STR_ATAMUI_SUS_ENTER_TOKEN,
  STR_ATAMUI_SUS_ATTEMPT_EXC,
  STR_ATAMUI_SUS_ENTER_SUCCESS,
  STR_ATAMUI_SUS_ENTER_FAILED,
  STR_ATAMUI_SUS_AUTH_FAILED,
  STR_ATAMUI_SUS_EXIT_QUESTION,
  STR_ATAMUI_SUS_EXIT_INFO,
  STR_ATAMUI_SUS_EXIT_SUCCESS,
  STR_ATAMUI_SUS_STAY,
  STR_ATAMUI_SUS_NONCE_FAILED,
//Grangeville AptioV server override - CPP Check
//STR_ATAMUI_RECO_ENTER_PASS,
  STR_ATAMUI_TIME_LEFT_SEC,
  STR_ATAMUI_SELECT_OPTION,
  STR_ATAMUI_CHECKING_AUTHENT,
  STR_ATAMUI_FAILED_TO_AUTHENT,

  ///
  /// this has to be a last element of this enum
  ///
  STR_ATAMUI_END_OF_TAB
} AT_AM_UI_STRING_ID;

typedef struct _AT_AM_UI_STRING {
  UINTN   StrID;
  UINT8   X;
  UINT8   Y;
  CHAR16  *String;
} AT_AM_UI_STRING;

#define AT_AM_UI_1_SECOND 1000000

EFI_STATUS
AtAmUiStrGetChar (
  EFI_INPUT_KEY *key
  )
/**

  @brief
  This function gets char.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrDisplayFix (
  UINT32 StringID,
  UINT32 UseXY
  )
/**

  @brief
  This routine displays the string out to the screen based on defined strings.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrDisplay (
  CHAR16 *String
  )
/**

  @brief
  This routine displays the string out to the screen.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrClearScreen (
  VOID
  )
/**

  @brief
  This routine clears the screen.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrSetCursor (
  UINT32 X,
  UINT32 Y
  )
/**

  @brief
  In text mode it is used to set the place of cursor row and cursor column.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrEnableCursor (
  BOOLEAN Enabled
  )
/**

  @brief
  This function enables or disables cursor.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiStrGetCursor (
  UINT32 *X,
  UINT32 *Y
  )
/**

  @brief
  In text mode it returns the place of cursor row and cursor column.

  @retval EFI_SUCCESS       The function completed successfully.

**/
;

EFI_STATUS
AtAmUiDelay (
  UINT32 delay
  )
/**

  Function handling delay code.

  @retval EFI_SUCCESS                 The function completed successfully.

**/
;

#endif
