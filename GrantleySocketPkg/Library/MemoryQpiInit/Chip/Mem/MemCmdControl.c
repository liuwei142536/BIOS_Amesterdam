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
 ************************************************************************
 *
 *  PURPOSE:
 *
 *      This file contains memory detection and initialization for
 *      IMC and DDR3 modules compliant with JEDEC specification.
 *
 ************************************************************************/

#include "SysFunc.h"
#include "CmdCtlDelay.h"


//
// Internal data types
//

#define MAX_SIGNALS   50
#define MAX_CMD_GROUPALL_3DS 3
#define X_OVER_ODD_FUB_OFFSET   32

struct signalIOGroup signals[MAX_SIGNALS] = {RAS_N, CmdGrp0, {0, SIDE_B}, CAS_N, CmdGrp0, {3, SIDE_B}, WE_N, CmdGrp0, {3, SIDE_B},
                                             BA0, CmdGrp2, {2, SIDE_A}, BA1, CmdGrp1, {5, SIDE_B}, BA2, CmdGrp1, {5, SIDE_A},
                                             A0, CmdGrp0, {4, SIDE_B}, A1, CmdGrp0, {1, SIDE_B}, A2, CmdGrp0, {1, SIDE_B},
                                             A3, CmdGrp1, {4, SIDE_A}, A4, CmdGrp1, {4, SIDE_A}, A5, CmdGrp1, {1, SIDE_A},
                                             A6, CmdGrp1, {1, SIDE_A}, A7, CmdGrp1, {3, SIDE_A}, A8, CmdGrp1, {3, SIDE_A},
                                             A9, CmdGrp1, {0, SIDE_A}, A10, CmdGrp0, {0, SIDE_B}, A11, CmdGrp1, {0, SIDE_A},
                                             A12, CmdGrp2, {2, SIDE_A}, A13, CmdGrp2, {5, SIDE_B}, A14, CmdGrp2, {3 , SIDE_B},
                                             A15, CmdGrp2, {3, SIDE_B}, A16, CmdGrp0, {0, SIDE_B},
                                             CS0_N, CtlGrp0, {6, SIDE_B}, CS1_N, CtlGrp1, {7, SIDE_B}, CS2_N, CtlGrp0, {8, SIDE_B},
                                             CS3_N, CtlGrp0, {8, SIDE_B}, CS4_N, CtlGrp2, {9, SIDE_B}, CS5_N, CtlGrp3, {10, SIDE_B},
                                             CS6_N, CtlGrp2, {6, SIDE_A}, CS7_N, CtlGrp2, {7, SIDE_A}, CS8_N, CtlGrp4, {8, SIDE_A},
                                             CS9_N, CtlGrp4, {9, SIDE_A},
                                             CKE0, CtlGrp0, {0, SIDE_A}, CKE1, CtlGrp1, {1, SIDE_A}, CKE2, CtlGrp2, {2, SIDE_A},
                                             CKE3, CtlGrp3, {3, SIDE_A}, CKE4, CtlGrp4, {4, SIDE_A}, CKE5, CtlGrp4, {5, SIDE_A},
                                             ODT0, CtlGrp0, {0, SIDE_B}, ODT1, CtlGrp1, {1, SIDE_B}, ODT2, CtlGrp2, {2, SIDE_B},
                                             ODT3, CtlGrp3, {3, SIDE_B}, ODT4, CtlGrp4, {4, SIDE_B}, ODT5, CtlGrp4, {5, SIDE_B},
                                             PAR, CmdGrp0, {4, SIDE_B},
                                             BG0, CmdGrp2, {5, SIDE_A}, BG1, CmdGrp2, {5, SIDE_A}, ACT_N, CmdGrp2, {2, SIDE_B},
                                             C2, CmdGrp2, {2, SIDE_B}
                                            };

struct signalIOGroup signalsA0[MAX_SIGNALS] = {RAS_N, CmdGrp0, {0, SIDE_B}, CAS_N, CmdGrp0, {0, SIDE_B}, WE_N, CmdGrp0, {0, SIDE_B},
                                             BA0, CmdGrp2, {2, SIDE_A}, BA1, CmdGrp1, {2, SIDE_B}, BA2, CmdGrp1, {2, SIDE_A},
                                             A0, CmdGrp0, {1, SIDE_B}, A1, CmdGrp0, {1, SIDE_B}, A2, CmdGrp0, {1, SIDE_B},
                                             A3, CmdGrp1, {1, SIDE_A}, A4, CmdGrp1, {1, SIDE_A}, A5, CmdGrp1, {1, SIDE_A},
                                             A6, CmdGrp1, {1, SIDE_A}, A7, CmdGrp1, {0, SIDE_A}, A8, CmdGrp1, {0, SIDE_A},
                                             A9, CmdGrp1, {0, SIDE_A}, A10, CmdGrp0, {0, SIDE_B}, A11, CmdGrp1, {0, SIDE_A},
                                             A12, CmdGrp2, {2, SIDE_A}, A13, CmdGrp2, {2, SIDE_B}, A14, CmdGrp2, {0, SIDE_B},
                                             A15, CmdGrp2, {0, SIDE_B}, A16, CmdGrp0, {0, SIDE_B},
                                             CS0_N, CtlGrp0, {6, SIDE_B}, CS1_N, CtlGrp1, {7, SIDE_B}, CS2_N, CtlGrp0, {8, SIDE_B},
                                             CS3_N, CtlGrp0, {8, SIDE_B}, CS4_N, CtlGrp2, {9, SIDE_B}, CS5_N, CtlGrp3, {10, SIDE_B},
                                             CS6_N, CtlGrp2, {6, SIDE_A}, CS7_N, CtlGrp2, {7, SIDE_A}, CS8_N, CtlGrp4, {8, SIDE_A},
                                             CS9_N, CtlGrp4, {9, SIDE_A},
                                             CKE0, CtlGrp0, {0, SIDE_A}, CKE1, CtlGrp1, {1, SIDE_A}, CKE2, CtlGrp2, {2, SIDE_A},
                                             CKE3, CtlGrp3, {3, SIDE_A}, CKE4, CtlGrp4, {4, SIDE_A}, CKE5, CtlGrp4, {5, SIDE_A},
                                             ODT0, CtlGrp0, {0, SIDE_B}, ODT1, CtlGrp1, {1, SIDE_B}, ODT2, CtlGrp2, {2, SIDE_B},
                                             ODT3, CtlGrp3, {3, SIDE_B}, ODT4, CtlGrp4, {4, SIDE_B}, ODT5, CtlGrp4, {5, SIDE_B},
                                             PAR, CmdGrp0, {1, SIDE_B},
                                             BG0, CmdGrp2, {2, SIDE_A}, BG1, CmdGrp2, {2, SIDE_A}, ACT_N, CmdGrp2, {2, SIDE_B},
                                             C2, CmdGrp2, {2, SIDE_B}
                                            };

// Map Platform CMD Groups to IO CMD Groups
// Platform Group 0 => IO Group
// Platform Group 1 => IO Group
// Platform Group 2 => IO Group
struct ioGroup platformCmdGroup0A0[MAX_CMD_GROUP0_A0] = {{0,SIDE_B},{1,SIDE_B}};
struct ioGroup platformCmdGroup1A0[MAX_CMD_GROUP1_A0] = {{0,SIDE_A},{1,SIDE_A}};
struct ioGroup platformCmdGroup2A0[MAX_CMD_GROUP2_A0] = {{2,SIDE_A},{2,SIDE_B}};
struct ioGroup platformCmdGroupAllA0[MAX_CMD_GROUPALL_A0] =
   {{0,SIDE_A},{0,SIDE_B},{1,SIDE_A},{1,SIDE_B},{2,SIDE_A},{2,SIDE_B}};

struct ioGroup platformCmdGroup0[MAX_CMD_GROUP0] = {{0,SIDE_B},{1,SIDE_B},{3,SIDE_B},{4,SIDE_B}};
struct ioGroup platformCmdGroup1[MAX_CMD_GROUP1] = {{0,SIDE_A},{1,SIDE_A},{3,SIDE_A},{4,SIDE_A}};
struct ioGroup platformCmdGroup2[MAX_CMD_GROUP2] = {{2,SIDE_A},{2,SIDE_B},{5,SIDE_A},{5,SIDE_B}};
struct ioGroup platformCmdGroupAll[MAX_CMD_GROUPALL] =
   {{0,SIDE_A},{0,SIDE_B},{1,SIDE_A},{1,SIDE_B},{2,SIDE_A},{2,SIDE_B},
    {3,SIDE_A},{3,SIDE_B},{4,SIDE_A},{4,SIDE_B},{5,SIDE_A},{5,SIDE_B}};
struct ioGroup platformCmdGroupAll3DS[MAX_CMD_GROUPALL_3DS] =
   {{8, SIDE_B}, {6, SIDE_A}, {7, SIDE_A}};

// Map Platform CTL Groups to IO CTL Groups
// Platform Group 0 => IO Group 0A,0B,6A,8A
// Platform Group 1 => IO Group 1A,1B,7A
// Platform Group 2 => IO Group 2A,2B,6B,7B,9A
// Platform Group 3 => IO Group 3A,3B,10A
// Platform Group 4 => IO Group 4A,4B,5A,5B,8B,9B
struct ioGroup platformCtlGroup0[MAX_CTL_GROUP0] = {{0,SIDE_B},{0,SIDE_A},{6,SIDE_B},{8,SIDE_B}};
struct ioGroup platformCtlGroup1[MAX_CTL_GROUP1] = {{1,SIDE_B},{1,SIDE_A},{7,SIDE_B}};
struct ioGroup platformCtlGroup2[MAX_CTL_GROUP2] = {{2,SIDE_B},{2,SIDE_A},{6,SIDE_A},{7,SIDE_A},{9,SIDE_B}};
struct ioGroup platformCtlGroup3[MAX_CTL_GROUP3] = {{3,SIDE_B},{3,SIDE_A},{10,SIDE_B}};
struct ioGroup platformCtlGroup4[MAX_CTL_GROUP4] = {{4,SIDE_B},{4,SIDE_A},{5,SIDE_B},{5,SIDE_A},{8,SIDE_A},{9,SIDE_A}};
struct ioGroup platformCtlGroupAll[MAX_CTL_GROUPALL] =
   {{0,SIDE_A},{0,SIDE_B},{1,SIDE_A},{1,SIDE_B},{2,SIDE_A},{2,SIDE_B},{3,SIDE_A},{3,SIDE_B},{4,SIDE_A},{4,SIDE_B}, {5,SIDE_A},
    {5,SIDE_B},{6,SIDE_A},{6,SIDE_B},{7,SIDE_A},{7,SIDE_B},{8,SIDE_A},{8,SIDE_B},{9,SIDE_A},{9,SIDE_B},{10,SIDE_B}};

struct ioGroup platformCmdCtlGroupAll[MAX_CMDCTL_GROUPALL] = {
  {1, SIDE_A},{3, SIDE_B},{2, SIDE_B},{1, SIDE_B},{0, SIDE_B},{7, SIDE_B},
  {8, SIDE_B},{9, SIDE_B},{10, SIDE_B},
  {11, SIDE_B},{7, SIDE_A},{8, SIDE_A},{9, SIDE_A},
  {3, SIDE_A},{12, SIDE_A},{5, SIDE_A},{6, SIDE_A},
  {4, SIDE_A},{13, SIDE_A},
  {4, SIDE_B},{11, SIDE_A},{2, SIDE_A},
  {0, SIDE_A},{10, SIDE_A}
};
//-----------------------------------------------------------------------------------
//
// This const array use as DDRIO convert table. Each index will be convert to real address.
//
const UINT32 CmdCtlAddressConvertTable[2] = {0x0, 0x600};

