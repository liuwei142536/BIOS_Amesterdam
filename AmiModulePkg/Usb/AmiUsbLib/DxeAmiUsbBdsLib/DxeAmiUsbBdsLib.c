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

/** @file DxeAmiUsbBdsLib.c

**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiUsbHcdLib.h>
#include <Protocol/AmiUsbController.h>
#include <Protocol/SmmControl2.h>
#include <AmiUsb.h>
#include <Xhci.h>
#include <Ehci.h>
#include <Uhcd.h>
#include <IndustryStandard/Pci.h>

USB_GLOBAL_DATA           *gUsbData = NULL;
USB_DATA_LIST             *gUsbDataList = NULL;
EFI_HANDLE                gUsbXhciHcHandle[MAX_USB_HC];
EFI_HANDLE                gUsbEhciHcHandle[MAX_USB_HC];
EFI_SMM_CONTROL2_PROTOCOL *gSmmCtl = NULL;
/**
    This function allocates XHCI scratchpad buffers. Data initialization will
    be done later, in SMI XhciStart function.
    @param  PciIo   Pointer to the PciIo structure
    @param  Usb3Hc  Pointer to USB3 host controller

    @retval EFI_STATUS Status of the operation
    
    @note   Usb3Hc->DcbaaPtr points to the beginning of memory block first 2048 Bytes
            of which is used for DCBAA.

**/
EFI_STATUS
XhciAllocateScratchpadBuffer (
    IN USB3_HOST_CONTROLLER  *Usb3Hc
)
{
    UINT32  NumBufs =  ((Usb3Hc->CapRegs.HcsParams2.MaxScratchPadBufsHi) << 5) + 
                        Usb3Hc->CapRegs.HcsParams2.MaxScratchPadBufsLo;
    UINT16      Count;
    VOID        *Buffer;
    UINTN       MemSize;

    if (NumBufs == 0) {
        return EFI_SUCCESS;
    }

    if (Usb3Hc->ScratchBufEntry == NULL) {
        // Allcate a PAGESIZE boundary for Scratchpad Buffer Array Base Address
        // because bit[0..5] are reserved in  Device Context Base Address Array Element 0.
        MemSize = TRANSFER_RING_OFFSET + Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED * TRB_RING_PADDED_SIZE +
                  RING_SIZE * Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED +
                  (XHCI_DEVICE_CONTEXT_ENTRIES * Usb3Hc->ContextSize) * Usb3Hc->CapRegs.HcsParams1.MaxSlots;
        if (((UINTN)Usb3Hc->DcbaaPtr + MemSize) & (UINTN)EFI_PAGE_MASK) {
            Usb3Hc->ScratchBufEntry = (VOID*)(((UINTN)Usb3Hc->DcbaaPtr + MemSize + EFI_PAGE_SIZE) & (~(UINTN)EFI_PAGE_MASK));
        } else {
            Usb3Hc->ScratchBufEntry = (VOID*)((UINTN)Usb3Hc->DcbaaPtr + MemSize);
        }
        
        if (Usb3Hc->ScratchBufEntry == NULL) {
            return EFI_OUT_OF_RESOURCES;
        }

        SetMem (Usb3Hc->ScratchBufEntry, (sizeof(*(Usb3Hc->ScratchBufEntry)) * NumBufs), 0);

        if (((UINTN)Usb3Hc->ScratchBufEntry + (sizeof(*(Usb3Hc->ScratchBufEntry)) * NumBufs)) & (UINTN)EFI_PAGE_MASK) {
            Buffer = (UINTN*)(((UINTN)Usb3Hc->ScratchBufEntry + (sizeof(*(Usb3Hc->ScratchBufEntry)) * NumBufs) + EFI_PAGE_SIZE) & (~(UINTN)EFI_PAGE_MASK));
        }else{
            Buffer = (UINTN*)((UINTN)Usb3Hc->ScratchBufEntry + (sizeof(*(Usb3Hc->ScratchBufEntry)) * NumBufs));
        }

        for (Count = 0; Count < NumBufs; Count++) {

            // Allocate scratchpad buffer: PAGESIZE block located on
            // a PAGESIZE boundary. Section 4.20.
            if (Buffer == NULL) {
                return EFI_OUT_OF_RESOURCES;
            }

            // Update *ScratchBufArrayBase
            Usb3Hc->ScratchBufEntry[Count] = (UINTN)Buffer;
            Buffer = (VOID*)((UINTN)Buffer + EFI_PAGES_TO_SIZE(Usb3Hc->PageSize4K));
        }
    }

    // Update scratchpad pointer only if # of scratch buffers >0
    if (NumBufs > 0) {
        *(UINTN*)Usb3Hc->DcbaaPtr = (UINTN)Usb3Hc->ScratchBufEntry;
    }

    return EFI_SUCCESS;
}

/**
    Parse Minor Revision of Xhci Supported Protocol Capability structures.
    @param  MinorRev   Minor Specification Release Number in Binary-Coded Decimal

    @retval BOOLEAN    TRUE if supported.

**/

BOOLEAN
EFIAPI 
IsUsb3xSupportProtocol (
    IN UINT32    MinorRev
)
{
    //
    // The bcdUSB field contains a BCD version number. The value of the bcdUSB field is 0xJJMN
    // for version JJ.M.N (JJ = major version number, M = minor version number, N = sub-minor
    // version number), e.g., version 3.10 is represented with a value of 0310H
    //
    switch (MinorRev) {
        case XHCI_EXT_PROTOCOL_MINOR_REV_01:
        case XHCI_EXT_PROTOCOL_MINOR_REV_10:
        case XHCI_EXT_PROTOCOL_MINOR_REV_20:
            return TRUE;
        break;
        default:
            return FALSE;
        break;
    }
}

