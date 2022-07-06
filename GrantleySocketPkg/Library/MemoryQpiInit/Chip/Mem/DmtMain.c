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

#include "SysHost.h"
#include "SysFunc.h"
#include "Cpgc.h"
#include "DmtMain.h"

//#define ResetCPGCEngine(host, socket, testParameters)

VOID
SetRefresh (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     refMaskValue
  )
/*++

  Sets CPGC register used to enable/disable refresh per rank


  @param host                - Pointer to sysHost
  @param socket              - Socket on which we want to modify the rank refresh mask

  @retval N/A

--*/
{
  UINT8                               ch;
  CPGC_MISCREFCTL_MCDDC_CTL_STRUCT    cpgcMiscRefCtlReg;

  for (ch = 0; ch < MAX_CH; ch++) {
    cpgcMiscRefCtlReg.Data = MemReadPciCfgEp (host, socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG);
    cpgcMiscRefCtlReg.Bits.ref_rankmask = refMaskValue;
    MemWritePciCfgEp (host, socket, ch, CPGC_MISCREFCTL_MCDDC_CTL_REG, cpgcMiscRefCtlReg.Data);
  } // ch loop
} // SetRefresh

VOID
DisableRefresh (
  PSYSHOST  host,
  UINT8     socket
)
/*++

  Disables refreshes on all ranks on all channels

  @param host                - Pointer to sysHost
  @param socket              - Socket on which we want to disable refreshes

  @retval N/A

--*/
{
  SetRefresh(host, socket, (UINT8)-1);
} // DisableRefresh

VOID
EnableRefresh (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Enables refreshes on all ranks on all channels

  @param host                - Pointer to sysHost
  @param socket              - Socket on which we want to enable refreshes

  @retval N/A

--*/
{
  SetRefresh(host, socket, 0);
} // EnableRefresh

INT32
InitializeConfig (
  dmtlpConfigParameters* testParams
  )
/*++

  Initialize a test parameter structure with default values

  @param testParams - Pointer to structure that contains the test parameters to be initialized to default values

  @retval SUCCESS
  @retval FAILURE

--*/
{
  UINT8 Index;

  testParams->modifySwizzle = FALSE;

  for (Index = 0; Index < 16; Index++) {
    testParams->swizzle[Index] = Index;
  } // Index loop

  testParams->numberLanesSwizzle   = 1;
  testParams->maxLaneSwizzle       = 0;
  testParams->traverseDiagonal     = FALSE;
  testParams->numberTests          = 1;
  testParams->testStrings[0]       = "^(fhammerd)";
  testParams->numberChannels       = DMTLP_MAX_NUM_CHANNELS;
  testParams->channelBitMask       = 0xF;
  testParams->numberRankPerChannel = DMTLP_MAX_RANK_IN_CHANNEL;

  for (Index = 0; Index < 2; Index++) {
    testParams->bottomAddress[Index].Bank = 0;
    testParams->bottomAddress[Index].Col  = 0;
    testParams->bottomAddress[Index].Row  = 0;
    testParams->bottomAddress[Index].Rank = 0;

    testParams->topAddress[Index].Bank    = 7;
    testParams->topAddress[Index].Col     = 1016;
    testParams->topAddress[Index].Row     = 65534;
    testParams->topAddress[Index].Rank    = 0;
  } // Index loop

  testParams->firstOrder   = COL;
  testParams->secondOrder  = ROW;
  testParams->thirdOrder   = BANK;
  testParams->fourthOrder  = RANK;
  testParams->backgroundBase = BACKBASE_ZEROS;
  testParams->backgroundType = BACKGROUND_PATTERN_SOLID;
  testParams->maxNumberErrors = 1;
  testParams->failOnError     = TRUE;
  testParams->dontStopErrors  = FALSE;
  testParams->startTestDelay  = 2;
  testParams->subsequenceWait = 0;
  testParams->addrSwizzleAlg  = ADDR_SWIZZLE_ALG_NONE;
  testParams->wrapTrgWANumCachelines = 15;//num cachelines = 2^wrapTrgWANumCachelines
  testParams->startBackground = BACKGROUND_PATTERN_ROW_STRIPES;
  testParams->endBackground   = BACKGROUND_PATTERN_ROW_STRIPES;
  testParams->enableDummyReads       = FALSE;
  testParams->runTestPerBank         = FALSE;
  testParams->runTestOnSecondOrder   = FALSE;
  testParams->OptimizeSecondOrderRun = FALSE;
  testParams->tRefiMultiplier        = 1;
  testParams->ranksToIgnoreBitMask[0] = 0;
  testParams->ranksToIgnoreBitMask[1] = 0;
  testParams->tCasValue         = 0;
  testParams->pauseRefreshDelay = 0;
  testParams->marginOperation   = 0;
  testParams->marginStartValue  = 0;
  testParams->marginStepValue   = 0;
  testParams->marginNumberSteps = 0;
  testParams->hammerRepetitions = 3;
  testParams->useWriteHammers   = TRUE;
  testParams->hammerCheckType   = HAMMER_CHECK_ROW_NB;
  testParams->hammerPageCloseDelay = 27;
  testParams->rowSwizzleType       = ROW_SWIZZLE_NONE;
  testParams->bankMask             = 0;
  testParams->singleRowForHammer   = FALSE;
  testParams->hammerIterationsOnRow = 1;

  return SUCCESS;
} // InitializeConfig

UINT32
Swizzle_GetLogicalAddressNoSwizzle (
  UINT32 physicalAddress
  )
/*++

  Calculates a logical row address given a physical one
  The algorithm for this swizzle is logicalAddress = physicalAddress

  @param physicalAddress - physical row address

  @retval logical address calculated using the physicalAddress input

--*/
{
  return physicalAddress;
} // Swizzle_GetLogicalAddressNoSwizzle


UINT32
Swizzle_GetLogicalAddress3xor1_3xor2 (
  UINT32 physicalAddress
  )
/*++

  Calculates a logical row address given a physical one
  The algorithm for this swizzle is address 3 xor 1 + address 3 xor 2

  @param physicalAddress - physical row address

  @retval logical address calculated using the physicalAddress input

--*/
{
  UINT32 xorMask;

  xorMask = (((physicalAddress & SWIZZLE_BIT3_MASK) >> 1) | ((physicalAddress & SWIZZLE_BIT3_MASK) >> 2));
  return (xorMask ^ physicalAddress);
}


VOID CPGCSettingsSetTestFail(
  void                       *host,
  UINT8                       socket,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters
  )
{

  UINT8                                 ch;
  UINT8                                 mcId;
  UINT32                                subsequenceOffset;
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT cpgcSeqBaincCtl00;
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT        cpgcSeqCtl0Reg;
  CPGC_SUBSEQ0_CTL0_MC_MAINEXT_STRUCT   cpgcSubseqCtl0Reg;
  CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT    cpgcSeqBaoCic0;

  subsequenceOffset = CPGC_SUBSEQ1_CTL0_MC_MAINEXT_REG - CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG;

  for (ch = 0; ch < MAX_CH; ch++) {
    mcId = GetMCID(host, socket, ch);

    //Setting increment to stay on the same cell inside a row
    cpgcSeqBaincCtl00.Data = MemReadPciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAINEXT_REG));
    cpgcSeqBaincCtl00.Data = 0;
    cpgcSeqBaincCtl00.Bits.column_ba_inc = 0;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAINEXT_REG), cpgcSeqBaincCtl00.Data);

    //Set start and stop pointers on the victims subsequences
    cpgcSeqCtl0Reg.Data = MemReadPciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQCTL0_MC_MAINEXT_REG));
    cpgcSeqCtl0Reg.Bits.subseq_start_pointer = 8;
    cpgcSeqCtl0Reg.Bits.subseq_end_pointer = 9;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQCTL0_MC_MAINEXT_REG), cpgcSeqCtl0Reg.Data);

    //Setting last 2 sequences to a read and a write with different polarities
    cpgcSubseqCtl0Reg.Data = 0;
    cpgcSubseqCtl0Reg.Bits.stop_baseseq_wrptrig = 0;
    cpgcSubseqCtl0Reg.Bits.inv_dataecc = 0;
    cpgcSubseqCtl0Reg.Bits.dataecc_addrinv = 0;
    cpgcSubseqCtl0Reg.Bits.rstcurr_ba_start = 0;
    cpgcSubseqCtl0Reg.Bits.savecurr_ba_start = 0;
    cpgcSubseqCtl0Reg.Bits.subseq_type = 1;//CO_WRITE
    cpgcSubseqCtl0Reg.Bits.subseq_wait = 2;
    cpgcSubseqCtl0Reg.Bits.num_cachelines_scale = 0;
    cpgcSubseqCtl0Reg.Bits.num_cachelines = 7;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * 8), cpgcSubseqCtl0Reg.Data);

    cpgcSubseqCtl0Reg.Bits.subseq_type = 0;//CO_READ
    cpgcSubseqCtl0Reg.Bits.inv_dataecc = 1;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * 9), cpgcSubseqCtl0Reg.Data);

    cpgcSeqBaoCic0.Data  = MemReadPciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAOCIC0_MC_MAINEXT_REG));
    cpgcSeqBaoCic0.Bits.rank_bw_trigen = 1;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAOCIC0_MC_MAINEXT_REG), cpgcSeqBaoCic0.Data);
  } // ch loop
} // CPGCSettingsSetTestFail


