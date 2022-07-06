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
#include "MemThrot.h"
#include "MemWeight.h"
#include "SysHost.h"

#if 0
#define MemDebugPrintLocal(dbgInfo)  debugPrintMem dbgInfo
#else
#define MemDebugPrintLocal(dbgInfo)
#endif

#define DRAM_MAX_PWR_DEFAULT 92
#define DRAM_SELF_REF_PWR_DEFAULT 2563
//
// Local Prototypes
//
UINT32 DramRaplProg(PSYSHOST host, UINT8 socket, DramRaplDataStruct *RAPLData);
void   DramRaplProgCalc(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, DramRaplDataStruct *RAPLData, DramRaplLUTDataStruct RAPLLUTData[MAX_DIMM]);

//
// Disable warning for unsued input parameters
//
#pragma warning(disable : 4100)

UINT16
RefreshScaledPwr(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT32   slope,
  UINT16   powerInput
  );

UINT32
InitThrottlingEarly (
  PSYSHOST host
  )
/*++

  Initialize memory throttling

  @param host  - Point to sysHost

  @retval SUCCESS

--*/
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 mcId;
  UINT8                                 dimm;
  DIMM_TEMP_TH_0_MCDDC_CTL_STRUCT       dimmTempTh;
  DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_STRUCT dimmTempThrtLmt;
  TSOD_CONTROL_PCU_FUN1_STRUCT          pcuTsodCtrlCfg;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT      smbCntl;
  THRT_PWR_DIMM_0_MCDDC_CTL_STRUCT      thrtPwrDimm;

  socket = host->var.mem.currentSocket;

  pcuTsodCtrlCfg.Data = ReadCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG);
  pcuTsodCtrlCfg.Bits.tsod_polling_interval = 0;
  WriteCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG, pcuTsodCtrlCfg.Data);

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG);
    smbCntl.Bits.start_tsod_poll = 0;
    MemWritePciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG, smbCntl.Data);

    smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, SMBCNTL_1_MC_MAIN_REG);
    smbCntl.Bits.start_tsod_poll = 0;
    MemWritePciCfgMC (host, socket, mcId, SMBCNTL_1_MC_MAIN_REG, smbCntl.Data);
  } // mcId loop

  for (ch = 0; ch < MAX_CH; ch++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Initialize the temp threshold to safe defaults
      //
      dimmTempTh.Data =  MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_TH_0_MCDDC_CTL_REG + (dimm * 4)));
      dimmTempTh.Bits.temp_lo = 0x59;
      MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_TH_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempTh.Data);

      //
      // Initialize threshold limits to maximum settings, effectively disabling bandwidth throttling
      //
      dimmTempThrtLmt.Data =  MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_REG + (dimm * 4)));
      dimmTempThrtLmt.Bits.thrt_mid = 0xFF;
      dimmTempThrtLmt.Bits.thrt_hi = 0xFF;
      dimmTempThrtLmt.Bits.thrt_crit = 0xFF;
      MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempThrtLmt.Data);

      //
      // Disable DIMM based power throttling
      //
      thrtPwrDimm.Data =  (UINT16)MemReadPciCfgEp (host, socket, ch, (THRT_PWR_DIMM_0_MCDDC_CTL_REG + (dimm * 2)));
      thrtPwrDimm.Bits.thrt_pwr_en = 0;
      MemWritePciCfgEp (host, socket, ch, (THRT_PWR_DIMM_0_MCDDC_CTL_REG + (dimm * 2)), (UINT32)thrtPwrDimm.Data);
    } // dimm loop
  } // ch loop

  return SUCCESS;
}

UINT32
InitThrottling (
  PSYSHOST host
  )
/*++

  Initialize memory throttling

  @param host  - Point to sysHost

  @retval SUCCESS

--*/
{
  UINT8                                       i;
  //UINT8                                       n = 0;
  UINT8                                       socket;
  UINT8                                       ch;
  UINT8                                       mcId;
  UINT8                                       dimm;
  UINT8                                       dimmInTTMODETable, dimmInTTTable, dimmInWTTable;
  UINT8                                       raplmodeset = 0;
  UINT16                                      TTMRow = 0;
  UINT16                                      TTRow = 0;
  UINT16                                      WTRow = 0;
  //UINT32                                      bwlimittfdata = 0;
  //UINT32                                      CsrReg = 0;
  BOOLEAN                                     DualDIMMVrPresent;
  const struct DimmTTRowEntryStruct           *ptrDimmTT = NULL;
  struct DimmTTRowEntryStruct                 TTcurrentDimm;
  const struct DimmWTRowEntryStruct           *ptrDimmWT = NULL;
  struct DimmWTRowEntryStruct                 WTcurrentDimm;
  const struct DimmTThrottlingStruct           *ptrDimmTTMODE = NULL;
  struct DimmTThrottlingStruct                 TTMODEcurrentDimm;
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  struct channelNvram                         (*channelNvList)[MAX_CH];
  DramRaplDataStruct                          RAPLData;
  DramRaplLUTDataStruct                       RAPLLUTData[MAX_DIMM] = {0};
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT               ChnTempCfgReg;
  ET_CFG_MCDDC_CTL_STRUCT                     etCfgReg;
  PM_CMD_PWR_0_MCDDC_CTL_STRUCT               pmCmdPwrReg;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT            smbCntl;
  MH_MAINCNTL_MC_MAIN_HSX_BDX_STRUCT          mhMainCntl;
  MH_SENSE_500NS_CFG_MC_MAIN_STRUCT           mhSense500nsReg;
  DIMM_TEMP_TH_0_MCDDC_CTL_STRUCT             dimmTempTh;
  DIMM_TEMP_OEM_0_MCDDC_CTL_STRUCT            dimmTempOem;
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT        dimmTempEvOfst;
  DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_STRUCT       dimmTempThrtLmt;
  MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_STRUCT  memTrmlEstimationCfgReg;
  SMB_TSOD_POLL_RATE_MC_MAIN_STRUCT           smbTsodPollRate;
  CMD_DRAM_PM_CH_VR_MAP                       PmChVrMap;
  PCU_BIOS_SPARE2_PCU_FUN3_STRUCT             pcuBiosSpare2Reg;
  FEATURES_VCU_FUN_0_STRUCT                   VcnCrFeaturesCFG;
  DYNAMIC_PERF_POWER_CTL_PCU_FUN2_HSX_BDX_STRUCT  dynamicPerfPowerCtl;

  UINT32 PcodeMailboxStatus = 0;
  UINT32 ActiveVrMask = 0;
  UINT32 MemoryCurrentEstimate = 0;
  UINT32 MemoryCurrentCaculate = 0;
  UINT32 phaseShedData = 0;
  struct smbDevice  tsod;
  UINT16 mstData;
  UINT8  varThermalThrottlingOptions;
  UINT16 perChMaxMemPwr[MAX_CH] = {0};
  UINT16 DramMaxPwr[MAX_CH] = {0};
  UINT16 DramSelfRefPwr[MAX_CH] = {0};
  UINT16 perChSelfRefreshPwr[MAX_CH] = {0};

  socket = host->var.mem.currentSocket;

//4986014 HSX Clone: Crashdump 0x91 PECI completion code after an IERR injected with a real PCIe card
  dynamicPerfPowerCtl.Data = ReadCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG);
  VcnCrFeaturesCFG.Data = ReadCpuPciCfgEx (host, socket, 0, FEATURES_VCU_FUN_0_REG);

  dynamicPerfPowerCtl.Bits.allow_peci_pcode_error_rsp = 1;
  WriteCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, dynamicPerfPowerCtl.Data);

  VcnCrFeaturesCFG.Bits.allow_peci_vcode_error_rsp = 1;
  WriteCpuPciCfgEx (host, socket, 0, FEATURES_VCU_FUN_0_REG, VcnCrFeaturesCFG.Data);


