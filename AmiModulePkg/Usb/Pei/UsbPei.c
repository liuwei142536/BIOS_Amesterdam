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

/** @file UsbPei.c
    PEI USB initialization code

**/

#include "UsbPei.h"
#include "UsbBotPeim.h"
#include <UsbPeiElinks.h>
#include <PiPei.h>
#include <Ppi/AmiKeyCodePpi.h>
#include <Ppi/Stall.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/BootInRecoveryMode.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/IoMmu.h>

//this funciton is created from InitList.c template file during build process
typedef EFI_STATUS USB_PEI_HOST_DRIVER_INIT (
    IN EFI_FFS_FILE_HEADER *FfsHeader,
    IN EFI_PEI_SERVICES    **PeiServices );
    
extern USB_PEI_HOST_DRIVER_INIT USB_PEI_HOST_DRIVER EndOfUsbPeiHostDriverList;
USB_PEI_HOST_DRIVER_INIT* gUsbPeiHostDriver[]= {USB_PEI_HOST_DRIVER NULL};

EFI_STATUS
EFIAPI
UsbPeiEntryIoMmu (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
    IN VOID                       *InvokePpi
);

EFI_STATUS
UsbPeiEntryRun (
    IN CONST EFI_PEI_SERVICES     **PeiServices
);

EFI_GUID gPeiAmikeycodeInputPpiGuid = EFI_PEI_AMI_KEYCODE_PPI_GUID;
EFI_GUID gPeiChipUsbRecoveryInitPpiGuid = PEI_USB_CHIP_INIT_PPI_GUID;
EFI_GUID gAmiPeiUsbHostControllerPpiGuid = AMI_PEI_USB_HOST_CONTROLLER_PPI_GUID;
EFI_GUID gAmiPeiUsbIoPpiGuid = AMI_PEI_USB_IO_PPI_GUID;

// USB keyboard buffer, its head and tail pointers
EFI_PEI_AMIKEYCODE_DATA gKeys[PEI_MAX_USB_KEYS];
EFI_PEI_AMIKEYCODE_DATA *gKeysHead;
EFI_PEI_AMIKEYCODE_DATA *gKeysTail;

UINT8   gNumLockOn = 0;
UINT8   gScrlLockOn = 0;
UINT8   gCapsLockOn = 0;

EDKII_IOMMU_PPI *gEdk2IoMmuPpi   = NULL;

BOOLEAN gUsbBootInRecoveryNotifyFlag = FALSE;
BOOLEAN gUsbKeybaordBootNotifyFlag = FALSE;

EFI_PEI_NOTIFY_DESCRIPTOR gUsbPeiNotifyList = {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    NotifyOnRecoveryCapsuleLoaded};

EFI_PEI_NOTIFY_DESCRIPTOR gUsbBootInRecoveryNotifyList = {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiBootInRecoveryModePpiGuid,
    InitializeUsbHostDevice};

EFI_PEI_NOTIFY_DESCRIPTOR gUsbKeybaordBootInRecoveryNotifyList = {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiBootInRecoveryModePpiGuid,
    InitializeUsbMassDevice};

EFI_PEI_NOTIFY_DESCRIPTOR gEdkiiIoMmuPpiList = {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEdkiiIoMmuPpiGuid,
    UsbPeiEntryIoMmu
};

USB_PEI_GLOBAL_DATA    gUsbGlobalData;
USB_PEI_GLOBAL_DATA    *gUsbPeiData = &gUsbGlobalData;

