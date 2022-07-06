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

/** @file Elib.c
    AMI USB MEM/IO/PCI access routines

**/


#include "AmiDef.h"
#include "UsbDef.h"

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

UINT32
EFIAPI
HcReadHcMem(
    HC_STRUC    *HcStruc,
    UINT32      Offset
);

VOID
EFIAPI
HcWriteHcMem(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Data
);

VOID
EFIAPI
FixedDelay(
    UINTN           Usec                           
);

/**
    USB Host controller structure validation

    @param  HcStruc    Pointer to host controller structure     
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error
**/
EFI_STATUS
UsbHcStrucValidation(
    HC_STRUC* HcStruc
)
{
    UINTN       Index;

    if (HcStruc == NULL) {
        return EFI_ACCESS_DENIED;
    }
    
    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        if (HcStruc == gHcTable[Index]) {
            break;
        }
    }

    if (Index == gUsbDataList->HcTableCount) {
        return EFI_ACCESS_DENIED;
    }

    if ((HcStruc->HcFlag & (HC_STATE_USED | HC_STATE_INITIALIZED)) 
        != (HC_STATE_USED | HC_STATE_INITIALIZED)) {
        return EFI_ACCESS_DENIED;
    }

    return EFI_SUCCESS;
}


/**
    USB device information structure validation

    @param  DevInfo    Pointer to DEV_INFO structure     
    
    @retval EFI_SUCCESS       Success
    @retval Others            Error
**/
EFI_STATUS
UsbDevInfoValidation(
    DEV_INFO* DevInfo
)
{
    UINTN       Index;
    UINT8       *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    if (DevInfo == NULL) {
        return EFI_ACCESS_DENIED;
    }
    
    for (Index = 0; Index < gUsbData->MaxDevCount; Index++) {
        if (DevInfo == &gUsbDataList->DevInfoTable[Index]) {
            break;
        }
    }

    if (Index == gUsbData->MaxDevCount) {
        return EFI_ACCESS_DENIED;
    }


    if (gUsbData->UsbRuntimeDriverInSmm){
        if (DevInfo->PollEdPtr) {
            if (((UINTN)DevInfo->PollEdPtr < (UINTN)gUsbDataList->MemBlockStart) ||
                (((UINTN)DevInfo->PollEdPtr + sizeof(MEM_BLK)) > (UINTN)MemBlockEnd)) {
                return EFI_ACCESS_DENIED;
            }
        }
        
        if (DevInfo->PollTdPtr) {
            if (((UINTN)DevInfo->PollTdPtr < (UINTN)gUsbDataList->MemBlockStart) ||
                (((UINTN)DevInfo->PollTdPtr + sizeof(MEM_BLK)) > (UINTN)MemBlockEnd)) {
                return EFI_ACCESS_DENIED;
            }
        }
        
        if (DevInfo->PollDataBuffer) {
            if (((UINTN)DevInfo->PollDataBuffer < (UINTN)gUsbDataList->MemBlockStart) ||
                (((UINTN)DevInfo->PollDataBuffer + DevInfo->PollingLength) > (UINTN)MemBlockEnd)) {
                return EFI_ACCESS_DENIED;
            }
        }
    }
    return EFI_SUCCESS;
}

/**
    This routine reads a DWORD from the specified Memory Address

    @param BaseAddr   Memory address
    @param Offset     Offset of BaseAddr

    @retval Value     read data

**/

UINT32
EFIAPI
DwordReadMem(
    UINTN   BaseAddr, 
    UINT32  Offset
)
{
    return MmioRead32((UINTN)(BaseAddr + Offset));
}


/**
    This routine writes a dword to a specified Memory Address

    @param  BaseAddr   Memory address
    @param  Offset     Offset of BaseAddr
    @param  Value      Write value
    @retval None
**/

VOID
EFIAPI
DwordWriteMem(
    UINTN   BaseAddr, 
    UINT32  Offset, 
    UINT32  Value
)
{
    MmioWrite32((UINTN)(BaseAddr + Offset), Value);
}


/**
    This routine resets the specified bits at specified memory address

    This routine writes a dword to a specified Memory Address

    @param  BaseAddr   Memory address
    @param  Offset     Offset of BaseAddr
    @param  Value      value to reset
    @retval None

**/

