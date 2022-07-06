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

/** @file AmiUsbController.h
    AMI USB Driver Protocol definition

**/

#ifndef _AMI_USB_CONTROLLER_H
#define _AMI_USB_CONTROLLER_H

#include <Library/DebugLib.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/UsbPolicy.h>
#include <Protocol/BlockIo.h>
#include <IndustryStandard/Atapi.h>

#define EFI_USB_PROTOCOL_GUID    \
  { 0x2ad8e2d2, 0x2e91, 0x4cd1, 0x95, 0xf5, 0xe7, 0x8f, 0xe5, 0xeb, 0xe3, 0x16 }

#define AMI_USB_SMM_PROTOCOL_GUID    \
  { 0x3ef7500e, 0xcf55, 0x474f, 0x8e, 0x7e, 0x0, 0x9e, 0xe, 0xac, 0xec, 0xd2 }

typedef union {
    VOID  *UhciDescPtrs;
    VOID  *OhciDescPtrs;
    VOID  *EhciDescPtrs;
} DESC_PTRS;

typedef struct {
    UINT8      Flag;                    
    UINT8      ReportId;
    UINT16     UsagePage;
    UINT32     ReportCount;
    UINT8      ReportSize;
    UINT32     LogicalMin;
    UINT32     LogicalMax;
    UINT32     PhysicalMax;        //(EIP127014)
    UINT32     PhysicalMin;        //(EIP127014)
    UINT8      UnitExponent;       //(EIP127014)
    UINT16     UsageCount;
    UINT16     *Usages;
    UINT16     MaxUsages;
    UINT16     UsageMin;
    UINT16     UsageMax;
} HID_REPORT_FIELD;

typedef struct {
    UINT8      Flag;
    UINT16     AbsMaxX;                         
    UINT16     AbsMaxY;
    UINT16     ReportDescLen;
    UINT8      FieldCount;
    HID_REPORT_FIELD    **Fields;
} HID_REPORT;

typedef struct {
    VOID       **Buffer;
    UINT8      Head;
    UINT8      Tail;
    UINT8      MaxKey;
    UINT32     KeySize;
} KEY_BUFFER;

typedef struct _ABS_MOUSE{
    UINT8      ReportID;
    UINT8      ButtonStauts;
    UINT16     Xcoordinate;
    UINT16     Ycoordinate;
    UINT16     Pressure;
    UINT16     Max_X;
    UINT16     Max_Y;
} ABS_MOUSE;

#pragma pack(push, 1)

typedef struct {
    UINT8       DescLength;
    UINT8       DescType;
    UINT16      BcdUsb;
    UINT8       BaseClass;
    UINT8       SubClass;
    UINT8       Protocol;
    UINT8       MaxPacketSize0;
    UINT16      VendorId;
    UINT16      DeviceId;
    UINT16      BcdDevice;
    UINT8       MfgStr;
    UINT8       ProductStr;
    UINT8       SerialStr;
    UINT8       NumConfigs;
} DEV_DESC;

#pragma pack(pop)

typedef struct _HC_STRUC HC_STRUC;
typedef struct _DEV_INFO DEV_INFO;
typedef struct _DEV_DRIVER DEV_DRIVER;
typedef struct _URP_STRUC URP_STRUC;

#define AMI_USB_PORT_STAT_LOW_SPEED         0x00
#define AMI_USB_PORT_STAT_FULL_SPEED        0x01
#define AMI_USB_PORT_STAT_HIGH_SPEED        0x02

/**
    USB Host Controller structure

 Fields:   Name       Type    Description
------------------------------------------------------------
      HcNumber   UINT8   Host Controller number, 1-based
      HcType     UINT8   Host Controller Type, U/O/E HCI
      FrameList UINT32* Host Controller Frame List Address
      BaseAddress UINTN   Host Controller Base Address, memory (EHCI,OHCI) or IO (UHCI)
      NumPorts   UINT8   Number of root ports, 1-based
      BusDevFuncNum UINT16   PCI location, bus (Bits8..15), device (Bits3..7), function(bits0..2)
      fpIRQInfo   IRQ_INFO IRQ information
      DescPtrs  DESC_PTRS   Commonly used descriptor pointers, see definition of DESC_PTRS
      AsyncListSize  UINT16  Async. list size
      OpRegOffset UINT8  Operation region offset
      MaxBulkDataSize    UINT32 Maximum Bulk Transfer data size
      HcFlag     UINT32  Host Controller flag
      ExtCapPtr  UINT8   EHCI Extended Capabilities Pointer
      bRegOfs     UINT8   EHCI Capabilities PCI register Offset
      DebugPort   UINT8   Port number of EHCI debug port
      usbbus_data VOID*   USB Bus data specific to this Host Controller
      Controller  EFI_HANDLE  EFI Handle of this controller
      HcDevicePath   EFI_DEVICE_PATH_PROTOCOL* Pointer to this controller's device path
**/

struct _HC_STRUC {
    UINT8           HcNumber;
    UINT8           HcType;
    UINT64          OrgPciAttributes;
    UINT32          *FrameList;
    UINTN           BaseAddress;
    UINTN           BaseAddressSize;
    UINT8           NumPorts;
    UINT16          BusDevFuncNum;
    UINT8           Irq;
    DESC_PTRS       DescPtrs;
    UINT16          AsyncListSize;
    UINT8           OpRegOffset;
    UINT32          MaxBulkDataSize;
    UINT32          HcsParams;
    UINT32          HccParams;
    UINT32          HcFlag;
    UINT8           ExtCapPtr; // EHCI Extended Capabilities Pointer
    UINT8           DebugPort;
    VOID*           Usb3HcData;
    EFI_HANDLE      Controller;
    EFI_DEVICE_PATH_PROTOCOL *HcDevicePath;
    UINT8           PwrCapPtr;  //(EIP54018+)
    VOID            *PciIo;
    UINT16          Vid;
    UINT16          Did;
    EFI_HANDLE      HwSmiHandle;
    UINT16          SplitPeriodicIndex;
    VOID            *IsocTds;

    UINT32          *FrameListPhyAddr;
    VOID            *FrameListMapping;
    UINT32          MemPoolPages;
    UINT8           *MemPoolPhyAddr;
    VOID            *MemPoolMapping;  
    UINT8           *MemPool;
    UINT32          MemBlkStsBytes;
    UINT32          *MemBlkSts;
    UINT8           *MemBlockStartPhy;
    VOID            *MemBlockStartMapping;
    UINT8           *UsbMassConsumePhy;
};

// Isochronous transfer related fields of DEV_INFO
typedef struct _DEVINFO_ISOC_DETAILS DEVINFO_ISOC_DETAILS;
struct _DEVINFO_ISOC_DETAILS {
    UINT8       Endpoint;
    UINT16      EpMaxPkt;
    UINT8       EpMult;
    UINT32      *XferDetails;
    UINT8       *AsyncStatus;
    UINTN       XferStart;
    UINTN       XferRing;
    UINTN       XferKey;
};

