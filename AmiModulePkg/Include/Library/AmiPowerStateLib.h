//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  Definition of the AmiPowerStateLib library class.
*/
#ifndef __AMI_POWER_STATE_LIB__H__
#define __AMI_POWER_STATE_LIB__H__

#include <Uefi.h>

/**
  Returns Platform power state

  @param AcConnected Pointer to where return AC connected state (TRUE - platform is connected to AC, FALSE otherwise)
  @param BatteryCharge Pointer to where return Battery charge (0 - no battery, 1..100 - battery charge in percents)

  @retval  EFI_SUCCESS  Returned power state is valid
  @retval  Other        Error occurred during operation
*/
EFI_STATUS AmiGetPowerState (
    OUT BOOLEAN *AcConnected,
    OUT UINT16 *BatteryCharge
    );


#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
