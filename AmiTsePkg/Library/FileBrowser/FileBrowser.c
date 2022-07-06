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

/**
 * @file FileBrowser.c
 * @brief File Explorer library API implementation
 */

#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiString.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/DevicePath.h>
#include <Protocol/EsaFileBrowser.h>


#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>

#include "FileBrowser.h"

EFI_HII_HANDLE HiiHandle;
EFI_HII_PACKAGE_LIST_HEADER *PackageList = NULL;
EFI_HII_DATABASE_PROTOCOL *HiiDatabase;
EFI_HII_STRING_PROTOCOL *HiiString;
AMI_POST_MANAGER_PROTOCOL *AmiPostMgr;
AMI_ESA_FILE_BROWSER_PROTOCOL *EsaFileBrowser;

CHAR16 *FileTypes;

extern UINT8 AmiFileExplorerLibStrings[];

/**
 * @brief Add new string to HII database
 *
 * @param str Pointer to string to add
 *
 * @return String ID
 */
UINT16 NewString (CHAR16 *str)
{
	EFI_STATUS st;
	UINT16 t;
	st = HiiString->NewString (HiiString, HiiHandle, &t, "en-US", NULL, str, NULL);
	return (st) ? STRING_TOKEN(STR_ERROR) : t;
}

/**
 * @brief Replace string content in HII database
 *
 * @param str Pointer to new content
 * @param token String ID of string to replace
 */
VOID SetString (CHAR16 *str, UINT16 token)
{
	HiiString->SetString (HiiString, HiiHandle, token, "en-US", str, NULL);
}

/**
 * @brief Delete newly added strings from database
 */
VOID ResetStrings (VOID)
{
	HiiDatabase->UpdatePackageList (HiiDatabase, HiiHandle, PackageList);
}

/**
 * @brief Create device path for given device and filename
 *
 * @param hnd Handle of device
 * @param FileName Pointer to filename
 *
 * @return Pointer to device path
 */
EFI_DEVICE_PATH_PROTOCOL * CreateDevicePath (EFI_HANDLE hnd, CHAR16 *FileName)
{
	EFI_DEVICE_PATH_PROTOCOL *dp1;
	EFI_DEVICE_PATH_PROTOCOL *dp2;
	EFI_DEVICE_PATH_PROTOCOL *dp3;
	EFI_STATUS st;
	UINTN sz;
	
	st = gBS->HandleProtocol (hnd, &gEfiDevicePathProtocolGuid, (VOID **)&dp1);
	if (st)
		return NULL;
	
	sz = StrSize (FileName) + sizeof (EFI_DEVICE_PATH_PROTOCOL);
	dp2 = (EFI_DEVICE_PATH_PROTOCOL *)AllocatePool (sz);
	if (dp2 == NULL)
		return NULL;
	
	dp2->Type = MEDIA_DEVICE_PATH;
	dp2->SubType = MEDIA_FILEPATH_DP;
	SetDevicePathNodeLength (dp2, sz);
	CopyMem (dp2 + 1, FileName, StrSize (FileName));
	
	dp3 = AppendDevicePathNode (dp1, dp2);
	FreePool (dp2);
	return dp3;
}

/**
 * @brief Submit package list to HII database
 *
 * @return Status of operation
 */
EFI_STATUS SubmitPackageList (VOID)
{
	EFI_STATUS st;
	UINTN sz = 0;
	
	static EFI_GUID PackListGuid = 
           { 0x70e1a818, 0x0be1, 0x4449, {0xbf, 0xd4, 0x9e, 0xf6, 0x8c, 0x7f, 0x02, 0xa9 }};

	HiiHandle = HiiAddPackages (&PackListGuid, NULL, AmiFileExplorerLibStrings, NULL);
	if (HiiHandle == NULL)
		return EFI_OUT_OF_RESOURCES;
	
	st = HiiDatabase->ExportPackageLists (HiiDatabase, HiiHandle, &sz, PackageList);
	if (st == EFI_BUFFER_TOO_SMALL) {
    	    PackageList = AllocatePool (sz);
    	    st = HiiDatabase->ExportPackageLists (HiiDatabase, HiiHandle, &sz, PackageList);
	}
	return st;
}

/**
 * @brief File explorer API function
 *
 * @param RootDirectory Pointer to root directory
 * @param FileType Pointer to string containing file extensions to process
 * @param ChooseHandler Pointer to callback to pass choosen file
 * @param File Pointer to device path of choosen file
 *
 * @return Status of operation
 */
EFI_STATUS EFIAPI ChooseFile (
	IN  EFI_DEVICE_PATH_PROTOCOL  *RootDirectory,
	IN  CHAR16                    *FileType,  OPTIONAL
	IN  CHOOSE_HANDLER            ChooseHandler,  OPTIONAL
	OUT EFI_DEVICE_PATH_PROTOCOL  **File  OPTIONAL
        )
{
	EFI_STATUS st;
	DEVICE_MENU_ITEM *dev = NULL;
	CHAR16 *file = NULL;
	EFI_DEVICE_PATH_PROTOCOL *dp = NULL;

	st = gBS->LocateProtocol (&gEsaFileBrowserProtocolGuid, NULL, (VOID **)&EsaFileBrowser);
	if (!st)
	{
		st = EsaFileBrowser->ChooseFile(RootDirectory, FileType, ChooseHandler, File);
		return st;
	}
	
	st = gBS->LocateProtocol (&gEfiHiiDatabaseProtocolGuid, NULL, (VOID **)&HiiDatabase);
	if (st)
		return st;
	
	st = gBS->LocateProtocol (&gEfiHiiStringProtocolGuid, NULL, (VOID **)&HiiString);
	if (st)
		return st;

	st = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, (VOID **)&AmiPostMgr);
	if (st)
		return st;
	
	st = SubmitPackageList ();
	if (st)
		return st;
	
	if (FileType != NULL) {
		CHAR16 *tmp;
		FileTypes = AllocatePool (StrSize (FileType));
		tmp = FileTypes;
		while (*FileType != 0)
			*tmp++ = ToUpper (*FileType++);
		*tmp = 0;
	} else {
		FileTypes = NULL;
	}
	
	do {
		dev = (DEVICE_MENU_ITEM *) GetDeviceSelection ();
		if (dev == NULL)
			break;
		file = GetFileSelection (dev);
		
		if (file != NULL) {
			dp = CreateDevicePath (dev->Hnd, file);
			if (ChooseHandler != NULL) {
				BOOLEAN res;
				res = ChooseHandler (dp);
				if (res == FALSE) {	//have to stay within explorer
					FreePool (dp);
					file = NULL;
				}
			}
		}
		else
		    break;
	} while (file == NULL);
	
	HiiDatabase->RemovePackageList (HiiDatabase, HiiHandle);
	FreePool (PackageList);
	
	if (File != NULL && dp != NULL)
		*File = dp;
	
	if (file != NULL)
		FreePool (file);
	if (dev != NULL)
		FreePool (dev);
	if (FileTypes != NULL)
		FreePool (FileTypes);

	return (dp == NULL) ? EFI_NOT_FOUND : EFI_SUCCESS;
}

/**
 * @brief Return pointer to sorted file list
 *
 * @param Dir Pointer to directory, to return files from
 *
 * @return Pointer to sorted file list
 */
VOID * GetSortedFileList (
        VOID *Dir 
        )
{
	AMI_VECTOR *v;
	v = GetFileList (Dir);
	return v->buf;
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
