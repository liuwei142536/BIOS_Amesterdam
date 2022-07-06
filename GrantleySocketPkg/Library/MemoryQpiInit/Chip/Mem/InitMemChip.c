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
 * Copyright 2006 - 2016 Intel Corporation All Rights Reserved.
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
#include "MemPOR.h"
#include "DmtMain.h"

#ifdef SSA_FLAG
#include "MrcSsaServices.h"
#include <Pi/PiPeiCis.h>       //PeiServices
#include <Library/DebugLib.h> //ASSERT_EFI_ERROR
#endif  //SSA_FLAG
//
// DCLK ratios
//
const UINT8   DCLKs[MAX_SUP_FREQ] = {6, 10, 8, 12, 10, 14, 12, 18, 14, 20, 16, 22, 18, 26, 20, 28, 22, 30, 24};

//
// BCLK frequency
//
const UINT8   BCLKs[MAX_SUP_FREQ] = {133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133};

extern UINT8  lrdimmExtraOdtDelay[MAX_SUP_FREQ];
extern const UINT32 rxOffsetTable[MAX_STROBE];
extern const UINT8 tPARRECOVERY[MAX_SUP_FREQ];
extern const UINT8 tMOD_DDR4[MAX_SUP_FREQ];

//
// Local Prototypes
//
UINT32 InitMem(PSYSHOST host);
UINT32 InitADR(PSYSHOST host);
UINT32 EarlyConfig(PSYSHOST host);
UINT32 LateConfig(PSYSHOST host);
void   ExitSR(PSYSHOST host, UINT8 socket);
UINT32 PreTrainingInit(PSYSHOST host);
UINT32 PostTrainingInit(PSYSHOST host);
void   CheckLogicDelayTimingMode(PSYSHOST host, UINT8 socket);
UINT32 MemLate(PSYSHOST host);
void   CheckSecrets(PSYSHOST host);
UINT32 SwitchToNormalMode(PSYSHOST host);
void   PowerManagementSetup(PSYSHOST host, UINT8 socket);
// NVMDIMM_IF stuff (under construction)
VOID   EarlyDdrtConfig(PSYSHOST host, UINT8 socket);
void   ClearIMCErrors(PSYSHOST host);
VOID   ClearRASRegisters (PSYSHOST host, UINT8 socket);
UINT32 DDRResetLoop(PSYSHOST host);
// DRAM Maintenance
void   DramMaintenance(PSYSHOST host, UINT8 socket);
void   ConfigureTRRMode(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode);
UINT32 GetPassMask(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
UINT32 RunMmrc (PSYSHOST host);

#pragma warning(disable : 4740)

#define NO_POST_CODE  0xFF

// Special hack for vmse_err bit on Ch 2/3 for A0
#ifndef PCIE_REG_ADDR
#define PCIE_REG_ADDR(Bus,Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))
#endif

CallTableEntryType CallTableMaster[] = {
  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask, post_code, OEM command, policyFlag, iteration, debug_string
  {DispatchSlaves,      NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Dispatch Slaves")},
  {MemInitWarning,      NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Promote Warning Exception List")},
  {InitThrottlingEarly, NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Initialize Throttling Early")},
  {DetectDIMMConfig,    STS_DIMM_DETECT,      0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Detect DIMM Configuration")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Get Slave Data")},
  {CheckPORCompat,      NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Check POR Compatibility")},
  {InitDdrClocks,       STS_CLOCK_INIT,       0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Initialize DDR Clocks")},
  {CheckStatus,         NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_SERIAL,    CALL_TABLE_STRING("Check Status")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Send Status")},
  {CheckVdd,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Set Vdd")},
  {CheckDimmRanks,      STS_RANK_DETECT,      0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Check DIMM Ranks")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Send Data")},
  {InitMem,             NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Initialize Memory")},

  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask, post_code, OEM command, policyFlag, iteration, debug_string
  {GatherSPDData,       STS_SPD_DATA,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Gather SPD Data")},
  {ConfigureXMP,        NO_POST_CODE,         0,                        NULL,   MRC_PF_COLD,  MRC_MP_BOTH,      CALL_TABLE_STRING("Configure XMP")},
#ifdef MEM_NVDIMM_EN
  {CoreNVDIMMStatus,NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform NVDIMM Status")},
#endif //MEM_NVDIMM_EN
  {EarlyConfig,         STS_GLOBAL_EARLY,     0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early Configuration")},
  {InitDdrioInterface,  STS_DDRIO_INIT,       0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("DDRIO Initialization")},
  {PreTrainingInit,     NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Pre-Training Initialization")},
  {EarlyCtlClk,         STS_CHANNEL_TRAINING, EARLY_CTL_CLK        ,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early CTL/CLK")},
  {EarlyCmdClk,         STS_CHANNEL_TRAINING, EARLY_CMD_CLK        ,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early CMD/CLK")},
#ifdef LRDIMM_SUPPORT
  {Ddr4LrdimmBacksideRxPhase, STS_CHANNEL_TRAINING, RX_BACKSIDE_PHASE_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Phase RX")},
  {Ddr4LrdimmBacksideRxCycle, STS_CHANNEL_TRAINING, RX_BACKSIDE_CYCLE_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Cycle RX")},
  {Ddr4LrdimmBacksideRxDelay, STS_CHANNEL_TRAINING, RX_BACKSIDE_DELAY_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Delay RX")},
#endif
  {ReceiveEnable,       STS_CHANNEL_TRAINING, RECEIVE_ENABLE_BASIC, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Receive Enable")},
  {ReadDqDqs,           STS_CHANNEL_TRAINING, RX_DQ_DQS_BASIC,      NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Dq/Dqs Basic")},
#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
  {DqSwizzleDiscovery,  STS_CHANNEL_TRAINING, DQ_SWIZZLE_DISCOVERY, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Dq Swizzle Discovery")},
#endif
#ifdef LRDIMM_SUPPORT
  {Ddr4LrdimmBacksideTxFineWL, STS_CHANNEL_TRAINING, TX_BACKSIDE_FINE_WL_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Fine WL")},
  {Ddr4LrdimmBacksideTxCoarseWL, STS_CHANNEL_TRAINING, TX_BACKSIDE_COARSE_WL_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Coarse WL")},
  {Ddr4LrdimmBacksideTxDelay, STS_CHANNEL_TRAINING, TX_BACKSIDE_DELAY_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Delay TX")},
#endif
  {WriteLeveling,       STS_CHANNEL_TRAINING, WRITE_LEVELING_BASIC, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Write Leveling")},
  {WriteLevelingCleanUp, STS_CHANNEL_TRAINING, WR_FLY_BY,           NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Write Fly By")},
  {WrDqDqs,             STS_CHANNEL_TRAINING, TX_DQ_BASIC,          NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Dq Basic")},
  {PostPackageRepairMain,   STS_CHANNEL_TRAINING, PPR_FLOW,             NULL,   MRC_PF_ALL,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("PPR Flow")},
#ifndef RC_SIM
  {WrEarlyVrefCentering,STS_CHANNEL_TRAINING, E_TRAIN_WR_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Early Vref Centering")},
  {RdEarlyVrefCentering,STS_CHANNEL_TRAINING, E_TRAIN_RD_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Early Vref Centering")},
#endif
  {CmdVrefCentering,    STS_CHANNEL_TRAINING, CMD_VREF_CENTERING,   NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("CMD Vref Centering")},
  {LateCmdClk,          STS_CHANNEL_TRAINING, LATE_CMD_CLK,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Late Cmd/Clk")},
  {TrainTxEq,           STS_CHANNEL_TRAINING, TX_EQ_TRAINING,       NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Eq")},
  {TrainImode,          STS_CHANNEL_TRAINING, IMODE_TRAINING,       NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Imode")},
  {TrainCTLE,           STS_CHANNEL_TRAINING, T_CTLE,               NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("CTLE")},
  {TxPerBitDeskew,      STS_CHANNEL_TRAINING, PER_BIT_DESKEW_TX,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Per Bit Deskew")},
  {RxPerBitDeskew,      STS_CHANNEL_TRAINING, PER_BIT_DESKEW_RX,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Per Bit Deskew")},
#ifdef LRDIMM_SUPPORT
  {WrLRDIMMVrefCentering, STS_CHANNEL_TRAINING, TRAIN_WR_VREF_LRDIMM, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Vref Centering (LRDIMM)")},
  {RdLRDIMMVrefCentering, STS_CHANNEL_TRAINING, TRAIN_RD_VREF_LRDIMM, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Vref Centering (LRDIMM)")},
  {TxLRDIMMDqCentering,   STS_CHANNEL_TRAINING, TX_LRDIMM_DQ_CENTERING, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Dq Centering (LRDIMM)")},
  {RxLRDIMMDqCentering,   STS_CHANNEL_TRAINING, RX_LRDIMM_DQ_CENTERING, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Dq Centering (LRDIMM)")},
#endif
  {WrVrefCentering,     STS_CHANNEL_TRAINING, TRAIN_WR_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Vref Centering")},
  {RdVrefCentering,     STS_CHANNEL_TRAINING, TRAIN_RD_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Vref Centering")},
  {WrAdvancedCentering, STS_CHANNEL_TRAINING, TRAIN_WR_DQS,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Dq Adv")},
  {RdAdvancedCentering, STS_CHANNEL_TRAINING, TRAIN_RD_DQS,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Dq/Dqs Adv")},
  {RoundTripOptimize,   STS_CHANNEL_TRAINING, ROUND_TRIP_OPTIMIZE,  NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Round Trip Optimization")},
#ifdef   SERIAL_DBG_MSG
  {DisplayTrainResults,   NO_POST_CODE,         0,                  NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Display Training Results")},
#endif // SERIAL_DBG_MSG
  {PostTrainingInit,      NO_POST_CODE,         0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Post-Training Initialization")},
#ifdef MARGIN_CHECK
  {RankMarginTool,        STS_CHANNEL_TRAINING,         STS_RMT,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rank Margin Tool")},
#endif
#ifdef  BDAT_SUPPORT
  {FillBDATStructure,     NO_POST_CODE,         0,                  NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Fill BDAT Structure")},
#endif  // BDAT_SUPPORT
#ifdef SSA_FLAG
  {SSAInit,               SSA_API_INIT,        SSA_API_INIT_START,  NULL,   MRC_PF_ALL,   MRC_MP_BOTH,  CALL_TABLE_STRING("BIOS SSA Initialization")},
#endif  // SSA_FLAG
#ifdef MEM_NVDIMM_EN
  {CoreRestoreNVDIMMs,    NO_POST_CODE,         0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform Restore NVDIMMs")},
  {CoreArmNVDIMMs,NO_POST_CODE,         0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform Arm NVDIMMs")},
#endif  //MEM_NVDIMM_EN
  {LateConfig,          STS_CHANNEL_TRAINING, STS_MINOR_END_TRAINING,              NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Late Configuration")},
  {InitThrottling,      STS_INIT_THROTTLING,  0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Initialize Throttling")},
  {AdvancedMemTest,     STS_MEMBIST,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Advanced MemTest")},
  {MemTest,             STS_MEMBIST,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("MemTest")},
  {MemInit,             STS_MEMINIT,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("MemInit")},
  {CheckRASSupportAfterMemInit,  NO_POST_CODE,  0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Check Ras Support After MemInit")},
  {SwitchToNormalMode,  STS_CHANNEL_TRAINING, STS_MINOR_NORMAL_MODE,NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Switch to Normal Mode")},
#ifndef DE_SKU
  {InitADR,             NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Initialize ADR")},
#endif
  {PipeSync,            NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Get NVRAM Data")},
  {InitMemoryMap,       STS_DDR_MEMMAP,       0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Initialize Memory Map")},
  {SetRASConfig,        STS_RAS_CONFIG,       0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Set RAS Configuration")},
  {MemLate,             NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Memory Late")},
#ifdef SERIAL_DBG_MSG
  {DIMMInfo,            NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("DIMM Information")},
#endif // SERIAL_DBG_MSG
};

UINT8 CallTableMasterSize = sizeof(CallTableMaster) / sizeof(CallTableEntryType);

CallTableEntryType CallTableSlave[] = {
  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask,         Major POST Code, Minor POST Code,      OEM command, policyFlag,     MP flags,          Debug String
  {SlaveAck,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Slave Acknoledge")},
  {MemInitWarning,      NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Promote Warning Exception List")},
  {InitThrottlingEarly, NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Initialize Throttling Early")},
  {DetectDIMMConfig,    STS_DIMM_DETECT,      0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Detect DIMM Configuration")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Send Slave Data")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Get Status")},
  {PipeSync,            NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Get Data")},
  {InitMem,             NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP, CALL_TABLE_STRING("Initialize Memory")},

  // The functions are executed in the following order, as the policy flag dictates.
  // Mrctask, post_code, OEM command, policyFlag, iteration, debug_string
  {GatherSPDData,       STS_SPD_DATA,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Gather SPD Data")},
  {WaitForConfigureXMP, NO_POST_CODE,         0,                        NULL,   MRC_PF_COLD,  MRC_MP_PARALLEL,   CALL_TABLE_STRING("Wait for Configure XMP")},
#ifdef MEM_NVDIMM_EN
  {CoreNVDIMMStatus,NO_POST_CODE,         0,                            NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform NVDIMM Status")},
#endif //MEM_NVDIMM_EN
  {EarlyConfig,         STS_GLOBAL_EARLY,     0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early Configuration")},
  {InitDdrioInterface,  STS_DDRIO_INIT,       0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("DDRIO Initialization")},
  {PreTrainingInit,     NO_POST_CODE,         0,                        NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Pre-Training Initialization")},
  {EarlyCtlClk,         STS_CHANNEL_TRAINING, EARLY_CTL_CLK,            NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early CTL/CLK")},
  {EarlyCmdClk,         STS_CHANNEL_TRAINING, EARLY_CMD_CLK,            NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Early CMD/CLK")},
#ifdef LRDIMM_SUPPORT
  {Ddr4LrdimmBacksideRxPhase, STS_CHANNEL_TRAINING, RX_BACKSIDE_PHASE_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Phase RX")},
  {Ddr4LrdimmBacksideRxCycle, STS_CHANNEL_TRAINING, RX_BACKSIDE_CYCLE_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Cycle RX")},
  {Ddr4LrdimmBacksideRxDelay, STS_CHANNEL_TRAINING, RX_BACKSIDE_DELAY_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Delay RX")},
#endif
  {ReceiveEnable,       STS_CHANNEL_TRAINING, RECEIVE_ENABLE_BASIC, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Receive Enable")},
  {ReadDqDqs,           STS_CHANNEL_TRAINING, RX_DQ_DQS_BASIC,      NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Dq/Dqs Basic")},
#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
  {DqSwizzleDiscovery,  STS_CHANNEL_TRAINING, DQ_SWIZZLE_DISCOVERY, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Dq Swizzle Discovery")},
#endif
#ifdef LRDIMM_SUPPORT
  {Ddr4LrdimmBacksideTxFineWL, STS_CHANNEL_TRAINING, TX_BACKSIDE_FINE_WL_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Fine WL")},
  {Ddr4LrdimmBacksideTxCoarseWL, STS_CHANNEL_TRAINING, TX_BACKSIDE_COARSE_WL_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Coarse WL")},
  {Ddr4LrdimmBacksideTxDelay, STS_CHANNEL_TRAINING, TX_BACKSIDE_DELAY_TRAINING, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Lrdimm BS Delay TX")},
#endif
  {WriteLeveling,       STS_CHANNEL_TRAINING, WRITE_LEVELING_BASIC, NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Write Leveling")},
  {WriteLevelingCleanUp, STS_CHANNEL_TRAINING, WR_FLY_BY,           NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Write Fly By")},
  {WrDqDqs,             STS_CHANNEL_TRAINING, TX_DQ_BASIC,          NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Dq Basic")},
  {PostPackageRepairMain,   STS_CHANNEL_TRAINING, PPR_FLOW,             NULL,   MRC_PF_ALL,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("PPR Flow")},
  {WrEarlyVrefCentering,STS_CHANNEL_TRAINING, E_TRAIN_WR_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Early Vref Centering")},
  {RdEarlyVrefCentering,STS_CHANNEL_TRAINING, E_TRAIN_RD_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Early Vref Centering")},
  {CmdVrefCentering,    STS_CHANNEL_TRAINING, CMD_VREF_CENTERING,   NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("CMD Vref Centering")},
  {LateCmdClk,          STS_CHANNEL_TRAINING, LATE_CMD_CLK,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Late Cmd/Clk")},
  {TrainTxEq,           STS_CHANNEL_TRAINING, TX_EQ_TRAINING,       NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Eq")},
  {TrainImode,          STS_CHANNEL_TRAINING, IMODE_TRAINING,       NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Imode")},
  {TrainCTLE,           STS_CHANNEL_TRAINING, T_CTLE,               NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("CTLE")},
  {TxPerBitDeskew,      STS_CHANNEL_TRAINING, PER_BIT_DESKEW_TX,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Per Bit Deskew")},
  {RxPerBitDeskew,      STS_CHANNEL_TRAINING, PER_BIT_DESKEW_RX,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Per Bit Deskew")},
#ifdef LRDIMM_SUPPORT
  {WrLRDIMMVrefCentering, STS_CHANNEL_TRAINING, TRAIN_WR_VREF_LRDIMM, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Vref Centering (LRDIMM)")},
  {RdLRDIMMVrefCentering, STS_CHANNEL_TRAINING, TRAIN_RD_VREF_LRDIMM, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Vref Centering (LRDIMM)")},
  {TxLRDIMMDqCentering,   STS_CHANNEL_TRAINING, TX_LRDIMM_DQ_CENTERING, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Dq Centering (LRDIMM)")},
  {RxLRDIMMDqCentering,   STS_CHANNEL_TRAINING, RX_LRDIMM_DQ_CENTERING, NULL, MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Dq Centering (LRDIMM)")},
#endif
  {WrVrefCentering,     STS_CHANNEL_TRAINING, TRAIN_WR_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Wr Vref Centering")},
  {RdVrefCentering,     STS_CHANNEL_TRAINING, TRAIN_RD_VREF,        NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rd Vref Centering")},
  {WrAdvancedCentering, STS_CHANNEL_TRAINING, TRAIN_WR_DQS,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Tx Dq Adv")},
  {RdAdvancedCentering, STS_CHANNEL_TRAINING, TRAIN_RD_DQS,         NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rx Dq/Dqs Adv")},
  {RoundTripOptimize,   STS_CHANNEL_TRAINING, ROUND_TRIP_OPTIMIZE,  NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Round Trip Optimization")},
#ifdef   SERIAL_DBG_MSG
  {DisplayTrainResults,   NO_POST_CODE,         0,                  NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Display Training Results")},
#endif// SERIAL_DBG_MSG
  {PostTrainingInit,      NO_POST_CODE,         0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Post-Training Initialization")},
#ifdef MARGIN_CHECK
  {RankMarginTool,        STS_CHANNEL_TRAINING,         STS_RMT,    NULL,   MRC_PF_COLD,  MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Rank Margin Tool")},
#endif
#ifdef MEM_NVDIMM_EN
  {CoreRestoreNVDIMMs,    NO_POST_CODE,        0,                   NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform Restore NVDIMMs")},
  {CoreArmNVDIMMs,NO_POST_CODE,        0,                   NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Platform Arm NVDIMMs")},
#endif  //MEM_NVDIMM_EN
  {LateConfig,          STS_CHANNEL_TRAINING, STS_MINOR_END_TRAINING,            NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Late Configuration")},
  {InitThrottling,      STS_INIT_THROTTLING,  0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Initialize Throttling")},
  {AdvancedMemTest,     STS_MEMBIST,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Advanced MemTest")},
  {MemTest,             STS_MEMBIST,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("MemTest")},
  {MemInit,             STS_MEMINIT,          0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("MemInit")},
  {CheckRASSupportAfterMemInit,  NO_POST_CODE,  0,                  NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Check Ras Support After MemInit")},
  {SwitchToNormalMode,  STS_CHANNEL_TRAINING, STS_MINOR_NORMAL_MODE,NULL,   MRC_PF_ALL,   MRC_MP_BOTH_LOOP,  CALL_TABLE_STRING("Switch to Normal Mode")},
#ifndef DE_SKU
  {InitADR,             NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_BOTH,      CALL_TABLE_STRING("Initialize ADR")},
#endif
  {PipeSync,            NO_POST_CODE,         0,                    NULL,   MRC_PF_ALL,   MRC_MP_PARALLEL,  CALL_TABLE_STRING("Send NVRAM Data")},
};

UINT8 CallTableSlaveSize = sizeof(CallTableSlave) / sizeof(CallTableEntryType);

VddValues   VddValuesTable[] = {
  {(SPD_VDD_135 | SPD_VDD_125), MAILBOX_BIOS_125V_RAMP_ENCODING_VR12, MAILBOX_BIOS_125V_RAMP_ENCODING_VR12_5, 0xff, 0xff, CALL_TABLE_STRING("1.25V\n")},
  {SPD_VDD_150,                 MAILBOX_BIOS_150V_RAMP_ENCODING_VR12, MAILBOX_BIOS_150V_RAMP_ENCODING_VR12_5, 0xff, 0xff, CALL_TABLE_STRING("1.50V\n")},
  {SPD_VDD_135,                 MAILBOX_BIOS_135V_RAMP_ENCODING_VR12, MAILBOX_BIOS_135V_RAMP_ENCODING_VR12_5, 0xff, 0xff, CALL_TABLE_STRING("1.35V\n")},
  {SPD_VDD_120,                 MAILBOX_BIOS_120V_RAMP_ENCODING_VR12, MAILBOX_BIOS_120V_RAMP_ENCODING_VR12_5, 0xff, 0xff, CALL_TABLE_STRING("1.20V\n")},
  {0xFF,                        0xFF,                                 0xFF                                  , 0xff, 0xff, CALL_TABLE_STRING("Unknown VDD\n")},
};

UINT8 VddValuesTableSize = sizeof(VddValuesTable) / sizeof(VddValues);

// -------------------------------------
// Code section
// -------------------------------------

/**
  This function implements a MRC reset loop using scratchpad registers

  @param host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
DDRResetLoop (
  PSYSHOST                  host
  )
{
  UINT32        spad5;
  UINT32        spad0;
  UINT16        tempCounter;

  if (host->setup.mem.options & DDR_RESET_LOOP) {
    //
    // Check bit 2 to see if we need to keep cycling.
    //
    spad5 = ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);
    spad0 = ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG);
    //
    // If the user has requested MRC cycling and this is the first
    // time through the loop, set BIT2 of SPAD0 here to keep cycling
    // in the future.  Someone can then clear this bit with ITP or
    // external tools to stop cycling and proceed to system boot.
    //
    if ((spad5 & 0xFFFF) == 0) {
      spad0 |= BIT2;
      WriteCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD0_UBOX_MISC_REG, spad0);
    }
    if (spad0 & BIT2) {
      //
      // Increment the loop count.
      //
      tempCounter = (UINT16) spad5;
      tempCounter++;
      spad5 = (spad5 & 0xFFFF0000) + tempCounter;
      WriteCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, spad5);
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "DDR LC: %d\n", tempCounter));
      host->var.common.resetRequired |= POST_RESET_WARM;
      return  FAILURE;
    } else {
      //
      // Clear out the counter since we're done cycling.
      //
      spad5 = ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG);
      WriteCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD5_UBOX_MISC_REG, (spad5 & 0xFFFF0000));
    }
  } // if DDR_RESET_LOOP

  return SUCCESS;
} // DDRResetLoop

/**

  This routine gets memory ready to be written and read

  @param host  - Pointer to sysHost

  @retval N/A

**/
UINT32
InitMem (
  PSYSHOST host
  )
{
  UINT8                           socket;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT  mcInitStateG;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  // Clear WorkAround register prior to starting
  host->nvram.mem.socket[socket].wa = 0;

  //
  // Force CS to be driven during traning
  //
  mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
  mcInitStateG.Bits.cs_oe_en = 0xF;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);


  ClearIMCErrors(host);
  ClearRASRegisters (host, socket);

  return SUCCESS;
} // InitMem

/**

  This routine gets things ready for DDR training

  @param host  - Pointer to sysHost

  @retval N/A

**/
UINT32
PreTrainingInit (
  PSYSHOST host
  )
{
  UINT8                          ch;
  UINT8                          socket = host->var.mem.currentSocket;
  struct channelNvram            (*channelNvList)[MAX_CH];
  CPGC_INORDER_MCDDC_CTL_STRUCT  cpgcInOrder;

  if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast ||

  host->var.mem.subBootMode == ColdBootFast) return SUCCESS;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  channelNvList = &host->nvram.mem.socket[socket].channelList;


  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    cpgcInOrder.Data = MemReadPciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG);
    cpgcInOrder.Bits.wpq_inorder_en = 1;
    MemWritePciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG, cpgcInOrder.Data);
  } // ch loop

  InitStartingOffset (host, socket);

  return SUCCESS;
} // PreTrainingInit

/**

  This routine is run immediately after DDR training

  @param host  - Pointer to sysHost

  @retval N/A

**/
UINT32
PostTrainingInit (
  PSYSHOST host
  )
{
  UINT8 socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;

  IO_Reset(host, socket);

  //
  // WA - HSD: s4166802 - Sets the dis_ddrio_pwdn bit when running with 2n command timing and LogicDelay=3
  //
  if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
    CheckLogicDelayTimingMode(host, socket);
  }

  //
  // Update dimmmtr.rank_disable setting on mapout results after training
  //
  CheckRankPopLocal(host, socket);

  if (host->setup.mem.optionsExt & TURNAROUND_OPT_EN) {
    //
    // Program turnaround timings
    //
    TurnAroundTimings (host, socket);

#ifdef  DEBUG_TURNAROUDS
    //
    // Check turnaround timings and report via scratchpad
    //
    CheckTurnAroundTimings (host, socket);
#endif  //  DEBUG_TURNAROUDS
  }

  host->var.mem.DMTestPass[socket] = 1;

  if (host->setup.mem.DramMaintenanceTest == TRUE) {
    host->var.mem.DMTestPass[socket] = 0;
    DcttHt(host, socket);
    host->var.mem.DMTestPass[socket] = 1;
  }

  return SUCCESS;
} // PostTrainingInit


void
CheckLogicDelayTimingMode (
    PSYSHOST host,
    UINT8 socket
  )
/*++

  This routine is called within PostTrainingInit()
  WA - HSD: 4166802 - HSX Clone: HSX A0 PO: Self Refresh Commands do not push CS Low
  Sets the dis_ddrio_pwdn bit when running with 2n command timing and LogicDelay=3

  @param host   - Pointer to sysHost
  @param socket - Current socket

  @retval N/A

--*/
{
  GSM_GT                               ct_all_Group = 0;
  UINT8                                ch;
  UINT16                               maxCmdDelay;
  UINT16                               minCmdDelay;
  INT16                                delay;
  UINT16                               logicDelay = 0;
  struct channelNvram                  (*channelNvList)[MAX_CH];
  MCSCHED_CHKN_BIT2_MCDDC_CTL_STRUCT   ddriopwd;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  ct_all_Group = CtlAll;

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    minCmdDelay = MAX_CMD_MARGIN;
    maxCmdDelay = 0;
    GetSetCtlGroupDelay (host, socket, ch, ct_all_Group, GSM_READ_ONLY, &delay, &minCmdDelay, &maxCmdDelay);

    // Calculate the logicDelay
    logicDelay = (maxCmdDelay / 64);

    if ((logicDelay == 3) && ((*channelNvList)[ch].timingMode == TIMING_2N)) {
      ddriopwd.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT2_MCDDC_CTL_REG);
      ddriopwd.Bits.dis_ddrio_pwdn = 1;
      MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT2_MCDDC_CTL_REG, ddriopwd.Data );
    }
  }
} // CheckLogicDelayTimingMode



/**

  This routine gets memory ready to be written and read

  @param host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
EarlyConfig (
  PSYSHOST  host
  )
{
  UINT8                                     socket;
  UINT8                                     ch;
  UINT8                                     dimm;
  UINT8                                     rank;
  UINT8                                     strobe;
  UINT8                                     x4Mode;
  UINT8                                     nCLOrg;
  UINT8                                     casSupBase;
  UINT8                                     mcId;
  UINT8                                     maxCL;
  UINT16                                    csrRegDid;
  UINT32                                    bus;
  UINT32                                    dev;
  UINT32                                    func;
  UINT8                                     *regaddr;
  CSR_OFFSET                                RegOffset;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  struct ddrRank                            (*rankList)[MAX_RANK_DIMM];
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT          scrambleConfig;
  RXOFFSETN0RANK0_0_MCIO_DDRIO_STRUCT       rxOffset;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT              mcMtr;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT    mcInitStateG;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT chknBit;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT           dimmMtr;
  VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT        vmseError;
  TCLRDP_MCDDC_CTL_HSX_BDX_STRUCT           pagetbl;
#ifdef LRDIMM_SUPPORT
  UINT32                                    rm2xx;
#endif
  PMONUNITCTRL_MCDDC_CTL_STRUCT             pmonUnitCtrl;
  PMONCNTRCFG_1_MCDDC_CTL_STRUCT            pmonCntrCfg;

  socket = host->var.mem.currentSocket;

  if (host->setup.mem.options & SCRAMBLE_EN) {
    if ((host->var.common.bootMode == NormalBoot) &&
        ((host->var.mem.subBootMode == ColdBoot) || (host->var.mem.subBootMode == WarmBoot))) {

      //
      // Verify the value is in range if not use default.
      //
      if (host->setup.mem.scrambleSeedLow > MAX_SCRAMBLE_SEED_LOW){
        host->nvram.mem.scrambleSeed = (host->setup.mem.scrambleSeedHigh << 16) | SCRAMBLE_SEED_LOW;
      } else {
        host->nvram.mem.scrambleSeed = (host->setup.mem.scrambleSeedHigh << 16) | host->setup.mem.scrambleSeedLow;
      }
    }
  } // if scrambling

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return SUCCESS;


  //
  // IMC device ID
  //
  csrRegDid = (UINT16) MemReadPciCfgMain (host, socket, DID_MC_MAIN_REG);
  host->nvram.mem.socket[socket].imcDid = csrRegDid;  // Save IMC DVID for later use

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    mcMtr.Data        = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
    //
    // Enable ECC if required
    //
    if (host->nvram.mem.eccEn) {
      mcMtr.Bits.ecc_en = 1;
    }

    //
    // Enable DDR4 if required
    //
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      mcMtr.Bits.ddr4 = 1;
    }

    //
    // Enter CPGC training mode
    //
    mcMtr.Bits.trng_mode = 1;
    MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } // mcId loop

  //
  // Enable the DCLK
  //
  mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
  mcInitStateG.Bits.dclk_enable = 1;
  mcInitStateG.Bits.reset_io    = 0;
  MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);

  //
  // Check if there are secrets in memory
  //
  CheckSecrets(host);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
    //
    // Disable 2 cycle bypass before any CPGC activity
    //
    chknBit.Bits.dis_2cyc_byp = 1;
    chknBit.Bits.cmd_oe_alwy_on = 1;
    chknBit.Bits.dis_opp_ras = 1;
    if (!CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) chknBit.Bits.ovrd_odt_to_io = 1;
    MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit.Data);
  }

  if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        rankList = &(*dimmNvList)[dimm].rankList;
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          //
          // Construct CKE mask
          //
          (*channelNvList)[ch].ckeMask |= 1 << (*rankList)[rank].CKEIndex;

          for (strobe = 0; strobe < MAX_STROBE; strobe++) {

            rxOffset.Data = 0;
            // RxDqsPsel = 1 means OTHER amplifier output.
            // All Bytes, Nibble 0 = always 0
            // All Bytes, Nibble 1 = 1 if x8
            // All Bytes, Nibble 1 = 0 if x4
            if ((*dimmNvList)[dimm].x4Present || (strobe < 9)) {
              rxOffset.Bits.rxdqspsel = 0;
            } else {
              rxOffset.Bits.rxdqspsel = 1;
            }

            (*channelNvList)[ch].rxOffset[(*rankList)[rank].rankIndex][strobe] = rxOffset.Data;
            MemWritePciCfgEp (host, socket, ch, rxOffsetTable[strobe] + ((*rankList)[rank].logicalRank * 4), rxOffset.Data);
          } // strobe loop
        } // rank loop
      } // dimm loop

      //
      // tRCD must be at least 5
      //
      if ((*channelNvList)[ch].common.nRCD < 5) {
        (*channelNvList)[ch].common.nRCD = 5;
      }

      //
      // tRP must be at least 5
      //
      if ((*channelNvList)[ch].common.nRP < 5) {
        (*channelNvList)[ch].common.nRP = 5;
      }

      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        //
        // tWTR must be at least 4
        //
        if ((*channelNvList)[ch].common.nWTR < 4) {
          (*channelNvList)[ch].common.nWTR = 4;
        }
      } else { // DDR4
        //
        // tWTR_S must be at least 2
        //
        if ((*channelNvList)[ch].common.nWTR < 2) {
          (*channelNvList)[ch].common.nWTR = 2;
        }
        //
        // tWTR_L must be at least 4
        //
        if ((*channelNvList)[ch].common.nWTR_L < 4) {
          (*channelNvList)[ch].common.nWTR_L = 4;
        }
      }

      //
      // tRAS must be at least 10
      //
      if ((*channelNvList)[ch].common.nRAS < 10) {
        (*channelNvList)[ch].common.nRAS = 10;
      }

      //
      // tRAS must be at most 63
      //
      if ((*channelNvList)[ch].common.nRAS > 63) {
        (*channelNvList)[ch].common.nRAS = 63;
      }

      //
      // tRTP must be at least 4
      //
      if ((*channelNvList)[ch].common.nRTP < 4) {
        (*channelNvList)[ch].common.nRTP = 4;
      }

      //
      // tRRD must be at least 4
      //
      if ((*channelNvList)[ch].common.nRRD < 4) {
        (*channelNvList)[ch].common.nRRD = 4;
      }

      //
      // tRRD_L must be at least 4
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4){
        // WA for DDR4 1333 t_RRDL = 5
        if (host->nvram.mem.socket[socket].ddrFreq == DDR_1333){
          if ((*channelNvList)[ch].common.nRRD_L < 5){
          (*channelNvList)[ch].common.nRRD_L = 5;
          }
        } else {
          if ((*channelNvList)[ch].common.nRRD_L < 4){
            (*channelNvList)[ch].common.nRRD_L = 4;
          }
        }
      }
    } // ch loop
  } // S3

  for (ch = 0; ch < MAX_CH; ch++) {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG);
      dimmMtr.Bits.ddr4_mode = 1;
      MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG, dimmMtr.Data);
    }
    if ((*channelNvList)[ch].enabled == 0) continue;

    //
    // Initialize to 0
    //
    x4Mode      = 0;

    dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      //
      // Write the DIMM Memory Technology information to DIMMMTR
      //
      MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4), (UINT32) (*dimmNvList)[dimm].dimmMemTech);

      //
      // Check if this is a x4 DIMM
      //
      if ((*dimmNvList)[dimm].x4Present) {
        x4Mode |= (1 << dimm);
      }
    } // dimm loop

    //
    // Set bits indicating which DIMMs are x4 if any
    //
    if (x4Mode) {
      MemWritePciCfgEp (host, socket, ch, X4MODESEL_MCDDC_DP_REG, x4Mode);
    }

    if (host->var.common.bootMode == NormalBoot) {

      if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
        casSupBase = 4;
        maxCL = 15;
      } else {
        casSupBase = 7;
        maxCL = 32;
      }

      //
      // Use common timings across channels if in lockstep or mirror
      //
      if (((host->nvram.mem.RASmode == CH_LOCKSTEP) || (host->nvram.mem.RASmode == CH_MIRROR)) && ((ch == 0) || (ch == 2))) {

        FastestCommon   (&((*channelNvList)[ch].common.nRCD), &((*channelNvList)[ch + 1].common.nRCD), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nRP),  &((*channelNvList)[ch + 1].common.nRP),  0);
        FastestCommon   (&((*channelNvList)[ch].common.nRC),  &((*channelNvList)[ch + 1].common.nRC),  0);
        FastestCommon16 (&((*channelNvList)[ch].common.nRFC), &((*channelNvList)[ch + 1].common.nRFC), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nWTR), &((*channelNvList)[ch + 1].common.nWTR), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nRAS), &((*channelNvList)[ch + 1].common.nRAS), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nRTP), &((*channelNvList)[ch + 1].common.nRTP), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nFAW), &((*channelNvList)[ch + 1].common.nFAW), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nRRD), &((*channelNvList)[ch + 1].common.nRRD), 0);
        FastestCommon   (&((*channelNvList)[ch].common.nWR),  &((*channelNvList)[ch + 1].common.nWR),  0);
        FastestCommon   (&((*channelNvList)[ch].common.nCL),  &((*channelNvList)[ch + 1].common.nCL),  0);

        //
        // Make sure the selected CAS Latency is supported by each DIMM on both channels
        //
        nCLOrg = (*channelNvList)[ch].common.nCL;

        while ((!((1 << ((*channelNvList)[ch].common.nCL - casSupBase)) & (*channelNvList)[ch].common.casSup)) ||
               (!((1 << ((*channelNvList)[ch + 1].common.nCL - casSupBase)) & (*channelNvList)[ch + 1].common.casSup)))
        {
          (*channelNvList)[ch].common.nCL++;
          (*channelNvList)[ch + 1].common.nCL++;

          if ((*channelNvList)[ch].common.nCL == (maxCL + 1)) {
            // Use the original version of nCL if we can't find a good one
            (*channelNvList)[ch].common.nCL = nCLOrg;
            (*channelNvList)[ch + 1].common.nCL = nCLOrg;
            break;
          }
        }
      } else {
        //
        // Make sure the selected CAS Latency is supported by each DIMM on this channel
        //
        if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
          casSupBase = 4;
          maxCL = 15;
        } else {
          casSupBase = 7;
          maxCL = 24;
        }

        nCLOrg = (*channelNvList)[ch].common.nCL;

        while (!((1 << ((*channelNvList)[ch].common.nCL - casSupBase)) & (*channelNvList)[ch].common.casSup)) {
          (*channelNvList)[ch].common.nCL++;

          if ((*channelNvList)[ch].common.nCL >= maxCL) {
            // Use the original version of nCL if we can't find a good one
            (*channelNvList)[ch].common.nCL = nCLOrg;
            break;
          }
        }
      }
    } // S3 resume

#ifdef XMP_SUPPORT
    if ((host->var.common.bootMode == NormalBoot) && (host->var.mem.subBootMode == ColdBoot)) {
      // Save the default standard timings
      (*channelNvList)[ch].standard.nRRD = (*channelNvList)[ch].common.nRRD;
      (*channelNvList)[ch].standard.nRRD_L = (*channelNvList)[ch].common.nRRD_L;
      (*channelNvList)[ch].standard.nWTR = (*channelNvList)[ch].common.nWTR;
      (*channelNvList)[ch].standard.nRP  = (*channelNvList)[ch].common.nRP;
      (*channelNvList)[ch].standard.nRAS = (*channelNvList)[ch].common.nRAS;
      (*channelNvList)[ch].standard.nRCD = (*channelNvList)[ch].common.nRCD;
      (*channelNvList)[ch].standard.nRC  = (*channelNvList)[ch].common.nRC;
      (*channelNvList)[ch].standard.nRTP = (*channelNvList)[ch].common.nRTP;
      (*channelNvList)[ch].standard.nRFC = (*channelNvList)[ch].common.nRFC;
      (*channelNvList)[ch].standard.nWR  = (*channelNvList)[ch].common.nWR;
      (*channelNvList)[ch].standard.nCL  = (*channelNvList)[ch].common.nCL;
      (*channelNvList)[ch].standard.nFAW = (*channelNvList)[ch].common.nFAW;
    } // S3

    //
    // Check for manual overrides
    //
    if (host->setup.mem.options & MEM_OVERRIDE_EN) {
      if (host->setup.mem.inputMemTime.nRRD != 0) {
        (*channelNvList)[ch].common.nRRD = host->setup.mem.inputMemTime.nRRD;
      }

      if (host->setup.mem.inputMemTime.nRRD_L != 0) {
        (*channelNvList)[ch].common.nRRD_L = host->setup.mem.inputMemTime.nRRD_L;
      }

      if (host->setup.mem.inputMemTime.nWTR != 0) {
        (*channelNvList)[ch].common.nWTR = host->setup.mem.inputMemTime.nWTR;
      }

      if (host->setup.mem.inputMemTime.nRP != 0) {
        (*channelNvList)[ch].common.nRP = host->setup.mem.inputMemTime.nRP;
      }

      if (host->setup.mem.inputMemTime.nRAS != 0) {
        (*channelNvList)[ch].common.nRAS = host->setup.mem.inputMemTime.nRAS;
      }

      if (host->setup.mem.inputMemTime.nRCD != 0) {
        (*channelNvList)[ch].common.nRCD = host->setup.mem.inputMemTime.nRCD;
      }

      if (host->setup.mem.inputMemTime.nRC != 0) {
        (*channelNvList)[ch].common.nRC = host->setup.mem.inputMemTime.nRC;
      }

      if (host->setup.mem.inputMemTime.nRTP != 0) {
        (*channelNvList)[ch].common.nRTP = host->setup.mem.inputMemTime.nRTP;
      }

      if (host->setup.mem.inputMemTime.nRFC != 0) {
        (*channelNvList)[ch].common.nRFC = host->setup.mem.inputMemTime.nRFC;
      }

      if (host->setup.mem.inputMemTime.nWR != 0) {
        (*channelNvList)[ch].common.nWR = host->setup.mem.inputMemTime.nWR;
      }

      if (host->setup.mem.inputMemTime.nCL != 0) {
        (*channelNvList)[ch].common.nCL = host->setup.mem.inputMemTime.nCL;
      }

      if (host->setup.mem.inputMemTime.nFAW != 0) {
        (*channelNvList)[ch].common.nFAW = host->setup.mem.inputMemTime.nFAW;
      }

      if (host->setup.mem.inputMemTime.nCWL != 0) {
        (*channelNvList)[ch].common.nWL = host->setup.mem.inputMemTime.nCWL;
      }
    } // MEM_OVERRIDE_EN
#endif // XMP_SUPPORT

#ifdef  LRDIMM_SUPPORT
    // LRDIMM debug
    if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {

#ifdef  IDT_LRBUF_WA
      // LRDIMM Get the Did/Vid of iMB for WA
      dimmNvList  = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        // Can't read the DVid directly from the buffer since it is still in Reset
        // So cheat by checking the SPD. We have already established that this is an LRDIMM
        if ((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT)
          (*dimmNvList)[dimm].lrbufDVid = LRBUF_IDT_DVID;
        if ((*dimmNvList)[dimm].SPDRegRev == 0x20) {
          (*dimmNvList)[dimm].lrbufRid = LRBUF_IDT_RID_B0;
        } else {
          (*dimmNvList)[dimm].lrbufRid = 0; // Unknown Revision
        }
      }
#endif // IDT_LRBUF_WA

      //
      // Determine common Module Delay limits and boundary check
      // LRDIMM Debug - Always use lrdimmExtraOdtDelay for now
      if (((*channelNvList)[ch].common.nMDL == 0) || ((*channelNvList)[ch].common.nMDL > 7) ||
          ((*channelNvList)[ch].common.nMDD > 3) || (host->setup.mem.lrdimmModuleDelay==MODULE_DELAY_DIS)) {
        // if nMDL = 0 (or huge) use alternate hard coded table
        // Note: need to remove this code once SPD's get programmed correctly
        (*channelNvList)[ch].common.nMDL = lrdimmExtraOdtDelay[host->nvram.mem.socket[socket].ddrFreq];
        (*channelNvList)[ch].common.nMDD = 2;
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "nMDL= %d - nMDD= %d\n",
                       (*channelNvList)[ch].common.nMDL,
                       (*channelNvList)[ch].common.nMDD));
      }
      if ((*channelNvList)[ch].common.nMDD == 0) {
        (*channelNvList)[ch].common.nMDD = 1;  // May need optmization
      }
    }
#endif // LRDIMM_SUPPORT

#ifdef   SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX)) {
      getPrintFControl(host);
      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Mem Timings:\n"));
      if (((*channelNvList)[ch].lrRankMultEnabled) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tCCD=%d\n", 4 + LR_RANK_MULT_TA));
      } else {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tCCD=%d\n", 4));
      }
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tCCD_L=%d\n", GetTccd(host, socket)));
      }
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tCWL=%d\n", (*channelNvList)[ch].common.nWL));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tCL=%d\n", (*channelNvList)[ch].common.nCL));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRP=%d\n", (*channelNvList)[ch].common.nRP));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRCD=%d\n", (*channelNvList)[ch].common.nRCD));
      if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tRRD=%d\n", (*channelNvList)[ch].common.nRRD));
      } else {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tRRD_S=%d\n", (*channelNvList)[ch].common.nRRD));
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "tRRD_L=%d\n", (*channelNvList)[ch].common.nRRD_L));
      }
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tWTR=%d\n", (*channelNvList)[ch].common.nWTR));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRAS=%d\n", (*channelNvList)[ch].common.nRAS));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRTP=%d\n", (*channelNvList)[ch].common.nRTP));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tWR=%d\n", (*channelNvList)[ch].common.nWR));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tFAW=%d\n", (*channelNvList)[ch].common.nFAW));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRC=%d\n", (*channelNvList)[ch].common.nRC));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "tRFC=%d\n", (*channelNvList)[ch].common.nRFC));
      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "casSup=0x%x\n", (*channelNvList)[ch].common.casSup));
      releasePrintFControl(host);
    }
#endif


    //
    // Enable 2:2 crossover mode if we are running at 2133 or higher
    //
    if ((host->nvram.mem.socket[socket].ddrFreq >= DDR_2133) && !(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
      host->var.mem.xoverModeVar = XOVER_MODE_2TO2;
    } else {
      host->var.mem.xoverModeVar = XOVER_MODE_1TO1;
    }

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "xoverModeVar = %d\n", host->var.mem.xoverModeVar));

    //
    // Program memory timings for this ch
    //
    ProgramTimings (host, socket, ch);

    //
    // Set ODT Matrix
    //
    SetOdtMatrix (host, socket, ch);

    //
    // Set ODT timing parameters
    //
    SetOdtTiming (host, socket, ch);

    //
    // Enable Scrambling
    //
    scrambleConfig.Data = 0;
    if ((host->setup.mem.options & SCRAMBLE_EN) && (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG)))) {
      //
      // Write the scrambling seed
      //
      MemWritePciCfgEp (host, socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG, host->nvram.mem.scrambleSeed);
    } // if scrambling

    //
    // Write the scrambling config to lock the seed value and disable scrambling until after training
    //
    scrambleConfig.Bits.seed_lock = 1;
    MemWritePciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

    //
    //Enable Page Table Aliasing
    //
    pagetbl.Data = MemReadPciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG);
    if ((*channelNvList)[ch].lrDimmPresent && (((*channelNvList)[ch].maxDimm < 3) && ((*channelNvList)[ch].encodedCSMode != 2))) {
      pagetbl.Bits.en_pgta2 = 1;
    } else {
      pagetbl.Bits.en_pgta2 = 0;
    }


    //
    // In encoded mode, set subrank timing mode settings
    //
    dimmNvList = GetDimmNvList(host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      if ((*channelNvList)[ch].encodedCSMode == 2) {
        //
        //Enable 3DS support
        //
        pagetbl.Bits.en_3ds = 1;

        switch ((*dimmNvList)[dimm].dieCount) {
          case 2:
            rm2xx = 1;
            break;

          case 4:
            rm2xx = 2;
            break;

          case 8:
            rm2xx = 3;
            break;

          default:
            rm2xx = 0;
            break;
        }

        //
        // Disable subrank tracking for 3DPC or // Only apply to TSV RDIMM case , Disable subrank tracking for TSV RDIMM
        //
        if (((*channelNvList)[ch].maxDimm > 2) || (!(*channelNvList)[ch].lrDimmPresent)) {
          rm2xx = 0;
        }

        switch (dimm) {
          case 0:
            pagetbl.Bits.rm2x0 = rm2xx;
            break;

          case 1:
            pagetbl.Bits.rm2x1 = rm2xx;
            break;

          case 2:
            pagetbl.Bits.rm2x2 = rm2xx;
            break;
        }
      } else {
        switch (dimm) {
          case 0:
            pagetbl.Bits.rm2x0 = (*dimmNvList)[dimm].lrRankMult >> 1;
            break;

          case 1:
            pagetbl.Bits.rm2x1 = (*dimmNvList)[dimm].lrRankMult >> 1;
            break;

          case 2:
            pagetbl.Bits.rm2x2 = (*dimmNvList)[dimm].lrRankMult >> 1;
            break;
        }
      }

      //
      // initialize F0BC1x variable
      //
      (*dimmNvList)[dimm].lrBuf_BC1x = 0;
    }
    MemWritePciCfgEp (host, socket, ch, TCLRDP_MCDDC_CTL_REG, pagetbl.Data);

    //
    // Reset counters
    //
    pmonUnitCtrl.Data = 0;
    pmonUnitCtrl.Bits.resetcounterconfigs = 1;
    pmonUnitCtrl.Bits.resetcounters = 1;

    MemWritePciCfgEp (host, socket, ch, PMONUNITCTRL_MCDDC_CTL_REG, pmonUnitCtrl.Data);

    if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
      //
      //Setup CAS Perfmon for syncing Adaptive Paging Counters in Lockstep
      //
      if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
        //
        // Enable CAS counters
        //
        pmonCntrCfg.Data = 0;
        pmonCntrCfg.Bits.eventselect = 0x4;
        pmonCntrCfg.Bits.unitmask = 0xf;
        pmonCntrCfg.Bits.counterenable = 1;
        MemWritePciCfgEp (host, socket, ch, PMONCNTRCFG_1_MCDDC_CTL_REG, pmonCntrCfg.Data);
      } // lockstep + adaptive
    } // less than C0
  } // ch loop

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    //
    // HSD 4166563 Set vmse_error.ignore_vmse_err = 1 (A0 only)
    //
    for (ch = 0; ch < MAX_CH; ch++) {
      vmseError.Data = MemReadPciCfgEp (host, socket, (ch & 0x02), VMSE_ERROR_MCDDC_DP_REG);
      vmseError.Bits.ignore_vmse_err = 1;
      MemWritePciCfgEp (host, socket, (ch & 0x02), VMSE_ERROR_MCDDC_DP_REG, vmseError.Data);
    }

    // vmse_err bit on Ch 2/3 for 2HA part
    if (Is2HA(host)) {
      vmseError.Data = MemReadPciCfgEp (host, socket, 2, VMSE_ERROR_MCDDC_DP_REG);
      vmseError.Bits.ignore_vmse_err = 1;
      RegOffset.Data = VMSE_ERROR_MCDDC_DP_REG;
      bus = host->var.common.busUncore[socket];
      dev = 21;
      func = 2;
      regaddr = (UINT8 *) (host->var.common.mmCfgBase | PCIE_REG_ADDR(bus, dev, func, RegOffset.Bits.offset));
      *(volatile UINT32 *) regaddr = vmseError.Data;
      dev = 24;
      regaddr = (UINT8 *) (host->var.common.mmCfgBase | PCIE_REG_ADDR(bus, dev, func, RegOffset.Bits.offset));
      *(volatile UINT32 *) regaddr = vmseError.Data;
    }
  }

  return SUCCESS;

} // EarlyConfig


/**

  Report VDD Error

  @param host  - Pointer to sysHost
  @param socket  - Socket Id

  @retval N/A

**/
void
ReportVddError (
    PSYSHOST host,
    UINT8    socket
    )
{
    MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Cant program VDD!\n"));
    OutputError (host, ERR_SET_VDD, ERR_UNKNOWN_VR_MODE, socket, 0xFF, 0xFF, 0xFF);
}

/**

  Program Vddq

  @param host  - Pointer to sysHost
  @param socket  - Socket Id
  @param SvidEncodingValue - VRM12 defined VR Ramp Encoding Value

  @retval N/A

**/
void
SetVdd (
  PSYSHOST host,
  UINT8    socket
  )
{
  UINT8                                     ch;
  UINT8                                     userVdd;
  UINT8                                     status;
  UINT32                                    PcodeMailboxStatus = 0;
  UINT32                                    ActiveVrMask = 0;
  UINT32                                    VrMode = 0;
  UINT32                                    VRData;
  UINT32                                    svidEncoding = 0x00;
  UINT8                                     VrCount;
  UINT8                                     Count;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT          biosScratchPad1;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT    mcInitStateG;
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT  miscCKECtl;
  UINT32 VrAddress[] = {
    MAILBOX_BIOS_DRAM0_VR_ADDR,
    MAILBOX_BIOS_DRAM1_VR_ADDR,
    MAILBOX_BIOS_DRAM2_VR_ADDR,
    MAILBOX_BIOS_DRAM3_VR_ADDR
  };
  VddValues        *vddptr;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  OutputExtendedCheckpoint((host, STS_CLOCK_INIT, SUB_SET_CLK_VDD, socket));
  //
  // Only assert reset for cold boot cases
  //
  if (((host->var.common.bootMode == NormalBoot) &&
      ((host->var.mem.subBootMode == ColdBoot) || (host->var.mem.subBootMode == ColdBootFast))) || (host->var.mem.subBootMode == AdrResume)){
    //
    // Assert reset
    //
    mcInitStateG.Data             = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
    // Clear ddr_reset to assert reset
    mcInitStateG.Bits.ddr_reset   = 0;
    mcInitStateG.Bits.reset_io    = 0;
    MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  }

  // Loop for each channel
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    miscCKECtl.Bits.cke_override = (*channelNvList)[ch].ckeMask;
    miscCKECtl.Bits.cke_on = 0;
    MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
  } // ch loop

  //
  // Read the Active DRAM VR's from the Mailbox Data Register
  //
  PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_ACTIVE_VR_MASK, 0);
  if (PcodeMailboxStatus == 0) ActiveVrMask = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);
  else ReportVddError(host, socket);


  // Set Command
  VRData = (MAILBOX_BIOS_DATA_VR_REG_PROTOCOL_ID << 16) | (MAILBOX_BIOS_VR_CMD_GET_REG << 4);

  //
  // Make sure the requested Vdd makes sense
  //
  userVdd = host->setup.mem.inputMemTime.vdd;
  if (userVdd != 0) {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
      if((userVdd < 100) || (userVdd > 185)) {
        userVdd = 0;
      }
    } else {
      if((userVdd < 90) || (userVdd > 185)) {
        userVdd = 0;
      }
    }
  }

  for (Count = 0 ; Count < sizeof(VrAddress)/sizeof(UINT32) ; Count++) {

#ifdef SERIAL_DBG_MSG
    getPrintFControl(host);
#endif // SERIAL_DBG_MSG

    VRData &= ~0xF;
    VRData |= VrAddress[Count];
    //
    // If VR is active, program requested voltage
    //
    if (ActiveVrMask & (BIT0 << Count)) {
      //
      // Use MAILBOX_BIOS_CMD_VR_INTERFACE commandto read protocol ID register 0x05 for VR Mode
      //
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, ((UINT32)MAILBOX_BIOS_CMD_VR_INTERFACE), VRData);
      if (PcodeMailboxStatus == 0) VrMode = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);
      else ReportVddError(host, socket);

      //
      // Setup Encoding value based on VrMode and Selected Voltage
      //
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "VR%d DDR Voltage: ", Count));

      if (userVdd != 0) {
        //
        // Use the user provided Vdd if it is not 0
        //
        if (VrMode == VR_MODE_12) {
          svidEncoding = (((userVdd * 10) - 250) / 5) + 1;
        } else {
          svidEncoding = (((userVdd * 10) - 500) / 10) + 1;
        }
        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "%d.%d\n", userVdd / 100, userVdd % 100));
      } else {
        //
        // Use the standard Vdd based on DIMM type
        //
        vddptr = &VddValuesTable[0];
        for (VrCount = 0 ; VrCount < VddValuesTableSize ; VrCount++) {
          vddptr = &VddValuesTable[VrCount];
          if (vddptr->ddrVoltageValue == host->nvram.mem.socket[socket].ddrVoltage) break;
        }

        MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, vddptr->string_ptr));

        if (vddptr->ddrVoltageValue == 0xFF) ReportVddError(host, socket);
        else if (VrMode == VR_MODE_12)   svidEncoding = vddptr->Vr12Encoding;
        else if (VrMode == VR_MODE_12_5) svidEncoding = vddptr->Vr125Encoding;
      }

      //
      // Set appropriate Vdd
      //
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_VDD_RAMP | (VrAddress[Count] << 8)), svidEncoding);
      if (PcodeMailboxStatus != 0) {
        ActiveVrMask &= ~(BIT0 << Count);
        MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "VR%d SVID Command Failed\n", Count));
      }
    }

#ifdef SERIAL_DBG_MSG
    releasePrintFControl(host);
#endif // SERIAL_DBG_MSG
  } // count loop

  if ((ActiveVrMask & (BIT3 | BIT2 | BIT1 | BIT0)) == 0) {
    //
    // No SVID detected, call a hook to see if a platform specific Set VDD function is available
    //
    status = CoreSetVdd(host, socket);
    if (status == FAILURE) {
      //
      // If failure, change the host structure to reflect it's running at default
      //
      if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
        host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_120;
      } else {
        host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_150;
      }
    }
  }

  //
  // set base voltage into bios scratchpad1 register bits 1-0 (zero out rest of bits for now)
  //  0 = Not set yet (read before MRC initializes the value)
  //  1 = 1.5V
  //  2 = 1.35V
  //  3 = Reserved for 1.2x setting
  //
  biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
  biosScratchPad1.Data = biosScratchPad1.Data & ~(BIT1+BIT0);

  if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_120) {
    biosScratchPad1.Data = biosScratchPad1.Data | 3;
  } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_135) {
    biosScratchPad1.Data = biosScratchPad1.Data | 2;
  } else if (host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_150) {
    biosScratchPad1.Data = biosScratchPad1.Data | 1;
  } else {
    biosScratchPad1.Data = biosScratchPad1.Data | 0;
  }

  WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);

  //
  // Initialization complete
  //
} // SetVdd


#define LT_ESTS   0xFED30008
#define LT_E2STS  0xFED308F0
/**

  Checks for secrets in memory

  @param host  - Pointer to sysHost

  @retval N/A

**/
void
CheckSecrets (
  PSYSHOST host
  )
{
  CAPID0_PCU_FUN3_BDX_STRUCT capId0;

  capId0.Data = host->var.common.procCom[0].capid0;
  if (capId0.Bits.lt_sx_en) {
    if(((*(UINT32 *) LT_ESTS) & BIT6)) {
      host->var.mem.wipeMemory = 1;
      host->var.mem.skipMemoryInit = 0;
    } else if(((*(UINT32 *) LT_E2STS) & BIT1)) {
      host->var.mem.wipeMemory = 1;
      host->var.mem.skipMemoryInit = 0;
    } else {
      host->var.mem.wipeMemory = 0;
    }
  }
} // CheckSecrets


/**

  Reset the DDR channels for the given socket number (or per MC for BSSA)

  host      - Pointer to sysHost
  socket    - Socket Id
  chBitMask - Bitmask of channels to reset

  @retval N/A

**/
void
ResetAllDdrChannels (
  PSYSHOST host,
  UINT8    socket,
  UINT32   chBitMask
  )
{
  UINT8   ch;
  UINT8   mcId;
  UINT8   mcStart = 0xF;
  UINT8   mcEnd = 0;
  UINT8   nv_ckeMask;
  UINT8   dimm;
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
#ifdef  IDT_LRBUF_WA
  UINT8   ckMask[MAX_CH];
#endif
  struct channelNvram (*channelNvList)[MAX_CH];
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT mcInitStateG;
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT  miscCKECtl;

  //
  // Bypass for the S3 resume path
  //
  if ((host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast) &&
      (host->var.mem.subBootMode != AdrResume)) {

    return;
  }

  MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "\nReset All Channels\n"));

#ifdef DEBUG_PERFORMANCE_STATS
#ifdef SERIAL_DBG_MSG
  countTrackingData(host, JEDEC_COUNT, 1);
#endif
#endif

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  //Here, from the Channel Bit Mask, we calculate the number of MC's present //For non-BSSA case, this will always be 2 for SKX
  //Possibilities in HSX: Ch 0,1,2,3 - MC0    OR    Ch 0,1 - MC0 and Ch 2,3 - MC1
  //Possibilities in SKX: Ch 0,1,2 - MC0    OR    Ch 3,4,5 - MC1
  for (ch = 0; ch < MAX_CH; ch++) {
   //mcId = 0 or 1; if 0, then mcStart = 0 ... GetMCID - Returns the memory controller ID
    mcId = GetMCID(host, socket, ch); //Get MC channel belongs to
    if(mcStart > mcId) {  //always set to 0
      mcStart = mcId; // 0 or 1
    }
    if (mcEnd < mcId) {  //0 OR will be set to 1 --> if we have 2 MC's (HSX)
      mcEnd = mcId;
    }
  } //ch loop

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if ((chBitMask & (1 << ch)) == 0) continue;

    if ((*channelNvList)[ch].encodedCSMode) {
      dimmNvList = GetDimmNvList(host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        dimmMtr.Data = (*dimmNvList)[dimm].dimmMemTech;
        // Force encoded CS mode to disabled in the iMC because the register is in normal mode after reset
        dimmMtr.Bits.ddr4_3dsnumranks_cs = 0;
        MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4), (UINT32) dimmMtr.Data);
      } //dimm
    } // encodedCSMode
  } //ch

  // De-assert CKE
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if ((chBitMask & (1 << ch)) == 0) continue;
    if ((host->setup.mem.socket[socket].ddrCh[ch].batterybacked) && (host->var.mem.subBootMode == AdrResume)) continue;

    miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    miscCKECtl.Bits.cke_override = (*channelNvList)[ch].ckeMask;
    miscCKECtl.Bits.cke_on = 0;
    MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
  } // ch loop

#ifdef  LRDIMM_SUPPORT

  // If this is not the first call, use the LR soft reset feature
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) &&
      host->nvram.mem.socket[socket].lrDimmPresent &&
      host->nvram.mem.socket[socket].lrInitDone) {

    // Loop for each channel
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      if ((chBitMask & (1 << ch)) == 0) continue;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*channelNvList)[ch].dimmList[dimm].dimmPresent == 0) continue;
        // Do soft reset to MB, with QRST_n to the DRAMs
        // F2-RC1: Reset Control
        WriteRcLrbuf(host, socket, ch, dimm, 0, BIT0, 2, 1);

      }
    }
    // Reset should be held for Stable power + PLL lock time
    FixedDelay (host, 10);

  } else
#endif
  {
    //
    // Assert reset
    //
    mcInitStateG.Data             = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);

    //Setting the reset bits for each MC
    for (mcId = mcStart; mcId <= mcEnd; mcId++) {
    //
    // DDR_RESET & RESET_IO is controlled from HA0
    //
    // Clear ddr_reset to assert reset
    mcInitStateG.Bits.ddr_reset   = 0;
    mcInitStateG.Bits.reset_io    = 0;
    MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    } // mcId loop

    //
    // Reset should be held for JEDEC mandated 200us, but a 2 usec
    // fixed delay in MRC should be plenty for stable power DDR reset flow.
    //
    if (host->var.mem.firstJEDECDone) {
      FixedDelay (host, 2);
    } else {
      FixedDelay (host, 200);
      host->var.mem.firstJEDECDone = 1;
    }
  }

#ifdef  LRDIMM_SUPPORT
#ifdef  IDT_LRBUF_WA
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) && host->nvram.mem.socket[socket].lrDimmPresent) {

    // Clear out all rank masks first
    for (ch = 0; ch < MAX_CH; ch++) ckMask[ch] = 0;

    // Loop for each channel
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      if ((*channelNvList)[ch].lrDimmPresent == 0) continue;
      if ((chBitMask & (1 << ch)) == 0) continue;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*channelNvList)[ch].dimmList[dimm].dimmPresent == 0) continue;

        dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

        if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
            ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_B0)) {
          // CK pair 0/2 -> DIMM/Slot 0
          // CK pair 1/3 -> DIMM/Slot 1
          // CK      2   -> DIMM/Slot 2 (only for 3 DPC/SPC systems)
          ckMask[ch] |= 0x05 << dimm;
        }
      } // dimm
    }  // ch

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      if ((*channelNvList)[ch].lrDimmPresent == 0) continue;
      if ((chBitMask & (1 << ch)) == 0) continue;
      if (ckMask[ch]) {
        // Sighting 200002
        SetRankUsed(host,socket,ch,~ckMask[ch]);
        FixedDelay(host,1);
      }
    } // ch
  } // lrDimmPresent
#endif // IDT_LRBUF_WA
#endif // LRDIMM_SUPPORT

  //
  // De-assert reset
  //
  //
  // DDR_RESET & RESET_IO is controlled from HA0
  //
  //Setting the bits for each MC
  for (mcId = mcStart; mcId <= mcEnd; mcId++) {
    mcInitStateG.Bits.ddr_reset = 1;
    MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  } // mcId loop

#ifdef  LRDIMM_SUPPORT
#ifdef  IDT_LRBUF_WA
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if ((*channelNvList)[ch].lrDimmPresent == 0) continue;
    if ((chBitMask & (1 << ch)) == 0) continue;
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4) continue;
    if (ckMask[ch]) {
      // Sighting 200002
      FixedDelay(host,1);
      SetRankUsed(host,socket,ch,0xFF);
    } // chMask
  } // ch
#endif // IDT_LRBUF_WA
#endif // LRDIMM_SUPPORT

  //
  // Reset deassertion to CKE assertion (JEDEC mandated 500us)
  //
  //Skip for DDR4 RDIMMs: HSD s4987693
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR3) || (host->nvram.mem.dimmTypePresent != RDIMM)) {

    FixedDelay (host, 500);  //500uS delay from reset going high to when CKE can go high
    // Loop for each channel
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      if ((chBitMask & (1 << ch)) == 0) continue;
      if ((host->setup.mem.socket[socket].ddrCh[ch].batterybacked) && (host->var.mem.subBootMode == AdrResume)) continue;
      nv_ckeMask =  (*channelNvList)[ch].ckeMask;
      // Assert CKE
      miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
      miscCKECtl.Bits.cke_override = (*channelNvList)[ch].ckeMask;
      miscCKECtl.Bits.cke_on = nv_ckeMask;
      MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
    } // ch loop

    //
    // wait the 20 nano sec tCKSRX
    //
    FixedDelay (host, 1);

  } else {

    //
    // wait 5 usec for tStab
    //
    FixedDelay (host, 5);
  }

} // ResetAllDdrChannels

/*
  Handles the S3 exit flow. After the DIMM configuration is
  restored, BIOS can perform the following steps to bring the
  DRAM out of self-refresh and enabling the refresh operations:
  1. Enable DCLK at DDR interface and wait for tSTAB (~6usec)
     for clock to be stable.
  2. Set up  commands:
     A. NOP with tXS wait
     B. Refresh with tRFC wait
     C. ZQCL with tZQoper wait
  3. Set rank disable at DIMMMTR_x.RANK_DISABLE for any rank
     that is populated but need to be disabled from all
     channels.
  4. Set CKE via MC_INIT_STATE_Cx.CKE_ON[5:0] for all
     occupied channels (up to four CSR writes).
  5. Wait tRFC+10ns (but subtract the amount of time took for
     the last CSR write from step #4) via subsequence control
  7. Poll for done
  8. Enable automatic refresh via setting REFRESH_ENABLE in
     MC_INIT_STATE_G

  Due to the timing constraint of the above step 4-8 which may affect the DRAM refresh
  operation without violating 9xTREFI panic refresh requirement, these steps (4-8) must
  be pre-loaded into the LLC during the Non-Evicted Memory Mode (NEM); otherwise, the
  fetch delay from the Flash device may take a long time and violate the refresh timing.
*/
void
ExitSelfRefresh (
  PSYSHOST host,
  UINT8 socket
  )
{
  UINT32                              chBitMask;
  UINT8                               ch;
  UINT8                               mcId;
  struct channelNvram               (*channelNvList)[MAX_CH];
#ifdef MEM_NVDIMM_EN
  struct   dimmNvram                (*dimmNvList)[MAX_DIMM];
  UINT8                              dimm;
  struct   ddrRank                  (*rankList)[MAX_RANK_DIMM];
  UINT8                              rank;
  struct   rankDevice               (*rankStruct)[MAX_RANK_DIMM];
  UINT16                            MR6;
#endif  //MEM_NVDIMM_EN
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT     mcInitStateG;
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT   miscCKECtl;

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Exit Self Refresh\n"));

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  //
  // Setup engine with commands needed for SR exit.
  // 1. NOP with tXS wait
  // 2. ZQCL with tZQoper wait
  // 3. Refresh with tRFC wait
  //

  chBitMask = GetChBitmaskAll (host, socket);

#ifdef SERIAL_DBG_MSG
#endif

  //
  // Disable output messages between SR exit and automatic refresh enable
  //
  disableAllMsg(host);

  //
  // Bring the IO out of reset on S3 and warm fast boot path before enabling refresh
  // and switching to normal mode.
  //
  IO_Reset(host, socket);

  //
  // Assert CKE on each channel
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
    miscCKECtl.Bits.cke_override = (*channelNvList)[ch].ckeMask;
    miscCKECtl.Bits.cke_on = (*channelNvList)[ch].ckeMask;
    MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
#ifdef MEM_NVDIMM_EN
    if (host->var.mem.subBootMode == NvDimmResume) {  // Initialize MRx rgisters for NVDIMMs

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
        if ( (*dimmNvList)[dimm].dimmPresent == 0 ) continue;

        if ((*dimmNvList)[dimm].nvDimmType) {
          rankList    = &host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList;
          rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
          for (rank = 0; rank < host->var.mem.socket[socket].maxRankDimm; rank++) {
            if ((*rankList)[rank].enabled == 0) continue;
            if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
              DoRegisterInit (host, socket, ch, dimm, rank);
              WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
              WriteMRS (host, socket, ch, dimm, rank,                       0, BANK3);   // MR3 = 0
              WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
              WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);
            } else if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
                DoRegisterInitDDR4 (host, socket, ch, dimm, rank);
                MR6 = GetVrefRange(host, (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex]);
              #ifdef LRDIMM_SUPPORT
                if (host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

                  MR6 = (UINT16)(*rankList)[rank].lrbufTxVref[0];
                }
              #endif
                MR6 |= ((GetTccd (host, socket) - 4) & 7) << 10;
                //
                // Follow MRS programming order in the DDR4 SDRAM spec
                //
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR3, BANK3);
                //
                // The first write ensures VrefDQ Training Mode is disabled
                //
                WriteMRS (host, socket, ch, dimm, rank, MR6, BANK6);
                //
                // The second write enters VrefDQ Training Mode and sets the range
                //
                WriteMRS (host, socket, ch, dimm, rank, MR6 | BIT7, BANK6);
                //
                // The third write sets the VrefDQ value
                //
                WriteMRS (host, socket, ch, dimm, rank, MR6 | BIT7, BANK6);
                //
                // The fourth write exits VrefDq Training Mode
                //
                WriteMRS (host, socket, ch, dimm, rank, MR6, BANK6);
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4, BANK4);
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
                WriteMRS (host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);
            }
#ifdef SERIAL_DBG_MSG
            if (checkMsgLevel(host, SDBG_MAX))  rcPrintf ((host, "ExitSR: MR0 0x%02x  MR1 0x%02x MR2 0x%02x\n", (*rankStruct)[rank].MR0, (*rankStruct)[rank].MR1, (*rankStruct)[rank].MR2));
#endif
          }// rank loop
        }
      } // dimm loop
    } // if NvDimmResume
#endif // MEM_NVDIMM_EN
  }

  // Enable Refresh
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    mcInitStateG.Data = MemReadPciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG);
    mcInitStateG.Bits.refresh_enable  = 1;
    MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  }

  //
  // Restore output mseeages after auto refresh is enabled
  //
  restoreMsg(host);

#ifdef SERIAL_DBG_MSG
#endif

}

UINT8
ExitSelfRefreshEndMarker ()
/*
  This function needs to immediately follow the
  ExitSelfRefresh() funtion.  This provides an end label that C
  can use to read the entire function (ExitSelfRefresh) into
  cache before executing it.
*/
{
  UINT8 status;
  status = 0;
  return status;
}

/**

  Initialize channel specific memory controller registers after training and before normal mode

  @param host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
LateConfig (
  PSYSHOST host
  )
{
  UINT8                                           socket;
  UINT8                                           ch;
  UINT8                                           dimm;
  UINT8                                           strobe;
  UINT8                                           mcId;
  UINT8                                           busSeg;
  UINT32                                          smbCntlReg;
  UINT32                                          lrbufData = 0;
  UINT32                                          evenChCASCount;
  UINT32                                          diffChCASCount;
  UINT8                                           numCacheLines;
  UINT8                                           targetCh;
  UINT8                                           PointerSeperationHA;
  struct  channelNvram                            (*channelNvList)[MAX_CH];
  struct  dimmNvram                               (*dimmNvList)[MAX_DIMM];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT                dataControl0;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT                scrambleConfig;
  MCCRDTTHRLD_HA_CFG_STRUCT                       McCrdtThrld;
  CPGC_INORDER_MCDDC_CTL_STRUCT                   cpgcInOrder;
  IDLETIME_MCDDC_CTL_STRUCT                       IdleTime;
  PMONCNTR_1_N0_MCDDC_CTL_STRUCT                  pmonCntrLo;
  SMBCNTL_0_MC_MAIN_HSX_BDX_STRUCT                smbCntl;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT           haSYSDEFEATURE2;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT                    mcMtr;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT          mcInitStateG;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT       chknBit1;
  TCDBP_MCDDC_CTL_HSX_BDX_STRUCT                  tcdbp;
  HASYSDEFEATURE_HA_CFG_STRUCT                    haSYSDEFEATURE;
  AMAP_MCDDC_DESC_STRUCT                          altAddMap;
  CAPID3_PCU_FUN3_BDX_STRUCT                      capId3;
  ROWHAMMER_TIMER_MCDDC_CTL_STRUCT                rowHammerTimer;
  ROWHAMCONFIG_MCDDC_CTL_STRUCT                   rowHamConfig;
  ROWHAMCONFIG2_MCDDC_CTL_STRUCT                  rowHamConfig2;
  ROWHAMCONFIG3_MCDDC_CTL_STRUCT                  rowHamConfig3;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRCmdNCmdControls;
  DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRCmdSCmdControls;
  DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCkeCtlControls;
  DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCtlCtlControls;
  UINT8											  applyRowHammerWA = 0;
  TCRFP_MCDDC_CTL_HSX_STRUCT                      tcrfp;
  UINT8                                           temp = 0;
  struct smbDevice                                spd;

  evenChCASCount = 0; // to avoid compiler warning
  socket = host->var.mem.currentSocket;
  PointerSeperationHA = host->var.qpi.OutPointerSeperationHA;

  OutputExtendedCheckpoint((host, STS_CHANNEL_TRAINING, STS_RMT, (UINT16)((SUB_LATE_CONFIG << 8)|socket)));
  //
  // Return if this socket is disabled
  //
  if (host->nvram.mem.socket[socket].enabled == 0) return SUCCESS;
  if (host->nvram.mem.socket[socket].maxDimmPop == 0) {
    //
    // Disable channels on any socket that exists but doesnt have any DIMMS populated
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if (Is1HA(host) && (mcId > 0)) continue;

      mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);

      for (ch = 0; ch < MAX_CH; ch++) {
        if (mcId != GetMCID(host, socket, ch)) continue;
        //
        // For 1 MC: MC0.mcMtr.Bits.chn_disable [19:16] = channel 3-0
        // For 2 MC: MC0.mcMtr.Bits.chn_disable [17:16] = channel 1-0 & MC1.mcMtr.Bits.chn_disable [17:16] = channel 3-2
        //
        if (mcId == 1) {
          mcMtr.Bits.chn_disable |= (BIT0 << (ch & BIT0));
        } else {
          mcMtr.Bits.chn_disable |= (BIT0 << ch);
        }

      } // ch loop

      //
      // HSD 4986683: One channel must remain enabled on each MC
      //
      if (Is2HA(host)) {
        //
        // 2 HA processor
        //
        if ((mcMtr.Bits.chn_disable & 3) == 3) {
          mcMtr.Bits.chn_disable &= ~BIT0; // Keep CH0 enabled
        }
      } else {
        //
        // 1 HA processor
        //
        if (mcMtr.Bits.chn_disable == 0xF) {
          capId3.Data = host->var.common.procCom[socket].capid3;
          if (capId3.Bits.chn_disable & BIT0) {
            mcMtr.Bits.chn_disable = 0xD;
          } else {
            mcMtr.Bits.chn_disable = 0xE;
          }
        }
      }
      MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
    } // mcId loop
    return SUCCESS;
  }

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  //
  // Configure DRAM Maintenance
  //
  if (!(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
    DramMaintenance(host, socket);
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      // Set SPD devices to page 0 as default for DDR4.
      // DDR3 always set to page0 since it has only one page.
      dimmNvList = GetDimmNvList(host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        spd.compId = SPD;
        spd.address.controller = PROCESSOR;
        spd.address.deviceType = DTI_EEPROM;
        GetSmbAddress(host, socket, ch, dimm, &spd);
        // Update current page
        host->nvram.mem.socket[socket].imc[spd.address.busSegment].spdPageAddr = 0;
        // Set page
        spd.address.deviceType = DTI_WP_EEPROM;
        spd.address.strapAddress = SPD_EE_PAGE_SELECT_0;
        // Write to SPA
        WriteSmb (host, socket, spd, 0, &temp); 
      }
    }
  }//DDR4

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    if((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (host->nvram.mem.dimmTypePresent == RDIMM)) {
      applyRowHammerWA = 0;

      //4987948 WA
      if ((host->var.common.cpuType == CPU_HSX) ||
      (IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX)) ||
       (( IsBDXEP(host) && CheckSteppingLessThan( host, CPU_BDX, B0_REV_BDX)) && !IsMccChop(host)) ) { // WA reversed for BDX ML A0 and DE V0
        if (((*channelNvList)[ch].maxDimm > 2) || (((*channelNvList)[ch].lrDimmPresent) && ((*channelNvList)[ch].maxDimm > 1))) {
          applyRowHammerWA = 1;
        }
      }

      if (applyRowHammerWA) {
        rowHammerTimer.Data = 0x00000001;
        MemWritePciCfgEp (host, socket, ch, ROWHAMMER_TIMER_MCDDC_CTL_REG, rowHammerTimer.Data);

        rowHamConfig2.Data = 0x50800000;
        MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG2_MCDDC_CTL_REG, rowHamConfig2.Data);

        rowHamConfig3.Data = 0x00000FFF;
        MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG3_MCDDC_CTL_REG, rowHamConfig3.Data);

        rowHamConfig.Data = 0x803F8032;
        MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG_MCDDC_CTL_REG, rowHamConfig.Data);
      }

      //5003013 and 5003112: BTTO with thermal throttle content on 3DPC 3DS LRDIMM/RDIMM
      if (IsBDXEP(host)) {
        if (((*channelNvList)[ch].maxDimm > 2) && ((((*channelNvList)[ch].encodedCSMode & SPD_3DS_TYPE) == SPD_3DS_TYPE) || ((*channelNvList)[ch].lrDimmPresent))) {
          tcrfp.Data              = MemReadPciCfgEp (host, socket, ch, TCRFP_MCDDC_CTL_REG);
          tcrfp.Bits.ref_hi_wm    = tcrfp.Bits.ref_panic_wm + 1;
          MemWritePciCfgEp (host, socket, ch, TCRFP_MCDDC_CTL_REG, tcrfp.Data);
        }
      }
    }

    tcdbp.Data= MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
    tcdbp.Bits.cmd_oe_on = 0;
    tcdbp.Bits.cmd_oe_cs = 1;
    MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);

    if ((host->var.common.bootMode == NormalBoot) || (host->var.mem.subBootMode == ColdBoot)) {
      if (!(CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX))) {
        // This is a workaround. BIOS HSD 4166169. Do not clear rfon for HSX A0.
        dataControl0.Data = (*channelNvList)[ch].dataControl0;
        dataControl0.Bits.rfon = 0;
        (*channelNvList)[ch].dataControl0 = dataControl0.Data;

        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
        } // strobe loop
      }

    } else if (host->var.mem.subBootMode == ColdBootFast && host->setup.mem.options & RMT_COLD_FAST_BOOT){

      dataControl0.Bits.rfon = 0;

      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
      } // strobe loop

    }

    cpgcInOrder.Data = MemReadPciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG);
    cpgcInOrder.Bits.wpq_inorder_en = 0;
    MemWritePciCfgEp (host, socket, ch, CPGC_INORDER_MCDDC_CTL_REG, cpgcInOrder.Data);

    if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
      if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
      //if (((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) && (host->setup.mem.options & ADAPTIVE_PAGE_EN)) {
        IdleTime.Data   = MemReadPciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG);
        pmonCntrLo.Data = MemReadPciCfgEp (host, socket, ch, PMONCNTR_1_N0_MCDDC_CTL_REG);
        //For Robust solution, also use pmonCntrHi
        //pmonCntrHi.Data = MemReadPciCfgEp (host, socket, ch, PMONCNTR_1_N1_MCDDC_CTL_REG);
        if ((ch & BIT0) == 0) {
         evenChCASCount = pmonCntrLo.Data;
        } else {
          if (pmonCntrLo.Data > evenChCASCount) {
            //Issue diffChCASCount # of cpgc reads to (ch-1) aka the even channel
            targetCh = ch-1;
            diffChCASCount = pmonCntrLo.Data - evenChCASCount;
          } else {
            //Issue diffChCASCount # of cpgc reads to the odd (aka current) channel.
            targetCh = ch;
            diffChCASCount = evenChCASCount - pmonCntrLo.Data;
          }

          if (diffChCASCount == 0) {
          } else {
            while (diffChCASCount != 0) {
              if (diffChCASCount > 127) {
                numCacheLines = 127;
              } else {
                numCacheLines = (UINT8)diffChCASCount;
              }
              CpgcWrite (host, socket, (1<<targetCh), numCacheLines);
              diffChCASCount = diffChCASCount - numCacheLines;
            } // while
          } // WA
        } // odd ch
      } // lockstep
    } // less than C0
  } // ch loop

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    //
    // Enable adaptive page policy  (We set it to disabled in programTimings)
    //
    if (host->setup.mem.options & ADAPTIVE_PAGE_EN) {
      IdleTime.Data = MemReadPciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG);

      if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
        pmonCntrLo.Data = MemReadPciCfgEp (host, socket, ch, PMONCNTR_1_N0_MCDDC_CTL_REG);
        //For Robust solution, also use pmonCntrHi
        //pmonCntrHi.Data = MemReadPciCfgEp (host, socket, ch, PMONCNTR_1_N1_MCDDC_CTL_REG);
        if ((ch & BIT0) == 0) {
          evenChCASCount = pmonCntrLo.Data;
        } else {
          if (pmonCntrLo.Data > evenChCASCount) {
            diffChCASCount = pmonCntrLo.Data - evenChCASCount;
          } else {
            diffChCASCount = evenChCASCount - pmonCntrLo.Data;
          }
        } // odd ch
      } // lockstep

      IdleTime.Bits.adapt_pg_clse = 1;
      MemWritePciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG, IdleTime.Data);
    }


    //
    // Enable Scrambling
    //
    if ((host->setup.mem.options & SCRAMBLE_EN) && (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG)))) {
      scrambleConfig.Data = 0;
      scrambleConfig.Bits.rx_enable = 1;
      scrambleConfig.Bits.tx_enable = 1;
      scrambleConfig.Bits.ch_enable = 1;
      //
      // HSD 4166541: Implement HSX/BDX Processor Security Locks as per SeCoE Recommendation
      //
      scrambleConfig.Bits.seed_lock = 1;
      //
      // Write the scrambling config
      //
      MemWritePciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);
    } // if scrambling
  } // ch loop


  if ((host->var.mem.xoverModeVar == XOVER_MODE_2TO2) &&
      (( IsDESku(host) && CheckSteppingGreaterThan( host, CPU_BDX, B0_REV_BDX)) || ( IsBDXEP(host)))) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      ddrCRCmdNCmdControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdNCmdControls.Bits.sr_gate_2to2 = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdNCmdControls.Data);

      ddrCRCmdSCmdControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdSCmdControls.Bits.sr_gate_2to2 = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdSCmdControls.Data);

      ddrCRCkeCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCkeCtlControls.Bits.sr_gate_2to2 = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCkeCtlControls.Data);

      ddrCRCtlCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlCtlControls.Bits.sr_gate_2to2 = 1;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlCtlControls.Data);
    } // ch loop
  }

  //
  // HSD 4166546: Implement DIMM SPD Configuration and Write-Protection as per SeCoE recommendation
  //
  for (busSeg = 0; busSeg < 2; busSeg++) {
    if ((busSeg == 0) || Is2HA(host)) {
      smbCntlReg  = SMBCNTL_0_MC_MAIN_REG;
    } else {
      smbCntlReg  = SMBCNTL_1_MC_MAIN_REG;
    }

    if (Is2HA(host)){
      mcId = busSeg;
    } else {
      mcId = 0;
    }
    smbCntl.Data = MemReadPciCfgMC (host, socket, mcId, smbCntlReg);
    smbCntl.Bits.smb_dis_wrt = 1;
    MemWritePciCfgMC (host, socket, mcId, smbCntlReg, smbCntl.Data);
  }
  //
  // Configure Misc HA Features
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {

    haSYSDEFEATURE2.Data = MemReadPciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG);

    //
    // MC Channel Hash
    //
    haSYSDEFEATURE2.Bits.mcchanhashen = 1;

    MemWritePciCfgMC (host, socket, mcId, HASYSDEFEATURE2_HA_CFG_REG, haSYSDEFEATURE2.Data);

    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    McCrdtThrld.Data = MemReadPciCfgMC (host, socket, mcId, MCCRDTTHRLD_HA_CFG_REG);
    if (host->setup.mem.mcBgfThreshold == 0) {
      McCrdtThrld.Bits.mcbypassbgfthreshold = 4 - PointerSeperationHA;
    } else {
      McCrdtThrld.Bits.mcbypassbgfthreshold = host->setup.mem.mcBgfThreshold;
    }
    {
      McCrdtThrld.Bits.cntlbgfthreshold = 4 - PointerSeperationHA;
      McCrdtThrld.Bits.updtbgfthreshold = 4 - PointerSeperationHA;
      McCrdtThrld.Bits.wrbgfthresh      = 8 - PointerSeperationHA;
    }
    if (host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG)) {
      McCrdtThrld.Bits.mcbypassbgfthreshold = 1;
      McCrdtThrld.Bits.cntlbgfthreshold = 1;
      McCrdtThrld.Bits.updtbgfthreshold = 1;
      McCrdtThrld.Bits.wrbgfthresh      = 5;
    }
    MemWritePciCfgMC (host, socket, mcId, MCCRDTTHRLD_HA_CFG_REG, McCrdtThrld.Data);

    //  The number of retries is determined by values of the fields RetryForever, RetryThreshold: 0,1 = 4 retries after read
    haSYSDEFEATURE.Data = MemReadPciCfgMC(host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG);
    haSYSDEFEATURE.Bits.retryforever = 0;
    haSYSDEFEATURE.Bits.retrythreshold = 1;
    MemWritePciCfgMC(host, socket, mcId, HASYSDEFEATURE_HA_CFG_REG, haSYSDEFEATURE.Data);


  } // mcId loop

  //
  // Configure C/A Parity
  //
  // cid Bits are to be programmed prior to enabling CAP. This applies to 3DS and 
  // the bug is valid for HSX and BDX. But we are fixing it only for BDX.
  if ((!(host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast)) || (IsBDXEP(host))) {
    if ((host->setup.mem.options & CA_PARITY_EN) && (host->nvram.mem.dimmTypePresent == RDIMM)) {
      EnableParityChecking(host, socket);
    } else {
      DisableParityChecking(host, socket);
    }
  }

  if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
    if (host->setup.mem.optionsExt & CAP_ERR_FLOW) {
      EnableErrorFlow ( host, socket);
    }
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    dimmNvList = GetDimmNvList(host, socket, ch);

    if ((host->nvram.mem.dimmTypePresent == RDIMM) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        //
        // Disable register SMBUS per BIOS sighting #5001600
        //
        WriteRC (host, socket, ch, dimm, 0, 0x1, RDIMM_RC2x);
      } //dimm
    } //DDR4 RDIMM/LRDIMM

#ifdef  LRDIMM_SUPPORT
   if ((*channelNvList)[ch].lrDimmPresent) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        if (host->nvram.mem.dramType == SPD_TYPE_DDR3) {
          //
          // Set F0-RC7 to 0 at the end of MRC for PPD-S
          // Note must be done after any LRDIMM WorkArounds
          //
          //                               Rank Data Funk Addr
          WriteRcLrbuf(host, socket, ch, dimm, 0,   0,   0,   7);
        }  else {
          WriteBC(host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
          (*dimmNvList)[dimm].rcLrFunc = 0;
        }
      } //dimm
    } // lrDimmPresent
#endif // LRDIMM_SUPPORT
  } // ch loop
  // Disable any further RCW's
  host->nvram.mem.socket[socket].wa = host->nvram.mem.socket[socket].wa | WA_NO_MORE_RCW;

  //
  // Configure lockstep
  //
  SetLockstepMode (host, socket);

  if (ALTERNATE_ADDRESS_MAP_DEFAULT == ALTERNATE_ADDRESS_MAP_EN)
  {
    for (ch = 0; ch < MAX_CH; ch++){
      if ((*channelNvList)[ch].enabled == 0) continue;

      altAddMap.Data = MemReadPciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG);
      if (host->var.common.emulation & (SOFT_SDV_FLAG | SIMICS_FLAG))
      {
        altAddMap.Bits.hsxplus = 0;
      } else {
        altAddMap.Bits.hsxplus = 1;
      }
      MemWritePciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG, altAddMap.Data);
    }
  }

  //
  // Disable Unused Channels
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if (Is1HA(host) && (mcId > 0)) continue;

    mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled)  continue;

      if (mcId != GetMCID(host, socket, ch)) continue;

      //
      // For 1 MC: MC0.mcMtr.Bits.chn_disable [19:16] = channel 3-0
      // For 2 MC: MC0.mcMtr.Bits.chn_disable [17:16] = channel 1-0 & MC1.mcMtr.Bits.chn_disable [17:16] = channel 3-2
      //
      if (mcId == 1) {
        mcMtr.Bits.chn_disable |= (BIT0 << (ch & BIT0));
      } else {
        mcMtr.Bits.chn_disable |= (BIT0 << ch);
      }

    } // ch loop

    //
    // HSD 4986683: One channel must remain enabled on each MC
    //
    if (Is2HA(host)) {
      //
      // 2 HA processor
      //
      if ((mcMtr.Bits.chn_disable & 3) == 3) {
        mcMtr.Bits.chn_disable &= ~BIT0; // Keep CH0 enabled
      }
    } else {
      //
      // 1 HA processor
      //
      if (mcMtr.Bits.chn_disable == 0xF) {
        capId3.Data = host->var.common.procCom[socket].capid3;
        if (capId3.Bits.chn_disable & BIT0) {
          mcMtr.Bits.chn_disable = 0xD;
        } else {
          mcMtr.Bits.chn_disable = 0xE;
        }
      }
    }
    MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } // mcId loop

#ifdef MEM_NVDIMM_EN
   if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast || host->var.mem.subBootMode == ColdBootFast || host->var.mem.subBootMode == NvDimmResume)
#else
  if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast || host->var.mem.subBootMode == ColdBootFast)
#endif
  {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      MemWritePciCfgEp (host, socket, ch, TCMR0SHADOW_MCDDC_CTL_REG, (*channelNvList)[ch].TCMr0Shadow);
      MemWritePciCfgEp (host, socket, ch, TCMR2SHADOW_MCDDC_CTL_REG, (*channelNvList)[ch].TCMr2Shadow);
      MemWritePciCfgEp (host, socket, ch, TCMR5SHADOW_MCDDC_CTL_REG, (*channelNvList)[ch].TCMr5Shadow);
    } // ch loop
  }

  if ((host->var.common.bootMode == NormalBoot && host->var.mem.subBootMode == ColdBootFast) || (host->var.mem.subBootMode == AdrResume)) {
    IO_Reset(host, socket);
  }

#ifdef DE_SKU
  InitADR(host);
#endif

#if !defined(DE_SKU) || defined(MEM_SV_HOOKS)
    if ((host->nvram.mem.WrCRC) && (host->nvram.mem.dramType == SPD_TYPE_DDR4) && (!(host->nvram.mem.RASmode & CH_LOCKSTEP))) {
      EnableWrCRC(host, socket);
    }
#endif

  //
  // If we're coming out of S3, enable CKE_ON to bring DIMMs out
  // of self-refresh before enabling refresh and turning on normal
  // mode.
  //
#ifdef MEM_NVDIMM_EN
  if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast || host->var.mem.subBootMode == NvDimmResume)
#else
  if (host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast)
#endif
  {
    //
    // Exit self refresh
    //
    ExitSR(host, socket);

    if ((host->nvram.mem.dimmTypePresent == RDIMM) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) { //RDIMM and DDR4
      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        dimmNvList = GetDimmNvList(host, socket, ch);
        for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
          //
          // Skip if no DIMM present
          //
          if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

          if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) &&
              ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_B0)) {

            ReadLrbufSmb (host, socket, ch, dimm, 0x1010, &lrbufData);
            if ((((lrbufData >> 9) & 0x1) != 1) || (((lrbufData >> 12) & 0x1) != 0)) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,"PLL lock error\n"));
              FatalError (host, 0, 0);
            }
          } // IDT check
        } // dimm loop
      } // ch loop
    } // RDIMM/DDR4 check

    if ((host->setup.mem.options & CA_PARITY_EN) && (host->nvram.mem.dimmTypePresent == RDIMM) &&
      !((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR3))) {
      EnableParityChecking(host, socket);
    } else {
      DisableParityChecking(host, socket);
    }
  } // if (S3Resume)
  else
  {
    //
    // Enable Refresh
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      mcInitStateG.Data = MemReadPciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.refresh_enable  = 1;
      MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    }
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) && (host->var.qpi.OutIsocEn == TRUE) && (host->nvram.mem.RASmode == CH_MIRROR)) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      chknBit1.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
      chknBit1.Bits.pri_starvation_wim = 1;
      MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit1.Data);
    }
  }

  return SUCCESS;
} // LateConfig

#ifdef SSA_FLAG
UINT32
SSAInit (
  PSYSHOST host
  )
/*++

  Initialize the SSA API

  @param host  - Pointer to sysHost

  @retval SUCCESS

--*/
{
#ifdef BDAT_SUPPORT
  UINT8     ResultIdx;

  //Initializing the BSSA BDAT variables - BiosSaveToBdat()
  host->var.mem.bssaBdatSize      = 0;

  for (ResultIdx = 0; ResultIdx < MAX_NUMBER_SSA_BIOS_RESULTS; ResultIdx++) {
    host->var.mem.bssaNumberHobs[ResultIdx] = 0;
  }

  host->var.mem.bssaNumberDistinctGuids = 0;
#endif //BDAT_SUPPORT

  // Enabling the BSSA module and loader is dependent on:
  // 1. OemDetectPhysicalPresenceSSA() returning TRUE. OEMs should add their own jumper detect function.
  // 2. Loader setup knob being enabled.
  if (OemDetectPhysicalPresenceSSA(host) == TRUE) {
    if (host->setup.mem.enableBiosSsaLoader) {
      SsaBiosInitialize (host);                        //Calling InstallPpi, Initializing the API and SsaBiosLoadStatus()

#ifdef SSA_LOADER_FLAG
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Calling the BIOS SSA API\n"));
      OutputCheckpoint (host, SSA_API_INIT, SSA_API_INIT_EV_LOADER, 0);  // Issue a SSA check point to invoke the EV loader
#endif //SSA_LOADER_FLAG
    } //enableBiosSsaLoader
  }

  return SUCCESS;
} //SSAInit
#endif  //SSA_FLAG

/**

  Enables CA Parity / Write CRC Error flow

  @param host    - Pointer to sysHost
  @param socket  - Processor socket
  @param enable  - CRC/CA Parity flow select (1 - WrCRC error flow, 0 - CA parity flow)

  @retval N/A

**/
void
EnableErrorFlow (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8                               ch;
  ERF_DDR4_CMD_REG0_MCDDC_CTL_STRUCT  erfDdr4CmdReg0;
  ERF_DDR4_CMD_REG1_MCDDC_CTL_STRUCT  erfDdr4CmdReg1;
  ERF_DDR4_CMD_REG2_MCDDC_CTL_STRUCT  erfDdr4CmdReg2;
  ERF_DDR4_CMD_REG3_MCDDC_CTL_STRUCT  erfDdr4CmdReg3;
  ERF_DDR4_CMD_REG4_MCDDC_CTL_STRUCT  erfDdr4CmdReg4;
  ERF_DDR4_CMD_REG5_MCDDC_CTL_STRUCT  erfDdr4CmdReg5;
  ERF_DDR4_CMD_REG6_MCDDC_CTL_STRUCT  erfDdr4CmdReg6;
  ERF_DDR4_CMD_REG7_MCDDC_CTL_STRUCT  erfDdr4CmdReg7;

    //erfDdr4CmdReg0
    erfDdr4CmdReg0.Data       = 0x0;
    erfDdr4CmdReg0.Bits.ad    = 0x04;
    erfDdr4CmdReg0.Bits.bank  = 0x3;
    erfDdr4CmdReg0.Bits.rcw   = 0x0;
    erfDdr4CmdReg0.Bits.en    = 0x1;
    erfDdr4CmdReg0.Bits.rdimm = 0;

    //erfDdr4CmdReg1
    erfDdr4CmdReg1.Data       = 0x0;
    erfDdr4CmdReg1.Bits.ad    = 0x64;
    erfDdr4CmdReg1.Bits.bank  = 0x7;
    erfDdr4CmdReg1.Bits.rcw   = 0x0;
    erfDdr4CmdReg1.Bits.en    = 0x1;
    erfDdr4CmdReg1.Bits.rdimm = 1;

    //erfDdr4CmdReg2
    erfDdr4CmdReg2.Data       = 0x0;
    erfDdr4CmdReg2.Bits.ad    = 0x400;
    erfDdr4CmdReg2.Bits.bank  = 0x0;
    erfDdr4CmdReg2.Bits.rcw   = 0x5;
    erfDdr4CmdReg2.Bits.en    = 0x1;
    erfDdr4CmdReg2.Bits.rdimm = 1;

    //erfDdr4CmdReg3
    erfDdr4CmdReg3.Data       = 0x0;
    erfDdr4CmdReg3.Bits.ad    = 0x5;
    erfDdr4CmdReg3.Bits.bank  = 0x3;
    erfDdr4CmdReg3.Bits.rcw   = 0x0;
    erfDdr4CmdReg3.Bits.en    = 1;
    erfDdr4CmdReg3.Bits.rdimm = 0;

    //erfDdr4CmdReg4
    erfDdr4CmdReg4.Data       = 0x0;
    erfDdr4CmdReg4.Bits.ad    = 0x400;
    erfDdr4CmdReg4.Bits.bank  = 0x7;
    erfDdr4CmdReg4.Bits.rcw   = 0x5;
    erfDdr4CmdReg4.Bits.en    = 1;
    erfDdr4CmdReg4.Bits.rdimm = 0;

    //erfDdr4CmdReg5
    erfDdr4CmdReg5.Data       = 0x0;
    erfDdr4CmdReg5.Bits.ad    = 0x40;
    erfDdr4CmdReg5.Bits.bank  = 0x5;
    erfDdr4CmdReg5.Bits.rcw   = 0x0;
    erfDdr4CmdReg5.Bits.en    = 0;
    erfDdr4CmdReg5.Bits.rdimm = 0;

    //erfDdr4CmdReg6
    erfDdr4CmdReg6.Data       = 0x0;
    erfDdr4CmdReg6.Bits.ad    = 0x0;
    erfDdr4CmdReg6.Bits.bank  = 0x3;
    erfDdr4CmdReg6.Bits.rcw   = 0x0;
    erfDdr4CmdReg6.Bits.en    = 0x1;
    erfDdr4CmdReg6.Bits.rdimm = 0;

    //erfDdr4CmdReg7
    erfDdr4CmdReg7.Data       = 0x0;
    erfDdr4CmdReg7.Bits.ad    = 0x66;
    erfDdr4CmdReg7.Bits.bank  = 0x7;
    erfDdr4CmdReg7.Bits.rcw   = 0x0;
    erfDdr4CmdReg7.Bits.en    = 0x1;
    erfDdr4CmdReg7.Bits.rdimm = 1;

    for (ch = 0; ch < MAX_CH; ch++){
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG0_MCDDC_CTL_REG, erfDdr4CmdReg0.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG1_MCDDC_CTL_REG, erfDdr4CmdReg1.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG2_MCDDC_CTL_REG, erfDdr4CmdReg2.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG3_MCDDC_CTL_REG, erfDdr4CmdReg3.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG4_MCDDC_CTL_REG, erfDdr4CmdReg4.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG5_MCDDC_CTL_REG, erfDdr4CmdReg5.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG6_MCDDC_CTL_REG, erfDdr4CmdReg6.Data);
      MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG7_MCDDC_CTL_REG, erfDdr4CmdReg7.Data);
  } // ch loop

}
/**

  Enables CMD/ADDR Parity

  @param host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
void
EnableCAParityRuntime (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8                               ch;
  struct  channelNvram                (*channelNvList)[MAX_CH];
  DDR4_CA_CTL_MCDDC_DP_STRUCT         ddr4cactl;
  ERF_DDR4_CMD_REG2_MCDDC_CTL_STRUCT  erfDdr4CmdReg2;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT     dp_chkn;
  VMSE_ERROR_MCDDC_DP_HSX_BDX_STRUCT  vmseError;
#ifdef  LRDIMM_SUPPORT
  DDR4_CBIT_PARITY_MCDDC_CTL_STRUCT   ddr4cbitParity;
#endif

  channelNvList = GetChannelNvList(host, socket);

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "Enabling C/A Parity\n"));


  for (ch = 0; ch < MAX_CH; ch += 2){

    ddr4cactl.Data = MemReadPciCfgEp(host, socket, ch, DDR4_CA_CTL_MCDDC_DP_REG);

    //
    // Program DDR4 tPar Recovery
    //
    ddr4cactl.Bits.tpar_recov_ch1 = tPARRECOVERY[host->nvram.mem.ratioIndex];
    ddr4cactl.Bits.tpar_recov_ch0 = tPARRECOVERY[host->nvram.mem.ratioIndex];

    //
    // Program RDIMM/LRDIMM Present on channel
    //
    if (host->nvram.mem.dimmTypePresent == RDIMM) {
      ddr4cactl.Bits.erf_regpart0 = 1;
      ddr4cactl.Bits.erf_regpart1 = 1;
    }

    MemWritePciCfgEp(host, socket, ch, DDR4_CA_CTL_MCDDC_DP_REG, ddr4cactl.Data);
  } //ch loop

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX) == 0) {
    for (ch = 0; ch < MAX_CH; ch += 2){
      vmseError.Data = MemReadPciCfgEp (host, socket, ch, VMSE_ERROR_MCDDC_DP_REG);
      vmseError.Bits.ignore_vmse_err = 0;
      MemWritePciCfgEp (host, socket, ch, VMSE_ERROR_MCDDC_DP_REG, vmseError.Data);
    } // ch loop
  }

  for (ch = 0; ch < MAX_CH; ch++){
    //
    // Enable parity error logging in the MC
    //
    if (ch & BIT0) {
      dp_chkn.Data= MemReadPciCfgEp (host, socket, (ch & BIT1), MC1_DP_CHKN_BIT_MCDDC_DP_REG);
      dp_chkn.Bits.dis_rdimm_par_chk = 0;
      MemWritePciCfgEp (host, socket, (ch & BIT1), MC1_DP_CHKN_BIT_MCDDC_DP_REG, dp_chkn.Data);
    } else {
      dp_chkn.Data= MemReadPciCfgEp (host, socket, (ch & BIT1), MC0_DP_CHKN_BIT_MCDDC_DP_REG);
      dp_chkn.Bits.dis_rdimm_par_chk = 0;
      MemWritePciCfgEp (host, socket, (ch & BIT1), MC0_DP_CHKN_BIT_MCDDC_DP_REG, dp_chkn.Data);
    }
  } // ch loop

  for (ch = 0; ch < MAX_CH; ch++){
    //
    // Enable parity error logging in the MC
    //
    if (ch & BIT0) {
      dp_chkn.Data= MemReadPciCfgEp (host, socket, (ch & BIT1), MC1_DP_CHKN_BIT_MCDDC_DP_REG);
      if (host->nvram.mem.socket[socket].cmdClkTrainingDone && !(host->nvram.mem.RASmode == CH_LOCKSTEP)) {
        dp_chkn.Bits.en_rdimm_par_err_log = 1;
      } else {
        dp_chkn.Bits.en_rdimm_par_err_log = 0;
      }
      MemWritePciCfgEp (host, socket, (ch & BIT1), MC1_DP_CHKN_BIT_MCDDC_DP_REG, dp_chkn.Data);
    } else {
      dp_chkn.Data= MemReadPciCfgEp (host, socket, (ch & BIT1), MC0_DP_CHKN_BIT_MCDDC_DP_REG);
      if (host->nvram.mem.socket[socket].cmdClkTrainingDone && !(host->nvram.mem.RASmode == CH_LOCKSTEP)) {
        dp_chkn.Bits.en_rdimm_par_err_log = 1;
      } else {
        dp_chkn.Bits.en_rdimm_par_err_log = 0;
      }
      MemWritePciCfgEp (host, socket, (ch & BIT1), MC0_DP_CHKN_BIT_MCDDC_DP_REG, dp_chkn.Data);
    }
  } // ch loop

  for (ch = 0; ch < MAX_CH; ch++){
    //
    // Writes RC6 to 6 to clear Parity Error
    //
    erfDdr4CmdReg2.Data       = 0x0;
    erfDdr4CmdReg2.Bits.ad    = 0x66;
    erfDdr4CmdReg2.Bits.bank  = 0x7;
    erfDdr4CmdReg2.Bits.en    = 0x1;
    if (host->nvram.mem.dimmTypePresent == RDIMM) {
      erfDdr4CmdReg2.Bits.rdimm = 1;
    } else {
      erfDdr4CmdReg2.Bits.rdimm = 0;
    }
    MemWritePciCfgEp(host, socket, ch, ERF_DDR4_CMD_REG2_MCDDC_CTL_REG, erfDdr4CmdReg2.Data);
  } // ch loop

  for (ch = 0; ch < MAX_CH; ch++) {
    //program Per bit inclusion control for ddr4 Cbits in command/address parity generation.
    ddr4cbitParity.Data = MemReadPciCfgEp(host, socket, ch, DDR4_CBIT_PARITY_MCDDC_CTL_REG);
    ddr4cbitParity.Bits.enable = 0;
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      ddr4cbitParity.Bits.enable = (*channelNvList)[ch].cidBitMap;
    }
    MemWritePciCfgEp(host, socket, ch, DDR4_CBIT_PARITY_MCDDC_CTL_REG, ddr4cbitParity.Data);
  } // ch loop

  // Enable error flow for steppings > A0
  if (!CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    if (host->nvram.mem.dramType == SPD_TYPE_DDR4){
      for (ch = 0; ch < MAX_CH; ch += 2){
        ddr4cactl.Data = MemReadPciCfgEp(host, socket, ch & BIT1, DDR4_CA_CTL_MCDDC_DP_REG);
        ddr4cactl.Bits.erf_en0 = 1;
        ddr4cactl.Bits.erf_en1 = 1;
        MemWritePciCfgEp(host, socket, ch & BIT1, DDR4_CA_CTL_MCDDC_DP_REG, ddr4cactl.Data);
      } // ch loop
    } // if DDR4
  }

} // EnableCAParity

/**

  Exits self refresh

  @param host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
void
ExitSR (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8                     ch;
  UINT32                    data;
  VOID                      (*functionPtr)(PSYSHOST host, UINT8 socket);
  UINT8                     (*endFunctionPtr)();
  UINT32                    **dataPtr;
  UINT32                    **endDataPtr;
  UINT32                    tczqcalOrg[MAX_CH];
  struct  channelNvram      (*channelNvList)[MAX_CH];
  TCZQCAL_MCDDC_CTL_STRUCT  tczqcal;

  channelNvList = GetChannelNvList(host, socket);
  //
  // First read the entire function into cache so that there are
  // no flash reads during the S3 exit flow.  A flash read takes
  // too long and will violate timing parameters.
  //
  functionPtr = ExitSelfRefresh;
  endFunctionPtr = ExitSelfRefreshEndMarker;
  dataPtr = (UINT32 **)(&functionPtr);
  endDataPtr = (UINT32 **)(&endFunctionPtr);
  //
  // Check to make sure the compiler didn't reorganize the end
  // function marker to someplace before the critical function.
  // If it did, then set the endFunctionPtr to a place 1000 hex
  // bytes after the start of the function.  This is more than
  // enough bytes to read the entire function into processor
  // cache.
  if (*endDataPtr <= *dataPtr) {
    *endDataPtr = *dataPtr + 0x1000;
  }

  while (*dataPtr < *endDataPtr) {
    data = **dataPtr;
    (*dataPtr)++;
  }

  if(host->var.mem.subBootMode == AdrResume)
  {
    // Call the platform hook to release the CKE/DDR Reset Clamps
    CoreReleaseADRClamps(host, socket);
  }

  //
  // Make the ZQCAL period short to make sure we get plenty of ZQCS's before accessing memory
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    tczqcalOrg[ch] = 0;
    if ((*channelNvList)[ch].enabled == 0) continue;

    tczqcal.Data            = MemReadPciCfgEp (host, socket, ch, TCZQCAL_MCDDC_CTL_REG);
    tczqcalOrg[ch]          = tczqcal.Data;
    tczqcal.Bits.zqcsperiod = 2;
    MemWritePciCfgEp (host, socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcal.Data);
  } // ch loop

  //
  // At this point, the entire function - ExitSelfRefresh - has
  // been read into processor cache.  Now we can execute it.
  //
  ExitSelfRefresh(host, socket);

  //
  // Restore ZQCAL period
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    MemWritePciCfgEp (host, socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcalOrg[ch]);
  } // ch loop
} // ExitSR

#ifdef  LRDIMM_SUPPORT
#ifdef  SERIAL_DBG_MSG
void DumpLrbuf (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
/*++

  Dump LRBUF

  @param host   - Pointer to sysHost
  @param socket - Socket Id
  @param ch     - Current channel
  @param dimm   - DIMM on ch

  @retval None

--*/
{
  UINT32 lrbufData;
  UINT16 offset;

  if (checkMsgLevel(host, SDBG_MAX)) {

    getPrintFControl(host);
    // debug... dump all MB config space via SMBus reads
    rcPrintf ((host, "\nN%d.C%d.D%d - LRDIMM config dump:\n", socket, ch, dimm));
    rcPrintf ((host, "Byte offset: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F"));

    for (offset = 0; offset < 0xFF; offset = offset + 4) {

      if (SUCCESS == ReadLrbufSmb (host, socket, ch, dimm, offset, &lrbufData)) {
         if (offset % 16 == 0) {
            rcPrintf ((host, "\n       0x%03x:", offset));
         }
         rcPrintf ((host, " 0x%02x 0x%02x 0x%02x 0x%02x",
                 lrbufData & 0xFF, (lrbufData >> 8) & 0xFF, (lrbufData >> 16) & 0xFF, (lrbufData >> 24) & 0xFF));
      } else {
         rcPrintf ((host, "Failure: LRDIMM SMBus read at offset 0x%03x\n", offset));
      }
    }
    rcPrintf ((host, "\n\n"));

    releasePrintFControl(host);
  }
}
#endif  // SERIAL_DBG_MSG
#endif  //LRDIMM_SUPPORT


                                         // addr, cid, bank, pdatag
PatCadbProg0 ZqWAPattern0[CADB_LINES] = {{0x0,   0x1, 0x0, 0x0}, // Row 0
                                          {0x0,   0x1, 0x0, 0x0}, // Row 1
                                          {0x0,   0x1, 0x0, 0x0}, // Row 2
                                          {0x0,   0x1, 0x0, 0x0}, // Row 3
                                          {0x0,   0x1, 0x0, 0x0}, // Row 4
                                          {0x0,   0x1, 0x0, 0x0}, // Row 5
                                          {0x0,   0x0, 0x0, 0x0}, // Row 6
                                          {0x0,   0x0, 0x0, 0x0}, // Row 7
                                          {0x0,   0x0, 0x0, 0x0}, // Row 8
                                          {0x0,   0x0, 0x0, 0x0}, // Row 9
                                          {0x0,   0x0, 0x0, 0x0}, // Row 10
                                          {0x0,   0x0, 0x0, 0x0}, // Row 11
                                          {0x0,   0x0, 0x0, 0x0}, // Row 12
                                          {0x0,   0x0, 0x0, 0x0}, // Row 13
                                          {0x0,   0x0, 0x0, 0x0}, // Row 14
                                          {0x0,   0x0, 0x0, 0x0}, // Row 15
                                         };

                                         // CS  , Control, ODT,  CKE, Par
PatCadbProg1 ZqWAPattern1[CADB_LINES] = {{0x3FE,     0xE, 0x0, 0x3F, 1}, // Row 0
                                          {0x3FD,     0xE, 0x0, 0x3F, 1}, // Row 1
                                          {0x3EF,     0xE, 0x0, 0x3F, 1}, // Row 2
                                          {0x3DF,     0xE, 0x0, 0x3F, 1}, // Row 3
                                          {0x2FF,     0xE, 0x0, 0x3F, 1}, // Row 4
                                          {0x1FF,     0xE, 0x0, 0x3F, 1}, // Row 5
                                          {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 6
                                          {0x3FF,     0xF, 0x0, 0x3F, 0}, // Row 7
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 8
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 9
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 10
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 11
                                          {0x3FE,     0xF, 0x0, 0x3F, 0}, // Row 12
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 13
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 14
                                          {0x3FF,     0xF, 0x0, 0x3F, 1}, // Row 15
                                         };
/**

  Switches to normal mode

  @param host    - Pointer to sysHost
  @param socket  - Socket to switch

**/
UINT32
SwitchToNormalMode (
  PSYSHOST  host
  )
{
  UINT8                                   socket = host->var.mem.currentSocket;
  UINT8                                   dimm;
  UINT8                                   ch;
  UINT8                                   mcId;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  struct channelNvram                     (*channelNvList)[MAX_CH];
  RPQAGE_MCDDC_CTL_STRUCT                 rpqAge;
  BIOS_RESET_CPL_PCU_FUN1_STRUCT          biosResetCpl;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT   chknBit;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT            mcMtr;
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT  mcInitStateG;
  CPGC_MISCCKECTL_MCDDC_CTL_HSX_BDX_STRUCT miscCKECtl;
  UINT32                                  PcodeMailboxStatus = 0;
  UINT32                                  PCUData = 0;
  UINT32                                  tempData;
  UINT8                                   strobe;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT        dataControl0;
  DATACONTROL2_0_MCIO_DDRIO_BDX_STRUCT    dataControl2;
  DATACONTROL3_0_MCIO_DDRIO_STRUCT        dataControl3;
  DATACONTROL4_0_MCIO_DDRIO_BDX_STRUCT    dataControl4;
  DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_HSX_STRUCT     ddrCRCtlControls;
  DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_HSX_STRUCT    ddrCRCmdControlsCmdS;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_HSX_STRUCT    ddrCRCmdControlsCmdN;
  TSOD_CONTROL_PCU_FUN1_STRUCT            pcuTsodCtrlCfg;
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT    dimmTempEvOfst;

#ifdef  LRDIMM_SUPPORT
  CMD_B273548_WA_CHANNELS                 ChWaMask;
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT          seqCtl0;
  VMSE_LINK_WIDTH_MCDDC_DP_STRUCT         vmseLinkWidth;
#endif //LRDIMM_SUPPORT

  // Continue if this socket is not present
  if (host->nvram.mem.socket[socket].enabled == 0) return SUCCESS;


  //
  // Let the pCode know which imc's are enabled
  //
  if(!IsDESku(host)) {
    tempData = 0;
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if (host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
        tempData |= (1 << mcId);
      } // end if
    } // mcId loop
    WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_IO_SA_CONFIG, tempData);
  }

  pcuTsodCtrlCfg.Data = ReadCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG);
  if ((host->setup.mem.thermalThrottlingOptions & CLTT_EN) && (host->nvram.mem.socket[socket].DimmWithoutThermalSensorFound == 0)) {
    pcuTsodCtrlCfg.Bits.tsod_polling_interval = 8;  //1 second - tuned post-Si
  } else {
    pcuTsodCtrlCfg.Bits.tsod_polling_interval = 0;
  }
  WriteCpuPciCfgEx (host, socket, 0, TSOD_CONTROL_PCU_FUN1_REG, pcuTsodCtrlCfg.Data);

  //
  // Execute only if this socket has memory
  //
  if (host->nvram.mem.socket[socket].maxDimmPop) {

    channelNvList = &host->nvram.mem.socket[socket].channelList;

    //HSD s4988158 WA: Start
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        dimmTempEvOfst.Data = MemReadPciCfgEp (host, socket, ch, (DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG + (dimm * 4)));

        if (((*channelNvList)[ch].features & DOUBLE_REFI) || (((*channelNvList)[ch].twoXRefreshSetPerCh >= 20) && ((*channelNvList)[ch].twoXRefreshSetPerCh <= 40)) || (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
          dimmTempEvOfst.Bits.ev_2x_ref_templo_en = 1;
        } else {
          dimmTempEvOfst.Bits.ev_2x_ref_templo_en = 0;
        }
        MemWritePciCfgEp (host, socket, ch, (DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG + (dimm * 4)), dimmTempEvOfst.Data);
      }//dimm
    }//ch-loop
    //HSD s4988158 WA: End

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      dataControl0.Data = (*channelNvList)[ch].dataControl0;
      if (!CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
        dataControl0.Bits.internalclockson = 0;
      }

      //
      // BIOS sighting: 4988090 - Disable RX CLK gating for BDX to
      // WA post-training margin degradation issue
      //
      if (host->var.common.cpuType == CPU_BDX) { //BDX
        if (IsDESku(host) ||
           (IsBDXEP(host) && CheckSteppingLessThan(host, CPU_BDX, B0_REV_BDX) && IsHccChop(host))) { //DE or EP A0 HCC SKU,
          dataControl0.Bits.rxpion = 1;
        } else { // BDX EP B0 and above
          dataControl0.Bits.rxpion = 0;
        }
      } else { // HSX
        dataControl0.Bits.rxpion = 0;
      }
      dataControl0.Bits.txpion = 0;

      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
        dataControl2.Data = (*channelNvList)[ch].dataControl2[strobe];
        dataControl2.Bits.forcebiason = 1;
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL2_0_MCIO_DDRIO_REG), dataControl2.Data);
      } // strobe loop
      (*channelNvList)[ch].dataControl0 = dataControl0.Data;

      ddrCRCmdControlsCmdN.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControlsCmdN.Bits.xovercal = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdN.Data);

      ddrCRCmdControlsCmdS.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdControlsCmdS.Bits.xovercal = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControlsCmdS.Data);

      ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCtlControls.Bits.xovercal = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

      ddrCRCtlControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlControls.Bits.xovercal = 0;
      MemWritePciCfgEp (host, socket, ch, DDRCRCTLCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControls.Data);

      if ((host->var.common.cpuType == CPU_HSX) ||( IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX))) { // WA reversed for BDX ML A0 and DE V0
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          dataControl3.Data = (*channelNvList)[ch].dataControl3[strobe];
          dataControl3.Bits.lvlshft_holden = host->nvram.mem.lvlshft_holden = 1;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG), dataControl3.Data);
        } // strobe loop
      } else { //BDX EP and DE C0
        for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
          dataControl4.Data =(*channelNvList)[ch].dataControl4[strobe];
          dataControl4.Bits.bypassinitoptolatchenable = 1;
          (*channelNvList)[ch].dataControl4[strobe] = dataControl4.Data;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL4_0_MCIO_DDRIO_REG), dataControl4.Data);
        } // strobe loop
      }
      //
      // Enable 2 cycle bypass after CPGC activity is done
      //
      chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
      chknBit.Bits.dis_2cyc_byp = 0;
      chknBit.Bits.cmd_oe_alwy_on = 0;
      MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit.Data);

      //
      // Program Read Pending Queue Age Counters
      //
      rpqAge.Data = MemReadPciCfgEp (host, socket, ch, RPQAGE_MCDDC_CTL_REG);
      rpqAge.Bits.cpugtcount = CPU_GT_COUNT;
      rpqAge.Bits.iocount = IO_COUNT;
      MemWritePciCfgEp (host, socket, ch, RPQAGE_MCDDC_CTL_REG, rpqAge.Data);
    } // ch loop

    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;
      //
      // Get MCMTR
      //
      mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);

      if (host->setup.mem.options & BANK_XOR_EN) {
        //
        // Enable bank xor address mapping
        //
        mcMtr.Bits.bank_xor_enable = 1;
      } else {
        mcMtr.Bits.bank_xor_enable = 0;
      }

      MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
      //
      // Allow CS to float when a command is not being issued
      //
      mcInitStateG.Data = MemReadPciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.cs_oe_en = 0;
      MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);

      mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
      //
      // Enter normal mode
      //
      mcMtr.Bits.normal     = 1;
      mcMtr.Bits.trng_mode  = 3;

      //
      // Set to closed page mode
      //
      if (host->setup.mem.options & PAGE_POLICY) {
        mcMtr.Bits.close_pg = CLOSED_PAGE_EN;
      }
      MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
    } // mcId loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      //
      // Clear the CKE override
      //
      miscCKECtl.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
      miscCKECtl.Bits.cke_override = 0;
      miscCKECtl.Bits.cke_on = 0;
      MemWritePciCfgEp (host, socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
    } // ch loop

    //
    // Configure power management features
    //
    PowerManagementSetup(host, socket);


#ifdef  LRDIMM_SUPPORT

    //
    // Workaround for BIOS HSD 4986863: ZQCal won't be issued to all ranks when running LRDIMM encoded CS mode
    //
    if ((host->nvram.mem.socket[socket].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {

      ChWaMask.Data = 0;

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((*channelNvList)[ch].enabled == 0) continue;

        //
        // Workaround for BIOS HSD 4987380
        //
        if (((*channelNvList)[ch].lrDimmPresent) && ((*channelNvList)[ch].maxDimm == 3)) {
          vmseLinkWidth.Data =  MemReadPciCfgEp (host, socket, ch, VMSE_LINK_WIDTH_MCDDC_DP_REG);
          vmseLinkWidth.Bits.ena_cpgc_verr_fsm = 1;
          MemWritePciCfgEp (host, socket, ch, VMSE_LINK_WIDTH_MCDDC_DP_REG, vmseLinkWidth.Data);
        }

        //
        // Workaround for BIOS HSD 4986863
        //
        if ((*channelNvList)[ch].encodedCSMode == 1) {
          if ((ch < 2) || Is1HA(host)) {
            ChWaMask.Bits.imc0chmask |= 1 << ch;
          } else {
            ChWaMask.Bits.imc1chmask |= 1 << (ch - 2);
          }

          //
          // Setup CADB to be used by pcode later
          //
          MemWritePciCfgEp (host, socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, 0);
          MemWritePciCfgEp (host, socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);
          seqCtl0.Data = 0;
          seqCtl0.Bits.global_control = 1;
          seqCtl0.Bits.initialization_mode = 2;
          MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                            CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(GetMCID(host, socket, ch), ch)), seqCtl0.Data);
          ProgramCADB (host, socket, ch, 0, 6, ZqWAPattern0, ZqWAPattern1);

        } // if encoded mode
      } // ch loop

      //
      // Write the mailbox command
      //
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) (MAILBOX_BIOS_CMD_B273548_WA_CHANNELS | (ChWaMask.Data << 8)), 0);
      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "MboxStatus: %d MAILBOX_BIOS_CMD_B273548_WA_CHANNELS = 0x%x\n", PcodeMailboxStatus, ChWaMask.Data));
    } // DDR4 lrDimmPresent

#endif   //LRDIMM_SUPPORT



    //
    // Start periodic RCOMPS
    //

    //
    //Workaround for BIOS HSD 4166184 & 4166091: Disable RCOMP's
    //
    //
    // HSD4167626 Use Pcode mailbox PCU_MISC_CONFIG to disable periodic RCOMPS
    //

    // Read PCU_MISC_CONFIG data
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
    if (PcodeMailboxStatus == 0) {
      PCUData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);

      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "MboxStatus: %d PCU_MISC_CONFIG = 0x%x\n", PcodeMailboxStatus, PCUData));
    }

    // Clear bit 24 to enable periodic RCOMP
    PCUData &= ~BIT24;

    if (host->setup.mem.optionsExt & RCOMP_TYPE) {
      PCUData |= BIT22;
    } else {
      PCUData &= ~BIT22;
    }

    if ((host->var.common.cpuType == CPU_HSX) || (IsDESku(host) && CheckSteppingLessThan(host, CPU_BDX, C0_REV_BDX))){
      //
      // HSD 4986822: PKGC RCOMP disable WA
      //
      PCUData |= (BIT0 << 23);
    } else {
      //
      // Do not apply the PKGC RCOMP disable WA for BDX-EP >= A0 and BDX-DE >= C0
      //
      PCUData &= ~(BIT0 << 23);
    }

    // Write back PCU_MISC_CONFIG data
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PCUData);

    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);

    if (PcodeMailboxStatus == 0) {
      PCUData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);

      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "MBoxStatus: %d PCU_MISC_CONFIG = 0x%x\n", PcodeMailboxStatus, PCUData));
    }
  } else if (host->var.common.cpuType == CPU_BDX) { // If memory not installed and CPU is BDX
    // Read PCU_MISC_CONFIG data
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
    if (PcodeMailboxStatus == 0) {
      PCUData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);

      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "MboxStatus: %d PCU_MISC_CONFIG = 0x%x\n", PcodeMailboxStatus, PCUData));
    }

    if (host->setup.mem.optionsExt & RCOMP_TYPE) {
      PCUData |= BIT22;
    } else {
      PCUData &= ~BIT22;
    }
    PCUData &= ~BIT23; // Do not apply HSD 4986822: PKGC RCOMP disable WA

    // Write back PCU_MISC_CONFIG data
    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, (UINT32) MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PCUData);

    PcodeMailboxStatus = WriteBios2PcuMailboxCommand (host, socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);

    if (PcodeMailboxStatus == 0) {
      PCUData = ReadCpuPciCfgEx (host, socket, 0, BIOS_MAILBOX_DATA);

      MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
          "MBoxStatus: %d PCU_MISC_CONFIG = 0x%x for non-populated skt\n", PcodeMailboxStatus, PCUData));
    }
  } // if/not memory installed

  biosResetCpl.Data = ReadCpuPciCfgEx (host, socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG);
  biosResetCpl.Bits.rst_cpl2 = 1;
  WriteCpuPciCfgEx (host, socket, 0, BIOS_RESET_CPL_PCU_FUN1_REG, biosResetCpl.Data);

  return SUCCESS;
} // SwitchToNormalMode

