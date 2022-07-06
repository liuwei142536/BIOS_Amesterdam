/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  2007 - 2013, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  QuiesceSupport.c

Abstract:

  Quiesce Driver implementation file. 

Revision History:

--*/
//BKL_PORTING - disable warning 4005; some MACROS are re-defined. Remove this after clean-up
#pragma warning(disable :4005)

#include "QuiesceSupport.h"
#include "QuiesceMain.h"

#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#define IA32_PG_P                   1u
#define IA32_PG_RW                  (1u << 1)
#define IA32_PG_USR                 (1u << 2)
#define IA32_PG_WT                  (1u << 3)
#define IA32_PG_CD                  (1u << 4)
#define IA32_PG_A                   (1u << 5)
#define IA32_PG_D                   (1u << 6)
#define IA32_PG_PS                  (1u << 7)
#define IA32_PG_G                   (1u << 8)
#define IA32_PG_AVL1                (7u << 9)
#define IA32_PG_PAT_2M              (1u << 12)
#define IA32_PG_PAT_4K              IA32_PG_PS

// don't want to have lower 12bit
#define PAGE_TABLE_ENTRY_MASK          (0xFFEFF000)

#define PAGE_TABLE_SIZE_4K             (1 << 7)


extern UINT8                      QuiesceCacheCodeTemplate[];
extern UINT32                     QuiesceCacheCodeSize;
extern UINT64                     Misc_feature_control;

static BOOLEAN                    DataBufferLock;
extern UINTN                      mCpuCounter;
extern SPIN_LOCK                  mApLock; 
extern UINT32                     mCpuFamilModel;

QUIESCE_COMMUNICATION_DATA_STRUCT mQuiesceCommData;

EFI_SMM_CPU_PROTOCOL               *mSmmCpu;
EFI_SMM_CPU_SERVICE_PROTOCOL       *mSmmCpuServiceProtocol;

IIO_UDS                            *mIioUdsData;

EFI_QUIESCE_SUPPORT_PROTOCOL mQuiesceSupport[] = {
    GetQuiesceCodeMemoryInfo,
    GetQuiesceDataMemoryInfo,
    QuiesceApProc,
    AquireDataBuffer,
    ReleaseDataBuffer,
    MonarchAPCheckIn,
    QuiesceMain,
    SystemQuiesceUnQuiesce,
};

EFI_HANDLE            mHandle = NULL;

STATIC UINT32 mIioBusBase[MAX_SOCKET];
STATIC UINT32 mPcieMsiMskData[MAX_SOCKET][11];
STATIC UINT32 mCsiPIntrVlwData[MAX_SOCKET];
STATIC UINT32 mIIR_CIPDCASAD[MAX_SOCKET];


//
// Prototypes
//
EFI_STATUS
QuiesceSupportInitialization (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
);

EFI_STATUS
AllocateQuiesceMemory (
    VOID
);


/**********************************
 * This routine prepare page table
 * to use in QuiesceRunOutOfCache.
 ***********************************/
VOID
PreparePageTable(
    IN EFI_PHYSICAL_ADDRESS QuiesceBase
);

UINT64
ReadCr3 (
    VOID
);

VOID
Setup4KPageTable(
    IN EFI_PHYSICAL_ADDRESS QuiesceBase,
    IN EFI_PHYSICAL_ADDRESS QuiescePageEntry,
    IN BOOLEAN  IsQuiescePage
);

// prototypes related ASM 
UINT8
QuiesceAcquireLock (
    IN   UINT8            *Lock
);

VOID
QuiesceReleaseLock (
    IN   UINT8            *Lock
);

    
BOOLEAN
IsSmrrSupported (
  VOID
  )
{
  UINT64                            MtrrCap;

  MtrrCap = AsmReadMsr64(EFI_MSR_IA32_MTRR_CAP);
  if ((MtrrCap & IA32_MTRR_SMRR_SUPPORT_BIT) == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

EFI_STATUS
EFIAPI
QuiesceSupportInitialization (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
/*++

Routine Description:

  Initializes the Quiesce Support Driver.

Arguments:

  ImageHandle  -  The image handle of Wake On Lan driver.
  SystemTable  -  The starndard EFI system table.

Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.
  Other                 -  Error occured during execution. 

--*/
{
  EFI_STATUS                  Status;
  EFI_IIO_UDS_PROTOCOL        *IioUds;

  mQuiesceCommData.AvailableBufferLegth = QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE - sizeof(QUIESCE_DATA_TABLE_ENTRY);

  //
  // Install the Quiesce Protocol.
  //
  Status = gSmst->SmmInstallProtocolInterface (
      &mHandle,
      &gEfiQuiesceProtocolGuid,
      EFI_NATIVE_INTERFACE,
      &mQuiesceSupport
  );

  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Error, failed to install mQuiesceSupport: %x\n", Status));
    return EFI_UNSUPPORTED;
  }

  AllocateQuiesceMemory();

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuProtocolGuid, NULL, (VOID **) &mSmmCpu);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiSmmCpuServiceProtocolGuid, NULL, (VOID **) &mSmmCpuServiceProtocol);
  ASSERT_EFI_ERROR (Status);

  Status = SystemTable->BootServices->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &IioUds);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mIioUdsData = (IIO_UDS *)IioUds->IioUdsPtr;

  QuiesceMainInit();

  return EFI_SUCCESS;
}


