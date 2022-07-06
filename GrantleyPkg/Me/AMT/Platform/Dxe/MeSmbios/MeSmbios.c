/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
 
Copyright (c) 2013 Intel Corporation.  All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  MeSmbios.c
  
Abstract: 

  This driver parses the mMiscSubclassDataTable structure and reports
  any generated data to the DataHub.

--*/

#include "MeSmbios.h"
#include <PiDxe.h>
#include <Protocol/DataHub.h>
#include <Protocol/Smbios.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/SmBios.h>
#include "Library/MeTypeLib.h"
#ifndef USE_DATA_HUB_PROTOCOL
#else
extern EFI_GUID gMiscProducerGuid;
#endif // USE_DATA_HUB_PROTOCOL

extern UINT8 SmbiosMiscStrings[];

/**
  Add an SMBIOS record.

  @param  SmBios                The EFI_SMBIOS_PROTOCOL instance.
  @param  SmBiosHandle          A unique handle will be assigned to the SMBIOS record.
  @param  Record                The data for the fixed portion of the SMBIOS record. The format of the record is
                                determined by EFI_SMBIOS_TABLE_HEADER.Type. The size of the formatted area is defined 
                                by EFI_SMBIOS_TABLE_HEADER.Length and either followed by a double-null (0x0000) or 
                                a set of null terminated strings and a null.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.

**/
EFI_STATUS
AddSmbiosRecord (
  IN EFI_SMBIOS_PROTOCOL        *SmBios,
  OUT EFI_SMBIOS_HANDLE         *SmBiosHandle,
  IN EFI_SMBIOS_TABLE_HEADER    *Record
  )
{
  *SmBiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  return SmBios->Add (
                   SmBios,
                   NULL,
                   SmBiosHandle,
                   Record
                   );
}


