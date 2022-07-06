/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c)  1999 - 2016, Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AcpiPlatform.c

Abstract:

  ACPI Platform Driver

--*/
#include <Token.h> // AptioV server override

#include "AcpiPlatform.h"
#include "AcpiPlatformHooks.h"
#include "AcpiPlatformHooksLib.h"
#include <IndustryStandard/Acpi.h>
// APTIOV_OVERRIDE_START: EIP#
// To get AmiPlatformAcpiLib functions.
#include <AmiCrbPkg\Include\Library\AmiPlatformAcpiLib.h> 
// APTIOV_OVERRIDE_END: EIP#
#include <Protocol/AcpiTable.h>

#pragma optimize("", off)

// AptioV server override start: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.
#include <AcpiOemElinks.h>
UINT8 PLAFTORM_OEM_ID[6]     = ACPI_OEM_ID_MAK;
UINT8 PLAFTORM_OEM_TBL_ID[8] = ACPI_OEM_TBL_ID_MAK;
// AptioV server override end: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.

EFI_GLOBAL_NVS_AREA_PROTOCOL  mGlobalNvsArea;

BOOLEAN               mFirstNotify;
BOOLEAN               mX2ApicEnabled;
EFI_PLATFORM_INFO    *mPlatformInfo;
BIOS_ACPI_PARAM      *mAcpiParameter;
struct SystemMemoryMapHob *mSystemMemoryMap;
EFI_GUID              mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
SYSTEM_CONFIGURATION  mSystemConfiguration;
UINT8                 KBPresent = 0;
UINT8                 MousePresent = 0;
EFI_IIO_UDS_PROTOCOL  *mIioUds;
EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;
EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *mSpcrTable = NULL;

extern UINT32                mNumOfBitShift;
extern EFI_CPU_ID_ORDER_MAP  mCpuApicIdOrderTable[MAX_CPU_NUM];
extern EFI_GUID gEfiAcpiTableStorageGuid;
extern EFI_GUID gEfiAcpiHwpStorageGuid; // fix compilation error
extern EFI_GUID gEfiGlobalVariableGuid;
extern EFI_GUID gEfiMpServiceProtocolGuid;
//extern EFI_GUID gEfiAcpiSupportProtocolGuid;
extern EFI_GUID gEfiMemoryMapGuid;

// AptioV Server Override start: Fix for ASL name object not being getting updated.
EFI_STATUS LibGetDsdt(EFI_PHYSICAL_ADDRESS *DsdtAddr, EFI_ACPI_TABLE_VERSION Version);
UINT64 OemDsdtTblId;   // AptioV Server Override: Added AMI DSDT OemTableId identification

VOID InitAmiLib(
	IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable
);
// AptioV Server Override end: Fix for ASL name object not being getting updated.

//Aptio V Server Override
VOID AmiUpdatePowerManagementSsdt(IN EFI_ACPI_DESCRIPTION_HEADER *Table);

EFI_STATUS
LocateSupportProtocol (
  IN   EFI_GUID       *Protocol,
  IN   EFI_GUID       EfiAcpiStorageGuid,
  OUT  VOID           **Instance,
  IN   UINT32         Type
  )
/*++

Routine Description:

  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

Arguments:

  Protocol  -  The protocol to find.
  Instance  -  Return pointer to the first instance of the protocol.
  Type      -  The type of protocol to locate. 

Returns:

  EFI_SUCCESS           -  The function completed successfully.
  EFI_NOT_FOUND         -  The protocol could not be located.
  EFI_OUT_OF_RESOURCES  -  There are not enough resources to find the protocol.

--*/
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  FvStatus = 0;
  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  Protocol,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }
  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    Protocol,
                    Instance
                    );
    ASSERT (!EFI_ERROR (Status));

    if (!Type) {
      //
      // Not looking for the FV protocol, so find the first instance of the
      // protocol.  There should not be any errors because our handle buffer
      // should always contain at least one or LocateHandleBuffer would have
      // returned not found.
      //
      break;
    }
    //
    // See if it has the ACPI storage file
    //
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                              *Instance,
                                                              &gEfiAcpiTableStorageGuid,
                                                              NULL,
                                                              &Size,
                                                              &FileType,
                                                              &Attributes,
                                                              &FvStatus
                                                              );

    //
    // If we found it, then we are done
    //
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}

BOOLEAN
IsTerminalDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  OUT UINT8                    *TerminalType,
  OUT UINTN                    *Com
  )
/*++

Routine Description:
  Test whether DevicePath is a valid Terminal

Arguments:
  DevicePath  -   DevicePath to be checked
  TerminalType-   If is terminal, give its type
  Com         -   If is Com Port, give its type
  
Returns:
  TRUE        -   If DevicePath point to a Terminal
  FALSE
  
--*/
{
  UINT8                 *Ptr;
  VENDOR_DEVICE_PATH    *Vendor;
  ACPI_HID_DEVICE_PATH  *Acpi;

  Ptr = (UINT8 *) DevicePath;
  Ptr    = Ptr + GetDevicePathSize (DevicePath) -
           sizeof (VENDOR_DEVICE_PATH) - sizeof (EFI_DEVICE_PATH_PROTOCOL);
  Vendor = (VENDOR_DEVICE_PATH *) Ptr;

  if (Vendor->Header.Type == MESSAGING_DEVICE_PATH &&
      Vendor->Header.SubType == MSG_VENDOR_DP &&
      DevicePathNodeLength (&Vendor->Header) == sizeof(VENDOR_DEVICE_PATH)) {
    //
    // There are four kinds of Terminal types
    // check to see whether this devicepath
    // is one of that type
    //
    if (CompareGuid (&Vendor->Guid, &gEfiVT100Guid)) {
      *TerminalType = 0;
    } else if (CompareGuid (&Vendor->Guid, &gEfiVT100PlusGuid)) {
      *TerminalType = 1;
    } else if (CompareGuid (&Vendor->Guid, &gEfiVTUTF8Guid)) {
      *TerminalType = 2;
    } else if (CompareGuid (&Vendor->Guid, &gEfiPcAnsiGuid)) {
      *TerminalType = 3;
    } else {
      return FALSE;
    }

    Ptr   = Ptr - sizeof (UART_DEVICE_PATH) - sizeof (ACPI_HID_DEVICE_PATH);
    Acpi  = (ACPI_HID_DEVICE_PATH *) Ptr;
    if (Acpi->Header.Type == ACPI_DEVICE_PATH &&
        Acpi->Header.SubType == ACPI_DP &&
        DevicePathNodeLength (&Acpi->Header) == sizeof (ACPI_HID_DEVICE_PATH) &&
        Acpi->HID == EISA_PNP_ID (0x0501)) {
      *Com = Acpi->UID;
      return TRUE;
    }
  }
  return FALSE;
}

BOOLEAN
UpdateSpcrTable (
  )
