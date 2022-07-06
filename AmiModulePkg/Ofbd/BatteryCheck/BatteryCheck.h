//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file 
BatteryCheck.h

Header file for the BatteryCheck

*/

#ifndef _EFI_EC_BATTERY_CHECK_STATUS_H_
#define _EFI_EC_BATTERY_CHECK_STATUS_H_
#ifdef __cplusplus
extern "C" {
#endif

VOID BatteryCheckEntry (IN VOID *Buffer, IN OUT UINT8 *pOFBDDataHandled);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif


