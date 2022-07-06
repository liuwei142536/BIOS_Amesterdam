/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

    SetupInfoRecords.c

Abstract:

    This is the filter driver to retrieve data hub entries.

Revision History

--*/
#include "SetupPlatform.h"
#include <Cpu\CpuRegs.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>

#ifdef SDP_FLAG
#include <Sps.h> 
#endif

#define NEWSTRING_SIZE 0x200
#include <Guid/HiiSetupEnter.h>	 //AptioV Server Override: Use GUID for TSE.

// AptioV Server Override Start: Update ME setup strings
#include "Library/MeTypeLib.h"
#if defined(ICC_SUPPORT) && ICC_SUPPORT
#include "IccCallbacks.h"
#include "IccSetup.h"
extern BOOLEAN                  gIccReset;
#endif // ICC_SUPPORT
// AptioV Server Override Start: Update ME setup strings


//Aptio V Server Override Start: Use SMBIOS string SDL tokens.
#define ___INTERNAL_CONVERT_TO_WSTRING___(a) L###a
#define CONVERT_TO_WSTRING(a) ___INTERNAL_CONVERT_TO_WSTRING___(a)

#define SMBIOS_MAX_NUM_SOCKETS 8
CHAR16 *gSocketDesgination[8] = {
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_0),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_1),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_2),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_3),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_4),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_5),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_6),
    CONVERT_TO_WSTRING(SMBIOS_TYPE_4_SOCKET_DESIGINTATION_SOCKET_7)
};
//Aptio V Server Override End: Use SMBIOS string SDL tokens.

extern EFI_GUID gSignalBeforeEnterSetupGuid;
#ifdef SDP_FLAG
MKHI_MSG_GET_FW_VERSION_RSP      FwVerRspExt;
#endif

STATIC EFI_EXP_BASE10_DATA mCoreFrequencyList[] = {
  { 0, 0},  // 0 Means "Auto", also, the first is the default.
  {-1, 0}   // End marker
};

STATIC EFI_EXP_BASE10_DATA mFsbFrequencyList[] = {
  { 0, 0},  // 0 Means "Auto", also, the first is the default.
  {-1, 0}   // End marker
};

EFI_GUID mProcessorSubClass = EFI_PROCESSOR_SUBCLASS_GUID;
EFI_GUID mCacheSubClass     = EFI_CACHE_SUBCLASS_GUID;
EFI_GUID mMemorySubClass    = EFI_MEMORY_SUBCLASS_GUID;
EFI_GUID mMiscSubClass      = EFI_MISC_SUBCLASS_GUID;

EFI_GUID                      mProcessorProducerGuid;
VOID                          *mRegistration;
UINT64                        mMonotonicCount;
UINT16                        mMemorySpeed = 0xffff;
EFI_PLATFORM_TYPE_PROTOCOL    *mPlatformType;
SYSTEM_CONFIGURATION          mSystemDefaults;
EFI_IIO_UDS_PROTOCOL          *mIioUds;
EFI_MP_SERVICES_PROTOCOL      *mMpService;

CPU_SETUP_INFO                mCpuSetupInfo[MAX_CPU_SOCKET];

VOID
ConfigIio (
  VOID
  );

VOID
DisplayMemTopology (
  VOID
  );

//
// Display Memory Type (DDR) related
//
#define MEMORY_TYPE_DDR_STR L"DDR3"

VOID
UpdateMemType (
  VOID
  );

EFI_STRING_ID MemTypeStrIdTab [] = {
  (STRING_REF) STR_DDR_PHASE_SHEDDING_PROMPT_HELP,
  (STRING_REF) STR_MEMORY_TRAINING_DISPLAY_FORM_HELP,
  (STRING_REF) STR_ENFORCE_POR_HELP,
  (STRING_REF) STR_DDR_VOLTAGE_PROMPT,
};

EFI_STATUS
FindDataRecords (
  VOID
  )
/*++

Routine Description:

  Register event handler to find setup data records in Data Hub.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  The data hub filter event handler was registered.

--*/
{
  EFI_STATUS                  Status;
  EFI_DATA_HUB_PROTOCOL       *DataHub;
  EFI_EVENT                   FilterEvent;
  EFI_EVENT                   DiskInfoEvent;
  VOID                        *DiskInfo;
  VOID                        *FilterDummyRegistration;
  //
  // Get the Data Hub Protocol. Assume only one instance
  // of Data Hub Protocol is availabe in the system.
  //
  Status = gBS->LocateProtocol (&gEfiDataHubProtocolGuid, NULL, &DataHub);
  ASSERT_EFI_ERROR (Status);

  //
  // Register our Setup Data Filter Function.
  // This function is notified at EFI_TPL_CALLBACK.
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SetupDataFilter,
                  NULL,
                  &FilterEvent
                  );
  ASSERT_EFI_ERROR (Status);
  Status = gBS->RegisterProtocolNotify (
                  &gEfiSetupEnterGuid,	//AptioV Server Override - Use GUID for TSE.
                  FilterEvent,
                  &FilterDummyRegistration
                  );
  ASSERT_EFI_ERROR (Status);

  // Aptiov Server Override Start: Commenting as the same is not required
  /*Status = DataHub->RegisterFilterDriver (
                      DataHub,
                      FilterEvent,
                      TPL_APPLICATION,
                      EFI_DATA_RECORD_CLASS_DATA,
                      NULL
                      );*/
 // Aptiov Server Override End: Commenting as the same is not required

  ASSERT_EFI_ERROR (Status);
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    IdeCallback,
                    NULL,
                    &DiskInfoEvent
                    );

    ASSERT_EFI_ERROR (Status);

    Status = gBS->RegisterProtocolNotify (
                    &gEfiDiskInfoProtocolGuid,
                    DiskInfoEvent,
                    &DiskInfo
                    );

  return EFI_SUCCESS;
}

EFI_STRING_ID
UpdateAhciString (
  IN  UINTN     UpdateIndex
  )
{
  switch (UpdateIndex) {
    case 0:
      return (STRING_REF) STR_SATA0_NAME;
    case 1:
      return (STRING_REF) STR_SATA1_NAME;
    case 2:
      return (STRING_REF) STR_SATA2_NAME;
    case 3:
      return (STRING_REF) STR_SATA3_NAME;
    case 4:
      return (STRING_REF) STR_SATA4_NAME;
    case 5:
      return (STRING_REF) STR_SATA5_NAME;
    default:
      return 0;
  }
}

EFI_STRING_ID
UpdateSSataString (
  IN  UINTN     UpdateIndex
  )
{
  switch (UpdateIndex) {
    case 0:
      return (STRING_REF) STR_SSATA0_NAME;
    case 1:
      return (STRING_REF) STR_SSATA1_NAME;
    case 2:
      return (STRING_REF) STR_SSATA2_NAME;
    case 3:
      return (STRING_REF) STR_SSATA3_NAME;
    default:
      return 0;
  }
}

VOID
EFIAPI
IdeCallback(
  IN EFI_EVENT              Event,
  IN VOID                   *Context
  )
