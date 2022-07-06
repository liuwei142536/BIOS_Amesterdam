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

    OemIioInit.h

Abstract:

    Oem Hooks file for UncoreInit Module

--*/
#ifndef _IIO_OEMINITIALIZE_H_
#define _IIO_OEMINITIALIZE_H_

#include <Token.h> // AptioV server override

#include <Library/IioPlatformData.h>
#include "IioSetupDefaults.h"
#include "IioAccess.h"
#include <Setup/IioUniversalData.h>
#include <Library/IioEarlyInitializeLib.h>
#ifndef IA32
#include <PiDxe.h>
#include <Protocol/PciCallback.h>
#include <IndustryStandard\Pci.h>
#endif // IA32
#include "SysFunc.h"


#define  IN_DXE_PHASE    0
#define  IN_PEI_PHASE    1

/*
 * Table determining the primary port index for every port in
 * the IOU bifurcation map
 */
static UINT8 IOU2_PrimaryPortIndexMap[IOU2_BIF_RANGE][IOU2_MAX_PORTS] = { //Table for IOU2 cluster
    {PORT_1A_INDEX, PORT_1B_INDEX}, //x4x4 (operate lanes 7:4 as x4, 3:0 as x4)
    {PORT_1A_INDEX, PORT_1A_INDEX}, //x8
};

static UINT8 IOU0_PrimaryPortIndexMap[x16_IOU_BIF_RANGE][x16_IOU_MAX_PORTS] = { //Table for IOU0
    // x4x4x4x4 (operate lanes 15:12 as x4, 11:8 as x4, 7:4 as x4 and 3:0 as x4)
    {PORT_2A_INDEX, PORT_2B_INDEX, PORT_2C_INDEX, PORT_2D_INDEX},

    // x4x4x8 (operate lanes 15:12 as x4, 11:8 as x4 and 7:0 as x8)
    {PORT_2A_INDEX, PORT_2A_INDEX, PORT_2C_INDEX, PORT_2D_INDEX},

    // x8x4x4 (operate lanes 15:8 as x8, 7:4 as x4 and 3:0 as x4)
    {PORT_2A_INDEX, PORT_2B_INDEX, PORT_2C_INDEX, PORT_2C_INDEX},

    // x8x8 (operate lanes 15:8 as x8, 7:0 as x8)
    {PORT_2A_INDEX, PORT_2A_INDEX, PORT_2C_INDEX, PORT_2C_INDEX},

    // x16
    {PORT_2A_INDEX, PORT_2A_INDEX, PORT_2A_INDEX, PORT_2A_INDEX},
};

static UINT8 IOU1_PrimaryPortIndexMap[x16_IOU_BIF_RANGE][x16_IOU_MAX_PORTS] = { //Table for IOU1
    // x4x4x4x4 (operate lanes 15:12 as x4, 11:8 as x4, 7:4 as x4 and 3:0 as x4)
    {PORT_3A_INDEX, PORT_3B_INDEX, PORT_3C_INDEX, PORT_3D_INDEX},

    // x4x4x8 (operate lanes 15:12 as x4, 11:8 as x4 and 7:0 as x8)
    {PORT_3A_INDEX, PORT_3A_INDEX, PORT_3C_INDEX, PORT_3D_INDEX},

    // x8x4x4 (operate lanes 15:8 as x8, 7:4 as x4 and 3:0 as x4)
    {PORT_3A_INDEX, PORT_3B_INDEX, PORT_3C_INDEX, PORT_3C_INDEX},

    // x8x8 (operate lanes 15:8 as x8, 7:0 as x8)
    {PORT_3A_INDEX, PORT_3A_INDEX, PORT_3C_INDEX, PORT_3C_INDEX},

    // x16
    {PORT_3A_INDEX, PORT_3A_INDEX, PORT_3A_INDEX, PORT_3A_INDEX},
};


#ifdef DE_SKU
// DE & NS Storage Sku - CBDMA on D2:F0
// NS Comms Sku - CPM on D2:F0
#define IS_BDX_DE_IOSF_CBDMA_CPM_DEVICE_FUNCTION( Device , Function ) \
  ( \
    (Device == PCIE_PORT_2_DEV) && (Function == PCIE_PORT_2A_FUNC)   /*D2:F0*/  \
  )

#define IS_BDX_DE_IOSF_GBE_DEVICE_FUNCTION( Device , Function ) \
  ( \
    (Device == PCIE_PORT_2_DEV) && (Function == PCIE_PORT_2C_FUNC)   /*D2:F2*/  \
  )

