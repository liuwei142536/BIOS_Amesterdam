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

/** @file DxeElib.c
    AMI USB MEM/IO/PCI access routines

**/

#include <Library/DevicePathLib.h>
#include <Protocol/DevicePath.h>
#include <Protocol/PciIo.h>
#include <AmiDef.h>
#include <UsbDef.h>
#include <IndustryStandard/Pci.h>

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;

/**
    This routine delays for specified number of micro seconds

    @param Usec      Amount of delay (count in 1 microsec)


**/

VOID
EFIAPI
FixedDelay(
    UINTN           Usec
)
{
    gBS->Stall(Usec);
    return;
}

/**
    This routine reads a dword value from the PCI configuration
    register space of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to read
    
    @retval Value      Return the value.

**/

UINT32
EFIAPI
HcReadPciReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    EFI_STATUS              Status;
    UINT32                  Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, Offset, 1, &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Pci Io Read error: %r at %x\n", Status, Offset);
        Data = 0xFFFFFFFF;
    }
    return Data;
}

/**
    This routine writes a dword value to the PCI configuration
    register space of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to write
    @param Data        Value to write

**/

VOID
EFIAPI
HcWritePciReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Data
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Pci.Write(PciIo, EfiPciIoWidthUint32, Offset, 1, &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Pci Io Write32 error: %r at %x\n", Status, Offset);
    }

}

/**
    This routine writes a word value to the PCI configuration
    register space of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to write
    @param Data        Value to write

**/

VOID
EFIAPI
HcWordWritePciReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT16      Data
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Pci.Write(PciIo, EfiPciIoWidthUint16, Offset, 1, &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Pci Io Write16 error: %r at %x\n", Status, Offset);
    }
}

/**
    This routine reads a dword value from the MMIO register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to read
    
    @retval Value    Return the value.

**/

UINT32
EFIAPI
HcReadHcMem(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    EFI_STATUS              Status;
    UINT32                  Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, Offset, 1, &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Pci Io MMIO Read error: %r at %x\n", Status, Offset);
        Data = 0xFFFFFFFF;
    }
    return Data;
}


