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
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"


VOID
SetSetupOptionsChip (
  PSYSHOST host
  )
/*++

  Set Setup Options

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval N/A

--*/
{
  return;
}

#ifdef SERIAL_DBG_MSG
/**

    Print Memory Setup Options

    @param host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
VOID
PrintMemSetupOptionsChip (
  PSYSHOST host,
  UINT8    socket
  )
{
  rcPrintf ((host, "  lowGap:          %02X\n", host->setup.common.lowGap));
  rcPrintf ((host, "  highGap:         %02X\n", host->setup.common.highGap));
  rcPrintf ((host, "  mmiohSize:       %08X\n", host->setup.common.mmiohSize));
  rcPrintf ((host, "  isocEn:          %02X\n", host->setup.common.isocEn));
  rcPrintf ((host, "  dcaEn:           %02X\n", host->setup.common.dcaEn));
  rcPrintf ((host, "  options (Chip):  %08X\n", host->setup.mem.options));
  rcPrintf ((host, " CMD_CLK_TRAINING_EN "));
  if (host->setup.mem.options & CMD_CLK_TRAINING_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ALLOW2XREF_EN "));
  if (host->setup.mem.options & ALLOW2XREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      RAS_TO_INDP_EN      "));
  if (host->setup.mem.options & RAS_TO_INDP_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      BANK_XOR_EN         "));
  if (host->setup.mem.options & BANK_XOR_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, " optionsExt (Chip): %08X\n", host->setup.mem.optionsExt));
  rcPrintf ((host, "  EARLY_CMD_CLK_TRAINING_EN "));
  if (host->setup.mem.optionsExt & EARLY_CMD_CLK_TRAINING_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  CMD_REF_EN "));
  if (host->setup.mem.optionsExt & CMD_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  LRDIMM_BACKSIDE_VREF_EN "));
  if (host->setup.mem.optionsExt & LRDIMM_BACKSIDE_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  LRDIMM_WR_VREF_EN "));
  if (host->setup.mem.optionsExt & LRDIMM_WR_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  LRDIMM_RD_VREF_EN "));
  if (host->setup.mem.optionsExt & LRDIMM_RD_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  LRDIMM_RX_DQ_CENTERING "));
  if (host->setup.mem.optionsExt & LRDIMM_RX_DQ_CENTERING) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  LRDIMM_TX_DQ_CENTERING "));
  if (host->setup.mem.optionsExt & LRDIMM_TX_DQ_CENTERING) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  XOVER_EN "));
  if (host->setup.mem.optionsExt & XOVER_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  DRAM_RON_EN "));
  if (host->setup.mem.optionsExt & DRAM_RON_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  RX_ODT_EN "));
  if (host->setup.mem.optionsExt & RX_ODT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  RTT_WR_EN "));
  if (host->setup.mem.optionsExt & RTT_WR_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  MC_RON_EN "));
  if (host->setup.mem.optionsExt & MC_RON_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  TX_EQ_EN "));
  if (host->setup.mem.optionsExt & TX_EQ_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  IMODE_EN "));
  if (host->setup.mem.optionsExt & IMODE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  RX_CTLE_TRN_EN "));
  if (host->setup.mem.optionsExt & RX_CTLE_TRN_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  SENSE_EN "));
  if (host->setup.mem.optionsExt & SENSE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  ROUND_TRIP_LATENCY_EN "));
  if (host->setup.mem.optionsExt & ROUND_TRIP_LATENCY_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  WR_CRC "));
  if (host->setup.mem.optionsExt & WR_CRC) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  DDR4_PLATFORM "));
  if (host->setup.mem.optionsExt & DDR4_PLATFORM) rcPrintf ((host, "1\n")); 
  else rcPrintf ((host, "0\n"));
  if (host->setup.mem.optionsExt & CAP_ERR_FLOW) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  if (host->setup.mem.optionsExt & CECC_WA) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  RASmode:               %02X\n", host->setup.mem.RASmode));
  rcPrintf ((host, "      RK_SPARE           "));
  if (host->setup.mem.RASmode & RK_SPARE) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      CH_LOCKSTEP "));
  if (host->setup.mem.RASmode & CH_LOCKSTEP) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      CH_MIRROR          "));
  if (host->setup.mem.RASmode & CH_MIRROR) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "\nstruct sysHost.setup.mem (Chip) {\n"));
  rcPrintf ((host, "  socketInter:     %02X\n", host->setup.mem.socketInter));
  rcPrintf ((host, "  rankInter:       %02X\n", host->setup.mem.rankInter));
  rcPrintf ((host, " dramraplExtendedRange: %02X\n", host->setup.mem.DramRaplExtendedRange));
  rcPrintf ((host, " dramMaint: %02X\n", host->setup.mem.dramMaint));
  rcPrintf ((host, " dramMaintTRRMode: %02X\n", host->setup.mem.dramMaintTRRMode));
  rcPrintf ((host, " dramMaintMode: %02X\n", host->setup.mem.dramMaintMode));
  rcPrintf ((host, "  electricalThrottling:  %02X\n", host->setup.mem.electricalThrottling));
  rcPrintf ((host, "  altitude: %02X\n", host->setup.mem.altitude));
  rcPrintf ((host, " forceRankMult: %02X\n", host->setup.mem.forceRankMult));
  rcPrintf ((host, " ceccWaChMask: %02X\n", host->setup.mem.ceccWaChMask));
  rcPrintf ((host, " perBitDeSkew: %02X\n", host->setup.mem.perBitDeSkew));
  rcPrintf ((host, "  spareErrTh:      %02X\n", host->setup.mem.spareErrTh));
  rcPrintf ((host, "  leakyBktLo:      %02X\n", host->setup.mem.leakyBktLo));
  rcPrintf ((host, "  leakyBktHi:      %02X\n", host->setup.mem.leakyBktHi));
#ifdef MEM_NVDIMM_EN
    rcPrintf ((host, " restoreNVDIMMS: %02X\n", host->setup.mem.restoreNVDIMMS));
#endif
#ifdef NVMEM_FEATURE_EN
  rcPrintf ((host, " interNVDIMMS: %02X\n", host->setup.mem.interNVDIMMS));
#endif
  rcPrintf ((host, " lockstepEnableX4: %02X\n", host->setup.mem.lockstepEnableX4));
  rcPrintf ((host, "  numSparTrans:  %04X\n", host->setup.mem.numSparTrans));
  rcPrintf ((host, " phaseShedding: %02X\n", host->setup.mem.phaseShedding));
  rcPrintf ((host, "  };\n"));

  //
  // ** struct ddrIMCSetup **
  //
  rcPrintf ((host, "  struct ddrIMCSetup[%02X] {\n", socket));
  rcPrintf ((host, "    enabled:        %08X\n", host->setup.mem.socket[socket].enabled));
  rcPrintf ((host, "  ddrVddLimit: %02X\n", host->setup.mem.socket[socket].ddrVddLimit));
  rcPrintf ((host, "  }\n"));

  return;
}
#endif // SERIAL_DBG_MSG

VOID
InitDdrFreqChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Initialize DDR Frequench

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval N/A

--*/
{

  return;
} // InitDdrFreqChip


