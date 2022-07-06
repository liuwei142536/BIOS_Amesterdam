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

/** @file AmiUsb.c
    AMI USB API implementation. The following code will be
    copied to SMM; only RT functions can be used. gUsbData
    is obtained from AMIUHCD in the entry point and can be
    used afterwards.

**/
#include "AmiDef.h"
#include "AmiUsb.h"
#include <UsbDevDriverElinks.h>
#include <Library/TimerLib.h>
#include "UsbKbd.h"
#include "UsbMass.h"
#include <Protocol/AmiUsbHid.h>
#include <Library/AmiUsbHcdLib.h>

EFI_EMUL6064MSINPUT_PROTOCOL* gMsInput = 0;
EFI_EMUL6064KBDINPUT_PROTOCOL* gKbdInput = 0;
EFI_EMUL6064TRAP_PROTOCOL* gEmulationTrap = 0;

USB_GLOBAL_DATA     *gUsbData;
BOOLEAN gLockSmiHandler = FALSE;
BOOLEAN gLockHwSmiHandler = FALSE;
BOOLEAN gCheckUsbApiParameter = FALSE;

VOID    StopControllerType(UINT8);
VOID    StartControllerType(UINT8);
UINT8   USB_StopDevice (HC_STRUC*,  UINT8, UINT8);
UINT8   USB_EnumerateRootHubPorts(UINT8);
VOID    StopControllerBdf(UINT16);

VOID FillHcdEntries();

typedef VOID USB_DEV_DELAYED_DRIVER_CHECK (DEV_DRIVER*);
extern USB_DEV_DELAYED_DRIVER_CHECK USB_DEV_DELAYED_DRIVER EndOfUsbDevDelayedDriverList;
USB_DEV_DELAYED_DRIVER_CHECK* UsbDevDelayedDrivers[]= {USB_DEV_DELAYED_DRIVER NULL};

typedef VOID USB_DEV_DRIVER_CHECK (DEV_DRIVER*);
extern USB_DEV_DRIVER_CHECK USB_DEV_DRIVER EndOfUsbDevDriverList;
USB_DEV_DRIVER_CHECK* UsbDevDrivers[]= {USB_DEV_DRIVER NULL};

extern  UINT8 EFIAPI UsbFillHcdEntriesDummy(HCD_HEADER*);
extern UINT8 UHCI_FillHCDEntries(HCD_HEADER*);
extern UINT8 OHCI_FillHCDEntries(HCD_HEADER*);
extern UINT8 EHCI_FillHCDEntries(HCD_HEADER*);
extern UINT8 XHCI_FillHCDEntries(HCD_HEADER*);

HCD_HEADER      gUsbHcdTable[MAX_HC_TYPES];
DEV_DRIVER      gUsbDevDriverTable[MAX_DEVICE_TYPES];
DEV_DRIVER      gUsbDelayedDrivers[MAX_DEVICE_TYPES]; 
HCD_MEM_HEADER  gUsbHcdMemTable;

URP_STRUC       *gParameters = NULL;
USB_DATA_LIST   *gUsbDataList = NULL;
HC_STRUC        **gHcTable = NULL;
UINT8           gDataInSmm = FALSE;


/**
    This routine allocates blocks of memory from the global
    memory pool

    @param Ptr       Pointer to the memory block to be freed
    @param NumBlk    Number of 32 byte blocks needed

    @retval EFI_SUCCESS             On success
    @retval EFI_OUT_OF_RESOURCES    Failure

    @note  This routine allocates continuous 32 byte memory blocks.

**/

EFI_STATUS
EFIAPI
UsbHcdMemAlloc(
    VOID _FAR_ **Ptr,
    UINT16     NumBlk
)
{
    *Ptr = USB_MemAlloc(NumBlk);
    if (*Ptr == NULL) return  EFI_OUT_OF_RESOURCES;
    return  EFI_SUCCESS;
}


/**
    This routine frees the chunk of memory allocated using
    the USBMem_Alloc call

    @param Ptr       Pointer to the memory block to be freed
    @param NumBlk    Number of 32 byte blocks to be freed

    @retval EFI_SUCCESS      On success
    @retval EFI_NOT_FOUND    Failure

    @note  This routine frees continuous memory blocks starting from the Ptr.

**/

EFI_STATUS
EFIAPI
UsbHcdMemFree (
    VOID _FAR_ *Ptr,
    UINT16     NumBlk
)
{
    if (USB_MemFree(Ptr, NumBlk) != USB_SUCCESS) {
        return  EFI_NOT_FOUND;
    }
    return  EFI_SUCCESS;
}

/**
    Call corresponding function for filling the host controller driver routine pointers with feature flag,
    and install SMM USB Hcd protocol.

    @param   None.
    @retval  None.

**/
VOID
FillHcdEntries(
  VOID
  )
{ 
    //Check UHCI Support flag
    if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == USB_HC_UHCI_SUPPORT){
        UHCI_FillHCDEntries(&gUsbHcdTable[USB_INDEX_UHCI]);
    } else {
        UsbFillHcdEntriesDummy(&gUsbHcdTable[USB_INDEX_UHCI]);
    }
    
    //Check OHCI Support flag
    if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT){
        OHCI_FillHCDEntries(&gUsbHcdTable[USB_INDEX_OHCI]);
    } else {
        UsbFillHcdEntriesDummy(&gUsbHcdTable[USB_INDEX_OHCI]);
    }
    
    //Check EHCI Support flag
    if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT){
        EHCI_FillHCDEntries(&gUsbHcdTable[USB_INDEX_EHCI]);
    } else {
        UsbFillHcdEntriesDummy(&gUsbHcdTable[USB_INDEX_EHCI]);
    }
    
    //Check XHCI Support flag
    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT){
        XHCI_FillHCDEntries(&gUsbHcdTable[USB_INDEX_XHCI]);

    } else {
        UsbFillHcdEntriesDummy(&gUsbHcdTable[USB_INDEX_XHCI]);
    }
    
    gUsbHcdMemTable.FnHcdMemAlloc = UsbHcdMemAlloc;
    gUsbHcdMemTable.FnHcdMemFree  = UsbHcdMemFree;

    // Install SMM USB Hcd protocol
    InitHcdTable((HCD_HEADER*)&gUsbHcdTable[0],
                 (VOID*)&gUsbData,
                 (VOID*)&gHcTable,
                 (HCD_MEM_HEADER*)&gUsbHcdMemTable
                 );
}


/**
 Type:  Function Dispatch Table
 This is the table of functions used by USB API
**/
API_FUNC gUsbApiTable[] = {
    USBAPI_CheckPresence,
    USBAPI_Start,
    USBAPI_Stop,
    USBAPI_DisableInterrupts,
    USBAPI_EnableInterrupts,
    USBAPI_MoveDataArea,
    USBAPI_GetDeviceInfo,
    USBAPI_CheckDevicePresence,
    USBAPI_MassDeviceRequest,
    USBAPI_PowerManageUSB,
    USBAPI_PrepareForOS,
    USBAPI_SecureInterface,
    USBAPI_LightenKeyboardLEDs,
    USBAPI_ChangeOwner,
    USBAPI_HC_Proc,
    USBAPI_Core_Proc,
    USBAPI_LightenKeyboardLEDs_Compatible,
    USBAPI_KbcAccessControl,
    USBAPI_LegacyControl,
    USBAPI_GetDeviceAddress,
    USBAPI_ExtDriverRequest,
    USBAPI_CCIDRequest,
    USBAPI_UsbStopController,
    USBAPI_HcStartStop,
    UsbApiLockUsbSwSmi
};

/**
 Type:Function Dispatch Table
 This is the table of functions used by USB Mass Storage API
**/

API_FUNC gUsbMassApiTable[] = {
    USBMassAPIGetDeviceInformation, // USB Mass API Sub-Func 00h
    USBMassAPIGetDeviceGeometry,    // USB Mass API Sub-Func 01h
    USBMassAPIResetDevice,          // USB Mass API Sub-Func 02h
    USBMassAPIReadDevice,           // USB Mass API Sub-Func 03h
    USBMassAPIWriteDevice,          // USB Mass API Sub-Func 04h
    USBMassAPIVerifyDevice,         // USB Mass API Sub-Func 05h
    USBMassAPIFormatDevice,         // USB Mass API Sub-Func 06h
    USBMassAPICommandPassThru,      // USB Mass API Sub-Func 07h
    USBMassAPIAssignDriveNumber,    // USB BIOS API function 08h
    USBMassAPICheckDevStatus,       // USB BIOS API function 09h
    USBMassAPIGetDevStatus,         // USB BIOS API function 0Ah
    USBMassAPIGetDeviceParameters,  // USB BIOS API function 0Bh
    USBMassAPIEfiReadDevice,        // USB Mass API Sub-Func 0Ch
    USBMassAPIEfiWriteDevice,       // USB Mass API Sub-Func 0Dh
    USBMassAPIEfiVerifyDevice,      // USB Mass API Sub-Func 0Eh
    USBMassAPIGetIdentifyData       // USB Mass API Sub-Func 0Fh
};

