/*++
  This file contains 'Framework Code' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may not be modified, except as allowed by
  additional terms of your license agreement.
--*/
/*++
// **************************************************************************
// *                                                                        *
// *      Intel Restricted Secret                                           *
// *                                                                        *
// *      QPI Reference Code                                                *
// *                                                                        *
// *      ESS - Enterprise Silicon Software                                 *
// *                                                                        *
// *      Copyright (c) 2004 - 2016 Intel Corp.                             *
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
// *      This file contains entry point of QPIRC.                          *
// *                                                                        *
// **************************************************************************
--*/

#include "DataTypes.h"
#include "PlatformHost.h"
#include "SysHost.h"
#include "SysFunc.h"
#include "QpiLib.h"
#include "QpiCoh.h"
#include "QpiMisc.h"

extern     CreditTablePara   CreditTableHsx[MAX_CONFIG_TYPE];
extern     CreditTablePara   CreditTableBdx[MAX_CONFIG_TYPE];

typedef enum {
  QPIRAS_EVNT_ONLINE = 0,      // Logical/Physical Online event
  QPIRAS_EVNT_OFFLINE,         // Logical/Physical Offline event
  QPIRAS_EVNT_RESET_SM         // Reset QPI RAS state machine
}QPIRAS_EVENT_TYPE;

VOID  
DataDumpQpiRcEntry (
  struct sysHost          *host
  );

VOID  
DataDumpQpiRcExit (
  struct sysHost          *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal  
  );

QPI_STATUS
FillEarlySystemInfo (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
SetupSbspPathToAllSockets (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
SetupSbspConfigAccessPath (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE         *Interm,
  TOPOLOGY_TREE_NODE         *Dest,
  UINT8                      DestPort
  );

QPI_STATUS
SetupRemoteCpuBootPath (
  struct sysHost             *host,
  QPI_SOCKET_DATA            *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE         *Interm,
  TOPOLOGY_TREE_NODE         *Dest,
  UINT8                      DestPort
  );

QPI_STATUS
ProgramR3QpiRoute (
  struct sysHost            *host,
  UINT8                     InPort,                 
  UINT8                     InterSocket,
  UINT8                     DestSocId,
  UINT8                     OutPort  
  );

QPI_STATUS
ProgramQpiAgentRouteForWa (
  struct sysHost            *host,
  UINT8                     InPort,                 
  UINT8                     InterSocket,
  UINT32                    *Data32Ptr
  );

QPI_STATUS
UpdateCpuInfoInQpiInternal(
 struct sysHost             *host,
 QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
 UINT8                      SocktId
 );

BOOLEAN
CheckThisSocketInfoWithSbsp(
 struct sysHost             *host,
 QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
 UINT8                      SbspSktId,
 UINT8                      SocketId
 );

QPI_STATUS
PreProcessQpiLinks (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
CheckForDegraded4S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
HandleDegraded4S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA       *DiscEngData
  );

#if MAX_CPU_SOCKETS > 4
QPI_STATUS
CheckForDegraded8S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
HandleDegraded8S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
DegradeTo6S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA        *DiscEngData,
  UINT8                CommonSoc
  );
#endif

QPI_STATUS
PrimeHostStructure (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
PrimeHostStructureIsoc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
PrimeHostStructureIvt (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
PrimeHostStructureHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
IdentifySysConfigTypeHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SyncUpPbspForReset (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                  RstType
  );

QPI_STATUS
ProgramSystemRoute (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupSystemIoSadEntries (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
ProgramCpuIoSadEntries (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                  SocId,
  UINT32                 IoTgtLst,
  UINT32                 ApicTgtLst,
  UINT32                 MmioTgtLst
  );

QPI_STATUS
ProgramSystemAddressMap (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8                  SocId
  );

QPI_STATUS
SetupHaCredits (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal 
  );

QPI_STATUS
SetupHaCreditsIvt (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal 
  );

QPI_STATUS
SetupHaCreditsHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal 
  );

QPI_STATUS
SetupSystemCoherency (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupSystemCoherencyIvt (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupSystemCoherencyHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupQpiMisc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupQpiMiscIvt (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupQpiMiscHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
CollectAndClearErrors (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupRbt(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
CheckS3ResumePath (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
SetupHtBase(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
QpiTopologyDiscovery (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
QpiTransitionFullSpeed (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
QpiPcuMiscConfig (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS
PrepareDiscEngData (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_DISC_DATA       *DiscEngData
  );

QPI_STATUS
LocateRings (
  struct sysHost      *host,
  QPI_DISC_DATA       *DiscEngData
  );

QPI_STATUS
QpiPhyLinkAfterWarmReset (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS QpiPhyLinkL1WaHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );

QPI_STATUS QpiPhyLBC46ClearOffWA (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  );


QPI_STATUS
ProgramMsrForWa (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
HideOrUnHideDev (
  struct sysHost      *host,
  UINT8                Soc,
  UINT32               DevBitMap,
  BOOLEAN              Hide,
  UINT8                FuncStart,
  UINT8                FuncEnd
  );

QPI_STATUS
QpiProgramBGFOverrides (
  struct sysHost         *host,
  QPI_SOCKET_DATA        *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  );

QPI_STATUS
QpiMain (
  struct sysHost             *host
  )
/*++

Routine Description:

  QPI entry point

Arguments:

  host  - Pointer to the system host (root) structure

Returns:

  QPI STATUS

--*/
{
  QPI_SOCKET_DATA SocketData;
  QPI_HOST_INTERNAL_GLOBAL   QpiInternalGlobal;  

  //
  // Dump all the setup options and other input options that are expected to be initialized at this point
  //
  DataDumpQpiRcEntry (host);

  //
  // Initialize SocketData to zero
  //
  MemSetLocal ((UINT8 *) &SocketData, 0x00, sizeof (QPI_SOCKET_DATA));
  MemSetLocal ((UINT8 *) &QpiInternalGlobal, 0x00, sizeof (QPI_HOST_INTERNAL_GLOBAL));

  //
  // Fill in the system info that is known at this point
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Collecting Early System Information - START *******"));
  FillEarlySystemInfo (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Collecting Early System Information - END   *******"));

  //
  // Setup MinPath from SBSP to all sockets and collect the LEP information
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Setting up Minimum Path - START *******"));
  SetupSbspPathToAllSockets (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Setting up Minimum Path - END   *******"));
  
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Initialize MCTP - START *******"));
  InitializeMCTP (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Initialize MCTP - END   *******"));

  //
  // Check if the sysetm has valid QPI topology. Otherwise degrade it to supported topology
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Check for QPI Topology Degradation - START *******"));
  PreProcessQpiLinks (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Check for QPI Topology Degradation - END *******"));

  //
  // Update QPIRC output structure and verify host if the input values comply with system topology; otherwise force them to default value
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Checking QPIRC Input Structure - START *******"));
  PrimeHostStructure (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Checking QPIRC Input Structure - END   *******"));

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Allocate RTIDs - START *******"));
  AllocateRtids (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Allocate RTIDs - END *******"));

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Cacluate Resource Allocation - START *******"));
  AllocateIioResources(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Cacluate Resource Allocation - END   *******"));

  //
  // If we are not in Cold Reset path, make sure the system topology is same as the one before reset
  //
  if (QpiInternalGlobal.CurrentReset != POST_RESET_POWERGOOD) {
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Check for QPI Topology change across reset - START *******"));
    CheckForTopologyChange (host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Check for QPI Topology change across reset - END *******"));
  }


  if (QpiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD || QpiInternalGlobal.ForceColdResetFlow == TRUE) {
    //
    // Program the RTIDs and other credits that require reset to take effect
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Programming RTIDs and other Credits - START *******"));
    ProgramRtids (host, &SocketData, &QpiInternalGlobal);
    QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_RING_CRDT, host);
    ProgramRingCredits (host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Programming RTIDs and other Credits - END   *******"));

    //
    // Sync up the PBSPs
    //
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Sync Up PBSPs - START *******"));
    SyncUpPbspForReset(host, &SocketData, &QpiInternalGlobal, POST_RESET_POWERGOOD);
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Sync Up PBSPs - END   *******"));

    //
    // Program MSR for w/a after all PBSPs are ready for subsequent warm reset
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Programming MSR for w/a - START *******"));
    ProgramMsrForWa (host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Programming MSR for w/a - END   *******"));
  }

  if (QpiInternalGlobal.CurrentReset == POST_RESET_POWERGOOD) {
    //
    // Program BGF Pointer Seperation Overrides
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Programming BGF Overrides - START *******"));
    QpiProgramBGFOverrides(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Programming BGF Overrides - END *******"));
    
    //
    // Transition the links to full speed operation.
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Full Speed Transition - START *******"));
    QpiTransitionFullSpeed(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Full Speed Transition - END *******"));

    //
    // PCU Misc Configuration through pcode mail box command
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Cod Activate - START *******"));
    QpiPcuMiscConfig(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Cod Activate - END *******"));

  } else {    
    //
    // Phy/LL work on warm reset
    //
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Phy/Link Updates On Warm Reset - START *******"));  
    QpiPhyLinkAfterWarmReset(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Phy/Link Updates On Warm Reset - END *******"));
  }

  // Sync up the PBSPs in warm reset flow
  if (QpiInternalGlobal.CurrentReset != POST_RESET_POWERGOOD && QpiInternalGlobal.ForceColdResetFlow != TRUE) {
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Sync Up PBSPs - START *******"));
    SyncUpPbspForReset(host, &SocketData, &QpiInternalGlobal, POST_RESET_WARM);
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Sync Up PBSPs - END   *******"));
  }

  if (host->var.common.resetRequired) {
    goto ExitQpiMain;
  }

  // HSX W/A (4169009 - All Steppings): HSX Clone: <ES2 Gate>DRT: pkg_cstate fails to enter deep cstate with B0 patch 0xc
  // Note: MUST be applied after each and EVERY warm reset.
  // Note: MUST be performed AFTER all the sockets have syncronized themselves.
  if (host->var.common.cpuType == CPU_HSX || host->var.common.cpuType == CPU_BDX) {
    QpiPhyLinkL1WaHsx(host, &SocketData, &QpiInternalGlobal);
  }

  // Grantley BDX HSD : 5002247 BDX EP: Changes to CTLEP Values for QPI and
  // Brickland BDX HSD : 4971066 BDX EX: QPI CTLE Stage 1 Setting Improvement
  // Note: Must be applied after each and every warm reset.
  // Note: Must be performed after all the sockets have synchronized themselves.
  if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) {
    QpiPhyLBC46ClearOffWA(host, &SocketData, &QpiInternalGlobal);
  }

  //
  // Program Route Back Table
  //
  if ((QpiInternalGlobal.BtModeEn == TRUE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) {
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Route Back Table - START *******"));
    SetupRbt(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program Route Back Table - END   *******"));
  }

  //
  // Do the topology discovery and optimum route calculation
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Topology Dicovery and Optimum Route Calculation - START *******"));
  QpiTopologyDiscovery (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Topology Dicovery and Optimum Route Calculation - END   *******"));

  //
  // Program the route table computed in the above step for each socket. Unlike the minimum path that we set before,
  // this is the complete, optimum route setting for the topology that is discovered dynamically.
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Optimum Route Table Settings - START *******"));
  ProgramSystemRoute (host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;******* Program Optimum Route Table Settings - END   *******"));

  //
  // Setup the final IO SAD entries for all CPU sockets
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Final IO SAD Setting - START *******"));
  SetupSystemIoSadEntries(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program Final IO SAD Setting - END   *******"));



  //
  // Setup QPI Protocol Layer miscellaneous registers
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Misc. QPI Parameters - START *******"));
  SetupQpiMisc(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program Misc. QPI Parameters - END   *******"));


  //
  // Program the HA credit registers
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Home Agent Credits - START *******"));
  SetupHaCredits(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program Home Agent Credits - END   *******"));

  //
  // Program Home Tracker and Route Back Table
  //
  if (QpiInternalGlobal.BtModeEn != TRUE) {
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program Home tracker and Route Back Table - START *******"));
    SetupHtBase(host, &SocketData, &QpiInternalGlobal);
    QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program Home tracker and Route Back Table - END   *******"));
  }

  //
  // Program System Coherency registers
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Program System Coherency Registers - START *******"));
  SetupSystemCoherency(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Program System Coherency Registers - END   *******"));

  //
  // S3 resume check to make sure the system configuration didn't change across S3
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Check for S3 Resume - START *******"));
  CheckS3ResumePath(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Check for S3 Resume - END   *******"));

  //
  // Collect the errors that might have occurred in previous boot and clear the error registers
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n\n\n;******* Collect Previous Boot Error - START *******"));
  CollectAndClearErrors(host, &SocketData, &QpiInternalGlobal);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* Collect Previous Boot Error - END   *******"));

ExitQpiMain:

  //
  // Dump output structure
  //
  DataDumpQpiRcExit (host, &QpiInternalGlobal);
                      
  QpiCheckPoint (0xFF, 0xFF, 0xFF, STS_QPI_COMPLETE, 0, host);
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;******* QPIRC Exit  *******\n\n"));
  return QPI_SUCCESS;
}

STATIC
UINT8
GetDefaultPointerSeperationValues (
  struct sysHost      *host
  )
{
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < B0_REV_HSX)) {
    return PTR_SEP_VAL_HSX_A0;
  } else {
    return PTR_SEP_VAL_HSX_BDX;
  }
}

VOID  
DataDumpQpiRcEntry (
    struct sysHost          *host
    )
{
  UINT8 soc;          //Target socket # (loop index)       
  UINT8 port;         //Target port #   (loop ind)

  //
  // Dump QPIRC input structure
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n\n******* QPI Setup Structure *******"));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nPPINrOptIn: %u", host->setup.qpi.PPINrOptIn));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nBus   Ratio: %u %u %u %u", host->setup.qpi.BusRatio[0], host->setup.qpi.BusRatio[1], host->setup.qpi.BusRatio[2], host->setup.qpi.BusRatio[3]));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nIO    Ratio: %u %u %u %u", host->setup.qpi.IoRatio[0], host->setup.qpi.IoRatio[1], host->setup.qpi.IoRatio[2], host->setup.qpi.IoRatio[3]));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nMMIOL Ratio: %u %u %u %u", host->setup.qpi.MmiolRatio[0], host->setup.qpi.MmiolRatio[1], host->setup.qpi.MmiolRatio[2], host->setup.qpi.MmiolRatio[3]));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nLegacyVgaSoc: %u", host->setup.qpi.LegacyVgaSoc));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nMmioP2pDis: %u", host->setup.qpi.MmioP2pDis));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nIsocAzaliaVc1En: %u", host->setup.qpi.IsocAzaliaVc1En));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nDebugPrintLevel: %u", host->setup.qpi.DebugPrintLevel));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nClusterOnDieEn: %u", host->setup.qpi.ClusterOnDieEn));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nIBPECIEn: %u", host->setup.qpi.IBPECIEn)); 
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nE2EParityEn: %u", host->setup.qpi.E2EParityEn));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nEarlySnoopEn: %u", host->setup.qpi.EarlySnoopEn));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nHomeDirWOSBEn: %u", host->setup.qpi.HomeDirWOSBEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nDegradePrecedence: %u", host->setup.qpi.DegradePrecedence));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkSpeedMode: %u (%s)", host->setup.qpi.QpiLinkSpeedMode, ((host->setup.qpi.QpiLinkSpeedMode == 0) ? "SLOW" : "FAST")));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkSpeed: %u", host->setup.qpi.QpiLinkSpeed));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkL0pEn: %u", host->setup.qpi.QpiLinkL0pEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkL1En: %u", host->setup.qpi.QpiLinkL1En));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkL0rEn: %u", host->setup.qpi.QpiLinkL0rEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLbEn: %u", host->setup.qpi.QpiLbEn));

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nIioUniphyDisable (per socket): "));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  %u", host->setup.qpi.IioUniphyDisable[soc]));
  }

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkCreditReduce: %u", host->setup.qpi.QpiLinkCreditReduce));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiConfigTxWci: %u", host->setup.qpi.QpiConfigTxWci));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiCrcMode: %u", host->setup.qpi.QpiCrcMode));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiCpuSktHotPlugEn: %u", host->setup.qpi.QpiCpuSktHotPlugEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiCpuSktHotPlugTopology: %u", host->setup.qpi.QpiCpuSktHotPlugTopology));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiSkuMismatchCheck: %u", host->setup.qpi.QpiSkuMismatchCheck));

  // Phy/Link Layer Options (per Port)
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiPortDisable (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Link[port].QpiPortDisable));
    }
  }

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkCreditReduce (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Link[port].QpiLinkCreditReduce));
    }
  }
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiLinkSpeed (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Phy[port].QpiLinkSpeed));
    }
  }
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiProbeType (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Phy[port].QpiProbeType));
    }
  }
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiConfigTxWci (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Phy[port].QpiConfigTxWci));
    }
  }
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nRsvd (per port):"));
  for (soc=0; soc < MAX_CPU_SOCKETS; soc++) {
      QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "  S%1u:", soc));
      for (port=0; port < MAX_QPI_PORTS; port++) {       
        QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "%1u ", host->setup.qpi.PhyLinkPerPortSetting[soc].Phy[port].Rsvd));
    }
  }


  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n\n******* Common Setup Structure *******"));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmCfgBase: 0x%08X ", host->setup.common.mmCfgBase));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmCfgSize: 0x%08X", host->setup.common.mmCfgSize));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmiolBase: 0x%08X ", host->setup.common.mmiolBase));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmiolSize: 0x%08X ", host->setup.common.mmiolSize));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmiohBase: 0x%08X-00000000", host->setup.common.mmiohBase));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmiohSize: %u GB ", host->setup.common.mmiohSize));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nnumaEn: %u ", host->setup.common.numaEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nisocEn: %u ", host->setup.common.isocEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmesegEn: %u ", host->setup.common.mesegEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\ndcaEn: %u ", host->setup.common.dcaEn));

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n\n******* Common Var Structure *******"));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nresetRequired: %u ", host->var.common.resetRequired));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nstate: %u ", host->var.common.bootMode));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nnumCpus: %u ", host->var.common.numCpus));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nsocketPresentBitMap: 0x%02X ", host->var.common.socketPresentBitMap));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nbusIio: 0x%02X 0x%02X 0x%02X 0x%02X", host->var.common.busIio[0], host->var.common.busIio[1], host->var.common.busIio[2], host->var.common.busIio[3]));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nbusUncore: 0x%02X 0x%02X 0x%02X 0x%02X ", host->var.common.busUncore[0], host->var.common.busUncore[1], host->var.common.busUncore[2], host->var.common.busUncore[3]));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nmmCfgBase: 0x%08X ", host->var.common.mmCfgBase));

  return;
}


VOID  
DataDumpQpiRcExit (
    struct sysHost          *host,
    QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
    )
{

  //
  // Dump QPI output structure
  //
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n\n******* QPI Output Structure *******"));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutLegacyVgaSoc: %u", host->var.qpi.OutLegacyVgaSoc));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutIsocEn: %u", host->var.qpi.OutIsocEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutMesegEn: %u", host->var.qpi.OutMesegEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutIsocAzaliaVc1En: %u", host->var.qpi.OutIsocAzaliaVc1En));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutClusterOnDieEn: %u", host->var.qpi.OutClusterOnDieEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutIBPECIEn: %u", host->var.qpi.OutIBPECIEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutE2EParityEn: %u", host->var.qpi.OutE2EParityEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutEarlySnoopEn: %u", host->var.qpi.OutEarlySnoopEn));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutHomeDirWOSBEn: %u", host->var.qpi.OutHomeDirWOSBEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiCurrentLinkSpeedMode: %u (%s)", host->var.qpi.QpiCurrentLinkSpeedMode, ((host->var.qpi.QpiCurrentLinkSpeedMode == 0) ? "SLOW" : "FAST")));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutQpiLinkSpeed: %u", host->var.qpi.OutQpiLinkSpeed));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutQpiLinkL0pEn: %u", host->var.qpi.OutQpiLinkL0pEn));   
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutQpiLinkL1En: %u", host->var.qpi.OutQpiLinkL1En)); 
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutQpiLinkL0rEn: %u", host->var.qpi.OutQpiLinkL0rEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutIioUniphyDisable: %u, %u, %u, %u", host->var.qpi.OutIioUniphyDisable[0], host->var.qpi.OutIioUniphyDisable[1], host->var.qpi.OutIioUniphyDisable[2], host->var.qpi.OutIioUniphyDisable[3]));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutQpiCrcMode: %u", host->var.qpi.OutQpiCrcMode));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutClusterOnDieReduction: %u", host->var.qpi.OutClusterOnDieReduction));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nOutPointerSeperationHA: %u", host->var.qpi.OutPointerSeperationHA));  
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n"));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->BtModeEn: %u", QpiInternalGlobal->BtModeEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->BtMode: %u", QpiInternalGlobal->BtMode));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->BtMode2Alt: %u", QpiInternalGlobal->BtMode2Alt));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->Sys4SClusterOnDieEn: %u", QpiInternalGlobal->Sys4SClusterOnDieEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->SnoopFanoutEn: %u", QpiInternalGlobal->SnoopFanoutEn));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->SysSnoopMode: %u", QpiInternalGlobal->SysSnoopMode));
  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\nQpiInternalGlobal->IodcEn: %u", QpiInternalGlobal->IodcEn));
  return;
}

STATIC 
VOID
UpdatePhysicalChopInfoInQpiInternal (
  struct  sysHost           *host,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  UINT8                     SockId
  )
{
  CAPID4_PCU_FUN3_HSX_STRUCT  capid4_hsx;
  CAPID4_PCU_FUN3_BDX_STRUCT  capid4_bdx;

  if (host->var.common.cpuType == CPU_HSX) {
    capid4_hsx.Data = QpiReadPciCfg (host, SockId, 0, CAPID4_PCU_FUN3_REG);
    QpiInternalGlobal->ChopType[SockId] = (UINT8)capid4_hsx.Bits.physical_chop;
  } else { // cpuType is CPU_BDX
    capid4_bdx.Data = QpiReadPciCfg (host, SockId, 0, CAPID4_PCU_FUN3_REG);
    if ((capid4_bdx.Bits.physical_chop & 0x4) == 0) { // CAPID4[8] is cleared
      QpiInternalGlobal->ChopType[SockId] = (UINT8)capid4_bdx.Bits.physical_chop;
    } else {
      // Use 3 as base and sum with [7:6]. 3 is LCCDE and 4 is HCCDE
      QpiInternalGlobal->ChopType[SockId] = 3 + ((UINT8)capid4_bdx.Bits.physical_chop & 0x3);
    }
  } // cpuType check
}


