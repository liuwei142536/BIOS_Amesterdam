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

/** @file UsbHid.c
    USB HID class device driver

**/


#include "AmiUsb.h"
#include "UsbKbd.h"
#include <UsbDevDriverElinks.h>
#include <Protocol/AmiUsbHid.h>
#include <Library/AmiUsbHcdLib.h>

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;

typedef BOOLEAN (USB_HID_TYPE_CHECK_FUNCTIONS)( 
    DEV_INFO            *DevInfo,
    HID_REPORT_FIELD    *Field
        );
extern USB_HID_TYPE_CHECK_FUNCTIONS USB_HID_TYPE_CHECK_ELINK_LIST EndOfCheckTypeList;
USB_HID_TYPE_CHECK_FUNCTIONS* HidTypeCheckFunctionsList[] = {
    USB_HID_TYPE_CHECK_ELINK_LIST NULL};

typedef BOOLEAN (USB_HID_BUFFER_CHECK_FUNCTIONS)( 
    DEV_INFO    *DevInfo,
    UINT8       *Buffer,
    UINT16      DataLength
        );
extern USB_HID_BUFFER_CHECK_FUNCTIONS USB_HID_BUFFER_CHECK_ELINK_LIST EndOfCheckBufferList;
USB_HID_BUFFER_CHECK_FUNCTIONS* HidBufferCheckFunctionsList[] = {
    USB_HID_BUFFER_CHECK_ELINK_LIST NULL};

UINT8   UsbControlTransfer(HC_STRUC*, DEV_INFO*, DEV_REQ, UINT16, VOID*);

VOID        EFIAPI USBHIDInitialize (VOID);
UINT8       EFIAPI USBHIDCheckForDevice (DEV_INFO*, UINT8, UINT8, UINT8);
DEV_INFO*   EFIAPI USBHIDConfigureDevice (HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16);
UINT8           EFIAPI USBHIDProcessData ( HC_STRUC*, DEV_INFO*, UINT8*, UINT8*, UINT16);
UINT8       EFIAPI USBHIDDisconnectDevice (DEV_INFO*);

UINT8           HidGetReportDescriptor(HC_STRUC*, DEV_INFO*, HID_DESC*);  

VOID        USBMSInitialize (VOID);
DEV_INFO*   USBMSConfigureDevice (HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16); 
DEV_INFO*   USBKBDConfigureDevice (DEV_INFO*); 
DEV_INFO*   USBAbsConfigureDevice (HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16);  
VOID        CheckInputMode(DEV_INFO *DevInfo, HID_REPORT_FIELD * Field); 


/**
    This function fills the HID device driver routine pointers

    @param DevDriver    Pointer to the device driver structure

    @retval None
**/
VOID
USBHIDFillDriverEntries(
    DEV_DRIVER *DevDriver
)
{
    DevDriver->DevType               = BIOS_DEV_TYPE_HID;
    DevDriver->BaseClass             = BASE_CLASS_HID;
    DevDriver->SubClass              = 0;
    DevDriver->Protocol              = 0;
    DevDriver->FnDeviceInit          = USBHIDInitialize;
    DevDriver->FnCheckDeviceType     = USBHIDCheckForDevice;
    DevDriver->FnConfigureDevice     = USBHIDConfigureDevice;
    DevDriver->FnDisconnectDevice    = USBHIDDisconnectDevice;
    return;
}

/**
    This function returns fills the host controller driver
    routine pointers in the structure provided

    @param VOID

    @retval VOID

**/

VOID
EFIAPI
USBHIDInitialize(
    VOID
)
{
    USBKBDInitialize();
    USBMSInitialize();
    USB_InstallCallBackFunction(USBHIDProcessData);
    return;
}

/**
    This routine checks for HID type device from the
    interface data provided
    
    @param DevInfo    Pointer to devInfo structure
    @param BaseClass  USB base class code
    @param SubClass   USB sub-class code
    @param Protocol   USB protocol code

    @retval BIOS_DEV_TYPE_HID  on success 
    @retval USB_ERROR0FFH      on error

**/
UINT8
EFIAPI
USBHIDCheckForDevice(
    DEV_INFO*   DevInfo,
    UINT8       BaseClass,
    UINT8       SubClass,
    UINT8       Protocol
)
{
    //
    // Check the BaseClass, SubClass and Protocol for a HID/Boot/Mouse device.
    //
    if (BaseClass != BASE_CLASS_HID) {
        return USB_ERROR;
    }

    if (((gUsbData->UsbFeature & USB_BOOT_PROTOCOL_SUPPORT) == USB_BOOT_PROTOCOL_SUPPORT) || 
        (DevInfo->IncompatFlags & USB_INCMPT_HID_BOOT_PROTOCOL_ONLY)) {
        if (SubClass != SUB_CLASS_BOOT_DEVICE) {
            return USB_ERROR;
        }
    
        if (Protocol != PROTOCOL_KEYBOARD &&
            Protocol != PROTOCOL_MOUSE) {
            return USB_ERROR;
        }
    }

    return BIOS_DEV_TYPE_HID;
}

