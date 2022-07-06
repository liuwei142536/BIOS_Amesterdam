/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c)  2005 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  MeBiosExtensionSetup.h

@brief
  Me Bios Extension 16 Setup Options Guid definitions

**/
#ifndef _EFI_ME_BIOS_EXTENSION_SETUP_GUID_H_
#define _EFI_ME_BIOS_EXTENSION_SETUP_GUID_H_

#include "Uefi.h"

#define EFI_ME_BIOS_EXTENSION_SETUP_GUID \
  { \
    0x1bad711c, 0xd451, 0x4241, 0xb1, 0xf3, 0x85, 0x37, 0x81, 0x2e, 0xc, 0x70 \
  }

#define EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME         (L"MeBiosExtensionSetup")
#define EFI_ME_BIOS_EXTENSION_SETUP_FORMAT_VARIABLE_NAME  ("MeBiosExtensionSetup")

extern CHAR16   gEfiMeBiosExtensionSetupName[];

extern EFI_GUID gEfiMeBiosExtensionSetupGuid;

#pragma pack(1)

typedef struct {
  UINT16  InterfaceVersion;
  UINT16  Reserved;
  UINT32  Flags;
  UINT8   PlatformMngSel;
  UINT8   Reserved4;
  UINT8   AmtSolIder;
  UINT8   Reserved1[2];
  UINT8   RemoteAssistanceTriggerAvailablilty;
  UINT8   KvmEnable;
  UINT8   MebxDefaultSolIder;
  UINT8   Reserved2[10];
} ME_BIOS_EXTENSION_SETUP;

#define MNT_OFF     0x00
#define MNT_ON      0x01
#define LAN_ENABLE  0x01
#define SOL_ENABLE  0x01
#define IDER_ENABLE 0x02
#define KVM_ENABLE  0x01

#pragma pack()

#endif
