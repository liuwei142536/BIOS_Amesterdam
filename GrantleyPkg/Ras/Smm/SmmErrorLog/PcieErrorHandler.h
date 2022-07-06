/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2007 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PcieErrorHandler.h

Abstract:

  Header file for Whea generic implementation for error logging.

-----------------------------------------------------------------------*/

#ifndef _PCIE_ERROR_HANDLER_H_
#define _PCIE_ERROR_HANDLER_H_

#include <Uefi.h>
#include "PchRegs.h"
#include <Iio/PciExpress.h>
#include <Protocol/WheaPlatformSupport.h>

//
// 2.5.2. Advanced Error Reporting Enhanced Capability Registers...
//
#define R_PCIE_EXT_CAP_HEADER                       0x148 // Aptio V Server Override : CAP HEADER offset (0x100) is not valid 
//
// Invalid header, used for terminating list...
//
#define PCIE_EXT_CAP_HEADER_INVALID                 0xFFFF

#define EFI_PCI_BRIDGE_DEV_CLASS_CODE               0x06040000
#define EFI_PCI_BRIDGE_SUBTRACTIVE_DEV_CLASS_CODE   0x06040100
//
// Constants...
//
#define PCIE_CORRECTABLE_ERROR_COUNT_LIMIT    10

#define EFI_PCI_CAPABILITY_ID_PCIEXP  0x10

//
// Following are the PCI status control bit
//
//#define EFI_PCI_STATUS_CAPABILITY             0x0010

#define PCI_PRIMARY_STATUS_OFFSET                   0x06
#define PCI_COMMAND_OFFSET                          0x04

//#define EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE    0x0001
//#define EFI_PCI_BRIDGE_CONTROL_SERR                     0x0002
//#define EFI_PCI_COMMAND_PARITY_ERROR_RESPOND            0x0040
//#define EFI_PCI_COMMAND_SERR                            0x0100

#define   B_PCIE_PORT_XPCORERRSTS_LBS               BIT1
#define   V_PCIE_PORT_XPUNCERRMSK                   (BIT1 + BIT3 + BIT4 + BIT5 + BIT6 + BIT8 + BIT9)
#define   V_PCIE_PORT_UNCEDMASK                     (BIT4 + BIT5 + BIT12 + BIT13 + BIT14 + BIT15 + BIT16 + BIT17 + BIT18 + BIT20 + BIT21)
#define PCI_BRIDGE_CONTROL_REGISTER_OFFSET          0x3E
#define PCI_BRIDGE_STATUS_REGISTER_OFFSET           0x1E
#define PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET    0x19
#define PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET  0x1a
#define PCI_MAX_DEVICE  31
#define PCI_MAX_FUNC    7
#define EFI_PCI_CAPABILITY_PTR              0x34
#define PCIE_EXT_CAP_HEADER_AERC          0x0001
#define PCIE_EXT_CAP_HEADER_LERC          0x0004
#define PCIE_EXT_CAP_HEADER_LERC_REVISION 0x0002
#define PCIE_ENHANCED_CAPABILITY_PTR      0x0100

#define PCI_VENDOR_ID_OFFSET              0x00
#define PCI_REVISION_ID_OFFSET            0x08
#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)


typedef struct {
  UINT32                ExtCapId:16;
  UINT32                CapabilityVersion:4;
  UINT32                CapabilityOffset:12;
} PCIE_EXTENDED_CAPABILITY;


extern UINT8   mPciXPUnsupportedRequest;
extern UINT8   mPcieCorErrThreshold;
/*
#ifdef  CRB_FLAG
extern UINT32  MaxCardDataCount;
extern CARD_DATA   CardData[100];
#endif
*/
/***************************************************************************
                   PCI/PCIe Device Information
 ***************************************************************************/

