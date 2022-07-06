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

/** @file UsbHub.c
    AMI USB Hub support implementation

**/

#include "AmiUsb.h"
#include <Library/AmiUsbHcdLib.h>

extern  USB_GLOBAL_DATA *gUsbData;
extern  USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

VOID*   USB_MemAlloc(UINT16);
UINT8   USB_MemFree(void _FAR_*, UINT16);
DEV_INFO*   EFIAPI USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);
UINT8   USB_StopDevice (HC_STRUC*,  UINT8, UINT8);
VOID    EFIAPI FixedDelay(UINTN);
UINT8   USB_ProcessPortChange (HC_STRUC*, UINT8, UINT8, UINT8);
UINT8   EFIAPI USB_InstallCallBackFunction (CALLBACK_FUNC  pfnCallBackFunction);
UINT8   USBCheckPortChange (HC_STRUC*, UINT8, UINT8);
UINT8   UsbControlTransfer(HC_STRUC*, DEV_INFO*, DEV_REQ, UINT16, VOID*);

UINT8   EFIAPI USBHUBDisconnectDevice (DEV_INFO*);
UINT8   USBHub_EnablePort(HC_STRUC*, UINT8, UINT8);
UINT8   USBHub_DisablePort(HC_STRUC*, UINT8, UINT8);
UINT8   USBHub_ResetPort(HC_STRUC*, UINT8, UINT8);

