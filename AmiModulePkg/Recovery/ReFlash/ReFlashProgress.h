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
 * @file ReFlashProgress.h
 * @brief Flash progress report interface
 */

#ifndef __REFLASH_PROGRESS_H
#define __REFLASH_PROGRESS_H

typedef struct _FLASH_PROGRESS FLASH_PROGRESS;

typedef void progress_init (FLASH_PROGRESS *self);
typedef void progress_update (FLASH_PROGRESS *self, UINTN percent);
typedef void progress_close (FLASH_PROGRESS *self);

struct _FLASH_PROGRESS {
    progress_init *PrInit;
    progress_update *PrUpdate;
    progress_close *PrClose;
    void *Extra;
};

extern FLASH_PROGRESS esrt_progress;
extern FLASH_PROGRESS tse_progress;

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