/**

  Configure power management features

  @param host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
void
PowerManagementSetup (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8                                       ch;
  UINT8                                       dimm;
  UINT8                                       ckeThrottling;
  UINT8                                       mem_pwr_save = 0;
  UINT8                                       pdwnCkModeWA;
  UINT8                                       pmSrefValue = 0;  //Last "sref_en" value programmed on a enabled channel
#ifdef  LRDIMM_SUPPORT
  UINT8                                       srefDisabledForLrdimm = 0;
#endif  //LRDIMM_SUPPORT
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  PM_PDWN_MCDDC_CTL_STRUCT                    pm_pdwn;
  PM_SREF_MCDDC_CTL_STRUCT                    pm_sref;
  TCRFTP_MCDDC_CTL_STRUCT                         tcrftp;
  DYNAMIC_PERF_POWER_CTL_PCU_FUN2_HSX_BDX_STRUCT  dynamicPerfPowerCtl;

  socket = host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //
  if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) return;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  dynamicPerfPowerCtl.Data = ReadCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG);

  if (host->setup.mem.MemPwrSave == MEM_PWR_SAV_AUTO) {
    if (host->var.common.cpuType == CPU_HSX) {
      mem_pwr_save = MEM_PWR_SAV_DEFAULT_HSX;
    } else if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
      mem_pwr_save = MEM_PWR_SAV_DEFAULT_DDR4;
    } else {
      mem_pwr_save = MEM_PWR_SAV_DEFAULT_DDR3;
    }
  } else {
    mem_pwr_save = host->setup.mem.MemPwrSave;
    if ((host->var.common.cpuType == CPU_BDX) && (host->nvram.mem.dramType == SPD_TYPE_DDR4) && (MEM_PWR_SAV_SLOW == mem_pwr_save)) {
      mem_pwr_save = MEM_PWR_SAV_FAST; // PPD Slow mode is not supported in DDR4. If manually seleted to slow mode , override to Fast mode.
    }
  }

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

#ifdef  LRDIMM_SUPPORT
    if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode) && (mem_pwr_save == MEM_PWR_SAV_FAST)) {
      //
      // BIOS HSD 5002622, if DDR4 LRDIMM 3DPC present, disable PPD
      //
      mem_pwr_save = MEM_PWR_SAV_APD;
    }
#endif  //LRDIMM_SUPPORT

    //
    // Memory Power Savings Mode
    //
    pm_pdwn.Data = MemReadPciCfgEp (host, socket, ch, PM_PDWN_MCDDC_CTL_REG);
    pm_sref.Data = MemReadPciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG);

      switch (mem_pwr_save) {
        // Disabled
        case MEM_PWR_SAV_DISABLE:
          pm_pdwn.Bits.pdwn_mode_apd        = 0;
          pm_pdwn.Bits.pdwn_mode_ppd        = 0;
          pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
          pm_sref.Bits.sref_en              = 0;
          dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 1;
        dynamicPerfPowerCtl.Data |= BIT30;
          break;

        // Slow (HW Auto Tune)
        case MEM_PWR_SAV_SLOW:
          pm_pdwn.Bits.pdwn_mode_apd        = 0;
          pm_pdwn.Bits.pdwn_mode_ppd        = 1;
          pm_pdwn.Bits.pdwn_mode_slow_exit  = 1;
          pm_sref.Bits.sref_en              = 1;
          pm_pdwn.Bits.pdwn_idle_cntr       = PDWN_IDLE_CNTR;
          dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 0;
          break;

        // Fast (HW Auto Tune)
        case MEM_PWR_SAV_FAST:
          pm_pdwn.Bits.pdwn_mode_apd        = 0;
          pm_pdwn.Bits.pdwn_mode_ppd        = 1;
          pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
          pm_sref.Bits.sref_en              = 1;
          pm_pdwn.Bits.pdwn_idle_cntr       = PDWN_IDLE_CNTR;
          dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 0;
          break;

        // APD
        case MEM_PWR_SAV_APD:
          pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
          pm_pdwn.Bits.pdwn_mode_ppd        = 0;
          pm_pdwn.Bits.pdwn_mode_apd        = 1;
          pm_sref.Bits.sref_en              = 1;
          pm_pdwn.Bits.pdwn_idle_cntr       = PDWN_IDLE_CNTR;
          dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 0;
          break;

        case MEM_PWR_SAV_USER:
          //
          // Set to 0 based on HSD 4986998
          //
          dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 1;
        dynamicPerfPowerCtl.Data |= BIT30;

          ckeThrottling = host->setup.mem.ckeThrottling;

          // Override the users selection if we don't support that mode
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (ckeThrottling == CKE_PPDS)) {
            ckeThrottling = CKE_PPDF;
          } else if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && (ckeThrottling == CKE_APD_PPDS)){
            ckeThrottling = CKE_APD_PPDF;
          }

#ifdef  LRDIMM_SUPPORT
          if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode) && (ckeThrottling != CKE_MODE_OFF)) {
            //
            // BIOS HSD 5002622, if DDR4 LRDIMM 3DPC present, force ckeThrottling to APD
            //
            ckeThrottling = CKE_APD;
          }
#endif  //LRDIMM_SUPPORT
          switch (ckeThrottling) {
            case CKE_MODE_OFF:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
              pm_pdwn.Bits.pdwn_mode_ppd        = 0;
              pm_pdwn.Bits.pdwn_mode_apd        = 0;
              break;

            case CKE_APD:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
              pm_pdwn.Bits.pdwn_mode_ppd        = 0;
              pm_pdwn.Bits.pdwn_mode_apd        = 1;
              break;

            case CKE_PPDF:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
              pm_pdwn.Bits.pdwn_mode_ppd        = 1;
              pm_pdwn.Bits.pdwn_mode_apd        = 0;
              break;

            case CKE_PPDS:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 1;
              pm_pdwn.Bits.pdwn_mode_ppd        = 1;
              pm_pdwn.Bits.pdwn_mode_apd        = 0;
              break;

            case CKE_APD_PPDF:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 0;
              pm_pdwn.Bits.pdwn_mode_ppd        = 1;
              pm_pdwn.Bits.pdwn_mode_apd        = 1;
              break;

            case CKE_APD_PPDS:
              pm_pdwn.Bits.pdwn_mode_slow_exit  = 1;
              pm_pdwn.Bits.pdwn_mode_ppd        = 1;
              pm_pdwn.Bits.pdwn_mode_apd        = 1;
              break;
          } // switch (host->setup.mem.ckeThrottling)

          // Manual Opportunistic self-refresh
          if (host->setup.mem.options & OPP_SELF_REF_EN) {
            pm_sref.Bits.sref_en = 1;
          } else {
            pm_sref.Bits.sref_en = 0;
          }
          pm_pdwn.Bits.pdwn_idle_cntr         = PDWN_IDLE_CNTR;

      } // switch (host->setup.mem.MemPwrSave)

      //
      // Always force imc_apm_override_enable to 1 for lockstep configs per BIOS HSD 4987493
      //
      if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
        dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 1;
      }
      WriteCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, dynamicPerfPowerCtl.Data);

      pm_pdwn.Bits.pdwn_ibt_off_en = 0x0;


      //
      // HSD 4985867:
      //
      pdwnCkModeWA = 0;
      dimmNvList = GetDimmNvList(host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) && ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_B0)) {
          pdwnCkModeWA = 1;
          break;
        }
      } //dimm

      if (host->setup.mem.pdwnCkMode == CK_SELF_REF_AUTO) {
        if (pdwnCkModeWA) {
          pm_pdwn.Bits.pdwn_ck_mode = CK_SELF_REF_HIGH;
        } else {
          pm_pdwn.Bits.pdwn_ck_mode = CK_SELF_REF_LOW;
        }
      } else {
        pm_pdwn.Bits.pdwn_ck_mode = host->setup.mem.pdwnCkMode;
      }

      //MDLL_SDEN = 1 and CK/CK# = driven is not a legal combination.  Force CK/CK# to be low
      if ((host->setup.mem.options & MDLL_SHUT_DOWN_EN) && (pm_pdwn.Bits.pdwn_ck_mode == CK_SELF_REF_DRIVEN) && ((*channelNvList)[ch].lrDimmPresent == 0)) {
        pm_pdwn.Bits.pdwn_ck_mode = CK_SELF_REF_LOW;
      }

      if ((host->setup.mem.options & PDWN_SR_CKE_MODE) && (host->nvram.mem.dimmTypePresent == RDIMM)) {
        pm_pdwn.Bits.pdwn_sr_cke_mode     = 0x1;
      } else {
        pm_pdwn.Bits.pdwn_sr_cke_mode     = 0x0;
      }

#ifdef  LRDIMM_SUPPORT
      if ((*channelNvList)[ch].lrDimmPresent) {
        pm_pdwn.Bits.pdwn_rdimm_rc9_a4_a3 = 0;
      }
#endif  //LRDIMM_SUPPORT

      MemWritePciCfgEp (host, socket, ch, PM_PDWN_MCDDC_CTL_REG, pm_pdwn.Data);

#ifdef  LRDIMM_SUPPORT
    if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode)) {
      //
      // BIOS HSD 4986863, if DDR4 LRDIMM 3DPC present, force Opportunistic self-refresh to disabled
      //
      pm_sref.Bits.sref_en = 0;
      srefDisabledForLrdimm = 1;
    }
#endif  //LRDIMM_SUPPORT

    if (pm_sref.Bits.sref_en == 1)  pmSrefValue = 1;
    else  pmSrefValue = 0;

    //
    // PM Self Refresh Control
    //
    tcrftp.Data                 = MemReadPciCfgEp (host, socket, ch, TCRFTP_MCDDC_CTL_REG);
    pm_sref.Bits.sref_idle_cntr = SREF_IDLE_CNTR;
    MemWritePciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG, pm_sref.Data);

  } // ch loop

  // We write only the "sref_en" bit to all channels regardless of whether there is memory on it or not (HSD 4987829)
  for (ch = 0; ch < MAX_CH; ch++) {
#ifdef  LRDIMM_SUPPORT
    if (((*channelNvList)[ch].lrDimmPresent) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {  //Writing "sref_en" to all Non-EncodedModeCS Channels
      pm_sref.Data = MemReadPciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG);  //Reading from the register to not overwrite other values for enabled channels
      if (srefDisabledForLrdimm)  pm_sref.Bits.sref_en = 0; //If LRDIMM Encoded mode is present on any channel, then we disable sref for all channels
      MemWritePciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG, pm_sref.Data);
    } else
#endif  //LRDIMM_SUPPORT
    {
      if ((*channelNvList)[ch].enabled == 1) continue;  //Program sref for all "disabled" channels
      pm_sref.Data = MemReadPciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG);
      pm_sref.Bits.sref_en = pmSrefValue;
      MemWritePciCfgEp (host, socket, ch, PM_SREF_MCDDC_CTL_REG, pm_sref.Data);
    }
  } // ch loop

  //WA: HSD 4987829 - (MRC needs to set sref_en same across all channels)
  if (pmSrefValue == 0) {
    dynamicPerfPowerCtl.Data = ReadCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG);
    dynamicPerfPowerCtl.Bits.sapm_osr_override = 1;
    WriteCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, dynamicPerfPowerCtl.Data);
  }//if

#ifdef LRDIMM_SUPPORT
  if ((srefDisabledForLrdimm == 1) && (host->var.common.cpuType == CPU_BDX)) {
    //
    // BIOS HSD 5002879, if DDR4 LRDIMM 3DPC/3DS present, force imc_apm_override_enable and sapm_osr_override to 1
    //
    dynamicPerfPowerCtl.Data = ReadCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG);
    dynamicPerfPowerCtl.Bits.imc_apm_override_enable = 1;
    dynamicPerfPowerCtl.Bits.sapm_osr_override       = 1;
    WriteCpuPciCfgEx (host, socket, 0, DYNAMIC_PERF_POWER_CTL_PCU_FUN2_REG, dynamicPerfPowerCtl.Data);
  }
#endif  //LRDIMM_SUPPORT

} // PowerManagementSetup


/**

  This function writes the txon bit on or off for all strobes of DATACONTROL0_0_MCIO_DDRIO

  @param host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)
  @param setting         - 0 /1 = txon bit value

  @retval N/A

**/
void
setTxon (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    setting
  )
{
  UINT8                                       strobe;
  struct channelNvram (*channelNvList)[MAX_CH];
  DATACONTROL0_0_MCIO_DDRIO_STRUCT            dataControl0;

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  dataControl0.Data = (*channelNvList)[ch].dataControl0;
  dataControl0.Bits.txon = setting;

  for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
    //
    // Skip if this is an ECC strobe when ECC is disabled
    //
    if ((!host->nvram.mem.eccEn) && ((strobe == 8) || (strobe == 17))) continue;
    MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
  } // strobe loop
}

