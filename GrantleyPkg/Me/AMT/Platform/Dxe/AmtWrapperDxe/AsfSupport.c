/*++
 This file contains an 'Intel Peripheral Driver' and is
 licensed for Intel CPUs and chipsets under the terms of your
 license agreement with Intel or your vendor.  This file may
 be modified by the user, subject to additional terms of the
 license agreement
--*/

/*++

Copyright (c) 2005-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  AsfSupport.c

Abstract:

  Support routines for ASF boot options in the BDS

--*/
#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/LegacyDevOrder.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LoadFile.h>
#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolume.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/AcpiS3Save.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/AcpiS3Save.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/GenericBdsLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include "CommonIncludes.h"
#include "AsfSupport.h"
#include "AmtWrapperDxe.h"
#include "Protocol/AlertStandardFormat.h"
#include "Protocol/IderControllerDriverProtocol.h"

#pragma pack(push,1)

typedef struct {
	UINT32 Attributes;
	UINT16 FilePathListLength;
} EFI_LOAD_OPTION;
//
// Legacy Device Order
//
typedef struct{
  UINT8 Type;
  UINT16 Length;
  UINT16 Device[1];
} LEGACY_DEVICE_ORDER;
#pragma pack(pop)

//
// Global variables
//
EFI_ASF_BOOT_OPTIONS            *mAsfBootOptions;

static EFI_DEVICE_PATH_PROTOCOL EndDevicePath[] = {
  END_DEVICE_PATH_TYPE,
  END_ENTIRE_DEVICE_PATH_SUBTYPE,
  END_DEVICE_PATH_LENGTH,
  0
};


EFI_STATUS
BdsAsfInitialization (
  IN  VOID
  )
/*++

Routine Description:

  Retrieve the ASF boot options previously recorded by the ASF driver.

Arguments:

  None.

Returns:

  Initialize Boot Options global variable and AMT protocol

--*/
{
  EFI_STATUS                          Status;
  EFI_ALERT_STANDARD_FORMAT_PROTOCOL  *Asf;

  mAsfBootOptions = NULL;

  //
  // Amt Library Init
  //
  Status = AmtLibInit ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Info : Error init AmtLibInit -> %r\n", Status));
    return Status;
  }
  //
  // Get Protocol for ASF
  //
  Status = gBS->LocateProtocol (
                  &gEfiAlertStandardFormatProtocolGuid,
                  NULL,
                  &Asf
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Info : Error gettings ASF protocol -> %r\n", Status));
    return Status;
  }

  Status = Asf->GetBootOptions (Asf, &mAsfBootOptions);

  return Status;
}

BDS_COMMON_OPTION *
BdsCreateBootOption (
  IN  EFI_DEVICE_PATH_PROTOCOL       *DevicePath,
  IN  CHAR16                         *Description
  )
/*++

Routine Description:

  This function will create a BootOption from the give device path and
  description string.

Arguments:

  DevicePath       - The device path which the option represent
  Description      - The description of the boot option

Returns:

  BDS_COMMON_OPTION - A BDS_COMMON_OPTION pointer

--*/
{
  BDS_COMMON_OPTION *Option;

  Option = AllocateZeroPool (sizeof (BDS_COMMON_OPTION));
  if (Option == NULL) {
    return NULL;
  }

  Option->Signature   = BDS_LOAD_OPTION_SIGNATURE;
  Option->DevicePath  = AllocateZeroPool (GetDevicePathSize (DevicePath));
  CopyMem (Option->DevicePath, DevicePath, GetDevicePathSize (DevicePath));

  Option->Attribute   = LOAD_OPTION_ACTIVE;
  Option->Description = AllocateZeroPool (StrSize (Description));
  CopyMem (Option->Description, Description, StrSize (Description));

  return Option;
}

EFI_DEVICE_PATH_PROTOCOL *
BdsCreateShellDevicePath (
  VOID
  )
/*++

Routine Description:

  This function will create a SHELL BootOption to boot.

Arguments:

  None.

Returns:

  Shell Device path for booting.

--*/
{
  UINTN                             FvHandleCount;
  EFI_HANDLE                        *FvHandleBuffer;
  UINTN                             Index;
  EFI_STATUS                        Status;
  EFI_FIRMWARE_VOLUME_PROTOCOL      *Fv;
  EFI_FV_FILETYPE                   Type;
  UINTN                             Size;
  EFI_FV_FILE_ATTRIBUTES            Attributes;
  UINT32                            AuthenticationStatus;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH ShellNode;

  DevicePath  = NULL;
  Status      = EFI_SUCCESS;

  gBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiFirmwareVolumeProtocolGuid,
        NULL,
        &FvHandleCount,
        &FvHandleBuffer
        );

  for (Index = 0; Index < FvHandleCount; Index++) {
    gBS->HandleProtocol (
          FvHandleBuffer[Index],
          &gEfiFirmwareVolumeProtocolGuid,
          (VOID **) &Fv
          );

    Status = Fv->ReadFile (
                  Fv,
                  PcdGetPtr(PcdShellFile),
                  NULL,
                  &Size,
                  &Type,
                  &Attributes,
                  &AuthenticationStatus
                  );
    if (EFI_ERROR (Status)) {
      //
      // Skip if no shell file in the FV
      //
      continue;
    } else {
      //
      // Found the shell
      //
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    //
    // No shell present
    //
    if (FvHandleCount) {
      FreePool (FvHandleBuffer);
    }
    return NULL;
  }
  //
  // Build the shell boot option
  //
  DevicePath = DevicePathFromHandle (FvHandleBuffer[Index]);

  //
  // Build the shell device path
  //
  ShellNode.Header.Type     = MEDIA_DEVICE_PATH;
  ShellNode.Header.SubType  = MEDIA_PIWG_FW_FILE_DP;
  SetDevicePathNodeLength (&ShellNode.Header, sizeof (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH));
  CopyMem (&ShellNode.FvFileName, PcdGetPtr(PcdShellFile), sizeof (EFI_GUID));
  DevicePath = AppendDevicePathNode (DevicePath, (EFI_DEVICE_PATH_PROTOCOL *) &ShellNode);

  if (FvHandleCount) {
    FreePool (FvHandleBuffer);
  }

  return DevicePath;
}