/**
    This function is call to get PCD policy

    @param  None

    @retval None

**/
VOID
GetPeiUsbPcdPolicy (VOID)
{

    UINTN    Index;
    UINT8    *UsbData;
    UINT8    Count;
    PCD_PEI_INFO_INIT_TABLE PcdInfoInitTable[] = {    
    //  Token name,                                                             Memory Address                 Flag                                    PCD Tpe
    //=======================================================================================================================================================================
    // Init Usb State flag
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiUhciSupport),            &gUsbPeiData->UsbPeiFeature,   USB_UHCI_PEI_SUPPORT,                   USB_PEI_PCD_TYPE_BOOLEAN },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiOhciSupport),            &gUsbPeiData->UsbPeiFeature,   USB_OHCI_PEI_SUPPORT,                   USB_PEI_PCD_TYPE_BOOLEAN },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiEhciSupport),            &gUsbPeiData->UsbPeiFeature,   USB_EHCI_PEI_SUPPORT,                   USB_PEI_PCD_TYPE_BOOLEAN },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiXhciSupport),            &gUsbPeiData->UsbPeiFeature,   USB_XHCI_PEI_SUPPORT,                   USB_PEI_PCD_TYPE_BOOLEAN },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiKeyboardNumlock),(UINT32*)&gNumLockOn,                  USB_PEI_KB_NUMLOCK,                     USB_PEI_PCD_TYPE_BOOLEAN },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiHostErrorSupport),       &gUsbPeiData->UsbPeiFeature,   USB_HOST_ERROR_SUPPORT,                 USB_PEI_PCD_TYPE_BOOLEAN },
    
    // PcdGetEx8 
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiRecoveryReqRet),(UINT32*)&gUsbPeiData->RecoveryReqRet,  0                   ,                   USB_PEI_PCD_TYPE_UINT8   },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiRecoveryReqTimeout),(UINT32*)&gUsbPeiData->TimingPolicy.RecReqTimeout,  0   ,                   USB_PEI_PCD_TYPE_UINT8   },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiRecScanCode),(UINT32*)&gUsbPeiData->RecoveryScanCode ,  0                   ,                   USB_PEI_PCD_TYPE_UINT8   },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiDevConnectTimeout),(UINT32*)&gUsbPeiData->TimingPolicy.DevConnectTimeout,0  ,                   USB_PEI_PCD_TYPE_UINT8   },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiKbConnectDelay),(UINT32*)&gUsbPeiData->TimingPolicy.KbConnectDelay,      0  ,                   USB_PEI_PCD_TYPE_UINT8   },

    // PcdGetEx16 
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiUhciIoBase),       (UINT32*)&gUsbPeiData->UhciIoBase,   0                   ,                   USB_PEI_PCD_TYPE_UINT16  },

    // PcdGetEx32 
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiOhciIoBase),       (UINT32*)&gUsbPeiData->OhciIoBase,   0                   ,                   USB_PEI_PCD_TYPE_UINT32  },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiXhciIoBase),       (UINT32*)&gUsbPeiData->XhciIoBase,   0                   ,                   USB_PEI_PCD_TYPE_UINT32  },
    { PcdTokenEx(&gAmiUsbPkgTokenSpaceGuid,AmiPcdUsbPeiXhciMmioSize),     (UINT32*)&gUsbPeiData->XhciMmioSize, 0                   ,                   USB_PEI_PCD_TYPE_UINT32  },
    };

    // Init Usb Pei Global data
    ZeroMem (gUsbPeiData, sizeof (USB_PEI_GLOBAL_DATA));

    for (Index = 0; Index < sizeof(PcdInfoInitTable)/sizeof(PCD_PEI_INFO_INIT_TABLE); Index++) {  
        switch (PcdInfoInitTable[Index].Type){
        case USB_PEI_PCD_TYPE_BOOLEAN:
            if (LibPcdGetExBool (&gAmiUsbPkgTokenSpaceGuid, PcdInfoInitTable[Index].TokenName)){
                *(PcdInfoInitTable[Index].MemAddr) |=  PcdInfoInitTable[Index].Value;  
            }
            break;
        case USB_PEI_PCD_TYPE_UINT8:
            *(UINT8*)(PcdInfoInitTable[Index].MemAddr) = 
                LibPcdGetEx8(&gAmiUsbPkgTokenSpaceGuid, PcdInfoInitTable[Index].TokenName);
            break;
        case USB_PEI_PCD_TYPE_UINT16:
            *(UINT16*)(PcdInfoInitTable[Index].MemAddr) = 
                LibPcdGetEx16(&gAmiUsbPkgTokenSpaceGuid, PcdInfoInitTable[Index].TokenName);
            break;
        case USB_PEI_PCD_TYPE_UINT32:
            *(UINT32*)(PcdInfoInitTable[Index].MemAddr) = 
                LibPcdGetEx32(&gAmiUsbPkgTokenSpaceGuid, PcdInfoInitTable[Index].TokenName);
            break;
        case USB_PEI_PCD_TYPE_UINT64:
            *(UINT64*)(PcdInfoInitTable[Index].MemAddr) = 
                LibPcdGetEx64(&gAmiUsbPkgTokenSpaceGuid, PcdInfoInitTable[Index].TokenName);
            break;
        }
    }

    // Init USB PEI UHCI Controller table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiUhciPciDev);
    gUsbPeiData->UhciHcCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->UhciHcCount > MAX_USB_PEI_UHCI_HOST) {
      DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current UhciHcCount:[%d], Max count:[%d]\n",gUsbPeiData->UhciHcCount, MAX_USB_PEI_UHCI_HOST));
      ASSERT(gUsbPeiData->UhciHcCount <= MAX_USB_PEI_UHCI_HOST);
      gUsbPeiData->UhciHcCount = MAX_USB_PEI_UHCI_HOST;
    }
    if (gUsbPeiData->UhciHcCount) CopyMem(gUsbPeiData->UhciPciDev, UsbData, sizeof(PCI_BUS_DEV_FUNCTION)*gUsbPeiData->UhciHcCount);

    // Init USB PEI Ehci Controller table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiEhciPciDev);
    gUsbPeiData->EhciHcCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->EhciHcCount > MAX_USB_PEI_EHCI_HOST) {
      DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current EhciHcCount:[%d], Max count:[%d]\n",gUsbPeiData->EhciHcCount, MAX_USB_PEI_EHCI_HOST));
      ASSERT(gUsbPeiData->EhciHcCount <= MAX_USB_PEI_EHCI_HOST);
      gUsbPeiData->EhciHcCount = MAX_USB_PEI_EHCI_HOST;
    }
    if (gUsbPeiData->EhciHcCount) CopyMem(gUsbPeiData->EhciPciDev, UsbData, sizeof(PCI_BUS_DEV_FUNCTION)*gUsbPeiData->EhciHcCount);

    // Init USB PEI Ehci Io Resource table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiEhciIoBase);
    Count = *UsbData;
    UsbData++;
    ASSERT(Count<=MAX_USB_PEI_EHCI_HOST);
    if (Count > MAX_USB_PEI_EHCI_HOST) {
      DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current EhciMmioBaseCount:[%d], Max EHCI count:[%d]\n",Count, MAX_USB_PEI_EHCI_HOST));
      ASSERT(Count<=MAX_USB_PEI_EHCI_HOST);
      Count = MAX_USB_PEI_EHCI_HOST;
    }
    if (Count) CopyMem(gUsbPeiData->EhciIoResource, UsbData, sizeof(UINT32)*Count);

    // Init USB PEI Xhci Controller table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiXhciPciDev);
    gUsbPeiData->XhciHcCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->XhciHcCount > MAX_USB_PEI_XHCI_HOST) {
        DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current XhciHcCount:[%d], Max count:[%d]\n",gUsbPeiData->XhciHcCount, MAX_USB_PEI_XHCI_HOST));
        ASSERT(gUsbPeiData->XhciHcCount<=MAX_USB_PEI_XHCI_HOST);
        gUsbPeiData->XhciHcCount = MAX_USB_PEI_XHCI_HOST;
    }
    if (gUsbPeiData->XhciHcCount) CopyMem(gUsbPeiData->XhciPciDev, UsbData, sizeof(PCI_BUS_DEV_FUNCTION)*gUsbPeiData->XhciHcCount);

    // Init USB PEI Xhci PCI register table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiXhciPciRegVal);
    gUsbPeiData->XhciRegInitCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->XhciRegInitCount > MAX_USB_PEI_XHCI_REG_INIT_COUNT) {
        DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current XhciRegInitCount:[%d], Max count:[%d]\n",gUsbPeiData->XhciRegInitCount, MAX_USB_PEI_XHCI_REG_INIT_COUNT));
        ASSERT(gUsbPeiData->XhciRegInitCount <= MAX_USB_PEI_XHCI_REG_INIT_COUNT);
        gUsbPeiData->XhciRegInitCount = MAX_USB_PEI_XHCI_REG_INIT_COUNT;
    }
    if (gUsbPeiData->XhciRegInitCount) CopyMem(gUsbPeiData->XhciRegTbl, UsbData, sizeof(PCI_DEV_REGISTER_VALUE)*gUsbPeiData->XhciRegInitCount);

    // Init USB PEI Key Modifier bit
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiKeyModifierValue);
    gUsbPeiData->ModifierBitsCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->ModifierBitsCount > MAX_KEY_MODIIFIER) {
        DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current ModifierBitsCount:[%d], Max count:[%d]\n",gUsbPeiData->ModifierBitsCount, MAX_KEY_MODIIFIER));
        ASSERT(gUsbPeiData->ModifierBitsCount<=MAX_KEY_MODIIFIER);
        gUsbPeiData->ModifierBitsCount = MAX_KEY_MODIIFIER;
    }
    if (gUsbPeiData->ModifierBitsCount) CopyMem(gUsbPeiData->KeyModifierVal, UsbData, sizeof(UINT8)*gUsbPeiData->ModifierBitsCount);

    // Init USB PEI OHCI Controller table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiOhciPciDev);
    gUsbPeiData->OhciHcCount = *UsbData;
    UsbData++;

    if (gUsbPeiData->OhciHcCount > MAX_USB_PEI_OHCI_HOST) {
        DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current OhciHcCount:[%d], Max count:[%d]\n",gUsbPeiData->OhciHcCount, MAX_USB_PEI_OHCI_HOST));
        ASSERT(gUsbPeiData->OhciHcCount<=MAX_USB_PEI_OHCI_HOST);
        gUsbPeiData->OhciHcCount = MAX_USB_PEI_OHCI_HOST;
    }
    if (gUsbPeiData->OhciHcCount) CopyMem(gUsbPeiData->OhciPciDev, UsbData, sizeof(UINT32)*gUsbPeiData->OhciHcCount);

    // Init USB PEI Ohci PCI register table.
    UsbData = (UINT8*)PcdGetExPtr (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiOhciPciRegVal);
    gUsbPeiData->OhciRegInitCount = *UsbData;
    UsbData++;
    if (gUsbPeiData->OhciRegInitCount > MAX_USB_PEI_OHCI_REG_INIT_COUNT) {
        DEBUG((DEBUG_ERROR, "UsbPei: Reach Limit! Current OhciRegInitCount:[%d], Max count:[%d]\n",gUsbPeiData->OhciRegInitCount, MAX_USB_PEI_OHCI_REG_INIT_COUNT));
        ASSERT(gUsbPeiData->OhciRegInitCount <= MAX_USB_PEI_OHCI_REG_INIT_COUNT);
        gUsbPeiData->OhciRegInitCount = MAX_USB_PEI_OHCI_REG_INIT_COUNT;
    }
    if (gUsbPeiData->OhciRegInitCount) CopyMem(gUsbPeiData->OhciRegTbl, UsbData, sizeof(PCI_DEV_REGISTER_VALUE)*gUsbPeiData->OhciRegInitCount);
}

