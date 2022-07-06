/*++
  This file contains 'Framework Code' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may not be modified, except as allowed by
  additional terms of your license agreement.
--*/
/*++

Copyright (c)  2012 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  AcpiPlatformVTDHooks.c

Abstract:

  ACPI Platform Driver VT-D Hooks

--*/
// AptioV Server Override: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
#include <Token.h>
#include "AcpiPlatform.h"
#include "AcpiPlatformHooks.h"
#include "AcpiPlatformHooksLib.h"
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/AcpiTable.h>
// AptioV Server Override Start: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
#if USB_DRIVER_MAJOR_VER >= 10
#undef EFI_ACPI_TABLE_VERSION_ALL //undefined here as it is defined from AmiDxeLib.h which is included in AmiUsbController.h
#endif
#include <Protocol\AmiUsbController.h>
#endif
// AptioV Server Override End: For filling Reserved Base Address and Limit for RMRR structure of DMAR table

// AptioV Grangeville Override Start: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER
#ifndef _PCI22_H
#define _PCI22_H
#define PCI_MAX_BAR_NO                  6
#include <PciBus.h>
#endif
// AptioV Grangeville Override End: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER


// AptioV Server Override Start: OEM VTD RMRR support
#if defined OemVtdRmrr_SUPPORT && OemVtdRmrr_SUPPORT == 1
#include <Protocol/OemVtdRmrrProtocol.h>
#endif
// AptioV Server Override End: OEM VTD RMRR support

extern EFI_PLATFORM_INFO         *mPlatformInfo;
extern BIOS_ACPI_PARAM           *mAcpiParameter;
extern struct SystemMemoryMapHob *mSystemMemoryMap;
extern EFI_IIO_UDS_PROTOCOL   *mIioUds;
extern SYSTEM_CONFIGURATION  mSystemConfiguration;
extern UINT32 mVariableAttr; // AptioV Server Override: Remove NVRAM RT Attribute 
BOOLEAN VtdIntRemapEnFalg = FALSE;
extern EFI_GUID mSystemConfigurationGuid;

//Aptio V Server Override - Add define for X2APIC_OPT_OUT.
#define EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT BIT1

#define EFI_PCI_CAPABILITY_PTR              0x34
#define EFI_PCIE_CAPABILITY_BASE_OFFSET     0x100
#define EFI_PCIE_CAPABILITY_ID_ACS          0x000D
#define EFI_PCI_CAPABILITY_ID_PCIEXP        0x10
#define EFI_PCI_EXPRESS_CAPABILITY_REGISTER 0x02

#define ACS_CAPABILITY_REGISTER      0x04
#define ACS_SOURCE_VALIDATION        BIT0
#define ACS_P2P_REQUEST_REDIRECT     BIT2
#define ACS_P2P_COMPLETION_REDIRECT  BIT3
#define ACS_UPSTREAM_FORWARDING      BIT4

#define ACS_CONTROL_REGISTER                0x06
#define ACS_SOURCE_VALIDATION_ENABLE        BIT0
#define ACS_P2P_REQUEST_REDIRECT_ENABLE     BIT2
#define ACS_P2P_COMPLETION_REDIRECT_ENABLE  BIT3
#define ACS_UPSTREAM_FORWARDING_ENABLE      BIT4

#define R_VTD_GCMD_REG                            0x18
#define R_VTD_GSTS_REG                            0x1C
#define R_VTD_IQT_REG                             0x88
#define R_VTD_IQA_REG                             0x90
#define R_VTD_IRTA_REG                            0xB8

#define VTD_ISOCH_ENGINE_OFFSET             0x1000

typedef struct {
  UINTN BusNumber;
  UINTN DevNumber;
  UINTN FunNumber;
  UINT32 AcsOffset;
}ACS_DEV;

//
// a flag to indicate if we should disable Vt-d for ACS WA
//
BOOLEAN DisableVtd = FALSE;

#pragma optimize("", off)

EFI_DMA_REMAP_PROTOCOL    DmaRemapProt;
#define  VTD_SUPPORT_INSTANCE_FROM_THIS(a) CR(a, EFI_VTD_SUPPORT_INSTANCE, DmaRemapProt, EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE)
// #define  DMAR_HOST_ADDRESS_WIDTH   46
#define  EFI_PCI_CAPABILITY_ID_ATS  0x0F

#define SEGMENT0           0x00
#define MEM_BLK_COUNT      0x140
#define INTRREMAP          BIT3

PCI_NODE  mPciPath0_1[]   = {
  {PCI_DEVICE_NUMBER_PCH_AZALIA, PCI_FUNCTION_NUMBER_PCH_AZALIA},
  {(UINT8) -1,  (UINT8) -1},
};

//
// IOAPIC1  - PCH IoApic
//


//
// IOAPIC2  - IIO IoApic
//
PCI_NODE  mPciPath1_2[]   = {
  {APIC_DEV_NUM, APIC_FUNC_NUM},
  {(UINT8) -1,  (UINT8) -1},
};

