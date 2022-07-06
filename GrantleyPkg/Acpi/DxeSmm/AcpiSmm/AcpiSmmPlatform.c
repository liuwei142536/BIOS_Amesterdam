/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  2009 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AcpiSmmPlatform.c
    
Abstract:

  ACPISMM Driver implementation file. 

--*/

#include <AcpiSmmPlatform.h>


#define PCILIB_TO_COMMON_ADDRESS(Address) \
        ((UINT64) ((((UINTN) ((Address>>20) & 0xff)) << 24) + (((UINTN) ((Address>>15) & 0x1f)) << 16) + (((UINTN) ((Address>>12) & 0x07)) << 8) + ((UINTN) (Address & 0xfff ))))

BIOS_ACPI_PARAM   *mAcpiParameter;
EFI_IIO_UDS_PROTOCOL            *mIioUds = NULL;  


//
// Modular variables needed by this driver
//
EFI_ACPI_SMM_DEV                 mAcpiSmm;
SYSTEM_CONFIGURATION             mSystemConfiguration;
CHAR16    mEfiMemoryOverwriteControlVariable[] = MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME;
// AptioV server Override-Start
//CHAR16    mEfiPhysicalPresenceVariable[]       = PHYSICAL_PRESENCE_VARIABLE;
//EFI_GUID  mEfiPhysicalPresenceGuid             = EFI_PHYSICAL_PRESENCE_DATA_GUID;
EFI_GUID  mEfiMemoryOverwriteControlDataGuid   = MEMORY_ONLY_RESET_CONTROL_GUID;
// AptioV server Override-End

STATIC UINT8  mPciCfgRegTable[] = {
  //
  // Logic to decode the table masks to arrive at the registers saved
  // Dword Registers are saved. For a given mask, the Base+offset register
  // will be saved as in the table below.
  // (example) To save register 0x24, 0x28 the mask at the Base 0x20 will be 0x06
  //     Base      0x00   0x20   0x40  0x60  0x80  0xA0  0xC0  0xE0
  // Mask  offset   
  // 0x01   0x00
  // 0x02   0x04
  // 0x04   0x08
  // 0x08   0x0C
  // 0x10   0x10
  // 0x20   0x14
  // 0x40   0x18
  // 0x80   0x1C
  //

  //
  // Bus,   Dev,  Func,
  // 00-1F, 20-3F, 40-5F, 60-7F, 80-9F, A0-BF, C0-DF, E0-FF
  // Only Bus 0 device is supported now
  //

  PCI_DEVICE   (0, 0, 0),					//DMI
  PCI_REG_MASK (0x00, 0x00, 0x10, 0x00, 0x30, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 1, 0),					//PCIE 1
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 1, 1),					//PCIE 2
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 0),					//PCIE 3
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 1),					//PCIE 4
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 2),					//PCIE 5
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 2, 3),					//PCIE 6
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 0),					//PCIE 7
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 1),					//PCIE 8
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 2),					//PCIE 9
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 3, 3),					//PCIE A
  PCI_REG_MASK (0x08, 0x80, 0x00, 0x00, 0x50, 0x46, 0x00, 0x00), 

  PCI_DEVICE   (0, 4, 0),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 1),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 2),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 3),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 4),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 5),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 6),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 4, 7),					//CBDMA
  PCI_REG_MASK (0x3A, 0x80, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 0),					//VT
  PCI_REG_MASK (0x08, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 1),					//Mem Hot
  PCI_REG_MASK (0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 5, 4),					//IOAPIC
  PCI_REG_MASK (0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 4),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 0),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE   (0, 17, 5),					
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save Memory base register of D25F0
  PCI_DEVICE   (0, 25, 0),
  PCI_REG_MASK (0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save PIRQ register of D31F0
  PCI_DEVICE   (0, 31, 0),
  PCI_REG_MASK (0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00),

  // Save CMD register of D31F2
  PCI_DEVICE   (0, 31, 2),
  PCI_REG_MASK (0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  // Save CMD, MEM register of D31F3
  PCI_DEVICE   (0, 31, 3),
  PCI_REG_MASK (0xF2, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00),

  PCI_DEVICE_END
};

// AptioV server Override
/*
EFI_STATUS
SmiPpCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  );
*/
EFI_STATUS
SmiMorCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  );


/**
  Allocate EfiACPIMemoryNVS below 4G memory address.

  This function allocates EfiACPIMemoryNVS below 4G memory address.

  @param Size   Size of memory to allocate.

  @return       Allocated address for output.

**/
VOID*
AllocateAcpiNvsMemoryBelow4G (
  IN UINTN  Size
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;
  VOID*                 Buffer;

  Pages = EFI_SIZE_TO_PAGES (Size);
  Address = 0xffffffff;

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Buffer = (VOID *) (UINTN) Address;
  ZeroMem (Buffer, Size);

  return Buffer;
}