INT32
ParseTest (
  PSYSHOST                    host,
  UINT8                       socket,
  INT8                       *testString,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters
  )
/*++

  Sets up DRAM Maintenance test parameters by reading setup options

  @param host                  - Pointer to sysHost
  @param socket                - Node on which we are going to run the DRAM Maintenance test
  @param testParameters        - Pointer to a test parameters struct
  @param execParameters        - Pointer to a DRAM Maintenance test parameters structure

  @retval SUCCESS
  @retval FAILURE

--*/
{
  UINT8  ch;
  UINT8  dimm;
  UINT8  mixdimm;
  struct dimmNvram     (*dimmNvList)[MAX_DIMM];
  struct channelNvram  (*channelNvList)[MAX_CH];

  execParameters->Direction             = host->setup.mem.DMTDir;
  execParameters->baseInverted          = host->setup.mem.DMTInv;
  execParameters->numberRepetitions     = host->setup.mem.DMTRep;
  testParameters->rowSwizzleType        = host->setup.mem.DMTSwizzleEn;
  execParameters->swizzleFunction       = Swizzle_GetLogicalAddressNoSwizzle;
  if (testParameters->rowSwizzleType == ROW_SWIZZLE_3XOR1_3XOR2) {
    execParameters->swizzleFunction = Swizzle_GetLogicalAddress3xor1_3xor2;
  }

  execParameters->baseOperation         = CO_WRITE;
  testParameters->hammerIterationsOnRow = host->setup.mem.DMTIter;

  channelNvList = GetChannelNvList(host, socket);
  mixdimm = 0;

  for(ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmNvList    = GetDimmNvList(host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

      if (host->setup.mem.DMTSwizzleEn == 2) {
        if ((*dimmNvList)[dimm].SPDDramMfgId == MFGID_SAMSUNG) {
          testParameters->rowSwizzleType    = ROW_SWIZZLE_3XOR1_3XOR2;
          mixdimm = 1;
          if (mixdimm == 2) {
            MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "ERROR: Mix Dimm configuration unsupported on DRAM maintenence test\n"));
            FatalError(host, WARN_DM_TEST_ERROR_CODE, WARN_DM_TEST_CONFIGURATION_ERROR_MINOR_CODE);
          }
        } else {
          testParameters->rowSwizzleType    = ROW_SWIZZLE_NONE;
          mixdimm = 2;
          if (mixdimm == 1) {
            MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "ERROR: Mix Dimm configuration unsupported on DRAM maintenence  test\n"));
            FatalError(host, WARN_DM_TEST_ERROR_CODE, WARN_DM_TEST_CONFIGURATION_ERROR_MINOR_CODE);
          }
        }
      } else if (host->setup.mem.DMTSwizzleEn == 1) {
        testParameters->rowSwizzleType      = ROW_SWIZZLE_3XOR1_3XOR2;
      } else {
        testParameters->rowSwizzleType      = ROW_SWIZZLE_NONE;
      }
    } // dimm loop
  } // ch loop

  return SUCCESS;
} // ParseTest

UINT32 CalculateSubsequenceLoopcounts(
  PSYSHOST  host,
  UINT32    numberHammerRepetitions,
  UINT32    subsequenceLoopcountBits[HT_NUMBER_SUBSEQUENCE_HAMMER],
  UINT8    *numberSubsequencesUsed
  )
/*++

  Calculates the number of bits to use on each of the CPGC subsequence loopcounts to get the desired total number of hammer repetitions
  Notice that we can end up with an effective total number of hammer repetitions that is less than the desired ones if we do not have enough subsequences available

  @param host                      - Pointer to sysHost
  @param numberHammerRepetitions   - Desired number of hammer repetitions for the row hammer test
  @param subsequenceLoopcountBits  - Array containing the number of loopcount bits to be used on each subsequence loopcount
  @param numberSubsequencesUsed    - Pointer to a UINT8 variable to be set to the total number of subsequences to be used on the hammer test

  @retval Real number of hammer repetitions to be used on the test

--*/
{
  INT32   bitNumber;
  UINT32  hammerRepetitionsApprox = 0;
  UINT32  bitMask;

  *numberSubsequencesUsed = 0;

  for (bitNumber = HT_MAX_BIT_HAMMER_REPETITIONS; bitNumber >= 0; bitNumber--) {
    bitMask = (1 << bitNumber);

    if (bitMask & numberHammerRepetitions) {
      subsequenceLoopcountBits[*numberSubsequencesUsed] = bitNumber;
      *numberSubsequencesUsed                           = *numberSubsequencesUsed + 1;
      hammerRepetitionsApprox                           += bitMask;

      if (*numberSubsequencesUsed >= HT_NUMBER_SUBSEQUENCE_HAMMER)  break;
    }
  }

  if (*numberSubsequencesUsed == 0) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "ERROR: Couldn't calculate number loopcounts for subsequences\n"));
    FatalError(host, WARN_DM_TEST_ERROR_CODE, WARN_DM_TEST_EXECUTION_ERROR_MINOR_CODE);
  }

  return hammerRepetitionsApprox;
} // CalculateSubsequenceLoopcounts

VOID
SetHammerSubsequences (
  PSYSHOST                    host,
  UINT8                       socket,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters,
  UINT32                      subsequenceLoopcountBits[HT_NUMBER_SUBSEQUENCE_HAMMER],
  UINT8                       numberSubsequencesUsed,
  UINT8                       columnBits[MAX_CH]
  )
