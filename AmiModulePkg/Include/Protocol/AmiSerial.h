//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093       **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file AmiSerial.h
    AMI Serial Protocol Definition.

    The Protocol is used to describe non-standard UART devices.
**/

#ifndef __AMI_SERIAL_PROTOCOL_H__
#define __AMI_SERIAL_PROTOCOL_H__

//AMI Serial Protocol GUID
#define AMI_SERIAL_PROTOCOL_GUID \
        {0x50dc5c90, 0x1d33, 0x4fd6, { 0x87, 0xe5, 0x06, 0x3b, 0x1d, 0xfa, 0x21, 0x70 } }

typedef struct _AMI_SERIAL_PROTOCOL AMI_SERIAL_PROTOCOL;

/// Serial device properties
typedef struct {
    UINT64 UartClock;     ///< UART Clock
    UINT64 BaseAddress;   ///< UART Base Address
    UINT8 Irq;            ///< UART IRQ
    BOOLEAN MmioAccess; ///< UART Access method. TRUE - MMIO, FALSE - I/O
    UINT8 MmioWidth;      ///< MMIO width if the base address is MMIO. The field is not used for I/O devices.
} AMI_SERIAL_DEVICE_INFO;

/**
    Get the UART device details

    This function called to get the UART device details like Base Address
    IRQ, Access Method, Etc.

    @param  This        AMI Serial device protocol pointer.
    @parm   DeviceInfo  pointer to the device information data structure populated by the function.

    @retval EFI_SUCCESS The operation completed successfully.
    @retval EFI_NOT_FOUND Error in getting device details

**/
typedef EFI_STATUS (EFIAPI *AMI_GET_SERIAL_DEVICE_INFO) (
    IN AMI_SERIAL_PROTOCOL *This, OUT AMI_SERIAL_DEVICE_INFO *DeviceInfo
);

// AMI Serial Protocol definition.
struct _AMI_SERIAL_PROTOCOL {
    AMI_GET_SERIAL_DEVICE_INFO  GetSerialDeviceInfo;
};

extern EFI_GUID gAmiSerialProtocolGuid;
#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