/**
    This function enables USB controllers on the PCI bus, programs BARs,
    starts controllers and installs EFI_PEI_USBINIT_PPI.
    @param  FileHandle             Handle of the file being invoked.
    @param  PeiServices            Describes the list of possible PEI Services.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   Usb initialization is done successfully.
                                   Otherwise, returns any type of error.

**/

EFI_STATUS
EFIAPI
UsbPeiEntryPoint (
    IN       EFI_PEI_FILE_HANDLE    FileHandle,
    IN CONST EFI_PEI_SERVICES       **PeiServices
)
{
    EFI_STATUS              Status;

    GetPeiUsbPcdPolicy();

    if (TRUE == PcdGetExBool (&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiEdkiiIommuPpiSupport)) {
        Status = (*PeiServices)->LocatePpi (
                                     PeiServices,
                                     &gEdkiiIoMmuPpiGuid,
                                     0,
                                     NULL,
                                     (VOID **)&gEdk2IoMmuPpi
                                     );
        if (EFI_ERROR(Status)) {
            return (**PeiServices).NotifyPpi(PeiServices, &gEdkiiIoMmuPpiList);
        }
    }

    return UsbPeiEntryRun(PeiServices);
}

/**
    This nitify function enables USB controllers on the PCI bus, programs BARs,
    starts controllers and installs EFI_PEI_USBINIT_PPI.
    @param  PeiServices            Describes the list of possible PEI Services.
    @param  NotifyDescriptor       Address of the notification descriptor data structure.
    @param  InvokePpi              Address of the PPI that was installed.

    @retval EFI_UNSUPPORTED        gEdkiiIoMmuPpiGuid is not located.
    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   Usb initialization is done successfully.
                                   Otherwise, returns any type of error.

**/