static
EFI_DEVICE_PATH_PROTOCOL *
BdsCreatePxeDevicePath (
  IN UINT16     DeviceIndex
  )
/*++

Routine Description:

  This function will create a PXE BootOption to boot.

Arguments:

  DeviceIndex - PXE handle index

Returns:

  PXE Device path for booting.

--*/
{
  UINTN                     Index;
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     NumberLoadFileHandles;
  EFI_HANDLE                *LoadFileHandles;
  VOID                      *ProtocolInstance;

  DevicePath  = NULL;
  Status      = EFI_SUCCESS;

  //
  // We want everything connected up for PXE
  //
  BdsLibConnectAllDriversToAllControllers ();

  //
  // Parse Network Boot Device
  //
  gBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiSimpleNetworkProtocolGuid,
        NULL,
        &NumberLoadFileHandles,
        &LoadFileHandles
        );
  for (Index = 0; Index < NumberLoadFileHandles; Index++) {
    Status = gBS->HandleProtocol (
                    LoadFileHandles[Index],
                    &gEfiLoadFileProtocolGuid,
                    (VOID **) &ProtocolInstance
                    );
    if (EFI_ERROR (Status)) {
      //
      // try next handle
      //
      continue;
    } else {
      if (Index == DeviceIndex) {
        //
        // Found a PXE handle
        //
        break;
      } else {
        Status = EFI_UNSUPPORTED;
      }
    }
  }

  if (EFI_ERROR (Status)) {
    //
    // No PXE present
    //
    if (NumberLoadFileHandles) {
      FreePool (LoadFileHandles);
    }
    return NULL;
  }
  //
  // Build the PXE device path
  //
  DevicePath = DevicePathFromHandle (LoadFileHandles[Index]);

  if (NumberLoadFileHandles) {
    FreePool (LoadFileHandles);
  }

  return DevicePath;
}


BOOLEAN 
ComparePathNode(
  IN EFI_DEVICE_PATH_PROTOCOL *PathNode1,
  IN EFI_DEVICE_PATH_PROTOCOL *PathNode2
)
{
  BOOLEAN st = FALSE;
  UINTN Size1, Size2;
  UINT8 *p1, *p2;

  if ((PathNode1 == NULL) || (PathNode2 == NULL)) {
    return FALSE;
  }

  if (PathNode1 == PathNode2) {
    st = TRUE;
  } else {
    Size1 = DevicePathNodeLength(PathNode1);
    Size2 = DevicePathNodeLength(PathNode2);
    p1 = (UINT8 *)PathNode1;
    p2 = (UINT8 *)PathNode2;
    if ((Size1 == Size2)
        && (DevicePathType(PathNode1) == DevicePathType(PathNode2))
        && (CompareMem(p1+1, p2+1, Size1-1) == 0)) {
      st = TRUE;
    }
  }

  return st;
}

BOOLEAN
CompareDevicePaths(
  IN  EFI_DEVICE_PATH_PROTOCOL *BootOptionDP,
  IN  EFI_DEVICE_PATH_PROTOCOL *FileSysDP
)
/*++
      Compare two device paths node by node up to MEDIA_DEVICE_PATH node
    
      @param[in] BootOptionDP     Device path acquired from BootXXXX EFI variable
      @param[in] FileSysDP    Device path acquired through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer
    
      @retval TRUE   Both device paths point to the same device
      @retval FALSE   Device paths point to different devices
--*/
{
  EFI_DEVICE_PATH_PROTOCOL     *DevPathNodeA;
  EFI_DEVICE_PATH_PROTOCOL     *DevPathNodeB;

  if (BootOptionDP == NULL || FileSysDP == NULL) {
    return FALSE;
  }

  DevPathNodeA = BdsLibUnpackDevicePath(BootOptionDP);
  if (DevPathNodeA == NULL) {
    return FALSE;
  }

  DevPathNodeB = BdsLibUnpackDevicePath(FileSysDP);
  if (DevPathNodeB == NULL) {
    return FALSE;
  }

  while (!IsDevicePathEnd(DevPathNodeB)) {
    if (DevicePathType(DevPathNodeB) == MEDIA_DEVICE_PATH) {
      //
      // If we have reached MEDIA_DEVICE_PATH node and all previous
      // nodes matched - we can be sure path points to the same device
      //
      return TRUE;
    }

    if (!ComparePathNode(DevPathNodeA, DevPathNodeB)) {
      break;
    }

    DevPathNodeA = NextDevicePathNode(DevPathNodeA);
    DevPathNodeB = NextDevicePathNode(DevPathNodeB);
  }

  return FALSE;
}

EFI_DEVICE_PATH_PROTOCOL *
GetFullBootDevicePath(
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
)
/*++
  Get EFI device path through EFI_SIMPLE_FILE_SYSTEM_PROTOCOL Handles buffer. Acquired path must
  point to the same device as argument DevicePath passed to the function.

  @param[in] DevicePath   Device path acquired from BootXXXX EFI variable

  @retval EFI_DEVICE_PATH_PROTOCOL   Device path for booting
--*/
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *DPath;
  EFI_DEVICE_PATH_PROTOCOL *DevPath;
  UINTN                    HandleNum;
  EFI_HANDLE               *HandleBuf;
  UINTN                    Index;

  DevPath = NULL;

  if (DevicePath == NULL) {
    return NULL;
  }
  
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
    Status = gBS->HandleProtocol (
                    HandleBuf[Index],
                    &gEfiDevicePathProtocolGuid,
                    &DPath
                    );

    if (CompareDevicePaths(DevicePath, DPath)) {
      DevPath = DuplicateDevicePath(DPath);
      break;
    }
  }

  return DevPath;
}