VOID
EFIAPI
DwordResetMem(
    UINTN   BaseAddr, 
    UINT32  Offset, 
    UINT32  Value
)
{
    UINT32 Data = DwordReadMem(BaseAddr, Offset);
    
    Data &= ~Value;
    DwordWriteMem(BaseAddr, Offset, Data);
}


/**
    This routine sets the specified bits at specified memory address

    @param  BaseAddr   Memory address
    @param  Offset     Offset of BaseAddr
    @param  Value      value to set
    
    @retval None
**/

VOID
EFIAPI
DwordSetMem(
    UINTN   BaseAddr, 
    UINT32  Offset, 
    UINT32  Value
)
{
    UINT32 Data = DwordReadMem(BaseAddr, Offset);
    
    Data |= Value;
    DwordWriteMem(BaseAddr, Offset, Data);
}


/**
    This routine reads a byte from the specified IO address

    @param IoAddr     I/O address to read

    @retval Value     Value read

**/

UINT8
EFIAPI 
ByteReadIO(
    UINT16 IoAddr
)
{
    return IoRead8(IoAddr);
}


/**
    This routine writes a byte to the specified IO address

    @param IoAddr     I/O address to write
    @param Value      Byte value to write

    @retval None

**/

VOID
EFIAPI
ByteWriteIO(
    UINT16  IoAddr,
    UINT8   Value
)
{
    IoWrite8(IoAddr, Value);
}


/**
    This routine reads a word from the specified IO address

    @param IoAddr     I/O address to read
    
    @retval Value      Byte value to read

**/

UINT16
EFIAPI
WordReadIO(
    UINT16 IoAddr
)
{
    return IoRead16(IoAddr);
}


/**
    This routine writes a word to the specified IO address

    @param
    @param  IoAddr     I/O address to write
    @param  Value      Word value to write

    @retval None

**/

VOID
EFIAPI
WordWriteIO(
    UINT16 IoAddr,
    UINT16 Value
)
{
    IoWrite16(IoAddr, Value);
}


/**
    This routine reads a dword from the specified IO address

    @param  IoAddr     I/O address to read
    @retval Value read

**/

UINT32
EFIAPI
DwordReadIO(
    UINT16 IoAddr
)
{
    return IoRead32(IoAddr);
}


/**
    This routine writes a double word to the specified IO address

    @param
        IoAddr     I/O address to write
        Value      Double word value to write

    @retval VOID

**/

VOID
EFIAPI
DwordWriteIO(
    UINT16 IoAddr, 
    UINT32 Value
)
{
    IoWrite32(IoAddr, Value);
}


/**
    This routine reads from the PCI configuration space register
    the value can be typecasted to 8bits - 32bits

    @param
    @param BusDevFunc   Bus, device & function number of the PCI device
    @param Register     Register offset to read

    @retval Value       Value to read

**/

UINT32
EFIAPI
ReadPCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register
)
{
    UINT32  Data;
    UINTN   Address = (gUsbData->PciExpressBaseAddress + (UINTN)(BusDevFunc << 12 | (Register & 0xFC)));

    Data = *(volatile UINT32*)(Address);
   
    return (Data >> ((Register & 3) << 3));
}

/**
    This routine writes a byte value to the PCI configuration
    register space


    @param  BusDevFunc   Bus, device & function number of the PCI device
    @param  Register     Register offset to write
    @param  Value        Value to write
    
    @retval None
**/

VOID
EFIAPI
ByteWritePCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register,
    UINT8   Value
)
{
    UINTN   Address = (gUsbData->PciExpressBaseAddress + (UINTN)(BusDevFunc << 12 | Register));

    *(volatile UINT8*)(Address) = Value;
}

/**
    This routine writes a word value to the PCI configuration
    register space

    @param  BusDevFunc   Bus, device & function number of the PCI device
    @param  Register     Register offset to write
    @param  Value        Value to write
    
    @retval None

**/

VOID
EFIAPI
WordWritePCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register,
    UINT16  Value
)
{
    UINTN   Address = (gUsbData->PciExpressBaseAddress + (UINTN)(BusDevFunc << 12 | Register));

    *(volatile UINT16*)(Address) = Value;
}

