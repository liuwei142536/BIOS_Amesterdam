/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c) 2007 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PcieErrorHandler.c

Abstract:

  Whea generic implementation for error logging.

--*/
#include "CommonErrorHandlerIncludes.h"
#define SW_RAID_VID_DID 0x91301B4B
#define SW_RAID_ROOT_PORT_BUS_NUM 0
#define SW_RAID_ROOT_PORT_DEV_NUM 0x1C
#define SW_RAID_ROOT_PORT_FUN_NUM 0

BOOLEAN
IsSwRaidBehindBridge (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function checks if a SW RAID Device behind this bridge.

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number
    
Returns: 
    TRUE     - Device present at the specified address
    FALSE    - Device not present at the specified address

--*/
{
	  UINT8 SecondaryBusNumber;
	  UINT8 SubordinateBusNumber;
	  UINT8 SubBus;
	  UINT8 SubDev;
	  UINT8 SubFunc;
	  
	  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {

	    SecondaryBusNumber = PciExpressRead8 (
	                          PCI_PCIE_ADDR(Bus,
	                          Device,
	                          Function,
	                          PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
	                          ));

	    SubordinateBusNumber = PciExpressRead8 (
	                            PCI_PCIE_ADDR(Bus,
	                            Device,
	                            Function,
	                            PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET
	                            ));

	    if((SecondaryBusNumber ==0) && (SubordinateBusNumber == 0)) return FALSE;

	    for (SubBus = SecondaryBusNumber; SubBus <= SubordinateBusNumber; SubBus++) {
	      for (SubDev = 0; SubDev <= PCI_MAX_DEVICE; SubDev++) {
	        for (SubFunc = 0; SubFunc <= PCI_MAX_FUNC; SubFunc++) {
	          if ((PciExpressRead32 (PCI_PCIE_ADDR(SubBus,SubDev,SubFunc,0x0)))== SW_RAID_VID_DID) {
	        	  return TRUE;
	          }
	        }
	      }
	    }
	  }
	return FALSE;
}

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
{
  UINT16    Data16;

  //
  // Check if a device is present at this location...
  //
  Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_VENDOR_ID_OFFSET));
  if (Data16 == 0xFFFF) {
    return FALSE;
  }

  return TRUE;
}

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
{
  UINT8   Device;
  UINT8   Function;

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {

      if (PciErrLibIsDevicePresent (Bus, Device, Function)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

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
{
  UINT32  Data32;

  Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_REVISION_ID_OFFSET));
  Data32 &= 0xFFFFFF00;
  if ( (Data32 == EFI_PCI_BRIDGE_DEV_CLASS_CODE) || 
       (Data32 == EFI_PCI_BRIDGE_SUBTRACTIVE_DEV_CLASS_CODE ) ) {
    return TRUE;
  }

  return FALSE;
}

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
{
  UINT8   CapabilitiesOffset;

  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  return TRUE;
}

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
{
  if (Bus != DEFAULT_PCI_BUS_NUMBER_PCH) {
    return FALSE;
  }

  if (Device != PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS) {
    return FALSE;
  }

  if ((Function < PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1) ||
        (Function > PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8)) {
    return FALSE;
  }

  return TRUE;
}

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
{
  UINT8 SecondaryBusNumber;
  UINT8 SubordinateBusNumber;

  //
  // Enable error logging in the current device...
  //
  PciErrLibEnableElogDevice (Bus, Device, Function);

  //
  // Enable error logging in the devices behind a bridge...
  //

  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {

    SecondaryBusNumber = PciExpressRead8 (
                          PCI_PCIE_ADDR(Bus,
                          Device,
                          Function,
                          PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                          ));

    SubordinateBusNumber = PciExpressRead8 (
                            PCI_PCIE_ADDR(Bus,
                            Device,
                            Function,
                            PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET
                            ));

    if((SecondaryBusNumber ==0) && (SubordinateBusNumber == 00)) return;

    for (Bus = SecondaryBusNumber; Bus <= SubordinateBusNumber; Bus++) {
      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
          PciErrLibEnableElogDevice (Bus, Device, Function);
        }
      }
    }
  }


}

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
{
  if (PciErrLibIsDevicePresent (Bus, Device, Function)) {

    PciErrLibClearLegacyPciDeviceStatus (Bus, Device, Function);

    PciErrLibEnablePciSerrPerrGeneration (Bus, Device, Function);

    //
    // Check if this is a PCIe device...
    //

    if (PciErrLibIsPcieDevice (Bus, Device, Function)) {
      PciErrLibEnablePcieELogCapabilities (Bus, Device, Function);
    }

    if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {
      //
      // Enable SERR and PERR Error Logging in Bridge Control...
      // Serves both PCI and PCIe bridges...
      //
      PciErrLibEnableBridgeControl (Bus, Device, Function);
    }
  }
}

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
{
  UINT16  StatusReg16;

  StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET), StatusReg16);

  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {
    StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET));
    PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET), StatusReg16);
  }
}

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
{
  UINT8 Data8;

  Data8 = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET));
  //
  // Enable SERR if selected in Setup
  //
  if (mNmiOnSerr) {
    Data8 |= EFI_PCI_BRIDGE_CONTROL_SERR;
    //
    // enable PERR if selected in Setup
    //
    if (mNmiOnPerr) {
      Data8 |= EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE;
    } else {
      //
      // if PERR is disabled in Setup, disable PERR alone
      //
      Data8 &= ~EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE;
    }
  } else {
    //
    // if SERR is disabled in setup, disable both SERR and PERR generation
    //
    Data8 &= ~(EFI_PCI_BRIDGE_CONTROL_SERR | EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE);
  }

    // Aptio V Server Override Start : Board Override Disable PERR/SERR for Video Device.
    if ((Bus == ONBOARD_VIDEO_ROOT_PORT_BUS_NUM)  && (Device == ONBOARD_VIDEO_ROOT_PORT_DEV_NUM) && (Function == ONBOARD_VIDEO_ROOT_PORT_FUN_NUM)) {
        Data8 &= ~(EFI_PCI_BRIDGE_CONTROL_SERR | EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE);
    }
    // Aptio V Server Override End : Board Override Disable PERR/SERR for Video Device.

    // Grangeville Aptio V Server Override Start : Disable Errors for SW RAID device root port (D28:F0)
    if ((Bus == SW_RAID_ROOT_PORT_BUS_NUM)  && (Device == SW_RAID_ROOT_PORT_DEV_NUM) && (Function == SW_RAID_ROOT_PORT_FUN_NUM)) {
    	if(IsSwRaidBehindBridge(Bus,Device,Function))
    		Data8 &= ~(EFI_PCI_BRIDGE_CONTROL_SERR | EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE);
    }
    // Grangeville Aptio V Server Override end : Disable Errors for SW RAID device root port (D28:F0)
    
  PciExpressWrite8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), Data8);
}

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
{
  UINT16  Data16;

  //
  // Check if Assert on SERR is enabled in setup
  //
  Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_COMMAND_OFFSET));
  if (mNmiOnSerr) {
    Data16 |= EFI_PCI_COMMAND_SERR;
    //
    // Check if Assert NMI on PERR is enabled in setup
    //
    if (mNmiOnPerr) {
      Data16 |= EFI_PCI_COMMAND_PARITY_ERROR_RESPOND;
    } else {
      Data16 &= ~EFI_PCI_COMMAND_PARITY_ERROR_RESPOND;
    }
  } else {
    Data16 &= ~(EFI_PCI_COMMAND_SERR | EFI_PCI_COMMAND_PARITY_ERROR_RESPOND);
  }
  // Aptio V Server Override Start : Board Override Disable PERR/SERR for Video Device.
  if ((Bus == ONBOARD_VIDEO_ROOT_PORT_BUS_NUM)  && (Device == ONBOARD_VIDEO_ROOT_PORT_DEV_NUM) && (Function == ONBOARD_VIDEO_ROOT_PORT_FUN_NUM)) {
      Data16 &= ~(EFI_PCI_COMMAND_SERR | EFI_PCI_COMMAND_PARITY_ERROR_RESPOND);  
  }
  // Aptio V Server Override End : Board Override Disable PERR/SERR for Video Device.

  
  // Grangeville Aptio V Server Override Start : Disable Errors for SW RAID device root port (D28:F0)
  if ((Bus == SW_RAID_ROOT_PORT_BUS_NUM)  && (Device == SW_RAID_ROOT_PORT_DEV_NUM) && (Function == SW_RAID_ROOT_PORT_FUN_NUM)) {
	  if(IsSwRaidBehindBridge(Bus,Device,Function))
		  Data16 &= ~(EFI_PCI_COMMAND_SERR | EFI_PCI_COMMAND_PARITY_ERROR_RESPOND);  
  }
  // Grangeville Aptio V Server Override end : Disable Errors for SW RAID device root port (D28:F0)

  
  // Aptio V Server Override Start : Disable #SERR for SATA controller. EIP#228771 
  if ((Bus == ICH_SATA_BUS_NUMBER)  && (Device == ICH_SATA_DEVICE_NUMBER) && (Function == ICH_SATA_FUNCTION_NUMBER || Function == ICH_SATA1_FUNCTION_NUMBER)) {
      Data16 &= ~(EFI_PCI_COMMAND_SERR);  
  }
  // Aptio V Server Override End : Disable #SERR for SATA controller. EIP#228771

  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_COMMAND_OFFSET), Data16);
}

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
{
  UINT8   PciPrimaryStatus;
  UINT8   CapabilityOffset;
  UINT8   CapId;

  PciPrimaryStatus = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));

  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {

    CapabilityOffset = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, EFI_PCI_CAPABILITY_PTR));

    do {

      CapId = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilityOffset));
      if (CapId == EFI_PCI_CAPABILITY_ID_PCIEXP) {
        break;
      }
      CapabilityOffset = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilityOffset + 1));

    } while (CapabilityOffset != 0);

  } else {
    CapabilityOffset = 0;
  }

  return CapabilityOffset;
}