/*++

  Sets the subsequence CPGC registers to be used on a DRAM Maintenance test

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to set up the registers
  @param testParameters            - Pointer to a test parameters struct
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure
  @param subsequenceLoopcountBits  - Array containing the number of loopcount bits to be used on each subsequence loopcount. This array is filled by function CalculateSubsequenceLoopcounts
  @param numberSubsequencesUsed    - Total number of subsequences to be used on the DRAM Maintenance test. This array is filled by function CalculateSubsequenceLoopcounts
  @param columnBits                - Array containing the number of column bits per channel

  @retval N/A

--*/
{
  UINT8                                 subseqNumber;
  UINT8                                 ch;
  UINT32                                subsequenceOffset;
  UINT32                                numCol;
  CPGC_SUBSEQ0_CTL0_MC_MAINEXT_STRUCT   cpgcSubseqCtl0Reg;

  subsequenceOffset = CPGC_SUBSEQ1_CTL0_MC_MAINEXT_REG - CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG;

  //Setting the subsequence for hammer
  cpgcSubseqCtl0Reg.Bits.stop_baseseq_wrptrig = 0;
//  cpgcSubseqCtl0Reg.Bits.inv_dataecc          = execParameters->baseInverted;
  cpgcSubseqCtl0Reg.Bits.dataecc_addrinv      = 0;//We are not running batch tests, so this will suffice
  cpgcSubseqCtl0Reg.Bits.rstcurr_ba_start     = 0;
  cpgcSubseqCtl0Reg.Bits.savecurr_ba_start    = 0;
  cpgcSubseqCtl0Reg.Bits.subseq_type          = CO_READ;
  cpgcSubseqCtl0Reg.Bits.subseq_wait          = testParameters->subsequenceWait;
  cpgcSubseqCtl0Reg.Bits.num_cachelines_scale = 0;

  cpgcSubseqCtl0Reg.Bits.inv_dataecc = 1;

  for (subseqNumber = 0; subseqNumber < numberSubsequencesUsed; subseqNumber++) {
    if (subsequenceLoopcountBits[subseqNumber] > 31) {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "ERROR: Wrong cacheline number for subsequence %d: %d\n",
                    subseqNumber, subsequenceLoopcountBits[subseqNumber]));
      FatalError(host, WARN_DM_TEST_ERROR_CODE, WARN_DM_TEST_EXECUTION_ERROR_MINOR_CODE);
    }

    cpgcSubseqCtl0Reg.Bits.num_cachelines = subsequenceLoopcountBits[subseqNumber];

    for (ch = 0; ch < MAX_CH; ch++) {
      MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                        GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * subseqNumber),
                        cpgcSubseqCtl0Reg.Data);
    } // ch loop
  } // subseqNumber loop

  //Setting the subsequence that sets the attacker row
  cpgcSubseqCtl0Reg.Bits.subseq_type = CO_WRITE;
  for (ch = 0; ch < MAX_CH; ch++) {
    numCol = columnBits[ch] - 3;
    cpgcSubseqCtl0Reg.Bits.num_cachelines = numCol;
    MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                      GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * 7),
                      cpgcSubseqCtl0Reg.Data);
  } // ch loop

  //Setting the subsequence that sets the victim rows
  cpgcSubseqCtl0Reg.Bits.inv_dataecc = 0;
  cpgcSubseqCtl0Reg.Bits.subseq_type = CO_WRITE;
  for (ch = 0; ch < MAX_CH; ch++)  {
    numCol                                = columnBits[ch] - 3;
    cpgcSubseqCtl0Reg.Bits.num_cachelines = numCol;
    MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                      GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * 8),
                      cpgcSubseqCtl0Reg.Data);
  } // ch loop

  //Setting the subsequence that checks the victim rows
  cpgcSubseqCtl0Reg.Bits.subseq_type = CO_READ;
  for (ch = 0; ch < MAX_CH; ch++) {
    numCol                                = columnBits[ch] - 3;
    cpgcSubseqCtl0Reg.Bits.num_cachelines = numCol;
    MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                      GetCpgcRegOffset (host, socket, ch, CPGC_SUBSEQ0_CTL0_MC_MAINEXT_REG) + (subsequenceOffset * 9),
                      cpgcSubseqCtl0Reg.Data);
  } // ch loop
} // SetHammerSubsequences

VOID
WriteTestStartAndWrap (
  PSYSHOST                                      host,
  UINT8                                         socket,
  UINT8                                         ch,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT  cpgcSeqBaseAddrStart,
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT  cpgcSeqBaseAddrStartL0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT   cpgcSeqBaseAddrWrap,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT   cpgcSeqBaseAddrWrapL0
  )
/*++

  Sets the CPGC address start and wrap registers

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to set up the registers
  @param ch                        - Channel on which we are going to set up the registers
  @param cpgcSeqBaseAddrStart      - CPGC Base address start register structure
  @param cpgcSeqBaseAddrWrap       - CPGC Base address start register structure

  @retval N/A

--*/
{
  UINT8  mcId;

  mcId = GetMCID(host, socket, ch);

  MemWritePciCfgMC (host, socket, mcId,
                    GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_REG),
                    cpgcSeqBaseAddrStart.Data);

  MemWritePciCfgMC (host, socket, mcId,
                    GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_REG),
                    cpgcSeqBaseAddrWrap.Data);

  MemWritePciCfgMC (host, socket, mcId,
                    GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_REG),
                    cpgcSeqBaseAddrStartL0.Data);
  MemWritePciCfgMC (host, socket, mcId,
                    GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_REG),
                    cpgcSeqBaseAddrWrapL0.Data);
} // WriteTestStartAndWrap

VOID
WriteSeqCtlSubseqPointers (
  PSYSHOST                            host,
  UINT8                               socket,
  UINT8                               ch,
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT      cpgcSeqCtl0Reg[MAX_CH],
  UINT8                               startPointer,
  UINT8                               endPointer
  )
/*++

  Sets the start and stop subsequences on the CPGC main sequence registers

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to set up the registers
  @param cpgcSeqCtl0Reg            - Array containing the value of the main sequence register structure
  @param startPointer              - first subsequence to run on the test
  @param endPointer                - last subsequence to run on the test

  @retval N/A

--*/
{
  //UINT8 ch;

  //for (ch = 0; ch < MAX_CH; ch++) {
    cpgcSeqCtl0Reg[ch].Bits.subseq_start_pointer = startPointer;
    cpgcSeqCtl0Reg[ch].Bits.subseq_end_pointer   = endPointer;
    MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch),
                      GetCpgcRegOffset (host, socket, ch, CPGC_SEQCTL0_MC_MAINEXT_REG),
                      cpgcSeqCtl0Reg[ch].Data);
  //} // ch loop
} // WriteSeqCtlSubseqPointers

VOID
ReadSeqCtl (
  PSYSHOST                        host,
  UINT8                           socket,
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT  cpgcSeqCtl0Reg[MAX_CH]
  )
/*++

  Reads the value of the sequence control registers for all channels
  It returns default values for some of the fields on the register

  @param host                      - Pointer to sysHost
  @param socket                    - Socket from which we are going to read the registers
  @param cpgcSeqCtl0Reg            - Array on which we are going to write the value of the main sequence register structure

  @retval N/A

--*/
{
  UINT8 ch;

  for (ch = 0; ch < MAX_CH; ch++) {
    cpgcSeqCtl0Reg[ch].Data = MemReadPciCfgMC (host, socket, GetMCID(host, socket, ch),
                                               GetCpgcRegOffset (host, socket, ch, CPGC_SEQCTL0_MC_MAINEXT_REG));

    //Insuring that the "default value" of these registers is OK
    cpgcSeqCtl0Reg[ch].Bits.loopcount                     = 1;
    cpgcSeqCtl0Reg[ch].Bits.initialization_mode           = 1;
    cpgcSeqCtl0Reg[ch].Bits.enable_constant_write_strobe  = 0;
    cpgcSeqCtl0Reg[ch].Bits.enable_dummy_reads            = 0;
    cpgcSeqCtl0Reg[ch].Bits.address_update_rate_mode      = 0;
    cpgcSeqCtl0Reg[ch].Bits.stop_base_seq_on_wrap_trigger = 0;
  } // ch loop
} // ReadSeqCtl

