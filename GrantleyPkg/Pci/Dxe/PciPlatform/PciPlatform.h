//
// This file contains an 'Intel Pre-EFI Module' and is licensed
// for Intel CPUs and Chipsets under the terms of your license 
// agreement with Intel or your vendor.  This file may be      
// modified by the user, subject to additional terms of the    
// license agreement                                           
//
/*++

Copyright (c)  2004 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PciPlatform.h

Abstract:

  This is PCI platform initialization code.

--*/

#ifndef PCI_PLATFORM_H_
#define PCI_PLATFORM_H_

#include <PiDxe.h>
#include <PchRegs.h>
#include <Cpu\CpuRegs.h>
#include <Platform.h>
#include <Library\BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/S3PciLib.h>
#include <Library/PciLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Protocol\Runtime.h>
#include <Protocol\PciCallback.h>
#include <Protocol\PciPlatform.h>
#include <Protocol\PciIovPlatform.h>
#include <Protocol\PciIo.h>
#include <Protocol\FirmwareVolume2.h>
#include <Guid\SetupVariable.h>
#include <Guid\PlatformInfo.h>
#include <IndustryStandard\Pci.h>
#include <Library/PcieSlotOpromLib.h>

//
// Global variables for Option ROMs
//
#define NULL_ROM_FILE_GUID \
  { \
    0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
  }

//
// {7301762E-4BF3-4b06-ADA0-766041BE36AF}
//
#define ONBOARD_VIDEO_OPTION_ROM_FILE_GUID \
  { \
    0x7301762e, 0x4bf3, 0x4b06, 0xad, 0xa0, 0x76, 0x60, 0x41, 0xbe, 0x36, 0xaf \
  }

//
// {A6F63D76-6975-4144-96B3-26D624189B2B}
//
#define PXE_UNDI_OPTION_ROM_FILE_GUID \
  { \
    0xa6f63d76, 0x6975, 0x4144, 0x96, 0xb3, 0x26, 0xd6, 0x24, 0x18, 0x9b, 0x2b \
  }

#define PXE_BASE_OPTION_ROM_FILE_GUID \
  { \
    0x6f2bc426, 0x8d83, 0x4f17, 0xbe, 0x2c, 0x6b, 0x1f, 0xcb, 0xd1, 0x4c, 0x80 \
  }

//
// {7A39517D-DD10-4abb-868F-B859FBBBBA87}
//
#define IDE_RAID_OPTION_ROM_FILE_GUID \
  { \
    0x7a39517d, 0xdd10, 0x4abb, 0x86, 0x8f, 0xb8, 0x59, 0xfb, 0xbb, 0xba, 0x87 \
  }

#define BARTON_HILLS_DEVICE_ID 0x150E

// {BD5D4CA5-674F-4584-8CF9-CE4EA1F54DD1}
#define SATA_RAID_OPTION_ROM_FILE_GUID \
  { \
    0xbd5d4ca5, 0x674f, 0x4584, 0x8c, 0xf9, 0xce, 0x4e, 0xa1, 0xf5, 0x4d, 0xd1 \
  }

    // {2CC25173-BD9F-4C89-89CC-29256A3FD9C3}
#define SSATA_RAID_OPTION_ROM_FILE_GUID \
  { \
    0x2cc25173, 0xbd9f, 0x4c89, 0x89, 0xcc, 0x29, 0x25, 0x6a, 0x3f, 0xd9, 0xc3 \
  }
  

#define ATS_VIDEO_OPTION_ROM_FILE_GUID \
  { \
    0x744DEFE9, 0x1325, 0x48d5, 0x8b, 0x90, 0x16, 0x66, 0x06, 0x62, 0xA8, 0x12 \
  }

typedef struct {
  EFI_GUID  FileName;
  UINTN     Segment;
  UINTN     Bus;
  UINTN     Device;
  UINTN     Function;
  UINT16    VendorId;
  UINT16    DeviceId;
} PCI_OPTION_ROM_TABLE;

#define INVALID                     0xBD

#define PCI_CALLBACK_DATA_SIGNATURE SIGNATURE_32 ('P', 'c', 'i', 'c')

typedef struct {
  UINT32                    Signature;
  LIST_ENTRY                Link;
  EFI_PCI_CALLBACK_FUNC     Function;
  EFI_PCI_ENUMERATION_PHASE Phase;
} PCI_CALLBACK_DATA;

typedef struct {
  EFI_HANDLE                      PciPlatformHandle;
  EFI_HANDLE                      RootBridgeHandle;
  EFI_PCI_PLATFORM_PROTOCOL       PciPlatform;
  EFI_PCI_CALLBACK_PROTOCOL       PciCallback;
#ifdef EFI_PCI_IOV_SUPPORT
  EFI_PCI_IOV_PLATFORM_PROTOCOL   PciIovPlatform;
#endif
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  EFI_CPU_IO_PROTOCOL             *CpuIo;
  EFI_LIST_ENTRY                  PciCallbackList;
  EFI_PCI_CALLBACK_CONTEXT        Context;
  EFI_PCI_ENUMERATION_PHASE       PciEnumerationPhase;
  EFI_PLATFORM_INFO               *PlatformInfo;
  UINT8                           BusAssignedTime;
} PCI_PLATFORM_PRIVATE_DATA;

