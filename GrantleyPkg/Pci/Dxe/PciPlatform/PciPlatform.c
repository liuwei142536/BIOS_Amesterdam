/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed  
  for Intel CPUs and Chipsets under the terms of your license   
  agreement with Intel or your vendor.  This file may be        
  modified by the user, subject to additional terms of the      
  license agreement                                             
--*/
/*++

Copyright (c)  2004 - 2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PciPlatform.c
    
Abstract:

    Registers onboard PCI ROMs with PCI.IO.
    
Revision History:

--*/
#include <Token.h> // AptioV server override
#include <PiDxe.h>
#include "PciPlatform.h"
#ifdef EFI_PCI_IOV_SUPPORT
#include "PciIovPlatformPolicy.h"
#endif


#define MAX_ROM_NUMBER  5

PCI_PLATFORM_PRIVATE_DATA mPciPrivateData;
//AptioV server override - start. Remove OpROM processing code
#if 0
SYSTEM_CONFIGURATION      mSystemConfiguration;
PCI_OPTION_ROM_TABLE      mPciOptionRomTable[] = {
  //
  // OnBoard Video OptionROM
  //
  {
    ONBOARD_VIDEO_OPTION_ROM_FILE_GUID, // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    0,                                  // Device Number
    0,                                  // Function Number
    IGD_VID,                            // Vendor ID
    IGD_DID                             // Device ID
  },
  {
    ATS_VIDEO_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    8,                                  // Bus Number
    0,                                  // Device Number
    0,                                  // Function Number
    ATS_VID,                            // Vendor ID
    ATS_DID                             // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_LPTH_SATA_DEVICE_ID_D_RAID         // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_LPTH_SATA_DEVICE_ID_D_RAID_PREM         // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_LPTH_SATA_DEVICE_ID_D_RAID_ALTDIS  // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_LPTH_SATA_DEVICE_ID_D_RAID1   // Device ID
  },
#ifdef PCH_SERVER_BIOS_FLAG
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SATA_DEVICE_ID_D_RAID     // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SATA_DEVICE_ID_D_RAID_PREMIUM         // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SATA_DEVICE_ID_D_RAID_PREMIUM_DSEL0         // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SATA_DEVICE_ID_D_RAID_PREMIUM_DSEL1         // Device ID
  },
  {
    SATA_RAID_OPTION_ROM_FILE_GUID,     // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_PCH_SATA,         // Device Number
    PCI_FUNCTION_NUMBER_PCH_SATA,       // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SATA_DEVICE_ID_D_RAID1    // Device ID
  },
  {
    SSATA_RAID_OPTION_ROM_FILE_GUID,    // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_EVA,              // Device Number
    PCI_FUNCTION_NUMBER_EVA_SSATA,      // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SSATA_DEVICE_ID_D_RAID    // Device ID
  },
  {
    SSATA_RAID_OPTION_ROM_FILE_GUID,    // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_EVA,              // Device Number
    PCI_FUNCTION_NUMBER_EVA_SSATA,                  // Function Number
    V_PCH_SATA_VENDOR_ID,                           // Vendor ID
    V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM        // Device ID
  },
  {
    SSATA_RAID_OPTION_ROM_FILE_GUID,    // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_EVA,              // Device Number
    PCI_FUNCTION_NUMBER_EVA_SSATA,                         // Function Number
    V_PCH_SATA_VENDOR_ID,                                  // Vendor ID
    V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL0         // Device ID
  },
  {
    SSATA_RAID_OPTION_ROM_FILE_GUID,    // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_EVA,              // Device Number
    PCI_FUNCTION_NUMBER_EVA_SSATA,                        // Function Number
    V_PCH_SATA_VENDOR_ID,                                 // Vendor ID
    V_PCH_WBG_SSATA_DEVICE_ID_D_RAID_PREMIUM_DSEL1        // Device ID
  },
  {
    SSATA_RAID_OPTION_ROM_FILE_GUID,   // Guid
    0,                                  // Segment
    0,                                  // Bus Number
    PCI_DEVICE_NUMBER_EVA,              // Device Number
    PCI_FUNCTION_NUMBER_EVA_SSATA,      // Function Number
    V_PCH_SATA_VENDOR_ID,               // Vendor ID
    V_PCH_WBG_SSATA_DEVICE_ID_D_RAID1   // Device ID
  },
#endif // PCH_SERVER_BIOS_FLAG
  //
  // End of OptionROM Entries
  //
  {
    NULL_ROM_FILE_GUID, // Guid
    0,                  // Segment
    0,                  // Bus Number
    0,                  // Device Number
    0,                  // Function Number
    0xffff,             // Vendor ID
    0xffff              // Device ID
  }
};
#endif
//AptioV server override - end

EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL         *This,
  OUT EFI_PCI_PLATFORM_POLICY           *PciPolicy
  )
/*++

Routine Description:

  Set the PciPolicy as EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_NO_ALIAS.

Arguments:

  This       -  The pointer to the Protocol itself.
  PciPolicy  -  the returned Policy.

Returns:

  EFI_UNSUPPORTED        -  Function not supported.
  EFI_INVALID_PARAMETER  -  Invalid PciPolicy value.

--*/
{
  if (PciPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

//AptioV server override - start. Remove OpROM processing code
#if 0
EFI_STATUS
GetRawImage (
  IN     EFI_GUID                       *NameGuid,
  IN OUT VOID                           **Buffer,
  IN OUT UINTN                          *Size
  )
/*++

Routine Description:

  Get an indicated image in raw sections.

Arguments:

  NameGuid  -  NameGuid of the image to get.
  Buffer    -  Buffer to store the image get.
  Size      -  size of the image get.

Returns:

  EFI_NOT_FOUND   -  Could not find the image.
  EFI_LOAD_ERROR  -  Error occurred during image loading.
  EFI_SUCCESS     -  Image has been successfully loaded.

--*/
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         HandleCount;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *Fv;
  UINT32                        AuthenticationStatus;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status) || HandleCount == 0) {
    return EFI_NOT_FOUND;
  }
  //
  // Find desired image in all Fvs
  //
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    &Fv
                    );
    if (EFI_ERROR (Status)) {
      return EFI_LOAD_ERROR;
    }
    //
    // Try a raw file
    //
    *Buffer = NULL;
    *Size   = 0;
    Status = Fv->ReadSection (
                  Fv,
                  NameGuid,
                  EFI_SECTION_RAW,
                  0,
                  Buffer,
                  Size,
                  &AuthenticationStatus
                  );

    if (!EFI_ERROR (Status)) {
      break;
    }
  }

  if (Index >= HandleCount) {
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}
#endif
//AptioV server override - end

EFI_STATUS
EFIAPI
GetPciRom (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL   *This,
  IN  EFI_HANDLE                        PciHandle,
  OUT VOID                              **RomImage,
  OUT UINTN                             *RomSize
  )
