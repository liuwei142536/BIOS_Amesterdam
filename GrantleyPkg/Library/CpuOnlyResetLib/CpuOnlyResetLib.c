//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/** @file
  Library functions for CpuOnlyResetLib.
  This library instance provides CPU only reset functionality with Gmch945 chipset.

  Copyright (c) 2007 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuLib.h>

/**
  This function does a CPU only reset.
  This function must be called after PeiMchPreMemInit();
  If the function cannot complete it'll ASSERT().
**/
VOID
EFIAPI
CpuOnlyReset(
  VOID
  )
{
  //
  // Make sure interrupts are disabled 
  //
  DisableInterrupts ();

  //
  // Reset Cpu
  //
  //Var32 = PciExpressRead32 (MC_MMIO_CSINCC1); 
  //Var32 |= BIT4;
  //PciExpressWrite32(MC_MMIO_CSINCC1, Var32)

  //
  // Execute a HLT instruction
  //
  while (TRUE) {
    CpuSleep();
  }
}
