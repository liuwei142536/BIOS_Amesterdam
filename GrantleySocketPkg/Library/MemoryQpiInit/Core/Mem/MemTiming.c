/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
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


                                      //   800   1000   1067   1200   1333   1400   1600   1800   1867   2000   2133  2200  2400  2600  2667  2800  2933  3000  3200
const UINT32  preciseTCK[MAX_SUP_FREQ] = {25000, 20000, 18750, 16667, 15000, 14286, 12500, 11111, 10714, 10000, 9375, 9091, 8333, 7692, 7500, 7143, 6818, 6667, 6250};
const UINT8   tCK[MAX_SUP_FREQ]       = { 20, 16, 15, 13, 12, 12, 10, 9, 9, 8, 8 ,8, 7, 7, 6, 6, 6, 6, 5 }; // This won't work right for 1867 and 2133
const UINT16  tRefiX9[MAX_SUP_FREQ]   = { 0x1B, 0x22, 0x24, 0x29, 0x2D, 0x2F, 0x36, 0x3D, 0x3F, 0x44, 0x49, 0x4B, 0x52, 0x59, 0x5B, 0x5F, 0x64, 0x66, 0x6D};
const UINT16  tRefi[MAX_SUP_FREQ]     = { 3120, 3900, 4161, 4679, 5198, 5459, 6240, 7019, 7281, 7800, 8318, 8579, 9359, 10139, 10401, 10919, 11438, 11699, 12480};
const UINT8   tCKE[MAX_SUP_FREQ]      = { 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8};
const UINT8   tCKE_DDR4[MAX_SUP_FREQ] = { 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8};
const UINT8   tWL[MAX_SUP_FREQ]       = { 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 11, 11, 11, 11, 11};
const UINT8   tWL_DDR4[MAX_SUP_FREQ]  = { 9, 9, 9, 9, 9, 11, 11, 11, 12, 12, 14, 14, 16, 16, 18, 18, 18, 18, 20};
const UINT8   tWLO[MAX_SUP_FREQ]      = { 4, 5, 5, 6, 6, 6, 6, 7, 8, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12};
const UINT8   tXPDLL[MAX_SUP_FREQ]    = { 10, 12, 13, 15, 16, 17, 20, 22, 23, 24, 26, 27, 29, 32, 32, 34, 36, 36, 39};
const UINT8   tXSOFFSET[MAX_SUP_FREQ] = { 4, 5, 6, 6, 7, 7, 8, 10, 10, 10, 11, 12, 13, 14, 14, 15, 15, 15, 16};
const UINT8   tXP[MAX_SUP_FREQ]       = { 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 9, 9, 9, 10};
const UINT8   tCKSRX[MAX_SUP_FREQ]    = { 4, 5, 6, 6, 7, 7, 8, 10, 10, 10, 11, 12, 13, 14, 14, 15, 15, 15, 16};
const UINT16  tSTAB[MAX_SUP_FREQ]     = { 2400, 3000, 3200, 3600, 4000, 4200, 4800, 5400, 5600, 6000, 6400, 6600, 7200, 7800, 8000, 8400, 8800, 9000, 9600};
const UINT16  tSTAB_DDR4[MAX_SUP_FREQ]= { 2000, 2500, 2668, 3000, 3333, 3500, 4000, 4500, 4668, 5000, 5333, 5500, 6000, 6500, 6668, 7000, 7333, 7500, 8000};
const UINT8   tMRD_DDR3               = 1;
const UINT8   tMRD_DDR4               = 4;
const UINT8   tPARRECOVERY[MAX_SUP_FREQ] = { 0, 0, 0, 0, 104, 105, 105, 122, 122, 138, 139, 156, 157, 157, 157, 158, 158, 158, 159};
const UINT8   tMOD[MAX_SUP_FREQ]      = { 12, 12, 12, 12, 12, 12, 12, 14, 15, 15, 16, 17, 18, 20, 20, 21, 22, 23, 24};
const UINT8   tMOD_DDR4               = 24;
const UINT8   tCCD_L[MAX_SUP_FREQ]    = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3};
const UINT8   tZQCS[MAX_SUP_FREQ]     = { 64, 64, 64, 64, 64, 64, 64, 72, 75, 80, 86, 88, 96, 104, 107, 112, 118, 120, 128 };

const UINT16  mdllTimer[MAX_SUP_FREQ] = { 200, 250, 267, 300, 334, 350, 400, 451, 468, 500, 534, 551, 600, 650, 667, 701, 734, 750, 800};
const UINT8   maxRefAllowed[MAX_DENSITY] = { 12, 9, 5, 3 };