VOID
WriteRowColIncCtrl (
  PSYSHOST                              host,
  UINT8                                 socket,
  UINT8                                 ch,
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT cpgcSeqBaincCtl00
  )
/*++

  Sets the increment control CPGC registers

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to set up the registers
  @param ch                        - Channel on which we are going to set up the registers
  @param cpgcSeqBaincCtl00         - increment control register structure

  @retval N/A

--*/
{
  MemWritePciCfgMC (host, socket, GetMCID(host, socket, ch), GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAINCCTL00_MC_MAINEXT_REG), cpgcSeqBaincCtl00.Data);
} // WriteRowColIncCtrl

VOID
GetRowColIncCtrl (
  PSYSHOST                              host,
  UINT8                                 socket,
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT cpgcSeqBaincCtl00[MAX_CH]
  )
/*++

  Gets the increment control CPGC registers default value

  @param host                      - Pointer to sysHost
  @param socket                    - Socket from which we are going to get the registers
  @param cpgcSeqBaincCtl00         - Array containing the value of the increment control register structures

  @retval N/A

--*/
{
  UINT8 ch;

  for (ch = 0; ch < MAX_CH; ch++) {
    cpgcSeqBaincCtl00[ch].Bits.column_ba_updscale = 0;
    cpgcSeqBaincCtl00[ch].Bits.column_ba_updrate  = 0;
    cpgcSeqBaincCtl00[ch].Bits.column_ba_inc      = 1;

    //Rows are never going to change
    cpgcSeqBaincCtl00[ch].Bits.row_ba_inc         = 0;
    cpgcSeqBaincCtl00[ch].Bits.row_ba_updrate     = 0;
    cpgcSeqBaincCtl00[ch].Bits.row_ba_updscale    = 0;
  } // ch loop
} // GetRowColIncCtrl

VOID CPGCSettingsOverride (
  PSYSHOST                    host,
  UINT8                       socket,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters,
  UINT8                       ddrChEnMap,
  UINT8                       rank,
  UINT8                       colAddressBits[MAX_CH],
  UINT8                       rowAddressBits[MAX_CH],
  UINT8                       bankAddressBits[MAX_CH]
  )
/*++

  Overrides some CPGC register settings from the value set by BIOS training functions

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to set up the registers
  @param testParameters            - Pointer to a test parameters struct
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure
  @param ddrChEnMap                - Mask of enabled channels
  @param rank                      - Logical rank on which we want to override the CPGC register's values
  @param colAddressBits            - Array containing the number of column bits per channel
  @param rowAddressBits            - Array containing the number of row bits per channel
  @param bankAddressBits           - Array containing the number of bank bits per channel

  @retval Nothing

--*/
{
  UINT8                                         ch;
  UINT8                                         mcId;
  CPGC_SEQ_BAINCCTL10_MC_MAINEXT_HSX_BDX_STRUCT cpgcSeqBaincCtl10;
  CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT            cpgcSeqBaoCic0;
  CPGC_ERR_CTL_MCDDC_CTL_STRUCT                 cpgcErrCtlReg;
  CPGC_PATWDBCLCTL_MCDDC_CTL_HSX_BDX_STRUCT     cpgcPatWdbClCtl;
  MCMTR_MC_MAIN_HSX_BDX_STRUCT                  mcMtr;

  for (ch = 0; ch < MAX_CH; ch++) {
    mcId = GetMCID(host, socket, ch);

    //Carries
    cpgcSeqBaoCic0.Bits.rank_bw_cryen   = 0;
    cpgcSeqBaoCic0.Bits.bank_bw_cryen   = 0;
    cpgcSeqBaoCic0.Bits.row_bw_cryen    = 0;
    cpgcSeqBaoCic0.Bits.column_bw_cryen = 0;
    //Triggers
    cpgcSeqBaoCic0.Bits.rank_bw_trigen   = 0;
    cpgcSeqBaoCic0.Bits.bank_bw_trigen   = 0;
    cpgcSeqBaoCic0.Bits.row_bw_trigen    = 0;
    cpgcSeqBaoCic0.Bits.column_bw_trigen = 0;
    //Inversion
    cpgcSeqBaoCic0.Bits.rank_ba_inven    = 0;
    cpgcSeqBaoCic0.Bits.bank_ba_inven    = 0;
    cpgcSeqBaoCic0.Bits.row_ba_inven     = 0;
    cpgcSeqBaoCic0.Bits.column_ba_inven  = 0;
    cpgcSeqBaoCic0.Bits.addr_inv_rate    = 0;
    //Order; only supported order is Col, Row, Bank, Rank
    cpgcSeqBaoCic0.Bits.column_addr_order = 0;
    cpgcSeqBaoCic0.Bits.row_addr_order    = 1;
    cpgcSeqBaoCic0.Bits.bank_addr_order   = 2;
    cpgcSeqBaoCic0.Bits.rank_addr_order   = 3;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAOCIC0_MC_MAINEXT_REG), cpgcSeqBaoCic0.Data);

    //Increment rates for bank, ranks; these never change; so we can set them to 0
    cpgcSeqBaincCtl10.Bits.rank_ba_inc      = 0;
    cpgcSeqBaincCtl10.Bits.rank_ba_updrate  = 0;
    cpgcSeqBaincCtl10.Bits.rank_ba_updscale = 1;
    cpgcSeqBaincCtl10.Bits.bank_ba_inc      = 0;
    cpgcSeqBaincCtl10.Bits.bank_ba_updrate  = 0;
    cpgcSeqBaincCtl10.Bits.bank_ba_updscale = 1;
    cpgcSeqBaincCtl10.Bits.column_ba_inc_msb = 0;
    MemWritePciCfgMC (host, socket, mcId, GetCpgcRegOffset (host, socket, ch, CPGC_SEQ_BAINCCTL10_MC_MAINEXT_REG), cpgcSeqBaincCtl10.Data);

    ////TODO: (If necessary) add the WDB control here
    // Program CPGC WDB Pointer Control
    cpgcPatWdbClCtl.Data            = 0;
    cpgcPatWdbClCtl.Bits.incr_rate  = 0;
    cpgcPatWdbClCtl.Bits.incr_scale = 0;
    cpgcPatWdbClCtl.Bits.start_pntr = 0;
    cpgcPatWdbClCtl.Bits.end_pntr   = 0;
    MemWritePciCfgEp (host, socket, ch, CPGC_PATWDBCLCTL_MCDDC_CTL_REG, cpgcPatWdbClCtl.Data);

    ////TODO: Add the error stop mode here
    cpgcErrCtlReg.Data                          = 0;
    cpgcErrCtlReg.Bits.stop_on_ntherr           = 1;
    cpgcErrCtlReg.Bits.stop_on_errcontrol       = 1;
    cpgcErrCtlReg.Bits.selective_err_enb_cl     = CPGC_ERR_MONITOR_ALL_CL;
    cpgcErrCtlReg.Bits.selective_err_enb_chunk  = CPGC_ERR_MONITOR_ALL_CHUNK;
    //cpgcErrCtlReg.Bits.stop_on_errcontrol       = CPGC_ERR_STOP_ON_ALL_ERR;
    MemWritePciCfgEp (host, socket, ch, CPGC_ERR_CTL_MCDDC_CTL_REG, cpgcErrCtlReg.Data);
  } // ch loop

  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
    mcMtr.Bits.close_pg = CLOSED_PAGE_EN;//CLOSED_PAGE_EN = 1
    MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } // mcId loop
} // CPGCSettingsOverride

