/*++

Copyright (c) 1999 - 2012, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PcieSlotOpromLib.h
  

--*/

#ifndef __PCIE_SLOT_OPROM_LIB__
#define __PCIE_SLOT_OPROM_LIB__

#include <Platform.h>
#include <Protocol\PciRootBridgeIo.h>
#include <Library\HobLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid\HobList.h>
#include <Guid\PlatformInfo.h>
#include <IndustryStandard\Pci.h>

typedef union {
  
  UINT32 PcieSlotInfo;
  
  struct {
    UINT32 RtPrtFunNum : 8;
    UINT32 RtPrtDevNum : 8;
    UINT32 CpuNum : 8;
    UINT32 Dummy : 8;
    /* Slot definition for the root ports of Grantley CRBs
     */
  } RootPrt;  
} PLATFORM_PCIE_SLOT_STRUCT;

BOOLEAN
EFIAPI
PcieRootPortCheck (
  IN  UINTN                 Bus,
  IN  SYSTEM_CONFIGURATION  *mSystemConfiguration
  );
/*++

Routine Description:
  
  Check the device connect to PCI-E slot or not.

Arguments:

  Bus                   -  PCI Bus number.
  mSystemConfiguration  -  Setup data.

Returns:
  
  TRUE    -  This device on the PCI-E slot and it's Oprom not be run.
  FALSE   -  We wiil run the Oprom from device.

--*/
  
VOID
EFIAPI
SetupPcieSlotNumber (
  IN  SYSTEM_CONFIGURATION  *mSystemConfiguration
  );
/*++

Routine Description:
  
  Count of the PCI-E slot by CRB.

Arguments:

  mSystemConfiguration  -  Setup data.

Returns:
  
  VOID.

--*/

#endif

