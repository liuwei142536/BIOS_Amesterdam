//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
    SMM instance of the SmmAmiUsbHcLib library class.
**/
#include <Library/SmmServicesTableLib.h>
#include <AmiDef.h>
#include <UsbDef.h>
#include <Xhci.h>

extern EFI_GUID             gAmiSmmUsbHcdProtocolGuid;

HCD_HEADER           *gSmmUsbHcdTable = NULL;

USB_GLOBAL_DATA      gSmmUsbData;
USB_DATA_LIST        gSmmUsbDataList;
HC_STRUC             *gSmmHcStrucs[MAX_USB_HC];
HC_STRUC             gSmmHcStruc[MAX_USB_HC];
USB3_HOST_CONTROLLER gSmmUsb3Hc[MAX_USB_HC];

USB_GLOBAL_DATA      *gDxeUsbData      = NULL;
USB_DATA_LIST        *gDxeUsbDataList  = NULL;
HC_STRUC             **gDxeHcStruc     = NULL;
HC_STRUC             **gDxeHcStrucAddr = NULL;
USB3_HOST_CONTROLLER *gDxeUsb3Hc[MAX_USB_HC];

AMI_USB_SMM_HCD_PROTOCOL  gUsbSmmHcdProtocol;

/**
  This routine call to get Host Controller Driver Table.
   @param  None

   @retval USB_SUCCESS        Success to get USB Host controller protocol
   @retval EFI_UNSUPPORTED    Failure
**/
EFI_STATUS
EFIAPI
GetHcdTable (
)
{
    AMI_USB_SMM_HCD_PROTOCOL  *UsbSmmHcdProtocol;
    EFI_STATUS                Status;

    if (gSmmUsbHcdTable==NULL){
        Status = gSmst->SmmLocateProtocol (
                            &gAmiSmmUsbHcdProtocolGuid,
                            NULL,
                            (VOID**)&UsbSmmHcdProtocol
                            );
        if (!EFI_ERROR(Status))
            gSmmUsbHcdTable = UsbSmmHcdProtocol->UsbHcdTable;
    }
    if (gSmmUsbHcdTable!=NULL) {
        return EFI_SUCCESS;
    } else {
        return EFI_UNSUPPORTED;
    }
}

/**
  This routine is call to initila Host Controller Driver Table.

  @param  UsbHcdTable          The Usb Host controller driver table Pointer.
  @param  UsbGlobdaDataAddr    The Usb Global data address.
  @param  HcTableAddr          The Usb Host table address.
**/
VOID 
EFIAPI
InitHcdTable(
  IN    HCD_HEADER        *UsbHcdTable,
  IN    VOID              *UsbGlobdaDataAddr,
  IN    VOID              *HcTableAddr,
  IN    HCD_MEM_HEADER    *UsbHcdMemHeader
)
{ 
    EFI_HANDLE     ImageHandle = NULL;
    
    gSmmUsbHcdTable = UsbHcdTable;
    gUsbSmmHcdProtocol.UsbHcdTable       = UsbHcdTable;
    gUsbSmmHcdProtocol.UsbGlobalDataAddr = UsbGlobdaDataAddr;
    gUsbSmmHcdProtocol.HcTableAddr       = HcTableAddr;
    gUsbSmmHcdProtocol.Flag              = 0;
    gUsbSmmHcdProtocol.UsbSmmHcdMemHeader = UsbHcdMemHeader;

    // Install SMM USB Hcd protocol
    gSmst->SmmInstallProtocolInterface (
        &ImageHandle,
        &gAmiSmmUsbHcdProtocolGuid,
        EFI_NATIVE_INTERFACE,
        &gUsbSmmHcdProtocol
        );
}

/**
    This API function is called to start a Usb host controller.
    The input to the routine is the pointer to the HC structure
    that defines this host controller

    @param HcStruc   Ptr to the host controller structure

    @retval USB_SUCCESS  Success to start Usb host controller.
    @retval USB_ERROR    Fail to start Usb host controller.

**/