VOID
GetLogicRows (
  void                       *host,
  UINT8                       socket,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters,
  UINT32                      physicalRow[MAX_CH],
  UINT32                      totalNumberRows[MAX_CH],
  UINT32                      neighborRows[2][MAX_CH],
  UINT32                      currentLogicalRow[MAX_CH]
  )
/*++

  Calculates the logical addresses of row neighbors and the given address from a physical address

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to run the DRAM Maintenance test
  @param testParameters            - Pointer to a test parameters struct
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure
  @param physicalRow               - Array containing the address of the current physical rows per channel
  @param totalNumberRows           - Array containing the total number of rows per channel
  @param neighborRows              - Array on which we are going to write the logical addresses of the two neighbors of the physicalRow per channel
  @param currentLogicalRow         - Array on which we are going to write the logical addresses corresponding to the physicalRow address per channel

  @retval N/A

--*/
{
  UINT8 ch;

  for (ch = 0; ch < MAX_CH; ch++) {
    //Setting the physical row neighbor addresses
    if (physicalRow[ch] == 0) {
      neighborRows[0][ch] = execParameters->swizzleFunction(physicalRow[ch] + 1);
      //Next is not a "real" neighbor but it simplifies the test
      neighborRows[1][ch] = execParameters->swizzleFunction(totalNumberRows[ch]);
    } else if (physicalRow[ch] == totalNumberRows[ch]) {
      neighborRows[0][ch] = execParameters->swizzleFunction(physicalRow[ch] - 1);
      //Next is not a "real" neighbor but it simplifies the test
      neighborRows[1][ch] = execParameters->swizzleFunction(0);
    } else {
      neighborRows[0][ch] = execParameters->swizzleFunction(physicalRow[ch] + 1);
      neighborRows[1][ch] = execParameters->swizzleFunction(physicalRow[ch] - 1);
    }
    //Setting the hammer row address
    currentLogicalRow[ch] = execParameters->swizzleFunction(physicalRow[ch]);
  } // ch loop
} // GetLogicRows

UINT8
GetChErrorStatus (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Returns the CPGC channel error status

  @param host                      - Pointer to sysHost
  @param socket                    - Socket from which we are going to read the registers

  @retval CPGC channel error information

--*/
{
  UINT8                                   errStatus = 0;
  CPGC_GLOBALERR_MC_MAINEXT_STRUCT        cpgcGlobalErrReg;

  cpgcGlobalErrReg.Data = MemReadPciCfgMC (host, socket, GetMCID(host, socket, 0), GetCpgcRegOffset (host, socket, 0, CPGC_GLOBALERR_MC_MAINEXT_REG));

  errStatus  = (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_0;
  errStatus |= (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_1 << 1;

  if (host->var.common.cpuSubType == CPU_HSX_2HA) {
    // 2HA
    cpgcGlobalErrReg.Data = MemReadPciCfgMC (host, socket, GetMCID(host, socket, 2), GetCpgcRegOffset (host, socket, 0, CPGC_GLOBALERR_MC_MAINEXT_REG));
    errStatus |= (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_0 << 2;
    errStatus |= (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_1 << 3;
  } else {
    // 1HA
    errStatus |= (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_2 << 2;
    errStatus |= (UINT8) cpgcGlobalErrReg.Bits.channel_err_status_3 << 3;
  }

  return (errStatus & 0xFFFF);
} // GetChErrorStatus

VOID
DisplayError (
  PSYSHOST   host,
  UINT8      socket,
  UINT8      logicalRank,
  UINT8      errorStatus,
  UINT32     currentBank,
  UINT32    *currentRow
  )
/**

  Display Error

  @param host        - Pointer to sysHost
  @param socket      - Socket from which we are going to read the registers
  @param logicalRank - Logical Rank
  @param errorStatus - error 
  @param currentBank - Current bank
  @param currentRow  - Current row

  @retval N/A

**/
{
  UINT8  ch;
  UINT8  dimm;
  UINT8  rank;
  UINT32 col;
  UINT32 errorDq;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  CPGC_ERR_ADDR1_MCDDC_CTL_HSX_BDX_STRUCT errorAdrr;

  channelNvList = GetChannelNvList(host, socket);

  for (ch = 0; ch < MAX_CH; ch++) {
    if ((*channelNvList)[ch].enabled == 0) continue;
    if (((errorStatus >> ch) & 0x1) !=0 ) {

      errorAdrr.Data  = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_ADDR1_MCDDC_CTL_REG);
      col             = (UINT32) errorAdrr.Bits.column;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        dimmNvList = GetDimmNvList(host, socket, ch);
        if ((*dimmNvList)[dimm].dimmPresent == 0) continue;

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          rankList  =  GetRankNvList(host, socket, ch, dimm);
          if (CheckRank(host, socket, ch, dimm, rank, CHECK_MAPOUT)) continue;

          if (logicalRank == GetLogicalRank(host, socket, ch, dimm, rank)){

            errorDq = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_DATA0_S_MCDDC_CTL_REG);
            if (errorDq != 0) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "BNK: %d, ROW: %d, COL: %d, EVEN Dq[31:0]: 0x%x \n",
                             currentBank, currentRow[ch], col, errorDq));
            }

            errorDq = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_DATA1_S_MCDDC_CTL_REG);
            if (errorDq != 0) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "BNK: %d, ROW: %d, COL: %d, EVEN Dq[63:32]: 0x%x \n",
                             currentBank, currentRow[ch], col, errorDq));
            }

            errorDq = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_DATA2_S_MCDDC_CTL_REG);
            if (errorDq != 0) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "BNK: %d, ROW: %d, COL: %d, ODD Dq[31:0]: 0x%x \n",
                             currentBank, currentRow[ch], col, errorDq));
            }

            errorDq = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_DATA3_S_MCDDC_CTL_REG);
            if (errorDq != 0) {
              MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "BNK: %d, ROW: %d, COL: %d, ODD Dq[63:32]: 0x%x \n",
                             currentBank, currentRow[ch], col, errorDq));
            }

            if (host->nvram.mem.eccEn) {
              errorDq = MemReadPciCfgEp (host, socket, ch, CPGC_ERR_ECC_S_MCDDC_CTL_REG);
              if (errorDq != 0) {
                MemDebugPrint((host, SDBG_MINMAX, socket, ch, dimm, rank, NO_STROBE, NO_BIT,
                              "BNK: %d, ROW: %d, COL: %d, ECC Dq[15:0]: 0x%x \n",
                               currentBank, currentRow[ch], col, errorDq));
              }
            } // if ECC

          }// if logical Rank
        } // rank loop
      } // dimm loop
    } // if errorstatus
  } // ch loop
} // DisplayError

VOID
RunHTTest (
  PSYSHOST                    host,
  UINT8                       socket,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters
  )
