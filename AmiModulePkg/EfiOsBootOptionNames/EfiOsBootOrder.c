//***********************************************************************
//*                                                                     *
//*                      Copyright (c) 1985-2021, AMI.                  *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file EfiOsBootOrder.c

    Deal with UEFI Boot Devices that contain UEFI OS.

    Create boot option if device is existed, delete 
    boot option if there is a duplicated one and then 
    adjust the BootOrder.
*/

#include <BootOptions.h>
#include "EfiOsNamesFilePathMaps.h"
#include <Protocol/DiskInfo.h>
#include <Token.h>
#include <EfiOsBootOptionNames.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>

#if FIXED_BOOT_ORDER_SUPPORT
#include "FixedBootOrder/FixedBootOrder.h"
#endif

typedef EFI_STATUS (CREATE_EFI_OS_BOOT_OPTIONS) (EFI_HANDLE Handle);
extern CREATE_EFI_OS_BOOT_OPTIONS CREATE_EFI_OS_BOOT_OPTIONS_FUNC_PTR;
CREATE_EFI_OS_BOOT_OPTIONS* CreateEfiOsBootOptionsFuncPtr = CREATE_EFI_OS_BOOT_OPTIONS_FUNC_PTR;

typedef UINTN (CREATE_EFI_OS_BOOT_OPTION_FUNCS) (EFI_HANDLE Handle, EFI_FILE_PROTOCOL* File);
extern CREATE_EFI_OS_BOOT_OPTION_FUNCS CREATE_EFI_OS_BOOT_OPTION_FUNCTIONS EndOfCreateEfiOsBootOptionFunctions;
CREATE_EFI_OS_BOOT_OPTION_FUNCS* CreateEfiOsBootOptionFunctions[] = {CREATE_EFI_OS_BOOT_OPTION_FUNCTIONS NULL};

typedef BOOLEAN (CREATE_TARGET_EFI_OS_BOOT_OPTION) (EFI_HANDLE Handle, NAME_MAP* NameMap);
extern CREATE_TARGET_EFI_OS_BOOT_OPTION CREATE_TARGET_EFI_OS_BOOT_OPTION_FUNC_PTR;
CREATE_TARGET_EFI_OS_BOOT_OPTION* CreateTargetEfiOsBootOptionFuncPtr = CREATE_TARGET_EFI_OS_BOOT_OPTION_FUNC_PTR;

typedef VOID (ADJUST_EFI_OS_BOOT_ORDER_FUNCS) (SIMPLE_BOOT_OPTION* Options, UINTN OptionCount);
extern ADJUST_EFI_OS_BOOT_ORDER_FUNCS ADJUST_EFI_OS_BOOT_ORDER_FUNCTIONS EndOfAdjustEfiOsbootOrderFunctions;
ADJUST_EFI_OS_BOOT_ORDER_FUNCS* AdjustEfiOsBootOrderFunctions[] =  {ADJUST_EFI_OS_BOOT_ORDER_FUNCTIONS NULL};

typedef BOOLEAN (CHECK_UEFI_OS_BOOT_OPTION_FUNCS) (BOOT_OPTION* Options);
extern CHECK_UEFI_OS_BOOT_OPTION_FUNCS CHECK_UEFI_OS_BOOT_OPTION_FUNCTIONS EndOfCheckUefiOsFunctions;
CHECK_UEFI_OS_BOOT_OPTION_FUNCS* CheckUefiOsBootOptionFunctions[] =  {CHECK_UEFI_OS_BOOT_OPTION_FUNCTIONS NULL};

/**
    Arrary of eLinks that contain many OS image paths and their corresponding OS names.
*/
NAME_MAP FILE_NAME_MAPS[] = { EfiOsFilePathMaps {NULL,NULL} };	//(EIP103672+)

extern BOOLEAN NormalizeBootOptionName ;
extern BOOLEAN NormalizeBootOptionDevicePath ;

#if DISPLAY_FULL_OPTION_NAME_WITH_FBO
extern EFI_HANDLE GetPhysicalBlockIoHandle (IN EFI_HANDLE BlockIoHandle) ;
extern UINTN RemoveTrailingSpaces(CHAR16 *Name, UINTN NumberOfCharacters);
extern UINT16 gSATA[3][2] ;
#endif

EFI_DEVICE_PATH_PROTOCOL **NewEfiOsOptionDpList = NULL ;
UINTN NewEfiOsOptionDpListCount = 0 ;

typedef BOOLEAN (*COMPARE_DEVICE_PATH) (EFI_DEVICE_PATH_PROTOCOL *Dp1, EFI_DEVICE_PATH_PROTOCOL *Dp2);
EFI_STATUS CreateBootOptionWithUefiFileName(EFI_HANDLE Handle, EFI_FILE_PROTOCOL *File);
BOOLEAN IsRemovableDevice(EFI_HANDLE Handle);
BOOLEAN SkipRemovableDevice(EFI_HANDLE Handle);
BOOLEAN AllSupported (EFI_HANDLE Handle);
VOID CreatedBootOptionsCallback(EFI_HANDLE Handle, UINTN Count);
VOID CreatedFilePathListCallback(EFI_HANDLE Handle, NAME_MAP *NameMsp, EFI_DEVICE_PATH_PROTOCOL *Dp);
VOID AddedBootOptionToListCallback(EFI_HANDLE FileSystemHandle, BOOT_OPTION *Option);
UINT16 GetHDDPort( IN EFI_HANDLE Handle );
BOOLEAN IsSpecifiedUefiOsBootOption(BOOT_OPTION *Option);
EFI_DEVICE_PATH_PROTOCOL* GetDevicePathNodeByTypeSubtype(EFI_DEVICE_PATH_PROTOCOL* Dp, UINT16 Type, UINT16 SubType);
struct {
    BOOLEAN (*IsSupported) (EFI_HANDLE Handle);
    VOID (*CreatedBootOptionsCallback) (EFI_HANDLE Handle, UINTN Count);
    VOID (*CreatedFilePathListCallback) (EFI_HANDLE Handle, NAME_MAP *NameMsp, EFI_DEVICE_PATH_PROTOCOL *Dp);
    VOID (*AddedBootOptionToListCallback) (EFI_HANDLE Handle, BOOT_OPTION *Option);
} gEobonHook = {
#if !defined(EOBON_SKIP_REMOVABLE_DEVICE) || EOBON_SKIP_REMOVABLE_DEVICE == 1 
    SkipRemovableDevice,
#else
    AllSupported,
#endif
    CreatedBootOptionsCallback,
    CreatedFilePathListCallback,
    AddedBootOptionToListCallback
};

/** 
   Check if removable device.

   @param Handle  Handle to check
   
   @retval TRUE   Removable device
   @retval FALSE  Not a Removable device
**/
BOOLEAN IsRemovableDevice(EFI_HANDLE Handle)
{
    EFI_STATUS Status;
    EFI_BLOCK_IO_PROTOCOL *BlkIo;
    Status = pBS->HandleProtocol (
                        Handle,
                        &gEfiBlockIoProtocolGuid,
                        (VOID **)&BlkIo
                        );
    if (EFI_ERROR(Status)) return FALSE;

    return BlkIo->Media->RemovableMedia;
}

/** 
   All types of device are supported.

   @param Handle  Handle to check
   
   @retval TRUE  Supported
**/
BOOLEAN AllSupported (EFI_HANDLE Handle)
{
    return TRUE;
}

/** 
   Removable device is not supported

   @param Handle  Handle to check
   
   @retval TRUE   Supported
   @retval FALSE  Is not supported
**/
BOOLEAN SkipRemovableDevice(EFI_HANDLE Handle)
{
    return !IsRemovableDevice(Handle);
}

/** 
   Callback after creating the boot option.

   @param Handle  Used to create the boot option
   @param Count   Number of the boot options created
**/
VOID CreatedBootOptionsCallback(EFI_HANDLE Handle, UINTN Count)
{
#if CREATE_BOOT_OPTION_WITH_UEFI_FILE_NAME_POLICY == 1
    if(!Count)
    {
        EFI_STATUS Status;
        EFI_FILE_PROTOCOL *File;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;

        Status = pBS->HandleProtocol (
                            Handle,
                            &gEfiSimpleFileSystemProtocolGuid,
                            (VOID **)&SimpleFileSystem
                            );
        if (EFI_ERROR(Status)) return;

        Status = SimpleFileSystem->OpenVolume ( SimpleFileSystem, &File); 
        if (EFI_ERROR(Status)) return;

        CreateBootOptionWithUefiFileName(Handle, File);
    }
#endif
}