/**
    This routine checks an interface descriptor of the USB device
    detected to see if it describes a HID/Boot/Keyboard device.
    If the device matches the above criteria, then the device is
    configured and initialized

    @param HcStruc    HCStruc pointer
    @param DevInfo    Device information structure pointer
    @param Desc       Pointer to the descriptor structure
    @param Start      Offset within interface descriptor
                      supported by the device
    @param End        End offset of the device descriptor

    @retval FPDEV_INFO  New device info structure, 0 on error

**/

DEV_INFO*
EFIAPI
USBHIDConfigureDevice (
    HC_STRUC*   HcStruc,
    DEV_INFO*   DevInfo,
    UINT8*      Desc,
    UINT16      Start,
    UINT16      End
)
{
    ENDP_DESC       *EndpDesc;
    INTRF_DESC      *IntrfDesc;
    UINT8           *DescEnd;
    HID_DESC        *HidDesc = NULL;
    DEV_REQ         Request;  
    UINT8           Status;
    
    
    SetMem(&Request, sizeof(DEV_REQ), 0);
    
    DevInfo->DeviceType = (UINT8)BIOS_DEV_TYPE_HID;
    DevInfo->HidDevType = 0;
    DevInfo->IntInEndpoint = 0;
    DevInfo->IntOutEndpoint = 0;

    ZeroMem(&DevInfo->HidReport, sizeof(DevInfo->HidReport));

    USB_DEBUG(DEBUG_INFO, 3, "USBHIDConfigureDevice...  \n");

    DevInfo->CallBackIndex = USB_InstallCallBackFunction(USBHIDProcessData);

    IntrfDesc = (INTRF_DESC*)(Desc + Start);
    DescEnd = Desc + ((CNFG_DESC*)Desc)->TotalLength; // Calculate the end of descriptor block
    EndpDesc = (ENDP_DESC*)((char*)IntrfDesc + IntrfDesc->DescLength);

    //Select correct endpoint
    for (;(EndpDesc->DescType != DESC_TYPE_INTERFACE) && ((UINT8*)EndpDesc < DescEnd);
        EndpDesc = (ENDP_DESC*)((UINT8 *)EndpDesc + EndpDesc->DescLength)){
        if (!(EndpDesc->DescLength)) {
            break;  // Br if 0 length desc (should never happen, but...)
        }

        if (EndpDesc->DescType == DESC_TYPE_HID ) {
            HidDesc = (HID_DESC*)EndpDesc;
             continue;
        }

        if (EndpDesc->DescType != DESC_TYPE_ENDPOINT ) {
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
            if (DevInfo->IntInEndpoint == 0) {
                DevInfo->IntInEndpoint = EndpDesc->EndpointAddr;
                DevInfo->IntInMaxPkt = EndpDesc->MaxPacketSize;
                DevInfo->PollInterval = EndpDesc->PollInterval;
                USB_DEBUG(DEBUG_INFO, 3, "interrupt in endpoint addr: %x, max packet size: %x\n", 
                          DevInfo->IntInEndpoint, DevInfo->IntInMaxPkt);
            }
        } else {
            if (DevInfo->IntOutEndpoint == 0) {
                DevInfo->IntOutEndpoint = EndpDesc->EndpointAddr;
                DevInfo->IntOutMaxPkt = EndpDesc->MaxPacketSize;
                USB_DEBUG(DEBUG_INFO, 3, "interrupt out endpoint addr: %x, max packet size: %x\n", 
                          DevInfo->IntOutEndpoint, DevInfo->IntOutMaxPkt);
            }
        }
    }

    if (DevInfo->IntInEndpoint == 0) {
        return 0;
    }

        //Set protocol (Option)
    if ((((gUsbData->UsbFeature & USB_BOOT_PROTOCOL_SUPPORT) == USB_BOOT_PROTOCOL_SUPPORT) &&
        !(DevInfo->IncompatFlags & USB_INCMPT_SET_BOOT_PROTOCOL_NOT_SUPPORTED)) ||
        (DevInfo->IncompatFlags & USB_INCMPT_HID_BOOT_PROTOCOL_ONLY)) {
                //
                // Send the set protocol command, wValue = 0 (Boot protocol)
                //
        Request.RequestType = HID_RQ_SET_PROTOCOL;
        Request.Value = 0;              // 0: Boot Protocol
        Request.Index = DevInfo->InterfaceNum;
        Request.DataLength = 0;
                
        UsbControlTransfer(HcStruc, DevInfo, Request, 100, NULL);
        } 

    //Send Set_Idle command 
    Request.RequestType = HID_RQ_SET_IDLE;
    Request.Value = 0;
    Request.Index = DevInfo->InterfaceNum;
    Request.DataLength = 0;
        
    UsbControlTransfer(HcStruc, DevInfo, Request, 100, NULL);

    if (((gUsbData->UsbFeature & USB_BOOT_PROTOCOL_SUPPORT) == 0) && 
        !(DevInfo->IncompatFlags & USB_INCMPT_HID_BOOT_PROTOCOL_ONLY)) {
        Status = HidGetReportDescriptor(HcStruc, DevInfo, HidDesc);
        if (Status == USB_ERROR) {
            return 0;
        }
        DevInfo->HidReport.Flag |= HID_REPORT_FLAG_REPORT_PROTOCOL;
        if (DevInfo->PollingLength < DevInfo->IntInMaxPkt) {
            DevInfo->PollingLength = DevInfo->IntInMaxPkt;
        }
    } else {
        DevInfo->PollingLength = DevInfo->IntInMaxPkt;
        switch (DevInfo->Protocol) {
            case PROTOCOL_KEYBOARD:
                DevInfo->HidDevType = HID_DEV_TYPE_KEYBOARD;
                break;

            case PROTOCOL_MOUSE:
                DevInfo->HidDevType = HID_DEV_TYPE_MOUSE;
                break;

            default:
                break;
        }
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "UsbHidDevType: %x, PollingLength: %x, PollInterval: %x\n", 
        DevInfo->HidDevType, DevInfo->PollingLength, DevInfo->PollInterval);

    if (DevInfo->HidDevType & HID_DEV_TYPE_KEYBOARD) { 
        if (!(USBKBDConfigureDevice(DevInfo))) {
            return 0;
        }
    }

    // Microsoft Wired Keyboard 600 (two interface)
    // Sometimes the device won't send data, because the first interrupt 
    // data is too quick to send after we configure the second interface.
    // Workaround: we activates the polling with the first interface
    // when configuring the second interface
    if ((DevInfo->VendorId == MICROSOFT_VID) && (DevInfo->DeviceId == MICROSOFT_KB600_DID)) {
       DEV_INFO*   FirstDevInfo;
       UINTN       DeviceIndex;

       if (DevInfo->IntInEndpoint == (EP_DESC_ADDR_DIR_BIT | EP_DESC_ADDR_EP_NUM_1))  return DevInfo;

       for (DeviceIndex = 1; DeviceIndex < gUsbData->MaxDevCount; DeviceIndex++) {
           FirstDevInfo = &gUsbDataList->DevInfoTable[DeviceIndex];
           if ((FirstDevInfo->Flag & DEV_INFO_VALIDPRESENT) != DEV_INFO_VALIDPRESENT) {
               continue;
           }
           if (FirstDevInfo->DeviceAddress == DevInfo->DeviceAddress) {
               if (FirstDevInfo->IntInEndpoint == (EP_DESC_ADDR_DIR_BIT | EP_DESC_ADDR_EP_NUM_1)) {
                   if ((FirstDevInfo->HidDevType & (HID_DEV_TYPE_KEYBOARD | HID_DEV_TYPE_MOUSE | 
                      HID_DEV_TYPE_POINT | HID_DEV_TYPE_CONSUMER | HID_DEV_TYPE_OEM)) &&
                      (FirstDevInfo->PollingLength != 0) && (FirstDevInfo->PollInterval != 0)) {
                       if (!((FirstDevInfo->HidDevType & HID_DEV_TYPE_MOUSE) && (gUsbData->UsbStateFlag & USB_FLAG_EFIMS_DIRECT_ACCESS))) {
                           AmiUsbActivatePolling(HcStruc, FirstDevInfo);
                       }
                   }
                   break;
               }
           }
       }
    }

    //Active polling
    if ((DevInfo->HidDevType & (HID_DEV_TYPE_KEYBOARD | HID_DEV_TYPE_MOUSE |
        HID_DEV_TYPE_POINT | HID_DEV_TYPE_CONSUMER | HID_DEV_TYPE_OEM)) &&
        (DevInfo->PollingLength != 0) && (DevInfo->PollInterval != 0)) {
        if (!((DevInfo->HidDevType & HID_DEV_TYPE_MOUSE) && (gUsbData->UsbStateFlag & USB_FLAG_EFIMS_DIRECT_ACCESS))) {
            AmiUsbActivatePolling(HcStruc, DevInfo);
        }
    }

    return DevInfo;
} 

