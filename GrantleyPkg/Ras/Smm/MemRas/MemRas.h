//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2008 - 2014 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    MemRas.h
   
Abstract:
    MemRas functions and platform data, which needs to be 
    ported.

-----------------------------------------------------------------------------*/


#ifndef _MEM_RAS_H_
#define _MEM_RAS_H_

//
// Statements that include other header files
//

#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>

#include <Protocol/MemRasProtocol.h>
#include <Protocol/IioUds.h>
#include <Protocol\GlobalNvsArea.h>
#include <Protocol/CpuCsrAccess.h>
#include <Protocol\SmmSxDispatch.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/SmmPeriodicTimerDispatch2.h>

#include <Token.h>   //AptioV server override
extern EFI_GUID  gEfiSmmPeriodicTimerDispatch2ProtocolGuid;
EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL   *mSmmPeriodicTimerDispatch;

//
// Status get/set commands; used by GetOrSetErrorHandlingStatus()
//
typedef enum {
  UpdateEventProgressStatus,
  UpdateEventPendingStatus,
  GetEventProgressStatus,
  GetEventPendingStatus
} ERR_STATUS_OPCODE;

#define ENTRY_INVALID 0xFF


extern   BIOS_ACPI_PARAM *mAcpiParameter;
extern   EFI_CPU_CSR_ACCESS_PROTOCOL    *mCpuCsrAccess;

UINT32   SchedulingCall (VOID *, ...);  // The parameter MUST be less or equal to 4

VOID *
EFIAPI
AsmFlushCacheLine (
  IN      VOID                      *LinearAddress
  );

EFI_MEM_RAS_PROTOCOL  *mMemRas;

typedef struct {
  UINTN                          Signature;
  EFI_HANDLE                     Handle;
  EFI_MEM_RAS_PROTOCOL           *mMemRas; // Policy protocol this driver installs
} EFI_MEM_RAS_DRIVER_PRIVATE;

EFI_STATUS
InitializeMemRasData(
);

EFI_STATUS
EFIAPI
IsMemNodeEnabled(
    IN UINT8 Node,
    OUT BOOLEAN *IsMemNodeEnabledFlag
);

EFI_STATUS
EFIAPI
EnableErrorSignallingtoBIOS(
    IN UINT8 Node
);


EFI_STATUS
EFIAPI
GetCurrentMemoryNodeState (
    IN UINT8      Node,
    IN OUT UINT8 *MemState
);

void
EFIAPI
MemWritePciCfg (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset,
    IN UINT32   Data
);

UINT32
EFIAPI
MemReadPciCfg (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset
);
//
//4987134 start
//

UINTN
EFIAPI
GetCpuCfgPciAddress (
    IN UINT8    SktId,
    IN UINT8    BoxInst,
    IN UINT32   Offset
);

//
//4987134 end
//


void
EFIAPI
PciWriteCfg (
    IN UINT8    SktId,
    IN UINT32   Offset,
    IN UINT32   Data
);

UINT32
EFIAPI
PciReadCfg (
    IN UINT8    SktId,
    IN UINT32   Offset
);



void
EFIAPI
GetOrSetErrorHandlingStatus (
  IN UINT8                           NodeId,
  IN ERR_STATUS_OPCODE               Opcode,
  IN OUT UINT32                      *Value
 );

BOOLEAN
IsErrorExcdForThisRank(
    IN UINT8 NodeId,
    IN UINT8 DdrCh,
    IN UINT8 Rank
 );

void
ClearErrorsForThisRank(
    IN UINT8 NodeId,
    IN UINT8 DdrCh,
    IN UINT8 Rank
 );

EFI_STATUS
InitializeMpst (
  );

void
EFIAPI
GetBitMapOfNodeWithEventInProgress(
    IN OUT UINT32                     * EventProgressBitmap
  );

void
EFIAPI
GetBitmapOfNodeWithErrors(
    IN OUT UINT32                     * ErrorBitmap
  );

void
EFIAPI
GetLastErrInfo(
    IN UINT8                          NodeId,
    IN UINT8                          *DdrCh,
    IN UINT8                          *Dimm,
    IN UINT8                          *Rank
 );

EFI_STATUS
EFIAPI
OpenPamRegion(
    IN UINT8 SocketId
 );

EFI_STATUS
EFIAPI
RestorePamRegion(
    IN UINT8 SocketId
 );

EFI_STATUS
EFIAPI
GetErrorLogSysAddr(
    IN  UINT8    NodeId,
    OUT UINT64   *ErrorLogSysAddr
 );

EFI_STATUS
EFIAPI EnablePatrolScrubForPoisonDetection(
    IN UINT8 NodeId
 );



UINT8
NodeToSkt(
  UINT8 Node
);

UINT8
NodeToHaNum
(
  UINT8 Node
);

UINT8
SktHaToNode
(
  UINT8 Skt,
  UINT8 HaNum
);

UINT8
SktHaToHaNodeId
(
  UINT8 Skt,
  UINT8 HaNum
);

UINT8
HaNodeIdToNode (
  UINT8 HaNodeId
);

UINT8
HaNodeIdToSkt (
  UINT8 haNodeId
);

UINT8
HaNodeIdToHa (
  UINT8 haNodeId
);

UINT8
NodeChToDdrCh(
  UINT8 node,
  UINT8 ch
);

/*
UINT8
GetHaCh(
  UINT8 node,
  UINT8 ch
);
*/

VOID
IniMemRasS3 (
  VOID
);