UINT8
EFIAPI
AmiUsbControllerStart(
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdStart==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdStart(HcStruc);
}
/**
    This function stops the Usb controller.

    @param HcStruc   Host controller's HCStruc structure

    @retval USB_SUCCESS  Success to stop USB controller.
    @retval USB_ERROR    Fail to stop USB controller.
    
**/

UINT8
EFIAPI
AmiUsbControllerStop(
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdStop==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdStop(HcStruc);
}

/**
    This function enumerates the HC ports for devices

    @param HcStruc   Host controller's HCStruc structure

    @retval USB_SUCCESS  Success to enumerate port.
    @retval USB_ERROR    Fail to enumerate port.

**/

UINT8
EFIAPI
AmiUsbEnumeratePorts(
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnumeratePorts==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnumeratePorts(HcStruc);
}

/**
    This function disables the HC interrupts

    @param HcStruc   Pointer to the HCStruc structure

    @retval USB_ERROR   Fail to disabe USB interrupt.
    @retval USB_SUCCESS Success to disable USB interrupt.

**/

UINT8
EFIAPI
AmiUsbDisableInterrupts (
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableInterrupts==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableInterrupts(HcStruc);
}

/**
    This function enables the HC interrupts

    @param HcStruc   Pointer to the HCStruc structure

    @retval USB_ERROR   Fail to enable USB interrupt.
    @retval USB_SUCCESS Success to enable USB interrupt.

**/

UINT8
EFIAPI
AmiUsbEnableInterrupts (
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableInterrupts==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableInterrupts(HcStruc);
}
/**
    This function is called when the USB interrupt bit is
    set.
    
    @param HcStruc      Pointer to the HCStruc structure

    @retval USB_ERROR   Need more Interrupt processing
    @retval USB_SUCCESS No interrupts pending

**/

UINT8
EFIAPI
AmiUsbProcessInterrupt(
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdProcessInterrupt==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdProcessInterrupt(HcStruc);
}

/**
    This function returns the port connect status for the
    root hub port

    @param HcStruc     Pointer to HCStruc of the host controller
    @param PortNum     Port in the HC whose status is requested
    @param ClearChangeBits     TRUE: Clear all change status bits.

    @retval Value      Port status flags (see USB_PORT_STAT_XX equates)

**/

UINT32
EFIAPI
AmiUsbGetRootHubStatus(
    HC_STRUC    *HcStruc,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdGetRootHubStatus==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdGetRootHubStatus(
               HcStruc, PortNum, ClearChangeBits);
}

/**
    This function disables the EHCI HC Ruoot hub port.

    @param HcStruc      Pointer to HCStruc of the host controller
    @param PortNum      Port in the HC to disable

    @retval USB_SUCCESS Success to disable root hub.
    @retval USB_ERROR   Fail to disable root hub.

**/

UINT8
EFIAPI
AmiUsbDisableRootHub(
    HC_STRUC    *HcStruc,
    UINT8       PortNum
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableRootHub==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableRootHub(
               HcStruc, PortNum);
}

/**
    This function enables the EHCI HC Root hub port.

    @param HcStruc      Pointer to HCStruc of the host controller
    @param PortNum      Port in the HC to enable

    @retval USB_SUCCESS Success to enable root hub.
    @retval USB_ERROR   Fail to enable root hub.

**/

UINT8
EFIAPI
AmiUsbEnableRootHub(
    HC_STRUC    *HcStruc,
    UINT8       PortNum
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableRootHub==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableRootHub(
               HcStruc, PortNum);
}