/*UINT16
PciErrLibGetLERCapabilitiesOffset (
    IN UINT8   Bus,
    IN UINT8   Device,
    IN UINT8   Function
)
/*++

Routine Description:
    This function obtains the PCI configuration space register offset of the specified device's
    PCI Express Capabilities register set.  This will get the offset for LER capability 

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number

Returns:
    0        - PCIe capability ID not found
    non-zero - Offset of the PCIe capability register set
--*/
/*{
  UINT8                       PciPrimaryStatus;
  UINT16                      CapabilityOffset = 0;
  PCIE_EXTENDED_CAPABILITY   *ExtCapability;
  UINT32                      ExtCapabilityData;
  UINT16                      ExtCapId =0;
  LER_HDR_IIO_PCIE_STRUCT     LERCapabilityHeader;
  UINT16                      PreviousCapabilityOffset = 0;

  PciPrimaryStatus = PciExpressRead8(PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));

  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    ExtCapabilityData = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, PCIE_ENHANCED_CAPABILITY_PTR));
    ExtCapability =(PCIE_EXTENDED_CAPABILITY  *)&ExtCapabilityData;
    CapabilityOffset = (UINT16)ExtCapability->CapabilityOffset;    
    ExtCapId = (UINT16)ExtCapability->ExtCapId;
    while (CapabilityOffset != 0 && ExtCapId != PCIE_EXT_CAP_HEADER_INVALID){  
      if(ExtCapId == PCIE_EXT_CAP_HEADER_AERC){
         LERCapabilityHeader.Data = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, (PreviousCapabilityOffset+4)));
         if ((LERCapabilityHeader.Bits.vsecid == PCIE_EXT_CAP_HEADER_LERC) && (LERCapabilityHeader.Bits.vsecrev == PCIE_EXT_CAP_HEADER_LERC_REVISION))
            break;
      }      
      ExtCapabilityData = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, CapabilityOffset));
      ExtCapability =(PCIE_EXTENDED_CAPABILITY  *)&ExtCapabilityData;
      PreviousCapabilityOffset = CapabilityOffset;
      CapabilityOffset = (UINT16)ExtCapability->CapabilityOffset;
      ExtCapId = (UINT16)ExtCapability->ExtCapId;
    }
    if ((CapabilityOffset == 0) && (PreviousCapabilityOffset != 0)){
         if(ExtCapId == PCIE_EXT_CAP_HEADER_AERC){
            LERCapabilityHeader.Data = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, (PreviousCapabilityOffset+4)));
            if ((LERCapabilityHeader.Bits.vsecid != PCIE_EXT_CAP_HEADER_LERC) || (LERCapabilityHeader.Bits.vsecrev != PCIE_EXT_CAP_HEADER_LERC_REVISION))
               PreviousCapabilityOffset = 0;
         } 
    }
  }
  return PreviousCapabilityOffset;
}*/
 