VOID    EFIAPI UsbHubDeviceInit(VOID);
UINT8   EFIAPI USBHubCheckDeviceType (DEV_INFO*, UINT8, UINT8, UINT8);
UINT8   EFIAPI USBHub_ProcessHubData(HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
DEV_INFO* EFIAPI USBHUBConfigureDevice (HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16);
UINT8   UsbHubResetPort(HC_STRUC*, DEV_INFO*, UINT8, BOOLEAN);

UINT8   UsbHubCearHubFeature(HC_STRUC*, DEV_INFO*, HUB_FEATURE);
UINT8   UsbHubClearPortFeature(HC_STRUC*, DEV_INFO*, UINT8, HUB_FEATURE);
UINT8   UsbHubGetHubDescriptor(HC_STRUC*, DEV_INFO*, VOID*, UINT16);
UINT8   UsbHubGetHubStatus(HC_STRUC*, DEV_INFO*, UINT32*);
UINT8   UsbHubGetPortStatus(HC_STRUC*, DEV_INFO*, UINT8, UINT32*);
UINT8   UsbHubGetErrorCount(HC_STRUC*, DEV_INFO*, UINT8, UINT16*);
UINT8   UsbHubSetHubDescriptor(HC_STRUC*, DEV_INFO*, VOID*, UINT16);
UINT8   UsbHubSetHubFeature(HC_STRUC*, DEV_INFO*, HUB_FEATURE);
UINT8   UsbHubSetHubDepth(HC_STRUC*, DEV_INFO*, UINT16);
UINT8   UsbHubSetPortFeature(HC_STRUC*, DEV_INFO*, UINT8, HUB_FEATURE);

VOID
USBHubFillDriverEntries(
    DEV_DRIVER *DevDriver
)
{
    DevDriver->DevType               = BIOS_DEV_TYPE_HUB;
    DevDriver->Protocol               = 0;
    DevDriver->FnDeviceInit          = UsbHubDeviceInit;
    DevDriver->FnCheckDeviceType     = USBHubCheckDeviceType;
    DevDriver->FnConfigureDevice     = USBHUBConfigureDevice;
    DevDriver->FnDisconnectDevice    = USBHUBDisconnectDevice;
}



/**
    This function checks an interface descriptor of a device
    to see if it describes a USB hub.  If the device is a hub,
    then it is configured and initialized.

    @param HcStruc    HcStruc pointer
    @param DevInfo    Device information structure pointer
    @param Desc       Pointer to the descriptor structure
                      supported by the device
    @param Start      Start offset of the device descriptor
    @param End        End offset of the device descriptor

    @retval DevInfo   New device info structure, 0 on error

**/

DEV_INFO*
EFIAPI
USBHUBConfigureDevice(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Start,
    UINT16      End
)
{
    UINT8           PortNum;
    UINTN           DelayValue;
    UINT8*          Buffer;
    HUB_DESC        *HubDesc;
    UINT8           Status;
    DEV_INFO*       ParentHub;
    BOOLEAN         SetPortPower = FALSE;
    ENDP_DESC       *EndpDesc;
    INTRF_DESC      *IntrfDesc;
    INTRF_DESC      *AltIntrfDesc;
    UINT16          DescLength;
    UINT16          TotalLength;

    USB3_HUB_PORT_STATUS  *Usb3HubPortSts = NULL;

    if (gUsbDataList->HubPortStatusBuffer == NULL) {
        gUsbDataList->HubPortStatusBuffer = (UINT32*)USB_MemAlloc(1);
        if (gUsbDataList->HubPortStatusBuffer == NULL) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "USB_MemAlloc for HubPortStatusBuffer fail, out of resource \n");
            return NULL;
        }
    }
    Usb3HubPortSts = (USB3_HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;

    //
    // Set the BiosDeviceType field in DeviceTableEntry[0].  This serves as a flag
    // that indicates a usable interface has been found in the current
    // configuration. This is needed so we can check for other usable interfaces
    // in the current configuration (i.e. composite device), but not try to search
    // in other configurations.
    //
    DevInfo->DeviceType = BIOS_DEV_TYPE_HUB;
    DevInfo->CallBackIndex = USB_InstallCallBackFunction(USBHub_ProcessHubData);

    IntrfDesc = (INTRF_DESC*)(Desc + Start);

    // Check if the hub supports multiple TTs.
    if (DevInfo->Flag & DEV_INFO_ALT_SETTING_IF) {
        DescLength = Start;
        TotalLength = ((CNFG_DESC*)Desc)->TotalLength;
        for (;DescLength < TotalLength;) {
            AltIntrfDesc = (INTRF_DESC*)(Desc + DescLength);
            if ((AltIntrfDesc->DescLength == 0) || 
                ((AltIntrfDesc->DescLength + DescLength) > TotalLength)) {
                break;
            }
            if ((AltIntrfDesc->DescType == DESC_TYPE_INTERFACE) && (AltIntrfDesc->AltSettingNum != 0)) {
                if ((AltIntrfDesc->BaseClass == BASE_CLASS_HUB) && 
                    (AltIntrfDesc->SubClass == SUB_CLASS_HUB) && 
                    (AltIntrfDesc->Protocol == PROTOCOL_HUB_MULTIPLE_TTS)) {
                    DevInfo->Protocol = AltIntrfDesc->Protocol;
                    DevInfo->AltSettingNum = AltIntrfDesc->AltSettingNum;
                    IntrfDesc = AltIntrfDesc;
                    break;
                }
            }
            if (AltIntrfDesc->DescLength) {
                DescLength += (UINT16)AltIntrfDesc->DescLength;
            } else {
                break;
            }
        }
    }

    Desc+=((CNFG_DESC*)Desc)->TotalLength; // Calculate the end of descriptor block
    EndpDesc = (ENDP_DESC*)((char*)IntrfDesc + IntrfDesc->DescLength);

    for( ;(EndpDesc->DescType != DESC_TYPE_INTERFACE) && ((UINT8*)EndpDesc < Desc);
        EndpDesc = (ENDP_DESC*)((UINT8 *)EndpDesc + EndpDesc->DescLength)){

        if(!(EndpDesc->DescLength)) {
            break;  // Br if 0 length desc (should never happen, but...)
        }

        if( EndpDesc->DescType != DESC_TYPE_ENDPOINT ) {
            continue;
        }

        //
        // Check for and configure Interrupt endpoint if present
        //
        if ((EndpDesc->EndpointFlags & EP_DESC_FLAG_TYPE_BITS) !=
                EP_DESC_FLAG_TYPE_INT) {    // Bit 1-0: 10 = Endpoint does interrupt transfers
          continue;
        }

       if (EndpDesc->EndpointAddr & EP_DESC_ADDR_DIR_BIT) {
         DevInfo->IntInEndpoint = EndpDesc->EndpointAddr;
         DevInfo->IntInMaxPkt = EndpDesc->MaxPacketSize;
         DevInfo->PollInterval = EndpDesc->PollInterval;
         break;
       }
    }

    if ((HcStruc->HcFlag & HC_STATE_SPECIFIC_CONTROLLER) &&
        (DevInfo->HubDeviceNumber & BIT7)) {
        DevInfo->IncompatFlags |= USB_INCMPT_SPECIFIC_HUB_DEVICE;   
    }

    DevInfo->HubDepth = 0;
    ParentHub = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR, 
                                       NULL, DevInfo->HubDeviceNumber, NULL);
    if (ParentHub) {
        DevInfo->HubDepth = ParentHub->HubDepth + 1;
    }
  
    if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
        UsbHubSetHubDepth(HcStruc, DevInfo, DevInfo->HubDepth);
    }

    //
    // Allocate memory for getting hub descriptor
    //
    Buffer    = USB_MemAlloc(sizeof(MEM_BLK));
    if (!Buffer) {
      return NULL;
    }

    Status = UsbHubGetHubDescriptor(HcStruc, DevInfo, Buffer, sizeof(MEM_BLK));
    if (Status != USB_SUCCESS) {    // Error
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the hub descriptor\n");
        USB_MemFree(Buffer, sizeof(MEM_BLK));
        return NULL;
    }
    
    HubDesc = (HUB_DESC*)Buffer;
    DevInfo->HubNumPorts     = HubDesc->NumPorts;
    DevInfo->HubPowerOnDelay = HubDesc->PowerOnDelay; // Hub's ports have not been enumerated

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Usb Hub:: number of downstream port: %d, PowerOnDelay %d\n", 
              HubDesc->NumPorts, HubDesc->PowerOnDelay);

    if (DevInfo->Flag & DEV_INFO_ALT_SETTING_IF) {
        if (DevInfo->AltSettingNum != 0) {
            // Select this alternate setting for multiple TTs.
            UsbSetInterface(HcStruc, DevInfo, DevInfo->AltSettingNum);
        }
    }

    //
    // Turn on power to all of the hub's ports by setting its port power features.
    // This is needed because hubs cannot detect a device attach until port power
    // is turned on.
    //
    for (PortNum = 1; PortNum <= DevInfo->HubNumPorts; PortNum++) {

        if (DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE &&
            PortNum == HcStruc->DebugPort) {
            continue;
        }

        if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
           (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
           UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
           if (Usb3HubPortSts->PortStatus.Power == 1) {
               continue;
           }
        }

        UsbHubSetPortFeature(HcStruc, DevInfo, PortNum, PortPower);
        SetPortPower = TRUE;
    }

    //
    // Delay the amount of time specified in the PowerOnDelay field of
    // the hub descriptor: in ms, add 30 ms to the normal time and multiply
    // by 64 (in 15us).
    //
    if(SetPortPower) {
        if (!(DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE)) {
          if (gUsbData->PowerGoodDeviceDelay == 0) {
              DelayValue = (UINTN)DevInfo->HubPowerOnDelay * 2 * 1000;  // "Auto"
          } else {
              DelayValue = (UINTN)gUsbData->PowerGoodDeviceDelay * 1000* 1000;  // convert sec->15 mcs units
          }
          FixedDelay(DelayValue);
        }
    }

    DevInfo->PollTdPtr  = 0;
    DevInfo->PollEdPtr  = 0;

    //
    // Free the allocated buffer
    //
    USB_MemFree(Buffer, sizeof(MEM_BLK));

    DevInfo->HubPortConnectMap = 0;

    //
    // Check for new devices behind the hub
    //
    for (PortNum = 1; PortNum <= DevInfo->HubNumPorts; PortNum++) {
        USBCheckPortChange(HcStruc, DevInfo->DeviceAddress, PortNum);
    }

    DevInfo->PollingLength = DevInfo->IntInMaxPkt;
    AmiUsbActivatePolling(HcStruc, DevInfo);

    return DevInfo;
}


