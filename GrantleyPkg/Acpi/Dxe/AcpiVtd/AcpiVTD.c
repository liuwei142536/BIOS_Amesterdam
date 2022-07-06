/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
Copyright (c) 1996 - 2006, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  AcpiVtd.c

Abstract:

  

--*/

//
// Statements that include other files
//
#include "Token.h"   // AptioV server override: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.
#include <Acpi\AcpiVTD.h>
#include <Acpi\Bdat.h>   // AptioV server override: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.

#include <AcpiOemElinks.h> //AptioV server override: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces

EFI_VTD_SUPPORT_INSTANCE mPrivateData;

// AptioV server override start: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.

// AptioV server override start: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces
UINT8 PLAFTORM_OEM_ID[6]     = ACPI_OEM_ID_MAK;		
UINT8 PLAFTORM_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK;
// AptioV server override End: EIP#494758 - CONVERT_TO_STRING macro replaced to avoid strips leading and trailing spaces

// AptioV server override end: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.

EFI_STATUS
EFIAPI
InsertDmaRemap (
  IN  EFI_DMA_REMAP_PROTOCOL            *This,
  IN  REMAP_TYPE                        RemapType,
  IN  VOID                              *RemapEntry
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  This        - GC_TODO: add argument description
  RemapType   - GC_TODO: add argument description
  RemapEntry  - GC_TODO: add argument description

Returns:

  EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  EFI_SUCCESS           - GC_TODO: Add description for return value

--*/
{
  UINTN                                                       DevIndex;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE                             *Dmar;
  EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE            *Drhd;
  EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE    *Rmrr;
  EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE  *Atsr;
  EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE  *Rhsa;
  EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE                  *DevScope;
  DMAR_DRHD                                                   *DmaRemap;
  DMAR_RMRR                                                   *RevMemRegion;
  DMAR_ATSR                                                   *AtsrRegion;
  DMAR_RHSA                                                   *RhsaRegion;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciPath;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciInputPath;

  Dmar = mPrivateData.Dmar;
  if (RemapType == DrhdType) {
    DmaRemap = (DMAR_DRHD *) RemapEntry;
    ASSERT (DmaRemap->Signature == DRHD_SIGNATURE);
    Drhd                      = (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Drhd->Type                = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_DRHD;
    Drhd->Length              = sizeof (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE);
    Drhd->Flags               = DmaRemap->Flags;
    Drhd->SegmentNumber       = DmaRemap->SegmentNumber;
    Drhd->RegisterBaseAddress = DmaRemap->RegisterBase;
    DevScope                  = NULL;

    for (DevIndex = 0; DevIndex < DmaRemap->DeviceScopeNumber; DevIndex++) {
      DevScope                        = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *) ((UINT8 *) Drhd + Drhd->Length);
      DevScope->DeviceScopeEntryType  = DmaRemap->DeviceScope[DevIndex].DeviceType;
      DevScope->Length                = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
      DevScope->EnumerationID         = DmaRemap->DeviceScope[DevIndex].EnumerationID;
      DevScope->StartingBusNumber     = DmaRemap->DeviceScope[DevIndex].StartBusNumber;
      PciPath                         = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
      PciInputPath                    = (EFI_ACPI_DMAR_PCI_PATH *) DmaRemap->DeviceScope[DevIndex].PciNode;
      while (*(UINT8 *) PciInputPath != (UINT8) -1) {
        CopyMem (PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
        DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        PciInputPath++;
        PciPath++;
      }
      if (DevScope) {
        Drhd->Length = Drhd->Length + (UINT16) DevScope->Length;
      }
    }

    Dmar->Header.Length += Drhd->Length;

  } else if (RemapType == RmrrType) {
    RevMemRegion = (DMAR_RMRR *) RemapEntry;
    ASSERT (RevMemRegion->Signature == RMRR_SIGNATURE);
    Rmrr                                    = (EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Rmrr->Type                              = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RMRR;
    Rmrr->Length                            = sizeof (EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE);
    Rmrr->SegmentNumber                     = RevMemRegion->SegmentNumber;
    Rmrr->ReservedMemoryRegionBaseAddress   = RevMemRegion->RsvdMemBase;
    Rmrr->ReservedMemoryRegionLimitAddress  = RevMemRegion->RsvdMemLimit;
    // Rmrr->ReservedMemoryLimit -= 0x1;
    DevScope = NULL;
    for (DevIndex = 0; DevIndex < RevMemRegion->DeviceScopeNumber; DevIndex++) {
      DevScope                        = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *) ((UINT8 *) Rmrr + Rmrr->Length);
      DevScope->DeviceScopeEntryType  = RevMemRegion->DeviceScope[DevIndex].DeviceType;
      DevScope->StartingBusNumber     = RevMemRegion->DeviceScope[DevIndex].StartBusNumber;
      DevScope->Length                = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
      PciPath                         = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
      PciInputPath                    = (EFI_ACPI_DMAR_PCI_PATH *) RevMemRegion->DeviceScope[DevIndex].PciNode;
      while (*(UINT8 *) PciInputPath != (UINT8) -1) {
        CopyMem (PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
        DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        PciInputPath++;
        PciPath++;
      }

      if (DevScope) {
        Rmrr->Length = Rmrr->Length + (UINT16) DevScope->Length;
      }
    }

    Dmar->Header.Length += Rmrr->Length;

  } else if (RemapType == AtsrType) {
    AtsrRegion = (DMAR_ATSR *) RemapEntry;
    ASSERT (AtsrRegion->Signature == ATSR_SIGNATURE);
    Atsr                  = (EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Atsr->Type            = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_ATSR;
    Atsr->Flags           = AtsrRegion->Flags;
    Atsr->SegmentNumber   = AtsrRegion->SegmentNumber;
    Atsr->Length          = sizeof (EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE);
    DevScope = NULL;
    for (DevIndex = 0; DevIndex < AtsrRegion->DeviceScopeNumber; DevIndex++) {
      if((AtsrRegion->ATSRPresentBit & (01 << DevIndex)) == 00) {
        continue;
      }
      DevScope                        = (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE *) ((UINT8 *) Atsr + Atsr->Length);
      DevScope->DeviceScopeEntryType  = AtsrRegion->DeviceScope[DevIndex].DeviceType;
      DevScope->StartingBusNumber     = AtsrRegion->DeviceScope[DevIndex].StartBusNumber;
      DevScope->Length                = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE);
      PciPath                         = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
      PciInputPath                    = (EFI_ACPI_DMAR_PCI_PATH *) AtsrRegion->DeviceScope[DevIndex].PciNode;
      while (*(UINT8 *) PciInputPath != (UINT8) -1) {
        CopyMem (PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
        DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        PciInputPath++;
        PciPath++;
      }

      if (DevScope) {
        Atsr->Length = Atsr->Length + (UINT16) DevScope->Length;
      }
    }

    Dmar->Header.Length += Atsr->Length;

  } else if (RemapType == RhsaType) {
    RhsaRegion = (DMAR_RHSA *) RemapEntry;
    ASSERT (RhsaRegion->Signature == RHSA_SIGNATURE);
    Rhsa                  = (EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Rhsa->Type            = EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RHSA;
    Rhsa->ProximityDomain = RhsaRegion->Domian;
    Rhsa->RegisterBaseAddress   = RhsaRegion->RegisterBase;
    Rhsa->Length          = sizeof (EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE);
    Dmar->Header.Length += Rhsa->Length;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Dmar->Header.Length < TABLE_SIZE);
  return EFI_SUCCESS;
}

EFI_STATUS
GetTablesInfo (
  UINT8       *Entry,
  BOOLEAN     *Type,
  BOOLEAN     *IncludeAll,
  UINTN       *Length
  )
{
  EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE  *Comm;

  Comm    = (EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE *) Entry;
  *Length = Comm->Length;

  if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RMRR) {
    *Type = RmrrType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_DRHD) {
    *Type = DrhdType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_ATSR) {
    *Type = AtsrType;
  } else if (Comm->Type == EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RHSA) {
    *Type = RhsaType;
  } else {
    *Type = 0xFF;
  }

  if ((Comm->Flags == EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_SET) || (Comm->Flags == EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_SET)) {
    *IncludeAll = TRUE;
  } else {
    *IncludeAll = FALSE;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ReorderTables (
  VOID
  )
{
  BOOLEAN   Type;
  BOOLEAN   IncludeAll;
  UINTN     Length;
  UINTN     CurrLength;
  UINTN     TableLength;
  UINT8     *Ptr;
  UINT8     *PtrOrder;

  Ptr       = (UINT8 *) mPrivateData.Dmar;
  PtrOrder  = (UINT8 *) mPrivateData.DmarOrder;

  CopyMem (PtrOrder, Ptr, sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE));
  PtrOrder += sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);

  TableLength = mPrivateData.Dmar->Header.Length;

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RmrrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == AtsrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RhsaType) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetDmarTable (
  IN  EFI_DMA_REMAP_PROTOCOL            *This,
  IN  VOID                              **DmarTable
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  This      - GC_TODO: add argument description
  DmarTable - GC_TODO: add argument description

Returns:

  EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  EFI_UNSUPPORTED       - GC_TODO: Add description for return value
  EFI_SUCCESS           - GC_TODO: Add description for return value

--*/
{
  if (DmarTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mPrivateData.Dmar->Header.Length <= sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE)) {
    return EFI_UNSUPPORTED;
  }

  ReorderTables ();
  *DmarTable = mPrivateData.DmarOrder;
  return EFI_SUCCESS;
}

EFI_STATUS
VtdTableEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  UINT64                                Size;
  EFI_ACPI_DMAR_DESCRIPTION_TABLE       *Dmar;
  SYSTEM_CONFIGURATION                  mSystemConfiguration;
  EFI_HOB_GUID_TYPE                     *GuidHob;
//  UINT64                                TempOemId, TempOemTableId; // AptioV server override: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.  

  Status = EFI_SUCCESS;


  //
  // Initialize our protocol
  //
  ZeroMem (&mPrivateData, sizeof (EFI_VTD_SUPPORT_INSTANCE));

  mPrivateData.DmaRemapProt.VtdFlag = TRUE;
  Size = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(L"IntelSetup",
                            &gEfiSetupVariableGuid,
                            NULL,
                            &Size,
                            &mSystemConfiguration);
  if (!EFI_ERROR (Status)) {
    if (!(mSystemConfiguration.VTdSupport)) {
      mPrivateData.DmaRemapProt.VtdFlag  = FALSE;
    }
  }

  //
  // Search for the Platform Info PEIM GUID HOB.
  //
  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  mPrivateData.Signature                    = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  mPrivateData.Dmar                         = (EFI_ACPI_DMAR_DESCRIPTION_TABLE *) AllocateZeroPool (3 * TABLE_SIZE);

  //Aptio v server override start: Condition check to resolve CPP check errors.
  if(NULL == mPrivateData.Dmar ) {
      DEBUG ((EFI_D_ERROR, "Assert.  mPrivateData.Dmar  AllocateZeroPool returns NULL\n"));
      ASSERT_EFI_ERROR(EFI_OUT_OF_RESOURCES);
      return EFI_OUT_OF_RESOURCES;
    } 
  //Aptio v server override End: Condition check to resolve CPP check errors.

  mPrivateData.DmarOrder                    = (EFI_ACPI_DMAR_DESCRIPTION_TABLE *) ((UINT8 *) mPrivateData.Dmar + TABLE_SIZE);
  mPrivateData.DmaRemapProt.InsertDmaRemap  = InsertDmaRemap;
  mPrivateData.DmaRemapProt.GetDmarTable    = GetDmarTable;

  if(NULL == mPrivateData.Dmar ) {
      DEBUG ((EFI_D_ERROR, "Asserting the AllocateZeroPool returns NULL\n"));
      ASSERT_EFI_ERROR(EFI_INVALID_PARAMETER);
    } 
  Dmar                          = mPrivateData.Dmar;
  Dmar->Header.Length           = sizeof (EFI_ACPI_DMAR_DESCRIPTION_TABLE);
  Dmar->Header.Signature        = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Revision         = EFI_ACPI_DMAR_DESCRIPTION_TABLE_REVISION;
  Dmar->Header.OemRevision      = EFI_ACPI_DMAR_OEM_REVISION;
  Dmar->Header.CreatorId        = EFI_ACPI_DMAR_OEM_CREATOR_ID;
  Dmar->Header.CreatorRevision  = EFI_ACPI_DMAR_OEM_CREATOR_REVISION;
  Dmar->HostAddressWidth        = PlatformInfo->CpuData.CpuAddressWidth - 1;

  // AptioV server override start: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.  
  /*TempOemId      = PcdGet64(PcdAcpiOemId);
  TempOemTableId = PcdGet64(PcdAcpiTableOemId);

  CopyMem (&Dmar->Header.OemId, &TempOemId, 6);
  CopyMem (&Dmar->Header.OemTableId, &TempOemTableId, 8);*/
  CopyMem (&Dmar->Header.OemId, PLAFTORM_OEM_ID, 6);
  CopyMem ((UINT8 *)&Dmar->Header.OemTableId, PLAFTORM_OEM_TBL_ID, 8);   
  // AptioV server override end: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.  

  Status = gBS->InstallProtocolInterface (
                  &mPrivateData.Handle,
                  &gEfiDmaRemapProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPrivateData.DmaRemapProt
                  );

  return Status;
}
