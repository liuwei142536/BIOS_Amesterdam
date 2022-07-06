/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2017 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PCHErrorHandler.c

Abstract:

 Error handling for PCH
----------------------------------------------------------------------*/
#include "CommonErrorHandlerIncludes.h"

UINT8     mIchBusNum;
UINT8     mIchPciErrors =1;
UINT8     mIchPcieErrors =1;
BOOLEAN   mIohErrPinSmiEn;

//
// PCH PCI/PCI-EX Device Table for Error handling
//
ICH_PCI_DEVICES  mIchPciDeviceTable[] = {
//
//   Segment        Bus                     Device                            Function                            Bridge Y/N
// -------------------------------------------------------------------------------------------------------------------------
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_LPC,        PCI_FUNCTION_NUMBER_PCH_LPC,                FALSE },
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_SATA,       PCI_FUNCTION_NUMBER_PCH_SATA,               FALSE }, 
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_SATA2,      PCI_FUNCTION_NUMBER_PCH_SATA2,              FALSE },
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_USB,        PCI_FUNCTION_NUMBER_PCH_EHCI,               FALSE },  
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_USB_EXT,    PCI_FUNCTION_NUMBER_PCH_EHCI2,              FALSE },  
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_AZALIA,     PCI_FUNCTION_NUMBER_PCH_AZALIA,             FALSE },  
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_LAN,        PCI_FUNCTION_NUMBER_PCH_LAN,                FALSE },  
  {00,            DEFAULT_PCI_BUS_NUMBER_PCH,  PCI_DEVICE_NUMBER_PCH_XHCI,       PCI_FUNCTION_NUMBER_PCH_XHCI,               FALSE },  
  {INVALID_FLAG,  INVALID_FLAG,        INVALID_FLAG,                     INVALID_FLAG,                               FALSE }
};

//--------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                          //
//                                               PCH Error generation Functions                                            //
//                                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------------//


EFI_STATUS
EnableElogP2P (
  IN UINT8                  Bus,
  IN UINT8                  Device,
  IN UINT8                  Function
  )
/*++

Routine Description:
    Enable error log for standard P2P bridge and its PCI devices.
    Any specific P2P enable stuff like ICH3's HI PCI bridge 8-bit protocol
    should add to the end of this call in that module (ICH3).

Arguments:
    Bus - Device's bus number
    Device - Device's device number
    Function - Device's function number
    
Returns: 
    EFI_SUCCESS if succeed    

--*/
{

  UINT32  Data;
  UINT16  Data16;

  //
  // Standard Initialization...
  //
  PciErrLibEnableElogDeviceAndSubDevices (Bus, Device, Function);

  //
  // Enable Discard Timer SERR# enable ,Secondary Discard timer
  //
  Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET));
  Data16 |= (BIT9 | BIT11);
  PciExpressWrite16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), Data16);

  //
  // Force SERR on PERR and SERR on Target Abort
  //
  if (mNmiOnSerr) {
    Data = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCH_PCI_PCI_BPC));
    Data |= (B_PCH_PCI_PCI_BPC_PSE | B_PCH_PCI_PCI_BPC_RTAE);
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCH_PCI_PCI_BPC), Data);
  }

  return EFI_SUCCESS;
}

VOID
ElogIchPciExEnable (
  IN UINT8              Bus
  )
