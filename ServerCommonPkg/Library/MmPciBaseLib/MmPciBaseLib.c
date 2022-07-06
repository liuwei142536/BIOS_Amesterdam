/** @file
  This file contains routines that get PCI Express Address

@copyright
 Copyright (c) 2013 - 2014 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>

UINTN mMmPciBaseLibPcieBase = 0;

#define PCI_INDXE_PORT  0xCF8
#define PCI_DATA_PORT   0xCFC

/**
  This procedure will get PCIE address
  
  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number

  @retval PCIE address
**/
UINTN
MmPciBase (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
)
{
  BOOLEAN InterruptState;
  UINTN   PcieAddress;

  ASSERT ((Bus <= 0xFF) && (Device <= 0x1F) && (Function <= 0x7));

  PcieAddress = mMmPciBaseLibPcieBase;
  if (PcieAddress == 0) {
    InterruptState = SaveAndDisableInterrupts ();
        //
        // Get MmcfgBase as in PurleySocketPkg
        //
#if defined(IA32)
        OutPort32 (NULL, 0x0cf8, (0x80000000 | (Bus << 16) |  (5 << 11)  | (0 << 8) | 0x90));
        PcieAddress = InPort32 (NULL, 0x0cfc);
#else
        IoWrite32 (0x0cf8, (0x80000000 | (Bus << 16) |  (5 << 11)  | (0 << 8) | 0x90));
        PcieAddress = IoRead32 (0x0cfc);
#endif
    SetInterruptState (InterruptState);

    mMmPciBaseLibPcieBase = PcieAddress;
  }
  //
  // Use local variable PcieAddress here. This library is for PEI\DXE\SMM
  // When PEI, the module variable can't be updated. Need a local variable to get the pcie base address + offset.
  //
  PcieAddress += (UINTN) ((Bus << 20) + (Device << 15) + (Function << 12));

  //DEBUG ((DEBUG_INFO, "MM PCI Address = 0x%08x\n", PcieAddress));
  return PcieAddress;
}
