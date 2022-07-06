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

/** @file UsbDeviceCaps.h
    This header file contains the definitions for 
    USB device capability descriptors.
**/

#ifndef _USB_DEVICE_CAPS_H_
#define _USB_DEVICE_CAPS_H_


#pragma pack(push, 1)

typedef struct _BOS_CAP_DESC BOS_CAP_DESC; 
struct _BOS_CAP_DESC{
  UINT8   DescLength;
  UINT8   DescType;
  UINT8   DevCapType;
};



//
// USB 2.0 Extension descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT32         Attributes;
} USB20_EXT_DESC;



//
// SuperSpeed device capability descriptor 
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8          Attributes;
    UINT16         SpeedsSupported;
    UINT8          FnSupport;
    UINT8          U1DevExitLat;
    UINT16         U2DevExitLat;
} SUPER_SPEED_CAP_DESC;


//
// Container ID descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8          Reserved0;
    UINT8          ContainerId[16];
} CONTAINER_ID_DESC;

//
// Platform descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8          Reserved0;
    UINT8          PlatformCapUuid[16];
} USB_PLATFORM_DESC;

//
//SupperSpeedPlus USB device capability descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8          Reserved0;
    UINT32         Attributes;
    UINT16         FnSupport;
    UINT16         Reserved1;
    UINT32         SubLinkSpeedAttr[1];
} SSP_DEV_CAP_DESC;


//
//Precision Time Measurement descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
} PRECISION_TIME_MEASUREMENT_DESC;


//
//Configuration Summary descriptor
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT16         BcdVersion;
    UINT8          Class;
    UINT8          SubClass;
    UINT8          Protocol;
    UINT8          ConfigCount;
    UINT8          ConfigIndex[1];
} CONFIG_SUMMARY_DESC;


//****************************************************
//9.2 PD Specific Descriptors (USB Power Delivery Specification Revision 3.0)
// There are four PD Specific descriptors:
// 1).POWER_DELIVERY_CAPABILITY (06h)
//    Defines the various PD Capabilities of this device 
// 2).BATTERY_INFO_CAPABILITY (07H)
//    Provides information on each Battery supported by the device
// 3).PD_CONSUMER_PORT_CAPABILITY (08H)
//    The Consumer characteristics of a Port on the device
// 4).PD_PROVIDER_PORT_CAPABILITY (09H)
//    The provider characteristics of a Port on the device
//****************************************************

//
// USB Power Delivery Capability Descriptor(USB PD30 Spec 9.2.1)
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8          Reserved0;
    UINT32         Attributes;
    UINT16         BcdBcVersion;
    UINT16         BcdPdVersion;
    UINT16         BcdUsbTypeCVersion;
} PD_USB_CAP_DESC;


//
// Battery Info Capability Descriptor(USB PD30 Spec 9.2.2)
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8   Battery;
    UINT8   Serial;
    UINT8   Manufacturer;
    UINT8   BatteryId;
    UINT8   Reserved;
    UINT32  ChargedThreshold;
    UINT32  WeakThreshold;
    UINT32  BatteryDesignCapacity;
    UINT32  BatteryLastFullChargeCapacity;
} BATTERY_INFO_CAP_DESC;


//
// PD Consumer Port Capability Descriptor(USB PD30 Spec 9.2.3)
//
typedef struct {
    BOS_CAP_DESC   CapsHeader;
    UINT8   Reserved0;
    UINT16  Capabilities;
    UINT16  MinVoltage;
    UINT16  MaxVoltage;
    UINT16  Reserved1;
    UINT32  MaxOperatingPower;
    UINT32  MaxPeakPower;
    UINT32  MaxPeakPowerTime;
} PD_CONSUMER_PORT_DESC;

//
// PD Provider Port Capability Descriptor(USB PD30 Spec 9.2.4)
//
typedef struct {
    BOS_CAP_DESC  CapsHeader;
    UINT8         Reserved0;
    UINT16        Capabilities;
    UINT8         NumOfPDObjects;
    UINT8         Reserved1;
    UINT32        PowerDataObjects[1];
} PD_PROVIDER_PORT_DESC;

#pragma pack(pop)

#endif

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
