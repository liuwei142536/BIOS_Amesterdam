//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiProgressReportLib.c

The library class is used by the ReFlash module to show flash process progress

**/
//

#include <Uefi.h>
#include <Protocol/AMIPostMgr.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AMIScreenMgmt.h>
#include "Library/PcdLib.h"

/**
    Displays Progress indicator (0 - 100)

    @param UINTN

    @retval EFI_STATUS

**/

extern EFI_GUID gEfiGraphicsOutputProtocolGuid;
static EFI_GRAPHICS_OUTPUT_PROTOCOL *gGOP;
static UINTN Counter = 0;
static AMI_POST_MANAGER_PROTOCOL *AmiPostMgr;
extern EFI_GUID gAmiPostManagerProtocolGuid;
extern EFI_GUID gTSEScreenMgmtProtocolGuid;
static VOID* Messagehandle = NULL;
AMI_TSE_SCREEN_MGMT_PROTOCOL *mAmiTseScreenMgmt = NULL;
UINT8 PcdProgressBarPolicy();
EFI_STATUS AmiShowProgress (IN UINTN PercentComplete )
{
	EFI_STATUS Status = EFI_SUCCESS;
    UINTN i;
    UINTN HorizontalResolution = 0;
    UINTN VerticalResolution = 0;
    TSE_POST_STATUS PostStatus;
        
//    DEBUG ((-1,"AmiShowProgress: %d% \n",PercentComplete));
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
    
    if((PercentComplete == 0) &&  (PcdProgressBarPolicy() != 0x0)){
        Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGOP);
        if(gGOP){
            HorizontalResolution = gGOP->Mode->Info->HorizontalResolution;
            VerticalResolution = gGOP->Mode->Info->VerticalResolution;
            AmiPostMgr->InitProgressBar( (HorizontalResolution- (3*HorizontalResolution/5))/2, 
                                    VerticalResolution/2 - 40, 
                                    3*HorizontalResolution/5, 
                                    20,
                                    100);
        }
        Counter =0;
    }
    else if((PercentComplete == 0) &&  (PcdProgressBarPolicy() == 0x0)){
        Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGOP);
        if(gGOP){
            HorizontalResolution = gGOP->Mode->Info->HorizontalResolution;
            VerticalResolution = gGOP->Mode->Info->VerticalResolution;
            AmiPostMgr->InitProgressBar( HorizontalResolution/5, 
                                    VerticalResolution - 40, 
                                    3*HorizontalResolution/5, 
                                    10,
                                    100);
        }
        Counter =0;
    }
    for (i = Counter; i < PercentComplete; i++, Counter++)
    {
    		AmiPostMgr->SetProgressBarPosition();
    }
    return Status;
}