EFI_STATUS
EFIAPI
ReservedS3Memory (
  UINTN  SystemMemoryLength
  
  ) 
/*++

Routine Description:

  Reserved S3 memory for InstallS3Memory

Arguments:


Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.

--*/
{

  VOID                                      *GuidHob;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK            *DescriptorBlock;
  VOID                                      *AcpiReservedBase; 
  
  UINTN                                     TsegIndex;
  UINTN                                     TsegSize;
  UINTN                                     TsegBase;
  RESERVED_ACPI_S3_RANGE                    *AcpiS3Range;
  //
  // Get Hob list for SMRAM desc
  //
  GuidHob    = GetFirstGuidHob (&gEfiSmmPeiSmramMemoryReserveGuid);
  ASSERT (GuidHob);
  DescriptorBlock = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (DescriptorBlock);
    
  //
  // Use the hob to get SMRAM capabilities
  //  
  TsegIndex = DescriptorBlock->NumberOfSmmReservedRegions - 1;
  ASSERT (TsegIndex <= (MAX_SMRAM_RANGES - 1));  
  TsegBase  = DescriptorBlock->Descriptor[TsegIndex].PhysicalStart;
  TsegSize  = DescriptorBlock->Descriptor[TsegIndex].PhysicalSize;
  
  DEBUG ((EFI_D_ERROR, "SMM  Base: %08X\n", TsegBase));
  DEBUG ((EFI_D_ERROR, "SMM  Size: %08X\n", TsegSize));

  //
  // Now find the location of the data structure that is used to store the address
  // of the S3 reserved memory.
  //
  AcpiS3Range = (RESERVED_ACPI_S3_RANGE*) (UINTN) (TsegBase + RESERVED_ACPI_S3_RANGE_OFFSET);

  //
  // Allocate reserved ACPI memory for S3 resume.  Pointer to this region is
  // stored in SMRAM in the first page of TSEG.
  //
  AcpiReservedBase = AllocateAcpiNvsMemoryBelow4G (PcdGet32 (PcdS3AcpiReservedMemorySize));
  if (AcpiReservedBase != NULL) {
    AcpiS3Range->AcpiReservedMemoryBase = (UINT32)(UINTN) AcpiReservedBase;
    AcpiS3Range->AcpiReservedMemorySize = PcdGet32 (PcdS3AcpiReservedMemorySize);
  }
  AcpiS3Range->SystemMemoryLength = (UINT32)SystemMemoryLength;
  
  DEBUG ((EFI_D_ERROR, "S3 Memory  Base:    %08X\n", AcpiS3Range->AcpiReservedMemoryBase));
  DEBUG ((EFI_D_ERROR, "S3 Memory  Size:    %08X\n", AcpiS3Range->AcpiReservedMemorySize));
  DEBUG ((EFI_D_ERROR, "S3 SysMemoryLength: %08X\n", AcpiS3Range->SystemMemoryLength));  

  return EFI_SUCCESS;
}

