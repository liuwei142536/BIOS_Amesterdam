/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  LegacyRegion.c

Abstract:
  This file contains functions needed for manipulation of
  PAM registers that control access to C0000-FFFFF range.
  The PAM registers are assumed to reside in the PCI config
  space of the device 0, function 0, bus 0.
  Given that, the only code needed to be modified here for
  purposes of porting to a new platform/chipset are the values
  in the staic initialization of the mPamData[] variable.

--*/
#include "LegacyRegion.h"

#define C_PCSR_BIOSEN     0x40D8
//
// Module Global:
//  Since this driver will only ever produce one instance of
//  the Private Data protocol you are not required to dynamically
//  allocate the PrivateData.
//
LEGACY_REGION_INSTANCE  mPrivateData;


//
// Data specific to the chipset defining mappings between
// addresses/sizes of regions and control registers/bits.
//
EFI_IIO_UDS_PROTOCOL        *mIioUds;


STATIC PAM_DATA         mPamData[] = {
  {
    0xF0000,
    0x10000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM0),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xC0000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM1),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xC4000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM1),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xC8000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM2),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xCC000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM2),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xD0000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM3),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xD4000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM3),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xD8000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM4),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xDC000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM4),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xE0000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM5),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xE4000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM5),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  },
  {
    0xE8000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM6),
    V_PAM_LOENABLE_DRAM_RD,
    V_PAM_LOENABLE_DRAM_WR
  },
  {
    0xEC000,
    0x04000,
    EFI_PCI_ADDRESS(0x00, FixedPcdGet8(PcdCboDev), FixedPcdGet8(PcdCboFunc), R_CQ1_CR_PAM6),
    V_PAM_DMIENABLE_DRAM_RD,
    V_PAM_DMIENABLE_DRAM_WR
  }
};

EFI_STATUS
EFIAPI
LegacyRegionGetInfo (
  IN  EFI_LEGACY_REGION2_PROTOCOL   *This,
  OUT UINT32                        *DescriptorCount,
  OUT EFI_LEGACY_REGION_DESCRIPTOR  **Descriptor
  )
{
  return EFI_UNSUPPORTED;
}



BOOLEAN
LegacyRegionRangeContainsRange (
  IN  UINT32                              Start1,
  IN  UINT32                              Length1,
  IN  UINT32                              Start2,
  IN  UINT32                              Length2
  )
