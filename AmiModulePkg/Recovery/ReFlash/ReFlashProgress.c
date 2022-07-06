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
 * @file ReFlashProgress.c
 * @brief Flash progress API implementation
 */

#include <Protocol/AMIPostMgr.h>
#include <Library/AmiProgressReportLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include "ReFlashProgress.h"

static EFI_GUID AmiPostManagerProtocolGuid = AMI_POST_MANAGER_PROTOCOL_GUID;
static AMI_POST_MANAGER_PROTOCOL *AmiPostMgr = NULL;

static void esrt_progress_init (FLASH_PROGRESS *self) {}
static void esrt_progress_close (FLASH_PROGRESS *self) {}
static void esrt_progress_update (FLASH_PROGRESS *self, UINTN percent)
{
    AmiShowProgress (percent);
}


static void tse_progress_init (FLASH_PROGRESS *self)
{
    AMI_POST_MGR_KEY OutKey;
    if (AmiPostMgr == NULL)
        gBS->LocateProtocol(&AmiPostManagerProtocolGuid, NULL, (VOID **)&AmiPostMgr);

    if(AmiPostMgr != NULL) {
        AmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_INIT,
                 L"Flash Update",
                 NULL,
                 NULL,
                 0,
                 &self->Extra,
                 &OutKey);
    }
}

static void tse_progress_update (FLASH_PROGRESS *self, UINTN percent)
{
    AMI_POST_MGR_KEY OutKey;
    if(AmiPostMgr != NULL) {
        AmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_UPDATE,
                 L"Flash Update",
                 L"Updating main firmware",
                 NULL,
                 percent, 
                 &self->Extra,
                 &OutKey);
    }
}

static void tse_progress_close (FLASH_PROGRESS *self)
{
    AMI_POST_MGR_KEY OutKey;
    if(AmiPostMgr != NULL) {
        AmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_CLOSE,
                 L"Flash Update",
                 NULL,
                 NULL,
                 0,
                 &self->Extra,
                 &OutKey);
    }
}

FLASH_PROGRESS esrt_progress = {
    esrt_progress_init,
    esrt_progress_update,
    esrt_progress_close,
    0
};

FLASH_PROGRESS tse_progress = {
    tse_progress_init,
    tse_progress_update,
    tse_progress_close,
    0
};

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