/*++

Routine Description:
    Enable error log for Low Pin Count to PCI bridge device.

Arguments:

--*/
{

  UINT8   Function;
  UINT8   Device;
  UINT8   Data8;
  UINT8   SecondaryBusNumber;
  UINT16  Data16;
  UINT8   CapabilitiesOffset;
  UINT32  Data32;

  Device = (UINT8)PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS;

  for (Function = 0; Function <= PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8; Function++) {


    //
    // Check if the root port is enabled.
    //
    if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
      continue ;
    }

    Data8 = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCH_PCIE_PCISTS)); // Cap bit in status register
    if (!(Data8 & B_PCH_PCIE_PCISTS_CAP_LST)) {
      continue ;
    }

    //
    // Verify this is a Root Port.
    //
    CapabilitiesOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
    if (CapabilitiesOffset == 0) {
      continue ;
    }

    Data16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCH_PCIE_XCAP ));
    if ((Data16 & B_PCH_PCIE_XCAP_DT) != V_PCIE_CAPABILITY_DPT_ROOT_PORT){
      continue ;
    }
    //
    // Do not enable error logging if there are no devices behind the bridge.
    //
    SecondaryBusNumber = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
    if ((SecondaryBusNumber == 0) || !(PciErrLibDeviceOnBus (SecondaryBusNumber))) {
      continue ;
    }

    
    //Enable Advanced error reporting  
    Data32 = BIT4 | V_PCIE_EXT_CAP_HEADER_AERC;
    PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device,  Function, R_PCIE_EXT_CAP_HEADER),Data32);

    //
    // Enable error reporting in the root port and all subordinate devices.
    //
    PciErrLibEnableElogDeviceAndSubDevices (Bus, Device, Function);

  }

  return ;
}

VOID
ElogIchPciEnable (
  VOID
  )
{
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN DetectedError;
  UINT8   Index;

  DetectedError = FALSE;
  Index         = 0;
  //
  // Call the device-specific handler for each device in the table...
  //
  while ((mIchPciDeviceTable[Index].Segment != INVALID_FLAG) &&
         (mIchPciDeviceTable[Index].Bus != INVALID_FLAG)) {

    Bus       = mIchPciDeviceTable[Index].Bus;
    Device    = mIchPciDeviceTable[Index].Device;
    Function  = mIchPciDeviceTable[Index].Function;

    //
    // Check device status before device error handler initialing
    //
    if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
      //
      // Check device present or not
      //
      Index++;
      continue;
    }

    if (mIchPciDeviceTable[Index].Bridge == TRUE)  {
      EnableElogP2P (Bus, Device, Function);
    }
    else  {
      PciErrLibEnableElogDevice (Bus, Device, Function);
    }
    Index++;
  };
  return ;
}

VOID
ClearDMIErrors(
	IN UINT8			   Bus
	)
{

	UINT32	  *UES;
	UINT32	  *CES;
	UINT32	  *RES;
	UINT32	  RCBA;
    UINT32    ErrorStatus;

	RCBA  = PciExpressRead32 (PCI_PCIE_ADDR(Bus, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_RCBA )) & 0xFFFFC000;

	UES 	= (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_UES);
	CES 	= (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_CES);
	RES 	= (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_RES);

	//
	// Clear the uncorrectable error status bits
	//
	ErrorStatus   = *UES;
	*UES		  = ErrorStatus;

	//
	// Clear the correctable error status bits
	//
	ErrorStatus   = *CES;
	*CES		  = ErrorStatus;

	//
	// Clear the correctable error status bits
	//
	ErrorStatus   = *RES;
	*RES		  = ErrorStatus;

	return;
}

VOID
ElogIchDmiEnable (
  IN UINT8                 Bus
  )
{

  UINT32    *UES;
  UINT32    *UEM;
  UINT32    *CES;
  UINT32    *CEM;
  UINT32    *REC;
  UINT32    *RES;
  UINT32    RCBA;
  UINT32    UEMValue;
  UINT32    CEMValue;
  //
  // Get the RCBA base address
  //
  RCBA  = PciExpressRead32 (PCI_PCIE_ADDR(Bus, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_RCBA )) & 0xFFFFC000;
  UES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_UES);
  UEM     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_UEM);
  CES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_CES);
  CEM     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_CEM);
  REC     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_REC);
  RES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_RES);

  //
  // UnMask the uncorrectable errors
  //
  UEMValue  = *UEM;
  UEMValue  &= ~( B_PCH_RCRB_UE_DLPE | B_PCH_RCRB_UE_PT | B_PCH_RCRB_UE_CA | 
                 B_PCH_RCRB_UE_RO | B_PCH_RCRB_UE_MT );
  *UEM      |= UEMValue;

  //
  // UnMask the Correctable errors
  //
  CEMValue  = *CEM;
  CEMValue  &= ~( B_PCH_RCRB_CE_RE | B_PCH_RCRB_CE_BT | B_PCH_RCRB_CE_BD | B_PCH_RCRB_CE_RNR );
  *CEM      |= CEMValue;

  ClearDMIErrors(Bus);
  return;
}