UINTN
GetBootDeviceType (
  IN UINTN    DeviceType, 
  IN BOOLEAN  Efi
  )
/*++
  Translate ASF request type to BBS or EFI device path type

  @param[in] DeviceType     - ASF request type
  @param[in]  Efi           - Set to TRUE if DeviceType is to be translated
                              to EFI device path type; FALSE if BBS type
  @retval UINTN Translated device type
--*/
{
  UINTN Type = 0;
  
  switch (DeviceType) {
    case FORCE_PXE:
      if (Efi) { 
        Type = MEDIA_FILEPATH_DP; 
      } else { 
        Type = BBS_EMBED_NETWORK; 
      }
      break;
    case FORCE_HARDDRIVE:      
    case FORCE_SAFEMODE:
      if (Efi) { 
        Type = MEDIA_HARDDRIVE_DP; 
      } else { 
        Type = BBS_TYPE_HARDDRIVE; 
      }
      break;
    case FORCE_DIAGNOSTICS:
      if (Efi) { 
        Type = MEDIA_FILEPATH_DP; 
      }
      break;
    case FORCE_CDDVD:
      if (Efi) { 
        Type = MEDIA_CDROM_DP; 
      } else { 
        Type = BBS_TYPE_CDROM; 
      }
      break;
    default:
      break;
  }

  return Type;
}


EFI_STATUS
RefreshBbsTableForBoot (
  IN     UINT16        DeviceIndex,
  IN     UINT16        DevType,
  IN     BOOLEAN       IderBoot
  )
/*++
      Update the BBS table with our required boot device
    
      @param[in] DeviceIndex   Boot device whose device index
      @param[in] DevType     Boot device whose device type
      @param[in] BbsCount    Number of BBS_TABLE structures
      @param[in] BbsTable    BBS entry
      @param[in] IderBoot    set to TRUE if this is IDER boot
    
      @retval EFI_SUCCESS   BBS table successfully updated
--*/
{
  EFI_STATUS                Status;
  UINTN                     Index;
  UINT16                    TempIndex;
  BOOLEAN                   IderBootDevice;
  BOOLEAN                   RegularBootDevice;
  HDD_INFO                  *LocalHddInfo;
  EFI_LEGACY_BIOS_PROTOCOL  *LegacyBios;
  BBS_TABLE                 *BbsTable;
  UINT16                    HddCount;
  UINT16                    BbsCount;
  
  TempIndex       = (IderBoot) ? 0 : ((DeviceIndex <= 1) ? DeviceIndex : 1);

  //
  // Make sure the Legacy Boot Protocol is available
  //
  LegacyBios = NULL;
  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if (EFI_ERROR (Status) || (LegacyBios == NULL)) {
    return EFI_ABORTED;
  }

  //
  // Get BBS table instance
  //
  Status = LegacyBios->GetBbsInfo (
                        LegacyBios,
                        &HddCount,
                        &LocalHddInfo,
                        &BbsCount,
                        &BbsTable
                        );
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  Status = EFI_NOT_FOUND;
  
  //
  // For debug
  //
  PrintBbsTable (BbsTable);

  //
  // Find the first present boot device whose device type
  // matches the DevType, we use it to boot first. This is different
  // from the other Bbs table refresh since we are looking for the device type
  // index instead of the first device to match the device type.
  //
  // And set other present boot devices' priority to BBS_UNPRIORITIZED_ENTRY
  // their priority will be set by LegacyBiosPlatform protocol by default
  //
  for (Index = 0; Index < BbsCount; Index++) {
    if (BbsTable[Index].BootPriority == BBS_IGNORE_ENTRY) {
      continue;
    }
    BbsTable[Index].BootPriority = BBS_DO_NOT_BOOT_FROM;
    IderBootDevice = IderBoot && IS_IDER(BbsTable[Index].Bus, BbsTable[Index].Device, BbsTable[Index].Function) &&
    BbsTable[Index].DeviceType == DevType;
    RegularBootDevice = !IderBoot && (BbsTable[Index].DeviceType == DevType ||
      (DevType == BBS_EMBED_NETWORK && IS_PXE(BbsTable[Index].DeviceType, BbsTable[Index].Class)) ||
      (DevType == BBS_TYPE_CDROM && IS_CDROM(BbsTable[Index].DeviceType, BbsTable[Index].Class)));

    if ((IderBootDevice || RegularBootDevice) && Status != EFI_SUCCESS) {
      if (IderBoot || (TempIndex++ == DeviceIndex)) {
        BbsTable[Index].BootPriority  = 0;
        Status                        = EFI_SUCCESS;
        continue;
      }
    }
  }
  //
  // For debug
  //
  PrintBbsTable (BbsTable);

  return Status;
}

UINTN
EfiStrLen (
   IN CHAR16   *String  
 )
/*++

Routine Description:  
 Return the number of Unicode characters in String. This is not the same as
 the length of the string in bytes.

 Arguments:
   String - String to processReturns:  Number of Unicode characters in String--*/
{
  UINTN Length;
  for (Length=0; *String; String++, Length++);  return Length;
}


