//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
/** @file
  Implementation of the AmiFlashControlLib library NULL instance
*/
#include <Library/AmiFlashControlLib.h>

/**
  Returns Flash control table

  @param InputImage Pointer to pointer of flash update image
  @param InputImageSize Version of the ROM Layout data structure
  @param FlashControlTable Pointer where to store pointer to Flash control table

  @retval  EFI_SUCCESS  Returned flash control table is valid
  @retval  Other        Error occured during operation
*/
EFI_STATUS AmiGetFlashUpdateMethod (
    IN OUT VOID    **InputImage,
    IN OUT UINT32  *InputImageSize,
    OUT AMI_CAPSULE_IMAGE **FlashControlTable
    )
{
    return EFI_NOT_FOUND;
}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

