//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/*++

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AcpiPlatform.h

Abstract:

--*/

#ifndef _ACPI_PLATFORM_H_
#define _ACPI_PLATFORM_H_

//
// Statements that include other header files
//
#include <PiDxe.h>
#include <PchAccess.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <Acpi/DMARemappingReportingTable.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/IioUds.h>
#include <Protocol/DmaRemap.h>
#include <Protocol/PciIo.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SerialIo.h>
//#include <Protocol/LpcPolicy.h>  AptioV server override: Remove LpcPolicy component
#include <Protocol/MpService.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/MpstSupport.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <Guid/HobList.h>
#include <Guid/MemoryMapData.h>
#include <Protocol/PlatformType.h>
#include <Protocol/CpuCsrAccess.h>
//#include <Protocol/IioSystem.h> AptioV server override: File included after Plaftorm.h to avoid redefinition error for PCH_RCBA_ADDRESS
#include <Cpu/CpuRegs.h>
#include <Acpi/Mcfg.h>
#include <Acpi/Hpet.h>
#include <Acpi/Srat.h>
#include <Acpi/Mpst.h>
#include <Acpi/Pmtt.h>
#include <Acpi/Pcct.h>
#include <Acpi/Slit.h>
#include <Acpi/Msct.h>
#include <Acpi/Bdat.h>
#include "Platform.h"
#include <Acpi/AcpiVTD.h>
#include <Library/PciExpressLib.h>
#include <Iio/MmioAccess.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include "RcRegs.h"
#include <Protocol/IioSystem.h>

#include <AmiProtocol.h>

#define APICID_SHIFTBIT_32    5
#define THREAD_MASK_64        (0x3F)
#define THREAD_MASK_32        (0x1F)
#define CPUIDX_SKT_SHIFT_BIT  6
#define SKU_18C               0x3FFFF

//
// +++++++++++++++++++++++++++++++++++++++++++++++++++
// ESS - GV3 Definitions
// +++++++++++++++++++++++++++++++++++++++++++++++++++
//
#pragma pack(1)

typedef struct _AML_GV3_STATE_PACKAGE {
  UINT8   PkgOp;                // 0x12
  UINT8   PkgLeadByte;
  UINT8   NoData;
  UINT8   DWPrefix0;
  UINT32  CoreFreq;
  UINT8   DWPrefix1;
  UINT32  PowerConsumption;
  UINT8   DWPrefix2;
  UINT32  TransactionLatency;
  UINT8   DWPrefix3;
  UINT32  BusMasterLatency;
  UINT8   DWPrefix4;
  UINT32  ControlValue;
  UINT8   DWPrefix5;
  UINT32  StatusValue;
} AML_GV3_STATE_PACKAGE;

typedef struct _AML_PSS_PACKAGE {
  UINT8                 NameOp; // 0x08
  UINT8                 Sign[4];
  UINT8                 PkgOp;  // 0x12
  UINT8                 Res1[2];
  UINT8                 NoOfGV3Packages;
  AML_GV3_STATE_PACKAGE Pstates[16];
} AML_PSS_PACKAGE;

typedef struct _SMM_APICID_MAP {
  UINT32  ApicID;
  UINT16  PhysicalCpuNumber;
  UINT16  CpuNumber;
  UINT16  Version;
} SMM_APICID_MAP;

typedef struct _SMM_GV3_FVID {
  UINT16  VidValue : 8;
  UINT16  FreqRatio : 8;
  UINT16  SystemBusFreq : 16;
} SMM_GV3_FVID;

#pragma pack()

#define GV3_SSDT_OEM_TABLE_IDBASE 0x4000

//
// Private Driver Data
//
//
// Define Union of IO APIC & Local APIC structure;
//
typedef union {
  EFI_ACPI_2_0_PROCESSOR_LOCAL_APIC_STRUCTURE AcpiLocalApic;
  EFI_ACPI_2_0_IO_APIC_STRUCTURE              AcpiIoApic;
  EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC_STRUCTURE AcpiLocalx2Apic;
  struct {
    UINT8 Type;
    UINT8 Length;
  } AcpiApicCommon;
} ACPI_APIC_STRUCTURE_PTR;

//
// Protocol private structure definition
//
EFI_STATUS
InstallAcpiPlatform (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description:

  Entry point of the ACPI platform driver.


Arguments:

  ImageHandle  -  EFI_HANDLE: A handle for the image that is initializing this driver.
  SystemTable  -  EFI_SYSTEM_TABLE: A pointer to the EFI system table.

Returns:

  EFI_SUCCESS           -  Driver initialized successfully.
  EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

--*/
;

EFI_STATUS
EFIAPI
LoadAcpiHwpTables(
  VOID
  )
/*++
Routine Description:

  This function loads the ACPI HWP SSDT Tables.


Arguments:
  None

Returns:

  EFI_SUCCESS           -  Driver initialized successfully.
  EFI_NOT_FOUND			-  HWP SSDT Not Found in FW
  EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

--**/
;

EFI_ACPI_TABLE_VERSION
GetAcpiTableVersion (
  VOID
  )
/*++

Routine Description:

  Get Acpi Table Version.

Arguments:

  ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

Returns:

  EFI_SUCCESS:              Driver initialized successfully
  EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

--*/
;

EFI_STATUS
AcpiPlatformGetOemFields (
  OUT UINT8   *OemId,
  OUT UINT64  *OemTableId,
  OUT UINT32  *OemRevision
  )
/*++

Routine Description:

  The funtion returns Oem specific information of Acpi Platform.

Arguments:

  OemId        -  OemId returned.
  OemTableId   -  OemTableId returned.
  OemRevision  -  OemRevision returned.

Returns:

  EFI_STATUS  -  Status of function execution.

--*/
;

EFI_ACPI_TABLE_VERSION
AcpiPlatformGetAcpiSetting (
  VOID
  )
/*++

Routine Description:

  The function returns Acpi table version.

Arguments:

  None.

Returns:

  EFI_ACPI_TABLE_VERSION  -  Acpi table version encoded as a UINT32.

--*/
;

EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++

Routine Description:

  Entry point for Acpi platform driver.

Arguments:

  ImageHandle  -  A handle for the image that is initializing this driver.
  SystemTable  -  A pointer to the EFI system table.

Returns:

  EFI_SUCCESS           -  Driver initialized successfully.
  EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

--*/
;

EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER  *Table,
  IN     UINTN                   NumberOfCPUs
  );

EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  IN   EFI_GUID       gEfiAcpiMultiTableStorageGuid,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  );

EFI_STATUS
VtdAcpiTablesUpdateFn (
  VOID
  );

VOID
UpdateVtdIntRemapFlag (
  );

#endif
