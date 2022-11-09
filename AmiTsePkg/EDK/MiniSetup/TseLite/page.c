//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//

/** @file page.c
    This file contains code to handle page level operations

**/

#include "minisetup.h"

PAGE_METHODS gPage =
{
	(OBJECT_METHOD_CREATE)PageCreate,
	(OBJECT_METHOD_DESTROY)PageDestroy,
	(OBJECT_METHOD_INITIALIZE)PageInitialize,
	(OBJECT_METHOD_DRAW)PageDraw,
	(OBJECT_METHOD_HANDLE_ACTION)PageHandleAction,
	(OBJECT_METHOD_SET_CALLBACK)PageSetCallback
};
CHAR16 *gPageTitle = NULL;

//////////////////////////////////////////////////
//		Extern variables
//////////////////////////////////////////////////
extern EFI_GRAPHICS_OUTPUT_PROTOCOL	*gGOP;
extern BOOLEAN 		gPackUpdatePending;
extern UINT16 		gSelIdx;
extern UINT8 		gAddBgrtResolutions;

extern EFI_STATUS	FileBrowserLaunchFileSystem (UINT32 Variable);
extern  BOOLEAN IsTseCallFormOpenOnPgChangeSupport(void);
extern  VOID UEFICallSetupFormCallBackForm(UINT16 PageID, UINTN Action );
//////////////////////////////////////////////////
//		Function Declarations
//////////////////////////////////////////////////
EFI_STATUS 	GetPageSubTitle(UINT32 PgIndex, UINT16 *TitleToken);
VOID 		SetGetBgrtCoordinates ( UINTN	*DestX, UINTN *DestY, UINTN *Width,	UINTN *Height, BOOLEAN Command );
VOID 		*ConvertBlt2Bmp ();
EFI_STATUS 	ProcessPackNotificationHook (VOID);
BOOLEAN 	IsPageModal(UINT32 PgIndex); 
VOID DeactivateControls(PAGE_DATA *page);
extern EFI_HANDLE ReconnectPageHandle;
extern EFI_STATUS DoDisconnectAndConnectController (UINT32 OldPageId, UINT32 NewPageId);
/**
    function to create a page

    @param object 

    @retval status

**/
EFI_STATUS PageCreate( PAGE_DATA **object )
{
	EFI_STATUS Status = EFI_SUCCESS;

	if ( *object == NULL )
	{
		*object = EfiLibAllocateZeroPool(sizeof(PAGE_DATA));
		if ( *object == NULL )
			return EFI_OUT_OF_RESOURCES;
	}

	Status = gObject.Create((VOID**) object );

	if (EFI_ERROR(Status))
	{
		MemFreePointer( (VOID **)object );
		return Status;
	}

	(*object)->Methods = &gPage;

	return Status;
}

/**
    function to destroy a page, which uses the HotKey Fucntions

    @param page BOOLEAN freeMem

    @retval status

**/
EFI_STATUS PageDestroy( PAGE_DATA *page, BOOLEAN freeMem )
{
	UINT32	i;

	gObject.Destroy( page, FALSE );

	for ( i = 0; i < page->HotKeyCount; i++ )
	{
		gHotKey.Destroy( page->HotKeyList[i], TRUE );
	}

	MemFreePointer( (VOID **)&page->HotKeyList );

	for ( i = 0; i < page->FrameCount; i++ )
	{
		gFrame.Destroy( page->FrameList[i], TRUE );
	}
	page->FrameCount = 0;
	MemFreePointer( (VOID **)&page->FrameList );

	if ( freeMem )
		MemFreePointer( (VOID **)&page );

	return EFI_SUCCESS;
}