// Define maximums for CMD, CTL and CK for both processors
#define MAX_CMD_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CTL_DELAY   (3*64+63)   // Logic value of 3 and Pi value of 63
#define MAX_CK_DELAY    128
//-----------------------------------------------------------------------------------

//
// Local Prototypes
//
void   EvaluateCMDMargins(PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 ckEnabled[MAX_CLK], UINT8 ctlIndex[MAX_CLK],
         INT16 cmdLeft[][MAX_CLK], INT16 cmdRight[][MAX_CLK], INT16 cmdOffset[], UINT8 evaluateCIDs);
MRC_STATUS SignalToPiGroup(PSYSHOST host, GSM_CSN sig, struct signalIOGroup *signalGroup);
MRC_STATUS GetSetCmdVrefHostSide (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 mode, INT16 *value);
MRC_STATUS GetSetCmdVrefFnv (PSYSHOST host, UINT8 socket, UINT8 ch, UINT8 dimm, UINT8 mode, INT16 *value);

MRC_STATUS
GetSetSignal (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  GSM_LT    level,
  GSM_CSN   sig,
  UINT8     mode,
  INT16     *value
  )
/*++
  Multi-use function to either get or set signal delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param level:    IO level to access
  @param sig:      Enumerated signal name
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

--*/
{
  MRC_STATUS            status;
#ifdef SERIAL_DBG_MSG
  //char                  strBuf0[128];
  //char                  strBuf1[128];
#endif  // SERIAL_DBG_MSG
  struct signalIOGroup  signalGroup;

  status = MRC_STATUS_SUCCESS;
  //
  // Get the silicon pi group this signal belongs to
  //
  status = SignalToPiGroup (host, sig, &signalGroup);

  //MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
  //              "signal %s, platform group %s, num %d, side %d\n",
  //              GetSignalStr(signalGroup.sig, (char *)&strBuf0),
  //              GetPlatformGroupStr(signalGroup.platformGroup, (char *)&strBuf1),
  //              signalGroup.group.num, signalGroup.group.side));

  if (status == MRC_STATUS_SUCCESS) {
    if (CheckCMDGroup(signalGroup.platformGroup)) {
      status = GetSetCmdDelay (host, socket, ch, signalGroup.group.num, signalGroup.group.side, mode, value);
    } else if (CheckCTLGroup(signalGroup.platformGroup)) {
      status = GetSetCtlDelay (host, socket, ch, signalGroup.group.num, signalGroup.group.side, mode, value);
    } else {
      status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    }
  }

  return status;
} // GetSetSignal

MRC_STATUS
SignalToPiGroup (
  PSYSHOST              host,
  GSM_CSN               sig,
  struct signalIOGroup  *signalGroup
  )
/*++
  Gets the silicon pi group from the signal name

  @param host:     Pointer to SYSHOST
  @param sig:      Enumerated signal name

  @retval MRC_STATUS

--*/
{
  UINT8       sigLoop;
  MRC_STATUS  status;
#ifdef SERIAL_DBG_MSG
  char        strBuf0[128];
#endif  // SERIAL_DBG_MSG

  for (sigLoop = 0; sigLoop < MAX_SIGNALS; sigLoop++) {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      if (signalsA0[sigLoop].sig == sig) {
        *signalGroup = signalsA0[sigLoop];
        break;
      }
    } else {
      if (signals[sigLoop].sig == sig) {
        *signalGroup = signals[sigLoop];
        break;
      }
    }
  } // sigLoop loop

  if (sigLoop == MAX_SIGNALS) {
    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "Unable to find signal %s\n", GetSignalStr(sig, (char *)&strBuf0)));
    status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  } else {
    status = MRC_STATUS_SUCCESS;
  }

  return status;
} // SignalToPiGroup

void UpdateMinMax(
  UINT16  val,
  UINT16  *minVal,
  UINT16  *maxVal
  )
/*++
  Updates provided min and max values based on an input

  @param val:    New value to evaluate
  @param minVal: Current minimum value
  @param maxVal: Current maximum value

  @retval minVal and maxVal

--*/
{
  if (val < *minVal) *minVal = val;
  if (val > *maxVal) *maxVal = val;
} // UpdateMinMax

MRC_STATUS
GetSetCtlGroupDelay (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value,
  UINT16    *minVal,
  UINT16    *maxVal
  )
/*++
  Muli-use function to either get or set control delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param group:    Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program
  @param *minVal:  Current minimum control delay
  @param *maxVal:  Current maximum control delay


  @retval minVal, maxVal and MRC_STATUS

--*/
{
  UINT8           maxPlatformGroup;
  UINT8           maxIoGroup;
  UINT8           iog;
  UINT8           iogNum;
  UINT8           iogSide;
  struct ioGroup  *iogPtr;
  MRC_STATUS      status = MRC_STATUS_SUCCESS;

  maxPlatformGroup = (UINT8)group + 1;

  // Associate CTL groups for RDIMM
  if (host->nvram.mem.dimmTypePresent == RDIMM) {    // or LRDIMM
    if ((group == CtlGrp0) || (group == CtlGrp2)) maxPlatformGroup++;
  }

  for ( ; group < maxPlatformGroup; group++) {

    switch (group) {
    case CtlGrp0:
      maxIoGroup = MAX_CTL_GROUP0;
      iogPtr = platformCtlGroup0;
      break;
    case CtlGrp1:
      maxIoGroup = MAX_CTL_GROUP1;
      iogPtr = platformCtlGroup1;
      break;
    case CtlGrp2:
      maxIoGroup = MAX_CTL_GROUP2;
      iogPtr = platformCtlGroup2;
      break;
    case CtlGrp3:
      maxIoGroup = MAX_CTL_GROUP3;
      iogPtr = platformCtlGroup3;
      break;
    case CtlGrp4:
      maxIoGroup = MAX_CTL_GROUP4;
      iogPtr = platformCtlGroup4;
      break;
    case CtlAll:
      maxIoGroup = MAX_CTL_GROUPALL;
      iogPtr = platformCtlGroupAll;
      break;
    default:
      maxIoGroup = 0;
      iogPtr = platformCtlGroup0;
      status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      MemDebugPrint((host, SDBG_MEM_TRAIN, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "Ctl platform group not found - %d\n", group));
      // Assert error
    }

    if (status == MRC_STATUS_SUCCESS) {
      for (iog = 0; iog < maxIoGroup; iog++) {

        // Get IO Group number and side
        iogNum = (iogPtr + iog)->num;
        iogSide = (iogPtr + iog)->side;

        // Program the IO delay offset
        status = GetSetCtlDelay (host, socket, ch, iogNum, iogSide, mode, value);

        // Update min/max values
        UpdateMinMax(*value, minVal, maxVal);
      } // iog loop
    } // status
  }

  return status;
} // GetSetCtlGroupDelay

MRC_STATUS
GetSetCmdGroupDelay (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  GSM_GT    group,
  UINT8     mode,
  INT16     *value,
  UINT16    *minVal,
  UINT16    *maxVal
  )
/*++
  Muli-use function to either get or set command delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param group:    Group number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program
  @param *minVal:  Current minimum command delay
  @param *maxVal:  Current maximum command delay


  @retval minVal and maxVal

--*/
{
  UINT8           maxIoGroup;
  UINT8           iog;
  UINT8           iogNum;
  UINT8           iogSide;
  MRC_STATUS      status = MRC_STATUS_SUCCESS;
  struct ioGroup  *iogPtr;

  if (group == CmdVref) {
    status = GetSetCmdVref (host, socket, ch, 0, mode, value);
  } else {
    if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
      switch (group) {
        case CmdGrp0:
          maxIoGroup = MAX_CMD_GROUP0_A0;
          iogPtr = platformCmdGroup0A0;
          break;
        case CmdGrp1:
          maxIoGroup = MAX_CMD_GROUP1_A0;
          iogPtr = platformCmdGroup1A0;
          break;
        case CmdGrp2:
          maxIoGroup = MAX_CMD_GROUP2_A0;
          iogPtr = platformCmdGroup2A0;
          break;
        case CmdAll:
          maxIoGroup = MAX_CMD_GROUPALL_A0;
          iogPtr = platformCmdGroupAllA0;
          break;
        default:
          maxIoGroup = MAX_CMD_GROUPALL_A0;
          iogPtr = platformCmdGroupAllA0;
          status = MRC_STATUS_GROUP_NOT_SUPPORTED;
          break;
      }
    } else {
      switch (group) {
        case CmdGrp0:
          maxIoGroup = MAX_CMD_GROUP0;
          iogPtr = platformCmdGroup0;
          break;
        case CmdGrp1:
          maxIoGroup = MAX_CMD_GROUP1;
          iogPtr = platformCmdGroup1;
          break;
        case CmdGrp2:
          maxIoGroup = MAX_CMD_GROUP2;
          iogPtr = platformCmdGroup2;
          break;
        case CmdAll:
          maxIoGroup = MAX_CMD_GROUPALL;
          iogPtr = platformCmdGroupAll;
          break;
        default:
          maxIoGroup = MAX_CMD_GROUPALL;
          iogPtr = platformCmdGroupAll;
          status = MRC_STATUS_GROUP_NOT_SUPPORTED;
          break;
      }
    }

    if (status == MRC_STATUS_SUCCESS) {
      for (iog = 0; iog < maxIoGroup; iog++) {

        // Get IO Group number and side
        iogNum = (iogPtr + iog)->num;
        iogSide = (iogPtr + iog)->side;

        status = GetSetCmdDelay (host, socket, ch, iogNum, iogSide, mode, value);

        // Update min/max values
        UpdateMinMax(*value, minVal, maxVal);
      }
    } // status
  }

  return status;
} // GetSetCmdGroupDelay

MRC_STATUS
GetSetCmdDelay (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    ioGroup,
  UINT8    side,
  UINT8    mode,
  INT16    *value
  )
