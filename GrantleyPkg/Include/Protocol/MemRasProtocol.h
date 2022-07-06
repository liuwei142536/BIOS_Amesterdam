//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemRasProtocol.h

Abstract:

  Mem Ras ProtocolCode

--*/

#ifndef _MEM_RAS_PROTOCOL_H_
#define _MEM_RAS_PROTOCOL_H_

#include <Token.h> // AptioV server override

#include <Guid/MemoryMapData.h>

#define EFI_MEM_RAS_PROTOCOL_GUID \
  {0x6d7e4a32, 0x9a73, 0x46ba, 0x94, 0xa1, 0x5f, 0x2f, 0x25, 0xef, 0x3e, 0x29}

#define EFI_FORWARD_DECLARATION(x) typedef struct _##x x
EFI_FORWARD_DECLARATION (EFI_MEM_RAS_PROTOCOL);
//Grangeville AptioV override start - To remove RAS_DEBUG messages in release mode
//###DEBUG #define  RAS_DEBUG(x)   if (TRUE) { DebugPrint x ; }
//#define  RAS_DEBUG(x) 
#ifdef SERIAL_DBG_MSG
#define  RAS_DEBUG(x)  { DebugPrint x ; }
#else
#define  RAS_DEBUG(x)
#endif
//Grangeville AptioV override end - To remove RAS_DEBUG messages in release mode
extern UINT64 mRasFlags;

// BIT0-BIT31 will be checked by MRC code
#define  RAS_FLAG_KEEP_POWERON                BIT0
#define  RAS_FLAG_INJ_ECC                     BIT30
#define  RAS_FLAG_RESERVE                     BIT31   // please avoid use this to avoid sign extension

// BIT32-BIT61 will be consumed by RAS only
#define  RAS_FLAG_MRC_DEBUG                   BIT63
#define  RAS_FLAG_TIME_SLICING                BIT62
#define  RAS_FLAG_RAS_DEBUG                   BIT61
#define  RAS_FLAG_QUIESCE_MIGRATION           BIT60


#define  RAS_PARAM_MEASURE_DATA               0
#define  RAS_PARAM_QUICSES_DATA               1

#define  SAVE_DISABLE_CKE_LOW                 1
#define  RESTORE_CKE_LOW                      2
#define  B2B_DCK_ENABLE                       1
#define  B2B_DCK_DISABLE                      0

#define PARAM_UNUSED  0xFF // Used in place of ChIdOrBoxInst in MemRead/WritePciCfg for MC functional blocks
                           // that have only 1 instance per MC

//
// RAS Event type
//
typedef enum {
  NEW_EVENTS,             // New events (events yet to be handled)
  EVENTS_IN_PROGRESS,     // Events that are in progress
} EVENT_TYPE;


typedef
UINT32
(EFIAPI *EFI_READ_JCK_CFG) (
  IN UINT8    NodeId,
  IN UINT8    DdrCh,
  IN UINT8    Link,
  IN UINT32   Offset
  );

typedef
void
(EFIAPI *EFI_WRITE_JCK_CFG) (
  IN UINT8    NodeId,
  IN UINT8    DdrCh,
  IN UINT8    Link,
  IN UINT32   Offset,
  IN UINT32   Data
  );

typedef
EFI_STATUS
(EFIAPI *EFI_MEM_NODE_ONLINE) (
  IN UINT8 Node,
  IN OUT UINT8 *MemHpStatus
  );

typedef
EFI_STATUS
(EFIAPI *EFI_MEM_NODE_OFFLINE) (
  IN UINT8 Node,
  IN OUT UINT8 *MemHpStatus
  );
  
//
//4987134 start
//

typedef
UINTN
(EFIAPI *EFI_CPU_CFG_PCI_ADDRESS) (
    IN UINT8    SktId,
    IN UINT8    BoxInst,
    IN UINT32   Offset
);
//
//4987134 end
//

typedef
UINT32
(EFIAPI *EFI_MEM_READ_PCI_CFG) (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset
);

typedef
void
(EFIAPI *EFI_MEM_WRITE_PCI_CFG) (
    IN UINT8    SktOrNodeId,
    IN UINT8    ChIdOrBoxInst,
    IN UINT32   Offset,
    IN UINT32   Data
);

typedef
UINT32
(EFIAPI *EFI_PCI_READ_CFG) (
    IN UINT8    SktId,
    IN UINT32   Offset
);

