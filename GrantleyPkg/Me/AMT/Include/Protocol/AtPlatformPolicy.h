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
  AtPlatformPolicy.h

@brief
  This driver implements the AT Platform Policy protocol.

**/
#ifndef _DXE_AT_PLATFORM_POLICY_H_
#define _DXE_AT_PLATFORM_POLICY_H_

///
/// AT policy provided by platform for DXE phase
///
#define DXE_PLATFORM_AT_POLICY_GUID \
  { \
    0x3f4ea4d4, 0xb465, 0x4f3f, 0x9c, 0x92, 0xb3, 0x40, 0xab, 0x58, 0x96, 0xb5 \
  }

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
#define DXE_PLATFORM_AT_POLICY_PROTOCOL_REVISION_1 1

extern EFI_GUID gDxePlatformAtPolicyGuid;
#pragma pack(1)
typedef struct {
  UINT8 AtAmBypass;           ///< Byte 0, bit definition for functionality enable/disable
  UINT8 AtRecoveryAttempt;
  UINT8 AtEnterSuspendState;
  UINT8 ByteReserved[29];   ///< Bytes 3-31 Reserved and make AT_CONFIG as 32 bit alignment
} AT_CONFIG;
#pragma pack()
///
/// AMT DXE Platform Policy
///
typedef struct _DXE_AT_POLICY_PROTOCOL {
  UINT8       Revision;
  AT_CONFIG   At;
} DXE_AT_POLICY_PROTOCOL;
#endif
