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

/** @file UsbSb.c
    USB South Bridge Porting Hooks

**/

#include <AmiUsb.h>
#include <UsbKbd.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmUsbDispatch2.h>


#include <Library/AmiUsbSmmGlobalDataValidationLib.h>
#include <Library/DxeSmmUsbSbLib.h>
#include <Library/AmiUsbHcdLib.h>


#define AMI_ACPI_EN_DISPATCH_PROTOCOL_GUID \
        { 0xbd88ec68, 0xebe4, 0x4f7b, 0x93, 0x5a, 0x4f, 0x66, 0x66, 0x42, 0xe7, 0x5f }
EFI_GUID gAmiAcpiEnDispatchProtocolGuid = AMI_ACPI_EN_DISPATCH_PROTOCOL_GUID;

typedef VOID (EFIAPI *AMI_ACPI_DISPATCH) (
    IN EFI_HANDLE           DispatchHandle
);
typedef struct _AMI_ACPI_DISPATCH_PROTOCOL AMI_ACPI_DISPATCH_PROTOCOL;
typedef EFI_STATUS (EFIAPI *EFI_ACPI_REGISTER) (
    IN AMI_ACPI_DISPATCH_PROTOCOL   *This,
    IN AMI_ACPI_DISPATCH            DispatchFunction,
    OUT EFI_HANDLE                  *DispatchHandle
);

typedef EFI_STATUS (EFIAPI *EFI_ACPI_UNREGISTER) (
    IN AMI_ACPI_DISPATCH_PROTOCOL   *This,
    IN EFI_HANDLE                   DispatchHandle
);
struct _AMI_ACPI_DISPATCH_PROTOCOL {
    EFI_ACPI_REGISTER       Register;
    EFI_ACPI_UNREGISTER     UnRegister;
};


EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL *gPeriodicTimerDispatch = NULL;

EFI_HANDLE  gPeriodicTimerHandle = NULL;
EFI_HANDLE  gUsbIntTimerHandle = NULL;
BOOLEAN gInstallXhciHwSmiHandler = FALSE;
BOOLEAN gInstallUsbIntTimerHandler = FALSE;

extern USB_GLOBAL_DATA     *gUsbData;
extern HC_STRUC            **gHcTable;
extern BOOLEAN             gLockSmiHandler;
extern BOOLEAN             gLockHwSmiHandler;
extern USB_DATA_LIST       *gUsbDataList;
extern UINT8               gDataInSmm;

EFI_STATUS EFIAPI SettingTimerSMI(IN BOOLEAN  EnableSmi);
EFI_STATUS EFIAPI GetSmiSource(IN UINT8 *SmiSource);

EFI_STATUS
EFIAPI
UhciHWSMIHandler (
    IN EFI_HANDLE   DispatchHandle,
    IN CONST VOID   *Context OPTIONAL,
    IN OUT VOID     *CommBuffer OPTIONAL,
    IN OUT UINTN    *CommBufferSize OPTIONAL
    );

EFI_STATUS
EFIAPI
OhciHWSMIHandler (
    IN EFI_HANDLE   DispatchHandle,
    IN CONST VOID   *Context OPTIONAL,
    IN OUT VOID     *CommBuffer OPTIONAL,
    IN OUT UINTN    *CommBufferSize OPTIONAL
    );

EFI_STATUS
EFIAPI
EhciHWSMIHandler (
    IN EFI_HANDLE   DispatchHandle,
    IN CONST VOID   *Context OPTIONAL,
    IN OUT VOID     *CommBuffer OPTIONAL,
    IN OUT UINTN    *CommBufferSize OPTIONAL
    );

EFI_STATUS
EFIAPI
XhciHwSmiHandler (
    IN EFI_HANDLE   DispatchHandle,
    IN CONST VOID   *Context OPTIONAL,
    IN OUT VOID     *CommBuffer OPTIONAL,
    IN OUT UINTN    *CommBufferSize OPTIONAL
    );

