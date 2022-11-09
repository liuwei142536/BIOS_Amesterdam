//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file PopupSel.c
    This file contains code to handle Popup selections

**/

#include "minisetup.h"

BOOLEAN OneofMultilineDisable(POPUPSEL_DATA *popupSel);
VOID _PopupSelGetSelection( POPUPSEL_DATA *popupSel );
VOID GetValidCurrentOption(POPUPSEL_DATA *popupSel, UINT16 Action);
static BOOLEAN bSelectionChanged = FALSE;

extern EFI_STATUS _FrameScroll( FRAME_DATA *frame, BOOLEAN bScrollUp );
extern UINT16 _gHDOrderPageID;
extern UINT16 _gCDOrderPageID;
extern UINT16 _gFDOrderPageID;
extern UINT16 _gNetOrderPageID;
extern UINT16 _gBevOrderPageID;
extern EFI_STATUS UefiGetValidOptionsCondition(CONTROL_INFO *CtrlInfo, POSTMENU_TEMPLATE* ItemInfo, UINT16 MaxItems);
VOID _PopupSelGetSelectionBitFieldVarstore( POPUPSEL_DATA *popupSel );
POPUPSEL_METHODS gPopupSel =
{
		(OBJECT_METHOD_CREATE)PopupSelCreate,
		(OBJECT_METHOD_DESTROY)PopupSelDestroy,
		(OBJECT_METHOD_INITIALIZE)PopupSelInitialize,
		(OBJECT_METHOD_DRAW)PopupSelDraw,
		(OBJECT_METHOD_HANDLE_ACTION)PopupSelHandleAction,
		(OBJECT_METHOD_SET_CALLBACK)PopupSelSetCallback,
		(CONTROL_METHOD_SET_FOCUS)PopupSelSetFocus,
		(CONTROL_METHOD_SET_POSITION)PopupSelSetPosition,
		(CONTROL_METHOD_SET_DIMENSIONS)PopupSelSetDimensions,
		(CONTROL_METHOD_SET_ATTRIBUTES)PopupSelSetAttributes,
		(CONTROL_METHOD_GET_CONTROL_HIGHT)PopupSelGetControlHeight		

};

/**
    this function uses the create function of control
    and creates the Popup Selected

    @param object 

    @retval status

**/
EFI_STATUS PopupSelCreate( POPUPSEL_DATA **object )
{
	EFI_STATUS Status = EFI_OUT_OF_RESOURCES;

	if ( *object == NULL )
	{
		*object = EfiLibAllocateZeroPool( sizeof(POPUPSEL_DATA) );

		if ( *object == NULL )
			return Status;
	}

	Status = gControl.Create( (void**)object );
	if ( ! EFI_ERROR(Status) )
		(*object)->Methods = &gPopupSel;

	return Status;
}

/**
    This function destroys PopupSel control
    Every member ptr within control structure has to be freed regardless of freeMem (true/false). 
    When freeMem is TRUE, free control data [POPUPSEL_DATA]
    Destroy() has to be called with FALSE inside another Destroy(), 
    unless it’s an explicit member object that is allocated during Initialize or Create [gListBox].
    Destroy() has to be called with TRUE during control removal.


    @param popupSel BOOLEAN freeMem

    @retval status

**/
EFI_STATUS PopupSelDestroy( POPUPSEL_DATA *popupSel, BOOLEAN freeMem )
{
	if(NULL == popupSel)
	  return EFI_SUCCESS;

	gControl.Destroy( popupSel, FALSE );

	if ( popupSel->ListBoxCtrl )
		gListBox.Destroy( popupSel->ListBoxCtrl, TRUE );

	popupSel->ListBoxCtrl = NULL;

    MemFreePointer( (VOID **)&(popupSel->PtrTokens) );
	if( freeMem )
	{
		MemFreePointer( (VOID **)&popupSel );
	}
	return EFI_SUCCESS;
}

/**
    this function uses the initialize function of control
    and initializes the Popup Selected

    @param popupSel VOID *data

    @retval status

**/
EFI_STATUS PopupSelInitialize( POPUPSEL_DATA *popupSel, VOID *data )
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
	UINT16 Index = 0;
	UINT16 *OptionList=NULL;
	UINT64 *ValueList=NULL;


	Status = gControl.Initialize( popupSel, data );
	if (EFI_ERROR(Status))
		return Status;

	popupSel->PopupSelHandle = popupSel->ControlData.ControlHandle;
	popupSel->Title = UefiGetPromptField(popupSel->ControlData.ControlPtr);

	// add extra initialization here...
	Status = UefiGetOneOfOptions(&popupSel->ControlData,&popupSel->PopupSelHandle,
			&OptionList, &ValueList, &popupSel->ItemCount,&popupSel->bInteractive,&popupSel->CallBackKey);

	if (EFI_ERROR(Status))
		return Status;

	popupSel->Interval = (UINT8)(popupSel->ControlData.ControlFlags.ControlRefresh);

	if( popupSel->PtrTokens != NULL )
      MemFreePointer( (VOID **)&(popupSel->PtrTokens));
	popupSel->PtrTokens = EfiLibAllocatePool( popupSel->ItemCount * sizeof(PTRTOKENS) );
	if ( popupSel->PtrTokens == NULL )
		Status = EFI_OUT_OF_RESOURCES;
	else
	{
		for( Index = 0; Index < popupSel->ItemCount; Index++ )
		{
			popupSel->PtrTokens[Index].Option = OptionList[Index];
			popupSel->PtrTokens[Index].Value = ValueList[Index];
		}

	    MemFreePointer( (VOID **)&(OptionList));
	    MemFreePointer( (VOID **)&(ValueList));

		if(popupSel->ControlData.ControlFlags.ControlInteractive)
	    {
			popupSel->bInteractive = TRUE;
			popupSel->CallBackKey = popupSel->ControlData.ControlKey;
		}

	    // boundary overflow  check

		popupSel->ListBoxCtrl = NULL;
		popupSel->ListBoxEnd = FALSE;
		popupSel->LabelMargin = (UINT8)gControlLeftMargin;

		// We dont have to do Get selection here it is done before we draw a page
		//_PopupSelGetSelection( popupSel );

		SetControlColorsHook(&(popupSel->BGColor), &(popupSel->FGColor), NULL, NULL ,&(popupSel->SelBGColor), &(popupSel->SelFGColor),
			                    NULL , &(popupSel->NSelFGColor),NULL,&(popupSel->LabelFGColor) ,
								&(popupSel->NSelLabelFGColor),NULL ,NULL,NULL, NULL );
	
	}

	return Status;
}

