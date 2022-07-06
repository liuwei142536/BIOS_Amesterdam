//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++

  Copyright (c) 2012 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval 
  system, or transmitted in any form or by any means without the express 
  written consent of Intel Corporation.
 
Abstract:

  This file describes the contents of the ACPI Memory Power State Description Table
  (MPST).  Some additional ACPI values are defined in Acpi1_0.h, Acpi2_0.h, and Acpi3_0.h
  All changes to the MPST contents should be done in this file.

--*/

#ifndef _MPST_H_
#define _MPST_H_

//
// Include files
//
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/SmBios.h>
#include "Platform.h"

//
// MPST Table definition
//
#define MpstActionItemFiller  { \
  EFI_ACPI_3_0_SYSTEM_MEMORY, \
  8,                          \
  0,                          \
  EFI_ACPI_3_0_BYTE,          \
  0}

#define MpstActionItemExecute { \
  EFI_ACPI_3_0_SYSTEM_IO,     \
  8,                          \
  0,                          \
  EFI_ACPI_3_0_BYTE,          \
  0xB2}

#define EFI_ACPI_SET_MEMORY_POWER_STATE                 0x00
#define EFI_ACPI_GET_MEMORY_POWER_STATE                 0x01
#define EFI_ACPI_REQUESTED_POWER_STATE_ID               0x02
#define EFI_ACPI_CURRENT_POWER_STATE_ID                 0x03
#define EFI_ACPI_MEMORY_POWER_NODE_ID                   0x04
#define EFI_ACPI_EXECUTE_OPERATION                      0x05
#define EFI_ACPI_CHECK_BUSY_STATUS                      0x06
#define EFI_ACPI_GET_COMMAND_STATUS                     0x07

#define INSTRUCTION_READ_REGISTER                       0x00
#define INSTRUCTION_READ_REGISTER_VALUE                 0x01
#define INSTRUCTION_WRITE_REGISTER                      0x02
#define INSTRUCTION_WRITE_REGISTER_VALUE                0x03
#define INSTRUCTION_NO_OPERATION                        0x04

#define FLAG_NOTHING                                    0x00
#define FLAG_PRESERVE_REGISTER                          0x01

#define SET_MEMORY_POWER_STATE                          0x00000000
#define GET_MEMORY_POWER_STATE                          0x00000001

//
// MPST Definitions
//
#define EFI_ACPI_2_0_MEMORY_POWER_STATE_TABLE_SIGNATURE 0x5453504D
#define EFI_ACPI_OEM_MPST_REVISION                      0x00000001

#define EFI_ACPI_MPST_ID                                0x8086A201
//
// ACPI table information used to initialize tables.
//
#ifndef EFI_ACPI_OEM_ID
#define EFI_ACPI_OEM_ID           'I', 'N', 'T', 'E', 'L', ' '  // OEMID 6 bytes long
#endif
#ifndef EFI_ACPI_OEM_TABLE_ID
#define EFI_ACPI_OEM_TABLE_ID     EFI_SIGNATURE_64 ('I', 'N', 'T', 'E', 'L', 'C', 'R', 'B') // OEM table id 8 bytes long
#endif
#ifndef EFI_ACPI_OEM_REVISION
#define EFI_ACPI_OEM_REVISION     0x00000003
#endif
#ifndef EFI_ACPI_CREATOR_ID
#define EFI_ACPI_CREATOR_ID       EFI_SIGNATURE_32 ('M', 'S', 'F', 'T')
#endif
#ifndef EFI_ACPI_CREATOR_REVISION
#define EFI_ACPI_CREATOR_REVISION 0x0100000D
#endif
// AptioV Server override - to avoid redefinition in AmiProtocol.h
//#ifndef EFI_ACPI_TABLE_VERSION_ALL
//#define EFI_ACPI_TABLE_VERSION_ALL  (EFI_ACPI_TABLE_VERSION_1_0B|EFI_ACPI_TABLE_VERSION_2_0|EFI_ACPI_TABLE_VERSION_3_0)
//#endif
#define MPST_ACPI_VERSION_SUPPORT   (EFI_ACPI_TABLE_VERSION_ALL)

//
// MPST Revision (defined in spec)
//
#define EFI_ACPI_MEMORY_POWER_STATE_TABLE_REVISION    0x01
#define EFI_SET_MEMORY_POWER_STAGES_ENTRY_EXIT        0xF2  // Used in SW SMI Generation 
#define EFI_GET_MEMORY_POWER_STAGES_ENTRY_EXIT        0xF1

//
// Memory Power State Comamnd Mailbox Address Information
//
#define EFI_ACPI_COMMAND_MAILBOX_ADDRESS_SPACE_ID     EFI_ACPI_3_0_SYSTEM_IO
#define EFI_ACPI_COMMAND_MAILBOX_BIT_WIDTH            0x20
#define EFI_ACPI_COMMAND_MAILBOX_BIT_OFFSET           0x00
#define EFI_ACPI_COMMAND_MAILBOX_ACCESS_SIZE          EFI_ACPI_3_0_BYTE
#define EFI_ACPI_COMMAND_MAILBOX_ADDRESS              0x81  // Need to change???, Currently using 0x80-0x83 IoPort

#define EFI_ACPI_MPST_ATTRIBUTES                      0x00

// Identifier of the MPST Platform Communication Channel
#define EFI_ACPI_MPST_PCC_IDENTIFIER                  0x00

//
// ACPI MPST Spec defines 8 instructions to be specified as action entires 
//
#define EFI_ACPI_MPST_ACTION_ENTRY_COUNT              8