/*++

Routine Description:
  Tests if range 1 contains range 2 in its entirety.

Arguments:

  Start1   -  start of containing range.
  Length1  -  size of containing range.
  Start2   -  start of contained range.
  Length2  -  size of contained range.

Returns:

  TRUE  -  Range 1 contains range 2, FALSE otherwise.

--*/
{
  if ((Start2 >= Start1) && ((Start2 + Length2) <= (Start1 + Length1))) {
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
LegacyRegionRangeIntersectsRange (
  IN  UINT32                              Start1,
  IN  UINT32                              Length1,
  IN  UINT32                              Start2,
  IN  UINT32                              Length2
  )
/*++

Routine Description:
  Tests if two ranges have a common section i.e., they intersect.

Arguments:

  Start1   -  start of first range.
  Length1  -  size of first range.
  Start2   -  start of second range.
  Length2  -  size of second range.

Returns:

  TRUE  -  If ranges intersect, FALSE otherwise.

--*/
{
  if (((Start1 >= Start2) && (Start1 < (Start2 + Length2))) ||
      (((Start1 + Length1) > Start2) && ((Start1 + Length1) <= (Start2 + Length2))) ||
      ((Start2 >= Start1) && (Start2 < (Start1 + Length1))) ||
      (((Start2 + Length2) > Start1) && ((Start2 + Length2) <= (Start1 + Length1)))
      ) {
    return TRUE;
  }

  return FALSE;
}


EFI_STATUS
LegacyRegionAccessControl (
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  IN  BOOLEAN3                            EnableRead,
  IN  BOOLEAN3                            EnableWrite,
  OUT UINT32                              *Granularity OPTIONAL
  )
/*++

Routine Description:
  Modify PAM registers for region specified.
  This function is written in a chipset inspecific way.
  All it assumes is that the region control registers are
  in the PCI config space on bus 0. device 0, function 0.
  Specific bytes and bits covering the specific regions are
  defined in a global variable mPamData.
  Porting of this function should be limited to only
  updating entries in the mPamData.

Arguments:

  Start        -  Start of the region to control access to.
  Length       -  Lenght  of the region to control access to.
  EnableRead   -  Controls if Read asscess should be enabled/disabled/untouched.
  EnableWrite  -  Controls if Write asscess should be enabled/disabled/untouched.
  Granularity  -  Granularity of region in bytes.

Returns:

  EFI_SUCCESS  -  Legacy BIOS protocol installed.

--*/
{
  UINT8                           Idx;
  UINT8                           Reg;
  UINT8                           i;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *Pci;
  BOOLEAN                         Intersects;
  UINT32                          TempGranularity;
  BOOLEAN                         Contains;
  UINT64                          RegOffset;  

  Pci             = mPrivateData.PciRootBridgeIo;
  TempGranularity = 0;

  //
  // Iterate regions controlled by individual PAM registers.
  //
  for (Idx = 0; Idx < PAM_DATA_NO; Idx++) {
    //
    // Test if the ranges intersect and/or contain each other.
    //
    Intersects = LegacyRegionRangeIntersectsRange (
                  Start,
                  Length,
                  mPamData[Idx].Start,
                  mPamData[Idx].Length
                  );

    //
    // Non-intersection means non-relevance.
    //
    if (!Intersects) {
      continue;
    }
    //
    // Save the max length of the region for the granularity
    //
    if (mPamData[Idx].Length > TempGranularity) {
      TempGranularity = mPamData[Idx].Length;
    }

    Contains = LegacyRegionRangeContainsRange (
                Start,
                Length,
                mPamData[Idx].Start,
                mPamData[Idx].Length
                );

    //
    // Read the PAM reg for the iterated region from the first available populated CPU socket 
    //
    RegOffset = mPamData[Idx].RegOffset;
    if (mPrivateData.SocketPopulated[0]) {
        RegOffset |= ((UINT64)mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[0].BusNum <<24);
    } else {
        RegOffset |= ((UINT64)mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[1].BusNum <<24);         // Move to Socket 1 processor if Socket0 not populated!
    }

    Pci->Pci.Read (
              Pci,
              EfiPciWidthUint8,
              RegOffset,
              1,
              &Reg
              );

    //
    // Enable/Disable Read/Write region attributes.
    // The enable will occur if the requested region
    // intesects with the region controlled by this PAM.
    // The disable will occur only if the requested region
    // is contained within the region controlled by this PAM.
    //
    if (EnableRead == Bool3False) {

      if (Contains) {
        //
        // Disables Read.
        //
        Reg &= ~mPamData[Idx].Read;

      }

    } else if (EnableRead == Bool3True) {
      //
      // Enables Read.
      //
      Reg |= mPamData[Idx].Read;

    }

    if (EnableWrite == Bool3False) {

      if (Contains) {
        //
        // Disables Write.
        //
        Reg &= ~mPamData[Idx].Write;

      }

    } else if (EnableWrite == Bool3True) {
      //
      // Enables Write.
      //
      Reg |= mPamData[Idx].Write;

    }

    for (i=0; i < MAX_SOCKET; i++){
      //  Program PAM value in Socket i processor if populated!
      if (mPrivateData.SocketPopulated[i]) {
          Pci->Pci.Write (
                    Pci,
                    EfiPciWidthUint8,
                    mPamData[Idx].RegOffset | ((UINT64)mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[i].BusNum <<24),
                    1,
                    &Reg
                    );
      }
    }
  }

  if (Granularity != NULL) {
    *Granularity = TempGranularity;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LegacyRegionDecode (
  IN  EFI_LEGACY_REGION2_PROTOCOL          *This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                             *Granularity,
  IN  BOOLEAN                             *On
  )
/*++
Routine Description:

  Enable/Disable decoding of the given region.

Arguments:

  This    -  EFI_LEGACY_REGION2_PROTOCOL
  Start   -  Starting address of region.
  Length  -  Length of region in bytes.
  On      -  0 = Disable decoding, 1 = Enable decoding.

Returns:

  EFI_SUCCESS  -  Decoding change affected.
  Other        -  Decoding unmodified.

--*/
{
  if ((Start < 0xC0000) || ((Start + Length - 1) > 0xFFFFF)) {
    return EFI_INVALID_PARAMETER;
  }

  
  

  return EFI_SUCCESS;

}

EFI_STATUS
EFIAPI
LegacyRegionLock (
  IN  EFI_LEGACY_REGION2_PROTOCOL          * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  )
/*++

Routine Description:
  Make the indicated region read from ROM /write to ROM.

Arguments:

  This         -  EFI_LEGACY_REGION2_PROTOCOL.
  Start        -  Starting address of region.
  Length       -  Length of region in bytes.
  Granularity  -  Granularity of region in bytes.

Returns:

  EFI_SUCCESS  -  Region locked or made R/O.

--*/
{
    if ((Start < 0xC0000) || ((Start + Length - 1) > 0xFFFFF)) {
    return EFI_INVALID_PARAMETER;
  }


  //
  // Disable write, do not touch read.
  //
  return LegacyRegionAccessControl (Start, Length, Bool3DontKnow, Bool3False, Granularity);
}

EFI_STATUS
EFIAPI
LegacyRegionBootLock (
  IN EFI_LEGACY_REGION2_PROTOCOL           * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  )
/*++

Routine Description:
  Make the indicated region write protected.
  In addition to LegacyRegionLock(), this function assures 
  that the address is not aliased to other addressable space.

Arguments:

  This         -  Add argument and description to function.
  Start        -  Starting address of region.
  Length       -  Length of region in bytes.
  Granularity  -  Granularity of region in bytes.

Returns:

  EFI_SUCCESS  -  Region locked or made R/O.

--*/
{
  if ((Start < 0xC0000) || ((Start + Length - 1) > 0xFFFFF)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Disable write, do not touch read.
  //
  return LegacyRegionAccessControl (Start, Length, Bool3DontKnow, Bool3False, Granularity);
}

EFI_STATUS
EFIAPI
LegacyRegionUnlock (
  IN  EFI_LEGACY_REGION2_PROTOCOL          * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  )
/*++

Routine Description:
  Make the indicated region read from RAM /write to RAM.

Arguments:

  This         -  Pointer to Efi legacy region protocol interface instance.
  Start        -  Starting address of region.
  Length       -  Length of region in bytes.
  Granularity  -  Granularity of region in bytes.

Returns:

  EFI_SUCCESS  -  Region unlocked or made R/W.

--*/
{
  if ((Start < 0xC0000) || ((Start + Length - 1) > 0xFFFFF)) {
    return EFI_INVALID_PARAMETER;
  }


  //
  // Enable write, enable read.
  //
  return LegacyRegionAccessControl (Start, Length, Bool3True, Bool3True, Granularity);
}

EFI_STATUS
EFIAPI
LegacyRegionInstall (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description:
  Install Driver to produce Legacy Region protocol.

Arguments:

  ImageHandle  -  The image handle.
  SystemTable  -  The system table.

Returns:

  EFI_SUCCESS  -  Legacy Region protocol installed.
  Other        -  No protocol installed, unload driver.

--*/
{
  UINT8                       Index;
  EFI_STATUS                  Status;


  Status = gBS->LocateProtocol (
                &gEfiPciRootBridgeIoProtocolGuid,
                NULL,
                &mPrivateData.PciRootBridgeIo
                );
  ASSERT_EFI_ERROR (Status);

  //
  // Located the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (
                &gEfiIioUdsProtocolGuid,
                NULL,
                &mIioUds
                );
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < MAX_SOCKET; Index++) {
      mPrivateData.SocketPopulated[Index] = (UINT8)mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Index].Valid;
      mPrivateData.CpuSocketId[Index] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Index].SocId;
  }

  mPrivateData.Signature              = LEGACY_REGION_INSTANCE_SIGNATURE;
  mPrivateData.Handle                 = NULL;
  mPrivateData.ImageHandle            = ImageHandle;
  mPrivateData.LegacyRegion.Decode    = LegacyRegionDecode;
  mPrivateData.LegacyRegion.UnLock    = LegacyRegionUnlock;
  mPrivateData.LegacyRegion.Lock      = LegacyRegionLock;
  mPrivateData.LegacyRegion.BootLock  = LegacyRegionBootLock;
  mPrivateData.LegacyRegion.GetInfo   = LegacyRegionGetInfo;

  //
  // Install the protocol on a new handle.
  //
  Status = gBS->InstallProtocolInterface (
                &mPrivateData.Handle,
                &gEfiLegacyRegion2ProtocolGuid,
                EFI_NATIVE_INTERFACE,
                &mPrivateData.LegacyRegion
                );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