PCI_NODE  mPciPath2_0[]   = {
  {PCIE_PORT_0_DEV, PCIE_PORT_0_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_1[]   = {
  {PCIE_PORT_1_DEV, PCIE_PORT_1A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_2[]   = {
  {PCIE_PORT_1_DEV, PCIE_PORT_1B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_3[]   = {
  {PCIE_PORT_2_DEV, PCIE_PORT_2A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_4[]   = {
  {PCIE_PORT_2_DEV, PCIE_PORT_2B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_5[]   = {
  {PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_6[]   = {
  {PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_7[]   = {
  {PCIE_PORT_3_DEV, PCIE_PORT_3A_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_8[]   = {
  {PCIE_PORT_3_DEV, PCIE_PORT_3B_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_9[]   = {
  {PCIE_PORT_3_DEV, PCIE_PORT_3C_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};
PCI_NODE  mPciPath2_10[]  = {
  {PCIE_PORT_3_DEV, PCIE_PORT_3D_FUNC},
  {(UINT8) -1,  (UINT8) -1},
};

DEVICE_SCOPE              mDevScopeDRHD[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT,    // Device type - HD Audio
    00,                                           // Enumeration ID
    DEFAULT_PCI_BUS_NUMBER_PCH,                           // Start Bus Number
    &mPciPath0_1[0]
  },
};

DEVICE_SCOPE              mDevScopeDRHD_INT[] = {
  {
    00,                                           // Device type
    00,                                           // Enumeration ID - ApicID
    00,                                           // Start Bus Number
    &mPciPath1_2[0]
  },
};

DEVICE_SCOPE              mDevScopeATSR[] = {
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_0[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port1
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_1[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port2
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_2[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port3
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_3[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port4
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_4[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port5
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_5[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port6
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_6[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port7
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_7[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port8
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_8[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port9
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_9[0]
  },
  {
    EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE,      // Pcie Port10
    00,                                           // Enumeration ID
    IIO_BUS_NUM,
    &mPciPath2_10[0]
  },
};

DMAR_DRHD                 mDrhd = {
  DRHD_SIGNATURE,
  00,                                             // Flags
  SEGMENT0,                                       // Segment number
  00,                                             // Base Address
  00,                                             // Number of dev scope structures
  &mDevScopeDRHD[0]
};

DMAR_DRHD                 mDrhdIsoc = {
  DRHD_SIGNATURE,
  00,                                             // Flags
  SEGMENT0,                                       // Segment number
  00,                                             // Base Address
  00,                                             // Number of dev scope structures
  &mDevScopeDRHD[0]
};

DMAR_ATSR                 mAtsr = {
  ATSR_SIGNATURE,
  SEGMENT0,                                       // Segment number
  (UINT8) EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR,
  PCIE_PORT_MAX,
  00,
  &mDevScopeATSR[0]
};
/* // AptioV Server Override Start: Adding Dynamic Device Scope Entries for RMRR structure
PCI_NODE     PciPath[] = {
  { 00,      00},
  { (UINT8)-1,   (UINT8)-1},
};


PCI_NODE     PciPath7[] = {
  { PCI_DEVICE_NUMBER_PCH_USB,      PCI_FUNCTION_NUMBER_PCH_EHCI  },
  { (UINT8)-1,   (UINT8)-1},
};
PCI_NODE     PciPath8[] = {
  { PCI_DEVICE_NUMBER_PCH_USB_EXT,  PCI_FUNCTION_NUMBER_PCH_EHCI2 },
  { (UINT8)-1,   (UINT8)-1},
};

DEVICE_SCOPE DevScopeRmrr[] = {
  {
    1,                                  // RMRR dev Scope - EHCI1
    0,                                  // Enumeration ID
    0,                                  // Start Bus Number
    &PciPath7[0]
  },
  {
    1,                                  // RMRR dev Scope - EHCI2
    0,                                  // Enumeration ID
    0,                                  // Start Bus Number
    &PciPath8[0]
  },
};

DMAR_RMRR    mRmrr = {
  RMRR_SIGNATURE,                       // Signature
  SEGMENT0,                             // Segment number
  ' ',                                  // Reserved Memory RegionBase Address
  ' ',                                  // Reserved Memory RegionLimit Address
  ' ',                                  // Number of Dev Scope structures
  &DevScopeRmrr[0] 
};
*/
DMAR_RMRR               mRmrr;
// AptioV Server Override End: Adding Dynamic Device Scope Entries for RMRR structure
typedef struct {
    UINT8   aBuf[32];
} MEM_BLK;

DMAR_RHSA                 mRhsa;


EFI_STATUS
LocateCapRegBlock(
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                CapID,
  OUT    UINT8                *PciExpressOffset,
  OUT    UINT8                *NextRegBlock
  );

EFI_STATUS
LocatePciExpressCapRegBlock (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT16               CapID,
  OUT    UINT32               *Offset,
  OUT    UINT32               *NextRegBlock
);

DMAR_DRHD                 mDrhd;

DMAR_ATSR                 mAtsr;
DMAR_RHSA                 mRhsa;

VOID
UpdateVtdIntRemapFlag (
  )
{
  VtdIntRemapEnFalg = FALSE;
if( (mSystemConfiguration.ProcessorXapic)  &&
    (mSystemConfiguration.ProcessorVirtualWireMode == 01)  &&   // procesoor should boot in virtual wire B mode.
    (mIioUds->IioUdsPtr->SetupInfo.Config.VTdSupport) &&
    (mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap) )
    {
      VtdIntRemapEnFalg = TRUE;
    }
}

VOID
EnableVtdIntRemapping(
  )
{
  EFI_STATUS                  Status;
  UINT64                      *xApicAddr;
  UINT64                      *IRTA;
  UINT64                      *Addr;
  UINT64                      Value=0;
  UINT16                      IRTECount;
  UINT16                      Count;
  UINT64                      IRTEValue;
  volatile UINT64             TempDWord[MAX_SOCKET];
  UINT8                       RemapEng;
  UINT8                       RemapEngCount;
  EFI_CPUID_REGISTER          CpuidRegisters;
  UINT32                      VtdBarAddress;

  IRTEValue   = 00;
  RemapEng = 0;
  RemapEngCount = mIioUds->IioUdsPtr->PlatformData.numofIIO;
  //
  // Xapic tables update
  //
  IRTECount = 16 * 24;    // Total 24 IRTE entries with 128 bits each.
  // Allocate 4K alligned space for IRTE entries  Added extra space of 500 bytes.
  Status = gBS->AllocatePool (EfiACPIReclaimMemory, IRTECount + 0x1500, &xApicAddr);
  ASSERT_EFI_ERROR (Status);

  // Allocate IRT - Allocate zero-initialized, 4KB aligned, 4KB memory for interrupt-remap-table and mark this memory as "ACPI Reclaim Memory"
  xApicAddr = (UINT64 *)((UINT64)xApicAddr & (~0xFFF));
  ZeroMem (xApicAddr, IRTECount +0x1500);

  // 1. Program IRTE - Initialize the interrupt-remap-table as follows: (this table will be shared by all VT-d units)
  for(Count = 0; Count < 24; Count++)  {

    IRTEValue = 00;
    if (Count == 0) {
      IRTEValue = (7 << 05) + 03;    // Preset flag set, Ext int enabled, FPD set
    }

    AsmCpuid (
      EFI_CPUID_CORE_TOPOLOGY,
      &CpuidRegisters.RegEax,
      &CpuidRegisters.RegEbx,
      &CpuidRegisters.RegEcx,
      &CpuidRegisters.RegEdx
      );
    IRTEValue |= (UINT64)(((UINT64)CpuidRegisters.RegEdx) << 32);    // Destination Processor Apic ID

//            if(mSystemConfiguration.ForcePhysicalModeEnable == 00) {
//              IRTEValue |= 0x4;
//            }

    *(volatile UINT64 *)((UINT64)xApicAddr + (Count * 16))= IRTEValue;

    //Perform a CLFLUSH instruction for each cachline in this 4KB memory to ensure that updates to the interrupt-remap-table are visible in memory
    AsmFlushCacheLine ((VOID *)((UINT64)xApicAddr + (Count * 16)));
  }
  // 3. Program the VT-D remap engines
  for (RemapEng=0; RemapEng < RemapEngCount; RemapEng++) {

    VtdBarAddress = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RemapEng].VtdBarAddress;
    // 2. For each VT-d unit in the platform, allocate and initialize the invalidation queue/commands as follows

    // Allocate memory for the queued invalidation.
    Status = gBS->AllocatePool (EfiACPIReclaimMemory, 0x1000 + 0x1000, &Addr);
    ASSERT_EFI_ERROR (Status);
    ZeroMem (Addr, 0x1000 + 0x1000);
    Addr = (UINT64 *)((UINT64)Addr & (~0xFFF));

// Submit two descriptors to the respective VT-d unit's invalidation queue as follows:
    //  ??????????????? Program 1st descriptor in invalidation-queue as Interrupt-Entry-Cache Invalidation Descriptor
    // with G (Granularity) field Clear
    Addr[0] = 0x04;     // Interrupt Entry Cache Invalidate Descriptor
    Addr[1] = 0x00;

    // Program 2nd descriptor in invalidation-queue as Invalidation-Wait-Descriptor as follows:          +Status-Data=1
    // +Status-Address=address of variable tmp[unit +SW=1 +FN=1 +IF=0

    Addr[2] = ((UINT64)1 << 32) + (06 << 04) + 05;      // Invalidation Wait Descriptor

    TempDWord[RemapEng] = 00;
    Addr[3] = (UINTN)&TempDWord[RemapEng];    // Status Address [63:2] bits[127:65]

    // 3. Program the IRTA register to point to the IRT table.
    // For each VT-d unit in the platform, program interrupt-remap-table address and enable extended-interrupt-mode as follows
    IRTA  = (UINT64 *)((UINT64)VtdBarAddress + R_VTD_IRTA_REG);
    // ???? *(volatile UINT64*)IRTA = 04  + 0x800 + (UINT64)xApicAddr ;   // [0:3] size = 2 Power (X+1). Bit11 =1 Xapic mode Bit[12:63] addrerss
    *(volatile UINT64*)IRTA = 07  + 0x800 + (UINT64)xApicAddr ;   // [0:3] size = 2 Power (X+1). Bit11 =1 Xapic mode Bit[12:63] addrerss

    // b. Set SIRTP in the command register.
    Count = 0x1000;
    Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
    *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GCMD_REG) = (UINT32)(Value | BIT24);
    // save the above write for S3 resume
    Value = (UINT32)(Value | BIT24);
    S3BootScriptSaveMemWrite(
        S3BootScriptWidthUint32,
        ((UINT64)VtdBarAddress+ R_VTD_GCMD_REG),
        1,
        &Value
        );

    // Wait till the status bit is set indicating the completion of the SIRTP.
    while (Count)  {
      Count--;
      Value = *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GSTS_REG);
      if (Value & BIT24) break;
    }
    if (Count == 0) CpuDeadLoop ();
    *(volatile UINT64 *)((UINT64)VtdBarAddress+ R_VTD_IQA_REG) = (UINT64)Addr;
    // save the above write for S3 resume
    S3BootScriptSaveMemWrite(
        S3BootScriptWidthUint64,
        ((UINT64)VtdBarAddress+ R_VTD_IQA_REG),
        1,
        &Addr
        );
  }

  for (RemapEng=0; RemapEng < RemapEngCount; RemapEng++) {

    VtdBarAddress = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RemapEng].VtdBarAddress;
    // 4. For each VT-d unit in the platform, setup invalidation-queue base registers and enable invalidation as follows
    // Initialize a single descriptor which invalidates all the interrupt entries.
    // IQA register write (zeros IQH and IQT)

    // Enable queued invalidation in the command register.
    Count = 0x1000;
    Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
    *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GCMD_REG) = (UINT32)(Value | BIT26);
    // save the above write for S3 resume
    Value = (UINT32)(Value | BIT26);
    S3BootScriptSaveMemWrite(
            S3BootScriptWidthUint32,
            ((UINT64)VtdBarAddress + R_VTD_GCMD_REG),
            1,
            &Value);


    while (Count)  {
      Count--;
      Value = *(volatile UINT32 *)((UINT64)VtdBarAddress+ R_VTD_GSTS_REG);
      if( Value & BIT26) break;
    }
    if (Count == 0) CpuDeadLoop ();

    // Start invalidations, program the IQT register
    // Write the invalidation queue tail (IQT_REG) register as follows to indicate to hardware two descriptors are submitted:
    // +Bits 63:19 are 0 +Bits 18:4 gets value of 2h +Bits 3:0 are 0

    *(volatile UINT64 *)((UINT64)VtdBarAddress + R_VTD_IQT_REG) = (02 << 04); // Set tail to 02
    // save the above write to S3 resume
    Value = (02 << 04);
    S3BootScriptSaveMemWrite(
        S3BootScriptWidthUint64,
        ((UINT64)VtdBarAddress + R_VTD_IQT_REG),
        1,
        &Value
        );
  }

  for (RemapEng=0; RemapEng < RemapEngCount; RemapEng++) {

    VtdBarAddress = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RemapEng].VtdBarAddress;
    // 5. For each VT-d unit in the platform, wait for invalidation completion, and enable interrupt remapping as follows
    // Wait till the previously submitted invalidation commands are completed as follows

    // Poll on the variable tmp[unit] in memory, until its value is 1h.
    Count = 0x1000;
    while (Count)  {
      Count--;
      Value = TempDWord[RemapEng];
      if(Value & 01) break;
    }
    if (Count == 0) CpuDeadLoop ();
#if 1
    //
    // 5002479: Inband SMI not being generated on IIO Errors when X2APIC Mode is enabled
    //  [Reverting back the changes done for 4986502, as this results in interrupt generation loss
    //   on Processor Uncore/IIO.
    //   It is Intel Architectural requirement that when X2APIC is enabled, the VTd Interrupt Remap
    //   has to be enabled.]
    //


    // 4. Enable interrupt remapping

    // b. Set IRE in the command register
    Count = 0x1000;
    Value = *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GSTS_REG);
    *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GCMD_REG) = (UINT32)(Value | BIT25);
    // save the above write for S3 resume
    Value = (UINT32)(Value | BIT25);
    S3BootScriptSaveMemWrite(
                S3BootScriptWidthUint32,
                ((UINT64)VtdBarAddress + R_VTD_GCMD_REG),
                1,
                &Value);

    while (Count)  {
      Count--;
      Value = *(volatile UINT32 *)((UINT64)VtdBarAddress + R_VTD_GSTS_REG);
      if( Value & BIT25) break;
    }
    if (Count == 0) CpuDeadLoop ();
#endif
  }

  // 5. Enable external interrupts in the IOAPIC RTE entry 0
  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS)        = 0x10; //
  // save the above write for S3 resume
  Value = (UINT32)(0x10);
  S3BootScriptSaveMemWrite(
      S3BootScriptWidthUint32,
      ((UINT64)PCH_IOAPIC_ADDRESS),
      1,
      &Value);

  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS + 0x10) = 0x00; // Set index to the IRTE0
  // save the above write for S3 resume
  Value = (UINT32)(0x00);
  S3BootScriptSaveMemWrite(
      S3BootScriptWidthUint32,
      ((UINT64)PCH_IOAPIC_ADDRESS + 0x10),
      1,
      &Value);

  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS)        = 0x10+1;
  // save the above write for S3 resume
  Value = (UINT32)(0x10+1);
  S3BootScriptSaveMemWrite(
      S3BootScriptWidthUint32,
      ((UINT64)PCH_IOAPIC_ADDRESS),
      1,
      &Value);
  *(volatile UINT32 *)((UINT64)PCH_IOAPIC_ADDRESS + 0x10) = 0x10000;// Set Remap enable bit
  // save the above write for S3 resume
  Value = (UINT32)(0x10000);
  S3BootScriptSaveMemWrite(
      S3BootScriptWidthUint32,
      ((UINT64)PCH_IOAPIC_ADDRESS + 0x10),
      1,
      &Value);
}

// AptioV Grangeville Override Start: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER

VOID AddFullDevicePath( PCI_DEV_INFO  *dev, PCI_NODE  *PciNode, UINT8  *PciNodeIndex)
{
    EFI_PCI_IO_PROTOCOL         *PciIo;
    UINTN                       Segment, Bus, Device, Function;
    EFI_STATUS              Status;
    
    if ((dev == NULL) || (PciNode == NULL) || (PciNodeIndex == NULL))
    {
        return; // Return for Invalid parameter 
    }
    
    if (dev->Type == tPciRootBrg)
    {
        /* Uncomment this code if you want to add HOST bridge also*/
//        PciNode[*PciNodeIndex].Device          = (UINT8) 0;
//        PciNode[*PciNodeIndex].Function        = (UINT8) 0;
//        *PciNodeIndex++;
        return;
    }
    else
    {
        PciIo = (EFI_PCI_IO_PROTOCOL *)dev;          
                 
        Status = PciIo->GetLocation(PciIo, &Segment, &Bus, &Device, &Function);
        
        ASSERT_EFI_ERROR(Status);

        AddFullDevicePath(dev->ParentBrg, PciNode, PciNodeIndex); // Add parent device DATA 1st.

        PciNode[*PciNodeIndex].Device          = (UINT8) Device;
        PciNode[*PciNodeIndex].Function        = (UINT8) Function;
        *PciNodeIndex++;
    }
}
// AptioV Grangeville Override End: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER

EFI_STATUS
ReportDmar (
  EFI_DMA_REMAP_PROTOCOL      *DmaRemap
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT8                       RootBridgeLoop, IioSocketId, IioBusBase;
  UINT8                       DevIndex;
  UINT8                       PciNodeIndex;
  UINT8                       PciPortIndex;
  UINT8                       CBIndex,CBDMAIx;
  UINT64                      VtdMmioExtCap;
  UINT32                      VtdBase;
  EFI_VTD_SUPPORT_INSTANCE    *DmarPrivateData;
  UINT16                      NumberOfHpets;
  UINT16                      HpetCapIdValue;
  DEVICE_SCOPE                *DevScope;
  PCI_NODE                    *PciNode;
  // AptioV Server Override Start: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  //  EFI_PHYSICAL_ADDRESS        Pointer;
  //  UINT32                      AllignedSize;
  //  UINT32                      NumberofPages;
  // AptioV Server Override End: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  //AptioV server override start: Program IOAT based on one setup control EnableIOAT
  //UINT8                       *Array=NULL;
  //AptioV server override end: Program IOAT based on one setup control EnableIOAT
  BOOLEAN                     IntrRemapSupport = FALSE;
  BOOLEAN                     DmarFlagDone = FALSE;
  EFI_IIO_SYSTEM_PROTOCOL     *mIioSystem;
  EFI_CPUID_REGISTER          CpuidRegisters;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT32                      VidDid;  
  UINT8                       DmaFunc=0;  
  // AptioV Server Override Start: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  #if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    static EFI_GUID UsbProtocolGuid = EFI_USB_PROTOCOL_GUID;
    EFI_USB_PROTOCOL *UsbProtocol;
    EFI_PHYSICAL_ADDRESS    StartAddress = 0, EndAddress = 0;
  #endif
  // AptioV Server Override End: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
// AptioV Server Override Start: Uncommented the code for 2nd IIO
  UINT8                       PcieDeviceList[VARIABLE_GROUP_ELEMENTS] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
  UINT8                       PcieFunctionList[VARIABLE_GROUP_ELEMENTS] = {0, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3};
// AptioV Server Override End
// AptioV Server Override Start: Adding Dynamic Device Scope Entries for RMRR structure
  UINTN                       HandleCount;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  PCI_TYPE00                  PciConfigHeader;
  UINTN                       Segment, Bus, Device, Function;
  UINTN                       VariableSize;
// AptioV Server Override End: Adding Dynamic Device Scope Entries for RMRR structure

// AptioV Grangeville Override Start: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER
  PCI_DEV_INFO                *dev;
// AptioV Grangeville Override End: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER

  DmarPrivateData = VTD_SUPPORT_INSTANCE_FROM_THIS (DmaRemap);
  //
  // Get DMAR_HOST_ADDRESS_WIDTH from CPUID.(EAX=80000008h) return the Phyical Address
  // Size in the EAX register. EAX[7:0]
  // Sync with Brickland code  DMAR_HOST_ADDRESS_WIDTH 45 = 46 - 1
  //
      AsmCpuid (
      EFI_CPUID_VIR_PHY_ADDRESS_SIZE,
      &CpuidRegisters.RegEax,
      &CpuidRegisters.RegEbx,
      &CpuidRegisters.RegEcx,
      &CpuidRegisters.RegEdx
      );

  DmarPrivateData->Dmar->HostAddressWidth = (UINT8)((CpuidRegisters.RegEax & 0xFF)-1);
  DmarPrivateData->Dmar->Flags = EFI_ACPI_DMAR_TABLE_FLAGS_CLEAR;

  //
  // Locate PCI root bridge I/O protocol, for confirming PCI functions respond
  // to PCI configuration cycles.
  //
  Status = gBS->LocateProtocol (
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &PciRootBridgeIo
                );
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate memory to DevScope structures
  //
  Status = gBS->AllocatePool (EfiBootServicesData, 100 * sizeof (DEVICE_SCOPE), &DevScope);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->AllocatePool (EfiBootServicesData, 100 * sizeof (PCI_NODE), &PciNode);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &mIioSystem);
  ASSERT_EFI_ERROR (Status);

  for ( RootBridgeLoop =0; RootBridgeLoop < mIioUds->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++) {
    if(!mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) continue;

    IioSocketId = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].SocketID;
    IioBusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].BusBase;

    DevIndex                      = 00;
    PciNodeIndex                  = 00;

    mDrhd.Signature               = DRHD_SIGNATURE;
    mDrhd.SegmentNumber           = SEGMENT0;
    mDrhd.DeviceScopeNumber       = 00;
    mDrhd.DeviceScope             = DevScope;
    mDrhd.RegisterBase            = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress;

    ZeroMem (DevScope, 100 * sizeof (DEVICE_SCOPE));
    ZeroMem (PciNode, 100 * sizeof (PCI_NODE));

    VtdBase = (UINT32)mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress;
    VtdMmioExtCap     = *(volatile UINT64 *)(UINTN) (VtdBase + R_VTD_EXT_CAP_LOW);

    //
    // Check Interrupt Remap support.
    //
    if ((VtdMmioExtCap) & INTRREMAP) {
      IntrRemapSupport = TRUE;
    }

    DEBUG((EFI_D_ERROR, "T_TEST: Build DRHD Table IioSocketId    =%d.\n", IioSocketId));
    DEBUG((EFI_D_ERROR, "        mSystemConfiguration.IIO0Present=%d.\n", mSystemConfiguration.IIO0Present));
    DEBUG((EFI_D_ERROR, "        mSystemConfiguration.IIO1Present=%d.\n", mSystemConfiguration.IIO1Present));
    DEBUG((EFI_D_ERROR, "        mSystemConfiguration.IIO2Present=%d.\n", mSystemConfiguration.IIO2Present));
    DEBUG((EFI_D_ERROR, "        mSystemConfiguration.IIO3Present=%d.\n", mSystemConfiguration.IIO3Present));

    //
    // Setting these System wide flags once if InterruptRemap supported on any of the socket
    //
    if((mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap) && (IntrRemapSupport) && (DmarFlagDone == FALSE)){
      DmarPrivateData->Dmar->Flags = EFI_ACPI_DMAR_TABLE_FLAGS_INTR_REMAP_SET;
      if ( mAcpiParameter->CpuType == CPU_BDX) {
        if ( mSystemConfiguration.ProcessorXapic && mSystemConfiguration.X2ApicOptOut) {
          DmarPrivateData->Dmar->Flags |= EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT_SET;
        }
      } // CPU_BDX
      DmarFlagDone = TRUE; // DMAR Flags done
    } // InterruptRemap


    if (IioSocketId == 0 && ((mSystemConfiguration.IIO0Present))) {
      //
      // DRHD - Legacy IOH
      //
      mDrhd.Flags                   = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_SET;
      DEBUG((EFI_D_ERROR, "The InterruptRemap value is %02d\n",mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap));
      if((mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap) && (IntrRemapSupport)){

        //Aptio V Server Override - Enable X2APIC_OPT_OUT flag if not in xAPIC mode.
        if (!mSystemConfiguration.ProcessorXapic) DmarPrivateData->Dmar->Flags |= EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT;
        
        // PCH - IOAPIC
        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
        DevScope[DevIndex].EnumerationID      = PCH_IOAPIC_ID;
        DevScope[DevIndex].StartBusNumber     = PCI_BUS_NUMBER_PCH_IOAPIC;
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
        DevIndex++;

        PciNode[PciNodeIndex].Device          = (UINT8)PCI_DEVICE_NUMBER_PCH_IOAPIC;
        PciNode[PciNodeIndex].Function        = (UINT8)PCI_FUNCTION_NUMBER_PCH_IOAPIC;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;
        //
        //IIO0 - IOAPIC
        //
        if (mPlatformInfo->SysData.SysIoApicEnable & IIO0_IOAPIC){

          DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
          DevScope[DevIndex].EnumerationID      = IIO0_IOAPIC_ID;
          DevScope[DevIndex].StartBusNumber     = IioBusBase;
          DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
          DevIndex++;

          PciNode[PciNodeIndex].Device          = APIC_DEV_NUM;
          PciNode[PciNodeIndex].Function        = APIC_FUNC_NUM;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
        } // End of if (mPlatformInfo->SysData.SysIoApicEnable & IIO0_IOAPIC)

        HpetCapIdValue = *(UINT16 *)(UINTN)(HPET_BLOCK_ADDRESS);
        NumberOfHpets = (HpetCapIdValue >> 0x08) & 0x1F;  // Bits [8:12] contaoins the number of Hpets

        if(NumberOfHpets && (NumberOfHpets != 0x1f) &&
          (*((volatile UINT32 *)(UINTN)(HPET_BLOCK_ADDRESS + 0x100)) & BIT15)) {

          DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_MSI_CAPABLE_HPET;
          DevScope[DevIndex].EnumerationID      = 00;
          DevScope[DevIndex].StartBusNumber     = PCI_BUS_NUMBER_PCH_HPET;
          DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
          DevIndex++;

          PciNode[PciNodeIndex].Device          = PCI_DEVICE_NUMBER_PCH_HPET;
          PciNode[PciNodeIndex].Function        = PCI_FUNCTION_NUMBER_PCH_HPET0;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
        }
        IntrRemapSupport = FALSE;
      } // End of if(mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap)
    } else { // End of if (IioSocketId == 0)
      if (IioSocketId == 1 && ((mSystemConfiguration.IIO1Present))) {

        // DRHD - IIO1
        mDrhd.Flags                   = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_CLEAR;
        // IIO1 IOAPIC
        if ((mPlatformInfo->SysData.SysIoApicEnable & IIO1_IOAPIC)
             && (mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap) && (IntrRemapSupport)){
          DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
          DevScope[DevIndex].EnumerationID      = IIO1_IOAPIC_ID;
          DevScope[DevIndex].StartBusNumber     = IioBusBase;
          DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
          DevIndex++;

          PciNode[PciNodeIndex].Device          = APIC_DEV_NUM;
          PciNode[PciNodeIndex].Function        = APIC_FUNC_NUM;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
          IntrRemapSupport = FALSE;
        }
        // Dev_Scope - CB DMA
          //AptioV server override : Program IOAT based on one setup control EnableIOAT
       // Array = &(mSystemConfiguration.Cb3DmaEn[8]);

      } else if( IioSocketId == 2 && ((mSystemConfiguration.IIO2Present))) { // End of else if (IioSocketId == 1)
        // DRHD - IIO2
        mDrhd.Flags                   = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_CLEAR;

        // IIO2 IOAPIC
        if ((mPlatformInfo->SysData.SysIoApicEnable & IIO2_IOAPIC)
             && (mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap) && (IntrRemapSupport)){
          DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
          DevScope[DevIndex].EnumerationID      = IIO2_IOAPIC_ID;
          DevScope[DevIndex].StartBusNumber     = IioBusBase;
          DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
          DevIndex++;

          PciNode[PciNodeIndex].Device          = APIC_DEV_NUM;
          PciNode[PciNodeIndex].Function        = APIC_FUNC_NUM;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
          IntrRemapSupport = FALSE;
        }
        // Dev_Scope - CB DMA
        //AptioV server override : Commenting Array
        //Array = &(mSystemConfiguration.Cb3DmaEn[16]);

      } else if( IioSocketId == 3 && ((mSystemConfiguration.IIO3Present))) { // End of else if (IioSocketId == 2)

        // DRHD - IIO3
        mDrhd.Flags                   = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_CLEAR;

        // IIO3 IOAPIC
        if ((mPlatformInfo->SysData.SysIoApicEnable & IIO3_IOAPIC)
             && (mIioUds->IioUdsPtr->SetupInfo.Config.InterruptRemap)){
          DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC;
          DevScope[DevIndex].EnumerationID      = IIO3_IOAPIC_ID;
          DevScope[DevIndex].StartBusNumber     = IioBusBase;
          DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
          DevIndex++;

          PciNode[PciNodeIndex].Device          = APIC_DEV_NUM;
          PciNode[PciNodeIndex].Function        = APIC_FUNC_NUM;
          PciNodeIndex++;
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;

          mDrhd.DeviceScopeNumber++;
          IntrRemapSupport = FALSE;
        }
        // Dev_Scope - CB DMA
        //AptioV server override : Commenting Array
      //  Array = &(mSystemConfiguration.Cb3DmaEn[24]);
      } // end of else if( IioSocketId == 3)
 
      else continue;

      DEBUG((EFI_D_ERROR, "    CB: Build CB Index into DRHD.\n"));

      for(CBDMAIx=0;CBDMAIx < mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceCount;CBDMAIx++){
      //AptioV server override start: Program IOAT based on one setup control EnableIOAT
      if(mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceInfo[CBDMAIx].DmaControllerPresent && mIioSystem->IioGlobalData->SetupData.EnableIOAT){
      //AptioV server override end: Program IOAT based on one setup control EnableIOAT
        
         DmaFunc  = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceInfo[CBDMAIx].DmaStartAddress.PciFunNo;
         for(CBIndex = 0; CBIndex < mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceInfo[CBDMAIx].MaxDmaChannels; CBIndex++) {
        //
        // Skip end point devices which do not respond to PCI configuration cycles.
        //
        Status = PciRootBridgeIo->Pci.Read (
                    PciRootBridgeIo,
                    EfiPciWidthUint32,
                    EFI_PCI_ADDRESS (
                    IioBusBase,
                    04,
                    CBIndex,
                    0),
                    1,
                    &VidDid);

        if (EFI_ERROR (Status) || VidDid == 0xffffffff) {
          continue;
        }
        //DEBUG((EFI_D_ERROR, "    CB: Build CBIndex=%d when Array[CBIndex]=%d.\n",CBIndex,Array[CBIndex])); //AptioV server override : Commenting as Array is used

        //if( Array[CBIndex] == 0 ) continue;

        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
        DevScope[DevIndex].EnumerationID      = 00;
            DevScope[DevIndex].StartBusNumber     = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceInfo[CBDMAIx].DmaStartAddress.PciBusNo;
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
        DevIndex++;

        PciNode[PciNodeIndex].Device          = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].DmaDeviceInfo[CBDMAIx].DmaStartAddress.PciDevNo;
        PciNode[PciNodeIndex].Function        = DmaFunc;
        DmaFunc++;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;
      }
      //AptioV server override start: Program IOAT based on one setup control EnableIOAT
      }
      //AptioV server override end: Program IOAT based on one setup control EnableIOAT
      } // End of for for(CBIndex = 0; CBIndex <= 07; CBIndex++)
  
      //
      // DRHD - PCI-Ex ports
      //
      for(PciPortIndex = 0; PciPortIndex <= PCIE_PORT_MAX; PciPortIndex++) {

      //
      // Skip root ports which do not respond to PCI configuration cycles.
      //
      Status = PciRootBridgeIo->Pci.Read (
                  PciRootBridgeIo,
                  EfiPciWidthUint32,
                  EFI_PCI_ADDRESS (
                    IioBusBase,
                    mDevScopeATSR [PciPortIndex].PciNode->Device,
                    mDevScopeATSR [PciPortIndex].PciNode->Function,
                    0),
                  1,
                  &VidDid);
        if (EFI_ERROR (Status) || VidDid == 0xffffffff) {
          continue;
        }

        DEBUG((EFI_D_ERROR, " PCIEX: Build PCIEX root port=%d.\n",PciPortIndex));
        DEBUG((EFI_D_ERROR, "        PXPMap.PciPortIndex=%d.\n",mIioSystem->IioGlobalData->IioOutData.CurrentPXPMap[(RootBridgeLoop*(VARIABLE_GROUP_ELEMENTS))+PciPortIndex]));

        if(!mIioSystem->IioGlobalData->IioOutData.CurrentPXPMap[(RootBridgeLoop*(VARIABLE_GROUP_ELEMENTS))+PciPortIndex]) continue;

        DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
        DevScope[DevIndex].EnumerationID      = 00;
        DevScope[DevIndex].StartBusNumber     = IioBusBase;
        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
        DevIndex++;

        PciNode[PciNodeIndex].Device          = mDevScopeATSR[PciPortIndex].PciNode->Device;
        PciNode[PciNodeIndex].Function        = mDevScopeATSR[PciPortIndex].PciNode->Function;
        PciNodeIndex++;
        PciNode[PciNodeIndex].Device    = (UINT8) -1;
        PciNode[PciNodeIndex].Function  = (UINT8) -1;
        PciNodeIndex++;

        mDrhd.DeviceScopeNumber++;
      } // End of for(PciPortIndex = 0; PciPortIndex <= PCIE_PORT_MAX; PciPortIndex++)
    } // End of if (IioSocketId == 0)

    DmaRemap->InsertDmaRemap (DmaRemap, DrhdType, &mDrhd);

  } // End of for (RootBridgeLoop = 0; RootBridgeLoop < mIioUds->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++)

  //
  // Add Isoc Engine to DRHD table
  //
  if ((mIioUds->IioUdsPtr->SystemStatus.IsocEnable) || (mIioUds->IioUdsPtr->SystemStatus.MeSegEn)) {
    DevIndex                      = 00;
    PciNodeIndex                  = 00;

    mDrhdIsoc.Signature               = DRHD_SIGNATURE;
    mDrhdIsoc.SegmentNumber           = SEGMENT0;
    mDrhdIsoc.DeviceScopeNumber       = 00;
    mDrhdIsoc.DeviceScope             = DevScope;
    mDrhdIsoc.RegisterBase            = mIioUds->IioUdsPtr->PlatformData.IIO_resource[0].VtdBarAddress + VTD_ISOCH_ENGINE_OFFSET;

    ZeroMem (DevScope, 1 * sizeof (DEVICE_SCOPE));
    ZeroMem (PciNode, 1 * sizeof (PCI_NODE));

    DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
    DevScope[DevIndex].EnumerationID      = 00;
    DevScope[DevIndex].StartBusNumber     = 00;
    DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
    DevIndex++;

    PciNode[PciNodeIndex].Device          = PCI_DEVICE_NUMBER_PCH_AZALIA;
    PciNode[PciNodeIndex].Function        = PCI_FUNCTION_NUMBER_PCH_AZALIA;
    PciNodeIndex++;
    PciNode[PciNodeIndex].Device    = (UINT8) -1;
    PciNode[PciNodeIndex].Function  = (UINT8) -1;
    PciNodeIndex++;

    mDrhdIsoc.DeviceScopeNumber++;
    DmaRemap->InsertDmaRemap (DmaRemap, DrhdType, &mDrhdIsoc);
  }
  //
  // ATSR
  //
  if (mIioUds->IioUdsPtr->SetupInfo.Config.ATS) {
// AptioV Server Override Start. Only one ATSR should be there per PCI segment. So moving loop after filling ATSR header.
   /* for (RootBridgeLoop = 0; RootBridgeLoop < mIioUds->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++) {

      DEBUG((EFI_D_ERROR, "T_TEST: Build ATSR RootBridgeLoop=%d.\n",RootBridgeLoop));
      DEBUG((EFI_D_ERROR, "        IIO_resource.valid=%d.\n",mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid));

      if(!mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) continue;
      IioSocketId = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].SocketID;        

      if((IioSocketId == 00) && (!(mSystemConfiguration.IIO0Present)) ) continue;
      if((IioSocketId == 01) && (!(mSystemConfiguration.IIO1Present)) ) continue;
      if((IioSocketId == 02) && (!(mSystemConfiguration.IIO2Present)) ) continue;
      if((IioSocketId == 03) && (!(mSystemConfiguration.IIO3Present)) ) continue;

      IioBusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].BusBase;*/
// AptioV Server Override End: Only one ATSR should be there per PCI segment. So moving loop after filling ATSR header.

      PciNodeIndex            = 00;
      DevIndex                = 00;

      ZeroMem (DevScope, 100 * sizeof (DEVICE_SCOPE));
      ZeroMem (PciNode, 100 * sizeof (PCI_NODE));

      mAtsr.Signature         = ATSR_SIGNATURE;
      mAtsr.Flags             = 00;
      mAtsr.SegmentNumber     = SEGMENT0;
      mAtsr.DeviceScopeNumber = 00;
      mAtsr.DeviceScope       = DevScope;
      mAtsr.ATSRPresentBit    = (UINT32)-1;   // Not useful really Backwards project compatability (removwe it later)

      // AptioV Server Override Start: Only one ATSR should be there per PCI segment. So moving loop after filling ATSR header.
    for (RootBridgeLoop = 0; RootBridgeLoop < mIioUds->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++) {
      if(!mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) continue;
      IioSocketId = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].SocketID;        

      if((IioSocketId == 00) && (!(mSystemConfiguration.IIO0Present)) ) continue;
      if((IioSocketId == 01) && (!(mSystemConfiguration.IIO1Present)) ) continue;
      if((IioSocketId == 02) && (!(mSystemConfiguration.IIO2Present)) ) continue;
      if((IioSocketId == 03) && (!(mSystemConfiguration.IIO3Present)) ) continue;

      IioBusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
      // AptioV Server Override end

      //
      // Check device IOTLBs supported or not in VT-d Extended capability register
      //
      VtdBase = (UINT32)mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress;
      VtdMmioExtCap     = *(volatile UINT64 *)(UINTN) (VtdBase + R_VTD_EXT_CAP_LOW);

      //
      // ATSR is applicable only for platform supporting device IOTLBs through the VT-d extended capability register
      //
      if ((VtdMmioExtCap & BIT2) != 0) {
        //
        // Loop From Port 1 to 10 for Legacy IOH and 0 to 10 for Non-Legacy IOH
        //
        for(PciPortIndex = 0; PciPortIndex <= PCIE_PORT_MAX; PciPortIndex++)  {
          if((IioSocketId == 00) && (PciPortIndex == 00)) continue;

        //
        // Skip root ports which do not respond to PCI configuration cycles.
        //
        Status = PciRootBridgeIo->Pci.Read (
                    PciRootBridgeIo,
                    EfiPciWidthUint32,
                    EFI_PCI_ADDRESS (
                      IioBusBase,
                      mDevScopeATSR [PciPortIndex].PciNode->Device,
                      mDevScopeATSR [PciPortIndex].PciNode->Function,
                      0),
                    1,
                    &VidDid);
          if (EFI_ERROR (Status) || VidDid == 0xffffffff) {
            continue;
          }

          if(mIioSystem->IioGlobalData->IioOutData.CurrentPXPMap[(RootBridgeLoop*(VARIABLE_GROUP_ELEMENTS))+PciPortIndex]) {
            DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE;
            DevScope[DevIndex].EnumerationID      = 00;
            DevScope[DevIndex].StartBusNumber     = IioBusBase;
            DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
            DevIndex++;

            PciNode[PciNodeIndex].Device          = mDevScopeATSR[PciPortIndex].PciNode->Device;
            PciNode[PciNodeIndex].Function        = mDevScopeATSR[PciPortIndex].PciNode->Function;
            PciNodeIndex++;
            PciNode[PciNodeIndex].Device    = (UINT8) -1;
            PciNode[PciNodeIndex].Function  = (UINT8) -1;
            PciNodeIndex++;

            mAtsr.DeviceScopeNumber++;
          } // (mIioSystem->IioGlobalData->IioOutData.CurrentPXPMap[RootBridgeLoop][PciPortIndex])
        } // End of for(PciPortIndex = 0; PciPortIndex <= PCIE_PORT_MAX; PciPortIndex++)
      }
      /* AptioV server override. Publish only one ATSR for all IIO's. So moving code outside the RootBridgeLoop.
      if(mAtsr.DeviceScopeNumber)
        DmaRemap->InsertDmaRemap (DmaRemap, AtsrType, &mAtsr); */
    } // End of for (RootBridgeLoop = 0; RootBridgeLoop < mIohUds->IohUdsPtr->PlatformData.numofIIO; RootBridgeLoop++)
    // AptioV server override - start: To get information for all the IIO's
    if(mAtsr.DeviceScopeNumber)
      DmaRemap->InsertDmaRemap (DmaRemap, AtsrType, &mAtsr);
    // AptioV server override - end
  } // End of if (mIioUds->IioUdsPtr->SetupInfo.Config.ATS) {

  //
  // RMRR
  //
  // AptioV Server Override Start: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  #if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    Status = gBS->LocateProtocol(&UsbProtocolGuid, NULL, &UsbProtocol);
    if(!EFI_ERROR(Status))
    {
        Status = UsbProtocol->UsbGetRuntimeRegion(&StartAddress, &EndAddress);
        DEBUG((EFI_D_ERROR, "The RMRR Mem Base is 0x%x\n", StartAddress));
    }
  #endif

  /*  AlignedSize  = (MEM_BLK_COUNT * sizeof(MEM_BLK));

  if( AlignedSize % 0x1000)  AlignedSize  = ( (MEM_BLK_COUNT * sizeof(MEM_BLK)) & (~0xfff) ) + 0x1000; // aligend to 4k Boundary
  NumberofPages = AlignedSize/0x1000;
  //
  // Allocate memory (below 4GB)
  //
  Pointer = 0xffffffff;
  Status = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiReservedMemoryType,
                   NumberofPages,
                   &Pointer // Base address need to be 4K aligned for VT-d RMRR
                   );
  ASSERT_EFI_ERROR(Status);
  DEBUG((EFI_D_ERROR, "The RMRR Mem Base is 0x%x\n", Pointer));*/
  // AptioV Server Override End: For filling Reserved Base Address and Limit for RMRR structure of DMAR table
  
  if (DmaRemap->VtdFlag == TRUE) {
    //
    // RMRR
    //
// AptioV Server Override Start: Adding Dynamic Device Scope Entries for RMRR structure
    VariableSize = sizeof (SYSTEM_CONFIGURATION);
    Status = gRT->GetVariable (
                                L"IntelSetup",
                                &mSystemConfigurationGuid,
                                NULL,
                                &VariableSize,
                                &mSystemConfiguration
                                );
    ASSERT_EFI_ERROR (Status);
  
    PciNodeIndex            = 00;
    DevIndex                    = 00;

    ZeroMem (DevScope, 100 * sizeof (DEVICE_SCOPE));
    ZeroMem (PciNode, 100 * sizeof (PCI_NODE));

    mRmrr.Signature         = RMRR_SIGNATURE;
    mRmrr.SegmentNumber     = SEGMENT0;
    mRmrr.DeviceScopeNumber = 00;
#if defined AMIUSB_SUPPORT && AMIUSB_SUPPORT == 1
    mRmrr.RsvdMemBase       = StartAddress;
    mRmrr.RsvdMemLimit      = EndAddress;
#endif
    mRmrr.DeviceScope       = DevScope;
    //
    // Locate all PciIo protocol
    //
    Status = gBS->LocateHandleBuffer (
                 ByProtocol,
                 &gEfiPciIoProtocolGuid,
                 NULL,
                 &HandleCount,
                 &HandleBuffer
                 );
    for (Index = 0; Index < HandleCount; Index ++) {
      Status = gBS->HandleProtocol (
            HandleBuffer[Index],
            &gEfiPciIoProtocolGuid,
            &PciIo
            );
      if (EFI_ERROR(Status)) 
          continue;
      PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint32,
                  0,
                  sizeof (PciConfigHeader) / sizeof (UINT32),
                  &PciConfigHeader
                  );
      if ((PciConfigHeader.Hdr.ClassCode[0] == 0x20 || PciConfigHeader.Hdr.ClassCode[0] == 0x30) && PciConfigHeader.Hdr.ClassCode[1] == 0x03 && PciConfigHeader.Hdr.ClassCode[2] == 0x0C) {
          PciIo->GetLocation (
                  PciIo,
                  &Segment,
                  &Bus,
                  &Device,
                  &Function
                  );
          DEBUG((EFI_D_ERROR, "\n Found USB Device \n"));
          if (((mSystemConfiguration.PchUsb30Mode == 0) && (Device == PCI_DEVICE_NUMBER_PCH_XHCI)  && (Function == PCI_FUNCTION_NUMBER_PCH_XHCI)) ||
              ((mSystemConfiguration.PchUsb20[0] == 0) && (Device == PCI_DEVICE_NUMBER_PCH_USB)  && (Function == PCI_FUNCTION_NUMBER_PCH_EHCI)) ||
              ((mSystemConfiguration.PchUsb20[1] == 0) && (Device == PCI_DEVICE_NUMBER_PCH_USB_EXT)  && (Function == PCI_FUNCTION_NUMBER_PCH_EHCI2)))
                  continue;
          // determine if onboard or offboard device
          if ((Device == PCI_DEVICE_NUMBER_PCH_XHCI  && Function == PCI_FUNCTION_NUMBER_PCH_XHCI) ||
              (Device == PCI_DEVICE_NUMBER_PCH_USB  && Function == PCI_FUNCTION_NUMBER_PCH_EHCI) ||
              (Device == PCI_DEVICE_NUMBER_PCH_USB_EXT  && Function == PCI_FUNCTION_NUMBER_PCH_EHCI2)) {
              DEBUG((EFI_D_ERROR, "Found onboard USB Device \n"));
              DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
                        DevScope[DevIndex].EnumerationID      = 00;
                        DevScope[DevIndex].StartBusNumber     = (UINT8) Bus;
                        DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
                        DevIndex++;

                        PciNode[PciNodeIndex].Device          = (UINT8) Device;
                        PciNode[PciNodeIndex].Function        = (UINT8) Function;
                        PciNodeIndex++;
          } else {
                DEBUG((EFI_D_ERROR, "Found offboard USB Device \n"));
                DevScope[DevIndex].DeviceType         = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT;
                DevScope[DevIndex].EnumerationID      = 00;
                // AptioV Grangeville Override Start: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER
                //          DevScope[DevIndex].StartBusNumber     = (UINT8) Bus;
                DevScope[DevIndex].StartBusNumber     = (UINT8) 0; // Starting from Root port
                // AptioV Grangeville Override End: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER
                DevScope[DevIndex].PciNode            = &PciNode[PciNodeIndex];
                DevIndex++;
                
                // AptioV Grangeville Override Start: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER
                dev = (PCI_DEV_INFO *)PciIo;
                
                ASSERT( dev->Handle ==  HandleBuffer[Index] );
                
                if(dev->Type != tPciDevice){
                    continue;
                }
                
                AddFullDevicePath(dev, PciNode,&PciNodeIndex);
          } // end checking for onboard or offboard USB device

// Adding following data to show END of PATH.
          PciNode[PciNodeIndex].Device    = (UINT8) -1;
          PciNode[PciNodeIndex].Function  = (UINT8) -1;
          PciNodeIndex++;
// AptioV Grangeville Override End: For filling FULL DEVICE PATH for RMRR structure of OFF BOARD USB CONTROLLER

          mRmrr.DeviceScopeNumber++;
      }
    }
//    if(HandleBuffer)
//            pBS->FreePool(HandleBuffer);
// AptioV Server Override End: Adding Dynamic Device Scope Entries for RMRR structure
    DmaRemap->InsertDmaRemap (DmaRemap, RmrrType, &mRmrr);

// AptioV Server Override Start: OEM VTD RMRR support
    #if defined OemVtdRmrr_SUPPORT && OemVtdRmrr_SUPPORT == 1
    {
        EFI_OEM_VTD_RMRR_PROTOCOL *OemVtdRmrrProtocol;
        static EFI_GUID gEfiOemVtdRmrrProtocolGuid = EFI_OEM_VTD_RMRR_PROTOCOL_GUID;

        Status = gBS->LocateProtocol(&gEfiOemVtdRmrrProtocolGuid, NULL, &OemVtdRmrrProtocol);
        DEBUG((EFI_D_ERROR, "LocateProtocol gEfiOemVtdRmrrProtocolGuid. Status: %r \n", Status));
        if( !EFI_ERROR(Status) ) {
          Status = OemVtdRmrrProtocol->InsertOemVtdRmrr (DmaRemap);
          DEBUG((EFI_D_ERROR, "OemVtdRmrrProtocol->InsertOemVtdRmrr Status: %r \n", Status));
        }
    }
    #endif
// AptioV Server Override End: OEM VTD RMRR support

  }

  // AptioV Server Override Start: Adding the code for RHSA structure information
    if( mSystemMemoryMap != NULL) {
      // Update RHSA structure
      for ( RootBridgeLoop =0; RootBridgeLoop < MAX_SOCKET; RootBridgeLoop++) {
        if(!mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) continue;
            mRhsa.Signature         = RHSA_SIGNATURE;
            mRhsa.RegisterBase      = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress;
            mRhsa.Domian            = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[RootBridgeLoop].SocId;
            DmaRemap->InsertDmaRemap (DmaRemap, RhsaType, &mRhsa);
      }
    }
  // AptioV Server Override End: Adding the code for RHSA structure information

  gBS->FreePool (PciNode);
  gBS->FreePool (DevScope);

  return EFI_SUCCESS;
}

EFI_STATUS
VtdAcpiTablesUpdateFn (
  VOID
  )
{

  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_VERSION      TableVersion;
  EFI_DMA_REMAP_PROTOCOL      *DmaRemap;
  UINTN                       TableHandle;
  EFI_ACPI_COMMON_HEADER      *CurrentTable;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  EFI_MP_SERVICES_PROTOCOL    *MpService;
  UINTN                       NumberOfCPUs;
  UINTN                       NumberOfEnabledCPUs;
  UINTN                       MaximumNumberOfCPUs;

  UINTN                       HandleCount;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  PCI_TYPE01                  PciConfigHeader;
  UINTN                       Segment;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;
  UINT8                       PciExpressOffset;
  UINT32                      AcsOffset;
  UINT16                      PciExpressCapabilityReg;
  UINT8                       AcsCapCount;
  UINT16                      RequiredAcsCap;
  UINT32                      AcsCapRegValue;
  UINT16                      AcsConRegValue;
  ACS_DEV                     *AcsDevArray;
#ifdef ACS_WA
  UINT8                       RootBridgeLoop;
  UINT32                      VtdBase;
  UINT8                       CORE20_BUSNUM=0;
#endif

  
  PciExpressOffset = 0;
  AcsOffset        = 0;
  AcsCapCount      = 0;
  AcsCapRegValue   = 0;
  AcsConRegValue   = 0;
  RequiredAcsCap   =  ACS_SOURCE_VALIDATION | ACS_P2P_REQUEST_REDIRECT | ACS_P2P_COMPLETION_REDIRECT | ACS_UPSTREAM_FORWARDING;

  if (mSystemConfiguration.VTdSupport == 0) return EFI_NOT_FOUND;
  //
  // Locate all PciIo protocol
  //
  Status = gBS->LocateHandleBuffer (
               ByProtocol,
               &gEfiPciIoProtocolGuid,
               NULL,
               &HandleCount,
               &HandleBuffer
               );

  AcsDevArray = AllocateZeroPool (sizeof (ACS_DEV) * HandleCount);
  if (AcsDevArray == NULL) {
    ASSERT (AcsDevArray != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < HandleCount; Index ++) {
    gBS->HandleProtocol (
          HandleBuffer[Index],
          &gEfiPciIoProtocolGuid,
          &PciIo
          );
    PciIo->Pci.Read (
                PciIo,
                EfiPciIoWidthUint32,
                0,
                sizeof (PciConfigHeader) / sizeof (UINT32),
                &PciConfigHeader
                );
    if ((PciConfigHeader.Hdr.ClassCode[0] == 0x00 || PciConfigHeader.Hdr.ClassCode[0] == 0x01) && PciConfigHeader.Hdr.ClassCode[1] == 0x04 && PciConfigHeader.Hdr.ClassCode[2] == 0x06) {
      //
      // 060400h or 060401h indicates it's PCI-PCI bridge, get its bus number, device number and function number
      //
      // Check for ACS Control setup
      //
      if ( mSystemConfiguration.AcsControl == 1 ) {
          if (PciConfigHeader.Bridge.PrimaryBus != 0) {
              continue;
          }
      }
      PciIo->GetLocation (
              PciIo,
              &Segment,
              &Bus,
              &Device,
              &Function
              );
      if (PciConfigHeader.Hdr.Status == EFI_PCI_STATUS_CAPABILITY) {
        //
        // the bridge support Capability list and offset 0x34 is the pointer to the data structure
        //
        //
        // Detect if PCI Express Device
        //
        Status = LocateCapRegBlock (PciIo, EFI_PCI_CAPABILITY_ID_PCIEXP, &PciExpressOffset, NULL);

        if (Status == EFI_SUCCESS) {
          //
          // this bridge device is a PCI Express bridge
          // Check if it is downstream port of PCIE switch
          //
          PciExpressCapabilityReg = PciExpressRead16(PCI_PCIE_ADDR(Bus, Device , Function, PciExpressOffset + EFI_PCI_EXPRESS_CAPABILITY_REGISTER));

          //
          // BIT 7:4 indicate Device/port type, 0110b indicates downstream port of PCI express switch
          //
          if ((PciExpressCapabilityReg & 0x00F0) == 0x60) {
            //
            // it is downstream port of PCI Express switch
            // Look for ACS capability register in PCI express configuration space
            //
            Status = LocatePciExpressCapRegBlock (PciIo, EFI_PCIE_CAPABILITY_ID_ACS, &AcsOffset, NULL);
            DEBUG((EFI_D_ERROR, "ACS capable port is B%x.D%x.F%x - ACS Cap offset - 0x%x\n", Bus, Device, Function, AcsOffset));

            if (Status == EFI_SUCCESS) {
              //
              // Read ACS capability register
              //
              AcsCapRegValue = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, AcsOffset + ACS_CAPABILITY_REGISTER));

              if ((AcsCapRegValue & RequiredAcsCap) == RequiredAcsCap) {
                //
                // The PCI express downstream port support ACS, record this port
                //
                AcsDevArray[AcsCapCount].BusNumber = Bus;
                AcsDevArray[AcsCapCount].DevNumber = Device;
                AcsDevArray[AcsCapCount].FunNumber = Function;
                AcsDevArray[AcsCapCount].AcsOffset = AcsOffset;
                AcsCapCount++;
#ifdef ACS_WA
              } else {
                //
                // Even though this PCIe switch downstream port support ACS, but it does not meet all the required capabilities, so also disable the VT-d
                //
                DisableVtd = TRUE;
                break;
              }
            } else {
              //
              // At least one PCIe switch downstream port does not support ACS, just disable the VT-d report
              //
              DisableVtd = TRUE;
              break;
            }
#else
          }
        }
#endif
          }
      }
    }
  }
  }  /// End for

  //
  //Free the Handle buffer
  //
  if (HandleBuffer != NULL) {
    gBS->FreePool (HandleBuffer);
  }

  ASSERT (AcsCapCount <= HandleCount);

#ifdef ACS_WA
  if (DisableVtd == TRUE ) {
    //
    // at least one PCIE downstream port does not support ACS so need to disable VT-d
    //
    //
    // Disable VT-d BAR
    //
    for ( RootBridgeLoop =0; RootBridgeLoop < mIioUds->IioUdsPtr->PlatformData.numofIIO; RootBridgeLoop++) {
      if(!mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].Valid) continue;
      VtdBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].VtdBarAddress;
      CORE20_BUSNUM = mIioUds->IioUdsPtr->PlatformData.IIO_resource[RootBridgeLoop].BusBase;
      PciExpressWrite32(PCI_PCIE_ADDR(CORE20_BUSNUM, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_VTBAR), VtdBase & (~B_IIO_VT_BAR_EN));
    }

    //
    // Disable Setup option of VT-d
    //
    mSystemConfiguration.VtdAcsWa = FALSE;

  // AptioV server override: Changed variable name to Intelsetup
    Status = gRT->SetVariable (
                            L"IntelSetup",
                            &mSystemConfigurationGuid,
                            //SETUP_VARIABLE_ATTRIBUTES,// AptioV Server Override
                            mVariableAttr, // AptioV Server Override: Remove NVRAM RT attribute
                            sizeof(SYSTEM_CONFIGURATION),
                            &mSystemConfiguration
                            );

    //
    // Do not report DMAR table to OS/VMM then OS/VMM have no knowledge of VT-d and VT-d2
    //
    FreePool (AcsDevArray);
    return EFI_NOT_FOUND;
  } else {
#endif
    //
    // all PCI express switch downstream ports support ACS and meet the required ACS capabilities
    // for each downstream ports, enable the required Capabilities in ACS control register
    //
    for (Index = 0; Index < AcsCapCount; Index ++) {
      //
      // Program the corresponding bits in ACS control register
      //
      AcsConRegValue = PciExpressRead16(PCI_PCIE_ADDR(AcsDevArray[Index].BusNumber, AcsDevArray[Index].DevNumber , AcsDevArray[Index].FunNumber, AcsDevArray[Index].AcsOffset + ACS_CONTROL_REGISTER));
      DEBUG((EFI_D_ERROR, "AcsConRegValue is 0x%x\n", AcsConRegValue));
      AcsConRegValue |= (ACS_SOURCE_VALIDATION_ENABLE|ACS_P2P_REQUEST_REDIRECT_ENABLE|ACS_P2P_COMPLETION_REDIRECT_ENABLE|ACS_UPSTREAM_FORWARDING_ENABLE);
      DEBUG((EFI_D_ERROR, "After Enable BITs AcsConRegValue is 0x%x\n", AcsConRegValue));
      PciExpressWrite16(PCI_PCIE_ADDR(AcsDevArray[Index].BusNumber, AcsDevArray[Index].DevNumber , AcsDevArray[Index].FunNumber, AcsDevArray[Index].AcsOffset + ACS_CONTROL_REGISTER), AcsConRegValue);
      //
      // report VT-d and other features to OS/VMM, report DMAR and remapping engine to OS/VMM
      //
  } //
#ifdef ACS_WA
  }
#endif

  //
  // Locate the MP services protocol
  //
  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
              MpService,
              &NumberOfCPUs,
              &NumberOfEnabledCPUs
              );

  MaximumNumberOfCPUs = MAX_CPU_NUM;

  //
  // Find the AcpiSupport protocol
  //
//  Status = LocateSupportProtocol (&gEfiAcpiSupportProtocolGuid, &AcpiSupport, 0);
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  TableVersion = EFI_ACPI_TABLE_VERSION_2_0;

  Status = gBS->LocateProtocol (
                  &gEfiDmaRemapProtocolGuid,
                  NULL,
                  &DmaRemap
                  );
  if ((Status == EFI_SUCCESS) && (DmaRemap->VtdFlag == TRUE)) {
    ReportDmar (DmaRemap);
    Status = DmaRemap->GetDmarTable (DmaRemap, &CurrentTable);

    if (EFI_ERROR (Status)) {

      DEBUG ((EFI_D_ERROR, "\nWARNING: VT-D ACPI table was not found!\n"));
      ASSERT_EFI_ERROR (Status);

    } else {
      //
      // Perform any table specific updates.
      //
      Status = PlatformUpdateTables (CurrentTable, NumberOfCPUs);
      ASSERT_EFI_ERROR (Status);

      TableHandle = 0;
// APTIOV OVERRIDE START: EIP#470404
// To support EDKII implemetation of ACPI Protocols
#if 0      
      Status = AcpiSupport->SetAcpiTable (
                            AcpiSupport,
                            CurrentTable,
                            TRUE,
                            TableVersion,
                            &TableHandle
                            );
      ASSERT_EFI_ERROR (Status);
#endif
        Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            CurrentTable,
                            CurrentTable->Length,
                            &TableHandle
                            );
        ASSERT_EFI_ERROR (Status);
// APTIOV OVERRIDE END: EIP#470404
    }
  } else {
    DEBUG ((EFI_D_ERROR, "\nWARNING: VT-D ACPI protocol not installed - Cannot find gEfiDmaRemapProtocolGuid!\n"));
  }

  // Enable VT-D interrupt remapping
  if(VtdIntRemapEnFalg == TRUE) {
    EnableVtdIntRemapping();
  }

  FreePool (AcsDevArray);

  return EFI_SUCCESS;
}


EFI_STATUS
LocateCapRegBlock(
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                CapID,
  OUT    UINT8                *PciExpressOffset,
  OUT    UINT8                *NextRegBlock)
{
  UINT16  CapabilityID;
  UINT32  Temp;
  UINT8   CapabilityPtr;
  UINT16  CapabilityEntry;

  PciIo->Pci.Read (
            PciIo,
            EfiPciIoWidthUint32,
            PCI_CAPBILITY_POINTER_OFFSET,
            1,
            &Temp
            );

  CapabilityPtr = (UINT8)Temp;

  while ((CapabilityPtr >= 0x40) && (CapabilityPtr < 0x100)) {
    //
    // Mask it to DWORD alignment per PCI spec
    //
    CapabilityPtr &= 0xFC;
    PciIo->Pci.Read (
               PciIo,
               EfiPciIoWidthUint16,
               CapabilityPtr,
               1,
               &CapabilityEntry
               );

    CapabilityID = (UINT8) CapabilityEntry;

    if (CapabilityID == CapID) {
      *PciExpressOffset = CapabilityPtr;
      if (NextRegBlock != NULL) {
        *NextRegBlock = (UINT8) ((CapabilityEntry >> 8) & 0xFC);
      }

      return EFI_SUCCESS;
    }

    CapabilityPtr = (UINT8) ((CapabilityEntry >> 8) & 0xFC);
  }

  return EFI_NOT_FOUND;
}


EFI_STATUS
LocatePciExpressCapRegBlock (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT16               CapID,
  OUT    UINT32               *Offset,
  OUT    UINT32               *NextRegBlock
)
{
  UINT32  CapabilityPtr;
  UINT32  CapabilityEntry;
  UINT16  CapabilityID;
  // AptioV Server Override Start: To get the correct CapabilityEntry for Dev 8 Fun 0
    UINTN                       Segment;
    UINTN                       Bus;
    UINTN                       Device;
    UINTN                       Function;
  // AptioV Server Override End: To get the correct CapabilityEntry for Dev 8 Fun 0

  CapabilityPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;

  while ((CapabilityPtr != 0) && (CapabilityPtr < 0x1000)) {
    //
    // Mask it to DWORD alignment per PCI spec
    //
    CapabilityPtr &= 0xFFC;
    // AptioV Server Override Start: To get the correct CapabilityEntry for Dev 8 Fun 0
    /*    PciIo->Pci.Read (
                   PciIo,
                   EfiPciIoWidthUint32,
                   CapabilityPtr,
                   1,
                   &CapabilityEntry
                   );*/
        PciIo->GetLocation (
                PciIo,
                &Segment,
                &Bus,
                &Device,
                &Function
                );
        CapabilityEntry = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, CapabilityPtr));
    // AptioV Server Override End: To get the correct CapabilityEntry for Dev 8 Fun 0

    CapabilityID = (UINT16) CapabilityEntry;

    if(CapabilityID == 0xFFFF) break;   //AptioV Server Override: Check if Extended PciExpress Config. Space is accessible.

    if (CapabilityID == CapID) {
      *Offset = CapabilityPtr;
      if (NextRegBlock != NULL) {
        *NextRegBlock = (CapabilityEntry >> 20) & 0xFFF;
      }

      return EFI_SUCCESS;
    }

    CapabilityPtr = (CapabilityEntry >> 20) & 0xFFF;
  }

  return EFI_NOT_FOUND;
}