EFI_STATUS
AllocateQuiesceMemory (
    VOID
)
{

  EFI_STATUS                Status;
  EFI_PHYSICAL_ADDRESS      QuiesceBaseAddress;
  EFI_PHYSICAL_ADDRESS      PageTableBasAddress;
  UINTN                     TsegBase;

  QuiesceBaseAddress  = 0;
  PageTableBasAddress = 0;
  TsegBase = 0;

  if (IsSmrrSupported == FALSE)
      ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);


  TsegBase = AsmReadMsr64(0x1F2) & 0xFFFFF000; //read SMRR base

  DEBUG ((DEBUG_ERROR, "TSEG Base: %x\n", TsegBase));

  QuiesceBaseAddress = TsegBase + EFI_PAGE_SIZE;	//One page is used by PiSmmCpuDxeSmm driver for SmramDescriptor->CpuStart (bugbug dependency on AcpiVariableHobOnSmramReserveHobThunk)

  // 32 pages (128K) Quiesce code and data, 16 more pages to 
  // setup page tables for Quiesce memory
  Status = gSmst->SmmAllocatePages (
      AllocateAddress, //AllocateAnyPages, //AllocateAddress, //
      EfiRuntimeServicesData,
      64, //48,     //pages, if use AnyPage, 64
      &QuiesceBaseAddress
  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Allocate Quiesce mem failed: %x\n", Status));
    return Status;
  }

  //if use AllocateAnyPages:
  QuiesceBaseAddress &= 0xFFFF0000;
  QuiesceBaseAddress += 0x10000;

  mQuiesceCommData.CodeBaseAddress = QuiesceBaseAddress;
  mQuiesceCommData.CodeSize = QUIESCE_CODE_SIZE;

  mQuiesceCommData.DataBaseAddress = QuiesceBaseAddress + 2 * QUIESCE_CODE_SIZE;
  mQuiesceCommData.DataSize = QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE - sizeof(QUIESCE_DATA_TABLE_ENTRY);

  mQuiesceCommData.PageTableBaseAddr = QuiesceBaseAddress + QUIESCE_CODE_SIZE;


  // save Quiesce code in Code area
  CopyMem((VOID *)QuiesceBaseAddress, QuiesceCacheCodeTemplate, QuiesceCacheCodeSize);
  PreparePageTable(QuiesceBaseAddress);

  DEBUG ((DEBUG_ERROR, "Quiesce Base: %x\n", QuiesceBaseAddress));


  DEBUG ((DEBUG_ERROR, "Quiesce Prot CodeBase: %x\n", 
      mQuiesceCommData.CodeBaseAddress));


  return Status;


}

EFI_STATUS
GetQuiesceCodeMemoryInfo (
    IN OUT UINTN *QuiesceCodeBaseAddress,
    IN OUT UINTN *QuiesceCodeSize
) 
/*++

Routine Description:

  This routine returns pointer to base address and size of .
  Quiesce Code memory.

Arguments:

  QuiesceCodeBaseAddress  - Base address of Quiesce Code memory
  QuiesceCodeSize         - Quiesce Code memory size

Returns:

  EFI_STATUS       

--*/
{
  EFI_STATUS Status;

  Status = EFI_UNSUPPORTED;

  if((QuiesceCodeBaseAddress == NULL) || (QuiesceCodeSize == NULL)) {
    return Status;
  }

  *QuiesceCodeBaseAddress = (UINTN) mQuiesceCommData.CodeBaseAddress;
  *QuiesceCodeSize = QUIESCE_CODE_SIZE;

  return EFI_SUCCESS;

}