/**
    This function is the entry point for this USB driver. 

    @param   ImageHandle   The handle associated with this image being loaded into memory
    @param   SystemTable   Pointer to the system table

    @retval  EFI_SUCCESS   Success to initial USB Driver.

**/
EFI_STATUS
EFIAPI
USBDriverEntryPoint(
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
)
{
    EFI_STATUS Status;

    Status = InitUsbHcdDriver(ImageHandle, SystemTable);
    ASSERT_EFI_ERROR(Status);
    
    return Status;
}

/**
    This function initializes the USB global data.
    @param  None
    @retval None
**/
EFI_STATUS
EFIAPI
InitializeUsbGlobalData(
  VOID
)
{

    UINTN    DriverIndex;
    UINTN    DelayedIndex;
    UINTN    Index;
    UINTN    MaxIndex;
    //
    // Initialize host controller drivers
    //
    FillHcdEntries();

    DriverIndex = 0;
    DelayedIndex = 0;
    MaxIndex = MAX_DEVICE_TYPES;

    //
    // Get Usb Data list, USB request Packet and host controller Table pointers.
    //
    if (gUsbData != NULL){
      if (gUsbData->UsbDataList != NULL)
        gUsbDataList = gUsbData->UsbDataList;      
      if (gUsbData->UsbDataList->UsbUrp != NULL)
        gParameters = gUsbData->UsbDataList->UsbUrp;      
     if (gUsbDataList->HcTable != NULL)
         gHcTable = gUsbDataList->HcTable;
    }

    Index = 0;
    while (UsbDevDelayedDrivers[Index]) {
      if (DelayedIndex == MaxIndex) {
        break;
      }
      UsbDevDelayedDrivers[Index](&gUsbDelayedDrivers[DelayedIndex]);
      if (gUsbDelayedDrivers[DelayedIndex].FnDeviceInit) {
       (*gUsbDelayedDrivers[DelayedIndex].FnDeviceInit)();
      }
      if (gUsbDelayedDrivers[DelayedIndex].DevType) {
        Index++;
        DelayedIndex++;
      }
    }

    ASSERT(DelayedIndex != MaxIndex);

    Index = 0;
    while (UsbDevDrivers[Index]) {
      if (DriverIndex == MaxIndex) {
        break;
      }
      UsbDevDrivers[Index](&gUsbDevDriverTable[DriverIndex]);
      if (gUsbDevDriverTable[DriverIndex].FnDeviceInit) {
        (*gUsbDevDriverTable[DriverIndex].FnDeviceInit)();
      }
      if (gUsbDevDriverTable[DriverIndex].DevType) {
        Index++;
        DriverIndex++;
      }
    }

    ASSERT(DriverIndex != MaxIndex);
    //
    // Allow to enumerate ports
    //
    gUsbData->EnumFlag = FALSE;

    if (gUsbDataList != NULL){
        if (gUsbDataList->HubPortStatusBuffer == NULL){
            gUsbDataList->HubPortStatusBuffer = (UINT32*)USB_MemAlloc(1);
        }
        if (gUsbDataList->InterruptStatus == NULL){
            gUsbDataList->InterruptStatus = (UINT32*)USB_MemAlloc(1);
        }
        USB_DEBUG(DEBUG_ERROR, 3, "HubPortStatusBuffer %x  InterruptStatus %x \n", gUsbDataList->HubPortStatusBuffer, gUsbDataList->InterruptStatus);
    }

    return USB_SUCCESS;
}


/**
    This function calls USB API handler.
    @param Param   Param
    @retval None
**/
VOID
EFIAPI
UsbApiHandler(
  VOID* Param
)
{
    URP_STRUC   *UsbUrp = (URP_STRUC*)Param;
    UINT8       FuncIndex;
    UINT8       NumberOfFunctions;

    if (UsbUrp == NULL) {
      return;
    }

    FuncIndex = UsbUrp->bFuncNumber;
    NumberOfFunctions = sizeof(gUsbApiTable) / sizeof (API_FUNC *);

    //
    // Make sure function number is valid; if function number is not zero
    // check for valid extended USB API function.
    //  UsbApiTable is 0-based
    //
    if (FuncIndex && ((FuncIndex < USB_NEW_API_START_FUNC ) ||
      FuncIndex >= (NumberOfFunctions + USB_NEW_API_START_FUNC - 1))) {
      USB_DEBUG(DEBUG_ERROR, 3, "UsbApiHandler Invalid function#%x\n", FuncIndex);
      return;
    }

    if (FuncIndex) {
      FuncIndex = (UINT8)(FuncIndex - USB_NEW_API_START_FUNC + 1);
    }

    // Call the appropriate function
    gUsbApiTable[FuncIndex](UsbUrp);

    UsbUrp->bFuncNumber = 0;

}


//////////////////////////////////////////////////////////////////////////////
//
// USB API Functions
//
//////////////////////////////////////////////////////////////////////////////

/**
    This routine services the USB API function number 27h. It
    handles all the mass storage related calls from the higher
    layer. Different sub-functions are invoked depending on
    the sub-function number

    @param UsbUrp    Pointer to the URP structure
        Subfunction number
        00  Get Device Information
        01  Get Device Parameter
        02  Reset Device
        03  Read Device
        04  Write Device
        05  Verify Device
        06  Format Device
        07  Request Sense
        08  Test Unit Ready
        09  Start Stop Unit
        0A  Read Capacity
        0B  Mode Sense
        0C  Device Inquiry
        0D  Send Command
        0E  Assign drive number

    @retval None
    @note URP structure is updated with the relevant information


**/

VOID
EFIAPI
USBAPI_MassDeviceRequest(
    URP_STRUC *UsbUrp
)
{
    UINT8 MassFuncIndex = UsbUrp->bSubFunc;
    UINT8 NumberOfMassFunctions = sizeof(gUsbMassApiTable) / sizeof(API_FUNC *);

    //
    // Make sure function number is valid
    //
    if (MassFuncIndex >= NumberOfMassFunctions) {
        USB_DEBUG(DEBUG_ERROR, 3, "UsbApi MassDeviceRequet Invalid function#%x\n", MassFuncIndex);
        return;
    }
    gUsbData->UsbMassDevReqFlag = 01;
    //
    // Function number is valid - call it
    //
    gUsbMassApiTable[MassFuncIndex](UsbUrp);
    gUsbData->UsbMassDevReqFlag = 00;
}


/**
    This routine services the USB API function number 0.  It
    reports the USB BIOS presence, its version number and
    its current status information

    @param  Urp    Pointer to the URP structure
    @retval None
    
    @note URP structure is updated with the following information
        CkPresence.wBiosRev       USB BIOS revision (0210h means r2.10)
        CkPresence.bBiosActive    0 - if USB BIOS is not running
        CkPresence.bNumBootDev    Number of USB boot devices found
        CkPresence.bNumHC         Number of host controller present
        CkPresence.NumPorts       Number of root hub ports
        CkPresence.dUsbDataArea   Current USB data area

**/

VOID
EFIAPI
USBAPI_CheckPresence (
  URP_STRUC *Urp
  )
{
    Urp->bRetValue                       = USB_SUCCESS;
    Urp->ApiData.CkPresence.BiosActive   = 0;
    Urp->ApiData.CkPresence.NumBootDev   = 0;    // Number of USB boot devices found
    Urp->ApiData.CkPresence.NumKeyboards = 0;    // Number of USB keyboards present
    Urp->ApiData.CkPresence.NumMice      = 0;    // Number of USB mice present
    Urp->ApiData.CkPresence.NumPoint     = 0;    // Number of USB Point present  
    Urp->ApiData.CkPresence.NumHubs      = 0;    // Number of USB hubs present
    Urp->ApiData.CkPresence.NumStorage   = 0;    // Number of USB storage devices present

    Urp->ApiData.CkPresence.BiosRev = USB_DRIVER_MAJOR_VER;
    Urp->ApiData.CkPresence.BiosActive = USB_ACTIVE; // Set USB BIOS as active
    if (!(gUsbData->UsbStateFlag & USB_FLAG_DISABLE_LEGACY_SUPPORT)) {
        Urp->ApiData.CkPresence.BiosActive |= USB_LEGACY_ENABLE;
    }
    if (gUsbData->UsbStateFlag & USB_FLAG_6064EMULATION_ON) {
        Urp->ApiData.CkPresence.BiosActive |= USB_6064_ENABLE;
    }
    
    // Get active USB devices
    USBWrap_GetDeviceCount(Urp);  
}


/**
    This API routine configures the USB host controllers and
    enumerate the devices

    @param Urp URP structure with input parameters
    @retval None        
    @note  StartHc.DataAreaFlag Indicates which data area to use
           StartHc.DataAreaFlag Returns current data area pointer


**/

VOID
EFIAPI
USBAPI_Start (
    URP_STRUC *Urp
)
{
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBSMI: Start\n");
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\tUSBAPI_HC_Proc:%x\n", &USBAPI_HC_Proc);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\tUSBAPI_Core_Proc:%x\n", &USBAPI_Core_Proc);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\tUSB_ReConfigDevice:%x\n", &USB_ReConfigDevice);
    Urp->bRetValue = USB_StartHostControllers();
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_StartHostControllers returns %d\n", Urp->bRetValue);
}


