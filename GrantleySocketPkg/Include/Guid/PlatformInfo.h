/** @file
  PlatformInfo GUID

  These GUIDs point the ACPI tables as defined in the ACPI specifications.
  ACPI 2.0 specification defines the ACPI 2.0 GUID. UEFI 2.0 defines the 
  ACPI 2.0 Table GUID and ACPI Table GUID.

  Copyright (c) 2006 - 2009, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

  @par Revision Reference:
  GUIDs defined in UEFI 2.0 spec.

**/

#ifndef _PLATFORM_INFO_GUID_H_
#define _PLATFORM_INFO_GUID_H_

#include "Token.h" // AptioV override

extern EFI_GUID gEfiPlatformInfoGuid;
extern CHAR16   EfiPlatformInfoVariable[];

#pragma pack(1)

typedef struct {
  //
  // These are global resource mapping of the system. Per IOH resource mapping is available in UDS.
  //
  UINT16  PciResourceIoBase;
  UINT16  PciResourceIoLimit;
  UINT32  PciResourceMem32Base;
  UINT32  PciResourceMem32Limit;
  UINT64  PciResourceMem64Base;
  UINT64  PciResourceMem64Limit;
  UINT64  PciExpressBase;
  UINT32  PciExpressSize;
  BOOLEAN Pci64BitResourceAllocation;
} EFI_PLATFORM_PCI_DATA;

typedef struct {
  UINT8   CpuAddressWidth;
  UINT8   UncoreBusNum[4];
  UINT32  CpuPCPSInfo;
  UINT32  SkuSlices;
} EFI_PLATFORM_CPU_DATA;

typedef struct {
  UINT8   SysIoApicEnable;
  UINT8   SysSioExist;
} EFI_PLATFORM_SYS_DATA;

typedef struct {
  UINT32  MemTolm;
  UINT32  MemMaxTolm;
  UINT32  MemTsegSize;
  UINT32  MemIedSize;
  UINT64  MemMir0;
  UINT64  MemMir1;
#ifdef LT_FLAG
  UINT32  MemLtMemSize; //Below TSEG, contains (high to low: (Heap, SinitMem, reserved)3M, LtDevMemNonDMA 2M)
  UINT32  MemDprMemSize;//6M DPR contained in LtMem
  UINT32  MemLtMemAddress;
#endif
#if ME_SUPPORT_FLAG
  UINT64  MemMESEGBase;
  UINT64  MemMESEGSize; 
#endif //ME_SUPPORT_FLAG
} EFI_PLATFORM_MEM_DATA;

typedef struct {
  UINT8   PchPciePortCfg1;   // PCIE Port Configuration Strap 1
  UINT8   PchPciePortCfg2;   // PCIE Port Configuration Strap 2 
  UINT8   PchPcieSBDE;
  UINT8   PchLanSupported;   // Indicates if PCH LAN on board is supported or not
  UINT8   GbePciePortNum;    // Indicates the PCIe port qhen PCH LAN on board is connnected. 
  UINT8   GbeRegionInvalid;
} EFI_PLATFORM_PCH_DATA;
//
// This HOB definition must be consistent with what is created in the 
// PlatformInfo protocol definition.  This way the information in the
// HOB can be directly copied upon the protocol and only the strings
// will need to be updated.
//
typedef struct _EFI_PLATFORM_INFO_HOB {
  UINT8                       SystemUuid[16];     // 16 bytes
  UINT32                      Signature;          // "$PIT" 0x54495024
  UINT32                      Size;               // Size of the table
  UINT16                      Revision;           // Revision of the table
  UINT16                      Type;               // Platform Type
  UINT8                       CpuType;            // Cpu Type
  UINT8                       CpuStepping;        // Cpu Stepping
  UINT32                      TypeRevisionId;     // Board Revision ID
  UINT16                      IioSku;
  UINT16                      IioRevision;
  UINT16                      PchSku;
  UINT16                      PchRevision;
  UINT16                      PchType;            // Retrive PCH SKU type installed
  BOOLEAN                     ExtendedInfoValid;  // If TRUE then below fields are Valid
  UINT8                       Checksum;           // Checksum minus SystemUuid is valid in DXE only.
  UINT64                      TypeStringPtr;
  UINT64                      IioStringPtr;
  UINT64                      PchStringPtr;
  UINT8                       BoardId;  
  EFI_PLATFORM_PCI_DATA       PciData;
  EFI_PLATFORM_CPU_DATA       CpuData;
  EFI_PLATFORM_MEM_DATA       MemData;
  EFI_PLATFORM_SYS_DATA       SysData;
  EFI_PLATFORM_PCH_DATA       PchData;
  UINT8                       IioRiserId;
  UINT8                       PcieRiser1Type;
  UINT8                       PcieRiser2Type;
  UINT8                       Emulation;         // 001b = SoftSDV, 010b = Virtual Platform, 100b = Simics
  UINT32                      PfatPhysBase;
  UINT32                      PfatMemSize;
  UINT64                      Pci1394Guid;       // GUID for 1394 controller
  UINT8                       IsocEn;        /// Isoc Enabled
  UINT8                       MeSegEn;       /// Me Seg Enabled. 
  UINT8                       DmiVc1;       // 0 - Disable, 1 - Enable  
  UINT8                       DmiVcp;       // 0 - Disable, 1 - Enable  
  UINT8                       DmiVcm;       // 0 - Disable, 1 - Enable  
  UINT8                       DmiReserved;  // for alignment
#ifdef  DE_SKU
  UINT8                       Disable_SC_Cb3Dma;
  UINT8                       Disable_SC_GbE;
  UINT8                       SC_GbE_PF0;
  UINT8                       SC_GbE_PF1;
  UINT8                       DisableSCUart;
  UINT8                       SC_Gbe_Status;
#endif  //DE_SKU
#ifdef DE_SKU
  UINT8                       Disable_SC_CPM;
  UINT8                       Disable_SC_GbE1;
  UINT8                       SC_GbE1_PF0;
  UINT8                       SC_GbE1_PF1;
  UINT8                       DE_NS_SocketSkuType;
  UINT8                       SC_Gbe_Status1;
#endif
} EFI_PLATFORM_INFO;

#pragma pack()

#endif
