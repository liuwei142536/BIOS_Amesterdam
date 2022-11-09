//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file tselitecommon.c
    This file contains code for the TSE Lite common functions

**/

#include "minisetup.h"


SETUP_PKG		*gSetupPkg;
SCREEN_INFO		*gScreen;
PAGE_LIST		*gToolPages;
PAGE_LIST		*gPages;
CONTROL_INFO	*gToolControlInfo;
CONTROL_INFO	*gControlInfo;
VARIABLE_LIST	*gToolVariables;
VARIABLE_LIST	*gVariables;
HPK_INFO		*gHpkInfo;
GUID_LIST		*gGuidList;
PAGE_ID_LIST	*gPageIdList;
PAGE_ID_LIST	*gToolPageIdList;
PAGE_ID_INFO	*gPageIdInfo;
PAGE_ID_INFO	*gToolPageIdInfo;
FRAME_DATA		*gActiveFrame = NULL;

UINT16	gCheckboxTokens[] =
{
	STRING_TOKEN(STR_CTRL_CHKBOX_DISABLE),
	STRING_TOKEN(STR_CTRL_CHKBOX_ENABLE)
};

UINTN gLabelLeftMargin = 2 ;
UINTN gControlLeftMargin = 24;
UINTN gControlLeftPad = 2 ;
UINTN gControlRightAreaWidth = 27 ;
BOOLEAN	gVariableChanged = FALSE;
BOOLEAN	gResetRequired = FALSE;
BOOLEAN gUserTseCacheUpdated = FALSE;
UINT32 gStartPage = 0;

APPLICATION_DATA	*gApp = NULL;
UINTN           gHpkInfoLength;
UINT32 GetControlConditionVarId(CONTROL_INFO *control);

extern CHAR8 *GlyphWidthCache;
extern UINT32 gRefreshIdCount;
extern REFRESH_ID_INFO *gRefreshIdInfo;
extern UINT32 *gProcessedForms; 
extern UINT32 gProFormOffsetAllocCount;
void FreeTseAllocMemories (void);
extern VOID RevertFlushLines();
extern BOOLEAN gBootOverrideFlag;
extern EFI_STATUS CleanNotificationQueue();
extern BOOLEAN FormBrowserHandleValid(VOID);
extern VOID FreeSpecialCtrlInfo();

/**
    Freesup UI related resources.

    @param VOID

    @retval VOID

**/
BOOLEAN TseDebugDisableApplicationDestructor (VOID);

#if EFIVARSTORE_LOOKUP_TABLE_SUPPORT
VOID _CleanVarKeyEfiTable();
#endif
VOID MiniSetupUIExit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
	UINTN index;

	// Don't do the MiniSetupExit if it is not a first call to PostManagerHandshake.
	if( !FormBrowserHandleValid() && (gPostManagerHandshakeCallIndex>2 || (gPostManagerHandshakeCallIndex == 2 && gBootOverrideFlag))) //gApp should be preserved when it is not freed.
	{
		gApp->Quit=FALSE;
		gApp->ClearScreen = TRUE;
		return;
	}
	
	if ( gApp != NULL )
	{
		if (!TseDebugDisableApplicationDestructor ())
		{
			gApplication.Destroy( gApp, TRUE );
		}
		gApp = NULL;

		MouseDestroy();
	}

    //Close the Event and Free the NotifyContext created for Controls with Refresh Id.
    if(gRefreshIdInfo) {
        for(index = 0; index < gRefreshIdCount; index++) 
        {
            if(gRefreshIdInfo[index].pEvent)//Close the event
            {
	            Status = gBS->CloseEvent(gRefreshIdInfo[index].pEvent);
	            if(Status == EFI_SUCCESS)
                {
		            gRefreshIdInfo[index].pEvent = NULL;
	            }		
            }
            
            MemFreePointer( (VOID **)&gRefreshIdInfo[index].pNotifyContext );
        }
        MemFreePointer( (VOID **)&gRefreshIdInfo );
        gRefreshIdCount = 0;
    }

    UEFICallSetupFormCallBack(AMI_CALLBACK_FORM_CLOSE);
    CleanNvramVariableList((NVRAM_VARIABLE **)&gVariableList );
    CleanTempNvramVariableList();
    FreeTseAllocMemories (); //Memory Allocated by the Optimal/Failsafe Buffer for Dynamic Variables is not freed
    HiiExit( );

    RevertFlushLines();// To avoid post logo corruption if exiting from setup because of !CurrentPageUpdated.
    if(GlyphWidthCache) {
        MemFreePointer((VOID **)&GlyphWidthCache);// free the memory which was used to store the widths of each possible CHAR16 glyph.
    }
    if(gProcessedForms) 
    {
        MemFreePointer((VOID **)&gProcessedForms);
        gProFormOffsetAllocCount = 0;
    }
    FreeSpecialCtrlInfo();
