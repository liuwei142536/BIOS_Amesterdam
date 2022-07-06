//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*************************************************************************
 *
 * Reference Code
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
 *
 ************************************************************************/
#ifndef  _platformfunccommon_h
#define  _platformfunccommon_h

#ifndef ASM_INC
#include "DataTypes.h"
#include "SysHost.h"

// OemMemoryQpiInit.c / OemProcMemInit.c
VOID   OemInitializePlatformData (struct sysHost *host);
VOID   OemCheckAndHandleResetRequests (struct sysHost *host);
VOID   MEBIOSCheckAndHandleResetRequests (struct sysHost *host);
UINT32 OemMemRiserInfo (struct sysHost *host);
UINT32 OemMemRASHook (struct sysHost *host, UINT8  flag ); // flag = 0 (Pre MRC RAS hook), flag = 1 (Post MRC RAS hook)
UINT8  OemGetPlatformType (struct sysHost *host);
UINT32 OemGetPlatformRevId (struct sysHost *host);
VOID   OemPublishDataForPost (struct sysHost *host);
VOID   OemTurnOffVrsForHedt (struct sysHost *host);
VOID   OemPlatformHookMst (PSYSHOST host,UINT8 socket, struct smbDevice dev, UINT8 byteOffset, UINT8 *data);
#ifdef SSA_FLAG
BOOLEAN OemDetectPhysicalPresenceSSA (struct sysHost *host);
#endif  //SSA_FLAG
VOID   OemPostQpiInit (struct sysHost *host, UINT32 QpiStatus);
VOID   OemPreQpiInit (struct sysHost *host);
VOID   OemPreUncoreInit (struct sysHost *host);

// CommonSetup.c
void GetSetupOptions(struct sysSetup *setup);

// CommonHooks.c
void PlatformInitSmb(PSYSHOST host);

// MemHooks.c
void PlatformMemInitGpio(PSYSHOST host);
void PlatformMemSelectSmbSeg(PSYSHOST host, UINT8 seg);
void PlatformClearADR (PSYSHOST host);
UINT32 PlatformSetErrorLEDs (PSYSHOST host, UINT8 socket);
UINT32 OemUpdatePlatformConfig(PSYSHOST host);
UINT32 OemInitThrottlingEarly (PSYSHOST host);
UINT32 OemDetectDIMMConfig (PSYSHOST host);
UINT32 OemInitDdrClocks (PSYSHOST host);
UINT32 OemSetDDRFreq (PSYSHOST host);
UINT32 OemConfigureXMP (PSYSHOST host);
UINT32 OemCheckVdd (PSYSHOST host);
UINT32 OemEarlyConfig (PSYSHOST host);
UINT32 OemLateConfig (PSYSHOST host);
UINT32 OemInitThrottling (PSYSHOST host);
UINT32 PlatformDramMemoryTest (PSYSHOST host, UINT8 socket, UINT8 chEnMap);

VOID PlatformExMemoryLinkReset (PSYSHOST host);

// CpuSetup.c
void GetCpuSetupOptions(struct sysSetup *setup);

// CpuHooks.c
void PlatformCpuTemp(PSYSHOST host);

// KtiSetup.c
void GetKtiSetupOptions(struct sysSetup *setup);

// MemSetup.c
void GetMemSetupOptions(PSYSHOST host);

// QpiSetup.c
void GetQpiSetupOptions(struct sysSetup *setup);

// UncoreSetup.c
void GetUncoreSetupOptions(struct sysSetup *setup);

// KtiHooks.c
INT32 OemKtiGetEParams (struct sysHost *host, UINT8 SocketID, UINT8 Link, UINT8 Freq, VOID *LinkSpeedParameter);
BOOLEAN OemKtiGetMmioh(struct sysHost *host, UINT16 *MmiohSize, UINT16 *MmiohBaseHi);
BOOLEAN OemKtiGetMmiol(struct sysHost *host, UINT8 *MmiolSize, UINT32 *MmiolBase);
BOOLEAN OemCheckCpuPartsChangeSwap(struct sysHost *host);

//MemHooks.c
BOOLEAN OemGetAdaptedEqSettings (struct sysHost *host, UINT8 Speed, VOID *AdaptTbl);
VOID OemDebugPrintKti (struct sysHost *host,UINT32 DbgLevel, char* Format, ...);
VOID OemWaitTimeForPSBP (struct sysHost *host, UINT32 *WaitTime );

void OemEarlyVideo (VOID);

#endif // ASM_INC
#endif  //_platformfunccommon_h
