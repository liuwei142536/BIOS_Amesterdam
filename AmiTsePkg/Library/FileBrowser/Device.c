//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
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
  This file implements DEVICE file browser menu
  
**/

#include <Guid/FileSystemInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/DevicePath.h>
#include <Protocol/AMIPostMgr.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include "FileBrowser.h"
#include "Vector.h"

static UINT16 FilesystemIndex = 0;

extern EFI_HII_HANDLE HiiHandle;
extern AMI_POST_MANAGER_PROTOCOL *AmiPostMgr;
UINT64
EFIAPI
RShiftU64 (
    IN UINT64 Value,
    IN UINTN  Shift
    );

CHAR16 *message_types[] = {
    L"ATAPI",
    L"SCSI",
    L"FibreChannel",
    L"1394",
    L"USB",
    L"SATA",
    L"Unknown"
};

CHAR16 *media_types[] = {
    L"HDD",
    L"CDROM",
    L"Unknown"
};

/**
  This function returns Device messaging type name by id
  
  @param  id Device messaging type ID
  
  @return Pointer to Device messaging type name

**/
CHAR16 * get_message_name (UINT8 id)
{
    switch (id) {
        case 1:
            return message_types[0];
        case 2:
            return message_types[1];
        case 5:
        case 15:
        case 16:
            return message_types[4];
        case 18:
            return message_types[5];
        default:
            break;
    }
    return message_types[6];
}

/**
  This function returns Device media type name by id
  
  @param  id Device media type ID
  
  @return Pointer to Device media type name

**/
CHAR16 * get_media_name (UINT8 id)
{
    switch (id) {
        case 1:
            return media_types[0];
        case 2:
            return media_types[1];
        default:
            break;
    }
    return media_types[2];
}

/**
  This function returns volume name and size
  
  @param  fs Pointer to volume file system protocol
  @param vsz Pointer to store volume size
  
  @return Pointer to volume name

**/
CHAR16 * get_volume_name_size (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs, UINT64 *vsz)
{
    EFI_FILE_PROTOCOL *r;
    EFI_STATUS st;
    UINTN sz = 0;
    EFI_FILE_SYSTEM_INFO *info = NULL;
    CHAR16 *out = NULL;

    st = fs->OpenVolume (fs, &r);
    if (st) {
        *vsz = 0;
        return NULL;
    }
    st = r->GetInfo (r, &gEfiFileSystemInfoGuid, &sz, info);
    if (st == EFI_BUFFER_TOO_SMALL) {
        info = AllocatePool (sz);
        st = r->GetInfo (r, &gEfiFileSystemInfoGuid, &sz, info);
    }
    if (st) {
        *vsz = 0;
        return NULL;
    }
    *vsz = info->VolumeSize;
    if (info->VolumeLabel[0] != 0) {
        sz = StrSize (info->VolumeLabel);
        out = AllocatePool (sz);
        CopyMem (out, info->VolumeLabel, sz);
    }
    r->Close (r);
    FreePool (info);
    return out;
}

/**
  This function returns device path subtype for given type
  
  @param   dp Pointer to device path protocol
  @param type Device type
  
  @return Device subtype

**/
UINT8 get_dp_subtype (EFI_DEVICE_PATH_PROTOCOL *dp, UINT8 type)
{
    while (!IsDevicePathEndType (dp)) {
        UINT8 s;
        s = DevicePathType (dp);
        if (s == type)
            return DevicePathSubType (dp);
        dp = NextDevicePathNode (dp);
    }
    return 0xff;
}

/**
  This function copies Src name to Dst name shortening it if required
  
  @param   Dst Pointer to output string
  @param   Src Pointer to input string
  @param   MaxLen Max output string length
  
  @return Number of characters copied

**/
UINTN copy_name (CHAR16 *Dst, CHAR16* Src, UINTN MaxLen)
{
	UINTN len = StrLen (Src);
	if (len < MaxLen) { 
		CopyMem (Dst, Src, sizeof (CHAR16) * len);
		return len;
	}
	CopyMem (Dst, Src, sizeof (CHAR16) * (MaxLen - 3));
	CopyMem (&Dst[MaxLen - 3], L"...", sizeof (CHAR16) * 3);
	return MaxLen;
}

/**
  This function creates human-readable name from device path nodes
  
  @param   Dst Pointer to output string
  @param   dp  Pointer to device path protocol
  @param   MaxLen Max output string length
  
  @return Number of characters copied

**/
UINTN copy_dp_name (CHAR16 *Dst, EFI_DEVICE_PATH_PROTOCOL *dp, UINTN MaxLen)
{
    CHAR16 *ms, *md;
    UINTN lms, lmd;
    UINTN max = MaxLen * sizeof (CHAR16);	//max length in bytes
    ms = get_message_name (get_dp_subtype (dp, MESSAGING_DEVICE_PATH ));
    md = get_media_name (get_dp_subtype (dp, MEDIA_DEVICE_PATH));
    lms = StrLen (ms);
    lmd = StrLen (md);
    UnicodeSPrintAsciiFormat (Dst, max, "%s %s", md, ms);
   	return ((lms + lmd + 1) < MaxLen) ? lms + lmd + 1 : MaxLen;
}

