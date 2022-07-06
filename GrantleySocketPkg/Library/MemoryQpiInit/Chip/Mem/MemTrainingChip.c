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

extern const UINT32 dataTrainFeedbackTable[MAX_STROBE];
extern const UINT32 rxVrefCtrlTable[MAX_STROBE];
extern const UINT32 rxOffsetTable[MAX_STROBE];

/**

  Quick centering of Vref

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs

  @retval SUCCESS

**/
UINT8
CmdVrefQuick (
  PSYSHOST  host,
  UINT8     socket,
  GSM_GT    group
  )
{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT16              mode;
  UINT32              patternLength;
  INT16               compLow   = 0;
  INT16               compHigh  = 0;
  INT16               worstLow  = 0;
  INT16               worstHigh = 0;
  INT16               offset[MAX_RANK_CH];
  struct bitMask      filter;
  struct rankMargin   resultsRank;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               rankPerChannel;
  INT16               final_offset[] = {0,0};

  channelNvList = GetChannelNvList(host, socket);
  patternLength = 64;
  mode = MODE_VA_DESELECT;

  // Setup bitMask
  for (ch = 0; ch < MAX_CH; ch++) {
    for (rankPerChannel = 0; rankPerChannel < MAX_RANK_CH; rankPerChannel++ ) {
      resultsRank.rank[ch][rankPerChannel].n = 0;
      resultsRank.rank[ch][rankPerChannel].p = 0;
      offset[rankPerChannel] = 0;
    }
  } // ch loop

  //
  // Clear the filter bits to enable error checking on every bit
  //
  ClearFilter(host, socket, &filter);

  GetMargins (host, socket, DdrLevel, group, mode, SCOPE_RANK, &filter, (void *)&resultsRank, patternLength, 0, 0, 0,
              WDB_BURST_LENGTH);

  for (ch = 0; ch < MAX_CH; ch++) {
    if (ch %2 == 0){
      worstLow = -0xFF;
        worstHigh = 0xFF;
    }
    if ((*channelNvList)[ch].enabled == 0) continue;
      compLow = -0xFF;
      compHigh = 0xFF;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      rankList = GetRankNvList(host, socket, ch, dimm);
      for (rank = 0; rank < (*channelNvList)[ch].dimmList[dimm].numRanks; rank++) {
        if ((*rankList)[rank].enabled == 0) continue;

        if (resultsRank.rank[ch][(*rankList)[rank].rankIndex].n > resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) {
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].n = 0;
          resultsRank.rank[ch][(*rankList)[rank].rankIndex].p = 0;
        }
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                      "High = %d - Low = %d\n",
                      resultsRank.rank[ch][(*rankList)[rank].rankIndex].p, resultsRank.rank[ch][(*rankList)[rank].rankIndex].n));

        if (compLow < resultsRank.rank[ch][(*rankList)[rank].rankIndex].n) {
          compLow = resultsRank.rank[ch][(*rankList)[rank].rankIndex].n;
        }
        if (compHigh > resultsRank.rank[ch][(*rankList)[rank].rankIndex].p) {
          compHigh = resultsRank.rank[ch][(*rankList)[rank].rankIndex].p;
        }
      } // rank loop
    } // dimm loop

    if (worstLow < compLow) {
      worstLow = compLow;
    }
    if (worstHigh > compHigh) {
      worstHigh = compHigh;
    }

    final_offset[ch/2] = (worstLow + worstHigh) / 2;
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Composite High = %d - Composite Low = %d final_offset = %d\n", worstHigh, worstLow, final_offset[ch/2] ));
  } // ch loop

  //
  // Reset the IO
  //
  IO_Reset (host, socket);
  JedecInitSequence (host, socket, CH_BITMASK);
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "(compHigh + compLow)/2 = %d\n", final_offset[ch/2]));


#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    getPrintFControl (host);
    rcPrintf ((host, "START_DATA_CMD_VREF_CENTERING\n"));
  }
#endif  // SERIAL_DBG_MSG

  // Apply new center
  for (ch = 0; ch < MAX_CH; ch ++) {
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Applying offset of %d...\n",final_offset[ch/2] ));
    GetSetCmdVref (host, socket, ch, 0, GSM_WRITE_OFFSET | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &final_offset[ch/2]);
  }

#ifdef SERIAL_DBG_MSG
  if (checkMsgLevel(host, SDBG_MAX)) {
    rcPrintf ((host, "STOP_DATA_CMD_VREF_CENTERING\n"));
    releasePrintFControl (host);
  }
#endif  // SERIAL_DBG_MSG

  return SUCCESS;
}

/**

  Find Index Resource Data

  @param strobe  - Strobe number (0-based)

  @retval Index

**/
UINT8
FindIndexResData (
                  UINT8 strobe
                 )
{
  return strobe & 0x03;
} //FindIndexResData

#if 0
void
InitCmdAddrParityErrorDetect (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     enablePeDetect
  )
/*++
  Collects the results of the previous test

  @param host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
--*/
{
  UINT32                              csrReg;
  struct  channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  //
  // Enable Command Address parity checking for RDIMM/LRDIMM
  //
  if (host->nvram.mem.dimmTypePresent == RDIMM) {

    csrReg = MemReadPciCfgEp (host, socket, ch, WR_CRDT_RTN_TH_MCDDC_DP_REG);
    if ((*channelNvList)[ch].enabled && enablePeDetect) {
      csrReg |= BIT5;
    }
    else {
      csrReg &= ~BIT5;
    }
    MemWritePciCfgEp (host, socket, ch, WR_CRDT_RTN_TH_MCDDC_DP_REG, csrReg);

  } // RDIMM
} // InitCmdAddrParityErrorDetect
#endif // 0

