//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformEarlyInit.h

Abstract:

  Platform Stage1 header file

--*/

#ifndef _EFI_PLATFORM_EARLY_INIT_H_
#define _EFI_PLATFORM_EARLY_INIT_H_

#include <Token.h> // Aptio V server override
#include <PiPei.h>
#include <Uefi.h>
#include <Ppi/BaseMemoryTest.h>
#include <Ppi/Stall.h>
#include <Ppi/FlashMap.h>
#include <Ppi/PlatformMemorySize.h>
#include <Ppi/BootInRecoveryMode.h>
#include <Ppi/PchPlatformPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Ppi/Reset.h>
#include <Ppi/Smbus2.h>

#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/PlatformTxt.h>
#include <Guid/BiosId.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PlatformHooksLib.h>
#include <Library/PlatformClocksLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/PchPlatformLib.h>
#include <Library/PrintLib.h>
#include <Library/BiosIdLib.h>

#include <Cpu\CpuBaseLib.h>
#include <Platform.h>
#include <PchRegs.h>
#include <IndustryStandard/Pci22.h>

#include <Framework/Hob.h>
#include <Guid/Capsule.h>
#include <SioRegs.h>
#include <FlashMap.h>

#include <Ppi/SpiSoftStraps.h>
// #include <TridentData.h> 
#include <IchRegsAliases.h> 
#include <Ppi/MpService.h>
#include <Ppi/PostBootScriptTable.h>
//
// Wake Event Types
//
#define SMBIOS_WAKEUP_TYPE_RESERVED           0x00
#define SMBIOS_WAKEUP_TYPE_OTHERS             0x01
#define SMBIOS_WAKEUP_TYPE_UNKNOWN            0x02
#define SMBIOS_WAKEUP_TYPE_APM_TIMER          0x03
#define SMBIOS_WAKEUP_TYPE_MODEM_RING         0x04
#define SMBIOS_WAKEUP_TYPE_LAN_REMOTE         0x05
#define SMBIOS_WAKEUP_TYPE_POWER_SWITCH       0x06
#define SMBIOS_WAKEUP_TYPE_PCI_PME            0x07
#define SMBIOS_WAKEUP_TYPE_AC_POWER_RESTORED  0x08

#define UUID_OFFSET   0
//-----------------------------------------------------------------------
// SPI control regs.
//-----------------------------------------------------------------------
#define R_SB_SPI_HSFSTS 0x04
#define B_SB_SPI_LOCK_DOWN 0x8000;
#define R_SB_SPI_HSFCTL 0x06
#define B_SB_SPI_READ_CYCLE  BIT0;
#define B_SB_SPI_WRITE_CYCLE BIT2|BIT0;
#define B_SB_SPI_BLOCK_ERASE BIT2|BIT1|BIT0;
#define B_SB_SPI_GO_CYCLE    BIT0;
#define SPI_VSCC_MINUM_COMMUN_DENOMINATOR  0xD817
#define DESCRIP_ADDR        0x10;
#define R_SB_SPI_FREG4 0x64
#define R_SB_SPI_FADDR  0x08
#define DESCRIPT_SIZE   0x1000
#define BLOCK_SIZE_256B  0x100
#define BLOCK_SIZE_4KB   0x1000
#define BLOCK_SIZE_8KB   0x2000
#define BLOCK_SIZE_32KB  0x8000
#define BLOCK_SIZE_64KB  0x10000

#define R_SB_SPI_LVSCC 0xC4
#define R_SB_SPI_UVSCC 0xC8
#define R_SB_SPI_FDAT0 0x10

//AptioV server override start: WA to avoid redefinition errors when including Token.h
#ifndef  SB_RCBA
#define SB_RCBA           0x0FED1C000
#endif
#if SPI_BASE_ADDRESS == 0x3800
#undef SPI_BASE_ADDRESS
#define SPI_BASE_ADDRESS  (SB_RCBA+0x3800)
#endif

//AptioV server override end: WA to avoid redefinition errors when including Token.h

#define RESET_PORT                0x0CF9
#define CLEAR_RESET_BITS          0x0F1
#define COLD_RESET                0x02  // Set bit 1 for cold reset
#define RST_CPU                   0x04  // Setting this bit triggers a reset of the CPU
#define FULL_RESET                0x08  // Set bit 4 with bit 1 for full reset

#define PCHSTRAP_1   1
#define PCHSTRAP_9   9
#define PCHSTRAP_10  10
#define PCHSTRAP_16  16
#define PCHSTRAP_17  17

#define OPTION_DISABLED 0
#define OPTION_ENABLED  1
#define OPTION_AUTO     2
#define WBG_SS01_TOS_ENABLE          0x02000000   //Set bit 25 for Tpm On Spi


//
// From WBG Soft Straps WIP.xlsx
//
#define WBG_DOWNSKU_STRAP_DSKU      0x80046000
#define WBG_DOWNSKU_STRAP_BSKU      0x8004E003
#define WBG_DOWNSKU_STRAP_TSKU      0x00044000

#define DESCRIP_FLMAP1      0x18;

#define AUTO_BIF     4