/**
    function to draw the specific popup window

    @param popupSel 

    @retval status

**/
EFI_STATUS PopupSelDraw( POPUPSEL_DATA *popupSel )
{
	CHAR16 *text=NULL,*text1=NULL;
	UINTN i=0,Len=0;
	EFI_STATUS Status = EFI_SUCCESS;
	UINT16 Index = 0, TempHeight=0;
	UINT16 *OptionList=NULL;
	UINT64 *ValueList=NULL;
	UINT16 TempItemCount = 0;
	
	UINT16 tmpHeight=0;

	UINT8 ColorNSel = popupSel->NSelFGColor;
	UINT8 ColorLabel = (popupSel->ControlFocus) ? popupSel->LabelFGColor : popupSel->NSelLabelFGColor ;

	if(  popupSel->ListBoxCtrl != NULL)
	{
		gListBox.Draw( popupSel->ListBoxCtrl );
	}
	else
	{
		// check conditional ptr if necessary
		//if( popupSel->ControlData.ControlConditionalPtr != 0x0)
		//{
			switch( CheckControlCondition( &popupSel->ControlData ) )
			{
				case COND_NONE:
					break;
				case COND_GRAYOUT:
					Status = EFI_WARN_WRITE_FAILURE;
					ColorNSel =  ColorLabel = CONTROL_GRAYOUT_COLOR;
					break;
				default:
					return EFI_UNSUPPORTED;
					break;
			}
		//}
		// If not Special
		if(TseLiteIsSpecialOptionList((CONTROL_DATA *)popupSel) != TRUE)
		{
			TempItemCount = popupSel->ItemCount;
			Status = UefiGetOneOfOptions(&popupSel->ControlData,&popupSel->PopupSelHandle,
					&OptionList, &ValueList, &popupSel->ItemCount,NULL,NULL);
			if (EFI_ERROR(Status))
				return Status;
			//In some cases browser callback can change the variable values. So ItemCount can be changed from PopupSelInitialize to PopupSelDraw.
			//so reinitializing the memory else adding more entries will assert while freeing.
			if (TempItemCount != popupSel->ItemCount)
			{
				if( popupSel->PtrTokens != NULL )
			      MemFreePointer( (VOID **)&(popupSel->PtrTokens));
				popupSel->PtrTokens = EfiLibAllocatePool( popupSel->ItemCount * sizeof(PTRTOKENS) );
				if ( popupSel->PtrTokens == NULL )
				{
					Status = EFI_OUT_OF_RESOURCES;
					return Status;
				}
			}
			for( Index = 0; Index < popupSel->ItemCount; Index++ )
			{
				popupSel->PtrTokens[Index].Option = OptionList[Index];
				popupSel->PtrTokens[Index].Value = ValueList[Index];
			}
			MemFreePointer( (VOID **)&(OptionList));
			MemFreePointer( (VOID **)&(ValueList));
		}

		_PopupSelGetSelection( popupSel );
		text1 = HiiGetString( popupSel->ControlData.ControlHandle, popupSel->Title );
		if ( text1 != NULL )
		{
			if(popupSel->Height>1 && IsTSEMultilineControlSupported())
			{	
				DrawMultiLineStringWithAttribute( (UINTN)popupSel->Left, (UINTN) popupSel->Top, 
						(UINTN)(popupSel->LabelMargin - popupSel->Left),(UINTN) popupSel->Height,
						text1, popupSel->BGColor |  ColorLabel );
			}
			else
			{
	            // boundary overflow  check
				if((TestPrintLength( text1) / (NG_SIZE))> (UINTN)(popupSel->LabelMargin - popupSel->Left  ))
					text1[HiiFindStrPrintBoundary(text1 ,(UINTN)(popupSel->LabelMargin - popupSel->Left  ))] = L'\0';
	
				DrawStringWithAttribute( popupSel->Left , popupSel->Top, (CHAR16*)text1, 
						popupSel->BGColor |  ColorLabel  );

			}
			MemFreePointer( (VOID **)&text1 );
		}
		
		/*EfiStrCpy(text, L"[");
		DrawStringWithAttribute( popupSel->Left + popupSel->LabelMargin , popupSel->Top, (CHAR16*)text, 
				(UINT8)( (popupSel->ControlFocus) ? 
						 popupSel->SelBGColor  | popupSel->SelFGColor : 
						  popupSel->BGColor  | ColorNSel ));
*/
		if (popupSel->ItemCount)
		    text1 = HiiGetString( popupSel->PopupSelHandle, popupSel->PtrTokens[popupSel->Sel].Option);
		else
		    text1 = NULL;
		
		if(popupSel->Height>1 && OneofMultilineDisable(popupSel))
		{
			TempHeight = popupSel->Height;
			popupSel->Height = 1;
		}
		
		if(popupSel->Height>1 && IsTSEMultilineControlSupported())
		{
			Len = TestPrintLength( text1 ) / (NG_SIZE);
			text = EfiLibAllocateZeroPool( (Len+3)*2 ); 
			
			if(text1)
			    SPrint( text,(UINTN)( (Len+3)*2 )/*(popupSel->Width - popupSel->Left - popupSel->LabelMargin +2)*/, L"[%s]", text1 );
			else
			    SPrint( text,(UINTN)( (Len+3)*2 ), L"[]");
			
			MemFreePointer( (VOID **)&text1 );	

			text1 = StringWrapText( (CHAR16*)text, (popupSel->Width - popupSel->LabelMargin ), &tmpHeight );

			if(tmpHeight == 1)
				DrawStringWithAttribute( popupSel->Left + popupSel->LabelMargin  , popupSel->Top, (CHAR16*)text, 
				(UINT8)( (popupSel->ControlFocus) ? popupSel->SelBGColor  | popupSel->SelFGColor : popupSel->BGColor  | ColorNSel   ));
			else
				DrawMultiLineStringWithAttribute( (UINTN)popupSel->Left + (UINTN)popupSel->LabelMargin, (UINTN) popupSel->Top,
												(UINTN)(popupSel->Width - popupSel->LabelMargin),(UINTN)popupSel->Height, (CHAR16*)text,
												(UINT8)( (popupSel->ControlFocus)? popupSel->SelBGColor  | popupSel->SelFGColor : 
												popupSel->BGColor  | ColorNSel   ));
			
			MemFreePointer( (VOID **)&text1 );

        }
		else
		{
		    UINTN DestMax;
    		if(!text1)
    			text1 = EfiLibAllocateZeroPool(2);
    
    		//MemSet( text, sizeof(text), 0 );
    		Len = TestPrintLength( text1 ) / (NG_SIZE);
    		// XXX: may be broken for wide character languages !!!
    		if ( Len /*StringDisplayLen( text1 )*/ > /*19*/(UINTN)(popupSel->Width - popupSel->LabelMargin -2/*4*/)/*gControlRightAreaWidth*/ ) // Adjusted the cordinates to show maximum string in control left area
    		{  
			    DestMax =(EfiStrLen(text1)+1) - HiiFindStrPrintBoundary(text1 ,(UINTN)(popupSel->Width - popupSel->LabelMargin -5/*16*/));
    		    StrCpyS(&text1[HiiFindStrPrintBoundary(text1 ,(UINTN)(popupSel->Width - popupSel->LabelMargin -5/*16*/))], DestMax, L"..." );  
    		}
            text = EfiLibAllocateZeroPool( (Len+3)*2 ); 
			
            if(text1)
    		SPrint( text,(UINTN)( (Len+3)*2 )/*(popupSel->Width - popupSel->Left - popupSel->LabelMargin +2)*/, L"[%s]", text1 );
            else
                SPrint( text,(UINTN)( (Len+3)*2 ), L"[]");
            
            MemFreePointer( (VOID **)&text1 );
    		DrawStringWithAttribute( popupSel->Left + popupSel->LabelMargin  , popupSel->Top, (CHAR16*)text, 
    				(UINT8)( (popupSel->ControlFocus) ? 
    						 popupSel->SelBGColor  | popupSel->SelFGColor : 
    						  popupSel->BGColor  | ColorNSel   ));

        }
		
		if((TempHeight > 0) && OneofMultilineDisable(popupSel))
		{
			 popupSel->Height = TempHeight;
		}
  		i=TestPrintLength(text) / (NG_SIZE);
        MemFreePointer( (VOID **)&text );
		/*EfiStrCpy(text, L"]");
		DrawStringWithAttribute( popupSel->Left + popupSel->LabelMargin +i +1, popupSel->Top, (CHAR16*)text, 
				(UINT8)( (popupSel->ControlFocus) ? 
						 popupSel->SelBGColor  | popupSel->SelFGColor : 
						  popupSel->BGColor  | ColorNSel ));
*/
        // erase extra spaces if neccessary
		for(;(UINT16)(popupSel->Left + popupSel->LabelMargin + i) <= (popupSel->Width-1);i++)
			DrawStringWithAttribute( popupSel->Left + popupSel->LabelMargin +i, popupSel->Top, L" ", 
						 popupSel->BGColor  |  ColorNSel );

		FlushLines( popupSel->Top, popupSel->Top+popupSel->Height );
	}

	return Status;

}
/**
    Function to handle the PopupString actions

    @param popupSel OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie 

    @retval STATUS

**/
EFI_STATUS PopupSelSetCallback( POPUPSEL_DATA *popupSel, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie )
{
	return gControl.SetCallback( popupSel, container, callback, cookie );
}

