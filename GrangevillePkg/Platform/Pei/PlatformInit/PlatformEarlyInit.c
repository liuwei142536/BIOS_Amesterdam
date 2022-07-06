/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2019 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformEarlyInit.c

Abstract:

  Do platform specific stage1 initializations.

--*/
//Aptio V server override start: Include token.h for using NB_IOxAPIC_SUPPORT token.
#include <token.h>                  

#include "PlatformEarlyInit.h"
#include <Iio\IioRegs.h>

#include <Ppi/BaseMemoryTest.h>
#include <Ppi/FlashMap.h>
#include <Ppi/SmbusPolicy.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/ReadOnlyVariable2.h>
//#include <Library/CmosAccessLib.h> // AptioV Server Override: Removed Intel CMOSACCESSLIB and using AMI CMOS manager

#include <Guid/FirmwareFileSystem2.h>
#include <Guid/TcgDataHob.h>
#include <Guid/AdminPasswordGuid.h>
#include <Guid/SetupBreakpointGuid.h>
#include <Guid/SetupVariable.h>
#if defined(AMT_SUPPORT) && AMT_SUPPORT
#include "Library/MeTypeLib.h"
#endif //AMT_SUPPORT

extern EFI_GUID gEfiMfgUUIDHobGuid;
EFI_GUID  gEfiAfterMrcGuid            = EFI_AFTER_MRC_GUID;
EFI_GUID  gEfiAfterQpircGuid          = EFI_AFTER_QPIRC_GUID;
EFI_GUID  gEfiAfterFullSpeedSetupGuid = EFI_AFTER_FULL_SPEED_SETUP_GUID;
EFI_GUID  gEfiReadyForIbistGuid       = EFI_READY_FOR_IBIST_GUID;

static PEI_BASE_MEMORY_TEST_PPI     mPeiBaseMemoryTestPpi = { BaseMemoryTest };

static PEI_PLATFORM_MEMORY_SIZE_PPI mMemoryMemorySizePpi  = { GetPlatformMemorySize };

static EFI_PEI_STALL_PPI                mStallPpi = {
  PEI_STALL_RESOLUTION,
  Stall
};

static EFI_PEI_RESET_PPI                mResetPpi = { PchResetPlatform };

static EFI_PEI_PPI_DESCRIPTOR       mInstallStallPpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiStallPpiGuid,
  &mStallPpi
};

static EFI_PEI_PPI_DESCRIPTOR       mPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gEfiPeiResetPpiGuid,
    &mResetPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiBaseMemoryTestPpiGuid,
    &mPeiBaseMemoryTestPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiPlatformMemorySizePpiGuid,
    &mMemoryMemorySizePpi
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMasterBootModePpiGuid,
    NULL
  }
};

static EFI_PEI_NOTIFY_DESCRIPTOR    mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiEndOfPeiSignalPpiGuid,
    EndOfPeiPpiNotifyCallback
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiSmbus2PpiGuid,
    ConfigurePlatformClocks
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiPeiSmbus2PpiGuid,
    PeiPrintPlatformInfo
  },
  { EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiAfterMrcGuid,
    AfterMrcBreakpoint
  },
  { EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiAfterQpircGuid,
    AfterQpircBreakpoint
  },
  { EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiAfterFullSpeedSetupGuid,
    AfterFullSpeedSetupBreakpoint
  },
  { EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
    &gEfiReadyForIbistGuid,
    ReadyForIbistBreakpoint
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMemoryDiscoveredPpiGuid,
    MemoryDiscoveredPpiNotifyCallback
  }
};




//#ifdef SPLIT_RECOVERY_FV // AptioV Server Override: Split Recovery Support Not required
static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMrcNormalInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM2_GUID,
  (VOID*)FLASH_REGION_FV_MRC_NORMAL_BASE,
  FLASH_REGION_FV_MRC_NORMAL_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvMrcNormal = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMrcNormalInfoPpi
};

static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMrcRecoveryInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM2_GUID,
  (VOID*)FLASH_REGION_FV_MRC_RECOVERY_BASE,
  FLASH_REGION_FV_MRC_RECOVERY_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvMrcRecovery = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMrcRecoveryInfoPpi
};
// AptioV Server Override Start: Split Recovery Support Not required
/*
#else
static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMrcNormalInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM2_GUID,
  (VOID*)FLASH_REGION_FV_MRC_NORMAL_BASE,
  FLASH_REGION_FV_MRC_NORMAL_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvMrcNormal = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMrcNormalInfoPpi
};

#endif
*/  // AptioV Server Override End: Split Recovery Support Not required

EFI_STATUS
PeiPrintPlatformInfo (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
/*++

Routine Description:
  Print platform information such as Bios ID and platform Type in PEI.

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *NotifyDescriptor - The notification structure this PEIM registered on install
  *Ppi - Pointer to the PPI
  
Returns:
  EFI_SUCCESS -  Function has completed successfully.


--*/
{ 
  EFI_STATUS                  Status;
  EFI_PLATFORM_INFO           *PlatformInfo;
  EFI_HOB_GUID_TYPE           *GuidHob;
  BIOS_ID_IMAGE               BiosIdImage;
  //
  // Search for the Platform Info PEIM GUID HOB.
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);
  
  DEBUG ((EFI_D_ERROR, "Platform Type = %d\n", PlatformInfo->BoardId));
  
  Status = PeiGetBiosId(PeiServices, &BiosIdImage);

  if (!EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "Bios ID: %s\n", (CHAR16 *) (&(BiosIdImage.BiosIdString))));
  }
  

  return EFI_SUCCESS;

}

EFI_STATUS
PeiGetBiosId (
  IN EFI_PEI_SERVICES       **PeiServices,
  IN OUT  BIOS_ID_IMAGE     *BiosIdImage
  )
/*++

Routine Description:
  Find the BIOS ID from the BIOS ID binary included in the flash

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *BiosIdImage  - Pointer to the BIOS ID structure being populated
  
Returns:
  EFI_NOT_FOUND - Failed to find the Bios ID binary.
  EFI_SUCCESS   - Found the BIOS ID binary.


--*/
{
  EFI_STATUS                  Status;
  VOID                        *Address;
  UINTN                       Size; 
  EFI_PEI_FV_HANDLE           VolumeHandle;
  EFI_PEI_FILE_HANDLE         FileHandle;
  EFI_FFS_FILE_HEADER         *FileHeader;
  UINTN                       Instance          = 0;
  BOOLEAN                     BiosIdBinaryFound = FALSE;
  
  //
  // Find BiosId Binary - First loop for Volume Handle
  //  
  while (TRUE) {                    
    VolumeHandle = NULL;
    Status = (*PeiServices)->FfsFindNextVolume (PeiServices, Instance++, &VolumeHandle);
    
    if (EFI_ERROR(Status)) {                        
      return Status;
    }
      
    FileHandle = NULL;
    //
    // Second loop for FileHandle
    //
    while (TRUE) { 
      Status = (*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, VolumeHandle, &FileHandle);
      // According to PI spec, FileHandle is FileHeader.
      FileHeader = (EFI_FFS_FILE_HEADER *)FileHandle;
      if (EFI_ERROR (Status)){
        break;
      }
      //
      // Compare GUID to find BIOS ID FFS
      //
      if (CompareGuid (&(FileHeader->Name), &gEfiBiosIdGuid)) {
        BiosIdBinaryFound = TRUE;
        break;
      }
    }//end of second loop
    //
    //  If the binary was found, break Volume loop
    //
    if(BiosIdBinaryFound)
      break;
  }//end of first loop  

  Status = (*PeiServices)->FfsFindSectionData (
            PeiServices,
            EFI_SECTION_RAW,
            FileHandle,
            &Address
            );
    
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  Size = sizeof (BIOS_ID_IMAGE);
    
  (*PeiServices)->CopyMem ((VOID *) BiosIdImage, Address, Size);
  
  return EFI_SUCCESS;

}

//TODO We have to port for Grangeville 

