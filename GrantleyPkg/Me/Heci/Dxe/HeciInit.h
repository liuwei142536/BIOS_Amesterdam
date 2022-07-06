/*++
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2007-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  HeciInit.h

Abstract:

  HECI Intializion DXE Driver

--*/
#ifndef _DXE_HECIINIT_H_
#define _DXE_HECIINIT_H_

#include <PiDxe.h>

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include "Library/HeciCoreLib.h"
#include "MeState.h"

#define ONE_SECOND_TIMEOUT  1000000
#define FWU_TIMEOUT         90

#define CMOS_ADDR_PORT          0x70
#define CMOS_DATA_PORT          0x71
#define UPPER_CMOS_ADDR_PORT    0x72
#define UPPER_CMOS_DATA_PORT    0x73
//AptioV workstaion override start: WA to avoid redefinition errors when including Token.h
#ifndef CMOS_BAD_REG
#define CMOS_BAD_REG            0x0E
#endif
//AptioV workstaion override end: WA to avoid redefinition errors when including Token.h


#endif // _DXE_HECIINIT_H_

