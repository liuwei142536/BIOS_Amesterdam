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
 * Copyright 2006 - 2019 Intel Corporation All Rights Reserved.
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


extern const UINT32 rxOffsetTable[MAX_STROBE];
extern  UINT32  preciseTCK[MAX_SUP_FREQ];
extern  UINT16  tRefiX9[MAX_SUP_FREQ];
extern  UINT16  tRefi[MAX_SUP_FREQ];
extern  UINT8   tCKE_DDR4[MAX_SUP_FREQ];
extern  UINT8   tCKE[MAX_SUP_FREQ];
extern  UINT8   tXPDLL[MAX_SUP_FREQ];
extern  UINT8   tXSOFFSET[MAX_SUP_FREQ];
extern  UINT8   tXP[MAX_SUP_FREQ];
extern  UINT8   tCKSRX[MAX_SUP_FREQ];
extern  UINT16  tSTAB[MAX_SUP_FREQ];
extern  UINT16  tSTAB_DDR4[MAX_SUP_FREQ];
extern  UINT8   tMRD_DDR3;
extern  UINT8   tMRD_DDR4;
extern  UINT8   tPARRECOVERY[MAX_SUP_FREQ];
extern  UINT8   tMOD[MAX_SUP_FREQ];
extern  UINT8   tMOD_DDR4;
extern  UINT8   tCCD_L[MAX_SUP_FREQ];
extern  UINT8   tZQCS[MAX_SUP_FREQ];
extern  UINT16  mdllTimer[MAX_SUP_FREQ];
extern  UINT8   maxRefAllowed[MAX_DENSITY];
extern  UINT16  tREFnStaggerDDR4[MAX_DENSITY] [MAX_SUP_FREQ];
extern  UINT16  tREFnStaggerLRDIMM[MAX_DENSITY - 1] [MAX_SUP_FREQ];
extern  UINT16  tZQOPERAdder[MAX_DENSITY][MAX_SUP_FREQ - 4];

const UINT16  tREFnStagger[MAX_DENSITY] [MAX_SUP_FREQ] = {
//    400MHz 533MHz 667MHz 800MHz 933MHz 1066Mhz
    { 0x022, 0x02A, 0x02D, 0x032, 0x038, 0x03B, 0x043, 0x04B, 0x04E, 0x054, 0x059, 0x05C, 0x064, 0x06D, 0x070, 0x075, 0x07B, 0x07D, 0x086 }, // 2Gbit Density
    { 0x02D, 0x038, 0x03C, 0x043, 0x04B, 0x04E, 0x059, 0x064, 0x068, 0x070, 0x077, 0x07B, 0x086, 0x091, 0x095, 0x09C, 0x0A3, 0x0A7, 0x0B2 }, // 4Gbit Density
    { 0x050, 0x064, 0x06B, 0x078, 0x086, 0x08C, 0x0A0, 0x0B4, 0x0BB, 0x0C8, 0x0D6, 0x0DC, 0x0F0, 0x104, 0x10B, 0x118, 0x126, 0x12C, 0x140 }, // 8Gbit Density
    { 0x086, 0x0A7, 0x0B2, 0x0C8, 0x0DF, 0x0EA, 0x10B, 0x12C, 0x138, 0x14E, 0x164, 0x16F, 0x190, 0x1B2, 0x1BD, 0x1D3, 0x1E9, 0x1F4, 0x216 }  // 16Gbit Density
 };

const UINT8   fastBoot = 2;
// LRDIMM debug
const UINT8   lrdimmExtraOdtDelay[MAX_SUP_FREQ - 4]   = { 2, 2, 3, 3, 5, 5, 6, 6, 7, 7, 7, 7, 7, 7, 7};

const UINT8   PiTicks  = 64;

// Local Prototypes
void   InitTurnAroundTimes(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, TCRWP_MCDDC_CTL_HSX_STRUCT *tcrwp, TCOTHP2_MCDDC_CTL_STRUCT *tcothp2, TCLRDP_MCDDC_CTL_STRUCT *tclrdp);
void   Sett_zqcs(PSYSHOST host, UINT8 socket, TCZQCAL_MCDDC_CTL_STRUCT *tczqcal);
void   SetRdimmTimingCntl(PSYSHOST host, RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT *RdimmTimingCntl);
void   PartialWrStarvationCounter(PSYSHOST host, UINT8 socket, UINT8 ch, PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_STRUCT *pmmStarvCntr);
#ifdef  LRDIMM_SUPPORT
void   TimeConstraintLrdimm(PSYSHOST host, UINT8 socket, TCRAP_MCDDC_CTL_STRUCT *tcrap, TCRWP_MCDDC_CTL_HSX_STRUCT tcrwp, TCDBP_MCDDC_CTL_HSX_STRUCT tcdbp);
#endif // LRDIMM_SUPPORT
void   Ddr3MrsTimingReg(PSYSHOST host, UINT8 socket, UINT8 ch, RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT RdimmTimingCntl);
UINT8  Sett_ckev(PSYSHOST host, RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT *RdimmTimingCntl2);
void   SaveRefreshRate(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 refreshRate);
void   SetEncodeCSMode(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm,
#ifdef  LRDIMM_SUPPORT
         UINT32 *lrdimmQrDimmBitMask,
#endif
         UINT8 *density, UINT16 *t_stagger_ref, void *ptcstagger);

void   SetStaggerRefEn(PSYSHOST host, UINT8 socket, UINT8 ch);
void   IdtLrbufWA(PSYSHOST host, UINT8 socket, UINT8 ch, WORK_AROUND_TYPE wa, void *data);
void   SetMiscDelay(PSYSHOST host, UINT8 socket, UINT8 ch, void *tcrap);
void   SetMasterDllWakeUpTimer(PSYSHOST host, UINT8 socket, UINT8 ch);

void
ProgramTimings (
               PSYSHOST host,
               UINT8    socket,
               UINT8    ch
               )
/*++

  Programs timing parameters

  @param host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number
  @param burstLength - Burst length

  @retval N/A

--*/
{
  UINT8                                       dimm;
  UINT8                                       density;
  UINT16                                      tREFI;
  UINT16                                      t_stagger_ref = 0;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  TCRFTP_MCDDC_CTL_STRUCT                     tcrftp;
  TCDBP_MCDDC_CTL_HSX_STRUCT                  tcdbp;
  TCRFP_MCDDC_CTL_HSX_STRUCT                  tcrfp;
  TCOTHP_MCDDC_CTL_HSX_STRUCT                 tcothp;
  TCSTAGGER_REF_MCDDC_CTL_HSX_STRUCT          tcstagger;
  TCRWP_MCDDC_CTL_HSX_STRUCT                  tcrwp;
  TCRAP_MCDDC_CTL_STRUCT                      tcrap;
  TCZQCAL_MCDDC_CTL_STRUCT                    tczqcal;
  IDLETIME_MCDDC_CTL_STRUCT                   IdleTime;
  RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT            RdimmTimingCntl;
  RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT           RdimmTimingCntl2;
  PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_STRUCT  pmmStarvCntr;
  WDBWM_MCDDC_CTL_STRUCT                      wdbwm;
  TCOTHP2_MCDDC_CTL_STRUCT                    tcothp2;
  TDDR4_MCDDC_CTL_STRUCT                      tddr4;
  WMM_READ_CONFIG_MCDDC_CTL_STRUCT            wmmReadConfig;
  UINT8                                       refreshRate;
  TCLRDP_MCDDC_CTL_STRUCT                     tclrdp;
#ifdef  LRDIMM_SUPPORT
  UINT32                                      lrdimmQrDimmBitMask = 0;
#endif

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef XMP_SUPPORT
  if ((host->setup.mem.options & MEM_OVERRIDE_EN) && host->setup.mem.inputMemTime.tREFI) {
    tREFI = host->setup.mem.inputMemTime.tREFI;
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Refresh rate = %d\n", host->setup.mem.inputMemTime.tREFI));
  } else
#endif // XMP_SUPPORT
  {
    //
    // Establish Refresh Rate
    // Units in 0.1x of standard 7.8 usec interval
    //
    if ((host->setup.mem.customRefreshRate >= 20) && (host->setup.mem.customRefreshRate <= 40)) {
      //
      // BIOS HSD 3617584: Add MRC option for Custom Refresh Rate between 2x and 4x
      //
      refreshRate = host->setup.mem.customRefreshRate;

    } else if (Check2xRefreshWA(host, socket, ch)){

      refreshRate = 20;

    } else {
      //
      // Default to legacy setting of 1x
      //
      refreshRate = 10;
    }

    //
    // Save Refresh Rate for later use
    //
    SaveRefreshRate(host, socket, ch, refreshRate);

    //  tREFI = tRefi[host->nvram.mem.ratioIndex];
    // Calculate tREFI
    if (refreshRate >= 20) {

      // Assume temp_lo = 0, so divide requested rate by 2
      tREFI = (UINT16) ((7800 * 20) / refreshRate * 10000 / preciseTCK[host->nvram.mem.ratioIndex]);

    } else {

      // Otherwise use 1x formula
      tREFI = (UINT16) ((7800 * 10) / refreshRate * 10000 / preciseTCK[host->nvram.mem.ratioIndex]);
    }
  }
  //
  // Decreasing refresh period by 2% to be safe
  //
  //tREFI                 = tREFI - (tREFI / 50);

  tcrftp.Data           = MemReadPciCfgEp (host, socket, ch, TCRFTP_MCDDC_CTL_REG);
  tcrftp.Bits.t_refi    = tREFI;
  tcrftp.Bits.t_refix9  = tRefiX9[host->nvram.mem.ratioIndex];
  tcrftp.Bits.t_rfc     = (*channelNvList)[ch].common.nRFC;

  //
  // HSD 5002633: W/A due to t_xsoffset size limitation
  //
  if (host->nvram.mem.socket[socket].ddrFreq >= DDR_3200) {
    tcrftp.Bits.t_rfc = tcrftp.Bits.t_rfc + 1;
  }
  MemWritePciCfgEp(host, socket, ch, TCRFTP_MCDDC_CTL_REG, tcrftp.Data);

  //
  // REF-REF separation
  //
  tcstagger.Data = MemReadPciCfgEp (host, socket, ch, TCSTAGGER_REF_MCDDC_CTL_REG);

  //
  // Defaults to 3
  //
  tcstagger.Bits.t_stagger_ref = 3;

  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
    if ((*dimmNvList)[dimm].numDramRanks < 2) continue; // Use default for single rank

    OutputExtendedCheckpoint((host, STS_GLOBAL_EARLY, SUB_PROG_TIMINGS, (UINT16)((socket << 8)|(ch << 4)|(dimm))));

    // Calculate refresh stagger for 2 or 4 ranks. single rank DIMMs default to T_STAGGER = 3
    // Formula is as follows:
    // (Time period (1us) + maxRefAllowed[density [1,2,4Gbits]]) / maxRefAllowed[density] * DCLK (666.666..) / 1000
    // (((1000+12)/12) * 666) / 1000 = 55 (should be 56 according to spreadsheet)

    // Get the dram Density from SPD DATA Byte 4 bits 0-3
    density = (*dimmNvList)[dimm].sdramCapacity;

    // Check density for boundaries
    if ((density < SPD_2Gb) || (density > SPD_16Gb)) {
      MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "ERROR: T_Stagger - DRAM density out of range: %d\n", density));
      density = SPD_4Gb; // force it to 4 for now to error on large t-stagger value
    }

    if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
      t_stagger_ref = (UINT16) tREFnStagger [density - SPD_2Gb][host->nvram.mem.socket[socket].ddrFreq];
    } else {
      t_stagger_ref = (UINT16) tREFnStaggerDDR4 [density - SPD_2Gb][host->nvram.mem.socket[socket].ddrFreq];
    }

    SetEncodeCSMode(host,
                    socket,
                    ch,
                    dimm,
#ifdef  LRDIMM_SUPPORT
                    &lrdimmQrDimmBitMask,
#endif
                    &density,
                    &t_stagger_ref,
                    (void*)(&tcstagger)
                    );
  } // dimm loop