#if EFIVARSTORE_LOOKUP_TABLE_SUPPORT
    _CleanVarKeyEfiTable();
#endif
}


/**
    Timeout value from the variable.

    @param DefaultValue

    @retval Timeout value

**/
UINT16 GetBootTimeOutLocal(UINT16 DefaultValue)
{
	UINT16 *TimeOut=NULL, Value = 0;
	UINTN size = 0;

	// Install boot timeout
	TimeOut = VarGetNvram( VARIABLE_ID_BOOT_TIMEOUT, &size );

	if ( TimeOut != NULL )
	{
		Value = *TimeOut;
	}
	else
	{
		Value = DefaultValue;
		VarSetNvram( VARIABLE_ID_BOOT_TIMEOUT, &Value, sizeof(UINT16) );
	}
	
	if(TimeOut) //For MemoryLeak Fix
		MemFreePointer( (VOID **)&TimeOut );

	return Value;
}
/**
    Function to get control variable.

    @param control 

    @retval UINT32

**/
UINT32 _GetControlVariable(CONTROL_INFO *control)
{
	return control->ControlVariable;
}

/**
    Returns pointer to the PAGE_INFO struct of given PgIndex

    @param PgIndex Page index
    @param PageInfo Pointer to the page information struct

    @retval EFI_STATUS status - EFI_SUCCESS if successful, else EFI_ERROR
**/
EFI_STATUS _GetPageInfo(UINT32 PgIndex, PAGE_INFO **PageInfo)
{
  EFI_STATUS status = EFI_SUCCESS;

  if((gApplicationData == NULL) || (gPages == NULL))
  {
    status = EFI_NOT_READY;
    goto DONE;
  }

  if(PgIndex > gPages->PageCount)
  {
    status = EFI_INVALID_PARAMETER;
    goto DONE;
  }

  *PageInfo = (PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[PgIndex]);

  if(NULL == *PageInfo)
	  status = EFI_NOT_FOUND;
DONE:
  return status;
}

/**
    Is this a modal page 

    @param PgIndex 

    @retval BOOLEAN
**/
#define ASSERT_ERROR_STATUS(assertion)		if(EFI_ERROR(assertion)) goto DONE
BOOLEAN IsPageModal(UINT32 PgIndex)
{
  EFI_STATUS status = EFI_SUCCESS;
  BOOLEAN isTrue = FALSE;
  PAGE_INFO *pageInfo = NULL;

  status = _GetPageInfo(PgIndex, &pageInfo);
  ASSERT_ERROR_STATUS(status);
  isTrue = pageInfo->PageFlags.PageModal ? TRUE : FALSE;

DONE:
  return isTrue;
}

/**
    Get the Page Subtitle String Index

    @param PgIndex 
    @param subtitleToken 

    @retval EFI_STATUS
        EFI_SUCCESS
        EFI_NOT_READY
        EFI_INVALID_PARAMETER
**/
EFI_STATUS GetPageSubTitle(UINT32 PgIndex, UINT16 *SubtitleToken)
{
  EFI_STATUS status = EFI_SUCCESS;
  PAGE_INFO *pageInfo = NULL;

  status = _GetPageInfo(PgIndex, &pageInfo);
  ASSERT_ERROR_STATUS(status);

  *SubtitleToken = pageInfo->PageSubTitle;

DONE:
  return status;
}