BOOLEAN
PCIeIsRootPort(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function will verify whether a port is PCIe root port

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number

Returns:
    True     - if PCIe is root port
    False    - if PCIe is not root port
--*/
{
  BOOLEAN RootPort;
  UINT16  PcieCapabilities;
  UINT16  CapabilitiesOffset;

  RootPort = FALSE;

  if (!PciErrLibIsDevicePresent (Bus, Device, Function))
     return FALSE;
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, (CapabilitiesOffset + PCIE_CAPABILITY_OFFSET)));
  if ((PcieCapabilities & 0xF0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    RootPort = TRUE;
  } else {
    RootPort = FALSE;
  }
  return RootPort;
}

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
{
  UINT8   CapabilitiesOffset;
  UINT16  PcieCapabilities;
  UINT16  DeviceControl;
  UINT16  RootControl;
  UINT16  StatusReg16;
  UINT16  AerRegisterOffset;
  UINT32  StatusReg;
  BOOLEAN RootPort;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  UINT16  DeviceId;
  BOOLEAN UseDMIPortRegister = FALSE;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return ;
  }

  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    RootPort = TRUE;
  } else {
    RootPort = FALSE;
  }
  //
  // Check for presence of AER 
  //
  AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_PCIE_EXT_CAP_HEADER_AERC,0);
  if (AerRegisterOffset != 0) {
    //
    // Clear AER status registers including uncorrectable and correctable errors...
    //
    StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES));
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES), StatusReg);
  
    StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES));
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES), StatusReg);
  
    //
    // Program AER mask registers for uncorrectable and correctable errors,if any
    //
  
    //
    // Program AER severity registers for uncorrectable,if any
    //
  }
  //
  // Clear the PCIe device status register
  //
  
  // Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (Bus == 0 && Device == 0 && Function == 0)
  {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
          
  }

  if (UseDMIPortRegister)
  {
      StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET));
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET), StatusReg16);
  }
  
  else
  {
      StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET), StatusReg16);
  }

// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  //
  // If this is a PCIe Root Port of the PCIe root complex, then configure its
  // Root Control Register also...
  //
  if (RootPort) {
    RootControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET));
    //
    // Enable SERR on Fatal, Non-Fatal, and Correctable errors for error reporting.
    // An NMI will not be generated for correctable errors.
    //
    if(mPcieCorrErrorEn == TRUE)
       RootControl |= (B_PCIE_ROOT_CONTROL_COR_ERR_EN);
    if(mPcieUnCorrErrorEn == TRUE)
       RootControl |= (B_PCIE_ROOT_CONTROL_NF_ERR_EN);
    if(mPcieFatalErrorEn == TRUE)
       RootControl |= (B_PCIE_ROOT_CONTROL_FAT_ERR_EN);

    // Aptio V Server Override Start : Disable Errors for PCH Root Port (D28:F3) to which on board video device is connected. 
    if ((Bus == ONBOARD_VIDEO_ROOT_PORT_BUS_NUM)  && (Device == ONBOARD_VIDEO_ROOT_PORT_DEV_NUM) && (Function == ONBOARD_VIDEO_ROOT_PORT_FUN_NUM)) {
        RootControl  &= ~(B_PCIE_ROOT_CONTROL_COR_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_FAT_ERR_EN);
    }
    // Aptio V Server Override End : Disable Errors for PCH Root Port (D28:F3) to which on board video device is connected.

    // Grangeville Aptio V Server Override Start : Disable Errors for SW RAID device root port (D28:F0)
    if ((Bus == SW_RAID_ROOT_PORT_BUS_NUM)  && (Device == SW_RAID_ROOT_PORT_DEV_NUM) && (Function == SW_RAID_ROOT_PORT_FUN_NUM)) {
    	if(IsSwRaidBehindBridge(Bus,Device,Function))
    		RootControl  &= ~(B_PCIE_ROOT_CONTROL_COR_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_FAT_ERR_EN);
    }
    // Grangeville Aptio V Server Override end : Disable Errors for SW RAID device root port (D28:F0)
    
    
    PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET), RootControl);
  }

  //
  // Enable Error logging in Device Control...
  //
  
  // Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (UseDMIPortRegister)
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET));
  }

  else
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support    
    if(mPcieCorrErrorEn == TRUE)
       DeviceControl |= (B_PCIE_DEVICE_CONTROL_COR_ERR_EN);
    if(mPcieUnCorrErrorEn == TRUE)
       DeviceControl |= (B_PCIE_DEVICE_CONTROL_NF_ERR_EN);
    if(mPcieFatalErrorEn == TRUE)
       DeviceControl |= (B_PCIE_DEVICE_CONTROL_FAT_ERR_EN);
  DeviceControl &= ~(B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN);
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  

// Aptio V Server Override Start : Disable Errors for PCH Root Port (D28:F3) to which on board video device is connected. 
    if ((Bus == ONBOARD_VIDEO_ROOT_PORT_BUS_NUM)  && (Device == ONBOARD_VIDEO_ROOT_PORT_DEV_NUM) && (Function == ONBOARD_VIDEO_ROOT_PORT_FUN_NUM)) {
        DeviceControl  &= ~(B_PCIE_DEVICE_CONTROL_COR_ERR_EN | B_PCIE_DEVICE_CONTROL_NF_ERR_EN | B_PCIE_DEVICE_CONTROL_FAT_ERR_EN);
    }
// Aptio V Server Override End : Disable Errors for PCH Root Port (D28:F3) to which on board video device is connected.

// Grangeville Aptio V Server Override Start : Disable Errors for SW RAID device root port (D28:F0)
   if ((Bus == SW_RAID_ROOT_PORT_BUS_NUM)  && (Device == SW_RAID_ROOT_PORT_DEV_NUM) && (Function == SW_RAID_ROOT_PORT_FUN_NUM)) {
	   if(IsSwRaidBehindBridge(Bus,Device,Function))
		   DeviceControl  &= ~(B_PCIE_DEVICE_CONTROL_COR_ERR_EN | B_PCIE_DEVICE_CONTROL_NF_ERR_EN | B_PCIE_DEVICE_CONTROL_FAT_ERR_EN);
   }
// Grangeville Aptio V Server Override end : Disable Errors for SW RAID device root port (D28:F0)

    if (UseDMIPortRegister)
    {
         PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET), DeviceControl);
    }
    else
    {
        PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET), DeviceControl);
    }    
// Aptio V Server Override End - Changes done to support the DMI error Logging Support
}

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
{
  UINT8   CapabilitiesOffset;
  UINT16  StatusReg16;

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  UINT16  DeviceId;
  BOOLEAN UseDMIPortRegister = FALSE;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support

  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return ;
  }

  //
  // Clear the PCIe device status register
  //
  // Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (Bus == 0 && Device == 0 && Function == 0)
  {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
          
  }

  if (UseDMIPortRegister)
  {
      StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET));
  }
  
  else
  {
      StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
  }

// Aptio V Server Override End - Changes done to support the DMI error Logging Support
  
  if(StatusReg16 & B_PCIE_DEVICE_STATUS_FAT_ERR)  {
    StatusReg16 |= (B_PCIE_DEVICE_STATUS_FAT_ERR);
  }

  if((StatusReg16 & B_PCIE_DEVICE_STATUS_NF_ERR) && (mAllowClearingOfUEStatus == FALSE))  {
    StatusReg16 &= ~(B_PCIE_DEVICE_STATUS_NF_ERR);
  }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support
  if (UseDMIPortRegister)
  {
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET), StatusReg16);
  }
  
  else
  {
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET), StatusReg16);
  }   

// Aptio V Server Override End - Changes done to support the DMI error Logging Support
  PciErrLibClearLegacyPciDeviceStatus (Bus, Device, Function);
}
/*
#ifdef CRB_FLAG
VOID UpdateCardData(
    IN UINT8   Bus,
    IN UINT8   Device,
    IN UINT8   Function
    )
{
    UINT8 i;
    UINT32 ReadBack = 0;
    for (i=0; i<MaxCardDataCount; i++){ 
        if ((Bus == CardData[i].RootPortBus) && (Device == CardData[i].RootPortDevice) && (Function == CardData[i].RootPortFunction)){
          DEBUG((EFI_D_INFO | EFI_D_ERROR | EFI_D_WARN, "Writing Back to Bus  : %d   Device : %d Function %d \n", CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction));
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, PCI_COMMAND_OFFSET), CardData[i].PCICommand);
          while (TRUE){
              ReadBack =  PciExpressRead32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, PCI_COMMAND_OFFSET));
              if (ReadBack == CardData[i].PCICommand)
                break;
              PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, PCI_COMMAND_OFFSET), CardData[i].PCICommand);
          }
          
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, REMAP1_OFFSET), CardData[i].Remap1);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, REMAP2_OFFSET), CardData[i].Remap2);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, REMAP3_OFFSET), CardData[i].Remap3);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, REMAP4LOW_OFFSET), CardData[i].Remap4Low);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, REMAP4HIGH_OFFSET), CardData[i].Remap4High);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, PCIINTERRUPT_LINE__OFFSET), CardData[i].PCIInterruptLine);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, PCIEXP_LINKCTRL_OFFSET), CardData[i].PCIExpLinkCtrl);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, LINK_DBG_CTRL_OFFSET), CardData[i].LinkDbgCtrl);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, DFS1REGISTER_OFFSET), CardData[i].DFS1Register);
          PciExpressWrite32(PCI_PCIE_ADDR(CardData[i].PcieSlotBus, CardData[i].PcieSlotDevice,CardData[i].PcieSlotFunction, TLCOMPLETION_TIMEOUT_OFFSET), CardData[i].TLCompletionTimeout);
        }
    }
    ASSERT_EFI_ERROR(i != MaxCardDataCount);
}
#endif
*/