/**
    This function executes a device request command transaction
    on the USB. One setup packet is generated containing the
    device request parameters supplied by the caller.  The setup
    packet may be followed by data in or data out packets
    containing data sent from the host to the device
    or vice-versa. This function will not return until the
    request either completes successfully or completes in error
    (due to time out, etc.)

    @param HcStruc     Pointer to HCStruc of the host controller
    @param DevInfo     DeviceInfo structure (if available else 0)
    @param Request     Request type (low byte)
                       Bit 7   : Data direction
                                 0 = Host sending data to device
                                 1 = Device sending data to host
                       Bit 6-5 : Type
                                 00 = Standard USB request
                                 01 = Class specific
                                 10 = Vendor specific
                                 11 = Reserved
                       Bit 4-0 : Recipient
                                 00000 = Device
                                 00001 = Interface
                                 00010 = Endpoint
                                 00100 - 11111 = Reserved
                       Request code, a one byte code describing
                       the actual device request to be executed
                       (ex: Get Configuration, Set Address etc)
    @param Index      Index request parameter (meaning varies)
    @param Value      Value request parameter (meaning varies)
    @param Buffer     Buffer containing data to be sent to the
                      device or buffer to be used to receive data
    @param Length     Length request parameter, number of bytes
                      of data to be transferred in or out
                      of the host controller

    @retval Value     Number of bytes actually transferred

**/

UINT16
EFIAPI
AmiUsbControlTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT16      Request,
    UINT16      Index,
    UINT16      Value,
    UINT8       *Buffer,
    UINT16      Length
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return 0;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdControlTransfer==NULL) return 0;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdControlTransfer(
               HcStruc, DevInfo, Request, Index, Value, Buffer, Length);
}


/**
    This function executes a bulk transaction on the USB. The
    transfer may be either DATA_IN or DATA_OUT packets containing
    data sent from the host to the device or vice-versa. This
    function wil not return until the request either completes
    successfully or completes with error (due to time out, etc.)
    @note  Make sure that amount of bytes to transfer should not
              exceed MAX_EHCI_DATA_SIZE

    @param HcStruc    Pointer to HCStruc of the host controller
    @param DevInfo    DeviceInfo structure (if available else 0)
    @param XferDir    Transfer direction
                      Bit 7: Data direction
                      0 Host sending data to device
                      1 Device sending data to host
                      Bit 6-0 : Reserved
    @param Buffer     Buffer containing data to be sent to the
                      device or buffer to be used to receive data
                      value in Segment:Offset format
    @param Length     Length request parameter, number of bytes
                      of data to be transferred in or out
                      of the host controller

    @retval Value     Amount of data transferred

**/

UINT32
EFIAPI
AmiUsbBulkTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return 0;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdBulkTransfer==NULL) return 0;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdBulkTransfer(
               HcStruc, DevInfo, XferDir,Buffer, Length);
}

/**
    This function executes a isochronous transaction on the USB. The transfer may be
    either DATA_IN or DATA_OUT packets containing data sent from the host to the
    device or vice-versa. Depending on the value passed in AsyncIndicator this transfer
    would either be a blocking (synchronous) or non-blocking (asynchronous).

    @param  HcStruc         Pointer to HCStruc of the host controller
    @param  DevInfo         DeviceInfo structure (if available else 0)
    @param  XferDir         Transfer direction
                            Bit 7: Data direction
                              0 Host sending data to device
                              1 Device sending data to host
                            Bit 6-0 : Reserved
    @param  Buffer          32-bit buffer containing data to be sent to the device or buffer
                            to be used to receive data
    @param  Length          Number of bytes of data to be transferred in or out
    @param  AsyncIndicator  If NULL, the call is not returned until all data is transferred
                                if not NULL, it points to the UINT8 data which will be updated
                                when transfer is complete.

    @retval Value           For blocking transfer - amount of data transferred, for non-blocking transfer 0
                            Transfer status is updated in gUsbData->bLastCommandStatus and in 
                            gUsbData->UsbLastCommandStatusExtended

    @note Isochronous transfer implies no checking for the data transmission
          errors, i.e. transfer completes successfully when the last iTD becomes inactive.
**/
UINT32
EFIAPI
AmiUsbIsocTransfer(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length,
    UINT8       *AsyncIndicator
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return 0;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdIsocTransfer == NULL) return 0;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdIsocTransfer(
               HcStruc, DevInfo, XferDir, Buffer, Length, AsyncIndicator);
}
/**
    This function executes an interrupt transaction on the USB.
    The data transfer direction is always DATA_IN. This
    function will not return until the request either completes
    successfully or completes in error (due to time out, etc.)

    @param HcStruc         Pointer to HCStruc of the host controller
    @param DevInfo         DeviceInfo structure (if available else 0)
    @param EndpointAddress The destination USB device endpoint to which the device request 
                           is being sent.
    @param MaxPktSize      Indicates the maximum packet size the target endpoint is capable 
                           of sending or receiving.
    @param Buffer          Buffer containing data to be sent to the
                           device or buffer to be used to receive data
    @param Length          Length request parameter, number of bytes
                           of data to be transferred in

    @retval Value          Number of bytes transferred


    @note  DO NOT TOUCH THE LINK POINTER OF THE TDInterruptData. It is
              statically allocated and linked with other items in the
              1ms schedule

**/

