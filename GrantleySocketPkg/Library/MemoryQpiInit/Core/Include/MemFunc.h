//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
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

#ifndef _memfunc_h
#define _memfunc_h
#include <Token.h> // AptioV server override
#include "DataTypes.h"
#include "SysHost.h"
#include "CpuPciAccess.h"
#include "Cpgc.h"
#include "MemProjectSpecific.h"
#include "MemHost.h"
#include "MemFuncChipCommon.h"
#include "PlatformFuncCommon.h"
#include "CoreApi.h"
#include "ChipApi.h"
#include "PlatformApi.h"


// Main flow

// MemMain.c
UINT32 CheckStatus(PSYSHOST host);    // CALLTABLE

// InitMem.c
UINT32 CheckVdd(PSYSHOST host);                   // CALLTABLE
UINT32 InitDdrClocks(PSYSHOST host);              // CALLTABLE
UINT32 CheckDimmRanks(PSYSHOST host);             // CALLTABLE
void   DeassertIoReset ( PSYSHOST host, UINT8 socket, UINT8 channel);     // OEM
#ifdef SSA_FLAG
UINT32 SSAInit(PSYSHOST host);                                            //Calling the BSSA API
#endif  //SSA_FLAG

// MrcSsaServices.c
#ifdef SSA_FLAG
extern void SsaBiosInitialize(PSYSHOST host);                            //Initialize the SsaBiosServices data structures
#endif  //SSA_FLAG

// Memory timing routines
UINT8  GetMCCh(UINT8 mcId, UINT8 ch);                                     // CHIP

// MemPpr.c
UINT32 PostPackageRepairMain (PSYSHOST  host);        // CALLTABLE
UINT32 PostPackageRepair (PSYSHOST  host, UINT32 chBitMask);

