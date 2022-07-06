//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  AmiImageTypeLib instance.
  It implements AmiGetImageType by analyzing image device path.
**/

#include <Protocol/DevicePath.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include <Protocol/BlockIo2.h>
#include <Protocol/LoadFile.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/AmiImageTypeLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiRomLayoutLib.h>
#include <Library/HobLib.h>
#include <Library/AmiItemListLib.h>

#define AMI_MEDIA_DEVICE_PATH_GUID \
    { 0x5023b95c, 0xdb26, 0x429b, { 0xa6, 0x48, 0xbd, 0x47, 0x66, 0x4c, 0x80, 0x12 } }

typedef UINT32 (*DEVICE_PATH_HANDLER)(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN EFI_DEVICE_PATH_PROTOCOL *Node
);

typedef struct {
    UINT8 Type;
    UINT8 SubType;
    DEVICE_PATH_HANDLER Handler;
} DEVICE_PATH_DESCRIPTOR;

static EFI_GUID AmiMediaDevicePathGuid = AMI_MEDIA_DEVICE_PATH_GUID;
static AMI_ITEM_LIST FvHandles = AMI_ITEM_LIST_VARIABLE_INITIALIZE();
static VOID* FwVolumeRegistration = NULL;
static EFI_EVENT FwVolumeEvent = NULL;
static EFI_EVENT SmmReadyToLockEvent = NULL;

/**
  Locate Protocol associated with the given device path.

  The function calls LocateDevicePath to find the closest handle that supports the protocol
  and then calls HandleProtocol to get protocol interface.

 @param[in,out]  DevicePath    In input, device path to match.
                               On output the pointer is updated by the LocateDevicePath with the address
                               of the first unmatched node in the original device path.
 @param[in]      ProtocolGuid  GUID of the protocol to locate
 @param[out]     HandlePtr     Handle on which ProtocolGuid is installed.

 @return         Interface Pointer
**/
VOID* AmiImageTypeLibLocateInterface(
    EFI_DEVICE_PATH_PROTOCOL **DevicePath, EFI_GUID *ProtocolGuid, EFI_HANDLE *HandlePtr
){
    VOID *Interface;
    EFI_HANDLE Handle;
    EFI_DEVICE_PATH_PROTOCOL *TmpDevicePath = *DevicePath;

    if (EFI_ERROR(gBS->LocateDevicePath(ProtocolGuid, &TmpDevicePath, &Handle)))
        return NULL;
    if (EFI_ERROR(gBS->HandleProtocol(Handle, ProtocolGuid, &Interface)))
        return NULL;
    *DevicePath = TmpDevicePath;
    if (HandlePtr != NULL) *HandlePtr = Handle;
    return Interface;
}

/// The function handles FV Device Path
UINT32 AmiImageTypeLibFvDevicePathHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    EFI_HANDLE Handle;

    // Make sure FV protocol exists.
    if (AmiImageTypeLibLocateInterface(&DevicePath, &gEfiFirmwareVolume2ProtocolGuid, &Handle)==NULL)
        return AMI_IMAGE_UNKNOWN;
    // Make sure we found the right instance of the FV protocol
    do{
        // DevicePath may contain more than one MEDIA_PIWG_FW_VOL_DP node.
        // Advance Node beyond the last MEDIA_PIWG_FW_VOL_DP node.
        Node=NextDevicePathNode(Node);
    }while(Node->Type == MEDIA_DEVICE_PATH && Node->SubType == MEDIA_PIWG_FW_VOL_DP);
    if (DevicePath != Node)
        return AMI_IMAGE_UNKNOWN;

    // Make sure the rest of device path looks fine
    // We're expecting FwFile node...
    if (Node->Type != MEDIA_DEVICE_PATH || Node->SubType != MEDIA_PIWG_FW_FILE_DP)
        return AMI_IMAGE_UNKNOWN;
    // ...followed by the terminating node
    if (!IsDevicePathEnd(NextDevicePathNode(Node)))
        return AMI_IMAGE_UNKNOWN;

    // Make sure we are dealing with the known FV.
    return AmiItemListFind(&FvHandles, Handle, NULL) ? AMI_IMAGE_FROM_FV : AMI_IMAGE_UNKNOWN;
}

