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

/** @file SmmAmiUsb.c
    AMI USB API implementation. The following code will be
    copied to SMM; only RT functions can be used. gUsbData
    is obtained from AMIUHCD in the entry point and can be
    used afterwards.

**/

#include <AmiDef.h>
#include <AmiUsb.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/TimerLib.h>
#include <Library/AmiBufferValidationLib.h>
#include <Library/AmiUsbSmmGlobalDataValidationLib.h>
#include <Protocol/AmiUsbHid.h>
#include <Library/AmiUsbHcdLib.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmUsbDispatch2.h>

extern EFI_EMUL6064MSINPUT_PROTOCOL  *gMsInput;
extern EFI_EMUL6064KBDINPUT_PROTOCOL *gKbdInput;
extern EFI_EMUL6064TRAP_PROTOCOL     *gEmulationTrap;
extern USB_GLOBAL_DATA               *gUsbData;
extern UINT8                         gDataInSmm;
extern BOOLEAN gLockSmiHandler;
extern BOOLEAN gLockHwSmiHandler;
extern BOOLEAN gCheckUsbApiParameter;
extern API_FUNC gUsbApiTable[];
extern API_FUNC gUsbMassApiTable[];
extern URP_STRUC       *gParameters;
extern USB_DATA_LIST   *gUsbDataList;
extern UINT8           gDataInSmm;
extern HC_STRUC        **gHcTable;
extern UINT8           gValidateErrorCode;

EFI_STATUS EFIAPI SettingTimerSMI(IN BOOLEAN  EnableSmi); 
VOID    EFIAPI USBKeyRepeat(HC_STRUC*,UINT8);

EFI_STATUS
EFIAPI
USBSWSMIHandler(
        IN EFI_HANDLE   DispatchHandle,
        IN CONST VOID   *Context OPTIONAL,
        IN OUT VOID     *CommBuffer OPTIONAL,
        IN OUT UINTN    *CommBufferSize OPTIONAL
);
EFI_STATUS
EFIAPI
Emul6064TrapCallback(
        IN CONST EFI_GUID  *Protocol,
        IN VOID            *Interface,
        IN EFI_HANDLE      Handle
);

AMI_USB_SMM_PROTOCOL    gUsbSmmProtocol = {0};

/**
    SMM entry point of AMIUSB driver
    
    @param  ImageHandle   Image handle number
    @param  SystemTable   Pointer to system table
    
    @retval EFI_STATUS    Status of the operation
**/