/**
    Parse all Xhci capability structures.
    @param  BaseAddress   Xhci controller Bar0 address
    @param  PciIo         Pointer to the PciIo structure
    @param  Usb3Hc        Pointer to USB3 host controller

    @retval EFI_STATUS Status of the operation

**/
EFI_STATUS
XhciExtCapbilityParser (
    UINTN                    BaseAddress,
    EFI_PCI_IO_PROTOCOL      *PciIo,
    IN USB3_HOST_CONTROLLER  *Usb3Hc
)
{
    UINT32                  CurPtrOffset;
    UINT32                  XhciLegCtrlSts;
    UINT32                  XhciLegSup;
    XHCI_EXT_CAP            *XhciExtCap = (XHCI_EXT_CAP*)&XhciLegSup;
    EFI_STATUS              Status;
    UINT8                   Usb3xProtocolCount = 0;
    XHCI_EXT_PROTOCOL       *Usb3xProtocol;

    if (Usb3Hc->CapRegs.HccParams1.Xecp == 0) {
        return EFI_SUCCESS;
    }

    // Starts from first capability
    CurPtrOffset = Usb3Hc->CapRegs.HccParams1.Xecp << 2;

    // Traverse all capability structures
    for (;;) {
        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 1, XhciExtCap);
        if (EFI_ERROR (Status)) {
           USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Extended Capability error: %r at %x\n", Status, CurPtrOffset);
           return Status;
        }
        switch (XhciExtCap->CapId) {
            case XHCI_EXT_CAP_USB_LEGACY:
                
                Usb3Hc->UsbLegSupOffSet = CurPtrOffset;
                
                // Clear HC BIOS and OS Owned Semaphore bit
                XhciLegSup &= ~(XHCI_BIOS_OWNED_SEMAPHORE | XHCI_OS_OWNED_SEMAPHORE);
                Status = PciIo->Mem.Write (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 1, &XhciLegSup);
                if (EFI_ERROR (Status)) {
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Clear HC BIOS and OS Owned Semaphore bit error: %r at %x\n",
                              Status, CurPtrOffset);
                    return Status;
                }
                
                // Clear SMI enable bit
                Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, 
                    CurPtrOffset + XHCI_LEGACY_CTRL_STS_REG, 1, &XhciLegCtrlSts);
                if (EFI_ERROR (Status)) {
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Legacy Support Control/Status error: %r at %x\n", Status, 
                            CurPtrOffset + XHCI_LEGACY_CTRL_STS_REG);
                    return Status;
                }

                XhciLegCtrlSts &= ~(XHCI_SMI_ENABLE | XHCI_SMI_OWNERSHIP_CHANGE_ENABLE);
                Status = PciIo->Mem.Write (PciIo, EfiPciIoWidthUint32, 0, 
                    CurPtrOffset + XHCI_LEGACY_CTRL_STS_REG, 1, &XhciLegCtrlSts);
                if (EFI_ERROR (Status)) {
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Clear SMI enable bit error: %r at %x\n", Status, 
                            CurPtrOffset + XHCI_LEGACY_CTRL_STS_REG);
                    return Status;
                }
                USB_DEBUG(DEBUG_INFO, 3, "XHCI: USB Legacy Support Offset %x\n", Usb3Hc->UsbLegSupOffSet);
            break;

            case XHCI_EXT_CAP_SUPPORTED_PROTOCOL:
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XHCI: CAP supported : MajorRev %x MinorRev %x\n", ((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MajorRev, ((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MinorRev);
                if (((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MajorRev == XHCI_EXT_PROTOCOL_MAJOR_REV_02) {
                    Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 3, &Usb3Hc->Usb2Protocol);
                    if (EFI_ERROR (Status)) {
                        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: USB2 Support Protocol error: %r at %x\n", Status, CurPtrOffset);
                        return Status;
                    }
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XHCI: USB2 Support Protocol %x, PortOffset %x PortCount %x\n", 
                        (UINTN*)(BaseAddress + CurPtrOffset), Usb3Hc->Usb2Protocol.Port.PortOffset, Usb3Hc->Usb2Protocol.Port.PortCount);
                } else if (((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MajorRev == XHCI_EXT_PROTOCOL_MAJOR_REV_03) {
                    if (((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MinorRev == XHCI_EXT_PROTOCOL_MINOR_REV_00) {
                        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 3, &Usb3Hc->Usb3Protocol);
                        if (EFI_ERROR (Status)) {
                            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XHCI: USB3.0 Support Protocol error: %r at %x\n", Status, CurPtrOffset);
                            return Status;
                        }
                        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XHCI: USB3.0 Support Protocol %x, PortOffset %x PortCount %x\n", 
                          (UINTN*)(BaseAddress + CurPtrOffset), Usb3Hc->Usb3Protocol.Port.PortOffset, Usb3Hc->Usb3Protocol.Port.PortCount);
                    } else if (IsUsb3xSupportProtocol(((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MinorRev)) {
                        if ((Usb3Hc->Usb3xProtocolCount) && (Usb3xProtocolCount < Usb3Hc->Usb3xProtocolCount)) {
                            Usb3xProtocol = (XHCI_EXT_PROTOCOL *)((UINTN)Usb3Hc->Usb3xProtocol + Usb3xProtocolCount * (sizeof(XHCI_EXT_PROTOCOL)));
                            Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 3, Usb3xProtocol);
                            if (EFI_ERROR (Status)) {
                                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: USB3.x Support Protocol error: %r at %x\n", Status, CurPtrOffset);
                                return Status;
                            }
                            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XHCI: USB3.x Support Protocol %x, PortOffset %x PortCount %x\n",
                              (UINTN*)(BaseAddress + CurPtrOffset), Usb3xProtocol->Port.PortOffset, Usb3xProtocol->Port.PortCount);
                            Usb3xProtocolCount++;
                        }
                    }
                }
                break;

            case XHCI_EXT_CAP_POWERMANAGEMENT:
            case XHCI_EXT_CAP_IO_VIRTUALIZATION:
                break;
            case XHCI_EXT_CAP_USB_DEBUG_PORT:
                Usb3Hc->DbCapOffset = CurPtrOffset;
                USB_DEBUG(DEBUG_INFO, 3, "XHCI: USB Debug Capability Offset %x\n", Usb3Hc->DbCapOffset);
                break;
        }
        if (XhciExtCap->NextCapPtr == 0) {
            break;
        }
        // Point to next capability
        CurPtrOffset += XhciExtCap->NextCapPtr << 2;
    }

    return EFI_SUCCESS;
}