QPI_STATUS
FillEarlySystemInfo (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:
  Routine to collect information that is available at this stage of booting and needed to initialize QPI.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  0 - Successful
  Other - failure

--*/
{
  UINT32  Data32, BusBase, BusSize;
  UINT8   sbspSktId, Idx1, Port;
  UINT32  CpuList = 0;
  QPIPCSTS_QPIAGENT_LL_STRUCT     PcSts;
  QPIMISCSTAT_QPIAGENT_LL_STRUCT  QpiMiscSts;

  QpiCheckPoint (0xFF, 0xFF, 0xFF, STS_COLLECT_EARLY_SYSTEM_INFO, 0, host);


  // SBSP socket id & Stepping
  sbspSktId = QpiInternalGlobal->SbspSoc = GetSbspSktIdFromLocal(host);

  UpdateCpuInfoInQpiInternal(host, QpiInternalGlobal, sbspSktId);

  // Update the SkuType in host structure to be later used for Hob data
  host->var.common.skuType = QpiInternalGlobal->SkuType[sbspSktId];  
  
  // For BDX-DE SKU, force the number of TotR3Qpi and TotQpiAgent to zero
  if ((host->var.common.cpuType == CPU_BDX) && \
      (QpiInternalGlobal->SkuType[sbspSktId] == SKU_DE || QpiInternalGlobal->SkuType[sbspSktId] == SKU_NS)) {
    QpiInternalGlobal->TotR3Qpi[sbspSktId] = 0;
    QpiInternalGlobal->TotQpiAgent[sbspSktId] = 0;
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  SBSP Socket: %u   SKU: 0x%02X   SubSKU: 0x%02X   Stepping: 0x%02X   CAPID4[sbsp]: 0x%08X",
                       sbspSktId, QpiInternalGlobal->SkuType[sbspSktId], QpiInternalGlobal->SubSkuType[sbspSktId],
                       host->var.common.stepping, host->var.common.procCom[sbspSktId].capid4));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Total Cbos: %02u   Cbo List: 0x%02X   Total HA: %02u   Total R3Qpi: %02u   Total QpiAgent: %02u\n",
                       QpiInternalGlobal->TotCbo[sbspSktId], QpiInternalGlobal->CboList[sbspSktId],
                       QpiInternalGlobal->TotHa[sbspSktId], QpiInternalGlobal->TotR3Qpi[sbspSktId], QpiInternalGlobal->TotQpiAgent[sbspSktId]));

  //
  // Extract the current system reset type from Sticky Scratch 7 (updated by CPURC)
  //
  Data32 = QpiReadPciCfg (host, sbspSktId, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
  if ((Data32 >> 5) & 0x1) {
    //
    // Indicates the successful completion of Cold Reset flow.
    //
    QpiInternalGlobal->CurrentReset = POST_RESET_WARM;
  } else {
    QpiInternalGlobal->CurrentReset = POST_RESET_POWERGOOD;
  }

  //
  // Fill the host common structure with the Unocre & IIO bus numbers assigned to each 
  // populated socket so that the CSR access routines become usable for all sockets. 
  // Note that the bus number for SBSP is filled by CPURC; so the CSR access routines can be
  // used for SBSP CSR space before we do this step.
  //
  if (QpiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) {
    // 
    // In PwrGd reset flow, assume maximal configuration and initialize the output structure accordingly
    // If S0 is not the legacy socket, then assign the bus number such that Legacy Socket (i.e SBSP) gets bus range starting from 0
    // 
    host->var.common.numCpus = MAX_CPU_SOCKETS;
    host->var.common.socketPresentBitMap = FLD_MASK(MAX_CPU_SOCKETS, 0);
    BusBase = 0;
    BusSize = 256 / MAX_CPU_SOCKETS;
    host->var.common.busIio[QpiInternalGlobal->SbspSoc] = (UINT8)BusBase;
    BusBase = BusBase + BusSize;
    host->var.common.busUncore[QpiInternalGlobal->SbspSoc] = (UINT8)(BusBase - 1);
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if (Idx1 != QpiInternalGlobal->SbspSoc) {
        host->var.common.busIio[Idx1] = (UINT8)BusBase;
        BusBase = BusBase + BusSize;
        host->var.common.busUncore[Idx1] = (UINT8)(BusBase - 1);
      } 
    } 
  } else { // In other reset flows, use the Sticky SR07 register value that QPIRC updated in previous boot
    Data32 = QpiReadPciCfg (host, sbspSktId, 0, BIOSSCRATCHPAD2_UBOX_MISC_REG);
    CpuList = (UINT32)(Data32 & 0xFF);

    host->var.common.numCpus = 1; // mark SBSP's presence
    host->var.common.socketPresentBitMap = (UINT8) CpuList & FLD_MASK(MAX_CPU_SOCKETS, 0);
    if( QpiInternalGlobal->QpiCpuSktHotPlugEn == FALSE){   
      Data32 = QpiReadPciCfg (host, sbspSktId, 0,  BIOSSCRATCHPAD3_UBOX_MISC_REG);
      BusBase = 0;
      BusSize = (((UINT8)(Data32 >> (QpiInternalGlobal->SbspSoc * 3)) & 0x7) + 1) * 32;
      host->var.common.busIio[QpiInternalGlobal->SbspSoc] = (UINT8)BusBase;
      BusBase = BusBase + BusSize;
      host->var.common.busUncore[QpiInternalGlobal->SbspSoc] = (UINT8)(BusBase - 1);
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if ((CpuList & (1 << Idx1)) && (Idx1 != QpiInternalGlobal->SbspSoc)) {
          host->var.common.numCpus++;
          BusSize = (((UINT8)(Data32 >> (Idx1 * 3)) & 0x7) + 1) * 32;
          host->var.common.busIio[Idx1] = (UINT8)BusBase;
          BusBase = BusBase + BusSize;
          host->var.common.busUncore[Idx1] = (UINT8)(BusBase - 1);
        } 
      }
    } else {   // if hotplug enable, force to hot-plug pre-allocate bus flow during warm reset
      BusBase = 0;
      BusSize = 256 / MAX_CPU_SOCKETS;
      host->var.common.busIio[QpiInternalGlobal->SbspSoc] = (UINT8)BusBase;
      BusBase = BusBase + BusSize;
      host->var.common.busUncore[QpiInternalGlobal->SbspSoc] = (UINT8)(BusBase - 1);
      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if ((CpuList & (1 << Idx1)) && (Idx1 != QpiInternalGlobal->SbspSoc)) {
          host->var.common.numCpus++;
          host->var.common.busIio[Idx1] = (UINT8) (Idx1 * (UINT8)BusSize);
          BusBase = host->var.common.busIio[Idx1] + BusSize;
          host->var.common.busUncore[Idx1] = (UINT8)(BusBase - 1);
        } 
      }     
    }
    Data32  = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0,  BIOSSCRATCHPAD0_UBOX_MISC_REG);
    QpiInternalGlobal->ExtRtidMode = (BOOLEAN) ((Data32  & 0x80) >> 7);
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  TotCpus: %u  CpuList: 0x%02X ", host->var.common.numCpus, host->var.common.socketPresentBitMap));
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  busIio:    "));
   for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if ((CpuList & (1 << Idx1)) || (QpiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD)) {   // if warm reset, only display the valid socket and cold reset, display all sockets  
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "0x%02X ", host->var.common.busIio[Idx1]));
   } 
  } 

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  busUncore: "));
   for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if ((CpuList & (1 << Idx1)) || (QpiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD)) {   // if warm reset, only display the valid socket and cold reset, display all sockets
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "0x%02X ", host->var.common.busUncore[Idx1]));
   }
  }
  
  //
  // Make sure MMIOL Base/Size is naturally aligned at 64MB, MMIOL Base is >= 256M,
  // and MMIOL base + size <= MMIOL_LIMIT+1.
  //
  QPI_ASSERT((host->setup.common.mmiolBase % 0x4000000 == 0 && host->setup.common.mmiolSize % 0x4000000 == 0), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT((host->setup.common.mmiolBase >= 0x10000000 && host->setup.common.mmiolBase <= MMIOL_LIMIT), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  Data32 = MMIOL_LIMIT - host->setup.common.mmiolBase + 1;
  QPI_ASSERT((host->setup.common.mmiolSize <= Data32), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  //
  // Validate MMCFG base and size:
  //  - MMCFG Base: MUST be 256M aligned (HSD-3615509).  MMCFG Base is >= 256M, and MUST NOT be set to address 4G-64M.
  //  - MMCFG base + size <= MMIOL_LIMIT
  //  - MMCFG Size is 64M, 128M or 256M.
  //
  QPI_ASSERT((host->setup.common.mmCfgBase % 0x10000000 == 0 &&
                    host->setup.common.mmCfgBase >= 0x10000000 && host->setup.common.mmCfgBase <= MMIOL_LIMIT), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  QPI_ASSERT((host->setup.common.mmCfgSize == 0x4000000 ||
                    host->setup.common.mmCfgSize == 0x8000000 || host->setup.common.mmCfgSize == 0x10000000), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

         
  Data32 = MMIOL_LIMIT - host->setup.common.mmCfgBase + 1;
  QPI_ASSERT((host->setup.common.mmCfgSize <= Data32), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

  //
  // Make sure MMCFG & MMIOL regions are not overlapping
  //
  QPI_ASSERT ((host->setup.common.mmCfgBase != host->setup.common.mmiolBase), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  if (host->setup.common.mmCfgBase < host->setup.common.mmiolBase) {
    Data32 = host->setup.common.mmiolBase - host->setup.common.mmCfgBase;
    QPI_ASSERT((Data32 >= host->setup.common.mmCfgSize), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  } else {
    Data32 = host->setup.common.mmCfgBase - host->setup.common.mmiolBase;
    QPI_ASSERT((Data32 >= host->setup.common.mmiolSize), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
  }

  //
  // Logic to detect the system QPI link speed mode (slow or fast). Note that all the valid links
  // must be in same speed mode. i.e some can't be in full speed mode while the rest are in slow mode.
  // Find a valid link of SBSP to read the QpiMisc and PhCtr registers. 
  // Linkspeed mode info while for HSX/IVT PhCtr provides this info.
  //
  QpiMiscSts.Data = 0;
  host->var.qpi.QpiCurrentLinkSpeedMode = QPI_LINK_SLOW_SPEED_MODE;
  for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
    Data32 = QpiReadPciCfg (host, sbspSktId, Port, VID_QPIAGENT_LL_REG);
    if ((Data32 & 0xFFFF) == 0xFFFF) {
      continue;
    }
    PcSts.Data = QpiReadPciCfg (host, sbspSktId, Port, QPIPCSTS_QPIAGENT_LL_REG);
    if (PcSts.Bits.ll_status == LINK_SM_NORMAL_OPERATION) {
      QpiMiscSts.Data = QpiReadPciCfg (host, sbspSktId, (UINT8)((Port/2)*2), QPIMISCSTAT_QPIAGENT_LL_REG); // Only ports 0 & 2 have this register
      Data32 = QpiReadPciCfg (host, sbspSktId, Port, QPIREUT_PH_PIS_QPIAGENT_REUT_REG);
      host->var.qpi.QpiCurrentLinkSpeedMode = (UINT8)(Data32 >> 26) & 0x1; //  "initializespeed" field
      break;
    }
  }
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reset Type: %s   Link Speed: %s", (QpiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) ? "Cold Reset" : "Warm Reset", 
     ((host->var.qpi.QpiCurrentLinkSpeedMode == QPI_LINK_SLOW_SPEED_MODE) ? "Slow Speed" : "Fast Speed")));

  QpiInternalGlobal->QpiRunAdaptation = FALSE;

  //
  // Get COD status during warm reset if enabled in setup
  //
  if ((QpiInternalGlobal->CurrentReset == POST_RESET_WARM) && (host->setup.qpi.ClusterOnDieEn == 1)) {
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;"));
    SendMailBoxCmdToPcode(host, 0, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
    Data32 = QpiReadPciCfg (host, 0, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);  
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG returned 0x%08x", Data32));
    QpiInternalGlobal->ClusterOnDieActive = (Data32 & BIT31) >> 31;
  }

  return QPI_SUCCESS;
}

QPI_STATUS
SetupSbspPathToAllSockets (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal 
  )
/*++

Routine Description:
  Routine to set minimum required route to allow config accesses coming from one of SBSP's core
  to be routed to the agent that claims the region. It also constructs the SBSP minimum tree
  and collects information such as CPU sockets populated and their LEP info to facilitate
  the topology discovery process to be done without any config access. The route set by this
  routine may not be the optimum path. Note that, the route set by this routine allows only the 
  SBSP be able to access the config space of other CPU sockets in the system; not from
  one PBSP to other PBSPs.
  
  It also sets up the route to the Legacy CPU so that >1 hop away CPUs can be released from reset
  and brought under BIOS control.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

  0 - Successful
  Other - failure

--*/
{
  QPI_STATUS          Status;
  TOPOLOGY_TREE_NODE  GrandParent;
  TOPOLOGY_TREE_NODE  Parent;
  TOPOLOGY_TREE_NODE  Child;
  UINT8               Index;
  UINT32              Ctr;
  UINT8               QpiLink;
  UINT8               Port;
  UINT32              Data32, WaitTime;
  BOOLEAN             CheckInFlag, FeatureMismatch;
  UINT8               sbspSktId, LegacyPchTargetForAp;

  sbspSktId = QpiInternalGlobal->SbspSoc;
  QpiCheckPoint (sbspSktId, SOCKET_TYPE_CPU, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_ADD_SOCKET_TO_MIN_PATH_TREE, host);

  //
  //  Add the Sbsp to minimum path tree
  //
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; Constructing SBSP minimum path Topology Tree "));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; -------------------------------------------- "));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; Adding SBSP (CPU%u) to the tree", sbspSktId));

  Parent.RawNode          = 0;
  Parent.Node.Valid       = TRUE;
  Parent.Node.SocId       = sbspSktId;
  Parent.Node.SocType     = SOCKET_TYPE_CPU;
  Parent.Node.ParentPort  = 0xF;    //Init 4-bit field to its max value (meaning 'invalid')
  Parent.Node.Hop         = 0;
  Parent.Node.ParentIndex = 0x7F;   //Init 7-bit field to its max value (meaning 'invalid')

  if ((Status = AddNodeToTopologyTree (host, SocketData->SbspMinPathTree, &Parent)) != QPI_SUCCESS) {
    QpiCheckPoint ((UINT8)(1 << Parent.Node.SocId), SOCKET_TYPE_CPU, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Adding CPU%u to Minimum Path Tree failed - Data Structure Overflow.", Parent.Node.SocId));
    QPI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
  }
  //
  // Add the Sbsp to the socketdata
  //
  SocketData->Cpu[sbspSktId].Valid  = TRUE;
  SocketData->Cpu[sbspSktId].SocId  = sbspSktId;
  
  //
  // Discover the neighbors of Sbsp and setup path to neighbor's R2PCIe/Ubox. Repeat it until no
  // new sockets are found. Also, setup the boot path if a CPU is >1 hop away from Legacy CPU.
  //
  for (Index = 0; SocketData->SbspMinPathTree[Index].Node.Valid == TRUE; ++Index) {

    Parent = SocketData->SbspMinPathTree[Index];
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      
      //
      // Make sure the pCode has completed initializing the socket before making any CSR access in that socket.
      // Limit delay to 5s.
      // 
      CheckInFlag = FALSE;
      WaitTime = 5000;
      for (Ctr = 0; Ctr < WaitTime; ++Ctr) { 
        Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId, 0, BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG);
        if ((Data32 & 0xFF00) == 0x0200) { // Bits [15:8] should be 0x2 (LLC BIST complete) before the socket can be accessed.
          CheckInFlag = TRUE;
          break;        
        }
        QpiFixedDelay(host, 1000); // 1000 microsecs   
      }
      
      if (CheckInFlag == FALSE) { 
          // Halt the System
          QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE,  host);
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\npCode has not completed initilization of AP Socket %u. System Halted!!!", Parent.Node.SocId));
          QPI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
      }
      
      //
      // Make sure each discovered socket has Cbo Count/List, HA, R3QPI. Socket Sku Type that matches that of SBSP's.
      //
      if (Parent.Node.SocId != sbspSktId) {

        UpdateCpuInfoInQpiInternal(host, QpiInternalGlobal, (UINT8) Parent.Node.SocId);
        GetLegacyPchTargetSktId(host, (UINT8) Parent.Node.SocId, &LegacyPchTargetForAp);
        FeatureMismatch = CheckThisSocketInfoWithSbsp(host, QpiInternalGlobal, sbspSktId, (UINT8) Parent.Node.SocId);
        
        if (sbspSktId != LegacyPchTargetForAp) {
          QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_LEGACY_PCH_MISMATCH, host);
          FeatureMismatch = TRUE;
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Legacy PCH target NID mismatch between SBSP (CPU%u) and CPU%u", sbspSktId, Parent.Node.SocId));
        }
          
        if (FeatureMismatch == TRUE) {
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n System Configuration not supported!!! "));
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }
      //
      // Parse the LEP of the discovered socket.
      //
      QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_COLLECT_LEP, host);
      ParseCpuLep (host, SocketData, QpiInternalGlobal, (UINT8) Parent.Node.SocId);
    } else {
      QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    }

    //
    // Make sure the parent has checked-in. NonSticky Scratch Register 02 in SBSP, is used by each AP sockets to indicate
    // that they have successfully checked-in. If it hasn't checked in yet, handle error.
    //
    if (Parent.Node.SocId != sbspSktId) { // SBSP has checked in, otherwise we won't be here
      QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_CHK_PBSP_CHKIN, host);

      //
      // The AP that is directly connected to Legacy CPU must have checked-in.
      //
      if (CheckCpuConnectedToLegCpu (host, SocketData, QpiInternalGlobal, (UINT8) Parent.Node.SocId) == TRUE) {
        CheckInFlag = FALSE;
        OemWaitTimeForPSBP(host, &WaitTime); 
        
        // COD failure recovery
        if (QpiInternalGlobal->ClusterOnDieActive) {
          for (Ctr = 0; Ctr < MAX_QPI_PORTS; Ctr++) {
            if (SocketData->Cpu[Parent.Node.SocId].LinkData[Ctr].Valid == FALSE) {
              QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n Warning: A possible link failure in COD mode condition detected."));
              CheckInFlag = TRUE;
              QpiInternalGlobal->SkippedCheckedInCheck |= (1 << (Parent.Node.SocId));
              host->var.qpi.OutClusterOnDieReduction = 1;
              break;
            }
          }
        }
        
        for (Ctr = 0; Ctr < WaitTime; ++Ctr) { 
          Data32 = QpiReadPciCfg (host, (UINT8)Parent.Node.SocId, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
          if ((Data32 & 0x1) != 0) {            //check bit 0 to indicate AP check in 
            CheckInFlag = TRUE;
            break;
          }
          QpiFixedDelay(host, 1000); // 1000 microsecs 
        }
        if (CheckInFlag == FALSE) {
          QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE,  host);
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n\n AP Socket %u is discovered but hasn't come out of reset. System Halted!!!", Parent.Node.SocId));
          QPI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
        }
      } else {
        //
        // Setup the boot path for the parent which is not directly connected to Legacy CPU.
        //
        QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_SET_PBSP_BOOT_PATH, host);
        GetParentSocket (host, SocketData->SbspMinPathTree, &Parent, &GrandParent);

        Port = GetChildLocalPortFromLEP (host, SocketData->Cpu, &GrandParent, &Parent);
        if (Port == 0xFF) {
          QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INVALID_PORT_CONNECTION, host);
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u. ", GrandParent.Node.SocId, Parent.Node.SocId));
          QPI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INVALID_PORT_CONNECTION);
        }

        SetupRemoteCpuBootPath (host, SocketData, QpiInternalGlobal, &GrandParent, &Parent, Port);
        
        // COD failure recovery
        if (QpiInternalGlobal->ClusterOnDieActive) {
          for (Ctr = 0; Ctr < MAX_QPI_PORTS; Ctr++) {
            if (SocketData->Cpu[Parent.Node.SocId].LinkData[Ctr].Valid == FALSE) {
              QpiDebugPrintWarn ((host, QPI_DEBUG_WARN, "\n Warning: A possible link failure in COD mode condition detected."));
              host->var.qpi.OutClusterOnDieReduction = 1;
              break;
            }
          }
        }
      }
    }


    Child.RawNode     = 0;
    Child.Node.Valid  = TRUE;
    Child.Node.Hop    = Parent.Node.Hop + 1; // One level down from parent
    Child.Node.ParentIndex = Index; // Remember who is the immediate parent of this child
    for (QpiLink = 0; QpiLink < MAX_QPI_PORTS; ++QpiLink) {
      if (SocketData->Cpu[Parent.Node.SocId].LinkData[QpiLink].Valid == TRUE) {
        //
        // Get the Child Socket Id, Type and port that connects it to the parent
        //
        Child.Node.SocId      = SocketData->Cpu[Parent.Node.SocId].LinkData[QpiLink].PeerSocId;
        Child.Node.SocType    = SocketData->Cpu[Parent.Node.SocId].LinkData[QpiLink].PeerSocType;
        Child.Node.ParentPort = SocketData->Cpu[Parent.Node.SocId].LinkData[QpiLink].PeerPort;

        //
        // Make sure that this socket is not already found in the tree.
        //
        if (NodeFoundInMinPathTree (host, SocketData->SbspMinPathTree, &Child) == TRUE) {
          continue;
        }

        //
        // Add the node to the tree.
        //
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; Adding CPU%u to the tree", Child.Node.SocId));
        QpiCheckPoint ((UINT8) (1 << Child.Node.SocId), (UINT8) Child.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_ADD_SOCKET_TO_MIN_PATH_TREE, host);
        if ((Status = AddNodeToTopologyTree (host, SocketData->SbspMinPathTree, &Child)) != QPI_SUCCESS) {
          QpiCheckPoint ((UINT8) (1 << Child.Node.SocId), (UINT8) Child.Node.SocType, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE, host);
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n; Adding CPU%u to Minimum Path Tree failed - Data Structure Overflow.", Child.Node.SocId));
          QPI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
        }
        //
        // Add the node to the socketdata
        //
        if (Child.Node.SocType == SOCKET_TYPE_CPU) {
          SocketData->Cpu[Child.Node.SocId].Valid = TRUE;
          SocketData->Cpu[Child.Node.SocId].SocId = (UINT8) Child.Node.SocId;
        } else {
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
        //
        // Setup path from Sbsp to the new found node so that the Sbsp can access the new found node's CSRs.
        //
        QpiCheckPoint ((UINT8) (1 << Child.Node.SocId), (UINT8) Child.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_SET_SBSP_CONFIG_PATH, host);
        SetupSbspConfigAccessPath (host, SocketData, QpiInternalGlobal, &Parent, &Child, QpiLink);
      }
    } // for(QpiLink...)
  } // for(Index...)

  //
  // Print the Minimum Path Tree
  //
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;SBSP Minimum Path Tree"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;----------------------"));
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;Index  Socket  ParentPort  Hop  ParentIndex"));
  for (Index = 0; SocketData->SbspMinPathTree[Index].Node.Valid != FALSE; ++Index) {
    if (SocketData->SbspMinPathTree[Index].Node.SocId == sbspSktId) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %02u     CPU%u    --         %1u     --", Index, 
        SocketData->SbspMinPathTree[Index].Node.SocId, 0));
    } else {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %02u     CPU%u    %02u         %1u     %02u", Index,
        SocketData->SbspMinPathTree[Index].Node.SocId, SocketData->SbspMinPathTree[Index].Node.ParentPort,
        SocketData->SbspMinPathTree[Index].Node.Hop, SocketData->SbspMinPathTree[Index].Node.ParentIndex));
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
SetupSbspConfigAccessPath (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  )
/*++

Routine Description:
  Routine to setup the config access path from SBSP to the given socket.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data
  Interm     - Immediate parent of the socket in SBSP minimum tree for which the config
               access path needs to be set from SBSP
  Dest       - socket for which the config access path needs to be set from SBSP
  DestPort   - Port of the given socket's immediate parent that connects them.

Returns:
  
  QPI_SUCCESS

--*/
{
  TOPOLOGY_TREE_NODE               Parent;
  TOPOLOGY_TREE_NODE               Child;
  UINT8                            cLocalPort;
  UINT8                            pLocalPort = 0, pLocalPort1;
  UINT8                            Ctr;
  UINT32                           Data32;
  UINT32                           QpiQrtMaskBits;
  UINT8                            sbspSktId;
  UINT8                            PortFieldWidth, CboPortFieldWidth;
  QPIAGCTRL_QPIAGENT_LL_HSX_STRUCT QpiAgCtrl;

  sbspSktId = QpiInternalGlobal->SbspSoc;

  PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2
  CboPortFieldWidth = 4; // Four bit field to encode link 0, 1 & 2
  QpiQrtMaskBits = 0xFFFF00FF;      // Clear the inhibit_write field

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;   Setting path between SBSP and CPU%u. ", Dest->Node.SocId));

  //
  // Set the path to "Dest" in all the sockets that exist between SBSP and "Interm"
  //
  Parent      = *Interm;
  Child       = *Dest;
  cLocalPort  = DestPort;  // Note: Variable set to parent's local port (not the child's local port)
  pLocalPort1 = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[cLocalPort].PeerPort;  //Note: Variable set to Child's local port (not parent's)

  while (TRUE) {
    if (Parent.Node.SocId == sbspSktId && Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // In Sbsp, setup Cbo/R2PCIe route table for "Dest" socket; no need to setup HA route table
      // since at this point only Config transactions are active in the system.
      //
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;   In SBSP setting route to CPU%u using port %u. ", Dest->Node.SocId, cLocalPort));

      //
      // Cbo RT in SBSP
      // IvyBridge has changed to be socket ID based as all other IvyBridge routing,
      // i.e. using Node ID bit[3], bit[1], bit[0]. Node ID[2] remains but will not be used. 
      // See EX Platform sighting 4029894 from Ivytown bugeco 4368192.
      //
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotCbo[Parent.Node.SocId]; ++Ctr) {
        Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId, Ctr, QPI_ROUTING_TABLE_CBO_CFG_REG);

        Data32 = Data32 & ~FLD_MASK(CboPortFieldWidth, (CboPortFieldWidth * Dest->Node.SocId));
        Data32 = Data32 | (cLocalPort << (CboPortFieldWidth * Dest->Node.SocId));
        QpiWritePciCfg (host, (UINT8) Parent.Node.SocId, Ctr, QPI_ROUTING_TABLE_CBO_CFG_REG, Data32);
      }

      //
      // HSD s4969234: Route-through not working for socket ID > 3
      // W/A required for Remote Socket with ID > 3
      // Note: CSR not updated by pcode so access prior to check of BiosScratchPad0 on Dest node is OK. 
      //
      if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
        QpiAgCtrl.Data = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId, cLocalPort, QPIAGCTRL_QPIAGENT_LL_REG);
        QpiAgCtrl.Bits.disable_pe = 1;
        QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIAGCTRL_QPIAGENT_LL_REG, QpiAgCtrl.Data);

        QpiAgCtrl.Data = QpiReadPciCfg (host, (UINT8) Dest->Node.SocId, pLocalPort1, QPIAGCTRL_QPIAGENT_LL_REG);
        QpiAgCtrl.Bits.disable_pe = 1;
        QpiWritePciCfg (host, (UINT8) Dest->Node.SocId, pLocalPort1, QPIAGCTRL_QPIAGENT_LL_REG, QpiAgCtrl.Data);
      }

      // R2PCIe RT in SBSP socket
      Data32  = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG);
      Data32  = Data32 | (cLocalPort << (PortFieldWidth * Dest->Node.SocId));
      QpiWritePciCfg (host, (UINT8) Parent.Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG, Data32);

      // R2PCIe RT in "Dest" socket
      // Note: CSR not updated by pcode so access prior to check of BiosScratchPad0 on Dest node is OK. 
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;   In CPU%u using port %u to set the R2PCIe route. ", Dest->Node.SocId, pLocalPort1));
      Data32  = QpiReadPciCfg (host, (UINT8) Dest->Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG);
      Data32  = Data32 | (pLocalPort1 << (PortFieldWidth * Parent.Node.SocId));
      QpiWritePciCfg (host, (UINT8) Dest->Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG, Data32);

      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the path in intermediate CPU socket found between Sbsp & Dest
      //
      pLocalPort = (UINT8) Parent.Node.ParentPort;
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    In CPU%u connecting port %u to %u. ", Parent.Node.SocId, pLocalPort, cLocalPort));
      QPI_ASSERT ((pLocalPort != cLocalPort), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

      //
      // Forward Path in QPI Agent
      //
      Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIQRT_QPIAGENT_LL_REG);
      Data32 = (Data32 & QpiQrtMaskBits) | (1 << Dest->Node.SocId); // Clear the inhibit_write field
      ProgramQpiAgentRouteForWa(host, pLocalPort, (UINT8) Parent.Node.SocId, &Data32);
      QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIQRT_QPIAGENT_LL_REG, Data32);

      //
      // Backward Path in QPI Agent
      //
      Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIQRT_QPIAGENT_LL_REG);
      Data32 = (Data32 & QpiQrtMaskBits) | (1 << sbspSktId); // Clear the inhibit_write field
      ProgramQpiAgentRouteForWa(host, cLocalPort, (UINT8) Parent.Node.SocId, &Data32);
      QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIQRT_QPIAGENT_LL_REG, Data32);

      // Forward Path in R3QPI
      ProgramR3QpiRoute (host, pLocalPort, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort);

      // Backward Path in R3QPI
      ProgramR3QpiRoute (host, cLocalPort, (UINT8) Parent.Node.SocId, (UINT8) sbspSktId, pLocalPort);

      //
      // HSD s4969234: Route-through not working for socket ID > 3
      // W/A required for Remote Socket with ID > 3
      //
      // BDX : TODO : Required or not ?
      if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
        QpiAgCtrl.Data = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIAGCTRL_QPIAGENT_LL_REG);
        QpiAgCtrl.Bits.disable_pe = 1;
        QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIAGCTRL_QPIAGENT_LL_REG, QpiAgCtrl.Data);

        QpiAgCtrl.Data = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIAGCTRL_QPIAGENT_LL_REG);
        QpiAgCtrl.Bits.disable_pe = 1;
        QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIAGCTRL_QPIAGENT_LL_REG, QpiAgCtrl.Data);
      }
    } else {
      QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE, host);
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Unknown Intermediate Socket Type found. Topology not supported"));
      QPI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
    //
    // Go up one step towards the Sbsp & repeat the loop
    //
    Child = Parent;
    GetParentSocket (host, SocketData->SbspMinPathTree, &Child, &Parent);
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      cLocalPort = (UINT8) SocketData->Cpu[Child.Node.SocId].LinkData[pLocalPort].PeerPort;
    } else {
      QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    }
  }

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n"));
  return QPI_SUCCESS;
}

QPI_STATUS
SetupRemoteCpuBootPath (
  struct sysHost            *host,
  QPI_SOCKET_DATA           *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal,
  TOPOLOGY_TREE_NODE        *Interm,
  TOPOLOGY_TREE_NODE        *Dest,
  UINT8                     DestPort
  )
/*++

Routine Description:
  Routine to setup the boot path from Remote CPUs (i.e >1 hop away) to Legacy CPU.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data
  Interm     - Immediate parent of the "Dest" in SBSP minimum tree
  Dest       - Remote CPU socket for which the boot path needs to be set
  DestPort   - Port of Remote CPU socket's immediate parent that connects them.

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  TOPOLOGY_TREE_NODE  Parent;
  TOPOLOGY_TREE_NODE  Child;
  UINT8               cLocalPort;
  UINT8               pLocalPort = 0;
  UINT8               Ctr;
  UINT32              Data32;
  UINT32              QpiQrtMaskBits;
  UINT8               sbspSktId;
  UINT8               PortFieldWidth, CboPortFieldWidth;

  sbspSktId = QpiInternalGlobal->SbspSoc;
  PortFieldWidth = 2; // Two bit field to encode link 0, 1 & 2
  CboPortFieldWidth = 4; // Four bit field to encode link 0, 1 & 2

  QpiQrtMaskBits = 0xFFFF00FF;      // Clear the inhibit_write field

  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; Setting boot path for CPU%u ", Dest->Node.SocId));

  //
  // Set the boot path for Remote CPU
  //
  Parent      = *Interm;
  Child       = *Dest;
  cLocalPort  = DestPort;
  while (TRUE) {
    if (Parent.Node.SocId == sbspSktId && Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // In Remote CPU, set route to Legacy CPU
      //
      pLocalPort = (UINT8) SocketData->Cpu[Interm->Node.SocId].LinkData[DestPort].PeerPort;
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    In CPU%u setting Cbo route to port %u", Dest->Node.SocId, pLocalPort));
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotCbo[Dest->Node.SocId]; ++Ctr) {
        Data32 = QpiReadPciCfg (host, (UINT8) Dest->Node.SocId, Ctr, QPI_ROUTING_TABLE_CBO_CFG_REG);

        Data32 = Data32 & ~FLD_MASK(CboPortFieldWidth, (CboPortFieldWidth * QpiInternalGlobal->SbspSoc));
        Data32 = Data32 | (pLocalPort << (CboPortFieldWidth * QpiInternalGlobal->SbspSoc));
        QpiWritePciCfg (host, (UINT8) Dest->Node.SocId, Ctr, QPI_ROUTING_TABLE_CBO_CFG_REG, Data32);
      }
      break;
    } else if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      //
      // Setup the pass-thru path in intermediate CPU socket
      //
      pLocalPort = (UINT8) Parent.Node.ParentPort;
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;    In CPU%u connecting port %u to %u. ", Parent.Node.SocId, pLocalPort, cLocalPort));
      QPI_ASSERT ((cLocalPort != pLocalPort), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);

      //
      // Forward Path in Qpi Agent
      //
      Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIQRT_QPIAGENT_LL_REG);
      Data32 = (Data32 & QpiQrtMaskBits) | (1 << Dest->Node.SocId); // Clear the inhibit_write field
      ProgramQpiAgentRouteForWa(host, pLocalPort, (UINT8) Parent.Node.SocId, &Data32);
      QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,pLocalPort, QPIQRT_QPIAGENT_LL_REG, Data32);

      //
      // Backward Path in Qpi Agent
      //
      Data32 = QpiReadPciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIQRT_QPIAGENT_LL_REG);
      Data32 = (Data32 & QpiQrtMaskBits) | (1 << sbspSktId); // Clear the inhibit_write field
      ProgramQpiAgentRouteForWa(host, cLocalPort, (UINT8) Parent.Node.SocId, &Data32);
      QpiWritePciCfg (host, (UINT8) Parent.Node.SocId,cLocalPort, QPIQRT_QPIAGENT_LL_REG, Data32);

      // Forward Path in R3QPI
      ProgramR3QpiRoute (host, pLocalPort, (UINT8) Parent.Node.SocId, (UINT8) Dest->Node.SocId, cLocalPort);

      // Backward Path in R3QPI
      ProgramR3QpiRoute (host, cLocalPort, (UINT8) Parent.Node.SocId, (UINT8) sbspSktId, pLocalPort);      
    } else {
      QpiCheckPoint ((UINT8) (1 << Parent.Node.SocId), (UINT8) Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE, host);
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n; Unknown Intermediate Socket Type found. Topology not supported"));
      QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
    }
    //
    // Go up one step towards the Sbsp & repeat the loop
    //
    Child = Parent;
    GetParentSocket (host, SocketData->SbspMinPathTree, &Child, &Parent);
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {
      cLocalPort = (UINT8) SocketData->Cpu[Child.Node.SocId].LinkData[pLocalPort].PeerPort;
    } else {
      QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
    }
  }

  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n"));
  return QPI_SUCCESS;
}

QPI_STATUS
ProgramR3QpiRoute (
  struct sysHost            *host,
  UINT8                     InPort,                 
  UINT8                     InterSocket,
  UINT8                     DestSocId,
  UINT8                     OutPort  
  )
{
  UINT8  R3Qpi;
  UINT32 Data32;
  UINT32 EncodedRoute = 0;

  R3Qpi = PORT_TO_R3QPI(InPort);
  ConvertR3QPIRoutingTable(InPort, OutPort, DestSocId, &EncodedRoute); 

  Data32 = QpiReadPciCfg (host, (UINT8) InterSocket, R3Qpi, R3QQRT_R3QPI_CFG_REG);
  Data32 = Data32 | EncodedRoute; 

  QpiWritePciCfg (host, (UINT8) InterSocket, R3Qpi, R3QQRT_R3QPI_CFG_REG, Data32);

  return QPI_SUCCESS;
}


QPI_STATUS
ProgramQpiAgentRouteForWa (
  struct sysHost            *host,
  UINT8                     InPort,                 
  UINT8                     InterSocket,
  UINT32                    *Data32Ptr
  )
{
  return QPI_SUCCESS;
}


QPI_STATUS
UpdateCpuInfoInQpiInternal(
 struct sysHost             *host,
 QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
 UINT8                      SocktId
 )
{ 
  UINT32  Data32;

  // Socket SKU type
  GetSocketSkuType (host, SocktId, &QpiInternalGlobal->SkuType[SocktId], &QpiInternalGlobal->SubSkuType[SocktId]);
  
  // Get the physical chop for this socket and update in host struct
  UpdatePhysicalChopInfoInQpiInternal(host, QpiInternalGlobal, SocktId);
  host->var.common.chopType[SocktId] = QpiInternalGlobal->ChopType[SocktId];
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    Physical Chop: %d", SocktId, host->var.common.chopType[SocktId]));
  
  // Get the number of and list of Cbos present in SBSP.
  GetCboCountAndList (host, SocktId, &QpiInternalGlobal->TotCbo[SocktId], &QpiInternalGlobal->CboList[SocktId]);
  
  // Get the number of Sbos present in SBSP.
  GetSboCount (host, SocktId, &QpiInternalGlobal->TotSbo[SocktId]);

  // Get the number of HA present in SBSP.
  GetHACount (host, SocktId, &QpiInternalGlobal->TotHa[SocktId]);
  // Save TotHa into SYSHOST
  host->var.cpu.TotHa[SocktId] = QpiInternalGlobal->TotHa[SocktId];

  // Get the number of R3QPI present in SBSP.
  GetR3QPICount (host, SocktId, &QpiInternalGlobal->TotR3Qpi[SocktId]);

  // Get the number of QPIAgents present in SBSP.
  GetQPIAgentCount (host, SocktId, &QpiInternalGlobal->TotQpiAgent[SocktId]);
  
  // Only IVT EXA have online/offline capability
  if ((QpiInternalGlobal->SkuType[SocktId] == SKU_EXA) && (host->setup.qpi.QpiCpuSktHotPlugEn == TRUE)){
    QpiInternalGlobal-> QpiCpuSktHotPlugEn = TRUE;  
  }
  
  //
  // SBSP has initialized capid5 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID5_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid5 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID5: 0x%08X", SocktId, Data32));

  //
  // SBSP has initialized capid4 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID4_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid4 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID4: 0x%08X", SocktId, Data32));

  // 
  // SBSP has initialized capid3 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID3_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid3 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID3: 0x%08X", SocktId, Data32));

  // 
  // SBSP has initialized capid2 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID2_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid2 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID2: 0x%08X", SocktId, Data32));

  // 
  // SBSP has initialized capid1 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID1_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid1 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID1: 0x%08X", SocktId, Data32));

  // 
  // SBSP has initialized capid0 earlier.  PBSP has wait until here after being detected.
  //
  Data32 = QpiReadPciCfg (host, SocktId, 0, CAPID0_PCU_FUN3_REG);
  host->var.common.procCom[SocktId].capid0 = Data32;
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n    SocketId: %u    CAPID0: 0x%08X", SocktId, Data32));

  return QPI_SUCCESS;

}

BOOLEAN
CheckThisSocketInfoWithSbsp(
 struct sysHost             *host,
 QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
 UINT8                      SbspSktId,
 UINT8                      SocketId
 )
{ 
  BOOLEAN  FeatureMismatch = FALSE;

  // Verify Cbo counts match
  if ((QpiInternalGlobal->TotCbo[SocketId] != QpiInternalGlobal->TotCbo[SbspSktId])) {
    QpiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_CBO_COUNT_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Cbo Count/List mismatch between SBSP (CPU%u) and CPU%u:  %u vs %u",
                       SbspSktId, SocketId, QpiInternalGlobal->TotCbo[SbspSktId], QpiInternalGlobal->TotCbo[SocketId]));
    FeatureMismatch = TRUE;
  }

  // Verify Sbo counts match
  if (QpiInternalGlobal->TotSbo[SocketId] != QpiInternalGlobal->TotSbo[SbspSktId]) {
    QpiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_SBO_COUNT_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Sbo Count mismatch between SBSP (CPU%u) and CPU%u:  %u vs %u",
                       SbspSktId, SocketId, QpiInternalGlobal->TotSbo[SbspSktId], QpiInternalGlobal->TotSbo[SocketId]));
      FeatureMismatch = TRUE;
  }

  // Verify HA counts match
  if (QpiInternalGlobal->TotHa[SocketId] != QpiInternalGlobal->TotHa[SbspSktId]) {
    QpiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_HA_COUNT_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n HA Count mismatch between SBSP (CPU%u) and CPU%u - (%u vs %u)",
                       SbspSktId, SocketId, QpiInternalGlobal->TotHa[SbspSktId], QpiInternalGlobal->TotHa[SocketId]));
    FeatureMismatch = TRUE;
  }
  
  // Verify R3QPI counts match
  if (QpiInternalGlobal->TotR3Qpi[SocketId] != QpiInternalGlobal->TotR3Qpi[SbspSktId]) {
    QpiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_R3QPI_COUNT_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n R3Qpi Count mismatch between SBSP (CPU%u) and CPU%u:  %u vs %u",
                       SbspSktId, SocketId, QpiInternalGlobal->TotR3Qpi[SbspSktId], QpiInternalGlobal->TotR3Qpi[SocketId]));
      FeatureMismatch = TRUE;
  }

  // Verify SKU Type and Sub-Type values match
  if ((QpiInternalGlobal->SkuType[SocketId] != QpiInternalGlobal->SkuType[SbspSktId]) ||
      (QpiInternalGlobal->SubSkuType[SocketId] != QpiInternalGlobal->SubSkuType[SbspSktId])) {
    QpiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_SKU_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Processor SKU type/sub-type mismatch between SBSP (CPU%u) and CPU%u:  SKU Type %u vs %u   Sub-Type %u vs %u",
                       SbspSktId, SocketId, QpiInternalGlobal->SkuType[SbspSktId], QpiInternalGlobal->SkuType[SocketId],
                                            QpiInternalGlobal->SubSkuType[SbspSktId], QpiInternalGlobal->SubSkuType[SocketId]));
    if (host->setup.qpi.QpiSkuMismatchCheck) {
      FeatureMismatch = TRUE;
    } else {
      QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n WARNING: Ignoring previous mismatch"));
    }
  }

  return FeatureMismatch;
 }


QPI_STATUS
PreProcessQpiLinks (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
   Routine to check if the system has supported topology. Following terminology is used in
   describing the system topology:
   1S    - Only socket 0 is populated
   2S EN -Socket 0 & 1 are populated; there is only one link between them. Link number doesn't matter.
   2S EP -Socket 0 & 1 are populated; there are two links between them. (Link 0 & 1 only)
   4S -Total number of populated sockets is <= 4, populated sockets have ID < 4, and it is not a 1S/2S system.
   8S -All other configurations.

Note:
  When degraded topology is detected (i.e one or more links failed), BIOS will try to reduce it to one of the
  supported topology by forcing certain links and/or sockets out. Link/Socket removal is done by marking the
  appropriate data structures; nothing specifically done to the h/w that will disable a link or socket. That will
  take effect only after next reset. We track the number of and list of sockets populated before and after
  topology reduction is applied. While QPIRC uses only the reduced topology information, other modules
  such as the code that issues Warm reset needs to be aware of what is the list of sockets populated
  currently. So we update the host numCpus/socketPresentBitMap structure with the current number of and
  list of sockets populated, not based on the reduced topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8   Idx;
  UINT8   TempTotCpu;
  UINT32  TempCpuList;
  UINT8   Topology[SYS_CONFIG_MAX][5] = {{"1S"}, {"2SEN"}, {"2SEP"}, {"4S"}, {"8S"}};

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CHK_TOPOLOGY, host);

  // Sanity check the LEP info
  SanityCheckLep (host, SocketData->Cpu);
  LepDump (host, SocketData->Cpu);

  //
  // Identify the number of and list of sockets populated.
  //
  TempTotCpu  = 0;
  TempCpuList = 0;
  for (Idx = 0; Idx < MAX_CPU_SOCKETS; ++Idx) {
    if (SocketData->Cpu[Idx].Valid == TRUE) {
      TempTotCpu++;
      TempCpuList = TempCpuList | (1 << Idx);
    }
  }
  QpiInternalGlobal->OrigTotCpu = TempTotCpu;
  QpiInternalGlobal->OrigCpuList = TempCpuList;

  host->var.qpi.SysConfig = SYS_CONFIG_MAX;
  if (TempTotCpu == 1) { // 1S system
    host->var.qpi.SysConfig = SYS_CONFIG_1S;
  } else if ((TempTotCpu == 2) && (TempCpuList == 0x3)) { // 2S system
    //
    // 2SEP is special case. A system will be declared as 2S only when it has sockets 0 & 1 populated, 2 QPI links are connecting
    // the 2 sockets and the 2 sockets are connected by QPI links 0 & 1.
    // Check if the dual links are twisted and mark it accordingly.
    //
    if (SocketData->Cpu[0].LinkData[0].Valid == TRUE && SocketData->Cpu[0].LinkData[1].Valid == TRUE && 
        SocketData->Cpu[1].LinkData[0].Valid == TRUE && SocketData->Cpu[1].LinkData[1].Valid == TRUE) {
      SocketData->Cpu[0].LinkData[1].DualLink = TRUE; // Mark link1 as DualLink
      SocketData->Cpu[1].LinkData[1].DualLink = TRUE;
      host->var.qpi.SysConfig = SYS_CONFIG_2SEP;
      if (SocketData->Cpu[0].LinkData[0].PeerPort != 0) { // Port 0 <-> Port 0?
        QpiInternalGlobal->TwistedQpiLinks = TRUE;
      }
    } else {
      //
      // Ensure that only one link connection exist between the 2 sockets. For EX SKUs this topology
      // should be treated as 4S config. For EP SKUs, it will be treated as 2SEN config.
      //
      RemoveDualLink (host, SocketData);
      if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
        host->var.qpi.SysConfig = SYS_CONFIG_4S;
      } else {
        host->var.qpi.SysConfig = SYS_CONFIG_2SEN;
      }
    }
  } else if ((TempTotCpu <= 4) && ((TempCpuList & 0xF0) == 0)) { // 4S system
    //
    // Check and remove the dual links if there is any. Also check if it has supported topology otherwise force
    // supported topology.
    //
    RemoveDualLink (host, SocketData);
    CheckForDegraded4S (host, SocketData, QpiInternalGlobal);
  } else { // 8S system
#if MAX_CPU_SOCKETS > 4
    //
    // Check and remove the dual links if there is any. Also check if it has supported topology otherwise force
    // supported topology.
    //
    RemoveDualLink (host, SocketData);
    CheckForDegraded8S (host, SocketData, QpiInternalGlobal);
#else
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Unsupported topology"));
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
#endif   
  }

  //
  // If IVT EXA and hot plug enabled, will program the RTIDs and other credits assuming it as 4S or 8S system;
  // so that the RTIDs/Credits for already populated sockets need not be reprogrammed during hot add/remove event.
  //  
  if (QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE){
    host->var.qpi.SysConfig = SYS_CONFIG_4S;      // force to 4S for future hot plug
    TempTotCpu =  MAX_CPU_SOCKETS;
    if ((host->setup.qpi.QpiCpuSktHotPlugTopology == 1) && (TempTotCpu == 8)) {
      host->var.qpi.SysConfig = SYS_CONFIG_8S;   // force to 8S for future hot plug
    } 
  } 
  //
  // Sanity check the LEP info to make sure the degraded topology handling is done properly
  //
  SanityCheckLep (host, SocketData->Cpu);

  //
  // Check for feature Vs Tolology Precedence
  //
  PreProcessFeatureVsTopology (host, SocketData, QpiInternalGlobal);

  //
  // Update the global structure with the number of and list of sockets populated
  //
  QpiInternalGlobal->TotCpu = 0;
  QpiInternalGlobal->CpuList = 0;
  for (Idx = 0; Idx < MAX_CPU_SOCKETS; ++Idx) {
    if (SocketData->Cpu[Idx].Valid == TRUE) {
      QpiInternalGlobal->TotCpu++;
      QpiInternalGlobal->CpuList = QpiInternalGlobal->CpuList | (1 << Idx);
    }
  }
  if (QpiInternalGlobal->CpuList != QpiInternalGlobal->OrigCpuList) {
    LepDump (host, SocketData->Cpu);
  } else {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n;  Already Reduced to Supported Topology"));
  }

  //
  // Check to make sure the system configuration and SKU type match
  //
  QPI_ASSERT((host->var.qpi.SysConfig < SYS_CONFIG_MAX), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
    QPI_ASSERT((host->var.qpi.SysConfig != SYS_CONFIG_2SEN),ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  } else {
    QPI_ASSERT((host->var.qpi.SysConfig != SYS_CONFIG_8S),ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  System will be treated %s Configuration", Topology[host->var.qpi.SysConfig]));

  return QPI_SUCCESS;
}

QPI_STATUS
CheckForDegraded4S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Check if the system has degraded 4S topology. If yes, then force certain links and/or
  sockets out to make it a supported topology.
 
  The following topologies are supported (in order of preference):

      4S Ring Topology
      3S Ring Topology
      3S Chain Topology
      2S Single Link Topology
      1S Topology

   This routine should be called only when sockets with ID > 3 are invalid for the system.
   (So, in case of 8S system, this should be called only after sockets 4 to 7 has been 
   marked as invalid already) 

   Note that, in case of certain socket(s) forced out because it is not part of the topology,
   we mark the socket as invalid in SocketData structure. This will ensure that the rest of
   code that runs after this routine will be agnostic of the sockets being removed and it
   will not assign resources for the sockets that will not be part of the system after reset.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful

--*/
{
  UINT8   Idx1, Idx2;
  BOOLEAN Degraded;

  //
  // Check if it is 4S topology (fully connected for EX, Ring for EP); if not, it is a degraded 4S topology.
  //
  Degraded = FALSE;
  for (Idx1 = 0; Idx1 < 4; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid != TRUE) {
          Degraded = TRUE;
          break;
        }
      }
      if (Degraded == TRUE) {
        break;
      }
    }
  }

  if (Degraded == FALSE) {
    host->var.qpi.SysConfig = SYS_CONFIG_4S;
    if (MAX_QPI_PORTS == 2) {
      QpiInternalGlobal->Is4SRing = TRUE;  
    }
    return QPI_SUCCESS;
  }

  //
  // It is a degraded topology; reduce it to one of the supported topology.
  //
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n\n;  Degraded 4S topology is detected."));
  HandleDegraded4S (host, SocketData, QpiInternalGlobal, NULL);

  return QPI_SUCCESS;
}