/**
    This routine disconnects the hub by disconnecting all the
    devices behind it

    @param DevInfo    Device info structure pointer
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
USBHUBDisconnectDevice(
    DEV_INFO*   DevInfo
)
{
    HC_STRUC* HcStruc = gHcTable[DevInfo->HcNumber - 1];
    UINT8 Port;

    // Stop polling the endpoint
    AmiUsbDeactivatePolling(HcStruc,DevInfo);
    DevInfo->IntInEndpoint = 0;

    //
    // A hub device is being disconnected.  For each of the hub's ports disconnect
    // any child device connected.
    //
    HcStruc = gHcTable[DevInfo->HcNumber - 1];

    for (Port = 1; Port <= (UINT8)DevInfo->HubNumPorts; Port++) {

        if (DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE &&
            Port == HcStruc->DebugPort) {
            continue;
        }

        USB_StopDevice (HcStruc,  DevInfo->DeviceAddress, Port);
    }

    return USB_SUCCESS;

}

/**
    Initial Usb hub device

    @param   None
    
    @retval  Nones

**/
VOID
EFIAPI
UsbHubDeviceInit(
    VOID
)
{
    USB_InstallCallBackFunction(USBHub_ProcessHubData);
    return;
}

/**
    This routine checks for hub type device from the
    interface data provided
    
    @param DevInfo    Pointer to device info structure
    @param BaseClass  USB base class code
    @param SubClass   USB sub-class code
    @param Protocol   USB protocol code

    @retval BIOS_DEV_TYPE_HUB type   on success 
    @retval USB_ERROR                on error

**/