#if TSE_DEBUG_MESSAGES
/**

    @param 

    @retval 
**/
VOID SetupToggleDebugFeature(UINT16 FeatureBit, CHAR16 *Value);
extern EFI_STATUS DisplayParseFormSetData(PAGE_INFO * PageInfo) ;
extern EFI_STATUS DebugShowAllHiiVariable(UINT32 formID);
BOOLEAN SetupDebugKeyHandler(UINT16 ItemIndex, UINT16 Unicode, CHAR16 *Value)
{
          
    switch(ItemIndex)
	{
	    case 0:
            SetupToggleDebugFeature(PRINT_UEFI_PARSE, Value) ;
            break ;
        case 1:
            SetupToggleDebugFeature(PRINT_UEFI, Value) ;
            break ;
        case 2:
            SetupToggleDebugFeature(PRINT_UEFI_CALLBACK, Value) ;
            break ;
        case 3:    
            SetupToggleDebugFeature(PRINT_UEFI_NOTIFICATION, Value) ;
            break ;
        case 4:    
            SetupToggleDebugFeature(PRINT_UEFI_VAR, Value) ;
            break ;
        case 5:
        {
            PAGE_DATA *Page = (PAGE_DATA *)NULL;
                            
            if(gApp != NULL)
            {
                Page = gApp->PageList[gApp->CurrentPage];
                DisplayParseFormSetData((PAGE_INFO*)&(Page->PageData)) ; 
            }
            
        } 
            break;
	    case 6:
            DebugShowAllHiiVariable(0) ;
            break;
	    default:
            
		    break;
	}
           
	return TRUE ;
}
#endif

/**
    Function to Freeing memories while exiting TSE

    @param 

    @retval 
**/
void FreeTseAllocMemories (void)
{
	UINT32 index;
	
	extern UINT16 	*gBootOptionTokenArray;
	extern UINTN 	gBootOptionTokenCount;
	extern UINT16 	*gDriverOptionTokenArray;
	extern UINTN 	gDriverOptionTokenCount;
	
	extern EFI_HII_VALUE *mOpCodeScopeStack;
	extern EFI_HII_VALUE *mOpCodeScopeStackEnd;
	extern EFI_HII_VALUE *mOpCodeScopeStackPointer;

	extern EFI_HII_VALUE *mExpressionEvaluationStack;
	extern EFI_HII_VALUE *mExpressionEvaluationStackEnd;
	extern EFI_HII_VALUE *mExpressionEvaluationStackPointer;
	
	extern CHAR8 *gPrevLanguage;
	
//TSElite
	
	if (gOptimalDefaults || gFailsafeDefaults)
	{
		for ( index = 0; index < gVariables->VariableCount; index++)
		{
			if (gOptimalDefaults && NULL != gOptimalDefaults [index].Buffer)
				MemFreePointer ((void**)&gOptimalDefaults [index].Buffer);
			if (gFailsafeDefaults && NULL != gFailsafeDefaults [index].Buffer)
				MemFreePointer ((void**)&gFailsafeDefaults [index].Buffer);
		}
		if (NULL != gOptimalDefaults)
			MemFreePointer ((void**)&gOptimalDefaults);
		if (NULL != gFailsafeDefaults)
			MemFreePointer ((void**)&gFailsafeDefaults);		
	}

	


	MemFreePointer ((void**)&gPrevLanguage);
	
//BootOnly
	MemFreePointer ((void**)&gBootOptionTokenArray);
	gBootOptionTokenCount = 0;

	MemFreePointer ((void**)&gDriverOptionTokenArray);
	gDriverOptionTokenCount = 0; //For MemoryLeak Fix

//Uefi21
	MemFreePointer ((void**)&mOpCodeScopeStack);
	mOpCodeScopeStackEnd = NULL;
	mOpCodeScopeStackPointer = NULL;

	MemFreePointer ((void**)&mExpressionEvaluationStack);
	mExpressionEvaluationStackEnd = NULL;
	mExpressionEvaluationStackPointer = NULL;
	if(!FormBrowserHandleValid())
		CleanNotificationQueue(); //For MemoryLeak Fix - for gNotificationQueue
}