// MemJedec.c
#define A15_PARITY     BIT2
#define A17_PARITY     BIT3
#define A_SIDE         0
#define B_SIDE         1
#define REF_STAG_0     0
#define REF_STAG_20    1
#define REF_STAG_30    2
#define REF_STAG_40    3
#define REF_STAG_60    4
#define REF_STAG_80    5
#define REF_STAG_100   6
#define REF_STAG_120   7
UINT32 SetAepTrainingMode (PSYSHOST host, UINT8 socket, UINT8 mode);
UINT32 SetAepTrainingModeSMB (PSYSHOST host, UINT8 socket, UINT8 mode);
void GetDdrtTiming(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 *CASTemp, UINT8 *CWLTemp);
void GetDdrtTimingBufferDelay(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 *tCLAdd, UINT8 *tCWLAdd);
UINT8 isCmdVrefTrainingDone (PSYSHOST host, UINT8 socket);
MRC_STATUS GetSetClkDelayCore (PSYSHOST  host, UINT8 socket, UINT8 ch, UINT8 dimm, GSM_LT DdrLevel, UINT8 clk, UINT8 mode, INT16 *value);
MRC_STATUS GetSetCtlGroupDelayCore (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, GSM_LT level, GSM_GT group, UINT8 mode, INT16 *value, UINT16 *minVal, UINT16 *maxVal);
MRC_STATUS GetSetCmdGroupDelayCore (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, GSM_LT level, GSM_GT group, UINT8 mode, INT16 *value, UINT16 *minVal, UINT16 *maxVal);
MRC_STATUS GetSetCmdVrefCore (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, GSM_LT level, UINT8 mode, INT16 *value);
UINT8 IsBacksideCmdMarginEnabled (PSYSHOST host, UINT8 socket);
UINT8 IsDdr4RegisterRev2 (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void IndirectRCWrite(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 controlWordData, UINT8 controlWordFunc, UINT8 controlWordAddr);
UINT32 BacksideShift(PSYSHOST host, UINT8 socket, INT8 direction);

//
// MemTraining.c
//
UINT32 RdLRDIMMVrefCentering(PSYSHOST host);
UINT32 TxLRDIMMDqCentering(PSYSHOST host);
UINT32 RxLRDIMMDqCentering(PSYSHOST host);

//
// MemPowerTraining.c
//
UINT32 TrainCTLE (PSYSHOST host);
UINT32 TrainMcRon (PSYSHOST host);
UINT32 TrainRttWr (PSYSHOST host);
UINT32 TrainMcOdt (PSYSHOST host);
UINT32 TrainNonTgtOdt(PSYSHOST host);
UINT32 TrainDramRon (PSYSHOST host);
#define LINEAR_SWEEP {{0},{1},{2}.{3}.{4},{5},{6},{7},{8},{9},{10},{11},{12},{13},{14},{15},{16},{17},{18},{19}}    // Power Training definitions
#define LINEAR_ARRAY {0,100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500,1600,1700,1800,1900}

void TrainDDROptParam (PSYSHOST host,UINT8 socket,UINT16 mode,UINT32 patternLength,GSM_GT group[MAX_GROUPS],UINT8 numGroups,UINT8 marginType,INT16 settingList[MAX_ODT_SETTINGS][MAX_PARAMS],UINT16 powerLevel[MAX_SETTINGS],UINT8 numSettings,GSM_GT param[MAX_PARAMS],UINT8 paramType,UINT8 numParams);

void CalcPowerTrend (PSYSHOST host, UINT8 socket, INT16 averageMargins[MAX_GROUPS][MAX_ODT_SETTINGS], UINT16 powerLevel[MAX_SETTINGS], UINT8 slopeFactor, INT16 powerTrendLine[MAX_SETTINGS], UINT8 numSettings, UINT8 numGroups);
INT16 FindOptimalTradeOff(PSYSHOST host,UINT8 socket,INT16 averageMargins[MAX_GROUPS][MAX_ODT_SETTINGS],INT16 powerTrendLine[MAX_SETTINGS],UINT8 numSettings,UINT8 numGroups);

#ifdef RC_SIM_FEEDBACK
extern const UINT8 RCSimTxEqScale[MAX_TX_EQ];                                                           // CORE / CHIP
extern const UINT8 RCSimImodeScale[MAX_IMODE];                                                          // CORE / CHIP
extern const UINT8 RCSimRxCTLEScale[MAX_CTLE];                                                          // CORE / CHIP
extern const UINT8 RCSimDRAMRonScale[MAX_DRAMRON];                                                      // CORE / CHIP
extern const UINT8 RCSimMCODTScale[MAX_MCODT];                                                          // CORE / CHIP
extern const UINT8 RCSimNONTGTODTScale[MAX_NONTGTODT];                                                  // CORE / CHIP
extern const UINT8 RCSimRTTWRScale[MAX_RTTWR];                                                          // CORE / CHIP
extern const UINT8 RCSimMCRonScale[MAX_MCRON];                                                          // CORE / CHIP
#endif //RC_SIM_FEEDBACK

// MemRecEnable.c

// MemODT.c
// ODT programming
#define  RD_TARGET      0
#define  WR_TARGET      1

#ifdef  LRDIMM_SUPPORT
// MemDdr4Lrdimm.c
// DDR4 LRDIMM Backside Training (RX)
UINT32 Ddr4LrdimmBacksideRxPhase (PSYSHOST host);   // CALL TABLE
UINT32 Ddr4LrdimmBacksideRxCycle (PSYSHOST host);   // CALL TABLE
UINT32 Ddr4LrdimmBacksideRxDelay (PSYSHOST host);   // CALL TABLE
// DDR4 LRDIMM Backside Training (TX)
UINT32 Ddr4LrdimmBacksideTxFineWL (PSYSHOST host);  // CALL TABLE
UINT32 Ddr4LrdimmBacksideTxCoarseWL (PSYSHOST host);  // CALL TABLE
UINT32 Ddr4LrdimmBacksideTxDelay (PSYSHOST host);   // CALL TABLE
void InphiPIWA(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void ToggleInternalClocksOnFnv(PSYSHOST host, UINT8 socket, UINT32 chBitmask, UINT8 dimm, UINT8 value);
void ToggleTxOnFnv(PSYSHOST host, UINT8 socket, UINT32 chBitmask, UINT8 dimm, UINT8 value);
#endif // LRDIMM_SUPPORT


// PeLoader.c
void   PlacePeImage(PSYSHOST host, UINT8 *baseAddr);    // DEBUG

#ifdef SERIAL_DBG_MSG
// PerfTracker.c
UINT32 PrintAllStats(PSYSHOST host);                                                        // CALL TABLE
#endif // SERIAL_DBG_MSG


#ifdef   XMP_SUPPORT
// MemXMP.c
#endif // XMP_SUPPORT

// MemDisplay.c
#define NO_SOCKET       0xFF
#define NO_CH           0xFF
#define NO_DIMM         0xFF
#define NO_RANK         0xFF
#define NO_STROBE       0xFF
#define NO_BIT          0xFF
#ifdef SERIAL_DBG_MSG
#define SINGLE_LINE     0
#define DOUBLE_LINE     1
#define NOCRLF_FLAG     0
#define CRLF_FLAG       1
UINT32 DIMMInfo (PSYSHOST host);                                                                            // CALLTABLE
UINT32 PrintDimmConfig(PSYSHOST host);                                                                       // CALLTABLE
void   EarlyVideoInit(void);                                                                                // CHIP
UINT32 DisplayTrainResults (PSYSHOST host);                                                                 // CALLTAB
#endif // SERIAL_DBG_MSG
UINT32 TrainingResultOffsetCfg (PSYSHOST host);


#ifdef  BDAT_SUPPORT
// MemBdata.c
UINT32 FillBDATStructure(PSYSHOST host);                      // CHIP / CORE
#endif  //  BDAT_SUPPORT

// MemSpdSimInit.c
UINT32 MemSpdbinRead (UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 *spdbin, UINT32 *size);         // CHIP

//
// MemRdDqDqs.c
//
UINT32 ReadDqDqsPerBit (PSYSHOST host);                     // CALLTABLE

// MemCmdClk.c
UINT32 DeNormalizeCCC(PSYSHOST host, UINT8 socket, INT8 direction);
UINT32 WLPushOut(PSYSHOST  host, UINT8 socket);
void ToggleBcom(PSYSHOST host,UINT8 socket, UINT8 value);

// Timing
// MemTiming.c
void   TxRoundtripAdjust (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 prevTimingMode, UINT8 newTimingMode); // CHIP
void   UpdateTrcwr (PSYSHOST host, UINT8 socket, UINT8 ch);

// CPGC
// MemCpgc.c
void   SetBwErrorMask(PSYSHOST host, UINT8 socket, UINT8 dimm, UINT8 rank, struct bitMask *mask);   // CHIP


//
// ***********************       CORE to CHIP API Prototypes      ******************
//

//
// Functions which handle register accesses and other project-specific details but have the
// same arguments/return values across all platforms.
//
// InitMemChip.c
void   ResetSystemEx(PSYSHOST host, UINT8 resetType);                       // CORE
UINT8  GetPhyRank(UINT8 dimm, UINT8 rank);                                // CORE / CHIP
struct imcNvram (*GetImcNvList (PSYSHOST host, UINT8 socket))[MAX_IMC];                                 // CORE / CHIP
struct channelNvram (*GetChannelNvList (PSYSHOST host, UINT8 socket))[MAX_CH];                          // CORE / CHIP
struct dimmNvram (*GetDimmNvList (PSYSHOST host, UINT8 socket, UINT8 ch))[MAX_DIMM];                    // CORE / CHIP
struct ddrRank (*GetRankNvList (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm))[MAX_RANK_DIMM];     // CORE / CHIP
struct rankDevice (*GetRankStruct (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm))[MAX_RANK_DIMM];  // CORE / CHIP
VOID   SetDclkRatio (PSYSHOST host, UINT8 socket, UINT8 req_type, UINT8 req_data);                            // CORE
VOID   SetChannelDisable (PSYSHOST host,UINT8 socket, UINT8 ch);
void   CheckTwrMargins (PSYSHOST  host, UINT8 socket, UINT8 ch, UINT8 tWR, UINT8 *DisCh);                     // CORE
UINT8  GetVocVrefFactor (PSYSHOST host);                                                                      // CORE
UINT32 SetMaxDelta (PSYSHOST host, UINT32 maxAllowedDelta);                                                   // CORE
void   NormalizeLockstepTiming (PSYSHOST host, UINT8 socket);                                                 // CORE
UINT8  GetCurrentTimingMode (PSYSHOST host, UINT8 socket, UINT8 ch);                                          // CORE / CHIP

//
// MemStartChip.c
//

// MemSPDChip.c
struct dimmDevice (*GetChDimmList(PSYSHOST host, UINT8 socket, UINT8 ch))[MAX_DIMM];                    // CORE

// Memory Commands
// MemIOControl.c
void   IODdrtReset(PSYSHOST host, UINT8 socket, UINT8 dimm);                                      // CORE / CHIP
BOOLEAN CheckCTLLoopback(PSYSHOST host);
UINT8 ChipGetParityErrorStatus(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void ChipSetCTLLoopback(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);

// MemRAS.c
UINT32 RASCheckDimmRanks(struct sysHost *host);                                 // CORE

//
// MemTrainingChip.c
//
extern const INT16 RCSimSenseAmp [MAX_STROBE][4];                               // CORE / CHIP
UINT8  FindIndexResData (UINT8 strobe);                                         // CORE
UINT32 TrainMcRon (PSYSHOST host);
UINT32 TrainRttWr (PSYSHOST host);
UINT32 TrainDramRon (PSYSHOST host);


// Timing
// MemTimingChip.c
// CORE API for SKX, HSX only
UINT8  Check2xRefreshWA(PSYSHOST host, UINT8 socket, UINT8 ch);                             // CORE

// MemCpg.c
#ifdef LRDIMM_SUPPORT
void   WritePBA (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 *controlWordDataPtr,
              UINT8 controlWordFUnc, UINT8 controlWordAddr, UINT8 bufferAddr);                                            // CORE
#endif // LRDIMM_SUPPORT

// MemPciAccess.c
#define PARAM_UNUSED  0xFF // Used in place of ChIdOrBoxInst in MemRead/WritePciCfg for MC functional blocks
                           // that have only 1 instance per MC

// MemDisplayChip.c
#ifdef SERIAL_DBG_MSG
void   DisplayTcothpData(PSYSHOST host, UINT8 socket, UINT8 ch, GSM_GT group);                              // CORE
void   DisplayXoverResults(PSYSHOST  host, UINT8     socket);                                               // CORE
#endif // SERIAL_DBG_MSG

//
// MemProjectSpecfic.c
//
UINT32 GetNonStickyScratchpad(PSYSHOST host, UINT8 socket, UINT8 scratchpad);
void   SetNonStickyScratchpad(PSYSHOST host, UINT8 socket, UINT8 scratchpad, UINT32 data);
void   ClearEridLfsrErr (PSYSHOST host, UINT8 socket, UINT8 ch);
void   SetNvmGnt2Erid (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 data);
UINT16 GetDdrCrDdrt (PSYSHOST host, UINT8 socket, UINT8 ch);
UINT8  GetDdrtTrainResult(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void   GetEridCombineResult (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 *eridCombinedResult);
void   SetEridEnable (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 data);
void   SetupCoerseEridTraining (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void   DisableEridLfsrMode (PSYSHOST host, UINT8 socket, UINT8 ch);
void   SetupCpgcWdbBuffErid (PSYSHOST host, UINT8 socket, UINT8 ch);
void   CollectTestResultsErid (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT16 offset, UINT8 index, UINT16 eridOffset, UINT8 *gnt2eridPassed);
void   RPQDrain(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);
void   DisableEridReturn(PSYSHOST host, UINT8 socket);
//MemTrainingChip.c have helper functions landed here
void   DisableAlForRank(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank); // knl
void   DisplayCCCResults (PSYSHOST host, UINT8 socket);
#ifdef SERIAL_DBG_MSG
void   DisplayPowerTraingResults(PSYSHOST host, UINT8 socket, GSM_GT group);
void   DisplayCMDVrefResults(PSYSHOST host, UINT8 socket);
void   DisplaySenseAmpTrainingResults(PSYSHOST host, UINT8 socket);
void   DisplayResultsChip (PSYSHOST host, UINT8 socket);
#endif // SERIAL_DBG_MSG
#ifdef LRDIMM_SUPPORT
#endif // LRDIMM_SUPPORT
UINT8  GetImodeSettingsList(PSYSHOST host, UINT8 socket, INT16 settingList[MAX_ODT_SETTINGS][MAX_PARAMS]);
UINT8  GetChipAdjForRTL(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
void   ProgramNoDataTrnEn(PSYSHOST host, UINT8 socket, UINT8 value);
UINT8  CaVrefSelect (PSYSHOST host);
// MemWrLeveling.c helper functions
void   SetChipSimRisingEdge(UINT16 risingEdge[MAX_CH][MAX_STROBE], UINT8 ch, UINT8 strobe);
// MemCmdClk.c helper functions
void   ChipSimEnableNmlCMDTrng(PSYSHOST host, UINT8 socket);
BOOLEAN ShouldChipEnableRxVrefTuning(PSYSHOST host);
VOID RxVrefTuning (PSYSHOST host, UINT8 socket, GSM_GT group);
void   ReadDqDqsCleanup(PSYSHOST host, UINT8 socket);
#ifdef RC_SIM_FEEDBACK
void   ChipCtlClkFeedBack (PSYSHOST host, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 sign, INT16 *ctlMargins);
#endif
void   DeassertIoResetEx ( PSYSHOST host, UINT8 socket, UINT8 channel);

//
// ***********************     CORE only Prototypes     ******************
//

// Main flow

// MemMain.c
#ifndef ASM_INC
UINT32 MemMain(PSYSHOST host, CallTableEntryType *ctptrOrg, UINT8 callTableEntries); // CORE
#endif // !ASM_INC

// InitMem.c
void   SetPatternBuffers(PSYSHOST host, UINT8 socket, UINT16 mode, UINT16 loopNum); // CORE
UINT32 GetChBitmaskLrdimm(PSYSHOST host, UINT8 socket, UINT8 dimm, UINT8 rank);     // CORE
void CheckAndProgramDqSwizzle(PSYSHOST host, UINT8 socket);

// Memory timing routines
void   DisableDIMM(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);              // CORE Only
void   DisableRank(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank);  // CORE
void   CheckMappedOut (PSYSHOST  host, UINT8 socket, UINT8 dimm);                   // CORE

// MemSPDDDR4.c
void   GetCommonDDR4DIMMConfig(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);    // CORE

// MemJedec.c
UINT32 TwrScaling (PSYSHOST host);                                                                 // CORE
void   SetAllRankMPR (PSYSHOST host, UINT8 socket, UINT8 mprMode);                                                         // CORE
void   SetRankMPRPattern (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT16 mprMode, UINT16 pattern);  // CORE
void   SetRankMPRPatternDA (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT16 mprMode, UINT16 pattern);  // CORE
void   IssueDllReset(PSYSHOST host, UINT8 socket);                                                  // CORE

// MemMargins.c
#define SET_ALL   0xFF

// MemRecEnable.c
#define   GNT2ERID_START  17
void   EvaluatePiSettingResults(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe,
                              UINT16 risingEdge[MAX_CH][MAX_STROBE], UINT16 centerPoint[MAX_CH][MAX_STROBE],
                              UINT16 fallingEdge[MAX_CH][MAX_STROBE], UINT16 pulseWidth[MAX_CH][MAX_STROBE], UINT8 step); // CORE
void   SetupRecEnFineTest (PSYSHOST host, UINT8 socket, UINT32 chBitmask);                                                // CORE
#ifdef   SERIAL_DBG_MSG
void   PrintSampleArray (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 ReadMode);                              // CORE
void   DisplayEdges (PSYSHOST host, UINT8 socket, UINT8 ch, UINT16 pulseWidth[MAX_STROBE], UINT16 fallingEdge[MAX_STROBE],
                   UINT16 risingEdge[MAX_STROBE], UINT16 centerPoint[MAX_STROBE], UINT8 maxStrobe);                       // CORE
void   DisplayLrdimmMrdEdges (PSYSHOST host, UINT8 socket, UINT8 ch, UINT16 pulseWidth[MAX_STROBE], UINT16 fallingEdge[MAX_STROBE],
                   UINT16 risingEdge[MAX_STROBE], UINT16 centerPoint[MAX_STROBE], UINT8 maxStrobe);                       // CORE
#endif // SERIAL_DBG_MSG

// MemWrLeveling.c
void   OverrideRttNom(PSYSHOST host, UINT8 socket, UINT8 dimm, UINT8 rank);     // CORE
void   RestoreRttNom(PSYSHOST host, UINT8 socket, UINT8 dimm, UINT8 rank);      // CORE

// MemODT.c
// ODT programming
struct odtActStruct *LookupOdtAct(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank); // CORE

#ifdef  DEBUG_PERFORMANCE_STATS
#ifdef SERIAL_DBG_MSG
// PerfTracker.c
void   SetMemPhaseCP(PSYSHOST host, UINT32 majorCheckPoint, UINT32 minorCheckPoint);        // CORE
void   ClearMemPhaseCP(PSYSHOST host, UINT32 majorCheckPoint, UINT32 minorCheckPoint);      // CORE
#endif // SERIAL_DBG_MSG
#endif // DEBUG_PERFORMANCE_STATS

// MemDisplay.c
#ifdef SERIAL_DBG_MSG
void   DisplayDDRFreq (PSYSHOST host, UINT8 socket);                                          // CORE
#endif // SERIAL_DBG_MSG
#ifdef SERIAL_DBG_MSG
void   EyeDiagram(PSYSHOST host, UINT8 socket, GSM_LT level, GSM_GT group, UINT16 mode, UINT8 scope, UINT32 patternLength,
                UINT8 chMask, UINT8 rankMask, UINT8 dataMask[MAX_STROBE/2]);                              // CORE
#endif // SERIAL_DBG_MSG
#ifdef SERIAL_DBG_MSG
void   DisplayBwSerr(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 rank, UINT32 bwSerr[3], INT16 margin);  // CORE
void   DisplayResults(PSYSHOST host, UINT8 socket, GSM_GT group);                                         // CORE
#ifdef LRDIMM_SUPPORT
void   DisplayDdr4LrdimmTrainingResults(PSYSHOST host, UINT8 socket, GSM_GT group);
#endif  
#endif // SERIAL_DBG_MSG                                                                                                            

UINT8  BCDtoDecimal (UINT8 BcdValue);                                                                     // CORE

#ifdef  BDAT_SUPPORT
// MemBdata.c
void CopyBDATPerBitMargins(PSYSHOST host);
void CopyPerBitMargins(PSYSHOST host, UINT8 socket, struct bitMargin *resultsBit, GSM_GT group);
#endif  //  BDAT_SUPPORT

// MemWDB.c
void   WriteTemporalPattern( PSYSHOST host, UINT8 socket, UINT32 temporalPattern, UINT8 numCachelines);                         // CORE

#ifdef RC_SIM
// MemSpdSimInit.c
UINT32 MemIniFileProcessing (PSYSHOST host, FILE *iniFile);                                     // CORE
#endif //RC_SIM

//
// MemRdDqDqs.c
//
#ifdef   SERIAL_DBG_MSG
void   DqDqsDebugDataSummarization(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank,
  TErrorResult *ErrorResult, UINT8 PiMaxNumber, UINT8 ReadMode, UINT8 AggressiveMode);   // CORE only
void   DisplayErrorStartEnd(PSYSHOST host, UINT8 socket, UINT8 ch, UINT16 piPosition[MAX_STROBE],
  UINT16 errorStart[MAX_STROBE], UINT16 errorEnd[MAX_STROBE]);  // CORE
#endif // SERIAL_DBG_MSG

// MemCmdClk.c


//#ifndef SIM_BUILD
//
// Decompress.c
//
//#endif //SIM_BUILD

//
// MemTraining.c
//
#ifdef RC_SIM_FEEDBACK
#ifdef RC_SIM_EYE_DATA_FEEDBACK
void RCSIMFeedbackResults (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 scope, GSM_GT group,
        INT16 previousMargin, UINT32 bwSerr[MAX_CH][3]);
#else
void RCSIMFeedbackResults (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 rank, UINT8 scope, GSM_GT group, INT16 previousMargin, UINT32 bwSerr[MAX_CH][3]);
#endif
#endif
UINT8  EvaluateFaultyParts(PSYSHOST host, UINT8 socket, UINT8 ch);                                        // CORE
void   ClearCmdAddrParityError(PSYSHOST host, UINT8 socket, UINT8 ch);                                    // CORE
UINT32 CheckCmdAddrParityError(PSYSHOST host, UINT8 socket, UINT8 ch);                                    // CORE
void   GetMultiVref(PSYSHOST host, UINT8 socket, GSM_LT level, GSM_GT group, UINT16 mode, UINT8 scope, struct bitMask  *filter, void *results,
         UINT32 patternLength, UINT8 chMask, UINT8 rankMask, UINT8 stepSize, UINT8 numPoints);   // CORE
void   EvaluateStrobeStatus (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bitMask,
     UINT32 status, INT16 margin, UINT8 sign, UINT8 searchUp, UINT8 foundPass[MAX_CH][MAX_RANK_CH][MAX_STROBE/2],
     UINT8 foundFail[MAX_CH][MAX_RANK_CH][MAX_STROBE/2], void *marginData, UINT8 scope, UINT16 mode);     // CORE only

// Emulation Polling Loops
#define LabelPrint(){                                                         \
if (host->var.common.CurrentReset == POST_RESET_POWERGOOD)                    \
  rcPrintf ((host, "\nLABEL_c_mbox%d:\n", host->var.common.LoopBackNumber));    \
else                                                                          \
  rcPrintf ((host, "\nLABEL_w_mbox%d:\n", host->var.common.LoopBackNumber));    \
}

#define MaskPrint(tempLabelCount) {                                           \
rcPrintf ((host, "and eax, %8xh\n", mask));                                   \
if (host->var.common.CurrentReset == POST_RESET_POWERGOOD)                    \
  rcPrintf ((host, "jz LABEL_c_mbox%d\n", tempLabelCount));                 \
else                                                                          \
  rcPrintf ((host, "jz LABEL_w_mbox%d\n", tempLabelCount));                 \
}

#define ReadByteAndShift { \
  rcPrintf ((host, "mov bl, BYTE PTR ds:[%8xh]\n", RegAddr)); \
  rcPrintf ((host, "shl ecx, 8\n")); \
  rcPrintf ((host, "or cl, bl\n")); \
}

void EmulationPoll (PSYSHOST host, UINT8 socket, UINT8 BoxInst, UINT32 Offset, UINT32 mask, UINT8 logic);

// Chip/OEM API via Core
VOID OemInitializePlatformDataCore (PSYSHOST host);
VOID OemPostUncoreInitCore (PSYSHOST host, UINT32 UncoreStatus);
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  //  Call ME BIOS hook before triggering a reset
  //
VOID MEBIOSCheckAndHandleResetRequestsCore(PSYSHOST host);
#endif // ME_SUPPORT_FLAG
void InphiPIWA(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm);
#endif // _memfunc_h