/**
    This function allocates XHCI memory buffers.
    @param  PciIo   Pointer to the PciIo structure
    @param  Usb3Hc  Pointer to USB3 host controller

    @retval EFI_STATUS Status of the operation

    @note  Scratchpad buffers are optional, they are allocated and initialized separately.
    1. DCBAAP + ScrPadBuf pointers + InpCtx + ERST <-- 8KB
    2. CommandRing  <-- 1KB
    3. EventRing    <-- 1KB
    4. XferRings    <-- 1KB*MaxSlots, 1KB = 32EP per slot times 32 (padded size of TRB_RING)
    4. N*sizeof(XHCI_DEVICE_CONTEXT) for device context segment <-- N KB or 2*N KB,
       N is SlotNumber from CONFIG register
    5. TransferRings <-- MaxSlots*32*1KB

**/
EFI_STATUS
XhciInitHcMemory (
    IN EFI_PCI_IO_PROTOCOL     *PciIo,
    IN USB3_HOST_CONTROLLER    *Usb3Hc
)
{
    UINTN       MemSize;
    UINTN       DeviceContextSize;
    UINTN       XfrRingsSize;
    UINTN       XfrTrbsSize;
    UINTN       TotalPages;
    UINT32      NumBufs;

    // 32 endpoints per device, 32 padded size of TRB_RING
    XfrRingsSize = Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED * TRB_RING_PADDED_SIZE;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XfrRingsSize %x\n", XfrRingsSize);
    XfrTrbsSize = RING_SIZE * Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XfrTrbsSize %x\n", XfrTrbsSize);
    DeviceContextSize = (XHCI_DEVICE_CONTEXT_ENTRIES * Usb3Hc->ContextSize) * Usb3Hc->CapRegs.HcsParams1.MaxSlots;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "DeviceContextSize %x\n", DeviceContextSize);
    NumBufs =  ((Usb3Hc->CapRegs.HcsParams2.MaxScratchPadBufsHi) << 5) + 
                                    Usb3Hc->CapRegs.HcsParams2.MaxScratchPadBufsLo;
//    MemSize = 0x2800 + XfrRingsSize + XfrTrbsSize + DeviceContextSize;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "NumBufs %x\n", NumBufs);
    MemSize = TRANSFER_RING_OFFSET + XfrRingsSize + XfrTrbsSize + DeviceContextSize;
    TotalPages = EFI_SIZE_TO_PAGES(MemSize);
    TotalPages += EFI_SIZE_TO_PAGES(sizeof(*(Usb3Hc->ScratchBufEntry)) * NumBufs);
    TotalPages += (Usb3Hc->PageSize4K * NumBufs);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "TotalPages %x\n", TotalPages);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb3Hc->DcbaaPtr %x\n", Usb3Hc->DcbaaPtr);
    if ((gUsbData->UsbRuntimeDriverInSmm == USB_RUNTIME_DRV_MODE_0) ||
        ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
        ((gUsbData->UsbStateFlag & USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)==0))) {
        if (Usb3Hc->DcbaaPtr == NULL) {
        // InputContext : DcbaaPtr + 0x940
        // ERST         : DcbaaPtr + 0x1200
        // CmdRing      : DcbaaPtr + 0x2000
        // EvtRing      : DcbaaPtr + 0x2400
        // XfrRings     : DcbaaPtr + 0x6400
        // XfrTrbs      : XfrRings + XfrRingsSize
        // DeviceContext: XfrTrbs  + XfrTrbsSize
        // ScratchBuffer: After DeviceContext, start from a new page.
            Usb3Hc->DcbaaPtr = (XHCI_DCBAA*)AllocateHcMemory (PciIo, TotalPages, USB_PAGE_SIZE_ALIGNMENT);
            if (Usb3Hc->DcbaaPtr == NULL) {
                return EFI_OUT_OF_RESOURCES;
            }
            SetMem(Usb3Hc->DcbaaPtr, EFI_PAGES_TO_SIZE(TotalPages), 0);  
        } else {
            SetMem(Usb3Hc->DcbaaPtr, MemSize, 0);
        }
    }

    Usb3Hc->DcbaaPages = TotalPages;

    USB_DEBUG(DEBUG_INFO, 3, "XHCI: Memory allocation - total %x Bytes:\n 0x6400+XfrRings(%x)+XfrTrbs(%x)+DevCtx(%x)\n",
        MemSize, XfrRingsSize, XfrTrbsSize, DeviceContextSize);


    return EFI_SUCCESS;
}