/*++

Routine Description: 
   
  This function updates SPCR table.  

  Note that user may change CR settings via setup or other methods.
  The SPCR table must match.

Arguments:              
  
  None

Returns:                

  TRUE - SPCR Table updated and is valid.

--*/
{
  EFI_STATUS Status;
  UINT8      BaudRate;
  UINT8      TerminalType;
  UINT32     Control;
  UINTN      Com;
  UINTN      Size;
  EFI_SERIAL_IO_PROTOCOL   *SerialIo;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *DevicePathInstance;
  EFI_HANDLE               Handle;
  //EFI_LPC_POLICY_PROTOCOL  *LpcPolicy; // AptioV server override: Remove LpcPlatform component.

  if (mSpcrTable == NULL) {
    return FALSE;
  }
  // AptioV server override start: Remove LpcPlatform component. COM port data is hard-coded as done in LpcPlatform.
  /*
  //Status = LocateSupportProtocol (&gEfiLpcPolicyProtocolGuid, &LpcPolicy, 0);
  //ASSERT_EFI_ERROR (Status);

  //if (LpcPolicy->PolicyCom.Enable[0] != TRUE) {
  //  return FALSE;
  //}
  //mSpcrTable->BaseAddress.Address = LpcPolicy->PolicyCom.Address[0];
  //mSpcrTable->Irq = (UINT8) LpcPolicy->PolicyCom.Irq[0];
  //mSpcrTable->GlobalSystemInterrupt = mSpcrTable->Irq;
  */
  mSpcrTable->BaseAddress.Address = 0x3F8;
  mSpcrTable->Irq = 4;
  mSpcrTable->GlobalSystemInterrupt = mSpcrTable->Irq;
  // AptioV server override end

  //
  // Locate the COM1 terminal
  //

  DevicePath = GetVariable (
              L"ConIn",
              &gEfiGlobalVariableGuid
              );
  if (DevicePath == NULL) {
    return FALSE;
  }

  TerminalType = 0;
  TempDevicePath = DevicePath;
  do {
    DevicePathInstance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (DevicePathInstance == NULL) {
      break;
    }
    if (IsTerminalDevicePath (DevicePathInstance, &TerminalType, &Com) && Com == 0) {
      break;
    }
    gBS->FreePool (DevicePathInstance);
    DevicePathInstance = NULL;
  } while (TempDevicePath != NULL);

  if (DevicePathInstance == NULL) {
    gBS->FreePool (DevicePath);    
    return FALSE;
  }

  TempDevicePath = DevicePathInstance;
  Status = gBS->LocateDevicePath (
                  &gEfiSerialIoProtocolGuid,
                  &TempDevicePath,
                  &Handle
                  );
  gBS->FreePool (DevicePathInstance);    
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiSerialIoProtocolGuid,
                  (VOID **) &SerialIo
                  );
  ASSERT_EFI_ERROR (Status);

  switch (SerialIo->Mode->BaudRate) {
  case 9600:
    BaudRate = 3;
    break;
  case 19200:
    BaudRate = 4;
    break;
  case 28800:
  case  38400:
	BaudRate = 5;
	break;
  case 57600:
    BaudRate = 6;
    break;
  case 115200:
    BaudRate = 7;
    break;
  default:
    DEBUG(( EFI_D_ERROR, "\nSPCR: Unsupported baud rate!\n" ));
    goto UpdateSpcrExit;
    break;
  }            
  mSpcrTable->BaudRate = BaudRate;
  if (SerialIo->Mode->Parity != NoParity) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support no parity!\n" ));
    goto UpdateSpcrExit;
  }
  if (SerialIo->Mode->DataBits != 8) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support 8 data bits!\n" ));
    goto UpdateSpcrExit;
  }
  if (SerialIo->Mode->StopBits != OneStopBit) {
    DEBUG(( EFI_D_ERROR, "\nSPCR: Only support 1 stop bit!\n" ));
    goto UpdateSpcrExit;
  }
  Status = SerialIo->GetControl (
                        SerialIo,
                        &Control
                        );
  ASSERT_EFI_ERROR (Status);
  
  if (Control & EFI_SERIAL_HARDWARE_FLOW_CONTROL_ENABLE) {
    mSpcrTable->FlowControl |= 2;
  }
  mSpcrTable->TerminalType = TerminalType;
  return TRUE;

UpdateSpcrExit:
  return FALSE;
}

BOOLEAN
IsCpuEnableHook (
  UINT32 ApicId
)
{  
  return TRUE;
}

EFI_STATUS
EFIAPI
LoadAcpiHwpTables(
  VOID
  )
/**
@brief
  This procedure loads the ACPI HWP SSDT tables.

**/
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  INTN                          Instance;
  EFI_ACPI_COMMON_HEADER        *Table;
  UINTN                         Size;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINT32                        FvStatus;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINTN                         TableHandle;
  EFI_ACPI_TABLE_VERSION        Version;
  BOOLEAN                       LoadTable;


  FwVol = NULL;
  Table = NULL;

  ///
  /// Locate FV protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "No Efi Firmware Volume Protocol available.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Efi Firmware Volume Protocol is loaded.\n"));
  }
  ASSERT_EFI_ERROR (Status);

  ///
  /// Look for FV with ACPI storage file
  ///
  for (Index = 0; Index < NumberOfHandles; Index++) {

    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                  HandleBuffer[Index],
                  &gEfiFirmwareVolume2ProtocolGuid,
                  (VOID **) &FwVol
                  );
    ASSERT_EFI_ERROR (Status);
   
    if ((Status == EFI_SUCCESS) && (FwVol != NULL)) {
      ///
      /// See if it has the ACPI storage file
      ///
      Size      = 0;
      FvStatus  = 0;
      Status = FwVol->ReadFile (
                      FwVol,
                      &gEfiAcpiHwpStorageGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

      ///
      /// If we found it, then we are done
      ///
	
      if (Status == EFI_SUCCESS) {
        DEBUG ((DEBUG_INFO, "Acpi Table Storage for RVP is found.\n"));
        break;
      }
    }
  }

  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  ///
  /// By default, a table belongs in all ACPI table versions published.
  ///
  Version = EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0;

  ///
  /// Find the Table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                          FwVol,
                          &gEfiAcpiHwpStorageGuid,
                          EFI_SECTION_RAW,
                          Instance,
                          (VOID **) &Table,
                          &Size,
                          &FvStatus
                          );

    if (!EFI_ERROR (Status)) {
      ///
      /// Check the table size is at least as large as an EFI_ACPI_COMMON_HEADER
      ///
      if (Size < sizeof (EFI_ACPI_COMMON_HEADER)) {
        return EFI_BUFFER_TOO_SMALL;
      }

      LoadTable = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      switch (((EFI_ACPI_DESCRIPTION_HEADER*) TableHeader)->OemTableId) {

      case SIGNATURE_64 ('S', 'S', 'D', 'T', ' ', 'H', 'W', 'P'):
        ///
        /// This is HWP SSDT. Found the table, so load this.
        ///
        LoadTable = TRUE;
        DEBUG ((DEBUG_INFO, "HWP: Found HWP SSDT signature.\n"));
        break;

      default:
        break;
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        ///
        /// Check the length field isn't larger than the size read in section
        ///
        if (Table->Length > Size) {
          return EFI_BAD_BUFFER_SIZE;
        }
        Status = AcpiTable->InstallAcpiTable (
                                  AcpiTable,
                                  Table,
                                  Table->Length,
                                  &TableHandle
                                  );
      }

      ///
      /// Increment the instance
      ///
      Instance++;
      Table = NULL;
    }
  }
  return EFI_SUCCESS;
}

EFI_STATUS
PlatformUpdateTables (
  IN OUT EFI_ACPI_COMMON_HEADER  *Table,
  IN     UINTN                   NumberOfCPUs
  )
