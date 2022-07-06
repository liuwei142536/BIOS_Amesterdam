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

/** @file UsbMass.c
    AMI USB Mass Storage support implementation

**/


#include "AmiUsb.h"
#include "UsbMass.h"
#include <Library/BaseLib.h>
#include <Library/AmiUsbHcdLib.h>

extern  USB_GLOBAL_DATA *gUsbData;
extern  BOOLEAN          gCheckUsbApiParameter;
extern  USB_DATA_LIST   *gUsbDataList;
extern HC_STRUC         **gHcTable;


VOID        EFIAPI USBMassInitialize(VOID);
UINT8       EFIAPI USBMassCheckForStorageDevice(DEV_INFO*, UINT8, UINT8, UINT8);
DEV_INFO*   EFIAPI USBMassConfigureStorageDevice(HC_STRUC*, DEV_INFO*,
                                        UINT8*, UINT16, UINT16);
UINT8       EFIAPI USBMassDisconnectStorageDevice(DEV_INFO*);
UINT16      USBMassSendCBICommand(DEV_INFO*, MASS_XACT_STRUC*);
UINT32      USBMassProcessBulkData(DEV_INFO*, MASS_XACT_STRUC*);
UINT32      USBMassIssueBOTTransaction(DEV_INFO*, MASS_XACT_STRUC*);
VOID        USBMassClearBulkEndpointStall(DEV_INFO*, UINT8);
VOID        USBMassBOTResetRecovery(DEV_INFO*);
UINT16      USBMassSendBOTCommand(DEV_INFO*, MASS_XACT_STRUC*);
UINT8       USBMassGetBOTStatus(DEV_INFO*, MASS_XACT_STRUC*);
UINT16      USBMassCBIGetStatus(DEV_INFO*);
UINT32      USBMassIssueCBITransaction(DEV_INFO*, MASS_XACT_STRUC*);
UINT8       USBMassReadCapacity10Command(DEV_INFO*);
UINT32      USBMassCheckDeviceReady(DEV_INFO*);
UINT32      USBMassRequestSense(DEV_INFO* FpDevInfo);
VOID        USBMassSenseKeyParsing(DEV_INFO* , UINT32);
MASS_INQUIRY   *USBMassInquiryCommand(DEV_INFO*);
UINT8       USBMassUpdateDeviceGeometry( DEV_INFO* FpDevInfo );
UINT16      USBMassBOTGetMaxLUN(DEV_INFO*);
UINT8       USBMassIdentifyDeviceType(DEV_INFO*, UINT8*);
UINT32      USBMassIssueBulkTransfer(DEV_INFO*, UINT8, UINT8*, UINT32);
EFI_STATUS  iPodShufflePatch(MASS_GET_DEV_INFO*);
VOID        USBMassUpdateCylinderInfo(DEV_INFO*, UINT64);
UINT8       USBMassSetDefaultGeometry(DEV_INFO*, UINT64);
UINT8       USBMassValidatePartitionTable(MASTER_BOOT_RECORD*, UINT64, MBR_PARTITION*);
UINT16      USBMassSetDefaultType(DEV_INFO*, UINT64);
VOID        USBMassGetPhysicalDeviceType(DEV_INFO*, UINT8*);
UINT8       USB_SetAddress(HC_STRUC*, DEV_INFO*, UINT8);
UINT32      dabc_to_abcd(UINT32);
DEV_INFO*   USBGetProperDeviceInfoStructure(DEV_INFO*, UINT8);
UINT32      USBMassTestUnitReady(DEV_INFO*);
VOID        StoreUsbMassDeviceName(DEV_INFO*, UINT8*);
VOID EFIAPI AddPortNumbertoDeviceString(DEV_INFO*);
UINT8       USBMassGetConfiguration(DEV_INFO*);

VOID* USB_MemAlloc (UINT16);
DEV_INFO* EFIAPI USB_GetDeviceInfoStruc(UINT8, DEV_INFO*, UINT8, HC_STRUC*);
MASS_INQUIRY* USBMassGetDeviceParameters(DEV_INFO*);
ATA_IDENTIFY_DATA* USBMassGetIdentifyData(DEV_INFO*);
ATA_IDENTIFY_DATA* USBMassIdentifyCommand(DEV_INFO*);

UINT8 USB_MemFree  (VOID*,  UINT16);
VOID EFIAPI FixedDelay(UINTN);    
VOID EFIAPI SpeakerBeep (UINT8, UINT16, HC_STRUC*); 

static char* IOMegaZIPString        = "IOMEGA  ZIP";
#define IOMegaZIPStringLength       11

static char* MSysDiskOnKeyString    = "M-Sys   DiskOnKey";
#define MSysDiskOnKeyStringLength   17

BOOLEAN CheckDeviceLimit(UINT8);


/**
    This function fills the USB Mass storage device driver
    routine pointers

    @param FnDevDriver    Pointer to the device driver structure

    @retval None
**/
VOID
USBMassFillDriverEntries (DEV_DRIVER    *FnDevDriver)
{
    FnDevDriver->DevType               = BIOS_DEV_TYPE_STORAGE;
    FnDevDriver->Protocol              = 0;
    FnDevDriver->FnDeviceInit          = USBMassInitialize;
    FnDevDriver->FnCheckDeviceType     = USBMassCheckForStorageDevice;
    FnDevDriver->FnConfigureDevice     = USBMassConfigureStorageDevice;
    FnDevDriver->FnDisconnectDevice    = USBMassDisconnectStorageDevice;
}

/**
    This function initializes mass storage device related data.
    @param  None
    @retval None
**/
VOID
EFIAPI
USBMassInitialize (
  VOID        
)
{
    //
    // Set default value for the delay. Selections are: 20,40,60,80 for 10,20,30,40 sec.
    //
    gUsbData->UsbStorageDeviceDelayCount = (gUsbDataList->UsbSetupData->UsbMassResetDelay + 1)*10;

}

/**
    This routine checks for USB mass storage type device from the
    interface data provided

    @param DevInfo    Pointer to device information structure
    @param BaseClass  USB base class code
    @param SubClass   USB sub-class code
    @param Protocol   USB protocol code

    @retval BIOS_DEV_TYPE_STORAGE  USB storage type
    @retval Others                 Not USB storage type
**/

UINT8
EFIAPI
USBMassCheckForStorageDevice (
    DEV_INFO*   DevInfo,
    UINT8       BaseClass,
    UINT8       SubClass,
    UINT8       Protocol
)
{
    if (BaseClass != BASE_CLASS_MASS_STORAGE) {
        return USB_ERROR;
    }
                                                                                
    if (!gUsbDataList->UsbSetupData->UsbMassDriverSupport) {
        return USB_ERROR;
    }
    //                                                                 
    //Skip USB mass storage devices enumeration when legacy is disabled.
    //
    if (gUsbData->UsbStateFlag & USB_FLAG_DISABLE_LEGACY_SUPPORT) {
        if (((gUsbData->UsbFeature & USB_LEGACY_DISABLE_FOR_USB_MASS) == USB_LEGACY_DISABLE_FOR_USB_MASS)) {    
           return USB_ERROR;
        }
    }
    //
    // Base class is okay. Check the protocol field for supported protocols.
    // Currently we support CBI, CB and BOT protocols.
    //
    if ((Protocol != PROTOCOL_CBI) &&
        (Protocol != PROTOCOL_CBI_NO_INT) &&
        (Protocol != PROTOCOL_BOT)) {
        return USB_ERROR;
    }

    return BIOS_DEV_TYPE_STORAGE;
    
}

/**
    This function finds a free mass device info structure and
    returns the pointer to it

    @param DevInfo  Pointer to the Mass Device Info structure
    @param Index    USB Mass storage Device index
    @retval Dev     Pointer to the Mass Device Info (0 on failure)
                    The number mass storage DeviceInfo structure (0-based)

**/

DEV_INFO*
USBMassGetFreeMassDeviceInfoStruc(
    DEV_INFO    *DevInfo,
    UINT8       *Index
)
{
    DEV_INFO*   Dev = &gUsbDataList->DevInfoTable[1];
    UINT8       Count;
    UINT8       MassDevIndx = 0;
    
    for (Count = 0; Count < (gUsbData->MaxDevCount-1); Count++, Dev++) {
        if (!(Dev->Flag & DEV_INFO_VALID_STRUC)) {
             continue;
        }
        if (Dev->DeviceType == BIOS_DEV_TYPE_STORAGE) {
            MassDevIndx++;
        }
        if (Dev == DevInfo) {
            break;
        }
    }
    if (Count == (gUsbData->MaxDevCount-1)) {
        return NULL;
    }
    *Index = MassDevIndx;
    
    return Dev;
}


/**
    This function finds a free mass device info structure and
    copies the current mass device info structure into it

    @param Dev              Current mass device info structure
    @param EmulIndex        Emulation index
    @retval Dev             New mass device info

**/
DEV_INFO*
USBMassFindFreeMassDeviceInfo(
    DEV_INFO*   Dev,
    UINT8       *EmulIndex
)
{
    UINT8       Indx = 0;
    DEV_INFO    *NewDev;
    UINT8     MaxMassCount = 0;

    // Get the free mass device info structure pointer
    NewDev = USBMassGetFreeMassDeviceInfoStruc(Dev, &Indx);

    if (NewDev == NULL) {
        return NULL;   // No free entry found.
    }

    if(gUsbData != NULL){
      MaxMassCount  =  gUsbData->MaxMassCount;
    }

    // Get the emulation type setup question associated with this device
    ASSERT(Indx > 0 && Indx < (MaxMassCount + 1));
    if ((Indx == 0) || (Indx > MaxMassCount)) {
        return NULL;
    }

    Dev->EmulationOption = gUsbData->USBMassEmulationOptionTable[Indx-1];
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBMassFindFreeMassDeviceInfo-------- indx %d, emu %d\n", Indx, Dev->EmulationOption);

    // Set default device type and emulation type to 0
    Dev->StorageType = 0;
    Dev->Lun0DevInfoPtr = 0;
    Dev->Flag |= DEV_INFO_DEV_PRESENT;

    *EmulIndex = Indx-1;

    return Dev;
    
}

/**
    This function verifies the presence of logical units (LUN)
    in the USB mass device and creates appropriate device info
    structures for them

    @param DevInfo      Device information structure pointer
    @param MaxLun       Maximum number of logical units present (non-ZERO)
    @param EmulIndex    Emulation index

    @retval USB_ERROR   On error
    @retval USB_SUCCESS On successfully completion

**/
UINT8
USBMassCreateLogicalUnits(
    DEV_INFO*   DevInfo,
    UINT8       MaxLun,
    UINT8       EmulIndex
)
{
    UINT8           Lun;
    DEV_INFO*       NewDevInfo;
    MASS_INQUIRY    *Inq;

    for (Lun = 1; Lun <= MaxLun; Lun++) {

        if (CheckDeviceLimit(BASE_CLASS_MASS_STORAGE) == TRUE) {
            break;
        }
        //
        // Get the proper device info structure
        //
        NewDevInfo = USBGetProperDeviceInfoStructure(DevInfo, Lun);
        if (!NewDevInfo) {
            return USB_ERROR;
        }
        //
        // Check whether this device is reconnected by checking the
        // valid structure flag
        //
        if ((NewDevInfo->Flag & DEV_INFO_MASS_DEV_REGD)) {
            //
            // Indicate device as connected
            //
            NewDevInfo->Flag |= DEV_INFO_DEV_PRESENT;

            // Change the parent HC number and port number in the existing DEV_INFO
            NewDevInfo->HcNumber = DevInfo->HcNumber;
            NewDevInfo->HubDeviceNumber = DevInfo->HubDeviceNumber;
            NewDevInfo->HubPortNumber = DevInfo->HubPortNumber;
            NewDevInfo->EndpointSpeed = DevInfo->EndpointSpeed;
            NewDevInfo->Endp0MaxPacket = DevInfo->Endp0MaxPacket;
            NewDevInfo->DevMiscInfo = DevInfo->DevMiscInfo;
            NewDevInfo->DeviceAddress = DevInfo->DeviceAddress;
            NewDevInfo->BulkInEndpoint = DevInfo->BulkInEndpoint;
            NewDevInfo->BulkInMaxPkt = DevInfo->BulkInMaxPkt;
            NewDevInfo->BulkOutEndpoint = DevInfo->BulkOutEndpoint;
            NewDevInfo->BulkOutMaxPkt = DevInfo->BulkOutMaxPkt;
            NewDevInfo->IntInEndpoint = DevInfo->IntInEndpoint;
            NewDevInfo->IntInMaxPkt = DevInfo->IntInMaxPkt;
            NewDevInfo->PollInterval = DevInfo->PollInterval;
            NewDevInfo->Lun0DevInfoPtr = DevInfo;
        } else {    
            // This is different device, it was not reconnected
            //
            // Copy the old device info structure into the new one
            //
            CopyMem((UINT8*)NewDevInfo, (UINT8*)DevInfo, sizeof (DEV_INFO));
            NewDevInfo->Lun  = Lun; // Change LUN number
            NewDevInfo->EmulationOption = gUsbData->USBMassEmulationOptionTable[EmulIndex + Lun];
            ZeroMem(NewDevInfo->DevNameString, 64);
            //
            // Save the Lun0 device info pointer in the current LUN
            //
            NewDevInfo->Lun0DevInfoPtr  = DevInfo;

            //
            // The Lun0 device might have been already locked by the
            // bus (USBBUS.usbhc_on_timer), clear it for current LUN.
            //
            NewDevInfo->Flag &= ~DEV_INFO_DEV_BUS;
            
            Inq = USBMassGetDeviceParameters(NewDevInfo);
            if (Inq == NULL) return USB_ERROR;

            StoreUsbMassDeviceName(NewDevInfo, (UINT8*)Inq + 8);
            if (NewDevInfo->StorageType == USB_MASS_DEV_CDROM) {
                USBMassGetConfiguration(NewDevInfo);
            }
        }
    }
    return USB_SUCCESS;
}
/**
    This function stores USB mass storage name to DevNameString.

    @param DevInfo      Device information structure pointer
    @param Str          USB mass storage name

    @retval None
**/
VOID
StoreUsbMassDeviceName(
    DEV_INFO    *Device,
    UINT8       *Str
)
{
    UINT8   Index;
    UINT8   Count;
    UINT8   StrIndex;
    UINT8   StrIndexMax;

    for (Index = 0; Index < 64; Index++) {
        if (Device->DevNameString[Index] != 0) {
            return;
        }
    }

    for (Index = 0, Count = 0; Index < 32; Index++) {
        if (*Str == 0) {
            Str++; Count++;     // supress leading zeroes
        }
    }

    // INQUIRY Data Format
    // The Vendor Identification field contains 8 bytes of ASCII data identifying the vendor of the product.
    // The Product Identification field contains 16 bytes of ASCII data as defined by the vendor.
    // The Product Revision Level field contains 4 bytes of ASCII data as defined by the vendor.
    
    switch (gUsbData->MassStorageDeviceName) {
        // Vendor Information + Product Identification + Product Revision Level
        case 0:
            StrIndexMax = 28;
            break;
        // Vendor Information + Product Identification
        case 1:
            StrIndexMax = 24;
            break;
        // Vendor Information
        case 2:
            StrIndexMax = 8;
            break;
        default:
            StrIndexMax = 28;
            break;            
    }

    for (Index = 0, StrIndex = 0; (Index < (32 - Count)) && (StrIndex < StrIndexMax); Index++, StrIndex++) {
        // supress spaces if more than one
        if ((Index > 0) && (Device->DevNameString[Index-1] == ' ') && (Str[StrIndex] == ' ')) {
            Index--;
            continue;
        }

        // Filter out the character if it is invisible.
        if (((Str[StrIndex] != 0) && (Str[StrIndex] < 0x20)) || (Str[StrIndex] > 0x7E)) {
            Index--;
            continue;
        }
        Device->DevNameString[Index] = Str[StrIndex];
    }

    //
    // Add Device number to the USB device string
    //
    if ((gUsbData->UsbFeature & USB_DIFFERENTIATE_DEVICE_NAME) == USB_DIFFERENTIATE_DEVICE_NAME)
        AddPortNumbertoDeviceString(Device);

}

/**
    This function checks an interface descriptor of a device
    to see if it describes a USB mass device.  If the device
    is a mass storage device,  then it is configured
    and initialized.

    @param HcStruc     Pointer to the host controller structure
    @param FpDevInfo   Device information structure pointer
    @param FpDesc      Pointer to the descriptor structure
    @param StartOffset Start offset of the device descriptor
    @param EndOffset    End offset of the device descriptor

    @retval New device info structure, NULL on error

**/

