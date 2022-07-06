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

/** @file UsbMemMisc.c
    AMI USB driver memory related miscellaneous routines

**/

#include <AmiDef.h>
#include <UsbDef.h>
#include "Uhcd.h"
#include <Protocol/PciIo.h>
#include <Protocol/AmiUsbHcMemRecordProtocol.h>
#include <IndustryStandard/Pci.h>

BOOLEAN  gFirstCall = TRUE;
VOID    *gGlobalPointer;
VOID    *gStartPointer;
VOID    *gEndPointer;


/**
    Returns the aligned address.
    @param  PhyAddress           Address to check the alignment
    @param  AlignSize            Alignment requirements
    @retval AlignAddr            Address of memory with the given alignment.

**/

UINTN
AlignPhysicalAddress(
    UINTN   PhyAddress,
    UINT16  AlignSize
)
{
    UINTN   AlignAddr;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "Un-aligned address : %lX\n", PhyAddress);
    if ((PhyAddress % AlignSize) != 0) {
        AlignAddr = PhyAddress - (PhyAddress % (UINT32)AlignSize) + AlignSize;
    } else {
        AlignAddr = PhyAddress;
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_7, "Aligned address : %lX\n", AlignAddr);

    return AlignAddr;
}

/**
    Allocates memory with the given alignment.
    @param  AllocSize      Number of pages
    @param  Align          Alignment requirements
    @retval Buffer         Address of memory allocated with the given alignment.

**/

VOID*
EFIAPI
AllocAlignedMemory (
    UINT32 AllocSize,
    UINT16 Align
)
{
    UINTN                   Ptr;
    UINT32                  Size;
    EFI_STATUS              Status;
    EFI_PHYSICAL_ADDRESS    MemAddress;

    if (AllocSize == 0) {
        return NULL;
    }
    //
    // If this is the first time the function is called,
    // allocate the USB memory and make the size 4K aligned (VTD).
    //
    if (gFirstCall) {
        // Make sure to only allocate once.
        gFirstCall = FALSE;
        // Express Size in pages.
        Size = AllocSize >> 12;
        //
        // Allocate and zero memory in pages.
        //
        MemAddress = 0xFFFFFFFF;
        Status = gBS->AllocatePages(AllocateMaxAddress, EfiRuntimeServicesData,
                Size, &MemAddress);
        if (EFI_ERROR(Status)) {
            Status = gBS->AllocatePages(AllocateAnyPages, EfiRuntimeServicesData, Size, &MemAddress);
            if (EFI_ERROR(Status)) {
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AllocAlignedMemory AllocateAnyPages %r\n", Status);
                return NULL;
            }
        }

        gGlobalPointer = (VOID*)(UINTN)MemAddress;
        SetMem (gGlobalPointer, (Size << 12), 0);
        //
        // Save pointers to beginning and end of region.
        //
        gStartPointer = gGlobalPointer;
        gEndPointer = (VOID *)((UINTN)gGlobalPointer + (Size << 12) - 1);
        
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Memroy Allocation: 0x%0lx - 0x%0lx, Data size: 0x%0lx\n", 
                     (UINTN)gStartPointer, (UINTN)gEndPointer, (Size << 12));
        
        return gStartPointer;
    }


    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_6, "Unaligned : %Fp, %X, %X\n", gGlobalPointer, AllocSize, Align);
    Ptr = AlignPhysicalAddress((UINTN)gGlobalPointer, Align);
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_6, "Aligned : %Fp, %X, %X\n", Ptr, AllocSize, Align);

    gGlobalPointer = (VOID*)(Ptr + AllocSize);

    if (gGlobalPointer < gEndPointer) {
        return (VOID*)Ptr;
    }
    return NULL;
}