/**
    function to initialize a page, which uses the Frame Fucntions

    @param page PAGE_INFO *data

    @retval status

**/
EFI_STATUS PageInitialize( PAGE_DATA *page, PAGE_INFO *data )
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINT32 i;

	page->CurrentFrame = 0;
	MemCopy( &page->PageData, data, sizeof(PAGE_INFO) );

	page->FrameCount = StyleGetPageFrames( data->PageID );

	page->FrameList = EfiLibAllocateZeroPool( sizeof(FRAME_DATA *) * page->FrameCount );
	if ( page->FrameList == NULL )
		return EFI_OUT_OF_RESOURCES;

	for ( i = 0; i < page->FrameCount; i++ )
	{
		if ( gFrame.Create( (void**)&page->FrameList[i] ) != EFI_SUCCESS )
			continue;

		page->FrameList[i]->PageID = data->PageID;
		gFrame.Initialize( page->FrameList[i], StyleGetFrameInitData( data->PageID, i ) );

		Status = StyleInitializeFrame( page->FrameList[i], page->FrameList[i]->FrameData.FrameType, data );
	}

	return Status;
}
/**
 *  function to deactivate controls if any of them are being edited
 * @return void
 */

VOID DeactivateControls(PAGE_DATA *page)
{
    UINT32 MainFrameIndex=0;
    FRAME_DATA *MainFrame = NULL;
    CONTROL_DATA **control;
    CONTROL_INFO *ControlInfo = NULL;
    UINT16 i;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    
    MainFrameIndex =StyleFrameIndexOf(MAIN_FRAME); 
    MainFrame = page->FrameList[MainFrameIndex];

     if(MainFrame->CurrentControl == FRAME_NONE_FOCUSED)
           return;
     else
           control = &MainFrame->ControlList[MainFrame->CurrentControl];
           
     if ( *control != NULL )
     {
         if( (*control)->ControlActive == TRUE )
         { 
            //backup controlinfo
		 	ControlInfo = (CONTROL_INFO*)EfiLibAllocateZeroPool( sizeof(CONTROL_INFO) );
           	MemCopy( ControlInfo, &((*control)->ControlData), sizeof(CONTROL_INFO) );
 
            (*control)->Methods->Destroy( *control, TRUE);
            *control = NULL;
            for(i=0;gTseControlMap[i].ControlType != CONTROL_TYPE_NULL;i++)
            {
                 if(gTseControlMap[i].ControlType == ControlInfo->ControlType)
                 {
                     Status = gTseControlMap[i].ControlMethods->Create((void**)control);
                     if ( ! EFI_ERROR(Status) )
                     {
                         if(gTseControlMap[i].CallbackSuppored )
                             gTseControlMap[i].ControlMethods->SetCallback(*control, MainFrame, 
                                     (OBJECT_CALLBACK)(UINTN)gTseControlMap[i].CallbackFunction, 
                                     (VOID*)gTseControlMap[i].CallbackContext);
                         (*control)->Methods->Initialize( *control, ControlInfo );
                         //Set left margin; top will be dynamically updated during draw
                         (*control)->Methods->SetPosition( *control, MainFrame->FrameData.Left + (UINT8)gLabelLeftMargin, MainFrame->FrameData.Top );
                         break;
                     }
                 }
             }
	         MemFreePointer( (VOID **)&ControlInfo);
         }
     }
}

/**
    function to draw a page with attributes

    @param page 

    @retval status

**/
extern UINTN gSetupStatus;  

