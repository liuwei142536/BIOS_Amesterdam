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
 *
 ************************************************************************/
#include <Token.h> // AptioV server override
#include "SysFunc.h"

/**

  Programs input parameters for the MRC

  @param host - Pointer to the host structure

  @retval N/A

**/
void
GetMemSetupOptions (
  PSYSHOST host
  )
{
  UINT8 socket;
  UINT8 ch;
  struct sysSetup *setup;
  setup = (struct sysSetup *) &host->setup;

  //
  // RASmode
  //  CH_INDEPENDENT
  //  CH_MIRROR
  //  CH_LOCKSTEP
  //  RK_SPARE
  //
  setup->mem.RASmode = CH_INDEPENDENT;

  //
  //  RASmodeEx
  //    DMNDSCRB_EN
  //    PTRLSCRB_EN
  //    DEVTAGGING_EN
  //    A7_MODE_EN
  //
  setup->mem.RASmodeEx = 0;
  setup->mem.RASmodeEx |= PTRLSCRB_EN;
  setup->mem.RASmodeEx |= DMNDSCRB_EN;
  setup->mem.RASmodeEx &= ~DEVTAGGING_EN;

  if (A7_MODE_DEFAULT == A7_MODE_ENABLE) {
    setup->mem.RASmodeEx |= A7_MODE_EN;
  } else {
    setup->mem.RASmodeEx &= ~A7_MODE_EN;
  }



  //
  //  options
  //    TEMPHIGH_EN
  //    PAGE_POLICY
  //    ALLOW2XREF_EN
  //    ADAPTIVE_PAGE_EN
  //    SCRAMBLE_EN
  //    NUMA_AWARE
  //    ECC_CHECK_EN
  //    DISABLE_WMM_OPP_RD
  //    BALANCED_4WAY_EN
  //    CLTT_EN
  //    OLTT_EN
  //    SPLIT_BELOW_4GB_EN
  //    RAS_TO_INDP_EN
  //    CLTT_TEST_EN
  //    MARGIN_RANKS_EN
  //
#ifdef XMP_SUPPORT
  //
  //    MEM_OVERRIDE_EN
  //
  setup->mem.options &= ~MEM_OVERRIDE_EN;
  setup->mem.inputMemTime.nCL = 0;
  setup->mem.inputMemTime.nRP = 0;
  setup->mem.inputMemTime.nRCD = 0;
  setup->mem.inputMemTime.nRRD = 0;
  setup->mem.inputMemTime.nRRD_L = 0;
  setup->mem.inputMemTime.nWTR = 0;
  setup->mem.inputMemTime.nRAS = 0;
  setup->mem.inputMemTime.nRTP = 0;
  setup->mem.inputMemTime.nWR = 0;
  setup->mem.inputMemTime.nFAW = 0;
  setup->mem.inputMemTime.nCWL = 0;
  setup->mem.inputMemTime.nRC = 0;
  setup->mem.inputMemTime.nCMDRate = 0;
  setup->mem.inputMemTime.tREFI = 0;
  setup->mem.inputMemTime.nRFC = 0;
#endif
  //
  //    DRAMDLL_OFF_PD_EN
  //
  setup->mem.options = 0;
  setup->mem.thermalThrottlingOptions = (OLTT_EN | CLTT_EN);
  setup->mem.thermalThrottlingOptions &= ~(MH_OUTPUT_EN | MH_SENSE_EN);
  setup->mem.electricalThrottling = ET_AUTO;
  setup->mem.altitude = UNKNOWN;

  setup->mem.thermalThrottlingOptions |= MH_SENSE_EN;

  //
  //    DRAM RAPL Init
  //
  setup->mem.dramraplen = 2;   // enable DRAM RAPL baseline in mode 1
  setup->mem.dramraplbwlimittf = 1;
  setup->mem.dramraplRefreshBase = DRAM_RAPL_REFRESH_BASE_DEFAULT;
  setup->mem.DramRaplExtendedRange = DRAM_RAPL_EXTENDED_RANGE_DEFAULT;
  setup->mem.customRefreshRate = 0;       // Disable by default

  //
  // DRAM Maintenance
  //
  setup->mem.dramMaint        = DRAM_MAINT_AUTO;
  setup->mem.dramMaintMode    = DRAM_MAINT_TRR;
  setup->mem.dramMaintTRRMode = DRAM_MAINT_MODE_A;

  //
  // Data Scrambling
  //
  if (SCRAMBLE_DEFAULT == SCRAMBLE_ENABLE) {
     setup->mem.options |= SCRAMBLE_EN;
     setup->mem.scrambleSeedLow = SCRAMBLE_SEED_LOW;
     setup->mem.scrambleSeedHigh = SCRAMBLE_SEED_HIGH;
  }

  // ECC Support
  if (ECC_DEFAULT) setup->mem.options |= ECC_CHECK_EN;

  // ECC-NonECC Mix dimms
  setup->mem.options &= ~ECC_MIX_EN;

  // Enable WMM Opportunistic Reads
  if (OPP_RD_WMM_DEFAULT == OPP_RD_WMM_EN) {
    setup->mem.options &= ~DISABLE_WMM_OPP_READ;
  }

  // Setup option for memory test
  if (MEM_TEST_DEFAULT == MEM_TEST_EN) {
    setup->mem.options |= MEMORY_TEST_EN;
  }

  // Setup option for memory test when fast boot
  if (MEM_TEST_FAST_BOOT_DEFAULT == MEM_TEST_FAST_BOOT_EN) {
    setup->mem.options |= MEMORY_TEST_FAST_BOOT_EN;
  }

  // Setup option for fast boot
  if (FAST_BOOT_DEFAULT == FAST_BOOT_EN) {
    setup->mem.options |= ATTEMPT_FAST_BOOT;
  }

  // Setup option for fast cold boot
  if (FAST_BOOT_COLD_DEFAULT == FAST_BOOT_COLD_EN) {
    setup->mem.options |= ATTEMPT_FAST_BOOT_COLD;
  }

  //
  // Advanced Memory Test
  //
  setup->mem.advMemTestEn = ADV_MEM_TEST_DEFAULT;
  setup->mem.advMemTestOptions = \
//    ADV_MT_XMATS8 |
//    ADV_MT_XMATS16 |
    ADV_MT_XMATS32 |
//    ADV_MT_XMATS64 |
//    ADV_MT_WCMATS8 |
//    ADV_MT_WCMCH8 |
//    ADV_MT_GNDB64 |
    ADV_MT_MARCHCM64;

  //
  // Memory test loops
  //
  setup->mem.memTestLoops = MEM_TEST_LOOPS_DEFAULT;

  setup->mem.DramMaintenanceTest = 0;
  setup->mem.DMTDir        = 0;
  setup->mem.DMTInv        = 0;
  setup->mem.DMTRep        = 3;
  setup->mem.DMTIter       = 1;
  setup->mem.DMTSwizzleEn  = 0;
  setup->mem.DMRefreshEn   = 1;

  //
  // Phase Shedding
  //
  if (PHASE_SHEDDING_DEFAULT == PHASE_SHEDDING_EN) {
    setup->mem.phaseShedding = PHASE_SHEDDING_EN;
  } else {
    setup->mem.phaseShedding = PHASE_SHEDDING_DIS;
  }


  // Page mode
  if (CLOSED_PAGE_DEFAULT == CLOSED_PAGE_DIS) {
    // Open Fixed
    setup->mem.options &= ~PAGE_POLICY;
    setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else if (CLOSED_PAGE_DEFAULT == CLOSED_PAGE_EN) {
    // Closed
    setup->mem.options |= PAGE_POLICY;
    setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else if (CLOSED_PAGE_DEFAULT == OPEN_PAGE_ADAPTIVE) {
    // Open Adaptive
    setup->mem.options &= ~PAGE_POLICY;
    setup->mem.options |= ADAPTIVE_PAGE_EN;
  }

#ifdef MARGIN_CHECK
  //
  // Rank Margin Tool
  //
  if (RMT_DEFAULT == RMT_EN) {
    setup->mem.options |= MARGIN_RANKS_EN;
  }
#endif  // MARGIN_CHECK

  //
  // CKE Throttling
  //
  setup->mem.MemPwrSave = MEM_PWR_SAV_AUTO;
  setup->mem.ckeThrottling = CKE_MODE_DEFAULT;

  //
  // CK/CK# OFF for Self Refresh
  //
  setup->mem.pdwnCkMode = CK_SELF_REF_AUTO;

  //
  // Opportunistic Self-Refresh
  //
  if (OPP_SR_DEFAULT) {
     setup->mem.options |= OPP_SELF_REF_EN;
  } else {
     setup->mem.options &= ~OPP_SELF_REF_EN;
  }

  //
  // MDLL Shut Down Enable
  //

  if (MDLL_SDEN_DEFAULT == MDLL_SDEN_DIS) {
    setup->mem.options &= ~MDLL_SHUT_DOWN_EN;
  }else {
    setup->mem.options |= MDLL_SHUT_DOWN_EN;
  }

  //
  // number of sparing transactions and Normal operation interval.
  //
  setup->mem.normOppIntvl = NORMAL_OPPERATION_INTERVAL;
  setup->mem.numSparTrans = SPARING_TRANS_INTERVAL;

  // MCODT_50/100 ohms
  setup->mem.setMCODT = MC_ODT_AUTO;

  //
  // High Temp Enable
  //
  if (HTE_DEFAULT) {
     setup->mem.options |= TEMPHIGH_EN;
  } else {
     setup->mem.options &= ~TEMPHIGH_EN;
  }

  //
  // Allow 2x Refresh
  //
  if (A2R_DEFAULT) {
     setup->mem.options |= ALLOW2XREF_EN;
  } else {
     setup->mem.options &= ~ALLOW2XREF_EN;
  }

  //
  // C/A Parity
  //
  if (CA_PARITY_DEFAULT == CA_PARITY_ENABLE){
    setup->mem.options |= CA_PARITY_EN;
  } else {
    setup->mem.options &= ~CA_PARITY_EN;
  }

  //
  //  ddrFreqLimit
  //    0:Auto
  //    1:Force DDR-800
  //    2:Force DDR-1066
  //    3:Force DDR-1333
  //    4:Force DDR-1600
  //    5:Force DDR-1867
  //    6:Force DDR-2133
  //
  setup->mem.ddrFreqLimit = DDR3_FREQ_DEFAULT;
  setup->mem.enforcePOR = ENFORCE_POR_DEFAULT;

  //
  // Force socket interleave to 1 if NUMA enabled
  //
  if (NUMA_DEFAULT == NUMA_EN) {
    setup->mem.socketInter = 1;
  } else {
    // Maximum of 2 way interleaving is supported
    setup->mem.socketInter = 2;
  }

  if (setup->mem.socketInter == 1) {
    setup->mem.options |= NUMA_AWARE;
  } else {
    setup->mem.options &= ~NUMA_AWARE;
  }

  //
  //  chInter
  //    0:Auto
  //    1:1-way channel interleave
  //    2:2-way channel interleave
  //    3:3-way channel interleave
  //    4:4-way channel interleave
  //
  setup->mem.chInter = CH_INTER_DEFAULT;

  //
  //  chInter
  //    0:Auto
  //    1:1-way rank interleave
  //    2:2-way rank interleave
  //    4:4-way rank interleave
  //    8:8-way rank interleave
  //
  setup->mem.rankInter = RANK_INTER_DEFAULT;

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  // Size requested for ME UMA, 0 by default
  //
  setup->mem.meRequestedSize = 0x0;

#endif // ME_SUPPORT_FLAG

  //
  // Multi-Threaded MRC
  //
  if (MULTI_THREADED_DEFAULT == MULTI_THREADED_EN) {
     setup->mem.options |= MULTI_THREAD_MRC_EN;
  } else {
     setup->mem.options &= ~MULTI_THREAD_MRC_EN;
  }

  //
  // Cross Over Calibration
  //
  if (XOVER_CALIB_DEFAULT == XOVER_CALIB_EN) {
     setup->mem.optionsExt |= XOVER_EN;
  } else {
     setup->mem.optionsExt &= ~XOVER_EN;
  }

  //
  // Allow SBE during memory training
  //
  if ( ALLOW_CORRECTABLE_ERROR_DEFAULT == ALLOW_CORRECTABLE_ERROR_ENABLE) {
     setup->mem.optionsExt |= ALLOW_CORRECTABLE_ERROR;
  } else {
     setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_ERROR;
  }

  //
  // Sense Amp Training
  //
  if (SENSE_AMP_DEFAULT == SENSE_AMP_EN) {
     setup->mem.optionsExt |= SENSE_EN;
  } else {
     setup->mem.optionsExt &= ~SENSE_EN;
  }

  //
  // Late CMD/CLK Training
  //
  if (CMD_CLK_DEFAULT == CMD_CLK_EN) {
     setup->mem.options |= CMD_CLK_TRAINING_EN;
  } else {
     setup->mem.options &= ~CMD_CLK_TRAINING_EN;
  }

  //
  // Early CMD/CLK Training
  //
  if (EARLY_CMD_CLK_DEFAULT == EARLY_CMD_CLK_EN) {
     setup->mem.optionsExt |= EARLY_CMD_CLK_TRAINING_EN;
  } else {
     setup->mem.optionsExt &= ~EARLY_CMD_CLK_TRAINING_EN;
  }

  //
  // Early CTL/CLK Training
  //
  if (EARLY_CTL_CLK_DEFAULT == EARLY_CTL_CLK_EN) {
     setup->mem.optionsExt |= EARLY_CTL_CLK_TRAINING_EN;
  } else {
     setup->mem.optionsExt &= ~EARLY_CTL_CLK_TRAINING_EN;
  }

  //
  // CMD Normalization
  //
  if (IsBDXEP(host)) {
    if (CMD_NORMAL_DEFAULT == CMD_NORMAL_ENABLE) {
      setup->mem.optionsExt |= CMD_NORMAL_EN;
    } else {
      setup->mem.optionsExt &= ~CMD_NORMAL_EN;
    }
  } else {
    setup->mem.optionsExt &= ~CMD_NORMAL_EN;
  }
  //
  // Round Trip Latency
  //
  if (ROUND_TRIP_LATENCY_DEFAULT == ROUND_TRIP_LATENCY_ENABLE) {
     setup->mem.optionsExt |= ROUND_TRIP_LATENCY_EN;
  } else {
     setup->mem.optionsExt &= ~ROUND_TRIP_LATENCY_EN;
  }

  //
  // Duty Cycle Training
  if (DUTY_CYCLE_AUTO == DUTY_CYCLE_ENABLE){
    setup->mem.optionsExt |= DUTY_CYCLE_EN;
  } else {
    setup->mem.optionsExt &= ~DUTY_CYCLE_EN;
  }

  //
  // Read Vref Training
  //
  if (RX_VREF_DEFAULT == RX_VREF_ENABLE) {
     setup->mem.optionsExt |= RD_VREF_EN;
  } else {
     setup->mem.optionsExt &= ~RD_VREF_EN;
  }

  //
  // PDA
  //
  if (PDA_DEFAULT == PDA_ENABLE) {
     setup->mem.optionsExt |= PDA_EN;
  } else {
     setup->mem.optionsExt &= ~PDA_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if (TURNAROUND_DEFAULT == TURNAROUND_ENABLE) {
     setup->mem.optionsExt |= TURNAROUND_OPT_EN;
  } else {
     setup->mem.optionsExt &= ~TURNAROUND_OPT_EN;
  }
  //
  // IOT MEMORY BUFFER RESERVATION
  //
  setup->mem.iotMemBufferRsvtn = IOT_BUFFER_DEFAULT;

  //
  // THREE MONTH TIMEOUT FOR MEMORY RE-TRAINING
  //
  setup->mem.enforceThreeMonthTimeout = ENFORCE_TIMEOUT_DEFAULT;

#ifdef SSA_FLAG
  //
  // Enabling BIOS SSA
  //
  setup->mem.enableBiosSsaLoader = BIOS_SSA_DEFAULT;
#endif

#ifdef LRDIMM_SUPPORT
  //
  // Disable Backside RMT
  //
  setup->mem.enableBacksideRMT = BACKSIDE_RMT_DEFAULT;
#endif

  //
  // Write Vref Training
  //
  if (TX_VREF_DEFAULT == TX_VREF_ENABLE) {
     setup->mem.optionsExt |= WR_VREF_EN;
  } else {
     setup->mem.optionsExt &= ~WR_VREF_EN;
  }

  //
  // Command Vref Training(BDX Only)
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
      if (CMD_VREF_DEFAULT == CMD_VREF_ENABLE) {
        setup->mem.optionsExt |= CMD_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~CMD_VREF_EN;
      }
  } else { //HSX // Always disabled for HSX
    setup->mem.optionsExt &= ~CMD_VREF_EN;
  }

  //
  // LRDIMM Backside Vref Training
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (IsBDXEP(host)) {
      if (LRDIMM_BACKSIDE_VREF_DEFAULT_BDX == LRDIMM_BACKSIDE_VREF_ENABLE) {
        setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
      } else {
        setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
      }
    } else {// DE
      setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
   }
  } else { //HSX
    if (LRDIMM_BACKSIDE_VREF_DEFAULT == LRDIMM_BACKSIDE_VREF_ENABLE) {
      setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
    } else {
      setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
    }
  }

  //
  //  LRDIMM WR VREF
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (LRDIMM_WR_VREF_DEFAULT == LRDIMM_WR_VREF_ENABLE) {
      setup->mem.optionsExt |= LRDIMM_WR_VREF_EN;
    } else {
      setup->mem.optionsExt &= ~LRDIMM_WR_VREF_EN;
    }
  } else { // Always disabled for HSX
    setup->mem.optionsExt &= ~LRDIMM_WR_VREF_EN;
  }

  //
  //  LRDIMM RD VREF
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (LRDIMM_RD_VREF_DEFAULT == LRDIMM_RD_VREF_ENABLE) {
      setup->mem.optionsExt |= LRDIMM_RD_VREF_EN;
    } else {
      setup->mem.optionsExt &= ~LRDIMM_RD_VREF_EN;
    }
  } else { // Always disabled for HSX
    setup->mem.optionsExt &= ~LRDIMM_RD_VREF_EN;
  }

  //
  //  LRDIMM RX DQ CENTERING
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (LRDIMM_RX_DQ_CENTERING_DEFAULT == LRDIMM_RX_DQ_CENTERING_ENABLE) {
      setup->mem.optionsExt |= LRDIMM_RX_DQ_CENTERING;
    } else {
      setup->mem.optionsExt &= ~LRDIMM_RX_DQ_CENTERING;
    }
  } else { // Always disabled for HSX
    setup->mem.optionsExt &= ~LRDIMM_RX_DQ_CENTERING;
  }

  //
  //  LRDIMM TX DQ CENTERING
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (LRDIMM_TX_DQ_CENTERING_DEFAULT == LRDIMM_TX_DQ_CENTERING_ENABLE) {
      setup->mem.optionsExt |= LRDIMM_TX_DQ_CENTERING;
    } else {
      setup->mem.optionsExt &= ~LRDIMM_TX_DQ_CENTERING;
    }
  } else { // Always disabled for HSX
    setup->mem.optionsExt &= ~LRDIMM_TX_DQ_CENTERING;
  }

  //
  // DRAM RON Training
  //
  if (DRAM_RON_DEFAULT == DRAM_RON_ENABLE) {
    setup->mem.optionsExt |= DRAM_RON_EN;
  } else {
    setup->mem.optionsExt &= ~DRAM_RON_EN;
  }

  //
  // RX ODT Training
  //
  if (RX_ODT_DEFAULT == RX_ODT_ENABLE) {
    setup->mem.optionsExt |= RX_ODT_EN;
  } else {
    setup->mem.optionsExt &= ~RX_ODT_EN;
  }

  //
  // RTT WR Training
  //
  if (RTT_WR_DEFAULT == RTT_WR_ENABLE) {
    setup->mem.optionsExt |= RTT_WR_EN;
  } else {
    setup->mem.optionsExt &= ~RTT_WR_EN;
  }

  //
  // MC RON Training
  //
  if (MC_RON_DEFAULT == MC_RON_ENABLE) {
    setup->mem.optionsExt |= MC_RON_EN;
  } else {
    setup->mem.optionsExt &= ~MC_RON_EN;
  }
  //
  // TxEq Calibration
  //
  if (host->var.common.cpuType == CPU_BDX) { //BDX
    if (TX_EQ_DEFAULT== TX_EQ_ENABLE) {
      setup->mem.optionsExt |= TX_EQ_EN;
    } else {
      setup->mem.optionsExt &= ~TX_EQ_EN;
    }
  } else { //HSX
    if (TX_EQ_DEFAULT_HSX == TX_EQ_ENABLE) {
      setup->mem.optionsExt |= TX_EQ_EN;
    } else {
      setup->mem.optionsExt &= ~TX_EQ_EN;
    }
  }

  //
  // IMode training
  //
  if (host->var.common.cpuType == CPU_BDX) { //BDX
    if (IMODE_DEFAULT == IMODE_ENABLE) {
      setup->mem.optionsExt |= IMODE_EN;
    } else {
      setup->mem.optionsExt &= ~IMODE_EN;
    }
  } else { //HSX
    if (IMODE_DEFAULT_HSX == IMODE_ENABLE) {
      setup->mem.optionsExt |= IMODE_EN;
    } else {
      setup->mem.optionsExt &= ~IMODE_EN;
    }
  }

  //
  // RX CTLE Training
  //
  if (host->var.common.cpuType == CPU_BDX) { //BDX
    if (RX_CTLE_DEFAULT == RX_CTLE_ENABLE) {
      setup->mem.optionsExt |= RX_CTLE_TRN_EN;
    } else {
      setup->mem.optionsExt &= ~RX_CTLE_TRN_EN;
    }
  } else { //HSX
    setup->mem.optionsExt &= ~RX_CTLE_TRN_EN;
  }

  //
  // Per bit deskew
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (IsBDXEP(host)) { //ML
      if (BIT_DESKEW_DEFAULT_BDX == BIT_DESKEW_ENABLE) {
        setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
      } else {
        setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
      }
    } else {// DE - Enable for V0 and above
      if ((BIT_DESKEW_DEFAULT_DE == BIT_DESKEW_ENABLE) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX)) {
        setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
      } else {
        setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
      }
    }
  } else { //HSX
    if (BIT_DESKEW_DEFAULT == BIT_DESKEW_ENABLE) {
      setup->mem.perBitDeSkew = BIT_DESKEW_ENABLE;
    } else {
      setup->mem.perBitDeSkew = BIT_DESKEW_DISABLE;
    }
  }

  //
  // DDR4 SMBus Speed
  //
  setup->mem.SpdSmbSpeed = SMB_CLK_DEFAULT;

  //
  // Bank XOR support
  //
  if (BANK_XOR_DEFAULT == BANK_XOR_ENABLE) {
    setup->mem.options |= BANK_XOR_EN;
  } else {
    setup->mem.options &= ~BANK_XOR_EN;
  }


  // CECC WA Enable
  if (host->var.common.cpuType == CPU_BDX) { //BDX
    if (CECCWA_DEFAULT == CECCWA_AUTO) {
      if ((IsBDXEP(host) && ((IsMccChop(host) && CheckSteppingLessThan(host, CPU_BDX, B0_REV_BDX))))
          || (IsBDXEP(host) && CheckSteppingGreaterThan(host, CPU_BDX, A0_REV_BDX)) 
          || (IsDESku(host) && CheckSteppingGreaterThan(host, CPU_BDX, C0_REV_BDX))) { 
        setup->mem.optionsExt &= ~CECC_WA;
      } else {
        setup->mem.optionsExt |= CECC_WA;
      }
    } else if (CECCWA_DEFAULT == CECCWA_ENABLE) {
      setup->mem.optionsExt |= CECC_WA;
    } else {
      setup->mem.optionsExt &= ~CECC_WA; 
    }
  } else {// HSX
    setup->mem.optionsExt &= ~CECC_WA;
  }

  //CAP ERROR FLOW 
  if (CAP_ERR_FLOW_DEFAULT == CAP_ERR_FLOW_ENABLE) {
    setup->mem.optionsExt |= CAP_ERR_FLOW;  
  } else {
    setup->mem.optionsExt &= ~CAP_ERR_FLOW;
  }