UINT32
SenseAmpOffset (
  PSYSHOST  host
)
/*++

  This function implements Sense Amp Offset training.
  SenseAmp/ODT offset cancellation
  Find the best “average” point for Vref Control
  Test Vref point with SampOffset=-7 and Test Vref Point with SampOffset=+7
  Find Vref on per ch/byte basis where -7 samples all 1 and +7 samples all 0

  @param host    - Pointer to sysHost

  @retval SUCCESS


--*/
{
  UINT8                                     socket;
  UINT8                                     ch;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     byte;
  UINT8                                     nibble;
  UINT8                                     bit;
  UINT8                                     i;
  UINT32                                    status;
  INT16                                     sumBits[MAX_CH][MAX_STROBE];
  INT16                                     FirstBestPoint[MAX_CH][MAX_STROBE];
  INT16                                     LastBestPoint[MAX_CH][MAX_STROBE];
  INT16                                     firstZero[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                     lastOne[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                     sampOffset;
  INT16                                     vref;
  INT16                                     VrefWidth;
  INT16                                     vrefStart;
  INT16                                     vrefMax;
  INT16                                     calVref;
  UINT8                                     HighMask[MAX_CH][MAX_STROBE];
  UINT8                                     LowMask[MAX_CH][MAX_STROBE];
  INT16                                     presavedvalue[MAX_CH][MAX_STROBE];
  UINT8                                     RxVrefArray[MAX_CH][MAX_STROBE / 2];
  UINT32                                    compData1Org[2];
  UINT32                                    ddrCrCompOvrOrg[2];
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                         (*rankStruct)[MAX_RANK_DIMM];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT          dataControl0;
  DATACONTROL3_0_MCIO_DDRIO_STRUCT          dataControl3;
  DATATRAINFEEDBACKN0_0_MCIO_DDRIO_STRUCT   dataTrainFeedback;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffset;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffsetTmp;
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT          rxVrefCtrl;
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT     ddrCRCompCtl0;
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT     ddrCRCompCtl0Org;
  DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_BDX_STRUCT  ddrCrCompOvr;
  COMPDATA1_0_MCIO_DDRIO_BDX_STRUCT         compData1[2];
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT      dataControl2;

#ifdef SERIAL_DBG_MSG
  UINT64_STRUCT             startTsc  = { 0, 0 };
  UINT64_STRUCT             endTsc = { 0, 0 };
#endif  // SERIAL_DBG_MSG

  if ((~host->memFlows & MF_SENSE_AMP_EN) || ((host->setup.mem.optionsExt & SENSE_EN) == 0)) return SUCCESS;

#ifdef SERIAL_DBG_MSG
  getPrintFControl(host);

  //
  // Start time
  //
  ReadTsc64 (&startTsc);
#endif // SERIAL_DBG_MSG

  socket = host->var.mem.currentSocket;
  channelNvList = GetChannelNvList(host, socket);
  status = 0;

  MemSetLocal ((UINT8 *) LastBestPoint, 0, sizeof (LastBestPoint));

  // init FirstBestPoint to -8, LoaMask to 0xff and HighMask to 0
  MemSetLocal ((UINT8 *) FirstBestPoint, (UINT32) (-8), sizeof (FirstBestPoint));
  MemSetLocal ((UINT8 *) LowMask, (UINT32 )(-1), sizeof(LowMask));
  MemSetLocal ((UINT8 *) HighMask, 0, sizeof (HighMask));
  MemSetLocal ((UINT8 *) sumBits, 0, sizeof (sumBits));

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    if (host->var.common.cpuType == CPU_HSX) {
      for (ch = 0; ch < MAX_CH; ch+=2) {
        ddrCrCompOvrOrg[ch/2] = ddrCrCompOvr.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG);
        ddrCrCompOvr.Bits.dqodtu = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);
        compData1Org[ch/2]  = compData1[ch/2].Data = MemReadPciCfgEp (host, socket, ch, DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG);
        compData1[ch/2].Bits.rcompodtup = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG, compData1[ch/2].Data);
      }  //ch Loop
    }
    //
    // Start a COMP cycle
    //
    DoComp (host, socket);

    //
    // Set Rtt Park to disabled
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = GetDimmNvList(host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankStruct  = GetRankStruct(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank or if it is mapped out
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

#ifdef  LRDIMM_SUPPORT
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && host->nvram.mem.socket[socket].lrDimmPresent) {
            WriteLrbuf(host, socket, ch, dimm, 0, DDR4_RTT_PARK_DIS, LRDIMM_F0, LRDIMM_BC02);
          } else
#endif
          {
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5 & ~DDR4_RTT_PRK_ODT_MASK, BANK5);
          }
        } // rank loop
      } // dimm loop
    } // ch loop
  } // DDR4 loop

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                             "Stage 1: Vref Offset Training\nPlot Of SumOfBits across Vref settings\n"));
  if (host->nvram.mem.eccEn) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "        VR SA   0  1  2  3  4  5  6  7  8\n"));
  } else {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "        VR SA   0  1  2  3  4  5  6  7\n"));
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    // Force RXAmp and Bias on -MUST use Per byte as preious DqControl2 values depended on byte number
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      dataControl2.Data             = (*channelNvList)[ch].dataControl2[byte];
      dataControl2.Bits.forcebiason = 1;
      dataControl2.Bits.forcerxon   = 1;
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
    }
    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;
      // Need data structure here to save off initial VREF settings to restore at end of test, I called it presavedvalue, but I know thats not perfect.
      GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_READ_ONLY, &presavedvalue[ch][nibble]);
    }
  } // ch loop

  // Sweep through vref settings and find point SampOffset of +/- 7 passes

  // Need to record initial VREF settings(vrefperbit) before this step because init code will setup VREF dependent on config

  // if DDR4 and 'A0', then calVREF = 31, lvmode=0x2(DDR3U)
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    calVref = 31;
  } else {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        // Force RXAmp and Bias on -MUST use Per byte as preious DqControl2 values depended on byte number
        for (byte = 0; byte < MAX_STROBE / 2; byte++) {
          if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

          dataControl2.Data               = (*channelNvList)[ch].dataControl2[byte];
          dataControl2.Bits.forcebiason   = 1;
          dataControl2.Bits.forcerxon     = 1;
          dataControl2.Bits.cttermination = 1;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
        }
      } // ch loop

      calVref = 31;

      ddrCRCompCtl0.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);
      ddrCRCompCtl0Org.Data = ddrCRCompCtl0.Data;

      ddrCRCompCtl0.Bits.lvmode = 2;

      // Time 2: lvmode = target value, lvmodevalide = 0
      ddrCRCompCtl0.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 3: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 4: lvmode = target value, lvmode valid =0
      ddrCRCompCtl0.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 5: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
    } else {
      if (host->var.common.cpuType == CPU_BDX) {
        calVref = 42;
      } else {
        calVref = 63;
      }
    }
  }

  vrefStart = calVref - 32;
  vrefMax = calVref + 32;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    //
    // Set the rank to rank 0
    //
    dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.readrfrank            = 0;
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
    } // byte loop
  } // ch loop

  for (vref = vrefStart; vref <= vrefMax; vref += 1) {

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        dataControl3.Data = (*channelNvList)[ch].dataControl3[byte];
        dataControl3.Bits.odtsegmentenable = 7;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL3_0_MCIO_DDRIO_REG), dataControl3.Data);

      } // byte loop
      for (nibble = 0; nibble < MAX_STROBE; nibble++) {
        if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

        GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, &vref);
      } // nibble Loop
    } // ch loop

    //
    // Program settings for Vref and SampOffset = +7
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = GetDimmNvList(host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = 19;
          rxOffset.Bits.saoffset1 = 19;
          rxOffset.Bits.saoffset2 = 19;
          rxOffset.Bits.saoffset3 = 19;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
      } // dimm loop
    } // ch loop

    //
    // To run test, enable Offset Cancel mode and Enable ODT
    // Check Results and Update variables.  Ideal result is all 0
    // Clear Offset Cancel mode at end test to enable writing RX_OffsetV
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      IO_Reset(host, socket);

      FixedDelay(host, 1);

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[byte]);
        sumBits[ch][byte] = 0 - CountBitsEqOne (dataTrainFeedback.Bits.datatrainfeedback);
        LowMask[ch][byte] &= (UINT8) dataTrainFeedback.Bits.datatrainfeedback;
      }

      dataControl0.Bits.readrfrd             = 0;
      dataControl0.Bits.senseamptrainingmode = 0;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop
    } // ch loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%3d 19 ", vref));

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " %2d", sumBits[ch][byte]));
      } // byte loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));
    } // ch loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          // Program settings for SampOffset = -7
          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = 11;
          rxOffset.Bits.saoffset1 = 11;
          rxOffset.Bits.saoffset2 = 11;
          rxOffset.Bits.saoffset3 = 11;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
      } // dimm loop
    } // ch loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop
    } // ch loop

    IO_Reset(host, socket);

    FixedDelay(host, 1);

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[byte]);
        sumBits[ch][byte] += CountBitsEqOne (dataTrainFeedback.Bits.datatrainfeedback);
        HighMask[ch][byte] |= (UINT8) dataTrainFeedback.Bits.datatrainfeedback;

        // Check if this point is better
        if (sumBits[ch][byte] > FirstBestPoint[ch][byte]) {
          FirstBestPoint[ch][byte] = sumBits[ch][byte];
          LastBestPoint[ch][byte]  = vref;
          RxVrefArray[ch][byte] = (UINT8)vref;
        } else if (sumBits[ch][byte] == FirstBestPoint[ch][byte]) {
          LastBestPoint[ch][byte]  = vref;
        }
        //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        //               "%d ", sumBits[ch][byte]));
      } // byte loop

      dataControl0.Data            = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd   = 0;
      dataControl0.Bits.readrfwr   = 0;
      dataControl0.Bits.readrfrank = 0;
      dataControl0.Bits.forceodton = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      //               "\n"));
    } // ch loop
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%3d 11 ", vref));
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " %2d", sumBits[ch][byte]));
      } // byte loop
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));
    } // ch loop

  } // vref loop

