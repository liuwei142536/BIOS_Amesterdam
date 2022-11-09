//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Date.c
    This file contains code to handle Date control operations

**/

#include "minisetup.h"


DATE_METHODS gDate =
{
	(OBJECT_METHOD_CREATE)DateCreate,
	(OBJECT_METHOD_DESTROY)DateDestroy,
	(OBJECT_METHOD_INITIALIZE)DateInitialize,
	(OBJECT_METHOD_DRAW)DateDraw,
	(OBJECT_METHOD_HANDLE_ACTION)DateHandleAction,
	(OBJECT_METHOD_SET_CALLBACK)DateSetCallback,
	(CONTROL_METHOD_SET_FOCUS)DateSetFocus,
	(CONTROL_METHOD_SET_POSITION)DateSetPosition,
	(CONTROL_METHOD_SET_DIMENSIONS)DateSetDimensions,
	(CONTROL_METHOD_SET_ATTRIBUTES)DateSetAttributes,
	(CONTROL_METHOD_GET_CONTROL_HIGHT)DateGetControlHeight

};
BOOLEAN CheckKeyinHotKeysList(AMI_EFI_KEY_DATA Key);
VOID _DateUpdateEntry(DATE_DATA *time);
EFI_STATUS _DateHandleActionKeyMouse(DATE_DATA *date,  ACTION_DATA *Data);
VOID SetSoftKbdPosition(UINT32 Height, UINT32 Top);
UINT16 DayNames[] =
{
	STRING_TOKEN(STR_DAY_SUNDAY),
	STRING_TOKEN(STR_DAY_MONDAY),
	STRING_TOKEN(STR_DAY_TUESDAY),
	STRING_TOKEN(STR_DAY_WEDNESDAY),
	STRING_TOKEN(STR_DAY_THURSDAY),
	STRING_TOKEN(STR_DAY_FRIDAY),
	STRING_TOKEN(STR_DAY_SATURDAY),
};

CHAR16 DateTempNum[5];
extern void GetDateData(UINT8 *DefaultDay, UINT8 *DefaultMonth, UINT16 *DefaultYear,  UINT16 *MinYear, UINT16 *MaxYear);
/**
    Function to create the date, which uses the Control fucntions.

    @param object 

    @retval status

**/
EFI_STATUS DateCreate( DATE_DATA **object )
{
	EFI_STATUS Status = EFI_OUT_OF_RESOURCES;

	if ( *object == NULL )
	{
		*object = EfiLibAllocateZeroPool( sizeof(DATE_DATA) );

		if ( *object == NULL )
			return Status;
	}

	Status = gControl.Create((void**) object );
	if ( ! EFI_ERROR(Status) )
		(*object)->Methods = &gDate;

	return Status;
}

/**
    Function to destroy the date, which uses the Control fucntions.

    @param date BOOLEAN freeMem

    @retval status

**/
EFI_STATUS DateDestroy( DATE_DATA *date, BOOLEAN freeMem )
{
	if(NULL == date)
	  return EFI_SUCCESS;

	gControl.Destroy( date, FALSE );

	if ( freeMem )
		MemFreePointer( (VOID **)&date );

	return EFI_SUCCESS;
}

/**
    Function to initialize the date, which uses the Control fucntions.

    @param date VOID *data

    @retval status

**/
EFI_STATUS DateInitialize( DATE_DATA *date, VOID *data )
{
	EFI_STATUS Status = EFI_UNSUPPORTED;

	Status = gControl.Initialize( date, data );
	if (EFI_ERROR(Status))
		return Status;

	// add extra initialization here...
	date->ControlData.ControlHelp = UefiGetHelpField(date->ControlData.ControlPtr);
	date->Interval = (UINT8)date->ControlData.ControlFlags.ControlRefresh;
	date->LabelMargin = (UINT8)gControlLeftMargin;
	date->Sel = 0;
	date->ControlFocus = FALSE;
	date->ControlActive = FALSE;
	GetDateData(&(date->DefaultDay), &(date->DefaultMonth), &(date->DefaultYear),&(date->MinYear), &(date->MaxYear) );
	SetControlColorsHook(NULL, NULL,
						NULL, &(date->FGColor), 
						&(date->SelBGColor), &(date->SelFGColor),
						&(date->BGColor), &(date->NSelFGColor),
						NULL, &(date->LabelFGColor),
						&(date->NSelLabelFGColor),
						NULL, NULL,
						NULL, NULL );

	return Status;
}