const UINT16  tREFnStaggerLRDIMM[MAX_DENSITY - 1] [MAX_SUP_FREQ] = {
//    400MHz 533MHz 667MHz 800MHz 933MHz 1066Mhz
    { 0x20, 0x28, 0x2B, 0x30, 0x36, 0x38, 0x40, 0x48, 0x4B, 0x50, 0x56, 0x58, 0x60, 0x68, 0x6B, 0x70, 0x76, 0x78, 0x80 }, // 2Gbit Density
    { 0x3C, 0x4B, 0x50, 0x5A, 0x64, 0x69, 0x78, 0x87, 0x8C, 0x96, 0xA0, 0xA5, 0xB4, 0xC3, 0xC8, 0xD2, 0xDC, 0xE1, 0xF0 }, // 4Gbit Density
    { 0X46, 0x58, 0x5E, 0x69, 0x75, 0x7B, 0x8C, 0x9E, 0xA4, 0xAF, 0xBB, 0xC1, 0xD2, 0xE4, 0xEA, 0xF5, 0X101, 0x107, 0x118 }  // 8Gbit Density
 };

const UINT16  tREFnStaggerDDR4[MAX_DENSITY] [MAX_SUP_FREQ] = {
//     800   1000   1067   1200   1333   1400   1600   1800   1867   2000   2133   2200   2400   2600   2667   2800   2933   3000   3200
    { 0x022, 0x02A, 0x02D, 0x032, 0x038, 0x03B, 0x043, 0x04B, 0x04E, 0x054, 0x059, 0x05C, 0x064, 0x06D, 0x070, 0x075, 0x07B, 0x07D, 0x086 }, // 2Gbit Density
    { 0x02D, 0x038, 0x03C, 0x043, 0x04B, 0x04E, 0x059, 0x064, 0x068, 0x070, 0x077, 0x07B, 0x086, 0x091, 0x095, 0x09C, 0x0A3, 0x0A7, 0x0B2 }, // 4Gbit Density
    { 0x02F, 0x03A, 0x03E, 0x046, 0x04E, 0x052, 0x059, 0x069, 0x06D, 0x074, 0x07C, 0x080, 0x08C, 0x097, 0x09B, 0x0A3, 0x0AB, 0x0AE, 0x0BA }, // 8Gbit Density
    { 0x086, 0x0A7, 0x0B2, 0x0C8, 0x0DF, 0x0EA, 0x10B, 0x12C, 0x138, 0x14E, 0x164, 0x16F, 0x190, 0x1B2, 0x1BD, 0x1D3, 0x1E9, 0x1F4, 0x216 }  // 16Gbit Density
 };


const UINT16  tZQOPERAdder[MAX_DENSITY][MAX_SUP_FREQ - 4] = {
//    400MHz 533MHz 667MHz 800MHz 933MHz 1066Mhz
    { 0x0A4, 0x085, 0x067, 0x048, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }, // 1Gbit Density
    { 0x07C, 0x050, 0x024, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }, // 2Gbit Density
    { 0x00C, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }, // 4Gbit Density
    { 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000 }  // 8Gbit Density
 };

extern  UINT8   PiTicks;

//
// Local Prototypes
//
UINT16 GetMaxClkDiffDD(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode);
UINT8  Cas2RecEnGap(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode);
UINT8  Cas2DrvEnGap(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode);
void   EffectiveBusDelay(PSYSHOST host, UINT8 socket, UINT8 ch, UINT32 *effBusDelaySR, UINT32 *effBusDelayDR, UINT32 *effBusDelayDD);
UINT8  CkDiffGap(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode);

void
RestoreTimings (
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch
             )
/*++

  Restores trained timing values register

  @param host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number

  @retval N/A

--*/
{
  UINT8                                       dimm;
  UINT8                                       rank;
  UINT8                                       strobe;
  UINT8                                       ctlIndex;
  UINT8                                       clk;
  UINT8                                       bit;
  UINT8                                       maxBit = 4;
  INT16                                       zeroValue = 0;
  UINT16                                      cmdMinVal;
  UINT16                                      cmdMaxVal;
  UINT16                                      ctlMinVal;
  UINT16                                      ctlMaxVal;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  dimmNvList    = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Restore Timings\n"));

  SetRestoreTimings(host, socket, ch);

  //
  // Restore CMD timings
  //
  GetSetCmdGroupDelay (host, socket, ch, CmdAll, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &zeroValue, &cmdMinVal, &cmdMaxVal);

  //
  // Restore CTL timings
  //
  for (ctlIndex = 0; ctlIndex < NUM_CTL_PLATFORM_GROUPS; ctlIndex++) {
    GetSetCtlGroupDelay (host, socket, ch, ctlIndex + CtlGrp0, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &zeroValue, &ctlMinVal, &ctlMaxVal);
  }

  //
  // Restore CLK timings
  //
  for (clk = 0; clk < 4; clk++) {
    GetSetClkDelay (host, socket, ch, clk, GSM_FORCE_WRITE + GSM_WRITE_OFFSET, &zeroValue);
  }

  //
  // Restore all timings on fast boot.
  //
  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
        GetSetTxDelay (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
                       &zeroValue);
        GetSetRxDelay (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxDqsDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
                       &zeroValue);
        GetSetRxVref (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RxVref, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
                      &zeroValue);
        for (bit = 0; bit < maxBit; bit++) {
          if (DESKEW_DELAY == RxDqsBitDelay) {
            GetSetRxDelayBit (host, socket, ch, dimm, rank, strobe, bit, DdrLevel, RxDqsBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
              &zeroValue);
          } else {
            GetSetRxDelayBit (host, socket, ch, dimm, rank, strobe, bit, DdrLevel, RxDqsPBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
              &zeroValue);
            GetSetRxDelayBit (host, socket, ch, dimm, rank, strobe, bit, DdrLevel, RxDqsNBitDelay, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
              &zeroValue);
          }
        }
        CheckTxVrefSupport(host, socket, ch, dimm, rank, strobe, &zeroValue);
        WriteFastBootTimings(host, socket, ch, dimm, rank, strobe);
      } // strobe loop
    } // rank loop
  } // dimm loop

  //
  // Other Timing Parameters
  //
  RestoreTimingsChip (host, socket, ch);

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
               "tcothp = 0x%x, (*channelNvList)[ch].tcothp = 0x%x\n", MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG), (*channelNvList)[ch].tcothp));

}