/// The function handles Relative Offset Device Path (OpROM images)
UINT32 AmiImageTypeLibRelativeOffsetDevicePathHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    EFI_PCI_IO_PROTOCOL *PciIo;
    UINT64 Attributes;

    PciIo = AmiImageTypeLibLocateInterface(&DevicePath, &gEfiPciIoProtocolGuid, NULL);
    // Make sure it's a PCI device
    if ( PciIo==NULL )
        return AMI_IMAGE_UNKNOWN;
    // Make sure we found the right PCI I/O instance
    if (DevicePath != Node)
        return AMI_IMAGE_UNKNOWN;
    // Make sure relative offset node is the last node in the device path.
    if (!IsDevicePathEnd(NextDevicePathNode(Node)))
        return AMI_IMAGE_UNKNOWN;

    // Check if this is device with an embedded OpROM. Embedded OpROM are loaded from one of the firmware FV.
    if (EFI_ERROR(PciIo->Attributes(PciIo, EfiPciIoAttributeOperationGet, 0, &Attributes)))
        return AMI_IMAGE_UNKNOWN;

    return ( Attributes & EFI_PCI_IO_ATTRIBUTE_EMBEDDED_ROM ) ? AMI_IMAGE_FROM_FV : AMI_IMAGE_FROM_OPTION_ROM;
}

/// The function handles Memory Mapped Device Path
UINT32 AmiImageTypeLibMemMapDevicePathHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    UINT32 ImageType;
    // We handle two breeds of memory-mapped device paths:
    // 1. Device path installed by PciBus driver for embedded OpROM images.
    // 2. Device path installed on handles of FV that do not expose FV name GUID via extended FV header.
    ImageType = AmiImageTypeLibRelativeOffsetDevicePathHandler(DevicePath,Node);
    if (ImageType != AMI_IMAGE_UNKNOWN) return ImageType;
    return AmiImageTypeLibFvDevicePathHandler(DevicePath,Node);
}

/// The function handles File Device Path
UINT32 AmiImageTypeLibFileDevicePathHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    EFI_BLOCK_IO_PROTOCOL *BlockIo;
    EFI_BLOCK_IO2_PROTOCOL *BlockIo2;

    // Use BlockIo or BlockIo2 instance to detect the media type.
    // If we are dealing with file system without an associated Block I/O device, return AMI_IMAGE_UNKNOWN.
    BlockIo = AmiImageTypeLibLocateInterface(&DevicePath, &gEfiBlockIoProtocolGuid, NULL);
    if ( BlockIo != NULL ){
        return (BlockIo->Media->RemovableMedia) ? AMI_IMAGE_FROM_REMOVABLE_MEDIA : AMI_IMAGE_FROM_FIXED_MEDIA;
    }
    BlockIo2 = AmiImageTypeLibLocateInterface(&DevicePath, &gEfiBlockIo2ProtocolGuid, NULL);
    if ( BlockIo2 != NULL ){
        return (BlockIo2->Media->RemovableMedia) ? AMI_IMAGE_FROM_REMOVABLE_MEDIA : AMI_IMAGE_FROM_FIXED_MEDIA;
    }

    return AMI_IMAGE_UNKNOWN;
}

/// The function handles embedded applications such ash Shell loaded via FwLoadFileInterface published by BDS
UINT32 AmiImageTypeLibEmbeddedApplicationHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath, IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    // Make sure we are dealing with the vendor device path installed by BDS
    if (   Node!=DevicePath // Vendor DP Node must the the first node
        || DevicePathNodeLength(Node) != sizeof(VENDOR_DEVICE_PATH)
        || !CompareGuid((EFI_GUID*)(Node+1), &AmiMediaDevicePathGuid)
    ) return AMI_IMAGE_UNKNOWN;
    // Make sure in instance of LoadFile exists
    if (AmiImageTypeLibLocateInterface(&DevicePath, &gEfiLoadFileProtocolGuid, NULL)==NULL)
        return AMI_IMAGE_UNKNOWN;
    // Make sure we found the right LoadFile instance
    Node=NextDevicePathNode(Node);
    if (DevicePath != Node)
        return AMI_IMAGE_UNKNOWN;

    // Vendor DP may be optionally followed by a FwFile node...
    if (Node->Type == MEDIA_DEVICE_PATH && Node->SubType == MEDIA_PIWG_FW_FILE_DP)
        Node=NextDevicePathNode(Node);
    // ...followed by the terminating node
    if (!IsDevicePathEnd(Node))
        return AMI_IMAGE_UNKNOWN;

    // At this point we know that we are dealing with the embedded application.
    // Return image type based on the platform policy set forth by the AmiPcdLoadUnsignedEmbeddedApplications PCD.
    return (PcdGetBool(AmiPcdLoadUnsignedEmbeddedApplications)) ? AMI_IMAGE_FROM_FV : AMI_IMAGE_FROM_FIXED_MEDIA;
}