/**
    Function to delete the boot option

    @param POPUPSEL_DATA *

    @retval BOOLEAN

**/
BOOLEAN DoBootDelBootOption (POPUPSEL_DATA *popupSel)
{
    if( popupSel->ControlData.ControlVariable == VARIABLE_ID_DEL_BOOT_OPTION )
    {
        //delete selected option
        BootDelBootOption( (UINT16) popupSel->PtrTokens[popupSel->Sel].Value );

        //Complete redraw
        gApp->CompleteRedraw = TRUE;

        //Variable should not be set so return
        return 1;
    }
	return 0;
}

/**
    Function to delete the driver option

    @param POPUPSEL_DATA *

    @retval BOOLEAN

**/
BOOLEAN DoDriverDelDriverOption(POPUPSEL_DATA * popupSel)
{
    if (VARIABLE_ID_DEL_DRIVER_OPTION == popupSel->ControlData.ControlVariable)
    {
        //delete selected option
       	DriverDelDriverOption ((UINT16) popupSel->PtrTokens[popupSel->Sel].Value);

        //Complete redraw
        gApp->CompleteRedraw = TRUE;

        //Variable should not be set so return
        return 1;
    }
	return 0;
}

/**
    Callback function of the PopupSel

    @param popupSel 

    @retval void

**/
VOID DoPopupSelCallBack(POPUPSEL_DATA **popupSel)
{
//#if SETUP_SUPPORT_ADD_BOOT_OPTION
	if( TseDoBootDelBootOption(*popupSel) )
		return;
//#endif

	if( TseDoDriverDelDriverOption(*popupSel) )		
		return;

    if ( (*popupSel)->Cookie != NULL )
	{
        VOID *ifrData = (VOID *)(*popupSel)->ControlData.ControlPtr;
		CALLBACK_VARIABLE *callbackData = (CALLBACK_VARIABLE *)(*popupSel)->Cookie;

		callbackData->Variable = (*popupSel)->ControlData.ControlVariable;
		callbackData->Offset = UefiGetQuestionOffset(ifrData);
		callbackData->Length = UefiGetWidth(ifrData);
		callbackData->Data = (VOID*)&((*popupSel)->PtrTokens[(*popupSel)->Sel].Value);
	}
	// To support UEFI 2.1.C spec to Not to update the control when Callback fails.
	UefiPreControlUpdate(&((*popupSel)->ControlData));
	(*popupSel)->Callback( (*popupSel)->Container, (*popupSel), (*popupSel)->Cookie );

    if((*popupSel)->bInteractive)
    {   
//        EFI_STATUS Status; Set unused
		VOID * Handle=(*popupSel)->ControlData.ControlHandle;
		UINT16 Key = (*popupSel)->CallBackKey;
		CONTROL_DATA *Control;

//		Status = CallFormCallBack(&((*popupSel)->ControlData),(*popupSel)->CallBackKey,0, AMI_CALLBACK_CONTROL_UPDATE);//Updated the action to AMI_CALLBACK_CONTROL_UPDATE
		 CallFormCallBack(&((*popupSel)->ControlData),(*popupSel)->CallBackKey,0, AMI_CALLBACK_CONTROL_UPDATE);// Updated the action to AMI_CALLBACK_CONTROL_UPDATE
		Control = GetUpdatedControlData((CONTROL_DATA*)(*popupSel),(*popupSel)->ControlData.ControlType,Handle,Key);

	
		if(Control == NULL){ // Control deleted ?
			*popupSel = NULL;
             return;
        }
		if(Control != (CONTROL_DATA*)(*popupSel))
			*popupSel = (POPUPSEL_DATA*)Control; //control Updated
    }
    if( GetTseInconsistentPolicy()){
        CheckInconsistence((PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[gApp->CurrentPage]));
    }

}

