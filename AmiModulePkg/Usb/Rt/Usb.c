#pragma warning(disable: 4001)
#pragma warning(disable: 4127)
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

/** @file Usb.c
    AMI USB main wrapper

**/
#include "AmiUsb.h"
#include <Library/AmiUsbHcdLib.h>
#include <Library/TimerLib.h>
BOOLEAN     gKeyRepeatStatus = FALSE;
extern BOOLEAN  gCheckUsbApiParameter;

BOOLEAN  EFIAPI OEMSkipList(UINT8,UINT8,UINT16,UINT8,UINT8);  
VOID EFIAPI UsbOemCheckNonCompliantDevice(HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16);

UINT8   USBHub_EnablePort(HC_STRUC*, UINT8, UINT8);
UINT8   USBHub_DisablePort(HC_STRUC*, UINT8, UINT8);
UINT8   USBHub_ResetPort(HC_STRUC*, UINT8, UINT8);
UINT32  USBHub_GetPortStatus(HC_STRUC*, UINT8, UINT8, BOOLEAN);


VOID    EFIAPI SpeakerBeep (UINT8, UINT16, HC_STRUC*);
VOID    EFIAPI FixedDelay(UINTN);
VOID USBAPI_CheckDevicePresence (URP_STRUC*);

extern  USB_BADDEV_STRUC gUsbBadDeviceTable[];

DEV_INFO*   EFIAPI USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);
VOID*       USB_MemAlloc(UINT16);
UINT8       USB_MemFree (VOID*, UINT16);
UINT8*      USB_GetDescriptor (HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT8 , UINT8);
UINT8       USB_SetAddress(HC_STRUC*,DEV_INFO*, UINT8);
DEV_INFO*   USBIdentifyAndConfigureDevice (HC_STRUC* , DEV_INFO* , UINT8* , UINT16 ,UINT16 );
UINT8       USB_DisconnectDevice (HC_STRUC*, UINT8, UINT8 );
VOID        USB_InitFrameList (HC_STRUC*, UINT32);
DEV_DRIVER* UsbFindDeviceDriverEntry(UINTN DevDriverIndex);

UINT8   USB_MemInit (VOID);
UINT8   USBInitHostController(UINT16 *, UINT8);
UINT8   USB_EnumerateRootHubPorts(UINT8);
UINT8   USBLogError(UINT16);
BOOLEAN CheckDeviceLimit(UINT8);

VOID    EFIAPI USBKeyRepeat(HC_STRUC*,UINT8);
extern  USB_GLOBAL_DATA  *gUsbData;  // Defined in AMIUHCD

CALLBACK_FUNC   gAmiUsbCallBackFunTbl[MAX_CALLBACK_FUNCTION];
extern URP_STRUC     *gParameters;
extern USB_DATA_LIST *gUsbDataList;
extern HC_STRUC      **gHcTable;
extern UINT8         gDataInSmm;


/**
    This function initializes the USB host controllers and
    enumerate the root hub ports for possible USB devices.

    @param  None

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_StartHostControllers(
    VOID
)
{
    return USB_SUCCESS;
}

/**
    This function stops all USB host controllers of a given type

    @param  HcType          Host controller type
    @retval None
**/

VOID
StopControllerType(
    UINT8 HcType
)
{
    UINT8 i;
    HC_STRUC*   HcStruc;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "stopping all HC type %x:", HcType);
    for (i = 0; i < gUsbDataList->HcTableCount; i++) {
        HcStruc = gHcTable[i];
        if (HcStruc == NULL) {
            continue;
        }
        if ((HcStruc->HcType == HcType) &&
            (HcStruc->HcFlag & HC_STATE_RUNNING)) {
            AmiUsbControllerStop(HcStruc);
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, ".");
        }
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");
}

/**
    This function start all USB host controllers of a given type

    @param  HcType          Host controller type
    @retval None

**/

VOID
StartControllerType(
    UINT8 HcType
)
{
    UINT8 Index;
    HC_STRUC*   HcStruc;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "starting all HC type %x:", HcType);
    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if ((HcStruc->HcFlag & (HC_STATE_USED | HC_STATE_INITIALIZED)) 
                != (HC_STATE_USED | HC_STATE_INITIALIZED)) {
            continue;
        }
        if ((HcStruc->HcType == HcType)) {
            AmiUsbControllerStart(HcStruc);
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, ".");
        }
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\n");
}

/**
    This function stops the USB host controllers of a given Bus Dev Function

    @param BusDevFuncNum       Pci/Pcie (Bus, device, function)
    @retval None
**/

VOID
StopControllerBdf(
    UINT16 BusDevFuncNum
)
{
    UINT8 Index;
    HC_STRUC*   HcStruc;

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if ((HcStruc->BusDevFuncNum == BusDevFuncNum) &&
            (HcStruc->HcFlag & HC_STATE_RUNNING)) {
            AmiUsbControllerStop(HcStruc);
            break;
        }
    }
}


/**
    This function stops the USB host controllers and
    frees the data structures associated with the host controllers
    In case of USB2.0 first stop USB1.1 controllers, then USB2.0.

    @param  None
        
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_StopHostControllers(
    VOID
)
{
    // Suppress disconnect beeps as they might be confusing
    gUsbData->UsbStateFlag  &= (~USB_FLAG_ENABLE_BEEP_MESSAGE);

    if ((gUsbData->UsbFeature & USB_HC_XHCI_SUPPORT) == USB_HC_XHCI_SUPPORT)
        StopControllerType(USB_HC_XHCI);

    if ((gUsbData->UsbFeature & USB_HC_UHCI_SUPPORT) == USB_HC_UHCI_SUPPORT)
        StopControllerType(USB_HC_UHCI);

    if ((gUsbData->UsbFeature & USB_HC_OHCI_SUPPORT) == USB_HC_OHCI_SUPPORT)
        StopControllerType(USB_HC_OHCI);

    if ((gUsbData->UsbFeature & USB_HC_EHCI_SUPPORT) == USB_HC_EHCI_SUPPORT)
        StopControllerType(USB_HC_EHCI);

    return USB_SUCCESS;
}


/**
   Register USB SMI for all host controllers which recorded in gHcTable.

    @param None
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
UsbHcRegisterSmi(
  VOID      
  )
{
    UINTN     Index;
    EFI_STATUS Status;

    if (gUsbDataList->HcTableCount == 0) return USB_SUCCESS;

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
      if (gHcTable[Index] == NULL) continue;
      Status = InstallSmmHandle(gHcTable[Index]);
      USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB: hc#%x: InstallSmmHandle(%r)\n",Index, Status);
    }
    return USB_SUCCESS;      
}


/**
    This function initializes the USB host controller and
    enumerate the root hub ports for possible USB devices.

    @param HcStruc         HC struc pointer

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
UsbHcStart(HC_STRUC* HcStruc)
{
    UINT8       Index;
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Starting HC %X, HCNum %d, type %x\n",
        HcStruc->BusDevFuncNum, HcStruc->HcNumber, HcStruc->HcType);

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        if (gHcTable[Index] == NULL) {
            continue;
        }
        if (HcStruc == gHcTable[Index]) {
            continue;
        }
        if (HcStruc->FrameList != NULL) {
            if (HcStruc->FrameList == gHcTable[Index]->FrameList) {
                break;
            }
        }
        if (HcStruc->Usb3HcData != NULL) {
            if (HcStruc->Usb3HcData == gHcTable[Index]->Usb3HcData) {
                break;
            }
        }

        if ((gUsbData->UsbRuntimeDriverInSmm) &&
           (HcStruc->BusDevFuncNum == gHcTable[Index]->BusDevFuncNum)) {
            break;
        }

    }

    if (Index != gUsbDataList->HcTableCount) {
        return USB_ERROR;
    }

    HcStruc->HcFlag |= HC_STATE_INITIALIZED;
    
    AmiUsbControllerStart(HcStruc);
    
    // Check if the HC is running
    if ((HcStruc->HcFlag & HC_STATE_RUNNING) == 0) {
        HcStruc->HcFlag &= ~HC_STATE_INITIALIZED;
        return USB_ERROR;
    }
    for (Index = 0; Index < MAX_USB_CONTROLLER_DELAY_LIST_COUNT; Index++) {
        if ((gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].Vid == 0) &&
            (gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].Did == 0)) {
            break;
        }       
        if ((gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].Vid == HcStruc->Vid) &&
            (gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].Did == HcStruc->Did)) {
            if (gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].DelayTime) {
                FixedDelay(gUsbDataList->UsbTimingPolicy->ControllersDelayList[Index].DelayTime * 1000);
            }
            break;
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Enumerating HC#%d, type 0x%x\n", HcStruc->HcNumber, HcStruc->HcType);
    // Issue enumerate call for this HC
    AmiUsbEnumeratePorts(HcStruc);

    if (gUsbData->UsbRuntimeDriverInSmm == USB_RUNTIME_DRV_MODE_2) {
        AmiUsbSmiControl(HcStruc, FALSE);
    }

    return USB_SUCCESS;
}

/**
    This function stops the USB host controller.

    @param HcStruc           HC struc pointer

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
UsbHcStop(HC_STRUC* HcStruc)
{
    if ((HcStruc->HcFlag & HC_STATE_RUNNING) == 0) {
        return USB_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Stopping HC %X, HCNum %d, type %x\n", 
         HcStruc->BusDevFuncNum, HcStruc->HcNumber, HcStruc->HcType);
    AmiUsbControllerStop(HcStruc);
    return USB_SUCCESS;
}

/**
    This function checks bios owned hc. Clear USB_FLAG_DRIVER_STARTED
    if we don't start any host controller.

    @param  None
    @retval None

**/

VOID
CheckBiosOwnedHc(
    VOID
)
{
    UINT8       Index;
    HC_STRUC    *HcStruc;

    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
            continue;
        }
        if (HcStruc->HcFlag & HC_STATE_RUNNING) {
            return;
        }
    }
    
    gUsbData->UsbStateFlag &= ~(USB_FLAG_DRIVER_STARTED);

    return;
}


/**
    This function enumerates the root hub ports of the all
    selected type HCs

    @param HcStruc           HC struc pointer

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_EnumerateRootHubPorts(
    UINT8 Type
)
{
    UINT8    Index;
    HC_STRUC*   HcStruc;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "Enumerating HC Ports.\n");
    for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
        //
        // Get the HCStruc pointer associated with this controller
        //
        HcStruc = gHcTable[Index];
        if (HcStruc == NULL) {
              continue;
        }

        if((HcStruc->HcType) == Type && (HcStruc->HcFlag & HC_STATE_RUNNING)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Enumerating HC#%d, type 0x%x\n", HcStruc->HcNumber, Type);
            //
            // Issue enumerate call for this HC
            //
            AmiUsbEnumeratePorts(HcStruc);
        }
    }
    return USB_SUCCESS;
}


/**
    This function initializes the specified type of the HC
    from the provided list of host controller PCI addresses

    @param pHCPCIAddrList  Pointer to table of HC PCI addresses in the system
    @param HcType         Type of HC to be initialized (EHCI, OHCI etc)

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USBInitHostController(
    UINT16  *HcPciAddrList,
    UINT8   HcType
)
{
    return USB_SUCCESS;
}


/**
    This function returns the hub port status

    @param HcStruc           HC struc pointer
    @param HubAddr           USB device address of the hub or HC number
                              BIT7 = 1/0  Roothub/Hub
    @param PortNum           Port number
    @param ClearChangeBits   TRUE: Clear change status bits
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT32
USB_GetHubPortStatus (
    HC_STRUC*  HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
)
{

    //
    // Check whether the request is for root hub or generic hub
    //
    if (HubAddr & BIT7) {
    // Root hub
        return AmiUsbGetRootHubStatus(HcStruc, PortNum, ClearChangeBits);
    } else {

        if ((gUsbData->UsbDevSupport & USB_HUB_DEV_SUPPORT) == USB_HUB_DEV_SUPPORT){
            return USBHub_GetPortStatus(HcStruc, HubAddr, PortNum, ClearChangeBits);
        } else {
            return 0;
        }
    }
}

/**
    This function disables the hub port

    @param HcStruc           HC struc pointer
    @param HubAddr           USB device address of the hub or HC number
                              BIT7 = 1/0  Roothub/Hub
    @param PortNum           Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_DisableHubPort(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum
)
{
    //
    // Check whether the request is for root hub or generic hub
    //
    if (HubAddr & BIT7) {
        //
        // Issue the disable root hub call to disable the hub port
        //
        AmiUsbDisableRootHub(HcStruc, PortNum);

    } else {
        if ((gUsbData->UsbDevSupport & USB_HUB_DEV_SUPPORT) == USB_HUB_DEV_SUPPORT)
            USBHub_DisablePort(HcStruc, HubAddr, PortNum);

    }
    return USB_SUCCESS;
}


/**
    This function enables the hub port

    @param HcStruc    HC struc pointer
    @param HubAddr    USB device address of the hub or HC number
                      BIT7 = 1/0  Roothub/Hub
    @param PortNum    Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_EnableHubPort (
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum
)
{
    //
    // Check whether the request is for root hub or generic hub
    //
    if (HubAddr & BIT7) {
        //
        // Root hub
        // Issue the disable root hub call to disable the hub port
        //

        return AmiUsbEnableRootHub(HcStruc, PortNum);
    } else {
        if ((gUsbData->UsbDevSupport & USB_HUB_DEV_SUPPORT) == USB_HUB_DEV_SUPPORT){
            return USBHub_EnablePort(HcStruc, HubAddr, PortNum);
        } else {
            return USB_ERROR;   // Only root hub could be successfully enabled
        }
    }
}


/**
    Send get descriptor command to get device BOS descriptor and sore in DevInfo.

    @param HcStruc    HC struc pointer
    @param DevInfo    Device Information

    @retval USB_SUCCESS     Success to get BOS descriptor.
    @retval Others        Failure

**/
EFI_STATUS
GetBosDescriptor(
    HC_STRUC*   HcStruc,    
    DEV_INFO*   DevInfo
){
    
    BOS_DESC        *BosDesc;
    UINT16          TotalLength;
    UINT8           *Buffer = NULL;

    
    Buffer = USB_MemAlloc (GET_MEM_BLK_COUNT(DEFAULT_BOS_DESC_SIZE));
    if (Buffer == NULL) {
         return EFI_BAD_BUFFER_SIZE;
     }
    
    BosDesc = (BOS_DESC*)USB_GetDescriptor(
                           HcStruc,
                           DevInfo,
                           Buffer,
                           0x8,
                           DESC_TYPE_BOS,
                           0
                           );
    if (BosDesc == NULL) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "BOS descriptor not support.\n");     
      return EFI_UNSUPPORTED;
      }
    
    
    TotalLength = BosDesc->TotalLength;
    USB_MemFree(Buffer, (UINT8)(DEFAULT_BOS_DESC_SIZE / sizeof(MEM_BLK)));
    Buffer = USB_MemAlloc (GET_MEM_BLK_COUNT(TotalLength));
    if (Buffer == NULL) {
      return EFI_BAD_BUFFER_SIZE;
    }
     
    BosDesc = (BOS_DESC*)USB_GetDescriptor(
                             HcStruc,
                             DevInfo,
                             Buffer,
                             TotalLength,
                             DESC_TYPE_BOS,
                             0
                             );

    if (BosDesc == NULL) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "BOS descriptor not support.\n");
        return EFI_UNSUPPORTED;
    }
 
    
    DevInfo->BosDescPtr = Buffer;
    DevInfo->BosTotalLength = BosDesc->TotalLength;
    if(DevInfo->BosDescPtr == NULL){
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
    
}