EFI_STATUS PageDraw( PAGE_DATA *page )
{
//	UINT32	i, PageNum, MainFrameIndex=0, CurrentControl;
	UINT32	i,  MainFrameIndex=0, CurrentControl;
		
    FRAME_DATA *MainFrame = NULL;

    CONTROL_DATA **control;
    UINTN BackUpSetupStatus = gSetupStatus;
    gSetupStatus = TSE_SETUP_STATUS_BLOCK_PACKUPDATEPROCESS;
	MainFrameIndex =StyleFrameIndexOf(MAIN_FRAME); 
	MainFrame = page->FrameList[MainFrameIndex];

	StyleBeforeFrame( MainFrame ) ;

	// update the help for the current selection
	_PageUpdateHelp( page );
	_PageUpdateTitle( page );
	_PageUpdateSubtitle( page );
	_PageUpdateNavStrings( page );
	_PageUpdateFrameStrings( page );

//	PageNum = gApp->CurrentPage ; // PageNum Set unused

	//Call Oem hooks to decide item focus on complete redraw
	
    if(gApp->CompleteRedraw && gApp->OnRedraw) {
		
		if(gApp->OnRedraw == MENU_COMPLETE_REDRAW)
			StylePageItemFocus(page, MainFrame);
		else if(gApp->OnRedraw == SUBMENU_COMPLETE_REDRAW) 
			StyleSubPageItemFocus(page, MainFrame);
		else if(gApp->OnRedraw == DEACTIVATE_CONTROLS)
		{
		    DeactivateControls(page);
		}
		    
		gApp->OnRedraw = 0 ;
			
	}
		


    //If there is an active control in Main frame only draw that frame
    if(MainFrame->CurrentControl == FRAME_NONE_FOCUSED)
		control = NULL;
	else
		control = &MainFrame->ControlList[MainFrame->CurrentControl];

	if ( control != NULL )
	{
        //If a control is active only draw that
		if( (*control) && (*control)->ControlActive == TRUE )
		{
			StyleBeforeFrame( MainFrame ) ;
			gFrame.Draw( MainFrame );
		    gSetupStatus = BackUpSetupStatus;
            return EFI_SUCCESS;
		}
	}

	// Backup the MainFrame->CurrentControl
	CurrentControl = MainFrame->CurrentControl;
    for ( i = 0; i < page->FrameCount; i++ )
  	{
		StyleBeforeFrame( page->FrameList[i] ) ;
		gFrame.Draw( page->FrameList[i] );
		if(i == MainFrameIndex)
		{
			// After expression evalution if current control is affected
			if(MainFrame->CurrentControl != CurrentControl)
			{
				// Help might be updated
				_PageUpdateHelp( page );
				CurrentControl = MainFrame->CurrentControl;
				i = 0; // Redraw all the frames again
			}
		}
  	}

    gSetupStatus = BackUpSetupStatus;

	return EFI_SUCCESS;
}