/*++

Routine Description:

  IDE data filter function.

Arguments:

  None

Returns:

  None.

--*/
{
  EFI_STATUS                      Status;
  UINTN                           HandleCount;
  EFI_HANDLE                      *HandleBuffer;
  EFI_DISK_INFO_PROTOCOL          *DiskInfo;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePathNode;
  PCI_DEVICE_PATH                 *PciDevicePath;
  UINTN                           Index;
  UINT8                           Index1;
  UINT32                          BufferSize;
  UINT64                          DriveSize;
  UINT32                          IdeChannel;
  UINT32                          IdeDevice;
  EFI_IDENTIFY_DATA               *IdentifyDriveInfo;
  CHAR16                          *NewString;
  CHAR16                          SizeString[20];
  EFI_STRING_ID                   NameToUpdate;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
  UINT8                           PciBuffer;
  UINT8                           SataMode;
  //
  // Assume no line strings is longer than 256 bytes.
  //
  NewString     = AllocateZeroPool (NEWSTRING_SIZE);
  ASSERT (NewString != NULL);
  PciDevicePath = NULL;

  NameToUpdate = (STRING_REF) STR_SATA0_NAME;
  //
  // Initialize IDE Combined mode
  //
  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, &PciRootBridgeIo);

  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint8,
                                  0x1F020A,
                                  1,
                                  &PciBuffer
                                  );
  SataMode = PciBuffer;

  //
  // Fill IDE Information for IDE mode
  //
  Status = gBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiDiskInfoProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
        );
  if (EFI_ERROR(Status)) {
//Grangeville AptioV server override - CPP Check  
    FreePool(NewString);
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    PciDevicePath = NULL;
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID *) &DevicePath
                    );
    ASSERT_EFI_ERROR (Status);

    DevicePathNode = DevicePath;
    while (!IsDevicePathEnd (DevicePathNode)) {
      if ((DevicePathType (DevicePathNode) == HARDWARE_DEVICE_PATH) &&
          (DevicePathSubType (DevicePathNode) == HW_PCI_DP)
          ) {
        PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
        break;
      }

      DevicePathNode = NextDevicePathNode (DevicePathNode);
    }

    if (PciDevicePath == NULL) {
      continue;
    }
    //
    // Check for onboard SATA
    //
    if (((PciDevicePath->Device == 0x1F)&&(PciDevicePath->Function == 2 || PciDevicePath->Function == 5))||
       ((PciDevicePath->Device == 0x11)&&(PciDevicePath->Function == 4))) { 
//AptioV server Override EIP 140147 start
    	  Status = PciRootBridgeIo->Pci.Read (
    	                                  PciRootBridgeIo,
    	                                  EfiPciWidthUint8,
    	                                  (PciDevicePath->Device << 16) | (PciDevicePath->Function << 8) | 0x0A,
    	                                  1,
    	                                  &PciBuffer
    	                                  );
    	  SataMode = PciBuffer;
 //AptioV server Override EIP 140147 End    	
      Status = gBS->HandleProtocol (HandleBuffer[Index], &gEfiDiskInfoProtocolGuid, &DiskInfo);
      ASSERT_EFI_ERROR (Status);

      Status = DiskInfo->WhichIde (
                          DiskInfo,
                          &IdeChannel,
                          &IdeDevice
                          );
      //ASSERT_EFI_ERROR (Status);
	  if(EFI_ERROR(Status)) { 
	    continue;
	  }

      BufferSize = sizeof (EFI_IDENTIFY_DATA);
      IdentifyDriveInfo = AllocatePool (BufferSize);

      Status = DiskInfo->Identify (
                          DiskInfo,
                          IdentifyDriveInfo,
                          &BufferSize
                          );
      //ASSERT_EFI_ERROR (Status);
	  if(EFI_ERROR(Status)) {
	    continue;
	  }

      if (SataMode == PCI_CLASS_MASS_STORAGE_IDE) {   // IDE MODE
        Status = DiskInfo->WhichIde (
                             DiskInfo,
                             &IdeChannel,
                             &IdeDevice
                             );
        IdeDevice = (IdeChannel * 2) + IdeDevice;
        // Port 0 = Primary Master
        // Port 2 = Primary Slave
        // Port 1 = Secondary Master
        // Port 3 = Secondary Slave
        if (IdeDevice == 1 || IdeDevice == 2) {   // SINDX Serial ATA Index (D31:F2)
          IdeDevice ^= 3;
        }
        if (PciDevicePath->Function == 5) {  //Port 4, 5 in SATA2
          IdeDevice += 4;
        }

        if ( PciDevicePath->Device == 0x11 )
          NameToUpdate = UpdateSSataString(IdeDevice);
        else
          NameToUpdate = UpdateAhciString(IdeDevice);

      } else { // SATA or RAID MODE
        Status = DiskInfo->WhichIde (
                             DiskInfo,
                             &IdeChannel,
                             &IdeDevice
                             );
        if ( PciDevicePath->Device == 0x11 )
          NameToUpdate = UpdateSSataString(IdeChannel);
        else
          NameToUpdate = UpdateAhciString(IdeChannel);
      }

      SwapEntries ((CHAR8 *) &IdentifyDriveInfo->AtaData.ModelName, 40);
      AsciiToUnicode ((CHAR8 *) &IdentifyDriveInfo->AtaData.ModelName, NewString);

      //
      // Chop it off after 14 characters
      //
      NewString[14] = 0;

      //
      // For HardDisk append the size. Otherwise display atapi
      //
      if ((IdentifyDriveInfo->AtaData.config & 0x8000) == 00) {
        //
        // 48 bit address feature set is supported, get maximum capacity
        //
        if ((IdentifyDriveInfo->AtaData.command_set_supported_83 & 0x0400) == 0) {
          DriveSize =
            (
              (
                (
                  (
                    (IdentifyDriveInfo->AtaData.user_addressable_sectors_hi << 16) +
                    IdentifyDriveInfo->AtaData.user_addressable_sectors_lo
                  ) / 1000
                ) * 512
              ) /
              1000
            );
        } else {
          DriveSize = IdentifyDriveInfo->AtaData.maximum_lba_for_48bit_addressing[0];
          for (Index1 = 1; Index1 < 4; Index1++) {
            DriveSize |= LShiftU64 (
                          IdentifyDriveInfo->AtaData.maximum_lba_for_48bit_addressing[Index1],
                          16 * Index1
                          );
          }
          DriveSize = (UINT32) DivU64x64Remainder (MultU64x32 (DivU64x64Remainder (DriveSize, 1000, 0), 512), 1000, 0);
        }

        StrCat (NewString, L" - ");
        UnicodeValueToString (SizeString, LEFT_JUSTIFY, DriveSize / 1000, 5);
        StrCat (NewString, SizeString);
        StrCat (NewString, L".");
        UnicodeValueToString (SizeString, LEFT_JUSTIFY, (DriveSize % 1000) / 100, 1);
        StrCat (NewString, SizeString);
        StrCat (NewString, L" GB");
      } else {
        StrCat (NewString, L" - ATAPI");
      }

      HiiSetString(mAdvancedHiiHandle, NameToUpdate, NewString, NULL);	//Update Sata Port # page with info
// AptioV Server Override
//-	  HiiSetString(mMainHiiHandle, NameToUpdate, NewString, NULL);		//Update Main page with info as well
      gBS->FreePool(IdentifyDriveInfo);
    }
  }

  if (HandleBuffer != NULL) {
    gBS->FreePool (HandleBuffer);
  }

  gBS->FreePool (NewString);

  return ;
}

typedef struct {
  UINT8   Type;
  UINT8   Length;
  UINT16  Handle;
} SMBIOS_HEADER;

/**

  Find the SMBIOS structure with the specified Type and Handle.
  If Type is NULL then only Handle will be matched.

  @param    Handle            The Handle to search for.
  @param    Type              The SMBIOS Type to search for.
  @param    Record            The SMBIOS record if a match is found

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetStructure (
  IN      UINT16  Handle,
  IN      EFI_SMBIOS_TYPE         Type,
  OUT     EFI_SMBIOS_TABLE_HEADER **Record
  )
{
  EFI_SMBIOS_HANDLE                 SmbiosHandle;
  EFI_SMBIOS_PROTOCOL               *Smbios;
  SMBIOS_HEADER                     *SmbiosHeader;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  while (1) {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, &Type, Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }
    SmbiosHeader = (SMBIOS_HEADER *) *Record;
	if (SmbiosHeader->Handle == Handle)
      return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**

  Acquire the string associated with the Index from SMBIOS structure and return it.
  The caller is responsible for freeing the string buffer.

  @param    OptionalStrStart  The start position to search the string
  @param    Index             The index of the string to extract
  @param    String            The string that is extracted

  @retval   EFI_SUCCESS       The function returns EFI_SUCCESS if successful.
  @retval   EFI_NOT_FOUND     The function returns EFI_NOT_FOUND if unsuccessful.

**/
EFI_STATUS
SmbiosGetOptionalStringByIndex (
  IN      CHAR8                   *OptionalStrStart,
  IN      UINT8                   Index,
  OUT     CHAR16                  **String
  )
{
  UINTN          StrSize;

  if (Index == 0) {
    return EFI_INVALID_PARAMETER;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart); // size includes null terminator
  // SMBIOS strings are NULL terminated, and end of all strings is indicated with NULL
  // loop until at end of all SMBIOS strings (found NULL terminator at string index past string's NULL), and Index != 0
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero
    return EFI_INVALID_PARAMETER;
  } else {
    AsciiStrToUnicodeStr (OptionalStrStart, *String);
  }

  return EFI_SUCCESS;
}

// AptioV Server Override Start: Added for Vga Priority Porting
VOID
UpdateVgaCpuTarget (
  )
{
	  EFI_STATUS		Status;
	  CHAR16                  *NewString;
	  UINTN			VarSize;
	  UINTN                                 StrSize;

	  StrSize  = 0x20;
	  NewString    = AllocateZeroPool (StrSize);

	  VarSize = sizeof (SYSTEM_CONFIGURATION);
	  Status = gRT->GetVariable(
				L"IntelSetup",
				&mSystemConfigurationGuid,
				NULL,
				&VarSize,
				&mSystemDefaults
				);
	  if (EFI_ERROR(Status)) {
		   mSystemDefaults.LegacyVgaSoc = 0;
	   }

	   UnicodeSPrint(NewString,StrSize, L"Vga From CPU %d",mSystemDefaults.LegacyVgaSoc);
	   HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_TARGET_VGA_NAME), NewString, NULL);

}
// AptioV Server Override End: Added for Vga Priority Porting

VOID
GetCpuInformation (
  VOID
  )
{
  UINT32                       ApicId, SocketId;
  UINT32                       RegEax, RegEbx, RegEcx, RegEdx;
  UINT32                       SocketIdShift; //Aptio V Server Override - SocketIdMask, i;

  // Determine socket ID
  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  ApicId = (RegEbx >> 24);
  SocketIdShift = 5;

  AsmCpuid(EFI_CPUID_SIGNATURE, &RegEax, NULL, NULL, NULL);
  if(RegEax >= 0xB) {
    //is X2 Capable Processor, get ApicId from leaf B
    AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 0, NULL, NULL, NULL, &RegEdx);
    ApicId = RegEdx;

    // How many bits to shift to get Socket ID?
    AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 1, &RegEax, NULL, NULL, NULL);
    SocketIdShift = RegEax & B_EFI_CPUID_CORE_TOPOLOGY_SHIFT_MASK;
  }

  SocketId = ApicId >> SocketIdShift;

//Aptio V Server Override Start
//Excluded original Intel code assumes Core ID 0 (ex. APICID 0) exists, but this isn't always true.
#if 0  
  // SocketIdMask is SocketIdShift number of bits.
  SocketIdMask = 0;
  for(i = 0; i < SocketIdShift; i++) {
    SocketIdMask = (SocketIdMask << 1) | 1;
  }

  //
  // Get microcode revision from primary thread in the socket
  //
  if((ApicId & SocketIdMask) == 0x00) {
    mCpuSetupInfo[SocketId].Ucode = (UINT32) RShiftU64 (AsmReadMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
  }
#endif
  mCpuSetupInfo[SocketId].Ucode = (UINT32) RShiftU64 (AsmReadMsr64 (EFI_MSR_IA32_BIOS_SIGN_ID), 32);
//Aptio V Server Override End
}


VOID
UpdateCpuSetupInfo (
  IN EFI_DATA_HUB_PROTOCOL              *DataHub
  )
{
  EFI_STATUS                            Status;
  UINTN                                 StrSize;
  EFI_STRING_ID                         TokenToUpdate;
  CHAR16                                *NewString;
  CHAR16                                *VersionString[MAX_CPU_SOCKET];
  UINTN                                 SocketIndex;
  UINTN                                 BspPackageNumber;
  UINT32                                BspCpuidSignature;
  UINT8                                 StrIndex;
  UINT32                                i; //Aptio V Server Override: Smbios Type4 & Type7 support

  EFI_SMBIOS_HANDLE                     SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  SMBIOS_TABLE_TYPE4                    *SmbiosType4Record;
  SMBIOS_TABLE_TYPE7                    *SmbiosType7Record;
  EFI_SMBIOS_TYPE                       SmbiosType;
  EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord;
  EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord2;

  UINTN                                 NaLen;
  UINT32                                SocketState;
  UINT32                                RegEax, RegEbx, RegEcx, RegEdx;
  CHAR16                                *NotAvailable;

  UINT32                                CapId1;
  UINT32                                CapId4;
  EFI_CPU_CSR_ACCESS_PROTOCOL           *mCpuCsrAccess;
  // Aptio V Server Override : Changes done related to Make sure enable all 3 SMX,VMX,LockChipset setup options enabled if ProcessorTxtEnable is "enabled"
  UINTN                           VariableSize; 
  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  CapId1 = mCpuCsrAccess->ReadCpuCsr (0, 0, CAPID1_PCU_FUN3_REG);
  CapId4 = mCpuCsrAccess->ReadCpuCsr (0, 0, CAPID4_PCU_FUN3_REG);

  
  Status = gBS->LocateProtocol (
                  &gEfiIioUdsProtocolGuid,
                  NULL,
                  &mIioUds
                  );
  ASSERT_EFI_ERROR (Status);

  StrSize      = NEWSTRING_SIZE;
  NewString    = AllocateZeroPool (StrSize);
  NotAvailable = L"   N/A   ";
  NaLen        = StrLen (NotAvailable) * 2;
  ZeroMem (mCpuSetupInfo, sizeof (CPU_SETUP_INFO) * MAX_CPU_SOCKET);
  for (SocketIndex = 0; SocketIndex < MAX_CPU_SOCKET; SocketIndex++) {
    VersionString[SocketIndex] = AllocateZeroPool (StrSize);
  }

  //
  // Get BSP CPU ID
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = RegEax;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);

