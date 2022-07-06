//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2018, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
/** @file EsaFileBrowser.h


**/

#ifndef _AMI_ESA_FILE_BROWSER_H_
#define	_AMI_ESA_FILE_BROWSER_H_

#define AMI_ESA_FILE_BROWSER_PROTOCOL_GUID \
		{0xb898d2ff, 0x5c34, 0x49d7, {0xa4, 0x95, 0x45, 0xa9, 0x3d, 0xa8, 0xd6, 0x34}}

typedef struct AMI_ESA_FILE_BROWSER_PROTOCOL  AMI_ESA_FILE_BROWSER_PROTOCOL;

/**
  Prototype for the next process after user chosed one file.
  @param[in] FilePath     The device path of the find file.
  @retval    TRUE         Need exit file explorer after do the extra task.
  @retval    FALSE        Not need to exit file explorer after do the extra task.

**/
typedef
BOOLEAN
(EFIAPI *CHOOSE_HANDLER)(
  IN EFI_DEVICE_PATH_PROTOCOL  *FilePath
  );

/**
  Choose a file in the specified directory. 

  If user input NULL for the RootDirectory, will choose file in the system.

  If user input *File != NULL, function will return the allocate device path
  info for the choosed file, caller has to free the memory after use it.

  @param  RootDirectory    Pointer to the root directory.
  @param  FileType         The file type need to choose.
  @param  ChooseHandler    Function pointer to the extra task need to do
                           after choose one file.
  @param  File             Return the device path for the last time chosed file.

  @retval EFI_SUCESS       Choose the file success.
  @retval Other errors     Choose the file failed.
**/
typedef
EFI_STATUS
(EFIAPI   *CHOOSE_FILE) (
  IN  EFI_DEVICE_PATH_PROTOCOL  *RootDirectory,
  IN  CHAR16                    *FileType,  OPTIONAL
  IN  CHOOSE_HANDLER            ChooseHandler,  OPTIONAL
  OUT EFI_DEVICE_PATH_PROTOCOL  **File  OPTIONAL
  );

struct AMI_ESA_FILE_BROWSER_PROTOCOL {
  CHOOSE_FILE                          ChooseFile;
};

extern EFI_GUID gEsaFileBrowserProtocolGuid;


#endif /* _AMI_ESA_FILE_BROWSER_H_ */

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
