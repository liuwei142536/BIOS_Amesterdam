/** @file
  Routines to access scratch pad register

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

#ifdef SSA_FLAG
#include "ScratchPadReg.h"
//
// Routines to access scratch pad register
//

UINT32
readScratchPad_CMD (
  PSYSHOST host
  )
{
  UINT32 val = 0;
  val = 0x0000FFFF & ReadCpuPciCfgEx (host, 0,  0, EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS);
  return val;
}

VOID
writeScratchPad_CMD (
  PSYSHOST             host,
  UINT32               val
  )
{
  WriteCpuPciCfgEx (host, 0, 0, EV_AGENT_SCRATCHPAD_CMD_REG_ADDRESS, val);
}

UINT32
readScratchPad_DATA0 (
  PSYSHOST              host
  )
{
  UINT32 val = 0;
  val = ReadCpuPciCfgEx (host, 0,  0, EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS);
  return val;
}

VOID
writeScratchPad_DATA0 (
  PSYSHOST              host,
  UINT32                val
  )
{
  WriteCpuPciCfgEx (host, 0, 0, EV_AGENT_SCRATCHPAD_DATA0_REG_ADDRESS, val);
}

UINT32
readScratchPad_DATA1 (
  PSYSHOST              host
  )
{
  UINT32 val = 0;
  val = ReadCpuPciCfgEx (host, 0,  0, EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS);
  return val;
}

VOID
writeScratchPad_DATA1 (
  PSYSHOST              host,
  UINT32                val
  )
{
  WriteCpuPciCfgEx (host, 0, 0, EV_AGENT_SCRATCHPAD_DATA1_REG_ADDRESS, val);
}

UINT32
readScratchPad0 (
  PSYSHOST              host
  )
{
  UINT32 val = 0;
  val = ReadCpuPciCfgEx (host, 0,  0, EV_AGENT_SCRATCHPAD_CHECKPOINT_REG_ADDRESS);
  return val;
}

VOID
writeScratchPad_currentCheckPoint (
  PSYSHOST             host,
  UINT32               val
  )
{
  WriteCpuPciCfgEx (host, 0, 0, EV_AGENT_SCRATCHPAD_CURRENT_CHECKPOINT_REG_ADDRESS, val);
}

#else
// Future placeholder: BSSA code intentionally left out for now
#endif  //SSA_FLAG
