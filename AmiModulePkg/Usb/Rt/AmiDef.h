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

/** @file AmiDef.h
    AMI USB driver definitions, framework specific

**/
#ifndef     __AMI_DEF_H
#define     __AMI_DEF_H

#include <Token.h>
#include <IndustryStandard/Pci.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeSmmUsbSbLib.h>
#define USB_DRIVER_VERSION  USB_DRIVER_MAJOR_VER
#define _FAR_

//
//Define the Generic USB cable support for USB debugger
//
#ifndef GENERIC_USB_CABLE_SUPPORT
#define GENERIC_USB_CABLE_SUPPORT 0
#endif
#if GENERIC_USB_CABLE_SUPPORT
#undef USB_DEBUG
#define USB_DEBUG
#endif

// 
// Define USB Debug level
//
#define TOP_DEBUG_LEVEL        3
#define BOTTOM_DEBUG_LEVEL     3
#define USB_DEBUG(EdkErrorLevel, UsbErrorLevel, ...) \
    if ((UsbErrorLevel == 0) || ((UsbErrorLevel <= TOP_DEBUG_LEVEL) && \
    (UsbErrorLevel >= BOTTOM_DEBUG_LEVEL))) \
        DEBUG((-1, __VA_ARGS__))

#define  MAX_DEBUG_LEVEL        8
#define DEBUG_LEVEL_8           8
#define DEBUG_LEVEL_7           7
#define DEBUG_LEVEL_6           6
#define DEBUG_LEVEL_5           5
#define DEBUG_LEVEL_4           4
#define DEBUG_LEVEL_3           3
#define DEBUG_LEVEL_2           2
#define DEBUG_LEVEL_1           1
#define DEBUG_LEVEL_0           0

// 
// Common defination
//
#define MICROSECOND                 10
#define MILLISECOND                 (1000 * MICROSECOND)
#define ONESECOND                   (1000 * MILLISECOND)
#define MILLISECOND_FIXED_DELAY     1000
#define COUNTOF(x) (sizeof(x)/sizeof((x)[0]))
// 
// Define USB EBDA offset
//
#define USB_DATA_EBDA_OFFSET         0x104
#define USB_PROTOCOL_EBDA_OFFSET     0x32

//
// Define macros to build data structure signatures from characters.
//
#define EFI_SIGNATURE_16(A,B)             ((A) | (B<<8))
#define EFI_SIGNATURE_32(A,B,C,D)         (EFI_SIGNATURE_16(A,B)     | (EFI_SIGNATURE_16(C,D)     << 16))
#define EFI_SIGNATURE_64(A,B,C,D,E,F,G,H) (EFI_SIGNATURE_32(A,B,C,D) | ((UINT64)(EFI_SIGNATURE_32(E,F,G,H)) << 32))

#endif      // __AMI_DEF_H

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