VOID
PlatformLanEarlyInit (
  IN SYSTEM_CONFIGURATION       *SystemConfiguration,
  IN EFI_PEI_SERVICES           **PeiServices,
  IN UINT16                     PlatformType
  )
{

  //
  // Disable the on-board NIC ports
  // LAN0_DIS (GPO44) controls LOM on some boards
  // LAN1_DIS (GPO45) controls PCH LAN or BMC LAN on some boards
  // Set GPIO low to disable
  //
  switch(PlatformType) {
    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
    // DO nothing now

      break;

    default:
      break;
    }

  return;
}

EFI_STATUS
EFIAPI
GetWakeupEventAndSaveToHob (
  IN EFI_PEI_SERVICES   **PeiServices
  )
/*++

Routine Description:

  Parse the status registers for figuring out the wake-up event and save it into
  an GUID HOB which will be referenced later. However, modification is required
  to meet the chipset register definition and the practical hardware design. Thus,
  this is just an example.

Arguments:

  PeiServices   - pointer to the PEI Service Table
  EFI_SUCCESS   - Always return Success
Returns:

  None


--*/
{
  UINT16  Pm1Sts;
  UINTN   Gpe0Sts;
  UINTN   WakeEventData;

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_PM1_STS);
  Gpe0Sts = IoRead32 (PCH_ACPI_BASE_ADDRESS + R_PCH_ACPI_GPE0a_STS);

  //
  // Figure out the wake-up event
  //
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_PWRBTN) != 0) {
    WakeEventData = SMBIOS_WAKEUP_TYPE_POWER_SWITCH;
  } else if (((Pm1Sts & B_PCH_ACPI_PM1_STS_WAK) != 0) || ((Gpe0Sts & B_PCH_ACPI_GPE0a_STS_PME) != 0)) {
    WakeEventData = SMBIOS_WAKEUP_TYPE_PCI_PME;
  } else if ((Gpe0Sts & B_PCH_ACPI_GPE0a_STS_RI) != 0) {
    WakeEventData = SMBIOS_WAKEUP_TYPE_MODEM_RING;
  } else if (Gpe0Sts != 0) {
    WakeEventData = SMBIOS_WAKEUP_TYPE_OTHERS;
  } else {
    WakeEventData = SMBIOS_WAKEUP_TYPE_UNKNOWN;
  }

  DEBUG ((EFI_D_ERROR, "ACPI Wake Status Register: %04x\n", Pm1Sts));

  BuildGuidDataHob (
    &gEfiTcgWakeEventDataHobGuid,
    &WakeEventData,
    sizeof (UINTN)
    );

  return EFI_SUCCESS;
}


EFI_STATUS
SendBiosIdToMcu (
    IN EFI_PEI_SERVICES    **PeiServices,
    IN BIOS_ID_STRING      *BiosIdString
  )
/*++

Routine Description:

  Allow DVP users to view the BIOS version from the MCU LCD.  
  Request to also output if ME FW is SV or non-SV

Arguments:

  IN EFI_PEI_SERVICES    **PeiServices - Pointer to PEI Services table
  IN BIOS_ID_STRING      *BiosIdString - Pointer to the BIOS ID String

Returns:

  EFI_SUCCESS    The function completed successfully.

--*/
{
  EFI_PEI_STALL_PPI           *StallPpi;
  EFI_STATUS                  Status;
  EFI_PEI_SMBUS2_PPI          *SmbusPpi;
  EFI_SMBUS_DEVICE_COMMAND    SmbusCommand;
  EFI_SMBUS_DEVICE_ADDRESS    SlaveAddress;
  CHAR8                       *FwType = "BIOS Version:";
  UINTN                       SmbusLength = 16;
  UINT8                       SmbusData[32] = {0x00};
  UINT8                       Index;
  UINT8                       *BiosId = (UINT8*)BiosIdString;

  if(IsSoftSdvPlatform())
    return EFI_SUCCESS;

  //
  // Locate SmBus Ppi
  //
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiSmbus2PpiGuid,
                            0,
                            NULL,
                            &SmbusPpi
                            );
  ASSERT_EFI_ERROR (Status);

  //
  // Locate Stall PPI
  //
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiStallPpiGuid,
                            0,
                            NULL,
                            &StallPpi
                            );
  ASSERT_EFI_ERROR (Status);

  //
  // MCU address = 0x72, Command to MCU = 0x32
  // MCU will then return SmbusLength bytes at location SmbusData (max of 32 bytes).
  // First line: SV or Non-SV. Length is +2 (leading line number byte, trailing null byte)
  //
  SlaveAddress.SmbusDeviceAddress = (0x72 >> 1);
  SmbusCommand = 0x32;
  SmbusData[0] = 0;

  for (Index = 0; (Index < (SmbusLength - 1)) && (Index <= AsciiStrLen(FwType)); Index++) {
    SmbusData[Index+1] = FwType[Index];
  }

  //
  // Use EfiSmbusBWBRProcessCall to communicate with MCU
  //
  Status = SmbusPpi->Execute( SmbusPpi,
                              SlaveAddress,
                              SmbusCommand,
                              EfiSmbusBWBRProcessCall,
                              FALSE,
                              &SmbusLength,
                              &SmbusData );

  StallPpi->Stall (PeiServices, StallPpi, 20000);

  //
  // Ensure that we have back 3 bytes and retry util buffer is free
  //
  if(Status == EFI_SUCCESS)
  {
    while(SmbusData[0] == 1)// Check if BWBR was deferred by MCU
    {
      SmbusLength = 1; // Send Length of 1
      SmbusCommand = 0xb;
      SmbusData[0] = SmbusData[1]; // Send buffer ID to read
      Status = SmbusPpi->Execute(SmbusPpi,
                                SlaveAddress,
                                SmbusCommand,
                                EfiSmbusBWBRProcessCall,
                                FALSE,
                                &SmbusLength,
                                &SmbusData );
    }
  }

  //
  // Second line: BIOS version including Date/time
  // e.g. "12.D82.06110935"
  //
  SlaveAddress.SmbusDeviceAddress = (0x72 >> 1);
  SmbusCommand = 0x32;
  SmbusLength  = 19;
  SmbusData[0] = 1;
  
  BiosId += 30;  // Skip to Major Version entry

  for (Index = 0; Index < 17; Index++) {
     SmbusData[Index+1] = *BiosId;
     BiosId += 2;  // Converting Unicode to ASCII
  }

  //
  // Use EfiSmbusBWBRProcessCall to communicate with MCU
  //
  Status = SmbusPpi->Execute( SmbusPpi,
                              SlaveAddress,
                              SmbusCommand,
                              EfiSmbusBWBRProcessCall,
                              FALSE,
                              &SmbusLength,
                              &SmbusData );

  StallPpi->Stall (PeiServices, StallPpi, 20000);
  //
  // Ensure that we have back 3 bytes and retry until buffer is free
  //
  if(Status == EFI_SUCCESS)
  {
    while (SmbusData[0] == 1){ // Check if BWBR was deferred by MCU
      SmbusLength = 1; // Set length to 1 to only send buffer ID
      SmbusCommand = 0xb;
      SmbusData[0] = SmbusData[1]; // Send buffer ID to read
      Status = SmbusPpi->Execute( SmbusPpi,
                                  SlaveAddress,
                                  SmbusCommand,
                                  EfiSmbusBWBRProcessCall,
                                  FALSE,
                                  &SmbusLength,
                                  &SmbusData );
    }
  }
  return Status;
}


