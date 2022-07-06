//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file VirtualSerial.h
    Header file for the Virtual Serial Device.

**/

#ifndef _EFI_VIRTUAL_SERIAL_H_
#define _EFI_VIRTUAL_SERIAL_H_

//---------------------------------------------------------------------------

#include <AmiDxeLib.h>
#include <Token.h>
#include <Protocol/AmiSio.h>
#include <Protocol/SuperIo.h>
#include <AcpiRes.h>
#include <IndustryStandard/Acpi.h>

//---------------------------------------------------------------------------
#define MAXIMUM_ISA_IRQ_NUMBER 15
#define RESOURCE_CONSUMER 1
#define VIRTUAL_SERIAL_SIG       SIGNATURE_32('V','I','R','S')

typedef struct {
    CHAR8       AslDevicename[5];
    UINT32      Uid;
    UINT16      BaseAddress;
    UINT32      IrqNum;             // Interrupt Number
    BOOLEAN     InterruptMode;      // 0 = Level-Trigger, 1 = Edge-Trigger 
    BOOLEAN     InterruptLevel;     // 0 = Active-High ,  1 = Active-Low  
    UINT8       InterruptShareType; // 0 = Exclusive ( Not Shared), 1 = Shared, 2 = ExclusiveAndWake, 3 = SharedAndWake 
} VIRTUAL_SERIAL_DETAILS;

typedef struct {
    ACPI_HID_DEVICE_PATH      AcpiDevicePath;
    EFI_DEVICE_PATH_PROTOCOL  EndDevicePath;
} VIRTUAL_SERIAL_DEVICEPATH;

typedef union _IRQ_INFORMATION{
    UINT8 Byte;
    struct {
        UINT8 IntMode : 1;
        UINT8 IntLevel : 1;
        UINT8 IntSharType : 2;
        UINT8 Reserved : 4;
    }Bits;
  }IRQ_INFORMATION;
  
typedef struct _VIRTUAL_DEV_DATA {
    AMI_SIO_PROTOCOL      AmiSio;
    EFI_SIO_PROTOCOL      EfiSio;
    UINT16                DevBase;
    UINT32                DevIrq;
    UINT8                 DevDma;
    IRQ_INFORMATION       IrqInfo;
    UINT32                Signature;
} VIRTUAL_DEV_DATA;

//
// AMI SIO protocol API
//
EFI_STATUS 
EFIAPI
VirtualSerialRegister (
    IN  AMI_SIO_PROTOCOL *This,
    IN  BOOLEAN          Write,
    IN  BOOLEAN          ExitCfgMode,
    IN  UINT8               Register,
    IN  OUT UINT8           *Value
 );

EFI_STATUS 
EFIAPI
VirtualSerialCRS (
    IN  AMI_SIO_PROTOCOL *This,
    IN  BOOLEAN          Set,
    IN  OUT T_ITEM_LIST  **Resources
 );

EFI_STATUS 
EFIAPI
VirtualSerialPRS (
    IN  AMI_SIO_PROTOCOL     *This,
    IN  BOOLEAN              Set,
    IN  OUT T_ITEM_LIST      **Resources
 );

//
// EFI SIO protocol API
//
EFI_STATUS
EFIAPI 
VirtualEfiSioRegisterAccess (
    IN  CONST  EFI_SIO_PROTOCOL  *This,
    IN  BOOLEAN           Write,
    IN  BOOLEAN           ExitCfgMode,
    IN  UINT8             Register,
    IN  OUT UINT8             *Value
 );

EFI_STATUS
EFIAPI 
VirtualEfiSioGetResources ( 
    IN  CONST EFI_SIO_PROTOCOL            *This,
    OUT ACPI_RESOURCE_HEADER_PTR    *ResourceList
 );

EFI_STATUS 
EFIAPI
VirtualEfiSioSetResources (
    IN  CONST  EFI_SIO_PROTOCOL        *This,
    IN  ACPI_RESOURCE_HEADER_PTR ResourceList
 );

EFI_STATUS 
EFIAPI
VirtualEfiSioPossibleResources (
    IN  CONST EFI_SIO_PROTOCOL         *This,
    OUT ACPI_RESOURCE_HEADER_PTR *ResourceCollection
);

EFI_STATUS 
EFIAPI
VirtualEfiSioModify (
    IN  CONST EFI_SIO_PROTOCOL         *This,
    IN  CONST EFI_SIO_REGISTER_MODIFY  *Command,
    IN  UINTN                    NumberOfCommands
 );

#endif