DEV_INFO*
EFIAPI
USBMassConfigureStorageDevice (
    HC_STRUC*   HcStruc,
    DEV_INFO*   FpDevInfo,
    UINT8*      FpDesc,
    UINT16      StartOffset,
    UINT16      EndOffset
    )
{
    UINT8           TempByte;
    UINT16          RetValue;
    ENDP_DESC       *FpEndpDesc;
    INTRF_DESC      *FpIntrfDesc;
    UINT8           MaxLUN;
    DEV_INFO*       NewDev;
    MASS_INQUIRY    *Inq;
    BOOLEAN         CheckFddHotPlug, CheckCdromHotPlug, CheckHddHotPlug;
    UINT8           EmulIndex;
    UINT8           Index;

    RetValue       = 0;
    MaxLUN         = 0;
    EmulIndex      = 0;

//
// Set FpDevInfo->DeviceType.  This serves as a flag
// that indicates a usable interface has been found in the current
// configuration. This is needed so we can check for other usable interfaces
// in the current configuration (composite device) without trying to search
// in other configurations.
//
    FpDevInfo->DeviceType      = BIOS_DEV_TYPE_STORAGE;
    FpDevInfo->PollTdPtr       = 0;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBMassConfigureDevice ....\n");

    TempByte = 0x03;       // bit 1 = Bulk In, bit 0 = Bulk Out

    FpDevInfo->BulkOutEndpoint = 0;
    FpDevInfo->BulkInEndpoint = 0;
    FpDevInfo->IntInEndpoint  = 0;

    FpIntrfDesc = (INTRF_DESC*)(FpDesc + StartOffset);
    FpDesc+=((CNFG_DESC*)FpDesc)->TotalLength; // Calculate the end of descriptor block
    FpEndpDesc = (ENDP_DESC*)((char*)FpIntrfDesc + FpIntrfDesc->DescLength);
    for( ;(FpEndpDesc->DescType != DESC_TYPE_INTERFACE) && ((UINT8*)FpEndpDesc < FpDesc);
        FpEndpDesc = (ENDP_DESC*)((char*)FpEndpDesc + FpEndpDesc->DescLength)){

        if(!(FpEndpDesc->DescLength)) {
            break;  // Break if 0 length desc (should never happen, but...)
        }

        if( FpEndpDesc->DescType != DESC_TYPE_ENDPOINT ) {
            continue;
        }

        if ((FpEndpDesc->EndpointFlags & EP_DESC_FLAG_TYPE_BITS) ==
                EP_DESC_FLAG_TYPE_BULK) {   // Bit 1-0: 10 = Endpoint does bulk transfers
            if(!(FpEndpDesc->EndpointAddr & EP_DESC_ADDR_DIR_BIT)) {
                //
                // Bit 7: Dir. of the endpoint: 1/0 = In/Out
                // If Bulk-Out endpoint already found then skip subsequent ones
                // on the interface.
                //
                if (TempByte & 1) {
                    FpDevInfo->BulkOutEndpoint = (UINT8)(FpEndpDesc->EndpointAddr
                                                        & EP_DESC_ADDR_EP_NUM);
                    FpDevInfo->BulkOutMaxPkt = FpEndpDesc->MaxPacketSize;
                    TempByte &= 0xFE;
                    USB_DEBUG(DEBUG_INFO, 3, "bulk out endpoint addr: %x, max packet size: %x\n", 
                        FpDevInfo->BulkOutEndpoint, FpDevInfo->BulkOutMaxPkt);
                }
            } else {
                //
                // If Bulk-In endpoint already found then skip subsequent ones
                // on the interface
                //
                if (TempByte & 2) {
                    FpDevInfo->BulkInEndpoint  = (UINT8)(FpEndpDesc->EndpointAddr
                                                        & EP_DESC_ADDR_EP_NUM);
                    FpDevInfo->BulkInMaxPkt    = FpEndpDesc->MaxPacketSize;
                    TempByte   &= 0xFD;
                    USB_DEBUG(DEBUG_INFO, 3, "bulk in endpoint addr: %x, max packet size: %x\n", 
                        FpDevInfo->BulkInEndpoint, FpDevInfo->BulkInMaxPkt);
                }
            }
        }

        //
        // Check for and configure Interrupt endpoint if present
        //
        if ((FpEndpDesc->EndpointFlags & EP_DESC_FLAG_TYPE_BITS) !=
                EP_DESC_FLAG_TYPE_INT) {    // Bit 1-0: 11 = Endpoint does interrupt transfers
            continue;
        }

        if (FpEndpDesc->EndpointAddr & EP_DESC_ADDR_DIR_BIT ) {
            FpDevInfo->IntInEndpoint = FpEndpDesc->EndpointAddr;
            FpDevInfo->IntInMaxPkt = FpEndpDesc->MaxPacketSize;
            FpDevInfo->PollInterval = FpEndpDesc->PollInterval;  
            USB_DEBUG(DEBUG_INFO, 3, "interrupt in endpoint addr: %x, max packet size: %x\n", 
                FpDevInfo->IntInEndpoint, FpDevInfo->IntInMaxPkt);
        }
    }

    //
    // Check the compatibility flag for LUN support
    //
    if (!(FpDevInfo->IncompatFlags & USB_INCMPT_SINGLE_LUN_DEVICE)) {
        //
        // If it is a BOT device, get maximum LUN supported
        //
        if (FpDevInfo->Protocol == PROTOCOL_BOT) {
            MaxLUN = (UINT8)USBMassBOTGetMaxLUN(FpDevInfo);
        }
    }

    //
    // Check whether the device is already registered. If so, proceed with current
    // mass info structure
    //
    if (FpDevInfo->Flag & DEV_INFO_MASS_DEV_REGD) {
        NewDev = FpDevInfo;
        goto UMCM_MassDeviceOkay;
    }

    // Find a new mass device info structure and copy the old one into the new one
    // Note: this is called before GetDeviceParameters because it sets up dev->EmulationOption
    NewDev = USBMassFindFreeMassDeviceInfo(FpDevInfo, &EmulIndex);

    if (NewDev == NULL) goto UMCM_Error;
        FpDevInfo = NewDev;

    Inq = USBMassGetDeviceParameters(FpDevInfo);
    if (Inq == NULL) {
        FpDevInfo->DeviceType = 0;
        goto UMCM_Error;
    }

    //
    // Do not enumerate device if it is not a CD-ROM and has the block size different from 512 Bytes
    // EIP#15595, iPod nano makes POST hang.
    //
    //if ( FpDevInfo->PhyDevType!=USB_MASS_DEV_CDROM ) {
    //    if( FpDevInfo->BlockSize!=0x200 && FpDevInfo->BlockSize!=0xFFFF && FpDevInfo->BlockSize!=0 )
    //        goto UMCM_Error;
    //}

    StoreUsbMassDeviceName(FpDevInfo, (UINT8*)Inq+8);

    // Check for the hotplug devices current status, install the new one if needed
    if ( !(gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) ) {
        // Find out if FDD/HDD/CDROM hotplugging is a valid option
        CheckFddHotPlug = ((gUsbDataList->UsbSetupData->UsbHotplugFddSupport == SETUP_DATA_HOTPLUG_ENABLED) ||
                          ((gUsbDataList->UsbSetupData->UsbHotplugFddSupport == SETUP_DATA_HOTPLUG_AUTO) &&
                           (gUsbData->NumberOfFDDs == 0))) &&
                           !(BOOLEAN)(gUsbData->UsbStateFlag & USB_HOTPLUG_FDD_ENABLED);

        CheckHddHotPlug = ((gUsbDataList->UsbSetupData->UsbHotplugHddSupport == SETUP_DATA_HOTPLUG_ENABLED) ||
                          ((gUsbDataList->UsbSetupData->UsbHotplugHddSupport == SETUP_DATA_HOTPLUG_AUTO) &&
                           (gUsbData->NumberOfHDDs == 0))) &&
                            !(BOOLEAN)(gUsbData->UsbStateFlag & USB_HOTPLUG_HDD_ENABLED);

        CheckCdromHotPlug = ((gUsbDataList->UsbSetupData->UsbHotplugCdromSupport == SETUP_DATA_HOTPLUG_ENABLED) ||
                            ((gUsbDataList->UsbSetupData->UsbHotplugCdromSupport == SETUP_DATA_HOTPLUG_AUTO) &&
                             (gUsbData->NumberOfCDROMs == 0))) &&
                              !(BOOLEAN)(gUsbData->UsbStateFlag & USB_HOTPLUG_CDROM_ENABLED);

        if ( CheckFddHotPlug || CheckCdromHotPlug || CheckHddHotPlug ) {
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "connecting hotplug...");
//          Inq = USBMassGetDeviceParameters(FpDevInfo);
//          if (Inq == NULL) goto UMCM_Error;
            USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "devtype phy %d, emu %d...", FpDevInfo->PhyDevType, FpDevInfo->EmuType);

            if ( CheckFddHotPlug &&
                (FpDevInfo->StorageType == USB_MASS_DEV_ARMD) ) {
                NewDev = (DEV_INFO*)(VOID*)&gUsbData->FddHotplugDev;
                gUsbData->UsbStateFlag |= USB_HOTPLUG_FDD_ENABLED;
            }

            if ( CheckHddHotPlug && (FpDevInfo->EmuType == USB_EMU_HDD_ONLY) ) {
                NewDev = (DEV_INFO*)(VOID*)&gUsbData->HddHotplugDev;
                gUsbData->UsbStateFlag |= USB_HOTPLUG_HDD_ENABLED;
            }

            if ( CheckCdromHotPlug && (FpDevInfo->PhyDevType == USB_MASS_DEV_CDROM) ) {
                NewDev = (DEV_INFO*)(VOID*)&gUsbData->CdromHotplugDev;
                gUsbData->UsbStateFlag |= USB_HOTPLUG_CDROM_ENABLED;
            }

            FpDevInfo->Flag |= DEV_INFO_HOTPLUG;
            //*newDev = *FpDevInfo;   // Copy device into DevInfo dedicated to hotplug
            CopyMem(NewDev, FpDevInfo, sizeof(DEV_INFO));
            FpDevInfo->Flag &= ~DEV_INFO_VALIDPRESENT; // Release FpDevInfo
            FpDevInfo = NewDev;
        }
    }

UMCM_MassDeviceOkay:
    if ( (NewDev->EmuType == USB_EMU_FLOPPY_ONLY) ||
        (NewDev->EmuType == USB_EMU_FORCED_FDD) ) {
        gUsbData->NumberOfFDDs++;
    }

    if ( NewDev->EmuType == USB_EMU_HDD_ONLY ) {
        gUsbData->NumberOfHDDs++;
    }

//    if ( newDev->PhyDevType == USB_EMU_HDD_OR_FDD ) {
    if ( NewDev->PhyDevType == USB_MASS_DEV_CDROM ) {
        gUsbData->NumberOfCDROMs++;
        USBMassGetConfiguration(NewDev);
    }

    if (MaxLUN) {
        USBMassCreateLogicalUnits(NewDev, MaxLUN, EmulIndex);
    }


    if (gUsbData->UsbStateFlag & USB_FLAG_SKIP_CARD_READER_CONNECT_BEEP) {        
        if ((NewDev->LastStatus & USB_MASS_MEDIA_PRESENT) ||
            NewDev->PhyDevType == USB_MASS_DEV_CDROM ||
            NewDev->PhyDevType == USB_MASS_DEV_FDD) {
            SpeakerBeep(4, 0x1000, HcStruc);
        } else if (MaxLUN) {
            for(Index = 1; Index < gUsbData->MaxDevCount; Index++) {
                if (gUsbDataList->DevInfoTable[Index].Lun0DevInfoPtr == NewDev) {
                    if (gUsbDataList->DevInfoTable[Index].LastStatus & USB_MASS_MEDIA_PRESENT) {
                        SpeakerBeep(4, 0x1000, HcStruc);
                        break;
                    }
                }
            }
        }
    }


    return  NewDev;

UMCM_Error:
    return NULL;
}


/**
    This function disconnects the storage device

    @param DevInfo        Device info structure pointer
    @retval USB_SUCCESS   Sucess to disconnect the usb storage device
    @retval Others        On fail
**/

UINT8
EFIAPI
USBMassDisconnectStorageDevice (
    DEV_INFO* DevInfo
)
{
//  USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBMassDisconnectDevice ....  \n");

    DevInfo->BulkOutEndpoint = 0;
    DevInfo->BulkInEndpoint  = 0;
    DevInfo->IntInEndpoint   = 0;

    if ((DevInfo->EmuType == USB_EMU_FLOPPY_ONLY) ||
        (DevInfo->EmuType == USB_EMU_FORCED_FDD)) {
        gUsbData->NumberOfFDDs--;
    }

    if (DevInfo->EmuType == USB_EMU_HDD_ONLY) {
        gUsbData->NumberOfHDDs--;
    }

//    if ( newDev->PhyDevType == USB_EMU_HDD_OR_FDD ) {
    if ( DevInfo->PhyDevType == USB_MASS_DEV_CDROM ) {
        gUsbData->NumberOfCDROMs--;
    }

    if (DevInfo->Flag & DEV_INFO_HOTPLUG) {
        DevInfo->Flag &= ~DEV_INFO_HOTPLUG;
        if (DevInfo == (DEV_INFO*)(VOID*)&gUsbData->FddHotplugDev) {
            gUsbData->UsbStateFlag &= ~USB_HOTPLUG_FDD_ENABLED;
        } else if (DevInfo == (DEV_INFO*)(VOID*)&gUsbData->HddHotplugDev) {
            gUsbData->UsbStateFlag &= ~USB_HOTPLUG_HDD_ENABLED;
        } else if (DevInfo == (DEV_INFO*)(VOID*)&gUsbData->CdromHotplugDev) {
            gUsbData->UsbStateFlag &= ~USB_HOTPLUG_CDROM_ENABLED;
        }
    }

    return USB_SUCCESS;
}

/**
    This function clears the mass transaction structure
    @param MassXactStruc   the mass transaction structure pointer
    @retval None
**/

VOID
USBMassClearMassXactStruc(
    MASS_XACT_STRUC *MassXactStruc
)
{
    UINT8   Count;
    UINT8*  Cleaner = (UINT8*)MassXactStruc;
    
    for (Count = 0; Count < sizeof (MASS_XACT_STRUC); Count++ ) {
        *Cleaner++ = 0;
    }
}


/**
    This function clears the bulk endpoint stall by sending
    CLEAR_FEATURE command to bulk endpoints

    @param DevInfo   Pointer to DeviceInfo structure
    @param Direc     Endpoint direction
    @retval VOID

**/

VOID
USBMassClearBulkEndpointStall(
    DEV_INFO*   DevInfo,
    UINT8       Direc
)
{
    UINT8           Shift;
    UINT16          EndPoint;
    HC_STRUC        *HcStruc;

    EFI_STATUS      Status;

    HcStruc = gHcTable[DevInfo->HcNumber - 1];
    EndPoint = (UINT16)((DevInfo->BulkInEndpoint) | BIT7);

    if (!(Direc & BIT7)) {
        EndPoint = DevInfo->BulkOutEndpoint;
    }
    //
    // Issue clear port feature command
    //
    AmiUsbControlTransfer(HcStruc, DevInfo, (UINT16)ENDPOINT_CLEAR_PORT_FEATURE, 
                          EndPoint,(UINT16)ENDPOINT_HALT, 0, 0);       
    //
    // Reset the toggle bit
    //
    Shift = (EndPoint & 0xF) - 1;

    if (Direc & BIT7) {
        DevInfo->DataInSync &= ~((UINT16)(1 << Shift));
    } else {
        DevInfo->DataOutSync &= ~((UINT16)(1 << Shift));
    }

    if (DevInfo->Lun0DevInfoPtr == NULL) {
        return;
    }
    
    Status = UsbDevInfoValidation(DevInfo->Lun0DevInfoPtr);
    
    if (EFI_ERROR(Status)) {
        return;
    }

    if (Direc & BIT7) {
        DevInfo->Lun0DevInfoPtr->DataInSync &= ~((UINT16)(1 << Shift));
    } else {
        DevInfo->Lun0DevInfoPtr->DataOutSync &= ~((UINT16)(1 << Shift));
    }


}

/**
    This function performs a mass storage transaction by
    invoking proper transaction protocol.

    @param Pointer to DeviceInfo structure
    @param Pointer to the mass storage transaction structure
    @retval DataLength Amount of data actually transferred

**/
UINT32
USBMassIssueMassTransaction(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{
    UINT32      DataLength;
    HC_STRUC    *HcStruc;
    
    if ((DevInfo->Protocol == PROTOCOL_CBI) ||
        (DevInfo->Protocol == PROTOCOL_CBI_NO_INT)) {
        return USBMassIssueCBITransaction(DevInfo, MassXactStruc);
    }

    if (DevInfo->Protocol == PROTOCOL_BOT) {
        
        // Block to process periodic list to prevent that we might send the wrong
        // command sequences to the same device.
        gUsbData->ProcessingPeriodicList = FALSE;
        
        DataLength = USBMassIssueBOTTransaction(DevInfo, MassXactStruc);
        
        // To process any pending periodic list.
        gUsbData->ProcessingPeriodicList = TRUE;
        HcStruc = gHcTable[DevInfo->HcNumber - 1];
        AmiUsbProcessInterrupt(HcStruc);
        
        return DataLength;
    }

    return 0;

}

/**
    This function gets the USB mass device parameters such as
    max cylinder, head, sector, block size and

    @param Pointer to DeviceInfo structure
    @retval Inq     Mass storage inquiry data
    @retval Others  fail to get the USB mass device parameters

**/

MASS_INQUIRY*
USBMassGetDeviceParameters(
    DEV_INFO* DevInfo
)
{
    MASS_INQUIRY    *Inq;
    UINT8           Retry;
    UINT8           Status;

    for (Retry = 0; Retry < 2; Retry++) {
        Inq = USBMassInquiryCommand(DevInfo);
        if (Inq) {
            break;
        }
        if (!(gUsbData->UsbLastCommandStatus & USB_BULK_STALLED)) {
            break;
        }
    }
    
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "fpMassInquiry = %x\n", Inq);
    
    if (!Inq) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the inquiry data\n");
        return NULL;
    }

    DevInfo->BlockSize = 0xFFFF; // Clear the cached block size

    //
    // Find the device type and update the device type structure accordingly
    //
    Status = USBMassIdentifyDeviceType(DevInfo, (UINT8*)Inq);

    if (Status == USB_ERROR) {
        Inq = NULL;
    }
    
    return Inq;
    
}

/**
    This function get identify data from the USB mass storage
    device

    @param  DevInfo   Pointer to DeviceInfo structure

    @retval IdentifyData Pointer to the identify data
    @retval Others       fail to get identify data
**/
ATA_IDENTIFY_DATA*
USBMassGetIdentifyData(
    DEV_INFO* DevInfo  
)
{
    ATA_IDENTIFY_DATA *IdentifyData;
    IdentifyData = USBMassIdentifyCommand(DevInfo);
    
    if(IdentifyData) {
        return IdentifyData;
    } else {
        return NULL;
    }
}

/**
    This function sends identify command to the USB mass storage
    device

    @param  DevInfo   Pointer to DeviceInfo structure

    @retval UsbTempBuffer Pointer to the identify data
    @retval Others        fail to get identify data
**/

