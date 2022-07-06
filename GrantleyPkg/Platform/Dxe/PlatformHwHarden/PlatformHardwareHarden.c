/**

Copyright (c)  1999 - 2022 Intel Corporation. All rights
reserved This software and associated documentation (if any) is 
furnished under a license and may only be used or copied in 
accordance with the terms of the license. Except as permitted by 
such license, no part of this software or documentation may be 
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file PlatformHardwareHarden.c

**/

#include "PlatformHardwareHarden.h"
#include <Setup/IioUniversalData.h>
#include <Library/DxeServicesTableLib.h>

#define MmPciAddressl(Bus, Device, Function, Register) \
      ( \
        (UINTN) PcdGet64(PcdPciExpressBaseAddress) + (UINTN) (Bus << 20) + (UINTN) (Device << 15) + (UINTN) \
          (Function << 12) + Register \
      )


EFI_HHT_TABLE                   mHardwareHardeningTable;
UINTN                           mHardwareResource;
UINT8                           mIndirectDeviceList[MAX_INDIRECT_DEVICE_COUNT] = 
                                  {
                                    0x44, 0x28
                                  };
EFI_HANDLE                      mImageHandle;
EFI_MP_SERVICES_PROTOCOL        *mMpServices;
PHH_MSR_REGISTER_STRUCT         *mPhhMsrRegStruct;
UINT32                          mSmbusIoBase;
UINT64                          mSmbusRegBase;
UINT64                          mSmbusMmioBase;


//seeing asm interface call using regs not stack but asm code expects stack.
//thus need to turn optimize off so c compiler continues to use staxck for the parameters
//#pragma optimize("", off)


  VOID
PlatformHardwareHardenSetResources (
  IN UINT64     *Buffer
)
{
  UINT32     MsrIndex;
  UINT64     *MsrMailBox = (UINT64 *)NULL;


  MsrIndex = (UINT32)(EFI_MSR_PHH_VIRTUAL_MSR);
  MsrMailBox =  (UINT64 *)mPhhMsrRegStruct;
  AsmWriteMsr64 (MsrIndex, (UINT64)MsrMailBox);

  return;

}

BOOLEAN
CheckuCodePatch(
  VOID
)
{
  UINT64    MsrData;
  MsrData = AsmReadMsr64(EFI_PLATFORM_INFORMATION);
  if (MsrData & BIT63) {
    return TRUE;
  }
  return FALSE;
}

VOID
ProgramGenProtRangeRegsl(
  UINTN         BaseAddr,
  UINTN         Length
  )
/*++

Routine Description:

  This function will program the legacy range and RCBA range into the
  General Protected Range registers.

Arguments:

  VOID

Returns:

  VOID

--*/
{

  UINT32              Data32;
  UINT32              Value32;
  UINTN               PciAddress;
  UINT8               BusNum = 0;
  
  DEBUG ((EFI_D_ERROR, "\nProgramming ProgramGenProtRangeRegsl for Hardware Harden Start\n"));

  while(1) {

    //
    // Cover SMBUS MMIO region with GenProtRange register
    //
    DEBUG ((EFI_D_ERROR, "ProgramGenProtRangeRegsl BaseAddr:0x%x Bus=%x\n",BaseAddr,BusNum));
    Data32 = (UINT32)(BaseAddr & 0xFFFF0000); // Base address of region
    PciAddress = MmPciAddressl(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE0Base);
    *(UINT32*)PciAddress = Data32;
    DEBUG ((EFI_D_ERROR, "ProgramGenProtRangeRegsl Data32:0x%x\n",Data32));
    PciAddress = MmPciAddressl(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE0Base_1);
    *(UINT32*)PciAddress = 00;


    Data32 = (UINT32)(BaseAddr & 0xFFFF0000); // Base address of region
    Data32 = (Data32 + (UINT32)Length + 0xffff) & 0xffff0000; 	// Limit address of PAM region [31:16], [15:0] assumed one
    PciAddress = MmPciAddressl(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE0Limit);
    *(UINT32*)PciAddress = Data32;
    DEBUG ((EFI_D_ERROR, "ProgramGenProtRangeRegsl Length_Data32:0x%x\n",Data32));
    PciAddress = MmPciAddressl(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_GENPROTRANGE0Limit_1);
    *(UINT32*)PciAddress = 00;

    
    //
    // Find out Bus# of next IIO, Uncore Bus# + 1
    //
    PciAddress = MmPciAddressl(BusNum, IIO_VTD_DEV, IIO_VTD_FUN, R_IIO_CPUBUSNO);
    Value32 = *(UINT32*)PciAddress;
    
    BusNum = ((UINT8)((Value32 >> 8) & 0x0ff)) + 1;
    
    //
    // If we have reached the end of the sockets, exit loop
    //
    if (BusNum == 0)
      break;
  }
  DEBUG ((EFI_D_ERROR, "Programming ProgramGenProtRangeRegsl for Hardware Harden Stop\n"));

}