EFI_STATUS
GetQuiesceDataMemoryInfo (
    IN OUT UINTN *QuiesceDataBaseAddress,
    IN OUT UINTN *QuiesceDataSize
)
/*++

Routine Description:

  This routine returns pointer to base address and size of .
  Quiesce Data memory.

Arguments:

  QuiesceDataBaseAddress  - Base address of Quiesce Data memory
  QuiesceDataSize         - Quiesce Data memory size

Returns:

  EFI_STATUS       

--*/
{
  EFI_STATUS Status;

  Status = EFI_UNSUPPORTED;

  if((QuiesceDataBaseAddress == NULL) || (QuiesceDataSize == NULL)) {
    return Status;
  }
  *QuiesceDataBaseAddress = (UINTN) mQuiesceCommData.DataBaseAddress;
  *QuiesceDataSize = QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE - sizeof(QUIESCE_DATA_TABLE_ENTRY);

  return EFI_SUCCESS;

}

EFI_STATUS 
EFIAPI 
QuiesceApPollingProc (
    VOID
)
/*++

Routine Description:

  This routine is API for AP to enter Quiesce AP handler and wait for BSP completion.

Arguments:

  None

Returns:

  EFI_STATUS       

--*/
{
  UINT64                OldMiscMsr, OldPatMsr;
  UINT32                ApicId;

  OldMiscMsr = 0;
  OldPatMsr  = 0;

  while (!AcquireSpinLockOrFail (&mApLock));
  mCpuCounter ++;
  ReleaseSpinLock (&mApLock);

  ApicId = GetApicId();
  //
  // Disable prefetch
  //
  if((ApicId & 1) == 0) { //only primary thread do core scope msr
  OldMiscMsr = AsmReadMsr64(MSR_MISC_FEATURE_CONTROL);
    AsmWriteMsr64(MSR_MISC_FEATURE_CONTROL, 0x000F);
  }
  OldPatMsr  = AsmReadMsr64(MSR_IA32_CR_PAT);
  AsmWriteMsr64(MSR_IA32_CR_PAT, (OldPatMsr & ~(UINT64)0xFFFFFFFF) | 0x0406);    

  // poll Quiesce status till it finish quiesce
  while (!(mQuiesceCommData.QuiesceStatus4APs & B_UNQUIESCED));    

  //
  // Restroe prefetch
  //
  if((ApicId & 1) == 0) {
  AsmWriteMsr64(MSR_MISC_FEATURE_CONTROL, OldMiscMsr);
  }
  AsmWriteMsr64(MSR_IA32_CR_PAT, OldPatMsr);

  return EFI_SUCCESS;
}

EFI_STATUS 
EFIAPI 
QuiesceApProc (
    VOID
)
/*++

Routine Description:

  This routine is API for AP to enter Quiesce AP handler.

Arguments:

  None

Returns:

  EFI_STATUS       

--*/
{
  QuiesceApHandler(mQuiesceSupport);
  return EFI_SUCCESS;

}


EFI_STATUS
EFIAPI
AquireDataBuffer(
    IN OUT EFI_PHYSICAL_ADDRESS   *BufferAddress
)
/*++

Routine Description:

  This routine returns pointer to next avaialble address in
  Quiesce Data memory.

Arguments:

  BufferAddress       -  pointer to next avialble quiesce memory

Returns:

  EFI_STATUS       

--*/
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  if(BufferAddress != NULL) {

    Status = QuiesceAcquireLock(&DataBufferLock);
    if(Status != EFI_SUCCESS) {
      return EFI_ACCESS_DENIED;
    }

    if(mQuiesceCommData.AvailableBufferLegth < sizeof (QUIESCE_DATA_TABLE_ENTRY)) {
      Status = EFI_OUT_OF_RESOURCES;
      *BufferAddress = 0;
    } else {
      *BufferAddress = (mQuiesceCommData.DataBaseAddress + \
          QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE - mQuiesceCommData.AvailableBufferLegth - sizeof(QUIESCE_DATA_TABLE_ENTRY));

      mQuiesceCommData.AvailableBufferLegth = 0;
    }   
  } else {
    Status = EFI_INVALID_PARAMETER;
  }


  return Status;

}