EFI_DEVICE_PATH_PROTOCOL *
BdsCreateBootDevicePath (
  IN UINT16     DeviceType,
  IN UINT16     DeviceIndex,
  IN BOOLEAN    IdeRBoot,
  IN BOOLEAN    EfiBoot
  )
{
  EFI_DEVICE_PATH_PROTOCOL     *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL     *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL     *FullDevicePath;
  UINTN                        OptionOrderSize;
  UINT16                       *OptionOrder;
  EFI_LOAD_OPTION              *Option;
  CHAR16                       OptionName[sizeof ("Driver####")];
  UINT16                       OptionNumber;
  UINTN                        OptionIndex;
  UINTN                        OptionCount;
  UINTN                        Index;
  UINTN                        OptionSize;
  UINTN                        TempIndex;
  EFI_DEVICE_PATH_PROTOCOL     *DevPathNode;
//AptioV server override EFI_DEVICE_PATH_PROTOCOL     *DevPathNodeBackup;
  ATAPI_DEVICE_PATH            *AtaPath;
  BOOLEAN                      AtaDeviceMatch;
  PCI_DEVICE_PATH              *PciPath;
  BOOLEAN                      PciDeviceMatch;
  UINT8                        PrimarySecondary;
  UINT8                        SlaveMaster;
  UINTN                        EfiDeviceType;
  UINTN                        LegacyDeviceType;
  BOOLEAN                      TypeMatched;
  UINTN                        EfiNodeType;

  PrimarySecondary = ((mAsfBootOptions->SpecialCommandParam >> IDER_BOOT_DEVICE_SHIFT) & IDER_PRIMARY_SECONDARY_MASK)
                        >> IDER_PRIMARY_SECONDARY_SHIFT;
  SlaveMaster      = (mAsfBootOptions->SpecialCommandParam >> IDER_BOOT_DEVICE_SHIFT) & IDER_MASTER_SLAVE_MASK;
  DevicePath       = NULL;
  FullDevicePath   = NULL;
  TempIndex        = 1;
  AtaDeviceMatch   = FALSE;
  PciDeviceMatch   = FALSE;
  EfiDeviceType    = GetBootDeviceType(DeviceType, TRUE);
  LegacyDeviceType = GetBootDeviceType(DeviceType, FALSE);
  TypeMatched      = FALSE;

  EfiNodeType      = (EfiDeviceType == MSG_MAC_ADDR_DP) ? MESSAGING_DEVICE_PATH : MEDIA_DEVICE_PATH;

  if (IdeRBoot && !EfiBoot) {
    LegacyDeviceType = (SlaveMaster == 1) ? BBS_CDROM : BBS_FLOPPY; //AptioV server override 
  }
  
  //
  // Read the BootOrder variable.
  //
  OptionOrder = BdsLibGetVariableAndSize (L"BootOrder", &gEfiGlobalVariableGuid, &OptionOrderSize);
  if (OptionOrder == NULL) {
    return NULL;
  }

  OptionCount = OptionOrderSize/sizeof(UINT16);
  OptionIndex = 0;

  for (Index = 0; Index < OptionCount; Index++) {

    OptionNumber = OptionOrder[Index];
    UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", OptionNumber);
    Option = BdsLibGetVariableAndSize (OptionName, &gEfiGlobalVariableGuid, &OptionSize);
    if (Option == NULL) {
      continue;
    }

    //
    // Extract device path from the boot order entry
    //
    TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL*)
        (   //skip the header
          (UINT8*)(Option+1)
          //skip the string
          +(EfiStrLen((CHAR16*)(Option+1))+1)*sizeof(CHAR16)
            );

    if (DevicePathType(TempDevicePath) == BBS_DEVICE_PATH && DevicePathSubType(TempDevicePath) == BBS_BBS_DP) {
      FullDevicePath = DuplicateDevicePath(TempDevicePath);
    } else {
      //
      // If this is EFI boot option, we need to get full device path from EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
      // to determine type of device and provide LoadImage with proper path to bootloader image later on
      //
      FullDevicePath = GetFullBootDevicePath(TempDevicePath);
      if (FullDevicePath == NULL) {
        continue;
      }
    }

    TempDevicePath = FullDevicePath;
    DevPathNode = TempDevicePath; //AptioV server override 

    /* AptioV server override Don't use intel code.
	DevPathNode = BdsLibUnpackDevicePath(TempDevicePath);    
    if (DevPathNode == NULL) {
      continue;
    }

    DevPathNodeBackup = DevPathNode;
	AptioV server override  Don't use intel code*/

    //
    // Check if this is our requested boot device
    //
    while (!IsDevicePathEnd(DevPathNode)) {
      if (IdeRBoot && EfiBoot) {
        //
        // IDER EFI boot, check for PCI/ATA device match
        //
        if ((DevicePathType(DevPathNode) == HARDWARE_DEVICE_PATH) &&
          (DevicePathSubType(DevPathNode) == HW_PCI_DP)) {
           PciPath = (PCI_DEVICE_PATH*) DevPathNode;

           if ((PciPath->Device == IDER_DEVICE_NUMBER)
             && (PciPath->Function == IDER_FUNCTION_NUMBER)) {
             PciDeviceMatch = TRUE;
           }
        } else if ((DevicePathType(DevPathNode) == MESSAGING_DEVICE_PATH) &&
          (DevicePathSubType(DevPathNode) == MSG_ATAPI_DP)) {
           AtaPath = (ATAPI_DEVICE_PATH*) DevPathNode;

           if ((AtaPath->PrimarySecondary == PrimarySecondary)
            && (AtaPath->SlaveMaster == SlaveMaster)) {
             AtaDeviceMatch = TRUE;
           }
        }

        if (PciDeviceMatch && AtaDeviceMatch) {
          TypeMatched = TRUE;
        } 
      } else {
        if (DevicePathType(DevPathNode) == BBS_DEVICE_PATH && DevicePathSubType(DevPathNode) == BBS_BBS_DP) {
          //
          // Legacy boot option
          //
          if (((BBS_BBS_DEVICE_PATH *)DevPathNode)->DeviceType == LegacyDeviceType) {
            TypeMatched = TRUE;
          }
        } else {
          //
          // EFI boot option
          //
              if (DevicePathType(DevPathNode) == EfiNodeType && DevicePathSubType(DevPathNode) == EfiDeviceType) {            
                if (DeviceType == FORCE_DIAGNOSTICS) {
                  //
                  // If boot to EFI shell, find shell file by GUID
                  //
                  if (CompareMem(&((MEDIA_FW_VOL_FILEPATH_DEVICE_PATH*)DevPathNode)->FvFileName, PcdGetPtr(PcdShellFile), sizeof(EFI_GUID))) {
                    TypeMatched = TRUE;
                  }
                } else {
                  TypeMatched = TRUE;
                }
              }
        }
      }

      if (TypeMatched) {
        //
        // Type matched, check for device index
        //
        if (!IdeRBoot && TempIndex < DeviceIndex) {
          TempIndex++;
          TypeMatched = FALSE;
          break;
        }

        DevicePath = DuplicateDevicePath(TempDevicePath);
        //
        // Refresh BBS table if legacy option
        //
        if (DevicePathType(DevicePath) == BBS_DEVICE_PATH && DevicePathSubType(DevicePath) == BBS_BBS_DP) {
          RefreshBbsTableForBoot(DeviceIndex, (UINT16)LegacyDeviceType, IdeRBoot);
        }
        break;
      }

      DevPathNode = NextDevicePathNode(DevPathNode);
    }

    if (FullDevicePath != NULL) {
      FreePool(FullDevicePath);
      FullDevicePath = NULL;
    }

    //AptioV server override Don't use intel code. FreePool(DevPathNodeBackup);
    FreePool(Option);

    if (DevicePath != NULL) {
      //
      // Set Boot Current and leave
      //
      gRT->SetVariable (
          L"BootCurrent",
          &gEfiGlobalVariableGuid,
          EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
          sizeof (UINT16),
          &OptionNumber
          );
      break;
    }
  }

  FreePool(OptionOrder);
  
  return DevicePath;
}