/**
    This routine stops the USB host controllers

    @param  Urp    Pointer to the URP structure
    @retval None  

**/

VOID
EFIAPI 
USBAPI_Stop (
    URP_STRUC *Urp
)
{
    gCheckUsbApiParameter = FALSE;
    Urp->bRetValue = USB_StopHostControllers();
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_StopHostControllers returns %d\n", Urp->bRetValue);
    gUsbData->UsbStateFlag &= ~(USB_FLAG_DRIVER_STARTED);
}


/**
    This routine suspends the USB host controllers

    @param  Urp    Pointer to the URP structure
    @retval None

**/

VOID
EFIAPI
USBAPI_PowerManageUSB (
    URP_STRUC *Urp
)
{
    Urp->bRetValue = USB_NOT_SUPPORTED;
}


/**
    This routine updates data structures to reflect that
    POST is completed

    @param  Urp    Pointer to the URP structure
    @retval None

**/

VOID
EFIAPI
USBAPI_PrepareForOS (
    URP_STRUC *Urp
)
{
    Urp->bRetValue = USB_NOT_SUPPORTED;
}


/**
    This routine handles the calls related to security device

    @param  Urp    Pointer to the URP structure
    @retval None

**/

VOID
USBAPI_SecureInterface (
    URP_STRUC *Urp
)
{
    Urp->bRetValue = USB_NOT_SUPPORTED;
}


/**
    This routine stops the USB host controllers interrupts

    @param  Urp    Pointer to the URP structure
    @retval None

**/

VOID
EFIAPI
USBAPI_DisableInterrupts (
    URP_STRUC *Urp
)
{
    Urp->bRetValue = USB_NOT_SUPPORTED;
}


/**
    This routine re-enable the USB host controller interrupts

    @param  Urp     Pointer to the URP structure
    @retval None

**/

VOID
EFIAPI
USBAPI_EnableInterrupts (
    URP_STRUC *Urp
)
{
    Urp->bRetValue = USB_NOT_SUPPORTED;
}


/**
    This routine stops the USB host controllers and moves
    the data area used by host controllers to a new area.
    The host controller is started from the new place.

    @param  Urp    Pointer to the URP structure
    @retval None

**/

VOID
EFIAPI
USBAPI_MoveDataArea (
    URP_STRUC *Urp
)
{
    if (Urp->bSubFunc == USB_DATA_SMM_TO_DXE) {
        AmiUsbGlobalDataSwitch (
            &gUsbData, 
            &gUsbDataList, 
            (HC_STRUC**)&gHcTable,
            &gDataInSmm, 
            USB_DATA_SMM_TO_DXE
            );
    }
    Urp->bRetValue = USB_SUCCESS;
}


/**
    This routine returns the information regarding
    a USB device like keyboard, mouse, floppy drive etc

    @param Urp            URP structure with input parameters
           GetDevInfo.bDevNumber   Device number (1-based) whose
           information is requested
    @retval None
    @note URP structure is updated with the following information
        GetDevInfo.HcNumber - HC number in which the device is found
        GetDevInfo.DevType  - Type of the device
        bRetValue will be one of the following value
        USB_SUCCESS         on successfull completion
        USB_PARAMETER_ERROR if bDevNumber is invalid
        USB_ERROR           on error

**/

VOID 
EFIAPI
USBAPI_GetDeviceInfo (URP_STRUC *Urp)
{
    DEV_INFO* FpDevInfo;

    //
    // Initialize the return values
    //
    Urp->ApiData.GetDevInfo.HcNumber = 0;
    Urp->ApiData.GetDevInfo.DevType  = 0;
    Urp->bRetValue                   = USB_ERROR;

    //
    // Check for parameter validity
    //
    if ((!Urp->ApiData.GetDevInfo.DevNumber) || 
        (Urp->ApiData.GetDevInfo.DevNumber >= gUsbData->MaxDevCount)) {
        return;
    }

    Urp->bRetValue = USB_PARAMETER_ERROR;

    //
    // Get the device information structure for the 'n'th device
    //
    FpDevInfo = USBWrap_GetnthDeviceInfoStructure(Urp->ApiData.GetDevInfo.DevNumber);
    //
    // Return value
    //
    Urp->ApiData.GetDevInfo.DevType  = FpDevInfo->DeviceType;
    Urp->ApiData.GetDevInfo.HcNumber = FpDevInfo->HcNumber;
    Urp->bRetValue                   = USB_SUCCESS;

}


/**
    This routine checks whether a particular type of USB device
    is installed in the system or not.

    @param Urp   URP structure with input parameters
        ChkDevPrsnc.DevType    Device type to find
        ChkDevPrsnc.fpHCStruc   Pointer to HC being checked for device
        connection; if NULL then the total number of devices
        connected to ANY controller is returned.
        ChkDevPrsnc.bNumber     Number of devices connected
        
    @retval None
    @note bRetValue will be one of the following value
        USB_SUCCESS     if device type present, ChkDevPrsnc.bNumber <> 0
        USB_ERROR       if device type absent, ChkDevPrsnc.bNumber returns 0

**/

VOID
EFIAPI
USBAPI_CheckDevicePresence (URP_STRUC *Urp)
{
    UINT8 SearchFlag;
    UINTN Data;

    SearchFlag = USB_SRCH_DEV_NUM;
    if (Urp->bSubFunc == 1)
    {
        SearchFlag = USB_SRCH_DEVBASECLASS_NUM;
    }
    //
    // The total number of devices connected to ANY controller has been requested
    //
    Data = (UINTN) USB_GetDeviceInfoStruc( SearchFlag,
            0, Urp->ApiData.ChkDevPrsnc.DevType, Urp->ApiData.ChkDevPrsnc.HcStruc);

    Urp->ApiData.ChkDevPrsnc.Number = (UINT8)Data;
    Urp->bRetValue = (UINT8)((Urp->ApiData.ChkDevPrsnc.Number)?
                                            USB_SUCCESS : USB_ERROR);
}


/**
    This function is part of the USB BIOS MASS API. This function
    returns the device information of the mass storage device

    @param  Urp    Pointer to the URP structure
    @retval None

    @note Initially the DevAddr should be set to 0 as input. This
          function returns the information regarding the first mass
          storage device (if no device found it returns DevAddr as
          0FFh) and also updates DevAddr to the device address of
          the current mass storage device. If no other mass storage
          device is found then the routine sets the bit7 to 1
          indicating current information is valid but no more mass
          device found in the system. The caller can get the next
          device info if DevAddr is not 0FFh and bit7 is not set

**/
VOID
EFIAPI
USBMassAPIGetDeviceInformation (URP_STRUC *Urp)
{
    Urp->bRetValue = USBMassGetDeviceInfo (&Urp->ApiData.MassGetDevInfo);
}

/**
    This function is part of the USB BIOS MASS API. Get mass storage parameters data.

    @param  Urp    Pointer to the URP structure
    @retval None
**/
VOID
EFIAPI
USBMassAPIGetDeviceParameters (URP_STRUC *Urp)
{
    DEV_INFO    *DevInfo;
    EFI_STATUS  Status;

    DevInfo = Urp->ApiData.MassGetDevParms.FpDevInfo;

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return;
    }

    Urp->ApiData.MassGetDevParms.FpInqData = USBMassGetDeviceParameters(DevInfo);
    Urp->bRetValue = (Urp->ApiData.MassGetDevParms.FpInqData == NULL)? USB_ERROR : USB_SUCCESS;
}

/**
    This function is part of the USB BIOS MASS API. Get mass storage identify data.

    @param  Urp    Pointer to the URP structure
    @retval None
**/
VOID 
EFIAPI
USBMassAPIGetIdentifyData (URP_STRUC *Urp)
{
    DEV_INFO    *DevInfo;

    DevInfo = Urp->ApiData.MassGetIdentifyData.DevInfo;
    Urp->ApiData.MassGetIdentifyData.Identify = USBMassGetIdentifyData (DevInfo);
    Urp->bRetValue = (Urp->ApiData.MassGetIdentifyData.Identify == NULL)? USB_ERROR : USB_SUCCESS;
}

/**
    This function returns the drive status and media presence status

    @param  Urp    Pointer to the Urp structure
    @retval None

    @note USB_SUCCESS  Success
    @note USB_ERROR    Failure
**/

VOID
EFIAPI
USBMassAPIGetDevStatus(URP_STRUC *Urp)
{
    if ((gUsbData->UsbDevSupport & USB_MASS_DEV_SUPPORT) == USB_MASS_DEV_SUPPORT)
        Urp->bRetValue    = USBMassGetDeviceStatus (&Urp->ApiData.MassGetDevSts);
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBMassAPIGetDevStatus ... check function call correct?\n");
}



/**
    This function is part of the USB BIOS MASS API.

    @param  Urp    Pointer to the Urp structure
    @retval None
    
    @note bRetValue   Return value
        Urp             Pointer to the URP structure
        SenseData       Sense data of the last command
        NumHeads        Number of heads
        NumCylinders    Number of cylinders
        NumSectors      Number of sectors
        BytesPerSector  Number of bytes per sector
        MediaType       Media type

**/