ATA_IDENTIFY_DATA*
USBMassIdentifyCommand(
    DEV_INFO* DevInfo
)
{
    IDENTIFY_DEVICE_PACKET_CMD *CmdBuffer;
    UINT32 Data;
    MASS_XACT_STRUC MassXactStruc;
    EFI_STATUS      Status;

    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(IDENTIFY_DEVICE_PACKET_CMD));
    if(!CmdBuffer) {
        return NULL;
    }

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)gUsbDataList->UsbTempBuffer, sizeof(ATA_IDENTIFY_DATA));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return NULL;
        }
        gCheckUsbApiParameter = FALSE;
    }

    CmdBuffer->OpCode = ATA_PASS_THROUGH_16_OPCODE;
    CmdBuffer->Protocol = ATA_PROTOCOL_DEVICE_RESET;
    CmdBuffer->TLength = ATA_TRANSFER_LENGTH;
    CmdBuffer->SectorCount = ATA_SECTOR_COUNT;
    CmdBuffer->Feature = 0x0000;
    CmdBuffer->LbaLow = 0x0000;
    CmdBuffer->LbaMid = 0x0000;
    CmdBuffer->LbaHigh = 0x0000;
    CmdBuffer->Command = ATA_IDENTIFY_DEVICE_DATA;
    CmdBuffer->Device = 0x00;

    USBMassClearMassXactStruc(&MassXactStruc);

    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
    MassXactStruc.CmdSize = sizeof (IDENTIFY_DEVICE_PACKET_CMD);
    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = gUsbDataList->UsbTempBuffer;
    MassXactStruc.Length = sizeof(ATA_IDENTIFY_DATA);

    if(MassXactStruc.Length  > MAX_TEMP_BUFFER_SIZE){
      USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "UsbMass:ERROR!USBMassIdentifyCommand buffer is to small!\n");  
      USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(IDENTIFY_DEVICE_PACKET_CMD)); 
      return NULL;
    }
    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);
    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(IDENTIFY_DEVICE_PACKET_CMD));

    if(Data) {
        return (ATA_IDENTIFY_DATA*)gUsbDataList->UsbTempBuffer;
    } else {   
        return NULL;
    }
}

/**
    This procedure check whether device return valid device name
    if no valid device name returned, assign default name for it

    @param  InqData  Inquiry Data
    @retval None
**/
VOID ValidateDeviceName (
    MASS_INQUIRY *InqData
)
{
    static UINT8 DefaultName[] = "USB     Storage Device";
    UINT8 *Name = ((UINT8*)InqData) + 8;
    UINT8 *DefName = DefaultName;
    UINT8 Count;

    // check for a blank name
    if (*Name) return;

//  for (Count = 0; Count < 28; Count++) {
//      if (*(Name + Count)) return;  // Not blank
//  }

    // copy default name
    for (Count = 0; Count < sizeof(DefaultName); Count++) {
        *(Name + Count) = *(DefName + Count);
    }
}

/**
    This function fills and returns the mass get device info
    structure

    @param MassGetDevInfo    Pointer to the mass get info struc
                              DevAddr-USB device address of the device
                              SenseData-Sense data of the last command
                              DevType-Device type byte (HDD, CD, Removable)
                              EmuType-Emulation type used
                              DevId-Far pointer to the device ID
    @retval USB_SUCCESS      Success to get device information.
    @retval Others           Fail to get device information.

    @note  Initially the DevAddr should be set to 0 as input. This
          function returns the information regarding the first mass
          storage device (if no device found it returns DevAddr as
          0FFh) and also updates DevAddr to the device address of
          the current mass storage device. If no other mass storage
          device is found then the routine sets the bit7 to 1
          indicating current information is valid but no more mass
          device found in the system. The caller can get the next
          device info if DevAddr is not 0FFh and bit7 is not set

**/

UINT8
USBMassGetDeviceInfo (
    MASS_GET_DEV_INFO *MassGetDevInfo
)
{
    DEV_INFO        *DevInfo;
    MASS_INQUIRY    *MassInq;
    UINT8           Dev = MassGetDevInfo->DevAddr;

    //
    // Get the total number of Mass Storage Devices
    //
    MassGetDevInfo->TotalMassDev = (UINT8)(UINTN)USB_GetDeviceInfoStruc(USB_SRCH_DEV_NUM,
                            0, BIOS_DEV_TYPE_STORAGE, 0);

    if (Dev == 0) {
        iPodShufflePatch(MassGetDevInfo);
    }

    if (Dev & BIT7) {
        return USB_ERROR;  // Check for device address validity
    }

    //
    // If bDev = 0 then get information about first mass storage device
    //
    if (!Dev) {
        DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_TYPE, 0, BIOS_DEV_TYPE_STORAGE, 0);
        //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "Get Mass0 info: %x\n", DevInfo);

        if (!DevInfo) {   // Set as no more device found
            MassGetDevInfo->DevAddr  = 0xFF;
            return USB_SUCCESS;
        }
    } else {  //  Not the first mass device
        //
        // Get the device info structure for the matching device index
        //
        DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, Dev, 0);
        ASSERT(DevInfo);
        if ( (!DevInfo) || (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT)) ) {   // Error
            return USB_ERROR;
        }
        //
        // Get device info structure for next device
        //
        DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_TYPE, DevInfo, BIOS_DEV_TYPE_STORAGE, 0);
        ASSERT(DevInfo);
        if (!DevInfo) {   // Error. Exit !
            return USB_ERROR;
        }
    }
    MassInq = USBMassGetDeviceParameters(DevInfo);

    if (!MassInq) {
        return USB_ERROR;
    }

    MassGetDevInfo->DevType = DevInfo->PhyDevType;
//  MassGetDevInfo->PhyDevType = FpDevInfo->PhyDevType;
    MassGetDevInfo->EmuType = DevInfo->EmuType;
    MassGetDevInfo->PciInfo = gHcTable[DevInfo->HcNumber - 1]->BusDevFuncNum;
    MassGetDevInfo->DevId = (UINT32)(UINTN)((UINT8*)MassInq + 8);
//  MassGetDevInfo->DevId = USBMassAdjustIdString((UINT32)MassInq + 8);

    Dev = (UINT8)(UINTN)USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, DevInfo, 0, 0);
    ASSERT(Dev);

    Dev |= BIT7;   // Assume that no more mass device present

    //
    // Check whether more mass device is present
    //
    if (USB_GetDeviceInfoStruc(USB_SRCH_DEV_TYPE, DevInfo, BIOS_DEV_TYPE_STORAGE, 0)) {
        Dev &= ~BIT7;
    }

    DevInfo->Flag |= DEV_INFO_MASS_DEV_REGD;
    MassGetDevInfo->DevAddr = Dev;

    *(UINTN*)MassGetDevInfo->Handle = *(UINTN*)DevInfo->Handle;

    return USB_SUCCESS;
}

/**
    This check whether iPod shuffle attached to system and move
    iPod shuffle to first initial device.

    @param MassGetDevInfo Pointer to the mass get info structure

    @retval VOID

    @note  Attaching iPod shuffle and iPod mini to system causes BIOS POST
          stop. iPod shuffle must be initialized as early as possible.
          iPod mini cosumes about 2 seconds to complete initialization,
          init iPod shuffle first to fix problem.
**/

EFI_STATUS
iPodShufflePatch(
    MASS_GET_DEV_INFO *MassGetDevInfo
)
{
    return EFI_UNSUPPORTED;
}


/**
    Get USB MassStorage device status. Include Media Informarion.
 
    @param MassGetDevSts Pointer to Device status structure
    @retval USB_SUCCESS  On success.
    @retval Others       On fail.

**/

UINT8
USBMassGetDeviceStatus  (
    MASS_GET_DEV_STATUS *MassGetDevSts
)
{
    DEV_INFO*   DevInfo;
    UINT8       DevAddr = MassGetDevSts->DevAddr;
    UINT8       LastStatus;

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);

    ASSERT(DevInfo != NULL);
    if (DevInfo == NULL) {
        return USB_ERROR;
    }

    LastStatus= DevInfo->LastStatus;

    USBMassCheckDeviceReady(DevInfo);

    // When the Media is not present in the drive and insert the Media
    // it's just sends the status as Media Present. So check the last status
    // and if the media not present and current stauts is media present
    // report it as Media changed
    if ((LastStatus ^ DevInfo->LastStatus) & USB_MASS_MEDIA_PRESENT) {
        // Report the Last Status along with Media Changed status
        DevInfo->LastStatus |= USB_MASS_MEDIA_CHANGED;
    }

    MassGetDevSts->DeviceStatus = DevInfo->LastStatus;

    if (DevInfo->LastStatus & USB_MASS_MEDIA_CHANGED) {
        //
        // Clear Media Change Status.
        //
        DevInfo->LastStatus &= (UINT8)(~USB_MASS_MEDIA_CHANGED);
    }
    return USB_SUCCESS;
}

/**
    This function issues the command/data sequence provided
    as input.  This function can be used to send raw data
    to the USB mass storage device


    @param MassCmdPassThru   Pointer to the mass command pass through structure.
                              DevAddr-USB device address of the device
                              SenseData-Sense data of the last command
                              CmdBuffer-Far pointer to the command buffer
                              CmdLength-Command length
                              DataBuffer-Far pointer for data buffer
                              DataLength-Data length
                              XferDir-Data transfer direction
                              
    @retval USB_SUCCESS  On success.
                         Update DataLength and SenseData.
    @retval Others       On fail.    

**/

UINT8
USBMassCmdPassThru (
    MASS_CMD_PASS_THRU  *MassCmdPassThru
)
{
    UINT8           *CmdBuffer;
    UINT8           *Src;
    UINT8           *Dst;
    DEV_INFO        *DevInfo;
    UINT8           CmdBlkSize;
    UINT8           Count;
    UINT16          Data16;
    UINT32          Data32;
    UINT8           DevAddr = MassCmdPassThru->DevAddr;
    MASS_XACT_STRUC MassXactStruc;

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);
    if ( (!DevInfo) || (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT)) ) {   // Error
        return USB_ERROR;
    }

    CmdBlkSize = (UINT8)((MassCmdPassThru->CmdLength +
                USB_MEM_BLK_SIZE - 1) >> USB_MEM_BLK_SIZE_SHIFT);

    //
    // Check whether the drive is ready for read TOC command
    //
    USBMassCheckDeviceReady(DevInfo);

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc((UINT8)GET_MEM_BLK_COUNT(CmdBlkSize));
    if (!CmdBuffer) {
        return USB_ERROR;
    }

    //
    // Copy the command into (just allocated) mass command buffer
    //
    Src = (UINT8*)(UINTN)MassCmdPassThru->CmdBuffer;
    Dst = CmdBuffer;
    for (Count = 0; Count < MassCmdPassThru->CmdLength; Count++) {
        *Dst++ = *Src++;
    }

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = CmdBuffer;
    MassXactStruc.CmdSize = (UINT8)MassCmdPassThru->CmdLength;
    MassXactStruc.XferDir = MassCmdPassThru->XferDir;
    MassXactStruc.FpBuffer = (UINT8*)(UINTN)MassCmdPassThru->DataBuffer;
    MassXactStruc.Length = (UINT32)MassCmdPassThru->DataLength;

    Data16 = (UINT16)USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    //
    // Update the actual data length processed/returned
    //
    MassCmdPassThru->DataLength = Data16;

    Data32 = USBMassRequestSense (DevInfo);

    MassCmdPassThru->SenseData = Data32;

    //
    // Check and free command buffer
    //

    USB_MemFree(CmdBuffer, (UINT16)GET_MEM_BLK_COUNT(CmdBlkSize));

    return USB_SUCCESS;

}

/**
    This function issues read capacity of the mass storage

    @param DevInfo Pointer to DeviceInfo structure

    @retval USB_SUCCESS  Finish to read capacity.
    @retval Others       On fail

    @note  This routine will update the MassDeviceInfo structure
           with the block size & last LBA values obtained from the
           device
**/
UINT8
USBMassReadCapacity16Command  (
    DEV_INFO* DevInfo
)
{
    UINT32                          Data;
    COMN_READ_CAPACITY_16_CMD       *CmdBuffer;
    MASS_XACT_STRUC                 MassXactStruc;
    
    if (!VALID_DEVINFO(DevInfo)) {
        return USB_ERROR;
    }
    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_16_CMD));
    
    if (!CmdBuffer) {
        return USB_ERROR;
    }
    
    CmdBuffer->OpCode = COMMON_READ_CAPACITY_16_OPCODE;
    CmdBuffer->ServiceAction = 0x10;
    CmdBuffer->AllocLength = 0x0C000000;

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);

    //
    // Change the bulk transfer delay to 10 seconds (For CDROM drives)
    //
    gUsbData->UsbBulkDataXferDelay = 10000;

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
    MassXactStruc.CmdSize   = sizeof(COMN_RWV_16_CMD);
    MassXactStruc.XferDir   = BIT7;     // IN
    MassXactStruc.FpBuffer  = gUsbDataList->UsbTempBuffer;
    MassXactStruc.Length    = 0xC;
	
    if (MassXactStruc.Length  > MAX_TEMP_BUFFER_SIZE){
      USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "UsbMass:ERROR!USBMassReadCapacity16Command buffer is to small!\n"); 
      USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_16_CMD));  
      return USB_ERROR;
    }
    
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "rcc..");
    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    //
    // Reset the delay back
    //
    gUsbData->UsbBulkDataXferDelay = 0;

    if (!Data) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_16_CMD));
        
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "err ");
        return  USB_ERROR;
    }

    Data = *((UINT32*)(gUsbDataList->UsbTempBuffer + 8));

    //
    // Change little endian format to big endian(INTEL) format
    //
    Data = dabc_to_abcd(Data);
                                                                                        //(EIP37167+)>
    if (!Data) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_16_CMD));
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "err ");
        return USB_ERROR;
    }

    DevInfo->BlockSize = (UINT16)Data;

    USB_DEBUG(DEBUG_INFO, 3,"BlockSize %x\n", DevInfo->BlockSize);

    //
    // Store the last LBA number in the mass info structure
    //
    Data = *((UINT32*)(gUsbDataList->UsbTempBuffer));

    Data = dabc_to_abcd(Data);

    DevInfo->MaxLba = LShiftU64(Data, 32);

    Data = *((UINT32*)(gUsbDataList->UsbTempBuffer + 4));

    Data = dabc_to_abcd(Data);

    DevInfo->MaxLba |= Data;

    USB_DEBUG(DEBUG_INFO, 3,"MaxLba %lx\n", DevInfo->MaxLba);

    return USB_SUCCESS;

}

/**
    This function issues read capacity of the mass storage

    @param DevInfo Pointer to DeviceInfo structure

    @retval USB_SUCCESS  Finish to read capacity.
    @retval Others       On fail

    @note  This routine will update the MassDeviceInfo structure
          with the block size & last LBA values obtained from the device
**/
UINT8
USBMassReadCapacity10Command  (
    DEV_INFO* DevInfo
)
{
    UINT32                          Data;
    COMN_READ_CAPACITY_10_CMD       *CmdBuffer;
    MASS_XACT_STRUC                 MassXactStruc;

    if (!VALID_DEVINFO(DevInfo)) {
        return USB_ERROR;
    }
    
    if (RShiftU64(DevInfo->MaxLba, 32)) {
        USBMassReadCapacity16Command(DevInfo);
        return USB_SUCCESS;
    }
    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));

    if (!CmdBuffer) {
        return USB_ERROR;
    }

    CmdBuffer->bOpCode = COMMON_READ_CAPACITY_10_OPCODE;

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);

    //
    // Change the bulk transfer delay to 10 seconds (For CDROM drives)
    //
    gUsbData->UsbBulkDataXferDelay = 10000;

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;

    if (DevInfo->SubClass == SUB_CLASS_SCSI) {
        MassXactStruc.CmdSize = 0x0A;    //SBC-3_66
    } else {
        MassXactStruc.CmdSize = sizeof (COMN_READ_CAPACITY_10_CMD);
    }

    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = gUsbDataList->UsbTempBuffer;
    MassXactStruc.Length = 8;

    if(MassXactStruc.Length  > MAX_TEMP_BUFFER_SIZE){
      USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "UsbMass:ERROR!buffer is to small!\n");   
      USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));
      return USB_ERROR;
    }
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "rcc..");
    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    //
    // Reset the delay back
    //
    gUsbData->UsbBulkDataXferDelay = 0;

    if (!Data) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "err ");
        return USB_ERROR;
    }

    USB_DEBUG(DEBUG_INFO, 3,"Read Capacity 10 LBA %x\n", *(UINT32*)gUsbDataList->UsbTempBuffer);

    if (*(UINT32*)gUsbDataList->UsbTempBuffer == 0xFFFFFFFF) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));
        USBMassReadCapacity16Command(DevInfo);
        return USB_SUCCESS;
    }

    Data = *((UINT32*)(gUsbDataList->UsbTempBuffer + 4));

    //
    // Change little endian format to big endian(INTEL) format
    //
    Data = dabc_to_abcd(Data);

    if (!Data) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "err ");
        return  USB_ERROR;
    }

    DevInfo->BlockSize = (UINT16)Data;
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "succ: %x, %x\n", dData, FpDevInfo);
    //
    // Store the last LBA number in the mass info structure
    //
    Data = *((UINT32*)(gUsbDataList->UsbTempBuffer));

    Data = dabc_to_abcd(Data);

    if (!Data) {
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "err ");
        return USB_ERROR;
    }

    DevInfo->MaxLba = Data + 1; // 1-based value

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "%x ", DevInfo->MaxLba);

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_CAPACITY_10_CMD));

    return USB_SUCCESS;
}

/**
    This function sends read format capacity command to the USB
    mass storage device

    @param  FpDevInfo    Pointer to DeviceInfo structure
    @retval USB_SUCCESS  Finish to read format capacity.
    @retval Others       On fail

    @note  This routine will update the MassDeviceInfo structure
              with the block size & last LBA values obtained from the
              device
**/