#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
  if (WR_CRC_DEFAULT == WR_CRC_ENABLE) {
    setup->mem.optionsExt |= WR_CRC;
  }else {
    setup->mem.optionsExt &= ~WR_CRC;
  }
#endif

  //DDR4_PLATFORM
  if (DDR4_PLATFORM_DEFAULT == DDR4_PLATFORM_ENABLE) {
    setup->mem.optionsExt |= DDR4_PLATFORM;
  }else {
    setup->mem.optionsExt &= ~DDR4_PLATFORM;
  }

  //
  // Enhanced Log parsing support
  //
  setup->mem.logParsing = LOG_PARSING_DIS;
  //
  // Enhanced Log parsing support
  //
  setup->mem.logParsing = LOG_PARSING_DIS;
  //
  // Periodic RCOMP type
  //
  if (host->var.common.cpuType == CPU_BDX) { //ML
    if (RCOMP_TYPE_DEFAULT_BDX == RCOMP_TYPE_HW) {
    setup->mem.optionsExt |= RCOMP_TYPE;
  } else {
    setup->mem.optionsExt &= ~RCOMP_TYPE;
  }
  } else {
    if (RCOMP_TYPE_DEFAULT == RCOMP_TYPE_HW) {
      setup->mem.optionsExt |= RCOMP_TYPE;
    } else {
      setup->mem.optionsExt &= ~RCOMP_TYPE;
    }
  }
  //
  //  Per Bit Margin Data.
  //
  if (host->var.common.cpuType == CPU_BDX) {//BDX
    if (IsBDXEP(host)) { //ML
      if (PER_BIT_MARGIN_DEFAULT_BDX == PER_BIT_MARGIN_ENABLE) {
        setup->mem.optionsExt |= PER_BIT_MARGINS;
      } else {
        setup->mem.optionsExt &= ~PER_BIT_MARGINS;
      }
    } else {// DE - Enable for V0 and above
      if ((PER_BIT_MARGIN_DEFAULT_DE == PER_BIT_MARGIN_ENABLE) && CheckSteppingGreaterThan(host, CPU_BDX, B0_REV_BDX)) {
        setup->mem.optionsExt |= PER_BIT_MARGINS;
      } else {
        setup->mem.optionsExt &= ~PER_BIT_MARGINS;
      }
    }
  } else { //HSX
    if (PER_BIT_MARGIN_DEFAULT == PER_BIT_MARGIN_ENABLE) {
      setup->mem.optionsExt |= PER_BIT_MARGINS;
    } else {
      setup->mem.optionsExt &= ~PER_BIT_MARGINS;
    }
  }

  //
  //  dimmTypeSupport
  //    0:RDIMM
  //    1:UDIMM
  //    2:RDIMM and UDIMM
  //
  setup->mem.dimmTypeSupport = MEM_TYPE_AUTO;

  //
  //  lrdimmModuleDelay
  //    0:Auto
  //    1:Disable
  //
  setup->mem.lrdimmModuleDelay = MODULE_DELAY_AUTO;

  //
  //  spareErrTh
  //
  setup->mem.spareErrTh = 0x7FFF;

  //
  //  patrolScrubDuration
  //    Patrol Scrub duration defaults to 24 hours
  //
  setup->mem.patrolScrubDuration = PATROL_SCRUB_DURATION_DEFAULT;

  //
  // Leaky bucket values (1 leak per 10 days for 1GHz DCLK)
  //
  setup->mem.leakyBktLo = LEAKY_BUCKET_LO;
  setup->mem.leakyBktHi = LEAKY_BUCKET_HI;

  //
  // DLL Reset Test
  //
  setup->mem.dllResetTestLoops = DLL_RESET_TEST_LOOPS;

