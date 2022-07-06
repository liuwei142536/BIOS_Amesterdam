/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MemoryCallback.c
   
Abstract:

  EFI 2.0 PEIM to provide the platform support functionality on the Bridgeport.

--*/


//Aptio V server Override Start
#include <AmiPeiLib.h>
#include <AmiStatusCodes.h>
//Aptio V server Override End

#include "PlatformEarlyInit.h"
#include <Guid/FirmwareFileSystem2.h>
#include <Guid/SetupVariable.h>

#include <Ppi/AtaPolicy.h>
#include <Ppi/Cache.h>

#include <Cpu\CpuRegs.h>
#include <FlashMap.h>

static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMainInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM2_GUID,
  (VOID*)FLASH_REGION_FVMAIN_BASE,
  FLASH_REGION_FVMAIN_SIZE,
  NULL, 
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiList1 = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMainInfoPpi
};


static PEI_ATA_POLICY_PPI     mAtaPolicyPpi = { PCH_IDE_BASE_ADDRESS };

static EFI_PEI_PPI_DESCRIPTOR mAtaPolicyPpiList[1] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gPeiAtaPolicyPpiGuid,
    &mAtaPolicyPpi
  }
};

EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
/*++

Routine Description:

  PEI termination callback. 

Arguments:

  PeiServices       -  General purpose services available to every PEIM.
  NotifyDescriptor  -  Not uesed.
  Ppi               -  Not uesed.
  
Returns:

  EFI_SUCCESS  -  If the interface could be successfully
                  installed.

--*/
{
  EFI_STATUS        Status;
  UINT64                      MemoryTop;
  UINT64            LowUncableBase;
  EFI_PLATFORM_INFO *PlatformInfo;
  UINT32            HecBaseHigh;
  EFI_BOOT_MODE     BootMode;
  EFI_HOB_GUID_TYPE *GuidHob;
  UINT32                      VariableMtrrLimit;

  VariableMtrrLimit = (UINT32) (AsmReadMsr32(EFI_IA32_MTRR_CAP) & B_EFI_IA32_MTRR_VARIABLE_SUPPORT);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Set the some PCI and chipset range as UC
  // And align to 1M at leaset
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  DEBUG ((EFI_D_ERROR, "Memory TOLM: %X\n", PlatformInfo->MemData.MemTolm));
  DEBUG (
    (EFI_D_ERROR,
    "PCIE   BASE: %lX     Size : %X\n",
    PlatformInfo->PciData.PciExpressBase,
    PlatformInfo->PciData.PciExpressSize)
    );
  DEBUG (
    (EFI_D_ERROR,
    "PCI32  BASE: %X     Limit: %X\n",
    PlatformInfo->PciData.PciResourceMem32Base,
    PlatformInfo->PciData.PciResourceMem32Limit)
    );
  DEBUG (
    (EFI_D_ERROR,
    "PCI64  BASE: %lX     Limit: %lX\n",
    PlatformInfo->PciData.PciResourceMem64Base,
    PlatformInfo->PciData.PciResourceMem64Limit)
    );
  DEBUG ((EFI_D_ERROR, "UC    START: %lX     End  : %lX\n", PlatformInfo->MemData.MemMir0, PlatformInfo->MemData.MemMir1));

  LowUncableBase = PlatformInfo->MemData.MemMaxTolm;
  LowUncableBase &= (0x0FFF00000);
  MemoryTop = (0x100000000);

  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // In BIOS, HECBASE will be always below 4GB
    //
    HecBaseHigh = (UINT32) RShiftU64 (PlatformInfo->PciData.PciExpressBase, 28);
    ASSERT (HecBaseHigh < 16);

    //
    // Programe HECBASE for DXE phase
    //
    // PlatformInfo->PciData.PciExpressSize == 0x10000000
    //

  }

  return Status;
}