/**
    Allocates a number of pages with the given alignment.
    @param  PciIo         Pointer to the PciIo structure
    @param  Pages         Number of pages
    @param  Alignment     Alignment requirements

    @retval TRUE          External controller
    @retval FALSE         Not External controller

    @note  The minimum alignment passed to this function is CPU page, 4K
**/

VOID*
EFIAPI
AllocateHcMemory (
    IN EFI_PCI_IO_PROTOCOL      *PciIo,
    IN UINTN                    Pages,
    IN UINTN                    Alignment
)
{
    EFI_STATUS            Status;
    VOID                  *Memory;
    UINTN                 AlignedMemory;
    UINTN                 AlignmentMask;
    UINTN                 UnalignedPages;
    UINTN                 RealPages;

    //
    // Alignment must be a power of two or zero.
    //
    ASSERT ((Alignment & (Alignment - 1)) == 0);
 
    if (Pages == 0) {
        return NULL;
    }
    if (Alignment > EFI_PAGE_SIZE) {
        //
        // Caculate the total number of pages since alignment is larger than page size.
        //
        AlignmentMask  = Alignment - 1;
        RealPages      = Pages + EFI_SIZE_TO_PAGES (Alignment);
        //
        // Make sure that Pages plus EFI_SIZE_TO_PAGES (Alignment) does not overflow.
        //
        ASSERT (RealPages > Pages);

        Memory = (VOID*)0xFFFFFFFF;
        Status = PciIo->AllocateBuffer(PciIo, AllocateMaxAddress, EfiRuntimeServicesData,
                                    RealPages, &Memory, 0);
        if (EFI_ERROR(Status)) {
            Status = PciIo->AllocateBuffer(PciIo, AllocateAnyPages, EfiRuntimeServicesData,
                                    RealPages, &Memory, 0);
            if (EFI_ERROR(Status)) {
                return NULL;
            }
        }
        AlignedMemory = ((UINTN) Memory + AlignmentMask) & ~AlignmentMask;
        UnalignedPages = EFI_SIZE_TO_PAGES(AlignedMemory - (UINTN)Memory);
        if (UnalignedPages > 0) {
            //
            // Free first unaligned page(s).
            //
            Status = PciIo->FreeBuffer(PciIo, UnalignedPages, Memory);
            if (EFI_ERROR(Status)) {
                return NULL;
            }
        }
        Memory = (VOID*)(AlignedMemory + EFI_PAGES_TO_SIZE(Pages));
        UnalignedPages = RealPages - Pages - UnalignedPages;
        if (UnalignedPages > 0) {
            //
            // Free last unaligned page(s).
            //
            Status = PciIo->FreeBuffer(PciIo, UnalignedPages, Memory);
            if (EFI_ERROR(Status)) {
                return NULL;
            }
        }
    } else {
        //
        // Do not over-allocate pages in this case.
        //
        Memory = (VOID*)0xFFFFFFFF;
        Status = PciIo->AllocateBuffer(PciIo, AllocateMaxAddress, EfiRuntimeServicesData,
                     Pages, &Memory, 0);
        if (EFI_ERROR(Status)) {
            Status = PciIo->AllocateBuffer(PciIo, AllocateAnyPages, EfiRuntimeServicesData,
                     Pages, &Memory, 0);
            if (EFI_ERROR(Status)) {
                return NULL;
            }
        }
        AlignedMemory = (UINTN)Memory;
    }
    return (VOID*)AlignedMemory;
}

/**
    This function returns the beginning and the end of USB 
    runtime memory region.
    @param  Start         Beginning of USB runtime memory region
    @param  End           End of USB runtime memory region

    @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
GetRuntimeRegion (
    OUT EFI_PHYSICAL_ADDRESS *Start,
    OUT EFI_PHYSICAL_ADDRESS *End
)
{
    *Start = (EFI_PHYSICAL_ADDRESS)gStartPointer;
    *End = (EFI_PHYSICAL_ADDRESS)gEndPointer;
    return EFI_SUCCESS;
}

/**
    Free the memory allocated by AllocateHcMemory().
    @param  PciIo         Pointer to the PciIo structure
    @param  Pages         Number of pages
    @param  Memory        Pointer to the memory address

    @retval None

**/