EFI_STATUS
InSmmFunction(
    EFI_HANDLE          ImageHandle,
    EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      SwSmiHandle = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT     SwSmiContext;
    EFI_SMM_SW_DISPATCH2_PROTOCOL   *SwSmiDispatch;
    UINT32                          KbcEmulFeature = 0;
    VOID                            *ProtocolNotifyRegistration;
    EFI_HANDLE                      UsbSmmProtocolHandle = NULL;

    InstallUsbProtocols();
    InitializeUsbGlobalData();
    
    Status = gSmst->SmmLocateProtocol(
                    &gEmul6064TrapProtocolGuid,
                    NULL,
                    (VOID**)&gEmulationTrap);

    if (EFI_ERROR(Status)) {
        Status = gSmst->SmmRegisterProtocolNotify(
                    &gEmul6064TrapProtocolGuid,
                    Emul6064TrapCallback,
                    &ProtocolNotifyRegistration
                    );
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Call Back Register 6064Trap Status = %r\n", Status);
        }
    }
    if (!gUsbData->KbcSupport) {
        Status = gSmst->SmmLocateProtocol(&gEmul6064MsInputProtocolGuid, 
                                    NULL, (VOID**)&gMsInput);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Call Back Locate 6064Ms Status = %r\n", Status);
        Status = gSmst->SmmLocateProtocol(&gEmul6064KbdInputProtocolGuid,
                                    NULL, (VOID**)&gKbdInput);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Call Back Locate 6064Kbd Status = %r\n", Status);
        if (Status == EFI_SUCCESS) {
            gUsbData->UsbStateFlag |= USB_FLAG_6064EMULATION_ON;
            if (gEmulationTrap) {
                KbcEmulFeature = gEmulationTrap->FeatureSupported(gEmulationTrap);
            }
            if (KbcEmulFeature & IRQ_SUPPORTED) {
                gUsbData->UsbStateFlag |= USB_FLAG_6064EMULATION_IRQ_SUPPORT;
            }
        } else {
            InitSysKbc( &gKbdInput, &gMsInput );
        }
    } else {
        //
        //Init Fake Emulation interface
        //
        InitSysKbc( &gKbdInput, &gMsInput );
    }
    
    if ((gUsbData->UsbFeature & USB_IRQ_INTERRUPT_SUPPORT) == 0){
        Status = USBSB_InstallSmiEventHandlers();
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AMIUSB global data at 0x%x\n", gUsbData);

    //
    // Register the USB SW SMI handler
    //
    Status = gSmst->SmmLocateProtocol(&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwSmiDispatch);

    if (EFI_ERROR (Status)) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_0, "SmmSwDispatch protocol: %r\n", Status);
        return Status;
    }

    SwSmiContext.SwSmiInputValue = gUsbData->UsbSwSmi;
    Status = SwSmiDispatch->Register(SwSmiDispatch, USBSWSMIHandler, &SwSmiContext, &SwSmiHandle);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AMIUSB SW SMI registration:: %r\n", Status);

    gUsbSmmProtocol.UsbStopUnsupportedHc = USB_StopUnsupportedHc;
    gUsbSmmProtocol.UsbApiTable = gUsbApiTable;
    gUsbSmmProtocol.UsbMassApiTable = gUsbMassApiTable;
    //
    // Create Random Value by Getting PerformanceCounter.
    //
    Status = gSmst->SmmInstallProtocolInterface(
                    &UsbSmmProtocolHandle,
                    &gAmiUsbSmmProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &gUsbSmmProtocol
                    );
        
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AMIUSB SMM protocol: %r\n", Status);

    return Status;
}


/**
    Invoked on reads from SW SMI port with value USB_SWSMI. This
    function dispatches the USB Request Packets (URP) to the
    appropriate functions.

    @param   EBDA:USB_DATA_EBDA_OFFSET  Pointer to the URP (USB Request
                                        Packet structure)
    @param   DispatchHandle   SMM Handle
    @param   Context          Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param   CommBuffer       Pointer to Intput buffer
    @param   CommBufferSize   Pointer to Intput buffer size

    @retval  EFI_SUCCESS      Success to exectue SW function.
**/
EFI_STATUS
EFIAPI
USBSWSMIHandler (
    EFI_HANDLE    DispatchHandle,
    CONST VOID    *Context OPTIONAL,
    VOID          *CommBuffer OPTIONAL,
    UINTN         *CommBufferSize OPTIONAL
)
{
    URP_STRUC   *UsbUrp;
    UINT16      EbdaSeg;
    EFI_STATUS  Status;
    VOID        *AddrCheck = ((VOID*)0x40E);
    
    if (gLockSmiHandler == TRUE) {
        return EFI_SUCCESS;
    }

    Status = AmiUsbSmmGlobalDataValidation((USB_GLOBAL_DATA*)&gUsbData, (USB_DATA_LIST*)&gUsbDataList, &gDataInSmm);
    if (EFI_ERROR(Status)) USB_DEBUG(DEBUG_ERROR, 3, "Sw SMI validate DATA fail. Error code = %X\n",gValidateErrorCode);
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) {
        gLockHwSmiHandler = TRUE;
        gLockSmiHandler = TRUE;
        return EFI_SUCCESS;
    }
    
    Status = AmiValidateMemoryBuffer(gParameters, sizeof(URP_STRUC));
    if (EFI_ERROR(Status)) return EFI_SUCCESS;
    if (gParameters->bFuncNumber != 0) {                          // Call from AMIUSB C area
        UsbUrp = gParameters;
    } else {
        if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_OS) {
            return EFI_SUCCESS;
        }

        Status = AmiValidateMemoryBuffer(AddrCheck, sizeof(UINT32));
        if (EFI_ERROR(Status)) 
            return EFI_SUCCESS;

        //
        // Get the UsbUrp pointer from EBDA
        //
        EbdaSeg = *((UINT16*)0x40E);
        UsbUrp = *(URP_STRUC**)(UINTN)(((UINT32)EbdaSeg << 4) + USB_DATA_EBDA_OFFSET);
        UsbUrp = (URP_STRUC*)((UINTN)UsbUrp & 0xFFFFFFFF);
        Status = AmiValidateMemoryBuffer((VOID*)UsbUrp, sizeof(URP_STRUC));
        if (EFI_ERROR(Status)) {
            USB_DEBUG(DEBUG_ERROR, 3, "UsbApiHandler Invalid Pointer, the address is in SMRAM.\n");
            return EFI_SUCCESS;
        }


        if((UsbUrp->bFuncNumber == USB_API_CORE_PROC)||(UsbUrp->bFuncNumber == USB_API_MOVE_DATA_AREA)){
          return EFI_SUCCESS;        
        }

    }

    if (UsbUrp == NULL) {
        return EFI_SUCCESS;
    }

    gCheckUsbApiParameter = TRUE;
    
    UsbApiHandler(UsbUrp);
    
    gCheckUsbApiParameter = FALSE;

    return EFI_SUCCESS;
}