/**
    This routine disconnects the keyboard by freeing
    the USB keyboard device table entry

    @param DevInfo   Pointer to DeviceInfo structure
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure

**/
UINT8
EFIAPI
USBHIDDisconnectDevice (
    DEV_INFO    *DevInfo
)
{
    HC_STRUC  *HcStruc = gHcTable[DevInfo->HcNumber - 1];
    UINT16    Index;
    UINT8     *MemBlockEnd = (UINT8*)((UINTN)gUsbDataList->MemBlockStart + (gUsbData->MemPages << 12));

    // Stop polling the endpoint
    AmiUsbDeactivatePolling(HcStruc, DevInfo);
    DevInfo->IntInEndpoint = 0;

    if (DevInfo->HidDevType & HID_DEV_TYPE_KEYBOARD) {
        USBKBDDisconnectDevice(DevInfo);
    }

    if (DevInfo->HidReport.Fields != NULL) {
        for (Index = 0; Index < DevInfo->HidReport.FieldCount; Index++) {
            
            if (((UINTN)DevInfo->HidReport.Fields[Index] < (UINTN)gUsbDataList->MemBlockStart) ||
                (((UINTN)DevInfo->HidReport.Fields[Index] + sizeof(HID_REPORT_FIELD)) > (UINTN)MemBlockEnd)) {
                DevInfo->HidReport.Fields = NULL;
                DevInfo->HidReport.ReportDescLen = 0;
                return USB_SUCCESS;
            }
                                 
            if (DevInfo->HidReport.Fields[Index]->Usages != NULL) {
                if (((UINTN)DevInfo->HidReport.Fields[Index]->Usages < (UINTN)gUsbDataList->MemBlockStart) ||
                    (((UINTN)DevInfo->HidReport.Fields[Index]->Usages + sizeof(UINT16)) > (UINTN)MemBlockEnd)) {
                    DevInfo->HidReport.Fields = NULL;
                    DevInfo->HidReport.ReportDescLen = 0;
                    return USB_SUCCESS;
                }

                USB_MemFree(DevInfo->HidReport.Fields[Index]->Usages, 
                GET_MEM_BLK_COUNT(DevInfo->HidReport.Fields[Index]->UsageCount * sizeof(UINT16)));
                DevInfo->HidReport.Fields[Index]->Usages = NULL;
            }
            USB_MemFree(DevInfo->HidReport.Fields[Index], GET_MEM_BLK_COUNT(sizeof(HID_REPORT_FIELD)));
            DevInfo->HidReport.Fields[Index] = NULL;
        }

        USB_MemFree(DevInfo->HidReport.Fields, GET_MEM_BLK_COUNT(DevInfo->HidReport.FieldCount * sizeof(HID_REPORT_FIELD*)));
        DevInfo->HidReport.Fields = NULL;
        DevInfo->HidReport.ReportDescLen = 0;
    }

    return USB_SUCCESS;         
} 