UINT32
SavedSetupDataChangedChip (
  PSYSHOST host
  )
/*++

  Initialize internal data structures

  @param host   - Pointer to the system host (root) structure

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  if (
      host->nvram.mem.savedSetupData.RASmode != host->setup.mem.RASmode ||
      host->nvram.mem.savedSetupData.socketInter != host->setup.mem.socketInter ||
      host->nvram.mem.savedSetupData.rankInter != host->setup.mem.rankInter ||
      host->nvram.mem.savedSetupData.DramRaplExtendedRange != host->setup.mem.DramRaplExtendedRange ||
      host->nvram.mem.savedSetupData.dramMaint != host->setup.mem.dramMaint ||
      host->nvram.mem.savedSetupData.dramMaintTRRMode != host->setup.mem.dramMaintTRRMode ||
      host->nvram.mem.savedSetupData.dramMaintMode != host->setup.mem.dramMaintMode ||
      host->nvram.mem.savedSetupData.electricalThrottling != host->setup.mem.electricalThrottling ||
      host->nvram.mem.savedSetupData.altitude != host->setup.mem.altitude ||
      host->nvram.mem.savedSetupData.forceRankMult != host->setup.mem.forceRankMult ||
      host->nvram.mem.savedSetupData.spareErrTh != host->setup.mem.spareErrTh ||
      host->nvram.mem.savedSetupData.phaseShedding != host->setup.mem.phaseShedding ||
      host->nvram.mem.savedSetupData.logParsing != host->setup.mem.logParsing ||
      host->nvram.mem.savedSetupData.ceccWaChMask != host->setup.mem.ceccWaChMask
     ) {
      result = TRUE;
  }
  return result;
} // SavedSetupDataChangedChip


UINT32
DetectIMCEnabledChangeChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Detect IMC Enabled Change

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  return result;
} // DetectIMCEnabledChangeChip


UINT32
SavedSetupDataChangedIMCChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Initialize internal data structures

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  if (
       host->nvram.mem.savedSetupData.socket[socket].options != host->setup.mem.socket[socket].options ||
       host->nvram.mem.savedSetupData.socket[socket].ddrVddLimit != host->setup.mem.socket[socket].ddrVddLimit
     ) {
        result = TRUE;
  }

  return result;
} // SavedSetupDataChangedIMCChip


UINT32
DetectSocketPopulatedChangedChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Detect if the socket population has changed

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  return result;
} // DetectSocketPopulatedChangedChip

UINT32
DetectMCAErrorsChip (
  PSYSHOST host,
  UINT8 socket,
  UINT32 *logData
  )
/*++

  Detect MCA Errors

  @param host    - Pointer to the system host (root) structure
  @param socket  - socket id
  @param logData - pointer to 32-bit lag data

  @retval TRUE
  @retval FALSE

--*/
{
  UINT8  n;
  UINT32 result;
  MCA_ERR_SRC_LOG_PCU_FUN2_HSX_BDX_STRUCT errSrcLog;

  result = FALSE;

  //
  // Disable fast boot if there were any core errors on any socket
  //
  for (n = 0; n < MAX_SOCKET; n++) {
    if ((host->var.common.socketPresentBitMap & (BIT0 << n)) == 0) continue;

    //errSrcLog.Data = ReadCpuPciCfgEx (host, n, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
    errSrcLog.Data = 0;
    *logData = errSrcLog.Data;
    if (errSrcLog.Bits.mcerr || errSrcLog.Bits.ierr || errSrcLog.Bits.caterr) {
      result = TRUE;
      break;
    }
  } // socket loop

  return result;
} // DetectMCAErrorsChip


