/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 2011 - 2014, Intel Corporation. All rights reserved.
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

--*/
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/DevicePath.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/IdeControllerInit.h>
#include <Library/UefiBootManagerLib.h>
#include <MePlatformPolicy.h>
#include <CommonIncludes.h>
#include "MeState.h"
#include "AmtPetAlert.h"
#include "Library/MeTypeLib.h"

EFI_STATUS
EFIAPI
InitializeAmtPetAlert (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
)
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  //
  // Register Ready to Boot Event for AMT Wrapper
  //
  Status = EfiCreateEventReadyToBootEx (
              TPL_CALLBACK,
              AmtPetAlertReadyToBoot,
              (VOID *) &ImageHandle,
              &ReadyToBootEvent
              );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

VOID SwapEntries (
  IN CHAR8    *Data,
  IN UINT16   Size
)
{
  UINT16  Index;
  CHAR8   Temp8;

  for (Index = 0; (Index+1) < Size; Index+=2) {
    Temp8           = Data[Index];
    Data[Index]     = Data[Index + 1];
    Data[Index + 1] = Temp8;
  }
}

// Example 1: 16 10 0F 6F 02 68 08 FF FF 22 00 40 13 XX XX XX
// Example 2: 15 10 0F 6F 02 68 10 FF FF 22 00 AA 13 03 03 02
BOOT_AUDIT_ENTRY mPetAlertWithoutChange = {
  0x16, 0x10, 0x0F, 0x6F, 0x02, 0x68, 0x08, 0xFF, 0xFF, 0x22, 0x00, 0x40, 0x13, 0x00, 0x00, 0x00
};

BOOT_AUDIT_ENTRY mPetAlertWithChange = {
  0x15, 0x10, 0x0F, 0x6F, 0x02, 0x68, 0x10, 0xFF, 0xFF, 0x22, 0x00, 0xAA, 0x13, 0x03, 0x03, 0x02
};

EFI_STATUS
SendBaeMessage(
  BOOT_AUDIT_ENTRY                *Buffer
)
{
  EFI_HECI_PROTOCOL               *Heci;
  UINT32                          HeciLength;
  BOOT_AUDIT_ENTRY_PACK           Pack;
  UINT32                          MeStatus;
  EFI_STATUS                      TempStatus;
  EFI_STATUS                      Status;

  CopyMem(&Pack.Data, Buffer, sizeof(BOOT_AUDIT_ENTRY));

  Pack.Command                = EFI_ASF_MESSAGE_COMMAND_MESSAGE;  // 0x04
  Pack.ByteCount              = sizeof(BOOT_AUDIT_ENTRY);
  HeciLength                  = sizeof(BOOT_AUDIT_ENTRY_PACK);    // length include Command & ByteCount

  Status = gBS->LocateProtocol (
                &gEfiHeciProtocolGuid,
                NULL,
                &Heci
                );
  if (!EFI_ERROR(Status)) {
    TempStatus = Heci->GetMeStatus(&MeStatus);
    ASSERT_EFI_ERROR(TempStatus);

    //
    // Only send ASF Push Progress code when ME is ready.  Ignore FW Init Status.
    //
    if (ME_STATUS_ME_STATE_ONLY(MeStatus) == ME_READY) {
      Status = Heci->SendMsg(
                      (UINT32*)&Pack,
                      HeciLength,
                      BIOS_ASF_HOST_ADDR,
                      HECI_ASF_MESSAGE_ADDR
                      );
      if (EFI_ERROR(Status)) {

      }
    } else {

    }
  } else {

  }

  return Status;
}

