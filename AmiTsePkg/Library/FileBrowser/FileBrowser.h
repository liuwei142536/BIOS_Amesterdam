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
  This file

**/
#ifndef __FILE_BROWSER__H__
#define __FILE_BROWSER__H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "Vector.h"
#include <Protocol/FileExplorer.h>

#define MAX_MENU_ITEM_NAME 50

typedef struct {
	EFI_HANDLE Hnd;
	VOID *Fs;
	UINT16 StrToken;
	CHAR16 Name[MAX_MENU_ITEM_NAME];
} DEVICE_MENU_ITEM;

typedef struct {
	VOID *Info;
	UINT16 StrToken;
	CHAR16 Name[MAX_MENU_ITEM_NAME];
} FILE_MENU_ITEM;

VOID Printf(CHAR8 *FormatString, ...);
VOID * GetDeviceSelection (VOID);
AMI_VECTOR * GetDeviceList (VOID);
UINT16 NewString (CHAR16 *str);
VOID SetString (CHAR16 *str, UINT16 token);
VOID ResetStrings (VOID);
CHAR16 * GetFileSelection (DEVICE_MENU_ITEM *dev);
AMI_VECTOR * GetFileList (VOID *Dir);
CHAR16 ToUpper (CHAR16 c);
EFI_STATUS EFIAPI ChooseFile (
    IN  EFI_DEVICE_PATH_PROTOCOL  *RootDirectory,
    IN  CHAR16                    *FileType,  OPTIONAL
    IN  CHOOSE_HANDLER            ChooseHandler,  OPTIONAL
    OUT EFI_DEVICE_PATH_PROTOCOL  **File  OPTIONAL
        );
UINT16 GetFilesystemIndex();
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
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