EFI_STATUS
GetSetupVariable (
  IN EFI_PEI_SERVICES                **PeiServices,
  IN   SYSTEM_CONFIGURATION          *SystemConfiguration
  )
{
  UINTN                   VariableSize;
  EFI_STATUS              Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                   Index;
  // Aptio V Server Override start
// EIP200326 Start:
  EFI_PEI_SPI_SOFTSTRAPS_PPI      *mSpiAccPpi;
  UINT32                          PchStrp1;
// EIP200326 End:
// Aptio V Server Override end

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  ZeroMem (SystemConfiguration, sizeof (SYSTEM_CONFIGURATION));

  //
  // Locate Variable PPI
  //
  Status = (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );  
  // AptioV server override start
  ASSERT_EFI_ERROR (Status);
  // AptioV server override end
  if (EFI_ERROR (Status))
  {
        DEBUG((EFI_D_ERROR, "LocatePpi Error in PlatformEarlyInit.c\n"));
  }

// AptioV server override: Changed variable name to IntelSetup
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"IntelSetup",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &VariableSize,
                          SystemConfiguration
                          );

  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Use the default parameters...\n"));
    //
    // Get Default Value
    //
    SystemConfiguration->MmcfgBase = 0;
    SystemConfiguration->PciMinSecondaryBus = 1;
    SystemConfiguration->TsegSize = 4;      //default 64M
    SystemConfiguration->IedSize = 1;
    SystemConfiguration->ProcessorHyperThreadingDisable = 0;
    SystemConfiguration->SataInterfaceMode = 1; // Keep AHCI mode as Default
#ifdef PCH_SERVER_BIOS_FLAG
    SystemConfiguration->sSataInterfaceMode = 1; // Keep AHCI mode as Default
#endif //PCH_SERVER_BIOS_FLAG
    SystemConfiguration->SataType[0] = 0; //HDD by default
    SystemConfiguration->SataType[1] = 0; //HDD by default
    SystemConfiguration->IdeMode = 0; //Default Legacy (0/1 Legacy/Native)
    SystemConfiguration->PchLan = 1;
    SystemConfiguration->PchSerm = 0; // Disabled by default
    SystemConfiguration->PfatEnabled = 0;  // Disable Pfat by default
    SystemConfiguration->FixupPlatformSpecificSoftstraps = 0;
    for (Index = 0; Index <  GetPchMaxPciePortNum (); Index++) {
      SystemConfiguration->PcieRootPortSpeed[Index] = 0; //Auto
    }
    SystemConfiguration->ProcessorFlexibleRatio = 63;
    SystemConfiguration->BspSelection = 0;
    SystemConfiguration->VideoSelect = 2; // AptioV Server Override - force offboard video, no onboard video
    SystemConfiguration->DevPresIoApicIio0 = 1;
    SystemConfiguration->DevPresIoApicIio1 = 1;
    SystemConfiguration->DevPresIoApicIio2 = 1;
    SystemConfiguration->DevPresIoApicIio3 = 1;
    SystemConfiguration->serialDebugMsgLvl = 1;
    SystemConfiguration->PowerState = 1;
  }else {
    DEBUG((EFI_D_ERROR, "Use the Variable...\n"));
  }

// Aptio V Server Override start: Disable IIO IO APICs when NB_IOxAPIC_SUPPORT token disabled
#if NB_IOxAPIC_SUPPORT==0
  SystemConfiguration->DevPresIoApicIio0 = 0;
  SystemConfiguration->DevPresIoApicIio1 = 0;
  SystemConfiguration->DevPresIoApicIio2 = 0;
  SystemConfiguration->DevPresIoApicIio3 = 0;
#endif
// Aptio V Server Override end
  
// Aptio V Server Override start
// EIP200326 Start:
  Status = (**PeiServices).LocatePpi(
                            PeiServices,
                            &gPeiSpiSoftStrapsPpiGuid,
                            0,
                            NULL,
                            &mSpiAccPpi
                            );

  if (!EFI_ERROR(Status)) {
    Status = mSpiAccPpi->SoftStrapRead(
                          PeiServices,
                          mSpiAccPpi,
                          0,
                          PCHSTRAP_1 * sizeof(UINT32),
                          &PchStrp1
                          );
    if (PchStrp1 & BIT25) {
      SystemConfiguration->TpmOnSpi = 1;
    }
  }
// EIP200326 End:
// Aptio V Server Override end

  return Status;
}

EFI_STATUS
UpdatePlatformInfo (
  IN   SYSTEM_CONFIGURATION          *SystemConfiguration
  )
/*++

Routine Description:

  Initialize POC register by Variable.

Arguments:

  *SystemConfiguration  -  Pointer to SystemConfiguration variables.

Returns:

  EFI_SUCCESS  -  Success.

--*/
{
//  UINT8             TpmSupport;
  EFI_PLATFORM_INFO *PlatformInfo;
  EFI_HOB_GUID_TYPE *GuidHob;
#ifdef LT_FLAG
  UINT32            DprSize;
#endif
  //
  // Update the PCIE base and 32/64bit PCI resource support
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  PlatformInfo->SysData.SysIoApicEnable       = PCH_IOAPIC;

  if (SystemConfiguration->DevPresIoApicIio0) {
    PlatformInfo->SysData.SysIoApicEnable |= IIO0_IOAPIC;
  }
  if (SystemConfiguration->DevPresIoApicIio1) {
    PlatformInfo->SysData.SysIoApicEnable |= IIO1_IOAPIC;
  }
  if (SystemConfiguration->DevPresIoApicIio2) {
    PlatformInfo->SysData.SysIoApicEnable |= IIO2_IOAPIC;
  }
  if (SystemConfiguration->DevPresIoApicIio3) {
    PlatformInfo->SysData.SysIoApicEnable |= IIO3_IOAPIC;
  }

  //
  // Check to make sure TsegSize is in range, if not use default.
  //
  if (SystemConfiguration->TsegSize > MAX_PROCESSOR_TSEG) {
    SystemConfiguration->TsegSize = MAX_PROCESSOR_TSEG; // if out of range make default 64M
  } 
  PlatformInfo->MemData.MemTsegSize  = (0x400000 << SystemConfiguration->TsegSize);
  
  if (SystemConfiguration->IedSize > 0) {
    PlatformInfo->MemData.MemIedSize = (0x400000 << (SystemConfiguration->IedSize - 1));
  } else {
    PlatformInfo->MemData.MemIedSize = 0;
  }

  //
  // Minimum SMM range in TSEG should be larger than 3M
  //
  ASSERT (PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemIedSize >= 0x300000);

#ifdef LT_FLAG
//Aptio V Server Override - only fill size if Ltsx is enabled.
if (SystemConfiguration->ProcessorLtsxEnable) {
  DprSize = SystemConfiguration->DPRSize;

  PlatformInfo->MemData.MemDprMemSize = (DprSize << 20);
  PlatformInfo->MemData.MemLtMemSize  = PlatformInfo->MemData.MemDprMemSize + LT_DEV_MEM_SIZE;
} else {
    PlatformInfo->MemData.MemLtMemSize = 0;
    PlatformInfo->MemData.MemDprMemSize = 0;
}
#endif
/*
  //
  // Build TPM support GUID HOB
  //
  TpmSupport = SystemConfiguration->TpmSupport;
  if (TpmSupport == 0) {
    //
    // Clear TPM Command CMOS area
    //
    IoWrite8(R_ICH_IOPORT_CMOS_UPPER_INDEX, TCG_CMOS_AREA_OFFSET);
    IoWrite8(R_ICH_IOPORT_CMOS_UPPER_DATA, 0);
  }
  BuildGuidDataHob (
    &gPeiTpmSupportGuid,
    &TpmSupport,
    1
    );
*/
  return EFI_SUCCESS;
}
#ifdef LT_FLAG
#pragma optimize ("", off)
BOOLEAN
FindFITRecord (
  IN    UINT8          Type,
  OUT   UINTN          *FITRecordAddressInFit,
  OUT   UINTN          *FITRecordAddress
)
/*++

Routine Description:

  Finds the address of a FIT Record in the bootblock region of bios.

Arguments:

  FITRecordAddressInFit = The address of the FIT Record (in the FIT table) if one exists, or 0 otherwise.
  FITRecordAddress      = The address of the FIT REcord if one exists, or 0 otherwise.

Returns:

  Nothing.

--*/
{
  UINTN     *FITAddressPtr;
  UINT8     *FITAddress;
  UINT16     NumberOfFitEntries;
  BOOLEAN   OkAddress;
  UINTN     FITAddressTmp;
  BOOLEAN   FindValidFit;

  OkAddress = FALSE;
  FindValidFit = FALSE;

  FITAddressPtr = (UINTN *) 0xFFFFFFC0;      // first try the defualt address for bios that supports only WSM CPU family
  FITAddress = (UINT8 *) (* FITAddressPtr);
  if (FITAddress == (UINT8 *)0xFFFFFFFF) { // Bad Fit pointer - do not try to dereference
    *FITRecordAddressInFit = (UINTN) 0;
    *FITRecordAddress = (UINTN) 0;
    return FindValidFit;
  }


  if (  ((FITAddress[0])=='_') && ((FITAddress[1])=='F') && ((FITAddress[2])=='I') && ((FITAddress[3])=='T') && ((FITAddress[4])=='_')  ) {
      OkAddress = TRUE;
    }

  if (!OkAddress) {
    FITAddressPtr = (UINTN *) 0xFFFFFFE8;      // now try the defualt address for bios that supports only the special NHM C0 fused CPU
    FITAddress = (UINT8 *) (* FITAddressPtr);
    if (  ((FITAddress[0])=='_') && ((FITAddress[1])=='F') && ((FITAddress[2])=='I') && ((FITAddress[3])=='T') && ((FITAddress[4])=='_')  ) {
        OkAddress = TRUE;
      }
    }

  if (OkAddress) {
    OkAddress = FALSE;
    NumberOfFitEntries = (UINT16) FITAddress[8];
    while (NumberOfFitEntries) {
      if ( FITAddress[0x0E] == Type ) {
        FITAddressTmp = (UINTN) FITAddress[3];
        FITAddressTmp = FITAddressTmp << 8;
        FITAddressTmp |= (UINTN) FITAddress[2];
        FITAddressTmp = FITAddressTmp << 8;
        FITAddressTmp |= (UINTN) FITAddress[1];
        FITAddressTmp = FITAddressTmp << 8;
        FITAddressTmp |= (UINTN) FITAddress[0];
        FITAddressTmp =  FITAddressTmp;
        OkAddress = TRUE;
        FindValidFit = TRUE;
        FITAddressPtr = (UINTN *) FITAddressTmp;
        break;
      }
      FITAddress += 0x10;               // move to the next entry in the FIT table
      NumberOfFitEntries --;
    }
  }

  if (!OkAddress) {
    // specified FIT entry type not found in FIT table
    FITAddress = 0;
    FITAddressPtr = 0;
  }

  *FITRecordAddressInFit = (UINTN) FITAddress;
  *FITRecordAddress = (UINTN) FITAddressPtr;

  return FindValidFit;
}
#pragma optimize ("", on)
#endif

