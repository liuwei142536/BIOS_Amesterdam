/**

Copyright (c) 2009-2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file Cpgc.h

  Memory Initialization Module.

**/

#ifndef _CPGC_H
#define _CPGC_H

#define UNSUPPORT           0
#define SUPPORT             1

//
// general  macros
//
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX1(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x)  (((x) < 0) ? (-(x)) : (x))
#endif

typedef struct {
  UINT16 Start[4];                   // (4, uint16)    // Rank, Bank, Row, Col
  UINT16 Stop[4];                    // (4, uint16)    // Rank, Bank, Row, Col
  UINT8  Order[4];                   // [4, uint8)     // Rank, Bank, Row, Col
  UINT32 IncRate[4];                 // (4, unit32)    // Rank, Bank, Row, Col
  UINT16 IncVal[4];                  // (4, unit16)    // Rank, Bank, Row, Col
} TCPGCAddress;

typedef struct {
  UINT16 IncRate;                    // How quickly the WDB walks through cachelines (uint16)
  UINT32 Start;                      // Starting pointer in WDB
  UINT32 Stop;                       // Stopping pointer in WDB
  UINT8  DQPat;                      // [0:BasicVA, 1:SegmentWDB, 2:CADB, 3:TurnAround, 4:LMNVa]
} TWDBPattern;

typedef enum {
  BasicVA = 0,                    // Use 2 LFSR VicAggressor pattern with rotation of 10 bits
  SegmentWDB,                     // Use 2 LFSR VA pattern 10 bit rotation + 3 LFSR VA pattern 6 bit rotation
  CADB,                           // Do CADB on command/address bus and LMN VA on DQ (power supply noise)
  TurnAround,
  LMNVa,                          // Use (LMN aggressor + LFSR Victim) with rotation of 10 bits
  TurnAroundWR,                   // Run 8 tests, one for each subsequence with RankIncRate = 1/2/2/2/2/2/2/1
  TurnAroundODT                   // Run 4 tests, one for each subsequence with RankIncRate = 1/2/1/2
} TDqPat;

typedef enum {
  NSOE = 0,                       // Never Stop On Any Error
  NTHSOE,                         // Stop on the Nth Any Lane Error
  ABGSOE,                         // Stop on All Byte Groups Error
  ALSOE                           // Stop on All Lanes Error
} TStopOnError;

//
// CADB commands
//
#define MRS_CMD                             0
#define REF_CMD                             1
#define PRE_CMD                             2
#define ACT_CMD                             3
#define WR_CMD                              4
#define RD_CMD                              5
#define ZQ_CMD                              6
#define NOP_CMD                             7
#define RCW_CMD                             8
#define RSVD_CMD                            0xff

//
// CPGC CmdPattern
//
#define PatWrRd                             0
#define PatWr                               1
#define PatRd                               2
#define PatRdWrTA                           3
#define PatWrRdTA                           4
#define PatODTTA                            5
#define DimmTest                            6
#define PatRdWrRd                           7

//
// CPGC Stop On error - SOE
//
#define StpNever                            0
#define StpAnyLane                          1
#define StpAllByteGroups                    2
#define StpAllLanes                         3

//
// CADB Modes
//
#define CADB_MODE_SYNC                      0 // Sequencer and CAS command Driven: Same as Global Deselect though in this
                                              // mode determinism is enforced as now CADB valid deselect patterns will depend on
                                              // the values programmed on Lane_Deselect_Enable and CMD_Deselect_Enable thus
                                              // allowing Deselect to synchronize with various CMD events and Subsequence control bits.
#define CADB_MODE_ON_DESLECT                1 // Global Deselect: Pattern stored in the command/address buffers will be
                                              // driven on the bus during deselect cycles only
#define CADB_MODE_ALWAYS_ON                 2 // Always On: Pattern stored in the command/address buffers will be driven on
                                              // all command and address signals on every DCLK cycle only