/**
    Collect protocol capability pointer of USB3.1 and above version.
    @param  PciIo   Pointer to the PciIo structure
    @param  Usb3Hc  Pointer to USB3 host controller
    @param  ControllerHandleIndex Index number of USB3 host controller handle

    @retval EFI_STATUS Status of the operation

**/
EFI_STATUS
XhciExtCapUsb3xCnt (
    EFI_PCI_IO_PROTOCOL     *PciIo,
    IN USB3_HOST_CONTROLLER *Usb3Hc,
    UINTN                   ControllerHandleIndex
)
{
    UINT32                  CurPtrOffset;
    UINT32                  XhciLegSup;
    XHCI_EXT_CAP            *XhciExtCap = (XHCI_EXT_CAP*)&XhciLegSup;
    UINT8                   Usb3xProtocolCount;
    UINTN                   XhciHcSize = 0;

    Usb3xProtocolCount = 0;
    if (Usb3Hc->CapRegs.HccParams1.Xecp == 0) {
        return EFI_SUCCESS;
    }

    // Starts from first capability
    CurPtrOffset = Usb3Hc->CapRegs.HccParams1.Xecp << 2;

    // Traverse all capability structures
    for (;;) {
        PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, CurPtrOffset, 1, XhciExtCap);
        switch (XhciExtCap->CapId) {
            case XHCI_EXT_CAP_SUPPORTED_PROTOCOL:
                if (((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MajorRev == XHCI_EXT_PROTOCOL_MAJOR_REV_03) {
                    if (IsUsb3xSupportProtocol(((XHCI_EXT_PROTOCOL*)XhciExtCap)->Field.MinorRev)) {
                        Usb3xProtocolCount++;
                    }
                }
                break;
        }
        if (XhciExtCap->NextCapPtr == 0) {
            break;
        }
        // Point to next capability
        CurPtrOffset += XhciExtCap->NextCapPtr << 2;
    }

    if (Usb3xProtocolCount > 0) {
        XhciHcSize = (sizeof(USB3_HOST_CONTROLLER*) + sizeof(USB3_HOST_CONTROLLER)) * gUsbDataList->Usb3HcCount;
        XhciHcSize += (MAX_USB3X_PROTOCOL_DATA_SIZE * ControllerHandleIndex);
        Usb3Hc->Usb3xProtocolCount = Usb3xProtocolCount;
        Usb3Hc->Usb3xProtocol = (VOID*)((UINTN)gUsbDataList->Usb3HcMem + XhciHcSize);
        USB_DEBUG(DEBUG_INFO, 3, "Usb3Hc[%d] Usb3xProtocol %x \n", ControllerHandleIndex, Usb3Hc->Usb3xProtocol);
    }
    return EFI_SUCCESS;
}

/**
    Collect USB EHCI controllers and allocate ISOCTRANSFER buffers.
    @param  None
    @retval None

**/
VOID
CollectUsbEhciControllers (
    VOID
    )
{
    EFI_STATUS                Status;
    UINTN                     NumPages;
    VOID                      **IsocTdsDataPtr = NULL;
    VOID                      *IsocTdsAddr = NULL;
    EHCI_ITD_DATA             *UsbHc = NULL;
    EFI_PCI_IO_PROTOCOL       *PciIo;
    EFI_HANDLE                ControllerHandle;
    UINTN                     ControllerHandleIndex;
    UINTN                     TotalPages;

    if (gUsbDataList == NULL) return;
    if (gUsbDataList->IsocTdsData == NULL) return;
    if (!(((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT) &&
        ((gUsbData->UsbStateFlag & USB_FLAG_USB_ISOCTRANSFER_SUPPORT) == USB_FLAG_USB_ISOCTRANSFER_SUPPORT))) return;


    if ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
        ((gUsbData->UsbStateFlag & USB_FLAG_ITD_MEM_ALLOC_FOR_ALL_EHCI)==USB_FLAG_ITD_MEM_ALLOC_FOR_ALL_EHCI)) {
        ControllerHandle = gUsbEhciHcHandle[0];
        Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR (Status)) return;

        NumPages = EFI_SIZE_TO_PAGES(EHCI_FRAMELISTSIZE * (sizeof(EHCI_ITD)));
        TotalPages = NumPages + EFI_SIZE_TO_PAGES(USB_EHCI_FRAME_LIST_SIZE);

        IsocTdsAddr = (EHCI_ITD*)AllocateHcMemory (PciIo, (TotalPages * gUsbDataList->IsocTdsDataCount), USB_PAGE_SIZE_ALIGNMENT);
        if (IsocTdsAddr == NULL) return;
    }

    IsocTdsDataPtr = (VOID**)gUsbDataList->IsocTdsData;
    for (ControllerHandleIndex = 0; ControllerHandleIndex < gUsbDataList->IsocTdsDataCount; ControllerHandleIndex++) {
        ControllerHandle = gUsbEhciHcHandle[ControllerHandleIndex];
        Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR (Status)) continue;
        UsbHc = (EHCI_ITD_DATA*)((UINTN)IsocTdsDataPtr + ((sizeof(VOID*)) * gUsbDataList->IsocTdsDataCount) + ((sizeof(EHCI_ITD_DATA)) * ControllerHandleIndex));
        USB_DEBUG(DEBUG_INFO, 3, "UsbHc[%d] IsocTdsData %x \n", ControllerHandleIndex, UsbHc);

        Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &UsbHc->Vid);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Ehci: Vendor ID error: %r at %x\n", Status, PCI_VENDOR_ID_OFFSET);
            continue;
        }

        Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &UsbHc->Did);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Ehci: Device ID error: %r at %x\n", Status, PCI_DEVICE_ID_OFFSET);
            continue;
        }
        UsbHc->Controller = ControllerHandle;

        if ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
            ((gUsbData->UsbStateFlag & USB_FLAG_ITD_MEM_ALLOC_FOR_ALL_EHCI)==USB_FLAG_ITD_MEM_ALLOC_FOR_ALL_EHCI)){
            UsbHc->IsocTds = (EHCI_ITD*)((UINTN)IsocTdsAddr + (ControllerHandleIndex * (EFI_PAGES_TO_SIZE(TotalPages))) + USB_EHCI_FRAME_LIST_SIZE);
        } else {
            NumPages = EFI_SIZE_TO_PAGES(EHCI_FRAMELISTSIZE * (sizeof(EHCI_ITD)));
            UsbHc->IsocTds = (EHCI_ITD*)AllocateHcMemory (PciIo, NumPages, USB_PAGE_SIZE_ALIGNMENT);
        }
        if (UsbHc->IsocTds == NULL) continue;
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "UsbHc->IsocTds[%d] %x\n", ControllerHandleIndex, UsbHc->IsocTds);

        SetMem ((VOID*)UsbHc->IsocTds, (EHCI_FRAMELISTSIZE * (sizeof(EHCI_ITD))), 0);

        IsocTdsDataPtr[ControllerHandleIndex] = (VOID*)UsbHc;
    }
}