//
// Table providing details on clocks supported by this library
//
// It is critical that this table be properly constructed.
// The table entries must line up with the clock generatory types
//
CLOCK_GENERATOR_DETAILS mSupportedClockGeneratorTable[] = {
{
    ClockGeneratorCk410,
    CK410_GENERATOR_ID,
    CK410_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK410_GENERATOR_SPREAD_SPECTRUM_BIT
  },
{
    ClockGeneratorCk420,
    CK420_GENERATOR_ID,
    CK420_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK420_GENERATOR_SPREAD_SPECTRUM_BIT
  },
{
    ClockGeneratorCk505,
    CK505_GENERATOR_ID,
    CK505_GENERATOR_SPREAD_SPECTRUM_BYTE,
    CK505_GENERATOR_SPREAD_SPECTRUM_BIT
  }
};

EFI_STATUS
ConfigurePlatformClocks (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *SmbusPpi
  )
/*++

Routine Description:

  Configure the clock generator to enable free-running operation.  This keeps
  the clocks from being stopped when the system enters C3 or C4.

Arguments:

  None

Returns:

  EFI_SUCCESS    The function completed successfully.

--*/
{
//
// Not supported on SoftSDV
//
  EFI_STATUS                            Status;
  BOOLEAN                               EnableSpreadSpectrum;
  UINT8                                 Data;
  UINTN                                 VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  SYSTEM_CONFIGURATION                  SystemConfiguration;



  UINT8 ClockAddress;
  ClockAddress = CLOCK_GENERATOR_ADDRESS;

  if(IsSoftSdvPlatform())
    return EFI_SUCCESS;

  Data = 0;

  //
  // Locate Variable PPI
  //
  (**PeiServices).LocatePpi (
                  PeiServices,
                  &gEfiPeiReadOnlyVariable2PpiGuid,
                  0,
                  NULL,
                  &PeiVariable
                  );

  if (!PeiVariable) {
    EnableSpreadSpectrum = FALSE;
  } else {
    //
    // Get Setup Variable
    //
    VariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
    Status = PeiVariable->GetVariable (
                            PeiVariable,
                            L"IntelSetup",
                            &gEfiSetupVariableGuid,
                            NULL,
                            &VariableSize,
                            &SystemConfiguration
                            );
    if (Status != EFI_SUCCESS) {
      EnableSpreadSpectrum = FALSE;
      
    } else {
      EnableSpreadSpectrum = SystemConfiguration.EnableClockSpreadSpec;
      
    }
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
PlatformEarlyInitEntry (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Platform specific initializations in stage1.

Arguments:

  FileHandle         Pointer to the PEIM FFS file header.
  PeiServices       General purpose services available to every PEIM.

Returns:

  EFI_SUCCESS       Operation completed successfully.
  Otherwise         Platform initialization failed.
--*/
{
  EFI_STATUS                            Status;
  SYSTEM_CONFIGURATION                  SystemConfiguration;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_BOOT_MODE                         BootMode;
  UINT32                                SerialDebugMsgLevelSetting;
  UINT32                                RprBitSet = 0;

#ifdef LT_FLAG
  EFI_PLATFORM_TXT_POLICY_DATA          PlatformTxtPolicyData;
  UINTN                                 StartupAcmAddressInFit;
  UINTN                                 StartupAcmAddress;
  BOOLEAN                               FoundFit;
  UINTN                                 FailSafeAddressInFit=0;
  UINTN                                 FailSafeAddress=0;
  UINTN                                 LcpSize=0;
#endif
  //
  // TODO: Shadow into memory
  //

  //
  // Get platform information HOB entry
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  //
  // Program all the BASEs, including GPIO, SIO...
  //
  PchBaseInit ();
  
  //
  // Set the new boot mode
  //
  Status = UpdateBootMode ((EFI_PEI_SERVICES**)PeiServices, PlatformInfo->BoardId);
  ASSERT_EFI_ERROR (Status);

  //
  // Port80 to LPC
  //
  GetSetupVariable ((EFI_PEI_SERVICES**)PeiServices, &SystemConfiguration);
  if(SystemConfiguration.PchPort80Route == 0)	{   // LPC
		RprBitSet = MmioRead32(PCH_RCRB_BASE + R_PCH_RCRB_GCS);
		RprBitSet = RprBitSet & ~B_PCH_RCRB_GCS_RPR;
		MmioWrite32(PCH_RCRB_BASE + R_PCH_RCRB_GCS, RprBitSet);
	}
  else	{  // PCI
		RprBitSet = MmioRead32(PCH_RCRB_BASE + R_PCH_RCRB_GCS);
		RprBitSet = RprBitSet | B_PCH_RCRB_GCS_RPR;
		MmioWrite32(PCH_RCRB_BASE + R_PCH_RCRB_GCS, RprBitSet);
	}

  //
  // Set LPC SIO
  //
//  PchLpcPciCfg16Or (, );
if(PlatformInfo->BoardId==TypePiestewaPeak)  {
  PciOr16(
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_ENABLES),
    (B_PCH_LPC_ENABLES_CNF2_EN|B_PCH_LPC_ENABLES_CNF1_EN)
    );
  }else {  
  PciOr16(
    PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0,R_PCH_LPC_ENABLES),
    B_PCH_LPC_ENABLES_CNF1_EN
    );
   }
  if(PlatformInfo->BoardId!=TypePiestewaPeak)  
  LpcSioEarlyInit ();

  //AptioV server override start: Fault Tolerant Firmware Recovery from DVD.
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
   
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    SystemConfiguration.SataInterfaceMode = 0; // IDE mode for recovery
     }
  //AptioV server override start: Fault Tolerant Firmware Recovery from DVD.
 //
  //
  // Do basic PCH init
  //
  Status = PlatformPchInit (&SystemConfiguration, (EFI_PEI_SERVICES**)PeiServices, PlatformInfo->BoardId);

  ASSERT_EFI_ERROR (Status);
if(PlatformInfo->BoardId==TypePiestewaPeak)  
  AST2400EarlyInit(); 
 //
 // Initialize W83527Sio 
 //
  
  //AptioV server override start: Fault Tolerant Firmware Recovery from DVD.
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
   
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    SystemConfiguration.SataInterfaceMode = 0; // IDE mode for recovery
     }
  //AptioV server override end: Fault Tolerant Firmware Recovery from DVD.

  //
  // IIO initialization
  //
  PeiIioInit ();

  //
  // Initialize platform PPIs
  //
  Status = PeiServicesInstallPpi (&mInstallStallPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Install MRC {Normal,Recovery} FV info
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
/* AptioV Server Override
  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvMrcNormal);
  }
  ASSERT_EFI_ERROR (Status);
*/
  //
  // Create the platform Flash Map
  //
  Status = PeimInitializeFlashMap (FileHandle, PeiServices);
  ASSERT_EFI_ERROR (Status);

  if((EFI_ERROR (Status) )) {
    // If first boot, setup variable not yet set, so use default debug print level.
    SerialDebugMsgLevelSetting = PcdGet32(PcdDebugPrintErrorLevel);
  } else {
    switch(SystemConfiguration.serialDebugMsgLvl & 0x03) {
    case 0:
      // Disabled
      SerialDebugMsgLevelSetting = 0x00000000;
      break;
    case 1:
      // Error only
      SerialDebugMsgLevelSetting = EFI_D_ERROR;
      break;
    case 2:
    default:
      // Normal
      SerialDebugMsgLevelSetting = PcdGet32(PcdDebugPrintErrorLevel);
      break;
    case 3:
      // Maximum
      SerialDebugMsgLevelSetting = (EFI_D_INIT | EFI_D_WARN | EFI_D_LOAD | EFI_D_FS | EFI_D_POOL | EFI_D_PAGE |
                                    EFI_D_INFO | EFI_D_VARIABLE | EFI_D_BM | EFI_D_BLKIO | EFI_D_NET | EFI_D_UNDI |
                                    EFI_D_LOADFILE | EFI_D_EVENT | EFI_D_ERROR);
      break;
    }
  }
  SetDebugPrintErrorLevel(SerialDebugMsgLevelSetting);

  UpdatePlatformInfo (&SystemConfiguration);

  if (BootMode == BOOT_WITH_MFG_MODE_SETTINGS) {
   CheckForMfgUuid((EFI_PEI_SERVICES**)PeiServices);
//Aptio V Grantley Override: Commenting
//  PcdSetBool (PcdTpmPhysicalPresence, TRUE);
  }


  //
  // Do basic CPU init
  //
  Status = PlatformCpuInit ((EFI_PEI_SERVICES**)PeiServices, &SystemConfiguration);

  //
  // Do platform specific on-board Zoar init
  //
  PlatformLanEarlyInit(&SystemConfiguration, (EFI_PEI_SERVICES**)PeiServices, PlatformInfo->BoardId);
  
  //
  // Initialize platform PPIs
  //
  Status = PeiServicesInstallPpi (&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  Status = PeimInstallFlashMapPpi (FileHandle, PeiServices);
  ASSERT_EFI_ERROR (Status);

#ifdef LT_FLAG
if (SystemConfiguration.ProcessorLtsxEnable) {  //Aptio V Server Override - Only execute if Ltsx is enabled.
  if(IsSoftSdvPlatform()== FALSE) {    //If this is a softSdvPlatform, do not excute the following code HSD#4166209-Unmapped access to address 0xeeeeeeee
  //
  // Add BIOS ACM address hob
  //
  FoundFit = FindFITRecord (PLATFORM_TXT_FIT_BIOSACM_TYPE, &StartupAcmAddressInFit, &StartupAcmAddress);
  //
  // Get BIOS LCP PD Base & Size by searching FAIL_SAFE  Policy in FIT Table
  //
  FindFITRecord (PLATFORM_TXT_FIT_FAILSAFE_TYPE, &FailSafeAddressInFit, &FailSafeAddress);
  if (FailSafeAddress != 0) {
      LcpSize = (UINTN)*(UINT64 *)(FailSafeAddressInFit+8);
      LcpSize = LcpSize & 0x0000FFFFFF;
      LcpSize = LcpSize << 4;
      LcpSize = LcpSize + 0xC;
  }
  // Only build HOB when LTSX strapping ON, in which case ucode would setup LT_EXIST.
  if ( (FoundFit) && ((*(volatile UINT32 *)(UINTN)LT_SPAD_EXIST)) ) {
   // PlatformTxtPolicyData.BiosAcmAddress           = ACM_BASE; //AptioV Server override: Use SDL token for ACM BASE.
    PlatformTxtPolicyData.BiosAcmAddress           = ACM_BASE;
    PlatformTxtPolicyData.StartupAcmAddressInFit   = (EFI_PHYSICAL_ADDRESS) StartupAcmAddressInFit; // This is to be used later in EFI shell utility when updating the FI table to enable the ACM module (might not be needed anymore)
    PlatformTxtPolicyData.StartupAcmAddress        = (EFI_PHYSICAL_ADDRESS) StartupAcmAddress;
    PlatformTxtPolicyData.MiscPolicy               = (EFI_PLATFORM_TXT_POLICY_MISC_NO_SCHECK_IN_RESUME | EFI_PLATFORM_TXT_POLICY_MISC_NO_NODMA_TABLE);
    PlatformTxtPolicyData.BiosOsDataRegionRevision = 3;
    PlatformTxtPolicyData.LcpPolicyDataBase        = (EFI_PHYSICAL_ADDRESS) FailSafeAddress;
    PlatformTxtPolicyData.LcpPolicyDataSize        = (UINTN)LcpSize;
    PlatformTxtPolicyData.TxtScratchAddress        = 0;
    PlatformTxtPolicyData.BiosAcmPolicy            = 0;
    PlatformTxtPolicyData.FlagVariable             = 0;

    BuildGuidDataHob (
      &gEfiPlatformTxtPolicyDataGuid,
      &PlatformTxtPolicyData,
      sizeof (PlatformTxtPolicyData)
      );
  } else {
    DEBUG((EFI_D_ERROR, "Fit Table not found, do nothing for LT-SX ...\n"));
  }
}
    // AptioV Server Override: Eip 443149 changes - Start
    // System Lock should not be configurable in release BIOS

    //Add platform policy for LT Config Lock control
    //User can decide whether to perform LT Config Lock by setting PCD for platform specific requirement.
    //Need to force LT config lock
    if ((SystemConfiguration.LockChipset == 0) || (SystemConfiguration.DFXEnable == 1))
        PcdSetBool (PcdLtConfigLockEnable, FALSE);
    // AptioV Server Override: Eip 443149 changes - End
}
#endif

  //
  // Initialize platform PPIs
  //

    Status = PeiServicesNotifyPpi (&mNotifyList[0]);
  
  ASSERT_EFI_ERROR (Status);
#if FDT_LOCK_ENABLE //AptioV server override: Lock FDT based on SDL
  PchLockDescriptorRegion((EFI_PEI_SERVICES**)PeiServices);
#endif //AptioV server override: Lock FDT based on SDL

  switch (PlatformInfo->BoardId) {
   case TypeEchoCanyon:
    PcdSet64 (PcdOemSkuPHHDIndirectDevicesLo, 0x0000000400100000);
    PcdSet64 (PcdOemSkuPHHDIndirectDevicesHi, 0x0000000000000000);
    break;
  }
  
  return Status;
}

EFI_STATUS SpiDescriptorAccess (
  UINT8  BYTE_COUNT,
  UINT8  FLASH_CYCLE,
  UINT32 FLASH_ADDR,
  UINT32 *DATA_BUFF
)
{
  UINT16 Mask;
  UINT8  Index;
  UINT8  ReadCmd;
  UINT8  WriteCmd;

  ReadCmd  = B_SB_SPI_READ_CYCLE;
  WriteCmd = B_SB_SPI_WRITE_CYCLE;

  Mask     = (1<<15)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3);

  //set Flash Address to get Descriptor
  *(volatile UINT32 *)(SPI_BASE_ADDRESS + R_SB_SPI_FADDR) = FLASH_ADDR;

  //initiate Read cycle to Flash and get Descriptor Region 0


  //
  // HSFCTL offset 0x6: Hardware Sequencing Flash Control
  // need to clear following bits:
  //     bits [13:8] Flash Data Byte Count(FDBC), specifies the number of bytes
  //                 to shift in or out during data portion of SPI cycle
  //     bits [07:1] Flash Cycle, this field specifies the cycle type
  //                 00 Read     (1 to 64 defined by FDBC)
  //                 01 Reserved
  //                 10 Write    (1 to 64 defined by FDBC)
  //                 11 Block Erase
  //     bit  [0]    Flash Cycle Go, A write to this register with 1 will initiate
  //                 an spi cycle
  //
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    &=   Mask;                //clear register
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    |= ((UINT16)BYTE_COUNT)<<8; //read/write X bytes [13:8]


  //READ command to SPI
  if(FLASH_CYCLE == ReadCmd){

     //send Read Command
     *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    |=   FLASH_CYCLE;

     //
     //wait for Data to be ready
     // HSFSTS offset 0x4: Hardware Sequencing Flash Status
     // SPI Cycle in progress bit [5]
     //     Hardware sets this bit when software sets the flash cycle go
     //     this bit remains set until the cycle completes on SPI interface
     //
     while((*(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1<<5));

     //Data is ready copy to Buffer
     for(Index=0; Index <= (BYTE_COUNT/4); Index++){
         DATA_BUFF[Index] = *(volatile UINT32 *)(SPI_BASE_ADDRESS + R_SB_SPI_FDAT0 + Index );
     }
  }

  //WRITE command to SPI
  if(FLASH_CYCLE == WriteCmd){

     //Copy Buffer to registers for writing
     for(Index=0; Index <= (BYTE_COUNT/4); Index++){
         *(volatile UINT32 *)(SPI_BASE_ADDRESS + R_SB_SPI_FDAT0 + Index ) = DATA_BUFF[Index];
     }

     //send Write Command
     *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    |=   FLASH_CYCLE;

     //
     //wait for Data to be ready
     // HSFSTS offset 0x4: Hardware Sequencing Flash Status
     // SPI Cycle in progress bit [5]
     //     Hardware sets this bit when software sets the flash cycle go
     //     this bit remains set until the cycle completes on SPI interface
     //
     while((*(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1<<5));
  }

  return(EFI_SUCCESS);
}

EFI_STATUS DescriptorCheck(void)
{

  UINT32   Data32;
  UINT32   FlashAddr;
  UINT8    CmdCycle;

  CmdCycle  = B_SB_SPI_READ_CYCLE;
  FlashAddr = DESCRIP_ADDR;

  //
  //FLVALSIG offset 010h: Flash Valid signature
  //       bits [31:00]: 0x0FF0A55Ah
  //       This field identifies the flash descriptor sector as vaild
  //       if content is not expected value, then flash descriptor region
  //       is assumed un-programmed or corrupted
  //
  SpiDescriptorAccess(0x3,CmdCycle,FlashAddr,&Data32);

  if(Data32 == 0x0FF0A55A){
     //Valid signature found
     return(EFI_SUCCESS);
  }else{
     //Valid signature not found we are not in descriptor mode
     return(EFI_UNSUPPORTED);
  }//end valid signature

}

EFI_STATUS SetupVSCC()
{
    UINT16 VSCC = 0;

  VSCC=SPI_VSCC_MINUM_COMMUN_DENOMINATOR;

  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_LVSCC  )    = VSCC; //Set Erase block upcode
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_UVSCC  )    = VSCC; //Set Erase block upcode

  if(*(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_UVSCC  ) == VSCC){
      return(EFI_SUCCESS);
  }else{
      return(EFI_UNSUPPORTED);
  }
}