UINT8
EFIAPI
USBHubCheckDeviceType(
    DEV_INFO    *DevInfo,
    UINT8       BaseClass,
    UINT8       SubClass,
    UINT8       Protocol
)
{
    if (BaseClass == BASE_CLASS_HUB) {
        return BIOS_DEV_TYPE_HUB;
    } else {
        return USB_ERROR;
    }
}


/**
    This routine returns the hub port status
    
    @param HcStruc          Pointer to HcStruc data
    @param HubAddr          Hub address
                             bit 7   : 1 - Root hub, 0 for other hubs
                             bit 6-0 : Device address of the hub
    @param PortNum          Port number                         
    @param ClearChangeBits  True: Clear all port status change
     
    @retval PortSts         Port status flags (Refer USB_PORT_STAT_XX equates)

**/

UINT32
USBHub_GetPortStatus(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum,
    BOOLEAN     ClearChangeBits
)
{
    UINT32      PortSts = USB_PORT_STAT_DEV_OWNER;
    UINT8       Status;
    DEV_INFO    *DevInfo;
    HUB_FEATURE  Feature;
    UINT16      PortChange;
    UINT8       Index;

    HUB_PORT_STATUS*    HubPortSts = (HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;
    USB3_HUB_PORT_STATUS*  Usb3HubPortSts = (USB3_HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;  

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR, NULL, HubAddr, HcStruc);
    ASSERT(DevInfo);
    if (DevInfo == NULL) {
        return 0;
    }

    if (DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE &&
        PortNum == HcStruc->DebugPort){
        return 0;
    }

    Status = UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
    if (Status == USB_ERROR) {
        return USB_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);

    if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
        for (Index = 0; Index < 20; Index++) {
            if (Usb3HubPortSts->PortStatus.Reset == 0) {
                break;
            }
            FixedDelay(10 * 1000);     // 10ms
            UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
        }
        
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);

        switch (Usb3HubPortSts->PortStatus.LinkState) {
            case USB3_HUB_PORT_LINK_U0:
            case USB3_HUB_PORT_LINK_RXDETECT:
                break;
            case USB3_HUB_PORT_LINK_U3:
                PortSts |= USB_PORT_STAT_DEV_SUSPEND;
                break;
            case USB3_HUB_PORT_LINK_RECOVERY:
                for (Index = 0; Index < 20; Index++) {
                    FixedDelay(10 * 1000);
                    UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
                    if (Usb3HubPortSts->PortStatus.LinkState != USB3_HUB_PORT_LINK_RECOVERY) {
                        break;
                    }
                }
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);
                break;
            case USB3_HUB_PORT_LINK_POLLING:
                for (Index = 0; Index < 50; Index++) {
                    FixedDelay(10 * 1000);
                    UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
                    if (Usb3HubPortSts->PortStatus.LinkState != USB3_HUB_PORT_LINK_POLLING) {
                        break;
                    }
                }
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);
                if (Usb3HubPortSts->PortStatus.LinkState == USB3_HUB_PORT_LINK_U0 || 
                    Usb3HubPortSts->PortStatus.LinkState == USB3_HUB_PORT_LINK_RXDETECT) {
                    break;
                }
            case USB3_HUB_PORT_LINK_INACTIVE:
                // A downstream port can only exit from this state when directed, 
                // or upon detection of an absence of a far-end receiver termination 
                // or upon a Warm Reset.
                // The Timeout of SS.Inactive.Quiet is 12 ms.
                FixedDelay(12 * 1000);
                UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);
                if (Usb3HubPortSts->PortStatus.LinkState == USB3_HUB_PORT_LINK_RXDETECT) {
                    break;
                }
            case USB3_HUB_PORT_LINK_COMPLIANCE_MODE:
                UsbHubResetPort(HcStruc, DevInfo, PortNum, TRUE);
                UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Hub port[%d] status: %08x\n", PortNum, *gUsbDataList->HubPortStatusBuffer);
                break;
            default:
                PortSts |= USB_PORT_STAT_DEV_CONNECTED;
                break;
        }
        if (Usb3HubPortSts->PortChange.ConnectChange) {
            PortSts |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
        }
        if (Usb3HubPortSts->PortStatus.Connected) {
            if (!(DevInfo->HubPortConnectMap & (UINT16) (1 << PortNum))) {
                PortSts |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
                DevInfo->HubPortConnectMap |= (UINT16) (1 << PortNum);
            }
            PortSts |= USB_PORT_STAT_DEV_CONNECTED | USB_PORT_STAT_DEV_SUPERSPEED;

            // USB 3.0 hub may not set Connect Status Change bit,
            // set the connect change flag if the BH Reset change or Reset change is set.
            if (Usb3HubPortSts->PortChange.BhResetChange || Usb3HubPortSts->PortChange.ResetChange) {
                PortSts |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
                PortSts |= USB_PORT_STAT_DEV_RESET_CHANGED;
            }
            if (Usb3HubPortSts->PortStatus.Enabled) {
                PortSts |= USB_PORT_STAT_DEV_ENABLED;
            }
        } else {
            DevInfo->HubPortConnectMap &= (UINT16) (~(1 << PortNum));
        }
        if (Usb3HubPortSts->PortChange.OverCurrentChange) {
            PortSts |= USB_PORT_STAT_DEV_OVERCURRENT_CHANGED;
            if ((Usb3HubPortSts->PortStatus.OverCurrent == 0) && 
                (Usb3HubPortSts->PortStatus.Power == 0)) {
                UsbHubSetPortFeature(HcStruc, DevInfo, PortNum, PortPower);
                FixedDelay((UINTN)(DevInfo->HubPowerOnDelay * 2 * 1000));
            }
        }
        if (Usb3HubPortSts->PortStatus.OverCurrent) {
            PortSts |= USB_PORT_STAT_DEV_OVERCURRENT;
        }
        if (Usb3HubPortSts->PortStatus.Reset) {
            PortSts |= USB_PORT_STAT_DEV_RESET;
        }
        if (Usb3HubPortSts->PortStatus.Power) {
            PortSts |= USB_PORT_STAT_DEV_POWER;
        }
    } else {
        if (HubPortSts->PortChange.ConnectChange) {
          PortSts |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
        }
        if (HubPortSts->PortStatus.Connected) {
            if (!(DevInfo->HubPortConnectMap & (UINT16) (1 << PortNum))) {
                PortSts |= USB_PORT_STAT_DEV_CONNECT_CHANGED;
                DevInfo->HubPortConnectMap |= (UINT16) (1 << PortNum);
            }
            PortSts |= USB_PORT_STAT_DEV_CONNECTED;
            if (HubPortSts->PortStatus.LowSpeed) {
                PortSts |= USB_PORT_STAT_DEV_LOWSPEED;
            } else if (HubPortSts->PortStatus.HighSpeed) {
                PortSts |= USB_PORT_STAT_DEV_HISPEED;
            } else {
                PortSts |= USB_PORT_STAT_DEV_FULLSPEED;
            }
            if (HubPortSts->PortStatus.Enabled) {
                PortSts |= USB_PORT_STAT_DEV_ENABLED;
            }
        } else {
            DevInfo->HubPortConnectMap &= (UINT16) (~(1 << PortNum));
        }
        if (HubPortSts->PortStatus.Suspend) {
            PortSts |= USB_PORT_STAT_DEV_SUSPEND;
        }
        if (HubPortSts->PortStatus.OverCurrent) {
            PortSts |= USB_PORT_STAT_DEV_OVERCURRENT;
        }
        if (HubPortSts->PortStatus.Reset) {
            PortSts |= USB_PORT_STAT_DEV_RESET;
        }
        if (HubPortSts->PortStatus.Power) {
            PortSts |= USB_PORT_STAT_DEV_POWER;
        }
        if (HubPortSts->PortChange.OverCurrentChange) {
            if ((HubPortSts->PortStatus.OverCurrent == 0) && 
                (HubPortSts->PortStatus.Power == 0)) {
                UsbHubSetPortFeature(HcStruc, DevInfo, PortNum, PortPower);
                FixedDelay((UINTN)(DevInfo->HubPowerOnDelay * 2 * 1000));
            }
        }
        if (HubPortSts->PortChange.EnableChange) {
            PortSts |= USB_PORT_STAT_DEV_ENABLE_CHANGED;
        }
        if (HubPortSts->PortChange.SuspendChange) {
            PortSts |= USB_PORT_STAT_DEV_SUSPEND_CHANGED;
        }
        if (HubPortSts->PortChange.ResetChange) {
            PortSts |= USB_PORT_STAT_DEV_RESET_CHANGED;
        }
    }

    // Clear all port status change
    //UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);
    if (ClearChangeBits == TRUE) {
      PortChange = (*((UINT16*)&HubPortSts->PortChange));
      for (Feature = PortConnectChange; Feature <= PortResetChange; Feature++) {
          if (PortChange & 1) {
              UsbHubClearPortFeature(HcStruc, DevInfo, PortNum, Feature);
          }
          PortChange >>= 1;
      }

      if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
            (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
          if (Usb3HubPortSts->PortChange.LinkStateChange) {
              UsbHubClearPortFeature(HcStruc, DevInfo, PortNum, PortLinkStateChange);
          }
          if (Usb3HubPortSts->PortChange.ConfigErrorChange) {
              UsbHubClearPortFeature(HcStruc, DevInfo, PortNum, PortConfigErrorChange);
          }
          if (Usb3HubPortSts->PortChange.BhResetChange) {
              UsbHubClearPortFeature(HcStruc, DevInfo, PortNum, BhPortResetChange);
          }
      }
    }

    return PortSts;
}