QPI_STATUS
HandleDegraded4S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA       *DiscEngData
  )
/*++

Routine Description:

  Identifies what is the "best" topology that can be derived out of system with degraded topology.

  If each socket has connection to at least 2 different other sockets, 4S Ring topology is possible.
  If a fully connected 3S with legacy socket is possible then 3S Ring topology is possible.
  If legacy socket is connected to 2 different other sockets then 3S Chain topology is possible.
  If legacy socket is connected to 1 other socket then 2S Single link topology is possible.
  Otherwise it is a 1S topology.

  This function should be called only when the system has degraded 4S topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  QPI_SUCCESS

--*/
{
  UINT8   Idx1, Idx2, Idx3;
  UINT8   Soc1, Soc2;
  UINT8   Temp1, Temp2;
  UINT8   PeerSocList[MAX_CPU_SOCKETS];
  UINT8   PeerSocCnt[MAX_CPU_SOCKETS];
  UINT8   Ring3SList;
  UINT8   PeerSocId, PeerPort = 0;
  UINT8   RingNumber, RingNode, RingNodeList = 0;
  BOOLEAN Is4SRing, Is3SRing, Is3SChain, Is2S, Is1S;
  BOOLEAN IsDegradedFrom4S;
  
  // 
  // if DiscEngData is NULL, this function is called from the 4S degradation path
  //
  if (DiscEngData == NULL) {
    IsDegradedFrom4S = TRUE;  //Degrade 4S config to 4S-Ring or less
  } else {
    IsDegradedFrom4S = FALSE; //Degrade > 4S config to 4S-Ring or less
  }

  //
  // It can only be non-RT if it is a 3SRing, 2S, or 1S at this point.
  // We'll set it back to false in case we get to 3SRing
  //
  QpiInternalGlobal->IsRouteThroughConfig = TRUE;
  
  //
  // Identify the peer socket list & count for each populated socket.
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    PeerSocList[Idx1] = 0;
    PeerSocCnt[Idx1] = 0;
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          if ((PeerSocList[Idx1] & (1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0) {
            PeerSocList[Idx1] = PeerSocList[Idx1] |(UINT8)(1 << SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId);
            ++PeerSocCnt[Idx1];
          }
        }
      }
    }
  }


  Is4SRing = Is3SRing = Is3SChain = Is2S = Is1S = FALSE;
  Ring3SList = 0;

  //
  // If legacy socket is not connected to any other sockets then it is 1S;  Else if it is conneted to one another socket, it is 2S single link.
  //
  if (PeerSocCnt[QpiInternalGlobal->SbspSoc] == 0) {
    Is1S = TRUE;
    QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 1S"));
  } else if (PeerSocCnt[QpiInternalGlobal->SbspSoc] == 1) {
    if( QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] != SKU_EXA  && PeerSocList[QpiInternalGlobal->SbspSoc] != 02){   //if it is not EXA and peer is not socket 1
      Is1S = TRUE;
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 1S"));
    } else {
      Is2S = TRUE;
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 2S Single Link "));
    }
  } else { //4S or larger degredation
    //
    // Determine if a 4S-Ring is possible
    //
    if (IsDegradedFrom4S) {
      //
      // If each socket has 2 or more peers (i.e links) then it can be reduced to 4S ring
      //
      Is4SRing = TRUE;
      for (Idx1 = 0; Idx1 < 4; ++Idx1) {
        if (PeerSocCnt[Idx1] < 2) {
          Is4SRing = FALSE;
          break;
        }
      }//for each Socket
    } else { //Larger than 4S degration to 4S-Ring or less
      //
      // If there is a ring with legacy socket then it can be reduced to 4S ring
      //
      Is4SRing = ((DiscEngData->RingInfo[QpiInternalGlobal->SbspSoc][0] == 1) ?  TRUE : FALSE);
    }//Larger than 4S degredation

    //
    // Determine degredation landing-zone
    //
    if (Is4SRing == TRUE) {
      QpiInternalGlobal->Is4SRing = TRUE;
      QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 4S Ring "));
    } else if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
      //
      // It must be either 3S Ring or Chain. If there is a triangle involving the legacy socket then it is 3S Ring.
      // Otherwise it is 3S Chain.
      // 

      // 
      // Detect if there is any "triangle" pattern involving the legacy socket in the topology. To detect it,
      // first we need to check if legacy socket is connected to at least 2 immediate neighbor sockets.
      // And at least 2 of those immediate neighbors have a connection between them.
      //
      for (Idx2 = 0; Idx2 < MAX_CPU_SOCKETS; ++Idx2) {
        if (PeerSocList[QpiInternalGlobal->SbspSoc] & (1 << Idx2)) {
          for (Idx3 = 0; Idx3 < MAX_CPU_SOCKETS; ++Idx3) {
            if ((PeerSocList[Idx2] & (1 << Idx3)) && (PeerSocList[QpiInternalGlobal->SbspSoc] & (1 << Idx3))) {
              //
              // If Idx2's neighbor socket Idx3 is also a neighbor of QpiInternalGlobal->SbspSoc
              // then QpiInternalGlobal->SbspSoc, Idx2 and Idx3 form a triangle.
              //
              Is3SRing = TRUE;
              Ring3SList = (1 << QpiInternalGlobal->SbspSoc)  | (1 << Idx2) | (1 << Idx3);
              break;
            }
          }
          if (Is3SRing == TRUE) {
            break;
          }
        }
      }

      if (Is3SRing == TRUE) {
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 3S Ring (socket list: 0x%02X)", Ring3SList));
      } else {
        Is3SChain = TRUE;
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 3S Chain "));
      }
    } else { //Not 4S-Ring or EXA
      //
      // EP reduces to 2SEN if S0-S1, otherwise degrade to 1S
      //
      if((PeerSocList[QpiInternalGlobal->SbspSoc] & 02)) {
        Is2S = TRUE;
      } else {
        Is1S = TRUE;
        QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 1S"));
      }
    }//Degredation landing-zone
  }//4S or larger degredation

  //
  // Ensure only one Reduced Topology is selected.
  //
  Temp1 = (Is4SRing << 4) | (Is3SRing << 3) | (Is3SChain << 2) | (Is2S << 1) | Is1S; 
  Temp2 = 0;
  for (Idx1 = 0; Idx1 < 5; ++Idx1) {
    if (Temp1 & (1 << Idx1)) ++Temp2;
  }
  if (Temp2 != 1) {
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Reducing to supported topology failed"));
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  //
  // Carry out the topology reduction by marking the appropriate socket(s) and/or link(s) as invalid
  //
  if (Is1S == TRUE) {
    QpiInternalGlobal->IsRouteThroughConfig = FALSE;
    // 
    // Mark all sockets except legacy socket as invalid; also mark all the links as invalid
    // 
    host->var.qpi.SysConfig = SYS_CONFIG_1S;
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if (Idx1 != QpiInternalGlobal->SbspSoc) {
        SocketData->Cpu[Idx1].Valid = FALSE;
      }
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
      }
    }
  } else if (Is2S == TRUE) {
    QpiInternalGlobal->IsRouteThroughConfig = FALSE;
    //
    // Retain only the two sockets and the link between them
    //

    // Identify the peer socket and port
    if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
      PeerSocId = 0xFF;
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].Valid == TRUE) {
          PeerSocId = (UINT8) SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].PeerSocId;
          PeerPort   = (UINT8) SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].PeerPort;
          break;
        }
      }
    } else { //Regular 2S (not 2S EXA)
      PeerSocId = 0x1;
      PeerPort = 0xFF;
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if ((SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].Valid == TRUE) &&
            (PeerSocId == (UINT8) SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].PeerSocId)) {
             PeerPort  = (UINT8) SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[Idx2].PeerPort;
             break;
        }
      }
      QPI_ASSERT((PeerPort != 0xFF), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }//2S EXA vs 2S
    QPI_ASSERT((PeerSocId != 0xFF), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

    // Mark all other sockets and links as invalid
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if ((Idx1 != QpiInternalGlobal->SbspSoc) && (Idx1 != PeerSocId)) {
        SocketData->Cpu[Idx1].Valid = FALSE;
      }
      for (Idx3 = 0; Idx3 < MAX_QPI_PORTS; ++Idx3) {
        if ((Idx1 == QpiInternalGlobal->SbspSoc) && (Idx3 == Idx2)) {
          continue;
        } else if ((Idx1 == PeerSocId) && (Idx3 == PeerPort)) {
          continue;
        }
        SocketData->Cpu[Idx1].LinkData[Idx3].Valid = FALSE;
      }
    }//for each socket

    // 
    // For EX SKUs, this topology should be treated as 4S or 8s config. For EP SKU, if the sockets
    // have ID < 2, then it is 2SEN config. Otherwise it is treated as 4S Config.
    // 
    if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
      host->var.qpi.SysConfig = ((PeerSocId <= 3) ? SYS_CONFIG_4S : SYS_CONFIG_8S);
    } else {
      host->var.qpi.SysConfig = ((PeerSocId <= 1) ? SYS_CONFIG_2SEN : SYS_CONFIG_4S);
    }

  } else if (Is3SChain == TRUE) {
    //
    // If the third link of the legacy socket is valid, mark it as invalid. For the remaining 2 peers
    // of the legacy socket make sure all their links except the one that connects them to
    // the legacy socket are marked as invalid.
    //

    // Grantley BDX HSD 5002002 : [HSX-BDX] [HP 6000074770] [H] [BDX] HandleDegraded4S() accesses SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData out of bounds
    // Note : A 3S chain or 3S ring topology during degradation can occur with EX- SKU which has 3 QPI ports. This is not applicable for EP- SKU which has 2 ports.
    // The following code is applicable for EX- only where there are MAX_QPI_PORTS (= 3) QPI ports and thus can have 3 peer neighbors.
    if (PeerSocCnt[QpiInternalGlobal->SbspSoc] == 3) {
      // Take out the socket that is connected to the 3rd link of the legacy socket
      PeerSocId = (UINT8) SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[MAX_QPI_PORTS - 1].PeerSocId;
      SocketData->Cpu[QpiInternalGlobal->SbspSoc].LinkData[MAX_QPI_PORTS - 1].Valid = FALSE;
      SocketData->Cpu[PeerSocId].Valid = FALSE;
      PeerSocList[QpiInternalGlobal->SbspSoc] = PeerSocList[QpiInternalGlobal->SbspSoc] & (UINT8) (~(1 << PeerSocId));
      PeerSocCnt[QpiInternalGlobal->SbspSoc] = 2;
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        SocketData->Cpu[PeerSocId].LinkData[Idx2].Valid = FALSE;
      }
    }

    host->var.qpi.SysConfig = ((PeerSocList[QpiInternalGlobal->SbspSoc] <= 0x0F) ? SYS_CONFIG_4S : SYS_CONFIG_8S);

    // 
    // Mark all the links that are not part of the chain as invalid
    // (Legacy socket's link are taken care above)
    // 
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if ((Idx1 != QpiInternalGlobal->SbspSoc) && (SocketData->Cpu[Idx1].Valid == TRUE)) {
        if (!(PeerSocList[QpiInternalGlobal->SbspSoc] & (1 << Idx1))) { // Remove the socket is not part of  the chain
          SocketData->Cpu[Idx1].Valid = FALSE;
        }
        for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
          if ((SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) && 
              ((UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId != QpiInternalGlobal->SbspSoc)) {
            SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
          }
        }
      }
    }
  } else if (Is3SRing == TRUE) {
    //
    // Retain only the links that are part of the 3S Ring.
    //
    QpiInternalGlobal->IsRouteThroughConfig = FALSE;
    for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
      if (Ring3SList & (1 << Idx1)) { // If this socket is part of the 3S ring
        for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
          if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
            //
            // Check if this link's peer socket is part of the 3S ring and mark it as invalid
            //
            PeerSocId = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            if (!(Ring3SList & (1 << PeerSocId))) {
              PeerPort  = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerPort;
              SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
              SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = FALSE;
            }
          }
        }
      } else {
        // 
        // Mark the socket that is not part of the 3S Ring and all its links as invalid
        // 
        SocketData->Cpu[Idx1].Valid = FALSE;
        for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
          SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
        }
      }
    }//for each socket in 3S Ring

    host->var.qpi.SysConfig = ((Ring3SList <= 0x0F) ? SYS_CONFIG_4S : SYS_CONFIG_8S);
  
  } else { //4S or greater degredation
    if (IsDegradedFrom4S) {  // 4S degradation
      // 
      // If there is any socket that has more than 2 links, take one of the link out.
      // Note that since each socket is assured of at least two links at this point,
      // if a socket has 3 links there must be other socket that has 3 links as well.
      // By taking link between those 2 sockets 4S Ring can be formed.
      // 
      host->var.qpi.SysConfig = SYS_CONFIG_4S;
      Soc1 = Soc2 = 0xFF;
      for (Idx1 = 0; Idx1 < 4; ++Idx1) {
        if (PeerSocCnt[Idx1] == 3) {
          Soc1 = Idx1;
          for (Idx2 = Idx1 + 1; Idx2 < 4; ++Idx2) {
            if (PeerSocCnt[Idx2] == 3) {
              Soc2 = Idx2;
              break;
            }
          }
          break;
        }
      }//for each socket

      // Mark the link between the sockets as invalid
      if (Soc1 != 0xFF) {
        QPI_ASSERT ((Soc2 != 0xFF), ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
        for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
          if ((SocketData->Cpu[Soc1].LinkData[Idx2].Valid == TRUE) && (SocketData->Cpu[Soc1].LinkData[Idx2].PeerSocId == (UINT32) Soc2)) {
            PeerPort = (UINT8) SocketData->Cpu[Soc1].LinkData[Idx2].PeerPort;
            SocketData->Cpu[Soc1].LinkData[Idx2].Valid = FALSE;
            SocketData->Cpu[Soc2].LinkData[PeerPort].Valid = FALSE;
            break;
          }
        }//for each port
      }//if connected to a socket

      // Make sure it is a 4S ring
      for (Idx1 = 0; Idx1 < 4; ++Idx1) {
        Soc1 = Soc2 = 0xFF;
        for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
          if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
            if (Soc1 == 0xFF) {
              Soc1 = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            } else if (Soc2 == 0xFF) {
              Soc2 = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;
            } else {
              QPI_ASSERT (FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
            }
          }
        }//for each port
        if ((Soc1 == 0xFF) || (Soc2 == 0xFF)) {
          QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n  Reducing the Topology to 4S Ring failed!!!"));
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }//for each socket
    } else { //Greater than 4S degradation
      RingNumber = DiscEngData->RingInfo[QpiInternalGlobal->SbspSoc][1];
      for (Idx1 = 0; Idx1 < 4; ++Idx1) {
        RingNode = DiscEngData->Rings[RingNumber][Idx1];
        RingNodeList |= 1 << RingNode;
      }

      for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
        if (!(RingNodeList & (1 << Idx1))) {	// Remove sockets not part of the ring
          SocketData->Cpu[Idx1].Valid = FALSE;
          for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
            SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE;
          }
        } else {	// Socket is part of the ring
          for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
            PeerSocId = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId;	
            if (!(RingNodeList & (1 << PeerSocId))) {	// Peer socket not part of the ring
              SocketData->Cpu[Idx1].LinkData[Idx2].Valid = FALSE; // Remove the link
            }
          }
        }
      }//for each socket

      host->var.qpi.SysConfig = ((RingNodeList <= 0x0F) ?  SYS_CONFIG_4S : SYS_CONFIG_8S);
    }//4S vs > 4S degredation
    
  }//4S or greater degredation

  return QPI_SUCCESS;
} // HandleDegraded4S()

#if MAX_CPU_SOCKETS > 4
QPI_STATUS
CheckForDegraded8S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Reduces the degraded 8S topology to 6S topology if possible. Otherwise it will reduced using
  degraded 4S handling rules.

  This function should be called only when the system has degraded 4S topology.


Arguments:
  
  host   - Input/Output structure of the QPIRC
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:
  
  0 - Successful

--*/
{
  UINT8   Idx1, Idx2;
  BOOLEAN Degraded;

  //
  // Check if all the links of all the 8 sockets are valid
  //
  Degraded = FALSE;
  for (Idx1 = 0; Idx1 < 8; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid != TRUE) {
          Degraded = TRUE;
          break;
        }
      }
      if (Degraded == TRUE) {
        break;
      }
    }
  }

  if (Degraded == FALSE) {
    host->var.qpi.SysConfig = SYS_CONFIG_8S;
    return QPI_SUCCESS;
  }

  //
  // It is a degraded 8S topology; reduce it to one of the supported topology.
  //
  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n;  Degraded 8S topology is detected."));
  HandleDegraded8S (host, SocketData, QpiInternalGlobal);

  return QPI_SUCCESS;
}


QPI_STATUS
HandleDegraded8S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Reduces the degraded 8S topology to 6S topology if possible. Otherwise it will reduced using
  degraded 4S handling rules.

  This function should be called only when the system has degraded 8S topology.


Arguments:
  
  host   - Input/Output structure of the QPIRC
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.

Returns:
  
  QPI_SUCCESS

--*/
{
  BOOLEAN                 Degrade4S = FALSE;
  UINT8                   Idx1, RingNumber, RingNode = 0;
  QPI_DISC_DATA           TmpDiscEngData;


  // Prepare the discovery engine data structure
  PrepareDiscEngData (host, SocketData, &TmpDiscEngData);

  // Check and identify the rings present in the topology
  LocateRings (host, &TmpDiscEngData);

  // 
  // Any degraded 8S topology can have maximum of 3 rings and the legacy socket 
  // can be part of upto only 2 rings.
  // 
  QPI_ASSERT((TmpDiscEngData.TotRings <= 3), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  QPI_ASSERT((TmpDiscEngData.RingInfo[QpiInternalGlobal->SbspSoc][0] <= 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  //
  // Try to salvage a 6S topology by retaining 2 rings. Preference is given to the rings that
  // the legacy socekt is part of. If the legacy socket is not part of any ring, then we can not salvage 
  // a 6S topology; the topology will be reduced using 4S degrading rules. 
  //

  if (TmpDiscEngData.RingInfo[QpiInternalGlobal->SbspSoc][0] == 2) {
    DegradeTo6S (host, SocketData, QpiInternalGlobal, &TmpDiscEngData, QpiInternalGlobal->SbspSoc);
  } else if (TmpDiscEngData.RingInfo[QpiInternalGlobal->SbspSoc][0] == 1) {
    //
    // In this case, 6S topology is possible only if there is a ring adjacent to the ring that the legacy socket is part of.
    // Otheriwse it must be reduced to 4S ring topology.
    //

    // Check if any sockets of the legacy socket's ring is part of two rings 
    RingNumber = TmpDiscEngData.RingInfo[QpiInternalGlobal->SbspSoc][1];
    for (Idx1 = 0; Idx1 < 4; ++Idx1) {
      RingNode = TmpDiscEngData.Rings[RingNumber][Idx1];
      if (TmpDiscEngData.RingInfo[RingNode][0] == 2) {
        break;
      }
    }

    if (Idx1 < 4) {
      DegradeTo6S (host, SocketData, QpiInternalGlobal, &TmpDiscEngData, RingNode);
    } else {
      Degrade4S = TRUE;
    }
  } else {
    Degrade4S = TRUE;
  }

  if (Degrade4S == TRUE) {
    HandleDegraded4S (host, SocketData, QpiInternalGlobal, &TmpDiscEngData);
  }

  return QPI_SUCCESS;
}

QPI_STATUS
DegradeTo6S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  QPI_DISC_DATA        *DiscEngData,
  UINT8                CommonSoc
  )
/*++

Routine Description:

  Degrade the system to 1S topology.

Arguments:
  
  host   - Input/Output structure of the QPIRC
  SocketData - Socket specific data structure
  QpiInternalGlobal - QPIRC internal variables.
  CommonSoc - Socket that is part of any 2 rings

Returns:
  
  QPI_SUCCESS

--*/
{
  UINT8                   Idx1, Idx2, RingNumber;
  QPI_DISC_DATA           TmpDiscEngData1;

  //
  // Prepare a mask based on the sockets that part of the 2 rings. By removing the sockets that are not
  // part of the 2 rings and the links connecting to those sockets, the topology can be reduced to 6S.
  //
  DiscEngData->CpuList = 0;
  for (Idx1 = 1; Idx1 < 3; ++Idx1) {
    RingNumber = DiscEngData->RingInfo[CommonSoc][Idx1];
    for (Idx2 = 0; Idx2 < 4; ++Idx2) {
      DiscEngData->CpuList = DiscEngData->CpuList | (1 << DiscEngData->Rings[RingNumber][Idx2]);
    }
  }

  // Make sure there are 6 sockets present in the 2 rings of the CommonSoc
  DiscEngData->TotCpus = 0;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->CpuList & (1 << Idx1)) {
      ++DiscEngData->TotCpus;
    }
  }
  QPI_ASSERT((DiscEngData->TotCpus == 6), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // Mark the remaining 2 sockets and the link connecting to them as invalid
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ((DiscEngData->CpuList & (1 << Idx1)) == 0) {
      MemSetLocal ((VOID *)(&DiscEngData->Cpu[Idx1]), 0x00, sizeof (DiscEngData->Cpu[Idx1]));
    }
  }
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (DiscEngData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if ((DiscEngData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) &&
            ((DiscEngData->CpuList & (1 << DiscEngData->Cpu[Idx1].LinkData[Idx2].PeerSocId)) == 0)) {
          MemSetLocal ((VOID *)(&DiscEngData->Cpu[Idx1].LinkData[Idx2]), 0x00, sizeof (DiscEngData->Cpu[Idx1].LinkData[Idx2]));
        }
      }
    }
  }

  // Make sure the topology is reduced correctly (2 rings present and the legacy socket is part of both the rings)
  MemSetLocal ((VOID *)(&TmpDiscEngData1), 0x00, sizeof (TmpDiscEngData1));
  MemCopy ((UINT8 *) TmpDiscEngData1.Cpu, (UINT8 *) DiscEngData->Cpu, sizeof (TmpDiscEngData1.Cpu));
  TmpDiscEngData1.TotCpus = DiscEngData->TotCpus;
  TmpDiscEngData1.CpuList = DiscEngData->CpuList;
  LocateRings (host, &TmpDiscEngData1);
  QPI_ASSERT((TmpDiscEngData1.TotRings == 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  QPI_ASSERT((TmpDiscEngData1.RingInfo[CommonSoc][0] == 2), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  // 
  // Update SocketData structure with the reduced topology info.
  // System will still be considered as 8S for all configuration purposes
  // 
  QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;  Reducing it to 6S Topology "));
  host->var.qpi.SysConfig = SYS_CONFIG_8S;
  MemCopy ((UINT8 *) SocketData->Cpu, (UINT8 *) TmpDiscEngData1.Cpu, sizeof (SocketData->Cpu));

  return QPI_SUCCESS;
}

#endif

QPI_STATUS
DegradeTo1S (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Degrade the system to 1S topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  QPI_SUCCESS

--*/
{
  UINT8   Idx;

  for (Idx = 0; Idx < MAX_CPU_SOCKETS; ++Idx) {
    if (Idx != QpiInternalGlobal->SbspSoc) {
      // Mark all sockets except socket legacy socket as invalid.
      SocketData->Cpu[Idx].Valid = 0;
      SocketData->Cpu[Idx].SocId = 0;
    }
    MemSetLocal ((UINT8 *) SocketData->Cpu[Idx].LinkData, 0x00, sizeof (SocketData->Cpu[Idx].LinkData));
  }

  return QPI_SUCCESS;
}

BOOLEAN
Is4SFullyConnected (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Only check if 4S topology is fully connected for EX 
 
Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
 
  TRUE
  FALSE
 
--*/
{
  UINT8   Idx1, Idx2;
  BOOLEAN FullyConnected;

  if (host->var.qpi.SysConfig == SYS_CONFIG_8S) {
     return (FALSE);
  }

  FullyConnected = TRUE;
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == FALSE) {
       FullyConnected = FALSE;
       break;
    } else {
       for (Idx2 = 0; Idx2 < MAX_QPI_PORTS; ++Idx2) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid != TRUE) {
          FullyConnected = FALSE;
          break;
        }
       }
       if (FullyConnected == FALSE) {
        break;
       }
    }
  }

  return FullyConnected;
}

QPI_STATUS
PrimeHostStructure (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Prime the QPIRC data structure based on input option and system topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8      Ctr;


  // 
  // Update the sysHost common structure with the topology info
  // 
  host->var.common.numCpus = QpiInternalGlobal->TotCpu;
  QPI_ASSERT (( host->var.common.numCpus <= MAX_CPU_SOCKETS), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY); // Defensive check for data structure overflows
  host->var.common.socketPresentBitMap = QpiInternalGlobal->CpuList;
  for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
    if ((QpiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
      host->var.common.busIio[Ctr] = 0;
      host->var.common.busUncore[Ctr] = 0;
    } 
  } 

  //
  // Update QPIRC output structure
  //

  //
  // The host setup structure is a const strucuture. If the options provided in setup structure is 
  // invalid for the given topology QPIRC will force supported value for that option. To facilitate this
  // we copy those kind of options into host qpi structure and modify it to match the topology.
  //
  host->var.qpi.OutLegacyVgaSoc= host->setup.qpi.LegacyVgaSoc;
  host->var.qpi.OutQpiLinkSpeed = host->setup.qpi.QpiLinkSpeed;
  host->var.qpi.OutQpiLinkL0pEn = host->setup.qpi.QpiLinkL0pEn;
  host->var.qpi.OutQpiLinkL1En = host->setup.qpi.QpiLinkL1En;
  host->var.qpi.OutQpiLinkL0rEn = host->setup.qpi.QpiLinkL0rEn;
  host->var.qpi.OutClusterOnDieEn = host->setup.qpi.ClusterOnDieEn;
  host->var.qpi.OutIBPECIEn = host->setup.qpi.IBPECIEn;
  host->var.qpi.OutE2EParityEn = host->setup.qpi.E2EParityEn;
  host->var.qpi.OutEarlySnoopEn = host->setup.qpi.EarlySnoopEn;
  host->var.qpi.OutHomeDirWOSBEn = host->setup.qpi.HomeDirWOSBEn;
  host->var.qpi.OutIsocEn = host->setup.common.isocEn;
  host->var.qpi.OutMesegEn = host->setup.common.mesegEn;
  host->var.qpi.OutIsocAzaliaVc1En = host->setup.qpi.IsocAzaliaVc1En;

  for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
    //
    // BIOS Setup for QPI.
    //
    host->var.qpi.OutIioUniphyDisable[Ctr] = host->setup.qpi.IioUniphyDisable[Ctr];
  }
  host->var.qpi.OutQpiCrcMode = host->setup.qpi.QpiCrcMode;

  //
  // Initialize the QPIRC global variables and check the host input structure to make sure it is
  // consist with the system topology.
  //

  // Disabling IIO UniPhy on legacy socket will also kill DMI.
  host->var.qpi.OutIioUniphyDisable[QpiInternalGlobal->SbspSoc] = FALSE;

  // Make sure the legacy VGA range target is valid socket
  if ((host->var.qpi.OutLegacyVgaSoc >= MAX_CPU_SOCKETS) || (SocketData->Cpu[host->var.qpi.OutLegacyVgaSoc].Valid != TRUE)) {
    QpiLogWarning(host, 0xFF, 0xFF, 0xFF, WARN_VGA_TARGET_SOC_NOT_PRESENT);
    QpiDebugPrintWarn((host, QPI_DEBUG_WARN, "\n WARNING:  Legacy VGA range target not populated; assiging the range to Legacy CPU socket. "));
    host->var.qpi.OutLegacyVgaSoc = QpiInternalGlobal->SbspSoc; // Make Legacy socket as the VGA range target
  }

  PrimeHostStructureIsoc (host, SocketData, QpiInternalGlobal);
  
  PrimeHostStructureHsxAndBdx (host, SocketData, QpiInternalGlobal);
  IdentifySysConfigTypeHsxAndBdx (host, SocketData, QpiInternalGlobal);




  return QPI_SUCCESS;
}

QPI_STATUS
PrimeHostStructureIsoc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Prime the QPIRC data structure based on input option and system topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT  capid1_bdx;
  UINT8                           Index;

  //
  // EX overrides
  //  
   if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) ||
       ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX)) 
      ) {
    host->var.qpi.OutIsocEn = FALSE;
    host->var.qpi.OutMesegEn = FALSE;
    host->var.qpi.OutIsocAzaliaVc1En = FALSE;
    return QPI_SUCCESS;
    
  }
    
  // 
  // HSXEP allows Isoc in 1/2S.  2Link, 1/2HA
  // 
  if ((host->var.qpi.SysConfig != SYS_CONFIG_1S && host->var.qpi.SysConfig != SYS_CONFIG_2SEP)) {
    host->var.qpi.OutIsocEn = FALSE;
  } else if ((host->var.common.cpuType == CPU_BDX) && \
             (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_DE || QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_NS)) {
    host->var.qpi.OutIsocEn = FALSE;
  }

  //
  // BDX-EP : Restrict the isoc support to <= 20 Cbos
  // Grantley HSD 5002465 : [Isoc]: Isoc support restricted to <=20 CBOs
  // 
  if ((host->var.common.cpuType == CPU_BDX) && (QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] > 20)) {
    host->var.qpi.OutIsocEn = FALSE;
  }

  //
  // Grantley HSD 5002464 : [Isoc] Enable isoc knob by default on HEDT and WS SKUs
  // Disable isoc for all cases except when BDX-A1- and later parts with HCC chops are detected
  //
  // Grantley HSD 5002699 : [Isoc]BDX EP MCC Isoc enable in BIOS
  // Enable Isoc for MCC chops for now. Leave isoc to be disabled for LCC
  //
  // Grantley HSD 5002815 : Clone from BDX Sighting:BIOS 258.V10 does not allow ISOC to be enable on R0 (LCC) parts
  // From now, isoc is enabled from all physical chops which are BDX-A1+
  //
  // Grantley HSD 5003146 : [BDX-EP] : Allow ISOC feature to be enabled for HSX- CPUs
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; Index++) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      // Read CAPID1 from the current socket
      capid1_bdx.Data = host->var.common.procCom[Index].capid1;

      if ((host->var.common.cpuType == CPU_HSX) || 
          ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX) && (capid1_bdx.Data & BIT20)) ||
          ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping >= B0_REV_BDX))) {
        // HSX- or BDX-A1 or BDX-B0+ cpu detected; do nothing
      } else {
          host->var.qpi.OutIsocEn = FALSE;
      } // cpuType and stepping check
    } // socket valid check
  } // for socket loop


  //
  // MeSeg can be enabled in any snoop mode/RBT layout for HSX
  //
  if (host->var.qpi.OutIsocEn == TRUE) {
    // MESEG is mutually exclusive with Isoc mode
    host->var.qpi.OutMesegEn = FALSE;
  } else {
    // IsocAzalia can not be enabled if Isoc is not enabled
    host->var.qpi.OutIsocAzaliaVc1En = FALSE;
  }
                     
  // If IsocAzaliaVc1 is set to "Auto", then it is safe to set it to "TRUE" since this not a 4S system and ISOC is enabled
  if (host->var.qpi.OutIsocAzaliaVc1En > 1) {
    host->var.qpi.OutIsocAzaliaVc1En = TRUE;
  }


  return QPI_SUCCESS;
}


