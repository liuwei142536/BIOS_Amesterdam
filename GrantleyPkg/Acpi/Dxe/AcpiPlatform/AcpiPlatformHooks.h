//
// This file contains 'Framework Code' and is licensed as such
// under the terms of your license agreement with Intel or your
// vendor.  This file may not be modified, except as allowed by
// additional terms of your license agreement.
//
/*++
Copyright (c) 1996 - 2016, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  AcpiPlatformHooks.h
  
Abstract:

--*/

#ifndef _ACPI_PLATFORM_HOOKS_H_
#define _ACPI_PLATFORM_HOOKS_H_

//
// Statements that include other header files
//
#include <PiDxe.h>
#include <Protocol/CpuIo.h>
#include <Library/CpuConfigLib.h>
#include <Library/PchPlatformLib.h>
#define AML_NAME_OP           0x08
#define AML_METHOD_OP         0x14
#define AML_OPREGION_OP       0x80
#define AML_PACKAGE_OP        0x12  // Package operator.
//#define AML_NOOP_OP           0xA3
#define AML_NAME_PREFIX_SIZE  0x06
#define AML_NAME_DWORD_SIZE   0x0C

#ifndef R_APM_CNT
#define R_APM_CNT 0xb2
#endif

// PlatformInfo->CpuData.CpuPCPSInfo DEFINITIONS
#define B_PCPS_SPD_ENABLE                  (1 << 16) // Bit 16
#define B_PCPS_HT_ENABLE                   (1 << 17) // Bit 17


// OC 
#define OC_LIB_CMD_GET_OC_CAPABILITIES      0x01
#define OC_MAILBOX_MSR						0x150
#define MAILBOX_WAIT_TIMEOUT  10000  

#ifndef  GRANGEVILLE_FLAG
#define OC_LIB_CMD_GET_CORE_CONTROL      0x1C
#define MAILBOX_TYPE_OC                   0x00000002
#define MAILBOX_READ_TIMEOUT              10          ///< 10 microseconds
#define MAILBOX_WAIT_STALL                1           ///< 1 microsecond
#endif

#ifdef BDAT_SUPPORT
#define BDAT_PRIMARY_VER    0x0004
#define BDAT_SECONDARY_VER  0x0000
#define SCHEMA_LIST_LENGTH  3
#endif

#define NITRO_OEM_TABLE_ID SIGNATURE_64('N','I','T','R','O','M','F','C')


#pragma pack(1)

typedef struct {
  UINT32   AcpiProcessorId;
  UINT32   ApicId;
  UINT32   Flags;
  UINT32   SwProcApicId;
  UINT32   SocketNum;
} EFI_CPU_ID_ORDER_MAP;

typedef struct {
  UINT32             Index;
  UINT32             SortId;
} EFI_CPU_LOCATION_LIST;

typedef struct {
  UINT8   AcpiProcessorId;
  UINT8   ApicId;
  UINT16  Flags;
} EFI_CPU_ID_MAP;

typedef struct {
  UINT8   StartByte;
  UINT32  NameStr;
  UINT8   Size;
  UINT32  Value;      
} EFI_ACPI_NAMEPACK_DWORD;

typedef struct {
  UINT8   StartByte;
  UINT32  NameStr;
  UINT8   OpCode;
  UINT16  Size;                     // Hardcode to 16bit width because the table we use is fixed size
  UINT8   NumEntries;
} EFI_ACPI_NAME_COMMAND;

typedef struct {
  UINT8   PackageOp;
  UINT8   PkgLeadByte;
  UINT8   NumEntries;
  UINT8   DwordPrefix0;
  UINT32  CoreFreq;
  UINT8   DwordPrefix1;
  UINT32  Power;
  UINT8   DwordPrefix2;
  UINT32  TransLatency;
  UINT8   DwordPrefix3;
  UINT32  BMLatency;
  UINT8   DwordPrefix4;
  UINT32  Control;
  UINT8   DwordPrefix5;
  UINT32  Status;
} EFI_PSS_PACKAGE;

typedef struct {
  UINT8 PackageOp;
  UINT8 PkgLeadByte;
  UINT8 NumEntries;
  UINT8 BytePrefix0;
  UINT8 Entries;
  UINT8 BytePrefix1;
  UINT8 Revision;
  UINT8 BytePrefix2;
  UINT8 Domain;
  UINT8 BytePrefix3;
  UINT8 Coordinate;
  UINT8 BytePrefix4;
  UINT8 ProcNumber;
} EFI_PSD_PACKAGE;

typedef struct {
  UINT8     NameOp;           // 08h ;First opcode is a NameOp.
  UINT32    PackageName;      // PSDC/PSDD/PSDE 
  UINT8     Length;           // 12h
  UINT8     DwordPrefix1;     // 0Fh
  UINT8     Revision;         // 01h
  UINT8     PackageOp;        // 12h
  UINT8     PackageLen;       // 0Ch
  UINT8     PackLen;          // 05h
  UINT16    WordValue1;       // 0A05h
  UINT16    WordValue2;       // 0A00h
  UINT8     BytePrefix2;      // 00h
  UINT8     Domain;           // 08h
  UINT8     BytePrefix3;      // 0Ah
  UINT8     CoordType;        // 0FCh/0FDh/0FEh
  UINT8     BytePrefix4;      // 0Ah
  UINT8     NumProcessors;    // 10h
} PSD_PACKAGE_LAYOUT;