EFI_STATUS
EFIAPI
ReleaseDataBuffer(
    IN EFI_PHYSICAL_ADDRESS  BufferAddress
)
/*++

Routine Description:

  This routine release the Quiesce Data memory from the address 
  passed in.

Arguments:

  BaseAddress       -  Start address to be freed

Returns:

  EFI_STATUS       

--*/
{
  EFI_STATUS Status;         
  EFI_PHYSICAL_ADDRESS BaseAddress;              


  Status = EFI_SUCCESS;

  BaseAddress = mQuiesceCommData.DataBaseAddress;

  if(((UINT64)BufferAddress >= BaseAddress) && 
      ((UINT64)BufferAddress < (BaseAddress + QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE))) {

    QuiesceReleaseLock(&DataBufferLock);

    mQuiesceCommData.AvailableBufferLegth = QUIESCE_DATA_SIZE - QUIESCE_STACK_SIZE - sizeof(QUIESCE_DATA_TABLE_ENTRY) - (UINT32) ((EFI_PHYSICAL_ADDRESS)BufferAddress - BaseAddress);

  } else {
    Status = EFI_INVALID_PARAMETER;
  } 

  return Status;

}

VOID
DisableIioPcieMsg(
    UINT8     ProgAll
)
{
  UINT8   i;
  UINT32  Data32;
  UINT32  PciePort = 0;
  UINT32  PciAddress;
  UINT32  BusBase;
  UINT32  Device;
  UINT32  Function;
  UINT32  MySocketId = 0xFF;    //invlaid socket Id

  if(ProgAll == PROGRAM_SELF) {
    MySocketId = GetApicId() >> mSocketLevelShift;
  }

  for(i = 0; i < MAX_SOCKET; i++) {
    if(ProgAll == PROGRAM_SELF) {    //if only program myself
      if(MySocketId != i) continue;
    }

    if(mIioUdsData->PlatformData.IIO_resource[i].Valid) { 
      BusBase = mIioUdsData->PlatformData.IIO_resource[i].BusBase;
      //disable dcaen
      PciAddress = (UINT32)mQuiesceCommData.MMCfgBase | PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_CIPDCASAD);
      Data32 = *(volatile UINT32 *) (UINTN)PciAddress;
      mIIR_CIPDCASAD[i] = Data32;     //save it
      //AptioV Server Override: Cpu Signature check correction.
      if ((((mCpuFamilModel & 0xFFFF0) >> 4) == CPU_FAMILY_HSX) ||
          (((mCpuFamilModel & 0xFFFF0) >> 4) == CPU_FAMILY_BDX_DE) ||
          (((mCpuFamilModel & 0xFFFF0) >> 4) == CPU_FAMILY_BDX)) { 
        Data32 = (UINT32)-1;      
      } else {
      Data32 &= ~B_IIO_CIP_TPH_DCA_EN;  //disable dca
      }
      *(volatile UINT32 *) (UINTN)PciAddress = Data32;

      //Mask VLW CIPINTRC 0:5:0:0x14C bit13:8
      if(BusBase == 0) {  //VLW only come from legacy socket
        PciAddress &= 0xFFFFF000;         //clear offset
        PciAddress |= R_IIO_CIPINTRC;
        Data32  = *(volatile UINT32 *) (UINTN)PciAddress;
        mCsiPIntrVlwData[i] = Data32;
        Data32 |= (0x3F << 8);  //set bit13:8 
        *(volatile UINT32 *) (UINTN)PciAddress = Data32;   
      }

      //disable MSI
      PciAddress = (UINT32)mQuiesceCommData.MMCfgBase | (BusBase << 20) | R_PCIE_PORT_MSIMSK;

      for(Device = 0; Device <= 3; Device++) {  //IIO Root Port devices
        if((Device == 1) || ((BusBase == 0) && (Device ==0))) {  //Platform doesn't have D1 or DMI
          continue; 
        }
        PciAddress |= (Device << 15); 
        for(Function = 0; Function < 4; Function++) {
          PCIEDEVFUNC2PORTINDEX(PciePort, Device, Function);
          PciAddress |= (Function << 12) ;
          //save MsiMsk 
          Data32 = *(volatile UINT32 *) (UINTN)PciAddress;
          mPcieMsiMskData[i][PciePort] = Data32;
          Data32 |= 0x3;  //mask MSI
          *(volatile UINT32 *) (UINTN)PciAddress = Data32;
          PciAddress &= 0xFFFF8FFF; //clear F bits
        } // end of for Function
        PciAddress &= 0xFFF00FFF;   //clear D:F bits
      }  // end of for Device
      PciAddress &= 0xFFF00000;   //clear D:F:R bits
    } // end of if IioList
  } //end of for Iio

  return;
}

