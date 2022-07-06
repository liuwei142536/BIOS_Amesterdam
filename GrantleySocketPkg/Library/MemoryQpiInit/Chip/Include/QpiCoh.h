//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2015 Intel Corp.                             *
// *                                                                        *
// *      This program has been developed by Intel Corporation.             *
// *      Licensee has Intel's permission to incorporate this source code   *
// *      into their product, royalty free.  This source code may NOT be    *
// *      redistributed to anyone without Intel's written permission.       *
// *                                                                        *
// *      Intel specifically disclaims all warranties, express or           *
// *      implied, and all liability, including consequential and other     *
// *      indirect damages, for the use of this code, including liability   *
// *      for infringement of any proprietary rights, and including the     *
// *      warranties of merchantability and fitness for a particular        *
// *      purpose.  Intel does not assume any responsibility for any        *
// *      errors which may appear in this code nor any responsibility to    *
// *      update it.                                                        *
// *                                                                        *
// **************************************************************************
// **************************************************************************
// *                                                                        *
// *  PURPOSE:                                                              *
// *                                                                        *
// *      This file contains platfrom specific routines which might need    *
// *      porting for a particular platform.                                *
// *                                                                        *
// **************************************************************************
--*/

#ifndef _QPI_COH_H_
#define _QPI_COH_H_

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"

QPI_STATUS
AllocateRtids (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtidSHT1SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtidSHT1SBdxHCCDE (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtidSHTIsoc2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtidSHTNonIsoc2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtidDHTCommonHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

#ifdef QPI_4S_COD_FEATURE_PROTOTYPE
VOID  
AllocateRtids4SCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );
#endif
  
VOID  
AllocateRtids4SCODBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

UINT8
NumCBosPerRBT2SCOD (
  UINT8 NumCBos,
  UINT8 RBT
  );
  
UINT8 
RTIDsPerCBo2SCOD(
  UINT8 NumCBos,
  UINT8 xRTID,
  UINT8 RBT,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

UINT8
COD2SCBo(
  UINT8 RBTEntry, 
  UINT8 NumCBos, 
  UINT8 xRTID,
  UINT8 RBT,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );
               
VOID
AllocateRtidDHTCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtid8SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
AllocateRtid8SBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

UINT8
HandleCboRtidStraddle (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      *RtidBase,
  UINT8                      *RtidSize,
  UINT8                      *ExtraRtids,
  UINT8                      CboRtidBase,
  UINT8                      CboIndex,
  UINT8                      RtidPerCbo,
  UINT8                      ReallocBase
  );

VOID
CheckAndSplitCboRtids (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      *RtidBase,
  UINT8                      *RtidSize
  );

QPI_STATUS
ProgramRtids (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

VOID
ProgramRtidSHT1SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
ProgramRtidSHT2SHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
ProgramRtidCommon (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
ProgramRtidCod (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

#ifdef QPI_4S_COD_FEATURE_PROTOTYPE 
VOID
ProgramRtid4SCODHsx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );
#endif  
  
VOID
ProgramRtid4SCODBdx (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );
  
QPI_STATUS
ProgramRingCredits (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
ProgramRingCredits1S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
ProgramRingCreditsCommonHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
ProgramR3QpiAndQpiAgentCreditIvt (
  struct sysHost      *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8               Soc,
  UINT8               Port,
  R3QPI_RING_CREDIT_PARAM   *R3QpiRingCreditParam
 );


VOID
PushDownRtidBase (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      LocalIdx,
  UINT8                      RemoteIdx
  );

QPI_STATUS
InitializeMCTP (
  struct sysHost          *host,
  QPI_SOCKET_DATA         *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );
  

VOID
DumpRtidAllocationSHT (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
DumpRtidAllocation (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
DumpRtidAllocationBdxBtMode2 (
  struct sysHost             *host,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

#define RTID_BASE_OFFSET(x) ((x >= (MAX_RBT_ENTRIES/2)) ? (x - (MAX_RBT_ENTRIES/2)) : x)

#endif // _QPI_COH_H_
