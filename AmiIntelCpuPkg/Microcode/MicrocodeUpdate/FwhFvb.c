//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file FwhFvb.c
    This file contains code for flash update used by the MicrocodeUpdate module.

**/


#include "Token.h"
#include <Library/AmiFlashLib.h>


/**
    This function erases a block of Flash.

    @param Address          Address of the block to erase.
    @param NumberOfBytes    The number of bytes to erase

    @retval EFI_SUCCESS Return Status based on errors that occurred while erasing.

**/

EFI_STATUS FwhErase(
    IN UINTN      Address,
    IN UINTN      NumberOfBytes
)
{
    UINTN FlashBlockStart = Address;
    UINTN FlashBlockEnd = Address + NumberOfBytes - 1;
    UINTN FlashBlock;
    BOOLEAN bStatus = FALSE;
    
    if ((FlashBlockStart & (FLASH_BLOCK_SIZE - 1)) != 0) {
        return EFI_INVALID_PARAMETER;
    }
    
    if ((NumberOfBytes & (FLASH_BLOCK_SIZE - 1)) != 0) {
        return EFI_INVALID_PARAMETER;
    }

    AmiFlashDeviceWriteEnable();

    for (FlashBlock = FlashBlockStart; FlashBlock <= FlashBlockEnd; FlashBlock += FLASH_BLOCK_SIZE) {
        AmiFlashBlockWriteEnable((UINT8*)FlashBlock);
		bStatus = AmiFlashEraseBlock((UINT8*)FlashBlock);
		AmiFlashBlockWriteDisable((UINT8*)FlashBlock);
        if (!bStatus) break;
    }

    AmiFlashDeviceWriteDisable();

    return bStatus ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}

/**
    This function writes a block of Flash.

    @param Buffer           Pointer to a buffer containing data to write.
    @param Address          Address of the block to write.
    @param NumberOfBytes    The number of bytes to write

    @retval EFI_SUCCESS Return Status based on errors that occurred while writing.

**/

EFI_STATUS FwhWrite(
    IN UINT8      *Buffer, 
    IN UINTN      Address, 
    IN UINTN      NumberOfBytes
)
{
    BOOLEAN  bStatus;
    UINTN FlashBlockStart = Address;
    UINTN FlashBlockEnd = Address + NumberOfBytes - 1;
    UINTN FlashBlock;
    
    if ((FlashBlockStart & (FLASH_BLOCK_SIZE - 1)) != 0) {
        return EFI_INVALID_PARAMETER;
    }
    
    if ((NumberOfBytes & (FLASH_BLOCK_SIZE - 1)) != 0) {
        return EFI_INVALID_PARAMETER;
    }

    AmiFlashDeviceWriteEnable();

    for (FlashBlock = FlashBlockStart; FlashBlock <= FlashBlockEnd; FlashBlock += FLASH_BLOCK_SIZE) {
        AmiFlashBlockWriteEnable((UINT8*)FlashBlock);
    }


    bStatus = AmiFlashProgram((UINT8*)Address, Buffer, (UINT32)NumberOfBytes);


    for (FlashBlock = FlashBlockStart; FlashBlock <= FlashBlockEnd; FlashBlock += FLASH_BLOCK_SIZE) {
        AmiFlashBlockWriteDisable((UINT8*)FlashBlock);
    }

    AmiFlashDeviceWriteDisable();

    return bStatus ? EFI_SUCCESS : EFI_DEVICE_ERROR;
}