VOID
RestoreIioPcieMsg(
    UINT8   ProgAll
)
{
  UINT8   i;
  UINT32  PciePort = 0;
  UINT32  PciAddress;
  UINT32  BusBase;
  UINT32  Device;
  UINT32  Function;
  UINT32  MySocketId = 0xFF;    //invliad socket Id
  UINT32  Data32;

  if(ProgAll == PROGRAM_SELF) {
    MySocketId = GetApicId() >> mSocketLevelShift;
  }
  //restore PCIE port Msi/VLW settings
  for(i = 0; i < MAX_SOCKET; i++) {
    if(ProgAll == PROGRAM_SELF) {    //if only program myself
      if(MySocketId != i) continue;
    }

    if(mIioUdsData->PlatformData.IIO_resource[i].Valid) { 
      BusBase = mIioUdsData->PlatformData.IIO_resource[i].BusBase;
      //restore dcaen
      PciAddress = (UINT32)mQuiesceCommData.MMCfgBase | PCI_PCIE_ADDR(BusBase, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_CIPDCASAD);    
      Data32 = *(volatile UINT32 *) (UINTN)PciAddress;    
      Data32 &= ~B_IIO_CIP_TPH_DCA_EN;  //disable dca
      Data32 |= ( mIIR_CIPDCASAD[i] & B_IIO_CIP_TPH_DCA_EN);   //restore back from buffer
      *(volatile UINT32 *) (UINTN)PciAddress = Data32;

      // restore VLW CIPINTRC 0:5:0:0x14C bit13:8
      if(BusBase == 0) {  //VLW only come from legacy socket
        PciAddress &= 0xFFFFF000;   // clear D:F:R fields
        PciAddress |= R_IIO_CIPINTRC;
        *(volatile UINT32 *) (UINTN)PciAddress = mCsiPIntrVlwData[i];  
      }

      //restore MSI
      PciAddress = (UINT32)mQuiesceCommData.MMCfgBase | (BusBase << 20) | R_PCIE_PORT_MSIMSK;

      for(Device = 0; Device <= 3; Device++) {  //Iio Root Port Devices
        if((Device == 1) || ((BusBase == 0) && (Device ==0))) {  //Platform doesn't have D1 or DMI
          continue; 
        }
        PciAddress |= (Device << 15);

        for(Function = 0; Function < 4; Function++) {
          PCIEDEVFUNC2PORTINDEX(PciePort, Device, Function);
          PciAddress |= (Function << 12);
          *(volatile UINT32 *) (UINTN)PciAddress = mPcieMsiMskData[i][PciePort];
          PciAddress &= 0xFFFF8FFF;   //clear F bits
        } //end of for Function
        PciAddress &= 0xFFF00FFF; //clear D:F bits
      }  //end of for Device
    } //end of if IioList
  }  //end of for Iio

  return;
}

EFI_STATUS
EFIAPI
MonarchAPCheckIn (
    IN BOOLEAN  CheckIn
)
{
  mQuiesceCommData.MonarchApCheckInStatus = CheckIn;
  return EFI_SUCCESS;
}



//Private functions below!!!!!!!
/**********************************
 * This routine prepare page table
 * to use in QuiesceRunOutOfCache.
 ***********************************/