/** 
   Callback after creating the FilePathList for the boot option. 

   @param Handle  Used to create the boot option
   @param Option  Added to the list
**/
VOID CreatedFilePathListCallback(EFI_HANDLE FileSystemHandle, NAME_MAP *NameMap, EFI_DEVICE_PATH_PROTOCOL *Dp)
{
#if DISPLAY_FULL_OPTION_NAME_WITH_FBO
    //
    // Uefi OS in Setup as style "OS Name(PX: DeviceName)" because of 
    // EfiOsBootOptionNames module creates it by ChangeUefiBootNames
    // eLink to change bootData->Name, however, FixedBootOrder module
    // creates its oneof items with BootXXXX variable's description 
    // which saved according to Option->Description, that makes only
    // "OS Name" appear in setup, hence we need to use FixedBootOrder
    // protcol to set new description.
    //
    {
        EFI_STATUS Status;
        CHAR16 *ControllerName ;
        CHAR16 *DeviceName ;
        UINT16 PortNumber = 0xff ;
        UINTN  NumberOfCharacters = 0, DeviceNameLength = 0;
        EFI_HANDLE Handle ;
        EFI_FIXED_BOOT_ORDER_PROTOCOL *Fbo = NULL ;
        EFI_GUID FixedBootOrderGuid = FIXED_BOOT_ORDER_GUID ;
        
        Status = pBS->LocateProtocol(&FixedBootOrderGuid, NULL, &Fbo);
        Handle = GetPhysicalBlockIoHandle(FileSystemHandle);
        if (GetControllerName(Handle, &ControllerName) && !EFI_ERROR(Status)) 
        {   
            DeviceNameLength = (StrLen(ControllerName)+1)*sizeof(CHAR16) ;
            DeviceName = MallocZ(DeviceNameLength) ;
            PortNumber = GetHDDPort( Handle );
            NumberOfCharacters = UnicodeSPrint(DeviceName, DeviceNameLength, L"%S", ControllerName);
            DeviceNameLength = RemoveTrailingSpaces(DeviceName, NumberOfCharacters);
            if ( DeviceNameLength )
            {
                FBO_DEVICE_INFORM *inform ;
                CHAR16 *String ;
                UINTN StringLength = DeviceNameLength*sizeof(CHAR16)
                                     + StrLen(NameMap->BootOptionName)*sizeof(CHAR16)
                                     + 0xf ;
                String = MallocZ(StringLength) ;
                if ( PortNumber != 0xff )
                {
                    UnicodeSPrint ( String,
                                StringLength,
                                L"%S (P%d: %S)",
                                NameMap->BootOptionName,
                                PortNumber,
                                DeviceName );
                    DEBUG((DEBUG_BM | DEBUG_VERBOSE, "CreateTargetEfiOsBootOption :: Reset the Boot Option Name:%S \n", String)) ;
                }
                else  // It don't have port number
                {
                    UnicodeSPrint ( String,
                                StringLength,
                                L"%S (%S)",
                                NameMap->BootOptionName,
                                DeviceName );
                    DEBUG((DEBUG_BM | DEBUG_VERBOSE, "CreateTargetEfiOsBootOption :: Reset the Boot Option Name:%S \n", String)) ;
                }
                
                inform = MallocZ(sizeof(FBO_DEVICE_INFORM)) ;
                inform->DevName = String ;
                inform->DevPath = MallocZ(DPLength(Dp)) ;
                MemCpy(inform->DevPath, Dp, DPLength(Dp)) ;
                Status = Fbo->SetNewDescription(inform) ;
                if (EFI_ERROR(Status))
                    DEBUG((DEBUG_BM | DEBUG_VERBOSE | DEBUG_ERROR, "CreateTargetEfiOsBootOption :: Can't set new description with fbo protocol")) ;
            }
            pBS->FreePool(DeviceName) ;
        }
    }
#endif
}

/** 
   Callback after adding the boot option to the list.

   @param Handle  Used to create the boot option
   @param Option  Added to the list
**/
VOID AddedBootOptionToListCallback(EFI_HANDLE FileSystemHandle, BOOT_OPTION *Option)
{
    VOID **ptr = NULL ;
    ptr = MallocZ(sizeof(VOID*)
                  * (NewEfiOsOptionDpListCount+1)) ;
    if (ptr)
    {
        MemCpy(ptr, NewEfiOsOptionDpList, sizeof(VOID*)
                                      * NewEfiOsOptionDpListCount) ;
        *(ptr+NewEfiOsOptionDpListCount) = Option->FilePathList ;

        NewEfiOsOptionDpListCount++ ;
        pBS->FreePool(NewEfiOsOptionDpList) ;
        NewEfiOsOptionDpList = (EFI_DEVICE_PATH_PROTOCOL**)ptr ;
    }
}

/**
   Get the device path node by the type and the subtype.

   @param  Dp       Device path for getting the node
   @param  Type     Type of the node to get
   @param  SubType  SubType of the node to get

   @retval  NULL                       If input Dp is NULL or find no matched device path node
   @retval  EFI_DEVICE_PATH_PROTOCOL*  Get the matched device path node

   @note  This function doesn't support getting the end device node.

**/
EFI_DEVICE_PATH_PROTOCOL*
GetDevicePathNodeByTypeSubtype(EFI_DEVICE_PATH_PROTOCOL* Dp, UINT16 Type, UINT16 SubType)
{
    EFI_DEVICE_PATH_PROTOCOL *DpWalker = Dp;
    if (!Dp) return NULL;

    for (; !isEndNode(DpWalker); DpWalker = NEXT_NODE(DpWalker))
    {
        if (DpWalker->Type == Type
                && DpWalker->SubType == SubType)
            return DpWalker;
    }

    return NULL;
}

/**
    Locates HD node in DevicePath associated with Handle.

    @param  Handle    Handle with DevicePath protocol for which HD
                      node should be located.
    @param  DevPath   Pointer to HD node, if found.

    @retval  EFI_SUCCESS    HD node was found and returned.
    @retval  EFI_NOT_FOUND  No HD node was found.
    @return  Other errors, if Handle does not have DevicePath protocol.
*/
EFI_STATUS GetHdNode (
    IN EFI_HANDLE Handle,
    OUT EFI_DEVICE_PATH_PROTOCOL** DevPath
)
{
    EFI_STATUS Status;
    EFI_DEVICE_PATH_PROTOCOL* DevicePath;

    *DevPath = NULL;

    // Get DevicePath attached to handle.
    Status = pBS->HandleProtocol (
                 Handle,
                 &gEfiDevicePathProtocolGuid,
                 &DevicePath
             );
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_BM | DEBUG_VERBOSE | DEBUG_ERROR, "GetHdNode :: HandleProtocol: %r\n", Status));
        return Status;
    }

    DevicePath = GetDevicePathNodeByTypeSubtype(DevicePath, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
    if (!DevicePath) return EFI_NOT_FOUND;

    *DevPath = DevicePath;
    return EFI_SUCCESS;
}

/**
    Non case sensitive string comparison.

    @param  Str1   String to compare
    @param  Str2   String be compared to

    @retval  TRUE    Strings are the same
    @retval  FALSE   Strings aren't the same
**/
BOOLEAN StrNoCaseCmp(CHAR16* Str1, CHAR16* Str2)
{
    while(*Str1)
    {
        if(*Str1 != *Str2)
        {
            CHAR16 Char = *Str1;

            if(Char >= 0x41 && Char <= 0x5a)
                Char += 0x20;
            else if(Char >= 0x61 && Char <= 0x7a)
                Char -= 0x20;
            else
                return FALSE;

            if(Char != *Str2)
                return FALSE;
        }

        Str1++;
        Str2++;
    }

    return (*Str1 == *Str2);
    
}

#if DISPLAY_FULL_OPTION_NAME_WITH_FBO
/**
    Use handle and EFI_DISK_INFO_PROTOCOL to get sata hard disk port.

    @param  Handle  Use in locating EFI_DISK_INFO_PROTOCOL. 
  
    @retval  UINT16  Sata port number.
*/
UINT16 GetHDDPort( IN EFI_HANDLE Handle )
{
    EFI_STATUS Status;
    EFI_DISK_INFO_PROTOCOL *DiskInfo;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath;
    UINT32 IdeChannel;
    UINT32 IdeDevice;

    Status = pBS->HandleProtocol (
                 Handle,
                 &gEfiDevicePathProtocolGuid,
                 (VOID *) &DevicePath );

    if ( !EFI_ERROR( Status ))
    {
        EFI_DEVICE_PATH_PROTOCOL *DevicePathNode = NULL;
        EFI_DEVICE_PATH_PROTOCOL *MessagingDevicePath = NULL;
        PCI_DEVICE_PATH          *PciDevicePath = NULL;

        DevicePathNode = DevicePath;
        while (!isEndNode (DevicePathNode))
        {
            if ((DevicePathNode->Type == HARDWARE_DEVICE_PATH)
                    && (DevicePathNode->SubType == HW_PCI_DP))
                PciDevicePath = (PCI_DEVICE_PATH *) DevicePathNode;
            else if (DevicePathNode->Type == MESSAGING_DEVICE_PATH)
                MessagingDevicePath = DevicePathNode;

            DevicePathNode = NEXT_NODE (DevicePathNode);
        }

        Status = pBS->HandleProtocol ( Handle, &gEfiDiskInfoProtocolGuid, &DiskInfo );
        if ( !EFI_ERROR(Status) )
        {
            Status = DiskInfo->WhichIde ( DiskInfo, &IdeChannel, &IdeDevice );
            if ( !EFI_ERROR(Status) )
            {
                if (MessagingDevicePath && MessagingDevicePath->SubType == MSG_ATAPI_DP ) //IDE mode?
                {
                    if (PciDevicePath && PciDevicePath->Function == 5)
                        return gSATA[IdeDevice+2][IdeChannel];
                    else
                        return gSATA[IdeDevice][IdeChannel];
                }
                else
                    return (UINT16)IdeChannel;  //AHCI Port Number
            }
        }
    }
    return 0xff;
}
#endif