EFI_STATUS SpiAccessCheck(void)
{

  UINT32   Data32;
  UINT16   Data16;

  // Check if SPI controller is available
  Data32 = *(volatile UINT32 *)(UINT32)(SPI_BASE_ADDRESS);

  if(Data32 != 0xFFFFFFFF){

     SetupVSCC(); // Setup VSCC values.

     //
     // HSFSTS offset 0x4: Hardware Sequencing Flash Status
     // Flash Configuration Lock-Down bit [15]
     //     when set to 1 Flash program registers controlled by FLOCKDN
     //     cannot be written.
     //

     Data16    = *(volatile UINT16 *)(UINT32)( SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS );

     if (Data16 & 0x8000){
         //if Lock down is set there is nothing to do
         return(EFI_UNSUPPORTED);
     }else{
         //SPI Controller is available
         return(EFI_SUCCESS);
     }//end lock bit set

  }else{
     //SPI controller was not available signal error
     return(EFI_UNSUPPORTED);
  }//end SPI controller available

}

EFI_STATUS SpiBlockErase (
  UINT32 FLASH_ADDR
)
{
  UINT16 Mask;

  Mask     = (1<<15)|(1<<7)|(1<<6)|(1<<5)|(1<<4)|(1<<3);

  //set Flash Address to get Descriptor
  *(volatile UINT32 *)(SPI_BASE_ADDRESS + R_SB_SPI_FADDR) = FLASH_ADDR;

  //
  // HSFCTL offset 0x6: Hardware Sequencing Flash Control
  // need to clear following bits:
  //     bits [13:8] Flash Data Byte Count(FDBC), specifies the number of bytes
  //                 to shift in or out during data portion of SPI cycle
  //     bits [07:1] Flash Cycle, this field specifies the cycle type
  //                 00 Read     (1 to 64 defined by FDBC)
  //                 01 Reserved
  //                 10 Write    (1 to 64 defined by FDBC)
  //                 11 Block Erase
  //     bit  [0]    Flash Cycle Go, A write to this register with 1 will initiate
  //                 an spi cycle
  //
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    &=   Mask;                //clear register
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    |=   0x3F << 8;           //set 64 bytes [13:8]

  //send Erase Command
  *(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFCTL )    |=   B_SB_SPI_BLOCK_ERASE;

  //wait for Data to be ready
  while((*(volatile UINT16 *)(SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS)) & (1<<5));

  return(EFI_SUCCESS);
}