/*++
  Muli-use function to either get or set command delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param ioGroup:  Group number
  @param side:     Side number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

--*/
{
  UINT32                                          address = 0;
  UINT32                                          csrReg = 0;
  UINT8                                           logicVal;
  UINT8                                           piVal;
  UINT16                                          curVal;
  UINT16                                          piDelay;
  UINT16                                          curValZone;
  UINT16                                          piDelayZone;
  UINT16                                          xoverOffset;
  UINT16                                          oddFubOffset;
  UINT16                                          invertPiClkBits;
  UINT16                                          maxVal = 0;
  INT16                                           tmp;
  UINT8                                           regIndex;
  struct socketNvram                              *socketNvram;
  DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT      cmdCsr;
  DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_STRUCT     cmdCsr2;
  DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_STRUCT      cmdCsr3;
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT      ddrCRCmdTraining;
  DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_BDX_STRUCT ddrCRCmdControls2Cmd;
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXTMCC_BDX_STRUCT ddrCrCtlRanksUsed;

  socketNvram = &host->nvram.mem.socket[socket];

  cmdCsr.Data = 0;
  cmdCsr2.Data = 0;
  cmdCsr3.Data = 0;
  ddrCRCmdControls2Cmd.Data = 0;
  ddrCRCmdTraining.Data = 0;

  //
  // Maximum CMD delay
  //
  maxVal = MAX_CMD_DELAY;

  regIndex = ioGroup / 3;

  //
  // Point to the correct register
  //
  switch (regIndex) {
    case 0:
      address = DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
    case 1:
      address = DDRCRCMDPICODING2_CMDN_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
    case 2:
      address = DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
  }

  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
    switch (side)  {
      case SIDE_A:
        ddrCRCmdTraining.Data = socketNvram->channelList[ch].ddrCRCmdTrainingCmdN;
        break;
      case SIDE_B:
        ddrCRCmdTraining.Data = socketNvram->channelList[ch].ddrCRCmdTrainingCmdS;
        break;
    }
  }


  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    csrReg = MemReadPciCfgEp (host, socket, ch, address);
    switch (regIndex) {
      case 0:
        cmdCsr.Data = csrReg;
        break;
      case 1:
        cmdCsr2.Data = csrReg;
        break;
      case 2:
        cmdCsr3.Data = csrReg;
        break;
    }

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      switch (side)  {
        case SIDE_A:
          // Read xover training values from cache, read invertpiclk values from register
          ddrCRCmdControls2Cmd.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG);
          break;
        case SIDE_B:
          // Read xover training values from cache, read invertpiclk values from register
          ddrCRCmdControls2Cmd.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG);
          break;
      }
    }
  } else {
    //
    // Read from cache
    //
    switch (regIndex) {
      case 0:
        cmdCsr.Data = socketNvram->channelList[ch].cmdCsr[side][regIndex];
        break;
      case 1:
        cmdCsr2.Data = socketNvram->channelList[ch].cmdCsr[side][regIndex];
        break;
      case 2:
        cmdCsr3.Data = socketNvram->channelList[ch].cmdCsr[side][regIndex];
        break;
    }

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      switch (side)  {
        case SIDE_A:
          // Read xover training values from cache, read invertpiclk values from register
          ddrCRCmdControls2Cmd.Data = socketNvram->channelList[ch].ddrCRCmdControls2CmdN;
          break;
        case SIDE_B:
          // Read xover training values from cache, read invertpiclk values from register
          ddrCRCmdControls2Cmd.Data = socketNvram->channelList[ch].ddrCRCmdControls2CmdS;
          break;
      }
    }
  }

  switch (ioGroup) {
    case 0:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay0;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode0;
      break;

    case 1:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay1;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode1;
      break;

    case 2:
      logicVal = (UINT8)cmdCsr.Bits.cmdpilogicdelay2;
      piVal = (UINT8)cmdCsr.Bits.cmdpicode2;
      break;

    case 3:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay3;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode3;
      break;

    case 4:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay4;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode4;
      break;

    case 5:
      logicVal = (UINT8)cmdCsr2.Bits.cmdpilogicdelay5;
      piVal = (UINT8)cmdCsr2.Bits.cmdpicode5;
      break;

    case 6:
      logicVal = (UINT8)cmdCsr3.Bits.ctlpilogicdelay10;
      piVal = (UINT8)cmdCsr3.Bits.ctlpicode10;
      break;

    default:
      logicVal = 0;
      piVal = 0;
      // Assert error
  } // switch ioGroup

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    //
    // If in xover 2:2 mode, check to see if invertpiclk bit needs to be toggled
    //
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {

      xoverOffset = 0;
      oddFubOffset = 0;

      //
      //Gets register containing xover training offset
      //If 'odd' fub, add 32 to compensate for delay path differences
      //
      switch (regIndex) {
        case 0:
          xoverOffset = (UINT16)ddrCRCmdTraining.Bits.trainingoffset;
          if (((ch % 2 == 0) && (side == SIDE_A)) || ((ch % 2 == 1) && (side == SIDE_B))) {
            oddFubOffset = X_OVER_ODD_FUB_OFFSET;
          }
          break;
        case 1:
          xoverOffset = (UINT16)ddrCRCmdTraining.Bits.trainingoffset2;
          if (ch % 2 == 1) {
            oddFubOffset = X_OVER_ODD_FUB_OFFSET;
          }
          break;
      }

      //
      //Find the ‘zone’ for the values. If 0, value < 64; If 1, 64 <=value < 128; and so on.
      //
      curValZone  = ((curVal) % 64 + xoverOffset + oddFubOffset) / 64;
      piDelayZone = ((piDelay) % 64 + xoverOffset + oddFubOffset) / 64;

      invertPiClkBits = 0;

      //
      //Determine which invertpiclk bits based on group
      //
      switch (side){
        case SIDE_A:
          switch (ioGroup) {
            case 0:
              invertPiClkBits = BIT1 | BIT7;
              break;

            case 1:
              invertPiClkBits = BIT8 | BIT11;
              break;

            case 2:
              invertPiClkBits = BIT0 | BIT6;
              break;

            case 3:
              invertPiClkBits = BIT3 | BIT5;
              break;

            case 4:
              invertPiClkBits = BIT9 | BIT10;
              break;

            case 5:
              invertPiClkBits = BIT2 | BIT4;
              break;

            case 6:
              invertPiClkBits = BIT10;
              break;
        } // switch ioGroup
        break;

        case SIDE_B:
          switch (ioGroup) {
            case 0:
              invertPiClkBits = BIT3 | BIT8;
              break;

            case 1:
              invertPiClkBits = BIT2 | BIT4;
              break;

            case 2:
              invertPiClkBits = BIT0 | BIT1;
              break;

            case 3:
              invertPiClkBits = BIT10 | BIT11;
              break;

            case 4:
              invertPiClkBits = BIT6 | BIT7;
              break;

            case 5:
              invertPiClkBits = BIT5 | BIT9;
              break;
        } // switch ioGroup
        break;

      } // switch side

      //
      //If curValZone is not equal to piDelayZone, then the boundary is crossed at least once.
      //If abs(curValZone – piDelayZone) is even, the boundary and even number of times, and don’t flip invertpiclk
      //Else if abs(curValZone - piDelayZone) is odd, the boundary is crossed an odd number of times, and flip invertpiclk
      //
      if (curValZone != piDelayZone){
        if (ioGroup == 6) {
          ddrCrCtlRanksUsed.Bits.ddrcrinvertpiclk ^= invertPiClkBits;
        } else {
        ddrCRCmdControls2Cmd.Bits.ddrcrinvertpiclk ^= invertPiClkBits;
        }
      }
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (ioGroup) {
      case 0:
        cmdCsr.Bits.cmdpilogicdelay0 = logicVal;
        cmdCsr.Bits.cmdpicode0 = piVal;
        break;

      case 1:
        cmdCsr.Bits.cmdpilogicdelay1 = logicVal;
        cmdCsr.Bits.cmdpicode1 = piVal;
        break;

      case 2:
        cmdCsr.Bits.cmdpilogicdelay2 = logicVal;
        cmdCsr.Bits.cmdpicode2 = piVal;
        break;

      case 3:
        cmdCsr2.Bits.cmdpilogicdelay3 = logicVal;
        cmdCsr2.Bits.cmdpicode3 = piVal;
        break;

      case 4:
        cmdCsr2.Bits.cmdpilogicdelay4 = logicVal;
        cmdCsr2.Bits.cmdpicode4 = piVal;
        break;

      case 5:
        cmdCsr2.Bits.cmdpilogicdelay5 = logicVal;
        cmdCsr2.Bits.cmdpicode5 = piVal;
        break;

      case 6:
        cmdCsr3.Bits.ctlpilogicdelay10 = logicVal;
        cmdCsr3.Bits.ctlpicode10 = piVal;
        break;
    } // switch ioGroup

    switch (regIndex) {
      case 0:
        csrReg = cmdCsr.Data;
        break;
      case 1:
        csrReg = cmdCsr2.Data;
        break;
      case 2:
        csrReg = cmdCsr3.Data;
        break;
    }
    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].cmdCsr[side][regIndex] != csrReg) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (host, socket, ch, address, csrReg);
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2){
      switch (side)  {
          case SIDE_A:
            // Read xover training values from cache, read invertpiclk values from register
            MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls2Cmd.Data);
            break;
          case SIDE_B:
            // Read xover training values from cache, read invertpiclk values from register
            MemWritePciCfgEp (host, socket, ch, DDRCRCMDCONTROLS2_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls2Cmd.Data);
            break;
        }
      }
    }

    IO_Reset(host, socket);

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    switch (regIndex) {
      case 0:
        socketNvram->channelList[ch].cmdCsr[side][regIndex] = cmdCsr.Data;
        break;
      case 1:
        socketNvram->channelList[ch].cmdCsr[side][regIndex] = cmdCsr2.Data;
        break;
      case 2:
        socketNvram->channelList[ch].cmdCsr[side][regIndex] = cmdCsr3.Data;
        break;
    }

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      switch (side)  {
        case SIDE_A:
          // Read xover training values from cache, read invertpiclk values from register
          socketNvram->channelList[ch].ddrCRCmdControls2CmdN = ddrCRCmdControls2Cmd.Data;
          break;
        case SIDE_B:
          // Read xover training values from cache, read invertpiclk values from register
          socketNvram->channelList[ch].ddrCRCmdControls2CmdS = ddrCRCmdControls2Cmd.Data;
          break;
      }
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdDelay

MRC_STATUS
GetSetCmdVref (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     mode,
  INT16     *value
  )
/*++
  Muli-use function to either get or set command vref

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

--*/
{
  INT16                                         curVal;
  INT16                                         vref;
  UINT16                                        maxVal = 0;
  INT16                                         tmp;
  struct socketNvram                            *socketNvram;
  DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_BDX_STRUCT ddrCrDimmVrefControl1;


  socketNvram = &host->nvram.mem.socket[socket];

  //
  // Maximum CMD delay
  //

  if (host->var.common.cpuType == CPU_BDX) {
    maxVal = MAX_CMD_VREF;
  } else {
    maxVal = MAX_CMD_DELAY;
    ch = (ch & BIT1);
  }
  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {
    ddrCrDimmVrefControl1.Data = MemReadPciCfgEp (host, socket, ch & BIT1, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG);
  } else {
    ddrCrDimmVrefControl1.Data = socketNvram->channelList[ch].dimmVrefControl1;
  }

  //
  // BDX has one extra bit for this field. Mask it off for HSX.
  //
  if (host->var.common.cpuType == CPU_HSX) {
    ddrCrDimmVrefControl1.Data &= ~BIT7;
  }

  //
  // Get the CA Vref current setting
  //
  curVal = (INT16)(ddrCrDimmVrefControl1.Bits.cavrefctl >> 1);

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by value
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }

    // Check for boundaries
    // value is INT16 and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      vref = tmp;
    } else {
      vref = 0;
    }
    // Ensure we do not exceed maximums
    if (vref > maxVal) {
      vref = maxVal;
    }

    //
    // Program the new value
    //
    ddrCrDimmVrefControl1.Bits.cavrefctl = vref << 1;

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].dimmVrefControl1 != ddrCrDimmVrefControl1.Data) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (host, socket, ch & BIT1, DDRCRDIMMVREFCONTROL1_MCIO_DDRIOEXTMCC_REG, ddrCrDimmVrefControl1.Data);
    }
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    socketNvram->channelList[ch].dimmVrefControl1 = ddrCrDimmVrefControl1.Data;
  }

  return MRC_STATUS_SUCCESS;
} // GetSetCmdVref

MRC_STATUS
GetSetCtlDelay (
   PSYSHOST host,
   UINT8    socket,
   UINT8    ch,
   UINT8    ioGroup,
   UINT8    side,
   UINT8    mode,
   INT16    *value
   )