/**
    Function to draw the date.

    @param date 

    @retval status

**/
EFI_STATUS DateDraw( DATE_DATA *date )
{
	CHAR16 *t,text[50],txt[5];
	UINTN day;
	EFI_TIME Tm;
	UINTN nIndex,nDateLen,nEFIDateLen,i = 0;
	UINTN j, DayMaxLen =0;
	EFI_STATUS Status = EFI_SUCCESS;
	UINT8 ColorDate = date->NSelFGColor /*date->FGColor*/ , NScolor,SColor;
	UINT16 MarginLeft = date->Left + date->LabelMargin;
	UINT8 ColorLabelDate = (date->ControlFocus) ? date->LabelFGColor:date->NSelLabelFGColor ;
	BOOLEAN bShowDay = StyleShowDay() ;
	UINTN dateFormat = StyleGetDateFormat() ;	
	UefiGetTime(&date->ControlData, &Tm);
	
	if (Tm.Year > date->MaxYear || Tm.Year < date->MinYear || Tm.Year == 0){
	    EfiLibReportStatusCode(EFI_ERROR_CODE|(EFI_ERROR_MINOR), GENERIC_BAD_DATE_TIME_ERROR, 0, NULL, NULL);
	    Tm.Year  = date->DefaultYear;
	    Tm.Month = date->DefaultMonth;
	    Tm.Day   = date->DefaultDay;
	    if(Tm.Hour > 23 || Tm.Minute > 59 || Tm.Second > 59)
	    {
	        Tm.Hour   =0;
	        Tm.Minute =0;
	        Tm.Second =0;
	    }
	    UefiSetTime(&date->ControlData, &Tm);
	}
	
	// check conditional ptr if necessary
    // Support grayout condition for readonly controls
	//if ( date->ControlData.ControlConditionalPtr != 0x0 )
	//{
		switch( CheckControlCondition( &date->ControlData ) )
		{
			case COND_NONE:
				break;
			case COND_GRAYOUT:
				Status = EFI_WARN_WRITE_FAILURE;
				ColorDate = ColorLabelDate = CONTROL_GRAYOUT_COLOR;
				break;
			default:
				return EFI_UNSUPPORTED;
				break;
		}
	//}

	NScolor = date->BGColor | ColorDate;
	SColor = date->SelBGColor | date->SelFGColor;

	// label
	t = HiiGetString( date->ControlData.ControlHandle, UefiGetPromptField(date->ControlData.ControlPtr));
	if (NULL == t){
		t = EfiLibAllocateZeroPool(2*sizeof(CHAR16));
		if(!t)
			return EFI_NOT_FOUND;
		StrCpyS(t, 2 , L" ");
	}
	// boundary overflow  check
	//TSE_MULTILINE_CONTROLS moved to binary
	if(date->Height>1 && IsTSEMultilineControlSupported())
	{
		DrawMultiLineStringWithAttribute( date->Left , date->Top, 
				(UINTN)(date->LabelMargin - date->Left ),(UINTN) date->Height,
				t, date->BGColor | ColorLabelDate );
	}
	else
	{
		if ( (TestPrintLength(t) / (NG_SIZE)) > (UINTN)(date->LabelMargin - date->Left  )/*21*/ )
				t[date->LabelMargin - date->Left ] = L'\0';
	
		DrawStringWithAttribute( date->Left , date->Top, (CHAR16*)t, 
				date->BGColor | ColorLabelDate );
	}
	MemFreePointer ((VOID **)&t);

	day = _DateGetWeekDay( &Tm );

	//To find Day Maximum Length
    for (j = 0;j < (sizeof(DayNames)/sizeof(UINT16)); j++)
    {
        t = HiiGetString(gHiiHandle, DayNames[j]);
        if (t && (EfiStrLen(t) > DayMaxLen))
            DayMaxLen = EfiStrLen(t);
        MemFreePointer ((VOID **)&t);
    }
	
	
	t = HiiGetString( gHiiHandle, (bShowDay)? DayNames[day]:STRING_TOKEN(STR_EMPTY_STRING));
	if (NULL == t){
		t = EfiLibAllocateZeroPool(2*sizeof(CHAR16));
		if(!t)
			return EFI_NOT_FOUND;
		StrCpyS(t, 2, L" ");
	}
	
	nIndex = TestPrintLength(t) / (NG_SIZE);
	nDateLen = TestPrintLength(t) / (NG_SIZE);
	nEFIDateLen = EfiStrLen(t);
	
	if ( t != NULL )
	{
	    UINTN DestMax;
		SPrint( text, sizeof(text), L"[%s ", t );
		text[nEFIDateLen + 1] = L' ';
		i = (bShowDay)? (nEFIDateLen + 2):(nEFIDateLen + 1);
		DestMax = 50  - i;
		if ((dateFormat == DATE_STYLE_MMDDYYYY) || (dateFormat == DATE_STYLE_DDMMYYYY))
		{	    
		    StrCpyS(text+(i), DestMax, L"XX/XX/XXXX]");
		}
        else
        {	
		StrCpyS(text+(i), DestMax, L"XXXX/XX/XX]");
        }
		//If nEFIDateLen is less than DayMaxLen, then we are filling remaining length with space to avoid screen corruption.
	    if (nEFIDateLen < DayMaxLen)
	    {
	        for (j = 0; j < (DayMaxLen - nEFIDateLen); j++)
	            StrCatS(text, 50, L" ");

	    }
	}
	MemFreePointer( (VOID **)&t );
	i = (bShowDay)? (nDateLen + 2):(nDateLen + 1) ; //(StyleShowDay()==TRUE)?2:1;
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ , date->Top, (CHAR16*)text, 
			date->BGColor | ColorDate );

	nIndex = i;

	if ((dateFormat == DATE_STYLE_MMDDYYYY)||(dateFormat == DATE_STYLE_DDMMYYYY))
	{	
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
	( (date->Sel==0) && (date->ControlActive == TRUE ) ) ? DateTempNum :StrZeroPad((dateFormat == DATE_STYLE_MMDDYYYY)?Tm.Month:Tm.Day,txt), 
			(UINT8)((((date->Sel == 0) && (date->ControlFocus)) ? 
					 SColor/*date->SelBGColor | date->SelFGColor*/ : 
					 NScolor/*date->BGColor | ColorDate*/ )) );

	nIndex+=3;
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
	( (date->Sel==1) && (date->ControlActive == TRUE ) ) ? DateTempNum :StrZeroPad((dateFormat == DATE_STYLE_MMDDYYYY)?Tm.Day:Tm.Month,txt), 
			(UINT8)( (((date->Sel == 1) && (date->ControlFocus)) ? 
					  SColor/*date->SelBGColor | date->SelFGColor*/ :
					  NScolor/*date->BGColor | ColorDate*/ )) );

	nIndex+=3;
	if((Tm.Year < 1000) &&( atoi_base(DateTempNum, 10) < 1000))
		SPrint(txt,sizeof(txt),L"%04d",Tm.Year); //Update the date string when year is ZERO.

	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
		( (date->Sel==2) && (date->ControlActive == TRUE ) ) ? DateTempNum :((Tm.Year < 1000) ?  txt : StrZeroPad( Tm.Year,txt)), 
			(UINT8)( (((date->Sel == 2) && (date->ControlFocus)) ? 
					  SColor/*date->SelBGColor | date->SelFGColor*/ :
					  NScolor/*date->BGColor | ColorDate*/ )) );
	}
	else 
	{

	if((Tm.Year < 1000) &&( atoi_base(DateTempNum, 10) < 1000))
		SPrint(txt,sizeof(txt),L"%04d",Tm.Year); //Update the date string when year is ZERO.
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
	( (date->Sel==0) && (date->ControlActive == TRUE ) ) ? DateTempNum :((Tm.Year < 1000) ?  txt : StrZeroPad( Tm.Year,txt)), 
			(UINT8)( (((date->Sel == 0) && (date->ControlFocus)) ? 
					  SColor/*date->SelBGColor | date->SelFGColor*/ :
					  NScolor/*date->BGColor | ColorDate*/ )) );

	nIndex+=5;
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
	( (date->Sel==1) && (date->ControlActive == TRUE ) ) ? DateTempNum :StrZeroPad((dateFormat == DATE_STYLE_YYYYDDMM)?Tm.Day:Tm.Month,txt), 
			(UINT8)( (((date->Sel == 1) && (date->ControlFocus)) ? 
					  SColor/*date->SelBGColor | date->SelFGColor*/ :
					  NScolor/*date->BGColor | ColorDate*/ )) );

	nIndex+=3;
	DrawStringWithAttribute( MarginLeft/*date->Left + date->LabelMargin*/ + nIndex , date->Top, 
	( (date->Sel==2) && (date->ControlActive == TRUE ) ) ? DateTempNum :StrZeroPad((dateFormat == DATE_STYLE_YYYYDDMM)?Tm.Month:Tm.Day,txt), 
			(UINT8)((((date->Sel == 2) && (date->ControlFocus)) ? 
					 SColor/*date->SelBGColor | date->SelFGColor*/ : 
					 NScolor/*date->BGColor | ColorDate*/ )) );
	}

	FlushLines(date->Top, date->Top);
	return Status;

}