EFI_STATUS UpdatePdrUUID(
  IN        EFI_PEI_SERVICES                **PeiServices,
  IN        UINT8                           *Buffer,
  IN        UINT32                          DestOffset,
  IN        UINT32                          BufferSize
)
{
  UINT32      BlockSize;
  UINT8       *BackUpBlock1, *BackUpBlock2, *TargetAddress;
  UINT8       *BackUpBlock1Start, *BackUpBlock2Start;
  UINT32      Index;
  UINT32      FlashAddr;
  UINT32      DescSize;
  UINT32      Data32;
  UINT16      Data16;
  UINT8       CmdCycle;
  UINT32      PdrAddress;
  EFI_STATUS  Status;

  //====================================================================
  // Check SPI Controller availability
  //====================================================================
  //
  // If RCBA has not been properly set SPI control registers will return
  // 0xF's function will return with unsupported request
  //

  Status      = SpiAccessCheck();

  if( Status != EFI_SUCCESS){
      //SPI controller was not available signal error
      DEBUG((EFI_D_ERROR, "ERROR: SPI Controller not available \n"));
      return(Status);
  }else{
      //SPI controller was is available
      DEBUG((EFI_D_ERROR, "SPI Controller is available \n"));
  }

 //====================================================================
 // Check if we are working in Descriptor Mode
 //====================================================================
 //
 // SPI can work in descriptor or non descriptor mode this is establish
 // by reading a valid descriptor signature 0x0FF0A55A
 //
  Status      = DescriptorCheck();

  if( Status != EFI_SUCCESS){
      //Valid signature not found we are not in descriptor mode
      DEBUG((EFI_D_ERROR,"ERROR: VALID SIGNATURE NOT FOUND.\n"));
      return(Status);
  }else{
      //Valid signature found
       DEBUG((EFI_D_ERROR,"VALID SIGNATURE FOUND.\n"));
  }

  PdrAddress = *(volatile UINT32 *)(SPI_BASE_ADDRESS + R_SB_SPI_FREG4);
  PdrAddress = (PdrAddress & B_PCH_SPI_FREG4_BASE_MASK) << 12;

 //====================================================================
 // Start Write Process
 //====================================================================
  Data16    = *(volatile UINT16 *)(UINT32)( SPI_BASE_ADDRESS + R_SB_SPI_HSFSTS );

  //
  //Get Block Size from HSFSTS
  //    HSFSTS offset 0x4: Hardware Sequencing Flash Status
  //    bits [4:3] Block/Sector Erase Size
  //    valid bit settings:
  //          00: 256 Byte
  //          01: 4K  Byte
  //          10: 8K  Byte
  //          11: 64K Byte

  BlockSize = (Data16&0x1F)>>3; // get bits 4:3 from HSFSTS

  switch(BlockSize){
     case 0: BlockSize =BLOCK_SIZE_256B; // ~256 Bytes
             DEBUG((EFI_D_ERROR, "BLOCK SIZE 256B \n"));
             break;
     case 1: BlockSize = BLOCK_SIZE_4KB; // ~4KB
             DEBUG((EFI_D_ERROR, "BLOCK SIZE  4KB \n"));
             break;
     case 2: BlockSize = BLOCK_SIZE_8KB; // ~8KB
             DEBUG((EFI_D_ERROR, "BLOCK SIZE  8KB \n"));
             break;
     case 3: BlockSize =BLOCK_SIZE_64KB; // ~64KB
             DEBUG((EFI_D_ERROR, "BLOCK SIZE 64KB \n"));
             break;
     default:DEBUG((EFI_D_ERROR, "Invalid BLOCK SIZE \n"));
             return(EFI_UNSUPPORTED);
  }//end switch

  //
  //SPI Write operation requires a block erase since even
  //for a single bit the erase operation will clear an entire block
  //so before we attempt a write we must find a way to back up the
  //block that it will be erased.
  //

  //DescSize = BLOCK_SIZE_64KB;
  DescSize = BLOCK_SIZE_32KB;
  //This function will create a buffer of block size to back up data
  Status   = (*PeiServices)->AllocatePool (
                             PeiServices,
                             DescSize,
                             &BackUpBlock1
                             );
  Status   = (*PeiServices)->AllocatePool (
                             PeiServices,
                             DescSize,
                             &BackUpBlock2
                             );
  if(Status!= EFI_SUCCESS){
     //unable to create buffer of size BlockSize
     DEBUG((EFI_D_ERROR, "ERROR: UNABLE to allocate buffer of size=%x \n",BlockSize));
     return(Status);
  } else  {
    BackUpBlock1Start = BackUpBlock1;
    BackUpBlock2Start = BackUpBlock2;
    //
    // Read the PDR region, copy to buffer
    //
    FlashAddr = PdrAddress;

    //copy first 32kb
    for(Index=0; Index < DescSize ; Index+=4, FlashAddr += 4){
      CmdCycle  = B_SB_SPI_READ_CYCLE;
      SpiDescriptorAccess(0x3,CmdCycle,FlashAddr,&Data32);
      *(UINT32*)BackUpBlock1 = Data32;
      BackUpBlock1 += 4;
    }

//  FlashAddr += 4;
    //copy 2nd 32kb
    for(Index=0; Index < DescSize ; Index+=4, FlashAddr += 4){
      CmdCycle  = B_SB_SPI_READ_CYCLE;
      SpiDescriptorAccess(0x3,CmdCycle,FlashAddr,&Data32);
      *(UINT32*)BackUpBlock2 = Data32;
      BackUpBlock2 += 4;
    }

    //
    // Adjust the BackupBlock pointer to the offset passed in, and copy the data in the buffer passed in
    // to the BackupBlock
    //
    BackUpBlock1  = BackUpBlock1Start;
    BackUpBlock2  = BackUpBlock2Start;

    if(DestOffset<DescSize)
        TargetAddress  = BackUpBlock1 + DestOffset;
    else
        TargetAddress  = BackUpBlock2 + (DestOffset - DescSize);

    for (Index = 0; Index < BufferSize; Index++, TargetAddress++) {
      *TargetAddress = Buffer[Index];
    }

    //
    // Erase the PDR
    //
    SpiBlockErase(PdrAddress);

    //
    // Write back the PDR with the new data added.
    //

    FlashAddr = PdrAddress;
    //Write the first 32 kb of the block
    for(Index=0; Index < DescSize ; Index+=4, FlashAddr += 4){
      CmdCycle  = B_SB_SPI_WRITE_CYCLE;
      Data32 = *(UINT32 *)BackUpBlock1;
      SpiDescriptorAccess(0x3,CmdCycle,FlashAddr,&Data32);
      BackUpBlock1 += 4;
    }

    //Write the second 32 kb of theblock
    for(Index=0; Index < DescSize ; Index+=4, FlashAddr += 4){
      CmdCycle  = B_SB_SPI_WRITE_CYCLE;
      Data32 = *(UINT32 *)BackUpBlock2;
      SpiDescriptorAccess(0x3,CmdCycle,FlashAddr,&Data32);
      BackUpBlock2 += 4;
    }

   DEBUG((EFI_D_ERROR,"PDR UPDATE COMPLETE \n"));

  }// end else allocate buffer

  return(Status);
//return EFI_SUCCESS;
}