#define USB_NAME_STRING_64  64
/**
    USB Device Information Structure

     Name          Type      Description
------------------------------------------------------------
  Flag             UINT8     Device Information flags
  DeviceType       UINT8     Device Type
  VendorId         UINT16    Device VID
  DeviceId         UINT16    Device DID
  DeviceAddress    UINT8     Device USB Address
  HcNumber         UINT8     Host Controller Number this device is attached to
  HubDeviceNumber  UINT8     USB Hub Device Number this device is attached to
  HubPortNumber    UINT8     USB Hub Port Number this device is attached to
  bEndpointNum     UINT8     Endpoint number
  EndpointSpeed    UINT8     Endpoint speed
  Lun              UINT8     Device Logical Unit number
  Endp0MaxPacket   UINT16    Endpoint0 max packet size, in Bytes
  bNumConfigs      UINT8     Number of configurations
  ConfigNum        UINT8     Active configuration number (0-based)
  InterfaceNum     UINT8     Active interface number
  AltSettingNum    UINT8     Alternate setting number (0-based)
  CallBackIndex    UINT8     Callback function index
  PollTdPtr        UINT8*    Polling TD pointer
  fpPollTEPtr      UINT8*    Polling ED pointer
  HubNumPorts      UINT8     Hub # of ports (USB hubs only)
  HubPowerOnDelay  UINT8     Hub power-on delay (USB hubs only)
  Lun0DevInfoPtr   DEV_INFO* Pointer to Lun0 device (for multiple-LUN devices)
  DataInSync       UINT16    toggle tracking information
  DataOutSync      UINT16    toggle tracking information
  StorageType      UINT8     USB_MASS_DEV_ARMD, USB_MASS_DEV_HDD, etc.
  BulkInEndpoint   UINT8     Bulk-In endpoint number
  BulkOutEndpoint  UINT8     Bulk-Out endpoint number
  Protocol         UINT8     Protocol
  EmulationOption  UINT16    USB Mass Storage Drive Emulation Option, from Setup
  HiddenSectors    UINT8     Number of hidden sectors, for USB mass storage devices only
  SubClass         UINT8     Device sub-class
  BlockSize        UINT16    USB Mass Storage Device block size, in Bytes
  Sectors          UINT8     USB Mass Storage Device # of sectors
  Cylinders        UINT16    USB Mass Storage Device # of cylinders
  NonLbaSectors    UINT8     USB Mass Storage Device # of sectors reported in Non-LBA (CHS) functions
  NonLbaCylinders  UINT16    USB Mass Storage Device # of cylinders reported in Non-LBA (CHS) functions
  EmuType          UINT8     USB Mass Storage Device emulation type
  PhyDevType       UINT8     USB Mass Storage Device physical type
  MediaType        UINT8     USB Mass Storage Device media type
  BulkInMaxPkt     UINT16    USB Mass Storage Device Bulk-In max packet size, in Bytes
  BulkOutMaxPkt    UINT16    USB Mass Storage Device Bulk-Out max packet size, in Bytes
  IncompatFlags    UINT16    USB Mass Storage Device Incompatibility flags
  MassDev          VOID*     USB Mass Storage Device EFI handle
  fpDeviceDriver   DEV_DRIVER* Device driver pointer
  LastStatus       UINT8     Last transaction status
  ExtraData        UINT8*    Pointer to extra device specific data
  Handle           UINT32[2] USB Device Handle
**/
struct _DEV_INFO {
    UINT32      Flag;
    UINT8       DeviceType;
    UINT16      VendorId;
    UINT16      DeviceId;
    UINT8       DeviceAddress;

    UINT8       HcNumber;
    UINT8       HubDeviceNumber;
    UINT8       HubPortNumber;
    UINT8       EndpointSpeed;
    UINT8       Lun;
    UINT16      Endp0MaxPacket;

    UINT8       ConfigNum;
    UINT8       InterfaceNum;
    UINT8       AltSettingNum;

    UINT8       CallBackIndex;
    UINT8       *PollTdPtr;
    UINT8       *PollEdPtr;
    UINT8        *BosDescPtr;
    UINT16      BosTotalLength;
    UINT8       HubNumPorts;
    UINT8       HubPowerOnDelay;

    DEV_INFO    *Lun0DevInfoPtr;

    UINT16      DataInSync;
    UINT16      DataOutSync;
    UINT8       StorageType;   //USB_MASS_DEV_ARMD, USB_MASS_DEV_HDD, etc.
    UINT16      IntInMaxPkt;
    UINT8       IntInEndpoint;
    UINT16      IntOutMaxPkt;
    UINT8       IntOutEndpoint;
    UINT8       BulkInEndpoint;
    UINT8       BulkOutEndpoint;

    UINT8       BaseClass;     // BASE_CLASS_HID, BASE_CLASS_MASS_STORAGE or BASE_CLASS_HUB
    UINT8       SubClass;
    UINT8       Protocol;
    UINT16      EmulationOption;
    UINT8       HiddenSectors;

    UINT16      BlockSize;
    UINT64      MaxLba;
    UINT16      Heads;
    UINT8       Sectors;
    UINT16      Cylinders;
    UINT16      NonLbaHeads;
    UINT8       NonLbaSectors;
    UINT16      NonLbaCylinders;
    UINT8       EmuType;
    UINT8       PhyDevType;
    UINT8       MediaType;

    UINT16      BulkInMaxPkt;
    UINT16      BulkOutMaxPkt;
    UINT16      IncompatFlags;

    UINT64      DetectStartTime;
    UINT64      DetectEndTime;
    UINT64      ConfigStartTime;

    UINTN       DevDriverIndex;
    UINT8       LastStatus; 
    UINT8       *ExtraData;        
    UINT32      Handle[2];
    UINT8       DevNameString[USB_NAME_STRING_64];
    VOID        *DevMiscInfo;
    UINT8       HubDepth;
    UINT8       *PollDataBuffer;        //Polling Data Buffer    //(EIP54782+)
    
    HID_REPORT  HidReport;
    UINT64      HidDevType;
    UINT8       PollInterval;          //(EIP84455+)
    UINT16      PollingLength;
    UINT16      HubPortConnectMap;
    UINT8       BpbMediaDesc;
    KEY_BUFFER  KeyCodeBuffer;
    KEY_BUFFER  UsbKeyBuffer;
    ABS_MOUSE   AbsMouseData;
    DEVINFO_ISOC_DETAILS    IsocDetails;
    DEV_DESC    DevDesc;
    VOID        *SpecificDevData;
    UINT16      BusErrorStaus;
};
#define BUS_ERROR_DEVICE_CONFIG_DESCRIPTOR          BIT0    //Get Device Config Descriptor error
#define BUS_ERROR_ENDPOINT_DESCRIPTOR               BIT1    //Get Endpoint Descriptor error
#define BUS_ERROR_GROUP_FOUND_IN_TREE               BIT2    //Get Endpoint Descriptor error 
#define BUS_NO_ERROR_DEVICE_CONFIG_DESCRIPTOR       0x0000  // 

/**
    USB Device Driver Structure

 Fields:   Name       Type    Description
      ------------------------------------------------------------
  DevType    UINT8   Device Type
  BaseClass  UINT8   Device Base Type
  SubClass   UINT8   Device Subclass
  Protocol   UINT8   Device Protocol
  FnDeviceInit   VOID    Device Initialization Function
  FnCheckDeviceType  UINT8   Check Device Type Function
  FnConfigureDevice  DEV_INFO*   Configure Device Function
  FnDisconnectDevice UINT8   Disconnect Device Function
**/

struct _DEV_DRIVER {
    UINT8           DevType;
    UINT8           BaseClass;
    UINT8           SubClass;
    UINT8           Protocol;
    VOID            (EFIAPI *FnDeviceInit)(VOID);
    UINT8           (EFIAPI *FnCheckDeviceType)(DEV_INFO*, UINT8, UINT8, UINT8);
    DEV_INFO*       (EFIAPI *FnConfigureDevice)(HC_STRUC*, DEV_INFO*, UINT8*, UINT16, UINT16);
    UINT8           (EFIAPI *FnDisconnectDevice)(DEV_INFO*);
    VOID            (EFIAPI *FnDriverRequest)(DEV_INFO*, URP_STRUC*);
};

#pragma pack(push, 1)