#ifdef SERIAL_DBG_MSG
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      // Exit with error if any bit did not change
      if (!(HighMask[ch][byte] ^ LowMask[ch][byte])) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "\nERROR! At least one bit with unexpected results for Chl %u Byte %u\n",
                      ch, byte));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "A '0' in the following BitMask value represents the failing Bit(s) 0x%x\n",
                      (HighMask[ch][byte] ^ LowMask[ch][byte])));
      }
    } // byte loop
  } // ch loop

#endif // SERIAL_DBG_MSG

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    //Display the selected Read Vref per byte
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Vref\t"));
    // Upate RxVref delay center
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      VrefWidth = (INT8) (LastBestPoint[ch][byte] - RxVrefArray[ch][byte]);
      vref = (INT8) (RxVrefArray[ch][byte] + (VrefWidth / 2));

      // Add 1 to Round Up and find the center
      if (vref < 0) {
        vref--;
      } else {
        vref++;
      }

      // step size for RxVref is about 7.8mv AND for RxVrefOffset is about 3.9mv
      RxVrefArray[ch][byte] = (UINT8)vref;

      for (i = 0; i < 2; i++) { // set both nibbles
        nibble = byte + (i * 9);
        GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, &vref);
      }  // i loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%2d ", (INT8)RxVrefArray[ch][byte]));
    } // byte loop
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\n"));
  } // ch loop

  //
  // Initialize firstZero and lastOne to 99. These are not valid values
  //
  //MemSetLocal ((UINT8 *) firstZero, 99, sizeof (firstZero));
  //MemSetLocal ((UINT8 *) lastOne, 99, sizeof (lastOne));
  for (ch = 0; ch < MAX_CH; ch++) {
    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      for (bit = 0; bit < 4; bit++) {
        firstZero[ch][nibble][bit] = 99;
        lastOne[ch][nibble][bit] = 99;
      } // bit loop
    } // nibble loop
  } // ch loop

#ifdef SERIAL_DBG_MSG
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n\nStage 2: SampOffset Training\n"));
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "           "));
  for (nibble = 0; nibble < MAX_STROBE; nibble++) {
    if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "%2d  ", nibble));
    if (nibble & BIT0) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    " "));
    }
  } // byte loop

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n       SA  "));
  for (byte = 0; byte < MAX_STROBE / 2; byte++) {
    if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "01230123 "));
  } // byte loop
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "\n"));

#endif // SERIAL_DBG_MSG

  for (sampOffset = 0; sampOffset <= 31; sampOffset++) {

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);
        //for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          // Program Offset and Propagate new value from RF
          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = sampOffset;
          rxOffset.Bits.saoffset1 = sampOffset;
          rxOffset.Bits.saoffset2 = sampOffset;
          rxOffset.Bits.saoffset3 = sampOffset;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
        //} // rank loop
      } // dimm loop
    } // ch loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      IO_Reset(host, socket);

      FixedDelay(host, 1);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%2d  ", sampOffset));
      for (nibble = 0; nibble < MAX_STROBE; nibble++) {
        if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[nibble]);

#ifdef RC_SIM_FEEDBACK
        dataTrainFeedback.Data = 0;
        for (bit = 0; bit < BITS_PER_NIBBLE; bit++) {
          if (sampOffset <= RCSimSenseAmp[nibble][bit]+ch){
            dataTrainFeedback.Data |= 1<<bit;
          } // if
        } // bit
#endif //RC_SIM_FEEDBACK
        //
        // The upper nibbles are in bits [7:4]
        if (nibble > 8) {
          dataTrainFeedback.Data = dataTrainFeedback.Data >> 4;
        }

        for (bit = 0; bit < 4; bit++) {
          if (dataTrainFeedback.Bits.datatrainfeedback & (1 << bit)) {
            lastOne[ch][nibble][bit] = sampOffset;
          } else {
            if (firstZero[ch][nibble][bit] == 99) {
              firstZero[ch][nibble][bit] = sampOffset;
            }
          }

          //Display in bits
          if ((1 << bit) & dataTrainFeedback.Bits.datatrainfeedback) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "1"));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "0"));
          }
        } // bit loop
        if (nibble < 9) {
          if (nibble & BIT0) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          " "));
          }
        } else {
          if (((host->nvram.mem.eccEn) && (nibble & BIT0)) || ((!host->nvram.mem.eccEn) && !(nibble & BIT0))) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                                     " "));
          }
        }
      } // nibble loop

      dataControl0.Bits.senseamptrainingmode = 0;
      dataControl0.Bits.readrfrd    = 0;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
    } // ch loop
  } // sampOffset loop
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nBitSAmp\n            bit:\t  0  1  2  3\n"));

  // Calculate and Program Offsets and display per bit SenseAmp Offset
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Nibble %d:\t", nibble));

      rxOffset.Data = 0;
      for (bit = 0; bit < 4; bit++) {
        // Find vref center, add 1 for Round Up
        vref = (firstZero[ch][nibble][bit] + lastOne[ch][nibble][bit]) / 2;

        // Check for saturation conditions
        // to make sure we are as close as possible to vdd/2 (750mv)
        if (firstZero[ch][nibble][bit] == 99) {
          vref = 31;
        }
        if (lastOne[ch][nibble][bit] == 99) {
          vref = 0;
        }

        switch(bit){
          case 0:
            rxOffset.Bits.saoffset0 = vref;
            break;
          case 1:
            rxOffset.Bits.saoffset1 = vref;
            break;
          case 2:
            rxOffset.Bits.saoffset2 = vref;
            break;
          case 3:
            rxOffset.Bits.saoffset3 = vref;
            break;
        } // switch

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", vref));

        if (bit == 3) {
          dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

            rankList = GetRankNvList(host, socket, ch, dimm);
            for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
              if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

              rxOffsetTmp.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
              rxOffsetTmp.Bits.saoffset0 = 0;
              rxOffsetTmp.Bits.saoffset1 = 0;
              rxOffsetTmp.Bits.saoffset2 = 0;
              rxOffsetTmp.Bits.saoffset3 = 0;
              rxOffsetTmp.Data |= rxOffset.Data;
              (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble] = rxOffsetTmp.Data;
              MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4),
                                (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble]);
            } // rank loop
          } // dimm loop

          rxOffset.Data = 0;
        } // if bit == 3
      } // bit loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
    } // nibble loop

    // Propagate delay values (without a read command)
    dataControl0.Data            = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.readrfrd   = 1;
    dataControl0.Bits.readrfwr   = 0;
    dataControl0.Bits.readrfrank = 0;
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
    } // byte loop

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));

    for (nibble = 0; nibble < MAX_STROBE; nibble++) { // set both nibbles
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

      rxVrefCtrl.Data = (UINT32)presavedvalue[ch][nibble];
      GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, (INT16 *)&rxVrefCtrl.Data);
    } // nibble loop
  } // ch loop

  // Clean up after test

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
      // Disable force Sense amp
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl2[byte]);

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL3_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl3[byte]);
    } // byte loop
  } // ch loop
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      // Time 2: lvmode = target value, lvmodevalide = 0
      ddrCRCompCtl0Org.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

      // Time 3: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0Org.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

      // Time 4: lvmode = target value, lvmode valid =0
      ddrCRCompCtl0Org.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

      // Time 5: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0Org.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
    }
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // Restore Rtt Park
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankStruct  = GetRankStruct(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank or if it is mapped out
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

#ifdef  LRDIMM_SUPPORT
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && host->nvram.mem.socket[socket].lrDimmPresent) {
            WriteLrbuf(host, socket, ch, dimm, 0, (*dimmNvList)[dimm].lrBuf_BC02, LRDIMM_F0, LRDIMM_BC02);
          } else
