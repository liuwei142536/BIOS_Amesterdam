//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c) 2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MpstSupport.h
    
Abstract:

  MPST Support protocol

--*/

#ifndef _MPST_SUPPORT_PROTOCOL_H_
#define _MPST_SUPPORT_PROTOCOL_H_

extern EFI_GUID gEfiMpstSupportProtocolGuid;
extern CHAR16   EfiMpstSuppportVariable[];

#define MPST_COMMAND_SET  0
#define MPST_COMMAND_GET  1
#define MPST_COMMAND_GET_AVG_PWR_CONSUMED  03  
#define MPST_COMMAND_GET_MEM_ENERGY_CONSUMED  04 

#define MPST_POWER_STATE_ACTIVE   0
#define MPST_POWER_STATE_SR       1

#define MPST_RESPONSE_COMPLETED   0
#define MPST_RESPONSE_VALID       1

#define MPST_STATUS_SUCCESS       0
#define MPST_STATUS_INVALID       1
#define MPST_STATUS_UNSUPPORTED   2
#define MPST_STATUS_BUSY          3
#define MPST_STATUS_FAILED        4
#define MPST_STATUS_ABORTED       5
#define MPST_STATUS_INVALID_DATA  6
#define MPST_STATUS_PSINPROOG_SUCCESS  0x10

#define EFI_ACPI_MEMORY_POWER_NODES_COUNT         (MAX_MEM_NODE * MAX_CH) + 4

typedef EFI_STATUS (EFIAPI *MPST_SMM_PWR_STATE_FN) (VOID );

#define PCCT_SWSMI_VALUE                        0xF3
#define PCC_MEM_POWER_STATE_STS_SUCCESS         00
#define PCC_MEM_POWER_STATE_STS_NOT_VALID       01
#define PCC_MEM_POWER_STATE_STS_NOT_SUPPORTED   02
#define PCC_MEM_POWER_STATE_STS_BUSY            03
#define PCC_MEM_POWER_STATE_STS_FAILED          04
#define PCC_MEM_POWER_STATE_STS_ABORTED         05
#define PCC_MEM_POWER_STATE_STS_INVALID_DATA    06

#define PCC_SBSTANCE_MPST_ENTRY                 00  
#define PCC_SBSTANCE_MPST_GET_PWR_STATE         01  
#define PCC_SBSTANCE_MPST_SET_PWR_STATE         02  
#define PCC_SBSTANCE_MPST_GET_AVG_PWR_CONSUMED  03  
#define PCC_SBSTANCE_MPST_GET_MEM_ENERGY_CONSUMED  04  
#define PCC_GLOBAL_FLAGS_SCI_DOORBELL           BIT0
#pragma pack (1)

typedef struct {
  UINT8   BusyStatus:1;       // 0-completed/1-valid & busy
  UINT8   Rsvd:2;             // Reserved
  UINT8   Status:5;           // 0-Success,b-Invalid,2-Not Supported,3-Busy,4-Failed,5-Aborted,6-Invalid Data
} EFI_ACPI_MPST_RESPONSE;

typedef struct {
  UINT8                     Command;
  EFI_ACPI_MPST_RESPONSE  Response;
  EFI_ACPI_MPST_RESPONSE  BusySts;
  UINT8                     PowerState;
  UINT16                    MemoryPowerNodeId; 
  UINT8                     Data[11];
} EFI_ACPI_MPST_MAILBOX;

typedef struct {
  UINT16	NodeChNum;
  UINT16	PwrNodeId;
  UINT64	EnergyValue;
  UINT64	AvgPwr;
} MPST_NODE_ENERGY_CONSUMED;

typedef struct {
  UINT8		PowerManagedFlag;
  UINT8		Node;
  UINT8		Tad;
  UINT8		Ch;
  UINT8		DimmMap;
  UINT16	MemoryPowerNode;
} MPST_PWR_NODE_MEM_CFG_STRUC;

typedef union {
   struct{
	    UINT16 CmdComplete:1;
	    UINT16 SciDoorBell:1;
	    UINT16 Error:1;
	    UINT16 PlatformNotification:1;
	    UINT16 Rsvd:12;
   } Bits;
   UINT16 Data;
} ADDR_PCC_STS_FIELD;

typedef union {
   struct{
	    UINT16 Cmd:8;
	    UINT16 Rsvd:7;
	    UINT16 GenerateSci:1;
   } Bits;
   UINT16 Data;
} ADDR_PCC_CMD_FIELD;


/*
Signature	4	0	The PCC signature 0x24504343
Command	2	4	PCC command field, described below.
Status	2	4	PCC status field, described below.
Communication Space	-	8	Memory region for reading/writing PCC data. Size is PCC structure Length field - 8.
*/

typedef struct {
  UINT32                                  Signature;
  ADDR_PCC_CMD_FIELD                      Command;
  ADDR_PCC_STS_FIELD                      Status;
  // Communication space
  UINT32                                  MemPowerStateCmd;
  UINT32                                  MemPowerStateSts;
  UINT32                                  PowerStateId;       
  UINT32                                  MemPowerNodeId;     
  UINT64                                  MemEnergyConsumed;     
  UINT64                                  ExpectedAvgPwrConsumed;     
} EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST;

typedef struct {
  EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST   *SharedMemRegion;
} EFI_PCCT_SUPPORT_PROTOCOL;

typedef struct _EFI_MPST_SUPPORT_PROTOCOL {
  EFI_ACPI_MPST_MAILBOX        *MpstMailbox;
  UINT16                       MaxPwrNode;
  UINT8                        MemoryPowerNodeStatus[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
  MPST_PWR_NODE_MEM_CFG_STRUC  MemPwrNodeInfo[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
  MPST_NODE_ENERGY_CONSUMED    NodeEnergyConsumed;
  MPST_SMM_PWR_STATE_FN        MpstSmmPwrStateFn; 
  UINT64		                   ChPwrConsumed[MAX_MEM_NODE][MAX_CH];
  EFI_ACPI_PCC_SHARED_MEMORY_REGION_MPST   *SharedMemRegion;
  VOID                         *MpstTable;
} EFI_MPST_SUPPORT_PROTOCOL;

#pragma pack ()

#endif