/**
    Search GPT HDD and return Hard disk handle.

    @param  DevicePath  Converting to HARDDRIVE_DEVICE_PATH.
    
    @retval  EFI_HANDLE  Hard Disk handle or NULL.
*/
EFI_HANDLE GetGptPartitionHandle(EFI_DEVICE_PATH_PROTOCOL *DevicePath)
{
    EFI_STATUS	Status;
    EFI_HANDLE	*Handle, RetHandle = NULL;
    UINTN	Count, i;

    HARDDRIVE_DEVICE_PATH* BootParitionDevicePath;

    BootParitionDevicePath = (HARDDRIVE_DEVICE_PATH*)GetDevicePathNodeByTypeSubtype(DevicePath, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
    if (!BootParitionDevicePath) return NULL;

    //get list of available Block I/O devices
    Status = pBS->LocateHandleBuffer(ByProtocol,&gEfiBlockIoProtocolGuid,NULL,&Count,&Handle);
    if (EFI_ERROR(Status)) return NULL;

    for ( i=0; i<Count; i++ )
    {
        EFI_BLOCK_IO_PROTOCOL		*BlockIo;
        EFI_DEVICE_PATH_PROTOCOL	*Dp;
        HARDDRIVE_DEVICE_PATH*		PartitionNode;

        Status = pBS->HandleProtocol(Handle[i], &gEfiBlockIoProtocolGuid, &BlockIo);
        if (EFI_ERROR(Status)) continue;

        // if this is not partition, continue
        if (!BlockIo->Media->LogicalPartition) continue;

        Status = pBS->HandleProtocol(Handle[i], &gEfiDevicePathProtocolGuid, &Dp);
        if (EFI_ERROR(Status)) continue;

        PartitionNode = (HARDDRIVE_DEVICE_PATH*)GetDevicePathNodeByTypeSubtype(Dp, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
        if (!PartitionNode) continue;

        if ( PartitionNode->PartitionNumber == BootParitionDevicePath->PartitionNumber
                && PartitionNode->SignatureType == BootParitionDevicePath->SignatureType
                && !MemCmp(PartitionNode->Signature, BootParitionDevicePath->Signature, sizeof(PartitionNode->Signature)))
        {
            RetHandle = Handle[i];
            break;
        }
    }

    pBS->FreePool(Handle);
    return RetHandle;
}

//(EIP126686+)>
/**
    Compare whether two HDD device paths are the same.

    @param  DevDp1  Device path in comparison.
    @param  DevDp2  Device path in comparison.

    @retval  TRUE    Two HDD paths are the same.
    @retval  FALSE   Two HDD paths are not the same.
*/
BOOLEAN CompareHddDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevDp1, EFI_DEVICE_PATH_PROTOCOL *DevDp2 )
{
    DevDp1 = GetDevicePathNodeByTypeSubtype(DevDp1, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
    if (!DevDp1) return FALSE;

    DevDp2 = GetDevicePathNodeByTypeSubtype(DevDp2, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
    if (!DevDp2) return FALSE;

    if (MemCmp(DevDp1+1, DevDp2+1, sizeof(HARDDRIVE_DEVICE_PATH)-sizeof(EFI_DEVICE_PATH_PROTOCOL)) == 0) //Skip Header EFI_DEVICE_PATH_PROTOCOL.
    {
        EFI_DEVICE_PATH_PROTOCOL *UpDevDp1=NULL;
        EFI_DEVICE_PATH_PROTOCOL *UpDevDp2=NULL;
        DevDp1 = NEXT_NODE(DevDp1);
        DevDp2 = NEXT_NODE(DevDp2);

        if ( DevDp1->Type !=  MEDIA_DEVICE_PATH 
                || DevDp1->SubType != MEDIA_FILEPATH_DP
                || DevDp2->Type != MEDIA_DEVICE_PATH
                || DevDp2->SubType != MEDIA_FILEPATH_DP)
            return FALSE;
        pBS->AllocatePool (EfiBootServicesData,DPLength(DevDp1),&UpDevDp1);
        pBS->AllocatePool (EfiBootServicesData,DPLength(DevDp2),&UpDevDp2);
        pBS->CopyMem(UpDevDp1,DevDp1,DPLength(DevDp1));
        pBS->CopyMem(UpDevDp2,DevDp2,DPLength(DevDp2));

        Wcsupr( (CHAR16*)UpDevDp1+1 );
        Wcsupr( (CHAR16*)UpDevDp2+1 );

        if (MemCmp(UpDevDp1, UpDevDp2, DPLength(UpDevDp2)) == 0){
            pBS->FreePool(UpDevDp1);
            pBS->FreePool(UpDevDp2);
            return TRUE;
        }
    }

    return FALSE;
}

/**
    Compare whether two device paths are the same.

    @param  Dp1  Device path in comparison.
    @param  Dp2  Device path in comparison.

    @retval  TRUE    Two device paths are the same.
    @retval  FALSE   Two device paths are not the same.
*/
BOOLEAN CompareFullDevicePath(EFI_DEVICE_PATH_PROTOCOL *Dp1, EFI_DEVICE_PATH_PROTOCOL *Dp2)
{
    if (GetDevicePathSize(Dp1) != GetDevicePathSize(Dp2))
            return FALSE;

    return !MemCmp(Dp1, Dp2, GetDevicePathSize(Dp1));
}

/**
    Check the status of the OS Boot option.

    @param  Option  Boot option to check.

    @retval  TRUE   OS create bootoption is not hidden
    @retval  FALSE  OS create bootoption is hidden
*/
BOOLEAN CheckOsBootOptionStatus (BOOT_OPTION* Option)
{
    //We use FwBootOption to check if the option is create by OS, OS created option is Non FwBootOption.
    if(IsSpecifiedUefiOsBootOption(Option) && !Option->FwBootOption) {
        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"CheckOsBootOptionStatus :: This option is create by OS "));
        if (( Option->Attributes & LOAD_OPTION_HIDDEN ) == LOAD_OPTION_HIDDEN ) {
            DEBUG((DEBUG_BM | DEBUG_VERBOSE," Attributes is Hidden\n"));
            return FALSE;
        }else{
            DEBUG((DEBUG_BM | DEBUG_VERBOSE," Attributes is not Hidden\n"));
            return TRUE;
        }
    }
    return TRUE;
}

/**
  	Search all BootOptionList and found out the matched HDD device path.

    @param  HdDevPath  HDD device path.
    @param  Compare    Comparision function for the device path.

    @retval  EFI_SUCCESS    Find out the matched HDD device path.
    @retval  EFI_NOT_FOUND  Can't find any matched HDD device path.
*/
EFI_STATUS CheckBootOptionMatch (
    EFI_DEVICE_PATH_PROTOCOL* HdDevPath,
    COMPARE_DEVICE_PATH CompareDpMethod
)
{
    DLINK *Link;
    BOOT_OPTION *Option;

    FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option) {

        if (CompareDpMethod(Option->FilePathList, HdDevPath)) {
		//If we still work under FBO it will create multiple duplicate bootoption for every boot.
		//Because "BootOption->Attributes" won't work in FBO, FBO use it's own variable to check options.
		//Ex:UefiDevOrder and FboLegacyOrder 
		//It will also overwrite the "BootOption->Attributes" to sync with it's own variable.
#if !FIXED_BOOT_ORDER_SUPPORT
            if( !CheckOsBootOptionStatus(Option) ) {
                return EFI_NOT_FOUND;
            }
#endif  
            return EFI_SUCCESS;
        }
    }
    return EFI_NOT_FOUND;
}

//(EIP103672+)>
/**
    Count the elements in eLink FILE_NAME_MAPS.

    @retval  UINT16  Number of elements in eLink FILE_NAME_MAPS. 
*/
UINT16 GetEfiOsBootNameItemCount(void)
{
    UINT16 ItemCount=0;

    do {

        if ( FILE_NAME_MAPS[ItemCount].FilePath == NULL ) break;

        ItemCount++;

    } while (1);

    return ItemCount;
}
//<(EIP103672+)


