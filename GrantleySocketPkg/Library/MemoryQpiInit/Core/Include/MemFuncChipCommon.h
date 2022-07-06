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
#ifndef  _memfuncchipcommon_h
#define  _memfuncchipcommon_h
#ifndef ASM_INC

#include "DataTypes.h"
#include "SysHost.h"
#include "CpuPciAccess.h"
#include "Cpgc.h"
#include "MemProjectSpecific.h"

typedef struct {
  UINT32              (*mrcTask) (PSYSHOST host); // Ptr to function to execute, with parameter list.
  UINT8               postCodeMajor;      // BIOS post code output to the debug port if value <> 0.
  UINT8               postCodeMinor;      // BIOS post code output to the debug port if value <> 0.
  UINT32              (*oemHook) (PSYSHOST host); // Ptr to function to execute, with parameter list.
  UINT8               policyFlag;        // Call table flags
  UINT8               mpFlag;
#ifdef SERIAL_DBG_MSG
  char                *debug_string_ptr;  // Output string describing this task (potentially output to debug serial port).
#endif //MRC_DEBUG_PRINT
} CallTableEntryType;

#ifdef SERIAL_DBG_MSG
#define CALL_TABLE_STRING(x) x
#else
#define CALL_TABLE_STRING(x)
#endif

typedef struct {
  UINT8               ddrVoltageValue;
  UINT32              Vr12Encoding;
  UINT32              Vr125Encoding;
  UINT32              Vr135Encoding;
  UINT32              Vr1310Encoding;
#ifdef SERIAL_DBG_MSG
  char                *string_ptr;
#endif //MRC_DEBUG_PRINT
} VddValues;

//-----------------------------------------------------------------------------
// Test Modes (mode)
#define MODE_XTALK            BIT0  // Placeholder - This has not been implemented
#define MODE_VIC_AGG          BIT1
#define MODE_START_ZERO       BIT2
#define MODE_POS_ONLY         BIT3
#define MODE_NEG_ONLY         BIT4
#define MODE_DATA_MPR         BIT5
#define MODE_DATA_FIXED       BIT6
#define MODE_DATA_LFSR        BIT7
#define MODE_ADDR_LFSR        BIT8
#define MODE_ADDR_CMD0_SETUP  BIT9
#define MODE_CHECK_PARITY     BIT10
#define MODE_DESELECT         BIT11
#define MODE_VA_DESELECT      BIT12

// Scope of the margin
#define SCOPE_SOCKET      0
#define SCOPE_CH        1
#define SCOPE_DIMM      2
#define SCOPE_RANK      3
#define SCOPE_STROBE    4
#define SCOPE_BIT       5

struct baseMargin {
  INT16 n;
  INT16 p;
};

// Output structures based on scope
#define  MAX_BITS    72

struct baseMargin socketMargin;

struct chMargin {
  struct baseMargin channel[MAX_CH];
};
struct dimmMargin {
  struct baseMargin dimm[MAX_CH][MAX_DIMM];
};
struct rankMargin {
  struct baseMargin rank[MAX_CH][MAX_RANK_CH];
};
struct strobeMargin {
  struct baseMargin strobe[MAX_CH][MAX_RANK_CH][MAX_STROBE];
};
struct bitMargin {
  struct baseMargin bits[MAX_CH][MAX_RANK_CH][MAX_BITS];
};

#ifdef LRDIMM_SUPPORT
struct subRankMargin {
  struct rankMargin subRank[MAX_SUBRANK];
};
#endif

// Mask of bits to ignore
struct bitMask {
   UINT8 bits[MAX_CH][MAX_RANK_CH][MAX_STROBE/2];
};



// Function declarations

//
// Chip layer only Prototypes. Functions defined and used only in the chip layer.
//

// InitMemChip.c
UINT8  GetMCCh(UINT8 mcId, UINT8 ch);                                     // CHIP
void   EnableCAParity(PSYSHOST host, UINT8 socket);                       // CHIP
void   DisableCAParity(PSYSHOST host, UINT8 socket);                      // CHIP
void   CheckRankPopLocal(PSYSHOST host, UINT8 socket);                    // CHIP
UINT32 ValidateCurrentConfigRASHook(PSYSHOST host);

// Address mapping
// MemAddrMap.c - Chip
UINT32 InitMemoryMap(PSYSHOST host);                                              // CALLTAB / CHIP
void   GetMcBaseAddr(PSYSHOST host, UINT8 socket, UINT8 mcId);                    // CHIP
void   WriteRIRForChannel(PSYSHOST host, UINT8 socket, UINT8 ch);                 // CHIP

// MemSPDChip.c
UINT32 GatherSPDData(PSYSHOST host);                                                                    // CALLTAB
UINT32 GatherSpdDataStage2(PSYSHOST host);                                                              // CALLTAB
UINT32 CheckPORCompat(PSYSHOST host);                                                                   // CALLTAB
void   MemRankGeometry (PSYSHOST  host, UINT8 socket, UINT8 cBits[MAX_RANK_CH][MAX_CH], UINT8 rBits[MAX_RANK_CH][MAX_CH],
                      UINT8 bBits[MAX_RANK_CH][MAX_CH], UINT8 rankEnabled[MAX_RANK_CH][MAX_CH],
                      UINT8 dimmRank[MAX_RANK_CH][MAX_CH], UINT8 rankChEnabled[MAX_CH],
                      UINT8 *maxEnabledRank, UINT8 logical2Physical[MAX_RANK_CH][MAX_CH]);              // CHIP

