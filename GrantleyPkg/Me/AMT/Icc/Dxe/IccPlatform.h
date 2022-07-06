//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++
Copyright (c)  2009-2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  IccPlatform.h

Abstract:

  Platform-specific ICC code

--*/
#ifndef ICC_PLATFORM_H
#define ICC_PLATFORM_H
#include "CoreBiosMsg.h"
#include <MeLib.h>
#include "Platform/Dxe/Setup/IccSetup.h"
#include "PchRegs/PchRegsPcie.h"

extern EFI_GUID gIccDataGuid;
#ifndef ICC_DATA_GUID
#define ICC_DATA_GUID {0x64192dca, 0xd034, 0x49d2, 0xa6, 0xde, 0x65, 0xa8, 0x29, 0xeb, 0x4c, 0x74}
#endif

#define PROFILE_SELECTED_BY_BIOS  0
#define PROFILE_SELECTED_BY_ME    1

#define OPTION_DISABLED           0
#define OPTION_ENABLED            1

#define OPTION_USE_OEM            7
#define OPTION_OVERRIDE           8
//
// OCLKEN (ICC clock enables) register bit definitions
//
#define CLOCK_Flex0       BIT0
#define CLOCK_Flex1       BIT1
#define CLOCK_Flex2       BIT2
#define CLOCK_Flex3       BIT3
#define CLOCK_PCI_Clock0  BIT7
#define CLOCK_PCI_Clock1  BIT8
#define CLOCK_PCI_Clock2  BIT9
#define CLOCK_PCI_Clock3  BIT10
#define CLOCK_PCI_Clock4  BIT11
#define CLOCK_SRC0        BIT16
#define CLOCK_SRC1        BIT17
#define CLOCK_SRC2        BIT18
#define CLOCK_SRC3        BIT19
#define CLOCK_SRC4        BIT20
#define CLOCK_SRC5        BIT21
#define CLOCK_SRC6        BIT22
#define CLOCK_SRC7        BIT23
#define CLOCK_CSI_SRC8    BIT24
#define CLOCK_CSI_DP      BIT25
#define CLOCK_PEG_A       BIT26
#define CLOCK_PEG_B       BIT27
#define CLOCK_DMI         BIT28

#pragma pack(1)
typedef struct {
  UINT32  Clock;
  UINT8   DeviceNumber;
} PLATFORM_PCI_SLOTS;

typedef struct {
  UINT32  Clock;
  UINT8   RootPortNumber;
} PLATFORM_PCIE_SLOTS;

typedef struct _ICC_MPB_DATA {
  UINT8 Profile;
  UINT8 NumProfiles;
  UINT8 ProfileSelectionAllowed;
  UINT8 LockMask;
} ICC_MBP_DATA;
#pragma pack()

#define ICC_MBP_DATA_NAME L"IccMbpData"

PLATFORM_PCIE_SLOTS PciExpressMobile[] = {
    { CLOCK_SRC1, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1},
    { CLOCK_SRC2, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2},
    { CLOCK_SRC3, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3},
    { CLOCK_SRC4, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4},
    { CLOCK_SRC5, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5},
  };

PLATFORM_PCIE_SLOTS PciExpressUltErb[] = {
    { CLOCK_SRC0, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1},
    { CLOCK_SRC1, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2},
    { CLOCK_SRC2, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3},
    { CLOCK_SRC3, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4},
    { CLOCK_SRC4, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5},
    { CLOCK_SRC5, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6},
  };

PLATFORM_PCIE_SLOTS PciExpressUltCrb[] = {
    { CLOCK_SRC0, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1},
    { CLOCK_SRC1, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2},
    { CLOCK_SRC2, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3},
    { CLOCK_SRC3, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4},
    { CLOCK_SRC5, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6},
  };

PLATFORM_PCIE_SLOTS PciExpressFlatheadCreekEV[] = {
    { CLOCK_SRC6, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8},
    { CLOCK_SRC5, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5},
    { CLOCK_SRC7, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6},
    { CLOCK_SRC0, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7},
  };


EFI_STATUS
DetectUsedClocks (
  OUT UINT32 *UsedClocks,
  OUT UINT32 *AllClocks
  );

EFI_STATUS
ReadMainSetupData (
   IN EFI_HANDLE ImageHandle
  );

VOID
SendLateIccMessages (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

#endif // ICC_PLATFORM_H