//
// s5003276: SMBIOS UUID Changes On Every Boot
//
EFI_STATUS
EFIAPI
SavePDRUUID (
  )
{
  EFI_STATUS                    Status;
  EFI_GUID                      *SystemUuid;
  EFI_PLATFORM_INFO             *pPIT;
  EFI_SPI_PROTOCOL              *SpiProtocol;
  EFI_CPU_IO_PROTOCOL           *CpuIo;
  EFI_PLATFORM_TYPE_PROTOCOL    *PlatformType;
  UINT8                         *MemSourcePtr;
  UINT32                        PDRAddress;
  UINT8                         AllSet, NonSet;
  UINT8                         Index;
  
  Status = gBS->LocateProtocol (
                    &gEfiCpuIoProtocolGuid,
                    NULL,
                    &CpuIo
                    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  CpuIo->Mem.Read (
              CpuIo,
              EfiCpuIoWidthUint32,
              (PCH_RCBA_ADDRESS + R_PCH_RCRB_SPI_BASE + 0x64),  // Flash Region 4(Platform Data)(32bits)
              1,
              &PDRAddress
              );
  
  PDRAddress = (PDRAddress & B_PCH_SPI_FREG4_BASE_MASK) << 12;
  
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSpiProtocolGuid,
                  NULL,
                  &SpiProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  MemSourcePtr = NULL;
  
  Status = gBS->AllocatePool( EfiBootServicesData,
                              sizeof(EFI_GUID),
                              &MemSourcePtr
                             );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  ZeroMem (MemSourcePtr, sizeof(EFI_GUID));
  
  pPIT = (EFI_PLATFORM_INFO *)MemSourcePtr;

  //
  // copy PDR data (64KB) to memory buffer
  //
  Status = SpiProtocol->Execute (
                          SpiProtocol,
                          SPI_OPCODE_READ_INDEX,  // OpcodeIndex
                          0,                      // PrefixOpcodeIndex
                          TRUE,                   // DataCycle
                          TRUE,                   // Atomic
                          FALSE,                  // ShiftOut
                          PDRAddress,             // Address, offset from descriptor region
                          sizeof(EFI_GUID),       // Data Number
                          (UINT8 *)MemSourcePtr,
                          EnumSpiRegionAll        // EnumSpiRegionPlatformData
                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error during PDR read\n"));
  }
  
  //
  // Check if there is a proper UUID (improper contains only 0x0 or only 0xFf bytes)
  //
  AllSet = 0xFF;
  NonSet = 0x0;
  for (Index = 0; Index < sizeof(EFI_GUID); Index ++) {
    AllSet &= pPIT->SystemUuid[Index]; //looking for "0"
    NonSet |= pPIT->SystemUuid[Index]; //looking for "1"
  }
  
  if( AllSet == 0xFF || NonSet == 0x0 ){ //means that UUID contains only 0x00 of 0xFF bytes
    //
    // Get System UUID
    //
    Status = gBS->LocateProtocol (
                    &gEfiPlatformTypeProtocolGuid,
                    NULL,
                    &PlatformType
                    );
    SystemUuid = (EFI_GUID *)PlatformType->SystemUuid;
    ASSERT_EFI_ERROR (Status);
    if ((PlatformType->SystemUuid == NULL) || (EFI_ERROR (Status))) {
      gBS->FreePool (MemSourcePtr);
      return Status;
    }
    
    //
    // Save PDR UUID to SPI
    //
    Status = SpiProtocol->Execute (
            SpiProtocol,
            SPI_OPCODE_WRITE_INDEX,         // OpcodeIndex
            0,                              // PrefixOpcodeIndex
            TRUE,                           // DataCycle
            TRUE,                           // Atomic
            TRUE,                           // ShiftOut
            PDRAddress,                     // Address, offset from descriptor region
            sizeof(EFI_GUID),               // Data Number
            (UINT8 *)SystemUuid,
            EnumSpiRegionAll                // EnumSpiRegionPlatformData
            );
    AsmWbinvd ();
    
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "New platform UUID can't be saved to SPI.\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "New Platform UUID (%g) is written to SPI.\n", (EFI_GUID*) SystemUuid));
    }
  }
  
  gBS->FreePool (MemSourcePtr);

  return EFI_SUCCESS;
}
//
// s5003276: SMBIOS UUID Changes On Every Boot
//

EFI_STATUS
EFIAPI
InitAcpiSmmPlatform (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
/*++

Routine Description:

  Initializes the SMM S3 Handler Driver.

Arguments:

  ImageHandle  -  The image handle of Wake On Lan driver.
  SystemTable  -  The starndard EFI system table.
    
Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.
  Other                 -  Error occured during execution. 

--*/
{
  EFI_STATUS                      Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol = NULL;
  UINTN                           MemoryLength;   
  EFI_PEI_HOB_POINTERS            Hob;

  
  // Located the IOH Protocol Interface
  if (mIioUds == NULL){
  Status = gBS->LocateProtocol (
                &gEfiIioUdsProtocolGuid,
                NULL,
                &mIioUds
                );
  ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->LocateProtocol (
                  &gEfiGlobalNvsAreaProtocolGuid, 
                  NULL, 
                  &AcpiNvsProtocol
                  );
  ASSERT_EFI_ERROR (Status);
 

  mAcpiParameter = AcpiNvsProtocol->Area;
  ASSERT (mAcpiParameter);
  
  DEBUG ((DEBUG_ERROR, "TPM ACPI Parameter Block Address: 0x%X\n", mAcpiParameter));
  
    mAcpiSmm.BootScriptSaved  = 0;

    //
  // Calculate the system memory length by memory hobs
  //
  MemoryLength  = 0x100000;
  Hob.Raw = GetFirstHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR);
  ASSERT (Hob.Raw != NULL);
  while ((Hob.Raw != NULL) && (!END_OF_HOB_LIST (Hob))) {
    if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
      //
      // Skip the memory region below 1MB
      //
      if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000) {
        MemoryLength += Hob.ResourceDescriptor->ResourceLength;
      }
    }
    Hob.Raw = GET_NEXT_HOB (Hob);
    Hob.Raw = GetNextHob (EFI_HOB_TYPE_RESOURCE_DESCRIPTOR, Hob.Raw);
  }

  ReservedS3Memory(MemoryLength);

    //
    // Locate and Register to Parent driver
    //
    Status = RegisterToDispatchDriver ();
    ASSERT_EFI_ERROR (Status);

  //
  // s5003276: SMBIOS UUID Changes On Every Boot
  // Save PDR UUID to SPI
  //
  SavePDRUUID();

  return EFI_SUCCESS;
}

