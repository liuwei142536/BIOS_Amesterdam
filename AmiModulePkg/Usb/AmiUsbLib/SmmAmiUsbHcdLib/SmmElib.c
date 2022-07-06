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

/** @file SmmElib.c
    AMI USB MEM/IO/PCI access routines

**/

#include <AmiDef.h>
#include <UsbDef.h>
#include <Library/AmiBufferValidationLib.h>
#include <Library/AmiUsbSmmGlobalDataValidationLib.h>
#include <Library/SmmServicesTableLib.h>

#ifndef EFI_SMM_RUNTIME_SERVICES_TABLE_GUID
#define EFI_SMM_RUNTIME_SERVICES_TABLE_GUID \
    { 0x395c33fe, 0x287f, 0x413e, { 0xa0, 0x55, 0x80, 0x88, 0xc0, 0xe1, 0xd4, 0x3e } }
#endif

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;
extern UINT8           gDataInSmm;

UINT32
EFIAPI
HcReadPciReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset
);

VOID
EFIAPI
DwordWritePCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register,
    UINT32  Value
);

VOID
EFIAPI
WordWritePCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register,
    UINT16  Value
);

UINT32
EFIAPI
DwordReadMem(
    UINTN   BaseAddr, 
    UINT32  Offset
);

VOID
EFIAPI
DwordWriteMem(
    UINTN   BaseAddr, 
    UINT32  Offset, 
    UINT32  Value
);

UINT8
EFIAPI
ByteReadIO(
    UINT16 IoAddr
);

VOID
EFIAPI
ByteWriteIO(
    UINT16  IoAddr,
    UINT8   Value
);

UINT16
EFIAPI
WordReadIO(
    UINT16 IoAddr
);

VOID
EFIAPI
WordWriteIO(
    UINT16 IoAddr,
    UINT16 Value
);

UINT32
EFIAPI
DwordReadIO(
    UINT16 IoAddr
);

VOID
EFIAPI
DwordWriteIO(
    UINT16 IoAddr,
    UINT32 Value
);

UINT32
EFIAPI
ReadPCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register
);

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
    UINTN   Counter;
    UINTN   Index;
    UINT32  Data32;
    UINT32  PrevData;

    Counter = Usec * 3;
    Counter += Usec / 2;
    Counter += (Usec * 2) / 25;

    //
    // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
    // periods, thus attempting to ensure Microseconds of stall time.
    //
    if (Counter != 0) {

        //PM1_TMR
        PrevData = IoRead32(PM_BASE_ADDRESS + 8);
        for (Index = 0;  Index < Counter; ) {
            //PM1_TMR 
            Data32 = IoRead32(PM_BASE_ADDRESS + 8);
            // Reset if there is a overlap
            if (Data32 < PrevData) {
                PrevData = Data32;
                continue;
            }
            Index += (Data32 - PrevData);
            PrevData = Data32;
        }
    }
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
    return ReadPCIConfig(HcStruc->BusDevFuncNum, (UINT8)Offset);
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
    DwordWritePCIConfig(HcStruc->BusDevFuncNum, (UINT8)Offset, Data);
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
    WordWritePCIConfig(HcStruc->BusDevFuncNum, (UINT8)Offset, Data);
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
    if (Offset > HcStruc->BaseAddressSize) {
        return 0;
    }
    return DwordReadMem(HcStruc->BaseAddress, Offset);
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
    if ((Offset + sizeof(UINT32)) > HcStruc->BaseAddressSize) {
        return;
    }
    DwordWriteMem(HcStruc->BaseAddress, Offset, Data);
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
    return ByteReadIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset);
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
    ByteWriteIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset, Data);
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
    return WordReadIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset);
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
    WordWriteIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset, Data);
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
    return DwordReadIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset);
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
    DwordWriteIO((UINT16)HcStruc->BaseAddress + (UINT16)Offset, Data);
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
    UINT8       *MemBlockEnd = NULL;

    if (gUsbData == NULL || gUsbDataList == NULL) {
        *PhyAddr = BufferAddr;
        return USB_SUCCESS;
    }
    MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    if ((!(gUsbData->UsbStateFlag & USB_FLAG_EDKII_IOMMU_PROTOCOL_ON)) ||
        (HcStruc->MemBlockStartPhy == NULL) ||
        ((UINTN)BufferAddr < (UINTN)gUsbDataList->MemBlockStart) ||
        (((UINTN)BufferAddr) > (UINTN)MemBlockEnd)) {
        *PhyAddr = BufferAddr;
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Smm HcDmaMap PhyAddr %x\n", BufferAddr);
    } else {
        *PhyAddr = (UINT8*)((UINTN)(HcStruc->MemBlockStartPhy) + ((UINTN)BufferAddr - (UINTN)(gUsbDataList->MemBlockStart)));
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Smm HcDmaMap BufferAddr %x\n", *PhyAddr);
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
    return USB_SUCCESS;
}