// Memory Commands
// MemIOControl.c
UINT32 InitDdrioInterfaceLate(PSYSHOST host);                                                     // CALLTAB
void   ClearBWErrorStatus(PSYSHOST host, UINT8 socket, UINT8 ch);                                 // CHIP
void   DoComp(PSYSHOST host, UINT8 socket);                                                       // CHIP
#define NOT_CACHED  0
#define CACHED      1
MRC_STATUS GetSetTxVref(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 strobe, UINT8 bit, GSM_LT level,
                        GSM_GT group, UINT8 mode, INT16 *value);                                                               // CHIP
void   WriteRdVref(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 strobe, UINT8 vref);                                           // DEBUG
UINT32 OffsetVref(UINT32 vref, INT16 offset);                                                                                  // DEBUG
UINT32 UpdateIoRegisterCh (PSYSHOST host, UINT8 byte, UINT32 regOffset);                                                       // CHIP
UINT8  GetMaxRcvenLogicDelayChannel (PSYSHOST host, UINT8 socket, UINT8 ch);

// MemThrot.c
UINT32 InitThrottlingEarly (PSYSHOST host);                                     // CALLTAB
UINT32 InitThrottling (PSYSHOST host);                                          // CALLTAB
UINT32 ControlDramPMThermalViaPcode (PSYSHOST host, UINT8 flag);                // CHIP
UINT32 ControlRasViaPcode (PSYSHOST host, UINT8 flag);                          // CHIP

// MemRAS.c
UINT32 CheckRASSupportAfterMemInit (PSYSHOST host);                             // CALLTAB
UINT32 SetRASConfig(struct sysHost *host);                                      // CALLTAB
void   SetLockstepMode (struct sysHost *host, UINT8 socket);                    // CHIP
void   SetLockstepModeLate (struct sysHost *host, UINT8 socket);                // CHIP
UINT32 AdjustMemAddrMapForMirror (PSYSHOST Host);                               // CHIP

// MemXoverCalib.c
UINT32 CrossoverCalib(PSYSHOST host);                                           // CHIP

// MemTrainingChip.c
UINT32 SenseAmpOffset(PSYSHOST host);                                                                     // CHIP
UINT32 SenseAmpOffsetBdx(PSYSHOST host);                                                                  // CHIP
UINT32 TxEqTraining(PSYSHOST host);                                                                       // CALLTAB
UINT32 ImodeTraining(PSYSHOST host);
UINT8  CheckSkipRankTrain(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, GSM_LT level, GSM_GT group);

// CMD/CTL/CLK Access
// MemCmdControl.c
void   SetRankUsed(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 Value);                                                           // CHIP
void   SetStartingCCC(PSYSHOST host, UINT8 socket);                                                                               // CHIP
MRC_STATUS SetCombinedCtlGroup(PSYSHOST host, UINT8 socket, UINT8 ch, UINT16 listSize, VOID *value);                              // CHIP

// Timing
// MemTimingChip.c
void   ProgramTimings(PSYSHOST host, UINT8 socket, UINT8 ch);                                       // CHIP
void   RestoreTimingsChip (PSYSHOST host, UINT8 socket, UINT8 ch);                                  // CHIP
UINT32 TurnAroundTimings(PSYSHOST host, UINT8 socket);                                              // CHIP
#ifdef DEBUG_TURNAROUNDS
UINT32 CheckTurnAroundTimings(PSYSHOST host, UINT8 socket);                                         // CHIP
#endif  // DEBUG_TURNAROUNDS
void   WrLvlDqsToCmdDelay (PSYSHOST host, UINT8 socket, UINT8 ch, INT16 offset, BOOLEAN updateCache); // CHIP

// MemCpg.c
void   WriteMRSPDA(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 rank, UINT8 dram, UINT16 data, UINT8 bank);   // CHIP

// MemProjectSpecific.c
void SetCommonRoundtrip (PSYSHOST host, UINT8 socket);                                              // CHIP
UINT32 EarlyCtlClkLoopback(PSYSHOST host);                                                          // CHIP

// FnvAccess.c
UINT32 ReadFnvCfgSmb(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 reg, UINT32 *data);          // CHIP
UINT32 WriteFnvCfgSmb (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 reg, UINT32 data);         // CHIP
UINT32 WriteFnvCfgEmrs (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT32 reg, UINT32 data);        // CHIP
void   EmulationPoll (PSYSHOST host, UINT8 socket, UINT8 BoxInst, UINT32 Offset, UINT32 mask, UINT8 logic); // CHIP

// MemPciAccess.c
UINT32 MemReadPciCfgEx  (PSYSHOST host, UINT8 NodeId, UINT8 ChIdOrBoxInst, UINT32 Offset);                // CHIP
VOID   MemWritePciCfgEx (PSYSHOST host, UINT8 NodeId, UINT8 ChIdOrBoxInst, UINT32 Offset, UINT32 Data);   // CHIP
UINT32 MemReadPciCfgMC_Ch  (PSYSHOST host, UINT8 socket, UINT8 ch, UINT32 Offset, UINT32 chOffset); // CHIP
void   MemWritePciCfgMC_Ch (PSYSHOST host, UINT8 socket, UINT8 ch, UINT32 Offset, UINT32 chOffset, UINT32 Data); // CHIP



#endif // ASM_INC

#endif   //  _memfuncchipcommon_h
