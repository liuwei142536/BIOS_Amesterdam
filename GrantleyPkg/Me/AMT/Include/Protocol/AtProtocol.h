/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c)  2004 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AtProtocol.h

@brief
  Defines and prototypes for the AT driver.
  This driver implements the AT protocol.

**/
#ifndef _DXE_AT_PROTOCOL_H_
#define _DXE_AT_PROTOCOL_H_
#include "MkhiMsgs.h"
///
/// Define the  protocol GUID
///
#define EFI_AT_PROTOCOL_GUID \
  { \
    0x2df8cdbe, 0x79ce, 0x48a0, 0x8c, 0x59, 0x50, 0xf7, 0x1d, 0xe1, 0xad, 0xd1 \
  }

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
#define AT_PROTOCOL_REVISION  1

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gEfiAtProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
EFI_FORWARD_DECLARATION (EFI_AT_PROTOCOL);

///
/// Protocol definitions
///
typedef
EFI_STATUS
(EFIAPI *EFI_AT_AUTHETICATE_CREDENTIAL) (
  IN     EFI_AT_PROTOCOL    *This,
  IN     UINT8              *PassPhrase,
  IN     UINT32             *PassType,
  IN OUT UINT8              *IsAuthenticated
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_COMPUTE_HASH) (
  IN     EFI_AT_PROTOCOL    * This,
  IN     UINT8              *PassPhrase,
  IN OUT UINT8              *Hash
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_NONCE) (
  IN     EFI_AT_PROTOCOL    * This,
  IN OUT UINT8              *Nonce
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_TIMER_INFO) (
  IN     EFI_AT_PROTOCOL    * This,
  IN OUT UINT32             *Interval,
  IN OUT UINT32             *TimeLeft
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_RECOVERY_STRING) (
  IN     EFI_AT_PROTOCOL    * This,
  IN     UINT32             *StringId,
  IN OUT UINT8              *IsvString,
  IN OUT UINT32             *IsvStringLength
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_ISV_ID) (
  IN     EFI_AT_PROTOCOL              * This,
  IN OUT UINT32                       *IsvId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_SET_SUSPEND_STATE) (
  IN     EFI_AT_PROTOCOL              * This,
  IN     UINT32                       TransitionState,
  IN     UINT8                        *Token
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_INIT_WWAN_RECOV) (
  IN     EFI_AT_PROTOCOL             * This
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_WWAN_NIC_STATUS) (
  IN     EFI_AT_PROTOCOL              * This,
  IN OUT UINT8                        *RadioStatus,
  IN OUT UINT8                        *NetworkStatus
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_SEND_ASSERT_STOLEN) (
  IN     EFI_AT_PROTOCOL    * This,
  IN OUT UINT8              *CompletionCode
  );

typedef
EFI_STATUS
(EFIAPI *EFI_AT_GET_STATE_UNSIGNED) (
  IN     EFI_AT_PROTOCOL   * This,
  IN OUT AT_STATE_INFO     * StateUnsigned
  );

///
/// Protocol definition
///
typedef struct _EFI_AT_PROTOCOL {
  UINT8                         Revision;
  EFI_AT_AUTHETICATE_CREDENTIAL AuthenticateCredential;
  EFI_AT_COMPUTE_HASH           ComputeHash;
  EFI_AT_GET_NONCE              GetNonce;
  EFI_AT_GET_TIMER_INFO         GetTimerInfo;
  EFI_AT_GET_RECOVERY_STRING    GetRecoveryString;
  EFI_AT_GET_ISV_ID             GetIsvId;
  EFI_AT_SEND_ASSERT_STOLEN     SendAssertStolen;
  EFI_AT_SET_SUSPEND_STATE      SetSuspendState;
  EFI_AT_INIT_WWAN_RECOV        InitWWAN;
  EFI_AT_GET_WWAN_NIC_STATUS    GetWWANNicStatus;
  EFI_AT_GET_STATE_UNSIGNED     GetStateUnsigned;
} EFI_AT_PROTOCOL;

#endif
