//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file AmiUsbSmmGlobalDataValidationLib.h
  Defines the AmiUsbSmmGlobalDataValidationLib library functions.

  Buffer Validation Functions.
**/
#ifndef __AMI_USB_SMM_GLOBAL_DATA_VALIDATION_LIB__H
#define __AMI_USB_SMM_GLOBAL_DATA_VALIDATION_LIB__H

// define error code.
#define    VALIDATE_USB_DUMMY                 0x00
#define    VALIDATE_USB_GLOBAL_DATA           0x01
#define    VALIDATE_USB_DATA_LIST             0x02
#define    VALIDATE_USB_MEM_BLOCK             0x03
#define    VALIDATE_USB_MEM_BLOCK_STATUS      0x04
#define    VALIDATE_USB_TEMP_BUFFER           0x05
#define    VALIDATE_USB_MASS_MEM_BUFFER       0x06
#define    VALIDATE_USB_TIMING_POLICY         0x07
#define    VALIDATE_USB_SETUP_DATA            0x08
#define    VALIDATE_USB_SKIP_LIST             0x09
#define    VALIDATE_USB_HC_STRUC              0x0A
#define    VALIDATE_USB_FRAME_LIST            0x0B
#define    VALIDATE_USB_EHCI_ISOCTDS          0x0C
#define    VALIDATE_USB_XHCI_BUS_DATA         0x0D
#define    VALIDATE_USB_DEV_CON_INFO          0x0E
#define    VALIDATE_USB_MS_INPUT_BUFFER       0x0F
#define    VALIDATE_USB_KB_CHAR_BUFFER        0x10
#define    VALIDATE_USB_KB_SCAN_BUFFER        0x11
#define    VALIDATE_USB_KB_REPEAT_DEV         0x12
#define    VALIDATE_USB_KB_DEV                0x13
#define    VALIDATE_USB_HC_STRUC_POINTER      0x14
#define    VALIDATE_USB_UHCI_MMIO             0x15
#define    VALIDATE_HUB_PORT_STATUS_BUFFER    0x16
#define    VALIDATE_INTERRUPT_STATUS          0x17
#define    VALIDATE_DEVICE_TABLE              0x18
#define    VALIDATE_KB_DEVICE_TABLE           0x19
#define    USB_CRC_ERROR                      0x80

typedef struct _SMM_VALIDATE_STRUCT SMM_VALIDATE_STRUCT;
struct _SMM_VALIDATE_STRUCT {
    VOID    *BufferInRuntime;
    VOID    *BufferInSmm;
    UINTN   VaildateSize;
    UINT8   ErrorCode;
};

EFI_STATUS EFIAPI AmiUsbSmmGlobalDataValidation(USB_GLOBAL_DATA*, USB_DATA_LIST*,UINT8*);

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