/**
    function to handle the PopupSel Action keys

    @param popupSel AMI_EFI_KEY_DATA key

    @retval status

**/
EFI_STATUS _PopupSelHandleActionKeyMouse(POPUPSEL_DATA *popupSel, ACTION_DATA *Data)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
    CONTROL_ACTION Action=ControlActionUnknown;
	AMI_EFI_KEY_DATA key=Data->Input.Data.AmiKey;

	if(Data->Input.Type == ACTION_TYPE_MOUSE)
	{
		//check whether MouseTop is within the Height and Width of PopupSel or not
		if((Data->Input.Data.MouseInfo.Top >= (UINT32)popupSel->Top) && (Data->Input.Data.MouseInfo.Top < (UINT32)(popupSel->Top+popupSel->Height)) &&
		   (Data->Input.Data.MouseInfo.Left >= (UINT32)popupSel->Left) && (Data->Input.Data.MouseInfo.Left < (UINT32)(popupSel->Left+popupSel->Width)))
			Action = MapControlMouseActionHook(&Data->Input.Data.MouseInfo);
			// can be only ControlActionSelect;
			
	}

	if(Data->Input.Type == ACTION_TYPE_KEY)
 	   Action = MapControlKeysHook(key);

    switch(Action)
    {
        case ControlActionDecreament:
			//Handle special case for boot order & Legacy dev order
			if (
				(VARIABLE_ID_BOOT_ORDER == popupSel->ControlData.ControlVariable) ||
				(VARIABLE_ID_BBS_ORDER == popupSel->ControlData.ControlVariable) ||
				(VARIABLE_ID_DRIVER_ORDER == popupSel->ControlData.ControlVariable)
				)
			{
				RearrangeBootDriverOrderVariable (popupSel, FALSE);
			}
            else if( ( CheckForAddDelBootOption() ) && 
					 (popupSel->ControlData.ControlVariable == VARIABLE_ID_DEL_BOOT_OPTION ) )
            {
            }
            else if( ( CheckForAddDelDriverOption() ) && 
					 (popupSel->ControlData.ControlVariable == VARIABLE_ID_DEL_DRIVER_OPTION ) )
			 {
			 }
			else
			{
				if ( popupSel->Sel > 0 )
					popupSel->Sel--;
				else
					popupSel->Sel = popupSel->ItemCount - 1;

                GetValidCurrentOption(popupSel, ControlActionDecreament);       //Getting valid focusible item in popupSel->Sel
				    
				DoPopupSelCallBack(&popupSel);
                if(popupSel == NULL)
                    return EFI_SUCCESS;
			}
			Status = EFI_SUCCESS;
        break;

        case ControlActionIncreament:
			//Handle special case for boot order & Legacy dev order
			if (
				(VARIABLE_ID_BOOT_ORDER == popupSel->ControlData.ControlVariable) ||
				(VARIABLE_ID_BBS_ORDER == popupSel->ControlData.ControlVariable) ||
				(VARIABLE_ID_DRIVER_ORDER == popupSel->ControlData.ControlVariable)
				)
			{
				RearrangeBootDriverOrderVariable (popupSel, TRUE);
			}
            else if( ( CheckForAddDelBootOption() ) && 
					 (popupSel->ControlData.ControlVariable == VARIABLE_ID_DEL_BOOT_OPTION ) )
            {
            }
            else if( ( CheckForAddDelDriverOption() ) && 
					 (popupSel->ControlData.ControlVariable == VARIABLE_ID_DEL_DRIVER_OPTION ) )
			 {
			 }
			else
			{
				if ( popupSel->Sel < (UINTN)(popupSel->ItemCount - 1) )
					popupSel->Sel++;
				else
					popupSel->Sel = 0;

				GetValidCurrentOption(popupSel, ControlActionIncreament);   //Getting valid focusible item in popupSel->Sel
                
				DoPopupSelCallBack(&popupSel);
                if(popupSel == NULL)
                    return EFI_SUCCESS;
			}
			Status = EFI_SUCCESS;
        break;

        case ControlActionSelect:
            if(!popupSel->ItemCount)
                return EFI_SUCCESS;
			if ( gListBox.Create( (void**)&(popupSel->ListBoxCtrl) ) == EFI_SUCCESS )
			{
				popupSel->ControlActive = TRUE;
				gListBox.Initialize( popupSel->ListBoxCtrl, &(popupSel->ControlData) );
				if(TseLiteIsSpecialOptionList((CONTROL_DATA *)popupSel) == TRUE)
				{
					UINT16 TempLength=0, i;
					//Need fix the ListBox options
					for( i = 0 ; i < popupSel->ItemCount; i++ )
					{
						popupSel->ListBoxCtrl->PtrTokens[i] = popupSel->PtrTokens[i].Option;
						TempLength = (UINT16)HiiMyGetStringLength( popupSel->ListBoxCtrl->ListHandle, popupSel->ListBoxCtrl->PtrTokens[i] );
						TempLength+=5; // to included barders
						if ( TempLength > popupSel->ListBoxCtrl->Width )
							popupSel->ListBoxCtrl->Width = TempLength;
					}

				}
		
				if(popupSel->ListBoxCtrl->Width > (UINT16)(gMaxCols-5))
					popupSel->ListBoxCtrl->Width = (UINT16)(gMaxCols-5);
				popupSel->ListBoxCtrl->Sel = popupSel->Sel;
				gListBox.SetCallback(popupSel->ListBoxCtrl, popupSel, (OBJECT_CALLBACK) _PopupSelCallback, NULL);
				
				MouseStop();

				gListBox.Draw( popupSel->ListBoxCtrl );

				MouseRefresh();
			} 
			Status = EFI_SUCCESS; 
        break;

        default:
        break;
    }
    return Status;
}

/**
    function to handle the PopupSel Actions

    @param popupSel ACTION_DATA *Data

    @retval status

**/
EFI_STATUS PopupSelHandleAction( POPUPSEL_DATA *popupSel, ACTION_DATA *Data)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
    UINT8 u8ChkResult;

	if ( Data->Input.Type == ACTION_TYPE_TIMER )
	{
		// List box is active don't refresh
		if ( popupSel->ListBoxCtrl != NULL )
			return Status;

		if (popupSel->Interval == 0)
			return Status;

		if(IsActiveControlPresent(gApp->PageList[gApp->CurrentPage]))
			return Status;
		
		if ( --(popupSel->Interval) == 0 )
		{
			// initialize the interval
			popupSel->Interval = (UINT8)(popupSel->ControlData.ControlFlags.ControlRefresh);
			return UefiRefershQuestionValueNvRAM(&(popupSel->ControlData));
		}
		else
			return Status;
	}

    u8ChkResult = CheckControlCondition( &popupSel->ControlData );
	 
    //Not to perform action for control when token TSE_SETUP_GRAYOUT_SELECTABLE is enable to set focus for GrayoutIf control
    if ( IsGrayoutSelectable() && (COND_GRAYOUT == u8ChkResult) ) {
        return EFI_UNSUPPORTED;
    }

    if(popupSel->ControlData.ControlFlags.ControlReadOnly)
		return EFI_UNSUPPORTED;

	if(!popupSel->ControlFocus)
		return Status;

    if ( popupSel->ListBoxCtrl != NULL )
    {
    	Status = gListBox.HandleAction(popupSel->ListBoxCtrl,Data);
    	if ( popupSel->ListBoxEnd )
    	{
            if(bSelectionChanged)
            {
                if(popupSel->Sel != popupSel->ListBoxCtrl->Sel)
                {
                    popupSel->Sel = popupSel->ListBoxCtrl->Sel;
           			DoPopupSelCallBack(&popupSel);
                    if(popupSel == NULL)
                        return EFI_SUCCESS;

                }
                bSelectionChanged = FALSE;
            }
    
    		gListBox.Destroy(popupSel->ListBoxCtrl,TRUE);
    		popupSel->ListBoxCtrl = NULL;
    		popupSel->ListBoxEnd = FALSE;
    		popupSel->ControlActive = FALSE;
    	}
    }
    else
    {
        if((Data->Input.Type == ACTION_TYPE_KEY) || (Data->Input.Type == ACTION_TYPE_MOUSE))
        {
            return _PopupSelHandleActionKeyMouse(popupSel, Data);
		}
	}
	return Status;
}

