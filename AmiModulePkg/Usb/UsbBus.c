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

/** @file UsbBus.c
    USB Bus driver implementation

**/
#include "Uhcd.h"

#include "UsbBus.h"
#include "ComponentName.h"

//#pragma warning(disable: 4244)

EFI_DRIVER_BINDING_PROTOCOL gUsbBusDriverBinding = {
    UsbBusSupported,
    UsbBusStart,
    UsbBusStop,
    USB_DRIVER_VERSION,
    NULL,
    NULL
};

extern USB_GLOBAL_DATA *gUsbData;
extern USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC        **gHcTable;
extern QUEUE_T         gQueueCnnctDisc;

TREENODE_T  gUsbRootRootNull;
TREENODE_T  *gUsbRootRoot   = &gUsbRootRootNull;
EFI_EVENT   gEvUsbEnumTimer = 0;
int         gCounterUsbEnumTimer =0;
int         gBustreeLock    = 0;

/**
    This function returns a pointer to USB device from UsbIo.
    @param   UsbIo       Pointer to the EFI_USB_IO_PROTOCOL instance.
    @retval  Pointer to USBDEV_T corresponding to the device

**/

USBDEV_T*
UsbIo2Dev (
    EFI_USB_IO_PROTOCOL  *UsbIo
)
{
    return (USBDEV_T*)((char*)UsbIo - (UINTN)&((USBDEV_T*)0)->io);
}


/**
    Predicate for searching host controller node in the tree
    by HcNumber
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node is not null and node type is host controller and host controller number is same as input data.
                        0 - Node is null or node type is not host controller or host controller number is not same as input data.

**/

int
HcByIndex (
    VOID  *Node,
    VOID  *Data
)
{
    USBBUS_HC_T  *HcNode = (USBBUS_HC_T*)Node;

    return Node && (HcNode->type == NodeHC)
         && (HcNode->hc_data->HcNumber == *(UINT8*)Data);
}


/**
    Predicate for searching host controller node in the tree
    by EFI controller handle
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is host controller and host controller handle is same as input data.
                        0 - Node type is not host controller or host controller handle is not same as input data.

**/

int
HcByHandle (
    VOID  *Node,
    VOID  *Data
)
{
    USBBUS_HC_T  *HcNode = (USBBUS_HC_T*)Node;
    return (HcNode->type == NodeHC) && (HcNode->hc_data->Controller == *(EFI_HANDLE*)Data);
}


/**
    Predicate for searching device node in the tree
    by index of the DEV_INFO structure in the DevInfoTable
    array of USB data
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is Device and Device info is in the DevInfoTable array.
                        0 - Node type is not Device or Device info is not in the DevInfoTable array.

**/

int
DevByIndex (
    VOID  *Node,
    VOID  *Data
)
{
    USBDEV_T  *Dev = (USBDEV_T*)Node;
    return (Dev->type == NodeDevice) && (Dev->dev_info ==
        gUsbDataList->DevInfoTable  + *(UINT8*)Data);
}


/**
    Predicate for searching device node in the tree
    by USB address of the device
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is Device or Group and Device address is same as the input data.
                        0 - Node type is not Device or Group, or Device address is not same as the input data.

**/

int
DevGrpByAddr (
    VOID  *Node,
    VOID  *Data
)
{
    USBDEV_T  *Dev = (USBDEV_T*)Node;
    return (Dev->type == NodeDevice || Dev->type ==  NodeGroup) &&
        (Dev->dev_info->DeviceAddress == *(UINT8*)Data);
}

/**
    Predicate for searching device node in the tree
    by parent hub port of the device, interface and LUN
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is Device or Group and Device information is same as the input data.
                        0 - Node type is not Device or Group, or Device information is not same as the input data.

**/

int
DevGrpByPortIf (
    VOID  *Node,
    VOID  *Data
)
{
    USBDEV_T  *Dev = (USBDEV_T*)Node;
    return (Dev->type == NodeDevice || Dev->type ==  NodeGroup) &&
        (Dev->dev_info->HubPortNumber == ((DEV_INFO*)Data)->HubPortNumber ) &&
        (Dev->dev_info->InterfaceNum == ((DEV_INFO*)Data)->InterfaceNum ) &&
        (Dev->dev_info->Lun == ((DEV_INFO*)Data)->Lun);
}


/**
    Predicate for searching device node in the tree
    by comparing pointers to the DEV_INFO structure
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is Device and Device information is same as the input data.
                        0 - Node type is not Device or Device information is not same as the input data.

**/

int
DevByInfo (
    VOID  *Node,
    VOID  *Data
)
{
    USBDEV_T  *Dev = (USBDEV_T*)Node;
    return (Dev->type == NodeDevice) && (Dev->dev_info == (DEV_INFO*)Data);
}


/**
    Predicate for searching device node in the tree
    by USB address and interface number of the device
    @param   Node       Pointer to Node.
    @param   Data       Pointer to Data.
    @retval  Value      1 - Node type is Device and Device information is same as the input data.
                        0 - Node type is not Device or Device information is not same as the input data.

**/

int
DevByAdrIf (
    VOID  *Node,
    VOID  *Data
)
{
    USBDEV_T  *Dev = (USBDEV_T*)Node;

    return ((Dev->type == NodeDevice) &&
        (Dev->dev_info->DeviceAddress == ((DEV_INFO*)Data)->DeviceAddress) &&
        (Dev->dev_info->InterfaceNum == ((DEV_INFO*)Data)->InterfaceNum) &&
        (Dev->dev_info->Lun == ((DEV_INFO*)Data)->Lun));
}


/**
    Retrieve DEVGROUP_T that is parent of
    the specified USB device in the USB Bus tree

    @param    Dev   Pointer to Device for which the parent is requested
    @retval   Data  Pointer to parent data of the specified USB device in the USB Bus tree.

**/

DEVGROUP_T*
UsbDevGetGroup (
    USBDEV_T  *Dev
)
{
    return (DEVGROUP_T*)Dev->node.parent->data;
}

/**
    Search gUsbData for information about Device linked to an EFI handle

    @param    Controller    Device handle
    @retval   DevInfo       Pointer to Device information.

**/

DEV_INFO* 
FindDevStruc(
    EFI_HANDLE Controller
)
{
    UINTN   Index;
 
    for (Index = 0; Index < gUsbData->MaxDevCount; Index++) {
        if ((gUsbDataList->DevInfoTable[Index].Flag & DEV_INFO_VALIDPRESENT) 
            != DEV_INFO_VALIDPRESENT) {
            continue;
        }
        if (Controller == (EFI_HANDLE)*(UINTN*)gUsbDataList->DevInfoTable[Index].Handle) {
            return &gUsbDataList->DevInfoTable[Index];
        }
    }
    
    return NULL;
}


/**
    Builds a new path appending a USB segment

    @param    Dp               Pointer to Device path
    @param    HubPortNumber    Port Number under this Hub
    @param    InterfaceNum     Interface Number
    @retval   Dp               Pointer to new Device path

**/

EFI_DEVICE_PATH_PROTOCOL*
DpAddUsbSegment(
    EFI_DEVICE_PATH_PROTOCOL    *Dp,
    UINT8                       HubPortNumber,
    UINT8                       InterfaceNum
)
{
    USB_DEVICE_PATH    DpNewSegment;
    
    SetMem(&DpNewSegment, sizeof(USB_DEVICE_PATH), 0);
    DpNewSegment.Header.Type = MESSAGING_DEVICE_PATH;
    DpNewSegment.Header.SubType = MSG_USB_DP;
    SetDevicePathNodeLength(&DpNewSegment.Header, sizeof(DpNewSegment));

    DpNewSegment.InterfaceNumber = InterfaceNum;
    DpNewSegment.ParentPortNumber = HubPortNumber;
    return  AppendDevicePathNode(Dp, (EFI_DEVICE_PATH_PROTOCOL*)&DpNewSegment);
}


/**
    Builds a new path appending a LUN node

    @param    Dp               Pointer to Device path
    @param    Lun              Device Logical Unit number

    @retval   Dp               Pointer to new Device path

**/

EFI_DEVICE_PATH_PROTOCOL*
DpAddLun(
    EFI_DEVICE_PATH_PROTOCOL  *Dp,
    UINT8                     Lun
)
{
    DEVICE_LOGICAL_UNIT_DEVICE_PATH DpNewSegment;

    SetMem(&DpNewSegment, sizeof(DEVICE_LOGICAL_UNIT_DEVICE_PATH), 0);
    DpNewSegment.Header.Type = MESSAGING_DEVICE_PATH;
    DpNewSegment.Header.SubType = MSG_DEVICE_LOGICAL_UNIT_DP;
    SetDevicePathNodeLength(&DpNewSegment.Header, sizeof(DpNewSegment));

    DpNewSegment.Lun = Lun;
    return  AppendDevicePathNode(Dp,(EFI_DEVICE_PATH_PROTOCOL*)&DpNewSegment);
}


/**
    This function executes a get descriptor command to the
    given USB device and endpoint

    @param  Dev        Pointer to USBDEV_T corresponding to the device
    @param  Buffer     Buffer to be used for the transfer
    @param  Length     Size of the requested descriptor
    @param  DescType   Requested descriptor type
    @param  DescIndex  Descriptor index
    @param  LangIndex  LangIndex

    @retval Buffer     Pointer to memory buffer containing the descriptor
    @retval NULL       On error

**/

UINT8*
ReadUsbDescriptor(
    USBDEV_T  *Dev,
    UINT8     *Buffer,
    UINT16    Length,
    UINT8     DescType,
    UINT8     DescIndex,
    UINT16    LangIndex
)
{
    HC_STRUC    *HcStruc = Dev->hc_info;
    DEV_INFO    *DevInfo = Dev->dev_info;
    UINT8       GetDescIteration;
    UINT16      Reg;
    UINT16      Status;

    for (GetDescIteration = 0; GetDescIteration < 3; GetDescIteration++) {
        Reg = (UINT16)((DescType << 8) + DescIndex);
        Status = UsbSmiControlTransfer(
                        HcStruc,
                        DevInfo,
                        (UINT16)USB_RQ_GET_DESCRIPTOR,
                        (UINT16)LangIndex,
                        Reg,
                        Buffer,
                        Length);
        if (Status) {
            return Buffer;
        }
        if (gUsbData->UsbLastCommandStatusExtended & USB_TRNSFR_TIMEOUT) {
            break;
        }
        // 10 ms
        gBS->Stall(10 * 1000);
    }

    return NULL;
}


/**
    Allocates memory necessary to hold complete descriptor
    and returns the descriptor there

    @param  Dev         Pointer to USBDEV_T corresponding to the device
    @param  Type        Requested descriptor type
    @param  Index       Descriptor index
    @param  LangIndex   LangIndex

    @retval Desc        Pointer to memory buffer containing the descriptor
    @retval NULL        On error

**/

USB_DESCRIPTOR_T*
GetUsbDescriptor(
    USBDEV_T    *Dev,
    UINT8       Type,
    UINT8       Index,
    UINT16      LangIndex
)
{
    UINT8               Buffer[0xFF];
    USB_DESCRIPTOR_T    *Desc;
    UINT8               *DescBuffer;
    
    SetMem(&Buffer, sizeof(Buffer), 0);
    DescBuffer= ReadUsbDescriptor(Dev, Buffer, sizeof(Buffer), Type, Index, LangIndex);

    if ((DescBuffer == NULL) || 
        (((USB_DESCRIPTOR_T*)Buffer)->DescriptorType != Type) ||
        (((USB_DESCRIPTOR_T*)Buffer)->Length == 0)) {
        return NULL;
    }
    gBS->AllocatePool(EfiBootServicesData, ((USB_DESCRIPTOR_T*)Buffer)->Length, (VOID**)&Desc);

    CopyMem(Desc, Buffer, ((USB_DESCRIPTOR_T*)Buffer)->Length);

    return Desc;
}


/**
    Delocates memory that was used by the descriptor
    @param  Desc        Pointer to Config Descriptor

**/

VOID
FreeConfigDesc (
    VOID  *Desc
)
{
    if (Desc != 0) gBS->FreePool(Desc);
}


/**
    Returns pointer to the next descriptor for the pack of
    USB descriptors located in continues memory segment
    - result of reading CONFIG_DESCRIPTOR
    @param  Desc        Pointer to Config Descriptor
    @param  Offset      Pointer to Offset
    @retval TRUE        Next descriptor is in the pack of USB descriptors
    @retval FALSE       Next descriptor is not in the pack of USB descriptors

    @note     Uses TotalLength of the CONFIG_DESCRIPTOR and Length
              field of each USB descriptor found inside the pack

**/

BOOLEAN
NextDescriptor(
    IN EFI_USB_CONFIG_DESCRIPTOR  *Desc,
    IN OUT UINTN                  *Offset
)
{
    if (Desc == NULL || *Offset >= Desc->TotalLength) return FALSE;
    if (((EFI_USB_CONFIG_DESCRIPTOR*)((char*)Desc + *Offset))->Length == 0) return FALSE;
    *Offset += ((EFI_USB_CONFIG_DESCRIPTOR*)((char*)Desc + *Offset))->Length;
    if (*Offset >= Desc->TotalLength) return FALSE;

    return TRUE;
}


/**
    Returns a pointer to the memory containing CONFIG_DESCRIPTOR
    reported by the USB device
    @param   Grp    Pointer to the DEVGROUP_T structure
    @retval  Desc   Pointer to Config Descriptor
**/

EFI_USB_CONFIG_DESCRIPTOR*
DevGroupConfigDesc (
    DEVGROUP_T  *Grp
)
{
    return Grp->f_DevDesc && (Grp->active_config != -1)? 
        Grp->configs[Grp->active_config]:NULL;
}

/**
    Returns a pointer to the memory containing CONFIG_DESCRIPTOR
    reported by the USB device
    @param   Dev    Pointer to USBDEV_T corresponding to the device
    @retval  Desc   Pointer to Config Descriptor
**/

EFI_USB_CONFIG_DESCRIPTOR*
UsbDevConfigDesc (
    USBDEV_T  *Dev
)
{
    return DevGroupConfigDesc(UsbDevGetGroup(Dev));
}


