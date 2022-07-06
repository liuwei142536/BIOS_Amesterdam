//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file PtuLoader.c

  SPS Node Manager may require Power-Thermal Utility application to run in host
  to measure the system power consumption. This application is provided as
  Option ROM for MROM device. But not all PCH have implement MROM.
  On systems where there is no MROM PTU is invoked by this module.
  
**/

/*++

  Copyright (c)  2014 - 2015 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

Module Name:

  PtuLoader.c

Abstract:
  SPS Node Manager may require Power-Thermal Utility application to run in host
  to measure the system power consumption. This application is provided as
  Option ROM for MROM device. But not all PCH have implement MROM.
  On systems where there is no MROM PTU is invoked by this module.

--*/

#include <Token.h> // AptioV Server Override.
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/Spi.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Library/DevicePathLib.h>
#include <IndustryStandard/Pci22.h>
#include <Uefi.h>
#include <Protocol/DriverBinding.h>
#include <Library/PciLib.h>
#include <MeChipset.h>
#include <MeAccess.h>
#include <Sps.h>
#include <HeciRegs.h>

// AptioV Server OverRide Start: For using AMI's flash library.

typedef enum
{
    FlashRegionBios = 0,
    FlashRegionMe,
    FlashRegionGbE,
    FlashRegionDescriptor,
    FlashRegionPlatformData,
    FlashRegionDeviceExpansion, // DER Region 5
    FlashRegionMax
    
}FLASH_REGION_TYPE;

// Intel Flash Region Operation
typedef enum
{
    FlashRegionRead = 0,
    FlashRegionErase,
    FlashRegionWrite
    
}FLASH_OPERATION;

/*
 * This function reads data from the specified Flash Region
 *
 * @param Type Type of Flash region.   
 * @param Address Start Address of region.                      
 * @param Length number of bytes to read.                      
 * @param Buffer pointer to data to read from the flash region.
 *
 * @retval EFI_SUCCESS Operation Completed
 * @retval EFI_INVALID_PARAMETER Operation not Completed
 */
EFI_STATUS
FlashRegionOperation (
    FLASH_REGION_TYPE   Type,
    FLASH_OPERATION     Operation,
    IN UINT32           Address,
    IN UINT32           Length,
    IN UINT8            *Buffer
);

// AptioV Server OverRide End: For using AMI's flash library.

/**
 * PTU Loader entry point.
 *
 * param[in] ImageHandle    Handle of driver image
 * param[in] pSysTable      Pointer to the system table
 *
 * return EFI_STATUS is returned.
 */
EFI_STATUS
PtuLoaderEntryPoint (
        IN EFI_HANDLE         ImageHandle,
        IN EFI_SYSTEM_TABLE   *SystemTable
        )
{
  EFI_STATUS                                  Status = EFI_SUCCESS;
  UINT32                                      ImageSize;
// AptioV Server OverRide Start: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.
//###DEBUG  EFI_SPI_PROTOCOL                            *pSpiProtocol;
// AptioV Server OverRide End: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.
  EFI_PCI_EXPANSION_ROM_HEADER                PtuRomHeader;
  MEDIA_RELATIVE_OFFSET_RANGE_DEVICE_PATH     EfiOpRomImageNode;
  EFI_DEVICE_PATH_PROTOCOL                    *pPciOptionRomImageDevicePath;
  PCI_DATA_STRUCTURE                          Pcir;
  VOID                                        *pPtuImage = NULL;
  UINT16                                      ImageOffset;
  UINT32                                      InitializationSize;
  SPS_NMFS                                    NmFirmwareStatus;

  DEBUG((EFI_D_INFO, "[SPS] PtuLoaderEntryPoint\n"));

  NmFirmwareStatus.DWord = Heci2PciRead32(SPS_R_NMFS);


if (NmFirmwareStatus.DWord == 0xFFFFFFFF || !NmFirmwareStatus.Bits.NmEnabled || !NmFirmwareStatus.Bits.DirectPtuExecution)
  {
    DEBUG((EFI_D_INFO, "[SPS] PtuLoader: Direct PTU.efi execution not required (NMFS: 0x%08X)\n", NmFirmwareStatus.DWord));
    return EFI_NOT_FOUND;
  }

// AptioV Server OverRide Start: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

//###DEBUG START
//####  Status = gBS->LocateProtocol(&gEfiSpiProtocolGuid, NULL, (VOID**)&pSpiProtocol);
//####  if (EFI_ERROR(Status))
//####  {
//####    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: Cannot locate SPI protocol (%r)\n", Status));
//####    return Status;
//####  }
//
//####  Status = pSpiProtocol->Execute(
//####            pSpiProtocol,
//####            SPI_OPCODE_READ_INDEX,
//####            0,
//####            TRUE,
//####            TRUE,
//####            FALSE,
//####            0,
//####            sizeof(PtuRomHeader),
//####            (UINT8*)&PtuRomHeader,
//####            EnumSpiRegionDer
//####            );
//###DEBUG END

  Status = FlashRegionOperation(
              FlashRegionDeviceExpansion,
              FlashRegionRead,
              0,
              sizeof(PtuRomHeader),
              (UINT8*)&PtuRomHeader
              );
// AptioV Server OverRide End: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: SPI read PCI expansion ROM header filed: %r\n", Status));
    return Status;
  }

  if (PtuRomHeader.Signature != PCI_EXPANSION_ROM_HEADER_SIGNATURE)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: PCI expansion ROM signature not found\n"));
    return EFI_NOT_FOUND;
  }
  