/**
    function to handle the page actions

    @param page ACTION_DATA *action

    @retval status

**/
extern BOOLEAN gEnableProcessPack;
VOID HandleReadOpOnChanged(UINT32 CurrentPage);
EFI_STATUS PageHandleAction( PAGE_DATA *page, ACTION_DATA *action )
{
	UINT32	i;
    EFI_STATUS Status = EFI_UNSUPPORTED;
	UINT32 stackIndex = 0xFFFFFFFF, tmpcounter = 0;
	UINT16 parentPgID = 0, TitleToken = 0;
	
	UINT32 TempCurPage = gApp->CurrentPage;

	// On printScreen HotKey, Just return Unsupported and It will handed by Application Hotkey Handler.
	if( ( action->Input.Type == ACTION_TYPE_KEY ) &&
		(TsePrintScreenEventSupport(action->Input.Data.AmiKey.Key.ScanCode) ) ) 
	{
		GetPageSubTitle(page->PageData.PageID, &TitleToken);
		gPageTitle = HiiGetString( page->PageData.PageHandle, TitleToken);

		if (NULL != gPageTitle)
		{
			while(gPageTitle[tmpcounter]!= L'\0')
			{
				if (	('/' == gPageTitle[tmpcounter]) || 
						('\\' == gPageTitle[tmpcounter]) ||
						('<' == gPageTitle[tmpcounter]) ||
						('>' == gPageTitle[tmpcounter]) ||
						('?' == gPageTitle[tmpcounter]) ||
						('*' == gPageTitle[tmpcounter]) ||
						(':' == gPageTitle[tmpcounter]) ||
						('"' == gPageTitle[tmpcounter]) ||
						('|' == gPageTitle[tmpcounter])
					) //to avoid create file with special characters. If any of these character found then replace it with '-' character
					gPageTitle[tmpcounter] = '-';
				tmpcounter++;
			}
		}
		Status = EFI_UNSUPPORTED;
		goto DONE;
        
	}
/*
#if TSE_PRN_SCRN_EVENT_SUPPORT
	if (( action->Input.Type == ACTION_TYPE_KEY ) && ( action->Input.Data.AmiKey.Key.ScanCode == TSE_PRN_SCRN_KEY_SCAN ))
            return EFI_UNSUPPORTED;
#endif
*/
	for ( i = 0; i < page->FrameCount; i++ )
	{
		if ( gFrame.HandleAction( page->FrameList[StyleFrameIndexOf(i)], action ) == EFI_SUCCESS )
		{	
            Status = EFI_SUCCESS;
		    goto DONE;
        }
	}

	//If Page is Modal do not proceed further
	if(IsPageModal(page->PageData.PageID))
	{    	
		Status = EFI_UNSUPPORTED; //IF page is Modal
 
		if( action->Input.Type == ACTION_TYPE_KEY )
      	action->Input.Type = ACTION_TYPE_NULL ;
		goto DONE;
	}

	for ( i = 0; i < page->HotKeyCount; i++ )
	{
		if ( gHotKey.HandleAction( page->HotKeyList[i], action ) == EFI_SUCCESS )
        {
			Status = EFI_SUCCESS;
		    goto DONE;
        }
	}

	if(
        (( action->Input.Type == ACTION_TYPE_KEY ) && (ControlActionAbort == MapControlKeysHook(action->Input.Data.AmiKey)))
        || ((action->Input.Type == ACTION_TYPE_MOUSE) && (ControlActionAbort == MapControlMouseActionHook(&(action->Input.Data.MouseInfo))) /*(TSEMOUSE_RIGHT_CLICK == action->Input.Data.MouseInfo.ButtonStatus)*/)
      )
	{
        if ( page->PageData.PageParentID == 0 )
        {
            Status = EFI_UNSUPPORTED;
            if(IsLinkHistorySupport())
            {    
			    Status = GetParentFormID((UINT16)gApp->CurrentPage, &parentPgID, &stackIndex); //Gets the parent page of the current page
			
                if(Status == EFI_SUCCESS)
                {
                    gApp->CurrentPage = parentPgID; //Else set the current page to the parent page
			    }
            }
            
             goto DONE;
        }

        if ( page->PageData.PageID != page->PageData.PageParentID )
        {
            // check for inconsistency here too.
            if (CheckInconsistence((PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[page->PageData.PageID]))!= TRUE )
            {
                // reset focus to first control
                FRAME_DATA *frame = page->FrameList[StyleFrameIndexOf(MAIN_FRAME)];

                for ( i = 0; i < frame->ControlCount; i++ )
                {
                    if ( _FrameSetControlFocus( frame, i ) )
                    {
                        frame->FirstVisibleCtrl = 0;
                        break;
                    }
                }
                if(IsLinkHistorySupport())
                {
                    // tell application that we need to change pages
                    Status = GetParentFormID((UINT16)gApp->CurrentPage, &parentPgID, &stackIndex); //Gets the parent page of the current page
    				if(Status != EFI_SUCCESS)
       					gApp->CurrentPage = page->PageData.PageParentID;
    				 else
                        gApp->CurrentPage = parentPgID; //Else set the current page to the parent page
                }
                else
                {
                    gApp->CurrentPage = page->PageData.PageParentID;
                }
                gApp->CompleteRedraw = TRUE;
				gApp->OnRedraw = SUBMENU_COMPLETE_REDRAW;
            }

            Status = EFI_SUCCESS;
        }
    }
DONE:
	if (gPackUpdatePending)				//Update If any pack pending
	{
		if(!IsActiveControlPresent(page))
		{
			SETUP_DEBUG_UEFI_NOTIFICATION ("\n[TSE] PageHandleAction: ProcessPackNotificationHook\n");
			gEnableProcessPack = TRUE;
			Status = ProcessPackNotificationHook ();
			gEnableProcessPack = FALSE;
		}
	}
	if (TempCurPage != gApp->CurrentPage)
	{
	    PAGE_INFO* OldPage = (PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[TempCurPage]);
	    PAGE_INFO* NewPage = (PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[gApp->CurrentPage]);
	    PAGE_ID_INFO* OldPageIdInfo = (PAGE_ID_INFO *)(((UINT8 *) gPageIdList) + gPageIdList->PageIdList[OldPage->PageIdIndex]);
        PAGE_ID_INFO* NewPageIdInfo = (PAGE_ID_INFO *)(((UINT8 *) gPageIdList) + gPageIdList->PageIdList[NewPage->PageIdIndex]);
        EFI_STATUS  PageChangeStatus = EFI_SUCCESS;
        
        SETUP_DEBUG_TSE ("\n[TSE] SetupPageChange: From: PageID %d, FormID %d, %g, To: PageID %d, FormID %d, %g\n", 
                TempCurPage, OldPage->PageFormID, &(OldPageIdInfo->PageGuid), gApp->CurrentPage, NewPage->PageFormID, &(NewPageIdInfo->PageGuid));
        
        PageChangeStatus = SetupPageChangeHookHook(&(OldPageIdInfo->PageGuid), &(OldPage->PageFormID),&(NewPageIdInfo->PageGuid), &(NewPage->PageFormID));
        if (EFI_ACCESS_DENIED == PageChangeStatus) 
        {  //reset back, do not change pages    
            gApp->CurrentPage = TempCurPage;
            return Status;
        }
	}    
    if(IsLinkHistorySupport())
    {
        if(Status == EFI_SUCCESS)
        {
   		    SetParentFormID(page->PageData.PageID, (UINT16)gApp->CurrentPage);
	    }
    }
    if(ReconnectPageHandle && (TempCurPage != gApp->CurrentPage))
    {   
        DoDisconnectAndConnectController (TempCurPage, gApp->CurrentPage);
    }
    if (IsTseCallFormOpenOnPgChangeSupport())
	{
    	if (TempCurPage != gApp->CurrentPage)
    	{
 //   	    SETUP_DEBUG_TSE("[TSE] Changing pages from PageID %d to %d, calling AMI_CALLBACK_FORM_OPEN\n", TempCurPage,  gApp->CurrentPage );
    		UEFICallSetupFormCallBackForm((UINT16)gApp->CurrentPage, AMI_CALLBACK_FORM_OPEN );
    		HandleReadOpOnChanged(gApp->CurrentPage);			//Evaluating Read opcode for all the controls in destination page to update to the latest edited value while navigating
    	}
	}
    return Status;
}