UINT8
GetTccd (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Gets tCCD

  @param host    - Point to sysHost
  @param socket  - Socket numer

  @retval tCCD

--*/
{
  UINT8   tCCD = 4;

  if (host->nvram.mem.dramType != SPD_TYPE_DDR3) {

    // Enforce tCCD_L based on freq
    tCCD = tCCD_L[host->nvram.mem.socket[socket].ddrFreq] + 4;
  }

  return tCCD;

} // GetTccd


void
GetTTVar (
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
         )
/*++

  Gets the variables needed to calculate turnaround times

  @param host    - Point to sysHost
  @param socket  - Socket numer
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval N/A

--*/
{
  UINT32                        effBusDelaySR;
  UINT32                        effBusDelayDR;
  UINT32                        effBusDelayDD;
  RD_ODT_TBL2_MCDDC_CTL_STRUCT  rdOdtTiming;
  WR_ODT_TBL2_MCDDC_CTL_STRUCT  wrOdtTiming;


  //
  // Get the effective bus delays for same rank (SR), different rank same DIMM (DR) and different DIMM (DD)
  //
  EffectiveBusDelay(host, socket, ch, &effBusDelaySR, &effBusDelayDR, &effBusDelayDD);
  (*ttVars).effBusDelaySR = (UINT8)effBusDelaySR;
  (*ttVars).effBusDelayDR = (UINT8)effBusDelayDR;
  (*ttVars).effBusDelayDD = (UINT8)effBusDelayDD;

  (*ttVars).cas2RecEnDR = Cas2RecEnGap(host, socket, ch, DR_MODE);
  (*ttVars).cas2RecEnDD = Cas2RecEnGap(host, socket, ch, DD_MODE);

  (*ttVars).cas2DrvEnDR = Cas2DrvEnGap(host, socket, ch, DR_MODE);
  (*ttVars).cas2DrvEnDD = Cas2DrvEnGap(host, socket, ch, DD_MODE);

  GetOdtTiming(host, socket, ch, &rdOdtTiming.Data, &wrOdtTiming.Data);
  (*ttVars).odtRdLeading  = (UINT8)rdOdtTiming.Bits.extraleadingodt;
  (*ttVars).odtRdTrailing = (UINT8)rdOdtTiming.Bits.extratrailingodt;
  (*ttVars).odtWrLeading  = (UINT8)wrOdtTiming.Bits.extra_leading_odt;
  (*ttVars).odtWrTrailing = (UINT8)wrOdtTiming.Bits.extra_trailing_odt;

  (*ttVars).clkDiffDR = CkDiffGap(host, socket, ch, DR_MODE);
  (*ttVars).clkDiffDD = CkDiffGap(host, socket, ch, DD_MODE);
} // GetTTVar

UINT8
CalctWWDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         UINT8     minWWDR,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the write to write turnaround time for different ranks on the same DIMM

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tWWDR

--*/
{
  UINT8               tWWDR;
  DramSpecCalctWWDR(host, socket, ch, ttVars, &tWWDR);

  //
  // Check for min WWDR
  //
  if (tWWDR < minWWDR) {
    tWWDR = minWWDR;
  }

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tWWDR = %d, minWWDR = %d\n", tWWDR, minWWDR));
  //
  // s4928481: Cloned From SKX Si Bug Eco: TxDqPiClkSel changing through MCO corrupts TX FIFO pointers during PXC mode
  //
  EnforceWWDR(host, ch, &tWWDR);

  return tWWDR;
} // CalctWWDR

UINT8
CalctWWDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         UINT8     minWWDD,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the write to write turnaround time for different DIMMs

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tWWDD

--*/
{
  UINT8 tWWDD = 0;
  tWWDD = (*ttVars).cas2DrvEnDD;

  DramSpecCalctWWDD(host, socket, ch, ttVars, &tWWDD);

  //
  // Check for min WWDD
  //
  if (tWWDD < minWWDD) {
    tWWDD = minWWDD;
  }

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tWWDD = %d, minWWDD = %d\n", tWWDD, minWWDD));

  EnforceWWDD(host, ch, &tWWDD);
  return tWWDD;
} // CalctWWDD

UINT8
CalctRWSR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         UINT32    specMin,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the read to write turnaround time for the same rank

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval rRWSR

--*/
{
  UINT8                                 tRWSR;
  struct channelNvram                   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  tRWSR = (*ttVars).effBusDelaySR;

  DramSpecCalctRWSR(host, socket, ch, ttVars, &tRWSR);

  if (((*channelNvList)[ch].lrDimmPresent) && (tRWSR < specMin)) {
    tRWSR = (UINT8)specMin;
  }
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "specMin = %d, tRWSR = %d\n", specMin, tRWSR));
  return tRWSR;
} // CalctRWSR