/**
    Function to set focus

    @param popupSel BOOLEAN focus

    @retval Status

**/
EFI_STATUS PopupSelSetFocus(POPUPSEL_DATA *popupSel, BOOLEAN focus)
{
	UINT8 u8ChkResult;
	
	if(focus != FALSE)
	{
		u8ChkResult = CheckControlCondition( &popupSel->ControlData );
        
		//Setting focus to control which has no condtion and token TSE_SETUP_GRAYOUT_SELECTABLE is enable to make GrayoutIf control to focus and selectable
		if (	((u8ChkResult != COND_NONE) && (u8ChkResult != COND_GRAYOUT)) ||
				(!IsGrayoutSelectable() && (u8ChkResult == COND_GRAYOUT))
			){
			return EFI_UNSUPPORTED;
		}
	}

	if( !(popupSel->ControlFocus && focus) )
		popupSel->ControlFocus = focus;

	return EFI_SUCCESS;
}

/**
    Function to set position.

    @param popupSel UINT16 Left, UINT16 Top

    @retval STATUS

**/
EFI_STATUS PopupSelSetPosition(POPUPSEL_DATA *popupSel, UINT16 Left, UINT16 Top )
{
	return gControl.SetPosition((CONTROL_DATA*) popupSel, Left, Top );
}

/**
    Function to set dimension.

    @param popupSel UINT16 Width, UINT16 Height

    @retval STATUS

**/
EFI_STATUS PopupSelSetDimensions(POPUPSEL_DATA *popupSel, UINT16 Width, UINT16 Height )
{
	return gControl.SetDimensions( (CONTROL_DATA*)popupSel, Width, Height );
}

/**
    Function to set attributes.

    @param popupSel UINT8 FGColor, UINT8 BGColor

    @retval STATUS

**/
EFI_STATUS PopupSelSetAttributes(POPUPSEL_DATA *popupSel, UINT8 FGColor, UINT8 BGColor )
{
	return gControl.SetAttributes((CONTROL_DATA*)popupSel, FGColor, BGColor );
}

/**
    PopupSel callback function.

    @param container CONTROL_DATA *popupSel, VOID *cookie

    @retval VOID

**/
VOID _PopupSelCallback( POPUPSEL_DATA *container, CONTROL_DATA *popupSel, VOID *cookie )
{
	container->ListBoxEnd = TRUE;
	if ( cookie != NULL )
    {
        bSelectionChanged = TRUE;
    }
}

/**
    function to handle the PopupSel set and get operations for Bitwise variables

    @param popupSel 

    @retval status

**/
VOID _PopupSelGetSelectionBitFieldVarstore( POPUPSEL_DATA *popupSel )
{
    VOID *temp;
    VOID *ifrData = popupSel->ControlData.ControlPtr;
    int i=0;
    EFI_STATUS Status = EFI_SUCCESS;
    NVRAM_VARIABLE *list = gVariableList;

    if (( list == NULL ) )
    {
        list = gOptimalDefaults;
    }
    
    popupSel->Sel = 0;
    
    if (!popupSel->ItemCount)
        return;

    temp = EfiLibAllocateZeroPool( 8 * 2 );
    if ( temp == NULL )
        return;

    Status = _VarGetSetValueForBitField( VAR_COMMAND_GET_VALUE, list, popupSel->ControlData.ControlVariable, 
            UefiGetQuestionOffset(ifrData),  UefiGetWidth(ifrData), 
            temp, ifrData );
    if (EFI_ERROR (Status)) 
    {
        return;
    }

    while( i< popupSel->ItemCount)
    {
        if(popupSel->PtrTokens[i].Value == *(UINT64 *)temp)
        {
            popupSel->Sel = (UINT16)i;
            break;
        }
        i++;
    }
    if(i < popupSel->ItemCount) {
            //Getting valid focusible item in popupSel->Sel. If there is any grayout condition in option list and it is satisfying to true to the current selected value.
        GetValidCurrentOption(popupSel, ControlActionSelect);
        if (popupSel->Sel != i){
                //Doing VarSetValue with the updated value corresponding to the current selection.
            Status = _VarGetSetValueForBitField( VAR_COMMAND_SET_VALUE, list, popupSel->ControlData.ControlVariable, 
                UefiGetQuestionOffset(ifrData),  
                UefiGetWidth(ifrData), 
                &(popupSel->PtrTokens[popupSel->Sel].Value), ifrData );
            if (EFI_ERROR (Status)) 
            {
                return;
            }
            HandleWriteOpOnChanged(&(popupSel->ControlData));       //Write opcode for a current control when the control value is edited
            HandleReadOpOnChanged(gApp->CurrentPage);               //Evaluating Read opcode for all the controls in current page to update to the latest edited value
        }
    }
    MemFreePointer( (VOID **)&temp );
}

