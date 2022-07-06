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

/** @file DxeEhci.c
   AMI USB EHCI DXE driver
**/


#include <AmiDef.h>
#include <UsbDef.h>

/**
    This function is call to read debug register

    @param HcStruc   Ptr to the host controller structure
    @param BarIndex  Bar Index
    @param Offset    Offset for read.

    @retval Value    Return the value.

**/
UINT32
EFIAPI
EhciReadDebugReg(
    HC_STRUC    *HcStruc,
    UINT8       BarIndex,
    UINT32      Offset
)
{
    
    EFI_STATUS              Status;
    UINT32                  Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;
    
    Status = PciIo->Mem.Read(
                     PciIo,
                     EfiPciIoWidthUint32,
                     BarIndex,
                     Offset,
                     1,
                     &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Ehci: Read Debug Reg error: %r at %x\n", Status, Offset);
        Data = 0xFFFFFFFF;
    }
    return Data;
}
/**
    This function is call to enable legacy key repeat.

    @param HcStruc   Ptr to the host controller structure

    @retval EFI_SUCCESS  Success to enable legacy key repeat.

**/

EFI_STATUS
EFIAPI
EhciEnableLegacyKeyRepeat(
    HC_STRUC    *HcStruc
)
{
    return EFI_UNSUPPORTED;
}

/**
    This function returns the PCI register offset for the legacy
    support in EHCI controller

    @param HcStruc   Ptr to the host controller structure
    @param PciAddr   PCI address of the EHCI host controller

    @retval 0        If the feature is not present
           <>0       Legacy support capability offset

**/

UINT8
EFIAPI
EHCIGetLegacySupportOffset(
    HC_STRUC*   HcStruc,
    UINT16      PciAddr
)
{
    return 0;
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