/**
    Parses through the configuration descriptor searching for IAD
    If found, IAD details are added to the DEVGROUP_T structure
    @param   Grp         Pointer to the DEVGROUP_T structure
    @retval  EFI_STATUS  Status of the operation

**/
EFI_STATUS
FindIadDetails (
    DEVGROUP_T  *Grp
)
{
    EFI_USB_CONFIG_DESCRIPTOR *CfgDesc;
    EFI_USB_INTERFACE_DESCRIPTOR  *TmpDesc;
    UINTN Offset;
    IAD_DETAILS *IadDetails;
    UINT8 Index;
    EFI_STATUS Status;

    // Count IADs
    for(Index = 0; Index < Grp->config_count; Index++)
    {
        CfgDesc = Grp->configs[Index];
        for(Offset = 0; NextDescriptor(CfgDesc, &Offset);)
        {
            TmpDesc = (EFI_USB_INTERFACE_DESCRIPTOR *)((UINT8*)CfgDesc + Offset);
            if (TmpDesc->DescriptorType == DESC_TYPE_IAD)
            {
                Grp->iad_count++;
            }
        }
    }
    if (Grp->iad_count == 0) return EFI_NOT_FOUND;    // No IADs found
    
    // Allocate iad_details
    Status = gBS->AllocatePool (EfiBootServicesData,
        Grp->iad_count*sizeof(IAD_DETAILS), (VOID**)&Grp->iad_details);
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;
    
    SetMem(Grp->iad_details, Grp->iad_count*sizeof(IAD_DETAILS), 0);
    
    // Fill in the IAD details
    IadDetails = Grp->iad_details;
    for(Index = 0; Index < Grp->config_count; Index++)
    {
        CfgDesc = Grp->configs[Index];
        for(Offset = 0; NextDescriptor(CfgDesc, &Offset);)
        {
            TmpDesc = (EFI_USB_INTERFACE_DESCRIPTOR *)((UINT8*)CfgDesc + Offset);
            if (TmpDesc->DescriptorType != DESC_TYPE_IAD) continue;

            IadDetails->ConfigIndex = Index;
            IadDetails->Descriptor = (AMI_USB_INTERFACE_ASSOCIATION_DESCRIPTOR*)TmpDesc;

            // Allocate and zero IadDetails->Data
            Status = gBS->AllocatePool (EfiBootServicesData,
                IadDetails->Descriptor->bInterfaceCount*sizeof(USBIAD_DATA_T),
                (VOID**)&IadDetails->Data);
            ASSERT_EFI_ERROR(Status);
            if (EFI_ERROR(Status)) return Status;
            
            SetMem(IadDetails->Data, IadDetails->Descriptor->bInterfaceCount*sizeof(USBIAD_DATA_T), 0);
            
            IadDetails++;
        }
    }
    USB_DEBUG(DEBUG_INFO, 3, "IAD details: totally %d IADs:\n", Grp->iad_count);
    IadDetails = Grp->iad_details;
    for (Index = 0; Index < Grp->iad_count; Index++, IadDetails++)
    {
        USB_DEBUG(DEBUG_INFO, 3, " IAD#%d. ConfigIndex %d, IAD Descriptor Ofs %x-%x=%x (%d), ifFrom %d ifCount %d\n",
                Index, IadDetails->ConfigIndex,
                Grp->configs[IadDetails->ConfigIndex],
                IadDetails->Descriptor,
                (UINT8*)IadDetails->Descriptor - (UINT8*)Grp->configs[IadDetails->ConfigIndex],
                (UINT8*)IadDetails->Descriptor - (UINT8*)Grp->configs[IadDetails->ConfigIndex],                
                IadDetails->Descriptor->bFirstInterface, IadDetails->Descriptor->bInterfaceCount);
    }
    return EFI_SUCCESS;
}


/**
    Reads DEVICE and CONFIG descriptors for each
    configuration available in the device. Marks
    the index of the buffer containing CONFIG descriptor
    for active configurations currently selected in
    USB device
    @param   Dev    Pointer to USBDEV_T corresponding to the device

**/

VOID
UsbDevLoadAllDescritors(
    DEVGROUP_T  *Dev
)
{
    UINT8      Index;
    EFI_STATUS Status;

    Dev->configs = NULL;
    //
    // Device descriptor
    //
    CopyMem((UINT8*)&Dev->dev_desc, (UINT8*)&Dev->dev_info->DevDesc, sizeof(Dev->dev_desc));
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USB Bus: dev descr: ");
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "cls:%x;subcls:%x;proto:%x;vndr:%x;id:%x\n",
                Dev->dev_desc.DeviceClass,
                Dev->dev_desc.DeviceSubClass,
                Dev->dev_desc.DeviceProtocol,
                Dev->dev_desc.IdVendor,
                Dev->dev_desc.IdProduct);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "\t\tstr1:%x,str2:%x,str3:%x\n",
                Dev->dev_desc.StrManufacturer,
                Dev->dev_desc.StrProduct,
                Dev->dev_desc.StrSerialNumber );

    Dev->f_DevDesc = TRUE;

    //
    // Config descriptor
    //
    Dev->config_count = Dev->dev_desc.NumConfigurations;
    Status = gBS->AllocatePool (EfiBootServicesData,
                   Dev->config_count*sizeof(EFI_USB_CONFIG_DESCRIPTOR*), 
                   (VOID**)&Dev->configs);
    ASSERT_EFI_ERROR(Status);
    SetMem(Dev->configs, Dev->config_count*sizeof(EFI_USB_CONFIG_DESCRIPTOR*), 0);

    Dev->active_config = -1;
    for (Index = 0; Index < Dev->config_count; ++Index) {
        //read each configuration
        //first failed read will terminate loop

        //Optimization: allloc&read MAX size first
        //      and read second time only if total length is greater
        //Read 1 : get total length
        EFI_USB_CONFIG_DESCRIPTOR Tmp = {0,0,};
        UINT8  *Ptr = ReadUsbDescriptor((USBDEV_T*)Dev,(UINT8*)&Tmp,
            sizeof(Tmp),DESC_TYPE_CONFIG,Index, 0);
        //ASSERT(Tmp.DescriptorType == DESC_TYPE_CONFIG);
        //ASSERT(Tmp.TotalLength >= sizeof(Tmp));
        //
        //Addressing timeouts caused by device errors - empty DESC structure will be returned
        //
        if ((Ptr == NULL) || (Tmp.DescriptorType == 0)) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the total length of the configuration descriptor\n");
            break;
        }

        //Read 2: Actual content
        Status = gBS->AllocatePool (EfiBootServicesData, Tmp.TotalLength, (VOID**)&Dev->configs[Index]);
                ASSERT_EFI_ERROR(Status);
                SetMem(Dev->configs[Index], Tmp.TotalLength, 0);

                Ptr = ReadUsbDescriptor((USBDEV_T*)Dev, (UINT8*)Dev->configs[Index],
                Tmp.TotalLength, DESC_TYPE_CONFIG, Index, 0);
        //ASSERT(Dev->configs[Index]->DescriptorType == DESC_TYPE_CONFIG);  //(EIP60640-)
        //
        //Addressing timeouts caused by device errors - empty DESC structure will be returned
        //
        if ((Ptr == NULL) || (Dev->configs[Index]->DescriptorType == 0)) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the configuration descriptor\n");
            gBS->FreePool(Dev->configs[Index]);
            Dev->configs[Index] = 0;
            break;
        }

        if (Dev->configs[Index]->Length == 0) {
            USB_DEBUG(DEBUG_WARN, 3, "The length of the configuration descriptor is 0.\n");
            Dev->configs[Index]->Length = 0x09;
        }
        //config Desc is here

        //Active Config
        if (Dev->configs[Index]->ConfigurationValue == Dev->dev_info->ConfigNum) {
            Dev->active_config = Index;
        }
    }
}

/**
    Enable endpoints for ALT_IF_SETTING
    @param   Device      Pointer to USBDEV_T corresponding to the device
    @param   AltSetting  Setting for ALT_IF_SETTING
    @retval  EFI_STATUS  Status of the operation

**/

EFI_STATUS
EnableEndponts(
    USBDEV_T    *Device,
    UINT8       AltSetting
)
{
    EFI_PHYSICAL_ADDRESS Address = 0;
    EFI_STATUS Status;
    USB_DEBUG(DEBUG_INFO, 3, "EnableEndpoints: HcType is %x\n", Device->hc_info->HcType);
    if (Device->hc_info->HcType != USB_HC_XHCI) return EFI_UNSUPPORTED;
    if (Device->dev_info->IsocDetails.XferRing == 0)
    {
        /*
         Allocate transfer ring's BASE address and store it in Device->dev_info->IsocDetails.XferRing
         Note that after UsbSmiEnableEndpoints call this field will change and will point to BASE's
         parent structure. That is why while freeing we will use *Device->dev_info->IsocDetails.XferRing
        */ 
        Status = gBS->AllocatePages(AllocateAnyPages, EfiBootServicesData, EFI_SIZE_TO_PAGES(0x20000), &Address);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return Status;
        Device->dev_info->IsocDetails.XferRing = (UINTN)Address;
    }
    
    UsbSmiEnableEndpoints(
        Device->hc_info, Device->dev_info, (UINT8*)Device->descIF[AltSetting]
    );

    return EFI_SUCCESS;
}
        
/**
    Locates information about each endpoint inside the
    descriptors pack loaded with CONFIG descriptor
    @param   Dev         Pointer to USBDEV_T corresponding to the device
    @retval  EFI_STATUS  Status of the operation

**/

EFI_STATUS
UsbDevLoadEndpoints(
    USBDEV_T  *Dev
)
{
    EFI_USB_INTERFACE_DESCRIPTOR  *TmpDesc;
    EFI_USB_ENDPOINT_DESCRIPTOR   *EpDesc;
    EFI_USB_CONFIG_DESCRIPTOR     *CfgDesc;
    UINTN                         Index;
    UINTN                         Offset;
    DEVGROUP_T                    *Grp = UsbDevGetGroup(Dev);
    UINT8                         AltIfSetting = Dev->dev_info->AltSettingNum;
    
    ASSERT(AltIfSetting < USB_MAX_ALT_IF);
    
    if ((Dev->LoadedAltIfMap & (1 << AltIfSetting)) != 0) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: LoadEndpoints have been loaded\n");
        return EFI_SUCCESS;
    }
    
    Dev->first_endpoint[AltIfSetting] = Grp->endpoint_count;
    Dev->end_endpoint[AltIfSetting] = Grp->endpoint_count;

    CfgDesc = UsbDevConfigDesc(Dev);
    
    if (CfgDesc == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "USBBUS: LoadEndpoints aborted - no Configuration descriptor\n");
        return EFI_NOT_FOUND;
    }
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
        "\tScanning config desc: %x(type:%x;len:%x;val:%x;total:%x)\n",
        CfgDesc,CfgDesc->DescriptorType,CfgDesc->Length,
        CfgDesc->ConfigurationValue,CfgDesc->TotalLength );

    //
    // Search interface descriptor
    //
    for (Offset = 0; NextDescriptor(CfgDesc, &Offset);) {
        TmpDesc = (EFI_USB_INTERFACE_DESCRIPTOR*)((UINT8*)CfgDesc + Offset);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
            "\t\tdesc: %x(type:%x;len:%x;if:%x;aif:%x)\n",
            TmpDesc,TmpDesc->DescriptorType, TmpDesc->Length,
            TmpDesc->InterfaceNumber,  TmpDesc->AlternateSetting);
        
        if (TmpDesc->DescriptorType == DESC_TYPE_INTERFACE &&
            TmpDesc->InterfaceNumber == Dev->dev_info->InterfaceNum &&
            TmpDesc->AlternateSetting == AltIfSetting)
        {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "\t...IF found.\n" );

            Dev->descIF[AltIfSetting] = TmpDesc;

            ASSERT(TmpDesc->NumEndpoints < COUNTOF(Grp->endpoints));
            for (Index = 0; Index < TmpDesc->NumEndpoints && NextDescriptor(CfgDesc, &Offset);) {
                EpDesc = (EFI_USB_ENDPOINT_DESCRIPTOR*)((UINT8*)CfgDesc + Offset);
                if (EpDesc->DescriptorType == DESC_TYPE_ENDPOINT) {
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
                        "\t\tendpoint desc: %x index:%x;type:%x;len:%x;addr:%x;pcksz:%x;attr:%x,interval:%x)\n",
                        EpDesc, Grp->endpoint_count, EpDesc->DescriptorType,EpDesc->Length,
                        EpDesc->EndpointAddress, EpDesc->MaxPacketSize, EpDesc->Attributes, EpDesc->Interval);
                    Grp->endpoints[Grp->endpoint_count++] = EpDesc;
                    Grp->a2endpoint[COMPRESS_EP_ADR(EpDesc->EndpointAddress)] = EpDesc;
                    Index++;
                }
                if (EpDesc->DescriptorType == DESC_TYPE_INTERFACE){
                    //Oops, we stepped into another interface
                    break;
                }
            }
            Dev->end_endpoint[AltIfSetting] = Grp->endpoint_count;
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
                    "\t\tinterface endpoints: first:%x; last:%x, grp.endpoint_count:%x\n",
                        Dev->first_endpoint[AltIfSetting], Dev->end_endpoint[AltIfSetting], Grp->endpoint_count);
            Dev->LoadedAltIfMap |= (1 << AltIfSetting);
            if ((AltIfSetting != 0) && (Dev->dev_info->BaseClass != BASE_CLASS_HUB)) {
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "\t\t...Enable endpoints for ALT_IF_SETTING %d...\n", AltIfSetting);
                EnableEndponts(Dev, AltIfSetting);
            }
            break;
        }
    }
    return EFI_SUCCESS;
}


/**
    Retrieves information about a max packet size
    for the specified endpoint of the device
    @param   Endpoint    Endpoint of the device
    @param   Dev         Pointer to USBDEV_T corresponding to the device
    @retval  Size        Max Packet Size for the specified endpoint of the device

**/

UINT16
GetMaxPacket(
    UINT8       Endpoint,
    USBDEV_T    *Dev
)
{
    DEVGROUP_T                   *Grp = UsbDevGetGroup(Dev);
    EFI_USB_ENDPOINT_DESCRIPTOR  *Desc = Grp->a2endpoint[COMPRESS_EP_ADR(Endpoint)];
    if (Desc == 0) return 0;
    return Desc->MaxPacketSize;
}


/**
    Retrieves endpoint descriptor pointer
    @param   Endpoint    Endpoint of the device
    @param   Dev         Pointer to USBDEV_T corresponding to the device
    @retval  Size        Max Packet Size for the specified endpoint of the device
    @retval  Desc        Pointer to endpoint Descriptor

**/

EFI_USB_ENDPOINT_DESCRIPTOR*
GetEndpointDesc(
    UINT8       Endpoint,
    USBDEV_T    *Dev
)
{
    DEVGROUP_T                   *Grp = UsbDevGetGroup(Dev);
    EFI_USB_ENDPOINT_DESCRIPTOR  *Desc;

    if (((Endpoint & 0x7F)==0) || ((Endpoint & 0x7F) > 0xF))
        return NULL;
    Desc = Grp->a2endpoint[COMPRESS_EP_ADR(Endpoint)];

    return Desc;
}


