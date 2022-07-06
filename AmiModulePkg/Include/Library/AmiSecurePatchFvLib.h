//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2017, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************

/** @file
    The library is used to implement FV processing override hook
**/


//---------------------------------------------------------------------------


#ifndef __AMI_SECURE_PATCH_FV_LIB_H__
#define __AMI_SECURE_PATCH_FV_LIB_H__

#include <Protocol/FirmwareVolumeBlock.h>

/**
    AMI hook that allows driver overriding.

    @param [in] Fvb                 FV being processed.
    @param [in] PatchFfsHeader      FFS file to override.
    @param [in] FfsFileListHeader   List of drivers in the FV being processed.
    @param [in] FileCached          Deinfes if file location has been cached(copied) to memory.
    @param [in] FileState           PatchFfsHeader file state

    @retval EFI_SUCCESS                 No issues
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.

**/
EFI_STATUS AmiFvOverrideCheck(
    IN EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL *Fvb,
    IN EFI_FFS_FILE_HEADER *PatchFfsHeader,
    IN LIST_ENTRY *FfsFileListHeader,
    IN OUT BOOLEAN *FileCached,
    IN EFI_FFS_FILE_STATE FileState
);

#endif