VOID
DisableAriForwarding (
  VOID
  )
{
  EFI_STATUS                  Status;
  UINTN                       HandleCount;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  PCI_TYPE01                  PciConfigHeader;
  UINTN                       Segment;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;
  UINT8                       PciExpressOffset;
  UINT16                      DevCon2;
  UINT16                      PcieCap;  // AptioV server override - Grantley platform is always disabling ARI forwarding on PCI Express rev. 1.0 bridge.

  //
  // Disable ARI forwarding before handoff to OS, as it may not be ARI-aware
  //
  //
  // ARI forwarding exist in bridge
  //

  //
  // Locate all PciIo protocol
  //
  Status = gBS->LocateHandleBuffer (
               ByProtocol,
               &gEfiPciIoProtocolGuid,
               NULL,
               &HandleCount,
               &HandleBuffer
               );
  for (Index = 0; Index < HandleCount; Index ++) {
    gBS->HandleProtocol (
          HandleBuffer[Index],
          &gEfiPciIoProtocolGuid,
          &PciIo
          );
    PciIo->Pci.Read (
                PciIo,
                EfiPciIoWidthUint32,
                0,
                sizeof (PciConfigHeader) / sizeof (UINT32),
                &PciConfigHeader
                );
    if ((PciConfigHeader.Hdr.ClassCode[0] == 0x00 || PciConfigHeader.Hdr.ClassCode[0] == 0x01) && PciConfigHeader.Hdr.ClassCode[1] == 0x04 && PciConfigHeader.Hdr.ClassCode[2] == 0x06) {
      //
      // 060400h or 060401h indicates it's PCI-PCI bridge, get its bus number, device number and function number
      //
      PciIo->GetLocation (
              PciIo,
              &Segment,
              &Bus,
              &Device,
              &Function
              );
      if (PciConfigHeader.Hdr.Status == EFI_PCI_STATUS_CAPABILITY) {
        //
        // the bridge support Capability list and offset 0x34 is the pointer to the data structure
        //
        //
        // Detect if PCI Express Device
        //
        Status = LocateCapRegBlock (PciIo, EFI_PCI_CAPABILITY_ID_PCIEXP, &PciExpressOffset, NULL);
        if (Status == EFI_SUCCESS) {

// AptioV server override start- Grantley platform is always disabling ARI forwarding on PCI Express rev. 1.0 bridge.
          PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + 0x02,
                  1,
                  &PcieCap
                  );
          //
          // Check PCIe Capability Version
          // ARI Forwarding is not available in PCIe rev 1.0.
          //
          if ((PcieCap & 0x0F) < 2) continue;
// AptioV server override end- Grantley platform is always disabling ARI forwarding on PCI Express rev. 1.0 bridge.

          //
          // this bridge device is a PCI Express bridge, Check ARI forwarding bit in Device Control 2 register
          //
          PciIo->Pci.Read (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + 0x28,
                  1,
                  &DevCon2
                  );
          if ((DevCon2 & BIT5) != 0) {
            //
            // ARI forwarding enable bit is set, we need to clear this bit before handing off control to OS
            // because OS may not ARI aware
            //
            DevCon2 = DevCon2 & (~BIT5);
            DEBUG((EFI_D_INFO, "ARI forwarding disable before booting OS,DevCon2 value is 0x%x\n", DevCon2));
            PciIo->Pci.Write (
                  PciIo,
                  EfiPciIoWidthUint16,
                  PciExpressOffset + 0x28,
                  1,
                  &DevCon2
                  );
          }
        }
      }
    }
  }
}