/**

  Clears IMC error registers

  @param host      - Pointer to sysHost

  @retval N/A

**/
void
ClearIMCErrors (
  PSYSHOST host
  )
{
} // ClearIMCErrors

void
ResetSystemEx (
            PSYSHOST host,
            UINT8    resetType
            )
/**


  Execute a platform reset

  @param host      - Pointer to sysHost
  @param resetType - Reset type

  @retval N/A

**/
{

    //
    // Reset the system
    //
    OutPort8 (host, 0xcf9, resetType);

} // ResetSystem

/**

  Programming required late in the memory initialization process

  @param host  - Pointer to sysHost

  @retval N/A

**/
UINT32
MemLate (
  PSYSHOST host
  )
{
  UINT8                                 socket;
  UINT8                                 mcId;
  UINT8                                 ch;
  UINT8                                 strobe;
  DATACONTROL0_0_MCIO_DDRIO_STRUCT      dataControl0;
  DATACONTROL3_0_MCIO_DDRIO_STRUCT      dataControl3;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT      biosScratchPad1;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT          mcMtr;
  struct channelNvram                   (*channelNvList)[MAX_CH];
//#ifdef MEM_SV_HOOKS
  MCBGFTUNE_MC_MAIN_BDX_STRUCT          mcbgftune_mc_main_bdx;
  HASYSDEFEATURE3_HA_CFG_BDX_STRUCT     hasysdefeature3_ha_bdx;
//#endif
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT       mcmain_chkn_bits;

  if (host->setup.mem.options & PAGE_POLICY) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      // Continue if this socket is not present
      if (host->nvram.mem.socket[socket].enabled == 0) continue;

      for (mcId = 0; mcId < MAX_IMC; mcId++) {
        if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

        // Set to closed page mode
        mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
        mcMtr.Bits.close_pg = CLOSED_PAGE_EN;

        MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
      } // mcId loop
    } // socket loop
  } // if closed page

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (host->nvram.mem.socket[socket].enabled == 0) continue;
    channelNvList = &host->nvram.mem.socket[socket].channelList;


    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
        dataControl0.Data = (*channelNvList)[ch].dataControl0;

      //  BDX: 4985906, 5097293 - Keep internalclockson for all BDX steppings
      if ((host->var.common.cpuType == CPU_BDX)) {
        dataControl0.Bits.internalclockson = 1;
      }

      for (strobe = 0; strobe < MAX_STROBE / 2; strobe++) {
        MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL0_0_MCIO_DDRIO_REG), dataControl0.Data);
        if ((host->var.common.bootMode == S3Resume || host->var.mem.subBootMode == WarmBootFast) && (host->nvram.mem.lvlshft_holden)) {
          dataControl3.Data = MemReadPciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG));
          dataControl3.Bits.lvlshft_holden = 1;
          MemWritePciCfgEp (host, socket, ch, UpdateIoRegister (host, 0, strobe, DATACONTROL3_0_MCIO_DDRIO_REG), dataControl3.Data);
        } // lvlshft_holden
      } // strobe loop
      (*channelNvList)[ch].dataControl0 = dataControl0.Data;
    } // ch loop
    biosScratchPad1.Data = ReadCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    biosScratchPad1.Data &= ~BIT6;
    WriteCpuPciCfgEx (host, socket, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, biosScratchPad1.Data);

    if ((host->nvram.mem.RASmode & CH_LOCKSTEP) != CH_LOCKSTEP){ //Apply HADB_RD & WR only if lockstep is not enabled
      if ((host->var.common.cpuType == CPU_BDX) && (2 == host->var.mem.numChPerHA)) {// DE and ML with 2CHperHA SKU.
        mcbgftune_mc_main_bdx.Data = MemReadPciCfgMain (host, socket, MCBGFTUNE_MC_MAIN_REG);
        mcbgftune_mc_main_bdx.Bits.ha_bandwidth_fix = 1;
        MemWritePciCfgMain (host, socket, MCBGFTUNE_MC_MAIN_REG, mcbgftune_mc_main_bdx.Data);

        hasysdefeature3_ha_bdx.Data  = MemReadPciCfgMain (host, socket, HASYSDEFEATURE3_HA_CFG_REG);
        hasysdefeature3_ha_bdx.Bits.endoublehadbrdbw = 1;
        MemWritePciCfgMain (host, socket, HASYSDEFEATURE3_HA_CFG_REG, hasysdefeature3_ha_bdx.Data);
      }

      if (host->var.common.cpuType == CPU_BDX) {// BDX.
        hasysdefeature3_ha_bdx.Data  = MemReadPciCfgMain (host, socket, HASYSDEFEATURE3_HA_CFG_REG);
        hasysdefeature3_ha_bdx.Bits.endoublehadbwrbw = 1;
        MemWritePciCfgMain (host, socket, HASYSDEFEATURE3_HA_CFG_REG, hasysdefeature3_ha_bdx.Data);
      }
    }

    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;
      // Set defeature_10 bit
      mcmain_chkn_bits.Data = MemReadPciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG);
      mcmain_chkn_bits.Bits.defeature_10 = 1;

      MemWritePciCfgMC (host, socket, mcId, MCMAIN_CHKN_BITS_MC_MAIN_REG, mcmain_chkn_bits.Data);
    } // mcId loop

  } // socket loop

  //
  // Indicate data is good
  //
  host->nvram.mem.DataGood = 1;

  return SUCCESS;
} // MemLate