VOID
EFIAPI
USBMassAPIGetDeviceGeometry(URP_STRUC *Urp)
{
    Urp->bRetValue = USBMassGetDeviceGeometry (&Urp->ApiData.MassGetDevGeo);
}


/**
    This function is part of the USB BIOS MASS API.

    @param  Urp    Pointer to the Urp structure
    @retval None 

**/

VOID
EFIAPI
USBMassAPIResetDevice (URP_STRUC *Urp)
{
    UINT8       DevAddr;
    DEV_INFO    *FpDevInfo;
    UINT16      Result;

    DevAddr = Urp->ApiData.MassReset.DevAddr;

    //
    // Get the device info structure for the matching device address
    //
    FpDevInfo   = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);
    if ((FpDevInfo == NULL)|| (!(FpDevInfo->Flag & DEV_INFO_DEV_PRESENT))) {
        Urp->bRetValue = USB_ATA_TIME_OUT_ERR;
        return;
    }
    //
    // Send Start/Stop Unit command to UFI class device only
    //
    Urp->bRetValue    = USB_SUCCESS;
    if (FpDevInfo->SubClass ==  SUB_CLASS_UFI) {
        Result = USBMassStartUnitCommand (FpDevInfo);
        if (Result) {
            Urp->bRetValue  = USBWrapGetATAErrorCode(Urp->ApiData.MassReset.SenseData);
        }
    }
}

/**
    This function is part of the USB BIOS MASS API.
    @param  Urp    Pointer to the Urp structure
    @retval None 
    
**/
VOID
EFIAPI
USBMassAPIReadDevice (
    URP_STRUC *Urp
)
{
    URP_STRUC               Parameters;

    Parameters.bRetValue = USB_ERROR;
    Parameters.ApiData.EfiMassRead.DevAddr = Urp->ApiData.MassRead.DevAddr;
    Parameters.ApiData.EfiMassRead.StartLba = Urp->ApiData.MassRead.StartLba;
    Parameters.ApiData.EfiMassRead.NumBlks = Urp->ApiData.MassRead.NumBlks;
    Parameters.ApiData.EfiMassRead.PreSkipSize = Urp->ApiData.MassRead.PreSkipSize;
    Parameters.ApiData.EfiMassRead.PostSkipSize = Urp->ApiData.MassRead.PostSkipSize;
    Parameters.ApiData.EfiMassRead.BufferPtr = Urp->ApiData.MassRead.BufferPtr;

    USBMassAPIEfiReadDevice(&Parameters);

    Urp->bRetValue = Parameters.bRetValue;

    return;
}

/**
    This function is part of the USB BIOS MASS API.
    @param  Urp    Pointer to the Urp structure
    @retval None 
    
**/
VOID
EFIAPI
USBMassAPIEfiReadDevice (
    URP_STRUC *Urp
)
{
    DEV_INFO    *DevInfo;
    UINT8       DevAddr;
    UINT16      Result;
    UINT32      Data;
    UINT8       OpCode;

    DevAddr = Urp->ApiData.EfiMassRead.DevAddr;

    if (((DevAddr == USB_HOTPLUG_FDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_FDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_HDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_HDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_CDROM_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_CDROM_ENABLED) == FALSE))) {
        Urp->bRetValue = USB_ATA_DRIVE_NOT_READY_ERR;
        return;
    }

    //
    // Get the device info structure for the matching device address
    //
    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);
    if ((DevInfo == NULL)|| (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT))) {
        Urp->bRetValue = USB_ATA_TIME_OUT_ERR;
        return;
    }


    if ((gUsbData->UsbFeature & USB_EXTRA_CHECK_DEVICE_READY) == USB_EXTRA_CHECK_DEVICE_READY) {
        //
        // Check device ready
        //
        Data = USBMassCheckDeviceReady(DevInfo);
        if (Data) {
            Urp->ApiData.EfiMassRead.SenseData = Data;
            Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassRead.SenseData); 
            return;
        }
    }

    //
    // Issue read command
    //
    if (RShiftU64(DevInfo->MaxLba, 32)) {
        OpCode = COMMON_READ_16_OPCODE;
    } else {
        OpCode = COMMON_READ_10_OPCODE;
    }
    Result = USBMassRWVCommand(DevInfo, OpCode, &Urp->ApiData.EfiMassRead);
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " wr(%x):%x %x", DevAddr, Result, Urp->ApiData.EfiMassRead.SenseData);
    if (Result) {
        Urp->bRetValue = USB_SUCCESS;
        return;
    }
    Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassRead.SenseData);
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, " er(%x):%x", DevAddr, Urp->bRetValue);
}

/**
    This function is part of the USB BIOS MASS API.
        
    @param Urp    Pointer to the URP structure
                  DevAddr         USB device address of the device
                  StartLba        Starting LBA address
                  NumBlks         Number of blocks to write
                  PreSkipSize     Number of bytes to skip before
                  PostSkipSize    Number of bytes to skip after
                  BufferPtr       Buffer pointer
    @retval None 

    @retval Urp             Pointer to the Urp structure
            bRetValue       Return value
            SenseData       Sense data of the last command

**/

VOID
EFIAPI
USBMassAPIWriteDevice(
    URP_STRUC *Urp
)
{
    URP_STRUC               Parameters;

    Parameters.bRetValue = USB_ERROR;
    Parameters.ApiData.EfiMassWrite.DevAddr = Urp->ApiData.MassWrite.DevAddr;
    Parameters.ApiData.EfiMassWrite.StartLba = Urp->ApiData.MassWrite.StartLba;
    Parameters.ApiData.EfiMassWrite.NumBlks = Urp->ApiData.MassWrite.NumBlks;
    Parameters.ApiData.EfiMassWrite.PreSkipSize = Urp->ApiData.MassWrite.PreSkipSize;
    Parameters.ApiData.EfiMassWrite.PostSkipSize = Urp->ApiData.MassWrite.PostSkipSize;
    Parameters.ApiData.EfiMassWrite.BufferPtr = Urp->ApiData.MassWrite.BufferPtr;

    USBMassAPIEfiWriteDevice(&Parameters);

    Urp->bRetValue = Parameters.bRetValue;

    return;
}

/**
    This function is part of the USB BIOS MASS API.

    @param Urp     Pointer to the URP structure
                   DevAddr       USB device address of the device
                   StartLBA      Starting LBA address
                   NumBlks       Number of blocks to write
                   PreSkipSize   Number of bytes to skip before
                   PostSkipSize  Number of bytes to skip after
                   BufferPtr     Far buffer pointer
                   bRetValue     Return value
                   SenseData     Sense data of the last command

    @retval None
**/

VOID
EFIAPI
USBMassAPIEfiWriteDevice (
    URP_STRUC *Urp
)
{
    DEV_INFO    *DevInfo;
    UINT8       DevAddr;
    UINT16      Result;
    UINT32      Data;
    UINT8       OpCode;

    DevAddr = Urp->ApiData.EfiMassWrite.DevAddr;

    if (((DevAddr == USB_HOTPLUG_FDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_FDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_HDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_HDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_CDROM_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_CDROM_ENABLED) == FALSE))) {
        Urp->bRetValue = USB_ATA_DRIVE_NOT_READY_ERR;
        return;
    }

    //
    // Get the device info structure for the matching device address
    //
    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);
    if ((DevInfo == NULL)|| (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT))) {
        Urp->bRetValue = USB_ATA_TIME_OUT_ERR;
        return;
    }

    if ((gUsbData->UsbFeature & USB_EXTRA_CHECK_DEVICE_READY) == USB_EXTRA_CHECK_DEVICE_READY) {
        //
        // Check device ready
        //
        Data = USBMassCheckDeviceReady(DevInfo);
        if (Data) {
            Urp->ApiData.EfiMassWrite.SenseData = Data;
            Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassWrite.SenseData); 
            return;
        }
    }

    //
    // Issue write command
    //
    if (RShiftU64(DevInfo->MaxLba, 32)) {
        OpCode = COMMON_WRITE_16_OPCODE;
    } else {
        OpCode = COMMON_WRITE_10_OPCODE;
    }
    Result = USBMassRWVCommand(DevInfo, OpCode, &Urp->ApiData.EfiMassWrite);
    if (Result) {
        Urp->bRetValue = USB_SUCCESS;
        return;
    }

    Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassWrite.SenseData);
}

/**
    This function is part of the USB BIOS MASS API.

    @param Urp    Pointer to the URP structure
                  DevAddr        USB device address of the device
                  StartLBA       Starting LBA address
                  NumBlks        Number of blocks to write
                  PreSkipSize    Number of bytes to skip before
                  PostSkipSize   Number of bytes to skip after
                  BufferPtr Far buffer pointer
                  bRetValue      Return value
                  SenseData      Sense data of the last command
                  
    @retval None

**/

