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

/** @file UsbPort2.c
    AMI USB Porting file

**/

#include <Setup.h>
#include "Uhcd.h"
#include <Library/PcdLib.h>


extern  USB_DATA_LIST    *gUsbDataList;

//-----------------------------------------------------------------------------
// This part is linked with UHCD and located outside SMI
extern  USB_GLOBAL_DATA  *gUsbData;



/**
    This function returns a name string of connected mass storage
    device.

    @param  SkipStruc    Pointer to a skip list to be filled
    @param  TotalStruc   Total size of the data which copy to a buffer
    @param  GetSkipList  TRUE if get skip list, otherwise FALSE
    @retval EFI_SUCCESS  Success to get USB skip list

**/

EFI_STATUS
EFIAPI
UsbGetSkipList(
    USB_SKIP_LIST    *SkipStruc,
    UINT8            *TotalStruc,
    BOOLEAN          GetSkipList
)
{
    if (TotalStruc == NULL) return EFI_INVALID_PARAMETER;

    if ((*TotalStruc > gUsbData->MaxSkipListCount) && (!GetSkipList)) return EFI_OUT_OF_RESOURCES;
    if ((*TotalStruc == 0) && (!GetSkipList)) return EFI_INVALID_PARAMETER;
    if (SkipStruc == NULL) {
        if (GetSkipList) {
            *TotalStruc = gUsbData->CurrentSkipListCount;
            return EFI_OUT_OF_RESOURCES;
        } else {
            return EFI_INVALID_PARAMETER;
        }
    }

    if (!GetSkipList) {
        SetMem(gUsbDataList->UsbSkipListTable, ((*TotalStruc + 1) * sizeof(USB_SKIP_LIST)), 0);
        CopyMem(gUsbDataList->UsbSkipListTable, SkipStruc, ((*TotalStruc) * sizeof(USB_SKIP_LIST)));
        gUsbData->CurrentSkipListCount = *TotalStruc;
    } else {
        CopyMem(SkipStruc, gUsbDataList->UsbSkipListTable, gUsbData->CurrentSkipListCount * sizeof(USB_SKIP_LIST));
        *TotalStruc = gUsbData->CurrentSkipListCount;
    }
{
    UINT8   Index;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb skip device table list, the total of list is %d\n", *TotalStruc);
    for (Index = 0; Index < *TotalStruc; Index++) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "SkipType: %02x, Flag: %02x, BDF: %04x, RootPort: %02x, RoutePath: %08x, BaseClass: %02x\n",
            SkipStruc[Index].SkipType, SkipStruc[Index].Flag, SkipStruc[Index].BDF,
            SkipStruc[Index].RootPort, SkipStruc[Index].RoutePath, SkipStruc[Index].BaseClass);
    }
}

    return EFI_SUCCESS;
}

/**
    This function is called from the UHCD entry point, HcPciInfo
    can be updated here depending on the platform and/or chipset
    requirements.

    @retval EFI_STATUS Updating succeeded / failed

**/

EFI_STATUS
EFIAPI
UpdateHcPciInfo (
)
{
    return EFI_UNSUPPORTED;
}


/**
    This procedure return specific USB host controller index and
    port number for BIOS to give specific mass storage device
    have highest boot priority.

    @param  UsbHcIndx               USB host index (1-based)
    @param  UsbHubPortIndx          USB hub port index (1-based)

    @retval EFI_SUCCESS             USB boot device assigned
    @retval EFI_UNSUPPORTED         No USB boot device assigned
    @retval EFI_INVALID_PARAMETER   UsbHcIndx or UsbHubPortIndx are NULL

**/

EFI_STATUS
EFIAPI
OemGetAssignUsbBootPort(
    UINT8   *UsbHcIndx,
    UINT8   *UsbHubPortIndx
)
{
    if (UsbHcIndx == NULL || UsbHubPortIndx == NULL) {
        return EFI_INVALID_PARAMETER;
    }
/*
    // The code below is the sample implementation that reports Port#3 of HC#7
    // to be a port of boot priority device
    *UsbHcIndx = 7;
    *UsbHubPortIndx = 3;

    return EFI_SUCCESS;
*/
    return EFI_UNSUPPORTED;
}


/**
    This is porting function that fills in USB related fields in
    gSetupData variable according to the setup settings and OEM
    policy.

    @param  UsbData     Pointers to USB global data

    @retval EFI_STATUS  The status of gSetupData initialization

**/

