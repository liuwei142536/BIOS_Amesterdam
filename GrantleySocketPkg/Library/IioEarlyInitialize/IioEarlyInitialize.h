//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioEarlyInitialize.h

Abstract:

  This file defines everything that's needed for firmware to access
  and manipulate the Intel(R) silicon chip.
  All registers, bits, constants, etc. that are described in the chip's
  EDS are represented here.
  This file usurps all defines that relate to this chip. This means that
  it is inappropriate to define any of such define in any other file.
  Although this file maybe incomplete while under construction (maybe it
  will never be completed as there may be no need for access to all
  features), any new define must go here and follow naming conventions
  set therefore.
  In an ideal world, this file dould originate from the hardware design
  team that designed the chip.

--*/
#ifndef _IIO_EARLY_INITIALIZE_H_
#define _IIO_EARLY_INITIALIZE_H_

#include <Library\OemIioInit.h>
#include "PciAccess.h"
#include "MmioAccess.h"
#include "IoAccess.h"
#include "IioMisc.h"
#include "IoAccess.h"
#include "IioVData.h"
#include <Library/IioEarlyInitializeLib.h>
#ifndef IA32
#include <Library/BaseMemoryLib.h>
#include <Library/S3BootScriptLib.h>
#endif // IA32
#include "IioGen3.h"
#include "SysFunc.h"

VOID
IioEarlyWorkAround (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio,
    IIO_INIT_ENUMERATION     Phase
    );

VOID
DfxPcieInit (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio
  );

VOID
WaitTrainingFinished (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    BusNumber,
    UINT8                    PciePortDeviceNumber,
    UINT8                    Port
  );

VOID
LimitPcieLinkWidth(
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio
);

VOID
ProgramIioClockConfiguration (
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio
);

VOID
WriteCsrTable(
    IIO_GLOBALS              *IioGlobalData,
    UINT8                    Iio,
    UINT8                    BusNumber,
    CSR_ACCESS_OP_STRUCT     *RxRecipe,
    UINT32                   RxRecipeSize
 );

VOID IioPreLinkInit(
   IIO_GLOBALS *IioGlobalData,
   UINT8  Iio
);

VOID IioRxRecipeSettingsAuto (
  IIO_GLOBALS                *IioGlobalData,
  UINT8                      Iio
);

VOID
Gen3ProgCTLEp(
   IIO_GLOBALS               *IioGlobalData,
   UINT8                     Iio,
   UINT8                     BusNumber
);

#ifdef BDX_HOST
VOID ProgramEcrc(
  IIO_GLOBALS *IioGlobalData,
  UINT8  Iio
);
#endif // BDX_HOST

VOID
IioUpdatePcieConfigurationData(
  IIO_GLOBALS *IioGlobalData,
  UINT8       Iio
);
#endif
