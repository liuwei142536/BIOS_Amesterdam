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

/** @file SmmXhci.c
   AMI USB XHCI SMM driver
**/


#include <AmiDef.h>
#include <UsbDef.h>
#include <Xhci.h>

UINT32      EFIAPI HcReadHcMem(HC_STRUC*, UINT32);
VOID        EFIAPI HcWriteHcMem(HC_STRUC*, UINT32, UINT32);
VOID        EFIAPI HcSetHcMem(HC_STRUC*, UINT32, UINT32);
extern USB_GLOBAL_DATA *gUsbData;
/**
    HC may or may not support 64-bit writes to MMIO area. If it does, write
    Data directly, otherwise split into two DWORDs.
    
    @param HcStruc   Ptr to the host controller structure
    @param Usb3Hc    Ptr to the XHCI host controller structure
    @param Address   Address.
    
    @retval Value    Return the value.

**/

UINT64
EFIAPI
XHCI_Mmio64Read(
    HC_STRUC             *HcStruc,
    USB3_HOST_CONTROLLER *Usb3Hc,
    UINTN                Address
)
{
    UINT64    Data = 0;
    UINT32    Offset;

    Offset = (UINT32)(Address - HcStruc->BaseAddress);

    if ((Offset + sizeof(UINT64)) > HcStruc->BaseAddressSize) {
        return 0;
    }

    if (Usb3Hc->CapRegs.HccParams1.Ac64) {
        Data = MmioRead64(Address);
    } else {
        Data = MmioRead32(Address);
        Data |= LShiftU64(MmioRead32((Address + sizeof(UINT32))), 32);
    }
    return Data;
}
/**
    HC may or may not support 64-bit writes to MMIO area. If it does, write
    Data directly, otherwise split into two DWORDs.
    
    @param HcStruc   Ptr to the host controller structure
    @param Usb3Hc    Ptr to the XHCI host controller structure
    @param Address   Address.
    @param Data      This value for write.

**/
VOID
EFIAPI
XHCI_Mmio64Write(
    HC_STRUC             *HcStruc,
    USB3_HOST_CONTROLLER *Usb3Hc,
    UINTN                Address,
    UINT64               Data
)
{
    UINT32  Offset;

    Offset = (UINT32)(Address - HcStruc->BaseAddress);

    if ((Offset + sizeof(UINT64)) > HcStruc->BaseAddressSize) {
        return;
    }
    
    if (Usb3Hc->CapRegs.HccParams1.Ac64) {
        MmioWrite64(Address, Data);
    }
    else {
        MmioWrite32(Address, (UINT32)Data);
        MmioWrite32((Address + sizeof(UINT32)), (UINT32)(RShiftU64(Data, 32)));
    }
}

/**
    This routine is call to enable legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to enable legacy register
        
**/

EFI_STATUS
EFIAPI
XhciEnableLegacyReg (
    HC_STRUC    *HcStruc
)
{
    USB3_HOST_CONTROLLER *Usb3Hc = (USB3_HOST_CONTROLLER*)HcStruc->Usb3HcData;
    if (((!(HcStruc->HcFlag & HC_STATE_EXTERNAL)) || (gUsbData->XhciEventServiceMode > 0)) &&
        (!(HcStruc->HcFlag & HC_STATE_IRQ))) {
        // Enable USB SMI, Ownership Change SMI and clear all status
        HcSetHcMem(HcStruc, Usb3Hc->UsbLegSupOffSet + XHCI_LEGACY_CTRL_STS_REG, 
                XHCI_SMI_ENABLE | XHCI_SMI_OWNERSHIP_CHANGE_ENABLE |
                XHCI_SMI_OWNERSHIP_CHANGE | XHCI_SMI_PCI_CMD |
                XHCI_SMI_PCI_BAR);
    }

    return EFI_SUCCESS;
}
/**
    This routine is call to disable legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to Disable legacy register
        
**/
EFI_STATUS
EFIAPI
XhciDisableLegacyReg(
    HC_STRUC    *HcStruc
)
{
    UINT32                LegCtlStsReg;
    USB3_HOST_CONTROLLER  *Usb3Hc = (USB3_HOST_CONTROLLER*)HcStruc->Usb3HcData;

    if (((!(HcStruc->HcFlag & HC_STATE_EXTERNAL)) || (gUsbData->XhciEventServiceMode > 0)) &&
        (gUsbData->UsbRuntimeDriverInSmm>USB_RUNTIME_DRV_MODE_0) && (!(HcStruc->HcFlag & HC_STATE_IRQ))) {
        // Disable USB SMI and Clear all status
        LegCtlStsReg = HcReadHcMem(HcStruc, Usb3Hc->UsbLegSupOffSet + XHCI_LEGACY_CTRL_STS_REG);
        LegCtlStsReg &= ~XHCI_SMI_ENABLE;
        LegCtlStsReg |= XHCI_SMI_OWNERSHIP_CHANGE | XHCI_SMI_PCI_CMD | XHCI_SMI_PCI_BAR;
        HcWriteHcMem(HcStruc, Usb3Hc->UsbLegSupOffSet + XHCI_LEGACY_CTRL_STS_REG, LegCtlStsReg);
    }
    return EFI_SUCCESS;
}

/**
    This routine is call to uninstall key repeat.
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to uninstall key repeat.
        
**/

EFI_STATUS
EFIAPI
XhciUninstallKeyRepeat(
    HC_STRUC*   HcStruc
)
{
    if (HcStruc->HwSmiHandle != NULL) {
        USBKeyRepeat(HcStruc, 3);
    }
    return EFI_SUCCESS;
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