VOID
EFIAPI
InstallFvInfoPpi (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_FV_HANDLE           VolumeHandle
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_PPI_DESCRIPTOR         *mPpiList;
  PEI_FV_INFO_PPI_PRIVATE        *FvInfoInstance;

  //
  // Prepare to install FirmwareVolumeInfo PPI to expose new FV to PeiCore.
  //
  FvInfoInstance =  AllocatePool (sizeof (PEI_FV_INFO_PPI_PRIVATE));

  mPpiList = &FvInfoInstance->PpiList;

  CopyMem (
    &FvInfoInstance->FvInfoPpi.FvFormat,
    &(((EFI_FIRMWARE_VOLUME_HEADER*)VolumeHandle)->FileSystemGuid),
    sizeof (EFI_GUID)
   );

  FvInfoInstance->FvInfoPpi.FvInfo = VolumeHandle;
  FvInfoInstance->FvInfoPpi.FvInfoSize = (UINT32)((EFI_FIRMWARE_VOLUME_HEADER*)VolumeHandle)->FvLength;
  FvInfoInstance->FvInfoPpi.ParentFvName = NULL;
  FvInfoInstance->FvInfoPpi.ParentFileName = NULL;          

  //
  // Install FirmwareVolumeInfo PPI to export new Firmware Volume to Core.
  //
  mPpiList->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  mPpiList->Guid  = &gEfiPeiFirmwareVolumeInfoPpiGuid;
  mPpiList->Ppi   = &FvInfoInstance->FvInfoPpi;
  Status          = (**PeiServices).InstallPpi (PeiServices, mPpiList);
  ASSERT_EFI_ERROR (Status);

}


EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
/*++

Routine Description:

  Install Firmware Volume Hob's once there is main memory

Arguments:

  PeiServices       General purpose services available to every PEIM.
  NotifyDescriptor  Notify that this module published.
  Ppi               PPI that was installed.
    
Returns:

  EFI_SUCCESS     The function completed successfully.

--*/
{
  EFI_STATUS                  Status;
  EFI_BOOT_MODE               BootMode;
  PEI_CACHE_PPI           *CachePpi;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_HOB_GUID_TYPE           *GuidHob;
  UINT64                      MemoryTop;
  UINT64                      MemoryBase;
  UINT64                      HighMemoryTop;
  UINT64                      MemoryLength;
  UINT64                      LowMemoryLength;
  UINT64                      LowUncableBase;
  UINT64                      MaxLowMemoryLength;
  UINT64                      HighMemoryLength;
  UINT64                      MaxHighMemoryLength;
  EFI_PLATFORM_INFO           *PlatformInfo;
  EFI_CPUID_REGISTER          FeatureInfo;
  UINT8                       CpuAddressWidth;
  BOOLEAN                     MTRRDefTypeUncachable;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  SYSTEM_CONFIGURATION        SysCfg;
  UINTN                       VariableSize;
  UINT64                      TempQword;
  UINT16                      Pm1Cnt;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Check if user wants to turn off in PEI phase
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    CheckPowerOffNow ();
  } else {
    Pm1Cnt  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SLP_TYP;
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }
  //
  // Set low to 1 MB. Since 1MB cacheability will always be set
  // until override by CSM.
  // Initialize high memory to 0.
  //
  LowMemoryLength   = 0x100000;
  HighMemoryLength  = 0;

  Status            = (*PeiServices)->GetHobList (PeiServices, &Hob.Raw);
  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
        if (Hob.ResourceDescriptor->PhysicalStart >= EFI_MAX_ADDRESS) {
          HighMemoryLength += Hob.ResourceDescriptor->ResourceLength;
        } else if (Hob.ResourceDescriptor->PhysicalStart >= 0x100000) {
          LowMemoryLength += Hob.ResourceDescriptor->ResourceLength;
        }
      }
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  //
  // Check if a UC regions are present
  //
  MaxLowMemoryLength  = LowMemoryLength;
  MaxHighMemoryLength = HighMemoryLength;

  if (LowMemoryLength != GetPowerOfTwo64 (LowMemoryLength)) {
    MaxLowMemoryLength = GetPowerOfTwo64 (LShiftU64 (LowMemoryLength, 1));
  }

  if (HighMemoryLength != GetPowerOfTwo64 (HighMemoryLength)) {
    MaxHighMemoryLength = GetPowerOfTwo64 (LShiftU64 (HighMemoryLength, 1));
  }

  DEBUG ((EFI_D_ERROR, "Memory Length (below 4GB) = %lx.\n", LowMemoryLength));
  DEBUG ((EFI_D_ERROR, "Power Of Two Memory Length (below 4GB) = %lx.\n", MaxLowMemoryLength));
  DEBUG ((EFI_D_ERROR, "Memory Length (above 4GB) = %lx.\n", HighMemoryLength));
  DEBUG ((EFI_D_ERROR, "Power Of Two Memory Length (above 4GB) = %lx.\n", MaxHighMemoryLength));

  PEI_PROGRESS_CODE(PeiServices, PEI_CPU_CACHE_INIT);   //Aptio V Server Override

  //
  // Load Cache PPI
  //
  Status = PeiServicesLocatePpi (
            &gPeiCachePpiGuid,  // GUID
            0,                  // Instance
            NULL,               // PEI_PPI_DESCRIPTOR
            &CachePpi           // PPI
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Set the some PCI and chipset range as UC
  // And align to 1M at leaset
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  ASSERT (PlatformInfo);

  if (PlatformInfo->PciData.PciExpressBase < PlatformInfo->PciData.PciResourceMem32Base) {
    LowUncableBase = PlatformInfo->PciData.PciExpressBase;
  } else {
    LowUncableBase = PlatformInfo->PciData.PciResourceMem32Base;
  }

  LowUncableBase &= (0x0FFF00000);
  MemoryTop     = 0;
  MemoryLength  = 0;

  MTRRDefTypeUncachable = FALSE;
  //
  // Clear the CAR Settings
  //
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid,
                            0,
                            NULL,
                            &VariableServices
                            );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SYSTEM_CONFIGURATION);

  // AptioV server override: Changed variable name to Intelsetup
  Status = VariableServices->GetVariable (
                              VariableServices,
                              L"IntelSetup",
                              &gEfiSetupVariableGuid,
                              NULL,
                              &VariableSize,
                              &SysCfg
                              );
  if (!EFI_ERROR (Status)) {
      MTRRDefTypeUncachable = SysCfg.MTRRDefTypeUncachable;
  }

  //
  // Load Cache PPI
  //
  Status = PeiServicesLocatePpi (
            &gPeiCachePpiGuid,  // GUID
            0,                  // Instance
            NULL,               // PEI_PPI_DESCRIPTOR
            &CachePpi           // PPI
            );

  if (!EFI_ERROR (Status)) {
    //
    // Clear the CAR Settings
    //
    CachePpi->ResetCache (
                PeiServices,
                CachePpi
                );

    //
    // Set fixed cache for memory range below 1MB
    //
    CachePpi->SetCache (
                PeiServices,
                CachePpi,
                0,
                0xA0000,
                EFI_CACHE_WRITEBACK
                );

    CachePpi->SetCache (
                PeiServices,
                CachePpi,
                0xA0000,
                0x60000,
                EFI_CACHE_UNCACHEABLE
                );
    // Robson technology. Enable caching for C0000 to E0000
    CachePpi->SetCache(
                PeiServices,
                CachePpi,
                0xC0000,
                0x20000,
                EFI_CACHE_WRITEPROTECTED
                );

    if (MTRRDefTypeUncachable) {
       MemoryBase = 0;
       MemoryTop  = PlatformInfo->MemData.MemTolm;
       while (MemoryBase != MemoryTop) {
        MemoryLength = GetPowerOfTwo64 (MemoryTop - MemoryBase);
        CachePpi->SetCache (
          PeiServices, 
          CachePpi, 
          MemoryBase,
          MemoryLength,
          EFI_CACHE_WRITEBACK
          );
        MemoryBase += MemoryLength;
      }

      HighMemoryTop = 0;
   
      Status = (*PeiServices)->GetHobList (PeiServices, &Hob.Raw);
      while (!END_OF_HOB_LIST (Hob)) {    
        if (Hob.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) { 
          if (Hob.ResourceDescriptor->ResourceType == EFI_RESOURCE_SYSTEM_MEMORY) {
            if (Hob.ResourceDescriptor->PhysicalStart > EFI_MAX_ADDRESS) {
              TempQword = Hob.ResourceDescriptor->PhysicalStart + Hob.ResourceDescriptor->ResourceLength;
              if (TempQword > HighMemoryTop) {
                HighMemoryTop = TempQword; 
              }
            }
          }
        }  
        Hob.Raw = GET_NEXT_HOB (Hob);
      }

      //
      // Begin with 4GB
      //
      MemoryBase    = (UINT64)EFI_MAX_ADDRESS + (UINT64) 1;
      MemoryLength  = MemoryBase;

      while (MemoryBase < HighMemoryTop) {
        //
        // Make sure cache range does not exceed physical memory
        //
        while (MemoryBase + MemoryLength > HighMemoryTop) {
          //Aptio Server override: Changing the code to resolve __aulldiv undefined external symbol.
          //MemoryLength = MemoryLength / 2;
          MemoryLength = (UINT64)RShiftU64(MemoryLength, 1);
        }
        
        CachePpi->SetCache (
          PeiServices, 
          CachePpi,
          MemoryBase,
          MemoryLength,
          EFI_CACHE_WRITEBACK
        );
        MemoryBase += MemoryLength; 
        if (MemoryBase == (UINT64)LShiftU64(MemoryLength, 1)) { // prevents infinite loop
          MemoryLength = MemoryBase;
        }
      }      

      CachePpi->SetCache (
          PeiServices, 
          CachePpi,
          FLASH_BASE,
          FLASH_SIZE,
          EFI_CACHE_WRITEPROTECTED
        );

    } else {
      MemoryTop = EFI_MAX_ADDRESS + (UINT64) 1;
      while (LowUncableBase != MemoryTop) {
        MemoryLength = GetPowerOfTwo64 (MemoryTop - LowUncableBase);
        CachePpi->SetCache (
                  PeiServices,
                  CachePpi,
                  MemoryTop - MemoryLength,
                  MemoryLength,
                  EFI_CACHE_UNCACHEABLE
                  );
        MemoryTop -= MemoryLength;
      }      

      //
      // Set the 64-bit PCI/HEC resource as UC
      //
      if (PlatformInfo->MemData.MemMir1 > PlatformInfo->MemData.MemMir0) {
        CachePpi->SetCache (
                    PeiServices,
                    CachePpi,
                    PlatformInfo->MemData.MemMir0,
                    PlatformInfo->MemData.MemMir1 - PlatformInfo->MemData.MemMir0,
                    EFI_CACHE_UNCACHEABLE
                    );
      }

      //
      // Workaround here
      // Enable default MTRR to be cachable
      //
      TempQword = AsmReadMsr32 (EFI_CACHE_IA32_MTRR_DEF_TYPE);
      TempQword |= EFI_CACHE_WRITEBACK;
      AsmWriteMsr64 (EFI_CACHE_IA32_MTRR_DEF_TYPE, TempQword);

#if ME_SUPPORT_FLAG
      //
      // Set MESEG memory range to be un-cachable if MESEG enabled
      //
      DEBUG((EFI_D_ERROR, " MESEG in MemoryCallBack.c: Base:%08x Size:%08x.\n", (UINT32)PlatformInfo->MemData.MemMESEGBase, (UINT32)PlatformInfo->MemData.MemMESEGSize));
      if( PlatformInfo->MemData.MemMESEGSize != 0 ) {
          Status = CachePpi->SetCache (
                PeiServices,
                CachePpi,
                (EFI_PHYSICAL_ADDRESS)PlatformInfo->MemData.MemMESEGBase,
                (UINT64)PlatformInfo->MemData.MemMESEGSize,
                EFI_CACHE_UNCACHEABLE
                );
          ASSERT_EFI_ERROR (Status);    // assert if MESEG enabled and no MTRR available to set it to UC.
      }
#endif //ME_SUPPORT_FLAG
    }
  }

  //
  // Create the firmware volume HOB's
  //
  Status = PeiServicesGetBootMode (&BootMode);

  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    DEBUG((EFI_D_ERROR, "=======================\n"));
    DEBUG((EFI_D_ERROR, " Boot in Normal mode\n"));
    DEBUG((EFI_D_ERROR, "=======================\n"));