BOOLEAN
PciErrLibIsDevicePresent (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks if a PCI or PCIe device exists at the specified PCI address.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - Device present at the specified address
    FALSE    - Device not present at the specified address

--*/
;

BOOLEAN
PciErrLibDeviceOnBus (
  IN UINT8   Bus
  )
/*++

Routine Description:
    This function checks if a PCI device is present on the specified bus.

Arguments:
    Bus      - PCI bus number to check
    
Returns: 
    TRUE     - A PCI device was found on the bus
    FALSE    - A PCI device was not found on the bus

--*/
;

VOID
PciErrLibEnableElogDeviceAndSubDevices (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function initializes and enables error reporting in the specified PCI or PCIe
    device and all subordinate PCI or PCIe devices.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    Nothing

--*/
;

VOID
PciErrLibEnableElogDevice (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function initializes and enables error reporting in the specified PCI or PCIe device.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    Nothing

--*/
;

VOID
PciErrLibClearLegacyPciDeviceStatus (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function clears the specified device's primary and secondary (if appropriate) 
    legacy PCI status registers.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    Nothing

--*/
;

VOID
PciErrLibEnableBridgeControl (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function enables SERR and Parity error reporting in the P2P bridge's
    bridge control register.

Arguments:
    Bus      - P2P Bridge Device's bus number
    Device   - P2P Bridge Device's device number
    Function - P2P Bridge Device's function number
    
Returns: 
    Nothing

--*/
;
VOID
PciErrLibEnableBridgeControl (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function enables SERR and Parity error reporting in the P2P bridge's
    bridge control register.

Arguments:
    Bus      - P2P Bridge Device's bus number
    Device   - P2P Bridge Device's device number
    Function - P2P Bridge Device's function number
    
Returns: 
    Nothing

--*/
;

BOOLEAN
PciErrLibIsPciBridgeDevice (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks if a PCI-to-PCI bridge exists at the specified PCI address.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - P2P present at the specified address
    FALSE    - P2P not present at the specified address

--*/
;

VOID
PciErrLibEnablePciSerrPerrGeneration (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function enables SERR and Parity error reporting in the specified device's 
    legacy PCI command register.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    Nothing

--*/
;

BOOLEAN
PciErrLibIsPcieDevice (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks if a specified device is a PCI Express device.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - PCIe device present at the specified address
    FALSE    - PCIe device not present at the specified address

--*/
;

UINT8 
PciErrLibGetCapabilitiesOffset (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function obtains the PCI configuration space register offset of the specified device's
    PCI Express Capabilities register set.  Note: this is not the offset to the Advanced
    Error Reporting register set.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    0        - PCIe capability ID not found
    non-zero - Offset of the PCIe capability register set

--*/
;

VOID
PciErrLibEnablePcieELogCapabilities (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function performs the following operations on the specified PCIe device:
      1.  Programs the Uncorrectable Error Mask register if applicable
      2.  Programs the Uncorrectable Error Severity register if applicable
      3.  Programs the Correctable Error Mask register if applicable
      4.  Clears the standard PCIe status registers
      5.  Disables legacy SERR reporting if appropriate (no AER registers)
      6.  Enables error reporting for root ports and regular devices

Arguments:
    Bus      - PCIe Device's bus number
    Device   - PCIe Device's device number
    Function - PCIe Device's function number
    
Returns: 
    Nothing

--*/
;
BOOLEAN
ElogIsIchPcieRootPort (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This checks if the specified device is an ESB2 PCIe Root Port.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number

Returns:
    TRUE     - Device is an ESB2 PCIe Root Port
    FALSE    - Device is not an ESB2 PCIe Root Port

--*/
;

VOID
PciErrLibClearPcieDeviceStatus (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function clears the legacy and standard PCI Express status registers for the
    specified PCI Express device.

Arguments:
    Bus      - PCIe Device's bus number
    Device   - PCIe Device's device number
    Function - PCIe Device's function number
    
Returns: 
    Nothing

--*/
;

BOOLEAN
PciErrLibIsRootPortErrReportingEnabled (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks to see if PCIe error reporting is enabled in the specified 
    PCIe root port device.

Arguments:
    Bus      - PCIe root port device's bus number
    Device   - PCIe root port device's device number
    Function - PCIe root port device's function number
    
Returns: 
    TRUE     - Error reporting is enabled
    FALSE    - Error reporting is not enabled or it is under OS control

--*/
;

BOOLEAN
PciErrLibIsErrOnPcieRootPort (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks the Root Error Status register to see if one of the enabled errors
    has occurred.

Arguments:
    Bus      - PCIe root port device's bus number
    Device   - PCIe root port device's device number
    Function - PCIe root port device's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected, or the device is not a root port

--*/
;


BOOLEAN
PciErrLibPcieRootPortErrorHandler (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    PCI Express error handler for ESB2 and BNB PCI Express Root Ports.

Arguments:
    Bus      - PCIe root port device's bus number
    Device   - PCIe root port device's device number
    Function - PCIe root port device's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
;

BOOLEAN
PciErrLibPcieDeviceErrorHandler (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function determines if the specified device is reporting an error and handles the
    error if one is detected.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
;

BOOLEAN
PciErrLibIsFatalPcieDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks the SERR and PCIe device status to see if a PCIe Uncorrectable Fatal
    error has occurred.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    TRUE     - A PCIe Uncorrectable Fatal error was detected
    FALSE    - A PCIe Uncorrectable Fatal error was not detected

--*/
;

BOOLEAN
PciErrLibIsCorrectablePcieDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks the PCIe device status and the correctable error registers to see 
    if a PCIe Correctable error has occurred.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    TRUE     - A PCIe Correctable error was detected
    FALSE    - A PCIe Correctable error was not detected

--*/
;

BOOLEAN
PciErrLibIsNonfatalPcieDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks the SERR and PCIe device status to see if a Uncorrectable Non-Fatal
    PCIe error has occurred.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    TRUE     - A PCIe Uncorrectable Non-Fatal error was detected
    FALSE    - A PCIe Uncorrectable Non-Fatal error was not detected

--*/
;

BOOLEAN
PciErrLibIsSubordinateDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function determines if there is a device reporting an error on the specified 
    P2P bridge's secondary bus or on a subordinate bus.

Arguments:
    Bus      - P2P bridge's bus number
    Device   - P2P bridge's device number
    Function - P2P bridge's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
;

BOOLEAN
PciErrLibIsPciDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks for errors on standard PCI devices.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
;

VOID
PciErrLibHandleUncorrectablePcieDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function handles PCIe Uncorrectable (Fatal and Non-Fatal) errors.
    An error is reported and the PCIe error detection status is updated.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    Nothing

--*/
;
VOID
PciErrLibHandleCorrectablePcieDeviceError (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function handles PCIe Correctable errors.
    An error is reported if the correctable error threshold has not been reached.
    If the threshold has been reached, correctable errors are masked in the device.

Arguments:
    Bus      - PCIe device's bus number
    Device   - PCIe device's device number
    Function - PCIe device's function number
    
Returns: 
    Nothing

--*/
;


BOOLEAN
PciErrLibPciDeviceErrorHandler (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks for, and handles, errors on standard PCI devices.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
;

VOID
PciErrLibReportError (
  IN UINT8                    Bus,
  IN UINT8                    Device,
  IN UINT8                    Function,
  IN UINT8                    PciErrorType
  )
/*++

Routine Description:
    This function logs the appropriate type of SEL error and updates the status
    of PCI error detection flags.

Arguments:
    Bus       - Device's bus number
    Device    - Device's device number
    Function  - Device's function number
    ErrorType - Type of the error to report (SERR/PERR/Correctable/...)

Returns: 
    Nothing

--*/
;

BOOLEAN
PciErrLibCheckAerCapability(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*
Routine Description:
    This function checks whether the PCIe device supports Advanced error capabaility(AER)

Arguments:
    Bus       - Device's bus number
    Device    - Device's device number
    Function  - Device's function number

Returns: 
  TRUE  - if device supports AER
  FALSE - if device doesn't support AER
*/
;

BOOLEAN
PCIeIsRootPort(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  );

UINT16
PciErrLibGetLERCapabilitiesOffset (
    IN UINT8   Bus,
    IN UINT8   Device,
    IN UINT8   Function
);

UINT16 
PciErrLibGetExtendedCapabilitiesOffset (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  ExtCapabilityId,
  IN UINT16  VendorSpecificId
  );

VOID
  PciErrLibClearDeviceStatus(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  );

#endif