/*++

  Main function that implements a single DRAM Maintenance test algorithm

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to run the DRAM Maintenance test
  @param testParameters            - Pointer to a test parameters struct
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure

  @retval N/A

--*/
{
  UINT8  ch;
  UINT8  loopCount;
  UINT8  columnBits[MAX_RANK_CH][MAX_CH]={0};
  UINT8  rowBits[MAX_RANK_CH][MAX_CH]={0};
  UINT8  bankBits[MAX_RANK_CH][MAX_CH]={0};
  UINT8  rankEnabled[MAX_RANK_CH][MAX_CH]={0};
  UINT8  dimmRank[MAX_RANK_CH][MAX_CH]={0};
  UINT8  rankChEnabled[MAX_RANK_CH]={0};
  UINT8  maxEnabledRank;
  UINT8  logical2Physical[MAX_RANK_CH][MAX_CH]={0};
  UINT8  rankChEnMap;
  UINT8  logicalRank;
  UINT32 ddrChEnabled;
  UINT8  numberHammerSubsequences;
  UINT8  testMask;
  UINT8  errorStatus;
  UINT32 rowIteration;
  UINT32 maxNumCol;
  UINT32 maxNumRow;
  UINT32 numCachelinesSubsequence[HT_NUMBER_SUBSEQUENCE_HAMMER];
  UINT32 hammerApproximation;
  UINT32 numBanks;
  UINT32 currentBank;
  UINT32 numCol;
  UINT32 numRow;
  UINT32 currRowCount;
  UINT32 startRow[MAX_CH], startCol[MAX_CH];
  UINT32 wrapRow[MAX_CH], wrapCol[MAX_CH];
  UINT32 numberRows[MAX_CH];
  UINT32 currentRow[MAX_CH];
  UINT32 neighborRows[2][MAX_CH], currentLogicalRow[MAX_CH];
  UINT8  maxEnabledSubRank;
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT  cpgcSeqBaseAddrStart[MAX_CH];
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT  cpgcSeqBaseAddrStartL0[MAX_CH];
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT   cpgcSeqBaseAddrWrap[MAX_CH];
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT   cpgcSeqBaseAddrWrapL0[MAX_CH];
  CPGC_SEQCTL0_MC_MAINEXT_STRUCT                cpgcSeqCtl0Reg[MAX_CH];
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT         cpgcSeqBaincCtl00Reg[MAX_CH];
  MC_INIT_STATE_G_MC_MAIN_HSX_BDX_STRUCT        mcInitStateG;

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Running DRAM Maintenance Test\n"));

  //
  // Phase 1: Global CPGC setup
  //

  // One loop (2^(n-1)) through the sequence (subseq numCacheLines set to test entire rank)
  loopCount = 1;
  //CpgcAdvTrainingSetup (host, socket, ddrChEnMap, 0, DDR_CPGC_MEM_TEST_PATTERN | flags, loopCount);

  for (ch = 0; ch < MAX_CH; ch++)  {
    if (!execParameters->baseInverted) {
      CpgcMemTestGlobalSetup (host, socket, ch, 0, loopCount, DDR_CPGC_MEM_TEST_ALL0);
    } else {
      CpgcMemTestGlobalSetup (host, socket, ch, 0, loopCount, DDR_CPGC_MEM_TEST_ALL0 | DDR_CPGC_MEM_TEST_INVERTPAT);
    }
  } // ch loop

  ddrChEnabled = GetChBitmaskAll(host, socket);

  //
  // Phase 2: Per logical rank setup and test
  //
  //
  // Find geometry for logical rank within channel.
  //
  MemRankGeometry (host, socket, columnBits, rowBits, bankBits, rankEnabled,
                   dimmRank, rankChEnabled, &maxEnabledRank, logical2Physical);

  hammerApproximation = CalculateSubsequenceLoopcounts(host, execParameters->numberRepetitions,
                                                       numCachelinesSubsequence, &numberHammerSubsequences);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Number of Hammer Repetitions Set: %d\n", hammerApproximation));

  for (logicalRank = 0; logicalRank <= maxEnabledRank; logicalRank++) {
    rankChEnMap = ((UINT8)testParameters->channelBitMask) & ddrChEnabled;
    rankChEnMap &= ~((rankEnabled[logicalRank][0] == 0) |
                    ((rankEnabled[logicalRank][1] == 0) << 1) |
                    ((rankEnabled[logicalRank][2] == 0) << 2) |
                    ((rankEnabled[logicalRank][3] == 0) << 3));
    ////TODO: Add here the config rank mask

    //
    // Skip to next rank if no ranks enabled on any channel
    //
    if (rankChEnMap == 0) continue;
    maxEnabledSubRank = 0;

    for (ch = 0; ch < MAX_CH; ch++) {
      if (rankEnabled[logicalRank][ch] == 0) continue;
      CpgcSetLogical2PhysicalRankMap (host, socket, ch);   //AP DEBUG change vs repercution

      if (rankChEnMap & (1 << ch)) {
        MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "Running rank %d in channel %d\n", logicalRank, ch));
      }
    } // ch loop

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((ddrChEnabled & (1 << ch)) == 0) continue;
      if ((rankChEnMap & (1 << ch)) == 0) continue;

      if (host->nvram.mem.socket[socket].channelList[ch].encodedCSMode) {
        maxEnabledSubRank = 1;
      }
    } // ch loop


    //Letting the function do some initialization that will be overwritten later.
    CpgcMemTestRankSetup (host, socket, rankChEnMap, logicalRank, maxEnabledSubRank, columnBits[logicalRank],
                          rowBits[logicalRank], bankBits[logicalRank]);                        //AP DEBUG function change

    if (host->setup.mem.DMRefreshEn == DMT_REFRESH_EN) {
      //
      // Enable Refresh
      //
      mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.refresh_enable  = 1;
      MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
      EnableRefresh(host, socket);
    } else {
      //
      // Disable Refresh
      //
      mcInitStateG.Data = MemReadPciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG);
      mcInitStateG.Bits.refresh_enable  = 0;
      MemWritePciCfgMain (host, socket, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
      DisableRefresh(host, socket);
    }

    CPGCSettingsOverride(host, socket, testParameters, execParameters, rankChEnMap, logicalRank,
                          columnBits[logicalRank], rowBits[logicalRank], bankBits[logicalRank]);

    //Setting the hammer subsequences
    SetHammerSubsequences(host, socket, testParameters, execParameters, numCachelinesSubsequence,
                          numberHammerSubsequences, columnBits[logicalRank]);
    ReadSeqCtl(host, socket, cpgcSeqCtl0Reg);
    GetRowColIncCtrl(host, socket, cpgcSeqBaincCtl00Reg);

    numBanks = 0;

    for (ch = 0; ch < MAX_CH; ch++) {
      if ((rankChEnMap & (1 << ch)) == 0) continue;

      if (((UINT32)(1 << bankBits[logicalRank][ch])) > numBanks) {
        ////TODO: Ask if we can have a case of a socket with different # of banks on different channels
        ////This will only work if the number of banks is the same.
        numBanks = 1 << bankBits[logicalRank][ch];
      }
      cpgcSeqBaseAddrStart[ch].Bits.rank_addr = logicalRank;
      cpgcSeqBaseAddrWrap[ch].Bits.rank_addr = logicalRank;
    } // ch loop

    for (currentBank = 0; currentBank < numBanks; currentBank++) {

      maxNumCol = 0;
      maxNumRow = 0;

      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Running test on bank %d\n", currentBank));

      for (ch = 0; ch < MAX_CH; ch++) {
        if ((rankChEnMap & (1 << ch)) == 0) continue;

        numCol = (1 << (columnBits[logicalRank][ch] - 3)) - 1;

        if (numCol > maxNumCol) {
          maxNumCol = numCol;
        }

        numRow = (1 << rowBits[logicalRank][ch]) - 1;
        numberRows[ch] = numRow;

        if (numRow > maxNumRow) {
          maxNumRow = numRow;
        }

        cpgcSeqBaseAddrStartL0[ch].Bits.bank_addr = currentBank;
        cpgcSeqBaseAddrWrapL0[ch].Bits.bank_addr  = currentBank;
        currentRow[ch]                          = 0;

        if (execParameters->Direction == UP) {
          startRow[ch] = testParameters->bottomAddress[ch].Row;
          wrapRow[ch]  = testParameters->topAddress[ch].Row;

          startCol[ch] = testParameters->bottomAddress[ch].Col;
          wrapCol[ch] = testParameters->topAddress[ch].Col;

          if (wrapRow[ch] > numRow) {
            wrapRow[ch] = numRow;
          }

          if (wrapCol[ch] > numCol) {
            wrapCol[ch] = numCol;
          }
        } else {
          startRow[ch] = testParameters->topAddress[ch].Row;
          wrapRow[ch]  = testParameters->bottomAddress[ch].Row;

          startCol[ch] = testParameters->topAddress[ch].Col;
          wrapCol[ch]  = testParameters->bottomAddress[ch].Col;

          if (startRow[ch] > numRow) {
            startRow[ch] = numRow;
          }

          if (startCol[ch] > numCol) {
            startCol[ch] = numCol;
          }
        }

        //Column address will always be the same
        cpgcSeqBaseAddrStartL0[ch].Bits.column_addr = 0;
        cpgcSeqBaseAddrWrapL0[ch].Bits.column_addr = numCol;
      } //ch loop

      if (testParameters->singleRowForHammer) {
        maxNumRow = 0;
      } else {
        maxNumRow--;////TODO: Check this value
      }

      testMask = rankChEnMap;

      for (currRowCount = 0; currRowCount <= maxNumRow; currRowCount++) {

        if (testMask == 0) break;

        for (ch = 0; ch < MAX_CH; ch++) {
          //If running on a single row, print on which
          if (testParameters->singleRowForHammer) {
            if (testMask & (1 << ch)) {
              MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                             "Testing Physical Row %d on channel %d\n", currentRow[ch], ch));
            }
          }
        } // ch loop

        GetLogicRows(host, socket, testParameters, execParameters, currentRow, numberRows, neighborRows, currentLogicalRow);

        for(rowIteration = 0; rowIteration < testParameters->hammerIterationsOnRow; rowIteration++) {
          //Setting the victim rows
          //Setting the first victim
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;

            WriteSeqCtlSubseqPointers(host, socket, ch, cpgcSeqCtl0Reg, HT_NUMBER_SUBSEQUENCE_HAMMER + 1, HT_NUMBER_SUBSEQUENCE_HAMMER + 1);
            //Loopcount setting is part of the subsequence
            //Setting first neighbor address
            cpgcSeqBaseAddrStart[ch].Bits.row_addr      = neighborRows[0][ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr       = neighborRows[0][ch];
            cpgcSeqBaincCtl00Reg[ch].Bits.column_ba_inc = 1;//Setting increment to move in a row
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
            WriteRowColIncCtrl(host, socket, ch, cpgcSeqBaincCtl00Reg[ch]);
          } // ch loop

          //CPGCSettingsSetTestFail ((void *) host, socket, testParameters, execParameters);
          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          //Setting the second victim
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;
            cpgcSeqBaseAddrStart[ch].Bits.row_addr = neighborRows[1][ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr  = neighborRows[1][ch];
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
          } // ch loop

          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          //Setting the aggressor row before the DRAM Maintenance test
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;

            WriteSeqCtlSubseqPointers(host, socket, ch, cpgcSeqCtl0Reg, HT_NUMBER_SUBSEQUENCE_HAMMER, HT_NUMBER_SUBSEQUENCE_HAMMER);
            //Loopcount setting is part of the subsequence
            //Setting first neighbor address
            cpgcSeqBaseAddrStart[ch].Bits.row_addr = currentLogicalRow[ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr = currentLogicalRow[ch];
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
          } // ch loop

          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          //Setting the hammer portion of the test
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;

            WriteSeqCtlSubseqPointers(host, socket, ch, cpgcSeqCtl0Reg, 0, numberHammerSubsequences - 1);
            //Loopcount setting is part of the subsequence
            //Setting first neighbor address
            cpgcSeqBaseAddrStart[ch].Bits.row_addr = currentLogicalRow[ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr  = currentLogicalRow[ch];
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
            WriteRowColIncCtrl(host, socket, ch, cpgcSeqBaincCtl00Reg[ch]);////TODO: check if we can delete this line
          } // ch loop

          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          //Running the check portion of the test
          //Checking the first victim
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;

            WriteSeqCtlSubseqPointers(host, socket, ch, cpgcSeqCtl0Reg, HT_NUMBER_SUBSEQUENCE_HAMMER + 2 , HT_NUMBER_SUBSEQUENCE_HAMMER + 2);
            //Loopcount setting is part of the subsequence
            //Setting first neighbor address
            cpgcSeqBaseAddrStart[ch].Bits.row_addr      = neighborRows[0][ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr       = neighborRows[0][ch];
            cpgcSeqBaincCtl00Reg[ch].Bits.column_ba_inc = 1;//Setting increment to move in a row
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
            WriteRowColIncCtrl(host, socket, ch, cpgcSeqBaincCtl00Reg[ch]);
          } // ch loop

          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          errorStatus = GetChErrorStatus(host, socket);
          //MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "testMask: 0x%x\n", testMask));

          if (errorStatus & testMask)  {
            DisplayError ( host, socket, logicalRank, errorStatus, currentBank, currentRow);
            MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "ERROR STATUS: %d\n", errorStatus));
          }

          //Checking the second victim
          for (ch = 0; ch < MAX_CH; ch++) {
            if ((rankChEnMap & (1 << ch)) == 0) continue;

            cpgcSeqBaseAddrStart[ch].Bits.row_addr = neighborRows[1][ch];
            cpgcSeqBaseAddrWrap[ch].Bits.row_addr  = neighborRows[1][ch];
            WriteTestStartAndWrap(host, socket, ch, cpgcSeqBaseAddrStart[ch], cpgcSeqBaseAddrStartL0[ch], cpgcSeqBaseAddrWrap[ch], cpgcSeqBaseAddrWrapL0[ch]);
          } // ch loop

          CpgcClearGlobalErrorStatus (host, socket);
          CpgcAdvTrainingExecTest (host, socket, rankChEnMap, DDR_CPGC_MEM_TEST_PATTERN);//Check that this execute works for me

          errorStatus = GetChErrorStatus(host, socket);
          if (errorStatus & testMask)  {
            DisplayError ( host, socket, logicalRank, errorStatus, currentBank, currentRow);
            MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "ERROR STATUS: %d\n", errorStatus));
          }

        } // rowIteration loop

        //Only enable channels on which row is in range and is populated
        for (ch = 0; ch < MAX_CH; ch++) {
          if ((rankChEnMap & (1 << ch)) == 0) continue;

          //First enable all populated channels
          if (execParameters->Direction == UP)  {
            if (currentRow[ch] < wrapRow[ch]) {
              currentRow[ch] = currentRow[ch] + 1;
            } else if (testMask & (1 << ch)) {
              //Channel  row is out of range
              //disable it
              CpgcDisableGlobalStart (host, socket, ch);
              testMask = (testMask & (~(1 << ch)));
            }
          } else {
            //going down
            if (currentRow[ch] > wrapRow[ch]) {
              currentRow[ch] = currentRow[ch] - 1;
            } else if (testMask & (1 << ch)) {
              CpgcDisableGlobalStart (host, socket, ch);
              testMask = (testMask & (~(1 << ch)));
            }
          }
        } // ch loop
      } // currRowCount loop
    } // currentBank loop
  } // logicalRank loop
} // RunHTTest