/**
    USB Hardware SMI handler.

    @param Host controller type.

**/
VOID
UsbHwSmiHandler (UINT8 HcType)
{
    UINT8       Index;
    HC_STRUC    *HcStruc;
    EFI_STATUS  Status;

    if (gLockHwSmiHandler == TRUE) {
        return;
    }

    Status = AmiUsbSmmGlobalDataValidation((USB_GLOBAL_DATA*)&gUsbData, (USB_DATA_LIST*)&gUsbDataList, &gDataInSmm);
    if (EFI_ERROR(Status)) USB_DEBUG(DEBUG_ERROR, 3, "Hw SMI validate DATA fail. Error code = %X\n",gValidateErrorCode);
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) {
        gLockHwSmiHandler = TRUE;
        gLockSmiHandler = TRUE;
        return;
    }
    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if ((HcStruc->HcFlag & (HC_STATE_USED | HC_STATE_INITIALIZED)) 
                != (HC_STATE_USED | HC_STATE_INITIALIZED)) {
                continue;
        }
        if (HcStruc->HcType == HcType) { // Process appropriate interrupt
            AmiUsbProcessInterrupt(HcStruc);
        }
    }

}

/**
    UHCI Hardware SMI handler.

    @param   DispatchHandle   SMM Handle
    @param   Context          Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param   CommBuffer       Pointer to Intput buffer
    @param   CommBufferSize   Pointer to Intput buffer size

    @retval EFI_SUCCESS       Success to execture USB HW SMI handler

**/

EFI_STATUS
EFIAPI
UhciHWSMIHandler (
        EFI_HANDLE      DispatchHandle,
        CONST VOID      *Context,
        VOID            *CommBuffer,
        UINTN           *CommBufferSize
)
{
    UsbHwSmiHandler(USB_HC_UHCI);
    return EFI_SUCCESS;
}

/**
    Ohci Hardware SMI handler.

    @param   DispatchHandle   SMM Handle
    @param   Context          Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param   CommBuffer       Pointer to Intput buffer
    @param   CommBufferSize   Pointer to Intput buffer size

    @retval EFI_SUCCESS       Success to execture USB HW SMI handler

**/

EFI_STATUS
EFIAPI
OhciHWSMIHandler (
    EFI_HANDLE  DispatchHandle,
    CONST VOID  *Context,
    VOID        *CommBuffer,
    UINTN       *CommBufferSize
)
{
    UsbHwSmiHandler(USB_HC_OHCI);
    return EFI_SUCCESS;
}