EFI_STATUS
EFIAPI
UsbPeiEntryIoMmu (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
    IN VOID                       *InvokePpi
)
{
    EFI_STATUS              Status;

    Status = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gEdkiiIoMmuPpiGuid,
                                 0,
                                 NULL,
                                 (VOID **)&gEdk2IoMmuPpi
                                 );
    if (EFI_ERROR(Status)) {
        return EFI_UNSUPPORTED;
    }

    Status = UsbPeiEntryRun(PeiServices);

    return Status;
}

/**
    This function enables USB controllers on the PCI bus, programs BARs,
    starts controllers and installs EFI_PEI_USBINIT_PPI.
    @param  PeiServices            Describes the list of possible PEI Services.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   Usb initialization is done successfully.
                                   Otherwise, returns any type of error.

**/

EFI_STATUS
UsbPeiEntryRun (
    IN CONST EFI_PEI_SERVICES   **PeiServices
)
{

    UINTN                       PpiInstance;
    EFI_PEI_PPI_DESCRIPTOR      *TempPpiDescriptor;
    PEI_USB_IO_PPI              *UsbIoPpi;
    EFI_STATUS                  Status;
    EFI_BOOT_MODE               BootMode;

    if (PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiKeyboardSupport)){

    if (!PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiInS3Support)){
        (**PeiServices).GetBootMode(PeiServices, &BootMode);
        if (BootMode == BOOT_ON_S3_RESUME) {
            return EFI_UNSUPPORTED;
        }
    }

    InitializeUsbHostDevice((EFI_PEI_SERVICES **)PeiServices, NULL, NULL);

    if (!PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiMassSupportPolicy)){
        (**PeiServices).NotifyPpi(PeiServices, &gUsbKeybaordBootInRecoveryNotifyList);
    } else {
        InitializeUsbMassDevice((EFI_PEI_SERVICES **)PeiServices, NULL, NULL);
    }

    for (PpiInstance = 0; PpiInstance < PEI_MAX_USB_IO_PPI; PpiInstance++) {
        Status = (**PeiServices).LocatePpi(PeiServices,
            &gAmiPeiUsbIoPpiGuid,
            PpiInstance,
            &TempPpiDescriptor,
            (VOID**)&UsbIoPpi);

        if (EFI_ERROR(Status)) {
            break;
        }

        InitUsbKbdPPI((EFI_PEI_SERVICES **)PeiServices, UsbIoPpi);
    }
    } else {  

    (**PeiServices).NotifyPpi(PeiServices, &gUsbBootInRecoveryNotifyList);

    }
    return EFI_SUCCESS;
}


/**
    Gets the keyboard data from TD if available and returns the next
    keystroke from input buffer.
    @param  PeiServices            Describes the list of possible PEI Services.
    @param  This                   Protocol instance pointer.
    @param  KeyData                Pointer to EFI_PEI_AMIKEYCODE_DATA.

    @retval EFI_NOT_READY          There was no keystroke data available.
    @retval EFI_SUCCESS            KeyData is filled with the most up-to-date keypress

**/