#define PCI_CALLBACK_DATA_FROM_LINK(_node) \
  CR ( \
    _node, \
    PCI_CALLBACK_DATA, \
    Link, \
    PCI_CALLBACK_DATA_SIGNATURE \
    )

extern PCI_PLATFORM_PRIVATE_DATA  mPciPrivateData;
extern EFI_GUID  gPchSataEfiLoadProtocolGuid;

EFI_STATUS
EFIAPI
PhaseNotify (
  IN EFI_PCI_PLATFORM_PROTOCOL                        *This,
  IN EFI_HANDLE                                       HostBridge,
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE    Phase,
  IN EFI_PCI_CHIPSET_EXECUTION_PHASE                  ChipsetPhase
  )
/*++

Routine Description:

  Perform initialization by the phase indicated.

Arguments:

  This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.
  HostBridge    -  The associated PCI host bridge handle.
  Phase         -  The phase of the PCI controller enumeration.
  ChipsetPhase  -  Defines the execution phase of the PCI chipset driver.

Returns:

  EFI_SUCCESS  -  Must return with success.

--*/
;

EFI_STATUS
EFIAPI
PlatformPrepController (
  IN  EFI_PCI_PLATFORM_PROTOCOL                      *This,
  IN  EFI_HANDLE                                     HostBridge,
  IN  EFI_HANDLE                                     RootBridge,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS    PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE   Phase,
  IN  EFI_PCI_CHIPSET_EXECUTION_PHASE                ChipsetPhase
  )
/*++

Routine Description:

  The PlatformPrepController() function can be used to notify the platform driver so that 
  it can perform platform-specific actions. No specific actions are required. 
  Several notification points are defined at this time. More synchronization points may be 
  added as required in the future. The PCI bus driver calls the platform driver twice for 
  every PCI controller-once before the PCI Host Bridge Resource Allocation Protocol driver 
  is notified, and once after the PCI Host Bridge Resource Allocation Protocol driver has 
  been notified. 
  This member function may not perform any error checking on the input parameters. It also 
  does not return any error codes. If this member function detects any error condition, it 
  needs to handle those errors on its own because there is no way to surface any errors to 
  the caller.  
    
Arguments:

  This          -  Pointer to the EFI_PCI_PLATFORM_PROTOCOL instance.  
  HostBridge    -  The associated PCI host bridge handle. 
  RootBridge    -  The associated PCI root bridge handle.
  PciAddress    -  The address of the PCI device on the PCI bus. 
  Phase         -  The phase of the PCI controller enumeration. 
  ChipsetPhase  -  Defines the execution phase of the PCI chipset driver. 
    
Returns:

  EFI_SUCCESS  -  The function completed successfully.
    
--*/
;

EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL            *This,
  OUT EFI_PCI_PLATFORM_POLICY              *PciPolicy
  )
/*++

Routine Description:

  Set the PciPolicy as EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_NO_ALIAS.

Arguments:

  This       -  The pointer to the Protocol itself.
  PciPolicy  -  the returned Policy.

Returns:

  EFI_UNSUPPORTED        -  Function not supported.
  EFI_INVALID_PARAMETER  -  Invalid PciPolicy value.

--*/
;

EFI_STATUS
EFIAPI
GetPciRom (
  IN   CONST EFI_PCI_PLATFORM_PROTOCOL      *This,
  IN   EFI_HANDLE                           PciHandle,
  OUT  VOID                                 **RomImage,
  OUT  UINTN                                *RomSize
  )
/*++

Routine Description:
  
  Return a PCI ROM image for the onboard device represented by PciHandle.

Arguments:
  
  This       -  Protocol instance pointer.
  PciHandle  -  PCI device to return the ROM image for.
  RomImage   -  PCI Rom Image for onboard device.
  RomSize    -  Size of RomImage in bytes.

Returns:

  EFI_SUCCESS    -  RomImage is valid.
  EFI_NOT_FOUND  -  No RomImage.

--*/
;

EFI_STATUS
EFIAPI
RegisterPciCallback (
  IN  EFI_PCI_CALLBACK_PROTOCOL               *This,
  IN  EFI_PCI_CALLBACK_FUNC                   Function,
  IN  EFI_PCI_ENUMERATION_PHASE               Phase
  )
/*++

Routine Description:

  Register a callback during PCI bus enumeration

Arguments:

  This       -  Protocol instance pointer.
  Function   -  Callback function pointer.
  Phase      -  PCI enumeration phase.

Returns:

  EFI_SUCCESS           - Function has registed successfully
  EFI_UNSUPPORTED       - The function has been regisered
  EFI_InVALID_PARAMETER - The parameter is incorrect

--*/
;

#endif
