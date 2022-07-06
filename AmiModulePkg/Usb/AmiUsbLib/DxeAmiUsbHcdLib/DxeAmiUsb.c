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

/** @file DxeAmiUsb.c
    AMI USB API implementation. The following code will be
    copied to SMM; only RT functions can be used. gUsbData
    is obtained from AMIUHCD in the entry point and can be
    used afterwards.

**/

#include <AmiDef.h>
#include <AmiUsb.h>
#include <Library/AmiUsbHcdLib.h>

extern USB_GLOBAL_DATA               *gUsbData;


/**
    This function is call to init USB hcd driver.

    @param   ImageHandle   The handle associated with this image being loaded into memory
    @param   SystemTable   Pointer to the system table

    @retval  EFI_SUCCESS   Sucess to initial Usb hcd Driver.

**/
EFI_STATUS
EFIAPI
InitUsbHcdDriver(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_STATUS    Status;

    Status = InstallUsbProtocols(); 
    InitializeUsbGlobalData();

    return Status;
}

/*
    This function initializes the USB global data.

    @retval  EFI_SUCCESS   Sucess to Install USB protocol.
**/

EFI_STATUS
EFIAPI
InstallUsbProtocols(
    VOID
)
{
    EFI_STATUS        Status;
    EFI_USB_PROTOCOL  *UsbProtocol;

    Status = gBS->LocateProtocol(&gEfiUsbProtocolGuid, NULL, (VOID**)&UsbProtocol);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    gUsbData = UsbProtocol->USBDataPtr;

    if (gUsbData->UsbRuntimeDriverInSmm==USB_RUNTIME_DRV_MODE_0) {
        UsbProtocol->UsbRtKbcAccessControl = UsbKbcAccessControl;
        //Hook USB legacy control function for shutdown/init USB legacy support
        UsbProtocol->UsbLegacyControl = USBRT_LegacyControl;
        UsbProtocol->UsbStopUnsupportedHc = USB_StopUnsupportedHc;
    }
    UsbProtocol->UsbInvokeApi = UsbApiHandler;

    return Status;
}


/*
    This function is interrupt call back routine.

    @param   IntDevInfo   Pointer to Interrupt devcie info table.
    @param   Buffer       Pointer to device buffer
    @param   DataLength   Data length.


    @retval  EFI_UNSUPPORTED   This routine unsupported.
**/

EFI_STATUS
EFIAPI
InterruptCallBack(
    VOID       *IntDevInfo,
    UINT8      *Buffer,
    UINT16     DataLength
)
{
    USBHC_INTERRUPT_DEVNINFO_T *InterruptDevInfo = (USBHC_INTERRUPT_DEVNINFO_T*)IntDevInfo;
    if (InterruptDevInfo->CallbackFunction) {
        InterruptDevInfo->CallbackFunction(
            Buffer, DataLength, InterruptDevInfo->Context, EFI_USB_NOERROR);
    }
    return EFI_SUCCESS;
}

/*
    This function is install SMM handle.

    @param HcStruc   Ptr to the host controller structure
    
    @retval  EFI_UNSUPPORTED   This routine unsupported.
**/

EFI_STATUS
EFIAPI
InstallSmmHandle(
    HC_STRUC*    HcStruc
)
{
    return EFI_UNSUPPORTED;
}


/*
    Get the specific device capability descriptor from BOS descriptor.

    @param BosDesc       The pointer of whole BOS descriptor.
    @param CapDescType   The target device capaility descriptor type
    @param DescLength     Out put the descriptor Length.
    @param Buffer        The pointer of the destination buffer.
    @param BufLength      buffer Length.

    @retval  EFI_INVALID_PARAMETER   The pointer of DescLength is null.
    @retval  EFI_BAD_BUFFER_SIZE     Get the descriptor, but the pointer of buffer is null.
    @retval  EFI_SUCCESS             Sucess to get the descriptor and copy to the buffer.
    @retval  EFI_NOT_FOUND           The descriptor is not found.
**/
EFI_STATUS
EFIAPI
GetBosDevCapDesc(
    IN   UINT8   *BosDesc,
    IN   UINT8   CapDescType,
    OUT  UINT8   *DescLength,
    IN   VOID    *Buffer,
    IN   UINT32  BufLength
)
{
    UINT8          Index; 
    UINT8          Offset = 0;
    BOS_CAP_DESC   *CapDesc;
    
    if (DescLength == NULL){
      return EFI_INVALID_PARAMETER;  
    }
    
    for (Index = 0; Index < ((BOS_DESC *)BosDesc)->NumDeviceCaps; ++Index) {
        Offset +=((BOS_DESC *)(BosDesc + Offset))->DescLength; 
        CapDesc = (BOS_CAP_DESC*)(BosDesc + Offset);  
      
      if (CapDesc->DevCapType == CapDescType){
         *DescLength = CapDesc->DescLength;
        if (Buffer == NULL){
            return EFI_BAD_BUFFER_SIZE;  
        }

        if (BufLength < CapDesc->DescLength)
          CopyMem(Buffer, CapDesc, BufLength);
        else
          CopyMem(Buffer, CapDesc, CapDesc->DescLength);

        return EFI_SUCCESS;
      }
      
    }
    
    *DescLength = 0;
    return EFI_NOT_FOUND;
}

/*
    This function sets Usb SMI.

    @param Enable   TRUE: Enable USB SMI/ FALSE: Disable USB SMI 
    
    @retval  EFI_UNSUPPORTED   This routine unsupported.
**/
EFI_STATUS
EFIAPI
SettingUsbSmi(
    BOOLEAN    Enable
)
{
    return EFI_UNSUPPORTED;  
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