/**
    This function resets the hub port

    @param HcStruc   HC struc pointer
    @param HubAddr   USB device address of the hub or HC number
                     BIT7 = 1/0  Roothub/Hub
    @param PortNum   Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_ResetHubPort (
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum)
{
    //
    // Check whether the request is for root hub or generic hub
    //
    if (HubAddr & BIT7) {
        //
        // Root hub
        // Issue the reset root hub call to reset the hub port
        //

        return AmiUsbResetRootHub(HcStruc, PortNum);
    } else {
        if ((gUsbData->UsbDevSupport & USB_HUB_DEV_SUPPORT) == USB_HUB_DEV_SUPPORT){
            return USBHub_ResetPort (HcStruc, HubAddr, PortNum);
        }else{
            return USB_ERROR;   // Only root hub could be successfully reset
        }
    }
}

/**
    This function checks the port status provided and depending
    on the status it invokes device connect/disconnect routine

    @param HcStruc    Pointer to HCStruc
    @param HubAddr    For root port this is the host controller index
                      in gUsbData->aHCStrucTable combined with BIT7;
                      For devices connected to a hub this is parent
                      hub USB address
    @param HubPort    Parent hub port number
    @param PortStatus Port status read

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

DEV_INFO*
USB_DetectNewDevice(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       HubPort,
    UINT32      PortStatus
)
{
    UINT8           ErrorFlag  = 0;
    UINT16          DescLength;
    UINT8           DevConfigured;
    UINTN           ConfigLevel = 0;
    UINT16          TotalLength;
    UINT8           *Buffer = NULL;
    DEV_INFO        *DevInfo;
    DEV_INFO        *Pointer;
    DEV_DESC        *DevDesc;
    CNFG_DESC       *CnfgDesc;
    INTRF_DESC      *IntrfDesc;
    UINT8           *DevMiscInfo = NULL;
    UINT8           Status;
    BOOLEAN         SkipConnectBeep = FALSE;  
    UINT16          OrgTimeOutValue;     
    UINT8           DeviceAddress;
    USB_DEV_CONFIG_LINK *DevConfigLink = NULL;
    LIST_ENTRY      *Link = NULL;
    UINT8           GetCnfgIndex;
    BOOLEAN         FirstInterfaceConfig = FALSE;
    
    //
    // Get the temporary device info structure pointer (index 0)
    //
    DevInfo = gUsbDataList->DevInfoTable;

    //
    // Fill the necessary entries in the device info
    //
    DevInfo->Flag            = DEV_INFO_VALID_STRUC;
    DevInfo->DeviceAddress   = 0;
    DevInfo->DeviceType      = 0;
    DevInfo->IncompatFlags   = 0;
    DevInfo->DevMiscInfo     = NULL;

    //
    // Fill the hub/host controller information
    //
    DevInfo->HubDeviceNumber = HubAddr;
    DevInfo->HubPortNumber   = HubPort;
    DevInfo->BosDescPtr      = NULL;
    DevInfo->BosTotalLength  = 0;
 
    // Get Device initial Start time.
    if ((gUsbData->UsbFeature2 & USB_PERFORMANCE_INFORMATION) && (!(gUsbData->UsbStateFlag & USB_FLAG_BOOT_EVENT))){
        DevInfo->DetectStartTime = GetPerformanceCounter();
    }
    //
    // Fill the device speed
    //
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_DetectNewDevice: PortStatus = %x\n", PortStatus);
    DevInfo->EndpointSpeed = (PortStatus & USB_PORT_STAT_DEV_SPEED_MASK) >>
                        USB_PORT_STAT_DEV_SPEED_MASK_SHIFT;

    if ((gUsbData->UsbFeature & USB_SHOW_SKIP_PORT_INFORMATION) == 
        USB_SHOW_SKIP_PORT_INFORMATION) {
        UINT8   i;
        DEV_INFO    *TmpDevInfo; 
        TmpDevInfo = DevInfo;   
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "==== SHOW_SKIP_PORT_INFORMATION ==== \n");           
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "BDF %x \nRoutePath = ",HcStruc->BusDevFuncNum);  
        for(i = 0;i < 5;i++) {
            if(TmpDevInfo->HubDeviceNumber & BIT7) {
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "\nRootPort %x \n",TmpDevInfo->HubPortNumber);             
                break;
            }
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%x ",TmpDevInfo->HubPortNumber);           
            TmpDevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR, 0, TmpDevInfo->HubDeviceNumber, 0);     
            if (TmpDevInfo == NULL) {
                break;
            }     
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "==== SHOW_SKIP_PORT_INFORMATION ==== \n");           
    }


    switch (DevInfo->EndpointSpeed) {
        case USB_DEV_SPEED_SUPER_PLUS:
        case USB_DEV_SPEED_SUPER:
            DevInfo->Endp0MaxPacket = 0x200;
            break;
        case USB_DEV_SPEED_HIGH:
            DevInfo->Endp0MaxPacket = 0x40;
            break;
        case USB_DEV_SPEED_FULL:
        case USB_DEV_SPEED_LOW:
            DevInfo->Endp0MaxPacket = 0x08;
            break;
    }   
    //
    // Fill the HC struc index value
    //
    DevInfo->HcNumber = HcStruc->HcNumber;

    ErrorFlag = TRUE;      // Assume as error
    DevConfigured = FALSE;     // No device configured

    //
    // Allocate memory for device requests
    //
    ConfigLevel = USB_ERR_DEV_INIT_MEM_ALLOC;   // For proper error handling
    Buffer = USB_MemAlloc (GET_MEM_BLK_COUNT(MAX_CONTROL_DATA_SIZE));
    if (Buffer == NULL) {
        SpeakerBeep(8, 0x2000, HcStruc);
        return (DEV_INFO*)ConfigLevel;
    }
    ConfigLevel = USB_ERR_DEV_INIT_GET_DESC_8;

    // Initialize HC specific data before device configuration
    Status = AmiUsbInitDeviceData(HcStruc, DevInfo, PortStatus, &DevMiscInfo);

  if(Status != USB_SUCCESS) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to initialize HC specific data for the device\n");
        USB_MemFree(Buffer, (UINT8)(MAX_CONTROL_DATA_SIZE / sizeof(MEM_BLK)));
        SpeakerBeep(8, 0x2000, HcStruc);
        return (DEV_INFO*)ConfigLevel;
    }
    DevInfo->DevMiscInfo = (VOID*)DevMiscInfo;

//
// Next send a GetDescriptor command to the device to get its Device
// Descriptor. Assume a MaxPacket size of 64 bytes (the device will use 8,
// 16, 32, or 64). Regardless of the packet size used by te device we can
// always get the real MaxPacket size that the device is using, because
// this piece of information is at offset 7 in the device descriptor.
//
    OrgTimeOutValue = gUsbData->UsbReqTimeOutValue;
    gUsbData->UsbReqTimeOutValue = 1000;

    DevDesc = (DEV_DESC*)USB_GetDescriptor(
                            HcStruc,
                            DevInfo,
                            Buffer,
                            8,
                            DESC_TYPE_DEVICE,
                            0);

    gUsbData->UsbReqTimeOutValue = OrgTimeOutValue;
    if (DevDesc == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the first 8 bytes of the device descriptor.\n");
        goto detection_complete;
    }

    //
    // Get and store the endpoint 0 max packet size
    //
    ConfigLevel = USB_ERR_DEV_INIT_SET_ADDR;
    //
    // Endpoint 0 max packet size check.
    // CyQ've USB modem(Model:MQ4UFM560) return invalid device descriptor after 
    // warm reset.
    //
    if (DevDesc->BcdUsb >= 0x0300) {
        DevInfo->Endp0MaxPacket = (UINT16)1 << DevDesc->MaxPacketSize0;
    } else {
        DevInfo->Endp0MaxPacket = (DevDesc->MaxPacketSize0)?
                (UINT16)DevDesc->MaxPacketSize0 : 0x40;
    }
    if((DevInfo->EndpointSpeed == USB_DEV_SPEED_LOW) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_FULL) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_HIGH)){
        FixedDelay(10 * 1000);     // 10msec delay
    }

    //To assign an address to a USB device, the USB device transitions the state 
    //from the Default to the Address state.
    for (DeviceAddress = 1; DeviceAddress < 64; DeviceAddress++) {
        if (gUsbData->DeviceAddressMap & LShiftU64(1, DeviceAddress)) {
            break;
        }
    }

    if (DeviceAddress == 64) {
        goto detection_complete;
    }
    Status = USB_SetAddress(HcStruc, DevInfo, DeviceAddress);
    if (Status == USB_ERROR) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to set device address.\n");
        goto detection_complete;
    }
    gUsbData->DeviceAddressMap &= ~(LShiftU64(1, DeviceAddress));
    DevInfo->DeviceAddress = DeviceAddress;
    FixedDelay(2 * 1000);
    
    //
    // Now send a GetDescriptor command to the device to get its device descriptor.
    //
    DevDesc = (DEV_DESC*)USB_GetDescriptor(
                            HcStruc,
                            DevInfo,
                            Buffer,
                            18,
                            DESC_TYPE_DEVICE,
                            0);

    //ASSERT(fpDevDesc != NULL);
    if (DevDesc == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the completed device descriptor.\n");
        goto detection_complete;
    }
    // If a descriptor returns with a value in its length field that is 
    // less than defined by USB specification, the descriptor is invalid.
    if (DevDesc->DescLength < 18) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "The device descriptor is invalid, the length is less than 18.\n");
        goto detection_complete;
    }
    if (DevDesc->NumConfigs == 0) {
        USB_DEBUG(DEBUG_WARN, DEBUG_LEVEL_3, "Number of possible configurations is 0.\n");
        DevDesc->NumConfigs = 1;
    }

    CopyMem((UINT8*)&DevInfo->DevDesc, DevDesc, sizeof(DEV_DESC));

    ConfigLevel = USB_ERR_DEV_INIT_GET_DESC_200;
    //
    // Get the relevant information from the descriptor and store it in
    // device information struture
    //
    DevInfo->VendorId    = DevDesc->VendorId;
    DevInfo->DeviceId    = DevDesc->DeviceId;
//
// Look at each of the device's ConfigDescriptors and InterfaceDescriptors
// until an InterfaceDescriptor is found with BaseClass, SubClass, and
// Protocol fields indicating boot keyboard, mouse, hub or storage support.
//
    GetCnfgIndex = 0;

    for (Link = gUsbData->DevConfigInfoList.ForwardLink;
        Link != (VOID*)&gUsbData->DevConfigInfoList; 
        Link = Link->ForwardLink ) {
        DevConfigLink = BASE_CR(Link, USB_DEV_CONFIG_LINK, Link);
        if (DevConfigLink->DevConfigInfo.Vid == DevInfo->VendorId) {
            if (DevConfigLink->DevConfigInfo.Did == DevInfo->DeviceId) {
                GetCnfgIndex = DevConfigLink->DevConfigInfo.Config - 1;  // zero base
                break;
            }
        }
    }

    if (DevInfo->EndpointSpeed == USB_DEV_SPEED_FULL) {
        FixedDelay(100);
    }



    if ((gUsbData->UsbFeature2 & USB_GET_BOS_DESC_SUPPORT)== USB_GET_BOS_DESC_SUPPORT){
       if (DevInfo->DevDesc.BcdUsb >= 0x0300){
          GetBosDescriptor(HcStruc, DevInfo);
        }
    }

    do {    // For processing multiple configurations
        CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(
                        HcStruc,
                        DevInfo,
                        Buffer,
                        0xFF,
                        DESC_TYPE_CONFIG,
                        GetCnfgIndex);
        if (CnfgDesc == NULL) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the configuration descriptor\n");
            break;
        }
        TotalLength = CnfgDesc->TotalLength;
        if (TotalLength > 0xFF) {
            if(TotalLength > (MAX_CONTROL_DATA_SIZE - 1)) {
                TotalLength = MAX_CONTROL_DATA_SIZE - 1;
            }
            CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(
                        HcStruc,
                        DevInfo,
                        Buffer,
                        TotalLength,
                        DESC_TYPE_CONFIG,
                        GetCnfgIndex);

            if (CnfgDesc == NULL) {
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the configuration descriptor, the total length is %x\n", TotalLength);
                break;
            }
        }
  
        if (DevInfo->EndpointSpeed == USB_DEV_SPEED_FULL) {
            FixedDelay(100);
        }
//
// CnfgDesc should now point to a ConfigDescriptor.  Verify this and
// then get some fields out of it.  Then point to the next descriptor.
//
        if (CnfgDesc->DescType == DESC_TYPE_CONFIG) {
            // Some devices might return the wrong length of the configuration 
            // descriptor, we force the length filed to 0x09. 
            if (CnfgDesc->DescLength == 0) {
                USB_DEBUG(DEBUG_WARN, DEBUG_LEVEL_3, "The length of the configuration descriptor is 0.\n");
                CnfgDesc->DescLength = 0x09;
            }

            AmiUsbEnableEndpoints(HcStruc, DevInfo, (UINT8*)CnfgDesc);

            DescLength = (UINT8)CnfgDesc->DescLength;
            DevInfo->ConfigNum = CnfgDesc->ConfigValue;

            // Check if the device has alternate setting for the interface.
            for (;DescLength < TotalLength;) {
                //
                // CnfgDesc should now point to an InterfaceDescriptor.  Verify this
                // and then check its BaseClass, SubClass, and Protocol fields for
                // usable devices.
                //
                IntrfDesc = (INTRF_DESC*)((UINT8*)CnfgDesc + DescLength);
                if ((IntrfDesc->DescLength == 0) || 
                   ((IntrfDesc->DescLength + DescLength) > TotalLength)) {
                   break;
                }
                if ((IntrfDesc->DescType == DESC_TYPE_INTERFACE) && (IntrfDesc->AltSettingNum != 0)) {
                    DevInfo->Flag |= DEV_INFO_ALT_SETTING_IF;
                    break;
                }
                if (IntrfDesc->DescLength) {
                    DescLength += (UINT16)IntrfDesc->DescLength;
                } else {
                    break;
                }
            }

            DescLength = (UINT8)CnfgDesc->DescLength;

            for (;DescLength < TotalLength;) {
                //
                // IntrfDesc should now point to an InterfaceDescriptor.  Verify this
                // and then check its BaseClass, SubClass, and Protocol fields for
                // usable devices.
                //
                IntrfDesc = (INTRF_DESC*)((UINT8*)CnfgDesc + DescLength);
                if ((IntrfDesc->DescLength == 0) || 
                    ((IntrfDesc->DescLength + DescLength) > TotalLength)) {
                    break;
                }
                if ((IntrfDesc->DescType == DESC_TYPE_INTERFACE) && (IntrfDesc->AltSettingNum == 0)) {
                    
                    DevInfo->InterfaceNum    = IntrfDesc->InterfaceNum;
                    DevInfo->AltSettingNum   = 0;
                    
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBIdentifyAndConfigureDevice:: %04x/%04x InterfaceNum %d, AltSetting %d\n",
                                          DevInfo->VendorId, DevInfo->DeviceId, IntrfDesc->InterfaceNum, IntrfDesc->AltSettingNum);
                    
                    USB_DEBUG(DEBUG_INFO, 3, "CnfgDesc %x, DescLength 0x%x, TotalLength 0x%x\n", CnfgDesc, DescLength, TotalLength);
                    
                    // Get Device initial Start time.
                    if(FirstInterfaceConfig)
                        if ((gUsbData->UsbFeature2 & USB_PERFORMANCE_INFORMATION) && (!(gUsbData->UsbStateFlag & USB_FLAG_BOOT_EVENT))){
                            DevInfo->DetectStartTime = GetPerformanceCounter();
                        }

                    Pointer = USBIdentifyAndConfigureDevice(
                                    HcStruc,
                                    DevInfo,
                                    (UINT8*)CnfgDesc,
                                    DescLength,
                                    TotalLength);
                    if (Pointer != NULL) {
                        DevInfo = Pointer;
                        FirstInterfaceConfig = TRUE;
                        
                        // Get Device initial end time.
                        if ((gUsbData->UsbFeature2 & USB_PERFORMANCE_INFORMATION) && (!(gUsbData->UsbStateFlag & USB_FLAG_BOOT_EVENT))){
                            DevInfo->DetectEndTime = GetPerformanceCounter();
                        }
                        
                        DevConfigured = TRUE;  // At-least one device is configured
                        if (gUsbData->UsbStateFlag & USB_FLAG_SKIP_CARD_READER_CONNECT_BEEP) {
                            if (DevInfo->BaseClass == BASE_CLASS_MASS_STORAGE) {
                                SkipConnectBeep = TRUE;
                            }
                        }
                    }
                    
                    //
                    // There is one more config. Set device info structure entry 0 for it
                    //
                    if ((CnfgDesc->NumInterfaces > 1) && DevConfigured) {
                      gUsbDataList->DevInfoTable[0].Flag   |= DEV_INFO_MULTI_IF;
                    }
                    
                }
                if (IntrfDesc->DescLength && 
                    !(IntrfDesc->DescType == DESC_TYPE_INTERFACE &&
                            IntrfDesc->BaseClass == BASE_CLASS_HUB)) {
                    DescLength += (UINT16)IntrfDesc->DescLength;
                    if (DescLength < TotalLength) {
                        DevInfo = gUsbDataList->DevInfoTable;
                    }
                } else {
                    break;   // IntrfDesc->DescLength == 0
                }
            }   // while ()
        }   // if
        //
        // Check if we have at least one usable device
        //
        if (DevConfigured) {
            ErrorFlag = FALSE; // Device successfully configured
            ConfigLevel = (UINTN)gUsbDataList->DevInfoTable;
            goto detection_complete;
        } else {
            GetCnfgIndex++;
        }
    } while (GetCnfgIndex < DevInfo->DevDesc.NumConfigs);  // while

detection_complete:
    //
    // At this point, if bErrorFlag is FALSE then we successfully configured
    // atleast a device.
    // If bErrorFlag is TRUE then there is error in configuring the device
    //
    if (ErrorFlag) {
        USBLogError((UINT16)ConfigLevel); // Log configuration level

        SpeakerBeep(8, 0x2000, HcStruc);

        AmiUsbDeinitDeviceData(HcStruc, DevInfo);
        
        if((DevInfo->BosDescPtr != NULL)&&(DevInfo->BosTotalLength != 0)){
           USB_MemFree(DevInfo->BosDescPtr, GET_MEM_BLK_COUNT(DevInfo->BosTotalLength));
        }
        if (ConfigLevel != USB_ERR_DEV_INIT_GET_DESC_8) {
            //
            // Disable the hub port
            //
            USB_DisableHubPort(
                HcStruc,
                DevInfo->HubDeviceNumber,
                DevInfo->HubPortNumber);
            ConfigLevel = 0;
        }
        if (DevInfo->DeviceAddress) {
            gUsbData->DeviceAddressMap |= LShiftU64(1, DevInfo->DeviceAddress);
        }
    } else {
        if (!SkipConnectBeep) {
            SpeakerBeep(4, 0x1000, HcStruc);
        }
    }
    USB_MemFree(Buffer, (UINT8)(MAX_CONTROL_DATA_SIZE / sizeof(MEM_BLK)));

    return (DEV_INFO*)ConfigLevel;

}


/**
    This function stops the device:
    - calls its disconnect function if available
    - stops polling the device's interrupt endpoint
    - updates device address memory map


    @param  HcStruc          Pointer to HCStruc
    @param  HcHubAddr        address
    @param  HcPort           Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_StopDevice(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       HcPort
)
{
    UINT8           Count;
    DEV_DRIVER      *DevDriver;
    DEV_INFO        *DevInfo;
    UINT8           Status;

    Status = USB_ERROR;
    
    //
    // Find the device entry that would match the input.
    //
    for (Count = 1; Count < gUsbData->MaxDevCount; Count++) {
        DevInfo = &gUsbDataList->DevInfoTable[Count];
        if ((DevInfo->Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_DUMMY)) ==
            DEV_INFO_VALID_STRUC) {
            if ((DevInfo->HubDeviceNumber == HubAddr) && 
                (DevInfo->HubPortNumber == HcPort)) {
                //
                // Device found - issue disconnect call for the device
                //
                DevDriver = UsbFindDeviceDriverEntry(DevInfo->DevDriverIndex);

                //
                // Check disconnect function is valid, if yes - execute it
                //
                if (DevDriver && DevDriver->FnDisconnectDevice) {
                    DevDriver->FnDisconnectDevice(DevInfo);
                    DevInfo->DevDriverIndex = NULL_DRIVERS_INDEX;
                } else {
                    //
                    // Stop polling the device's interrupt endpoint
                    //
                    if (DevInfo->IntInEndpoint) {
                        AmiUsbDeactivatePolling(HcStruc, DevInfo);
                        DevInfo->IntInEndpoint = 0;
                    }
                }

                // HC device removal call
                Status = AmiUsbDeinitDeviceData(HcStruc, DevInfo);

                // Reset the disconnecting flag
                DevInfo->Flag &= ~DEV_INFO_DEV_DISCONNECTING;

                //
                // Update Device Address Map, preserving the address for registered devices
                //
                if (DevInfo->DeviceAddress != 0) {
                    gUsbData->DeviceAddressMap |= LShiftU64(1, DevInfo->DeviceAddress);
                    DevInfo->DeviceAddress = 0;
                }

                if((DevInfo->BosDescPtr != NULL)&&(DevInfo->BosTotalLength != 0)){
                   USB_MemFree(DevInfo->BosDescPtr, GET_MEM_BLK_COUNT(DevInfo->BosTotalLength));
                }

                DevInfo->Flag &= ~DEV_INFO_DEV_PRESENT;
                if (!(DevInfo->Flag & (DEV_INFO_DEV_BUS | DEV_INFO_MASS_DEV_REGD))) {
                    // Reset the device info structure validity ~flag
                    DevInfo->Flag &= ~DEV_INFO_VALID_STRUC;
                }
                USB_DEBUG(DEBUG_INFO, 3, "Release Dev[%d]: %x, flag %x\n", Count, DevInfo, DevInfo->Flag);
            }
        }
    }
    return Status;
}

/**
    This function is called when a device disconnect is
    detected. This routine disables the hub port and stops the
    device and its children by calling another routine.

    @param HcStruc    Far pointer to HCStruc of the host controller
    @param HubAddr    USB device address of the hub whose status
                      has changed
                        bit 7   : 1 - Root hub, 0 for other hubs
                        bit 6-0 : Device address of the hub
    @param HCPort     Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_DisconnectDevice(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       HCPort
)
{
    //
    // A device has been disconnected from the USB.  First disable the hub port
    // that the device was plugged into.  Then free up the device's entry in the
    // DeviceTable.  If there an error occurs while disabling the port, assume
    // that the device is still present an leave its DeviceTable entry in place.
    //
    USB_DisableHubPort(HcStruc, HubAddr, HCPort);

    USB_StopDevice(HcStruc, HubAddr, HCPort);

    return USB_SUCCESS;

}


/**
    This routine processes the port status change (like connect,
    disconnect, etc.) for the root hub and external hubs.

    @param HcStruc     Pointer to Host Controller structure
    @param HubAddr     Device address of the hub whose status
                       has changed:
                       bit 7  : 1 - Root hub, 0 for other hubs
                       bit 6-0  : Device address of the hub
    @param PortNum     Hub port number

**/
UINT8
USBCheckPortChange (
    HC_STRUC    *HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum
)
{
    UINT32      PortStatus;
    DEV_INFO    *Dev;
    UINT8       Count;

    for (Count = 0; Count < USB_DEVICE_ENUMERATE_RETRY_COUNT; Count++) {
        
        PortStatus = USB_GetHubPortStatus(HcStruc, HubAddr, PortNum, TRUE);
        //
        // Check the obtained port status
        //
        if (PortStatus == USB_ERROR) {
            return USB_ERROR;
        }
        
        if (!(PortStatus & USB_PORT_STAT_DEV_OWNER)) {
            return USB_SUCCESS;
        }
        
        if (OEMSkipList(HubAddr,PortNum,HcStruc->BusDevFuncNum,0,0)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Match the skip table ; skipping this device.\n");
            return USB_SUCCESS;
        }
        
        if (!Count && !(PortStatus & USB_PORT_STAT_DEV_CONNECT_CHANGED)) {
            return USB_SUCCESS;
        }
    
        if (PortStatus & USB_PORT_STAT_DEV_CONNECTED) {
            if (gUsbData->HandOverInProgress) {
                USB_DisableHubPort(HcStruc, HubAddr, PortNum);
                return USB_SUCCESS;
            }
            
            if ((Count != 0) || !(PortStatus & USB_PORT_STAT_DEV_ENABLED) || 
                ((gUsbData->UsbFeature & USB_HC_RESET_PORT_POLICY) == USB_HC_RESET_PORT_POLICY)) {
                // Reset and enable the port
                USB_ResetHubPort(HcStruc, HubAddr, PortNum);
                USB_EnableHubPort(HcStruc, HubAddr, PortNum);
                PortStatus = USB_GetHubPortStatus(HcStruc, HubAddr, PortNum, TRUE);

                if (PortStatus == USB_ERROR) {
                    return USB_ERROR;
                }
                
                if (!(PortStatus & USB_PORT_STAT_DEV_OWNER)) {
                    return USB_SUCCESS;
                }
                if (!(PortStatus & USB_PORT_STAT_DEV_CONNECTED)) {
                    // Some device will be disconnected after 
                    // port reset, and reconnected for a while.
                    FixedDelay(100 * 1000);
                    continue;
                }
                // Check whether port is enabled
                if (!(PortStatus & USB_PORT_STAT_DEV_ENABLED)) {
                    FixedDelay(100 * 1000);  // 100msec delay
                    continue;
                }
            }
            
            Dev = USB_DetectNewDevice(HcStruc, HubAddr, PortNum, PortStatus);
            if ((UINTN)Dev == USB_ERR_DEV_INIT_GET_DESC_8) {
                // The device might be configured to the wrong speed and doesn't work.
                // Try to disable and reset the port, it might be reconfigured to the right speed.
                if (Count >= (USB_DEVICE_ENUMERATE_RETRY_COUNT / 2)) {
                    USB_DisableHubPort(HcStruc, HubAddr, PortNum);
                }
                FixedDelay(100 * 1000);     // 100msec delay
                continue;
            }
            if ((UINTN)Dev == 0) {
                return USB_ERROR;
            }
            if ((UINTN)Dev > USB_ERR_DEV_INIT_GET_DESC_200) {
                return USB_SUCCESS;
            }
            SpeakerBeep(16, 0x4000, HcStruc);  // Issue error beep
            return USB_ERROR;
        } else {  // Disconnect
            USB_DisconnectDevice(HcStruc, HubAddr, PortNum);
            SpeakerBeep(8, 0x1000, HcStruc);
            return USB_SUCCESS;
        }
    }

    if (Count == USB_DEVICE_ENUMERATE_RETRY_COUNT) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Usb Device enumeration is failed.\n");
        USB_DisableHubPort(HcStruc, HubAddr, PortNum);
        USB_GetHubPortStatus(HcStruc, HubAddr, PortNum, TRUE);
        return USB_ERROR;
    }

    return USB_SUCCESS;
}