//Aptio V Server Override start: Smbios Type4 & Type7 support
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  
  for (i = 0; ; ++i) {
	Status = Smbios->GetNext (Smbios, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }

    SmbiosType4Record = (SMBIOS_TABLE_TYPE4 *) SmbiosRecord;
    if ((SmbiosType4Record->Status & 0x40) == 0) continue; //Not populated.

    StrIndex = SmbiosType4Record->Socket;
    Status = SmbiosGetOptionalStringByIndex ((CHAR8*)((UINT8*)SmbiosType4Record + SmbiosType4Record->Hdr.Length), StrIndex, &NewString);
    ASSERT_EFI_ERROR(Status);
    // if Status [6] is set then socket is populated

    for (SocketIndex = 0; SocketIndex < SMBIOS_MAX_NUM_SOCKETS; ++SocketIndex) {
        if (StrCmp(gSocketDesgination[SocketIndex], NewString) == 0) break;
    }
    ASSERT(SocketIndex < SMBIOS_MAX_NUM_SOCKETS);
    
    if (SocketIndex < MAX_CPU_SOCKET) {
//Aptio V Server Override end: Smbios Type4 & Type7 support
        mCpuSetupInfo[SocketIndex].Id = *(UINT32 *) &SmbiosType4Record->ProcessorId.Signature;
        mCpuSetupInfo[SocketIndex].Freq = SmbiosType4Record->CurrentSpeed;
        mCpuSetupInfo[SocketIndex].MaxRatio = (UINT32) mIioUds->IioUdsPtr->PlatformData.MaxCoreToBusRatio[SocketIndex];
        mCpuSetupInfo[SocketIndex].MinRatio = (UINT32) mIioUds->IioUdsPtr->PlatformData.MinCoreToBusRatio[SocketIndex];

        if (SmbiosGetStructure (SmbiosType4Record->L1CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 0) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            mCpuSetupInfo[SocketIndex].L1Cache = SmbiosType7Record->InstalledSize & 0x7FFF;
            // if granularity is 64 KB, then multiply by 64
			if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L1Cache *= 64;
            }
          }
        }

        if (SmbiosGetStructure (SmbiosType4Record->L2CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 1) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            mCpuSetupInfo[SocketIndex].L2Cache = SmbiosType7Record->InstalledSize & 0x7FFF;
            // if granularity is 64 KB, then multiply by 64
            if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L2Cache *= 64;
            }
          }
        }

        if (SmbiosGetStructure (SmbiosType4Record->L3CacheHandle, EFI_SMBIOS_TYPE_CACHE_INFORMATION, &SmbiosRecord2) == EFI_SUCCESS) {
          SmbiosType7Record = (SMBIOS_TABLE_TYPE7 *) SmbiosRecord2;
          if ((SmbiosType7Record->CacheConfiguration & 0x7) == 2) { // [2:0] is Cache Level; verify it matches
            // [15] indicates granularity; 0=1 KB, 1=64 KB
            // set cache size in bytes by setting the value to 1024*size
            mCpuSetupInfo[SocketIndex].L3Cache = SmbiosType7Record->InstalledSize & 0x7FFF;
            // if granularity is 64 KB, then multiply by 64
            if (SmbiosType7Record->InstalledSize & 0x8000) {
              mCpuSetupInfo[SocketIndex].L3Cache *= 64;
            }
          }
        }

        StrIndex = SmbiosType4Record->ProcessorVersion;
        if (SmbiosGetOptionalStringByIndex ((CHAR8*)((UINT8*)SmbiosType4Record + SmbiosType4Record->Hdr.Length), StrIndex, &VersionString[SocketIndex]) == EFI_SUCCESS) {
          mCpuSetupInfo[SocketIndex].Version = VersionString[SocketIndex];
        }
      }
    }


 Status = gBS->LocateProtocol (
      &gEfiMpServiceProtocolGuid,
      NULL,
      &mMpService
      );
  if(!EFI_ERROR(Status)) {
    //
    // Get Microcode version from each socket
    //
    GetCpuInformation();
    mMpService->StartupAllAPs (
           mMpService,
           (EFI_AP_PROCEDURE)GetCpuInformation,
           TRUE,
           NULL,
           0,
           NULL,
           NULL
           );
  } else {
    //
    // If MP Protocol not found, fill in BSP info anyway.
    //
    GetCpuInformation();
  }


  BspPackageNumber  = 0;

  if (mCpuSetupInfo[0].Version) {
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET0_VALUE2;
    StrCpy (NewString, mCpuSetupInfo[0].Version);
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString + 23, NULL);
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET0_VALUE;
    NewString[23] = L'\0';
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
  }

  if (mCpuSetupInfo[1].Version) {
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET1_VALUE2;
    StrCpy (NewString, mCpuSetupInfo[1].Version);
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString + 23, NULL);
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET1_VALUE;
    NewString[23] = L'\0';
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
  }

  if (mCpuSetupInfo[2].Version) {
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET2_VALUE2;
    StrCpy (NewString, mCpuSetupInfo[2].Version);
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString + 23, NULL);
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET2_VALUE;
    NewString[23] = L'\0';
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
  }

  if (mCpuSetupInfo[3].Version) {
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET3_VALUE2;
    StrCpy (NewString, mCpuSetupInfo[3].Version);
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString + 23, NULL);
    TokenToUpdate = STR_PROCESSOR_VERSION_SOCKET3_VALUE;
    NewString[23] = L'\0';
    HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
  }

  SocketState = 0;
  for (SocketIndex = 0; SocketIndex < MAX_CPU_SOCKET; SocketIndex++) {
    SocketState += (mCpuSetupInfo[SocketIndex].Id ? 0 : 1 << SocketIndex);
  }

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_ID_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"%08x%c",
    (UINTN)mCpuSetupInfo[0].Id,
    BspPackageNumber ? ' ' : '*'
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_ID_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_FREQ_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"%1d.%03dGHz",
    (UINTN) (mCpuSetupInfo[0].Freq / 1000),
    (UINTN) (mCpuSetupInfo[0].Freq % 1000)
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_FREQ_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_MAX_RATIO_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"%02xH",
    (UINTN) (mCpuSetupInfo[0].MaxRatio)
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_MAX_RATIO_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_MIN_RATIO_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"%02xH",
    (UINTN) (mCpuSetupInfo[0].MinRatio)
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_MIN_RATIO_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_MICROCODE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"%08x",
    (UINTN)mCpuSetupInfo[0].Ucode
    );
  if (mCpuSetupInfo[0].Ucode == 0) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_MICROCODE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_L1_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"  %4dKB",
    (UINTN)mCpuSetupInfo[0].L1Cache
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_L1_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_L2_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L"  %4dKB",
    (UINTN)mCpuSetupInfo[0].L2Cache
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_L2_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR_L3_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L" %5dKB",
    (UINTN)mCpuSetupInfo[0].L3Cache
    );
  if (SocketState & 1) {
    CopyMem (&NewString[0], NotAvailable, NaLen);
  }
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_PROCESSOR2_3_L3_CACHE_VALUE;
  UnicodeSPrint (
    NewString,
    StrSize,
    L""
    );
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);
// AptioV Server Override Start
/* Commenting the unwanted Intel Setup Page

  //
  // Display Processor Revision in the Main page
  //
  TokenToUpdate = STR_CPU_BSP_REVISION_VALUE;
  switch (BspCpuidSignature) {
    case 0x206D0:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D1:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown A1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D2:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D3:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown B1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D5:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown C0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D6:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown C1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x206D7:
      UnicodeSPrint (NewString, StrSize, L"%5X - Jaketown C2", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x306E0:
      UnicodeSPrint (NewString, StrSize, L"%5X - Ivytown A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x306E1:
      UnicodeSPrint (NewString, StrSize, L"%5X - Ivytown A1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x306F0:
      UnicodeSPrint (NewString, StrSize, L"%5X - Haswell A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x306F1:
      UnicodeSPrint (NewString, StrSize, L"%5X - Haswell B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      break;
    case 0x306F2:
      if ((CapId4 & (BIT29 | BIT7 | BIT6)) == (BIT29 | BIT7)) {
        UnicodeSPrint (NewString, StrSize, L"%5X - Haswell C1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      } 
	  else if ((CapId4 & (BIT29 | BIT7 | BIT6)) == (BIT29 | BIT6)) {
        UnicodeSPrint (NewString, StrSize, L"%5X - Haswell M1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);	   
	  }
      else {	  
        UnicodeSPrint (NewString, StrSize, L"%5X - Haswell C0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      }

      break;
    case 0x406F0:
      if (CapId1 & BIT20) {
        UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell A1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      } 
      else {
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      }
    break;
    case 0x406F1:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell B0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
    break;
    case 0x50660:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE T0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
    break;
    case 0x50661:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE U0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
    break;
    case 0x50662:
      if (CapId1 & BIT20) {
        UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE V1", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      }
      else {
        UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE V0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
      }
    break;	
    case 0x50663:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE V2", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);   
      break;	
    case 0x50664:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE Y0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);   
      break;
    case 0x50665:
      UnicodeSPrint (NewString, StrSize, L"%5X - Broadwell DE NS A0", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);   
      break;  
    default:
      UnicodeSPrint (NewString, StrSize, L"%5X - Unknown", (UINTN)mCpuSetupInfo[BspPackageNumber].Id);
  }
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display Microcode Revision in the Main page
  //
  TokenToUpdate = STR_PROCESSOR_MICROCODE_VALUE;
  UnicodeSPrint (NewString, StrSize, L"%08X", mCpuSetupInfo[0].Ucode);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
*/
/* Commenting the unwanted Intel Setup Page*/
// AptioV Server Override End

  gBS->FreePool (NewString);
  for (SocketIndex = 0; SocketIndex < MAX_CPU_SOCKET; SocketIndex++) {
    gBS->FreePool (VersionString[SocketIndex]);
  }
  
  // AptioV Server Override Start : Updating the SMX and VMX setup option based on ProcessorLtsxEnable
  
  VariableSize = sizeof(SYSTEM_CONFIGURATION);
    // AptioV server override: Changed variable name to Intelsetup
    Status = gRT->GetVariable (
                    L"IntelSetup",
                    &mSystemConfigurationGuid,
                    NULL,
                    &VariableSize,
                    &mSystemDefaults
                    );
    //
    // ASSERT_EFI_ERROR(Status);
    //
    if (EFI_ERROR (Status)) {
      DEBUG((EFI_D_ERROR, "Failed to get Variable in UpdateCpuSetupInfo!\n"));
      return ;

    }
  
  if (mSystemDefaults.ProcessorLtsxEnable) {
  DEBUG((EFI_D_ERROR, "mSystemDefaults.ProcessorLtsxEnable = %d\n",mSystemConfiguration.ProcessorLtsxEnable));
  	  mSystemDefaults.ProcessorSmxEnable = 1;
  	  mSystemDefaults.ProcessorVmxEnable = 1;
  	  mSystemDefaults.LockChipset = 1;
    }
  
  // Restoring the default values of VMX,SMX,LockChipset when ProcessorLtsxEnable is disabled