EFI_STATUS
AsfDoLegacyBoot (
  IN  BDS_COMMON_OPTION           *Option
  )
/*++

Routine Description:

  Boot the legacy system with the boot option

Arguments:

  Option           - The legacy boot option which have BBS device path

Returns:

  EFI_UNSUPPORTED  - There is no legacybios protocol, do not support
                     legacy boot.

  EFI_STATUS       - Return the status of LegacyBios->LegacyBoot ().

--*/
{
  EFI_STATUS                Status;
  EFI_LEGACY_BIOS_PROTOCOL  *LegacyBios;

  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if (EFI_ERROR (Status)) {
    //
    // If no LegacyBios protocol we do not support legacy boot
    //
    return EFI_UNSUPPORTED;
  }
  //
  // Write boot to OS performance data to a file
  //
  WRITE_BOOT_TO_OS_PERFORMANCE_DATA;

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Legacy Boot: %S\n", Option->Description));
  return LegacyBios->LegacyBoot (
                      LegacyBios,
                      (BBS_BBS_DEVICE_PATH *) Option->DevicePath,
                      Option->LoadOptionsSize,
                      Option->LoadOptions
                      );
}

BOOLEAN CompareDP(
	EFI_DEVICE_PATH_PROTOCOL *dp1,
	EFI_DEVICE_PATH_PROTOCOL *dp2
	)
{
	UINTN s1,s2;
	BOOLEAN st = FALSE;

	s1 = GetDevicePathSize(dp1);

	s2 = GetDevicePathSize(dp2);

	if (s1 == s2) {
		if (CompareMem(dp1, dp2, s1) == 0)
			st = TRUE;
	}

	return st;
}


UINT16 GetBootCurrent(
  IN  EFI_DEVICE_PATH_PROTOCOL      *DevicePath
)
{
  UINT16 *BootOrder;
  EFI_LOAD_OPTION *BootOption;
  UINTN BootOrderSize; //size of BootOrder Variable
  UINTN BootOptionSize;
  CHAR16 BootVarName[9];
  UINTN i;
  EFI_STATUS Status;
  BOOLEAN st;
  UINT16 BootCurrent;
  EFI_DEVICE_PATH_PROTOCOL *Dp;

  BootOrder = NULL;
  BootOption = NULL;
  BootCurrent = 0;
  BootOrderSize = 0;

  Status = gRT->GetVariable(
              L"BootOrder",
              &gEfiGlobalVariableGuid,
              NULL,
              &BootOrderSize,
              NULL
            );

  if (Status == EFI_BUFFER_TOO_SMALL) {
      BootOrder = AllocateZeroPool(BootOrderSize);
      if (BootOrder != NULL){
          Status = gRT->GetVariable(
                  L"BootOrder",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &BootOrderSize,
                  BootOrder
                  );

          if (!EFI_ERROR(Status)) {
              for (i=0; i<BootOrderSize/sizeof(UINT16); i++){
                  UnicodeSPrint (BootVarName, sizeof(BootVarName), L"Boot%04d", BootOrder[i] );
                  BootOptionSize = 0;
                  Status = gRT->GetVariable(
                          BootVarName,
                          &gEfiGlobalVariableGuid,
                          NULL,
                          &BootOptionSize,
                          NULL
                        );
                   if (Status == EFI_BUFFER_TOO_SMALL) {
                       BootOption = AllocateZeroPool(BootOptionSize);
                       if (BootOption != NULL){
                       Status = gRT->GetVariable(
                                        BootVarName,
                                        &gEfiGlobalVariableGuid,
                                        NULL,
                                        &BootOptionSize,
                                        BootOption
                                      );
                            if (!EFI_ERROR(Status)) {
                                Dp = (EFI_DEVICE_PATH_PROTOCOL*)
                                    (   //skip the header
                                      (UINT8*)(BootOption+1)
                                      //skip the string
                                      +(StrLen((CHAR16*)(BootOption+1))+1)*sizeof(CHAR16)
                                        );
                                st = CompareDP(Dp, DevicePath);
                                if (st == TRUE) {
                                    BootCurrent = BootOrder[i];
                                }
                            }
                        FreePool(BootOption);
                      }
                  }
              }
          }
          FreePool(BootOrder);
      }
  }
  return BootCurrent;
}