/**
    This routine allocates blocks of memory from the global
    memory pool

    @param NumBlk  Number of 32 byte blocks needed

    @retval Start offset to the allocated block (NULL on error)

    @note  This routine allocates continuous 32 byte memory blocks.

**/

VOID _FAR_ *
USB_MemAlloc(
    UINT16  NumBlk
)
{
    UINT8       MemIsFound     = FALSE;
    UINT8       BitCount       = 0;
    UINT8       Start          = 0;
    UINT16      Num;
    UINT16      Count          = 0;    // Contiguous blocks counter

    UINT16      BlkOffset      = 0;
    UINT16      MapDwordPtr;
    UINT16      PageCount = 0;
    UINT16      MapDwordCount;
    UINT32      BlksStsDwordsPerPage = 0;

    UINT32      Mask;
    UINT32      Temp;

    if (NumBlk == 0) {
        return NULL;
    }
    

    if (NumBlk % 2) {
        NumBlk++;
    }
    Num = NumBlk;

    BlksStsDwordsPerPage = (gUsbData->MemBlkStsBytes >> 2) / gUsbData->MemPages;

    //
    // Locate wNumBlk contiguous blocks from each memory page
    //
    for(PageCount = 0; (PageCount < gUsbData->MemPages) && !MemIsFound; PageCount++) {

        // Do not reset the counter if the allocated blocks greater than a page.
        if (NumBlk <= (0x1000 / sizeof(MEM_BLK))) {
            Count = 0;  // Reset contiguous blocks counter
        }

      for (MapDwordCount = 0; MapDwordCount < BlksStsDwordsPerPage; MapDwordCount++) {
          //
          // Read the next DWORD memory map data
          //
          MapDwordPtr = (UINT16)(PageCount * BlksStsDwordsPerPage) + MapDwordCount;
          Temp = gUsbDataList->MemBlkSts[MapDwordPtr];

          for (BitCount = 0; BitCount < 32; BitCount++)  {
              BlkOffset++;
              if (Temp & (UINT32)(LShiftU64(1, BitCount)))  {
                  Count++;    // Found another free block
                  if(Count == Num) {
                      BlkOffset = (UINT16)(BlkOffset-Count);
                      MemIsFound = TRUE;
                      break;  // Found the requested number of free blocks
                  }
              }
              else
              {
                  Count = 0;  // Reset contiguous blocks counter
              }
          }
          if (MemIsFound) break;
      }
    }

    if (!MemIsFound) {
        ASSERT(FALSE);
        return NULL;
    }

//
// Change the appropriate bits in the memory map to indicate that some memory
// is being allocated
//
// At this point,
//  bBitCount points to the end of the block within DWORD
//  wMapDwordPtr points to the status dword in question

// We have to reset bCount number of bits starting from
// wMapDwordPtr[bBitCount] to wStsX[BitPosY]
// where wStsX is the status double word of the starting block,
// BitPosY is the bit position of the starting block.
//
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "MapDwordPtr = %d\n", MapDwordPtr);
//
// Let us have a do loop to do the trick
//
    do {
        //
        // Find out how many bits we can reset in current (pointed by wMapDwordPtr)
        // double word
        //
        Count = (UINT16)((BitCount >= (Num-1)) ? Num : BitCount+1);
        //
        // Find out the starting bit offset
        //
        Start = (UINT8)(BitCount + 1 - Count);
        //
        // Form the 32bit mask for the AND operation
        // First prepare the bits left on the left
        //
        // Note: FFFFFFFF << 32 treated differently by different compilers; it
        // results as 0 for 16 bit compiler and FFFFFFFF for 32 bit. That's why
        // we use caution while preparing the AND mask for the memory map update.
        //
        Mask = ((Count + Start) < 32) ? (0xFFFFFFFF << (Count + Start)) : 0;

        //
        // Second, prepare the bits on the right
        //
        if (Start) {
            Mask = Mask | ~(0xFFFFFFFF << Start);
        }

        //
        // Reset the specified number of bits
        //
        gUsbDataList->MemBlkSts[MapDwordPtr] &= Mask;

        //
        // Update the bCount, StsWordCount & BitCount
        //
        BitCount = 31;     // End of previous double word where we have to start
        MapDwordPtr--;     // Previous double word
        Num = Num - Count;
    } while (Num);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "MemAlloc: %d block(s) at %x %x %x\n",
            NumBlk,
            gUsbDataList->MemBlockStart + BlkOffset * sizeof(MEM_BLK),
            gUsbDataList->MemBlkSts[0],
            gUsbDataList->MemBlkSts[1]);

    return  ((VOID _FAR_ *)
        (gUsbDataList->MemBlockStart + (UINT32)BlkOffset * sizeof(MEM_BLK)));
}