/*++
  Muli-use function to either get or set control delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param ioGroup:  Group number
  @param side:     Side number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program

  @retval Pi delay value

--*/
{
  UINT32                                      csrReg = 0;
  UINT8                                       regIndex;
  UINT32                                      address = 0;
  UINT8                                       logicVal;
  UINT8                                       piVal;
  UINT16                                      curVal;
  UINT16                                      maxVal;
  INT16                                       tmp;
  UINT16                                      piDelay;
  UINT16                                      curValZone;
  UINT16                                      piDelayZone;
  UINT16                                      xoverOffset;
  UINT16                                      oddFubOffset;
  UINT16                                      invertPiClkBits;
  UINT32                                      invertPiValue;
  struct socketNvram                          *socketNvram;
  DDRCRCTLPICODING_CKE_MCIO_DDRIOEXT_STRUCT   ctlCsr;
  DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXT_STRUCT  ctlCsr2;
  DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_STRUCT  ctlCsr3;
  DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRCtlControlsCkeRanksUsed;
  DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRCtlControlsRanksUsed;
  DDRCRCTLTRAINING_CTL_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCtlTraining;
  DDRCRCTLTRAINING_CKE_MCIO_DDRIOEXT_BDX_STRUCT   ddrCRCkeTraining;

  socketNvram = &host->nvram.mem.socket[socket];

  ctlCsr.Data = 0;
  ctlCsr2.Data = 0;
  ctlCsr3.Data = 0;
  invertPiValue = 0;
  ddrCRCtlControlsCkeRanksUsed.Data = 0;
  ddrCRCtlControlsRanksUsed.Data = 0;
  ddrCRCkeTraining.Data = 0;
  ddrCRCtlTraining.Data = 0;

  //
  // Maximum CTL delay
  //
  maxVal = MAX_CTL_DELAY;

  regIndex = ioGroup / 4;
  //
  // Point to the correct register
  //
  switch (regIndex) {
    case 0:
      address = DDRCRCTLPICODING_CKE_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
    case 1:
      address = DDRCRCTLPICODING2_CKE_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
    case 2:
      address = DDRCRCTLPICODING3_CKE_MCIO_DDRIOEXT_REG + CmdCtlAddressConvertTable[side];
      break;
  }

  if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
    ddrCRCkeTraining.Data = socketNvram->channelList[ch].ddrCRCkeTraining;
    ddrCRCtlTraining.Data = socketNvram->channelList[ch].ddrCRCtlTraining;
  }

  //
  // Get register data
  //
  if (mode & GSM_READ_CSR) {

    csrReg = MemReadPciCfgEp (host, socket, ch, address);
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      ddrCRCtlControlsCkeRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCtlControlsRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG);
    }

    switch (regIndex) {
      case 0:
        ctlCsr.Data = csrReg;
        break;
      case 1:
        ctlCsr2.Data = csrReg;
        break;
      case 2:
        ctlCsr3.Data = csrReg;
        break;
    }
  } else {

    //
    // Read from cache
    //
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      ddrCRCtlControlsCkeRanksUsed.Data = socketNvram->channelList[ch].ddrCRCtlControlsCkeRanksUsed;
      ddrCRCtlControlsRanksUsed.Data = socketNvram->channelList[ch].ddrCRCtlControlsRanksUsed;
    }

    switch (regIndex) {
      case 0:
        ctlCsr.Data = socketNvram->channelList[ch].ctlCsr[side][regIndex];
        break;
      case 1:
        ctlCsr2.Data = socketNvram->channelList[ch].ctlCsr[side][regIndex];
        break;
      case 2:
        ctlCsr3.Data = socketNvram->channelList[ch].ctlCsr[side][regIndex];
        break;
    }
  }

  switch (ioGroup) {
    case 0:
      logicVal = (UINT8)ctlCsr.Bits.ctlpilogicdelay0;
      piVal = (UINT8)ctlCsr.Bits.ctlpicode0;
      break;

    case 1:
      logicVal = (UINT8)ctlCsr.Bits.ctlpilogicdelay1;
      piVal = (UINT8)ctlCsr.Bits.ctlpicode1;
      break;

    case 2:
      logicVal = (UINT8)ctlCsr.Bits.ctlpilogicdelay2;
      piVal = (UINT8)ctlCsr.Bits.ctlpicode2;
      break;

    case 3:
      logicVal = (UINT8)ctlCsr.Bits.ctlpilogicdelay3;
      piVal = (UINT8)ctlCsr.Bits.ctlpicode3;
      break;

    case 4:
      logicVal = (UINT8)ctlCsr2.Bits.ctlpilogicdelay4;
      piVal = (UINT8)ctlCsr2.Bits.ctlpicode4;
      break;

    case 5:
      logicVal = (UINT8)ctlCsr2.Bits.ctlpilogicdelay5;
      piVal = (UINT8)ctlCsr2.Bits.ctlpicode5;
      break;

    case 6:
      logicVal = (UINT8)ctlCsr2.Bits.ctlpilogicdelay6;
      piVal = (UINT8)ctlCsr2.Bits.ctlpicode6;
      break;

    case 7:
      logicVal = (UINT8)ctlCsr2.Bits.ctlpilogicdelay7;
      piVal = (UINT8)ctlCsr2.Bits.ctlpicode7;
      break;

    case 8:
      logicVal = (UINT8)ctlCsr3.Bits.ctlpilogicdelay8;
      piVal = (UINT8)ctlCsr3.Bits.ctlpicode8;
      break;

    case 9:
      logicVal = (UINT8)ctlCsr3.Bits.ctlpilogicdelay9;
      piVal = (UINT8)ctlCsr3.Bits.ctlpicode9;
      break;

    case 10:
      logicVal = (UINT8)ctlCsr3.Bits.ctlpilogicdelay10;
      piVal = (UINT8)ctlCsr3.Bits.ctlpicode10;
      break;

    default:
      logicVal = 0;
      piVal = 0;
  }

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CMD delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      tmp = curVal + *value;
    } else {
      //
      // Absolute value
      //
      tmp = *value;
    }
    // Check for boundaries
    // offset is INT8 (char) and can be a negative value
    // Value and curVal are UINT8
    // Do not allow negative wraparound (curVal = 3 and offset = -4)
    if (tmp > 0) {
      piDelay = tmp;
    } else {
      piDelay = 0;
    }
    // Ensure we do not exceed maximums
    if (piDelay > maxVal) {
      piDelay = maxVal;
    }

    //
    // If in xover 2:2 mode, check to see if invertpiclk bit needs to be toggled
    //
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      // Gets register containing invertpiclk bit and register containing trainoffset
      // Gets register containing xover training offset
      xoverOffset = 0;
      oddFubOffset = 0;
      switch (side) {
        case SIDE_A:
          xoverOffset = (UINT16)ddrCRCkeTraining.Bits.trainingoffset;
          break;
        case SIDE_B:
          xoverOffset = (UINT16)ddrCRCtlTraining.Bits.trainingoffset;
          break;
      }

      //
      // If 'odd' fub, add 32 to compensate for delay path differences
      //
      if (((ch % 2 == 0) && (side == SIDE_A)) || ((ch % 2 == 1) && (side == SIDE_B))){
        oddFubOffset = X_OVER_ODD_FUB_OFFSET;
      }

      //
      // Find the ‘zone’ for the values. If 0, value < 64; If 1, 64 <=value < 128; and so on.
      //
      curValZone  = ((curVal) % 64 + xoverOffset + oddFubOffset) / 64;
      piDelayZone = ((piDelay) % 64 + xoverOffset + oddFubOffset) / 64;

      invertPiClkBits = 0;
      //
      //Determine which invertpiclk bits based on group
      //
      switch (side) {
        case SIDE_A:
          invertPiValue = ddrCRCtlControlsCkeRanksUsed.Bits.ddrcrinvertpiclk;
          switch (ioGroup) {
            case 0:
              invertPiClkBits = BIT1;
              break;

            case 1:
              invertPiClkBits = BIT3 | BIT7;
              break;

            case 2:
              invertPiClkBits = BIT6;
              break;

            case 3:
              invertPiClkBits = BIT0;
              break;

            case 4:
              invertPiClkBits = BIT2;
              break;

            case 5:
              invertPiClkBits = BIT4;
              break;

            case 6:
              invertPiClkBits = BIT5;
            break;

            case 7:
              invertPiClkBits = BIT8;
            break;

            case 8:
              invertPiClkBits = BIT11;
            break;

            case 9:
              invertPiClkBits = BIT9;
            break;

            case 10:
              invertPiClkBits = BIT10;
              break;
          } // switch ioGroup
          break;
        case SIDE_B:
          invertPiValue = ddrCRCtlControlsRanksUsed.Bits.ddrcrinvertpiclk;
          switch (ioGroup) {
            case 0:
              invertPiClkBits = BIT2;
              break;

            case 1:
              invertPiClkBits = BIT6;
              break;

            case 2:
              invertPiClkBits = BIT1;
              break;

            case 3:
              invertPiClkBits = BIT5;
              break;

            case 4:
              invertPiClkBits = BIT9;
              break;

            case 5:
              invertPiClkBits = BIT10;
              break;

            case 6:
              invertPiClkBits = BIT4;
            break;

            case 7:
              invertPiClkBits = BIT0;
            break;

            case 8:
              invertPiClkBits = BIT3 | BIT7;
            break;

            case 9:
              invertPiClkBits = BIT8;
            break;

            case 10:
              invertPiClkBits = BIT11;
              break;
          } // switch ioGroup
          break;
      } // switch side

      //
      //If curValZone is not equal to piDelayZone, then the boundary is crossed at least once.
      //If abs(curValZone – piDelayZone) is even, the boundary and even number of times, and don’t flip invertpiclk
      //Else if abs(curValZone - piDelayZone) is odd, the boundary is crossed an odd number of times, and flip invertpiclk
      //
      if (curValZone != piDelayZone) {
        switch (side) {
          case SIDE_A:
            ddrCRCtlControlsCkeRanksUsed.Bits.ddrcrinvertpiclk ^= invertPiClkBits;
            invertPiValue = ddrCRCtlControlsCkeRanksUsed.Bits.ddrcrinvertpiclk;
            break;
          case SIDE_B:
            ddrCRCtlControlsRanksUsed.Bits.ddrcrinvertpiclk ^= invertPiClkBits;
            invertPiValue = ddrCRCtlControlsRanksUsed.Bits.ddrcrinvertpiclk;
            break;
        }
      }
    } //xover 2:2 only

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (ioGroup) {
      case 0:
        ctlCsr.Bits.ctlpilogicdelay0 = logicVal;
        ctlCsr.Bits.ctlpicode0 = piVal;
        break;

      case 1:
        ctlCsr.Bits.ctlpilogicdelay1 = logicVal;
        ctlCsr.Bits.ctlpicode1 = piVal;
        break;

      case 2:
        ctlCsr.Bits.ctlpilogicdelay2 = logicVal;
        ctlCsr.Bits.ctlpicode2 = piVal;
        break;

      case 3:
        ctlCsr.Bits.ctlpilogicdelay3 = logicVal;
        ctlCsr.Bits.ctlpicode3 = piVal;
        break;

      case 4:
        ctlCsr2.Bits.ctlpilogicdelay4 = logicVal;
        ctlCsr2.Bits.ctlpicode4 = piVal;
        break;

      case 5:
        ctlCsr2.Bits.ctlpilogicdelay5 = logicVal;
        ctlCsr2.Bits.ctlpicode5 = piVal;
        break;

      case 6:
        ctlCsr2.Bits.ctlpilogicdelay6 = logicVal;
        ctlCsr2.Bits.ctlpicode6 = piVal;
        break;

      case 7:
        ctlCsr2.Bits.ctlpilogicdelay7 = logicVal;
        ctlCsr2.Bits.ctlpicode7 = piVal;
        break;

      case 8:
        ctlCsr3.Bits.ctlpilogicdelay8 = logicVal;
        ctlCsr3.Bits.ctlpicode8 = piVal;
        break;

      case 9:
        ctlCsr3.Bits.ctlpilogicdelay9 = logicVal;
        ctlCsr3.Bits.ctlpicode9 = piVal;
        break;

      case 10:
        ctlCsr3.Bits.ctlpilogicdelay10 = logicVal;
        ctlCsr3.Bits.ctlpicode10 = piVal;
        break;
    }

    switch (regIndex) {
      case 0:
        csrReg = ctlCsr.Data;
        break;
      case 1:
        csrReg = ctlCsr2.Data;
        break;
      case 2:
        csrReg = ctlCsr3.Data;
        break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].ctlCsr[side][regIndex] != csrReg) || (mode & GSM_FORCE_WRITE)) {
       MemWritePciCfgEp (host, socket, ch, address, csrReg);
      //
      //Write invertpiclk value back to register
      //
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        switch (side) {
          case SIDE_A:
            MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CKE_MCIO_DDRIOEXT_REG, ddrCRCtlControlsCkeRanksUsed.Data);
            break;
          case SIDE_B:
            MemWritePciCfgEp (host, socket, ch, DDRCRCTLRANKSUSED_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlControlsRanksUsed.Data);
            break;
        }
      }
    }

    IO_Reset(host, socket);
  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
    switch (regIndex) {
      case 0:
        socketNvram->channelList[ch].ctlCsr[side][regIndex] = ctlCsr.Data;
        break;
      case 1:
        socketNvram->channelList[ch].ctlCsr[side][regIndex] = ctlCsr2.Data;
        break;
      case 2:
        socketNvram->channelList[ch].ctlCsr[side][regIndex] = ctlCsr3.Data;
        break;
    }

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      switch (side) {
        case SIDE_A:
          socketNvram->channelList[ch].ddrCRCtlControlsCkeRanksUsed = ddrCRCtlControlsCkeRanksUsed.Data;
          break;
        case SIDE_B:
          socketNvram->channelList[ch].ddrCRCtlControlsRanksUsed = ddrCRCtlControlsRanksUsed.Data;
          break;
      }
    }
  }
  return MRC_STATUS_SUCCESS;
} // GetSetCtlDelay