EFI_STATUS
AsfBootViaBootOption (
  IN  BDS_COMMON_OPTION             * Option,
  IN  EFI_DEVICE_PATH_PROTOCOL      * DevicePath,
  OUT UINTN                         *ExitDataSize,
  OUT CHAR16                        **ExitData OPTIONAL
  )
/*++

Routine Description:

  Process the boot option follow the EFI 1.1 specification and
  special treat the legacy boot option with BBS_DEVICE_PATH.

Arguments:

  Option       - The boot option need to be processed
  DevicePath   - The device path which describe where to load
                 the boot image or the legcy BBS device path
                 to boot the legacy OS
  ExitDataSize - Returned directly from gBS->StartImage ()
  ExitData     - Returned directly from gBS->StartImage ()

Returns:

  EFI_SUCCESS   - Status from gBS->StartImage (),
                  or BdsBootByDiskSignatureAndPartition ()
  EFI_NOT_FOUND - If the Device Path is not found in the system

--*/
{
  EFI_STATUS                Status;
  EFI_HANDLE                Handle;
  EFI_HANDLE                ImageHandle;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *FilePath;
  EFI_LOADED_IMAGE_PROTOCOL *ImageInfo;
  EFI_EVENT                 ReadyToBootEvent;
  EFI_ACPI_S3_SAVE_PROTOCOL *AcpiS3Save;

  *ExitDataSize = 0;
  *ExitData     = NULL;

  //
  // Notes: put EFI64 ROM Shadow Solution
  //
  EFI64_SHADOW_ALL_LEGACY_ROM ();

  //
  // Notes: this code can be remove after the s3 script table
  // hook on the event EFI_EVENT_SIGNAL_READY_TO_BOOT or
  // EFI_EVENT_SIGNAL_LEGACY_BOOT
  //
  Status = gBS->LocateProtocol (&gEfiAcpiS3SaveProtocolGuid, NULL, &AcpiS3Save);
  if (!EFI_ERROR (Status)) {
    AcpiS3Save->S3Save (AcpiS3Save, NULL);
  }
  //
  // If it's Device Path that starts with a hard drive path,
  // this routine will do the booting.
  //
  Status = BdsBootByDiskSignatureAndPartition (
            Option,
            (HARDDRIVE_DEVICE_PATH *) DevicePath,
            Option->LoadOptionsSize,
            Option->LoadOptions,
            ExitDataSize,
            ExitData
            );
  if (!EFI_ERROR (Status)) {
    //
    // If we found a disk signature and partition device path return success
    //
    return EFI_SUCCESS;
  }
  //
  // Signal the EFI_EVENT_SIGNAL_READY_TO_BOOT event
  //


  Option->BootCurrent = GetBootCurrent(DevicePath);

    //
    // Set Boot Current
    //
    gRT->SetVariable (
        L"BootCurrent",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        sizeof (UINT16),
        &Option->BootCurrent
        );

  DEBUG ((EFI_D_ERROR, "boot current = %d device path = %s\n", Option->BootCurrent, DevicePathToStr(DevicePath)));

  //
  // Abstract: Modified for EFI 2.0 required
  //
  Status = EfiCreateEventReadyToBoot (&ReadyToBootEvent);
  if (!EFI_ERROR (Status)) {
    gBS->SignalEvent (ReadyToBootEvent);
    gBS->CloseEvent (ReadyToBootEvent);
  }

  if ((DevicePathType (Option->DevicePath) == BBS_DEVICE_PATH) &&
      (DevicePathSubType (Option->DevicePath) == BBS_BBS_DP)
      ) {
    //
    // Check to see if we should legacy BOOT. If yes then do the legacy boot
    //
    return AsfDoLegacyBoot (Option);
  }
  //
  // Drop the TPL level from TPL_CALLBACK to TPL_APPLICATION
  //
  gBS->RestoreTPL (TPL_APPLICATION);

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Booting EFI 1.1 way %S\n", Option->Description));

  Status = gBS->LoadImage (
                  TRUE,
                  mBdsImageHandle,
                  DevicePath,
                  NULL,
                  0,
                  &ImageHandle
                  );

  //
  // If we didn't find an image, we may need to load the default
  // boot behavior for the device.
  //
  if (EFI_ERROR (Status)) {
    //
    // Find a Simple File System protocol on the device path. If the remaining
    // device path is set to end then no Files are being specified, so try
    // the removable media file name.
    //
    TempDevicePath = DevicePath;
    Status = gBS->LocateDevicePath (
                    &gEfiSimpleFileSystemProtocolGuid,
                    &TempDevicePath,
                    &Handle
                    );
    if (!EFI_ERROR (Status) && IsDevicePathEnd (TempDevicePath)) {
      FilePath = FileDevicePath (Handle, DEFAULT_REMOVABLE_FILE_NAME);
      if (FilePath) {
        Status = gBS->LoadImage (
                        TRUE,
                        mBdsImageHandle,
                        FilePath,
                        NULL,
                        0,
                        &ImageHandle
                        );
        if (EFI_ERROR (Status)) {
          //
          // The DevicePath failed, and it's not a valid
          // removable media device.
          //
          goto Done;
        }
      }
    } else {
      Status = EFI_NOT_FOUND;
    }
  }

  if (EFI_ERROR (Status)) {
    //
    // It there is any error from the Boot attempt exit now.
    //
    goto Done;
  }
  //
  // Provide the image with it's load options
  //
  Status = gBS->HandleProtocol (ImageHandle, &gEfiLoadedImageProtocolGuid, &ImageInfo);
  ASSERT_EFI_ERROR (Status);

  if (Option->LoadOptionsSize != 0) {
    ImageInfo->LoadOptionsSize  = Option->LoadOptionsSize;
    ImageInfo->LoadOptions      = Option->LoadOptions;
  }
  //
  // Before calling the image, enable the Watchdog Timer for
  // the 5 Minute period
  //
  gBS->SetWatchdogTimer (5 * 60, 0x0000, 0x00, NULL);

  Status = gBS->StartImage (ImageHandle, ExitDataSize, ExitData);
  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Image Return Status = %r\n", Status));

  //
  // Clear the Watchdog Timer after the image returns
  //
  gBS->SetWatchdogTimer (0x0000, 0x0000, 0x0000, NULL);

Done:
  //
  // Clear Boot Current
  //
  gRT->SetVariable (
        L"BootCurrent",
        &gEfiGlobalVariableGuid,
        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
        0,
        &Option->BootCurrent
        );

  //
  // Raise the TPL level back to TPL_CALLBACK
  //
  gBS->RaiseTPL (TPL_CALLBACK);

  return Status;
}