VOID
DcttHtToolVersion (
  UINT16 *majorVer,
  UINT16 *minorVer,
  UINT32 *rev
  )
/*++

  Returns the version of the DRAM Maintenance test algorithm implementation

  @param majorVer                  - Pointer to a UINT16 where the major version will be written
  @param minorVer                  - Pointer to a UINT16 where the minor version will be written
  @param rev                       - Pointer to a UINT32 where the revision version will be written

  @retval N/A

--*/
{
#ifdef TOOL_MAJOR_VERSION
  *majorVer = TOOL_MAJOR_VERSION;
#else
  *majorVer = 0;
#endif
#ifdef TOOL_MINOR_VERSION
  *minorVer = TOOL_MINOR_VERSION;
#else
  *minorVer = 0;
#endif
#ifdef TOOL_REVISION
  *rev = TOOL_REVISION;
#else
  *rev = 0;
#endif
} // DcttHtToolVersion

VOID
PrintWDBPattern (
  PSYSHOST            host,
  BackgroundBaseTypes background
)
/*++

  Helper function used to print the test background type

  @param host                      - Pointer to sysHost
  @param background                - Test background enumeration

  @retval N/A

--*/
{
  switch(background) {
    case BACKBASE_ZEROS:
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "0\n"));
      break;
    case BACKBASE_ONES:
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "1\n"));
      break;
    case BACKBASE_FIVE:
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "5\n"));
      break;
    case BACKBASE_A:
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "A\n"));
      break;
    default:
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "UNKNOWN\n"));
      break;
  }
} // PrintWDBPattern