/*   AptioV Server Override Start: Allow user to change VMX and SMX in setup
  if (mSystemDefaults.ProcessorLtsxEnable == 0) {
    DEBUG((EFI_D_ERROR, "mSystemDefaults.ProcessorLtsxEnable = %d\n",mSystemConfiguration.ProcessorLtsxEnable));
    	  mSystemDefaults.ProcessorSmxEnable = 0;
    	  mSystemDefaults.ProcessorVmxEnable = 1;
    	  mSystemDefaults.LockChipset = 1;
      } 
*/ // AptioV Server Override End
  // AptioV server override: Changed variable name to Intelsetup
    Status = gRT->SetVariable(
  		            L"IntelSetup",
  		            &mSystemConfigurationGuid,
  		            EFI_VARIABLE_BOOTSERVICE_ACCESS |EFI_VARIABLE_NON_VOLATILE,
  		            sizeof (SYSTEM_CONFIGURATION),
  		            &mSystemDefaults
  	                    );
    
    if (EFI_ERROR (Status)) {
          DEBUG((EFI_D_ERROR, "Failed to set Variable in UpdateCpuSetupInfo!\n"));
          return ;

        }
  
  // AptioV Server Override End : Updating the SMX and VMX setup option based on ProcessorLtsxEnable
  return;
}