VOID
EFIAPI
USBMassAPIVerifyDevice(
    URP_STRUC *Urp
)
{
    URP_STRUC               Parameters;

    Parameters.bRetValue = USB_ERROR;
    Parameters.ApiData.EfiMassVerify.DevAddr = Urp->ApiData.MassVerify.DevAddr;
    Parameters.ApiData.EfiMassVerify.StartLba = Urp->ApiData.MassVerify.StartLba;
    Parameters.ApiData.EfiMassVerify.NumBlks = Urp->ApiData.MassVerify.NumBlks;
    Parameters.ApiData.EfiMassVerify.PreSkipSize = Urp->ApiData.MassVerify.PreSkipSize;
    Parameters.ApiData.EfiMassVerify.PostSkipSize = Urp->ApiData.MassVerify.PostSkipSize;
    Parameters.ApiData.EfiMassVerify.BufferPtr = Urp->ApiData.MassVerify.BufferPtr;

    USBMassAPIEfiVerifyDevice(&Parameters);

    Urp->bRetValue = Parameters.bRetValue;

    return;
}

/**
    This function is part of the USB BIOS MASS API.

    @param Urp    Pointer to the URP structure
                  DevAddr        USB device address of the device
                  StartLBA       Starting LBA address
                  NumBlks        Number of blocks to write
                  PreSkipSize    Number of bytes to skip before
                  PostSkipSize   Number of bytes to skip after
                  BufferPtr      Far buffer pointer
                  bRetValue      Return value
                  SenseData      Sense data of the last command
                  
    @retval None
**/
VOID
EFIAPI
USBMassAPIEfiVerifyDevice(
    URP_STRUC *Urp
)
{
    DEV_INFO    *DevInfo;
    UINT8       DevAddr;
    UINT16      Result;
    UINT32      Data;
    UINT8       OpCode;

    DevAddr = Urp->ApiData.EfiMassVerify.DevAddr;

    if (((DevAddr == USB_HOTPLUG_FDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_FDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_HDD_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_HDD_ENABLED) == FALSE)) ||
        ((DevAddr == USB_HOTPLUG_CDROM_ADDRESS) &&
            ((gUsbData->UsbStateFlag & USB_HOTPLUG_CDROM_ENABLED) == FALSE))) {
        Urp->bRetValue = USB_ATA_DRIVE_NOT_READY_ERR;
        return;
    }

    //
    // Get the device info structure for the matching device address
    //
    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);
    if ((DevInfo == NULL)|| (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT))) {
        Urp->bRetValue = USB_ATA_TIME_OUT_ERR;
        return;
    }

    if ((gUsbData->UsbFeature & USB_EXTRA_CHECK_DEVICE_READY) == USB_EXTRA_CHECK_DEVICE_READY) {
        //
        // Check device ready
        //
        Data = USBMassCheckDeviceReady(DevInfo);
        if (Data) {
            Urp->ApiData.EfiMassVerify.SenseData = Data;
            Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassVerify.SenseData); 
            return;
        }
    }

    //
    // Issue write command
    //
    if (RShiftU64(DevInfo->MaxLba, 32)) {
        OpCode = COMMON_VERIFY_16_OPCODE;
    } else {
        OpCode = COMMON_VERIFY_10_OPCODE;
    }
    Result = USBMassRWVCommand(DevInfo, OpCode, &Urp->ApiData.EfiMassVerify);
    if (Result) {
        Urp->bRetValue = USB_SUCCESS;
        return;
    }

    Urp->bRetValue = USBWrapGetATAErrorCode(Urp->ApiData.EfiMassVerify.SenseData);

}

/**
    This function is part of the USB BIOS MASS API.

    @param Urp          Pointer to the URP structure
                        bRetValue   Return value
    @retval None

**/
VOID
EFIAPI
USBMassAPIFormatDevice(URP_STRUC *Urp)
{
    Urp->bRetValue = USB_SUCCESS;
}


/**
    This function is part of the USB BIOS MASS API. This
    function can be used to pass raw command/data sequence to
    the USB mass storage device

    @param Urp      Pointer to the URP structure
                    bRetValue   Return value
    @retval None

**/

VOID
EFIAPI
USBMassAPICommandPassThru (URP_STRUC *Urp)
{
    UINT8                   Result;
    MASS_CMD_PASS_THRU      *MassCmdPassThru;
    EFI_STATUS              Status;

    MassCmdPassThru = &Urp->ApiData.MassCmdPassThru;

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)MassCmdPassThru->CmdBuffer, MassCmdPassThru->CmdLength);
        if (Status != EFI_ABORTED){
            if (EFI_ERROR(Status)) {
                return;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)MassCmdPassThru->DataBuffer, MassCmdPassThru->DataLength);
            if (EFI_ERROR(Status)) {
                return;
            }
            gCheckUsbApiParameter = FALSE;
        }
    }

    Result = USBMassCmdPassThru(MassCmdPassThru);
    
    Urp->bRetValue = Result;
}


/**
    This function is part of the USB BIOS MASS API. This function
    assigns the logical drive device according to the information of the
    mass storage device

    @param Urp     Pointer to the URP structure
                   U_API_DATA->DevAddr               USB device address of the device
                   MASS_ASSIGN_DRIVE_NUM->LogDevNum  Logical Drive Number to assign to the device

    @retval USB_ERROR     Failure
    @retval USB_SUCCESS   Success

**/

VOID
EFIAPI
USBMassAPIAssignDriveNumber (
    URP_STRUC    *Urp
)
{
    // No errors expected after this point
    Urp->bRetValue = USB_SUCCESS;
}


/**
    This function is part of the USB BIOS MASS API. This function
    invokes USB Mass Storage API handler to check whether device
    is ready. If called for the first time, this function retrieves
    the mass storage device geometry and fills the corresponding
    FpDevInfo fields.

    @param Urp    Pointer to the URP structure
                  Urp->ApiData.FpDevInfo  pointer to USB device that is requested to be checked

    @retval USB_ERROR     Failure
    @retval USB_SUCCESS   Success

**/
VOID
EFIAPI
USBMassAPICheckDevStatus (
    URP_STRUC    *Urp
)
{

    UINT32      Result;
    DEV_INFO    *DevInfo;
    EFI_STATUS  Status;

    if ((gUsbData->UsbDevSupport & USB_MASS_DEV_SUPPORT) == USB_MASS_DEV_SUPPORT) {
        DevInfo = Urp->ApiData.MassChkDevReady.FpDevInfo;

        Status = UsbDevInfoValidation(DevInfo);
        
        if (EFI_ERROR(Status)) {
            return;
        }
        
        gCheckUsbApiParameter = FALSE;
        
        Result = USBMassCheckDeviceReady(Urp->ApiData.MassChkDevReady.FpDevInfo);
        Urp->bRetValue = (UINT8)Result;
    }

}


/**
    This function is part of the USB BIOS API. This function
    controls LED state on the connected USB keyboards

    @param Urp    Pointer to the URP structure

    @retval USB_ERROR     Failure 
    @retval USB_SUCCESS   Success

**/
VOID
EFIAPI
USBAPI_LightenKeyboardLEDs (
    URP_STRUC    *fpURP
)
{

    EFI_STATUS  Status;
    DEV_INFO    *DevInfo = (DEV_INFO*)fpURP->ApiData.EfiKbLedsData.DevInfoPtr;

    if ((gUsbData->UsbDevSupport & USB_KB_DEV_SUPPORT) == USB_KB_DEV_SUPPORT) {
        if (DevInfo) {
            Status = UsbDevInfoValidation(DevInfo);
            if (EFI_ERROR(Status)) {
                fpURP->bRetValue = USB_PARAMETER_ERROR;
                return;
            }
        }
        
        gCheckUsbApiParameter = FALSE;
        
        if (fpURP->ApiData.EfiKbLedsData.LedMapPtr) {
            gUsbData->UsbKbShiftKeyStatus = 0;
            if(((LED_MAP*)fpURP->ApiData.EfiKbLedsData.LedMapPtr)->NumLock) {
                gUsbData->UsbKbShiftKeyStatus |= KB_NUM_LOCK_BIT_MASK;
            }
            if(((LED_MAP*)fpURP->ApiData.EfiKbLedsData.LedMapPtr)->CapsLock) {
                gUsbData->UsbKbShiftKeyStatus |= KB_CAPS_LOCK_BIT_MASK;
            }
            if(((LED_MAP*)fpURP->ApiData.EfiKbLedsData.LedMapPtr)->ScrLock) {
                gUsbData->UsbKbShiftKeyStatus |= KB_SCROLL_LOCK_BIT_MASK;
            }
        }
        
        if (DevInfo) {
            UsbKbdSetLed(DevInfo, ((gUsbData->UsbKbShiftKeyStatus) >> 4) & 0x07);
        }
        
        fpURP->bRetValue = USB_SUCCESS;

    } else {
        fpURP->bRetValue = USB_NOT_SUPPORTED;
    }

}