EFI_STATUS
PetAlert(
  UINT8                   DeviceType,
  CHAR8                   *HarddriveTag
)
{
  PET_ALERT_CFG           PetAlertCfgVar;
  UINTN                   VarSize;
  UINTN                   UpdateFlag = 0;
  EFI_STATUS              Status;
  BOOT_AUDIT_ENTRY        *PetAlertMsg = &mPetAlertWithoutChange;

  if (HarddriveTag == NULL) {
    HarddriveTag = "";
  }

  VarSize = sizeof(PetAlertCfgVar);
  Status = gRT->GetVariable (
                L"PetAlertCfg",
                &gEfiSetupVariableGuid,
                NULL,
                &VarSize,
                &PetAlertCfgVar
                );
  if (EFI_ERROR (Status)) {
    UpdateFlag = 1;
    PetAlertCfgVar.BootQueue[0] = DeviceType;
    PetAlertCfgVar.BootQueue[1] = DeviceType;
    PetAlertCfgVar.BootQueue[2] = DeviceType;
    AsciiStrCpy(PetAlertCfgVar.BootHarddriveTag, HarddriveTag);
  }

  DEBUG((DEBUG_INFO, "PetAlert HDD ModelSerialNumber = %a\n", PetAlertCfgVar.BootHarddriveTag));

  if ((DeviceType != BAE_HDD_DEVICE) ||
    ((DeviceType == BAE_HDD_DEVICE) && AsciiStrCmp(HarddriveTag, PetAlertCfgVar.BootHarddriveTag))) {                   
    //
    // If booting from a network/removable device or from a different HDD send expanded BAE message
    //
    UpdateFlag = 1;
    PetAlertMsg               = &mPetAlertWithChange;
    PetAlertMsg->EventData5   = PetAlertCfgVar.BootQueue[2] = PetAlertCfgVar.BootQueue[1];
    PetAlertMsg->EventData4   = PetAlertCfgVar.BootQueue[1] = PetAlertCfgVar.BootQueue[0];
    PetAlertMsg->EventData3   = PetAlertCfgVar.BootQueue[0] = DeviceType;
  }

  SendBaeMessage(PetAlertMsg);

  if (UpdateFlag) {
    AsciiStrCpy(PetAlertCfgVar.BootHarddriveTag, HarddriveTag);
    VarSize = sizeof(PetAlertCfgVar);
    Status = gRT->SetVariable (
                  L"PetAlertCfg",
                  &gEfiSetupVariableGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  VarSize,
                  &PetAlertCfgVar
                  );
    ASSERT_EFI_ERROR(Status);
  }

  return Status;
}