//
// CPGC Subsequence types     (defined by CPGC_SUBSEQx_CTL_A_MCDDC_CTL_STRUCT.subsequence_type)
//
#define BRd                                 0
#define BWr                                 1
#define BRdWr                               2
#define BWrRd                               3
#define ORd                                 4
#define OWr                                 5

//
// CPGC Subsequence transaction sizes    (defined by CPGC_SUBSEQ0_CTL_B_MCDDC_CTL_STRUCT.request_data_size)
//
#define BURST_SIZE_32B  0
#define BURST_SIZE_64B  1

//
// CPGC Mux Control
//
#define LMNMode                             0
#define BTBUFFER                            1
#define LFSRMode                            2

//
// DQ time centering param: read or write
//
//
// Margin params
//

/*
 1D Margin Types:
  RcvEn:  Shifts just RcvEn.  Only side effect is it may eat into read dq-dqs for first bit of burst
  RdT:    Shifts read DQS timing, changing where DQ is sampled
  WrT:    Shifts write DQ timing, margining DQ-DQS timing
  WrDqsT: Shifts write DQS timing, margining both DQ-DQS and DQS-CLK timing
  RdV:    Shifts read Vref voltage for DQ only
  WrV:    Shifts write Vref voltage for DQ only
  WrLevel: Shifts write DQ and DQS timing, margining only DQS-CLK timing
  WrTBit:  Shifts write DQ per bit timing.
  RdTBit:  Shifts read DQ per bit timing.
  RdVBit:  Shifts read DQ per bit voltage.

 2D Margin Types (Voltage, Time)
  RdFan2:  Margins both RdV and RdT at { (off, -2/3*off), (off, 2/3*off) }
  WrFan2:  Margins both WrV and WrT at { (off, -2/3*off), (off, 2/3*off) }
  RdFan3:  argins both RdV and RdT at { (off, -2/3*off),  (5/4*off, 0), (off, 2/3*off)  }
  WrFan3:  Margins both WrV and WrT at { (off, -2/3*off), (5/4*off, 0), (off, 2/3*off)  }
*/

#define RcvEna                              0
#define RdT                                 1
#define WrT                                 2
#define WrDqsT                              3
#define RdV                                 4
#define WrV                                 5
#define WrLevel                             6

#define WrTBit                              8
#define RdTBit                              9
#define RdVBit                              10

#define RdFan2                              12
#define WrFan2                              13
#define RdFan3                              14
#define WrFan3                              15

#define DDR_CPGC_DQ_PATTERN         0
#define DDR_CPGC_ADV_CMD_PATTERN    1
#define DDR_CPGC_CMD_G2_PATTERN     2
#define DDR_CPGC_CMD_G3MPR_PATTERN  3
#define DDR_CPGC_MEM_TEST_PATTERN   4
#define DDR_CPGC_PATTERN_MASK       (BIT0 | BIT1 | BIT2 | BIT3)
#define DDR_CPGC_MEM_TEST_READ      BIT8
#define DDR_CPGC_MEM_TEST_WRITE     BIT9
#define DDR_CPGC_MEM_TEST_INVERTPAT BIT10
#define DDR_CPGC_MEM_TEST_ALL0      BIT11
#define DDR_CPGC_MEM_TEST_A5        BIT12
#define DDR_CPGC_PPR_TEST           BIT13

typedef
struct CpgcErrorStatus {
  UINT32 cpgcErrDat0S;
  UINT32 cpgcErrDat1S;
  UINT32 cpgcErrDat2S;
  UINT32 cpgcErrDat3S;
  UINT32 cpgcErrEccS;
} CPGC_ERROR_STATUS;

typedef
struct CpgcGlobalTrainingSetup {
  UINT8     startDelay;
  UINT8     addrUpdRateMode;
  UINT8     seqLoopCnt;
  UINT8     rwMode;
  UINT8     numCacheLines;
  UINT8     burstSeparation;
  UINT8     errEnChunk;
  UINT8     errEnCl;
  UINT8     stopOnErrCtl;
  UINT8     subseqEndPtr;
  UINT8     useSubSeq1;
  UINT8     cpgcLinkTrain;
  UINT8     back2backWrEn;
  UINT8     mprControl;
  UINT8     refMask;
  UINT8     zqMask;
  UINT8     x8Mode;
} CPGC_GLOBAL_TRAINING_SETUP;

