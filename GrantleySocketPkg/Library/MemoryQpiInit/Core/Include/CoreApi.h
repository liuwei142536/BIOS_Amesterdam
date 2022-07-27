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
 *      This file contains API declarations defined in the MRC Core layer.
 *
 ************************************************************************/
#ifndef  _CORE_API_H_
#define  _CORE_API_H_

#include "DataTypes.h"
#include "SysHost.h"
#include "CommonAccessType.h"

UINT32
MemStart(
    PSYSHOST host          // Pointer to the system host (root) structure
    );

VOID
MemoryInit(
    struct sysHost   *host        // Pointer to the system host (root) structure
    );

BOOLEAN
CheckMemPresentStatus(
    PSYSHOST host          // Pointer to the system host (root) structure
    );

void
DisableChannel(
    PSYSHOST host,              // Pointer to the system host (root) structure
    UINT8    socket,              // socket number
    UINT8    ch                   // channel number
    );

void
FastestCommon(
    UINT8 *time1,   // 1st time to check
    UINT8 *time2,   // 2nd time to check
    UINT8 *time3    // 3rd time to check (ignored if 0)
    );

void
FastestCommon16(
    UINT16 *time1,  // 1st time to check
    UINT16 *time2,  // 2nd time to check
    UINT16 *time3   // 3rd time to check (ignored if 0)
    );

UINT32
GetChBitmask(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // socket number
    UINT8    dimm,      // dimm number
    UINT8    rank       // rank number
    );

UINT32
GetChBitmaskAll(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket     // socket number
    );

UINT8
GetClosestFreq(
    PSYSHOST host,              // Pointer to the system host (root) structure
    UINT8    socket               // socket number
    );

void
GetDimmPresent(
    PSYSHOST host,                    // Pointer to the system host (root) structure
    UINT8     socket,                     // Processor socket to check
    UINT8     *dimmMask[MAX_CH]   // Pointer to results
    );

void
GetRankPresent(
    PSYSHOST host,                    // Pointer to the system host (root) structure
    UINT8     socket,                     // Processor socket to check
    UINT8     *rankMask[MAX_CH]     // Pointer to results
    );

#ifdef SERIAL_DBG_MSG
void
InitUSBDebug(
    PSYSHOST host               // Pointer to the system host (root) structure
    );
#endif //SERIAL_DBG_MSG

UINT32
MemInit(
    PSYSHOST host               // Pointer to the system host (root) structure
    );

UINT32
DetectDIMMConfig(
    PSYSHOST host               // Pointer to the system host (root) structure
    );

UINT8
MemReadSmb(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,         // socket number
    struct smbDevice  dev,            // SMB device structure
    UINT16               byteOffset,     // offset of data to read
    UINT8               *data           // Pointer to data to be returned
    );

UINT8
MemWriteSmb(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    struct smbDevice  dev,            // SMB device structure
    UINT8               byteOffset,     // offset of data to read
    UINT8               *data           // Pointer to data to be returned
    );

UINT16
TimeBaseToDCLK(
    PSYSHOST   host,              // Pointer to host structure (outside scope of this spec)
    UINT8   socket,             // Processor socket within the system (0-based)
    UINT8       ch,                 // DDR channel number within the processor socket (0-based)
    UINT8     dimm,       // DIMM number within a DDR channel (0-based, farthest slot == 0)
    UINT16      mtbTime,    // Time in MTB (Medium Time Base)
    UINT16      ftbTime     // Time in FTB (Fine Time Base)
    );

UINT32
DetectDDR3DIMMConfig(
    PSYSHOST    host, // Pointer to host structure (outside scope of this spec)
    UINT8     socket, // Processor socket within the system (0-based)
    UINT8     ch, // DDR channel number within the processor socket (0-based)
    UINT8     dimm // DIMM number within a DDR channel (0-based, farthest slot == 0)
    );

UINT32
GatherSPDDataDDR3(
    PSYSHOST host               // Pointer to the system host (root) structure
    );

void
InitDDRVoltageDDR3(
    PSYSHOST  host,               // Pointer to the system host (root) structure
    UINT8     socket        // socket number
    );

UINT32
DetectDDR4DIMMConfig(
    PSYSHOST    host, // Pointer to host structure (outside scope of this spec)
    UINT8     socket, // Processor socket within the system (0-based)
    UINT8     ch, // DDR channel number within the processor socket (0-based)
    UINT8     dimm // DIMM number within a DDR channel (0-based, farthest slot == 0)
    );

void
InitDDRVoltageDDR4(
    PSYSHOST  host,               // Pointer to the system host (root) structure
    UINT8     socket        // socket number
    );

UINT32
DetectAepDIMMConfig(
    PSYSHOST    host, // Pointer to host structure (outside scope of this spec)
    UINT8     socket, // Processor socket within the system (0-based)
    UINT8     ch, // DDR channel number within the processor socket (0-based)
    UINT8     dimm // DIMM number within a DDR channel (0-based, farthest slot == 0)
    );

UINT32
MemTest(
    PSYSHOST host               // Pointer to the system host (root) structure
    );

UINT32
AdvancedMemTest(
    PSYSHOST host               // Pointer to the system host (root) structure
    );

void
ExecuteCmdSignalTest(
    PSYSHOST  host,   // Pointer to host structure (outside scope of this spec)
    UINT8     socket,   // Processor socket within the system (0-based)
    UINT32    chBitmask          // List of channels that were tested
    );

UINT32
AggressiveCMDTest(
    PSYSHOST host,                    // Pointer to the system host (root) structure
    UINT8     socket                                        // Socket Id
    );