/**
    Protocol USB IO function that performs USB transfer.

    @param    UsbIo       Pointer to EFI_USB_IO_PROTOCOL
    @param    Request     Request type (low byte)
                          Request code, a one byte code describing the actual
                          device request to be executed (ex: Get Configuration,
                          Set Address, etc.)
    @param    Direction   Bit 7   : Data direction
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
    @param    Timeout     Time out value
    @param    Data        Buffer containing data to be sent to the device or buffer
                          to be used to receive data
    @param    DataLength  Length request parameter, number of bytes of data to be
                          transferred in or out of the host controller
    @param    UsbStatus   Point to transfer result

    @retval   EFI_STATUS  Status of the operation

**/

EFI_STATUS
EFIAPI
UsbIoControlTransfer(
    IN EFI_USB_IO_PROTOCOL      *UsbIo,
    IN EFI_USB_DEVICE_REQUEST   *Request,
    IN EFI_USB_DATA_DIRECTION   Direction,
    IN UINT32                   Timeout,
    IN OUT VOID                 *Data,
    IN UINTN                    DataLength,
    OUT UINT32                  *UsbStatus
)
{
    USBDEV_T    *Dev;
    EFI_STATUS  Status;
    EFI_TPL     OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    
    if (Request == NULL || UsbStatus == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }
    
    if (Direction > EfiUsbNoData) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    if ((Request->Request == (USB_RQ_SET_INTERFACE >> 8)) &&
        (Request->RequestType == (USB_RQ_SET_INTERFACE & 0x0F) ) &&
        (Request->Index == Dev->dev_info->InterfaceNum)) {
        
        Dev->dev_info->AltSettingNum = (UINT8)Request->Value;
        Status = UsbDevLoadEndpoints(Dev);
        
        if (EFI_ERROR(Status)) {
            gBS->RestoreTPL(OldTpl);
            return EFI_DEVICE_ERROR;
        }
    }

    Status = Dev->hc->ControlTransfer(
                 Dev->hc, Dev->dev_info->DeviceAddress, GetSpeed(Dev),
                 (UINTN)Dev->dev_info->Endp0MaxPacket,       //(EIP81612)
                 Request, Direction, Data, &DataLength, Timeout, NULL, UsbStatus);

    gBS->RestoreTPL(OldTpl);

    return Status;
}


/**
    Protocol USB IO function that performs USB bulk transfer.

    @param    UsbIo       Pointer to EFI_USB_IO_PROTOCOL
    @param    Endpoint    Endpoint of the device
    @param    Data        Buffer containing data to be sent to the device or buffer
                          to be used to receive data
    @param    DataLength  Length request parameter, number of bytes of data to be
                          transferred in or out of the host controller
    @param    Timeout     Time out value
    @param    UsbStatus   Point to transfer result

    @retval EFI_SUCCESS             The bulk transfer succeeded.
    @retval EFI_INVALID_PARAMETER   Input is not valid.
**/

EFI_STATUS
EFIAPI
UsbIoBulkTransfer(
  IN EFI_USB_IO_PROTOCOL    *UsbIo,
  IN UINT8                  Endpoint,
  IN OUT VOID               *Data,
  IN OUT UINTN              *DataLength,
  IN UINTN                  Timeout,
  OUT UINT32                *UsbStatus
)
{
    USBDEV_T                     *Dev;
    UINT16                       MaxPacket;
    UINT8                        ToggleBit = (Endpoint & 0xF) - 1;
    UINT16                       *DataSync;
    UINT8                        Toggle;
    EFI_STATUS                   Status;
    EFI_USB_ENDPOINT_DESCRIPTOR  *EpDesc;
    DEV_INFO                     *DevInfoToDataSync;
    EFI_TPL                      OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    EpDesc = GetEndpointDesc(Endpoint, Dev);
 
    if (((Endpoint & 0x7F)==0) || ((Endpoint & 0x7F) > 0xF)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Data == NULL || DataLength == NULL || UsbStatus == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (EpDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }
    
    MaxPacket = EpDesc->MaxPacketSize;
    ASSERT(Dev->dev_info);
    ASSERT(Dev->hc);

    if (UsbStatus == NULL || MaxPacket == 0) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if ((EpDesc->Attributes & EP_DESC_FLAG_TYPE_BITS) != EP_DESC_FLAG_TYPE_BULK) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    if (Dev->dev_info->Lun0DevInfoPtr) {
        DevInfoToDataSync = Dev->dev_info->Lun0DevInfoPtr;
    }else {
        DevInfoToDataSync = Dev->dev_info;
    }

    if (Endpoint & 0x80) {
        DataSync = &DevInfoToDataSync->DataInSync;
    }else {
        DataSync = &DevInfoToDataSync->DataOutSync;
    }

    GETBIT(*DataSync, Toggle, ToggleBit);

    Status = Dev->hc->BulkTransfer(
        Dev->hc, Dev->dev_info->DeviceAddress, Endpoint, GetSpeed(Dev),
        MaxPacket, 1, &Data, DataLength, &Toggle, Timeout, NULL, UsbStatus);

    SETBIT(*DataSync, Toggle, ToggleBit);

    gBS->RestoreTPL(OldTpl);

    return Status;
}


/**
    Protocol USB IO function that performs USB async interrupt transfer.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Endpoint              Endpoint of the device
    @param    IsNewTransfer         New transfer check
    @param    PollingInterval       Polling interval
    @param    DataLength            Data length
    @param    InterruptCallback     Callback function
    @param    Context               Point to Context
    
    @retval EFI_SUCCESS             The Async interrupt transfer succeeded.
    @retval EFI_INVALID_PARAMETER   Input is not valid.
**/

EFI_STATUS
EFIAPI
UsbIoAsyncInterruptTransfer(
    IN EFI_USB_IO_PROTOCOL              *UsbIo,
    IN UINT8                            Endpoint,
    IN BOOLEAN                          IsNewTransfer,
    IN UINTN                            PollingInterval,
    IN UINTN                            DataLength,
    IN EFI_ASYNC_USB_TRANSFER_CALLBACK  InterruptCallback,
    IN VOID                             *Context
)
{
    USBDEV_T                     *Dev;
    UINT8                        Toggle;
    UINT8                        ToggleBit = (Endpoint & 0xF) - 1;
    UINT16                       *DataSync;
    EFI_STATUS                   Status;
    EFI_USB_ENDPOINT_DESCRIPTOR  *EpDesc;
    DEV_INFO                     *DevInfoToDataSync;
    EFI_TPL                      OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    EpDesc = GetEndpointDesc(Endpoint, Dev);

        // Check whether Endpoint is valid
    if (EpDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if ((EpDesc->Attributes & EP_DESC_FLAG_TYPE_BITS) != EP_DESC_FLAG_TYPE_INT) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (IsNewTransfer && (PollingInterval < 1 || PollingInterval > 255)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (IsNewTransfer) {
        if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
            gBS->RestoreTPL(OldTpl);
            return EFI_DEVICE_ERROR;
        }
    }

    if (Dev->dev_info->Lun0DevInfoPtr) {
        DevInfoToDataSync = Dev->dev_info->Lun0DevInfoPtr;
    } else {
        DevInfoToDataSync = Dev->dev_info;
    }

    if (Endpoint & 0x80) {
        DataSync = &DevInfoToDataSync->DataInSync;
    } else {
        DataSync = &DevInfoToDataSync->DataOutSync;
    }

    GETBIT(*DataSync, Toggle, ToggleBit);

    Status = Dev->hc->AsyncInterruptTransfer(
        Dev->hc, Dev->dev_info->DeviceAddress, Endpoint,
        GetSpeed(Dev), EpDesc->MaxPacketSize, IsNewTransfer,
        &Toggle, PollingInterval, DataLength, NULL,
        InterruptCallback, Context);

    SETBIT(*DataSync, Toggle, ToggleBit);

    if (!EFI_ERROR(Status)) {
        Dev->async_endpoint = IsNewTransfer ? Endpoint : 0;
    }
    
    gBS->RestoreTPL(OldTpl);
    
    return Status;
}


/**
    Protocol USB IO function that performs USB sync interrupt transfer.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Endpoint              Endpoint of the device
    @param    Data                  Point to interrupt transfer data
    @param    DataLength            Point to interrupt transfer data length
    @param    Timeout               Time out value
    @param    UsbStatus             Point to transfer result
    
    @retval EFI_SUCCESS             The interrupt transfer succeeded.
    @retval EFI_INVALID_PARAMETER   Input is not valid.
**/

EFI_STATUS
EFIAPI
UsbIoSyncInterruptTransfer(
    IN     EFI_USB_IO_PROTOCOL  *UsbIo,
    IN     UINT8                Endpoint,
    IN OUT VOID                 *Data,
    IN OUT UINTN                *DataLength,
    IN     UINTN                Timeout,
    OUT    UINT32               *UsbStatus
)
{
    USBDEV_T                     *Dev;
    UINT8                        Toggle;
    UINT8                        ToggleBit = (Endpoint & 0xF) - 1;
    UINT16                       *DataSync;
    EFI_STATUS                   Status;
    EFI_USB_ENDPOINT_DESCRIPTOR  *EpDesc;
    DEV_INFO                     *DevInfoToDataSync;
    EFI_TPL                      OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    EpDesc = GetEndpointDesc(Endpoint, Dev);

    ASSERT(Dev->dev_info);
    ASSERT(Dev->hc);

    // Check whether Endpoint is valid
    if (EpDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if ((EpDesc->Attributes & EP_DESC_FLAG_TYPE_BITS) != EP_DESC_FLAG_TYPE_INT) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Data == NULL || DataLength == NULL || UsbStatus == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }
                                                                                //(EIP84215+)>
    if (Dev->dev_info->Lun0DevInfoPtr) {
        DevInfoToDataSync = Dev->dev_info->Lun0DevInfoPtr;
    } else {
        DevInfoToDataSync = Dev->dev_info;
    }

    if (Endpoint & 0x80) {
        DataSync = &DevInfoToDataSync->DataInSync;
    } else {
        DataSync = &DevInfoToDataSync->DataOutSync;
    }
                                                                                //<(EIP84215+)
    GETBIT(*DataSync, Toggle, ToggleBit);
    Status = Dev->hc->SyncInterruptTransfer(
        Dev->hc, Dev->dev_info->DeviceAddress, Endpoint,
        GetSpeed(Dev), EpDesc->MaxPacketSize,
        Data, DataLength, &Toggle, Timeout, NULL, UsbStatus);
    
    SETBIT(*DataSync, Toggle, ToggleBit);

    gBS->RestoreTPL(OldTpl);

    return Status;
}

/**
    Protocol USB IO function that performs USB sync isochronous transfer.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Endpoint              Endpoint of the device
    @param    Data                  Point to interrupt transfer data
    @param    DataLength            Point to interrupt transfer data length
    @param    UsbStatus             Point to transfer result

    @retval   EFI_SUCCESS           The Isochronous transfer succeeded.
    @retval   EFI_UNSUPPORTED       The featue not support.
**/

EFI_STATUS
EFIAPI
UsbIoIsochronousTransfer(
    IN EFI_USB_IO_PROTOCOL  *UsbIo,
    IN     UINT8            Endpoint,
    IN OUT VOID             *Data,
    IN     UINTN            DataLength,
    OUT    UINT32           *UsbStatus
)
{
    USBDEV_T                     *Dev;
    EFI_USB_ENDPOINT_DESCRIPTOR  *EpDesc;
    EFI_TPL                      OldTpl;
    EFI_STATUS                   Status;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    EpDesc = GetEndpointDesc(Endpoint, Dev);
    
    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );

    if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
        USB_DEBUG(DEBUG_ERROR, 3, "UsbIoIsochronousTransfer ERROR: device is not present\n");
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }
    
    if (EpDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }
    
    if ((EpDesc->Attributes & EP_DESC_FLAG_TYPE_BITS) != EP_DESC_FLAG_TYPE_ISOC) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if ( Endpoint != Dev->dev_info->IsocDetails.Endpoint ) {
        USB_DEBUG(DEBUG_INFO, 3, "UsbIoIsochronousTransfer ERROR: Endpoint %x whereas dev_info->IsocEndpoint %x\n",
                Endpoint, Dev->dev_info->IsocDetails.Endpoint);
        gBS->RestoreTPL(OldTpl);
        return EFI_UNSUPPORTED;
    }

    Status = Dev->hc->IsochronousTransfer(
                Dev->hc, Dev->dev_info->DeviceAddress, Endpoint, GetSpeed(Dev),
                Dev->dev_info->IsocDetails.EpMaxPkt, 1, &Data, DataLength, NULL, UsbStatus);

    gBS->RestoreTPL(OldTpl);

    return Status;
}


/**
    Protocol USB IO function that performs USB async isochronous transfer.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Endpoint              Endpoint of the device
    @param    Data                  Point to Async interrupt transfer data
    @param    DataLength            Point to Async interrupt transfer data length
    @param    IsochronousCallback   Point to call back function.
    @param    Context               Point to Context

    @retval   EFI_SUCCESS           The Isochronous transfer succeeded.
    @retval   EFI_UNSUPPORTED       The featue not support.
**/

EFI_STATUS
EFIAPI
UsbIoAsyncIsochronousTransfer(
    IN EFI_USB_IO_PROTOCOL              *UsbIo,
    IN UINT8                            Endpoint,
    IN OUT VOID                         *Data,
    IN     UINTN                        DataLength,
    IN EFI_ASYNC_USB_TRANSFER_CALLBACK  IsochronousCallback,
    IN VOID                             *Context
)
{
    USBDEV_T                     *Dev;
    EFI_USB_ENDPOINT_DESCRIPTOR  *EpDesc;
    EFI_TPL                      OldTpl;
    EFI_STATUS                   Status;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    EpDesc = GetEndpointDesc(Endpoint, Dev);
    
    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );

    if (!(Dev->dev_info->Flag & DEV_INFO_DEV_PRESENT)) {
        USB_DEBUG(DEBUG_ERROR, 3, "UsbIoAsyncIsochronousTransfer ERROR: device is not present\n");
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }
    
    if (EpDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }
    
    if ((EpDesc->Attributes & EP_DESC_FLAG_TYPE_BITS) != EP_DESC_FLAG_TYPE_ISOC) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Endpoint != Dev->dev_info->IsocDetails.Endpoint) {
        USB_DEBUG(DEBUG_ERROR, 3, "UsbIoAsyncIsochronousTransfer ERROR: Endpoint %x whereas dev_info->IsocEndpoint %x\n",
                Endpoint, Dev->dev_info->IsocDetails.Endpoint);
        gBS->RestoreTPL(OldTpl);
        return EFI_UNSUPPORTED;
    }

    Status = Dev->hc->AsyncIsochronousTransfer(
        Dev->hc, Dev->dev_info->DeviceAddress, Endpoint, GetSpeed(Dev),
        Dev->dev_info->IsocDetails.EpMaxPkt, 1, &Data, DataLength, NULL, IsochronousCallback, Context);

    gBS->RestoreTPL(OldTpl);

    return Status;
}