/**
    This routine disconnects the keyboard by freeing
    the USB keyboard device table entry

    @param Report    Pointer to report data buffer
    @param Offset    The offset for report data
    @param Size      requested report size
    @retval Data     report data

**/
UINT32
ExtractInputReportData (
    UINT8   *Report,
    UINT32  Offset,
    UINT16  Size
)
{
    UINT32      *Start;
    UINT8       BitOffset;
    UINT32      Data;

    Start = (UINT32*)((UINTN)Report + (Offset >> 3));
    BitOffset = (UINT8)Offset & 0x7;
    Data = (*Start >> BitOffset) & ((0x1 << Size) - 1);

    return Data;
}

/**
    This funtion copy data of the item to buffer.

    @param  Item         Pointer to HID item structure
    @param  Buffer       The target buffer
    @param  BufferSize   Buffer size
    @retval None
**/
VOID
GetItemData (
    HID_ITEM        *Item,
    VOID            *Buffer,
    UINT32          BufferSize
)
{
    UINT32  Size = Item->bSize > BufferSize ? BufferSize : Item->bSize;
    
    ZeroMem(Buffer, BufferSize);
    CopyMem(Buffer, &Item->data, Size);
}
 
/**

    This funtion adds usage into usage table.
    @param  Field         Pointer to HID REPORT Field structure
    @param  Usage         The usage that need to be added
    @retval None

**/
VOID
AddUsage (
    HID_REPORT_FIELD    *Field,
    UINT16              Usage
)
{
    if (Field->UsageCount >= Field->MaxUsages) {
        return;
    }

    Field->Usages[Field->UsageCount++] = Usage;
}

