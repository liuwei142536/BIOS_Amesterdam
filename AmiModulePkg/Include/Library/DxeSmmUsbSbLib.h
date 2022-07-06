//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**                 5555 Oakbrook Pkwy, Norcross, GA 30093                 **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file DxeSmmUsbSbLib.h
    Defines the DxeSmmUsbSblib library functions.
**/

#ifndef _DXE_SMM_USB_SB_LIB_H
#define _DXE_SMM_USB_SB_LIB_H

#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/UsbPolicy.h>
#include <Protocol/AmiUsbHid.h>

#define USB_HANDLE   0xFF555342       //'U''S''B'

EFI_STATUS EFIAPI USBSB_InstallXhciHwSmiHandler(VOID);
EFI_STATUS EFIAPI USBSB_InstallUsbIntTimerHandler(VOID);
EFI_STATUS EFIAPI USBSB_UninstallTimerHandlers(VOID);
EFI_STATUS EFIAPI USBSB_InstallSmiEventHandlers(VOID);
#endif

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**                 5555 Oakbrook Pkwy, Norcross, GA 30093                 **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