UINT16
EFIAPI
AmiUsbInterruptTransfer (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       EndpointAddress,
    UINT16      MaxPktSize,
    UINT8       *Buffer,
    UINT16      Length
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return 0;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdInterruptTransfer == NULL) return 0;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdInterruptTransfer(
               HcStruc, DevInfo, EndpointAddress, MaxPktSize, Buffer, Length);
}

/**
    This function disables the keyboard repeat rate logic by
    enabling the repeat TD

    @param HcStruc    Pointer to the HCStruc structure

    @retval USB_SUCCESS Success to disable key repeat
    @retval USB_ERROR   Fail to disable key repeat


**/

UINT8
EFIAPI
AmiUsbDisableKeyRepeat (
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableKeyRepeat == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDisableKeyRepeat(
               HcStruc);
}

/**
    This function disables the keyboard repeat rate logic by
    enabling the repeat TD

    @param HcStruc   Pointer to the HCStruc structure

    @retval USB_SUCCESS Success to enable key repeat
    @retval USB_ERROR   Fail to enable key repeat

**/

UINT8
EFIAPI
AmiUsbEnableKeyRepeat (
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableKeyRepeat == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableKeyRepeat(
               HcStruc);
}

/**
    This function parses the device descriptor data and enables the endpoints
    by 1)assigning the Transfer TRB and 2)executing ConfigureEndpoint command
    for the slot. Section 4.3.5.

    @param HcStruc   Pointer to the HCStruc structure
    @param DevInfo   A device for which the endpoins are being enabled
    @param Desc      Device Configuration Descriptor data pointer

    @retval USB_SUCCESS Success to enable endpoints
    @retval USB_ERROR   Fail to enable endpoints.

    @note  
  1) DevInfo->DevMiscInfo points to the device context
  2) This call is executed before SET_CONFIGURATION control transfer
  3) EP0 information is valid in the Device

**/

UINT8
EFIAPI
AmiUsbEnableEndpoints (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Desc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableEndpoints == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdEnableEndpoints(
               HcStruc, DevInfo, Desc);
}
/**
    This function activates the polling QH for the requested
    device. The device may be a USB keyboard or USB hub

    @param HcStruc     Pointer to the HC structure
    @param DevInfo     Pointer to the device information structure

    @retval USB_SUCCESS Success to active polling
    @retval USB_ERROR   Fail to active polling

    @note  For the keyboard device this routine allocates TDRepeat
              also, if it is not already allocated. This routine allocate
              a polling TD and schedule it to 8ms schedule for keyboards
              and to 1024ms schedule for hubs.

**/