/// The function handles MAC Address Device Path
UINT32 AmiImageTypeLibMacDevicePathHandler(
    IN EFI_DEVICE_PATH_PROTOCOL *DevicePath,
    IN EFI_DEVICE_PATH_PROTOCOL *Node
){
    return AMI_IMAGE_FROM_REMOVABLE_MEDIA;
}

DEVICE_PATH_DESCRIPTOR DevicePathHandler[] = {
    { MEDIA_DEVICE_PATH, MEDIA_PIWG_FW_VOL_DP, AmiImageTypeLibFvDevicePathHandler}, // FV
    { HARDWARE_DEVICE_PATH, HW_MEMMAP_DP, AmiImageTypeLibMemMapDevicePathHandler}, // FV or Embedded OpROM
    { MEDIA_DEVICE_PATH, MEDIA_RELATIVE_OFFSET_RANGE_DP, AmiImageTypeLibRelativeOffsetDevicePathHandler}, // PCI
    { MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP, AmiImageTypeLibFileDevicePathHandler}, // File
    { MEDIA_DEVICE_PATH, MEDIA_VENDOR_DP, AmiImageTypeLibEmbeddedApplicationHandler}, // Shell
    { MESSAGING_DEVICE_PATH, MSG_MAC_ADDR_DP, AmiImageTypeLibMacDevicePathHandler}, // Network
    {0,0,NULL}
};

/**
  The function returns image type.

  @param[in]  File         A device path the image was loaded from.
                           This is a device path that was used to call gBS->LoadImage().
  @param[in]  FileBuffer   A pointer to the PE buffer of the image
  @param[in]  FileSize     The size of FileBuffer.
  @param[in]  BootPolicy   A boot policy that was used to call gBS->LoadImage().

  @return Image Type. See @ref AmiImageTypeLibImageTypes "Image Types" for a list of supported values.
**/
UINT32 AmiGetImageType (
    IN CONST EFI_DEVICE_PATH_PROTOCOL   *File,
    IN VOID *FileBuffer, IN UINTN FileSize, IN BOOLEAN BootPolicy
){
    DEVICE_PATH_DESCRIPTOR *Descriptor;
    EFI_DEVICE_PATH_PROTOCOL *Node;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath = (EFI_DEVICE_PATH_PROTOCOL *)File;

    if ( DevicePath==NULL ) return AMI_IMAGE_UNKNOWN;

    for( Node = DevicePath; !IsDevicePathEnd(Node); Node=NextDevicePathNode(Node) ){
        for(Descriptor = DevicePathHandler; Descriptor->Handler!=0; Descriptor++){
            if (Node->Type != Descriptor->Type || Node->SubType != Descriptor->SubType) continue;
            return Descriptor->Handler(DevicePath,Node);
        }
    }

    return AMI_IMAGE_UNKNOWN;
}

/// DxeSmmReadyToLock protocol callback
VOID EFIAPI AmiImageTypeLibSmmReadyToLockCallBack(IN EFI_EVENT Event, IN VOID *Context){
    // All FV published after SmmReadyToLock are considered untrusted.
    if (FwVolumeEvent != NULL) gBS->CloseEvent(FwVolumeEvent);
    gBS->CloseEvent(Event);
}