/**
    This routine frees the chunk of memory allocated using
    the USBMem_Alloc call

    @param Ptr       Pointer to the memory block to be freed
    @param NumBlk    Number of 32 byte blocks to be freed

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

    @note  This routine frees continuous memory blocks starting
              from the fpPtr.

**/

UINT8
USB_MemFree (
    VOID _FAR_ * Ptr,
    UINT16    NumBlk
)
{
    UINT8   Offset;
    UINT8   Count;
    UINT16  BlkCount;
    UINT16  BlkOffset;
    UINT16  StsWord;


    if (NumBlk % 2) {
        NumBlk++;
    }

    BlkCount = NumBlk;
    BlkOffset = 0;

    //
    // Check for pointer validity
    //
    if (Ptr == NULL) return USB_ERROR;

    if ((Ptr < (VOID *)gUsbDataList->MemBlockStart) ||
        (Ptr > (VOID *)(gUsbDataList->MemBlockStart +
        (gUsbData->MemPages * (4096 / USB_MEM_BLK_SIZE)+1)*sizeof(MEM_BLK)))) {
        return  USB_ERROR;
    }

    BlkOffset = (UINT16)((UINTN)Ptr - (UINTN)gUsbDataList->MemBlockStart) / sizeof (MEM_BLK);

    if (BlkOffset >= (gUsbData->MemPages * (4096 / USB_MEM_BLK_SIZE))) {
        return USB_ERROR;
    }

    StsWord = (UINT16)(BlkOffset >> 5);   // Divide by 32
    Offset = (UINT8)(BlkOffset & 0x1F);   // Mod 32
    Count = 0;

    do {
        gUsbDataList->MemBlkSts[StsWord] |= ((UINT32)1 << (Count + Offset));
        BlkCount--;
        Count++;

        if ((Count + Offset) && (!((Count + Offset) & 0x1F))) {
            StsWord ++;
            Count = Offset = 0;
        }
    } while (BlkCount);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "MemFree: %d block(s) at %x %x %x\n",
                NumBlk, Ptr,
                gUsbDataList->MemBlkSts[0], gUsbDataList->MemBlkSts[1]);
    //
    // Pointer is valid. Fill the memory with 0's
    //
    ZeroMem(Ptr, (UINT32)(NumBlk * sizeof (MEM_BLK)));

    return USB_SUCCESS;
}

/**
    This function adds a new callback function to the globall
    callback function list and returns the index of it.

    @param CallBackFunction     Callback function address

    @retval (Index+1)           Callback function index

**/
UINT8
EFIAPI
USB_InstallCallBackFunction (
    CALLBACK_FUNC      CallBackFunction
)
{
    UINT8   Index;
    UINT8   MaxIndex;


    Index = 0;
    MaxIndex = MAX_CALLBACK_FUNCTION;


    //
    // Check whether this function is already installed or none found
    //
    for (; Index < MaxIndex; Index++) {
        //
        // Check for null entry
        //
        if (gAmiUsbCallBackFunTbl[Index] == 0) {
            break;  // No entry found
        }

        if (gAmiUsbCallBackFunTbl[Index] == CallBackFunction) {
            return (UINT8)(Index+1);
        }
    }

    ASSERT(Index != MaxIndex);
    if (Index == MaxIndex) {
//        EFI_DEADLOOP(); // Exceeding max # of callback function is illegal
        return 0;         // NULL return for callback function is illegal.
    } else {
        //
        // Store the call back function
        //
        gAmiUsbCallBackFunTbl[Index] = CallBackFunction;
    }
    return (UINT8)(Index + 1);
}

/**
    This function adds a new callback function to the global
    callback function list and returns the index of it.

    @param DevDriverIndex       The index in gUsbDelayedDrivers table.

    @retval DEV_DRIVER          Return the specific DEV_DRIVER. 

**/
DEV_DRIVER*
UsbFindDeviceDriverEntry(
    UINTN    DevDriverIndex
)
{
    UINTN   Index;

    if (DevDriverIndex == NULL_DRIVERS_INDEX) {
        return NULL;
    }

    if (DevDriverIndex>=DELAYED_DRIVERS_BIT){
        Index = (DevDriverIndex&(~DELAYED_DRIVERS_BIT));
        return &gUsbDelayedDrivers[Index];
    } else {
        return &gUsbDevDriverTable[DevDriverIndex];
    }
 
}

/**
    This function executes a get descriptor command to the
    given USB device and endpoint.

    @param HcStruc    HCStruc pointer
    @param DevInfo    Device info pointer
    @param Buffer     Buffer to be used for the transfer
    @param Length     Size of the requested descriptor
    @param DescType   Requested descriptor type
    @param DescIndex  Descriptor index

    @retval Buffer    Pointer to memory buffer containing the descriptor
                      NULL on error

**/

UINT8*
USB_GetDescriptor(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*  Buffer,
    UINT16  Length,
    UINT8   DescType,
    UINT8   DescIndex
)
{
    UINT8           GetDescIteration;
    UINT16          Reg;
    UINT16          CtlReturn;
    EFI_STATUS      Status;

    Status = UsbHcStrucValidation(HcStruc);

    if (EFI_ERROR(Status)) {
        return NULL;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return NULL;
    }


    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)Buffer, Length);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return NULL;
        }
        gCheckUsbApiParameter = FALSE;
    }

    for (GetDescIteration = 0; GetDescIteration < 5; GetDescIteration++) {
        Reg = (UINT16)((DescType << 8) + DescIndex);
        CtlReturn = AmiUsbControlTransfer(
                     HcStruc,
                     DevInfo,
                     (UINT16)USB_RQ_GET_DESCRIPTOR,
                     (UINT16)0,
                     Reg,
                     Buffer,
                     Length);

        if (CtlReturn) {
            return Buffer;
        }
        
        if (gUsbData->UsbLastCommandStatusExtended & USB_TRNSFR_TIMEOUT) {
            break;
        }
        FixedDelay(10 * 1000);
    
    }

    return NULL;
}


/**
    This function sets the USB device address of device 0 to
    the given value. After this call the USB device will respond
    at its new address.

    @param HcStruc      Pointer to HCStruc structure
    @param DevInfo      Pointer to device info structure
    @param NewDevAddr   New device address to set

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

    @note  Skip SET_ADDRESS request for XHCI controllers

**/

UINT8
USB_SetAddress(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       NewDevAddr
)
{
    UINT8           SetAddressIteration;

    for (SetAddressIteration = 0; SetAddressIteration < 5; SetAddressIteration++) {
        AmiUsbControlTransfer(
                     HcStruc,
                     DevInfo,
                     (UINT16)USB_RQ_SET_ADDRESS,
                     0,
                     (UINT16)NewDevAddr,
                     0,
                     0);
                     
        if (!(gUsbData->UsbLastCommandStatus & USB_CONTROL_STALLED )) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USB_SetAddress#%d\n",NewDevAddr);
            return USB_SUCCESS;
        }
    }
    return USB_ERROR;

}

/**
    This function sets the device configuration.

    @param HcStruc   Pointer to HCStruc structure
    @param DevInfo   Pointer to device info structure
    @param ConfigNum Configuration Value

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USB_SetConfig(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       ConfigNum)
{
    AmiUsbControlTransfer(
        HcStruc,
        DevInfo,
        USB_RQ_SET_CONFIGURATION,
        0,
        (UINT16)ConfigNum,
        0,
        0);

    return USB_SUCCESS;
}

/**
    This function sets the device interface.

    @param HcStruc       Pointer to HCStruc structure
    @param DevInfo       Pointer to device info structure
    @param InterfaceNum  Interface Value

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
UsbSetInterface(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       InterfaceNum
)
{
    USB_DEBUG(DEBUG_INFO, 3, "UsbSetInterface %d\n", InterfaceNum);
    AmiUsbControlTransfer(
        HcStruc,
        DevInfo,
        USB_RQ_SET_INTERFACE,
        0,
        (UINT16)InterfaceNum,
        0,
        0);
    return USB_SUCCESS;
}

/**
    This routine logs the USB error in the data area. This
    logged errors will be displayed during the POST.

    @param  ErrorCode        Error code to log

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USBLogError(
    UINT16  ErrorCode
)
{
    //
    // First check for end of the buffer
    //
    if(gUsbData->ErrorLogIndex < MAX_USB_ERRORS_NUM) {
        //
        // Still have space to log errors
        //
        gUsbData->ErrorLogBuffer[gUsbData->ErrorLogIndex] = ErrorCode;
        gUsbData->ErrorLogIndex++;
    }
    return USB_ERROR;
}


/**
    This function is used to retrieve the device info structure
    for the particular device address & HCStruc

    @param SearchFlag  Flag indicating search type
                       = USB_SRCH_DEV_ADDR to search by device address and HCStruc pointer
                       = USB_SRCH_DEV_TYPE to search by device type
                       = USB_SRCH_HC_STRUC to search by HC struc pointer
                       = USB_SRCH_DEV_NUM to count the number of devices connected:
                       if HcStruc is not NULL - count only devices connected to
                       certain controller, otherwise - all devices of requested type.
                       = USB_SERCH_DEV_INDX to search by device location in the DEV_INFO:
                       a) if FpDevInfo <> 0 return index or the FpDevInfo
                       b) if DevAddr <> 0 return the corresponding FpDevInfo
                       c) if both DevAddr <> 0 and FpDevInfo <> 0, consider a)

    @param DevInfo    Pointer to the device info structure from where the
                      search begins (if 0 start from first entry)
    @param Dev        Device address/drive number/device type
    @param HcStruc    Pointer to the HCStruc structure

    @retval DeviceCounter  Depending on bSearchFlag this function returns:
                           - pointer to DEV_INFO structure
                           - table index
                           - number of devices
                           Function returns NULL if device is not found.

**/

DEV_INFO*
EFIAPI
USB_GetDeviceInfoStruc(
    UINT8       SearchFlag,
    DEV_INFO*   DevInfo,
    UINT8       Dev,
    HC_STRUC*   HcStruc
)
{
    UINT8       Index;
    BOOLEAN     TerminateSearch = FALSE;
    UINT32      DeviceCounter  = 0;

    if (SearchFlag == USB_SRCH_DEV_INDX) {
        if (DevInfo) {
            for (Index=1; Index < gUsbData->MaxDevCount; Index++) {
                if (&gUsbDataList->DevInfoTable[Index] == DevInfo) {
                    return (DEV_INFO*)(UINTN)Index;
                }
            }
            return NULL;    // Device address not found in the table
        }
        if (Dev == USB_HOTPLUG_FDD_ADDRESS) return (DEV_INFO*)(VOID*)&gUsbData->FddHotplugDev;
        if (Dev == USB_HOTPLUG_HDD_ADDRESS) return (DEV_INFO*)(VOID*)&gUsbData->HddHotplugDev;
        if (Dev == USB_HOTPLUG_CDROM_ADDRESS) return (DEV_INFO*)(VOID*)&gUsbData->CdromHotplugDev;

        if (Dev) return &gUsbDataList->DevInfoTable[Dev];
        return NULL;        // Invalid input - both bDev and FpDevInfo are zeroes.
    }

    for (Index = 1; Index < gUsbData->MaxDevCount; Index ++) {
        //
        // if fpDev_Info is not null then position the search at the correct
        // index that matches the fpDev_Info
        //
        if (DevInfo) {
            if (&gUsbDataList->DevInfoTable[Index] != DevInfo)
                continue;
            else {
                DevInfo = 0;
                continue;
            }
        }
        //
        // For USB_SRCH_DEVBASECLASS_NUM devices are counted regardless of their
        // DEV_INFO_VALID_STRUC flag
        //
        if (SearchFlag == USB_SRCH_DEVBASECLASS_NUM)
        {
            if(gUsbDataList->DevInfoTable[Index].BaseClass == Dev) {
                if (HcStruc) {
                    //
                    // Check if device belongs to the specified HC
                    //
                    if (gUsbDataList->DevInfoTable[Index].HcNumber != HcStruc->HcNumber)
                    {
                        continue;
                    }
                }
                if (gUsbDataList->DevInfoTable[Index].Flag & DEV_INFO_DEV_PRESENT)
                {
                    DeviceCounter++;
                }
            }
            continue;
        }

        if ((gUsbDataList->DevInfoTable[Index].Flag & DEV_INFO_VALIDPRESENT) ==
            DEV_INFO_VALIDPRESENT){
            switch(SearchFlag) {
                case  USB_SRCH_HC_STRUC:
                    if (HcStruc == NULL) return NULL;
                    if (gHcTable[gUsbDataList->DevInfoTable[Index].HcNumber-1] == HcStruc) {
                        TerminateSearch = TRUE;
                    }
                    break;

                case  USB_SRCH_DEV_TYPE:
                    if (gUsbDataList->DevInfoTable[Index].DeviceType == Dev) {
                        TerminateSearch = TRUE;
                    }
                    break;
                case  USB_SRCH_DEV_NUM:
                    if (gUsbDataList->DevInfoTable[Index].DeviceType == Dev) {
                        if (HcStruc) {
                            //
                            // Check if device belongs to the specified HC
                            //
                            if (gUsbDataList->DevInfoTable[Index].HcNumber != HcStruc->HcNumber)
                            {
                                break;
                            }
                        }
                        DeviceCounter++;
                    }
                    break;  // Do not change TerminateSearch so loop continues
                case  USB_SRCH_DEV_ADDR:
                    if (gUsbDataList->DevInfoTable[Index].DeviceAddress == Dev) {
                        if ((HcStruc == NULL) ||
                            (gHcTable[gUsbDataList->DevInfoTable[Index].HcNumber-1] == HcStruc)) {
                            TerminateSearch = TRUE;
                        }
                    }
                    break;

                default:
                    return NULL;
            }
        }
        if (TerminateSearch) return ((DEV_INFO*)&gUsbDataList->DevInfoTable[Index]);
    }
    if ( (SearchFlag == USB_SRCH_DEV_NUM) || (SearchFlag == USB_SRCH_DEVBASECLASS_NUM) )
         return (DEV_INFO*)(UINTN)DeviceCounter;

    return NULL;
}


/**
    Finds a non-used DEV_INFO record in DevInfoTable and marks it
    reserved. To free the user need to clear DEV_INFO_VALID_STRUC
    bit in bFlag of DEV_INFO
    
    @param  None
    @retval NewDevInfo   Pointer to new device info. struc. 0 on error

**/
DEV_INFO* UsbAllocDevInfo()
{
    UINT8       Index;
    DEV_INFO    *NewDevInfo;

    for (Index = 1; Index < gUsbData->MaxDevCount; Index ++){
        NewDevInfo = gUsbDataList->DevInfoTable + Index;
        if ((NewDevInfo->Flag &
        ( DEV_INFO_VALID_STRUC | DEV_INFO_DEV_BUS)) == 0 ){
            //
            // Free device info structure. Save it if not.
            //
            NewDevInfo->Flag |= DEV_INFO_VALID_STRUC |  DEV_INFO_DEV_PRESENT;
            return  NewDevInfo;
        }
    }
    return NULL;
}


/**
    This routine searches for a device info structure that
    matches the vendor and device id, and LUN of the device
    found. If such a device info structure not found, then it
    will return a free device info structure

    @param Dev       Pointer to new device info
    @param Lun       Current LUN

    @retval DevInfo  Pointer to new device info. struc. NULL on error

**/

