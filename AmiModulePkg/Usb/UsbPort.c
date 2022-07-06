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

/** @file UsbPort.c
    AMI USB Porting file

**/

#include "Uhcd.h"

static UINT8   DeviceNumber = 1;

extern USB_DATA_LIST     *gUsbDataList;

//-----------------------------------------------------------------------------
// This part is linked with USBRT and located in SMI

/**
    This table contains list of vendor & device IDs of USB
    devices that are non-compliant. This is currently used
    only for USB mass storage devices but can be extended to
    other type of non-compliant devices also.

**/

USB_BADDEV_STRUC    gUsbBadDeviceTable[] = {
// Intel, Lacie hard disk
    {0x059f, 0xa601,
        0, 0, PROTOCOL_CBI_NO_INT,
        0},

// In-systems ATA bridge
    {0x05ab, 0x0060,
        0, 0, 0,
        USB_INCMPT_START_UNIT_NOT_SUPPORTED},

// Data Store Technologies, USB 2 ATA bridge
    {0x04e6, 0x0001,
        BASE_CLASS_MASS_STORAGE, 0, PROTOCOL_CBI_NO_INT,
        USB_INCMPT_BOT_STATUS_FAILED},

// NEC, Floppy drive
    {0x0409, 0x0040,
        0, 0, PROTOCOL_CBI_NO_INT,
        0},

// Hana flash drive
    {0x090a, 0x1001,
        0, 0, 0, \
        USB_INCMPT_SINGLE_LUN_DEVICE +
        USB_INCMPT_MODE_SENSE_NOT_SUPPORTED},

// Compact Flash reader
    {0x04e6, 0x000a,
        0, 0, 0,
        USB_INCMPT_MODE_SENSE_NOT_SUPPORTED},

// ScanLogic SL11R-IDE and Ennyah RW4420U
    {0x04ce, 0x0002,
        0, SUB_CLASS_SL11R, PROTOCOL_BOT,
        0},

// BAFO Slim CDR-W BF-2100
    {0x09cc, 0x0404,
        0, 0, PROTOCOL_CBI,
        0},

//Panasonic USB CD/RW Model:KXL-RW21AN
    {0x04da, 0x0d06,
        BASE_CLASS_MASS_STORAGE, 0, PROTOCOL_CBI,
        0},

    {0x04da, 0x0d07,
        BASE_CLASS_MASS_STORAGE, 0, PROTOCOL_CBI,
        0},

//TaiDen Technology:CoolFlash
    {0x0ea0, 0x6803,
        0, 0, 0,
        USB_INCMPT_MODE_SENSE_NOT_SUPPORTED},

// A-Bit USB Mouse(Model:97M32U)
    {0x0605, 0x0001,
        0, 0, 0,
        USB_INCMPT_SET_BOOT_PROTOCOL_NOT_SUPPORTED},

// The problem with this card reader is fixed elsewhere
// HP 4-in-1 Card reader (does not support Read Format Capacity command)
//  {0x6050, 0x0034,
//      0, 0, 0,
//      USB_INCMPT_FORMAT_CAPACITY_NOT_SUPPORTED},

// Silicon Motion Inc., Taiwan: USB Flash Disk
    {0x090c, 0x1000,
        0, 0, 0,
        USB_INCMPT_GETMAXLUN_NOT_SUPPORTED},

//Intel KVM HID
    {0x8086, 0x2b,
        0, 0, 0,
        USB_INCMPT_HID_DATA_OVERFLOW},

// SANWA Supply MA-LS11DS USB Mouse
    {0x04fc, 0x0801,
        0, 0, 0,
        USB_INCMPT_BOOT_PROTOCOL_IGNORED},
                                        //(EIP75441+)>
// SanDisk 2GB Sansa Clip+ MP3 Player
    {0x0781, 0x74D0,
        0xFF, 0xFF, 0xFF,
        0},

// SanDisk Sansa Fuze 4GB Flash Portable Media Player
    {0x0781, 0x74C2,
        0xFF, 0xFF, 0xFF,
        0},
                                        //<(EIP75441+)
// Genovation USB Mini-Terminal Model #904-RJ
    {0x16C0, 0x0604,
        0, 0, 0,
        USB_INCMPT_DISABLE_DEVICE},
// End of incompatible device list
    {0, 0, 0, 0, 0, 0}
};

extern  USB_GLOBAL_DATA  *gUsbData;  //EIP58029

DEV_INFO*  EFIAPI  USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);