QPI_STATUS
PrimeHostStructureHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Prime the QPIRC data structure based on input option and system topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  CAPID5_PCU_FUN3_BDX_STRUCT      Capid5_Bdx;
  CAPID5_PCU_FUN3_HSX_STRUCT      Capid5_Hsx;
  UINT32                          Capid5_Bits_cod_enable;
  CAPID3_PCU_FUN3_BDX_STRUCT      capid3_bdx;
  CAPID4_PCU_FUN3_BDX_STRUCT      capid4_bdx;
  UINT8                           Index;
  BOOLEAN                         Support4SCOD;

  if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
    QpiInternalGlobal->QpiCpuSktHotPlugEn = host->setup.qpi.QpiCpuSktHotPlugEn;
  } else {
    QpiInternalGlobal->QpiCpuSktHotPlugEn = 0;
  }

  //
  // EX overrides
  //  
  if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
    host->var.qpi.OutEarlySnoopEn = FALSE;
    host->var.qpi.OutMesegEn = FALSE;
  }

  // For HSX-EX (only), apply the overrides for COD feature. HSX-EP/EN supports COD.
  // BDX-EP and BDX-EX supports COD feature.
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) {
#ifndef QPI_4S_COD_FEATURE_PROTOTYPE
    host->var.qpi.OutClusterOnDieEn = FALSE;
#endif // QPI_4S_COD_FEATURE_PROTOTYPE
  }

  //
  // Check COD support for SBSP from CAPID5[31]
  //
  if (host->var.common.cpuType == CPU_HSX) {
    Capid5_Hsx.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, CAPID5_PCU_FUN3_REG);
    Capid5_Bits_cod_enable  = Capid5_Hsx.Bits.cod_enable;
  } else {
    Capid5_Bdx.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, CAPID5_PCU_FUN3_REG);
    Capid5_Bits_cod_enable  = Capid5_Bdx.Bits.cod_enable;
  }

  // (HSD-5003319):Change EP CoD Check to look for lcc chop rather than core count based (>10c)
  // BDX uses the CAPID5.codenablebit and HA number to adjudge the CoD enable/disable.
  if ((Capid5_Bits_cod_enable == 0) ||                                //COD not supported by this CPU
      (QpiInternalGlobal->TotHa[QpiInternalGlobal->SbspSoc] != 2) ||  //COD requires 2 HAs
      (host->var.common.cpuType == CPU_HSX && QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] < 10)) {   // COD not allowed on LCC on HSX
    host->var.qpi.OutClusterOnDieEn = FALSE;
  }

  
  //
  // COD requires 1S or 2S-2Q in HSX,
  // COD requires 1S or 2S-2Q or 4S-ring in BDX
  //
  Support4SCOD= Is4SCODSupportedOnTopology( host, SocketData, QpiInternalGlobal);

  if ((host->var.qpi.SysConfig != SYS_CONFIG_1S) && (host->var.qpi.SysConfig != SYS_CONFIG_2SEP) && (Support4SCOD == FALSE)) {  
    host->var.qpi.OutClusterOnDieEn = FALSE;
  }
  
  if ((host->var.qpi.OutClusterOnDieEn == TRUE) && (Support4SCOD == TRUE)) {
    QpiInternalGlobal->Sys4SClusterOnDieEn = TRUE;
  } else {
    QpiInternalGlobal->Sys4SClusterOnDieEn = FALSE;
  }

  //
  // ISOCH configs use Early Snoop
  //
  if (host->var.qpi.OutIsocEn == TRUE) {
    host->var.qpi.OutEarlySnoopEn = TRUE;
  }
  
  //
  // EarlySnoop requires 1S or 2Q
  //
  if ((host->var.qpi.SysConfig != SYS_CONFIG_1S) && (host->var.qpi.SysConfig != SYS_CONFIG_2SEP)) {
    host->var.qpi.OutEarlySnoopEn = FALSE;
  }
  
  //
  // Home Directory Snoop w/ IVT- Style OSB is turned ON by default for 2SEN(2S-1Q) w/o COD, 
  // 2SEP(2S-1Q) w/o COD and 4S- w/ COD in both EP- and EX- segments.
  //
  if ((host->var.qpi.SysConfig == SYS_CONFIG_2SEN && host->var.qpi.OutClusterOnDieEn == FALSE) ||
      (host->var.qpi.SysConfig == SYS_CONFIG_2SEP && host->var.qpi.OutClusterOnDieEn == FALSE) ||
      (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA && QpiInternalGlobal->TotCpu == 2 && host->var.qpi.OutClusterOnDieEn == FALSE) ||
      (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE)) {
      // Do nothing, home Directory Snoop w/ IVT- Style OSB is supported
  } else {
    host->var.qpi.OutHomeDirWOSBEn = FALSE;
  }
  
  //
  // Home Directory Snoop w/ IVT- Style OSB trumps over ES (Isoc indirectly)
  //
  if (host->var.qpi.OutHomeDirWOSBEn == TRUE) {
    host->var.qpi.OutEarlySnoopEn = FALSE;
    host->var.qpi.OutIsocEn = FALSE;
  }
  
  //
  // COD takes precedence over ES/Isoc and HomeDir W/ OSB
  //
  if (host->var.qpi.OutClusterOnDieEn == TRUE) {
    host->var.qpi.OutEarlySnoopEn = FALSE;
    host->var.qpi.OutIsocEn = FALSE;
  }
  
  //
  // Only allow Meseg in 1S/2S non-COD configurations for HSX- and
  // in 1S/2S both COD and non-COD configs for BDX-
  //
  // Refer BDX 5001795 : [HSX-BDX] [HP] Requesting COD mode with concurrent support for ME traffic (VCM)
  if ((host->var.common.cpuType == CPU_HSX && host->var.qpi.OutClusterOnDieEn == TRUE) ||
      (host->var.qpi.SysConfig > SYS_CONFIG_2SEP)) {
    host->var.qpi.OutMesegEn = FALSE;
  }
  
  //
  // Set snoop mode.  HOME+DIR for COD and 4S and  >1S EX.  ES if enabled (Isoc or user 1/2SEP).   HS for everything else
  // ClusterOnDie and 4S uses Home Snoop with OSB (Opportunistic Snoop Broadcast) and Directory mode.
  //                            
  if (((host->var.qpi.OutClusterOnDieEn == TRUE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) || 
      ((host->var.common.cpuSubType == CPU_HSX_EX) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) ||
      ((host->var.common.cpuSubType == CPU_BDX_EX) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) ||
      (host->var.qpi.SysConfig == SYS_CONFIG_4S) ||
      (host->var.qpi.OutHomeDirWOSBEn == TRUE)) {
    QpiInternalGlobal->SysSnoopMode = HOME_DIR_OSB_SNOOP_MODE;
  } else if (host->var.qpi.OutEarlySnoopEn == TRUE) { 
      QpiInternalGlobal->SysSnoopMode = EARLY_SNOOP_MODE;
  } else {
      QpiInternalGlobal->SysSnoopMode = HOME_SNOOP_MODE;
  }
    

  //
  // Identify the BT mode to be used for the system. 
  // EP 1S & 2S ISOCH configs, and 2S early snoop do not use BT
  // 8S uses BtMode2
  // COD uses BtMode3 for 1S and 2SEP
  // All others use BT_mode1.
  // IODC & HitMe only with DIR
  //
  if (((host->var.common.cpuSubType != CPU_HSX_EX) && (host->var.qpi.SysConfig == SYS_CONFIG_1S) && (host->var.qpi.OutClusterOnDieEn == FALSE)) || (host->var.qpi.OutIsocEn == TRUE) || (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE)) {
    QpiInternalGlobal->BtModeEn = FALSE;
  } else if ((host->var.qpi.OutClusterOnDieEn == TRUE) && (QpiInternalGlobal->Sys4SClusterOnDieEn == FALSE)) { // For 1S-COD and 2SEP-COD only
    QpiInternalGlobal->BtModeEn = TRUE;
    QpiInternalGlobal->BtMode = BT_MODE_3;
  } else if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
    QpiInternalGlobal->BtModeEn = TRUE;
    QpiInternalGlobal->BtMode = BT_MODE_2;
  } else if (host->var.qpi.SysConfig == SYS_CONFIG_8S) {
    QpiInternalGlobal->BtModeEn = TRUE;
    QpiInternalGlobal->BtMode = BT_MODE_2;
  } else {
    QpiInternalGlobal->BtModeEn = TRUE;
    QpiInternalGlobal->BtMode = BT_MODE_1;
  }
  
  if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE) || (QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE)) {
    QpiInternalGlobal->HitMeEn = 1;
  } else {
    QpiInternalGlobal->HitMeEn = 0;
  }  
  
  // Disable HitMe while enabling the IVT- style OSB
  if (host->var.qpi.OutHomeDirWOSBEn == TRUE) {
    QpiInternalGlobal->HitMeEn = 0;
  }
  
  // If requesting Early Snoop mode, disable BT
  if (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
    QpiInternalGlobal->BtModeEn = FALSE;
    QpiInternalGlobal->BtMode = 0;
  }

  //
  // Identify the Cbo RTID Pool mode to be used for the system based on the BT mode
  //
  if (QpiInternalGlobal->BtMode == BT_MODE_1) {
    QpiInternalGlobal->CboPoolMode = DOUBLE_POOLIVT_MODE;
  } else if (QpiInternalGlobal->BtMode == BT_MODE_3) {
    QpiInternalGlobal->CboPoolMode = DOUBLE_POOLIVT_MODE;
  } else {
    QpiInternalGlobal->CboPoolMode = DOUBLE_POOL8S_MODE;
  }

  QpiInternalGlobal->SnoopFanoutEn = FALSE;

  //
  // Enable for  2S COD, 4S COD, 4S Ring and beyond > 4S
  //
  if (((host->var.qpi.OutClusterOnDieEn == TRUE) && (host->var.qpi.SysConfig == SYS_CONFIG_2SEP)) || 
      ((host->var.qpi.SysConfig == SYS_CONFIG_4S) && (QpiInternalGlobal->Is4SRing)) || 
      (host->var.qpi.SysConfig > SYS_CONFIG_4S) ||
      (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE)){
    QpiInternalGlobal->SnoopFanoutEn = TRUE;
  }

  // 
  // SnoopFanoutEn disable for RAS. It can be enabled manually by Setup if needed. Debugging in progress. 
  //
  if (QpiInternalGlobal->QpiCpuSktHotPlugEn) {
    QpiInternalGlobal->SnoopFanoutEn = FALSE;   
  }


  // (HSD-4969795):HSX Clone : QPI: fast retrain loops using reset or L1 entry/exit causes lane drop and EP Init Aborts / EX DFO
  // LaneDropPcodeFixFn is disabled so that the silicon fix is used for the Qpi lane drop issue
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX) && (host->var.common.stepping == C0_REV_HSX)) {
    QpiInternalGlobal->LaneDropPcodeFixEn = TRUE;
  } else {
    QpiInternalGlobal->LaneDropPcodeFixEn = FALSE;
  }

  
  // VN1 is not supported in EP
  QpiInternalGlobal->Vn1En = FALSE;  
  if (host->var.qpi.SysConfig == SYS_CONFIG_8S) {
     QpiInternalGlobal->Vn1En = TRUE;   
  } 
  
  //
  // D2C allowed in all configs, except when running BTMode2 128/48 (Alt-1) on HSX-D0+ e.g. 8S config
  //
  QpiInternalGlobal->D2cEn = TRUE;

  if (QpiInternalGlobal->BtMode == BT_MODE_2 && host->var.common.cpuType == CPU_HSX && host->var.common.stepping >= D0_REV_HSX) {
    QpiInternalGlobal->BtMode2Alt = 1;  //Default for BTMode2 is 128/48 RTIDs  (0 = 112/48 RTIDs)
    QpiInternalGlobal->D2cEn = FALSE;
  }


  if (host->var.common.cpuType == CPU_BDX) {
    // Enable by IODC by default when directory is activated
    if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE) || (QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE)) {
      QpiInternalGlobal->IodcEn = 1;
    } else {
      QpiInternalGlobal->IodcEn = 0;
    }

    // Read from CAPID3 register from all sockets see if dir is active in all of them
    for (Index = 0; Index < MAX_CPU_SOCKETS; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        // Read CAPID3 from given socket
        capid3_bdx.Data = QpiReadPciCfg (host, Index, 0, CAPID3_PCU_FUN3_REG);

        if (capid3_bdx.Bits.disable_dir) {
          // When bit 11 disable_dir is set, directory is disabled
          QpiInternalGlobal->IodcEn = 0;
          break;
        } // capid3 check
      } // socket valid check
    } // for loop
  } else {
    // IODC is not supported on HSX, so disable IODC for all HSX configs
    QpiInternalGlobal->IodcEn = 0;
  } // cpu check
  
  //
  // Grantley BDX HSD : 5002678 : [BDX-EP]: 4S COD POR Change : Disable IODC, HitMe$ and 1+ hops DCA accesses for 4S-COD- EP configs
  // Brickland BDX HSD : 4971403 : BIOS brickland BDX: POR change for 4S COD 
  // Note : This is applicable only for 4S-COD-EP- and not for 4S-COD-EX- configs
  // 
  // Grantley BDX HSD : 5002725 : CLONE from broadwell_server: Clone from BDX Sighting:4S COD IVT Style Snoop mode with IODC: PCIE Remote BW shortfall
  // Note : This is same as 5002678 but resulted from Perf. debugging
  if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE && QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] != SKU_EXA) {
    QpiInternalGlobal->IodcEn = 0;
  }


  //
  // Set the PointerSeperationHA which MRC can use in future
  //
  host->var.qpi.OutPointerSeperationHA = GetDefaultPointerSeperationValues(host);


  //
  // Grantley HSD 5002377 : EP-ES2 QHU0 SKU - QPI speed for this SKU should be clipped at 6.4 GT/s since OC is enabled
  //
  if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping == A0_REV_BDX)) {
    // Force to 6.4 GT/s operation when CAPID4[oc_enabled] is set in atleast one of the sockets
    for (Index = 0; Index < MAX_CPU_SOCKETS; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        // Read CAPID4 from the current socket
        capid4_bdx.Data = QpiReadPciCfg (host, Index, 0, CAPID4_PCU_FUN3_REG);

        if (capid4_bdx.Bits.oc_enabled == 1) {
          QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;    Detected an over-clocked part in S%u. Forcing 6.4 GT/s operation\n", Index));
          host->var.qpi.OutQpiLinkSpeed = SPEED_REC_64GT;
        } // capid4 check
      } // socket valid check
    } // for loop
  } //cpuType check

  return QPI_SUCCESS;
}


QPI_STATUS
IdentifySysConfigTypeHsxAndBdx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Prime the QPIRC data structure based on input option and system topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8 ConfigType = 0;
  CAPID4_PCU_FUN3_BDX_STRUCT      Capid4_Bdx;
  CAPID4_PCU_FUN3_HSX_STRUCT      Capid4_Hsx;
  UINT32                          Capid4_Bits_physical_chop;

  if (host->var.common.cpuType == CPU_HSX) {
    Capid4_Hsx.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, CAPID4_PCU_FUN3_REG);
    Capid4_Bits_physical_chop = Capid4_Hsx.Bits.physical_chop;
  } else {
    Capid4_Bdx.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, CAPID4_PCU_FUN3_REG);
    Capid4_Bits_physical_chop = Capid4_Bdx.Bits.physical_chop;
  }

  // Identify the config type
  if (host->var.qpi.SysConfig == SYS_CONFIG_8S) {          //8S
    if (Capid4_Bits_physical_chop == HCC) {
      ConfigType = EXA_8S_HCC; 
    } else {
      QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);     //invalid configuration
    }
  } else if (host->var.qpi.SysConfig == SYS_CONFIG_4S) {  //4S
    if (Capid4_Bits_physical_chop == HCC) {
      if (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXA) {
        ConfigType = EXA_4S_HCC; 
      } else {
        ConfigType = EP_4S_HCC; 
      }
    } else {
      ConfigType = EP_4S_HCC;                          //use EP_4S_HCC for EP_4S_MCC configuration    
    }
  } else if (host->var.qpi.SysConfig == SYS_CONFIG_2SEP ||                //2S_EP 
    QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EP ||   //Or sku_EP (maybe 1 link active only)
    QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_EXB) {  //Or sku_EXB(maybe 1 link active only)

    if (Capid4_Bits_physical_chop == HCC) {                  // EP_HCC
       
        if (QpiInternalGlobal->BtModeEn == TRUE) {
          ConfigType = EP_HCC_DHT_ENABLED;
        } else if (host->var.qpi.OutIsocEn == FALSE) {
          if (host->var.qpi.OutEarlySnoopEn == TRUE) {
            ConfigType = EP_HCC_ES_ISOC_DISABLED; 
          } else {
            ConfigType = EP_HCC_SHT_ISOC_DISABLED; 
          }
        } else {
          ConfigType = EP_HCC_SHT_ISOC_ENABLED; 
        }

    } else if (Capid4_Bits_physical_chop == MCC) {          // EP_MCC

        if (QpiInternalGlobal->BtModeEn == TRUE) {
          ConfigType = EP_MCC_DHT_ENABLED;
        } else if (host->var.qpi.OutIsocEn == FALSE) {
          if (host->var.qpi.OutEarlySnoopEn == TRUE) {
            ConfigType = EP_MCC_ES_ISOC_DISABLED; 
          } else {
            ConfigType = EP_MCC_SHT_ISOC_DISABLED; 
          }
        } else {
          ConfigType = EP_MCC_SHT_ISOC_ENABLED; 
        }
        
    } else {                                              // EP_LCC
        
        if (QpiInternalGlobal->BtModeEn == TRUE) {
          ConfigType = EP_LCC_DHT_ENABLED;
        } else if (host->var.qpi.OutIsocEn == FALSE) {
          ConfigType = EP_LCC_SHT_ISOC_DISABLED; 
        } else {
          ConfigType = EP_LCC_SHT_ISOC_ENABLED; 
        }

      }

  } else if (host->var.qpi.SysConfig == SYS_CONFIG_2SEN) {     // 2S_EN
      if (Capid4_Bits_physical_chop == HCC) {                  // EN_HCC
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE); // Invalid configuration      

      } else if (Capid4_Bits_physical_chop == MCC) {          // EN_MCC
        if (QpiInternalGlobal->BtModeEn == TRUE) {
          ConfigType = EN_MCC_DHT_ENABLED;
        } else {
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE); // Invalid configuration           
        }

      } else if (Capid4_Bits_physical_chop == LCC) {
        if (QpiInternalGlobal->BtModeEn == TRUE) {            // EN_LCC
          ConfigType = EN_LCC_DHT_ENABLED;
        } else {
          QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE); // Invalid configuration           
        }            

      }
  } else {                      //1S
      ConfigType = SYS_1S;
  }

  QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;    Sys configuration Type = %d", ConfigType));
  QpiInternalGlobal->SysConfigType = ConfigType;

  return QPI_SUCCESS;
}

QPI_STATUS
SyncUpPbspForReset (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                  RstType
  )
/*++

Routine Description:
  Routine to sync up the PBSPs and update the necessary scratch registers. As part of that, 
  the sockets taht are not connected to legacy are issued "BOOT_GO" command to bring them
  under BIOS control.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data
  RstType - Boot path type.  powergood or warm reset flow 
Returns:

--*/
{
  UINT8           Index, Port, Cbo, Idx2, NotCheckedInSocketNum;
  UINT32          Data32, Data32_1, Sr07, Sr02, Sr03, GblNidMap0, GblNidMap1, GidNidMap;
  UINT32          Ctr, WaitTime;
  SAD_TARGET_CBOBC_IOSAD_HSX_BDX_STRUCT      SadTarget;
  UINT8           CheckInFlag[MAX_CPU_SOCKETS], AllCheckInFlag;

  if (RstType == POST_RESET_POWERGOOD) {
    //
    // Update the Ubox sticky scratch register 07 in each socket the following info:
    // [19:16]  - SBSP socket id 
    // 
    Sr07 = (UINT32) (QpiInternalGlobal->SbspSoc << 16);
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;    Setting Ubox Sticky SR07 to 0x%08X ", Sr07));
 
    // 
    // Update the Ubox sticky scratch register 03 in each socket the following info:
    // [2:0]   - Total number of buses  assigned to CPU0 in 32 Bus granularity; 00 - 32 buses, 1 - 64 buses ... 7 - 256 buses
    // [5:3]   - CPU1 bus range
    // [8:6]   - CPU2 bus range
    // [11:6]  - CPU3 bus range
    // [14:12] - CPU4 bus range
    // [17:15] - CPU5 bus range
    // [20:18] - CPU6 bus range
    // [23:21] - CPU7 bus range
    // [29:24] - MMCFG Base, to store Address [31:26] of MMCFG BASE
    // 
    Sr03 = 0;
//#ifndef HW_EMULATION
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) { // CPU list
      if (SocketData->Cpu[Index].Valid == TRUE){
        Sr03 |= ((((host->var.qpi.CpuInfo[Index].CpuRes.BusLimit - host->var.qpi.CpuInfo[Index].CpuRes.BusBase + 1) / 32) - 1) << (Index * 3 ));
      }
    }
//#else
//    Sr03 = 0x249; //ff,bf,7f,3f
//#endif
    Sr03 |= (host->setup.common.mmCfgBase >> 2); // 26 -> 24
    
    //
    // Communicate to pcode if ES or Isoc is to be enabled
    //
    Sr03 = Sr03 & ~(BIT31 | BIT30);
    if ((QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) {
      Sr03 = Sr03 | BIT30;
    }
    if (host->var.qpi.OutIsocEn == TRUE) {
      Sr03 = Sr03 | BIT31;
    }

    //
    // If EX and Vn1enable, set SR07 bit 0 to 1 to communicate CPURC enable before QPI traffic
    //
    if (QpiInternalGlobal->Vn1En == TRUE) {
      Sr07 = Sr07 | BIT0;
    }
    
    //
    // If hot plug enabled, set sr07 bit 1 to 1
    //
    if( QpiInternalGlobal->QpiCpuSktHotPlugEn == TRUE ) {
      Sr07 = Sr07 | BIT1;
    }
    
    //
    // Update the SBSP Ubox sticky scratch register 02 with the LEP info. This info is used to verify
    // if the system QPI topology changed across boots and take remidial action. Note that in case
    // of reduced topology scenario, scratch registers are updated with the reduced topology, not
    // the current topology. The current reset flow has been running under the assumption of the
    // reduced topology. (For example RTIDs/Credits are programmed for reduced topology scenario)
    // [07:00]   - CPU list; this list reflects the CPUs after the topology is degraded, if needed
    // [10:08]   - CPU0 Link 0, 1, 2 Valid
    // [13:11]   - CPU1 Link 0, 1, 2 Valid
    // [16:14]   - CPU2 Link 0, 1, 2 Valid
    // [19:17]   - CPU3 Link 0, 1, 2 Valid
    // [22:20]   - CPU4 Link 0, 1, 2 Valid
    // [25:23]   - CPU5 Link 0, 1, 2 Valid
    // [28:26]   - CPU6 Link 0, 1, 2 Valid
    // [31:29]   - CPU7 Link 0, 1, 2 Valid
    //
    Sr02 = 0;
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
          if (SocketData->Cpu[Index].LinkData[Port].Valid == TRUE) {
            Sr02 = Sr02 | (1 << ((Index * SI_MAX_QPI_PORTS) + Port));
          }
        }
      }
    }
    Sr02 = (Sr02 << 8) | (QpiInternalGlobal->CpuList);

    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;    Setting Ubox Sticky SR03 to 0x%08X ", Sr03));
    QpiDebugPrintInfo1((host, QPI_DEBUG_INFO1, "\n\n;    Setting Ubox Sticky SR02 to 0x%08X ", Sr02));

    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE){
        Data32 = QpiReadPciCfg (host, Index, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
        Data32 = (Data32 & 0xFFF0FFFF) | Sr07;
        QpiWritePciCfg (host, Index, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG, Data32);
        QpiWritePciCfg (host, Index, 0, BIOSSCRATCHPAD3_UBOX_MISC_REG, Sr03);
        QpiWritePciCfg (host, Index, 0, BIOSSCRATCHPAD2_UBOX_MISC_REG, Sr02);
      }
    }
  }

  //
  // Set the "BOOT_GO" flag in the remote socket to bring it under BIOS control.
  //
  MemSetLocal ((UINT8 *) &CheckInFlag, FALSE, sizeof (CheckInFlag));

  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if((Index == QpiInternalGlobal->SbspSoc) || (SocketData->Cpu[Index].Valid != TRUE)) {
      CheckInFlag[Index] = TRUE;
      continue;
    } else if (CheckCpuConnectedToLegCpu (host, SocketData, QpiInternalGlobal, Index) == FALSE) {

      // 
      // BIOS has to initialize the following fields of SAD_TARGET_CBOBC_IOSAD register for the remote socket.
      // For the legacy socket and the sockets directly connected to it, the pCode will set them for this register.
      // 
      SadTarget.Data = QpiReadPciCfg (host, Index, 0, SAD_TARGET_CBOBC_IOSAD_REG);
      SadTarget.Bits.flash_target       = QpiInternalGlobal->SbspSoc;
      SadTarget.Bits.legacy_pch_target  = QpiInternalGlobal->SbspSoc;
      QpiWritePciCfg (host, Index, 0, SAD_TARGET_CBOBC_IOSAD_REG, SadTarget.Data);

      // 
      // BIOS has to initialize the LT_CONTROL register for the remote socket.
      // For the legacy socket and the sockets directly connected to it, the pCode will set this register.
      // So we just replicate the register setting from SBSP to the remote socket.
      // 
      Data32 = QpiReadPciCfg(host, QpiInternalGlobal->SbspSoc, 0, LT_CONTROL_CBOBC_IOSAD_REG);
      if (Data32 & 0x8) { // LT_Target_enable is set?
        Data32_1 = QpiReadPciCfg(host, Index, 0, LT_CONTROL_CBOBC_IOSAD_REG);
        Data32_1 = (Data32_1 & 0xFFFFFFF0) | (Data32 & 0xF); // Set LT_Target_enable & LT_target fields
        QpiWritePciCfg(host, Index, 0, LT_CONTROL_CBOBC_IOSAD_REG, Data32_1);
      }
      

      //
      // In LT enabled system, releasing socket2 may cause a LT_SHUTDOWN, which causes a surprise warm reset. 
      // Set bit29 of SP7 to indicate we are releasing the remote socket, so that if LT_SHUTDOWN happes.
      // BIOS can detect this and handle it.
      //
      for (Idx2 = 0; Idx2 < MAX_CPU_SOCKETS; ++Idx2) {
        if (SocketData->Cpu[Idx2].Valid == TRUE) {
          Data32 = QpiReadPciCfg(host, Idx2, 0,BIOSSCRATCHPAD7_UBOX_MISC_REG);
          Data32 = Data32 | 1 << 29;
          QpiWritePciCfg(host, Idx2, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG, Data32);
        }
      }
      
      // 
      // Issue BOOT_GO command
      // 
      QpiCheckPoint((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0, STS_PBSP_SYNC_UP, MINOR_STS_ISSUE_BOOT_GO, host);
      QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;    Issuing BOOT_GO command to CPU%u. ", SocketData->Cpu[Index].SocId));
      Data32 = QpiReadPciCfg(host, Index, 0, BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG);
      Data32 = (Data32 & 0xFFFFFF00) | 0x2; // Issue BOOT_GO
      QpiWritePciCfg(host, Index, 0, BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG, Data32);
    } else {                         // skip the socket directly connected to SBSP
      CheckInFlag[Index] = TRUE;
    }
  }

  //
  // Wait for the remote CPU(s) to check-in.
  //
  QpiDebugPrintInfo0((host, QPI_DEBUG_INFO0, "\n;    Verifying if the remote socket(s) checked-in. "));
  OemWaitTimeForPSBP(host, &WaitTime);
  NotCheckedInSocketNum  = 0;
  // Loop until either all CPUs checked-in or the wait time expires
  for (Ctr = 0; Ctr < WaitTime; ++Ctr) {
    AllCheckInFlag = TRUE; // Assume all CPUs checked-in
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (CheckInFlag[Index] == FALSE) { // If this CPU has not checked-in yet
        Data32 = QpiReadPciCfg (host, Index, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
        if ((Data32 & 0x1) != 0) {  //check bit 0 to indicate AP check in
          CheckInFlag[Index] = TRUE;
        } else {
          AllCheckInFlag = FALSE; // Indicate that at least one CPU hasn't checked-in
          NotCheckedInSocketNum = Index;
        }
      }
    }
    if (AllCheckInFlag == FALSE) {
      QpiFixedDelay(host, 1000); // 1000 microsecs
    } else {
      break;
    }
  }

  //
  // Handle the failure
  //
  if (Ctr == WaitTime) { // If the remote socket(s) hasn't checked-in, assert; it is a fatal condition
    QpiCheckPoint((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0xFF, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE, host);
    QpiDebugPrintFatal((host, QPI_DEBUG_WARN, "\n\n  Remote Socket %u is discovered but hasn't come out of reset. System Halted!!!", NotCheckedInSocketNum)); 
    QPI_ASSERT(FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
  }

  //
  // In LT enabled system, releasing socket2 may cause a LT_SHUTDOWN, which causes a surprise warm reset. 
  // If we mde it here, that is not the case, so clear the bit
  //
  for (Idx2 = 0; Idx2 < MAX_CPU_SOCKETS; ++Idx2) {
    if (SocketData->Cpu[Idx2].Valid == TRUE) {
      Data32 = QpiReadPciCfg(host, Idx2, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
      Data32 = Data32 & ~(1 << 29);
      QpiWritePciCfg(host, Idx2, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG, Data32);
    }
  }


  if (RstType == POST_RESET_POWERGOOD) {
    //
    // Update the number/list of CPUs populated
    //
    GblNidMap0 = 0;         // for socket 0-3
    GblNidMap1 = 0;         // for socket 4-7
    GidNidMap = 0;
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
          if ( Index < 4) {
              GblNidMap0 = GblNidMap0 | (1 << (28 + Index)) | (Index << (4 * Index));
          } else {
              GblNidMap1 = GblNidMap1 | (1 << (28 + (Index - 4))) | (Index << (4 * (Index - 4)));
          } 
          GidNidMap = GidNidMap | (Index << (3 * Index));
      }
    }

    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        // Local Node id
        Data32 = QpiReadPciCfg (host, Index, 0, CPUNODEID_UBOX_CFG_REG);
        Data32 = (Data32 & 0x7)  | Index;
        QpiWritePciCfg (host, Index, 0, CPUNODEID_UBOX_CFG_REG, Data32);

        QpiWritePciCfg (host, Index, 0, GIDNIDMAP_UBOX_CFG_REG, GidNidMap);

        // CpuEnable
        Data32 = (1 << 31) | QpiInternalGlobal->CpuList; // Set the list of CPUs in the system and enable the register
        QpiWritePciCfg (host, Index, 0, CPUENABLE_UBOX_CFG_REG, Data32);

        // Bcastlist
        Data32 = (QpiInternalGlobal->CpuList << 24) | (QpiInternalGlobal->CpuList << 16) |
                 (QpiInternalGlobal->CpuList << 8) | (QpiInternalGlobal->CpuList);
        QpiWritePciCfg (host, Index, 0, BCASTLIST_UBOX_CFG_REG, Data32);

        // CpuNodeID
        QpiWritePciCfg (host, Index, 0, GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG, GblNidMap0);
        if( GblNidMap1 != 0) {
          QpiWritePciCfg (host, Index, 0, GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG, GblNidMap1);
        }
        // Workaround: Cbo sticky register programming requires some dummy read before issuing a reset
        for (Cbo = 0; Cbo < QpiInternalGlobal->TotCbo[Index]; ++Cbo) {
          QpiReadPciCfg(host, Index, Cbo, VID_CBO_CFG_REG);
        }
      }
    }

    //
    // At this point we are in cold-reset flow; now all CPUs can access all other socket's CSRs and thier
    // path to FWH agent is set. Request for reset.
    //
    host->var.common.resetRequired |= POST_RESET_WARM;
    MemCopy ((UINT8 *) host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].TopologyInfo, 
             (UINT8 *) SocketData->SbspMinPathTree, sizeof (host->var.qpi.CpuInfo[QpiInternalGlobal->SbspSoc].TopologyInfo));
  }

  //
  // Read stepping from scratch pad register of each PBSP and store them into sysHost struct
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
  
      Data32 = QpiReadPciCfg (host, Index, 0, BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG);
      //
      // CSR bit[30:27] = CPU Stepping
      //
      host->var.cpu.packageBspStepping[Index] = (UINT8) ((Data32 >> 27) & 0x0f);
      
      //
      // Update common.stepping to the lowest stepping level
      //                                            
      if (host->var.cpu.packageBspStepping[Index] < host->var.common.stepping) {
        host->var.common.stepping = host->var.cpu.packageBspStepping[Index];
        
      }
    }
  }

  return QPI_SUCCESS;
}