UINT8
EFIAPI
AmiUsbActivatePolling(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdActivatePolling == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdActivatePolling(
               HcStruc, DevInfo);
}
/**
    This function de-activates the polling QH for the requested
    device. The device may be a USB keyboard or USB hub

    @param HcStruc   Pointer to the HC structure
    @param DevInfo   Pointer to the device information structure

    @retval USB_SUCCESS Success to deactivate polling
    @retval USB_ERROR   Fail to deactivate polling

**/

UINT8
EFIAPI
AmiUsbDeactivatePolling(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDeactivatePolling == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDeactivatePolling(
               HcStruc, DevInfo);
}


/**
    This is an API function for early device initialization.

    @param HcStruc    Pointer to the HC structure
    @param DevInfo    Pointer to the device information structure
    @param PortStatus Port status.
    @param DeviceData Pointer to the device data.

    @retval USB_SUCCESS Success to Init device data
    @retval USB_ERROR   Fail to Init devcie data

**/

UINT8
EFIAPI
AmiUsbInitDeviceData(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT32      PortStatus,
    UINT8       **DeviceData
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdInitDeviceData == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdInitDeviceData(
               HcStruc, DevInfo, PortStatus, DeviceData);
}

/**
    This is an API function for removing device related information from HC.
    @param HcStruc    Pointer to the HC structure
    @param DevInfo    Pointer to the device information structure


    @retval USB_SUCCESS Success to DeInit device data
    @retval USB_ERROR   Fail to DeInit devcie data

**/

UINT8
EFIAPI
AmiUsbDeinitDeviceData(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDeinitDeviceData == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdDeinitDeviceData(
               HcStruc, DevInfo);
}

/**
    This function resets the EHCI HC Root hub port.

    @param HcStruc      Pointer to HCStruc of the host controller
    @param PortNum      Port in the HC to enable

    @retval USB_SUCCESS Success to reset root hub
    @retval USB_ERROR   Faile to reset root hub

**/

UINT8
EFIAPI
AmiUsbResetRootHub(
    HC_STRUC    *HcStruc,
    UINT8       PortNum
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdResetRootHub == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdResetRootHub(
               HcStruc, PortNum);
}


/**
    This function is call to clear endpoint state.

    @param HcStruc      Pointer to HCStruc of the host controller
    @param DevInfo      Pointer to DEV_INFO structure
    @param Endpoint     The target endpoint

    @retval USB_SUCCESS Success to clear endpoint state
    @retval USB_ERROR   Faile to clear endpoint state

**/

UINT8
EFIAPI
AmiUsbClearEndpointState(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       Endpoint
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdClearEndpointState == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdClearEndpointState(
               HcStruc, DevInfo, Endpoint);
}

/**
    This function is call to suspend Usb host and device.

    @param HcStruc      Pointer to HCStruc of the host controller

    @retval USB_SUCCESS Success to suspend USB host and device
    @retval USB_ERROR   Faile to suspend USB host and device

**/

UINT8
EFIAPI
AmiUsbGlobalSuspend (
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdGlobalSuspend == NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdGlobalSuspend(
               HcStruc);
}

/**
    This function is call to control SMI enable/disable

    @param HcStruc      Pointer to HCStruc of the host controller
    @param Enable       TRUE  : Enable USB SMI
                        FALSE : Disable USB SMi 

    @retval USB_SUCCESS Success to control SMI enable/disable
    @retval USB_ERROR   Faile to control SMI enable/disable

**/
UINT8
EFIAPI
AmiUsbSmiControl (
    HC_STRUC    *HcStruc,
    BOOLEAN     Enable
)
{
    EFI_STATUS    Status;

    Status = GetHcdTable();
    if (EFI_ERROR(Status)) return USB_ERROR;

    if (gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdSmiControl==NULL) return USB_ERROR;
    return gSmmUsbHcdTable[GET_HCD_INDEX(HcStruc->HcType)].FnHcdSmiControl(
               HcStruc, Enable);
}