/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call CheckPresence (API #0)

**/

typedef struct {
    UINT16      BiosRev;       // USB BIOS Revision
    UINT8       BiosActive;    // USB BIOS active/inactive
    UINT8       NumBootDev;    // # of USB boot device
    UINT16      UsbDataArea;   // USB Data area
    UINT8       NumKeyboards;  // Number of USB keyboards present
    UINT8       NumMice;       // Number of USB mice present
    UINT8       NumPoint;      // Number of USB point present                          //<(EIP38434+)
    UINT8       NumHubs;       // Number of USB hubs present
    UINT8       NumStorage;    // Number of USB storage devices present
///////// DO NOT ADD ANY FIELD HERE. IF IT IS NECESSARY PLEASE UPDATE THE CODE
///////// IN THE FUNCTION USBWrap_GetDeviceCount in the file USBWRAP.ASM
    UINT8       NumHarddisk;   // Number of hard disk emulated USB devices
    UINT8       NumCDROM;      // Number of CDROM emulated USB devices
    UINT8       NumFloppy;     // Number of floppy emulated USB devices
} CK_PRESENCE;


/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call StartHC and MoveDataArea (API #20 & #24)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DataAreaFlag   UINT16  Indicates which data area to use

**/

typedef struct {
    UINT16    DataAreaFlag;      // Data area to use
} START_HC;


/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call GetDeviceInfo (API #25)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevNumber  UINT8   Device # whose info is requested
      HcNumber   UINT8   HC # to which this device is connected (0 if no such device found)
      DevType    UINT8   Device type (0 if no such device found)

**/

typedef struct {
    UINT8   DevNumber;
    UINT8   HcNumber;
    UINT8   DevType;
} GET_DEV_INFO;


/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call CheckDevicePresence (API #26)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevType     UINT8           Type of device to look for
      HcStruc     FPHC_STRUC      Pointer to HC being checked for device connection
      Number      UINT8           Number of devices connected

**/

typedef struct {
    UINT8       DevType;
    HC_STRUC    *HcStruc;
    UINT8       Number;
} CHK_DEV_PRSNC;


typedef struct {
    UINT8   ScrLock:    1;
    UINT8   NumLock:    1;
    UINT8   CapsLock:   1;
    UINT8   Resrvd:     5;
} LED_MAP;

/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call LightenKeyboardLeds(API #2B)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      LedMapPtr         UINT32          32-bit Pointer to LED_MAP structure
                DevInfoPtr      UINT32          32-bit Pointer to DEV_INFO structure

**/

typedef struct {
    UINT32    LedMapPtr;
    UINT32    DevInfoPtr;
} KB_LEDS_DATA;

/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call LightenKeyboardLeds(API #2B)

    Fields:   Name          Type        Description
              ------------------------------------------------------------
              LedMapPtr     UINT32      32-bit Pointer to LED_MAP structure
              DevInfoPtr    UINT32      32-bit Pointer to DEV_INFO structure

**/

typedef struct {
        UINTN   LedMapPtr;
        UINTN   DevInfoPtr;
} EFI_KB_LEDS_DATA;


/**
    This is a URP (USB Request Packet) structure for the BIOS
    API call SecurityInterface (API #2Ah)

 Fields:        Name           Type            Description
               ------------------------------------------------------------
               Buffer        FAR     Buffer pointer to read/write data
               Length         UINT32   Length of the buffer
               WaitTime       UINT32   Wait time for the transaction in msec

**/

typedef struct {
        UINT32  Buffer;
        UINT32  Length;
        UINT32  WaitTime;
} SECURITY_IF;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassGetDeviceInfo (API #27h, SubFunc 00h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr     UINT8   USB device address of the device
      SenseData   UINT32  Sense data of the last command
      DevType     UINT8   Device type byte (HDD, CD, Removable)
      EmuType     UINT8   Emulation type used
      DevId       UINT32  Far pointer to the device ID
**/

typedef struct {
    UINT8       DevAddr;       // (Return value)
    UINT32      SenseData;     // USB Sense data
    UINT8       DevType;       // Device type
    UINT8       EmuType;       // Emulation type
    UINT32      DevId;        // Far ptr to the device id
// DO NOT ADD OR DELETE ANY FIELD ABOVE - This should match the MASS_INQUIRY
// structure for proper working
    UINT8       TotalMassDev;  // TotalNumber of devices
    UINT8       Reserved;
    UINT16      PciInfo;       // PCI Bus/Dev/Func number of HC the device is connected to
    UINT32      Handle[2];      // Device handle
} MASS_GET_DEV_INFO;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MASS_GET_DEV_STATUS (API #27h, SubFunc XXh)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      DeviceStatus   UINT8   Connection status of the Mass device
**/

typedef struct {
    UINT8       DevAddr;
    UINT8       DeviceStatus;
} MASS_GET_DEV_STATUS;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassGetDeviceGeometry (API #27h,
    SubFunc 01h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      NumHeads   UINT8   Number of heads
      NumCylinders   UINT16  Number of cylinders
      NumSectors UINT8   Number of sectors
      LbaNumHeads    UINT8   Number of heads (for INT13h function 48h)
      LbaNumCyls UINT16  Number of cylinders (for INT13h function 48h)
      LbaNumSectors  UINT8   Number of sectors (for INT13h function 48h)
      wUINT8sPerSector    UINT16  Number of bytes per sector
      MediaType  UINT8   Media type
      dLastLBA    UINT32  Last LBA address
**/

typedef struct {
    UINT8           DevAddr;   // (Return value)
    UINT32          SenseData; // USB Sense data
    UINT16          NumHeads;
    UINT16          NumCylinders;
    UINT8           NumSectors;
    UINT16          LbaNumHeads;
    UINT16          LbaNumCyls;
    UINT8           LbaNumSectors;
    UINT16          BytesPerSector;
    UINT8           MediaType;
    UINT64          LastLBA;
    UINT8           Int13FuncNum;      //(EIP13457+)
    UINT8           BpbMediaDesc;
} MASS_GET_DEV_GEO;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassResetDevice (API #27h, SubFunc 02h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
} MASS_RESET;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call USBMass_AssignDriveNumber
    (API #27h, SubFunc 0Eh)

 Fields:   Name       Type    Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      LogDevNum  UINT8   Logical Drive Number to assign to the device
      Heads      UINT8   Number of heads
      Sectors    UINT8   Number of sectors/track
      Cylinders  UINT16  Number of cylinders
      BlockSize  UINT16  Sector size in bytes
      Lun        UINT8   Maximum LUNs in the system
      Speed      UINT8   <>0 if the device is hi-speed device

**/

typedef struct {
    UINT8       DevAddr;   // USB Device Address
    UINT8       LogDevNum; // Logical Drive Number to assign to the device
    UINT8       Heads;
    UINT8       Sectors;
    UINT16      Cylinders;
    UINT16      BlockSize;
    UINT8       Lun;
    UINT8       Speed;
} MASS_ASSIGN_DRIVE_NUM;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassReadDevice (API #27h, SubFunc 03h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      dStartLBA   UINT32  Starting LBA address
      wNumBlks    UINT16  Number of blocks to read
      wPreSkipSize    UINT16  Number of bytes to skip before
      wPostSkipSize   UINT16  Number of bytes to skip after
      fpBufferPtr UINT32  Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to read
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINT32      BufferPtr;      // Far buffer pointer
} MASS_READ;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassWriteDevice (API #27h, SubFunc 04h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      dStartLBA   UINT32  Starting LBA address
      wNumBlks    UINT16  Number of blocks to write
      wPreSkipSize    UINT16  Number of bytes to skip before
      wPostSkipSize   UINT16  Number of bytes to skip after
      fpBufferPtr UINT32  Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to write
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINT32      BufferPtr;      // Far buffer pointer
} MASS_WRITE;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassVerifyDevice (API #27h, SubFunc 05h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      dStartLBA   UINT32  Starting LBA address
      wNumBlks    UINT16  Number of blocks to verify
      wPreSkipSize    UINT16  Number of bytes to skip before
      wPostSkipSize   UINT16  Number of bytes to skip after
      fpBufferPtr UINT32  Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to verify
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINT32      BufferPtr;      // Far buffer pointer
} MASS_VERIFY;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassReadDevice (API #27h, SubFunc 0Ch)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr         UINT8   USB device address of the device
      SenseData       UINT32  Sense data of the last command
      StartLBA        UINT64  Starting LBA address
      NumBlks         UINT16  Number of blocks to write
      PreSkipSize     UINT16  Number of bytes to skip before
      PostSkipSize    UINT16  Number of bytes to skip after
      BufferPtr       UINTN   Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to read
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINTN       BufferPtr;      // Far buffer pointer
} EFI_MASS_READ;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassWriteDevice (API #27h, SubFunc 0Dh)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr         UINT8   USB device address of the device
      SenseData       UINT32  Sense data of the last command
      StartLBA        UINT64  Starting LBA address
      NumBlks         UINT16  Number of blocks to write
      PreSkipSize     UINT16  Number of bytes to skip before
      PostSkipSize    UINT16  Number of bytes to skip after
      BufferPtr       UINTN   Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to write
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINTN       BufferPtr;      // Far buffer pointer
} EFI_MASS_WRITE;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassVerifyDevice (API #27h, SubFunc 0Eh)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr         UINT8   USB device address of the device
      SenseData       UINT32  Sense data of the last command
      StartLBA        UINT64  Starting LBA address
      NumBlks         UINT16  Number of blocks to verify
      PreSkipSize     UINT16  Number of bytes to skip before
      PostSkipSize    UINT16  Number of bytes to skip after
      BufferPtr       UINTN   Far buffer pointer

**/

typedef struct {
    UINT8       DevAddr;        // USB Device Address
    UINT32      SenseData;      // USB Sense data
    UINT64      StartLba;       // Starting LBA address
    UINT16      NumBlks;        // Number of blocks to verify
    UINT16      PreSkipSize;    // Number of bytes to skip before
    UINT16      PostSkipSize;   // Number of bytes to skip after
    UINTN       BufferPtr;      // Far buffer pointer
} EFI_MASS_VERIFY;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassFormatDevice (API #27h, SubFunc 06h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      bHeadNumber UINT8   Head number to format
      bTrackNumber    UINT8   Track number to format

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
    UINT8       bHeadNumber;    // Head number to format
    UINT8       bTrackNumber;   // Track number to format
} MASS_FORMAT;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassRequestSense (API #27h, SubFunc 07h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
} MASS_REQ_SENSE;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassTestUnitReady (API #27h, SubFunc 08h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
} MASS_TEST_UNIT_RDY;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassStartStopUnit (API #27h, SubFunc 09h)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      Command    UINT8   0 - Stop, 1 - Start

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
    UINT8       Command;       // 0 - Stop, 1 - Start
} MASS_START_STOP_UNIT;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassReadCapacity (API #27h, SubFunc 0Ah)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      MaxLBA     UINT32  Maximum LBA address
      BlockSize  UINT32  Block size

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
    UINT32      MaxLBA;        // Max LBA address
    UINT32      BlockSize;     // Block size
} MASS_READ_CAPACITY;


/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassModeSense (API #27h, SubFunc 0Bh)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      NumHeads   UINT8   Number of heads
      NumCylinders   UINT16  Number of cylinders
      NumSectors UINT8   Number of sectors
      BytesPerSector UINT16  Number of bytes per sector
      MediaType  UINT8   Media type

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
    UINT8       NumHeads;      // Number of heads
    UINT16      NumCylinders;  // Number of cylinders
    UINT8       NumSectors;    // Number of sectors
    UINT16      BytesPerSector;// Number of bytes per sector
    UINT8       MediaType;     // Media type
} MASS_MODE_SENSE;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassInquiry (API #27h, SubFunc 0Ch)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    UINT8   USB device address of the device
      SenseData  UINT32  Sense data of the last command
      DevType    UINT8   Device type byte (HDD, CD, Removable)
      EmuType    BYTE    Emulation type used
      DevId      UINT32  Far pointer to the device ID

**/

typedef struct {
    UINT8       DevAddr;       // USB Device Address
    UINT32      SenseData;     // USB Sense data
    UINT8       DevType;       // Device type
    UINT8       EmuType;       // Emulation type
    UINT32      DevId;        // Far ptr to the device id
// DO NOT ADD OR DELETE ANY FIELD ABOVE - This should match the
// MASS_GET_DEV_INFO structure for proper working
} MASS_INQUIRY;

typedef struct {
    DEV_INFO        *FpDevInfo;
    MASS_INQUIRY    *FpInqData;
} MASS_GET_DEV_PARMS;

typedef struct{
    DEV_INFO            *DevInfo;
    ATA_IDENTIFY_DATA   *Identify;
} MASS_GET_IDENTIFY_DATA;

typedef struct {
    DEV_INFO*   FpDevInfo;
} MASS_CHK_DEV_READY;

/**
    This is a Mass URP (Mass USB Request Packet) structure for
    the BIOS API call MassCmdPassThru command (API #27h,
    SubFunc 0Dh)

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      DevAddr    BYTE    USB device address of the device
      SenseData  UINT32  Sense data of the last command
      CmdBuffer UINT32  Far pointer to the command buffer
      CmdLength  UINT16  Command length
      DataBuffer    UINT32  Far pointer for data buffer
      DataLength UINT16  Data length
      XferDir    BYTE    Data transfer direction

**/

typedef struct {
    UINT8       DevAddr;
    UINT32      SenseData;
    UINT32      CmdBuffer;
    UINT16      CmdLength;
    UINT32      DataBuffer;
    UINT16      DataLength;
    UINT8       XferDir;
} MASS_CMD_PASS_THRU;

/**
    N/A

**/
typedef struct _HCPROC_PARAM HCPROC_PARAM;
struct _HCPROC_PARAM{
    VOID*       ParamBuffer; //parameters as they should apear in stack of
                            // of the corresponding function invocation
    unsigned    HcType;
    unsigned    ParamSize;
    UINTN       RetVal;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call core command (API #2eh )
**/
typedef struct _COREPROC_PARAM COREPROC_PARAM;
typedef struct _COREPROC_PARAM *FPCOREPROC_PARAM;
struct _COREPROC_PARAM{
    VOID*       ParamBuffer; //parameters as they should apear in stack of
                            // of the corresponding function invocation
    unsigned    ParamSize;
    UINTN       RetVal;
};

/**
    This is a URP structure for the BIOS API(API #32h)
**/

typedef struct {
    UINT16  Vid;            // Vendor Id
    UINT16  Did;            // Device Id
    UINT8   DevAddr;        // USB Device Address
} GET_DEV_ADDR;

/**
    This is a URP structure for the BIOS API(API #36)
**/

typedef struct {
    UINT8       Start;
    HC_STRUC    *HcStruc;
} HC_START_STOP;

#define STOP_HOST_CONTROLLER      0x00
#define START_HOST_CONTROLLER     0x01
#define REGISTER_USB_SMI          0x02

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_GETSMARTCLASSDESCRIPTOR CCID_GETSMARTCLASSDESCRIPTOR;
typedef struct _CCID_GETSMARTCLASSDESCRIPTOR *FPCCID_GETSMARTCLASSDESCRIPTOR;
struct _CCID_GETSMARTCLASSDESCRIPTOR{
    OUT UINTN            ResponseBuffer;
    IN  UINT8            Slot;
    OUT UINTN            FpDevInfo;            
};


/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_ATR CCID_ATR;
typedef struct _CCID_ATR *FPCCID_ATR;
struct _CCID_ATR{
    IN UINT8            Slot;
    IN OUT UINTN        ATRData;
    OUT UINTN           FpDevInfo;
};


/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_POWERUP_SLOT CCID_POWERUP_SLOT;
typedef struct _CCID_POWERUP_SLOT *FPCCID_POWERUP_SLOT;
struct _CCID_POWERUP_SLOT{
    IN UINT8             Slot;
    OUT UINT8            bStatus;
    OUT UINT8            bError;
    IN OUT UINTN         ATRData;
    OUT UINTN            FpDevInfo;                        
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_POWERDOWN_SLOT CCID_POWERDOWN_SLOT;
typedef struct _CCID_POWERDOWN_SLOT *FPCCID_POWERDOWN_SLOT;
struct _CCID_POWERDOWN_SLOT{
    IN  UINT8            Slot;
    OUT UINT8            bStatus;
    OUT UINT8            bError;
    OUT UINTN            FpDevInfo;            
};


/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_GETSLOT_STATUS CCID_GETSLOT_STATUS;
typedef struct _CCID_GETSLOT_STATUS *FPCCID_GETSLOT_STATUS;
struct _CCID_GETSLOT_STATUS{
    OUT UINT8            bStatus;
    OUT UINT8            bError;
    OUT UINT8            bClockStatus;    
    IN  UINT8            Slot;
    OUT UINTN            FpDevInfo;            
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_CCID_XFRBLOCK CCID_XFRBLOCK;
typedef struct _CCID_CCID_XFRBLOCK *FPCCID_XFRBLOCK;
struct _CCID_CCID_XFRBLOCK{
    IN UINTN             CmdLength;
    IN UINTN             CmdBuffer;
    IN UINT8             ISBlock;
    OUT UINT8            bStatus;
    OUT UINT8            bError;
    IN OUT UINTN         ResponseLength;
    OUT UINTN            ResponseBuffer;
    IN  UINT8            Slot;
    OUT UINTN            FpDevInfo;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _CCID_GET_PARAMS CCID_GET_PARAMS;
typedef struct _CCID_GET_PARAMS *FPCCID_GET_PARAMS;
struct _CCID_GET_PARAMS{
    OUT UINT8            bStatus;
    OUT UINT8            bError;
    IN OUT UINTN         ResponseLength;
    OUT UINTN            ResponseBuffer;
    IN  UINT8            Slot;
    OUT UINTN            FpDevInfo;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_CONNECT SMART_CARD_READER_CONNECT;
typedef struct _SMART_CARD_READER_CONNECT *FPSMART_CARD_READER_CONNECT;
struct _SMART_CARD_READER_CONNECT{
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo;
    IN  UINT32           AccessMode;
    IN  UINT32           CardAction;
    IN  UINT32           PreferredProtocols;
    OUT UINTN            ActiveProtocol;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_DISCONNECT SMART_CARD_READER_DISCONNECT;
typedef struct _SMART_CARD_READER_DISCONNECT *FPSMART_CARD_READER_DISCONNECT;
struct _SMART_CARD_READER_DISCONNECT{
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo; 
    IN  UINT32           CardAction;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_STATUS SMART_CARD_READER_STATUS;
typedef struct _SMART_CARD_READER_STATUS *FPSMART_CARD_READER_STATUS;
struct _SMART_CARD_READER_STATUS {
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo;
    OUT UINTN            ReaderName;
    IN OUT UINTN         ReaderNameLength;
    OUT UINTN            State;
    OUT UINTN            CardProtocol;
    OUT UINTN            Atr;
    IN OUT UINTN         AtrLength;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_TRANSMIT SMART_CARD_READER_TRANSMIT;
typedef struct _SMART_CARD_READER_TRANSMIT *FPSMART_CARD_READER_TRANSMIT;
struct _SMART_CARD_READER_TRANSMIT{
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo;
    IN  UINTN            CAPDU;
    IN  UINTN            CAPDULength;
    OUT UINTN            RAPDU;
    IN OUT UINTN         RAPDULength;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_CONTROL SMART_CARD_READER_CONTROL;
typedef struct _SMART_CARD_READER_CONTROL *FPSMART_CARD_READER_CONTROL;
struct _SMART_CARD_READER_CONTROL{
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo;
    IN UINT32            ControlCode;
    IN UINTN             InBuffer;
    IN UINTN             InBufferLength;
    OUT UINTN            OutBuffer;
    IN OUT UINTN         OutBufferLength;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a Core Procedure URP  structure for
    the BIOS API call USB_API_CCID_DEVICE_REQUEST command (API #2Fh )
**/
typedef struct _SMART_CARD_READER_GETATTRIB SMART_CARD_READER_GETATTRIB;
typedef struct _SMART_CARD_READER_GETATTRIB *FPSMART_CARD_READER_GETATTRIB;
struct _SMART_CARD_READER_GETATTRIB{
    IN  UINT8            Slot;
    IN  UINTN            FpDevInfo;
    IN UINT32            Attrib;
    OUT UINTN            OutBuffer;
    IN OUT UINTN         OutBufferLength;
    OUT EFI_STATUS       EfiStatus;
};

/**
    This is a union data type of all the API related data

**/

typedef union {
    CK_PRESENCE             CkPresence;
    START_HC                StartHc;
    GET_DEV_INFO            GetDevInfo;
    CHK_DEV_PRSNC           ChkDevPrsnc;
    SECURITY_IF             SecurityIf;
    MASS_GET_DEV_INFO       MassGetDevInfo;
    MASS_GET_DEV_STATUS     MassGetDevSts;
    MASS_GET_DEV_GEO        MassGetDevGeo;
    MASS_RESET              MassReset;
    MASS_READ               MassRead;
    MASS_WRITE              MassWrite;
    MASS_VERIFY             MassVerify;
    MASS_FORMAT             MassFormat;
    MASS_REQ_SENSE          MassReqSense;
    MASS_TEST_UNIT_RDY      MassTstUnitRdy;
    MASS_START_STOP_UNIT    MassStartStop;
    MASS_READ_CAPACITY      MassReadCap;
    MASS_MODE_SENSE         MassModeSense;
    MASS_INQUIRY            MassInquiry;
    MASS_CMD_PASS_THRU      MassCmdPassThru;
    MASS_ASSIGN_DRIVE_NUM   MassAssignNum;
    MASS_CHK_DEV_READY      MassChkDevReady;
    MASS_GET_DEV_PARMS      MassGetDevParms;
    KB_LEDS_DATA            KbLedsData;
    UINT8                   Owner;
    HCPROC_PARAM            HcProc;
    COREPROC_PARAM          CoreProc;
    UINT8                   KbcControlCode;            //(EIP29733+)
    GET_DEV_ADDR            GetDevAddr;
    UINT8                   DevAddr;

    //    CCID APIs
    CCID_GETSMARTCLASSDESCRIPTOR CCIDSmartClassDescriptor;
    CCID_ATR                     CCIDAtr;
    CCID_POWERUP_SLOT            CCIDPowerupSlot;
    CCID_POWERDOWN_SLOT          CCIDPowerdownSlot;
    CCID_GETSLOT_STATUS          CCIDGetSlotStatus;
    CCID_XFRBLOCK                CCIDXfrBlock;
    CCID_GET_PARAMS              CCIDGetParameters;

    // SMART CARD READER PROTOCOL APIs
    SMART_CARD_READER_CONNECT      SmartCardReaderConnect;
    SMART_CARD_READER_DISCONNECT   SmartCardReaderDisconnect;
    SMART_CARD_READER_STATUS       SmartCardReaderStatus;
    SMART_CARD_READER_TRANSMIT     SmartCardReaderTransmit;
    SMART_CARD_READER_CONTROL      SmartCardReaderControl;
    SMART_CARD_READER_GETATTRIB    SmartCardReaderGetAttrib;

    UINT16                    HcBusDevFuncNum;    //(EIP74876+)
    HC_START_STOP             HcStartStop;
    EFI_MASS_READ             EfiMassRead;
    EFI_MASS_WRITE            EfiMassWrite;
    EFI_MASS_VERIFY           EfiMassVerify;
    EFI_KB_LEDS_DATA          EfiKbLedsData;
    MASS_GET_IDENTIFY_DATA    MassGetIdentifyData;
} U_API_DATA;

/**
    This structure is the URP structure

 Fields:   Name       Type        Description
      ------------------------------------------------------------
        bFuncNumber UINT8       Function number of the URP
        bSubFunc    UINT8       Sub-func number of the URP
        bRetValue   UINT8       Return value
        ApiData     API_DATA    Refer structure definition
**/
struct _URP_STRUC {
    UINT8       bFuncNumber;
    UINT8       bSubFunc;
    UINT8       bRetValue;
    U_API_DATA  ApiData;
};

#pragma pack(pop)

typedef struct _CONNECTED_USB_DEVICES_NUM CONNECTED_USB_DEVICES_NUM;
struct _CONNECTED_USB_DEVICES_NUM{
    UINT8   NumUsbKbds;
    UINT8   NumUsbMice;
    UINT8   NumUsbPoint;                            //(EIP38434+)
    UINT8   NumUsbMass;
    UINT8   NumUsbHubs;
    UINT8   NumUsbCcids;
    UINT8   NumUhcis;
    UINT8   NumOhcis;
    UINT8   NumEhcis;
    UINT8   NumXhcis;
};

typedef VOID (EFIAPI *EFI_USB_REPORT_DEVICES ) (
  CONNECTED_USB_DEVICES_NUM     *);

typedef struct _EFI_USB_HOTPLUG_DEVS EFI_USB_HOTPLUG_DEVS;
struct _EFI_USB_HOTPLUG_DEVS {
        BOOLEAN cdrom;
        BOOLEAN floppy;
};

typedef EFI_STATUS (EFIAPI *EFI_USB_GET_HOTPLUG_DEVS ) (
  EFI_USB_HOTPLUG_DEVS *);

typedef EFI_STATUS (EFIAPI *EFI_USB_GET_RUNTIME_REGION ) (
  EFI_PHYSICAL_ADDRESS *,
  EFI_PHYSICAL_ADDRESS *);

typedef UINT8 (EFIAPI *EFI_USB_GET_NEXT_MASS_DEVICE_NAME ) (
  UINT8*, UINT8, UINT8);

#define USB_MASS_DEV_SIGNATURE  EFI_SIGNATURE_32('u','m','a','s')

#define USB_MASS_DEV_FROM_THIS(a,b) \
    CR(a, USB_MASS_DEV, b, USB_MASS_DEV_SIGNATURE)

typedef struct _USB_MASS_DEV USB_MASS_DEV;
struct _USB_MASS_DEV{
    UINTN                   Signature;
    EFI_BLOCK_IO_PROTOCOL   BlockIoProtocol;
    EFI_DISK_INFO_PROTOCOL  DiskInfoProtocol;
    EFI_BLOCK_IO_MEDIA      *Media;
    VOID                    *DevInfo;
    UINT16                  LogicalAddress;
    EFI_HANDLE              Handle;
    UINT16                  PciBDF;
    UINT8                   *DevString;
    UINT8                   StorageType;
};

//(EIP51653+)>
/**
    If your roothub port 4 insert a hub.You want to skip hub's port 2.
    Set bRootPort = 4,  dRoutePath =2
    If your roothub port 4 insert a hub1.And hub1 port 1 insert a hub2. 
    You want to skip hub2's port 2. 
    Set bRootPort =0x4,  dRoutePath =0x21 
    Fields:   Name       Type        Description
    ------------------------------------------------------------
              SkipType   BYTE        Skip by which Type
              Flag       BYTE        Skip port or keep port or skip level. 
              BDF        WORD        Bus Dev Function
              RootPort   BYTE        Root port path
              RoutePath  DWORD       Hub route path. See description.
              BaseClass  BYTE        Device Type

**/
#define SKIP_FOR_ALLCONTROLLER          0x1             //(EIP62695)
                                                        //(EIP88776+)>
#define SKIP_FLAG_SKIP_PORT             0x0          
#define SKIP_FLAG_KEEP_PORT             0x1          
#define SKIP_FLAG_SKIP_LEVEL            0x2 //Skip usb ports on the same level.
#define SKIP_FLAG_SKIP_MULTI_LEVEL      0x3 //Skip usb ports which include down stream ports.
                                                        //<(EIP88776+)

typedef struct _USB_SKIP_LIST USB_SKIP_LIST;
struct _USB_SKIP_LIST{
    UINT8   SkipType;
    UINT8   Flag;                                      //(EIP88776)
    UINT16  BDF;
    UINT8   RootPort;
    UINT32  RoutePath;
    UINT8   BaseClass;
};
//<(EIP51653+) 

//----------------------------------------------------------------------------
//      USB Mass Storage Related Data Structures and Equates
//----------------------------------------------------------------------------
#define USB_EMU_NONE            0
#define USB_EMU_FLOPPY_ONLY     1
#define USB_EMU_HDD_ONLY        2
#define USB_EMU_HDD_OR_FDD      3
#define USB_EMU_FORCED_FDD      4

#define BAID_TYPE_HDD           1
#define BAID_TYPE_RMD_HDD       2
#define BAID_TYPE_CDROM         3
#define BAID_TYPE_RMD_FDD       4
#define BAID_TYPE_FDD           5

// Values for Mass Storage Device type
//-------------------------------------
#define USB_MASS_DEV_UNKNOWN    0
#define USB_MASS_DEV_HDD        1
#define USB_MASS_DEV_CDROM      2
#define USB_MASS_DEV_ARMD       3
#define USB_MASS_DEV_FDD        4
#define USB_MASS_DEV_MO         5


#define STOP_USB_CONTROLLER     0                               //(EIP43475+)
#define START_USB_CONTROLLER    1                               //(EIP43475+)

typedef struct _EFI_USB_PROTOCOL EFI_USB_PROTOCOL;

typedef VOID (EFIAPI *API_FUNC)(URP_STRUC*);
typedef VOID (EFIAPI *EFI_USB_CHANGE_EFI_TO_LEGACY) (UINT8);

typedef EFI_STATUS (EFIAPI *EFI_INSTALL_USB_LEGACY_BOOT_DEVICES)(VOID);
typedef EFI_STATUS (EFIAPI *EFI_USB_INSTALL_LEGACY_DEVICE)(USB_MASS_DEV*);
typedef EFI_STATUS (EFIAPI *EFI_USB_UNINSTALL_LEGACY_DEVICE)(USB_MASS_DEV*);
typedef EFI_STATUS (EFIAPI *EFI_GET_ASSIGN_USB_BOOT_PORT)(UINT8*, UINT8*);
typedef VOID (EFIAPI *EFI_KBC_ACCESS_CONTROL)(UINT8);
typedef EFI_STATUS (EFIAPI *EFI_USB_RT_LEGACY_CONTROL)(VOID *);
typedef VOID (EFIAPI *EFI_USB_STOP_UNSUPPORTED_HC)();
typedef VOID (EFIAPI *EFI_USB_SHUTDOWN_LEGACY)();                       //<(EIP52339+)
typedef EFI_STATUS (EFIAPI *EFI_USB_COPY_SKIP_TABLE)(USB_SKIP_LIST*, UINT8*, BOOLEAN);                   //(EIP51653+)   
typedef VOID (EFIAPI *EFI_USB_RT_STOP_CONTROLLER)(UINT16);                  //(EIP74876+)
typedef VOID (EFIAPI *EFI_USB_INVOKE_API)(VOID*);
typedef EFI_STATUS (EFIAPI *EFI_USB_ACCUMULATE_ISOCHRONOUS_DATA)
        (EFI_USB_PROTOCOL*, VOID*, UINT8*, UINT8, UINT16, UINT8, UINT32, UINT32*, UINT8*);

struct _EFI_USB_PROTOCOL {
    VOID                            *USBDataPtr;
    EFI_USB_REPORT_DEVICES          UsbReportDevices;
    EFI_USB_GET_NEXT_MASS_DEVICE_NAME UsbGetNextMassDeviceName;
    EFI_USB_CHANGE_EFI_TO_LEGACY    UsbChangeEfiToLegacy;
    EFI_USB_GET_RUNTIME_REGION      UsbGetRuntimeRegion;
    EFI_INSTALL_USB_LEGACY_BOOT_DEVICES InstallUsbLegacyBootDevices;
    EFI_USB_INSTALL_LEGACY_DEVICE   UsbInstallLegacyDevice;
    EFI_USB_UNINSTALL_LEGACY_DEVICE UsbUninstallLegacyDevice;
    EFI_GET_ASSIGN_USB_BOOT_PORT    UsbGetAssignBootPort;
    EFI_KBC_ACCESS_CONTROL          UsbRtKbcAccessControl;
    EFI_USB_RT_LEGACY_CONTROL       UsbLegacyControl;
    EFI_USB_STOP_UNSUPPORTED_HC     UsbStopUnsupportedHc;
    EFI_USB_SHUTDOWN_LEGACY         UsbRtShutDownLegacy;      //EIP52339+
    EFI_USB_COPY_SKIP_TABLE         UsbCopySkipTable;         //(EIP51653+)   
    EFI_USB_RT_STOP_CONTROLLER      UsbRtStopController;      //(EIP74876+)
    EFI_USB_INVOKE_API              UsbInvokeApi;
    EFI_USB_ACCUMULATE_ISOCHRONOUS_DATA UsbAccumulateIsochronousData;
};

/**
    USB Host Controller Driver function list structure.

 Fields:   Name       Type    Description
 ------------------------------------------------------------
    Flag                   UINT8 Driver Header Status
    FnHcdStart             UINT8 Driver Start
    FnHcdStop              UINT8 Driver Stop
    FnHcdEnumeratePorts    UINT8 Enumerate Root Ports
    FnHcdDisableInterrupts UINT8 Disable Interrupts
    FnHcdEnableInterrupts  UINT8 Enable Interrupts
    FnHcdProcessInterrupt  UINT8 Process Interrupt
    FnHcdGetRootHubStatus  UINT8 Get Root Hub Ports Status
    FnHcdDisableRootHub    UINT8 Disable Root Hub 
    FnHcdEnableRootHub     UINT8 Enable Root Hub
    FnHcdControlTransfer   UINT16 Perform Control Transfer
    FnHcdBulkTransfer      UINT32 Perform Bulk Transfer
    FnHcdInterruptTransfer UINT8 Perform Interrupt Transfer
    FnHcdDeactivatePolling UINT8 Deactivate Polling
    FnHcdActivatePolling   UINT8 Activate Polling
    FnHcdDisableKeyRepeat  UINT8 Disable Key Repead
    FnHcdEnableKeyRepeat   UINT8 Enable Key Repeat
**/
//!!!!
//!!!! If you change this structure, please, check UN_HCD_HEADER also.
//!!!!
typedef struct {
    UINT8       Flag;
    UINT8       (EFIAPI *FnHcdStart) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdStop) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdEnumeratePorts) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdDisableInterrupts) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdEnableInterrupts) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdProcessInterrupt) (HC_STRUC*);
    UINT32      (EFIAPI *FnHcdGetRootHubStatus) (HC_STRUC*, UINT8, BOOLEAN);
    UINT8       (EFIAPI *FnHcdDisableRootHub) (HC_STRUC*,UINT8);
    UINT8       (EFIAPI *FnHcdEnableRootHub) (HC_STRUC*,UINT8);
    UINT16      (EFIAPI *FnHcdControlTransfer) (HC_STRUC*,DEV_INFO*,UINT16,UINT16,UINT16,UINT8*,UINT16);
    UINT32      (EFIAPI *FnHcdBulkTransfer) (HC_STRUC*,DEV_INFO*,UINT8,UINT8*,UINT32);
    UINT32      (EFIAPI *FnHcdIsocTransfer) (HC_STRUC*,DEV_INFO*,UINT8,UINT8*,UINT32,UINT8*);
    UINT16      (EFIAPI *FnHcdInterruptTransfer) (HC_STRUC*, DEV_INFO*, UINT8, UINT16, UINT8*, UINT16);
    UINT8       (EFIAPI *FnHcdDeactivatePolling) (HC_STRUC*,DEV_INFO*);
    UINT8       (EFIAPI *FnHcdActivatePolling) (HC_STRUC*,DEV_INFO*);
    UINT8       (EFIAPI *FnHcdEnableEndpoints) (HC_STRUC*,DEV_INFO*,UINT8*);    
    UINT8       (EFIAPI *FnHcdDisableKeyRepeat) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdEnableKeyRepeat) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdInitDeviceData) (HC_STRUC*, DEV_INFO*, UINT32, UINT8**);
    UINT8       (EFIAPI *FnHcdDeinitDeviceData) (HC_STRUC*,DEV_INFO*);
    UINT8       (EFIAPI *FnHcdResetRootHub) (HC_STRUC*,UINT8);
    UINT8       (EFIAPI *FnHcdClearEndpointState) (HC_STRUC*,DEV_INFO*,UINT8);
    UINT8       (EFIAPI *FnHcdGlobalSuspend) (HC_STRUC*);
    UINT8       (EFIAPI *FnHcdSmiControl) (HC_STRUC*, BOOLEAN);
} HCD_HEADER;

/**
    USB Host Controller Driver Memory function list structure.

 Fields:   Name             Type          Description
 ------------------------------------------------------------
           FnHcdMemAlloc    EFI_STATUS    Memory Allocate
           FnHcdMemFree     EFI_STATUS    Memory Free
**/
typedef struct {
    EFI_STATUS  (EFIAPI *FnHcdMemAlloc) (VOID**, UINT16);
    EFI_STATUS  (EFIAPI *FnHcdMemFree) (VOID*, UINT16);
} HCD_MEM_HEADER;

typedef struct _AMI_USB_SMM_PROTOCOL AMI_USB_SMM_PROTOCOL;
struct _AMI_USB_SMM_PROTOCOL{
    EFI_USB_STOP_UNSUPPORTED_HC     UsbStopUnsupportedHc;
    API_FUNC                        *UsbApiTable;
    API_FUNC                        *UsbMassApiTable;
};

typedef struct _AMI_USB_SMM_HCD_PROTOCOL AMI_USB_SMM_HCD_PROTOCOL;
struct _AMI_USB_SMM_HCD_PROTOCOL{
    HCD_HEADER                      *UsbHcdTable;
    VOID                            *UsbGlobalDataAddr;
    VOID                            *HcTableAddr;
    UINT32                          Flag;
    HCD_MEM_HEADER                  *UsbSmmHcdMemHeader;

};
#define     SMM_HCD_DATA_VALID          BIT0
#define     DATA_IN_SMM                 BIT1

#pragma pack(push, 1)
typedef struct _QUEUE_T QUEUE_T;
struct _QUEUE_T{
    VOID* volatile* data;
    int maxsize;
    volatile int head;
    volatile int tail;
};

typedef struct _MOUSE_DATA MOUSE_DATA;
struct _MOUSE_DATA{
    UINT8   ButtonStatus;
    INT32   MouseX;
    INT32   MouseY;
    INT32   MouseZ;
};

#define MAX_NOTIFICATIONS_COUNT 100
#define MAX_DEV_CONFIG_LINK     20

typedef struct _USB_DEV_CONFIG_LINK USB_DEV_CONFIG_LINK;
struct _USB_DEV_CONFIG_LINK{
    LIST_ENTRY      Link;
    USB_DEV_CONFIG_INFO DevConfigInfo;
};

typedef struct _USB_DATA_LIST USB_DATA_LIST;
struct _USB_DATA_LIST{
    URP_STRUC       *UsbUrp;
    HC_STRUC        **HcTable;
    UINT8           HcTableCount;
    UINT8           NumOfHc;
    UINT8           *MemBlockStart;
    UINT32          *MemBlkSts;
    UINT8           *KbcCharacterBufferHead;
    UINT8           *KbcCharacterBufferTail;
    UINT8           *KbcScanCodeBufferPtr;

    DEV_INFO        *KeyRepeatDevInfo;
    DEV_INFO        *DevInfoTable;
    DEV_INFO        **UsbKbDeviceTable;

    UINT8           *MouseInputBufferHeadPtr;
    UINT8           *MouseInputBufferTailPtr;

    UINT8           *UsbTempBuffer;
    UINT8           *UsbMassConsumeBuffer;
    
    HC_STRUC        *UhciRootHubHcStruc;
    
    DEV_INFO        *QueueData[MAX_NOTIFICATIONS_COUNT];
    UINT8           *UsbSkipListTable;

    USB_TIMING_POLICY *UsbTimingPolicy;
    USB_SUPPORT_SETUP *UsbSetupData;
    
    QUEUE_T         ICCQueueCnnctDisc;
    DEV_INFO*       ICCQueueData[6];

    UINT32          *HubPortStatusBuffer;
    UINT32          *InterruptStatus;

    VOID            *Usb3HcMem;
    UINT32          Usb3HcCount;

    VOID            *IsocTdsData;
    UINT32          IsocTdsDataCount;
};

/**
    This structure is used to define a non-compliant USB device

 Fields:   Name       Type        Description
      ------------------------------------------------------------
      Vid        WORD        Vendor ID of the device
      Did        WORD        Device ID of the device
      BaseClass  BYTE        Base class of the device
      SubClass   BYTE        Sub class of the device
      Protocol   BYTE        Protocol used by the device
      Flag      INCMPT_FLAGS    Incompatibility flags

**/

typedef struct {
    UINT16  Vid;
    UINT16  Did;
    UINT8   BaseClass;
    UINT8   SubClass;
    UINT8   Protocol;
    UINT16  Flag;
} USB_BADDEV_STRUC;

/**
    USB Global Data Area structure

**/
#define MAX_USB_ERRORS_NUM              0x30    // 48 errors max
#define USB_SHIFT_KEY_STS_BUFFER_SIZE   16
#define USB_KB_DEVICE_ID_BUFFER_SIZE    16
#define USB_SCAN_CODE_BUFFER_SIZE       16
#define USB_KB_CHARACTER_BUFFER_SIZE    128
#define USB_KB_INPUT_BUFFER_SIZE        16
#define USB_MS_INPUT_BUFFER_SIZE        12
#define MAX_SMI_PIN_TABLE_COUNT         3
#define MAX_BAD_DEVICE_TABLE_COUNT      5

#define USB_KB_DEV_SUPPORT              0x00000001
#define USB_MS_DEV_SUPPORT              0x00000002
#define USB_HUB_DEV_SUPPORT             0x00000004
#define USB_MASS_DEV_SUPPORT            0x00000008
#define USB_POINT_DEV_SUPPORT           0x00000010
#define USB_CCID_DEV_SUPPORT            0x00000020

#define MAX_USB_MASS_STORAGE    32  //Define the max storage count in the system (UEFI mode)

typedef struct _USB_GLOBAL_DATA USB_GLOBAL_DATA;
struct _USB_GLOBAL_DATA{
    UINT32          UsbStateFlag;
    UINT16          ErrorLogBuffer[MAX_USB_ERRORS_NUM];
    UINT8           ErrorLogIndex;

    DEV_INFO        FddHotplugDev;
    DEV_INFO        HddHotplugDev;
    DEV_INFO        CdromHotplugDev;
    UINT64          DeviceAddressMap;
    UINT8           EnumFlag;
    UINT32          MemPages;
    UINT32          MemBlkStsBytes;
//KBD related fields
// Buffer to store keyboard shift key status bytes. This is correlated with
// scan code buffer to generate proper scan code sequence
    UINT8           KbcShiftKeyStatusBufferStart[USB_SHIFT_KEY_STS_BUFFER_SIZE];
    UINT8           KbcDeviceIdBufferStart[USB_KB_DEVICE_ID_BUFFER_SIZE];    // Buffer to store keyboard device ID
// Buffer to store keyboard shift key status bytes. This is correlated with
// scan code buffer to generate proper scan code sequence
    UINT8           KbcScanCodeBufferStart[USB_SCAN_CODE_BUFFER_SIZE];
    UINT8           KbcCharacterBufferStart[USB_KB_CHARACTER_BUFFER_SIZE]; 
    UINT8           CurrentUsbKeyCode;
    UINT8           UsbKbShiftKeyStatus;
    UINT8           NonUsbKbShiftKeyStatus;
    UINT8           KbShiftKeyStatusUnderOs;
    UINT8           UsbKbcExtStatusFlag;
    UINT8           Set2ScanCode;          // Temporary storage for the scan code set 2 scan code

    UINT8           BreakCodeDeviceId;     // Device IDs for the keyboards generating break code
    UINT8           CurrentDeviceId;       // Current USB keyboard device ID
    UINT16          UsbKbcStatusFlag;
    UINT16          RepeatCounter;         // Typematic repeat counter
    UINT16          RepeatRate;            // Typematic repeat rate
    UINT8           UsbMassDevReqFlag;

    UINT8           UsbKbInputBuffer[USB_KB_INPUT_BUFFER_SIZE];  // Keyboard expanded input buffer pointer (null-terminated)
    UINT8           bCCB;
    UINT8           RepeatKey;
    UINT8           KeyRepeatHcIndex;
// Added by mouse driver
    MOUSE_DATA      MouseData;                          
    UINT8           UsbMsInputBuffer[USB_MS_INPUT_BUFFER_SIZE];
    UINT8           bMouseStatusFlag;
                    // Bit 7   : Mouse enabled bit (1/0)
                    // Bit 6   : Mouse data ready (1/0)
                    // BIT 5   : Mouse data from USB (1/0)
                    // BIT 4   : 4-byte mouse data (1/0)
                    // Bit 3-0 : Reserved

    USB_DATA_LIST   *UsbDataList;
// BOTCommandTag used to maintain BOT command block number
    UINT32          BotCommandTag;
    UINT8           UsbStorageDeviceDelayCount;
    UINT16          UsbBulkDataXferDelay;
// Flag that allows mass storage device to handle special conditions. The
// bit pattern is defined by the USBMASS_MISC_FLAG_XXX equates in USB.EQU
//    UINT16          wMassStorageMiscFlag;
    UINT8           UsbGeometryCommandStatus;
    UINT8           UsbModeSenseSectors;
    UINT8           UsbModeSenseHeads;
    UINT16          UsbModeSenseCylinders;
    UINT16          UsbModeSenseBlockSize;
    UINT32          UsbModeSenseMaxLBA;
    UINT8           UsbReadCapSectors;
    UINT8           UsbReadCapHeads;
    UINT16          UsbReadCapCylinders;

    UINT8           DiskMediaType;
    UINT16          UsbReqTimeOutValue;
    UINT8           UsbLastCommandStatus;
    UINT32          UsbLastCommandStatusExtended;
// Added by EHCI driver
    UINT8           IgnoreConnectStsChng;
    UINT8           ProcessingPeriodicList;
    UINT8           HandOverInProgress;
    
// Tokens representation for the module binary
    UINT8           KbcSupport;
    UINT8           RepeatIntervalMode;     // 0/1 for 8ms/16ms interval

    UINT8           PowerGoodDeviceDelay;     // 0/1/5/6/../10 seconds
    UINT32          UsbFeature;

    UINT8           XhciEventServiceMode;

    UINT8           MassStorageDeviceName;

    UINT8           UsbRuntimeDriverInSmm;
    UINT64          InterruptPollingPeriod;

    UINT32          HwSmiPinTable[MAX_SMI_PIN_TABLE_COUNT];

    USB_BADDEV_STRUC UsbBadDevices[MAX_BAD_DEVICE_TABLE_COUNT];
    UINT32          UsbDevSupport;
    UINT8           UsbSendCmdToKbc;
    UINT8           MaxDevCount;
    UINT8           MaxHidCount;
    UINT8           MaxHubCount;
    UINT8           MaxMassCount;
    UINT8           MaxCcidCount;
    UINT8           UseT0T1Protocol;
    UINT8           UsbSwSmi;
    UINT64          SizeForFloppyEmulation;
    UINT16          HighSpeedMaxBulkDataSize;
    UINT16          FullSpeedMaxBulkDataSizePerFrame;

    UINT8           NumberOfFDDs;
    UINT8           NumberOfHDDs;
    UINT8           NumberOfCDROMs;
    UINT8           USBMassEmulationOptionTable[MAX_USB_MASS_STORAGE];

    UINTN           PciExpressBaseAddress;
    BOOLEAN         EfiMakeCodeGenerated;
    BOOLEAN         LegacyMakeCodeGenerated;
    UINT8           IsKbcAccessBlocked;
    UINT8           MsOrgButtonStatus;
    UINT8           CcidSequence;
    LIST_ENTRY      DevConfigInfoList;
    BOOLEAN         RegisterUsbSmiBeforeEndOfDxe;
    UINT8           MaxSkipListCount;
    UINT8           CurrentSkipListCount;
    USB_DEV_CONFIG_LINK DevConfigLink[MAX_DEV_CONFIG_LINK];
    UINT32          UsbFeature2;
};
#pragma pack(pop)

// Macro for Periodic Interval
#define PERIODIC_TIMER_SMI_16_MS                   160000

// SMI source
#define PERIODIC_TIMER_SMI_SOURCE_IS_ENABLE        0x01
#define USB_HW_SMI_SOURCE_IS_DISABLE               0x02
#define USB_GPI_SMI_SOURCE_IS_ENABLE               0x04
extern EFI_GUID gEfiUsbProtocolGuid;
extern EFI_GUID gAmiUsbSmmProtocolGuid;

#endif // _AMI_USB_CONTROLLER_H

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
