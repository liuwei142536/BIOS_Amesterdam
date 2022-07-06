//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file OA3_SmiFlashHook.c

**/

#include "OA3.h"
#include <Protocol/SmiFlash.h>
#include <Library/DebugLib.h>


/**
    This function is SW SMI hook that sets Flash Block Description
    type for AMI AFU utility. 

    @param 
        SwSmiNum    - SW SMI value number
        Buffer      - Flash descriptor address

    @retval VOID

**/

VOID OemActivationUpdateBlockTypeId (
    IN UINT8  SwSmiNum,
    IN UINT64 Buffer )
{
    BLOCK_DESC *BlockDesc;
    UINTN  i;

    // return if SW SMI value is not "Get Flash Info"
    if (SwSmiNum != SMIFLASH_GET_FLASH_INFO)
        return;

    BlockDesc = (BLOCK_DESC*)&((INFO_BLOCK*)Buffer)->Blocks;

    for (i = 0; i < ((INFO_BLOCK*)Buffer)->TotalBlocks; i++) {

        DEBUG ((DEBUG_INFO, "OemActivationUpdateBlockTypeId: %08X(%08X), Block %08X\n",OEM_ACTIVATION_BLOCK_ADDRESS,OEM_ACTIVATION_BLOCK_END,BlockDesc[i].StartAddress));

        if (BlockDesc[i].StartAddress < OEM_ACTIVATION_BLOCK_ADDRESS)
            continue;

        if (BlockDesc[i].StartAddress >= OEM_ACTIVATION_BLOCK_END)
            continue;

        DEBUG ((DEBUG_INFO, "OemActivationUpdateBlockTypeId: Found Blocks %08X\n",BlockDesc[i].StartAddress));

        BlockDesc[i].Type = OA3_FLASH_BLOCK_DESC_TYPE;
    }
}


//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2018, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