/**
    This function allocates XHCI memory buffers.
    @param  None
    @retval None

    @note   Scratchpad buffers are optional, they are allocated and initialized separately.

**/
VOID
CollectUsbXhciControllers (
    VOID
    )
{

    EFI_STATUS                Status;
    EFI_PCI_IO_PROTOCOL       *PciIo;
    UINTN                     BaseAddress = 0;
    EFI_HANDLE                ControllerHandle;
    UINTN                     ControllerHandleIndex;
    USB3_HOST_CONTROLLER      **Usb3HcMem = NULL;
    USB3_HOST_CONTROLLER      *Usb3Hc = NULL;
    UINT64                    Capabilities;
    UINTN                     TotalPages = 0;
    XHCI_DCBAA                *DcbaaPtr;
    UINTN                     DcbaaPages = 0;

    UINTN                     XfrRingsSize;
    UINTN                     XfrTrbsSize;

    if (gUsbDataList == NULL) return;
    if (gUsbDataList->Usb3HcMem == NULL) return;
    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == 0) return;

    Usb3HcMem = (USB3_HOST_CONTROLLER**)gUsbDataList->Usb3HcMem;
    for (ControllerHandleIndex = 0; ControllerHandleIndex < gUsbDataList->Usb3HcCount; ControllerHandleIndex++) {
        ControllerHandle = gUsbXhciHcHandle[ControllerHandleIndex];
        Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR (Status)) continue;
        Usb3Hc = (USB3_HOST_CONTROLLER*)((UINTN)Usb3HcMem + ((sizeof(USB3_HOST_CONTROLLER*)) * gUsbDataList->Usb3HcCount) + ((sizeof(USB3_HOST_CONTROLLER)) * ControllerHandleIndex));
        USB_DEBUG(DEBUG_INFO, 3, "Usb3Hc[%d] %x \n", ControllerHandleIndex, Usb3Hc);

        Status = PciIo->Attributes (
                            PciIo,
                            EfiPciIoAttributeOperationSupported,
                            0,
                            &Capabilities
                            );
        if (EFI_ERROR (Status)) {
            continue;
        }

        Status = PciIo->Attributes (
                          PciIo,
                          EfiPciIoAttributeOperationEnable,
                          (Capabilities & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE)),
                          NULL
                          );
        if (EFI_ERROR (Status)) {
            continue;
        }

        // Get Capability Registers offset off the BAR
        Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, PCI_BASE_ADDRESSREG_OFFSET, 1, &BaseAddress);
        if (EFI_ERROR (Status)) {
            continue;
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Read the 32 bits BAR0: %x\n", (UINT32)BaseAddress);
        if (((UINT8)BaseAddress & (BIT1 | BIT2)) == BIT2) {
          PciIo->Pci.Read (
                       PciIo, 
                       EfiPciIoWidthUint32,
                       PCI_BASE_ADDRESSREG_OFFSET,
                       sizeof(VOID*)/sizeof(UINT32),
                       &BaseAddress
                       );
           USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Support 64 bits address, BaseAddress is: %lx\n", BaseAddress);
        }


        //clear all attributes before use
        BaseAddress &= ~(0x7F);

        Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &Usb3Hc->Vid);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Vendor ID error: %r at %x\n", Status, PCI_VENDOR_ID_OFFSET);
            continue;
        }

        Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &Usb3Hc->Did);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Device ID error: %r at %x\n", Status, PCI_DEVICE_ID_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint8, 0, XHCI_CAPLENGTH_OFFSET, 1, &Usb3Hc->CapRegs.CapLength);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Capability Register length error: %r at %x\n", Status, XHCI_CAPLENGTH_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint16, 0, XHCI_HCIVERSION_OFFSET, 1, &Usb3Hc->CapRegs.HciVersion);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Interface Version Number error: %r at %x\n", Status, XHCI_HCIVERSION_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_HCSPARAMS1_OFFSET, 1, &Usb3Hc->CapRegs.HcsParams1);
        if (EFI_ERROR(Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Structural Parameters 1 error: %r at %x\n", Status, XHCI_HCSPARAMS1_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_HCSPARAMS2_OFFSET, 1, &Usb3Hc->CapRegs.HcsParams2);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Structural Parameters 2 error: %r at %x\n", Status, XHCI_HCSPARAMS2_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_HCSPARAMS3_OFFSET, 1, &Usb3Hc->CapRegs.HcsParams3);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Structural Parameters 3 error: %r at %x\n", Status, XHCI_HCSPARAMS3_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_HCCPARAMS1_OFFSET, 1, &Usb3Hc->CapRegs.HccParams1);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Capability Parameters 1 error: %r at %x\n", Status, XHCI_HCCPARAMS1_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_DBOFF_OFFSET, 1, &Usb3Hc->CapRegs.DbOff);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Doorbell Offset error: %r at %x\n", Status, XHCI_DBOFF_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read (PciIo, EfiPciIoWidthUint32, 0, XHCI_RTSOFF_OFFSET, 1, &Usb3Hc->CapRegs.RtsOff);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Runtime Register Space Offset error: %r at %x\n", Status, XHCI_RTSOFF_OFFSET);
            continue;
        }

        Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, XHCI_HCCPARAMS2_OFFSET, 1, &Usb3Hc->CapRegs.HccParams2);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Capability Parameters 2 error: %r at %x\n", Status, XHCI_HCCPARAMS2_OFFSET);
            continue;
        }
 
        Usb3Hc->OpRegs = (XHCI_HC_OP_REGS*)(BaseAddress + Usb3Hc->CapRegs.CapLength);

        Status = PciIo->Mem.Read(PciIo, EfiPciIoWidthUint32, 0, Usb3Hc->CapRegs.CapLength + XHCI_PAGESIZE_OFFSET, 1, &Usb3Hc->PageSize4K);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Page Size Register error: %r at %x\n", 
                  Status, Usb3Hc->CapRegs.CapLength + XHCI_PAGESIZE_OFFSET);
            continue;
        }

        Usb3Hc->ContextSize = 0x20 << Usb3Hc->CapRegs.HccParams1.Csz;

        PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, XHCI_PCI_SBRN, 1, &Usb3Hc->SBRN);
        USB_DEBUG(DEBUG_INFO, 3, "XHCI: Serial Bus Release Number is %x\n", Usb3Hc->SBRN);




        USB_DEBUG(DEBUG_INFO, 3, "XHCI: MaxSlots %x, MaxIntrs %x, Doorbell Offset %x\n", 
            Usb3Hc->CapRegs.HcsParams1.MaxSlots, 
            Usb3Hc->CapRegs.HcsParams1.MaxIntrs,
            Usb3Hc->CapRegs.DbOff);

        Status = XhciExtCapUsb3xCnt (PciIo, Usb3Hc, ControllerHandleIndex);
        if (EFI_ERROR (Status)) {
            continue;
        }

        // Parse all capability structures
        XhciExtCapbilityParser (BaseAddress, PciIo, Usb3Hc);

        // Allocate and initialize memory blocks
        Status = XhciInitHcMemory (PciIo, Usb3Hc);
        if (EFI_ERROR (Status)) {
            continue;
        }
        Usb3Hc->Controller = ControllerHandle;
        Usb3HcMem[ControllerHandleIndex] = Usb3Hc;
        if ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
            ((gUsbData->UsbStateFlag & USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)==USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)) {
            TotalPages += Usb3Hc->DcbaaPages;
        }
    }

    if ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
        ((gUsbData->UsbStateFlag & USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)==USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)) {
        if (TotalPages > 0) {
            ControllerHandle = gUsbXhciHcHandle[0];
            Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
            if (EFI_ERROR (Status)) return;
            DcbaaPtr = (XHCI_DCBAA*)AllocateHcMemory (PciIo, TotalPages, USB_PAGE_SIZE_ALIGNMENT);
            if (DcbaaPtr == NULL) return;
            SetMem(DcbaaPtr, EFI_PAGES_TO_SIZE(TotalPages), 0);  
        }
    }
    for (ControllerHandleIndex = 0; ControllerHandleIndex < gUsbDataList->Usb3HcCount; ControllerHandleIndex++) {
        ControllerHandle = gUsbXhciHcHandle[ControllerHandleIndex];
        Usb3Hc = Usb3HcMem[ControllerHandleIndex];
        if ((gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) && 
            ((gUsbData->UsbStateFlag & USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)==USB_FLAG_DCBAA_MEM_ALLOC_FOR_ALL_XHCI)) {
            if (Usb3Hc->DcbaaPages > 0) {
                Usb3Hc->DcbaaPtr = (XHCI_DCBAA*)((UINTN)DcbaaPtr + (EFI_PAGES_TO_SIZE(DcbaaPages)));
                USB_DEBUG(DEBUG_INFO, 3, "Usb3Hc->DcbaaPtr %x\n", Usb3Hc->DcbaaPtr);
                DcbaaPages += Usb3Hc->DcbaaPages;
            }
        }

        if (Usb3Hc->DcbaaPtr == NULL || Usb3Hc->DcbaaPages == 0) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: Usb3Hc->DcbaaPtr %x Usb3Hc->DcbaaPages %x\n", Usb3Hc->DcbaaPtr, Usb3Hc->DcbaaPages);
            continue;
        }

        Status = XhciAllocateScratchpadBuffer (Usb3Hc);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: AllocateScratchpadBuffers: %r\n", Status);
            continue;
        }

        XfrRingsSize = Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED * TRB_RING_PADDED_SIZE;
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XfrRingsSize %x\n", XfrRingsSize);
        XfrTrbsSize = RING_SIZE * Usb3Hc->CapRegs.HcsParams1.MaxSlots * END_POINTS_PADDED;
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "XfrTrbsSize %x\n", XfrTrbsSize);

        // Assign Input Context; the size of Input Context is either
        // 0x420 or 0x840 depending on HCPARAMS.Csz
        if (Usb3Hc->InputContext == NULL) {
            Usb3Hc->InputContext = (VOID*)((UINTN)Usb3Hc->DcbaaPtr + INPUT_CONTEXT_OFFSET);
        }

        // Initialize Transfer Rings pointer and store it in Usb3Hc; actual
        // xfer ring initialization happens later, when the EP is being enabled
        //    Usb3Hc->XfrRings = (TRB_RING*)((UINTN)Usb3Hc->DcbaaPtr + 0x2800);
        if (Usb3Hc->XfrRings == NULL) {
            Usb3Hc->XfrRings = (TRB_RING*)((UINTN)Usb3Hc->DcbaaPtr + TRANSFER_RING_OFFSET);
        }

        // 1024 = 32 bytes is padded sizeof(TRB_RING) times 32 EP per device
        if (Usb3Hc->XfrTrbs == 0) {
            Usb3Hc->XfrTrbs = (UINTN)Usb3Hc->XfrRings + XfrRingsSize;
        }

        // Assign device context memory: Usb3Hc->MaxSlots devices,
        // 1024 (2048 if HCPARAMS.Csz is set) Bytes each
        if (Usb3Hc->DeviceContext == NULL) {
            Usb3Hc->DeviceContext = (VOID*)((UINTN)Usb3Hc->XfrTrbs + XfrTrbsSize);
        }

        Status = UsbHcMemoryRecord (ControllerHandle,(EFI_PHYSICAL_ADDRESS)(UINTN)Usb3Hc->DcbaaPtr, Usb3Hc->DcbaaPages);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Xhci: UsbHcMemoryRecord: %r\n", Status);
        }
    }

}