UINT8
CalctRWDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         UINT32    specMin,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the read to write turnaround time for different ranks on the same DIMM

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tRWDR

--*/
{
  UINT8                                 tRWDR;
  struct channelNvram                   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  tRWDR = (*ttVars).effBusDelayDR;

  if (((*ttVars).odtRdTrailing > 1) || ((*ttVars).odtWrLeading > 1)) {
    tRWDR += 1;
  }

  DramSpecCalctRWDR(host, socket, ch, ttVars, &tRWDR);

  if (((*channelNvList)[ch].lrDimmPresent) && (tRWDR < specMin)) {
    tRWDR = (UINT8)specMin;
  }
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "specMin = %d, tRWDR = %d\n", specMin, tRWDR));

  return tRWDR;
} // CalctRWDR

UINT8
CalctRWDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the read to write turnaround time for different DIMMs

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tRWDD

--*/
{
  UINT8                                 tRWDD;

  tRWDD = (*ttVars).effBusDelayDD;

  DramSpecCalctRWDD(host, socket, ch, ttVars, &tRWDD);

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 " tRWDD = %d\n", tRWDD));

  return tRWDD;
} // CalctRWDD

UINT8
CalctWRDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the write to read turnaround time for different ranks on the same DIMM

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tWRDR

--*/
{
  UINT8 tWRDR;

  DramSpecCalctWRDR(host, socket, ch, ttVars, &tWRDR);
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tWRDR = %d\n", tWRDR));

  return tWRDR;
} // CalctWRDR

UINT8
CalctWRDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
         )
/*++

  This function calculates the write to read turnaround time for different DIMMs

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tWRDD

--*/
{
  UINT8 tWRDD;

  DramSpecCalctWRDD(host, socket, ch, ttVars, &tWRDD);

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tWRDD = %d\n", tWRDD));

  return tWRDD;
} // CalctWRDD

/*++

  CAS2RCVEN: The time from sending CAS command @ IO till RCV_EN is asserted per this command.
  It should be calculated per byte as
  CAS2RCVEN(rank, byte) = 1/2 * (round_trip_latency(rank) + RxRcvEnLogicDelay(rank, byte)
        - IO_latency(rank) + RcvPIsetting(rank, byte) / 64) - const
  The const is the latency from pads to MC. It doesn't affect the further calculations because these will be the differences
  of CAS2RCVEN of different ranks same byte.
  The gap between two data transfers is determined by two limits - drive period and ODT period. The limiting factor is
  different for different rank and for different DIMM.
  R2RDR_GAP: On different ranks same DIMM the ODT value doesn't change in the turnaround, so the limiters are derived from
  need to keep non-overlap between the two drivers. For this purpose 0.6 DCLK cycle is enough. The final equation is
            tRRDR-D0 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R0, byte) - CAS2RCVEN(R1, byte)| + 0.25
            tRRDR-D2 = max(Byte = 0 to 7, ECC) |CAS2RCVEN(R2, byte) - CAS2RCVEN(R3, byte)| + 0.25
            tRRDR = round-up (max (tRRDR-D0, tRRDR-D2)) -1

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param Mode    - Timing Mode:
                     SR_MODE
                     DR_MODE
                     DD_MODE

  @retval  gap

--*/
UINT8
Cas2RecEnGap(
            PSYSHOST  host,
            UINT8     socket,
            UINT8     ch,
            UINT8     mode
            )
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               logRank;
  UINT8               strobe;
  INT16               recEnPi;
  UINT16              cas2Rcven;
  UINT16              maxGap = 0;
  UINT16              strobeMin[MAX_STROBE];
  UINT16              strobeMax[MAX_STROBE];
  UINT16              gap;
  UINT16              gapDimm = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  gap = 0;

  if (mode == DD_MODE) {
    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      strobeMin[strobe] = 0xFFF;
      strobeMax[strobe] = 0;
    } // strobe loop
  }


  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    if (mode == DR_MODE) {
      gapDimm = 0;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        strobeMin[strobe] = 0xFFF;
        strobeMax[strobe] = 0;
      } // strobe loop
    }