#ifdef  LRDIMM_SUPPORT
  if ((*channelNvList)[ch].encodedCSMode == 1) {
    if ((host->var.common.cpuType == CPU_HSX) ||
        (IsBDXEP(host) && CheckSteppingLessThan( host, CPU_BDX, B0_REV_BDX))) {
      tcstagger.Bits.t_shrtloop_stagger_ref = 3;
      tcstagger.Bits.t_shrtloop_num = 1;
    }
  } else
#endif // LRDIMM_SUPPORT
  {
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      tcstagger.Bits.t_shrtloop_num = 3;
      tcstagger.Bits.t_stagger_ref = 10;
      tcstagger.Bits.t_shrtloop_stagger_ref = (*channelNvList)[ch].common.nRFC / 3;
      tcstagger.Bits.maxnum_active_reffsm = 0;
    }
  }
  //START HSX HSD 4987948 WA/5002499
#ifdef  LRDIMM_SUPPORT
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) &&
     ((*channelNvList)[ch].lrDimmPresent) &&
     ((*channelNvList)[ch].encodedCSMode == 1)) {
    tcstagger.Bits.t_shrtloop_stagger_ref = 0x64;
    tcstagger.Bits.t_shrtloop_num = 1;
  }
#endif // LRDIMM_SUPPORT
//END HSX HSD 4987948 WA

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "t_stagger_ref = 0x%x\n", tcstagger.Bits.t_stagger_ref));

  MemWritePciCfgEp (host, socket, ch, TCSTAGGER_REF_MCDDC_CTL_REG, tcstagger.Data);

  SetStaggerRefEn(host, socket, ch);

  //
  // Timing Contstraints DDR3 Bin Parameters
  //
  tcdbp.Data        = MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
  tcdbp.Bits.t_ras  = (*channelNvList)[ch].common.nRAS;
  tcdbp.Bits.t_cwl  = (*channelNvList)[ch].common.nWL;
  tcdbp.Bits.t_cl   = (*channelNvList)[ch].common.nCL;

  tcrwp.Data   = MemReadPciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG);
  tcothp2.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP2_MCDDC_CTL_REG);

  //
  // Initial turnaround times
  //
  tclrdp.Data  = MemReadPciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG);
  InitTurnAroundTimes(host, socket, ch, dimm, &tcrwp, &tcothp2, &tclrdp);

  tcdbp.Bits.t_rp   = (*channelNvList)[ch].common.nRP;
  tcdbp.Bits.t_rcd  = (*channelNvList)[ch].common.nRCD;
  MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);


  //
  // DDR4 Timing Parameters
  //
  if (host->nvram.mem.dramType != SPD_TYPE_DDR3) {
    tddr4.Data         = MemReadPciCfgEp (host, socket, ch, TDDR4_MCDDC_CTL_REG);
    if ((*channelNvList)[ch].common.nRRD_L > 7){
      tddr4.Bits.t_rrd_l = 7;
    } else {
      tddr4.Bits.t_rrd_l = (*channelNvList)[ch].common.nRRD_L;
    }
    tddr4.Bits.t_wtr_l = (*channelNvList)[ch].common.nWTR_L;
    tddr4.Bits.t_ccd_l = tCCD_L[host->nvram.mem.socket[socket].ddrFreq];
    tddr4.Bits.t_ccd_wr_l = tCCD_L[host->nvram.mem.socket[socket].ddrFreq];
    if (host->nvram.mem.WrCRC) {
      tddr4.Bits.t_ccd_wr_l += 1;
    }

    // cache t_ccd values
    (*channelNvList)[ch].common.tCCD      = tcrwp.Bits.t_ccd & 0xff;
    (*channelNvList)[ch].common.tCCD_WR   = tclrdp.Bits.t_ccd_wr & 0xff;
    (*channelNvList)[ch].common.tCCD_L    = tddr4.Bits.t_ccd_l & 0xff;
    (*channelNvList)[ch].common.tCCD_WR_L = tddr4.Bits.t_ccd_wr_l & 0xff;

  #ifdef LRDIMM_SUPPORT
    tddr4.Bits.qrlrdimm2cs = lrdimmQrDimmBitMask;
  #endif
    MemWritePciCfgEp (host, socket, ch, TDDR4_MCDDC_CTL_REG, tddr4.Data);
  }

  //
  // Timing Constraints Regular Access Parameters
  //
  tcrap.Data              = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
  tcrap.Bits.t_rrd        = (*channelNvList)[ch].common.nRRD;
  tcrap.Bits.t_rtp        = (*channelNvList)[ch].common.nRTP;
  if (host->nvram.mem.dramType != SPD_TYPE_DDR3) {
    tcrap.Bits.t_cke        = tCKE_DDR4[host->nvram.mem.ratioIndex];
  } else {
    tcrap.Bits.t_cke        = tCKE[host->nvram.mem.ratioIndex];
  }
  tcrap.Bits.t_wtr        = (*channelNvList)[ch].common.nWTR;
#ifdef LRDIMM_SUPPORT
  TimeConstraintLrdimm(host, socket, &tcrap, tcrwp, tcdbp);
#endif
  tcrap.Bits.t_faw        = (*channelNvList)[ch].common.nFAW;

  tcrap.Bits.t_wr = (*channelNvList)[ch].common.nWR & 0x1F; // Bit field limit for HSX
  if (host->var.common.cpuType == CPU_BDX) {
    tcrap.Bits.t_wr_upper   = ((*channelNvList)[ch].common.nWR & 0x20) >> 5;
  }

  // tPRPDen, tACTPDEN, and tREFPDEN needs to be 2 for 2133 and higher
  if (host->nvram.mem.socket[socket].ddrFreq >= DDR_2133) {
    tcrap.Bits.t_prpden = 2;
  }

  if (host->var.common.bootMode == S3Resume ||
      ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode != ColdBoot))) {
    tcrap.Bits.cmd_stretch  = (*channelNvList)[ch].timingMode;
  } else {
    // Set 3N timing for basic training
    (*channelNvList)[ch].trainTimingMode = TIMING_3N;
    tcrap.Bits.cmd_stretch = (*channelNvList)[ch].trainTimingMode;
  }

  IdtLrbufWA(host, socket, ch, t_cke, &tcrap);
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tcrap.Data);

  SetMiscDelay(host, socket, ch, (void*)&tcrap);

  //
  // Restore roundtrip, IO latencies, etc, for fast boot.
  //
  if (host->var.common.bootMode == S3Resume ||
      ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode != ColdBoot))) {
    if (host->var.mem.xoverModeVar == XOVER_MODE_1TO1) {
      RestoreTimings(host, socket, ch);
    }

  } else {
    tcothp.Data         = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);
    if (tXPDLL[host->nvram.mem.ratioIndex] > 0x1F) {
      tcothp.Bits.t_xpdll = 0x1F;
    } else {
      tcothp.Bits.t_xpdll = tXPDLL[host->nvram.mem.ratioIndex];
    }
    IdtLrbufWA(host, socket, ch, t_xp, &tcothp);
    IdtLrbufWA(host, socket, ch, t_odt_oe, NULL);

    MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, tcothp.Data);
    (*channelNvList)[ch].tcothp = tcothp.Data;
  }
  //
  // Timing Constraints DDR3 Refresh Timing Parameters
  //
  tcrfp.Data              = MemReadPciCfgEp (host, socket, ch, TCRFP_MCDDC_CTL_REG);
  tcrfp.Bits.orefni       = OREFNI;
  tcrfp.Bits.ref_hi_wm    = REF_HI_WM;
  tcrfp.Bits.ref_panic_wm = REF_PANIC_WM;
  MemWritePciCfgEp (host, socket, ch, TCRFP_MCDDC_CTL_REG, tcrfp.Data);

  //
  // Timing Constraints DDR3 Self-Refresh Timing Parameters
  //
  IdtLrbufWA(host, socket, ch, t_xsdll, NULL);

  //
  // Timing Constraints DDR3 ZQ Calibration Timing Parameters
  //
  tczqcal.Data            = MemReadPciCfgEp (host, socket, ch, TCZQCAL_MCDDC_CTL_REG);
  Sett_zqcs(host, socket, &tczqcal);
  tczqcal.Bits.zqcsperiod = ZQCS_PERIOD;
  MemWritePciCfgEp (host, socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcal.Data);

  //
  // Initial Page Policy and Timing Parameters
  //
  IdleTime.Data = MemReadPciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG);
  //HSD 5002509: CPGC Idle Time is set to 0x3F for 2400 to enable back-to-back writes
  if (host->nvram.mem.socket[socket].ddrFreq >= DDR_2400) {
    IdleTime.Bits.idle_page_rst_val = 0x3F;
  } else {
    IdleTime.Bits.idle_page_rst_val = IDLE_PAGE_RST_VAL;
  }
  IdleTime.Bits.win_size          = WIN_SIZE;
  IdleTime.Bits.ppc_th            = PPC_TH;
  IdleTime.Bits.opc_th            = OPC_TH;
  //
  // Set to open page mode for training
  //
  IdleTime.Bits.adapt_pg_clse = 0;

  MemWritePciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG, IdleTime.Data);

  RdimmTimingCntl.Data        = MemReadPciCfgEp (host, socket, ch, RDIMMTIMINGCNTL_MCDDC_CTL_REG);

  RdimmTimingCntl.Bits.t_stab = tCKSRX[host->nvram.mem.ratioIndex];

  if (host->nvram.mem.dimmTypePresent == RDIMM) {

    SetRdimmTimingCntl(host, &RdimmTimingCntl);
  } // if RDIMM

  //
  // RDIMM Timing Control 2
  //
  RdimmTimingCntl2.Data         = MemReadPciCfgEp (host, socket, ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG);
  if (host->nvram.mem.dimmTypePresent == RDIMM) {
    RdimmTimingCntl2.Bits.t_ckoff = 0x1F;
  } else {
    RdimmTimingCntl2.Bits.t_ckoff = RdimmTimingCntl.Bits.t_stab;
  }

  if (!Sett_ckev(host, &RdimmTimingCntl2)) {
    if (host->nvram.mem.socket[socket].ddrFreq <= DDR_2133) {
      RdimmTimingCntl2.Bits.t_ckev = 4;
    } else if (host->nvram.mem.socket[socket].ddrFreq <= DDR_2667){
      RdimmTimingCntl2.Bits.t_ckev = 6;
    } else {
      RdimmTimingCntl2.Bits.t_ckev = 8;
    }
  }

  MemWritePciCfgEp (host, socket, ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG, RdimmTimingCntl2.Data);
  MemWritePciCfgEp (host, socket, ch, RDIMMTIMINGCNTL_MCDDC_CTL_REG, RdimmTimingCntl.Data);

  Ddr3MrsTimingReg(host, socket, ch, RdimmTimingCntl);

  //
  // DDR3 Partial Write Starvation Counter
  //
  PartialWrStarvationCounter(host, socket, ch, &pmmStarvCntr);

  //
  // DDR3 MRS Timing Register
  //
  wdbwm.Data            = MemReadPciCfgEp (host, socket, ch, WDBWM_MCDDC_CTL_REG);
  if (host->setup.common.isocEn == 1){
    wdbwm.Bits.wmm_enter  = WMM_ENTRY;
    wdbwm.Bits.wmm_exit   = WMM_EXIT;
    wdbwm.Bits.starve_cnt = STARVATION_COUNTER;
  }
  else{
    wdbwm.Bits.wmm_enter  = PWMM_ENTRY;
    wdbwm.Bits.wmm_exit   = PWMM_EXIT;
    wdbwm.Bits.starve_cnt = PSTARVATION_COUNTER;
  }

  // Make sure WMMEnter is less than or equal to WPQ_IS - 1 for > A0.
  if (wdbwm.Bits.wmm_enter > (pmmStarvCntr.Bits.wpq_is - 1)) {
    wdbwm.Bits.wmm_enter = pmmStarvCntr.Bits.wpq_is - 1;
  }

  // Make sure WMMEnter is greater than or equal to 2.
  if (wdbwm.Bits.wmm_enter < 2) {
    wdbwm.Bits.wmm_enter = 2;
  }
  // Make sure WMMExit is greater than 0.  We will never exit WMM if it is set to 0.
  if (wdbwm.Bits.wmm_exit == 0) {
    wdbwm.Bits.wmm_exit = 1;
  }
  // Make sure WMMExit is less than WMMEnter.
  if (wdbwm.Bits.wmm_exit >= wdbwm.Bits.wmm_enter) {
    wdbwm.Bits.wmm_exit = wdbwm.Bits.wmm_enter - 1;
  }

  MemWritePciCfgEp (host, socket, ch, WDBWM_MCDDC_CTL_REG, wdbwm.Data);

  if (host->setup.mem.options & DISABLE_WMM_OPP_READ) {
    wmmReadConfig.Data = MemReadPciCfgEp (host, socket, ch, WMM_READ_CONFIG_MCDDC_CTL_REG);
    wmmReadConfig.Bits.dis_opp_rd = 1;
    MemWritePciCfgEp (host, socket, ch, WMM_READ_CONFIG_MCDDC_CTL_REG, wmmReadConfig.Data);
  }

  SetMasterDllWakeUpTimer(host, socket, ch);

  return ;
} // ProgramTimings