/**
    This function intends to skip port.
    @param  HubAddr              Hub address
    @param  PortNum              Port Number
    @param  Bdf                  Data of Bus, Defice, Function
    @param  BaseClass            Base class of device
    @param  ValidBaseClass       If ValidBaseClass = 1, the BaseClass is valid.
    @retval TRUE                 Match the skip table, skip this device.
    @retval FALSE                Not match the skip table.
**/ 

BOOLEAN
EFIAPI
OEMSkipList (
    UINT8       HubAddr,
    UINT8       PortNum,
    UINT16      Bdf,
    UINT8       BaseClass,
    UINT8       ValidBaseClass
)
{
                                        //(EIP88776+)>
    USB_SKIP_LIST   *UsbSkipListTable;
    DEV_INFO        *TmpDevInfo;
    UINT8           TablePortNumber;
    UINT8           ConnectDeviceNumber;
    UINT8           ConnectPortNumber;
    UINT16          Index;
    UINT16          TableLevel;
    UINT16          ConnectLevel;
    UINT32          TablePath;
    UINT32          ConnectPath;
    UINT32          ConnectPathMask;
    UINT8           SkipListCount;
 
    UsbSkipListTable = (USB_SKIP_LIST*) gUsbDataList->UsbSkipListTable;

    if (UsbSkipListTable == NULL) {
        return FALSE;
    }
    if ((UsbSkipListTable->BDF == 0) && (UsbSkipListTable->BaseClass == 0))
        return FALSE;

    for (SkipListCount = 1; (UsbSkipListTable->BDF != 0) && (SkipListCount < gUsbData->MaxSkipListCount);
         UsbSkipListTable++, SkipListCount++) {
        TablePortNumber = UsbSkipListTable->RootPort;
        TablePath = UsbSkipListTable->RoutePath;
        for (Index = 1; Index < 5; Index++, TablePath >>= 4) {
            if (TablePath == 0) {
                break;
            }
        }

        TablePath = UsbSkipListTable->RoutePath;
        TableLevel = Index;
        USB_DEBUG(DEBUG_INFO, 4, "TableLevel = %x TablePath = %x TablePortNumber = %x BDF = %x\n",
                TableLevel,TablePath,TablePortNumber, UsbSkipListTable->BDF);

        //get connect path and level
        ConnectDeviceNumber = HubAddr; 
        ConnectPortNumber = PortNum;
        for (Index = 1, ConnectPath = 0; Index < 5; Index++) {
            if (ConnectDeviceNumber & BIT7) {
                break;
            }
            ConnectPath = ConnectPath << 4;
            ConnectPath |= ConnectPortNumber;

            // Get the device info structure for the matching device address
            TmpDevInfo = USB_GetDeviceInfoStruc (
                             USB_SRCH_DEV_ADDR,
                             0,
                             ConnectDeviceNumber,
                             0
                             );
            if (TmpDevInfo == NULL) {
                break;
            }
            ConnectDeviceNumber = TmpDevInfo->HubDeviceNumber;
            ConnectPortNumber   = TmpDevInfo->HubPortNumber;
        }
        ConnectLevel = Index;
        USB_DEBUG(DEBUG_INFO, 4, "ConnectLevel = %x ConnectPath = %x ConnectPortNumber = %x BDF = %x\n",
                    ConnectLevel, ConnectPath, ConnectPortNumber, Bdf);

        //Skip by all 
        if (UsbSkipListTable->SkipType == SKIP_FOR_ALLCONTROLLER) {
            if (UsbSkipListTable->Flag == SKIP_FLAG_SKIP_LEVEL) {
                if (TableLevel != ConnectLevel) {
                    continue;
                }
            }

            if (UsbSkipListTable->BaseClass == 0) {
                return TRUE;
            }

            if (UsbSkipListTable->BaseClass == BaseClass) {
                return TRUE;
            }

            continue;
        }

        if (UsbSkipListTable->BDF != Bdf) {
            continue;
        }

        //Skip by controller
        if (UsbSkipListTable->RootPort == 0) {
            if (UsbSkipListTable->BaseClass != 0) {
                if (UsbSkipListTable->BaseClass != BaseClass) {
                    continue;
                } else {
                    return TRUE;
                }
            }
            return TRUE;
        }
        //Skip usb ports which include down stream ports.
        if (UsbSkipListTable->Flag == SKIP_FLAG_SKIP_MULTI_LEVEL) {
            ConnectPathMask = 0xFFFFFFFF >> (4 * (8 - (TableLevel)));
            ConnectPathMask = ConnectPathMask >> 4;
            if (((ConnectPath & ConnectPathMask) == TablePath) && (ConnectPortNumber == TablePortNumber)) {
                if (UsbSkipListTable->BaseClass != 0) {
                    if (UsbSkipListTable->BaseClass == BaseClass) {
                        return TRUE;
                    }
                    continue;
                }
                return TRUE;
            }
            continue;
        }

        if (TableLevel != ConnectLevel) {
            continue;
        }
        //Skip usb ports on the same level.
        if (UsbSkipListTable->Flag == SKIP_FLAG_SKIP_LEVEL) {
            if (UsbSkipListTable->BaseClass == 0) {
                return TRUE;
            }

            if (UsbSkipListTable->BaseClass == BaseClass) {
                return TRUE;
            }
        }
        for (Index = 0; Index < TableLevel; Index++, ConnectPath >>= 4, TablePath >>= 4) {
            if (Index == (TableLevel - 1)) { 
                switch (UsbSkipListTable->Flag) {
                    case SKIP_FLAG_SKIP_PORT:
                        if ((ConnectPath == TablePath) && (ConnectPortNumber == TablePortNumber)) {
                            if (UsbSkipListTable->BaseClass != 0) {
                                if (UsbSkipListTable->BaseClass == BaseClass) {
                                    return TRUE;
                                }
                                break;
                            }
                            return TRUE;
                        }
                        break;
                    case SKIP_FLAG_KEEP_PORT:
                        if (ConnectPortNumber == TablePortNumber) {
                            if (ConnectPath == TablePath) {
                                if (UsbSkipListTable->BaseClass != 0) {
                                    if (ValidBaseClass == 1) {
                                        if (UsbSkipListTable->BaseClass != BaseClass) {
                                            return TRUE;
                                        }
                                    }
                                }
                                break;
                            }
                            return TRUE;
                        }
                        break;
                    default:
                        break;
                }
            } else {
                if ((ConnectPath & 0xf) != (TablePath & 0xf)) {
                    break;
                }
            }
        }
    }

    return FALSE;
                                        //<(EIP88776+)
}