#ifdef DEBUG_TURNAROUNDS
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\nCAS To Receive Enable Data %d DIMM %d\n", ch, dimm));
    if (mode == DD_MODE) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Different DIMM\n"));
    } else {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Different Rank\n"));
    }
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "           | rank 0 | rank 1 | rank 2 | rank 3\n", dimm));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "-----------------------------------------------\n"));
#endif

    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;


#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "Strobe %2d:", strobe));
#endif

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank(host, socket, ch, dimm, rank);
#ifdef YAM_ENV
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RecEnDelay,  GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &recEnPi);
#else
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &recEnPi);
#endif
        cas2Rcven = (GetRoundTrip (host, socket, ch, logRank) * MAX_PHASE_IN_FINE_ADJUSTMENT) -
                    (GetIOLatency (host, socket, ch, logRank) * MAX_PHASE_IN_FINE_ADJUSTMENT) + recEnPi;

#ifdef DEBUG_TURNAROUNDS
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " |  %3d ", cas2Rcven));
#endif

        UpdateMinMax(cas2Rcven, &strobeMin[strobe], &strobeMax[strobe]);
      } // rank loop

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));
#endif

    } // strobe loop

    if (mode == DR_MODE) {

      maxGap = 0;
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        if (maxGap < (strobeMax[strobe] - strobeMin[strobe])) {
          maxGap = strobeMax[strobe] - strobeMin[strobe];
        }
      } // strobe loop

      //
      // An extra DCLK is required if the diff is > 39 ticks
      //
      if (maxGap >= 39) {
        gapDimm = maxGap - 39;
      } else {
        gapDimm = 0;
      }

      //
      // Account for separation between commands (convert pi ticks to DCLK)
      //
      if ((gapDimm % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
        gapDimm = (gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
      } else {
        gapDimm = gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
      }
    }


#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                     "gapDimm = %d\n", gapDimm));
#endif

      if (gap < gapDimm) gap = gapDimm;
  } // dimm loop

  if (mode == DD_MODE) {
    //
    // Account for separation between commands (convert pi ticks to DCLK)
    //
    maxGap = 0;
    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      if (maxGap < (strobeMax[strobe] - strobeMin[strobe])) {
        maxGap = strobeMax[strobe] - strobeMin[strobe];
      }
    } // strobe loop

    CalcFinalGap(&gap, maxGap, gapDimm);
  }


#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                     "cas2RecEnGap = %d\n", gap));
#endif

  return(UINT8)gap;
} // Cas2RecEnGap

UINT8
Cas2DrvEnGap(
            PSYSHOST  host,
            UINT8     socket,
            UINT8     ch,
            UINT8     mode
            )
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               strobe;
  INT16               piDelay;
  UINT16              gap = 0;
  UINT16              gapDimm = 0;
  UINT16              cas2DrvEn;
  UINT16              maxGap = 0;
  UINT16              strobeMin[MAX_STROBE];
  UINT16              strobeMax[MAX_STROBE];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  if (mode == DD_MODE) {
    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      strobeMin[strobe] = 0xFFF;
      strobeMax[strobe] = 0;
    } // strobe loop
  }

  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

#ifdef DEBUG_TURNAROUNDS
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\nCAS To Drive Enable Data %d DIMM %d\n", ch, dimm));
    if (mode == DD_MODE) {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Different DIMM\n"));
    } else {
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Different Rank\n"));
    }
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "           | rank 0 | rank 1 | rank 2 | rank 3\n", dimm));
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "-----------------------------------------------\n"));
#endif

    if (mode == DR_MODE) {
      gapDimm = 0;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        strobeMin[strobe] = 0xFFF;
        strobeMax[strobe] = 0;
      } // strobe loop
    }


    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      //
      // Skip if this is an ECC strobe when ECC is disabled
      //
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "Strobe %2d:", strobe));
#endif

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
#ifdef YAM_ENV
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay,  GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &piDelay);
#else
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &piDelay);
#endif
        cas2DrvEn = piDelay;