// NS Comms Sku - GbE1 on D2:F3; Storage Sku - CPM on D2:F3
#define IS_BDX_DE_IOSF_CPM_GBE1_DEVICE_FUNCTION( Device , Function ) \
  ( \
    (Device == PCIE_PORT_2_DEV) && (Function == PCIE_PORT_2D_FUNC)   /*D2:F3*/  \
  )

#endif // DE_SKU

VOID
IioStall (
  IIO_GLOBALS  *IioGlobalData,
  UINT32   UsDelay
  );

VOID
OemGetIioPlatformInfo(
  IIO_GLOBALS *IioGlobalData
);

VOID
OemVariableInitPerSocket (
 IIO_GLOBALS                *IioGlobalData,
 UINT8                      IioIndex
  );

VOID
OemVariableGetCurrent (
  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemIioPreLinkDataInitPerSocket(
  IIO_GLOBALS       *IioGlobalData,
  UINT8        Iio
);

VOID
IioInitDriverInstalled (
  IIO_GLOBALS  *IioGlobalData
);

VOID
OemIioInitDriverInstalled (
  IIO_GLOBALS  *IioGlobalData
);

VOID
OemDmiPreInit (  
   PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
  IIO_GLOBALS  *IioGlobalData
  );

VOID
OemDmiDeviceInit (  
   PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress,
  IIO_GLOBALS  *IioGlobalData
  );


VOID
OemDmiResourceAssignedInit (
   PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
   IIO_GLOBALS   *IioGlobalData
  );

#ifndef IA32
VOID
OemIioInitCallbackEndPoint (  
    PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress,
    EFI_PCI_ENUMERATION_PHASE                     Phase,
    IIO_GLOBALS                                   *IioGlobalData
);
#endif // IA32

VOID
OemIioErrorEnable(
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
);


VOID
OemIioPortDisable (
  UINT8       Iio,
  IIO_GLOBALS   *IioGlobalData,
  UINT32    *IioPortDisableBitList
);

VOID
OemIioUplinkPortDetails(
  UINT8   Iio,
  UINT8   *PortIndex,
  UINT8   *Bus,
  UINT8   *Dev,
  UINT8   *Func
);

#ifndef IA32
EFI_STATUS
EFIAPI
OemAllocate32BitMmio(
    IIO_GLOBALS            *IioGlobalData,
    UINT8                  Socket,
    EFI_HANDLE             ImageHandle,
    UINTN                  Alignment,
    UINT64                 Length,
    EFI_PHYSICAL_ADDRESS   *BaseAddress
);

EFI_STATUS
EFIAPI
OemFreeMemorySpace (
  EFI_PHYSICAL_ADDRESS  BaseAddress,
  UINT64                Length
);

EFI_STATUS
EFIAPI
OemAllocateSystemMemory(
    IIO_GLOBALS            *IioGlobalData,
    EFI_HANDLE             ImageHandle,
    UINTN                  Alignment,
    UINT64                 Length,
    EFI_PHYSICAL_ADDRESS   BelowRegion,
    VOID   **BaseAddress
);

EFI_STATUS
EFIAPI
OemFreeSystemMemorySpace (
    EFI_PHYSICAL_ADDRESS  BaseAddress,
    UINT64                Length
);
#endif // IA32

IIO_STATUS
OemGetCTLEInfo (
  IIO_GLOBALS  *IioGlobalData,
  UINT8         Iio,
  UINT8         Dev,
  UINT8         Func,
  UINT32        *OrValueCtle_0,
  UINT32        *OrValueCtle_1,
  UINT32        *AndValueCtle_0,
  UINT32        *AndValueCtle_1,
  UINT8         LinkSpeed
  );


/*---------------------------------------------------------------------------*/
/*                      OEM hooks for PciHostBridge driver                   */
/*---------------------------------------------------------------------------*/
UINT8
CheckSoftSimulationPlatform(
VOID
);

#ifndef IA32
EFI_STATUS
OemCheckMmio64Enabled(
  BOOLEAN    *Is64bitEnabled
);

EFI_STATUS
OemEnableMmio64bitAllocation(
    VOID
);
#endif //IA32


// AptioV Server Override Start: Commenting the function as it is not required. We are handling it in OemEarlyUncoreInit() function.
/*VOID
OemUpdateUdsSetupToSystemConfig(
    SETUP_INFO                            *SetupInfo
);*/
// AptioV Server Override End: Commenting the function as it is not required. We are handling it in OemEarlyUncoreInit() function.
VOID
OemInitializeIioInputData (
  IIO_GLOBALS  *IioGlobalData
);

VOID
OemUpdateDmiVcxSettings(
  IIO_GLOBALS *IioGlobalData
);

BOOLEAN
OemIdentifyIioPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyDmiPort(
   IIO_GLOBALS   *IioGlobalData,
   UINT8         Bus,
   UINT8         Device,
   UINT8         Function
);

BOOLEAN
OemIdentifyDmiDfxPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         DfxDevice,
  UINT8         DfxFunction
);

