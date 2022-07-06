//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2017, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//

/** @file AmiProgressReportLib.c

The library class is used by the ReFlash module to show flash process progress

**/
//
#include <Uefi.h>
#include <Protocol/AMIPostMgr.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
    Displays Progress indicator (0 - 100)

    @param UINTN

    @retval EFI_STATUS

**/

extern EFI_GUID gAmiPostManagerProtocolGuid;
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;
static EFI_GRAPHICS_OUTPUT_PROTOCOL *gGOP;
static UINTN Counter = 0;
static AMI_POST_MANAGER_PROTOCOL *AmiPostMgr;

EFI_STATUS AmiShowProgress (IN UINTN PercentComplete )
{
	EFI_STATUS Status;
    UINTN i;
    UINTN HorizontalResolution = 0;
    UINTN VerticalResolution = 0;
    TSE_POST_STATUS PostStatus;
        
    DEBUG ((-1,"AmiShowProgress: %d% \n",PercentComplete));
    if (AmiPostMgr == NULL)
    {
    	Status = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, (VOID **)&AmiPostMgr);
    	if (EFI_ERROR(Status))
    	    return Status;
    }
    //Check for the post status. The library supports progress display during QuietBoot screen or immediately after
    //It won't display progress during Post screen due to scrolling.
    
    PostStatus = AmiPostMgr->GetPostStatus();
    if( PostStatus > TSE_POST_STATUS_IN_BOOT_TIME_OUT)
        return EFI_UNSUPPORTED;
    
    if(PercentComplete == 0){
        Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGOP);
        if(gGOP){
            HorizontalResolution = gGOP->Mode->Info->HorizontalResolution;
            VerticalResolution = gGOP->Mode->Info->VerticalResolution;
            AmiPostMgr->InitProgressBar( HorizontalResolution/5, 
                                    VerticalResolution - 10, 
                                    3*HorizontalResolution/5, 
                                    10,
                                    100);
        }
    }
    for (i = Counter; i < PercentComplete; i++, Counter++)
    {
    		AmiPostMgr->SetProgressBarPosition();
    }
    return EFI_SUCCESS;
}


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