/*++

Routine Description: 

  This function will update any runtime platform specific information.  
  This currently includes:
    Setting OEM table values, ID, table ID, creator ID and creator revision.
    Enabling the proper processor entries in the APIC tables.

Arguments:
  
  Table  -  The table to update

Returns:

  EFI_SUCCESS  -  The function completed successfully.

--*/
{
  EFI_ACPI_DESCRIPTION_HEADER               *TableHeader;
  EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *FadtHeader;
  UINT8                                     *CurrPtr;
  UINT8                                     *EndPtr;
  ACPI_APIC_STRUCTURE_PTR                   *ApicPtr;
	UINTN                                     MaximumNumberOfCPUs;
  UINT8                                     CurrProcessor;
//  EFI_STATUS                                Status; // AptioV override
  ACPI_APIC_STRUCTURE_PTR                   *ProcessorLocalApicEntry;
  UINT16                                    LegacyDevice;
  //EFI_LPC_POLICY_PROTOCOL                   *LpcPolicy; // AptioV server override: Remove LpcPlatform component
  UINT8                                     Index;
  UINT8                                     NewIndex;
  UINT8                                     LegacyNodeId;
  UINT8                                     CpuThreadIndex;
  UINT8                                     NodeId;
  UINT64                                    LastEntryBase;
  UINT64                                    MemoryAddress;
  UINT64                                    MemorySize;
  //UINT16                                    LpcDeviceId;
  //UINT16                                    NumberOfHpets;
  //UINT16                                    HpetCapIdValue;
  //UINT32                                    HpetBlockID;
  UINTN                                     HighTopMemory;
  UINTN                                     HotPlugBase;
  UINTN                                     HotPlugLen;      
  UINT32                                    LocalApicCounter;
//  UINT64                                    TempOemId, TempOemTableId;
  UINT8                                     SourceNode, TargetNode;
  UINT8                                     NodeCount, NodesUsed;
// AptioV Server Override Start: SLIT and SRAT changes
  UINT8                                     ElementIndex;
  BOOLEAN                                   TolmChecked= FALSE;
// AptioV Server Override End: SLIT and SRAT changes
  UINT8                                     SocketDomain;
  BOOLEAN                                   LocalNode;
  UINT16                                    NodeArray = 0;
  UINT32                                    Data32;

  TableHeader             = NULL;

  CurrPtr                 = NULL;
  EndPtr                  = NULL;
  ApicPtr                 = NULL;
  CurrProcessor           = 0;
  LocalApicCounter        = 0;
  ProcessorLocalApicEntry = NULL;
  MaximumNumberOfCPUs     = MAX_CPU_NUM;

  if (Table->Signature != EFI_ACPI_1_0_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE &&
      Table->Signature != EFI_ACPI_2_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE
      ) {
    TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;
    //
    // Update the OEMID and OEM Table ID.
    //
// AptioV server override start: OEM ID PCD's are not required. SDL tokens in ACPI.sdl are used to update table ID's.
    /*TempOemId = PcdGet64(PcdAcpiOemId);
    TempOemTableId = PcdGet64(PcdAcpiTableOemId);
    
    CopyMem (&TableHeader->OemId, &TempOemId, 6);
    CopyMem (&TableHeader->OemTableId, &TempOemTableId, 8);*/   
    CopyMem (TableHeader->OemId, PLAFTORM_OEM_ID, 6);
    CopyMem ((UINT8 *)&TableHeader->OemTableId, PLAFTORM_OEM_TBL_ID, 8);    
// AptioV server override end: OEM ID's PCD are not required. SDL tokens in ACPI.sdl are used to update table ID's.
    
    //
    // Update the creator ID
    //
    TableHeader->CreatorId = EFI_ACPI_CREATOR_ID;

    //
    // Update the creator revision
    //
    TableHeader->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
  }
  //
  // Complete this function
  //
  //
  // Update the processors in the APIC table
  //
  switch (Table->Signature) {
  // AptioV Server Override Start:
  #if 0
  case EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_SIGNATURE:  
    //don't need 4.0 if not X2Apic, or 3.0 and less with X2Apic enabled
    if((!mX2ApicEnabled && (TableHeader->Revision == EFI_ACPI_4_0_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION)) ||
       (mX2ApicEnabled && (TableHeader->Revision <= EFI_ACPI_3_0_MULTIPLE_APIC_DESCRIPTION_TABLE_REVISION))) {
      return EFI_UNSUPPORTED;
    }

    CurrPtr = (UINT8 *) &((EFI_ACPI_DESCRIPTION_HEADER *) Table)[1];
    CurrPtr = CurrPtr + 8;
    //
    // Size of Local APIC Address & Flag
    //
    EndPtr  = (UINT8 *) Table;
    EndPtr  = EndPtr + Table->Length;
    while (CurrPtr < EndPtr) {
      ApicPtr = (ACPI_APIC_STRUCTURE_PTR *) CurrPtr;
      switch (ApicPtr->AcpiApicCommon.Type) {
      case EFI_ACPI_3_0_PROCESSOR_LOCAL_APIC:
      case EFI_ACPI_4_0_PROCESSOR_LOCAL_X2APIC:
        //
        // ESS override
        // Fix for Ordering of MADT to be maintained as it is in MADT table.
        //
        // Update processor enabled or disabled and keep the local APIC
        // order in MADT intact
        //
        // Sanity check to make sure proc-id is not arbitrary
        //
        ASSERT (ApicPtr->AcpiLocalApic.AcpiProcessorId < MaximumNumberOfCPUs);
        Status = AppendCpuMapTableEntry (
                    &(ApicPtr->AcpiLocalApic),
                    LocalApicCounter);

        if(Status != EFI_SUCCESS) {
          DEBUG(( EFI_D_ERROR, "Fail to append!\n"));
        } else {
          LocalApicCounter++;
          ASSERT (LocalApicCounter <= MaximumNumberOfCPUs);
          DEBUG(( EFI_D_ERROR, "\n"));
        }
        //
        // If no APIC-ID match, the cpu may not be populated
        //
        break;

      case EFI_ACPI_3_0_IO_APIC:
        //
        // IO APIC entries can be patched here
        //

        //
        // Check if the IOAPIC has been enabled
        //
        if (ApicPtr->AcpiIoApic.IoApicId == IIO0_IOAPIC_ID) {
          if ((mPlatformInfo->SysData.SysIoApicEnable & IIO0_IOAPIC) == 0) {
            //
            // Not enabled, remove it from the MADT by marking the entry as reserved
            //
            ApicPtr->AcpiIoApic.Type = 127;
          }
        }

        if (ApicPtr->AcpiIoApic.IoApicId == IIO1_IOAPIC_ID) {
          if ((mPlatformInfo->SysData.SysIoApicEnable & IIO1_IOAPIC) == 0) {
            //
            // Not enabled, remove it from the MADT by marking the entry as reserved
            //
            ApicPtr->AcpiIoApic.Type = 127;
          }
        }
        if (ApicPtr->AcpiIoApic.IoApicId == IIO2_IOAPIC_ID) {
          if ((mPlatformInfo->SysData.SysIoApicEnable & IIO2_IOAPIC) == 0) {
            //
            // Not enabled, remove it from the MADT by marking the entry as reserved
            //
            ApicPtr->AcpiIoApic.Type = 127;
          }
        }
        if (ApicPtr->AcpiIoApic.IoApicId == IIO3_IOAPIC_ID) {
          if ((mPlatformInfo->SysData.SysIoApicEnable & IIO3_IOAPIC) == 0) {
            //
            // Not enabled, remove it from the MADT by marking the entry as reserved
            //
            ApicPtr->AcpiIoApic.Type = 127;
          }
        }
        break;
      }

      CurrPtr = CurrPtr + ApicPtr->AcpiApicCommon.Length;
    }
    break;
#endif
  // AptioV Server Override End:
  case EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
    //
    // Patch FADT for legacy free
    //
    LegacyDevice  = 0;
    FadtHeader    = (EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE *) Table;
    // AptioV server override Start: Remove LpcPlatform component. Pilot3 has a COM port, so setting the Legacy device
    // flag in FADT/FACP.
    //Status        = gBS->LocateProtocol (&gEfiLpcPolicyProtocolGuid, NULL, &LpcPolicy);
    //if (!EFI_ERROR (Status)) {
    //  if (LpcPolicy->PolicyCom.Enable[0] ||
    //      LpcPolicy->PolicyCom.Enable[1] ||
    //      LpcPolicy->PolicyParallel.Enable[0] ||
    //      LpcPolicy->PolicyFloppy.Enable[0]
    //      ) {
        //
        // GreenCity and Burnside are legacy free system, do not enable 8042 controller
        // even though we have external LPC card
        //
        LegacyDevice |= 1;
//      }
      
//      if(LpcPolicy->PolicyKeyboard.Enable)  {
//        KBPresent     = 1;      // copy later to ACPI NVS
//        LegacyDevice |= 2;      // set 8042 FADT flag
//      }
//      if(LpcPolicy->PolicyMouse.Enable)  {
//        MousePresent  = 1;      // copy later to ACPI NVS
//        LegacyDevice |= 2;      // set 8042 FADT flag
//      }
//    }
    // AptioV server override End: Remove LpcPlatform component. Pilot3 has a COM port, so setting the Legacy device

    if (mSystemConfiguration.PcieGlobalAspm == 0) {
      //
      // 4986782: ASPM being disabled in Linux b/c of wrong setting in ACPI tables
      //          "PCIe ASPM Controls" bit should not be set for CRB in order to enable ASPM Control
      //
      // s#3401575 - Disable CSI L1/L0s, PCIe L0s by default - start
      LegacyDevice |= (1 << 4); // Prevent OS from tampering with ASPM settings
      // s#3401575 - Disable CSI L1/L0s, PCIe L0s by default - end
    }

    FadtHeader->IaPcBootArch = LegacyDevice;
    FadtHeader->Flags |= (mSystemConfiguration.ForcePhysicalModeEnable) ? EFI_ACPI_3_0_FORCE_APIC_PHYSICAL_DESTINATION_MODE : 0;
    //FadtHeader->Flags |= EFI_ACPI_3_0_FORCE_APIC_CLUSTER_MODEL;
    break;

  case EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
    //
    // Patch the memory resource    
    //
    PatchDsdtTable ((EFI_ACPI_DESCRIPTION_HEADER *) Table);
    break;

  case EFI_ACPI_3_0_SECONDARY_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
	//Aptio V Server Override - Start
      AmiUpdatePowerManagementSsdt ((EFI_ACPI_DESCRIPTION_HEADER *) Table);
    //
    // Gv3 support
    //
	//if( mSystemConfiguration.ProcessorEistEnable )
    //  PatchGv3SsdtTable ((EFI_ACPI_DESCRIPTION_HEADER *) Table);
	//Aptio V Server Override - End
    break;

  case EFI_ACPI_2_0_MEMORY_POWER_STATE_TABLE_SIGNATURE:
    //
    // Expand MPST table to match platform memory population.
    //
    if( mSystemConfiguration.Mpst )
      PatchMemoryPowerStateTable((EFI_ACPI_MEMORY_POWER_STATE_TABLE *)Table);
    break;

  case EFI_ACPI_2_0_PMTT_TABLE_SIGNATURE:
    //
    // Expand PMTT table to match platform memory population.
    //
    if( mSystemConfiguration.Mpst )
      PatchPlatformMemoryTopologyTable((EFI_ACPI_PLATFORM_MEMORY_TOPOLOGY_TABLE *)Table);
    break;

  case EFI_ACPI_3_0_PCTT_SIGNATURE:
    //
    // Expand PCCT table to match platform memory population.
    //
    if( mSystemConfiguration.Mpst )
      PatchPlatformCommunicationsChannelTable((EFI_ACPI_PLATFORM_COMMUNICATIONS_CHANNEL_TABLE *)Table);
    break;

  case EFI_ACPI_3_0_HIGH_PRECISION_EVENT_TIMER_TABLE_SIGNATURE:
    //
    // Adjust HPET Table to correct the Base Address
    // Get the address bits in RCRB that configure HPET MMIO space
    //  and create an offset to the pre-defined HEPT base address
    //
    Data32 = (MmioRead32 (PCH_RCBA_ADDRESS + R_PCH_RCRB_HPTC) & 3) << 12;

    //
    // Add the offset to the base address and copy into HPET DSDT table
    //
    ((EFI_ACPI_HIGH_PRECISION_EVENT_TIMER_TABLE_HEADER *) Table)->BaseAddressLower32Bit.Address
       = EFI_ACPI_EVENT_TIMER_BLOCK_ADDRESS + Data32;
    break;

  case EFI_ACPI_3_0_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE:
    //
    // Update MCFG base and end bus number
    //
    ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE *) Table)->Segment[0].BaseAddress  
        = mPlatformInfo->PciData.PciExpressBase;
    ((EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE *) Table)->Segment[0].EndBusNumber
        = (UINT8)RShiftU64 (mPlatformInfo->PciData.PciExpressSize, 20) - 1;
    break;

  case EFI_ACPI_2_0_SYSTEM_LOCALITY_INFORMATION_TABLE_SIGNATURE:
    //
    // Modify SLIT table
    //
    DEBUG(( EFI_D_ERROR, "SLIT: Updating SLIT! NumEntries: %d, CodEn: %d, NumCpus: %d\n", mSystemMemoryMap->numberEntries, mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn, mIioUds->IioUdsPtr->SystemStatus.numCpus ));

    //
    // Need to protect against non-interleaved HAs when COD is disabled.  Only sockets are NUMA nodes in this case.
    //
    if( (mSystemMemoryMap->numberEntries > mIioUds->IioUdsPtr->SystemStatus.numCpus) &&
         !mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn ) {
      NodeCount = mIioUds->IioUdsPtr->SystemStatus.numCpus;
    } else {
      NodeCount = mSystemMemoryMap->numberEntries;
    }

    // AptioV Server Override Start: Changes done to publish only valid SLIT entries
    //((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->Header.Header.Length = sizeof(EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE);

    NodesUsed = (NodeCount*NodeCount);

    ((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->Header.Header.Length = \
            sizeof(EFI_ACPI_3_0_SYSTEM_LOCALITY_DISTANCE_INFORMATION_TABLE_HEADER) + \
            ( NodesUsed*sizeof(EFI_ACPI_NUMBER_OF_SYSTEM_LOCALITIES_STRUCTURE) );
    // AptioV Server Override End: Changes done to publish only valid SLIT entries

    ((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->Header.NumberOfSystemLocalities = NodeCount; 

	Index = 0;
	for (SourceNode=0; SourceNode < NodeCount; SourceNode++) {
		for (TargetNode=0; TargetNode < NodeCount; TargetNode++) {
			if (mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn) {
				if ((ABS(SourceNode - TargetNode)) == 0) {
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ZERO_HOP;
				}else if ((ABS((SourceNode & ~(BIT0)) - (TargetNode & ~BIT0))) == 0) {   // Same socket - Different Node
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ONE_ONE;
				}else if (((ABS((SourceNode & ~(BIT0)) - (TargetNode & ~BIT0))) > 1) && 
						((ABS((SourceNode & ~(BIT0)) - (TargetNode & ~BIT0))) != DISTANT_NODE_4S_EP_COD)) { //Adjacent Socket
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ONE_HOP;
				}else { // Distant Socket
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = TWO_HOP;
				}				
			} else {
				if ((ABS(SourceNode - TargetNode)) == 0) {
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ZERO_HOP;
				}else if ((ABS(SourceNode - TargetNode)) == 1 ) {	// Adjacent socket
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ONE_HOP;
				}else if (((ABS(SourceNode - TargetNode)) > 1) && (ABS(SourceNode - TargetNode) != DISTANT_NODE_4S_EP)) {  // Adjacent socket
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = ONE_HOP;
				}else { // Distant socket
					((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = TWO_HOP;
				}
			}
			Index++;
		}
	}


    // AptioV Server Override Start: Changes done to publish only valid SLIT entries
    #if 0
    //
    //  Zero out the unused nodes in SLIT.
    //
    NodesUsed = NodeCount*NodeCount;
    for (Index = NodesUsed; Index < EFI_ACPI_SYSTEM_LOCALITIES_ENTRY_COUNT; Index++) {
      ((EFI_ACPI_SYSTEM_LOCALITY_INFORMATION_TABLE *) Table)->NumSlit[Index].Entry = 0;
    }
    #endif
    // AptioV Server Override End: Changes done to publish only valid SLIT entries
    break;

  case EFI_ACPI_2_0_STATIC_RESOURCE_AFFINITY_TABLE_SIGNATURE:
    //don't need 4.0 if not X2Apic, or 3.0 and less with X2Apic enabled
// AptioV Server Override Start: Do not consider mX2ApicEnabled.
#if 0
    if((!mX2ApicEnabled && (TableHeader->Revision == EFI_ACPI_4_0_SYSTEM_RESOURCE_AFFINITY_TABLE_REVISION)) ||
       (mX2ApicEnabled && (TableHeader->Revision <= EFI_ACPI_3_0_SYSTEM_RESOURCE_AFFINITY_TABLE_REVISION))) {
      return EFI_UNSUPPORTED;
    }
#endif
// AptioV Server Override End: Do not consider mX2ApicEnabled.
    //
    // Correct SRAT Revision in the header
    //
    ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Header.Header.Revision = 3;

    //
    // Modify SRAT table.  Added condition to enable booting.
    //
    if (mSystemMemoryMap != NULL) {

      //
      // Construct Node Array for use in SRAT population
      //
      for (Index = 0; Index < mSystemMemoryMap->numberEntries; Index++) {
        NodeArray |= (1 << mSystemMemoryMap->Element[Index].NodeId);
      }

      if (mSystemConfiguration.SratCpuHotPlug) {
        PatchSRATableAllApicIds((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *)Table);
      } else {
        for (CpuThreadIndex = 0; CpuThreadIndex < EFI_ACPI_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE_COUNT; CpuThreadIndex++) {
          if (CpuThreadIndex < NumberOfCPUs) {
// AptioV Server Override Start: Do not consider mX2ApicEnabled.
#if 0
            if(mX2ApicEnabled) {
              ((EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->x2Apic[CpuThreadIndex].ProximityDomain = mCpuApicIdOrderTable[CpuThreadIndex].SocketNum;
              ((EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->x2Apic[CpuThreadIndex].X2ApicId = mCpuApicIdOrderTable[CpuThreadIndex].ApicId;
              ((EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->x2Apic[CpuThreadIndex].Flags = EFI_ACPI_4_0_PROCESSOR_LOCAL_APIC_SAPIC_ENABLED;
            } else {
#else
            {
#endif
// AptioV Server Override End: Do not consider mX2ApicEnabled.
              //
              //  Find the local Socket that contains the NUMA node
              //             
              SocketDomain = (UINT8)mCpuApicIdOrderTable[CpuThreadIndex].SocketNum + 1;
              LocalNode = FALSE;

              while (LocalNode == FALSE) {
                SocketDomain--;
                if(((NodeArray >> SocketDomain) & 0x01) == 1  || 
                      ((NodeArray >> (SocketDomain + 4)) & 0x01) == 1) {  // Check local nodes
                  LocalNode = TRUE;
                  break;
                }
              }

              //
              //  If COD is enabled, there is 1 extra Proximity Domain per socket
              //
              if (mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn) {

                if(((mCpuApicIdOrderTable[CpuThreadIndex].ApicId & ((1 << mNumOfBitShift) - 1)) >> (mNumOfBitShift - 1)) % 2) { //odd core#
                  //
                  // Prefer 2nd HA for Odd cores
                  //
                  if(((NodeArray >> (SocketDomain + 4)) & 0x01) == 1) {
                    SocketDomain = (SocketDomain << 1) + 1;
                  } else {
                    SocketDomain = (SocketDomain << 1);
                  }
                } else { //even core#
                  //
                  // Prefer 1st HA for Even Cores
                  //
                  if(((NodeArray >> SocketDomain) & 0x01) == 1) {
                    SocketDomain = (SocketDomain << 1);
                  } else {
                    SocketDomain = (SocketDomain << 1) + 1;
                  }
                }
                ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Apic[CpuThreadIndex].ProximityDomain = SocketDomain;
              } else {
                ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Apic[CpuThreadIndex].ProximityDomain = SocketDomain;
              }

              ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Apic[CpuThreadIndex].ApicId = (UINT8)mCpuApicIdOrderTable[CpuThreadIndex].ApicId;
              ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Apic[CpuThreadIndex].Flags = EFI_ACPI_PROCESSOR_LOCAL_APIC_SAPIC_ENABLED;
            }
          } else {
// AptioV Server Override Start: Do not consider mX2ApicEnabled.
#if 0
            if(mX2ApicEnabled) {
              ((EFI_ACPI_4_0_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->x2Apic[CpuThreadIndex].X2ApicId = (UINT32)-1;
            } else {
#else
            {
#endif
// AptioV Server Override End: Do not consider mX2ApicEnabled.
              ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Apic[CpuThreadIndex].ApicId = 0xFF;
            }
          }
        }
      }

      LegacyNodeId = 0xFF;
      LastEntryBase = (UINT64)(-1);
      DEBUG(( EFI_D_ERROR, "\nSRAT: Updating SRAT memory information!\n" ));
      for (Index = 0, ElementIndex =0; ElementIndex < mSystemMemoryMap->numberEntries; ElementIndex++, Index++) { // AptioV Server Override: SRAT changes: ElementIndex is used.

        NodeId = mSystemMemoryMap->Element[ElementIndex].NodeId; // AptioV Server Override: SRAT changes: Index is replaced with ElementIndex

        ASSERT (NodeId >= 0 && NodeId < MAX_MEM_NODE); 

        if (Index >= EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT) {
          ASSERT (0);
          break;
        }

        //
        // As the HOB has base addr in 64 MB chunks
        //
        MemoryAddress = ((UINT64)mSystemMemoryMap->Element[ElementIndex].BaseAddress << MEM_ADDR_SHFT_VAL); // AptioV Server Override: SRAT changes: Index is replaced with ElementIndex
        if (MemoryAddress == LastEntryBase) {
          //
          // Skip duplicate entries if applicable
          //
          continue;
        }

        ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseLow = (UINT32)(MemoryAddress & 0xFFFFFFFF);
        ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseHigh  = (UINT32)((UINT64)MemoryAddress >> 32);
        LastEntryBase = MemoryAddress;

        //
        // As the HOB has Length in 64 MB chunks
        //
        MemorySize  = ((UINT64)mSystemMemoryMap->Element[ElementIndex].ElementSize << MEM_ADDR_SHFT_VAL); // AptioV Server Override: SRAT changes: Index is replaced with ElementIndex
        ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthLow = (UINT32)(MemorySize & 0xFFFFFFFF);
        ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthHigh = (UINT32)((UINT64)MemorySize >> 32);

        //
        // Find Proximity Domain from NodeId
        // NodeId = {0, 4, 1, 5, 2, 6, 3, 7} for 8 Memory controllers on 4 socket COD system
        //
        if (mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn) {  // COD enabled
          if(NodeId > 3)
            SocketDomain = ((NodeId - 4) << 1) + 1;
          else
            SocketDomain = NodeId << 1;
        } else {  // Numa with no COD
          if(NodeId > 3)
            SocketDomain = NodeId - 4;
          else
            SocketDomain = NodeId;
        }

        //
        // Fill in Proximity Domain for memory node
        //
        ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].ProximityDomain = SocketDomain;

        if (MemoryAddress == 0 && MemorySize > 0) {
          LegacyNodeId = NodeId;
        }

        // AptioV Server Override Start: Added code to consider TOLM for Memory Affinity Structure
        if( (!TolmChecked) && (MemoryAddress + MemorySize > mPlatformInfo->MemData.MemTolm) ) {

            #define FOUR_GB_MEM_IN_BYTES    0x100000000

            UINT64      RemainingSize = MemorySize;
            UINT64      LowGap = FOUR_GB_MEM_IN_BYTES-mPlatformInfo->MemData.MemTolm;

            MemorySize = mPlatformInfo->MemData.MemTolm - MemoryAddress;

            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthLow = (UINT32)(MemorySize & 0xFFFFFFFF);
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthHigh = (UINT32)((UINT64)MemorySize >> 32);
            //
            // Enable the Memory structure
            //
            if ((LegacyNodeId == NodeId) || (!mSystemConfiguration.SratMemoryHotPlug)) {
                
              ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].Flags = EFI_ACPI_MEMORY_ENABLED;
              DEBUG(( EFI_D_ERROR, "\nEntry %d in SRAT Enabled. Base - %08x%08x, Length - %08x%08x Proximity - %d\n", Index,
                      (UINT64)((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseHigh,
                      ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseLow,
                      (UINT64)((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthHigh,
                      ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthLow,
                      ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].ProximityDomain));
            } else {
              ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].Flags = EFI_ACPI_MEMORY_ENABLED | EFI_ACPI_MEMORY_HOT_REMOVE_SUPPORTED;
            }

            //
            // Fill Next Memory Affinity structure
            //
            Index++;
            MemoryAddress = FOUR_GB_MEM_IN_BYTES;
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseLow = (UINT32)(MemoryAddress & 0xFFFFFFFF);
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseHigh  = (UINT32)((UINT64)MemoryAddress >> 32);
            LastEntryBase = MemoryAddress;

            RemainingSize -= (MemorySize + LowGap);
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthLow = (UINT32)(RemainingSize & 0xFFFFFFFF);
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthHigh = (UINT32)((UINT64)RemainingSize >> 32);
            //
            // Fill in Proximity Domain for memory node
            //
            ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].ProximityDomain = SocketDomain;
            //
            // Update the TolmChecked boolean variable.
            //
            TolmChecked = TRUE;
        }
        // AptioV Server Override End: Added code to consider TOLM for Memory Affinity Structure

        //
        // Enable the Memory structure
        //
        if ((LegacyNodeId == NodeId) || (!mSystemConfiguration.SratMemoryHotPlug)) {
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].Flags = EFI_ACPI_MEMORY_ENABLED;
          DEBUG(( EFI_D_ERROR, "\nEntry %d in SRAT Enabled. Base - %08x%08x, Length - %08x%08x Proximity - %d\n", Index,
                  (UINT64)((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseHigh,
                  ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].AddressBaseLow,
                  (UINT64)((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthHigh,
                  ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].LengthLow,
                  ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].ProximityDomain));
        } else {
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[Index].Flags = EFI_ACPI_MEMORY_ENABLED | EFI_ACPI_MEMORY_HOT_REMOVE_SUPPORTED;
        }
      }

      HotPlugBase = 0x100;
      if (mSystemConfiguration.MemoryHotPlugBase == 0) { // Auto
        // Read the actual TOHM and set it as the hot memory base
        HighTopMemory  = (UINT64)mIioUds->IioUdsPtr->SystemStatus.tohmLimit << 26;
        HotPlugBase = (HighTopMemory >> 32);
        if ((UINT32)HighTopMemory > 0) {
          HotPlugBase++;
        }
      } else if (mSystemConfiguration.MemoryHotPlugBase) { // Number
        HotPlugBase = mSystemConfiguration.MemoryHotPlugBase * 0x80;
      }
      HotPlugLen = (mSystemConfiguration.MemoryHotPlugLen + 1) * 0x10;

      if (mSystemConfiguration.SratMemoryHotPlug) {
        for (NodeId = 0; NodeId < MAX_MEM_NODE; NodeId++) {
          NewIndex = Index + NodeId;
          if (NewIndex >= EFI_ACPI_MEMORY_AFFINITY_STRUCTURE_COUNT) {
            ASSERT (0);
            break;
          }
         //
          // As the HOB has base addr in 1 GB chunks
          //
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].ProximityDomain = (NodeId >> 1);
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].AddressBaseLow  = 0;
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].AddressBaseHigh = (UINT32)(HotPlugBase +  NodeId * HotPlugLen);
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].LengthLow  = 0;
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].LengthHigh = (UINT32)HotPlugLen;
          ((EFI_ACPI_STATIC_RESOURCE_AFFINITY_TABLE *) Table)->Memory[NewIndex].Flags = EFI_ACPI_MEMORY_ENABLED | EFI_ACPI_MEMORY_HOT_REMOVE_SUPPORTED;
        }
      }
    }
    break;

  case EFI_ACPI_4_0_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE_SIGNATURE:
    PatchMsctAcpiTable((EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE *) Table);
    break;


#ifdef BDAT_SUPPORT
  case EFI_BDAT_TABLE_SIGNATURE:
    PatchBdatAcpiTable((EFI_BDAT_ACPI_DESCRIPTION_TABLE *) Table);
    break;
#endif

  default:
    break;
  }
  //
  //
  // Update the hardware signature in the FACS structure
  //
  //
  //
  return EFI_SUCCESS;
}


STATIC
VOID
EFIAPI
OnReadyToBoot (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  Event   - GC_TODO: add argument description
  Context - GC_TODO: add argument description

Returns:

  GC_TODO: add return values

--*/
{
  EFI_STATUS                  Status;
  EFI_ACPI_TABLE_VERSION      TableVersion;
  UINTN                       TableHandle;
// APTIOV_OVERRIDE_START
//  EFI_ACPI_SUPPORT_PROTOCOL   *AcpiSupport;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
// APTIOV_OVERRIDE_END
 UINTN                       VariableSize;
  EFI_CPUID_REGISTER          CpuidRegisters;   

  if (mFirstNotify) {
    return ;
  }

  mFirstNotify = TRUE;

  //
  // Update the ACPI parameter blocks finall
  //
  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable (
                            L"IntelSetup",
                            &mSystemConfigurationGuid,
                            NULL,
                            &VariableSize,
                            &mSystemConfiguration
                            );
  ASSERT_EFI_ERROR (Status);
  
  mAcpiParameter->TpmEnable = mSystemConfiguration.TpmSupport;

  // CpuPm.Asl: External (CSEN, FieldUnitObj)
  mAcpiParameter->CStateEnable = mSystemConfiguration.ProcessorCcxEnable;
  // CpuPm.Asl: External (C3EN, FieldUnitObj)
  mAcpiParameter->C3Enable     = mSystemConfiguration.C3Enable;
  // CpuPm.Asl: External (C6EN, FieldUnitObj)
  mAcpiParameter->C6Enable     = mSystemConfiguration.C6Enable;
  if(mAcpiParameter->C6Enable && mAcpiParameter->C3Enable) {  //C3 and C6 enable are exclusive
    mAcpiParameter->C6Enable = 1;
    mAcpiParameter->C3Enable = 0;
  }
  // CpuPm.Asl: External (C7EN, FieldUnitObj)
  mAcpiParameter->C7Enable     = 0; //made 0 for HSX 
  // CpuPm.Asl: External (OSCX, FieldUnitObj)
  mAcpiParameter->OSCX         = mSystemConfiguration.OSCx;
  // CpuPm.Asl: External (MWOS, FieldUnitObj)
  mAcpiParameter->MonitorMwaitEnable = mSystemConfiguration.MonitorMwaitEnable;
  // CpuPm.Asl: External (PSEN, FieldUnitObj)
  mAcpiParameter->PStateEnable = mSystemConfiguration.ProcessorEistEnable;
  //HWPM changes-start
  // CpuPm.Asl: External (HWEN, FieldUnitObj)
   mAcpiParameter->HWPMEnable = mSystemConfiguration.ProcessorHWPMEnable;
   // CpuPm.Asl: External (ACEN, FieldUnitObj)
   mAcpiParameter->AutoCstate = mSystemConfiguration.ProcessorAutonumousCstateEnable;
  //HWPM changes-end
  
  //mAcpiParameter->HWAllEnable = mSystemConfiguration.ProcessorEistPsdFunc; //Update in PatchGv3SsdtTable 

  mAcpiParameter->KBPresent    = KBPresent;
  mAcpiParameter->MousePresent = MousePresent;
  mAcpiParameter->TStateEnable = mSystemConfiguration.TStateEnable;
  //Fine grained T state
  AsmCpuid (EFI_CPUID_POWER_MANAGEMENT_PARAMS,  &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
 if(CpuidRegisters.RegEax & EFI_FINE_GRAINED_CLOCK_MODULATION){
  mAcpiParameter->TStateFineGrained = 1;   
 }

  mAcpiParameter->EMcaLogEn    = mSystemConfiguration.EMcaLogEn;
  mAcpiParameter->WheaSupport  = mSystemConfiguration.WheaSupport;

  mAcpiParameter->PcieAcpiHotPlugEnable = (UINT8) (BOOLEAN) (mSystemConfiguration.PcieAcpiHotPlugEnable != 0);  
  //
  // Initialize USB3 mode from setup data
  //
  // If mode != manual control
  //  just copy mode from setup
  if (mSystemConfiguration.PchUsb30Mode != 4) {
    mAcpiParameter->XhciMode = mSystemConfiguration.PchUsb30Mode;

  // Mode = manual control - only enable or disable
  //
  } else {
    mAcpiParameter->XhciMode = mSystemConfiguration.UsbXhciSupport;
  }

  //
  // Find the AcpiSupport protocol
  //
//  Status = LocateSupportProtocol (&gEfiAcpiSupportProtocolGuid, &AcpiSupport, 0);
  // Find the AcpiTable protocol
  //
// APTIOV OVERRIDE START: EIP#470404
// To support EDKII implemetation of ACPI Protocols
//#if 0
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );
  ASSERT_EFI_ERROR (Status);


  TableVersion  = EFI_ACPI_TABLE_VERSION_2_0;

  //
  if (UpdateSpcrTable ()) {
    //
    // Add SPCR table
    //
    TableVersion = EFI_ACPI_TABLE_VERSION_2_0;
    TableHandle = 0;
      Status = AcpiTable->InstallAcpiTable (
        AcpiTable,
        mSpcrTable,
        mSpcrTable->Header.Length,
        &TableHandle
        );
    ASSERT_EFI_ERROR (Status);
  }
  if (mSpcrTable != NULL) {
    gBS->FreePool (mSpcrTable);
  }
//#endif
// APTIOV OVERRIDE END: EIP#470404
  VtdAcpiTablesUpdateFn();

  
  DisableAriForwarding();

  //
  // Publish ACPI 1.0 or 2.0 Tables
  //
// APTIOV OVERRIDE START: EIP#470404
#if 0
  Status = AcpiSupport->PublishTables (
                          AcpiSupport,
                          TableVersion
                          );
  ASSERT_EFI_ERROR (Status);
#endif
// APTIOV OVERRIDE END: EIP#470404

  gBS->CloseEvent(Event); //AptioV server override: Close event so that is isn't called again.
}


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
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  BOOLEAN                       Installed;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance = 0;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;
  EFI_EVENT                     Event;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  VOID                          *HobList;
  UINTN                         VarSize;
  EFI_PHYSICAL_ADDRESS          AcpiParameterAddr;
  EFI_HANDLE                    Handle;
  EFI_HOB_GUID_TYPE             *GuidHob;
  EFI_CPUID_REGISTER            CpuidRegisters;  
  UINT32                        Index;
  UINT32                        TempVal = 0;
  EFI_MP_SERVICES_PROTOCOL      *MpService;
  UINTN                         NumberOfCPUs;
  UINTN                         NumberOfEnabledCPUs;
  UINTN                         MaximumNumberOfCPUs;


  mFirstNotify      = FALSE;

  TableVersion      = EFI_ACPI_TABLE_VERSION_2_0;
  Instance          = 0;
  CurrentTable      = NULL;
  TableHandle       = 0;

  //
  // Update HOB variable for PCI resource information
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  mPlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mPlatformInfo);

  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid,NULL,&mIioUds);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  //
  // Search for the Memory Map GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob    = GetFirstGuidHob (&gEfiMemoryMapGuid);
  mSystemMemoryMap = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mSystemMemoryMap);
  VarSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(
                            L"IntelSetup",
                            &gEfiSetupVariableGuid,
                            NULL,
                            &VarSize,
                            &mSystemConfiguration
                            );
  if (EFI_ERROR (Status)) {
    mSystemConfiguration.ProcessorEistEnable = 0;
    mSystemConfiguration.PackageCState       = 0;
    mSystemConfiguration.ProcessorCcxEnable  = 0;
    mSystemConfiguration.PwrPerfTuning       = 1;
    mSystemConfiguration.TurboMode           = 0;
    mSystemConfiguration.ProcessorXapic      = 0;
    mSystemConfiguration.X2ApicOptOut        = 0;
    mSystemConfiguration.InterleaveType      = 0;
    mSystemConfiguration.Srat                = 1;
  }

  //
  // Find the AcpiTable protocol
  //
//  Status = LocateSupportProtocol (&gEfiAcpiSupportProtocolGuid, &AcpiSupport, 0);
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
//  Status = LocateSupportProtocol (&gEfiFirmwareVolume2ProtocolGuid, &FwVol, 1);
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &FwVol,
            TRUE
            );
  ASSERT_EFI_ERROR (Status);

  Status    = EFI_SUCCESS;
  Instance  = 0;

  PlatformHookInit ();
  UpdateVtdIntRemapFlag();

  //
  // Allocate 256 runtime memory to pass ACPI parameter
  // This Address must be < 4G because we only have 32bit in the dsdt
  //
  AcpiParameterAddr = 0xffffffff;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof(BIOS_ACPI_PARAM)),
                  &AcpiParameterAddr
                  );  
  ASSERT_EFI_ERROR (Status); 
  mAcpiParameter = (BIOS_ACPI_PARAM *)AcpiParameterAddr;

  DEBUG ((EFI_D_ERROR, "ACPI Parameter Block Address: 0x%X\n", mAcpiParameter));

  ZeroMem (mAcpiParameter, sizeof (BIOS_ACPI_PARAM));
  mAcpiParameter->PlatformId    = (UINT32)mPlatformInfo->BoardId;
  mAcpiParameter->IoApicEnable  = mPlatformInfo->SysData.SysIoApicEnable;
#ifdef	DE_SKU
  mAcpiParameter->SOLE= mPlatformInfo->DisableSCUart;
  mAcpiParameter->STEP =mPlatformInfo->CpuStepping;
#endif

  mAcpiParameter->CpuType = mIioUds->IioUdsPtr->SystemStatus.cpuType;
  DEBUG ((EFI_D_ERROR, "CPU is : 0x%X\n", mAcpiParameter->CpuType));

  Handle = NULL;
  mGlobalNvsArea.Area = mAcpiParameter;
  Status = gBS->InstallProtocolInterface ( //AptioV status override: Assign result to Status
         &Handle,
         &gEfiGlobalNvsAreaProtocolGuid,
         EFI_NATIVE_INTERFACE,
         &mGlobalNvsArea
         );  
  ASSERT_EFI_ERROR (Status);

  AsmCpuid (EFI_CPUID_VERSION_INFO,  &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  mAcpiParameter->ProcessorId = (CpuidRegisters.RegEax & 0xFFFF0);
  

  //Counter number of cores
  for (Index = 0; Index < 32; Index++) {
    if ((mPlatformInfo->CpuData.SkuSlices >> Index) & 0x1) {
      TempVal++;
    }
  }

  // support up to 64 threads/socket
  AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;

  mAcpiParameter->ProcessorApicIdBase[0] = (UINT32) (0 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[1] = (UINT32) (1 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[2] = (UINT32) (2 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[3] = (UINT32) (3 << mNumOfBitShift);

  // Set the bit shift value for CPU SKU
  mAcpiParameter->CpuSkuNumOfBitShift = (UINT8) mNumOfBitShift;

  //
  // If Cluster On Die is enabled, set the ACPI variable for PXM value
  //
  if(!mIioUds->IioUdsPtr->SystemStatus.OutClusterOnDieEn){
    mAcpiParameter->ClusterOnDieEnable = 0;
  } else {
    mAcpiParameter->ClusterOnDieEnable = 1;
  }

#ifndef  GRANGEVILLE_FLAG  
  //
  // If Turbo Boost Max Technology 3.0 isn't supported, set the ACPI variable for Nitrous
  //
  if((mPlatformInfo->BoardId == TypeHedtSKU1CRB) ||
     (mPlatformInfo->BoardId == TypeHedtEv)      ||
     (mPlatformInfo->BoardId == TypeHedtDDR3)    ||
     (mPlatformInfo->BoardId == TypeAztecCityCRBSRP)
     ){
    Status = GetCoreControlCapability ();
    if (Status == EFI_SUCCESS) {
      mAcpiParameter->NitrousNotSupport = 0;  // Support
    } else {
      mAcpiParameter->NitrousNotSupport = 1;  // Not support
    }
  } else {
    mAcpiParameter->NitrousNotSupport = 1;    // Not support
  }
#endif

  //
  // Locate the MP services protocol
  //
  //
  // Find the MP Protocol. This is an MP platform, so MP protocol must be
  // there.
  //
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &MpService
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Determine the number of processors
  //
  MpService->GetNumberOfProcessors (
              MpService,
              &NumberOfCPUs,
              &NumberOfEnabledCPUs
              );
  MaximumNumberOfCPUs = MAX_CPU_NUM;

  DEBUG(( EFI_D_ERROR, "NumberOfCPUs:%d MaximumNumberOfCPUs:%d NumberOfEnabledCPUs:%d\n",NumberOfCPUs,MaximumNumberOfCPUs,NumberOfEnabledCPUs ));

  ASSERT (NumberOfCPUs <= MAX_CPU_NUM && NumberOfEnabledCPUs >= 1);

	//
	// Move all valid APIC ID entries to the top of the array
	// Array pointer is mCpuApicIdOrderTable
	// MADT and SRAT tables need a list of APIC IDs
	//
	SortCpuLocalApicInTable (
									MpService,
									NumberOfCPUs
									);
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;
    TableVersion = EFI_ACPI_TABLE_VERSION_NONE;
    TableHandle = 0;
    Installed = FALSE;

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
    // AptioV Server Override-Start: Fix for ASL name object not being getting updated.

    //
    // DSDT table is published in AMI ACPI module by reading from Dsdt.aml file.
    // All the updates to ASL name objects are done in that DSDT table.
    // Here, DSDT table is again published by reading from DSDT.aml file.
    // Because of this, all the updates which are already done till now to ASL name objects are getting lost.
    // So getting and using the DSDT table which was already published by AMI ACPI module
    //
        
        if (CurrentTable->Signature != EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
//            Instance++;
//            continue;
        }
        else {
            DEBUG ((EFI_D_ERROR,"*** DSDT Found***\n"));

            
        }
        
        /*
      if (CurrentTable->Signature == EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
          EFI_PHYSICAL_ADDRESS           DsdtAddress = 0;
          EFI_ACPI_DESCRIPTION_HEADER           *Dsdt;
          //
          // Initialize Global variables.
          //
          InitAmiLib(ImageHandle,SystemTable);
          //
          // Get AMI DSDT using library function.
          //
          Status=LibGetDsdt(&DsdtAddress,EFI_ACPI_TABLE_VERSION_ALL);
          if(EFI_ERROR(Status)){
              DEBUG ((EFI_D_ERROR,"AcpiPlatform: Failed to Get DSDT - returned %r\n", Status));
              ASSERT_EFI_ERROR(Status);
          } else {
              Dsdt = (EFI_ACPI_DESCRIPTION_HEADER*)DsdtAddress;
              OemDsdtTblId = Dsdt->OemTableId;  // AptioV Server Override: Added AMI DSDT OemTableId identification
              DEBUG ((EFI_D_ERROR,"AcpiPlatform: AMI DSDT found !! DSDT address: %x   DSDT OEM Table ID: %lx\n", DsdtAddress, OemDsdtTblId));
              CopyMem (CurrentTable, Dsdt, Dsdt->Length);
          }
      }
      */
      // AptioV Server Override-End: Fix for ASL name object not being getting updated.

      //
      // Allow platform specific code to reject the table or update it
      //
      AcpiStatus = AcpiPlatformHooksIsActiveTable (CurrentTable);
      
      
      if (CurrentTable->Signature == EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE){
          AcpiStatus = EFI_SUCCESS;
      }    
    
    
      if (!EFI_ERROR (AcpiStatus)) {
        //
        // Perform any table specific updates.
        //
        AcpiStatus = PlatformUpdateTables (CurrentTable, NumberOfCPUs);
        if (!EFI_ERROR (AcpiStatus)) {
          //
          // Add the table
          //
          TableHandle = 0;
// APTIOV_OVERRIDE_START: EIP#460399
// To support EDKII implemetation of ACPI Protocols
//          AcpiStatus = AcpiSupport->SetAcpiTable (
//                                    AcpiSupport,
//                                    CurrentTable,
//                                    TRUE,
//                                    TableVersion,
//                        &TableHandle
//                       );
                AcpiStatus = AcpiTable->InstallAcpiTable (AcpiTable, CurrentTable, CurrentTable->Length, &TableHandle);
                if (!EFI_ERROR (AcpiStatus)) {
                  Installed = TRUE;
                }
// APTIOV_OVERRIDE_END: EIP#460399
//APTIOV_OVERRIDE_END
// Aptio V server override start - AMI ACPI DRV rejects FACS & FACP SetAcpiTable.
//-          ASSERT_EFI_ERROR (AcpiStatus);
          if (EFI_ERROR (AcpiStatus)) 
            DEBUG ((EFI_D_ERROR, "AcpiPlatformEntryPoint Fail to SetAcpiTable : %c%c%c%c\n", 
           		(UINT8) CurrentTable->Signature,
           		(UINT8) (CurrentTable->Signature >> 8),
           		(UINT8) (CurrentTable->Signature >> 16),
           		(UINT8) (CurrentTable->Signature >> 24)));
// Aptio V server override end
 
        }
      }
      
      // APTIOV_OVERRIDE_START
      // To free memory allocated by ReadSection
      FreePool(CurrentTable);
      // APTIOV_OVERRIDE_END
      //
      // Increment the instance
      //
      Instance++;
    }
  }

  //
  // Load HWP SSDT only for BDX CPU if HWPM knob is enabled, after performing updates to the legacy tables as necessary. 
  // 
  if ((mIioUds->IioUdsPtr->SystemStatus.cpuType == CPU_BDX) && (mSystemConfiguration.ProcessorHWPMEnable == 1)) {
	LoadAcpiHwpTables();
  }
		  
  //
  // Publish ACPI Tables
  //
// APTIOV OVERRIDE START: EIP#470404
#if 0
  Status = AcpiSupport->PublishTables (
                          AcpiSupport,
                          EFI_ACPI_TABLE_VERSION_2_0
                          );
  ASSERT_EFI_ERROR (Status);
#endif
// APTIOV OVERRIDE END: EIP#470404
  Status = EfiCreateEventReadyToBootEx(
             TPL_NOTIFY,
             OnReadyToBoot,
             NULL,
             &Event
             );

  //
  // Finished
  //
  return EFI_SUCCESS;
}
