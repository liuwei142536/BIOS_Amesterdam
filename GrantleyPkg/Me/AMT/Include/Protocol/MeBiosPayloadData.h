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
  MeBiosPayloadData.h

@brief
  Interface definition details for MBP during DXE phase.

**/
#ifndef _MBP_DATA_PROTOCOL_H_
#define _MBP_DATA_PROTOCOL_H_
#include "Library/CoreBiosMsg.h"

///
/// MBP Protocol for DXE phase
///
#define ME_BIOS_PAYLOAD_DATA_PROTOCOL_GUID \
  { \
    0x71a19494, 0x2ab6, 0x4e96, 0x85, 0x81, 0xcf, 0x34, 0x25, 0x42, 0x73, 0xfe \
  }

///
/// Revision 1:  Original version
///
#define DXE_MBP_DATA_PROTOCOL_REVISION_1  1
#define DXE_MBP_DATA_PROTOCOL_REVISION_2  2
extern EFI_GUID gMeBiosPayloadDataProtocolGuid;

#pragma pack(push, 1)
typedef struct _MBP_FW_VERSION_NAME {
  UINT32  MajorVersion : 16;
  UINT32  MinorVersion : 16;
  UINT32  HotfixVersion : 16;
  UINT32  BuildVersion : 16;
} MBP_FW_VERSION_NAME;

typedef struct _MBP_ICC_PROFILE {
  UINT8               NumIccProfiles;
  UINT8               IccProfileSoftStrap;
  UINT8               IccProfileIndex;
  UINT8               Reserved;
  ICC_LOCK_REGS_INFO  IccLockRegInfo;
} MBP_ICC_PROFILE;

typedef struct _MBP_FW_CAPS_SKU {
  MEFWCAPS_SKU  FwCapabilities;
  BOOLEAN       Available;
} MBP_FW_CAPS_SKU;

typedef struct _MBP_ROM_BIST_DATA {
  UINT16  DeviceId;
  UINT16  FuseTestFlags;
  UINT32  UMCHID[4];
} MBP_ROM_BIST_DATA;

typedef struct _MBP_PLATFORM_KEY {
  UINT32  Key[8];
} MBP_PLATFORM_KEY;

typedef struct _MBP_PLAT_TYPE {
  PLATFORM_TYPE_RULE_DATA RuleData;
  BOOLEAN                 Available;
} MBP_PLAT_TYPE;


typedef union _HWA_DATA {
  UINT32  Raw;
  struct {
    UINT32  MediaTablePush : 1;
    UINT32  Reserved : 31;
  } Fields;
} HWA_DATA;

typedef struct _MBP_HWA_REQ {
  HWA_DATA Data;
  BOOLEAN Available;
} MBP_HWA_REQ;

typedef struct {
  MBP_FW_VERSION_NAME FwVersionName;
  MBP_FW_CAPS_SKU     FwCapsSku;
  MBP_ROM_BIST_DATA   RomBistData;
  MBP_PLATFORM_KEY    PlatformKey;
  MBP_PLAT_TYPE       FwPlatType;
  MBP_ICC_PROFILE     IccProfile;
  AT_STATE_INFO       AtState;
  UINT32              MFSIntegrity;
  MBP_HWA_REQ         HwaRequest;
} ME_BIOS_PAYLOAD;
#pragma pack(pop)
///
/// MBP DXE Protocol
///
typedef struct _DXE_MBP_DATA_PROTOCOL {
  EFI_HANDLE      Handle;
  UINT8           Revision;
  ME_BIOS_PAYLOAD MeBiosPayload;
} DXE_MBP_DATA_PROTOCOL;

#endif