INT8
*getBackgroundPattern (
  UINT8 patternType
  )
/*++

  Helper function that returns a background pattern string given the backgound enumeration value

  @param patternType                - Test background pattern type

  @retval String containing the backgorund pattern type

--*/
{
  switch(patternType) {
    case BACKGROUND_PATTERN_SOLID: return "SOLID";
    case BACKGROUND_PATTERN_COL_STRIPES: return "COL STRIPES";
    case BACKGROUND_PATTERN_ROW_STRIPES: return "ROW STRIPES";
    case BACKGROUND_PATTERN_CHECKERBOARD: return "CHECKERBOARD";
    default: return "UNKNOWN";
  }
} // getBackgroundPattern



VOID
PrintHeader (
  PSYSHOST               host,
  UINT8                  socket,
  dmtlpConfigParameters *testParameters
  )
/*++

  Helper function used to print some basic test information

  @param host                      - Pointer to sysHost
  @param socket                      - Node on which we are going to run the DRAM Maintenance test
  @param testParameters            - Pointer to a test parameters struct

  @retval N/A

--*/
{

  UINT16 majorVer, minorVer;
  UINT32 rev;


  DcttHtToolVersion(&majorVer, &minorVer, &rev);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Starting HSX Memory Tester\n"));
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Memory Traversing Order: COL, ROW, BANK, RANK\n"));
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Values Written to WDB: "));

  PrintWDBPattern(host, testParameters->backgroundBase);
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Memory Tester Version: %d.%d.%d\n", majorVer, minorVer, rev));

} // PrintHeader

VOID
PrintDirection (
  PSYSHOST                    host,
  HammerSymbolExecParameters *execParameters
  )
/*++

  Helper function used to print the test direction

  @param host                      - Pointer to sysHost
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure

  @retval N/A

--*/
{

  if (execParameters->Direction == UP) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Direction Function: Changing Test direction to UP\n"));
  } else {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Direction Function: Changing Test direction to DOWN\n"));
  }

} // PrintDirection

VOID
PrintHammerParameters (
  PSYSHOST                    host,
  dmtlpConfigParameters      *testParameters,
  HammerSymbolExecParameters *execParameters
  )
/*++

  Helper function used to print the row hammer test parameters

  @param host                      - Pointer to sysHost
  @param testParameters            - Pointer to a test parameters struct
  @param execParameters            - Pointer to a DRAM Maintenance test parameters structure

  @retval N/A

--*/
{

  if (execParameters->baseOperation == CO_WRITE) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "W"));
  } else {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "R"));
  }

  if (execParameters->baseInverted) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "I"));
  } else {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT, "D"));
  }

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Fast Hammer Number of repetitions: %d\n", execParameters->numberRepetitions));
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Checking Row Neighbors, Row Swizzle: %d\n",testParameters->rowSwizzleType));
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "DMRefresh status: "));
  if (host->setup.mem.DMRefreshEn == DMT_REFRESH_EN) {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Enabled\n"));
  } else {
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "Disabled\n"));
  }
  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                 "Number of Hammer Iterations per Row: %d\n", testParameters->hammerIterationsOnRow));

} // PrintHammerParameters

INT32
DcttHt (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Entry point for the single DRAM Maintenance test

  @param host                      - Pointer to sysHost
  @param socket                    - Socket on which we are going to run the DRAM Maintenance test

  @retval SUCCESS
  @retval FAILURE

--*/
{
  UINT8                         ch;
  UINT8                         mcId;
  INT8                          *testToRun;
  UINT8                         testNumber;
  UINT32                        mcMtrSave[2];
  UINT32                        ddrChEnabled;
  HammerSymbolExecParameters    execParameters;
  dmtlpConfigParameters         testParameters;
  struct channelNvram           (*channelNvList)[MAX_CH];
  MCMTR_MC_MAIN_HSX_BDX_STRUCT  mcMtr;

  //
  // Enter CPGC mode
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    mcMtr.Data = MemReadPciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG);
    mcMtrSave[mcId] = mcMtr.Data;
    mcMtr.Bits.trng_mode = 0x01; // CPGC_MODE
    MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } // mcId loop

  //
  // Get the channel bit mask
  //
  ddrChEnabled = GetChBitmaskAll(host, socket);

  //
  // Save the state of the CPGC test
  //
  SaveIOTestState(host, socket, ddrChEnabled);

  InitializeConfig( &testParameters);
  PrintHeader(host, socket, &testParameters);

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "START DRAM Maintenance Test \n"));

  //TODO: what is this for?
  //host->var.mem.serialDebugMsgLvl &= ~(UINT8)BIT3;

  channelNvList = GetChannelNvList(host, socket);

  if (host->nvram.mem.eccEn == 0) {
    for (ch = 0; ch < MAX_CH; ch++) {
      if ((*channelNvList)[ch].enabled == 0) continue;

      //
      // Mask off the ECC lanes if ECC is disabled
      //
      MemWritePciCfgEp (host, socket, ch, CPGC_ERR_ECC_MCDDC_CTL_REG, 0xFF);

    } // ch loop
  } // if ECC

  for(testNumber = 0; testNumber < testParameters.numberTests; testNumber++) {
    testToRun = testParameters.testStrings[testNumber];

    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Running Test %i: %s\n", testNumber, testToRun));
    MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Pattern: %s\n", getBackgroundPattern(testParameters.startBackground)));

    if(ParseTest(host, socket, testToRun, &testParameters, &execParameters) == SUCCESS) {

      PrintDirection(host, &execParameters);
      PrintHammerParameters(host, &testParameters, &execParameters);

      MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Executing Test\n"));
      RunHTTest(host, socket, &testParameters, &execParameters);
    } else {
      MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "WARNING: Couldn't parse test %d \n", testNumber));
    }
  } // testNumber loop

  MemDebugPrint((host, SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "STOP DRAM Maintenance Test \n"));

  //
  // Step 1: Write all 0's pattern
  //
  CpgcMemTest (host, socket, ddrChEnabled, DDR_CPGC_MEM_TEST_WRITE | DDR_CPGC_MEM_TEST_ALL0);

  //
  // Restore the state of the CPGC test
  //
  RestoreIOTestState(host, socket, ddrChEnabled);

  //
  // Restore MCMTR
  //
  for (mcId = 0; mcId < MAX_IMC; mcId++) {
    if(host->var.mem.socket[socket].imcEnabled[mcId] == 0) continue;

    MemWritePciCfgMC (host, socket, mcId, MCMTR_MC_MAIN_REG, mcMtrSave[mcId]);
  } // mcId loop


  return SUCCESS;
} // DcttHt
