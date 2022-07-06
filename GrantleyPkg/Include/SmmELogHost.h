//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 2007 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  SmmELogHost.h

Abstract:

--*/

#ifndef _SMM_ELOG_HOST_H
#define _SMM_ELOG_HOST_H
#include "SysFunc.h"

//
// ELOG Host Structure
//
typedef struct {
  UINT8	maxDimm;
  UINT8	dimmEnabled;
  UINT8 dimmNum;
}DIMM_INFO;

typedef struct _SMMELOG_CHANNELLIST {
  UINT8 chEnabled;
  UINT8 chFailed;
  UINT8 spareDimm;
  UINT8 spareLogicalRank;
  UINT8 spareInUse;
  UINT8 oldLogicalRank;
  UINT8 devTag[MAX_RANK_CH];
  UINT32 leakyBucket2ndCounter;
  UINT32 rankErrCountInfo[MAX_RANK_CH];
  UINT8  maxDimm;
  DIMM_INFO dimmInfo[MAX_DIMM];
} SMMELOG_CHANNELLIST;

typedef struct _SMMELOG_IMC {
  UINT8 enabled;
  SMMELOG_CHANNELLIST channelList[MAX_CH];
} SMMELOG_IMC;

typedef struct _SMMELOG_CPUINFO {
  UINT8 cpuNumber[(MAX_CORE * MAX_THREAD)];
  UINT32 ApicId[(MAX_CORE * MAX_THREAD)];
  UINT8 ApicVer[(MAX_CORE * MAX_THREAD)];
} SMMELOG_CPUINFO;


typedef struct _SMMELOG_HOST {
  UINT8 bootMode;
  UINT8 RASmode[MAX_MEM_NODE];
  UINT8 RASModesEx[MAX_MEM_NODE];
  UINT8 cpuCount;
  UINT32 socketPresentBitMap;
  UINT16 spareErrTh;
  UINT32 options;
  UINT32 mmCfgBase;
  UINT8 busIio[MAX_SOCKET];
  UINT8 busUncore[MAX_SOCKET];
  UINT16 CpuStepping;
  SMMELOG_IMC imc[MAX_MEM_NODE];
  SMMELOG_CPUINFO cpuInfo[MAX_SOCKET];
} SMMELOG_HOST;

extern SMMELOG_HOST mELogHost;

#endif
