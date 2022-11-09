//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2015, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/TseAdvanced/bootflowLib.c $
//
// $Author: Madhans $
//
// $Revision: 6 $
//
// $Date: 3/26/10 5:06p $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//

#include "minisetup.h"
BOOLEAN 	IsMouseSupported(VOID);
BOOLEAN 	IsSoftKbdSupported(VOID);
static MSGBOX_DATA *_gBootFlowMsg = NULL;
extern BOOLEAN TimerEnabled;
extern BOOT_FLOW    *gBootFlowTable;
extern UINTN gPSMaxRows;
extern UINTN gPSMaxCols;
extern VOID RevertFlushLines();

VOID EFIAPI _BootFlowCancelTimer( EFI_EVENT Event, VOID *Context )
{
	if ( _gBootFlowMsg != NULL )
		gMsgBox.Destroy( _gBootFlowMsg, TRUE );

	_gBootFlowMsg = NULL;

	TimerStopTimer( &Event );
	TimerEnabled = FALSE;
}
extern VOID SetPostScreenScrollArea(UINTN TopRow, UINTN TopCol, UINTN BottomRow, UINTN BottomCol);
VOID _BootFlowShowMessageBox( BOOT_FLOW *bootFlowPtr )
{
	AMI_IFR_MSGBOX msgBox = { 0, 0, 0, INVALID_HANDLE, 0 };
	CONTROL_INFO dummy;
	ACTION_DATA     *action = gMsgBoxAction;
	EFI_EVENT       Event;
	EFI_STATUS      Status = EFI_SUCCESS;

#if !TSE_USE_EDK_LIBRARY
#ifndef STANDALONE_APPLICATION
	if ( gConsoleControl != NULL )
		gConsoleControl->SetMode( gConsoleControl, EfiConsoleControlScreenText );
#endif
#endif

	if (gST->ConOut == NULL) // Headless system/NoVideo in BIOS
		return;

	gST->ConOut->Reset( gST->ConOut, FALSE );
#if !APTIO_4_00
    SetDesiredTextMode();
#else
	//  In Aptio use the Text mode as is it in post screen
	if(EFI_ERROR(gST->ConOut->QueryMode( gST->ConOut, gST->ConOut->Mode->Mode, &gMaxCols, &gMaxRows )))
	{ 
		gMaxRows = StyleGetStdMaxRows(); 
		gMaxCols = StyleGetStdMaxCols(); 
	}
#endif
	if(IsMouseSupported()&& IsSoftKbdSupported())//Initializing the mouse at post when mouse and softkbd present
		MouseInit();
	ClearScreen( EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY );
	
	if(gMsgBoxAction == NULL)
	{
	    Status = gAction.Create(&action);
	    if(EFI_ERROR(Status))
	        return; 
	}

	//MemSet( &msgBox, sizeof(msgBox), 0 );
	msgBox.Text = bootFlowPtr->MessageBoxToken;

	MemSet( &dummy, sizeof(dummy), 0 );
	dummy.ControlHandle = INVALID_HANDLE;
	dummy.ControlPtr = (VOID*)(UINTN)&msgBox;
	dummy.ControlFlags.ControlVisible = TRUE;

	gMsgBox.Create( (void**)&_gBootFlowMsg );
	if ( _gBootFlowMsg == NULL )
		return;

	gMsgBox.Initialize( _gBootFlowMsg, &dummy );
	gMsgBox.SetType( _gBootFlowMsg, MSGBOX_TYPE_NULL );
	gMsgBox.Draw( _gBootFlowMsg );

	if ( bootFlowPtr->MessageBoxTimeout != 0 )
	{
		// Fix to avoid "Checker: SIGN_EXTENSION"
		TimerCreateTimer( &Event, _BootFlowCancelTimer, NULL,
			TimerRelative, MultU64x32( (UINT64)TIMER_ONE_SECOND, (UINT32) bootFlowPtr->MessageBoxTimeout ), EFI_TPL_CALLBACK );
		
	    TimerEnabled = TRUE;
	    //TimerEnabled will change its value when _DisplayInfoCancelTimer is called.
	    //_DisplayInfoCancelTimer is called if timer expires or a key is pressed
	    while( TRUE == TimerEnabled) 
	    {
	        if ( action->Input.Type != ACTION_TYPE_NULL )
	            gAction.ClearAction( action );
	        if ( gAction.GetAction( action ) != EFI_SUCCESS )
	            continue;
	        Status = gMsgBox.HandleAction( _gBootFlowMsg, action );
	        if(EFI_SUCCESS == Status)
	            _BootFlowCancelTimer( Event, NULL);
	    }
	}

    if(action && (gMsgBoxAction == NULL))
    {
        gAction.Destroy(action,TRUE);
    }
    SetPostScreenScrollArea(0,0,gPSMaxRows -1,gPSMaxCols);
	RevertFlushLines();
}

VOID _BootFlowSetActivePage( BOOT_FLOW *bootFlowPtr )
{
	PAGE_INFO *pageInfo;
	UINT32	i;

	for ( i = 0; i < gPages->PageCount; i++ )
	{
		pageInfo = (PAGE_INFO *)((UINTN)gSetupPkg + gPages->PageList[i]);
		if (
			( bootFlowPtr->PageClass == gPageIdInfo[pageInfo->PageIdIndex].PageClass ) &&
			( bootFlowPtr->PageSubClass == gPageIdInfo[pageInfo->PageIdIndex].PageSubClass ) &&
			( bootFlowPtr->PageFormID == pageInfo->PageFormID ) 
		   )
		{
			gEnterSetup = TRUE;
			gStartPage = i;
			break;
		}
	}

}

VOID _BootFlowSetActiveControl( BOOT_FLOW *bootFlowPtr )
{
	// select the control from the currently active page
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