typedef
void
(EFIAPI *EFI_PCI_WRITE_CFG) (
    IN UINT8    SktId,
    IN UINT32   Offset,
    IN UINT32   Data
);

typedef
void
(EFIAPI *EFI_B2B_OPERATION) (
    IN UINT64  Command,
    IN UINT64* ParamArray
);

typedef
EFI_STATUS
(EFIAPI *EFI_IS_MEM_NODE_ENABLED) (
  IN UINT8 Node,
  OUT BOOLEAN *IsMemNodeEnabledFlag
  );

typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CONFIG_IN_QUIESCE) (
  IN UINT8 Operation
  );

typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_ERROR_SIGNAL_TO_BIOS) (
  IN UINT8 Node
  );

typedef
EFI_STATUS
(EFIAPI *EFI_REENABLE_PATROL_SCRUB_ENGINE) (
  IN UINT8 NodeId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_PATROL_SCRUB_ENGINE) (
  IN UINT8 NodeId
  );

typedef
EFI_STATUS
(EFIAPI *EFI_MIGRATION_POSSIBLE) (
    IN UINT8 MasterNodeId,
    IN UINT8 SlaveNodeId,
    OUT BOOLEAN *MigrationPossibleFlag,
    IN BOOLEAN IsMirrorMigration
    );

typedef
EFI_STATUS
(EFIAPI *EFI_SETUP_MEM_MIGRATION) (
    IN UINT8 SrcNodeId,
    IN UINT8 DestNodeId
    );

typedef
EFI_STATUS
(EFIAPI *EFI_DISABLE_MEM_MIGRATION) (
    IN UINT8 SrcNodeId,
    IN UINT8 DestNodeId
    );

typedef
EFI_STATUS
(EFIAPI *EFI_CALL_MEM_MIGRATION_STATE_MACHINE) (
    IN UINT8 MasterNodeId,
    IN UINT8 SlaveNodeId,
    IN UINT8 CacheLineSize,    // In Bytes
    IN UINT64 BlockSize,       // In Bytes
    OUT UINT8 *MigrationOpFlag
    );

typedef
EFI_STATUS
(EFIAPI *EFI_GET_CURRENT_MEM_NODE_STATE) (
    IN UINT8      Node,
    IN OUT UINT8 *MemState
);

typedef
EFI_STATUS
(EFIAPI *EFI_MODIFY_RAS_FLAG) (
    IN UINT64                         Flag,
    IN UINT8                          Value
);

typedef
EFI_STATUS
(EFIAPI *EFI_GET_PARAMETER) (
    IN UINT64                         ParamId,
    IN UINT64                        *Value  
);

typedef
EFI_STATUS 
(EFIAPI  *EFI_INITIALIZE_DOUBLE_CHIPKILL)(
);

typedef
EFI_STATUS
(EFIAPI *EFI_SV_MEM_RAS_SERVICE) (
    IN UINT64                         ParamIn0,
    IN UINT64                         ParamIn1,
    IN UINT64                         ParamIn2,
    IN UINT64                         *ParamOut,
    OUT BOOLEAN                       *PeriodicCallbackDisFlag
);

typedef
EFI_STATUS
(EFIAPI *EFI_CKE_Low_Ctrl) (
    IN UINT8         Node,
    IN UINT8         Operation
);