MRC_STATUS
GetSetClkDelay (
   PSYSHOST host,
   UINT8    socket,
   UINT8    ch,
   UINT8    clk,
   UINT8    mode,
   INT16    *value
   )
/*++
  Muli-use function to either get or set control delays based on the provided group number

  @param host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param clk:      Clock number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program

  @retval Pi delay value

--*/
{
  UINT8                               logicVal;
  UINT8                               piVal;
  UINT16                              curVal;
  INT16                               tmp;
  UINT16                              piDelay;
  UINT16                              curValZone;
  UINT16                              piDelayZone;
  UINT16                              xoverOffset;
  UINT16                              oddFubOffset;
  struct socketNvram                  *socketNvram;
  DDRCRCLKPICODE_MCIO_DDRIOEXT_STRUCT clkCsr;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT  ddrCRClkTraining;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_BDX_STRUCT ddrCRClkRanksUsed;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_BDX_STRUCT  ddrCRClkControls;

  socketNvram = &host->nvram.mem.socket[socket];

  //
  // Get register data
  //
  ddrCRClkTraining.Data = 0;
  ddrCRClkRanksUsed.Data = 0;


  if ((mode & GSM_READ_ONLY) == 0) {
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 1;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);
      //Register containing trainoffset
      ddrCRClkTraining.Data = socketNvram->channelList[ch].ddrCRClkTraining;
    }
  }

  //Register containing invertpiclk bit
  if (mode & GSM_READ_CSR) {
    clkCsr.Data = (UINT16)MemReadPciCfgEp (host, socket, ch, DDRCRCLKPICODE_MCIO_DDRIOEXT_REG);
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      ddrCRClkRanksUsed.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
    }
  } else {
    clkCsr.Data = socketNvram->channelList[ch].clkCsr;
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      ddrCRClkRanksUsed.Data = socketNvram->channelList[ch].ddrCRClkRanksUsed;
    }
  }

  switch (clk) {
    case 0:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank0;
      piVal = (UINT8)clkCsr.Bits.pisettingrank0;
      break;

    case 1:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank1;
      piVal = (UINT8)clkCsr.Bits.pisettingrank1;
      break;

    case 2:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank2;
      piVal = (UINT8)clkCsr.Bits.pisettingrank2;
      break;

    case 3:
      logicVal = (UINT8)clkCsr.Bits.pilogicdelayrank3;
      piVal = (UINT8)clkCsr.Bits.pisettingrank3;
      break;

    default:
      logicVal = 0;
      piVal = 0;
      // Assert error
  }

  // Combine into delay
  curVal = (logicVal * 64) + piVal;

  // Read only?
  if (mode & GSM_READ_ONLY) {
    *value = (INT16)curVal;
  }
  // Handle writes
  else {

    // Adjust the current CLK delay value by offset
    if (mode & GSM_WRITE_OFFSET) {
      // Get the new offset (note: can be negative)
      tmp = curVal + *value;
    } else {
      // FORCE_WRITE
      tmp = *value;
    }
    // Check if we are below 0
    if (tmp >= 0) {
      piDelay = (UINT8) tmp;
    } else {
      piDelay = 128 + tmp;
    }
    if (piDelay > MAX_CK_DELAY) {
      piDelay = tmp - MAX_CK_DELAY;
    }

    //
    // If in xover 2:2 mode, check to see if invertpiclk bit needs to be toggled
    //
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      xoverOffset = 0;
      oddFubOffset = 0;
      xoverOffset = (UINT16)ddrCRClkTraining.Bits.trainingoffset;

      //
      //If 'odd' fub, add 32 to compensate for delay path differences
      //
      if (ch % 2 == 1) {
        oddFubOffset = X_OVER_ODD_FUB_OFFSET;
      }

      //
      //Find the ‘zone’ for the values. If 0, value < 64; If 1, 64 <=value < 128; and so on.
      //
      curValZone  = ((curVal) % 64 + xoverOffset + oddFubOffset) / 64;
      piDelayZone = ((piDelay) % 64 + xoverOffset + oddFubOffset) / 64;

      //If curValZone is not equal to piDelayZone, then the boundary is crossed at least once.
      //If abs(curValZone – piDelayZone) is even, the boundary and even number of times, and don’t flip invertpiclk
      //Else if abs(curValZone - piDelayZone) is odd, the boundary is crossed an odd number of times, and flip invertpiclk
      if (curValZone != piDelayZone) {
        switch (clk) {
          case 0:
            ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk ^= BIT0;
            break;

          case 1:
            ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk ^= BIT1;
            break;

          case 2:
            ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk ^= BIT2;
            break;

          case 3:
            ddrCRClkRanksUsed.Bits.ddrcrinvertpiclk ^= BIT3;
            break;
        }
      }
    }

    // Program the IO delay
    logicVal = (UINT8) (piDelay / 64);
    piVal    = (UINT8) (piDelay % 64);

    switch (clk) {
      case 0:
        clkCsr.Bits.pilogicdelayrank0 = logicVal;
        clkCsr.Bits.pisettingrank0 = piVal;
        break;

      case 1:
        clkCsr.Bits.pilogicdelayrank1 = logicVal;
        clkCsr.Bits.pisettingrank1 = piVal;
        break;

      case 2:
        clkCsr.Bits.pilogicdelayrank2 = logicVal;
        clkCsr.Bits.pisettingrank2 = piVal;
        break;

      case 3:
        clkCsr.Bits.pilogicdelayrank3 = logicVal;
        clkCsr.Bits.pisettingrank3 = piVal;
        break;
    }

    //
    // Write it back if the current data does not equal the cache value or if in force write mode
    //
    if ((socketNvram->channelList[ch].clkCsr != clkCsr.Data) || (mode & GSM_FORCE_WRITE)) {
      MemWritePciCfgEp (host, socket, ch, DDRCRCLKPICODE_MCIO_DDRIOEXT_REG, clkCsr.Data);
      if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
      }
    }

    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
        IO_Reset(host, socket);
        ddrCRClkControls.Data = MemReadPciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
        ddrCRClkControls.Bits.xovercal = 0;
        MemWritePciCfgEp (host, socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data);
        FixedDelay(host, 2000);
    }

  }

  //
  // Save to cache
  //
  if (mode & GSM_UPDATE_CACHE) {
     socketNvram->channelList[ch].clkCsr = clkCsr.Data;
    if (host->var.mem.xoverModeVar == XOVER_MODE_2TO2) {
      socketNvram->channelList[ch].ddrCRClkRanksUsed = ddrCRClkRanksUsed.Data;
    }
  }

  return MRC_STATUS_SUCCESS;
} // GetSetClkDelay

//-----------------------------------------------------------------------------------
void
SetCtlRankCnfg (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT16   Value
  )
/*++

  Disables unused CMD/CTL signals

  @param host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)
  @param Value - Value to program

  @retval N/A

--*/
{
} // SetCtlRankCnfg


//-----------------------------------------------------------------------------------
void
SetRankUsed (
  PSYSHOST host,
  UINT8    socket,
  UINT8    ch,
  UINT8    Value
  )
/*++

  Disables unused clocks

  @param host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch    - Channel number (0-based)
  @param Value - Value to program

  @retval N/A

--*/
{
} // SetRankUsed

// Table of # pi ticks per pico second for each supported frequency
// Formula is: ((1/(Frq/2))*1.0E6)/64 - ((1/(1066/2))*1000000)/64 = 29.29
//                                         800 1066 1333 1600 1833 2133 2400
//                                         400 533   667  800  933 1066 1200
const UINT8 piPsFrqTable[MAX_SUP_FREQ] = { 20, 16, 15, 14, 12, 12, 10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 6, 6, 5};

void
SetStartingCCC (
  PSYSHOST  host,
  UINT8     socket
  )
