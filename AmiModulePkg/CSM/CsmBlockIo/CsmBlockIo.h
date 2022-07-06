//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CsmBlockIo.h
    CSM BlockIO driver header file.

**/

#ifndef _CsmBlockIo_
#define _CsmBlockIo_

#ifdef __cplusplus
extern "C" {
#endif

#include <Efi.h>
#include <Token.h>
#include <Dxe.h>
#include <Pci.h>
#include <AmiDxeLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyBiosExt.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/ComponentName.h>
#include <Protocol/BlockIo.h>
#include <Library/DxeServicesTableLib.h>

#include "CsmEdd.h"

#ifndef ACCESS_PAGE0_CODE
#define ACCESS_PAGE0_CODE(statements)                           \
    {                                                           \
      statements;                                               \
    }
#endif

// Global Variables
#if EFI_DEBUG
extern EFI_COMPONENT_NAME_PROTOCOL gBiosBlockIoComponentName;
#endif


// Define the I2O class code

#define PCI_BASE_CLASS_INTELLIGENT      0x0e
#define PCI_SUB_CLASS_INTELLIGENT       0x00

// SD class/subclass defined in PCI.H in 4.6.3.7
#ifndef PCI_CL_SYSTEM_PERIPHERALS
#define PCI_CL_SYSTEM_PERIPHERALS		0x08
#endif
#ifndef PCI_CL_SYSTEM_PERIPHERALS_SCL_SD
#define PCI_CL_SYSTEM_PERIPHERALS_SCL_SD	0x05
#endif


// Number of pages needed for our buffer under 1MB

#define BLOCK_IO_BUFFER_PAGE_SIZE (((sizeof (EDD_DEVICE_ADDRESS_PACKET) + sizeof (CSM_LEGACY_DRIVE) + MAX_EDD11_XFER) / EFI_PAGE_SIZE) + 1)


// PROTOTYPES

// Driver Binding Protocol functions

EFI_STATUS
EFIAPI
CsmBlockIoSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
CsmBlockIoStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  );

EFI_STATUS
EFIAPI
CsmBlockIoStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   Controller,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  );
  
// Block I/O functions

EFI_STATUS
EFIAPI
CsmBlockIoFlushBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL           *This
    );

EFI_STATUS
EFIAPI
CsmBlockIoReset (
    IN  EFI_BLOCK_IO_PROTOCOL           *This,
    IN  BOOLEAN                ExtendedVerification
    );

// These prototypes aren't actually used -- they are the generic for the specific
// functions below
EFI_STATUS
EFIAPI
CsmBlockIoReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
CsmBlockIoWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL           *This,
    IN  UINT32                 MediaId,
    IN  EFI_LBA                LBA,
    IN  UINTN                  BufferSize,
    OUT VOID                   *Buffer
    );

// Specific read/write function prototypes
EFI_STATUS
EFIAPI
Edd30CsmReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This, 
    IN  UINT32                MediaId, 
    IN  EFI_LBA               LBA, 
    IN  UINTN                 BufferSize, 
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
Edd30CsmWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
Edd11CsmReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
Edd11CsmWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
LegacyCsmReadBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

EFI_STATUS
EFIAPI
LegacyCsmWriteBlocks (
    IN  EFI_BLOCK_IO_PROTOCOL          *This,
    IN  UINT32                MediaId,
    IN  EFI_LBA               LBA,
    IN  UINTN                 BufferSize,
    OUT VOID                  *Buffer
    );

// Local support function prototypes
BOOLEAN
InitBlockIo (
    IN  CSM_BLOCK_IO_DEV        *Dev
    );

// Local function prototypes
VOID
BuildDevicePath (
    IN  EFI_DEVICE_PATH_PROTOCOL  *BaseDevicePath,
    IN  CSM_LEGACY_DRIVE          *Drive,
    IN  EFI_DEVICE_PATH_PROTOCOL  **DevPath
    );

EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
AppendDevicePathNode (
    IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevPath,
    IN CONST EFI_DEVICE_PATH_PROTOCOL  *Node
    );

UINTN AlignAddress (
    UINTN                       Address
    );

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif

