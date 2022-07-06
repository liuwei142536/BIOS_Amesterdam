//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file
  Header file for the RtcWakeup Driver.

Copyright (c) 2006 - 2010, Intel Corporation. All rights reserved.<BR>

This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/
#ifndef _RTC_WAKEUP_H_
#define _RTC_WAKEUP_H_

#include <PiDxe.h>
#include <PchRegs.h>
#include <Protocol/SmmSxDispatch.h>
#include <Protocol/SmmBase2.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>

#define R_ICH_RTC_REGC 0x0C

  // Function Prototypes
  //
UINT8
ReadRtcIndex (
  IN UINT8 Index
  );

void
WriteRtcIndex (
  IN UINT8 Index,
  IN UINT8 Value
  );

void
SetWakeupTime (
  IN EFI_TIME   *Time
  );

EFI_STATUS
S5SleepEntryCallBack (
  IN EFI_HANDLE                      DispatchHandle,
  IN CONST VOID                      *DispatchContext,
  IN OUT VOID                        *CommonBuffer,
  IN OUT UINTN                       *CommonBufferSize
  );

EFI_STATUS
RegisterToDispatchDriver (
  VOID
);

EFI_STATUS
InitializeRTCWakeup (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  );

typedef struct {
  UINT8 RTCWakeupDate;
  UINT8 RTCWakeupTimeHour;
  UINT8 RTCWakeupTimeMinute;
  UINT8 RTCWakeupTimeSecond;
} SMMData;

#endif