typedef
EFI_STATUS
(EFIAPI *EFI_SYSTEM_ADDRESS_TO_DIMM_ADDRESS) (
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


typedef
EFI_STATUS
(EFIAPI *EFI_DIMM_ADDRESS_TO_SYSTEM_ADDRESS) (
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


typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_LOGGING_ECC_CORR_ERR) (
    IN UINT8 Node
 );

typedef
EFI_STATUS
(EFIAPI *EFI_INJECT_POISON) (
    IN UINT8   NodeId,
    IN UINT64  ErrorLogAddress              // error log Address
                                            // 63:0 - SystemAddress
 );

typedef
EFI_STATUS
( EFIAPI * EFI_MODIFY_CACHE_LINE_AND_LOG_ERR) (
   IN UINT64   Start,         // start  logical(page mapped virtual) address for migration chunk   (do you need physical address here?)
   IN UINT64   Len,           // length for the chunk, must be aligned to CachelineSize
   IN UINT64  *ErrLog,        // The mbox local address log buffer  for error logging (1024 entries available)
                              // Bits 33:0 - MBOX local address, BIT63:56 - NodeIdx
   IN UINT8    CachelineSize, // 8 bytes  for UC;  64 bytes for WB
   IN UINT8    CacheFlush,    //  Is cflush required after each cache line copy   
   IN UINT8    Node
   );

typedef
EFI_STATUS
(EFIAPI * EFI_INITIALIZE_HARD_ERROR_DETECTION)(
 );

typedef
EFI_STATUS
(EFIAPI * EFI_CHECK_AND_HANDLE_HARD_ERROR_DETECTION)(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

typedef
BOOLEAN
(EFIAPI * EFI_SUPPRESS_CMCI) (
);

typedef
EFI_STATUS
(EFIAPI * EFI_INITIALIZE_RANK_SPARING)(
 );

typedef
EFI_STATUS
(EFIAPI * EFI_CHECK_AND_HANDLE_RANK_SPARING)(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

typedef
EFI_STATUS
(EFIAPI * EFI_INITIALIZE_DDDC) (
 );

typedef
EFI_STATUS
(EFIAPI * EFI_CHECK_AND_HANDLE_DDDC)(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

typedef
EFI_STATUS
(EFIAPI * EFI_INITIALIZE_DEV_TAGGING)(
 );

typedef
EFI_STATUS
(EFIAPI * EFI_CHECK_AND_HANDLE_DEV_TAGGING)(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );

typedef
EFI_STATUS
(EFIAPI * EFI_CLEAR_UNHANDLED_CORR_ERROR)(
    IN UINT8 NodeId,
    IN EVENT_TYPE EventType,
    OUT BOOLEAN *IsEventHandled
 );



typedef
void
(EFIAPI * EFI_GET_BITMAP_OF_NODE_WITH_ERRORS) (
    IN OUT UINT32                     * ErrorBitmap
  );

typedef
void
(EFIAPI * EFI_GET_BITMAP_OF_NODE_WITH_EVENT_IN_PROGRESS) (
    IN OUT UINT32                     * EventProgressBitmap
  );

typedef
void
(EFIAPI * EFI_GET_LAST_ERR_INFO)(
    IN UINT8                          NodeId,
    IN UINT8                          *DdrCh,
    IN UINT8                          *Dimm,
    IN UINT8                          *Rank
 );

typedef
EFI_STATUS
(EFIAPI * EFI_OPEN_PAM_REGION)(
    IN UINT8 SocketId
 );

typedef
EFI_STATUS
(EFIAPI * EFI_RESTORE_PAM_REGION)(
    IN UINT8  SocketId
 );

typedef
EFI_STATUS
(EFIAPI * EFI_GET_ERROR_LOG_SYS_ADDR)(
    IN  UINT8    NodeId,
    OUT UINT64   *ErrorLogSysAddr
 );

typedef
EFI_STATUS
(EFIAPI * EFI_ENABLE_PATROL_SCRUB_FOR_POISON_DETECTION)(
    IN UINT8 NodeId
 );

typedef
EFI_STATUS
(EFIAPI * EFI_HAS_PATROL_STOPPED_ON_ERROR)(
    IN  UINT8 NodeId,
    OUT BOOLEAN *HasStoppedOnError,
    OUT UINT64  *ErrorLogSysAddr,
    OUT BOOLEAN *IsPatrolDone
 );

typedef struct {
    UINT8   IsMapped;
    UINT8   IsMirror;
    UINT8   MirrorNode;
    UINT8   Reserved;
    UINT32  Length;
    UINT32  Base0;
    UINT32  Base1;
} BOOT_MEM_MAP;

//
// Protocol Definition
//
typedef struct _EFI_MEM_RAS_PROTOCOL {
  struct SystemMemoryMapHob         *SystemMemInfo;
  BOOT_MEM_MAP                      BootMemMap[MAX_MEM_NODE];
  UINT32                            MmCfgBaseAddress;
  EFI_READ_JCK_CFG                  ReadJckCfg;
  EFI_WRITE_JCK_CFG                 WriteJckCfg;
#ifndef GRANGEVILLE_FLAG
  EFI_MEM_NODE_ONLINE               MemoryNodeOnline;
  EFI_MEM_NODE_OFFLINE              MemoryNodeOffline;
#endif  
  EFI_CPU_CFG_PCI_ADDRESS           GetCpuCfgPciAddress;
  EFI_B2B_OPERATION                 B2BOperation;
  EFI_MEM_READ_PCI_CFG              MemReadPciCfg;
  EFI_MEM_WRITE_PCI_CFG             MemWritePciCfg;
  EFI_PCI_READ_CFG                  PciReadCfg;
  EFI_PCI_WRITE_CFG                 PciWriteCfg;
  EFI_IS_MEM_NODE_ENABLED           IsMemNodeEnabled;
  EFI_UPDATE_CONFIG_IN_QUIESCE      UpdateConfigInQuiesce;
  EFI_ENABLE_ERROR_SIGNAL_TO_BIOS   EnableErrorSignallingtoBIOS;
  EFI_REENABLE_PATROL_SCRUB_ENGINE  ReEnablePatrolScrubEngine;
  EFI_DISABLE_PATROL_SCRUB_ENGINE   DisablePatrolScrubEngine;
#ifndef GRANGEVILLE_FLAG  
  EFI_MIGRATION_POSSIBLE            MigrationPossible;
  EFI_SETUP_MEM_MIGRATION           SetupMemoryMigration;
  EFI_DISABLE_MEM_MIGRATION         DisableMemoryMigration;
  EFI_CALL_MEM_MIGRATION_STATE_MACHINE CallMemoryMigrationStateMachine;
#endif  
  EFI_GET_CURRENT_MEM_NODE_STATE    GetCurrentMemoryNodeState;
  EFI_MODIFY_RAS_FLAG               ModifyRasFlag;
  EFI_GET_PARAMETER                 GetParameter;
  EFI_SV_MEM_RAS_SERVICE            SvMemRasService;
  EFI_SYSTEM_ADDRESS_TO_DIMM_ADDRESS   SystemAddressToDimmAddress;
  EFI_DIMM_ADDRESS_TO_SYSTEM_ADDRESS   DimmAddressToSystemAddress;
  EFI_ENABLE_LOGGING_ECC_CORR_ERR      EnableLoggingEccCorrErr;
  EFI_MODIFY_CACHE_LINE_AND_LOG_ERR    ModifyCacheLineAndLogErr;
  EFI_INJECT_POISON                    InjectPoison;
#ifndef GRANGEVILLE_FLAG    
  EFI_INITIALIZE_HARD_ERROR_DETECTION  InitializeHardErrorDetection;
  EFI_CHECK_AND_HANDLE_HARD_ERROR_DETECTION  CheckAndHandleHardErrorDetection;
  EFI_SUPPRESS_CMCI                    SuppressCMCI;
  EFI_INITIALIZE_RANK_SPARING          InitializeRankSparing;
  EFI_CHECK_AND_HANDLE_RANK_SPARING    CheckAndHandleRankSparing;
  EFI_INITIALIZE_DDDC                  InitializeDddc;
  EFI_CHECK_AND_HANDLE_DDDC            CheckAndHandleDddc;
#endif  
  EFI_INITIALIZE_DEV_TAGGING           InitializeDevTagging;
  EFI_CHECK_AND_HANDLE_DEV_TAGGING     CheckAndHandleDevTagging;
  EFI_CLEAR_UNHANDLED_CORR_ERROR       ClearUnhandledCorrError;
  EFI_GET_BITMAP_OF_NODE_WITH_ERRORS            GetBitMapOfNodeWithErrors;
  EFI_GET_BITMAP_OF_NODE_WITH_EVENT_IN_PROGRESS GetBitMapOfNodeWithEventInProgress;
  EFI_GET_LAST_ERR_INFO                         GetLastErrInfo;
  EFI_OPEN_PAM_REGION                  OpenPamRegion;
  EFI_RESTORE_PAM_REGION               RestorePamRegion;
  EFI_GET_ERROR_LOG_SYS_ADDR           GetErrorLogSysAddr;
  EFI_ENABLE_PATROL_SCRUB_FOR_POISON_DETECTION EnablePatrolScrubForPoisonDetection;
  EFI_HAS_PATROL_STOPPED_ON_ERROR      HasPatrolStoppedOnError;
} EFI_MEM_RAS_PROTOCOL;

extern EFI_GUID gEfiMemRasProtocolGuid;

#endif