/**
    This function is registers periodic timer callbacks.

    @param  DispatchHandle  Not used
    @param  Context         Not used
    @param  CommBuffer      Pointer to Buffer
    @param  CommBufferSize  Pointer to Buffer Size

    @retval  EFI_SUCCESS If timers are initialized or function is not implemented
                         timer initialization error

    @note    If function is not implemented (timers are not needed for this chipset),
             function must return EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
USBSB_PeriodicTimerCallBack (
    EFI_HANDLE  DispatchHandle,
    CONST VOID  *Context,
    VOID        *CommBuffer,
    UINTN       *CommBufferSize
)
{
    HC_STRUC        *HcStruc;
    UINT8           Index;
    DEV_INFO        *DevInfo = NULL;
    DEV_INFO        *Dev = gUsbDataList->DevInfoTable;
    EFI_STATUS      Status;

    if ((gUsbData->UsbRuntimeDriverInSmm == USB_RUNTIME_DRV_MODE_2) && 
        (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI)) {
        return EFI_SUCCESS;
    }

    if (gLockSmiHandler == TRUE) {
        return EFI_SUCCESS;
    }

    Status = AmiUsbSmmGlobalDataValidation((USB_GLOBAL_DATA*)&gUsbData, (USB_DATA_LIST*)&gUsbDataList, &gDataInSmm);

    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status)) {
        gLockHwSmiHandler = TRUE;
        gLockSmiHandler = TRUE;
        return EFI_SUCCESS;
    }

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if (HcStruc->HwSmiHandle != NULL) {
            continue;
        }
        if (HcStruc->HcFlag & HC_STATE_RUNNING) {
            AmiUsbProcessInterrupt(HcStruc);
        }
    }

    if ((gUsbData->UsbFeature & USB_HID_USE_SETIDLE) == 0){
        if (gUsbData->KeyRepeatHcIndex != 0) {
            return EFI_SUCCESS;
        }

        for (Index = 0; Index < gUsbData->MaxHidCount; Index++) {
            DevInfo = gUsbDataList->UsbKbDeviceTable[Index];
            if (DevInfo != NULL) {
                break;
            }
        }
        if (DevInfo == NULL) {
            for (Index = 0; Index < gUsbData->MaxDevCount; ++Index, ++Dev) {
                if ((Dev->Flag & DEV_INFO_VALID_STRUC) != 0 &&
                    Dev->DeviceType == BIOS_DEV_TYPE_HID &&
                    (Dev->HidDevType & HID_DEV_TYPE_MOUSE)) {
                    DevInfo = Dev;
                    break; 
                }
            }
        }

        if (DevInfo != NULL) {
            USBKBDPeriodicInterruptHandler(gHcTable[DevInfo->HcNumber - 1]);
        }
    }
    return EFI_SUCCESS;
}

/**
    This function registers XHCI hardware SMI callback function.
    
    @retval EFI_STATUS Status of the operation
    @note   Currently EHCI, UHCI and OHCI drivers install their SMI handlers in the
            corresponding Start functions. In the future all code related to SMI
            registration can be moved here.

**/

EFI_STATUS
EFIAPI
USBSB_InstallXhciHwSmiHandler (
)
{
    EFI_STATUS                        Status = EFI_SUCCESS;
    EFI_HANDLE                        Handle = NULL;
    //GPI service
    EFI_SMM_GPI_DISPATCH2_PROTOCOL    *GpiDispatch = NULL;
    EFI_SMM_GPI_REGISTER_CONTEXT      Context;
    UINT8                             Index;

    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == 0)
        return Status;
    
    if (gInstallXhciHwSmiHandler) {
        return Status;
    }

    gInstallXhciHwSmiHandler = TRUE;
    
    if (gUsbData->XhciEventServiceMode > 0) {
        Status = gSmst->SmmLocateProtocol(&gEfiSmmGpiDispatch2ProtocolGuid, NULL, (VOID**)&GpiDispatch);

        if (!EFI_ERROR(Status)) {
            for (Index = 0; Index < MAX_SMI_PIN_TABLE_COUNT; Index++) {
                if (gUsbData->HwSmiPinTable[Index] == 0xFFFFFFFF) continue;
                if ((gUsbData->UsbFeature & USB_GPI_DISPATCH_BY_BITMAP) == 0) {
                    Context.GpiNum = gUsbData->HwSmiPinTable[Index];
                } else {
                    Context.GpiNum = LShiftU64 (1, gUsbData->HwSmiPinTable[Index]);
                }
                GpiDispatch->Register(GpiDispatch, XhciHwSmiHandler, &Context, &Handle);
            }
        }
    }

    return Status;
}



/**
    This function registers periodic timer callback function.
    @retval EFI_STATUS Status of the operation
**/