VOID
ClearPCHErrors(VOID)
{

    ClearDMIErrors(DEFAULT_PCI_BUS_NUMBER_PCH);
	return;
}

EFI_STATUS
EnableElogPCH (
   VOID
  )
{
  mIchBusNum = DEFAULT_PCI_BUS_NUMBER_PCH;
  mIchPciErrors = 0;
  mIchPcieErrors = 0;

  if ((mErrLogSetupData->SystemErrorEn == 1) &&
      (mErrLogSetupData->PchErrorEn == 1)) {

      //
      // Enable PCI device error reporting
      //
      mIchPciErrors  = 1;
      ElogIchPciEnable ();
    if (mErrLogSetupData->PcieErrorsEn == 1)  {
      //
      // setup PCI-ex error reporting
      //
      mIchPcieErrors  = 1;
      ElogIchDmiEnable (mIchBusNum);
      ElogIchPciExEnable (mIchBusNum);
    }
  }
  return EFI_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------------//
//                                                                                                                          //
//                                               PCH SMI error handler function                                            //
//                                                                                                                          //
//--------------------------------------------------------------------------------------------------------------------------//

BOOLEAN
IchPcieErrorDetected (
  VOID
  )
/*++

Routine Description:
    Function that checks to see if any PCH PCI Express Root Ports have detected an error.

Arguments:
    None
    
Returns: 
    TRUE     - An error was detected
    FALSE    - An error was not detected

--*/
{
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN ErrorDetected;
  BOOLEAN OriginalPcieFatalErrorState;

  Bus           = DEFAULT_PCI_BUS_NUMBER_PCH;
  Device        = PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS;
  Function      = PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1;
  ErrorDetected = FALSE;
  //
  // Save the current state of the PCIe Fatal error detected flag.
  // The function call to check if an error occurred on a root port may
  // set this flag.  This would cause the BNB ESI root port to ignore
  // the ESI error, but a NMI would occur without executing the PCH6 SMI handlers
  // to determine what device reported the error.
  //
  OriginalPcieFatalErrorState = mPcieFatalErrDetected;

  //
  // Check all PCH PCIe root ports for errors.
  //
  for (Function = PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1; Function <= PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8; Function++) {

    if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
      //
      // Check device present or not
      //
      continue;
    }
    if (!PciErrLibIsRootPortErrReportingEnabled (Bus, Device, Function)) {
      continue;
    }

    if (PciErrLibIsErrOnPcieRootPort (Bus, Device, Function)) {
      ErrorDetected = TRUE;
      //
      // Found a root port reporting an error, stop the search
      //
      break;
    }
  }

  mPcieFatalErrDetected = OriginalPcieFatalErrorState;

  return ErrorDetected;
}

VOID
ElogIchClearSerrStatus (
  VOID
  )
