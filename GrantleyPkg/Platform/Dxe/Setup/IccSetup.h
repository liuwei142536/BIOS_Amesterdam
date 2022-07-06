//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++
Copyright (c)  2009 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  IccSetup.h

Abstract:

  Setup hooks for ICC

--*/
#ifndef ICC_SETUP_H
#define ICC_SETUP_H

#define MAX_UI_CLOCKS    3

#define CLOCK2 1
#define CLOCK3 2

#define STATIC_REGISTERS_MASK2          0x00047F01
#define STATIC_REGISTERS_MASK1          0x000D0401
#define STATIC_REGISTERS_MASK0          0x000D0301
#define STATIC_REGISTERS_BUNDLE_TRAD    3
#define STATIC_REGISTERS_BUNDLE_ULT     1

#ifndef ICC_DATA_GUID
#define ICC_DATA_GUID {0x64192dca, 0xd034, 0x49d2, 0xa6, 0xde, 0x65, 0xa8, 0x29, 0xeb, 0x4c, 0x74}
#endif

#define DONT_DISPLAY    0
#define DISPLAY         1

#define OPTION_DISABLED           0
#define OPTION_ENABLED            1

#define OPTION_LOCK_STATIC        0
#define OPTION_LOCK_ALL           1
#define OPTION_UNLOCK_ALL         2

#define OPTION_USE_OEM            7
#define OPTION_OVERRIDE           8

#define ICC_SETUP_DATA_C_NAME  L"IccData"

#ifndef ME_CONFIG_DATA_FILE
//
// Ensure proper structure formats
//
#pragma pack(1)
typedef struct _ICC_SETUP_DATA {
  UINT16 Frequency[MAX_UI_CLOCKS];
  UINT8  Spread[MAX_UI_CLOCKS];
  UINT8  DmiPegRatio[MAX_UI_CLOCKS];
  UINT8  ShowClock[MAX_UI_CLOCKS];
  UINT8  ShowFrequency[MAX_UI_CLOCKS];
  UINT8  ShowSpread[MAX_UI_CLOCKS];
  UINT8  ShowRatio[MAX_UI_CLOCKS];
  UINT8  LockMask;
  UINT8  ShowCustomLock;
  UINT8  AllowAdvancedOptions;
  UINT8  PlatformType;
  //
  UINT8  ShowProfile;
  UINT8  Profile;
} ICC_SETUP_DATA;
#pragma pack()
#endif // ME_CONFIG_DATA_FILE
#endif