UINT32
DetectBootDateOldChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Detect Boot Date old

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;
  UINT16 bootDate;

  result = FALSE;

  //
  // Check if training data is old and needs re-training
  // Only check on First pass and if we are attempting Fast Boot.
  // Check for both Warm and Cold Boot
  //
  if ((host->var.common.bootMode == NormalBoot) &&
      ((host->setup.mem.options & ATTEMPT_FAST_BOOT) || (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD)) &&
      (host->var.mem.firstPass != 0) && (host->var.mem.dateCheckedOnFirstSocket != 1)) {

   host->var.mem.dateCheckedOnFirstSocket = 1;  //Indicates date to be checked only for the first socket;
    //If setupChanged == 1, then do a cold boot through memory from socket0; do not check the date for Socket1/2/3

    bootDate = MemGetCmosDate (host);
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Current Linear Date = %u days - Last Boot Date = %u days\n", bootDate, host->nvram.mem.lastBootDate));
    if ((host->setup.mem.enforceThreeMonthTimeout) && (bootDate > (host->nvram.mem.lastBootDate + MEM_MAX_COLD_BOOT_DAYS))) { //s4988567: option to disable 3 month timeout to re-train memory
      MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Training Data older than %u days. Forcing cold boot\n", MEM_MAX_COLD_BOOT_DAYS));
      result = TRUE;
    }
  }

  return result;
} // DetectBootDateOldChip


UINT64_STRUCT
GetPpinChip (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  Gets Processor PPINs

  @param host  - pointer to sysHost structure on stack

  @retval N/A
--*/

{
  UINT64_STRUCT msrReg;
  UINT64_STRUCT tmpReg;
  UINT64_STRUCT procPpin;

  procPpin.hi = 0x0;
  procPpin.lo = 0x0;
  rcPrintf ((host, "Get socket PPIN\n"));
  if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {
    msrReg = ReadMsrPipe(host, socket, MSR_PLATFORM_INFO);
    if((msrReg.lo & BIT23) == 0) {
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "Socket not PPIN Capable\n"));
#endif
      return procPpin; // CPU is not PPIN capable
    }
 
    //
    // Save PPIN_CTRL state
    //
    msrReg = ReadMsrPipe(host, socket, MSR_PPIN_CTL);
    tmpReg = msrReg;
//AptioV override start: If PPIN_CNTL is locked, then return.    
    if (msrReg.lo & BIT0) 
    {
	    return procPpin; // Unable to read the PPIN.
    }
//AptioV override end: If PPIN_CNTL is locked, then reurn.    
       
    //
    // Unlock and enable PPIN_CTRL
    //
    msrReg.lo = (msrReg.lo & 0xFFFFFFFC) | 0x2;
    WriteMsrPipe(host, socket, MSR_PPIN_CTL, msrReg);

    //
    // Get PPIN
    //
    procPpin = ReadMsrPipe(host, socket, MSR_PPIN);

    //
    // Restore PPIN_CTRL
    //
    WriteMsrPipe(host, socket, MSR_PPIN_CTL, tmpReg);
  }

  MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "PPIN Hi = 0x%x, PPIN Lo = 0x%x\n", procPpin.hi, procPpin.lo));

  return procPpin;
} // GetPpinChip