EFI_STATUS
RegisterToDispatchDriver (
  VOID
  )
/*++

Routine Description:

  Register to dispatch driver.
  
Arguments:

  None.
      
Returns:

  EFI_SUCCESS  -  Successfully init the device.
  Other        -  Error occured whening calling Dxe lib functions.
  
--*/
{
  UINTN                         Length;
  UINTN                         VarSize;
  EFI_STATUS                    Status;
  EFI_SMM_SX_DISPATCH2_PROTOCOL  *SxDispatch;
  EFI_SMM_SW_DISPATCH2_PROTOCOL  *SwDispatch;
   EFI_SMM_SX_REGISTER_CONTEXT   *EntryDispatchContext;
   EFI_SMM_SX_REGISTER_CONTEXT   *EntryS1DispatchContext;
   EFI_SMM_SX_REGISTER_CONTEXT   *EntryS3DispatchContext;
   EFI_SMM_SX_REGISTER_CONTEXT   *EntryS4DispatchContext;
   EFI_SMM_SX_REGISTER_CONTEXT   *EntryS5DispatchContext;
   EFI_SMM_SW_REGISTER_CONTEXT   *SwContext;
   // AptioV server override start: Use AcpiModeEnable chipset module part
   //EFI_SMM_SW_REGISTER_CONTEXT   *AcpiDisableSwContext;
   //EFI_SMM_SW_REGISTER_CONTEXT   *AcpiEnableSwContext;
   // AptioV server override end: Use AcpiModeEnable chipset module part
   EFI_SMM_SW_REGISTER_CONTEXT   *PpCallbackSwContext;
   EFI_SMM_SW_REGISTER_CONTEXT   *MorCallbackSwContext;

   Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSxDispatch2ProtocolGuid,
                  NULL,
                  &SxDispatch
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gSmst->SmmLocateProtocol(
                  &gEfiSmmSwDispatch2ProtocolGuid,
                  NULL,
                  &SwDispatch
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
// AptioV server override: Changed variable name to Intelsetup
   VarSize = sizeof (mSystemConfiguration);
    Status = gRT->GetVariable (
                    L"IntelSetup",
                    &gEfiSetupVariableGuid,
                    NULL,
                    &VarSize,
                    &mSystemConfiguration
                    );
    if (EFI_ERROR (Status)) {
       return Status;
  }

 // AptioV override start: Commenting below line since no SW register context is registered here. Only SX context is registered. AcpiModeEn/Dis SW SMI registered in
 // AcpiModeEnable chipset module part.
 //Length = sizeof ( EFI_SMM_SX_REGISTER_CONTEXT) * 4 + sizeof ( EFI_SMM_SW_REGISTER_CONTEXT) * 2;
  Length = sizeof ( EFI_SMM_SX_REGISTER_CONTEXT) * 4 ;
  // AptioV override end
  Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      Length,
                      &EntryDispatchContext
                      );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SetMem (EntryDispatchContext, Length, 0);

  EntryS1DispatchContext  = EntryDispatchContext++;
  EntryS3DispatchContext  = EntryDispatchContext++;
  EntryS4DispatchContext  = EntryDispatchContext++;
  EntryS5DispatchContext  = EntryDispatchContext++;

  SwContext = ( EFI_SMM_SW_REGISTER_CONTEXT *)EntryDispatchContext;
  // AptioV server override start: Use AcpiModeEnable chipset module part
  // AcpiDisableSwContext = SwContext++;
  // AcpiEnableSwContext  = SwContext++;
  // AptioV server override end: Use AcpiModeEnable chipset module part

  // AptioV server override start: Registering PP and MOR is removed for PO_WA and allocation is done only for 2 SW register context that is for
  // AcpiModeEn and AcpiModeDis
  PpCallbackSwContext  = SwContext++;
  MorCallbackSwContext = SwContext++;
  // AptioV server override end: Registering PP and MOR is removed for PO_WA and allocation is done only for 2 SW register context that is for
  // AcpiModeEn and AcpiModeDis

  // AptioV server override start: Use AcpiModeEnable chipset module part
  /*
  //
  // Register the enable handler
  //
  AcpiEnableSwContext->SwSmiInputValue = EFI_ACPI_ACPI_ENABLE;
  Status = SwDispatch->Register (
                        SwDispatch,
                        EnableAcpiCallback,
                        AcpiEnableSwContext,
                        &(mAcpiSmm.DisableAcpiHandle)
                        );

  //
  // Register the disable handler
  //
  AcpiDisableSwContext->SwSmiInputValue = EFI_ACPI_ACPI_DISABLE;
  Status = SwDispatch->Register (
                        SwDispatch,
                        DisableAcpiCallback,
                        AcpiDisableSwContext,
                        &(mAcpiSmm.EnableAcpiHandle)
                        );
*/
  Status = GetPchPmBase (gSmst); //AptioV server override: Update mAcpiSmm.PchPmBase
  // AptioV server override end: Use AcpiModeEnable chipset module part



  //
  // Register entry phase call back function for S1
  //
  EntryS1DispatchContext->Type  = SxS1;
  EntryS1DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                        SxDispatch,
                        SxSleepEntryCallBack,
                        EntryS1DispatchContext,
                        &(mAcpiSmm.S1SleepEntryHandle)
                        );

  //
  // Register entry phase call back function
  //
  EntryS3DispatchContext->Type  = SxS3;
  EntryS3DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                        SxDispatch,
                        SxSleepEntryCallBack,
                        EntryS3DispatchContext,
                        &(mAcpiSmm.S3SleepEntryHandle)
                        );

  //
  // Register entry phase call back function for S4
  //
  EntryS4DispatchContext->Type  = SxS4;
  EntryS4DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                        SxDispatch,
                        SxSleepEntryCallBack,
                        EntryS4DispatchContext,
                        &(mAcpiSmm.S4SleepEntryHandle)
                        );

  //
  // Register callback for S5 in order to workaround the LAN shutdown issue
  //
  EntryS5DispatchContext->Type  = SxS5;
  EntryS5DispatchContext->Phase = SxEntry;
  Status = SxDispatch->Register (
                        SxDispatch,
                        SxSleepEntryCallBack,
                        EntryS5DispatchContext,
                        &(mAcpiSmm.S5SoftOffEntryHandle)
                        );

  return Status;
}