QPI_STATUS
ProgramSystemRoute (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:
  Routine to program the final route, VN selection and fanout setting for the given topology.
  Refer to QpiDisc.c file for more info on how the route settings are calcualted.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{

  TOPOLOGY_TREE_NODE  *SbspTree;
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  TreeNode;
  UINT8               TreeIdx;
  UINT8               Ctr, Ctr1, Port;
  UINT8               Nid, ActivePortMask;
  UINT32              Ha4SCODSnpFanOut;
  UINT32              Data32;
  UINT8               sbspSktId;
  UINT32              TempR2pGnCtrl[MAX_CPU_SOCKETS];
  UINT32              NumOfHops[MAX_CPU_SOCKETS];

  R3EGRCTRL_R3QPI_CFG_BDX_STRUCT  R3egrctrl_Bdx;
  R3EGRCTRL_R3QPI_CFG_HSX_STRUCT  R3egrctrl_Hsx;
  UINT32                          R3egrctrl_Bits_pe_mode;
  UINT32                          R3egrctrl_Bits_homsnpspawnenable;

  MemSetLocal ((UINT8 *) &TempR2pGnCtrl, 0x00, sizeof (TempR2pGnCtrl));
  QpiCheckPoint (0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_ROUTE, 0xFF, host);

  sbspSktId = QpiInternalGlobal->SbspSoc;
  SbspTree = SocketData->CpuTree[sbspSktId];

  // Program NonBC registers
  if ((host->var.qpi.RasInProgress != TRUE) || (host->var.qpi.ProgramNonBC == TRUE)) {
    //
    // While the route tables are being programmed, the route is in transient state; since we have to do read-modify-write,
    // we have to read it and remember the register settings so while programming we just modify and write it.
    //
    for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        TempR2pGnCtrl[Ctr] = QpiReadPciCfg (host, Ctr, 0, R2PGNCTRL_R2PCIE_CFG_REG);
      }
    }

    //
    // Program the R2PCIe route table
    //
    for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; ++TreeIdx) {
      TreeNode = SbspTree[TreeIdx];
      if (TreeNode.Node.SocType == SOCKET_TYPE_CPU) {
        Data32  = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG);
        Data32  = (Data32 & 0xFFFF0000) | SocketData->Route[TreeNode.Node.SocId].R2PcieQpiRt;
        
        // When 4S-COD is enabled, R2PCIE_QRT need to be extended to DNID4-7 which correspond to the
        // cluster 1 of S0-S3
        if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
          Data32  = (Data32 & 0xFFFF00FF) | (SocketData->Route[TreeNode.Node.SocId].R2PcieQpiRt << 8);
        } // 4S-COD check
        QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, 0, QPI_RT_R2PCIE_CFG_REG, Data32);
      } else {
        QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
    }

    //
    // Calculate NumberOfHops for Performance Response Encoding
    //
    for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      NumOfHops[Ctr]=0;
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        TopologyTree = SocketData->CpuTree[Ctr];
        for (Ctr1 = 0; TopologyTree[Ctr1].Node.Valid != FALSE; ++Ctr1) {
          NumOfHops[Ctr] |= TopologyTree[Ctr1].Node.Hop << (2 * TopologyTree[Ctr1].Node.SocId);
        }
        NumOfHops[Ctr] |= NumOfHops[Ctr] << 16;
        
        if (host->var.qpi.OutClusterOnDieEn == TRUE) {
          NumOfHops[Ctr] |= (NumOfHops[Ctr] & 0xff0000) << 8;
        }
        QpiWritePciCfg (host, Ctr, 0, NUM_OF_HOPS_CBOBC_COH_REG, NumOfHops[Ctr]);
      }
    }

    for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; ++TreeIdx) {
      TreeNode = SbspTree[TreeIdx];
      if (TreeNode.Node.SocType == SOCKET_TYPE_CPU) {

        //
        // Enable the R2PCIe responses to use the RT for all topologies except 2S
        //
        if ((host->var.qpi.SysConfig != SYS_CONFIG_1S) && (host->var.qpi.SysConfig != SYS_CONFIG_2SEP) && (host->var.qpi.SysConfig != SYS_CONFIG_2SEN)) {
          Data32 = TempR2pGnCtrl[TreeNode.Node.SocId];
          Data32 |= 0x2; // Set qpi_rt_sel_ndr_drs
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, 0, R2PGNCTRL_R2PCIE_CFG_REG, Data32);
        }

        //
        // Program Cbo Route Table; it is non BC register. So we need to program in each Cbo.
        // For 2S 2QPI 2HA case, we need to enable the Cbo interleave mode.
        //
        if (host->var.qpi.SysConfig == SYS_CONFIG_2SEP && QpiInternalGlobal->TotHa[QpiInternalGlobal->SbspSoc] == 2) {
          if (host->var.qpi.OutIsocEn == TRUE) {
            Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, 0, CBO_COH_CONFIG_CBOBC_COH_REG);
            Data32 = Data32 | (1 << 26);
            QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, 0, CBO_COH_CONFIG_CBOBC_COH_REG, Data32);
          }
        }

        for (Ctr = 0; Ctr < QpiInternalGlobal->TotCbo[TreeNode.Node.SocId]; ++Ctr) {
          if (host->var.qpi.OutClusterOnDieEn == TRUE) {

            //
            // Interleave based on Cboid[1]
            //
            if ((Ctr >> 1) % 2) {
              Data32 = SocketData->Route[TreeNode.Node.SocId].CboOddQpiRt;
            } else {
              Data32 = SocketData->Route[TreeNode.Node.SocId].CboEvenQpiRt;
            }
            //
            // Alias for DNID[3] = 1
            //
            Data32 |= Data32 << 16;
          
          } else if (host->var.qpi.OutIsocEn == TRUE) {
            //
            //  HaInterleaving 
            //
            Data32 = 0;
            if ((QpiInternalGlobal->TwistedQpiLinks == TRUE) && ((UINT8) TreeNode.Node.SocId == 1)) { 
              Data32 = 0x1;
            }
          
          } else {
            //
            // Interleave based on ~(cboid[1] ^ cboid[0])
            //
            if ((Ctr % 2) ^ ((Ctr >> 1) % 2)) {
              Data32 = SocketData->Route[TreeNode.Node.SocId].CboEvenQpiRt;
            } else {
              Data32 = SocketData->Route[TreeNode.Node.SocId].CboOddQpiRt;
            }
          
          }

          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPI_ROUTING_TABLE_CBO_CFG_REG, Data32);

          // For 2S topology setup the Peer NodeId in Cbo's QPI RT register
          if (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
            Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPI_XNC_MASK_CBO_CFG_REG);
            (TreeNode.Node.SocId == QpiInternalGlobal->SbspSoc) ? (Data32 = Data32 | (1 << 28)) : (Data32 = Data32 & 0x0FFFFFFF); //peer_nodeid
            QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPI_XNC_MASK_CBO_CFG_REG, Data32);
          }
        }

        //
        // Program Home Agent Route Table & Snoop Fanout
        //
        if (host->var.qpi.RasInProgress == FALSE){
          for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[TreeNode.Node.SocId]; ++Ctr) {

            Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPILUT_HA_CFG_REG);
            Data32 = Data32 & 0xFFFE0000; // Clear qpisrcroute & qpiroute
            if (host->var.qpi.SysConfig == SYS_CONFIG_2SEP) {
              Data32 = Data32 | (1 << 16); // Set qpisrcroute
            } else {
              Data32 = Data32 | SocketData->Route[TreeNode.Node.SocId].HaQpiRt; // Set qpiroute

              // When 4S-COD is enabled, QPILUT need to be extended to DNID4-7 which correspond to the
              // cluster 1 of S0-S3
              if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
                Data32 = (Data32 & 0xFFFF00FF) | (SocketData->Route[TreeNode.Node.SocId].HaQpiRt << 8);
              } // 4S-COD check
            }

            if (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
              Data32 = Data32 | (1 << 16); // Set qpisrcroute
            }

            QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPILUT_HA_CFG_REG, Data32);

            if (host->var.qpi.SysConfig == SYS_CONFIG_2SEP) { 
              Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, HASYSDEFEATURE2_HA_CFG_REG);
              if (host->var.qpi.OutIsocEn == FALSE) {
                Data32 = Data32 | (1 << 16); // Set cboidsnoopintlv to 1  
              }
              
              
              // For the "twisted" 2S topology, make sure S1 HAs swap the odd/even link usage
              if ((QpiInternalGlobal->TwistedQpiLinks == TRUE) && ((UINT8) TreeNode.Node.SocId == 1)) { 
                Data32 = Data32 | (1 << 25); // Set SwitchQPI0vs1 to 1
              } 
              QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, HASYSDEFEATURE2_HA_CFG_REG, Data32);
            }
        
          }
        }
        
        //
        // Program R3QPI Route Table & Vn attribute & Snoop Fanout & pe_mode
        //
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotR3Qpi[TreeNode.Node.SocId]; ++Ctr) {
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiRt[Ctr];
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QQRT_R3QPI_CFG_REG, Data32); //Route Table

          // Program Vn attribute
          // Egress Vn Sel for port0/1 pass through
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiVnSel[Ctr].EgrVnSel;
          // HSX W/A (HSD-4032931):  R3EGRVNSEL should always be programmed to 0xff00ff00 with no exceptions
          // BDX TODO : Whether required or not ?
          Data32 = 0xFF00FF00;

          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QEGR1VNSEL_R3QPI_CFG_REG, Data32);

          //Ingress Vn Sel for port 0
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiVnSel[Ctr].IgrVnSel;
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QIGRVNSEL0_R3QPI_CFG_REG, Data32);

          // Ingress Vn Sel for port 1
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiVnSel[Ctr].IgrVnSel;
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QIGRVNSEL1_R3QPI_CFG_REG, Data32);


          Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QCTRL_R3QPI_CFG_REG);
          Data32 = Data32 | (TreeNode.Node.SocId << 2);    // SocketId

          //TODO: bug to remove to BC 
          if (QpiInternalGlobal->SnoopFanoutEn == TRUE) {
            Data32 = Data32 | (1 << 1);    // snoopfanoutenable
          }
          if (QpiInternalGlobal->Vn1En == TRUE) {
            Data32 = Data32 | (1 << 5);    // vn1enable
          }

          if (host->var.qpi.OutClusterOnDieEn == 1) {
            Data32 = Data32 | (1 << 6);    // codenable
          }

          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3QCTRL_R3QPI_CFG_REG, Data32);

          
          // Retrieve current reg values based on its type
          Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3EGRCTRL_R3QPI_CFG_REG);
          if (host->var.common.cpuType == CPU_HSX) {
            R3egrctrl_Hsx.Data = Data32;
            R3egrctrl_Bits_pe_mode             = R3egrctrl_Hsx.Bits.pe_mode;
            R3egrctrl_Bits_homsnpspawnenable   = R3egrctrl_Hsx.Bits.homsnpspawnenable;
          } else {  //CPU_BDX
            R3egrctrl_Bdx.Data = Data32;
            R3egrctrl_Bits_pe_mode             = R3egrctrl_Bdx.Bits.pe_mode;
            R3egrctrl_Bits_homsnpspawnenable   = R3egrctrl_Bdx.Bits.homsnpspawnenable;
          }

          R3egrctrl_Bits_pe_mode = 0;                // clear pe_mode bits

          //
          //   IODC was supported on IVT.  HSX DOES NOT support it. BDX again has support for the same (Not exact as IVT though).
          //   Should always be 0b00 if Isoc and MeSeg is disabled, and it should always be 0b11 if Isoc or Meseg is enabled. 
          //   Unlike IVT IODC, for BDX, there is no case of pe_mode as 2.
          //
          if ((host->var.qpi.OutIsocEn == 1) || (host->var.qpi.OutMesegEn == 1)) {
            R3egrctrl_Bits_pe_mode = 3;       
          } else {
            R3egrctrl_Bits_pe_mode = 0;    
          }

          // Grantley BDX HSD 5002811 : Clone from BDX Sighting:DCA clusterid and PE bit for MEseg both overloaded 
          // on DNID bit 3 causing akrtidmiss due to dnid corruption
          // Override the pe_mode to 2 (instead of 3) when COD and MeSeg are enabled together
          if (host->var.common.cpuType == CPU_BDX) {
            if ((host->var.qpi.OutMesegEn == 1) && (host->var.qpi.OutClusterOnDieEn == 1)) {
              R3egrctrl_Bits_pe_mode = 2;
            } // COD + MeSeg check
          } // cpuType check

          if ((QpiInternalGlobal->SysSnoopMode != EARLY_SNOOP_MODE) || (host->var.qpi.SysConfig == SYS_CONFIG_1S)) { 
            R3egrctrl_Bits_homsnpspawnenable = 0;
          }

          // Program the actual reg based on its type
          if (host->var.common.cpuType == CPU_HSX) {
            R3egrctrl_Hsx.Bits.pe_mode             = R3egrctrl_Bits_pe_mode;
            R3egrctrl_Hsx.Bits.homsnpspawnenable   = R3egrctrl_Bits_homsnpspawnenable;
            Data32 = R3egrctrl_Hsx.Data;
          } else {  //CPU_BDX
            R3egrctrl_Bdx.Bits.pe_mode             = R3egrctrl_Bits_pe_mode;
            R3egrctrl_Bdx.Bits.homsnpspawnenable   = R3egrctrl_Bits_homsnpspawnenable;
            Data32 = R3egrctrl_Bdx.Data; 
          }
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3EGRCTRL_R3QPI_CFG_REG, Data32); 
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, NUMOFHOPSCR_P0_R3QPI_CFG_REG, NumOfHops[TreeNode.Node.SocId]);

        }//For each R3QPI

        //
        // Program QPI Agent Route Table
        //
        for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
          if (SocketData->Cpu[TreeNode.Node.SocId].LinkData[Port].Valid == TRUE) {
            Data32  = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Port, QPIQRT_QPIAGENT_LL_REG);
            Data32  = (Data32 & 0xFFFF0000) | SocketData->Route[TreeNode.Node.SocId].QpiAgentRt[Port];
            if (host->var.qpi.SysConfig < SYS_CONFIG_4S) {
              Data32 &= 0xFFFF00FF; //Clear Inhibit_write field for 1S and 2S
            }

            if (QpiInternalGlobal->Is4SRing) {
              Data32 |= (1 << (TreeNode.Node.SocId+8));
            }

           
            //This is specific for IVT only
            //The D2C related info needs to be updated only at the requesting socket. The way the h/w determines that is by comparing the RHNID to the local socket id. 
            //There is no field in QPIAGENT that conveys the local socket id. The way QPIAGENT gets that info is from QPIQRT.local_vs_remote field.
            //At this point,  we set only fields corresponding to the neighbor/remote NID to 1. All other fields are set 0. 
            //However the h/w expects only one bit  corresponding to the local socket ID to be 0; all other fields including the NID corresponding to unpopulated socket must be 1.
            Data32 |= (UINT32) ((~(1 << TreeNode.Node.SocId)) & 0x000000FF);           

            // Permanent IVT W/A (be4572841,s3616514,s4030383): CLONE from ivytown: R3 DNID[3]/PE[0] bit codec fix with pe_mode[0] configuration. 
            // Summary: Except for 8S configurations, the Port-0 and Port-1 routing tables each need to be modified 
            // so that the S0-S3 routes are repeated in the entries for the S4-S7 routes even. Both R3QPI and QPIAGENT need the same workaround.
            if(host->var.qpi.SysConfig < SYS_CONFIG_8S) {
             Data32 = (Data32 & 0xFFFFFF0F) | ((Data32 & 0x0000000F) << 4);
            }

            //
            // Set 'inhibit_write' bits when D2C is disabled e.g. BtMode2-128/48 (Alt-1) on 8S on HSX-D0+.
            //
            if (QpiInternalGlobal->D2cEn == FALSE) {
              Data32 = (Data32 & 0xFFFF00FF) | (0xFF << 8);
            }
            QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Port, QPIQRT_QPIAGENT_LL_REG, Data32);
          }
        }
      } else {
        QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
      }
    }
  }  

  // Program BC registers
  if ((host->var.qpi.RasInProgress != TRUE) || (host->var.qpi.ProgramBC == TRUE)) {
    if ( QpiInternalGlobal->SnoopFanoutEn == TRUE ) {
    
      for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; ++TreeIdx) {
        TreeNode = SbspTree[TreeIdx];
        //
        // Program Home Agent Route Table & Snoop Fanout
        //
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[TreeNode.Node.SocId]; ++Ctr) {
           Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, SNPFANOUT_CONFIG_HA_CFG_REG);
           if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
             Ha4SCODSnpFanOut = 0;
             ActivePortMask = 0;
             // We find the number of active ports in a given socket
             for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
               if (SocketData->Cpu[TreeNode.Node.SocId].LinkData[Port].Valid == TRUE) {
                 ActivePortMask |= (1 << Port);
               }
             } // Port for loop
             
             // For 4S-COD, we need to send the snoops on all active links so we copy 
             // the active ports from DNID 0 to DNID 7
             for (Nid=0; Nid < 8; Nid++) {
               Ha4SCODSnpFanOut |= (ActivePortMask << (Nid * 3));    // We have 3 bits for port mask
             } // NID for loop
             
             Data32 = Data32 | (Ha4SCODSnpFanOut << 5) | 0x1;
           } else {
             Data32 = Data32 | SocketData->Route[TreeNode.Node.SocId].HaQpiFanOut | 0x1; // Set enablefanout and send2qpiports_nx
           } // 4S-COD check
           
           QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, SNPFANOUT_CONFIG_HA_CFG_REG, Data32);                  
        }          
        //
        // Program R3QPI Route Table & Vn attribute & Snoop Fanout & pe_mode
        //
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotR3Qpi[TreeNode.Node.SocId]; ++Ctr) {
          // Snoop Fanout for P0
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiSnpFanOut[Ctr][0];
          if (host->var.qpi.OutClusterOnDieEn == TRUE) {
            // Handle COD use of DNID[3]
            Data32 = Data32 | ((Data32 & 0xfff) << 12);
          }
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3SNPFANOUT_P0_R3QPI_CFG_REG, Data32); 
      
          // Snoop Fanout for P1
          Data32  = SocketData->Route[TreeNode.Node.SocId].R3QpiSnpFanOut[Ctr][1];
          if (host->var.qpi.OutClusterOnDieEn == TRUE) {
            // Handle COD use of DNID[3]
            Data32 = Data32 | ((Data32 & 0xfff) << 12);
          }
          QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, R3SNPFANOUT_P1_R3QPI_CFG_REG, Data32);
        }   
      } 
    }  

    //
    // Program Home Agent number of active links
    //
    for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; ++TreeIdx) {
      TreeNode = SbspTree[TreeIdx];
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[TreeNode.Node.SocId]; ++Ctr) {
         Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, SNPFANOUT_CONFIG_HA_CFG_REG);
         Data32 &= 0xFFFFFFE3;
         for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
          if (SocketData->Cpu[TreeNode.Node.SocId].LinkData[Port].Valid == TRUE) {
            Data32 |= (1 << 2) << Port;
          }
         }
         QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, SNPFANOUT_CONFIG_HA_CFG_REG, Data32);                  
      }          
    }
    
    // If ras in progress, program HA route table 
    if (host->var.qpi.RasInProgress == TRUE){
      for (TreeIdx = 0; SbspTree[TreeIdx].Node.Valid == TRUE; ++TreeIdx) {
        TreeNode = SbspTree[TreeIdx];
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[TreeNode.Node.SocId]; ++Ctr) {
           Data32 = QpiReadPciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPILUT_HA_CFG_REG);
           Data32 = Data32 & 0xFFFE0000; // Clear qpisrcroute & qpiroute
           Data32 = Data32 | SocketData->Route[TreeNode.Node.SocId].HaQpiRt; // Set qpiroute
           
           // TODO : Check on this requirement on 4S-COD with CPU onlining
           // When 4S-COD is enabled, QPILUT need to be extended to DNID4-7 which correspond to the
           // cluster 1 of S0-S3
           //if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
           //  Data32 = (Data32 & 0xFFFF00FF) | (SocketData->Route[TreeNode.Node.SocId].HaQpiRt << 8);
           //} // 4S-COD check
           QpiWritePciCfg (host, (UINT8) TreeNode.Node.SocId, Ctr, QPILUT_HA_CFG_REG, Data32);                             
        }  
      } 
    }
  }

  return QPI_SUCCESS;
}


QPI_STATUS
SetupSystemIoSadEntries (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal 
  )
/*++

Routine Description:
  Routine to program the final IO SAD setting for the given topology.

Arguments:
  
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8                 Index;
  UINT32                IoTgtLst, ApicTgtLst, MmioTgtLst;

  //
  // Construct the 8-entry target list for IO/IOAPIC/MMIO decoders
  //
  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_FILL_SAD_TGTLST, host);
  FillSadTargetList(host, SocketData, QpiInternalGlobal, &IoTgtLst, &ApicTgtLst, &MmioTgtLst);

  //
  // Setup all the IO SAD & Address Map related registers for all sockets
  //
  for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      QpiCheckPoint((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0xFF, STS_PROGRAM_FINAL_IO_SAD, MINOR_STS_PROGRAM_CPU_SAD_ENTRIES, host);
      ProgramCpuIoSadEntries (host, SocketData, QpiInternalGlobal, Index, IoTgtLst, ApicTgtLst, MmioTgtLst);
      ProgramSystemAddressMap(host, SocketData, Index);
    }
  }

  return QPI_SUCCESS;
}


QPI_STATUS
ProgramCpuIoSadEntries (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8                  SocId,
  UINT32                 IoTgtLst,
  UINT32                 ApicTgtLst,
  UINT32                 MmioTgtLst
  )
/*++

Routine Description:
  Routine to program the final IO SAD setting for the given topology.

Arguments:
  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data
  SocId - CPU socket to be programmed
  IoTgtLst   - Legacy IO region target list
  ApicTgtLst   - IO APIC region target list
  MmioTgtLst   - MMIOL/H region target list

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8                 Idx1, LastMmiolRule;
  UINT32                Data32;
  
  UINT32                MmioRuleOffset[16][2] = {{MMIO_RULE_0_N0_CBOBC_IOSAD_REG, MMIO_RULE_0_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_1_N0_CBOBC_IOSAD_REG, MMIO_RULE_1_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_2_N0_CBOBC_IOSAD_REG, MMIO_RULE_2_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_3_N0_CBOBC_IOSAD_REG, MMIO_RULE_3_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_4_N0_CBOBC_IOSAD_REG, MMIO_RULE_4_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_5_N0_CBOBC_IOSAD_REG, MMIO_RULE_5_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_6_N0_CBOBC_IOSAD_REG, MMIO_RULE_6_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_7_N0_CBOBC_IOSAD_REG, MMIO_RULE_7_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_8_N0_CBOBC_IOSAD_REG, MMIO_RULE_8_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_9_N0_CBOBC_IOSAD_REG, MMIO_RULE_9_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_10_N0_CBOBC_IOSAD_REG, MMIO_RULE_10_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_11_N0_CBOBC_IOSAD_REG, MMIO_RULE_11_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_12_N0_CBOBC_IOSAD_REG, MMIO_RULE_12_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_13_N0_CBOBC_IOSAD_REG, MMIO_RULE_13_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_14_N0_CBOBC_IOSAD_REG, MMIO_RULE_14_N1_CBOBC_IOSAD_REG},
                                                 {MMIO_RULE_15_N0_CBOBC_IOSAD_REG, MMIO_RULE_15_N1_CBOBC_IOSAD_REG}};
 
  // Define the two pointers that list the CSR offset values for the
  // MMIO Rules that are assigned to MMIOL and MMIOH, based on EP vs. EX.
  // Each pointer points to a list of 1-dimentional arrays each consisting of
  // exactly 2 UINT32 elements which contain an MMIO Rule offset.
  UINT32                (*MmiolRuleOffset)[2];
  UINT32                (*MmiohRuleOffset)[2];
                                                 
  MMIO_RULE_0_N0_CBOBC_IOSAD_STRUCT  RuleLo;  
  MMIO_RULE_0_N1_CBOBC_IOSAD_STRUCT  RuleHi;
  
  // Grab the right set of MMIO CSR offset values based on EP vs EX.
  MmiolRuleOffset = &(MmioRuleOffset[0]); //EP/EX MMIO-L starts with Rule-0
  if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX)) || 
      ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType != CPU_BDX_EX))) {
    MmiohRuleOffset = &(MmioRuleOffset[4]); //EP MMIO-H starts with Rule-4
    LastMmiolRule = 3;
  } else {
    MmiohRuleOffset = &(MmioRuleOffset[8]); //EX MMIO-H starts with Rule-8
    LastMmiolRule = 7;
  }  
  
  // Legacy IO SAD target list
  QpiWritePciCfg (host, SocId, 0, IOPORT_TARGET_LIST_CBOBC_IOSAD_REG, IoTgtLst);

  // IO APIC SAD target list
  QpiWritePciCfg (host, SocId, 0, IOAPIC_TARGET_LIST_CBOBC_IOSAD_REG, ApicTgtLst);

  // Here is default setting
  // EP uses MMIO rules 0-3 for MMIOL region of CPUs 0-3 and rules 4-7 for MMIOH region of CPUs 0-3.
  // EX uses MMIO rules 0-7 for MMIOL region of CPUs 0-7 and rules 8-15 for MMIOH region of CPUs 0-7.
  // The last MMIOL rule (IVT-EP Rule-3 or IVT-EX Rule-7) is setup by CPURC to map the 4G to 4G-64M region.
  // In a fully populated system (4S for IVT-EP or 8S for IVT-EX), Rule-3/Rule-7 will simply be extended to
  // cover the MMIOL region allocated to Socket-3/Socket-7 plus the MMCFG region if MMCFG is placed between
  // MMIOL and 4G.  In other configs, since Rule-3/Rule-7 is not used to cover any MMIOL region, it can
  // continue to cover the 4G-64M to 4G region.
  //
  RuleLo.Data = 1; // Enable the rule
  RuleHi.Data = 0;                                 
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {

      // MMIOL Rule
      if (host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase < host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit) {
        RuleLo.Bits.baseaddress_lsb_bits = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase >> 24;
        RuleLo.Bits.baseaddress = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolBase >> 26;
        RuleLo.Bits.limitaddress_lsb_bits = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit >> 24;
        RuleLo.Bits.limitaddress = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit >> 26;
        if ((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiolLimit == MMIOL_LIMIT) && (QpiInternalGlobal->CpuList & (1<< LastMmiolRule )) ){
          //
          // Make sure the last rule (Rule-3/Rule-7) as default setting or oem special rule setup reaches MMIOL_LIMIT if the last socket presents
          // covers the last socket's MMIOL space and all the way to 4G.
          //
          RuleLo.Bits.limitaddress_lsb_bits = 0x3;
          RuleLo.Bits.limitaddress = 0x3F;
        }
        QpiWritePciCfg (host, SocId, 0, MmiolRuleOffset[Idx1][1], 0); // This will be zero for all MMIOL regions since it is below 4G
        QpiWritePciCfg (host, SocId, 0, MmiolRuleOffset[Idx1][0], RuleLo.Data);
      }

      // MMIOH Rule
      RuleLo.Bits.baseaddress_lsb_bits = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.lo >> 24;
      RuleLo.Bits.baseaddress = (host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.lo >> 26) | ((host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohBase.hi & 0x3FFF) << 6);
      RuleLo.Bits.limitaddress_lsb_bits = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.lo >> 24;
      RuleLo.Bits.limitaddress = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.lo >> 26;
      RuleHi.Bits.limitaddress = host->var.qpi.CpuInfo[Idx1].CpuRes.MmiohLimit.hi & 0x3FFF;
      QpiWritePciCfg (host, SocId, 0, MmiohRuleOffset[Idx1][1], RuleHi.Data);
      QpiWritePciCfg (host, SocId, 0, MmiohRuleOffset[Idx1][0], RuleLo.Data);

    } else if ((host->var.qpi.RasInProgress == TRUE) && (host->var.qpi.RasEvtType == QPIRAS_EVNT_OFFLINE)) {
       //
       // When a socket gets offline, it needs to be reflected in MMIO CSR's for SBSP and all other sockets that are remaining active.
       // DoTo: NEM uses the above: the last rule (Rule-3/Rule-7) covers the last socket's MMIOL space and all the way to 4G.
       //       if socket3/7 gets offline, it is gone anyways. 
       QpiWritePciCfg (host, SocId, 0, MmiolRuleOffset[Idx1][1], 0);
       QpiWritePciCfg (host, SocId, 0, MmiolRuleOffset[Idx1][0], 0);
       QpiWritePciCfg (host, SocId, 0, MmiohRuleOffset[Idx1][1], 0);
       QpiWritePciCfg (host, SocId, 0, MmiohRuleOffset[Idx1][0], 0);
    }

  }

  // Program the target list for Rules 0-7 and Rules 8-15.  Target list parameter (passed in)
  // contains settings for Rules 0-7.   
  // For EP, Rules 0-7 used for MMIOL and MMIOH.  Rules 8-15 not used. 
  // For EX, Rules 0-7 used for MMIOL.  Rules 8-15 used for MMIOH.  Same target list for both.
  QpiWritePciCfg (host, SocId, 0, MMIO_TARGET_LIST_0_CBOBC_IOSAD_REG, MmioTgtLst);  //Rules 0-7

  if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) || 
      ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX))) {
    QpiWritePciCfg (host, SocId, 0, MMIO_TARGET_LIST_1_CBOBC_IOSAD_REG, MmioTgtLst); //EX Rules 8-15
  } else {
    QpiWritePciCfg (host, SocId, 0, MMIO_TARGET_LIST_1_CBOBC_IOSAD_REG, 0);  //EP Rules 8-15
  }

  //
  // SAD Target - program the legacy VGA range target id, SourceID and Enable the SourceID featrue
  //
  Data32 = QpiReadPciCfg (host, SocId, 0, SAD_TARGET_CBOBC_IOSAD_REG);
  Data32 = (Data32 & 0xFFFFFE3F) | (host->var.qpi.OutLegacyVgaSoc << 6);
  Data32 = (Data32 & 0xFFFFF1FF) | (SocId << 9) | BIT12;
  QpiWritePciCfg (host, SocId, 0, SAD_TARGET_CBOBC_IOSAD_REG, Data32);

  //
  // SAD Control - program the local socket id (node id)
  //
  QpiWritePciCfg (host, SocId, 0, SAD_CONTROL_CBOBC_IOSAD_REG, SOCID_TO_NID(SocId, 0));

  return QPI_SUCCESS;
}


QPI_STATUS
ProgramSystemAddressMap (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  UINT8                  SocId
  )
/*++

Routine Description:
  Routine to program the IIO registers related to system address space.

Arguments:
  host        - Input/Output structure of the QPIRC
  SocketData  - CPU socket info
  SocID       - processor socket ID

Returns:
  
  0 - Successful
  Other - failure

--*/
{
  UINT8               MmiolBase;
  UINT8               MmiolLimit;   
  LMMIOH_BASE_N0_IIO_VTD_STRUCT            MmiohBaseRegLo = {0};
  LMMIOH_BASE_N1_IIO_VTD_STRUCT            MmiohBaseRegHi = {0};
  LMMIOH_LIMIT_N0_IIO_VTD_STRUCT           MmiohLimitRegLo = {0};
  LMMIOH_LIMIT_N1_IIO_VTD_STRUCT           MmiohLimitRegHi = {0};

  if (host->setup.qpi.MmioP2pDis == TRUE) {
    //
    // Set the Base > Limit to disable the P2P traffic
    //
    MmiolBase = 0xFF;
    MmiolLimit = 0;
    MmiohBaseRegLo.Bits.base = 0x3F;     //Init  6-bit field to its max value
    MmiohBaseRegHi.Bits.base = 0x7FFFF;  //Init 19-bit field to its max value
    MmiohLimitRegLo.Bits.limit = 0;
    MmiohLimitRegHi.Bits.limit = 0;
  } else {
    MmiolBase = (UINT8) (host->var.qpi.CpuInfo[SocId].CpuRes.MmiolBase >> 24);
    MmiolLimit = (UINT8) (host->var.qpi.CpuInfo[SocId].CpuRes.MmiolLimit >> 24);
    MmiohBaseRegLo.Bits.base = (UINT16)(host->var.qpi.CpuInfo[SocId].CpuRes.MmiohBase.lo >> 26);
    MmiohBaseRegHi.Bits.base = host->var.qpi.CpuInfo[SocId].CpuRes.MmiohBase.hi;
    MmiohLimitRegLo.Bits.limit = (UINT16)(host->var.qpi.CpuInfo[SocId].CpuRes.MmiohLimit.lo >> 26);
    MmiohLimitRegHi.Bits.limit = host->var.qpi.CpuInfo[SocId].CpuRes.MmiohLimit.hi;
  }

  //
  // Local MMIOL range register
  //
  QpiWritePciCfg (host, SocId,  0, LMMIOL_BASE_IIO_VTD_REG,  (MmiolBase << 8));
  QpiWritePciCfg (host, SocId,  0, LMMIOL_LIMIT_IIO_VTD_REG, (MmiolLimit << 8));

  //
  // Local MMIOH range register
  //
  QpiWritePciCfg (host, SocId, 0, LMMIOH_BASE_N0_IIO_VTD_REG, MmiohBaseRegLo.Data);
  QpiWritePciCfg (host, SocId, 0, LMMIOH_BASE_N1_IIO_VTD_REG, MmiohBaseRegHi.Data);
  QpiWritePciCfg (host, SocId, 0, LMMIOH_LIMIT_N0_IIO_VTD_REG, MmiohLimitRegLo.Data);
  QpiWritePciCfg (host, SocId, 0, LMMIOH_LIMIT_N1_IIO_VTD_REG, MmiohLimitRegHi.Data);

  return QPI_SUCCESS;
}


QPI_STATUS
SetupHaCredits (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  SetupHaCreditsHsx(host, SocketData, QpiInternalGlobal);
  return QPI_SUCCESS;
}                     