DEV_INFO*
USBGetProperDeviceInfoStructure(
    DEV_INFO*  Dev,
    UINT8     Lun
)
{
    UINT8       Count;
    DEV_INFO    *DevInfo;
    DEV_INFO    *FreeDevInfo;

    FreeDevInfo = NULL;

//
// Scan through the device info table for a free entry. Also if the device
// connected is a mass storage device look for a device info structure whose
// device is disconnected and its vendor & device id matches the one of
// current device. If such a structure found that means this device may be
// reconnected - use the same structure
//
    for (Count = 1; Count < gUsbData->MaxDevCount; Count++)
    {
        DevInfo = (DEV_INFO*)&gUsbDataList->DevInfoTable[Count];

        if (DevInfo->Flag & DEV_INFO_DEV_DUMMY) {
            continue;
        }

        // Check whether the structure is valid
        if (!(DevInfo->Flag & DEV_INFO_VALID_STRUC)) {    
            if (FreeDevInfo == NULL) {
                FreeDevInfo = DevInfo;    // Store the value of the free device info
            }
        } else {
            //
            // Yes, structure is valid. Check for device presence
            //
            if (DevInfo->Flag & DEV_INFO_DEV_PRESENT) {
              if ((DevInfo->HubDeviceNumber != Dev->HubDeviceNumber) ||
                  (DevInfo->HubPortNumber != Dev->HubPortNumber)) {
                  continue;
              }
            }
            //
            // Device is not present. Match the vendor, device id  and LUN with
            // current device info
            //
            if(gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI){
                if ((DevInfo->VendorId == Dev->VendorId) &&
                    (DevInfo->DeviceId == Dev->DeviceId) &&
                    (DevInfo->InterfaceNum == Dev->InterfaceNum) &&
                    (DevInfo->EndpointSpeed == Dev->EndpointSpeed) &&
                    (DevInfo->Lun == Lun)) {
                    return DevInfo;   // "Abandoned" device entry found
                }
            }else{
                if ((DevInfo->VendorId == Dev->VendorId) &&
                    (DevInfo->DeviceId == Dev->DeviceId) &&
                    (DevInfo->InterfaceNum == Dev->InterfaceNum) &&
                    (DevInfo->Lun == Lun)) {
                    return DevInfo;   // "Abandoned" device entry found
                }
            }
        }
    }
    return FreeDevInfo;
}


/**
    This routine completes the USB device configuration for
    the devices supported by USB BIOS. This routine
    handles the generic configuration for the devices.

    @param HcStruc      HCStruc pointer
    @param DevInfo      Device information structure pointer
    @param Desc         Pointer to the descriptor structure
    @param Start        Offset within interface descriptor supported by the device
    @param End          End offset of the device descriptor

    @retval NewDevInfo  Pointer to new device info. struc. 0 on error

**/

DEV_INFO*
USB_ConfigureDevice (
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Start,
    UINT16      End
)
{
    DEV_INFO    *NewDevInfo;
    HC_STRUC    *NewHcStruc;
    DEV_DRIVER  *NewDevDriver;

    NewDevInfo = USBGetProperDeviceInfoStructure(DevInfo, 0);

    if (NewDevInfo == NULL) {
        return NULL;
    }
    //
    // Check whether this device is reconnected by checking the valid
    // structure flag
    //
    if (NewDevInfo->Flag & DEV_INFO_VALID_STRUC) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_ConfigureDevice: Existing device.\n");
        //
        // This device is reconnected. Reuse the old device address so that
        // INT13h can identify this drive properly
        //
        DevInfo->Flag |= NewDevInfo->Flag;
        DevInfo->HidDevType = NewDevInfo->HidDevType;
        NewDevInfo->DataInSync = 0;
        NewDevInfo->DataOutSync = 0;
        
        NewHcStruc = gHcTable[NewDevInfo->HcNumber - 1];
        NewDevDriver = UsbFindDeviceDriverEntry(NewDevInfo->DevDriverIndex);

        if (NewDevDriver && NewDevDriver->FnDisconnectDevice) {
            NewDevDriver->FnDisconnectDevice(NewDevInfo);
            NewDevInfo->DevDriverIndex = NULL_DRIVERS_INDEX;
            } else {
            //
            // Stop polling the device's interrupt endpoint
            //
            if (NewDevInfo->IntInEndpoint) {
                AmiUsbDeactivatePolling(NewHcStruc, NewDevInfo);
                NewDevInfo->IntInEndpoint = 0;
            }
        }
        if ((NewDevInfo->DevMiscInfo != NULL) && (NewDevInfo->DevMiscInfo != DevInfo->DevMiscInfo)) {
            AmiUsbDeinitDeviceData(NewHcStruc, NewDevInfo);
        }
        //
        // Update Device Address Map, preserving the address for registered devices
        //
        if (NewDevInfo->Flag & DEV_INFO_DEV_PRESENT) {
            if (NewDevInfo->DeviceAddress != 0) {
                gUsbData->DeviceAddressMap |= LShiftU64(1, NewDevInfo->DeviceAddress);
                NewDevInfo->DeviceAddress = 0;
            }
        }
    }
    else {
        //
        // Check whether we reached the limit of devices of this type
        //
        if (CheckDeviceLimit(DevInfo->BaseClass) == TRUE) {
            return NULL;
        }
    }

    //
    // For registered devices skip updating bFlag
    //
    if (!(NewDevInfo->Flag & DEV_INFO_MASS_DEV_REGD)) {
        //
        // Since DeviceInfo[0] already has many fields filled in, the new entry
        // should be initialized with a copy of DeviceInfo[0].  But, the new
        // DeviceInfo should not be  marked as "present" until the device
        // is successfully initialized.
        //
        // Copy old DeviceInfo struc to new DeviceInfo struc and zero device[0]
        //
        DevInfo->HidReport.Flag = NewDevInfo->HidReport.Flag;
        CopyMem((UINT8*)NewDevInfo, (UINT8*)DevInfo, sizeof (DEV_INFO));
        NewDevInfo->Flag &= DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT |
                            DEV_INFO_MASS_DEV_REGD | DEV_INFO_DEV_BUS |
                            DEV_INFO_IN_QUEUE | DEV_INFO_ALT_SETTING_IF;
    } else {
        // Change the parent HC number and port number in the existing DEV_INFO
        NewDevInfo->HcNumber = DevInfo->HcNumber;
        NewDevInfo->HubDeviceNumber = DevInfo->HubDeviceNumber;
        NewDevInfo->HubPortNumber = DevInfo->HubPortNumber;
        NewDevInfo->EndpointSpeed = DevInfo->EndpointSpeed;
        NewDevInfo->Endp0MaxPacket = DevInfo->Endp0MaxPacket;
        NewDevInfo->DevMiscInfo = DevInfo->DevMiscInfo;
        NewDevInfo->DeviceAddress = DevInfo->DeviceAddress;
    }

    //
    // Do a SetConfiguration command to the device to set it to its
    // HID/Boot configuration.
    //
    NewDevInfo->Flag |= DEV_INFO_VALIDPRESENT;
    if (!(DevInfo->Flag & DEV_INFO_MULTI_IF)) {
        USB_SetConfig(HcStruc, NewDevInfo, NewDevInfo->ConfigNum);
        if (DevInfo->Flag & DEV_INFO_ALT_SETTING_IF) {
            UsbSetInterface(HcStruc, NewDevInfo, NewDevInfo->AltSettingNum);
        }
    }

    USB_DEBUG(DEBUG_INFO, 3, "new dev: %x, flag: %x, addr %d\n",
                NewDevInfo, NewDevInfo->Flag, NewDevInfo->DeviceAddress);

    return NewDevInfo;

}