//
// CPGC configuration training defines
//
#define CPGC_UPDATE_BY_CL        0 // 0 means that the four Address Update Rate fields
                                   // (Rank, Bank, Row, and Column) are updated based on the # of read and
                                   // write cacheline transactions.

#define CPGC_UPDATE_BY_LOOP_CNT  1 // means that the four Address Update Rate fields
                                   // (Rank, Bank, Row, and Column) are updated by based on the Loopcount
                                   // (# of sequences perfromed).

#define CPGC_BASE_READ_SUBSEQ          0 // 0000: Base read
#define CPGC_BASE_WRITE_SUBSEQ         1 // 0001: Base Write
#define CPGC_BASE_RW_SUBSEQ            2 // 0010: Base Read Write
#define CPGC_BASE_WR_SUBSEQ            3 // 0011: Base Write Read
#define CPGC_BASE_OFFSET_READ_SUBSEQ   4 // 0100: Offset Read
#define CPGC_BASE_OFFSET_WRITE_SUBSEQ  5 // 0101: Offset Write
#define CPGC_SUBSEQ_WAIT_DEFAULT       4 // subseq wait to allow credits to drain

#define CPGC_NO_SUBSEQ1                0xFF  // do not use subseq1

//
// WDB control
//
#define CPGC_WDB_MUX_CTL_LMN                0           // LMN counter
#define CPGC_WDB_MUX_CTL_PATBUF             1           // pattern buffer
#define CPGC_WDB_MUX_CTL_LFSR24             2           // LFSR24
#define CPGC_WDB_MUX_CTL_LFSR24_PATBUF      3           // LFSR + Pattern Buffer

#define CPGC0_LFSR0_INIT_SEED_HSX  0x00FFFF00
#define CPGC0_LFSR1_INIT_SEED_HSX  0x0000FFFF
#define CPGC0_LFSR2_INIT_SEED_HSX  0x00000000
#define CPGC1_LFSR0_INIT_SEED_HSX  0x00555000
#define CPGC1_LFSR1_INIT_SEED_HSX  0x00000555
#define CPGC1_LFSR2_INIT_SEED_HSX  0x00000000
#define CPGC_LFSR0_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR0_INIT_SEED_HSX:CPGC1_LFSR0_INIT_SEED_HSX)
#define CPGC_LFSR1_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR1_INIT_SEED_HSX:CPGC1_LFSR1_INIT_SEED_HSX)
#define CPGC_LFSR2_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR2_INIT_SEED_HSX:CPGC1_LFSR2_INIT_SEED_HSX)

//
// Data Pattern Configuration
//
typedef union {
  struct {
    UINT32   seqMode0: 2;
    UINT32   seqMode1: 2;
    UINT32   seqMode2: 2;
    UINT32   eccDis: 1;
    UINT32   eccReplaceByteControl: 1;
    UINT32   eccDataSourceSel: 1;
    UINT32   saveLfsrSeedRate: 5;
    UINT32   reloadLfsrSeedRate: 8;
    UINT32   rsvd: 10;
  } Bits;
  UINT32  Data;
} SequencerMode;

//
// MRC common functions
//
void
SetupCADB(
  PSYSHOST host,
  UINT8 socket,
  UINT8 ch,
  UINT8 VicSpread,
  UINT8 VicBit,
  UINT8 LMNE
  );

void
SetupIOTest(
  PSYSHOST host,
  UINT8  socket,
  UINT32 ChbitMask,
  UINT8  CmdPat,
  UINT16 NumCL,
  UINT8  LC,
  TCPGCAddress *CPGCAddress,
  UINT8  SOE,
  TWDBPattern  *WDBPattern,
  UINT8  EnCADB,
  UINT8  EnCKE,
  UINT16 SubSeqWait
  );

void
SaveIOTestState(
  PSYSHOST host,
  UINT8  socket,
  UINT32 ChbitMask
  );