/**
    Protocol USB IO function that performs USB Port Reset.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @retval   EFI_SUCCESS           The PortReset succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/

EFI_STATUS
EFIAPI
UsbIoPortReset(
    IN EFI_USB_IO_PROTOCOL  *UsbIo
)
{
    USBDEV_T    *Dev;
    DEVGROUP_T  *Grp;
    UINT8       Status;
    UINT8       Index;
    EFI_TPL     OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    Grp = UsbDevGetGroup(Dev);

    if (Dev->dev_info->DeviceType == BIOS_DEV_TYPE_HUB) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    Status = UsbResetAndReconfigDev(Dev->hc_info, Dev->dev_info);
    if (Status != USB_SUCCESS) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    if (UsbSmiGetDescriptor(Dev->hc_info, Dev->dev_info, (UINT8*)&Grp->dev_desc,
        sizeof(Grp->dev_desc), DESC_TYPE_DEVICE, 0) == 0) {
        gBS->RestoreTPL(OldTpl);
        return EFI_DEVICE_ERROR;
    }

    for (Index = 0; Index < Grp->dev_desc.NumConfigurations; Index++) {
        if (ReadUsbDescriptor(Dev, (UINT8*)Grp->configs[Index],
            Grp->configs[Index]->TotalLength, DESC_TYPE_CONFIG, Index, 0) == 0) {
            gBS->RestoreTPL(OldTpl);
            return EFI_DEVICE_ERROR;
        }
    }

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}


/**
    Retrieves the USB Device Descriptor.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Desc                  Pointer to the caller allocated USB Device Descriptor.
    @retval   EFI_SUCCESS           The Get Device Descriptor succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/

EFI_STATUS
EFIAPI
UsbIoGetDeviceDescriptor(
    IN EFI_USB_IO_PROTOCOL          *UsbIo,
    OUT EFI_USB_DEVICE_DESCRIPTOR   *Desc
)
{
    USBDEV_T    *Dev;
    DEVGROUP_T  *Grp;
    EFI_TPL     OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    Grp = UsbDevGetGroup(Dev);
    
    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );
    
    if (Desc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Grp->f_DevDesc) {
        *Desc = Grp->dev_desc;
        gBS->RestoreTPL(OldTpl);
        return EFI_SUCCESS;
    } else {
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_FOUND;
    }
}


/**
    Retrieves the USB Config Descriptor.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Desc                  Pointer to the caller allocated USB Config Descriptor.
    @retval   EFI_SUCCESS           The Get Config Descriptor succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/

EFI_STATUS
EFIAPI
UsbIoGetConfigDescriptor(
    IN EFI_USB_IO_PROTOCOL          *UsbIo,
    OUT EFI_USB_CONFIG_DESCRIPTOR   *Desc
)
{
    USBDEV_T*   Dev;
    DEVGROUP_T* Grp;
    EFI_TPL OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);
    Grp = UsbDevGetGroup(Dev);
    
    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );
    
    if (Desc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Grp->configs && Grp->active_config != -1 &&
        Grp->configs[Grp->active_config]) {
        *Desc = *Grp->configs[Grp->active_config];
        gBS->RestoreTPL(OldTpl);
        return EFI_SUCCESS;
    } else {
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_FOUND;
    }
}


/**
    Retrieves the USB Interface Descriptor.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL
    @param    Desc                  Pointer to the caller allocated USB Interface Descriptor.
    @retval   EFI_SUCCESS           The Get Interface Descriptor succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/

EFI_STATUS
EFIAPI
UsbIoGetInterfaceDescriptor(
    IN EFI_USB_IO_PROTOCOL              *UsbIo,
    OUT EFI_USB_INTERFACE_DESCRIPTOR    *Desc
)
{
    USBDEV_T    *Dev;
    EFI_TPL     OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);

    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );
    if (Desc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    if (Dev->descIF[Dev->dev_info->AltSettingNum] == NULL) {
        USB_DEBUG(DEBUG_ERROR, 3, "IF NOT FOUND on UsbIo %x Dev %x\n", UsbIo, Dev);
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_FOUND;
    }

    CopyMem (Desc, Dev->descIF[Dev->dev_info->AltSettingNum], sizeof(EFI_USB_INTERFACE_DESCRIPTOR));

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}


/**
    Retrieves the USB Endpoint Descriptor.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @param    EndpointIndex         Index of USB Endpoint.
    @param    Desc                  Pointer to the caller allocated USB Endpoint Descriptor.
    @retval   EFI_SUCCESS           The Get Endpoint Descriptor succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/

EFI_STATUS
EFIAPI
UsbIoGetEndpointDescriptor(
    IN EFI_USB_IO_PROTOCOL           *UsbIo,
    IN  UINT8                        EndpointIndex,
    OUT EFI_USB_ENDPOINT_DESCRIPTOR  *Desc
)
{
    USBDEV_T                     *Dev;
    DEVGROUP_T                   *Grp;
    EFI_USB_ENDPOINT_DESCRIPTOR  *DescCopy;
    UINT8                        FirstEp;
    UINT8                        LastEp;
    EFI_TPL                      OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);

    if (Desc == NULL || EndpointIndex >= 0x10) {
        gBS->RestoreTPL(OldTpl);
        return EFI_INVALID_PARAMETER;
    }

    ASSERT(Dev->dev_info);
    ASSERT(Dev->hc);
    
    FirstEp = (UINT8)Dev->first_endpoint[Dev->dev_info->AltSettingNum];
    LastEp = (UINT8)Dev->end_endpoint[Dev->dev_info->AltSettingNum];

    USB_DEBUG(DEBUG_INFO, 3, "Get Endpoint desc: devaddr: 0x%x; Endpoint: 0x%x\n",
            Dev->dev_info->DeviceAddress, EndpointIndex);
    USB_DEBUG(DEBUG_INFO, 3, "\tfirst Endpoint: 0x%x; last Endpoint: 0x%x\n",
            FirstEp, LastEp - 1);

    if (FirstEp + EndpointIndex >= LastEp) {
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_FOUND;
    }

    ASSERT(FirstEp + EndpointIndex < 0x20);

    Grp = UsbDevGetGroup(Dev);
    DescCopy = Grp->endpoints[FirstEp + EndpointIndex];

    ASSERT( DescCopy );

    if (DescCopy==NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_NOT_FOUND;
    }

    USB_DEBUG(DEBUG_INFO, 3, "\tendp addr: 0x%x; attr: 0x%x; MaxPacket: 0x%x\n",
            DescCopy->EndpointAddress,
            DescCopy->Attributes, DescCopy->MaxPacketSize );

    CopyMem (Desc, DescCopy, sizeof(EFI_USB_ENDPOINT_DESCRIPTOR));

    gBS->RestoreTPL(OldTpl);

    return EFI_SUCCESS;
}


/**
    Retrieves the USB String Descriptor.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @param    LangID                The Language ID for the string being retrieved.
    @param    StringID              The ID of the string being retrieved.
    @param    String                A pointer to a buffer allocated by this function with
                                    AllocatePool() to store the string.If this function 
    @retval   EFI_SUCCESS           The Get String Descriptor succeeded.
    @retval   EFI_INVALID_PARAMETER Input is not valid.

**/
EFI_STATUS
EFIAPI
UsbIoGetStringDescriptor(
    IN EFI_USB_IO_PROTOCOL  *UsbIo,
    IN  UINT16              LangId,
    IN  UINT8               StringId,
    OUT CHAR16              **String
)
{
    USBDEV_T                        *Dev;
    EFI_USB_STRING_DESCRIPTOR       *StrDesc = NULL;
    UINT16                          Index;
    UINT16                          *LangIdTable;
    DEVGROUP_T                      *Grp;
    EFI_STATUS                      Status = EFI_NOT_FOUND;
    EFI_TPL                         OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);

    ASSERT(Dev->dev_info);
    ASSERT(Dev->hc);

    Grp = UsbDevGetGroup(Dev);

    if (StringId == 0 || Grp->lang_table == NULL) {
        //reserved ids: stringid==0 => langid table descripto;
        gBS->RestoreTPL(OldTpl);
        return Status;
    }
    // search langid_table
    LangIdTable = Grp->lang_table->langID;
    for (Index = 0; Index < Grp->lang_table->len ; Index++) {
        if (LangId == LangIdTable[Index]) {
            break;
        }
    }
    
    if (Index == Grp->lang_table->len) {
        gBS->RestoreTPL(OldTpl);
        return Status;
    }
    
    if (LangId == USB_US_LAND_ID) {
        if (StringId == Grp->dev_desc.StrSerialNumber) {
            StrDesc = Grp->SerialNumberStrDesc;
        } else if (StringId == Grp->dev_desc.StrProduct) {
            StrDesc = Grp->ProductStrDesc;
        } else if (StringId == Grp->dev_desc.StrManufacturer) {
            StrDesc = Grp->ManufacturerStrDesc;
        }
    }
    //
    // Get string descriptor: variable size
    //
    if (StrDesc == NULL) {
        StrDesc = (EFI_USB_STRING_DESCRIPTOR*)GetUsbDescriptor(Dev, DESC_TYPE_STRING,
                                StringId, LangId);
    }
    
    if (StrDesc == NULL) {
        gBS->RestoreTPL(OldTpl);
        return Status;
    }

    if (StrDesc->Length > 2 && StrDesc->DescriptorType == DESC_TYPE_STRING) {
        //
        // Allocate memory for string & copy
        //
        if (String != NULL) {
            gBS->AllocatePool(EfiBootServicesData, StrDesc->Length, (VOID**)String);
            SetMem(*String, StrDesc->Length, 0);
            CopyMem(*String, StrDesc->String, StrDesc->Length - 2);
        }
        Status = EFI_SUCCESS;
    }
    if ((StrDesc != Grp->SerialNumberStrDesc) && (StrDesc != Grp->ProductStrDesc) &&
        (StrDesc != Grp->ManufacturerStrDesc)) {
        gBS->FreePool(StrDesc);
    }

    gBS->RestoreTPL(OldTpl);
    
    return Status;
}


/**
    Retrieves the USB Get Supported Languages.

    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @param    LangIdTable           Pointer to the Language ID table.
    @param    TableSize             Pointer to the table size.
    @retval   EFI_SUCCESS           Success to Get Supported Languages.

**/

EFI_STATUS
EFIAPI
UsbIoGetSupportedLanguages(
    IN EFI_USB_IO_PROTOCOL  *UsbIo,
    OUT UINT16              **LangIdTable,
    OUT UINT16              *TableSize )
{
    USBDEV_T        *Dev;
    DEVGROUP_T      *Grp;
    EFI_TPL         OldTpl;

    OldTpl = gBS->RaiseTPL(TPL_NOTIFY);

    Dev = UsbIo2Dev(UsbIo);

    ASSERT( Dev->dev_info );
    ASSERT( Dev->hc );
    
    Grp = UsbDevGetGroup(Dev);
    
    if (LangIdTable == NULL || TableSize == NULL) {
        gBS->RestoreTPL(OldTpl);
        return EFI_SUCCESS;
    }
    
    if (Grp->lang_table == NULL) {
        *LangIdTable  = 0;
        *TableSize = 0;
    } else {
        *LangIdTable = Grp->lang_table->langID;
        *TableSize = Grp->lang_table->len * 2;
    }

    gBS->RestoreTPL(OldTpl);
    return EFI_SUCCESS;
}


/**
    loads STRING descriptor that corresponds to
    the name of the USB device
    @param    Dev         Pointer to USBDEV_T corresponding to the device
    @retval   String      Pointer to String that corresponds to the name of the USB device

**/

CHAR16*
LoadName(
    USBDEV_T  *Dev
)
{
    EFI_USB_INTERFACE_DESCRIPTOR  DescIf;
    EFI_USB_DEVICE_DESCRIPTOR     DescDev;
    DEVGROUP_T                    *Grp = UsbDevGetGroup(Dev);
    CHAR16                        *StrIf = 0;
    CHAR16                        *StrProduct = 0;
    CHAR16                        Lang;
    CHAR16                        *MassStorageName;
    UINT8                         *String;
    UINT8                         Index;

    SetMem(&DescIf, sizeof(EFI_USB_INTERFACE_DESCRIPTOR), 0);
    SetMem(&DescDev, sizeof(EFI_USB_DEVICE_DESCRIPTOR), 0);

    for (Index = 0; Index < USB_NAME_STRING_64; Index++) {
        if (Dev->dev_info->DevNameString[Index] != 0) {
            break;
        }
    }

    if (Index != USB_NAME_STRING_64) {
        gBS->AllocatePool (EfiBootServicesData, USB_NAME_STRING_128, (VOID**)&MassStorageName);
        SetMem(MassStorageName, USB_NAME_STRING_128, 0);
        for (String = (UINT8*)&Dev->dev_info->DevNameString, Index = 0; Index < USB_NAME_STRING_64; Index++) {
            if (String[Index] == 0) break;
            MassStorageName[Index] = (CHAR16)String[Index];
        }
        return MassStorageName;
    }

    if (Grp->lang_table == 0 || Grp->lang_table->len == 0) return 0;

    Lang = Grp->lang_table->langID[0];

    UsbIoGetInterfaceDescriptor(&Dev->io, &DescIf);
    if( DescIf.Interface && !EFI_ERROR(
        UsbIoGetStringDescriptor(&Dev->io, Lang,
        DescIf.Interface, &StrIf )))
        return StrIf;

    UsbIoGetDeviceDescriptor(&Dev->io, &DescDev);
    if( DescDev.StrProduct && !EFI_ERROR(
        UsbIoGetStringDescriptor(&Dev->io, Lang,
        DescDev.StrProduct, &StrProduct )))
        return StrProduct;

    return NULL;
}


