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
  AtAmUiString.c

@brief
 This file contines function used for handling strings.

**/

/**

 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement

**/
#include <Library/UefiBootServicesTableLib.h>
#include "AtAmUiString.h"

AT_AM_UI_STRING AtAmUiUSStringIDTab[] = {
  {STR_ATAMUI_LOCKED_TEXT,      4,  19, L"This device has been locked and may have been lost.\r\n    If found, Please use the following information to return the device\r\n    "},
  {STR_ATAMUI_PLATFORM_ID,      4,  22, L"Platform ID: "},
  {STR_ATAMUI_PROVIDER_ID,      4,  23, L"Intel(R) Anti-Theft service provider Id: "},
  {STR_ATAMUI_LOCK_DUE_TO,      4,  2,  L"Intel(R) Anti-Theft system lock due to: "},
  {STR_ATAMUI_SELECT_PASS,      4,  4,  L"Please select one of the following for platform recovery:\r\n    1 - User Password\r\n    2 - Server Token Password"},
  {STR_ATAMUI_TIME_LEFT,        4,  3,  L"Time Left to enter Password: "},
  {STR_ATAMUI_USER_PASS,        4,  4,  L"System Recovery"},
  {STR_ATAMUI_RECO_ENTER_PASS,  4,  5,  L"Enter Password: "},
  {STR_ATAMUI_SERV_BASED_RECOV, 4,  5,  L"Server Based Recovery"},
  {STR_ATAMUI_INVALID_SELECT,   4,  9,  L"Invalid Selection, Press 1 or 2  "},
  {STR_ATAMUI_LOCK_TIME,        4,  0,  L"Disable Timer expired"},
  {STR_ATAMUI_LOCK_STOLEN,      4,  0,  L"Stolen Message received"},
  {STR_ATAMUI_LOCK_THRESHOLD,   4,  0,  L"Logon Threshold exceeded"},
  {STR_ATAMUI_LOCK_ATTACK,      4,  0,  L"Platform Attack detected"},
  {STR_ATAMUI_CHECKING_PASS,    4,  7,  L"Checking Recovery Password..."},
  {STR_ATAMUI_RECO_TRY_AGAIN,   4,  9,  L"Recovery Failed. Please try again ..."},
  {STR_ATAMUI_RECO_FAILED,      4, 10,  L"Intel(R) Anti-Theft Recovery Failed.\r\n    System will shutdown..."},
  {STR_ATAMUI_RECO_SUCCESS,     4, 10,  L"Intel(R) Anti-Theft Recovery Successful.\r\n    System Boot Continue ....."},
  {STR_ATAMUI_SUS_ENTER_INFO,   4,  1,  L"User Has Requested To Enter Intel(R) Anti-Theft Suspend Mode....."},
  {STR_ATAMUI_SUS_REFER,        4,  4,  L"Refer the following Platform ID to IT: "},
  {STR_ATAMUI_SUS_ENTER_TOKEN,  4,  3,  L"Enter Server based Suspend Token: "},
  {STR_ATAMUI_SUS_ATTEMPT_EXC,  4,  8,  L"Exceeded Max Attempts - Exiting ....      "},
  {STR_ATAMUI_SUS_ENTER_SUCCESS,4,  4,  L"Successfully Put Platform in Suspended Mode"},
  {STR_ATAMUI_SUS_ENTER_FAILED, 4,  4,  L"Failed to Put Platform into Suspended Mode"},
  {STR_ATAMUI_SUS_AUTH_FAILED,  4,  4,  L"Failed to Authenticate                    "},
  {STR_ATAMUI_SUS_EXIT_QUESTION,4,  1,  L"Intel(R) Anti-Theft in Suspended State: Exit? (y/n)?"},
  {STR_ATAMUI_SUS_EXIT_INFO,    4,  2,  L"Exit Suspend Mode....."},
  {STR_ATAMUI_SUS_EXIT_SUCCESS, 4,  6,  L"Successfully Exited Suspend Mode"},
  {STR_ATAMUI_SUS_STAY,         4,  6,  L"Stay in Suspend Mode...."},
  {STR_ATAMUI_SUS_NONCE_FAILED, 0,  0,  L"NONCE unknown"},
  {STR_ATAMUI_TIME_LEFT_SEC,    0,  0,  L" Second"},
  {STR_ATAMUI_SELECT_OPTION,    4,  8,  L"Select one of the above options to proceed ..."},
  {STR_ATAMUI_CHECKING_AUTHENT, 4,  5,  L"Checking Authentication .."},
//Grangeville AptioV server override - CPP Check
  {STR_ATAMUI_FAILED_TO_AUTHENT,4,  4,  L"Authentication Failed .."}
};