void
RestoreIOTestState(
  PSYSHOST host,
  UINT8  socket,
  UINT32 ChbitMask
  );

UINT8
RunIOTest(
  PSYSHOST host,
  UINT8  Node,
  UINT32 ChbitMask,
  UINT8  DQPat,
  UINT8  *SeqLCs,
  UINT8  ClearErrors,
  UINT16  Mode
  );

void
SetupIOTestCADB(
  PSYSHOST host,
  UINT8  Node,
  UINT32 ChbitMask,
  UINT8  LC,
  UINT8  SOE,
  UINT8  EnCADB,
  UINT8  EnCKE
  );

void
SetupIOTestBasicVA(
  PSYSHOST host,
  UINT8  socket,
  UINT32 ChbitMask,
  UINT8  LC,
  UINT8  SOE,
  UINT8  EnCADB,
  UINT8  EnCKE,
  UINT32 Spread
  );

void
SetupIOTestDQ(
  PSYSHOST host,
  UINT8  Node,
  UINT32 ChbitMask,
  UINT8  LC,
  UINT8  SOE,
  UINT8  EnCADB,
  UINT8  EnCKE
  );

void
SetupIOTestC2C(
  PSYSHOST host,
  UINT8  Node,
  UINT32 ChbitMask,
  UINT8  LC,
  UINT8  SOE,
  UINT8  EnCADB,
  UINT8  EnCKE
  );

void
SetupIOTestMPR(
  PSYSHOST host,
  UINT8  Node,
  UINT32 ChbitMask,
  UINT8  LC,
  UINT8  SOE,
  UINT8  EnCADB,
  UINT8  EnCKE
  );

UINT8
SelectCPGCRanks(
  PSYSHOST host,
  UINT8  socket,
  UINT8  ch,
  UINT8  RankBitMask,
  UINT8  RankFeatureEnable,
  UINT8  SubRankBitMask
  );

UINT32
WriteCADBCmd (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     Command,
  UINT8     Bank,
  UINT32    Data,
  UINT8     Delay
  );

void
MRSCmdStacking(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     Command
);

UINT32
CpgcWriteMRS (
  PSYSHOST host,
  UINT8  Node,
  UINT8  Ch,
  UINT8  Rank,
  UINT8  Bank,
  UINT16 Data
  );

UINT32
CpgcIssueZQ (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     zqType
  );

UINT32
CpgcAdvDqSetup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     rank,
  UINT8     subRank,
  UINT8     dqLoopCount
  );

#define MT_MAX_SUBSEQ       2
#define MT_PATTERN_DEPTH    2

#define MT_CPGC_WRITE       0
#define MT_CPGC_READ_WRITE  1
#define MT_CPGC_READ        2

#define MT_ADDR_DIR_UP      0
#define MT_ADDR_DIR_DN      1

UINT32
CpgcMemTestGlobalSetupMATS (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     rank,
  UINT8     dqLoopCount,
  UINT8     mode,
  UINT8     seqDataInv[MT_MAX_SUBSEQ],
  UINT64_STRUCT patternQW[MT_PATTERN_DEPTH],
  UINT8     uiShl[MT_PATTERN_DEPTH],
  UINT8     numCacheLines,
  UINT8     patternDepth,
  UINT8     direction
  );

VOID
CpgcMemTestRankSetupMATS (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     logRank,
  UINT8     logSubRank,
  UINT8     colAddressBits[MAX_CH],
  UINT8     rowAddressBits[MAX_CH],
  UINT8     bankAddressBits[MAX_CH],
  UINT8     mode,
  UINT8     direction,
  UINT8     bank,
  UINT8     baseBits,
  UINT8     seqDataInv[MT_MAX_SUBSEQ]
  );

UINT32
CpgcAdvTrainingExecTest (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT16    pattern
  );

UINT32
CpgcAdvTrainingErrorStatusMATS (
  PSYSHOST host,
  UINT8  socket,
  UINT32 ddrChEnMap,
  CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH]
  );