/*++

Routine Description:
    Subroutine for clearing the PCI SERR# status (and LPC IOCHK# status) in the
    NMI Status and Control Register.  This keeps the system from executing in an
    infinite SMI loop when the system is configured to SMI on NMI.

Arguments:
    None

Returns:
    VOID

--*/
{
  UINT8       Data;
  UINT8       OriginalErrorMask;
  //
  // Clear the PCI SERR# and LPC IOCHK# status in the NMI status and control register if set
  //
  Data = IoRead8 (R_PCH_NMI_SC);
  if ((Data & 0x0C) == 0) {
    //
    // SERR and IOCHK status already cleared
    //
    return ;
  }
  //
  // Clear and configure the NMI status and control register so SERR# and IOCHK# will cause NMI.
  //
  OriginalErrorMask = Data & 0x0C;
  //
  // SERR# and IOCHK# error reporting masks are in bits <3:2>
  //
  Data |= 0x0C;
  //
  // Set <3:2> for clearing SERR# and IOCHK# NMI Enables
  //
  Data &= 0x0F;
  //
  // Not allowed to set bits in the upper nibble
  //
  IoWrite8 (R_PCH_NMI_SC, Data );
  //
  // Clear status
  //
  Data &= 0x03;
  Data |= OriginalErrorMask;
  //
  // Restore original error reporting masks
  //
  IoWrite8 (R_PCH_NMI_SC, Data );
}


EFI_STATUS
ElogIchPciExHandlerSubFn (
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function
  )
{
  UINT8   Data8;
  UINT32  Data32;
  UINT8   CapOffset;
  BOOLEAN ErrorDetected = FALSE;

  //
  // Check if error reporting is enabled for this root port.
  //
  if (!PciErrLibIsRootPortErrReportingEnabled (Bus, Device, Function)) {
    return FALSE;
  }

  if (PciErrLibIsErrOnPcieRootPort (Bus, Device, Function)) {

    CapOffset = PciErrLibGetCapabilitiesOffset (Bus, Device, Function);
    if (CapOffset == 0) {
      return FALSE;
    }
    Data8 = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device,  Function, CapOffset + PCIE_DEVICE_STATUS_OFFSET));

    if (Data8 & B_PCIE_DEVICE_STATUS_COR_ERR) {
      //
      // Correctable
      //
      Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device,  Function, R_PCIEXP_AER_CES));
      Data32 |= V_PCIEXP_AER_CES;
      PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device,  Function, R_PCIEXP_AER_CES), Data32);
    } else if (Data8 & B_PCIE_DEVICE_STATUS_NF_ERR) {
      //
      // Non-Fatal
      //
      if(mAllowClearingOfUEStatus)  {
        Data32 = PciExpressRead32 (PCI_PCIE_ADDR(Bus, Device,  Function, R_PCIEXP_AER_UES));
        Data32 |= V_PCIEXP_AER_UES;
        PciExpressWrite32 (PCI_PCIE_ADDR(Bus, Device,  Function, R_PCIEXP_AER_UES), Data32);
      }
    } else if (Data8 & B_PCIE_DEVICE_STATUS_FAT_ERR) {
      //
      // Fatal
      //
    } else if (Data8 & B_PCIE_DEVICE_STATUS_UNSUP_REQ) {  // UR
      Data8 |= BIT3;
      PciExpressWrite8 (PCI_PCIE_ADDR(Bus, Device,  Function, CapOffset + PCIE_DEVICE_STATUS_OFFSET), Data8);
    }

    //
    // Error flagged on this root port, process and log the error.
    //
    ErrorDetected = PciErrLibPcieRootPortErrorHandler (Bus, Device, Function);
  }

  return ErrorDetected;

}
EFI_STATUS
ElogIchPciExHandler (
  IN UINT8            Bus
  )
/*++

Routine Description:
    Handler for Low Pin Count to PCI bridge device.

Arguments:

--*/
{

  UINT8   Function;
  UINT8   Device;
  Device = (UINT8)PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORTS;

  for (Function = 0; Function <= PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8; Function++) {

    ElogIchPciExHandlerSubFn (Bus, Device, Function);
  }    
  return EFI_SUCCESS;
}

BOOLEAN
ElogP2PHandler (
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function
  )