/**
   Shows progress of the operation along with progress messages
  In Post, the progress is shown using DisplayProgress 
  In QuietBoot, the progress and messages are drawn at the bottom of the screen

  @param  Title - Message box title
  @param  Message - Help message
  @param PercentComplete Operation completeness percentage (0 - 100)

  @retval  EFI_SUCCESS  Progress displayed
  @retval  Other        Error occurred during operation

**/
EFI_STATUS AmiShowProgressEx (IN CHAR16* Title, IN CHAR16* Message, IN UINTN PercentComplete ) 
{
    TSE_POST_STATUS PostStatus;
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN HorizontalResolution = 0;
    UINTN VerticalResolution = 0;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL   ForeGround = {0xFF,0xFF,0xFF,0};
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL   BackGround = {0,0,0,0};
    AMI_POST_MGR_KEY MessageOutkey;
    AMI_POST_MANAGER_PROTOCOL    *gAmiPostMgr;
    Status = gBS->LocateProtocol((EFI_GUID*)&gAmiPostManagerProtocolGuid,(VOID*) NULL,(VOID**) &gAmiPostMgr);
        if(EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }
    //Check for the post status. The library supports progress and messages are displayed during QuietBoot at the bottom of the screen 
	//Progress bar should always start with percent zero.
    PostStatus = gAmiPostMgr->GetPostStatus();
    if((PostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) && (PcdProgressBarPolicy() != 0x2))
    {
      //PcdProgressBar value is non zero then, in quiet boot screen, clearing the graphicsscreen and in TSE Text Screen resoulution, showing the progress bar at the middle of the screen. if we have Gif Logo then gif animation is stopped.
        if((PercentComplete == 0) && (PcdProgressBarPolicy() != 0x0))
        {
            Status = gBS->LocateProtocol(&gTSEScreenMgmtProtocolGuid, NULL,(void**) &mAmiTseScreenMgmt);

            if (!EFI_ERROR(Status)) {
                mAmiTseScreenMgmt->ClearScreen(EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY);
            }
          
        } 
        gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gGOP);
        HorizontalResolution = gGOP->Mode->Info->HorizontalResolution;
        VerticalResolution = gGOP->Mode->Info->VerticalResolution;

        if(PcdProgressBarPolicy() == 0x0)
        {
          //PcdProgressBar value is zero then default behavior, ie, in quiet boot screen ,progress bar is shown at the leftside below the quietboot logo.
            AmiShowProgress(PercentComplete);
            gAmiPostMgr->DisplayQuietBootMessage(Title, HorizontalResolution/5, VerticalResolution - 80, CA_AttributeCustomised,
                    ForeGround, BackGround
            );
            gAmiPostMgr->DisplayQuietBootMessage(Message, HorizontalResolution/5, VerticalResolution - 60, CA_AttributeCustomised,
                    ForeGround, BackGround
            );
           
        }
        else if (PcdProgressBarPolicy() == 0x1)
        {
          //PcdProgressBar value is 1 then, screen cleared and progress bar is shown at the middle of the screen along with the quietboot messages.
            AmiShowProgress(PercentComplete);
            gAmiPostMgr->DisplayQuietBootMessage(Title, (HorizontalResolution/2)-100, (VerticalResolution/2)-80 , CA_AttributeCustomised,
                    ForeGround, BackGround
            );
            gAmiPostMgr->DisplayQuietBootMessage(Message, (HorizontalResolution/2)-100, (VerticalResolution/2)-60, CA_AttributeCustomised,
                    ForeGround, BackGround
            );
            
        }
    }
    else 
    {       
        //In Post, the progress and messages are shown using DisplayProgress 
        if(PercentComplete == 0)
        {            
            if(PostStatus == TSE_POST_STATUS_IN_POST_SCREEN)
            {
                gST->ConOut->ClearScreen( gST->ConOut);

            }
            else  if((PcdProgressBarPolicy() == 0x2) &&(PostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN))
            {
              //PcdProgressBar value is 2 then, clearing the screen and then displaying the progress bar as in the post screen.
                Status = gBS->LocateProtocol(&gTSEScreenMgmtProtocolGuid, NULL,(void**) &mAmiTseScreenMgmt);

                if (!EFI_ERROR(Status)) {
                    mAmiTseScreenMgmt->ClearScreen(EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY);
                }
            }
            gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_INIT, 
                                            Title,
                                            Message,
                                            NULL,
                                            PercentComplete,               
                                            &Messagehandle,
                                            &MessageOutkey);
        }
        else if(PercentComplete == 100)
        {
            gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_UPDATE, 
                                                       Title,
                                                       Message,
                                                       NULL,
                                                       PercentComplete,               
                                                       &Messagehandle,
                                                       &MessageOutkey);
            gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_CLOSE, 
                                                       NULL,
                                                       NULL,
                                                       NULL,
                                                       PercentComplete,               
                                                       &Messagehandle,
                                                       &MessageOutkey);
        }
        else
		{      
            gAmiPostMgr->DisplayProgress(AMI_PROGRESS_BOX_UPDATE, 
                                                       Title,
                                                       Message,
                                                       NULL,
                                                       PercentComplete,               
                                                       &Messagehandle,
                                                       &MessageOutkey);
        }
    }
    return Status;
}

UINT8 PcdProgressBarPolicy()
{
    return PcdGet8(AmiTsePcdProgressBar);
}