/**
    This function is call to switch USB data in SMM or DXE

    @param UsbGlobdaDataAddr  Pointer to Usb global data address.
    @param UsbDataListAddr    Pointer to Usb data list address.
    @param HcTableAddr        Pointer to Usb Host table address.
    @param DataInSmm          Pointer to Usb SMM flag address.
    @param Direction          USB_DATA_DXE_TO_SMM  : Usb data in SMM.
                              USB_DATA_SMM_TO_DXE  : Usb data in DXE. 

    @retval EFI_SUCCESS Success to switch USB dta to SMM or DXE.

**/
EFI_STATUS
EFIAPI
AmiUsbGlobalDataSwitch(
    USB_GLOBAL_DATA    **UsbGlobdaDataAddr,
    USB_DATA_LIST      **UsbDataListAddr,
    HC_STRUC           **HcTableAddr,
    UINT8              *DataInSmm,
    UINT8              Direction
)
{
    UINTN     Index = 0;
    UINTN     KbIndex = 0;
    UINTN     IndexUsb3 = 0;
    switch (Direction) {
        case USB_DATA_DXE_TO_SMM:

                 if((gUsbSmmHcdProtocol.Flag &(DATA_IN_SMM|SMM_HCD_DATA_VALID))== (DATA_IN_SMM|SMM_HCD_DATA_VALID)){
                     return EFI_SUCCESS;
                   }

                 // Store gUsbData gUsbDataList gHcStruc
                 if (gDxeUsbData == NULL) gDxeUsbData = *UsbGlobdaDataAddr;
                 if (gDxeUsbDataList == NULL) gDxeUsbDataList = *UsbDataListAddr;
                 if (gDxeHcStruc == NULL) gDxeHcStruc = (HC_STRUC**)*HcTableAddr;
                 if (gDxeHcStrucAddr == NULL) gDxeHcStrucAddr = HcTableAddr;
                 
                 //Init gSmmUsbDataList
                 CopyMem(&gSmmUsbDataList, *UsbDataListAddr, sizeof(USB_DATA_LIST));
                 *UsbDataListAddr = &gSmmUsbDataList;
                                 
                 //Init gSmmUsbData
                 CopyMem(&gSmmUsbData, *UsbGlobdaDataAddr, sizeof(USB_GLOBAL_DATA));
                 InitializeListHead(&gSmmUsbData.DevConfigInfoList);
                 for (Index = 0; Index < MAX_DEV_CONFIG_LINK; Index++){
                     if ((gSmmUsbData.DevConfigLink[Index].DevConfigInfo.Vid != 0)&&
                         (gSmmUsbData.DevConfigLink[Index].DevConfigInfo.Did != 0))
                         InsertTailList(&gSmmUsbData.DevConfigInfoList, &gSmmUsbData.DevConfigLink[Index].Link);
                 }
                 (*UsbDataListAddr)->MouseInputBufferHeadPtr = &gSmmUsbData.UsbMsInputBuffer[0];
                 (*UsbDataListAddr)->MouseInputBufferTailPtr = &gSmmUsbData.UsbMsInputBuffer[0];
                 SetMem(gSmmUsbData.KbcCharacterBufferStart, sizeof(gSmmUsbData.KbcCharacterBufferStart), 0);

                 (*UsbDataListAddr)->KbcCharacterBufferHead = &gSmmUsbData.KbcCharacterBufferStart[0];
                 (*UsbDataListAddr)->KbcCharacterBufferTail = &gSmmUsbData.KbcCharacterBufferStart[0];

                 SetMem(gSmmUsbData.KbcScanCodeBufferStart, sizeof(gSmmUsbData.KbcScanCodeBufferStart), 0);
                 (*UsbDataListAddr)->KbcScanCodeBufferPtr = &gSmmUsbData.KbcScanCodeBufferStart[0];
                 ZeroMem(gSmmUsbData.KbcDeviceIdBufferStart, sizeof(gSmmUsbData.KbcDeviceIdBufferStart));
                 ZeroMem(gSmmUsbData.KbcShiftKeyStatusBufferStart, sizeof(gSmmUsbData.KbcShiftKeyStatusBufferStart));
                 ZeroMem(gSmmUsbData.UsbKbInputBuffer, sizeof(gSmmUsbData.UsbKbInputBuffer));

                 gSmmUsbData.UsbStateFlag  &= ~(USB_FLAG_RUNNING_UNDER_EFI);

                 *UsbGlobdaDataAddr = &gSmmUsbData;
                 gUsbSmmHcdProtocol.UsbGlobalDataAddr = (VOID*)&gSmmUsbData;

                 //Copy HcStruc.
                 for (Index = 0; Index < (*UsbDataListAddr)->HcTableCount; Index++){
                     CopyMem((VOID*)&gSmmHcStruc[Index], (VOID*)(*UsbDataListAddr)->HcTable[Index], sizeof(HC_STRUC));
                     gSmmHcStrucs[Index] = &gSmmHcStruc[Index];
                     if (gSmmHcStrucs[Index]->Usb3HcData != NULL) {
                         CopyMem((VOID*)&gSmmUsb3Hc[IndexUsb3], (VOID*)gSmmHcStrucs[Index]->Usb3HcData, sizeof(USB3_HOST_CONTROLLER));
                         gDxeUsb3Hc[IndexUsb3] = gSmmHcStrucs[Index]->Usb3HcData;
                         gSmmHcStrucs[Index]->Usb3HcData = &gSmmUsb3Hc[IndexUsb3];
                         IndexUsb3++;
                     }
                 }
                 *HcTableAddr = (HC_STRUC*)&gSmmHcStrucs[0];
                 gSmmUsbDataList.HcTable = &gSmmHcStrucs[0];
                 gUsbSmmHcdProtocol.HcTableAddr = (VOID*)&gSmmHcStrucs[0];
                 gSmmUsbData.UsbDataList = &gSmmUsbDataList;
                 gUsbSmmHcdProtocol.Flag |= DATA_IN_SMM|SMM_HCD_DATA_VALID;
                 *DataInSmm = TRUE;
                 break;
        case USB_DATA_SMM_TO_DXE: 
             break;  //Currently, not allow to change USB data from SMM to DXE.
             if((!(gUsbSmmHcdProtocol.Flag & DATA_IN_SMM)) && (gUsbSmmHcdProtocol.Flag & SMM_HCD_DATA_VALID)){          
                return EFI_SUCCESS;
             }
            //Init gUsbDataList
            *UsbDataListAddr = gDxeUsbDataList;

                 //Init gUsbData
            CopyMem(gDxeUsbData, &gSmmUsbData, sizeof(USB_GLOBAL_DATA));

                 InitializeListHead(&gDxeUsbData->DevConfigInfoList);
                 for (Index = 0; Index < MAX_DEV_CONFIG_LINK; Index++){
                     if ((gSmmUsbData.DevConfigLink[Index].DevConfigInfo.Vid != 0)&&
                         (gSmmUsbData.DevConfigLink[Index].DevConfigInfo.Did != 0))
                         InsertTailList(&gDxeUsbData->DevConfigInfoList, &gDxeUsbData->DevConfigLink[Index].Link);
                 }
                 (*UsbDataListAddr)->MouseInputBufferHeadPtr = &gDxeUsbData->UsbMsInputBuffer[0];
                 (*UsbDataListAddr)->MouseInputBufferTailPtr = &gDxeUsbData->UsbMsInputBuffer[0];
                 SetMem(gDxeUsbData->KbcCharacterBufferStart, sizeof(gDxeUsbData->KbcCharacterBufferStart), 0);

                 (*UsbDataListAddr)->KbcCharacterBufferHead = &gDxeUsbData->KbcCharacterBufferStart[0];
                 (*UsbDataListAddr)->KbcCharacterBufferTail = &gDxeUsbData->KbcCharacterBufferStart[0];

                 SetMem(gDxeUsbData->KbcScanCodeBufferStart, sizeof(gDxeUsbData->KbcScanCodeBufferStart), 0);
                 (*UsbDataListAddr)->KbcScanCodeBufferPtr = &gDxeUsbData->KbcScanCodeBufferStart[0];
                 ZeroMem(gDxeUsbData->KbcDeviceIdBufferStart, sizeof(gDxeUsbData->KbcDeviceIdBufferStart));
                 ZeroMem(gDxeUsbData->KbcShiftKeyStatusBufferStart, sizeof(gDxeUsbData->KbcShiftKeyStatusBufferStart));
                 ZeroMem(gDxeUsbData->UsbKbInputBuffer, sizeof(gDxeUsbData->UsbKbInputBuffer));

                 gSmmUsbData.UsbStateFlag  |= USB_FLAG_RUNNING_UNDER_EFI;

                 *UsbGlobdaDataAddr = gDxeUsbData;
                 gUsbSmmHcdProtocol.UsbGlobalDataAddr = (VOID*)gDxeUsbData;

                 //Copy HcStruc.
                 for (Index = 0; Index < gSmmUsbDataList.HcTableCount; Index++){
                     CopyMem((VOID*)gDxeHcStruc[Index], (VOID*)&gSmmHcStruc[Index], sizeof(HC_STRUC));
                     if (gSmmHcStrucs[Index]->Usb3HcData != NULL) {
                         gSmmHcStrucs[Index]->Usb3HcData = gDxeUsb3Hc[IndexUsb3];
                         CopyMem((VOID*)gDxeHcStruc[Index]->Usb3HcData, (VOID*)(VOID*)&gSmmUsb3Hc[IndexUsb3], sizeof(USB3_HOST_CONTROLLER));
                         IndexUsb3++;
                     }
                 }
                 *HcTableAddr = (HC_STRUC*)gDxeHcStruc;
                 gSmmUsbDataList.HcTable = gDxeHcStruc;
                 gUsbSmmHcdProtocol.HcTableAddr = (VOID*)gDxeHcStrucAddr;

                 *DataInSmm = FALSE;
                 gUsbSmmHcdProtocol.Flag &= ~DATA_IN_SMM;
                 gUsbSmmHcdProtocol.Flag |= SMM_HCD_DATA_VALID;
                 break;
    }

    return EFI_SUCCESS;
}