VOID
EFIAPI
FreeHcMemory (
    IN EFI_PCI_IO_PROTOCOL  *PciIo,
    IN UINTN                Pages,
    IN VOID*                Memory
)
{
    EFI_STATUS            Status;

    Status = PciIo->FreeBuffer(PciIo, Pages, Memory);
    ASSERT_EFI_ERROR(Status);
    return;
}

/**
    @param  OldSize         Original size of memory allocated
    @param  NewSize         New size of memory for reallocate
    @param  OldBuffer       Pointer to the memory address allocated with new size
    @retval EFI_SUCCESS     Success to allocate memory
    @retval EFI_STATUS      Status of the operation

**/

EFI_STATUS
EFIAPI
ReallocateMemory (
  IN UINTN  OldSize,
  IN UINTN  NewSize,
  IN VOID   **OldBuffer
)
{
    EFI_STATUS  Status;
    VOID        *NewBuffer = NULL;
    VOID        *FreeBuffer = NULL;

    if (OldBuffer == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->AllocatePool (EfiRuntimeServicesData, NewSize, (VOID**)&NewBuffer);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    SetMem(NewBuffer, NewSize, 0);

    if (OldSize > 0 && *OldBuffer != NULL) {
        CopyMem(NewBuffer, *OldBuffer, (OldSize < NewSize) ? OldSize : NewSize);
        FreeBuffer = *OldBuffer;
    }

    *OldBuffer = NewBuffer;

    if (FreeBuffer != NULL) {
        gBS->FreePool(FreeBuffer);
    }
    return EFI_SUCCESS;
}

/**
    This function record USB host controller memory start address 
    and total memory pages used
    @param  Handle              Host controller handle number
    @param  MemoryStartAddress  Host controller memory start address
    @param  MemoryTotalPages    Host controller total memory used

    @retval EFI_STATUS Status of the operation

**/
EFI_STATUS
EFIAPI
UsbHcMemoryRecord (
    EFI_HANDLE              Handle,
    EFI_PHYSICAL_ADDRESS    MemoryStartAddress,
    UINTN                   MemoryTotalPages
)
{
    EFI_STATUS                      Status;
    EFI_GUID                        AmiUsbHcMemRecordProtocolGuid = AMI_USB_HC_MEM_RECORD_PROTOCOL_GUID;
    AMI_USB_HC_MEM_RECORD_PROTOCOL  *AmiUsbHcMemRecord = NULL;

    Status = gBS->HandleProtocol (
                      Handle,
                      &AmiUsbHcMemRecordProtocolGuid,
                      (VOID **) &AmiUsbHcMemRecord
                      );

    if (EFI_ERROR (Status)){
        Status = gBS->AllocatePool (
                          EfiBootServicesData,
                          sizeof(AMI_USB_HC_MEM_RECORD_PROTOCOL),
                          (VOID**)&AmiUsbHcMemRecord
                          );
        if (EFI_ERROR (Status)) {
            return Status;
        }

        SetMem (AmiUsbHcMemRecord, sizeof(AMI_USB_HC_MEM_RECORD_PROTOCOL), 0);
        AmiUsbHcMemRecord->HcMemStart = MemoryStartAddress;
        AmiUsbHcMemRecord->MemTotalPages = MemoryTotalPages;
        Status = gBS->InstallProtocolInterface (
                          &Handle,
                          &AmiUsbHcMemRecordProtocolGuid,
                          EFI_NATIVE_INTERFACE,
                          AmiUsbHcMemRecord
                          );
        if (EFI_ERROR (Status)) {
            gBS->FreePool (AmiUsbHcMemRecord);
            return Status;
        }
    }
    return Status;
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