/*++

Routine Description:
  
  Return a PCI ROM image for the onboard device represented by PciHandle.

Arguments:

  This       -  Protocol instance pointer.
  PciHandle  -  PCI device to return the ROM image for.
  RomImage   -  PCI Rom Image for onboard device.
  RomSize    -  Size of RomImage in bytes.

Returns:
  
  EFI_SUCCESS    -  RomImage is valid.
  EFI_NOT_FOUND  -  No RomImage.

--*/
{
//AptioV server override - start. Remove OpROM processing code
#if 0
  EFI_STATUS                        Status;
  EFI_PCI_IO_PROTOCOL               *PciIo;
  UINTN                             VarSize;
  UINTN                             Segment;
  UINTN                             Bus;
  UINTN                             Device;
  UINTN                             Function;
  UINT16                            VendorId;
  UINT16                            DeviceId;
  UINT16                            DeviceClass;
  UINTN                             TableIndex;
  UINTN                             RomImageNumber;
  VOID                              *OpRomBase;
  UINTN                             OpRomSize;
  EFI_PCI_ROM_HEADER                RomHeader;
  PCI_DATA_STRUCTURE                *Pcir;
  BOOLEAN                           SlotOptionRomDisabled;

  OpRomBase = NULL;
  OpRomSize = 0;

  Status = gBS->HandleProtocol (
                  PciHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  if (mSystemConfiguration.BootNetwork == INVALID) {
    VarSize = sizeof (mSystemConfiguration);
  // AptioV server override: Changed variable name to Intelsetup
    Status = gRT->GetVariable (
                    L"IntelSetup",
                    &gEfiSetupVariableGuid,
                    NULL,
                    &VarSize,
                    &mSystemConfiguration
                    );
    if (EFI_ERROR (Status)) {
      mSystemConfiguration.BootNetwork = 0;
    }
  }

  PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);

  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_CLASSCODE_OFFSET + 1, 1, &DeviceClass);

  //
  // Run PXE ROM only if Boot network is enabled
  //
  if ((mSystemConfiguration.BootNetwork == 0) &&
      (DeviceClass == ((PCI_CLASS_NETWORK << 8) | PCI_CLASS_NETWORK_ETHERNET))
      ) {
    DEBUG ((EFI_D_INFO, "Boot network disabled for PCI device [%02x|%02x|%02x] - PciCfg: %04x:%04x\n", Bus, Device, Function));
    return EFI_NOT_FOUND;
  }

  //
  // Run each PCI-E slot ROM only if PCI-E Slot Oprom is enabled.
  //  
  if ( Bus != 0 ) {
    if (mSystemConfiguration.PcieSlotOprom1 == INVALID) {
      
      VarSize = sizeof (mSystemConfiguration);
      
  // AptioV server override: Changed variable name to Intelsetup
      Status = gRT->GetVariable (
                      L"IntelSetup",
                      &gEfiSetupVariableGuid,
                      NULL,
                      &VarSize,
                      &mSystemConfiguration
                      );
      if (EFI_ERROR (Status)) {
        mSystemConfiguration.PcieSlotOprom1 = 1;
        mSystemConfiguration.PcieSlotOprom2 = 1;
        mSystemConfiguration.PcieSlotOprom3 = 1;
        mSystemConfiguration.PcieSlotOprom4 = 1;
        mSystemConfiguration.PcieSlotOprom5 = 1;
        mSystemConfiguration.PcieSlotOprom6 = 1;
        mSystemConfiguration.PcieSlotOprom7 = 1;
        mSystemConfiguration.PcieSlotOprom8 = 1;
      }
    }
    
    SlotOptionRomDisabled = PcieRootPortCheck(Bus, &mSystemConfiguration);
    
    if (SlotOptionRomDisabled) {
      DEBUG ((EFI_D_INFO, "PCI-E Slot Oprom is disabled for PCI device [%02x|%02x|%02x] \n", Bus, Device, Function));
      return EFI_NOT_FOUND;
    }
  }

  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &VendorId);
  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &DeviceId);
  DEBUG ((EFI_D_INFO, "PCI device [%02x|%02x|%02x] - PciCfg: %04x:%04x\n", Bus, Device, Function, (UINTN)VendorId, (UINTN)DeviceId));

  RomHeader.Raw = PciIo->RomImage;
  Pcir = (PCI_DATA_STRUCTURE *) (RomHeader.Raw + RomHeader.Generic->PcirOffset);
  DEBUG ((EFI_D_INFO, "OpromPatchEntrypoint - Pcir: %04x:%04x\n", (UINTN)Pcir->VendorId, (UINTN)Pcir->DeviceId));

  //
  // Fix MS-HD5770 video adapter can not work:
  // This device is not a OPROM 3.0 and does not have device id list as well.
  // It only have 1 device id in OPROM.
  // Device Id in OpROM is not same with the value in PCI configuration space
  // it will cause VBIOS fails to start
  //
  if ((VendorId == 0x1002) && (DeviceId == 0x68BE)) {
    DEBUG ((EFI_D_ERROR, "MS-HD5770 video adapter\n"));
    if ((Pcir->VendorId == 0x1002) &&(Pcir->DeviceId == 0x68B8)) {
      //
      // Assign same device id in PCI configuration space
      //
      Pcir->DeviceId = DeviceId;
    }
  }
#ifdef PCH_SERVER_BIOS_FLAG
   // If setup value requested EFI, we don't load the RAID OROM. 
  if ((VendorId == V_PCH_SATA_VENDOR_ID) && 
      ((IS_PCH_WBG_SATA_RAID_DEVICE_ID(DeviceId)) && 
       (mSystemConfiguration.SataRaidLoadEfiDriver == 1 )) ||
      ((IS_PCH_WBG_SSATA_RAID_DEVICE_ID(DeviceId)) && 
       (mSystemConfiguration.sSataRaidLoadEfiDriver == 1 ))  
      ) 
    { 
      return EFI_NOT_FOUND;
    }
#endif // PCH_SERVER_BIOS_FLAG
  //
  // Loop through table of video option rom descriptions
  //
  RomImageNumber = 0;
  for (TableIndex = 0; mPciOptionRomTable[TableIndex].VendorId != 0xffff; TableIndex++) {
    //
    // See if the PCI device specified by PciHandle matches at device in mPciOptionRomTable
    //
    if (VendorId != mPciOptionRomTable[TableIndex].VendorId ||
        DeviceId != mPciOptionRomTable[TableIndex].DeviceId ||
          ((mPciOptionRomTable[TableIndex].Device !=0) && (Device != mPciOptionRomTable[TableIndex].Device)) ||
          ((mPciOptionRomTable[TableIndex].Function!=0) && (Function != mPciOptionRomTable[TableIndex].Function))
        ) {
      continue;
    }

    Status = GetRawImage (
              &mPciOptionRomTable[TableIndex].FileName,
              &OpRomBase,
              &OpRomSize
              );

    if (EFI_ERROR (Status)) {
      continue;
    } else {
      RomImageNumber++;
      if (RomImageNumber == MAX_ROM_NUMBER) {
        break;
      }
    }
  }

  if (RomImageNumber == 0) {
    DEBUG((EFI_D_INFO, "No ROM image for this device\n"));
    return EFI_NOT_FOUND;

  } else {

    *RomImage = OpRomBase;
    *RomSize  = OpRomSize;
    DEBUG((EFI_D_INFO, "ROM image: base = 0x%x, size = 0x%x\n", OpRomBase, OpRomSize));
    return EFI_SUCCESS;
  }