EFI_STATUS
EFIAPI
InitUsbSetupVars (
    USB_GLOBAL_DATA         *UsbData
)
{

    UINTN               VariableSize;
    USB_MASS_DEV_NUM    MassDevNum;
    EFI_STATUS          Status;
    UINT8               Index;
    USB_MASS_DEV_VALID  MassDevValid = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT32              VariableAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
    USB_CONTROLLER_NUM  UsbControllerNum;
    EFI_GUID            EfiSetupGuid = SETUP_GUID;

    if ((gUsbData->UsbFeature & USB_SETUP_VARIABLE_RUN_ACCESS) == USB_SETUP_VARIABLE_RUN_ACCESS)
        VariableAttributes |= EFI_VARIABLE_RUNTIME_ACCESS;

    UsbControllerNum.UhciNum = 0;
    UsbControllerNum.OhciNum = 0;
    UsbControllerNum.EhciNum = 0;
    UsbControllerNum.XhciNum = 0;
    VariableSize = sizeof(UsbControllerNum);
    
    Status = gRT->SetVariable (
                      L"UsbControllerNum",
                      &EfiSetupGuid,
                      VariableAttributes,
                      VariableSize,
                      &UsbControllerNum
                      );
    if (EFI_ERROR(Status)) {
        return Status;
    }
    MassDevNum.UsbMassDevNum = 0;
    MassDevNum.IsInteractionAllowed = 1;
    VariableSize = sizeof(MassDevNum);

    Status = gRT->SetVariable (
                      L"UsbMassDevNum",
                      &EfiSetupGuid,
                      VariableAttributes,
                      VariableSize,
                      &MassDevNum
                      );
    if (EFI_ERROR(Status)) {
        return Status;
    }
    //
    // Initilize the Variable to 0
    //
    VariableSize = sizeof(MassDevValid);
    gRT->SetVariable (
           L"UsbMassDevValid",
           &EfiSetupGuid,
           VariableAttributes,
           VariableSize,
           &MassDevValid
           );

    if (UsbData == NULL) {
        return EFI_SUCCESS;
    }

    VariableSize = sizeof(USB_SUPPORT_SETUP);
    Status = gRT->GetVariable (
                      L"UsbSupport",
                      &EfiSetupGuid,
                      NULL,
                      &VariableSize,
                      gUsbDataList->UsbSetupData
                      );
    if (EFI_ERROR(Status)||(VariableSize != sizeof(USB_SUPPORT_SETUP))) {
        // Set default values and save "UsbSupport" variable.
        SetMem(gUsbDataList->UsbSetupData, sizeof(USB_SUPPORT_SETUP), 0);

        gUsbDataList->UsbSetupData->UsbMainSupport = 1;

        if ((gUsbData->UsbFeature & USB_HOTPLUG_FDD_SUPPORT) == USB_HOTPLUG_FDD_SUPPORT) {
            gUsbDataList->UsbSetupData->UsbHotplugFddSupport = SETUP_DATA_HOTPLUG_AUTO;
        }
        if ((gUsbData->UsbFeature & USB_HOTPLUG_HDD_SUPPORT) == USB_HOTPLUG_HDD_SUPPORT) {
            gUsbDataList->UsbSetupData->UsbHotplugHddSupport = SETUP_DATA_HOTPLUG_AUTO;
        }
        if ((gUsbData->UsbFeature & USB_HOTPLUG_CDROM_SUPPORT) == USB_HOTPLUG_CDROM_SUPPORT) {
            gUsbDataList->UsbSetupData->UsbHotplugCdromSupport = SETUP_DATA_HOTPLUG_AUTO;
        }

        gUsbDataList->UsbSetupData->UsbMassResetDelay = SETUP_DATA_RESETDELAY_20S;
        gUsbDataList->UsbSetupData->UsbControlTimeOut = 20;      //(EIP30079+)
        gUsbDataList->UsbSetupData->UsbXhciSupport = 1;
        gUsbDataList->UsbSetupData->UsbHiSpeedSupport = 1;
        gUsbDataList->UsbSetupData->UsbMassDriverSupport = 1;

        VariableAttributes |= EFI_VARIABLE_NON_VOLATILE;

        Status = gRT->SetVariable (
                          L"UsbSupport",
                          &EfiSetupGuid,
                          VariableAttributes,
                          sizeof(USB_SUPPORT_SETUP),
                          gUsbDataList->UsbSetupData
                          );
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    for (Index = 0; Index < MAX_USB_MASS_STORAGE; Index++) {
        UsbData->USBMassEmulationOptionTable[Index] =
        *((UINT8*)&gUsbDataList->UsbSetupData->UsbEmu1 + Index);
    }
    if (gUsbDataList->UsbSetupData->UsbLegacySupport == 1) {
        UsbData->UsbStateFlag |= USB_FLAG_DISABLE_LEGACY_SUPPORT;
    }
    UsbData->UsbReqTimeOutValue = gUsbDataList->UsbSetupData->UsbControlTimeOut * 1000 ;   //(EIP30079+)
    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT) {
        if (!gUsbDataList->UsbSetupData->UsbXhciSupport) gUsbData->UsbFeature &= (~USB_HC_XHCI_SUPPORT);
    }

    UsbData->PowerGoodDeviceDelay =
        (gUsbDataList->UsbSetupData->PowerGoodDeviceDelay == 0)? 0 : gUsbDataList->UsbSetupData->PowerGoodDeviceNumDelay;

    return EFI_SUCCESS;
}


