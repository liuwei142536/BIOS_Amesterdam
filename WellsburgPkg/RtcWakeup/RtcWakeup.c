/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/** @file

  RtcWakeup Driver 

  Copyright (c) 2009 - 2010, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.


**/
#include "RtcWakeup.h"
#include "Guid\SetupVariable.h" // AptioV server override: Use RtcWakeup setup options instead of PCDs.

// Global variable
//
EFI_SMM_BASE2_PROTOCOL  *mSmmBase;
SMMData                gSmmData;
UINT32                 mPmBase;

UINT8
ReadRtcIndex (
  IN UINT8 Index
  )
/*++

Routine Description:

  Reads the RTC Index register

Arguments:

  Index - Index register

Returns:

  Value in Index register

--*/
{
  UINT8 volatile  Value;
  //
  // Check if Data Time is valid
  //
  if (Index <= 9) {
    do {
      IoWrite8 (R_PCH_RTC_INDEX, R_PCH_RTC_REGA | B_PCH_RTC_REGA_UIP);
      Value = IoRead8 (R_PCH_RTC_TARGET);
    } while (Value & B_PCH_RTC_REGA_UIP);
  }
  IoWrite8 (R_PCH_RTC_INDEX, Index | B_PCH_RTC_REGA_UIP);
  //
  // Read register.
  //
  Value = IoRead8 (R_PCH_RTC_TARGET);
  if (Index <= 9) {
    Value = BcdToDecimal8 (Value);
  }
  return (UINT8) Value;
}

void
WriteRtcIndex (
  IN UINT8 Index,
  IN UINT8 Value
  )
/*++

Routine Description:

  Writes to an RTC Index register

Arguments:

  Index - Index to be written
  Value - Value to be written to Index register

Returns:

  VOID

--*/
{
  IoWrite8 (R_PCH_RTC_INDEX, Index | B_PCH_RTC_REGA_UIP);
  if (Index <= 9) {
    Value = DecimalToBcd8 (Value);
  }
  //
  // Write Register.
  //
  IoWrite8 (R_PCH_RTC_TARGET, Value);
}

void
SetWakeupTime (
  IN EFI_TIME     *Time
  )
/*++

Routine Description:

  Sets the time to RTC register

Arguments:

  Time  - Time to be set

Returns:

  VOID

--*/
{
  UINT8 Value;
  WriteRtcIndex (5, Time->Hour);
  WriteRtcIndex (3, Time->Minute);
  WriteRtcIndex (1, Time->Second);
  //
  // Set Enable
  //
  Value = ReadRtcIndex (R_PCH_RTC_REGB);
  Value |= 1 << 5;
  WriteRtcIndex (R_PCH_RTC_REGB, Value);
}

EFI_STATUS
S5SleepEntryCallBack (
  IN EFI_HANDLE                               DispatchHandle,
  IN CONST VOID                               *DispatchContext,
  IN OUT VOID                                 *CommonBuffer,
  IN OUT UINTN                                *CommonBufferSize
  )
/*++

Routine Description:

  Callback handle on S5 

Arguments:

  DispatchHandle  - DispatchHandle
  DispatchContext - Context with which handle was dispatched

Returns:

  VOID

--*/
{
  //
  // Enable RTC wake
  //
  EFI_TIME  Time;

  Time.Hour   = gSmmData.RTCWakeupTimeHour;
  Time.Minute = gSmmData.RTCWakeupTimeMinute;
  Time.Second = gSmmData.RTCWakeupTimeSecond;
  //
  // Clear RTC PM1 status
  //
  IoWrite16 (mPmBase, (IoRead16 (mPmBase) | (1 << N_PCH_ACPI_PM1_EN_RTC)));
  SetWakeupTime (&Time);
  //
  // set RTC_EN bit in PM1_EN to wake up from the alarm
  //
  IoWrite16 (mPmBase + R_PCH_ACPI_PM1_EN, (IoRead16 (mPmBase + R_PCH_ACPI_PM1_EN) | (1 << N_PCH_ACPI_PM1_EN_RTC)));

  return EFI_SUCCESS;
  
}

