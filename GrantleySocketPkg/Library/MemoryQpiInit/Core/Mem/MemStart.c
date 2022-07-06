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
#include "SysRegs.h"

//
// Disable inline assembly warning
//
#pragma warning(disable : 4740)

extern CallTableEntryType CallTableMaster[];
extern CallTableEntryType CallTableSlave[];
extern UINT8 CallTableMasterSize;
extern UINT8 CallTableSlaveSize;

#ifdef RC_SIM
extern UINT8 WarmBootFlag;
#endif // RC_SIM

#pragma warning( disable : 4731 ) //AP remove warning from ebp modification
//
// Function declarations
//
void
PlatformInitializeData (
                       PSYSHOST host,
                       UINT8    socket
                       );
UINT32
InitStructures (
               PSYSHOST host,
               UINT8    socket
               );

#ifdef SERIAL_DBG_MSG
/**

    Print Memory Setup Options

    @param host   - Pointer to SysHost
    @param socket - Processor Socket

    @retval None

**/
VOID
PrintMemSetupOptions (
                     PSYSHOST host,
                     UINT8    socket
                     )
{
  UINT8           j, k, l;

  if (host->var.common.socketId == 0) {
    //
    // Save initial timestamp for latency measurements
    //
    ReadTsc64 (&host->var.mem.startTsc);
  }

  SetSetupOptionsChip (host);

  getPrintFControl (host);
  rcPrintf ((host, "\nsizeof sysHost       = %d\n", sizeof (struct sysHost)));
#ifdef  BDAT_SUPPORT
  rcPrintf ((host, "\nsizeof BDAT          = %d ", sizeof (BDAT_MEMORY_DATA_STRUCTURE) + sizeof (BDAT_RMT_STRUCTURE)));
#endif //BDAT_SUPPORT
  rcPrintf ((host, "\nsizeof memSetup      = %d\n", sizeof (struct memSetup)));
  rcPrintf ((host, "\nsizeof memNvram      = %d\n", sizeof (struct memNvram)));
  rcPrintf ((host, "\nsizeof socketNvram   = %d\n", sizeof (struct socketNvram)));
  rcPrintf ((host, "\nsizeof memVar        = %d\n", sizeof (struct memVar)));
  rcPrintf ((host, "\nsizeof Socket        = %d\n", sizeof (struct Socket)));
  rcPrintf ((host, "\nsizeof ddrChannel    = %d\n", sizeof (struct ddrChannel)));
  rcPrintf ((host, "\nsizeof dimmDevice    = %d\n", sizeof (struct dimmDevice)));
  rcPrintf ((host, "\nsizeof SADTable      = %d\n", sizeof (struct SADTable)));
  rcPrintf ((host, "\nsizeof TADTable      = %d\n", sizeof (struct TADTable)));
//  rcPrintf ((host, "\nsizeof homeAgent     = %d\n", sizeof (struct homeAgent)));
//  rcPrintf ((host, "\nsizeof IMC           = %d\n", sizeof (struct IMC)));

  rcPrintf ((host, "\nMAX_SOCKET           = %d\n", MAX_SOCKET));

  //
  // Output sysHostSetup structure via serial debug messages
  //
  rcPrintf ((host, "\nsizeof sysHostSetup  = %d\n", sizeof (struct sysSetup)));

  //
  // *** struct sysHostSetup ***
  //
  rcPrintf ((host, "\nstruct sysHost.common {\n"));
  rcPrintf ((host, "  options:         %08X\n", host->setup.common.options));
  rcPrintf ((host, "      PROMOTE_WARN_EN     "));
  if (host->setup.common.options & PROMOTE_WARN_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      HALT_ON_ERROR_EN    "));
  if (host->setup.common.options & HALT_ON_ERROR_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  serialDebugMsgLvl:%02X\n", host->setup.common.serialDebugMsgLvl));
  rcPrintf ((host, "  bsdBreakpoint:   %02X\n", host->setup.common.bsdBreakpoint));
  rcPrintf ((host, "  maxAddrMem:      %02X\n", host->setup.common.maxAddrMem));
  rcPrintf ((host, "  debugPort:       %02X\n", host->setup.common.debugPort));
  rcPrintf ((host, "  nvramPtr:        %08X\n", host->setup.common.nvramPtr));
  rcPrintf ((host, "  sysHostBufferPtr:%08X\n", host->setup.common.sysHostBufferPtr));
  rcPrintf ((host, "  mmCfgBase:       %08X\n", host->setup.common.mmCfgBase));
  rcPrintf ((host, "  mmCfgSize:       %08X\n", host->setup.common.mmCfgSize));
  rcPrintf ((host, "  pchumaEn:        %02X\n", host->setup.common.pchumaEn));
  rcPrintf ((host, "  numaEn:          %02X\n", host->setup.common.numaEn));
  rcPrintf ((host, "  logParsing:      %02X\n", host->setup.mem.logParsing));
  rcPrintf ((host, "  bdatEn:          %02X\n", host->setup.common.bdatEn));
  rcPrintf ((host, "  consoleComPort:  %02X\n", host->setup.common.consoleComPort));
  rcPrintf ((host, "\nstruct sysHost.setup.mem {\n"));
  rcPrintf ((host, "  options:         %08X\n", host->setup.mem.options));
  rcPrintf ((host, "      TEMPHIGH_EN         "));
  if (host->setup.mem.options & TEMPHIGH_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      PDWN_SR_CKE_MODE    "));
  if (host->setup.mem.options & PDWN_SR_CKE_MODE) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      OPP_SELF_REF_EN     "));
  if (host->setup.mem.options & OPP_SELF_REF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MDLL_SHUT_DOWN_EN   "));
  if (host->setup.mem.options & MDLL_SHUT_DOWN_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      PAGE_POLICY         "));
  if (host->setup.mem.options & PAGE_POLICY) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MULTI_THREAD_MRC_EN "));
  if (host->setup.mem.options & MULTI_THREAD_MRC_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ADAPTIVE_PAGE_EN    "));
  if (host->setup.mem.options & ADAPTIVE_PAGE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  SCRAMBLE_EN "));
  if (host->setup.mem.options & SCRAMBLE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - X_OVER_EN   "));
  if (host->setup.mem.memFlows & MF_X_OVER_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - SENSE_AMP_EN   "));
  if (host->setup.mem.memFlows & MF_SENSE_AMP_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - E_CMDCLK_EN   "));
  if (host->setup.mem.memFlows & MF_E_CMDCLK_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - REC_EN_EN   "));
  if (host->setup.mem.memFlows & MF_REC_EN_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RD_DQS_EN   "));
  if (host->setup.mem.memFlows & MF_RD_DQS_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - WR_LVL_EN   "));
  if (host->setup.mem.memFlows & MF_WR_LVL_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - WR_FLYBY_EN   "));
  if (host->setup.mem.memFlows & MF_WR_FLYBY_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - WR_DQ_EN   "));
  if (host->setup.mem.memFlows & MF_WR_DQ_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - CMDCLK_EN   "));
  if (host->setup.mem.memFlows & MF_CMDCLK_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RD_ADV_EN   "));
  if (host->setup.mem.memFlows & MF_RD_ADV_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - WR_ADV_EN   "));
  if (host->setup.mem.memFlows & MF_WR_ADV_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RD_VREF_EN   "));
  if (host->setup.mem.memFlows & MF_RD_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - WR_VREF_EN   "));
  if (host->setup.mem.memFlows & MF_WR_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RT_OPT_EN   "));
  if (host->setup.mem.memFlows & MF_RT_OPT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RX_DESKEW_EN   "));
  if (host->setup.mem.memFlows & MF_RX_DESKEW_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - TX_DESKEW_EN   "));
  if (host->setup.mem.memFlows & MF_TX_DESKEW_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - TX_EQ_EN   "));
  if (host->setup.mem.memFlows & MF_TX_EQ_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - IMODE_EN   "));
  if (host->setup.mem.memFlows & MF_IMODE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - EARLY_RID_EN   "));
  if (host->setup.mem.memFlows & MF_EARLY_RID_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - DQ_SWIZ_EN   "));
  if (host->setup.mem.memFlows & MF_DQ_SWIZ_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - LRBUF_RD_EN   "));
  if (host->setup.mem.memFlows & MF_LRBUF_RD_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - LRBUF_WR_EN   "));
  if (host->setup.mem.memFlows & MF_LRBUF_WR_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - RANK_MARGIN_EN   "));
  if (host->setup.mem.memFlows & MF_RANK_MARGIN_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - MEMINIT_EN   "));
  if (host->setup.mem.memFlows & MF_MEMINIT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - FNVSIMICSSIM_EN   "));
  if (host->setup.mem.memFlows & MF_FNVSIMICSSIM_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - MEMTEST_EN   "));
  if (host->setup.mem.memFlows & MF_MEMTEST_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - NORMAL_MODE_EN   "));
  if (host->setup.mem.memFlows & MF_NORMAL_MODE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS - E_CTLCLK_EN   "));
  if (host->setup.mem.memFlows & MF_E_CTLCLK_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS_EXT - RX_CTLE_EN   "));
  if (host->setup.mem.memFlowsExt & MF_EXT_RX_CTLE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "   MEM_FLOWS_EXT - PXC_EN   "));
  if (host->setup.mem.memFlowsExt & MF_EXT_PXC_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  //rcPrintf ((host, "      RX_SENSE_AMP_EN     "));
  //if (host->setup.mem.options & RX_SENSE_AMP_EN) rcPrintf ((host, "1\n"));
  //else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      DDR_RESET_LOOP      "));
  if (host->setup.mem.options & DDR_RESET_LOOP) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      NUMA_AWARE          "));
  if (host->setup.mem.options & NUMA_AWARE) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      DISABLE_WMM_OPP_READ "));
  if (host->setup.mem.options & DISABLE_WMM_OPP_READ) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ECC_CHECK_EN        "));
  if (host->setup.mem.options & ECC_CHECK_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ECC_MIX_EN "));
  if (host->setup.mem.options & ECC_MIX_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      BALANCED_4WAY_EN    "));
  if (host->setup.mem.options & BALANCED_4WAY_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      CA_PARITY_EN "));
  if (host->setup.mem.options & CA_PARITY_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      SPLIT_BELOW_4GB_EN  "));
  if (host->setup.mem.options & SPLIT_BELOW_4GB_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MARGIN_RANKS_EN     "));
  if (host->setup.mem.options & MARGIN_RANKS_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
#ifdef XMP_SUPPORT
  rcPrintf ((host, "      MEM_OVERRIDE_EN     "));
  if (host->setup.mem.options & MEM_OVERRIDE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
#endif // XMP_SUPPORT
  rcPrintf ((host, "      DRAMDLL_OFF_PD_EN   "));
  if (host->setup.mem.options & DRAMDLL_OFF_PD_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MEMORY_TEST_EN      "));
  if (host->setup.mem.options & MEMORY_TEST_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MEMORY_TEST_FAST_BOOT_EN "));
  if (host->setup.mem.options & MEMORY_TEST_FAST_BOOT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ATTEMPT_FAST_BOOT   "));
  if (host->setup.mem.options & ATTEMPT_FAST_BOOT) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ATTEMPT_FAST_BOOT_COLD "));
  if (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      SW_MEMORY_TEST_EN   "));
  if (host->setup.mem.options & SW_MEMORY_TEST_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      RMT_COLD_FAST_BOOT   "));
  if (host->setup.mem.options & RMT_COLD_FAST_BOOT) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      DISPLAY_EYE_EN "));
  if (host->setup.mem.options & DISPLAY_EYE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      PER_NIBBLE_EYE_EN "));
  if (host->setup.mem.options & PER_NIBBLE_EYE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, " optionsExt: %08X\n", host->setup.mem.optionsExt));
  rcPrintf ((host, "  RD_VREF_EN "));
  if (host->setup.mem.optionsExt & RD_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  WR_VREF_EN "));
  if (host->setup.mem.optionsExt & WR_VREF_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  PDA_EN "));
  if (host->setup.mem.optionsExt & PDA_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  TURNAROUND_OPT_EN "));
  if (host->setup.mem.optionsExt & TURNAROUND_OPT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  PER_BIT_MARGINS  "));
  if (host->setup.mem.optionsExt & PER_BIT_MARGINS) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  RASmodeEx:             %02X\n", host->setup.mem.RASmodeEx));
  rcPrintf ((host, "      DMNDSCRB_EN        "));
  if (host->setup.mem.RASmodeEx & DMNDSCRB_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      PTRLSCRB_EN        "));
  if (host->setup.mem.RASmodeEx & PTRLSCRB_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      A7_MODE_EN         "));
  if (host->setup.mem.RASmodeEx & A7_MODE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
#ifndef SKX_HOST
  rcPrintf ((host, "      DEVTAGGING_EN      "));
  if (host->setup.mem.RASmodeEx & DEVTAGGING_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
#else
  rcPrintf ((host, "      SDDC_EN      "));
  if (host->setup.mem.RASmodeEx & SDDC_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      ADDDC_EN      "));
  if (host->setup.mem.RASmodeEx & ADDDC_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
#endif
  rcPrintf ((host, "\nstruct sysHost.setup.mem {\n"));
  rcPrintf ((host, "  bclk: %02X\n", host->setup.mem.bclk));
  rcPrintf ((host, "  enforcePOR:      %02X\n", host->setup.mem.enforcePOR));
  rcPrintf ((host, "  ddrFreqLimit:    %02X\n", host->setup.mem.ddrFreqLimit));
  rcPrintf ((host, "  chInter:         %02X\n", host->setup.mem.chInter));
  rcPrintf ((host, "  dimmTypeSupport: %02X\n", host->setup.mem.dimmTypeSupport));
  rcPrintf ((host, "  vrefStepSize:    %02X\n", host->setup.mem.vrefStepSize));
  rcPrintf ((host, "  vrefAbsMaxSteps: %02X\n", host->setup.mem.vrefAbsMaxSteps));
  rcPrintf ((host, "  vrefOpLimitSteps:%02X\n", host->setup.mem.vrefOpLimitSteps));
  rcPrintf ((host, "  pdwnCkMode:      %02X\n", host->setup.mem.pdwnCkMode));
  rcPrintf ((host, "  MemPwrSave:      %02X\n", host->setup.mem.MemPwrSave));
#ifdef PPR_SUPPORT
  rcPrintf ((host, "  pprType:         %02X\n", host->setup.mem.pprType));
  rcPrintf ((host, "  pprErrInjTest    %02X\n", host->setup.mem.pprErrInjTest));
#endif // PPR_SUPPORT
  rcPrintf ((host, "  ckeThrottling:   %02X\n", host->setup.mem.ckeThrottling));
  rcPrintf ((host, "  olttPeakBWLIMITPercent: %02X\n", host->setup.mem.olttPeakBWLIMITPercent));
  rcPrintf ((host, "  thermalThrottlingOptions:  %02X\n", host->setup.mem.thermalThrottlingOptions));
  rcPrintf ((host, "      CLTT_EN             "));
  if (host->setup.mem.thermalThrottlingOptions & CLTT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      OLTT_EN             "));
  if (host->setup.mem.thermalThrottlingOptions & OLTT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MH_OUTPUT_EN        "));
  if (host->setup.mem.thermalThrottlingOptions & MH_OUTPUT_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "      MH_SENSE_EN         "));
  if (host->setup.mem.thermalThrottlingOptions & MH_SENSE_EN) rcPrintf ((host, "1\n"));
  else rcPrintf ((host, "0\n"));
  rcPrintf ((host, "  DimmTempStatValue  %02X\n", host->setup.mem.DimmTempStatValue));
  rcPrintf ((host, "  dramraplen:      %02X\n", host->setup.mem.dramraplen));
  rcPrintf ((host, "  dramraplbwlimittf: %02X\n", host->setup.mem.dramraplbwlimittf));
  rcPrintf ((host, "  lrdimmModuleDelay: %02X\n", host->setup.mem.lrdimmModuleDelay));
  rcPrintf ((host, "  customRefreshRate: %02X\n", host->setup.mem.customRefreshRate));
  //rcPrintf ((host, "  vrefDelayUs:     %02X\n", host->setup.mem.vrefDelayUs));
  //rcPrintf ((host, "  vddDelayUs:      %02X\n", host->setup.mem.vddDelayUs));
  rcPrintf ((host, "  rxVrefTraining: %02X\n", host->setup.mem.rxVrefTraining));
  rcPrintf ((host, "  iotMemBufferRsvtn: %02X\n", host->setup.mem.iotMemBufferRsvtn));
#ifdef SSA_FLAG
  rcPrintf ((host, "  enableBiosSsaLoader: %02X\n", host->setup.mem.enableBiosSsaLoader));
#endif // SSA_FLAG
  rcPrintf ((host, "  enforceThreeMonthTimeout: %02X\n", host->setup.mem.enforceThreeMonthTimeout));
#ifdef MARGIN_CHECK
  rcPrintf ((host, "  rmtPatternLength:%02X\n", host->setup.mem.rmtPatternLength));
  rcPrintf ((host, "  rmtPatternLengthExt(CMD/CTL):%02X\n", host->setup.mem.rmtPatternLengthExt));
#endif // MARGIN_CHECK
  rcPrintf ((host, "  patrolScrubDuration:%02X\n", host->setup.mem.patrolScrubDuration));
  rcPrintf ((host, "  memTestLoops:    %02X\n", host->setup.mem.memTestLoops));
  //rcPrintf ((host, "  errLedBitOverride: %02X\n", host->setup.mem.errLedBitOverride));
  rcPrintf ((host, "  scrambleSeedLow: %08X\n", host->setup.mem.scrambleSeedLow));
  rcPrintf ((host, "  scrambleSeedHigh:%08X\n", host->setup.mem.scrambleSeedHigh));
  rcPrintf ((host, "  ADREn:           %02X\n", host->setup.mem.ADREn));
#ifdef MEM_NVDIMM_EN
  rcPrintf ((host, "  eraseArmNVDIMMS: %02X\n", host->setup.mem.eraseArmNVDIMMS));
#endif  //MEM_NVDIMMEN
  rcPrintf ((host, "  check_pm_sts:    %02X\n", host->setup.mem.check_pm_sts));
  rcPrintf ((host, "  check_platform_detect: %02X\n", host->setup.mem.check_platform_detect));
  rcPrintf ((host, "  mcBgfThreshold:  %02X\n", host->setup.mem.mcBgfThreshold));
  rcPrintf ((host, "  normOppIntvl:  %04x\n", host->setup.mem.normOppIntvl));
  rcPrintf ((host, "  SpdSmbSpeed: %02X\n", host->setup.mem.SpdSmbSpeed));
  //
  // * struct ddrChannelSetup *
  //
  for (j = 0; j < MAX_CH; j++) {
    rcPrintf ((host, "    struct ddrChannelSetup[%02X] {\n", j));
    rcPrintf ((host, "      enabled:       %02X\n", host->setup.mem.socket[socket].ddrCh[j].enabled));
    rcPrintf ((host, "      numDimmSlots:  %02X\n", host->setup.mem.socket[socket].ddrCh[j].numDimmSlots));
    rcPrintf ((host, "      batterybacked: %02X\n", host->setup.mem.socket[socket].ddrCh[j].batterybacked));
    for (k = 0; k < MAX_DIMM; k++) {
      rcPrintf ((host, "      struct ddrDimmSetup[%02X] {\n", k));
      for (l = 0; l < MAX_RANK_DIMM; l++) {
        rcPrintf ((host, "        mapOut[%02X]: %02X\n", l, host->setup.mem.socket[socket].ddrCh[j].dimmList[k].mapOut[l]));
      }
      rcPrintf ((host, "      };\n"));
#ifdef MEM_NVDIMM_EN
      rcPrintf ((host, "            NvDimmIdx: %02d\n", host->nvram.mem.socket[socket].channelList[j].dimmList[k].nvDimmType));
#endif  //MEM_NVDIMM_EN
    }
    rcPrintf ((host, "    };\n"));
  }

#ifdef XMP_SUPPORT
  rcPrintf ((host, "  struct memTiming {\n"));
  rcPrintf ((host, "    nCL:            %02X\n", host->setup.mem.inputMemTime.nCL));
  rcPrintf ((host, "    nRP:            %02X\n", host->setup.mem.inputMemTime.nRP));
  rcPrintf ((host, "    nRCD:           %02X\n", host->setup.mem.inputMemTime.nRCD));
  rcPrintf ((host, "    nRRD:           %02X\n", host->setup.mem.inputMemTime.nRRD));
  rcPrintf ((host, "    nWTR:           %02X\n", host->setup.mem.inputMemTime.nWTR));
  rcPrintf ((host, "    nRAS:           %02X\n", host->setup.mem.inputMemTime.nRAS));
  rcPrintf ((host, "    nRTP:           %02X\n", host->setup.mem.inputMemTime.nRTP));
  rcPrintf ((host, "    nWR:            %02X\n", host->setup.mem.inputMemTime.nWR));
  rcPrintf ((host, "    nFAW:           %02X\n", host->setup.mem.inputMemTime.nFAW));
  rcPrintf ((host, "    nRC:            %02X\n", host->setup.mem.inputMemTime.nRC));
  rcPrintf ((host, "    nCWL:           %02X\n", host->setup.mem.inputMemTime.nCWL));
  rcPrintf ((host, "    nCMDRate:       %02X\n", host->setup.mem.inputMemTime.nCMDRate));
  rcPrintf ((host, "    ddrFreqLimit:   %02X\n", host->setup.mem.inputMemTime.ddrFreqLimit));
  rcPrintf ((host, "    vdd:            %02X\n", host->setup.mem.inputMemTime.vdd));
  rcPrintf ((host, "    ucVolt:         %02X\n", host->setup.mem.inputMemTime.ucVolt));
  rcPrintf ((host, "    casSup:         %02X\n", host->setup.mem.inputMemTime.casSup));
  rcPrintf ((host, "    tREFI:          %02X\n", host->setup.mem.inputMemTime.tREFI));
  rcPrintf ((host, "    nRFC:           %02X\n", host->setup.mem.inputMemTime.nRFC));
  rcPrintf ((host, "    ddrFreq:        %02X\n", host->setup.mem.inputMemTime.ddrFreq));
  rcPrintf ((host, "  };\n"));
#endif // XMP_SUPPORT
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  rcPrintf ((host, "  meRequestedSize:  %08X\n", host->setup.mem.meRequestedSize));
#endif // ME_SUPPORT_FLAG
  rcPrintf ((host, "  };\n"));
  rcPrintf ((host, "  serialDebugMsgLvl:   %02X\n", host->setup.common.serialDebugMsgLvl));
  PrintMemSetupOptionsChip (host, socket);

  rcPrintf ((host, "}; //struct sysHost.setup\n\n"));
  releasePrintFControl (host);
}
#endif // SERIAL_DBG_MSG

/**

  Memory Controller initialization entry point

  @param host  - Pointer to the system host (root) structure

  @retval 0 - success (Memory Initialization Complete)
  @retval Other - failure (Did not complete memory initialization)

**/
UINT32
MemStart (
         PSYSHOST host
         )
{
  UINT32          status;
  struct memSetup tempSetupData;
  UINT8           socket;
  UINT8           maxNode;
  UINT32          csrReg;
  UINT8           sbspSktId = GetSbspSktId(host);
  UINT32          exitFrame;
  UINT32          exitVector;

  PUSH_EXIT_FRAME(exitFrame, exitVector);

  host->var.common.exitFrame  = exitFrame;
  host->var.common.exitVector = exitVector;

  socket = host->var.common.socketId;


#ifdef SERIAL_DBG_MSG
  if (host->setup.common.serialDebugMsgLvl & SDBG_MAX) {
    PrintMemSetupOptions(host, socket);
  }
#endif // SERIAL_DBG_MSG

  //
  // Save input structure into a temporary buffer
  //
  tempSetupData = host->setup.mem;
  //
  // Set firstPass variable for InitStructures().
  //
  host->var.mem.firstPass = 1;

  //
  // Force serial during until certain KTI routes are complete if > 2S
  //
  if ((host->setup.mem.options & MULTI_THREAD_MRC_EN) && ((host->var.common.socketPresentBitMap < 0x7) || (host->var.common.resetRequired == 0))) {
    maxNode = socket + 1;
  } else {
    maxNode = MAX_SOCKET;
    socket = 0;
  }


  host->var.mem.dateCheckedOnFirstSocket = 0;  //Always 0 till the Clock Date Changes on the first Socket Present

  for (; socket < maxNode; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    //
    // Initialize DDR and platform control structures
    //
    PlatformInitializeData (host, socket);

    //
    // Initialize DDR and platform control structures
    //
    InitStructures (host, socket);
  } // socket loop

#ifdef SERIAL_DBG_MSG

  if (host->setup.common.serialDebugMsgLvl & SDBG_MAX) {
    rcPrintf ((host, "\nsizeof sysNvram = %d\n\n", sizeof (struct sysNvram)));
  }

  if (host->var.mem.previousBootError) {
    rcPrintf ((host, "Previous error(s); forcing Cold Boot\n"));
  }

  if (host->var.common.bootMode == NormalBoot) {
    rcPrintf ((host, "bootMode = NormalBoot\n"));
    if (host->var.mem.subBootMode == WarmBoot) {
      rcPrintf ((host, "subBootMode = WarmBoot\n"));
    } else if (host->var.mem.subBootMode == WarmBootFast) {
      rcPrintf ((host, "subBootMode = WarmBootFast\n"));
    } else if (host->var.mem.subBootMode == ColdBoot) {
      rcPrintf ((host, "subBootMode = ColdBoot\n"));
    } else if (host->var.mem.subBootMode == ColdBootFast) {
      rcPrintf ((host, "subBootMode = ColdBootFast\n"));
    } else {
      rcPrintf ((host, "subBootMode = unknown.\n"));
      FatalError (host, ERR_MRC_STRUCT, ERR_INVALID_SUB_BOOT_MODE);
    }
  } else if (host->var.common.bootMode == S3Resume) {
    rcPrintf ((host, "bootMode = S3Resume\n"));
    if (host->var.mem.subBootMode == AdrResume)
      rcPrintf ((host, "subBootMode = AdrResume.\n"));
#ifdef MEM_NVDIMM_EN
  } else if (host->var.mem.subBootMode == NvDimmResume) {
    rcPrintf ((host, "subBootMode = NvDimmResume\n"));
#endif  //MEM_NVDIMM_EN
  } else {
    rcPrintf ((host, "bootMode is unknown.\n"));
    FatalError (host, ERR_MRC_STRUCT, ERR_INVALID_BOOT_MODE);
  }
#endif // SERIAL_DBG_MSG

  //
  // Call into memory reference code
  //
  //
  // Force serial during until certain KTI routes are complete if > 2S
  //
  if (host->var.common.resetRequired != POST_RESET_POWERGOOD) {
#ifdef UNCORE_SV_HOOKS
    if (host->setup.uncore.SvParm.SvOpioTrainingDis && host->var.uncore.McdramInstances != 0) {
      status = SUCCESS;
    } else
#endif // UNCORE_SV_HOOKS
    if (host->var.common.socketId == host->nvram.common.sbspSocketId) {
      status = MemMain (host, CallTableMaster, CallTableMasterSize);
    } else {
      status = MemMain (host, CallTableSlave, CallTableSlaveSize);
    }
  } else {
    status = FAILURE;
  }

  //
  // Only execute this for the SBSP
  //
  if (AcquireCommandPipe (host, host->var.common.socketId)) {

    //
    // check for FRB errors
    //
    csrReg = GetStickyScratchpad (host, sbspSktId, 7);

    if (csrReg & BIT28) {
      //
      // disable FRB indication for next boot
      //

      SetStickyScratchpad (host, sbspSktId, 7, (csrReg & ~BIT28));

    }//endif FRB error


    //
    // on cold boot, save MRC input structure into NVRAM and store date
    //
    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
      host->nvram.mem.lastBootDate = MemGetCmosDate(host);
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Setting Last Boot Date = %u days\n", host->nvram.mem.lastBootDate));
      host->nvram.mem.savedSetupData = tempSetupData;
    }

    //
    // Output data on normal boot path
    //
    if (host->var.common.bootMode == NormalBoot) {
      //
      // Call OEM hook to write NVRAM data
      // OemWriteNvram(host);
      //
#ifdef SERIAL_DBG_MSG
#if 0
      rcPrintf ((host, "Normal boot path host->nvram contents:"));
      for (i = 0; i < sizeof (struct sysNvram); i+=4) {
        if (i % 16 == 0) {
  //        rcPrintf (host, "\nord1 0%Xp = ", (UINT8 *) &host->nvram + i);
          rcPrintf ((host, "\n"));
        }

        rcPrintf ((host, "0x%08X", *((UINT32 *) &host->nvram + i) ));
        if (i % 16 < 15) {
          rcPrintf ((host, ", "));
        }
      }

      rcPrintf ((host, "\n"));
#endif // 0
#endif // SERIAL_DBG_MSG
    }


    //
    // Indicate that MRC is complete
    //
    rcPrintf ((host, "STOP_MRC_RUN\n"));


    if (!host->var.common.resetRequired) {
      OutputCheckpoint (host, STS_MRC_DONE, 0, 0);
      SetMemDone (host);
    }
    //
    // Hard-coded memory config for Virtual Platform single socket
    //
  }
  //
  // if SBSP
  //

  //
  // Return MRC status
  //
  POP_EXIT_FRAME(status);
  return status;
}

/**

  Initialize internal data structures

  @param host  - Pointer to the system host (root) structure
  @param socket  - processor socket ID

  @retval 0 - successful
  @retval Other - failure

**/
UINT32
InitStructures (
               PSYSHOST host,
               UINT8    socket
               )
{
  UINT8                     ch;
  UINT8                     dimm;
  UINT8                     rank;
  UINT8                     setupChanged;
  UINT8                     n;
  UINT32                    csrReg;
  INT16                     data16;
  INT16                     oldCrc16;
  UINT32                    logData;
  char                      *memNvramPtr;
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  BOOLEAN                   forceColdBoot;
  INT32                     i;
#ifdef LRDIMM_SUPPORT
  UINT8                    strobe;
  struct ddrRank           (*rankList)[MAX_RANK_DIMM];
#endif // LRDIMM_SUPPORT


  InitDdrFreqChip (host, socket);

  setupChanged  = 0;
  //
  // Initialize to ColdBoot at first.  Can change to a faster boot method later
  // if certain conditions are met and the caller requests a faster boot mode attempt.
  //
  host->var.mem.subBootMode = ColdBoot;
  host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
  //
  // MRC only needs to worry about 2 main boot modes - NormalBoot and S3Resume.
  // In case the caller passes in some other value, make sure it gets reset
  // to NormalBoot to take the main flow through MRC.
  //
  if (host->var.common.bootMode != S3Resume) {
    host->var.common.bootMode = NormalBoot;
  }

  for (i = 0; i < MAX_CH; i++) {
    host->var.mem.socket[socket].channelList[i].mcId = (UINT8)(i / host->var.mem.numChPerMC);
  }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  // For S3 resume patch meRequestedSize is not read from ME
  // Saved value has to be used instead.
  //
  if (host->var.common.bootMode == S3Resume) {
    ((struct sysSetup *)&host->setup)->mem.meRequestedSize = host->nvram.mem.savedSetupData.meRequestedSize;
  }
  host->var.mem.meStolenSize = host->setup.mem.meRequestedSize;
#endif //ME_SUPPORT_FLAG

  //
  // Check if the MRC version has changed
  //
  if (host->nvram.common.rcVersion != RC_REVISION) {
    setupChanged = 1;
  }
  //
  // Check if the MRC input has changed since the previously saved cold boot data
  //
  if (
     (host->nvram.mem.savedSetupData.options & ~FASTBOOT_MASK) != (host->setup.mem.options & ~FASTBOOT_MASK) ||
     host->nvram.mem.savedSetupData.optionsExt != host->setup.mem.optionsExt ||
     host->nvram.mem.savedSetupData.enforcePOR != host->setup.mem.enforcePOR ||
     host->nvram.mem.savedSetupData.RASmodeEx != host->setup.mem.RASmodeEx ||
     host->nvram.mem.savedSetupData.ddrFreqLimit != host->setup.mem.ddrFreqLimit ||
     host->nvram.mem.savedSetupData.chInter != host->setup.mem.chInter ||
     host->nvram.mem.savedSetupData.dimmTypeSupport != host->setup.mem.dimmTypeSupport ||
     host->nvram.mem.savedSetupData.vrefStepSize != host->setup.mem.vrefStepSize ||
     host->nvram.mem.savedSetupData.vrefAbsMaxSteps != host->setup.mem.vrefAbsMaxSteps ||
     host->nvram.mem.savedSetupData.vrefOpLimitSteps != host->setup.mem.vrefOpLimitSteps ||
     host->nvram.mem.savedSetupData.pdwnCkMode != host->setup.mem.pdwnCkMode ||
     host->nvram.mem.savedSetupData.MemPwrSave != host->setup.mem.MemPwrSave ||
     host->nvram.mem.savedSetupData.ckeThrottling != host->setup.mem.ckeThrottling ||
     host->nvram.mem.savedSetupData.olttPeakBWLIMITPercent != host->setup.mem.olttPeakBWLIMITPercent ||
     host->nvram.mem.savedSetupData.thermalThrottlingOptions != host->setup.mem.thermalThrottlingOptions ||
     host->nvram.mem.savedSetupData.dramraplen != host->setup.mem.dramraplen ||
     host->nvram.mem.savedSetupData.dramraplbwlimittf != host->setup.mem.dramraplbwlimittf ||
     host->nvram.mem.savedSetupData.lrdimmModuleDelay != host->setup.mem.lrdimmModuleDelay ||
     host->nvram.mem.savedSetupData.iotMemBufferRsvtn != host->setup.mem.iotMemBufferRsvtn ||
#ifdef SSA_FLAG
     host->nvram.mem.savedSetupData.enableBiosSsaLoader != host->setup.mem.enableBiosSsaLoader ||
#endif // SSA_FLAG
     host->nvram.mem.savedSetupData.enforceThreeMonthTimeout != host->setup.mem.enforceThreeMonthTimeout ||
#ifdef MARGIN_CHECK
     host->nvram.mem.savedSetupData.rmtPatternLength != host->setup.mem.rmtPatternLength ||
     host->nvram.mem.savedSetupData.rmtPatternLengthExt != host->setup.mem.rmtPatternLengthExt ||
#endif // MARGIN_CHECK
     host->nvram.mem.savedSetupData.patrolScrubDuration != host->setup.mem.patrolScrubDuration ||
     host->nvram.mem.savedSetupData.memTestLoops != host->setup.mem.memTestLoops ||
     host->nvram.mem.savedSetupData.scrambleSeedLow != host->setup.mem.scrambleSeedLow ||
     host->nvram.mem.savedSetupData.scrambleSeedHigh != host->setup.mem.scrambleSeedHigh ||
     host->nvram.mem.savedSetupData.ADREn != host->setup.mem.ADREn ||
     host->nvram.mem.savedSetupData.check_pm_sts != host->setup.mem.check_pm_sts ||
     host->nvram.mem.savedSetupData.check_platform_detect != host->setup.mem.check_platform_detect ||
     host->nvram.mem.savedSetupData.oemHooks.oemCallback != host->setup.mem.oemHooks.oemCallback ||
     SavedSetupDataChangedChip (host)

     //
     // check ddrIMCsetup after this
     //

#ifdef XMP_SUPPORT
     // memTiming
     ||
     host->nvram.mem.savedSetupData.inputMemTime.nCL != host->setup.mem.inputMemTime.nCL ||
     host->nvram.mem.savedSetupData.inputMemTime.nRP != host->setup.mem.inputMemTime.nRP ||
     host->nvram.mem.savedSetupData.inputMemTime.nRCD != host->setup.mem.inputMemTime.nRCD ||
     host->nvram.mem.savedSetupData.inputMemTime.nRRD != host->setup.mem.inputMemTime.nRRD ||
     host->nvram.mem.savedSetupData.inputMemTime.nWTR != host->setup.mem.inputMemTime.nWTR ||
     host->nvram.mem.savedSetupData.inputMemTime.nRAS != host->setup.mem.inputMemTime.nRAS ||
     host->nvram.mem.savedSetupData.inputMemTime.nRTP != host->setup.mem.inputMemTime.nRTP ||
     host->nvram.mem.savedSetupData.inputMemTime.nWR != host->setup.mem.inputMemTime.nWR ||
     host->nvram.mem.savedSetupData.inputMemTime.nFAW != host->setup.mem.inputMemTime.nFAW ||
     host->nvram.mem.savedSetupData.inputMemTime.nCWL != host->setup.mem.inputMemTime.nCWL ||
     host->nvram.mem.savedSetupData.inputMemTime.nRC != host->setup.mem.inputMemTime.nRC ||
     host->nvram.mem.savedSetupData.inputMemTime.nCMDRate != host->setup.mem.inputMemTime.nCMDRate ||
     host->nvram.mem.savedSetupData.inputMemTime.nRFC != host->setup.mem.inputMemTime.nRFC ||
     host->nvram.mem.savedSetupData.inputMemTime.ddrFreqLimit != host->setup.mem.inputMemTime.ddrFreqLimit ||
     host->nvram.mem.savedSetupData.inputMemTime.vdd != host->setup.mem.inputMemTime.vdd ||
     host->nvram.mem.savedSetupData.inputMemTime.ucVolt != host->setup.mem.inputMemTime.ucVolt ||
     host->nvram.mem.savedSetupData.inputMemTime.casSup != host->setup.mem.inputMemTime.casSup ||
     host->nvram.mem.savedSetupData.inputMemTime.tREFI != host->setup.mem.inputMemTime.tREFI ||
     host->nvram.mem.savedSetupData.inputMemTime.ddrFreq != host->setup.mem.inputMemTime.ddrFreq
#endif // XMP_SUPPORT
     ) {
    //
    // Indicate a change occured to the input parameters
    //
    setupChanged = 1;

  }

  //
  // Check if IMC enabled changed
  //
  if (DetectIMCEnabledChangeChip (host, socket)) {
    setupChanged = 1;
  }

  //
  // Check if the DIMM map out input has changed
  //
  if (host->nvram.mem.savedSetupData.socket[socket].enabled != host->setup.mem.socket[socket].enabled ||
      SavedSetupDataChangedIMCChip (host, socket))
  {
    setupChanged = 1;
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if (host->nvram.mem.savedSetupData.socket[socket].ddrCh[ch].enabled != host->setup.mem.socket[socket].ddrCh[ch].enabled) {
      setupChanged = 1;
      break;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (host->nvram.mem.savedSetupData.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank] !=
            host->setup.mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank]) {
          //
          // Indicate a change occured to the input parameters
          //
          setupChanged = 1;
          break;
        }
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Detect if socket is populated
  //
  if (DetectSocketPopulatedChangedChip (host, socket)) {
    setupChanged = 1;
  }
  // Initialize to do not force cold boot
  //
  forceColdBoot = FALSE;

  //
  // Check if there was a failure on the previous boot and disable fast boot if yes
  //
  if (host->var.mem.previousBootError) {
    setupChanged = 1;
  }
  //
  // Disable fast boot if there were any core errors on any socket
  //
  for (n = 0; n < MAX_SOCKET; n++) {
    if ((host->var.common.socketPresentBitMap & (BIT0 << n)) == 0) continue;

    if (DetectMCAErrorsChip (host, socket, &logData)) {
      setupChanged = 1;

      MemDebugPrint((host, SDBG_MINMAX, n, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "A core error occured during the previous boot. MCA_ERR_SRC_LOG = 0x%x\n", logData));
      break;
    }
  } // n loop

  //
  // check for FRB errors
  //
  csrReg = GetStickyScratchpad (host, socket, 7);

  if (csrReg & BIT28) {
    host->var.mem.subBootMode = ColdBoot;
    forceColdBoot             = TRUE;
    rcPrintf ((host,"BIST error found disable fast boot\n"));
  }//endif FRB error

  if (host->var.common.bootMode == S3Resume) {
    //
    // Initialize to skip memory init.  We want to skip memory init
    // on S3.
    //
    host->var.mem.skipMemoryInit = 1;
  }

  if (host->setup.mem.ADREn) {
    UINT32 *pch_pm_sts;
    if (host->setup.mem.check_pm_sts) {

#ifdef PCH_SPT
      pch_pm_sts = (UINT32 *)(host->var.common.pwrmBase + SSB_PM_STS);
#else
      pch_pm_sts = (UINT32 *)(host->var.common.rcbaBase + SSB_PM_STS);
#endif //PCH_SPT
      if (*pch_pm_sts & ADR_RST_STS) {
        MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "ADR recovery triggered by pch_pm_sts\n"));
        host->var.common.bootMode = S3Resume;
        host->var.mem.subBootMode = AdrResume;
      }
    }
    if (host->setup.mem.check_platform_detect) {
      // Call the OEM hook to check for ADR.
      if (PlatformDetectADR(host)) {
        MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "ADR recovery triggered via PlatformDetectADR\n"));
        host->var.common.bootMode = S3Resume;
        host->var.mem.subBootMode = AdrResume;
      }
    }
    if (host->var.mem.subBootMode == AdrResume) {
      host->var.mem.skipMemoryInit = 0;
    } else {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "No ADR event detected\n"));
      PlatformReleaseADRClamps(host, 0);
    }
#ifdef MEM_NVDIMM_EN
#ifndef DE_SKU
    if (host->setup.mem.ADREn == ADR_NVDIMM) {
    // Make sure ADR Complete isn't high
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "Clearing ADR_STS register\n"));
    pch_pm_sts = (UINT32 *)(host->var.common.rcbaBase + SSB_PM_STS); // PCH_PM_STS
    *pch_pm_sts = ADR_RST_STS;
    }
#endif
#endif
  } // ADREn

  if (host->var.common.bootMode == NormalBoot) {
    //
    // On normal boot paths (i.e. not S3), do meminit by default.
    //
    host->var.mem.skipMemoryInit = 0;
    //
    // Check BIT1 for SSPAD0 on each socket.  If this bit is set, it means we are on a warm
    // boot cycle.
    //
    csrReg = GetStickyScratchpad (host, socket, 0);
    //
    // If this CPU does not have this bit set, it means we are not
    // on a warm boot path.
    //
    if ((csrReg & BIT1) == 0) {
      forceColdBoot = TRUE;
    }
	
	// forced coldboot on a warmreset if FCB is enabled and FWB is not.
    if ((csrReg & BIT1) && (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD) && !(host->setup.mem.options & ATTEMPT_FAST_BOOT)) {
      forceColdBoot = TRUE;
    }
	
    //
    // Clear out this register so that if MRC fails, the next reboot
    // will take the cold boot path with full training.
    //
    SetStickyScratchpad (host, socket, 0, csrReg & ~BIT1);

    //
    // Switch to warm boot if all CPUs have the warm reset bit set.
    //
    if (forceColdBoot) {
      host->var.mem.subBootMode = ColdBoot;
      if (!(host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD)){
      setupChanged = 1;
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "forceColdBoot bit set\n"));
      }
    } else {
      host->var.mem.subBootMode = WarmBoot;
    }
  }
  
  // AptioV Server Override Start: Added to skip LastBoot Error Handler for cold boot.
  if ( host->var.mem.subBootMode == ColdBoot ) {
      host->var.mem.IsColdBoot = TRUE;
  }
  // AptioV Server Override End: Added to skip LastBoot Error Handler for cold boot.

  //

  //
  // At this point, we have determined whether or not we are on a NormalBoot path or an
  // S3Resume path.  In addition, we have determined whether we're on a WarmBoot path
  // or ColdBoot path as a subset (sub-boot type) of the NormalBoot path through MRC.
  //
  // Now we need to do some further checks to see if these boot types need to be
  // modified for speed.
  //
  if ((host->setup.mem.options & ATTEMPT_FAST_BOOT) || (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD) || (host->var.common.bootMode == S3Resume)) {
    //
    // Check the CRC of the memory portion of the NVRAM structure.
    // If it doesn't pass, then something is wrong and we can not
    // attempt a fast boot or S3 boot since they require the NVRAM structure
    // to be healthy.
    //
    oldCrc16 = host->nvram.mem.crc16;
    host->nvram.mem.crc16 = 0;
    memNvramPtr = (char *)(&(host->nvram.mem));
    data16 = Crc16 (memNvramPtr, sizeof(host->nvram.mem));
    host->nvram.mem.crc16 = data16;
    //
    // If NVRAM structure failed its CRC check...
    //
    if (oldCrc16 != data16) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "NVRAM CRC check failed. Disable fast boot\n"));
      setupChanged = 1;
    }

    if (host->var.mem.nvramDecompressionFailed) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "NVRAM Decompression failed. Disable fast boot\n"));
      setupChanged = 1;
    }
  }

  //
  // Check boot date
  //
    //If setupChanged == 1, then do a cold boot through memory from socket0; do not check the date for Socket1/2/3
  if (DetectBootDateOldChip (host, socket)) {
    setupChanged = 1;
  }

  //
  // Check processor for cold fast boot
  //
  if (setupChanged) {
    host->var.mem.socket[socket].procPpin = GetPpinChip (host, socket);
  } else {
    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot) && (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD)) {
      rcPrintf ((host, "Cold Fast Boot Check\n"));
      if(ProcPpinChangedCheck(host, socket)){
        setupChanged = 1;
      }
      if (DetectProcessorRemovedChip (host)) {
        setupChanged = 1;
        rcPrintf ((host, "CPU Changed\n"));
      }
    }
  }

  rcPrintf ((host,"setupChanged: %d\n", setupChanged));

  //
  // Enable WarmBoot if there haven't been any changes to setup
  //
  if (!setupChanged) {
    //
    // Nothing has changed from boot to boot.  Now, see if we can take a fast
    // path through MRC based on the input parameter ATTEMPT_FAST_BOOT.
    //
    if (host->var.common.bootMode == NormalBoot) {
      //
      // If input parameters request a fast boot attempt, change the boot type to
      // WarmBootFast or ColdBootFast, depending on the type of boot.
      //
      if ( (host->var.mem.subBootMode == WarmBoot) && (host->setup.mem.options & ATTEMPT_FAST_BOOT) ) {
        host->var.mem.subBootMode = WarmBootFast;
        // For fast warm boot, skip memory init.  This is an optimization for boot time
        // and it is also a requirement for ME UMA systems because the ME UMA memory
        // must not be re-initialized on a warm boot.
        //
        host->var.mem.skipMemoryInit = 1;
      }
      if ((host->var.mem.subBootMode == ColdBoot) && (host->setup.mem.options & ATTEMPT_FAST_BOOT_COLD)) {
        if (AttemptColdBootFastChip (host, socket)) {
          host->var.mem.subBootMode = ColdBootFast;
          host->var.mem.socket[socket].socketSubBootMode = ColdBootFast;
        } else {
          if (host->var.mem.firstPass != 0) {
#ifdef SERIAL_DBG_MSG
            rcPrintf ((host, "ColdBootFast disabled, clearing the MRC NVRAM structure for ColdBoot.\n"));
#endif
            host->var.mem.firstPass = 0;
            MemSetLocal ((UINT8 *) &host->nvram.mem, 0, sizeof (struct memNvram));
          }
        }
      }  // Attempt Fast Cold Boot
    } // NormalBoot
  } else {
    //
    // If we are on an S3 Resume path and something has changed,
    // do not attempt an S3 boot.  Rather, go back to a normal boot.
    //
    // HSD 3614451: MRC parameter mismatch results in hybrid boot (S3 tag)
    // Make a parameter mismatch in S3 path a Fatal error
    //
    if (host->var.common.bootMode == S3Resume) {
      rcPrintf ((host, "S3 resume path but something changed\n"));
      FatalError (host, ERR_MRC_STRUCT, ERR_INVALID_BOOT_MODE);
    }
    //
    // If this is our first pass through this routine, then zero out the NVRAM
    // structure.  We don't do this on subsequent passes because they would blow
    // away the NVRAM data for CPUs that have already been initialized in this
    // routine on previous passes.
    //
    if (host->var.mem.firstPass != 0) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "Clearing the MRC NVRAM structure.\n"));
#endif
      host->var.mem.firstPass = 0;
      MemSetLocal ((UINT8 *) &host->nvram.mem, 0, sizeof (struct memNvram));
    }
  }

  //
  // There are no differences between Warm and Cold boot from an MRC
  // perspective so take the warm boot and make it go through the cold boot path.
  //
  if (host->var.mem.subBootMode == WarmBoot) {
    host->var.mem.subBootMode = ColdBoot;
  }
  //
  // Copy host setup parameters
  //
  host->var.mem.ddrFreqLimit = host->setup.mem.ddrFreqLimit;
  //
  // Check if QR DIMM is disabled in HW
  //

  if (GetDisableQRDimmChip (host, socket)) {
    host->var.mem.socket[socket].maxRankDimm = 2;
  } else {
    host->var.mem.socket[socket].maxRankDimm = MAX_RANK_DIMM;

  }
#ifdef SERIAL_DBG_MSG
  host->var.mem.serialDebugMsgLvl = host->setup.common.serialDebugMsgLvl;
#endif // SERIAL_DBG_MSG

  //
  // Only update the NVRAM structure if not resuming from S3 or warm boot
  //
  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
    SetIMCEnabledChip (host, socket);
  }
  for (ch = 0; ch < MAX_CH; ch++) {
    //
    // Only update the NVRAM structure if not resuming from S3 or warm boot
    //
    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
      host->nvram.mem.socket[socket].channelList[ch].enabled = host->setup.mem.socket[socket].ddrCh[ch].enabled;
      host->nvram.mem.socket[socket].imc[GetMCID(host, socket, ch)].enabled = host->var.mem.socket[socket].imcEnabled[GetMCID(host, socket, ch)];
    }

    if (GetChnDisableChip (host, socket, ch)) {
      host->nvram.mem.socket[socket].channelList[ch].enabled = 0;
    }

    host->var.mem.socket[socket].channelList[ch].numDimmSlots = host->setup.mem.socket[socket].ddrCh[ch].numDimmSlots;
    //
    // Check if DIMM slots 1 and 2 are disabled in HW
    //
    if ((host->setup.mem.socket[socket].ddrCh[ch].numDimmSlots > 1) && GetDisable2dpcChip (host, socket)) {
      host->var.mem.socket[socket].channelList[ch].numDimmSlots = 1;
    }

    //
    // Check if DIMM slot 2 is disabled in HW
    //
    if ((host->setup.mem.socket[socket].ddrCh[ch].numDimmSlots > 2) && GetDisable3dpcChip (host, socket)) {
      host->var.mem.socket[socket].channelList[ch].numDimmSlots = 2;
    }

    dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

    for (dimm = 0; dimm < host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {

#ifdef XMP_SUPPORT
      if (host->setup.mem.options & MEM_OVERRIDE_EN &&
          ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot))
         ) {
        (*dimmNvList)[dimm].XMPChecksum = host->setup.mem.XMPChecksum[socket][ch][dimm];
      }
#endif // XMP_SUPPORT
      //
      // Only update the NVRAM structure if not resuming from S3 or warm boot
      //
      if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          (*dimmNvList)[dimm].mapOut[rank] = host->setup.mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank];

          //
          // All DIMMs behind a DIMM that is mapped out also have to be mapped out
          //
          //if ((dimm > 0) && (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm - 1].mapOut[rank])) {
          //  (*dimmNvList)[dimm].mapOut[rank] = 1;
          //}
#ifdef LRDIMM_SUPPORT
          //
          // For DDR4 LRDIMM, initialize the training data structures back to 0 before JEDEC INIT
          // so that all training will start from the same state. THe NVRAM data is only needed
          // for fast cold boot support
          //
          if (IsLrdimmPresent(host, socket, ch, dimm)) {
            rankList = GetRankNvList(host, socket, ch, dimm);

            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBC4x5x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBC8x9x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = 0;
              if (strobe < (MAX_STROBE / 2)) {
                  (*rankList)[rank].lrBuf_FxBCCxEx[strobe] = 0;
                  (*rankList)[rank].lrBuf_FxBCDxFx[strobe] = 0;
              }
            }
          }
#endif
        } // rank loop
      }
    } // dimm loop
  } // ch loop

  SetSteppingsChip (host, socket);

#ifdef SERIAL_DBG_MSG
  if (GetStickyScratchpad (host, 0, 0) & BIT4) {
    host->var.mem.serialDebugMsgLvl = host->var.mem.serialDebugMsgLvl | SDBG_MAX;
  }
#endif // SERIAL_DBG_MSG

#ifdef LRDIMM_SUPPORT
  host->var.mem.InPbaWaMode = 0;
  host->var.mem.InOvrRttPrkMode = 0;
#endif // LRDIMM_SUPPORT

  return SUCCESS;
}


UINT32
ProcPpinChangedCheck (
  PSYSHOST host,
  UINT8    socket
)
/*++

Routine Description:

  Check if any processor has been changed across boots

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  None.

--*/
//HSX HSD s5001717
{

  if (host->var.common.socketPresentBitMap & (BIT0 << socket)) {

    host->var.mem.socket[socket].procPpin = GetPpinChip (host, socket);

    if (host->var.mem.socket[socket].procPpin.hi == 0 && host->var.mem.socket[socket].procPpin.lo == 0) {
      // If PPIN is not supported, disable cold fast boot.
      rcPrintf ((host, "Cold fast boot not capable\n"));
      return TRUE;
    }

#ifdef SERIAL_DBG_MSG
    rcPrintf ((host,"Checking PPIN\n"));
    rcPrintf ((host,"nvram[%d].ppin.hi: 0x%x, var[%d].ppin.hi: 0x%x \n", socket, host->nvram.mem.socket[socket].procPpin.hi, socket, host->var.mem.socket[socket].procPpin.hi ));
    rcPrintf ((host,"nvram[%d].ppin.lo: 0x%x, var[%d].ppin.lo: 0x%x \n", socket, host->nvram.mem.socket[socket].procPpin.lo, socket, host->var.mem.socket[socket].procPpin.lo ));
#endif

    //
    // Check if processors changed
    //
    if ((host->nvram.mem.socket[socket].procPpin.hi != host->var.mem.socket[socket].procPpin.hi) || (host->nvram.mem.socket[socket].procPpin.lo != host->var.mem.socket[socket].procPpin.lo)) {
      rcPrintf ((host, "PPIN Changed\n"));
      return TRUE;
    }//if-procPpin
  }
  return FALSE;
}

UINT16
MemGetCmosDate (
  PSYSHOST host
  )
/*++

  Read and return a linear date based on year, month, and day of the month from RTC

  @param host  - Pointer to the system host (root) structure

  @retval linear approximate # of day's since Jan 1 2000

--*/
{
  UINT16 retVal = 0;
  UINT8  updateInProgragessBit = 1;
  UINT16 cmosDay, cmosMonth, cmosYear;

  // We do not poll the "update in progress" bit since we are only interested in date/month/year
  // and accuracy is not paramount.
  while (updateInProgragessBit) {
    OutPort8 (host, RTC_INDEX_REG, RTC_NMI_MASK | RTC_REG_A_INDEX);
    updateInProgragessBit = ((UINT8) ((InPort8 (host, RTC_DATA_REG)) >> 7) & 1); //Just read BIT7

    //MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    //              "updateInProgragessBit = %u\n", updateInProgragessBit));
    if (updateInProgragessBit == 0) break;
  }

  OutPort8 (host, RTC_INDEX_REG, RTC_NMI_MASK | RTC_DAY_OF_MONTH_REG); // Select Day of Month
  cmosDay = (UINT16) BCDtoDecimal(InPort8 (host, RTC_DATA_REG));


  OutPort8 (host, RTC_INDEX_REG, RTC_NMI_MASK | RTC_MONTH_REG); // Select Month
  cmosMonth = (UINT16) BCDtoDecimal(InPort8 (host, RTC_DATA_REG));

  OutPort8 (host, RTC_INDEX_REG, RTC_NMI_MASK | RTC_YEAR_REG); // Select year (2 digits)
  cmosYear = (UINT16) BCDtoDecimal(InPort8 (host, RTC_DATA_REG));

//  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
//                "Year = %u, Month = %u, Day = %u\n", cmosYear, cmosMonth, cmosDay));

  // Check for valid date range
  if ((cmosYear < 99) && ((cmosMonth > 0) && (cmosMonth <=12)) && ((cmosDay > 0) && (cmosDay <= 31))) {
    // Subtract one from month make 0 based for multiplication;
    cmosMonth = cmosMonth - 1;

    retVal = (cmosYear * 365) + (cmosMonth * 30) + cmosDay;
  }
  return (retVal);
}

