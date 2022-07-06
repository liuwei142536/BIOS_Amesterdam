/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c) 2011 - 2013 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  PfatInit.h

@brief
  Describes the functions visible to the rest of the PfatInit.
**/
#ifndef _PFAT_INIT_H_
#define _PFAT_INIT_H_

#include "PfatDefinitions.h"
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Iio\IioRegs.h>
#include <Library/PchPlatformLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryQpiInitLib.h>
#include <Library\DebugLib.h>
#include <Ppi\Reset.h>
#include <PchAccess.h>
#include <Cpu\CpuRegs.h>
#include "SysFunc.h"

#define QPILP2_REG    0x54
#define QPI1_CFG_DEV  0x9

///
/// PFAT Configuration for all processor security features configuration structures
/// are in this field. Platform code can pass relevant configuration data thru this field.
///
typedef struct {
  ///
  /// PFAT update package header that will be packaged along with PFAT script and update data.
  ///
  PUP_HEADER    PupHeader;
  ///
  /// PFAT Platform Data Table contains all the platform data that will be parsed by PFAT module.
  ///
  PPDT          Ppdt;
  UINT64        PpdtHash[4];       ///< Hash of the PPDT that will be programmed to PLAT_FRMW_PROT_HASH_0/1/2/3 MSR.
  UINT8         NumSpiComponents;  ///< Number of SPI flash components on platform.
  UINT8         ComponentSize[MAX_SPI_COMPONENTS]; ///< Size of each flash component on platform.
  UINT8         PfatMemSize;       ///< PFAT memory size.
  UINT8         EcCmdDiscovery;    ///< EC Command discovery.
  UINT8         EcCmdProvisionEav; ///< EC Command Provision Eav.
  UINT8         EcCmdLock;         ///< EC Command Lock.
  PFAT_LOG      PfatLog;           ///< PFAT log.
} PFAT_CONFIG;

///
/// Function Prototypes
///
/**
  Execute Early-Post initialization of PFAT specific MSRs

  @param[in] PeiServices                  - Indirect reference to the PEI Services Table.
  @param[in] TotalSystemConfigurationPtr  - Pointer to Setup Configuration structure
  @param[in] host                         - SysHost structure with configuration values
**/
VOID
PfatInit (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN SYSTEM_CONFIGURATION       *TotalSystemConfigurationPtr,
  IN struct sysHost             *host
  );

#endif