void
RestoreTimingsChip (
               PSYSHOST host,
               UINT8    socket,
               UINT8    ch
               )
/*++

  Restores other timing parameters

  @param host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number

  @retval N/A

--*/
{
  struct channelNvram                         (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, (*channelNvList)[ch].tcothp);
  MemWritePciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG, (*channelNvList)[ch].tclrdp);
  MemWritePciCfgEp (host, socket, ch, TCOTHP2_MCDDC_CTL_REG, (*channelNvList)[ch].tcothp2);
  MemWritePciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG, (*channelNvList)[ch].tcrwp);
  //MemWritePciCfgEp (host, socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, (*channelNvList)[ch].ddrtDimm0BasicTiming);
  //MemWritePciCfgEp (host, socket, ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG, (*channelNvList)[ch].ddrtDimm1BasicTiming);
} // RestoreTimingsChip


UINT8
Check2xRefreshWA (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  )
/*++

  Checks to see if any workarounds require 2x refresh

  @param host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

**/
{
  UINT8               dimm;
  UINT8               trrMode;
  UINT8               twoXRefresh = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  trrMode = DetectTRRSupport(host, socket, ch);

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "trrMode = %d\n", trrMode));
  if ((host->var.common.cpuType == CPU_HSX) ||( IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX))) { // WA reversed for BDX ML A0 and DE V0
    //
    // Workaround for BIOS HSD 4986027
    //
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      if ((trrMode & TRR_MODE_A) && (((*channelNvList)[ch].numRanks > 4) || ((*channelNvList)[ch].maxDimm > 2))) {
        twoXRefresh = 1;
      }
#ifdef  TRR_MODE_B_SUPPORT
      else if ((trrMode & TRR_MODE_B) && ((*channelNvList)[ch].numRanks > 4)) {
        twoXRefresh = 1;
      }
#endif // TRR_MODE_B_SUPPORT
    } // 4986027
  }

  // WorkAround for BIOS HSD 4168340
  if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX) && (twoXRefresh == 0)) {
    if (trrMode & TRR_MODE_A){
      for (dimm = 0; dimm < MAX_DIMM; dimm++){
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        if ((*dimmNvList)[dimm].SPDMMfgId == MFGID_SAMSUNG || (*dimmNvList)[dimm].SPDMMfgId == MFGID_HYNIX || (*dimmNvList)[dimm].SPDMMfgId == MFGID_ELPIDA){
          twoXRefresh = 1;
          break;
        }
      } // dimm loop
    } // if mode A
  } // if < C0

  //
  // Workaround for BIOS HSD 4986648: TRR mode is not supported for LRDIMMs
  //
  if (host->nvram.mem.socket[socket].lrDimmPresent) {
    if (trrMode & TRR_MODE_A) {
      twoXRefresh = 1;
    }
#ifdef  TRR_MODE_B_SUPPORT
    else if (trrMode & TRR_MODE_B) {
      twoXRefresh = 1;
    }
#endif // TRR_MODE_B_SUPPORT
  } // 4986648

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "twoXRefresh = %d\n", twoXRefresh));
  return twoXRefresh;
} // Check2xRefreshWA

void
SetMR0Shadow (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT32    MR0Data
  )
{
    //
    // Shadow MR0 but do not set DLL_RESET
    MemWritePciCfgEp (host, socket, ch, TCMR0SHADOW_MCDDC_CTL_REG, MR0Data & ~MR0_DLL);
    //
    // Save the value for S3 resume.
    //
    host->nvram.mem.socket[socket].channelList[ch].TCMr0Shadow = MR0Data & ~MR0_DLL;
}

void
SetMR2Shadow (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT32    MR2Data
  )
/*++

  Programs the MR2 shadow register

  @param host    - Pointer to sysHost
  @param socket    - Socket number
  @param ch      - Channel number
  @param dimm    - DIMM number
  @param MR2Data - MR2 data

  @retval N/A

--*/
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  TCMR2SHADOW_MCDDC_CTL_HSX_BDX_STRUCT TCMr2Shadow;

  dimmNvList        = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
  TCMr2Shadow.Data  = MemReadPciCfgEp (host, socket, ch, TCMR2SHADOW_MCDDC_CTL_REG);

  //
  // If inversion required
  //
  if ((host->nvram.mem.dimmTypePresent != UDIMM) && (host->nvram.mem.dimmTypePresent != SODIMM) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    TCMr2Shadow.Bits.two_mrs_en = 1;
  } else {
    TCMr2Shadow.Bits.two_mrs_en = 0;
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    //
    // Check if SRT is enabled
    //
    if (MR2Data & MR2_SRT) {
      //
      // Set if enabled
      //
      TCMr2Shadow.Bits.mr2_shdw_a7_srt |= 1 << dimm;
    } else {
      //
      // Clear if disabled
      //
      TCMr2Shadow.Bits.mr2_shdw_a7_srt &= ~(1 << dimm);
    }

    //
    // Check if ASR is enabled
    //
    if (MR2Data & MR2_ASR) {
      //
      // Set if enabled
      //
      TCMr2Shadow.Bits.mr2_shdw_a6_asr |= (1 << dimm);
    } else {
      //
      // Clear if disabled
      //
      TCMr2Shadow.Bits.mr2_shdw_a6_asr &= ~(1 << dimm);
    }
  }
  //
  // Are the address and bank mappings mirrored?
  //
  if ((*dimmNvList)[dimm].SPDAddrMapp) {
    //
    // Set if not swizzled
    //
    TCMr2Shadow.Bits.addr_bit_swizzle |= (1 << dimm);
  } else {
    //
    // Clear if not swizzled
    //
    TCMr2Shadow.Bits.addr_bit_swizzle &= ~(1 << dimm);
  }
  //
  // Shadow MR2 bits 15:8 to bits 23:16
  //
  TCMr2Shadow.Bits.mr2_shdw_a15to8 = (MR2Data >> 8) & 0x0FF;

  //
  // Shadow MR2 bits 5:0 to bits 5:0
  //
  TCMr2Shadow.Bits.mr2_shdw_a5to0 = MR2Data & 0x3F;

  MemWritePciCfgEp (host, socket, ch, TCMR2SHADOW_MCDDC_CTL_REG, TCMr2Shadow.Data);
  //
  // Save it to be restored on S3 resume.
  //
  host->nvram.mem.socket[socket].channelList[ch].TCMr2Shadow = TCMr2Shadow.Data;

} // SetMR2Shadow

void
SetMR4Shadow (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT32    MR4Data
  )
{
//    MemWritePciCfgEp (host, socket, ch, TCMR4SHADOW_MCDDC_CTL_REG, MR4Data);
    host->nvram.mem.socket[socket].channelList[ch].TCMr4Shadow = MR4Data;
}

void
SetMR5Shadow (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT32    MR5Data
  )
{
    MemWritePciCfgEp (host, socket, ch, TCMR5SHADOW_MCDDC_CTL_REG, MR5Data);
    host->nvram.mem.socket[socket].channelList[ch].TCMr5Shadow = MR5Data;
}


UINT32
TurnAroundTimings (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  Programs turnaround times

  @param host  - Point to sysHost
  @param socket  - Socket numer

  @retval SUCCESS

--*/
{
  UINT8                       ch;
  UINT8                       chA;
  UINT32                      status;
  UINT8                       ttA;
  UINT8                       tt;
  TT_VARS                     ttVars;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram         (*channelNvList)[MAX_CH];
  TCOTHP2_MCDDC_CTL_STRUCT    tcothp2;
  TCOTHP2_MCDDC_CTL_STRUCT    tcothp2A;
  TCOTHP_MCDDC_CTL_STRUCT     tcothp;
  TCOTHP_MCDDC_CTL_STRUCT     tcothpA;
  TCRWP_MCDDC_CTL_STRUCT      tcrwp;
  TCRWP_MCDDC_CTL_STRUCT      tcrwpA;
  TCLRDP_MCDDC_CTL_HSX_BDX_STRUCT tclrdp;
  TCLRDP_MCDDC_CTL_HSX_BDX_STRUCT tclrdpA;
  TCLRDP1_MCDDC_CTL_STRUCT    tclrdp1;
  TCLRDP1_MCDDC_CTL_STRUCT    tclrdp1A;
  UINT32                        specMin=0;
  UINT8                         dimm = 0;

#ifdef LRDIMM_SUPPORT
  UINT32    tPDM_RD=0;
  UINT32    tPDM_WR=0;
  UINT32    tWRPRE=0;
  UINT32    tRPRE=0;
  UINT32    BL=0;
  UINT32    tCK=0;

  tCK = preciseTCK[host->nvram.mem.ratioIndex];
  tPDM_RD = (167000000/tCK) +2500;   // unit is in tCKs
  tPDM_WR = (167000000/tCK) +2500;   // unit is in tCKs
  if (host->setup.mem.writePreamble >0) tWRPRE = 10000; // if enabled +10000
  if (host->setup.mem.readPreamble >0) tRPRE = 10000; // if enabled +10000
  BL = (((host->setup.mem.optionsExt & WR_CRC)/WR_CRC)*20000)+80000; // burst length is 8 or 10 (x10)
  specMin = (tPDM_RD +tPDM_WR +tWRPRE +tRPRE/2 +BL/2 + 27000 - 60000)/10000;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "(x10000) tck = %d, tPDM_RD=%d, tPDM_WR=%d, tWRPRE=%d, tRPRE=%d, BL=%d, specMin=%d\n",tCK,tPDM_RD,tPDM_WR,tWRPRE,tRPRE,BL,specMin));