/**
    This routine writes a dword value to the MMIO register 
    of the host controller.
    
    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to write
    @param Data        Value to write
        
**/
VOID
EFIAPI
HcWriteHcMem(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Data
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Mem.Write(PciIo, EfiPciIoWidthUint32, 0, Offset, 1, &Data);
    if (EFI_ERROR(Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Pci Io MMIO Write error: %r at %x\n", Status, Offset);
    }
}

/**
    This routine reads a byte value from the Io register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to read
    
    @retval Value    Return the value.

**/

UINT8
EFIAPI
HcByteReadHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    EFI_STATUS              Status;
    UINT8                   Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Read(PciIo, EfiPciIoWidthUint8, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
    return Data;
}

/**
    This routine writes a byte value to the Io register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to write
    @param Data        Value to write
        
**/

VOID
EFIAPI
HcByteWriteHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT8       Data
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Write(PciIo, EfiPciIoWidthUint8, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
}

/**
    This routine reads a word value from the Io register 
    of the host controller.

    @param HcStruc    HcStruc pointer
    @param Offset     Register offset to read
    @retval Value    Return the value.


**/

UINT16
EFIAPI
HcWordReadHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    EFI_STATUS              Status;
    UINT16                  Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Read(PciIo, EfiPciIoWidthUint16, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
    return Data;
}

/**
    This routine writes a word value to the Io register 
    of the host controller.

    @param HcStruc    HcStruc pointer
    @param Offset     Register offset to write
    @param Data       Value to write
        
**/

VOID
EFIAPI
HcWordWriteHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT16      Data
)
{
    EFI_STATUS             Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Write(PciIo, EfiPciIoWidthUint16, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
}

/**
    This routine reads a dword value from the Io register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to read
    @retval Value    Return the value.

**/

UINT32
EFIAPI
HcDwordReadHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    EFI_STATUS              Status;
    UINT32                  Data = 0;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Read(PciIo, EfiPciIoWidthUint32, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
    return Data;
}

/**
    This routine writes a dword value to the Io register 
    of the host controller.

    @param HcStruc    HcStruc pointer
    @param Offset     Register offset to write
    @param Data       Value to write
        
**/

VOID
EFIAPI
HcDwordWriteHcIo(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Data
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    Status = PciIo->Io.Write(PciIo, EfiPciIoWidthUint32, 4, Offset, 1, &Data);
    ASSERT_EFI_ERROR(Status);
}

/**
    This routine uses Map of the DMA services for DMA operations. 

    @param HcStruc     HcStruc pointer
    @param Direction   Data direction 
    @param BufferAddr  BufferAddr for DmaMap.
    @param BufferSize  BufferSize for DmaMap
    @param PhyAddr     Phyaddr to access
    @param Mapping     A resulting value to pass to HcDmaUnmap.
    
    @retval USB_SUCCESS  Success to DMA map.
**/

UINT8
EFIAPI
HcDmaMap(
    HC_STRUC    *HcStruc,
    UINT8       Direction,
    UINT8       *BufferAddr,
    UINT32      BufferSize,
    UINT8       **PhyAddr,
    VOID        **Mapping
)
{

    EFI_PCI_IO_PROTOCOL_OPERATION       Operation;
    EFI_PHYSICAL_ADDRESS                Addr;
    EFI_STATUS                          Status;
    UINTN                               Bytes = BufferSize;
    EFI_PCI_IO_PROTOCOL                 *PciIo = HcStruc->PciIo;
    UINT8                               *MemBlockEnd;

    if (gUsbData == NULL || gUsbDataList == NULL) {
        *PhyAddr = BufferAddr;
        return USB_SUCCESS;
    }
    MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));
    if ((!(gUsbData->UsbStateFlag & USB_FLAG_EDKII_IOMMU_PROTOCOL_ON)) ||
        (HcStruc->MemBlockStartPhy == NULL) ||
        ((UINTN)BufferAddr < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)BufferAddr) > (UINTN)MemBlockEnd)) {
        if (Direction & BIT7) {
            Operation = EfiPciIoOperationBusMasterWrite;
        } else {
            Operation = EfiPciIoOperationBusMasterRead;
        }

        Status = PciIo->Map(PciIo, Operation, BufferAddr, &Bytes, &Addr, Mapping);

        if (EFI_ERROR(Status) || Bytes != BufferSize) {
            *PhyAddr = BufferAddr;
            return USB_ERROR;
        }

        *PhyAddr = (UINT8*)Addr;
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Dxe HcDmaMap PhyAddr %x\n", Addr);
    } else {
        *PhyAddr = (UINT8*)((UINTN)(HcStruc->MemBlockStartPhy) + ((UINTN)BufferAddr - (UINTN)(gUsbDataList->MemBlockStart)));
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Dxe HcDmaMap BufferAddr %x\n", *PhyAddr);
    }
    return USB_SUCCESS;
}

/**
    This routine uses UnMap of the DMA services for DMA operations. 

    @param HcStruc    HcStruc pointer
    @param Mapping     The mapping value returned from HcDmaMap.
    
    @retval USB_SUCCESS  Success to UnDMA map.
**/

UINT8
EFIAPI
HcDmaUnmap(
    HC_STRUC    *HcStruc,
    VOID        *Mapping
)
{
    EFI_STATUS              Status;
    EFI_PCI_IO_PROTOCOL     *PciIo = HcStruc->PciIo;

    if (Mapping != NULL) {
        Status = PciIo->Unmap(PciIo, Mapping);
        if (EFI_ERROR(Status)) {
            return USB_ERROR;
        }
    }
    return USB_SUCCESS;
}