/**
    Create target efi os boot option.

    @param  FileSystemHandle   Handle to get file system
    @param  NameMap            Contain boot file name and boot option name

    @retval  TRUE    if find any matched boot option / create boot option successfully
    @retval  FALSE   if one of inputs is invalid / fail to allocate pool / fail to get hdd node 

**/
BOOLEAN CreateTargetEfiOsBootOption(EFI_HANDLE FileSystemHandle, NAME_MAP* NameMap)
{
    UINTN OptionSize;
    EFI_DEVICE_PATH_PROTOCOL *DevicePath = NULL;
    BOOT_OPTION *Option;
    COMPARE_DEVICE_PATH CompareDpMethod;


    if(!FileSystemHandle || !NameMap)
        return FALSE;

    DevicePath = FileDevicePath(FileSystemHandle, NameMap->FilePath);
    if (!DevicePath) return FALSE;

    if (!IsRemovableDevice(FileSystemHandle))
    {
        EFI_DEVICE_PATH_PROTOCOL *TmpDp;

        //
        //  Only need the device path from type MEDIA_DEVICE_PATH and subtype MEDIA_HARDDRIVE_DP
        //
        TmpDp = GetDevicePathNodeByTypeSubtype(DevicePath, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
        TmpDp = DuplicateDevicePath(TmpDp);
        pBS->FreePool(DevicePath);
        DevicePath = TmpDp;
        if (!DevicePath) return FALSE;

        CompareDpMethod = CompareHddDevicePath;
    }
    else
    {
        CompareDpMethod = CompareFullDevicePath;
    }
    OptionSize = GetDevicePathSize(DevicePath);
    gEobonHook.CreatedFilePathListCallback(FileSystemHandle, NameMap, DevicePath);
    
#if CREATE_EFI_OS_BOOT_OPTIONS_FOR_CDROM == 0 && EOBON_SKIP_REMOVABLE_DEVICE == 0
    if (GetDevicePathNodeByTypeSubtype(DevicePath, MEDIA_DEVICE_PATH, MEDIA_CDROM_DP) != NULL ){
        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"CreateTargetEfiOsBootOption :: Don't Create EfiOsBootOption for CD-Rom\n"));
        pBS->FreePool(DevicePath);
        return TRUE;
    }
#endif
    
    if ( CheckBootOptionMatch( DevicePath, CompareDpMethod ) == EFI_SUCCESS )
    {
        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"CreateTargetEfiOsBootOption :: CheckBootOptionMatch Matched.....\n"));
        pBS->FreePool(DevicePath);
        return TRUE;
    }

    Option = CreateBootOption(BootOptionList);
    Option->FwBootOption = DefaultFwBootOption;

    pBS->AllocatePool (
        EfiBootServicesData,
        ((Wcslen(NameMap->BootOptionName) + 1) * sizeof(CHAR16)),
        &Option->Description);

    MemCpy(Option->Description,
           NameMap->BootOptionName,
           ((Wcslen(NameMap->BootOptionName) + 1) * sizeof(CHAR16)) );

    
    Option->FilePathList = DevicePath;
    Option->FilePathListLength = OptionSize;

    gEobonHook.AddedBootOptionToListCallback(FileSystemHandle, Option);

    return TRUE;
}

/**
    Create boot option with uefi file name if there is no other matched path.

    @param  Handle                     Handle to locate file system

**/    
EFI_STATUS CreateBootOptionWithUefiFileName(EFI_HANDLE Handle, EFI_FILE_PROTOCOL* File)
{
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL* NewFile;

    if(!Handle || !File) return EFI_INVALID_PARAMETER;

    Status = File->Open (
                 File,
                 &NewFile,
                 EFI_BOOT_FILE_NAME,
                 EFI_FILE_MODE_READ,
                 0
             );

    DEBUG((DEBUG_BM | DEBUG_VERBOSE, "CreateBootOptionWithUefiFileName :: Open(%S): %r\n", EFI_BOOT_FILE_NAME, Status));
    if(!EFI_ERROR(Status))
    {
        static NAME_MAP NameMap = {EFI_BOOT_FILE_NAME, NAME_OF_UEFI_OS};
        CreateTargetEfiOsBootOptionFuncPtr(Handle, &NameMap);

        NewFile->Close(NewFile);
    }

    return Status;
}

/**
    Create boot options by map.

    @param  Handle   Handle to locate file system.

    @retval  UINTN   Number of boot option created
**/
UINTN CreateBootOptionsByMap(EFI_HANDLE Handle, EFI_FILE_PROTOCOL* File)
{
    UINTN j;
    UINTN Count = 0;
    EFI_STATUS Status;
    UINT16 AUTO_BOOT_ENTRY_COUNT = GetEfiOsBootNameItemCount();

    if(!Handle || !File) return Count;

    for (j = 0; j < AUTO_BOOT_ENTRY_COUNT; j++)
    {

        EFI_FILE_PROTOCOL* NewFile;

        Status = File->Open (
                     File,
                     &NewFile,
                     FILE_NAME_MAPS[j].FilePath,
                     EFI_FILE_MODE_READ,
                     0
                 );

        DEBUG((DEBUG_BM | DEBUG_VERBOSE, "CreateBootOptionsByMap :: Open(%S): %r\n", FILE_NAME_MAPS[j].FilePath, Status));
        if (EFI_ERROR(Status)) continue;
        NewFile->Close(NewFile);

        if(CreateTargetEfiOsBootOptionFuncPtr(Handle, &FILE_NAME_MAPS[j]))
            Count++;
    }

    return Count;
}

/**
    Create efi os boot options.

    @param  Handle   Handle to check and create boot options.

    @retval  UINTN   Number of boot option created
**/
UINTN CreateEfiOsBootOptions(EFI_HANDLE Handle)
{
    UINTN             i = 0, Count = 0;
    EFI_STATUS        Status;
    EFI_FILE_PROTOCOL *File;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;

    if(!Handle) return Count;

    Status = pBS->HandleProtocol (
                 Handle,
                 &gEfiSimpleFileSystemProtocolGuid,
                 &SimpleFileSystem
             );
    if (EFI_ERROR(Status)) return Count;

    Status = SimpleFileSystem->OpenVolume ( SimpleFileSystem, &File); 
    if (EFI_ERROR(Status)) return Count;

    for (; CreateEfiOsBootOptionFunctions[i]; i++)
        Count += CreateEfiOsBootOptionFunctions[i](Handle, File);

    return Count;
}

/**
	Try to find UEFI OSs and create the boot options for them if 
    they haven't been listed in BootOptionList.

    @retval  EFI_SUCCESS   Do not encounter any errors.
    @retval  !=EFI_SUCESS  Any errors.
*/
EFI_STATUS CreateEfiOsBootOption(VOID)
{
    EFI_STATUS   Status;
    EFI_HANDLE*  HandleBuffer = NULL;
    UINTN        HandleCount;
    UINTN        i;

    Status = pBS->LocateHandleBuffer (
                 ByProtocol,
                 &gEfiSimpleFileSystemProtocolGuid,
                 NULL,
                 &HandleCount,
                 &HandleBuffer
             );
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_BM | DEBUG_VERBOSE | DEBUG_ERROR, "CreateEfiOsBootOption :: LocateHandleBuffer: %r\n", Status));
        return Status;
    }
    // For each handle found, check if eLink files exist.
    for (i = 0; i < HandleCount; i++) {
        UINTN Count;

        if (!gEobonHook.IsSupported(HandleBuffer[i])) continue;

        Count = CreateEfiOsBootOptionsFuncPtr(HandleBuffer[i]);

        gEobonHook.CreatedBootOptionsCallback(HandleBuffer[i], Count);
    }

    if ( HandleBuffer )
        pBS->FreePool(HandleBuffer);

    return EFI_SUCCESS;
}

                                                                 //(EIP138397+)>
/**
    Remove the AMI specific device path from SrcDevicePath.

    @param  SrcDevicePath   Device Path that has AMI mask device path.
    @param  DevicePathSize  Size of SrcDevicePath.
*/   
VOID RemoveAmiMaskDevicePath( EFI_DEVICE_PATH_PROTOCOL **SrcDevicePath, UINT16 DevicePathSize)
{
    EFI_DEVICE_PATH_PROTOCOL *TmpDevicePath = *SrcDevicePath;

    if(  TmpDevicePath->Type == HARDWARE_DEVICE_PATH
     && TmpDevicePath->SubType == HW_VENDOR_DP 
     && guidcmp(&AmiMaskedDevicePathGuid, &((VENDOR_DEVICE_PATH*)TmpDevicePath)->Guid) == 0 )
    {

        do{
 
            if( TmpDevicePath->Type == MEDIA_DEVICE_PATH 
             && TmpDevicePath->SubType == MEDIA_HARDDRIVE_DP )
            {
                *(SrcDevicePath) = TmpDevicePath;
                break;
            }
            else
            {
                DevicePathSize -= (*(UINT16*)&(TmpDevicePath)->Length[0]);
                TmpDevicePath = NEXT_NODE( TmpDevicePath );
            }

        }while( DevicePathSize > 0 );
    }
}
                                                                 //<(EIP138397+)