EFI_HANDLE AmiImageTypeLibFindFvHandleByGuid(CONST EFI_GUID *Guid){
    UINTN NumberOfHandles;
    EFI_HANDLE *Handle;
    UINTN i;
    EFI_DEVICE_PATH_PROTOCOL *Dp;
    EFI_STATUS Status;
    EFI_HANDLE FvHandle;

    Status = gBS->LocateHandleBuffer (
        ByProtocol, &gEfiFirmwareVolume2ProtocolGuid,
        NULL,  &NumberOfHandles, &Handle
    );
    if (EFI_ERROR(Status)) return NULL;

    FvHandle = NULL;
    for(i=0; i<NumberOfHandles; i++ ){
        MEDIA_FW_VOL_DEVICE_PATH *LastNode;

        Status = gBS->HandleProtocol(Handle[i], &gEfiDevicePathProtocolGuid, (VOID**)&Dp);
        if (EFI_ERROR(Status)) continue;
        for(LastNode=NULL; !IsDevicePathEndType(Dp); Dp=NextDevicePathNode(Dp))
            LastNode = (MEDIA_FW_VOL_DEVICE_PATH*)Dp;

        if (    LastNode != NULL
             && LastNode->Header.Type == MEDIA_DEVICE_PATH
             && LastNode->Header.SubType == MEDIA_PIWG_FW_VOL_DP
             && CompareGuid(&LastNode->FvName, Guid)
        ){
            FvHandle = Handle[i];
            break;
        }

    }
    FreePool(Handle);
    return FvHandle;
}

/// Checks if FV associated with Handle or, in case of nested FV, a parent FV is signed.
BOOLEAN AmiImageTypeLibIsSignedFv(EFI_HANDLE Handle){
    EFI_FIRMWARE_VOLUME_BLOCK2_PROTOCOL *FvBlock;
    EFI_DEV_PATH_PTR Dp;
    EFI_STATUS Status;
    AMI_ROM_AREA *Area;
    EFI_PHYSICAL_ADDRESS Address;
    EFI_FIRMWARE_VOLUME_HEADER *Fv;
    EFI_HOB_FIRMWARE_VOLUME2 *Fv2Hob;
#ifdef EFI_HOB_TYPE_FV3
    EFI_HOB_FIRMWARE_VOLUME3 *Fv3Hob;
#endif

    // If the FV is knows to be singed, return TRUE
    if (AmiItemListFind(&FvHandles, Handle, NULL)) return TRUE;

    // To find out if FV is signed or not, we need to find ROM area descriptor(AMI_ROM_AREA)
    // corresponding to the FV.

    // Attempt to find ROM area descriptor using device path information
    Status = gBS->HandleProtocol(Handle, &gEfiDevicePathProtocolGuid, (VOID**)&Dp.DevPath);
    if (EFI_ERROR(Status)) return FALSE;

    // If FV has been extracted from another FV (a.k.a. nested FV),
    // its device path may contain more than one node associated with FV.
    // The last node represents FV being processed.
    // Earlier FV nodes represent parent FV.
    // We start analysis from parent FV. If parent FV is signed, all the nested FV are also signed.
    for(Area=NULL ; !IsDevicePathEndType(Dp.DevPath); Dp.DevPath = NextDevicePathNode(Dp.DevPath)){
        if (Dp.DevPath->Type == MEDIA_DEVICE_PATH && Dp.DevPath->SubType == MEDIA_PIWG_FW_VOL_DP){
            Area = AmiGetFirstRomAreaByGuid(&Dp.FirmwareVolume->FvName);
        }else if (Dp.DevPath->Type == HARDWARE_DEVICE_PATH && Dp.DevPath->SubType == HW_MEMMAP_DP){
            Area = AmiGetRomAreaByAddress(Dp.MemMap->StartingAddress);
        }else {
            Area = NULL;
        }
        // If Area is signed, return TRUE.
        // If Area is unsigned, continue with the loop to process child FV.
        if (Area != NULL && (Area->Attributes & AMI_ROM_AREA_SIGNED)!= 0) return TRUE;
    }
    // If we're here, the area is either has not been found or is unsigned.
    if (Area != NULL) return FALSE;

    // We failed to find ROM area descriptor using device path information.
    // Use FvBlock protocol instance to find parent FV.
    Status = gBS->HandleProtocol(Handle, &gEfiFirmwareVolumeBlockProtocolGuid, (VOID**)&FvBlock);
    if (EFI_ERROR(Status)) return FALSE;

    if (FvBlock->ParentHandle != NULL){
        return AmiImageTypeLibIsSignedFv(FvBlock->ParentHandle);
    }
    // If FvBlock->ParentHandle is not available, use FV HOB information.
    Status = FvBlock->GetPhysicalAddress(FvBlock, &Address);
    if (EFI_ERROR(Status) || (UINTN)Address!=Address) return FALSE;
    Fv = (EFI_FIRMWARE_VOLUME_HEADER*)(UINTN)Address;
#ifdef EFI_HOB_TYPE_FV3
    // Scan FV3 HOBs.
    for(   Fv3Hob = GetFirstHob(EFI_HOB_TYPE_FV3)
         ; Fv3Hob != NULL
         ; Fv3Hob = GetNextHob(EFI_HOB_TYPE_FV3, Fv3Hob)
    ){
        if (    Fv3Hob->BaseAddress == Address
             && Fv3Hob->Length == Fv->FvLength
             && Fv3Hob->ExtractedFv
        ){
            Handle = AmiImageTypeLibFindFvHandleByGuid(&Fv3Hob->FvName);
            if (Handle!=NULL) return AmiImageTypeLibIsSignedFv(Handle);
        }
        Fv3Hob = GET_NEXT_HOB (Fv3Hob);
    }
#endif
    // Scan FV2 HOBs.
    for(   Fv2Hob = GetFirstHob(EFI_HOB_TYPE_FV2)
         ; Fv2Hob != NULL
         ; Fv2Hob = GetNextHob(EFI_HOB_TYPE_FV2, Fv2Hob)
    ){
        if (    Fv2Hob->BaseAddress == Address
             && Fv2Hob->Length == Fv->FvLength
        ){
            Handle = AmiImageTypeLibFindFvHandleByGuid(&Fv2Hob->FvName);
            if (Handle!=NULL) return AmiImageTypeLibIsSignedFv(Handle);
        }
        Fv2Hob = GET_NEXT_HOB (Fv2Hob);
    }
    // No luck. Giving up...
    return FALSE;
}