VOID
EFIAPI
SetupDataFilter (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
/*++

Routine Description:

  Setup data filter function. This function is invoked when there is data records
  available in the Data Hub.

Arguments:

  Event    -  the event that is signaled.
  Context  -  not used here.

Returns:

  None.

--*/
{

  EFI_STATUS                            Status;
  EFI_DATA_HUB_PROTOCOL                 *DataHub;

  // AptioV Server Override Start
/*Commenting the unwanted Intel Setup Page
  EFI_DATA_RECORD_HEADER                *Record;
  UINT8                                 *SrcData;
  UINT32                                SrcDataSize;
  UINT32                                MemSize = 0;
  EFI_SUBCLASS_TYPE1_HEADER             *DataHeader;
  CHAR16                                *NewString;
  CHAR16                                *NewString2;
  EFI_STRING_ID                         TokenToUpdate;
//  UINTN                                 Index;
//  EFI_PROCESSOR_VERSION_DATA            *ProcessorVersion;
//  EFI_PROCESSOR_ID_DATA                 *ProcessorId;
//  UINT32                                ProcessorSignature;
  VOID                                  *HobList;
  struct SystemMemoryMapHob             *mSystemMemoryMap;
  UINTN                                 DataSize;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_TYPE_PROTOCOL            *PlatformType;
  EFI_GUID                              *SystemUuid;
*/
  
//  EFI_PROCESSOR_MICROCODE_REVISION_DATA *CpuUcodeRevisionData;
  //EFI_SMBIOS_TABLE_HEADER               *SmbiosRecord;
  //EFI_SMBIOS_HANDLE                     SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                   *Smbios;
  //SMBIOS_TABLE_TYPE0                    *SmbiosType0Record;
  //UINT8                                 StrIndex;
  //EFI_SMBIOS_TYPE                       SmbiosType;

  // AptioV Server Override start
/*  DataSize    = 0;
  NewString   = AllocateZeroPool (NEWSTRING_SIZE);
  NewString2  = AllocateZeroPool (NEWSTRING_SIZE);
*/
// AptioV Server Override End
  Status = gBS->LocateProtocol (
                  &gEfiIioUdsProtocolGuid,
                  NULL,
                  &mIioUds
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);


  //
  // Get the Data Hub Protocol. Assume only one instance
  //
  Status = gBS->LocateProtocol (&gEfiDataHubProtocolGuid, NULL, &DataHub);
  ASSERT_EFI_ERROR (Status);

  UpdateCpuSetupInfo (DataHub);

// AptioV Server Override: Added for Vga Priority Porting
  UpdateVgaCpuTarget ();
  
  // AptioV Server Override Start: Update ME setup strings
  InitMeSetupStrings();
#if defined(ICC_SUPPORT) && ICC_SUPPORT
  if (MeTypeIsAmt())
  {
    InitIccHandles (mAdvancedHiiHandle);
  }
#endif // ICC_SUPPORT
  // AptioV Server Override End: Update ME setup strings  

// AptioV Server Override Start
/*
  //
  // Get all available data records from data hub
  //
  Record = NULL;

  do {
    Status = DataHub->GetNextRecord (DataHub, &mMonotonicCount, NULL, &Record);
    if (!EFI_ERROR (Status)) {
      if (Record->DataRecordClass == EFI_DATA_RECORD_CLASS_DATA) {
        DataHeader  = (EFI_SUBCLASS_TYPE1_HEADER *) (Record + 1);
        SrcData     = (UINT8 *) (DataHeader + 1);
        SrcDataSize = Record->RecordSize - Record->HeaderSize - sizeof (EFI_SUBCLASS_TYPE1_HEADER);
        if (CompareGuid (&Record->DataRecordGuid, &mMemorySubClass)) {
          //
          // Memory
          //
          switch (DataHeader->RecordType) {
          case EFI_MEMORY_DEVICE_START_ADDRESS_RECORD_NUMBER:
            break;

          case EFI_MEMORY_ARRAY_LINK_RECORD_NUMBER:
            MemoryArrayLink = (EFI_MEMORY_ARRAY_LINK *) SrcData;

            if (MemoryArrayLink->MemorySpeed > 0) {
              //
              // Save the lowest speed memory module
              //
              if (MemoryArrayLink->MemorySpeed < mMemorySpeed) {
                mMemorySpeed = MemoryArrayLink->MemorySpeed;
              }
            }
            break;

          case EFI_MEMORY_ARRAY_START_ADDRESS_RECORD_NUMBER:
            break;

          default:
            break;
          }
        }
      }
    }
  } while (!EFI_ERROR (Status) && (mMonotonicCount != 0));

  //
  // Display the BIOS Version
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_BIOS_INFORMATION;
  Status = Smbios->GetNext (Smbios, &SmbiosHandle, &SmbiosType, &SmbiosRecord, NULL);
  if (!(EFI_ERROR(Status))) {
    SmbiosType0Record = (SMBIOS_TABLE_TYPE0 *) SmbiosRecord;
    StrIndex = SmbiosType0Record->BiosVersion;
    Status = SmbiosGetOptionalStringByIndex
             ((CHAR8*)((UINT8*)SmbiosType0Record + SmbiosType0Record->Hdr.Length), StrIndex, &NewString);

    if (Status == EFI_SUCCESS) {
      TokenToUpdate = (STRING_REF) STR_BIOS_REVISION_VALUE;
      HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
    }
  }

  //
  // Display the RC Versions in the Setup
  //
  UnicodeSPrint (NewString,NEWSTRING_SIZE, L"%x.%x.%x", (mIioUds->IioUdsPtr->SystemStatus.RcVersion & 0xFF000000) >> 24,
                                       (mIioUds->IioUdsPtr->SystemStatus.RcVersion  & 0xFF0000) >> 16,
                                       (mIioUds->IioUdsPtr->SystemStatus.RcVersion  & 0xFF00) >> 8);
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_RC_REVISION_VALUE);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display the Firmware Version in Setup
  //
  UnicodeSPrint (
    NewString,
    NEWSTRING_SIZE,
    L"0x%x",
    gST->FirmwareRevision
    );
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_FIRMWARE_VERSION_VALUE);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);


  //
  // Display the System UUID in Setup
  //

  Status = gBS->LocateProtocol (
                  &gEfiPlatformTypeProtocolGuid,
                  NULL,
                  &PlatformType
                  );
	SystemUuid = (EFI_GUID *)PlatformType->SystemUuid;
  if (!EFI_ERROR (Status)) {
		TokenToUpdate = (STRING_REF) STR_SYSTEM_UUID_VALUE;
		UnicodeSPrint (
      NewString,
      NEWSTRING_SIZE,
      L"%08X-%04X-%04X",
      SystemUuid->Data1,
      SystemUuid->Data2,
      SystemUuid->Data3
      );
		HiiSetString  (mMainHiiHandle, TokenToUpdate, NewString, NULL);
	}
*/
// AptioV Server Override End



/*
  //
  // Display the CSI, MRC and CSM Versions in the Setup
  //
  AsciiToUnicode (CSI_VERSION, NewString);
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_CSI_REVISION_VALUE);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  AsciiToUnicode (MRC_VERSION, NewString);
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_MRC_REVISION_VALUE);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  AsciiToUnicode (CSM_MAJOR_VERSION, NewString);
  AsciiToUnicode (CSM_MINOR_VERSION, NewString2);
  StrCat (NewString, L".");
  StrCat (NewString, NewString2);
  TokenToUpdate = (STRING_REF) STRING_TOKEN (STR_CSM_REVISION_VALUE);
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
*/
  //
  // Update setup variable for memory display
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  //Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList); // AptioV Server Override
  //ASSERT_EFI_ERROR (Status);  // AptioV Server Override

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  /*AptioV Server Override: Commenting the unwanted Intel Setup Page
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if ( GuidHob != NULL)
    mSystemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);
  else {
    mSystemMemoryMap = NULL;
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Display Memory Size
  //

  EfiValueToString (
    NumString,
    CpuIndex,
    LEFT_JUSTIFY,
    4
    );

  UnicodeValueToString (
    NumString,
    LEFT_JUSTIFY,
    CpuIndex,
    4
    );

  MemSize = (UINT32)mSystemMemoryMap->memSize >> 4;
  UnicodeValueToString (
    NewString,
    LEFT_JUSTIFY,
    MemSize,
    6
    );
  StrCat (NewString, L" GB");
  TokenToUpdate = (STRING_REF) STR_TOTAL_MEMORY_SIZE_VALUE;
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display Memory Freq
  //
  UnicodeValueToString (
                    NewString,
                    LEFT_JUSTIFY,
                    mSystemMemoryMap->memFreq * 2,
                    6
                    );
  StrCat (NewString, L" MT/s");
  TokenToUpdate = (STRING_REF) STR_SYSTEM_MEMORY_SPEED_VALUE;
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Display Mem Mode supported
  //
  if (mSystemMemoryMap->memMode == 0) {
    StrCpy (NewString, L"Independent");
  } else if (mSystemMemoryMap->memMode == 1) {
    StrCpy (NewString, L"Lock Step");
  } else {
    StrCpy (NewString, L"Error");
  }

  TokenToUpdate = (STRING_REF) STR_MEMORY_MODE_VALUE;
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Update the overall memory speed
  //
  if (mMemorySpeed != 0xffff) {
    UnicodeValueToString (NewString, LEFT_JUSTIFY, mMemorySpeed * 2, 5);
    StrCat (NewString, L" MT/s");

    TokenToUpdate = (STRING_REF) STR_SYSTEM_MEMORY_SPEED_VALUE;
    HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
  }

*/
  //
  // IOH Configuration
  //
  ConfigIio ();
/*
  //
  // ReserveMem setup
  //
  ReserveMemPage();

  //
  // Diplay Platform Info
  //
  Status = DisplayPlatformInfo (mPlatformType);

  //
  // PCI-E Port Configuration
  //
  //ConfigPciePort();
*/
  // AptioV Server Override end : Under IntelRcSetup page->Memory configuration->memory topology DIMM information is not displayed.  
  //
  // Update Memory Type (DDR)
  // 
  UpdateMemType ();

  /*
  // AptioV Server Override Start : Under IntelRcSetup page->Memory configuration->memory topology DIMM information is not displayed.
  gBS->FreePool (NewString);
  gBS->FreePool (NewString2);
  */ 

  // AptioV Server Override End : Under IntelRcSetup page->Memory configuration->memory topology DIMM information is not displayed.

// AptioV Server Override End

  return ;
}

EFI_STATUS
SetIdeMode (
  VOID
  )
/*++

Routine Description:

  Set Ide's mode.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Must return success on return.

--*/
{
  return EFI_SUCCESS;
}

EFI_STATUS
ProcessorHtMode (
  VOID
  )
/*++

Routine Description:

  Setup process in HT mode.

Arguments:

  None.

Returns:

  EFI_SUCCESS  -  Must complete successfully.

--*/
{
  UINT16              State;
  EFI_STATUS          Status;
  VOID                *HobList;
  UINT32              RegEbx, RegEdx;
  EFI_HOB_GUID_TYPE   *GuidHob;
  State = 0;

  //
  // Get Hob list
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  GuidHob = GetFirstGuidHob (&gEfiCpuHtCapableGuid);
  if (GuidHob != NULL)
     State = *(UINT16*) GET_GUID_HOB_DATA (GuidHob);

  //
  // Check if processor is HT capable
  //
  AsmCpuid (EFI_CPUID_VERSION_INFO, NULL, &RegEbx, NULL, &RegEdx);
  State |= (UINT16) (!((RegEdx & 0x10000000) && (((RegEbx >> 16) & 0xFF) <= 1)));

  gRT->SetVariable (
        VAR_EQ_HT_MODE_DECIMAL_NAME,
        &mSystemConfigurationGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS,
        2,
        (VOID *) &State
        );

  return EFI_SUCCESS;
}


EFI_STATUS
DisplayPlatformInfo (
  EFI_PLATFORM_TYPE_PROTOCOL            *mPlatformType
  )
/*++

Routine Description:

  Displays the Platform, NorthBridge and SouthBridge info in the Setup.

Arguments:

  mPlatformType.

Returns:

  EFI_SUCCESS  -  Must complete successfully.

--*/

{
// AptioV Server Override Start
	/* Commenting the unwanted Intel Setup Page
  EFI_STRING_ID  TokenToUpdate;
  CHAR16      *NewString;

  //
  // Platform Type Info
  //
  NewString     = (CHAR16 *)mPlatformType->TypeStringPtr;
  TokenToUpdate = (STRING_REF) STR_PLATFORM_REVISION_VALUE;
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // South Bridge Info
  //
  NewString     = (CHAR16 *)mPlatformType->PchStringPtr;
  TokenToUpdate = (STRING_REF) STR_PCH_REVISION_VALUE;
  HiiSetString(mMainHiiHandle, TokenToUpdate, NewString, NULL);
*/
// AptioV Server Override End
  return EFI_SUCCESS;
}

VOID
ConfigPciePort (
  )
/*++

Routine Description:

  This function configures the PCI Express Port Settings.

Arguments:

  none

Returns:

  none

--*/
{
  UINT8                                 PcieLinkStatus;
  UINT8                                 PcieLinkMax;
  UINT8                                 PcieLinkSpeed;
  UINTN                                 Index;
  CHAR16                                *NewString;
  CHAR16                                *NewString1;
  CHAR16                                *NewMaxString;
  CHAR16                                *NewMaxString1;
  CHAR16                                *NewSpeedString;
  CHAR16                                *NewSpeedString1;
  EFI_STRING_ID                         TokenToUpdate;
  EFI_STRING_ID                         TokenToUpdateMax;
  EFI_STRING_ID                         TokenToUpdateSpeed;
  EFI_IIO_SYSTEM_PROTOCOL               *mIioSystem;
  EFI_STATUS                            Status = EFI_SUCCESS;

  PcieLinkStatus = 0;
  TokenToUpdate = (STRING_REF) STR_LINK_STATUS;
  NewString   = AllocateZeroPool (0x50);
  NewString1  = AllocateZeroPool (0x50);
  PcieLinkMax = 0;
  TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS;
  NewMaxString   = AllocateZeroPool (0x50);
  NewMaxString1  = AllocateZeroPool (0x50);
  TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS;
  NewSpeedString   = AllocateZeroPool (0x50);
  NewSpeedString1  = AllocateZeroPool (0x50);
  PcieLinkSpeed = 0;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &mIioSystem);
  //
  // Get the PCI-E Link Status
  //
  // Aptio V Override Start : PCIe root port on the 3rd CPU show "Link Training Error" in Setup if 2nd CPU doesn't installed 
  //  for (Index = 0; Index < (mIioUds->IioUdsPtr->PlatformData.numofIIO*VARIABLE_GROUP_ELEMENTS); Index++) {
  // Aptio V Override End : PCIe root port on the 3rd CPU show "Link Training Error" in Setup if 2nd CPU doesn't installed 
  for (Index = 0; Index < MaxIIO*VARIABLE_GROUP_ELEMENTS ; Index++) {    
      if (Status == EFI_SUCCESS) {
        PcieLinkStatus = mIioSystem->IioGlobalData->IioOutData.LinkedPXPMap[Index];
        PcieLinkMax = mIioSystem->IioGlobalData->IioOutData.MaxPXPMap[Index];
        PcieLinkSpeed = mIioSystem->IioGlobalData->IioOutData.SpeedPXPMap[Index];
      }
      //DEBUG((EFI_D_ERROR, "the Index and PcieLinkStatus are %x, %x\n", Index,PcieLinkStatus));
      if (PcieLinkStatus != 0) {
        UnicodeValueToString (
                          NewString,
                          LEFT_JUSTIFY,
                          PcieLinkStatus,
                          2
                          );
        StrCpy (NewString1, L"Linked as x");
        StrCat (NewString1, NewString);
        } else {
        StrCpy (NewString1, L"Link Did Not Train");
      }
      if (PcieLinkMax != 0) {
        UnicodeValueToString (
                          NewMaxString,
                          LEFT_JUSTIFY,
                          PcieLinkMax,
                          2
                          );
        StrCpy (NewMaxString1, L"Max Width x");
        StrCat (NewMaxString1, NewMaxString);
        } else {
        StrCpy (NewMaxString1, L"ERROR: Not Available");
      }

      if (PcieLinkStatus != 0) {
        StrCpy(NewSpeedString1, L"Gen ");
        UnicodeValueToString (
                          NewSpeedString,
                          LEFT_JUSTIFY,
                          PcieLinkSpeed,
                          2
                          );
        StrCat(NewSpeedString1, NewSpeedString);

        switch(PcieLinkSpeed) {
        // Values for link speed come from PCI-E spec. Add new cases for future generations
        case 1:
          StrCat (NewSpeedString1, L" (2.5 GT/s)");
          break;
        case 2:
          StrCat (NewSpeedString1, L" (5.0 GT/s)");
          break;
        case 3:
          StrCat (NewSpeedString1, L" (8.0 GT/s)");
          break;
        default:
          StrCat (NewSpeedString1, L" (Undefined)");
          break;
        }
      } else {
        StrCpy (NewSpeedString1, L"Link Did Not Train");
      }

      if (Index == 0) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT0;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT0;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT0;
      } else if (Index == 1) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT1;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT1;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT1;
      } else if (Index == 2) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT2;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT2;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT2;
      } else if (Index == 3) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT3;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT3;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT3;
      } else if (Index == 4) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT4;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT4;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT4;
      } else if (Index == 5) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT5;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT5;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT5;
      } else if (Index == 6) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT6;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT6;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT6;
      } else if (Index == 7) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT7;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT7;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT7;
      } else if (Index == 8) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT8;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT8;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT8;
      } else if (Index == 9) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT9;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT9;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT9;
      } else if (Index == 10) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO0PORT10;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO0PORT10;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO0PORT10;
      } else if (Index == 11) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT0;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT0;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT0;
      } else if (Index == 12) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT1;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT1;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT1;
      } else if (Index == 13) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT2;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT2;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT2;
      } else if (Index == 14) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT3;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT3;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT3;
      } else if (Index == 15) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT4;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT4;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT4;
      } else if (Index == 16) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT5;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT5;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT5;
      } else if (Index == 17) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT6;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT6;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT6;
      } else if (Index == 18) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT7;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT7;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT7;
      } else if (Index == 19) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT8;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT8;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT8;
      } else if (Index == 20) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT9;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT9;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT9;
      } else if (Index == 21) {
        TokenToUpdate    = (STRING_REF) STR_LINK_STATUS_IIO1PORT10;
        TokenToUpdateMax = (STRING_REF) STR_MAXLINK_STATUS_IIO1PORT10;
        TokenToUpdateSpeed = (STRING_REF) STR_PCIESPEED_STATUS_IIO1PORT10;
      }
      HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString1, NULL);
      HiiSetString(mAdvancedHiiHandle, TokenToUpdateMax, NewMaxString1, NULL);
      HiiSetString(mAdvancedHiiHandle, TokenToUpdateSpeed, NewSpeedString1, NULL);
    }

  gBS->FreePool (NewString);
  gBS->FreePool (NewString1);
  gBS->FreePool (NewMaxString);
  gBS->FreePool (NewMaxString1);
  gBS->FreePool (NewSpeedString);
  gBS->FreePool (NewSpeedString1);

  return ;
}