/**
    This function is part of the USB BIOS API. This function
    controls LED state on the connected USB keyboards

    @param Urp    Pointer to the URP structure

    @retval None

**/
VOID
EFIAPI
USBAPI_LightenKeyboardLEDs_Compatible (
    URP_STRUC    *Urp
)
{

    if ((gUsbData->UsbDevSupport & USB_KB_DEV_SUPPORT) == USB_KB_DEV_SUPPORT) {
    if (Urp->ApiData.KbLedsData.LedMapPtr) {
        gUsbData->UsbKbShiftKeyStatus = 0;
        if (((LED_MAP*)Urp->ApiData.KbLedsData.LedMapPtr)->NumLock) {
            gUsbData->UsbKbShiftKeyStatus |= KB_NUM_LOCK_BIT_MASK;
        }
        if (((LED_MAP*)Urp->ApiData.KbLedsData.LedMapPtr)->CapsLock) {
            gUsbData->UsbKbShiftKeyStatus |= KB_CAPS_LOCK_BIT_MASK;
        }
        if (((LED_MAP*)Urp->ApiData.KbLedsData.LedMapPtr)->ScrLock) {
            gUsbData->UsbKbShiftKeyStatus |= KB_SCROLL_LOCK_BIT_MASK;
        }
    }

    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3," LEDs: %d\n", gUsbData->UsbKbShiftKeyStatus);
    USBKB_LEDOn();

    Urp->bRetValue = USB_SUCCESS;

    } else {

        Urp->bRetValue = USB_NOT_SUPPORTED;
    }

}


/**
    This function is part of the USB BIOS API. This function
    is used to control whether KBC access in USB module 
    should be blocked or not.

    @param ControlSwitch    TRUE: Access block / FALSE: Not block
    @retval None

**/
VOID
EFIAPI
UsbKbcAccessControl(
    UINT8 ControlSwitch
)
{
    UINT8       Index;
    HC_STRUC    *HcStruc;

    EFI_STATUS Status;
    
    Status = AmiUsbGlobalDataValidation(gUsbData);

    if (Status != EFI_ABORTED){
        ASSERT_EFI_ERROR(Status);
    
        if (EFI_ERROR(Status)) {
            gLockHwSmiHandler = TRUE;
            gLockSmiHandler = TRUE;
            return;
        }
    }


    gCheckUsbApiParameter = FALSE;
  
    gUsbData->IsKbcAccessBlocked = (ControlSwitch != 0)? TRUE : FALSE;

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if (HcStruc->HcFlag & HC_STATE_RUNNING) {
            break;
        }
    }

    if (Index == gUsbDataList->HcTableCount) {
        return;
    }

    //Reflush USB data buffer if intend to disable usb keyboard data throughput.
    if (gUsbData->IsKbcAccessBlocked) {
        USBKeyRepeat(NULL, 1);  // Disable Key repeat
        gUsbData->RepeatKey = 0;

        // Clear Legacy USB keyboard buffer
        ZeroMem(gUsbData->KbcCharacterBufferStart, sizeof(gUsbData->KbcCharacterBufferStart));
        gUsbDataList->KbcCharacterBufferHead = gUsbData->KbcCharacterBufferStart;
        gUsbDataList->KbcCharacterBufferTail = gUsbData->KbcCharacterBufferStart;
                
        ZeroMem(gUsbData->KbcScanCodeBufferStart, sizeof(gUsbData->KbcScanCodeBufferStart));
        gUsbDataList->KbcScanCodeBufferPtr = gUsbData->KbcScanCodeBufferStart;
                                                                                        
        ZeroMem(gUsbData->KbcDeviceIdBufferStart, sizeof(gUsbData->KbcDeviceIdBufferStart));
        ZeroMem(gUsbData->KbcShiftKeyStatusBufferStart, sizeof(gUsbData->KbcShiftKeyStatusBufferStart));              
        ZeroMem(gUsbData->UsbKbInputBuffer, sizeof(gUsbData->UsbKbInputBuffer));
    }

}

/**
    This function is part of the USB BIOS API. This function
    is used to control whether KBC access in USB module 
    should be blocked or not.

    @param  Urp      Pointer to the URP structure
    @retval None

**/
VOID
EFIAPI
USBAPI_KbcAccessControl(URP_STRUC *Urp)
{
    UsbKbcAccessControl(Urp->ApiData.KbcControlCode);
}

/**
    This function is part of the USB BIOS API. This function initial USB 
    legacy support.

    @param  Urp      Pointer to the URP structure
    @retval None

**/
VOID
USB_StopLegacy(URP_STRUC *Urp)
{
    //shutdown device first
    UINT8       bIndex;
    DEV_INFO    *FpDevInfo;
    HC_STRUC    *fpHCStruc;
    
    for (bIndex = 1; bIndex < gUsbData->MaxDevCount; bIndex ++){
        FpDevInfo = gUsbDataList->DevInfoTable + bIndex;
        if ((FpDevInfo->Flag & 
            (DEV_INFO_VALID_STRUC |DEV_INFO_DEV_PRESENT)    ) ==   
            (DEV_INFO_VALID_STRUC |DEV_INFO_DEV_PRESENT)    ){
            //
            fpHCStruc = gHcTable[FpDevInfo->HcNumber - 1];
            //
            USB_StopDevice (fpHCStruc, FpDevInfo->HubDeviceNumber, FpDevInfo->HubPortNumber);
        }
    }

    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT)
        StopControllerType(USB_HC_XHCI);


    if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT)
        StopControllerType(USB_HC_EHCI);

    if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == USB_HC_UHCI_SUPPORT)
        StopControllerType(USB_HC_UHCI);

    if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT)
        StopControllerType(USB_HC_OHCI);
    
    //return as success
    Urp->bRetValue = USB_SUCCESS;
    
}

/**
    This function is part of the USB BIOS API. This function initial USB 
    legacy support.
    
    @param  Urp    Pointer to the URP structure
    @retval None

**/
VOID
USB_StartLegacy(URP_STRUC *Urp)
{

    gUsbData->HandOverInProgress = FALSE;

    //Start XHCI
    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT){
        StartControllerType(USB_HC_XHCI);
        USB_EnumerateRootHubPorts(USB_HC_XHCI);
    }

    //Start EHCI
    if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT){
        StartControllerType(USB_HC_EHCI);
        USB_EnumerateRootHubPorts(USB_HC_EHCI);
    }

    //Start UHCI
    if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == USB_HC_UHCI_SUPPORT){
        StartControllerType(USB_HC_UHCI);
        USB_EnumerateRootHubPorts(USB_HC_UHCI);
    }

    //Start OHCI
    if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT){
        StartControllerType(USB_HC_OHCI);
        USB_EnumerateRootHubPorts(USB_HC_OHCI);
    }

    //return as success
    Urp->bRetValue    = USB_SUCCESS;
}

/**
    This function is part of the USB BIOS API. This function
    is used to shutdown/initial USB legacy support.

    @param Urp    Pointer to the URP structure

    @retval None

**/
VOID
EFIAPI
USBAPI_LegacyControl (
    URP_STRUC  *Urp
)
{
    UINT8 SubLegacyFunc = Urp->bSubFunc, Index;       
    UINT8 Count = (UINT8)(gUsbDataList->KbcScanCodeBufferPtr - 
                  (UINT8*)gUsbData->KbcScanCodeBufferStart);   

    gCheckUsbApiParameter = FALSE;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBAPI_LegacyControl %d\n", Urp->bSubFunc);
    if (SubLegacyFunc == STOP_USB_CONTROLLER) {
        USB_StopLegacy (Urp);

        for (Index = Count; Index > 0; Index--)
            USBKB_DiscardCharacter(&gUsbData->KbcShiftKeyStatusBufferStart[Index - 1]); 

        if (gEmulationTrap)
            gEmulationTrap->TrapDisable(gEmulationTrap);
    }

    if (SubLegacyFunc==START_USB_CONTROLLER) {
        USB_StartLegacy (Urp);
        if (gEmulationTrap)
            gEmulationTrap->TrapEnable(gEmulationTrap);
    }          
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Result %d\n", Urp->bRetValue);
}
         
/**
    This function is part of the USB BIOS API. This function stops 
    the USB host controller.

    @param  Urp    Pointer to the URP structure
    @retval None

**/
VOID
EFIAPI
USBAPI_UsbStopController (
    URP_STRUC *Urp
)
{
    gCheckUsbApiParameter = FALSE;
    StopControllerBdf(Urp->ApiData.HcBusDevFuncNum);
}

/**
    This function is used to shutdown/init USB legacy support,
    it calls USBAPI_LegacyControl().
    
    @param  Urp    Pointer to the URP structure

    @retval EFI_SUCCESS    Success
    @retval Others         Fail
**/
EFI_STATUS
EFIAPI
USBRT_LegacyControl(
    VOID *Urp
)
{

    EFI_STATUS Status;
    
    Status = AmiUsbGlobalDataValidation(gUsbData);
    if (Status != EFI_ABORTED){
        ASSERT_EFI_ERROR(Status);
    
        if (EFI_ERROR(Status)) {
            gLockHwSmiHandler = TRUE;
            gLockSmiHandler = TRUE;
            return EFI_SUCCESS;
        }
    }

    USBAPI_LegacyControl ((URP_STRUC *)Urp);

    return((EFI_STATUS)(((URP_STRUC *)Urp)->bRetValue));
}