/*
//
// Driver Consumed Guids
//
#include EFI_GUID_DEFINITION (TcgDataHob)
#include EFI_GUID_DEFINITION (MemoryTypeInformation)
#include EFI_GUID_DEFINITION (PlatformInfo)
#include EFI_GUID_DEFINITION (SetupVariable)
#include EFI_GUID_DEFINITION (TpmSupport)

//
// Driver Consumed PPI Prototypes
//
#include EFI_PPI_DEPENDENCY (CpuIo)
#include EFI_PPI_DEPENDENCY (PciCfg)
#include EFI_PPI_DEPENDENCY (BaseMemoryTest)
#include EFI_PPI_DEFINITION (FlashMap)
#include EFI_PPI_DEFINITION (SmbusPolicy)
#include EFI_PPI_DEFINITION (Cache)
#include EFI_PPI_DEFINITION (MemoryDiscovered)
#include EFI_PPI_DEFINITION (EndOfPeiSignal)
#include EFI_PPI_DEFINITION (Capsule)

#include EFI_PPI_DEFINITION (PchPlatformPolicy)

//
// Driver Produced PPI Prototypes
//
#include EFI_PPI_PRODUCER (AcpiPolicy)
#include EFI_PPI_PRODUCER (AtaPolicy)
#include EFI_PPI_PRODUCER (BootMode)
#include EFI_PPI_PRODUCER (BootInRecoveryMode)
#include EFI_PPI_PRODUCER (Stall)
#include EFI_PPI_PRODUCER (Reset)
#include EFI_PPI_PRODUCER (Variable)
#include EFI_PPI_PRODUCER (Variable2)
#include EFI_PPI_PRODUCER (PlatformMemorySize)
*/

//
// Defines for stall ppi
//
#define PEI_STALL_RESOLUTION  1

//
// Used in PEI memory test routines
//
#define MEMORY_TEST_COVER_SPAN  0x40000
#define MEMORY_TEST_PATTERN     0x5A5A5A5A

#define LEAVE_POWER_STATE_UNCHANGED 0xFF

#ifndef EFI_DEADLOOP
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif

#define ___INTERNAL_CONVERT_TO_STRING___(a) #a
#define CONVERT_TO_STRING(a) ___INTERNAL_CONVERT_TO_STRING___(a)

//
// Function prototypes for breakpoint callbacks
//
EFI_STATUS
EFIAPI
AfterMrcBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );
    
EFI_STATUS
EFIAPI
AfterQpircBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );
  
EFI_STATUS
EFIAPI
ReadyForIbistBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );
  
EFI_STATUS
EFIAPI
AfterFullSpeedSetupBreakpoint (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  ); 
  
EFI_STATUS
PeiPrintPlatformInfo (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );
  
EFI_STATUS
PeiGetBiosId (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN OUT  BIOS_ID_IMAGE           *BiosIdImage
  );
  
EFI_STATUS
SendBiosIdToMcu (
    IN EFI_PEI_SERVICES           **PeiServices,
    IN BIOS_ID_STRING             *BiosIdString
  );

typedef struct {
  UINT8 Index;
  UINT8 Value;
} SIO_INDEX_DATA;


typedef struct _PEI_FV_INFO_PPI_PRIVATE {
  EFI_PEI_PPI_DESCRIPTOR                  PpiList;
  EFI_PEI_FIRMWARE_VOLUME_INFO_PPI        FvInfoPpi;
} PEI_FV_INFO_PPI_PRIVATE;

VOID
EFIAPI
InstallFvInfoPpi (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_FV_HANDLE           VolumeHandle
  );


//
// Function Prototypes
//
EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  PEI_BASE_MEMORY_TEST_PPI  *This,
  IN  EFI_PHYSICAL_ADDRESS      BeginAddress,
  IN  UINT64                    MemoryLength,
  IN  PEI_MEMORY_TEST_OP        Operation,
  OUT EFI_PHYSICAL_ADDRESS      *ErrorAddress
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices   - GC_TODO: add argument description
  This          - GC_TODO: add argument description
  BeginAddress  - GC_TODO: add argument description
  MemoryLength  - GC_TODO: add argument description
  Operation     - GC_TODO: add argument description
  ErrorAddress  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

EFI_STATUS
PlatformPchInit (
  IN SYSTEM_CONFIGURATION        *SystemConfiguration,
  IN EFI_PEI_SERVICES            **PeiServices,
  IN UINT16                       PlatformType
  );

EFI_STATUS
PlatformCpuInit (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
  );

EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES           **PeiServices,
  IN CONST EFI_PEI_STALL_PPI          *This,
  IN UINTN                      Microseconds
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices   - GC_TODO: add argument description
  This          - GC_TODO: add argument description
  Microseconds  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

EFI_STATUS
PeimInitializeFlashMap (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  );

EFI_STATUS
PeimInstallFlashMapPpi (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES           **PeiServices
  );