EFI_STATUS
RestorePchS3SwCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )
/*++

Routine Description:
  SMI handler to retore PchS3 code & context for S3 path
  This will be only triggered when BootScript got executed during resume

Arguments:
  DispatchHandle  - EFI Handle
  DispatchContext - Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT

Returns:
  Nothing

--*/
{
  //
  // Restore to original address by default
  //
  RestoreLockBox(&gPchS3CodeInLockBoxGuid, NULL, NULL);
  RestoreLockBox(&gPchS3ContextInLockBoxGuid, NULL, NULL);
  return EFI_SUCCESS;
}

EFI_STATUS
DisableAcpiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:
  SMI handler to disable ACPI mode

  Dispatched on reads from APM port with value 0xA1

  ACPI events are disabled and ACPI event status is cleared.  
  SCI mode is then disabled.
   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits
   Disable GPE0 sources
   Clear status bits
   Disable GPE1 sources
   Clear status bits
   Disable SCI

Arguments:
  DispatchHandle  - EFI Handle
  DispatchContext - Pointer to the  EFI_SMM_SW_REGISTER_CONTEXT

Returns:
  Nothing

--*/
{
  EFI_STATUS  Status;
  UINT16      Pm1Cnt;

  Status = GetPchPmBase (gSmst);
  ASSERT_EFI_ERROR (Status);
  Pm1Cnt = IoRead16 (mAcpiSmm.PchPmBase + R_PCH_ACPI_PM1_CNT);

  //
  // Disable SCI
  //
  Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SCI_EN;

  IoWrite16 (mAcpiSmm.PchPmBase + R_PCH_ACPI_PM1_CNT, Pm1Cnt); 

  return EFI_SUCCESS;
}

EFI_STATUS
EnableAcpiCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext,
  IN OUT    VOID                          *CommBuffer,     OPTIONAL
  IN OUT    UINTN                         *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:
  SMI handler to enable ACPI mode
  
  Dispatched on reads from APM port with value 0xA0

  Disables the SW SMI Timer.  
  ACPI events are disabled and ACPI event status is cleared.  
  SCI mode is then enabled.

   Disable SW SMI Timer

   Clear all ACPI event status and disable all ACPI events
   Disable PM sources except power button
   Clear status bits

   BIOS Lock Enable; BIOS Write Protect
   
   Disable GPE0 sources
   Clear status bits

   Disable GPE1 sources
   Clear status bits

   Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

   Enable SCI

Arguments:
  DispatchHandle  - EFI Handle
  DispatchContext - Pointer to the  EFI_SMM_SW_REGISTER_CONTEXT

Returns:
  Nothing