#define EFI_ACPI_NON_POWER_MANAGED_MEMORY             0x0  // Bit 0 is 0; Rest bits are reserved for future use
#define EFI_ACPI_POWER_MANAGED_MEMORY_NON_HP          0x1  // Bit 0 is 1
#define EFI_ACPI_POWER_MANAGED_MEMORY_HP              0x3  // Bit 0 is 1 Bit 1 is 1

#define FLAG_ENABLED        BIT0
#define FLAG_POWERMANAGED   BIT1
#define FLAG_HOTPLUGGABLE   BIT2

//
// M0 = Active, M1 = Self-Refresh, M2 = Standby (L0s), M3 = Offline (HP)
//
#define M0_M1_LAT   500000           // 500 micro seconds 
#define M1_M0_LAT   500000           // 500 micro seconds
 
#define M0_M2_LAT   (M0_M1_LAT + M1_M2_LAT)
#define M2_M0_LAT   (M2_M1_LAT + M1_M0_LAT)

//
// (PowerStateStructureId, Flag, Reserved, AvgPowerConsumed, PowerSaving, ExitLatencyToMps0, Rsvd
//
#define POWER_STATE_M0      { 0x41, 1, 0, 0x00000000, 0x00000000,         0, 00 }      // M0 - Active
// AptioV Server Override: Filled the Exit latency
#define POWER_STATE_M1      { 0x41, 1, 0, 0x00000000, 0x00000000,         M1_M0_LAT, 00 }      // M1 - Self-Refresh

//
// Four Power States = M0-Active, M1-SelfRefresh
//
#define EFI_ACPI_MEMORY_POWER_STATES_COUNT        2

//
// Maximum number of DIMMs supported in Channel
//
#define EFI_ACPI_PHYSICAL_COMPONENTS_COUNT        3

#define EFI_ACPI_ACTION_ENTRY_COUNT               8

#ifndef EFI_ACPI_MEMORY_POWER_NODES_COUNT
#define EFI_ACPI_MEMORY_POWER_NODES_COUNT         (MAX_MEM_NODE * MAX_CH) + 4
#endif
#pragma pack(1) // AptioV Server Override
//
//  Physical Component ID Structure definition
//
typedef struct {
  UINT8                                             PowerStateStructureID;
  UINT8                                             Flag;                   // BIT0: 0-memory contents are lost / 1-memory contents will be preserved
                                                                            // BIT1: 0-automatic hw assisted entry / 1-OS/VMM triggered entry
                                                                            // BIT2: 0-automatic hw assisted exit / 1-OS/VMM triggered exit
  UINT16                                            Reserved;
  UINT32                                            AveragePowerConsumed;   // milli watts
  UINT32                                            PowerSaving;            // MPSx/MPS0
  UINT64                                            ExitLatencyToMps0;      // MPSx --> MPS0 Latency in nano seconds
  UINT8                                             rsvd[8];                // rsvd // AptioV Server Override: Made changes as per APCI 5.0 specification.
} EFI_ACPI_MEMORY_POWER_STATE_INFO;

typedef struct {
  UINT8                                             PowerStateValue;
  UINT8                                             PowerStateInfoIndex;
} EFI_ACPI_MEMORY_POWER_STATE;

typedef struct {
  UINT8                                             PowerManagedFlag;   // BIT0: 0-Not Enabled or Present / 1-Present and Enabled
                                                                        // BIT1: 0-Not Power Managed / 1-Power Managed node
                                                                        // BIT2: 0-Not Hot-pluggable / 1-Memory power node hot pluggage 
  UINT8                                             Reserved;
  UINT16                                            MemoryPowerNodeId;
  UINT32                                            Length;
  UINT32                                            AddressBaseLow;
  UINT32                                            AddressBaseHigh;
  UINT32                                            LengthLow;
  UINT32                                            LengthHigh;
  UINT32                                            NumberOfPowerStates;
  UINT32                                            NumberOfPhysicalComponents;
  EFI_ACPI_MEMORY_POWER_STATE                       PowerState[EFI_ACPI_MEMORY_POWER_STATES_COUNT];
  UINT16                                            PhysicalComponent[MAX_DIMM];  
} EFI_ACPI_MEMORY_POWER_NODE;

typedef struct {
  UINT8                                   Operation;
  UINT8                                   Instruction;
  UINT8                                   Flags;
  UINT8                                   Reserved8;
  EFI_ACPI_3_0_GENERIC_ADDRESS_STRUCTURE  Register;
  UINT64                                  Value;
  UINT64                                  Mask;
} EFI_ACPI_MPST_ACTION_ITEM;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER               Header;
  UINT8                                     MpstPccIdentifier;
  UINT8                                     Rsvd[3];
} EFI_ACPI_MEMORY_POWER_STATE_TABLE_HEADER;

typedef struct {
  EFI_ACPI_MEMORY_POWER_STATE_TABLE_HEADER   Header;
  UINT16                                     MemPwrNodeCnt;
  UINT16                                     Rsvd1;
  EFI_ACPI_MEMORY_POWER_NODE                 MemoryPowerNode[EFI_ACPI_MEMORY_POWER_NODES_COUNT];
  UINT16                                     PwrStateInfoCnt;
  UINT16                                     Rsvd2;
  EFI_ACPI_MEMORY_POWER_STATE_INFO           PowerStateInfo[EFI_ACPI_MEMORY_POWER_NODES_COUNT][EFI_ACPI_MEMORY_POWER_STATES_COUNT];  
} EFI_ACPI_MEMORY_POWER_STATE_TABLE;

#pragma pack() // AptioV Server Override
#endif