#ifdef MARGIN_CHECK
  //
  //  rmtPatternLength
  //    Pattern length for the rank margin tool.  One pattern = the size of the Write Data Buffer
  //
  setup->mem.rmtPatternLength = RMT_PATTERN_LENGTH;
  setup->mem.rmtPatternLengthExt = CMD_PATTERN_LENGTH;
#endif  // MARGIN_CHECK

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Enable the memory controller
    //
    setup->mem.socket[socket].enabled = 1;

    //
    // Set options
    //
    setup->mem.socket[socket].options = 0;

    //
    //  ddrVddLimit
    //    0:Auto
    //    1:Force 1.5v
    //    2:Force 1.35v
    //
    setup->mem.socket[socket].ddrVddLimit = DDR3_VDD_AUTO;

//#ifndef HW_EMULATION
    for (ch = 0; ch < MAX_CH; ch++) {
#ifdef DE_SKU
      //
      // For DE, skip CH2 and 3
      //
      if (IsDESku(host) && (ch > 1)) {
        setup->mem.socket[socket].ddrCh[ch].enabled = 0;
        continue;
      }
#endif
      //
      // Enable this channel
      //
      setup->mem.socket[socket].ddrCh[ch].enabled = 1;
      //
      // MAX DIMM slots available per channel
      //
      setup->mem.socket[socket].ddrCh[ch].numDimmSlots = MAX_DIMM;
      //
      // 1 DIMM vref controller per 2 channels
      //
      if (ch & BIT0) {
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = DCP_ISL9072X;
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.controller    = PROCESSOR;
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.strapAddress  = 0x06;
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.deviceType    = DTI_DCP_FIVE;
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.address.busSegment = (ch & BIT1) >> 1;
      } else {
        setup->mem.socket[socket].ddrCh[ch].vrefDcp.compId = NO_DEV;
      }

      setup->mem.socket[socket].ddrCh[ch].rankmask = 0xFF;

    } // ch loop

  } // socket loop

  // ADR and NVDIMM related setup options.
#ifdef MEM_NVDIMM_EN
  setup->mem.ADREn = ADR_NVDIMM;; // Enable ADR for NVDIMM usage.
  setup->mem.eraseArmNVDIMMS = 1;
  setup->mem.restoreNVDIMMS = 1;
  setup->mem.check_platform_detect = 0;
#else
  setup->mem.ADREn = 0;
#endif
#ifdef DE_SKU
  setup->mem.BlockScTrafficOnAdr = 0;
#endif

#ifdef NVMEM_FEATURE_EN
  setup->mem.interNVDIMMS = 0;
#endif



  //
  // mem flows
  //
  setup->mem.memFlows = MEM_FLOWS;
  setup->mem.memFlowsExt  = MEM_FLOWS;


  //
  // On DE, Enable CECC WA for CH1 by default
  //
  if (setup->mem.optionsExt & CECC_WA) {
    setup->mem.ceccWaChMask = CECC_WA_CHMASK_DEFAULT;
  } else {
   setup->mem.ceccWaChMask = 0;
  }

  //
  // Initialize PPR options
  //
  setup->mem.pprType        = PPR_DISABLED;
  setup->mem.pprErrInjTest  = PPR_DISABLED;
}
