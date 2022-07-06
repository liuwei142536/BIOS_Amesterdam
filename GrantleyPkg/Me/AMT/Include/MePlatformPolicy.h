/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
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

@file
  MePlatformPolicy.h

@brief
  Interface definition details between ME and platform drivers during DXE phase.

**/
#ifndef _ME_PLATFORM_POLICY_H_
#define _ME_PLATFORM_POLICY_H_

///
/// ME policy provided by platform for DXE phase
///
#define DXE_PLATFORM_ME_POLICY_GUID \
  { \
    0x69bf9e8a, 0x4ad6, 0x9a28, 0x87, 0xf3, 0x09, 0xa0, 0x71, 0x29, 0x2a, 0x00 \
  }

#define DXE_PLATFORM_ME_POLICY_PROTOCOL_REVISION_1  1   ///< Initial Revision

#pragma pack(1)

typedef struct {
  ///
  /// Byte 0, bit definition for functionality enable/disable
  ///
  UINT8     MeFwDownGrade : 1;        ///< 0: Disabled; 1: Enabled
  UINT8     MeLocalFwUpdEnabled : 1;  ///< 0: Disabled; 1: Enabled
  UINT8     MbpSecurity : 1;          ///< 0: Disabled; 1: Enabled
  UINT8     EndOfPostEnabled : 1;     ///< 0: Disabled; 1: Enabled
  UINT8     EndOfPostDone : 1;        ///< 0: Not yet;  1: Done
  UINT8     MdesCapability : 1;       ///< 0: Disabled; 1: Enabled
  UINT8     SvtForPchCap: 1;          ///< 0: Disabled; 1: Enabled
  UINT8     MdesForBiosState : 1;     ///< 0: Disabled; 1: Enabled
  UINT8     ByteReserved[15];         ///< Byte 1-15 Reserved for other bit definitions in future
} ME_CONFIG;

///
/// ME ERROR Message ID
///
typedef enum {
  MSG_EOP_ERROR             = 0,
  MSG_ME_FW_UPDATE_FAILED,
  MSG_ASF_BOOT_DISK_MISSING,
  MSG_KVM_TIMES_UP,
  MSG_KVM_REJECTED,
  MSG_HMRFPO_LOCK_FAILURE,
  MSG_HMRFPO_UNLOCK_FAILURE,
  MSG_ME_FW_UPDATE_WAIT,
  MSG_ILLEGAL_CPU_PLUGGED_IN,
  MSG_KVM_WAIT,
  MAX_ERROR_ENUM
} ME_ERROR_MSG_ID;

typedef
VOID
(EFIAPI *ME_REPORT_ERROR) (
  ME_ERROR_MSG_ID
  );

typedef struct {
  UINT8 FviSmbiosType;
} ME_MISC_CONFIG;

#pragma pack()
///
/// ME DXE Platform Policy
///
typedef struct _DXE_ME_POLICY_PROTOCOL {
  UINT8           Revision;
  ME_CONFIG       MeConfig;
  ME_REPORT_ERROR MeReportError;
  ME_MISC_CONFIG  MeMiscConfig;
  // Variables required to Manage changes in Setup
  BOOLEAN         AnythingChanged;
#if defined(AMT_SUPPORT) && AMT_SUPPORT
  UINT8           PreviousMeStateControl;
#endif // AMT_SUPPORT
  UINT8           PreviousLocalFwUpdEnabled;
} DXE_ME_POLICY_PROTOCOL;

extern EFI_GUID gDxePlatformMePolicyGuid;

#endif