VOID
UpdateMeSmbiosTable (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
/*++
Description:

  Parses the mMiscSubclassDataTable structure and reports SMBIOS tables 82 and 83 to the
  DataHub.

Arguments:

  Event                   Pointer to the event that triggered this Callback Function
  Context                 VOID Pointer required for Callback function

Returns:

  EFI_SUCCESS             The data was successfully reported to the Data Hub

--*/
{
  EFI_MISC_SUBCLASS_DRIVER_DATA RecordData;
#ifndef USE_DATA_HUB_PROTOCOL
  EFI_SMBIOS_PROTOCOL           *SmBios;
  EFI_SMBIOS_HANDLE             SmBiosHandle;
#else
  EFI_DATA_HUB_PROTOCOL         *DataHub;
#endif // USE_DATA_HUB_PROTOCOL

  EFI_STATUS                    EfiStatus;
  UINTN                         Index;
  BOOLEAN                       LogRecordData;
  UINT32                        ThisRecordType;
  UINT32                        LastRecordType;
  //
  // Initialize constant portion of subclass header.
  //
  RecordData.Header.Version = EFI_MISC_SUBCLASS_VERSION;
  RecordData.Header.HeaderSize = sizeof(EFI_SUBCLASS_TYPE1_HEADER);
  RecordData.Header.Instance = 0;
  RecordData.Header.SubInstance = 0;
  ThisRecordType = 0;
  LastRecordType = 0;

#ifndef USE_DATA_HUB_PROTOCOL
  EfiStatus = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, &SmBios);
  if (EFI_ERROR(EfiStatus)) {
    DEBUG((EFI_D_ERROR, "Could not locate SmBios protocol: %r\n", EfiStatus));
    return;
  } else if (SmBios == NULL) {
    DEBUG((EFI_D_ERROR, "LocateProtocol(SmBios) returned NULL pointer!\n"));
    return;
  }
#else
  EfiStatus = gBS->LocateProtocol(&gEfiDataHubProtocolGuid, NULL, &DataHub);
  if (EFI_ERROR(EfiStatus)) {
    DEBUG((EFI_D_ERROR, "Could not locate DataHub protocol: %r\n", EfiStatus));
    return;
  } else if (DataHub == NULL) {
    DEBUG((EFI_D_ERROR, "LocateProtocol(DataHub) returned NULL pointer!\n"));
    return;
  }
#endif // USE_DATA_HUB_PROTOCOL

  for (Index = 0; Index < mSmbiosMiscDataTableEntries; Index++) {

    //
    // Initialize per-record portion of subclass header and
    // copy static data into data portion of subclass record.
    //
    RecordData.Header.RecordType = mMiscSubclassDataTable[Index].RecordType;
    
    ThisRecordType = RecordData.Header.RecordType;
    if (ThisRecordType != LastRecordType) {
      LastRecordType = ThisRecordType;
      RecordData.Header.Instance ++;
      RecordData.Header.SubInstance = 1;
    } else {      
      RecordData.Header.SubInstance ++;
    }
    
    if (mMiscSubclassDataTable[Index].RecordData == NULL) {
      ZeroMem (&RecordData.Record, mMiscSubclassDataTable[Index].RecordLen);
    } else {
      CopyMem (
            &RecordData.Record, 
            mMiscSubclassDataTable[Index].RecordData, 
            mMiscSubclassDataTable[Index].RecordLen
            );
    }

    //
    // If the entry does not have a function pointer, just log the data.
    //
    if (mMiscSubclassDataTable[Index].Function == NULL) {
#ifndef USE_DATA_HUB_PROTOCOL
      EfiStatus = AddSmbiosRecord(SmBios, &SmBiosHandle, (EFI_SMBIOS_TABLE_HEADER *) &(RecordData.Record));

      if (EFI_ERROR(EfiStatus)) {
        DEBUG ((
            EFI_D_ERROR, "MeSmBios#1: AddSmBiosRecord(%d bytes) == %r\n",
            mMiscSubclassDataTable[Index].RecordLen,
            EfiStatus
            ));
      }
#else
      //
      // Log RecordData to Data Hub.
      //
      EfiStatus = DataHub->LogData(
                              DataHub,
                              &gEfiMiscSubClassGuid,
                              &gMiscProducerGuid,
                              EFI_DATA_RECORD_CLASS_DATA,
                              &RecordData,
                              sizeof(EFI_SUBCLASS_TYPE1_HEADER) + mMiscSubclassDataTable[Index].RecordLen
                              );
      if (EFI_ERROR(EfiStatus)) {
        DEBUG ((
           EFI_D_ERROR, "LogData(%d bytes) == %r\n", 
           sizeof(EFI_SUBCLASS_TYPE1_HEADER) +
           mMiscSubclassDataTable[Index].RecordLen,
           EfiStatus
           ));
      }
#endif // USE_DATA_HUB_PROTOCOL

      continue;
    }

    //
    // The entry has a valid function pointer.
    // Keep calling the function and logging data until there
    // is no more data to log.
    //
    for (;;) {
      EfiStatus = (*mMiscSubclassDataTable[Index].Function) (
                            mMiscSubclassDataTable[Index].RecordType,
                            &mMiscSubclassDataTable[Index].RecordLen,
                            &RecordData.Record,
                            &LogRecordData
                            );
      if (EFI_ERROR(EfiStatus)) {
        break;
      }

      if (!LogRecordData) {
        break;
      }
#ifndef USE_DATA_HUB_PROTOCOL
      EfiStatus = AddSmbiosRecord(SmBios, &SmBiosHandle, (EFI_SMBIOS_TABLE_HEADER *) &(RecordData.Record));
      if (EFI_ERROR(EfiStatus)) {
        DEBUG ((
            EFI_D_ERROR, "MeSmBios#2: AddSmBiosRecord(%d bytes) == %r\n",
            mMiscSubclassDataTable[Index].RecordLen,
            EfiStatus
            ));
      }
#else
      EfiStatus = DataHub->LogData(
                              DataHub,
                              &gEfiMiscSubClassGuid,
                              &gMiscProducerGuid,
                              EFI_DATA_RECORD_CLASS_DATA,
                              &RecordData,
                              sizeof(EFI_SUBCLASS_TYPE1_HEADER) + mMiscSubclassDataTable[Index].RecordLen
                              );

      if (EFI_ERROR(EfiStatus)) {
        DEBUG ((
            EFI_D_ERROR, "LogData(%d bytes) == %r\n",
            sizeof(EFI_SUBCLASS_TYPE1_HEADER) + mMiscSubclassDataTable[Index].RecordLen,
            EfiStatus
            ));
      }
#endif // USE_DATA_HUB_PROTOCOL

    }
  }

  return;
}

EFI_STATUS 
EFIAPI
MeSmbiosDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
/*++
Description:

  Standard EFI driver point. This driver registers  the notify function to reports any generated
  data to the DataHub.

Arguments:

  ImageHandle             Handle for the image of this driver
  SystemTable             Pointer to the EFI System Table

Returns:

  EFI_SUCCESS             The function completed successfully

--*/
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID        *Registration;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif //SPS_SUPPORT

  // EfiInitializeDriverLib(ImageHandle, SystemTable);

  //
  // Create a callback event to check MBP FWSTS bit
  //
  Status = gBS->CreateEvent (
              EVT_NOTIFY_SIGNAL,
              TPL_NOTIFY, // need to guarantee this runs before Mbp data is removed and MEBx loader push table to BPF
              UpdateMeSmbiosTable,
              NULL,
              &Event
              );
  ASSERT_EFI_ERROR (Status);
  //
  // Register EXIT_PM_AUTH_PROTOCOL notify function
  //
  Status = gBS->RegisterProtocolNotify (
              &gExitPmAuthProtocolGuid,
              Event,
              &Registration
              );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