/**
    function to handle the PopupSel set and get operations

    @param popupSel 

    @retval status

**/
VOID _PopupSelGetSelection( POPUPSEL_DATA *popupSel )
{
	VOID *temp;
	VOID *ifrData = popupSel->ControlData.ControlPtr;
	int i=0;
	EFI_STATUS Status = EFI_SUCCESS;
	
	popupSel->Sel = 0;
	
	if (!popupSel->ItemCount)
	    return;
    
	if (popupSel->ControlData.ControlFlags.BitFieldVarstore)
    {
        _PopupSelGetSelectionBitFieldVarstore( popupSel );
        return;
    }
	
	temp = EfiLibAllocateZeroPool( 8 * 2 );
	if ( temp == NULL )
		return;

	if ( VarGetValue( popupSel->ControlData.ControlVariable, UefiGetQuestionOffset(ifrData), UefiGetWidth(ifrData), temp ) == EFI_SUCCESS )
	{
		while( i< popupSel->ItemCount)
		{
			if(popupSel->PtrTokens[i].Value == *(UINT64 *)temp)
			{
				popupSel->Sel = (UINT16)i;
		        break;
			}
			i++;
		}
        if(i < popupSel->ItemCount) {
            //Getting valid focusible item in popupSel->Sel. If there is any grayout condition in option list and it is satisfying to true to the current selected value.
            GetValidCurrentOption(popupSel, ControlActionSelect);
            if (popupSel->Sel != i){
                //Doing VarSetValue with the updated value corresponding to the current selection.
                Status = VarSetValue (popupSel->ControlData.ControlVariable, 
                    UefiGetQuestionOffset (popupSel->ControlData.ControlPtr),
                    (UefiGetWidth (popupSel->ControlData.ControlPtr) > sizeof (UINT16)) ? sizeof (UINT16) : UefiGetWidth (popupSel->ControlData.ControlPtr),
                    &(popupSel->PtrTokens[popupSel->Sel].Value) );
                
                HandleWriteOpOnChanged(&(popupSel->ControlData));		//Write opcode for a current control when the control value is edited
                HandleReadOpOnChanged(gApp->CurrentPage);				//Evaluating Read opcode for all the controls in current page to update to the latest edited value
                
            }
        }
		if( i == popupSel->ItemCount)
		{
			//Boot order and driver order are special controls, so TSE is forming the one of with the current boot/driver options.
			//So no need of checking error case.
			if ( (VARIABLE_ID_BOOT_ORDER == popupSel->ControlData.ControlVariable) || (VARIABLE_ID_DRIVER_ORDER == popupSel->ControlData.ControlVariable) )
			{												
				popupSel->Sel = 0;
			}
			else
			{
			// The data does not correspond to the available selections
			if(popupSel->ControlData.ControlDataWidth != 0)					//One-of forced to new value but new value not passed to RouteConfig
			{
				//Try to set selection to the default option
				 VarGetDefaults( popupSel->ControlData.ControlVariable, 
										   UefiGetQuestionOffset(popupSel->ControlData.ControlPtr),
										   popupSel->ControlData.ControlDataWidth, temp );

				 i=0;
     			while( i< popupSel->ItemCount)
				{
                   if(popupSel->PtrTokens[i].Value == *(UINT16 *)temp)
                   {
                       popupSel->Sel = (UINT16)i;
                       GetValidCurrentOption(popupSel, ControlActionSelect);
                       if (popupSel->Sel == i){     //If popup selection is not changed then going with existing VarSetValue
                           Status = VarSetValue( popupSel->ControlData.ControlVariable, 
                                        UefiGetQuestionOffset(popupSel->ControlData.ControlPtr),
                                        (UefiGetWidth(popupSel->ControlData.ControlPtr) > sizeof(UINT16) )?sizeof(UINT16):UefiGetWidth(popupSel->ControlData.ControlPtr),
                                        temp );
                       }
                       else //If value changed, updating value corresponding to the current selection
                       {
                           Status = VarSetValue( popupSel->ControlData.ControlVariable, 
                                       UefiGetQuestionOffset(popupSel->ControlData.ControlPtr),
                                       (UefiGetWidth(popupSel->ControlData.ControlPtr) > sizeof(UINT16) )?sizeof(UINT16):UefiGetWidth(popupSel->ControlData.ControlPtr),
                                       &(popupSel->PtrTokens[popupSel->Sel].Value) );
                       }
                       
                       HandleWriteOpOnChanged(&(popupSel->ControlData));		//Write opcode for a current control when the control value is edited
                       HandleReadOpOnChanged(gApp->CurrentPage);				//Evaluating Read opcode for all the controls in current page to update to the latest edited value
                   
                       break;
                   }
                   i++;
				}
			}
			if( (i == popupSel->ItemCount) || (EFI_ERROR(Status)) )
			{
				//Try to set selection to the option with smallest value
				UINT16 TempoptIndex = 0; //holds index of option with smalleset value
				i = 0;
				while (i< popupSel->ItemCount)			//Trying for least option
				{
					if (popupSel->PtrTokens [i].Value < popupSel->PtrTokens [TempoptIndex].Value)
					{
						TempoptIndex = (UINT16)i;
					}
					i++;
				}
				popupSel->Sel = (UINT16)TempoptIndex; // if nothing else works, revert to the option with smallest value
				GetValidCurrentOption(popupSel, ControlActionSelect);   //If popupSel->Sel is pointing to grayout item, then getting valid item and updating value corresponding to the current selection
				Status = VarSetValue (popupSel->ControlData.ControlVariable, 
					UefiGetQuestionOffset (popupSel->ControlData.ControlPtr),
					(UefiGetWidth (popupSel->ControlData.ControlPtr) > sizeof (UINT16)) ? sizeof (UINT16) : UefiGetWidth (popupSel->ControlData.ControlPtr),
					&(popupSel->PtrTokens[popupSel->Sel].Value) );

                HandleWriteOpOnChanged(&(popupSel->ControlData));		//Write opcode for a current control when the control value is edited
                HandleReadOpOnChanged(gApp->CurrentPage);				//Evaluating Read opcode for all the controls in current page to update to the latest edited value
			}
			// either case (found default, reverted to first value)we have changed the variable so notify application
			}
		}
	}
	MemFreePointer( (VOID **)&temp );
}

/**
    Function to rearrange the boot/driver order variables

    @param popupSel UINT8 bIncrease

    @retval void

**/
VOID RearrangeBootDriverOrderVariable (POPUPSEL_DATA *popupSel, UINT8 bIncrease)
{
    UINTN optionNumber,size;
    UINT16 newOption, *optionList = NULL;
    VOID *ifrData = popupSel->ControlData.ControlPtr;
    FRAME_DATA *MainFrame = NULL ;
	UINTN OptionCount = 0;

    MainFrame = gApp->PageList[gApp->CurrentPage]->FrameList[StyleFrameIndexOf(MAIN_FRAME)];
    //Find newOption
    if (
		(VARIABLE_ID_BOOT_ORDER == popupSel->ControlData.ControlVariable) ||
		(VARIABLE_ID_DRIVER_ORDER == popupSel->ControlData.ControlVariable)
		 )
    {
		(VARIABLE_ID_BOOT_ORDER == popupSel->ControlData.ControlVariable) ? (OptionCount = gBootOptionCount) : (OptionCount = gDriverOptionCount);
        optionNumber = UefiGetQuestionOffset (ifrData) / sizeof(UINT16);
        size = 0;
        optionList = (UINT16 *)VarGetVariable (popupSel->ControlData.ControlVariable, &size);
        if (NULL == optionList){
            return;
        }

        if(optionList[optionNumber] == DISABLED_BOOT_OPTION)
            newOption = DISABLED_BOOT_OPTION;
        else
        {
            if(bIncrease)
            {
                if(0 == optionNumber)
                {
                    //MAY CHANGE: cant increase priority further
                    newOption = optionList [optionNumber];
                }
                else
                {
                    newOption = optionList [optionNumber - 1];
                    if (IsUpdateBootOrderCursor ())
                        _FrameScroll (MainFrame, TRUE);
                }
            }
            else
            {
                if ( (optionNumber+1) >= OptionCount )
                {
                    //MAY CHANGE: cant decrease priority further
                    newOption = optionList [optionNumber];
                }
                else if ( DISABLED_BOOT_OPTION == optionList[optionNumber+1] )
                {
                    //MAY CHANGE: cant decrease priority further
                    newOption = optionList [optionNumber];
                }
                else
                {
                    newOption = optionList [optionNumber + 1];
                    if (IsUpdateBootOrderCursor ())
                        _FrameScroll ( MainFrame, FALSE);
                }
            }
        }

        //Free Optionlist
        MemFreePointer((VOID **) &optionList);
    }
//#if TSE_CSM_SUPPORT
    else if(popupSel->ControlData.ControlVariable == VARIABLE_ID_BBS_ORDER)
    {
		CsmRearrangeBBSOrderVariable(popupSel, bIncrease, &newOption);
    }
//#endif //TSE_CSM_SUPPORT

    //Call callback to the container to record this change in gVariableList(cached copy)
	if ( popupSel->Cookie != NULL )
	{
		CALLBACK_VARIABLE *callbackData = (CALLBACK_VARIABLE *)popupSel->Cookie;

		callbackData->Variable = popupSel->ControlData.ControlVariable;
		callbackData->Offset = UefiGetQuestionOffset(ifrData);
		callbackData->Length = UefiGetWidth(ifrData);
		callbackData->Data = (VOID *)&newOption;
		
		popupSel->Callback( popupSel->Container, popupSel, popupSel->Cookie );
	}
}