#ifdef SERIAL_DBG_MSG
  getPrintFControl(host);
  MemDebugPrint((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Initialize Throttling\n\n"));

#endif // SERIAL_DBG_MSG
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  // Initialize data variables used later
  if (host->setup.mem.dramraplen != 0x00) {
    for (i = 0; i < 0x0f; i++ ) {
      if (i < 4) {
        RAPLData.RAPLRanksPresentData[i] = 0x00;
      }
      RAPLData.RAPLDRAMPmWrPwrCOEFFValConst[i] = 0x0000;
      RAPLData.RAPLDRAMPmWrPwrCOEFFValPower[i] = 0x0000;
      RAPLData.RAPLWritePwrScaleSval[i] = 0x0000;
    } // i loop

    RAPLData.RAPL3SigmaPolicy = 0x00;
    RAPLData.RAPLMode = 0x00;
    RAPLData.RAPLDramAdjVal = 0x00;
    RAPLData.RAPLDramPwrInfoMaxVal = 0x0000;
    RAPLData.RAPLDramPwrInfoTdpVal = 0x0000;
    RAPLData.RAPLDramPwrInfoMinVal = 0x0000;
    RAPLData.RAPLWritePwrScaleMval = 0x0000;
  }

  varThermalThrottlingOptions = host->setup.mem.thermalThrottlingOptions;
  host->nvram.mem.socket[socket].DimmWithoutThermalSensorFound = 0;

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    //
    // Initialize MH_SENSE_500NS register to recommended values
    //
    mhSense500nsReg.Data = MemReadPciCfgMC (host, socket, mcId, MH_SENSE_500NS_CFG_MC_MAIN_REG);
    switch (GetClosestFreq(host, socket)) {
      case DDR_2800:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x2BC;
        break;
      case DDR_2667:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x29A;
        break;
      case DDR_2400:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x258;
        break;
      case DDR_2133:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x216; // 1/(2133/2) = 0.9375 tCK(ns) -> 500 / 0.9375 = 533.33 clocks = 0x216 with roundup
        break;

      case DDR_1867:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x1D2;
        break;

      case DDR_1600:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x190;
        break;

      case DDR_1333:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x14D;
        break;

      case DDR_1067:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x10A;
        break;

      case DDR_800:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x0C8;
        break;

      default:
        mhSense500nsReg.Bits.cnfg_500_nanosec = 0x190;
        break;
    }
    MemWritePciCfgMC (host, socket, mcId, MH_SENSE_500NS_CFG_MC_MAIN_REG, mhSense500nsReg.Data);

    //
    // Configure MEMHOT
    //
    mhMainCntl.Data = MemReadPciCfgMC (host, socket, mcId, MH_MAINCNTL_MC_MAIN_REG);
    //
    // Set mh_output_en if requested by the user or if TSOD polling is enabled (HSD 4987262)
    if ((host->setup.mem.thermalThrottlingOptions & MH_OUTPUT_EN) || (varThermalThrottlingOptions & CLTT_EN)) {
      mhMainCntl.Bits.mh_output_en = 1;
    } else {
      mhMainCntl.Bits.mh_output_en = 0;
    }

    if (host->setup.mem.thermalThrottlingOptions & MH_SENSE_EN) {
      mhMainCntl.Bits.mh_sense_en = 1;
    } else {
      mhMainCntl.Bits.mh_sense_en = 0;
    }
    MemWritePciCfgMC (host, socket, mcId, MH_MAINCNTL_MC_MAIN_REG, mhMainCntl.Data);
  } // mcId

  PmChVrMap.Data = 0x00000000;    // Initialize data to 0
  DualDIMMVrPresent = TRUE;     // Assume we have 2 active DIMM VR's per socket
  //
  // Use MAILBOX_BIOS_CMD_ACTIVE_VR_MASK command code (0x87) to find the active VRs
  //
  PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_ACTIVE_VR_MASK, 0);

  //
  // Read the Active DRAM VR's from the Mailbox Data Register
  //
  if (PcodeMailboxStatus == 0) {
    ActiveVrMask = ReadCpuPciCfgEx(host, socket, 0, BIOS_MAILBOX_DATA);
  }

  if ((ActiveVrMask & (BIT0 + BIT1)) != (BIT0 + BIT1)) {
    DualDIMMVrPresent = FALSE;            // Indicate that there is only one Active DIMM VR
  }

  if (DualDIMMVrPresent == TRUE) {
    // Indicate that DIMM ch 0 & 1 are mapped to VR0 & DIMM ch 2 & 3 are mapped to VR1
    PmChVrMap.Bits.vr0chmask = 0x3;
    if (Is2HA(host)) {
      PmChVrMap.Bits.vr1chmask = 0x3;
    } else {
      PmChVrMap.Bits.vr1chmask = 0xc;
    }
  } else {
    // Indicate that all channels mapped to one VR
    PmChVrMap.Bits.vr0chmask = 0x7;
    PmChVrMap.Bits.vr1chmask = 0x0;
  }

  // Set DDR_PM_CHN_R_MAP
  PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_DRAM_PM_CHN_VR_MAP, PmChVrMap.Data);
  MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
         "Issued MBX DRAM_PM_CHN_VR_MAP with CMD data = 0x%08x\n ", PmChVrMap.Data ));

  for (ch = 0; ch < MAX_CH; ch++) {
    OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, STS_RMT, (UINT16)((SUB_INIT_THROTTLING << 8)|(ch << 4)|socket)));

    mcId = GetMCID(host, socket, ch);

    //
    // initialize the smbCntl variable
    //
    if (ch == 0) {
      smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG);
      smbCntl.Bits.tsod_present = 0;
    } else if (ch == 2) {
      if (Is2HA(host)) {
        smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG);
      } else {
        smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, SMBCNTL_1_MC_MAIN_REG);
      }
      smbCntl.Bits.tsod_present = 0;
    }

    ChnTempCfgReg.Data = MemReadPciCfgEp (host, socket, ch, CHN_TEMP_CFG_MCDDC_CTL_REG);
    //HSD 4986411: BIOS sets Thermal Throttling "Disabled" to "OLTT"
    if (varThermalThrottlingOptions & (OLTT_EN|CLTT_EN)) {
      ChnTempCfgReg.Bits.bw_limit_thrt_en = 1;
    } else {
      ChnTempCfgReg.Bits.bw_limit_thrt_en = 0;
    }

    MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
         ":: (*channelNvList)[%d].maxDimm = %d\n ", ch, (*channelNvList)[ch].maxDimm ));

    dimmInTTMODETable = 0;
    if (varThermalThrottlingOptions & (OLTT_EN|CLTT_EN)) {
       TTMODEcurrentDimm.TTMode = 0;
       if ((varThermalThrottlingOptions & CLTT_EN) && ((varThermalThrottlingOptions & OLTT_EN) == 0))  {
         TTMODEcurrentDimm.TTMode = 1;
       }

       TTMODEcurrentDimm.TTFREQ = host->nvram.mem.socket[socket].ddrFreq;
       TTMODEcurrentDimm.TTDIMMPC = (*channelNvList)[ch].maxDimm;

       if (dimmInTTMODETable == 0) {
          for (TTMRow = 0; TTMRow <= MAX_TTM_ROW; TTMRow++ ) {
            ptrDimmTTMODE = &DimmTTMODE[TTMRow];
            if ((ptrDimmTTMODE->TTMode == TTMODEcurrentDimm.TTMode) &&
                (ptrDimmTTMODE->TTDIMMPC == TTMODEcurrentDimm.TTDIMMPC) &&
                (ptrDimmTTMODE->TTFREQ == TTMODEcurrentDimm.TTFREQ)) {
              dimmInTTMODETable = 1;
              MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 ":: Found TTMRow: %d,   ", TTMRow ));
              MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "TTMode: %d,  TTDIMMPC: %d,  TTFREQ: %x,  TTBW_THROTTLE_TF: %x,  TTTHRT_HI: %x\n ",
                 ptrDimmTTMODE->TTMode,
                 ptrDimmTTMODE->TTDIMMPC,
                 ptrDimmTTMODE->TTFREQ,
                 ptrDimmTTMODE->TTBW_THROTTLE_TF,
                 ptrDimmTTMODE->TTTHRT_HI
               ));
              break;
            }
          } // TTMRow loop
        }
     }

    if ((*channelNvList)[ch].enabled) {
      ChnTempCfgReg.Bits.cltt_or_pcode_temp_mux_sel = 0;
/*
      // Override bw_limit_tf setting based on user configuration
      n = host->setup.mem.dramraplbwlimittf;     // Get user bw_limit_tf multipler
      CsrReg = (ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG)) & 0x0000003f;  // only preserve bits 5:0

      switch (CsrReg) {
        case 6:
          // DCLK = 400 MHz
          bwlimittfdata = (n * (400/8)) & 0x7FF;
          break;

        case 8:
          // DCLK = 533 MHz
          bwlimittfdata = (n * (533/8)) & 0x7FF ;
          break;

        case 10:
          // DCLK = 667 MHz
          bwlimittfdata = (n * (667/8)) & 0x7FF;
          break;

        case 12:
          // DCLK = 800 MHz
          bwlimittfdata = (n * (800/8)) & 0x7FF;
          break;

        case 14:
          // DCLK = 933.5 MHz
          bwlimittfdata = (n * (934/8)) & 0x7FF;
          break;

        case 16:
          // DCLK = 1066.5 MHz
          bwlimittfdata = (n * (1067/8)) & 0x7FF;
          break;

        case 18:
          // DCLK = 1200 MHz
          bwlimittfdata = (n * (1200/8)) & 0x7FF;
          break;

        default:
          bwlimittfdata = ChnTempCfgReg.Bits.bw_limit_tf; // Just use the exisiting value
      } // switch

      ChnTempCfgReg.Bits.bw_limit_tf = bwlimittfdata;
*/
      if (dimmInTTMODETable == 1) {
        ChnTempCfgReg.Bits.bw_limit_tf = ptrDimmTTMODE->TTBW_THROTTLE_TF;
      }

      MemDebugPrintLocal((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "\n:: ChnTempCfgReg.Bits.bw_limit_tf = 0x%04X\n ",ChnTempCfgReg.Bits.bw_limit_tf ));

      if ((varThermalThrottlingOptions & OLTT_EN) || (host->setup.mem.dramraplen != 0x00)) {
        //
        //enable oltt_en for hybrid mode or oltt_en only or if RAPL is enabled
        //
        ChnTempCfgReg.Bits.oltt_en = 1;
        if (dimmInTTMODETable == 1) {
          ChnTempCfgReg.Bits.thrt_ext = ptrDimmTTMODE->TTTHRT_HI; //HSD 4987488
        } else {
          ChnTempCfgReg.Bits.thrt_ext = OLTT_THRT_HI_DEFAULT;     //HSD 4987488
        }
    } else {
        ChnTempCfgReg.Bits.oltt_en = 0;
      }
      if ((varThermalThrottlingOptions & CLTT_EN) && ((varThermalThrottlingOptions & OLTT_EN) == 0)) {
        ChnTempCfgReg.Bits.cltt_or_pcode_temp_mux_sel = 1;
        if (dimmInTTMODETable == 1) {
          ChnTempCfgReg.Bits.thrt_ext = ptrDimmTTMODE->TTTHRT_HI; //HSD 4987488
        } else {
          ChnTempCfgReg.Bits.thrt_ext = CLTT_THRT_HI_DEFAULT;     //HSD 4987488
        }
      }
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
         "\n:: ChnTempCfgReg.Bits.thrt_ext = %x\n ", ChnTempCfgReg.Bits.thrt_ext));

      if ((host->var.common.cpuType == CPU_HSX) ||( IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX))) { // WA reversed for BDX ML A0 and DE V0
        //
        //HSD 4987201: Need thrt_allow_isoch=1 to avoid silent data corruption due to PRE sent during Powerdown
        //
        ChnTempCfgReg.Bits.thrt_allow_isoch = 1;
      }

      MemWritePciCfgEp (host, socket, ch, CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);
      //
      // Initialize per channel electrical throttling parameters
      //
      etCfgReg.Data = MemReadPciCfgEp (host, socket, ch, ET_CFG_MCDDC_CTL_REG);
      if ((host->setup.mem.electricalThrottling == ET_ENABLE) ||
          ((host->setup.mem.electricalThrottling == ET_AUTO) &&
           (((*channelNvList)[ch].numQuadRanks > 0) || (*channelNvList)[ch].lrDimmPresent))) {
        etCfgReg.Bits.et_en = 1;
        switch (GetClosestFreq(host, socket)) {
        case DDR_1867:
        case DDR_2133:
        case DDR_2400:
        case DDR_2667:
        case DDR_2800:
          etCfgReg.Bits.et_smpl_prd = 0x77;
          break;

        case DDR_1600:
          etCfgReg.Bits.et_smpl_prd = 0x66;
          break;

        case DDR_1333:
          etCfgReg.Bits.et_smpl_prd = 0x55;
          break;

        case DDR_1067:
          etCfgReg.Bits.et_smpl_prd = 0x44;
          break;

        case DDR_800:
        default:
          etCfgReg.Bits.et_smpl_prd = 0x33;
          break;
        }
      } else {
        etCfgReg.Bits.et_smpl_prd = 0;
        etCfgReg.Bits.et_en = 0;
      }
      MemWritePciCfgEp (host, socket, ch, ET_CFG_MCDDC_CTL_REG, etCfgReg.Data);
      //
      // Initialize per dimm throttling and power related registers
      //
      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        //
        // set TSOD present bit if a TSOD is present on the DIMM
        //
        if ((*dimmNvList)[dimm].dimmTs & BIT7) {
          if (ch & BIT0) {
            if (IsDESku(host) && (!CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX))) {
              smbCntl.Bits.tsod_present |= (BIT2 << dimm);
            } else {
              smbCntl.Bits.tsod_present |= (BIT4 << dimm);
            }
          } else {
            smbCntl.Bits.tsod_present |= (BIT0 << dimm);
          }
        } else if (varThermalThrottlingOptions & CLTT_EN) {
          //
          // here CLTT is enabled on input but a dimm is present with no thermal sensor...so
          // disable CLTT and print a warning message
          //
          varThermalThrottlingOptions &= ~CLTT_EN;
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "\nCLTT is forced off due to a dimm detected without a TSOD.\n"));
          host->nvram.mem.socket[socket].DimmWithoutThermalSensorFound = 1;
        }

        //
        // Look up DIMM command weight value in the fixed table
        // First get the data needed to match a table entry for this dimm
        //
        dimmInWTTable = 0;

        WTcurrentDimm.WTDimmType = (*dimmNvList)[dimm].actKeyByte2 & 0x0F;
        if (WTcurrentDimm.WTDimmType == SPD_LRDIMM_DDR4) WTcurrentDimm.WTDimmType = SPD_LRDIMM;

        if (!((WTcurrentDimm.WTDimmType == SPD_RDIMM) ||
              (WTcurrentDimm.WTDimmType == SPD_UDIMM) ||
              (WTcurrentDimm.WTDimmType == SPD_UDIMM_ECC) ||
              (WTcurrentDimm.WTDimmType == SPD_SODIMM) ||
              (WTcurrentDimm.WTDimmType == SPD_ECC_SO_UDIMM) ||
              (WTcurrentDimm.WTDimmType == SPD_LRDIMM))) {
          dimmInWTTable = 0xFF;
        }
        if (((WTcurrentDimm.WTDimmType == SPD_UDIMM)||(WTcurrentDimm.WTDimmType == SPD_SODIMM)) && ((*dimmNvList)[dimm].SPDMemBusWidth & BIT3)) {
          WTcurrentDimm.WTDimmType |= 0x10;
        }

        WTcurrentDimm.WTLvAndDramDensity = (*dimmNvList)[dimm].sdramCapacity;

        if ((WTcurrentDimm.WTLvAndDramDensity < SPD_1Gb) || (WTcurrentDimm.WTLvAndDramDensity > SPD_16Gb)) {
          dimmInWTTable = 0xFF;
        }

        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          WTcurrentDimm.WTLvAndDramDensity |= 0x40;
        } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
          WTcurrentDimm.WTLvAndDramDensity |= 0x20;
        } else {
          WTcurrentDimm.WTLvAndDramDensity |= 0x10;  //SPD_VDD_150
        }
        MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "\n::MPM:: 2> dimmInWTTable = %x, WTcurrentDimm.WTLvAndDramDensity = %x,   ddrVoltage = %x\n\n", dimmInWTTable, WTcurrentDimm.WTLvAndDramDensity, host->nvram.mem.socket[socket].ddrVoltage));

        WTcurrentDimm.WTSpdByte7 = (*dimmNvList)[dimm].SPDModuleOrg;
        WTcurrentDimm.WTFrequency = host->nvram.mem.socket[socket].ddrFreq;

        if (WTcurrentDimm.WTFrequency > DDR_2400) {
          dimmInWTTable = 0xFF;
        }

        if (dimm == 2) {
          WTcurrentDimm.WTDimmPos = 1;
        } else {
          WTcurrentDimm.WTDimmPos = dimm;
        }


        if (dimmInWTTable == 0) {

          if ( (host->nvram.mem.dramType == SPD_TYPE_DDR4) ) {
            MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\n::MEM DDR4:: DDR4 Memory is detected in system. Look at DDR4 Power table now...\n"));
            if (host->var.common.cpuType == CPU_HSX) {
              for (WTRow = 0; WTRow <= (sizeof(DimmWT4_HSX)/sizeof(WTcurrentDimm)); WTRow++ ) {
                ptrDimmWT = &DimmWT4_HSX[WTRow];
                if ((ptrDimmWT->WTDimmType == WTcurrentDimm.WTDimmType) &&
                  (ptrDimmWT->WTLvAndDramDensity == WTcurrentDimm.WTLvAndDramDensity) &&
                  (ptrDimmWT->WTSpdByte7 == WTcurrentDimm.WTSpdByte7) &&
                  (ptrDimmWT->WTFrequency == WTcurrentDimm.WTFrequency) &&
                  (ptrDimmWT->WTDimmPos == WTcurrentDimm.WTDimmPos)) {
                  dimmInWTTable = 1;
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "\n::MEM DDR4:: DDR4 Table is available to use. dimmInWTTable = %x\n\n", dimmInWTTable));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "::MEM DDR4:: (%d)        %x, %x, %x, %x, %x, ", WTRow, ptrDimmWT->WTDimmType, ptrDimmWT->WTLvAndDramDensity, ptrDimmWT->WTSpdByte7, ptrDimmWT->WTDimmPos, ptrDimmWT->WTFrequency));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "%d,  %d,  %d,  %d,  %d,  %d,  %d,  ", ptrDimmWT->WTPmCmdPwrData, ptrDimmWT->DramPmWritePowerCoeff, ptrDimmWT->DramMaxPwr, ptrDimmWT->DramTdp, ptrDimmWT->DramMinPwrClosedPage, ptrDimmWT->DramWritePwrScaleClosedPage));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "%d,  %d,  %d,  %d,  %d,  %d,  %d\n\n", ptrDimmWT->DramMaxPwrOpenPage, ptrDimmWT->DramTdpOpenPage, ptrDimmWT->DramMinPwrOpenPage, ptrDimmWT->DramWritePwrScaleOpenPage, ptrDimmWT->DramRefreshRateSlope, ptrDimmWT->DramSelfRefreshPower));
                  break;
                }
              } // WTRow loop
            } else {
              for (WTRow = 0; WTRow <= (sizeof(DimmWT4_BDX)/sizeof(WTcurrentDimm)); WTRow++ ) {
                ptrDimmWT = &DimmWT4_BDX[WTRow];
                if ((ptrDimmWT->WTDimmType == WTcurrentDimm.WTDimmType) &&
                  (ptrDimmWT->WTLvAndDramDensity == WTcurrentDimm.WTLvAndDramDensity) &&
                  (ptrDimmWT->WTSpdByte7 == WTcurrentDimm.WTSpdByte7) &&
                  (ptrDimmWT->WTFrequency == WTcurrentDimm.WTFrequency) &&
                  (ptrDimmWT->WTDimmPos == WTcurrentDimm.WTDimmPos)) {
                  dimmInWTTable = 1;
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "\n::MEM DDR4:: DDR4 Table is available to use. dimmInWTTable = %x\n\n", dimmInWTTable));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "::MEM DDR4:: (%d)        %x, %x, %x, %x, %x, ", WTRow, ptrDimmWT->WTDimmType, ptrDimmWT->WTLvAndDramDensity, ptrDimmWT->WTSpdByte7, ptrDimmWT->WTDimmPos, ptrDimmWT->WTFrequency));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "%d,  %d,  %d,  %d,  %d,  %d,  %d,  ", ptrDimmWT->WTPmCmdPwrData, ptrDimmWT->DramPmWritePowerCoeff, ptrDimmWT->DramMaxPwr, ptrDimmWT->DramTdp, ptrDimmWT->DramMinPwrClosedPage, ptrDimmWT->DramWritePwrScaleClosedPage));
                  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "%d,  %d,  %d,  %d,  %d,  %d,  %d\n\n", ptrDimmWT->DramMaxPwrOpenPage, ptrDimmWT->DramTdpOpenPage, ptrDimmWT->DramMinPwrOpenPage, ptrDimmWT->DramWritePwrScaleOpenPage, ptrDimmWT->DramRefreshRateSlope, ptrDimmWT->DramSelfRefreshPower));
                  break;
                }
              } // WTRow loop
            }
          } else {
            MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "\n::MEM DDR3:: DDR3 Memory is detected in system. Look at DDR3 Power table now...\n"));

            for (WTRow = 0; WTRow <= (sizeof(DimmWT)/sizeof(WTcurrentDimm)); WTRow++ ) {
              ptrDimmWT = &DimmWT[WTRow];

              if ((ptrDimmWT->WTDimmType == WTcurrentDimm.WTDimmType) &&
                (ptrDimmWT->WTLvAndDramDensity == WTcurrentDimm.WTLvAndDramDensity) &&
                (ptrDimmWT->WTSpdByte7 == WTcurrentDimm.WTSpdByte7) &&
                (ptrDimmWT->WTFrequency == WTcurrentDimm.WTFrequency) &&
                (ptrDimmWT->WTDimmPos == WTcurrentDimm.WTDimmPos)) {
                dimmInWTTable = 1;
                MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "\n::MEM DDR3:: DDR3 Table is available to use. dimmInWTTable = %x\n\n", dimmInWTTable));
                MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "\n::MEM DDR3:: DDR4 Table is available to use. dimmInWTTable = %x\n\n", dimmInWTTable));
                MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "::MEM DDR3:: (%d)        %x, %x, %x, %x, %x, ", WTRow, ptrDimmWT->WTDimmType, ptrDimmWT->WTLvAndDramDensity, ptrDimmWT->WTSpdByte7, ptrDimmWT->WTDimmPos, ptrDimmWT->WTFrequency));
                MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "%d,  %d,  %d,  %d,  %d,  %d,  %d,  ", ptrDimmWT->WTPmCmdPwrData, ptrDimmWT->DramPmWritePowerCoeff, ptrDimmWT->DramMaxPwr, ptrDimmWT->DramTdp, ptrDimmWT->DramMinPwrClosedPage, ptrDimmWT->DramWritePwrScaleClosedPage));
                MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "%d,  %d,  %d,  %d,  %d,  %d,  %d\n\n", ptrDimmWT->DramMaxPwrOpenPage, ptrDimmWT->DramTdpOpenPage, ptrDimmWT->DramMinPwrOpenPage, ptrDimmWT->DramWritePwrScaleOpenPage, ptrDimmWT->DramRefreshRateSlope, ptrDimmWT->DramSelfRefreshPower));
                break;
              }
            } // WTRow loop
          }
        }

        pmCmdPwrReg.Data = MemReadPciCfgEp (host, socket, ch, (PM_CMD_PWR_0_MCDDC_CTL_REG + (dimm * 4)));

        if (dimmInWTTable == 1) {
          //
          // look up pmCmdPwrReg value from the fixed table
          //
          pmCmdPwrReg.Data = ptrDimmWT->WTPmCmdPwrData;
          MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "::MPM::  get table value pmCmdPwrReg.Data = %x\n\n", pmCmdPwrReg.Data));

        } else {
          //
          // default DRAM Power setting
          //
          pmCmdPwrReg.Bits.pwrodt_cnt_dimm = 16;
          if (((*dimmNvList)[dimm].keyByte2 == SPD_UDIMM)||((*dimmNvList)[dimm].keyByte2 == SPD_SODIMM)) {
            pmCmdPwrReg.Bits.active_idle_dimm = 8;
            pmCmdPwrReg.Bits.pwrref_dimm = 133;
            pmCmdPwrReg.Bits.pwract_dimm = 29;
          } else if (((*dimmNvList)[dimm].actKeyByte2 & 0x0F) == SPD_LRDIMM) {
            pmCmdPwrReg.Bits.active_idle_dimm = 8;
            pmCmdPwrReg.Bits.pwrref_dimm = 106;
            pmCmdPwrReg.Bits.pwract_dimm = 31;
          } else {  // assume RDIMM
            pmCmdPwrReg.Bits.active_idle_dimm = 8;
            pmCmdPwrReg.Bits.pwrref_dimm = 167;
            pmCmdPwrReg.Bits.pwract_dimm = 11;
          }
          pmCmdPwrReg.Bits.pwrcasw_dimm = 15;
          pmCmdPwrReg.Bits.pwrcasr_dimm = 15;
        }
        MemWritePciCfgEp (host, socket, ch, (PM_CMD_PWR_0_MCDDC_CTL_REG + (dimm * 4)), pmCmdPwrReg.Data);


        //
        // Initialize socket DRAM TDP structure for use by the DRAM VR Phase Shedding initialization code
        //
        if (dimmInWTTable == 1) {
          // Read the TDP from the table
          perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + (ptrDimmWT->DramMaxPwr);
          if (!dimm) {
            DramMaxPwr[ch] = perChMaxMemPwr[ch];
            DramSelfRefPwr[ch] = perChSelfRefreshPwr[ch] + ptrDimmWT->DramSelfRefreshPower;
          }
          perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + (ptrDimmWT->DramSelfRefreshPower);
        } else {
          //load default value
          if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
             switch ((*dimmNvList)[dimm].keyByte2) {
               case SPD_UDIMM:
               case SPD_SODIMM:
                 perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 126;
                 perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 2679;
                 break;
               case SPD_UDIMM_ECC:
               case SPD_ECC_SO_UDIMM:
                 perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 114;
                 perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 2679;
                 break;
               case SPD_LRDIMM:
                 perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 120;
                 perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 2679;
                 break;
               case SPD_RDIMM:
               default:
                 perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 113;
                 perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 1782;
             } // end switch
          } else {
             switch ((*dimmNvList)[dimm].keyByte2) {
                case SPD_UDIMM:
                case SPD_SODIMM:
                  perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 93;
                  perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 561;
                  break;
                case SPD_LRDIMM:
                  perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 336;
                  perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 4446;
                  break;
                case SPD_RDIMM:
                default:
                  perChMaxMemPwr[ch] = perChMaxMemPwr[ch] + 251;
                  perChSelfRefreshPwr[ch] = perChSelfRefreshPwr[ch] + 2279;
             } // end switch
          } //if
         
          DramMaxPwr[ch] = DRAM_MAX_PWR_DEFAULT;
          DramSelfRefPwr[ch] = DRAM_SELF_REF_PWR_DEFAULT;
        }

        //
        // Init DRAM RAPL Settings here
        //
        if (host->setup.mem.dramraplen != 0x00) {
          MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                        "Initialize DRAM RAPL\n\n"));
          if (dimmInWTTable  != 1) {
            //
            // Initialize DRAM RAPL with defult values
            //
            if (((*dimmNvList)[dimm].keyByte2 == SPD_UDIMM)||((*dimmNvList)[dimm].keyByte2 == SPD_SODIMM)) {
              RAPLLUTData[dimm].DramRaplDataDramPmWritePowerCoeffCval = 113;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwr = UDIMM_MAX_POWER_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdp = UDIMM_DRAM_TDP_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrClosedPage = UDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleClosedPage = UDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwrOpenPage = UDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdpOpenPage = UDIMM_TDP_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrOpenPage = UDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleOpenPage = UDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope = UDIMM_REFRESH_RATE_SLOPE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramSelfRefreshPower = UDIMM_SELF_REFRESH_POWER_DEFAULT;
            } else if ((*dimmNvList)[dimm].actKeyByte2 == SPD_LRDIMM) {
              RAPLLUTData[dimm].DramRaplDataDramPmWritePowerCoeffCval = 336;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwr = LRDIMM_MAX_POWER_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdp = LRDIMM_DRAM_TDP_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrClosedPage = LRDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleClosedPage = LRDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwrOpenPage = LRDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdpOpenPage = LRDIMM_TDP_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrOpenPage = LRDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleOpenPage = LRDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope = LRDIMM_REFRESH_RATE_SLOPE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramSelfRefreshPower = LRDIMM_SELF_REFRESH_POWER_DEFAULT;
            } else {  // assume RDIMM
              RAPLLUTData[dimm].DramRaplDataDramPmWritePowerCoeffCval = 251;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwr = RDIMM_MAX_POWER_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdp = RDIMM_DRAM_TDP_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrClosedPage = RDIMM_MIN_PWR_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleClosedPage = RDIMM_WRITE_PWR_SCALE_CLOSED_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMaxPwrOpenPage = RDIMM_MAX_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramTdpOpenPage = RDIMM_TDP_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramMinPwrOpenPage = RDIMM_MIN_PWR_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleOpenPage = RDIMM_WRITE_PWR_SCALE_OPEN_PAGE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope = RDIMM_REFRESH_RATE_SLOPE_DEFAULT;
              RAPLLUTData[dimm].DramRaplDataDramSelfRefreshPower = RDIMM_SELF_REFRESH_POWER_DEFAULT;
            }
          }

          if (dimmInWTTable == 1) {
            //
            // Initialize DRAM RAPL with values from lookup table
            //
            RAPLLUTData[dimm].DramRaplDataDramPmWritePowerCoeffCval = ptrDimmWT->DramPmWritePowerCoeff;
            RAPLLUTData[dimm].DramRaplDataDramMaxPwr = ptrDimmWT->DramMaxPwr;
            RAPLLUTData[dimm].DramRaplDataDramTdp = ptrDimmWT->DramTdp;
            RAPLLUTData[dimm].DramRaplDataDramMinPwrClosedPage = ptrDimmWT->DramMinPwrClosedPage;
            RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleClosedPage = ptrDimmWT->DramWritePwrScaleClosedPage;
            RAPLLUTData[dimm].DramRaplDataDramMaxPwrOpenPage = ptrDimmWT->DramMaxPwrOpenPage;
            RAPLLUTData[dimm].DramRaplDataDramTdpOpenPage = ptrDimmWT->DramTdpOpenPage;
            RAPLLUTData[dimm].DramRaplDataDramMinPwrOpenPage = ptrDimmWT->DramMinPwrOpenPage;
            RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleOpenPage = ptrDimmWT->DramWritePwrScaleOpenPage;
            RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope = ptrDimmWT->DramRefreshRateSlope;
            RAPLLUTData[dimm].DramRaplDataDramSelfRefreshPower = ptrDimmWT->DramSelfRefreshPower;
            MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "::MPM::  DRAM RAPL is enabled, using table value in RAPLLUTData structure.\n\n"));
          }


          DramRaplProgCalc(host, socket, ch, dimm, &RAPLData, RAPLLUTData);

        } // end DRAM RAPL Init enable

        //
        // Initialize the temp thresholds and bandwidth limits
        //
        dimmTempThrtLmt.Data = MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_REG + (dimm * 4)));
        dimmTempTh.Data =  MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_TH_0_MCDDC_CTL_REG + (dimm * 4)));

        if (varThermalThrottlingOptions & CLTT_EN) {
          //
          // set the throttling register values for CLTT based on the Romley Thermal Throttling Whitepaper rev0.95
          dimmTempThrtLmt.Bits.thrt_mid = CLTT_THRT_MID_DEFAULT;

          if (dimmInTTMODETable == 1) {
            dimmTempThrtLmt.Bits.thrt_hi = ptrDimmTTMODE->TTTHRT_HI; //HSD 4987559
          } else {
            dimmTempThrtLmt.Bits.thrt_hi = CLTT_THRT_HI_DEFAULT / (*channelNvList)[ch].maxDimm;  //devide by #DIMMs installed per channel
          }

          dimmTempThrtLmt.Bits.thrt_crit = CLTT_THRT_CRIT_DEFAULT;
          if ((*channelNvList)[ch].features & (DOUBLE_REFI + EXTENDED_TEMP)) {
            dimmTempTh.Bits.temp_lo = CLTT_TEMP_LO_DOUBLE_REFRESH_DEFAULT;
            dimmTempTh.Bits.temp_mid = CLTT_TEMP_MID_DOUBLE_REFRESH_DEFAULT;
            dimmTempTh.Bits.temp_hi = CLTT_TEMP_HI_DOUBLE_REFRESH_DEFAULT;
          } else{
            dimmTempTh.Bits.temp_lo = CLTT_TEMP_LO_SINGLE_REFRESH_DEFAULT;
            dimmTempTh.Bits.temp_mid = CLTT_TEMP_MID_SINGLE_REFRESH_DEFAULT;
            dimmTempTh.Bits.temp_hi = CLTT_TEMP_HI_SINGLE_REFRESH_DEFAULT;
          }
        } else if (varThermalThrottlingOptions & OLTT_EN){
          dimmTempThrtLmt.Bits.thrt_mid = OLTT_THRT_MID_DEFAULT;

          if (dimmInTTMODETable == 1) {
            dimmTempThrtLmt.Bits.thrt_hi = ptrDimmTTMODE->TTTHRT_HI; //HSD 4987559
          } else {
            dimmTempThrtLmt.Bits.thrt_hi = ((host->setup.mem.olttPeakBWLIMITPercent * 255) / 100) / (*channelNvList)[ch].maxDimm;  //devide by #DIMMs installed per channel
          }

          dimmTempThrtLmt.Bits.thrt_crit = dimmTempThrtLmt.Bits.thrt_hi;
          dimmTempTh.Bits.temp_lo = OLTT_TEMP_LO_DEFAULT;
          dimmTempTh.Bits.temp_mid = OLTT_TEMP_MID_DEFAULT;
          dimmTempTh.Bits.temp_hi = OLTT_TEMP_HI_DEFAULT;
        }

        if (((*channelNvList)[ch].twoXRefreshSetPerCh >= 20) && ((*channelNvList)[ch].twoXRefreshSetPerCh <= 40)) {  //HSD: 4985866/4987658
          dimmTempTh.Bits.temp_lo = 0;
          dimmTempThrtLmt.Bits.thrt_mid = 0xFF;
        }

        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
           "\n:: dimmTempThrtLmt.Bits.thrt_hi = %x\n ", dimmTempThrtLmt.Bits.thrt_hi ));

        MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_THRT_LMT_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempThrtLmt.Data);
        MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_TH_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempTh.Data);

        //
        // Initialize the DIMM_TEMP_OEM registers based on the v0.95 Romley Thermal Throttling whitepaper
        //
        dimmTempOem.Data = MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_OEM_0_MCDDC_CTL_REG + (dimm * 4)));
        dimmTempOem.Bits.temp_oem_hi = 0x50;
        dimmTempOem.Bits.temp_oem_lo = 0x4B;
        MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_OEM_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempOem.Data);

        //
        // Look up DIMM thermal threshold offset in the fixed table
        // First get the data needed to match a table entry for this dimm
        //
        dimmInTTTable = 0;

        TTcurrentDimm.TTDDRType = 0;
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          TTcurrentDimm.TTDDRType = 1;
        }

        TTcurrentDimm.TTDimmType = (*dimmNvList)[dimm].actKeyByte2 & 0x0F;
        if (TTcurrentDimm.TTDimmType == SPD_LRDIMM_DDR4) TTcurrentDimm.TTDimmType = SPD_LRDIMM;

        if (!((TTcurrentDimm.TTDimmType == SPD_RDIMM) ||
              (TTcurrentDimm.TTDimmType == SPD_UDIMM) ||
              (TTcurrentDimm.TTDimmType == SPD_UDIMM_ECC) ||
              (TTcurrentDimm.TTDimmType == SPD_SODIMM) ||
              (TTcurrentDimm.TTDimmType == SPD_ECC_SO_UDIMM) ||
              (TTcurrentDimm.TTDimmType == SPD_LRDIMM))) {
          dimmInTTTable = 0xFF;
        }

        TTcurrentDimm.TTDramDensity = ((*dimmNvList)[dimm].actSPDSDRAMBanks & 0x0F);
        if ((TTcurrentDimm.TTDramDensity < SPD_1Gb) || (TTcurrentDimm.TTDramDensity > SPD_16Gb)) {
          dimmInTTTable = 0xFF;
        }
        if (TTcurrentDimm.TTDDRType) {
          if (TTcurrentDimm.TTDramDensity < SPD_4Gb) TTcurrentDimm.TTDramDensity = SPD_4Gb;
        } else {
          if (TTcurrentDimm.TTDramDensity != SPD_2Gb) TTcurrentDimm.TTDramDensity = SPD_2Gb;
        }

        TTcurrentDimm.TTSpdByte7 = (*dimmNvList)[dimm].actSPDModuleOrg;

        //
        // treat x16 DIMMs same as x8
        //
        if ((TTcurrentDimm.TTSpdByte7 & 0x7) == DEVICE_WIDTH_X16) {
          TTcurrentDimm.TTSpdByte7 &= 0xF8;
          TTcurrentDimm.TTSpdByte7 |= DEVICE_WIDTH_X8;
        }

        if (((TTcurrentDimm.TTSpdByte7 & 0x7) > DEVICE_WIDTH_X8) ||
             ((TTcurrentDimm.TTSpdByte7 & 0x38) > (SPD_NUM_RANKS_8 << 3))) {
          dimmInTTTable = 0xFF;
        }
        if (TTcurrentDimm.TTDDRType && ((TTcurrentDimm.TTSpdByte7 & 0x38) == (SPD_NUM_RANKS_8 << 3)) ) dimmInTTTable = 0xFF;

        TTcurrentDimm.TTFrequency = host->nvram.mem.socket[socket].ddrFreq;
        if (TTcurrentDimm.TTDDRType) {
          if (TTcurrentDimm.TTFrequency < DDR_1600) TTcurrentDimm.TTFrequency = DDR_1600;
          if (TTcurrentDimm.TTFrequency > DDR_2400) TTcurrentDimm.TTFrequency = DDR_2400;
        } else {
          if (TTcurrentDimm.TTFrequency > DDR_1867) TTcurrentDimm.TTFrequency = DDR_1867;
        }

        TTcurrentDimm.TTDPC = (*channelNvList)[ch].maxDimm; // Available DIMMs per channel

        //
        // BIOS HSD #3614145, thermal table v0.996 default checking (basically treat UDIMM the same as RDIMM)
        //
        if ( (TTcurrentDimm.TTDimmType == SPD_UDIMM) || (TTcurrentDimm.TTDimmType == SPD_UDIMM_ECC)||(TTcurrentDimm.TTDimmType == SPD_SODIMM)||(TTcurrentDimm.TTDimmType == SPD_ECC_SO_UDIMM) ) {
          TTcurrentDimm.TTDimmType = SPD_RDIMM;
        }

        if (dimmInTTTable == 0) {
          for (TTRow = 0; TTRow <= MAX_TT_ROW; TTRow++ ) {
            ptrDimmTT = &DimmTT[TTRow];
            if ((ptrDimmTT->TTDDRType == TTcurrentDimm.TTDDRType) &&
                (ptrDimmTT->TTDimmType == TTcurrentDimm.TTDimmType) &&
                (ptrDimmTT->TTDramDensity == TTcurrentDimm.TTDramDensity) &&
                (ptrDimmTT->TTSpdByte7 == TTcurrentDimm.TTSpdByte7) &&
                (ptrDimmTT->TTFrequency == TTcurrentDimm.TTFrequency) &&
                (ptrDimmTT->TTDPC == TTcurrentDimm.TTDPC)) {
              dimmInTTTable = 1;
              break;
            }
          } // TTRow loop
        }

        dimmTempEvOfst.Data = MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG + (dimm * 4)));

        if (dimmInTTTable == 1) {
          dimmTempEvOfst.Bits.dimm_temp_offset = ptrDimmTT->TTStaticData;
        } else {
          dimmTempEvOfst.Bits.dimm_temp_offset = TT_MAX_WORST_CASE_DIMM_TEMP_OFFSET;
        }

        dimmTempEvOfst.Bits.ev_2x_ref_templo_en = 0;  //HSD s4988158 WA
        MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempEvOfst.Data);
        //
        // if CLTT is enabled, initialize the TSOD device
        //
        if (varThermalThrottlingOptions & CLTT_EN) {
          tsod.compId = MTS;
          tsod.address.controller = PROCESSOR;
          tsod.address.deviceType = DTI_TSOD;
          tsod.address.busSegment = (ch & BIT1) >> 1;
          tsod.address.strapAddress = dimm + ((ch & BIT0) * 4);
          tsod.mcId = mcId;

#ifdef SERIAL_DBG_MSG
          ReadSmb (host, socket, tsod, MTS_MFGID, (UINT8 *)&mstData);
          ReadSmb (host, socket, tsod, MTS_DID_RID, (UINT8 *)&mstData);
          ReadSmb (host, socket, tsod, MTS_CAPABILITY, (UINT8 *)&mstData);
          ReadSmb (host, socket, tsod, MTS_TEMP, (UINT8 *)&mstData);
#endif
          //
          // Initialize Tcrit to 105 - temp offset
          //
          mstData = (((105 - (UINT16)dimmTempEvOfst.Bits.dimm_temp_offset) << 4) & 0x0FFF);
          WriteSmb (host, socket, tsod, MTS_CRITICAL, (UINT8 *)&mstData);
          //
          // Initialize the config register
          //
          mstData = OEM_MTS_CONFIG_VALUE;
          WriteSmb (host, socket, tsod, MTS_CONFIG, (UINT8 *)&mstData);
        }
      } // dimm loop
    } // ch enabled
    //
    // configure the SMBCNTL register TSOD present bits based on the variable set in the above loop
    //
    if (varThermalThrottlingOptions & CLTT_EN) {
      //
      // smb_tsod_poll_en should not be set when operating in cluster mode. Cluster mode is enabled by programming
      // tsod_polling_interval to a non-zero value
      //
      smbCntl.Bits.smb_tsod_poll_en = 0;
      smbCntl.Bits.smb_dti = DTI_TSOD;
      if (ch == 1) {
        MemWritePciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG, smbCntl.Data);
      } else if (ch == 3) {
        if (Is2HA(host)) {
          MemWritePciCfgMC (host, socket, mcId, SMBCNTL_0_MC_MAIN_REG, smbCntl.Data);
        } else {
          MemWritePciCfgMC (host, socket, mcId, SMBCNTL_1_MC_MAIN_REG, smbCntl.Data);
        }
      }
      //
      // Initialize the Smbus TSOD Polling rate to 130ms
      //
      if (ch == 1) {
        smbTsodPollRate.Data = MemReadPciCfgMC (host, socket, mcId, SMB_TSOD_POLL_RATE_MC_MAIN_REG);
        smbTsodPollRate.Bits.smb_tsod_poll_rate = 0x7D0;
        MemWritePciCfgMC (host, socket, mcId, SMB_TSOD_POLL_RATE_MC_MAIN_REG, smbTsodPollRate.Data);
      } else if ((ch == 3) && Is2HA(host)) {
        smbTsodPollRate.Data = MemReadPciCfgMC (host, socket, mcId, SMB_TSOD_POLL_RATE_MC_MAIN_REG);
        smbTsodPollRate.Bits.smb_tsod_poll_rate = 0x7D0;
        MemWritePciCfgMC (host, socket, mcId, SMB_TSOD_POLL_RATE_MC_MAIN_REG, smbTsodPollRate.Data);
      }
    }

    if (varThermalThrottlingOptions & OLTT_EN) {
      smbCntl.Bits.smb_tsod_poll_en = 0;
    }

  } // ch loop
  //
  // Program DRAM RAPL parameters host->setup.mem.dramraplen == 00 - dram rapl disabled, 01 - set DRAM rapl mode 0, 02 - set DRAM rapl mode 1, else invalid
  //
  if (host->setup.mem.dramraplen) {
  raplmodeset = (host->setup.mem.dramraplen - 1) & 0x01;    // dec value by 1 to make zero base & mask off any erroneous data
    RAPLData.RAPLMode = raplmodeset;
    DramRaplProg (host, socket, &RAPLData);
  }
  //
  // Initialize MEM_TRML_ESTIMATION_CONFIG REGISTER per Romley Thermal Throttling Whitepaper v0.9
  //
  memTrmlEstimationCfgReg.Data = ReadCpuPciCfgEx (host, socket, 0, MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_REG);
  memTrmlEstimationCfgReg.Bits.ambient = 0x32;
  memTrmlEstimationCfgReg.Bits.disable_bw_estimation = 1;

    memTrmlEstimationCfgReg.Bits.disable_peci_control = 1;

  if (varThermalThrottlingOptions & CLTT_EN) {
    memTrmlEstimationCfgReg.Bits.disable_imc = 0;
  } else {
    memTrmlEstimationCfgReg.Bits.disable_imc = 1;
  }

  // Modify default values for the MEM_TRML_ESTIMATION_CONFIG register per BIOS HSD #3614142
  memTrmlEstimationCfgReg.Bits.ddr_temp_decay_factor = 0x54;
  memTrmlEstimationCfgReg.Bits.ddr_thermal_resistance = 0x50;
  // BIOS HSD #3614142 end
  WriteCpuPciCfgEx (host, socket, 0, MEM_TRML_ESTIMATION_CONFIG_PCU_FUN0_REG, memTrmlEstimationCfgReg.Data);


  //
  // Initialize DRAM VR Phase Shedding
  //
  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nInitialize DRAM Phase Shedding\n"));

  //grab DRAM power structure and use mailbox commands to set PS1 or PS2

  //
  // Use MAILBOX_BIOS_CMD_ACTIVE_VR_MASK command code (0x87) to find the active VRs
  //
  PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_ACTIVE_VR_MASK, 0);

  //
  // Read the Active DRAM VR's from the Mailbox Data Register
  //
  if (PcodeMailboxStatus == 0) {
    ActiveVrMask = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);
  }
  //
  // Check for active VRs.  If none active, skip phase shedding init
  //
  if ((ActiveVrMask & (BIT0 + BIT1)) == (BIT0 + BIT1)) {
    if (host->setup.mem.phaseShedding & PHASE_SHEDDING_EN) {
      if (IsBDXEP(host)) {
        channelNvList = &host->nvram.mem.socket[socket].channelList;
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          MemoryCurrentEstimate += DramMaxPwr[ch];
          MemoryCurrentCaculate += DramSelfRefPwr[ch];
        }
        MemoryCurrentEstimate += (MemoryCurrentEstimate >> 1); //Multiply by 1.5
        MemoryCurrentCaculate += (MemoryCurrentCaculate >> 1); //Multiply by 1.5
        MemoryCurrentEstimate = MemoryCurrentEstimate >> 3;
        MemoryCurrentCaculate = MemoryCurrentCaculate / 1000;
      } else {
      //
      // If both VRs are active, then assume CH 0 & 1 -> VR0 and CH 2 & 3 -> VR1
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {   // == SPD_VDD_125, for DDR4 only
      //2458 = 1024 * 2 * 1.2v
          MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1]) * 1024) / 2458;
        //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1]) * 100) / 120) / 1000;
      } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
         // 2765 = 1024 * 2 * 1.35v
        MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1]) * 1024) / 2765;
        //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1]) * 100) / 135) / 1000;
      } else {   // == SPD_VDD_150
        // 3072 = 1024 * 2 * 1.5v
        MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1]) * 1024) / 3072;
        //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1]) * 100) / 150) / 1000;
      }

      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "::MPM:: MemoryCurrentEstimate = %x,    MemoryCurrentCaculate = %x\n", MemoryCurrentEstimate, MemoryCurrentCaculate));
      // convert units from 1/4 watt to full watt in terms of amps
      MemoryCurrentEstimate = MemoryCurrentEstimate / 4;
      }
      // DRAM MAX Active PSI Bit[1:0]
      if (MemoryCurrentEstimate == 0) {
        phaseShedData = MAILBOX_BIOS_DATA_PS2;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "VR0 Active PSI = PS2\n"));
      } else if (MemoryCurrentEstimate < 18) {
        phaseShedData = MAILBOX_BIOS_DATA_PS1;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0 Active PSI = PS1\n"));
      } else {
        phaseShedData = MAILBOX_BIOS_DATA_PS0;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0 Active PSI = PS0\n"));
      }

      // DRAM MAX SR PSI Bit[5:4]
      if (MemoryCurrentCaculate < 5) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS2 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0 MAX SR PSI = PS2\n"));
      } else if (MemoryCurrentCaculate < 18) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS1 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0 MAX SR PSI = PS1\n"));
      } else {
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0 MAX SR PSI = PS0\n"));
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS0 << 4);
      }
    } else {
      // phase shedding is disabled, set active and SR PSI to PS0
      phaseShedData = MAILBOX_BIOS_DATA_PS0;
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "VR0 Active PSI = PS0\n"));
    }
    //
    // Write MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED command code (0x80) and DRAM0 VR (0x2) to the appropriate ps setting
    //
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED | (MAILBOX_BIOS_DRAM0_VR_ADDR << 8)), phaseShedData);
    if (host->setup.mem.phaseShedding & PHASE_SHEDDING_EN) {
      if (IsBDXEP(host)) {
        channelNvList = &host->nvram.mem.socket[socket].channelList;
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          MemoryCurrentEstimate += DramMaxPwr[ch];
          MemoryCurrentCaculate += DramSelfRefPwr[ch];
        }
        MemoryCurrentEstimate += (MemoryCurrentEstimate >> 1); //Multiply by 1.5
        MemoryCurrentCaculate += (MemoryCurrentCaculate >> 1); //Multiply by 1.5
        MemoryCurrentEstimate = MemoryCurrentEstimate >> 3;
        MemoryCurrentCaculate = MemoryCurrentCaculate / 1000;
      } else {
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {   // == SPD_VDD_125, for DDR4 only
        //2458 = 1024 * 2 * 1.2v
          MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 2458;
          //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
          MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 120) / 1000;
        } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
          // 2765 = 1024 * 2 * 1.35v
          MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 2765;
          //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
          MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 135) / 1000;
        } else {     // == SPD_VDD_150
          // 3072 = 1024 * 2 * 1.5v
          MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 3072;
          //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
          MemoryCurrentCaculate = ((((UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 150) / 1000;
        }

        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "::MPM:: MemoryCurrentEstimate = %x,    MemoryCurrentCaculate = %x\n", MemoryCurrentEstimate, MemoryCurrentCaculate));
        // convert units from 1/4 watt to full watt in terms of amps
        MemoryCurrentEstimate = MemoryCurrentEstimate / 4;
      }
      // DRAM MAX Active PSI Bit[1:0]
      if (MemoryCurrentEstimate == 0) {
        phaseShedData = MAILBOX_BIOS_DATA_PS2;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 Active PSI = PS2\n"));
      } else if (MemoryCurrentEstimate < 18) {
        phaseShedData = MAILBOX_BIOS_DATA_PS1;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 Active PSI = PS1\n"));
      } else {
        phaseShedData = MAILBOX_BIOS_DATA_PS0;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 Active PSI = PS0\n"));
      }

      // DRAM MAX SR PSI Bit[5:4]
      if (MemoryCurrentCaculate < 5) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS2 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 MAX SR PSI = PS2\n"));
      } else if (MemoryCurrentCaculate < 18) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS1 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 MAX SR PSI = PS1\n"));
      } else {
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR1 MAX SR PSI = PS0\n"));
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS0 << 4);
      }
    } else {
      // phase shedding is disabled, set active and SR PSI to PS0
      phaseShedData = MAILBOX_BIOS_DATA_PS0;
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "VR1 Active PSI = PS0\n"));
    }
    //
    // Write MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED command code (0x80) and DRAM0 VR (0x2) to the appropriate ps setting
    //
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED | (MAILBOX_BIOS_DRAM1_VR_ADDR << 8)), phaseShedData);

  } else if ((ActiveVrMask & (BIT0 + BIT1)) != 0) {
    //
    // If one VRs is present, then assume that one controls all DIMMs
    //
    if (host->setup.mem.phaseShedding & PHASE_SHEDDING_EN) {
      if (IsBDXEP(host)) {
        channelNvList = &host->nvram.mem.socket[socket].channelList;
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((*channelNvList)[ch].enabled == 0) continue;
          MemoryCurrentEstimate += DramMaxPwr[ch];
          MemoryCurrentCaculate += DramSelfRefPwr[ch];
        }
        MemoryCurrentEstimate += (MemoryCurrentEstimate >> 1); //Multiply by 1.5
        MemoryCurrentCaculate += (MemoryCurrentCaculate >> 1); //Multiply by 1.5
        MemoryCurrentEstimate = MemoryCurrentEstimate >> 3;
        MemoryCurrentCaculate = MemoryCurrentCaculate / 1000;
      } else {
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {   // == SPD_VDD_125, for DDR4 only
      //2458 = 1024 * 2 * 1.2v
          MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1] + (UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 2458;
        //A = (mW * 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((( (UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1] + (UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 120) / 1000;
      } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
        // 2765 = 1024 * 2 * 1.35v
        MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1] + (UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 2765;
        //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((( (UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1] + (UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 135) / 1000;
      } else {     // == SPD_VDD_150
        // 3072 = 1024 * 2 * 1.5v
        MemoryCurrentEstimate = (((UINT32)perChMaxMemPwr[0] + (UINT32)perChMaxMemPwr[1] + (UINT32)perChMaxMemPwr[2] + (UINT32)perChMaxMemPwr[3]) * 1024) / 3072;
        //A = (mW / 1000)/ V = ((mW  * 100) / (V * 100))/1000
        MemoryCurrentCaculate = ((( (UINT32)perChSelfRefreshPwr[0] + (UINT32)perChSelfRefreshPwr[1] + (UINT32)perChSelfRefreshPwr[2] + (UINT32)perChSelfRefreshPwr[3]) * 100) / 150) / 1000;
      }

      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "::MPM:: MemoryCurrentEstimate = %x,    MemoryCurrentCaculate = %x\n", MemoryCurrentEstimate, MemoryCurrentCaculate));

      // convert units from 1/4 watt to full watt in terms of amps
      MemoryCurrentEstimate = MemoryCurrentEstimate / 4;
      }
      // DRAM MAX Active PSI Bit[1:0]
      if (MemoryCurrentEstimate == 0) {
        phaseShedData = MAILBOX_BIOS_DATA_PS2;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 Active PSI = PS2\n"));
      } else if (MemoryCurrentEstimate < 18) {
        phaseShedData = MAILBOX_BIOS_DATA_PS1;
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 Active PSI = PS1\n"));
      } else {
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 Active PSI = PS0\n"));
        phaseShedData = MAILBOX_BIOS_DATA_PS0;
      }

      // DRAM MAX SR PSI Bit[5:4]
      if (MemoryCurrentCaculate < 5) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS2 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 MAX SR PSI = PS2\n"));
      } else if (MemoryCurrentCaculate < 18) {
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS1 << 4);
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 MAX SR PSI = PS1\n"));
      } else {
        MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR0|1 MAX SR PSI = PS0\n"));
        phaseShedData = phaseShedData | (MAILBOX_BIOS_DATA_PS0 << 4);
      }
    } else {
      // phase shedding is disabled, set active and SR PSI to PS0
      phaseShedData = MAILBOX_BIOS_DATA_PS0;
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "VR0|1 Active PSI = PS0\n"));
    }

    if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
      //
      // Write MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED command code (0x80) and the active DRAM VR to the appropriate ps setting
      // This pcode command is not supported by soft-simulation
      //
      if (ActiveVrMask & BIT1) {
        PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED | (MAILBOX_BIOS_DRAM1_VR_ADDR << 8)), phaseShedData);
      } else {
        PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_DIMM_VR_PHASE_SHED | (MAILBOX_BIOS_DRAM0_VR_ADDR << 8)), phaseShedData);
      }
    }

  } else {
    //
    //If none active, skip phase shedding init
    //
    MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "WARNING: No Active DIMM VRs - Can not enable Phase Shedding\n"));
  }
  pcuBiosSpare2Reg.Data = ReadCpuPciCfgEx (host, socket, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG);

  pcuBiosSpare2Reg.Data = pcuBiosSpare2Reg.Data & ~(BIT15 + BIT14 + BIT13 + BIT12);
  if ((host->nvram.mem.dimmTypePresent == UDIMM)||(host->nvram.mem.dimmTypePresent == SPD_SODIMM)) {
    pcuBiosSpare2Reg.Data = pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_UDIMM;
  } else if ((host->nvram.mem.dimmTypePresent == RDIMM) && (host->nvram.mem.socket[socket].lrDimmPresent == 1)) {
    pcuBiosSpare2Reg.Data =  pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_LRDIMM;
  } else {
    // default to RDIMM
    pcuBiosSpare2Reg.Data =  pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_RDIMM;
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    pcuBiosSpare2Reg.Data =  pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_125V_DIMM;
  } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
    pcuBiosSpare2Reg.Data =  pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_135V_DIMM;
  } else {
    // default to 1.5v
    pcuBiosSpare2Reg.Data =  pcuBiosSpare2Reg.Data | HSX_PCU_BIOS_SPARE2_150V_DIMM;
  }

  MemDebugPrintLocal((host, SDBG_MAX , NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
               "\n::MPM:: host->nvram.mem.dimmTypePresent = %x,    pcuBiosSpare2Reg.Data = %x\n\n", host->nvram.mem.dimmTypePresent, pcuBiosSpare2Reg.Data));

  WriteCpuPciCfgEx (host, socket, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG, pcuBiosSpare2Reg.Data);