/**
    This routine disables the hub port
    @param HcStruc           HCStruc of the host controllerr
    @param HubAddr           USB device address of the hub whose status has changed
                              bit 7   : 1 - Root hub, 0 for other hubs
                              bit 6-0 : Device address of the hub
    @param PortNum           Port number

    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/

UINT8
USBHub_DisablePort(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum
)
{
    DEV_INFO*   DevInfo;

    HUB_PORT_STATUS*  HubPortSts = (HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;
  
    //
    // Get DeviceInfo pointer
    //
    DevInfo   = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR,
                            (DEV_INFO*)0,
                            HubAddr,
                            HcStruc);
//
// Disable the hub/port by clearing its Enable feature
//
    if (DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE &&
        PortNum == HcStruc->DebugPort) {
        return USB_SUCCESS;
    }

   if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
       (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
       return USB_SUCCESS;
    }

    UsbHubGetPortStatus(HcStruc, DevInfo, PortNum, gUsbDataList->HubPortStatusBuffer);

    // Perform control transfer with device request as HUB_RQ_CLEAR_PORT_FEATURE,
    // wIndex = Port number, wValue = HUB_FEATURE_PORT_ENABLE,
    // FpBuffer = 0 and wlength = 0
    //
    if(HubPortSts->PortStatus.Enabled) {
      UsbHubClearPortFeature(HcStruc, DevInfo, PortNum, PortEnable);
    }

    return USB_SUCCESS;

}


/**
    This routine enables the hub port

    @param HcStruc           HCStruc of the host controllerr
    @param HubAddr           USB device address of the hub whose status has changed
                              bit 7   : 1 - Root hub, 0 for other hubs
                              bit 6-0 : Device address of the hub
    @param PortNum           Port number

    @retval USB_SUCCESS      On success

**/

