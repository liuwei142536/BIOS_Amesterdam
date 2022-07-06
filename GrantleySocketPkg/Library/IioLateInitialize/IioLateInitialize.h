//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioLateInitialize.h

Abstract:

  This file defines everything that's needed for firmware to access
  and manipulate the Intel(R) silicon chip.
  All registers, bits, constants, etc. that are described in the chip's
  EDS are represented here.
  This file usurps all defines that relate to this chip. This means that
  it is inappropriate to define any of such define in any other file.
  Although this file maybe incomplete while under construction (maybe it
  will never be completed as there may be no need for access to all
  features), any new define must go here and follow naming conventions
  set therefore.
  In an ideal world, this file dould originate from the hardware design
  team that designed the chip.

--*/
#ifndef _IIO_LATE_INITIALIZE_H_
#define _IIO_LATE_INITIALIZE_H_

#include <PiDxe.h>
#include <IndustryStandard\Pci.h>
#include <Library/PciExpressLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/OemIioInit.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/OemIioInit.h>
#include <Library/IioLateInitializeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Protocol/Iiosystem.h>
#include <Protocol/ReadyForLock.h>
#include <Protocol/ExitPmAuth.h>
#include <Protocol/IioUds.h>
#include <Iio/IioRegs.h>
#include <Iio/MmioAccess.h>
#include <Iio/PciExpress.h>
#include <Library/UefiLib.h>
#include "IioLateInitialize.h"
#include "SysFunc.h"
#include <Protocol/MpService.h>

extern EFI_GUID gEfiEventExitBootServicesGuid;


VOID
DmiInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                           PciAddress,
  IIO_INIT_PHASE                                        Phase,
  IIO_GLOBALS                                               *IioGlobalData
  );

VOID
IioIoApicInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                       PciAddress,
  IIO_INIT_PHASE                                    Phase,
  IIO_GLOBALS                                       *IioGlobalData
  );

VOID
PcieEarlyInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS           PciAddress,
  IIO_INIT_PHASE                                        Phase,
  IIO_GLOBALS                                               *IioGlobalData
  );

VOID
PciePortInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                           PciAddress,
  IIO_INIT_PHASE                                        Phase,
  IIO_GLOBALS                                           *IioGlobalData
  );

VOID
IioWorkAround (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData,
    IIO_INIT_ENUMERATION                                InitPhase,
    VOID                                                *Buffer
    );

VOID
IioCpuWorkAround (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData,
    IIO_INIT_ENUMERATION                                InitPhase,
    VOID                                                *Buffer
  );

EFI_STATUS
VtdInitialization (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                           PciAddress,
  IIO_INIT_PHASE                                        Phase,
  IIO_GLOBALS                                           *IioGlobalData
);

VOID
IioDfxInit(
    PCI_ROOT_BRIDGE_PCI_ADDRESS                        PciAddress,
    IIO_INIT_PHASE                                     Phase,
    IIO_GLOBALS                                        *IioGlobalData
  );


VOID
NtbInit(
    IIO_GLOBALS                   *IioGlobalData,
    EFI_HANDLE   DriverImageHandle
  );

VOID
IioIoatInit( 
    IIO_GLOBALS                                    *IioGlobalData,
    UINT8                                          Iio,
    UINT8                                          CbDevNo,
    PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress
);

VOID 
IioPcieDeviceHide(  
  IIO_GLOBALS                           *IioGlobalData
  );

VOID
IioMiscInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                   PciAddress,
    IIO_INIT_PHASE                                Phase,
    IIO_GLOBALS                                   *IioGlobalData
);

VOID
PrintIioPciePortPcieLinkStatus(
  IIO_GLOBALS *IioGlobalData
  );

VOID
IioPortPowerDown(
  IIO_GLOBALS *IioGlobalData
  );

VOID
DmiPostInit (
    IIO_GLOBALS                                     *IioGlobalData
  );

VOID
IioIoatInitBootEvent(
  IIO_GLOBALS                                       *IioGlobalData,
  UINT8                                             Iio,
  IIO_INIT_PHASE                                    Phase
);

VOID
EarlyIoatInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                     PciAddress,
    IIO_INIT_PHASE                                  Phase,
    IIO_GLOBALS                                     *IioGlobalData
);

VOID
IoApicPostInit(
    IIO_GLOBALS                                     *IioGlobalData
);

VOID
IioIoApicInitBootEvent(
    IIO_GLOBALS         *IioGlobalData,
    UINT8           Iio
);

BOOLEAN
IsPciePortDLActive(
    IIO_GLOBALS                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
);

BOOLEAN
IioVData_ConfigurePciePortLate(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    );

BOOLEAN
IsIioPortsPopulated(
    IIO_GLOBALS                *IioGlobalData,
    UINT8                      Iio
   );

UINT8
CheckVarPortLinkSpeedLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
    );

UINT8
CheckVarPortASPMLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
    );

BOOLEAN
ChangeVarPortLinkSpeedLate(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
    );

BOOLEAN
CheckVarPortEnableLate(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
  );

BOOLEAN
IsThisUplinkPortLate(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

UINT8
GetAssignedPortIndexWidthLate(
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                             Iio,
  UINT8                             PortIndex
);

VOID S3_PciExpressWrite32( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT32    	Data
  );
  
VOID S3_PciExpressWrite16( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT16    	Data
  );

VOID S3_PciExpressWrite8( 
  IN  IIO_GLOBALS       *IioGlobalData,
  IN  UINT64    	Address,
  IN  UINT8    	Data
  );
  

VOID
InitPortClock (
    IIO_GLOBALS                                   *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS                   PciAddress,
    UINT8                                         PcieSlotClockSource,
    UINT8                                         PciePortClockConfiguration
);

VOID
PcieMiscInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS   *IioGlobalData
);

VOID
IioPortsPreBootInit(
  IN  IIO_GLOBALS   	*IioGlobalData,
  IN  UINT8				Iio,
  IN  IIO_INIT_PHASE	Phase
);

VOID
SaveCbDmaDeviceBaseAddress(
    IN  IIO_GLOBALS                                   *IioGlobalData,
    IN  UINT8                                         Iio,
    IN  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress
);

#ifndef DE_SKU
VOID
IioIoatFlushAll(
  IIO_GLOBALS                                   *IioGlobalData,
  UINT8                                         Iio,
  UINTN                                         Phase
);

VOID
IioIoatBreakpoint(
    IIO_GLOBALS   *IioGlobalData,
    UINTN         br
);

VOID
IioIoatReset(
    IIO_GLOBALS   *IioGlobalData
);

BOOLEAN
Apply_s328_WA(
    IIO_GLOBALS   *IioGlobalData
);

BOOLEAN
IioIoat_s328_WA_skip_GT1(
    IIO_GLOBALS   *IioGlobalData
);

VOID
IioIoat_s328_WA(
    IIO_GLOBALS   *IioGlobalData
);
#endif
#endif