/*++

Routine Description:
    Handler for standard PCI to PCI bridge device.

Arguments:
    Bus - Device's bus number
    Device - Device's device number
    Function - Device's function number
    
Returns: 
    EFI_SUCCESS if succeed

--*/
{
  UINT8           SecondaryBusNumber;
  UINT8           SubordinateBusNumber;
  UINT16          SecondaryStatus;
  BOOLEAN         ErrorDetected;
  UINT16          ErrorMap;

  ErrorDetected   = FALSE;
  ErrorMap        = EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR |
                    EFI_PCI_STATUS_DETECTED_PARITY_ERROR |
                    EFI_PCI_STATUS_DATA_PARITY_ERROR_DETECTED |
                    EFI_PCI_STATUS_RECEIVED_TARGET_ABORT;
  //
  // Handle the primary bridge device
  //
  if (PciErrLibPciDeviceErrorHandler (Bus, Device, Function)) {
    ErrorDetected = TRUE;
  }
  //
  // Check the subordinate devices for errors
  //
  if (PciErrLibIsPciBridgeDevice (Bus, Device, Function)) {
                                                                //Secondary Status Register
    SecondaryStatus = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET));
    if ((SecondaryStatus & ErrorMap) == 0) {
      return ErrorDetected;
    }
    //
    // Clear the secondary status bits...
    //
    PciErrLibClearLegacyPciDeviceStatus (Bus, Device, Function);

    SecondaryBusNumber = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
    SubordinateBusNumber = PciExpressRead8 (PCI_PCIE_ADDR(Bus, Device, Function, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));

    for (Bus = SecondaryBusNumber; Bus <= SubordinateBusNumber; Bus++) {
      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
          if (PciErrLibIsDevicePresent (Bus, Device, Function)) {
            if (PciErrLibPciDeviceErrorHandler (Bus, Device, Function)) {
              ErrorDetected = TRUE;
            }
            PciErrLibClearLegacyPciDeviceStatus (Bus, Device, Function);
          }
        }
      }
    }
  }
  return ErrorDetected;
}

EFI_STATUS
ElogIchPciHandler (
  VOID
  )
{

  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN DetectedError;
  UINT8   Index;

  DetectedError = FALSE;
  Index         = 0;
  //
  // Call the device-specific handler for each device in the table...
  //
  while ((mIchPciDeviceTable[Index].Segment != INVALID_FLAG) &&
         (mIchPciDeviceTable[Index].Bus != INVALID_FLAG)) {

    Bus       = mIchPciDeviceTable[Index].Bus;
    Device    = mIchPciDeviceTable[Index].Device;
    Function  = mIchPciDeviceTable[Index].Function;

    //
    // Check device status before device error handler initialing
    //
    if (!PciErrLibIsDevicePresent (Bus, Device, Function)) {
      //
      // Check device present or not
      //
      Index++;
      continue;
    }

    if (mIchPciDeviceTable[Index].Bridge == TRUE)  {
      DetectedError = ElogP2PHandler (Bus, Device, Function);
    }
    else  {
      DetectedError = PciErrLibPciDeviceErrorHandler (Bus, Device, Function);
    }

    Index++;
  }

  if (DetectedError) {
    //
    // Clean up the error status registers in the ICH6 so we don't get caught in
    // a SMI loop.  The ICH PCI Express root port may have detected a correctable
    // error and the system should continue to run after the error was processed.
    //
    ElogIchClearSerrStatus ();
  }
  return EFI_SUCCESS;
}