/**
    AMI_USB_IAD_PROTOCOL interface function. Returns Pointer to IAD_DETAILS.
    @param    IadProtocol         Pointer to AMI_USB_IAD_PROTOCOL
    @retval   Iad                 Pointer to IAD_DETAILS.
**/
IAD_DETAILS*
IadProt2Iad (
    AMI_USB_IAD_PROTOCOL  *IadProtocol
)
{
    return (IAD_DETAILS*)((char*)IadProtocol - (UINTN)&((IAD_DETAILS*)0)->Iad);
}


/**
    AMI_USB_IAD_PROTOCOL interface function. Returns USB Interface Association Descriptor
    @param    This         Pointer to AMI_USB_IAD_PROTOCOL
    @param    IaDesc       Pointer to AMI_USB_INTERFACE_ASSOCIATION_DESCRIPTOR
    @retval   EFI_SUCCESS  Success to get USB Interface Association Descriptor
**/

EFI_STATUS
EFIAPI
UsbIadGetIad (
    IN AMI_USB_IAD_PROTOCOL                       *This,
    OUT AMI_USB_INTERFACE_ASSOCIATION_DESCRIPTOR  *IaDesc
)
{
    IAD_DETAILS  *Iad = IadProt2Iad(This);
    CopyMem (IaDesc, Iad->Descriptor, sizeof(AMI_USB_INTERFACE_ASSOCIATION_DESCRIPTOR));

    return EFI_SUCCESS;
}


/**
    AMI_USB_IAD_PROTOCOL interface function. Returns USB device handle for a given IAD
    @param    This         Pointer to AMI_USB_IAD_PROTOCOL
    @param    UsbIoIndex   Index of USB Iad data array
    @param    UsbIoHandle  Pointer to Usb Io Handle
    @retval   EFI_SUCCESS  Success to get USB Interface Association Descriptor
**/

EFI_STATUS
EFIAPI
UsbIadGetUsbHandle(
    IN AMI_USB_IAD_PROTOCOL   *This,
    IN UINT8                  UsbIoIndex,
    OUT EFI_HANDLE            *UsbIoHandle
)
{
    IAD_DETAILS  *Iad = IadProt2Iad(This);

    if (UsbIoIndex >= Iad->Descriptor->bInterfaceCount) {
        return EFI_INVALID_PARAMETER;
    }
    ASSERT(Iad->Data[UsbIoIndex].Handle);

    CopyMem (UsbIoHandle, Iad->Data[UsbIoIndex].Handle, sizeof(EFI_HANDLE));

    return EFI_SUCCESS;
}


/**
    AMI_USB_IAD_PROTOCOL interface function. Returns the pointer for the
    device class specific data corresponding to the given UsbIo.
    @param    This         Pointer to AMI_USB_IAD_PROTOCOL
    @param    UsbIoIndex   Index of USB Iad data array
    @param    DataSize     Pointer to Data Size
    @param    Data         Pointer to Data
    @retval   EFI_SUCCESS  Success to get USB Interface Association Descriptor
**/

EFI_STATUS
EFIAPI
UsbIadGetInterfaceData(
    IN AMI_USB_IAD_PROTOCOL   *This,
    IN UINT8                  UsbIoIndex,
    IN OUT UINTN              *DataSize,
    OUT VOID                  *Data
)
{
    EFI_STATUS           Status;
    IAD_DETAILS          *Iad = IadProt2Iad(This);
    USBDEV_T             *Dev;
    EFI_USB_IO_PROTOCOL  *UsbIo;

    if (UsbIoIndex >= Iad->Descriptor->bInterfaceCount) {
        return EFI_INVALID_PARAMETER;
    }
    
    ASSERT(Iad->Data[UsbIoIndex].DataSize);
    
    if (*DataSize < Iad->Data[UsbIoIndex].DataSize) {
        *DataSize = Iad->Data[UsbIoIndex].DataSize;
        return EFI_BUFFER_TOO_SMALL;
    }

    Status = gBS->HandleProtocol(Iad->Data[UsbIoIndex].Handle, &gEfiUsbIoProtocolGuid, (VOID**)&UsbIo);
    ASSERT_EFI_ERROR(Status);
    
    Dev = UsbIo2Dev(UsbIo);
    CopyMem(Data, Dev->descIF[0], Iad->Data[UsbIoIndex].DataSize);
    
    *DataSize = Iad->Data[UsbIoIndex].DataSize;

    return EFI_SUCCESS;
}


/**
    This function checks if a given USBDEV_T is a part of any IAD listed 
    in the given DEVGROUP_T. If so, check for the completion of IAD. If
    IAD is fully populated, install AMI_USB_IAD_PROTOCOL.
    Note: this function called after UsbIo protocol is installed and
    USBDEV_T has a valid UsbIo handle.
    @param    Grp          Pointer to DEVGROUP_T
    @param    Dev          Pointer to USBDEV_T
    @retval   EFI_SUCCESS  Success to install AMI_USB_IAD_PROTOCOL.
**/

EFI_STATUS
ProcessIad (
    DEVGROUP_T  *Grp,
    USBDEV_T    *Dev
)
{    
    UINT8        Index;
    UINT8        Count;
    IAD_DETAILS  *IadDetails;
    UINT8        Interface;
    UINT8        IfFrom;
    UINT8        IfTo;
    EFI_GUID     AmiUsbIadProtocolGuid = AMI_USB_IAD_PROTOCOL_GUID;

    USB_DEBUG(DEBUG_INFO, 3,"ProcessIad: iad_count %d\n", Grp->iad_count);

    if (Grp->iad_count == 0) return EFI_NOT_FOUND;
    
    IadDetails = Grp->iad_details;
    for (Index = 0; Index < Grp->iad_count; Index++, IadDetails++) {
        EFI_USB_CONFIG_DESCRIPTOR *CfgDesc;

        CfgDesc = Grp->configs[IadDetails->ConfigIndex];

        //USB_DEBUG(DEBUG_INFO, 3,"ProcessIad[%d]: config %d %d\n", i, IadDetails->ConfigIndex, Dev->dev_info->ConfigNum);
        if (CfgDesc->ConfigurationValue != Dev->dev_info->ConfigNum) continue;
        //USB_DEBUG(DEBUG_INFO, 3,"ProcessIad[%d]: handle %x\n", i, IadDetails->IadHandle);
        if (IadDetails->IadHandle != NULL) continue;

        // Configuration number match, check for interface number
        Interface = Dev->descIF[0]->InterfaceNumber;
        IfFrom = Grp->iad_details[Index].Descriptor->bFirstInterface;
        IfTo = IfFrom + Grp->iad_details[Index].Descriptor->bInterfaceCount - 1;

        USB_DEBUG(DEBUG_INFO, 3,"ProcessIad[%d]: intrf %d (%d...%d)\n", Index, Interface, IfFrom, IfTo);

        if (Interface >= IfFrom && Interface <= IfTo) {
            // Interface found, update Grp->iad_details->Data
            UINT8 IfIndex = Interface - IfFrom;
            USBIAD_DATA_T* Data = &IadDetails->Data[IfIndex];
            UINTN IfOffset;
            UINTN Offset;
            EFI_USB_INTERFACE_DESCRIPTOR *TmpDesc;

            ASSERT(Data->Handle == NULL);
            if (Data->Handle != NULL) return EFI_DEVICE_ERROR;   // Duplicate interface found

            Data->Handle = Dev->handle;

            // Calculate this interface data size
            IfOffset = (UINTN)Dev->descIF[0] - (UINTN)Grp->configs[IadDetails->ConfigIndex];
            Offset = IfOffset;

            while (NextDescriptor(CfgDesc, &Offset))
            {
                TmpDesc = (EFI_USB_INTERFACE_DESCRIPTOR *)((UINT8*)CfgDesc + Offset);
                if (TmpDesc->DescriptorType == DESC_TYPE_INTERFACE &&
                        TmpDesc->AlternateSetting == 0) break;
                if (TmpDesc->DescriptorType == DESC_TYPE_IAD) break;
            }

            Data->DataSize = Offset - IfOffset;
            
            // Check if all UsbIo is found for this IAD. If so, install IadIoProtocol
            for (Count = 0; Count < Grp->iad_details[Index].Descriptor->bInterfaceCount; Count++) {
                if (IadDetails->Data[Count].Handle == NULL) break;
            }
            if (Count == Grp->iad_details[Index].Descriptor->bInterfaceCount) {
                EFI_STATUS Status;
                
                IadDetails->Iad.GetIad = UsbIadGetIad;
                IadDetails->Iad.GetInterfaceData = UsbIadGetInterfaceData;
                IadDetails->Iad.GetUsbHandle = UsbIadGetUsbHandle;
                Status = gBS->InstallMultipleProtocolInterfaces(
                        &IadDetails->IadHandle,
                        &AmiUsbIadProtocolGuid, &IadDetails->Iad,
                        NULL);
                ASSERT_EFI_ERROR(Status);
                USB_DEBUG(DEBUG_INFO, 3, "ProcessIad: Install IadIo protocol: %r\n", Status);

                Status = gBS->ConnectController (IadDetails->IadHandle, NULL, NULL, TRUE);

                break;
            }
        }
    }
    if (Index == Grp->iad_count) return EFI_NOT_FOUND;
    
    return EFI_SUCCESS;
}


/**
    Adds a device to the tree; creates an EFI handle for the
    usb device; installs USB_IO and DEVICEPATH protocols
    on a new device handle; connects a new device to
    EFI device drivers
    @param    HcNode      Pointer to TREENODE_T
    @param    DevInfo     Pointer to DEV_INFO
    @retval   HubNode     Pointer to TREENODE_T

**/

TREENODE_T*
UsbDevHubNode(
    TREENODE_T  *HcNode,
    DEV_INFO    *DevInfo
)
{
    TREENODE_T  *HubNode = 0;
    TREENODE_T  *HubGrpNode;

    HubGrpNode = TreeSearchDeep (HcNode->child, DevGrpByAddr, &DevInfo->HubDeviceNumber);
    if (HubGrpNode != NULL){
        HubNode = HubGrpNode->child;
        ASSERT(HubNode);
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: parent Hub found: %x\n", HubNode );
        return HubNode;
    }

    return HubNode;
}