/**
    This function provides Oem to check for non-compliant 
    USB devices and updates the data structures appropriately 
    to support the device.

    @param  HCStruc      HCStruc pointer
    @param  DevInfo      Device information structure pointer
    @param  Desc         Pointer to the descriptor structure
    @param  Length       Buffer length
    @param  DescLength   End offset of the device descriptor

**/

VOID
EFIAPI
UsbOemCheckNonCompliantDevice (
    HC_STRUC    *HCStruc,
    DEV_INFO    *DevInfo,
    UINT8       *Desc,
    UINT16      Length,
    UINT16      DescLength
)
{
}

/**
    This function will insert the USB device number into the devicename string.
    Format----> "U(DeviceNumber)-DevNameString"

    @param  Device    Pointer to a device data structure

  
**/
VOID
EFIAPI
AddPortNumbertoDeviceString (
    DEV_INFO        *Device
)
{
    UINT8   Index;
    UINT8   IndexTemp;
    UINT8   TempArray[50];
    UINT8   Appendarray1[5] = {'U', 0, '-', 0};
    UINT8   Appendarray2[6] = {'U', 0, 0, '-', 0};

    for (Index = 0 ; Device->DevNameString[Index]; Index++) {
        TempArray[Index] = Device->DevNameString[Index];
    }
    TempArray[Index] = 0;

    //
    // Check for device number, if <= 9 then convert to ASCII and insert in array,
    // else split the number and convert to ASCII and then insert in array.  
    //
    if (DeviceNumber <= 9) {
        Appendarray1[1] = DeviceNumber + 0x30;
        
        for (Index = 0; Appendarray1[Index]; Index++) {
            Device->DevNameString[Index] = Appendarray1[Index];
        }
    } else {
        Index = (DeviceNumber / 10) + 0x30;
        IndexTemp = (DeviceNumber % 10) + 0x30;

        Appendarray2[1] = Index;
        Appendarray2[2] = IndexTemp;

        for (Index = 0; Appendarray2[Index] ; Index++) {
            Device->DevNameString[Index] = Appendarray2[Index];
        }
    }

    for (IndexTemp = 0; TempArray[IndexTemp]; IndexTemp++, Index++) {
         Device->DevNameString[Index] = TempArray[IndexTemp];
    }

    Device->DevNameString[Index] = 0;

    DeviceNumber++;
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