EFI_STATUS
EFIAPI
SystemAddressToDimmAddress (
        IN  UINT64  SystemAddress,
        OUT UINT8*  SocketId,
        OUT UINT8*  MemoryControllerId,
        OUT UINT8*  ChannelId,
        OUT UINT8*  DimmSlot,
        OUT UINT8*  DimmRank,
        OUT UINT32* Row,
        OUT UINT32* Col,
        OUT UINT8*  Bank,
        OUT UINT8*  BankGroup,
        OUT UINT8*  ChipSelect
 );

EFI_STATUS
EFIAPI
DimmAddressToSystemAddress(
    IN  UINT8   SocketId,
    IN  UINT8   MemoryControllerId,
    IN  UINT8   ChannelId,
    IN  UINT8   DimmSlot,
    IN  UINT8   DimmRank,
    IN  UINT32  Row,
    IN  UINT32  Col,
    IN  UINT8   Bank,
    IN  UINT8   BankGroup,
    IN  UINT8   ChipSelect,
    OUT UINT64* SystemAddress
 );

UINT8
GetLogicalToPhysicalRankId(
    IN UINT8  NodeId,
    IN UINT8  DdrCh,
    IN UINT8  LogicalRank
 );

typedef struct _TRANSLATED_ADDRESS {
  UINT64  SystemAddress;
  UINT8   HaNodeId;
  UINT8   Node;
  UINT8   SocketId;
  UINT8   MemoryControllerId;
  UINT64  ChannelAddress;
  UINT8   ChannelId;
  UINT8   LockStepEn;       // If lock step is enabled, only primary Channel ID is reported
  UINT64  RankAddress;
  UINT8   PhysicalRankId;
  UINT8   DimmSlot;
  UINT8   DimmRank;
  UINT32  Row;
  UINT32  Col;
  UINT8   Bank;
  UINT8   BankGroup;
  UINT8   ChipSelect;
} TRANSLATED_ADDRESS, *PTRANSLATED_ADDRESS;

typedef struct {
  UINT8 dimmEnabled;
}DIMM_INFO;

typedef struct _SMMELOG_CHANNELLIST {
  UINT8 chFailed;
  UINT8 spareLogicalRank;
  UINT8 spareInUse;
  UINT8 oldLogicalRank;
  //UINT8 oldLogicalRank[MAX_RANK_CH];
  //UINT8 MultiSpareRankCnt;
  UINT8 MultiSpareRank;
  UINT8 devTag[MAX_RANK_CH];
  UINT32 leakyBucket2ndCounter;
  DIMM_INFO dimmInfo[MAX_DIMM];
} SMMELOG_CHANNELLIST;

typedef struct _SMMELOG_IMC {
  UINT8 enabled;
  SMMELOG_CHANNELLIST channelList[MAX_CH];
} SMMELOG_IMC;

typedef struct _SMMELOG_HOST {
  SMMELOG_IMC imc[MAX_MEM_NODE];
  UINT8 numChPerHA;
} SMMELOG_HOST;

EFI_STATUS
EFIAPI
ClearUnhandledCorrError(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );


#define INVALID_MEM_NODE    0xFF

extern struct SystemMemoryMapHob *mSystemMemoryMap;
extern EFI_GUID gEfiMemoryConfigDataGuid;

//
// Macros used in Address Translation code
//
#define MAX_COL_BITS_SUPPORTED  12
#define MAX_ROW_BITS_SUPPORTED  18
#define MAX_BANK_BITS_SUPPORTED 3

#define MAX_COL_BITS_FIXED_MAP  10  // Max # of col bits that use fixed map
#define MAX_ROW_BITS_FIXED_MAP  12  // Max # of Row bits that use fixed map
#define MAX_BANK_BITS_FIXED_MAP 3   // Max # of bank its that use fixed map

#define MAX_ROW_BITS_DYNAMIC_MAP 6  // Max # of row bits that use dynamic map
#define MAX_COL_BITS_DYNAMIC_MAP 4  // Max # of col bits that use dynamic map

#define MAX_COL_BITS_SUPPORTED_DDR4  10
#define MAX_ROW_BITS_SUPPORTED_DDR4  18
#define MAX_BANK_BITS_SUPPORTED_DDR4 2
#define MAX_BG_BITS_SUPPORTED_DDR4   2
#define MAX_CS_BITS_SUPPORTED_DDR4	 3

#define MAX_COL_BITS_FIXED_MAP_DDR4  10  // Max # of col bits that use fixed map
#define MAX_ROW_BITS_FIXED_MAP_DDR4  12  // Max # of Row bits that use fixed map
#define MAX_BANK_BITS_FIXED_MAP_DDR4 2   // Max # of bank its that use fixed map
#define MAX_BG_BITS_FIXED_MAP_DDR4   2

#define MAX_ROW_BITS_DYNAMIC_MAP_DDR4 6  // Max # of row bits that use dynamic map
#define MAX_COL_BITS_DYNAMIC_MAP_DDR4 0  // Max # of col bits that use dynamic map

//
// Opcode used by address translation functions
//
#define EXTRACT_ADDR_BITS        0
#define STUFF_ADDR_BITS          1

//
// Temp fix!!!!!! Should be removed when all memory RAS modules
// move to use CpuCsrAccess protocol instead of MemRead/WritePciCfg routines
// These macros and structures should match the definition in
// CpuCsrAccess.h
//
#define  BOX_MC              11
#define  BOX_MCDDC           12
#define  BOX_MCIO            13

// CSR_OFFSET definition moved to MemHost.h


#endif  //_MEM_RAS_H_