#endif
          {
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
          }
        } // rank loop
      } // dimm loop
    } // ch loop

    if (host->var.common.cpuType == CPU_HSX && host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      //
      // Restore DDRCRCompovr and data comp1
      //
      for (ch = 0; ch < MAX_CH; ch+=2) {
        MemWritePciCfgEp (host, socket, ch, DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG, compData1Org[ch/2]);
        MemWritePciCfgEp (host, socket, ch, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvrOrg[ch/2]);
      } // ch loop
    }

    //
    // Start a COMP cycle
    //
    DoComp (host, socket);
  }

  IO_Reset(host, socket);
#ifdef SERIAL_DBG_MSG
      //
      // End time
      //
      ReadTsc64 (&endTsc);
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "SenseAmpOffset - %dms\n", TimeDiff (host, startTsc, endTsc, TDIFF_UNIT_MS)));

  releasePrintFControl(host);
#endif // SERIAL_DBG_MSG

  return status;
}


UINT32
SenseAmpOffsetBdx(
  PSYSHOST  host
)
/*++

  This function implements Sense Amp Offset training.
  SenseAmp/ODT offset cancellation
  Find the best “average” point for Vref Control
  Test Vref point with SampOffset=-7 and Test Vref Point with SampOffset=+7
  Find Vref on per ch/byte basis where -7 samples all 1 and +7 samples all 0

  @param host    - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT8                                     socket;
  UINT8                                     ch;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     byte;
  UINT8                                     nibble;
  UINT8                                     bit;
  UINT8                                     i;
  UINT32                                    status;
  INT16                                     sumBits[MAX_CH][MAX_STROBE];
  INT16                                     FirstBestPoint[MAX_CH][MAX_STROBE];
  INT16                                     LastBestPoint[MAX_CH][MAX_STROBE];
  INT16                                     firstZero[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                     lastOne[MAX_CH][MAX_STROBE][MAX_BITS];
  INT16                                     sampOffset;
  INT16                                     vref;
  INT16                                     VrefWidth;
  INT16                                     vrefStart;
  INT16                                     vrefMax;
  INT16                                     calVref;
  UINT8                                     HighMask[MAX_CH][MAX_STROBE];
  UINT8                                     LowMask[MAX_CH][MAX_STROBE];
  INT16                                     presavedvalue[MAX_CH][MAX_STROBE];
  UINT8                                     RxVrefArray[MAX_CH][MAX_STROBE / 2];
  UINT32                                    compData1Org[2];
  UINT32                                    ddrCrCompOvrOrg[2];
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                         (*rankStruct)[MAX_RANK_DIMM];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT          dataControl0;
  DATATRAINFEEDBACKN0_0_MCIO_DDRIO_STRUCT   dataTrainFeedback;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffset;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffsetTmp;
  RXVREFCTRLN0_0_MCIO_DDRIO_STRUCT          rxVrefCtrl;
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT     ddrCRCompCtl0;
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT     ddrCRCompCtl0Org;
  DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_BDX_STRUCT  ddrCrCompOvr;
  COMPDATA1_0_MCIO_DDRIO_BDX_STRUCT         compData1[2];
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT      dataControl2;

#ifdef SERIAL_DBG_MSG
  UINT64_STRUCT             startTsc  = { 0, 0 };
  UINT64_STRUCT             endTsc = { 0, 0 };
#endif  // SERIAL_DBG_MSG

  if ((~host->memFlows & MF_SENSE_AMP_EN) || ((host->setup.mem.optionsExt & SENSE_EN) == 0)) return SUCCESS;

#ifdef SERIAL_DBG_MSG
  getPrintFControl(host);

  //
  // Start time
  //
  ReadTsc64 (&startTsc);
#endif // SERIAL_DBG_MSG

  socket = host->var.mem.currentSocket;
  channelNvList = GetChannelNvList(host, socket);
  status = 0;

  MemSetLocal ((UINT8 *) LastBestPoint, 0, sizeof (LastBestPoint));

  // init FirstBestPoint to -8, LoaMask to 0xff and HighMask to 0
  MemSetLocal ((UINT8 *) FirstBestPoint, (UINT32) (-8), sizeof (FirstBestPoint));
  MemSetLocal ((UINT8 *) LowMask, (UINT32 )(-1), sizeof(LowMask));
  MemSetLocal ((UINT8 *) HighMask, 0, sizeof (HighMask));
  MemSetLocal ((UINT8 *) sumBits, 0, sizeof (sumBits));

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      if (host->var.common.cpuType == CPU_HSX) {
        ddrCrCompOvrOrg[ch/2] = ddrCrCompOvr.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG);
        ddrCrCompOvr.Bits.dqodtu = 1;
        MemWritePciCfgEp (host, socket, (ch/2*2), DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvr.Data);
        compData1Org[ch/2]  = compData1[ch/2].Data = MemReadPciCfgEp (host, socket, ch, DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG);
        compData1[ch/2].Bits.rcompodtup = 0;
        MemWritePciCfgEp (host, socket, (ch/2*2), DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG, compData1[ch/2].Data);
      }
      //
      // Start a COMP cycle
      //
      DoComp (host, socket);

      //
      // Set Rtt Park to disabled
      //

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        //
        // Skip if no DIMM present
        //
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankStruct  = GetRankStruct(host, socket, ch, dimm);

        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          //
          // Skip if no rank or if it is mapped out
          //
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

#ifdef  LRDIMM_SUPPORT
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && host->nvram.mem.socket[socket].lrDimmPresent) {
            WriteLrbuf(host, socket, ch, dimm, 0, DDR4_RTT_PARK_DIS, LRDIMM_F0, LRDIMM_BC02);
          } else
#endif
          {
            WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5 & ~DDR4_RTT_PRK_ODT_MASK, BANK5);
          }
        } // rank loop
      } // dimm loop
    } // DDR4 loop

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                               "Stage 1: Vref Offset Training\nPlot Of SumOfBits across Vref settings\n"));
    if (host->nvram.mem.eccEn) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "        VR SA   0  1  2  3  4  5  6  7  8\n"));
    } else {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "        VR SA   0  1  2  3  4  5  6  7\n"));
    }
    // Force RXAmp and Bias on -MUST use Per byte as preious DqControl2 values depended on byte number
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      dataControl2.Data             = (*channelNvList)[ch].dataControl2[byte];
      dataControl2.Bits.forcebiason = 1;
      dataControl2.Bits.forcerxon   = 1;
      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
    }
    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;
      // Need data structure here to save off initial VREF settings to restore at end of test, I called it presavedvalue, but I know thats not perfect.
      GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_READ_ONLY, &presavedvalue[ch][nibble]);
    }

    // Sweep through vref settings and find point SampOffset of +/- 7 passes

    // Need to record initial VREF settings(vrefperbit) before this step because init code will setup VREF dependent on config

    // if DDR4 and 'A0', then calVREF = 31, lvmode=0x2(DDR3U)
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      calVref = 31;
    } else {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
        // Force RXAmp and Bias on -MUST use Per byte as preious DqControl2 values depended on byte number
        for (byte = 0; byte < MAX_STROBE / 2; byte++) {
          if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

          dataControl2.Data               = (*channelNvList)[ch].dataControl2[byte];
          dataControl2.Bits.forcebiason   = 1;
          dataControl2.Bits.forcerxon     = 1;
          dataControl2.Bits.cttermination = 1;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
        }

      calVref = 31;

      ddrCRCompCtl0.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);
      ddrCRCompCtl0Org.Data = ddrCRCompCtl0.Data;

      ddrCRCompCtl0.Bits.lvmode = 2;

      // Time 2: lvmode = target value, lvmodevalide = 0
      ddrCRCompCtl0.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 3: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 4: lvmode = target value, lvmode valid =0
      ddrCRCompCtl0.Bits.lvmodevalid  = 0;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);

      // Time 5: lvmode = target value, lvmodevalide = 1
      ddrCRCompCtl0.Bits.lvmodevalid  = 1;
      MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0.Data);
      } else {
        if (host->var.common.cpuType == CPU_BDX) {
          calVref = 42;
        } else {
          calVref = 63;
        }
      }
    }

    vrefStart = calVref - 32;
    vrefMax = calVref + 32;


    //
    // Set the rank to rank 0
    //
    dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.readrfrank            = 0;
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
    } // byte loop

    for (vref = vrefStart; vref <= vrefMax; vref += 1) {
      for (nibble = 0; nibble < MAX_STROBE; nibble++) {
        if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

        GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, &vref);
      } // nibble Loop

      //
      // Program settings for Vref and SampOffset = +7
      //

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = 19;
          rxOffset.Bits.saoffset1 = 19;
          rxOffset.Bits.saoffset2 = 19;
          rxOffset.Bits.saoffset3 = 19;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
      } // dimm loop

      //
      // To run test, enable Offset Cancel mode and Enable ODT
      // Check Results and Update variables.  Ideal result is all 0
      // Clear Offset Cancel mode at end test to enable writing RX_OffsetV
      //
      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      IO_Reset(host, socket);

      FixedDelay(host, 1);

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[byte]);
        sumBits[ch][byte] = 0 - CountBitsEqOne (dataTrainFeedback.Bits.datatrainfeedback);
        LowMask[ch][byte] &= (UINT8) dataTrainFeedback.Bits.datatrainfeedback;
      }

      dataControl0.Bits.readrfrd             = 0;
      dataControl0.Bits.senseamptrainingmode = 0;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%3d 19 ", vref));

      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " %2d", sumBits[ch][byte]));
      } // byte loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = GetRankNvList(host, socket, ch, dimm);
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          // Program settings for SampOffset = -7
          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = 11;
          rxOffset.Bits.saoffset1 = 11;
          rxOffset.Bits.saoffset2 = 11;
          rxOffset.Bits.saoffset3 = 11;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
      } // dimm loop

      if ((*channelNvList)[ch].enabled == 0) continue;

      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

    IO_Reset(host, socket);

    FixedDelay(host, 1);


      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[byte]);
        sumBits[ch][byte] += CountBitsEqOne (dataTrainFeedback.Bits.datatrainfeedback);
        HighMask[ch][byte] |= (UINT8) dataTrainFeedback.Bits.datatrainfeedback;

        // Check if this point is better
        if (sumBits[ch][byte] > FirstBestPoint[ch][byte]) {
          FirstBestPoint[ch][byte] = sumBits[ch][byte];
          LastBestPoint[ch][byte]  = vref;
          RxVrefArray[ch][byte] = (UINT8)vref;
        } else if (sumBits[ch][byte] == FirstBestPoint[ch][byte]) {
          LastBestPoint[ch][byte]  = vref;
        }
        //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        //               "%d ", sumBits[ch][byte]));
      } // byte loop

      dataControl0.Data            = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd   = 0;
      dataControl0.Bits.readrfwr   = 0;
      dataControl0.Bits.readrfrank = 0;
      dataControl0.Bits.forceodton = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      //               "\n"));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%3d 11 ", vref));
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " %2d", sumBits[ch][byte]));
      } // byte loop
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));

    } // vref loop

#ifdef SERIAL_DBG_MSG
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      // Exit with error if any bit did not change
      if (!(HighMask[ch][byte] ^ LowMask[ch][byte])) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "\nERROR! At least one bit with unexpected results for Chl %u Byte %u\n",
                      ch, byte));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "A '0' in the following BitMask value represents the failing Bit(s) 0x%x\n",
                      (HighMask[ch][byte] ^ LowMask[ch][byte])));
        DisableChannel(host, socket, ch);
        OutputWarning (host, WARN_SENS_AMP_TRAINING, WARN_SENS_AMP_CH_FAILIURE, socket, ch, 0xFF, 0xFF);
      }
    } // byte loop

#endif // SERIAL_DBG_MSG


    //Display the selected Read Vref per byte
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Vref\t"));
    // Upate RxVrefArray delay center
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      VrefWidth = (INT8) (LastBestPoint[ch][byte] - RxVrefArray[ch][byte]);
      vref = (INT8) (RxVrefArray[ch][byte] + (VrefWidth / 2));

      // Add 1 to Round Up and find the center
      if (vref < 0) {
        vref--;
      } else {
        vref++;
      }

      // step size for RxVrefArray is about 7.8mv AND for RxVrefOffset is about 3.9mv
      RxVrefArray[ch][byte] = (UINT8)vref;

      for (i = 0; i < 2; i++) { // set both nibbles
        nibble = byte + (i * 9);
        GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, &vref);
      }  // i loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%2d ", (INT8)RxVrefArray[ch][byte]));
    } // byte loop
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\n"));

  //
  // Initialize firstZero and lastOne to 99. These are not valid values
  //
  //MemSetLocal ((UINT8 *) firstZero, 99, sizeof (firstZero));
  //MemSetLocal ((UINT8 *) lastOne, 99, sizeof (lastOne));
    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      for (bit = 0; bit < 4; bit++) {
        firstZero[ch][nibble][bit] = 99;
        lastOne[ch][nibble][bit] = 99;
      } // bit loop
    } // nibble loop

#ifdef SERIAL_DBG_MSG
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n\nStage 2: SampOffset Training\n"));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "           "));
    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%2d  ", nibble));
      if (nibble & BIT0) {
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " "));
      }
    } // byte loop

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n       SA  "));
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "01230123 "));
    } // byte loop
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\n"));

#endif // SERIAL_DBG_MSG

    for (sampOffset = 0; sampOffset <= 31; sampOffset++) {

      // for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        // if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        dimm = 0;
        rankList = GetRankNvList(host, socket, ch, dimm);
        //for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
        //
        // This only needs to get set for rank 0
        //
        rank = 0;
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for (nibble = 0; nibble < MAX_STROBE; nibble++) {
          if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

          // Program Offset and Propagate new value from RF
          rxOffset.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
          rxOffset.Bits.saoffset0 = sampOffset;
          rxOffset.Bits.saoffset1 = sampOffset;
          rxOffset.Bits.saoffset2 = sampOffset;
          rxOffset.Bits.saoffset3 = sampOffset;

          MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
        } // nibble loop
        //} // rank loop
      // } // dimm loop


      // Propagate delay values (without a read command)
      dataControl0.Data                       = (*channelNvList)[ch].dataControl0;
      dataControl0.Bits.readrfrd              = 1;
      dataControl0.Bits.readrfwr              = 0;
      dataControl0.Bits.readrfrank            = 0;
      dataControl0.Bits.forceodton            = 1;
      dataControl0.Bits.senseamptrainingmode  = 1;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      IO_Reset(host, socket);

      FixedDelay(host, 1);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "%2d  ", sampOffset));
      for (nibble = 0; nibble < MAX_STROBE; nibble++) {
        if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

        dataTrainFeedback.Data = MemReadPciCfgEp (host, socket, ch, dataTrainFeedbackTable[nibble]);

        //
        // The upper nibbles are in bits [7:4]
        if (nibble > 8) {
          dataTrainFeedback.Data = dataTrainFeedback.Data >> 4;
        }

        for (bit = 0; bit < 4; bit++) {
          if (dataTrainFeedback.Bits.datatrainfeedback & (1 << bit)) {
            lastOne[ch][nibble][bit] = sampOffset;
          } else {
            if (firstZero[ch][nibble][bit] == 99) {
              firstZero[ch][nibble][bit] = sampOffset;
            }
          }

          //Display in bits
          if ((1 << bit) & dataTrainFeedback.Bits.datatrainfeedback) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "1"));
          } else {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "0"));
          }
        } // bit loop
        if (nibble < 9) {
          if (nibble & BIT0) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          " "));
          }
        } else {
          if (((host->nvram.mem.eccEn) && (nibble & BIT0)) || ((!host->nvram.mem.eccEn) && !(nibble & BIT0))) {
            MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                                     " "));
          }
        }
      } // nibble loop

      dataControl0.Bits.senseamptrainingmode = 0;
      dataControl0.Bits.readrfrd    = 0;
      for (byte = 0; byte < MAX_STROBE / 2; byte++) {
        if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // byte loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
    } // sampOffset loop
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\nBitSAmp\n            bit:\t  0  1  2  3\n"));

  // Calculate and Program Offsets and display per bit SenseAmp Offset

    for (nibble = 0; nibble < MAX_STROBE; nibble++) {
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Nibble %d:\t", nibble));

      rxOffset.Data = 0;
      for (bit = 0; bit < 4; bit++) {
        // Find vref center, add 1 for Round Up
        vref = (firstZero[ch][nibble][bit] + lastOne[ch][nibble][bit]) / 2;

        // Check for saturation conditions
        // to make sure we are as close as possible to vdd/2 (750mv)
        if (firstZero[ch][nibble][bit] == 99) {
          vref = 31;
        }
        if (lastOne[ch][nibble][bit] == 99) {
          vref = 0;
        }

        switch(bit){
          case 0:
            rxOffset.Bits.saoffset0 = vref;
            break;
          case 1:
            rxOffset.Bits.saoffset1 = vref;
            break;
          case 2:
            rxOffset.Bits.saoffset2 = vref;
            break;
          case 3:
            rxOffset.Bits.saoffset3 = vref;
            break;
        } // switch

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      " %2d", vref));

        if (bit == 3) {
          dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

          for (dimm = 0; dimm < MAX_DIMM; dimm++) {
            if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

            rankList = GetRankNvList(host, socket, ch, dimm);
            for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
              if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

              rxOffsetTmp.Data = (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble];
              rxOffsetTmp.Bits.saoffset0 = 0;
              rxOffsetTmp.Bits.saoffset1 = 0;
              rxOffsetTmp.Bits.saoffset2 = 0;
              rxOffsetTmp.Bits.saoffset3 = 0;
              rxOffsetTmp.Data |= rxOffset.Data;
              (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble] = rxOffsetTmp.Data;
              MemWritePciCfgEp (host, socket, ch, rxOffsetTable[nibble] + ((*rankList)[rank].logicalRank * 4),
                                (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][nibble]);
            } // rank loop
          } // dimm loop

          rxOffset.Data = 0;
        } // if bit == 3
      } // bit loop

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
    } // nibble loop

    // Propagate delay values (without a read command)
    dataControl0.Data            = (*channelNvList)[ch].dataControl0;
    dataControl0.Bits.readrfrd   = 1;
    dataControl0.Bits.readrfwr   = 0;
    dataControl0.Bits.readrfrank = 0;
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
    } // byte loop

    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));

    for (nibble = 0; nibble < MAX_STROBE; nibble++) { // set both nibbles
      if ((!host->nvram.mem.eccEn) && ((nibble == 8) || (nibble == 17))) continue;

      rxVrefCtrl.Data = (UINT32)presavedvalue[ch][nibble];
      GetSetDataGroup (host, socket, ch, 0, 0, nibble, ALL_BITS, DdrLevel, RxVref, GSM_UPDATE_CACHE, (INT16 *)&rxVrefCtrl.Data);
    } // nibble loop

  // Clean up after test

    // Disable force Sense amp
    for (byte = 0; byte < MAX_STROBE / 2; byte++) {
      if ((!host->nvram.mem.eccEn) && (byte == 8)) continue;

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL2_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl2[byte]);

      MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, byte, DATACONTROL3_0_MCIO_DDRIO_REG), (*channelNvList)[ch].dataControl3[byte]);
    } // byte loop
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "\n"));

    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
        // Time 2: lvmode = target value, lvmodevalide = 0
        ddrCRCompCtl0Org.Bits.lvmodevalid  = 0;
        MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
        MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

        // Time 3: lvmode = target value, lvmodevalide = 1
        ddrCRCompCtl0Org.Bits.lvmodevalid  = 1;
        MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
        MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

        // Time 4: lvmode = target value, lvmode valid =0
        ddrCRCompCtl0Org.Bits.lvmodevalid  = 0;
        MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
        MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);

        // Time 5: lvmode = target value, lvmodevalide = 1
        ddrCRCompCtl0Org.Bits.lvmodevalid  = 1;
        MemWritePciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
        MemWritePciCfgEp (host, socket, 2, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG, ddrCRCompCtl0Org.Data);
      }
    }

    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      //
      // Restore Rtt Park
      //

        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          rankStruct  = GetRankStruct(host, socket, ch, dimm);

          for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
            //
            // Skip if no rank 
            //
            if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

  #ifdef  LRDIMM_SUPPORT
            if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && host->nvram.mem.socket[socket].lrDimmPresent) {
              WriteLrbuf(host, socket, ch, dimm, 0, (*dimmNvList)[dimm].lrBuf_BC02, LRDIMM_F0, LRDIMM_BC02);
            } else
  #endif
            {
              WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
            }
          } // rank loop
        } // dimm loop

    if (host->var.common.cpuType == CPU_HSX && host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      //
      // Restore DDRCRCompovr and data comp1
      //
      MemWritePciCfgEp (host, socket, (ch/2*2), DDRCRDATACOMP1_MCIO_DDRIOEXTMCC_REG, compData1Org[ch/2]);
      MemWritePciCfgEp (host, socket, (ch/2*2), DDRCRCOMPOVR_MCIO_DDRIOEXTMCC_REG, ddrCrCompOvrOrg[ch/2]);
    }

      //
      // Start a COMP cycle
      //
      DoComp (host, socket);
    }

    IO_Reset(host, socket);
  }// ch loop
#ifdef SERIAL_DBG_MSG
  //
  // End time
  //
  ReadTsc64 (&endTsc);
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "SenseAmpOffset - %dms\n", TimeDiff (host, startTsc, endTsc, TDIFF_UNIT_MS)));

  releasePrintFControl(host);
#endif // SERIAL_DBG_MSG

  return status;
}

#define GRP_MUL_NONE  0   // Map one rank to each group
#define GRP_MUL_2_1   1   // Map two ranks to each group (0,2 and 1,3 to each group on QR)
#define GRP_MUL_FULL  2   // Map all DIMM ranks to one group (Not supported)
#define PPDS_GROUP_X4 BIT15 // Indicate that this group is x4 vs. x8

void MakeGroupToRankMap (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT16    groupToRankMap[MAX_PB_DESKEW_GROUP],
  UINT8     grpMul
  );
void CombineChRankDeskewData (
  PSYSHOST          host,
  UINT8             socket,
  UINT8             ch,
  UINT16            groupToRankMap[MAX_PB_DESKEW_GROUP],
  INT8              groupBitCenter[MAX_PB_DESKEW_GROUP][MAX_BITS],
  struct bitMargin  *resultsBit
  );
UINT8 GetPerBitDeskewGroup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT16    groupToRankMap[MAX_PB_DESKEW_GROUP],
  UINT8     logRank
  );


void MakeGroupToRankMap (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT16    groupToRankMap[MAX_PB_DESKEW_GROUP],
  UINT8     grpMul
  )
{
  UINT8               dimm, rank, logRank, group;
#ifdef SERIAL_DBG_MSG
  UINT8               sDBG = SDBG_MAX;
#endif
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);

  dimmNvList = GetDimmNvList(host, socket, ch);

  // First Init the RankMap Array
  for (group = 0; group < MAX_PB_DESKEW_GROUP; group++) {
    groupToRankMap[group] = 0;
  }

  // Set starting group
  group = 0;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      // Get the Logical rank #
      logRank = GetLogicalRank(host, socket, ch, dimm, rank);

      // Check if this DIMM is a x4, and indicated if so with BIT15 (MSB) set for later.
      //if ((*dimmNvList)[dimm].x4Present) groupToRankMap[group] |= BIT15;

      // if !LRDIMM but QuadRank and 2:1 group multiplier
      if ((!(*channelNvList)[ch].lrDimmPresent) && ((*dimmNvList)[dimm].numRanks == 4) && (grpMul == GRP_MUL_2_1)) {
        // Quad ranks are special in that ranks 0/2 are on the same side and ranks 1/3 are on the other. So group accordingly
        //
        // Check if Odd Rank
        if (rank & BIT0) {
          // just make sure that we have anough groups
          if (group < (MAX_PB_DESKEW_GROUP-1)) groupToRankMap[group+1] |= BIT0 << logRank;
        } else {
          groupToRankMap[group] |= BIT0 << logRank;
        }
      } else { // !lrDimmPresent && numRanks == 4 && 2:1 GrpMul (if QuadRank and 2:1)
        // Assign this rank to the current group
        groupToRankMap[group] |= BIT0 << logRank;
        // Only increment the group per rank if there is no grouping going on, so each rank gets its own group
        if (grpMul == GRP_MUL_NONE) {
          group++;
        }
      } // else clause from: if !lrDimmPresent && numRanks == 4 && 2:1 GrpMul (if QuadRank and 2:1)
    } // for Rank

    // Find next empty group - Use a find in case ranks are disabled leavin group empty.
    while ((groupToRankMap[group] != 0) && (group < MAX_PB_DESKEW_GROUP)) group++;
    // Check if we ran out of groups, then exit
    if (group >= MAX_PB_DESKEW_GROUP) continue;
  } // for DIMM

#ifdef SERIAL_DBG_MSG
  MemDebugPrint((host, sDBG, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Group to Rank Map\n"));
  for (group = 0; group < MAX_PB_DESKEW_GROUP; group++) {
    MemDebugPrint((host, sDBG, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Group %1d = 0x%04x\n", group, groupToRankMap[group]));
  }
#endif

  return;
} // MakeGroupToRankMap

UINT8 GetPerBitDeskewGroup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT16    groupToRankMap[MAX_PB_DESKEW_GROUP],
  UINT8     logRank
  )
{
  UINT8 group, rankMask;

  rankMask = BIT0 << logRank;
  for (group = 0; group < MAX_PB_DESKEW_GROUP; group++) {
    if (groupToRankMap[group] & rankMask) return (group);
  }

  MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "ERROR: Group not found in GroupToRankMap - logRank %1d\n", logRank));
  while (BIT0);
  return (0);
}

void CombineChRankDeskewData (
  PSYSHOST          host,
  UINT8             socket,
  UINT8             ch,
  UINT16            groupToRankMap[MAX_PB_DESKEW_GROUP],
  INT8              groupBitCenter[MAX_PB_DESKEW_GROUP][MAX_BITS],
  struct bitMargin  *resultsBit
  )
{
  UINT8   dimm, rank, bit, logRank, group, numGroupRanks;
  INT16   bitSkew;
  struct  ddrRank (*rankList)[MAX_RANK_DIMM];

  // Clear the groupBitCenter Array first
  for (bit = 0; bit < MAX_BITS; bit++) {
    for (group = 0; group < MAX_PB_DESKEW_GROUP; group++) {
      groupBitCenter[group][bit] = 0;
    }
  }

  for (bit = 0; bit < MAX_BITS; bit++) {
    // Skip bits 64-71 if ECC not enabled
    if ((!host->nvram.mem.eccEn) && (bit > 63)) continue;

    for (group = 0; group < MAX_PB_DESKEW_GROUP; group++) {
      numGroupRanks = 0;
      bitSkew = 0;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList(host, socket, ch, dimm);
        for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
          // Get the Logical rank #
          logRank = GetLogicalRank(host, socket, ch, dimm, rank);
          if (groupToRankMap[group] & (BIT0 << logRank)) {

            // Accumulate the skew from multiple ranks into bitSkew and make it * 10 for later rounding
            bitSkew += (((INT16) (resultsBit->bits[ch][(*rankList)[rank].rankIndex][bit].n +
                                  resultsBit->bits[ch][(*rankList)[rank].rankIndex][bit].p)) * 10) / 2;
            numGroupRanks++;
          } // if rank active in this group
        } // rank
      } // dimm

      // Get the average skew for this bit, but only if this group had any valid ranks and the skew is not 0 (Possible with good margins)
      if (numGroupRanks && bitSkew) {
        // Get the combined bitSkew from all the ranks in this group and divide by 10 to normalize the value
        groupBitCenter[group][bit] = (INT8) ((bitSkew / numGroupRanks) / 10);
      }
    } // For group
  } // for bit
} // CombineChRankDeskewData


/**

  This function applies RfOn setting to given channel.

  @param host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param rfOn    - rfOn setting to apply

@retval UINT32 - previous value

**/
UINT32
SetRfOn (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT32    rfOn
)
{
  struct channelNvram                   (*channelNvList)[MAX_CH];
  UINT8                                 strobe;
  UINT32                                retVal;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;

  channelNvList = GetChannelNvList(host, socket);

  dataControl0.Data = (*channelNvList)[ch].dataControl0;
  retVal = dataControl0.Bits.rfon;
  dataControl0.Bits.rfon = rfOn;
  for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
  } // strobe loop
  (*channelNvList)[ch].dataControl0 = dataControl0.Data;

  return retVal;
} // SetRfOn