/*BOOLEAN
PciErrLibLERClearStatus(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
/*++

Routine Description:
    This function will wait until LER_port_Quesced bit is cleared. Once LER_port_Quesced
    is cleared, this function will clear the LER status

Arguments:
    Bus      - Device's bus number
    Device   - Device's device number
    Function - Device's function number

Returns:

--*/
/*{
    LER_CTRLSTS_IIO_PCIE_STRUCT iioLerPCIeCtrlSts;
    UINT16   CapabilitiesOffset;
    //
    //  If it is DMI port do nothing, because LER is not enabled on DMI
    //
    if ((Bus == 0) && (Device == 0) && (Function == 0))
         return TRUE;
    //
    // Check if it is a root port.
    //
    if (!PCIeIsRootPort(Bus, Device, Function)) {
         return TRUE;
    }
    
    // Get capability offset for LER
    CapabilitiesOffset = PciErrLibGetLERCapabilitiesOffset(Bus, Device, Function);
    if (CapabilitiesOffset == 0)
        return TRUE;

    iioLerPCIeCtrlSts.Data = PciExpressRead32(PCI_PCIE_ADDR(Bus, Device, Function, (CapabilitiesOffset + 8)));
    if (iioLerPCIeCtrlSts.Bits.ler_ss_lnk_up_ok == 0){
        return FALSE;
    }
    iioLerPCIeCtrlSts.Bits.ler_ss_status = 1;
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, (CapabilitiesOffset + 0x8)), iioLerPCIeCtrlSts.Data);
#ifdef CRB_FLAG
    UpdateCardData(Bus, Device, Function);
#endif
    return TRUE;
}*/



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
{
  UINT8     CapabilitiesOffset;
  UINT16    PcieCapabilities;
  UINT16    RootControl;
  //Aptio V Server Override Start: WorkAround added to avoid the SMI Flood on Few Linux OS like CENT OS which is clearing the ROOTCNTL Registers when PCIE Errors are getting generated
  MISCCTRLSTS_1_IIO_PCIE_IVT_HSX_STRUCT iioMiscCtrlStsN1Reg;
  //Aptio V Server Override End: WorkAround added to avoid the SMI Flood on Few Linux OS like CENT OS which is clearing the ROOTCNTL Registers when PCIE Errors are getting generated.
  BOOLEAN   IsEnabled = FALSE;


  //
  // Check if a device is present at the specified location.
  //
  if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
    return FALSE;
  }

  //
  // Get the capabilities offset and verify this is a root port
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) != V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    return FALSE;
  }

  RootControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET));
  if ( RootControl & (B_PCIE_ROOT_CONTROL_FAT_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_COR_ERR_EN) ) {
    IsEnabled = TRUE;
  }

  //Aptio V Server Override Start: WorkAround added to avoid the SMI Flood on Few Linux OS like CENT OS which is clearing the ROOTCNTL Registers when PCIE Errors are getting generated.
  iioMiscCtrlStsN1Reg.Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, ONLY_REGISTER_OFFSET(MISCCTRLSTS_1_IIO_PCIE_REG)));
  
  if(iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_correctable_error_enable == 1||
  iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_non_fatal_error_enable ==1||
  iioMiscCtrlStsN1Reg.Bits.override_system_error_on_pcie_fatal_error_enable ==1)IsEnabled = TRUE;


  //Aptio V Server Override End: WorkAround added to avoid the SMI Flood on Few Linux OS like CENT OS which is clearing the ROOTCNTL Registers when PCIE Errors are getting generated.


  return IsEnabled;
}


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
{
  UINT8     CapabilitiesOffset;
  UINT16    PcieCapabilities;
  UINT16    RootControl;
  BOOLEAN   AerPresentFlag;
  UINT8     IohGlobalStatus;
  UINT8     Ich9RootStatus;
  BOOLEAN   UncorrectableErrorReportingEnabled = FALSE;
  BOOLEAN   CorrectableErrorReportingEnabled   = FALSE;
  BOOLEAN   IsError                            = FALSE;


  //
  // Get the capabilities offset and verify this is a root port
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) != V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    return FALSE;
  }

  // 
  // Determine which errors are enabled
  //
  RootControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET));
  if ( RootControl & (B_PCIE_ROOT_CONTROL_FAT_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN) ) {
    UncorrectableErrorReportingEnabled = TRUE;
  }
  if ( RootControl & (B_PCIE_ROOT_CONTROL_COR_ERR_EN) ) {
    CorrectableErrorReportingEnabled   = TRUE;
  }

  AerPresentFlag  = PciErrLibCheckAerCapability(Bus, Device, Function);
  if (AerPresentFlag) {
    if (!ElogIsIchPcieRootPort(Bus, Device, Function)){
       IohGlobalStatus = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCIE_PORT_XPGLBERRSTS));
       if (IohGlobalStatus & (B_PCIE_PORT_XPGLBERRSTS_CAER | B_PCIE_PORT_XPGLBERRSTS_NAER | B_PCIE_PORT_XPGLBERRSTS_FAER))
        IsError   = TRUE; 
    } else {
       Ich9RootStatus = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCH_PCIE_RES));
       if (Ich9RootStatus & (B_PCH_PCIE_RES_CR | B_PCH_PCIE_RES_ENR | B_PCH_PCIE_RES_MCR | B_PCH_PCIE_RES_MENR)) 
         IsError   = TRUE;       
    }
  }
  // Aptio V Server Override Start : For Some PCIE Cards, Global status register are not setting.
  if (IsError == FALSE)
  {
    if ((UncorrectableErrorReportingEnabled == TRUE) || (CorrectableErrorReportingEnabled == TRUE)) IsError = TRUE;
  }

  // Aptio V Server Override End : For Some PCIE Cards, Global status register are not setting.
  return IsError;
}


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
{
  UINT8     RootPortBus      = Bus;
  UINT8     RootPortDevice   = Device;
  UINT8     RootPortFunction = Function;
  BOOLEAN   ErrorDetected    = FALSE;
  UINT8     SecondaryBusNumber;
  UINT8     SubordinateBusNumber;
  BOOLEAN   IsMultiFuncDev[PCI_MAX_FUNC+1];
  UINT16    AerRegisterOffset;
  UINT32    StatusReg;
  // 
  // Check for errors on subordinate devices
  //
  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {

    SecondaryBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                           Device,
                                           Function,
                                           PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                                         ));

    SubordinateBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                             Device,
                                             Function,
                                             PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET
                                           ));

    for (Bus = SecondaryBusNumber; Bus <= SubordinateBusNumber; Bus++) {
      if((SecondaryBusNumber == 00) && (SubordinateBusNumber == 00)) break;

      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
          IsMultiFuncDev[Function]= FALSE;
          if ( PciErrLibIsDevicePresent (Bus, Device, Function) ) {
            IsMultiFuncDev[Function]= TRUE;
            if ( PciErrLibIsPcieDevice (Bus, Device, Function) ) {

              if ( PciErrLibPcieDeviceErrorHandler (Bus, Device, Function) ) {
                ErrorDetected = TRUE;
              }

            } else {

              if ( PciErrLibPciDeviceErrorHandler (Bus, Device, Function) ) {
                ErrorDetected = TRUE;
              }

              PciErrLibClearLegacyPciDeviceStatus (Bus, Device, Function);
            }
          }
        }
        
        for (Function=0;Function <= PCI_MAX_FUNC; Function++) {
          if (IsMultiFuncDev[Function] == TRUE){
          AerRegisterOffset = PciErrLibGetExtendedCapabilitiesOffset (Bus, Device, Function,V_PCIE_EXT_CAP_HEADER_AERC,0);
          if (AerRegisterOffset != 0) {
            //
            // Clear AER UES bit 20( received an unsupported request)...
            //
            StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES));
            StatusReg |= BIT20;
            PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_UES), StatusReg);
            //
            // Clear AER CES bit 13( advisory non-fatal error status)...
            //
            StatusReg = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES));
            StatusReg |= BIT13;
            PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, AerRegisterOffset + PCIE_AER_OFFSET_REG_CES), StatusReg);
            }
          }
        }
      }
    }
  }

  //
  // Check for a Root Port errors
  //
 // if ( !ErrorDetected ) {
    ErrorDetected = PciErrLibPcieDeviceErrorHandler (RootPortBus, RootPortDevice, RootPortFunction);
 // }

  // 
  // Clear the Root Port status (especially the Root Port Error Message Status)
  //
  if (mAllowClearingOfPcieDeviceStatus) {
    PciErrLibClearPcieDeviceStatus (RootPortBus, RootPortDevice, RootPortFunction);
    PciErrLibClearLegacyPciDeviceStatus(RootPortBus, RootPortDevice, RootPortFunction);
  }
  return ErrorDetected;
}

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
{
  BOOLEAN   ErrorDetected = FALSE;

  //
  // Check for an Uncorrectable PCIe error
  //
  if ( PciErrLibIsFatalPcieDeviceError (Bus, Device, Function) ||
       PciErrLibIsNonfatalPcieDeviceError (Bus, Device, Function) ) {

    ErrorDetected = TRUE;

    PciErrLibHandleUncorrectablePcieDeviceError (Bus, Device, Function);
  } 

  //
  // Check for a Correctable PCIe error
  //
  if ( PciErrLibIsCorrectablePcieDeviceError (Bus, Device, Function) ) {

    ErrorDetected = TRUE;

    PciErrLibHandleCorrectablePcieDeviceError (Bus, Device, Function);
  } 

  if ( ErrorDetected && mAllowClearingOfPcieDeviceStatus ) {
    PciErrLibClearPcieDeviceStatus (Bus, Device, Function);
  }
  
  return ErrorDetected;
}

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
{
  UINT8     CapabilitiesOffset;
  UINT16    DeviceStatus;
  UINT16    DeviceControl;
  UINT16    PrimaryStatus;
  BOOLEAN   SERREnabled;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  UINT16  DeviceId;
  BOOLEAN   UseDMIPortRegister = FALSE;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  

  //
  // Get the capabilities offset
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (Bus == 0 && Device == 0 && Function == 0)
  {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
          
  }

  // 
  // Check if the device status indicates that a FATAL error occurred
  //

  if (UseDMIPortRegister)
  {
     DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET));
  }
   
  else
  {
      DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  if ( !(DeviceStatus & B_PCIE_DEVICE_STATUS_FAT_ERR) ) {
    //
    // No FATAL error reported
    //
    return FALSE;
  }


  // 
  // FATAL error status set, now check if FATAL error reporting is enabled.
  // FATAL error reporting is enabled if SERR is enabled or FATAL reporting is enabled
  // in the PCIe device control register.
  //
  if (PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_SERR) {
    SERREnabled = TRUE;
  } else {
    SERREnabled = FALSE;
  }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (UseDMIPortRegister)
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET));
  }
  
  else
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  if ( !SERREnabled && !(DeviceControl & B_PCIE_DEVICE_CONTROL_FAT_ERR_EN) ) {
    //
    // FATAL error reporting is not enabled
    //
    return FALSE;
  }

  //
  // FATAL status and FATAL reporting enabled.
  // The PCIe error status could be set even if the error was masked.
  // The PCIe error status could be set even if there are not bits set in the AER status register.
  // Check SERR status to see if a FATAL/NON-FATAL error message was sent.
  //
  if (SERREnabled) {

    PrimaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
    if (PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
      //
      // SERR was signaled (FATAL or Non-Fatal error occurred)
      //
      return TRUE;
    } else {
      return FALSE;
    }

  } else {
    //
    // SERR not enabled, but PCIe reporting is enabled and status indicates an error.
    //
    return TRUE;
  }
}

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
{
  UINT8     CapabilitiesOffset;
  UINT16    DeviceStatus;
  UINT16    DeviceControl;
  UINT16    PrimaryStatus;
  BOOLEAN   SERREnabled;
  BOOLEAN   UnsupportedRequestDetected = FALSE;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  UINT16  DeviceId;
  BOOLEAN  UseDMIPortRegister = FALSE;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  

  //
  // Get the capabilities offset
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
    if (Bus == 0 && Device == 0 && Function == 0)
    {
          DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
          if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
              
    }
    
  // 
  // Check if the device status indicates that a NON-FATAL error occurred
  //

  if (UseDMIPortRegister)
  {
      DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET)); 
  }
  else
  {
        
      DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support    
  if ( !(DeviceStatus & B_PCIE_DEVICE_STATUS_NF_ERR) ) {
    //
    // No NON-FATAL error reported
    //
    return FALSE;
  }
  if (DeviceStatus & B_PCIE_DEVICE_STATUS_UNSUP_REQ) {
    UnsupportedRequestDetected = TRUE;
  }

  // 
  // NON-FATAL error status set, now check if NON-FATAL error reporting is enabled.
  // NON-FATAL error reporting is enabled if SERR is enabled or NON-FATAL reporting is enabled
  // in the PCIe device control register.
  //
  //AptioV Server Override: Changes done to read from the correct command register offset
  //if (PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_SERR)) {
  if (PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_COMMAND_OFFSET)) & EFI_PCI_COMMAND_SERR) {
    SERREnabled = TRUE;
  } else {
    SERREnabled = FALSE;
  }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (UseDMIPortRegister)
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET));
  }
  else
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  if ( !SERREnabled && !(DeviceControl & B_PCIE_DEVICE_CONTROL_NF_ERR_EN) ) {
    //
    // NON-FATAL error reporting is not enabled
    //
    return FALSE;
  }

  //
  // NON-FATAL status and NON-FATAL reporting enabled.
  // The PCIe status could be set even if the error was masked.
  // The PCIe status could be set even if there are not bits set in the AER status register.
  // Check SERR status to see if a FATAL/NON-FATAL error message was sent.
  //
  if (SERREnabled) {

    PrimaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
    if (PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
      //
      // SERR was signaled (FATAL or Non-Fatal error occurred)
      //
      return TRUE;
    } else {
      return FALSE;
    }

  } else {
    //
    // SERR not enabled, but PCIe reporting is enabled and status indicates an error.
    // This is likely an Unsupported Request error that occurred during PCI bus probe.
    //
    if (UnsupportedRequestDetected) {
      //
      // Even if we incorrectly ignore this error we will NMI (if appropriate) 
      // because of the Uncorrectable error check in PciErrLibIsErrOnPcieRootPort().
      //
      return FALSE;
    } else {
      return TRUE;
    }
  }
}

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
{
  UINT8     CapabilitiesOffset;
  UINT16    DeviceStatus;
  UINT16    DeviceControl;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  BOOLEAN   UseDMIPortRegister = FALSE;
  UINT16  DeviceId;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  
  //
  // Get the capabilities offset
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }
  
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
    if (Bus == 0 && Device == 0 && Function == 0)
    {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
          
    }
    
      // 
      // Check if the device status indicates that a CORRECTABLE error occurred
      //

    if (UseDMIPortRegister)
    {
        DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET));
    }
    
    else
    {

        DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
    }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support      
  if ( !(DeviceStatus & B_PCIE_DEVICE_STATUS_COR_ERR) ) {
    //
    // No CORRECTABLE error
    //
    return FALSE;
  }
  // 
  // Now check if CORRECTABLE error reporting is enabled
  //
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  if (UseDMIPortRegister)
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET));
  }
  else
  {
      DeviceControl = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support    
  if ( !(DeviceControl & B_PCIE_DEVICE_CONTROL_COR_ERR_EN) ) {
    //
    // CORRECTABLE error reporting not enabled
    //
    return FALSE;
  }
  return TRUE;
}

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
{
  UINT8     SecondaryBusNumber;
  UINT8     SubordinateBusNumber;


  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {

    SecondaryBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                           Device,
                                           Function,
                                           PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET
                                         ));

    SubordinateBusNumber = PciExpressRead8 ( PCI_PCIE_ADDR(Bus,
                                             Device,
                                             Function,
                                             PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET
                                           ));

    for (Bus = SecondaryBusNumber; Bus <= SubordinateBusNumber; Bus++) {
      if((SecondaryBusNumber == 00) && (SubordinateBusNumber == 00)) break;

      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {

          if ( PciErrLibIsDevicePresent (Bus, Device, Function) ) {

            if ( PciErrLibIsPcieDevice (Bus, Device, Function) ) {

              if ( PciErrLibIsFatalPcieDeviceError (Bus, Device, Function)      ||
                   PciErrLibIsNonfatalPcieDeviceError (Bus, Device, Function)   ||
                   PciErrLibIsCorrectablePcieDeviceError (Bus, Device, Function)   ) {

                return TRUE;
              }

            } else {

              if ( PciErrLibIsPciDeviceError (Bus, Device, Function) ) {
                return TRUE;
              }
            }
          }
        }
      }
    }
  }

  return FALSE;
}

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
{
  UINT16                PrimaryStatus;
  UINT16                ErrorMap = EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR      |
                                   EFI_PCI_STATUS_DETECTED_PARITY_ERROR      |
                                   EFI_PCI_STATUS_DATA_PARITY_ERROR_DETECTED |
                                   EFI_PCI_STATUS_RECEIVED_TARGET_ABORT;


  if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
    return FALSE;
  }

  PrimaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
  if (!(PrimaryStatus & ErrorMap)) {
    return FALSE;
  }

  return TRUE;
}

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
{
  UINT8                    CapabilitiesOffset;
  UINT16                   PCIeCapabilities;
  UINT8                    DeviceType;
  BOOLEAN                  PCIeToPCIX;
  UINT16                   SecondaryStatus;
  UINT16                   ErrorMap;
  UINT8                    PciErrorType;
  UINT16                   DeviceStatus;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  BOOLEAN                  UseDMIPortRegister = FALSE;
  UINT16  DeviceId;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  

  //
  // If PCIe-to-PCI-X bridge, check secondary status register for a PCI error
  //
  //
  // Get the capabilities offset
  //
  CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return;
  }

  PCIeCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  DeviceType = (UINT8)(PCIeCapabilities >> 4) & 0x0F;

  PCIeToPCIX = (DeviceType == 0x07) ? TRUE : FALSE;

  if ( PCIeToPCIX ) {

    ErrorMap = EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR      | EFI_PCI_STATUS_DETECTED_PARITY_ERROR |
               EFI_PCI_STATUS_DATA_PARITY_ERROR_DETECTED | EFI_PCI_STATUS_RECEIVED_TARGET_ABORT;

    SecondaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET));
    if ((SecondaryStatus & ErrorMap) != 0) {

      //
      // Error signaled on the secondary bus
      //
      if (SecondaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
        PciErrorType = SERROR_TYPE;
      } else {
        PciErrorType = PERROR_TYPE;
      }

      //
      // Report the error
      //
      PciErrLibReportError (Bus, Device, Function, PciErrorType);

      //
      // Clear the error bits
      //
      PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET), ErrorMap);

      return;
    }
  }
  //
  // Not a PCI error, report a FATAL PCIe error
  //
  
  // 
  // Check if the device status indicates that a FATAL error or uncorrected error occurred
  //
  
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
    if (Bus == 0 && Device == 0 && Function == 0)
    {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
          
    }

    if (UseDMIPortRegister)
    {
        DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_STATUS_OFFSET));
    }
    
    else
    {
  
        DeviceStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET));
    }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support      
  if ( (DeviceStatus & B_PCIE_DEVICE_STATUS_FAT_ERR) ) {
    //
    // FATAL error reported
    //
    // AptioV Server Override Start: To make sure errors are logged to End-Point device.
     //if((mPciXPUnsupportedRequest != 01) && (PciErrLibIsPciBridgeDevice (Bus, Device, Function))) {
     if( mPciXPUnsupportedRequest != 01){
     // AptioV Server Override End: To make sure errors are logged to End-Point device.
     PciErrLibReportError (Bus, Device, Function, FATAL_ERROR_TYPE);
     }
  }else{
     // AptioV Server Override Start: To make sure errors are logged to End-Point device.
     //if((mPciXPUnsupportedRequest != 01)&& (PciErrLibIsPciBridgeDevice (Bus, Device, Function))) {
     if( mPciXPUnsupportedRequest != 01){
     // AptioV Server Override End: To make sure errors are logged to End-Point device.
     PciErrLibReportError (Bus, Device, Function, NONFATAL_ERROR_TYPE);
     }
    }
  

}

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
{
  UINT8              CapabilitiesOffset;
  UINT16             PcieCapabilities;
  UINT16             DeviceControl;
  UINT16             RootControl;
// Aptio V Server Override Start - Changes done to support the DMI error Logging Support    
  BOOLEAN            UseDMIPortRegister = FALSE;
  UINT16  DeviceId;
// Aptio V Server Override End - Changes done to support the DMI error Logging Support  

  //
  // Check if the correctable-error reporting limit has already been reached.
  //
  if ((mNumCorrectablePcieErrs < mPcieCorErrThreshold) || (mPcieCorErrThreshold == 0)) {
    //
    // Report the correctable error.
    //
    //if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)){ // AptioV Server Override: To make sure errors are logged to End-Point device.
      PciErrLibReportError (Bus, Device, Function, COR_ERROR_TYPE);
      mNumCorrectablePcieErrs++;
      return;
    //} // AptioV Server Override: To make sure errors are logged to End-Point device.
  }

  // 
  // Error limit has been reached.  Mask correctable errors in the device.
  //
  if ((mNumCorrectablePcieErrs >= mPcieCorErrThreshold) && (mPcieCorErrThreshold)) {
     DEBUG ((DEBUG_INFO, "Pcie CE reached threshold,disabling error escalation from this device\n"));
     CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
     if (CapabilitiesOffset == 0) {
       return;
     }

// Aptio V Server Override Start - Changes done to support the DMI error Logging Support  
  if (Bus == 0 && Device == 0 && Function == 0)
  {
      DeviceId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCIE_DID)); 
      if (DeviceId == 0x2F00) UseDMIPortRegister = TRUE;
              
  }

  if (UseDMIPortRegister)
  {
      DeviceControl  = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, DMI_DEVICE_CONTROL_OFFSET));
  }
  else
  {
      DeviceControl  = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET));
  }