#ifdef DEBUG_TURNAROUNDS
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " |  %3d ", cas2DrvEn));
#endif

        UpdateMinMax(cas2DrvEn, &strobeMin[strobe], &strobeMax[strobe]);
      } // rank loop

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "\n"));
#endif

    } // strobe loop
    if (mode == DR_MODE) {

      maxGap = 0;
      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        if (maxGap < (strobeMax[strobe] - strobeMin[strobe])) {
          maxGap = strobeMax[strobe] - strobeMin[strobe];
        }
      } // strobe loop

      if (maxGap >= 39) {
        gapDimm = maxGap - 39;
      } else {
        gapDimm = 0;
      }

      //
      // Account for separation between commands (convert pi ticks to DCLK)
      //
      if ((gapDimm % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
        gapDimm = (gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
      } else {
        gapDimm = gapDimm / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
      }
    }

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                     "gapDimm = %d\n", gapDimm));
#endif

      if (gap < gapDimm) gap = gapDimm;
  } // dimm loop

  if (mode == DD_MODE) {
    maxGap = 0;
    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      if (maxGap < (strobeMax[strobe] - strobeMin[strobe])) {
        maxGap = strobeMax[strobe] - strobeMin[strobe];
      }
    } // strobe loop

    if (maxGap >= 39) {
      maxGap = maxGap - 39;
    } else {
      maxGap = 0;
    }

    if ((maxGap % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
      gap = (maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
    } else {
      gap = maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
    }

  }

#ifdef DEBUG_TURNAROUNDS
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "cas2DrvEnGap = %d\n", gap));
#endif

  return(UINT8)gap;
} // Cas2DrvEnGap

void
EffectiveBusDelay(
                 PSYSHOST  host,
                 UINT8     socket,
                 UINT8     ch,
                 UINT32    *effBusDelaySR,
                 UINT32    *effBusDelayDR,
                 UINT32    *effBusDelayDD
                 )
/*++

  Effective Bus Delay

  @param host          - Point to sysHost
  @param socket        - Socket number
  @param ch            - Channel number
  @param effBusDelaySR - effective bus delay for same rank (SR)
  @param effBusDelayDR - effective bus delay for different rank
                         same DIMM (DR)
  @param effBusDelayDD - effective bus delay for different DIMM
                         (DD)

  @retval N/A

--*/
{
  UINT8                   dimm;
  UINT8                   rank;
  UINT8                   logRank;
  UINT8                   strobe;
  UINT8                   Nmode;
  INT16                   RcvEnPi;
  INT16                   TxDqsPi;
  UINT8                   preamble;
  INT16                   CwlAdj;
  UINT16                  IntDelta;
  UINT16                  RTL;
  UINT16                  CL;
  UINT32                  EffectiveBusRoundTrip[MAX_DIMM][MAX_RANK_CH][MAX_STROBE];
  UINT32                  EffBusRT = 0;
  UINT32                  TxDelay[MAX_DIMM][MAX_RANK_CH][MAX_STROBE];
  UINT32                  TxDelayTemp = 0;
  UINT32                  IOComp;
  UINT32                  IOlat;
  UINT32                  longPreambleEnable;
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  struct ddrRank          (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  *effBusDelaySR = 0;
  *effBusDelayDR = 0;
  *effBusDelayDD = 0;

  EffectiveBusDelayInit(host, socket, ch, &CwlAdj, &IOComp, &IntDelta, &longPreambleEnable);

  preamble    =  (UINT8)(PiTicks * (longPreambleEnable + 1));
  CL          = ((*channelNvList)[ch].common.nCL) * PiTicks * 2;
  IOlat       = ((*channelNvList)[ch].ioLatency0);
  Nmode       = ((*channelNvList)[ch].timingMode);

  if (Nmode > 0) {
    Nmode = Nmode - 1;
  }
  Nmode = Nmode * 2 * PiTicks;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {

    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    rankList = GetRankNvList(host, socket, ch, dimm);

    for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
      //
      // Get the logical rank #
      //
      logRank = GetLogicalRank(host, socket, ch, dimm, rank);

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe] = 0;
        TxDelay[dimm][(*rankList)[rank].rankIndex][strobe] = 0;
      } // strobe loop

      //
      // Get round trip latency for each channel
      //
      RTL = GetRoundTrip(host, socket, ch, logRank) * PiTicks;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

#ifdef YAM_ENV
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &RcvEnPi);
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &TxDqsPi);
#else
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, RecEnDelay, GSM_READ_ONLY, &RcvEnPi);
        GetSetDataGroup (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &TxDqsPi);
#endif
        EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe] = RTL - CL - (UINT16)IOComp - ((UINT16)PiTicks * ((IOlat >> (4 * logRank)) & 0xF)) - \
                                                                           Nmode + RcvEnPi + IntDelta + preamble;

#ifdef DEBUG_TURNAROUNDS
        //
        //  Debug Code Start
        //
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "**************************************************************\n", RTL));
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,"\n"));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " RTL - CL - IOComp - IOLat - Nmode + RcvEnPi + IntDelta + preamble\n"));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "    0x%X - 0x%X - 0x%X", RTL, CL, IOComp));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " - 0x%X",  ((UINT16)PiTicks*((IOlat >> (4*logRank)) & 0xF)) ));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " - 0x%X + 0x%X", Nmode, RcvEnPi));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " + 0x%X + 0x%X", IntDelta, preamble));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " = 0x%X\n", EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe]));
        //
        //  Debug Code End
        //