/**
    This routine writes a Dword value to the PCI configuration
    register space

    @param  BusDevFunc   Bus, device & function number of the PCI device
    @param  Register     Register offset to write
    @param  Value        Value to write
    
    @retval None

**/

VOID
EFIAPI
DwordWritePCIConfig(
    UINT16  BusDevFunc,
    UINT8   Register,
    UINT32  Value
)
{
    UINTN   Address = (gUsbData->PciExpressBaseAddress + (UINTN)(BusDevFunc << 12 | Register));

    *(volatile UINT32*)(Address) = Value;
}


/**
    This routine sets the specified bits to the MMIO register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to set
    @param Data        Value to set
    
    @retval None
        
**/

VOID
EFIAPI
HcSetHcMem(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Bit
)
{
    UINT32  Data;

    if (Offset > HcStruc->BaseAddressSize) {
        return;
    }

    Data = HcReadHcMem(HcStruc, Offset) | Bit;
    HcWriteHcMem(HcStruc, Offset, Data);
    return;
}

/**
    This routine clears the specified bits to the MMIO register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to set
    @param Bit        Bit to clear
    
    @retval None
        
**/

VOID
EFIAPI
HcClearHcMem(
    HC_STRUC *HcStruc,
    UINT32  Offset,
    UINT32  Bit
)
{
    UINT32  Data;

    if (Offset > HcStruc->BaseAddressSize) {
        return;
    }

    Data = HcReadHcMem(HcStruc, Offset) & ~Bit;
    HcWriteHcMem(HcStruc, Offset, Data);
    return;
}

/**
    This routine reads a dword value from the operational register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to read
    
    @retval Value      Value to read
**/

UINT32
EFIAPI
HcReadOpReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset
)
{
    return HcReadHcMem(HcStruc, HcStruc->OpRegOffset + Offset);
}

/**
    This routine write a dword value to the operational register
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to write
    @param Data        Value to write
    
    @retval None
        
**/

VOID
EFIAPI
HcWriteOpReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Data
)
{
    HcWriteHcMem(HcStruc, HcStruc->OpRegOffset + Offset, Data);
    return;
}

/**
    This routine sets the specified bits to the operational register 
    of the host controller.
        
    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to set
    @param Data        Value to set
    
    @retval None            
**/

VOID
EFIAPI
HcSetOpReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Bit
)
{
    UINT32  Data;

    Data = HcReadOpReg(HcStruc, Offset) | Bit;
    HcWriteOpReg(HcStruc, Offset, Data);
    return;
}

/**
    This routine clears the specified bits to the operational register 
    of the host controller.

    @param HcStruc     HcStruc pointer
    @param Offset      Register offset to clear
    @param Data        Value to clear
    
    @retval None         
**/

VOID
EFIAPI
HcClearOpReg(
    HC_STRUC    *HcStruc,
    UINT32      Offset,
    UINT32      Bit
)
{
    UINT32  Data;

    Data = HcReadOpReg(HcStruc, Offset) & ~Bit;
    HcWriteOpReg(HcStruc, Offset, Data);
    return;
}


/**
    This routine produces a sound on the internal PC speaker
    
    @param Freq         Sound frequency
    @param Duration     Sound duration in 15 microsecond units
    @param HcStruc      Pointer to HCStruc

    @retval None

**/

VOID
EFIAPI
SpeakerBeep(
    UINT8       Freq,
    UINT16      Duration, 
    HC_STRUC    *HcStruc
)
{

    UINT8   Value;
    if ((gUsbData->UsbFeature & USB_BEEP_SUPPORT) == USB_BEEP_SUPPORT){
        if (gUsbData->UsbStateFlag & USB_FLAG_ENABLE_BEEP_MESSAGE) {
            ByteWriteIO((UINT8)0x43, (UINT8)0xB6);
            ByteWriteIO((UINT8)0x42, (UINT8)Freq);
            ByteWriteIO((UINT8)0x42, (UINT8)Freq);
            Value = ByteReadIO((UINT8)0x61);
            ByteWriteIO((UINT8)0x61, (UINT8)(Value | 03));
            FixedDelay((UINTN)Duration * 15);
            ByteWriteIO((UINT8)0x61, (UINT8)(Value));
        }
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