/**
    Ehci Hardware SMI handler.

    @param   DispatchHandle   SMM Handle
    @param   Context          Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param   CommBuffer       Pointer to Intput buffer
    @param   CommBufferSize   Pointer to Intput buffer size

    @retval EFI_SUCCESS       Success to execture USB HW SMI handler

**/

EFI_STATUS
EFIAPI
EhciHWSMIHandler (
    EFI_HANDLE  DispatchHandle,
    CONST VOID  *Context,
    VOID        *CommBuffer,
    UINTN       *CommBufferSize
)
{
    UsbHwSmiHandler(USB_HC_EHCI);
    return EFI_SUCCESS;
}

/**
    Xhci Hardware SMI handler.

    @param   DispatchHandle   SMM Handle
    @param   Context          Pointer to the EFI_SMM_SW_DISPATCH_CONTEXT
    @param   CommBuffer       Pointer to Intput buffer
    @param   CommBufferSize   Pointer to Intput buffer size

    @retval EFI_SUCCESS       Success to execture USB HW SMI handler

**/

EFI_STATUS
EFIAPI
XhciHwSmiHandler (
    EFI_HANDLE  DispatchHandle,
    CONST VOID  *Context,
    VOID        *CommBuffer,
    UINTN       *CommBufferSize
)
{
    UsbHwSmiHandler(USB_HC_XHCI);
    return EFI_SUCCESS;
}

/**
    This function registers USB hardware SMI callback function.

    @param HcStruc   Ptr to the host controller structure
    
    @retval EFI_SUCCESS      Success to install USB HW SMI handler
    @retval EFI_UNSUPPORTED  The USB HW SMI unsupported.

**/

EFI_STATUS
EFIAPI
UsbInstallHwSmiHandler(
    HC_STRUC    *HcStruc
)
{
    EFI_STATUS                      Status;
    EFI_SMM_USB_REGISTER_CONTEXT    UsbContext;
    EFI_SMM_USB_DISPATCH2_PROTOCOL  *UsbDispatch;
    EFI_SMM_HANDLER_ENTRY_POINT2    UsbCallback;
    EFI_HANDLE                      Handle = NULL;

    if (HcStruc->HwSmiHandle != NULL) {
        return EFI_SUCCESS;
    }
        
    Status = gSmst->SmmLocateProtocol(
                 &gEfiSmmUsbDispatch2ProtocolGuid,
                 NULL,
                (VOID**)&UsbDispatch);

    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    switch (HcStruc->HcType) {
                case USB_HC_UHCI:
                     if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == 0)
                         return EFI_UNSUPPORTED;
                     UsbCallback = UhciHWSMIHandler;
                     break;
                case USB_HC_OHCI:
                     if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == 0)
                         return EFI_UNSUPPORTED;
                     UsbCallback = OhciHWSMIHandler;
                     break;
                case USB_HC_EHCI:
                     if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == 0)
                         return EFI_UNSUPPORTED;
                     UsbCallback = EhciHWSMIHandler;
                     break;
                case USB_HC_XHCI:
                     if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == 0)
                         return EFI_UNSUPPORTED;
                     UsbCallback = XhciHwSmiHandler;
                     break;

                default:
                        return EFI_UNSUPPORTED;
    }

    UsbContext.Type = UsbLegacy;
    UsbContext.Device = HcStruc->HcDevicePath;

    Status = UsbDispatch->Register(
                UsbDispatch,
                UsbCallback,
                &UsbContext,
                &Handle);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AMIUSB HC type %x HW SMI registation status:: %r\n", HcStruc->HcType, Status);
    
    if (!EFI_ERROR(Status)) {
        HcStruc->HwSmiHandle = Handle;
    }
    
    return Status;
}

