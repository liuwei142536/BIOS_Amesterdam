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
 *      This file contains
 *
 ************************************************************************/

#ifndef _sysfunc_h
#define _sysfunc_h
#include <Token.h> // AptioV server override
#include "DataTypes.h"
#include "SysHost.h"
#include "CpuPciAccess.h"
#include "SysFuncChipCommon.h"
#include "SysFuncChip.h"
#include "ChipApi.h"
#ifdef SSA_FLAG
#include "MrcSsaServices.h"
#include "BiosSsaChipFunc.h"
#endif //SSA_FLAG

//************************** CHIP to CORE API Prototypes ***************************************
//
// Printf.c
//
void   DebugPrintRc (PSYSHOST  host, UINT32 dbgLevel, char* Format, ...);                    // CORE / CHIP

//
// Pipe.c
//
UINT32 SyncErrorsTbl(PSYSHOST host);                                                          // CALLTABLE
UINT8  SyncErrors(PSYSHOST host);                                                             // CHIP

//
// Decompress.c
//


//
// PmTimer.c
//
UINT32 GetTimeInSecondsFrom1970 (VOID);

#define  NO_TIMEOUT  0xFFFFFFFF

//
// Math.c
//
UINT32 GetPowerOfTwox32 (UINT32 Input);                                                       // CHIP
BOOLEAN IsPowerOfTwo (UINT32 Value);                                                          // CHIP

//
// Error.c
//
void CollectPrevBootFatalErrors(PSYSHOST host);                                               // CHIP
void MemCheckBoundary(PSYSHOST host, UINT32 boundary,UINT32 arrayBoundary);                   // CHIP
void MemCheckIndex(PSYSHOST host, UINT32 boundary,UINT32 arrayBoundary);                      // CHIP

//************************** CORE to CHIP API Prototypes ***************************************

//
// Smbus.c
//

//
// DebugCmd.c
//


//
// ProcMemInit.c
//
/**
  SAD target CF8 value for get SBSP.
  @retval SAD target CF8 value

**/
UINT32 SadTargetCf8Chip (void);

/**
  Get SBSP id for legacy remote SBSP
  @param   -  pointer to host struct
  @param   -  legacy Pch Target

  @retval legacy remote SBSP

**/
UINT8  LegacyRemoteSBSPChip (PSYSHOST host, UINT32 legacyPchTarget);

//
//************************** CORE only Prototypes *************************************************
//

//
// Pipe.c
//
void   UnlockCommandPipe(PSYSHOST Host, UINT8 Id);                                          // CORE
void   LockCommandPipe(PSYSHOST Host, UINT8 Id);                                            // CORE
void   WaitForCommandPipeUnLocked(PSYSHOST Host, UINT8 Id);                                 // CORE
BOOLEAN AcquireCommandPipe(PSYSHOST Host, UINT8 Id);                                        // CORE
void   WriteCommandPipe(PSYSHOST Host, UINT8 Id, UINT32 *CmdPipe);                          // CORE
UINT32 SendCommandPipe(PSYSHOST Host, UINT8 Id, UINT32 *CmdPipe);                           // CORE
UINT32 GetPipePackage(PSYSHOST Host, UINT8 Id, UINT8 *pBuffer, UINT32 Size);                // CORE
UINT32 SendPipePackage(PSYSHOST Host, UINT8 Id, UINT8 *pBuffer, UINT32 Size);               // CORE
UINT32 WaitForPipeCommand(PSYSHOST Host, UINT8 Id, UINT32 CmdType);                         // CORE
void   PipeDispatcher(PSYSHOST host);                                                       // CORE
UINT32 PipeInit(PSYSHOST host);                                                             // CORE

//
// PipeDataSync.c - CORE
//
void GetMemVarData(PSYSHOST host, UINT8 socketId);              // CORE
void SendMemVarData(PSYSHOST host, UINT8 socketId);             // CORE
void GetMemNvramCommonData(PSYSHOST host, UINT8 socketId);      // CORE
void SendMemNvramCommonData(PSYSHOST host, UINT8 socketId);     // CORE
void GetMemNvramData(PSYSHOST host, UINT8 socketId);            // CORE
void SendMemNvramData(PSYSHOST host, UINT8 socketId);           // CORE

//
// IoAccess.c
//
UINT8  InPort8_(UINT16 ioAddress);                                        // CORE
void   OutPort8_(UINT16 ioAddress, UINT8 data);                           // CORE

//
// CpuAccess.c
//
UINT8  GetProcApicId();                                        // CORE

//
// UsbDebugPort.c
//
UINT32 Usb2DebugPortSend (PSYSHOST host, UINT8 *Data, UINT32 *Length);   // CORE
UINT32 Usb2DebugGetChar (PSYSHOST host, char *c, UINT32 usTimeout);      // CORE

//
// MemUtil.c
//
UINT32 MemScan(UINT8 *dest, UINT32 value, UINT32 numBytes);              // NONE
UINT32 MemCompare(UINT8 *dest, UINT8 *src, UINT32 numBytes);             // CORE

//
// CpuPciAccess.c
//
//
//Moved from MemHooks.c to Error.c
//
UINT32 MemInitWarning(PSYSHOST host);

#endif // _sysfunc_h