#endif

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  status = SUCCESS;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    //
    // The following code is used to optimize turnaround times based on training results
    //
    tcrwp.Data = MemReadPciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG);
    tcothp.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);

    tcothp2.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP2_MCDDC_CTL_REG);
    tclrdp.Data  = MemReadPciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG);
    tclrdp1.Data  = MemReadPciCfgEp (host, socket, ch, TCLRDP1_MCDDC_CTL_REG);

    tcrwp.Bits.t_ccd = 0;

    if (((*channelNvList)[ch].lrRankMultEnabled) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
       tcrwp.Bits.t_ccd += LR_RANK_MULT_TA;
    }

    if (((*channelNvList)[ch].maxDimm > 2) && ((*channelNvList)[ch].lrDimmPresent) && ((*channelNvList)[ch].encodedCSMode == 2)) {
      tcrwp.Bits.t_ccd += LR_RANK_MULT_TA;
      (*channelNvList)[ch].common.tCCD = tcrwp.Bits.t_ccd & 0xff;
    }
    //
    // Get turnaround time variables
    //
    GetTTVar(host, socket, ch, &ttVars);

    //
    // BIOS HSD 4166843 - Mixing x4 and x8 might cut off last strobe so skip optimizing if mixed
    //
    tcrwp.Bits.t_rrdr = CalctRRDR(host, socket, ch, &ttVars);
    tcrwp.Bits.t_rrdd = CalctRRDD(host, socket, ch, &ttVars);

    tcrwp.Bits.t_wwdr = CalctWWDR(host, socket, ch, MIN_WWDR, &ttVars);
    tcrwp.Bits.t_wwdd = CalctWWDD(host, socket, ch, MIN_WWDD, &ttVars);
    tcothp2.Bits.t_rwsr = CalctRWSR(host, socket, ch, specMin, &ttVars);
    tcothp2.Bits.t_rwdr = CalctRWDR(host, socket, ch, specMin, &ttVars);
    if ((((*channelNvList)[ch].encodedCSMode) || ((*channelNvList)[ch].lrRankMultEnabled)) &&
        (tcothp2.Bits.t_rwsr < tcothp2.Bits.t_rwdr)) {
      tcothp2.Bits.t_rwsr = tcothp2.Bits.t_rwdr;
    }

    tcothp2.Bits.t_rwdd = CalctRWDD(host, socket, ch, &ttVars);
    tcrwp.Bits.t_wrdr = CalctWRDR(host, socket, ch, &ttVars);
    tcothp2.Bits.t_wrdd = CalctWRDD(host, socket, ch, &ttVars);

    //
	// Sighting 5003423: SODIMM Rawcard-H t_rrdd/t_rrdr +1.
	//
    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
      if ((((*dimmNvList)[dimm].SPDRawCard & 0x1F) == RAW_CARD_H)&&(((*dimmNvList)[dimm].actKeyByte2 & 0x0F) == SPD_SODIMM)){
        tcrwp.Bits.t_rrdr = tcrwp.Bits.t_rrdr + 1;
        tcrwp.Bits.t_rrdd = tcrwp.Bits.t_rrdd + 1;
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "SODIMM Rawcard H is detected and add the t_rrdr and t_rrdd by 1 \n"));

        break;
      }
    }
    MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "t_rrdr = %d, t_rrdd = %d\n", tcrwp.Bits.t_rrdr, tcrwp.Bits.t_rrdd));
    tclrdp.Bits.t_rrds = tcrwp.Bits.t_rrdr;
    tclrdp.Bits.t_wwds = tcrwp.Bits.t_wwdr;
    tclrdp.Bits.t_rwds = tcothp2.Bits.t_rwdr;
    tclrdp.Bits.t_wrds = tcrwp.Bits.t_wrdr;
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      // t_rwds = CL - CWL + RBL/2 + 1tCK + tWPRE
      tclrdp.Bits.t_rwds = (*channelNvList)[ch].common.nCL - (*channelNvList)[ch].common.nWL + 4 + 1;
      //tclrdp.Bits.t_rwds = 7;
      // t_wrds = CWL + WBL/2 + tWTR_L
      tclrdp.Bits.t_wrds = (*channelNvList)[ch].common.nWL + 4 + (*channelNvList)[ch].common.nWTR_L;
      //tclrdp.Bits.t_wrds = 0x12;
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "t_rwds = %d, t_wrds = %d\n", tclrdp.Bits.t_rwds, tclrdp.Bits.t_wrds));
    }
    tclrdp.Bits.t_ccd_wr = tcrwp.Bits.t_ccd;
    if (host->nvram.mem.WrCRC) {
      tclrdp.Bits.t_ccd_wr += 1;
    }
    (*channelNvList)[ch].common.tCCD_WR   = tclrdp.Bits.t_ccd_wr & 0xff;

    tclrdp1.Bits.t_rr_l_ds = tcrwp.Bits.t_rrdr;
    tclrdp1.Bits.t_ww_l_ds = tcrwp.Bits.t_wwdr;

    if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
      //
      // Syncronize turnaround timings
      //
      if ((ch == 1) || (ch == 3)) {
        chA = ch - 1;
        if(host->nvram.mem.socket[socket].channelList[chA].enabled == 0) continue;

        tcrwpA.Data = (*channelNvList)[chA].tcrwp;
        tcothpA.Data = (*channelNvList)[chA].tcothp;
        tcothp2A.Data = (*channelNvList)[chA].tcothp2;
        tclrdpA.Data  = (*channelNvList)[chA].tclrdp;
        tclrdp1A.Data  = (*channelNvList)[chA].tclrdp1;

        ttA = (UINT8)tcrwpA.Bits.t_ccd;
        tt = (UINT8)tcrwp.Bits.t_ccd;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_ccd = (UINT32)ttA;
        tcrwp.Bits.t_ccd = (UINT32)tt;

        ttA = (UINT8)tcrwpA.Bits.t_rrdr;
        tt = (UINT8)tcrwp.Bits.t_rrdr;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_rrdr = (UINT32)ttA;
        tcrwp.Bits.t_rrdr = (UINT32)tt;

        ttA = (UINT8)tcrwpA.Bits.t_rrdd;
        tt = (UINT8)tcrwp.Bits.t_rrdd;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_rrdd = (UINT32)ttA;
        tcrwp.Bits.t_rrdd = (UINT32)tt;

        ttA = (UINT8)tcrwpA.Bits.t_wwdr;
        tt = (UINT8)tcrwp.Bits.t_wwdr;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_wwdr = (UINT32)ttA;
        tcrwp.Bits.t_wwdr = (UINT32)tt;

        ttA = (UINT8)tcrwpA.Bits.t_wwdd;
        tt = (UINT8)tcrwp.Bits.t_wwdd;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_wwdd = (UINT32)ttA;
        tcrwp.Bits.t_wwdd = (UINT32)tt;

        ttA = (UINT8)tcothp2A.Bits.t_rwdr;
        tt = (UINT8)tcothp2.Bits.t_rwdr;
        FastestCommon (&ttA, &tt, 0);
        tcothp2A.Bits.t_rwdr = (UINT32)ttA;
        tcothp2.Bits.t_rwdr = (UINT32)tt;

        ttA = (UINT8)tcothp2A.Bits.t_rwdd;
        tt = (UINT8)tcothp2.Bits.t_rwdd;
        FastestCommon (&ttA, &tt, 0);
        tcothp2A.Bits.t_rwdd = (UINT32)ttA;
        tcothp2.Bits.t_rwdd = (UINT32)tt;

        ttA = (UINT8)tcrwpA.Bits.t_wrdr;
        tt = (UINT8)tcrwp.Bits.t_wrdr;
        FastestCommon (&ttA, &tt, 0);
        tcrwpA.Bits.t_wrdr = (UINT32)ttA;
        tcrwp.Bits.t_wrdr = (UINT32)tt;

        ttA = (UINT8)tcothp2A.Bits.t_wrdd;
        tt = (UINT8)tcothp2.Bits.t_wrdd;
        FastestCommon (&ttA, &tt, 0);
        tcothp2A.Bits.t_wrdd = (UINT32)ttA;
        tcothp2.Bits.t_wrdd = (UINT32)tt;

        ttA = (UINT8)tcothp2A.Bits.t_rwsr;
        tt = (UINT8)tcothp2.Bits.t_rwsr;
        FastestCommon (&ttA, &tt, 0);
        tcothp2A.Bits.t_rwsr = (UINT32)ttA;
        tcothp2.Bits.t_rwsr = (UINT32)tt;

        (*channelNvList)[chA].tcrwp = tcrwpA.Data;
        MemWritePciCfgEp (host, socket, chA, TCRWP_MCDDC_CTL_REG, tcrwpA.Data);

        (*channelNvList)[chA].tcothp = tcothpA.Data;
        MemWritePciCfgEp (host, socket, chA, TCOTHP_MCDDC_CTL_REG, tcothpA.Data);
        tclrdpA.Bits.t_ccd_wr  =  tcrwpA.Bits.t_ccd;
        if (host->nvram.mem.WrCRC) {
          tclrdpA.Bits.t_ccd_wr += 1;
        }
        (*channelNvList)[chA].tcothp2 = tcothp2A.Data;
        MemWritePciCfgEp (host, socket, chA, TCOTHP2_MCDDC_CTL_REG, tcothp2A.Data);

        ttA = (UINT8)tclrdpA.Bits.t_wrds;
        tt = (UINT8)tclrdp.Bits.t_wrds;
        FastestCommon (&ttA, &tt, 0);
        tclrdpA.Bits.t_wrds = (UINT32)ttA;
        tclrdp.Bits.t_wrds = (UINT32)tt;

        ttA = (UINT8)tclrdpA.Bits.t_rwds;
        tt = (UINT8)tclrdp.Bits.t_rwds;
        FastestCommon (&ttA, &tt, 0);
        tclrdpA.Bits.t_rwds = (UINT32)ttA;
        tclrdp.Bits.t_rwds = (UINT32)tt;

        ttA = (UINT8)tclrdpA.Bits.t_wwds;
        tt = (UINT8)tclrdp.Bits.t_wwds;
        FastestCommon (&ttA, &tt, 0);
        tclrdpA.Bits.t_wwds = (UINT32)ttA;
        tclrdp.Bits.t_wwds = (UINT32)tt;

        ttA = (UINT8)tclrdpA.Bits.t_rrds;
        tt = (UINT8)tclrdp.Bits.t_rrds;
        FastestCommon (&ttA, &tt, 0);
        tclrdpA.Bits.t_rrds = (UINT32)ttA;
        tclrdp.Bits.t_rrds = (UINT32)tt;

        (*channelNvList)[chA].tclrdp = tclrdpA.Data;
        MemWritePciCfgEp (host, socket, chA, TCLRDP_MCDDC_CTL_REG, tclrdpA.Data);

        ttA = (UINT8)tclrdp1A.Bits.t_rr_l_ds;
        tt = (UINT8)tclrdp1.Bits.t_rr_l_ds;
        FastestCommon (&ttA, &tt, 0);
        tclrdp1A.Bits.t_rr_l_ds = (UINT32)ttA;
        tclrdp1.Bits.t_rr_l_ds = (UINT32)tt;

        ttA = (UINT8)tclrdp1A.Bits.t_ww_l_ds;
        tt = (UINT8)tclrdp1.Bits.t_ww_l_ds;
        FastestCommon (&ttA, &tt, 0);
        tclrdp1A.Bits.t_ww_l_ds = (UINT32)ttA;
        tclrdp1.Bits.t_ww_l_ds = (UINT32)tt;

        (*channelNvList)[chA].tclrdp1 = tclrdp1A.Data;
        MemWritePciCfgEp (host, socket, chA, TCLRDP1_MCDDC_CTL_REG, tclrdp1A.Data);

      }
    } // if lockstep

    (*channelNvList)[ch].tcrwp = tcrwp.Data;
    MemWritePciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG, tcrwp.Data);

    (*channelNvList)[ch].tcothp = tcothp.Data;
    MemWritePciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG, tcothp.Data);
    (*channelNvList)[ch].tcothp2 = tcothp2.Data;
    MemWritePciCfgEp (host, socket, ch, TCOTHP2_MCDDC_CTL_REG, tcothp2.Data);

    (*channelNvList)[ch].tclrdp = tclrdp.Data;
    MemWritePciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG, tclrdp.Data);

    (*channelNvList)[ch].tclrdp1 = tclrdp1.Data;
    MemWritePciCfgEp (host, socket, ch, TCLRDP1_MCDDC_CTL_REG, tclrdp1.Data);

  } // ch loop

  return status;
} // TurnAroundTimings