/**
    This function checks for non-compliant USB devices by
    by comparing the device's vendor and device id with
    the non-compliant device table list and updates the
    data structures appropriately to support the device.

    @param HcStruc     HCStruc pointer
    @param DevInfo     Device information structure pointer
                       The routine Updates DevInfo->IncompatFlags field 
    @param Desc        Pointer to the descriptor structure
    @param Length      End offset of the device descriptor
    @param DescLength  Descriptor length
    @retval None

**/
VOID
USBCheckNonCompliantDevice(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Length,
    UINT16      DescLength
)
{
    USB_BADDEV_STRUC    *BadDevice;
    INTRF_DESC          *IntrfDesc;

    IntrfDesc = (INTRF_DESC*)((UINT8*)Desc + Length);

    //
    // Search the bad device table to get the structure for this device
    //
    for (BadDevice = gUsbBadDeviceTable;
         BadDevice->Did | BadDevice->Vid; BadDevice++) {

        if ((BadDevice->Did != DevInfo->DeviceId) ||
                (BadDevice->Vid != DevInfo->VendorId)) {
            continue;
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Found non-compatible device: DID=%x, VID=%x\n", BadDevice->Did, BadDevice->Vid);
        //
        // Save the incompatibility flag into device info structure
        //
        DevInfo->IncompatFlags = BadDevice->Flag;

        //
        // Check which fields to update in the interface descriptor
        //
        // Check for base class field
        //
        if (BadDevice->BaseClass) {
            //
            // Update base class field in the interface descriptor
            //
            IntrfDesc->BaseClass = BadDevice->BaseClass;
        }
        //
        // Check for base sub class field
        //
        if (BadDevice->SubClass) {
            //
            // Update sub class field in the interface descriptor
            //
            IntrfDesc->SubClass = BadDevice->SubClass;
        }
        //
        // Check for protocol field
        //
        if (BadDevice->Protocol) {
            //
            // Update protocol field in the interface descriptor
            //
            IntrfDesc->Protocol = BadDevice->Protocol;
        }
        break;
    }

    //
    // Search the ex-bad device table to get the structure for this device
    //
    for (BadDevice = (USB_BADDEV_STRUC*)&gUsbData->UsbBadDevices[0]; BadDevice->Did | BadDevice->Vid; BadDevice++) {
        
        if ((BadDevice->Did == 0) && (BadDevice->Vid == 0)) break;
        if ((BadDevice->Did != DevInfo->DeviceId) ||
                (BadDevice->Vid != DevInfo->VendorId)) {
            continue;
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Found non-compatible device: DID=%x, VID=%x\n", BadDevice->Did, BadDevice->Vid);
        //
        // Save the incompatibility flag into device info structure
        //
        DevInfo->IncompatFlags = BadDevice->Flag;

        //
        // Check which fields to update in the interface descriptor
        //
        // Check for base class field
        //
        if (BadDevice->BaseClass) {
            //
            // Update base class field in the interface descriptor
            //
            IntrfDesc->BaseClass = BadDevice->BaseClass;
        }
        //
        // Check for base sub class field
        //
        if (BadDevice->SubClass) {
            //
            // Update sub class field in the interface descriptor
            //
            IntrfDesc->SubClass = BadDevice->SubClass;
        }
        //
        // Check for protocol field
        //
        if (BadDevice->Protocol) {
            //
            // Update protocol field in the interface descriptor
            //
            IntrfDesc->Protocol = BadDevice->Protocol;
        }
        break;
    }


}


/**
    This routine invokes the device drivers 'check device type'
    routine and identifies the device.

    @param HcStruc         HCStruc pointer
    @param DevInfo         Device information structure pointer
                           The routine Updates DevInfo->IncompatFlags field 
    @param Desc            Pointer to the descriptor structure
    @param Length          End offset of the device descriptor
    @param DescLength      Descriptor length

    @retval DevInfoLocal   Pointer to new device info. struc, NULL on error


**/

DEV_INFO*
USBIdentifyAndConfigureDevice (
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Length,
    UINT16      DescLength)
{
    UINT8           BaseClass;
    UINT8           SubClass;
    UINT8           Protocol;
    UINTN           Index;
    UINT8           RetValue;
    UINT8           MaxIndex;
    DEV_INFO*       DevInfoLocal;
    DEV_DRIVER*     DevDriver;
    INTRF_DESC*     IntrfDesc;

    //
    // Check for non-compliant device. If non-compliant device found then
    // the descriptor values will get updated depending on the need.
    //
    USBCheckNonCompliantDevice (
            HcStruc,
            DevInfo,
            Desc,
            Length,
            DescLength);
    
    UsbOemCheckNonCompliantDevice(
            HcStruc,
            DevInfo,
            Desc,
            Length,
            DescLength);
  
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBIdentifyAndConfigureDevice...");

    //
    // Check whether device needs to be disable
    //
    if (DevInfo->IncompatFlags & USB_INCMPT_DISABLE_DEVICE) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "not compatible device.\n");
        return NULL;
    }

    IntrfDesc = (INTRF_DESC*)(Desc + Length);


    if(OEMSkipList(DevInfo->HubDeviceNumber,DevInfo->HubPortNumber,HcStruc->BusDevFuncNum,IntrfDesc->BaseClass,1)) {
        USB_DEBUG(DEBUG_INFO, 3, "Match the skip table ; skipping this device.\n"); 
        return NULL;
    }  
    //
    // Get the base, sub class & protocol values
    //
    BaseClass  = IntrfDesc->BaseClass;
    SubClass   = IntrfDesc->SubClass;
    Protocol   = IntrfDesc->Protocol;

    //
    // Check for matching device driver
    //
    DevInfoLocal = NULL;
    RetValue = USB_ERROR;
    

    Index = 0;
    MaxIndex = MAX_DEVICE_TYPES;
                                     
    for (; Index < MaxIndex; Index ++) {
        DevDriver = &gUsbDevDriverTable[Index];
        //
        // Check structure validity
        //
        if (!DevDriver->DevType) {
            continue;       // Driver table not valid
        }
        //
        // Verify presence of Check Device routine
        //
        if (DevDriver->FnCheckDeviceType) {

            //
            // Check device type is implemented. Execute it!
            //
            RetValue = (*DevDriver->FnCheckDeviceType)(DevInfo,BaseClass,SubClass,Protocol);
            if (RetValue != USB_ERROR) break;
        
        }  else {
            //
            // Check device type is not implemented. Compare the class codes
            //
            if((DevDriver->BaseClass == BaseClass) ||
                (DevDriver->SubClass == SubClass) ||
                (DevDriver->Protocol == Protocol)) {
                //
                // If the class codes match set bRetValue with the DevType from the Device Driver
                //
                RetValue = DevDriver->DevType;
                break;
            }
        }
    }
    
    if (RetValue != USB_ERROR) {
        //
        // Set the device type in the Device Info structure
        //
        DevInfo->DeviceType  = RetValue;        
    }
    
    //
    // Set Base Class, Subclass and Protocol information
    //
    DevInfo->BaseClass = BaseClass;
    DevInfo->Protocol = Protocol;
    DevInfo->SubClass = SubClass;

    //
    // Device identified. Issue common configure call
    // Call a common routine to handle the remaining initialization that is done
    // for all devices.
    //
    DevInfoLocal = USB_ConfigureDevice(
                        HcStruc,
                        DevInfo,
                        Desc,
                        Length,
                        DescLength);
  
    if (RetValue != USB_ERROR) {

        if (DevInfoLocal == NULL) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USB: Common configure failed.\n");
            return DevInfoLocal;
        }

        DevInfoLocal->DevDriverIndex = Index;
        DevInfoLocal = (*DevDriver->FnConfigureDevice)(
                                      HcStruc,
                                      DevInfoLocal,
                                      Desc,
                                      Length,
                                      DescLength);
        if (!DevInfoLocal || !(DevInfoLocal->Flag & DEV_INFO_VALID_STRUC)) {
            DevInfoLocal = 0;
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USB: Device specific configure failed.\n");
            return DevInfoLocal;
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%x\n", DevInfoLocal);

    return DevInfoLocal;
}

/**
    This routine initializes the frame list pointed by Ptr
    with the Value provided

    @param HcStruc   Pointer to the Host Controller structure
    @param Value     Value to be initialized with

    @retval None

**/


VOID
USB_InitFrameList(
    HC_STRUC*   HcStruc,
    UINT32      Value)
{
    UINT16  Index;
    UINT32  *Ptr = (UINT32*)HcStruc->FrameList;

    for (Index = 0; Index < HcStruc->AsyncListSize; Index ++) {
        Ptr[Index] = Value;
    }
    return;
}


/**
    This function handles different key repeat related functions depending on the input.

    @param HcStruc  Pointer for the HC that implements the key repeat function
    @param Action   Sub-function index:
                    0  Install key repeat HCStruc
                    1  Disable key repeat
                    2  Enable key repeat
                    3  Uninstall key repeat HCStruc

    @retval None

    @note  HcStruc is only relevant for sub-function 0.

**/
VOID
EFIAPI
USBKeyRepeat(
    HC_STRUC*   HcStruc,
    UINT8       Action
)
{
    UINT8    Index;

    switch (Action) {
        case 0:     // Sub-function 0: Save the HCStruc value for later use
            if (gKeyRepeatStatus == FALSE) {
                gUsbData->KeyRepeatHcIndex = HcStruc->HcNumber;
            }
            break;
        case 1:     // Sub-function 0: Disable key repeat
            if (gKeyRepeatStatus) {

                if ((gUsbData->UsbFeature & USB_HID_USE_SETIDLE) == USB_HID_USE_SETIDLE){
                if(gUsbDataList->KeyRepeatDevInfo != NULL) {
                //
                // Set the HID SET_IDLE request to 0
                //
                AmiUsbControlTransfer(gHcTable[gUsbDataList->KeyRepeatDevInfo->HcNumber - 1],
                    gUsbDataList->KeyRepeatDevInfo,(UINT16)HID_RQ_SET_IDLE, gUsbDataList->KeyRepeatDevInfo->InterfaceNum, 0, 0, 0);
                
              }

                } else {
               if (gUsbData->KeyRepeatHcIndex) {
                        AmiUsbDisableKeyRepeat(gHcTable[gUsbData->KeyRepeatHcIndex - 1]);
                }
                }
                gKeyRepeatStatus = FALSE;
            }
            break;
        case 2:     // Sub-function 0: Enable key repeat
            if (!gKeyRepeatStatus) {

                if ((gUsbData->UsbFeature & USB_HID_USE_SETIDLE) == USB_HID_USE_SETIDLE){
                if(gUsbDataList->KeyRepeatDevInfo != NULL) {
                  //
                  // Set the HID SET_IDLE request to 0x200 (8ms)
                  //
                  AmiUsbControlTransfer(gHcTable[gUsbDataList->KeyRepeatDevInfo->HcNumber - 1],
                          gUsbDataList->KeyRepeatDevInfo,(UINT16)HID_RQ_SET_IDLE, gUsbDataList->KeyRepeatDevInfo->InterfaceNum, 0x400, 0, 0);
                }
                } else {
                if (gUsbData->KeyRepeatHcIndex) {
                        AmiUsbEnableKeyRepeat(gHcTable[gUsbData->KeyRepeatHcIndex - 1]);
              }
              }
              gKeyRepeatStatus = TRUE;
            }
            break;
    case 3:
          if (gUsbData->KeyRepeatHcIndex == HcStruc->HcNumber) {
              gUsbData->KeyRepeatHcIndex = 0;
              for (Index = 0; Index < gUsbDataList->HcTableCount; Index++) {
                    if (gHcTable[Index] == NULL) {
                        continue;
                    }
                    if (gHcTable[Index] == HcStruc) {
                        continue;
                    }
                    if (gHcTable[Index]->HwSmiHandle == NULL) {
                        continue;
                    }
                    if (gHcTable[Index]->HcFlag & HC_STATE_RUNNING) {
                        gUsbData->KeyRepeatHcIndex = gHcTable[Index]->HcNumber;
                        if (gKeyRepeatStatus) {
                            gKeyRepeatStatus = FALSE;
                            USBKeyRepeat(NULL, 2);
                        }
                        break;
                  }
              }
          }
      break;
    }
}

/**
    This function calls interrupt call back routine

    @param HcStruc         Pointer to the HCStruc structure
    @param DevInfo         Pointer to the DEV_INFO structure
    @param Td              Pointer to the TD
    @param Buffer          Pointer to device buffer
    @param DataLength      Data length
    
    @retval USB_SUCCESS    On success
    @retval USB_ERROR      Failure

**/
UINT8
EFIAPI
USB_bus_interrupt_handler (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Td,
    UINT8       *Buffer,
    UINT16      DataLength
)
{
    USBHC_INTERRUPT_DEVNINFO_T *Idi = (USBHC_INTERRUPT_DEVNINFO_T *)DevInfo->ExtraData;
    EFI_STATUS  Status;
//      ASSERT(Idi);
    if (Idi == NULL) {
        return USB_SUCCESS;
    }

    if (!(gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI)) {
        return USB_SUCCESS;
    }
        
    Status = InterruptCallBack( 
                 Idi,
                 Buffer,
                 DataLength 
                 );
    if (EFI_ERROR(Status)) {
        return USB_SUCCESS;
    }else{
        return USB_ERROR;
    }
}


/**
    This function adds USB_bus_interrupt_handler to the global callback
    function list.
    
    @param HcStruc         Pointer to the HCStruc structure
    @param DevInfo         Pointer to the DEV_INFO structure
    @param Td              Pointer to the TD
    @param Buffer          Pointer to device buffer
    @param DataLength      Data length
    
    @retval USB_SUCCESS    On success
    @retval USB_ERROR      Failure

**/
VOID
EFIAPI
UsbBusDeviceInit(
    VOID
)
{
    USB_InstallCallBackFunction(USB_bus_interrupt_handler);
    return;
}


/**
    This function assigns the device type and callback function index for the specific
    DevInfo.
    
    @param HcStruc         Pointer to the HCStruc structure
    @param DevInfo         Pointer to the DEV_INFO structure
    @param Desc            Descriptor data
    @param Start           Start offset
    @param End             End offset
    
    @retval DevInfo        The DevInfo

**/
DEV_INFO*
EFIAPI
USB_on_configDev(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Desc,
    UINT16      Start,
    UINT16      End
)
{
    DevInfo->DeviceType      = (UINT8)BIOS_DEV_TYPE_USBBUS;
    DevInfo->CallBackIndex   = USB_InstallCallBackFunction(USB_bus_interrupt_handler);
    return(DevInfo);
}


/**
    Return the device type by checking UsbStateFlag.
    If it is under UEFI mode, it returns Bus type (BIOS_DEV_TYPE_USBBUS).
    
    @param DevInfo                   Pointer to devInfo structure
    @param BaseClass                 USB base class code
    @param SubClass                  USB sub-class code
    @param Protocol                  USB protocol code
    
    @retval BIOS_DEV_TYPE_USBBUS     UEFI mode
    @retval USB_ERROR                Legacy mode

**/
UINT8
EFIAPI
USB_on_identifyDev(
    DEV_INFO*   DevInfo,
    UINT8       BaseClass,
    UINT8       SubClass,
    UINT8       Protocol
)
{

    if (gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI)
        return  BIOS_DEV_TYPE_USBBUS;
    else
        return  USB_ERROR;

}

/**
    Disconnect the target device.
    
    @param DevInfo           Pointer to the devInfo structure

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
USB_on_disconnectDev(
    DEV_INFO* DevInfo
)
{
    return  USB_SUCCESS;
}


/**
    This function fills the USB Bus driver routine pointers

    @param DevDriver    Pointer to the device driver structure

    @retval None
**/
VOID
BusFillDriverEntries(
    DEV_DRIVER  *DevDriver
)
{
    DevDriver->DevType               = BIOS_DEV_TYPE_USBBUS;
    DevDriver->BaseClass             = 0;
    DevDriver->SubClass              = 0;
    DevDriver->Protocol              = 0;
    DevDriver->FnDeviceInit          = UsbBusDeviceInit;
    DevDriver->FnCheckDeviceType     = USB_on_identifyDev;
    DevDriver->FnConfigureDevice     = USB_on_configDev;
    DevDriver->FnDisconnectDevice    = USB_on_disconnectDev;
}


/**
    EFI code will call this function to give a chance for
    SMI dev driver to complete the configuration of device

    Before call, USB device is connected, address is assigned
    and configuration is set. DEV_INFO structure is initalized
    from information parsed from descriptors and linked
    to USBBUS dev driver. Device driver specific to the type
    of USB device wasn't called on this device

    After the call returns, a specific device driver
    initialization was  performed by calling FnCheckDeviceType
    and FnConfigureDevice functions of device driver. Parameters
    to those functions are taken from descriptors downloaded from
    the device. Device preserve old address and active configuration
    
    @param HcStruc         Pointer to the HCStruc structure    
    @param DevInfo         Pointer to devInfo structure
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure  
**/
UINT32 
USB_ReConfigDevice(
    HC_STRUC    *HcStruc, 
    DEV_INFO    *DevInfo
)
{
    INTRF_DESC  *IntrfDesc = NULL;
    UINT8       Config;
    UINT32      Status = USB_SUCCESS;
    UINT8       *Buffer;
    CNFG_DESC   *CnfgDesc = NULL;
    UINT16      OrgTimeOutValue;
    UINT16      TotalLength;
    UINT16      Offset;
    UINT16      DescLength;
    INTRF_DESC  *Intrf;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    EfiStatus = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    gCheckUsbApiParameter = FALSE;

    if (DevInfo->DeviceType != 0 && 
        DevInfo->DeviceType != BIOS_DEV_TYPE_USBBUS) {
        return USB_SUCCESS;
    }

    Buffer = USB_MemAlloc(GET_MEM_BLK_COUNT(MAX_CONTROL_DATA_SIZE));
    if (Buffer == NULL) {
        return USB_ERROR;
    }
    //
    // Find configuration desc
    //
    for (Config = 0; Config < DevInfo->DevDesc.NumConfigs; ++Config){
        
        OrgTimeOutValue = gUsbData->UsbReqTimeOutValue;
        gUsbData->UsbReqTimeOutValue = USB_GET_CONFIG_DESC_TIMEOUT_MS;
        
        CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(
            HcStruc,
            DevInfo,
            Buffer,
            0xFF,
            DESC_TYPE_CONFIG,
            Config);

        if (CnfgDesc == NULL) {
            gUsbData->UsbReqTimeOutValue = OrgTimeOutValue;
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the configuration descriptor\n");
            break;
        }
        
        TotalLength = CnfgDesc->TotalLength;

        if (TotalLength > 0xFF) {
            if (TotalLength > (MAX_CONTROL_DATA_SIZE - 1)) {
                TotalLength = MAX_CONTROL_DATA_SIZE - 1;
            }
            CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(
                        HcStruc,
                        DevInfo,
                        Buffer,
                        TotalLength,
                        DESC_TYPE_CONFIG,
                        Config);
            if (CnfgDesc == NULL) {
                gUsbData->UsbReqTimeOutValue = OrgTimeOutValue;
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the configuration descriptor, the total length is %x\n", TotalLength);
                break;
            }
        }
        
        gUsbData->UsbReqTimeOutValue = OrgTimeOutValue;
        
        if (DevInfo->EndpointSpeed == USB_DEV_SPEED_FULL) {
            FixedDelay(1000);           
        }
        if (CnfgDesc != NULL && CnfgDesc->DescType == DESC_TYPE_CONFIG &&
            DevInfo->ConfigNum == CnfgDesc->ConfigValue){
            break;
        }

        CnfgDesc = NULL;
    }

    if (CnfgDesc) {
        if (CnfgDesc->TotalLength > MAX_CONTROL_DATA_SIZE - 1) {
            CnfgDesc->TotalLength = MAX_CONTROL_DATA_SIZE - 1;
        }
        DescLength = CnfgDesc->TotalLength;
        for (Offset = (UINT16)CnfgDesc->DescLength; Offset <DescLength; Offset = Offset + (UINT16)Intrf->DescLength) {
            Intrf = (INTRF_DESC*)((UINT8*)CnfgDesc + Offset);
            if (Intrf->DescLength == 0) {
                break;
            }
            if (Intrf->DescType == DESC_TYPE_INTERFACE &&
                DevInfo->InterfaceNum == Intrf->InterfaceNum &&
                DevInfo->AltSettingNum == Intrf->AltSettingNum ) {
                IntrfDesc = Intrf;
                break;
            }
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3,
        "USB_reConfigDev:: CfgDsc=%x; IntrfDsc=%x\n",
        CnfgDesc, IntrfDesc);

    if (IntrfDesc && CnfgDesc) {
        Status = USB_ReConfigDevice2(HcStruc, DevInfo,
                                    CnfgDesc, IntrfDesc);
    } else {
        Status = USB_ERROR;
    }

    USB_MemFree(Buffer, (UINT8)(MAX_CONTROL_DATA_SIZE / sizeof(MEM_BLK)));

    return Status;
}

/**
    EFI code will call this function to give a chance for
    SMI dev driver to complete the configuration of device

    Before call, USB device is connected, address is assigned
    and configuration is set. DEV_INFO structure is initalized
    from information parsed from descriptors and linked
    to USBBUS dev driver. Device driver specific to the type
    of USB device wasn't called on this device

    After the call returns, a specific device driver
    initialization was  performed by calling FnCheckDeviceType
    and FnConfigureDevice functions of device driver. Parameters
    to those functions are taken from descriptors downloaded from
    the device. Device preserve old address and active configuration
    
    @param HcStruc           Pointer to the HCStruc structure    
    @param DevInfo           Pointer to devInfo structure
    @param CnfgDesc          Pointer to the configuration descriptor   
    @param IntrfDesc         Pointer to interface descriptor
         
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure  
**/
UINT32
USB_ReConfigDevice2(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    CNFG_DESC   *CnfgDesc,
    INTRF_DESC  *IntrfDesc
)
{

    UINTN           Index;
    UINTN           DriverIndex;
    UINTN           MaxIndex;
    UINT8           RetValue = USB_ERROR;
    DEV_DRIVER      *DevDriver = NULL;
    DEV_INFO        *DevInfoLocal;
    UINT8           BaseClass, SubClass, Protocol;
    EFI_STATUS      Status;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB_ReConfigDevice2.\n");

    Status = UsbHcStrucValidation(HcStruc);

    if (EFI_ERROR(Status)) {
        return USB_ERROR;
    }

    Status = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(Status)) {
        return USB_ERROR;
    }    

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)CnfgDesc, sizeof(CNFG_DESC));
        if (Status != EFI_ABORTED){
            if (EFI_ERROR(Status)) {
                return USB_ERROR;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)CnfgDesc, CnfgDesc->TotalLength);
            if (EFI_ERROR(Status)) {
                return USB_ERROR;
            }
            Status = AmiUsbValidateMemoryBuffer((VOID*)IntrfDesc, sizeof(INTRF_DESC));
            if (EFI_ERROR(Status)) {
                return USB_ERROR;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }
    //
    // Check for non-compliant device. If non-compliant device found then
    // the descriptor values will get updated depending on the need.
    //
    USBCheckNonCompliantDevice (
        HcStruc,
        DevInfo,
        (UINT8*)CnfgDesc,
        CnfgDesc->DescLength,
        CnfgDesc->TotalLength);

    //
    // Check whether device needs to be disable
    //
    if (DevInfo->IncompatFlags & USB_INCMPT_DISABLE_DEVICE)
    {
        return USB_ERROR;
    }

    //
    // Get the base, sub class & protocol values
    //
    BaseClass  = IntrfDesc->BaseClass;
    SubClass   = IntrfDesc->SubClass;
    Protocol   = IntrfDesc->Protocol;

    //
    // Check for matching device driver
    //
    DevInfoLocal = NULL;

    Index = 0;
    MaxIndex = MAX_DEVICE_TYPES;

    for (RetValue = USB_ERROR; Index < MaxIndex && RetValue == USB_ERROR; Index++) {
        
        DevDriver = &gUsbDelayedDrivers[Index];
        if (!DevDriver->DevType) continue;
        
        if (DevDriver->FnCheckDeviceType){
            RetValue = (*DevDriver->FnCheckDeviceType)(DevInfo,BaseClass,SubClass,Protocol);
            if(RetValue != USB_ERROR) DriverIndex = Index;  
        
        } else if((DevDriver->BaseClass == BaseClass) &&
                 (DevDriver->SubClass == SubClass) &&
                 (DevDriver->Protocol == Protocol)){
            RetValue = DevDriver->DevType;
        }
    }
    
    if(RetValue == USB_ERROR) return RetValue;

    //driver was found

    DevInfo->DeviceType = RetValue;
    DevInfo->DevDriverIndex = (DriverIndex + DELAYED_DRIVERS_BIT);
    DevInfoLocal = (*DevDriver->FnConfigureDevice)(
    HcStruc,DevInfo,(UINT8*)CnfgDesc,
    (UINT16)(UINTN)((char*)IntrfDesc - (char*)CnfgDesc),CnfgDesc->TotalLength);

    if ((!DevInfoLocal) ||(DevInfo->DevDriverIndex != (DriverIndex + DELAYED_DRIVERS_BIT))) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_0, "USB_ReConfigDevice2: Device specific configure failed.\n");
        return USB_ERROR;
    }
    return USB_SUCCESS;
}