#ifdef SERIAL_DBG_MSG
  releasePrintFControl (host);
#endif // SERIAL_DBG_MSG
  return SUCCESS;
}

UINT32
DramRaplProg (
  PSYSHOST host,
  UINT8    socket,
  DramRaplDataStruct *RAPLData
  )
/*++

  This outine will progam registers for Initializing DRAM RAPL

  @param host                        - Point to sysHost
  @param socket                      - Socket number
  @param pointer DRamRaplDataStruct  - Strucutre containing the values to be should be Programmed

  @retval SUCCESS

--*/
{

  UINT8                               ch = 0;
  UINT8                               dimm = 0;
  UINT8 numDimmsPerSocket = 0;
  UINT8                               RAPLDimmIndex = 0;
  UINT8 DimmTableIndex = 0;
  UINT32                              PcodeMailboxStatus = 0;
  DRAM_POWER_INFO_N0_PCU_FUN2_STRUCT  dramPowerInfoLow;
  DRAM_POWER_INFO_N1_PCU_FUN2_STRUCT  dramPowerInfoHigh;
  CMD_DRAM_PM_RAPL_MODE               RaplMode;
  CMD_WRITE_PWR_BW_SCALE_DATA         BwScaleData;
  CMD_DRAM_PM_WRITE_PWR_COEFF         WritePwrCoeff;
  CMD_DDR_RANKS_PRESENT               DDRRanksPresent;
  CMD_WRITE_DRAM_PBM_THRT_ADJUST_DATA PbmthrtAdjData;
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct socketNvram                  *socketNv;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  socketNv = &host->nvram.mem.socket[socket];

  // Set DDR Ranks Present
  for (ch = 0; ch < MAX_CH; ch++) {

    if ((*channelNvList)[ch].enabled == 0) continue;

    DDRRanksPresent.Data = 0x00000000;
    DDRRanksPresent.Bits.rankspresent = RAPLData->RAPLRanksPresentData[ch];

    if ((Is2HA(host)) && (ch > 1)) {

      DDRRanksPresent.Bits.channelindex = ch + 2;  //map channel 2 & 3 to 4 & 5 to indicate iMC 1
    } else {
      DDRRanksPresent.Bits.channelindex = ch;
    }

    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_DDR_RANKS_PRESENT, DDRRanksPresent.Data);
    // ProcessMbxErrorStatus(host, socket, PcodeMailboxStatus, MAILBOX_BIOS_CMD_DDR_RANKS_PRESENT);
    MemDebugPrintLocal((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
        "Issued DRAM RAPL MBX CMD DDR_RANKS_PRESENT with CMD data = 0x%08x\n ", DDRRanksPresent.Data ));

      // ....Program BW power Scale factor (s)
      BwScaleData.Data = 0x00000000;
      if ((Is2HA(host)) && (ch > 1)) {
        BwScaleData.Bits.channelindex = ch + 2;  //map channel 2 & 3 to 4 & 5 to indicate iMC 1
      } else {
        BwScaleData.Bits.channelindex = ch;
      }
      BwScaleData.Bits.selector = 0;            // indicate scale factor being programmed
      BwScaleData.Bits.cmdval = RAPLData->RAPLWritePwrScaleSval[ch];
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_WRITE_PWR_BW_SCALE, BwScaleData.Data);

      // ....Program BW power Scale Minimum Power (m)
      BwScaleData.Data = 0x00000000;
      if ((Is2HA(host)) && (ch > 1)) {
        BwScaleData.Bits.channelindex = ch + 2;  //map channel 2 & 3 to 4 & 5 to indicate iMC 1
      } else {
        BwScaleData.Bits.channelindex = ch;
      }
      BwScaleData.Bits.selector = 1;            // indicate minimum power being programmed
      BwScaleData.Bits.cmdval = RAPLData->RAPLWritePwrScaleMval;
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_WRITE_PWR_BW_SCALE, BwScaleData.Data);

      //
      // Use MAILBOX_BIOS_WRITE_DRAM_PBM_THRT_ADJUST command code (0x8A)
      //
      PbmthrtAdjData.Data = 0x00000000;
      if ((Is2HA(host)) && (ch > 1)) {
        PbmthrtAdjData.Bits.channelindex = ch + 2;  //map channel 2 & 3 to 4 & 5 to indicate iMC 1
      } else {
        PbmthrtAdjData.Bits.channelindex = ch;
      }
      PbmthrtAdjData.Bits.adjustFactor = RAPLData->RAPLDramAdjVal;
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_WRITE_DRAM_PBM_THRT_ADJUST, PbmthrtAdjData.Data);

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      numDimmsPerSocket++;

      // Calculate DIMMindex based on CH & DIMM - 0=ch0:dimm0, 1=ch0:dimm1, 2=ch0:dimm2, 3=ch1:dimm0, ..., 6=ch2:dimm0,..., 9=ch3:dimm0, ..., 0xc=ch0:dimm3, 0xb=ch1:dimm3....
      if (dimm == 3) {
        RAPLDimmIndex = 0XC + ch; // if we ever had a 4 DIMM it would be appened to the end of the Index
      } else {
        switch (ch) {
          case 0:
            RAPLDimmIndex = dimm;
            break;
          case 1:
            RAPLDimmIndex = (3 + dimm);
            break;
          case 2:
            RAPLDimmIndex = (6 + dimm);
            break;
          case 3:
            RAPLDimmIndex = (9 + dimm);
            break;
          } // end switch
      } // end else

      DimmTableIndex = RAPLDimmIndex;

      if ((Is2HA(host)) && (ch > 1)) {
        RAPLDimmIndex = RAPLDimmIndex + 6; //Convert ch 2 & 3 to MC1 index ch 0 & 1
      }

      // Set DDR_PM_WRITE_POWER_COEFF - Conversion factor
      WritePwrCoeff.Data = 0x00000000;
      WritePwrCoeff.Bits.selector = 1;        // Indicate we providing a constant (i.e. Conversion factor)
      WritePwrCoeff.Bits.index = RAPLDimmIndex;
      WritePwrCoeff.Bits.datavalue = RAPLData->RAPLDRAMPmWrPwrCOEFFValConst[DimmTableIndex];
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_DRAM_PM_WRITE_POWER_COEFF, WritePwrCoeff.Data);
      // ProcessMbxErrorStatus(host, socket, PcodeMailboxStatus, MAILBOX_BIOS_CMD_DRAM_PM_WRITE_POWER_COEFF);
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Issued DRAM RAPL MBX CMD DRAM_PM_WRITE_POWER_COEFF (Constant) with CMD data = 0x%08x\n ", WritePwrCoeff.Data ));

      // Set DDR_PM_WRITE_POWER_COEFF - Idle Power
      WritePwrCoeff.Data = 0x00000000;
      WritePwrCoeff.Bits.selector = 0;        // Idle Power
      WritePwrCoeff.Bits.index = RAPLDimmIndex;
      WritePwrCoeff.Bits.datavalue = RAPLData->RAPLDRAMPmWrPwrCOEFFValPower[DimmTableIndex];
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_DRAM_PM_WRITE_POWER_COEFF, WritePwrCoeff.Data);
      // ProcessMbxErrorStatus(host, socket, PcodeMailboxStatus, MAILBOX_BIOS_CMD_DRAM_PM_WRITE_POWER_COEFF);
      MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "Issued DRAM RAPL MBX CMD DRAM_PM_WRITE_POWER_COEFF (Idel Power) with CMD data = 0x%08x\n ", WritePwrCoeff.Data ));
    }
  }


  // Program only if socket is enabled
  if (socketNv->enabled) {

    // Set DDR_PM_RAPL_MODE
    RaplMode.Data = 0x00000000;
    RaplMode.Bits.raplmode = RAPLData->RAPLMode;
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_DRAM_RAPL_MODE, RaplMode.Data);
    MemDebugPrintLocal((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "Issued DRAM RAPL MBX CMD DRAM_RAPL_MODE with CMD data = 0x%08x\n ", RaplMode.Data ));

    // Set DDR_PM_WRITE_POWER_COEFF - Idle Power

    //
    // Program DRAM_POWER_INFO
    //
    //if (numDimmsPerSocket == 1) RAPLData->RAPLDramPwrInfoMinVal = ((RAPLData->RAPLDramPwrInfoMinVal / 2) * 3);

    if (host->setup.mem.DramRaplExtendedRange) RAPLData->RAPLDramPwrInfoMinVal = RAPLData->RAPLDramPwrInfoMinVal / 2;
    
    //HSD 5002989: Multiply MAX power and TDP by 1.5 
    if(IsBDXEP(host)) {
      RAPLData->RAPLDramPwrInfoMaxVal = RAPLData->RAPLDramPwrInfoMaxVal + (RAPLData->RAPLDramPwrInfoMaxVal >> 1);
      RAPLData->RAPLDramPwrInfoTdpVal = RAPLData->RAPLDramPwrInfoTdpVal + (RAPLData->RAPLDramPwrInfoTdpVal >> 1);
    }
    //
    //... Program lower dword
    //
    dramPowerInfoLow.Data = ReadCpuPciCfgEx (host, socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG);
    dramPowerInfoLow.Bits.dram_tdp = RAPLData->RAPLDramPwrInfoTdpVal;
    dramPowerInfoLow.Bits.dram_min_pwr = (RAPLData->RAPLDramPwrInfoMinVal / 10);
    WriteCpuPciCfgEx (host, socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG, dramPowerInfoLow.Data);

    //
    //... Program upper dword
    //
    dramPowerInfoHigh.Data = ReadCpuPciCfgEx (host, socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG);
    dramPowerInfoHigh.Bits.dram_max_pwr = RAPLData->RAPLDramPwrInfoMaxVal;
    WriteCpuPciCfgEx (host, socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG, dramPowerInfoHigh.Data);
  }


   return SUCCESS;
}