/**
  This function creates human-readable volume size from given size
  
  @param   Dst Pointer to output string
  @param   sz  Volume size in bytes
  @param   MaxLen Max output string length

**/
VOID copy_size (CHAR16 *Dst, UINT64 sz, UINTN MaxLen) {
	UINTN max = MaxLen * sizeof (CHAR16);	//max length in bytes
	if (sz < 0x100000) {	//up to 1MB
UnicodeSPrintAsciiFormat (Dst, max, " %dKB",  RShiftU64(sz,10));
		
		
		return;
	}
	if (sz < 0x40000000) {  //up to 1GB
		UnicodeSPrintAsciiFormat (Dst, max, " %dMB", RShiftU64(sz,20));
		return;
	}
	if (sz < 0x10000000000) { //up to 1TB
		UnicodeSPrintAsciiFormat (Dst, max, " %dGB", RShiftU64(sz,30));
		return;
	}
	UnicodeSPrintAsciiFormat (Dst, max, " %dTB", RShiftU64(sz,40));
}

/**
  This function creates device list vector

  @return Vector with device elements

**/
AMI_VECTOR * GetDeviceList (VOID)
{
    EFI_STATUS  st = EFI_SUCCESS;
    EFI_HANDLE *hnd;
    UINTN n_hnd;
    UINTN i;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs;
    AMI_VECTOR * dev;

    st = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &n_hnd, &hnd);
    if (st || n_hnd == 0)
    	return NULL;
    
    dev = VectorInit ((UINT32)n_hnd, sizeof (DEVICE_MENU_ITEM), 1);
    
    for (i = 0; i < n_hnd; i++) {
        CHAR16 *vol;
        UINT64 vsz;
        DEVICE_MENU_ITEM *mi;
        UINTN l = 0;
        
        st = gBS->HandleProtocol (hnd[i], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&fs);
        if (st)
            continue;
        
        mi = AllocateZeroPool (sizeof (*mi));
        mi->Fs = fs;
        mi->Hnd = hnd[i];
        
        vol = get_volume_name_size (fs, &vsz);
        if (vol != NULL) {
        	l = copy_name (mi->Name, vol, 40);
            FreePool (vol);
        } else {
            EFI_DEVICE_PATH_PROTOCOL *dp;
            st = gBS->HandleProtocol (hnd[i], &gEfiDevicePathProtocolGuid, (VOID **)&dp);
            //To avoid showing unknown / Unsupported DP etc for NT32
            if((get_dp_subtype (dp, MESSAGING_DEVICE_PATH)==0xFF) &&(get_dp_subtype (dp, MEDIA_DEVICE_PATH)==0xFF))
                copy_name (mi->Name,L"Unknown", 18);
            else
                l = copy_dp_name (mi->Name, dp, 40);
            /*if(StrStr(mi->Name ,media_types[2]))  //Skip the filesystem if ??? value found
            {
                FreePool (mi);
                continue;
            }*/
            
        }
        copy_size (&mi->Name[l], vsz, 50 - l);
        VectorAdd (dev, mi);
        FreePool (mi);
    }
    
    gBS->FreePool (hnd);
    return dev;
}

/**
  This function returns user-selected device

  @return Pointer to user-selected device info

**/
VOID * GetDeviceSelection (VOID)
{
	EFI_STATUS st;
	AMI_VECTOR * dev;
	UINTN num_dev;
	UINTN i;
	POSTMENU_TEMPLATE *items, *p;
	UINT16 r_idx = 0;
	VOID *r = NULL;
	
	dev = GetDeviceList();
	if (dev == NULL)
		return NULL;
	
	num_dev = VectorGetSize (dev);
	items = p = AllocateZeroPool (sizeof (*items) * num_dev);
	
	for (i = 0; i < num_dev; i++, p++) {
		DEVICE_MENU_ITEM *mi = (DEVICE_MENU_ITEM *) VectorGetElement (dev, i);
		p->ItemToken = NewString (mi->Name);
	}
	
	st = AmiPostMgr->DisplayPostMenu (HiiHandle, 
			STRING_TOKEN(STR_DIALOG_TITLE),
			STRING_TOKEN(STR_DIALOG_LEGEND),
			items, (UINT16) num_dev, &r_idx);
	
	FilesystemIndex = r_idx;
	
	FreePool (items);
	if (!st) {
		r = AllocatePool (sizeof (DEVICE_MENU_ITEM));
		CopyMem (r, VectorGetElement (dev, r_idx), sizeof (DEVICE_MENU_ITEM));
	}
	
	VectorDestroy (dev);
	ResetStrings ();
	return r;
}

UINT16 GetFilesystemIndex()
{
    return FilesystemIndex;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
