/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AtAmProtocol.h

@brief
  Header file for ATAM

**/
#ifndef _DXE_ATAM_PROTOCOL_H_
#define _DXE_ATAM_PROTOCOL_H_

#include <Protocol/AtAmProtocol.h>
#include "MkhiMsgs.h"

///
/// Define the  protocol GUID
///
#define EFI_ATAM_PROTOCOL_GUID \
  { \
    0x8aebaa5a, 0x556f, 0x404b, 0x96, 0x05, 0x38, 0xe0, 0xde, 0x29, 0x76, 0x2f \
  }

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gEfiAtProtocolGuid;

#define ATAM_PROTOCOL_REVISION  1

//
// Forward reference for ANSI C compatibility
//

#define ISV_PLATFORM_ID_LENGTH  16
#define SERVER_SHORTCODE_LENGTH 16
#define DEFAULT_LANGUAGE_STRING 4

//
// Forward reference for ANSI C compatibility
//
#ifndef EFI_FORWARD_DECLARATION
#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
#endif
EFI_FORWARD_DECLARATION (EFI_ATAM_PROTOCOL);

#pragma pack(1)
typedef struct {

  UINT8   BiosLanguageSupport[DEFAULT_LANGUAGE_STRING];
  UINT8   Reserved[2];

  ///
  /// Defines the number which will be used by BIOS before making BIOS AM as a
  /// default recovery mechanism
  ///
  UINT16  PbaOverRideThreshold;

  ///
  /// The value used by BIOS to try sending GPS location when AT stolen
  ///
  UINT8   BiosLocationBeconing;

  ///
  /// This will be used for Notebook transfer authorization request
  ///
  UINT8   PlatformAuthReq;

  ///
  /// This will be used for Notebook transfer authorization acknowledgement
  ///
  UINT8   PlatformAuthAck;

  ///
  /// This will be used for AT Server short code
  ///
  UINT8   ServerShortCode[SERVER_SHORTCODE_LENGTH];

  ///
  /// This allow ISV to set unique platform ID and will be used for displayed on need basis
  ///
  UINT16  IsvPlatformId[ISV_PLATFORM_ID_LENGTH];

} AT_BIOS_RECOVERY_CONFIG;
#pragma pack()

///
/// Protocol definitions
///
typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_ISV_ID) (
  IN  EFI_ATAM_PROTOCOL *This,
  OUT UINT8             *IsvString,
  OUT UINT32            *IsvId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_RECOVERY_CONFIG) (
  IN  EFI_ATAM_PROTOCOL       *This,
  OUT AT_BIOS_RECOVERY_CONFIG *RecoveryConfig
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_TIMER) (
  IN  EFI_ATAM_PROTOCOL *This,
  OUT UINT32            *TimeLeft,
  OUT UINT32            *TimeInterval
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_NONCE) (
  IN  EFI_ATAM_PROTOCOL *This,
  OUT UINT8             *NonceStr
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_VERIFY_PASSWORD) (
  IN  EFI_ATAM_PROTOCOL *This,
  IN  UINT8             *PasswordEntered,
  IN  UINT32            PassType,
  OUT UINT8             *IsAuthenticated
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_ATTEMPTS) (
  IN  EFI_ATAM_PROTOCOL *This,
  OUT UINT32            *Attempts
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_SET_SUSPEND_STATE) (
  IN EFI_ATAM_PROTOCOL  *This,
  IN UINT32             TransitionState,
  IN UINT8              *Token
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ATAM_GET_AT_STATE_INFO) (
  IN  EFI_ATAM_PROTOCOL *This,
  OUT AT_STATE_INFO     *AtStateInfo
  );

///
/// Protocol definition
///
typedef struct _EFI_ATAM_PROTOCOL {
  UINT8                         Revision;
  EFI_ATAM_GET_ISV_ID           AtAmGetIsvId;
  EFI_ATAM_GET_RECOVERY_CONFIG  AtAmGetRecoveryConfig;
  EFI_ATAM_GET_TIMER            AtAmGetTimer;
  EFI_ATAM_GET_NONCE            AtAmGetNonce;
  EFI_ATAM_VERIFY_PASSWORD      AtAmVerifyPassword;
  EFI_ATAM_GET_ATTEMPTS         AtAmGetAttempts;
  EFI_ATAM_SET_SUSPEND_STATE    AtAmSetSuspendState;
  EFI_ATAM_GET_AT_STATE_INFO    AtAmGetAtStateInfo;
} EFI_ATAM_PROTOCOL;

extern EFI_GUID  gEfiAtAmProtocolGuid;

#endif
