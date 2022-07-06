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

#ifndef _QPI_LIB_H_
#define _QPI_LIB_H_

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"
#include "CpuPciAccess.h"

UINT8
GetSbspSktIdFromLocal (
  struct sysHost   *host
  );

QPI_STATUS
GetSocketSkuType (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *SocketSkuType,
  UINT8                      *SubSkuType
  );

QPI_STATUS
GetCboCountAndList (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *CboCount,
  UINT32                     *CboMap  
  );

QPI_STATUS
GetSboCount (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *SboCount
  );

QPI_STATUS
GetHACount (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *HaCount
  );

QPI_STATUS
GetR3QPICount (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *R3QpiCount
  );

QPI_STATUS
GetQPIAgentCount (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *QpiAgentCount
  );

QPI_STATUS
GetLegacyPchTargetSktId (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      *LegacyPchSktId
  );

QPI_STATUS
ParseCpuLep (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      SocId
  );

QPI_STATUS
AllocateIioResources (
  struct sysHost          *host,
  QPI_SOCKET_DATA         *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
CalculateResourceRatio (
  struct sysHost          *host,
  QPI_SOCKET_DATA         *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8                   *BusSize,
  UINT8                   *IoSize,
  UINT8                   *MmiolSize,
  UINT8                   *IoApicSize
  );

QPI_STATUS
FillSadTargetList (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT32                 *IoTgtLst,
  UINT32                 *ApicTgtLst,
  UINT32                 *MmioTgtLst
  );

QPI_STATUS
ConvertR3QPIRoutingTable (
  UINT8                   InPort,
  UINT8                   OutPort,
  UINT8                   DestSocId,
  UINT32                  *EncodedRoute
  );

QPI_STATUS
CheckForTopologyChange (
  struct sysHost          *host,
  QPI_SOCKET_DATA         *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
SanityCheckDiscoveredTopology (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SanityCheckLep (
  struct sysHost         *host,
  QPI_CPU_SOCKET_DATA    *CpuData
  );

QPI_STATUS
DegradeTo1S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
PreProcessFeatureVsTopology (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

VOID
TopologyInfoDump (
  struct sysHost      *host,
  QPI_CPU_SOCKET_DATA     *Cpu,
  TOPOLOGY_TREE_NODE     (*CpuTree)[MAX_TREE_NODES],
  BOOLEAN              SiPort
  );

VOID
RouteTableDump (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal  
  );

VOID
LepDump (
  struct sysHost         *host,
  QPI_CPU_SOCKET_DATA    *CpuData
  );

UINT8
TranslateToGenPort (
  struct sysHost             *host,
  UINT8                   SiPort
  );

UINT8
TranslateToSiPort (
  struct sysHost          *host,
  UINT8                   GenPort
  );

BOOLEAN
CheckCpuConnectedToLegCpu (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      SocId
  );

BOOLEAN
DuplicateLinkExists (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  );

QPI_STATUS
RemoveDualLink(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData
  );

QPI_STATUS
GetDualLinkFromLEP (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *CpuData,
  TOPOLOGY_TREE_NODE      *Node1,
  TOPOLOGY_TREE_NODE      *Node2,
  UINT8                   *Port1,
  UINT8                   *Port2
  );

UINT8
GetChildLocalPortFromLEP (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA     *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  );

UINT8
GetChildPeerPortFromLEP (
  struct sysHost          *host,
  QPI_CPU_SOCKET_DATA    *CpuData,
  TOPOLOGY_TREE_NODE      *Parent,
  TOPOLOGY_TREE_NODE      *Child
  );

QPI_STATUS
GetParentSocket (
  struct sysHost         *host,
  TOPOLOGY_TREE_NODE     *TopologyTree,
  TOPOLOGY_TREE_NODE     *Child,
  TOPOLOGY_TREE_NODE     *Parent
  );

UINT8
GetParentIndex (
  struct sysHost           *host,
  QPI_CPU_SOCKET_DATA      *CpuData,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Child
  );

UINT8
GetChildHopLength (
  struct sysHost       *host,
  TOPOLOGY_TREE_NODE   *Tree,
  UINT8                SocId,
  UINT8                SocType
  );

BOOLEAN
NodeFoundInTopologyTree (
  struct sysHost           *host,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *SearchNode
  );

BOOLEAN
NodeFoundInMinPathTree (
  struct sysHost          *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  );

QPI_STATUS
AddNodeToTopologyTree (
  struct sysHost           *host,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Node
  );

QPI_STATUS
AddNodeToRingTree (
  struct sysHost          *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  );

UINT8
CountImmediateChildren (
  struct sysHost           *host,
  TOPOLOGY_TREE_NODE       *Tree,
  TOPOLOGY_TREE_NODE       *Parent
  );

BOOLEAN
ShorterPathExists (
  struct sysHost          *host,
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  );

BOOLEAN
CompareRing (
  struct sysHost       *host,
  UINT8                *Ring1,
  UINT8                *Ring2
  );

VOID
QpiCheckPoint (
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      QpiLink,
  UINT8                      MajorCode,
  UINT8                      MinorCode,
  struct sysHost             *host
  );

VOID
QpiAssert (
  struct sysHost             *host,
  UINT8                      MajorCode,
  UINT8                      MinorCode
  );

VOID
QpiLogWarning (
  struct sysHost             *host,
  UINT8                      SocId,
  UINT8                      SocType,
  UINT8                      QpiLink,
  UINT8                      WarnCode
  );

UINT32
QpiReadPciCfg(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset
);

VOID
QpiWritePciCfg(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT32                RegOffset,
  UINT32                data
  );

#define QPILIB_IAR_IOVC  20
#define QPILIB_IAR_PDQ   62
#define QPILIB_IAR_BCAST 63

UINT8
QpiReadIar(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT8                 RegOffset
  );

VOID
QpiWriteIar(
  struct sysHost        *host,
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT8                 RegOffset,
  UINT8                 data
  );

VOID
SpinUntilLbcIsFree (
  struct sysHost            *host,
  UINT8                     Socket,
  UINT8                     LinkIndex
  );

#ifdef BUILDING_FOR_X64
UINT32
ReadCpuPciCfgRas (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset
  );

void
WriteCpuPciCfgRas (
  PSYSHOST host,
  UINT8    SocId,
  UINT8    BoxInst,
  UINT32   Offset,
  UINT32   Data
  );
#endif

UINT64_STRUCT
AddUINT64 (
  UINT64_STRUCT Operand1,
  UINT64_STRUCT Operand2
  );

UINT64_STRUCT
AddUINT64x32 (
  UINT64_STRUCT Operand1,
  UINT32        Operand2
  );

UINT64_STRUCT
SubUINT64 (
  UINT64_STRUCT Operand1,
  UINT64_STRUCT Operand2
  );

UINT64_STRUCT
SubUINT64x32 (
  UINT64_STRUCT Operand1,
  UINT32        Operand2
  );

UINT64_STRUCT
MultUINT64x32 (
  UINT64_STRUCT Multiplicand,
  UINT32        Multiplier
  );

UINT64_STRUCT
LShiftUINT64 (
  UINT64_STRUCT Data,
  UINT32        Count
  );
     
VOID
QpiFixedDelay (
  struct sysHost      *host,
  UINT32          MicroSecs
  );

QPI_STATUS
QpiMain (
  struct sysHost             *host
  );

QPI_STATUS
QpiEvAutoRecipe (
  struct sysHost           *host,
  QPI_SOCKET_DATA          *SocketData,
  UINT8                    Socket,
  UINT8                    LinkIndex
  );

QPI_STATUS
OemQpiGetEParams (
  struct sysHost           *host,
  UINT8                    SocketID,
  UINT8                    Link,
  UINT8                    Freq,
  UINT8                    ProbeType,
  VOID                     *LinkSpeedParameter,
  UINT8                    SpeedType
  );

UINT32
OemPlatformSpecificEParam (
  struct sysHost             *host,
  UINT32                     *SizeOfTable,
  UINT32                     *per_lane_SizeOfTable,
  HSX_ALL_LANES_EPARAM_LINK_INFO **ptr,
  HSX_PER_LANE_EPARAM_LINK_INFO  **per_lane_ptr
  );

BOOLEAN
OemQpiGetMmioh(
   struct sysHost           *host,
   UINT16                   *MmiohSize,
   UINT16                   *MmiohBaseHi
   );

BOOLEAN
OemQpiGetMmiol(
   struct sysHost           *host,
   UINT8                    *MmiolSize,
   UINT32                   *MmiolBase
   );

QPI_STATUS
OemQpiGetHalfSpeedEParams (
  struct sysHost           *host,
  UINT8                     SocketID,
  UINT8                     Link,
  UINT8                     Freq,
  UINT8                     ProbeType,
  HS_EPARAM_LINK_INFO      *LinkHalfSpeedParameter
  );

VOID
OemDebugPrintQpi (
    struct sysHost *host,
    UINT32 DbgLevel,
    char* Format,
    ...
  );

VOID
OemWaitTimeForPSBP (
    struct sysHost *host,
    UINT32 *WaitTime
  );

VOID
GetNeighborSocs (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                      Soc,
  UINT8                      *NeighborSoc1,
  UINT8                      *NeighborSoc2,
  UINT8                      *RemoteSoc
  );

QPI_STATUS
SendMailBoxCmdToPcode (
  struct sysHost      *host,
  UINT8               Cpu,
  UINT32              Command,
  UINT32              Data
  );

BOOLEAN
Is4SCODSupportedOnTopology (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

UINT8
EfiDisableInterrupts_1(
VOID
  );

UINT8
EfiHalt_1(
VOID
  );


#endif // _QPI_LIB_H_