UINT8
USBMassReadFormatCapacity (DEV_INFO* FpDevInfo)
{
    COMN_READ_FMT_CAPACITY  *CmdBuffer;
    UINT32  dData;
    UINT16  wData;
    UINT8*      DataBuffer;
    UINT16  DataBufferSize = 0xFC;
    MASS_XACT_STRUC MassXactStruc;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));

    if(!CmdBuffer) {
        return USB_ERROR;
    }

    DataBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT(DataBufferSize));
    if (DataBuffer == NULL) {
      return USB_ERROR;
    }

    CmdBuffer->bOpCode = COMMON_READ_FORMAT_CAPACITY_OPCODE;
    CmdBuffer->wAllocLength = (UINT16)((DataBufferSize << 8) + (DataBufferSize >> 8));

    USBMassClearMassXactStruc(&MassXactStruc);   // Clear the common bulk transaction structure

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;

    if (FpDevInfo->SubClass == SUB_CLASS_SCSI) {
        MassXactStruc.CmdSize = 0x0A;
    } else {
        MassXactStruc.CmdSize = sizeof (COMN_READ_FMT_CAPACITY);
    }

    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = DataBuffer;
//  gUsbData->stMassXactStruc.Length = MAX_TEMP_BUFFER_SIZE;
//
// Temp buffer 40h-64h was used as device name string buffer.
// Limit Transaction size to 40h to prevent name string display problem.
//
    MassXactStruc.Length = DataBufferSize;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "Issue ReadFormatCapacityCommand .... \n");

    dData = USBMassIssueMassTransaction(FpDevInfo, &MassXactStruc);

    //
    // The amount of data obtained should be atleast of read format capacity structure size
    //
    if (dData < sizeof (COMN_READ_FMT_CAPACITY)) {
                USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));
        return USB_ERROR;
    }


    if ((DataBuffer[0] != 0) || (DataBuffer[1] != 0) || (DataBuffer[2] != 0) || (DataBuffer[3] < 0x08)) {
        USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));
        return USB_ERROR;
    }

    wData = *((UINT16*)(DataBuffer + 10));   // Offset 10
    if (wData == 0) {
        USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));
        return USB_ERROR;
    }
    FpDevInfo->BlockSize = (UINT16)((wData << 8) + (wData >> 8));

    dData = *((UINT32*)(DataBuffer + 4));   // Offset 4
    if (dData == 0) {
        USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));
        return USB_ERROR;
    }
    dData = dabc_to_abcd(dData);
    FpDevInfo->MaxLba = dData;

    if (dData == USB_144MB_FDD_MAX_LBA) {
        //
        // Return parameters for 1.44MB floppy
        //
        FpDevInfo->Heads            = USB_144MB_FDD_MAX_HEADS;
        FpDevInfo->NonLbaHeads      = USB_144MB_FDD_MAX_HEADS;
        FpDevInfo->Sectors         = USB_144MB_FDD_MAX_SECTORS;
        FpDevInfo->NonLbaSectors   = USB_144MB_FDD_MAX_SECTORS;
        FpDevInfo->Cylinders       = USB_144MB_FDD_MAX_CYLINDERS;
        FpDevInfo->NonLbaCylinders = USB_144MB_FDD_MAX_CYLINDERS;
        FpDevInfo->MediaType       = USB_144MB_FDD_MEDIA_TYPE;
    }

    USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_READ_FMT_CAPACITY));

    return  USB_SUCCESS;
}

/**
    This function sends get configuration command to the USB
    mass storage device

    @param  DevInfo      Pointer to DeviceInfo structure
    @retval USB_SUCCESS  Success to get configuration data.
    @retval Others       On fail

**/
UINT8
USBMassGetConfiguration(
    DEV_INFO* DevInfo
)
{
    COMMON_GET_CONFIGURATION        *CmdBuffer;
    UINT32                          Data;
    UINT8                               *DataBuffer;
        UINT16                          DataBufferSize = 0x8;
    MASS_XACT_STRUC                 MassXactStruc;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMMON_GET_CONFIGURATION));

    if (!CmdBuffer) {
        return USB_ERROR;
    }

        DataBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT(DataBufferSize));
        if (DataBuffer == NULL) {
                return USB_ERROR;
        }

    CmdBuffer->OpCode = COMMON_GET_CONFIGURATION_OPCODE;
    CmdBuffer->AllocLength = (UINT16)((DataBufferSize << 8) + (DataBufferSize >> 8));

    USBMassClearMassXactStruc(&MassXactStruc);  // Clear the common bulk transaction structure

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
    MassXactStruc.CmdSize = sizeof (COMMON_GET_CONFIGURATION);
    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = DataBuffer;
    MassXactStruc.Length = DataBufferSize;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "Issue GetConfigurationCommand .... \n");

    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(DataBufferSize));
    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMMON_GET_CONFIGURATION));

    if (Data) {
        return USB_SUCCESS;
    } else {
        return USB_ERROR;
    }

}

/**
    This function reads the sector at the LBA specified

    @param DevInfo  Pointer to DeviceInfo structure
    @param LBA      The target LBA
    @param Buffer   Data buffer pointer

    @retval USB_SUCCESS  Finish to read sector.
    @retval Others       On fail

**/

UINT8
USBMassReadSector(
    DEV_INFO*   DevInfo,
    UINT32      Lba,
    UINT8*      Buffer
)
{
    COMN_RWV_10_CMD    *CmdBuffer;
    UINT32              Data;
    UINT8               Counter;
    UINT8               RetValue = USB_ERROR;
    MASS_XACT_STRUC     MassXactStruc;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_RWV_10_CMD));

    if (!CmdBuffer) {
        return USB_ERROR;
    }

    USBMassClearMassXactStruc(&MassXactStruc);

    Counter = 10;
    do {
         //
         // Set opcode to read command
         //
         CmdBuffer->bOpCode = COMMON_READ_10_OPCODE;
         CmdBuffer->wTransferLength = 0x100;     // Big endian to little endian: 0x0001 -> 0x0100
         Data = Lba;
         //
         // Change LBA from big endian to little endian
         //
         Data = dabc_to_abcd(Data);
         CmdBuffer->dLba = Data;

        //
        // Fill the common bulk transaction structure
        //
        MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;

        if (DevInfo->SubClass == SUB_CLASS_SCSI) {
                MassXactStruc.CmdSize = 0x0A;   //SBC-3_60
        } else {
            MassXactStruc.CmdSize = sizeof (COMN_RWV_10_CMD);
        }

        MassXactStruc.XferDir = BIT7;     // IN
        MassXactStruc.FpBuffer = Buffer;
        MassXactStruc.Length = DevInfo->BlockSize;  
        MassXactStruc.PreSkip = 0;
        MassXactStruc.PostSkip= 0;

        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "Read Sector .... \n");

        Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);
        if (Data) {
                        RetValue = USB_SUCCESS;
            break;  // Success
        }
        //
        // May be drive error. Try to correct from it !
        // Check whether the drive is ready for read/write/verify command
        //
        Data = USBMassCheckDeviceReady(DevInfo);
        if (Data) {    // Device is not ready.
            RetValue = USB_ERROR;
            break;
        }
        ZeroMem((UINT8*)CmdBuffer, sizeof(COMN_RWV_10_CMD));
    } while (Counter--);

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_RWV_10_CMD));

    return RetValue;
    
}

/**
    This function parses the boot record and extract the CHS
    information of the formatted media from the boot record.
    This routine checks for DOS & NTFS formats only

    @param DevInfo     Pointer to DeviceInfo structure
    @param MaxLba      Maximum LBA in the device
    @param BootSetor   Boot record of the device

    @retval USB_ERROR   If the boot record is un-recognizable and CHS info
                        is not extracted
    @retval USB_SUCCESS If the boot record is recognizable and CHS info
                        is extracted. CHS information is updated in the
                        device info structure
**/
UINT8
USBMassUpdateCHSFromBootRecord(
    DEV_INFO        *DevInfo,
    UINT64          MaxLba,
    BOOT_SECTOR     *BootSetor
)
{
    UINT32  OemName = 0;
    UINT32  Fat16SysType = 0;
    UINT32  Fat32SysType = 0;

    if (BootSetor->Signature != 0xAA55) {
                return USB_ERROR;
    }

    //
    // Check for valid MSDOS/MSWIN/NTFS boot record
    //
    CopyMem((UINT8*)&OemName, (UINT8*)BootSetor->OEMName, sizeof(OemName));
    if ((OemName != 0x4F44534D) &&    // "ODSM", MSDO...
        (OemName != 0x4957534D) &&    // "IWSM", MSWI...
        (OemName != 0x5346544E)) {    // "SFTN", NTFS
        //
        // Check for valid FAT,FAT16 or FAT32 boot records
        //
        BootSetor->Fat.Fat16.FilSysType[3] = 0x20;
        CopyMem((UINT8*)&Fat16SysType,
                (UINT8*)BootSetor->Fat.Fat16.FilSysType, sizeof(Fat16SysType));
        CopyMem((UINT8*)&Fat32SysType,
                (UINT8*)BootSetor->Fat.Fat32.FilSysType, sizeof(Fat32SysType));
        if ((Fat16SysType != 0x20544146) &&     // " TAF", FAT
            (Fat32SysType != 0x33544146)) {     // "3TAF", FAT3

            //
            // None of the conditions met - boot record is invalid. Return with error
            //
            return  USB_ERROR;
        }
    }

    // zero check added to prevent invalid sector/head information in BPB
    if (BootSetor->SecPerTrk == 0) {
                return USB_ERROR;
    }

    DevInfo->Sectors = (UINT8)BootSetor->SecPerTrk;
        DevInfo->NonLbaSectors = (UINT8)BootSetor->SecPerTrk;

    // Wrong BPB in MSI MegaStick 128; this is preformat usility issue, wrong BPB
    // information built in device.
    if (BootSetor->NumHeads == 0) {
                return USB_ERROR;
    }

    DevInfo->Heads = BootSetor->NumHeads;
    DevInfo->NonLbaHeads = BootSetor->NumHeads;
    DevInfo->BpbMediaDesc = BootSetor->Media;

    USBMassUpdateCylinderInfo(DevInfo, MaxLba);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "CHS: %x %x %x\n",
                    DevInfo->Sectors,
                    DevInfo->Heads,
                    DevInfo->Cylinders);

    return USB_SUCCESS;
    
}

/**
    This procedure update cylinder parameter for device geometry.
    head and sector paramaters are required before invoke this
    function.

    @param Dev Pointer to DeviceInfo structure
    @param Lba Maximum LBA in the device
    @retval VOID

**/

VOID
USBMassUpdateCylinderInfo(
    DEV_INFO*   Dev,
    UINT64      Lba
)
{
    UINT64 Data;

    if ((Dev->Sectors != 0) && (Dev->Heads != 0)) {
        Data = DivU64x64Remainder(Lba, (Dev->Sectors * Dev->Heads), NULL);
    } else {
        Data = 0;
    }
   
    if (Data <= 1) {
        Data++;
    }
    if (Data > 0xFFFF) {
        Data = 0xFFFF;   // DOS workaround
    }
    Dev->Cylinders = (UINT16)Data;
    Dev->NonLbaCylinders = (UINT16)Data;
    
}

/**
    This function reads the first sector from the mass storage
    device and identifies the formatted type.

    @param DevInfo        Pointer to DeviceInfo structure
    @param MaxLba         Maximum LBA of the device
    @param Emu            MSB of emu - Emulation type, 
                          LSB of emu - Device type (Floppy, Harddisk or CDROM)

    @retval USB_ERROR     If could not identify the formatted type
    @retval USB_SUCCESS   If formatted type is identified

**/