/// FirmwareVolume2 protocol callback.
/// The function maintains array of singed FV that are part of the firmware image.
VOID EFIAPI AmiImageTypeLibFwVolumeCallBack(IN EFI_EVENT Event, IN VOID *Context){
    EFI_HANDLE Handle;
    EFI_STATUS Status;

    do{
        UINTN Size = sizeof (EFI_HANDLE);
        Status = gBS->LocateHandle (
            ByRegisterNotify, NULL, FwVolumeRegistration, &Size, &Handle
        );
        if (EFI_ERROR(Status)) return;
        if (!AmiImageTypeLibIsSignedFv(Handle)) continue;
        //add handle to the list of trusted FV
        AmiItemListAppend(&FvHandles, Handle);
    }while(TRUE);
}

/**
  Library constructor. Called in Driver's entry point.
  @param ImageHandle     Image handle.
  @param SystemTable    Pointer to the EFI system table.

  @retval: EFI_SUCCESS
**/
EFI_STATUS EFIAPI AmiImageTypeLibConstructor(
    IN  EFI_HANDLE ImageHandle, IN  EFI_SYSTEM_TABLE *SystemTable
){
    EFI_STATUS Status;
    VOID *Registration;

    if (FwVolumeEvent != NULL) return EFI_SUCCESS;

    Status = gBS->CreateEvent (
        EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
        AmiImageTypeLibSmmReadyToLockCallBack, NULL,  &SmmReadyToLockEvent
    );
    ASSERT_EFI_ERROR (Status);
    Status = gBS->RegisterProtocolNotify (
        &gEfiDxeSmmReadyToLockProtocolGuid, SmmReadyToLockEvent, &Registration
    );
    ASSERT_EFI_ERROR (Status);

    FwVolumeEvent = EfiCreateProtocolNotifyEvent (
        &gEfiFirmwareVolume2ProtocolGuid, TPL_CALLBACK,
        AmiImageTypeLibFwVolumeCallBack, NULL, &FwVolumeRegistration
    );
    ASSERT(FwVolumeEvent != NULL);
    return EFI_SUCCESS;
}

/**
  Library constructor. Called in Driver's entry point.
  @param ImageHandle     Image handle.
  @param SystemTable    Pointer to the EFI system table.

  @retval: EFI_SUCCESS
**/
EFI_STATUS EFIAPI AmiImageTypeLibDestructor(
    IN  EFI_HANDLE ImageHandle, IN  EFI_SYSTEM_TABLE *SystemTable
){
    gBS->CloseEvent(SmmReadyToLockEvent);
    gBS->CloseEvent(FwVolumeEvent);
    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