/*++

  Set starting/initial values for Clock and Control signals
  Initial values come from Analog Design

  @param host    - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

--*/
{
  UINT8               ai;
  UINT8               mode;
  UINT8               piPerPico;
  INT16               delay;
  UINT8               cpuSku;
  UINT8               entries;
  INT16               initOffset;
  //UINT8               coreCount = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  CAPID4_PCU_FUN3_BDX_STRUCT capid4;

  channelNvList = &host->nvram.mem.socket[socket].channelList;
  piPerPico = piPsFrqTable[host->nvram.mem.ratioIndex];

#ifdef SERIAL_DBG_MSG
  getPrintFControl(host);
#endif // SERIAL_DBG_MSG

  //
  // Initial offset for CMD and CTL
  //
  initOffset = 128;
  //initOffset = 0;

  mode = GSM_FORCE_WRITE + GSM_UPDATE_CACHE; // Use the cached value and don't read

  capid4.Data = host->var.common.procCom[socket].capid4;

  //
  // BIT8 has a different meaning for HSX so mask it off
  //
  if (host->var.common.cpuType == CPU_HSX) {
    capid4.Data &= ~BIT8;
  }

  switch(capid4.Bits.physical_chop) {
    // LCC
    case 0:
      cpuSku = 2;
      break;
    // MCC
    case 1:
      cpuSku = 1;
      break;
    // HCC
    case 2:
      cpuSku = 0;
      break;
    default:
      cpuSku = 0;
      break;
  }

  if (CheckSteppingLessThan(host, CPU_HSX, B0_REV_HSX)) {
    entries = (UINT8) (sizeof (igCtlA0) / sizeof (struct IoGroupDelayStruct));

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, CtlEntries=%3d\n",
                  cpuSku, entries));

    for (ai = 0; ai < entries; ai++) {
      if ((*channelNvList)[igCtlA0[ai].ch].enabled) {
        if (igCtlA0[ai].delay[cpuSku] != 0xFF) {
          delay = (igCtlA0[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
          MemDebugPrint((host, SDBG_MAX, socket, igCtl[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "CTL Group %d, CTL side %d, piDelay %d\n",igCtlA0[ai].iog, igCtlA0[ai].side, delay));
          GetSetCtlDelay (host, socket, igCtlA0[ai].ch, igCtlA0[ai].iog, igCtlA0[ai].side, mode, &delay);
        }
      }
    } // ai loop

    entries = (UINT8) (sizeof (igCmdA0) / sizeof (struct IoGroupDelayStruct));

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCC => CpuSku=%2d, CmdEntries=%3d\n",
                   cpuSku, entries));

    for (ai = 0; ai < entries; ai++) {
      if ((*channelNvList)[igCmdA0[ai].ch].enabled) {
        if (igCmdA0[ai].delay[cpuSku] != 0xFF) {
          delay = (igCmdA0[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
          MemDebugPrint((host, SDBG_MAX, socket, igCmdA0[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "CMD A0 Group %d, CMD side %d, piDelay %d\n",igCmdA0[ai].iog, igCmdA0[ai].side, delay));
          GetSetCmdDelay (host, socket, igCmdA0[ai].ch, igCmdA0[ai].iog, igCmdA0[ai].side, mode, &delay);
        }
      }
    } // ai loop

    entries = (UINT8) (sizeof (igClkA0) / sizeof (struct IoGroupClkDelayStruct));

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, ClkEntries=%3d\n",
                  cpuSku, entries));

    for (ai = 0; ai < entries; ai++) {
      if ((*channelNvList)[igClkA0[ai].ch].enabled) {
        if (igClkA0[ai].delay[cpuSku] != 0xFF) {
          delay = igClkA0[ai].delay[cpuSku] / piPerPico;  // Ensure proper rounding
          MemDebugPrint((host, SDBG_MAX, socket, igClkA0[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                        "CLK %d, piDelay %d\n",igClkA0[ai].clk, delay));
          GetSetClkDelay (host, socket, igClkA0[ai].ch, igClkA0[ai].clk, mode, &delay);
        }
      }
    } // ai loop
  } else {
    if (IsBDXEP(host)) {
      entries = (UINT8) (sizeof (igCtl_BDX) / sizeof (struct IoGroupDelayStruct));
    } else {
      entries = (UINT8) (sizeof (igCtl) / sizeof (struct IoGroupDelayStruct));
    }
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, CtlEntries=%3d\n",
                  cpuSku, entries));

    if (IsBDXEP(host)) {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igCtl_BDX[ai].ch].enabled) {
          if (igCtl_BDX[ai].delay[cpuSku] != 0xFF) {
            delay = (igCtl_BDX[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
            delay = delay - 30;    // Artificial delay to account for skewed CCC tables
            MemDebugPrint((host, SDBG_MAX, socket, igCtl_BDX[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "CTL Group %d, CTL side %d, piDelay %d\n",igCtl_BDX[ai].iog, igCtl_BDX[ai].side, delay));
            GetSetCtlDelay (host, socket, igCtl_BDX[ai].ch, igCtl_BDX[ai].iog, igCtl_BDX[ai].side, mode, &delay);
          }
        }
      } // ai loop
    } else {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igCtl[ai].ch].enabled) {
          if (igCtl[ai].delay[cpuSku] != 0xFF) {
            delay = (igCtl[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
            if (IsDESku(host)) {
              delay = initOffset;
              delay = delay - 20;    // Artificial delay to account for skewed CCC tables
            }
            MemDebugPrint((host, SDBG_MAX, socket, igCtl[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "CTL Group %d, CTL side %d, piDelay %d\n",igCtl[ai].iog, igCtl[ai].side, delay));
            GetSetCtlDelay (host, socket, igCtl[ai].ch, igCtl[ai].iog, igCtl[ai].side, mode, &delay);
          }
        }
      } // ai loop
    }

    if (IsBDXEP(host)) {
      entries = (UINT8) (sizeof (igCmd_BDX) / sizeof (struct IoGroupDelayStruct));
    } else {
      entries = (UINT8) (sizeof (igCmd) / sizeof (struct IoGroupDelayStruct));
    }

    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCC => CpuSku=%2d, CmdEntries=%3d\n",
                   cpuSku, entries));

    if (IsBDXEP(host)) {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igCmd_BDX[ai].ch].enabled) {
          if (igCmd_BDX[ai].delay[cpuSku] != 0xFF) {
            delay = (igCmd_BDX[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
            delay = delay - 35;    // Artificial delay to account for skewed CCC tables
            MemDebugPrint((host, SDBG_MAX, socket, igCmd_BDX[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "CMD Group %d, CMD side %d, piDelay %d\n",igCmd_BDX[ai].iog, igCmd_BDX[ai].side, delay));
            GetSetCmdDelay (host, socket, igCmd_BDX[ai].ch, igCmd_BDX[ai].iog, igCmd_BDX[ai].side, mode, &delay);
          }
        }
      } // ai loop
    } else {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igCmd[ai].ch].enabled) {
          if (igCmd[ai].delay[cpuSku] != 0xFF) {
            delay = (igCmd[ai].delay[cpuSku] / piPerPico) + initOffset;  // Ensure proper rounding
            if (IsDESku(host)) {
              delay = initOffset;
              delay = delay - 25;    // Artificial delay to account for skewed CCC tables
            }
            MemDebugPrint((host, SDBG_MAX, socket, igCmd[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "CMD Group %d, CMD side %d, piDelay %d\n",igCmd[ai].iog, igCmd[ai].side, delay));
            GetSetCmdDelay (host, socket, igCmd[ai].ch, igCmd[ai].iog, igCmd[ai].side, mode, &delay);
          }
        }
      } // ai loop
    }

    if (IsBDXEP(host)) {
      entries = (UINT8) (sizeof (igClk_BDX) / sizeof (struct IoGroupClkDelayStruct));
    } else {
      entries = (UINT8) (sizeof (igClk) / sizeof (struct IoGroupClkDelayStruct));
    }
    MemDebugPrint((host, SDBG_MAX, socket, NO_CH, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCC => CpuSku=%2d, ClkEntries=%3d\n",
                  cpuSku, entries));

    if (IsBDXEP(host)) {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igClk_BDX[ai].ch].enabled) {
          if (igClk_BDX[ai].delay[cpuSku] != 0xFF) {
            delay = igClk_BDX[ai].delay[cpuSku] / piPerPico;  // Ensure proper rounding
            MemDebugPrint((host, SDBG_MAX, socket, igClk_BDX[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                           "CLK %d, piDelay %d\n",igClk_BDX[ai].clk, delay));
            GetSetClkDelay (host, socket, igClk_BDX[ai].ch, igClk_BDX[ai].clk, mode, &delay);
          }
        }
      } // ai loop
    } else {
      for (ai = 0; ai < entries; ai++) {
        if ((*channelNvList)[igClk[ai].ch].enabled) {
          if (igClk[ai].delay[cpuSku] != 0xFF) {
            delay = igClk[ai].delay[cpuSku] / piPerPico;  // Ensure proper rounding
            if (IsDESku(host)) {
              delay = initOffset;
            }
            MemDebugPrint((host, SDBG_MAX, socket, igClk[ai].ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                          "CLK %d, piDelay %d\n",igClk[ai].clk, delay));
            GetSetClkDelay (host, socket, igClk[ai].ch, igClk[ai].clk, mode, &delay);
          }
        }
      } // ai loop
    }
  }

#ifdef SERIAL_DBG_MSG
  releasePrintFControl(host);
#endif // SERIAL_DBG_MSG
} // SetStartingCCC

MRC_STATUS
GetCmdGroupLimits (
  PSYSHOST  host,
  UINT8     socket,
  GSM_LT    level,
  GSM_GT    group,
  UINT16    *minLimit,
  UINT16    *maxLimit
  )
/*++

  Set starting/initial values for Clock and Control signals
  Initial values come from Analog Design

  @param host      - Pointer to host structure (outside scope of this spec)
  @param socket    - Processor socket within the system (0-based)
  @param level     - IO level to access
  @param group     - Command, clock or control group to access
  @param *minLimit - Minimum delay value allowed
  @param *maxLimit - Maximum delay value allowed

  @retval MRC_STATUS

--*/
{
  *minLimit = 0;

  if (group == CkAll) {
    *maxLimit = 127;
  } else {
    *maxLimit = 255;
  }

  return MRC_STATUS_SUCCESS;
} // GetCmdGroupLimits

#ifdef SERIAL_DBG_MSG
char *GetSignalStr(GSM_CSN sig, char *strBuf) {

  switch (sig) {
    case PAR:
      StrCpyLocal(strBuf, "PAR");
      break;
    case ACT_N:
      StrCpyLocal(strBuf, "ACT_N");
      break;
    case RAS_N:
      StrCpyLocal(strBuf, "RAS_N");
      break;
    case CAS_N:
      StrCpyLocal(strBuf, "CAS_N");
      break;
    case WE_N:
      StrCpyLocal(strBuf, "WE_N");
      break;
    case BA0:
      StrCpyLocal(strBuf, "BA0");
      break;
    case BA1:
      StrCpyLocal(strBuf, "BA1");
      break;
    case BA2:
      StrCpyLocal(strBuf, "BA2");
      break;
    case BG0:
      StrCpyLocal(strBuf, "BG0");
      break;
    case BG1:
      StrCpyLocal(strBuf, "BG1");
      break;
    case A0:
      StrCpyLocal(strBuf, "A0");
      break;
    case A1:
      StrCpyLocal(strBuf, "A1");
      break;
    case A2:
      StrCpyLocal(strBuf, "A2");
      break;
    case A3:
      StrCpyLocal(strBuf, "A3");
      break;
    case A4:
      StrCpyLocal(strBuf, "A4");
      break;
    case A5:
      StrCpyLocal(strBuf, "A5");
      break;
    case A6:
      StrCpyLocal(strBuf, "A6");
      break;
    case A7:
      StrCpyLocal(strBuf, "A7");
      break;
    case A8:
      StrCpyLocal(strBuf, "A8");
      break;
    case A9:
      StrCpyLocal(strBuf, "A9");
      break;
    case A10:
      StrCpyLocal(strBuf, "A10");
      break;
    case A11:
      StrCpyLocal(strBuf, "A11");
      break;
    case A12:
      StrCpyLocal(strBuf, "A12");
      break;
    case A13:
      StrCpyLocal(strBuf, "A13");
      break;
    case A14:
      StrCpyLocal(strBuf, "A14");
      break;
    case A15:
      StrCpyLocal(strBuf, "A15");
      break;
    case A16:
      StrCpyLocal(strBuf, "A16");
      break;
    case A17:
      StrCpyLocal(strBuf, "A17");
      break;
    case CS0_N:
      StrCpyLocal(strBuf, "CS0_N");
      break;
    case CS1_N:
      StrCpyLocal(strBuf, "CS1_N");
      break;
    case CS2_N:
      StrCpyLocal(strBuf, "CS2_N");
      break;
    case CS3_N:
      StrCpyLocal(strBuf, "CS3_N");
      break;
    case CS4_N:
      StrCpyLocal(strBuf, "CS4_N");
      break;
    case CS5_N:
      StrCpyLocal(strBuf, "CS5_N");
      break;
    case CS6_N:
      StrCpyLocal(strBuf, "CS6_N");
      break;
    case CS7_N:
      StrCpyLocal(strBuf, "CS7_N");
      break;
    case CS8_N:
      StrCpyLocal(strBuf, "CS8_N");
      break;
    case CS9_N:
      StrCpyLocal(strBuf, "CS9_N");
      break;
    case CKE0:
      StrCpyLocal(strBuf, "CKE0");
      break;
    case CKE1:
      StrCpyLocal(strBuf, "CKE1");
      break;
    case CKE2:
      StrCpyLocal(strBuf, "CKE2");
      break;
    case CKE3:
      StrCpyLocal(strBuf, "CKE3");
      break;
    case CKE4:
      StrCpyLocal(strBuf, "CKE4");
      break;
    case CKE5:
      StrCpyLocal(strBuf, "CKE5");
      break;
    case ODT0:
      StrCpyLocal(strBuf, "ODT0");
      break;
    case ODT1:
      StrCpyLocal(strBuf, "ODT1");
      break;
    case ODT2:
      StrCpyLocal(strBuf, "ODT2");
      break;
    case ODT3:
      StrCpyLocal(strBuf, "ODT3");
      break;
    case ODT4:
      StrCpyLocal(strBuf, "ODT4");
      break;
    case ODT5:
      StrCpyLocal(strBuf, "ODT5");
      break;
    case CK0:
      StrCpyLocal(strBuf, "CK0");
      break;
    case CK1:
      StrCpyLocal(strBuf, "CK1");
      break;
    case CK2:
      StrCpyLocal(strBuf, "CK2");
      break;
    case CK3:
      StrCpyLocal(strBuf, "CK3");
      break;
    case CK4:
      StrCpyLocal(strBuf, "CK4");
      break;
    case CK5:
      StrCpyLocal(strBuf, "CK5");
      break;
    case C0:
      StrCpyLocal(strBuf, "C0");
      break;
    case C1:
      StrCpyLocal(strBuf, "C1");
      break;
    case C2:
      StrCpyLocal(strBuf, "C2");
      break;
    default:
      StrCpyLocal(strBuf, "Unknown signal");
      break;
  }
  return strBuf;
} // GetSignalStr