// AptioV Server OverRide Start: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

//###DEBUG START
//####  Status = pSpiProtocol->Execute(
//####            pSpiProtocol,
//####            SPI_OPCODE_READ_INDEX,
//####            0,
//####            TRUE,
//####            TRUE,
 //####           FALSE,
//####            PtuRomHeader.PcirOffset,
//####            sizeof(Pcir),
//####            (UINT8*)&Pcir,
//####            EnumSpiRegionDer
//####            );
//###DEBUG END

  Status = FlashRegionOperation(
              FlashRegionDeviceExpansion,
              FlashRegionRead,
              PtuRomHeader.PcirOffset,
              sizeof(Pcir),
              (UINT8*)&Pcir
              );
// AptioV Server OverRide End: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

  if (Pcir.Signature != PCI_DATA_STRUCTURE_SIGNATURE)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: PCI data signature not found in DER region\n"));
    return EFI_NOT_FOUND;
  }

  EfiOpRomImageNode.Header.Type     = MEDIA_DEVICE_PATH;
  EfiOpRomImageNode.Header.SubType  = MEDIA_RELATIVE_OFFSET_RANGE_DP;
  SetDevicePathNodeLength(&EfiOpRomImageNode.Header, sizeof(EfiOpRomImageNode));

  EfiOpRomImageNode.StartingOffset  = 0x0;
  EfiOpRomImageNode.EndingOffset    = (UINT32)(Pcir.ImageLength * 512) - 1;

  pPciOptionRomImageDevicePath = AppendDevicePathNode(NULL, NULL);
  pPciOptionRomImageDevicePath = AppendDevicePathNode(pPciOptionRomImageDevicePath, &EfiOpRomImageNode.Header);
  if (pPciOptionRomImageDevicePath == NULL)
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: Unable to create PCI OpROM image device path\n"));
    Status = EFI_NOT_FOUND;
    goto ErrExit0;
  }

  ImageOffset        = PtuRomHeader.EfiImageHeaderOffset;
  InitializationSize = PtuRomHeader.InitializationSize * 512;
  ImageSize          = InitializationSize - ImageOffset;

  Status = gBS->AllocatePool(EfiBootServicesData, ImageSize, &pPtuImage);
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: AllocatePool for PtuImage failed: %r\n", Status));
    goto ErrExit0;
  }

// AptioV Server OverRide Start: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

//###DEBUG START
//####  Status = pSpiProtocol->Execute(
//####            pSpiProtocol,
//####           SPI_OPCODE_READ_INDEX,
//####            0,
//####            TRUE,
//####            TRUE,
//####            FALSE,
//####            ImageOffset,
//####            ImageSize,
//####            (UINT8*)pPtuImage,
//####            EnumSpiRegionDer
//####            );
//###DEBUG END

  Status = FlashRegionOperation(
                FlashRegionDeviceExpansion,
                FlashRegionRead,
                ImageOffset,
                ImageSize,
                (UINT8*)pPtuImage
                );
// AptioV Server OverRide End: Commenting code to use Intel SPI protocol. 
//                         AMI's flash library will be used.

  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: SPI read PtuImage filed: %r\n", Status));
    goto ErrExit1;
  }

  ImageHandle = NULL;
  Status = gBS->LoadImage(
            FALSE,
            gImageHandle,
            pPciOptionRomImageDevicePath,
            pPtuImage,
            ImageSize,
            &ImageHandle
            );
  if (EFI_ERROR (Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: LoadImage failed: %r\n", Status));
    goto ErrExit1;
  }
  else
  {
    DEBUG((EFI_D_INFO, "[SPS] PTU image loaded, starting...\n"));
  }

  Status = gBS->StartImage(ImageHandle, NULL, NULL);
  
  DEBUG((EFI_D_INFO, "[SPS] PTU execution status: %r\n", Status));
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: PtuLoader: StartImage failed (%r)\n", Status));
    goto ErrExit1;
  }
  return EFI_SUCCESS;

 ErrExit1:
  gBS->FreePool(pPtuImage);
  
 ErrExit0:
  gBS->FreePool(pPciOptionRomImageDevicePath);
  return Status;
} // PtuLoaderEntryPoint()

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
