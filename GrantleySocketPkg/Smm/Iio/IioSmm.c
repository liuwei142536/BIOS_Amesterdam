/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/**@file
  IIO SMM Driver implementation file. 

  This file contains the user entry point 

  Copyright (c) 2009 - 2010, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.
**/


#include "IioSmm.h"

//
// Modular variables needed by this driver
//
EFI_ACPI_SMM_DEV              mAcpiSmm;
IIO_GLOBALS                   *IioGlobalData;


EFI_STATUS
EFIAPI
IioSmmDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
/*++

Routine Description:

  Initializes the IIO and uncore SMM Handler Driver.

Arguments:

  ImageHandle  -  The image handle of Wake On Lan driver.
  SystemTable  -  The starndard EFI system table.

Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.
  Other                 -  Error occured during execution. 

--*/
{
  EFI_STATUS                  Status;
  EFI_IIO_SYSTEM_PROTOCOL     *IioSystemProtocol;  

  //
  // Locate and Register to Parent driver
  //
  Status = RegisterDispatchDriver ();

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  ASSERT_EFI_ERROR (Status);
  IioGlobalData = IioSystemProtocol->IioGlobalData;

  return Status;
}

EFI_STATUS
RegisterDispatchDriver (
  VOID
  )
/*++

Routine Description:

  Register to dispatch driver.
  
Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Successfully init the device.
  Other        -  Error occured whening calling Dxe lib functions.
  
--*/
{
  UINTN                         Length;
  EFI_STATUS                    Status;
  EFI_SMM_SX_DISPATCH2_PROTOCOL  *SxDispatch;
  EFI_SMM_SX_REGISTER_CONTEXT   *EntryDispatchContext;
  EFI_SMM_SX_REGISTER_CONTEXT   *SleepS1Entry;
  EFI_SMM_SX_REGISTER_CONTEXT   *SleepS3Entry;
  EFI_SMM_SX_REGISTER_CONTEXT   *SleepS4Entry;
  EFI_SMM_SX_REGISTER_CONTEXT   *SleepS5Entry;

  Status = gBS->LocateProtocol (&gEfiSmmSxDispatch2ProtocolGuid, NULL, &SxDispatch);
  if (EFI_ERROR (Status)) return Status;

  Length = sizeof (EFI_SMM_SX_REGISTER_CONTEXT) * 4;
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, Length, &EntryDispatchContext);
  if (EFI_ERROR (Status)) return Status;

  SetMem (EntryDispatchContext, Length, 0);

  SleepS1Entry  = EntryDispatchContext;
  SleepS3Entry  = EntryDispatchContext++;
  SleepS4Entry  = EntryDispatchContext++;
  SleepS5Entry  = EntryDispatchContext++;

  //
  // Register entry phase call back function for S1
  //
  SleepS1Entry->Type  = SxS1;
  SleepS1Entry->Phase = SxEntry;
  Status = SxDispatch->Register(SxDispatch, SleepSxEntry, SleepS1Entry, &(mAcpiSmm.SleepS1Handle));

  //
  // Register entry phase call back function
  //
  SleepS3Entry->Type  = SxS3;
  SleepS3Entry->Phase = SxEntry;
  Status = SxDispatch->Register(SxDispatch, SleepSxEntry, SleepS3Entry, &(mAcpiSmm.SleepS3Handle));

  //
  // Register entry phase call back function for S4
  //
  SleepS4Entry->Type  = SxS4;
  SleepS4Entry->Phase = SxEntry;
  Status = SxDispatch->Register(SxDispatch, SleepSxEntry, SleepS4Entry, &(mAcpiSmm.SleepS4Handle));

  //
  // Register callback for S5 in order to workaround the LAN shutdown issue
  //
  SleepS5Entry->Type  = SxS5;
  SleepS5Entry->Phase = SxEntry;
  Status = SxDispatch->Register(SxDispatch, SleepSxEntry, SleepS5Entry, &(mAcpiSmm.SleepS5Handle));

  return Status;
}

VOID
PerIioSleepSxEntry (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST EFI_SMM_SX_REGISTER_CONTEXT   *DispatchContext,
  IN  UINT8                         BusNumber,
  IN  UINT8                         IioIndex
  )

/*++

Routine Description:

  Callback function entry for Sx sleep state.

Arguments:

  DispatchHandle   -  The handle of this callback, obtained when registering.
  DispatchContext  -  The predefined context which contained sleep type and phase.

Returns:

  EFI_SUCCESS            -  Operation successfully performed.
  EFI_INVALID_PARAMETER  -  Invalid parameter passed in.

--*/
{
  UINT8                     j;
  UINT32                    TimeOut;

  //
  // Send PME_TURN_OFF to all PCIE port devices in system
  //
  for ( j = 0 ; j < NUMBER_PORTS_PER_SOCKET; j++ ) {
    //
    // Clear Received PME_TO_ACK status (which is RW1CS)
    //
    PciExpressOr32 (PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, 
                                  (R_PCIE_PORT_PCIEMISCCTRLSTS + 4)), (B_PCIE_PORT_PCIEMISCCTRLSTS_Rcv_PME_TO_ACK >> 32));

    //
    // PO_WA_START: Added TimeOut to fix the Windows Shutdown issue.
    //
    TimeOut = 0x10000;
    if (IsSoftSdvPlatform() == FALSE) {
      while (TimeOut > 0) {
        //
        // Wait for "Received PME_TO_ACK" 
        //
        if((PciExpressRead32(PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function,
                                           R_PCIE_PORT_PCIEMISCCTRLSTS + 4)) & B_PCIE_PORT_PCIEMISCCTRLSTS_Rcv_PME_TO_ACK >> 32)) break;
        TimeOut--;
      }
    }
    //
    // PO_WA_END: Added TimeOut to fix the Windows Shutdown issue.
    //
  }

}

EFI_STATUS
SleepSxEntry (
  IN  EFI_HANDLE                    DispatchHandle,
  IN     CONST   EFI_SMM_SX_REGISTER_CONTEXT   *DispatchContext,
  IN OUT VOID    *CommBuffer                   OPTIONAL,
  IN OUT UINTN   *CommBufferSize               OPTIONAL
  )

/*++

Routine Description:

  Callback function entry for Sx sleep state.

Arguments:

  DispatchHandle   -  The handle of this callback, obtained when registering.
  DispatchContext  -  The predefined context which contained sleep type and phase.

Returns:

  EFI_SUCCESS            -  Operation successfully performed.
  EFI_INVALID_PARAMETER  -  Invalid parameter passed in.

--*/
{
  UINT8 i;
  if (((DispatchHandle == mAcpiSmm.SleepS3Handle) && (DispatchContext->Type == SxS3)) ||
        ((DispatchHandle == mAcpiSmm.SleepS4Handle) && (DispatchContext->Type == SxS4)) ||
        ((DispatchHandle == mAcpiSmm.SleepS5Handle) && (DispatchContext->Type == SxS5))) {

    for (i = 0 ; i < MaxIIO ; i++) {
      if (IioGlobalData->IioVData.SocketPresent[i]) {
        PerIioSleepSxEntry(DispatchHandle, DispatchContext, IioGlobalData->IioVData.SocketBaseBusNumber[i],i);
      }
    }
  }

  if ((DispatchHandle == mAcpiSmm.SleepS5Handle) && (DispatchContext->Type == SxS5)) {
  } else if ((DispatchHandle == mAcpiSmm.SleepS3Handle) && (DispatchContext->Type == SxS3)) {
  }

  return EFI_SUCCESS;

}