#endif //DEBUG_TURNAROUNDS

        TxDelay[dimm][(*rankList)[rank].rankIndex][strobe] = (UINT16)(TxDqsPi) + CwlAdj;// - (PiTicks / 2);

#ifdef DEBUG_TURNAROUNDS
        //
        //  Debug Code Start
        //
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " TxDqsPi + CwlAdj = TxDelay\n"));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "  0x%X", TxDqsPi));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " + 0x%X", CwlAdj));
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       " = 0x%X\n", TxDelay[dimm][(*rankList)[rank].rankIndex][strobe]));
        //
        //  Debug Code End
        //
#endif //DEBUG_TURNAROUNDS

      } // strobe loop
    } // rank loop
  } // dimm loop

  //
  // Calculate effective bus delay for same rank (SR)
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    rankList = GetRankNvList(host, socket, ch, dimm);

    for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      for (strobe = 0; strobe < MAX_STROBE; strobe++) {
        if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

        if ((EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe] -
             TxDelay[dimm][(*rankList)[rank].rankIndex][strobe]) > *effBusDelaySR) {

          *effBusDelaySR = EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe] -
                           TxDelay[dimm][(*rankList)[rank].rankIndex][strobe];

#ifdef DEBUG_TURNAROUNDS
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, strobe, NO_BIT,
                         "EffBusRT %d, TxDelayTemp %d, effBusDelaySR = %d\n",
                         EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe],
                         TxDelay[dimm][(*rankList)[rank].rankIndex][strobe], *effBusDelaySR));
#endif

        }
      } // strobe loop
    } // rank loop
  } // dimm loop

  //
  // Calculate effective bus delay for different rank same DIMM (DR)
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    for (strobe = 0; strobe < MAX_STROBE; strobe++) {
      if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);

      EffBusRT = 0;
      TxDelayTemp = 0xFFF;

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (EffBusRT < EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe]) {
          EffBusRT = EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe];
        }
        if (TxDelayTemp > TxDelay[dimm][(*rankList)[rank].rankIndex][strobe]) {
          TxDelayTemp = TxDelay[dimm][(*rankList)[rank].rankIndex][strobe];
        }

      } // rank loop

      if ((EffBusRT - TxDelayTemp) > *effBusDelayDR) {
        *effBusDelayDR = EffBusRT - TxDelayTemp;

#ifdef DEBUG_TURNAROUNDS
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, strobe, NO_BIT,
                       "EffBusRT %d, TxDelayTemp %d, effBusDelayDR = %d\n",
                       EffBusRT, TxDelayTemp, *effBusDelayDR));
#endif

      }
    } // strobe loop
  } // dimm loop

  //
  // Calculate effective bus delay for different DIMM (DD)
  //
  for (strobe = 0; strobe < MAX_STROBE; strobe++) {
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;

    EffBusRT = 0;
    TxDelayTemp = 0xFFF;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        if (EffBusRT < EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe]) {
          EffBusRT = EffectiveBusRoundTrip[dimm][(*rankList)[rank].rankIndex][strobe];
        }
        if (TxDelayTemp > TxDelay[dimm][(*rankList)[rank].rankIndex][strobe]) {
          TxDelayTemp = TxDelay[dimm][(*rankList)[rank].rankIndex][strobe];
        }
      } // rank loop
    } // dimm loop

    if ((EffBusRT - TxDelayTemp) > *effBusDelayDD) {
      *effBusDelayDD = EffBusRT - TxDelayTemp;

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, strobe, NO_BIT,
                     "EffBusRT %d, TxDelayTemp %d, effBusDelayDD = %d\n",
                     EffBusRT, TxDelayTemp, *effBusDelayDD));
#endif

    }
  } // strobe loop


#ifdef DEBUG_TURNAROUNDS
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "EffBusRT %d, TxDelayTemp %d, effBusDelayDD = %d\n",
                 EffBusRT, TxDelayTemp, *effBusDelayDD));
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "**************************************************************\n"));

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "effBusDelaySR = %d -> %d\n", *effBusDelaySR, (*effBusDelaySR + (2 * PiTicks) - 1) / (2 * PiTicks)));
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "effBusDelayDR = %d -> %d\n", *effBusDelayDR, (*effBusDelayDR + (2 * PiTicks) - 1) / (2 * PiTicks)));
  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "effBusDelayDD = %d -> %d\n", *effBusDelayDD, (*effBusDelayDD + (2 * PiTicks) - 1) / (2 * PiTicks)));