EFI_STATUS PageSetCallback( PAGE_DATA *page, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie )
{
	return gObject.SetCallback( page, container, callback, cookie );
}

/**
    function to update the memo

    @param page UINT16 frameNumber, VOID* handle, UINT16 token

    @retval void

**/
VOID _PageUpdateMemo( PAGE_DATA *page, UINT32 frameType, /*EFI_HII_HANDLE*/VOID* handle, UINT16 token )

{
	MEMO_DATA *memo;
	FRAME_DATA *memoFrame;
	UINT32 frameIndex;

	frameIndex = StyleFrameIndexOf(frameType);	
	if ( ( page->FrameCount <= frameIndex ) || ( token == 0 ) )
		return;

	memoFrame = page->FrameList[frameIndex];


	if ( memoFrame->ControlCount == 0 )
		return;

	memo = (MEMO_DATA *)memoFrame->ControlList[0];
	memo->ControlData.ControlHandle = handle;

	if(memo->ControlData.ControlType == CONTROL_TYPE_MEMO)
		UefiSetSubTitleField((VOID *)memo->ControlData.ControlPtr,token);
}

/**
    function to update the page help

    @param page 

    @retval void

**/
VOID _PageUpdateHelp( PAGE_DATA *page )
{
	CONTROL_DATA *control;
	FRAME_DATA *mainFrame;

	mainFrame = page->FrameList[StyleFrameIndexOf(MAIN_FRAME)];

	_PageUpdateMemo( page, HELP_FRAME, gHiiHandle, STRING_TOKEN(STR_EMPTY_STRING) );
	if ( mainFrame->ControlCount == 0 )
		return;

	if(mainFrame->CurrentControl == FRAME_NONE_FOCUSED)
		return;

	control = mainFrame->ControlList[mainFrame->CurrentControl];

	if ( control->ControlFocus )
		_PageUpdateMemo( page, HELP_FRAME, control->ControlData.ControlHandle, control->ControlData.ControlHelp );

}