VOID
PreparePageTable(
    IN EFI_PHYSICAL_ADDRESS QuiesceBase
)
{

  UINT64                Cr3;
  EFI_PHYSICAL_ADDRESS  PageTableBase;
  UINT64                *OldPageTable;
  UINT64                *NewPageTable;
  UINT64                Quiesce2MPageEntry;
  UINT64                SMM4KPageEntry;
  UINT64                Value;
  UINT32                i;

  i = 0;
  Quiesce2MPageEntry = 0;
  SMM4KPageEntry = 0;

  Cr3 = ReadCr3();

  OldPageTable = (UINT64 *)(Cr3 + 10 * PAGE_SIZE_4K);

  // point to Quiesce Page meory base
  // page area is 64K away from Code base 
  PageTableBase = QuiesceBase + QUIESCE_CACHE_CODE_SIZE + 4*1024;
  NewPageTable = (UINT64 *) PageTableBase;
  // setup page-directory pointer
  *NewPageTable = (PageTableBase + 
      QUIESCE_PAGE_DIRECTORY_SIIZE) | IA32_PG_P;

  // setup page-directory entries
  NewPageTable += PAGE_SIZE_4K / sizeof(UINT64);
  *NewPageTable = (PageTableBase + 
      QUIESCE_PAGE_DIRECTORY_SIIZE +
      QUIESCE_PAGE_TABLE_SIZE) | IA32_PG_P;

  for(i = 0; i < 4; i++) {
    *(NewPageTable + i) = (PageTableBase + 
        QUIESCE_PAGE_DIRECTORY_SIIZE +
        QUIESCE_PAGE_TABLE_SIZE +
        i * QUIESCE_PAGE_TABLE_SIZE) | IA32_PG_P;
  }

  // setup page tables
  NewPageTable += PAGE_SIZE_4K / sizeof(UINT64);

  // get old page table
  CopyMem((VOID *)NewPageTable,
      OldPageTable, 4*PAGE_SIZE_4K);
  // modify for quiesce 
  NewPageTable--;

  for(i = 0; i < 4*PAGE_SIZE_4K / sizeof(UINT64); i++) {
    NewPageTable++;
    Value = *NewPageTable;

    if((Value & PAGE_TABLE_SIZE_4K) == 0) { //there is 4K page under it
      SMM4KPageEntry = (UINT64)NewPageTable;
    }

    if((QuiesceBase >= (Value & PAGE_TABLE_ENTRY_MASK)) &&
        (QuiesceBase < ((Value & PAGE_TABLE_ENTRY_MASK) + 0x200000))) {
      Value &= ~(IA32_PG_CD | IA32_PG_WT);  // make sure Quiesce region is WB
      Quiesce2MPageEntry = (UINT64)NewPageTable;      // save Quiesce Page Tbale entry address
    } else {
      Value |= IA32_PG_CD;      // make it UC

    }
    *NewPageTable = Value;
  }

  if (Quiesce2MPageEntry != 0) { // AptioV Server Override: Checking the condition so that memory corruption doesn't happen in case Quiesce2MPageEntry is zero
      // now setup 4K Quiesce page tables
      Setup4KPageTable(QuiesceBase, Quiesce2MPageEntry, TRUE);
  }								// AptioV Server Override: Checking the condition so that memory corruption doesn't happen in case Quiesce2MPageEntry is zero

  //if Quiesce area not fall into same 2M of SMM 4K page covered area
  if((SMM4KPageEntry != 0) && (Quiesce2MPageEntry != SMM4KPageEntry)) {
    Setup4KPageTable(QuiesceBase, SMM4KPageEntry, FALSE);
  }

}

VOID
Setup4KPageTable(
    IN EFI_PHYSICAL_ADDRESS QuiesceBase,
    IN EFI_PHYSICAL_ADDRESS PageEntry2M,
    IN BOOLEAN  IsQuiescePage
)
{
  UINT32  Index;
  UINT64  *PageEntry4K;
  UINT64  PageEntryVal;

  if(IsQuiescePage) {
    PageEntryVal = QuiesceBase & 0xFFE00000; //make sure starts from 2M base
    //Quiesce 4K page starts from enad of 2M pages, total 6 2M pages
    PageEntry4K = (UINT64 *) (QuiesceBase + 0x11000 + 0x6000);
    // change 2M entry point to 4K page entry
    *(volatile UINT64 *)PageEntry2M = (QuiesceBase + 0x11000 + 0x6000 + 1);

    //make Quiesce Code, page, and data area (192K) WB
    for(Index = 0; Index < 0x1000; Index += 8) {
      *(volatile UINT64 *)PageEntry4K = PageEntryVal + 0xE3; //WB
      if(((UINT64) PageEntryVal < QuiesceBase) || 
          ((UINT64) PageEntryVal >= (QuiesceBase + 0x30000))) { //if outside Quiesce
        *(volatile UINT64 *)PageEntry4K |= IA32_PG_CD;      //make it UC
      }
      PageEntry4K++;
      PageEntryVal += 0x1000;    
    }
  } else { 
    PageEntryVal = *(UINT64 *)PageEntry2M & 0xFFFFF000; //make sure starts from 2M base
    //copy SMM 4K page table after Quiesce 4K page table
    PageEntry4K = (UINT64 *) (QuiesceBase + 0x11000 + 0x7000);
    CopyMem((VOID *)PageEntry4K, 
        (VOID *)PageEntryVal,
        0x1000);
    //make them UC
    for(Index = 0; Index < 0x1000; Index += 8) {
      *(volatile UINT64 *)PageEntry4K++ |= IA32_PG_CD;
    }
    // change 2M entry point to new 4K page entry
    *(volatile UINT64 *)PageEntry2M = (QuiesceBase + 0x11000 + 0x7000 + 1);
  }  
}