--*/
{
  EFI_STATUS  Status;
  UINT32      SmiEn;
  UINT16      Pm1En;
  UINT16      Pm1Cnt;
  UINT8       Data8;

  Status  = GetPchPmBase (gSmst);
  ASSERT_EFI_ERROR (Status);

  SmiEn = IoRead32 (mAcpiSmm.PchPmBase + R_PCH_SMI_EN);

  //
  // Disable SW SMI Timer and legacy USB
  //
  SmiEn &= ~(B_PCH_SMI_EN_SWSMI_TMR | B_PCH_SMI_EN_LEGACY_USB | B_PCH_SMI_EN_LEGACY_USB2);

#ifdef SDP_FLAG
  //
  // s4987586: PROMOTE from haswell_server: USB eHCI Controller Fails to Start after Random Number of Warm Resets
  //           --> Conditioning LEGACY_USB SMI for SDP build only
  // s4986856: SDP: ESX unable to take ownership of USB EHCI (6000036320)
  // 
  SmiEn |= ( B_PCH_SMI_EN_LEGACY_USB | B_PCH_SMI_EN_LEGACY_USB2);
#endif // SDP_FLAG

  //
  // And enable SMI on write to B_PCH_ACPI_PM1_CNT_SLP_EN when SLP_TYP is written
  //
  SmiEn |= B_PCH_SMI_EN_ON_SLP_EN;
  IoWrite32 (mAcpiSmm.PchPmBase + R_PCH_SMI_EN, SmiEn);

  //
  // Disable PM sources except power button
  //
  Pm1En   = B_PCH_ACPI_PM1_EN_PWRBTN;
  IoWrite16 (mAcpiSmm.PchPmBase + R_PCH_ACPI_PM1_EN, Pm1En);

  //
  // BIOS Lock Enable; BIOS Write Protect
  //
  PciOr8 (
    PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
    PCI_DEVICE_NUMBER_PCH_LPC,
    0,
    R_PCH_LPC_BIOS_CNTL),
    (UINT8) (B_PCH_LPC_BIOS_CNTL_BLE|B_PCH_LPC_BIOS_CNTL_SMM_BWP)
    );
  
  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  Data8 = RTC_ADDRESS_REGISTER_D;
  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, Data8);
  Data8 = 0x0;
  IoWrite8 (R_IOPORT_CMOS_STANDARD_DATA, Data8);

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16 (mAcpiSmm.PchPmBase + R_PCH_ACPI_PM1_CNT);
  Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SCI_EN;
  IoWrite16 (mAcpiSmm.PchPmBase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);

  //
  // Do platform specific stuff for ACPI enable SMI
  //


  return EFI_SUCCESS;
}

//pwu3: Remove PP and MOR for PO_WA
#if 0

EFI_STATUS
SmiMorCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:

  When an smi event for Mor happen.

Arguments:

Returns:

  None

--*/
{
  EFI_STATUS            Status;
  UINTN                 DataSize;
  UINT8                 MorControl;

  if (mAcpiParameter->TpmParamter == 1) {
    //
    // Called from ACPI _DSM method, save the MOR data to variable.
    //
    MorControl = (UINT8)mAcpiParameter->MorData;
  } else if (mAcpiParameter->TpmParamter == 2) {
    //
    // Called from ACPI _PTS method, setup ClearMemory flags if needed.
    //  
    DataSize = sizeof (UINT8);  
    Status = SmmGetVariable (
               mEfiMemoryOverwriteControlVariable, 
               &mEfiMemoryOverwriteControlDataGuid, 
               NULL,
               &DataSize, 
               &MorControl
               );
    if (EFI_ERROR (Status)) {
      return;
    } 

    if (MOR_CLEAR_MEMORY_VALUE (MorControl) == 0x0) {       
      return;
    }   
    MorControl &= 0xFE;     
  }
  
  DataSize = sizeof (UINT8);
  Status = SmmSetVariable (
             mEfiMemoryOverwriteControlVariable, 
             &mEfiMemoryOverwriteControlDataGuid, 
             EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
             DataSize, 
             &MorControl
             );
  return EFI_SUCCESS;      
}

// AptioV server Override
/*
EFI_STATUS
SmiPpCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  )
  */
