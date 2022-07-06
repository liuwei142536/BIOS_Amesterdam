/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    OemCpuInit.c

Abstract:

    Oem Hooks file for CpuInit Module

--*/
#include <Token.h> // AptioV Server override
#include <Protocol/PchThermalLevels.h>
#include <Library/CpuPpmLib.h>
#include <Library/OemCpuInit.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

/**
  Function to send Pch Thermal Levels to pcode.
  
  @param  ppm               Pointer to CPU PM struct
  @param  PchThermalLimits  PCH thermal levels got from PchThermalLevels protocol
 **/
VOID
ProgramB2PPchTemp (EFI_CPU_PM_STRUCT *ppm, PCH_THERMAL_LEVELS *PchThermalLimits)
{
  UINT8   SocketNumber;
  UINT32 B2PRetVal;
  UINT32 TempData = 0;
  UINT8  Tjmax = 0;
  UINT8  ThrottleTemp = 0;
  const UINT8 TWO_DEGREES = 2;

  ThrottleTemp = PchThermalLimits->T1;
  ASSERT (ThrottleTemp <= UINT8_MAX - TWO_DEGREES);
  Tjmax = ThrottleTemp + TWO_DEGREES;

  SocketNumber = ppm->Info->SocketNumber;
  TempData = (Tjmax << 8) | (ThrottleTemp);

  B2PRetVal = ppm->CpuCsrAccess->Bios2PcodeMailBoxWrite(SocketNumber, (UINT32)MAILBOX_BIOS_CMD_WRITE_PCH_TEMP, TempData);
  if ((B2PRetVal & 0x000000ff) != MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND) {
    DEBUG ((EFI_D_INFO, "Sent PCH TjMax %d C and Throttle Temp %d C to pcode \n", Tjmax, ThrottleTemp));
  } else {
    DEBUG ((EFI_D_ERROR, "B2P commad, %2X is invalid \n", MAILBOX_BIOS_CMD_WRITE_PCH_TEMP));
  }
}

/**
  This is the Event call back function to get Pch Thermal Levels.
  
  @param  Event   Pointer to this event
  @param  Context Event hanlder private data 
 **/
VOID
EFIAPI
PchThermalLevelsCallBack (
  IN EFI_EVENT  Event,
  IN VOID *Context
  )
{
  EFI_STATUS   Status;
  PCH_THERMAL_LEVELS_PROTOCOL *PchThermalLevels;
  PCH_THERMAL_LEVELS PchThermalVals;

  Status = gBS->LocateProtocol (&gPchThermalLevelsProtocolGuid, NULL, &PchThermalLevels);
  if (!EFI_ERROR (Status)) {
    PchThermalLevels->GetPchThermalLevels(PchThermalLevels, &PchThermalVals);
  } else {
    DEBUG ((EFI_D_ERROR, "PchThermalLevelsProtocol is not published \n"));
    ASSERT (0);
  }

  //Send PCH T1 and T2 temperature levels to PCU using the B2P command, MAILBOX_BIOS_CMD_WRITE_PCH_TEMP (0xA0)
  ProgramB2PPchTemp((EFI_CPU_PM_STRUCT *)Context, &PchThermalVals);
  
  gBS->CloseEvent (Event);
  return;
} 

/**
  This is an OEM hook to initialize CPU thermal levels as per other external chip.
  
  @param  pCpuPmLib   Pointer to CPU PM struct
 **/
VOID
OemCpuOtherChipThermalInit (EFI_CPU_PM_STRUCT *pCpuPmLib)
{
  EFI_STATUS Status;
  EFI_EVENT AfterPchThermalLevelsInstalledEvent;
  void *Registration;

  Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_NOTIFY,
                      PchThermalLevelsCallBack,
                      (VOID *)pCpuPmLib,
                      &AfterPchThermalLevelsInstalledEvent
                      );

      Status = gBS->RegisterProtocolNotify (
                      &gPchThermalLevelsProtocolGuid,
                      AfterPchThermalLevelsInstalledEvent,
                      &Registration
                      );
      if(EFI_ERROR(Status)) {
	  	DEBUG ((EFI_D_ERROR, "Could not Register Protocol, gPchThermalLevelsProtocolGuid Notify Event\n"));
      	}
    return;
}