UINT8
USBHub_EnablePort(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum
)
{
    return USB_SUCCESS;
}

/**
    This routine resets the hub port
    @param HcStruc           HCStruc of the host controllerr
    @param HubAddr           USB device address of the hub whose status has changed
                              bit 7   : 1 - Root hub, 0 for other hubs
                              bit 6-0 : Device address of the hub
    @param PortNum           Port number

    @retval USB_SUCCESS      if the hub port is enabled. 
    @retval USB_ERROR        Error

**/

UINT8
USBHub_ResetPort(
    HC_STRUC*   HcStruc,
    UINT8       HubAddr,
    UINT8       PortNum)
{
    UINT8    Status;
    DEV_INFO*   DevInfo;

    //
    // Get DeviceInfo pointer
    //
    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_ADDR, 
                (DEV_INFO*)0, HubAddr, HcStruc);
    if (DevInfo == NULL) return USB_ERROR;

    if ((DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE) && 
    (PortNum == HcStruc->DebugPort)) {
        return USB_SUCCESS;
    }
    Status = UsbHubResetPort(HcStruc, DevInfo, PortNum, FALSE);

    return Status;
}


/**
    This routine is called with USB hub status change
    report data

    @param HcStruc    Pointer to HCStruc
    @param DevInfo    Pointer to device information structure
    @param Td         Pointer to the polling TD
    @param Buffer     Pointer to the data buffer
    @param DataLength Data length
    
    @retval USB_SUCCESS      On Success
    @retval USB_ERROR        Error
    
    
    @note  The status change data is an array of bit flags:
          Bit     Description
      ----------------------------------------------------------
          0   Indicate connect change status for all ports
          1   Indicate connect change status for port 1
          2   Indicate connect change status for port 2
          ...     ..............
          n   Indicate connect change status for port n
      -----------------------------------------------------------

**/