/**
    Add input or output item.

    @param  Report        Report data
    @param  Field         Pointer to HID REPORT Field structure

    @retval None

**/
VOID
AddField (
    HID_REPORT          *Report,
    HID_REPORT_FIELD    *Field
)
{
    HID_REPORT_FIELD    *NewField;
    HID_REPORT_FIELD    **Fields;
    UINT16              Index;

    NewField = USB_MemAlloc(GET_MEM_BLK_COUNT(sizeof(HID_REPORT_FIELD)));
    ASSERT(NewField != NULL);
    if (NewField == NULL) {
        return;
    }

    if (Field->ReportId != 0) {
        Report->Flag |= HID_REPORT_FLAG_REPORT_ID;
    }

    CopyMem(NewField, Field, sizeof(HID_REPORT_FIELD));
    NewField->Usages = NULL;

    USB_DEBUG(DEBUG_INFO, 4, "============================================== \n");
    USB_DEBUG(DEBUG_INFO, 4, "Field index = %02x, \t", Report->FieldCount);
    USB_DEBUG(DEBUG_INFO, 4, "Flag = %02x\n", NewField->Flag);
    USB_DEBUG(DEBUG_INFO, 4, "UsagePage = %4x, \t", NewField->UsagePage);
    USB_DEBUG(DEBUG_INFO, 4, "ReportId = %02x\n", NewField->ReportId);
    USB_DEBUG(DEBUG_INFO, 4, "ReportCount = %02x, \t", NewField->ReportCount);
    USB_DEBUG(DEBUG_INFO, 4, "bReportSize = %02x\n", NewField->ReportSize);
    USB_DEBUG(DEBUG_INFO, 4, "LogicalMin = %4x, \t", NewField->LogicalMin);
    USB_DEBUG(DEBUG_INFO, 4, "LogicalMax = %4x\n", NewField->LogicalMax);
    USB_DEBUG(DEBUG_INFO, 4, "PhysicalMax = %4x, \t", NewField->PhysicalMax);
    USB_DEBUG(DEBUG_INFO, 4, "PhysicalMin = %4x\n", NewField->PhysicalMin);
    USB_DEBUG(DEBUG_INFO, 4, "UnitExponent = %2x, \t", NewField->UnitExponent);
    USB_DEBUG(DEBUG_INFO, 4, "UsageCount = %4x\n", NewField->UsageCount);

    if (NewField->UsageCount != 0) {
        NewField->Usages = USB_MemAlloc (GET_MEM_BLK_COUNT(NewField->UsageCount * sizeof(UINT16)));
        ASSERT(NewField->Usages != NULL);
        if (NewField->Usages == NULL) {
            return;
        }

        CopyMem(NewField->Usages, Field->Usages, NewField->UsageCount * sizeof(UINT16));

        USB_DEBUG(DEBUG_INFO, 4, "Usages:\n");
        for (Index = 0; Index < NewField->UsageCount; Index++) {
            if ((NewField->UsagePage == HID_UP_GENDESK) && (NewField->Usages[Index] == HID_GENDESK_X)) {
                if (NewField->Flag & HID_REPORT_FIELD_FLAG_RELATIVE) {
                    Report->Flag |= HID_REPORT_FLAG_RELATIVE_DATA;
                } else {
                    Report->Flag |= HID_REPORT_FLAG_ABSOLUTE_DATA;
                }
                Report->AbsMaxX = (UINT16)NewField->LogicalMax;
            }
            if ((NewField->UsagePage == HID_UP_GENDESK) && (NewField->Usages[Index] == HID_GENDESK_Y)) {
                Report->AbsMaxY= (UINT16)NewField->LogicalMax;
            }
            USB_DEBUG(DEBUG_INFO, 4, "%02X ", NewField->Usages[Index]);         
            if ((Index & 0xF) == 0xF) {
                USB_DEBUG(DEBUG_INFO, 4, "\n"); 
            }
        }
    }
    USB_DEBUG(DEBUG_INFO, 4, "\n============================================== \n");

    Fields = USB_MemAlloc(GET_MEM_BLK_COUNT((Report->FieldCount + 1) * sizeof(HID_REPORT_FIELD*)));
    ASSERT(Fields != NULL);
    if (Fields == NULL) {
        return;
    }

    if (Report->Fields != NULL) {
        CopyMem(Fields, Report->Fields, Report->FieldCount * sizeof(HID_REPORT_FIELD*));
        USB_MemFree(Report->Fields, GET_MEM_BLK_COUNT(Report->FieldCount * sizeof(HID_REPORT_FIELD*)));
    }

    Report->Fields = Fields;
    Report->Fields[Report->FieldCount++] = NewField;

    return;
}

/**
    Parsing Hid data
    @param  DevInfo       Pointer to DevInfo structure
    @param  Field         Pointer to HID REPORT Field structure
    @param  Item          Pointer to HID Item structure
    
    @retval Zero          Finish to parsing

**/
UINT8
HidParserMain(
    DEV_INFO            *DevInfo,
    HID_REPORT_FIELD    *Field,
    HID_ITEM            *Item
)
{
    UINT8   Data = 0;
    UINT16  OemHookIndex;

    switch (Item->bTag) {
        case HID_MAIN_ITEM_TAG_BEGIN_COLLECTION:
            GetItemData(Item, &Data, sizeof(Data));

            // Check if it is application collection
            if (Data == HID_COLLECTION_APPLICATION) {
                switch (Field->UsagePage) {                 // Generic Desktop
                    case HID_UP_GENDESK:
                        switch (Field->Usages[Field->UsageCount - 1]) {
                            case HID_GENDESK_POINTER:       // Pointer
                            case HID_GENDESK_MOUSE:         // Mouse
                                DevInfo->HidDevType |= HID_DEV_TYPE_MOUSE;
                                break;
                            case HID_GENDESK_KEYBOARD:      // Keyboard
                            case HID_GENDESK_KEYPAD:        // KeyPad
                                DevInfo->HidDevType |= HID_DEV_TYPE_KEYBOARD;
                                break;
                            default:
                                break;
                        }
                        break;
                    case HID_UP_CONSUMER:
                        DevInfo->HidDevType |= HID_DEV_TYPE_CONSUMER;
                        break;
                    case HID_UP_DIGITIZER:                  // Digitizer
                        if (Field->Usages[Field->UsageCount - 1] == HID_DIGITIZER_TOUCH_SCREEN) {
                            DevInfo->HidDevType |= HID_DEV_TYPE_POINT;
                        }
                        break;
                    case HID_UP_FIDO:
                        if (Field->Usages[Field->UsageCount - 1] == HID_FIDO_U2F_AD) {
                            USB_DEBUG(DEBUG_INFO, 3, "FIDO U2F is detected\n");
                            DevInfo->HidDevType |= HID_DEV_TYPE_U2F;
                        }
                        break;
                    default:
                        break;
                }
                // Call all the OEM hooks that wants to check hid type.
                for (OemHookIndex = 0; HidTypeCheckFunctionsList[OemHookIndex]; OemHookIndex++) {
                    if (HidTypeCheckFunctionsList[OemHookIndex](DevInfo, Field)) {
                        break;
                    }
                }
            }
            break;

        case HID_MAIN_ITEM_TAG_END_COLLECTION:
            break;

        case HID_MAIN_ITEM_TAG_INPUT:
            GetItemData(Item, &Field->Flag, sizeof(Field->Flag));
            Field->Flag = (Field->Flag & 7) | HID_REPORT_FIELD_FLAG_INPUT;
            
            // Microstep USB Keyboard (Sonix Technology Co chip) workaround
            // The report descriptor has an error, Modifier keys is bitmap data, but 
            // it reports as array data. We force variable flag for Modifier keys input item.
            if ((DevInfo->VendorId == 0x0C45) && 
                ((DevInfo->DeviceId == 0x7603) || (DevInfo->DeviceId == 0x7624))) {
                if ((Field->UsagePage == HID_UP_KEYBOARD) && 
                    (Field->UsageMin == HID_KEYBOARD_LEFT_CTRL) &&
                    (Field->UsageMax == HID_KEYBOARD_RIGHT_GUI)) {
                    Field->Flag |= HID_REPORT_FIELD_FLAG_VARIABLE;
                }
            }
            
            AddField(&DevInfo->HidReport, Field);
            break;

        case HID_MAIN_ITEM_TAG_OUTPUT:
            GetItemData(Item, &Field->Flag, sizeof(Field->Flag));
            Field->Flag &= 7;
            if ((Field->UsagePage == HID_UP_LED) && (Field->UsageCount > 0)) {
                AddField(&DevInfo->HidReport, Field);
            }
            break;

        case HID_MAIN_ITEM_TAG_FEATURE:
            CheckInputMode(DevInfo, Field);
            break;

        default:
            break;
        }

    //Clear Local Item
    ZeroMem(Field->Usages, Field->UsageCount * sizeof(UINT16));
    Field->UsageCount = 0;
    Field->UsageMin = 0;
    Field->UsageMax = 0;
    
    return 0;
}  

