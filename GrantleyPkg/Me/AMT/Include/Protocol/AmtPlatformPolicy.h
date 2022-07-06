/**
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

@copyright
  Copyright (c)  2006 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  AmtPlatformPolicy.h

@brief
  Interface definition details between AMT and platform drivers during DXE phase.

**/
#ifndef _AMT_PLATFORM_POLICY_H_
#define _AMT_PLATFORM_POLICY_H_

///
/// AMT policy provided by platform for DXE phase
///
#define DXE_PLATFORM_AMT_POLICY_GUID \
  { \
    0x6725e645, 0x4a7f, 0x9969, 0x82, 0xec, 0xd1, 0x87, 0x21, 0xde, 0x5a, 0x57 \
  }

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
#define DXE_PLATFORM_AMT_POLICY_PROTOCOL_REVISION_1 1   ///< Initial Revision

#pragma pack(1)
typedef struct {
  //
  // Byte 0, bit definition for functionality enable/disable
  //
  UINT8   AsfEnabled : 1;               ///< 0: Disabled; 1: Enabled
  UINT8   iAmtEnabled : 1;              ///< 0: Disabled; 1: Enabled
  UINT8   iAmtbxPasswordWrite : 1;      ///< 0: Disabled; 1: Enabled
  UINT8   WatchDog : 1;                 ///< 0: Disabled; 1: Enabled
  UINT8   CiraRequest : 1;              ///< 0: No CIRA request; 1: Trigger CIRA request
  UINT8   ManageabilityMode : 1;        ///< 0: Disabled; 1:AMT
  UINT8   UnConfigureMe : 1;            ///< 0: No; 1: Un-configure ME without password
  UINT8   MebxDebugMsg : 1;             ///< 0: Disabled; 1: Enabled
  //
  // Byte 1, bit definition for functionality enable/disable
  //
  UINT8   ForcMebxSyncUp : 1;           ///< 0: No; 1: Force MEBX execution
  UINT8   UsbrEnabled : 1;              ///< 0: Disabled;  1: Enabled
  UINT8   UsbLockingEnabled : 1;        ///< 0: Disabled;  1: Enabled
  UINT8   HideUnConfigureMeConfirm : 1; ///< 0: Don't hide; 1: Hide Un-configure ME Confirmation Prompt
  UINT8   USBProvision : 1;             ///< 0: Disabled; 1: Enabled
  UINT8   FWProgress : 1;               ///< 0: Disabled; 1: Enabled
  UINT8   iAmtbxHotkeyPressed : 1;      ///< 0: Disabled; 1: Enabled
  UINT8   iAmtbxSelectionScreen : 1;    ///< 0: Disabled; 1: Enabled
  //
  // Byte 2, bit definition for functionality enable/disable
  //
  UINT8   AtEnabled : 1;                ///< 0: Disabled; 1: Enabled
  UINT8   Byte2Reserved : 6;
  UINT8   WatchDogOs : 1;               ///< 0: Disabled; 1: Enabled
  UINT16  WatchDogTimerOs;              ///< Byte 3-4 OS WatchDog Timer
  UINT16  WatchDogTimerBios;            ///< Byte 5-6 BIOS WatchDog Timer
  ///
  /// Byte 7 CIRA Timeout: Client Initiated Remote Access Timeout
  ///                      OEM defined timeout for MPS connection to be established.
  /// 0: 60 seconds by default, 0xFF: Wait until the connection succeeds.
  /// 1~0xFE: Values in seconds
  ///
  UINT8   CiraTimeout;

  ///
  /// Byte 8 CPU Replacement Timeout
  /// 0: 10 seconds; 1: 30 seconds; 2~5: Reserved; 6: No delay; 7: Unlimited delay
  ///
  UINT8   CpuReplacementTimeout;
  //
  // Byte 9-10 OemResolutionSettings
  //
  UINT16  MebxNonUiTextMode : 4;        ///< 0: Auto; 1: 80x25; 2: 100x31
  UINT16  MebxUiTextMode : 4;           ///< 0: Auto; 1: 80x25; 2: 100x31
  UINT16  MebxGraphicsMode : 4;         ///< 0: Auto; 1: 640x480; 2: 800x600; 3: 1024x768
  UINT16  OemResolutionSettingsRsvd : 4;
  //
  // Byte 11-14 Pointer to a list which contain on-board devices bus/device/fun number
  //
  UINT32  PciDeviceFilterOutTable;
  //
  // Byte 15-23 Reserved and make AMT_CONFIG as 32 bit alignment
  //
  UINT8   ByteReserved[9];
} AMT_CONFIG;

#pragma pack()
///
/// AMT DXE Platform Policy
///
typedef struct _DXE_AMT_POLICY_PROTOCOL {
  UINT8                 Revision;
  AMT_CONFIG            AmtConfig;
} DXE_AMT_POLICY_PROTOCOL;

extern EFI_GUID gDxePlatformAmtPolicyGuid;

#endif