/**
    This routine is call to validate USB data.

    @param UsbData    Usb global data.
    
    @retval USB_SUCCESS  Success to validate USB data.
**/
EFI_STATUS
EFIAPI
AmiUsbGlobalDataValidation(
    USB_GLOBAL_DATA *UsbData
)
{
    return AmiUsbSmmGlobalDataValidation((USB_GLOBAL_DATA*)&gUsbData, (USB_DATA_LIST*)&gUsbDataList, &gDataInSmm);
}

/**
    This routine is call to validate memory buffer.

    @param Buffer        The buffer to validate
    @param BufferSize    The buffer size to validate
    
    @retval USB_SUCCESS  Success to validate USB buffer.
**/
EFI_STATUS
EFIAPI
AmiUsbValidateMemoryBuffer (
    CONST VOID  *Buffer,
    CONST UINTN BufferSize
)
{
    EFI_STATUS    Status;
    
    Status = AmiValidateMemoryBuffer(Buffer, BufferSize);
    if (Status == EFI_ABORTED) Status = EFI_ACCESS_DENIED;
    return Status;
}

/**
    This routine is call to validate MMIO buffer.

    @param Buffer        The buffer to validate
    @param BufferSize    The buffer size to validate
    
    @retval USB_SUCCESS  Success to validate USB buffer.
**/
EFI_STATUS
EFIAPI
AmiUsbValidateMmioBuffer (
    CONST VOID   *Buffer,
    CONST UINTN  BufferSize
)
{
    EFI_STATUS    Status;
    
    Status = AmiValidateMmioBuffer(Buffer, BufferSize);
    if (Status == EFI_ABORTED) Status = EFI_ACCESS_DENIED;
    return Status;
}

/**
    This routine is call to reset system.

    @retval EFI_SUCCESS  Success to reset system.
**/
EFI_STATUS
EFIAPI
AmiUsbResetSystem (
    VOID
)
{
    UINTN                 SmmTableIndex = 0;
    EFI_RUNTIME_SERVICES  *SmmRuntimeVar = NULL;
    EFI_GUID              SmmRsTableGuid = EFI_SMM_RUNTIME_SERVICES_TABLE_GUID;

    for (; SmmTableIndex < gSmst->NumberOfTableEntries; ++SmmTableIndex) {
        if (CompareGuid(&gSmst->SmmConfigurationTable[SmmTableIndex].VendorGuid,
            &SmmRsTableGuid) == TRUE) {
            break;
        }
    }
    if (SmmTableIndex != gSmst->NumberOfTableEntries) {
        SmmRuntimeVar =(EFI_RUNTIME_SERVICES *)gSmst->SmmConfigurationTable[SmmTableIndex].VendorTable;
    }
    if ((SmmRuntimeVar != NULL) && (SmmRuntimeVar->ResetSystem != NULL))
        SmmRuntimeVar->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);

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