/**
    Returns the status of "USB legacy support" question from Setup.

    @retval  Value    0 Enable
                      1 Disable
                      2 Auto

**/

UINT8
EFIAPI
UsbSetupGetLegacySupport (
)
{
    return gUsbDataList->UsbSetupData->UsbLegacySupport;
}


/**
    Updates "UsbMassDevNum" setup variable according to the number
    of installed mass storage devices.

**/

EFI_STATUS
EFIAPI
UpdateMassDevicesForSetup (
)
{
    EFI_STATUS                  Status;
    UINTN                       VariableSize;
    CONNECTED_USB_DEVICES_NUM   Devs;
    USB_MASS_DEV_NUM            SetupData;
    UINT8                       MassDevValid[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT8                       Index;
    UINT32                      VariableAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
    EFI_GUID                    EfiSetupGuid = SETUP_GUID;

    if ((gUsbData->UsbFeature & USB_SETUP_VARIABLE_RUN_ACCESS) == USB_SETUP_VARIABLE_RUN_ACCESS)
        VariableAttributes |= EFI_VARIABLE_RUNTIME_ACCESS;

    VariableSize = sizeof(SetupData);
    Status = gRT->GetVariable (
                      L"UsbMassDevNum",
                      &EfiSetupGuid,
                      NULL,
                      &VariableSize,
                      &SetupData
                      );

    if (Status == EFI_SUCCESS) {
        if (!SetupData.IsInteractionAllowed) return EFI_SUCCESS;
    }

    ReportDevices(&Devs);

    SetupData.UsbMassDevNum = Devs.NumUsbMass;

    gRT->SetVariable (
           L"UsbMassDevNum",
           &EfiSetupGuid,
           VariableAttributes,
           VariableSize,
           &SetupData
           );

    //
    // Based on avilable USB mass storage device, set the device avilable flag
    //
    for (Index = 0; Index < Devs.NumUsbMass; Index++) {
        MassDevValid[Index] = 1;
    }

    VariableSize = sizeof(USB_MASS_DEV_VALID);

    Status = gRT->SetVariable (
                      L"UsbMassDevValid",
                      &EfiSetupGuid,
                      VariableAttributes,
                      VariableSize,
                      &MassDevValid
                      );

    return Status;
}

/**
    Returns maximum device slots to be enabled and programmed
    in MaxSlotsEn field of XHCI CONFIG register. Valid range
    is 1...HCPARAMS1.MaxSlots (see xhci.h for details)

    @param  HcStruc      Pointer to HcStruc data
    @param  MaxSlots     Max slots number
    @retval EFI_SUCCESS      Valid value is reported in MaxSlots
    @retval EFI_UNSUPPORTED  Function is not ported; MaxSlots will
                             be used from HCPARAMS1 field.
    @note  Porting is optional

**/

EFI_STATUS
EFIAPI
Usb3OemGetMaxDeviceSlots (
    IN     HC_STRUC    *HcStruc,
    IN OUT UINT8       *MaxSlots
)
{
    return EFI_UNSUPPORTED;
}

/**
    Timer call-back routine that is used to execute XHCI_ProcessInterrupt
    @param  Event       Efi event occurred upon Periodic Timer
    @param  Context     Pointer to HC_STRUC

**/

VOID
EFIAPI
XhciTimerCallback (
    EFI_EVENT   Event,
    VOID        *Context
)
{
    EFI_TPL OriginalTPL;
    
    OriginalTPL = gBS->RaiseTPL (TPL_HIGH_LEVEL);

    // Execute XHCI_ProcessInterrupt using SW SMI, Context points to HC_STRUC
    UsbSmiHc(opHC_ProcessInterrupt, USB_HC_XHCI, Context);

    gBS->RestoreTPL (OriginalTPL);
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