// Aptio V Server Override End - Changes done to support the DMI error Logging Support    
  DeviceControl &= ~(B_PCIE_DEVICE_CONTROL_COR_ERR_EN);
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET), DeviceControl);

  //
  // Update the Root Control register if this is a root port.
  //
  PcieCapabilities = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET));
  if ((PcieCapabilities & 0xF0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    RootControl  = PciExpressRead16 ( PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET));
    RootControl &= ~(B_PCIE_ROOT_CONTROL_COR_ERR_EN);
    PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET), RootControl);
     }
  }
}

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
{
  UINT16                    PrimaryStatus;
  UINT8                     PciErrorType;

  UINT16                    ErrorMap = EFI_PCI_STATUS_DETECTED_PARITY_ERROR      |
                                       EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR      |
                                       EFI_PCI_STATUS_RECEIVED_TARGET_ABORT      |
                                       EFI_PCI_STATUS_DATA_PARITY_ERROR_DETECTED;


  if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
    return FALSE;
  }

  //
  // The standard behavior is outlined below:
  // 1. #SERR generation is controlled thru CMD.SEE and CMD.PERE.
  // 2. #SERR generation can also occur if the device is capable of being a bus master 
  //    on the primary bus and can detect parity errors. Optionally, #SERR can also be
  //    triggered by setting a chipset-specific policy for bridges and bus masters for 
  //    target and master aborts...
  //
  PrimaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET));
  if (!(PrimaryStatus & ErrorMap)) {
    return FALSE;
  }

  //
  // Error signaled on the primary bus...
  //
  if (PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
    PciErrorType = SERROR_TYPE;
  } else {
    PciErrorType = PERROR_TYPE;
  }

  //
  // Report the error...
  //
  PciErrLibReportError (Bus, Device, Function, PciErrorType);

  //
  // Clear the error bits...
  //
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET), ErrorMap);

  return TRUE;
}

