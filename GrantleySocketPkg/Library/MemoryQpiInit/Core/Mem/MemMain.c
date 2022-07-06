/**
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
**/
/*************************************************************************
 *
 * Memory Reference Code
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
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains memory detection and initialization for
 *      IMC and DDR modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "Pipe.h"

//
// Local Prototypes
//
UINT32 CheckPF(PSYSHOST host, UINT8 policyFlag);
UINT32 CheckMPFlag(PSYSHOST host, UINT8 MPFlag);


// -------------------------------------
// Code section
// -------------------------------------

/**
  Main function used to initialize the memory controler on each processor.

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
MemMain (
  PSYSHOST                  host,
  CallTableEntryType        *ctptrOrg,
  UINT8                     callTableEntries
  )
{
  UINT8                     socket;
  UINT8                     startSocket;
  UINT8                     maxSocket;
  UINT8                     tableIndex;
#ifdef SERIAL_DBG_MSG
  UINT64_STRUCT             startTsc  = { 0, 0 };
  UINT64_STRUCT             firstStartTsc  = { 0, 0 };
  UINT64_STRUCT             endTsc = { 0, 0 };
#endif  // SERIAL_DBG_MSG
  UINT32                    status;
  CallTableEntryType        *ctptr;

  status = FAILURE;
  host->var.common.rcWriteRegDump = 1;

  //
  // Loop through the table
  //
  for (tableIndex = 0; tableIndex < callTableEntries; tableIndex++) {
    ctptr = &ctptrOrg[tableIndex];

    //
    // Decide if we need to execute the selected MRC task
    //
    if (ctptr->mrcTask == NULL) {
      //MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      //              "mrcTask == NULL; tableIndex = %d\n", tableIndex));
      continue;
    }

    //
    // Check policy flag and MP flag before calling this function
    //
    if  (CheckPF(host, ctptr->policyFlag) && CheckMPFlag(host, ctptr->mpFlag)) {

      //
      // Output debug string to serial output and execute the MRC task
      //
#ifdef SERIAL_DBG_MSG
      getPrintFControl(host);
      //MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      //              ctptr->debug_string_ptr));
      rcPrintf ((host, ctptr->debug_string_ptr));
      rcPrintf ((host, " -- Started\n"));
      releasePrintFControl(host);
#endif // SERIAL_DBG_MSG

      //
      // Output the POST code
      //
      if ((ctptr->postCodeMajor != 0) && (ctptr->postCodeMajor != 0xFF)) {
#ifdef  DEBUG_PERFORMANCE_STATS
#ifdef SERIAL_DBG_MSG
        SetMemPhaseCP (host, ctptr->postCodeMajor, ctptr->postCodeMinor);
#endif  // SERIAL_DBG_MSG
#endif  // DEBUG_PERFORMANCE_STATS
        OutputCheckpoint (host, ctptr->postCodeMajor, ctptr->postCodeMinor, 0);
      }

      // TODO: Call OEM function

      //
      // Set up socket loop
      //
      if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && ((host->var.common.socketPresentBitMap < 0x7) || (host->var.common.resetRequired == 0))) {
        startSocket = host->var.common.socketId;
        maxSocket = startSocket + 1;
      } else {
        if (ctptr->mpFlag & MRC_MP_LOOP) {
          // Loop for each socket
          startSocket = 0;
          maxSocket = MAX_SOCKET;
        } else {
          startSocket = host->var.common.socketId;
          maxSocket = startSocket + 1;
        }
      }

#ifdef SERIAL_DBG_MSG
      //
      // Start time
      //
      ReadTsc64 (&startTsc);
      if ((firstStartTsc.hi == 0) && (firstStartTsc.lo == 0)) {
        //
        // Save the first instance so we can calculate the total time spent in the MRC
        //
        firstStartTsc.hi = startTsc.hi;
        firstStartTsc.lo = startTsc.lo;
      }
#endif // SERIAL_DBG_MSG

      status = SUCCESS;

      for (socket = startSocket; socket < maxSocket; socket++) {
        if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

        //
        // Save the socket that this function is being called for
        //
        host->var.mem.currentSocket = socket;

        //
        // Call the function
        //
        status |= ctptr->mrcTask (host);
      } // socket loop

#ifdef SERIAL_DBG_MSG
      //
      // End time
      //
      ReadTsc64 (&endTsc);

#ifdef  DEBUG_PERFORMANCE_STATS
      countTrackingData (host, DURATION_TIME, TimeDiff (host, startTsc, endTsc, TDIFF_UNIT_MS));
      ClearMemPhaseCP(host, ctptr->postCodeMajor, ctptr->postCodeMinor);
#endif  // DEBUG_PERFORMANCE_STATS
      getPrintFControl(host);
      //MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      //              ctptr->debug_string_ptr));
      rcPrintf ((host, ctptr->debug_string_ptr));
#endif //SERIAL_DBG_MSG

      //
      // Only check to see if a reset is required. Do not exit the dispatcher for failures.
      //
      if ((status == POST_RESET_WARM) || (status == POST_RESET_POWERGOOD)){
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, " -- EXIT, status = %Xh\n", status));
        releasePrintFControl(host);
#endif //SERIAL_DBG_MSG
        break;
      } else {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, " - %dms\n", TimeDiff (host, startTsc, endTsc, TDIFF_UNIT_MS)));
        releasePrintFControl(host);
#endif //SERIAL_DBG_MSG
      }
    }
#ifdef SERIAL_DBG_MSG
    else {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "mrcTask skipped; Index = %d\n", tableIndex));
    }
#endif //SERIAL_DBG_MSG
  } // tableIndex loop

#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "Total MRC time = %dms\n", TimeDiff (host, firstStartTsc, endTsc, TDIFF_UNIT_MS)));
  releasePrintFControl(host);
#endif //SERIAL_DBG_MSG
  return status;
} // MemMain


/**
  This function dispatches the slave processors to start MemMain

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
DispatchSlaves (
  PSYSHOST                  host
  )
{
  UINT8         socket;
  COMMAND_PIPE  cmdPipe;

  cmdPipe.RawPipe = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    host->var.mem.socket[socket].fatalError = 0;
    host->var.mem.socket[socket].majorCode  = 0;
    host->var.mem.socket[socket].minorCode  = 0;

    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    //
    // Do not execute this for the SBSP
    //
    if (socket == host->var.common.socketId) continue;

    //
    // Initialize command pipe of slave before starting the slave
    //
    LockCommandPipe (host, socket);

    //
    // Can now dispatch the slaves
    // Cause the pipe dispatcher to call the memory init entry point on each slave
    //
    OutputExtendedCheckpoint((host, STS_DIMM_DETECT, SUB_DISPATCH_SLAVES, socket));
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Dispatch N%d for MemInit\n", socket));

    WriteCpuPciCfgEx (host, socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR, PIPE_DISPATCH_MEMORY_INIT);

    //
    // Parallel DDR Init. Send PIPE_RUN_MEMORY_COMMAND to each slave (package BSPs)
    //
    cmdPipe.Pipe.InternalCmd = PIPE_RUN_MEMORY_COMMAND;

    WaitForCommandPipeUnLocked (host, socket);
    cmdPipe.Pipe.CmdRequest = PIPE_REQUEST;
    cmdPipe.Pipe.TargetId   = socket;
    SendCommandPipe (host, socket, &cmdPipe.RawPipe);
  } // socket loop

  return SUCCESS;
} // DispatchSlaves


/**
  This function dispatches the slave processors to start MemMain

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
SlaveAck (
  PSYSHOST                  host
  )
{
  COMMAND_PIPE  cmdPipe;

  cmdPipe.RawPipe = 0;

  UnlockCommandPipe (host, host->var.common.socketId);

  //
  // Waits for PIPE Master's PIPE_RUN_MEMORY_COMMAND to run memory init
  //
  WaitForPipeCommand (host, host->var.common.socketId, RUN_MEMORY_COMMAND);

  //
  // Respond ACK
  //
  cmdPipe.Pipe.Status     = PIPE_SUCCESS;
  cmdPipe.Pipe.NumOfBytes = 0;
  cmdPipe.Pipe.Width_     = 0;
  cmdPipe.Pipe.CmdRequest = PIPE_ACK;
  WriteCommandPipe (host, host->var.common.socketId, &cmdPipe.RawPipe);

  return SUCCESS;
} // SlaveAck

/**
  This function syncs data across sockets

  @param host:         Pointer to sysHost

  @retval SUCCESS: Sync successfull
**/
UINT32
PipeSync (
  PSYSHOST                  host
  )
{
  UINT32  status = SUCCESS;

  //
  // Check to make sure none of the threads have encountered an error
  //
  SyncErrors(host);

  if (host->var.mem.currentSocket == host->nvram.common.sbspSocketId) {
    //
    // SBSP path
    //
    switch (host->var.mem.pipeSyncIndex) {
    case 0:
      GetSlaveData(host);
      break;
    case 1:
      status = SendStatus(host);
      break;
    case 2:
      SendData(host);
      break;
    case 3:
      GetNvramData(host);
      break;
    default:
      RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0)
    }
  } else {
    //
    // non-SBSP path
    //
    switch (host->var.mem.pipeSyncIndex) {
    case 0:
      SendSlaveData(host);
      break;
    case 1:
      status = GetStatus(host);
      break;
    case 2:
      GetData(host);
      break;
    case 3:
      SendNvramData(host);
      break;
    default:
      RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0)
    }
  }

  //
  // Increment index to the next sync point
  //
  host->var.mem.pipeSyncIndex++;

  return status;
} // PipeSync