BOOLEAN
OemIsDmiVcpEnable(
   IIO_GLOBALS *IioGlobalData
);

BOOLEAN
OemIdentifyPCIeRootPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyPCIeNTBPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyVendorSpecificPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function,
  BOOLEAN       DfxArguments
);

UINT32
OemGetPchRootComplexBaseAddress (
    IIO_GLOBALS   *IioGlobalData
);

BOOLEAN
OemSetPchDmiLinkSpeed (
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress,
  IIO_GLOBALS                   *IioGlobalData,
  UINT8                         LinkSpeed
);

BOOLEAN
OemOverrideMiscCtlep(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         BusNnmber,
  UINT8         Device,
  UINT8         Function
);

#ifdef  DE_SKU
BOOLEAN
OemIdentifyVendorSpecificPort_CBDMA_CPM(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyVendorSpecificPort_GbE(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyVendorSpecificPort_CPM_GbE1(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemRetrieve_IOSF_CBDMA_CPM_PciAddress(
   IIO_GLOBALS                                    *IioGlobalData,
   PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress,
   PCI_ROOT_BRIDGE_PCI_ADDRESS    *CbdmaPciAddress
);

BOOLEAN
OemRetrieve_IOSF_GbE_PciAddress(
  IIO_GLOBALS                                    *IioGlobalData,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    *GbePciAddress
);

BOOLEAN
OemRetrieve_IOSF_CPM_GbE1_PciAddress(
  IIO_GLOBALS                                    *IioGlobalData,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    *CpmPciAddress
);
#endif  //DE_SKU

BOOLEAN
OemIdentifyLegacyCbDmaDevice(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

BOOLEAN
OemIdentifyCbDmaDevice(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
);

UINT8
OemGetVppInvertRegValue(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Iio
);

#ifdef IA32
VOID
OemPchDmiTcVcMapInit(
 IIO_GLOBALS               *IioGlobalData
);


VOID
OemDmiTcVcEnablePoll(
  IIO_GLOBALS *IioGlobalData
);

VOID
OemDmiTcVcProg(
IIO_GLOBALS *IioGlobalData
);

VOID
OemDmiGen2Prog(
  IIO_GLOBALS *IioGlobalData
);
#endif // IA32
/*---------------------------------------------------------------------------*/

#pragma pack (push, 1)
typedef struct {
    UINT32                          ErrorStatus;
    UINT32                          NumberFailures;
} VALIDATION_CSI_ERROR_STRUCT;

typedef struct {
    UINT32                          ErrorStatus;
    UINT32                          NumberFailures;
} VALIDATION_PCIE_ERROR_STRUCT;


typedef struct {
    UINT16                          CounterOuter;       // Counter for number of Outer boot cycles that have occurred
    UINT16                          CounterInner;       // Counter for number of Inner boot cycles that have occurred
    UINT32                          TotalResets;        // Counter for total reset cycles completed
    UINT16                          MaxCountOuter;      // Requested number of cycles for Outer counter 
    UINT16                          MaxCountInner;      // Requested number of cycles for Inner counter 
    UINT8                           CsiCclkCurrent;     // Cclk value currently being tested
    UINT8                           OriginalLinkSpeed;  // Setup value for LinkSpeed at start of reboot cycling
    UINT8                           OriginalFreqSelect; // Setup value for FreqSelect at start of reboot cycling
    UINT8                           TestExecuted;       // Reset test executed that generated this data
    BOOLEAN                         ForceOutputDisplay; // Forces display of output on next boot
    BOOLEAN                         UserBreakout;       // 1-- Forces reenterable exit from testing, 0xFF -- Abandon testing
    BOOLEAN                         DataValid;          // method for later invalidating the data if needed
    BOOLEAN                         ContinueCycling;    // TRUE/FALSE means CONTINUE/STOP Cycling
    BOOLEAN                         CounterExpired;     // Required for multiple loop first loop counter complete status
    VALIDATION_CSI_ERROR_STRUCT     CsiError;           // CSI error definitions to be used when reporting out errors
    VALIDATION_PCIE_ERROR_STRUCT    PcieError;          // PCIe error definitions to be used when reporting out errors
} VALIDATION_RESET_STRUCT;
#pragma pack (pop)

/*
 * Following are OEM hooks to initialize platform specofoc data for each socket at a time
 */

#endif