VOID
PciErrLibReportError (
  IN UINT8                    Bus,
  IN UINT8                    Device,
  IN UINT8                    Function,
  IN UINT8                    ErrorType
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
{
  // 
  // NV ELog support 
  // 
  PCI_PCIE_ERROR_LOCAL_STRUC            PciErrStruc;
  //
  // Whea support
  //
// AptioV Server Override Start:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
//  PCIE_PCI_DEV_INFO                     PciDev;
//  UINT16                                WheaErrType;
// AptioV Server Override End:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
/*BOOLEAN                               UsePcieElogLinkSensor = FALSE;
  EFI_SEL_SENSOR_CRITICAL_INT_OFFSET    CriticalIntSensorOffset;
  EFI_SEL_RECORD_DATA                   *SelRecord = NULL;
  EFI_STATUS_CODE_TYPE                  StatusCodeSeverity;
  EFI_STATUS_CODE_VALUE                 StatusCodeMask;*/
// AptioV Server Override Start:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
//  PciDev.Segment      = 0;
//  PciDev.Bus          = Bus;
//  PciDev.Device       = Device;
//  PciDev.Function     = Function;
// AptioV Server Override End:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.

  PciErrStruc.ErrorSource  = PciSource;
  PciErrStruc.ErrorType    = ErrorType;
  PciErrStruc.Segment      = 0;
  PciErrStruc.Bus          = Bus;
  PciErrStruc.Device       = Device;
  PciErrStruc.Function     = Function;

  CommonErrorHandling((VOID *) &PciErrStruc); // AptioV Server Override:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.

// AptioV Server Override Start:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
/*  switch (ErrorType) {

    case SERROR_TYPE:

      mPciSerrDetected        = TRUE;
      /*UsePcieElogLinkSensor   = FALSE;
      CriticalIntSensorOffset = EFI_CRITICAL_INT_PCI_SERR;
      StatusCodeSeverity      = EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED;
      StatusCodeMask          = EFI_IO_BUS_PCI | EFI_IOB_PCI_EC_SERR;*//*

      WheaErrType             = GEN_ERR_SEV_PCI_DEV_FATAL;
      break;

    case PERROR_TYPE:
      mPciPerrDetected        = TRUE;
      /*UsePcieElogLinkSensor   = FALSE;
      CriticalIntSensorOffset = EFI_CRITICAL_INT_PCI_PERR;
      StatusCodeSeverity      = EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED;
      StatusCodeMask          = EFI_IO_BUS_PCI | EFI_IOB_PCI_EC_PERR;*//*

      WheaErrType             = GEN_ERR_SEV_PCI_DEV_FATAL;
      break;

    case COR_ERROR_TYPE:
      /*UsePcieElogLinkSensor   = TRUE;
      StatusCodeSeverity      = EFI_ERROR_CODE | EFI_ERROR_MINOR;
      CriticalIntSensorOffset = EFI_CRITICAL_INT_PCI_PERR;
      StatusCodeMask          = EFI_IO_BUS_PCI_EXPRESS | EFI_IOB_PCI_EXP_EC_PERR;*//*
      mPcieCorErrDetected     = TRUE; 

      WheaErrType             = GEN_ERR_SEV_PCIE_CORRECTED;
      break;

    case NONFATAL_ERROR_TYPE:
      mPcieNonFatalErrDetected    = TRUE;
      WheaErrType             = GEN_ERR_SEV_PCIE_FATAL;
      break;

    case FATAL_ERROR_TYPE:
      mPcieFatalErrDetected   = TRUE;
      /*UsePcieElogLinkSensor   = TRUE;
      CriticalIntSensorOffset = EFI_CRITICAL_INT_PCI_SERR;
      StatusCodeSeverity      = EFI_ERROR_CODE | EFI_ERROR_UNRECOVERED;
      StatusCodeMask          = EFI_IO_BUS_PCI_EXPRESS | EFI_IOB_PCI_EXP_EC_SERR;*//*

      WheaErrType             = GEN_ERR_SEV_PCIE_FATAL;
      break;

    default:
      return;
  }

  //
  // Whea support
  //
  if (mWheaPlatformSupportAvailable == TRUE) {
    if (WheaErrType == GEN_ERR_SEV_PCI_DEV_RECOVERABLE || WheaErrType == GEN_ERR_SEV_PCI_DEV_CORRECTED || \
        WheaErrType == GEN_ERR_SEV_PCI_DEV_FATAL || WheaErrType == GEN_ERR_SEV_PCI_DEV_INFORMATION) {
       mWheaPlatformSupport->WheaPlatformElogPciDev (mWheaPlatformSupport, WheaErrType, &PciDev);
    } else {
       mWheaPlatformSupport->WheaPlatformElogPcieRootDevBridge (mWheaPlatformSupport, WheaErrType, &PciDev);
    }
  }*/
// AptioV Server Override End:  Changes done to use AMI RuntimeError (RtErrorLogBMC/Whea) Listener code.
}

BOOLEAN
PciErrLibCheckAerCapability(
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function
  )
{
  UINT16 NextCurrentCapPtr;
  UINT16 CurrentCapPtr;
  UINT16 CapId;
  
  NextCurrentCapPtr = R_PCIE_EXT_CAP_HEADER;
  do {
    CurrentCapPtr = NextCurrentCapPtr;
    CapId = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CurrentCapPtr));
    NextCurrentCapPtr = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CurrentCapPtr + 2)) >> 4;
    if(CapId == V_PCIE_EXT_CAP_HEADER_AERC)
      return  TRUE; 
  } while (NextCurrentCapPtr != 0 && CapId != 0xffff && NextCurrentCapPtr > CurrentCapPtr);


  return  FALSE;  
}