/**
    Get device address

    @param  Urp    Pointer to the URP structure
    @retval None

**/
VOID
EFIAPI
USBAPI_GetDeviceAddress(
    URP_STRUC *Urp
)
{
    UINT8  Index;
    DEV_INFO *DevInfo = NULL;

    for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
        if (!(gUsbDataList->DevInfoTable[Index].Flag & DEV_INFO_VALID_STRUC)) {
            continue;
        }
        if ((gUsbDataList->DevInfoTable[Index].VendorId == Urp->ApiData.GetDevAddr.Vid) && 
            (gUsbDataList->DevInfoTable[Index].DeviceId == Urp->ApiData.GetDevAddr.Did)) {
            DevInfo = &gUsbDataList->DevInfoTable[Index];
        }
    }
    if (DevInfo == NULL) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    Urp->ApiData.GetDevAddr.DevAddr = DevInfo->DeviceAddress;
    Urp->bRetValue = USB_SUCCESS;
}

/**
    This is USB APT which calls deriver request.

    @param  Urp    Pointer to the URP structure
    @retval None

**/
VOID
EFIAPI
USBAPI_ExtDriverRequest (
    URP_STRUC *Urp
)
{
    DEV_INFO *DevInfo;
    DEV_DRIVER *DevDriver;

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR, 0, Urp->ApiData.DevAddr, 0);
    if (DevInfo == NULL) {
        Urp->bRetValue = USB_ERROR;
        return;
    }

    DevDriver = UsbFindDeviceDriverEntry(DevInfo->DevDriverIndex);

    if (DevDriver != NULL) {
        DevDriver->FnDriverRequest(DevInfo, Urp);
    }
}

/**
    This routine is called, from host controllers that supports
    OS handover functionality, when OS wants the BIOS to hand-over 
    the host controllers to the OS.  This routine will stop HC that 
    does not support this functionality.

    @param  None

    @retval None

**/

VOID
EFIAPI
USB_StopUnsupportedHc(
    VOID
)
{


    UINT8       Index;
    HC_STRUC*   HcStruc;
    EFI_STATUS Status;
    
    Status = AmiUsbGlobalDataValidation(gUsbData);

    if (Status != EFI_ABORTED){
        ASSERT_EFI_ERROR(Status);
    
        if (EFI_ERROR(Status)) {
            gLockHwSmiHandler = TRUE;
            gLockSmiHandler = TRUE;
            return;
        }
    


        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Stopping all external HCs");
        for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
            HcStruc = gHcTable[Index];
            if (HcStruc == NULL) {
                continue;
            }
            if (HcStruc->HwSmiHandle != NULL) {
                continue;
            }
            if (HcStruc->HcType == USB_HC_XHCI) {
                if (!(HcStruc->HcFlag & HC_STATE_EXTERNAL)) {
                    continue;
                }
            }
            if (HcStruc->HcFlag & HC_STATE_RUNNING) {

                AmiUsbControllerStop(HcStruc);
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, ".");
            }
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");
    }


    if((gUsbDataList->UsbSetupData->UsbXhciHandoff) &&
      ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT)) {
        StopControllerType(USB_HC_XHCI);
    }

    if((gUsbDataList->UsbSetupData->UsbEhciHandoff) &&
       ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT)) {
        gUsbData->HandOverInProgress = TRUE;
        StopControllerType(USB_HC_EHCI);
    }

    if((gUsbDataList->UsbSetupData->UsbOhciHandoff) &&
       ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT)) {
        StopControllerType(USB_HC_OHCI);
    }

    if (Status != EFI_ABORTED){
        // Disable kbc emulation trap if we don't own any xhci controller.
        for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
            HcStruc = gHcTable[Index];
            if (HcStruc == NULL) {
                continue;
            }
            if ((HcStruc->HcType == USB_HC_XHCI) && (HcStruc->HcFlag & HC_STATE_RUNNING)) {
                break;
            }
        }
        
        if (Index == gUsbDataList->HcTableCount) {
            if (gEmulationTrap) {
                gEmulationTrap->TrapDisable(gEmulationTrap);
            }
        }
    }
   
}

/**
    This function is part of the USB BIOS API. This function
    updates the global variables according to the new owner

    @param fpURP   Pointer to the URP structure

    @retval Return code USB_ERROR - Failure, USB_SUCCESS - Success

    @note  It is a caller responsibility to release the keyboard only if it
              was previously acquired.

**/

VOID
EFIAPI
USBAPI_ChangeOwner(URP_STRUC *Urp)
{
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBAPI_ChangeOwner..");

    if(Urp->ApiData.Owner) {  // Changing to Efi driver
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Urp->ApiData.Owner=%d\n", Urp->ApiData.Owner);
        if(gEmulationTrap) {
            gEmulationTrap->TrapDisable(gEmulationTrap);
        }
        gUsbData->UsbStateFlag |= USB_FLAG_RUNNING_UNDER_EFI;
    } else {    // Acquiring - check the current condition first
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Urp->ApiData.Owner=%d...", Urp->ApiData.Owner);
        if(gEmulationTrap) {
            gEmulationTrap->TrapEnable(gEmulationTrap);
        }

        if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) {
            //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_FLAG_RUNNING_UNDER_EFI\n");
            gUsbData->UsbStateFlag &= ~USB_FLAG_RUNNING_UNDER_EFI;
        } else {
            //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "not USB_FLAG_RUNNING_UNDER_EFI\n");
        }
    }
    Urp->bRetValue = USB_SUCCESS;
}

/**
    This function is part of the USB BIOS API. This function 
    starts/stops the USB host controller.

    @param  Urp     Pointer to the URP structure

    @retval None

**/

VOID
EFIAPI
USBAPI_HcStartStop(URP_STRUC *Urp)
{
    HC_STRUC*   HcStruc;
    UINTN       Index;


    if (Urp->ApiData.HcStartStop.Start == REGISTER_USB_SMI){
      // trigger SMI to register SMI for all Host controllers.
      gCheckUsbApiParameter = FALSE;
      Urp->bRetValue = UsbHcRegisterSmi();  
      return;
    }
      
    HcStruc = Urp->ApiData.HcStartStop.HcStruc;

    if (HcStruc == NULL) {
        return;
    }
    
    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        if (HcStruc == gHcTable[Index]) {
            break;
        }
    }

    if (Index == gUsbDataList->HcTableCount) {
        return;
    }

    if (!(HcStruc->HcFlag & HC_STATE_USED)) {
        return;
    }

    switch (HcStruc->HcType) {
        case USB_HC_UHCI:
            if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == 0){
                return;
            }
            break;
        case USB_HC_OHCI:
            if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == 0){
                return;
            }
            break;
        case USB_HC_EHCI:
            if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == 0){
                return;
            }
            break;
        case USB_HC_XHCI:
            if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == 0){
                return;
            }
            break;
        default:
            return;
    } 

    gCheckUsbApiParameter = FALSE;

    switch (Urp->ApiData.HcStartStop.Start) {
        case STOP_HOST_CONTROLLER:
            Urp->bRetValue = UsbHcStop(HcStruc);
            return;
        case START_HOST_CONTROLLER:
            Urp->bRetValue = UsbHcStart(HcStruc);
            return; 
        default:
            return;
    }
}

/**
    This function is part of the USB BIOS API. This function 
    locks Usb SW SMI.

    @param  Urp   Pointer to the URP structure

    @retval None

**/

VOID
EFIAPI
UsbApiLockUsbSwSmi(
    URP_STRUC *Urp
)
{
    // Switch Global data to SMM data.
    gCheckUsbApiParameter = FALSE;
    if (Urp->bSubFunc == USB_DATA_DXE_TO_SMM){
        AmiUsbGlobalDataSwitch( &gUsbData, 
                                &gUsbDataList, 
                                (HC_STRUC**)&gHcTable,
                                &gDataInSmm, 
                                USB_DATA_DXE_TO_SMM);     
    }
    gLockSmiHandler = TRUE;
    SettingUsbSmi(FALSE);
}

/**
    Invokes procedure passing parameters supplied in the buffer
    It replicates the stack frame so that target procedure can
    see the parameters passed to the stub.

    @retval Returns result of invoked proc

**/

//
// The following typedef corresponds to the min width type that can be passed
// into function call as a parameter without padding
//
typedef UINTN STACKWORD;

UINTN 
USBAPI_invoke_in_frame(
    VOID* pProc,
    VOID* buffer,
    UINT32 ParamSize )
{
    STACKWORD* params = (STACKWORD*)buffer;

    switch(ParamSize/sizeof(STACKWORD)){
    case 0: return ((STACKWORD (*)())pProc)();
    case 1: return ((STACKWORD (*)(STACKWORD))pProc)(params[0]);
    case 2: return ((STACKWORD (*)(STACKWORD,STACKWORD))pProc)(params[0],
                params[1]);
    case 3: return ((STACKWORD (*)(STACKWORD,STACKWORD,STACKWORD))pProc)(
                params[0],params[1],params[2]);
    case 4: return ((STACKWORD (*)(STACKWORD,STACKWORD,STACKWORD,
                STACKWORD))pProc)(
                params[0],params[1],params[2],params[3]);
    case 5: return ((STACKWORD (*)(STACKWORD,STACKWORD,STACKWORD,STACKWORD,
                STACKWORD))pProc)(
                params[0],params[1],params[2],params[3],params[4]);
    case 6: return ((STACKWORD (*)(STACKWORD,STACKWORD,STACKWORD,STACKWORD,
                STACKWORD,STACKWORD))pProc)(
                params[0],params[1],params[2],params[3],params[4],params[5]);
    case 7: return ((STACKWORD (*)(STACKWORD,STACKWORD,STACKWORD,STACKWORD,
                STACKWORD,STACKWORD,STACKWORD))pProc)(
                params[0],params[1],params[2],params[3],params[4],params[5],
                params[6]);
    default:
        ASSERT(ParamSize/sizeof(STACKWORD) < 4);
        return 0;
    }
/*  kept for reference
    __asm {
        push    ecx
        push    esi
        pushf
                        //Copy stak frame
        std
        mov     esi, buffer
        mov     ecx, ParamSize
        add     esi, ecx
        sub     esi, 4
        shr     ecx, 2
loop1:
        lodsd   //DWORD PTR ds:edi
        push    eax
        loop    loop1
                        //Call proc
        mov     eax, pProc
        cld
        call    eax
                        //Read return value
        mov     RetVal, eax

                        //Restore stack and registers
        add     esp, ParamSize
        popf
        pop     esi
        pop     ecx
    }
    return RetVal;*/
}