UINT32
CpgcAdvCmdPatternSetup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     rank,
  UINT8     dqLoopCount
  );

UINT32
CpgcFillWdbDqVa (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     mode
  );

UINT32
CpgcSetWdbVictimBit (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     wdbVictimBit
  );

UINT32
CpgcMemTestGlobalSetup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     rank,
  UINT8     dqLoopCount,
  UINT32    pattern
  );

UINT32
CpgcGlobalTrainingSetup (
  PSYSHOST                    host,
  UINT8                       socket,
  UINT8                       ch,
  CPGC_GLOBAL_TRAINING_SETUP  *cpgcGblTrainingSetup
  );

void
CpgcSetLogical2PhysicalRankMap (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  );

void
CpgcMemTestRankSetup (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     logRank,
  UINT8     logSubRank,
  UINT8     colAddressBits[MAX_CH],
  UINT8     rowAddressBits[MAX_CH],
  UINT8     bankAddressBits[MAX_CH]
  );

UINT32
CpgcSetupPprPattern (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row
  );

UINT32
CpgcPprTestGlobalSetup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     subRank,
  UINT32    dramMask,
  UINT8     bank,
  UINT32    row,
  UINT32    pattern
  );

UINT32
CpgcPprCleanup (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    chBitMask
  );

UINT32
CpgcClearGlobalErrorStatus (
  PSYSHOST  host,
  UINT8     socket
  );

void
CpgcDisableGlobalStart (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  );

UINT32
CpgcGlobalTestClear (
  PSYSHOST  host,
  UINT8     mcId,
  UINT8     ch
  );

UINT32
CpgcCtlClkPattern (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  GSM_CSN   signal
  );

UINT32
RestoreEarlyCmdPatterns (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     rank,
  UINT8     subrank,
  GSM_CSN   sig
  );

void
FixupAddrBits (
  PSYSHOST      host,
  GSM_CSN       sig,
  PatCadbProg0  prog0[CADB_LINES],
  PatCadbProg1  prog1[CADB_LINES]
  );

void
FixupCS (
  UINT8         cs,
  UINT8         subrank,
  PatCadbProg0  prog0[16],
  PatCadbProg1  prog1[16]
  );

void
ProgramCADB (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         ch,
  UINT8         startLine,
  UINT8         numLines,
  PatCadbProg0  prog0[16],
  PatCadbProg1  prog1[16]
  );

UINT32
GetCpgcRegOffset (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT32    regOffset
  );

void
ProgCADBRow (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT8         rank,
  UINT8         bank,
  UINT16        data,
  UINT8         side,
  UINT8         pdaTag,
  UINT8         control,
  PatCadbProg0  *patCADBProg0,
  PatCadbProg1  *patCADBProg1
  );

void
CpgcWrite (
  PSYSHOST  host,
  UINT8     socket,
  UINT32    ddrChEnMap,
  UINT8     numCacheLines
  );

void
SetupPatternSequencer (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         ch,
  SequencerMode sequencerMode,
  UINT32        *sequencerPattern
  );

void
SetCpgcIdleMode (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         ch
  );

void
DisableCadb (
  PSYSHOST      host,
  UINT8         socket,
  UINT8         mcId,
  UINT8         ch
  );

void
EnableCpgcClk(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     mcId,
  UINT8     cpgcClkEn
  );

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
  );
void
CpgcSetup64 (
            PSYSHOST host,
            UINT8  socket,
            UINT8  ch,
            UINT8  dimm,
            UINT32 dpa,
            UINT8  mode,
            UINT64_STRUCT *data
            );

void
WdbPreLoadChunk (
                PSYSHOST  host,
                UINT8     socket,
                UINT8     ch,
                UINT32    *wdbLines
                );

void
ClearBwErrorMask (
                 PSYSHOST host,
                 UINT8 socket
                 );

UINT32
CpgcPerNibbleErrorStatus (
                         PSYSHOST host,
                         UINT8 socket,
                         UINT8 ch
                         );

#include "CpgcChip.h"

#endif