UINT8
CalctRRDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
  )
/*++


  This function calculates the read to read turnaround time for different ranks on the same DIMM

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tRRDR channel value

  --*/
{
  UINT8 odtStretch;
  UINT8 tRRDR = 0;
  UINT8 tRRDRLimit;
  
  tRRDR = (*ttVars).cas2RecEnDR;

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // t_RRDR = 1(RTT Change Skew) + max(ODT Gap due to CK PI Difference (for UDIMM), ODT Stretch, 1 (ODT Gap due to CAS2RCVEN))
    //

    if (((*ttVars).odtRdTrailing > 1) || ((*ttVars).odtRdLeading > 0)) {
      odtStretch = 1;
    } else {
      odtStretch = 0;
    }

    //
    // RTT Change Skew
    //
    tRRDR += 1;
    tRRDR += (*ttVars).clkDiffDR;
    tRRDR += odtStretch;

    //
    // If LRDIMM then min setting is 2
    //
    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (tRRDR < 2)){
      tRRDR = 2;
    }

  } else {
    //
    // DDR3
    //
    tRRDR += (*ttVars).clkDiffDR;

    //
    // Enforce LRDIMM minimum RRDR command spacing of 6 cycles per MB spec
    //
    if (host->nvram.mem.socket[socket].lrDimmPresent && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
      tRRDRLimit = 1;
      if (tRRDR < tRRDRLimit) {
        tRRDR = tRRDRLimit;
      }
    }
  }
  //
  // Make sure we don't overflow
  //
  if (tRRDR > 7) tRRDR = 7;

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tRRDR = %d\n", tRRDR));

  return tRRDR;
} // CalctRRDR

UINT8
CalctRRDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars
  )
/*++

  This function calculates the read to read turnaround time for different DIMMs

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval tRRDD channel value

  --*/
{
  UINT8     tRRDD;
  UINT8     odtStretch; 

  //
  // t_RRDD = 1(RTT Change Skew) + max(ODT Gap due to CK PI Difference (for UDIMM), ODT Stretch, 1 (ODT Gap due to CAS2RCVEN))
  //

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

    if ((ttVars->odtRdTrailing > 1) || (ttVars->odtRdLeading > 0)) {
      odtStretch = 1;
    } else {
      odtStretch = 0;
    }
    //
    // RTT Change Skew
    //
    tRRDD = 1;

    tRRDD += ttVars->cas2RecEnDD;
    tRRDD += ttVars->clkDiffDD;
    tRRDD += odtStretch;

    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (tRRDD < 2)) {
      tRRDD = 2;
    }

  } else {
    //
    // DDR3
    //
    tRRDD = 2 + ttVars->clkDiffDD + (*ttVars).odtRdLeading;
    if (ttVars->odtRdTrailing > 0){
      tRRDD += (ttVars->odtRdTrailing - 1);
    }

    //
    // Temporary to get around a problem that is being debugged
    //
    tRRDD++;
  }

  //
  // Make sure we don't overflow
  //
  if (tRRDD > 7) tRRDD = 7;

  MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "tRRDD = %d\n", tRRDD));
  return tRRDD;
} // CalctRRDD

void
DramSpecCalctWWDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tWWDR
  )
{
  UINT8     odtStretch; 
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

    //
    // Rtt change skew
    //
    *tWWDR = 1;

    if ((*channelNvList)[ch].maxDimm > 1) {
      if (((*ttVars).odtWrTrailing > 0) || ((*ttVars).odtWrLeading > 1)) {
        odtStretch = 1;
      } else {
        odtStretch = 0;
      }
    } else {
      odtStretch = 0;
    }
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "odtStretch = %d\n", odtStretch));

    *tWWDR += (*ttVars).cas2DrvEnDR;
    *tWWDR += (*ttVars).clkDiffDR;
    *tWWDR += odtStretch;

    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (*tWWDR < 2)){
      *tWWDR = 2;
    }
  } else {
    //
    // DDR3
    //

    // ADD the ODT value
    *tWWDR = 2 + (*ttVars).clkDiffDR + (*ttVars).odtWrTrailing;

    //
    // There is no ODT change for LRDIMMs so subtract 1
    // Enforce LRDIMM minimum WWDR command spacing of 6 cycles per MB spec
    //
    if (host->nvram.mem.socket[socket].lrDimmPresent && (*tWWDR > 1)) {
      *tWWDR = *tWWDR - 1;
    }
  }
} // DramSpecCalctWWDR

void
DramSpecCalctWWDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tWWDD
  )
{
  UINT8     odtStretch; 
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {

    //
    // Get ODT stretch
    //
    if ((*channelNvList)[ch].maxDimm > 1) {
      if (((*ttVars).odtWrTrailing > 0) || ((*ttVars).odtWrLeading > 1)) {
        odtStretch = 1;
      } else {
        odtStretch = 0;
      }
    } else {
      odtStretch = 0;
    }
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "odtStretch = %d\n", odtStretch));

    //
    // Rtt change skew
    //
    *tWWDD += 1;
    *tWWDD += (*ttVars).clkDiffDD;
    *tWWDD += odtStretch;

  } else {
    //
    // DDR3
    //

    //
    // Adjust for ODT
    //
    *tWWDD = 2 +(*ttVars).clkDiffDD + (*ttVars).odtWrTrailing;
  }

} // DramSpecCalctWWDD

void
DramSpecCalctRWSR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tRWSR
  )
{
  struct channelNvram                   (*channelNvList)[MAX_CH];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // RRT Change Skew
    //
    *tRWSR += 1;

    if (host->nvram.mem.socket[socket].lrDimmPresent){
      dataControl0.Data = (*channelNvList)[ch].dataControl0;

      if(dataControl0.Bits.longpreambleenable && (*tRWSR < 4)) {
        *tRWSR = 4;
      } else if(dataControl0.Bits.longpreambleenable && (host->nvram.mem.socket[socket].ddrFreq < 2133) && (*tRWSR < 5)) {
        *tRWSR = 5;
      } else if(dataControl0.Bits.longpreambleenable && (host->nvram.mem.socket[socket].ddrFreq >= 2133) && (*tRWSR < 6)) {
        *tRWSR = 6;
      }
    }
  } else {
    //
    // DDR3
    //

    //
    // Add module delay for LRDIMM
    //
    if ((*channelNvList)[ch].lrDimmPresent) {
      *tRWSR += (*channelNvList)[ch].common.nMDL;
    }
  }

} // DramSpecCalctRWSR

void
DramSpecCalctRWDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tRWDR
  )
{
  struct channelNvram                   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // RRT Change Skew
    //
    *tRWDR += 1;

    //
    // Get clk diff
    //
    if ((*ttVars).clkDiffDR) {
      *tRWDR += 1;
    }

    //
    // Add an extra clock for now
    //
    *tRWDR += 1;

    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (*tRWDR < 7)) {
      *tRWDR = 7;
    }

  } else {
    //
    // DDR3
    //

    if ((*ttVars).odtRdTrailing > 0){
      *tRWDR += ((*ttVars).odtRdTrailing - 1);
    }

    //
    // Add module delay for LRDIMM
    //
    if ((*channelNvList)[ch].lrDimmPresent) {
      *tRWDR += (*channelNvList)[ch].common.nMDL;
    }
  }

} // DramSpecCalctRWDR

void
DramSpecCalctRWDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tRWDD
  )
{
  struct channelNvram                   (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // RRT Change Skew
    //
    *tRWDD += 1;

    if (((*ttVars).odtRdTrailing > 1) || ((*ttVars).odtWrLeading > 1)) {
      *tRWDD += 1;
    }

    //
    // Get clk diff
    //
    if ((*ttVars).clkDiffDD) {
      *tRWDD += 1;
    }

    //
    // Add an extra clock for now
    //
    *tRWDD += 1;
  } else {
    //
    // DDR3
    //
    if((*ttVars).odtRdTrailing > 0) {
      *tRWDD += ((*ttVars).odtRdTrailing - 1);
    }

    //
    // Add module delay for LRDIMM
    //
    if ((*channelNvList)[ch].lrDimmPresent) {
      *tRWDD += (*channelNvList)[ch].common.nMDL;
    }
  }
} // DramSpecCalctRWDD

void
DramSpecCalctWRDR(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tWRDR
  )
{
  UINT8     odtStretch; 
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // Rtt change skew
    //
    *tWRDR = 1;

    if ((*channelNvList)[ch].maxDimm > 1) {
      if (((*ttVars).odtWrTrailing > 0) || ((*ttVars).odtRdLeading > 0)) {
        odtStretch = 1;
      } else {
        odtStretch = 0;
      }
    } else {
      odtStretch = 0;
    }
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "odtStretch = %d\n", odtStretch));

    *tWRDR += (*ttVars).clkDiffDR;
    *tWRDR += odtStretch;

    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (*tWRDR < 2)){
      *tWRDR = 2;
    }

  } else {

    // Add extra ODT
    *tWRDR = 1 + (*ttVars).clkDiffDR + (*ttVars).odtWrTrailing + (*ttVars).odtRdLeading;

    if (host->nvram.mem.socket[socket].lrDimmPresent) {
      *tWRDR = *tWRDR - 1;
    }
  }
} // DramSpecCalctWRDR

void
DramSpecCalctWRDD(
         PSYSHOST  host,
         UINT8     socket,
         UINT8     ch,
         TT_VARS   *ttVars,
         UINT8     *tWRDD
  )