/**
    Bridge to a number of procedures supplied by HC driver


    @param Urp   Pointer to the URP structure

    @retval USB_ERROR  Failure
    @retval USB_SUCCESS  Success

    @note  
      Assumes that buffer has a correct image of the stack that
      corresponding function reads argument from
      Size of the buffer can be bigger than actually used.

      Following code copies the buffer (some stack frame) into new
      stack frame such that invoked dirver proc can read parametes
      supplied by buffer
**/

VOID 
EFIAPI 
USBAPI_HC_Proc(URP_STRUC *Urp)
{
    VOID* buffer = Urp->ApiData.HcProc.ParamBuffer;
    UINT32 ParamSize = // align size on DWORD
        (Urp->ApiData.HcProc.ParamSize + 3) & ~0x3;
    UN_HCD_HEADER* pHdr;

    ASSERT( GET_HCD_INDEX(Urp->ApiData.HcProc.HcType) <
        sizeof(gUsbHcdTable)/sizeof(HCD_HEADER));

   
    ASSERT( Urp->bSubFunc < sizeof(pHdr->asArray.proc)/sizeof(VOID*));

    switch (Urp->ApiData.HcProc.HcType) {
        case USB_HC_UHCI:
            if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == 0){
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid HcType %x\n", Urp->ApiData.HcProc.HcType);
                return;
            }
            break;
        case USB_HC_OHCI:
            if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == 0){
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid HcType %x\n", Urp->ApiData.HcProc.HcType);
                return;
            }
            break;
        case USB_HC_EHCI:
            if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == 0){
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid HcType %x\n", Urp->ApiData.HcProc.HcType);
                return;
            }
            break;
        case USB_HC_XHCI:
            if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == 0){
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid HcType %x\n", Urp->ApiData.HcProc.HcType);
                return;
            }
            break;
        default:
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid HcType %x\n", Urp->ApiData.HcProc.HcType);
            return;
    } 

    pHdr = (UN_HCD_HEADER*)(gUsbHcdTable + GET_HCD_INDEX(Urp->ApiData.HcProc.HcType));

    if ((Urp->bSubFunc >= (sizeof(pHdr->asArray.proc)/sizeof(VOID*))) || (pHdr->asArray.proc[Urp->bSubFunc] == NULL)) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_HC_Proc: Invalid SubFunc#%x\n", Urp->bSubFunc);
        return;
    }

    Urp->ApiData.HcProc.RetVal = USBAPI_invoke_in_frame(
        pHdr->asArray.proc[Urp->bSubFunc],buffer,ParamSize);
    Urp->bRetValue = USB_SUCCESS;
}


/**
    Bridge to a number of procedures supplied by Core proc table

    @param Urp   Pointer to the URP structure

    @retval USB_ERROR  Failure
    @retval USB_SUCCESS  Success

    @note  
      Assumes that buffer has a correct image of the stack that
      corresponding function reads argument from
      Size of the buffer can be bigger than actually used.

      Following code copies the buffer (some stack frame) into new
      stack frame such that invoked proc can read parameters
      supplied by buffer
**/


VOID* CORE_PROC_TABLE[] = {
        USB_GetDescriptor,
        USB_ReConfigDevice,
        USB_ReConfigDevice2,
        UsbAllocDevInfo,
        PrepareForLegacyOs,
        USB_ResetAndReconfigDev,
        USB_DevDriverDisconnect,
    };

VOID 
EFIAPI 
USBAPI_Core_Proc(URP_STRUC *Urp)
{
    VOID* buffer = Urp->ApiData.CoreProc.ParamBuffer;
    UINT32 ParamSize = // align size on DWORD
        (Urp->ApiData.CoreProc.ParamSize + 3) & ~0x3;

    ASSERT( Urp->bSubFunc < COUNTOF(CORE_PROC_TABLE));

    if (Urp->bSubFunc >= COUNTOF(CORE_PROC_TABLE)) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USBAPI_Core_Proc: Invalid bSubFunc#%x\n", Urp->bSubFunc);
        return;
    }

    Urp->ApiData.CoreProc.RetVal = USBAPI_invoke_in_frame(
        CORE_PROC_TABLE[Urp->bSubFunc],buffer,ParamSize);

    Urp->bRetValue = USB_SUCCESS;
}


//----------------------------------------------------------------------------
//          USB API Procedures Ends
//----------------------------------------------------------------------------


/**
    This routine converts the sense data information into
    ATAPI error code

    @param SenseData  Sense data obtained from the device

    @retval BYTE      ATAPI error code

**/

UINT8 USBWrapGetATAErrorCode (UINT32 SenseData)
{
    UINT8   sc = (UINT8)SenseData;             // Sense code
    UINT8   asc = (UINT8)(SenseData >> 16);    // Additional Sense Code (ASC)
    UINT8   ascq = (UINT8)(SenseData >> 8);    // Additional Sense Code Qualifier (ASCQ)

    if (ascq == 0x28) return USB_ATA_DRIVE_NOT_READY_ERR;
    if (sc == 7) return USB_ATA_WRITE_PROTECT_ERR;
    if ((asc == 0x80) && (ascq == 0x80)) return USB_ATA_TIME_OUT_ERR;
    if (ascq == 0x18) return USB_ATA_DATA_CORRECTED_ERR;
    if ((ascq==6) && (asc == 0)) return USB_ATA_MARK_NOT_FOUND_ERR;
    if ((ascq==0x3a) && (asc == 0)) return USB_ATA_NO_MEDIA_ERR;
    if ((ascq==0x11) && (asc == 0)) return USB_ATA_READ_ERR;
    if ((ascq==0x11) && (asc == 6)) return USB_ATA_UNCORRECTABLE_ERR;
    if (ascq==0x30) return USB_ATA_BAD_SECTOR_ERR;
    if ((ascq<0x20) || (ascq>0x26)) return USB_ATA_GENERAL_FAILURE;

    return USB_ATA_PARAMETER_FAILED;
}


/**
    This routine finds the 'n'th device's DeviceInfo entry.

    @param DevNumber  Device number (1-based)

    @retval DeviceInfo structure

**/

DEV_INFO*
USBWrap_GetnthDeviceInfoStructure(UINT8 DevNumber)
{
    return &gUsbDataList->DevInfoTable[DevNumber];
}


/**
    This routine searches through the device entry table
    and returns number of active USB devices configured
    by the BIOS.

    @param Urp    Pointer to the URP

    @retval Following fields in the URP are modified
        CkPresence.bNumBootDev      Number of USB boot devices found
        CkPresence.bNumKeyboards    Number of USB keyboards present
        CkPresence.bNumMice         Number of USB mice present
        CkPresence.bNumHubs         Number of USB hubs present
        CkPresence.bNumStorage      Number of USB storage devices present

**/

VOID
USBWrap_GetDeviceCount(URP_STRUC *Urp)
{
    DEV_INFO    *FpDevInfo;
    UINT8       Index;

    for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
        FpDevInfo   = &gUsbDataList->DevInfoTable[Index];

        if ( (FpDevInfo->Flag & DEV_INFO_VALID_STRUC) &&
            (FpDevInfo->Flag & DEV_INFO_DEV_PRESENT)) {
            Urp->ApiData.CkPresence.NumBootDev++;

            switch (FpDevInfo->DeviceType) {
    case BIOS_DEV_TYPE_HID:
                    if (FpDevInfo->HidDevType & HID_DEV_TYPE_KEYBOARD) {
                        Urp->ApiData.CkPresence.NumKeyboards++;
                    }
                    if (FpDevInfo->HidDevType & HID_DEV_TYPE_MOUSE) {
                        Urp->ApiData.CkPresence.NumMice++;
                    }
                    if (FpDevInfo->HidDevType & HID_DEV_TYPE_POINT) {
                        Urp->ApiData.CkPresence.NumPoint++;
                    }
     break;

                case  BIOS_DEV_TYPE_HUB:
                      Urp->ApiData.CkPresence.NumHubs++;
                            break;
                case  BIOS_DEV_TYPE_STORAGE:
                      Urp->ApiData.CkPresence.NumStorage++;
                            break;
                case  BIOS_DEV_TYPE_SECURITY:
                            break;
            }
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