/**
    Function to reorder the BBS order variable

    @param popupSel UINT8 bIncrease,UINT16 *newOption

    @retval void

**/
VOID RearrangeBBSOrderVariable(POPUPSEL_DATA *popupSel, UINT8 bIncrease,UINT16 *newOption)
{
    UINTN optionNumber,size;
    UINT16 *optionList = NULL;
    VOID *ifrData = popupSel->ControlData.ControlPtr;
    UINT8 *pDevOrder;
    UINT16 count = 0;
    UINTN offset = UefiGetQuestionOffset(ifrData);
    FRAME_DATA *MainFrame = NULL ;
    MainFrame = gApp->PageList[gApp->CurrentPage]->FrameList[StyleFrameIndexOf(MAIN_FRAME)];

    size = 0;
    pDevOrder = VarGetVariable( VARIABLE_ID_BBS_ORDER, &size );

    offset -= (sizeof(UINT32) + sizeof(UINT16));

    count = gCurrLegacyBootData->LegacyDevCount;
    offset -= gCurrLegacyBootData->LegacyEntryOffset;
    optionList = (UINT16 *)(pDevOrder + gCurrLegacyBootData->LegacyEntryOffset + sizeof(UINT32) + sizeof(UINT16));

    optionNumber = offset / sizeof(UINT16);

    if(optionList[optionNumber] == DISABLED_BOOT_OPTION)
        *newOption = DISABLED_BOOT_OPTION;
    else
    {
        if(bIncrease)
        {
            if(0 == optionNumber)
            {
                //MAY CHANGE: cant increase priority further
                *newOption = optionList[optionNumber];
            }
            else
            {
                *newOption = optionList[optionNumber - 1];
                if(IsUpdateBootOrderCursor())
                    _FrameScroll( MainFrame, TRUE);
            }
            
        }
        else
        {
            if( (optionNumber+1) >= count )
            {
                //MAY CHANGE: cant decrease priority further
                *newOption = optionList[optionNumber];
            }
            else if( DISABLED_BOOT_OPTION == optionList[optionNumber+1] )
            {
                //MAY CHANGE: cant decrease priority further
                *newOption = optionList[optionNumber];
            }
            else
            {
                *newOption = optionList[optionNumber + 1];
                if(IsUpdateBootOrderCursor())
                    _FrameScroll( MainFrame, FALSE);
            }
        }
    }
    //Free Order
    MemFreePointer((VOID **) &pDevOrder);
}

/**
    Fucntion to return the current selection based on the variable. If not matched with variable value
    Selection will be 0	

    @param POPUPSEL_DATA *

    @retval VOID

**/
VOID _GetPopUpSelectionFromVariable (POPUPSEL_DATA *popupSel)
{
	VOID *temp;
	VOID *ifrData = popupSel->ControlData.ControlPtr;
	int i = 0;
    NVRAM_VARIABLE *list = gVariableList;
    EFI_STATUS Status = EFI_SUCCESS;
    
	popupSel->Sel = 0;
	temp = EfiLibAllocateZeroPool (8 * 2);
   if (NULL == temp)
		return;
	
   if (popupSel->ControlData.ControlFlags.BitFieldVarstore)
   {
       if (( list == NULL ) )
       {
           list = gOptimalDefaults;
       }
       Status = _VarGetSetValueForBitField( VAR_COMMAND_GET_VALUE, list, popupSel->ControlData.ControlVariable, 
               UefiGetQuestionOffset(ifrData),  UefiGetWidth(ifrData), 
               temp, ifrData );
   }
   else
   {
       Status = VarGetValue (popupSel->ControlData.ControlVariable, UefiGetQuestionOffset (ifrData), UefiGetWidth (ifrData), temp);
   }   

   if (!EFI_ERROR(Status))
   {
       while (i< popupSel->ItemCount)
       {
           if (popupSel->PtrTokens [i].Value == *(UINT64 *)temp)
           {
               popupSel->Sel = (UINT16)i;
               break;
           }
           i++;
       }
   }
	MemFreePointer((VOID **) &temp);
}