/*++

  This function calculates the write to read turnaround time for different DIMMs

  @param host    - Point to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param ttVars  - Structure that holds the variables

  @retval WR to RD different dimm

  --*/
{
  UINT8 odtStretch;
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    //
    // Rtt change skew
    //
    *tWRDD = 1;

    if ((*channelNvList)[ch].maxDimm > 1) {
      if (((*ttVars).odtWrTrailing > 0) || ((*ttVars).odtRdLeading > 0)) {
        odtStretch = 1;
      } else {
        odtStretch = 0;
      }
    } else {
      odtStretch = 0;
    }
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "odtStretch = %d\n", odtStretch));

    *tWRDD += (*ttVars).clkDiffDD;
    *tWRDD += odtStretch;

  } else {

    // Add extra ODT
    *tWRDD = 1 + (*ttVars).clkDiffDD + (*ttVars).odtWrTrailing + (*ttVars).odtRdLeading;

    if (host->nvram.mem.socket[socket].lrDimmPresent) {
      *tWRDD = *tWRDD - 1;
    }
  }

} // DramSpecCalctWRDD


void
EffectiveBusDelayInit(
                 PSYSHOST  host,
                 UINT8     socket,
                 UINT8     ch,
                 INT16     *CwlAdj,
                 UINT32                              *IOComp,
                 UINT16                              *IntDelta,
                 UINT32                              *longPreambleEnable
  )
{
  struct channelNvram     (*channelNvList)[MAX_CH];
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  TCOTHP_MCDDC_CTL_STRUCT tCOTHP;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT  dataControl0;

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  tCOTHP.Data = MemReadPciCfgEp (host, socket, ch, TCOTHP_MCDDC_CTL_REG);

  if(tCOTHP.Bits.t_cwl_adj >= 4){
    *CwlAdj = -1;
  } else {
    *CwlAdj = (INT16)tCOTHP.Bits.t_cwl_adj;
  }
  *CwlAdj = *CwlAdj * (2 * PiTicks);


  dataControl0.Data = (*channelNvList)[ch].dataControl0;
  *longPreambleEnable = dataControl0.Bits.longpreambleenable;
  *IOComp      = ((*channelNvList)[ch].ioLatency1) * PiTicks;
  if ((host->var.common.socketType == SOCKET_EP) || (host->var.common.socketType == SOCKET_DE)) {
    *IntDelta    = (4 * PiTicks) + 32;        //equates to 4 UI
  } else {
    //
    // The EN parts have more internal delay
    //
    *IntDelta    = (6 * PiTicks) + 32;        //equates to 6 UI
  }
} // EffectiveBusDelayInit

#ifdef DEBUG_TURNAROUNDS

const INT8  taNameTable  [TA_POR_TYPES][6] = {
  { "nRRDR"},
  { "nRRDD"},
  { "nWWDR"},
  { "nWWDD"},
  { "nWRDR"},
  { "nWRDD"},
  { "nRWSR"},
  { "nRWDR"},
  { "nRWDD"},
  { "nCCD "},
  { "nEODT"}
};

/**

  Compares turnaround times with HAS table of expected values
  reports via scratchpad if worse than expected

  @param host  - Point to sysHost
  @param socket  - Socket numer

  @retval SUCCESS

**/
UINT32
CheckTurnAroundTimings(
  PSYSHOST host,
  UINT8    socket
  )
{
  UINT32  status;
  UINT8   ch;
  UINT8   dimm;
#ifdef   SERIAL_DBG_MSG
  UINT8   rank;
#endif  // SERIAL_DBG_MSG
  UINT8   worse;
  UINT8   i;
  UINT8   taTable[TA_POR_TYPES];
  UINT8   tableUpdate;
  UINT8   allSingle;
  TCOTHP_MCDDC_CTL_STRUCT           tcothp;
  TCRWP_MCDDC_CTL_STRUCT            tcrwp;
  TCOTHP2_MCDDC_CTL_STRUCT          tcothp2;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT  biosScratchPad1;

  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
#ifdef   SERIAL_DBG_MSG
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  struct RankCh       (*rankPerCh)[MAX_RANK_CH];
#endif  // SERIAL_DBG_MSG
  UINT8               taPORTableMin[TA_POR_TYPES] = {
                      // RDIMM/UDIMM
                      /* rrdr */  1,
                      /* rrdd */  1,
                      /* wwdr */  1,
                      /* wwdd */  1,
                      /* rwsr */  1,
                      /* wrdr */  1,
                      /* wrdd */  1,
                      /* rwdr */  1,
                      /* rwdd */  1,
                      /* ccd  */  0,
                      /* eodt */  0
                      };
  UINT8               taPORTableMax[TA_POR_TYPES] = {
                      // RDIMM/UDIMM
                      /* rrdr */  4,
                      /* rrdd */  5,
                      /* wwdr */  4,
                      /* wwdd */  5,
                      /* wrdr */  3,
                      /* wrdd */  3,
                      /* rwsr */  7,
                      /* rwdr */  8,
                      /* rwdd */  9,
                      /* ccd  */  0,
                      /* eodt */  0
                      };

  channelNvList = GetChannelNvList(host, socket);

  status = SUCCESS;
  worse = 0;        // Set worse indicator to "worse timings not detected yet"

  //
  // Until the end of the power on we put the max value
  //
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nCompare Turnaround timings against POR timings\n"));

  tableUpdate = 0;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    allSingle = 1;
    dimmNvList    = GetDimmNvList(host, socket, ch);

    if (((host->nvram.mem.dimmTypePresent == RDIMM) && (!(host->nvram.mem.socket[socket].lrDimmPresent && (host->nvram.mem.dramType == SPD_TYPE_DDR3)))) || ((host->nvram.mem.dimmTypePresent == UDIMM)||(host->nvram.mem.dimmTypePresent == SODIMM))) {
      if ((host->nvram.mem.socket[socket].ddrFreq >= DDR_1867) && (tableUpdate == 0)) {
        taPORTableMin[TA_POR_T_RWSR] = taPORTableMin[TA_POR_T_RWSR] + 1;
        taPORTableMin[TA_POR_T_RWDD] = taPORTableMin[TA_POR_T_RWDD] + 1;
        taPORTableMin[TA_POR_T_RWDR] = taPORTableMin[TA_POR_T_RWDR] + 1;
        taPORTableMin[TA_POR_T_WWDD] = taPORTableMin[TA_POR_T_WWDD] + 1;
        tableUpdate = 1;
      }
    }

    //
    // Timing Constraints DDR3 Read Write Parameters
    //

    tcrwp.Data = (*channelNvList)[ch].tcrwp;
    tcothp.Data = (*channelNvList)[ch].tcothp;
    tcothp2.Data = (*channelNvList)[ch].tcothp2;

    if (((*channelNvList)[ch].lrRankMultEnabled) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
      taPORTableMin[TA_POR_T_CCD] = LR_RANK_MULT_TA + TA_PAD;
      taPORTableMax[TA_POR_T_CCD] = LR_RANK_MULT_TA + TA_PAD;
    } else {
      taPORTableMin[TA_POR_T_CCD] = 0;
      taPORTableMax[TA_POR_T_CCD] = 0;
    }

    if ((*channelNvList)[ch].lrDimmPresent && (host->nvram.mem.dramType == SPD_TYPE_DDR3) && (tableUpdate == 0)) {
      taPORTableMin[TA_POR_T_WRDR] = taPORTableMin[TA_POR_T_WRDR] - 1;
      taPORTableMin[TA_POR_T_WRDD] = taPORTableMin[TA_POR_T_WRDD] - 1;
      taPORTableMin[TA_POR_T_WWDR] = taPORTableMin[TA_POR_T_WWDR] >> 1;
      taPORTableMin[TA_POR_T_RWSR] = taPORTableMin[TA_POR_T_RWSR] + (*channelNvList)[ch].common.nMDL;
      taPORTableMin[TA_POR_T_RWDD] = taPORTableMin[TA_POR_T_RWDD] + (*channelNvList)[ch].common.nMDL;
      taPORTableMin[TA_POR_T_RWDR] = taPORTableMin[TA_POR_T_RWDR] + (*channelNvList)[ch].common.nMDL;
      taPORTableMin[TA_POR_T_EODT] = taPORTableMin[TA_POR_T_EODT] + (*channelNvList)[ch].common.nMDL;
      taPORTableMin[TA_POR_T_WWDD] = taPORTableMin[TA_POR_T_WWDD] >> 1;
      taPORTableMin[TA_POR_T_RRDR] = taPORTableMin[TA_POR_T_RRDR] + 1;
      taPORTableMax[TA_POR_T_WRDR] = taPORTableMax[TA_POR_T_WRDR] - 1;
      taPORTableMax[TA_POR_T_WRDD] = taPORTableMax[TA_POR_T_WRDD] - 1;
      taPORTableMax[TA_POR_T_WWDR] = taPORTableMax[TA_POR_T_WWDR] - 1;
      taPORTableMax[TA_POR_T_RWSR] = taPORTableMax[TA_POR_T_RWSR] + (*channelNvList)[ch].common.nMDL;
      taPORTableMax[TA_POR_T_RWDD] = taPORTableMax[TA_POR_T_RWDD] + (*channelNvList)[ch].common.nMDL;
      taPORTableMax[TA_POR_T_RWDR] = taPORTableMax[TA_POR_T_RWDR] + (*channelNvList)[ch].common.nMDL;
      taPORTableMax[TA_POR_T_EODT] = taPORTableMax[TA_POR_T_EODT] + (*channelNvList)[ch].common.nMDL;
      taPORTableMax[TA_POR_T_WWDD] = taPORTableMax[TA_POR_T_WWDD] - 1;
      taPORTableMax[TA_POR_T_RRDR] = taPORTableMax[TA_POR_T_RRDR] + 1;
      tableUpdate = 1;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].numRanks >= 2) {
        allSingle = 0;
        break;
      }
    }

    if ((allSingle == 1) && (tableUpdate == 0)) {
      taPORTableMax[TA_POR_T_WWDD] = taPORTableMax[TA_POR_T_WWDD] - 1;
      tableUpdate = 1;
    }

    // Init actual timings table
    taTable[TA_POR_T_CCD]  = (UINT8) tcrwp.Bits.t_ccd;
    taTable[TA_POR_T_RWSR] = (UINT8) tcothp2.Bits.t_rwsr;
    taTable[TA_POR_T_WRDD] = (UINT8) tcothp2.Bits.t_wrdd;
    taTable[TA_POR_T_WRDR] = (UINT8) tcrwp.Bits.t_wrdr;
    taTable[TA_POR_T_RWDD] = (UINT8) tcothp2.Bits.t_rwdd;
    taTable[TA_POR_T_RWDR] = (UINT8) tcothp2.Bits.t_rwdr;
    taTable[TA_POR_T_WWDD] = (UINT8) tcrwp.Bits.t_wwdd;
    taTable[TA_POR_T_WWDR] = (UINT8) tcrwp.Bits.t_wwdr;
    taTable[TA_POR_T_RRDD] = (UINT8) tcrwp.Bits.t_rrdd;
    taTable[TA_POR_T_RRDR] = (UINT8) tcrwp.Bits.t_rrdr;
    taTable[TA_POR_T_EODT] = (UINT8) tcrwp.Bits.t_rwsr;
#ifdef   SERIAL_DBG_MSG
    // Print all this information out
    if (checkMsgLevel(host, SDBG_MAX)) {
      getPrintFControl(host);
      // Now compare tables and print/post appropriate messages
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "TurnA - Min - Max - Actual\n"));
      for (i = 0; i < TA_POR_TYPES; i++) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "%s - %2d  - %2d  - %2d", taNameTable[i], taPORTableMin[i], taPORTableMax[i], taTable[i]));
        if (taTable[i] > taPORTableMax[i]) {
          rcPrintf ((host, " - Too High"));
        } else if (taTable[i] < taPORTableMin[i]) {
          rcPrintf ((host, " - Too Low"));
        }// >
        rcPrintf ((host, "\n"));
      } // for i
      rcPrintf ((host, "\n"));
      releasePrintFControl(host);
    } // if checkMsgLevel
