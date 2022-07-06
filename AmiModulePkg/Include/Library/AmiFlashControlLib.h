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
  Definition of the AmiFlashControlLib library class.
*/
#ifndef __AMI_FLASH_CONTROL_LIB__H__
#define __AMI_FLASH_CONTROL_LIB__H__

#include <Uefi.h>

#define AMI_FC_BLOCK_FIXED_ADDRESS      0x1     //< Block resides at fixed address in flash
#define AMI_FC_BLOCK_EXTERNAL_FLASH     0x2     //< Block has to be flashed with external function call
#define AMI_FC_BLOCK_PRESERVE_CONTENT   0x4     //< Block content has to be preserved during update
#define AMI_FC_BLOCK_OVERRIDE_EXISTING  0x8     //< Migrate fxied block toa new address

typedef struct {
    EFI_GUID Name;      //< Block GUID
    UINT64 Address;     //< Block address in flash
    UINT32 Size;        //< Block size
    UINT32 Offset;      //< Block offset in update image
    UINT32 Flags;       //< Flash control flags
} AMI_CAPSULE_IMAGE_BLOCK;

typedef struct {
    UINT32 Signature;   //< Should be set to AMI_CAPSULE_IMAGE_SIGNATURE
    UINT32 Version;     //< Version of AMI_CAPSULE_IMAGE structure
    UINT32 Size;        //< Structure size including header
    //AMI_CAPSULE_IMAGE_BLOCK Blocks[];
} AMI_CAPSULE_IMAGE;

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
    );


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