/**
    This routine is call to validate USB data.

    @param UsbData    Usb global data.
    
    @retval EFI_ABORTED  Aborted this validation
**/
EFI_STATUS
EFIAPI
AmiUsbGlobalDataValidation(
    USB_GLOBAL_DATA *UsbData
)
{
    return EFI_ABORTED;
}

/**
    This routine is call to validate memory buffer.

    @param Buffer        The buffer to validate
    @param BufferSize    The buffer size to validate
    
    @retval EFI_ABORTED  Aborted this validation
**/
EFI_STATUS
EFIAPI
AmiUsbValidateMemoryBuffer (
    CONST VOID   *Buffer,
    CONST UINTN  BufferSize
)
{
    return EFI_ABORTED;
}

/**
    This routine is call to validate MMIO buffer.

    @param Buffer        The buffer to validate
    @param BufferSize    The buffer size to validate
    
    @retval EFI_ABORTED  Aborted this validation
**/
EFI_STATUS
EFIAPI
AmiUsbValidateMmioBuffer (
CONST VOID   *Buffer,
CONST UINTN  BufferSize
)
{
    return EFI_ABORTED;
}

/**
    This routine is call to reset system.

    @retval EFI_SUCCESS  Success to reset system.
**/
EFI_STATUS
EFIAPI
AmiUsbResetSystem(
    VOID
)
{
    return EFI_SUCCESS;
}

/**
    This function check whether the controller is behind bridge.
    @param  DevicePath    pointer to EFI_DEVICE_PATH_PROTOCOL structure

    @retval TRUE          External controller
    @retval FALSE         Not External controller

**/

BOOLEAN
EFIAPI 
IsExternalController (
    IN EFI_DEVICE_PATH_PROTOCOL    *DevicePath
)
{
    EFI_STATUS                  Status;
    EFI_DEVICE_PATH_PROTOCOL    *Dp = DevicePath;
    EFI_DEVICE_PATH_PROTOCOL    *DpPciNode = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *DpBridge = NULL;
    EFI_HANDLE                  Bridge = NULL;
    EFI_PCI_IO_PROTOCOL         *PciIo = NULL;
    UINT8                       PciClass[3];

    while (!IsDevicePathEnd (Dp)) {
        if ((Dp->Type == HARDWARE_DEVICE_PATH) &&
            (Dp->SubType == HW_PCI_DP)) {
            DpPciNode = Dp;
            break;
        }
        Dp = NextDevicePathNode(Dp);
    }
    if (DpPciNode == NULL) {
        return FALSE;
    }

    DpBridge = DuplicateDevicePath (DevicePath);
    if (DpBridge == NULL) {
        return FALSE;
    }

    Dp = (EFI_DEVICE_PATH_PROTOCOL*)((UINTN)DpBridge + 
            ((UINTN)NextDevicePathNode(DpPciNode) - (UINTN)DevicePath));

    Dp->Type = END_DEVICE_PATH_TYPE;
    Dp->SubType = END_ENTIRE_DEVICE_PATH_SUBTYPE;
    SetDevicePathNodeLength(Dp, sizeof(EFI_DEVICE_PATH_PROTOCOL));

    Dp = DpBridge;
    Status = gBS->LocateDevicePath (
                      &gEfiPciIoProtocolGuid,
                      &Dp,
                      &Bridge
                      );
    gBS->FreePool (DpBridge);
    if (EFI_ERROR (Status)) {
        return FALSE;
    }

    Status = gBS->HandleProtocol (Bridge, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
    if (EFI_ERROR (Status)) {
        return FALSE;
    }

    Status = PciIo->Pci.Read (
                    PciIo,
                    EfiPciIoWidthUint8,
                    PCI_CLASSCODE_OFFSET,
                    sizeof(PciClass),
                    PciClass
                    );
    if (EFI_ERROR (Status)) {
        return FALSE;
    }

    if (PciClass[2] != PCI_CLASS_BRIDGE || 
        PciClass[1] != PCI_CLASS_BRIDGE_P2P) {
        return FALSE;
    }

    return TRUE;
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