typedef union _OC_MAILBOX_COMMAND {
  UINT32 InterfaceData;
  struct {
    UINT8 CommandCompletion : 8;
    UINT8 Param1            : 8;
    UINT8 Param2            : 8;
    UINT8 Reserved          : 7;
    UINT8 RunBusy           : 1;
  } Fields;
} OC_MAILBOX_COMMAND;

typedef struct _OC_MAILBOX_ITEM {
  UINT32             Data;
  OC_MAILBOX_COMMAND Interface;
} OC_MAILBOX_ITEM;

typedef union _OC_MAILBOX_INTERFACE {
  UINT32 InterfaceData;
  struct {
    UINT8 CommandCompletion : 8;
    UINT8 Param1            : 8;
    UINT8 Param2            : 8;
    UINT8 Reserved          : 7;
    UINT8 RunBusy           : 1;
  } Fields;
} OC_MAILBOX_INTERFACE;

typedef struct _OC_MAILBOX_FULL {
  UINT32               Data;
  OC_MAILBOX_INTERFACE Interface;
} OC_MAILBOX_FULL;

#pragma pack()

#define ACPI_NAME_COMMAND_FROM_NAME_STR(a)  BASE_CR (a, EFI_ACPI_NAME_COMMAND, NameStr)
#define ACPI_NAME_COMMAND_FROM_NAMEPACK_STR(a)  BASE_CR (a, EFI_ACPI_NAMEPACK_DWORD, NameStr)

EFI_STATUS
PlatformHookInit (
  VOID
  );

EFI_STATUS
PatchDsdtTable (
  IN OUT   EFI_ACPI_DESCRIPTION_HEADER  *TableHeader
  );

EFI_STATUS
PatchGv3SsdtTable (
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER *Table
  );

EFI_STATUS
PatchErstTable (
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER *Table
  );


EFI_STATUS
AppendCpuMapTableEntry (
  IN VOID    *AcpiLocalApic,
  //IN  UINTN  NumberOfCPUs,
  //IN  UINTN  MaximumNumberOfCPUs,
  IN  UINT32  LocalApicCounter
  );

EFI_STATUS
PatchMemoryPowerStateTable (
  VOID *MpstTablev
  );

EFI_STATUS
PatchPlatformMemoryTopologyTable (
  VOID *PmttTablev
  );

EFI_STATUS
PatchPlatformCommunicationsChannelTable (
  VOID *PcttTablev
  );

EFI_STATUS
PatchSRATableAllApicIds (
  IN OUT   EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE  *SRAAcpiTable
  );

EFI_STATUS
PatchMsctAcpiTable (
  IN OUT   EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE *MsctAcpiTable
  );
  
  

EFI_STATUS
ReadOcCap (OUT UINT8*
);

EFI_STATUS
CheckMailboxReady (
VOID
);


// AptioV server override start: AMI ACPI driver updates sleep states object based on setup controls from ACPI configuration.
/*UINT32
FixupSystemSleepStates (
  IN UINT32 SleepStateForFixup
  );*/
VOID
FixupSystemSleepStates (
 VOID
 );
// AptioV server override end: AMI ACPI driver updates sleep states object based on setup controls from ACPI configuration.

#ifdef BDAT_SUPPORT
#pragma pack(1)
typedef struct {
  UINT8   BiosDataSignature[8]; // "BDATHEAD"
  UINT32  BiosDataStructSize;   // sizeof BDAT_STRUCTURE + sizeof BDAT_MEMORY_DATA_STRUCTURE + sizeof BDAT_RMT_STRUCTURE
  UINT16  Crc16;                // 16-bit CRC of BDAT_STRUCTURE (calculated with 0 in this field)
  UINT16  Reserved;
  UINT16  PrimaryVersion;       // Primary version
  UINT16  SecondaryVersion;     // Secondary version
  UINT32  OemOffset;            // Optional offset to OEM-defined structure
  UINT32  Reserved1;
  UINT32  Reserved2;
} BDAT_HEADER_STRUCTURE;

typedef struct bdatSchemaList {
  UINT16                       SchemaListLength; //Number of Schemas present
  UINT16                       Reserved;
  UINT16                       Year;
  UINT8                        Month;
  UINT8                        Day;
  UINT8                        Hour;
  UINT8                        Minute;
  UINT8                        Second;
  UINT8                        Reserved1;
  UINT32                       Schemas[SCHEMA_LIST_LENGTH];  //List of Bdat Memory Schemas
} BDAT_SCHEMA_LIST_STRUCTURE;

//BDAT Header Struct which contains information all exisitng BDAT Schemas
typedef struct bdatStruct {
  BDAT_HEADER_STRUCTURE        BdatHeader;
  BDAT_SCHEMA_LIST_STRUCTURE   BdatSchemas;
} BDAT_STRUCTURE;
#pragma pack()

EFI_STATUS
PatchBdatAcpiTable (
  IN OUT   EFI_BDAT_ACPI_DESCRIPTION_TABLE *BdatAcpiTable
  );
#endif

EFI_STATUS
SortCpuLocalApicInTable (
  IN  EFI_MP_SERVICES_PROTOCOL                  *MpService,
  IN  UINTN                                      NumberOfCPUs
  );

VOID
DisableAriForwarding (
  VOID
  );

#ifndef  GRANGEVILLE_FLAG
VOID
ConvertToMailboxFormat (
  OUT OC_MAILBOX_ITEM *MailboxData,
  IN UINT32           CommandId
  );

EFI_STATUS
EFIAPI
MailboxRead (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  );

EFI_STATUS
EFIAPI
PollMailboxReady (
  IN UINT32 MailboxType
  );

EFI_STATUS
EFIAPI
GetCoreControlCapability (VOID);
#endif

#endif