EFI_STATUS
SetResources (
  VOID
) 
{
  
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            ShadowBufferAddress;
  EFI_PHYSICAL_ADDRESS            MsrMbox;
  EFI_PHYSICAL_ADDRESS            MsrMbox64ByteAligned;
  EFI_PHYSICAL_ADDRESS            SmbusMmioBaseAddress;
  UINTN                           Pages;
  UINT32                          Temp1;
  UINT32                          Temp2;
  UINT64                          SmbusIoBaseAllocated;
  EFI_IIO_UDS_PROTOCOL            *IioUds;

  DEBUG ((EFI_D_ERROR, "Executing Set Services for Hardware Harden\n"));

  // Allocate 4KB memory
  ShadowBufferAddress = 0xFEAFFFFF; // less or equal address for the space allocated.
  Pages = EFI_SIZE_TO_PAGES(SIZE_4KB);
  Status = gBS->AllocatePages(
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  Pages,
                  &ShadowBufferAddress
               );
  DEBUG ((EFI_D_ERROR, "The shadow Buffer address is 0x%lx\n", ShadowBufferAddress));
  if (EFI_ERROR (Status)) {
    ReportStatusCode (
                      EFI_ERROR_CODE + EFI_ERROR_MAJOR,
                      EFI_SOFTWARE + EFI_SOFTWARE_DXE_BS_DRIVER + EFI_SW_DS_PC_ALLOCATE_MEMORY_SPACE
    );
    DEBUG ((EFI_D_ERROR, "gDS->AllocatePages ShadowBufferAddress Status = %r\n", Status));
    return Status;
  }

  /*
    Allocate NON-System-RAM space to be used for MMIO mapping of 32 bytes for 
    SMBUS host controller registers.
    
    To do this need to utilize something beyond the std DRAM buffer/memory allocation
    interfaces like AllocatePages.
    This actually requires allocation of memory mapped io space via DxeServices
    gDS->AllocateMemorySpace which creates and add a GCD (Global Coherency Domain) 
    descriptor to the MemoryMap database in the UEFI FW
      -->Also see MdeModulePkg\Core\Dxe\Gcd\Gcd.c for specifics.
    Once we have this MMIO range added we can then allocate a 4KBpage for use 
    by this code via gBS->AllocatePages for a fixed area we setup for this purpose.
    
    Need this MMIO space 4KB aligned (even though SMBUS BAR 0x10 and 0x14 
    indicated 256 byte alignmnent).  Need 4KB alignment for ucode.
    
    Also want this space under 3GByte address for ease of addressing.
    
    Need to allocate this space so it is not used by other code in UEFI FW for
    other pci devices during pci enumeration.
    
    Due to 4Kbyte alignment will provide an entire 4KB size for this region.
 */

  // Locate the IIO Protocol Interface
  DEBUG ((EFI_D_ERROR, "Locating IIO Protocol\n"));
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &IioUds);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((EFI_D_ERROR, "CPU0 mem range Base:0x%x    Limit:0x%x, \n",
  IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceMem32Base, IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceMem32Limit));
  DEBUG ((EFI_D_ERROR, "CPU0 io  range Base:0x%x    Limit:0x%x, \n",
  IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoBase, IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoLimit));


  // SmbusMmioBaseAddress = SMBUS_MMIO_BAR_MAX;
  SmbusMmioBaseAddress = IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceMem32Limit;
  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  ALIGN_4KB_POWER_OF_2,
                  SIZE_4KB,
                  &SmbusMmioBaseAddress,
                  mImageHandle,
                  NULL
                );
  DEBUG ((EFI_D_ERROR, "gDS->AllocateMemorySpace SmbusMmioBases is 0x%x, Status = %r\n",
          SmbusMmioBaseAddress, Status));
  if (EFI_ERROR (Status) || (SmbusMmioBaseAddress < IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceMem32Base)) {
    ReportStatusCode (
                      EFI_ERROR_CODE + EFI_ERROR_MAJOR,
                      EFI_SOFTWARE + EFI_SOFTWARE_DXE_BS_DRIVER + EFI_SW_DS_PC_ALLOCATE_MEMORY_SPACE
    );
    return Status;
  }

  mSmbusMmioBase = (UINT32) SmbusMmioBaseAddress;

  //
  // Allocate io in cpu0 allocation range
  //
  SmbusIoBaseAllocated = IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoLimit;
  Status = gDS->AllocateIoSpace (
                  EfiGcdAllocateMaxAddressSearchTopDown,
                  EfiGcdIoTypeIo,
                  ALIGN_32B_POWER_OF_2,
                  SIZE_32B,
                  &SmbusIoBaseAllocated,
                  mImageHandle,
                  NULL
                  );
  DEBUG ((EFI_D_ERROR, "gDS->AllocateIoSpace SmbusIoBase is 0x%x, Status = %r\n",
          SmbusIoBaseAllocated, Status));
  if (EFI_ERROR (Status) || (SmbusIoBaseAllocated < IioUds->IioUdsPtr->PlatformData.IIO_resource[0].PciResourceIoBase)){
    ReportStatusCode (
                      EFI_ERROR_CODE + EFI_ERROR_MAJOR,
                      EFI_SOFTWARE + EFI_SOFTWARE_DXE_BS_DRIVER + EFI_SW_DS_PC_ALLOCATE_IO_SPACE
    );
    return Status;
  }

  mSmbusIoBase = (UINT32)SmbusIoBaseAllocated;

  // Allocate 1KB Buffer.
  // 64 bytes to pass resource info align @ 64 Byte/Cache Line Boundary
  Status = gBS->AllocatePool (
                  EfiReservedMemoryType,
                  SIZE_1KB,
                  (VOID **)&MsrMbox
               );
  if (EFI_ERROR (Status)) {
    ReportStatusCode (
                      EFI_ERROR_CODE + EFI_ERROR_MAJOR,
                      EFI_SOFTWARE + EFI_SOFTWARE_DXE_BS_DRIVER + EFI_SW_DS_PC_ALLOCATE_MEMORY_SPACE
    );
    DEBUG ((EFI_D_ERROR, "gDS->AllocatePool MsrMbox Status = %r\n", Status));
    return Status;
  }

  ZeroMem ((VOID **)MsrMbox, SIZE_1KB);
  if (MsrMbox & (ALIGN_64_BYTES - 1)) {
    MsrMbox64ByteAligned = (MsrMbox + ALIGN_64_BYTES) & ~(ALIGN_64_BYTES - 1);
  } else {
    MsrMbox64ByteAligned = MsrMbox;
  }
  DEBUG ((EFI_D_ERROR, "1K Byte RSVD Pool start address 0x%lx\n", MsrMbox));
  DEBUG ((EFI_D_ERROR, "64 Byte MSR 0xD0 mailbox address, 64 Byte Aligned 0x%lx\n",
          MsrMbox64ByteAligned));

  mPhhMsrRegStruct = (PHH_MSR_REGISTER_STRUCT*)(MsrMbox64ByteAligned);
  DEBUG ((EFI_D_ERROR, "PhhMsrRegStruct  0x%lx\n", mPhhMsrRegStruct));

  /* 
    Below vars for IndirectDevicesListStatusLo & Hi have a total of 128 bits.
    This allows us to use a bitmap with bit position 0-127 to refer to each 
      of the total 128 bit permutations for a 7bit smbus address.
    For instance, the 8 bit smbus address on neon city/purley for the CKMNG
      chip (ie IDT932SQ420D - ck420 compatible clk generatior/chip).  It is
      configured to have an 8 bit smbus address of 0xD0 (writes bit0 =0) and 
      0xD1 (reads bit 0 = 1).
    Thus 8bit smbus = 0xd0=1101 0000)b 
    To get 7bit address SHR by 1 bit to remove write/read bit and pad msbit to
      0.
    Thus 7bit smbus = 0110 1000)b = 0x68
    To find out which bit in bitmap representes this address of 0x68 conver to
      decimal where:
      7bit addr = 0x68 = 104)d 
    Thus if set bit 104 (zero based bit addressing) will indicate to block read
    and write access to this smbus device.  if leave bit 104 = 0 allows
    transactions to go to device/target.
      -->note that with a high and a low 64 bit value, bit 104)b is in high 64
         bit value and is in below group 107-104.
    
      BitMap Bit Position Details for Reference:
      
     IndirectDevicesListStatusLo-LOW 64 bits:
       4 bitgrp  63-60 59-56 55-52 51-48 47-44 43-40 39-36 35-32 31-28 27-24 23-20 19-16 15-12 11-08 07-04 03-00
       hex val     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0     0
    
      IndirectDevicesListStatusHi-HIGH 64 bits:
       4 bitgrp  127-124 123-120 119-116 115-112 111-108 107-104 103-100 99-96 95-92 91-88 87-84 83-80 79-76 75-72 71-68 67-64
       hex val      0       0       0       0       0       0       0      0     0     0     0     0     0     0     0     0
    
     Where the following value would be set for the 2 64bit varaibles for bit 104)d as:
       mPhhMsrRegStruct->IndirectDevicesListStatusLo = 0x0000 0000 0000 0000;
       mPhhMsrRegStruct->IndirectDevicesListStatusHi = 0x0000 0100 0000 0000;
  */


  // Mayan city value:
  //7-bit addrs
  // 0X29 = 41 D
  // 0X2A = 42 D
  // 0X2D = 45 D
  // 0X2F = 47 D
  // 0X31 = 49 D
  // 0X33 = 51 D
  // 0X35 = 53 D
  // 0X37 = 55 D


  mPhhMsrRegStruct->IndirectDevicesListStatusLo = PcdGet64 (PcdOemSkuPHHDIndirectDevicesLo);
  mPhhMsrRegStruct->IndirectDevicesListStatusHi = PcdGet64 (PcdOemSkuPHHDIndirectDevicesHi);

  DEBUG ((EFI_D_ERROR, "Device List Lo  0x%llx ; Hi :0x%llx\n", mPhhMsrRegStruct->IndirectDevicesListStatusLo, mPhhMsrRegStruct->IndirectDevicesListStatusHi));

  mPhhMsrRegStruct->PchSmbusMmCfgBaseAddress = mSmbusRegBase;
  mPhhMsrRegStruct->PchSmbusMmioBAR = SmbusMmioBaseAddress;
  mPhhMsrRegStruct->PchSmbusIoBar = mSmbusIoBase;
  mPhhMsrRegStruct->ShadowBufferAddress = ShadowBufferAddress;

  // Program lower 32 bits of Mmio bar register
  MmioWrite32 (MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + 0x10,
                     (UINT32)SmbusMmioBaseAddress);
  // Program upper 32 bits of Mmio bar register
  MmioWrite32 (MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + 0x14,
                     (UINT32)(SmbusMmioBaseAddress >> 32));
  Temp1 = MmioRead32 (
                     MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + 0x10);
  Temp2 = MmioRead32 (
                     MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + 0x14);
  DEBUG ((EFI_D_ERROR, "SMBUS MMIO Bar - Temp2 : Temp1  0x%lx : 0x%lx\n", Temp2, Temp1));

  //
  // Program io bar register
  //
  MmioWrite32 (MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + R_PCH_SMBUS_BASE,
                     (UINT32)SmbusIoBaseAllocated);
  Temp1 = MmioRead32 (
                     MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_SMBUS,
                     PCI_FUNCTION_NUMBER_PCH_SMBUS)
                     + R_PCH_SMBUS_BASE) &  B_PCH_SMBUS_BASE_BAR;
  DEBUG ((EFI_D_ERROR, "SMBUS IO Bar - 0x%lx\n", Temp1));

  // Program PchSmbus Command register
  MmioOr8 (mSmbusRegBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_IO_SPACE | EFI_PCI_COMMAND_MEMORY_SPACE);

  //Enable the Smbus Controller
  MmioOr8 (mSmbusRegBase + R_PCH_SMBUS_HOSTC, B_PCH_SMBUS_HOSTC_HST_EN);

  Status = mMpServices->StartupAllAPs (
                 mMpServices,
                 PlatformHardwareHardenSetResources,
                 FALSE,
                 NULL,
                 0, 
                 NULL,
                 NULL
                 );
  ASSERT_EFI_ERROR (Status);

  PlatformHardwareHardenSetResources(NULL);

  ProgramGenProtRangeRegsl(SmbusMmioBaseAddress, SIZE_4KB);


  /* 
    Need to wait until after WRMSR before hide else ucode cannot see SMB controller.
   
    Hide Pch Smbus controller from later pci enum code  will unhide at event 
    created at driver entry time (this driver entry) for 
    gEfiPciEnumerationCompleteProtocolGuid
  */
  //MmioWrite32 (PCH_PCR_ADDRESS (PID_PSF3, R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE + 
  //                             R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS),
  //           (UINT32) B_PCH_PCR_PSFX_T0_SHDW_CFG_DIS_CFGDIS
  //            );
  return EFI_SUCCESS;
}