/*++

Routine Description:

  When an smi event for physical presence happen.

Arguments:

Returns:

  None

--*/  
// AptioV server Override-Start
/*
{
  EFI_STATUS            Status;
  UINTN                 DataSize;
  EFI_PHYSICAL_PRESENCE PpData;

  //
  // Get the Physical Presence variable
  //  
  DataSize = sizeof (EFI_PHYSICAL_PRESENCE);
  Status = SmmGetVariable (
             mEfiPhysicalPresenceVariable, 
             &mEfiPhysicalPresenceGuid, 
             NULL,
             &DataSize, 
             &PpData
             );
  if (EFI_ERROR (Status)) {      
    return;
  }

  if (mAcpiParameter->TpmParamter == 5) {
    //
    // Return TPM Operation Response to OS Environment
    // 
    mAcpiParameter->LastPPRequest = PpData.LastPPRequest;
    mAcpiParameter->PPResponse    = PpData.PPResponse;
  } else if (mAcpiParameter->TpmParamter == 2) {
    //
    // Submit TPM Operation Request to Pre-OS Environment
    //
    if (PpData.PPRequest != mAcpiParameter->PPRequest) {
      PpData.PPRequest = (UINT8)mAcpiParameter->PPRequest;
      DataSize = sizeof (EFI_PHYSICAL_PRESENCE);
      Status = SmmSetVariable (
                 mEfiPhysicalPresenceVariable, 
                 &mEfiPhysicalPresenceGuid, 
                 EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                 DataSize, 
                 &PpData
                 );
    }
  }  
  return EFI_SUCCESS;
}
*/
// AptioV server Override-End
#endif

EFI_STATUS
SxSleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN CONST  EFI_SMM_SX_REGISTER_CONTEXT  *DispatchContext,
  IN OUT    VOID                         *CommBuffer,     OPTIONAL
  IN OUT    UINTN                        *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:

  Callback function entry for Sx sleep state.

Arguments:

  DispatchHandle   -  The handle of this callback, obtained when registering.
  DispatchContext  -  The predefined context which contained sleep type and phase.
    
Returns:

  EFI_SUCCESS            -  Operation successfully performed.
  EFI_INVALID_PARAMETER  -  Invalid parameter passed in.
  