UINT8
EFIAPI
USBHub_ProcessHubData (
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Td,
    UINT8       *Buffer,
    UINT16      DataLength
 )
{
    UINT8       PortNum;
    UINT16      PortMap;
    BOOLEAN     ConnectDelay = FALSE;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBHub_ProcessHubData, gUsbData->EnumFlag = %d\n", gUsbData->EnumFlag);
    //
    // Check for enum flag and avoid hub port enumeration if needed
    //
    if (gUsbData->EnumFlag == TRUE) return USB_SUCCESS;
    
    for (PortNum = 1; PortNum <= DevInfo->HubNumPorts; PortNum++) {
        PortMap = (UINT16)(1 << PortNum);
        if (*(UINT16*)Buffer & PortMap) {
            if (!ConnectDelay && ((~DevInfo->HubPortConnectMap) & PortMap)) {
                // The interval with a minimum duration of 100 ms when 
                // the USB System Software is notified of a connection detection.
                FixedDelay(USB_PORT_CONNECT_STABLE_DELAY_MS * 1000);
                ConnectDelay = TRUE;
            }
            //
            // Set enumeration flag so that another device will not get enabled
            //
            gUsbData->EnumFlag = TRUE;

            USBCheckPortChange(HcStruc, DevInfo->DeviceAddress, PortNum);

            //
            // Reset enumeration flag so that other devices can be enumerated
            //
            gUsbData->EnumFlag = FALSE;
        }
    }
   return USB_SUCCESS;
}


/**
    Rest the specific port

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Port             Port number
    @param WarmReset        TRUE : do warm reset

    @retval USB_SUCCESS     On Success
    @retval USB_ERROR       Error

**/
UINT8
UsbHubResetPort(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       Port,
    BOOLEAN     WarmReset
)
{
    UINT8       Status;
    UINT32      Index;
    BOOLEAN     IsResetChange;
    HUB_PORT_STATUS        *HubPortSts = (HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;
    USB3_HUB_PORT_STATUS   *Usb3HubPortSts = (USB3_HUB_PORT_STATUS*)gUsbDataList->HubPortStatusBuffer;

    if (WarmReset && ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS))) {
        Status = UsbHubSetPortFeature(HcStruc, DevInfo, Port, BhPortReset);
        if (Status != USB_SUCCESS) {
            return USB_ERROR;
        }
    
        for (Index = 0; Index < (USB_HUB_WARM_RESET_PORT_TIMEOUT_MS / 10); Index++) {
            FixedDelay(10 * 1000);
            Status = UsbHubGetPortStatus(HcStruc, DevInfo, Port, gUsbDataList->HubPortStatusBuffer);
            if (Status != USB_SUCCESS) {
                return USB_ERROR;
            }
            if (Usb3HubPortSts->PortChange.BhResetChange) {
                break;
            }
        }
        
        if (!Usb3HubPortSts->PortChange.BhResetChange) {
            USB_DEBUG(DEBUG_ERROR, 3, "Usb super speed hub port[%d] status: %08x: warm reset port is failed\n",
                      Port, *gUsbDataList->HubPortStatusBuffer);
          return USB_ERROR;
        }
        
        Status = UsbHubClearPortFeature(HcStruc, DevInfo, Port, BhPortResetChange);
        if (Status != USB_SUCCESS) {
            return USB_ERROR;
        }
    
        Status = UsbHubClearPortFeature(HcStruc, DevInfo, Port, PortResetChange);
        if (Status != USB_SUCCESS) {
            return USB_ERROR;
        }

    } else {
        Status = UsbHubSetPortFeature(HcStruc, DevInfo, Port, PortReset);
        if(Status != USB_SUCCESS) return USB_ERROR;

        // The duration of the Resetting state is nominally 10 ms to 20 ms
        FixedDelay(20 * 1000);      // 20 ms delay

        for (Index = 0; Index < 10; Index++) {
            Status = UsbHubGetPortStatus(HcStruc, DevInfo, Port, gUsbDataList->HubPortStatusBuffer);
            if(Status != USB_SUCCESS) return USB_ERROR;

            if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
                (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
                IsResetChange = Usb3HubPortSts->PortChange.ResetChange ? TRUE : FALSE;
            } else {
                IsResetChange = HubPortSts->PortChange.ResetChange ? TRUE : FALSE;
            }

            if(IsResetChange) break;

            FixedDelay(5 * 1000);      // 5 ms delay
        }
        if (!IsResetChange) {
            return USB_ERROR;
        }

        Status = UsbHubClearPortFeature(HcStruc, DevInfo, Port, PortResetChange);
        if(Status != USB_SUCCESS) return USB_ERROR;

        if ((DevInfo->EndpointSpeed != USB_DEV_SPEED_SUPER) &&
            (DevInfo->EndpointSpeed != USB_DEV_SPEED_SUPER_PLUS)) {
            if (!(DevInfo->IncompatFlags & USB_INCMPT_SPECIFIC_HUB_DEVICE)) {
                FixedDelay(20 * 1000);     // 20 ms
            } else if (HubPortSts->PortStatus.LowSpeed == 1) {
                // 1 ms delay for Low-Speed device
                FixedDelay(1 * 1000);
            }
        }
    }
    return USB_SUCCESS;
}