/**
    Search for the HC_STRUC with specified HcNumber
    
    @param HcNumber          Host controller number         
    @retval HC_STRUC         return the HC_STRUC that is matched.
**/

HC_STRUC*
hcnum2hcstruc(
    UINT8 HcNumber
)
{
    return gHcTable[HcNumber - 1];
}

/**
    Changes global state of USBSMI module to function properly
    in non-EFI OS - without support from EFI drivers

    Before call USB BUS is a driver that handles all devices (
    except hub) and rest of the drivers are delayed. Number of
    devices are supported by SUBBUS driver and custom EFI driver

    After call returns, USBBUS driver is removed and all drivers
    that where delayed became active. All USBBUS devices are reconfigured.
    Devices that are not supported by now active drivers are decon-
    figured.
            
    @param  None    
    @retval None
**/

VOID
PrepareForLegacyOs(
    VOID
)
{
    UINTN       Index;
    DEV_INFO    *DevInfo;
    HC_STRUC    *HcStruc;

    gCheckUsbApiParameter = FALSE;

    //
    //First Reconfigure all USBBUS device (while drivers are in delayed array)
    //
    for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
        DevInfo = &gUsbDataList->DevInfoTable[Index];
        if (((DevInfo->Flag & DEV_INFO_VALIDPRESENT) == DEV_INFO_VALIDPRESENT) &&
            (DevInfo->DeviceType == BIOS_DEV_TYPE_USBBUS)) {
            HcStruc = hcnum2hcstruc(DevInfo->HcNumber);
            USB_ReConfigDevice(HcStruc, DevInfo);
        }
    }
 
    USBKeyRepeat(NULL, 1);  // Disable key repeat
    
    //Switch Global data
    AmiUsbGlobalDataSwitch( &gUsbData, 
                            &gUsbDataList, 
                            (HC_STRUC**)&gHcTable,
                            &gDataInSmm, 
                            USB_DATA_DXE_TO_SMM);
                        
}

/**
    This routine resets and reconfigures the device.
    
    @param HcStruc         Pointer to the HCStruc structure    
    @param DevInfo         Pointer to devInfo structure
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure 

**/
UINT32
USB_ResetAndReconfigDev(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    DEV_INFO    *Dev;
    UINT32      Status;
    UINT8       DevAddr;
    UINT8       *Buffer;
    DEV_DESC    *DevDesc;
    CNFG_DESC   *CnfgDesc;
    INTRF_DESC  *IntrfDesc;
    UINT8       ConfigIndx;
    UINT8       IntrfIndx;
    DEV_DRIVER  *DevDriver;
    UINT8       i;
    UINT32      PortStatus;
    UINT8       *DevMiscInfo = NULL;
    UINT16      TotalLength;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    EfiStatus = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    gCheckUsbApiParameter = FALSE;
    
    for (i = 1; i < gUsbData->MaxDevCount; i++) {
      Dev = &gUsbDataList->DevInfoTable[i];
      if ((Dev->Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT | 
           DEV_INFO_DEV_DUMMY)) != (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT)) {
           continue;
      }
      if ((Dev->HubDeviceNumber == DevInfo->HubDeviceNumber) && 
          (Dev->HubPortNumber == DevInfo->HubPortNumber) &&
          (Dev->DeviceType != BIOS_DEV_TYPE_USBBUS)) {
                    AmiUsbDeactivatePolling(HcStruc, Dev);
       }
     }


    Status = AmiUsbDeinitDeviceData(HcStruc, DevInfo);
    if (Status != USB_SUCCESS) {
      return Status;
    }
  
    Status = USB_ResetHubPort(HcStruc, DevInfo->HubDeviceNumber, DevInfo->HubPortNumber);
    if (Status != USB_SUCCESS) {
      return Status;
    }

    Status = USB_EnableHubPort(HcStruc, DevInfo->HubDeviceNumber, DevInfo->HubPortNumber);
    if (Status != USB_SUCCESS) {
      return Status;
    }
    
    PortStatus = USB_GetHubPortStatus(HcStruc, DevInfo->HubDeviceNumber, DevInfo->HubPortNumber, TRUE);

    if (PortStatus == USB_ERROR) {
        return USB_ERROR;
    }

    if (!(PortStatus & USB_PORT_STAT_DEV_ENABLED)) {
        return USB_ERROR;
    }

    Status = AmiUsbInitDeviceData(HcStruc, DevInfo, PortStatus, &DevMiscInfo);
    if (Status != USB_SUCCESS) {
        return Status;
    }

    DevInfo->DevMiscInfo = (VOID*)DevMiscInfo;

    Buffer = USB_MemAlloc(GET_MEM_BLK_COUNT(sizeof(DEV_DESC)));
    if (Buffer == NULL) {
        return USB_ERROR;
    }

    DevAddr = DevInfo->DeviceAddress;
    DevInfo->DeviceAddress = 0;

    DevDesc = (DEV_DESC*)USB_GetDescriptor(HcStruc, DevInfo, Buffer, sizeof(DEV_DESC), 
        DESC_TYPE_DEVICE, 0);
    if (DevDesc == NULL) {
      USB_MemFree(Buffer, GET_MEM_BLK_COUNT(sizeof(DEV_DESC)));
      return USB_ERROR;
    }

    Status = USB_SetAddress(HcStruc, DevInfo, DevAddr);  
    if (Status != USB_SUCCESS) {
      USB_MemFree(DevDesc, GET_MEM_BLK_COUNT(sizeof(DEV_DESC)));
      return Status;
    }  

    DevInfo->DeviceAddress = DevAddr;

    Buffer = USB_MemAlloc(GET_MEM_BLK_COUNT(MAX_CONTROL_DATA_SIZE));
    if (Buffer == NULL) {
      USB_MemFree(DevDesc, GET_MEM_BLK_COUNT(sizeof(DEV_DESC)));
      return USB_ERROR;
    }

    for (ConfigIndx = 0; ConfigIndx < DevDesc->NumConfigs; ConfigIndx++) {
       CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(HcStruc, DevInfo, Buffer, 
            0xFF, DESC_TYPE_CONFIG, ConfigIndx);
       if (CnfgDesc == NULL) {
         continue;
       }
       TotalLength = CnfgDesc->TotalLength;
        if (TotalLength > 0xFF) {
            if (TotalLength > (MAX_CONTROL_DATA_SIZE - 1)) {
                TotalLength = MAX_CONTROL_DATA_SIZE - 1;
            }
            CnfgDesc = (CNFG_DESC*)USB_GetDescriptor(HcStruc, DevInfo, Buffer, 
            TotalLength, DESC_TYPE_CONFIG, ConfigIndx);
            if (CnfgDesc == NULL) {
                continue;
            }
        }

        if (CnfgDesc->DescType == DESC_TYPE_CONFIG) {
            AmiUsbEnableEndpoints(HcStruc, DevInfo, (UINT8*)CnfgDesc);
        }

        USB_SetConfig(HcStruc, DevInfo, CnfgDesc->ConfigValue);
  
        IntrfDesc = (INTRF_DESC*)CnfgDesc;
        for (IntrfIndx = 0; IntrfIndx < CnfgDesc->NumInterfaces; IntrfIndx++) {
        do {
          IntrfDesc = (INTRF_DESC*)((UINTN)IntrfDesc + IntrfDesc->DescLength);
          if ((UINTN)IntrfDesc > ((UINTN)CnfgDesc + CnfgDesc->TotalLength) ||
             (UINTN)IntrfDesc > ((UINTN)CnfgDesc + MAX_CONTROL_DATA_SIZE)) {
             break;
          }
        } while (IntrfDesc->DescType != DESC_TYPE_INTERFACE);

        if (IntrfDesc->DescType != DESC_TYPE_INTERFACE) {
          break;
        }

        for (i = 1; i < gUsbData->MaxDevCount; i++) {
          Dev = &gUsbDataList->DevInfoTable[i];
          if ((Dev->Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT | 
               DEV_INFO_DEV_DUMMY)) != (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT)) {
               continue;
          }
          if ((Dev->HubDeviceNumber == DevInfo->HubDeviceNumber) && 
              (Dev->HubPortNumber == DevInfo->HubPortNumber) &&
              (Dev->ConfigNum == CnfgDesc->ConfigValue) &&
              (Dev->InterfaceNum == IntrfDesc->InterfaceNum) &&
              (Dev->AltSettingNum == IntrfDesc->AltSettingNum)) {
              break;
          }
      }
      if (i == gUsbData->MaxDevCount) {
        continue;
      }

      Dev->VendorId = DevDesc->VendorId;
      Dev->DeviceId = DevDesc->DeviceId;

      if (Dev->DeviceType != BIOS_DEV_TYPE_USBBUS) {
        DevDriver = UsbFindDeviceDriverEntry(DevInfo->DevDriverIndex);

        if (DevDriver != NULL) {
                    (*DevDriver->FnConfigureDevice)(HcStruc, Dev, (UINT8*)CnfgDesc,
                        (UINT16)((UINTN)IntrfDesc - (UINTN)CnfgDesc), CnfgDesc->TotalLength);
        }
      }
    }
  }

  USB_MemFree(DevDesc, GET_MEM_BLK_COUNT(sizeof(DEV_DESC)));
  USB_MemFree(Buffer, GET_MEM_BLK_COUNT(MAX_CONTROL_DATA_SIZE));

  return USB_SUCCESS;
}


/**
    This routine resets and reconfigures the device.
    
    @param HcStruc         Pointer to the HCStruc structure    
    @param DevInfo         Pointer to devInfo structure
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure 

**/
UINT32
USB_DevDriverDisconnect(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    DEV_DRIVER* DevDriver;
    UINT8       Index;
    UINT8       MaxIndex;
    EFI_STATUS  EfiStatus;

    EfiStatus = UsbHcStrucValidation(HcStruc);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    EfiStatus = UsbDevInfoValidation(DevInfo);

    if (EFI_ERROR(EfiStatus)) {
        return USB_ERROR;
    }

    gCheckUsbApiParameter = FALSE;

    DevDriver = UsbFindDeviceDriverEntry(DevInfo->DevDriverIndex);
  
    if (DevDriver && DevDriver->FnDisconnectDevice) {
        DevDriver->FnDisconnectDevice(DevInfo);

        DevInfo->DeviceType = 0;
        DevInfo->DevDriverIndex = NULL_DRIVERS_INDEX;


        Index = 0;
        MaxIndex = MAX_DEVICE_TYPES;
        for (; Index < MaxIndex; Index++) {
            DevDriver = &gUsbDevDriverTable[Index];

            if (DevDriver->DevType == BIOS_DEV_TYPE_USBBUS) {
                DevInfo->DeviceType = DevDriver->DevType;
                DevDriver->FnConfigureDevice(HcStruc, DevInfo, NULL, 0, 0);
                break;
            }
        }
    } else {
        if (DevInfo->IntInEndpoint) {

            // Stop polling the device's interrupt endpoint
            AmiUsbDeactivatePolling(HcStruc, DevInfo);
            DevInfo->IntInEndpoint = 0;
        }
    }

    return USB_SUCCESS;
}

/**
    Checks if DEV_INFO is a valid connected device info
    Due to hot-plug a DEV_INFO can become invalid in the
    midle of configuration.
    
    @param  DevInfo         Pointer to devInfo structure    
    @retval TRUE            The devInfo is valid.
    @retval FALSE           The devInfo is not valid.
**/
int VALID_DEVINFO(DEV_INFO* DevInfo)
{
    return (DevInfo->Flag & DEV_INFO_VALIDPRESENT)!=0;
}


/**
    Mark DEV_INFO not valid for all the devices connected to a
    given hub.
    
    @param  HubAddr         Hub address
    @retval None
**/

VOID
USB_AbortConnectHubChildren(
    UINT8 HubAddr
)
{
    UINT8       Index;
    DEV_INFO    *Dev = &gUsbDataList->DevInfoTable[1];

    for (Index = 1;  Index < gUsbData->MaxDevCount; Index++, Dev++) {
        if ((Dev->HubDeviceNumber == HubAddr) && (Dev->Flag & DEV_INFO_VALIDPRESENT)) {
             Dev->Flag &= ~DEV_INFO_DEV_PRESENT;
             if (!(Dev->Flag & DEV_INFO_MASS_DEV_REGD)) {
                 Dev->Flag &= ~DEV_INFO_VALID_STRUC;
             }

            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB: abort device [%x] connected to hub[%x]\n",
                Dev->DeviceAddress, HubAddr);

            if (Dev->DeviceType == BIOS_DEV_TYPE_HUB) {
                USB_AbortConnectHubChildren(Dev->DeviceAddress);
            }
        }
    }
}


/**
    This routine releases the given device's address by
    updating gUsbData->dDeviceAddressMap.
    
    @param  DevInfo         Pointer to devInfo structure    
    @retval None
**/

VOID
USB_FreeDeviceAddress(
    DEV_INFO    *DevInfo
)
{
    UINT8   Index;
    UINT8   Found = 0;

    if (DevInfo->DeviceAddress) {
        for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
            if (gUsbDataList->DevInfoTable+Index != DevInfo &&
                gUsbDataList->DevInfoTable[Index].DeviceAddress == DevInfo->DeviceAddress) {
                Found++;
            }
        }
        if (Found == 0){
            //The DevInfo was the only function with allocated address -
            // return the address to the pool
            if (DevInfo->DeviceAddress != 0) {
                gUsbData->DeviceAddressMap |= LShiftU64(1, DevInfo->DeviceAddress);
                DevInfo->DeviceAddress = 0;
            }
        }
    }
}


/**
    Mark DEV_INFO not valid and release its device address
    
    @param  DevInfo         Pointer to devInfo structure    
    @retval None
**/

VOID
USB_AbortConnectDev(
    DEV_INFO* DevInfo
)
{
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USB: abort connect [%x].flag = %x\n",
          DevInfo, DevInfo->Flag);

    DevInfo->Flag &= ~DEV_INFO_DEV_PRESENT;

    if (!(DevInfo->Flag & DEV_INFO_MASS_DEV_REGD)) {
        DevInfo->Flag &= ~DEV_INFO_VALID_STRUC;
        if (DevInfo->DeviceAddress == 0) return;

      USB_FreeDeviceAddress(DevInfo);
    }

    // Remove children (if any) from aborted parent hub device.
    // Assume the child device has not been connected since
    // the hub has to be connected first.
    if (DevInfo->DeviceType == BIOS_DEV_TYPE_HUB) {
        USB_AbortConnectHubChildren(DevInfo->DeviceAddress);
    }
}