/**

  ConFigure DRAM Maintenance

  @param host  - Pointer to sysHost
  @param socket - Processor socket

  @retval N/A

**/
void
DramMaintenance (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT8               trrMode[MAX_CH];
  UINT8               dimm;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);

  if (host->setup.mem.dramMaint == DRAM_MAINT_DIS) {
    //
    // Completely disable
    //
    return;
  }

  MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                "DRAM Maintenance\n"));

  if (host->setup.mem.dramMaint == DRAM_MAINT_AUTO) {

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      //
      // Determine what TRR modes are supported
      //
      trrMode[ch] = DetectTRRSupport(host, socket, ch);

      //
      // Workaround for BIOS HSD 4986027
      //
      if ((host->var.common.cpuType == CPU_HSX) ||( IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX))) { // WA reversed for BDX ML A0 and DE V0
        if (host->nvram.mem.dramType == SPD_TYPE_DDR4) {
          if ((trrMode[ch] & TRR_MODE_A) && (((*channelNvList)[ch].numRanks > 4) || ((*channelNvList)[ch].maxDimm > 2))) {
            trrMode[ch] = PTRR_MODE;
          }
#ifdef  TRR_MODE_B_SUPPORT
          else if ((trrMode[ch] & TRR_MODE_B) && ((*channelNvList)[ch].numRanks > 4)) {
            trrMode[ch] = PTRR_MODE;
          }
#endif // TRR_MODE_B_SUPPORT
        } // 4986027
      }
      // WorkAround for BIOS HSD 4168340
      if (CheckSteppingLessThan(host, CPU_HSX, C0_REV_HSX)) {
        if (trrMode[ch] & TRR_MODE_A){
          dimmNvList = GetDimmNvList(host, socket, ch);
          for (dimm = 0; dimm < MAX_DIMM; dimm++){
            if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
            if ((*dimmNvList)[dimm].SPDMMfgId == MFGID_SAMSUNG || (*dimmNvList)[dimm].SPDMMfgId == MFGID_HYNIX || (*dimmNvList)[dimm].SPDMMfgId == MFGID_ELPIDA){
              trrMode[ch] = 0;
              break;
            }
          }
        }
      }
      if ((host->var.common.cpuType == CPU_HSX) ||( IsDESku(host) && CheckSteppingLessThan( host, CPU_BDX, C0_REV_BDX))) { // WA reversed for BDX ML A0 and DE V0
      //
      // Workaround for BIOS HSD 4986648: TRR mode is not supported for LRDIMMs
      //
        if (host->nvram.mem.socket[socket].lrDimmPresent) {
          if (trrMode[ch] & TRR_MODE_A) {
            trrMode[ch] = PTRR_MODE;
          }
#ifdef  TRR_MODE_B_SUPPORT
          else if (trrMode[ch] & TRR_MODE_B) {
            trrMode[ch] = PTRR_MODE;
          }
#endif // TRR_MODE_B_SUPPORT
        } // 4986648
      }
    } // ch loop
    // If Lockstep mode, make sure channel pairs are using the same trrMode
    if ((host->nvram.mem.RASmode & CH_LOCKSTEP) == CH_LOCKSTEP) {
      for (ch = 0; ch < MAX_CH; ch += 2) {
        if ((*channelNvList)[ch].enabled == 0) continue;
        //  If lockstep pairs are not in the same mode, then force one to match the other
        if (trrMode[ch] != trrMode[ch ^ BIT0]) {
          // If Ch A is immune and Ch B is not, then Ch A must be set to Ch B mode
          if (trrMode[ch] & TRR_IMMUNE) {
            trrMode[ch] = trrMode[ch ^ BIT0];
          // If Ch B is immune and Ch A is not, then Ch B must be set to Ch A mode
          } else if (trrMode[ch ^ BIT0] & TRR_IMMUNE) {
            trrMode[ch ^ BIT0] = trrMode[ch];
          // If Ch A is pTRR and Ch B is not (and not immune), then Ch B must be set to pTRR
          } else if (trrMode[ch] & PTRR_MODE) {
            trrMode[ch ^ BIT0] = PTRR_MODE;
          // If Ch B is pTRR and Ch A is not (and not immune), then Ch A must be set to pTRR
          } else if (trrMode[ch ^ BIT0] & PTRR_MODE) {
            trrMode[ch] = PTRR_MODE;
          // If none of these is true, TRR must be set to disabled
          } else {
            trrMode[ch] = 0;
            trrMode[ch ^ BIT0] = 0;
          }
        }
      } // ch loop
    }
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      //
      // Continue to the next channel if TRR is not supported or if immune
      //
      if ((trrMode[ch] == 0) || (trrMode[ch] == TRR_IMMUNE)) continue;

      ConfigureTRRMode(host, socket, ch, trrMode[ch]);
    } // ch loop
  } else {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      //
      // Manual
      //
      if(host->setup.mem.dramMaintMode == DRAM_MAINT_PTRR) {
        ConfigureTRRMode(host, socket, ch, PTRR_MODE);
      } else {
        if(host->setup.mem.dramMaintTRRMode == DRAM_MAINT_MODE_A) {
          ConfigureTRRMode(host, socket, ch, TRR_MODE_A);
        }
#ifdef  TRR_MODE_B_SUPPORT
         else {
          ConfigureTRRMode(host, socket, ch, TRR_MODE_B);
        }
#endif //  TRR_MODE_B_SUPPORT
      }
    } // ch loop
  }
} // DramMaintenance