VOID
UpdateCSIStatusLines (
  VOID
  )
/*++

Routine Description:

  This function updates CSI status lines.

Arguments:

  None

Returns:

  None

--*/
{
  CHAR16                *NewString;
  STRING_REF            TokenToUpdate;
  UINTN                 StrSize;

  StrSize   = 0x250;
  NewString = AllocateZeroPool (StrSize);

  //
  // Number of CPU
  //
  TokenToUpdate = (STRING_REF) STR_CSI_NUMBER_OF_CPU_VAL;
  UnicodeSPrint (NewString,StrSize,L"%d",(UINTN)mIioUds->IioUdsPtr->SystemStatus.numCpus);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Number of IOH
  //
  TokenToUpdate = (STRING_REF) STR_CSI_NUMBER_OF_IIO_VAL;
  UnicodeSPrint (NewString,StrSize,L"%d",(UINTN)mIioUds->IioUdsPtr->PlatformData.numofIIO);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Current CSI Link Frequency
  //
  switch (mIioUds->IioUdsPtr->SystemStatus.CurrentCsiLinkFrequency) {
  case UDS_FREQ_5_600_GT:
    StrCpy (NewString, L"5.6");
	break;

  case UDS_FREQ_6_400_GT:
    StrCpy (NewString, L"6.4");
    break;

  case UDS_FREQ_7_200_GT:
    StrCpy (NewString, L"7.2");
    break;

  case UDS_FREQ_8_000_GT:
    StrCpy (NewString, L"8.0");
    break;

  case UDS_FREQ_8_800_GT:
	StrCpy (NewString, L"8.8");
	break;

  case UDS_FREQ_9_600_GT:
    StrCpy (NewString, L"9.6");
	break;

  default:
    StrCpy (NewString, L"Unknown");
    break;
  }

  StrCat (NewString, L" GT/s");

  //
  // If Link Speed is set to slow and multiple CPUs are present, then by definition the link frequency should be 50 MT/s
  //
  if((!mIioUds->IioUdsPtr->SystemStatus.CurrentCsiLinkSpeed) && (mIioUds->IioUdsPtr->SystemStatus.numCpus > 1)){
	  StrCpy (NewString, L"50 MT/s");
  }

  TokenToUpdate = (STRING_REF) STR_CSI_LINK_FREQ_STATUS;
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // Current CSI Link Speed
  //
  if (mIioUds->IioUdsPtr->SystemStatus.CurrentCsiLinkSpeed)
      StrCpy (NewString, L"Fast");
  else
      StrCpy (NewString, L"Slow");

  TokenToUpdate = (STRING_REF) STR_CSI_LINK_SPEED_STATUS;
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // CSI Global MMIO Low Base / Limit
  //
  TokenToUpdate = (STRING_REF) STR_CSI_GMMIOL_BASE_LIMIT_VAL;
  UnicodeSPrint (NewString,StrSize,L"%08x / %08x", (UINTN)mIioUds->IioUdsPtr->PlatformData.PfGMmiolBase, (UINTN)mIioUds->IioUdsPtr->PlatformData.PfGMmiolLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // CSI Global MMIO High Base / Limit
  //
  TokenToUpdate = (STRING_REF) STR_CSI_GMMIOH_BASE_LIMIT_VAL;
  UnicodeSPrint (NewString,StrSize,L"%011x / %011x", (UINTN)mIioUds->IioUdsPtr->PlatformData.PfGMmiohBase, (UINTN)mIioUds->IioUdsPtr->PlatformData.PfGMmiohLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  //
  // CSI Pci-e Configuration Base
  //
  TokenToUpdate = (STRING_REF) STR_CSI_PCI_E_CFG_BASE_VAL;
  UnicodeSPrint (NewString,StrSize,L"%08x / %08x", (UINTN)mIioUds->IioUdsPtr->PlatformData.PciExpressBase, (UINTN)mIioUds->IioUdsPtr->PlatformData.PciExpressSize);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);


  gBS->FreePool (NewString);

  return ;
}


VOID
UpdateCSIResourceAllocationLines (
  VOID
  )
/*++

Routine Description:

  This function updates CSI status lines.

Arguments:

  None

Returns:

  None

--*/
{
/* for Brickland

  CHAR16                  *NewString;
  STRING_REF              TokenToUpdate;
  UINTN                   StrSize;
  IIO_RESOURCE_INSTANCE   *IioResource;

  StrSize   = 0x250;
  NewString = AllocateZeroPool (StrSize);
  IioResource = mIioUds->IioUdsPtr->PlatformData.IIO_resource;

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET0_BUS_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[0].BusBase, (UINTN)IioResource[0].BusLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET0_IO_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[0].PciResourceIoBase, (UINTN)IioResource[0].PciResourceIoLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET0_MMIOL_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[0].PciResourceMem32Base, (UINTN)IioResource[0].PciResourceMem32Limit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET1_BUS_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[1].BusBase, (UINTN)IioResource[1].BusLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET1_IO_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[1].PciResourceIoBase, (UINTN)IioResource[1].PciResourceIoLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET1_MMIOL_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[1].PciResourceMem32Base, (UINTN)IioResource[1].PciResourceMem32Limit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET2_BUS_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[2].BusBase, (UINTN)IioResource[2].BusLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET2_IO_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[2].PciResourceIoBase, (UINTN)IioResource[2].PciResourceIoLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET2_MMIOL_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[2].PciResourceMem32Base, (UINTN)IioResource[2].PciResourceMem32Limit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET3_BUS_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[3].BusBase, (UINTN)IioResource[3].BusLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET3_IO_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[3].PciResourceIoBase, (UINTN)IioResource[3].PciResourceIoLimit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  TokenToUpdate = (STRING_REF) STR_CSI_SOCKET3_MMIOL_RANGE_VAL;
  UnicodeSPrint (NewString,StrSize,L"0x%x - 0x%x",(UINTN)IioResource[3].PciResourceMem32Base, (UINTN)IioResource[3].PciResourceMem32Limit);
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);

  for Platform */

  return ;
}

VOID
UpdateDramTypeStrings (
  BOOLEAN   DDR3DimmsPresent
  )
{

  CHAR16      *DimmString1 = NULL;

  if ( !DDR3DimmsPresent )
  {

    DimmString1 = AllocateZeroPool (0x500);
  
    if ( !DimmString1 )
      return;

    UnicodeSPrint ( DimmString1, 0x500, L"Enable to enforce POR restrictions for DDR4 frequency and voltage programming" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_ENFORCE_POR_HELP), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"DDR4 LA Probes" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_MEMORY_LAI_PROBES_PROMPT), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"Present indicates the presence of DDR4 logic analyzer probes" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_MEMORY_LAI_PROBES_HELP), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"DDR4 Voltage Level" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_DDR_VOLTAGE_PROMPT), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"DDR4 training options" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_MEMORY_TRAINING_DISPLAY_FORM_HELP), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"DDR4 VR Static Phase Shedding Support.  PS0: full-phase, PS1: single-phase, typically <18A load, PS2: fixed loss, typically <5A load" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_DDR_PHASE_SHEDDING_PROMPT_HELP), DimmString1, NULL);

    UnicodeSPrint ( DimmString1, 0x500, L"Enables the rank margin tool to run after DDR4 memory training" );
    HiiSetString(mAdvancedHiiHandle, STRING_TOKEN(STR_RMT_HELP), DimmString1, NULL);

    FreePool ( DimmString1 );
  }
}


VOID
ConfigIio (
  VOID
  )
