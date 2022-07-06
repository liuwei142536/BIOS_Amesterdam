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
  AtAm.h

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
#ifndef _ATAM_H_
#define _ATAM_H_

#include "AtHi.h"
#include "MeLib.h"
#include "AtAmHelper.h"
#include "AtPolicyLib.h"
#include "At.h"
#include "Protocol/AtAmProtocol.h"
#include "Guid/SetupVariable.h"

//
// Used during initialization
//
// #include EFI_PROTOCOL_CONSUMER (FirmwareVolume)
// #include EFI_PROTOCOL_CONSUMER (HECI)
// #include EFI_PROTOCOL_CONSUMER (At)
// #include EFI_PROTOCOL_CONSUMER (AtPlatformPolicy)

///
/// Driver Produced Protocols
///
// #include EFI_PROTOCOL_PRODUCER (AtAm)

#define ATAM_PRIVATE_DATA_SIGNATURE EFI_SIGNATURE_32 ('A', 'T', 'A', 'M')

#define MAX_HEX_BYTES             20
#define ATAM_ENTER_SUSPEND_STATE  1
#define ATAM_EXIT_SUSPEND_STATE   0

#define PBA_FAILED_COUNT_VARIABLE_GUID \
  { \
    0x7c66ffdc, 0x423c, 0xe5d4, 0x25, 0x1b, 0x55, 0xad, 0xba, 0x95, 0x26, 0x98 \
  }

#pragma pack(1)

typedef struct {
  UINTN             Signature;
  EFI_HANDLE        Handle;
  EFI_ATAM_PROTOCOL AtAmProtocol;
} ATAM_INSTANCE;

#define ATAM_INSTANCE_FROM_ATAM_PROTOCOL(a) CR (a, ATAM_INSTANCE, AtAm, ATAM_PRIVATE_DATA_SIGNATURE)

#pragma pack()

#define NONCE_LENGTH                16
#define STR_NONCE_LENGTH            33
#define ATAM_SETUP_PASSWORD_LENGTH  49
#define ATAM_TIMER_STRING_LENGTH    10
#define ISV_PLATFORM_ID_LENGTH      16
#define SERVER_SHORTCODE_LENGTH     16
#define DEFAULT_LANGUAGE_STRING     4
#define RECOVERY_STRING_LENGTH      256
#define MX_SMS_MESSAGES             99

typedef enum _AT_AM_SELECTION
{
  AT_AM_SELECTION_ATAM = 0,
  AT_AM_SELECTION_PBAM,
  AT_AM_SELECTION_MAX
} AT_AM_SELECTION;

EFI_STATUS
AtAmGetIsvId (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT8           *IsvString,
  OUT UINT32          *IsvId
  )
/**

  This function gets the ISV Strings stored by AT Server that BIOS will display.

  @param[in] This        The address of protocol
  @param[out] IsvString  Isv string pointer
  @param[out] IsvId      Intel(R) Anti-Theft service provider Id

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmGetRecoveryConfig (
  IN  EFI_AT_PROTOCOL         *This,
  OUT AT_BIOS_RECOVERY_CONFIG *RecoveryConfig
  )
/**

  This function gets recovery config.

  @param[in]  This            The address of protocol
  @param[out] RecoveryConfig  Pointer to structure

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmGetTimer (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT32          *TimeLeft,
  OUT UINT32          *TimeInterval
  )
/**

  This function returns time left to enter password.

  @param[in] This          The address of protocol
  @param[out] TimeLeft     Time
  @param[out] TimeInterval Time interval

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmGetNonce (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT8           *NonceStr
  )
/**

  This function gets 16 bytes nonce from firmware and also converts it to string according to format "XXXX-XXXX-XXXX-XXXX-XXXX-XXXX-XX".

  @param[in] This          The address of protocol
  @param[out] NonceStr     Pointer to Nonce string

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmVerifyPassword (
  IN  EFI_AT_PROTOCOL *This,
  IN  UINT8           *PasswordEntered,
  IN  UINT32          PassType,
  OUT UINT8           *IsAuthenticated
  )
/**

  This routine receives the data (passphrase or SRTK) from UI and verifies it if the password (either passphrase or SRTK) is acceptable.

  @param[in]  This            The address of protocol
  @param[in]  PasswordEntered Pointer to string
  @param[in]  PassType        Password type
  @param[out] IsAuthenticated Pointer to result of verification

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmGetAttempts (
  IN  EFI_AT_PROTOCOL *This,
  OUT UINT32          *Attempts
  )
/**

  This routine returns number of attempts.

  @param[in]  This      The address of protocol
  @param[out] Attempts  Number of attempts

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmSetSuspendState (
  IN EFI_AT_PROTOCOL  *This,
  IN UINT32           TransitionState,
  IN UINT8            *Token
  )
/**

  This routine receives the SRTK from UI and verifies it if the password is acceptable. This requests FW to enter or exit Suspend mode based on user input.

  @param[in] This             The address of protocol
  @param[in] TransitionState  1- enter suspend state, 0 - exit suspend state
  @param[in] Token            Pointer to token

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmGetAtStateInfo (
  IN  EFI_AT_PROTOCOL *This,
  OUT AT_STATE_INFO   *AtStateInfo
  )
/**

  This routine gets AT state.

  @param[in]  This         The address of protocol
  @param[out] AtStateInfo  State of AT

  @retval EFI_SUCCESS   The function completed successfully.

**/
;

EFI_STATUS
AtAmValidatePreferredAM (
  UINT8  *AtState,
  UINT16 *AtAmPref
  )
/**

  Validate preffered AM.

  @param[in] AtState   State of AT
  @param[in] AtAmPref  Preffered AT authentication

  @retval EFI_SUCCESS  The function completed successfully.

**/
;

#endif // _ATAM_H_