EFI_STATUS
ElogIchDmiHandler (
  IN UINT8            Bus
  )
{

  UINT32    *UES;
  UINT32    *CES;
  UINT32    *CEM;
  UINT32    *RES;
  UINT32    RCBA;
  UINT32    CEMValue;
  UINT32    ErrorStatus;
  UINT8     Device;
  UINT8     Function;

  Device    = 0;
  Function  = 0;
  //
  // Get the RCBA base address
  //
  RCBA  = PciExpressRead32 (PCI_PCIE_ADDR(Bus, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_RCBA )) & 0xFFFFC000;
  UES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_UES);
  CES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_CES);
  CEM     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_CEM);
  RES     = (UINT32 *)(UINTN)(RCBA + R_PCH_RCRB_RES);

  ErrorStatus   = *RES;
  if(ErrorStatus & B_PCH_RCRB_RES_CR) {
    //
    // Check if the correctable-error reporting limit has already been reached.
    //
    if (mNumCorrectablePcieErrs < PCIE_CORRECTABLE_ERROR_COUNT_LIMIT) {
      //
      // Report the correctable error.
      //
      PciErrLibReportError (Bus, Device, Function, COR_ERROR_TYPE);
      mNumCorrectablePcieErrs++;
    } else {
      //
      // UnMask the Correctable errors
      //
      CEMValue  = *CEM;
      CEMValue  |= ( B_PCH_RCRB_CE_RE | B_PCH_RCRB_CE_BT | B_PCH_RCRB_CE_BD | B_PCH_RCRB_CE_RNR );
      *CEM      |= CEMValue;
    }
 
  } 
  if(ErrorStatus & B_PCH_RCRB_RES_ENR)  {
    //
    // Report the Uncorrectable error.
    //
    PciErrLibReportError (Bus, Device, Function, FATAL_ERROR_TYPE);

  }

  return EFI_SUCCESS;

}


EFI_STATUS
ProcessPCHUncorrectedError (
VOID
  )
/*++

Routine Description:
    This function performs the ICH error checking and processing functions
    TODO: This function should be split, one for fatal, one for uncorrected and one for corrected. 
Arguments:

Returns:
    EFI_SUCCESS

--*/
{
  EFI_STATUS  Status;
  UINT8       Data;
  UINT16      Data16;
  UINT32      Data32;
  UINT16      PmBase;
  
  Status = EFI_SUCCESS;
  //
  // Call the error handler for PCI related errors
  //
  Data = IoRead8(R_PCH_NMI_SC);
  if(Data & (B_PCH_NMI_SC_SERR_NMI_STS | B_PCH_NMI_SC_IOCHK_NMI_STS)) { // SERR# or IOCHK# signaled 

    { // all this code is for DEBUG proposes
      Data = IoRead8(R_PCH_NMI_SC); //mdrA
      DEBUG ((DEBUG_ERROR, "PlatformErrorLog@->ProcessPCHUncorrectedError: NMI_STS_CNT = %x\n",Data));//mdrA

       //
       // Get the PM Base Address
       //
       PmBase = PciRead16 (PCI_LIB_ADDRESS (
                 DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
                 PCI_DEVICE_NUMBER_PCH_LPC, //PCI Device(7:3)
                 PCI_FUNCTION_NUMBER_PCH_LPC, // Func(2:0)
                 R_PCH_LPC_ACPI_BASE // Register
                 ));

       //
       // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
       // so they use the whole 16 bit
       //
       PmBase = (UINT16) (PmBase & 0xFF80);


       Data16 = IoRead16 ((PmBase + R_ACPI_ALT_GP_SMI_STS));
       Data32 = IoRead32 ((PmBase + R_ACPI_SMI_STS));
       Data16 = IoRead16 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)));
    }
    Status = ElogIchErrorHandler ();

  }

  return Status;
}




EFI_STATUS
ElogIchErrorHandler (
  VOID
  )
{
  EFI_STATUS  Status;
  
  Status  = EFI_SUCCESS;
  
  if(mIchPciErrors == 1) {
    //
    // error log PCH Pci device handler
    //
    Status = ElogIchPciHandler ();
  }
  if (mIchPcieErrors == 1)  {
    //
    // error log PCH Pci-Express device handler
    //
    Status = ElogIchPciExHandler (mIchBusNum);
    Status = ElogIchDmiHandler (mIchBusNum);
  }

  //
  // NMI if a fatal error occurred.  
  // We treat PCI parity errors and PCIe Fatal errors since
  // data corruption may have occurred.
  //
  if ( (mPciSerrDetected && mNmiOnSerr) ||
       (mPciPerrDetected && mNmiOnPerr) ||
        mPcieFatalErrDetected  ) {
    ElogGenerateNmiNow ();
  }

  return Status;
}