#endif

  *effBusDelaySR = (*effBusDelaySR + (2 * PiTicks) - 1) / (2 * PiTicks);
  *effBusDelayDR = (*effBusDelayDR + (2 * PiTicks) - 1) / (2 * PiTicks);
  *effBusDelayDD = (*effBusDelayDD + (2 * PiTicks) - 1) / (2 * PiTicks);

  return;
} // EffectiveBusDelay

UINT8
CkDiffGap(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         UINT8     mode
         )
/*++



  @param @param host        - Pointer to sysHost
  @param @param socket      - Socket number
  @param @param ch          - Channel number
  @param Mode               - Timing Mode:
                                SR_MODE
                                DR_MODE
                                DD_MODE

  @retval Maximum delay between clocks across DIMMs in Pi ticks

  --*/
{

  if (GetMaxClkDiffDD(host, socket, ch, mode) > 39) {
    return 1;
  } else {
    return 0;
  }
} // CkDiffGap

UINT16
GetMaxClkDiffDD(
               PSYSHOST  host,
               UINT8     socket,
               UINT8     ch,
               UINT8     mode
               )
/*++

  Finds the maximum clock difference between DIMMs in a channel

  @param host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number
  @param Mode        - Timing Mode:
                         SR_MODE
                         DR_MODE
                         DD_MODE

  @retval Maximum delay between clocks across DIMMs in Pi ticks

--*/
{
  UINT8               dimm;
  UINT8               rank;
  UINT8               clocksFound = 0;
  UINT8               clkIndex;
  INT16               clkDelay[MAX_RANK_CH];
  UINT16              diff = 0;
  UINT16              diffTemp = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  //
  // No need to ckeck for differences between DIMMs if there are not multiple DIMMs
  //
  if ((*channelNvList)[ch].maxDimm < 2) return 0;

  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    if (mode == DR_MODE) {
      clocksFound = 0;
    }

    rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
#ifdef YAM_ENV
      GetSetClkDelay (host, socket, ch, (*rankList)[rank].ckIndex,  GSM_READ_CSR | GSM_UPDATE_CACHE | GSM_READ_ONLY, &clkDelay[clocksFound]);
#else
      GetSetClkDelay (host, socket, ch, (*rankList)[rank].ckIndex, GSM_READ_ONLY, &clkDelay[clocksFound]);
#endif
      for (clkIndex = 0; clkIndex < clocksFound; clkIndex++) {
          if (clkDelay[clocksFound] > clkDelay[clkIndex]) {
            diffTemp = clkDelay[clocksFound] - clkDelay[clkIndex];
          } else {
            diffTemp = clkDelay[clkIndex] - clkDelay[clocksFound];
          }

          if (diffTemp > (128 - diffTemp)) {
            diffTemp = 128 - diffTemp;
          }

          if (diffTemp > diff) {
            diff = diffTemp;
          }
      } // clkIndex Loop

#ifdef DEBUG_TURNAROUNDS
      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                     "CLK %d Pi Delay = %d\n", (*rankList)[rank].ckIndex, clkDelay[clocksFound]));
#endif

      if (mode == DR_MODE) {

#ifdef DEBUG_TURNAROUNDS
        MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                       "diff = %d\n", diff));
#endif

      }
      clocksFound++;
    } // rank loop
  } // dimm loop


  if (mode == DD_MODE) {

#ifdef DEBUG_TURNAROUNDS
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Diff = %d\n", diff));
#endif

  }

  return diff;
} // GetMaxClkDiffDD

UINT8
CkPiDiffTable(
             PSYSHOST  host,
             UINT8     socket,
             UINT8     clkDiff
             )
/*++

  this function is conversion table. the function get the Ck diff value and retuen the TT time 2 or 3.

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param clkDiff - Clock difference

  @retval UINT8 table value conversion

--*/
{
  UINT8 TTvalue;
  UINT8 maxDiff[MAX_SUP_FREQ] = {MAX_DIFF_CK_PI_800, MAX_DIFF_CK_PI_1000, MAX_DIFF_CK_PI_1067, MAX_DIFF_CK_PI_1200,
    MAX_DIFF_CK_PI_1333, MAX_DIFF_CK_PI_1400, MAX_DIFF_CK_PI_1600, MAX_DIFF_CK_PI_1800,
    MAX_DIFF_CK_PI_1867, MAX_DIFF_CK_PI_2000, MAX_DIFF_CK_PI_2133, MAX_DIFF_CK_PI_2200,
    MAX_DIFF_CK_PI_2400, MAX_DIFF_CK_PI_2600, MAX_DIFF_CK_PI_2667, MAX_DIFF_CK_PI_2800,
    MAX_DIFF_CK_PI_2933, MAX_DIFF_CK_PI_3000, MAX_DIFF_CK_PI_3200};

  if (clkDiff > maxDiff[host->nvram.mem.socket[socket].ddrFreq]) {
    TTvalue = 3;
  } else {
    TTvalue = 2;
  }

  return TTvalue;
} // CkPiDiffTable