VOID
EFIAPI
LocateMpServiceProtocol (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS Status;
  // UINT32     PadCfgLockTxStatus;
 
  DEBUG ((EFI_D_ERROR, "Locating MpService Protocol\n"));
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&mMpServices);
  ASSERT_EFI_ERROR (Status);

  SetResources ();

  // GpioGetPadCfgLockTx (mHardwareHardeningTable.GpioPadNumberToTxLock, &PadCfgLockTxStatus);
  // CPLD GPIO 
  // DEBUG ((EFI_D_ERROR, "The status of PadCfgLockTxStatus before lock for %x is %x\n", mHardwareHardeningTable.GpioPadNumberToTxLock, PadCfgLockTxStatus));

  // Do the Tx Lock for the GPIO Pin connected to CPLD
  // GpioLockPadCfgTx (mHardwareHardeningTable.GpioPadNumberToTxLock);

  // GpioGetPadCfgLockTx (mHardwareHardeningTable.GpioPadNumberToTxLock, &PadCfgLockTxStatus);
  // DEBUG ((EFI_D_ERROR, "The status of PadCfgLockTxStatus after lock for %x is %x\n", mHardwareHardeningTable.GpioPadNumberToTxLock, PadCfgLockTxStatus));
     
  return;
}


VOID
EFIAPI
UnHidePchSmbusAfterEnumeration (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  
  // UINT64                Temp1, Temp2;
  // UINT32                PchPcr;

  // DEBUG ((EFI_D_ERROR, "Calling UnHidePchSmbusAfterEnumeration after Pcie Enumeration \n"));
 
  //unhide PCH SMBUS contrller
  // MmioWrite32 (PCH_PCR_ADDRESS (PID_PSF3, R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE + 
  //                              R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS),
  //            (UINT32) 0
  //            );

  // DEBUG ((EFI_D_ERROR, "Writing is done.  Do the read again\n"));


  // PchPcr = MmioRead32 (PCH_PCR_ADDRESS (PID_PSF3, R_PCH_PCR_PSF3_T0_SHDW_SMBUS_REG_BASE + 
  //                               R_PCH_PCR_PSFX_T0_SHDW_CFG_DIS));

  // DEBUG ((EFI_D_ERROR, "Read PchPcr after unhiding Smbus=0x%x\n", PchPcr));

  // mSmbusRegBase = MmPciBase (
  //                    DEFAULT_PCI_BUS_NUMBER_PCH,
  //                    PCI_DEVICE_NUMBER_PCH_SMBUS,
  //                    PCI_FUNCTION_NUMBER_PCH_SMBUS);
  
  // mSmbusIoBase = MmioRead32 (
  //                    MmPciBase (
  //                    DEFAULT_PCI_BUS_NUMBER_PCH,
  //                    PCI_DEVICE_NUMBER_PCH_SMBUS,
  //                    PCI_FUNCTION_NUMBER_PCH_SMBUS)
  //                    + R_PCH_SMBUS_BASE) & B_PCH_SMBUS_BASE_BAR;

  // Temp1 = MmioRead32 (
  //                    MmPciBase (
  //                    DEFAULT_PCI_BUS_NUMBER_PCH,
  //                    PCI_DEVICE_NUMBER_PCH_SMBUS,
  //                    PCI_FUNCTION_NUMBER_PCH_SMBUS)
  //                    + 0x10);

  // Temp2 = MmioRead32 (
  //                    MmPciBase (
  //                    DEFAULT_PCI_BUS_NUMBER_PCH,
  //                    PCI_DEVICE_NUMBER_PCH_SMBUS,
  //                    PCI_FUNCTION_NUMBER_PCH_SMBUS)
  //                    + 0x14);

  // DEBUG ((EFI_D_ERROR, "After Unhiding SmbusIoBase=0x%x\n", mSmbusIoBase));
  // DEBUG ((EFI_D_ERROR, "SmbusRegBase=0x%x\n", mSmbusRegBase));
  // DEBUG ((EFI_D_ERROR, "Smbus Mmio Bar Lo - Temp1=0x%x\n", Temp1));
  // DEBUG ((EFI_D_ERROR, "Smbus Mmio Bar Hi - Temp2=0x%x\n", Temp2));

  // mSmbusMmioBase = ((UINT64)Temp2 << 32) | (Temp1 & 0x100);
  // DEBUG ((EFI_D_ERROR, "mSmbusMmioBase=0x%x\n", mSmbusMmioBase));
 
  return;
}