UINT16 
PciErrLibGetExtendedCapabilitiesOffset (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  ExtCapabilityId,
  IN UINT16  VendorSpecificId
  )
/*++

Routine Description:
    This function obtains the extended PCI configuration space register offset for a
    specified Extended Capability for the specified PCI device.

Arguments:
    Bus             - Device's bus number
    Device          - Device's device number
    Function        - Device's function number
    ExtCapabilityId - ID of the desired Extended Capability
    VendoreSpecificId- vendor ID that is at offset ExtCapabilityId+4
Returns: 
    0        - PCIe extended capability ID not found
    non-zero - Extended PCI configuration space offset of the specified Ext Capability block

--*/
{
  UINT16    NextExtendedCapabilityOffset;
  UINT16    ExtendedCapabilitiesHdr;
  UINT32    NextPtr;
  UINT16    NextVendorSpecificptr;


  NextExtendedCapabilityOffset = PCIE_ENHANCED_CAPABILITY_PTR;

  do {

    ExtendedCapabilitiesHdr = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, NextExtendedCapabilityOffset));
    
    if (ExtendedCapabilitiesHdr == PCIE_EXT_CAP_HEADER_INVALID) {
      //
      // Done. No more extended capabilities to search...
      //
      return 0;
    }

    //
    // Check if this is the Extended Capability Header...
    //
    if (ExtendedCapabilitiesHdr == ExtCapabilityId) {
      if (VendorSpecificId != 0xFF){
         NextVendorSpecificptr = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, (NextExtendedCapabilityOffset+0x4)));
         if (NextVendorSpecificptr == VendorSpecificId)
            return NextExtendedCapabilityOffset;
      } else {
         return NextExtendedCapabilityOffset;
      }
    }

    //
    // Search the entire extended capability chain until it is exhausted...
    //
    NextPtr = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, NextExtendedCapabilityOffset));
    NextExtendedCapabilityOffset = (UINT16)(NextPtr >> 20);

    if ( (NextExtendedCapabilityOffset & 0x0FFF) == 0x0000) {
      //
      // Reached the end of the list.
      //
      return 0;
    }

  } while (TRUE);

  return 0;
}

VOID
  PciErrLibClearDeviceStatus(
  IN UINT8 Bus,
  IN UINT8 Device,
  IN UINT8 Function
  )
{
  UINT16  StatusReg16;
  UINT8  CapabilitiesOffset;

    CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
  if (CapabilitiesOffset == 0) {
    return ;
  }

  StatusReg16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset+PCIE_DEVICE_STATUS_OFFSET));
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, CapabilitiesOffset+PCIE_DEVICE_STATUS_OFFSET), StatusReg16);
}