void
DelayFn(UINT32 dCount)
// Description: This routine delays for specified number of micro seconds
// Input: wCount    Amount of delay (count in 15microsec)
//Output:   None
{
  UINT8   Reference;
  UINT8   Current;

  Reference = IoRead8 (0x61);
  Reference &= 0x10;


  while(dCount) {
    Current = IoRead8 (0x61);
    Current &= 0x10;
    if (Reference != Current)
    {
      Reference = Current;
      dCount--;
    }
  }
}


VOID
ElogGenerateNmiNow (
  VOID
  )
/*++

Routine Description:
    This function generates an NMI  to force an entry to the NMI handler..

Arguments:
    N/A

Returns:
    N/A

--*/
{

  UINT16  PmBase;
  UINT8   Data;
  UINT8   Save_Nmi2Smi_En;
  UINT8   Save_Port70;
  UINT16  Data16;

  //
  // Get the PM Base Address
  //
  PmBase = PciExpressRead16 (PCI_PCIE_ADDR(DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, PCI_FUNCTION_NUMBER_PCH_LPC, R_PCH_LPC_ACPI_BASE ));

  PmBase = (UINT16) (PmBase & 0xFF80);

  //
  // Read the NMI2SMI_EN bit, save it for future restore
  //
  Save_Nmi2Smi_En = IoRead8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)));

  //
  // Set the NMI2SMI_EN bit to 0
  //
  Data = (UINT8) (Save_Nmi2Smi_En & 0xFD);
  IoWrite8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), Data);

  //
  // Enable NMI_EN
  //
  Save_Port70 = IoRead8 (0x74);
  Data = (UINT8) (Save_Port70 & 0x7F);
  IoWrite8 (R_PCH_NMI_EN, Data);

  //
  // Set NMI_NOW = 1
  //
  Data = IoRead8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)));
  Data = (UINT8) (Data | 0x01);
  IoWrite8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), Data);

  //
  // Clear NMI_NOW = 0 by writing 1 to NMI_NOW bit
  //
  Data = IoRead8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)));
  Data = (UINT8) (Data | 0x01);
  IoWrite8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), Data);

  //
  // Restore NMI2SMI_EN
  //
  IoWrite8 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_CNT + 1)), Save_Nmi2Smi_En);

  //
  // Clear the MCHSERR_STS bit, bit 12
  //
  Data16 = IoRead16 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS )));
  Data16 = (UINT8) (Data16 | 0x1000);
  IoWrite16 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)), Data16);

  //
  // Clear the NMI2SMI_STS bit if set
  //
  Data16 = IoRead16 ((PmBase + (PCH_TCO_BASE + R_PCH_TCO1_STS)));
  if (Data16 & 0x0001) {
    //
    // check port 0x61
    //
    Data = IoRead8 (0x61);
    if (Data & 0x80) {
      Data = IoRead8 (0x61);
      Data  = (UINT8) (Data | 0x04);
      Data  = (UINT8) (Data & 0x0F);
      IoWrite8 (0x61, Data);
      Data = (UINT8) (Data & 0x0B);
      IoWrite8 (0x61, Data);
    }
  }
  //
  // Restore NMI_EN
  //
  IoWrite8 (R_PCH_NMI_EN, Save_Port70);
  return ;

}