QPI_STATUS
SetupHaCreditsHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8           Idx1, Ctr, Port, ConfigType, CreditPerCbo, NumofActiveQpiLink;
  UINT32          Data32 = 0, Data32_1;
  UINT32          BlVnaCrtType[SI_MAX_QPI_PORTS] = {0x52800, 0x52900, 0x52D00}; // sharcrdtrls, wren, crdttype
  UINT32          AdVnaCrtType[SI_MAX_QPI_PORTS] = {0x52A00, 0x52B00, 0x52E00}; // sharcrdtrls, wren, crdttype
  HACRDTCNT_HA_CFG_STRUCT      HaCrdtCnt = {0};
  MCCRDTTHRLD_HA_CFG_STRUCT  McCrdtThrld;
  HAVNACRDT_HA_CFG_HSX_BDX_STRUCT  HaVnaCrdt = {0};
  BT_CONFIG_HA_CFG_HSX_BDX_STRUCT  BtConfig;
  CreditTablePara   *CreditTableParam = NULL;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_HA_CRDT, host);

  // Identify the config type
  ConfigType = QpiInternalGlobal->SysConfigType;
  if (host->var.common.cpuType == CPU_HSX) {
    CreditTableParam = &CreditTableHsx[ConfigType];
  } else {
    CreditTableParam = &CreditTableBdx[ConfigType];
  }

  if (CreditTableParam == NULL) {
    QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE);
  }

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    //if this socket id is valid. 
    //For hot plug case, because number of active links is changed. we need to reprogram this portion for all valid sockets
    if ((host->var.qpi.RasInProgress == FALSE && SocketData->Cpu[Idx1].Valid == TRUE) || (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Idx1)) {
      // calculate the number of active QPI links, in order to program "numhtslotsrsvd_drs" field
      NumofActiveQpiLink = 0;
      for (Ctr = 0; Ctr < MAX_QPI_PORTS; ++Ctr) {       
         if (SocketData->Cpu[Idx1].LinkData[Ctr].Valid == TRUE) {
            NumofActiveQpiLink ++;
         }
      }

      for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[Idx1]; ++Ctr) {
        // 
        // For all topologies except 1S, program the AD/BL VNA credits in HA for the QPI links.
        // To program the credits, we need to set "sharcrdtrls" and "wren" fields to 1, set the
        // "crdttype" field to the credit type, and set "maincnt" to the crdit value.
        // 
        if (host->var.qpi.SysConfig != SYS_CONFIG_1S) {
          for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
            Data32 = BlVnaCrtType[Port] | CreditTableParam->BlVnaCrdt;
            Data32_1 = AdVnaCrtType[Port] | CreditTableParam->AdVnaCrdt;

            // s4167023 VNA credit workaround required for A0 presi bug 241466.
            //A0 stepping use C87 "VN1 Fix" value. B0 stepping use C87 "BL B0 Fix" value
            if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {             
              if(QpiInternalGlobal->SysConfigType == EXA_8S_HCC) {
                Data32 = BlVnaCrtType[Port] |  2;             
              } else if(QpiInternalGlobal->SysConfigType == EP_4S_HCC) {
                Data32 = BlVnaCrtType[Port] | 11;
              }
            }

            if (QpiInternalGlobal->SysConfigType == EXA_4S_HCC) {
              if (QpiInternalGlobal->Is4SRing == TRUE) {
                //
                // Use 4S Ring credits from EP config table
                //
                Data32 = BlVnaCrtType[Port] | 10;
                Data32_1 = AdVnaCrtType[Port] | 11;
              }
            } // EXA_4S_HCC

            QpiWritePciCfg (host, Idx1, Ctr, HACRDTCNT_HA_CFG_REG, Data32);
            QpiWritePciCfg (host, Idx1, Ctr, HACRDTCNT_HA_CFG_REG, Data32_1);
          }//For each QPI Port

          // Permanent HSW W/A (s4167885):  Don't use default allocation for HA Type-13 credits.
          // Permanent HSW W/A (s4168214):  CLONE from HSX: HA can deadlock when SBO AD credits < 2 * AD_SNP egress credits
          // Permanent HSW W/A (s4986655,s4969444):  HSX Clone: EX: Memic hangs with BT TO, TOR TO, and 3-Strike
          HaCrdtCnt.Data = 0x00050D00;    //Prep for write: crdttype=13, sharecrdtrls=1, wren=1
          if ((QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) || (host->var.qpi.OutIsocEn == TRUE)) {
            HaCrdtCnt.Bits.maincnt = 1;     //Set new credit allocation for Early Snoop or isoc mode
          } else {
            HaCrdtCnt.Bits.maincnt = 2;     //Set new credit allocation
          }
          QpiWritePciCfg (host, Idx1, Ctr, HACRDTCNT_HA_CFG_REG, HaCrdtCnt.Data);

          HaVnaCrdt.Data = QpiReadPciCfg (host, Idx1, Ctr, HAVNACRDT_HA_CFG_REG);
          HaVnaCrdt.Bits.snpipqcreditcnt =  CreditTableParam->Ipq;
          if (host->var.qpi.OutIsocEn == TRUE) {
            HaVnaCrdt.Bits.advnacriticalthrshld = 0;
            HaVnaCrdt.Bits.blvnacriticalthrshld = 0;     
            HaVnaCrdt.Bits.advnahighthrshld = 0;
            HaVnaCrdt.Bits.blvnahighthrshld = 0;
            
            if (host->var.qpi.SysConfig != SYS_CONFIG_1S) {
              HaVnaCrdt.Bits.advnahighthrshld = 2;
              HaVnaCrdt.Bits.blvnahighthrshld = 1;
            }
          }//Isoc mode
          QpiWritePciCfg (host, Idx1, Ctr, HAVNACRDT_HA_CFG_REG, HaVnaCrdt.Data);
        }  //Not SYS_CONFIG_1S

        //
        // Set Number of HT entries for ReservedHT, ReservedWBHT, ReservedAckCnfltHT
        //
        
        BtConfig.Data = QpiReadPciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG);
        if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
          BtConfig.Data = QpiReadPciCfg (host, Idx1, 0, BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG);
        }  
        BtConfig.Data &= (~(FLD_MASK(15, 4)));                                              // Clear all the fields bit[4-18] 
        BtConfig.Data |= 0x1210;      // bit4 =1, Bits[8:5]=0 / Bits[9]=1, Bits[11:10]=0 / Bits[12:12]=1, Bits[18:13] =0 
       
        if (QpiInternalGlobal->BtModeEn == TRUE) {
          BtConfig.Bits.numhtslotsrsvd_drs = CreditTableParam->Qpi_To_Ha_WB_Credit * NumofActiveQpiLink;                       //for Bits[8:5]
          BtConfig.Bits.numhtslotsrsvd_ackcnflt = 1;                                       //for Bits[11:10], ToDo: check with DE 

          // Brickland BDX HSD 4971313 : Clone from BDX Sighting:BDX EX : BT TO seen on 24C EX sku parts with memicals traffic passes on 23C EX
          // Updated to BDX- Buffer Credit Sheet v.4.6
          if (host->var.common.cpuType == CPU_HSX) {
            if (QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] > 12) {  // For HCC
              CreditPerCbo = 2;
            } else {                                                           // For MCC & LCC 
              CreditPerCbo = 3;
            }             
          } else { // For CPU_BDX
            CreditPerCbo = (UINT8)(TOT_CBO_CREDIT_BDX/QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc]);

            // Saturate CreditPerCbo at 3
            if (CreditPerCbo > 3) {
              CreditPerCbo = 3;
            }
          } // cpuType check

          BtConfig.Bits.numhtslotsrsvd_localreq = CreditPerCbo * QpiInternalGlobal->TotCbo[Idx1];  // program Bits[18:13] = #Cbo*credit/Cbo =  (# of Cbo) * 2  
        } else {  
          BtConfig.Bits.numhtslotsrsvd_localreq = 0;
        }
        if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) ||
            (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX)) {
          // If BT Mode already enabled, need to swizzle 'btenable' bit in order for new values to take affect.
          if (BtConfig.Bits.btenable) {
            BtConfig.Bits.btenable = 0;
            QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);
            BtConfig.Bits.btenable = 1;
          }
        } // cpuType check
        QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);

        if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
          QpiWritePciCfg (host, Idx1, 0, BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG, BtConfig.Data);
        }  
        


        //
        // For topologies that support ISOC, program the HA->iMC credits, as needed.
        // For all other topologies, deallocate the credits.
        // HA->iMC credits include 'high' and 'critical' priority 'reads' and 'writes'.
        // Assumption - Reg layout:  highrd=Bits[15:12], critrd=Bits[11:8], highwr=Bits[7:4], critwr=Bits[3:0]
        //
        McCrdtThrld.Data = QpiReadPciCfg (host, Idx1, Ctr, MCCRDTTHRLD_HA_CFG_REG);
        McCrdtThrld.Data &= ~(0x0000FFFF);  //Clear all 4 HA->iMC ISOCH credits (i.e. assume no ISOC) 
        if (host->var.qpi.OutMesegEn == TRUE) {
          McCrdtThrld.Data |= 0x0101;        
        } else if (host->var.qpi.OutIsocEn == TRUE) { // For ISOC, everything is converted to critical, 
            McCrdtThrld.Data |= 0x0B040;        
        }
        QpiWritePciCfg (host, Idx1, Ctr, MCCRDTTHRLD_HA_CFG_REG, McCrdtThrld.Data);

      }// for HA
    } //if socket valid
  } // for sockets
  
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    //if this socket id is valid. 
    //For hot plug case, because number of active links is changed. we need to reprogram this portion for all valid sockets
    if ((host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId != Idx1)) {
      // calculate the number of active QPI links, in order to program "numhtslotsrsvd_drs" field
      NumofActiveQpiLink = 0;
      for (Ctr = 0; Ctr < MAX_QPI_PORTS; ++Ctr) {       
         if (SocketData->Cpu[Idx1].LinkData[Ctr].Valid == TRUE) {
            NumofActiveQpiLink ++;
         }
      }
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[Idx1]; ++Ctr) {
        BtConfig.Data = QpiReadPciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG);
        if (QpiInternalGlobal->BtModeEn == TRUE) {
          BtConfig.Bits.numhtslotsrsvd_drs = CreditTableParam->Qpi_To_Ha_WB_Credit * NumofActiveQpiLink;                       //for Bits[8:5]
        }


        // If BT Mode already enabled, need to swizzle 'btenable' bit in order for new values to take affect.
        if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) ||
            (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX)) {
          if (BtConfig.Bits.btenable) {
            BtConfig.Bits.btenable = 0;
            QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);
            BtConfig.Bits.btenable = 1;
          }
        } // cpuType check
        QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);
      }//For each HA
    }//Existing socket during RAS
  }//For each Socket
  return QPI_SUCCESS;
}



QPI_STATUS
ConfigureOsbSetting(
  struct sysHost            *host,
    QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
    UINT8  socket,
    UINT8  ha)
{
  OSB_CONFIG_HA_CFG_STRUCT  OsbConfig;

  OsbConfig.Data = QpiReadPciCfg (host, socket, ha, OSB_CONFIG_HA_CFG_REG);      
  if (QpiInternalGlobal->SysSnoopMode != HOME_DIR_OSB_SNOOP_MODE) {
    // Not running "DIR OSB SNOOP" mode so disable all OSB features.
    OsbConfig.Bits.osbloi2e = 0;
    OsbConfig.Bits.osblord  = 0;
    OsbConfig.Bits.osbrmt   = 0;
  } else {
    // Running "DIR OSB SNOOP" mode so enable all OSB features and customize as needed.
    OsbConfig.Bits.osbloi2e = 1;

    // HSD 4970072 : PerfWG: Change OSB AD VNA threshold from 8 to 6
    if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType == CPU_HSX_EX)) || 
        ((host->var.common.cpuType == CPU_BDX) && (host->var.common.cpuSubType == CPU_BDX_EX))) {
      OsbConfig.Bits.loi2eadvnath = 6;
    } else { // For non-EX- SKU
      //
      // Grantley BDX HSD 5002773 : Perf: OSB tuning
      // Note : osbloi2e is enabled by default, using loi2eadvnath as 6 for BDX- 2SEP-COD configs,
      // 3 for BDX- 2S- non-COD configs and 8 for all others (includes HSX- all configs)
      //
      if ((host->var.common.cpuType == CPU_BDX) && (host->var.qpi.SysConfig == SYS_CONFIG_2SEN || host->var.qpi.SysConfig == SYS_CONFIG_2SEP) &&
          (host->var.qpi.OutClusterOnDieEn == FALSE)) {
        OsbConfig.Bits.loi2eadvnath = 3;    // Specific to BDX-EP- only and not applicable to BDX-EX-
      } else if ((host->var.common.cpuType == CPU_BDX) && (host->var.qpi.SysConfig == SYS_CONFIG_2SEP) && (host->var.qpi.OutClusterOnDieEn == TRUE)) {
        OsbConfig.Bits.loi2eadvnath = 6;    // Specific to BDX-EP- only and not applicable to BDX-EX-
      } else if (host->var.common.cpuType == CPU_BDX && host->var.qpi.SysConfig == SYS_CONFIG_4S) {
        // Grantley BDX HSD 5003326: [Perf: Request to update osb_config.loi2eadvnath to 1 for 4S EP default snoop mode and COD 
        OsbConfig.Bits.loi2eadvnath = 1;                     
      } else {
        OsbConfig.Bits.loi2eadvnath = 8;    // Default value for HSX-EP- and BDX-EP-
      } // cpuType, config check
    } // EX- SKU check

    // HSD 5002118 : Switch default 2s snoop mode to Dir + ivt style OSB (no CoD)
    // Applicable to BDX- only
    if ((host->var.common.cpuType == CPU_BDX) && (host->var.qpi.OutHomeDirWOSBEn == 1)) {
      OsbConfig.Bits.osblord = 1;
      OsbConfig.Bits.osbrmt  = 1;
    }

  }
  QpiWritePciCfg (host, socket, ha, OSB_CONFIG_HA_CFG_REG, OsbConfig.Data);
  return QPI_SUCCESS;
}

QPI_STATUS
SetupSystemCoherency (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  SetupSystemCoherencyHsx(host, SocketData, QpiInternalGlobal);  
  return QPI_SUCCESS;
}


QPI_STATUS
SetupSystemCoherencyHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8                                    Idx1, Port, Ctr;
  UINT32                                   Data32;
  BT_CONFIG_HA_CFG_HSX_BDX_STRUCT          BtConfig;
  HASYSDEFEATURE_HA_CFG_STRUCT             HaSysDefeature;
  HASYSDEFEATURE2_HA_CFG_HSX_BDX_STRUCT    HaSysDefeature2;
  CBO_COH_CONFIG_CBOBC_COH_HSX_BDX_STRUCT  CboCohBc;  
  SBO_CONFIG_SBO_CFG_STRUCT                SboConfig;
  HADIRECT2CORETHRESHOLDS_HA_CFG_STRUCT    HaDirect2CoreThresholds;
  R3EGRCTRL_R3QPI_CFG_BDX_STRUCT           R3EgrCtrl_Bdx;
  R3QCTRL_R3QPI_CFG_BDX_STRUCT             R3QCtrl_Bdx;
  QPIAGCTRL_QPIAGENT_LL_HSX_STRUCT         QpiAgtCtrl_Hsx;
  QPIAGCTRL_QPIAGENT_LL_BDX_STRUCT         QpiAgtCtrl_Bdx;
  UINT32                                   QpiAgtCtrl_Bits_enable_snoop_spawn;
  UINT32                                   QpiAgtCtrl_Bits_direct_to_core_disable;
  UINT32                                   QpiAgtCtrl_Bits_direct_to_core_threshold;
  UINT32                                   QpiAgtCtrl_Bits_disable_pe;
  UINT32                                   QpiAgtCtrl_Bits_cod_mode;
  UINT32                                   QpiAgtCtrl_Bits_num_of_cbos;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_SYSTEM_COHERENCY_SETUP, 0, host);

  //
  // Home Snoop with OSB and Directory is the mode used for all supported configurations.
  // DirectToCore (D2C) feature can be enabled in QPIAgent and Home Agent for all configs.
  //

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      // 
      // CBo coherency settings
      //   mtoibias, biasfwd, biasfwdlocalhome, sendrddataasmigratory, enablemp: Set to 1 for all configs
      //   biasfwddoubledata: Set 1 for all configs
      //   ego: Set 1 for 1S-noncod; leave it to default for all other configs
      //   disisoctorres, disisocegrres, disisocrtidres: Set all to 1 for non-ISOCH configs; leave it to default for all other configs
      //   These are removed in HSX: conddisablef, localkeepffwds: Set to 1 for all Directory Snoop modes; leave it to default for Home Snoop mode
      //

      CboCohBc.Data = QpiReadPciCfg (host, Idx1, 0, CBO_COH_CONFIG_CBOBC_COH_REG);
      CboCohBc.Bits.mtoibias                = 1;
      CboCohBc.Bits.biasfwd                 = 1;
      CboCohBc.Bits.biasfwdlocalhome        = 1;
      CboCohBc.Bits.sendrddataasmigratory   = 1;
      CboCohBc.Bits.enablemp                = 1;
      CboCohBc.Bits.biasfwddoubledata       = 1;
      CboCohBc.Bits.waitfordatacmp          = 0;
      CboCohBc.Bits.drdgosonem              = 1;

      if ((host->var.qpi.SysConfig == SYS_CONFIG_1S) && (host->var.qpi.OutClusterOnDieEn == FALSE)) {
          CboCohBc.Bits.ego                 = 1;
      }

      if (host->var.qpi.OutIsocEn != TRUE) {
          CboCohBc.Bits.disisoctorres       = 1;
          CboCohBc.Bits.disisocrtidres      = 1;
          CboCohBc.Bits.disisocegrres       = 1;
      } else if (Idx1 != QpiInternalGlobal->SbspSoc) {
          CboCohBc.Bits.disisoctorres       = 1;
          CboCohBc.Bits.disisocrtidres      = 1;
      }

      if (host->var.qpi.OutMesegEn != TRUE) {
        CboCohBc.Bits.disablemesegtorres    = 1;
      }

      if ((QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) {
        CboCohBc.Bits.dpsrcsnoop          = 1;
      }

      //
      // Enable Local WB credits when BT active
      //
      if (QpiInternalGlobal->BtModeEn) {
        CboCohBc.Bits.enablelochtcred = 1; 
        CboCohBc.Bits.uselochtcredforread = 1;
      }
      
      //
      // Program sleeptagsel for cbo ingress sleep
      // This selects the NID bit used to id the queue 
      //                                     
      if (QpiInternalGlobal->TotHa[0] == 2) {
        CboCohBc.Bits.sleeptagsel = 2;
      } else {
        CboCohBc.Bits.sleeptagsel = 0;
      }
      //
      // s4166522 CLONE from HSX: Disable LLCMiss for DRd by default
      //
      if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
        CboCohBc.Bits.llcmissendrd            = 0; 
      }
      
      //
      // Brickland BDX HSD 4971403 : BIOS brickland BDX: POR change for 4S COD 
      // Override sendrddataasmigratory as 0 when HitMe is disabled and IoDc is enabled for 4+ CAs
      if (host->var.common.cpuType == CPU_BDX) {
        if ((host->var.qpi.SysConfig != SYS_CONFIG_1S && host->var.qpi.OutClusterOnDieEn == TRUE) ||
            (host->var.common.numCpus >= 4)) {
          if (QpiInternalGlobal->IodcEn == 1 && QpiInternalGlobal->HitMeEn == 0) {
            CboCohBc.Bits.sendrddataasmigratory = 0;
          } // HitMe$ disabled and IoDc enabled  
        } // >= 4CAs check
      } // cpuType check

      QpiWritePciCfg (host, Idx1, 0, CBO_COH_CONFIG_CBOBC_COH_REG, CboCohBc.Data);

      //
      // Sbo
      //
      if ((QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)) {
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotSbo[Idx1]; ++Ctr) {
          SboConfig.Data = QpiReadPciCfg (host, Idx1, Ctr, SBO_CONFIG_SBO_CFG_REG);
          SboConfig.Bits.dpsrcsnoop = 1;
          QpiWritePciCfg (host, Idx1, Ctr, SBO_CONFIG_SBO_CFG_REG, SboConfig.Data);
        }  
      }
      
      // 
      // HA settings
      // 
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[Idx1]; ++Ctr) {

        Data32 = QpiReadPciCfg (host, Idx1, Ctr, H2SLUT_HA_CFG_REG);
        // Clear dirnode field
        Data32 = Data32 & 0xFF00FFFF;
        if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE) || (QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE)) {
          
          if (host->var.qpi.OutClusterOnDieEn == 1)  {
            //
            // In COD only clear the local cluster CA.
            //
            if (Ctr == 0) {
              Data32 = Data32 | ((QpiInternalGlobal->CpuList & (~(1 << Idx1))) << 16);
              Data32 = Data32 | (QpiInternalGlobal->CpuList << 20);
            } else {
              Data32 = Data32 | ((QpiInternalGlobal->CpuList & (~(1 << Idx1))) << 20);
              Data32 = Data32 | (QpiInternalGlobal->CpuList << 16);
            }
          } else {
            // Set dirnode to list of CPUs in the system excluding self
            Data32 = (Data32 & 0xFF00FFFF) | ((QpiInternalGlobal->CpuList & (~(1 << Idx1))) << 16);
          }
        }
        QpiWritePciCfg (host, Idx1, Ctr, H2SLUT_HA_CFG_REG, Data32);
        
        if (host->var.qpi.OutClusterOnDieEn == 1) {
          Data32 = QpiReadPciCfg (host, Idx1, Ctr, HASYSDEFEATURE3_HA_CFG_REG);
          Data32 = Data32 | 1;
          QpiWritePciCfg (host, Idx1, Ctr, HASYSDEFEATURE3_HA_CFG_REG, Data32);
        }

        Data32 = QpiReadPciCfg(host, Idx1, Ctr, MCMTR_MC_MAIN_REG);
        if (QpiInternalGlobal->SysSnoopMode == HOME_SNOOP_MODE || QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
          Data32 = Data32 & (~(1 << 3));  // clr dir_en
        } else {
          Data32 = Data32 | (1 << 3);     // Set dir_en
        }
        QpiWritePciCfg (host, Idx1, Ctr, MCMTR_MC_MAIN_REG, Data32); 

        //
        // HASYSDEFEATURE setting
        // dirupdate2sopten and alwaysflocalen not supported in HSX, leave Zero
        // enablehd: Set to 1 for Directory Snoop mode configs; leave it to default for all other configs
        //

        HaSysDefeature.Data = QpiReadPciCfg (host, Idx1, Ctr, HASYSDEFEATURE_HA_CFG_REG);

        HaSysDefeature.Bits.alwaysflocalen = 0;
        HaSysDefeature.Bits.dirupdate2sopten = 0;

        if (QpiInternalGlobal->HitMeEn == TRUE) {
          if(host->var.common.cpuType == CPU_HSX && host->var.common.stepping < C0_REV_HSX) {
            //
            // disablemcbyps when hitme enabled
            //
            HaSysDefeature.Data |= 1 << 19;
          }
        }

        if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE) || (QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE)) {
          HaSysDefeature.Bits.enablehd = 1;
          HaSysDefeature.Bits.disablees = 1;
        }
        
        if (QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
          HaSysDefeature.Bits.enablehd = 0;
          if (host->var.qpi.SysConfig != SYS_CONFIG_1S) {
            HaSysDefeature.Bits.disablees = 0;
          }
        }

        

        QpiWritePciCfg (host, Idx1, Ctr, HASYSDEFEATURE_HA_CFG_REG, HaSysDefeature.Data);


        //
        // HASYSDEFEATURE2 setting
        // rspbypassdisable: Clear it for Early and Home snoop. leave it to default for DIR modes
        // locdirbypassdisable, rmtdirbypassdisable: Set to 1 for all Home Snoop configs; leave it to default for all other configs
        // iodcen: Set to 1 for all Directory Snoop modes; leave it to default for all other configs
        // forcenonsnpordering: Set to 1 only for migration slave targets.; leave it to default for all other configs
        // snpearlyarbdisable: Clear it for all 1S/2S configs; leave it to default for all other configs
        //

        HaSysDefeature2.Data = QpiReadPciCfg (host, Idx1, Ctr, HASYSDEFEATURE2_HA_CFG_REG);

        if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE) || (QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE)) {
          HaSysDefeature2.Bits.rspbypassdisable = 1;
        } else {
          HaSysDefeature2.Bits.rspbypassdisable = 0;
        }

        if (host->var.qpi.SysConfig == SYS_CONFIG_1S || QpiInternalGlobal->SysSnoopMode == HOME_SNOOP_MODE || QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
          HaSysDefeature2.Bits.locdirbypassdisable = 1;
          HaSysDefeature2.Bits.rmtdirbypassdisable = 1;
        }

        // be249117 Hitting full cacheline corruption with error injection on upper codeword and RmtDirBypassEnabled w/ BT
        //
        if ((QpiInternalGlobal->BtModeEn == TRUE) && (host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < B0_REV_HSX)) {
          HaSysDefeature2.Bits.locdirbypassdisable = 1;
          HaSysDefeature2.Bits.rmtdirbypassdisable = 1;
        }
        
        if (QpiInternalGlobal->HitMeEn == TRUE) {
          HaSysDefeature2.Bits.enablehitme = 1;
        }

        if( host->var.qpi.SysConfig == SYS_CONFIG_1S  || host->var.qpi.SysConfig == SYS_CONFIG_2SEN || ((host->var.qpi.SysConfig == SYS_CONFIG_2SEP) && (host->var.qpi.OutClusterOnDieEn == FALSE))) {
          HaSysDefeature2.Bits.snpearlyarbdisable = 0;              // 0 = Enable optimization 
        }

        if(host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
          //
          // HSX A0 WA (be247348, 4166310)CLONE from HSX: Conflict latency optimization still leads to data corruption
          //
          HaSysDefeature2.Bits.cnfltlatoptdisable = 1;
        }
        
        if(QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) {
          //
          // HSX WA (4167988) BIOS should set HaSysDefeature2.cnfltlatoptdisable in early snoop
          //
          HaSysDefeature2.Bits.cnfltlatoptdisable = 1;
        }

        //
        // s4166941 set egcritalwaysfirstdisable=1 in HA for *all* configs
        //
        HaSysDefeature2.Bits.egcritalwaysfirstdisable = 1;

        // IODC Feature - Supported for BDX only
        HaSysDefeature2.Bits.iodcen = QpiInternalGlobal->IodcEn;

        QpiWritePciCfg (host, Idx1, Ctr, HASYSDEFEATURE2_HA_CFG_REG, HaSysDefeature2.Data);

        //
        // Program HA D2C related SV settings.
        //
        HaDirect2CoreThresholds.Data = QpiReadPciCfg (host, Idx1, Ctr, HADIRECT2CORETHRESHOLDS_HA_CFG_REG);
        HaDirect2CoreThresholds.Bits.d2cthresholden = 0x7;

        // Grantley Refresh HSD 4988324 : PerfWG: Change direct2core disable threshold to 2 
        // Applicable to BDX- DE, EP and EX SKUs
        if (host->var.common.cpuType == CPU_BDX) {
          HaDirect2CoreThresholds.Bits.d2cthresholddis = 0x2;
        } // cpuType check

        QpiWritePciCfg (host, Idx1, Ctr, HADIRECT2CORETHRESHOLDS_HA_CFG_REG, HaDirect2CoreThresholds.Data);

        if (QpiInternalGlobal->BtModeEn == TRUE) {
          BtConfig.Data = QpiReadPciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG);
          if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
            BtConfig.Data = QpiReadPciCfg (host, Idx1, 0, BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG);
          }  


          BtConfig.Bits.btenable = 1;
          BtConfig.Bits.enablenosplit = 1;
          
          //
          // program BT mode based on encodng: 0: Baseline, 1: BT_Mode1, 2: BT_Mode2, 4: BT_Mode3
          //
          if( QpiInternalGlobal->BtMode != BT_MODE_3) {
            BtConfig.Bits.btmode = QpiInternalGlobal->BtMode;
          } else {
            BtConfig.Bits.btmode = 4;
          }


          // If BT Mode already enabled, need to swizzle 'btenable' bit in order for new values to take affect.
          if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType == CPU_HSX_EX) ||
              (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType == CPU_BDX_EX)) {
            if (BtConfig.Bits.btenable) {
              BtConfig.Bits.btenable = 0;
              QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);
              BtConfig.Bits.btenable = 1;
            }
          } // cpuType check
          QpiWritePciCfg (host, Idx1, Ctr, BT_CONFIG_HA_CFG_REG, BtConfig.Data);

          if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < B0_REV_HSX) {
            QpiWritePciCfg (host, Idx1, 0, BIOSSTICKYLOCKBYPASSSCRATCHPAD2_UBOX_CFG_REG, BtConfig.Data);
          }  
        }//BTMode enabled

        //
        // Program OSB settings.
        //
        ConfigureOsbSetting(host, QpiInternalGlobal, Idx1, Ctr);  
      } // for Ctr loop

      // 
      // QPIAgent
      // 
      for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
        if (SocketData->Cpu[Idx1].LinkData[Port].Valid == TRUE) {
          Data32 = QpiReadPciCfg (host, Idx1, Port, QPIAGCTRL_QPIAGENT_LL_REG);
          if (host->var.common.cpuType == CPU_BDX) {
            QpiAgtCtrl_Bdx.Data = Data32;
            QpiAgtCtrl_Bits_enable_snoop_spawn       = QpiAgtCtrl_Bdx.Bits.enable_snoop_spawn;
            QpiAgtCtrl_Bits_direct_to_core_disable   = QpiAgtCtrl_Bdx.Bits.direct_to_core_disable;
            QpiAgtCtrl_Bits_direct_to_core_threshold = QpiAgtCtrl_Bdx.Bits.direct_to_core_threshold;
            QpiAgtCtrl_Bits_disable_pe               = QpiAgtCtrl_Bdx.Bits.disable_pe;
            QpiAgtCtrl_Bits_cod_mode                 = QpiAgtCtrl_Bdx.Bits.cod_mode;
            QpiAgtCtrl_Bits_num_of_cbos              = QpiAgtCtrl_Bdx.Bits.num_of_cbos;          
          } else {
            QpiAgtCtrl_Hsx.Data = Data32;
            QpiAgtCtrl_Bits_enable_snoop_spawn       = QpiAgtCtrl_Hsx.Bits.enable_snoop_spawn;
            QpiAgtCtrl_Bits_direct_to_core_disable   = QpiAgtCtrl_Hsx.Bits.direct_to_core_disable;
            QpiAgtCtrl_Bits_direct_to_core_threshold = QpiAgtCtrl_Hsx.Bits.direct_to_core_threshold;
            QpiAgtCtrl_Bits_disable_pe               = QpiAgtCtrl_Hsx.Bits.disable_pe;
            QpiAgtCtrl_Bits_cod_mode                 = QpiAgtCtrl_Hsx.Bits.cod_mode;
            QpiAgtCtrl_Bits_num_of_cbos              = QpiAgtCtrl_Hsx.Bits.num_of_cbos;          
          }
          if ((QpiInternalGlobal->SysSnoopMode == EARLY_SNOOP_MODE) && (host->var.qpi.SysConfig != SYS_CONFIG_1S)){
            QpiAgtCtrl_Bits_enable_snoop_spawn = 1; // Enable Early Snoop
          }
          if (QpiInternalGlobal->D2cEn == TRUE) {
            QpiAgtCtrl_Bits_direct_to_core_disable = 0;
          } else {
            QpiAgtCtrl_Bits_direct_to_core_disable = 1;
          }

          if (host->var.qpi.OutIsocEn) {
            QpiAgtCtrl_Bits_disable_pe = 0;
          } else {
            QpiAgtCtrl_Bits_disable_pe = 1;
          }
          
          if (host->var.qpi.OutClusterOnDieEn == 1) {
            QpiAgtCtrl_Bits_cod_mode = 1;
            QpiAgtCtrl_Bits_num_of_cbos = (QpiInternalGlobal->TotCbo[Idx1] / 2);
          } else {
            QpiAgtCtrl_Bits_cod_mode = 0;
            QpiAgtCtrl_Bits_num_of_cbos = QpiInternalGlobal->TotCbo[Idx1];
          }
          
          if (host->var.common.cpuType == CPU_BDX) {
            QpiAgtCtrl_Bdx.Bits.enable_snoop_spawn       = QpiAgtCtrl_Bits_enable_snoop_spawn;
            QpiAgtCtrl_Bdx.Bits.direct_to_core_disable   = QpiAgtCtrl_Bits_direct_to_core_disable;
            QpiAgtCtrl_Bdx.Bits.direct_to_core_threshold = QpiAgtCtrl_Bits_direct_to_core_threshold;
            QpiAgtCtrl_Bdx.Bits.disable_pe               = QpiAgtCtrl_Bits_disable_pe;
            QpiAgtCtrl_Bdx.Bits.cod_mode                 = QpiAgtCtrl_Bits_cod_mode;
            QpiAgtCtrl_Bdx.Bits.num_of_cbos              = QpiAgtCtrl_Bits_num_of_cbos;

            // IODC Feature - Supported for BDX only
            QpiAgtCtrl_Bdx.Bits.iodc_mode = QpiInternalGlobal->IodcEn;
            
            // 4S COD Feature - Supported for BDX only
            if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
              QpiAgtCtrl_Bdx.Bits.config_4s = 1;
            }

            Data32 = QpiAgtCtrl_Bdx.Data;
          } else {
            QpiAgtCtrl_Hsx.Bits.enable_snoop_spawn       = QpiAgtCtrl_Bits_enable_snoop_spawn;
            QpiAgtCtrl_Hsx.Bits.direct_to_core_disable   = QpiAgtCtrl_Bits_direct_to_core_disable;
            QpiAgtCtrl_Hsx.Bits.direct_to_core_threshold = QpiAgtCtrl_Bits_direct_to_core_threshold;
            QpiAgtCtrl_Hsx.Bits.disable_pe               = QpiAgtCtrl_Bits_disable_pe;
            QpiAgtCtrl_Hsx.Bits.cod_mode                 = QpiAgtCtrl_Bits_cod_mode;
            QpiAgtCtrl_Hsx.Bits.num_of_cbos              = QpiAgtCtrl_Bits_num_of_cbos;          
            Data32 = QpiAgtCtrl_Hsx.Data;
          }
          QpiWritePciCfg (host, Idx1, Port, QPIAGCTRL_QPIAGENT_LL_REG, Data32);
        }//if Link valid
      }//for each Port


      // 
      // R3QPI
      //
      for (Ctr=0; Ctr<QpiInternalGlobal->TotR3Qpi[Idx1]; Ctr++) {
        if (host->var.common.cpuType == CPU_BDX) {
          // IODC Feature - Supported for BDX only
          R3EgrCtrl_Bdx.Data = QpiReadPciCfg (host, Idx1, Ctr, R3EGRCTRL_R3QPI_CFG_REG);
          R3EgrCtrl_Bdx.Bits.iodc_mode = QpiInternalGlobal->IodcEn;
          QpiWritePciCfg (host, Idx1, Ctr, R3EGRCTRL_R3QPI_CFG_REG, R3EgrCtrl_Bdx.Data);

          // 4S COD Feature - Supported for BDX only
          if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
            R3QCtrl_Bdx.Data = QpiReadPciCfg (host, Idx1, Ctr, R3QCTRL_R3QPI_CFG_REG);
            R3QCtrl_Bdx.Bits.config_4s = 1;
            QpiWritePciCfg (host, Idx1, Ctr, R3QCTRL_R3QPI_CFG_REG, R3QCtrl_Bdx.Data);
          } //4S-COD check
        } // cpu check
      } // for loop

    }//if Socket Valid
  }//for each Socket

  return QPI_SUCCESS;
}


QPI_STATUS
ExecuteIioDisableFlow(
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal,
  UINT8               SocketId
  )
{
  UINT8     DevHide;
  UINT32    MailboxData;
  IIOMISCCTRL_N1_IIO_VTD_HSX_BDX_STRUCT  IioMiscCtrlN1;
  CC_SPARE2_IIO_DFX_GLOBAL_STRUCT        CcSpare2;

  // Case 1: If Powering Down is "Disable Ports and IIO without memory hotplug"
  //   Step 1. Set IIOMISCCTRL.uniphy_en_fuse4_pwrdn = 1
  //   Step 2. BIOS sets bits[10-0]=0x7FF to be send thru mailbox to disable all PCIe ports
  //   Step 3. BIOS programming CcSpare2 bits
  //   Step 4. BIOS hides all the IIO devices thru devhide register
  //   Step 5. BIOS sends final data with step 2 thru mailbox that will trigger the pCode to do the following:
  //           a. Pcode sets bit 31 of IO_PCU_CONFIG to block pcode requests to IIO
  //                Note: After step a, the only IIO CSR write PCU allows is the write to IIO BNDL_DISBL_REG
  //           b. Pcode writes 0x7ff to IIO BNDL_DISBL_REG
  //               Note: After step b, pcode will not accept further request to IIO BNDL_DISBL_REG
  //
  // Case 2: If Powering Down is "Disable Ports Only with memory hotplug"
  //   Step 1. Set IIOMISCCTRL.uniphy_en_fuse4_pwrdn = 1
  //   Step 2. BIOS sets bits[10-0]=0x7FF thru mailbox to disable all PCIe ports
  //   Step 5. BIOS sends final data with step 2 thru mailbox that will trigger the pCode

  //
  // Step 1. Set IIOMISCCTRL.uniphy_en_fuse4_pwrdn = 1
  //
  IioMiscCtrlN1.Data = QpiReadPciCfg (host, SocketId, 0, IIOMISCCTRL_N1_IIO_VTD_REG);
  IioMiscCtrlN1.Bits.uniphy_en_fuse4_pwrdn = 1;    
  QpiWritePciCfg (host, SocketId, 0, IIOMISCCTRL_N1_IIO_VTD_REG, IioMiscCtrlN1.Data);

  //
  // Step 2. Set bits[10-0]=0x7FF thru mailbox to disable all PCIe ports
  //
  MailboxData = 0xD7707FF;    // tx, ln, phy clk_dis & port disable bits also set

  if (host->var.qpi.OutIioUniphyDisable[SocketId] == 1) {     // If Powering Down is "Disable Ports and IIO without memory hotplug"
    //
    // Step 3. BIOS programming CcSpare2 bits
    // HSD 4166814: BIOS programming of cc_spare2 for Uniphy Clock disables only for non-legacy sockets
    //
    CcSpare2.Data = QpiReadPciCfg (host, SocketId, 0, CC_SPARE2_IIO_DFX_GLOBAL_REG);
    CcSpare2.Bits.cc_spare2 |= BIT9;    // no activity on IOU0(DEV2)
    CcSpare2.Bits.cc_spare2 |= BIT8;    // no activity on IOU1(DEV3), IOU2(DEV1) and Port0/DMI(DEV0)
    QpiWritePciCfg (host, SocketId, 0, CC_SPARE2_IIO_DFX_GLOBAL_REG, CcSpare2.Data);

    //
    // Step 4. BIOS hides all the IIO devices (dev0 -> dev7) thru devhide register in IIO & UBOX
    //
    for( DevHide = 0; DevHide <8 ; DevHide++){
      QpiWritePciCfg (host, SocketId, 0, (DEVHIDE0_PCU_FUN3_REG + (DevHide * 4)), 0xFF);
      QpiWritePciCfg (host, SocketId, 0, (DEVHIDE0_UBOX_MISC_REG + (DevHide * 4)), 0xFF);
    }
  }

  //
  // Step 5. BIOS sends B2P command thru mailbox that will trigger the pCode
  //
  SendMailBoxCmdToPcode(host, SocketId, MAILBOX_BIOS_CMD_IIO_DISABLE, MailboxData);

  //
  // After disable IIO, it will take 2000 cycles * 500 MHz for IIO clk to shut down (It's about 4 usec).
  // Need to wait for 1 ms to make sure we do not hit this corner case.
  //
  QpiFixedDelay(host, 1000); // 1000 microsecs
  return QPI_SUCCESS;
}    