/**

  This function applies the given CKE override

  @param host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param ckeMask    - CKE settings to apply

@retval none

**/
void
SetCkeOverride (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckeMask
)
{
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT  miscCKECtl;

  // Override CKE
  miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  miscCKECtl.Bits.cke_override = ckeMask;
  miscCKECtl.Bits.cke_on = ckeMask;
  MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);

} // SetCkeOverride

/**

  Check the DIMM Type

  @param host    - Pointer to sysHost

  @retval SUCCESS - DDR4
  @retval FAILURE - Anything except DDR4

**/
UINT32
CheckDimmType (
    PSYSHOST host
    )
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR4)
    return SUCCESS;
  return FAILURE;
}

/**

 Determine if we train a given rank for a given group

  @param host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param dimm    - Dimm
  @param rank    - Rank
  @param level   - DdrLevel or LrbufLevel
  @group         - Tx/RxVref, Tx/RxDq/Dqs, etc
  @retval        - 1 = skip, 0 = train

**/
UINT8
CheckSkipRankTrain(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  GSM_LT   level,
  GSM_GT   group
  )
{
  UINT8               status = 0;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if ((rank > 0) || (host->var.mem.currentSubRank > 0)){

#ifdef  LRDIMM_SUPPORT
    // only train rank0 for frontside TXVREF on LRDIMMs
    if (IsLrdimmPresent(host, socket, ch, dimm) && (!host->var.mem.runningRmt) &&
        (((group == TxVref) || (group == RxVref) || (group == TxDqDelay) ||
          (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqsDelay))) && (level == DdrLevel)) status = 1;

    //DDR4 LRDIMM backside
    if (IsLrdimmPresent(host, socket, ch, dimm) && (host->nvram.mem.dramType != SPD_TYPE_DDR3) && (level == LrbufLevel)) {
      // encoded mode or 3ds, skip ranks > 1
      if (((*channelNvList)[ch].encodedCSMode) &&
          (((host->var.mem.currentSubRank == 0) && (rank > 1)) ||
          ((host->var.mem.currentSubRank == 1) && (rank < 2)))) status = 1;
      // skip direct mapped mode if there are subranks
      if (((*channelNvList)[ch].encodedCSMode == 0) && (host->var.mem.currentSubRank == 1)) status = 1;
    }
#endif

    // only train rank0 for CMD training on RDIMMs
    if (host->nvram.mem.dimmTypePresent == RDIMM && CheckCMDGroup(group)
                && (!host->var.mem.runningRmt) && (level == DdrLevel)) status = 1;

  }
  // only do backside Cmd margining on Register Rev2
  if ((CheckCMDGroup(group) || CheckCTLGroup(group) || (group == CmdVref)) && (level == LrbufLevel)
      && !IsDdr4RegisterRev2(host, socket, ch, dimm)) status = 1;

 // restoreMsg (host);
 // if (status) MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT, "Skip = %d GROUP=%d \n", status, group));
 // disableAllMsg (host);
  return status;
}

