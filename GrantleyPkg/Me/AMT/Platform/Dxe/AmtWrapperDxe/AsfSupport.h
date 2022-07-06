/*++
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
--*/

/*++

Copyright (c) 2005-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AsfSupport.h

Abstract:

  ASF BDS Support include file

--*/
#ifndef _ASF_SUPPORT_H_
#define _ASF_SUPPORT_H_

#include "Library/BdsLib.h"
#include "Protocol/LegacyBios.h"
#include "IndustryStandard/Pci22.h"
#include "Amt.h"
#include "AmtLib.h"
#include "MeAccess.h"


#define IDER_PRIMARY_SECONDARY_MASK   0x02
#define IDER_MASTER_SLAVE_MASK        0x01
#define IDER_PRIMARY_SECONDARY_SHIFT  1


#define IS_IDER(BUS, DEVICE, FUNCTION) \
  ((BUS) == ME_BUS && (DEVICE) == ME_DEVICE_NUMBER && (FUNCTION) == IDER_FUNCTION_NUMBER)
#define IS_PXE(TYPE, CLASS) \
  ((TYPE) == BBS_TYPE_BEV && (CLASS) == PCI_CLASS_NETWORK)
#define IS_CDROM(TYPE, CLASS) \
  ((TYPE) == BBS_TYPE_BEV && (CLASS) == PCI_CLASS_MASS_STORAGE)


typedef union {
  UINT32  Data32;
  UINT16  Data16[2];
} DATA32_UNION;

EFI_STATUS
BdsAsfInitialization (
  IN  VOID
  )
/*++

Routine Description:

  Retrieve the ASF boot options previously recorded by the ASF driver.

Arguments:

  None.

Returns:

  Initialize Boot Options global variable and AMT protocol

--*/
;

EFI_STATUS
BdsBootViaAsf (
  IN  VOID
  )
/*++

Routine Description:

  Process ASF boot options and if available, attempt the boot

Arguments:

  None.

Returns:

  EFI_SUCCESS - The command completed successfully
  Other       - Error!!

--*/
;

VOID
PrintBbsTable (
  IN BBS_TABLE                      *LocalBbsTable
  )
/*++

Routine Description:

  Dump all devices of BBS.

Arguments:

  LocalBbsTable - BBS table entry.

Returns:

  None.

--*/
;

EFI_STATUS
BdsLibDoLegacyBoot (
  IN  BDS_COMMON_OPTION           *Option
  )
/*++

Routine Description:

  Boot the legacy system with the boot option

Arguments:

  Option           - The legacy boot option which have BBS device path

Returns:

  EFI_UNSUPPORTED  - There is no legacybios protocol, do not support
                     legacy boot.

  EFI_STATUS       - Return the status of LegacyBios->LegacyBoot ().

--*/
;

BOOLEAN
BdsCheckIderMedia (
  IN  VOID
  )
/*++

Routine Description:
  This will return if Media in IDE-R is present.

Arguments:
  None.

Returns:
  True    Media is present.
  False   Media is not present.

--*/
;

EFI_DEVICE_PATH_PROTOCOL  *
BdsCreateShellDevicePath (
  IN VOID
  )
/*++

Routine Description:

  This function will create a SHELL BootOption to boot.

Arguments:

  None.

Returns:

  Shell Device path for booting.

--*/
;

BDS_COMMON_OPTION         *
BdsCreateBootOption (
  IN  EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN  CHAR16                         *Description
  )
/*++

Routine Description:

  This function will create a BootOption from the give device path and
  description string.

Arguments:

  DevicePath       - The device path which the option represent
  Description      - The description of the boot option

Returns:

  BDS_COMMON_OPTION - A BDS_COMMON_OPTION pointer

--*/
;

#endif