/**
    Verifies whether the number of initialized devices of a given
    class has reached the limit.
    
    @param  BaseClass        The device base class    
    @retval TRUE             Reach the limit for supporting this kind of device
    @retval FALSE            Does not reach the limit
**/
BOOLEAN
CheckDeviceLimit(
    UINT8   BaseClass
)
{
    URP_STRUC Urp;
    UINT8 DevNumber;

    Urp.bFuncNumber = USB_API_CHECK_DEVICE_PRESENCE;
    Urp.bSubFunc = 1;
    Urp.ApiData.ChkDevPrsnc.HcStruc = NULL;
    Urp.ApiData.ChkDevPrsnc.DevType = BaseClass;

    USBAPI_CheckDevicePresence(&Urp);

    if (Urp.bRetValue == USB_SUCCESS)
    {
        DevNumber = Urp.ApiData.ChkDevPrsnc.Number;
        if ((BaseClass == BASE_CLASS_HID)
              && ((gUsbData->MaxHidCount == 0) || (DevNumber == gUsbData->MaxHidCount)))
        {
            USB_DEBUG(DEBUG_INFO, 3, "Reached the limit of supported HIDs (%d); skipping this device.\n", gUsbData->MaxHidCount);
            return TRUE;
        }

        if ((BaseClass == BASE_CLASS_HUB)
              && ((gUsbData->MaxHubCount == 0) || (DevNumber == gUsbData->MaxHubCount)))
        {
            USB_DEBUG(DEBUG_INFO, 3, "Reached the limit of supported HUBs (%d); skipping this device.\n", gUsbData->MaxHubCount);
            return TRUE;
        }

        if ((BaseClass == BASE_CLASS_MASS_STORAGE)
              && ((gUsbData->MaxMassCount == 0) || (DevNumber == gUsbData->MaxMassCount)))
        {
            USB_DEBUG(DEBUG_INFO, 3, "Reached the limit of supported Mass Storage Devices (%d); skipping this device.\n", gUsbData->MaxMassCount);
            return TRUE;
        }
        if ((BaseClass == BASE_CLASS_CCID_STORAGE)
              && ((gUsbData->MaxCcidCount == 0) || (DevNumber == gUsbData->MaxCcidCount+1)))
        {
            USB_DEBUG(DEBUG_INFO, 3, "Reached the limit of supported CCID Devices (%d); skipping this device.\n", gUsbData->MaxCcidCount);
            return TRUE;
        }
    }
    return FALSE;
}


/**
     The routine send control transfer with the command request.
    
    @param HcStruc           Pointer to the HCStruc structure    
    @param DevInfo           Pointer to devInfo structure
    @param DevReq            Request command
    @param Timeout           Request timeout value which the transfer is allowed to complete.
    @param Buffer            Buffer containing data to be sent to the device or buffer to be used to receive data
              
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure 
**/
UINT8
UsbControlTransfer(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    DEV_REQ     DevReq,
    UINT16      Timeout,
    VOID*       Buffer)
{
    UINT16      Status;
    UINT16      SavedTimeout;

    SavedTimeout = gUsbData->UsbReqTimeOutValue;
    gUsbData->UsbReqTimeOutValue = Timeout;

    Status = AmiUsbControlTransfer(
                 HcStruc,
                 DevInfo,
                 DevReq.RequestType,
                 DevReq.Index,
                 DevReq.Value,
                 Buffer,
                 DevReq.DataLength);
    gUsbData->UsbReqTimeOutValue = SavedTimeout;

    return DevReq.DataLength && (Status == 0)? USB_ERROR : USB_SUCCESS;
}

/**
    This function executes an interrupt transaction on the USB.
    @param HcStruc            Pointer to HCStruc of the host controller.
    @param DevInfo            DeviceInfo structure (if available else 0).
    @param EndpointAddress    The destination USB device endpoint to which the device request 
                              is being sent.
    @param MaxPktSize         Indicates the maximum packet size the target endpoint is capable 
                              of sending or receiving.
    @param Buffer             Buffer containing data to be sent to the device or buffer to be
                              used to receive data.
    @param Length             Length request parameter, number of bytes of data to be transferred.
    @param Timeout            Indicates the maximum time, in milliseconds, which the transfer 
                              is allowed to complete.
                    
    @retval USB_SUCCESS       On success
    @retval USB_ERROR         Failure

**/

UINT8
UsbInterruptTransfer (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       EndpointAddress,
    UINT16      MaxPktSize,
    VOID        *Buffer,
    UINT16      Length,
    UINT16      Timeout
)
{
    UINT16  SavedTimeout;
    UINT16  BytesTransferred;

    SavedTimeout = gUsbData->UsbReqTimeOutValue;
    gUsbData->UsbReqTimeOutValue = Timeout;

    BytesTransferred = AmiUsbInterruptTransfer(
                           HcStruc,
                           DevInfo,
                           EndpointAddress,
                           MaxPktSize,
                           Buffer,
                           Length);

    gUsbData->UsbReqTimeOutValue = SavedTimeout;

    if (BytesTransferred == 0) {
        return USB_ERROR;
    } else {
        return USB_SUCCESS;
    }
    
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement enable endpoint function.
    
    @param HcdHeader         Pointer to the HCD_HEADER

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
UsbFillHcdEntriesDummy(
    HCD_HEADER *HcdHeader
)
{
    HcdHeader->FnHcdStart                = UsbHcdStartDummy;
    HcdHeader->FnHcdStop                 = UsbHcdStopDummy;
    HcdHeader->FnHcdEnumeratePorts       = UsbHcdEnumeratePortsDummy;
    HcdHeader->FnHcdDisableInterrupts    = UsbHcdDisableInterruptsDummy;
    HcdHeader->FnHcdEnableInterrupts     = UsbHcdEnableInterruptsDummy;
    HcdHeader->FnHcdProcessInterrupt     = UsbHcdProcessInterruptDummy;
    HcdHeader->FnHcdGetRootHubStatus     = UsbHcdGetRootHubStatusDummy;
    HcdHeader->FnHcdDisableRootHub       = UsbHcdDisableRootHubDummy;
    HcdHeader->FnHcdEnableRootHub        = UsbHcdEnableRootHubDummy;
    HcdHeader->FnHcdControlTransfer      = UsbHcdControlTransferDummy;
    HcdHeader->FnHcdBulkTransfer         = UsbHcdBulkTransferDummy;
    HcdHeader->FnHcdIsocTransfer         = UsbHcdIsocTransferDummy;    
    HcdHeader->FnHcdInterruptTransfer    = UsbHcdInterruptTransferDummy;
    HcdHeader->FnHcdDeactivatePolling    = UsbHcdDeactivatePollingDummy;
    HcdHeader->FnHcdActivatePolling      = UsbHcdActivatePollingDummy;
    HcdHeader->FnHcdDisableKeyRepeat     = UsbHcdDisableKeyRepeatDummy;
    HcdHeader->FnHcdEnableKeyRepeat      = UsbHcdEnableKeyRepeatDummy;
    HcdHeader->FnHcdEnableEndpoints      = UsbHcdEnableEndpointsDummy;
    HcdHeader->FnHcdInitDeviceData       = UsbHcdInitDeviceDataDummy;
    HcdHeader->FnHcdDeinitDeviceData     = UsbHcdDeinitDeviceDataDummy;
    HcdHeader->FnHcdResetRootHub         = UsbHcdResetRootHubDummy;
    HcdHeader->FnHcdClearEndpointState    = UsbHcdClearEndpointStateDummy;
    HcdHeader->FnHcdGlobalSuspend        = UsbHcdGlobalSuspendDummy;
    HcdHeader->FnHcdSmiControl           = UsbHcdSmiControlDummy;

    return  USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement host controller start function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdStartDummy(
    HC_STRUC    *HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement host controller stop function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdStopDummy(
    HC_STRUC    *HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement enumerate USB ports function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdEnumeratePortsDummy(
    HC_STRUC    *HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement disable interrupts function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdDisableInterruptsDummy(
    HC_STRUC* HcStruc
)
{  
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement enable interrupts function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdEnableInterruptsDummy(
    HC_STRUC* HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement process interrupts function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdProcessInterruptDummy(
    HC_STRUC    *HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement get root hub status function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT32
EFIAPI
UsbHcdGetRootHubStatusDummy(
    HC_STRUC*   HcStruc,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
)
{
    return 0;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement disable root hub function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdDisableRootHubDummy(
    HC_STRUC    *HcStruc,
    UINT8       PortNum
)
{
    return USB_SUCCESS;
}


/**
    Dummy HC API function used by the HC drivers that do not need
    to implement enable root hub function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
UsbHcdEnableRootHubDummy(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
)
{
    return USB_SUCCESS;
}


/**
    Dummy HC API function used by the HC drivers that do not need
    to implement control transfer function.
    
    @param HcStruc           Pointer to the HCStruc structure 

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT16
EFIAPI
UsbHcdControlTransferDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT16      Request,
    UINT16      Index,
    UINT16      Value,
    UINT8       *Buffer,
    UINT16      Length
)
{
    return 0;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Bulk transfer function.

    @param HcStruc   Pointer to HCStruc of the host controller
    @param DevInfo   DeviceInfo structure (if available else 0)
    @param XferDir   Transfer direction
                     Bit 7: Data direction
                            0 Host sending data to device
                            1 Device sending data to host
                     Bit 6-0 : Reserved
    @param Buffer    Buffer containing data to be sent to the device or buffer to
                     be used to receive data value
    @param Length    Length request parameter, number of bytes of data to be
                     transferred in or out of the HC
    @retval 0        Amount of data transferred


**/
UINT32
EFIAPI
UsbHcdBulkTransferDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length
)
{
    return 0;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Bulk transfer function.

    @param HcStruc   Pointer to HCStruc of the host controller
    @param DevInfo   DeviceInfo structure (if available else 0)
    @param XferDir   Transfer direction
                     Bit 7: Data direction
                            0 Host sending data to device
                            1 Device sending data to host
                     Bit 6-0 : Reserved
    @param Buffer    Buffer containing data to be sent to the device or buffer to
                     be used to receive data value
    @param Length    Length request parameter, number of bytes of data to be
                     transferred in or out of the HC
                     
  @param AsyncIndicator  asynchronous indicator
    @retval 0        Amount of data transferred


**/
UINT32
EFIAPI
UsbHcdIsocTransferDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       XferDir,
    UINT8       *Buffer,
    UINT32      Length,
    UINT8       *AsyncIndicator
)
{
    return 0;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Interrupt Transfer function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             DeviceInfo structure (if available else 0)
    @param EndpointAddress     The destination USB device endpoint to which the device request 
                               is being sent.
    @param MaxPktSize          Indicates the maximum packet size the target endpoint is capable 
                               of sending or receiving.
    @param Buffer              Buffer containing data to be sent to the device or buffer to be
                               used to receive data
    @param Length              Length request parameter, number of bytes of data to be transferred

    @retval 0                  Number of bytes transferred


**/
UINT16
EFIAPI
UsbHcdInterruptTransferDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       EndpointAddress,
    UINT16      MaxPktSize,
    UINT8       *Buffer,
    UINT16      Length
)
{
    return 0;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Deactivate Polling function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             DeviceInfo structure (if available else 0)

    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdDeactivatePollingDummy(
    HC_STRUC* HcStruc,
    DEV_INFO* DevInfo
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Activate Polling function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             DeviceInfo structure (if available else 0)

    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdActivatePollingDummy(
    HC_STRUC* HcStruc,
    DEV_INFO* DevInfo
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement disable Key Repeat function.

    @param HcStruc             Pointer to HCStruc of the host controller

    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdDisableKeyRepeatDummy(
    HC_STRUC* HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Enable Key Repeat function.

    @param HcStruc             Pointer to HCStruc of the host controller

    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdEnableKeyRepeatDummy(
    HC_STRUC* HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement Enable EndPoint function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             A device for which the endpoins are being enabled
    @param Desc                Device Configuration Descriptor data pointer
    
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdEnableEndpointsDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Desc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement initial Device Data function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             A device for which the endpoins are being enabled
    @param PortStatus          Port status
    @param PortStatus          Port status    
    @retval DeviceData         Device Data
    
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdInitDeviceDataDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT32      PortStatus,
    UINT8       **DeviceData
)
{

    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement deinitial Device Data function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             A device for which the endpoins are being enabled
    
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdDeinitDeviceDataDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo
)
{
    return USB_SUCCESS;
}


/**
    Dummy HC API function used by the HC drivers that do not need
    to implement reset root hub function.

    @param HcStruc             Pointer to the HC structure
    @param PortNum             Port in the HC to disable
    
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdResetRootHubDummy(
    HC_STRUC*   HcStruc,
    UINT8       PortNum
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement clear endpoint state function.

    @param HcStruc             Pointer to HCStruc of the host controller
    @param DevInfo             A device for which the endpoins are being cleared
    @param Endpoint            The target endpoint
     
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdClearEndpointStateDummy(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       Endpoint
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement global suspend function.

    @param HcStruc             Pointer to HCStruc of the host controller
     
    @retval USB_SUCCESS        On success
    @retval USB_ERROR          Failure
**/
UINT8
EFIAPI
UsbHcdGlobalSuspendDummy(
    HC_STRUC*  HcStruc
)
{
    return USB_SUCCESS;
}

/**
    Dummy HC API function used by the HC drivers that do not need
    to implement set SMI control function.

    @param HcStruc           Pointer to HCStruc of the host controller
    @param Enable            TRUE : Enable
    
    @retval USB_SUCCESS     Success
    @retval USB_ERROR       Failure
**/
UINT8
EFIAPI
UsbHcdSmiControlDummy(
    HC_STRUC* HcStruc,
    BOOLEAN Enable
)
{
    return USB_SUCCESS;
}


/**
    Get data toggle 
    
    @param DevInfo             Pointer to DEV_INFO structure
    @param EndpointAddr        The target endpoint address
    
    @retval USB_SUCCESS     Success
    @retval USB_ERROR       Failure
**/
UINT8
UsbGetDataToggle(
    DEV_INFO  *DevInfo,
    UINT8      EndpointAddr
)
{
    DEV_INFO    *DevInfoToToggle;
    UINT8       ToggleBit = (EndpointAddr & 0xF) - 1;
    UINT16      *DataSync;
    EFI_STATUS  Status;

    if (DevInfo->Lun0DevInfoPtr) {
        Status = UsbDevInfoValidation(DevInfo->Lun0DevInfoPtr);
        if (EFI_ERROR(Status)) {
            return 0;
        }  
        DevInfoToToggle = DevInfo->Lun0DevInfoPtr;
    } else {
        DevInfoToToggle = DevInfo;
    }

    if (EndpointAddr & BIT7) {
        DataSync = &DevInfoToToggle->DataInSync;
    } else {
        DataSync = &DevInfoToToggle->DataOutSync;
    }

    return (UINT8)((*DataSync) >> ToggleBit) & 0x1;
}

/**
    Update data toggle 
    
    @param DevInfo             Pointer to DEV_INFO structure
    @param EndpointAddr        The target endpoint address
    @param DataToggle          The data toggle value
        
    @retval USB_SUCCESS     Success
    @retval USB_ERROR       Failure
**/
VOID
UsbUpdateDataToggle(
    DEV_INFO  *DevInfo,
    UINT8      EndpointAddr,
    UINT8      DataToggle
)
{
    DEV_INFO    *DevInfoToToggle;
    UINT8       ToggleBit = (EndpointAddr & 0xF) - 1;
    UINT16      *DataSync;
    EFI_STATUS  Status;

    if (DevInfo->Lun0DevInfoPtr) {
        Status = UsbDevInfoValidation(DevInfo->Lun0DevInfoPtr);
        if (EFI_ERROR(Status)) {
            return;
        }  
        DevInfoToToggle = DevInfo->Lun0DevInfoPtr;
    } else {
        DevInfoToToggle = DevInfo;
    }

    if (EndpointAddr & BIT7) {
        DataSync = &DevInfoToToggle->DataInSync;
    } else {
        DataSync = &DevInfoToToggle->DataOutSync;
    }
    
    *DataSync &= (UINT16)~(1 << ToggleBit);
    *DataSync |= (UINT16)(DataToggle << ToggleBit);
    return;
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