/**
    function to get the height of the label

    @param popupSel Frame, UINT16 *height

    @retval status

**/
EFI_STATUS PopupSelGetControlHeight(POPUPSEL_DATA *popupSel,VOID * frame, UINT16 *height)
{
	if(IsTSEMultilineControlSupported())
	{
		CHAR16 *text=NULL, *text1=NULL, *newtext=NULL ;
		UINT16 Width ;
		UINT16 tmpHeight=0;
		UINTN Len=0;
	
		UINT16 Index = 0;
		UINT16 *OptionList=NULL;
		UINT64 *ValueList=NULL;
		EFI_STATUS Status = EFI_UNSUPPORTED;
		UINT16 TempItemCount = 0;
	
		
	    if(TseLiteIsSpecialOptionList((CONTROL_DATA *)popupSel) != TRUE)
		{
			TempItemCount = popupSel->ItemCount;
			Status = UefiGetOneOfOptions(&popupSel->ControlData,&popupSel->PopupSelHandle,
					&OptionList, &ValueList, &popupSel->ItemCount,NULL,NULL);
			if (EFI_ERROR(Status))
				return Status;
			if (TempItemCount != popupSel->ItemCount)
			{
				if( popupSel->PtrTokens != NULL )
			      MemFreePointer( (VOID **)&(popupSel->PtrTokens));
				popupSel->PtrTokens = EfiLibAllocatePool( popupSel->ItemCount * sizeof(PTRTOKENS) );
				if ( popupSel->PtrTokens == NULL )
				{
					Status = EFI_OUT_OF_RESOURCES;
					return Status;
				}
			}
			for( Index = 0; Index < popupSel->ItemCount; Index++ )
			{
				popupSel->PtrTokens[Index].Option = OptionList[Index];
				popupSel->PtrTokens[Index].Value = ValueList[Index];
			}
			MemFreePointer( (VOID **)&(OptionList));
			MemFreePointer( (VOID **)&(ValueList));
		}

		Width = (UINT16)(popupSel->LabelMargin - (((FRAME_DATA*)frame)->FrameData.Left + (UINT8)gLabelLeftMargin));
	
		*height = 1 ;

//        _PopupSelGetSelection( popupSel );					// One-of forced to new value but new value not passed to RouteConfig
		_GetPopUpSelectionFromVariable (popupSel);			//In _PopupSelGetSelection fnc, boot manager variable behaving wrongly so introduced _GetPopUpSelectionFromVariable fnc 
																			//to read simply value from variable and set the option and not setting any value to variable
		if (popupSel->ItemCount)
		    text1 = HiiGetString( popupSel->PopupSelHandle, popupSel->PtrTokens[popupSel->Sel].Option);
		else
		    text1 = NULL;
		
 	    if (text1 != NULL)
	    {
	        //Len = TestPrintLength( text1 ) / (NG_SIZE);
			Len = (3 + (TestPrintLength( text1 ) / (NG_SIZE))) * sizeof(CHAR16);
			
		    text = EfiLibAllocateZeroPool(Len); 
			newtext = EfiLibAllocateZeroPool(Len);

			if ( text == NULL || newtext == NULL)
			{
				MemFreePointer( (VOID **)&text1 );
				if(text)
					MemFreePointer( (VOID **)&text );
				if(newtext)
					MemFreePointer( (VOID **)&newtext );
		    	return EFI_OUT_OF_RESOURCES;
			}
			MemCpy(newtext, text1, Len);	
			
			SPrint( text,(UINTN)( Len ), L"[%s]", newtext );
		    
			MemFreePointer( (VOID **)&text1 );
			MemFreePointer( (VOID **)&newtext );		
	
		    text1 = StringWrapText( (CHAR16*)text, (UINT16)(popupSel->Width - popupSel->LabelMargin), &tmpHeight );
		    MemFreePointer( (VOID **)&text1 );	
	
		    *height = tmpHeight;
	
		    MemFreePointer( (VOID **)&text );
	    }
	    
	    text = HiiGetString( popupSel->ControlData.ControlHandle, popupSel->Title ) ;
	
	    if(text == NULL)
	    {
	        return EFI_OUT_OF_RESOURCES;
	    }
	    
		text1 = StringWrapText(text, Width, &tmpHeight );
		
		if ((tmpHeight >= 1) && ( OneofMultilineDisable(popupSel)))
		{
			*height = tmpHeight;
		}
		else if(tmpHeight > *height)
			*height = tmpHeight;
	
		(*height) = (*height) ? (*height):1;
		    
		MemFreePointer( (VOID **)&text );
	    MemFreePointer( (VOID **)&text1 );
	}
	
	else
	{
		*height = 1;
	}
	return EFI_SUCCESS;
}
BOOLEAN TseSuppressOneofMultilineSupport(VOID);
BOOLEAN OneofMultilineDisable(POPUPSEL_DATA *popupSel)
{
	if((UefiIsOneOfControl(popupSel->ControlData.ControlPtr))&&(TseSuppressOneofMultilineSupport()))
	{
		return TRUE;
	}	
	
	return FALSE;
}

VOID GetValidCurrentOption(POPUPSEL_DATA *popupSel, UINT16 Action)
{
    UINT16 Index = 0;
    POSTMENU_TEMPLATE *PtrItems = NULL;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    UINT16 ItemCount = popupSel->ItemCount;
                    
    PtrItems = EfiLibAllocateZeroPool( ItemCount * sizeof(POSTMENU_TEMPLATE) ); 
    if ( PtrItems == NULL )
        Status = EFI_OUT_OF_RESOURCES;
    else
        Status = UefiGetValidOptionsCondition(&popupSel->ControlData, PtrItems, ItemCount); //Getting valid PtrItems 
    
    if ( EFI_ERROR(Status)) 
    {
        MemFreePointer( (VOID **)&PtrItems );
        return;
    }
    
    if (PtrItems[popupSel->Sel].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS)
    {
        return;
    }
    
    switch(Action)
    {
        case ControlActionDecreament:
           if (PtrItems[popupSel->Sel].Attribute == AMI_POSTMENU_ATTRIB_NON_FOCUS)
           {
               popupSel->Sel++; //Incrementing Sel, since it was already decremented in ControlActionDecreament case in _PopupSelHandleActionKeyMouse
               //Iterating from selection till 0th item to find the focusible entry. 
               for (Index = popupSel->Sel - 1; Index < ItemCount; Index-- ) 
               {
                    if (PtrItems[Index].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS) //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                    {
                       popupSel->Sel = Index;
                       break;
                    }
               }
               if ((INT16)Index < 0)        //If 0th index reached, start from end of the list item till the current selected item
               {
                   UINT16 Index1 = 0;
                   for (Index1 = ItemCount - 1;  Index1 > popupSel->Sel; Index1-- ) 
                   {
                       if (PtrItems[Index1].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS) //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                       {
                           popupSel->Sel = Index1;
                           break;
                       }
                   }
                   break;
               }
           }
           break;
        case ControlActionIncreament:
            if (PtrItems[popupSel->Sel].Attribute == AMI_POSTMENU_ATTRIB_NON_FOCUS)
            {
                popupSel->Sel--; //Decrementing Sel, since it was already incremented in ControlActionIncreament case in _PopupSelHandleActionKeyMouse
                //Iterating from selection till end of the list to find the focusible entry. 
               for (Index = popupSel->Sel + 1; Index < ItemCount; Index++ ) 
               {
                   if (PtrItems[Index].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS)  //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                   {
                       popupSel->Sel = Index;
                       break;
                   }
               }
               if (Index >= popupSel->ItemCount)    //If end of the list index reached, start from the beginning till the current selected item
               {
                   UINT16 Index1 = 0;
                   for (Index1 = 0; Index1 < popupSel->Sel; Index1++ ) 
                   {
                       if (PtrItems[Index1].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS) //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                       {
                           popupSel->Sel = Index1;
                           break;
                       }
                   }
               }
            }
            break;
        case ControlActionSelect:   //This case is used to fix the valid focusible item when the current selection is grayed out based on other control value.
            if (PtrItems[popupSel->Sel].Attribute == AMI_POSTMENU_ATTRIB_NON_FOCUS)
            {
                for (Index = popupSel->Sel + 1; Index < ItemCount; Index++ ) 
                {
                    if (PtrItems[Index].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS) //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                    {
                        popupSel->Sel = Index;
                        break;
                    }
                }
                if (Index >= popupSel->ItemCount)   //If end of the list index reached, start from the beginning till the current selected item
                {
                    UINT16 Index1 = 0;
                    for (Index1 = 0;  Index1 < popupSel->Sel; Index1++ ) 
                    {
                        if (PtrItems[Index1].Attribute != AMI_POSTMENU_ATTRIB_NON_FOCUS)    //If Attribute is not set to AMI_POSTMENU_ATTRIB_NON_FOCUS make this as selection
                        {
                            popupSel->Sel = Index1;
                            break;
                        }
                    }
                }
            }
            break;
       default:
           break;    
    }
    MemFreePointer( (VOID **)&PtrItems );
}