EFI_STATUS
EFIAPI
USBSB_InstallUsbIntTimerHandler (
)
{
    EFI_STATUS  Status;
    EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT  TimerContext;
    UINT64      *SmiTickInterval;
    UINT8       DisableTimerSmi = FALSE;
    UINT8       SmiSource;

    if (gInstallUsbIntTimerHandler) {
        return EFI_SUCCESS;
    }

    gInstallUsbIntTimerHandler = TRUE;

    Status = gSmst->SmmLocateProtocol (
                      &gEfiSmmPeriodicTimerDispatch2ProtocolGuid,
                      NULL,
                      (VOID**)&gPeriodicTimerDispatch
                      );

    if (!EFI_ERROR(Status)) {
        
        TimerContext.Period = PERIODIC_TIMER_SMI_16_MS;   //16Ms 
        TimerContext.SmiTickInterval = PERIODIC_TIMER_SMI_16_MS; 
        SmiTickInterval = NULL;
        
        Status = GetSmiSource(&SmiSource);
        if (!EFI_ERROR(Status)){
            if (!(SmiSource&PERIODIC_TIMER_SMI_SOURCE_IS_ENABLE)) DisableTimerSmi = TRUE;
        }        
        //Check SmiTickInterval that are supported by the chipset.
        do {
            Status = gPeriodicTimerDispatch->GetNextShorterInterval (
                                               gPeriodicTimerDispatch,
                                               &SmiTickInterval
                                               );
            if (EFI_ERROR(Status)) {
                break;
            }
            if (SmiTickInterval != NULL) {
                if (*SmiTickInterval <= TimerContext.SmiTickInterval) {
                    TimerContext.SmiTickInterval = *SmiTickInterval;
                    break;
                }
            }
        } while (SmiTickInterval != NULL);
        
        gPeriodicTimerDispatch->Register (
                                  gPeriodicTimerDispatch,
                                  USBSB_PeriodicTimerCallBack,
                                  &TimerContext,
                                  &gUsbIntTimerHandle
                                  );
                      
        // Disable Timer SMI if only USB Timer SMI event. 
        // will enable timer SMI before enter legacy.
        if (DisableTimerSmi) SettingTimerSMI(FALSE);
    }

    return Status;
}

/**
    This function unregisters all the periodic timer handles.
    @retval EFI_STATUS Status of the operation
**/

EFI_STATUS
EFIAPI
USBSB_UninstallTimerHandlers (
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    
    if (gPeriodicTimerDispatch == NULL) {
        return Status;
    }
    
    if (gUsbIntTimerHandle) {
        Status = gPeriodicTimerDispatch->UnRegister (
                     gPeriodicTimerDispatch, 
                     gUsbIntTimerHandle);
        ASSERT_EFI_ERROR(Status);
    
        gUsbIntTimerHandle = NULL;
    }
    
    if (gPeriodicTimerHandle) {
        Status = gPeriodicTimerDispatch->UnRegister (
                     gPeriodicTimerDispatch, 
                     gPeriodicTimerHandle);
        ASSERT_EFI_ERROR(Status);
    
        gPeriodicTimerHandle = NULL;
    }
    
    return Status;
}

/**
    This is ACPI mode enable callback function. It is a workaround for non 
    XHCI/EHCI aware OSes.
    @param  DispatchHandle  Not used

**/

VOID
EFIAPI
AcpiEnableCallBack(
    IN EFI_HANDLE   DispatchHandle
)
{
    USB_StopUnsupportedHc();
    gUsbData->UsbStateFlag |= USB_FLAG_RUNNING_UNDER_OS;
    gLockSmiHandler = TRUE;
    gUsbData->KbShiftKeyStatusUnderOs = gUsbData->UsbKbShiftKeyStatus;
    SettingTimerSMI(FALSE); 
}

/**
    This function registers ACPI enable callback function.
    @param  Protocol     Not used
    @param  Interface    Not used
    @param  Handle       Not used
    @retval EFI_STATUS   Status of the operation

**/
EFI_STATUS
EFIAPI
RegisterAcpiEnableCallBack (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      RegisterHandle;
    AMI_ACPI_DISPATCH_PROTOCOL      *AcpiEnDispatch;

    Status = gSmst->SmmLocateProtocol(&gAmiAcpiEnDispatchProtocolGuid, NULL, (VOID**)&AcpiEnDispatch);
    if (EFI_ERROR(Status)) {
        return Status;
    }
    RegisterHandle = (EFI_HANDLE)USB_HANDLE;
    Status = AcpiEnDispatch->Register(AcpiEnDispatch, AcpiEnableCallBack, &RegisterHandle);

    return Status;
}