#endif //  SERIAL_DBG_MSG

    for (i = 0; i < TA_POR_TYPES; i++) {
      if (taPORTableMax[i] != taTable[i]) {
        status = FAILURE;
        if (taTable[i] > taPORTableMax[i]) {
          worse = 1;
        } // >
      } // !=
    } // for i
  } // ch loop

  // Read/Modify/Write ScratchPad1 Bits 2/3
  biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
  biosScratchPad1.Data = biosScratchPad1.Data & ~(BIT3+BIT2);

  if (status == FAILURE) {
    biosScratchPad1.Data = biosScratchPad1.Data | BIT2;
  }
  if (worse) {
    biosScratchPad1.Data = biosScratchPad1.Data | BIT3;
  }
  WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);

#ifdef   SERIAL_DBG_MSG
  getPrintFControl(host);
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "   RoundTrip timings summary\n"));
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "      R0 - R1 - R2 - R3 \n"));
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    rankPerCh = &host->nvram.mem.socket[socket].channelList[ch].rankPerCh;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                    ""));

      for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
        if (rank != 0) {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        " - "));
        }
        if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "NA"));
        } else {
          MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "%2d", (*rankPerCh)[(*rankList)[rank].rankIndex].Roundtrip));
        }
      } //rank
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "\n"));
    } //dimm
  } //ch
  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\n"));
  releasePrintFControl(host);
#endif //  SERIAL_DBG_MSG

  return status;
}

#endif  // DEBUG_TURNAROUNDS

void
SaveRefreshRate(
               PSYSHOST host,
               UINT8    socket,
               UINT8    ch,
               UINT8    refreshRate
               )
{
  struct channelNvram  (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  //
  // Save Refresh Rate for later use
  //
  (*channelNvList)[ch].twoXRefreshSetPerCh = refreshRate;
}


void 
InitTurnAroundTimes(
                   PSYSHOST                   host, 
                   UINT8                      socket, 
                   UINT8                      ch, 
                   UINT8                      dimm,
                   TCRWP_MCDDC_CTL_HSX_STRUCT *tcrwp,
                   TCOTHP2_MCDDC_CTL_STRUCT   *tcothp2,
                   TCLRDP_MCDDC_CTL_STRUCT    *tclrdp
                   )
{
  TCLRDP1_MCDDC_CTL_STRUCT                    tclrdp1;
  struct channelNvram                         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

#ifdef LRDIMM_SUPPORT
  if ((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    tcrwp->Bits.t_rrdr = 5;
    tcrwp->Bits.t_rrdd = 6;
    tcrwp->Bits.t_wwdr = 7;
    tcrwp->Bits.t_wwdd = 7;
    tcrwp->Bits.t_wrdr = 6;
    tcrwp->Bits.t_rwsr = 0;
    tcrwp->Bits.t_rwdr = 0;

    tcothp2->Bits.t_rwdd = 14;
    tcothp2->Bits.t_rwdr = 14;
    tcothp2->Bits.t_rwsr = 14;
    tcothp2->Bits.t_wrdd = 8;
  } else
#endif
  {
    tcrwp->Bits.t_rrdr = 3;
    tcrwp->Bits.t_rrdd = 4;
    tcrwp->Bits.t_wwdr = 5;
    tcrwp->Bits.t_wwdd = 5;
    tcrwp->Bits.t_wrdr = 4;
    tcrwp->Bits.t_rwsr = 0;
    tcrwp->Bits.t_rwdr = 0;

    if (host->nvram.mem.socket[socket].lrDimmPresent) {
      tcothp2->Bits.t_rwdd = 6 + (*channelNvList)[ch].common.nMDL;
      tcothp2->Bits.t_rwdr = 6 + (*channelNvList)[ch].common.nMDL;
      tcothp2->Bits.t_rwsr = 6 + (*channelNvList)[ch].common.nMDL;
      //  Add extra leading ODT delay cycles for LRDIMM
      tcrwp->Bits.t_rwsr = ((*channelNvList)[ch].common.nMDL & 0x7);
      tcrwp->Bits.t_rwdr = (((*channelNvList)[ch].common.nMDL & 0x8) >> 3);
    } else {
      tcothp2->Bits.t_rwdd = 6;
      tcothp2->Bits.t_rwdr = 6;
      tcothp2->Bits.t_rwsr = 6;
    }

    tcothp2->Bits.t_wrdd = 4;
  }

  tclrdp->Bits.t_rrds = tcrwp->Bits.t_rrdr;
  tclrdp->Bits.t_wwds = tcrwp->Bits.t_wwdr;
  tclrdp->Bits.t_rwds = tcothp2->Bits.t_rwdr;
  tclrdp->Bits.t_wrds = tcrwp->Bits.t_wrdr;
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    // t_rwds = CL - CWL + RBL/2 + 1tCK + tWPRE
    tclrdp->Bits.t_rwds = (*channelNvList)[ch].common.nCL - (*channelNvList)[ch].common.nWL + 4 + 1;
    //tclrdp.Bits.t_rwds = 7;
    // t_wrds = CWL + WBL/2 + tWTR_L
    tclrdp->Bits.t_wrds = (*channelNvList)[ch].common.nWL + 4 + (*channelNvList)[ch].common.nWTR_L;
    //tclrdp.Bits.t_wrds = 0x12;
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "t_rwds = %d, t_wrds = %d\n", tclrdp->Bits.t_rwds, tclrdp->Bits.t_wrds));
  }
  MemWritePciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG, tclrdp->Data);

  tclrdp1.Data = MemReadPciCfgEp (host, socket, ch, TCLRDP1_MCDDC_CTL_REG);
  tclrdp1.Bits.t_rr_l_ds = tcrwp->Bits.t_rrdr;
  tclrdp1.Bits.t_ww_l_ds = tcrwp->Bits.t_wwdr;
  MemWritePciCfgEp (host, socket, ch, TCLRDP1_MCDDC_CTL_REG, tclrdp1.Data);

  (*channelNvList)[ch].tcrwp = tcrwp->Data;
  MemWritePciCfgEp (host, socket, ch, TCRWP_MCDDC_CTL_REG, tcrwp->Data);

  (*channelNvList)[ch].tcothp2 = tcothp2->Data;
  MemWritePciCfgEp (host, socket, ch, TCOTHP2_MCDDC_CTL_REG, tcothp2->Data);
}


void
SetEncodeCSMode(
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch,
             UINT8    dimm,
#ifdef  LRDIMM_SUPPORT
             UINT32   *lrdimmQrDimmBitMask, 
#endif
             UINT8    *density,
             UINT16   *t_stagger_ref,
             void     *ptcstagger
)
{
  TCSTAGGER_REF_MCDDC_CTL_HSX_BDX_STRUCT *tcstagger;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList    = GetDimmNvList(host, socket, ch);

  tcstagger = (TCSTAGGER_REF_MCDDC_CTL_HSX_BDX_STRUCT*)ptcstagger;
#ifdef  LRDIMM_SUPPORT
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && ((*dimmNvList)[dimm].lrRankMult > 1)) { // IF LRDIMM in Rank Multiplication mode.
    *density = (*dimmNvList)[dimm].sdramCapacity - ((*dimmNvList)[dimm].lrRankMult >> 1);
    if ((*density < SPD_2Gb) || (*density > SPD_8Gb)) {  // Only 2Gbit and 4Gbit densities supported
      MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "ERROR: T_Stagger LRDIMM - DRAM density out of range: %d\n", density));
      *density = SPD_4Gb; // force it to 4 for now to error on large t-stagger value
    }

    *t_stagger_ref = (UINT16) tREFnStaggerLRDIMM [*density - SPD_2Gb][host->nvram.mem.socket[socket].ddrFreq];
  }

  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].lrDimmPresent)) {
    if ((*dimmNvList)[dimm].lrRankMult > 1) {
      *lrdimmQrDimmBitMask |= (1 << dimm);
    }
  }
#endif // LRDIMM_SUPPORT

  if (*t_stagger_ref > (*tcstagger).Bits.t_stagger_ref) {
    (*tcstagger).Bits.t_stagger_ref = (UINT32)(*t_stagger_ref);
  }
}

void
SetStaggerRefEn(
             PSYSHOST                                    host,
             UINT8                                       socket,
             UINT8                                       ch
)
{
  MCMNT_CHKN_BIT_MCDDC_CTL_BDX_STRUCT         chknbitmcddcctlhsx;
   

  // set stagger_ref_en per HSD 4987583
  chknbitmcddcctlhsx.Data = MemReadPciCfgEp (host, socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
  chknbitmcddcctlhsx.Bits.stagger_ref_en = 1;
  MemWritePciCfgEp (host, socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, chknbitmcddcctlhsx.Data);
}

void
IdtLrbufWA(
             PSYSHOST                                    host,
             UINT8                                       socket,
             UINT8                                       ch,
             WORK_AROUND_TYPE                            wa,
             void                                        *data
)
{
  UINT8                                       dimm;
  UINT8                                       density;
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT              *tcrap;
  TCOTHP_MCDDC_CTL_STRUCT                     *tcothp;
  TCSRFTP_MCDDC_CTL_STRUCT                    tcsrftp;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  dimmNvList    = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  switch(wa){
    case t_cke:
#ifdef  LRDIMM_SUPPORT
#ifdef IDT_LRBUF_WA
    tcrap = (TCRAP_MCDDC_CTL_HSX_BDX_STRUCT*)data;
    if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        if (((*dimmNvList)[dimm].lrbufDVid == LRBUF_IDT_DVID) &&
            ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_RID_B0)) {
          // sighting 200011
          if (tcrap->Bits.t_cke < (16-3)) 
            tcrap->Bits.t_cke = tcrap->Bits.t_cke + 3 ;
          else 
            OutputWarning (host, WARN_DIMM_COMPAT, 00, socket, ch, dimm, NO_RANK);
          // get out of the 'for' loop - Timing is per channel
          dimm = 4; // just in case the break does not get us out of here.
          break;
        } // LRBUF_IDT_DVID_B0
      } // dimm
    } // lrDimmPresent
#endif // IDT_LRBUF_WA
#endif // LRDIMM_SUPPORT
    break; // t_cke
    case t_xp:
      tcothp = (TCOTHP_MCDDC_CTL_STRUCT*)data;
      if (tXP[host->nvram.mem.ratioIndex] > 7) {
        tcothp->Bits.t_xp = 7;
      } else {
        tcothp->Bits.t_xp = tXP[host->nvram.mem.ratioIndex];
      }