/**
    This function install usbio protocol and device path protocol
    for device

    @param  DevInfo   Pointer to the DevInfo structure

**/
VOID
InstallDevice (
    DEV_INFO  *DevInfo
)
{
    TREENODE_T  *HcNode;
    TREENODE_T  *HubNode;
    TREENODE_T  *ParentNode;
    USBDEV_T    *Dev;
    DEVGROUP_T  *Grp;
    EFI_STATUS  Status;
    EFI_TPL     OldTpl;
    UINT16      Index;
    UINT16      *LangId;
    UINT16      OrgTimeOutValue;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: InstallDevice ");
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "(hc:%x,hub:%x,port:%x,addr:%x,if:%x,aif:%x,lun:%x)\n",
        DevInfo->HcNumber, DevInfo->HubDeviceNumber, DevInfo->HubPortNumber, DevInfo->DeviceAddress,
        DevInfo->InterfaceNum, DevInfo->AltSettingNum, DevInfo->Lun );

    // Find HC node in tree
    HcNode = TreeSearchSibling (gUsbRootRoot->child, HcByIndex, &DevInfo->HcNumber);

    // Do not assert here: it's fine to see a DEV_INFO from not-yet-installed HC
    if( HcNode == NULL ) return;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: HC node found: %x\n", HcNode );

    // Find a hub node in tree
    if( DevInfo->HubDeviceNumber & BIT7){ // hub is a root HC
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: Connecting to root Hub\n", DevInfo->HcNumber );
        ASSERT( (DevInfo->HubDeviceNumber & ~BIT7 )== DevInfo->HcNumber );
        HubNode = HcNode;
    } else { // hub is usb hub device
        HubNode = UsbDevHubNode(HcNode, DevInfo);
        // Do not assert here: it may be to see a DEV_INFO from not-yet-installed hub.
        //ASSERT(HubNode != NULL);
        if (HubNode == NULL) return;
    }

    ParentNode = TreeSearchSibling(HubNode->child, 
        DevGrpByAddr, &DevInfo->DeviceAddress );

    if( ParentNode == NULL ){
        // Create group
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: group created\n" );
        Status = gBS->AllocatePool (EfiBootServicesData, sizeof(DEVGROUP_T), (VOID**)&Grp);
        if (EFI_ERROR (Status)) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: Allocate Pool Status %r\n", Status);
        }
        SetMem(Grp, sizeof(DEVGROUP_T), 0);

        Grp->dev_info = DevInfo;
        Grp->hc = ((USBBUS_HC_T*)HcNode->data)->hc;
        Grp->hc_info  = ((USBBUS_HC_T*)HcNode->data)->hc_data;
        Grp->f_DevDesc = FALSE;
        Grp->configs = NULL;
        Grp->config_count = 0;
        Grp->ManufacturerStrDesc = NULL;
        Grp->ProductStrDesc = NULL;
        Grp->SerialNumberStrDesc = NULL;
        Grp->type = NodeGroup;
        Grp->active_config = 0;
        UsbDevLoadAllDescritors(Grp);
        //
        // Check at least for Device Descriptor present before proceeding
        //
        if (Grp->f_DevDesc == FALSE) {
            //
            //When  no Device Descriptor present quit installing the device
            //
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: dev install aborted - no device descriptor\n");
            Grp->dev_info->BusErrorStaus |= BUS_ERROR_DEVICE_CONFIG_DESCRIPTOR;
            return;
        }
        if (Grp->active_config == -1) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: dev install aborted - no active Configuration descriptor\n");
            Grp->dev_info->BusErrorStaus |= BUS_ERROR_DEVICE_CONFIG_DESCRIPTOR;
            return;
        }
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: descriptors loaded\n" );

        if ((gUsbData->UsbFeature & USB_IAD_PROTOCOL) == USB_IAD_PROTOCOL) {
            // Add IAD information if necessary
            Grp->iad_count = 0;
            Grp->iad_details = NULL;
            if (Grp->dev_desc.DeviceClass == DEV_BASE_CLASS_MISC
                && Grp->dev_desc.DeviceSubClass == DEV_SUB_CLASS_COMMON
                && Grp->dev_desc.DeviceProtocol == DEV_PROTOCOL_IAD) {
                FindIadDetails(Grp);
            }
        }
 
                                        //(EIP66231+)>
        if ((Grp->dev_desc.StrManufacturer != 0) || (Grp->dev_desc.StrProduct != 0) || 
            (Grp->dev_desc.StrSerialNumber != 0)) {
            // Load langid table
            Grp->lang_table = (lang_table_t*)GetUsbDescriptor((USBDEV_T*)Grp, DESC_TYPE_STRING, 0, 0);
            if (Grp->lang_table && Grp->lang_table->len != 0) {
                Grp->lang_table->len = (Grp->lang_table->len -2) / sizeof(UINT16);
                USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: LangID table loaded\n" );
                
                LangId = Grp->lang_table->langID;
                
                for (Index = 0; Index < Grp->lang_table->len; Index++) {
                    if (LangId[Index] == USB_US_LAND_ID) {
                        break;
                    }
                }
             
                if (Index != Grp->lang_table->len) {
                    // Some devices may not respond getting string descriptors 
                    // whcih describing manufacturer, product set the timeout value to 100 ms.
                    OrgTimeOutValue = gUsbData->UsbReqTimeOutValue;
                    gUsbData->UsbReqTimeOutValue = USB_GET_STRING_DESC_TIMEOUT_MS;
                    if (Grp->dev_desc.StrManufacturer != 0) {
                        Grp->ManufacturerStrDesc = 
                            (EFI_USB_STRING_DESCRIPTOR*)GetUsbDescriptor(
                            (USBDEV_T*)Grp, DESC_TYPE_STRING, 
                            Grp->dev_desc.StrManufacturer, USB_US_LAND_ID);
                    }
                    if (Grp->dev_desc.StrProduct != 0) {
                        Grp->ProductStrDesc = 
                            (EFI_USB_STRING_DESCRIPTOR*)GetUsbDescriptor(
                            (USBDEV_T*)Grp, DESC_TYPE_STRING, 
                            Grp->dev_desc.StrProduct, USB_US_LAND_ID);
                    }
                    if (Grp->dev_desc.StrSerialNumber != 0) {
                        // Set timeout value to 3000 ms for the serial number string descriptor.
                        gUsbData->UsbReqTimeOutValue = USB_GET_SERIAL_NUMBER_DESC_TIMEOUT_MS;
                        Grp->SerialNumberStrDesc = 
                            (EFI_USB_STRING_DESCRIPTOR*)GetUsbDescriptor(
                            (USBDEV_T*)Grp, DESC_TYPE_STRING, 
                            Grp->dev_desc.StrSerialNumber, USB_US_LAND_ID);
                    }
                    gUsbData->UsbReqTimeOutValue = OrgTimeOutValue;
                }
            }
        }
                                        //<(EIP66231+)
        TreeAddChild(HubNode,(ParentNode = TreeCreate(&Grp->node, Grp)));
    } else {
        // Old group was found in tree
        TREENODE_T *Tmp = TreeSearchSibling(ParentNode->child, DevByAdrIf, DevInfo );
        if(Tmp){
            USB_DEBUG(DEBUG_WARN, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: IF is already in tree: %x\n", Tmp );
            DevInfo->BusErrorStaus |= BUS_ERROR_GROUP_FOUND_IN_TREE;
            return;
        }

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: multi-function; group node found: %x\n", ParentNode );
        Grp = (DEVGROUP_T*)ParentNode->data;
    }

    // Create new device node as a child of HubNode
    gBS->AllocatePool (EfiBootServicesData, sizeof(USBDEV_T), (VOID**)&Dev);
    ASSERT(Dev);
    if (Dev == NULL) return;
    SetMem(Dev, sizeof(USBDEV_T), 0);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: device node created: %x\n",
        &Dev->node );
    Dev->type = NodeDevice;
    TreeAddChild(ParentNode, TreeCreate(&Dev->node, Dev));
    Dev->dev_info = DevInfo;
    Dev->hc = ((USBBUS_HC_T*)HcNode->data)->hc;
    Dev->hc_info  = ((USBBUS_HC_T*)HcNode->data)->hc_data;
    Dev->f_connected = FALSE;

    Status = UsbDevLoadEndpoints(Dev);
    
    if (EFI_ERROR(Status)) {
        Dev->dev_info->BusErrorStaus |= BUS_ERROR_ENDPOINT_DESCRIPTOR;
        return;
    }

    // Speed 00/10/01 - High/Full/Low
                                        //(EIP81612)>
    switch (Dev->dev_info->EndpointSpeed) {
        case USB_DEV_SPEED_SUPER_PLUS:
            Dev->speed = EFI_USB_SPEED_SUPER_PLUS; 
            break;
        case USB_DEV_SPEED_SUPER:
            Dev->speed = EFI_USB_SPEED_SUPER; 
            break;
        case USB_DEV_SPEED_FULL:
            Dev->speed = EFI_USB_SPEED_FULL;
            break;
        case USB_DEV_SPEED_LOW:
            Dev->speed = EFI_USB_SPEED_LOW;
            break;
        case USB_DEV_SPEED_HIGH:
            Dev->speed = EFI_USB_SPEED_HIGH;
    }
                                        //<(EIP81612)
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: endpoints loaded\n" );

    // Create Device Path
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: preparing DP...\n" );

    ASSERT(((USBDEV_T*)HubNode->data)->dp);
    Dev->dp = DpAddUsbSegment(((USBDEV_T*)HubNode->data)->dp,
        DevInfo->HubPortNumber - 1, DevInfo->InterfaceNum);
    if (DevInfo->Lun) {
        Dev->dp = DpAddLun(Dev->dp, DevInfo->Lun);
    }
    ASSERT(Dev->dp);

    // Install USB_IO protocol
    Dev->io.UsbControlTransfer          = UsbIoControlTransfer;
    Dev->io.UsbBulkTransfer             = UsbIoBulkTransfer;
    Dev->io.UsbAsyncInterruptTransfer   = UsbIoAsyncInterruptTransfer;
    Dev->io.UsbSyncInterruptTransfer    = UsbIoSyncInterruptTransfer;
    Dev->io.UsbIsochronousTransfer      = UsbIoIsochronousTransfer;
    Dev->io.UsbAsyncIsochronousTransfer = UsbIoAsyncIsochronousTransfer;

    Dev->io.UsbGetDeviceDescriptor      = UsbIoGetDeviceDescriptor;
    Dev->io.UsbGetConfigDescriptor      = UsbIoGetConfigDescriptor;
    Dev->io.UsbGetInterfaceDescriptor   = UsbIoGetInterfaceDescriptor;
    Dev->io.UsbGetEndpointDescriptor    = UsbIoGetEndpointDescriptor;
    Dev->io.UsbGetStringDescriptor      = UsbIoGetStringDescriptor;
    Dev->io.UsbGetSupportedLanguages    = UsbIoGetSupportedLanguages;

    Dev->io.UsbPortReset                = UsbIoPortReset;

    // DEBUG_DELAY();
    //Install DP_ protocol
    Status = gBS->InstallMultipleProtocolInterfaces (
                 &Dev->handle,
                 &gEfiUsbIoProtocolGuid, &Dev->io,
                 &gEfiDevicePathProtocolGuid, Dev->dp,
                 NULL);
    ASSERT_EFI_ERROR(Status);
        
    *(UINTN*)Dev->dev_info->Handle = (UINTN)Dev->handle;
    Dev->dev_info->Flag |= DEV_INFO_DEV_BUS;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: USB_IO %x installed on Dev %x\n", &Dev->io, Dev );

    {
        VOID* Tmp;
        Status = gBS->OpenProtocol(
            Dev->hc_info->Controller,
            &gEfiUsb2HcProtocolGuid,
            (VOID**)&Tmp,
            gUsbBusDriverBinding.DriverBindingHandle,
            Dev->handle,
            EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER);
        
        ASSERT(Status == EFI_SUCCESS);

    }

    REPORT_STATUS_CODE(EFI_PROGRESS_CODE,DXE_USB_HOTPLUG);

    OldTpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);
    gBS->RestoreTPL(TPL_CALLBACK);

    // Connect controller to start device drvirs
    Status = gBS->ConnectController(Dev->handle, NULL, NULL, TRUE);

    gBS->RaiseTPL(TPL_HIGH_LEVEL);
    gBS->RestoreTPL(OldTpl);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: instdev: connect controller: %r\n", Status );

    if (!EFI_ERROR(Status)) {
        Dev->f_connected = TRUE;
    }
    if ((gUsbData->UsbFeature & USB_IAD_PROTOCOL) == USB_IAD_PROTOCOL)
        ProcessIad(Grp, Dev);
}


/**
    This function uinstalls AMI_USB_IADIO_PROTOCOL and frees memory
    associated with it
    @param    Dev          Pointer to USBDEV_T
    @retval   EFI_SUCCESS  Success to uninstall AMI_USB_IAD_PROTOCOL.
**/

EFI_STATUS
UninstallIad (
    USBDEV_T  *Dev
)
{
    UINT8        Index;
    IAD_DETAILS  *IadDetails;
    DEVGROUP_T   *Grp = UsbDevGetGroup(Dev);
    EFI_GUID     AmiUsbIadProtocolGuid = AMI_USB_IAD_PROTOCOL_GUID;
    
    if (Grp == NULL) return EFI_NOT_FOUND;
    if (Grp->iad_count == 0) return EFI_NOT_FOUND;

    IadDetails = Grp->iad_details;
    
    for (Index = 0; Index < Grp->iad_count; Index++) {
        gBS->DisconnectController(IadDetails->IadHandle, NULL, NULL);
        gBS->FreePool(IadDetails->Data);
        gBS->UninstallMultipleProtocolInterfaces (
                 IadDetails->IadHandle,
                 &AmiUsbIadProtocolGuid, &IadDetails->Iad,
                 NULL
                 );
        IadDetails++;
    }
    gBS->FreePool(Grp->iad_details);
    Grp->iad_details = NULL;
    Grp->iad_count = 0;

    return EFI_SUCCESS;
}


/**
    Disconnects device; uninstalls USB_IO and DEVICEPATH protocols
    delocates all memory used USB Bus driver to support the device
    removes device node from the tree; if device has children, all
    UninstallDevice procedure is repeated for each child.

    In case if disconnect or protocol uninstall fails, it reports
    error stauts returned from Boot service procedure.
    @param    Dev          Pointer to USBDEV_T
    @retval   EFI_SUCCESS  Success to disconnects device.

**/

EFI_STATUS
UninstallDevice (
    USBDEV_T  *Dev
)
{
    EFI_STATUS Status;
    EFI_TPL    OldTpl;

        // Uninstall IAD protocol, if any
    if ((gUsbData->UsbFeature & USB_IAD_PROTOCOL) == USB_IAD_PROTOCOL)
        UninstallIad(Dev);

    //
    // Uninstall connected devices if it's a hub
    //
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: UninstallDevice: node %x; ", &Dev->node);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "info:%x [adr:%d;if:%d] uninstalling children...\n",
        Dev->dev_info,Dev->dev_info->DeviceAddress, Dev->dev_info->InterfaceNum);
    if (TreeSearchSibling (Dev->node.child, eUninstallDevice, &Status))
        return Status;

    OldTpl = gBS->RaiseTPL(TPL_HIGH_LEVEL);
    gBS->RestoreTPL(TPL_CALLBACK);

    Status = gBS->DisconnectController(Dev->handle, NULL, NULL);

    gBS->RaiseTPL(TPL_HIGH_LEVEL);
    gBS->RestoreTPL(OldTpl);

    if (EFI_ERROR(Status)) {
        QueuePut(&gQueueCnnctDisc, Dev->dev_info);
        Dev->dev_info->Flag |= DEV_INFO_IN_QUEUE;
        return Status;
    }

    if (Dev->async_endpoint != 0) {
        Status = Dev->io.UsbAsyncInterruptTransfer(&Dev->io, (UINT8)Dev->async_endpoint, FALSE, 
                        0, 0, NULL, NULL);
        Dev->async_endpoint = 0;
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: AsyncInterruptTransfer(%r)",Status);

    }

    Status = gBS->CloseProtocol (
                      Dev->hc_info->Controller,
                      &gEfiUsb2HcProtocolGuid,
                      gUsbBusDriverBinding.DriverBindingHandle,
                      Dev->handle
                      );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;

    //
    // Try to uninstall protocols
    //
    // they can be denied to uninstall, which result in
    // keeping this device and all bus alive
    //
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: uninstdev: [%d:%d] uninstalling protocols...",
        Dev->dev_info->DeviceAddress, Dev->dev_info->InterfaceNum);
    
    Status = gBS->UninstallMultipleProtocolInterfaces (
                      Dev->handle,
                      &gEfiUsbIoProtocolGuid, &Dev->io,
                      &gEfiDevicePathProtocolGuid, Dev->dp,
                      NULL
                      );
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "%r\n", Status );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) return Status;

    Dev->dev_info->Flag &= ~DEV_INFO_DEV_BUS;
    if (!(Dev->dev_info->Flag & (DEV_INFO_MASS_DEV_REGD | DEV_INFO_DEV_PRESENT))) {
        Dev->dev_info->Flag &= ~DEV_INFO_VALID_STRUC;
    }

    if (Dev->dev_info->IsocDetails.XferRing != 0) {
        UINTN Address = *(UINTN*)Dev->dev_info->IsocDetails.XferRing;
        gBS->FreePages((EFI_PHYSICAL_ADDRESS)Address, EFI_SIZE_TO_PAGES(0x20000));
        Dev->dev_info->IsocDetails.XferRing = 0;
    }

    //
    // Unistall succeeded, free usbdev
    //
    TreeRemove(&Dev->node);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: uninstdev: [%d:%d] done.\n",
        Dev->dev_info->DeviceAddress, Dev->dev_info->InterfaceNum);

    if (Dev->name)
        gBS->FreePool(Dev->name);
    gBS->FreePool(Dev);

    return EFI_SUCCESS;
}