VOID
UpdateHht (
  VOID
) 
{

  UINT8  Index;


  mHardwareHardeningTable.PcieDeviceIoRegAddress = mSmbusIoBase;

  // Get the MMIOReg Address for PCH Smbus controller.  Initialize it to '0' for now
  mHardwareHardeningTable.PcieDeviceMmioRegAddress = 0x0;

  // Get the MMCFG address Base for PCH SMbus controller.
  mHardwareHardeningTable.PcieDeviceMmCfgBase = mSmbusRegBase;

  // Update Indirect Device List
  mHardwareHardeningTable.IndirectDeviceAddressCount = MAX_INDIRECT_DEVICE_COUNT;
  for (Index = 0;  Index < MAX_INDIRECT_DEVICE_COUNT; Index++) {
      mHardwareHardeningTable.IndirectDeviceAddress[Index] = mIndirectDeviceList[Index];
  }

  // GPIO Pad number for TxLock to prevent accessing CPLD.
  // mHardwareHardeningTable.GpioPadNumberToTxLock = GPIO_SKL_H_GPP_F6;
  return;
}

EFI_STATUS
EFIAPI
PlatformHardwareHardenEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
/**

  This is the main entry point of the Platform Hardware Harden Module.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Module launched successfully.

**/
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  EFI_EVENT                         Event1, Event2;
  VOID                              *EventRegistration1, *EventRegistration2;
  UINT32                            Temp1, Temp2;
  EFI_MP_SERVICES_PROTOCOL          *MpProtocolLocal;
  VOID                              *Interface;

  DEBUG ((EFI_D_ERROR, "Executing Platform Hardware Harden Driver\n"));

  //check valid indirect device list
  if (PcdGet64 (PcdOemSkuPHHDIndirectDevicesLo) == 0 && PcdGet64 (PcdOemSkuPHHDIndirectDevicesHi) == 0) {
    DEBUG ((EFI_D_ERROR, "PHHD device list not configured\n"));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((EFI_D_ERROR, "Printing the sku for PHHD: 0x%llx Hi: 0x%llx\n",  PcdGet64(PcdOemSkuPHHDIndirectDevicesLo), PcdGet64(PcdOemSkuPHHDIndirectDevicesHi)));

  //check ucode patch support or not
  if (!CheckuCodePatch()) {
    DEBUG ((EFI_D_ERROR, "Ucode patch support for PHHD not available\n"));
    return EFI_UNSUPPORTED;
  }
  mImageHandle = ImageHandle;
  ZeroMem (&mHardwareHardeningTable, sizeof (EFI_HHT_TABLE));
  UpdateHht();

  DEBUG ((EFI_D_ERROR, "Registering Protocol Notify for MpServiceProtocol\n"));
  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &MpProtocolLocal);
  if (Status == EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, " MpProtocol not found create callback!!!\n"));
    Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  LocateMpServiceProtocol,
                  NULL,
                  &Event1
                  );
  Status = gBS->RegisterProtocolNotify (
                  &gEfiMpServiceProtocolGuid,
                  Event1,
                  &EventRegistration1
                  );
  }
  else{
    LocateMpServiceProtocol (NULL, NULL);
  }

  //
  // create event to unhide after bus enumeration or just unhide if done
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );

  if (Status == EFI_NOT_FOUND) {
    DEBUG ((EFI_D_ERROR, "locate gEfiPciEnumerationCompleteProtocolGuid EFI_NOT_FOUND \n"));
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    UnHidePchSmbusAfterEnumeration,
                    NULL,
                    &Event2
                    );
    Status = gBS->RegisterProtocolNotify (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  Event2,
                  &EventRegistration2
                  );
  }
  else{
    UnHidePchSmbusAfterEnumeration (NULL, NULL);
  }

  mSmbusRegBase = MmPciBase (
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SMBUS,
                  PCI_FUNCTION_NUMBER_PCH_SMBUS);
  mSmbusIoBase = MmioRead32 (
                  MmPciBase (
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SMBUS,
                  PCI_FUNCTION_NUMBER_PCH_SMBUS)
                  + R_PCH_SMBUS_BASE) & B_PCH_SMBUS_BASE_BAR;

  Temp1 = MmioRead32 (
                  MmPciBase (
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SMBUS,
                  PCI_FUNCTION_NUMBER_PCH_SMBUS)
                  + 0x10);
  Temp2 = MmioRead32 (
                  MmPciBase (
                  DEFAULT_PCI_BUS_NUMBER_PCH,
                  PCI_DEVICE_NUMBER_PCH_SMBUS,
                  PCI_FUNCTION_NUMBER_PCH_SMBUS)
                  + 0x14);

  DEBUG ((EFI_D_ERROR, "For SV CAFE== SmbusIoBase=0x%x\n", mSmbusIoBase));
  DEBUG ((EFI_D_ERROR, "SmbusRegBase=0x%x\n", mSmbusRegBase));
  DEBUG ((EFI_D_ERROR, "Smbus Mmio Bar Lo - Temp1=0x%x\n", Temp1));
  DEBUG ((EFI_D_ERROR, "Smbus mmio Bar Hi - Temp2=0x%x\n", Temp2));

    mSmbusMmioBase = ((UINT64)Temp2 << 32) | (Temp1 & 0x1000);

  DEBUG ((EFI_D_ERROR, "PlatfomrHardware Harden Protocol Exit \n"));
  return Status;

}