//AptioV server override start: Do not build flash HOB's here. F;ash region will be reserved through RSVChipset. 
/*
    BuildFvHob (
      FLASH_REGION_FV_SECPEI_BASE,
      FLASH_REGION_FV_SECPEI_SIZE
      );

//#ifdef SPLIT_RECOVERY_FV // AptioV Server Override: Split Recovery Support Not required
    BuildFvHob (
      FLASH_REGION_FV_MRC_NORMAL_BASE,
      FLASH_REGION_FV_MRC_NORMAL_SIZE
      );
//#endif // AptioV Server Override: Split Recovery Support Not required

    BuildFvHob (
      FLASH_REGION_FVMAIN_BASE,
      FLASH_REGION_FVMAIN_SIZE
      );
      
    //
    // Install EFI_PEI_FIRMWARE_VOLUME_INFO_PPI to add this Fv into FvList.
    //
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiList1);
    ASSERT_EFI_ERROR (Status);

    BuildFvHob (
      FLASH_REGION_MICROCODE_FV_BASE,
      FLASH_REGION_MICROCODE_FV_SIZE
      );

    BuildFvHob (
      FLASH_REGION_OEM_FV_BASE,
      FLASH_REGION_OEM_FV_SIZE
      );

    BuildFvHob (
      FLASH_REGION_FLAVOR_FV_BASE,
      FLASH_REGION_FLAVOR_FV_SIZE
      );
*/
    //AptioV server override end: Do not build flash HOB's here. F;ash region will be reserved through RSVChipset.     
  } else {
    DEBUG((EFI_D_ERROR, "=======================\n"));
    DEBUG((EFI_D_ERROR, " Boot in Recovery mode\n"));
    DEBUG((EFI_D_ERROR, "=======================\n"));

    Status = PlatformPchUsbInit (PeiServices);
    ASSERT_EFI_ERROR(Status);

    //
    // Install AtaPolicyPpi to enable recovery from ATAPI devices
    //
    PeiServicesInstallPpi (&mAtaPolicyPpiList[0]);

    //
    // Prepare the recovery service
    //
    Status = PeimInitializeRecovery (PeiServices);
    ASSERT_EFI_ERROR (Status);
  }
  //AptioV server override Start: Dynamic mmcfg base address change
  BuildResourceDescriptorHob (
		  EFI_RESOURCE_MEMORY_MAPPED_IO,
		  (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
		  (UINTN) PcdGet64 (PcdPciExpressBaseAddress) ,
		  PCIEX_LENGTH
     );
  //AptioV server override End: Dynamic mmcfg base address change
  //
  // Do not need to build HOB for NV Storage FV
  //
  /*BuildFvHob (
    FLASH_REGION_RUNTIME_UPDATABLE_BASE,
    (FLASH_REGION_RUNTIME_UPDATABLE_SIZE + FLASH_REGION_NV_FTW_SPARE_SIZE)
    );*/
  //AptioV server override start: Do not build flash HOB's here. F;ash region will be reserved through RSVChipset.   
/*
  BuildResourceDescriptorHob (
    EFI_RESOURCE_FIRMWARE_DEVICE,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    FLASH_BASE,
    FLASH_SIZE
    );
*/
  //AptioV server override end: Do not build flash HOB's here. F;ash region will be reserved through RSVChipset. 
  
  //
  // Create a CPU hand-off information
  //
  CpuAddressWidth = 32;
  AsmCpuid (EFI_CPUID_EXTENDED_FUNCTION, &FeatureInfo.RegEax, NULL, NULL, NULL);
  if (FeatureInfo.RegEax >= EFI_CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (EFI_CPUID_VIR_PHY_ADDRESS_SIZE, &FeatureInfo.RegEax, NULL, NULL, NULL);
    CpuAddressWidth = (UINT8) (FeatureInfo.RegEax & 0xFF);
  }
  DEBUG ((EFI_D_ERROR, "CpuAddressWidth: %d\n", CpuAddressWidth));

  BuildCpuHob (CpuAddressWidth, 16);

  ASSERT_EFI_ERROR (Status);

  return Status;

}