EFI_STATUS
AtAmUiStrGetChar (
  EFI_INPUT_KEY *Key
  )
/**

  This function gets char.

  @retval EFI_SUCCESS       The function completed successfully.

**/
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN Index;
  EFI_TPL CurrentTpl = 0; 

  // lower the TPL so that key polling process won't block system events(Ex: Hot-plug events) happening higher TPL
  CurrentTpl = gBS->RaiseTPL( TPL_HIGH_LEVEL );
  gBS->RestoreTPL( TPL_APPLICATION);


  // Check "WaitForKey" Event is signalled or not before reading the key.
  // "WaitForKey" Event is signalled at TPL_NOTIFY so that it will not cause race-condition with USB driver resources.
  gBS->WaitForEvent( 1, gST->ConIn->WaitForKey, &Index );
  Status = gST->ConIn->ReadKeyStroke (gST->ConIn, Key);
  // Restore back to the Original TPL
  gBS->RaiseTPL( TPL_HIGH_LEVEL );
  gBS->RestoreTPL( CurrentTpl );

  return Status;
}

EFI_STATUS
AtAmUiStrDisplayFix (
  UINT32 StringID,
  UINT32 UseXY
  )
/**

  This routine displays the string out to the screen based on defined strings.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{
  EFI_STATUS  Status;
  INTN        i;

  for (i = 0; i < STR_ATAMUI_END_OF_TAB; i++) {
    if (AtAmUiUSStringIDTab[i].StrID == StringID) {
      if (UseXY) {
        Status = gST->ConOut->SetCursorPosition (gST->ConOut, AtAmUiUSStringIDTab[i].X, AtAmUiUSStringIDTab[i].Y);
      }

      Status = gST->ConOut->OutputString (gST->ConOut, AtAmUiUSStringIDTab[i].String);
      return Status;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
AtAmUiStrDisplay (
  CHAR16 *String
  )
/**

  This routine displays the string out to the screen.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{

  return gST->ConOut->OutputString (gST->ConOut, String);
}

EFI_STATUS
AtAmUiStrClearScreen (
  VOID
  )
/**

  This routine clears the screen.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{

  return gST->ConOut->ClearScreen (gST->ConOut);
}

EFI_STATUS
AtAmUiStrSetCursor (
  UINT32 CCol,
  UINT32 CRow
  )
/**

  In text mode it is used to set the place of cursor row and cursor column.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{

  return gST->ConOut->SetCursorPosition (gST->ConOut, CCol, CRow);
}

EFI_STATUS
AtAmUiStrEnableCursor (
  BOOLEAN Enabled
  )
/**

  This function enables or disables cursor.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{

  return gST->ConOut->EnableCursor (gST->ConOut, Enabled);
}

EFI_STATUS
AtAmUiStrGetCursor (
  UINT32 *CRow,
  UINT32 *CCol
  )
/**

  In text mode it returns the place of cursor row and cursor column.

  @retval EFI_SUCCESS  The function completed successfully.

**/
{
  *CRow = gST->ConOut->Mode->CursorRow;
  *CCol = gST->ConOut->Mode->CursorColumn;

  return EFI_SUCCESS;
}

EFI_STATUS
AtAmUiDelay (
  UINT32 delay
  )
/**

  Function handling delay code.

  @retval EFI_SUCCESS   The function completed successfully.

**/
{

  return gBS->Stall (delay);;
}

