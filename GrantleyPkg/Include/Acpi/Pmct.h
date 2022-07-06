//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++ 

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  Pmct.h

Abstract:

  This file describes the contents of the PMCT ACPI table.  

--*/

#ifndef _PMCT_H_
#define _PMCT_H_

//
// Statements that include other files
//
#include <IndustryStandard\Acpi.h>

//
// PMCT ACPI Definitions
//
#define PMCT_CAPABILITY_COUNT                     2             // Memory and CPU.
#define EFI_PMCT_ACPI_TABLE_SIGNATURE             SIGNATURE_32('P','M','C','T')
#define EFI_PMCT_ACPI_DESCRIPTION_TABLE_REVISION  0x01
#define EFI_PMCT_ACPI_OEM_REVISION                0x00000000

#define EFI_PMCT_MIGRATION_TYPE_MEMORY            0
#define EFI_PMCT_MEMORY_VALIDATION_BITS           0x0002        // RESOURCE_ID_VALID_BIT = 0; POLLING_INTERVAL_VALID_BIT = 1.
#define EFI_PMCT_MEMORY_FLAGS                     0x01          // MIGRATION_HARDWARE_NONBLOCKING.
#define EFI_PMCT_MEMORY_POLLING_INTERVAL          2000          // Set Polling Interval to 2 seconds.

#define EFI_PMCT_MIGRATION_TYPE_CPU               1
#define EFI_PMCT_CPU_VALIDATION_BITS              0x0000        // RESOURCE_ID_VALID_BIT = 0.
#define EFI_PMCT_CPU_FLAGS                        0x00          // OS needs to handle migrating CPU context.

#define EFI_PMCT_MIGRATION_TYPE_IO                2

typedef struct {
  UINT16                              Type;
  UINT16                              ValidationBits;
  UINT16                              Flags;
  UINT32                              PollingInterval;
  UINT16                              Reserved;
  UINT8                               Uuid[16];
} EFI_PMCT_MEMORY_MIGRATION_CAPABILITY;

typedef struct {
  UINT16                              Type;
  UINT16                              ValidationBits;
  UINT16                              Flags;
  UINT8                               Reserved[6];
  UINT8                               Uuid[16];
} EFI_PMCT_CPU_MIGRATION_CAPABILITY;

//
// IO Migration not used on Boxboro-EX/Beckton platforms.
//
typedef struct {
  UINT16                              Type;
  UINT16                              ValidationBits;
  UINT16                              Flags;
  UINT8                               Reserved[6];
  UINT8                               Uuid[16];
} EFI_PMCT_IO_MIGRATION_CAPABILITY;

//
// PMCT ACPI structure
//
typedef struct {

  EFI_ACPI_DESCRIPTION_HEADER           Header;
  // PMCT Speicific Entries
  UINT32                                MigrationCapabilityCount;
  EFI_PMCT_MEMORY_MIGRATION_CAPABILITY  MemoryMigrationCapability;
  EFI_PMCT_CPU_MIGRATION_CAPABILITY     CpuMigrationCapability;
} EFI_PMCT_ACPI_DESCRIPTION_TABLE;

#endif