UINT32
PerCMDGroup(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8     socket,   // Socket Id
    UINT16    testCase  // Test Case:
    //  1 =  COMPOSITE_GROUPS
    //   0 = Group 0
    );

void
ExecuteTest(
    PSYSHOST host,              // Pointer to the system host (root) structure
    UINT8     socket,     // Socket Id
    UINT32    ddrChEnMap, // Bit-mask of enabled channels to test. 1 = Enabled, 0 = Skipped
    UINT8     dimm,               // Target DIMM to test
    UINT8     rank,               // Target Rank to test
    GSM_GT    group,            // Parameter to be margined
    //   RxDqsDelay    : Margin Rx DqDqs
    //   TxDqDelay     : Margin Tx DqDqs
    //   RxVref         : Margin Rx Vref
    //   TxVref         : Margin Tx Vref
    //   RecEnDelay    : Margin Receive Enable
    //   WrLvlDelay    : Margin Write Level
    //   CmdGrp0        : Margin CMD group 0
    //   CmdGrp1        : Margin CMD group 1
    //   CmdGrp2        : Margin CMD group 2
    //   CmdAll         : Margin all CMD groups
    UINT16    mode,               // Margin mode to use
    //   MODE_XTALK            BIT0: Enable crosstalk (placeholder)
    //   MODE_VIC_AGG          BIT1: Enable victim/aggressor
    //   MODE_START_ZERO       BIT2: Start at margin 0
    //   MODE_POS_ONLY         BIT3: Margin only the positive side
    //   MODE_NEG_ONLY         BIT4: Margin only the negative side
    //   MODE_DATA_MPR         BIT5: Enable MPR Data pattern
    //   MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
    //   MODE_DATA_LFSR        BIT7: Enable Data LFSR
    //   MODE_ADDR_LFSR        BIT8: Enable Address LFSR
    //   MODE_ADDR_CMD0_SETUP  BIT9
    //   MODE_CHECK_PARITY     BIT10: Enable parity checking
    //   MODE_DESELECT         BIT11: Enable deselect patterns
    //   MODE_VA_DESELECT BIT12: Enable Victim - Aggressor deselect
    // patterns
    UINT32   patternLength,                   // Number of sequences to test (units of burstLength cache lines)
    UINT8     burstLength                     // Number of cachelines to test for each write/read operation.
    //   It is the repeat count for the WR/RD subsequence
    );

UINT8
CheckTestResults(
    PSYSHOST host,                    // Pointer to the system host (root) structure
    UINT8     socket,                                         // Socket Id
    UINT32    chBitmask,                                      // List of channels that were tested
    UINT8     dimmRank[MAX_RANK_CH][MAX_CH],  // DIMM containing logicalRank
    UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],  // Geometery for logical rank within channel
    UINT8     logicalRank,                                    // Logical rank
    BOOLEAN   secondPass                                    // Second Pass check: 0 = 1st Pass, 1 = 2nd Pass
    );

UINT32
CheckTestResultsMATS(
    PSYSHOST host,                                            // Pointer to the system host (root) structure
    UINT8     socket,                                         // Socket Id
    UINT32    chBitmask,                                      // List of channels that were tested
    UINT8     dimmRank[MAX_RANK_CH][MAX_CH],                  // DIMM containing logicalRank
    UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],          // Geometery for logical rank within channel
    UINT8     logicalRank,                                    // Logical rank
    UINT8     testType                                        // Memory test type for logging purposes
    );

UINT32
CollectTestResults(
    PSYSHOST  host,             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,           // Processor socket within the system (0-based)
    UINT32    chToCheck,        // channel to check
    UINT32    bwSerr[MAX_CH][3] // Test error status; 72 bits per channel for ECC modules
    );