/*++

Routine Description:

  This function configures the IIO settings.

Arguments:

  none

Returns:

  none

--*/
{

  EFI_STATUS  Status;
  UINTN       VarSize;
  UINT32	  Attributes;
  UINT32      IioBitMap;
  UINT8       node;
  struct SystemMemoryMapHob             *mSystemMemoryMap;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if ( GuidHob != NULL)
    mSystemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);
  else {
    mSystemMemoryMap = NULL;
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  //
  // IIO PCI-E Port Configuration
  //
  ConfigPciePort();

  DisplayMemTopology();

  VarSize = sizeof(SYSTEM_CONFIGURATION);

  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(
                            L"IntelSetup",
                            &mSystemConfigurationGuid,
                            &Attributes,
                            &VarSize,
                            &mSystemDefaults
                            );
  ASSERT_EFI_ERROR (Status);


  //
  // Default values of Setup configuration variable set to 0.
  //
  // default value assigned to setup variables
  mSystemDefaults.IIO0Present = 1;
  mSystemDefaults.IIO1Present = 1;
  mSystemDefaults.IIO2Present = 1;
  mSystemDefaults.IIO3Present = 1;

  IioBitMap =  mIioUds->IioUdsPtr->SystemStatus.socketPresentBitMap;  // storing value of IIO presence indicator byte in temporary variable
  if((IioBitMap & 0x1) == 0)
  {
	mSystemDefaults.IIO0Present = 0;
  }

  if((IioBitMap & 0x2) == 0)
  {
  	mSystemDefaults.IIO1Present = 0;
  }
  if((IioBitMap & 0x4) == 0)
  {
    mSystemDefaults.IIO2Present = 0;
  }

  if((IioBitMap & 0x8) == 0)
  {
    mSystemDefaults.IIO3Present = 0;
  }

  for (node = 0; node < MAX_MEM_NODE; node++) {
    if (mSystemMemoryMap->NodeInfo[node].Enabled == 0) continue;
    if (mSystemMemoryMap->NodeInfo[node].DramType == SPD_TYPE_DDR3) {
      mSystemDefaults.DDR3DimmsPresent = 1; //1 for DDR3
    } else {
      mSystemDefaults.DDR3DimmsPresent = 0; //O for DDR4
    }
  }

  mSystemDefaults.cpuType = mSystemMemoryMap->cpuType;
 
#ifdef DE_SKU  
  mSystemDefaults.DE_NS_SKU_ID = mIioUds->IioUdsPtr->SystemStatus.DE_NS_SKU_ID;
#endif

  mSystemDefaults.XMPProfilesSup = mSystemMemoryMap->XMPProfilesSup;

  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->SetVariable(
		            L"IntelSetup",
		            &mSystemConfigurationGuid,
					Attributes,
		            sizeof (SYSTEM_CONFIGURATION),
		            &mSystemDefaults
	                    );
  ASSERT_EFI_ERROR (Status);

  //
  // Update CSI status lines
  //
  UpdateCSIStatusLines ();

  //
  // Update CSI Resource Allocation Lines
  //
  UpdateCSIResourceAllocationLines ();

  //
  // Update Dram type strings
  //
  UpdateDramTypeStrings( ( BOOLEAN )mSystemDefaults.DDR3DimmsPresent );

  return ;
}

typedef struct {
  UINT16  MfgId;
  CHAR16  String[16];
} MEMORY_MODULE_MANUFACTURER_LIST;

MEMORY_MODULE_MANUFACTURER_LIST MemoryModuleManufacturerList[] =
{
  {0x2c00, L"Micron"},
  {0xce00, L"Samsung"},
  {0x4f01, L"Transcend"},
  {0x9801, L"Kinston"},
  {0xfe02, L"Elpida"},
  {0xad00, L"Hynix"},
  {0xc100, L"Infineon"},
  {0x9401, L"Smart"},
  {0x5705, L"Aeneon"},
  {0x5105, L"Qimonda"}
};

VOID
DisplayMemTopology (
  VOID
  )
/*++

Routine Description:

  This function displays System Memory Info.

Arguments:

  None

Returns:

  None

--*/
{
  struct      SystemMemoryMapHob *SystemMemoryMap;
  UINT8       Node,Channel,Dimm,socket;
  UINT8       haNum;
  UINT8       haCh;
  UINT8       MfdSize,MfdIndex,Cntr;
  UINT16      DimmSize;
  CHAR16      *DimmString1, *DimmString2;
  UINTN       StrSize;
  VOID        *HobList;
  EFI_STATUS  Status;
  STRING_REF  TokenToUpdate;
  EFI_HOB_GUID_TYPE   *GuidHob;
  UINT16      MemFreqArray[18] = {800, 1000, 1067, 1200, 1333, 1400, 1600, 1800, 1867, 2000, 2133, 2200, 2400, 2600, 2667,
                                 2800, 2933, 3000};
  STRING_REF            MemTopology[] = {
       STRING_TOKEN(STR_MEM_STRING1), STRING_TOKEN(STR_MEM_STRING2),STRING_TOKEN(STR_MEM_STRING3),STRING_TOKEN(STR_MEM_STRING4),
       STRING_TOKEN(STR_MEM_STRING5), STRING_TOKEN(STR_MEM_STRING6),STRING_TOKEN(STR_MEM_STRING7),STRING_TOKEN(STR_MEM_STRING8),
       STRING_TOKEN(STR_MEM_STRING9), STRING_TOKEN(STR_MEM_STRING10),STRING_TOKEN(STR_MEM_STRING11),STRING_TOKEN(STR_MEM_STRING12),
       STRING_TOKEN(STR_MEM_STRING13), STRING_TOKEN(STR_MEM_STRING14),STRING_TOKEN(STR_MEM_STRING15),STRING_TOKEN(STR_MEM_STRING16),
       STRING_TOKEN(STR_MEM_STRING17), STRING_TOKEN(STR_MEM_STRING18),STRING_TOKEN(STR_MEM_STRING19),STRING_TOKEN(STR_MEM_STRING20),
       STRING_TOKEN(STR_MEM_STRING21), STRING_TOKEN(STR_MEM_STRING22),STRING_TOKEN(STR_MEM_STRING23),STRING_TOKEN(STR_MEM_STRING24),
       STRING_TOKEN(STR_MEM_STRING25), STRING_TOKEN(STR_MEM_STRING26),STRING_TOKEN(STR_MEM_STRING27),STRING_TOKEN(STR_MEM_STRING28),
       STRING_TOKEN(STR_MEM_STRING29), STRING_TOKEN(STR_MEM_STRING30),STRING_TOKEN(STR_MEM_STRING31),STRING_TOKEN(STR_MEM_STRING32),
       STRING_TOKEN(STR_MEM_STRING33), STRING_TOKEN(STR_MEM_STRING34),STRING_TOKEN(STR_MEM_STRING35),STRING_TOKEN(STR_MEM_STRING36),
       STRING_TOKEN(STR_MEM_STRING37), STRING_TOKEN(STR_MEM_STRING38),STRING_TOKEN(STR_MEM_STRING39),STRING_TOKEN(STR_MEM_STRING40),
       STRING_TOKEN(STR_MEM_STRING41), STRING_TOKEN(STR_MEM_STRING42),STRING_TOKEN(STR_MEM_STRING43),STRING_TOKEN(STR_MEM_STRING44),
       STRING_TOKEN(STR_MEM_STRING45), STRING_TOKEN(STR_MEM_STRING46),STRING_TOKEN(STR_MEM_STRING47),STRING_TOKEN(STR_MEM_STRING48),
       STRING_TOKEN(STR_MEM_STRING49), STRING_TOKEN(STR_MEM_STRING50),STRING_TOKEN(STR_MEM_STRING51),STRING_TOKEN(STR_MEM_STRING52),
       STRING_TOKEN(STR_MEM_STRING53), STRING_TOKEN(STR_MEM_STRING54),STRING_TOKEN(STR_MEM_STRING55),STRING_TOKEN(STR_MEM_STRING56),
       STRING_TOKEN(STR_MEM_STRING57), STRING_TOKEN(STR_MEM_STRING58),STRING_TOKEN(STR_MEM_STRING59),STRING_TOKEN(STR_MEM_STRING60),
       STRING_TOKEN(STR_MEM_STRING61), STRING_TOKEN(STR_MEM_STRING62),STRING_TOKEN(STR_MEM_STRING63),STRING_TOKEN(STR_MEM_STRING64),
       STRING_TOKEN(STR_MEM_STRING65), STRING_TOKEN(STR_MEM_STRING66),STRING_TOKEN(STR_MEM_STRING67),STRING_TOKEN(STR_MEM_STRING68),
       STRING_TOKEN(STR_MEM_STRING69), STRING_TOKEN(STR_MEM_STRING70),STRING_TOKEN(STR_MEM_STRING71),STRING_TOKEN(STR_MEM_STRING72),
       STRING_TOKEN(STR_MEM_STRING73), STRING_TOKEN(STR_MEM_STRING74),STRING_TOKEN(STR_MEM_STRING75),STRING_TOKEN(STR_MEM_STRING76),
       STRING_TOKEN(STR_MEM_STRING77), STRING_TOKEN(STR_MEM_STRING78),STRING_TOKEN(STR_MEM_STRING79),STRING_TOKEN(STR_MEM_STRING80),
       STRING_TOKEN(STR_MEM_STRING81), STRING_TOKEN(STR_MEM_STRING82),STRING_TOKEN(STR_MEM_STRING83),STRING_TOKEN(STR_MEM_STRING84),
       STRING_TOKEN(STR_MEM_STRING85), STRING_TOKEN(STR_MEM_STRING86),STRING_TOKEN(STR_MEM_STRING87),STRING_TOKEN(STR_MEM_STRING88),
       STRING_TOKEN(STR_MEM_STRING89), STRING_TOKEN(STR_MEM_STRING90),STRING_TOKEN(STR_MEM_STRING91),STRING_TOKEN(STR_MEM_STRING92),
       STRING_TOKEN(STR_MEM_STRING93), STRING_TOKEN(STR_MEM_STRING94),STRING_TOKEN(STR_MEM_STRING95),STRING_TOKEN(STR_MEM_STRING96)
   };

  //
  // Search for the Memory Map GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if (GuidHob != NULL){
     SystemMemoryMap = (struct SystemMemoryMapHob*) GET_GUID_HOB_DATA (GuidHob);
  } else {
    SystemMemoryMap = NULL;
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  StrSize   = 0x250;
  DimmString1 = AllocateZeroPool (StrSize);
  DimmString2 = AllocateZeroPool (StrSize);
  Cntr = 0;
  MfdSize = sizeof (MemoryModuleManufacturerList) / sizeof (MemoryModuleManufacturerList[0]);
  for (Node = 0; Node < MAX_MEM_NODE; Node++) {
    socket = NODE_TO_SKT(Node);
    haNum = NODE_TO_MC(Node);
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      haCh = Channel;
      if(SystemMemoryMap->NodeInfo[Node].NumChPerHA == 2) { // 2 HA case.
        if(((haNum == 0) && (Channel >= 2)) || ((haNum == 1) && (Channel <  2)))  continue;   // skip ha0ch2,3 and ha1ch 0,1
        // 2ha : ch0,1,2,3 -> ha0ch0,1 and Ha1ch0,1
        if(haNum == 1) {
          haCh = Channel - 2;
        }
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (SystemMemoryMap->NodeInfo[Node].Enabled == 0 || SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].Enabled == 0
            || SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].Present == 0) {
          UnicodeSPrint (DimmString1,StrSize,L"");
        } else {
          for(MfdIndex = 0; MfdIndex < MfdSize; MfdIndex++) {
            if(MemoryModuleManufacturerList[MfdIndex].MfgId == SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].SPDMMfgId) {
               break;
            }
          } // MfdIndex loop
          if (MfdIndex < MfdSize) {
            UnicodeSPrint (DimmString1, StrSize, L"Socket%d.Ch%d.Dimm%d: %dMT/s %s ", socket, Channel, Dimm,
                           MemFreqArray[SystemMemoryMap->NodeInfo[Node].MemFreq], MemoryModuleManufacturerList[MfdIndex].String);
          } else {
            UnicodeSPrint (DimmString1, StrSize, L"Socket%d.Ch%d.Dimm%d: %dMT/s UNKNOWN ", socket, haCh, Dimm,
                           MemFreqArray[SystemMemoryMap->NodeInfo[Node].MemFreq]);
          }
          DimmSize = SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].DimmSize >> 4;
          if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].NumRanks == 1) {
            if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].X4Present) {
              UnicodeSPrint(DimmString2,StrSize, L"SRx4 %dGB", DimmSize);
            } else {
              UnicodeSPrint(DimmString2,StrSize, L"SRx8 %dGB", DimmSize);
            }
          } else if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].NumRanks == 2) {
            if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].X4Present) {
              UnicodeSPrint(DimmString2,StrSize, L"DRx4 %dGB", DimmSize);
            } else {
              UnicodeSPrint(DimmString2,StrSize, L"DRx8 %dGB", DimmSize);
            }
          } else if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].NumRanks == 4) {
            if (SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].X4Present) {
              UnicodeSPrint(DimmString2,StrSize, L"QRx4 %dGB", DimmSize);
            } else {
              UnicodeSPrint(DimmString2,StrSize, L"QRx8 %dGB", DimmSize);
            }
          }

          StrCat(DimmString1, DimmString2);

          switch (SystemMemoryMap->NodeInfo[Node].DimmType) {
            case 0:
              if (SystemMemoryMap->NodeInfo[Node].LrDimmPresent)  {
                UnicodeSPrint(DimmString2,StrSize, L" LRDIMM");
              } else {
                UnicodeSPrint(DimmString2,StrSize, L" RDIMM");
              }
              break;
            case 1:
              UnicodeSPrint(DimmString2,StrSize, L" UDIMM");
              break;
            case 2:
              UnicodeSPrint(DimmString2,StrSize, L" SODIMM");
              break;
            case 9:
              UnicodeSPrint(DimmString2,StrSize, L" LRDIMM");
              break;
            default:
              UnicodeSPrint(DimmString2,StrSize, L"N/A");
              break;
          }
          StrCat(DimmString1, DimmString2);
        }
        TokenToUpdate = MemTopology[Cntr];
        HiiSetString(mAdvancedHiiHandle, TokenToUpdate, DimmString1, NULL);

        if (SystemMemoryMap->NodeInfo[Node].Enabled && SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].Enabled
            && SystemMemoryMap->NodeInfo[Node].ChannelInfo[haCh].DimmInfo[Dimm].Present) {

          Cntr++;

        }
      } // dimm loop
    } // Channel loop
  } // Socket loop

  //
  // Fill in the remaining strings
  //
  if (Cntr < 96) {
    for (Cntr; Cntr < 96; Cntr++) {
      UnicodeSPrint (DimmString1,StrSize,L"");
      TokenToUpdate = MemTopology[Cntr];
      HiiSetString(mAdvancedHiiHandle, TokenToUpdate, DimmString1, NULL);
    } // Cntr loop
  }
  gBS->FreePool (DimmString1);
  gBS->FreePool (DimmString2);
}