#ifdef  LRDIMM_SUPPORT
#ifdef IDT_LRBUF_WA
      if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
          if (((*dimmNvList)[dimm].lrbufDVid == LRBUF_IDT_DVID) &&
              ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_RID_B0)) {
            // sighting 200011
            MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                          "tXP = %2x - tXPDLL = %2x\n", tcothp->Bits.t_xp, tcothp->Bits.t_xpdll));
            if (tcothp->Bits.t_xp < (8-3)) 
              tcothp->Bits.t_xp = tcothp->Bits.t_xp + 3 ;
            else {
              // OutputWarning (host, WARN_DIMM_COMPAT, 00, socket, ch, dimm, NO_RANK);
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                            "tXP exceeds size limit (7), setting to 7\n"));
              tcothp->Bits.t_xp = 0x7;
            }
            if (tcothp->Bits.t_xpdll < (32-3)) 
              tcothp->Bits.t_xpdll = tcothp->Bits.t_xpdll + 3 ;
            else 
              OutputWarning (host, WARN_DIMM_COMPAT, 00, socket, ch, dimm, NO_RANK);
            // get out of the 'for' loop - Timing is per channel
            dimm = 4; // just in case the break does not get us out of here.
            break;
          } // LRBUF_IDT_DVID_B0
        } // dimm
      } // lrDimmPresent
#endif // IDT_LRBUF_WA
#endif // LRDIMM_SUPPORT
    break; // t_xp
    case t_odt_oe:
    break;
    case t_xsdll:
      tcsrftp.Data            = MemReadPciCfgEp (host, socket, ch, TCSRFTP_MCDDC_CTL_REG);
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        tcsrftp.Bits.t_mod      = tMOD[host->nvram.mem.ratioIndex];
        //
        // Workaround for BIOS HSD
        //
        if ((*channelNvList)[ch].numRanks == 1) {
          density = (*dimmNvList)[0].sdramCapacity;
          tcsrftp.Bits.t_zqoper   = tZQOPER + tZQOPERAdder[density - 2][host->nvram.mem.socket[socket].ddrFreq];
        } else {
          tcsrftp.Bits.t_zqoper   = tZQOPER;
        }
        tcsrftp.Bits.t_xsdll    = tDLLK;
      } else {
        tcsrftp.Bits.t_zqoper   = tZQOPER_DDR4;
        tcsrftp.Bits.t_mod      = tMOD_DDR4;

        if (host->nvram.mem.socket[socket].ddrFreq <= DDR_1867) {
          tcsrftp.Bits.t_xsdll  = 597;
        } else {
          tcsrftp.Bits.t_xsdll  = 768;
        }
      }

      if (tXSOFFSET[host->nvram.mem.ratioIndex] > 15) {
        tcsrftp.Bits.t_xsoffset = 15;
      } else {
        tcsrftp.Bits.t_xsoffset = tXSOFFSET[host->nvram.mem.ratioIndex];
      }

#ifdef  LRDIMM_SUPPORT
#ifdef IDT_LRBUF_WA
      if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
          if (((*dimmNvList)[dimm].lrbufDVid == LRBUF_IDT_DVID) &&
              ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_RID_B0)) {
            // sighting 200011
            if (tcsrftp.Bits.t_xsdll < (4096-3)) 
              tcsrftp.Bits.t_xsdll = tcsrftp.Bits.t_xsdll + 3 ;
            else 
              OutputWarning (host, WARN_DIMM_COMPAT, 00, socket, ch, dimm, NO_RANK);

            if (tcsrftp.Bits.t_xsoffset < (16-3))
               tcsrftp.Bits.t_xsoffset = tcsrftp.Bits.t_xsoffset + 3 ;
            else 
              OutputWarning (host, WARN_DIMM_COMPAT, 00, socket, ch, dimm, NO_RANK);
            // get out of the 'for' loop - Timing is per channel
            dimm = 4; // just in case the break does not get us out of here.
            break;
          } // LRBUF_IDT_DVID_B0
        } // dimm
      } // lrDimmPresent
#endif // IDT_LRBUF_WA
#endif // LRDIMM_SUPPORT

    MemWritePciCfgEp (host, socket, ch, TCSRFTP_MCDDC_CTL_REG, tcsrftp.Data);
    break; // t_xsdll
  } // switch (wa)
}


void
SetMiscDelay(
             PSYSHOST host,
             UINT8    socket,
             UINT8    ch,
             void     *tcrap
)
{
}

void
Sett_zqcs(
             PSYSHOST                                    host,
             UINT8                                       socket,
             TCZQCAL_MCDDC_CTL_STRUCT                    *tczqcal
)
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
    tczqcal->Bits.t_zqcs     = tZQCS[host->nvram.mem.socket[socket].ddrFreq];
  } else {
    tczqcal->Bits.t_zqcs     = tZQCS_DDR4;
  }
}

void 
SetRdimmTimingCntl(
             PSYSHOST                                    host,
             RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT            *RdimmTimingCntl
)
{
    if (host->setup.mem.options & CA_PARITY_EN) {
    //
    // RDIMM Timing Control
    //
    if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->setup.mem.options & CA_PARITY_EN)){
      RdimmTimingCntl->Bits.t_mrd = 12;
    } else if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
      RdimmTimingCntl->Bits.t_mrd = tMRD_DDR3;
    } else {
      RdimmTimingCntl->Bits.t_mrd = tMRD_DDR4;
    }

    if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
      RdimmTimingCntl->Bits.t_stab = RdimmTimingCntl->Bits.t_stab + tSTAB[host->nvram.mem.ratioIndex];
    } else {
      RdimmTimingCntl->Bits.t_stab = RdimmTimingCntl->Bits.t_stab + tSTAB_DDR4[host->nvram.mem.ratioIndex];
    }
  }
}


UINT8
Sett_ckev(
             PSYSHOST                                    host,
             RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT           *RdimmTimingCntl2
)
{
  UINT8 Status = 0;
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    RdimmTimingCntl2->Bits.t_ckev = 2;
    Status = 1;
  } 
  return Status;
}

void
PartialWrStarvationCounter(
             PSYSHOST                                    host,
             UINT8                                       socket,
             UINT8                                       ch,
             PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_STRUCT  *pmmStarvCntr
)
{
  pmmStarvCntr->Data         = MemReadPciCfgEp (host, socket, ch, PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_REG);
  if((host->var.qpi.OutIsocEn == TRUE) && !((host->setup.mem.options & CA_PARITY_EN) && (host->nvram.mem.dimmTypePresent == RDIMM))){
    pmmStarvCntr->Bits.wpq_is = WPQ_IS;
  } else{
    pmmStarvCntr->Bits.wpq_is  = PWPQ_IS;
  }

  MemWritePciCfgEp (host, socket, ch, PWMM_STARV_CNTR_PRESCALER_MCDDC_CTL_REG, pmmStarvCntr->Data);
}

void 
SetMasterDllWakeUpTimer(
             PSYSHOST                                    host,
             UINT8                                       socket,
             UINT8                                       ch
)
{
  PM_DLL_MCDDC_CTL_STRUCT                     pm_dll;
  struct channelNvram                         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  //
  // PM DLL Control
  // Master DLL wake up timer = 500ns
  //
  pm_dll.Data             = MemReadPciCfgEp (host, socket, ch, PM_DLL_MCDDC_CTL_REG);
  pm_dll.Bits.mdll_wtimer = mdllTimer[host->nvram.mem.ratioIndex];
  if ((host->setup.mem.options & MDLL_SHUT_DOWN_EN) && ((*channelNvList)[ch].lrDimmPresent == 0)) {
    pm_dll.Bits.mdll_sden = 1;
  } else {
    pm_dll.Bits.mdll_sden = 0;
  }
  MemWritePciCfgEp (host, socket, ch, PM_DLL_MCDDC_CTL_REG, pm_dll.Data);
}

void
CheckTxVrefSupport(
             PSYSHOST                                    host,
             UINT8                                       socket,
             UINT8                                       ch,
             UINT8                                       dimm, 
             UINT8                                       rank, 
             UINT8                                       strobe,
             INT16                                       *zeroValue
)
{
  if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
    GetSetTxVref (host, socket, ch, dimm, rank, strobe, 0, DdrLevel, TxVref, GSM_FORCE_WRITE | GSM_WRITE_OFFSET,
                zeroValue);
  }
}

void
WriteFastBootTimings(
                     PSYSHOST   host,
                     UINT8      socket,
                     UINT8      ch,
                     UINT8      dimm, 
                     UINT8      rank, 
                     UINT8      strobe
)
{
  struct channelNvram           (*channelNvList)[MAX_CH];
  struct ddrRank                (*rankList)[MAX_RANK_DIMM];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  rankList = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;

  MemWritePciCfgEp (host, socket, ch, rxOffsetTable[strobe] + ((*rankList)[rank].logicalRank * 4),
                    (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][strobe]);
}

#ifdef  LRDIMM_SUPPORT
void
TimeConstraintLrdimm(
                    PSYSHOST                      host,
                    UINT8                         socket,
                    TCRAP_MCDDC_CTL_STRUCT        *tcrap,
                    TCRWP_MCDDC_CTL_HSX_STRUCT    tcrwp,
                    TCDBP_MCDDC_CTL_HSX_STRUCT    tcdbp
                    )
{

  if (host->nvram.mem.socket[socket].lrDimmPresent) {
    // MAX(T_WTR of LRDIMM, (T_WRDR' - T_CL + 2))
    if (tcrap->Bits.t_wtr < (tcrwp.Bits.t_wrdr - tcdbp.Bits.t_cl + 2)) {
      tcrap->Bits.t_wtr = tcrwp.Bits.t_wrdr - tcdbp.Bits.t_cl + 2;
    }
  }
}
#endif // LRDIMM_SUPPORT

void
Ddr3MrsTimingReg(
                PSYSHOST                                    host,
                UINT8                                       socket,
                UINT8                                       ch,
                RDIMMTIMINGCNTL_MCDDC_CTL_STRUCT            RdimmTimingCntl
                )
{
  TCMRS_MCDDC_CTL_STRUCT                      tcmrs;

  tcmrs.Data            = MemReadPciCfgEp (host, socket, ch, TCMRS_MCDDC_CTL_REG);
  tcmrs.Bits.tmrd_ddr3  = RdimmTimingCntl.Bits.t_mrd;
  MemWritePciCfgEp (host, socket, ch, TCMRS_MCDDC_CTL_REG, tcmrs.Data);
}

void 
CalcFinalGap(
            UINT16 *gap, 
            UINT16 maxGap, 
            UINT16 gapDimm
            )
{
  if (maxGap >= 39) {
    maxGap = maxGap - 39;
  } else {
    maxGap = 0;
  }

  if ((maxGap % (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) > 0) {
    *gap = (maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2)) + 1;
  } else {
    *gap = maxGap / (MAX_PHASE_IN_FINE_ADJUSTMENT * 2);
  }
}

void
SetRestoreTimings(
                 PSYSHOST  host,
                 UINT8     socket,
                 UINT8     ch
                 )
{
  struct channelNvram                         (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  MemWritePciCfgEp (host, socket, ch, ROUNDTRIP0_MCDDC_DP_REG, (*channelNvList)[ch].roundtrip0);
  MemWritePciCfgEp (host, socket, ch, ROUNDTRIP1_MCDDC_DP_REG, (*channelNvList)[ch].roundtrip1);
  MemWritePciCfgEp (host, socket, ch, IOLATENCY0_MCDDC_DP_REG, (*channelNvList)[ch].ioLatency0);
  MemWritePciCfgEp (host, socket, ch, IOLATENCY1_MCDDC_DP_REG, (*channelNvList)[ch].ioLatency1);
}

void
GetOdtTiming(
             PSYSHOST  host,
             UINT8     socket,
             UINT8     ch,
             UINT32    *rdOdtTiming, 
             UINT32    *wrOdtTiming
             )
{
  *rdOdtTiming = MemReadPciCfgEp (host, socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG);
  *wrOdtTiming = MemReadPciCfgEp (host, socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG);
}