UINT8
USBMassGetFormatType(
    DEV_INFO*   DevInfo,
    UINT64      MaxLba,
    UINT16      *Emu
)
{

    MBR_PARTITION  Partition = {0};
    UINT8          *UsbMassConsumeBuffer;
    HC_STRUC       *HcStruc = gHcTable[DevInfo->HcNumber - 1];

    if ((!(gUsbData->UsbStateFlag & USB_FLAG_EDKII_IOMMU_PROTOCOL_ON)) ||
        (HcStruc->UsbMassConsumePhy == NULL) ) {
        UsbMassConsumeBuffer = gUsbDataList->UsbMassConsumeBuffer;
    } else {
        UsbMassConsumeBuffer = HcStruc->UsbMassConsumePhy;
    }
    //
    // Read the first sector of the device
    //
    if (USBMassReadSector(DevInfo, 0, UsbMassConsumeBuffer) == USB_ERROR) {
        return USB_ERROR;
    }

    DevInfo->HiddenSectors = 0;

    //
    // Check for validity of the partition table/boot record
    //
    if (*((UINT16*)(gUsbDataList->UsbMassConsumeBuffer + 0x1FE)) != 0xAA55) {
        USBMassSetDefaultGeometry(DevInfo, MaxLba);
        return USB_ERROR;
    }

    if (USBMassValidatePartitionTable((MASTER_BOOT_RECORD*)gUsbDataList->UsbMassConsumeBuffer, 
        MaxLba, &Partition) == USB_SUCCESS) {
        //
        // Only one partition present, check the device size, if the device size
        // is less than 530 MB assume FDD or else assume the emulation as HDD
        //

        //if (((MaxLba >> 11) < MAX_SIZE_FOR_USB_FLOPPY_EMULATION) &&   //(EIP80382)
        //    !(gUsbData->UsbStateFlag & USB_FLAG_MASS_NATIVE_EMULATION)) {
        //    emu_ = (UINT16)(USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
        //}else {
        //    emu_ = (UINT16)(USB_EMU_HDD_ONLY << 8) + USB_MASS_DEV_HDD;
        //}
        //
        // Read boot sector, set the LBA number to boot record LBA number
        //
        DevInfo->HiddenSectors = (UINT8)Partition.StartingLba;

        if (USBMassReadSector(DevInfo, Partition.StartingLba,
                UsbMassConsumeBuffer) == USB_ERROR) {
            return USB_ERROR;
        }

        if (USBMassUpdateCHSFromBootRecord(DevInfo, MaxLba, 
           (BOOT_SECTOR*)gUsbDataList->UsbMassConsumeBuffer) == USB_SUCCESS) {
            if (((RShiftU64(MaxLba, 11)) < gUsbData->SizeForFloppyEmulation) &&
                !(gUsbData->UsbStateFlag & USB_FLAG_MASS_NATIVE_EMULATION)) {
                if (DevInfo->SubClass != SUB_CLASS_UFI) {
                    *Emu = (UINT16)(USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
                }
            }
            return USB_SUCCESS;
        } else {  // Reset hidden sector value and return HDD emulation
            USBMassSetDefaultGeometry(DevInfo, MaxLba);
            DevInfo->HiddenSectors = 0;
                                                                                //(EIP43711)>
            //don't emulate as HDD for UFI class even media has valid partition like HDD
            if (gUsbData->UsbStateFlag & USB_FLAG_MASS_SIZE_EMULATION) {
                    if (DevInfo->SubClass != SUB_CLASS_UFI) {
                            if ((RShiftU64(MaxLba, 11)) < gUsbData->SizeForFloppyEmulation) {
                        *Emu = (UINT16)(USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
                    }
                }
            }
                                                                                //<(EIP43711)
            return USB_SUCCESS;
        }
    }
    
    *Emu = USBMassSetDefaultType(DevInfo, MaxLba);
    
    if (USBMassUpdateCHSFromBootRecord(DevInfo, MaxLba, 
       (BOOT_SECTOR*)gUsbDataList->UsbMassConsumeBuffer) == USB_SUCCESS) {
        //*emu = USBMassSetDefaultType(FpDevInfo, MaxLba);
         if (gUsbData->UsbStateFlag & USB_FLAG_MASS_SIZE_EMULATION) {
            if (DevInfo->SubClass != SUB_CLASS_UFI) {
                if ((RShiftU64(MaxLba, 11)) >= gUsbData->SizeForFloppyEmulation) {
                    *Emu = (UINT16)(USB_EMU_HDD_ONLY << 8) + USB_MASS_DEV_HDD;
                }
            }
        }
        return USB_SUCCESS;
    }
    USBMassSetDefaultGeometry(DevInfo, MaxLba);

    //*emu = USBMassSetDefaultType(FpDevInfo, MaxLba);

    if (((RShiftU64(MaxLba, 11)) >= gUsbData->SizeForFloppyEmulation) &&
       !(gUsbData->UsbStateFlag & USB_FLAG_MASS_NATIVE_EMULATION)) {
        if (DevInfo->SubClass != SUB_CLASS_UFI) {
            *Emu = (UINT16)(USB_EMU_HDD_ONLY << 8) + USB_MASS_DEV_HDD;
        }
        DevInfo->HiddenSectors = 0;
    }
    //*emu = emu_;

    return USB_SUCCESS;
}

/**
    This procedure set device type depend on device class.

    @param Dev      Pointer to DeviceInfo structure
    @param Lba      Maximum LBA in the device (DWORD)
    @retval DevType Device Type (WORD)

**/

UINT16
USBMassSetDefaultType(
    DEV_INFO*   Dev,
    UINT64      Lba
)
{
    UINT16 DevType = (UINT16)(USB_EMU_FLOPPY_ONLY << 8) + USB_MASS_DEV_ARMD;

    if (Dev->SubClass != SUB_CLASS_UFI) {  // Check whether UFI class device
        // Assume force FDD emulation for non-UFI class device
        DevType = (UINT16)(USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
    }
    return DevType;
}

/**
    This procedure check whether partition table valid.

    @param Mbr the target MBR
    @param Lba Maximum LBA in the device
    @param Mbr Partition table content
    
    @retval USB_SUCCESS partion table is valid:
                        Possible valid entry count(1-based)
                        Table entry counts(0-based, 4 means all entries scaned)
                        Activate entry offset(Absolute offset)
    @retval USB_ERROR Invalid partition table

**/

UINT8
USBMassValidatePartitionTable(
    IN MASTER_BOOT_RECORD       *Mbr,
    IN UINT64                    Lba,
    OUT MBR_PARTITION           *Partition
    )
{
    UINT8   Index = 0;
    UINT8   ActivateIndex = 0;

    // The partition table area could be all 0's, and it would pass the below tests,
    // So test for that here (test sector count for all partitions).
    if ((Mbr->PartRec[0].SizeInLba == 0) && 
       (Mbr->PartRec[1].SizeInLba == 0) &&
       (Mbr->PartRec[2].SizeInLba == 0) &&
       (Mbr->PartRec[3].SizeInLba == 0)) {
        return USB_ERROR;
    }

    for (Index = 0; Index < 4; Index++) {
        // Boot flag check added to ensure that boot sector will not be treated as
        // a valid partation table.
        if (Mbr->PartRec[Index].BootIndicator & 0x7F) {
                        return USB_ERROR;   // BootFlag should be 0x0 or 0x80
        }

        // Check whether beginning LBA is reasonable
        if (Mbr->PartRec[Index].StartingLba > Lba) {
                        return USB_ERROR;
        }

        // Check whether the size is reasonable
#if HDD_PART_SIZE_CHECK
        if (Mbr->PartRec[Index].SizeInLba > Lba) {
            return USB_ERROR;
        }
#endif
        // Update activate entry offset
        if (!(Mbr->PartRec[Index].BootIndicator & 0x80)) {
            continue;
        }

        ActivateIndex = Index;
    }

    // If no activate partition table entry found use first entry
    CopyMem((UINT8*)Partition, (UINT8*)&Mbr->PartRec[ActivateIndex], sizeof(MBR_PARTITION));

    return USB_SUCCESS;

}

/**
    This procedure set default geometry for mass storage devices.

    @param DevInfo       Pointer to DeviceInfo structure
    @param Lba           Maximum LBA in the device

    @retval USB_ERROR    If could not identify the formatted type
    @retval USB_SUCCESS  If formatted type is identified
                         Emulation type - bits 8..15
                         Device type (Floppy, Harddisk or CDROM) - bits 0..7
**/

UINT8 USBMassSetDefaultGeometry(DEV_INFO* DevInfo, UINT64 Lba)
{
    if (DevInfo->SubClass == SUB_CLASS_UFI) {
        DevInfo->Heads = 0x02;
        DevInfo->NonLbaHeads = 0x02;
        DevInfo->Sectors = 0x12;
        DevInfo->NonLbaSectors = 0x12;
    }
    else {
        DevInfo->Sectors = 0x3F;
        DevInfo->NonLbaSectors = 0x3F;
// Use default heads that results in 1023 (3FF) cylinders or less for CHS
        if (Lba <= 0x1F7820) {
            DevInfo->Heads = 0x20;
            DevInfo->NonLbaHeads = 0x20;
        }
        else if ( (Lba > 0x1F7820) && (Lba <= 0x3EF040) ) {
            DevInfo->Heads = 0x40;
            DevInfo->NonLbaHeads = 0x40;
        }
        else if ( (Lba > 0x3EF040) && (Lba <= 0x7DE080) ) {
            DevInfo->Heads = 0x80;
            DevInfo->NonLbaHeads = 0x80;
        }
        else if (Lba > 0x7DE080) {
            DevInfo->Heads = 0xFF;
            DevInfo->NonLbaHeads = 0xFF;
        }
    }

    USBMassUpdateCylinderInfo(DevInfo, Lba);
    return USB_SUCCESS;
}


/**
    This function identifies the type of the USB mass storage
    device attached from the INQUIRY data obtained from the drive

    @param DevInfo Pointer to DeviceInfo structure
    @param InqData Pointer to the inquiry data (read from device)

    @retval USB_SUCCESS  On success
    @retval Others       On fail

**/

UINT8
USBMassIdentifyDeviceType(
    DEV_INFO*   DevInfo,
    UINT8*      InqData
)
{
    UINT16  EmulationType;
    UINT16  ForceEmulationType = 0;
    UINT32  Data = 0;
    UINT8   Count;
    BOOLEAN ReadCapacityTimeout;
    static  UINT16 UsbMassEmulationTypeTable[5] = {
        0,  // Auto
        (USB_EMU_FLOPPY_ONLY << 8) + USB_MASS_DEV_ARMD,     // Floppy
        (USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD,      // Forced floppy
        (USB_EMU_HDD_ONLY << 8) + USB_MASS_DEV_HDD,         // HDD
        (USB_EMU_HDD_OR_FDD << 8) + USB_MASS_DEV_CDROM };   // CDROM

    USBMassGetPhysicalDeviceType(DevInfo, InqData);

    // Note: at this point we assume that dev->EmulationOption is filled in
    // according to the setup question selection.
    if (!(DevInfo->Flag & DEV_INFO_HOTPLUG) || DevInfo->EmulationOption) {    // not auto
        EmulationType = UsbMassEmulationTypeTable[DevInfo->EmulationOption];
        ForceEmulationType = UsbMassEmulationTypeTable[DevInfo->EmulationOption];
    }
    
    //USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, ">>-- IdentifyDeviceType:: Device #%d, Emul#: %d, Emul: %x\n",
    //                              DevInfo->DeviceAddress, DevInfo->EmulationOption, EmulationType);
    if ((gUsbData->UsbFeature & USB_MASS_STORAGE_DEV_RMB_CHECK) == USB_MASS_STORAGE_DEV_RMB_CHECK) {
        if (*(InqData + 1) & 0x80) { // Check RMB status
            DevInfo->LastStatus |= USB_MASS_MEDIA_REMOVEABLE;
        }

    } else {
        DevInfo->LastStatus |= USB_MASS_MEDIA_REMOVEABLE;
    }
    DevInfo->LastStatus |= USB_MASS_MEDIA_PRESENT; // Assume Media Present

    if (*InqData == 5) {   // CDROM
        // Set the type as CDROM and emulation as HDD or FDD
        DevInfo->BlockSize = 0x800;
        EmulationType = (UINT16)(USB_EMU_HDD_OR_FDD << 8) + USB_MASS_DEV_CDROM;
        goto UMIDT_DeviceTypeOver;
    }

//  Start unit command before access it
    if ((gUsbData->UsbFeature & USB_MASS_START_UNIT_BEFORE_MSD_ENUMERATION) == USB_MASS_START_UNIT_BEFORE_MSD_ENUMERATION)
        USBMassStartUnitCommand(DevInfo);

    if (DevInfo->SubClass == SUB_CLASS_UFI) { 
        EmulationType = (UINT16)(USB_EMU_FLOPPY_ONLY << 8) + USB_MASS_DEV_ARMD;
    } else {
        EmulationType = (UINT16)(USB_EMU_HDD_ONLY << 8) + USB_MASS_DEV_HDD;
    }
                                        
    FixedDelay(gUsbDataList->UsbTimingPolicy->MassDeviceComeUp * 1000);    // Device is coming up give 500ms delay
    //
    // Some USB mass storage devces are not fast enough to accept mass storage
    // commands for parsing geometry, issue read capacity command to make sure device
    // is ready for further access. 
    //
    if (DevInfo->SubClass != SUB_CLASS_UFI) {
        for (Count = 0; Count < 30 && VALID_DEVINFO(DevInfo); Count++) {
            if (USBMassReadCapacity10Command(DevInfo) == USB_SUCCESS) {
                break;
            }
            if (gUsbData->UsbLastCommandStatus & USB_BULK_TIMEDOUT) {
                ReadCapacityTimeout = TRUE;
            } else {
                ReadCapacityTimeout = FALSE;
            }
            if ((UINT16)USBMassRequestSense(DevInfo) == 0x3A02 ) {     
                break;  // No media
            }
            if ((gUsbData->UsbLastCommandStatus & USB_BULK_TIMEDOUT) && (ReadCapacityTimeout == TRUE)) {
                USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "Failed to get the readcapacity data, timeout error\n");
                return USB_ERROR;
            }
        }
    }
    //
    // Get the block size & last LBA number
    //
    Data = USBMassCheckDeviceReady(DevInfo);

    if ((UINT16)Data == 0x3A02) {  // Check for media presence status
        //
        // Media not present. Try to get disk geometry from Format
        // capacity command
        //
        if (!(DevInfo->IncompatFlags & USB_INCMPT_FORMAT_CAPACITY_NOT_SUPPORTED)) {
            USBMassReadFormatCapacity(DevInfo);
            if ((DevInfo->MaxLba != 0) && (DevInfo->MaxLba <= USB_144MB_FDD_MAX_LBA)) {
                EmulationType = (UINT16)(USB_EMU_FLOPPY_ONLY << 8) + USB_MASS_DEV_ARMD;
            } else {
                if (!(gUsbData->UsbStateFlag & USB_FLAG_MASS_EMULATION_FOR_NO_MEDIA)) {
                    EmulationType = (UINT16)(USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
                }
            }
            goto UMIDT_DeviceTypeOver;
        }
    }

    //
    // Proceed with normal checking
    //
    if (!Data) {

        //
        // Get the max LBA & block size; if block size is other than
        // 512 bytes assume emulation as CDROM
        //
        //if ( dev->BlockSize > 0x200 ) {
        //    wEmulationType = (UINT16)(USB_EMU_HDD_OR_FDD << 8) + USB_MASS_DEV_CDROM;
        //    goto UMIDT_DeviceTypeOver;
        //}

        if (USBMassGetFormatType(DevInfo, DevInfo->MaxLba, &EmulationType) == USB_ERROR) {
            //
            // Find the device type by size
            //
            if (((RShiftU64(DevInfo->MaxLba, 11)) < gUsbData->SizeForFloppyEmulation) || 
               (gUsbData->UsbStateFlag & USB_FLAG_MASS_NATIVE_EMULATION)) {
                if (DevInfo->SubClass != SUB_CLASS_UFI) {
                    EmulationType = (USB_EMU_FORCED_FDD << 8) + USB_MASS_DEV_ARMD;
                }
            }
        }
    }


UMIDT_DeviceTypeOver:

    if (ForceEmulationType) {
        EmulationType = ForceEmulationType;
    }
    DevInfo->StorageType = (UINT8)EmulationType;
    DevInfo->EmuType = (UINT8)(EmulationType >> 8);

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "<<-- IdentifyDeviceType:: Emul: %x\n", EmulationType);

    return USB_SUCCESS;
}

/**
    This procedure classify USB mass storage devices according to
    inquiry command return data.

    @param DevInfo      Pointer to DeviceInfo structure
    @param IngDataBuf   Pointer to the inquiry data (read from device)

    @retval VOID

**/
VOID
USBMassGetPhysicalDeviceType(
    DEV_INFO*   DevInfo,
    UINT8       *IngDataBuf
)
{

    switch (*IngDataBuf) {
        case 0x0:
            if (DevInfo->SubClass == SUB_CLASS_UFI) {
                DevInfo->PhyDevType = USB_MASS_DEV_FDD;
                break;
            }
            DevInfo->PhyDevType = (*(IngDataBuf+1) & 0x80) ? 
                USB_MASS_DEV_ARMD : USB_MASS_DEV_HDD;
            break;
        case 0x5:
            DevInfo->PhyDevType = USB_MASS_DEV_CDROM;
            break;
        case 0x7:
            DevInfo->PhyDevType = USB_MASS_DEV_MO;
            break;
        case 0xE:
            DevInfo->PhyDevType = USB_MASS_DEV_ARMD;
            break;
        default:
            DevInfo->PhyDevType = USB_MASS_DEV_UNKNOWN;
            break;
    }
    
}


/**
    This function reads/writes the data to the mass storage
    device using bulk transfer. It also takes care of pre and
    post skip bytes.

    @param DevInfo        Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to the mass transaction structure
                          XferDir  - Transfer direction
                          FpBuffer - Data buffer far pointer
                          Length   - Amount of data to be transferred
                          PreSkip  - Number of bytes to skip before data
                          PostSkip - Number of bytes to skip after data

    @retval Data          Amount of data actually transferred

**/
UINT32
USBMassProcessBulkData(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{
    UINT32          Data;
    UINT16          Temp;
    UINT8           *Buffer;
    UINT8           *SrcBuffer;
    UINT8           *DestBuffer;
    UINT16          PreSkip;
    UINT32          XferSize;
    UINT32          XferedSize;
    UINT32          RemainingSize;

//USB_DEBUG (DEBUG_INFO, DEBUG_LEVEL_3, "Pre,%x;Post,%x\n", gUsbData->stMassXactStruc.PreSkip,
//                  gUsbData->stMassXactStruc.PostSkip);
    //
    // Check whether something we have to transfer
    //
    if (!MassXactStruc->Length) {
        return 0;
    }


    Temp   = gUsbData->UsbReqTimeOutValue;     // Save original value
    if (gUsbData->UsbBulkDataXferDelay) {    // Check the bulk data delay specified
        gUsbData->UsbReqTimeOutValue = gUsbData->UsbBulkDataXferDelay;
    }

    if ((MassXactStruc->PreSkip == 0) && (MassXactStruc->PostSkip == 0)) {
         Data = USBMassIssueBulkTransfer(
                            DevInfo,
                            MassXactStruc->XferDir,
                            MassXactStruc->FpBuffer,
                            MassXactStruc->Length);
    } else {
         // Allocate a data buffer
         Buffer = USB_MemAlloc((UINT16)GET_MEM_BLK_COUNT(DevInfo->BlockSize));
         PreSkip = MassXactStruc->PreSkip;
         RemainingSize = MassXactStruc->Length - (PreSkip + MassXactStruc->PostSkip);
         DestBuffer = MassXactStruc->FpBuffer;

         for (XferedSize = 0; XferedSize < MassXactStruc->Length;) {
              XferSize = MassXactStruc->Length >= DevInfo->BlockSize ?
                         DevInfo->BlockSize : MassXactStruc->Length;

              Data = USBMassIssueBulkTransfer(
                                DevInfo,
                                MassXactStruc->XferDir,
                                Buffer,
                                XferSize);
              if (Data == 0) {
                //return 0;
                XferedSize = 0;
                break;
              }

              XferedSize += XferSize;
              if (RemainingSize == 0) {
                  continue;
              }

              SrcBuffer = Buffer;
        
              if (PreSkip != 0) {
                  if (PreSkip >= XferSize) {
                      PreSkip -= (UINT16)XferSize;
                      continue;
                  }
        
                  SrcBuffer += PreSkip;
                  XferSize -= (UINT32)PreSkip;
                  PreSkip = 0;
              }

              XferSize = RemainingSize < XferSize ? RemainingSize : XferSize;
              CopyMem(DestBuffer, SrcBuffer, XferSize);

              // Update the destination buffer pointer
              DestBuffer += XferSize;
              RemainingSize -= XferSize;
         }
                
         USB_MemFree(Buffer, (UINT16)GET_MEM_BLK_COUNT(DevInfo->BlockSize));

         Data = XferedSize;
    }

    gUsbData->UsbReqTimeOutValue = Temp;   // Restore original timeout value
    gUsbData->UsbBulkDataXferDelay = 0;

    return Data;
}

/**
    This function sends inquiry command to the USB mass storage
    device

    @param DevInfo        Pointer to DeviceInfo structure
    @retval UsbTempBuffer Pointer to the inquiry data

**/

MASS_INQUIRY*
USBMassInquiryCommand (DEV_INFO* DevInfo)
{
    COMMON_INQ_CMD  *   CmdBuffer;
    UINT32              Data;
    MASS_XACT_STRUC     MassXactStruc;
    EFI_STATUS          Status;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMMON_INQ_CMD));
    if(!CmdBuffer) {
        return NULL;
    }

    if (gCheckUsbApiParameter) {
        Status = AmiUsbValidateMemoryBuffer((VOID*)(gUsbDataList->UsbTempBuffer + INQUIRY_TEMP_BUFFER_OFFSET), sizeof(MASS_INQUIRY));
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            return NULL;
        }
        gCheckUsbApiParameter = FALSE;
    }
    
    CmdBuffer->bOpCode = COMMON_INQUIRY_OPCODE;
    CmdBuffer->bAllocLength = INQUIRY_COMMAND_ALLOCATE_LENGTH;

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
                                        //(EIP51158+)>
    if (DevInfo->SubClass == SUB_CLASS_SCSI) {
                MassXactStruc.CmdSize = INQUIRY_SPC_4_COMMAND_SIZE;     //SPC-4_246     
    } else {
        MassXactStruc.CmdSize = sizeof (COMMON_INQ_CMD);
    }
                                                                                //<(EIP51158+)
    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = gUsbDataList->UsbTempBuffer + INQUIRY_TEMP_BUFFER_OFFSET;
    MassXactStruc.Length = INQUIRY_TRNASFER_LENGTH;

    if(MassXactStruc.Length  > MAX_TEMP_BUFFER_SIZE){
      USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "UsbMass:ERROR!Inquiry Command buffer is to small!\n"); 
      USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMMON_INQ_CMD));  
      return NULL;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "Issue Inquiry Command .... \n");

    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMMON_INQ_CMD));


    if (Data) {
        return (MASS_INQUIRY*)(gUsbDataList->UsbTempBuffer + INQUIRY_TEMP_BUFFER_OFFSET);
    }
    else {
        return NULL;
    }
}


/**
    This function reads/writes/verifies blocks of data from the
    USB mass device specified by its device address

    @param DevInfo   Pointer to DeviceInfo structure
    @param OpCode     Read/Write/Verify
    @param DataStruc  Pointer to the read command structure
                      DevAddr - USB device address of the device
                      StartLBA - Starting LBA address
                      NumBlks - Number of blocks to process
                      PreSkipSize - Number of bytes to skip before
                      PostSkipSize - Number of bytes to skip after
                      BufferPtr - Far buffer pointer

    @retval RetCode   0: failure, other: size read

**/