/**
    Creates a record for the beginning of a performance measurement.

    @param  Handle          Pointer to environment specific context used
                            to identify the component being measured.
    @param  Token           Pointer to a Null-terminated ASCII string
                            that identifies the component being measured.
    @param  Module          Pointer to a Null-terminated ASCII string
                            that identifies the module being measured.
    @param  TimeStamp       64-bit time stamp.

    @retval EFI_SUCCESS     The start of the measurement was recorded.

**/
EFI_STATUS
EFIAPI
AmiUsbPerfStart(
    IN VOID   *Handle,  
    IN CHAR8  *Token,   
    IN CHAR8  *Module,  
    IN UINT64 TimeStamp
)
{
    return EFI_SUCCESS;
}

/**
    Fills in the end time of a performance measurement.

    @param  Handle         Pointer to environment specific context used
                           to identify the component being measured.
    @param  Token          Pointer to a Null-terminated ASCII string
                           that identifies the component being measured.
    @param  Module         Pointer to a Null-terminated ASCII string
                           that identifies the module being measured.
    @param  TimeStamp      64-bit time stamp.

    @retval EFI_SUCCESS    The end of  the measurement was recorded.

**/
EFI_STATUS
EFIAPI
AmiUsbPerfEnd(
    IN VOID   *Handle,
    IN CHAR8  *Token,
    IN CHAR8  *Module,
    IN UINT64 TimeStamp
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
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