// AptioV Server Override Start: Commenting the unwanted Intel Setup Page
#if 0
VOID
ReserveMemPage(
   VOID
)
/*++

Routine Description:

  This is the function for initialization information on ReserveMem page.

Arguments:

  VOID.

Returns:

  VOID.

--*/
{
  EFI_STATUS                            Status;
  UINTN                                 VarSize;
  BOOLEAN                               ReserveMemFlag;
  CHAR16                                *NewString;
  EFI_STRING_ID                         TokenToUpdate;

  //
  //  Create buffer to hold the string.
  //
  NewString = AllocateZeroPool (NEWSTRING_SIZE);

  //
  //  Get Reserve Memory result to update the Status string.
  //  If GetVariable return error, set ReserveMemFlag to FALSE.
  //
  Status = gRT->GetVariable (L"ReserveMemFlag", &mSystemConfigurationGuid, NULL, &VarSize, &ReserveMemFlag);

  if ( EFI_ERROR (Status)) {
    ReserveMemFlag = FALSE;
  }

  //
  //  Fill the string by the Flag.
  //
  if ( ReserveMemFlag ) {
    StrCpy ( NewString, L"Memory Reserved ");
  }
  else {
    StrCpy ( NewString, L"Reserved Fail ");
  }

  //
  //  Update the Status string to display results of page reservation.
  //
  TokenToUpdate = (STRING_REF) STR_RESERVE_MEMORY_RESULT_VALUE;
  HiiSetString(mAdvancedHiiHandle, TokenToUpdate, NewString, NULL);

  gBS->FreePool (NewString);

  return;
}
#endif
// AptioV Server Override end: Commenting the unwanted Intel Setup Page

// AptioV server override start: Changes to update IIOxPresent setup variable
VOID
UpdateIioPresent (
VOID
)
/*++

Routine Description:

  Function updates IIOxPresent setupvariable
  
 
++*/
{
  EFI_STATUS  Status;
  UINTN       VarSize;
  UINT32      IioBitMap;

  VarSize = sizeof(SYSTEM_CONFIGURATION);

  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->GetVariable(
                            L"IntelSetup",
                            &mSystemConfigurationGuid,
                            NULL,
                            &VarSize,
                            &mSystemDefaults
                            );
  ASSERT_EFI_ERROR (Status);


  //
  // Default values of Setup configuration variable set to 0.
  //
  // default value assigned to setup variables
  mSystemDefaults.IIO0Present = 1;
  mSystemDefaults.IIO1Present = 1;
  mSystemDefaults.IIO2Present = 1;
  mSystemDefaults.IIO3Present = 1;
  
  // AptioV server override start: mIioUds structure is initialized by locating gEfiIioUdsProtocolGuid
  Status = gBS->LocateProtocol (
                    &gEfiIioUdsProtocolGuid,
                    NULL,
                    &mIioUds
                    );
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR(Status)) {
      IioBitMap =  mIioUds->IioUdsPtr->SystemStatus.socketPresentBitMap;  // storing value of IIO presence indicator byte in temporary variable
      if((IioBitMap & 0x1) == 0)
      {
          mSystemDefaults.IIO0Present = 0;
      }

      if((IioBitMap & 0x2) == 0)
      {
          mSystemDefaults.IIO1Present = 0;
      }
      if((IioBitMap & 0x4) == 0)
      {
          mSystemDefaults.IIO2Present = 0;
      }

      if((IioBitMap & 0x8) == 0)
      {
          mSystemDefaults.IIO3Present = 0;
      }
  }
  // AptioV server override End: mIioUds structure is initialized by locating gEfiIioUdsProtocolGuid
  // AptioV server override: Changed variable name to Intelsetup
  Status = gRT->SetVariable(
		            L"IntelSetup",
		            &mSystemConfigurationGuid,
		            EFI_VARIABLE_BOOTSERVICE_ACCESS |EFI_VARIABLE_NON_VOLATILE,
		            sizeof (SYSTEM_CONFIGURATION),
		            &mSystemDefaults
	                    );
  ASSERT_EFI_ERROR (Status);
  
}
// AptioV server override End: Changes to update IIOxPresent setup variable

VOID
UpdateMemType (
  VOID
)
{
  EFI_STATUS                              Status;
  EFI_SMBIOS_HANDLE                       SmbiosHandle;
  EFI_SMBIOS_PROTOCOL                    *Smbios;
  EFI_SMBIOS_TYPE                         SmbiosType;
  EFI_STRING                              AdvancedString;
  EFI_STRING                              MemoryTypeString;
  SMBIOS_TABLE_TYPE17                    *SmbiosType17Record;
  CHAR16                                  MemoryTypeChar;
  UINTN                                   i;

  DEBUG((EFI_D_INFO, "Enter UpdateMemType.\n"));

  // Get memory type from SMBIOS
  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID **) &Smbios
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Can not locate SMBIOS protocol.\n"));
    goto Done;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  SmbiosType = EFI_SMBIOS_TYPE_MEMORY_DEVICE;
  Status = Smbios->GetNext (
                    Smbios,
                    &SmbiosHandle,
                    &SmbiosType,
                    (EFI_SMBIOS_TABLE_HEADER **) (&SmbiosType17Record),
                    NULL);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_ERROR, "Can not find SMBIOS information.\n"));
    goto Done;
  }

  DEBUG((EFI_D_INFO, "SmbiosType17Record->MemoryType:0x%x\n", SmbiosType17Record->MemoryType));

  switch (SmbiosType17Record->MemoryType) {
    case MemoryTypeDdr3:
      MemoryTypeChar = L'3';
      break;
    default:
      MemoryTypeChar = L'4';
      break;
  }

  DEBUG((EFI_D_INFO, "Memory type is DDR%c\n", MemoryTypeChar));

  // Get string from Advanced menu.
  for (i = 0; i < sizeof (MemTypeStrIdTab) / sizeof (EFI_STRING_ID); ++i) {
    AdvancedString = HiiGetString (mAdvancedHiiHandle, MemTypeStrIdTab [i], NULL);
    if (NULL == AdvancedString) {
      DEBUG((EFI_D_ERROR, "Can not found memory type string.\n"));
      continue;
    }

    // Search the DDR# string
    MemoryTypeString = StrStr (AdvancedString, MEMORY_TYPE_DDR_STR);
    if (MemoryTypeString != NULL) {
      MemoryTypeString [3] = MemoryTypeChar;
      HiiSetString(mAdvancedHiiHandle, MemTypeStrIdTab [i], AdvancedString, NULL);
      DEBUG((EFI_D_INFO, "Updated memory type string:%s. \n", AdvancedString));
    }

    FreePool (AdvancedString);
    AdvancedString = NULL;
  }
Done:
  return;
}