EFI_DEVICE_PATH_PROTOCOL *
BdsAsfBoot (
  IN   BOOLEAN                         EfiBoot
  )
/*++
  Found out ASF boot options.

  @param[in] EfiBoot      Set to TRUE if this is EFI boot

  @retval EFI_DEVICE_PATH_PROTOCOL     Device path for booting.
--*/
{
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;

  DevicePath = NULL;

  //
  // First we check ASF boot options Special Command
  //
  switch (mAsfBootOptions->SpecialCommand) {
  //
  // No additional special command is included; the Special Command Parameter has no
  // meaning.
  //
  case NOP:
    break;

  //
  // The Special Command Parameter can be used to specify a PXE
  // parameter. When the parameter value is 0, the system default PXE device is booted. All
  // other values for the PXE parameter are reserved for future definition by this specification.
  //
  case FORCE_PXE:
    if (mAsfBootOptions->SpecialCommandParam != 0) {
      //
      // ASF spec says 0 currently only option
      //
      break;
    }

    if (EfiBoot == TRUE) {
      DevicePath = BdsCreatePxeDevicePath (mAsfBootOptions->SpecialCommandParam);
    } else {
      DevicePath = BdsCreateBootDevicePath (FORCE_PXE, mAsfBootOptions->SpecialCommandParam, FALSE, EfiBoot);
    }
    break;

  //
  // The Special Command Parameter identifies the boot-media index for
  // the managed client. When the parameter value is 0, the default hard-drive is booted, when the
  // parameter value is 1, the primary hard-drive is booted; when the value is 2, the secondary
  // hard-drive is booted and so on.
  //
  case FORCE_HARDDRIVE:
  //
  // The Special Command Parameter identifies the boot-media
  // index for the managed client. When the parameter value is 0, the default hard-drive is
  // booted, when the parameter value is 1, the primary hard-drive is booted; when the value is 2,
  // the secondary hard-drive is booted and so on.
  //
  case FORCE_SAFEMODE:
    DevicePath = BdsCreateBootDevicePath(FORCE_HARDDRIVE, mAsfBootOptions->SpecialCommandParam, FALSE, EfiBoot);    
    break;

  //
  // The Special Command Parameter can be used to specify a
  // diagnostic parameter. When the parameter value is 0, the default diagnostic media is booted.
  // All other values for the diagnostic parameter are reserved for future definition by this
  // specification.
  //
  case FORCE_DIAGNOSTICS:
    if (mAsfBootOptions->SpecialCommandParam != 0) {
      //
      // ASF spec says 0 currently only option
      //
      break;
    }

    DevicePath = BdsCreateShellDevicePath ();

    //
    // We want everything connected up for shell
    //
    BdsLibConnectAllDriversToAllControllers ();
    break;

  //
  // The Special Command Parameter identifies the boot-media index for
  // the managed client. When the parameter value is 0, the default CD/DVD is booted, when the
  // parameter value is 1, the primary CD/DVD is booted; when the value is 2, the secondary
  // CD/DVD is booted and so on.
  //
  case FORCE_CDDVD:
    DevicePath = BdsCreateBootDevicePath (FORCE_CDDVD, mAsfBootOptions->SpecialCommandParam, FALSE, EfiBoot);
    break;

  default:
    break;;
  }

  return DevicePath;
}


EFI_DEVICE_PATH_PROTOCOL *
BdsForcedBoot (
  IN   BOOLEAN                         EfiBoot
  )
/*++
      Check IdeR boot device and Asf boot device
    
      @param[in] EfiBoot      Set to TRUE if this is EFI boot
    
      @retval EFI_DEVICE_PATH_PROTOCOL     Device path for booting.
--*/
{
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  DevicePath = NULL;

  //
  // OEM command values; the interpretation of the Special Command and associated Special
  // Command Parameters is defined by the entity associated with the Enterprise ID.
  //
  if (ActiveManagementEnableIdeR ()) {
    //
    // Check if any media exist in Ider device
    //
    if (BdsCheckIderMedia ()) {
      DevicePath = BdsCreateBootDevicePath (
                      FORCE_CDDVD,
                      0,
                      TRUE,
                      EfiBoot
                      );
    }
  } else if (mAsfBootOptions->IanaId != ASF_INDUSTRY_CONVERTED_IANA) {
    DevicePath = BdsAsfBoot (EfiBoot);
  }

  return DevicePath;
}