UINT16
USBMassRWVCommand(
    DEV_INFO    *DevInfo,
    UINT8       OpCode,
    VOID        *DataStruc
)
{
    EFI_MASS_READ               *MassDataStruc = (EFI_MASS_READ*)DataStruc;
    COMN_RWV_16_CMD         *CmdBuffer;
    UINT64              StartLba;
    UINT32              BytesToRw;
    UINT32              Data;
    UINT32              SenseData;
    UINT8               Dir;       // BIT7 0/1 - R/W
    UINT8               RetryNum;
    UINT16              RetCode = 0;
    UINT8               CmdSize;
    EFI_STATUS          Status;
    MASS_XACT_STRUC     MassXactStruc;
    


    BOOLEAN             DoSecurityCheck = TRUE;
    
    if ((OpCode == COMMON_VERIFY_16_OPCODE) || (OpCode == COMMON_VERIFY_10_OPCODE)){  
      DoSecurityCheck = FALSE;
    }
    
    if (gCheckUsbApiParameter) {
        if (DoSecurityCheck){
        Status = AmiUsbValidateMemoryBuffer((VOID*)MassDataStruc->BufferPtr, 
                (UINT32)MassDataStruc->NumBlks * (UINT32)DevInfo->BlockSize);
        if ((EFI_ERROR(Status))&&(Status != EFI_ABORTED)){
            USB_DEBUG(DEBUG_ERROR, 3, "UsbMassRWVCommand Invalid Pointer, Buffer is in SMRAM.\n");
            return 0;
            }
        }
        gCheckUsbApiParameter = FALSE;
    }
    //
    // Set the sense code as 0
    //
    MassDataStruc->SenseData = 0;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = (COMN_RWV_16_CMD*)USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_RWV_16_CMD));
    if (!CmdBuffer) {
        return 0;
    }

    for (RetryNum = 0; RetryNum < 2; RetryNum++) {
         //
         // Load command into (just allocated) mass command buffer
         //
         CmdBuffer->OpCode = OpCode;
         StartLba = MassDataStruc->StartLba;

        if (StartLba > (DevInfo->MaxLba - MassDataStruc->NumBlks)) {
            StartLba = DevInfo->MaxLba - MassDataStruc->NumBlks;
        }

        //
        // If the "Forced FDD" option is selected that means the device has
        // to be emulated as a floppy drive even though it has a HDD emulated
        // image.  This is accomplished by hiding the first cylinder totally.
        // The partition table is in the first cylinder.  LBA value for all
        // the requests to the device will be offset with the number of sectors
        // in the cylinder.
        //

         //
         // Check for forced floppy emulated device and change LBA accordingly
         //
         if (DevInfo->EmuType == USB_EMU_FORCED_FDD) {
         if (!(gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI)) //(EIP113379+)
             //
             // Skip first track in case of floppy emulation
             //
             StartLba += DevInfo->HiddenSectors;
         }

        //
        // Check the validity of the block size
        //
        if (DevInfo->BlockSize != 0xFFFF) {
            //
            // Change big endian format (INTEL) to little endian format
            //
            if ((OpCode == COMMON_READ_10_OPCODE) ||
                (OpCode == COMMON_WRITE_10_OPCODE) ||
                (OpCode == COMMON_VERIFY_10_OPCODE)) {
                ((COMN_RWV_10_CMD*)CmdBuffer)->dLba = dabc_to_abcd((UINT32)StartLba);
                ((COMN_RWV_10_CMD*)CmdBuffer)->wTransferLength =
                    (UINT16)((MassDataStruc->NumBlks << 8) + (MassDataStruc->NumBlks >> 8));
                if (DevInfo->SubClass == SUB_CLASS_SCSI) {
                            CmdSize = 0x0A;     //SBC-3_60
                } else {
                    CmdSize = sizeof (COMN_RWV_10_CMD);
                }
            } else {
                CmdBuffer->Lba = LShiftU64(dabc_to_abcd((UINT32)StartLba), 32);
                CmdBuffer->Lba |= dabc_to_abcd((UINT32)RShiftU64(StartLba, 32));
                CmdBuffer->TransferLength = dabc_to_abcd(MassDataStruc->NumBlks);
                CmdSize = sizeof(COMN_RWV_16_CMD);
            }
            //
            // Verify command does not need delay
            //
            gUsbData->UsbBulkDataXferDelay = 0;

            //
            // Calculate number of bytes to transfer (for verify command nothing
            // to read/write.
            //
            BytesToRw = 0;
            if ((OpCode != COMMON_VERIFY_10_OPCODE) && 
                (OpCode != COMMON_VERIFY_16_OPCODE)){
                //
                // Read/write command may need long time delay
                //
                gUsbData->UsbBulkDataXferDelay = 20000;
                BytesToRw = (UINT32)MassDataStruc->NumBlks * (UINT32)DevInfo->BlockSize;
            }

            //
            // Set the direction properly
            //
            if ((OpCode == COMMON_WRITE_10_OPCODE) ||
                (OpCode == COMMON_WRITE_16_OPCODE)) {
                Dir = 0;
            } else {
                Dir = BIT7;
            }

            //
            // Fill the common bulk transaction structure
            // Fill Command buffer address & size
            //
            MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
            MassXactStruc.CmdSize = CmdSize;
            MassXactStruc.XferDir = Dir;
            MassXactStruc.FpBuffer = (UINT8*)(UINTN)MassDataStruc->BufferPtr;
            MassXactStruc.PreSkip = MassDataStruc->PreSkipSize;
            MassXactStruc.PostSkip = MassDataStruc->PostSkipSize;
            MassXactStruc.Length = BytesToRw;

            Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

            if ((Data) && ((RetryNum != 0) || (Data == BytesToRw))) {    // Some data processed. Set return value
                
                //
                // Bug fix for installing Linux from USB CD-ROM.
                // Linux64Bit Boot
                // If data read is 64K or higher return 0FFFFh
                //
                if (Data >= 0x010000) {
                    Data = 0xFFFF;
                }

                RetCode = (UINT16)Data;
                //
                // Check for forced floppy emulated device
                //
                if ((DevInfo->EmuType == USB_EMU_FORCED_FDD) &&
                     (OpCode == COMMON_READ_10_OPCODE) &&
                     (MassDataStruc->StartLba == 0) && 
                    !(gUsbData->UsbStateFlag & USB_FLAG_RUNNING_UNDER_EFI) ) {     //(EIP113379+)
                    //
                    // This is a floppy emulated ZIP drive, with read to
                    // first sector. Update the boot record so that floppy
                    // emulation is okay.
                    //
                    // Force #of hidden sectors to 0
                    //
                    *(UINT32*)((UINTN)MassDataStruc->BufferPtr + 0xB + 0x11) = 0;

                    //
                    // FreeDOS workaround
                    //
                    if ((*(UINT32*)((UINTN)MassDataStruc->BufferPtr + 3)==0x65657246) &&  // 'eerF'
                        (*(UINT32*)((UINTN)MassDataStruc->BufferPtr + 7)==0x20534F44) &&  // ' SOD'
                        (*(UINT32*)((UINTN)MassDataStruc->BufferPtr + 0x3A)!=0x20202032)) {                             //(EIP61388)
                        *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x42) =
                            *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x42)-(UINT16)DevInfo->HiddenSectors;
                        *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x46) =
                            *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x46)-(UINT16)DevInfo->HiddenSectors;
                        *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x4A) =
                            *(UINT16*)((UINTN)MassDataStruc->BufferPtr + 0x4A)-(UINT16)DevInfo->HiddenSectors;
                    }
                    //
                    // Force physical drive# to 0
                    // For FAT32, physical drive number is present in offset 40h
                    //
                    if ((*(UINT32*)((UINTN)MassDataStruc->BufferPtr + 0x52)) ==
                                        0x33544146) {       // "3TAF", FAT3
                        *(UINT8*)((UINTN)MassDataStruc->BufferPtr + 0x40) = 0;
                    }
                    else {
                        *(UINT8*)((UINTN)MassDataStruc->BufferPtr + 0x24) = 0;
                    }
                }
                break;  // dData ready

            }
            else {  // Error condition: dData = 0, RetCode = 0
                //
                // Check for error
                //
                SenseData = USBMassRequestSense(DevInfo);
                MassDataStruc->SenseData = SenseData;
                Data = SenseData;

                //
                // Check for write protect error code
                //
                if ((UINT8)Data == 7) {
                    break;
                }

                if (((OpCode == COMMON_VERIFY_10_OPCODE) ||
                    (OpCode == COMMON_VERIFY_16_OPCODE)) && (!SenseData)) {
                    //
                    // This is verify command so no data to send or read and
                    // also sense data is 0. So set return value to success.
                    //
                    RetCode = 0xFFFF;
                    break;
                }
            }
        }   // FpDevInfo->BlockSize != 0xFFFF

        //
        // UPRCC_ProceedIfRW
        // May be drive error, try to correct it
        // Check whether the drive is ready for read/write/verify command
        //
        Data = USBMassCheckDeviceReady(DevInfo);
        MassDataStruc->SenseData = Data;

        if (Data) {
            break;  // Return error
        }

        ZeroMem((UINT8*)CmdBuffer, sizeof(COMN_RWV_16_CMD));
    }   // Fof loop

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_RWV_16_CMD));

    return RetCode;
}


/**
    This function sends the start unit command to the mass device

    @param FpDevInfo   Pointer to DeviceInfo structure
    @retval Sense data: 0 - Success, Others - Error

**/

UINT16
USBMassStartUnitCommand (DEV_INFO* FpDevInfo)
{
    COMMON_START_STOP_UNIT_CMD  *       CmdBuffer;
    MASS_XACT_STRUC                     MassXactStruc;


    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBMProStartUnitCommand ....  \n");

    //
    // Check the compatibility flag for start unit command not supported
    //
    if (FpDevInfo->IncompatFlags & USB_INCMPT_START_UNIT_NOT_SUPPORTED) {
        return  USB_SUCCESS;
    }

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMMON_START_STOP_UNIT_CMD));
    if (!CmdBuffer) {
        return  USB_ERROR;
    }

    //
    // Load command into (just allocated) mass command buffer
    //
    CmdBuffer->bOpCode = COMMON_START_STOP_UNIT_OPCODE;
    CmdBuffer->bStart = 1;

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);
    gUsbData->UsbBulkDataXferDelay = 10000;  // Start unit command may need long time delay
    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;

    if (FpDevInfo->SubClass == SUB_CLASS_SCSI) {
        MassXactStruc.CmdSize = 0x06;   //SBC-3_77
    } else {
        MassXactStruc.CmdSize = sizeof (COMMON_START_STOP_UNIT_CMD);
    }
    USBMassIssueMassTransaction(FpDevInfo, &MassXactStruc);

    //
    // No data to read/write. So do not process return code.
    // Check and free command buffer
    //
    USB_MemFree(CmdBuffer,GET_MEM_BLK_COUNT_STRUC(COMMON_START_STOP_UNIT_CMD));

    return (UINT16)USBMassRequestSense(FpDevInfo);
}


/**
    This function requests the mode sense data page number 5 from
    the USB mass storage device

    @param FpDevInfo         Pointer to DeviceInfo structure
    @param FpModeSenseData   Pointer to mode sense data
                             SenseData - Sense data
                             NumHeads  - Number of heads
                             NumCylinders - Number of cylinders
                             NumSectors - Number of sectors
                             BytesPerSector - Number of bytes per sector
                             MediaType  - Media type
    @retval USB_SUCCESS      On success
    @retval USB_ERROR        Failure
**/

UINT8
USBMassModeSense(
    DEV_INFO        *FpDevInfo,
    MASS_MODE_SENSE *FpModeSenseData)
{
    UINT32                  Data;
    UINT8                   RetCode;
    COMN_MODE_SENSE_10CMD   *CmdBuffer;
    MODE_SENSE_10_HEADER    *FpModeSense10_Header;
    PAGE_CODE_5             *FpPageCode5;
    MASS_XACT_STRUC         MassXactStruc;

    Data = 0;
    RetCode = USB_ERROR;

    CmdBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMN_MODE_SENSE_10CMD));
    if (!CmdBuffer) {
        return  USB_ERROR;
    }

    //
    // Load command into (just allocated) mass command buffer
    //
    CmdBuffer->bOpCode = COMMON_MODE_SENSE_10_OPCODE;
    CmdBuffer->wAllocLength = 0x2800; // Allocation Length = 40 bytes (0x28)
    CmdBuffer->bPageCode = 5; // Page code

    //
    // Clear the common bulk transaction structure
    //
    USBMassClearMassXactStruc(&MassXactStruc);

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
                                        //(EIP51158+)>
    if (FpDevInfo->SubClass == SUB_CLASS_SCSI) {
        MassXactStruc.CmdSize = 0x0A;   //SPC-4_280
    } else {
        MassXactStruc.CmdSize = sizeof (COMN_MODE_SENSE_10CMD);
    }
                                                                                //<(EIP51158+)
    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = gUsbDataList->UsbTempBuffer;
    MassXactStruc.Length = 0x28;

    if(MassXactStruc.Length  > MAX_TEMP_BUFFER_SIZE){
      USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "UsbMass:ERROR!USBMassModeSense buffer is to small!\n");   
      USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_MODE_SENSE_10CMD));
      return  USB_ERROR;
    }
    //
    // Bulk in, with temp buffer & 40 bytes of data to read
    //
    Data = USBMassIssueMassTransaction(FpDevInfo, &MassXactStruc);

    if (!Data) {
        USBMassRequestSense( FpDevInfo );
        USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_MODE_SENSE_10CMD));
        return  USB_ERROR;
    }

    //
    // Fill in the output data
    //
    FpModeSense10_Header = (MODE_SENSE_10_HEADER*)gUsbDataList->UsbTempBuffer;

    //
    // Process media type
    //
    FpModeSenseData->MediaType = FpModeSense10_Header->MediaType;

    //
    // Position to the correct page code starting location
    //
    FpPageCode5 = (PAGE_CODE_5*)((UINT8*)FpModeSense10_Header +
                                        FpModeSense10_Header->wBlkDescSize +
                                        sizeof (MODE_SENSE_10_HEADER));
//  USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "USBMassModeSense ....  fpPageCode5->bPageCode %x\n",fpPageCode5->bPageCode);

    
    if(FpPageCode5->bPageCode == 5) {
        //
        // Process number of bytes per sector (the block size)
        //
        FpModeSenseData->BytesPerSector = (UINT16)((FpPageCode5->BlockSize << 8)
                                            + (FpPageCode5->BlockSize >>8));
        //
        // Process number of heads and number of sectors/track
        //
        FpModeSenseData->NumHeads = FpPageCode5->Heads;
        FpModeSenseData->NumSectors = FpPageCode5->Sectors;

        //
        // Process number of cylinders
        //
        FpModeSenseData->NumCylinders  = (UINT16)((FpPageCode5->Cylinders << 8)
                                        + (FpPageCode5->Cylinders >> 8));
        RetCode = USB_SUCCESS;
    }

    FpModeSenseData->SenseData = USBMassRequestSense( FpDevInfo );

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_MODE_SENSE_10CMD));
// USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBMProModeSense ....  wRetCode %x\n",wRetCode);

    return RetCode;

}


/**
    This function sends request sense command and returns
    the sense key information

    @param FpDevInfo   Pointer to DeviceInfo structure

    @retval Data       Sense data
    @retval USB_ERROR   Error (No sense data)
**/

UINT32
USBMassRequestSense(DEV_INFO* FpDevInfo)
{
    UINT32                      Data;
    UINT8                       *DataBuffer;
    COMMON_REQ_SENSE_CMD        *CmdBuffer;
    MASS_XACT_STRUC             MassXactStruc;

    //
    // Allocate memory for the command buffer
    //
    CmdBuffer = (COMMON_REQ_SENSE_CMD*)USB_MemAlloc(GET_MEM_BLK_COUNT_STRUC(COMMON_REQ_SENSE_CMD));
    if(!CmdBuffer) {
        return USB_ERROR;   // Error - return no sense data <>0
    }

    DataBuffer = USB_MemAlloc(GET_MEM_BLK_COUNT(1));
    if(!DataBuffer) {
        return USB_ERROR;   // Error - return no sense data <>0
    }

    //
    // Load command into (just allocated) mass command buffer
    //
    CmdBuffer->bOpCode = COMMON_REQUEST_SENSE_OPCODE;
    CmdBuffer->bAllocLength = 0x12;   // Length of transfer

    USBMassClearMassXactStruc(&MassXactStruc);    // Clear the common bulk transaction structure

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer  = (UINT8*)CmdBuffer;
                                        //(EIP51158+)>
    if (FpDevInfo->SubClass == SUB_CLASS_SCSI) {
                MassXactStruc.CmdSize = 0x06;   //SPC-4_350
    } else {
        MassXactStruc.CmdSize = sizeof (COMMON_REQ_SENSE_CMD);
    }
                                                                                //<(EIP51158+)
    MassXactStruc.XferDir = BIT7;     // IN
    MassXactStruc.FpBuffer = DataBuffer;
    MassXactStruc.Length  = 0x12;

    //
    // Bulk in, with locally allocated temp buffer & 12h bytes of data to read
    //
    Data = USBMassIssueMassTransaction(FpDevInfo, &MassXactStruc);

    if(Data) {
        //
        // Form the return value:
        //      Bit 0..7    - Sense key (offset 002d)
        //      Bit 8..15   - ASC code (offset 012d)
        //      Bit 16..23  - ASCQ code (offset 013d)
        //
        Data = (UINT32)(DataBuffer[2] +
               (DataBuffer[12] << 8) +
               (DataBuffer[13] << 16));
        USBMassSenseKeyParsing(FpDevInfo, Data);
    }
     else
       Data = USB_ERROR;

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMMON_REQ_SENSE_CMD));
    USB_MemFree(DataBuffer, GET_MEM_BLK_COUNT(1));

    return Data;
}


/**
    Translate USB sense key to USB MassStorage status.

    @param FpDevInfo   Pointer to DeviceInfo structure
    @param SenseData   Sense data
                       SenseData[23..16]   ASCQ
                       SenseData[15..08]   ASC
                       SenseData[07..00]   Sense Code
    @retval None

**/
VOID
USBMassSenseKeyParsing(DEV_INFO* FpDevInfo, UINT32 SenseData)
{
    if ((UINT16)SenseData == 0x3A02) {             
        FpDevInfo->LastStatus &= ~USB_MASS_MEDIA_PRESENT;
    }
    if((UINT16)SenseData == 0x2806) {
        FpDevInfo->LastStatus |= (USB_MASS_MEDIA_PRESENT | USB_MASS_MEDIA_CHANGED);
    }

    if(SenseData == 0) {
        FpDevInfo->LastStatus |= USB_MASS_MEDIA_PRESENT;
    }

}