/**
    This function allocates XHCI memory buffers.
    @param  None
    @retval None

    @note   Scratchpad buffers are optional, they are allocated and initialized separately.

**/
VOID
CollectUsbControllersEx (
    )
{
    EFI_STATUS                Status;
    EFI_PCI_IO_PROTOCOL       *PciIo;
    UINTN                     ControllerHandleCount = 0;
    EFI_HANDLE                *ControllerHandleBuffer = NULL;
    EFI_HANDLE                ControllerHandle;
    UINTN                     ControllerHandleIndex;
    UINT32                    UsbHcClassCode;
    UINT32                    XhciHcCount = 0;
    UINT32                    EhciHcCount = 0;
    EFI_USB_PROTOCOL          *AmiUsb = NULL;
    EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
    UINT32                    Size = 0;
    UINT32                    XhciHcSize = 0;
    VOID                      *Addr = NULL;

    Status = gBS->LocateProtocol (&gEfiUsbProtocolGuid, NULL, (VOID**)&AmiUsb);
    if (EFI_ERROR (Status)) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "gEfiUsbProtocolGuid %r\n", Status);
        return;
    }

    gUsbData = AmiUsb->USBDataPtr;
    if (gUsbData == NULL) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_GLOBAL_DATA not ready\n");
        return;
    }

    gUsbDataList = gUsbData->UsbDataList;
    if (gUsbDataList == NULL) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_DATA_LIST not ready\n");
        return;
    }

    Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiPciIoProtocolGuid,
                      NULL,
                      &ControllerHandleCount,
                      &ControllerHandleBuffer
                      );
    if (EFI_ERROR (Status)) return;

    for (ControllerHandleIndex = 0; ControllerHandleIndex < ControllerHandleCount; ControllerHandleIndex++) {
        ControllerHandle = ControllerHandleBuffer[ControllerHandleIndex];
        Status = gBS->HandleProtocol (ControllerHandle, &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR (Status)) continue;

        Status = PciIo->Pci.Read (
                            PciIo, 
                            EfiPciIoWidthUint32,
                            PCI_REVISION_ID_OFFSET,
                            1,
                            &UsbHcClassCode
                            );
        if (EFI_ERROR (Status)) continue;
        UsbHcClassCode = UsbHcClassCode >> 8;
        if (UsbHcClassCode != XHCI_CLASS_CODE && UsbHcClassCode != EHCI_CLASS_CODE) {
            continue;
        }
        Status = gBS->HandleProtocol (ControllerHandle, &gEfiDevicePathProtocolGuid, (VOID**)&DevicePath);
        if (EFI_ERROR (Status)) continue;
        
        if ((gUsbData->UsbFeature & USB_EXTERNAL_CONTROLLER_SUPPORT) == 0)
            if (IsExternalController (DevicePath)) {
                continue;
            }

        if (UsbHcClassCode == XHCI_CLASS_CODE) {
            gUsbXhciHcHandle[XhciHcCount] = ControllerHandle;
            XhciHcCount++;
        }
        if (UsbHcClassCode == EHCI_CLASS_CODE) {
            if (((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT) &&
                ((gUsbData->UsbStateFlag & USB_FLAG_USB_ISOCTRANSFER_SUPPORT) == USB_FLAG_USB_ISOCTRANSFER_SUPPORT)){
                gUsbEhciHcHandle[EhciHcCount] = ControllerHandle;
                EhciHcCount++;
            }
        }
    }
    if (XhciHcCount == 0 && EhciHcCount == 0) return;
    gUsbDataList->Usb3HcCount = XhciHcCount;
    gUsbDataList->IsocTdsDataCount = EhciHcCount;

    XhciHcSize = (sizeof(USB3_HOST_CONTROLLER*) + sizeof(USB3_HOST_CONTROLLER) + MAX_USB3X_PROTOCOL_DATA_SIZE) * XhciHcCount;
    Size = XhciHcSize + (sizeof(VOID*) + sizeof(EHCI_ITD_DATA)) * EhciHcCount;
    Status = gBS->AllocatePool (
                      EfiRuntimeServicesData,
                      Size,
                      (VOID**)&Addr
                      );
    if (EFI_ERROR (Status)) return;

    SetMem (Addr, Size, 0);
    if (XhciHcCount != 0) {
        gUsbDataList->Usb3HcMem = Addr;
        USB_DEBUG(DEBUG_INFO, 3, "gUsbDataList->Usb3HcMem %x XhciHcSize %x \n", gUsbDataList->Usb3HcMem, XhciHcSize);
    }
    if (EhciHcCount != 0) {
        gUsbDataList->IsocTdsData = (VOID*)((UINTN)Addr + XhciHcSize);
        USB_DEBUG(DEBUG_INFO, 3, "gUsbDataList->IsocTdsData %x Size %x \n", gUsbDataList->IsocTdsData, (Size - XhciHcSize));
    }

}

