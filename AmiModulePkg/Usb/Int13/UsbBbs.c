//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file UsbBbs.c

**/

#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AmiUsbController.h>

/**
    Create BBS table for each USB mass storage device.

**/

VOID
CollectUsbBbsDevices (
    VOID
)
{
    EFI_STATUS  Status;
    EFI_USB_PROTOCOL        *AmiUsb;

    Status = gBS->LocateProtocol(&gEfiUsbProtocolGuid, NULL, (VOID**)&AmiUsb);
    if (EFI_ERROR(Status)) {
        return;
    }

    AmiUsb->InstallUsbLegacyBootDevices();
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
