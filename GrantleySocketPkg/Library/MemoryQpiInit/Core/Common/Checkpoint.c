/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
/*************************************************************************
 *
 * Reference Code
 *
 * ESS - Enterprise Silicon Software
 *
 * INTEL CONFIDENTIAL
 *
 * Copyright 2006 - 2015 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors. Title to the Material
 * remains with Intel Corporation or its suppliers and licensors.
 * The Material contains trade secrets and proprietary and confidential
 * information of Intel or its suppliers and licensors. The Material
 * is protected by worldwide copyright and trade secret laws and treaty
 * provisions.  No part of the Material may be used, copied, reproduced,
 * modified, published, uploaded, posted, transmitted, distributed, or
 * disclosed in any way without Intel's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 *
 ************************************************************************/

#include "SysFunc.h"

#ifdef SSA_FLAG
#include "MrcSsaServices.h"
#include "ExecuteTargetOnly.h"
#endif //SSA_FLAG

#define JEDEC_INIT_SEQ_REQ  (1 << 24)
#define RANK_MARGIN_TOOL_REQ  (2 << 24)
/*
  Low Level Manager
  Tool for making BIOS function calls

  @param host - Pointer to sysHost

  @retval SUCCESS
*/
UINT32
ValRequestHandlerAtBreakPoint (
  PSYSHOST host
  )
{
  UINT8  socket;
  UINT8  currentSocket;
  UINT32 csrReg = 0;
  UINT32 spad6 = 0;
  UINT8  sbspSktId;

  sbspSktId = GetSbspSktId(host);

  csrReg = GetStickyScratchpad (host, sbspSktId, 5);
  if (csrReg & JEDEC_INIT_SEQ_REQ) {
    rcPrintf ((host, "Request to run Jedec Init\n"));
    spad6 = GetStickyScratchpad (host, sbspSktId, 6);
    SetStickyScratchpad (host, sbspSktId, 5, 0);
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) continue;
      JedecInitSequence(host, socket, CH_BITMASK);
    }
    csrReg &= ~JEDEC_INIT_SEQ_REQ;
    SetStickyScratchpad (host, sbspSktId, 5, csrReg);
    SetStickyScratchpad (host, sbspSktId, 6, spad6);
  }

  if ((csrReg & RANK_MARGIN_TOOL_REQ) && (host->setup.mem.options & MARGIN_RANKS_EN)) {
    rcPrintf ((host, "Request to run Rank Margin tool\n"));
    spad6 = GetStickyScratchpad (host, sbspSktId, 6);
    SetStickyScratchpad (host, sbspSktId, 6, 0);
    currentSocket = host->var.mem.currentSocket;
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) continue;
      host->var.mem.currentSocket = socket;
#ifdef MARGIN_CHECK
      RankMarginTool(host);
#endif
    }
    host->var.mem.currentSocket = currentSocket;
    csrReg &= ~RANK_MARGIN_TOOL_REQ;
    SetStickyScratchpad (host, sbspSktId, 5, csrReg);
    SetStickyScratchpad (host, sbspSktId, 6, spad6);
  }
  return SUCCESS;
}


//
// -----------------------------------------------------------------------------
//
/**

  Socket SBSP writes the checkpoint code to the checkpoint CSR on socket SBSP and calls
  the platform checkpoint routine.

  Socket N reads the breakpoint CSR on socket N. If the breakpoint code matches the
  checkpoint code, then execution will stall in a loop until the breakpoint CSR
  on socket N is changed via ITP or other means.

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major Checkpoint code to write
  @param minorCode - Minor Checkpoint code to write
  @param data      - Data specific to the minor checkpoint is written to
                     low word of the checkpoint CSR

  @retval N/A

**/
void
OutputCheckpoint (
                 PSYSHOST host,
                 UINT8    majorCode,
                 UINT8    minorCode,
                 UINT16   data
                 )
{
  UINT16 checkpoint;
  UINT32 csrReg;
  UINT8  socket;
#ifdef SSA_FLAG
#ifdef SSA_LOADER_FLAG
  MRC_EV_AGENT_CMD_STRUCT  agentCmd;
  UINT32                   agentReg;
#endif //SSA_LOADER_FLAG
#endif //SSA_FLAG
#ifdef YAM_ENV
  UINT32 temp;
#endif
  UINT8 sbspSktId;
  sbspSktId = GetSbspSktId(host);
  checkpoint = (UINT16)((majorCode << 8) | minorCode);
  csrReg = (UINT32)((checkpoint << 16) | data);

  //
  // Write checkpoint to local scratchpad
  //
  socket = host->var.common.socketId;
  SetNonStickyScratchpad (host, socket, SR_POST_CODE, csrReg);

  //
  // Cache last checkpoint value per socket
  //
  host->var.mem.lastCheckpoint[socket] = csrReg;

  if (host->var.common.socketId ==  host->nvram.common.sbspSocketId) {

    //
    // Update checkpoint in the host structure
    //
    host->var.common.checkpoint = csrReg;

    //
    // Call platform function to handle the major checkpoint
    //
    PlatformCheckpoint (host);

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) {
      //
      // Output the progress code
      //
      rcPrintf ((host, "Checkpoint Code: Socket %d, 0x%02X, 0x%02X, 0x%04X\n", host->var.common.socketId, majorCode, minorCode, data));
    }
#endif
  }

  // Compare the breakpoint CSR from socket 0
  csrReg = GetStickyScratchpad (host, sbspSktId, SR_BIOS_SERIAL_DEBUG);

  if ((csrReg >> 16) == checkpoint) {

#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX)) {

      // Output message indicating that execution is stalled
      rcPrintf ((host, "Breakpoint match found. S%d waiting...\n", host->var.common.socketId));
      SerialSendBuffer (host, 1);
    }
#endif

    // Stall the boot until breakpoint changes
    while (checkpoint == ((GetStickyScratchpad (host, sbspSktId, SR_BIOS_SERIAL_DEBUG)) >> 16)) {

      ValRequestHandlerAtBreakPoint(host);


#ifdef SSA_FLAG
#ifdef SSA_LOADER_FLAG
      if ((host->setup.mem.enableBiosSsaLoader) && (majorCode == 0xBE) && (minorCode == 0x02)) { //Enter if SSA EV loader is invoked
        // Check for opcode to start the SSA agent.
        agentReg = GetStickyScratchpad (host, 0, SR_BIOS_SERIAL_DEBUG);
        agentCmd.Data = 0x0000FFFF & agentReg;

        rcPrintf ((host, "Checkpoint() -- agentCmd.Bits.command_opcode: 0x%08x \n", agentCmd.Bits.command_opcode));
        switch(agentCmd.Bits.command_opcode) {

        case MRC_EVAGENT_CMD_START_AGENT:

          rcPrintf ((host, "MRC_EVAGENT_CMD_START_AGENT received. \n"));
          EvItpDownloadCmd(host, (VOID*)0);
          SetStickyScratchpad (host, 0, SR_BIOS_SERIAL_DEBUG, 0);
          break;

        case MRC_EVAGENT_CMD_START_TARGET_ONLY:

          rcPrintf ((host, "MRC_EVAGENT_CMD_START_TARGET_ONLY received. \n"));
          ExecuteTargetOnlyCmd(host);
          SetStickyScratchpad (host, 0, SR_BIOS_SERIAL_DEBUG, 0);
          break;
        }  //switch

        if (agentCmd.Bits.command_opcode == 0) { //To prevent a hang
          break;
        }
      } //enableBiosSsaLoader
#endif //SSA_LOADER_FLAG
#endif //SSA_FLAG
    } //while
  }
}