/**
    Check whether input boot option number is created by OS.

    @param  BootOptionNumber   Number to check
    
    @retval  TRUE    Created by OS
    @retval  FALSE   Isn't created by OS
*/    
BOOLEAN IsOsCreatedBootOption(UINT16 BootOptionNumber)
{
    EFI_STATUS Status;
    EFI_GUID   EfiOsBootOptionNamesGuid = EFI_OS_BOOT_OPTION_NAMES_GUID;
    UINT16*    EfiOsBootOrder = NULL;
    UINTN      Size = 0;
    BOOLEAN    Ret = TRUE;

    Status = GetEfiVariable(
                L"EfiOsBootOrder",
                &EfiOsBootOptionNamesGuid,
                NULL,
                &Size,
                &EfiOsBootOrder);

    if(!EFI_ERROR(Status))
    {
        UINTN      i;

        Size = Size/sizeof(UINT16);
        for(i = 0; i < Size; i++)
        {
            if(EfiOsBootOrder[i] == BootOptionNumber)
            {
                Ret = FALSE;
                break;
            }
        }

        pBS->FreePool(EfiOsBootOrder);
    }

    return Ret;
}
/**
    Check whether the CD-Rom had os boot option without boot file.

    @param   Option  BootOption

    @retval  FALSE   With boot file or not CD-Rom
    @retval  TRUE    Without boot file

**/
BOOLEAN IsCDRomWithoutBootFile(BOOT_OPTION *Option)
{
    
    EFI_DEVICE_PATH_PROTOCOL *Dp = Option->FilePathList ;

    if(GetDevicePathNodeByTypeSubtype(Dp, MEDIA_DEVICE_PATH, MEDIA_CDROM_DP))
    {
        EFI_STATUS Status;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystem = NULL;
        EFI_FILE_PROTOCOL *File= NULL;

        Status = pBS->HandleProtocol (
                Option->DeviceHandle,
                &gEfiSimpleFileSystemProtocolGuid,
                &SimpleFileSystem);

        if(!EFI_ERROR(Status))
        {
            Status = SimpleFileSystem->OpenVolume (
                        SimpleFileSystem,
                        &File);

            if(!EFI_ERROR(Status))
            {
                EFI_FILE_PROTOCOL* NewFile;

                Status = File->Open (
                          File,
                          &NewFile,
                          EFI_BOOT_FILE_NAME,
                          EFI_FILE_MODE_READ,
                          0);

                if(!EFI_ERROR(Status))
                {
                    NewFile->Close(NewFile);
                    return FALSE;
                }
                else if(Status == EFI_NOT_FOUND)
                {
                    return TRUE;
                }
            }
        }else
            return TRUE;
    }
    return TRUE;
}