void
ConfigureTRRMode (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     mode
  )
/*++

  Configure TRR Mode

  @param host    - Pointer to sysHost
  @param socket  - Processor socket
  @param ch      - Memory channel
  @param mode    - TRR Mode

  @retval N/A

--*/
{
  UINT8                             dimm;
  UINT8                             rank;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  ROWHAMMER_TIMER_MCDDC_CTL_STRUCT  rowHammerTimer;
  ROWHAMCONFIG_MCDDC_CTL_STRUCT     rowHamConfig;
  ROWHAMCONFIG2_MCDDC_CTL_STRUCT    rowHamConfig2;
  ROWHAMCONFIG3_MCDDC_CTL_STRUCT    rowHamConfig3;
  MCSCHED_CHKN_BIT_MCDDC_CTL_HSX_BDX_STRUCT chknBit;

  channelNvList = GetChannelNvList(host, socket);

  if(mode == PTRR_MODE) {
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "pTRR Mode Enabled\n"));
  } else if (mode == TRR_MODE_A) {
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "TRR Mode A Enabled\n"));
  }
#ifdef  TRR_MODE_B_SUPPORT
  else if (mode == TRR_MODE_B) {
    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "TRR Mode B Enabled\n"));
  }
#endif  // TRR_MODE_B_SUPPORT

  rowHammerTimer.Data = MemReadPciCfgEp (host, socket, ch, ROWHAMMER_TIMER_MCDDC_CTL_REG);
  rowHammerTimer.Bits.t_window = 0x400;
  MemWritePciCfgEp (host, socket, ch, ROWHAMMER_TIMER_MCDDC_CTL_REG, rowHammerTimer.Data);

  rowHamConfig.Data = MemReadPciCfgEp (host, socket, ch, ROWHAMCONFIG_MCDDC_CTL_REG);
  //
  // Assume DIMM rated for 125K accesses per 64ms
  //
  rowHamConfig.Bits.limit = 12500;
  //
  // Fully enable
  //
  rowHamConfig.Bits.enable = 3;
  //
  // Throttle threshold
  //
  rowHamConfig.Bits.throttle_thresh = 0xC;
  rowHamConfig.Bits.shrink_out_fifo = 0;
  rowHamConfig.Bits.disable2        = 0;
  rowHamConfig.Bits.inject_refresh  = 0;
  rowHamConfig.Bits.trigger_update  = 1;
  MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG_MCDDC_CTL_REG, rowHamConfig.Data);

  rowHamConfig2.Data = MemReadPciCfgEp (host, socket, ch, ROWHAMCONFIG2_MCDDC_CTL_REG);
  rowHamConfig2.Bits.ddr3_pause = (*channelNvList)[ch].common.nRP + tMOD_DDR4[host->nvram.mem.ratioIndex] + 1;
  rowHamConfig2.Bits.pass_mask = 0;
  if(mode == PTRR_MODE){
    rowHamConfig2.Bits.trr_mode = 0;
  } else {
    rowHamConfig2.Bits.trr_mode = 1;
  }
  rowHamConfig2.Bits.sweep_mode = 0;
  rowHamConfig2.Bits.swizzle_mode = 0;