/**
    Enumeration call-back function that is usded
    uninstall all enumerated device nodes
    Stops enumeration as soon as error was recieved
    @param    Node    Tree node of the USB device or group
    @param    Contex  Pointer to the EFI_STATUS variable that
                      recieves status information if error
                      was recieved
    @retval   TRUE    On error found, this will stop enumeration
    @retval   FALSE   On succesfull uninstall operation

**/
int
eUninstallDevice(
    VOID  *Node,
    VOID  *Context
)
{
    EFI_STATUS  *Status = (EFI_STATUS*)Context;
    DEVGROUP_T  *Grp = (DEVGROUP_T*)Node;
    ASSERT(Status);

    if (Grp->type == NodeGroup) {
        //
        // Uninstall all CONNECTED devices within group
        //
        TreeSearchSibling (Grp->node.child, eUninstallDevice, Status);
        if (EFI_ERROR(*Status)) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "USBBUS: devgroup uninstall failed: devaddr:0x%x\n",
                Grp->dev_info->DeviceAddress );
            return TRUE; //stop enumeration
        }
        // Free devgroup
        TreeRemove(&Grp->node);
        if (Grp->configs) {
            int Index;
            for (Index = 0; Index < Grp->config_count; ++Index) {
                if (Grp->configs[Index]) {
                    gBS->FreePool(Grp->configs[Index]);
                }
            }
            gBS->FreePool(Grp->configs);
        }
        if ((gUsbData->UsbFeature & USB_IAD_PROTOCOL) == USB_IAD_PROTOCOL) {
            if (Grp->iad_details) {
                gBS->FreePool(Grp->iad_details);
            }
        }
        if (Grp->ManufacturerStrDesc) {
            gBS->FreePool(Grp->ManufacturerStrDesc);
        }
        if (Grp->ProductStrDesc) {
            gBS->FreePool(Grp->ProductStrDesc);
        }
        if (Grp->SerialNumberStrDesc) {
            gBS->FreePool(Grp->SerialNumberStrDesc);
        }
        gBS->FreePool(Grp);
    } else if (Grp->type == NodeDevice) {
        //
        //Uninstall Device
        //
        *Status = UninstallDevice((USBDEV_T*)Node);
        if (EFI_ERROR(*Status)) {
            USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "USBBUS: usbdev uninstall failed: if:%d\n",
                ((USBDEV_T*)Node)->dev_info->InterfaceNum );
            //stop enumeration
            return TRUE;
        }
    }
    return FALSE;// continue enumeration
}


/**
    Finds the USB hub node that sits on the same
    path (seq. of (hub ports,if) ) but have different
    USB address or DEV_INFO node. This could be the result
    of lost disconnect event or previous error to uninstall
    USB_IO
    @param    HcNode         Pointer to TREENODE_T
    @param    Addr           Device Address
    @retval   EFI_SUCCESS    Echo wasn't found or was succesfully removed
                             otherwise return status resulted from attemp to remove the node
    @retval   EFI_NOT_FOUND  Device not found

**/

EFI_STATUS
RemoveHubEcho(
    TREENODE_T  *HcNode,
    UINT8       Addr
)
{
    while (!(Addr & BIT7)) {
        //
        // Find hub DEV_INFO
        //
        UINTN       Index;
        TREENODE_T  *HubNode;
        TREENODE_T  *DevNode;
        DEV_INFO    *DevInfo = NULL;

        for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
            if (((gUsbDataList->DevInfoTable[Index].Flag &
                (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT | DEV_INFO_DEV_DUMMY))
                != (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT)) &&
                gUsbDataList->DevInfoTable[Index].DeviceAddress == Addr) {
                DevInfo = gUsbDataList->DevInfoTable + Index;
                break;
            }
        }
        if (DevInfo == NULL)
            return EFI_NOT_FOUND;

        //
        // Search for parent hub
        //
        if (DevInfo->HubDeviceNumber & BIT7) {
            //Root hub
            HubNode = HcNode;
        } else {
            //USB hub device
            TREENODE_T  *HubGrpNode = TreeSearchDeep (HcNode->child, DevGrpByAddr, &DevInfo->HubDeviceNumber);
            if (HubGrpNode != NULL) {
                USBDEV_T* Dev;

                HubNode = HubGrpNode->child;
                ASSERT(HubNode);
                DevNode = TreeSearchSibling (HubNode->child, DevGrpByPortIf, DevInfo);
                if (DevNode==NULL) return EFI_SUCCESS;
                Dev = (USBDEV_T*)DevNode->data;
                if (Dev && (DevInfo->DeviceAddress !=
                    Dev->dev_info->DeviceAddress ||
                    DevInfo != Dev->dev_info )) {
                    //
                    // The disconnect event must have been droped
                    // disconnect old info now
                    //

                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: echo found [%x]:\n" );
                    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
                        "\t(hc:0x%x,hub:0x%x,port:%d,addr:0x%x,if:%d)\n",
                        Dev->dev_info->HcNumber,
                        Dev->dev_info->HubDeviceNumber,
                        Dev->dev_info->HubPortNumber,
                        Dev->dev_info->DeviceAddress,
                        Dev->dev_info->InterfaceNum);
                    return RemoveDevInfo(Dev->dev_info);
                }
                return EFI_SUCCESS;
            } else {
                //
                // Either hub wasn't added to bus yet; or there is echo for the
                // brunch. The the later case succesfull removal of the burnch
                // removes an echo for this device info
                //
                Addr = DevInfo->HubDeviceNumber;

            }
        }
    }
    return EFI_SUCCESS;
}


/**
    Finds the USB device node that sits on the same
    path (seq. of (hub ports,if) ) but have different
    USB address or DEV_INFO node. This could be the result
    of lost disconnect event or previous error to uninstall
    USB_IO
    @param  DevInfo      DEV_INFO structure that is checked for
                         echoes in the bus
    @retval EFI_SUCCESS  Echo wasn't found or was succesfully removed
                         otherwise return status resulted from attemp to remove the node

**/

EFI_STATUS
RemoveDevInfoEcho (
    DEV_INFO    *DevInfo
)
{
    USBDEV_T    *Dev;
    TREENODE_T  *DevNode;
    TREENODE_T  *HcNode;
    TREENODE_T  *HubNode;

    HcNode = TreeSearchSibling (gUsbRootRoot->child, HcByIndex, &DevInfo->HcNumber);
    if (HcNode == NULL) return EFI_SUCCESS;

    if (DevInfo->HubDeviceNumber & BIT7) {
        //Root hub
        HubNode = HcNode;
    } else {
        //USB hub device
        TREENODE_T  *HubGrpNode = TreeSearchDeep (HcNode->child, DevGrpByAddr, &DevInfo->HubDeviceNumber);
        if (HubGrpNode != NULL) {
            HubNode = HubGrpNode->child;
            ASSERT(HubNode);
        } else {
            //
            // Either hub wasn't added to bus yet; or there is echo for the
            // brunch. The the later case succesfull removal of the burnch
            // removes an echo for this device info
            //
            return EFI_SUCCESS;
        }
    }
    DevNode = TreeSearchSibling (HubNode->child, DevGrpByPortIf, DevInfo);
    if (DevNode == NULL) return EFI_SUCCESS;
    Dev = (USBDEV_T*)DevNode->data;
    if (Dev != NULL) {
        //
        // The disconnect event must have been droped
        // disconnect old info now
        //

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: echo found [%x]:\n" );
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
            "\t(hc:0x%x,hub:0x%x,port:%d,addr:0x%x,if:%d)\n",
            Dev->dev_info->HcNumber,
            Dev->dev_info->HubDeviceNumber,
            Dev->dev_info->HubPortNumber,
            Dev->dev_info->DeviceAddress,
            Dev->dev_info->InterfaceNum);
        return RemoveDevInfo(Dev->dev_info);
    }
    return EFI_SUCCESS;
}


/**
    Removes device node from the USB bus tree. Device node
    corresponds to the DEV_INFO. Device gets removed in response to
    the pending removal event sheduled from SMM when disconnect
    was detected on USB
    @param  DevInfo      DEV_INFO structure that was disconnect
    @retval EFI_SUCCESS  Echo wasn't found or was succesfully removed
                         otherwise return status resulted from attemp to remove the node

**/

EFI_STATUS
RemoveDevInfo (
    DEV_INFO  *DevInfo
)
{
    TREENODE_T  *DevNode;
    TREENODE_T  *HcNode;
    EFI_STATUS  Status;

    HcNode = TreeSearchSibling (gUsbRootRoot->child, HcByIndex, &DevInfo->HcNumber);
    if (HcNode == NULL) return EFI_NOT_FOUND;

    //Check for echoes

    DevNode = TreeSearchDeep (HcNode->child,DevGrpByAddr, &DevInfo->DeviceAddress);
    if (DevNode == NULL) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_USBBUS, "\tdevice is not found in the tree...\n" );
        return EFI_NOT_FOUND;
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "\tdevice found in the tree...\n" );

    Status = EFI_SUCCESS;
    eUninstallDevice(DevNode->data, &Status);
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "\tDisconnect complete: %r.\n", Status );

    return Status;
}


/**
    Timer call-back routine that is
    used to monitor changes on USB Bus
    It monitors the state of queueCnnct and queueDiscnct queues
    which get populated by UsbSmiNotify call-back routine

    When this routine finds a new device connected to usb it
    will install a device node for that device by calling
    InstallDevice

    When this routine finds a disconneced device it uninstalls the
    device node by calling UninstallDevice

    @param  Event         Efi event occurred upon Periodic Timer
    @param  Context       Not used

**/

VOID
EFIAPI
UsbHcOnTimer(
    EFI_EVENT   Event,
    VOID        *Context
)
{
    DEV_INFO    *EventCnnct = 0;
    static int  Index = 0;
    int         Lock;
    EFI_STATUS  Status;
    DEV_INFO    *DevInfo =  NULL;
    DEV_INFO    *DevInfoEnd = gUsbDataList->DevInfoTable + gUsbData->MaxDevCount;

    UsbSmiPeriodicEvent();

    if (Event) {
        gBS->SetTimer(Event, TimerCancel, ONESECOND /10);
    }

    ATOMIC({Lock = gBustreeLock; gBustreeLock = 1;});

    if (Lock) {
        USB_DEBUG(DEBUG_WARN, DEBUG_LEVEL_USBBUS, "UsbHcOnTimer::  is locked; return\n");
        return;
    }

    for (DevInfo = &gUsbDataList->DevInfoTable[1]; DevInfo != DevInfoEnd; ++DevInfo) {
        if (DevInfo->BusErrorStaus != BUS_NO_ERROR_DEVICE_CONFIG_DESCRIPTOR) {
            continue;
        }
        if (DevInfo->Flag & DEV_INFO_IN_QUEUE) {
            continue;
        }
        if ((DevInfo->Flag & DEV_INFO_VALIDPRESENT) == DEV_INFO_VALID_STRUC) {
            if (DevInfo->Flag & DEV_INFO_DEV_BUS) {
                QueuePut(&gQueueCnnctDisc, DevInfo);
                DevInfo->Flag |= DEV_INFO_IN_QUEUE;
                continue;
            }
        }
        if ((DevInfo->Flag & DEV_INFO_VALIDPRESENT) == DEV_INFO_VALIDPRESENT) {
            if (!(DevInfo->Flag & DEV_INFO_DEV_BUS)) {
                QueuePut(&gQueueCnnctDisc, DevInfo);
                DevInfo->Flag |= DEV_INFO_IN_QUEUE;
            }
        }

    }

    do {
        ATOMIC({EventCnnct = QueueGet(&gQueueCnnctDisc);});

        if (EventCnnct == NULL) {
            break;
        }
        EventCnnct->Flag &= ~DEV_INFO_IN_QUEUE;
        //
        // There is no need to raise tpl here: this is callback of Event with
        // TPL_CALLBACK, so this code doesn't reenter; the Install and Uninstall
        // are also called from driver stop and start. Stop and start protect
        // the code at TPL_CALLBACK level
        //
        if ((EventCnnct->Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT |
            DEV_INFO_DEV_DUMMY)) == (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT) ) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "UsbHcOnTimer:: event connect [%d]: %x\n", Index++, EventCnnct);

            RemoveDevInfoEcho(EventCnnct);
            InstallDevice(EventCnnct);
        } else if ((EventCnnct->Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT |
            DEV_INFO_DEV_DUMMY)) == DEV_INFO_VALID_STRUC) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
                "UsbHcOnTimer:: event disconnect [%d]: %x\n", Index++, EventCnnct);
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,
                "\t(hc:0x%x,hub:0x%x,port:%d,addr:0x%x,if:%d)\n",
                EventCnnct->HcNumber, EventCnnct->HubDeviceNumber,
                EventCnnct->HubPortNumber, EventCnnct->DeviceAddress,
                EventCnnct->InterfaceNum);
            Status = RemoveDevInfo(EventCnnct);
            if (EFI_ERROR(Status)) {
                break;
            }
        }
    } while (EventCnnct != NULL);
    
    gBustreeLock = 0;

    if (Event) {
        gBS->SetTimer(Event, TimerPeriodic, ONESECOND / 10);
    }
}


/**
    Enumerate all DEV_INFO structures in the DevInfoTable array
    and install all currently connected device
    @param  HCStruc           Pointer to the HCStruc structure
**/

VOID
PopulateTree (
    HC_STRUC    *HcStruc
)
{
    UINT16  Index;

    REPORT_STATUS_CODE(EFI_PROGRESS_CODE,DXE_USB_DETECT);

    UsbHcOnTimer(NULL, NULL);
    for (Index = 1; Index < gUsbData->MaxDevCount; Index++) {
        if ((gUsbDataList->DevInfoTable[Index].Flag & (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT | 
            DEV_INFO_DEV_DUMMY | DEV_INFO_DEV_BUS)) == (DEV_INFO_VALID_STRUC | DEV_INFO_DEV_PRESENT)) {
            if (HcStruc != gHcTable[gUsbDataList->DevInfoTable[Index].HcNumber - 1]) {
                continue;
            }
                //
                // Valid and present device behind specified HC, so insert it in the tree
                //
            if ((gUsbDataList->DevInfoTable[Index].BusErrorStaus != BUS_NO_ERROR_DEVICE_CONFIG_DESCRIPTOR)) {
                continue;
            }
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBBUS: PopulateTree: found valid dev info[%d]: %x\n", Index, gUsbDataList->DevInfoTable + Index );   
            if (!(gUsbDataList->DevInfoTable[Index].Flag & DEV_INFO_IN_QUEUE)) {
                QueuePut(&gQueueCnnctDisc, &gUsbDataList->DevInfoTable[Index]);
                gUsbDataList->DevInfoTable[Index].Flag |= DEV_INFO_IN_QUEUE;
            }
        }
    }
}


/**
    This function is a part of binding protocol, it returns
    the string with the controller name.
    @param   Controller      Handle of controller
    @param   Child           Child Handle of controller
    @retval  name            Controller name
**/

CHAR16*
UsbBusGetControllerName (
    EFI_HANDLE  Controller,
    EFI_HANDLE  Child
)
{
    EFI_STATUS           Status;
    EFI_USB_IO_PROTOCOL  *UsbIo = NULL;
    USBDEV_T             *Dev;

    Status = gBS->OpenProtocol (
                      Controller,
                      &gEfiUsb2HcProtocolGuid,
                      NULL,
                      gUsbBusDriverBinding.DriverBindingHandle,
                      Controller,
                      EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                      );

    if ((Status == EFI_SUCCESS) || (Status == EFI_ALREADY_STARTED)) {
        if (Child) {
            Status = gBS->HandleProtocol(
                              Child,
                              &gEfiUsbIoProtocolGuid,
                              (VOID**)&UsbIo
                              );
        } else {
            Status = EFI_UNSUPPORTED;
        }
    } else {
        Status = gBS->OpenProtocol(
                          Controller,
                          &gEfiUsbIoProtocolGuid,
                          (VOID**)&UsbIo,
                          gUsbBusDriverBinding.DriverBindingHandle,
                          Controller,
                          EFI_OPEN_PROTOCOL_GET_PROTOCOL
                          );
    }

    if (!EFI_ERROR(Status) && (UsbIo != NULL)) {
        //Get name for USB device
        Dev = UsbIo2Dev(UsbIo);
        if (Dev->name == NULL)
            Dev->name = LoadName(Dev);
        return Dev->name;
    } else {
        return NULL;
    }
}