/**
    This function sends test unit ready command

    @param  DevInfo       Pointer to DeviceInfo structure
    @retval Data          Sense data
    @retval USB_ERROR     Error(return no sense data)
**/

UINT32
USBMassTestUnitReady(
    DEV_INFO* DevInfo
)
{
    COMN_TEST_UNIT_READY_CMD        *CmdBuffer;
    UINT32                          Data;
    MASS_XACT_STRUC                 MassXactStruc;

    CmdBuffer = (COMN_TEST_UNIT_READY_CMD*)USB_MemAlloc(
                 GET_MEM_BLK_COUNT_STRUC(COMN_TEST_UNIT_READY_CMD));
    if (!CmdBuffer) {
        return USB_ERROR;       // Error - return no sense data
    }

    CmdBuffer->bOpCode = COMMON_TEST_UNIT_READY_OPCODE;
    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "USBMassTestUnitReady ....  \n");

    USBMassClearMassXactStruc(&MassXactStruc);    // Clear the common bulk transaction structure

    //
    // Fill the common bulk transaction structure
    //
    MassXactStruc.CmdBuffer = (UINT8*)CmdBuffer;
                                        //(EIP51158+)>
    if (DevInfo->SubClass == SUB_CLASS_SCSI) {
                MassXactStruc.CmdSize = 0x06;   //SPC-4_368
    } else {
        MassXactStruc.CmdSize = sizeof (COMN_TEST_UNIT_READY_CMD);
    }
                                                                                //<(EIP51158+)
    Data = USBMassIssueMassTransaction(DevInfo, &MassXactStruc);

    USB_MemFree(CmdBuffer, GET_MEM_BLK_COUNT_STRUC(COMN_TEST_UNIT_READY_CMD));

    if ((Data == USB_ERROR) || (DevInfo->Protocol == PROTOCOL_CBI) || 
        (DevInfo->Protocol == PROTOCOL_CBI_NO_INT)) {
        Data = USBMassRequestSense(DevInfo);
    }

    return Data;
}


/**
    This function makes sure the device is ready for next
    command

    @param FpDevInfo   Pointer to DeviceInfo structure
    @retval Data       Sense code

**/

UINT32
USBMassCheckDeviceReady (DEV_INFO* FpDevInfo)
{
    UINT8   Count, NomediaRetryCount;
    UINT8   NotReadyCount;    
    UINT32  Data = 0;

    Count = gUsbData->UsbStorageDeviceDelayCount;
    NomediaRetryCount = 3;
    NotReadyCount = 3;                                
    while (Count) {
        if (FpDevInfo->IncompatFlags & USB_INCMPT_TEST_UNIT_READY_FAILED) {
            break;  // consider device is ready
        }

        //
        // Issue test unit ready command and check the return value
        //
        Data = USBMassTestUnitReady( FpDevInfo );
//USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_3, "(%d)tur..%x ", FpDevInfo->DeviceAddress, dData);
        if ((UINT8)Data == 0) { // Device ready
            break;
        }
        //
        // Device is not ready.
        // Check for getting ready/reset command occurence in dData:
        //      Bit 0..7    - Sense Code
        //      Bit 8..15   - Additional Sense Code (ASC)
        //      Bit 16..23  - Additional Sense Code Qualifier (ASCQ)
        //
        if ((UINT16)Data == 0x2806) {
            //
            // Send Start/Stop Unit command to UFI class device only
            //
            if (FpDevInfo->SubClass == SUB_CLASS_UFI) {
                USBMassStartUnitCommand (FpDevInfo);
            }
            FixedDelay(100 * 1000);        // 100 msec delay
            Count--;
            continue;
        }
        if ((UINT16)Data == 0x3A02) {                  // Media is not present
            NomediaRetryCount--;
            if (NomediaRetryCount == 0) return  Data;  // No media
            FixedDelay(20 * 1000);        // 20 msec delay
            Count--;
            continue;
        }

                if (Data == 0x020402)
                {
                        USBMassStartUnitCommand (FpDevInfo);
                        FixedDelay(100 * 1000);
                        Count--;
                        continue;
                }

                if ((UINT16)Data == 0x1103) {
                        FixedDelay(100 * 1000);
                        Count--;
                        continue;
                }

        //
        // Check whether we can recover from this error condition
        // Currently only recoverable error condition are
        // 1. Device is getting ready (010402)
        // 2. Device reset occurred (002906)
        //
        if (Data != 0x010402) {
            //
            // Check for write protected command
            //
            if ( (UINT8)Data == 7 ) {
                break;
            }
            if (((UINT8)Data != 0x06) && ((UINT8)Data != 0x02)) {
                return  Data;
            }
        }

                                        //(EIP101623+)>
        if (Data == 0x02) {                    
            NotReadyCount--;
            if (NotReadyCount == 0) return  Data;  
            FixedDelay(20 * 1000); // 20 msec delay
            Count--;
            continue;
        }
                                        //<(EIP101623+)
            
        //
        // Prepare for the next itaration
        // Delay for the device to get ready
        //
        FixedDelay(1000 * 1000);       // 1 sec delay
        Count--;
    }   // while

    if (Count == 0) {
        return Data;
    }

    return USBMassUpdateDeviceGeometry(FpDevInfo);
}


/**
    This function obtains the device geometry from the device
    using mode sense command and updates the global variables

    @param Pointer to DeviceInfo structure

    @retval USB_ERROR   On error
    @retval USB_SUCCESS On success

**/

UINT8
USBMassUpdateParamUsingModeSense(DEV_INFO* FpDevInfo)
{
    MASS_MODE_SENSE ModeSenseData;

    gUsbData->UsbModeSenseCylinders = gUsbData->UsbModeSenseHeads =
    gUsbData->UsbModeSenseSectors = 0;
    //
    // Check the compatibility flag for mode sense support
    //
    if (FpDevInfo->IncompatFlags & USB_INCMPT_MODE_SENSE_NOT_SUPPORTED) {
        return  USB_SUCCESS;
    }

    //
    // CDROM devices never support mode sense page code 5 (Flexible disk page)
    // so skip it
    //
    if (FpDevInfo->StorageType == USB_MASS_DEV_CDROM) {
        return  USB_ERROR;
    }

    //
    // Issue mode sense command
    //
    if (USBMassModeSense(FpDevInfo, &ModeSenseData)) {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "ms..err ");
        return  USB_ERROR;
    }

    //
    // Mode sense is supported. Update the local structure.
    //
    gUsbData->UsbModeSenseCylinders  = ModeSenseData.NumCylinders;  // Number of cylinders
    gUsbData->UsbModeSenseHeads      = ModeSenseData.NumHeads;      // Number of heads
    gUsbData->UsbModeSenseSectors    = ModeSenseData.NumSectors;    // Number of sectors
    gUsbData->UsbModeSenseBlockSize  = ModeSenseData.BytesPerSector;// Number of bytes per sector
    gUsbData->DiskMediaType       = ModeSenseData.MediaType;     // Media type


    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "ms..%x %x %x %x %x ",
            gUsbData->UsbModeSenseCylinders,
            gUsbData->UsbModeSenseHeads,
            gUsbData->UsbModeSenseSectors,
            gUsbData->UsbModeSenseBlockSize,
            gUsbData->DiskMediaType);

    if (FpDevInfo->StorageType == USB_MASS_DEV_HDD) {
        gUsbData->DiskMediaType = USB_UNKNOWN_MEDIA_TYPE;
    }

    //
    // Calculate and update Max LBA
    //
    gUsbData->UsbModeSenseMaxLBA =
            (UINT32)(ModeSenseData.NumCylinders *
            ModeSenseData.NumHeads * ModeSenseData.NumSectors);
    //
    // Set the flag indicating mode sense is executed
    //
    gUsbData->UsbGeometryCommandStatus |= MODE_SENSE_COMMAND_EXECUTED;

    return  USB_SUCCESS;
}


/**
    This function obtains the device geometry from the device
    using read capacity command and updates the global variables

    @param Pointer to DeviceInfo structure

    @retval USB_ERROR   On error
    @retval USB_SUCCESS On success

**/

UINT8
USBMassUpdateParamUsingReadCapacity(
    DEV_INFO* DevInfo
)
{
    UINT8   Sectors;
    UINT8   Heads;

    //
    // Either mode sense not supported or failed. Try read capacity
    // Issue read capacity command
    //
    if (USBMassReadCapacity10Command(DevInfo)) {
        return  USB_ERROR;
    }

    //
    // Set the flag indicating read capacity is executed
    //
    gUsbData->UsbGeometryCommandStatus |= READ_CAPACITY_COMMAND_EXECUTED;

    //
    // Max LBA & block size are updated in MassDeviceInfo structure
    //
    if (DevInfo->MaxLba < 0x4000) {    //  last LBA < 16MB
        switch (DevInfo->MaxLba) {
            case USB_144MB_FDD_MAX_LBA:
                gUsbData->UsbReadCapHeads     = USB_144MB_FDD_MAX_HEADS;
                gUsbData->UsbReadCapSectors   = USB_144MB_FDD_MAX_SECTORS;
                gUsbData->UsbReadCapCylinders = USB_144MB_FDD_MAX_CYLINDERS;
                gUsbData->DiskMediaType       = USB_144MB_FDD_MEDIA_TYPE;
                return  USB_SUCCESS;

            case USB_720KB_FDD_MAX_LBA:
                gUsbData->UsbReadCapHeads     = USB_720KB_FDD_MAX_HEADS;
                gUsbData->UsbReadCapSectors   = USB_720KB_FDD_MAX_SECTORS;
                gUsbData->UsbReadCapCylinders = USB_720KB_FDD_MAX_CYLINDERS;
                gUsbData->DiskMediaType       = USB_720KB_FDD_MEDIA_TYPE;
                return  USB_SUCCESS;
        }
    }

    //
    // Convert to CHS
    //
    //gUsbData->wReadCapBlockSize = DevInfo->BlockSize;

    //
    // Do CHS conversion
    // Use fixed sectors/track & heads for CHS conversion
    //
    if (DevInfo->MaxLba < 0x400) {   // < 512 KB
        Sectors    = 1;
        Heads      = 1;
    }
    else {
        if (DevInfo->MaxLba < 0x200000) {  // < 1GB
            Sectors    = USB_FIXED_LBA_SPT_BELOW_1GB;
            Heads      = USB_FIXED_LBA_HPT_BELOW_1GB;
        }
        else {                                  // > 1GB
            Sectors    = USB_FIXED_LBA_SPT_ABOVE_1GB;
            Heads      = USB_FIXED_LBA_HPT_ABOVE_1GB;
        }
    }

    gUsbData->UsbReadCapSectors  = Sectors;
    gUsbData->UsbReadCapHeads    = Heads;

    //
    // Calculate number of cylinders Cyl = LBA/(Head*Sec)
    //

    gUsbData->UsbReadCapCylinders = (UINT16)DivU64x64Remainder(DevInfo->MaxLba, (Sectors * Heads), NULL);

    return  USB_SUCCESS;

}


/**
    This function updates the device geometry information

    @param DevInfo Pointer to device info structure
    @retval USB_ERROR   On error
    @retval USB_SUCCESS On success

**/

UINT8
USBMassUpdateDeviceGeometry(
    DEV_INFO* DevInfo
)
{
    UINT64          MaxLba;
    UINT8           Heads;
    UINT8           Sectors;
    UINT16          Cylinders;


    //
    // Try to update geometry if it is not valid
    // "Valid" block size is 1...FFFE
    // Additional check added to ensure the head, sector, and cylinder values are non-zero.
    //
                            //(EIP13457+)>
    if ((DevInfo->Heads != 0) && 
        (DevInfo->Sectors != 0) &&
        (DevInfo->Cylinders != 0) &&
        !(DevInfo->LastStatus & USB_MASS_MEDIA_CHANGED) &&
        (!((DevInfo->LastStatus & USB_MASS_GET_MEDIA_FORMAT) &&
        (DevInfo->SubClass == SUB_CLASS_UFI)))) {

        DevInfo->LastStatus &= ~USB_MASS_GET_MEDIA_FORMAT;

        if (DevInfo->BlockSize && (DevInfo->BlockSize != 0xFFFF)) {
            return USB_SUCCESS;
        }
    }

    DevInfo->LastStatus &= ~USB_MASS_GET_MEDIA_FORMAT;

    //
    // Set default values for the global variables
    //
    gUsbData->DiskMediaType = USB_UNKNOWN_MEDIA_TYPE;
    gUsbData->UsbGeometryCommandStatus &= ~(MODE_SENSE_COMMAND_EXECUTED |
                    READ_CAPACITY_COMMAND_EXECUTED);

    //
    // Get disk geometry using Mode Sense
    //
    if (DevInfo->SubClass == SUB_CLASS_UFI) {  
        USBMassUpdateParamUsingModeSense(DevInfo);
    }

    //
    // Get disk geometry using Read Capacity
    //
    USBMassUpdateParamUsingReadCapacity(DevInfo);

    //
    // Parameters are obtained and stored in respective global variables;
    // check whether any of the commands executed.
    //
    if (!(gUsbData->UsbGeometryCommandStatus & (READ_CAPACITY_COMMAND_EXECUTED |
        MODE_SENSE_COMMAND_EXECUTED)))  {
        USB_DEBUG(DEBUG_ERROR, DEBUG_LEVEL_3, "-error\n");
        return USB_ERROR;
    }

    //
    // Check whether read capacity is executed. If so, then max LBA & block size
    // are already updated in the MassDeviceInfo structure. If not update it using
    // mode sense parameters
    //
    if (!(gUsbData->UsbGeometryCommandStatus & READ_CAPACITY_COMMAND_EXECUTED)) {
        //
        // At this point we made sure atleast one of the command (Mode sense or Read
        // Capacity) was executed. So if one command is not executed then other
        // command is surely executed.
        //

        //
        // Update the max LBA & block size using mode sense parameters
        //
        DevInfo->BlockSize = gUsbData->UsbModeSenseBlockSize;
        DevInfo->MaxLba = gUsbData->UsbModeSenseMaxLBA;
        USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "size %x lba %lx\n", DevInfo->BlockSize, DevInfo->MaxLba);
    }

    //Some usb mass storages report media change even if they don't, we already 
    //update CHS from boot record and legacy boot doesn't support dynamic 
    //media insertion, we should not update it from read capacity parameters.

    if ((DevInfo->Heads != 0) && (DevInfo->Sectors != 0) && (DevInfo->Cylinders !=0)) {
       return USB_SUCCESS;
    }

    //
    // Update the media type byte
    //
    DevInfo->MediaType = gUsbData->DiskMediaType;

    //
    // Check whether mode sense is executed. If so, then update CHS from mode
    // sense value or else update from read capacity values.
    //

    //
    // Update the CHS values using mode sense parameters
    //
    Heads      = gUsbData->UsbModeSenseHeads;
    Sectors    = gUsbData->UsbModeSenseSectors;
    Cylinders  = gUsbData->UsbModeSenseCylinders;

//  if ((gUsbData->UsbGeometryCommandStatus & MODE_SENSE_COMMAND_EXECUTED) &&
    if ((Heads * Sectors * Cylinders) == 0) {
        //
        // Update the CHS values using read capacity parameters
        //
        Heads      = gUsbData->UsbReadCapHeads;
        Sectors    = gUsbData->UsbReadCapSectors;
        Cylinders  = gUsbData->UsbReadCapCylinders;
    }

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "Cyl-%x, Hds-%x, Sec-%x", Cylinders, Heads, Sectors);

    DevInfo->Heads       = Heads;
    DevInfo->Sectors     = Sectors;
    DevInfo->Cylinders   = Cylinders;

    //
    // Calculate non-LBA CHS values from max LBA
    //
    MaxLba = DevInfo->MaxLba;

    //
    // Do not translate sectors for non HDD devices
    //
    if ((!DevInfo->StorageType) || (DevInfo->StorageType == USB_MASS_DEV_HDD)) {
        //
        // If Total number of sectors < 1032192(0FC000h) CHS translation is not
        // needed
        //
        if (MaxLba >= 0xFC000) {
            Sectors    = 63;
            Heads      = 32;
            //
            // If Total number of sectors < 2064384(01F8000h) then use
            // 63 Sec/track and 32 head for translation
            //
            if (MaxLba >= 0x01F8000) {
                Heads = 64;
                //
                // If Total number of sectors < 4128768(03F0000h) then use
                // 63 Sec/track and 64 head for translation
                //
                if (MaxLba >= 0x03F0000) {
                    Heads = 128;
                    //
                    // If Total number of sectors < 8257536(07E0000h) then use
                    // 63 Sec/track and 128 head for translation else use 255 heads
                    //
                    if (MaxLba >= 0x7E0000) {
                        Heads      = 255;
                        MaxLba = DevInfo->MaxLba;
                    }
                }
            }
        }

        //
        // In any case, check the parameters for maximum values allowed by BIOS and
        // ATA specs (that is, 1024 cylinders, 16 heads and 63 sectors per track)
        //
        for (;;) {
            //
            // Calculate translated number of cylinders
            //
            if ((Sectors != 0) && (Heads != 0)) {
                Cylinders = (UINT16)DivU64x64Remainder(MaxLba, (Heads * Sectors), NULL);
            } else {
                Cylinders = 0;
            }

            //
            // Check whether number of cylinders is less than or equal to 1024
            //
            if (Cylinders <= 1024) break;

            //
            // Cylinders are getting larger than usually supported try increasing
            // head count keeping cylinders within safe limit
            //
            Cylinders = 1024;
            if (Heads == 0xFF) {
                break;  // Heads limit reached
            }
            //
            // Double number of heads
            //
            Heads <<= 1;
            if (!Heads) {
                Heads = 0xFF;
            }
        }
    }

    //
    // Save the parameters
    //
    DevInfo->NonLbaHeads     = Heads;
    DevInfo->NonLbaSectors   = Sectors;
    DevInfo->NonLbaCylinders = Cylinders;

    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_5, "BPS %d H %d S %d C %d MT %d\n",
        DevInfo->BlockSize,
        DevInfo->Heads,
        DevInfo->Sectors,
        DevInfo->Cylinders,
        DevInfo->MediaType);

    return USB_SUCCESS;

}