BOOLEAN
CheckIfFvIsValid (
  IN EFI_PEI_SERVICES       **PeiServices,
  IN UINTN                  Address
  )
/*++

Routine Description:

  Check if the files in the Backup Blocks are valid.

Arguments:

  PeiServices  -  General purpose services available to every PEIM.
  Address      -  Address of the backup block.
    
Returns:

  TRUE   -  If all the files are valid.
  FALSE  -  If one the files are invalid.

--*/
{
  return TRUE;
}

EFI_STATUS
ValidateFvHeader (
  IN EFI_FIRMWARE_VOLUME_HEADER            *FwVolHeader
  )
{
  UINT16  *Ptr;
  UINT16  HeaderLength;
  UINT16  Checksum;

  //
  // Verify the header revision, header signature, length
  // Length of FvBlock cannot be 2**64-1
  // HeaderLength cannot be an odd number
  //
  if ((FwVolHeader->Revision != EFI_FVH_REVISION) ||
      (FwVolHeader->Signature != EFI_FVH_SIGNATURE) ||
      (FwVolHeader->FvLength == ((UINT64) -1)) ||
      ((FwVolHeader->HeaderLength & 0x01) != 0)
      ) {
    return EFI_NOT_FOUND;
  }
  //
  // Verify the header checksum
  //
  HeaderLength  = (UINT16) (FwVolHeader->HeaderLength / 2);
  Ptr           = (UINT16 *) FwVolHeader;
  Checksum      = 0;
  while (HeaderLength > 0) {
    Checksum = *Ptr++;
    HeaderLength--;
  }

  if (Checksum != 0) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