/**
    Parsing Hid report global item
    @param  DevInfo       Pointer to DevInfo structure
    @param  Field         Pointer to HID REPORT Field structure
    @param  Item          Pointer to HID Item structure
    
    @retval USB_SUCCESS          Finish to parsing

**/
UINT8
HidParserGlobal (
    DEV_INFO            *DevInfo,
    HID_REPORT_FIELD    *Field,
    HID_ITEM            *Item
)
{
    switch (Item->bTag) {
        case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
            GetItemData(Item, &Field->UsagePage, sizeof(Field->UsagePage));
            //Get Led usage page
            if (Field->UsagePage == HID_UP_LED) {
                DevInfo->HidReport.Flag |= HID_REPORT_FLAG_LED_FLAG;
            }
            break;

        case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
            GetItemData(Item, &Field->LogicalMin, sizeof(Field->LogicalMin));
            break;
                
        case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
            GetItemData(Item, &Field->LogicalMax, sizeof(Field->LogicalMax));
            break;

        case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
            GetItemData(Item, &Field->PhysicalMin, sizeof(Field->PhysicalMin));
            break;
                
        case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
            GetItemData(Item, &Field->PhysicalMax, sizeof(Field->PhysicalMax));
            break;

        case HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT:
            GetItemData(Item, &Field->UnitExponent, sizeof(Field->UnitExponent));
            break;

        case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
            GetItemData(Item, &Field->ReportSize, sizeof(Field->ReportSize));
            break;
                
        case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
            GetItemData(Item, &Field->ReportCount, sizeof(Field->ReportCount));
            break;
                
        case HID_GLOBAL_ITEM_TAG_REPORT_ID:
            GetItemData(Item, &Field->ReportId, sizeof(Field->ReportId));
            break;
                
        default:
            break;
        } 

    return USB_SUCCESS;
}

/**
    Parsing Hid report local item
    @param  DevInfo       Pointer to DevInfo structure
    @param  Field         Pointer to HID REPORT Field structure
    @param  Item          Pointer to HID Item structure
    
    @retval USB_SUCCESS          Finish to parsing

**/
UINT8
HidParserLocal (
    DEV_INFO                *DevInfo,
    HID_REPORT_FIELD        *Field,
    HID_ITEM                *Item
)
{
    UINT32  Data;

    GetItemData(Item, &Data, sizeof(Data));

    switch (Item->bTag) {
        case HID_LOCAL_ITEM_TAG_USAGE:
             AddUsage(Field, (UINT16)Data);
             break;
                
        case HID_LOCAL_ITEM_TAG_USAGE_MINIMUM:
             Field->UsageMin = (UINT16)Data;
             break;
                
        case HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM:
             Field->UsageMax = (UINT16)Data;
            
             // Medigenic-Esterline USB keboard (Advanced Input Devices chip) 
             // workaround. This device reports the wrong local minimum for 
             // keyboard data in its report descriptor, local minimum should be 0x00.
            
             if ((DevInfo->VendorId == 0x059d) && (DevInfo->DeviceId == 0x0708)) {
                 if ((Field->UsageMin == 0x01) && (Field->UsageMax == 0x65)) {
                     Field->UsageMin = 0x00;
                 }
             }

             for (Data = Field->UsageMin; Data <= Field->UsageMax; Data++) {
                 AddUsage(Field, (UINT16)Data);
             }
             break;
                
        default:        
             break;
    }

    return USB_SUCCESS;
}