QPI_STATUS
SetupQpiMisc (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8                                           Idx1, LinkIndex;
  UINT32                                          Data32;
  QPILKDFXCTL_QPIAGENT_DFX_COMMON_HSX_BDX_STRUCT  QpiLkDfxCtl;

  SetupQpiMiscHsx(host, SocketData, QpiInternalGlobal);  

  //
  // Lock QPI DFX 
  //
  if (!host->var.common.EVMode) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n    Lock QPI DFX."));
  } else {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n    EV Mode active:  No locking QPI DFX."));
  }
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (QpiInternalGlobal->CpuList & (1 << Idx1)) {
      for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
        if ((SocketData->Cpu[Idx1].LinkData[LinkIndex].Valid == TRUE) || (host->setup.qpi.QpiLbEn)) {
          if (LinkIndex != 1) {
            QpiLkDfxCtl.Data = (UINT16) QpiReadPciCfg (host, Idx1, LinkIndex, QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG);
            if(!host->var.common.EVMode) {
              QpiLkDfxCtl.Bits.iodfxlck = 1;
            } else {
             QpiLkDfxCtl.Bits.iodfxlck = 0;
            }
           QpiWritePciCfg (host, Idx1, LinkIndex, QPILKDFXCTL_QPIAGENT_DFX_COMMON_REG, (UINT32) QpiLkDfxCtl.Data);
          } // LinkIndex != 1 check
          Data32 = QpiReadPciCfg(host, Idx1, LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG);
          if(!host->var.common.EVMode) {
            Data32 |= (BIT0 | BIT1 | BIT2);
          } else {
            Data32 &= ~(BIT0 | BIT1 | BIT2);
          }
          QpiWritePciCfg(host, Idx1, LinkIndex, QPI_DFX_LCK_CTL_CFG_QPIAGENT_LL_REG, Data32);
        } // if port valid
      }   //for Port
    } // if CpuList valid
  }   // for sockets

  return QPI_SUCCESS;
}


QPI_STATUS
SetupQpiMiscHsx (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8                                       Idx1, Idx2, Ctr;
  UINT32                                      Data32, Misclist, LocalCpuList; 
  UINT32                                      GidNidMap, GblNidMap0, GblNidMap1, DcaSad;
  UINT32                                      Mask, Val;
  UINT32                                      DcaSadSoc[MAX_CPU_SOCKETS];
  UINT8                                       IsocPool;
  IIOMISCCTRL_N0_IIO_VTD_HSX_BDX_STRUCT       IioMiscCtrl;
  CIPCTRL_IIO_VTD_HSX_BDX_STRUCT              Cipctrl;
  TSWCTL0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT       Tswctl0; 
  R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_STRUCT    R2pcie_glb_rsp_cntrl_0;
  R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_STRUCT     R2pcie_asc_ldval_hi_0;
  R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_STRUCT     R2pcie_asc_ldval_lo_0;
  R2PINGDBG_R2PCIE_CFG_HSX_BDX_STRUCT         R2pingdbg_HsxBdx;
  R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_STRUCT      R2pcie_asc_control_0;
  IRPEGCREDITS_IIO_VTD_STRUCT                 IrpEgCredits;
  HATIMEOUT_HA_CFG_STRUCT                     HaTimeout;
  UBOXERRCTL_UBOX_CFG_STRUCT                  UboxErrCtl;
  CSIPCTRL_0_IIO_DFX_GLOBAL_STRUCT            Csipctrl0;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_QPI_MISC_SETUP, host);

  if (host->var.qpi.OutIsocEn == TRUE) {
    IsocPool = DEFAULT_ISOC_RTIDS;
  } else if (host->var.qpi.OutMesegEn == TRUE) {
    IsocPool = 0;
  } else {
    IsocPool = 0;
  }

  //
  // Prepare the NID map. For DCA, the map is prepared with node id of the sockets that present.
  // For the entries corresponding to the sockets that are not populated, use the socket id on which
  // the map is programmed
  //
  GblNidMap0 = 0;         // for socket 0-3
  GblNidMap1 = 0;         // for socket 4-7
  GidNidMap = 0;
  DcaSad = 0;

  Misclist = QpiInternalGlobal->CpuList;                //Broadcast list for WbInvdAck/InvdAck/EOI

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (QpiInternalGlobal->CpuList & (1 << Idx1)) {
      if ( Idx1 < 4) {
         GblNidMap0 = GblNidMap0 | (1 << (28 + Idx1)) | (Idx1 << (4 * Idx1));
      } else {
         GblNidMap1 = GblNidMap1 | (1 << (28 + (Idx1 - 4))) | (Idx1 << (4 * (Idx1 - 4)));
      }   

      GidNidMap = GidNidMap | (Idx1 << (3 * Idx1));
      if (host->var.qpi.OutIioUniphyDisable[Idx1] == 1) {     // update Misc broadcast list 
        Misclist &= (~(1<< Idx1));
      }  
    }
  }
  
  
  //
  // Handle COD cases
  //
  LocalCpuList = QpiInternalGlobal->CpuList;
  if (host->var.qpi.OutClusterOnDieEn == TRUE) {
    LocalCpuList |= ((LocalCpuList & 0xf) << 4);
  }
             
  for (Idx1 = 0; Idx1 < SI_MAX_CPU_SOCKETS; ++Idx1) {
    if (LocalCpuList & (1 << Idx1)) {
      DcaSad = DcaSad | (SOCID_TO_NID(Idx1, 0) << (4 * Idx1));
    } else {
      //
      // Socket not present, program to DNID 0x4 which is not a valid CA in HSX
      //
      DcaSad = DcaSad | 0x4 << (4 * Idx1);
    }
  }
  
  // Grantley BDX HSD : 5002678 : [BDX-EP]: 4S COD POR Change : Disable IODC, HitMe$ and 1+ hops DCA accesses for 4S-COD- EP configs
  // Brickland BDX HSD : 4971403 : BIOS brickland BDX: POR change for 4S COD 
  // When 4S-COD is enabled the DCA SAD programmed per socket can differ based
  // on the neighbor's hop count from source socket. 
  // For all other cases, the DCA SAD programmed are uniform across
  // all sockets.
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; Idx1++) {
    DcaSadSoc[Idx1] = DcaSad;
    if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
      for (Idx2 = 0; Idx2 < MAX_TREE_NODES; Idx2++) {
        if (SocketData->CpuTree[Idx1][Idx2].Node.Valid == 1 && SocketData->CpuTree[Idx1][Idx2].Node.Hop > 1) {
          // Patch the NodeIDs associated with this socket as 0x4
          // to disable the DCA Hints going to this destination
          Val = (0x4 << (SocketData->CpuTree[Idx1][Idx2].Node.SocId * 4));
          Mask = (0xF << (SocketData->CpuTree[Idx1][Idx2].Node.SocId * 4));
          // Since COD is enabled, populate the upper 16 bits also with a copy of lower 16 bits of value or mask
          Val |= (Val << 16);
          Mask |= (Mask << 16);
          DcaSadSoc[Idx1] = (DcaSadSoc[Idx1] & (~Mask)) | Val;
        } // hops check 
      } // dest socket loop
    } // 4S-COD check
  } // src socket loop

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (QpiInternalGlobal->CpuList & (1 << Idx1)) {

      //  
      // IIO Interface
      // 

      // Coherent Interface Protocol Control
      Cipctrl.Data = QpiReadPciCfg (host, Idx1, 0, CIPCTRL_IIO_VTD_REG);
      Cipctrl.Bits.numrtids_isoc_pool0 = IsocPool;
      Cipctrl.Bits.numrtids_isoc_pool1 = IsocPool;
      Cipctrl.Bits.diswrupdtflow = 0x01;

      if((host->var.common.cpuType == CPU_HSX && host->var.common.stepping >= B0_REV_HSX) ||
         (host->var.common.cpuType == CPU_BDX) ) {
        if (host->setup.common.dcaEn == TRUE) {
          Cipctrl.Bits.dcaen = 1;  //set the dcaen field
        }
      }
      // Grantley HSD 4168320 : HSX Clone: CBo PRQ Credit Overflow With All Cores Reporting 3-strike
      // HSX-EP/EN only seem to apply this setting but HSX-EX doesn't
      if ((host->var.common.cpuType == CPU_HSX && host->var.common.cpuSubType != CPU_HSX_EX) ||
          (host->var.common.cpuType == CPU_BDX && host->var.common.cpuSubType != CPU_BDX_EX)) {
        if (host->var.common.cpuType == CPU_HSX && host->var.qpi.OutMesegEn == FALSE) {
          Cipctrl.Bits.vc1_pri_en = 0;  
        } // OutMesegEn check

        // No vc1 unless isoc is enabled on BDX-
        if (host->var.common.cpuType == CPU_BDX && host->var.qpi.OutIsocEn == FALSE) {
            Cipctrl.Bits.vc1_pri_en = 0;
        } // BDX- and OutIsocEn check
      } // Non EX- check
      QpiWritePciCfg (host, Idx1, 0, CIPCTRL_IIO_VTD_REG, Cipctrl.Data);

      if((host->var.common.cpuType == CPU_HSX && host->var.common.stepping >= B0_REV_HSX) ||
         (host->var.common.cpuType == CPU_BDX) ) {
        if (host->setup.common.dcaEn == TRUE) {
          QpiWritePciCfg (host, Idx1, 0, CIPDCASAD_IIO_VTD_REG, DcaSadSoc[Idx1]);
        }
      }

       // IIO Misc Control
      IioMiscCtrl.Data = QpiReadPciCfg (host, Idx1, 0, IIOMISCCTRL_N0_IIO_VTD_REG);
      IioMiscCtrl.Bits.azalia_on_vcp = 0;
      
      if (host->var.qpi.OutLegacyVgaSoc == Idx1) {
        IioMiscCtrl.Bits.rvgaen = 0;
      } else {
        IioMiscCtrl.Bits.rvgaen = 1;
      }

      IioMiscCtrl.Bits.tocm = (MAX_ADDR_LINE - 32);  //'tocm' = # of addr lines above 4GB-1 (i.e. beyond the lower 32 lines)
      IioMiscCtrl.Bits.tocmvalid = 1;
      QpiWritePciCfg (host, Idx1, 0, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrl.Data);

      if (host->var.qpi.OutIioUniphyDisable[Idx1] != 0) {
        ExecuteIioDisableFlow(host, SocketData, QpiInternalGlobal, Idx1);
      }

      if ((QpiInternalGlobal->SysSnoopMode == HOME_DIR_SNOOP_MODE) ||
          (QpiInternalGlobal->SysSnoopMode == HOME_DIR_OSB_SNOOP_MODE)) {
          Tswctl0.Data = QpiReadPciCfg (host, Idx1, 7, TSWCTL0_IIO_DFX_GLOBAL_REG);
          Tswctl0.Bits.global_nosnoop_disable = 1;
          QpiWritePciCfg (host, Idx1, 7, TSWCTL0_IIO_DFX_GLOBAL_REG, Tswctl0.Data);
      }

      // Override vc1_prio if isoc is disabled on BDX-
      if (host->var.common.cpuType == CPU_BDX && host->var.qpi.OutIsocEn == FALSE) {
        Csipctrl0.Data = QpiReadPciCfg(host, Idx1, 7, CSIPCTRL_0_IIO_DFX_GLOBAL_REG);
        Csipctrl0.Bits.vc1_priority = 0;
        QpiWritePciCfg(host, Idx1, 7, CSIPCTRL_0_IIO_DFX_GLOBAL_REG, Csipctrl0.Data);
      } // BDX- and OutIsocEn check

      //
      // 4167101: HSX Clone: SC: Credit loss between IRP and R2 interface from AD or AK traffic resulting in TOR TO and 100% stalling for lack of credits in ADQ
      // Needed for A0, B0, L0
      //
      if (host->var.common.cpuType == CPU_HSX && host->var.common.stepping < C0_REV_HSX) {
        IrpEgCredits.Data = QpiReadPciCfg (host, Idx1, 0, IRPEGCREDITS_IIO_VTD_REG);
        IrpEgCredits.Bits.ad_shared_cdt_threshold   = 0xb;
        IrpEgCredits.Bits.vc0p0_shared_cdt_threshold   = 0x1;
        IrpEgCredits.Bits.qpi_ndr_shared_cdt_threshold   = 0x0;
        QpiWritePciCfg (host, Idx1, 0, IRPEGCREDITS_IIO_VTD_REG, IrpEgCredits.Data);
      }

      if (host->setup.common.PSMIEnabledSupport) {      
        IrpEgCredits.Data = QpiReadPciCfg (host, Idx1, 0, IRPEGCREDITS_IIO_VTD_REG);
        IrpEgCredits.Bits.ad_shared_cdt_threshold   = 0x1;
        IrpEgCredits.Bits.ak_shared_cdt_threshold   = 0x1;
        IrpEgCredits.Bits.idi_cdt_threshold   = 0x1;
        IrpEgCredits.Bits.qpi_drs_cdt_threshold   = 0x1;
        QpiWritePciCfg (host, Idx1, 0, IRPEGCREDITS_IIO_VTD_REG, IrpEgCredits.Data);
      }

      // 
      // Ubox
      // 

      // Local Node id
      Data32 = QpiReadPciCfg (host, Idx1, 0, CPUNODEID_UBOX_CFG_REG);
      Data32 = (Data32 & 0xFFFFE318)  | (QpiInternalGlobal->SbspSoc << 10)  | (QpiInternalGlobal->SbspSoc << 5) | Idx1; // Set lgcnodeid, locknodeid, lclnodeid
      QpiWritePciCfg (host, Idx1, 0, CPUNODEID_UBOX_CFG_REG, Data32);

      // CpuEnable
      Data32 = (1 << 31) | QpiInternalGlobal->CpuList; // Set the list of CPUs in the system and enable the register
      QpiWritePciCfg (host, Idx1, 0, CPUENABLE_UBOX_CFG_REG, Data32);

      // Bcastlist
      if (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Idx1) {
        // CPU Socket being onlined should not be in the system interrupt domain until the last quiesce operation
        Data32 = 0;
      } else if (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId != Idx1) {
        // CPU Socket being onlined should not be in the system interrupt domain until the last quiesce operation
        Misclist &= (~(1<<  (host->var.qpi.RasSocId))); 
        LocalCpuList = (QpiInternalGlobal->CpuList & (~(1 << (host->var.qpi.RasSocId))));
        Data32 = (LocalCpuList << 24) | (Misclist << 16) | (LocalCpuList << 8) | (LocalCpuList);
      } else {
        Data32 = (QpiInternalGlobal->CpuList << 24) | (Misclist << 16) | (QpiInternalGlobal->CpuList << 8) | (QpiInternalGlobal->CpuList);
      }
      QpiWritePciCfg (host, Idx1, 0, BCASTLIST_UBOX_CFG_REG, Data32);


      // Lock Control
      Data32 = QpiReadPciCfg (host, Idx1, 0, LOCKCONTROL_UBOX_CFG_REG);
      Data32 = Data32 & (~(1 << 4)); // Disable compatibility mode
      Data32 = Data32 & 0xFFFFFFFE; // Enable the lock
      QpiWritePciCfg (host, Idx1, 0, LOCKCONTROL_UBOX_CFG_REG, Data32);

      // Group Id to Node Id Map
      QpiWritePciCfg (host, Idx1, 0, GIDNIDMAP_UBOX_CFG_REG, GidNidMap);

      // s4031310 Ubot timeout To set timeout duration 5 sec at 1.2GHz.
      //              NM  = (T0 * f )/NB     = (5.0 * 1.2E+9)/0xFFFFFF = 357.627889969 ~ 0x166[9:0]
      //              The count match register field specifies [9:4] of NM [9:0]
      //              0x166>>4 = 0x16[9:4]  ' 0x17[9:4] masking sure above 5 sec.

      UboxErrCtl.Data = QpiReadPciCfg (host, Idx1, 0, UBOXERRCTL_UBOX_CFG_REG);
      UboxErrCtl.Bits.smitimeoutcountmatch = 0x17;
      UboxErrCtl.Bits.masterlocktimeoutcountmatch = 0x17;       
      QpiWritePciCfg (host, Idx1, 0, UBOXERRCTL_UBOX_CFG_REG, UboxErrCtl.Data);

      // 
      // PCU
      // 

      // CpuNodeID
     QpiWritePciCfg (host, Idx1, 0, GLOBAL_NID_SOCKET_0_TO_3_MAP_PCU_FUN2_REG, GblNidMap0);  // Set the NID of CPUs in the system and enable the register
     if( GblNidMap1 != 0) {
        QpiWritePciCfg (host, Idx1, 0, GLOBAL_NID_SOCKET_4_TO_7_MAP_PCU_FUN4_REG, GblNidMap1);
     } 

      // 
      // Cbo
      // 

      // Isoc or MESEG Enable
      Data32 = QpiReadPciCfg (host, Idx1, 0, CBO_ISOC_CONFIG_CBOBC_COH_REG);
      Data32 &= ~0x00000003; //Clear MeSeg and Isoc flags
      if (host->var.qpi.OutIsocEn  == TRUE) {
        Data32 |= 0x00000001;  //Set ISOC flag
      } else {
        /// 4168320 - enme register only should be set when isoc is disabled
        if (host->var.qpi.OutMesegEn == TRUE) {
          Data32 |= 0x00000002;  //Set MESEG-only flag
        }
      }
      QpiWritePciCfg (host, Idx1, 0, CBO_ISOC_CONFIG_CBOBC_COH_REG, Data32);

      // 
      // HA
      // 
      for (Ctr = 0; Ctr < QpiInternalGlobal->TotHa[Idx1]; ++Ctr) {
        Data32 = QpiReadPciCfg (host, Idx1, Ctr, HASYSDEFEATURE_HA_CFG_REG);  
        Data32 = (Data32 & 0xFFFFFF) | (QpiInternalGlobal->CpuList << 24); // Program active_node_mask_bit_7 field
        
        if (host->var.qpi.OutClusterOnDieEn == 1) {
          //
          // Program active_node_mask_bit_7 for secondary CAs in COD mode
          //
          Data32 = (Data32 & 0xFFFFFFF) | (QpiInternalGlobal->CpuList << 28); 
        }
        
        QpiWritePciCfg (host, Idx1, Ctr, HASYSDEFEATURE_HA_CFG_REG, Data32);

        Data32 = QpiReadPciCfg (host, Idx1, Ctr, HASYSDEFEATURE2_HA_CFG_REG);  
        Data32 = Data32 & 0xFFFFFBFF; //Clear isochen
        Data32 = Data32 | (host->var.qpi.OutIsocEn << 10);
        if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < B0_REV_HSX)) {
          //
          // s4166961: Don't set isocen=1 in HA
          //
          Data32 = Data32 & 0xFFFFFBFF; //Clear isochen
        }
        QpiWritePciCfg (host, Idx1, Ctr, HASYSDEFEATURE2_HA_CFG_REG, Data32);
        
        Data32 = QpiReadPciCfg (host, Idx1, Ctr, H2SLUT_HA_CFG_REG); 
        
        if (host->var.qpi.OutClusterOnDieEn == 1) {
          if (Ctr == 0) {
            Data32 = (Data32 & 0x1FFFFFFF) | (Idx1 << 29); // Set SocketId to local socket id  
          } else {
            Data32 = (Data32 & 0x1FFFFFFF) | ((Idx1 + 4) << 29); // Set SocketId to local socket id + 4
          }
        } else {
          Data32 = (Data32 & 0x1FFFFFFF) | (Idx1 << 29); // Set SocketId to local socket id
        }
        QpiWritePciCfg (host, Idx1, Ctr, H2SLUT_HA_CFG_REG, Data32);

         // s4031310 Ha Timeout
        // 
        HaTimeout.Data = QpiReadPciCfg (host, Idx1, Ctr, HATIMEOUT_HA_CFG_REG);
        HaTimeout.Bits.bttotimebase = 3;
        HaTimeout.Bits.bttotimemult = 0x1f;
        QpiWritePciCfg (host, Idx1, Ctr, HATIMEOUT_HA_CFG_REG, HaTimeout.Data);  

      }

      // 
      // R2PCIE
      //
      if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX) && (host->var.common.stepping < C0_REV_HSX)) {   
        // Apply HSX B0 W/A (GRNT HSD-4168847): R2PCIe Setting Needed to Prevent System Slowdown or Softhang for HSX B0 Stepping
        // Note: This W/A also includes the following changes, but these regs are not CSRs and, therefore, can not be modified by BIOS:
        //    'R2DDEF8[7:4] = 1'  and  'r2pdfxmisc::asclock = 0'
        R2pcie_glb_rsp_cntrl_0.Data = (UINT16)QpiReadPciCfg(host, Idx1, 0, R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_REG);
        R2pcie_glb_rsp_cntrl_0.Bits.start_trigger_selector_for_global_response_0 = 0x2;
        R2pcie_glb_rsp_cntrl_0.Bits.stop_trigger_selector_for_global_response_0 = 0x7;
        R2pcie_glb_rsp_cntrl_0.Bits.start_trigger_selector_for_global_response_1 = 0x0;
        R2pcie_glb_rsp_cntrl_0.Bits.stop_trigger_selector_for_global_response_1 = 0x0;
        QpiWritePciCfg (host, Idx1, 0, R2PCIE_GLB_RSP_CNTRL_0_R2PCIE_CFG_REG, R2pcie_glb_rsp_cntrl_0.Data);

        R2pcie_asc_ldval_hi_0.Data = QpiReadPciCfg(host, Idx1, 0, R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_REG);
        R2pcie_asc_ldval_hi_0.Bits.load_high_value = 0x100;
        QpiWritePciCfg (host, Idx1, 0, R2PCIE_ASC_LDVAL_HI_0_R2PCIE_CFG_REG, R2pcie_asc_ldval_hi_0.Data);

        R2pcie_asc_ldval_lo_0.Data = QpiReadPciCfg(host, Idx1, 0, R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_REG);
        R2pcie_asc_ldval_lo_0.Bits.load_low_value = 0x10;
        QpiWritePciCfg (host, Idx1, 0, R2PCIE_ASC_LDVAL_LO_0_R2PCIE_CFG_REG, R2pcie_asc_ldval_lo_0.Data);

        R2pingdbg_HsxBdx.Data = QpiReadPciCfg(host, Idx1, 0, R2PINGDBG_R2PCIE_CFG_REG);
        R2pingdbg_HsxBdx.Bits.dbgbusen0 = 1;
        R2pingdbg_HsxBdx.Bits.rspfuncthrotbrdqpincs = 1;
        QpiWritePciCfg (host, Idx1, 0, R2PINGDBG_R2PCIE_CFG_REG, R2pingdbg_HsxBdx.Data);

        R2pcie_asc_control_0.Data = (UINT16)QpiReadPciCfg(host, Idx1, 0, R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_REG);
        R2pcie_asc_control_0.Bits.current_asc0_state_output = 0;
        R2pcie_asc_control_0.Bits.enable_asc0 = 1;
        QpiWritePciCfg (host, Idx1, 0, R2PCIE_ASC_CONTROL_0_R2PCIE_CFG_REG, R2pcie_asc_control_0.Data);
      }//HSX B0 W/A for R2PCIe
      
    }//Socket is present
  }//For each Socket

  return QPI_SUCCESS;
}

QPI_STATUS
HideOrUnHideDev (
  struct sysHost      *host,
  UINT8                Soc,
  UINT32               DevBitMap,
  BOOLEAN              Hide,
  UINT8                FuncStart,
  UINT8                FuncEnd
  )
{
  UINT32         Data32;
  UINT8          Func;
  
  //
  // Hide or unhide the devices thru devhide register in IIO & UBOX
  //
  for (Func = FuncStart; Func <= FuncEnd; Func++) {
    Data32 = QpiReadPciCfg (host, Soc, 0, DEVHIDE0_PCU_FUN3_REG + (Func * 4));
    if (Hide == TRUE) {
      Data32 |= DevBitMap;
    } else {
      Data32 &= ~DevBitMap;
    }
    QpiWritePciCfg (host, Soc, 0, (DEVHIDE0_PCU_FUN3_REG + (Func * 4)), Data32);

    Data32 = QpiReadPciCfg (host, Soc, 0, DEVHIDE0_UBOX_MISC_REG + (Func * 4));
    if (Hide == TRUE) {
      Data32 |= DevBitMap;
    } else {
      Data32 &= ~DevBitMap;
    }
    QpiWritePciCfg (host, Soc, 0, (DEVHIDE0_UBOX_MISC_REG + (Func * 4)), Data32);    
  }
  return QPI_SUCCESS;
}

QPI_STATUS
CollectAndClearErrors (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8                 Soc, LinkIndex, Ctr;
  UINT32                Data32;

  QPIERRCNT0_QPIAGENT_LL_STRUCT             QpiErrCnt0;
  QPIERRCNT1_QPIAGENT_LL_STRUCT             QpiErrCnt1;
  QPIERRDIS_QPIAGENT_LL_HSX_BDX_STRUCT      QpiErrDis;
  R2PINGERRMSK0_R2PCIE_CFG_HSX_BDX_STRUCT   R2PciIngrErrMask0;
  R3INGERRMASK2_R3QPI_CFG_STRUCT            R3IngrErrMask2;
  R2EGRERRLOG_R2PCIE_CFG_HSX_BDX_STRUCT     R2PciEgrErrLog;
  R3GLERRCFG_R3QPI_CFG_STRUCT               R3GlErrCfg;

  UINT32                                    HideBitmap = 0;
  BOOLEAN                                   Hide;

   for (Soc = 0; Soc < MAX_CPU_SOCKETS; ++Soc) {
     if (SocketData->Cpu[Soc].Valid == TRUE) {
        // R3QPI
        for (Ctr = 0; Ctr < QpiInternalGlobal->TotR3Qpi[Soc]; ++Ctr) {

          Data32 = QpiReadPciCfg (host, Soc, Ctr, R3EGRERRLOG0_R3QPI_CFG_REG); // RW1C
          host->var.qpi.CpuInfo[Soc].CpuErrLog.R3Qpi.R3Egrerrlog0 = Data32;
          //QpiWritePciCfg (host, Soc, Ctr, R3EGRERRLOG0_R3QPI_CFG_REG, Data32);

          Data32 = QpiReadPciCfg (host, Soc, Ctr, R3EGRERRLOG1_R3QPI_CFG_REG); // RW1C
          host->var.qpi.CpuInfo[Soc].CpuErrLog.R3Qpi.R3Egrerrlog1 = Data32;
          //QpiWritePciCfg (host, Soc, Ctr, R3EGRERRLOG1_R3QPI_CFG_REG, Data32);

          Data32 = QpiReadPciCfg (host, Soc, Ctr, R3INGERRLOG0_R3QPI_CFG_REG); // RW1C
          host->var.qpi.CpuInfo[Soc].CpuErrLog.R3Qpi.R3Ingerrlog0 = Data32;
          //QpiWritePciCfg (host, Soc, Ctr, R3INGERRLOG0_R3QPI_CFG_REG, Data32);

          Data32 = QpiReadPciCfg (host, Soc, Ctr, R3INGERRLOG1_R3QPI_CFG_REG); // RW1C
          host->var.qpi.CpuInfo[Soc].CpuErrLog.R3Qpi.R3Ingerrlog1 = Data32;
          //QpiWritePciCfg (host, Soc, Ctr, R3INGERRLOG1_R3QPI_CFG_REG, Data32);          

          // Permanent HSW W/A (s4167897): Clear certain E2E Error Masks when E2E Parity is enabled
          if (host->var.qpi.OutE2EParityEn == 1) {
            R3IngrErrMask2.Data = QpiReadPciCfg (host, Soc, Ctr, R3INGERRMASK2_R3QPI_CFG_REG);
            R3IngrErrMask2.Bits.csi0parerre2emask = 0;
            R3IngrErrMask2.Bits.csi1parerre2emask = 0;
            QpiWritePciCfg (host, Soc, Ctr, R3INGERRMASK2_R3QPI_CFG_REG, R3IngrErrMask2.Data);
          }//E2E Parity

          // Permanent HSW W/A (s4987052,s4969779): Program RCUTLCTRLCONFIG0_CR_R3GLERRCFG.MaskR3FatalError to 0xf 
          R3GlErrCfg.Data = QpiReadPciCfg (host, Soc, Ctr, R3GLERRCFG_R3QPI_CFG_REG);
          R3GlErrCfg.Bits.maskr3fatalerror = 0x0F;
          QpiWritePciCfg (host, Soc, Ctr, R3GLERRCFG_R3QPI_CFG_REG, R3GlErrCfg.Data);
        }//For each R3QPI

        // R2PCIe

        // Grantley Ref HSD 4988373 : Clone from BDX Sighting:[CBO][R2][U0] PRQ Credit Overflow while enabling CBO Clock Gating
        // (bugeco 281132, broadwell server 5097131)
        if ((host->var.common.cpuType == CPU_BDX) && (host->var.common.stepping >= B0_REV_BDX) && 
            (QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_DE || QpiInternalGlobal->SkuType[QpiInternalGlobal->SbspSoc] == SKU_NS)) {
          // Create a bit-mask to clear cbo16prqcrdoverflow and cbo17prqcrdoverflow bits only
          R2PciEgrErrLog.Data = 0x00;
          R2PciEgrErrLog.Bits.cbo16prqcrdoverflow = 1;    // Write 1 to clear this bit
          R2PciEgrErrLog.Bits.cbo17prqcrdoverflow = 1;    // Write 1 to clear this bit
          QpiWritePciCfg (host, Soc, 0, R2EGRERRLOG_R2PCIE_CFG_REG, R2PciEgrErrLog.Data);
        }

        Data32 = QpiReadPciCfg (host, Soc, 0, R2EGRERRLOG_R2PCIE_CFG_REG);  // RW1CS                 
        host->var.qpi.CpuInfo[Soc].CpuErrLog.R2Pcie.R2Egrerrlog = Data32;
        //QpiWritePciCfg (host, Soc, 0, R2EGRERRLOG_R2PCIE_CFG_REG, Data32);

        Data32 = QpiReadPciCfg (host, Soc, 0, R2PINGERRLOG0_R2PCIE_CFG_REG);// RW1CS
        host->var.qpi.CpuInfo[Soc].CpuErrLog.R2Pcie.R2Ingerrlog0 = Data32;
        //QpiWritePciCfg (host, Soc, 0, R2PINGERRLOG0_R2PCIE_CFG_REG, Data32);

        // HSD 4032142: CLONE from HSX: r2pcie: AD and BL SBO credit overflow fatal errors are not masked correctly 
        if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping <= B0_REV_HSX)) {   
          Data32 = QpiReadPciCfg (host, Soc, 0, R2GLERRCFG_R2PCIE_CFG_REG);
          Data32 |= (BIT13 + BIT14);               //maskr2fatalerror
          QpiWritePciCfg (host, Soc, 0, R2GLERRCFG_R2PCIE_CFG_REG, Data32);        
        }
        
        //
        // HSD 4166425: CLONE from HSX: r2pcie logs spurious credit overflow errors during reset
        //
        Data32 = QpiReadPciCfg (host, Soc, 0, R2EGRERRLOG2_R2PCIE_CFG_REG);// RW1CS
        QpiWritePciCfg (host, Soc, 0, R2EGRERRLOG2_R2PCIE_CFG_REG, Data32);

        host->var.qpi.CpuInfo[Soc].CpuErrLog.R2Pcie.R2Egrerrlog2 = 0;
        if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping >= B0_REV_HSX)) ||
             (host->var.common.cpuType == CPU_BDX)) {
          // log the data once the bug has been fixed
          host->var.qpi.CpuInfo[Soc].CpuErrLog.R2Pcie.R2Egrerrlog2 = Data32;
        }
        //Apply HSW W/A's
        // HSW W/A (s4167897): Clear certain E2E Error Masks when E2E Parity is enabled.
        if (host->var.qpi.OutE2EParityEn == 1) {
          R2PciIngrErrMask0.Data = QpiReadPciCfg (host, Soc, 0, R2PINGERRMSK0_R2PCIE_CFG_REG);
          R2PciIngrErrMask0.Bits.parerre2e0msk = 0;
          R2PciIngrErrMask0.Bits.parerre2e1msk = 0;
          if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.cpuSubType != CPU_HSX_EX) && (host->var.common.stepping < C0_REV_HSX)) {   
            R2PciIngrErrMask0.Bits.iioncscrderrmsk = 0;  //HSX B0 W/A (GRNT HSD-4168847)
          }
          QpiWritePciCfg (host, Soc, 0, R2PINGERRMSK0_R2PCIE_CFG_REG, R2PciIngrErrMask0.Data);
        }//E2E Parity
        
        // s3613189: Set up link error masks to avoid false errors.
        // s4168333: Change to QPI Error signalling.
        for (LinkIndex = 0; LinkIndex < MAX_QPI_PORTS; ++LinkIndex) {
          if (SocketData->Cpu[Soc].LinkData[LinkIndex].Valid == TRUE) {
            QpiErrCnt0.Data = QpiReadPciCfg (host, Soc,LinkIndex, QPIERRCNT0_QPIAGENT_LL_REG);
            QpiErrCnt0.Bits.mask |= 0x1;
            QpiWritePciCfg (host, Soc,LinkIndex, QPIERRCNT0_QPIAGENT_LL_REG, QpiErrCnt0.Data);
      
            QpiErrCnt1.Data = QpiReadPciCfg (host, Soc,LinkIndex, QPIERRCNT1_QPIAGENT_LL_REG);
            switch (host->var.common.cpuType) {
              case CPU_HSX:
              case CPU_BDX:
                QpiErrCnt1.Bits.mask |= 0xFFD0;
                break;
              default:
                QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
            }
            QpiWritePciCfg (host, Soc,LinkIndex, QPIERRCNT1_QPIAGENT_LL_REG, QpiErrCnt1.Data);
            
            // Enable/disable reporting of certain errors (QPIERRDIS).
            QpiErrDis.Data = QpiReadPciCfg (host, Soc, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG);
            switch (host->var.common.cpuType) {
              case CPU_HSX:
              case CPU_BDX:
                QpiErrDis.Bits.unc_mask = 0x00;  //Set to HW default - Needed for CPU Onlining to undo the 'Link not valid' value below.
                QpiErrDis.Bits.cor_mask = 0x0C;  //HSD-4168575: [HSX_C0_PO] Change to QPI Error signalling
                break;
              default:
                QPI_ASSERT(FALSE, ERR_GENERAL_FAILURE, MINOR_ERR_GENERAL);
            } //switch 
            QpiWritePciCfg (host, Soc, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG, QpiErrDis.Data);
          } else {
            // When LinkData is not valid, check for LinkIndex < TotQpiAgent before accessing QPIAGENT
            //  
            if (LinkIndex < QpiInternalGlobal->TotQpiAgent[Soc]) {
              // Link is unused and not hotplugen, disable all error reporting for it
              QpiErrDis.Data = QpiReadPciCfg (host, Soc, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG);
              QpiErrDis.Bits.unc_mask = 0x7FF;
              QpiErrDis.Bits.cor_mask = 0x7F;
              QpiWritePciCfg (host, Soc, LinkIndex, QPIERRDIS_QPIAGENT_LL_REG, QpiErrDis.Data);
            } // LinkIndex < TotQpiAgent[Socket]
          } //else Link not valid

        }//for each link


        if (QpiInternalGlobal->SkuType[Soc] == SKU_EXA) {
          // For EX- SKUs, we hide functions 3, 4, and 5 (phy layer)
          // Brickland HSD 4969182: SeCoe requirement to QPI Dev Hide at the boot flow and unside & hide it back in the QPIRC online flow
          HideBitmap = (1 << QPI0_DEV_NUM) + (1 << QPI1_DEV_NUM) + (1 << QPI2_HSX_DEV_NUM);
          HideOrUnHideDev(host, Soc, HideBitmap, TRUE, 3, 5);
        } else {
          // For EP- SKUs, we hide all functions, if both links are down. If one of them or both are up, we 
          // leave all the functions unhidden (Note: Default DEVHIDE reg value is 0x0).
          // The actual devhide logic is done in IioMiscHide() in DXE- phase, which hides certain functions
          // based on SKU type. Refer to Grantley HSD 4168685 : WHQL: PCI Compliance fails due to SSVID registers
          // for more details.
          HideBitmap = (1 << QPI0_DEV_NUM) + (1 << QPI1_DEV_NUM);

          if (SocketData->Cpu[Soc].LinkData[0].Valid == FALSE && SocketData->Cpu[Soc].LinkData[1].Valid == FALSE) {
            Hide = TRUE;
          } else {
            Hide = FALSE;
          }
          HideOrUnHideDev(host, Soc, HideBitmap, Hide, 0, 7);
        }

     }//If Socket valid 
   }//For each Socket 
   return QPI_SUCCESS;
}