EFI_STATUS
RegisterToDispatchDriver (
  VOID
)
/*++

Routine Description:

  This function is called from SMM during SMM registration.

Arguments:

  ImageHandle - ImageHandle Pointer
  SystemTable - Efi SystemTable Pointer

Returns:

  EFI_STATUS

--*/
{
  EFI_STATUS                    Status;
  EFI_SMM_SX_REGISTER_CONTEXT   SxDispatchContext;
  EFI_SMM_SX_DISPATCH2_PROTOCOL  *SxDispatchProtocol;
  EFI_HANDLE                    SxDispatchHandle;
    
  Status = gSmst->SmmLocateProtocol (
                  &gEfiSmmSxDispatch2ProtocolGuid,
                  NULL,
                  &SxDispatchProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Register the callback for S5 entry
  //
  SxDispatchContext.Type  = SxS5;
  SxDispatchContext.Phase = SxEntry;
  Status = SxDispatchProtocol->Register (
                        SxDispatchProtocol,
                        S5SleepEntryCallBack,
                        &SxDispatchContext,
                        &SxDispatchHandle
                        );
  return Status;
}

EFI_STATUS
InitializeRtcWakeup (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
/*++

Routine Description:

  This is the entrypoint of the RTCWakeup driver.

Arguments:

  ImageHandle - ImageHandle
  SystemTable - Efi SystemTable Pointer

Returns:

  EFI_STATUS

--*/
{
  EFI_STATUS            Status;
  UINT8                 Value;

// AptioV server override Start: Use RtcWakeup setup options instead of PCDs.
  SYSTEM_CONFIGURATION  SystemConfiguration;
  UINTN                 VariableSize = sizeof(SYSTEM_CONFIGURATION);

  //
  // Read Intel RC setup options.
  //
  Status = gRT->GetVariable (
                  L"IntelSetup",
                  &gEfiSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SystemConfiguration
                  );  
  if ( EFI_ERROR (Status) ) {
    //
    // Error case, load default values.
    //
    SystemConfiguration.WakeOnRTCS5 = 0;
  }
// AptioV server override End: Use RtcWakeup setup options instead of PCDs.

  mPmBase = PciRead32(PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_ACPI_BASE)) & (~BIT0);    
  //
  // Disable RTC alarm and clear RTC PM1 status
  // AptioV server override start: Commeting code for SCT RuntimeServicesTest\TimeServicesTest 
  //Value = ReadRtcIndex (R_PCH_RTC_REGB);
  //Value &= ~((UINT8) 1 << 5);
  //WriteRtcIndex (R_PCH_RTC_REGB, Value);
  // AptioV server override end: Commeting code for SCT RuntimeServicesTest\TimeServicesTest 
  
  //
  // Clear Alarm Flag (AF) by reading RTC Reg C
  //
  Value = ReadRtcIndex (R_PCH_RTC_REGC);
  IoWrite16 (mPmBase, (IoRead16 (mPmBase) | (1 << 10)));

// AptioV server override Start: Use RtcWakeup setup options instead of PCDs.
#if 0
  if (PcdGetBool(PcdWakeOnRTCS5)) {
    gSmmData.RTCWakeupTimeHour    = PcdGet8(PcdRtcWakeupTimeHour);
    gSmmData.RTCWakeupTimeMinute  = PcdGet8(PcdRtcWakeupTimeMinute);
    gSmmData.RTCWakeupTimeSecond  = PcdGet8(PcdRtcWakeupTimeSecond);    
#else
  if ( SystemConfiguration.WakeOnRTCS5 ) {
    gSmmData.RTCWakeupTimeHour    = SystemConfiguration.RTCWakeupTimeHour;
    gSmmData.RTCWakeupTimeMinute  = SystemConfiguration.RTCWakeupTimeMinute;
    gSmmData.RTCWakeupTimeSecond  = SystemConfiguration.RTCWakeupTimeSecond;
#endif
// AptioV server override End: Use RtcWakeup setup options instead of PCDs.

    //
    // Locate and Register to Parent driver
    //
    Status = RegisterToDispatchDriver ();
    if (EFI_ERROR (Status)) {
      return Status;
    }   
  }
  return EFI_SUCCESS;
}