char *GetPlatformGroupStr(GSM_GT group, char *strBuf) {

  switch (group) {
    case CmdAll:
      StrCpyLocal(strBuf, "CmdAll");
      break;
    case CmdGrp0:
      StrCpyLocal(strBuf, "CmdGrp0");
      break;
    case CmdGrp1:
      StrCpyLocal(strBuf, "CmdGrp1");
      break;
    case CmdGrp2:
      StrCpyLocal(strBuf, "CmdGrp2");
      break;
    case CtlAll:
      StrCpyLocal(strBuf, "CtlAll");
      break;
    case CtlGrp0:
      StrCpyLocal(strBuf, "CtlGrp0");
      break;
    case CtlGrp1:
      StrCpyLocal(strBuf, "CtlGrp1");
      break;
    case CtlGrp2:
      StrCpyLocal(strBuf, "CtlGrp2");
      break;
    case CtlGrp3:
      StrCpyLocal(strBuf, "CtlGrp3");
      break;
    case CtlGrp4:
      StrCpyLocal(strBuf, "CtlGrp4");
      break;
    case CtlGrp5:
      StrCpyLocal(strBuf, "CtlGrp5");
      break;
    case CkAll:
      StrCpyLocal(strBuf, "CkAll");
      break;
    case CmdCtlAll:
      StrCpyLocal(strBuf, "CmdCtlAll");
      break;
    default:
      StrCpyLocal(strBuf, "Unknown platform group");
      break;
  }
  return strBuf;
} // GetPlatformGroupStr
#endif  // SERIAL_DBG_MSG

/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param host                 - Pointer to host structure
  @param socket               - Processor socket within the system (0-based)
  @param ch                   - DDR channel number within the processor socket (0-based)
  @param listSize             - Number of entries in each list
  @param *value[MAX_RANK_CH]  - Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCtlGroup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT16    listSize,
  VOID      *value
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 listIndex;
  UINT8                 ctlPiGroup;
  INT16                 ctlRight[MAX_CMDCTL_GROUPALL];
  INT16                 ctlLeft[MAX_CMDCTL_GROUPALL];
  GSM_CSN               ctlSignal[MAX_CMDCTL_GROUPALL];
  INT16                 ctlValue[MAX_CMDCTL_GROUPALL];
#ifdef SERIAL_DBG_MSG
  char                  strBuf0[128];
#endif  // SERIAL_DBG_MSG
  MRC_STATUS            status;
  GSM_CSEDGE_CTL        (*signalListEdge)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP_LB];
  struct signalIOGroup  signalGroup;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];

  status = MRC_STATUS_SUCCESS;


  //
  // Initialize the edges for each CTL pi group
  //
  for (ctlPiGroup = 0; ctlPiGroup < MAX_CMDCTL_GROUPALL; ctlPiGroup++) {
    ctlLeft[ctlPiGroup] = 0;
    ctlRight[ctlPiGroup] = 255;
    ctlValue[ctlPiGroup] = 0;
  } // ctlPiGroup loop

  signalListEdge = value;

  dimmNvList = GetDimmNvList(host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {

    rankList = GetRankNvList(host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

        //
        // Loop through the list to create the composite left and right edges for each CTL group
        //
        for (listIndex = 0; listIndex < listSize; listIndex++) {

          //
          // Get the silicon pi group this signal belongs to
          //
          if ((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].toSample == 0) continue;
          status = SignalToPiGroup (host, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

          //
          // Get the index into the CTL silicon pi group
          //
          for (ctlPiGroup = 0; ctlPiGroup < MAX_CMDCTL_GROUPALL; ctlPiGroup++) {
            if ((signalGroup.group.num == platformCmdCtlGroupAll[ctlPiGroup].num) &&
                (signalGroup.group.side == platformCmdCtlGroupAll[ctlPiGroup].side)) {

              //
              // We found the CTL pi group so break out of the loop
              //
              break;
            }
          } // ctlPiGroup loop

          // Check if we didn't find the pi group
          RC_ASSERT(ctlPiGroup != MAX_CMDCTL_GROUPALL, ERR_RC_INTERNAL, 0);

          //
          // Update the composite left and right edges for the current CTL pi group relative to the clock
          //
          UpdateEdges((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                      &ctlLeft[ctlPiGroup], &ctlRight[ctlPiGroup]);
          ctlValue[ctlPiGroup] = (ctlLeft[ctlPiGroup] + ctlRight[ctlPiGroup])/2;
          ctlSignal[ctlPiGroup] = (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal;

          MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                       "%s:\t CTL Pi Group %d: le = %d re = %d, ctlLeft = %d ctlRight = %d ctlValue = %d\n",
                        GetSignalStr(signalGroup.sig, (char *)&strBuf0), ctlPiGroup,
                        (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                        ctlLeft[ctlPiGroup], ctlRight[ctlPiGroup], ctlValue[ctlPiGroup]));
      } // listIndex loop
    } // rank loop
  } // dimm loop

  for (ctlPiGroup = 0; ctlPiGroup < MAX_CMDCTL_GROUPALL; ctlPiGroup++) {
    if (ctlValue[ctlPiGroup] != 0) {
      status = GetSetSignal (host, socket, ch, DdrLevel, ctlSignal[ctlPiGroup], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &ctlValue[ctlPiGroup]);
    }
  }
  return status;
} // SetCombinedCtlGroup

MRC_STATUS
SetCombinedCmdGroup (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  GSM_LT    level,
  UINT8     mode,
  UINT8     listType,
  UINT16    listSize,
  VOID      *value
  )
/*++

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param host                 - Pointer to host structure
  @param socket               - Processor socket within the system (0-based)
  @param ch                   - DDR channel number within the processor socket (0-based)
  @param level                - IO level to access
  @param mode                 - Bit-field of different modes
  @param listType             - Selects type of each list
  @param listSize             - Number of entries in each list
  @param *value[MAX_RANK_CH]  - Array of pointers to listType

  @retval MRC_STATUS

--*/
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 clk;
  UINT8                 listIndex;
  UINT8                 cmdPiGroup;
  UINT8                 ckEnabled[MAX_CLK];
  UINT8                 ctlIndex[MAX_CLK];
  INT16                 cmdLeft[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS][MAX_CLK];
  INT16                 cmdRight[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS][MAX_CLK];
  INT16                 cmdOffset[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS];
  UINT16                cmdPiDelaySum[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS];
  UINT8                 cmdPiDelayCount[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS];
  INT16                 cmdPiDelay[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS];
  UINT8                 maxIoGroup;
  UINT8                 iog;
  struct ioGroup        *iogPtr;
#ifdef SERIAL_DBG_MSG
  char                  strBuf0[128];
#endif  // SERIAL_DBG_MSG
  MRC_STATUS            status;
  GSM_CSVAL             (*signalList)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CSEDGE            (*signalListEdge)[MAX_RANK_CH][NUM_SIGNALS_TO_SWEEP];
  GSM_CGVAL             (*groupList)[MAX_RANK_CH][8];
  GSM_CGEDGE            (*groupListEdge)[MAX_RANK_CH][MAX_CMD];
  struct signalIOGroup  signalGroup;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];

  status = MRC_STATUS_SUCCESS;

  dimmNvList = GetDimmNvList(host, socket, ch);

  for (clk = 0; clk < MAX_CLK; clk++) {
    ckEnabled[clk] = 0;
    ctlIndex[clk] = 0;
  } // clk loop

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    rankList = GetRankNvList(host, socket, ch, dimm);
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {

      // If rank enabled
      if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

      clk = (*rankList)[rank].ckIndex;
      ctlIndex[clk] = (*rankList)[rank].ctlIndex;
      ckEnabled[clk] = 1;
    } // rank loop
  } // dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS; cmdPiGroup++) {
    // Init CK composite edges to 127
    for (clk = 0; clk < MAX_CLK; clk++) {
      cmdLeft[cmdPiGroup][clk] = -255;
      cmdRight[cmdPiGroup][clk] = 255;
      //cmdOffsetClk[cmdPiGroup][clk] = 0;
    } // clk loop
    cmdOffset[cmdPiGroup] = 0;
  } // cmdPiGroup loop

  if (listType == CMD_GRP_DELAY) {
    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {
      groupListEdge = value;


      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          clk = (*rankList)[rank].ckIndex;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            //if ((*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group == CmdAll) {
            //  for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD; cmdPiGroup++) {
            //    // Init CK composite edges to 127
            //    cmdLeft[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].le;
            //    cmdRight[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].re;
            //  } // cmdPiGroup loop
            //} else {
              MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, rank, NO_STROBE, NO_BIT,
                            "Platform Group = %s\n", GetPlatformGroupStr((*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group, (char *)&strBuf0)));
              switch ((*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].group) {
                case CmdGrp0:
                  maxIoGroup = MAX_CMD_GROUP0;
                  iogPtr = platformCmdGroup0;
                  break;
                case CmdGrp1:
                  maxIoGroup = MAX_CMD_GROUP1;
                  iogPtr = platformCmdGroup1;
                  break;
                case CmdGrp2:
                  maxIoGroup = MAX_CMD_GROUP2;
                  iogPtr = platformCmdGroup2;
                  break;
                case CmdAll:
                  maxIoGroup = MAX_CMD_GROUPALL;
                  iogPtr = platformCmdGroupAll;
                  break;
                default:
                  maxIoGroup = MAX_CMD_GROUPALL;
                  iogPtr = platformCmdGroupAll;
                  status = MRC_STATUS_GROUP_NOT_SUPPORTED;
                  break;
              }

              for (iog = 0; iog < maxIoGroup; iog++) {

                //
                // Get the index into the CMD silicon pi group
                //
                for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
                  if (((iogPtr + iog)->num == platformCmdGroupAll[cmdPiGroup].num) &&
                      ((iogPtr + iog)->side == platformCmdGroupAll[cmdPiGroup].side)) {
                    //
                    // We found the CMD pi group so break out of the loop
                    //
                    MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, rank, NO_STROBE, NO_BIT,
                                  "Found CMD Pi group: %d side %d\n", platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side));
                    break;
                  }
                } // cmdPiGroup loop
                cmdLeft[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].le;
                cmdRight[cmdPiGroup][clk] = (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].re;
                cmdOffset[cmdPiGroup] = 0;
                MemDebugPrint((host, SDBG_MAX, NO_SOCKET, ch, dimm, rank, NO_STROBE, NO_BIT,
                              "cmdLeft[%d][%d] = %d : cmdRight[%d][%d] = %d\n", cmdPiGroup, clk,
                              (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].le,cmdPiGroup, clk,
                              (*groupListEdge)[(*rankList)[rank].rankIndex][listIndex].re));
              } // iog loop


            //}
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins(host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdOffset, 0);


    } else {
      //
      // Average center points
      //
      groupList = value;

      //
      // Initialize for each CMD pi group
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        cmdPiDelaySum[cmdPiGroup] = 0;
        cmdPiDelayCount[cmdPiGroup] = 0;
      } // cmdPiGroup loop

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++){

              switch ((*groupList)[(*rankList)[rank].rankIndex][listIndex].group) {
                case CmdGrp0:
                  maxIoGroup = MAX_CMD_GROUP0;
                  iogPtr = platformCmdGroup0;
                  break;
                case CmdGrp1:
                  maxIoGroup = MAX_CMD_GROUP1;
                  iogPtr = platformCmdGroup1;
                  break;
                case CmdGrp2:
                  maxIoGroup = MAX_CMD_GROUP2;
                  iogPtr = platformCmdGroup2;
                  break;
                case CmdAll:
                  maxIoGroup = MAX_CMD_GROUPALL;
                  iogPtr = platformCmdGroupAll;
                  break;
                default:
                  maxIoGroup = MAX_CMD_GROUPALL;
                  iogPtr = platformCmdGroupAll;
                  status = MRC_STATUS_GROUP_NOT_SUPPORTED;
                  break;
              }

              for (iog = 0; iog < maxIoGroup; iog++) {
            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
              if (((iogPtr + iog)->num == platformCmdGroupAll[cmdPiGroup].num) &&
                  ((iogPtr + iog)->side == platformCmdGroupAll[cmdPiGroup].side)) {

                //
                // Add this pi delay to the total and increment the count
                //
                cmdPiDelaySum[cmdPiGroup] += (*groupList)[(*rankList)[rank].rankIndex][listIndex].value;
                cmdPiDelayCount[cmdPiGroup]++;

                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop
              } // iog group
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Calculate the pi delay for each CMD si group
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) continue;

        cmdPiDelay[cmdPiGroup] = (INT16)(cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        GetSetCmdDelay (host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                        GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdPiDelay[cmdPiGroup]);
      } // cmdPiGroup loop
    }

  } else if ((listType == CMD_SIGNAL_DELAY) || (listType == CMDCTL_SIGNAL_DELAY)) {
    //
    // Combine edges if necessary
    //
    if (mode == GSM_COMBINE_EDGES) {

      signalListEdge = value;

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++) {

            clk = (*rankList)[rank].ckIndex;

            //
            // Get the silicon pi group this signal belongs to
            //
            status = SignalToPiGroup (host, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS; cmdPiGroup++) {
              if(cmdPiGroup >= MAX_CMD_GROUPALL) {
                if ((signalGroup.group.num == platformCmdGroupAll3DS[cmdPiGroup - MAX_CMD_GROUPALL].num) &&
                  (signalGroup.group.side == platformCmdGroupAll3DS[cmdPiGroup - MAX_CMD_GROUPALL].side)) {
                  //
                  // We found the CID pi group so break out of the loop
                  //
                  break;
                }
              } else {
                if ((signalGroup.group.num == platformCmdGroupAll[cmdPiGroup].num) &&
                   (signalGroup.group.side == platformCmdGroupAll[cmdPiGroup].side)) {
                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  break;
                }
              }
            } // cmdPiGroup loop

            //
            // Update the composite left and right edges for the current CMD pi group relative to the clock
            //
            UpdateEdges((*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                        &cmdLeft[cmdPiGroup][clk], &cmdRight[cmdPiGroup][clk]);
            MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                         "%s:\t CMD Pi Group %d clk %d: le %d re = %d, cmdLeft = %d cmdRight = %d\n",
                          GetSignalStr(signalGroup.sig, (char *)&strBuf0), cmdPiGroup, clk,
                          (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].le, (*signalListEdge)[(*rankList)[rank].rankIndex][listIndex].re,
                          cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]));
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Evaluate CMD margins and offset CMD, CTL, and CLK appropriately
      //
      EvaluateCMDMargins(host, socket, ch, ckEnabled, ctlIndex, cmdLeft, cmdRight, cmdOffset, 1);

    } else {
      //
      // Average center points
      //
      signalList = value;

      //
      // Initialize for each CMD pi group
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        cmdPiDelaySum[cmdPiGroup] = 0;
        cmdPiDelayCount[cmdPiGroup] = 0;
      } // cmdPiGroup loop

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        rankList = GetRankNvList(host, socket, ch, dimm);

        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;

          //
          // Loop through the list to create the composite left and right edges for each CMD group
          //
          for (listIndex = 0; listIndex < listSize; listIndex++){

            //
            // Get the silicon pi group this signal belongs to
            //
            status = SignalToPiGroup (host, (*signalList)[(*rankList)[rank].rankIndex][listIndex].signal, &signalGroup);

            //
            // Get the index into the CMD silicon pi group
            //
            for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
              if ((signalGroup.group.num == platformCmdGroupAll[cmdPiGroup].num) &&
                  (signalGroup.group.side == platformCmdGroupAll[cmdPiGroup].side)) {

                //
                // Add this pi delay to the total and increment the count
                //
                cmdPiDelaySum[cmdPiGroup] += (*signalList)[(*rankList)[rank].rankIndex][listIndex].value;
                cmdPiDelayCount[cmdPiGroup]++;

                //
                // We found the CMD pi group so break out of the loop
                //
                break;
              }
            } // cmdPiGroup loop
          } // listIndex loop
        } // rank loop
      } // dimm loop

      //
      // Calculate the pi delay for each CMD pi group
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        if (cmdPiDelayCount[cmdPiGroup] == 0) continue;

        cmdPiDelay[cmdPiGroup] = (INT16)(cmdPiDelaySum[cmdPiGroup] / cmdPiDelayCount[cmdPiGroup]);
      } // cmdPiGroup loop

      //
      // Program the delay
      //
      for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
        GetSetCmdDelay (host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                        GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdPiDelay[cmdPiGroup]);
      } // cmdPiGroup loop

    }

  } else {
    status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  return status;
} // SetCombinedCmdGroup