EFI_STATUS
BdsBootViaAsf (
  IN  VOID
  )
/*++
  Process ASF boot options and if available, attempt the boot

  @param[in] None.

  @retval EFI_SUCCESS    The command completed successfully
--*/
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  BDS_COMMON_OPTION         *BootOption;
  UINTN                     ExitDataSize;
  CHAR16                    *ExitData;
  BOOLEAN                   EfiBoot;
  EFI_LEGACY_BIOS_PROTOCOL  *LegacyBios;

  Status      = EFI_SUCCESS;
  DevicePath  = NULL;
  EfiBoot     = FALSE;
  LegacyBios  = NULL;

  //
  // Check if this is legacy or efi boot
  //
  Status = gBS->LocateProtocol (&gEfiLegacyBiosProtocolGuid, NULL, &LegacyBios);
  if ((EFI_ERROR (Status)) || (LegacyBios == NULL)) {
    EfiBoot = TRUE;
  }

  //
  // Check if ASF Boot Options is present.
  //
  if (mAsfBootOptions->SubCommand != ASF_BOOT_OPTIONS_PRESENT) {
    return EFI_NOT_FOUND;
  }

  DevicePath = BdsForcedBoot (EfiBoot);
  //
  // If device path was set, the we have a boot option to use
  //
  if (DevicePath == NULL) {
    return EFI_UNSUPPORTED;
  }

  BootOption = BdsCreateBootOption (DevicePath, L"ASF Boot");
  if (BootOption == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = AsfBootViaBootOption (BootOption, BootOption->DevicePath, &ExitDataSize, &ExitData);

  FreePool (BootOption);
  FreePool (DevicePath);

  return Status;
}


BOOLEAN
BdsCheckIderMedia (
  IN  VOID
  )
/*++

Routine Description:
  This will return if Media in IDE-R is present.

Arguments:
  None.

Returns:
  True    Media is present.
  False   Media is not present.

--*/
{
  UINTN                     HandleNum;
  EFI_HANDLE                *HandleBuf;
  EFI_HANDLE                Handle;
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DPath;
  UINTN                     Index;
  UINTN                     EventIndex;
  EFI_INPUT_KEY             Key;
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;
  EFI_DISK_INFO_PROTOCOL    *DiskInfo;
  EFI_BLOCK_IO_MEDIA        *BlkMedia;
  VOID                      *Buffer;
  UINT8                     IdeBootDevice;
  UINT32                    IdeChannel;
  UINT32                    IdeDevice;

  IdeBootDevice = ActiveManagementIderBootDeviceGet ();

  DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Ide Channel Device Index = %d\n", IdeBootDevice));

  //
  // Make sure the Legacy Boot Protocol is available
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  &HandleNum,
                  &HandleBuf
                  );
  if ((EFI_ERROR (Status)) || (HandleBuf == NULL)) {
    return FALSE;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (
                    HandleBuf[Index],
                    &gEfiDevicePathProtocolGuid,
                    &DPath
                    );
    if (!EFI_ERROR (Status)) {
      Status = gBS->LocateDevicePath (
                      &gEfiIderControllerDriverProtocolGuid,
                      &DPath,
                      &Handle
                      );
      if (!EFI_ERROR (Status)) {
        BlkIo = NULL;
        Status = gBS->HandleProtocol (
                        HandleBuf[Index],
                        &gEfiBlockIoProtocolGuid,
                        &BlkIo
                        );
        if ((EFI_ERROR (Status)) || (BlkIo == NULL)) {
		  continue;
		}

		DiskInfo = NULL;
        Status = gBS->HandleProtocol (
                        HandleBuf[Index],
                        &gEfiDiskInfoProtocolGuid,
                        &DiskInfo
                        );
        if ((EFI_ERROR (Status)) || (DiskInfo == NULL)) {
		  continue;
		}

        DiskInfo->WhichIde (DiskInfo, &IdeChannel, &IdeDevice);

        if (IdeBootDevice != (UINT8) (IdeChannel * 2 + IdeDevice)) {
          continue;
        }

        if (BlkIo->Media->MediaPresent) {
          if (HandleBuf != NULL) {
            FreePool (HandleBuf);
          }
          return TRUE;
        } else {
          while (TRUE) {
            BlkMedia  = BlkIo->Media;
            Buffer    = AllocatePool (BlkMedia->BlockSize);
            if (Buffer) {
              BlkIo->ReadBlocks (
                      BlkIo,
                      BlkMedia->MediaId,
                      0,
                      BlkMedia->BlockSize,
                      Buffer
                      );
              FreePool (Buffer);
            }

            if (BlkMedia->MediaPresent) {
              if (HandleBuf != NULL) {
                FreePool (HandleBuf);
              }
              return TRUE;
            }

            gST->ConOut->OutputString (
                          gST->ConOut,
                          L"Boot disk missing, please insert boot disk and press ENTER\r\n"
                          );
            Key.ScanCode    = 0;
            Key.UnicodeChar = 0;
            gBS->RestoreTPL (TPL_APPLICATION);
            while (!(Key.ScanCode == 0 && Key.UnicodeChar == L'\r')) {
              Status = gBS->WaitForEvent (1, &(gST->ConIn->WaitForKey), &EventIndex);
              gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);
            }

            gBS->RaiseTPL (TPL_CALLBACK);
          }

          if (HandleBuf != NULL) {
            FreePool (HandleBuf);
          }
          return FALSE;
        }
      }
    }
  }

  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }
  return FALSE;
}
