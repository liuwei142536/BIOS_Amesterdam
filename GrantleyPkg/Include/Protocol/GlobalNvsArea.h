//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license
// agreement with Intel or your vendor.  This file may be
// modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c) 2004 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  GlobalNvsArea.h

Abstract:

  Definition of the global NVS area protocol.  This protocol 
  publishes the address and format of a global ACPI NVS buffer used as a communications
  buffer between SMM code and ASL code.

  Note:  Data structures defined in this protocol are not naturally aligned.

--*/

#ifndef _EFI_GLOBAL_NVS_AREA_H_
#define _EFI_GLOBAL_NVS_AREA_H_

//
// Includes
//


//
// Forward reference for pure ANSI compatability
//

typedef struct EFI_GLOBAL_NVS_AREA_PROTOCOL;

//
// Global NVS Area Protocol GUID
//
#define EFI_GLOBAL_NVS_AREA_PROTOCOL_GUID \
{ 0x74e1e48, 0x8132, 0x47a1, 0x8c, 0x2c, 0x3f, 0x14, 0xad, 0x9a, 0x66, 0xdc }

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiGlobalNvsAreaProtocolGuid;

//
// Global NVS Area definition
//
#pragma pack (1)

typedef struct {
  UINT32 PlatformId;
  UINT8  IoApicEnable;
  UINT8  RES0           :8;          // Unused
  UINT8  TpmEnable      :1;
  UINT8  CStateEnable   :1;
  UINT8  C3Enable       :1;
  UINT8  C6Enable       :1;
  UINT8  C7Enable       :1;
  UINT8  MonitorMwaitEnable :1;
  UINT8  PStateEnable   :1;
  UINT8  EMcaLogEn      :1;
  UINT8  HWAllEnable    :1;
  UINT8  KBPresent      :1;
  UINT8  MousePresent   :1;
  UINT8  TStateEnable   :1;
  UINT8  TStateFineGrained: 1;
  UINT8  OSCX           :1;
  UINT8  RESX           :2;          // Unused
  UINT8  CpuChangeMask  :8;
  UINT8  IioChangeMask  :8;
  UINT8  IioPresentBitMask:8;
  //UINT8  PadReserved    :8;
  UINT8  CpuType; 
  UINT32 SocketBitMask;
  UINT32 ProcessorApicIdBase[4];
  UINT64 ProcessorBitMask[4];
  UINT16 MemoryBoardBitMask;
  UINT16 MemoryBoardChgEvent;
  UINT32 MmCfg;
  UINT32 TsegSize;
  UINT64 MemoryBoardBase[8];
  UINT64 MemoryBoardRange[8];
  UINT32 SmiRequestParam[4];
  UINT32 SciRequestParam[4];
  UINT64 MigrationActionRegionAddress;
  UINT8  Cpu0Uuid[16];
  UINT8  Cpu1Uuid[16]; 
  UINT8  Cpu2Uuid[16]; 
  UINT8  Cpu3Uuid[16]; 
  UINT8  Cpu4Uuid[16];
  UINT8  Cpu5Uuid[16]; 
  UINT8  Cpu6Uuid[16]; 
  UINT8  Cpu7Uuid[16]; 
  UINT8  CpuSpareMask;   
  UINT8  Mem0Uuid[16];  
  UINT8  Mem1Uuid[16]; 
  UINT8  Mem2Uuid[16]; 
  UINT8  Mem3Uuid[16]; 
  UINT8  Mem4Uuid[16];  
  UINT8  Mem5Uuid[16]; 
  UINT8  Mem6Uuid[16]; 
  UINT8  Mem7Uuid[16]; 
  UINT8  Mem8Uuid[16];  
  UINT8  Mem9Uuid[16]; 
  UINT8  Mem10Uuid[16]; 
  UINT8  Mem11Uuid[16]; 
  UINT8  Mem12Uuid[16];  
  UINT8  Mem13Uuid[16]; 
  UINT8  Mem14Uuid[16]; 
  UINT8  Mem15Uuid[16]; 
  UINT16 MemSpareMask;
  UINT64 DrhdAddr[3];   
  UINT64 AtsrAddr[3];   
  UINT64 RhsaAddr[3];
  UINT64 EmcaL1DirAddr;// L1 Directory Address
  UINT32 ProcessorId;

  // SR-IOV WA changes
  UINT8  WmaaSICaseValue;  
  UINT16 WmaaSISeg;        
  UINT8  WmaaSIBus;        
  UINT8  WmaaSIDevice;     
  UINT8  WmaaSIFunction;   
  UINT8  WmaaSISts; 
  UINT8  WheaSupport;
  
  //
  // PFAT
  //
  UINT64  PfatMemAddress;
  UINT8   PfatMemSize;
  UINT16  PfatIoTrapAddress;

  UINT8   CpuSkuNumOfBitShift;

  UINT8   XhciMode;
  UINT8   HostAlertVector1;
  UINT8   HostAlertVector2;

  UINT8  AcpiS3Enable;
  UINT8  AcpiS4Enable;

  UINT8   PcieAcpiHotPlugEnable;
  
  UINT8   ClusterOnDieEnable;
  
  //HWPM-start
  UINT8   HWPMEnable:2; //HWPM
  UINT8   AutoCstate:1; //HWPM
  UINT8   Reserved:5; //HWPM
  //HWPM-end
#ifndef	DE_SKU  
  UINT8   NitrousNotSupport; // Nitrous; 0: Support, 1: Not support 
#endif
#ifdef	DE_SKU  
  UINT8   SOLE;
  UINT8   STEP;
#endif
} BIOS_ACPI_PARAM;

#pragma pack ()

//
// Global NVS Area Protocol
//
typedef struct _EFI_GLOBAL_NVS_AREA_PROTOCOL {
  BIOS_ACPI_PARAM     *Area;
} EFI_GLOBAL_NVS_AREA_PROTOCOL;

#endif