/**
    Check whether the uefi os boot option without boot file.

    @param Dp   Device path to check

    @retval  TRUE    Find out the boot file.
    @retval  FALSE   Doesn't find out the boot file.

**/
BOOLEAN IsBootOptionWithoutBootFile(EFI_DEVICE_PATH_PROTOCOL *Dp)
{
    if(Dp)
    {
        EFI_HANDLE DevHandle = GetGptPartitionHandle(Dp);

        if(DevHandle)
        {
            EFI_STATUS Status;
            EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SimpleFileSystem = NULL;
            EFI_FILE_PROTOCOL *File= NULL;

            Status = pBS->HandleProtocol (
                    DevHandle,
                    &gEfiSimpleFileSystemProtocolGuid,
                    &SimpleFileSystem);

            if(!EFI_ERROR(Status))
            {
                Status = SimpleFileSystem->OpenVolume (
                            SimpleFileSystem,
                            &File);

                if(!EFI_ERROR(Status))
                {
                    EFI_FILE_PROTOCOL* NewFile;

                    Status = File->Open (
                              File,
                              &NewFile,
                              EFI_BOOT_FILE_NAME,
                              EFI_FILE_MODE_READ,
                              0);

                    if(!EFI_ERROR(Status))
                    {
                        NewFile->Close(NewFile);
                    }
                    else if(Status == EFI_NOT_FOUND)
                    {
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}


/**
    Read part of information from boot options into SIMPLE_BOOT_OPTION structure.

    @param  Options   To get the simple boot options

    @retval  UINT16   Number of simple options
**/
UINT16 GetSimpleBootOptions(SIMPLE_BOOT_OPTION **Options)
{
    EFI_STATUS  Status;
    UINT16      i;
    UINTN       OrderSize;
    UINT16*     Order = NULL;

    Status = GetEfiVariable(
                L"BootOrder", &EfiVariableGuid, NULL, &OrderSize, (VOID**)&Order);

    if(!EFI_ERROR(Status))
    {
        UINT16 OptionIndex = 0;
        UINT16 OrderCount;
        SIMPLE_BOOT_OPTION *SimpleBootOptions;

        OrderCount = (UINT16) OrderSize / sizeof(OrderCount);
        SimpleBootOptions = (SIMPLE_BOOT_OPTION*) MallocZ(sizeof(SIMPLE_BOOT_OPTION) * OrderCount);
        if(!SimpleBootOptions) return 0;
        
        for (i = 0; i < OrderCount; i++)
        {
            EFI_DEVICE_PATH_PROTOCOL*   Dp;
            CHAR16                      BootStr[9];
            UINT32*                     OptionalData;
            UINTN                       DescriptionSize;
            UINTN                       OptionalDataSize;

            EFI_LOAD_OPTION*            NvramOption;
            UINTN                       NvramOptionSize;
            SIMPLE_BOOT_OPTION*         Option;
               

            NvramOption = NULL;
            UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", Order[i]);
            Status = GetEfiVariable(
                       BootStr, &EfiVariableGuid, NULL, &NvramOptionSize, (VOID**)&NvramOption);
            if (EFI_ERROR(Status)) continue;
            DEBUG((DEBUG_BM | DEBUG_VERBOSE,"GetSimpleBootOptions :: Get Boot Option Boot%04X\n", Order[i]));

            DescriptionSize = (Wcslen((CHAR16*)(NvramOption+1))+1)*sizeof(CHAR16);
            Dp = (EFI_DEVICE_PATH_PROTOCOL*)((UINT8*)(NvramOption+1)+DescriptionSize);

            OptionalData =  (UINT32*)( (UINT8*)Dp + NvramOption->FilePathListLength);
            OptionalDataSize =  (UINT8*)NvramOption + NvramOptionSize - (UINT8*)OptionalData;

            RemoveAmiMaskDevicePath(&Dp, NvramOption->FilePathListLength);

            Option = &SimpleBootOptions[OptionIndex++];
            Option->NvramOption = NvramOption;
            Option->Size = NvramOptionSize;
            Option->Dp = Dp;
            Option->Num = Order[i];
            Option->Desc = (CHAR16*)(NvramOption+1) ; // It's not aligned on a 16-bit boundary

            if(OptionalDataSize >= sizeof(UINT32)
                && (ReadUnaligned32(OptionalData) == AMI_SIMPLE_BOOT_OPTION_SIGNATURE
                    || ReadUnaligned32(OptionalData) == AMI_GROUP_BOOT_OPTION_SIGNATURE) )
            {
                Option->Fw = TRUE;
            }

            if (OptionalDataSize == sizeof(LINUX_FIRMWARE_UPDATE_FILE)
                    && !MemCmp(OptionalData, LINUX_FIRMWARE_UPDATE_FILE, OptionalDataSize))
            {
                Option->Attr = LINUX_FIRMWARE_UPDATE_BOOT_OPTION;
            }
        }

        pBS->FreePool((VOID*)Order);

        if(OptionIndex)
        {
            *Options = SimpleBootOptions;
            return OptionIndex;
        }
    }

    return 0;
}

/**
    Remove boot option without uefi boot file in partition.

    @param  Options       Boot Options to check
    @param  OptionCount   Number of boot options
**/
VOID RemoveBootOptionWithoutUefiBootFileInPartition(SIMPLE_BOOT_OPTION *Options, UINTN OptionCount)
{
    if(Options && OptionCount)
    {
        UINTN i;

        for (i = 0; i < OptionCount; i++)
        {
            if(IsBootOptionWithoutBootFile(Options[i].Dp))
            {
                CHAR16  BootStr[9];

                //Clear variable Boot####
                UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", Options[i].Num);
                pRS->SetVariable(
                    BootStr, &EfiVariableGuid,
                    BOOT_VARIABLE_ATTRIBUTES, 0, NULL);

                Options[i].Invalid = TRUE;

                DEBUG((DEBUG_BM | DEBUG_VERBOSE, "RemoveBootOptionWithoutUefiBootFileInPartition :: BootOptionNumber(%X) Deleted\n", Options[i].Num));
            }
        }
    }
}

/**
    
    Within old version of Windows, if the nvram has orphan Windows's boot variable and current boot variable does not belong Windows, 
    Windows will put its BootXXXX in front of BootOrder for S4 return.
    But new version(RS3) didn't do that in all condition. So the function have to adjust boot option sequence for S4 return.

    @param  Options       Boot Options to check
    @param  OptionCount   Number of boot options
**/
VOID AdjustWindowsS4ResumeBootSequence(SIMPLE_BOOT_OPTION *Options, UINTN OptionCount)
{
    EFI_BOOT_MODE   BootMode ;
    UINTN           i, j;
    
    BootMode = GetBootMode() ;
    if (BootMode != BOOT_ON_S4_RESUME) return ;

    for (i = 0; i < OptionCount ; i++)
    {
        SIMPLE_BOOT_OPTION          *SrcOpt;
        EFI_DEVICE_PATH_PROTOCOL    *SrcDp;
        EFI_HANDLE                   GptHandle;
        
        SrcOpt = &Options[i];
        if (SrcOpt->Invalid) continue;
        
        // Only check NonFw and Windows boot variable
        if (Wcscmp(SrcOpt->Desc, L"Windows Boot Manager") != 0 || SrcOpt->Fw) continue ;
        
        SrcDp = GetDevicePathNodeByTypeSubtype(SrcOpt->Dp, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
        if (!SrcDp) continue;
            
        DEBUG((DEBUG_BM | DEBUG_VERBOSE, "%a :: BootOptionNumber(%X)\n", __FUNCTION__, SrcOpt->Num));
        GptHandle = GetGptPartitionHandle(SrcDp);
        DEBUG((DEBUG_BM | DEBUG_VERBOSE, "%a :: GptHandle=%d\n", __FUNCTION__, GptHandle));
        if (GptHandle == NULL) continue ;
        
        for (j = 0 ; j < i ; j++)
        {
            SIMPLE_BOOT_OPTION  *DstOpt;

            DstOpt = &Options[j];
            if(DstOpt->Invalid) continue;
            
            DEBUG((DEBUG_BM | DEBUG_VERBOSE, "%a :: Search BootOptionNumber(%x)\n", __FUNCTION__, DstOpt->Num));
            
            if (CompareHddDevicePath(SrcDp, DstOpt->Dp) == EFI_SUCCESS)
            {
                // Swap boot option, put OS variable in front.
                SIMPLE_BOOT_OPTION  Tmp ;
                
                DEBUG((DEBUG_BM | DEBUG_VERBOSE, "%a :: Option:%x/Option:%x Swap\n", __FUNCTION__, DstOpt->Num, SrcOpt->Num));
                MemCpy(&Tmp, DstOpt, sizeof(SIMPLE_BOOT_OPTION)) ;
                MemCpy(DstOpt, SrcOpt, sizeof(SIMPLE_BOOT_OPTION)) ;
                MemCpy(SrcOpt, &Tmp, sizeof(SIMPLE_BOOT_OPTION)) ;
                break ;
            }
        }
    }
}

/**
    Check if the reserved boot option.

    @param SrcOpt  The source option
    @param DstOpt  The destination option

    @return TRUE   DstOpt points to a reserved boot option.
    @return FALSE  DstOpt points to a non-reserved boot option.
**/
BOOLEAN IsReservedBootOption(SIMPLE_BOOT_OPTION *SrcOpt, SIMPLE_BOOT_OPTION *DstOpt)
{
    if (IsAttribute(SrcOpt->Attr, LINUX_FIRMWARE_UPDATE_BOOT_OPTION))
    {
        if (IsAttribute(DstOpt->Attr, LINUX_FIRMWARE_UPDATE_BOOT_OPTION))
            return FALSE;

        return TRUE;
    }

    if (IsAttribute(DstOpt->Attr, LINUX_FIRMWARE_UPDATE_BOOT_OPTION))
        return TRUE;


    return FALSE;
}

/**
    Check whether the OS create boot option Hidden.

    @param  Option    Boot option to check

    @retval  TRUE   Is Hidden
    @retval  FALSE  Not Hidden
**/
BOOLEAN IsOsCreateBootOptionHidden(SIMPLE_BOOT_OPTION* Option1 , SIMPLE_BOOT_OPTION* Option2)
{
    EFI_STATUS Status;
    CHAR16 BootStr[9];
    UINTN NvramOptionSize1;
	UINTN NvramOptionSize2;
    EFI_LOAD_OPTION *NvramOption1 = NULL;
    EFI_LOAD_OPTION *NvramOption2 = NULL;
    
    UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", Option1->Num);
    Status=GetEfiVariable(
            BootStr, 
            &gEfiGlobalVariableGuid, 
            NULL, 
            &NvramOptionSize1,
            (VOID**)&NvramOption1);
    if(EFI_ERROR(Status)){
        return FALSE;
    }
    
    UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", Option2->Num);
    Status=GetEfiVariable(
            BootStr, 
            &gEfiGlobalVariableGuid, 
            NULL, 
            &NvramOptionSize2,
            (VOID**)&NvramOption2);
    if(EFI_ERROR(Status)){
        if( NvramOption1 != NULL )
            pBS->FreePool(NvramOption1);
        return FALSE;
    }

    if( (NvramOption1->Attributes  & LOAD_OPTION_HIDDEN) == LOAD_OPTION_HIDDEN || 
		((NvramOption2->Attributes  & LOAD_OPTION_HIDDEN) == LOAD_OPTION_HIDDEN ) ) {
        pBS->FreePool(NvramOption1);
        pBS->FreePool(NvramOption2);
        return TRUE;
    }
    pBS->FreePool(NvramOption1);
    pBS->FreePool(NvramOption2);
    return FALSE;
}

/**
    if UEFI OS is existed, kill the duplicated UEFI OS boot options into one and move it to 
    one of their boot order position, and kill all UEFI OS boot options if deivces are disappeared
    in system.
*/
VOID AdjustEfiOsBootOrder(VOID)
{
    UINT16  BootIndex=0;
    BOOLEAN UpdateBootOrder=FALSE;
    UINTN i, j;

    SIMPLE_BOOT_OPTION *Options;
    UINT16             BootOrderCount;

    BootOrderCount = GetSimpleBootOptions(&Options);
    if (!BootOrderCount) return;

    //
    //  Elinks before doing the adjust
    //
    for (i = 0; AdjustEfiOsBootOrderFunctions[i]; i++)
        AdjustEfiOsBootOrderFunctions[i](Options, BootOrderCount);

    for (i=0; i<BootOrderCount; i++)
    {
        CHAR16                      BootStr[9];
        SIMPLE_BOOT_OPTION*         SrcOpt;
        EFI_DEVICE_PATH_PROTOCOL*   SrcDp;
        EFI_HANDLE                  GptHandle;
        
        SrcOpt = &Options[i];
        if( SrcOpt->Invalid ) continue;                   //(EIP138397)   

        SrcDp = GetDevicePathNodeByTypeSubtype(SrcOpt->Dp, MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP);
        if (!SrcDp) continue;

        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: BootOptionNumber(%X)\n", SrcOpt->Num));
        GptHandle = GetGptPartitionHandle(SrcDp);
        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: GptHandle=%d\n", GptHandle));
    
        if ( GptHandle != NULL)
        {
            // Only Non Fw Boot Option Kill Other Boot Option
            // This module check duplicate Boot Option exist or not when create boot option
            // So, ignore Fw Boot Option
            if(SrcOpt->Fw)
                continue;
            
            for (j = 0 ; j< BootOrderCount ; j++)
            {
                SIMPLE_BOOT_OPTION *DstOpt;

                DstOpt = &Options[j];
                if( DstOpt->Invalid || (i == j)) continue;

                DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: Search BootOptionNumber(%x)\n", DstOpt->Num));
                
#if KeepDuplicateNonFWBootOption
                if(!DstOpt->Fw)
                    continue;
#endif
                if ( CompareHddDevicePath(SrcDp, DstOpt->Dp))
                {
                    UINTN  ReserveIndex;
                    UINTN  DeleteIndex; 

                    if (IsReservedBootOption(SrcOpt, DstOpt))
                    {
                        DEBUG((DEBUG_BM | DEBUG_VERBOSE, "AdjustEfiOsBootOrder :: continue for the reserved boot option\n"));
                        continue;
                    }

                    DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: Matched BootOptionNumber(%x)\n", DstOpt->Num));
					//If we still work under FBO it will create multiple duplicate bootoption for every boot.
					//Because "BootOption->Attributes" won't work in FBO, FBO use it's own variable to check options.
					//Ex:UefiDevOrder and FboLegacyOrder 
					//It will also overwrite the "BootOption->Attributes" to sync with it's own variable.
#if !FIXED_BOOT_ORDER_SUPPORT
                    if ( IsOsCreateBootOptionHidden(SrcOpt, DstOpt) ) continue ;
#endif  
                    if(!DefaultFwBootOption && !IsOsCreatedBootOption(SrcOpt->Num))
                    {
                        //
                        //  BootOrder[i] is not created by OS, but BootOrder[j]
                        //
                        ReserveIndex = j;
                        DeleteIndex = i;
                    }
                    else
                    {
                        ReserveIndex = i;
                        DeleteIndex = j;
                    }
                    //Delete EfiOsBootOptionNames boot option.												
                    //Clear variable Boot####
                    UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", Options[DeleteIndex].Num);
                    pRS->SetVariable(
                        BootStr, &EfiVariableGuid,
                        BOOT_VARIABLE_ATTRIBUTES, 0, NULL);

                    //BootOrder_Flag[DeleteIndex] = 1;
                    Options[DeleteIndex].Invalid = TRUE;
                    UpdateBootOrder = TRUE;

#if FIXED_BOOT_ORDER_SUPPORT
                    // Handle Fixed Boot Order Priority issue.
                    {
                        UINTN               DevOrderSize = 0;
                        UEFI_DEVICE_ORDER   *DevOrder = NULL, *DevOrder2 = NULL;
                        EFI_GUID            gFixedBootOrderGuid = FIXED_BOOT_ORDER_GUID;
                        UINTN               k = 0;
                        UINT32              Attrib = 0;
                        EFI_STATUS          Status;
                        
                        Status = GetEfiVariable(L"UefiDevOrder", &gFixedBootOrderGuid, &Attrib, &DevOrderSize, &DevOrder);
                        if(!EFI_ERROR(Status))
                        {
                            DevOrder2 = DevOrder;
                            
                            for (DevOrder = DevOrder2
                                ; (UINT8*)DevOrder < (UINT8*)DevOrder2 + DevOrderSize
                                ; DevOrder = (UEFI_DEVICE_ORDER*)((UINT8*)DevOrder + DevOrder->Length + sizeof(DevOrder->Type)))
                            {
                                for (k = 0; k < DEVORDER_COUNT(DevOrder); k++)
                                {
                                    DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: DevOrder->Device[%x] %x,Options[%x] = %x \n", k, DevOrder->Device[k], DeleteIndex, Options[DeleteIndex].Num));
                                    if(DevOrder->Device[k] == Options[DeleteIndex].Num)
                                    {
                                        DevOrder->Device[k] = Options[ReserveIndex].Num;
                                        DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: Options[%x].Num %x \n", ReserveIndex, Options[ReserveIndex].Num));
                                    }
                                }
                            }
                            Status = pRS->SetVariable(L"UefiDevOrder", &gFixedBootOrderGuid, Attrib, DevOrderSize, DevOrder2);
                            pBS->FreePool(DevOrder2); 
                        }
                    }
#endif         
                }

            } //for (j=0; j<BootOrderSize/sizeof(UINT16); j++)
        } //if ( GptHandle != NULL)
        else
         //GPT HDD NOT FOUND, Remove This BootOrder
        {
            if ( (((HARDDRIVE_DEVICE_PATH*)SrcDp)->MBRType == MBR_TYPE_EFI_PARTITION_TABLE_HEADER) &&
                    SrcOpt->Fw)
            {
                UnicodeSPrint(BootStr, sizeof(BootStr), L"Boot%04X", SrcOpt->Num);
                //Clear variable Boot####
                pRS->SetVariable(
                   BootStr, &EfiVariableGuid,
                   BOOT_VARIABLE_ATTRIBUTES, 0, NULL);

                UpdateBootOrder = TRUE;
                //BootOrder_Flag[i] = 1;
                Options[i].Invalid = TRUE;
            }
        }
    }

    if ( UpdateBootOrder )
    {
        UINT16* NewBootOrder;

        DEBUG((DEBUG_BM | DEBUG_INFO | DEBUG_VERBOSE,"AdjustEfiOsBootOrder :: Update BootOrder, Dump New BootOrder \n\t"));

        NewBootOrder = MallocZ( BootOrderCount * sizeof(UINT16));
        if(NewBootOrder)
        {
            for(i = 0 ; i < BootOrderCount ; i++)
            {
                if(Options[i].Invalid)
                    continue;
                
                DEBUG((DEBUG_BM | DEBUG_INFO | DEBUG_VERBOSE,"%x ",Options[i].Num));
                NewBootOrder[BootIndex++] = Options[i].Num;
            }
            DEBUG((DEBUG_BM | DEBUG_INFO | DEBUG_VERBOSE,"\n"));
            
            pRS->SetVariable(
                L"BootOrder", &EfiVariableGuid,
                BOOT_VARIABLE_ATTRIBUTES, BootIndex * sizeof(UINT16), NewBootOrder);

            pBS->FreePool(NewBootOrder);
        }
    }

    for(i = 0; i < BootOrderCount; i++)
        pBS->FreePool(Options[i].NvramOption);
    pBS->FreePool(Options);
}

#if (CSM_SUPPORT == 1) && (RemoveLegacyGptHddDevice == 1)
/**
    Determine whether boot device is a UEFI HDD(GPT format).

    @param  Device  Boot device to be checked.

    @retval  TRUE   Boot Device is a UEFI HDD.
    @retval  FALSE  Boot Device is not a UEFI HDD.
*/
BOOLEAN RemoveLegacyGptHdd(BOOT_DEVICE *Device) {
    EFI_BLOCK_IO_PROTOCOL *BlkIo;
    EFI_STATUS Status;
    UINT8 *Buffer = NULL;
    UINTN index;
    PARTITION_ENTRY *pEntries;

    if (   Device->DeviceHandle == INVALID_HANDLE
            || Device->BbsEntry == NULL
       ) return FALSE;

    if ( Device->BbsEntry->DeviceType != BBS_HARDDISK ) return FALSE;

    Status=pBS->HandleProtocol(
               Device->DeviceHandle, &gEfiBlockIoProtocolGuid, &BlkIo
           );

    if (EFI_ERROR(Status) || BlkIo->Media->RemovableMedia) return FALSE;	//USB device?

    Status = pBS->AllocatePool( EfiBootServicesData, BlkIo->Media->BlockSize, &Buffer );
    if ( Buffer == NULL ) return FALSE;

    // read the first sector
    BlkIo->ReadBlocks ( BlkIo,
                        BlkIo->Media->MediaId,
                        0,
                        BlkIo->Media->BlockSize,
                        (VOID*)Buffer);

    if(Buffer[0x1fe]==(UINT8)0x55 && Buffer[0x1ff]==(UINT8)0xaa)	//MBR Signature
    {
        pEntries=(PARTITION_ENTRY *)(Buffer+0x1be);

        for (index=0; index<4; index++)
        {
            if ( pEntries[index].PartitionType == 0xee) 	//Check GPT Partition?
            {
                pBS->FreePool( Buffer );
                return TRUE;			//Set Can't Boot.
            }
        } //for(index=0;index<4;index++)
    }//if(Buffer[0x1fe] == 0x55 && Buffer[0x1ff] == 0xaa)

    pBS->FreePool( Buffer );
    return FALSE;
}
#endif

/**
    Check whether the target boot option by NameMap.

    @param  Option    Boot option to check
    @param  NameMap   Used to check the boot option

    @retval  TRUE   Is the target boot option
    @retval  FALSE  Isn't the target boot option
**/
BOOLEAN IsMatchedBootOptionByNameMap(BOOT_OPTION* Option, NAME_MAP* NameMap)
{
    if(Option && NameMap)
    {
        // check option name length, if OptionName != FileNameMaps, don't skip boot option
        if ( Wcslen(Option->Description) == Wcslen(NameMap->BootOptionName))
        {
            // check option string
            if ( !MemCmp( Option->Description, NameMap->BootOptionName, StrSize(NameMap->BootOptionName)) )
            {
                // check option device path.
                EFI_DEVICE_PATH_PROTOCOL *Dp = Option->FilePathList ;
                BOOLEAN HardDriveFlag=FALSE, MediaFilePathFlag=FALSE ;
                while(!isEndNode(Dp))
                {
                    if (Dp->Type == MEDIA_DEVICE_PATH && Dp->SubType == MEDIA_HARDDRIVE_DP)
                        HardDriveFlag = TRUE ;
                    if (Dp->Type == MEDIA_DEVICE_PATH && Dp->SubType == MEDIA_FILEPATH_DP)
                        MediaFilePathFlag = TRUE ;
                    Dp = NEXT_NODE(Dp) ;
                }
                
                if (HardDriveFlag && MediaFilePathFlag)
                {
                    DEBUG((DEBUG_BM | DEBUG_VERBOSE,"IsMatchedBootOptionByNameMap :: %S don't normalize.\n", Option->Description )) ;
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

/**
    Check whether uefi os boot option by boot file(ex.BootX64.efi).

    @param  Option   Boot option to check

    @retval  TRUE   Match the map
    @retval  FALSE  Doesn't match the map
**/
BOOLEAN CheckUefiOsBootOptionByUefiBootFile(BOOT_OPTION* Option)
{
    if(Option)
    {
        STATIC NAME_MAP NameMap = {EFI_BOOT_FILE_NAME, NAME_OF_UEFI_OS};

        if(IsMatchedBootOptionByNameMap(Option, &NameMap))
            return TRUE;
    }

    return FALSE;
}

/**
    Check whether uefi os boot option by map (FILE_NAME_MAPS).

    @param  Option   Boot option to check

    @retval  TRUE   Match the map
    @retval  FALSE  Doesn't match the map
**/
BOOLEAN CheckUefiOsBootOptionByMap(BOOT_OPTION* Option)
{
    if(Option)
    {
        UINTN i;

        for(i = 0; FILE_NAME_MAPS[i].FilePath; i++)
        {
            if(IsMatchedBootOptionByNameMap(Option, &FILE_NAME_MAPS[i]))
                return TRUE;
        }
    }

    return FALSE;
}

/**
    Check whether specified uefi os boot option.

    @param  Option   boot option to check

    @retval  TRUE    Is specified uefi os boot option
    @retval  FALSE   Isn't specified uefi os boot option
*/
BOOLEAN IsSpecifiedUefiOsBootOption(BOOT_OPTION *Option)
{
    if(Option && !IsLegacyBootOption(Option))
    {
        UINTN i;

        for (i = 0; CheckUefiOsBootOptionFunctions[i]; i++)
        {
            if(CheckUefiOsBootOptionFunctions[i](Option))
                return TRUE;
        }
    }

    return FALSE;
}

/**
    If normalization is enabled, regenerates all the description strings
    and/or file path lists.
    This function override BDS kernel, because Uefi Os Boot Option can't
    be normalize file path and names.
*/
VOID EfiOsName_NormalizeBootOptions(){
    DLINK *Link;
    BOOT_OPTION *Option;

    // Normalize boot options 
    //(regenerate the description string and the file path list)
    FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option){

        if (   !Option->FwBootOption || !IsBootOptionWithDevice(Option)
            || Option->BootOptionNumber == INVALID_BOOT_OPTION_NUMBER
        ) continue;
        
        // Skip Uefi Hdd Os boot option
        if (IsSpecifiedUefiOsBootOption(Option)) continue ;
#if CREATE_EFI_OS_BOOT_OPTIONS_FOR_CDROM 
        // If the CD Rom remove boot file we normalize it.
        if (!IsCDRomWithoutBootFile(Option)) continue ;
#endif
        if (NormalizeBootOptionDevicePath){
            EFI_DEVICE_PATH_PROTOCOL *OldFilePathList = Option->FilePathList;
            UINTN OldFilePathListLength = Option->FilePathListLength;
            Option->FilePathList = NULL;
            Option->FilePathListLength = 0;
            BuildBootOptionFilePath(Option);
            if (Option->FilePathList == NULL){
                Option->FilePathList = OldFilePathList;
                Option->FilePathListLength = OldFilePathListLength;
            }else if (OldFilePathList != NULL){
                pBS->FreePool(OldFilePathList);
            }
        }
        if (NormalizeBootOptionName){
            CHAR16 *OldDescription = Option->Description;
            Option->Description = NULL;
            ConstructBootOptionName(Option);
            if (Option->Description == NULL) 
                Option->Description = OldDescription;
            else if (OldDescription != NULL) 
                pBS->FreePool(OldDescription);
        }
    }
}

/**
    Adjust new UEFI OS boot option priority.
    Follow the policy NEW_UEFI_OS_OPTION_ORDER_POLICY to change priority with new
    UEFI OS boot option.
    
*/
VOID
AdjustNewUefiOsOptionPriority()
{
    EFI_STATUS  Status;
    DLINK       *Link;   
    BOOT_OPTION *Option;
    UINTN     Size;
    UINTN     x = 0;
    UINT32    Priority = 0;
    UINT8     NewOptionPolicy;
    EFI_GUID  EfiOsBootOptionNamesGuid = EFI_OS_BOOT_OPTION_NAMES_GUID;
    
    DEBUG((DEBUG_BM | DEBUG_VERBOSE,"AdjustNewUefiOsOptionPriority :: Update new Uefi OS option priority\n")) ;

    // if no new EFI OS boot option added, just return. 
    if (!NewEfiOsOptionDpListCount) return ;
	
	Size = sizeof (UINT8);
    Status = pRS->GetVariable (
              L"NewOptionPolicy",
              &EfiOsBootOptionNamesGuid,
              NULL,
              &Size,
              (VOID *) &NewOptionPolicy
              );
    
    if(EFI_ERROR(Status))
    {
        NewOptionPolicy = NEW_UEFI_OS_OPTION_ORDER_POLICY;
    }
        
    if (NewOptionPolicy == 0)
    {
        pBS->FreePool(NewEfiOsOptionDpList) ;
        return;
    }
    else if (NewOptionPolicy == 1)
    {
        Priority = LOWEST_BOOT_OPTION_PRIORITY ;
        FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option)
        {
            if (Option->Priority < Priority)
                Priority = Option->Priority ;
            Option->Priority += (UINT32)NewEfiOsOptionDpListCount ;
        }   
    }
    else if (NewOptionPolicy == 2)
    {
        Priority = 0 ;
        FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option)
        {
            if (Option->Priority > Priority)
                Priority = Option->Priority + 1  ;
        }
    }
    
    for ( x=0 ; x<NewEfiOsOptionDpListCount ; x++ )
    {
        EFI_DEVICE_PATH_PROTOCOL *Dp;
        
        Dp = *(NewEfiOsOptionDpList+x);
        FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option)
        {
            if (Option->FilePathList == Dp)
                Option->Priority = Priority++ ;
        }
    } 
    pBS->FreePool(NewEfiOsOptionDpList) ;
}

/**
    Save the boot order to check later, 
    then we can know which one is created by OS.
*/
VOID SaveEfiOsBootOrder()
{
    EFI_STATUS Status;
    UINTN      BootOrderSize = 0;
    UINT16*    BootOrder = NULL;

    Status = GetEfiVariable(
                L"BootOrder", 
                &EfiVariableGuid, 
                NULL, 
                &BootOrderSize, 
                &BootOrder);

    if(!EFI_ERROR(Status))
    {
        EFI_GUID  EfiOsBootOptionNamesGuid = EFI_OS_BOOT_OPTION_NAMES_GUID;

        pRS->SetVariable(
            L"EfiOsBootOrder", 
            &EfiOsBootOptionNamesGuid,
            EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
            BootOrderSize, 
            (VOID*)BootOrder);

        pBS->FreePool(BootOrder);
    }
}

/**
    Attempt to match the passed device path to the passed boot device.

    PartitionDevicePathTest call only checks the partition information, 
    but this function also checks whether boot file is existed or not.

    @param  OptionDevicePath   The device path to check
    @param  Device             The boot device

    @retval TRUE    The device path matches the device
    @retval FALSE   The device path does not match the device
*/
BOOLEAN EobonPartitionDevicePathTest(EFI_DEVICE_PATH_PROTOCOL* OptionDevicePath, BOOT_DEVICE* Device)
{
    EFI_STATUS                      Status;
    EFI_FILE_PROTOCOL               *File;
    EFI_FILE_PROTOCOL               *NewFile;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFileSystem;
    EFI_DEVICE_PATH_PROTOCOL        *Dp;
    
    if(!PartitionDevicePathTest(OptionDevicePath, Device))
        return FALSE;

    Dp = GetDevicePathNodeByTypeSubtype(OptionDevicePath, MEDIA_DEVICE_PATH, MEDIA_FILEPATH_DP);
    if (!Dp) return FALSE;
    
    Status = pBS->HandleProtocol (
                 Device->DeviceHandle,
                 &gEfiSimpleFileSystemProtocolGuid,
                 &SimpleFileSystem);
    if(EFI_ERROR(Status)) return FALSE;

    Status = SimpleFileSystem->OpenVolume (
                 SimpleFileSystem,
                 &File);
    if(EFI_ERROR(Status)) return FALSE;
    
    Status = File->Open (
              File,
              &NewFile,
              ((FILEPATH_DEVICE_PATH*)Dp)->PathName,
              EFI_FILE_MODE_READ,
              0);
    if(EFI_ERROR(Status)) return FALSE;

    NewFile->Close(NewFile);

    return TRUE;
}