EFI_STATUS
EFIAPI
PeiUsbReadKey(
    IN EFI_PEI_SERVICES         **PeiServices,
    IN EFI_PEI_AMIKEYCODE_PPI   *This,
    OUT EFI_PEI_AMIKEYCODE_DATA *KeyData
)
{
    UINTN          UsbIoPpiInstance;
    EFI_PEI_PPI_DESCRIPTOR *TempPpiDescriptor;
    PEI_USB_IO_PPI *UsbIoPpi;
    PEI_USB_DEVICE *PeiUsbDev;
    static UINT8    Data[8];
    static UINTN    DataLength = 8;
    EFI_STATUS      Status;
    UINT8           *Data8 = NULL;

    if (gEdk2IoMmuPpi) {
        Status = gEdk2IoMmuPpi->AllocateBuffer (
                                    gEdk2IoMmuPpi,
                                    EfiRuntimeServicesData,
                                    1,
                                    (VOID**)&Data8,
                                    EDKII_IOMMU_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                    );
        if (EFI_ERROR(Status)) {
            return EFI_OUT_OF_RESOURCES;
        }
    } else {
        Data8 = Data;
    }
    // Check the keypress for every available USB keyboard.
    for (UsbIoPpiInstance = 0;
         UsbIoPpiInstance < PEI_MAX_USB_IO_PPI;
         UsbIoPpiInstance++)
    {
        Status = (**PeiServices).LocatePpi( PeiServices,
            &gAmiPeiUsbIoPpiGuid,
            UsbIoPpiInstance,
            &TempPpiDescriptor,
            (VOID**)&UsbIoPpi);

        if (EFI_ERROR(Status)) break;

        PeiUsbDev = PEI_USB_DEVICE_FROM_THIS(UsbIoPpi);
        if (PeiUsbDev->DeviceType != BIOS_DEV_TYPE_KEYBOARD) continue;

        Status = UsbIoPpi->UsbSyncInterruptTransfer(
            PeiServices,
            UsbIoPpi,
            PeiUsbDev->IntEndpoint,
            Data8,
            &DataLength,
            gUsbPeiData->TimingPolicy.RecReqTimeout  // timeout, ms
        );
        if (!EFI_ERROR(Status)) {
            PeiUsbLibProcessKeypress(PeiServices, UsbIoPpi, Data8);    // This will update gKeys
        }
    }
    
    if (gEdk2IoMmuPpi) {
        gEdk2IoMmuPpi->FreeBuffer (
                           gEdk2IoMmuPpi,
                           1,
                           (VOID*)Data8
                           );
    }
    if (gKeysHead == gKeysTail) return EFI_NOT_READY;

    // Get the data and adjust the tail
    *KeyData = *gKeysTail++;
    if (gKeysTail == &gKeys[PEI_MAX_USB_KEYS])
    {
        gKeysTail = gKeys;
    }

    return EFI_SUCCESS;
}

/**
  Set certain state for the input device.

  @param  PeiServices            Describes the list of possible PEI Services.
  @param  This                   Protocol instance pointer.
  @param  KeyToggleState         The EFI_KEY_TOGGLE_STATE to set the
                                  state for the input device.

  @retval EFI_SUCCESS             The device state was set appropriately.
  @retval EFI_DEVICE_ERROR        The device is not functioning correctly and could
                                  not have the setting adjusted.
  @retval EFI_UNSUPPORTED         The device does not support the ability to have its state set.

**/

EFI_STATUS
EFIAPI
PeiUsbSetLedState(
    IN  EFI_PEI_SERVICES            **PeiServices,
    IN  EFI_PEI_AMIKEYCODE_PPI      *This,
    IN  EFI_KEY_TOGGLE_STATE        KeyToggleState
)
{
    UINTN                       Index;
    EFI_STATUS                  Status;
    EFI_PEI_PPI_DESCRIPTOR      *TempPpiDescriptor;
    PEI_USB_IO_PPI              *UsbIoPpi;
    
    if (!(KeyToggleState & TOGGLE_STATE_VALID)) {
        return EFI_UNSUPPORTED;
    }

    if (KeyToggleState & SCROLL_LOCK_ACTIVE) {
        gScrlLockOn = 1;
    } else {
        gScrlLockOn = 0;
    }

    if (KeyToggleState & NUM_LOCK_ACTIVE) {
        gNumLockOn = 1;
    } else {
        gNumLockOn = 0;
    }

    if (KeyToggleState & CAPS_LOCK_ACTIVE) {
        gCapsLockOn = 1;
    } else {
        gCapsLockOn = 0;
    }

    for (Index = 0;Index < PEI_MAX_USB_IO_PPI; Index++) {
        Status = (**PeiServices).LocatePpi( PeiServices,
            &gAmiPeiUsbIoPpiGuid,
            Index,
            &TempPpiDescriptor,
            (VOID**)&UsbIoPpi);

        if (EFI_ERROR(Status)) {
            break;
        }

        PeiUsbLibLightenKbLeds(PeiServices, UsbIoPpi);
    }
    
    return EFI_SUCCESS;
}

/**
    This function gets the key code and analyzes if it matches the recovery
    request pre-defined key sequence.

    @param 
        EFI_PEI_AMIKEYCODE_DATA *KeyData - key press information

         
    @retval TRUE recovery request key combination found
    @retval FALSE recovery is not requested

**/