QPI_STATUS
SetupRbt (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
  UINT8           Idx1, Idx2, Idx3;
  UINT8           Port, CboRingStopId;
  UINT8           RbtIdx1;
  UINT8           TmpRtidSize1, TmpCboIdx = 0;
  UINT8           UboxRingStopId;
  UINT8           RbtEntries[MAX_RBT_ENTRIES];
  UINT8           RbtEntries1[MAX_RBT_ENTRIES];
  UINT8           *RbtEntriesPtr;
  UINT32          Data32;

  UboxRingStopId = RING_STOP_ID_UBOX_HSX;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_HTRBT, host);

  //
  // Preload the Route Back Table (RBT) entries with the ringstopid of the agents that will use the entries.
  // IVT RBT operates in Dynamic mode wherein all the fields of an entry are updated each time a 
  // a transaction is sent out. But since the response might be coming back on different port, it is required 
  // to preload the RBT entries based on the RTID used by each agent.
  //

  MemSetLocal (RbtEntries, 0x00, sizeof (RbtEntries));
  MemSetLocal (RbtEntries1, 0x00, sizeof (RbtEntries));

  //
  // Fill the ring stop id for the RTID entries used by Ubox; Ubox is always assigned RTIDs 1-3.
  // no need to fill the WB RTID entry since the WB is shared by all CBos and there is no fixed ring
  // stop id associated with it.
  //
  RbtIdx1 = 1;
  for (Idx2 = 0; Idx2 < QpiInternalGlobal->UboxRtids; ++Idx2) {
    RbtEntries [RbtIdx1++] = UboxRingStopId;
  }

  RbtIdx1 = 1;
  for (Idx2 = 0; Idx2 < QpiInternalGlobal->UboxRtids; ++Idx2) {
    RbtEntries1 [RbtIdx1++] = UboxRingStopId;
  }

  //
  // In all BT modes except mode 2, all agents use (at most) identical RTIDs in Local first pool, Local second pool,
  // Remote First pool and Remote second pool (if applicable).  However, in some cases (e.g. MESeg-only mode),
  // Local Cbo-0's first pool and second pool may be slighly asymetrical.
  // In HSX BT mode 2, RBT entries 0-47 are allocated for Local first pool, remote socket uses the same entries (0-47) 
  // which overlaps with local first pool.
  // Entries 64-127 are allocated for Local second pool,there is no Remote second pool in this mode.
  //
  if ((QpiInternalGlobal->BtMode != BT_MODE_2) && (QpiInternalGlobal->BtMode != BT_MODE_3)) {
    RbtIdx1 = 65;                           
    for (Idx2 = 0; Idx2 < QpiInternalGlobal->UboxRtids; ++Idx2) {
      RbtEntries [RbtIdx1++] = UboxRingStopId;
    }
    // 
    // Fill the ring stop id for the entries used by all Cbos. No need to fill the ExtraRTIDs since no
    // agent is using them. Cbos 0 - (maxcbo-1) assigned ring stopi id 0 - (maxcbo-1) by the h/w. The loop below
    // fills in the RBT entries for each Cbo with each itteration handling both the Cbo's 1st and 2nd pool.
    // 
    TmpCboIdx = 3 + QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2; // Cbo RTIDs start from index 3
    MemCheckBoundary(host, TmpCboIdx, (MAX_CBO * 2 + 4)); // Klocwork 1866, 1867: Array 'QpiInternalGlobal->RtidBase[0]' of size 52 may use index value(s) 53..253.
    for (Idx3 = 3; Idx3 < TmpCboIdx; Idx3 = Idx3 + 2) { //Process both 1st and 2nd pool during each loop
      CboRingStopId = (Idx3 - 3) / 2; // Derive the Cbo number from Idx
      RbtIdx1 = QpiInternalGlobal->RtidBase[0][Idx3];       //Starting index of Cbo's 1st pool
      TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3];  //# of RTIDs in Cbo's 1st pool
      for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
        RbtEntries [RbtIdx1++] = CboRingStopId;
      }
      RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3+1];  //Starting index of Cbo's 2nd pool
      TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3+1];  //# of RTIDs in Cbo's 2nd pool
      for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
        RbtEntries [RbtIdx1++] = CboRingStopId;
      }
    }
  } else if (QpiInternalGlobal->BtMode == BT_MODE_3) {
      TmpCboIdx = 3 + QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2; // Cbo RTIDs start from index 3
      for (Idx3 = 3; Idx3 < TmpCboIdx; Idx3 = Idx3 + 2) { 
        CboRingStopId = (Idx3 - 3) / 2; // Derive the Cbo number from Idx
        if ((CboRingStopId >> 1) % 2) {
          RbtIdx1 = QpiInternalGlobal->RtidBase[0][Idx3];       //Starting index of Cbo's 1st pool
          TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3];  //# of RTIDs in Cbo's 1st pool
          for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
           RbtEntries1 [RbtIdx1++] = CboRingStopId;
          }
          
          RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3+1];  //Starting index of Cbo's 2nd pool
          TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3+1];  //# of RTIDs in Cbo's 2nd pool
          for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
            RbtEntries1 [RbtIdx1++] = CboRingStopId;
          }
        } else {
          RbtIdx1 = QpiInternalGlobal->RtidBase[0][Idx3];       //Starting index of Cbo's 1st pool
          TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3];  //# of RTIDs in Cbo's 1st pool
          for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
           RbtEntries [RbtIdx1++] = CboRingStopId;
          }
          
          RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3+1];  //Starting index of Cbo's 2nd pool
          TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3+1];  //# of RTIDs in Cbo's 2nd pool
          for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
            RbtEntries [RbtIdx1++] = CboRingStopId;
          }
        }
      }
  } else {            //bt mode 2 
    TmpCboIdx = 3 + QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] * 2; // Cbo RTIDs start from index 3
    for (Idx3 = 3; Idx3 < TmpCboIdx; Idx3 = Idx3 + 2) { //Process both 1st and 2nd pool during each loop
      CboRingStopId = (Idx3 - 3) / 2; // Derive the Cbo number from Idx

      // Program lower-half of RBT.
      // Depending on BtMode2 mode (Alt-0 vs. Alt-1), RBT lower-half is programmed using either 
      // A) Cbo's 1st local pool or B) Cbo's remote pool.
      if (QpiInternalGlobal->BtMode2Alt != 1) {               //Alt-0:
        RbtIdx1 = QpiInternalGlobal->RtidBase[0][Idx3];       //Starting RTID of Cbo's 1st local pool
        TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3];  //# of RTIDs in Cbo's 1st local pool
      } else {                                                //Alt-1:
        RbtIdx1 = QpiInternalGlobal->RtidBase[1][Idx3];       //Starting RTID of Cbo's Remote pool
        TmpRtidSize1 = QpiInternalGlobal->RtidSize[1][Idx3];  //# of RTIDs in Cbo's Remote pool
      }
      for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
        RbtEntries [RbtIdx1++] = CboRingStopId;
        }

      // Program upper-half of ROT.
      // Depending on RtMode2 mode (Alt-0 vs. Alt-1), RBT upper-half is programmed using either
      // A) Cbo's 2nd local pool or B) Cbo's 1st local pool. 
      if (QpiInternalGlobal->BtMode2Alt != 1) {                 //Alt-0:
        RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3+1];  //Starting RTID of Cbo's 2nd local pool
        TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3+1];  //# of RTIDs in Cbo's 2nd local pool
      } else {                                                  //Alt-1:
        RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3];    //Starting RTID of Cbo's 1st local pool (which is identical to Cbo's 2nd local pool)
        TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3];    //# of RTIDs in Cbo's 1st local pool (which is identical to Cbo's 2nd Local pool)
      }
      for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
        RbtEntries [RbtIdx1++] = CboRingStopId;
      }

      // For CPU_BDX, there is a second pool added in xRTID=0 for local HA communication.
      // Thus, we need to check these RTIDs while allocating the RBT entries
      if (host->var.common.cpuType == CPU_BDX) {
        if ((QpiInternalGlobal->RtidBase[2][Idx3] != 0) &&
            (QpiInternalGlobal->RtidSize[2][Idx3] > 0)) {
          // Set the RTID base as the RBT value
          RbtIdx1 = QpiInternalGlobal->RtidBase[2][Idx3];
          TmpRtidSize1 = QpiInternalGlobal->RtidSize[2][Idx3];
          for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
            RbtEntries [RbtIdx1++] = CboRingStopId;
          } // for loop - Idx2
        } // check whether second local xRTID=0 pool
      } // cpuType check
        
      RbtIdx1 = 64 + QpiInternalGlobal->RtidBase[0][Idx3+1];  //Starting index of Cbo's 2nd pool
      TmpRtidSize1 = QpiInternalGlobal->RtidSize[0][Idx3+1];  //# of RTIDs in Cbo's 2nd pool
      for (Idx2 = 0; Idx2 < TmpRtidSize1; ++Idx2) {
        RbtEntries [RbtIdx1++] = CboRingStopId;
      }
    } //For each Cbo index (Idx3 loop)
  }//BTMode-2


  QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; RBT Entries: "));
  
  if ((host->var.common.cpuType == CPU_BDX) && (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE)) {
    for (RbtIdx1 = 0; RbtIdx1 < MAX_RBT_ENTRIES; ++RbtIdx1) {
      // In BDX- 4S-COD, two Cbos (one from even cluster and other from odd cluster) share the same set of RTIDs.
      // The DNID[3] bit of a returning transaction is used to derive RBT[0] to find the right Cbo ring stop. 
      // Thus even Cbos are only seen in RBT entries. For other non-Cbo related RTIDs such as WB and UBOX,
      // this fact doesn't apply.
      if ((RbtIdx1 == 0) || (RbtIdx1 == 64) || (RbtEntries[RbtIdx1] == UboxRingStopId)) {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %03u : 0x%02X ", RbtIdx1, RbtEntries[RbtIdx1]));
      } else {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %03u : 0x%02X ", RbtIdx1, (RbtEntries[RbtIdx1] << 1)));
      }
    } // RbtIdx1 loop
  } else {
    for (RbtIdx1 = 0; RbtIdx1 < MAX_RBT_ENTRIES; ++RbtIdx1) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %03u : 0x%02X ", RbtIdx1, RbtEntries[RbtIdx1]));
    }
  } // 4S-COD check

  if (QpiInternalGlobal->BtMode == BT_MODE_3) {
    QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n\n; RBT1 Entries: "));
    for (RbtIdx1 = 0; RbtIdx1 < MAX_RBT_ENTRIES; ++RbtIdx1) {
      QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\n; %03u : 0x%02X ", RbtIdx1, RbtEntries1[RbtIdx1]));
    }
  }
  
  // Update QPI Agent's RBT-related fields such as RBT entries, RBT mode, ROT, inhibit_write, local_vs_remote, etc.
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if ( QpiInternalGlobal->CpuList & (1 << Idx1)) { 
      for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
        if (SocketData->Cpu[Idx1].LinkData[Port].Valid == TRUE) {
          //
          // For BTMode-2 Alt-1 Mode, adjust QPIQRT to accomodate the larger RTID allocation
          // Note: 'inhibit_write' and 'local_vs_remote' settings will also be done in ProgramSystemRoute()
          // but also need to be done in conjunction with the ROT changes.
          //
          if ((QpiInternalGlobal->BtMode == BT_MODE_2) && (QpiInternalGlobal->BtMode2Alt == 1)) {
            Data32  = QpiReadPciCfg (host, Idx1, Port, QPIQRT_QPIAGENT_LL_REG);
            Data32 = (Data32 & 0xFFF0FFFF) | (0xE  << 16);   //rot (Bits[19:16]) = 1110b (Use xRTID section of RBT except for regular remote RTIDs)
            Data32 = (Data32 & 0xFFFF00FF) | (0xFF <<  8);   //inhibit_write (Bits[15:8]) = 0xFF (D2C disabled)
            Data32 = (Data32 & 0xFFFFFF00) | (UINT32)((~(1 << Idx1)) & 0x000000FF); //local_vs_remote (Bits[7:0]) set so only the current socket is tagged as local
            QpiWritePciCfg (host, Idx1, Port, QPIQRT_QPIAGENT_LL_REG, Data32);
          }//BTMode2 Alt-1 (128/48)

          //
          // Set RBT mode
          //
          Data32 = QpiReadPciCfg (host, Idx1,Port, QPIAGCTRL_QPIAGENT_LL_REG);
          Data32 = Data32 & 0xFFFFFFFE; // Set rbt_mode to 0 (Dynamic)
          QpiWritePciCfg (host, Idx1,Port, QPIAGCTRL_QPIAGENT_LL_REG, Data32);

          //
          // Program the RBT entries; to program an entry, the entry must be first selected using
          // QPIRBTSO register; in other words this register must be first set with the index of the 
          // entry to be programmed. Any write to this register automatically increases the index to 
          // next entry. So we need to set this this register to entry 0; then program the RBT entries
          // using QPIRBTSD register.
          //
          for (RbtIdx1 = 0; RbtIdx1 < MAX_RBT_ENTRIES; ++RbtIdx1) {
            if ((QpiInternalGlobal->BtMode == BT_MODE_3)) {
              //
              // Account for twisted topologies in COD mode
              //
              if ((QpiInternalGlobal->TwistedQpiLinks == FALSE) || (Idx1 == 0)) {
                if (Port == 0) {
                  RbtEntriesPtr = RbtEntries;  
                } else {
                  RbtEntriesPtr = RbtEntries1;  
                }
              } else {
                if (Port == 0) {
                  RbtEntriesPtr = RbtEntries1;  
                } else {
                  RbtEntriesPtr = RbtEntries;  
                }
              }
            } else {
              RbtEntriesPtr = RbtEntries;
            }
            Data32 = 0x100 | (RbtEntriesPtr [RbtIdx1] & 0x1F);
            // In BDX- 4S-COD, two Cbos (one from even cluster and other from odd cluster) share the same set of RTIDs.
            // The DNID[3] bit of a returning transaction is used to derive RBT[0] to find the right Cbo ring stop. 
            // Thus even Cbos are only seen in RBT entries. For other non-Cbo related RTIDs such as WB and UBOX,
            // this fact doesn't apply.
            if ((host->var.common.cpuType == CPU_BDX) && (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) && 
                (RbtIdx1 != 0) && (RbtIdx1 != 64) && (RbtEntriesPtr[RbtIdx1] != UboxRingStopId)) {
              Data32 = 0x100 | ((RbtEntriesPtr [RbtIdx1] & 0x1F) << 1);
            } // when 4S-COD is enabled and a Cbo related RBT writeback
            QpiWritePciCfg (host, Idx1,Port, QPIRBTSO_QPIAGENT_LL_REG, 0x100 | (RbtIdx1 & 0x7f) ); // Set WE bit, and set offset to 0  
            QpiWritePciCfg (host, Idx1,Port, QPIRBTSD_QPIAGENT_LL_REG, Data32);
          }//For each RBT entry
        }//Port is valid
      }//For each Port
    }//Socket is populated
  }//For each Socket

  return QPI_SUCCESS;
}


QPI_STATUS
CheckS3ResumePath (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
{
#ifdef EP_FEATURES_ONLY
  UINT8      Ctr, Port;
  BOOLEAN    S3ResumeFail;

  //
  // S3 Resume check and data structure update. If we are not in S3 resume path, update the NVRAM
  // structure with the system topology info. If we are in S3 resume path, verify that the system topology
  // matches the pre-S3 resume path topology info stored previously in NVRAM. We have to do the check
  // only when no resets are pending; this is to make sure any transient system configuration is not 
  // interpreted as system configuration change in S3 resume path.
  //

  // When we are not in S3 Resume path copy the system configuration info to the output structure
  if (host->var.common.bootMode == NormalBoot) {
    host->nvram.qpi.Cpus = QpiInternalGlobal->CpuList;
    host->nvram.qpi.Cbos = QpiInternalGlobal->CboList[QpiInternalGlobal->SbspSoc];
    for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        MemCopy ((UINT8 *)host->nvram.qpi.LepInfo[Ctr], (UINT8 *)SocketData->Cpu[Ctr].LinkData, sizeof (host->nvram.qpi.LepInfo[Ctr]));
      }
    }
    return QPI_SUCCESS;
  }

  // We are in S3 resume path; check to make sure config didn't change
  S3ResumeFail = FALSE;
  if (host->var.common.resetRequired == 0) {
    if (host->nvram.qpi.Cpus != QpiInternalGlobal->CpuList || host->nvram.qpi.Cbos != QpiInternalGlobal->CboList[QpiInternalGlobal->SbspSoc]) {
      S3ResumeFail = TRUE;
    }
    for (Ctr = 0; Ctr < MAX_CPU_SOCKETS; ++Ctr) {
      if (S3ResumeFail == TRUE) {
        break;
      }
      if (SocketData->Cpu[Ctr].Valid == TRUE) {
        for (Port = 0; Port < MAX_QPI_PORTS; ++Port) {
          if (host->nvram.qpi.LepInfo[Ctr][Port].Valid != SocketData->Cpu[Ctr].LinkData[Port].Valid) {
            S3ResumeFail = TRUE;
            break;
          }
        }
      }
    }
  }

  if (S3ResumeFail == TRUE) {
    QpiCheckPoint (0xFF, 0xFF, 0xFF, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_TOPOLOGY_MISMATCH, host);
    QpiDebugPrintFatal ((host, QPI_DEBUG_ERROR, "\n Topology Doesn't Match - S3 Resume Failed."));
    QPI_ASSERT (FALSE, ERR_S3_RESUME, MINOR_ERR_S3_RESUME_TOPOLOGY_MISMATCH);
  }
#endif

  return QPI_SUCCESS;
}


QPI_STATUS
SetupHtBase (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL  *QpiInternalGlobal
  )
/*++

Routine Description:

  Program the Home Tracker Base setting

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData  - socket specific data structure

Returns:

  0 - Successful
  1 - Failures
  
--*/
{
#ifdef EP_FEATURES_ONLY
  UINT8           Idx1, Idx2, Idx3, Ha;
  UINT32          HtBaseHigh, TempBase;

  QpiCheckPoint(0xFF, 0xFF, 0xFF, STS_PROTOCOL_LAYER_SETTING, MINOR_STS_PROGRAM_HTRBT, host);

  //
  // Program the HT base for NID 0, 1, 4 & 5. Other NIDs are don't care since only 1S & 2S configs use SHT.
  // HT Baselow setting is always zero since the base is always 0 or 64.
  //
  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {   // for 1S & 2S configs only, valid checking will filter out sockets other than socket 0 & 1
    if (SocketData->Cpu[Idx1].Valid == TRUE) {      

      HtBaseHigh = 0;
      for (Idx2 = 0; Idx2 < 4; ++Idx2) {
        TempBase = (QpiInternalGlobal->HtBase[Idx1] >> ((Idx2 * 8) + 3)) & 0xF; // Extract bits[6:3] of the base value for each NID
        Idx3 = (((Idx2 / 2) * 4) + (Idx2 % 2)) * 4; //Calculate the index into the HTBase register for NID 0, 1, 4 or 5
        HtBaseHigh = HtBaseHigh | (TempBase << Idx3);
      }

      for (Ha = 0; Ha < QpiInternalGlobal->TotHa[Idx1]; ++Ha) {
        QpiWritePciCfg (host, Idx1, Ha, HTBASE_HA_CFG_REG, HtBaseHigh);
        QpiWritePciCfg (host, Idx1, Ha, HTBASELOW_HA_CFG_REG, 0);
      }
    }
  }
#else
  QPI_ASSERT(FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_SOCKET_TYPE); // EX should not reach this point becasue BTMode always enabled
#endif

  return QPI_SUCCESS;
}

QPI_STATUS
ProgramMsrForWa (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Program MSR as workarounds for QPI RC.
  SBSP writs the value into UBOX Sticky Scratch 07 in each cpu socket, BSP itself as well as all PBSPs
  At subsequent warm resets, each cpu socket will program MSRs as instructed by the value in its UBOX Sticky Scratch 07.
 
Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

--*/
{
  UINT8     Idx1;
  UINT32    Data32;
  PLATFORM_INFO_N0_PCU_FUN0_HSX_BDX_STRUCT  DataPpin;
  QPI_TIMEOUT_CTRL_CBOBC_COH_STRUCT         QpiTorTimeout;

  Data32 = 0;
  
  //
  //  QPI CBO TOR Timeout
  // 
  //  Bit format for UBOX Sticky Scratch 07:                                 
  // 
  //  [30]:         - 0 = Disable, 1 = Enable
  //  [23-20]:      - Total Cbo in a cpu socket
  //                       
  Data32 |= ((1 << 30) | ((QpiInternalGlobal->TotCbo[QpiInternalGlobal->SbspSoc] & 0x0F) << 20));

  //
  //  PPIN Opt-in
  //  Bit format for UBOX Sticky Scratch 07:                                 
  //  [31]:         - 0 = Disable, 1 = Enable
  //
  if (host->setup.qpi.PPINrOptIn) {
      DataPpin.Data = QpiReadPciCfg (host, QpiInternalGlobal->SbspSoc, 0, PLATFORM_INFO_N0_PCU_FUN0_REG); 
      if(DataPpin.Bits.ppin_cap) {
        Data32 |= (1 << 31);
      } else {
        QpiDebugPrintInfo1 ((host, QPI_DEBUG_INFO1, "\nPPIN opt-in enabled but not supported."));
      }
  }

  for (Idx1 = 0; Idx1 < MAX_CPU_SOCKETS; ++Idx1) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {

      QpiTorTimeout.Data = QpiReadPciCfg (host, Idx1, 0, QPI_TIMEOUT_CTRL_CBOBC_COH_REG);

      // program Time out values for HA/CBO/Ubox
      QpiTorTimeout.Bits.enabletortimeout   = 1;          //cbo timeout
      QpiTorTimeout.Bits.entry_ctr_inc_ctl  = 0x6;
      QpiTorTimeout.Bits.level1_offset      = 0x16;
      QpiTorTimeout.Bits.level3_offset      = 0x16;
      QpiTorTimeout.Bits.level4_offset      = 0x16;
      QpiTorTimeout.Bits.level6_offset      = 0x16;  

      QpiWritePciCfg (host, Idx1, 0, QPI_TIMEOUT_CTRL_CBOBC_COH_REG, QpiTorTimeout.Data);
   
    }
  }

  return QPI_SUCCESS;
}


QPI_STATUS
QpiProgramBGFOverrides (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Program BGF Pointer Seperation Overrides as workarounds for QPI RC.
  SBSP writes these values via Mailbox Commands to all populated sockets.  Values will take effect after a warm reset.

Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

--*/
{

  UINT8  Index;
  UINT32 MailBoxDataQpi = 0;
  UINT8  PtrSepValue;
  UINT8  PtrSepR3QPIGV, PtrSepR3QPINonGV;
  UINT8  PtrSepIIOGV, PtrSepIIONonGV;
  UINT8  PtrSepHAGV133, PtrSepHANonGV133;
  UINT8  PtrSepHAGV100, PtrSepHANonGV100;
  UINT8  PtrSepPCUGV, PtrSepPCUNonGV;
  UINT8  PtrSepR3QPIGV_IOT = 1;  // For HSX-A0/B0 WA (4986157)

  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG | SIMICS_FLAG))) {
    // Assign the pointer seperation values based on cpuType and cpuStepping
    PtrSepValue = GetDefaultPointerSeperationValues(host);
    PtrSepR3QPIGV = PtrSepIIOGV = PtrSepHAGV133 = PtrSepHAGV100 = PtrSepPCUGV = PtrSepValue;
    PtrSepR3QPINonGV = PtrSepIIONonGV = PtrSepHANonGV133 = PtrSepHANonGV100 = PtrSepPCUNonGV = PtrSepValue;

#ifdef BGF_PTR_SEP_IIO_WA
  // For HSX HSD 4987499 (bugeco 4906062)
  if ((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < B0_REV_HSX)) {
    PtrSepIIOGV = PtrSepIIONonGV = 2;
  } else { // HSX B0 onwards and BDX
    PtrSepIIOGV = PtrSepIIONonGV = 1;
  }
#endif


    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if ((host->var.qpi.RasInProgress == FALSE && (QpiInternalGlobal->CpuList & (1 << Index))) ||
          (host->var.qpi.RasInProgress == TRUE && host->var.qpi.RasSocId == Index)) {    
        // BIOS_MAILBOX_DATA: pointer separation data:
        // [2:0]: GV side pointer separation (pointer separation for R3PCIe, R3QPI, or HA)
        // [6:4]: Non-GV side pointer separation (pointer separation for PCIe, QPI, PCU, or MC)
        // Other bits reserved             
        MailBoxDataQpi = PtrSepR3QPIGV | (PtrSepR3QPINonGV << 4);
        if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping < C0_REV_HSX)) &&
            (host->setup.cpu.IotEn[Index])) { 
          MailBoxDataQpi = PtrSepR3QPIGV_IOT | (PtrSepR3QPINonGV << 4);  //HSX A0/B0 W/A (HSD-4986157)
        }
        SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_GV_PTR_OVERIDE | (MAILBOX_BIOS_BGF_ADDR_QPI_R3QPI << 8), MailBoxDataQpi);
        SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_GV_PTR_OVERIDE | (MAILBOX_BIOS_BGF_ADDR_IIO_R2PCIE << 8), PtrSepIIOGV | (PtrSepIIONonGV << 4));
        SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_GV_PTR_OVERIDE | (MAILBOX_BIOS_BGF_ADDR_HSX_HA_MC_133 << 8), PtrSepHAGV133 | (PtrSepHANonGV133 << 4));
        SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_GV_PTR_OVERIDE | (MAILBOX_BIOS_BGF_ADDR_HSX_HA_MC_100 << 8), PtrSepHAGV100 | (PtrSepHANonGV100 << 4));
        SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_GV_PTR_OVERIDE | (MAILBOX_BIOS_BGF_ADDR_HSX_PCU_R3QPI << 8), PtrSepPCUGV | (PtrSepPCUNonGV << 4));
      } // RAS if checks 
    } // socket loop
    host->var.common.resetRequired |= POST_RESET_WARM;
  } // emulation check

  return QPI_SUCCESS;
}

QPI_STATUS
QpiPcuMiscConfig (
  struct sysHost      *host,
  QPI_SOCKET_DATA     *SocketData,
  QPI_HOST_INTERNAL_GLOBAL   *QpiInternalGlobal
  )
/*++

Routine Description:

  Issue mailbox commands to configure miscellanout options
 
Arguments:

  host  - Pointer to the system host (root) structure
  SocketData - Pointer to socket specific data
  QpiInternalGlobal - QPIRC Internal Global data

Returns:

--*/
{


  UINT8  Index, CboIdx, Idx;
  UINT32 Data32=0x0, Data32_1;

  
  if (!(host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG))) {
    if ((host->var.qpi.OutClusterOnDieEn == 1) || (host->var.qpi.OutE2EParityEn == 1) || (QpiInternalGlobal->IodcEn == 1) || (QpiInternalGlobal->LaneDropPcodeFixEn == TRUE)) {
      for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
        if (SocketData->Cpu[Index].Valid == TRUE) {
//#ifndef HW_EMULATION
          SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, 0);
          Data32 = QpiReadPciCfg (host, Index, 0, BIOS_MAILBOX_DATA_PCU_FUN1_REG);  
          QpiDebugPrintInfo0 ((host, QPI_DEBUG_INFO0, "\n;MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG returned %u", Data32));
//#else
//          Data32 = 0;
//#endif
          if (host->var.qpi.OutClusterOnDieEn == 1) {     //COD ENABLE
             Data32 = Data32 | BIT31;
          }

          if (host->var.qpi.OutE2EParityEn == 1) {        //E2E PARITY ENABLE
            if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping >= B0_REV_HSX)) ||
                 (host->var.common.cpuType == CPU_BDX) ) {
               Data32 = Data32 | BIT26;
            }
          }

          // (HSD-4969795):HSX Clone : QPI: fast retrain loops using reset or L1 entry/exit causes lane drop and EP Init Aborts / EX DFO
          // Enables pcode fix instead of SI fix
          if (QpiInternalGlobal->LaneDropPcodeFixEn) {
            if (((host->var.common.cpuType == CPU_HSX) && (host->var.common.stepping >= D0_REV_HSX)) ||
                 (host->var.common.cpuType == CPU_BDX)) {
               Data32 = Data32 | BIT20;
            }  
          }

          // IODC Feature - Supported for BDX only
          if (QpiInternalGlobal->IodcEn == 1) {
              Data32 = Data32 | BIT19;
          }

          SendMailBoxCmdToPcode(host, Index, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, Data32);  
        }
      }
    }
  }

  if ((host->var.qpi.OutClusterOnDieEn == 1) && (host->var.qpi.SysConfig == SYS_CONFIG_2SEP)) {     //COD ENABLE
    for (Index = 0; Index < MAX_CPU_SOCKETS; ++Index) {
      if (SocketData->Cpu[Index].Valid == TRUE) {

        if (Index == 0) {
          Data32 = 0x0;
          Data32_1 = 0x00100010; 
        } else if (QpiInternalGlobal->TwistedQpiLinks == TRUE) {
          Data32 = 0x00010001;
          Data32_1 = 0x0;
                  
        } else {
          Data32 = 0x0;
          Data32_1 = 0x00010001; 
        
        }
        
        for (CboIdx = 0; CboIdx < QpiInternalGlobal->TotCbo[Index]; CboIdx++) {
          if ((CboIdx >> 1) % 2) {
            //Odd
            QpiWritePciCfg (host, Index, CboIdx, QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG, Data32_1);
          } else {
            //Even
            QpiWritePciCfg (host, Index, CboIdx, QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG, Data32);
          }
        }
      }
    }
  } else if (QpiInternalGlobal->Sys4SClusterOnDieEn == TRUE) {
    // Based on the socket's position, set the QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG for the directly connected sockets
    // so that PCODE can sync the same during the warm-reset and release them from reset.
    for (Index = 0; Index < MAX_CPU_SOCKETS; Index++) {
      // If the socket is VALID,
      if (SocketData->Cpu[Index].Valid == TRUE) {
        if ((SocketData->Cpu[Index].SocId == GetSbspSktIdFromLocal(host)) || (CheckCpuConnectedToLegCpu (host, SocketData, QpiInternalGlobal, Index) == FALSE)) {
          continue;
        } else {
          // Bits [1:0] in QPI_ROUTING_TABLE indicates the port through which this given socket
          // is connected to legacy socket. Assign this port to routing table of all the Cbos
          for (Idx=0; Idx < MAX_TREE_NODES; Idx++) {
            if ((SocketData->SbspMinPathTree[Idx].Node.SocId == SocketData->Cpu[Index].SocId)) {
              Data32 = (UINT8) SocketData->SbspMinPathTree[Idx].Node.ParentPort;
              break;
            }
          } // Idx loop
        } // legacy socket or sockets which are 1 hop away
      } // socket valid
      
      for (CboIdx = 0; CboIdx < QpiInternalGlobal->TotCbo[Index]; CboIdx++) {
        //
        QpiWritePciCfg (host, Index, CboIdx, QPI_ROUTING_TABLE_SHADOW_CBO_CFG_REG, Data32);
      } // CboIdx loop
    } // Index loop
  } // COD and topology check

  return QPI_SUCCESS;
}

/**
  Enable Mesh Mode

  This routine performs the necessary setup for mesh mode and send Mesh Mode
  mailbox command to Pcode.

  @param  host                    Pointer to the system host (root) structure.

  @retval None

**/
VOID
UncoreEnableMeshMode (
  struct sysHost             *host
  )
{
}