/**
    Update the KbcEmul feature when the Emul6064Trap Protocol becomes available.

    @param  Protocol    Points to the protocol's unique identifier
    @param  Interface   Points to the interface instance
    @param  Handle      The handle on which the interface was installed

    @retval EFI_SUCCESS Success to initial 60/64 trap feature.
**/
EFI_STATUS
EFIAPI
Emul6064TrapCallback (
    CONST EFI_GUID  *Protocol,
    VOID            *Interface,
    EFI_HANDLE      Handle
)
{
    EFI_STATUS  Status;
    UINT32      KbcEmulFeature = 0;
    
    Status = gSmst->SmmLocateProtocol(
                 &gEmul6064TrapProtocolGuid,
                 NULL,
                 (VOID**)&gEmulationTrap);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    if (!gUsbData->KbcSupport) {
        Status = gSmst->SmmLocateProtocol(&gEmul6064MsInputProtocolGuid,
                                        NULL, (VOID**)&gMsInput);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Call Back Locate 6064Ms Status = %r\n", Status);
        Status = gSmst->SmmLocateProtocol(&gEmul6064KbdInputProtocolGuid,
                                        NULL, (VOID**)&gKbdInput);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Call Back Locate 6064Kbd Status = %r\n", Status);
        if (Status == EFI_SUCCESS) {
            gUsbData->UsbStateFlag |= USB_FLAG_6064EMULATION_ON;
            if (gEmulationTrap) {
                KbcEmulFeature = gEmulationTrap->FeatureSupported(gEmulationTrap);
            }
            if (KbcEmulFeature & IRQ_SUPPORTED) {
                gUsbData->UsbStateFlag |= USB_FLAG_6064EMULATION_IRQ_SUPPORT;
            }
        } else {
            InitSysKbc( &gKbdInput, &gMsInput );
        }
    } else {
        //
        //Init Fake Emulation interface
        //
        InitSysKbc( &gKbdInput, &gMsInput );
    }
    
    return EFI_SUCCESS;
}

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
    Status = InSmmFunction(ImageHandle, SystemTable);
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
    return EFI_UNSUPPORTED;
}

/*
    This function is install SMM handle.

    @param HcStruc   Ptr to the host controller structure
    
    @retval  EFI_SUCCESS   Success to inatall SMM hanlde.
**/

EFI_STATUS
EFIAPI
InstallSmmHandle(
    HC_STRUC*    HcStruc
)
{
    EFI_STATUS    Status = EFI_SUCCESS;
    if ((gUsbData->UsbFeature & USB_IRQ_INTERRUPT_SUPPORT) == 0){
        if((gUsbData->RegisterUsbSmiBeforeEndOfDxe && (gParameters->ApiData.HcStartStop.Start == REGISTER_USB_SMI))||
           (!gUsbData->RegisterUsbSmiBeforeEndOfDxe && (gParameters->ApiData.HcStartStop.Start == START_HOST_CONTROLLER))){
            if (!(HcStruc->HcFlag & HC_STATE_IRQ)) {
                if (!(HcStruc->HcFlag & HC_STATE_EXTERNAL)) {
                    Status = UsbInstallHwSmiHandler(HcStruc);
                }
                if (HcStruc->HwSmiHandle != NULL) {
                    USBKeyRepeat(HcStruc, 0);
                } else {
                    if(HcStruc->HcType == USB_HC_XHCI){
                        if (gUsbData->XhciEventServiceMode!=1)
                            Status = USBSB_InstallUsbIntTimerHandler();
                    }else{
                        Status = USBSB_InstallUsbIntTimerHandler();
                    }
                        
                }
            }
        }
    }
    return Status;
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
    
    @retval  EFI_SUCCESS   Success.
**/
EFI_STATUS
EFIAPI
SettingUsbSmi(
    BOOLEAN    Enable
)
{
    if ((gUsbData != NULL) && ((gUsbData->UsbFeature & USB_EXTERNAL_CONTROLLER_SUPPORT) == 1)) 
      SettingTimerSMI(Enable);
      
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