#ifdef  TRR_MODE_B_SUPPORT
  if(mode == TRR_MODE_B) {
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if((*dimmNvList)[dimm].SPDMMfgId == MFGID_SAMSUNG) {
        rowHamConfig2.Bits.swizzle_mode |= (1 << dimm);
      }
    } // dimm loop
  } // if TRR mode B
  else
#endif // TRR_MODE_B_SUPPORT
  if(mode == PTRR_MODE) {
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if((*dimmNvList)[dimm].SPDMMfgId == MFGID_SAMSUNG) {
        rowHamConfig2.Bits.swizzle_mode |= (1 << dimm);
      }
    } // dimm loop
  } // if pTRR mode
  rowHamConfig2.Bits.half_t_ras = ((*channelNvList)[ch].common.nRAS + 1) / 2;
  rowHamConfig2.Bits.safe_mode = 0;
  rowHamConfig2.Bits.restart_delay = 0xC8;
  //
  // B0 workaround: HSD 4168430
  //
  if (CheckSteppingEqual(host, CPU_HSX, B0_REV_HSX)) {
    rowHamConfig2.Bits.safe_mode = 1;
  }
  //
  // Workaround for HSD 4987314
  //
  if((host->var.qpi.OutIsocEn == TRUE) && (mode == TRR_MODE_A)){
    rowHamConfig2.Bits.safe_mode = 1;
  }
  MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG2_MCDDC_CTL_REG, rowHamConfig2.Data);

  rowHamConfig3.Data = MemReadPciCfgEp (host, socket, ch, ROWHAMCONFIG3_MCDDC_CTL_REG);
  rowHamConfig3.Bits.pass_mask0 = GetPassMask(host, socket, ch, 0);
  rowHamConfig3.Bits.pass_mask1 = GetPassMask(host, socket, ch, 1);
  rowHamConfig3.Bits.pass_mask2 = GetPassMask(host, socket, ch, 2);
  rowHamConfig3.Bits.mirror = 0;
  rowHamConfig3.Bits.invert = 0;
  rowHamConfig3.Bits.trr_no_inc = 7;

#ifdef  TRR_MODE_B_SUPPORT
  if(mode == TRR_MODE_B){
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;

        if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
          rowHamConfig3.Bits.mirror |= (1 << (*rankList)[rank].CSIndex);
        }
      } // rank loop

      if ((host->nvram.mem.dimmTypePresent != UDIMM) && (host->nvram.mem.dimmTypePresent != SODIMM)) {
        rowHamConfig3.Bits.invert |= 1 << dimm;
      }

    } // dimm loop

    rowHamConfig3.Bits.trr_no_inc = 0;
  } // if TRR mode B
  else
#endif  // TRR_MODE_B_SUPPORT
  if(mode == PTRR_MODE){
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      rankList = GetRankNvList(host, socket, ch, dimm);

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;

        if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
          rowHamConfig3.Bits.mirror |= (1 << (*rankList)[rank].CSIndex);
        }
      } // rank loop

      if ((host->nvram.mem.dimmTypePresent != UDIMM) && (host->nvram.mem.dimmTypePresent != SODIMM)) {
        rowHamConfig3.Bits.invert |= 1 << dimm;
      }

    } // dimm loop

    rowHamConfig3.Bits.trr_no_inc = 0;
  } // if pTRR mode

  MemWritePciCfgEp (host, socket, ch, ROWHAMCONFIG3_MCDDC_CTL_REG, rowHamConfig3.Data);

  //
  // Workaround for 4987382
  //
  if(host->var.qpi.OutIsocEn == TRUE){
    chknBit.Data = MemReadPciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG);
    chknBit.Bits.dis_opp_cas = 1;
    MemWritePciCfgEp (host, socket, ch, MCSCHED_CHKN_BIT_MCDDC_CTL_REG, chknBit.Data);
  }

} // ConfigureTRRMode

UINT32
GetPassMask (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
/*++

  Get value for pass_mask

  @param host    - Pointer to sysHost
  @param socket  - Processor socket
  @param ch      - Channel number
  @param dimm    - DIMM number

  @retval Setting for pass_mask

--*/
{
  UINT32            passMask;
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(host, socket, ch);

  if ((*dimmNvList)[dimm].dimmPresent == 0) {
    passMask = 0;
  } else {
    switch((*dimmNvList)[dimm].numRowBits) {
      case 14:
        passMask = 0xF;
        break;
      case 15:
        passMask = 0xE;
        break;
      case 16:
        passMask = 0xC;
        break;
      case 17:
        passMask = 0x8;
        break;
      case 18:
        passMask = 0x0;
        break;
      default:
        passMask = 0x0;
        break;
    } // switch
  }

  return passMask;
} // GetPassMask

/**

  Set the bit indicating memory initialization is complete

  @param host  - Pointer to sysHost

  @retval N/A

**/
void
SetMemDone (
  PSYSHOST host
  )
{
  UINT8                             socket;
  UINT8                             ch;
  UINT8                             mcId;
  struct channelNvram               (*channelNvList)[MAX_CH];

  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT mcInitStateG;


  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;

    channelNvList = &host->nvram.mem.socket[socket].channelList;

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

    } // ch loop

    //
    // Set MRC_DONE
    //
    for (mcId = 0; mcId < MAX_IMC; mcId++) {
      if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

      mcInitStateG.Data = MemReadPciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.mrc_done  = 1;
      MemWritePciCfgMC (host, socket, mcId, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    }

  } // socket loop
} // SetMemDone

/**

  Returns the physical rank number

  @param dimm  - Current DIMM
  @param rank  - Rank number on DIMM to return

  @retval Physical rank number

**/
UINT8
GetPhyRank (
  UINT8 dimm,
  UINT8 rank
  )
{
  return (dimm * 4) + rank;
} // GetPhyRank


/**

    Initialize ADR

    @param host - Pointer to SysHost

    @retval SUCCESS

**/
UINT32
InitADR(
  PSYSHOST  host
  )
{
  UINT8 socket;
#ifndef GRANGEVILLE_FLAG
  UINT32          data;
  volatile UINT32 * pointer;
#endif
  PCU_BIOS_SPARE2_PCU_FUN3_STRUCT           pcuBiosSpare2Reg;
#ifdef DE_SKU
  UINT32                                    PcodeMailboxStatus = 0;
#endif
  socket = host->var.mem.currentSocket;

  // Clear ADR_RST_STS on all boots regardless of if ADR is enabled
  // or not to make sure ADR_COMPLETE works correct.
  if (!host->var.common.resetRequired) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX))
      rcPrintf ((host,"No Pending Reset, clearing the ADR status bit\n"));
#endif // SERIAL_DBG_MSG
#ifdef GRANGEVILLE_FLAG
   PlatformClearADR(host);
#else
    pointer = (UINT32 *)(host->var.common.rcbaBase + SSB_PM_STS); // PCH_PM_STS
    *pointer = ADR_RST_STS;
#endif
  }

  if (host->setup.mem.ADREn) {
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MINMAX))
      rcPrintf ((host,"Enabling ADR\n"));
#endif // SERIAL_DBG_MSG
#ifndef GRANGEVILLE_FLAG
    pointer = (UINT32 *)(host->var.common.rcbaBase + PM_SYNC_MISC_CFG);
    data = *pointer;
    data |= GPIO_B_SEL; // Set to GPIO_B to GPIO37
    *pointer = data;

    pointer = (UINT32 *)(host->var.common.rcbaBase + ADR_EN); // ADR_EN
    data = 0;
    data |= ADR_GPIO_SEL_B; // Set to GPIO_B
    // data |= ADR_GPIO_RST_EN; // Enable Reset Following ADR Entry
    data |= ADR_FEAT_EN;
    *pointer = data;

    pointer = (UINT32 *)(host->var.common.rcbaBase + PM_SYNC_MODE);
    data = *pointer;
    data |= GPIO_B_PM_SYNC_EN; // Enable GPIO_B to PM_SYNC
    data |= GPIO_B_C0_TX_EN;
    data |= REG_10_C0_TX_EN;
    *pointer = data;

    pointer = (UINT32 *)(host->var.common.rcbaBase + ADR_EN); // ADR_EN
    data = 0;
    data |= ADR_GPIO_SEL_B; // Set to GPIO_B
    // data |= ADR_GPIO_RST_EN; // Enable Reset Following ADR Entry
    data |= ADR_FEAT_EN;
    *pointer = data;
#endif
  pcuBiosSpare2Reg.Data = ReadCpuPciCfgEx(host, socket, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG);
  pcuBiosSpare2Reg.Data = pcuBiosSpare2Reg.Data |= BIT18;
  WriteCpuPciCfgEx(host, socket, 0, PCU_BIOS_SPARE2_PCU_FUN3_REG, pcuBiosSpare2Reg.Data);