/**
    This function performs a mass storage transaction using bulk
    only transport (BOT) protocol.

    @param DevInfo Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to mass transaction structure
                          CmdBuffer - Pointer to command buffer
                          CmdSize - Size of command block
                          XferDir - Transfer direction
                          FpBuffer - Data buffer far pointer
                          Length - Amount of data to be transferred
                          PreSkip - Number of bytes to skip before data
                          PostSkip - Number of bytes to skip after data

    @retval Data          Amount of data actually transferred

**/

UINT32
USBMassIssueBOTTransaction(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{
    UINT32  Data;
    UINT8   Status;

    Data = USBMassSendBOTCommand(DevInfo, MassXactStruc);   // Send the command control transfer

    if (!Data) {
        //
        // Check for stall/timedout condition
        //
        if (gUsbData->UsbLastCommandStatus & (USB_BULK_STALLED + USB_BULK_TIMEDOUT)) {
            //
            // Perform USB BOT reset recovery
            //
            USBMassBOTResetRecovery(DevInfo);
            return 0;
        }
        else {
            return 0;  // Unknown error exit
        }
    }

    if (!MassXactStruc->Length) {  // No data
        if (gUsbData->UsbBulkDataXferDelay) {
            //
            // Issue some delay
            //
            FixedDelay(100 * 1000);
            gUsbData->UsbBulkDataXferDelay   = 0;
        }
        //
        // Get the status for the last transfer
        //
        Data = USBMassGetBOTStatus(DevInfo, MassXactStruc);
        return Data;
    }

    //
    // Tranfer the bulk data
    //
    Data = USBMassProcessBulkData(DevInfo, MassXactStruc);  // Actual data size

    //
    // Check for stall/timeout condition
    //
    if (!(gUsbData->UsbLastCommandStatus & (USB_BULK_STALLED + USB_BULK_TIMEDOUT))) {
        //
        // Get the status for the last transfer
        //
        Status = USBMassGetBOTStatus(DevInfo, MassXactStruc);
                if ((Status == USB_ERROR) || (gUsbData->UsbLastCommandStatus & USB_BULK_TIMEDOUT)) {
                return 0;
                } else {
                return Data;
                }
    }

    //
    // Check for time out condition
    //
    if (gUsbData->UsbLastCommandStatus & USB_BULK_TIMEDOUT) {
        //
        // Perform USB BOT reset recovery
        //
        USBMassBOTResetRecovery(DevInfo);
        return 0;
    }

    //
    // Clear endpoint stall
    //
    USBMassClearBulkEndpointStall(DevInfo, MassXactStruc->XferDir);

    //
    // Get the status for the last transfer
    //
    USBMassGetBOTStatus(DevInfo, MassXactStruc);

    return Data;

}

/**
    This function performs a mass storage transaction using bulk
    only transport (BOT) protocol.

    @param FpDevInfo   Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to mass transaction structure
                          CmdBuffer - Pointer to command buffer
                          CmdSize - Size of command block
                          XferDir - Transfer direction
                          FpBuffer - Data buffer far pointer
                          Length - Amount of data to be transferred
                          PreSkip - Number of bytes to skip before data
                          PostSkip - Number of bytes to skip after data

    @retval data          Amount of data actually transferred

**/

UINT16
USBMassSendBOTCommand(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{
    UINT8           Count;
    UINT8           *Src;
    UINT8           *Dest;
    BOT_CMD_BLK     *BotCmdBlk;
    UINT8           CmdSize;

    BotCmdBlk = (BOT_CMD_BLK*)MassXactStruc->CmdBuffer;

    CmdSize = MassXactStruc->CmdSize;

    //
    // Make enough space for BOT command block wrapper
    // Move backwards
    //
    Src = MassXactStruc->CmdBuffer + CmdSize - 1;

    //
    // BOT_COMMAND_BLOCK + end of command
    //
    Dest = Src + ((UINT8*)BotCmdBlk->aCBWCB - (UINT8*)BotCmdBlk);

    for (Count = 0; Count < CmdSize; Count++) {
        *Dest = *Src;
        --Dest;
        --Src;
    }

    BotCmdBlk->dCbwSignature      = BOT_CBW_SIGNATURE;
    BotCmdBlk->dCbwTag            = ++(gUsbData->BotCommandTag);
    BotCmdBlk->dCbwDataLength     = MassXactStruc->Length;
    BotCmdBlk->bmCbwFlags         = MassXactStruc->XferDir;
    BotCmdBlk->bCbwLun            = DevInfo->Lun;
    BotCmdBlk->bCbwLength         = CmdSize;

    return (UINT16)USBMassIssueBulkTransfer(
                    DevInfo,
                    0,
                    (UINT8*)BotCmdBlk,
                    sizeof (BOT_CMD_BLK));
    
}

/**
    This function gets the BOT status sequence using
    bulk IN transfer

    @param DevInfo        Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to mass transaction structure
    
    @retval USB_SUCCESS   On success   
    @retval USB_ERROR     error

**/

UINT8
USBMassGetBOTStatus(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{

    UINT8*  CmdBuffer;
    UINT16  Data;

    CmdBuffer = MassXactStruc->CmdBuffer;

    Data = (UINT16)USBMassIssueBulkTransfer(DevInfo, BIT7,
                    CmdBuffer, sizeof (BOT_STATUS_BLOCK));
    if ((Data != sizeof (BOT_STATUS_BLOCK))) {
        if (gUsbData->UsbLastCommandStatus & USB_BULK_STALLED) {
            USBMassClearBulkEndpointStall(DevInfo, BIT7);
        }
        Data = (UINT16)USBMassIssueBulkTransfer(DevInfo, BIT7,
                    CmdBuffer, sizeof (BOT_STATUS_BLOCK));
        if (gUsbData->UsbLastCommandStatus & USB_BULK_STALLED) {
            USBMassBOTResetRecovery(DevInfo);
            return USB_ERROR;
        }
    }

    //
    // Check for valid CSW
    //
    if ((Data != sizeof (BOT_STATUS_BLOCK)) ||
        (((BOT_STATUS_BLOCK*)CmdBuffer)->dCswSignature != BOT_CSW_SIGNATURE) ||
        (((BOT_STATUS_BLOCK*)CmdBuffer)->dCswTag != gUsbData->BotCommandTag)) {
        //USBMassClearBulkEndpointStall(FpDevInfo, BIT7); 
        //USBMassClearBulkEndpointStall(FpDevInfo, BIT0); 
        return USB_ERROR;
    }

    //
    // Check for meaningful CSW
    //
    if (((BOT_STATUS_BLOCK*)CmdBuffer)->bmCswStatus) {
                if (((BOT_STATUS_BLOCK*)CmdBuffer)->bmCswStatus > 1) {
                //
                // Perform reset recovery if BOT status is phase error
                //
                USBMassBOTResetRecovery(DevInfo);
                }
                return USB_ERROR;
    }

    return USB_SUCCESS;
}

/**
    This function performs the BOT reset recovery

    @param  FpDevInfo   Pointer to DeviceInfo structure
    @retval None  
**/

VOID
USBMassBOTResetRecovery(DEV_INFO* FpDevInfo)
{
                                                                                                                                                          //<(EIP20863)
    AmiUsbControlTransfer(
        gHcTable[FpDevInfo->HcNumber - 1],
        FpDevInfo, ADSC_OUT_REQUEST_TYPE + (BOT_RESET_REQUEST_CODE << 8),
        (UINT16)FpDevInfo->InterfaceNum, 0, 0, 0);
        
    USBMassClearBulkEndpointStall(FpDevInfo, BIT7);
    USBMassClearBulkEndpointStall(FpDevInfo, BIT0);
}

/**
    This function gets the maximum logical unit number(LUN)
    supported by the device.  It is zero based value.

    @param DevInfo Pointer to DeviceInfo structure
    @retval MaxLun Max LUN supported

**/

UINT16
USBMassBOTGetMaxLUN(
    DEV_INFO* DevInfo
)
{
    UINT8   *Buffer = NULL;
    UINT8   MaxLun = 0;
    UINT16  Status;

    if (DevInfo->IncompatFlags & USB_INCMPT_GETMAXLUN_NOT_SUPPORTED) {
        return 0;
    }

    Buffer = USB_MemAlloc(1);
    ASSERT(Buffer);
    if (Buffer == NULL) {
       return 0;
    }
    Status = AmiUsbControlTransfer(
                 gHcTable[DevInfo->HcNumber - 1],
                 DevInfo, ADSC_IN_REQUEST_TYPE + (BOT_GET_MAX_LUN_REQUEST_CODE << 8),
                 DevInfo->InterfaceNum, 0, Buffer, 1);

    if (Status) {
      MaxLun = *Buffer;
    }
    USB_MemFree(Buffer, 1);

    return MaxLun;
}


/**
    This function performs a mass storage transaction using CBI
    or CB protocol.

    @param DevInfo   Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to mass transaction structure
                          CmdBuffer - Pointer to command buffer
                          CmdSize - Size of command block
                          XferDir - Transfer direction
                          FpBuffer - Data buffer far pointer
                          Length - Amount of data to be transferred
                          PreSkip - Number of bytes to skip before data
                          PostSkip - Number of bytes to skip after data

    @retval Data          Amount of data actually transferred

**/

UINT32
USBMassIssueCBITransaction(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc    
)
{
    UINT32  Data = 0;

    if (!(USBMassSendCBICommand(DevInfo, MassXactStruc))) {  // Returns 0 on error
        return  0;
    }

    if (MassXactStruc->Length) {
        Data = USBMassProcessBulkData(DevInfo, MassXactStruc);
        if (!Data) {
            if(gUsbData->UsbLastCommandStatus & USB_BULK_STALLED) {
                USBMassClearBulkEndpointStall(DevInfo, MassXactStruc->XferDir);
                return Data;
            }
        }
    }

    if (DevInfo->Protocol != PROTOCOL_CBI_NO_INT && DevInfo->IntInEndpoint != 0) {
        //
        // Bypass interrupt transaction if it is CB protocol
        //
        USBMassCBIGetStatus(DevInfo);
    }

    return Data;
}


/**
    This function performs a mass storage transaction using CBI
    or CB protocol.

    @param DevInfo   Pointer to DeviceInfo structure
    @param MassXactStruc  Pointer to mass transaction structure
                          CmdBuffer - Pointer to command buffer
                          CmdSize - Size of command block
                          XferDir - Transfer direction
                          FpBuffer - Data buffer far pointer
                          Length - Amount of data to be transferred
                          PreSkip - Number of bytes to skip before data
                          PostSkip - Number of bytes to skip after data

    @retval RetValue      Number of bytes actually transferred 
        

**/

UINT16
USBMassSendCBICommand(
    DEV_INFO*           DevInfo,
    MASS_XACT_STRUC*    MassXactStruc
)
{
    UINT16  RetValue;

    RetValue = AmiUsbControlTransfer(
                    gHcTable[DevInfo->HcNumber - 1],
                    DevInfo, ADSC_OUT_REQUEST_TYPE,
                    (UINT16)DevInfo->InterfaceNum, 0,
                    MassXactStruc->CmdBuffer,
                    (UINT16)MassXactStruc->CmdSize);
    return RetValue;

}


/**
    This function gets the status of the mass transaction
    through an interrupt transfer

    @param FpDevInfo    Pointer to DeviceInfo structure

    @retval RetValue    Return value from the interrupt transfer

**/

UINT16
USBMassCBIGetStatus(DEV_INFO*   FpDevInfo)
{

    if (gUsbDataList->InterruptStatus == NULL) {
        gUsbDataList->InterruptStatus = (UINT32*)USB_MemAlloc(1);
        if (gUsbDataList->InterruptStatus == NULL) {
            return 0;
        }
    }
    *gUsbDataList->InterruptStatus = 0;
    AmiUsbInterruptTransfer(
        gHcTable[FpDevInfo->HcNumber - 1],
        FpDevInfo, FpDevInfo->IntInEndpoint, 
        FpDevInfo->IntInMaxPkt, (UINT8*)gUsbDataList->InterruptStatus, 2);

    return ((UINT16)(*gUsbDataList->InterruptStatus));

}


/**
    This function executes a bulk transaction on the USB. The
    transfer may be either DATA_IN or DATA_OUT packets containing
    data sent from the host to the device or vice-versa. This
    function wil not return until the request either completes
    successfully or completes with error (due to time out, etc.)
    Size of data can be upto 64K

    @param FpDevInfo  DeviceInfo structure (if available else 0)
    @param XferDir    Transfer direction
                      Bit 7   : Data direction
                      0 Host sending data to device
                      1 Device sending data to host
                      Bit 6-0 : Reserved
    @param CmdBuffer  Buffer containing data to be sent to the device or
                      buffer to be used to receive data. Value in
    @param dSize      Length request parameter, number of bytes of data
                      to be transferred in or out of the host controller
    @retval data      Amount of data transferred

**/

UINT32
USBMassIssueBulkTransfer(DEV_INFO* FpDevInfo, UINT8 XferDir,
                    UINT8* CmdBuffer, UINT32 Size)
{

    return AmiUsbBulkTransfer(
               gHcTable[FpDevInfo->HcNumber -1],
               FpDevInfo, XferDir,
               CmdBuffer, Size);
}


/**
    This function fills and returns the mass get device geometry
    structure

    @param GetDevGeometry     Pointer to mass get geometry structure
                              SenseData - Sense data of the last command
                              NumHeads - Number of heads
                              NumCylinders - Number of cylinders
                              NumSectors - Number of sectors
                              BytesPerSector - Number of bytes per sector
                              MediaType - Media type
    @retval Return value

**/

UINT8
USBMassGetDeviceGeometry(
    MASS_GET_DEV_GEO *GetDevGeometry
 )
{
    DEV_INFO    *DevInfo;
    UINT8       DevAddr = GetDevGeometry->DevAddr;
    BOOLEAN     ValidGeo;
    MASS_GET_DEV_STATUS MassGetDevSts;

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);

    if ((!DevInfo) || (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT))) {   // Error
        return USB_ERROR;
    }

    MassGetDevSts.DevAddr = DevAddr;

    if (GetDevGeometry->Int13FuncNum == 0x20){
        DevInfo->LastStatus |= USB_MASS_GET_MEDIA_FORMAT;
    }
    if ((!DevInfo->BlockSize) || (DevInfo->BlockSize == 0xFFFF) ||
        (!(DevInfo->LastStatus & USB_MASS_MEDIA_PRESENT) ||
        (GetDevGeometry->Int13FuncNum == 0x20)) ) {
//      USBMassCheckDeviceReady(FpDevInfo);
        USBMassGetDeviceStatus(&MassGetDevSts);
    }        
    ValidGeo = (BOOLEAN)((DevInfo->BlockSize != 0xFFFF) && (DevInfo->BlockSize != 0));
    ValidGeo &= (DevInfo->LastStatus & USB_MASS_MEDIA_PRESENT);

    GetDevGeometry->BytesPerSector   = ValidGeo? DevInfo->BlockSize : 0;
    GetDevGeometry->LbaNumHeads       = ValidGeo? DevInfo->Heads : 0;
    GetDevGeometry->LbaNumSectors    = ValidGeo? DevInfo->Sectors : 1;
    GetDevGeometry->LbaNumCyls       = ValidGeo? DevInfo->Cylinders : 0;
    GetDevGeometry->NumHeads          = ValidGeo? DevInfo->NonLbaHeads : 0;
    GetDevGeometry->NumSectors       = ValidGeo? DevInfo->NonLbaSectors : 1;
    GetDevGeometry->NumCylinders     = ValidGeo? DevInfo->NonLbaCylinders : 0;
    GetDevGeometry->MediaType        = DevInfo->MediaType;
    GetDevGeometry->LastLBA           = ValidGeo? DevInfo->MaxLba : 0;
    GetDevGeometry->BpbMediaDesc      = ValidGeo? DevInfo->BpbMediaDesc : 0;



    USB_DEBUG(DEBUG_INFO, DEBUG_LEVEL_4, "BPS %d H %d S %d C %d MT %d\n",
                DevInfo->BlockSize,
                DevInfo->Heads,
                DevInfo->Sectors,
                DevInfo->Cylinders,
                DevInfo->MediaType);

    return USB_SUCCESS;

}

/**
    This function issues read capacity command to the mass
    device and returns the value obtained

    @param ReadCapacity  Pointer to the read capacity structure
                         DevAddr - USB device address of the device
                         
    @retval USB_SUCCESS  Finish to read capacity.
    @retval Others       On fail
**/

UINT8
USBMassReadCapacity(
    MASS_READ_CAPACITY *ReadCapacity
)
{
    DEV_INFO    *DevInfo;
    UINT8       DevAddr = ReadCapacity->DevAddr;

    DevInfo = USB_GetDeviceInfoStruc(USB_SRCH_DEV_INDX, 0, DevAddr, 0);

    if ((!DevInfo) || (!(DevInfo->Flag & DEV_INFO_DEV_PRESENT))) {   // Error
        return USB_ERROR;
    }

    return USBMassReadCapacity10Command(DevInfo);
}

/**
    This function swaps the bytes in dword: 0-3,1-2,2-1,3-0. Can be
    used for example in little endian->big endian conversions.

    @param Data      DWORD to swap
    @retval Value    Input value with the swapped bytes in it.

**/

UINT32 dabc_to_abcd(UINT32 Data)
{
    return (((Data & 0x000000FF) << 24)
            | ((Data & 0x0000FF00) << 8)
            | ((Data & 0x00FF0000) >> 8)
            | ((Data & 0xFF000000) >> 24));
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
