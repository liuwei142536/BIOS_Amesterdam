/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
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

#pragma warning(disable:4740)

#include "SysFunc.h"


static
struct mcBankTableEntry mcBankTable[] = {
    //
    // List the MC Banks in the order of closeness to possible source of error
    // i.e., first IMC, HA, QPI/PCU, IIO, and finally CBO
    //
    {9,   0x424}, // IMC0
    {10,  0x428}, // IMC1
    {11,  0x42c}, // IMC2
    {12,  0x430}, // IMC3
    {13,  0x434}, // IMC4
    {14,  0x438}, // IMC5
    {15,  0x43c}, // IMC6
    {16,  0x440}, // IMC7
    {7,   0x41c}, // HA0
    {8,   0x420}, // HA1
    {4,   0x410}, // PCU / QPI2
    {5,   0x414}, // QPI0/QPI1
    {6,   0x418}, // IIO
    {17,  0x444}, // CBO0
    {18,  0x448}, // CBO1
    {19,  0x44c}, // CBO2
    {20,  0x450}, // CBO3
    {21,  0x454}, // CBO4
    /*{22,  0x458}, // CBO5
    {23,  0x45c}, // CBO6
    {24,  0x460}, // CBO7
    {25,  0x464}, // CBO8
    {26,  0x468}, // CBO9
    {27,  0x46c}, // CBO10
    {28,  0x470}, // CBO11
    {29,  0x474}, // CBO12
    {30,  0x478}, // CBO13
    {31,  0x47c}, // CBO14*/
};

#define CBO00_MCBANK  17
#define CBO04_MCBANK  21


void
FatalError (
  PSYSHOST host,
  UINT8    majorCode,
  UINT8    minorCode
  )
/*++

  Outputs error code hand halts if halt on fatal error, else exits reference code

  @param host      - Pointer to the system host (root) structure
  @param majorCode - Major error code to log
  @param minorCode - Minor error code to log

  @retval VOID

--*/
{
  UINT8   haltOnError;
  UINT32  buffer;

  //
  // Read the last checkpoint
  //
  buffer = host->var.common.checkpoint & 0xFFFF0000;

  //
  // Write the major and minor error codes to the scratchpad register
  //
  buffer |= ((majorCode << 8) | minorCode);
  WriteCpuPciCfgEx (host, 0, 0, SR_ERROR_CODE_CSR, buffer);

  //
  // Default halt on error based on setup option
  //
  if (host->setup.common.options & HALT_ON_ERROR_EN) {
    haltOnError = 1;
  } else {
    haltOnError = 0;

  }

  if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && (host->var.common.socketPresentBitMap > 1)) {
    host->var.mem.socket[host->var.common.socketId].fatalError = 1;
    host->var.mem.socket[host->var.common.socketId].majorCode = majorCode;
    host->var.mem.socket[host->var.common.socketId].minorCode = minorCode;
    if (host->var.common.socketId == 0) {
      //
      // Let all the non-BSP's know the BSP has a fatal error
      //
      rcPrintf ((host, "\nFatalError: BSP - SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
                host->var.common.socketId, majorCode, minorCode));

      //
      // Sync with non-BSP sockets
      //
      SyncErrors(host);

    } else {
      //
      // Let the BSP know this non-BSP had a fatal error
      //
      rcPrintf ((host, "\nFatalError: nonBSP -  SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
                host->var.common.socketId, majorCode, minorCode));

      SyncErrors(host);
    }
  } else {
    rcPrintf ((host, "\nFatalError: SocketId = %d registered Major Code = 0x%2x, Minor Code = 0x%2x \n",
              host->var.common.socketId, majorCode, minorCode));
  }

  //
  // Call platform hook to handle fatal error
  //
  FatalErrorHook (host, majorCode, minorCode, &haltOnError);

  HaltOnError(host, majorCode, minorCode);
}

void
CollectPrevBootFatalErrors(
   PSYSHOST host
   )
/*++

  Checks MSRs with UC set and logs the register data for further use

  @param host

  @retval N/A

 Collect only UNCORE MC Bank information.
--*/
{
#ifdef IA32
  UINT16  noOfMcBanks = sizeof(mcBankTable)/sizeof(struct mcBankTableEntry);
  UINT8   mcIndex;
  UINT8   socket;
  UINT64_STRUCT tempData;
  UINT8   entry;
  UINT8   cboCount;
  UINT32  cboMap[MAX_CPU_SOCKETS];

  //
  // Get the list of CBOs present for each socket
  //
  for (socket = 0; socket < MAX_CPU_SOCKETS; socket++) {
    //
    // Skip skts that are not present
    //
    cboMap[socket] = 0;
    if ((host->var.common.socketPresentBitMap & (1 << socket)) == 0) continue;
    GetCboCountAndList (host, socket, &cboCount, &cboMap[socket]);
  }

  entry = 0;
  host->prevBootErr.validEntries = 0;
  for (mcIndex = 0; mcIndex < noOfMcBanks; mcIndex++) {
    for (socket = 0; socket < MAX_CPU_SOCKETS; socket++) {
      //
      // Skip skts that are not present
      //
      if ((host->var.common.socketPresentBitMap & (1 << socket)) == 0) continue;

      if (host->prevBootErr.validEntries >= MAX_PREV_BOOT_ERR_ENTRIES) {
#ifdef SERIAL_DBG_MSG
        rcPrintf ((host, "Warning! Num of MC BANKS with UC error exceeds %d - max entries!\n", MAX_PREV_BOOT_ERR_ENTRIES));
#endif // SERIAL_DBG_MSG
        return;
      }

      //
      // Skip CBOs that are not present
      //
      if ((mcBankTable[mcIndex].mcBankNum >= CBO00_MCBANK) && (mcBankTable[mcIndex].mcBankNum <= CBO04_MCBANK)) {
        if (((cboMap[socket] >> (mcBankTable[mcIndex].mcBankNum - CBO00_MCBANK)) & 1) == 0) {
#ifdef SERIAL_DBG_MSG
          rcPrintf ((host, "PrevBootErrors - CBO mcbank: %d - not present; skipping...\n", mcBankTable[mcIndex].mcBankNum));
#endif // SERIAL_DBG_MSG
          continue;
        }
      }

      tempData = ReadMsrPipe(host, socket, mcBankTable[mcIndex].mcBankAddress + MC_BANK_STATUS_REG);

      //
      // Skip if there is no fatal error / log not valid / not enabled
      //
      if ((tempData.hi & (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) != (MSR_LOG_VALID | MSR_LOG_UC | MSR_LOG_EN)) continue;

      //
      // Capture log information
      //
      host->prevBootErr.mcRegs[entry].socketId  = socket;
      host->prevBootErr.mcRegs[entry].mcBankNum = mcBankTable[mcIndex].mcBankNum;
      host->prevBootErr.mcRegs[entry].mcStatus  = tempData;
      host->prevBootErr.mcRegs[entry].mcAddress = ReadMsrPipe(host, socket, mcBankTable[mcIndex].mcBankAddress + MC_BANK_ADDRESS_REG);
      host->prevBootErr.mcRegs[entry].mcMisc    = ReadMsrPipe(host, socket, mcBankTable[mcIndex].mcBankAddress + MC_BANK_MISC_REG);

      entry++;
      host->prevBootErr.validEntries = entry;
    } // skt loop
  } // mcbank loop

#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "PrevBootErrors - Valid MCA UC entries: %d\n", host->prevBootErr.validEntries));
#endif // SERIAL_DBG_MSG
#endif // IA32
}