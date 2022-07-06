/** @file
  Definitions of scratch pad registers used by BIOS SSA EV agent.

@copyright
  Copyright (c) 1999 - 2014 Intel Corporation. All rights reserved
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

#ifndef __SCRATCHPAD_REG_H__
#define __SCRATCHPAD_REG_H__

#include "SysFunc.h"
#include "MrcSsaServices.h"
#include <Library/DebugLib.h>

#ifdef SSA_FLAG
//
// Address for the scratchpad registers
//

#define EV_AGENT_SCRATCHPAD_CHECKPOINT_REG_ADDRESS          (BIOSSCRATCHPAD6_UBOX_MISC_REG) // upper 16 bits
#define EV_AGENT_SCRATCHPAD_CURRENT_CHECKPOINT_REG_ADDRESS  (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)
#define EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS                 (BIOSSCRATCHPAD6_UBOX_MISC_REG) // lower 16 bits
#define EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS               (BIOSSTICKYLOCKBYPASSSCRATCHPAD0_UBOX_CFG_REG)
#define EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS               (BIOSSTICKYLOCKBYPASSSCRATCHPAD1_UBOX_CFG_REG)

//
// Routines to access scratch pad register
//
UINT32
readScratchPad_CMD (
  PSYSHOST        host
 );

VOID
writeScratchPad_CMD (
  PSYSHOST        host,
  UINT32          val
);


UINT32
readScratchPad_DATA0 (
  PSYSHOST        host
  );

VOID
writeScratchPad_DATA0 (
  PSYSHOST         host,
  UINT32           val
  );

UINT32
readScratchPad_DATA1 (
  PSYSHOST         host
  );

VOID
writeScratchPad_DATA1 (
  PSYSHOST        host,
  UINT32          val
  );

UINT32
readScratchPad0 (
  PSYSHOST        host
  );

VOID
writeScratchPad_currentCheckPoint (
  PSYSHOST        host,
  UINT32          val
  );
  
#else
// Future placeholder: BSSA code intentionally left out for now
#endif //SSA_FLAG
#endif //__SCRATCHPAD_REG_H__