/**
    This function collects USB controllers.
    @param  None
    @retval None

**/
VOID
EFIAPI
CollectUsbControllers (
    VOID
    )
{
    SetMem (gUsbXhciHcHandle, (sizeof(EFI_HANDLE) * MAX_USB_HC), 0);
    SetMem (gUsbEhciHcHandle, (sizeof(EFI_HANDLE) * MAX_USB_HC), 0);
    CollectUsbControllersEx ();
    CollectUsbXhciControllers ();
    CollectUsbEhciControllers ();
}

/**
    Trigger SMI.
    @param  None
    @retval EFI_STATUS   Status of the operation
    @retval EFI_SUCCESS  Success to trigger SMI.

**/
EFI_STATUS
TriggerSmi (
  IN UINT8   Data
)
{

  EFI_STATUS Status;
  UINT8 SwSmiValue = Data;
  UINT8 DataSize = 1;
    
  if(gSmmCtl == NULL) {
    Status = gBS->LocateProtocol(&gEfiSmmControl2ProtocolGuid, NULL, (VOID**)&gSmmCtl);
      if(EFI_ERROR(Status)) {return Status;}   
  }

  Status = gSmmCtl->Trigger(gSmmCtl, &SwSmiValue, &DataSize, 0, 0);  
  if(EFI_ERROR(Status)) {
      USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB: Fail to trigger SMI.\n");
      return Status;
  }
  
  return EFI_SUCCESS;
}