BOOLEAN
IsUsbKbdRecovery (
    EFI_PEI_AMIKEYCODE_DATA *KeyData
)
{
    UINT8 *ModifierBits = gUsbPeiData->KeyModifierVal;
    UINT8 Index;

    // Check for modifier match
    if (ModifierBits[0] != 0) {
        for (Index = 0; Index < gUsbPeiData->ModifierBitsCount; Index++) {
            if ((KeyData->KeyState.KeyShiftState & ModifierBits[Index]) != 0) {
                break;
            }
        }
        if ((Index > 0) && (Index == gUsbPeiData->ModifierBitsCount)) {
            return FALSE;
        }
    }

    // Check for the EFI scan code
    // Note: in future there might be a need to support UniCode, Efi key and PS2 scan codes
    if (KeyData->Key.ScanCode == gUsbPeiData->RecoveryScanCode) {
        return TRUE;
    }

    return FALSE;
}

/**
    Initialize USB keyboard input PPI.
    @param  PeiServices            Describes the list of possible PEI Services.
    @param  UsbIoPpi               PEI_USB_IO_PPI instance pointer.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   initialization of USB keyboard is done successfully.
                                   Otherwise, returns any type of error.

**/

EFI_STATUS
InitUsbKbdPPI(
    EFI_PEI_SERVICES    **PeiServices,
    PEI_USB_IO_PPI      *UsbIoPpi
)
{
    EFI_USB_ENDPOINT_DESCRIPTOR  *EndpointDescriptor;
    PEI_USB_DEVICE  *PeiUsbDev = PEI_USB_DEVICE_FROM_THIS(UsbIoPpi);
    UINT8           EpIndx;
    EFI_STATUS      Status;
    UINTN           Count;
    UINT32          UsbStatus;

    static BOOLEAN UsbKeyboardPpiInstalled = FALSE;

    static EFI_PEI_AMIKEYCODE_PPI UsbKeyboardInputPpi = {
        PeiUsbReadKey,
        PeiUsbSetLedState
    };

    static EFI_PEI_PPI_DESCRIPTOR UsbKeyboardInputPpiList = {
        (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
        &gPeiAmikeycodeInputPpiGuid,
        NULL
    };

    static EFI_PEI_AMIKEYCODE_DATA KeyData = {0};

    if (PeiUsbDev->DeviceType != BIOS_DEV_TYPE_KEYBOARD) {
        return EFI_NOT_FOUND;
    }

    DEBUG((DEBUG_INFO, "PEI USB Keyboard found.\n"));

    // Initialize IntEndpoint and IntMaxPkt fields
    for (EpIndx = 0; EpIndx < MAX_ENDPOINT; EpIndx++) {
        Status = PeiUsbGetEndpointDescriptor( PeiServices,
            UsbIoPpi, EpIndx, &EndpointDescriptor );
        if (EFI_ERROR(Status)) {
            return Status;
        }

        // See if it is Interrupt endpoint and it is IN, not OUT
        if (((EndpointDescriptor->Attributes & EP_DESC_FLAG_TYPE_BITS) == EP_DESC_FLAG_TYPE_INT) 
            && (EndpointDescriptor->EndpointAddress & EP_DESC_ADDR_DIR_BIT)) {
            PeiUsbDev->IntEndpoint = EndpointDescriptor->EndpointAddress;
            PeiUsbDev->IntMaxPkt = EndpointDescriptor->MaxPacketSize;
            break;
        }
    }

    ASSERT(EpIndx < MAX_ENDPOINT);

    if (EpIndx == MAX_ENDPOINT) {
        return EFI_NOT_FOUND;   // Interrupt endpoint is not found
    }

    // Send the HID SET_IDLE request w/ wValue = 0 (Indefinite)
    PeiUsbIoControlTransfer(PeiServices, UsbIoPpi,
            HID_RQ_SET_IDLE, 0, 0, 0, 0, &UsbStatus);

    // Send the set protocol command w/ wValue = 0 (Boot protocol)
    PeiUsbIoControlTransfer(PeiServices, UsbIoPpi,
            HID_RQ_SET_PROTOCOL, 0, 0, 0, 0, &UsbStatus);

    PeiUsbLibLightenKbLeds(PeiServices, UsbIoPpi);

    // Attach periodic schedule
    PeiUsbDev->UsbHcPpi->ActivatePolling(PeiServices, (VOID*)PeiUsbDev);

    // Install input PPI, only once
    if (UsbKeyboardPpiInstalled == FALSE) {
        UsbKeyboardInputPpiList.Ppi = &UsbKeyboardInputPpi;
        Status = (**PeiServices).InstallPpi(PeiServices, &UsbKeyboardInputPpiList);
    
        ASSERT(Status == EFI_SUCCESS);
        if (!EFI_ERROR(Status)) {
            UsbKeyboardPpiInstalled = TRUE;
        }
    }

    for (Count = 0; Count < gUsbPeiData->RecoveryReqRet; Count++) {
        // See if recovery request is active
        Status = PeiUsbReadKey(PeiServices, &UsbKeyboardInputPpi, &KeyData);
    
        if (!EFI_ERROR(Status)) {
            DEBUG((DEBUG_INFO, " key: sc %x, uc %x, shft %x, tggl %x, efi %x, ps2 %x, Count %d\n",
                KeyData.Key.ScanCode, KeyData.Key.UnicodeChar,
                KeyData.KeyState.KeyShiftState, KeyData.KeyState.KeyToggleState,
                KeyData.EfiKey, KeyData.PS2ScanCode, Count));
        }

        if (IsUsbKbdRecovery(&KeyData)) {
            static EFI_PEI_PPI_DESCRIPTOR RecoveryModePpi = {
                EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
                &gEfiPeiBootInRecoveryModePpiGuid, NULL
            };
    
            (*PeiServices)->SetBootMode(PeiServices, BOOT_IN_RECOVERY_MODE);
            (*PeiServices)->InstallPpi(PeiServices, &RecoveryModePpi);

            break;
        }
    }

    return EFI_SUCCESS;
}

/**
    This routine initializes usb mass devices.

    @param  PeiServices            Describes the list of possible PEI Services.
    @param  NotifyDescriptor       Address of the notification descriptor data structure.
    @param  InvokePpi              Address of the PPI that was installed.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   initialization of Usb Mass Device is done successfully.
                                   Otherwise, returns any type of error.
**/

EFI_STATUS
EFIAPI
InitializeUsbMassDevice(
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN VOID                      *InvokePpi
)
{
    EFI_STATUS                  Status;
    EFI_PEI_PPI_DESCRIPTOR      *TempPpiDescriptor;
    PEI_USB_IO_PPI              *UsbIoPpi;
    UINTN                       PpiInstance;
    EFI_PEI_STALL_PPI           *StallPpi;
    UINT8                       DevConnectTimeout = gUsbPeiData->TimingPolicy.DevConnectTimeout * 2 + 1;
    PEI_USB_DEVICE              *PeiUsbDev;
    PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi;

    if (gUsbKeybaordBootNotifyFlag == TRUE) {
        return EFI_SUCCESS;
    }

    gUsbKeybaordBootNotifyFlag = TRUE;
    
    Status = (**PeiServices).LocatePpi( PeiServices, &gEfiPeiStallPpiGuid,
                    0, NULL, (VOID**)&StallPpi);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    //Some devices may connect lately, polling process to wait for usb devices.
    for (;;) {
        PeimInitializeUsb(NULL, PeiServices); 
        for (PpiInstance = 0; PpiInstance < PEI_FAT_MAX_USB_IO_PPI; PpiInstance++) {
            Status = (**PeiServices).LocatePpi( PeiServices,
                                &gAmiPeiUsbIoPpiGuid,
                                PpiInstance,
                                &TempPpiDescriptor,
                                (VOID**)&UsbIoPpi);
            if (EFI_ERROR(Status)) {
                break;
            }
            
            PeiUsbDev = PEI_USB_DEVICE_FROM_THIS(UsbIoPpi);
            UsbHcPpi = PeiUsbDev->UsbHcPpi;
            
            if (PeiUsbDev->InterfaceDesc->InterfaceClass == BASE_CLASS_HUB) {
                PeiHubEnumeration(PeiServices, PeiUsbDev, 
                                 &UsbHcPpi->CurrentAddress, TRUE);
            }
        }
        
        DevConnectTimeout--;
        
        if (DevConnectTimeout == 0) {
            break;
        }
        
        StallPpi->Stall(PeiServices, StallPpi, 500 * 1000);
    }
    
    for (PpiInstance = 0; PpiInstance < PEI_MAX_USB_IO_PPI; PpiInstance++) {
        Status = (**PeiServices).LocatePpi(PeiServices,
            &gAmiPeiUsbIoPpiGuid,
            PpiInstance,
            &TempPpiDescriptor,
            (VOID**)&UsbIoPpi);

        if (EFI_ERROR(Status)) {
            break;
        }
        InitUsbBot(PeiServices, UsbIoPpi);
    }

    return EFI_SUCCESS;
}

/**
    This routine initializes usb host controllers and devices.

    @param  PeiServices            Describes the list of possible PEI Services.
    @param  NotifyDescriptor       Address of the notification descriptor data structure.
    @param  InvokePpi              Address of the PPI that was installed.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   initialization of Usb host controllers is done successfully.
                                   Otherwise, returns any type of error.
**/

EFI_STATUS
EFIAPI
InitializeUsbHostDevice(
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN VOID                      *InvokePpi
)
{
    EFI_STATUS                  Status;
    UINTN                       PpiInstance;
    PEI_USB_CHIP_INIT_PPI       *UsbChipsetRecoveryInitPpi;
    EFI_PEI_STALL_PPI           *StallPpi = NULL;
    PEI_USB_IO_PPI              *UsbIoPpi = NULL;
    PEI_USB_DEVICE              *PeiUsbDev = NULL;
    EFI_PEI_PPI_DESCRIPTOR      *TempPpiDescriptor = NULL;
    PEI_USB_HOST_CONTROLLER_PPI *UsbHcPpi = NULL;
    UINT8                       Index;

    if (gUsbBootInRecoveryNotifyFlag == TRUE) {
        return EFI_SUCCESS;
    }

    gUsbBootInRecoveryNotifyFlag = TRUE;

    for (PpiInstance = 0; PpiInstance < PEI_MAX_USB_RECOVERY_INIT_PPI; PpiInstance++) {
        Status = (**PeiServices).LocatePpi( PeiServices, &gPeiChipUsbRecoveryInitPpiGuid,
            PpiInstance, NULL, (VOID**)&UsbChipsetRecoveryInitPpi);
        if (EFI_ERROR(Status)) {
            break;
        }

        UsbChipsetRecoveryInitPpi->EnableChipUsbRecovery(PeiServices);
    }

    // Init USB controller
    for (Index = 0; gUsbPeiHostDriver[Index]; Index++) {
        gUsbPeiHostDriver[Index](0, PeiServices);
    }

    if (PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiKeyboardSupport)){
    if (gUsbPeiData->TimingPolicy.KbConnectDelay != 0) {
        Status = (**PeiServices).LocatePpi( PeiServices, &gEfiPeiStallPpiGuid,
                        0, NULL, (VOID**)&StallPpi);
        if (!EFI_ERROR(Status)) {
            StallPpi->Stall(PeiServices, StallPpi, gUsbPeiData->TimingPolicy.KbConnectDelay * 1000);
        }
    }
    }
    
    PeimInitializeUsb(NULL, PeiServices);
    
    // Initialize head and tail of the input buffer
    if (PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiKeyboardSupport)){
    gKeysHead = gKeys;
    gKeysTail = gKeys;
    PeimInitializeUsb(NULL, PeiServices); 
    for (PpiInstance = 0; PpiInstance < PEI_FAT_MAX_USB_IO_PPI; PpiInstance++) {
        Status = (**PeiServices).LocatePpi( PeiServices,
                            &gAmiPeiUsbIoPpiGuid,
                            PpiInstance,
                            &TempPpiDescriptor,
                            (VOID**)&UsbIoPpi);
        if (EFI_ERROR(Status)) {
            break;
        }
            
        PeiUsbDev = PEI_USB_DEVICE_FROM_THIS(UsbIoPpi);
        UsbHcPpi = PeiUsbDev->UsbHcPpi;
            
        if (PeiUsbDev->InterfaceDesc->InterfaceClass == BASE_CLASS_HUB) {
            PeiHubEnumeration(PeiServices, PeiUsbDev, 
                                 &UsbHcPpi->CurrentAddress, TRUE);
        }
    }
    }
    
    if (!PcdGetExBool(&gAmiUsbPkgTokenSpaceGuid, AmiPcdUsbPeiKeyboardSupport)){
        InitializeUsbMassDevice(PeiServices, NULL, NULL);
    }


    (**PeiServices).NotifyPpi(PeiServices, &gUsbPeiNotifyList);

    return EFI_SUCCESS;
}

