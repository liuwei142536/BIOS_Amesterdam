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
 *       DRAM Maintenance test.
 *
 ************************************************************************/

#ifndef __H_T_MAIN_H__
#define __H_T_MAIN_H__

#ifndef GROVEPORT_FLAG

#include "MemHost.h"
#include "SysFunc.h"

#define HT_SET_NEIGHBORS 0
#define HT_CHECK_NEIGHBORS 1

#define DMTLP_MAX_NUM_RANKS MAX_RANK_CH
#define DMTLP_MAX_NUM_CHANNELS MAX_CH
#define DMTLP_MAX_RANK_IN_CHANNEL DMTLP_MAX_NUM_RANKS

#define HT_NUMBER_SUBSEQUENCE_HAMMER 7
#define HT_MAX_BIT_HAMMER_REPETITIONS 31

#define SUCCESS 0

#pragma pack(push,1)

#define DMTLP_MAX_NUM_LANES 16
#define COL_GRANULARITY 8
#define INVALID_ORDER_NAME ORDER_NAMES_COUNT

#define BACKGROUND_PATTERN_SOLID 0
#define BACKGROUND_PATTERN_COL_STRIPES 1
#define BACKGROUND_PATTERN_ROW_STRIPES 2
#define BACKGROUND_PATTERN_CHECKERBOARD 3

#define MAX_NUMBER_TESTS 10

#define HAMMER_CHECK_NONE       0 //This is not a valid check??
#define HAMMER_CHECK_ROW_NB     1
#define HAMMER_CHECK_COL_NB     2
#define HAMMER_CHECK_DIAG_NB    4
#define HAMMER_CHECK_ALL_NB     7
#define HAMMER_CHECK_ALL_BANK   8

#define ROW_SWIZZLE_NONE               0
#define ROW_SWIZZLE_3XOR1_3XOR2        1
#define ROW_SWIZZLE_01234567EFCDAB89   2

#define SWIZZLE_BIT3_MASK 8

typedef enum BackgroundBaseTypes{
  BACKBASE_ZEROS = 0,
  BACKBASE_ONES,
  BACKBASE_FIVE,
  BACKBASE_A
}BackgroundBaseTypes;

typedef enum AddrSwizzleAlg{
  ADDR_SWIZZLE_ALG_NONE = 0,
  ADDR_SWIZZLE_ALG_ROW_WT
}AddrSwizzleAlg;

typedef struct RankBankRowColumn{
  UINT32 Rank;
  UINT32 Bank;
  UINT32 Row;
  UINT32 Col;
} RankBankRowColumn;

typedef enum OrderNames{
  COL = 0,
  ROW,
  BANK,
  RANK,
  ORDER_NAMES_COUNT
}OrderNames;

typedef struct dmtlpConfigParameters{
  char                *stringToParse;
  char                *testStrings[MAX_NUMBER_TESTS];
  UINT8                backgroundType;
  UINT8                numberChannels;
  UINT8                numberRankPerChannel;
  UINT8                numberTests;
  UINT8                DEBUGtestDirection;
  UINT8                maxNumberErrors;
  UINT8                startTestDelay;
  UINT8                subsequenceWait;
  UINT8                wrapTrgWANumCachelines;
  UINT8                startBackground;
  UINT8                endBackground;
  UINT8                swizzle[DMTLP_MAX_NUM_LANES];
  UINT8                numberLanesSwizzle;
  UINT8                maxLaneSwizzle;
  UINT8                ranksToIgnoreBitMask[DMTLP_MAX_NUM_CHANNELS];
  INT32                marginStartValue;
  INT32                marginStepValue;
  UINT32               channelBitMask;
  UINT32               failOnError;
  UINT32               dontStopErrors;
  UINT32               modifySwizzle;
  UINT32               traverseDiagonal;
  UINT32               enableDummyReads;
  UINT32               runTestPerBank;
  UINT32               runTestOnSecondOrder;
  UINT32               OptimizeSecondOrderRun;
  UINT32               tRefiMultiplier;
  UINT32               tCasValue;
  UINT32               pauseRefreshDelay;
  UINT32               marginOperation;
  UINT32               marginNumberSteps;
  UINT32               hammerRepetitions;
  UINT32               useWriteHammers;
  UINT32               hammerCheckType;
  UINT32               hammerPageCloseDelay;
  UINT32               rowSwizzleType;
  UINT32               bankMask;
  UINT32               singleRowForHammer;
  UINT32               hammerIterationsOnRow;
  OrderNames           firstOrder;
  OrderNames           secondOrder;
  OrderNames           thirdOrder;
  OrderNames           fourthOrder;
  BackgroundBaseTypes  backgroundBase;
  RankBankRowColumn    bottomAddress[DMTLP_MAX_NUM_CHANNELS];
  RankBankRowColumn    topAddress[DMTLP_MAX_NUM_CHANNELS];
  AddrSwizzleAlg       addrSwizzleAlg;
} dmtlpConfigParameters;

#pragma pack(pop)

typedef enum MemTravDir{
  UP,
  DOWN
} MemTravDir;

typedef enum CellOperation{
  CO_READ,
  CO_WRITE
}CellOperation;

typedef enum OffsetType{
  TO_NA,
  TO_NONE,
  TO_BFR
}OffsetType;

typedef UINT32 (*SWIZZLE_FUNCTION)(UINT32 physicalAddress);

typedef struct HammerSymbolExecParameters{
  MemTravDir Direction;
  UINT32 numberRepetitions;
  UINT32 numberIterationsRow;
  CellOperation baseOperation;
  BOOLEAN baseInverted;
  SWIZZLE_FUNCTION swizzleFunction;
//OffsetType offsetType;
//CellOperation offsetOperation;
//BOOLEAN offsetInverted;
//BOOLEAN singleRowHammer;
//UINT32 delay;
//UINT32 swizzleType;
}HammerSymbolExecParameters;

VOID RunHTTest(PSYSHOST host, UINT8 socket, dmtlpConfigParameters* testParameters, HammerSymbolExecParameters* execParameters);
VOID DisableRefresh(PSYSHOST host, UINT8 socket);
VOID EnableRefresh(PSYSHOST host, UINT8 socket);
void DcttHtToolVersion(UINT16* majorVer, UINT16* minorVer, UINT32* rev);

INT32 ParseTest(PSYSHOST host, UINT8 socket, INT8* testString, dmtlpConfigParameters* testParameters, HammerSymbolExecParameters* execParameters);
INT32 InitializeConfig(dmtlpConfigParameters* testParams);
INT32 DcttHt(PSYSHOST host, UINT8 socket);

#endif // __H_T_MAIN_H__
#endif // !GROVEPORT_FLAG

