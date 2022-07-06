//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/

#ifndef _AMT_WRAPPER_DXE_H_
#define _AMT_WRAPPER_DXE_H_

#include "Library/BdsLib.h"

#include "AmtLib.h"
#include "MeAccess.h"
#include "Guid/SetupVariable.h"

#define R_PCI_SVID                      0x2C

#define     DISPLAY_SCREEN              0x01				// for LegacySredr CSM16
#define     SUPPORT8025                 0x02				// for LegacySredr CSM16

// define for SREDIR_DISPLAY_MODE
#define 	DISPLAY_BY_SETUP			0x00
#define 	DISPLAY_BY_80x24			0x01
#define 	DISPLAY_BY_80x25			0x02

#define STOPB   0x4                 //      Bit2: Number of Stop Bits
#define PAREN   0x8                 //      Bit3: Parity Enable
#define EVENPAR 0x10                //      Bit4: Even Parity Select
#define STICPAR 0x20                //      Bit5: Sticky Parity
#define SERIALDB 0x3                //      Bit0-1: Number of Serial
                                    //
#define FW_IMAGE_TYPE_1_5MB               3
#define FW_IMAGE_TYPE_5MB                 4

#define EFI_WIN_NT_THUNK_PROTOCOL_GUID \
  { 0x58c518b1, 0x76f3, 0x11d4, 0xbc, 0xea, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 }

#define EFI_WIN_NT_BUS_DRIVER_IO_PROTOCOL_GUID \
  { 0x96eb4ad6, 0xa32a, 0x11d4, 0xbc, 0xfd, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 }

#define EFI_WIN_NT_SERIAL_PORT_GUID \
  { 0xc95a93d, 0xa006, 0x11d4, 0xbc, 0xfa, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 }

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           SerialOverLAN;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PCI_SERIAL_OVER_LAN_DEVICE_PATH;

#define gPciRootBridge \
  { \
	ACPI_DEVICE_PATH, \
	ACPI_DP, \
    (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), \
    (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8), \
    EISA_PNP_ID (0x0A03), 0 \
  }

#define gEndEntire \
  { \
    END_DEVICE_PATH_TYPE,\
    END_ENTIRE_DEVICE_PATH_SUBTYPE,\
    END_DEVICE_PATH_LENGTH,\
    0\
  }

EFI_STATUS
AmtWrapperInit(
    IN EFI_HANDLE ImageHandle,
    IN OUT EFI_SYSTEM_TABLE *SystemTable
);

BOOLEAN
AmtWrapperGet(
    UINTN Index,
    VOID *pVar
);

BOOLEAN
AmtWrapperSet(
    UINTN Index,
    VOID *pVar
);

BOOLEAN
ActiveManagementPauseBoot (
    IN VOID
);

BOOLEAN
ActiveManagementEnterSetup (
    IN VOID
);

BOOLEAN
ActiveManagementEnableSol (
    IN VOID
);

BOOLEAN
ActiveManagementEnableIdeR (
    IN VOID
);

EFI_STATUS
BdsAsfInitialization (
    IN VOID
);

EFI_STATUS
BdsBootViaAsf (
    IN VOID
);

BOOLEAN
AmtWrapperBootOptionExist (
    IN VOID
);

VOID
InitAmtWrapperLib(
	IN VOID
);

BOOLEAN
AmtWrapperEnableKvm(
	IN VOID
);


BOOLEAN
AmtWrapperEnableSol(
	IN VOID
);

BOOLEAN
AmtWrapperEnableIdeR(
	IN VOID
);

BOOLEAN
AmtWrapperPauseBoot(
	IN VOID
);

BOOLEAN
AmtWrapperEnterSetup(
	IN VOID
);

EFI_STATUS
AmtWrapperBdsBootViaAsf(
	IN VOID
);

VOID
AmtWrapperReadyToBoot(
	EFI_EVENT           Event,
    VOID                *ParentImageHandle
);

UINTN IsSOL(
	IN EFI_HANDLE Controller
);

UINTN IsIDER(
  IN EFI_HANDLE Controller
);

EFI_STATUS
GetSetupPrompt (
  IN OUT  UINTN     *PromptIndex,
  OUT     CHAR16    **String
);

EFI_STATUS
DetermineSetupHotKey(
  IN EFI_KEY_DATA       *KeyData
);

EFI_STATUS
GetSupportedHotkeys (
  OUT  UINTN             *KeyCnt,
  OUT  EFI_KEY_DATA      **KeyData
  );

#endif