/**
    function to update the subtitle of the page

    @param page 

    @retval void

**/
VOID _PageUpdateSubtitle( PAGE_DATA *page )
{
	/*EFI_HII_HANDLE*/VOID * hiiHandle = page->PageData.PageHandle;
	UINT16 token = page->PageData.PageSubTitle;

	if ( token == 0 )
	{
		hiiHandle = gHiiHandle;
		token = gScreen->MainCopyright;
	}

	_PageUpdateMemo( page, SUBTITLE_FRAME, hiiHandle, token );
}

/**
    function to update the title of the page

    @param page 

    @retval void

**/
VOID _PageUpdateTitle( PAGE_DATA *page )
{
	/*EFI_HII_HANDLE*/VOID * hiiHandle = page->PageData.PageHandle;
	UINT16 token = page->PageData.PageTitle;

	if ( token == 0 )
	{
		hiiHandle = gHiiHandle;
		token = gScreen->MainTitle;
	}

	_PageUpdateMemo( page, TITLE_FRAME, hiiHandle, token );
}

/**
    function to update the title of the page

    @param page 

    @retval void

**/
VOID _PageUpdateNavStrings( PAGE_DATA *page )
{
	_PageUpdateMemo( page, NAV_FRAME, gHiiHandle, StyleGetNavToken( page->PageData.PageID ) );
}


/**
    function to update the title of the page

    @param page 

    @retval void

**/
VOID _PageUpdateFrameStrings( PAGE_DATA *page )
{
	StyleUpdateFrameStrings(page);
}

/**
    function to update the Help in case of First Item focus set.

    @param page 

    @retval void

**/
VOID UpdatePageFocusedItemHelp(PAGE_DATA *page)
{
	//call the internal function to update the page help
	_PageUpdateHelp(page);
}

/**
    function to handle the left click on ESC in Navigation frame.

    @param VOID

    @retval VOID

**/
VOID HotclickESCaction(VOID)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
	PAGE_DATA *page;
	UINT32 stackIndex = 0xFFFFFFFF;
    UINT16 parentPgID = 0;
	  page = gApp->PageList[gApp->CurrentPage];

		if ( page->PageData.PageParentID == 0 )
		{
		    HandleExitApplicationNoMsg();//if no user changes present then exit without showing any MsgBox
            if(!gApp->Quit)
                gApp->Quit = ExitApplication();
			return;
		}
	
	    if ( page->PageData.PageID != page->PageData.PageParentID )
        {
            // check for inconsistency here too.
            if (CheckInconsistence((PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[page->PageData.PageID]))!= TRUE )
            {
                // reset focus to first control
                FRAME_DATA *frame = page->FrameList[StyleFrameIndexOf(MAIN_FRAME)];
                UINT32 i;

                for ( i = 0; i < frame->ControlCount; i++ )
                {
                    if ( _FrameSetControlFocus( frame, i ) )
                    {
                        frame->FirstVisibleCtrl = 0;
                        break;
                    }
                }

                if(IsLinkHistorySupport())
                {
                    // tell application that we need to change pages
                    Status = GetParentFormID((UINT16)gApp->CurrentPage, &parentPgID, &stackIndex); //Gets the parent page of the current page
    				if(Status != EFI_SUCCESS)
       					gApp->CurrentPage = page->PageData.PageParentID;
    					//gApp->CurrentPage = gNavStartPage; //If the parent page was not found or invalid, go to the first page
    				 else
                        gApp->CurrentPage = parentPgID; //Else set the current page to the parent page
                }
                else
                {
                    gApp->CurrentPage = page->PageData.PageParentID;
                }

                gApp->CompleteRedraw = TRUE;
				gApp->OnRedraw = SUBMENU_COMPLETE_REDRAW;
            }

            Status = EFI_SUCCESS;
        }
		
		
	    if(IsLinkHistorySupport())
	    {
	        if(Status == EFI_SUCCESS)
	        {
	   		    SetParentFormID(page->PageData.PageID, (UINT16)gApp->CurrentPage);
		    }
	    }
}