EFI_STATUS
EnableAsyncSmiLogic (
  ERROR_LOG_SETUP_DATA  *ErrorLogSetupData
  )
{
  UINT8                       Data;
  UINT16                      Data16;
  UINT32                      Data32;
  //UINT32                      GpiRouteData;
  UINT16					  PmBase;

  //
  // Get the PM Base Address
  //
  PmBase = PciRead16 (PCI_LIB_ADDRESS(
            DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
            PCI_DEVICE_NUMBER_PCH_LPC,
            PCI_FUNCTION_NUMBER_PCH_LPC, 
            R_PCH_LPC_ACPI_BASE         // Register
            ));

  //
  // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
  // so they use the whole 16 bit
  //
  PmBase = (UINT16) (PmBase & 0xFF80);

  //
  // Clear all Spurious Sources of the SMI at the SMI_STS register offset PMBASE + 34
  //
  Data32 = IoRead32 ((PmBase + R_ACPI_SMI_STS));
  IoWrite32 ((PmBase + R_ACPI_SMI_STS), Data32);

  //
  // enable ICH error generation
  // Setup the GBL_SMI_EN=1 to only cause SMI.
  // Set bit 13 TCO_EN and bit 0 GBL_SMI_EN
  //
  Data16 = IoRead16 ((PmBase + R_ACPI_SMI_EN));
  Data16 = (UINT16) (Data16 | B_TCO_EN | B_GBL_SMI_EN );
  IoWrite16 ((PmBase + R_ACPI_SMI_EN), Data16);

  //
  // Setup NMI_EN = 0;
  //
  // NMI is accessible thru the RTC/NMI register, mapped to I/O address 0x71 and also aliased to
  // I/O address 0x74.
  // Bit 7 controls NMI operation...
  //
  Data = IoRead8 (R_PCH_NMI_EN);
  Data = (UINT8) (Data & 0x7F);
  IoWrite8 (R_PCH_NMI_EN, Data);

  //
  // Enable PCI_SERR_EN to generate NMIs
  //
  Data = IoRead8 (R_PCH_NMI_SC);
  Data &= (UINT8) (~B_PCH_NMI_SC_PCI_SERR_EN);
  IoWrite8 (R_PCH_NMI_SC, Data);
  

  //
  // Enable IOH and Processor uncorrectable error generation
  //
  mIohErrPinSmiEn = FALSE;

  if(ErrorLogSetupData->ErrorPinProgrammingForIIO == V_SYSMAP_NoInband) {
    mIohErrPinSmiEn = TRUE;
  }

  //
  // caterr# is wired to GPI8
  //
  
  /* //Disable to WA 4030876 temporarily 
  if (mIohErrPinSmiEn) {

    // Set ALT_GPI_SMI_EN for GPI8
    Data32 = IoRead32 ((PmBase + R_ACPI_ALT_GP_SMI_EN));

    Data32 |= (BIT2 | BIT8);
  
    IoWrite32 ((PmBase + R_ACPI_ALT_GP_SMI_EN), Data32);
    
    //
    // Route GPI08 to cause SMI from Caterr#.
    //
    GpiRouteData = PciRead32 (PCI_LIB_ADDRESS (
                   DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
                   PCI_DEVICE_NUMBER_PCH_LPC,
                   PCI_FUNCTION_NUMBER_PCH_LPC, 
                   R_PCH_LPC_GPI_ROUT  // Register
                   ));
  
    //
    // Route GPI08 to SMI.
    //
    GpiRouteData = (GpiRouteData & ~(BIT17 | BIT16)) | BIT16;
    
    PciWrite32 (PCI_LIB_ADDRESS (
      DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC, 
      R_PCH_LPC_GPI_ROUT ), // Register
      GpiRouteData
      );

    //
    // Register SMM handler for processor uncorrectable errors (CATERR# -> gpio08 of ich)
    //
    EnableGpiChildHandler (ICH_GPI_SMI_CAT_ERROR_PIN_CV);
  }
  */

  return EFI_SUCCESS;
}

