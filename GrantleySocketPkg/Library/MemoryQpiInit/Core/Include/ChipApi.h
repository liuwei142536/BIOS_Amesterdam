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
 *      This file contains API declarations defined in the MRC Chip layer.
 *
 ************************************************************************/
#ifndef  _CHIP_API_H_
#define  _CHIP_API_H_

#include "DataTypes.h"
#include "SysHost.h"
#include "CommonAccessType.h"
#include "MemProjectSpecific.h"

UINT32
InitDdrioInterface(
    PSYSHOST  host                              // Pointer to host structure (outside scope of this spec)
    );

VOID
MemoryInitDdr (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT8
MemPresent (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       memPresent                        //
  );

void
ProgramBgfTable (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT16
CodeDeCodeTxRon (
  UINT16 value, 
  UINT8  code
);

UINT16
CodeDeCodeRxOdt (
  UINT16 value, 
  UINT8  code
);

MRC_STATUS
GetDataGroupLimits(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    GSM_LT    level,                            // IO level to access
    GSM_GT    group,                            // Data group to access
    UINT16    *minLimit,                        // Minimum delay value allowed
    UINT16    *maxLimit,                        // Maximum absolute value allowed
    UINT16    *usdelay                          // Time in usec required for new setting to take effect
    );

MRC_STATUS
GetSetDataGroup(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    UINT8     dimm,                             // DIMM number within a DDR channel (0-based, farthest slot == 0)
    UINT8     rank,                             // Logical rank number within a DIMM (0-based)
    UINT8     strobe,                           // Dqs data group within the rank
    UINT8     bit,                              // Bit index within the data group
    GSM_LT    level,                            // IO level to access
    GSM_GT    group,                            // Data group to access
    UINT8     mode,                             // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
    INT16     *value                            // Pointer to absolute value or offset based on mode
    );

MRC_STATUS
GetSetCmdGroupDelay (
    PSYSHOST host,                              // Pointer to host structure (outside scope of this spec)
    UINT8 socket,                               // Processor socket within the system (0-based)
    UINT8 ch,                                   // DDR channel number within the processor socket (0-based)
    GSM_GT group,                               // Platform Control Group
    UINT8 mode,                                 // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16 *value,                               // Value to program or offset
    UINT16 *minVal,                             // Updated if any value in the group is less than minVal
    UINT16 *maxVal                              // Updated if any value in the group is more than maxVal
    );

MRC_STATUS
GetSetCtlGroupDelay (
    PSYSHOST host,                              // Pointer to host structure (outside scope of this spec)
    UINT8 socket,                               // Processor socket within the system (0-based)
    UINT8 ch,                                   // DDR channel number within the processor socket (0-based)
    GSM_GT group,                               // Platform Control Group
    UINT8 mode,                                 // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16 *value,                               // Value to program or offset
    UINT16 *minVal,                             // Updated if any value in the group is less than minVal
    UINT16 *maxVal                              // Updated if any value in the group is more than maxVal
    );

MRC_STATUS
GetSetCmdVref (
    PSYSHOST host,                              // Pointer to host structure (outside scope of this spec)
    UINT8 socket,                               // Processor socket within the system (0-based)
    UINT8 ch,                                   // DDR channel number within the processor socket (0-based)
    UINT8 dimm,                                 // DIMM number within a DDR channel (0-based, farthest slot == 0)
    UINT8 mode,                                 // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16 *value                                // Value to program or offset
    );

MRC_STATUS
GetSetCmdDelay (
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    UINT8     ioGroup,                          // Group number
    UINT8     side,                             // Side number
    UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16     *value                            // Value to program or offset
    );

MRC_STATUS
GetSetCtlDelay(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    UINT8     ioGroup,                          // Group number
    UINT8     side,                             // Side number
    UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16     *value                            // Value to program or offset
    );

MRC_STATUS
GetSetClkDelay(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    UINT8     clk,                              // Clock number
    UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16      *value                           // Value to program
    );

MRC_STATUS
GetSetSignal(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    GSM_LT    level,                            // IO level to access
    GSM_CSN   sig,                              // Enumerated signal name
    UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                                // GSM_READ_ONLY - Do not write
                                                // GSM_WRITE_OFFSET - Write offset and not value
                                                // GSM_FORCE_WRITE - Force the write
    INT16      *value                           // Value to program
    );

UINT8
CmdVrefQuick(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    GSM_GT    group                             // Data group to access
    );

MRC_STATUS
GetCmdGroupLimits(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    GSM_LT    level,                            // IO level to access
    GSM_GT    group,                            // Command, clock or control group to access
    UINT16    *minLimit,                         // Minimum delay value allowed
    UINT16    *maxLimit                         // Maximum absolute value allowed
    );

MRC_STATUS
SetCombinedCmdGroup(
    PSYSHOST  host,                             // Pointer to host structure (outside scope of this spec)
    UINT8     socket,                           // Processor socket within the system (0-based)
    UINT8     ch,                               // DDR channel number within the processor socket (0-based)
    GSM_LT    level,                            // IO level to access
    UINT8     mode,                             // Bit-field of different modes
    UINT8     listType,                         // Selects type of each list
    UINT16    listSize,                         // Number of entries in each list
    VOID      *value                            // Array of pointers to listType
    );

VOID
GetDclkRatio (
  PSYSHOST    host,                             // Pointer to host structure (outside scope of this spec)
  UINT8       socket,                           // Processor socket within the system (0-based)
  UINT8       *req_type,                        // Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type
                                                //    0h     MC frequency request for 133 Mhz
                                                //    1h     MC frequency request for 100 Mhz
                                                //    All other values are reserved.
  UINT8       *req_data                         // Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data
                                                //   The data for the request.
                                                //   The encoding of this field is the 133 MHz multiplier for DCLK/QCLK:
                                                //   Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq
                                                //  ...
                                                //   00110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz
                                                //   01000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz
                                                //   01010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz
                                                //   01100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz
                                                //   01110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz
                                                //   10000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz
                                                //  ...
  );

VOID
SetDclkRatio (
  PSYSHOST  host,                               // Pointer to host structure (outside scope of this spec)
  UINT8     socket,                             // Processor socket within the system (0-based)
  UINT8     req_type,                           // Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type
                                                //    0h     MC frequency request for 133 Mhz
                                                //    1h     MC frequency request for 100 Mhz
                                                //    All other values are reserved.
  UINT8     req_data                            // Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data
                                                //   The data for the request.
                                                //   The encoding of this field is the 133 MHz multiplier for DCLK/QCLK:
                                                //   Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq
                                                //  ...
                                                //   00110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz
                                                //   01000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz
                                                //   01010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz
                                                //   01100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz
                                                //   01110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz
                                                //   10000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz
                                                //  ...
  );

VOID
SetRingRatio (
  PSYSHOST  host,
  UINT8     socket,
  UINT16    ddrFreqMHz
  );

UINT8
GetMaxFreq (
  PSYSHOST   host,                              // Pointer to the system host (root) structure
  UINT8      socket                             // Processor socket ID
  );

void
SetChTwr (
  PSYSHOST    host,                             // Pointer to host structure (outside scope of this spec)
  UINT8       socket,                           // Processor socket within the system (0-based)
  UINT8       ch,                               // DDR channel number within the processor socket (0-based)
  UINT8       nWR                               // nWR value
  );

VOID
SetDataTiming (
  PSYSHOST    host,                             // Pointer to host structure (outside scope of this spec)
  UINT8       socket,                           // Processor socket within the system (0-based)
  UINT8       ch,                               // DDR channel number within the processor socket (0-based)
  UINT8       nCL,                              // nCL value
  UINT8       nCWL                              // nCWL value
  );

void
PerformChipLRDIMMEarlyCtlClk(
  PSYSHOST  host,                               // Pointer to the system host (root) structure
  UINT8     socket                              // Processor socket ID
  );

void
TrnOnChipCMDADDOpEn(
  PSYSHOST  host,                               // Pointer to the system host (root) structure
  UINT8     socket,                             // Processor socket ID
  UINT8     ch,                                 // DDR channel (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

void
TrnOffChipCMDADDOpEn(
  PSYSHOST  host,                               // Pointer to the system host (root) structure
  UINT8     socket,                             // Processor socket ID
  UINT8     ch,                                 // DDR channel (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

UINT32
DetectSpdTypeDIMMConfig (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       keyByte                           // Key byte holds the DIMM type information
  );

void
GetModuleSerialNumber(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       i,                                // The number of the module serial byte to check
  UINT8       *SPDReg                           // Pointer to the SPD Register
  );

UINT32
ReadSpd (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT16      byteOffset,                       // Byte Offset to read
  UINT8       *data                             // Pointer to data
  );

UINT8
GetDdr4SpdPageEn (
  PSYSHOST         host,                        // Pointer to the system host (root) structure
  UINT8            socket                       // Socket Id
  );

void
AcquireReleaseGlobalSemaphore (
   PSYSHOST  host,                              // Pointer to the system host structure
   UINT8     SysSemaphoreNum,                   // SYSTEMSEMAPHORE register number (0 or 1)
   UINT8     OpCode                             // 1 = ACQUIRE_SEMAPHORE
                                                // 0 = RELEASE_SEMAPHORE
   );

void
AcquireReleaseAPSystemSemaphore (
   PSYSHOST  host,                              // Pointer to the system host structure
   UINT8     socket,                            // Target Socket ID
   UINT8     LocAPSemaphoreNum,                 // SYSTEMSEMAPHORE register number (0 or 1)
   UINT8     OpCode                             // 1 = ACQUIRE_SEMAPHORE
                                                // 0 = RELEASE_SEMAPHORE
);

UINT8
checkMsgLevel (
  PSYSHOST   host,                              // Pointer to the system host (root) structure
  UINT32     level                              // Debug message level to check
  );

void
disableAllMsg (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
restoreMsg (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
debugPrintMem (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT32      dbgLevel,                         // Minimum debug level in order to print
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT8       rank,                             // Rank
  UINT8       strobe,                           // Strobe
  UINT8       bit,                              // Bit
  char*       Format,                           // Printing formatting
  ...                                           // list of variables to print based on formatting
  );

void
FatalError (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       majorCode,                        // Major error code
  UINT8       minorCode                         // Minor error code
  );

VOID
DimmInfoDisplayProc (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Processor socket ID
  );

VOID
DimmInfoDisplayProcFeatures (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
EnableCAParityRuntime (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Processor socket ID
  );

UINT8
GetLvmode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Processor socket ID
  );

#ifdef SERIAL_DBG_MSG
char
*GetSignalStr(
  GSM_CSN     signal,                           // Signal name to access
  char        *strBuf                           // String buffer
);

char
*GetPlatformGroupStr(
  GSM_GT      group,                            // Platform Command group to test
  char        *strBuf                           // String buffer
);
#endif  // SERIAL_DBG_MSG

VOID
PrintMemSetupOptionsChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Processor socket ID
  );

UINT32
WriteBios2PcuMailboxCommand (
  struct sysHost  *host,                        // pointer to sysHost structure on stack
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      dwordCommand,                     // Pcu mailbox command to write
  UINT32      dworddata                         // Pcu mailbox data
  );

void
InitSmb (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );

void
GetSmbAddress (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // Dimm number
  struct smbDevice *dev                         // Pointer to smbDevice
  );

UINT32
ReadSmb (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Processor socket ID
  struct smbDevice  dev,                        // Device address on SMBUS
  UINT16       byteOffset,                       // Offset in Device
  UINT8       *data                             // Pointer to store value
                                                //   NOTE: For a PCH block read, the array must be 33bytes in length,
                                                //   and the first data byte pointed to is the number of bytes read.
                                                //   This implementation only allows a maximum of 32bytes to be read.
  );

UINT32
WriteSmb (
    PSYSHOST    host,                             // Pointer to the system host (root) structure
    UINT8       socket,                           // Processor socket ID
    struct smbDevice  dev,                        // Device address on SMBUS
    UINT8       byteOffset,                       // Offset in Device
    UINT8       *data                             // Pointer to store value
                                                  //   NOTE: For a PCH block read, the array must be 33bytes in length,
                                                  //   and the first data byte pointed to is the number of bytes read.
                                                  //   This implementation only allows a maximum of 32bytes to be read.
  );

void
IsSmbusSemaphoreNeeded (
    PSYSHOST    host,                              // Pointer to the system host (root) structure
    UINT8       socket                             // Processor socket ID
);

UINT8
GetSmbus (
    PSYSHOST    host,                              // Pointer to the system host (root) structure
    UINT8       socket                             // Processor socket ID
  );

void
ReleaseSmbus (
    PSYSHOST    host,                              // Pointer to the system host (root) structure
    UINT8       socket                             // Processor socket ID
  );

void
AdjustRoundtripForMpr(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  UINT16      mprMode                           // MPR Mode
  );

void
CheckRankPop (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
ChipEarlyCmdCKSvHook(
  PSYSHOST      host,                           // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

UINT32
GetMarginsHook (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  BOOLEAN     entry                             // TRUE when entering GetMargins, FALSE when exiting.
  );

void                                            
SetRankDAMprFnv (                                 
  PSYSHOST    host,                             // Pointer to the system host (root) structure                      
  UINT8       socket,                           // Socket Id                
  UINT8       ch,                               // Channel number (0-based) 
  UINT8       dimm,                             // Current dimm             
  UINT16      mprMode                           // Either 0 for Disable or MR3_MPR to Enable
  );

void                                            
SetRankDMprFnv (                                 
  PSYSHOST    host,                             // Pointer to the system host (root) structure                      
  UINT8       socket,                           // Socket Id                
  UINT8       ch,                               // Channel number (0-based) 
  UINT8       dimm,                             // Current dimm             
  UINT16      mprMode                           // Either 0 for Disable or MR3_MPR to Enable
  );

void
ProgramDAFnvMprPattern (
  PSYSHOST    host,                             // Pointer to the system host (root) structure                      
  UINT8       socket,                           // Socket Id                
  UINT8       ch,                               // Channel number (0-based) 
  UINT8       dimm,                             // Current dimm             
  UINT16      pattern                           // Pattern to program
  );                                            

void
ProgramDFnvMprPattern (
  PSYSHOST    host,                             // Pointer to the system host (root) structure                      
  UINT8       socket,                           // Socket Id                
  UINT8       ch,                               // Channel number (0-based) 
  UINT8       dimm,                             // Current dimm             
  UINT16      pattern                           // Pattern to program
  );                                            
VOID
SetDimmPop (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

VOID
SetEncodedCsMode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

void
DoZQ (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       zqType                            // Type of ZQ Calibration: Long or Short
  );

void
GetChipLogicalRank(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  PCHIP_DDRIO_STRUCT dataControl                // Pointer to data control structure
  );

UINT8
GetMCID (
  PSYSHOST   host,                              // Pointer to the system host (root) structure
  UINT8      SocId,                             // CPU Socket Node number (Socket ID)
  UINT8      ch                                 // Channel number
  );

UINT32
IsRankDisabled (
  PSYSHOST  host,                               // Pointer to the system host (root) structure
  UINT8     socket,                             // Socket Id
  UINT8     ch,                                 // Channel number (0-based)
  UINT8     dimm,                               // Current dimm
  UINT16    rank                                // Current rank
  );

void
ReadBcwDimmDdr4 (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       bcwFunc,                          // BWC Function
  UINT8       bcwByte,                          // BWC Byte
  UINT8       bcwNibble,                        // BWC Nibble
  UINT8       MprData[MAX_STROBE]               // each byte is updated with UI[0-7] data per DRAM device
  );

void
ReadMprRankDdr4 (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  UINT8       mprPage,                          // MPR page
  UINT8       mprLoc,                           // MPR loc
  UINT8       MprData[MAX_STROBE]               // each byte is updated with UI[0-7] data per DRAM device
  );

void
UpdateChipMCDelay(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  struct channelNvram (*channelNvList)[MAX_CH], // Host channel list in NVRam
  INT16       TargetOffset,                     // Target offset for byte lanes
  UINT32      *CRAddDelay,                      // Add delays
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Write Leveling clean up structure pointer
  );

void
UpdateAccessMode(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
WaitForMailboxReady (
  PSYSHOST    host,                             // Pointer to the system host (root) structure                      
  UINT8       socket                            // Socket Id                
  );                                            

VOID
PostDimmDetectChipHook (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
ResetAllDdrChannels (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      chBitMask                         // Bitmask of channels to reset
  );

void
IO_Reset (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
FifoTrainReset (
                PSYSHOST host,                  // Pointer to the system host (root) structure
                UINT8    socket                 // Socket Id
);
UINT8
CheckSktId (
            PSYSHOST host,                     // Pointer to the system host (root) structure
            UINT8    socket                    // Socket Id
);
UINT8
CheckMemRas (
            PSYSHOST    host                   // Pointer to the system host (root) structure
);
void
LoadCapId (
            PSYSHOST host,                     // Pointer to the system host (root) structure
            UINT8    socket                    // Socket Id
);
void
SetMemDone (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT32
CpgcAdvCmdParity (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT8       ch,                               // DDR channel to setup
  UINT8       rank,                             // Logical rank number
  UINT8       subrank,                          // Subrank rank number
  GSM_CSN     sig,                              // Signal to test
  UINT8       mode                              // CADB mode
  );

UINT32
CpgcAdvTrainingErrorStatus (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap,                       // 4-bit bit mask of ddr channels to stop
  CPGC_ERROR_STATUS cpgcErrorStatus[MAX_CH]     // error status for each channel
  );

UINT32
CpgcAdvTrainingExecTestGroup (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap,                       // 4-bit bit mask of ddr channels to stop
  UINT8       pattern,                          // ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2,
                                                //   3 = cmd  group 3)
  GSM_GT      group                             // Parameter to be margined
  );

UINT32
CpgcAdvTrainingSetup (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap,                       // 4-bit bit mask of ddr channels to stop
  UINT8       rank,                             // logical rank number (0-7)
  UINT8       subRank,                          // Sub rank
  UINT32      pattern,                          // ddr pattern (0-3) ( 0 = dq, 1 = cmd group 1, 2 = cmd group 2,
                                                //   3 = cmd  group 3)
  UINT8     dqLoopCount                         // dq sequence loop count - number of times dq sequence is
                                                //   executed (2^(dqLoopCount - 1))
  );

UINT32
CpgcGlobalTestStart (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap                        // 4-bit bit mask of ddr channels to test - (1 = active channel)
  );

UINT32
CpgcGlobalTestStop (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap                        // 4-bit bit mask of ddr channels to stop
  );

void
CpgcMemTest (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap,                       // 4-bit bit mask of ddr channels to stop
  UINT32      flags                             // Read or Write and data pattern flags
  );

UINT32
CpgcPollGlobalTestDone (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap                        // 4-bit bit mask of ddr channels to stop
  );

void
GetChipBwSerrErrors(
  UINT32 errMask,                                       // Bit mask of the errors to check
  UINT8     ch,                                         // Channel number (0-based)
  UINT8 errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE],   // Error results
  INT16 piIndex,                                        // The index within the Pi Sweep
  INT16 offset,                                         // CMD step size
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct   //Pointer to data
  );

void
EnableCADBDeselect (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket to train
  UINT32      ddrChEnMap,                       // 4-bit bit mask of ddr channels to stop
  UINT16      mode                              // MODE_DESELECT
                                                // MODE_VA_DESELECT
  );

VOID
SetCpgcPatControl (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       mux0,                             // Mux 0
  UINT8       mux1,                             // Mux 1
  UINT8       mux2,                             // Mux 2
  UINT32      patbuf0,                          // Pattern 0
  UINT32      patbuf1,                          // Pattern 1
  UINT32      patbuf2                           // Pattern 2
 );

UINT8
PatternLengthToCpgcLoopCnt(
  UINT32 PatternLength                          // Number of loops
  );

MRC_STATUS
GetSetRxDelay (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       strobe,                           // Strobe number (0-based)
  UINT8       bit,                              // Bit number
  GSM_LT      level,                            // CSM_LT - Level to program (DDR, VMSE, Buffer)
  GSM_GT      group,                            // CSM_GT - Parameter to program
  UINT8       mode,                             // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
  INT16       *value                            // Pointer to delay value or offset based on mode
  );

MRC_STATUS
GetSetTxDelay (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       strobe,                           // Strobe number (0-based)
  UINT8       bit,                              // Bit number
  GSM_LT      level,                            // CSM_LT - Level to program (DDR, VMSE, Buffer)
  GSM_GT      group,                            // CSM_GT - Parameter to program
  UINT8       mode,                             // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
  INT16       *value                            // Pointer to delay value or offset based on mode
  );

UINT8
GetTxEqSettingsList(
    PSYSHOST    host,                               // Pointer to the system host (root) structure
    UINT8       socket,                             // CPU Socket Node number (Socket ID)
    INT16 settingList[MAX_ODT_SETTINGS][MAX_PARAMS] // Settings List
    );


UINT8
GetCtleSettingsList(
    PSYSHOST    host,                               // Pointer to the system host (root) structure
    UINT8       socket,                             // CPU Socket Node number (Socket ID)
    INT16 settingList[MAX_ODT_SETTINGS][MAX_PARAMS] // Settings List
    );
    
UINT8
GetMcRonSettingsList(
    PSYSHOST    host,                               // Pointer to the system host (root) structure
    UINT8       socket,                             // CPU Socket Node number (Socket ID)
    INT16 settingList[MAX_ODT_SETTINGS][MAX_PARAMS] // Settings List
    );

MRC_STATUS
GetSetRxDelayBit (
    PSYSHOST    host,                           // Pointer to the system host (root) structure
    UINT8       socket,                         // Socket Id
    UINT8       ch,                             // Channel number (0-based)
    UINT8       dimm,                           // Current dimm
    UINT8       rank,                           // Rank number (0-based)
    UINT8       strobe,                         // Strobe number (0-based)
    UINT8       bit,                            // Bit number
    GSM_LT      level,                          // CSM_LT - Level to program (DDR, VMSE, Buffer)
    GSM_GT      group,                          // CSM_GT - Parameter to program
    UINT8       mode,                           // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
    INT16       *value                          // Pointer to delay value or offset based on mode
  );

MRC_STATUS
GetSetTxDelayBit (
    PSYSHOST    host,                           // Pointer to the system host (root) structure
    UINT8       socket,                         // Socket Id
    UINT8       ch,                             // Channel number (0-based)
    UINT8       dimm,                           // Current dimm
    UINT8       rank,                           // Rank number (0-based)
    UINT8       strobe,                         // Strobe number (0-based)
    UINT8       bit,                            // Bit number
    GSM_LT      level,                          // CSM_LT - Level to program (DDR, VMSE, Buffer)
    GSM_GT      group,                          // CSM_GT - Parameter to program
    UINT8       mode,                           // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
    INT16       *value                          // Pointer to delay value or offset based on mode
  );

UINT8
GetVrefRange (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       vref                              // Vref value to convert
  );

MRC_STATUS
GetSetRxVref (
    PSYSHOST    host,                           // Pointer to the system host (root) structure
    UINT8       socket,                         // Socket Id
    UINT8       ch,                             // Channel number (0-based)
    UINT8       dimm,                           // Current dimm
    UINT8       rank,                           // Rank number (0-based)
    UINT8       strobe,                         // Strobe number (0-based)
    UINT8       bit,                            // Bit number
    GSM_LT      level,                          // CSM_LT - Level to program (DDR, VMSE, Buffer)
    GSM_GT      group,                          // CSM_GT - Parameter to program
    UINT8       mode,                           // 0 - do not read from cache, 1 - read from cache, 2 - use cache to
                                                //   program register (used for fast boot or S3)
    INT16       *value                          // Pointer to delay value or offset based on mode
  );

void
UpdateSafeTxVref (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  GSM_LT      level                             // CSM_LT - Level to program (DDR, VMSE, Buffer)
  );

UINT8
GetIOLatency (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank                              // Rank number (0-based)
  );

void
SetIOLatency (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT32      IOLatencyValue                    // New IO Latency value
  );

void
DecIoLatency(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  UINT16      centerPoint[MAX_CH][MAX_STROBE]   // Center point
  );

UINT8
GetRoundTrip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank                              // Rank number (0-based)
  );

UINT8
SetRoundTrip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       RoundTripValue                    // New Round Trip Latency value
  );

void
SetTrainingMode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       mode                              // Training mode
  );

UINT32
UpdateIoRegister (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       rank,                             // Rank number (0-based)
  UINT8       strobe,                           // Strobe number (0-based)
  UINT32      regOffset                         // Register offset
  );

void
StepChipCtlCKIOLatency(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT8       rank,                             // Rank
  UINT8       logRank                           // Logical rank
  );

void
RestoreChipCtlCKIOLatency(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT8       rank,                             // Rank
  UINT8       logRank                           // Logical rank
  );

void
PerformChipODTMPRDimmActivationStep(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                                               // Channel number (0-based)
  UINT8       dimm,                                             // Current dimm
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct // Pointer to CTL CLK structure
  );

void
GetOdtActTablePointers (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT32      *odtActTableSize,                 // Size of the ODT Act Table
  struct odtActStruct **odtActTablePtr          // Pointer to the ODT Act Table
);

struct odtValueStruct *
LookupOdtValue (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

void
SetDdr3OdtMatrix (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT8       rank                              // Rank
  );

void
SetOdtActMatrixRegs (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT32      rdOdtTableData,                   // Read ODT table data
  UINT32      wrOdtTableData                    // Write ODT table data
  );

UINT16
SetOdtConfigInOdtValueIndex (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );

void
GetOdtTiming(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      *rdOdtTiming,                     // Pointer to Read ODT Timings
  UINT32      *wrOdtTiming                      // Pointer to Write ODT Timings
  );

void
SetOdtTiming (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

void
SetSenseAmpODTDelays (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );


UINTN
EFIAPI
GetPcieSegMmcfgBaseAddress (
  IN COMMON_ADDRESS_STRUCT    *Address          // Address: A pointer of the address of the Common
                                                // Address Structure with Csr or CsrEx type
                                                // e.g. COMMON_CSR_EX_ADDR_TYPE or
                                                // COMMON_CSR_ADDR_TYPE.
  );

IN COMMON_PCIE_ADDR_TYPE
EFIAPI
GetPcieAddress (
  IN COMMON_ADDRESS_STRUCT    *Address          // Address: A pointer of the address of the Common
                                                // Address Structure with Csr or CsrEx type
                                                // e.g. COMMON_CSR_EX_ADDR_TYPE or
                                                // COMMON_CSR_ADDR_TYPE.
  );

UINT32
ReadPciCfg (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      reg                               // input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  );

void
WritePciCfg (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      reg,                              // input parameter "reg" uses the format in the Bus_Dev_Func_CFG.H files
  UINT32      data                              // Value to write
  );

UINT32
ReadCpuPciCfgEx (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       SocId,                            // CPU Socket Node number (Socket ID)
  UINT8       BoxInst,                          // Box Instance, 0 based
  UINT32      Offset                            // Register offset; values come from the auto generated header file
  );

void
WriteCpuPciCfgEx (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       SocId,                            // CPU Socket Node number (Socket ID)
  UINT8       BoxInst,                          // Box Instance, 0 based
  UINT32      Offset,                           // Register offset; values come from the auto generated header file
  UINT32      Data                              // Register data to be written
  );

UINT32
MemReadPciCfgEp (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ChIdOrBoxInst,                    // DDR/VMSE channel Id within a memory controller
                                                //   or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  UINT32      Offset                            // Register offset; values come from the auto generated header file
  );

void
MemWritePciCfgEp (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ChIdOrBoxInst,                    // DDR/VMSE channel Id within a memory controller
                                                //   or Box Instance (in case of non-MC boxes), 0 based, 0xFF if not used
  UINT32      Offset,                           // Register offset; values come from the auto generated header file
  UINT32      Data                              // Register data to be written
  );

UINT32
MemReadPciCfgMC (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       mcId,                             // Memory controller ID
  UINT32      Offset                            // Register offset; values come from the auto generated header file
  );

void
MemWritePciCfgMC (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       mcId,                             // Memory controller ID
  UINT32      Offset,                           // Register offset; values come from the auto generated header file
  UINT32      Data                              // Register data to be written
  );

UINT32
MemReadPciCfgMain (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      regBase                           // MMIO Reg address of first base device
  );

void
MemWritePciCfgMain (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      regBase,                          // MMIO Reg address of first base device
  UINT32      data                              // Data to write
  );

void
GetChipTimingData(
  PSYSHOST    ost,                              // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Pointer to WL Cleanup structure
  );

void
SetChipDQDQSTiming(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  INT16       offsetValue,                      // Offset Value
  UINT32*     CRAddDelay,                       // Return value from UpdateGlobalOffsets()
  INT16*      GlobalByteOff,                    // Return value from UpdateGlobalOffsets()
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Pointer to WL Cleanup structure
  );

void
TxPiSampleChipInit(
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

void
SetChipTxPiSampleTrainingMode(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

void
SetChipTxPiSampleTrainingTXDQSStrobesI(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  INT16       piDelay,                          // piDelay for nibble
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

void
SetChipTxPiSampleTrainingTXDQSStrobesII(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

void
SetupPiSettingsParams(
  PSYSHOST  host,                                           // Pointer to the system host (root) structure
  UINT8     socket,                                         // CPU Socket Node number (Socket ID)
  UINT8     step,                                           // The PI step to work on
  UINT16    *piSetting,                                     // The PI setting returned
  UINT16    *piStart,                                       // Test starting PI
  UINT16    *NumberOfCycle,                                 // Number of cycles
  struct  TrainingResults  (**trainRes)[MAX_CH][MAX_STROBE] // Training results
  );

UINT8
CheckDqSwizzleSupport (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );

void
WriteRCFnv(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       controlWordData,                  // Data to be written
  UINT8       controlWordAddr                   // Control word address (RDIMM_RC0A or RDIMM_RC0E)
  );

void
SetChipRTTPark(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

UINT8
GetWrDqDqsOffset (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
SetupChipPatternSequencer(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

void
UpdateGlobalOffsets (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       ch,                               // Channel number (0-based)
  INT16       TargetOffset,                     // Target offset for byte lanes
  UINT32      *CRAddDelay,                      // Return value from UpdateGlobalOffsets()
  INT16       *GlobalByteOff                    // Return value from UpdateGlobalOffsets()
  );

void
DoFinalChipSpecificWLCleanUp(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

VOID
ReceiveEnableInitChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

void
SetInitRoundTrip(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

VOID
RankRoundTrip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

void
SetIoLatComp(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

void
SetIOLatencyComp(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rtError,                          // Error status on current channel
  UINT32      tcrapOrg[MAX_CH]                  // tCRAP data to set
  );

void
SetLongPreamble(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  BOOLEAN     set                               // Set or clear long preamble enable bit
  );

void
SetupRecEnFineParam(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       *loopCount,                       // Number of cycles
  UINT8       *numCl                            // Number of cache lines
  );

void
EnableChipRecieveEnableMode(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                                               // Channel number (0-based)
  UINT8       strobe,                                           // Strobe (0-based)
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct // Pointer to CTL CLK structure
  );

void
EnableChipRecieveEnableModeII(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                                               // Channel number (0-based)
  UINT8       strobe,                                           // Strobe (0-based)
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct // Pointer to CTL CLK structure
  );

void
SetActToActPeriod(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      tcrapOrg[MAX_CH]                  // Array of per-channel tCRAP data
  );

void
RecEnTrainFeedback (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      csrReg,                           // CSR Register
  UINT16      piDelay                           // Pi Delay
  );

void
DisableChipChlRecEnOffsetTrainingMode(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // Socket Id
  UINT8       ch,                                               // Channel number (0-based)
  PEXECUTE_CTL_CLK_TEST_CHIP_STRUCT executeCtlCLKTestChipStruct // Pointer to CTL CLK structure
  );

UINT8
SkipReceiveEnable (
  PSYSHOST      host                            // Pointer to the system host (root) structure
  );

void
ReceiveEnableCleanup(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
AttemptColdBootFastChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
DetectBootDateOldChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
DetectIMCEnabledChangeChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
DetectMCAErrorsChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      *logData                          // Pointer to log data
  );

UINT32
DetectProcessorRemovedChip (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT32
DetectSocketPopulatedChangedChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
GetChnDisableChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

UINT32
GetDisable2dpcChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
GetDisable3dpcChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
GetDisableQRDimmChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT64_STRUCT
GetPpinChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

VOID
InitDdrFreqChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT32
SavedSetupDataChangedChip (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT32
SavedSetupDataChangedIMCChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

VOID
SetIMCEnabledChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

VOID
SetSetupOptionsChip (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

VOID
SetSteppingsChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
CalcFinalGap(
  UINT16      *gap,                             // Pointer to the final gap value
  UINT16      maxGap,                           // Maximum gap
  UINT16      gapDimm                           // Gap calculated based on current DIMM
  );

UINT8
CalctRRDD(
    PSYSHOST  host,                             // Pointer to the system host (root) structure
    UINT8     socket,                           // CPU Socket Node number (Socket ID)
    UINT8     ch,                               // Channel number (0-based)
    TT_VARS   *ttVars                           // Pointer to the structure for timing variables
    );

UINT8
CalctRRDR(
    PSYSHOST  host,                             // Pointer to the system host (root) structure
    UINT8     socket,                           // CPU Socket Node number (Socket ID)
    UINT8     ch,                               // Channel number (0-based)
    TT_VARS   *ttVars                           // Pointer to the structure for timing variables
    );

void
DramSpecCalctRWDD(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tRWDD                            // Pointer to the return value for tRWDD
  );

void
DramSpecCalctRWDR(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tRWDR                            // Pointer to the return value for tRWDR
  );

void
DramSpecCalctRWSR(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tRWSR                            // Pointer to the return value for tRWSR
  );

void
DramSpecCalctWRDD(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tWRDD                            // Pointer to the return value for tWRDD
  );

void
DramSpecCalctWRDR(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tWRDR                            // Pointer to the return value for tWRDR
  );

void
DramSpecCalctWWDD(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tWWDD                            // Pointer to the return value for tWWDD
  );

void
DramSpecCalctWWDR(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  TT_VARS     *ttVars,                          // Pointer to the structure for timing variables
  UINT8       *tWWDR                            // Pointer to the return value for tWWDR
  );

void
SetMR0Shadow (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      MR0Data                           // MR0 Data to set
  );

void
SetMR2Shadow (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT32      MR2Data                           // MR2 Data to set
  );

void
SetMR4Shadow (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      MR4Data                           // MR4 data to set
  );

void
SetMR5Shadow (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      MR5Data                           // MR5 Data to set
  );

void
CheckTxVrefSupport(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank (0-based)
  UINT8       strobe,                           // Strobe number (0-based)
  INT16       *zeroValue                        // Pointer that will return the current TxVref value
  );

void
EffectiveBusDelayInit(
  PSYSHOST    host,                                   // Pointer to the system host (root) structure
  UINT8       socket,                                 // CPU Socket Node number (Socket ID)
  UINT8       ch,                                     // Channel number (0-based)
  INT16       *CwlAdj,                                // Pointer to CwlAdj
  UINT32      *IOComp,                                // Pointer to IOComp
  UINT16      *IntDelta,                              // Pointer to IntDelta
  UINT32      *longPreambleEnable                     // Pointer to the long preamble enable
  );

void
SetRestoreTimings(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

void
WriteFastBootTimings(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank (0-based)
  UINT8       strobe                            // Strobe number (0-based)
  );

UINT32
GetDataTrainFeedback (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe                            // Strobe number (0-based)
  );

UINT32
SetRfOn (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      rfOn                              // rfOn setting to apply
  );

void
EvalResSetRanksDone (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       scope,                            // Margin granularity
  UINT32      *ranksDone                        // Array of bits showing if rans are done.
                                                //   If the bit is set the rank is done
  );

void
PerBitDeskew(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  GSM_GT      group                             // Parameter to center
                                                //   RxDqsDelay   0: Center Rx DqDqs
                                                //   TxDqDelay    1: Center Tx DqDqs
  );

void
SetCkeOverride (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       ckeMask                           // CKE settings to apply
  );

void
AssertChipODTOveride(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      wrOdtTableData                    // ODT Signals
  );

void
DeAssertChipODTOveride(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

UINT32
ChipLateCmdClk(
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT32
ChipAggressiveCMDTest(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
ChipSet3NTimingandSaveOrigSetting(
  PSYSHOST    host,                                           // Pointer to the system host (root) structure
  UINT8       socket,                                         // Socket Id
  UINT8       ch,                                             // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct // Pointer to parity checking
  );

void
ChipRestoreOrigCMDtiming(
  PSYSHOST    host,                                            // Pointer to the system host (root) structure
  UINT8       socket,                                          // Socket Id
  UINT8       ch,                                              // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct  // Pointer to parity checking
  );

void
EnableChipFNV(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

void
EarlyCmdSignalTest(
  PSYSHOST    host ,                            // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      chBitmask                         // Channels to execute test on
  );

UINT32
EarlyCmdSignalStop(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      chBitmask,                        // Channels to execute test on
  UINT8       dimm,                             // Current dimm
  UINT32      bwSerr[MAX_CH][3]                 // Bitwise error array
  );

void
DisableChipFNV(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

void
DisableChipLRDIMMMPROverrideMode(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

void
EnableChipLRDIMMPreambleTrMode(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
DisableChipLRDIMMPreambleTrMode(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
DoChipCompatibleRTT_PARKRestore(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Rank number (0-based)
  );

BOOLEAN
DoesChipDetectCSorLRRnkDimm(
  UINT8       ch,                               // Channel number (0-based)
  struct channelNvram (*channelNvList)[MAX_CH]  // Channel NVRam list
  );

UINT8
GetChipWriteLevelingCleanUpOffset(
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
GetChipParamType (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       Param                             // Parameter input to convert to parameter type
  );

void
GetChipTiPiSamplepiDelayStartandEnd(
  PSYSHOST  host,                               // Pointer to the system host (root) structure
  UINT8     socket,                             // CPU Socket Node number (Socket ID)
  INT16     *piDelayStart,                      // Pointer to the loop start index
  UINT16    *piDelayEnd                         // Pointer to the loop end index
  );

void
GetCmdMarginsSweepInit(
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Pointer to struct to initialize
  );

void
UpdateCmdMarginMasks(
  PSYSHOST    host,                                   // Pointer to the system host (root) structure
  UINT8       socket,                                 // Socket Id
  UINT8       ch,                                     // Channel number (0-based)
  INT16       piIndex,                                // PI index
  INT16       offset,                                 // Max PI index
  UINT8 errorResult[MAX_CH][PAR_TEST_RANGE],          // Error results
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Pointer to structure
  );

void
SetChipSenseAmpOffset(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       nibble,                           // Current nibble
  UINT32      OffsetData                        // Sense amp offset data
  );

BOOLEAN
ShouldChipDisableDDR4LRDIMMWriteLeveling(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

BOOLEAN
ShouldChipEnableDDR4LRDIMMHostSideTraining(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Rank number (0-based)
  );

BOOLEAN
ShouldChipPerformEarlyCtlClk(
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

BOOLEAN
ShouldChipSetNormalCMDTiming(
  PSYSHOST    host,                                                // Pointer to the system host (root) structure
  UINT8       socket,                                              // Socket Id
  UINT8       ch,                                                  // Channel number (0-based)
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct  // Normal Command Timing structure
  );

BOOLEAN
ShouldChipSkipElyCMDTrAsPrtyNotSprtd(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
SetRankTerminationMask(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

void
WriteChipWrADDDelays(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  INT16       offsetValue,                      // Offset for byte lanes
  UINT32      *CRAddDelay,                      // Add delays
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Write Leveling clean up structure pointer
  );

UINT8
CheckPORDDRVdd (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       vdd                               // Vdd (1.5v or 1.35V)
  );

UINT8
GetPORDDRFreq (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       vdd                               // Vdd (1.5v or 1.35V)
  );

UINT32
CheckDimmType (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
CheckChDoneMask(
  PSYSHOST    host,                                   // Pointer to the system host (root) structure
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Mask data to check
  );

BOOLEAN
DoesChipSupportParityChecking(
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
ChipEnableParity(
  PSYSHOST    host,                                         // Pointer to the system host (root) structure
  UINT8       socket,                                       // CPU Socket Node number (Socket ID)
  UINT8       ch,                                           // Channel number (0-based)
PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct // Pointer to Parity struct
);

void
ChipEnableCheckParity(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT16      *mode                             // mode is either MODE_VA_DESELECT or MODE_DESELECT
  );

UINT8
CheckParityWorkaroundChipSpecific(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT16      dateCode                          // Data code for parity test
  );

UINT32
ChipCollectParityResults (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT32      bwSerr[MAX_CH][3]                 // Bit wise status structure
  );

BOOLEAN
ShouldChipClearCMDMGNParityError(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT32      status                            // Status of existing parity error
  );

void
ChipClearParityResults (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
ChipClearParityResultsAlt (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
ChipSpecificErrorEnable(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // Socket Id
  UINT8       ch,                                               // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct  // Pointer to parity checking
  );

void
ChipSpecificParityCheckDisable(
  PSYSHOST    host,                                             // Pointer to the system host (root) structure
  UINT8       socket,                                           // Socket Id
  UINT8       ch,                                               // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct  // Pointer to parity checking
  );

void
ChipClearDimmErrRslts(
  UINT8       ch,                                             // Channel number (0-based)
  UINT8       errorResult[MAX_CH][CHIP_CMD_CLK_TEST_RANGE],   // Error results
  UINT32      *chBitmask,                                     // Bit mask of channels
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct         // Channel data
  );

UINT32
ReadTrainFeedback (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe                            // Strobe (0-based)
  );

UINT8
GetTrainingResultIndex(
  UINT8       step,                             // Current training step
  UINT16      piSetting,                        // Pi Setting
  UINT16      piStart                           // Pi Start
  );

UINT32
GetErrorResults (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

void
GetResultsWL (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT16      piDelay                           // Pi Delay
  );

UINT32
LrDimmReadTrainFeedback (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT8       bit                               // Bit
  );

void
SetChipGetCtlClkResLowerNibbleStrobeFeedback(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      *csrReg,                          // CSR Register
  UINT32      *strobeFeedback                   // Pointer to or in the lower nibble
  );

void
SetChipGetCtlClkResUpperNibbleStrobeFeedback(
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      csrReg,                           // CSR Register
  UINT32      *strobeFeedback                   // Pointer to or in the upper nibble
  );

void
WdbPreLoadChip (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       NumberOfWdbLines,                 // Number of cachelines to load
  UINT8       *wdbLines,                        // Pointer to data to write to the WDB
  UINT8       StartCachelineIndex               // Start offset/cacheline number on the WDB.
  );

UINT32
GetStickyScratchpad (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       scratchpad                        // Scratch pad to read
  );

void
SetStickyScratchpad (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       scratchpad,                       // Scratch pad to read
  UINT32      data                              // Data to be written
 );

UINT8
Is2HA (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
Is1HA (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
IsBDXEP (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
IsDESku (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

void
SetDisable2cycBypass (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       data                              // Data for disable 2cyc bypass
  );

UINT8
SetMinGoodInit (
  PSYSHOST    host                              // Pointer to the system host (root) structure
  );

UINT8
IsLrdimmPresent (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

void
UpdateMinMax(
  UINT16      val,                              // New value to evaluate
  UINT16      *minVal,                          // Current minimum value
  UINT16      *maxVal                           // Current maximum value
  );

VOID
SetConfigBeforeRmt(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT8
GetCurrentTimingMode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

UINT8
SetTimingMode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       cmdTiming                         // Timing value to set
  );

VOID
SetMprTrainMode (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       enable                            // Enable or disable iMC to issue activates and precharges
  );

void
SetVdd (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket                            // Socket Id
  );

UINT16
StartBackOver (
  UINT8       step,                             // Current PI step
  UINT16      piStart                           // PI start value
  );

void
ResetSystemEx (
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       resetType                         // Reset type
  );

UINT8
GetSbspSktId(
    PSYSHOST  host                              // Pointer to the system host (root) structure
    );

VOID RcDeadLoop (
  UINT8 haltOnError
  );

#ifdef MEM_NVDIMM_EN
void
ChipDeassertCKE(
  PSYSHOST    host,                             // Pointer to the system host (root) structure
  UINT8       socket,                           // Socket Id                
  UINT8       ch,                               // Channel number (0-based) 
  UINT8       ckeMask
);
#endif

UINT32
PreAdvancedMemTest (
  PSYSHOST    host,                            // Pointer to the system host (root) structure
  UINT8       socket,                          // Socket Id                
  UINT8       ch                               // Channel number (0-based) 
  );

VOID
PostAdvancedMemTest (
  PSYSHOST    host,                            // Pointer to the system host (root) structure
  UINT8       socket,                          // Socket Id                
  UINT8       ch,                              // Channel number (0-based) 
  UINT32      scrambleConfigOrg                // DDR scrambler config returned by PreAdvancedMemTest
  );

UINT32
CpgcMemTestMATS (
  PSYSHOST  host,                            // Pointer to the system host (root) structure
  UINT8     socket,                          // Socket Id                
  UINT32    ddrChEnMap,                      // Bit-map of enabled channels
  UINT8     testType,                        // Type of Advanced Memory Test
  UINT8     mode,                            // Type of sequence MT_CPGC_WRITE, MT_CPGC_READ, or MT_CPGC_READ_WRITE
  UINT8     seqDataInv[MT_MAX_SUBSEQ],       // Enables pattern inversion per subsequence
  UINT64_STRUCT patternQW[MT_PATTERN_DEPTH], // 64-bit wide data pattern to use per UI
  UINT8     uiShl[MT_PATTERN_DEPTH],         // Enables pattern rotation per UI
  UINT8     numCacheLines,                   // Number of cachelines to use in WDB
  UINT8     patternDepth,                    // Number of UI to use in patternQW
  UINT8     direction                        // Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN
  );

BOOLEAN
IsChipPageTableTimerEnabled(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch
  );

void
SetChipPageTableTimer(
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     state
  );

void
EnforceWWDR(
            PSYSHOST host,
            UINT8    ch,
            UINT8    *tWWDR
);

void
EnforceWWDD(
            PSYSHOST host,
            UINT8    ch,
            UINT8    *tWWDD
);

void
AddChipRxDqsOffset(
                       PSYSHOST host,
                       UINT8 socket,
                       UINT8     ch,
                       UINT8     dimm,
                       UINT8     rank,
                       UINT8     strobe,
                       UINT16 *offset
);
#endif // _CHIP_API_H_