/**
    Parsing Hid report reserved item
    @param  DevInfo       Pointer to DevInfo structure
    @param  Field         Pointer to HID REPORT Field structure
    @param  Item          Pointer to HID Item structure
    
    @retval USB_SUCCESS          Finish to parsing

**/
UINT8
HidParserReserved(
    DEV_INFO            *DevInfo,
    HID_REPORT_FIELD    *Field,
    HID_ITEM            *Item
)
{
    return USB_SUCCESS;
} 

/**
    Parsing Hid report descriptor
    @param  DevInfo       Pointer to DevInfo structure
    @param  ReportDesc    Pointer to report descriptor data
    
    @retval None

**/
VOID
HidParseReportDescriptor (
    DEV_INFO    *DevInfo,
    UINT8       *ReportDesc
)
{
    HID_REPORT          *Report = &DevInfo->HidReport;
    UINT8               *Start = ReportDesc;
    UINT8               *End = Start + Report->ReportDescLen;
    UINT16              Usages[0x300]; 
    HID_REPORT_FIELD    Field; 
    HID_ITEM            Item;
    UINT8               Data;
    UINT8               DataSize[] = {0, 1, 2, 4};

    static  UINT8 (*DispatchType[]) (DEV_INFO *DevInfo, 
                HID_REPORT_FIELD *Field, HID_ITEM *Item) = {
                     HidParserMain,
                     HidParserGlobal,
                     HidParserLocal,
                     HidParserReserved
                };

    SetMem(&Usages, sizeof(Usages), 0);
    SetMem(&Field, sizeof(Field), 0);
    SetMem(&Item, sizeof(Item), 0);

    Field.Usages = Usages;
    Field.MaxUsages = COUNTOF(Usages);
        
    while (Start < End) {
        Data = *Start++;

        Item.bType = (Data >> 2) & 0x3;
        Item.bTag = (Data >> 4) & 0xF;
        Item.bSize = DataSize[Data & 0x3];

        if ((Start + Item.bSize) > End) {
            break;
        }
        
        CopyMem(&Item.data.u32, Start, Item.bSize);
        Start += Item.bSize;
        DispatchType[Item.bType](DevInfo, &Field, &Item);
    }

    return;
}

/**

    This function calculates max data length to be reported 
    in the HID device.
    
    @param  DevInfo       Pointer to DevInfo structure
    
    @retval MaxLength     Max data length
**/
UINT16
CalculateInputReportDataLength (
    DEV_INFO    *DevInfo
)
{
    UINT8               Index = 0;
    HID_REPORT_FIELD    *Field = NULL;
    UINT16              ReportLen[256]; 
    UINT16              Length = 0;
    UINT16              MaxLength = 0;
    UINT16              ReportId = 0;

    SetMem(&ReportLen, sizeof(ReportLen), 0);
    
    for (Index = 0; Index < DevInfo->HidReport.FieldCount; Index++) {
        Field = DevInfo->HidReport.Fields[Index];
        if (!(Field->Flag & HID_REPORT_FIELD_FLAG_INPUT)) {
            continue;
        }

        ReportId = Field->ReportId;
        ReportLen[ReportId] += (UINT16)(Field->ReportCount * Field->ReportSize);
    }

    for (ReportId = 0; ReportId < COUNTOF(ReportLen); ReportId++) {
        if (ReportLen[ReportId] == 0) {
            continue;
        }

        Length = (ReportLen[ReportId] + 7) >> 3;
        if (ReportId != 0) {
            Length++;
        }

        MaxLength = MaxLength < Length ? Length : MaxLength;
    }

    return MaxLength;
}

/**

    Get report descriptor
    
    @param  HcStruc       Pointer to HcStruc structure   
    @param  DevInfo       Pointer to DevInfo structure
    @param  HidDesc       Pointer to Hid descriptor data
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure
**/
UINT8
HidGetReportDescriptor(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    HID_DESC    *HidDesc
)
{
    UINT8       *ReportDesc = NULL;
    UINT8       Index = 0;
    UINT8       Status = USB_ERROR;
    DEV_REQ     Request = {0};

    SetMem(&Request, sizeof(Request), 0);

    if (HidDesc == NULL) {
        return USB_ERROR;
    }

    if (HidDesc->DescriptorLength == 0) {
        return USB_SUCCESS;
    }

    ReportDesc = USB_MemAlloc(GET_MEM_BLK_COUNT(HidDesc->DescriptorLength));
    if (ReportDesc == NULL) {
        return USB_ERROR;
    }

    Request.RequestType = HID_RQ_GET_DESCRIPTOR;
    Request.Value = DESC_TYPE_REPORT << 8;
    Request.Index = DevInfo->InterfaceNum;
    Request.DataLength = HidDesc->DescriptorLength;

    for (Index = 0; Index < 3; Index++) {
        Status = UsbControlTransfer(HcStruc, DevInfo, Request, USB_GET_REPORT_DESC_TIMEOUT_MS, ReportDesc);
        if (Status == USB_SUCCESS) {
            break;
        }
    }
    if (Status == USB_SUCCESS) {
        DevInfo->HidReport.ReportDescLen = HidDesc->DescriptorLength ;          
        HidParseReportDescriptor(DevInfo, ReportDesc);
        DevInfo->PollingLength = CalculateInputReportDataLength(DevInfo);
    } else {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the report descriptor\n");
    }
    USB_MemFree(ReportDesc, GET_MEM_BLK_COUNT(HidDesc->DescriptorLength));
    return Status;
}