UINT32
DetectProcessorRemovedChip (
  PSYSHOST host
  )
/*++

  Initialize internal data structures

  @param host  - Pointer to the system host (root) structure

  @retval TRUE @retval FALSE

--*/
{
  UINT32 result;

  result = FALSE;
  if  (host->nvram.mem.socketBitMap != host->var.common.socketPresentBitMap) {
     result = TRUE;
  }

  return result;
} // DetectProcessorRemovedChip


UINT32
AttemptColdBootFastChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Attempt Cold Boot Fast

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32 result;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT biosScratchPad1;

  result = FALSE;

  biosScratchPad1.Data = GetStickyScratchpad (host, socket, 1);
    if ((biosScratchPad1.Data & BIT6) == 0) {
     result = TRUE;
  }

  return result;
} // AttemptColdBootFastChip


UINT32
GetDisableQRDimmChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Check if QR DIMM is disabled in HW

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32                          result;
  CAPID3_PCU_FUN3_BDX_STRUCT      capId3;

  result = FALSE;

  //
  // Get IMC Fuse data
  //
  capId3.Data = host->var.common.procCom[socket].capid3;

  //
  // Check if QR DIMM is disabled in HW
  //
  if (capId3.Bits.disable_qr_dimm) {
    result = TRUE;
  }

  return result;
} // GetDisableQRDimmChip


VOID
SetIMCEnabledChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Set IMC Enabled

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  host->nvram.mem.socket[socket].enabled = host->setup.mem.socket[socket].enabled;
  return;
} // SetIMCEnabledChip


UINT32
GetChnDisableChip (
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch
  )
/*++

  Check if channel disabled in HW

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id
  @param ch     - ch id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32                          result;
  CAPID3_PCU_FUN3_BDX_STRUCT      capId3;

  result = FALSE;

  //
  // Get IMC Fuse data
  //
  capId3.Data = host->var.common.procCom[socket].capid3;

  //
  // Check if channel disabled in HW
  //
  //
  // Indicate this channel is disabled if it is fused off
  //
  if (Is2HA(host)) {
    if (capId3.Bits.chn_disable & (1 << (ch & BIT0))) {
      result = TRUE;
    }
  } else {
    if (capId3.Bits.chn_disable & (1 << ch)) {
      result = TRUE;
    }
  }

  return result;
} // GetChnDisableChip


UINT32
GetDisable2dpcChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Check if 2 DPC is disabled in HW

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32                          result;
  CAPID3_PCU_FUN3_BDX_STRUCT      capId3;
  result = FALSE;

  //
  // Get IMC Fuse data
  //
  capId3.Data = host->var.common.procCom[socket].capid3;

  if (capId3.Bits.disable_2_dpc) {
    result = TRUE;
  }

  return result;
} // GetDisable2dpcChip


UINT32
GetDisable3dpcChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Check if 3 DPC is disabled in HW

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
{
  UINT32                          result;
  CAPID3_PCU_FUN3_BDX_STRUCT      capId3;

  result = FALSE;

  //
  // Get IMC Fuse data
  //
  capId3.Data = host->var.common.procCom[socket].capid3;

  //
  // Check if 3 DPC is disabled in HW
  //
  if (capId3.Bits.disable_3_dpc) {
    result = TRUE;
  }

  return result;
} // GetDisable3dpcChip


VOID
SetSteppingsChip (
  PSYSHOST host,
  UINT8 socket
  )
/*++

  Set Steppings

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval N/A

--*/
{

  //
  // Initialize global pointers to ODT tables (DDR3)
  //
  host->var.mem.odtValueTablePtr = (UINT32)&odtValueTable;
  host->var.mem.odtActTablePtr = (UINT32)&odtActTable;

  //
  // Check Cluster On Die setting
  //
  if (host->var.qpi.OutClusterOnDieEn) {
    host->var.mem.doHAInterleave = 0;
  } else {
    host->var.mem.doHAInterleave = 1;
  }

  return;
} // SetSteppingsChip