/**
    Function to handle the date actions.

    @param date ACTION_DATA *Data

    @retval status

**/
EFI_STATUS DateHandleAction( DATE_DATA *date, ACTION_DATA *Data)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;

	if ( Data->Input.Type == ACTION_TYPE_TIMER )
	{
		if(date->ControlActive)
			return Status;

        if (0 == date->ControlData.ControlFlags.ControlRefresh)         //If control not has refresh no need of proceeding
            return Status;

		if(IsActiveControlPresent(gApp->PageList[gApp->CurrentPage]))
			return Status;

		if ( --(date->Interval) == 0 )
		{
			// initialize the interval
			date->Interval = (UINT8)(date->ControlData.ControlFlags.ControlRefresh);
			return UefiRefershQuestionValueNvRAM(&(date->ControlData));
		}
		else
			return Status;
	}

	if ( ! date->ControlFocus )
		return Status;

    if (( Data->Input.Type == ACTION_TYPE_KEY )||(Data->Input.Type == ACTION_TYPE_MOUSE))
    {
        return _DateHandleActionKeyMouse(date, Data);
    }

	return Status;
}

EFI_STATUS _DateHandleActionKeyMouse(DATE_DATA *date,  ACTION_DATA *Data)
{
    CONTROL_ACTION Action = ControlActionUnknown;
	EFI_STATUS Status = EFI_UNSUPPORTED;
	EFI_TIME Tm;
	AMI_EFI_KEY_DATA key = Data->Input.Data.AmiKey;
	UINTN dateFormat = StyleGetDateFormat() ;

    if(date->ControlData.ControlFlags.ControlReadOnly)
		return EFI_UNSUPPORTED;

    //Get time
	//gRT->GetTime( &Tm, NULL );
	UefiGetTime(&date->ControlData, &Tm);

    //Get mapping
	if(Data->Input.Type == ACTION_TYPE_KEY)
	{
      Action = MapControlKeysHook(key);
	}
	else if(Data->Input.Type == ACTION_TYPE_MOUSE)
	{
		if(ControlActionSelect == MapControlMouseActionHook(&Data->Input.Data.MouseInfo))
		{
		
			//check whether MouseTop is within the Height and Width of Date Control or not
			if((Data->Input.Data.MouseInfo.Top >= (UINT32)date->Top) && (Data->Input.Data.MouseInfo.Top < (UINT32)(date->Top+date->Height)) &&
		          (Data->Input.Data.MouseInfo.Left >= (UINT32)date->Left) && (Data->Input.Data.MouseInfo.Left < (UINT32)(date->Left+date->Width)))
			{
				Action = ControlActionSelect;		
			}
		}
	}

    switch(Action)
    {
        case ControlActionNextLeft:
#if SETUP_STYLE_AWARD
		if ( date->ControlActive )
			_DateUpdateEntry(date);

		if ( date->Sel == 0 )
			date->Sel = 2;
		else
			date->Sel--;

		Status = EFI_SUCCESS;		
		break;
#endif

        case ControlActionNextRight:
		if ( date->ControlActive )
			_DateUpdateEntry(date);

#if SETUP_STYLE_AWARD
		if ( date->Sel == 2 )
			date->Sel = 0;
		else
			date->Sel++;

		Status = EFI_SUCCESS;
#endif
		break;

        case ControlActionNextUp:
        case ControlActionNextDown:
            if ( date->ControlActive )
                _DateUpdateEntry(date);
	    break;

        case ControlActionAbort:
		if ( date->ControlActive )
		{
                	date->ControlActive = FALSE;
			Status = EFI_SUCCESS;
		}
		break;

        case ControlActionSelect:
		//Launching SoftKbd only once.
		//If Time control is double clicked and no softKbd is launched, 
		//then launch softKbd and dont change the date field.
		if( (ACTION_TYPE_MOUSE == Data->Input.Type) &&  //For mouse action
			 (FALSE == TSEMouseIgnoreMouseActionHook()) //If no softkbd present
		)
		{
			NumericSoftKbdInit();
			SetSoftKbdPosition(date->Height, date->Top);
			TSEStringReadLoopEntryHook(); 
			Status = EFI_SUCCESS;
			break;
		}
		
		_DateUpdateEntry(date);
// for EZPORT: expressely to go to next entry in date control after hitting enter
#if SETUP_STYLE_AWARD
        break;
#endif

        case ControlActionNextSelection:
		if ( date->ControlActive )
			_DateUpdateEntry(date);

		if ( date->Sel == 2 )
			date->Sel = 0;
		else
			date->Sel++;

		Status = EFI_SUCCESS;
		break;

	case ControlActionPrevSelection:
		if ( date->ControlActive )
			_DateUpdateEntry(date);

		if ( date->Sel == 0 )
		{
			date->Sel = 2;
		}
		else
		{
			date->Sel--;
		}

			Status = EFI_SUCCESS;
		break;

        case ControlActionDecreament:
			if(date->ControlActive)
    		{
        		_DateUpdateEntry(date);
    		}
			_DateDecrease( date );
			date->ControlActive = TRUE;
			Status = EFI_SUCCESS;
   			break;

        case ControlActionIncreament:
			if(date->ControlActive)
    		{
     			_DateUpdateEntry(date);
    		}
			_DateIncrease( date );
			date->ControlActive = TRUE;
			Status = EFI_SUCCESS;
			break;

        case ControlActionNumeric:
            	if( !(date->ControlActive) )
		{
                	MemSet(DateTempNum, 10, 0x0);
			date->ControlActive = TRUE;
			switch(date->Sel)
			{
				case 0:
					if((dateFormat == DATE_STYLE_YYYYDDMM) || (dateFormat == DATE_STYLE_YYYYMMDD))
						SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year );	
					else if(dateFormat == DATE_STYLE_DDMMYYYY)
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Day);	
					else
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Month);			
					break;
				case 1:
					if((dateFormat == DATE_STYLE_DDMMYYYY) || (dateFormat == DATE_STYLE_YYYYMMDD))
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Month);			
					else
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Day);			
					break;
				case 2:
					if(dateFormat == DATE_STYLE_YYYYDDMM)
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Month);	
					else if(dateFormat == DATE_STYLE_YYYYMMDD)
						SPrint(DateTempNum,sizeof(DateTempNum),L"%d",Tm.Day);	
					else
						SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year );			
					break;
			}
		}

		if(EfiStrLen(DateTempNum)>1)
			DateTempNum[0] =DateTempNum[1];

		if (
			( (2 == date->Sel) && (DATE_STYLE_MMDDYYYY == dateFormat || DATE_STYLE_DDMMYYYY == dateFormat) ) ||
			( (0 == date->Sel) && (DATE_STYLE_YYYYDDMM == dateFormat || DATE_STYLE_YYYYMMDD == dateFormat) )
			 )
		{
			DateTempNum[1] =DateTempNum[2];
			DateTempNum[2] =DateTempNum[3];
			DateTempNum[3] = key.Key.UnicodeChar;
		}else
			DateTempNum[1] = key.Key.UnicodeChar;

		Status = gDate.Draw(date);
        	break;

        default:
            if ( date->ControlActive )
            {
                _DateUpdateEntry(date);
            }
        	break;
	}
	
	if( (((!date->ControlActive)||(!IsMouseClickedonSoftkbd())) && (TSEMouseIgnoreMouseActionHook() == TRUE) )&&
		(((Data->Input.Type == ACTION_TYPE_KEY) &&
		((Action == ControlActionNextLeft) ||(Action == ControlActionNextRight) ||(Action == ControlActionNextUp) ||(Action == ControlActionNextDown) ||(Action == ControlActionPageUp) ||(Action == ControlActionPageDown) ||(Action == ControlActionAbort) ||(Action == ControlActionHome) ||(Action == ControlActionEnd) ||(CheckKeyinHotKeysList(key))) ) ||
		((Data->Input.Type == ACTION_TYPE_MOUSE)&&((!((Data->Input.Data.MouseInfo.Top >= (UINT32)date->Top) && (Data->Input.Data.MouseInfo.Top < (UINT32)(date->Top+date->Height))&&(Data->Input.Data.MouseInfo.Left >= (UINT32)date->Left) && (Data->Input.Data.MouseInfo.Left < (UINT32)(date->Left+date->Width))))||(Action == ControlActionAbort))))
	)
	{
		NumericSoftKbdExit();//Check for softkbd presence and existing the numeric softkbd 
	}
	_DateUpdateHelp( date );
	return Status;
}
/**
    Function to set callback.

    @param date OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie

    @retval STATUS

**/
EFI_STATUS DateSetCallback( DATE_DATA *date, OBJECT_DATA *container, OBJECT_CALLBACK callback, VOID *cookie )
{
	return EFI_UNSUPPORTED;
}
/**
    Function to set the Date focus.

    @param date BOOLEAN focus

    @retval status

**/
EFI_STATUS DateSetFocus( DATE_DATA *date, BOOLEAN focus)
{
	if(focus != FALSE)
	{
		if ( CheckControlCondition( &date->ControlData ) )
			return EFI_UNSUPPORTED;
	}

	if( !(date->ControlFocus && focus) )
	{
		date->ControlFocus = focus;

		// set the selected field to Month
		date->Sel = 0;
		_DateUpdateHelp( date );
	}

	return EFI_SUCCESS;
}
/**
    Function to set the date position.

    @param date UINT16 Left, UINT16 Top

    @retval status

**/
EFI_STATUS DateSetPosition( DATE_DATA *date, UINT16 Left, UINT16 Top )
{
	return gControl.SetPosition( (CONTROL_DATA*)date, Left, Top );
}
/**
    Function to set the date dimensions.

    @param date UINT16 Width, UINT16 Height

    @retval status

**/
EFI_STATUS DateSetDimensions( DATE_DATA *date, UINT16 Width, UINT16 Height )
{
	return gControl.SetDimensions( (CONTROL_DATA*)date, Width, Height );
}
/**
    Function to set the date attributes.

    @param date UINT8 FGColor, UINT8 BGColor

    @retval status

**/
EFI_STATUS DateSetAttributes( DATE_DATA *date, UINT8 FGColor, UINT8 BGColor )
{
	return gControl.SetAttributes((CONTROL_DATA*) date, FGColor, BGColor );
}