/**

    Process HID data
    
    @param HcStruc      Pointer to HCStruc
    @param DevInfo      Pointer to device information structure
    @param Td           Pointer to the polling TD
    @param Buffer       Pointer to the data buffer
    @param DataLength   Data length
    
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure
**/
UINT8
EFIAPI
USBHIDProcessData(
    HC_STRUC    *HcStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Td,
    UINT8       *Buffer,
    UINT16      DataLength
)
{
    UINT8               DataType = 0;
    UINT8               i;
    UINT16              j;
    HID_REPORT_FIELD    *Field = NULL;
    UINT16              OemHookIndex;

    // Call all the OEM hooks that wants to check hid buffer.
    for (OemHookIndex = 0; HidBufferCheckFunctionsList[OemHookIndex]; OemHookIndex++) {
        if (HidBufferCheckFunctionsList[OemHookIndex](DevInfo, Buffer, DataLength)) {
            return USB_SUCCESS;
        }
    }

    DataType = DevInfo->Protocol;

    if (DevInfo->HidReport.Flag & HID_REPORT_FLAG_REPORT_PROTOCOL) {
       for (i = 0; i < DevInfo->HidReport.FieldCount; i++) {
            Field = DevInfo->HidReport.Fields[i];

            //Check is input?
            if (!(Field->Flag & HID_REPORT_FIELD_FLAG_INPUT)) {
                  continue;
            }
            //if report id exist, check first byte
            if (Field->ReportId != 0 && Field->ReportId != Buffer[0]) {
                continue;
            }

            if (Field->UsagePage == HID_UP_KEYBOARD) {
                DataType = HID_BTYPE_KEYBOARD;
            }
            //Check X,Y
            if ((Field->UsagePage == HID_UP_GENDESK) && (Field->UsageCount != 0)) {
                for (j = 0; j < Field->UsageCount; j++) {
                   //find X
                     if (Field->Usages[j] == HID_GENDESK_X) {
                        if (Field->Flag & HID_REPORT_FIELD_FLAG_RELATIVE) {
                            DataType = HID_BTYPE_MOUSE;
                        } else {
                            DataType = HID_BTYPE_POINT;
                        }
                     }
                }    
            }
       }
    }

    switch(DataType) {
        case HID_BTYPE_KEYBOARD:
            USBKBDProcessKeyboardData(HcStruc, DevInfo, Td, Buffer, DataLength);
            break;
        case HID_BTYPE_MOUSE:
            USBMSProcessMouseData(HcStruc, DevInfo, Td, Buffer, DataLength);
            break;
        case HID_BTYPE_POINT:
            USBAbsProcessMouseData(HcStruc, DevInfo, Td, Buffer, DataLength);
            break;          
        default:
            break;
    }
    return USB_SUCCESS; 
}

/**

    Check input mode
    
    @param DevInfo      Pointer to device information structure
    @param Field        Pointer to the HID_REPORT_FIELD structure
    
    @retval None

**/
VOID
CheckInputMode(
        DEV_INFO                *DevInfo,
        HID_REPORT_FIELD        *Field
)
{
    HC_STRUC        *HcStruc = gHcTable[DevInfo->HcNumber - 1];
    DEV_REQ         Request;
    UINT8           *Buffer;
    UINT16          Index;

    SetMem(&Request, sizeof(Request), 0);
    
    Buffer = NULL;

    for (Index = 0; Index < Field->UsageCount; Index++) {
         if (Field->UsagePage == HID_UP_DIGITIZER) {
             if (Field->Usages[Index] == HID_DIGITIZER_DEVICE_MODE && 
                 Field->Usages[Index + 1] == HID_DIGITIZER_DEVICE_IDENTIFIER) {
                        Request.RequestType = HID_RQ_SET_REPORT;
                        Request.Value = (0x03 << 8) | Field->ReportId;
                        Request.Index = DevInfo->InterfaceNum;
                        Request.DataLength = 3;

                        Buffer = USB_MemAlloc (1); 
                        if (Buffer == NULL) {
                            break;
                        }
                        Buffer[0] = Field->ReportId;
                        Buffer[1] = 2;
                        Buffer[2] = 0;

                        UsbControlTransfer(HcStruc, DevInfo, Request, 100, Buffer);

                        USB_MemFree(Buffer, 1);
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