#ifdef DE_SKU
    //Check if BlockScTrafficOnAdr is enabled. If so send a b2p message to block all PCIe / SC traffic during ADR.
    if (host->setup.mem.BlockScTrafficOnAdr == 1) {
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand(host, socket, (UINT32)MAILBOX_BIOS_CMD_ADR_IIO_BLOCK, 1);
      if (PcodeMailboxStatus != 0)
        rcPrintf ((host, "Mailbox command to enable ADR_IIO_BLOCK failed\n"));
      else
        rcPrintf ((host, "ADR_IIO_BLOCK enabled via B2P nessage\n"));

    }
    else { // Disable blocking SC traffic
      PcodeMailboxStatus = WriteBios2PcuMailboxCommand(host, socket, (UINT32)MAILBOX_BIOS_CMD_ADR_IIO_BLOCK, 0);
      if (PcodeMailboxStatus != 0)
        rcPrintf ((host, "Mailbox command to disable ADR_IIO_BLOCK failed\n"));
      else
        rcPrintf ((host, "ADR_IIO_BLOCK disabled via B2P mailbox \n"));
    }
#endif
#ifdef SERIAL_DBG_MSG
    if (checkMsgLevel(host, SDBG_MAX))
      rcPrintf ((host,"Enabled ADR\n"));
#endif // SERIAL_DBG_MSG
  }
  return SUCCESS;
}

/**

  Returns the DDR frequency supported by this config

  @param host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param vdd   - Vdd (1.5v or 1.35V)

  @retval Supported DDR frequency

**/
UINT8
GetPORDDRFreq (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     vdd
  )
{
  UINT8                     dimm;
  UINT8                     maxRanks = 0;
  UINT8                     porTableIndex;
  UINT8                     ddrFreq = NOT_SUP;
  UINT8                     dimmType;
  UINT8                     socketType;
  UINT16                    freqTableLength;
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  struct DimmPOREntryStruct *freqTablePtr;
  UINT8                     rev0RawCardDimm = 0;

  freqTablePtr = &freqTable[0];
  freqTableLength = sizeof(freqTable)/sizeof(freqTable[0]);
  CoreLookupFreqTable(host, &freqTablePtr, &freqTableLength);

  channelNvList = &host->nvram.mem.socket[socket].channelList;

  dimmNvList = GetDimmNvList(host, socket, ch);

  // Loop for each DIMM
  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {

    // Skip if no DIMM present
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    if (maxRanks < (*dimmNvList)[dimm].numDramRanks) {
      if (((*dimmNvList)[dimm].SPDDeviceType & SPD_SIGNAL_LOADING) == SPD_3DS_TYPE){
        maxRanks = (*dimmNvList)[dimm].numDramRanks * (*dimmNvList)[dimm].dieCount;
      } else {
        maxRanks = (*dimmNvList)[dimm].numDramRanks;
      }
    }
    if (((*dimmNvList)[dimm].minTCK == DDR_2133_TCK_MIN) && ((((*dimmNvList)[dimm].SPDRawCard) & (BIT6 + BIT5)) == 0 )) {
      rev0RawCardDimm = 1;
    }
  } // dimm loop
  //
  // Default to RDIMM or UDIMM
  //
  dimmType = host->nvram.mem.dimmTypePresent;

  //
  // Check for LR-DIMM
  //
  if (host->nvram.mem.socket[socket].lrDimmPresent) {
    dimmType = LRDIMM;
  }

  //
  // Check socket type
  //
  if ((host->nvram.mem.dimmTypePresent == UDIMM) && !IsDESku(host) && (host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    socketType = SOCKET_HEDT;
  } else {
    socketType = host->var.common.socketType;
  }

  for (porTableIndex = 0; porTableIndex < freqTableLength; porTableIndex++) {
    if ((freqTablePtr[porTableIndex].procType == host->var.common.cpuFamily) &&
        (freqTablePtr[porTableIndex].socketType == socketType) &&
        (freqTablePtr[porTableIndex].dramType == host->nvram.mem.dramType) &&
        (freqTablePtr[porTableIndex].spc == host->var.mem.socket[socket].channelList[ch].numDimmSlots) &&
        (freqTablePtr[porTableIndex].dpc == (*channelNvList)[ch].maxDimm) &&
        (freqTablePtr[porTableIndex].dimmType == dimmType) &&
        (freqTablePtr[porTableIndex].numRanks >= maxRanks) &&
        (freqTablePtr[porTableIndex].vdd == vdd)) {

      if (host->setup.mem.enforcePOR == ENFORCE_POR_EN) {
        // POR setting
        ddrFreq = freqTablePtr[porTableIndex].porFreq;
      } else if (host->setup.mem.enforcePOR == ENFORCE_STRETCH_EN) {
        // Stretch Goal
        ddrFreq = freqTablePtr[porTableIndex].stretchFreq;
      }
      //HSD 5003133: Force 2133 RawCard 0 RDIMM's to run at 1866 for 2 DPC
      if (IsBDXEP(host) && (host->nvram.mem.dramType == SPD_TYPE_DDR4) && (dimmType == RDIMM) && ((*channelNvList)[ch].maxDimm == 2) && (rev0RawCardDimm)) {
        MemDebugPrint((host, SDBG_MINMAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "DDR-2133 RawCard 0 RDIMM is present, Forcing DDR frequency to 1867\n"));
        ddrFreq = DDR_1867;          
      }
      break;
    }
  } // porTableIndex

  //
  // Check if we found a valid config
  //
  if (porTableIndex == freqTableLength) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Configuration not in POR table!\n"));
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "Memory Configuration: Vdd = %d, procType = 0x%x, socketType = %d, dramType = %d, spc = %d, dpc = %d, dimmType = %d, numRanks = %d\n",
      vdd,
      host->var.common.cpuFamily,
      socketType,
      host->nvram.mem.dramType,
      host->var.mem.socket[socket].channelList[ch].numDimmSlots,
      (*channelNvList)[ch].maxDimm,
      dimmType,
      maxRanks));
  }


  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "GetPORDDRFreq returns ddrfreq = %d\n", ddrFreq));

  return ddrFreq;
} // GetPORDDRFreq

/**

  Checks if the requested Vdd is supported

  @param host  - Pointer to sysHost
  @param socket  - Socket Id
  @param ch    - Channel number (0-based)
  @param vdd   - Vdd (1.5v or 1.35V)

  @retval Supported DDR frequency

**/
UINT8
CheckPORDDRVdd (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     vdd
  )
{
  UINT8 porTableIndex;
  UINT8 dimmType;
  UINT8 maxRanks = 0;
  UINT8 dimm = 0;
  UINT8 status = FAILURE;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct DimmPOREntryStruct *freqTablePtr;
  UINT16 freqTableLength;

  freqTablePtr = &freqTable[0];
  freqTableLength = sizeof(freqTable)/sizeof(freqTable[0]);
  CoreLookupFreqTable(host, &freqTablePtr, &freqTableLength);

  dimmNvList = GetDimmNvList(host, socket, ch);
  channelNvList = &host->nvram.mem.socket[socket].channelList;

  // Loop for each DIMM
  for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {

    // Skip if no DIMM present
    if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

    if (maxRanks < (*dimmNvList)[dimm].numDramRanks) {
      maxRanks = (*dimmNvList)[dimm].numDramRanks;
    }
  } // dimm loop


  //
  // Default to RDIMM or UDIMM
  //
  dimmType = host->nvram.mem.dimmTypePresent;

  //
  // Check for LR-DIMM
  //
  if (host->nvram.mem.socket[socket].lrDimmPresent) {
    dimmType = LRDIMM;
  }

  for (porTableIndex = 0; porTableIndex < freqTableLength; porTableIndex++) {
    if ((freqTablePtr[porTableIndex].procType == host->var.common.cpuFamily) &&
        (freqTablePtr[porTableIndex].socketType == host->var.common.socketType) &&
        (freqTablePtr[porTableIndex].spc == host->var.mem.socket[socket].channelList[ch].numDimmSlots) &&
        (freqTablePtr[porTableIndex].dpc == (*channelNvList)[ch].maxDimm) &&
        (freqTablePtr[porTableIndex].numRanks >= maxRanks) &&
        (freqTablePtr[porTableIndex].dimmType == dimmType)) {

      //
      // Found the current configuration in the table. Check to see if there is a DDR frequency available.
      //
      if ((host->setup.mem.enforcePOR == ENFORCE_POR_EN) && (freqTablePtr[porTableIndex].porFreq == NOT_SUP)) {
        // Break if there is no valid POR frequency for this config
        break;
      } else if ((host->setup.mem.enforcePOR == ENFORCE_STRETCH_EN)  && (freqTablePtr[porTableIndex].stretchFreq == NOT_SUP)) {
        // Break if there is no valid stretch frequency for this config
        break;
      }

      //
      // Check if the desired Vdd is supported with this config. Break if yes. Keep looking if no.
      //
      if(freqTablePtr[porTableIndex].vdd & vdd) {
        status = SUCCESS;
        break;
      }
    }
  } // porTableIndex

  return status;
} // CheckPORDDRVdd


/**

  this function writes to CADB

  @param host            - include all the MRC data
  @param socket             - Memory Controller
  @param ch              - ch to send command to
  @param rank            - CS to send the command to

**/
void
ReadMprCADBSeq (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     side,
  UINT8     mprPage,
  UINT8     mprLoc
  )
{
  UINT8                                   mcId;
  UINT8                                   count;
  UINT8                                   i;
  UINT32                                  CRValueSave = 0;
  UINT8                                   bankMR3;
  UINT16                                  dataMR3;
  UINT8                                   bank;
  UINT16                                  data;
  UINT8                                   slot;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  struct ddrRank                          (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                       (*rankStruct)[MAX_RANK_DIMM];
  PatCadbProg0                            patCADBProg0[5];
  PatCadbProg1                            patCADBProg1[5];
  CPGC_PATCADBMRS_MCDDC_CTL_STRUCT        patCADBMrs;
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT          seqCtl0;
  CPGC_PATCADBCTL_MCDDC_CTL_HSX_BDX_STRUCT    cpgcPatCadbCtlReg;
  struct channelNvram                     (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(host, socket);
  dimmNvList = GetDimmNvList(host, socket, ch);
  rankList = GetRankNvList(host, socket, ch, dimm);
  rankStruct  = &host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;
  mcId = GetMCID(host, socket, ch);

  //
  // Pointer will be dynamically incremented after a write to CADB_PROG register
  //
  MemWritePciCfgEp (host, socket, ch, CPGC_PATCADBWRPNTR_MCDDC_CTL_REG, 0);
  //
  // Save before MR command
  //
  CRValueSave = MemReadPciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)));
  seqCtl0.Data                      = 0;
  seqCtl0.Bits.initialization_mode  = MRS_Mode;
  MemWritePciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)), seqCtl0.Data);

  cpgcPatCadbCtlReg.Data = MemReadPciCfgEp (host, socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG);
  cpgcPatCadbCtlReg.Bits.enable_cadb_on_deselect = 0;
  cpgcPatCadbCtlReg.Bits.enable_cadb_always_on = 0;
  cpgcPatCadbCtlReg.Bits.enable_onepasscadb_always_on = 0;
  MemWritePciCfgEp (host, socket, ch, CPGC_PATCADBCTL_MCDDC_CTL_REG, cpgcPatCadbCtlReg.Data);

  // Command 0 - MR3 write to side B if present, or side A if not
  slot = 0;
  dataMR3 = (*rankStruct)[rank].MR3 | MR3_MPR | mprPage;
  bankMR3 = BANK3;
  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (host, dataMR3);
    bankMR3  = MirrorBankBits (host, bankMR3);
  }
  //
  // Invert address/bank bits
  //
  if (host->nvram.mem.dimmTypePresent == RDIMM) {
   dataMR3  = InvertAddrBits (dataMR3);
   bankMR3  = InvertBankBits (bankMR3);
  }

  patCADBProg0[slot].address = dataMR3;
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode)) {
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  //
  // Calculate parity
  //
  count = 0;

  // Count the 1's in CID bits if they are to be included in the parity calculation
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    if (((*channelNvList)[ch].cidBitMap & BIT2) && (patCADBProg0[slot].cid & BIT2))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT1) && (patCADBProg0[slot].cid & BIT1))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT0) && (patCADBProg0[slot].cid & BIT0))
      count++;
  }

  // Count the 1's in the address bits
  for (i = 0; i < 17; i++) {
    if (patCADBProg0[slot].address & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the bank bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg0[slot].bank & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the control bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg1[slot].control & (1 << i)) {
      count++;
    }
  } // i loop

  if (count & BIT0) {
    patCADBProg1[slot].par = 1;
  } else {
    patCADBProg1[slot].par = 0;
  }

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  patCADBProg0[slot].address = dataMR3;
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) &&  ((*channelNvList)[ch].encodedCSMode)) {
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  // Command 1 - MR3 write to side A
  slot = 1;
  dataMR3 = (*rankStruct)[rank].MR3 | MR3_MPR | mprPage;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (host, dataMR3);
    bankMR3  = MirrorBankBits (host, bankMR3);
  }

  patCADBProg0[slot].address = dataMR3;
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode)) {
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  //
  // Calculate parity
  //
  count = 0;

  // Count the 1's in CID bits if they are to be included in the parity calculation
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    if (((*channelNvList)[ch].cidBitMap & BIT2) && (patCADBProg0[slot].cid & BIT2))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT1) && (patCADBProg0[slot].cid & BIT1))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT0) && (patCADBProg0[slot].cid & BIT0))
      count++;
  }

  // Count the 1's in the address bits
  for (i = 0; i < 17; i++) {
    if (patCADBProg0[slot].address & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the bank bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg0[slot].bank & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the control bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg1[slot].control & (1 << i)) {
      count++;
    }
  } // i loop

  if (count & BIT0) {
    patCADBProg1[slot].par = 1;
  } else {
    patCADBProg1[slot].par = 0;
  }

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 2 - RD from given side
  slot = 2;
  data = 0;
  bank = mprLoc;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    data  = MirrorAddrBits (host, data);
    bank  = MirrorBankBits (host, bank);
  }

  //
  // Invert address/bank bits
  //
  if (side == SIDE_B) {
    data  = InvertAddrBits (data);
    bank  = InvertBankBits (bank);
  }

  patCADBProg0[slot].address = data;
  patCADBProg0[slot].bank    = bank;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode)) {
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = RD_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  //
  // Calculate parity
  //
  count = 0;

  // Count the 1's in CID bits if they are to be included in the parity calculation
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    if (((*channelNvList)[ch].cidBitMap & BIT2) && (patCADBProg0[slot].cid & BIT2))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT1) && (patCADBProg0[slot].cid & BIT1))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT0) && (patCADBProg0[slot].cid & BIT0))
      count++;
  }

  // Count the 1's in the address bits
  for (i = 0; i < 17; i++) {
    if (patCADBProg0[slot].address & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the bank bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg0[slot].bank & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the control bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg1[slot].control & (1 << i)) {
      count++;
    }
  } // i loop

  if (count & BIT0) {
    patCADBProg1[slot].par = 1;
  } else {
    patCADBProg1[slot].par = 0;
  }

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 3 - MR3 write to side B if present, or side A if not
  slot = 3;
  dataMR3  = (*rankStruct)[rank].MR3;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (host, dataMR3);
    bankMR3  = MirrorBankBits (host, bankMR3);
  }

  //
  // Invert address/bank bits
  //
  if (host->nvram.mem.dimmTypePresent == RDIMM) {
    dataMR3  = InvertAddrBits (dataMR3);
    bankMR3  = InvertBankBits (bankMR3);
  }

  patCADBProg0[slot].address = dataMR3;
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;

  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  //
  // Calculate parity
  //
  count = 0;

  // Count the 1's in CID bits if they are to be included in the parity calculation
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    if (((*channelNvList)[ch].cidBitMap & BIT2) && (patCADBProg0[slot].cid & BIT2))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT1) && (patCADBProg0[slot].cid & BIT1))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT0) && (patCADBProg0[slot].cid & BIT0))
      count++;
  }

  // Count the 1's in the address bits
  for (i = 0; i < 17; i++) {
    if (patCADBProg0[slot].address & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the bank bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg0[slot].bank & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the control bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg1[slot].control & (1 << i)) {
      count++;
    }
  } // i loop

  if (count & BIT0) {
    patCADBProg1[slot].par = 1;
  } else {
    patCADBProg1[slot].par = 0;
  }

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Command 4 - MR3 write to side A
  slot = 4;
  dataMR3  = (*rankStruct)[rank].MR3;
  bankMR3 = BANK3;

  //
  // Mirror address/bank bits
  //
  if ((rank & BIT0) && (*dimmNvList)[dimm].SPDAddrMapp) {
    dataMR3  = MirrorAddrBits (host, dataMR3);
    bankMR3  = MirrorBankBits (host, bankMR3);
  }

  patCADBProg0[slot].address = dataMR3;
  patCADBProg0[slot].bank    = bankMR3;
  patCADBProg0[slot].pdatag  = NO_PDA;
  patCADBProg0[slot].cid     = 0;
  if ((host->nvram.mem.dramType == SPD_TYPE_DDR4) && ((*channelNvList)[ch].encodedCSMode)) { 
    // here we are in encoded quad CS mode
    patCADBProg0[slot].cid     = (rank >> 1) | 0x6;
  }
  patCADBProg1[slot].cs      = ~(1 << (*rankList)[rank].CSIndex) & ((1 << 11) - 1);
  patCADBProg1[slot].control = MRS_CMD;

  //
  // Assert ACT bit
  //
  patCADBProg1[slot].control |= BIT3;

  //
  // Calculate parity
  //
  count = 0;

  // Count the 1's in CID bits if they are to be included in the parity calculation
  if ((*channelNvList)[ch].encodedCSMode == 2) {
    if (((*channelNvList)[ch].cidBitMap & BIT2) && (patCADBProg0[slot].cid & BIT2))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT1) && (patCADBProg0[slot].cid & BIT1))
      count++;
    if (((*channelNvList)[ch].cidBitMap & BIT0) && (patCADBProg0[slot].cid & BIT0))
      count++;
  }

  // Count the 1's in the address bits
  for (i = 0; i < 17; i++) {
    if (patCADBProg0[slot].address & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the bank bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg0[slot].bank & (1 << i)) {
      count++;
    }
  } // i loop

  // Count the 1's in the control bits
  for (i = 0; i < 4; i++) {
    if (patCADBProg1[slot].control & (1 << i)) {
      count++;
    }
  } // i loop

  if (count & BIT0) {
    patCADBProg1[slot].par = 1;
  } else {
    patCADBProg1[slot].par = 0;
  }

  patCADBProg1[slot].cke = 0x3F;
  patCADBProg1[slot].odt = 0;

  // Program commands into CADB

  ProgramCADB (host, socket, ch, 0, 5, patCADBProg0, patCADBProg1);

  //
  // Execute MRS Mode
  //
  patCADBMrs.Data                     = 0;
  patCADBMrs.Bits.mrs_gap             = 5;
  patCADBMrs.Bits.mrs_start_pointer   = 0;
  patCADBMrs.Bits.mrs_end_pointer     = 4;
  MemWritePciCfgEp (host, socket, ch, CPGC_PATCADBMRS_MCDDC_CTL_REG, patCADBMrs.Data);

  // Start test and clear errors
  seqCtl0.Bits.local_clear_errs  = 1;
  seqCtl0.Bits.local_start_test  = 1;
  MemWritePciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)), seqCtl0.Data);

  //
  // Poll test completion
  //
  CpgcPollGlobalTestDone (host, socket, 1 << ch);

  //
  // Restore after MR command
  //
  seqCtl0.Data = MemReadPciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)));
  seqCtl0.Bits.local_start_test = 0;
  seqCtl0.Bits.local_clear_errs = 1;
  MemWritePciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)), seqCtl0.Data);
  MemWritePciCfgMC (host, socket, mcId, CPGC_SEQCTL0_MC_MAINEXT_REG + (4 * GetMCCh(mcId, ch)), CRValueSave);

} // ReadMprCADBSeq

/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
void
ReadMprRankDdr4 (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     mprPage,
  UINT8     mprLoc,
  UINT8     MprData[MAX_STROBE]
  )
{
  MR_READ_RESULT_DQ12TO0_MCDDC_DP_STRUCT  mrReadResultDq12to0;
  MR_READ_RESULT_DQ28TO16_MCDDC_DP_STRUCT mrReadResultDq28to16;
  MR_READ_RESULT_DQ44TO32_MCDDC_DP_STRUCT mrReadResultDq44to32;
  MR_READ_RESULT_DQ60TO48_MCDDC_DP_STRUCT mrReadResultDq60to48;
  MR_READ_RESULT_DQ68TO64_MCDDC_DP_STRUCT mrReadResultDq68to64;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];


  dimmNvList    = GetDimmNvList(host, socket, ch);
  rankStruct    = GetRankStruct(host, socket, ch, dimm);

  ReadMprCADBSeq (host, socket, ch, dimm, rank, SIDE_A, mprPage, mprLoc);

  // Get side A results
  mrReadResultDq12to0.Data  = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
  mrReadResultDq28to16.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
  mrReadResultDq44to32.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
  mrReadResultDq60to48.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
  mrReadResultDq68to64.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

  MprData[0] = (UINT8)((mrReadResultDq12to0.Data  >> 0)  & 0xFF);
  MprData[1] = (UINT8)((mrReadResultDq12to0.Data  >> 16) & 0xFF);
  MprData[2] = (UINT8)((mrReadResultDq28to16.Data >> 0)  & 0xFF);
  MprData[3] = (UINT8)((mrReadResultDq28to16.Data >> 16) & 0xFF);
  MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0)  & 0xFF);
  MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
  MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0)  & 0xFF);
  MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);
  MprData[8] = (UINT8)((mrReadResultDq68to64.Data >> 0)  & 0xFF);

  if ((*dimmNvList)[dimm].x4Present) {

    MprData[9]  = (UINT8)((mrReadResultDq12to0.Data  >> 8)  & 0xFF);
    MprData[10] = (UINT8)((mrReadResultDq12to0.Data  >> 24) & 0xFF);
    MprData[11] = (UINT8)((mrReadResultDq28to16.Data >> 8)  & 0xFF);
    MprData[12] = (UINT8)((mrReadResultDq28to16.Data >> 24) & 0xFF);
    MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8)  & 0xFF);
    MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
    MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8)  & 0xFF);
    MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);
    MprData[17] = (UINT8)((mrReadResultDq68to64.Data >> 8)  & 0xFF);

  } //x4

  if (host->nvram.mem.dimmTypePresent == RDIMM) {

    ReadMprCADBSeq (host, socket, ch, dimm, rank, SIDE_B, mprPage, mprLoc);

    // Get side B results
    mrReadResultDq12to0.Data  = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
    mrReadResultDq28to16.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
    mrReadResultDq44to32.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
    mrReadResultDq60to48.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
    mrReadResultDq68to64.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

    // Update results
    // Side B discovery of DRAM mapping not supported
    MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0)  & 0xFF);
    MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
    MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0)  & 0xFF);
    MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);

    if ((*dimmNvList)[dimm].x4Present) {

      MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8)  & 0xFF);
      MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
      MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8)  & 0xFF);
      MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);

    } //x4
  } // RDIMM

} // ReadMprRankDdr4

#ifdef LRDIMM_SUPPORT
/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
void
ReadBcwDimmDdr4 (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  )
{
  MR_READ_RESULT_DQ12TO0_MCDDC_DP_STRUCT  mrReadResultDq12to0;
  MR_READ_RESULT_DQ28TO16_MCDDC_DP_STRUCT mrReadResultDq28to16;
  MR_READ_RESULT_DQ44TO32_MCDDC_DP_STRUCT mrReadResultDq44to32;
  MR_READ_RESULT_DQ60TO48_MCDDC_DP_STRUCT mrReadResultDq60to48;
  MR_READ_RESULT_DQ68TO64_MCDDC_DP_STRUCT mrReadResultDq68to64;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8 controlWordData;


  dimmNvList    = GetDimmNvList(host, socket, ch);

  // Enable DB function override BC7x.  Subsequent accesses to function 0 will be redirected according to BC7x[3:0]
  WriteLrbuf(host, socket, ch, dimm, 0, bcwFunc, 0, LRDIMM_BC7x);

  // CW Source selection
  controlWordData = BIT4 | bcwByte;
  WriteRC (host, socket, ch, dimm, 0, controlWordData, RDIMM_RC4x);

  // CW Source selection
  //controlWordData = 0x00;  //auto-increment disable
  //WriteRC (host, socket, ch, dimm, 0, controlWordData, RDIMM_RC5x);

  // CW Source selection
  controlWordData = bcwNibble << 4;
  WriteRC (host, socket, ch, dimm, 0, controlWordData, RDIMM_RC6x);

  // CW Read operation
  controlWordData = 4;    //CMD 4 - CW Read Operation
  WriteRC (host, socket, ch, dimm, 0, controlWordData, RDIMM_RC06);

  ReadMprCADBSeq (host, socket, ch, dimm, 0, SIDE_A, 0, 0);

  // Disable DB function override
  WriteLrbuf(host, socket, ch, dimm, 0, 0, 0, LRDIMM_BC7x);

  // Get side A results
  mrReadResultDq12to0.Data  = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ12TO0_MCDDC_DP_REG);
  mrReadResultDq28to16.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ28TO16_MCDDC_DP_REG);
  mrReadResultDq44to32.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ44TO32_MCDDC_DP_REG);
  mrReadResultDq60to48.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ60TO48_MCDDC_DP_REG);
  mrReadResultDq68to64.Data = MemReadPciCfgEp (host, socket, ch, MR_READ_RESULT_DQ68TO64_MCDDC_DP_REG);

  MprData[0] = (UINT8)((mrReadResultDq12to0.Data  >> 0)  & 0xFF);
  MprData[1] = (UINT8)((mrReadResultDq12to0.Data  >> 16) & 0xFF);
  MprData[2] = (UINT8)((mrReadResultDq28to16.Data >> 0)  & 0xFF);
  MprData[3] = (UINT8)((mrReadResultDq28to16.Data >> 16) & 0xFF);
  MprData[4] = (UINT8)((mrReadResultDq44to32.Data >> 0)  & 0xFF);
  MprData[5] = (UINT8)((mrReadResultDq44to32.Data >> 16) & 0xFF);
  MprData[6] = (UINT8)((mrReadResultDq60to48.Data >> 0)  & 0xFF);
  MprData[7] = (UINT8)((mrReadResultDq60to48.Data >> 16) & 0xFF);
  MprData[8] = (UINT8)((mrReadResultDq68to64.Data >> 0)  & 0xFF);

  if ((*dimmNvList)[dimm].x4Present) {

    MprData[9]  = (UINT8)((mrReadResultDq12to0.Data  >> 8)  & 0xFF);
    MprData[10] = (UINT8)((mrReadResultDq12to0.Data  >> 24) & 0xFF);
    MprData[11] = (UINT8)((mrReadResultDq28to16.Data >> 8)  & 0xFF);
    MprData[12] = (UINT8)((mrReadResultDq28to16.Data >> 24) & 0xFF);
    MprData[13] = (UINT8)((mrReadResultDq44to32.Data >> 8)  & 0xFF);
    MprData[14] = (UINT8)((mrReadResultDq44to32.Data >> 24) & 0xFF);
    MprData[15] = (UINT8)((mrReadResultDq60to48.Data >> 8)  & 0xFF);
    MprData[16] = (UINT8)((mrReadResultDq60to48.Data >> 24) & 0xFF);
    MprData[17] = (UINT8)((mrReadResultDq68to64.Data >> 8)  & 0xFF);

  } //x4

} // ReadBcwRankDdr4
#endif //LRDIMM_SUPPORT