EFI_STATUS
CheckForMfgUuid (
  IN EFI_PEI_SERVICES           **PeiServices
  )
/*++

Routine Description:

  Check for variable containing the Mfg UUID.

Arguments:

  Pei Services

Returns:

  Status.

--*/
{
  EFI_STATUS                      Status = EFI_SUCCESS;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PeiVariable;
  UINTN                           SizeUUID;
  UINT8                           Buffer[sizeof (EFI_GUID)];

  //
  // Locate the Setup configuration value.
  //
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid,
                            0,
                            NULL,
                            &PeiVariable
                            );
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //
  // Check if MFG UUID variable is present
  //
  SizeUUID = sizeof (EFI_GUID);

  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"MfgSystemUuid",
                          &gEfiSetupVariableGuid,
                          NULL,
                          &SizeUUID,
                          Buffer
                          );

  if (SizeUUID == sizeof (EFI_GUID) && (!EFI_ERROR (Status))) {
    //
    // Update the PDR in flash with the UUID.
    //
    Status = UpdatePdrUUID (
      PeiServices,
      Buffer,
      UUID_OFFSET,
      SizeUUID
      );

    //
    // Create the HOB with the MfgUUID
    //
    BuildGuidDataHob (
               &gEfiMfgUUIDHobGuid,
               Buffer,
               SizeUUID
               );
  }

  return Status;
}

EFI_STATUS
EFIAPI
AfterMrcBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:
  Checks the setup option for breakpoint type and starts
  a deadloop if After MRC was selected

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *NotifyDescriptor - The notification structure this PEIM registered on install
  *Ppi - Pointer to the PPI

Returns:
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    SYSTEM_CONFIGURATION            SetupData;

    Status = GetSetupVariable(PeiServices, &SetupData);
    //
    // If setup variables haven't yet been created
    //
    if(EFI_ERROR(Status) )
      SetupData.ValidationBreakpointType = BP_NONE;

    if (SetupData.ValidationBreakpointType == BP_AFTER_MRC) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint After MRC is done! == \n"));
        EFI_DEADLOOP();
    }

    return Status;
}

