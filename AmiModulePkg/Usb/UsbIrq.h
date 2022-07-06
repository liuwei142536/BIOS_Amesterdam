//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**                 5555 Oakbrook Pkwy, Norcross, GA 30093                 **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file UsbIrq.h

**/

#ifndef _AMIUSB_IRQ_H
#define _AMIUSB_IRQ_H

typedef struct _AMI_USB_ISR_PROTOCOL AMI_USB_ISR_PROTOCOL;

typedef EFI_STATUS (EFIAPI *AMI_INSTALL_USB_ISR) (
    IN AMI_USB_ISR_PROTOCOL    *This
);

typedef EFI_STATUS (EFIAPI *AMI_INSTALL_USB_LEGACY_ISR) (
    IN AMI_USB_ISR_PROTOCOL    *This
);

struct _AMI_USB_ISR_PROTOCOL {
    AMI_INSTALL_USB_ISR             InstallUsbIsr;
    AMI_INSTALL_USB_LEGACY_ISR      InstallLegacyIsr;
};

#endif

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**                 5555 Oakbrook Pkwy, Norcross, GA 30093                 **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
