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

/** @file UsbMisc.c
    AMI USB driver miscellaneous routines

**/


#include "Uhcd.h"

#include <Protocol/SmmControl2.h>
EFI_SMM_CONTROL2_PROTOCOL *gSmmCtl = NULL;


extern USB_GLOBAL_DATA             *gUsbData;
extern EFI_USB_PROTOCOL            *gAmiUsbController;

/**
    Generates SW SMI using global SmmCtl pointer.
    @param  Data      Sw Smi Value

**/

VOID
EFIAPI
USBGenerateSWSMI (
    UINT8   Data
)
{
    EFI_STATUS    Status;
    UINT8         SwSmiValue = Data;
    UINT8         DataSize = 1;

    if (gUsbData->UsbRuntimeDriverInSmm > USB_RUNTIME_DRV_MODE_0) {
        if (gSmmCtl == NULL) {
            Status = gBS->LocateProtocol(&gEfiSmmControl2ProtocolGuid, NULL, (VOID**)&gSmmCtl);
            if (EFI_ERROR(Status)) {
                return;
            }
        }
        if (gSmmCtl != NULL)
            gSmmCtl->Trigger(gSmmCtl, &SwSmiValue, &DataSize, 0, 0);
    }
}

/**
    This function invokes USB API handler.
    @param  Urp    Pointer to the URP structure

**/

VOID
EFIAPI
InvokeUsbApi (
    URP_STRUC  *Urp
)
{
    EFI_TPL OldTpl;

    if (gAmiUsbController->UsbInvokeApi) {
        OldTpl = gBS->RaiseTPL(TPL_NOTIFY);
        gAmiUsbController->UsbInvokeApi(Urp);
        gBS->RestoreTPL(OldTpl);
    } else {
        USBGenerateSWSMI(gUsbData->UsbSwSmi);
    }
}

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