UINT32
RdAdvancedCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WrAdvancedCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
RdVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WrVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WrLRDIMMVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
RdEarlyVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WrEarlyVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
CmdVrefCentering(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
LRDIMMVrefCentering(
    PSYSHOST  host,    // Pointer to host structure (outside scope of this spec)
    UINT8     socket,             // Processor socket to check
    GSM_GT    group,      // Parameter to be margined
    GSM_LT    level,            // DDR or LRDIMM Host side, LRDIMM Backside, etc...
    UINT8     perRankCentering // 0 = false, 1 = true (if false, PDA training is done)
    );

UINT32
WriteLeveling(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WriteLevelingCleanUp(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

void
InitStartingOffset(
    PSYSHOST  host,  // Pointer to host structure (outside scope of this spec)
    UINT8     socket // Processor socket to check
    );

UINT32
RxPerBitDeskew(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
TxPerBitDeskew(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
ReadDqDqs(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
WrDqDqs(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
WriteDqDqsPerBit(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
DqSwizzleDiscovery(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
FindErrorGraphMin(
    PSYSHOST  host,                 // Pointer to the system host (root) structure
    UINT8     socket,               // Processor socket to check
    UINT8     ch,                   // Current Channel
    UINT8     dimm,                 // Dimm number (0-based)
    UINT8     rank,                 // Rank number (0-based)
    UINT8     strobe,                   // Strobe number (0-based)
    UINT8     *SdRamErrorSamples,     // Array with 64 sampes for each dqs pi setting
    UINT16    ErrorSamplesSize,         // Include how many sample we have in the array 72 or 64
    UINT16    *errorStart,              // Pointer to location for error start
    UINT16    *errorEnd,                // Pointer to location for error end
    UINT16    *piPosition               // Pointer to the PI Position
    );

UINT32
ReceiveEnable(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
RoundTripOptimize(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

void
FindMaxMinLogic(
    PSYSHOST  host,                 // Pointer to the system host (root) structure
    UINT8     socket,               // Processor socket to check
    UINT8     ch,                   // Current Channel
    UINT8     dimm,                 // Dimm number (0-based)
    UINT8     rank,                 // Rank number (0-based)
    UINT8     MaxSdramInDimm,     // Number of sdram in the current DIMM
    UINT8     *MaxLogicDelay,     // Return logic delay max value
    UINT8     *MinLogicDelay      // Return logic delay min value
    );

UINT32
ConfigureXMP(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32 WaitForConfigureXMP(
    PSYSHOST host     // Pointer to the system host (root) structure
    );

UINT8
GetMaxStrobe(
    PSYSHOST  host,    // Pointer to host structure (outside scope of this spec)
    UINT8     socket,     // Socket
    UINT8     ch,           // Channel
    UINT8     dimm,       // DIMM
    GSM_GT    group,      // Group type
    GSM_LT    level       // Level type
    );

UINT8
CountBitsEqOne(
    UINT32 Input    // The 32 bit value to check
    );

UINT8
CoreSetVdd(
    PSYSHOST host,    // Pointer to host structure (outside scope of this spec)
    UINT8    socket   // processor number
    );

void
CoreReleaseADRClamps(
    PSYSHOST host,    // Pointer to host structure (outside scope of this spec)
    UINT8 socket      // processor number
    );

void
CoreCheckPORCompat(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

void
CoreLookupFreqTable(
    PSYSHOST host,                                // Pointer to host structure (outside scope of this spec)
    struct DimmPOREntryStruct **freqTablePtr,     // Pointer to Intel POR memory frequency table.
    UINT16 *freqTableLength                       // Pointer to number of DimmPOREntryStruct entries in
    //    the POR Frequency Table.
    );

#ifdef MEM_NVDIMM_EN
UINT32
CoreDetectNVDIMM(
    PSYSHOST  host,             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,           // Processor socket within the system (0-based)
    UINT8     ch,               // DDR channel number within the processor socket (0-based)
    UINT8     dimm,             // DIMM number within a DDR channel (0-based, farthest slot == 0)
    struct smbDevice  spd       // Structure holding the SPD data
);

UINT32
CoreNVDIMMStatus(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

UINT32
CoreRestoreNVDIMMs(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

UINT32
CoreArmNVDIMMs(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
);
#endif

void
CoreGetSetupOptions(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

void
CoreGetCpuSetupOptions(
    struct sysSetup *setup  // pointer to setup area of host structure
    );

void
GetMemSetupOptionsCore(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

VOID
OemCheckAndHandleResetRequestsCore(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

VOID
OemPublishDataForPostCore(
    PSYSHOST host   // Pointer to host structure (outside scope of this spec)
    );

void
ResetSystem(
    PSYSHOST host,   // Pointer to host structure (outside scope of this spec)
    UINT8    resetType    // Reset type
    );

UINT32
SetDDRFreq(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket   // Socket Id
    );

void
GetMargins(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    GSM_LT          level,  // DDR or LRDIMM Host side, LRDIMM Backside, etc...
    GSM_GT          group,    // Parameter to be margined:
    //   RxDqsDelay    : Margin Rx DqDqs
    //   TxDqDelay     : Margin Tx DqDqs
    //   RxVref         : Margin Rx Vref
    //   TxVref         : Margin Tx Vref
    //   RecEnDelay    : Margin Receive Enable
    //   WrLvlDelay    : Margin Write Level
    //   CmdGrp0        : Margin CMD group 0
    //   CmdGrp1        : Margin CMD group 1
    //   CmdGrp2        : Margin CMD group 2
    //   CmdAll         : Margin all CMD groups
    UINT16          mode,   // Test mode to use
    //   MODE_XTALK            BIT0: Enable crosstalk (placeholder)
    //   MODE_VIC_AGG          BIT1: Enable victim/aggressor
    //   MODE_START_ZERO       BIT2: Start at margin 0
    //   MODE_POS_ONLY         BIT3: Margin only the positive side
    //   MODE_NEG_ONLY         BIT4: Margin only the negative side
    //   MODE_DATA_MPR         BIT5: Enable MPR Data pattern
    //   MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
    //   MODE_DATA_LFSR        BIT7: Enable Data LFSR
    //   MODE_ADDR_LFSR        BIT8: Enable Address LFSR
    //   MODE_ADDR_CMD0_SETUP  BIT9
    //   MODE_CHECK_PARITY     BIT10: Enable parity checking
    //   MODE_DESELECT         BIT11: Enable deselect patterns
    //   MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns
    UINT8           scope,    // Margin granularity
    //   SCOPE_SOCKET      0: Margin per processor socket
    //   SCOPE_CH        1: Margin per channel
    //   SCOPE_DIMM      2: Margin per DIMM
    //   SCOPE_RANK      3: Margin per rank
    //   SCOPE_STROBE    4: Margin per strobe group
    //   SCOPE_BIT       5: Margin per bit
    struct bitMask *mask, // Mask of bits to exclude from testing
    void *marginData,       // Pointer to the structure to store the margin results. The structure type varies // based on scope.
    UINT32          patternLength,    // RankMarginTest Pattern length
    UINT8           update,           // Update starting margins so the next pass will run faster or not
    UINT8           chMask,           // Bit Mask of channels to not be used
    UINT8           rankMask,         // Bit Mask of ranks to not be used
    UINT8           burstLength       // Number of cachelines to test for each write/read operation.  It is the
    // repeat count for the WR/RD subsequence
    );

UINT32
RankMarginTool(
    PSYSHOST  host                // Pointer to the system host (root) structure
    );

UINT32
SetOffset(
    PSYSHOST  host,               // Pointer to the system host (root) structure
    UINT8     socket,             // Processor socket to check
    UINT8     ch,                 // Current Channel
    UINT8     dimm,               // Dimm number (0-based)
    UINT8     rank,               // Rank number (0-based)
    INT16     offset,             // Pointer to offset
    GSM_LT    level,              // DDR or LRDIMM Host side, LRDIMM Backside, etc...
    GSM_GT    group,            // Test group
    INT16     previousMargin    // Previous Margin
    );

void
RestoreOffset(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8     ch,       // channel number
    UINT8     dimm,     // DIMM number
    UINT8     rank,     // rank number
    INT16     offset,   // Offset
    GSM_LT    level,    // DDR or LRDIMM Host side, LRDIMM Backside, etc...
    GSM_GT    group     // Test group
    );

void
TrainingResultOffsetFunction (
    PSYSHOST  host,
    UINT8     socket,
    UINT8     ch,
    UINT8     dimm,
    UINT8     rank,
    INT16     offset,
    GSM_LT    level,
    GSM_GT    group
    );

void
DisplayPerBitMargins(
    PSYSHOST          host,           // Pointer to the system host (root) structure
    UINT8             socket,         // current socket
    struct bitMargin  *resultsBit,    // Per bit margin results
    GSM_GT            group           // Command group
    );

void
DisplayREandLEMargins (
  PSYSHOST          host,
  UINT8             socket,
  UINT8             current_rank,
  struct bitMargin  *resultsBit,
  GSM_GT            group
  );

void
ClearFilter(
    PSYSHOST host,        // Pointer to the system host (root) structure
    UINT8           socket,     // Socket Id
    struct bitMask  *filter     // pointer to filter buffer
    );

void
GetTTVar(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    TT_VARS     *ttVars   // Structure that holds the variables
    );


UINT8
GetTccd(
    PSYSHOST  host,       // Pointer to the system host (root) structure
    UINT8     socket    // Socket Id
    );

UINT8
CalctWWDD(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    UINT8       minWWDR,  // Minimum value for WWDD
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctRWSR(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    UINT32  specMin,  // Minimum specified value for RWDR
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctRWDR(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    UINT32  specMin,  // Minimum specified value for RWDR
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctRWDD(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctWRDR(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctWRDD(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    TT_VARS     *ttVars   // Structure that holds the variables
    );

UINT8
CalctWWDR(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch,               // Current Channel
    UINT8       minWWDR,  // Minimum value for WWDR
    TT_VARS     *ttVars   // Structure that holds the variables
    );

void
RestoreTimings(
    PSYSHOST    host,               // Pointer to the system host (root) structure
    UINT8       socket,           // Processor socket to check
    UINT8       ch                // Current Channel
    );

UINT8
CheckCMDGroup(
    GSM_GT  group // Command Group
    );

UINT8
CheckCTLGroup(
    GSM_GT  group // Control Group
    );

void
EnableParityChecking(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket   // Socket Id
    );

void
DisableParityChecking(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket   // Socket Id
    );

void
GetCmdMarginsSweep(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8     dimm,       // DIMM number
    UINT8     rank,       // rank number
    GSM_CSN   signal,     // Signal
    INT16     *le,        // Left edge
    INT16     *re         // Right edge
    );

void
GetCmdMarginsSweepGroup(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8     dimm,       // DIMM number
    UINT8     rank,       // rank number
    GSM_GT    group,  // Group to sweep
    INT16     *le,        // Left edge
    INT16     *re         // Right edge
    );

UINT8
GetLogicalRank(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8     ch,       // channel number
    UINT8     dimm,       // DIMM number
    UINT8     rank      // rank number
    );

UINT32
NormalizeCCC(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket   // Socket Id
    );

void
SetNormalCMDTiming(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8     socket  // Socket Id
    );

void
UpdateEdges(
    INT16 le,        // Left edge
    INT16 re,        // Right edge
    INT16 *compLe,   // Composite left edge
    INT16 *compRe    // Composite right edge
    );

void
UpdateMinMaxInt(
    INT16 val,          // New value to evaluate
    INT16 *minVal,    // Current minimum value
    INT16 *maxVal     // Current maximum value
    );

void
FixedQClkDelay(
    PSYSHOST host,              // Pointer to the system host (root) structure
    UINT32   QClk      // Number of QClks to delay
    );

UINT32
EarlyCmdClk(
    PSYSHOST host       // Pointer to the system host (root) structure
    );

UINT32
EarlyCtlClk(
    PSYSHOST host       // Pointer to the system host (root) structure
    );

UINT32
LateCmdClk(
    PSYSHOST host       // Pointer to the system host (root) structure
    );

UINT8
CheckParityWorkaround(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8     ch,   // Channel
    UINT8     dimm    // DIMM
    );

void
FixedDelay(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT32   usDelay    // number of us units to delay
    );

UINT32
GetCount(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
GetDelay(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT32   startCount   // 24-bit counter value from GetCount()
    );

void
InitPmTimer(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
JedecInitSequence(
    PSYSHOST      host,         // Pointer to the system host (root) structure
    UINT8         socket,       // Socket to initizlize
    UINT32        chBitMask     // Bit Mask of Channels Present on the Socket
    );

void
JedecInitDdrAll(
    PSYSHOST   host,         // Pointer to the system host (root) structure
    UINT8      socket,       // Socket to initizlize
    UINT32     chBitMask     // Bit Mask of Channels Present on the Socket
    );

UINT16
MirrorAddrBits(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT16   data     // Data for the MRS write
    );

UINT8
MirrorBankBits(
    PSYSHOST  host,       // Pointer to the system host (root) structure
    UINT8     bank        // Bank address to be mirrored
    );

UINT16
InvertAddrBits(
    UINT16 data   // Address to be inverted
    );

UINT8
InvertBankBits(
    UINT8 bank  // Bank address to be inverted
    );

UINT8
DetectTRRSupport(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch             // Channel number (0-based)
    );

void
DoRegisterInit(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8    socket,          // Socket Id
    UINT8    ch,              // Channel number (0-based)
    UINT8    dimm,            // Dimm number (0-based)
    UINT8    rank           // Rank number (0-based)
    );

void
DoRegisterInitDDR4(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8    socket,          // Socket Id
    UINT8    ch,              // Channel number (0-based)
    UINT8    dimm,            // Dimm number (0-based)
    UINT8    rank           // Rank number (0-based)
    );

void
DoRegisterInitAep(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8    socket,          // Socket Id
    UINT8    ch,              // Channel number (0-based)
    UINT8    dimm           // Dimm number (0-based)
    );

void
SetRankMPR(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8    socket,          // Socket Id
    UINT8    ch,              // Channel number (0-based)
    UINT8    dimm,            // Dimm number (0-based)
    UINT8    rank,            // Rank number (0-based)
    UINT8     mprMode // Either 0 for Disable or MR3_MPR to Enable
    );

UINT32
ReadLrbufSmb(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch,            // Channel number (0-based)
    UINT8    dimm,          // DIMM number (0-based)
    UINT16   lrbufAddr,     // Address for the read
    UINT32   *lrbufData     // Pointer to the LR Buffer data
    );

UINT32
WriteLrbufSmb(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch,            // Channel number (0-based)
    UINT8    dimm,          // DIMM number (0-based)
    UINT8    size,          // number of bytes to be written
    UINT16   lrbufAddr,     // Address for the read
    UINT32   *lrbufData     // Pointer to the LR Buffer data
    );

void
WriteLrbuf(
    PSYSHOST host,                // Pointer to the system host (root) structure
    UINT8    socket,                // Socket Id
    UINT8    ch,                    // Channel number (0-based)
    UINT8    dimm,                  // Dimm number (0-based)
    UINT8    rank,                  // Rank number (0-based)
    UINT8    controlWordData,     // Control Word data
    UINT8    controlWordFunc,     // Function index in the LR buffer
    UINT8    controlWordAddr      // Control Word Address
    );

void
WriteLrbufPBA(
    PSYSHOST host,                // Pointer to the system host (root) structure
    UINT8    socket,              // Socket Id
    UINT8    ch,                  // Channel number (0-based)
    UINT8    dimm,                // Dimm number (0-based)
    UINT8    rank,                // Rank number (0-based)
    UINT8    *controlWordDataPtr,   // Pointer to a 9 byte array of control word data
    UINT8    controlWordFunc,         // Function index in the LR buffer
    UINT8    controlWordAddr,         // Control Word Address
    UINT8    bufferAddr               // Address of the individual buffer to target...or 'FF' for all
    // buffers
    );

void
WriteMRS(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8    ch,          // Channel number (0-based)
    UINT8    dimm,      // DIMM number (0-based)
    UINT8    rank,      // Rank number (0-based)
    UINT16   data,      // Data for the MRS write
    UINT8    bank       // Bank address for the MRS write
    );

void
WriteMrsDdr4(
    PSYSHOST host,      // Pointer to the system host (root) structure
    UINT8    socket,    // Socket Id
    UINT8    ch,          // Channel number (0-based)
    UINT8    dimm,      // DIMM number (0-based)
    UINT8    rank,      // Rank number (0-based)
    UINT16   data,      // Data for the MRS write
    UINT8    bank,      // Bank address for the MRS write
    UINT8    side       // Side
    );

void
WriteRC(
    PSYSHOST  host,               // Pointer to the system host (root) structure
    UINT8     socket,               // Socket Id
    UINT8     ch,                   // Channel number (0-based)
    UINT8     dimm,               // DIMM number (0-based)
    UINT8     rank,                 // Rank number (0-based)
    UINT8     controlWordData,    // Control Word data (a nibble)
    UINT8     controlWordAddr     // Control Word Address
    );

void
WriteBC(
    PSYSHOST  host,               // Pointer to the system host (root) structure
    UINT8     socket,               // Socket Id
    UINT8     ch,                   // Channel number (0-based)
    UINT8     dimm,               // DIMM number (0-based)
    UINT8     rank,                 // Rank number (0-based)
    UINT8     controlWordData,    // Control Word data (a nibble or byte)
    UINT8     controlWordFunc,    // Control Word Function
    UINT8     controlWordAddr     // Control Word Address
    );

void
WriteRcLrbuf(
    PSYSHOST host,                // Pointer to the system host (root) structure
    UINT8    socket,              // Socket Id
    UINT8    ch,                  // Channel number (0-based)
    UINT8    dimm,                // Dimm number (0-based)
    UINT8    rank,                  // Rank number (0-based)
    UINT8    controlWordData,   // Control Word data
    UINT8    controlWordFunc,   // Function index in the LR buffer
    UINT8    controlWordAddr    // Control Word Address
    );

void
WriteRcLrbufExtended(
    PSYSHOST host,            // Pointer to the system host (root) structure
    UINT8    socket,          // Socket Id
    UINT8    ch,              // Channel number (0-based)
    UINT8    dimm,            // Dimm number (0-based)
    UINT8    rank,            // Rank number (0-based)
    UINT8    lrbufData,       // 8-bit data to write
    UINT16   lrbufAddr        // 16-bit flat address space (4-bit function, 12-bit register offset)
    );

UINT32
IsTermRank(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch,            // Channel number (0-based)
    UINT8    dimm,    // DIMM number
    UINT8    rank     // Rank number
    );

UINT16 
ConvertOdtValue(
    GSM_GT group,           // Group type 
    UINT16 value,           // Value from Register/Cache
    UINT8  code,            // CODE or DECODE ODT
    UINT8  dimmtype         // Dimm type
    );

void
SetOdtMatrix(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch             // Channel number (0-based)
    );

ODT_VALUE_INDEX
GetOdtValueIndex(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch,            // Channel number (0-based)
    UINT8    freqLimit    // Frequency Limit
    );

UINT8
GetMcOdtValue(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,    // Socket number
    UINT8    ch,          // Channel number (0-based)
    UINT16   *mcodt     // 0 = 50 ohms; 1 = 100 ohms (HW default)
    );

struct ddr4OdtValueStruct*
LookupDdr4OdtValue(
    PSYSHOST host,          // Pointer to the system host (root) structure
    UINT8    socket,        // Socket Id
    UINT8    ch             // Channel number (0-based)
    );

UINT64_STRUCT
AddUINT64(
    UINT64_STRUCT Operand1, // first UINT64 to add
    UINT64_STRUCT Operand2  // second UINT64 to add
    );

UINT64_STRUCT
AddUINT64x32(
    UINT64_STRUCT Operand1, // UINT64 to add
    UINT32        Operand2  // UINT32 to add
    );

UINT64_STRUCT
SubUINT64(
    UINT64_STRUCT Operand1, // First UINT64
    UINT64_STRUCT Operand2  // Second UINT64
    );

UINT64_STRUCT
SubUINT64x32(
    UINT64_STRUCT Operand1,
    UINT32        Operand2
    );

UINT64_STRUCT
MulDw64(
    UINT32 dw1,   // first 32 bit value
    UINT32 dw2    // second 32 bit value
    );

UINT64_STRUCT
MultUINT64x32(
    UINT64_STRUCT Multiplicand, // Multiplicand
    UINT32        Multiplier    // what to multiply by
    );

UINT64_STRUCT
DivBy2Qw64(
    UINT64_STRUCT qw1    // 64 bit value to divide
    );

UINT64_STRUCT
DivUINT64x32(
    UINT64_STRUCT Dividend,    // Value to divide into
    UINT32        Divisor,     // Value to divide by
    UINT32        *Remainder   // Pointer to location to write remainder
    );

UINT8
Log2x32(
    UINT32 Value   // Input value
    );

UINT64_STRUCT
RShiftUINT64(
    UINT64_STRUCT Operand, // Value to be shifted
    UINT32        Count    // Number of times to shift right.
    );

UINT64_STRUCT
LShiftUINT64(
    UINT64_STRUCT Data,  // value to be shifted
    UINT32        Count    // number of time to shift
    );

INT16
Crc16(
    char *ptr,    // Pointer to the beginning of the data to work on
    int count     // number of bytes to check
    );

UINT32
MaxVal(
    UINT32 x,  // First value
    UINT32 y   // Second value
    );

UINT32
MinVal(
    UINT32 x,  // First value
    UINT32 y   // Second value
    );

UINT32
DispatchSlaves(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
PipeSync(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
GetData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
GetNvramData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
GetSlaveData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
GetStatus(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
SendData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
SendNvramData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
SendSlaveData(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
SendStatus(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

UINT32
SlaveAck(
    PSYSHOST  host  // Pointer to host structure (outside scope of this spec)
    );

void
InitHeap(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

void
rcFree(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    void     *memblock  // Pointer to memory to deallocate
    );

void*
rcMalloc(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT32 size   // Size of memory to allocate
    );

UINT32
MemCompare(
    UINT8 *dest,        // pointer to first memory location for compare
    UINT8 *src,           // pointer to second memory location for compare
    UINT32 numBytes     // Number of bytes to compare
    );

void
MemCopy(
    UINT8  *dest,       // pointer to copy destination
    UINT8  *src,          // pointer to source to copy
    UINT32 numBytes     // size to copy
    );

void
MemSetLocal(
    UINT8  *dest,         // Pointer to memory location
    UINT32 value,         // Value to flood location with
    UINT32 numBytes       // number of bytes to flood with value provided
    );

UINT32
TrainImode(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
TrainRXCTLE(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
TrainRxOdt(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
TrainTxEq(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT64_STRUCT
ReadMsrPipe(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8     socket,   // Socket number
    UINT32    msr       // Address of MSR to read
    );

void
WriteMsrPipe(
    PSYSHOST      host,      // Pointer to the system host (root) structure
    UINT8         socket,    // Socket number
    UINT32        msr,       // Address of MSR to read
    UINT64_STRUCT msrReg   // MSR Data
    );

VOID
ExitPipe(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

VOID
InitializePipe(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

INT8
putchar(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    INT8     c       // character received from serial buffer
    );

INT8
getchar(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

INT8
getchar_echo(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
getchar_timeout(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    INT8     *c,        // pointer to character received from serial buffer
    UINT32   usTimeout  // timeout in microseconds
    );

INT8
putchar_buffer(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    INT8      c       // pointer to character received from serial buffer
    );

UINT32
SerialBufferChar(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    INT8      c       // pointer to character received from serial buffer
    );

UINT32
SerialSendBuffer(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8    now      // 0 - wait for delay
                      // 1 - send immediately
    );

UINT8
getPrintFControl(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

void
releasePrintFControl(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

#ifdef SERIAL_DBG_MSG
#define rcPrintf(DbgInfo) rcPrintfunction DbgInfo
#else
#define rcPrintf(DbgInfo)
#endif
int
rcPrintfunction(
    PSYSHOST  host,       // Pointer to the system host (root) structure
    const INT8 *Format,   // Format string for output
    ...                   // Values matching the variables declared in
                          //   the Format parameter
    );

int
rcVprintf(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    const INT8 *Format,     // String containing characters to print and formating data.
    //    %[flags][width]type
    //
    //    [flags] '-' left align
    //    [flags] '+' prefix with sign (+ or -)
    //    [flags] '0' zero pad numbers
    //    [flags] ' ' prefix black in front of postive numbers
    //
    //    [width] non negative decimal integer that specifies
    //            the width to print a value.
    //    [width] '*' get the width from a int argument on the stack.
    //
    //    type    'd'|'i' signed decimal integer
    //    type    'u' unsinged integer
    //    type    'x'|'X' usinged hex using "ABCDEF"
    //    type    'c' print character
    //    type    'p' print a pointer to void
    //    type    'r' output a raw binary encoding
    va_list    Marker       // va_list that points to the arguments for Format that are on the stack
    );

void
setColor(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    INT8     foreGround,    // color code for foreground
    INT8     backGround,    // color code for background
    INT8     attribute      // attribute flag
    //   ATTRIBUTE_OFF         0
    //   ATTRIBUTE_BOLD        1
    //   ATTRIBUTE_UNDERSCORE  4
    //   ATTRIBUTE_BLINK       5
    //   ATTRIBUTE_REVERSE     7
    //   ATTRIBUTE_CONCEAL     8
    );

void
setRed(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

void
setWhite(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
StrCmpLocal(
    INT8 *src1, // first string to compare
    INT8 *src2  // second string to compare
    );

void
StrCpyLocal(
    INT8 *dest,  // Pointer to destination for string copy
    INT8 *src    // Pointer to source of string to copy
    );

UINT32
StrLenLocal(
    INT8 *Str     // Pointer to string to measure
    );

INT8
tolower_(
    INT8 c // character to convert
    );

UINT8
CheckRank(
    PSYSHOST host,             // Pointer to the system host (root) structure
    UINT8    socket,              // socket number
    UINT8    ch,                  // channel number
    UINT8    dimm,                // dimm number
    UINT8    rank,                // rank number
    UINT8    flags                // CHECK_MAPOUT
    );

#ifdef SERIAL_DBG_MSG
void
countTrackingData(
    PSYSHOST  host,    // Pointer to the system host (root) structure
    UINT8     mode,    //   PCI_ACCESS     0
                       //   JEDEC_COUNT    2
                       //   FIXED_DELAY    3
                       //   POLLING_COUNT  6
                       //   VREF_MOVE      7
    UINT32    data     // data to add to count
    );

void
SetMemPhase(
    PSYSHOST  host,    //  Pointer to the system host (root) structure
    UINT32    phase    // Current memory phase
    );

void
ClearMemPhase(
    PSYSHOST  host,    //  Pointer to the system host (root) structure
    UINT32    phase      // Current memory phase to clear
    );

UINT8
IsMemPhase(
    PSYSHOST host,  //  Pointer to the system host (root) structure
    UINT32 phase    // Current memory phase
    );

void
PrintLine(
    PSYSHOST  host,         // Pointer to the system host (root) structure
    UINT16    count,        // Number of units in the line
    UINT8     doubleLine,   // 0 prints '-'   1 prints '='
    UINT8     crLfBefore,   // 1 = return before printing; 0 = ignore
    UINT8     crLfAfter     // 1 = return after printing; 0 = ignore
    );

void
EmptyBlock(
    PSYSHOST  host,   // Pointer to sysHost
    UINT8     numSpaces // Number of spaces for the empty block
    );

void
DisplayDev(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8     socket,    // Processor socket
    UINT8     ch,       // Memory Channel
    UINT8     dimm,     // Specific DIMM on this Channel
    UINT8     rank,     // Rank on this DIMM
    UINT8     strobe,   // Strobe
    UINT8     bit       // Bit
    );

#endif // SERIAL_DBG_MSG

void
OutputCheckpoint(
    PSYSHOST host,           // Pointer to the system host (root) structure
    UINT8    majorCode,      // Major Checkpoint code to write
    UINT8    minorCode,      // Minor Checkpoint code to write
    UINT16   data            // Data specific to the minor checkpoint is written to
    //   low word of the checkpoint CSR
    );

#ifdef  BDAT_SUPPORT
UINT32
FillBDATStructure(
    PSYSHOST  host      // Pointer to the system host (root) structure
    );
#endif  //  BDAT_SUPPORT

void
WDBFill(
    PSYSHOST host,                      // Pointer to the system host (root) structure
    UINT8    socket,                    // Socket number
    UINT8    ch,                        // Channel number
    TWdbLine *WdbLines,               // Array that include data to write to the WDB
    UINT8    NumberOfWdbLines,        // The number of cachelines to write
    UINT8    StartCachelineIndex      // Start offset/cacheline number on the WDB.
    );

void
WriteWDBFixedPattern(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8     socket,    // Processor socket
    UINT8     ch,       // Memory Channel
    UINT32    pattern,  // Pattern to write into the cache line
    UINT8     spread, // The number of cache lines to write
    UINT8     cacheLine // Cache line to write
    );

UINT32
Decompress(
    void    *Source,         // The source buffer containing the compressed data.
    UINT32  SrcSize,       // The size of source buffer
    void    *Destination,    // The destination buffer to store the decompressed data
    UINT32  DstSize,         // The size of destination buffer.
    void    *Scratch,      // The buffer used internally by the decompress routine. This  buffer is
    //     needed to store intermediate data.
    UINT32  ScratchSize,     // The size of scratch buffer.
    UINT8   Version          // The version of de/compression algorithm.
    //      Version 1 for EFI 1.1 de/compression algorithm.
    //      Version 2 for Tiano de/compression algorithm.
    );

UINT32
DiscoverUsb2DebugPort(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
InitializeUsb2DebugPort(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

INTN
EFIAPI
GetRegisterAddress(
    IN COMMON_ADDRESS_STRUCT    *Address    // A pointer of the address of the Common
    //    Address Structure to read from.
    );

UINT8
CheckProcessorType(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8     cpuType   // CPU type (HSX, BWX,....)
    );


UINT8
CheckSteppingEqual(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8     cpuType,  // CPU type (HSX, BWX,....)
    UINT8     stepping  // CPU stepping
    );


UINT8
CheckSteppingGreaterThan(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8     cpuType,  // CPU type (HSX, BWX,....)
    UINT8     stepping  // CPU stepping
    );

UINT8
CheckSteppingLessThan(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8     cpuType,  // CPU type (HSX, BWX,....)
    UINT8     stepping  // CPU stepping
    );

VOID
GetCpuID(
    UINT32 *RegEax,  // ptr to EAX register
    UINT32 *RegEbx,  // ptr to EBX register
    UINT32 *RegEcx,  // ptr to ECX register
    UINT32 *RegEdx   // ptr to EDX register
    );

UINT64_STRUCT
ReadMsrLocal(
    UINT32 msrAddr    // Address of MSR to read
    );

void
WriteMsrLocal(
    UINT32        msrAddr,    // Address of MSR to write
    UINT64_STRUCT data        // 64 bit data to write to MSR
    );

void
InitCpuTimer(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

void
ReadTsc64(
    UINT64_STRUCT *count // location to read the time stamp counter into
    );

#define  TDIFF_UNIT_S     1
#define  TDIFF_UNIT_MS    1000
#define  TDIFF_UNIT_US    1000000
#define  TDIFF_UNIT_NS    1000000000

UINT32
TimeDiff(
    PSYSHOST  host,             // Pointer to the system host (root) structure
    UINT64_STRUCT startCount,   // First Time Stamp Counter value
    UINT64_STRUCT endCount,     // Second Time Stamp Counter value
    UINT32        unit          // For Time in Seconds (TDIFF_UNIT_S) = 1
    // For Time in milliseconds (TDIFF_UNIT_MS) = 1000
    // For Time in microseconds (TDIFF_UNIT_US) = 1000000
    // For Time in nanoseconds (TDIFF_UNIT_NS) = 1000000000
    );

void
HaltOnError(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8    majorCode,   // Major error code
    UINT8    minorCode    // Minor error code
    );

void
FatalErrorHook(
    PSYSHOST  host,         // Pointer to the system host (root) structure
    UINT8    majorCode,     // Major error code
    UINT8    minorCode,     // Minor error code
    UINT8    *haltOnError   // Halt on error = 1
    // Do not halt on error = 0
    );

void
LogError(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8    majorCode,   // Major error code
    UINT8    minorCode,    // Minor error code
    UINT32   logData        // 32 bit error specific data to log
    );

void
LogWarning(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8    majorCode,   // Major error code
    UINT8    minorCode,    // Minor error code
    UINT32   logData        // 32 bit error specific data to log
    );

void
OutputError(
    PSYSHOST  host,     // Pointer to the system host (root) structure
    UINT8    ErrorCode,         // Major warning code
    UINT8    minorErrorCode,    // Minor warning code
    UINT8    socket,              // socket number
    UINT8    ch,                  // channel number
    UINT8    dimm,                // dimm number
    UINT8    rank                 // rank number
    );

void
OutputWarning(
    PSYSHOST host,                // pointer to sysHost
    UINT8    warningCode,       // Major warning code
    UINT8    minorWarningCode,  // Minor warning code
    UINT8    socket,              // socket number
    UINT8    ch,                  // channel number
    UINT8    dimm,                // dimm number
    UINT8    rank                 // rank number
    );

UINT32
SetPromoteWarningException(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8    majorCode,     // Major error code to log
    UINT8    minorCode      // Minor error code to log
    );

UINT32
InitGpio(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT8
InPort8(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress    // IO Address to use
    );

UINT16
InPort16(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress    // IO Address to use
    );

UINT32
InPort32(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress    // IO Address to use
    );

void
OutPort8(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress,   // IO Address to use
    UINT8 data            // Data to write to IO Address
    );

void
OutPort16(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress,   // IO Address to use
    UINT16 data           // Data to write to IO Address
    );

void
OutPort32(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT16   ioAddress,   // IO Address to use
    UINT32 data           // Data to write to IO Address
    );

UINT32 GetPeEntrypoint(
    PSYSHOST  host,   // Pointer to the system host (root) structure
    UINT8 *baseAddress  // Pointer to the base address
    );

UINT32
EarlyReadID(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT16
MemGetCmosDate(
    PSYSHOST  host    // Pointer to the system host (root) structure
    );

UINT32
ProcPpinChangedCheck (
  PSYSHOST  host,    // Pointer to the system host (root) structure
  UINT8     socket   // Processor socket
  );

UINT64_STRUCT
RotateLeftUINT64 (
  UINT64_STRUCT Data,  // 64-bit data to rotate
  UINT32        Count  // Number of bits to rotate
  );

UINT64_STRUCT
RotateRightUINT64 (
  UINT64_STRUCT Data,  // 64-bit data to rotate
  UINT32        Count  // Number of bits to rotate
  );

#endif // _CORE_API_H_
