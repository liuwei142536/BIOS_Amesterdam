//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 1996 - 2014, Intel Corporation.

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

  EdkMemoryQpiInit.h

Abstract:

  Memory Initialization Module.

--*/

#ifndef _EDK_MEMORYQPI_INIT_H_
#define _EDK_MEMORYQPI_INIT_H_

#define size_t UINT32
#ifndef SUCCESS
#define SUCCESS 0
#endif

#include "SysFunc.h"
#include "ProcMemInit.h"
#include "Platform.h"
#include <PiPei.h>
#include <Setup/IioUniversalData.h>
#include <Setup/IioCsiConfigData.h>
// Aptio V Server Override - Start : Fix for Nvram Corruption
#include <Setup/IioUdsDefaults.h>
//#include <Setup/IioDxeUdsDefaults.h>
// Aptio V Server Override - End : Fix for Nvram Corruption
#include <PPI/ReadOnlyVariable2.h>
#include <Ppi/TxtSclean.h>
#include <Ppi/CpuIo.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/DebugMask.h>
#include <Guid/PlatformTxt.h>
#include <Guid/SetupBreakpointGuid.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PfatInitLib/PfatInit.h>
#include <Ppi/BaseMemoryTest.h>
#include <Ppi/PlatformMemorySize.h>
#include <Ppi/PlatformMemoryRange.h>
#include <Guid/MemoryMapData.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/AcpiVariableCompatibility.h>
#include <Guid\AcpiS3Context.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/MpstNodeData.h>
#include <Library/PcdLib.h>
#include <Guid/LastBootErrorLogHob.h>

//
// ------------------------ TSEG Base 
// ...............
// ------------------------ RESERVED_CPU_S3_SAVE_OFFSET
// CPU S3 data
// ------------------------ RESERVED_ACPI_S3_RANGE_OFFSET
// S3 Memory base structure
// ------------------------ TSEG + 1 page

#define RESERVED_CPU_S3_SAVE_OFFSET (RESERVED_ACPI_S3_RANGE_OFFSET - sizeof (SMM_S3_RESUME_STATE))

#define  RC_HEAP_SIZE   (32*1024)

extern EFI_GUID gEfiMemoryConfigDataHobGuid;

//
// SMRAM range definitions
//
#define MC_ABSEG_HSEG_PHYSICAL_START        0x000A0000
#define MC_ABSEG_HSEG_LENGTH                0x00020000
#define MC_ABSEG_CPU_START                  0x000A0000
#define MC_HSEG_CPU_START                   0xFEDA0000

//
// Maximum number of memory ranges supported
//
#define MAX_RANGES        (4 + 8)

#define MEM_EQU_4GB                     0x100000000
#define  RC_HEAP_SIZE                   (32*1024)

//
// Memory range types
//
typedef enum {
  DualChannelDdrMainMemory,
  DualChannelDdrSmramCacheable,
  DualChannelDdrSmramNonCacheable,
  DualChannelDdrGraphicsMemoryCacheable,
  DualChannelDdrGraphicsMemoryNonCacheable,
  DualChannelDdrReservedMemory,
  DualChannelDdrMaxMemoryRangeType
} PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE;

#pragma pack(1)

//
// Memory map range information
//
typedef struct {
  UINT8                                         RowNumber;
  EFI_PHYSICAL_ADDRESS                          PhysicalAddress;
  EFI_PHYSICAL_ADDRESS                          CpuAddress;
  EFI_PHYSICAL_ADDRESS                          RangeLength;
  PEI_DUAL_CHANNEL_DDR_MEMORY_RANGE_TYPE        Type;
} PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE;

//
// HOB to save Memory S3_RestoreData
//
typedef struct {
  EFI_HOB_GUID_TYPE         EfiHobGuidType;
  //
  //  MRC_PARAMS_SAVE_RESTORE   S3_RestoreData;
  //
} HOB_SAVE_MEM_RESTORE_DATA;

typedef struct {
   UINT32     RcrbBar;
   UINT32     VtdBar;
} IIO_PLATFORM_FIXED_MAP;

#pragma pack()

VOID
RetrieveRequiredMemorySize (
  IN PSYSHOST host,
  OUT UINTN *Size
  );

VOID
RetrieveTxtStolenMemorySize (
  IN PSYSHOST host,
  IN OUT UINT64 *Size
  ) ;

VOID
SaveTxtStolenMemoryBase (
  IN PSYSHOST host,
  IN EFI_PHYSICAL_ADDRESS Base
  ) ;

EFI_STATUS
GetMemoryMap (
  IN OUT PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE  *MemoryMap,
  IN OUT UINT8                                  *NumRanges,
  IN struct sysHost                             *host
  );

EFI_STATUS
InstallEfiMemory (
    IN struct sysHost             *host
  );

EFI_STATUS
InstallS3Memory (
    IN struct sysHost             *host
  );

#ifdef BDAT_SUPPORT
EFI_STATUS
SaveBdatDataToHob (
  struct sysHost         *host
  );
#endif

EFI_STATUS
MrcSaveS3RestoreDataToHob (
  IN struct sysHost      *host
  );

VOID
PublishHobData (
  struct sysHost             *host
  );


#endif