/**

Routine Description:

  Checks to see if the TxDqDqs is too close to the zero encoding
  Adjusts TxDqDqs if this is the case for both directions

Arguments:

  host      - Pointer to sysHost
  socket    - current socket
  direction - DENORMALIZE (-1) or RENORMALIZE (1)

Returns:

**/

UINT32
WLPushOut(
  PSYSHOST  host,
  UINT8     socket
)

{
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  UINT8               adjust = 0;
  INT16               piDelay;
  UINT16              minDelay;
  UINT16              maxDelay;
  INT8                curCWL;
  struct channelNvram (*channelNvList)[MAX_CH];

  TCOTHP_MCDDC_CTL_STRUCT   tCOTHP;

  // Output current direction  Revise output statement
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "WriteLeveling Pushout\n"));

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  " BEFORE WL pushout values\n"));
#ifdef   SERIAL_DBG_MSG
  DisplayResults(host, socket, TxDqsDelay);
  DisplayResults(host, socket, TxDqDelay);
#endif

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    minDelay      = PITICKRANGE;
    maxDelay      = 0;
    adjust        = 0;

    // Get current CWL
    tCOTHP.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);
    curCWL = (INT8)tCOTHP.Bits.t_cwl_adj;
    if( curCWL == 4) {
                curCWL = -1;
    }

    // Read results from TxDqDqs
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        // If rank enabled
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        for(strobe = 0; strobe < MAX_STROBE; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
          GetSetDataGroup(host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY,
                         (INT16 *)&piDelay);
          UpdateMinMaxInt(piDelay, (INT16 *)&minDelay, (INT16 *)&maxDelay);

          GetSetDataGroup(host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_READ_ONLY,
                         (INT16 *)&piDelay);
          UpdateMinMaxInt(piDelay, (INT16 *)&minDelay, (INT16 *)&maxDelay);
        } // strobe loop
      } // rank loop
    } // dimm loop

    piDelay       = 0;
    //CWL by 1 or 2 clock and TX PI's by 128 or 256
    if ((curCWL > 0) && (maxDelay <= (PITICKRANGE-2*DCLKPITICKS-EXTRAPIMARGIN/2))) { //511-256=255  -2
      curCWL = curCWL - 2;
      piDelay = 2*DCLKPITICKS;
      adjust = 1;
    } else if ((curCWL > -1) && (maxDelay <= (PITICKRANGE-DCLKPITICKS-EXTRAPIMARGIN/2))) { //511-128 = 383  -3
      curCWL = curCWL - 1;
      piDelay = DCLKPITICKS;
      adjust = 1;
    }


    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Pushout to CWL_ADJ=%d and TXDq/Dqs by +%d\n",curCWL,piDelay));
    if (adjust == 1){
      // move CWL_ADJ
      if (curCWL < 0){
        tCOTHP.Bits.t_cwl_adj = 4;                                         // map to -1 for the negative case for CWL
        // tCOTHP2.Bits.t_cwl_adj_neg = ABS(curCWL);
      } else {
        tCOTHP.Bits.t_cwl_adj = ABS(curCWL);                               // leave at current adjusted CWL value
        // tCOTHP2.Bits.t_cwl_adj_neg = 0;
      }
      MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, tCOTHP.Data);


      // Write TxDqDqs per strobe
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          // If rank enabled
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          for(strobe = 0; strobe < MAX_STROBE; strobe++) {
            //
            // Skip if this is an ECC strobe when ECC is disabled
            //
            if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

            GetSetDataGroup(host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
                           (INT16 *)&piDelay);

            GetSetDataGroup(host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE,
                           (INT16 *)&piDelay);
          } // strobe loop
        } // rank loop
      } // dimm loop
    } // adjust True
  } // Ch loop

  disableAllMsg (host);
  IO_Reset (host, socket);
  JedecInitSequence (host, socket, CH_BITMASK);
  restoreMsg (host);
  return SUCCESS;
} // WLPushOut
