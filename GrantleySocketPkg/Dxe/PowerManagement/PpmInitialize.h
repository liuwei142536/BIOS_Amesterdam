//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Header File for CPU Power Management

  Copyright (c) 2006 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  PpmInitialize.h

**/

#ifndef _PPM_INITIALIZE_H
#define _PPM_INITIALIZE_H

#include "Library/Pcdlib.h"
#include <Library/PciExpressLib.h>
#include <Protocol/PciCallBack.h>
#include <Protocol/PpmProcessorSupport.h>
#include <Protocol/MpService.h>
#include <Protocol/ExitPmAuth.h>
#include <Setup/IioUniversalData.h>
//Libraries
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Guid/HobList.h>
#include <Guid/PlatformInfo.h>
#include <Guid/MemoryMapData.h>
#include <Protocol/CpuCsrAccess.h>
#include <RcRegs.h>
#include <Library/UefiBootServicesTableLib.h>  
#include <Library/CpuPpmLib.h>

#define B_PCPS_SPD_ENABLE                  (1 << 16) // Bit 16
#define B_PCPS_HT_ENABLE                   (1 << 17) // Bit 17
#define LLC_CLICE_EN_MASK            ((1 << 24) - 1) // Bit[17:0]
#define SKU_18C                      0x3FFFF

#define THREAD_COUNT_MASK           (0xFFFF)

extern EFI_MP_SERVICES_PROTOCOL     *mMpService;
extern PPM_FROM_PPMINFO_HOB         *mPpmInfo;
extern EFI_CPU_PM_STRUCT            mPpmLib;


VOID 
InitializeCpuInfoStruct( 
    VOID
);

VOID
CpuPmProgramMsrs (
    EFI_CPU_PM_STRUCT *ppm
);

VOID
CpuPmProgramCsrs (
    VOID
);

EFI_STATUS
CpuPpmInitDone (
    VOID
);

VOID
EFIAPI
PpmOnReadyToBoot (
    IN      EFI_EVENT                 Event,
    IN      VOID                      *Context
);

#endif