/**
    Call USB API for register USB SMI.
    @param  None
    @retval None

**/
VOID
RegisterUsbSmi (
    VOID
    )
{


    URP_STRUC                 *mParameters = NULL;
 
 
    if (gUsbDataList == NULL) return;
    if (gUsbDataList->HcTableCount == 0) return;
    if (gUsbDataList->HcTable == NULL) return;
             

    mParameters = gUsbData->UsbDataList->UsbUrp;
      if(gUsbData->RegisterUsbSmiBeforeEndOfDxe){
        gBS->SetMem(mParameters, sizeof(URP_STRUC), 0);
        mParameters->ApiData.HcStartStop.HcStruc = NULL;
        mParameters->ApiData.HcStartStop.Start = REGISTER_USB_SMI;
        mParameters->bFuncNumber = USB_API_HC_START_STOP;
        TriggerSmi(gUsbData->UsbSwSmi); 
      }
 
}

/**
    Return a buffer of handle that contain PciIo protocols, and whose class/subclass
    match the passed class/subclasses.

    @param Class The class of PCI devices to search
    @param SubClass The subclass of PCI devices to search
    @param NumberOfHandles Pointer to the buffer of the number of Handles being returned
    @param HandleBuffer Double pointer used to return a buffer of PCI handles that match

    @retval EFI_NOT_FOUND No Handles were found that match the search criteria
    @retval EFI_SUCCESS The HandlerBuffer and NumberOfHandles being returned are valid

**/
EFI_STATUS
GetPciHandleByClass (
    IN     UINT8       Class,
    IN     UINT8       SubClass,
    IN     UINTN       *NumberOfHandles,
    IN OUT EFI_HANDLE  **HandleBuffer
    )
{
    EFI_STATUS           Status;
    EFI_HANDLE           *Handle;
    UINTN                Number;
    UINTN                Index;
    EFI_PCI_IO_PROTOCOL  *PciIo = NULL;
    UINT8                PciClass[4];

    if (!NumberOfHandles || !HandleBuffer) return EFI_INVALID_PARAMETER;
    //Get a list of all PCI devices
    Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiPciIoProtocolGuid, NULL, &Number, &Handle);
    if (EFI_ERROR (Status)) return Status;
    *NumberOfHandles = 0;
    for (Index=0; Index  <Number; Index++) {
        Status=gBS->HandleProtocol (Handle[Index], &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR (Status)) continue;
        Status=PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, PCI_REVISION_ID_OFFSET, 1, &PciClass);
        if (PciClass[3] == Class && PciClass[2] == SubClass) {
            Handle[(*NumberOfHandles)++] = Handle[Index];
        }
    }
    if (*NumberOfHandles == 0) {
        gBS->FreePool(Handle);
        return EFI_NOT_FOUND;
    }
    *HandleBuffer = Handle;
    return EFI_SUCCESS;
}

/**
    Function that connects USB controllers
    @param  Recursive      FALSE: non-recursively connects USB controllers
    @retval None

**/
VOID
ConnectUsbControllersEx (
    IN BOOLEAN Recursive
    )
{
    EFI_STATUS  Status;
    EFI_HANDLE  *Handle;
    UINTN       Number;
    UINTN       Index;

    //Get a list of all USB Controllers
    Status = GetPciHandleByClass (PCI_CLASS_SERIAL, PCI_CLASS_SERIAL_USB, &Number, &Handle);
    if (EFI_ERROR (Status)) return;
    for (Index = 0 ; Index < Number; Index++) {
        gBS->ConnectController (Handle[Index], NULL, NULL, Recursive);
    }
    gBS->FreePool(Handle);
}

/**
    Function that non-recursively connects USB controllers
    @param  None
    @retval None

**/
VOID
EFIAPI
StartUsbControllersEx (
    VOID
    )
{
    CollectUsbControllers();
    ConnectUsbControllersEx(FALSE);
    
    // Register USB SMI before end of DXE.
    if (gUsbData->UsbRuntimeDriverInSmm != USB_RUNTIME_DRV_MODE_0) { 
      RegisterUsbSmi();
    }

}

/**
    Function that attempts to connect the USB console in devices
    @param  None
    @retval None

**/
VOID
EFIAPI
ConnectUsbConInEx (
    VOID
    )
{
    ConnectUsbControllersEx(TRUE);
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