/**
    Function to get the data label height.

    @param date frame, UINT16 *height

    @retval status

**/
EFI_STATUS DateGetControlHeight(DATE_DATA *date, VOID * frame, UINT16 *height)
{
	if(IsTSEMultilineControlSupported())
	{
		CHAR16 *newText = NULL,*text=NULL;
		UINT16 Width;
	
		*height = 1;	
		Width = (UINT16)(date->LabelMargin - (((FRAME_DATA*)frame)->FrameData.Left + (UINT8)gControlLeftPad));

		text = HiiGetString( date->ControlData.ControlHandle, UefiGetPromptField(date->ControlData.ControlPtr) );		
		newText = StringWrapText( text, Width, height );
		
	    (*height) = (*height) ? (*height):1;
	
	    MemFreePointer( (VOID **)&newText );
		MemFreePointer( (VOID **)&text );
	}
	else
	{
		*height = 1;
	}
	return EFI_SUCCESS;
}

/**
    Function to get weekday.

    @param tm 

    @retval UINTN

**/
UINTN _DateGetWeekDay( EFI_TIME *tm )
{
	INTN d, m, y, c;
	INTN w;

	d = tm->Day;
	m = tm->Month - 2;
	y = tm->Year;

	if ( m <= 0 )
	{
		m += 12;
		y--;
	}

	c = y / 100;
	y %= 100;

	w = d + y + (5 * c) + ((13 * m - 1) / 5) + (y / 4) + (c / 4);

	return w % 7;

}
/**
    Function to decrement date.

    @param date 

    @retval VOID

**/
VOID _DateDecrease( DATE_DATA *date )
{
	EFI_TIME Tm;
    EFI_TIME TempTm;
	BOOLEAN Case_day = FALSE;
    BOOLEAN Case_Year = FALSE;
	CONTROL_INFO *ControlInfo = NULL;
	UINT8 Flags = 0;
	VOID *IfrPtr = NULL;
	UINTN dateFormat = StyleGetDateFormat() ;

	//gRT->GetTime( &Tm, NULL );
	EfiZeroMem (&Tm, sizeof (EFI_TIME));
	UefiGetTime(&date->ControlData, &Tm);
    TempTm =Tm;//Backup of input Tm
	switch ( date->Sel )
	{
		case 0:
			if((dateFormat == DATE_STYLE_YYYYDDMM) || (dateFormat == DATE_STYLE_YYYYMMDD)){
				Tm.Year--;
				SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
               Case_Year = TRUE;
			}else if(dateFormat == DATE_STYLE_DDMMYYYY)
			{	
				if ( Tm.Day > 1 )
					Tm.Day--;
				else
					Tm.Day = 31; // this is to find out wich is the highest day for the month.
				StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
				Case_day = TRUE;
			}else{
				if ( Tm.Month > 1 )
					Tm.Month--;
				else
					Tm.Month = 12;
				StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
			}
			break;
		case 1:
			if((dateFormat == DATE_STYLE_DDMMYYYY) || (dateFormat == DATE_STYLE_YYYYMMDD))
			{	
				if ( Tm.Month > 1 )
					Tm.Month--;
				else
					Tm.Month = 12;
				StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
			}else{
				if ( Tm.Day > 1 )
					Tm.Day--;
				else
					Tm.Day = 31; // this is to find out wich is the highest day for the month.
				StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
				Case_day = TRUE;
			}
			break;
		case 2:
			if(dateFormat == DATE_STYLE_YYYYDDMM)
			{	
				if ( Tm.Month > 1 )
					Tm.Month--;
				else
					Tm.Month = 12;
				StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
			}else if(dateFormat == DATE_STYLE_YYYYMMDD){
				if ( Tm.Day > 1 )
					Tm.Day--;
				else
					Tm.Day = 31; // this is to find out wich is the highest day for the month.
				StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
				Case_day = TRUE;
			}else{
				Tm.Year--;
					if(Tm.Year < date->MinYear)
						Tm.Year = date->MaxYear;
					SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
                    Case_Year = TRUE;
			}
			break;
	}

	ControlInfo = &date->ControlData;
	if((ControlInfo != NULL) && (ControlInfo->ControlPtr != NULL))
	{
		IfrPtr = ControlInfo->ControlPtr; 
		Flags = UefiGetFlagsField(IfrPtr) ;
		
		if((Flags & EFI_QF_DATE_STORAGE) == QF_DATE_STORAGE_NORMAL)// Incorrect logic used to check Time and Date control types
		{
			gUserTseCacheUpdated = TRUE;		//Setting TRUE to know whether setup question is changed or not
		}
	}
	  	
	while ( UefiSetTime(&date->ControlData, &Tm) != EFI_SUCCESS )
    {
        if(Case_day == TRUE)
        {
            if(Tm.Day > 28){
                Tm.Day--; //May be this month does not support days greater than 28
                StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
                continue;
            } 
        }else if(Case_Year == TRUE) {
            //SetTime failed due to year decremented beyound the range
            Tm.Year++;
            SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
        }
        if(UefiSetTime(&date->ControlData, &Tm) != EFI_SUCCESS)
        {
            //If all UefiSetTime get fails then revert the current day/date/year string 
            // in DateTempNum to avoid screen corruption.
            if(Case_Year)
                SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",TempTm.Year); //Update the static date string
            else if(Case_day)
                SPrint(DateTempNum,sizeof(DateTempNum),L"%02d",TempTm.Day); //Update the static date string 
            else
                SPrint(DateTempNum,sizeof(DateTempNum),L"%02d",TempTm.Month); //Update the static date string
        }    
        break;
    }
}
/**
    Function to increment date.

    @param date 

    @retval VOID

**/
VOID _DateIncrease( DATE_DATA *date )
{
    EFI_TIME Tm;
    EFI_TIME TempTm;
    CONTROL_INFO *ControlInfo = NULL;
    UINT8 Flags = 0;
    VOID *IfrPtr = NULL;
    UINTN dateFormat = StyleGetDateFormat() ;
    BOOLEAN Case_day = FALSE;
    BOOLEAN Case_Year = FALSE;

    //gRT->GetTime( &Tm, NULL );
    EfiZeroMem (&Tm, sizeof (EFI_TIME));
    UefiGetTime(&date->ControlData, &Tm);
    TempTm =Tm;//Backup of input Tm
    switch ( date->Sel )
    {
    case 0:
        if((dateFormat == DATE_STYLE_YYYYDDMM) || (dateFormat == DATE_STYLE_YYYYMMDD)){
            Tm.Year++;
            SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
            Case_Year = TRUE;
        } else if(dateFormat == DATE_STYLE_DDMMYYYY){
            if(Tm.Day < 31){
                Tm.Day++;
            } else{
                Tm.Day = 1;
            }
            StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
            Case_day = TRUE;
        } else{
            if ( Tm.Month < 12 )
                Tm.Month++;
            else 
                Tm.Month = 1;
            StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
        }
        break;
    case 1:
        if((dateFormat == DATE_STYLE_DDMMYYYY) || (dateFormat == DATE_STYLE_YYYYMMDD))
        {	
            if ( Tm.Month < 12 )
                Tm.Month++;
            else 
                Tm.Month = 1;
            StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
        } else{
            if(Tm.Day < 31){
                Tm.Day++;
            } else{
                Tm.Day = 1;
            }
            StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
            Case_day = TRUE;
        }
        break;
    case 2:
        if(dateFormat == DATE_STYLE_YYYYDDMM)
        {
            if ( Tm.Month < 12 )
                Tm.Month++;
            else 
                Tm.Month = 1;
            StrZeroPad(Tm.Month, DateTempNum); //Update the static date string
        }else if(dateFormat == DATE_STYLE_YYYYMMDD){
            if(Tm.Day < 31){
                Tm.Day++;
            } else{
                Tm.Day = 1;
            }
            StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
            Case_day =  TRUE;
        }else{
            Tm.Year++;
            if(Tm.Year > date->MaxYear)
                Tm.Year = date->MinYear;
            
            SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
            Case_Year = TRUE;
        }
        break;
    }

    ControlInfo = &date->ControlData;
    if((ControlInfo != NULL) && (ControlInfo->ControlPtr != NULL))
    {
        IfrPtr = ControlInfo->ControlPtr; 
        Flags = UefiGetFlagsField(IfrPtr) ;

        if((Flags & EFI_QF_DATE_STORAGE) == QF_DATE_STORAGE_NORMAL)// Incorrect logic used to check Time and Date control types
        {
            gUserTseCacheUpdated = TRUE;		//Setting TRUE to know whether setup question is changed or not
        }
    }

    //if ( gRT->SetTime( &Tm ) != EFI_SUCCESS )
    if ( UefiSetTime(&date->ControlData, &Tm) != EFI_SUCCESS )
    {
        //SetTime may be failed because of invalid day 
        if(Case_day == TRUE) 
        {
            Tm.Day = 1;
            StrZeroPad(Tm.Day, DateTempNum); //Update the static date string
        }
        //SetTime may be failed because of invalid year
        if(Case_Year == TRUE) 
        {
            Tm.Year--;
            SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",Tm.Year); //Update the static date string
        }
        if(UefiSetTime(&date->ControlData, &Tm) != EFI_SUCCESS)
        {
            //If all UefiSetTime get fails then revert the current day/date/year string 
            // in DateTempNum to avoid screen corruption.
            if(Case_Year)
                SPrint(DateTempNum,sizeof(DateTempNum),L"%04d",TempTm.Year); //Update the static date string
            else if(Case_day)
                SPrint(DateTempNum,sizeof(DateTempNum),L"%02d",TempTm.Day); //Update the static date string 
            else
                SPrint(DateTempNum,sizeof(DateTempNum),L"%02d",TempTm.Month); //Update the static date string
        }		
    }

}
/**
    Function to update entry.

    @param date 

    @retval VOID

**/
VOID _DateUpdateEntry(DATE_DATA *date)
{
	EFI_TIME Tm = {0};
	CONTROL_INFO *ControlInfo = NULL;
	UINT8 Flags = 0;
	VOID *IfrPtr = NULL;
	UINTN dateFormat = StyleGetDateFormat() ;

	//gRT->GetTime( &Tm, NULL );
	UefiGetTime(&date->ControlData, &Tm);

	if(date->ControlActive)
	{
		// focus stays on the selection becuase we just edited it
		switch(date->Sel)
		{
		case 0:
			if((dateFormat == DATE_STYLE_YYYYDDMM) || (dateFormat == DATE_STYLE_YYYYMMDD))
				Tm.Year = (UINT16)atoi_base(DateTempNum, 10);
			else if(dateFormat == DATE_STYLE_DDMMYYYY)
				Tm.Day = (UINT8)atoi_base(DateTempNum, 10);
			else
                Tm.Month = (UINT8)atoi_base(DateTempNum, 10);
			break;
		case 1:
			if((dateFormat == DATE_STYLE_DDMMYYYY) || (dateFormat == DATE_STYLE_YYYYMMDD))
				Tm.Month = (UINT8)atoi_base(DateTempNum, 10);
			else
    	        Tm.Day = (UINT8)atoi_base(DateTempNum, 10);
			break;
		case 2:
			if(dateFormat == DATE_STYLE_YYYYDDMM)
				Tm.Month = (UINT8)atoi_base(DateTempNum, 10);
			else if(dateFormat == DATE_STYLE_YYYYMMDD)
				Tm.Day = (UINT8)atoi_base(DateTempNum, 10);
			else
    	        Tm.Year = (UINT16)atoi_base(DateTempNum, 10);
			break;
		}
		
		UefiPreControlUpdate(&(date->ControlData));
		
		ControlInfo = &date->ControlData;
		if((ControlInfo != NULL) && (ControlInfo->ControlPtr != NULL))
		{
			IfrPtr = ControlInfo->ControlPtr; 
			Flags = UefiGetFlagsField(IfrPtr) ;
			
			if((Flags & EFI_QF_DATE_STORAGE) == QF_DATE_STORAGE_NORMAL)// Incorrect logic used to check Time and Date control types
			{
				gUserTseCacheUpdated = TRUE;		//Setting TRUE to know whether setup question is changed or not
			}
		}
			
		UefiSetTime(&date->ControlData, &Tm);
		//For interactive date control invoke the formcallback functions
		if(UefiIsInteractive(&date->ControlData)){
			Flags = UefiGetFlagsField(date->ControlData.ControlPtr) ;
			
			if(CheckDateFlags(Flags))
			{
				//for storage type QF_DATE_STORAGE_NORMAL generate time in EFI_HII_DATE format
				
				//Call the callbacks passing the generated input value and value size
				CallFormCallBack(&date->ControlData, UefiGetControlKey(&date->ControlData), 0, AMI_CALLBACK_CONTROL_UPDATE);
			}// else {
				//This case would be handled by UefiSetTime() outside using runtime services
			//}
		}
		if( GetTseInconsistentPolicy()){
		    CheckInconsistence((PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[gApp->CurrentPage]));
		}				
                date->ControlActive = FALSE;
	}
}
/**
    Function to update help.

    @param date 

    @retval VOID

**/
VOID _DateUpdateHelp( DATE_DATA *date )
{
	UINT16	helpToken = date->ControlData.ControlHelp;
	UINT16 Min,Max;
	UINTN dateFormat = StyleGetDateFormat() ;

	switch ( date->Sel )
	{
		case 0:
			if((dateFormat == DATE_STYLE_YYYYDDMM) || (dateFormat == DATE_STYLE_YYYYMMDD))
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_YEAR,&helpToken,&Min,&Max);
			else if(dateFormat == DATE_STYLE_DDMMYYYY)
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_DAY,&helpToken,&Min,&Max);
			else
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_MONTH,&helpToken,&Min,&Max);
			break;
		case 1:
			if((dateFormat == DATE_STYLE_DDMMYYYY) || (dateFormat == DATE_STYLE_YYYYMMDD))
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_MONTH,&helpToken,&Min,&Max);
			else
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_DAY,&helpToken,&Min,&Max);
			break;
		case 2:
			if(dateFormat == DATE_STYLE_YYYYDDMM)
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_MONTH,&helpToken,&Min,&Max);
			else if(dateFormat == DATE_STYLE_YYYYMMDD)
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_DAY,&helpToken,&Min,&Max);
			else
				UefiGetDateTimeDetails(date->ControlData.ControlPtr,AMI_DATE_YEAR,&helpToken,&Min,&Max);
			break;
		default:
			break;
	}

	date->ControlData.ControlHelp = helpToken;
}