#endif
  return EFI_NOT_FOUND; // AptioV server override. dummy return.
//AptioV server override - end
}

EFI_STATUS
EFIAPI
RegisterPciCallback (
  IN  EFI_PCI_CALLBACK_PROTOCOL               *This,
  IN  EFI_PCI_CALLBACK_FUNC                   Function,
  IN  EFI_PCI_ENUMERATION_PHASE               Phase
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  This      - GC_TODO: add argument description
  Function  - GC_TODO: add argument description
  Phase     - GC_TODO: add argument description

Returns:

  EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  EFI_UNSUPPORTED - GC_TODO: Add description for return value
  EFI_SUCCESS - GC_TODO: Add description for return value

--*/
{
  LIST_ENTRY     *NodeEntry;
  PCI_CALLBACK_DATA *PciCallbackData;

  if (Function == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ( (Phase &  (EfiPciEnumerationDeviceScanning | EfiPciEnumerationBusNumberAssigned \
              | EfiPciEnumerationResourceAssigned)) == 0) {
    return EFI_INVALID_PARAMETER; 
  }
  //
  // Check if the node has been added
  //
  NodeEntry = GetFirstNode (&mPciPrivateData.PciCallbackList);
  while (!IsNull (&mPciPrivateData.PciCallbackList, NodeEntry)) {
    PciCallbackData = PCI_CALLBACK_DATA_FROM_LINK (NodeEntry);
    if (PciCallbackData->Function == Function) {
      return EFI_UNSUPPORTED;
    }

    NodeEntry = GetNextNode (&mPciPrivateData.PciCallbackList, NodeEntry);
  }

  PciCallbackData = NULL;
  PciCallbackData = AllocateZeroPool (sizeof (PCI_CALLBACK_DATA));
  ASSERT (PciCallbackData != NULL);

  PciCallbackData->Signature  = PCI_CALLBACK_DATA_SIGNATURE;
  PciCallbackData->Function   = Function;
  PciCallbackData->Phase      = Phase;
  InsertTailList (&mPciPrivateData.PciCallbackList, &PciCallbackData->Link);

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
PciPlatformDriverEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
/*++

Routine Description:
  Main Entry point of the Pci Platform Driver.
  
Arguments:

  ImageHandle  -  Handle to the image.
  SystemTable  -  Handle to System Table.
  
Returns:

  EFI_STATUS  -  Status of the function calling.

--*/
{
  EFI_STATUS  Status;
  EFI_HOB_GUID_TYPE             *GuidHob;


  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  mPciPrivateData.PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mPciPrivateData.PlatformInfo);

  //EDK2_TODO Check if clearing mPciPrivateData.PlatformInfo (got above) is intended.
  ZeroMem (&mPciPrivateData, sizeof (mPciPrivateData));
  InitializeListHead (&mPciPrivateData.PciCallbackList);

  mPciPrivateData.PciPlatform.PlatformNotify          = PhaseNotify; 
  mPciPrivateData.PciPlatform.PlatformPrepController  = PlatformPrepController;
  mPciPrivateData.PciPlatform.GetPlatformPolicy       = GetPlatformPolicy;
  mPciPrivateData.PciPlatform.GetPciRom               = GetPciRom;
  mPciPrivateData.PciCallback.RegisterPciCallback     = RegisterPciCallback;
#ifdef EFI_PCI_IOV_SUPPORT
  mPciPrivateData.PciIovPlatform.GetSystemLowestPageSize = GetSystemLowestPageSize;
  mPciPrivateData.PciIovPlatform.GetPlatformPolicy       = GetIovPlatformPolicy;
#endif

  //  mSystemConfiguration.BootNetwork                    = INVALID; // AptioV server override:Use AMI network page

  //
  // Install on a new handle					
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPciPrivateData.PciPlatformHandle,
                  &gEfiPciPlatformProtocolGuid,
                  &mPciPrivateData.PciPlatform,
                  &gEfiPciCallbackProtocolGuid,
                  &mPciPrivateData.PciCallback,
#ifdef EFI_PCI_IOV_SUPPORT
                  &gEfiPciIovPlatformProtocolGuid,
                  &mPciPrivateData.PciIovPlatform,
#endif
                  NULL
                  );

  return Status;
}