EFI_STATUS
EFIAPI
GetAreaInfo (
  IN  EFI_PEI_SERVICES          **PeiServices,
  IN  EFI_FLASH_AREA_TYPE       AreaType,
  IN  EFI_GUID                  *AreaTypeGuid,
  OUT UINT32                    *NumEntries,
  OUT EFI_FLASH_SUBAREA_ENTRY   **Entries
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices   - GC_TODO: add argument description
  AreaType      - GC_TODO: add argument description
  AreaTypeGuid  - GC_TODO: add argument description
  NumEntries    - GC_TODO: add argument description
  Entries       - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

EFI_STATUS
EFIAPI
PchResetPlatform (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
/*++

Routine Description:

  Provide hard reset PPI service.
  To generate full hard reset.

Arguments:

  PeiServices       General purpose services available to every PEIM.
  
Returns:
  
  Not return        System reset occured.
  EFI_DEVICE_ERROR  Device error, could not reset the system.

--*/
;

BOOLEAN
GetSleepTypeAfterWakeup (
  IN  EFI_PEI_SERVICES          **PeiServices,
  OUT UINT16                    *SleepType
  );

EFI_STATUS
EFIAPI
GetWakeupEventAndSaveToHob (
  IN EFI_PEI_SERVICES   **PeiServices
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;


VOID
EFIAPI
SaveAdminPassToHob (
  IN EFI_PEI_SERVICES   **PeiServices
  )
/*++

Routine Description:
  Admin password is read and saved to HOB 
  to prevent its cleared during defaults load.

Arguments:

  PeiServices - pointer to the PEI Service Table

Returns:

  None.

--*/
;

EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices       - GC_TODO: add argument description
  NotifyDescriptor  - GC_TODO: add argument description
  Ppi               - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;
EFI_STATUS
EFIAPI
PeiGetVariable (
  IN CONST EFI_PEI_READ_ONLY_VARIABLE2_PPI    *This,
  IN CONST CHAR16                             *VariableName,
  IN CONST EFI_GUID                           *VariableGuid,
  OUT UINT32                                  *Attributes,
  IN OUT UINTN                                *DataSize,
  OUT VOID                                    *Data
  )
/*++

Routine Description:

  Provide the read variable functionality of the variable services.

Arguments:

  This             - Pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI.

  VariableName     - The variable name

  VendorGuid       - The vendor's GUID

  Attributes       - Pointer to the attribute

  DataSize         - Size of data

  Data             - Pointer to data

Returns:

  EFI_SUCCESS           - The interface could be successfully installed

  EFI_NOT_FOUND         - The variable could not be discovered

  EFI_BUFFER_TOO_SMALL  - The caller buffer is not large enough

--*/
;

EFI_STATUS
EFIAPI
PeiGetNextVariableName (
  IN CONST EFI_PEI_READ_ONLY_VARIABLE2_PPI    *This,
  IN OUT UINTN                                *VariableNameSize,
  IN OUT CHAR16                               *VariableName,
  IN OUT EFI_GUID                             *VariableGuid
  )
/*++

Routine Description:

  Provide the get next variable functionality of the variable services.

Arguments:

  This             - Pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI.
  VariabvleNameSize  - The variable name's size.
  VariableName       - A pointer to the variable's name.
  VendorGuid         - A pointer to the EFI_GUID structure.

  VariableNameSize - Size of the variable name

  VariableName     - The variable name

  VendorGuid       - The vendor's GUID

Returns:

  EFI_SUCCESS - The interface could be successfully installed

  EFI_NOT_FOUND - The variable could not be discovered

--*/
;

EFI_STATUS
EFIAPI
GetPlatformMemorySize (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      PEI_PLATFORM_MEMORY_SIZE_PPI           *This,
  IN OUT  UINT64                                 *MemorySize
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description
  This        - GC_TODO: add argument description
  MemorySize  - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

EFI_STATUS
UpdateBootMode (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      UINT16                                 PlatformType
  );

EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
EFIAPI
PeimInitializeRecovery (
  IN EFI_PEI_SERVICES     **PeiServices
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  PeiServices - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
;

VOID
CheckPowerOffNow (
  VOID
  );

VOID
PchBaseInit (
  );

VOID
LpcSioEarlyInit (
  VOID
  );

VOID
AST2400EarlyInit (
  VOID
  );


VOID
W83527SioEarlyInit (
  VOID
  );

VOID
PeiIioInit(
  VOID
  );

VOID
PchGpioInit (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN UINT16                      PlatformType,
  IN SYSTEM_CONFIGURATION        *SystemConfiguration
);

EFI_STATUS
PcieSecondaryBusReset (
  IN EFI_PEI_SERVICES  **PeiServices,
  IN UINT8             Bus,
  IN UINT8             Dev,
  IN UINT8             Fun
  );

EFI_STATUS
ConfigurePlatformClocks (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *SmbusPpi
  );

  
//   EDK2_TODO
#define EFI_MAX_ADDRESS   0xFFFFFFFF        /* Definition in EfiBind.h */

EFI_STATUS
PlatformPchUsbInit (
  IN EFI_PEI_SERVICES          **PeiServices
  );

EFI_STATUS
CheckForMfgUuid (
  IN EFI_PEI_SERVICES          **PeiServices
  );

EFI_STATUS
PchLockDescriptorRegion(
  IN EFI_PEI_SERVICES            **PeiServices
  );

#endif
