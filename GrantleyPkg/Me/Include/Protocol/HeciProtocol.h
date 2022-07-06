/*++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
--*/
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  Heci.h

Abstract:

  EFI HECI Protocol

--*/
#ifndef _HECI_PROTOCOL_H_
#define _HECI_PROTOCOL_H_

#define HECI_PROTOCOL_GUID \
  { \
    0xcfb33810, 0x6e87, 0x4284, {0xb2, 0x3, 0xa6, 0x6a, 0xbe, 0x7, 0xf6, 0xe8} \
  }

#define HECI_REINIT_TIMEOUT      500000    // .5 secs
#define HECI_INIT_TIMEOUT        25000000  // 25sec timeout in microseconds
#define HECI_SINGLE_READ_TIMEOUT 1000000   // 1 sec timeout in microseconds
                                           //       for single HECI buffer read
#define HECI_READ_TIMEOUT        12500000  // 12.5sec timeout in microseconds
#define HECI_SEND_TIMEOUT        12500000  // 12.5sec timeout in microseconds

#define HECI_MAX_RETRY           3
#define HECI_MSG_DELAY           2000000   // show warning msg and stay for 2 seconds.


typedef struct _EFI_HECI_PROTOCOL EFI_HECI_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_SENDWACK) (
  IN OUT  UINT32           *Message,
  IN OUT  UINT32           Length,
  IN OUT  UINT32           *RecLength,
  IN      UINT8            HostAddress,
  IN      UINT8            MEAddress
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_READ_MESSAGE) (
  IN      UINT32           Blocking,
  IN      UINT32           *MessageBody,
  IN OUT  UINT32           *Length
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_READ_FLUSH_MESSAGE) (
  IN      UINT32           Blocking
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_SEND_MESSAGE) (
  IN      UINT32           *Message,
  IN      UINT32           Length,
  IN      UINT8            HostAddress,
  IN      UINT8            MEAddress
  );
typedef
EFI_STATUS
(EFIAPI *EFI_HECI_RESET) (VOID);

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_INIT) (VOID);

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_REINIT) (VOID);

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_RESET_WAIT) (
  IN        UINT32           Delay
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_GET_ME_STATUS) (
  IN UINT32                       *Status
  );

typedef
EFI_STATUS
(EFIAPI *EFI_HECI_GET_ME_MODE) (
  IN UINT32                       *Mode
  );

typedef struct _EFI_HECI_PROTOCOL {
  EFI_HECI_SENDWACK           SendwACK;
  EFI_HECI_READ_MESSAGE       ReadMsg;
  EFI_HECI_SEND_MESSAGE       SendMsg;
  EFI_HECI_RESET              ResetHeci;
  EFI_HECI_INIT               InitHeci;
  EFI_HECI_RESET_WAIT         MeResetWait;
  EFI_HECI_REINIT             ReInitHeci;
  EFI_HECI_GET_ME_STATUS      GetMeStatus;
  EFI_HECI_GET_ME_MODE        GetMeMode;
  EFI_HECI_READ_FLUSH_MESSAGE ReadAndFlush;
} EFI_HECI_PROTOCOL;

extern EFI_GUID gEfiHeciProtocolGuid;

#endif // _HECI_PROTOCOL_H_

