//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
Copyright (c) 1996 - 2012, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  AcpiPlatformHooksLib.h
  
Abstract:

--*/

#ifndef _ACPI_PLATFORM_HOOKS_LIB_H_
#define _ACPI_PLATFORM_HOOKS_LIB_H_

//
// Statements that include other header files
//
#include <PiDxe.h>


EFI_ACPI_TABLE_VERSION
AcpiPlatformHooksGetAcpiTableVersion (
  VOID
  );

/*++

Routine Description:

  Returns the ACPI table version that the platform wants.

Arguments:

  None

Returns:

  EFI_ACPI_TABLE_VERSION_NONE if ACPI is to be disabled
  EFI_ACPI_TABLE_VERSION_1_0B if 1.0b
  EFI_ACPI_TABLE_VERSION_2_00 if 2.00
--*/
EFI_STATUS
AcpiPlatformHooksGetOemFields (
  OUT UINT8   *OemId,
  OUT UINT64  *OemTableId,
  OUT UINT32  *OemRevision
  );

/*++

Routine Description:

  Returns the OEMID, OEM Table ID, OEM Revision.

Arguments:

  None

Returns:
  OemId      - OEM ID string for ACPI tables, maximum 6 ASCII characters.
               This is an OEM-supplied string that identifies the OEM.
  OemTableId - An OEM-supplied string that the OEM uses to identify 
               the particular data table. This field is particularly useful 
               when defining a definition block to distinguish definition block 
               functions. The OEM assigns each dissimilar table a new OEM Table ID.
  OemRevision - An OEM-supplied revision number for ACPI tables. 
                Larger numbers are assumed to be newer revisions.

--*/
EFI_STATUS
AcpiPlatformHooksIsActiveTable (
  IN OUT EFI_ACPI_COMMON_HEADER     *Table
  );

/*++

Routine Description:

  Called for every ACPI table found in the BIOS flash.
  Returns whether a table is active or not. Inactive tables
  are not published in the ACPI table list. This hook can be
  used to implement optional SSDT tables or enabling/disabling
  specific functionality (e.g. SPCR table) based on a setup
  switch or platform preference. In case of optional SSDT tables,
  the platform flash will include all the SSDT tables but will
  return EFI_SUCCESS only for those tables that need to be
  published.
  This hook can also be used to update the table data. The header
  is updated by the common code. For example, if a platform wants
  to use an SSDT table to export some platform settings to the
  ACPI code, it needs to update the data inside that SSDT based
  on platform preferences in this hook.

Arguments:

  None

Returns:

  Status  - EFI_SUCCESS if the table is active
  Status  - EFI_UNSUPPORTED if the table is not active
*/
#endif