/**
    Function to handle PrintScreenKeyto get screen image as BltBuffer and save as BMP file to a available filesystem

    @param VOID

    @retval EFI_STATUS

**/
EFI_STATUS WriteDataToFileWrapper (CHAR16 *filename, VOID *Data, UINTN length, UINT32 index);
EFI_STATUS PrntScrnKeyHandleaction()
{
	BMP_IMAGE_HEADER  *BmpBuffer=NULL;
	EFI_STATUS 	Status = EFI_SUCCESS;
	EFI_TIME		time;

	CHAR16	*Title		= L"Snap of current page:";
	CHAR16	*Msg 			= NULL;
	CHAR16 	*Filename	= NULL;
	UINT8 	Sel = 0;
	UINTN 	StartX = 0, StartY = 0, Width = 0, Height = 0;
	UINT32 	variable = 0xffff;


	gAddBgrtResolutions = 1;
	Height = (UINTN)(gGOP->Mode->Info->VerticalResolution);
	Width = (UINTN)(gGOP->Mode->Info->HorizontalResolution);

	SetGetBgrtCoordinates (&StartX,&StartY,&Width,&Height,TRUE);

	//Converting blt buffer to BMP
	BmpBuffer =	ConvertBlt2Bmp ();

	if (BmpBuffer)
	{
		//List available FileSystem
		Status = FileBrowserLaunchFileSystem(variable);

		if (EFI_ERROR(Status))
		{
			PostManagerDisplayMsgBox (L"PrintScreen Error!!",L"Screen is not captured",MSGBOX_TYPE_OK,&Sel);
			goto DONE;
		}

		gRT->GetTime( &time, NULL );

		if (NULL == gPageTitle)
		{
			gPageTitle = EfiLibAllocateZeroPool ( (EfiStrLen(L"TSE_Snap")+1)*sizeof(CHAR16) );
			StrCpyS (gPageTitle, EfiStrLen(L"TSE_Snap")+1, L"TSE_Snap");

		}
	
		Filename = EfiLibAllocateZeroPool ( (EfiStrLen(gPageTitle)+16)*sizeof(CHAR16));//length of title and time format (HH-MM-SS) and .BMP character
		if (NULL == Filename)
		{
			Status = EFI_OUT_OF_RESOURCES;
			goto DONE;
		}

		SPrint(Filename,((EfiStrLen(gPageTitle)+16)*sizeof(CHAR16)),L"%s_[%02d-%02d-%02d].BMP",gPageTitle,time.Hour,time.Minute,time.Second );
		Status = WriteDataToFileWrapper (Filename,BmpBuffer,BmpBuffer->Size,gSelIdx);

		if (!EFI_ERROR(Status))
		{
			Msg = EfiLibAllocateZeroPool( (EfiStrLen(L"Image is stored with name : ") + EfiStrLen (Filename) + 1) * sizeof(CHAR16) );	
		
			if (NULL == Msg)
			{
				Status = EFI_OUT_OF_RESOURCES;
				goto DONE;
			}

			SPrint(Msg,( (EfiStrLen(L"Image is stored with name : ") + EfiStrLen (Filename)+1) * sizeof(CHAR16) ),L"Image is stored with name : %s",Filename);
			PostManagerDisplayMsgBox (Title,Msg,MSGBOX_TYPE_OK,&Sel);
		}

		else
			PostManagerDisplayMsgBox (L"Error Message",L"Failed to save to a file",MSGBOX_TYPE_OK,&Sel);		
	}

DONE:
	if (BmpBuffer)
		MemFreePointer((VOID **)&BmpBuffer);
	if (Filename)
		MemFreePointer((VOID **)&Filename);
	if (Msg)
		MemFreePointer((VOID **)&Msg);
	if (gPageTitle)
		MemFreePointer ((VOID **)&gPageTitle);

	return Status;
}

/**
    PrntScrnKeyNotification Function handle PrntScrnKey action

    @param CHAR16 * 

    @retval EFI_STATUS

**/
VOID PrntScrnKeyNotification(APPLICATION_DATA *app, HOTKEY_DATA *hotkey, VOID *cookie )
{
	if ( TRUE == TsePrintScreenEventSupport ( ((HOTKEY_DATA*)hotkey)->HotKeyData.Key.ScanCode ) )
		PrntScrnKeyHandleaction();
}