/**

  Returns the memory controller ID

  @param host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number

  @retval MC ID

**/
UINT8
GetMCID (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  )
{
  return host->var.mem.socket[socket].channelList[ch].mcId;
} // GetMCID

/**

  Returns the memory controller ID

  @param mcIdbb  - Memory Controller ID
  @param ch      - Channel number

  @retval MC ID

**/
UINT8
GetMCCh (
  UINT8     mcId,
  UINT8     ch
  )
{
  return (ch & ~(mcId * 2));
} // GetMCCh

//
// Host struct abstraction functions
//

struct imcNvram (*GetImcNvList(
  PSYSHOST host,
  UINT8    socket
  ))[MAX_IMC]
{
  return(&host->nvram.mem.socket[socket].imc);
}

struct channelNvram (*GetChannelNvList (
  PSYSHOST  host,
  UINT8     socket
  ))[MAX_CH]
{
  return (&host->nvram.mem.socket[socket].channelList);
}

struct dimmNvram (*GetDimmNvList (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  ))[MAX_DIMM]
{
  return (&host->nvram.mem.socket[socket].channelList[ch].dimmList);
}

struct ddrRank (*GetRankNvList (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  ))[MAX_RANK_DIMM]
{
  return (&host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList);
}

struct rankDevice (*GetRankStruct (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  ))[MAX_RANK_DIMM]
{
  return (&host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct);
}

/**

    Check to see if Rank for each socket is populated

    @param host - Pointer to SysHost

    @retval N/A

**/
void
CheckRankPop (
  PSYSHOST  host
  )
{
  UINT8 socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    CheckRankPopLocal(host, socket);

  } // socket loop
} // CheckRankPop


/**

    Check to see if Rank for specified socket is populated

    @param host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
void
CheckRankPopLocal (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8                           ch;
  UINT8                           rank;
  UINT8                           dimm;
  UINT32                          dimmAmap;
  UINT8                           chRankEnabled;
  struct dimmNvram                (*dimmNvList)[MAX_DIMM];
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;
  AMAP_MCDDC_DESC_STRUCT          imcAMAPReg;

  if (host->nvram.mem.socket[socket].enabled == 0) return;

  for (ch = 0; ch < MAX_CH; ch++) {

    dimmNvList = GetDimmNvList(host, socket, ch);
    dimmAmap = 0;
    chRankEnabled = 0;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if((*dimmNvList)[dimm].dimmPresent == 0) continue;

      for (rank = 0; rank < (*dimmNvList)[dimm].numDramRanks; rank++) {

        if ((*dimmNvList)[dimm].mapOut[rank] == 0) {
          chRankEnabled++;
        }
      } // rank loop
    } // dimm loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (chRankEnabled == 0) {
        dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4));
      } else {
        dimmMtr.Data = (*dimmNvList)[dimm].dimmMemTech;
      }

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (((*dimmNvList)[dimm].dimmPresent == 0) || (rank >= (*dimmNvList)[dimm].numDramRanks) || (chRankEnabled == 0)) {

          dimmMtr.Bits.rank_disable |= 1 << rank;

          if ((((*dimmNvList)[dimm].dimmPresent) && (*dimmNvList)[dimm].mapOut[rank] == 1) ||
            (((*dimmNvList)[dimm].SPDSecondaryDeviceType & (SPD_QUARTER_SIZE_SECOND_RANK | SPD_HALF_SIZE_SECOND_RANK)) && ((rank) % 2))) {
            // Disable Patrol Scrub on mapped out ranks and non-symmetric ranks
            dimmAmap |= (1 << (dimm * 8 + rank));

            // Update AMAP register
            imcAMAPReg.Data = MemReadPciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG);

            imcAMAPReg.Bits.dimm0_pat_rank_disable |= (UINT8)(dimmAmap & 0xff);
            imcAMAPReg.Bits.dimm1_pat_rank_disable |= (UINT8)((dimmAmap >> 8) & 0xff);
            imcAMAPReg.Bits.dimm2_pat_rank_disable |= (UINT8)((dimmAmap >> 16) & 0xff);
            MemWritePciCfgEp (host, socket, ch, AMAP_MCDDC_DESC_REG,imcAMAPReg.Data);
          }
        }
      } //rank loop

      (*dimmNvList)[dimm].dimmMemTech = dimmMtr.Data;
      MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4), (UINT32) dimmMtr.Data);

      MemDebugPrint((host, SDBG_MAX, socket, ch, dimm, NO_RANK, NO_STROBE, NO_BIT,
                   "dimmMtr: 0x%08x\n", (*dimmNvList)[dimm].dimmMemTech));
    } //dimm loop

    if (chRankEnabled == 0) {
      host->nvram.mem.socket[socket].channelList[ch].enabled = 0;
    }
  } // ch loop
} // CheckRankPopLocal

void
ProgramFnvMprPattern (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT16   pattern
  )
{
  RC_ASSERT(FALSE, ERR_RC_INTERNAL, 0);
}

void
SetRankMprFnv (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
} // SetRankMprFnv

/**

    Get Maximum Frequency

    @param host   - Pointer to SysHost
    @param socket - Current Socket

    @retval maxDdrFreq

**/
UINT8
GetMaxFreq (
  PSYSHOST host,
  UINT8    socket
  )
{
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT capId1[MAX_SOCKET];
  UINT8                          maxDdrFreq;

  //
  //Initialize Capid1 Variable
  //
  capId1[socket].Data = ReadCpuPciCfgEx(host, socket, 0, CAPID1_PCU_FUN3_REG);

  //
  // Get maximum DCLK ratio supported
  //
  if (capId1[socket].Bits.dmfc & BIT3) {
    //
    // No max if overclocking is enabled
    //
    maxDdrFreq = 0xFF;
  } else {
    //
    // Convert fuse value
    //
    switch (capId1[socket].Bits.dmfc & 0x7) {
    case (0x7):
      maxDdrFreq = DDR_1067;
      break;

    case (0x6):
      maxDdrFreq = DDR_1333;
      break;

    case (0x5):
      maxDdrFreq = DDR_1600;
      break;

    case (0x4):
      maxDdrFreq = DDR_1867;
      break;

    case (0x3):
      maxDdrFreq = DDR_2133;
      break;

    case (0x2):
      maxDdrFreq = DDR_2400;
      break;

    case (0x1):
      maxDdrFreq = DDR_2667;
      break;

    case (0x0):
      maxDdrFreq = DDR_2933;
      break;

    default:
      maxDdrFreq = DDR_2133;
      break;
    }
  }

  //
  // HSD 4986525: The maximum setting reported by DMFC is incorrect. Hard code it to 1867.
  //
  if ((CheckSteppingEqual(host, CPU_HSX, B0_REV_HSX)) && (host->nvram.mem.dramType == SPD_TYPE_DDR3)) {
    maxDdrFreq = DDR_1867;
  }

  return maxDdrFreq;
}

/**

    Get the DCLK Ratio

    @param host     - Pointer to SysHost
    @param socket   - Current Socket
    @param req_type - Pointer to req_type
    @param req_data - Pointer to req_data

    @retval N/A

**/
VOID
GetDclkRatio (
  PSYSHOST host,
  UINT8    socket,
  UINT8    *req_type,
  UINT8    *req_data
  )
{
  MC_BIOS_REQ_PCU_FUN1_STRUCT mcBiosReq;

  mcBiosReq.Data = ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);

  *req_type = (UINT8)mcBiosReq.Bits.req_type;
  *req_data = (UINT8)mcBiosReq.Bits.req_data;
}


/**

    Set the DCLK Ratio

    @param host     - Pointer to SysHost
    @param socket   - Current Socket
    @param req_type - req_type
    @param req_data - req_data

    @retval N/A

**/
VOID
SetDclkRatio (
  PSYSHOST host,
  UINT8    socket,
  UINT8    req_type,
  UINT8    req_data
  )
{
  MC_BIOS_REQ_PCU_FUN1_STRUCT       mcBiosReq;
  BIOSSCRATCHPAD1_UBOX_MISC_STRUCT  scratchPad1Reg;
  
  mcBiosReq.Data = ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
    "MRC reset request!\n Current DCLK: %d\n Desired DCLK: %d, req_type = %d\n",
    mcBiosReq.Bits.req_data, req_data, req_type));

  //
  // Set the desired DCLK Ratio
  //

  //
  //5003070: WA to detect if 100 MHz bclk based frequency is supported
  //
  if (req_type == 1) {
    scratchPad1Reg.Data = ReadCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG);
    if ((scratchPad1Reg.Data & (BIT10)) == 0) {
      scratchPad1Reg.Data |= BIT10;
      mcBiosReq.Bits.req_data = req_data;
      mcBiosReq.Bits.req_type = req_type;
    } else if ((scratchPad1Reg.Data & BIT10) != 0) {
      scratchPad1Reg.Data |= BIT11;
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Requested frequency is not supported, Setting frequency to POR frequency\n"));
    }
    WriteCpuPciCfgEx (host, 0, 0, BIOSSCRATCHPAD1_UBOX_MISC_REG, scratchPad1Reg.Data);
  } else {
    mcBiosReq.Bits.req_data = req_data;
    mcBiosReq.Bits.req_type = req_type;  
  }

  //
  // Issue command to the PCU to change the DDR3 frequency
  //
  WriteCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG, mcBiosReq.Data);
}

/**

    Set the DCLK Ratio

    @param host       - Pointer to SysHost
    @param socket     - Current Socket
    @param ddrFreqMHz - DDR bus frequency in MHz

    @retval N/A

**/
VOID
SetRingRatio (
  PSYSHOST  host,
  UINT8     socket,
  UINT16    ddrFreqMHz
  )
{
  UINT32        mailBoxCommand;
  UINT32        ratio;
  UINT64_STRUCT msrValue;

  if ((host->var.common.socketPresentBitMap > 1) && (host->setup.mem.options & MULTI_THREAD_MRC_EN)) return;

  //
  // Get minimum ring ratio required for this memory speed
  //
  ratio = (ddrFreqMHz + 99) / 100;

  //
  // Get min/max ring ratio settings
  //
  msrValue = ReadMsrPipe(host, socket, MSR_UNCORE_FREQ);

  //
  // Program a new ratio if one is needed and the new ratio is higher than the current ratio
  //
  if (((msrValue.lo >> 8) & 0xFF) < ratio) {
    MemDebugPrint((host, SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
      "\n CLR ring current min: %d\n desired min: %d\n",
      ((msrValue.lo >> 8) & 0xFF), ratio));
    mailBoxCommand = (ratio << 8) | (ratio << 16) | MAILBOX_BIOS_CMD_CLR_RING;
    WriteBios2PcuMailboxCommand (host, socket, mailBoxCommand, 0);

    msrValue.lo = ((msrValue.lo & 0x80FF)  | (ratio << 8 ));
    WriteMsrPipe(host, socket, MSR_UNCORE_FREQ, msrValue);
  }

} // SetRingRatio

VOID
SetChannelDisable (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  )
{
} // SetChannelDisable

/**

    Determine if rank is disabled

    @param host   - Pointer to SysHost
    @param socket - Current socket
    @param ch     - Current channel
    @param dimm   - Current dimm
    @param rank   - Current rank

    @retval 0

**/
UINT32
IsRankDisabled (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT16   rank
  )
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;

  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;
  dimmMtr.Data = (*dimmNvList)[dimm].dimmMemTech;

  // Skip ranks that are disabled
  if (dimmMtr.Bits.rank_disable & (1 << rank)) return 1;
  return 0;
}

/**

    Set the DIMM is populated bit

    @param host   - Pointer to SysHost
    @param socket - Current socket
    @param ch     - Current channel
    @param dimm   - Current dimm

    @retval N/A

**/
VOID
SetDimmPop (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
{
  DIMMMTR_0_MCDDC_DESC_BDX_STRUCT dimmMtr;

  //
  // Disable DIMM so no maintenance operations go to it
  //
  dimmMtr.Data = MemReadPciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4));
  dimmMtr.Bits.dimm_pop = 1;
  MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4), dimmMtr.Data);
}

/**

    display processor abbreviation, stepping and socket

    @param host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
VOID
DimmInfoDisplayProc (
  PSYSHOST host,
  UINT8    socket
  )
{
  //
  // display processor abbreviation, stepping and socket
  //
  if ((host->var.common.cpuFamily == CPU_FAMILY_HSX) || (host->var.common.cpuFamily == CPU_FAMILY_BDX)) {
    if (host->var.common.cpuFamily == CPU_FAMILY_HSX) {
      rcPrintf ((host, "HSX ", socket));
    } else {
      rcPrintf ((host, "BDX ", socket));
    }

    switch (host->var.common.stepping) {
    
    case A0_REV_HSX:
      rcPrintf ((host, "A0"));
      break;
    
    case B0_REV_HSX:
      rcPrintf ((host, "B0"));
      break;
    
    case C0_REV_HSX:
      rcPrintf ((host, "C0"));
      break;
    
    default:
      rcPrintf ((host, "Unknown"));
    }
  } else if ((host->var.common.cpuFamily == CPU_FAMILY_BDX_DE)) {
    rcPrintf ((host, "BDX ", socket));

    switch (host->var.common.stepping) {
    
    case A0_REV_BDX:
      rcPrintf ((host, "T0"));
      break;
    
    case B0_REV_BDX:
      rcPrintf ((host, "U0"));
      break;
    
    case C0_REV_BDX:
      rcPrintf ((host, "V0/V1"));
      break;
    
    case D0_REV_BDX:
      rcPrintf ((host, "V2/V3"));
      break;
      
    case Y0_REV_BDX:
      rcPrintf ((host, "Y0/Y1"));
      break;

    case A0_REV_BDX_NS:
      rcPrintf ((host, "NS A0"));
      break;
      
    default:
      rcPrintf ((host, "Unknown"));
    }
  } else {
    rcPrintf ((host, "? ", socket));

    switch (host->var.common.stepping) {
    
    case A0_REV_HSX:
      rcPrintf ((host, "A0"));
      break;
    
    case B0_REV_HSX:
      rcPrintf ((host, "B0"));
      break;
    
    case C0_REV_HSX:
      rcPrintf ((host, "C0"));
      break;
    
    default:
      rcPrintf ((host, "Unknown"));
    }
  }

  if (host->var.common.socketType == SOCKET_EN) {
    rcPrintf ((host, " - EN\n"));
  } else if (host->var.common.socketType == SOCKET_EP) {
    rcPrintf ((host, " - EP\n"));
  } else if (host->var.common.socketType == SOCKET_HEDT) {
    rcPrintf ((host, " - HEDT\n"));
  } else if (host->var.common.socketType == SOCKET_DE) {
    rcPrintf ((host, " - DE\n"));
  }
}

/**

    Get LV Mode

    @param host   - Pointer to SysHost
    @param socket - Current socket

    @retval (UINT8)ddrCRCompCtl0.Bits.lvmode

**/
UINT8
GetLvmode (
  PSYSHOST host,
  UINT8    socket
  )
{
  DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_STRUCT ddrCRCompCtl0;

  ddrCRCompCtl0.Data = MemReadPciCfgEp (host, socket, 0, DDRCRCOMPCTL0_MCIO_DDRIOEXTMCC_REG);
  return (UINT8)ddrCRCompCtl0.Bits.lvmode;
}

#ifdef SERIAL_DBG_MSG
/**

    Display DIMM Info

    @param host   - Pointer to SysHost

    @retval N/A

**/
VOID
DimmInfoDisplayProcFeatures (
  PSYSHOST host
  )
{
  UINT8 socket;
  UINT8 ch;
  struct channelNvram (*channelNvList)[MAX_CH];

  IDLETIME_MCDDC_CTL_STRUCT             idleTime;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT      scrambleConfig;
  SCRUBCTL_MC_MAINEXT_STRUCT            imcSCRUBCTL;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT haSYSDEFEATURE2;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT          mcMtr;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT       dp_chkn;

  idleTime.Data = 0;
  scrambleConfig.Data = 0;
  haSYSDEFEATURE2.Data = 0;
  imcSCRUBCTL.Data = 0;
  mcMtr.Data = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0))  continue;

    channelNvList = &host->nvram.mem.socket[socket].channelList;

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      idleTime.Data = MemReadPciCfgEp (host, socket, ch, IDLETIME_MCDDC_CTL_REG);
      scrambleConfig.Data = MemReadPciCfgEp (host, socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
      break;
    } // ch loop

    haSYSDEFEATURE2.Data = MemReadPciCfgEp (host, socket, 0, HASYSDEFEATURE2_HA_CFG_REG);
    imcSCRUBCTL.Data = MemReadPciCfgMain (host, socket, SCRUBCTL_MC_MAINEXT_REG);
    mcMtr.Data = MemReadPciCfgMain (host, socket, MCMTR_MC_MAIN_REG);
    break;
  } // socket loop

  // ECC Support
  rcPrintf ((host, "ECC Checking"));
  EmptyBlock(host, 8);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (mcMtr.Bits.ecc_en) {
    rcPrintf ((host, "     On"));
    EmptyBlock(host, 5);
  } else {
    rcPrintf ((host, "     Off"));
    EmptyBlock(host, 4);
  }
  rcPrintf ((host, "\n"));

  // CAP Support
  rcPrintf ((host, "CAP Checking"));
  EmptyBlock(host, 8);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  dp_chkn.Data= MemReadPciCfgEp (host, socket, 0, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
  if (dp_chkn.Bits.dis_rdimm_par_chk == 0) {
    rcPrintf ((host, "     On"));
    EmptyBlock(host, 5);
  } else {
    rcPrintf ((host, "     Off"));
    EmptyBlock(host, 4);
  }
  rcPrintf ((host, "\n"));

  // Scrubbing
  rcPrintf ((host, "Patrol/Demand Scrub"));
  EmptyBlock(host, 1);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (imcSCRUBCTL.Bits.scrub_en) {
    rcPrintf ((host, "   On/"));
  } else {
    rcPrintf ((host, "  Off/"));
  }

  if (haSYSDEFEATURE2.Bits.demandsrb) {
    rcPrintf ((host, "On"));
    EmptyBlock(host, 4);
  } else {
    rcPrintf ((host, "Off"));
    EmptyBlock(host, 3);
  }
  rcPrintf ((host, "\n"));

    // RAS Mode
  rcPrintf ((host, "RAS Mode"));
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (host->nvram.mem.RASmode == 0) {
    rcPrintf ((host, "   Indep"));
    EmptyBlock(host, 4);
  } else if (host->nvram.mem.RASmode == CH_MIRROR) {
    rcPrintf ((host, "   Mirror"));
    EmptyBlock(host, 3);
  } else if (host->nvram.mem.RASmode == CH_LOCKSTEP) {
    rcPrintf ((host, "  Lockstep"));
    EmptyBlock(host, 2);
  } else if (host->nvram.mem.RASmode == RK_SPARE) {
    rcPrintf ((host, " Rank Spare"));
    EmptyBlock(host, 1);
  } else if (host->nvram.mem.RASmode == CH_SL) {
    rcPrintf ((host, "    SP/LK"));
    EmptyBlock(host, 3);
  } else if (host->nvram.mem.RASmode == CH_MS) {
    rcPrintf ((host, "    MR/SP"));
    EmptyBlock(host, 3);
  }
  rcPrintf ((host, "\n"));

  // Sparing Per Channel

  // XOVER mode
  rcPrintf ((host, "Xover Mode"));
  EmptyBlock(host, 10);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
    rcPrintf ((host, "     2:2"));
  } else {
    rcPrintf ((host, "     1:1"));
  }
  EmptyBlock(host, 4);
  rcPrintf ((host, "\n"));

  // Paging Policy
  rcPrintf ((host, "Paging Policy"));
  EmptyBlock(host, 7);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (mcMtr.Bits.close_pg) {
    rcPrintf ((host, "   Closed"));
    EmptyBlock(host, 3);
  } else if (idleTime.Bits.adapt_pg_clse) {
    rcPrintf ((host, " Adapt Open"));
    EmptyBlock(host, 1);
  } else {
    rcPrintf ((host, "    Open"));
    EmptyBlock(host, 4);
  }
  rcPrintf ((host, "\n"));


  // Scrambling
  rcPrintf ((host, "Data Scrambling"));
  EmptyBlock(host, 5);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  EmptyBlock(host, 12);
  if (scrambleConfig.Bits.rx_enable) {
    rcPrintf ((host, "     On"));
    EmptyBlock(host, 5);
  } else {
    rcPrintf ((host, "     Off"));
    EmptyBlock(host, 4);
  }
  rcPrintf ((host, "\n"));

  // NUMA


}
#endif //SERIAL_MSG_DBG

/**

    Set tWR for this channel

    @param host   - Pointer to SysHost
    @param socket - Current socket
    @param ch     - Current channel
    @param nWR    - nWR value

    @retval N/A

**/
void
SetChTwr (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     nWR
  )
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT  tCrap;

  tCrap.Data = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
  tCrap.Bits.t_wr = nWR;
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tCrap.Data);
} // SetChTwr

/**
  Get the current timing mode

  @param host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)

  @retval Current timing mode
**/
UINT8
GetCurrentTimingMode (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  )
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT  tCrap;

  tCrap.Data = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
  return (UINT8)tCrap.Bits.cmd_stretch;
} // GetCurrentTimingMode

/**

  Set Timing Mode

  @param host            - Pointer to sysHost
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)
  @param cmdTiming       - Timing value to set

  @retval cmdTiming

**/
UINT8
SetTimingMode (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     cmdTiming
  )
{
  TCRAP_MCDDC_CTL_HSX_BDX_STRUCT  tCrap;

  tCrap.Data = MemReadPciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG);
  tCrap.Bits.cmd_stretch = cmdTiming;
  MemWritePciCfgEp (host, socket, ch, TCRAP_MCDDC_CTL_REG, tCrap.Data);
  return cmdTiming;
} // SetTimingMode

/**

    Set Data Timing

    @param host       - Pointer to sysHost
    @param socket     - Socket Id
    @param ch         - Channel number (0-based)
    @param nCL        - nCL value
    @param nCWL       - nCWL value

    @retval N/A

**/
VOID
SetDataTiming (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     nCL,
  UINT8     nCWL
  )
{
  TCDBP_MCDDC_CTL_HSX_BDX_STRUCT  tcdbp;

  tcdbp.Data = MemReadPciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG);
  tcdbp.Bits.t_cwl = nCWL;
  tcdbp.Bits.t_cl = nCL;
  MemWritePciCfgEp (host, socket, ch, TCDBP_MCDDC_CTL_REG, tcdbp.Data);
} // SetDataTiming

/**

    Set Encoded CS Mode

    @param host     - Pointer to sysHost
    @param socket   - Socket Id
    @param ch       - Channel number (0-based)

    @retval N/A

**/
VOID
SetEncodedCsMode (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  )
{
  UINT8 dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  dimmNvList = &host->nvram.mem.socket[socket].channelList[ch].dimmList;

  if ((*channelNvList)[ch].encodedCSMode) {
    dimmNvList = GetDimmNvList(host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      MemWritePciCfgEp (host, socket, ch, DIMMMTR_0_MCDDC_DESC_REG + (dimm * 4), (UINT32) (*dimmNvList)[dimm].dimmMemTech);
    } //dimm
  } // encodedCSMode
} //SetEncodedCsMode

/**

    Adjust roundtrip for MPR mode

    @param host     - Pointer to sysHost
    @param socket   - Socket Id
    @param ch       - Channel number (0-based)
    @param dimm     - Current dimm
    @param rank     - Current rank
    @param mprMode  - MPR Mode

    @retval None

**/
void
AdjustRoundtripForMpr(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    mprMode
  )
{
}

/**

    Set the configuration before RMT

    @param host     - Pointer to sysHost
    @param socket   - Socket Id

    @retval N/A

**/
VOID
SetConfigBeforeRmt(
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8               ch;

  //
  //Set RfOn = 0 before RMT per sighting #4987510
  //
  for (ch = 0; ch < MAX_CH; ch++) {
    SetRfOn(host, socket, ch, 0);
  } // ch loop
}

/**
  MemoryInitDdr()

  This routine performs the Memory Init on all the DIMMs present on MCs using patrol scrub
  engine and returns the operation result in the supplied MEM_IT_STATUS_STRUC array.

  NOTE: The memory Init is performed in Flat Memory Mode and All2All Memory Model.

  @param  Hhst         INPUT: Ptr to SYSHOST, system host (root) structure
  @param  socket       INPUT: Socket number

  @retval None

**/

VOID
MemoryInitDdr (
  PSYSHOST  host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT32              ddrChEnabled;
  struct channelNvram (*channelNvList)[MAX_CH];

  // Create the Channel Mask
  ddrChEnabled = 0;
  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
#ifdef NVMEM_FEATURE_EN
    if ((host->setup.mem.socket[socket].ddrCh[ch].batterybacked) && (host->var.mem.subBootMode == AdrResume)) continue;
#endif
    ddrChEnabled |= 1 << ch;
  }

  //
  // Step 1: Write all 0's pattern
  //
  CpgcMemTest (host, socket, ddrChEnabled, DDR_CPGC_MEM_TEST_WRITE | DDR_CPGC_MEM_TEST_ALL0);

} // MemoryInitDdr

UINT8
MemPresent (
  PSYSHOST host,
  UINT8    memPresent
)
{
  return memPresent;
} // MemErrorOverride


/**

  For certain DCLK to UCLK settings we need to add bubbles

  @param host    - Pointer to sysHost
  @param socket  - Socket to switch

**/
void
ProgramBgfTable (
    PSYSHOST  host,
    UINT8     socket
    )
{

} //ProgramBgfTable

VOID
ClearRASRegisters (
  PSYSHOST host,
  UINT8    socket
  )
/*++

  Reset decive tagging registers

  @param host   - Pointer to the system host (root) structure
  @param socket - socket id

  @retval N/A

-- */
{
  UINT8                         ch;
  UINT8                         dimm;
  UINT8                         rank;
  UINT8                         logicalRank;
  struct channelNvram           (*channelNvList)[MAX_CH];
  struct dimmNvram              (*dimmNvList)[MAX_DIMM];
  struct ddrRank                (*rankList)[MAX_RANK_DIMM];
  DEVTAG_CNTL_0_MCDDC_DP_STRUCT devTagReg;
  
  channelNvList = GetChannelNvList(host, socket);
  
  if (host->var.mem.previousBootError){
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;
      dimmNvList = GetDimmNvList(host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;
        rankList = GetRankNvList(host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          logicalRank = (*dimmNvList)[dimm].rankList[rank].logicalRank;
          devTagReg.Data = (UINT8) MemReadPciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG + logicalRank);
          if (devTagReg.Bits.en == 1){
            //Reset devtag control register
            devTagReg.Bits.faildevice = 0x3F;
            devTagReg.Bits.en = 0;
            MemWritePciCfgEp (host, socket, ch, DEVTAG_CNTL_0_MCDDC_DP_REG + logicalRank, devTagReg.Data);
          }        
        } // rank loop
      } // dimm loop
    } // ch loop
  } 
  return;
} // clearRASRegisters