VOID
DramRaplProgCalc (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  DramRaplDataStruct *RAPLData,
  DramRaplLUTDataStruct RAPLLUTData[MAX_DIMM]
  )
/*++

  This outine will calculate the values to be programmed for DRAM RAPL based upon the looked up values

  @param host                        - Point to sysHost
  @param socket                      - Socket number
  @param ch                          - channel
  @param dimm                        - dimm number
  @param pointer DRamRaplDataStruct  - Strucutre containing the values to be should be Programmed
  @param pointer DramRaplLUTData     - Strucutre containing looked up which the calculations will be based upon

  @retval SUCCESS

--*/
{
  UINT8                                 RankPresentShift = 0x00;
  UINT16                                RankPresentCalc = 0x0000;
  UINT8                                 RAPLDimmIndex = 0;
  UINT32                                x = 0;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT       dimmMtr;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT          mcMtr;
  TCMR0SHADOW_MCDDC_CTL_HSX_BDX_STRUCT  mr0shadow;
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT         ChnTempCfgReg;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  UINT8                                 dimmNum;
  UINT16                                maxPower;
  UINT16                                minPower;
  UINT16                                tdpValue;


  channelNvList = &host->nvram.mem.socket[socket].channelList;
  ChnTempCfgReg.Data = MemReadPciCfgEp (host, socket, ch, CHN_TEMP_CFG_MCDDC_CTL_REG);

  // ...calculate DIMM index to be used for Bios2PcuMailboxCommands
  // Calculate DIMMindex based on CH & DIMM - 0=ch0:dimm0, 1=ch0:dimm1, 2=ch0:dimm2, 3=ch1:dimm0, ..., 6=ch2:dimm0,..., 9=ch3:dimm0, ..., 0xc=ch0:dimm3, 0xb=ch1:dimm3....
  if (dimm == 3) {
    RAPLDimmIndex = 0XC + ch; // if we ever had a 4 DIMM it would be appened to the end of the Index
  } else {
    switch (ch) {
      case 0:
        RAPLDimmIndex = dimm;
        break;
      case 1:
        RAPLDimmIndex = (3 + dimm);
        break;
      case 2:
        RAPLDimmIndex = (6 + dimm);
        break;
      case 3:
        RAPLDimmIndex = (9 + dimm);
        break;
    } // end switch
  } // end else

  // ...calculate DDR Ranks Present Data for Bios2PcuMailboxCommands
  switch (dimm) {
    case 2:
      RankPresentShift = 0x8;
      break;
    case 1:
      RankPresentShift = 0x4;
      break;
    case 0:
    default:
      RankPresentShift = 0x0;
      break;
  }
  dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4));
  x = dimmMtr.Bits.rank_cnt;
  switch(x) {
  case 2:
    RankPresentCalc = 0x0F;
    break;
  case 1:
    RankPresentCalc = 0x03;
    break;
  default:
  case 0:
    RankPresentCalc = 0x01;
  }
  RAPLData->RAPLRanksPresentData[ch] |= (RankPresentCalc << RankPresentShift);

  // ... Calculate CKE policy mode

  //
  // Shadow MR0 but do not set DLL_RESET
  mr0shadow.Data = MemReadPciCfgEp (host, socket, ch, TCMR0SHADOW_MCDDC_CTL_REG);
  if (mr0shadow.Bits.mr0_shadow & BIT12) {
    RAPLData->RAPL3SigmaPolicy &= ~BIT0;  // for debug purposes weare going to force CKE policy to fast for now
  }else{
    RAPLData->RAPL3SigmaPolicy |= BIT0;   // Indicate CKE slow
  }

  // ... Set Page mode policy
  mcMtr.Data = MemReadPciCfgMC (host, socket, 0, MCMTR_MC_MAIN_REG);
  if (mcMtr.Bits.close_pg == 1) {
    RAPLData->RAPL3SigmaPolicy|= BIT1;    // Indicate close page (i.e. BIT1=1)
  }else{
    RAPLData->RAPL3SigmaPolicy &= ~BIT1;    // Indicate open page (i.e. BIT1=0)
  }

  // Set DRAM Adj Value
  RAPLData->RAPLDramAdjVal = 0x80;

  // Set Write Power Scale (m) value
  RAPLData->RAPLWritePwrScaleMval = 0x01;

  //
  // Initialize DRAM RAPL with values from lookup table
  //

  if (dimm == (((*channelNvList)[ch].maxDimm) - 1)) {
    maxPower = 0;
    minPower = 0;
    tdpValue = 0;
    for (dimmNum = 0 ; dimmNum < (*channelNvList)[ch].maxDimm ; dimmNum++) {

      if (RAPLData->RAPL3SigmaPolicy & BIT1) {
        // Init DramMaxPwr
        maxPower = maxPower + RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMaxPwr);
        // Init DramTdp
        tdpValue =  tdpValue + RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramTdp);

        if (ChnTempCfgReg.Bits.thrt_allow_isoch == 1) {
          if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
            minPower = minPower + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrClosedPage) * 13);
          } else {
            minPower = minPower + 24 + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrClosedPage) * 12);
          }
        } else {
          minPower = minPower + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrClosedPage) * 10);
        }
      } else {
        // Init DramMaxPwr
        maxPower = maxPower + RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMaxPwrOpenPage);
        // Init DramTdp
        tdpValue =  tdpValue + RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramTdpOpenPage);

        if (ChnTempCfgReg.Bits.thrt_allow_isoch == 1) {
          if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
            minPower = minPower + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrOpenPage) * 13);
          } else {
            minPower = minPower + 24 + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrOpenPage) * 12);
          }
        } else {
          minPower = minPower + (RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimmNum].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimmNum].DramRaplDataDramMinPwrOpenPage) * 10);
        }
      }
    } //dimm loop

    RAPLData->RAPLDramPwrInfoMaxVal = RAPLData->RAPLDramPwrInfoMaxVal + (maxPower / (*channelNvList)[ch].maxDimm);
    RAPLData->RAPLDramPwrInfoTdpVal = RAPLData->RAPLDramPwrInfoTdpVal + (tdpValue / (*channelNvList)[ch].maxDimm);
    RAPLData->RAPLDramPwrInfoMinVal = RAPLData->RAPLDramPwrInfoMinVal + (minPower / (*channelNvList)[ch].maxDimm);
  }
  // Get power scale (Write Power Scale (S) val) value based upon page mode
  if (RAPLData->RAPL3SigmaPolicy & BIT1) {
    RAPLData->RAPLWritePwrScaleSval[ch] = RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleClosedPage;
  } else {
    RAPLData->RAPLWritePwrScaleSval[ch] = RAPLLUTData[dimm].DramRaplDataDramWritePwrScaleOpenPage;
  }

  // Get DDR_PM_WRITE_POWER_COEFF Constant value
  RAPLData->RAPLDRAMPmWrPwrCOEFFValConst[RAPLDimmIndex] = RAPLLUTData[dimm].DramRaplDataDramPmWritePowerCoeffCval;
  // Calculate DDR_PM_WRITE_POWER_COEFF min Idle power value based on page policy
  if (RAPLData->RAPL3SigmaPolicy & BIT1) {
    RAPLData->RAPLDRAMPmWrPwrCOEFFValPower[RAPLDimmIndex] = RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimm].DramRaplDataDramMinPwrClosedPage);
  } else {
    RAPLData->RAPLDRAMPmWrPwrCOEFFValPower[RAPLDimmIndex] = RefreshScaledPwr(host, socket, ch, RAPLLUTData[dimm].DramRaplDataDramRefreshRateSlope, RAPLLUTData[dimm].DramRaplDataDramMinPwrOpenPage);
  }
}

UINT16
RefreshScaledPwr(
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT32   slope,
  UINT16   powerInput
  )
/*++

  This outine will scale the calculated power values to be programmed for DRAM RAPL based upon the looked up values
  based on the refresh rate and a linear scaling factor from looked up values.

  @param host          - Point to sysHost
  @param powerInput    - Unscaled power value

  @retval Scaled power value

--*/
{
  UINT16 scaledPower;
  UINT32 RefreshRate;
  UINT32 RefreshAdder = 0;
  struct channelNvram   (*channelNvList)[MAX_CH];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  scaledPower = powerInput;

  //
  // If refresh rate is >1x, then scale DRAM power calculations
  //
  RefreshRate = (UINT32) (*channelNvList)[ch].twoXRefreshSetPerCh;

  if ((host->setup.mem.dramraplRefreshBase >= 10) && (RefreshRate > host->setup.mem.dramraplRefreshBase) ) {
    RefreshAdder = (((UINT32)slope * (RefreshRate - host->setup.mem.dramraplRefreshBase)) / 10000);
    scaledPower = powerInput + (UINT16)RefreshAdder;
  }

  return scaledPower;
}