/**
  This function gets data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetSlaveData (
  PSYSHOST                  host
  )
{
  UINT8   socket;
  UINT16  sbspSktId;

  sbspSktId = GetSbspSktId (host);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    if (socket == host->var.common.socketId) continue;

    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "GetSlaveData S%d -> S%d\n", socket, sbspSktId));

    OutputExtendedCheckpoint((host, STS_DIMM_DETECT, SUB_GET_SLAVE_DATA, (UINT16)((socket << 8)|DATA_TYPE_VAR)));
    GetMemVarData (host, socket);

    GetMemNvramCommonData (host, socket);
  } // socket loop

  return SUCCESS;
} // GetSlaveData

/**
  This function sends data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendSlaveData (
  PSYSHOST                  host
  )
{
  UINT16  sbspSktId;

  sbspSktId = GetSbspSktId (host);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "SendSlaveData S%d -> S%d\n", host->var.common.socketId, sbspSktId));
  SendMemVarData (host, host->var.common.socketId);

  SendMemNvramCommonData (host, host->var.common.socketId);

  return SUCCESS;
} // SendSlaveData

/**
  This function sends the DDR frequency to slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendFreq (
  PSYSHOST                  host
  )
{
  UINT8   socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    if (socket == host->var.common.socketId) continue;

    //
    // Send the status variable to each socket
    //
    OutputExtendedCheckpoint((host, STS_CLOCK_INIT, SUB_SEND_FREQ, socket));
    SendPipePackage(host, socket, (UINT8*)&host->nvram.mem.socket[socket].ddrFreq, sizeof(UINT8));

  } // socket loop

  // Return requesting a reset
  return SUCCESS;
} // SendFreq

/**
  This function gets the DDR frequency from the master processor

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetFreq (
  PSYSHOST                  host
  )
{

  GetPipePackage(host, host->var.common.socketId, (UINT8*)&host->nvram.mem.socket[host->var.mem.currentSocket].ddrFreq, sizeof(UINT8));
  return SUCCESS;

} // GetFreq

/**
  This function returns the reset status

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
CheckStatus (
  PSYSHOST  host
  )
{

  // Return requesting a reset
  return host->var.common.resetRequired;
} // CheckStatus

/**
  This function sends data to slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendStatus (
  PSYSHOST  host
  )
{
  UINT8   socket;

  // Let the AP's know if a reset is needed
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    if (socket == host->var.common.socketId) continue;

    //
    // Send the status variable to each socket
    //
    OutputExtendedCheckpoint((host, STS_CLOCK_INIT, SUB_SEND_STATUS, socket));
    SendPipePackage(host, socket, (UINT8*)&host->var.common.resetRequired, sizeof(UINT32));

  } // socket loop

  // Return requesting a reset
  return host->var.common.resetRequired;
} // SendStatus

/**
  This function gets data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetStatus (
  PSYSHOST                  host
  )
{

  GetPipePackage(host, host->var.common.socketId, (UINT8*)&host->var.common.resetRequired, sizeof(UINT32));
  return host->var.common.resetRequired;

} // GetStatus

/**
  This function sends data to slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendData (
  PSYSHOST                  host
  )
{
  UINT8         socket;
  COMMAND_PIPE  cmdPipe;

  cmdPipe.RawPipe = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    if (socket == host->var.common.socketId) continue;

    OutputExtendedCheckpoint((host, STS_RANK_DETECT, SUB_SEND_DATA, (UINT16)((socket << 8)|DATA_TYPE_VAR)));
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SendData S%d -> S%d\n", host->var.common.socketId, socket));
    SendMemVarData (host, socket);

    OutputExtendedCheckpoint((host, STS_RANK_DETECT, SUB_SEND_DATA, (UINT16)((socket << 8)|DATA_TYPE_NVRAM_COMMON)));
    SendMemNvramCommonData (host, socket);

    OutputExtendedCheckpoint((host, STS_RANK_DETECT, SUB_SEND_DATA, (UINT16)((socket << 8)|DATA_TYPE_NVRAM_DATA)));
    SendMemNvramData (host, socket);

    //
    // Continue parallel DDR Init. Send PIPE_RUN_MEMORY_COMMAND to each socket
    //
    cmdPipe.Pipe.InternalCmd  = PIPE_RUN_MEMORY_COMMAND;

    cmdPipe.Pipe.CmdRequest   = PIPE_REQUEST;
    cmdPipe.Pipe.TargetId     = socket;

    SendCommandPipe (host, socket, &cmdPipe.RawPipe);
  } // socket loop

  return SUCCESS;
} // SendData

/**
  This function gets data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetData (
  PSYSHOST                  host
  )
{
  COMMAND_PIPE  cmdPipe;
  UINT8 sbspSktId;

  sbspSktId = GetSbspSktId (host);

  cmdPipe.RawPipe = 0;

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "GetData S%d -> S%d\n", sbspSktId, host->var.common.socketId));
  GetMemVarData (host, host->var.common.socketId);
  GetMemNvramCommonData (host, host->var.common.socketId);
  GetMemNvramData (host, host->var.common.socketId);

  //
  // Waits for PIPE Master's PIPE_RUN_MEMORY_COMMAND to run memory init
  //
  WaitForPipeCommand (host, host->var.common.socketId, RUN_MEMORY_COMMAND);

  //
  // Respond ACK
  //
  cmdPipe.Pipe.Status     = PIPE_SUCCESS;
  cmdPipe.Pipe.NumOfBytes = 0;
  cmdPipe.Pipe.Width_     = 0;
  cmdPipe.Pipe.CmdRequest = PIPE_ACK;
  WriteCommandPipe (host, host->var.common.socketId, &cmdPipe.RawPipe);

  return SUCCESS;
} // GetData

/**
  This function gets data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetNvramData (
  PSYSHOST                  host
  )
{
  UINT8 socket;
  UINT8 sbspSktId;

  sbspSktId = GetSbspSktId (host);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    if (socket == host->var.common.socketId) continue;

    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "GetNvramData S%d -> S%d\n", socket, sbspSktId));
    GetMemNvramCommonData (host, socket);
  } // socket loop

  return SUCCESS;
} // GetNvramData

/**
  This function sends data from slave processors

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendNvramData (
  PSYSHOST                  host
  )
{
  UINT8 sbspSktId;

  sbspSktId = GetSbspSktId (host);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "SendNvramData S%d -> S%d\n", host->var.common.socketId, sbspSktId));
  SendMemNvramCommonData (host, host->var.common.socketId);

  return SUCCESS;
} // SendNvramData

/**
  Checks the policy flag

  @param host:         Pointer to sysHost
  @param policyFlag

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
CheckPF (
  PSYSHOST  host,
  UINT8     policyFlag
  )
{
  if  (((host->var.common.bootMode == S3Resume) && (policyFlag & MRC_PF_S3))
    || (((host->var.mem.subBootMode == WarmBootFast) || (host->var.mem.subBootMode == ColdBootFast)) && (policyFlag & MRC_PF_FAST))
#ifdef MEM_NVDIMM_EN
    || (host->var.mem.subBootMode == NvDimmResume)
#endif
    || ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot) && (policyFlag & MRC_PF_COLD))) {
    return 1;
  } else {
    return 0;
  }
}

/**
  Checks the policy flag

  @param host:         Pointer to sysHost
  @param policyFlag

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
CheckMPFlag (
  PSYSHOST  host,
  UINT8     MPFlag
  )
{
  UINT8 parallel;
  if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && ((host->var.common.socketPresentBitMap < 0x7) || (host->var.common.resetRequired == 0))) {
    parallel = 1;
  } else {
    parallel = 0;
  }
  if  (((parallel == 0) && (MPFlag & MRC_MP_SERIAL))
    || ((parallel == 1) && (MPFlag & MRC_MP_PARALLEL))) {
    return 1;
  } else {
    return 0;
  }
}



