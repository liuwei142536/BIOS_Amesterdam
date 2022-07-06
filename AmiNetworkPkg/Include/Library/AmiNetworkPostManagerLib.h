//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//*************************************************************************
/** @file AmiNetworkPostManagerLib.h
    AmiNetworkPostManagerLib Definitions

**/
//*************************************************************************
#ifndef _AMI_NETWORK_POSTMANAGER_LIB_H_
#define _AMI_NETWORK_POSTMANAGER_LIB_H_

/**
  This function create pop-up Message 

  @param[in] String            The message in the pop-up.

  @retval VOID

**/

VOID 
AMICreatePopUp(
    IN  CHAR16          *String
);

/**
  This function create pop-up Message 

  @param[in] MsgTitle          Title of the pop-up box.
  @param[in] String            The message in the pop-up.

  @retval VOID

**/
VOID 
AMICreatePopUpEx(
    IN  CHAR16          *MsgTitle,
    IN  CHAR16          *String
);

/**
  This function create pop-up Message 

  @param[in] String            The message in the pop-up.

  @retval VOID

**/

VOID 
AMIPrintText(
    IN  CHAR16          *String
);

/**
  This function used for switch to post screen 

  @retval VOID

**/

VOID 
AMISwitchToPostScreen (
);

/**
  This function clears graphics screen 

  @param  VOID

  @retval VOID

**/

VOID 
ClearGraphicsScreen (
  VOID
);

/**
  Check for invalid character in IP address.

  @param  IpSource               A pointer to a Null-terminated 
                                 Unicode string.

  @return EFI_SUCCESS            If invalid characters are not present
  @return EFI_INVALID_PARAMETER  If in valid characters are present

**/

EFI_STATUS
CheckInvalidCharinIpAddress (
  IN CONST CHAR16       *IpSource,
  IN UINT8              IpMode
);

/**
  Retrieves a string from a string package in a specific language.  If the language
  is not specified, then a string from a string package in the current platform 
  language is retrieved.  If the string can not be retrieved using the specified 
  language or the current platform language, then the string is retrieved from 
  the string package in the first language the string package supports.  The 
  returned string is allocated using AllocatePool().  The caller is responsible 
  for freeing the allocated buffer using FreePool().
  
  If HiiHandle is NULL, then ASSERT().
  If StringId is 0, then ASSET.

  @param[in]  HiiHandle  A handle that was previously registered in the HII Database.
  @param[in]  StringId   The identifier of the string to retrieved from the string 
                         package associated with HiiHandle.
  @param[in]  Language   The language of the string to retrieve.  If this parameter 
                         is NULL, then the current platform language is used.  The 
                         format of Language must follow the language format assumed 
                         the HII Database.

  @retval NULL   The string specified by StringId is not present in the string package.
  @retval Other  The string was returned.

**/

EFI_STRING
EFIAPI
HiiGetString (
  IN EFI_HII_HANDLE  HiiHandle,
  IN EFI_STRING_ID   StringId,
  IN CONST CHAR8     *Language  OPTIONAL
);

/**
  Gets the string form Supported language. If it fails, gets the 
  string from Default language.

    @param HiiHandle StringId Language

    @retval String 
 
**/

CHAR16* NetLibHiiGetString( 
  EFI_HII_HANDLE HiiHandle, 
  EFI_STRING_ID StringId,
  CHAR8 *Language
);

#endif
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