/**
    This function is called from USBRT entry point inside SMM. Any SMI handlers
    registration related to USB driver can be done here.
    @retval EFI_STATUS Status of the operation
**/

EFI_STATUS
EFIAPI
USBSB_InstallSmiEventHandlers (
)
{
    EFI_STATUS                         Status;
    EFI_SMM_USB_REGISTER_CONTEXT       UsbContext;
    EFI_SMM_USB_DISPATCH2_PROTOCOL     *UsbDispatch;
    EFI_SMM_HANDLER_ENTRY_POINT2       UsbCallback = NULL;
    EFI_HANDLE                         Handle = NULL;
    EFI_USB_POLICY_PROTOCOL            *EfiUsbPolicyProtocol;
    UINT8                              Index;
    AMI_USB_HW_SMI_HC_CONTEXT          *HcContext;
    
    if (((gUsbData->UsbFeature & USB_EXTERNAL_CONTROLLER_SUPPORT) == USB_EXTERNAL_CONTROLLER_SUPPORT) &&
        ((gUsbData->UsbFeature & USB_REG_PERIODIC_TIMER_IN_DXE) == USB_REG_PERIODIC_TIMER_IN_DXE))
        USBSB_InstallUsbIntTimerHandler();

    Status = gSmst->SmmLocateProtocol(
                        &gEfiSmmUsbDispatch2ProtocolGuid,
                        NULL,
                        (VOID**)&UsbDispatch
                        );

    if (!EFI_ERROR(Status)) {
        Status = gBS->LocateProtocol(
                          &gEfiUsbPolicyProtocolGuid,
                          NULL,
                          (VOID**)&EfiUsbPolicyProtocol
                          );

        if (!EFI_ERROR(Status)) {
            gUsbData->RegisterUsbSmiBeforeEndOfDxe = EfiUsbPolicyProtocol->RegisterUsbSmiBeforeEndOfDxe;
            for (Index = 0; Index < EfiUsbPolicyProtocol->AmiUsbHwSmiHcTable.HcCount; Index++) {

                HcContext = EfiUsbPolicyProtocol->AmiUsbHwSmiHcTable.HcContext[Index];
                switch (HcContext->Type) {
                        case USB_HC_UHCI:
                             if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == USB_HC_UHCI_SUPPORT)
                                UsbCallback = UhciHWSMIHandler;
                             break;
                        case USB_HC_OHCI:
                             if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT)
                                UsbCallback = OhciHWSMIHandler;
                             break;
                        case USB_HC_EHCI:
                             if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT)
                                UsbCallback = EhciHWSMIHandler;
                             break;
                        case USB_HC_XHCI:
                             if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT)
                                UsbCallback = XhciHwSmiHandler;
                             break;
                        default:
                             UsbCallback = NULL;
                             break;
                }

                if (UsbCallback == NULL) {
                    continue;
                }

                UsbContext.Type = UsbLegacy;
                UsbContext.Device = HcContext->Device;

                Status = UsbDispatch->Register(
                            UsbDispatch,
                            UsbCallback,
                            &UsbContext,
                            &Handle);

                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "AMIUSB HC type %x HW SMI registation status:: %r\n", 
                            HcContext->Type, Status);
                if (!EFI_ERROR(Status)) {
                    HcContext->HwSmiHandle = Handle;
                }
            }

        }
    }
    USBSB_InstallXhciHwSmiHandler();
    {
        AMI_ACPI_DISPATCH_PROTOCOL      *AcpiEnDispatch;
        VOID                            *Reg;

        Status = gSmst->SmmLocateProtocol(&gAmiAcpiEnDispatchProtocolGuid, NULL, (VOID**)&AcpiEnDispatch);
        if (!EFI_ERROR(Status)) {
            RegisterAcpiEnableCallBack(NULL, NULL, NULL);
        } else {
            Status = gSmst->SmmRegisterProtocolNotify (
                                &gAmiAcpiEnDispatchProtocolGuid,
                                RegisterAcpiEnableCallBack,
                                &Reg
                                );
            ASSERT_EFI_ERROR(Status);
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
