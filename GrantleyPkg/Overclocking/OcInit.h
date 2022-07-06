/** @file
  Describes the functions visible to the rest of the OcInit.

@copyright
  Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
#ifndef _OC_INIT_H_
#define _OC_INIT_H_

#include <Protocol/PlatformType.h>

#include "CpuPlatformPolicy.h"
#include "SaPlatformPolicy.h"
#include "OcSupport.h"

#ifdef USE_WDT_IN_DEBUG_BIOS
//
// MRC takes a lot of time to execute in debug mode
//
#define WDT_TIMEOUT_BETWEEN_PEI_DXE 120
#else
#define WDT_TIMEOUT_BETWEEN_PEI_DXE 60
#endif

#define UBOXBUS 0x10
#define UBOXDEV 0x07
#define UBOXSCOFFSET 0x68

typedef struct _OC_DATA_HOB{
	UINT8  DmiPeg;
}OC_DATA_HOB;

///
/// Function Prototypes
///

EFI_STATUS
EFIAPI
OcEntryPoint(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
;
/**
  Initializes Overclocking settings in the processor.

  @param[in] PeiServices - General purpose services available to every PEIM.
  @param[in] OverclockingtConfig      Pointer to Policy protocol instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
SaOcInit (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN       SA_PLATFORM_POLICY_PPI      *SaPlatformPolicyPpi
  )
;

/**
  Initializes Overclocking settings in the processor.

  @param[in] PeiServices - General purpose services available to every PEIM.
  @param[in] OverclockingtConfig      Pointer to Policy protocol instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
CpuOcInit (
  EFI_PEI_SERVICES CONST      **PeiServices,
  volatile PEI_CPU_PLATFORM_POLICY_PPI *CpuPlatformPolicyPpi
  );

#endif //_OC_INIT_H_