/**
    This is AmiUsbProtocol API function. It activates
    endpoint polling for a given interrupt endpoint.
    @param    AmiUsbProtocol        Pointer to EFI_USB_PROTOCOL.
    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @param    IntEndpoint           Interrupt endpoint.
    @param    IntMaxPktSize         Max Packet size.
    @param    IntInterval           Polling Interval.
    @retval   EFI_SUCCESS           Success to Activate Polling.
    @retval   EFI_NOT_FOUND         Device not found.

**/

EFI_STATUS
UsbActivatePolling (
    EFI_USB_PROTOCOL  *AmiUsbProtocol,
    VOID              *UsbIo,
    UINT8             IntEndpoint,
    UINT16            IntMaxPktSize,
    UINT8             IntInterval
)
{
    USBDEV_T  *Dev = UsbIo2Dev((EFI_USB_IO_PROTOCOL*)UsbIo);
    if (Dev == NULL) return EFI_NOT_FOUND;
    
    Dev->dev_info->IntInEndpoint = IntEndpoint;
    Dev->dev_info->IntInMaxPkt = IntMaxPktSize;
    Dev->dev_info->PollingLength = IntInterval;
    Dev->dev_info->PollInterval = IntInterval;
    UsbSmiActivatePolling(Dev->hc_info, Dev->dev_info);
    
    return EFI_SUCCESS;
}

/**
    This is AmiUsbProtocol API function. It deactivates
    interrupt endpoint polling for a given UsbIo.
    @param    AmiUsbProtocol        Pointer to EFI_USB_PROTOCOL.
    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @retval   EFI_SUCCESS           Success to Deactivate Polling.
    @retval   EFI_NOT_FOUND         Device not found.
**/

EFI_STATUS
UsbDeactivatePolling (
    EFI_USB_PROTOCOL  *AmiUsbProtocol,
    VOID              *UsbIo
)
{
    USBDEV_T  *Dev = UsbIo2Dev((EFI_USB_IO_PROTOCOL*)UsbIo);

    if (Dev == NULL) return EFI_NOT_FOUND;
    UsbSmiDeactivatePolling(Dev->hc_info, Dev->dev_info);
    
    return EFI_SUCCESS;
}


/**
    This call executes non-blocking Asyncronous Isoch transfer. The
    standard UsbIo->UsbAsyncIsochronousTransfer is extended with the
    transfer completion information: XferComplete is updated when the
    requested amount of data (Length) is transferred.
    This call may be useful for isochronous streaming, where XferComplete
    indicator may signal the event indicating the certain amount transfer
    completion.
    @param    AmiUsbProtocol        Pointer to EFI_USB_PROTOCOL.
    @param    UsbIo                 Pointer to EFI_USB_IO_PROTOCOL.
    @param    Buffer                Pointer to Buffer.
    @param    IsocEndpoint          Isochronous endpoint.
    @param    MaxPkt                Max Packet.
    @param    Mult                  Transaction per microframe.
    @param    Length                Buffer Length.
    @param    TransferDetails       Pointer to TransferDetails.
    @param    XferComplete          Pointer to XferComplete.
    @retval   EFI_SUCCESS           Success to Deactivate Polling.
    @retval   EFI_NOT_FOUND         Device not found.
**/

EFI_STATUS
EFIAPI
UsbAccumulateIsochronousData (
    EFI_USB_PROTOCOL  *AmiUsbProtocol,
    VOID              *UsbIo,
    UINT8             *Buffer,
    UINT8             IsocEndpoint,
    UINT16            MaxPkt,
    UINT8             Mult,
    UINT32            Length,
    UINT32            *TransferDetails,
    UINT8             *XferComplete
)
{
    USBDEV_T  *Dev = UsbIo2Dev((EFI_USB_IO_PROTOCOL*)UsbIo);

    if (Dev == NULL) return EFI_NOT_FOUND;

    // Update DEV_INFO with the isoc endpoint details
    Dev->dev_info->IsocDetails.EpMaxPkt = MaxPkt;
    Dev->dev_info->IsocDetails.EpMult = Mult;
    Dev->dev_info->IsocDetails.XferDetails = TransferDetails;
    Dev->dev_info->IsocDetails.Endpoint = IsocEndpoint;

    ASSERT(Dev->dev_info->IsocDetails.Endpoint);
    
    CRITICAL_CODE( TPL_NOTIFY,
    {
        UsbSmiIsocTransfer(
            Dev->hc_info,
            Dev->dev_info,
            Dev->dev_info->IsocDetails.Endpoint & 0x80,
            Buffer,
            Length,
            XferComplete
        );
    )};

    return EFI_SUCCESS;
}


/**
    Install driver binding and controller name protocols
    for the USB bus driver. Hook up UsbProtocol with the
    UsbBus related functions.
    @param  ImageHandle    The firmware allocated handle for the EFI image.
    @param  SystemTable    A pointer to the EFI System Table.
    @retval   EFI_SUCCESS           Success
    @retval   Others                Failure
**/

EFI_STATUS
UsbBusInit (
    EFI_HANDLE  ImageHandle,
    EFI_HANDLE  ServiceHandle
)
{
    EFI_USB_PROTOCOL    *AmiUsbProtocol;
    EFI_STATUS          Status;

    Status = gBS->LocateProtocol(&gEfiUsbProtocolGuid, NULL, (VOID**)&AmiUsbProtocol);
    if (EFI_ERROR(Status)) {
        return Status;
    }

    AmiUsbProtocol->UsbAccumulateIsochronousData = UsbAccumulateIsochronousData;

    gUsbBusDriverBinding.DriverBindingHandle = ServiceHandle;
    gUsbBusDriverBinding.ImageHandle = ImageHandle;

    SetMem(&gUsbRootRootNull, sizeof(TREENODE_T), 0);
    
    // Install driver binding protocol here
    Status = EfiLibInstallDriverBindingComponentName2 (
                 ImageHandle,
                 gST,
                 &gUsbBusDriverBinding,
                 gUsbBusDriverBinding.DriverBindingHandle,
                 NULL,
                 &gComponentNameUsbBus
                 );

    return Status;
}


/**
    This is a binding protocol function that returns EFI_SUCCESS
    for USB controller and EFI_UNSUPPORTED for any other kind of
    controller.
    @param    This                 Protocol instance pointer
    @param    Controller           Handle of device to test
    @param    DevicePath           Pointer to EFI_DEVICE_PATH_PROTOCOL
    @retval   EFI_SUCCESS          Success for USB controller.
    @retval   EFI_UNSUPPORTED      Unsupport for any other kind of controller.

**/

EFI_STATUS
EFIAPI
UsbBusSupported (
    EFI_DRIVER_BINDING_PROTOCOL  *This,
    EFI_HANDLE                   Controller,
    EFI_DEVICE_PATH_PROTOCOL     *DevicePath
)
{
    EFI_STATUS  Status;
    VOID        *Ptr = 0;
    HC_STRUC    *HcStruc;

    Status = gBS->OpenProtocol ( Controller, &gEfiUsb2HcProtocolGuid,
        (VOID**)&Ptr, This->DriverBindingHandle, Controller, EFI_OPEN_PROTOCOL_BY_DRIVER );

    if (Status != EFI_SUCCESS && Status != EFI_ALREADY_STARTED) {
        return EFI_UNSUPPORTED;
    } else if (Status == EFI_ALREADY_STARTED) {
        return Status;
    }

    gBS->CloseProtocol ( Controller, &gEfiUsb2HcProtocolGuid,
        This->DriverBindingHandle, Controller);

    HcStruc = FindHcStruc(Controller);

    if (HcStruc == NULL) {
        return EFI_UNSUPPORTED;
    }

    Status = gBS->OpenProtocol ( Controller, &gEfiDevicePathProtocolGuid,
        NULL, This->DriverBindingHandle, Controller, EFI_OPEN_PROTOCOL_TEST_PROTOCOL );

    if (Status != EFI_SUCCESS && Status != EFI_ALREADY_STARTED) {
        return EFI_UNSUPPORTED;
    }

    return Status;
}

/**
    This function is part of binding protocol installed on USB
    controller. It stops the controller and removes all the
    children.
    @param This              Protocol instance pointer.
    @param Controller        Handle of Controller to stop driver on
    @param NumberOfChildren  Number of Children in the ChildHandleBuffer
    @param Children          List of handles for the children need to stop.
    @retval EFI_SUCCESS      This driver is removed ControllerHandle
    @retval other            This driver was not removed from this device

**/

EFI_STATUS
EFIAPI
UsbBusStop (
   EFI_DRIVER_BINDING_PROTOCOL     *This,
   EFI_HANDLE                      Controller,
   UINTN                           NumberOfChildren,
   EFI_HANDLE                      *Children
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    TREENODE_T  *HcNode;
    USBBUS_HC_T *HcDev;

    HcNode = TreeSearchSibling(gUsbRootRoot->child, HcByHandle, &Controller );
    ASSERT(HcNode);
    if (HcNode == NULL) {
        return EFI_DEVICE_ERROR;
    }
    HcDev = (USBBUS_HC_T*)HcNode->data;
    
    Status = gBS->SetTimer(gEvUsbEnumTimer, TimerCancel, ONESECOND / 10);
        
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return Status;
    }

    UsbHcOnTimer(NULL, NULL);
    
    if (TreeSearchSibling(HcNode->child, eUninstallDevice, &Status) != NULL) {
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, 
            "USBBUS: Stop HC: [%d] failed to uninstall children\n",
            ((USBBUS_HC_T*)HcNode->data)->hc_data->HcNumber);
        Status = gBS->SetTimer(gEvUsbEnumTimer, TimerPeriodic, ONESECOND / 10);

        ASSERT(Status == EFI_SUCCESS);

        return Status;
    }

    if (NumberOfChildren == 0) {
        TreeRemove(HcNode);
        
        //
        // Close Protocols opened by driver
        //
        gBS->CloseProtocol (
                Controller, &gEfiUsb2HcProtocolGuid,
                This->DriverBindingHandle, Controller);
        
        gBS->FreePool(HcDev);
        
        if (--gCounterUsbEnumTimer == 0) {
            Status = gBS->SetTimer(gEvUsbEnumTimer, TimerCancel, ONESECOND / 10);

            ASSERT(Status == EFI_SUCCESS);

            Status = gBS->CloseEvent(gEvUsbEnumTimer);

            ASSERT(Status == EFI_SUCCESS);

            gEvUsbEnumTimer = 0;
        }
    }

    if (gCounterUsbEnumTimer != 0) {
        Status = gBS->SetTimer(gEvUsbEnumTimer, TimerPeriodic, ONESECOND / 10);
        ASSERT(Status == EFI_SUCCESS);
    }

    return EFI_SUCCESS;
}


/**
    This function is part of binding protocol installed on USB
    controller. It starts the USB bus for a given controller.
    @param  This                Pointer to driver binding protocol
    @param  Controller          Controller handle.
    @param  DevicePathProtocol  Pointer to device path protocol

    @retval EFI_STATUS Status of the operation
**/

EFI_STATUS
EFIAPI
UsbBusStart (
    EFI_DRIVER_BINDING_PROTOCOL *This,
    EFI_HANDLE                  Controller,
    EFI_DEVICE_PATH_PROTOCOL    *DevicePathProtocol
)
{
    USBBUS_HC_T     *HcDev = 0;
    EFI_STATUS      Status;

    Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      sizeof(USBBUS_HC_T),
                      (VOID**)&HcDev
                      );
            
    ASSERT(Status == EFI_SUCCESS);

    if (EFI_ERROR(Status)) {
        return Status;
    }
    
    HcDev->type = NodeHC;
    HcDev->hc_data = FindHcStruc(Controller);

    if (HcDev->hc_data == NULL) {
        gBS->FreePool(HcDev);
        return EFI_DEVICE_ERROR;
    }

    //
    // Open Protocols
    //
    Status = gBS->OpenProtocol ( Controller,
                &gEfiUsb2HcProtocolGuid, (VOID**)&HcDev->hc,
                This->DriverBindingHandle, Controller,
                EFI_OPEN_PROTOCOL_BY_DRIVER );
        
    ASSERT(Status == EFI_SUCCESS);
        
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gBS->OpenProtocol ( Controller,
        &gEfiDevicePathProtocolGuid,
        (VOID**)&HcDev->dp, This->DriverBindingHandle,
        Controller, EFI_OPEN_PROTOCOL_GET_PROTOCOL );
    
    ASSERT(Status == EFI_SUCCESS);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    REPORT_STATUS_CODE(EFI_PROGRESS_CODE, EFI_IO_BUS_USB | EFI_IOB_PC_INIT);

    //
    // Install Polling timer
    //
    {
        if (gEvUsbEnumTimer != 0) {
            Status = gBS->SetTimer(gEvUsbEnumTimer, TimerCancel, ONESECOND / 10);
            ASSERT(Status == EFI_SUCCESS);
        }

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS,"**** \tnew hc_struc: %x(type:%x,number:%x)\n",
                HcDev->hc_data, HcDev->hc_data->HcType, HcDev->hc_data->HcNumber);

        //
        // Create HC branch in the USB root
        //
        TreeAddChild(gUsbRootRoot, TreeCreate(&HcDev->node,HcDev));

        gCounterUsbEnumTimer++;
        PopulateTree(HcDev->hc_data);

        UsbHcOnTimer(NULL, NULL);

        // Setting up global: gUsbDeviceToDisconnect, gUsbDeviceToConnect
        if (gEvUsbEnumTimer == 0) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_USBBUS, "USBBUS: Start: setup timer callback %x\n", &UsbHcOnTimer );
            Status = gBS->CreateEvent( EVT_TIMER | EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK, UsbHcOnTimer, 0, &gEvUsbEnumTimer);
            
            ASSERT(Status == EFI_SUCCESS);
        }
        Status = gBS->SetTimer(gEvUsbEnumTimer, TimerPeriodic, ONESECOND / 10);
        ASSERT(Status == EFI_SUCCESS);
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