/**
    This routine halts all available host controllers at end of PEI

    @param  PeiServices            Describes the list of possible PEI Services.
    @param  NotifyDescriptor       Address of the notification descriptor data structure.
    @param  InvokePpi              Address of the PPI that was installed.

    @retval EFI_STATUS             This function returns EFI_SUCCESS if the
                                   host controller is reset successfully. Otherwise, returns
                                   any type of error it encountered during the reset operation.

**/

EFI_STATUS
EFIAPI
NotifyOnRecoveryCapsuleLoaded(
    IN EFI_PEI_SERVICES          **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
    IN VOID                      *InvokePpi
)
{
    EFI_STATUS                      Status;
    UINT8                           ControllerIndex = 0;
    PEI_USB_HOST_CONTROLLER_PPI     *UsbHcPpi;
    UINT8                           NumOfRootPort;
    UINT8                           PortNum;

    while (TRUE) {
        
        Status = (**PeiServices).LocatePpi (
                    PeiServices,
                    &gAmiPeiUsbHostControllerPpiGuid,
                    ControllerIndex++,
                    NULL,
                    (VOID**)&UsbHcPpi
                    );

        if (EFI_ERROR (Status)) break;

        UsbHcPpi->GetRootHubPortNumber(
                    PeiServices,
                    UsbHcPpi,
                    &NumOfRootPort
                    );
        for (PortNum = 1; PortNum <= NumOfRootPort; PortNum++) {
            UsbHcPpi->ClearRootHubPortFeature(
                        PeiServices,
                        UsbHcPpi,
                        PortNum,
                        EfiUsbPortEnable
                        );
        }

        if (UsbHcPpi->Reset != NULL ){
            Status = UsbHcPpi->Reset (
                                 PeiServices, 
                                 UsbHcPpi, 
                                 EFI_USB_HC_RESET_GLOBAL
                                 );
            if (EFI_ERROR (Status)) {
                DEBUG((DEBUG_INFO, "PEI USB Host Controller Ppi Reset Status %r\n", Status));
            }
        }
    }    

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
