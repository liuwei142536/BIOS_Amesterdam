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

/** @file DxeXhci.c
   AMI USB XHCI DXE driver
**/


#include <AmiDef.h>
#include <UsbDef.h>
#include <Xhci.h>

UINT32      EFIAPI HcReadHcMem(HC_STRUC*, UINT32);
VOID        EFIAPI HcWriteHcMem(HC_STRUC*, UINT32, UINT32);

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
    
    Data = HcReadHcMem(HcStruc, Offset);
    Data |= LShiftU64(HcReadHcMem(HcStruc, (UINT32)(Offset + sizeof(UINT32))), 32);

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
    
    HcWriteHcMem(HcStruc, Offset, (UINT32)Data);
    HcWriteHcMem(HcStruc, (UINT32)(Offset + sizeof(UINT32)), (UINT32)(RShiftU64(Data, 32)));

}

/**
    This routine is call to enable legacy register
    
    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS    Success to enable legacy register
        
**/

EFI_STATUS
EFIAPI
XhciEnableLegacyReg(
    HC_STRUC*            HcStruc
)
{
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
    HC_STRUC*            HcStruc
)
{
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