/**
    Clear Hub feature

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param HubFeature       HubFeature data structure

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubCearHubFeature(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    HUB_FEATURE  HubFeature)
{
  return USB_ERROR;
}

/**
    Clear Hub port feature

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Port             Port number   
    @param PortFeature      HubFeature data structure

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubClearPortFeature(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       Port,
    HUB_FEATURE  PortFeature)
{
    DEV_REQ  DevReq;

    DevReq.RequestType = HUB_RQ_CLEAR_PORT_FEATURE;
    DevReq.Value = PortFeature;
    DevReq.Index = Port;
    DevReq.DataLength = 0;

    return UsbControlTransfer(HcStruc, DevInfo, DevReq, 50, NULL);
}

/**
    Get Hub descriptor

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Buffer           Descriptor data buffer
    @param Length           request Descriptor length

    @retval USB_SUCCESS     On success
    @retval USB_ERROR       Failure

**/
UINT8
UsbHubGetHubDescriptor(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    VOID*       Buffer,
    UINT16      Length)
{
    DEV_REQ  DevReq;

    DevReq.RequestType = USB_RQ_GET_CLASS_DESCRIPTOR;
    DevReq.Index = 0;
    DevReq.DataLength = Length;

    if ((DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER) ||
        (DevInfo->EndpointSpeed == USB_DEV_SPEED_SUPER_PLUS)) {
        DevReq.Value = DESC_TYPE_SS_HUB << 8;
    } else {
        DevReq.Value = DESC_TYPE_HUB << 8;
    }

    return UsbControlTransfer(HcStruc, DevInfo, DevReq, 150, Buffer);
}

/**
    Get Hub status

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param HubStatus        Hub status

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubGetHubStatus(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT32*     HubStatus)
{
    return USB_ERROR;
}

/**
    Get Hub port status

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Port             Port number
    @param PortStatus       Hub port status

    @retval USB_SUCCESS     On success
    @retval USB_ERROR       Failure

**/
UINT8
UsbHubGetPortStatus(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       Port,
    UINT32*     PortStatus)
{
    DEV_REQ    DevReq;

    if (PortStatus == NULL) {
        return USB_ERROR;
    }
    *PortStatus = 0;
    DevReq.RequestType = HUB_RQ_GET_PORT_STATUS;
    DevReq.Value = 0;
    DevReq.Index = Port;
    DevReq.DataLength = 4;

    return UsbControlTransfer(HcStruc, DevInfo, DevReq, 150, PortStatus);
}

/**
    Get Hub error count

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Port             Port number
    @param ErrorCount       Error count

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubGetErrorCount(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       Port,
    UINT16*     ErrorCount)
{
    return USB_ERROR;
}

/**
    Set hub descriptor

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Buffer           Data buffer
    @param Length           Data length

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubSetHubDescriptor(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    VOID*       Buffer,
    UINT16      Length)
{
    return USB_ERROR;
}

/**
    Set hub feature

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param HubFeature       Hub feature structure

    @retval USB_ERROR       Unsupported

**/
UINT8
UsbHubSetHubFeature(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    HUB_FEATURE  HubFeature)
{
    return USB_ERROR;
}

/**
    Set Hub Depth

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param HubDepth         Hub depth

    @retval USB_SUCCESS     On success
    @retval USB_ERROR       Failure

**/
UINT8
UsbHubSetHubDepth(
    HC_STRUC*  HcStruc,
    DEV_INFO*  DevInfo,
    UINT16      HubDepth)
{
    DEV_REQ  DevReq;

    DevReq.RequestType = HUB_RQ_SET_HUB_DEPTH;
    DevReq.Value = HubDepth;
    DevReq.Index = 0;
    DevReq.DataLength = 0;

    return UsbControlTransfer(HcStruc, DevInfo, DevReq, 50, NULL);
}

/**
    Set Hub Depth

    @param HcStruc          Pointer to HCStruc
    @param DevInfo          Pointer to device information structure
    @param Port             Port number
    @param PortFeature      Port feature structure
    
    @retval USB_SUCCESS     On success
    @retval USB_ERROR       Failure

**/
UINT8
UsbHubSetPortFeature(
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8       Port,
    HUB_FEATURE  PortFeature)
{
    DEV_REQ  DevReq;

    DevReq.RequestType = HUB_RQ_SET_PORT_FEATURE;
    DevReq.Value = PortFeature;
    DevReq.Index = Port;
    DevReq.DataLength = 0;

    return UsbControlTransfer(HcStruc, DevInfo, DevReq, 100, NULL);
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