/**
  Compare two device paths up to a size of Boot Opion's Device Path

  @param[in] BootOptionDp     Device path acquired from BootXXXX EFI variable
  @param[in] FileSysDp    Device path acquired through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer

  @retval TRUE   Both device paths point to the same device
  @retval FALSE   Device paths point to different devices
**/
BOOLEAN
CompareDevicePaths(
  IN  EFI_DEVICE_PATH_PROTOCOL *BootOptionDp,
  IN  EFI_DEVICE_PATH_PROTOCOL *FileSysDp
)
{
  UINTN BootOptionDpSize;
  UINTN FileSysDpSize;  

  if (BootOptionDp == NULL || FileSysDp == NULL) {

    return FALSE;
  }

  BootOptionDpSize = GetDevicePathSize (BootOptionDp) - END_DEVICE_PATH_LENGTH;
  FileSysDpSize    = GetDevicePathSize (FileSysDp) - END_DEVICE_PATH_LENGTH;

  if ((BootOptionDpSize <= FileSysDpSize) && (CompareMem (FileSysDp, BootOptionDp, BootOptionDpSize) == 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get EFI device path through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer. Acquired path must
  point to the same device as argument DevicePath passed to the function.

  @param[in] DevicePath   Device path acquired from BootXXXX EFI variable

  @retval EFI_DEVICE_PATH_PROTOCOL   Device path for booting
**/
EFI_DEVICE_PATH_PROTOCOL *
GetFullBootDevicePath(
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
)
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL *ReturnDevicePath;
  UINTN                    HandleNum;
  EFI_HANDLE               *HandleBuf;
  UINTN                    Index;

  ReturnDevicePath = NULL;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleNum,
                  &HandleBuf
                  );
  if ((EFI_ERROR (Status)) || (HandleBuf == NULL)) {
    return NULL;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    TempDevicePath     = DevicePathFromHandle (HandleBuf[Index]);    

    if (CompareDevicePaths(DevicePath, TempDevicePath)) {
      ReturnDevicePath = DuplicateDevicePath(TempDevicePath);
      break;
    }
  }
  
  return ReturnDevicePath;
}

UINT8
GetDeviceType(
  IN EFI_DEVICE_PATH_PROTOCOL *DevPath
  )
{
  UINT8                    DeviceType;
  BBS_BBS_DEVICE_PATH      *BbsDevicePath;

  DeviceType    = BAE_EMPTY_QUEUE;    
  BbsDevicePath = (BBS_BBS_DEVICE_PATH *)DevPath;

  if (BbsDevicePath->Header.Type == BBS_DEVICE_PATH && BbsDevicePath->Header.SubType == BBS_BBS_DP) {
    //
    // Get device type from legacy-BBS style device path
    //
    switch (BbsDevicePath->DeviceType) {
      case BBS_TYPE_EMBEDDED_NETWORK:
        DeviceType = BAE_NETWORK_DEVICE;
        break;
      case BBS_TYPE_HARDDRIVE:
        DeviceType = BAE_HDD_DEVICE;
        break;
      case BBS_TYPE_CDROM:
      case BBS_TYPE_USB:
      case BBS_TYPE_FLOPPY:
        DeviceType = BAE_REMOVABLE_DEVICE;
        break;
      default:
        DeviceType = BAE_EMPTY_QUEUE;
        break;
    }
  } else {   
    while (!IsDevicePathEnd(DevPath)) {
      if ((DevicePathType(DevPath) == MESSAGING_DEVICE_PATH) &&
                  (DevicePathSubType(DevPath) == MSG_USB_DP)){
        //
        // If device connected through USB, we automatically treat it as removable
        //
        DeviceType = BAE_REMOVABLE_DEVICE;
        break;
      } else if (DevicePathType(DevPath) == MEDIA_DEVICE_PATH) {
        if (DevicePathSubType(DevPath) == MEDIA_HARDDRIVE_DP) {
          DeviceType = BAE_HDD_DEVICE;
          break;
        } else if (DevicePathSubType(DevPath) == MEDIA_CDROM_DP) {
          DeviceType = BAE_REMOVABLE_DEVICE;
          break;
        } else if (DevicePathSubType(DevPath) == MEDIA_FILEPATH_DP) {
          DeviceType = BAE_NETWORK_DEVICE;
          break;
        }
      }
    
      DevPath = NextDevicePathNode(DevPath);
    }
  }
  
  return DeviceType;  
}

VOID
Trim(
  CHAR8 *sDesc,
  CHAR8 *sSrc
)
{
  UINTN p1 ,p2;
  CHAR8 ch;
  UINTN i,l;

  p1 = 0xFF;
  p2 = 0xFF;
  i = 0;
  for(;;) {
    ch = *(sSrc+i);
    if (ch == 0) {
      break;
    } else if (ch != 0x20) {
      p2 = i;
      if (p1 == 0xFF) {
        p1 = i;
      }
    }
      i++;
  }

  l = (p2 - p1) + 1;
  CopyMem(sDesc, sSrc+p1, l);
  *(sDesc+l) = 0;
}

EFI_STATUS
GetModelSerialNumber(
  IN EFI_HANDLE Handle,
  OUT CHAR8 *ModelSerialNumber
)
{
  EFI_STATUS                      Status = EFI_NOT_FOUND;
  EFI_DISK_INFO_PROTOCOL          *DiskInfo;
  EFI_ATAPI_IDENTIFY_DATA         *IdentifyDriveInfo = NULL;
  UINT32                          BufferSize;
  CHAR8                           ModelNumber[42];
  CHAR8                           SerialNumber[22];
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath = NULL;

  AsciiStrCpy(ModelSerialNumber, "");

  Status = gBS->HandleProtocol(
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  &DevicePath);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = gBS->LocateDevicePath (
                  &gEfiDiskInfoProtocolGuid,
                  &DevicePath,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDiskInfoProtocolGuid,
                  &DiskInfo
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  
  IdentifyDriveInfo = AllocatePool(sizeof(EFI_ATAPI_IDENTIFY_DATA));
  if (IdentifyDriveInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SetMem(IdentifyDriveInfo, sizeof (EFI_ATAPI_IDENTIFY_DATA), 0); 
  BufferSize = sizeof (EFI_ATAPI_IDENTIFY_DATA);
  Status = DiskInfo->Identify (
                        DiskInfo,
                        IdentifyDriveInfo,
                        &BufferSize
                        );
  if (EFI_ERROR(Status)) {
    FreePool(IdentifyDriveInfo);
    return Status;
  }    
                          
  CopyMem(SerialNumber, IdentifyDriveInfo->SerialNo, 20);
  SwapEntries (SerialNumber, 20);
  SerialNumber[20] = '\0';
  Trim(SerialNumber, SerialNumber);

  CopyMem(ModelNumber, IdentifyDriveInfo->ModelName, 40);
  SwapEntries (ModelNumber, 40);
  ModelNumber[40] = '\0';
  Trim(ModelNumber, ModelNumber);

  AsciiStrCpy(ModelSerialNumber, ModelNumber);
  AsciiStrCat(ModelSerialNumber, "_");
  AsciiStrCat(ModelSerialNumber, SerialNumber);

  FreePool(IdentifyDriveInfo);

  return EFI_SUCCESS;
}

EFI_STATUS
GetModelSerialNumberFromBbs(
  IN  UINT8   DeviceType,
  OUT CHAR8   *ModelSerialNumber    
)
{
  EFI_LEGACY_BIOS_PROTOCOL        *LegacyBios;
  UINT16                          HddCount = 0;
  HDD_INFO                        *LocalHddInfo = NULL;
  UINT16                          BbsCount = 0;
  BBS_TABLE                       *BbsTable;
  EFI_STATUS                      Status;
  UINTN                           i;
  EFI_HANDLE                      Handle;
  UINTN                           DeviceIndex;
  UINTN                           DevicePriority;

  if (ModelSerialNumber == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = LegacyBios->GetBbsInfo (
                          LegacyBios,
                          &HddCount,
                          &LocalHddInfo,
                          &BbsCount,
                          &BbsTable
                          );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  //
  // Find matching device by boot priority
  //
  DeviceIndex = (UINTN)-1;
  DevicePriority = (UINTN)-1;
  for (i = 0; i < BbsCount; i++) {
    if (BbsTable[i].BootPriority != BBS_IGNORE_ENTRY) {
      if (BbsTable[i].DeviceType == DeviceType) {               
        if (DevicePriority > BbsTable[i].BootPriority) {
          DevicePriority = BbsTable[i].BootPriority;
            DeviceIndex = i;
        }
      }
    }
  }

  //
  // Get model and serial number
  //
  if (DeviceIndex != (UINTN)-1) {
    Handle = *(VOID**)(&BbsTable[DeviceIndex].IBV1);            
    GetModelSerialNumber(Handle, ModelSerialNumber);
  }
  
  return EFI_SUCCESS;
}

EFI_STATUS
NotifyPetAlert(
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
)
{
  EFI_STATUS                      Status;
  UINT16                          BootCurrent;
  UINTN                           VarSize;
  CHAR16                          BootXXXX[16];
  EFI_DEVICE_PATH_PROTOCOL        *BootDevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *TempDevicePath;
  UINT8                           DeviceType;
  CHAR8                           ModelSerialNumber[64];
  EFI_HANDLE                      BootDeviceHandle;
  BBS_BBS_DEVICE_PATH *           LegacyBootDP;
  EFI_BOOT_MANAGER_LOAD_OPTION    BootOption;
  
  AsciiStrCpy(ModelSerialNumber, "");
  BootDeviceHandle            = 0;
  DeviceType                  = 0;
  BootDevicePath              = NULL;

  //
  // Get current boot option from NVRAM variables
  //
  VarSize = sizeof (UINT16);
  Status = gRT->GetVariable (
                L"BootCurrent",
                &gEfiGlobalVariableGuid,
                NULL,
                &VarSize,
                &BootCurrent
                );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  UnicodeSPrint(BootXXXX, sizeof(BootXXXX), L"Boot%04x", BootCurrent);

  Status = EfiBootManagerVariableToLoadOption (BootXXXX, &BootOption);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (((BBS_BBS_DEVICE_PATH *)BootOption.FilePath)->Header.Type == BBS_DEVICE_PATH) {
    BootDevicePath = DuplicateDevicePath(BootOption.FilePath);    
  } else {
    //
    // For EFI boot get full device path from EFI_SIMPLE_FILE_SYSTEM
    //
    BootDevicePath = GetFullBootDevicePath(BootOption.FilePath);
  }
  
  EfiBootManagerFreeLoadOption (&BootOption);
  
    if (BootDevicePath == NULL) {
    return EFI_ABORTED;
  }

  DeviceType = GetDeviceType(BootDevicePath);  

  //
  // Get model serial number if device is HDD
  //
  if (DeviceType == BAE_HDD_DEVICE) {
    LegacyBootDP = (BBS_BBS_DEVICE_PATH *)BootDevicePath;
    if (LegacyBootDP->Header.Type == BBS_DEVICE_PATH && 
      LegacyBootDP->Header.SubType == BBS_BBS_DP) {
      //
      // Temporary solution: use this string instead of serial/model no (pending DCN 2278153 implementation)
      //
      CopyMem(ModelSerialNumber, LegacyBootDP->String, AsciiStrLen(LegacyBootDP->String));
      //GetModelSerialNumberFromBbs(BBS_TYPE_HARDDRIVE, ModelSerialNumber);
    } else {
      TempDevicePath = BootDevicePath;
      Status = gBS->LocateDevicePath (
                    &gEfiSimpleFileSystemProtocolGuid,
                    &TempDevicePath,
                    &BootDeviceHandle
                    );

      if (EFI_ERROR(Status)) {
        FreePool(BootDevicePath);
        return Status;
      }

      GetModelSerialNumber(BootDeviceHandle, ModelSerialNumber);      
    }
  }

  FreePool(BootDevicePath);

  Status = PetAlert(DeviceType, ModelSerialNumber);

  return Status;
}

VOID
AmtPetAlertReadyToBoot(
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
)
{
  EFI_STATUS                      Status;
  
  if (ManageabilityModeSetting() != 0) {
    DEBUG((EFI_D_ERROR, "<BAE_ReadyToBoot=TRUE>\n"));
    Status = NotifyPetAlert(Event, ParentImageHandle);
  } else {
    DEBUG((EFI_D_ERROR, "<BAE_ReadyToBoot=FALSE>\n"));
  }

  gBS->CloseEvent(Event);
  return;
}