--*/
{
  EFI_STATUS  Status;
  UINT32      Data32;
  
  // AptioV override start: LastState support for PowerState setup option
  if ( mSystemConfiguration.PowerState == 2 ) {
	//
	// Program Chipset to be off after AC power loss
	//
	PciOr8 (
		PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_GEN_PMCON_3),
		B_PCH_LPC_GEN_PMCON_AFTERG3_EN
		);
  }
  // AptioV override End: LastState support for PowerState setup option

  //
  // Handling S1 - setting appropriate wake bits in GPE0_EN
  //
  if ((DispatchHandle == mAcpiSmm.S1SleepEntryHandle) && (((EFI_SMM_SX_REGISTER_CONTEXT *)DispatchContext)->Type == SxS1)) {
    //
    // Set the power state in the BMC
    //
    if (mSystemConfiguration.WakeOnLanSupport == 1) {
      //
      // Enable bit11 (PME_EN), 9 (PCI_EXP) in GPE0_EN
      //
      Status = gSmst->SmmIo.Io.Read( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
      Data32 |= (B_PCH_ACPI_GPE0a_EN_PME_B0 | B_PCH_ACPI_GPE0a_EN_PCI_EXP | B_PCH_ACPI_GPE0a_EN_PME);
      Status = gSmst->SmmIo.Io.Write( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
      return EFI_SUCCESS;
    }
  }

  //
  // Handling S4, S5 and WOL - setting appropriate wake bits in GPE0_EN
  //
  if (((DispatchHandle == mAcpiSmm.S4SleepEntryHandle) && (((EFI_SMM_SX_REGISTER_CONTEXT *)DispatchContext)->Type == SxS4)) ||
      ((DispatchHandle == mAcpiSmm.S5SoftOffEntryHandle) && (((EFI_SMM_SX_REGISTER_CONTEXT *)DispatchContext)->Type == SxS5))
     ) {
    if (mSystemConfiguration.WakeOnLanSupport == 1) {
      //
      // Enable bit11 (PME_EN)  ,9 (PCI_EXP) in GPE0_EN
      // Enable the WOL bits in GPE0_EN reg here for PME
      //
      Status = gSmst->SmmIo.Io.Read( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
      Data32 |= (B_PCH_ACPI_GPE0a_EN_PME_B0 | B_PCH_ACPI_GPE0a_EN_PCI_EXP | B_PCH_ACPI_GPE0a_EN_PME);
      Status = gSmst->SmmIo.Io.Write( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
      return EFI_SUCCESS;
    }
  }

  //
  // Already handled S1, S4, S5
  // If not S3, must be error
  //
  if ((DispatchHandle != mAcpiSmm.S3SleepEntryHandle) || (((EFI_SMM_SX_REGISTER_CONTEXT *)DispatchContext)->Type != SxS3)) {
    return EFI_INVALID_PARAMETER;
  }
  
  
  //
  // Assuming S3 from here on out
  //
  //
  // Reget PCH power mgmr regs base in case of OS changing it at runtime
  //
  Status  = GetPchPmBase (gSmst);
  //Status  = SaveRuntimeScriptTable (gSmst);   //AptioV server Override
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (mSystemConfiguration.WakeOnLanSupport == 1) {
    //
    // Enable bit11 (PME_EN), 9 (PCI_EXP) in GPE0_EN
    // Enable the WOL bits in GPE0_EN reg here for PME
    //
    Status = gSmst->SmmIo.Io.Read( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
    Data32 |= (B_PCH_ACPI_GPE0a_EN_PME_B0 | B_PCH_ACPI_GPE0a_EN_PCI_EXP | B_PCH_ACPI_GPE0a_EN_PME);
    Status = gSmst->SmmIo.Io.Write( &gSmst->SmmIo, SMM_IO_UINT32, mAcpiSmm.PchPmBase + R_PCH_ACPI_GPE0a_EN, 1, &Data32 );
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetPchPmBase (
  IN EFI_SMM_SYSTEM_TABLE2       *Smst
  )
/*++

Routine Description:

  Get PCH chipset LPC Power Management I/O Base at runtime.

Arguments:

  Smst  -  The standard SMM system table.
      
Returns:

  EFI_SUCCESS  -  Successfully init the device.
  Other        -  Error occured whening calling Dxe lib functions.
  
--*/
{
  mAcpiSmm.PchPmBase    = PciRead16 (PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_ACPI_BASE)) & ~BIT0;
  return EFI_SUCCESS;
}


EFI_STATUS
SaveRuntimeScriptTable (
  IN EFI_SMM_SYSTEM_TABLE2       *Smst
  )
{
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress;
  UINT32                Data32;
  UINT16                Data16;
  UINT8                 Data8;
  UINT8                 Mask;
  UINTN                 Index;
  UINTN                 Offset;


  //
  // Save PCI-Host bridge settings (0, 0, 0). 0x90, 94 and 9c are changed by CSM
  // and vital to S3 resume. That's why we put save code here
  //
  Index = 0;
  while (mPciCfgRegTable[Index] != PCI_DEVICE_END) {

    PciAddress.Bus              = mPciCfgRegTable[Index++];
    PciAddress.Device           = mPciCfgRegTable[Index++];
    PciAddress.Function         = mPciCfgRegTable[Index++];
    PciAddress.Register         = 0;
    PciAddress.ExtendedRegister = 0;

    Data16 = PciRead16 (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register));
    if (Data16 == 0xFFFF) {
      Index += 8;
      continue;
    }

    for (Offset = 0, Mask = 0x01; Offset < 256; Offset += 4, Mask <<= 1) {

      if (Mask == 0x00) {
        Mask = 0x01;
      }

      if (mPciCfgRegTable[Index + Offset / 32] & Mask) {

        PciAddress.Register = (UINT8) Offset;
        Data32 = PciRead32 (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register));

        //
        // Save latest settings to runtime script table
        //
        S3BootScriptSavePciCfgWrite (
            S3BootScriptWidthUint32,
            PCILIB_TO_COMMON_ADDRESS (PCI_LIB_ADDRESS(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PciAddress.Register)),
            1,
          &Data32
          );
      }
    }

    Index += 8;
  }

  //
  // Save GPIO LVL2 register to S3 script table
  //
  Data32 = IoRead32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL2);
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint32, (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL2), 1, &Data32);

  //
  // Selftest KBC
  //
  Data8 = 0xAA;
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint8, 0x64, 1, &Data8);

  //
  // Important to trap Sx for SMM
  //
  Data32 = IoRead32 (mAcpiSmm.PchPmBase + R_PCH_SMI_EN);
  S3BootScriptSaveIoWrite(S3BootScriptWidthUint32, (mAcpiSmm.PchPmBase + R_PCH_SMI_EN), 1, &Data32);

  //
  // Save B_TCO_LOCK so it will be done on S3 resume path.
  //
  /* AptioV override for Grangeville platform
  Data16 = IoRead16 (mAcpiSmm.PchPmBase + PCH_TCO_BASE + R_PCH_TCO1_CNT);

  S3BootScriptSaveIoWrite(S3BootScriptWidthUint16, (mAcpiSmm.PchPmBase + PCH_TCO_BASE + R_PCH_TCO1_CNT), 1, &Data16);
  */
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
BmcAcpiSwChildCallbackFunction (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
/*++

Routine Description:

  Updates the mBmcAcpiSwChild variable upon the availability of
  the BmcAcpiSwChild Protocol.

Arguments:

  (Standard EFI notify event - EFI_EVENT_NOTIFY)

Returns:

  None

--*/
{
    return EFI_SUCCESS;
}