void
EvaluateCMDMargins (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK],
  UINT8     ctlIndex[MAX_CLK],
  INT16     cmdLeft[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS][MAX_CLK],
  INT16     cmdRight[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS][MAX_CLK],
  INT16     cmdOffset[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS],
  UINT8     evaluateCIDs
  )
/*++

  Evaluates the CMD margins and determines the appropriate offset

  @param host                 - Pointer to host structure
  @param socket               - Processor socket within the system (0-based)

  @retval MRC_STATUS

--*/
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 clk;
  UINT8                 cmdPiGroup;
  INT16                 minCmdOffset;
  INT16                 maxCmdOffset;
  INT16                 cmdOffsetClk[MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS][MAX_CLK];
  INT16                 maxLeftOffset;
  UINT16                ctlMinVal;
  UINT16                ctlMaxVal;
  INT16                 minRightOffset;
  INT16                 ckOffset[MAX_CLK];
  UINT8                 earlyCmdClkDisable = 0;
  UINT8                 parityWorkaround = 0;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];

  //
  // Determine if early CMD/CLK training has been disabled
  //
  dimmNvList = GetDimmNvList(host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    parityWorkaround = CheckParityWorkaround(host, socket, ch, dimm);

    if(parityWorkaround) break;
  } // dimm loop

  earlyCmdClkDisable = ((~host->memFlows & MF_E_CMDCLK_EN) || ((host->setup.mem.optionsExt & EARLY_CMD_CLK_TRAINING_EN) == 0) ||
        (host->nvram.mem.dramType != SPD_TYPE_DDR4) || parityWorkaround);

  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) continue;
    //
    // Initialize these values so the maximum CMD delay can be determined
    //
    minCmdOffset = 255;
    maxCmdOffset = -255;

    //
    // Determine the offset for each CMD pi group and the max offset
    //
    for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS * evaluateCIDs; cmdPiGroup++) {

      //
      // Make sure the eye width is large enough
      //
      if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < 40) {
        MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                      "Eye width is too small: %d\n",
                      cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]));
        OutputWarning (host, WARN_CMD_CLK_TRAINING, 0, socket, ch, NO_DIMM, NO_RANK);
        DisableChannel(host, socket, ch);
        return;
      }
      cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

      UpdateMinMaxInt(cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                     "CMD Pi Group %d clk %d cmdOffset %d\n",
                      cmdPiGroup, clk, cmdOffsetClk[cmdPiGroup][clk]));
    } // cmdPiGroup loop

  } // clk offset

  //
  // Calculate the final offset for each CMD pi group
  //
  for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS * evaluateCIDs; cmdPiGroup++) {
    // Track minCgOffset, maxCgOffset across all CK groups
    minCmdOffset = 255;
    maxCmdOffset = -255;

    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk]) {

        //if(moveClk) {
        //  UpdateMinMaxInt(cmdPiShift[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
        //} else {
          UpdateMinMaxInt(cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
        //}
      }
    } // clk loop

    // Save common CMD offset
    cmdOffset[cmdPiGroup] = (maxCmdOffset + minCmdOffset) / 2;


    if(cmdPiGroup >= MAX_CMD_GROUPALL) {
      //
      // Write the ctl pi Group
      //
      GetSetCtlDelay (host, socket, ch, platformCmdGroupAll3DS[cmdPiGroup - MAX_CMD_GROUPALL].num, platformCmdGroupAll3DS[cmdPiGroup - MAX_CMD_GROUPALL].side,
                      GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdOffset[cmdPiGroup]);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "CMD Pi Group %d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                    cmdPiGroup - MAX_CMD_GROUPALL, maxCmdOffset, minCmdOffset, cmdOffset[cmdPiGroup]));
    } else {
      //
      // Write the CMD pi Group
      //
      GetSetCmdDelay (host, socket, ch, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
                      GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdOffset[cmdPiGroup]);

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "CMD Pi Group %d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                    cmdPiGroup, maxCmdOffset, minCmdOffset, cmdOffset[cmdPiGroup]));
    }
  } // cmdPiGroup loop


  //
  // Determine the clock offset
  //
  for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL + MAX_CMD_GROUPALL_3DS * evaluateCIDs; cmdPiGroup++) {
    for (clk = 0; clk < MAX_CLK; clk++) {
      if (ckEnabled[clk] == 0) continue;

      cmdLeft[cmdPiGroup][clk] = cmdLeft[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
      cmdRight[cmdPiGroup][clk] = cmdRight[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];

      MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                    "<--CMD Pi Group %d clk %d: cmdLeft %d - cmdRight %d\n",
                      cmdPiGroup, clk, cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]));
    } // clk offset
  } // cmdPiGroup loop

  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) continue;

    minRightOffset = 255;
    maxLeftOffset = -255;

    for (cmdPiGroup = 0; cmdPiGroup < MAX_CMD_GROUPALL; cmdPiGroup++) {
      UpdateMinMaxInt(cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
      UpdateMinMaxInt(cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
    } // cmdPiGroup loop

    ckOffset[clk] = -(minRightOffset + maxLeftOffset) / 2;

    //
    // Write the CTL delay
    //
    ctlMinVal = 255;
    ctlMaxVal = 0;
    GetSetCtlGroupDelay (host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk], &ctlMinVal, &ctlMaxVal);

    //
    // Write the CLK delay
    //
    GetSetClkDelay (host, socket, ch, clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk]);
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    if (earlyCmdClkDisable || (host->var.mem.earlyCmdClkExecuted == 1)){
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList(host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) continue;
          if ( clk == (*rankList)[rank].ckIndex) {
            GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                     (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                     (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (host, socket, ch, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
                     (INT16 *)&ckOffset[clk]);
          }
        } // rank loop
      } // dimm loop
    }

    MemDebugPrint((host, SDBG_MAX, socket, ch, NO_DIMM, NO_RANK, NO_STROBE, NO_BIT,
                   "<----clk %d ckOffset %d: -(maxLeftOffset:%d + minRightOffset:%d) / 2\n",
                     clk, ckOffset[clk], maxLeftOffset, minRightOffset));

  } // clk loop

} // EvaluateCMDMargins
