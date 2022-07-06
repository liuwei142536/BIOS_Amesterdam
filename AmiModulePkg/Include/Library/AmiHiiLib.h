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
  Ami HII Library
*/

#ifndef __AMI_HII_LIB__H__
#define __AMI_HII_LIB__H__
#include <Protocol/HiiConfigAccess.h>

typedef VOID (EFIAPI *AMI_INIT_HII_PACK)(EFI_HII_HANDLE HiiHandle);

/**
    Loads HII packages associated with
    the running image and publishes them to the HII database

    @param HiiConfigAccess - Pointer to HII Config Access Protocol
    @param InitFunction - initialization function to be launched once resources are published.
    @param HiiHandle - Pointer to Handle to return


    @retval - Status, based on result

**/
EFI_STATUS AmiPublishHiiResources(
   IN EFI_HII_CONFIG_ACCESS_PROTOCOL *HiiConfigAccess OPTIONAL,
   IN AMI_INIT_HII_PACK InitFunction OPTIONAL,
   OUT EFI_HII_HANDLE *HiiHandle
);

#endif
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