EFI_STATUS
EFIAPI
AfterQpircBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:
  Checks the setup option for breakpoint type and starts
  a deadloop if After QPIRC was selected

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *NotifyDescriptor - The notification structure this PEIM registered on install
  *Ppi - Pointer to the PPI

Returns:
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    SYSTEM_CONFIGURATION            SetupData;

    Status = GetSetupVariable(PeiServices, &SetupData);
    if(EFI_ERROR(Status) )
      SetupData.ValidationBreakpointType = BP_NONE;

    if (SetupData.ValidationBreakpointType == BP_AFTER_QPIRC) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint After QPIRC is done! == \n"));
        EFI_DEADLOOP();
    }

    return Status;
}


EFI_STATUS
EFIAPI
ReadyForIbistBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:
  Checks the setup option for breakpoint type and starts
  a deadloop if Ready for IBIST was selected

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *NotifyDescriptor - The notification structure this PEIM registered on install
  *Ppi - Pointer to the PPI

Returns:
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    SYSTEM_CONFIGURATION            SetupData;

    Status = GetSetupVariable(PeiServices, &SetupData);
    if(EFI_ERROR(Status) )
      SetupData.ValidationBreakpointType = BP_NONE;

    if (SetupData.ValidationBreakpointType == BP_READY_FOR_IBIST) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint when Ready for IBIST! == \n"));
        EFI_DEADLOOP();
    }

    return Status;
}

EFI_STATUS
EFIAPI
AfterFullSpeedSetupBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
/*++

Routine Description:
  Checks the setup option for breakpoint type and starts
  a deadloop if the After Full Speed Setup was selected

Arguments:
  **PeiServices - Pointer to the list of PEI services
  *NotifyDescriptor - The notification structure this PEIM registered on install
  *Ppi - Pointer to the PPI

Returns:
  EFI_SUCCESS  -  Function has completed successfully.

--*/
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    SYSTEM_CONFIGURATION            SetupData;

    Status = GetSetupVariable(PeiServices, &SetupData);
    if(EFI_ERROR(Status) )
      SetupData.ValidationBreakpointType = BP_NONE;

    if (SetupData.ValidationBreakpointType == BP_AFTER_FULL_SPEED_SETUP) {
        DEBUG((EFI_D_INFO, "== Validation Breakpoint before QPI go Fast! == \n"));
        EFI_DEADLOOP();
    }

    return Status;
}


VOID
SaveAdminPassToHob (
  IN EFI_PEI_SERVICES   **PeiServices
  )
/*++
Routine Description:
  Admin password is read and saved to HOB 
  to prevent its cleared on defaults load.

Arguments:

  PeiServices   - pointer to the PEI Service Table

Returns:

  None.

--*/
{
    SYSTEM_CONFIGURATION        SystemConfiguration;
    EFI_STATUS                  Status;

    
    Status = GetSetupVariable(PeiServices, &SystemConfiguration);
    if( EFI_ERROR(Status)){
        return;
    }

    BuildGuidDataHob (
        &gEfiAdminPasswordHobGuid,
        &SystemConfiguration.AdminPassword,
        sizeof (SystemConfiguration.AdminPassword)
        );

}

/*++

Routine Description:
  Update warm reset counter located in CMOS to let know to DXE drivers
  what type of reset (warm or cold) was executed.
  Counter <=1   executed cold reset
  Counter >1    executed warm reset

  Limit counter to WARM_RESET_COUNTER_LIMIT
  to avoid false cold reset indication

Arguments:
   None

Returns:
  nothing

--*/
VOID
UpdateWarmResetCounter(
    VOID
    )
{
  UINT8   Data8;
  BOOLEAN WarmReset;
  UINT8   CmosValue;

  DEBUG((EFI_D_INFO, "UpdateWarmResetCounter() "));

  // Read CMOS WarmReset Counter
  IoWrite8(R_IOPORT_CMOS_UPPER_INDEX, CMOS_WARM_RESET_COUNTER_OFFSET);
  CmosValue = IoRead8( R_IOPORT_CMOS_UPPER_DATA );

  Data8 = PciRead8 (
            PCI_LIB_ADDRESS (
              DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC,
              R_PCH_LPC_GEN_PMCON_2
            )
          );
  WarmReset = (BOOLEAN) !!(Data8 & B_PCH_LPC_GEN_PMCON_MEM_SR);

  if (WarmReset) {
    // current boot is warm
    CmosValue++;
    DEBUG((EFI_D_INFO, "to WARM\n"));
#define WARM_RESET_COUNTER_LIMIT 10
    if (CmosValue > WARM_RESET_COUNTER_LIMIT) {
      // limit CmosValue to avoid counter rewind
      // and false cold reset indication
      CmosValue = WARM_RESET_COUNTER_LIMIT;
    }
#undef WARM_RESET_COUNTER_LIMIT
  } else {
    // current boot is cold, reset warm reset counter
    CmosValue = 0;
    DEBUG((EFI_D_INFO, "to COLD\n"));
  }
  
  IoWrite8(R_IOPORT_CMOS_UPPER_INDEX, CMOS_WARM_RESET_COUNTER_OFFSET);
  IoWrite8(R_IOPORT_CMOS_UPPER_DATA, CmosValue);
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  PcdSet32(PcdMePlatformWarmResetCounter, (UINT32) CmosValue);
#endif // ME_SUPPORT_FLAG

}

EFI_STATUS
PchLockDescriptorRegion(
  IN EFI_PEI_SERVICES **PeiServices
  )
{
  UINT32      FlashAddr;
  UINT8       CmdCycle;
  UINT32      Flmap1;
  UINT32      fmba;
  UINT32      Data32;
  UINT8       Data8;
  EFI_STATUS  Status;

  DEBUG((EFI_D_INFO, "LockFlashDescriptor() Entry \n"));

  UpdateWarmResetCounter();

#if defined(AMT_SUPPORT) && AMT_SUPPORT
  if (MeTypeIsAmt()) {
    DEBUG((EFI_D_INFO, "For AMT do not execute LockFlashDescriptor()\n"));
    return EFI_SUCCESS;
  }
#endif // AMT_SUPPORT
  Status = SpiAccessCheck();

  if (Status != EFI_SUCCESS){
    DEBUG((EFI_D_ERROR,  "ERROR: SPI Controller not available \n")); 
    return(Status);
  }

  Status = DescriptorCheck();

  if (Status != EFI_SUCCESS){
    DEBUG((EFI_D_ERROR, "ERROR: VALID SIGNATURE NOT FOUND.\n"));
    return(Status);
  }

  FlashAddr = DESCRIP_FLMAP1;
  CmdCycle = B_SB_SPI_READ_CYCLE;
  SpiDescriptorAccess(0x3, CmdCycle, FlashAddr, &Flmap1);

  fmba = 0;
  fmba |= (Flmap1 & 0x000000FF) << 4;

  DEBUG((EFI_D_INFO, "FLASH MAP1 = %x, FMBA = %x.\n",Flmap1, fmba));

  //Flash Master 1 - Host CPU / BIOS
  FlashAddr = fmba;
  CmdCycle = B_SB_SPI_READ_CYCLE;
  SpiDescriptorAccess(0x3, CmdCycle, FlashAddr, &Data32);

  DEBUG((EFI_D_INFO, "Flash Master 1 = %x.\n", Data32));

  if ((Data32 & BIT24) == BIT24) {
    Data32 &= ~BIT24;

    FlashAddr = fmba;
    CmdCycle = B_SB_SPI_WRITE_CYCLE;
    SpiDescriptorAccess(0x3, CmdCycle, FlashAddr, &Data32);

    DEBUG((EFI_D_ERROR, "Performing global reset to block SPI flash descriptor...\n"));

    PciOr32 (
     PCI_LIB_ADDRESS(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_PMIR),
     (UINT32)B_PCH_LPC_PMIR_CF9GR
     );
    Data8 = COLD_RESET | FULL_RESET;
    IoWrite8 (RESET_PORT, Data8);
    Data8 |= RST_CPU;
    IoWrite8 (RESET_PORT, Data8);
    CpuDeadLoop();
  }

  return(Status);
}
