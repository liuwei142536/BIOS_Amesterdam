//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file protocol.c
    This file contains code for TSE exported protocol
    functions.

**/

#include "minisetup.h"
#include "boot.h"
#include "Include/Protocol/AMIPostMgr.h"
#include "Include/Protocol/AMIScreenMgmt.h"

UINT32 gBootFlow = BOOT_FLOW_CONDITION_NORMAL;
BOOLEAN gBootNextFlag = FALSE;
UINT16 *pOSRecoveryOrder = (UINT16 *)NULL;
BOOLEAN OsRecoveryFlag = FALSE; //To Check OS recovery Options are Executed.
BOOLEAN PlatformRecoveryFlag = FALSE;//To Check Platform recovery Options are Executed.
BOOLEAN gOsRecoverySupported=FALSE; //OS recovery Support 
BOOLEAN DoNormalBootFlag=TRUE;
BOOLEAN gPlatformRecoverySupported=FALSE;
BOOLEAN gDoNotBoot = FALSE;
BOOLEAN gLaunchOtherSetup = FALSE;
BOOLEAN gDrawQuietBootMessage = FALSE;
UINT8 gRecoveryBootingType = 0; /* 0 - Normal boot, 1 - OsRecovery Boot and 2 - PlatformRecovery Boot */
extern UINT16 gGifSrcX;
extern UINT16 gGifSrcY;
extern BOOLEAN GifImageFlag;
extern BOOLEAN ActivateInputDone;
BOOT_DATA *RecoveryGetRecoveryData( UINT16 Option, UINT8 RecoveryBootingType);
EFI_STATUS ProcessRecovery(UINT16 *pRecoveryOrder,UINTN size, UINT8 RecoveryBootingType);
VOID StopClickEvent(VOID);
void ClearGrphxScreen (void);

static EFI_HANDLE gProtocolHandle = NULL;
//EFI_GUID gAmiPostManagerProtocolGuid = AMI_POST_MANAGER_PROTOCOL_GUID;
BOOLEAN TseIgnoreKeyForFastBoot();
BOOLEAN GetBBSOptionStatus(BOOT_DATA *pBootData); 
extern VOID *SavePostScreen( UINTN *SizeOfX, UINTN *SizeOfY );
extern VOID RestorePostScreen( VOID *UgaBlt, UINTN SizeOfX, UINTN SizeOfY );
extern BOOLEAN IsTSEGopNotificationSupport();
extern UINTN GetAMITSESETUPSize();
extern UINT32 IsPartialKeySupport();
BOOLEAN IsSetupPrintEvalMessage();
VOID InvalidateStatusInBgrtWrapper (VOID);
extern UINTN gPSMaxRows,gPSMaxCols;
extern VOID RevertFlushLines();
static AMI_POST_MANAGER_PROTOCOL	gPostManagerProtocol =
{
	PostManagerHandshake,
	PostManagerDisplayPostMessage,
    PostManagerDisplayPostMessageEx,
    PostManagerDisplayQuietBootMessage,
    PostManagerDisplayMsgBox,
    PostManagerSwitchToPostScreen,
	PostManagerSetCursorPosition,
	PostManagerGetCursorPosition,
	PostManagerInitProgressBar,
	PostManagerSetProgressBarPosition,
	PostManagerGetPostStatus,
	PostManagerDisplayInfoBox,
	PostManagerSetAttribute,
	PostManagerDisplayMenu,
	PostManagerDisplayMsgBoxEx,
	PostManagerDisplayProgress,
	PostManagerGetAttribute, 	
	PostManagerDisplayTextBox 	
};

static AMI_TSE_SCREEN_MGMT_PROTOCOL	gScreenMgmtProtocol =
{
		ScreenMgmtSaveTseScreen, 	
		ScreenMgmtRestoreTseScreen,
		ScreenMgmtClearScreen
};

EFI_STATUS EFIAPI CreatePopup (  IN  EFI_HII_POPUP_PROTOCOL *This,IN  EFI_HII_POPUP_STYLE PopupStyle,IN  EFI_HII_POPUP_TYPE  PopupType,IN  EFI_HII_HANDLE  HiiHandle,IN  EFI_STRING_ID  Message,  OUT EFI_HII_POPUP_SELECTION   *UserSelection  OPTIONAL  );
EFI_HII_POPUP_PROTOCOL HiiPopupProtocol =
{
        EFI_HII_POPUP_PROTOCOL_REVISION,
        CreatePopup
};
static UINTN	gScreenWidth;
static UINTN 	gScreenHeight;
static EFI_UGA_PIXEL *gScreenBuffer;
static SCREEN_BUFFER *gScreenBuf = NULL;

EFI_STATUS EFIAPI InvalidateBgrtStatusByProtocol (VOID);
TSE_INVALIDATE_BGRT_STATUS_PROTOCOL gInvalidateBgrtStatus = {InvalidateBgrtStatusByProtocol};
EFI_STATUS InstallInvalBGRTStatusProtocol (EFI_HANDLE Handle);

EFI_STATUS ShowPostMsgBox(IN CHAR16  *MsgBoxTitle,IN CHAR16  *Message,IN UINT8  MsgBoxType, UINT8 *pSelection);


EFI_STATUS ShowPostMsgBoxEx(
	IN CHAR16			*Title,
 	IN CHAR16			*Message,
 	IN CHAR16			*Legend,
 	IN MSGBOX_EX_CATAGORY	 	MsgBoxExCatagory,
 	IN UINT8	 		MsgBoxType,
    	IN UINT16			*OptionPtrTokens,	// Valid only with MSGBOX_TYPE_CUSTOM 
    	IN UINT16	 		OptionCount,		// Valid only with MSGBOX_TYPE_CUSTOM
    	IN AMI_POST_MGR_KEY		*HotKeyList, 		// NULL - AnyKeyPress closes
    	IN UINT16 			HotKeyListCount, 
    	OUT UINT8			*MsgBoxSel,
    	OUT AMI_POST_MGR_KEY		*OutKey
	);
EFI_STATUS ShowInfoBox(IN CHAR16  *InfoBoxTitle, IN CHAR16  *Message, IN UINTN   TimeLimit, EFI_EVENT  *RetEvent);

EFI_STATUS HiiString2BltBuffer(	CHAR16 *Message,
								EFI_UGA_PIXEL Foreground, 
								EFI_UGA_PIXEL Background, 
								OUT	UINTN *Width,
								OUT EFI_UGA_PIXEL **BltBuffer,
								OUT UINTN *BltGlyphWidth);
EFI_STATUS DrawHiiStringBltBuffer(CHAR16 *Message, INTN CoOrdX, INTN CoOrdY, CO_ORD_ATTRIBUTE Attribute, EFI_UGA_PIXEL Foreground, EFI_UGA_PIXEL Background);
EFI_STATUS ShowPostMenu (
    IN VOID	*HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN UINT16 	LegendToken,	  
    IN POSTMENU_TEMPLATE *MenuData,
    IN UINT16 	MenuCount,
    OUT UINT16  *pSelection
);

EFI_STATUS ShowPostTextBox(
    IN VOID	    *HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN TEXT_INPUT_TEMPLATE *InputData,
    IN UINT16 	ItemCount,
    IN DISPLAY_TEXT_KEY_VALIDATE DisplayTextKeyValidate
);

EFI_STATUS ShowPostProgress(
    IN UINT8			ProgressBoxState, 
    IN CHAR16			*Title,
    IN CHAR16			*Message,
    IN CHAR16			*Legend,	
    IN UINTN 			Percent,	
    IN OUT VOID			**Handle,	
    OUT AMI_POST_MGR_KEY	*OutKey
);
BOOLEAN   	IsTseBestTextGOPModeSupported (VOID);
EFI_STATUS	SaveCurrentTextGOP ( UINTN *currenttextModeCols, UINTN *currenttextModeRows, UINT32 *currentGOPMode );
EFI_STATUS	RestoreTextGOPMode ( UINTN prevTextModeCols, UINTN prevTextModeRows, UINT32 prevGOPMode ); 
VOID SaveGraphicsScreen(VOID);
extern VOID CallAmiPauseKeyProtocol();
extern BOOLEAN GifImageFlag;
extern VOID StartGIFTimer();
extern VOID StopGIFTimer();
extern EFI_STATUS ESAInitFvHook (void);
/**
    This function installs different protocols exported.

    @param VOID

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS InstallProtocol( VOID )
{
	EFI_STATUS Status;

	Status = gBS->InstallMultipleProtocolInterfaces(
			&gProtocolHandle,
			&gAmiPostManagerProtocolGuid, &gPostManagerProtocol,
#ifdef USE_COMPONENT_NAME
			&gEfiComponentNameProtocolGuid, &gComponentName,
#endif
			NULL
			);
	if ( !EFI_ERROR( Status ) )
	{
		Status = InstallFormBrowserProtocol(gProtocolHandle);
		Status = InstallInvalBGRTStatusProtocol (gProtocolHandle);
		Status = InstallScreenMgmtProtocol (gProtocolHandle);
		Status = InstallHiiPopupProtocol(gProtocolHandle);
		
	}

	return Status;
}

/**
    This function uninstalls different protocols exported.

    @param VOID

    @retval VOID

**/
VOID UninstallProtocol( VOID )
{
	gBS->UninstallMultipleProtocolInterfaces(
			&gProtocolHandle,
			&gAmiPostManagerProtocolGuid, &gPostManagerProtocol,
#ifdef USE_COMPONENT_NAME
			&gEfiComponentNameProtocolGuid, &gComponentName,
#endif
			NULL
			);

	UnInstallFormBrowserProtocol(gProtocolHandle);
	UninstallScreenMgmtProtocol(gProtocolHandle);
	UninstallHiiProtocolProtocol(gProtocolHandle);
	
}

/**
    This function installs Screen management protocol.

    @param EFI_HANDLE Handle

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS InstallScreenMgmtProtocol( EFI_HANDLE Handle)
{
	EFI_STATUS Status = EFI_SUCCESS;
	
	Status = gBS->InstallMultipleProtocolInterfaces (
				  &Handle,
				  &gTSEScreenMgmtProtocolGuid,
				  &gScreenMgmtProtocol,
				  NULL
				  );
	
	return Status;
}

/**
    This function uninstalls screen management protocol.

    @param EFI_HANDLE Handle

    @retval VOID

**/
VOID UninstallScreenMgmtProtocol( EFI_HANDLE Handle)
{
	gBS->UninstallMultipleProtocolInterfaces(
			&Handle,
			&gTSEScreenMgmtProtocolGuid, 
			&gScreenMgmtProtocol,
			NULL
			);
}

VOID EFIAPI SetBootTimeout( EFI_EVENT Event, BOOLEAN *timeout )
{
	if ( timeout != NULL )
		*timeout = TRUE;
}

VOID AboartFastBootPath(VOID)
{
	gBootFlow = BOOT_FLOW_CONDITION_NORMAL;
	gPostManagerHandshakeCallIndex--;
    gRT->SetVariable(
    			L"BootFlow",
    			&_gBootFlowGuid,
    			EFI_VARIABLE_BOOTSERVICE_ACCESS,
    			sizeof(gBootFlow),
    			&gBootFlow
    			);
}
BOOLEAN IsBootTimeOutValueZero(VOID);
/**
    This function is the handshake function to which BDS
    hands-off.

    @param VOID

    @retval This function never returns. It only boots different
        options.

**/
extern VOID SetPostScreenScrollArea(UINTN TopRow, UINTN TopCol, UINTN BottomRow, UINTN BottomCol);
EFI_STATUS EFIAPI PostManagerHandshake( VOID )
{
	EFI_EVENT timer = NULL;
	EFI_STATUS Status = EFI_UNSUPPORTED;

	volatile BOOLEAN bootTimeout = FALSE;
	UINT16  Value = 1;
    UINT64  TimeoutValue;
	UINTN   size = 0;
	EFI_GUID    AmitseAfterFirstBootOptionGuid = AMITSE_AFTER_FIRST_BOOT_OPTION_GUID;
	EFI_GUID AmiTseBeforeTimeOutGuid = AMITSE_BEFORE_TIMEOUT_GUID;
	EFI_GUID AmiTseAfterTimeOutGuid = AMITSE_AFTER_TIMEOUT_GUID;
	BOOLEAN AfterFirstBootSignalled = FALSE;
	UINT16  *pBootOrder=NULL;
	UINT16  u16BootCount = 0,i;				//Dont change i data type, in infinite loop it is reinitialized to -1.
	UINT16  BootNext, *pBootNext = NULL;
	UINTN   RecoverySize = 0;
	UINT16 *pRecoveryOrder=NULL;
	BOOLEAN BootSuccess=FALSE;
	UINT64 		OsIndications = 0;
	UINT32 		Attributes = 0;
	UINTN 		DataSize = sizeof (UINT64);  
	AMI_POST_MANAGER_PROTOCOL   *mPostMgr                   = NULL;

	// Performance measurement starta
	PERF_START (0, AMITSE_TEXT("Boot"), NULL, 0);

	RUNTIME_DEBUG( L"mre" );

	gPostManagerHandshakeCallIndex++;

	if(gPostManagerHandshakeCallIndex!=1)
	{
		gEnterSetup = TRUE;
		goto _ShowMainMenu;
	}

    PostManagerHandShakeHookHook();

#if !TSE_USE_EDK_LIBRARY
#ifndef STANDALONE_APPLICATION
	if ( ! gConsoleControl )
	{
		//All necessary protocols are not available yet.
		//We can still proceed if Uga Draw is the only protocol not available yet.
		ActivateApplication();
		if (!IsDelayLogoTillInputSupported())
		{
			ActivateInputDone = TRUE;
			ActivateInput();
		}
	}
#endif
#endif

    if(!gVariableList)
    {
        VarLoadVariables( (VOID **)&gVariableList, NULL );
    }

	TimerStopTimer( &gKeyTimer );

	// this *MUST* be run a EFI_TPL_APPLICATION
	gBS->RaiseTPL( EFI_TPL_HIGH_LEVEL );	// guarantees that RestoreTPL won't ASSERT
	gBS->RestoreTPL( EFI_TPL_APPLICATION );

	// Performance measurement Pause
//	PERF_END (0,L"Boot", NULL, 0);
	// Exclude IDEPasswordCheck IDEPasswordCheck that it may get the use input.
	// IDE password Module takes care of the Password check.
	//TSEIDEPasswordCheck(); 

	// Performance measurement continue
//	PERF_START (0, L"Boot", NULL, 0);    

    // get the current boot options and languages
	BootGetBootOptions();
	BootGetLanguages();

	StyleUpdateVersionString();

	gPostStatus = TSE_POST_STATUS_IN_BOOT_TIME_OUT;

	//Providing boot time out value depending on TSE_BOOT_TIME_OUT_AS_ZERO token
	if(!IsBootTimeOutValueZero())
		Value = GetBootTimeOut(Value);
	else
		Value = 0;

#ifdef STANDALONE_APPLICATION
    Value = 0;
    gEnterSetup = TRUE;
#endif

	if ( Value == 0 )
		bootTimeout = TRUE;

	if ( !TseIgnoreKeyForFastBoot() ) 
	{
		CheckForKeyHook( (EFI_EVENT)NULL, NULL );
	}

    if((BOOT_FLOW_CONDITION_NORMAL != gBootFlow) || (gEnterSetup==TRUE))
    {
        UINT32 condition = BOOT_FLOW_CONDITION_NORMAL;
        UINT32 *conditionPtr;

        size = 0;
        conditionPtr = VarGetNvramName( L"BootFlow", &_gBootFlowGuid, NULL, &size );
        if ( conditionPtr != NULL )
        	condition = *conditionPtr;
        
        MemFreePointer( (VOID **)&conditionPtr );

		if((condition == BOOT_FLOW_CONDITION_FAST_BOOT) && ((gBootFlow != condition)||(gEnterSetup==TRUE)))
		{
			// Take the Normal boot path as Fast boot path is altered by CheckForKeyHook().
			AboartFastBootPath();
			return EFI_UNSUPPORTED;
		}

        if(BOOT_FLOW_CONDITION_NORMAL == condition)
        {
            gRT->SetVariable(
    					L"BootFlow",
    					&_gBootFlowGuid,
    					EFI_VARIABLE_BOOTSERVICE_ACCESS,//BootFlow wil use RS attribute to avoid inconsistent attributes
    					sizeof(UINT32),
    					&gBootFlow
    					);
        }
        else
            gBootFlow = BOOT_FLOW_CONDITION_NORMAL;
    }

	Status = BootFlowManageEntry();
	if (Status == EFI_UNSUPPORTED)
    {
        Value = 0xFFFF;
        gDoNotBoot = TRUE;
    }
	if(Status == EFI_NOT_STARTED)
		bootTimeout = TRUE;

	if(gBootFlow == BOOT_FLOW_CONDITION_FAST_BOOT)
	{
		UINT32 PasswordInstalled = PasswordCheckInstalled();
		
		if((gPasswordType == AMI_PASSWORD_NONE) && (PasswordInstalled != AMI_PASSWORD_NONE))
		{
			if(CheckSystemPasswordPolicy(PasswordInstalled))
			{
				// Take the Normal boot path as Password is installed and not validated by the user yet
				AboartFastBootPath();
				return EFI_UNSUPPORTED;
			}
		}
	}
    else
    {
	// Reinit Before Boottimeout So any action can set the boot flow again.
	gBootFlow = BOOT_FLOW_CONDITION_NORMAL;
    }

	if ( Value != 0xFFFF )
    {
        TimeoutValue = (UINT64) TIMER_TENTH_SECOND;
        TimeoutValue = MultU64x32( TimeoutValue, (UINT32) Value );

        TimerCreateTimer( &timer, (EFI_EVENT_NOTIFY) SetBootTimeout, (VOID *)&bootTimeout, TimerRelative, TimeoutValue, EFI_TPL_CALLBACK );
    }

	// Performance measurement Pause
	PERF_END (0, AMITSE_TEXT("Boot"), NULL, 0);

	// While the boot timeout has not expired
	EfiLibNamedEventSignal (&AmiTseBeforeTimeOutGuid);
	while ( ! bootTimeout )
	{
		//PAUSE key protocol support  
		CallAmiPauseKeyProtocol();
		if ( gEnterSetup || gBootFlow )
		{
			bootTimeout = TRUE;
            continue;
		}
		if ( !TseIgnoreKeyForFastBoot() ) 
		{
			// check for 'hotkey' actions that would invoke something
			CheckForKeyHook( (EFI_EVENT)NULL, NULL );
		}

        TimeOutLoopHookHook();
	}
	EfiLibNamedEventSignal (&AmiTseAfterTimeOutGuid);

	// Performance measurement continue
	PERF_START (0, AMITSE_TEXT("Boot"), NULL, 0);

	TimerStopTimer( &timer );

    if ((!gDoNotBoot) && (gBootFlow))
    {
        gRT->SetVariable(
					L"BootFlow",
					&_gBootFlowGuid,
					EFI_VARIABLE_BOOTSERVICE_ACCESS,//BootFlow wil use RS attribute to avoid inconsistent attributes
					sizeof(UINT32),
					&gBootFlow
					);
        BootFlowManageEntry();
    }

_ShowMainMenu:

	if ( gEnterSetup )
	{
    	// Performance measurement Pause
    	PERF_END (0, AMITSE_TEXT("Boot"), NULL, 0);
    	// Disable access to the post manager protocol display post message functions and to 
        //  the switch to post screen functions of post manager protocol
    	gPostMsgProtocolActive = FALSE;

		gPostStatus = TSE_POST_STATUS_ENTERING_TSE;
		gSetupContextActive = TRUE;

		Status = gST->ConIn->Reset( gST->ConIn, FALSE );
		Status = MainSetupLoopHook();
		gSetupContextActive = FALSE;
		

    	// Reenable access to the post manager protocol display post message functions and to 
        //  the switch to post screen functions of post manager protocol
    	gPostMsgProtocolActive = TRUE;
    	gMaxRows = gPSMaxRows;          //Setting screen with post screen resolution after exiting from setup, Clearing screen, displaying post messages from top when we do save and exit from setup
    	gMaxCols =gPSMaxCols;
    	SetDesiredTextMode ();
    	ClearScreen( EFI_BACKGROUND_BLACK | EFI_LIGHTGRAY );
        SetPostScreenScrollArea(0,0,gPSMaxRows -1,gPSMaxCols);
		RevertFlushLines();
    	// Performance measurement continue
    	PERF_START (0, AMITSE_TEXT("Boot"), NULL, 0);
	}


	if(gPostManagerHandshakeCallIndex!=1)
	{
		// In case of not a first call
		// Don't do the Boot Manager Work just exit the to caller.
		gPostManagerHandshakeCallIndex--;
		return Status;
	}

	// Pass control to the boot process to handle the selected boot option
	// if the boot is allowed
    if (gDoNotBoot)
    {
        gEnterSetup = TRUE;
		goto _ShowMainMenu;
	}

	size = 0;
	//VarGetNvram( VARIABLE_ID_AMITSESETUP, &size );
	HelperGetVariable( VARIABLE_ID_AMITSESETUP, (CHAR16 *)NULL, (EFI_GUID *)NULL, NULL, &size );
	
	if(size < GetAMITSESETUPSize())
	{
		Status = MiniSetupEntry();
		MiniSetupExit( Status );
	}

	gPostStatus = TSE_POST_STATUS_PROCEED_TO_BOOT;

	// LoadDriverOrder();	//Remove Load Driver Option from TSE 2.x as it is handling from Core.
	 if(GifImageFlag)   //Clearing the gif image before launching bootoption
	 {
	     CleanUpLogo ();
	 }

    ProcessProceedToBootHook();

    TSEUnlockHDD();
    {
        //Get the Boot options Count first,Because under Platform recovery and BootFlow InfiniteLoop
		//we need to execute Platform Recovery Options First and normal boot options Next.
	    UINTN bootordersize=0;
	    size =0 ;
	    pBootOrder = (UINT16 *)VarGetNvramName( L"BootOrder", &gEfiGlobalVariableGuid, NULL, &size );
	    bootordersize = size/sizeof(UINT16);
	#ifdef EFI_NT_EMULATOR
	    if(!pBootOrder || !gBootData)
	    {
	        VarSetNvramName(L"BootOrder",
	        &gEfiGlobalVariableGuid,
	        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
	        NULL,
	        0);
	
	        return Status;
	    }
	#endif
	
	    //Get enabled options count
	    for(u16BootCount=0; u16BootCount<(UINT16)(size/sizeof(UINT16)); u16BootCount++ )
	    {
            if(!IsPreservedDisabledBootOptionOrder())
            {
    	        BOOT_DATA *pBootData;

	            pBootData = BootGetBootData(pBootOrder[u16BootCount]);
	            if(pBootData == NULL)
	                continue;
	                       
	            if(!(pBootData->Active & LOAD_OPTION_ACTIVE))
	                break;
            }
	    }
	    
	    
		//Checks for PlatfromRecovery Bit.if The bit is set,platform will boot PlatformRecovery boot options
		//it won't execute BootNext,Boot Options,OS options when PlatfromRecovery Bit set		
		if(IsRecoverySupported() && gPlatformRecoverySupported && !PlatformRecoveryFlag)
		{
			goto _ProcessToPlatformRecovery;
		}
		
	    //Try boot next first
	    size = 0;
	    pBootNext = (UINT16 *)VarGetNvramName(L"BootNext", &gEfiGlobalVariableGuid, NULL, &size);
	    if(pBootNext && (size == sizeof(UINT16)))
	    {
	        BootNext = *pBootNext;
	        //Clear Boot next
	        VarSetNvramName(L"BootNext",
	                        &gEfiGlobalVariableGuid,
	                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
	                        pBootNext,
	                        0);
	        if( gBootFlow == BOOT_FLOW_CONDITION_FAST_BOOT) { 
				// FastBoot module normally checks for Bootnext before taking the FastBoot path
				// If the Boot next is set in FastBoot case then do the full boot.
				    AboartFastBootPath();
					return Status;
				}
			else {
				BBSSetBootNowPriorityForBootNext(BootNext, NULL, 0);
			}
		}
	
	    if(pBootNext)
	    {
	        MemFreePointer((void **) &pBootNext);
	    }

		if(!u16BootCount)
	    {
			gPostStatus = TSE_POST_STATUS_NO_BOOT_OPTION_FOUND;
	        //No valid/enabled boot option
			Status = BootFlowManageExit();
	            //infinite loop
	            while(EFI_NOT_STARTED == Status)
	            {
                    CHAR16 *text = NULL;
                    
                    text = HiiGetString( gHiiHandle, STRING_TOKEN(STR_NO_BOOT_OPTIONS));
                    if ( ( text != NULL ) && (gST->ConOut != NULL))
                    {
                        if( gQuietBoot)
                            gST->ConOut->OutputString(gST->ConOut, text);
                        else
                        { 
                            if(!mPostMgr)
                            {
                                Status = pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL,(void**) &mPostMgr);
                                if (EFI_SUCCESS != Status) 
                                    mPostMgr=NULL;
                            }   

                            if(mPostMgr)
                                mPostMgr->DisplayPostMessage(text);
                        }
                        MemFreePointer( (VOID **)&text );
                    }
                    
                    //Wait for key
                    gBS->WaitForEvent( 1, &gST->ConIn->WaitForKey, &size );
                    CheckForKeyHook( (EFI_EVENT)NULL, NULL );
                    if (TRUE == gEnterSetup)                          //If setup key detects then launch the setup
                    {
                        goto _ShowMainMenu;
                    }
                    Status = BootFlowManageExit ();                 //If user plugs the any thumb drive after boot then TSE will try to launch the image from that file system using the path efi\boot\bootxxxx.efi
					if( gBootFlow )// Checking for gBootFlow, making gBootFlow to normal such that the key will be consumed in chcekforkey
						gBootFlow = BOOT_FLOW_CONDITION_NORMAL;
	            }
	    }
	
		if ( !gEnterSetup )// If no key is pressed to go to setup then stopping MousePointingDevice and stopping ClickEvents
		{
			StopClickEvent();
			MouseDestroy();
		}
//If Recovery support is disabled, normal boot options only should be processed
        if(IsRecoverySupported() && gOsRecoverySupported && !OsRecoveryFlag && !BootSuccess)
      	  DoNormalBootFlag = FALSE;

//Check for OS Recovery and process normal boot if unavailable
_ProcessBootOptions:			
SETUP_DEBUG_TSE("\n[TSE] _ProcessBootOptions Label Entering:\n DoNormalBootFlag = %d u16BootCount = %d\n",DoNormalBootFlag,u16BootCount);
	    for ( i = 0; (DoNormalBootFlag) && (i < u16BootCount); i++)
	    {
	        BOOT_DATA *pBootData;

	        pBootData = BootGetBootData(pBootOrder[i]);
	  
	        if(pBootData)
	        {
	        	SETUP_DEBUG_TSE("[TSE] Option[%d] = 0x%04X, Name = %s, Active = %d, BBSValidDevicePath = %d\n",i,pBootData->Option, pBootData->Name, pBootData->Active,BBSValidDevicePath(pBootData->DevicePath));
	        }
  	        if(pBootData == NULL || !(pBootData->Active & LOAD_OPTION_ACTIVE))
  	        {
  		        SETUP_DEBUG_TSE("[TSE] Invalid/Disabled boot option 0x%04X\n",pBootOrder[i]);
  		        if ((i + 1) >= u16BootCount)
  		        {
  		            goto _BootFlowExitAllOptionsFailed;
  		        }
  		        else
  		        {
  		            continue;
  		        }
  	        }

			if ( BBSValidDevicePath(pBootData->DevicePath) ) 
			{
			    if ( GetBBSOptionStatus (pBootData) )
				{
	  		        SETUP_DEBUG_TSE("[TSE] Disabled Legacy option 0x%04X\n",pBootOrder[i]);
	  		        if ((i + 1) >= u16BootCount)
	  		        {
	  		            goto _BootFlowExitAllOptionsFailed;
	  		        }
	  		        else
	  		        {
	  		            continue;
	  		        }
				}
			}
	        Status = BootLaunchBootOption(pBootOrder[i], pBootOrder+i, bootordersize - i);
	        if(!EFI_ERROR(Status))
	        	BootSuccess=TRUE;
	        if ( !AfterFirstBootSignalled )
			{
				EfiLibNamedEventSignal ( &AmitseAfterFirstBootOptionGuid );

				if( gBootFlow == BOOT_FLOW_CONDITION_FAST_BOOT) {
				    AboartFastBootPath();
					return Status;
				}	

				AfterFirstBootSignalled = TRUE;
			}
	    
_BootFlowExitAllOptionsFailed:
	        if((i+1) >= u16BootCount)
	        {
	        	gPostStatus = TSE_POST_STATUS_ALL_BOOT_OPTIONS_FAILED;
	        	Status = BootFlowManageExit();
	        	if(!EFI_ERROR(Status))  //On BootFlowManage Success it won't execute the reset of Boot Cases.
		        	BootSuccess=TRUE;
	        	
	        	//if the BootFlow is in InfiniteLoop and OS recovery and PlatfromRecovey is not performed,Infinite loop code won't execute
	        	if ( (EFI_NOT_STARTED == Status)  && 
	        			(!IsRecoverySupported() || ((OsRecoveryFlag == TRUE) && (PlatformRecoveryFlag == TRUE)))
	        			)
				{
                    CheckForKeyHook( (EFI_EVENT)NULL, NULL );
                    if (TRUE == gEnterSetup)                          //If setup key detects then launch the setup
                    {
                        goto _ShowMainMenu;
                    }
					// Infinite loop is true so we start again
					i = 0xffff;											//Changed 0 to 0xffff. Then only in next iteration it will be zero otherwise it will be 1.
				}													//If it is i = 0 then it will miss first boot option
	        }
	    }
	    if(IsRecoverySupported())
	    {
            //OSRecovery is performed if first time boot failed or gOsRecoverySupported flag is true
            if( (!OsRecoveryFlag) && (!BootSuccess ) )
            {
            	 DoNormalBootFlag = TRUE;
                Status = pRS->GetVariable (L"OsIndications", &gEfiGlobalVariableGuid, &Attributes, &DataSize, (VOID *)&OsIndications);
               
                if (!EFI_ERROR (Status) && DataSize ==  sizeof (UINT64))
                {
                   OsIndications = OsIndications & (~EFI_OS_INDICATIONS_START_OS_RECOVERY);
                   Status = pRS->SetVariable (L"OsIndications", &gEfiGlobalVariableGuid, Attributes, DataSize, (VOID *)&OsIndications);
                   
                   pOSRecoveryOrder  = (UINT16 *)VarGetNvramName( L"OsRecoveryOrder", &gEfiGlobalVariableGuid, NULL, &RecoverySize );
                   if(pOSRecoveryOrder != NULL && RecoverySize){
                		gRecoveryBootingType = OS_RECOVERY_OPTION_BOOTING;
                		Status= ProcessRecovery(pOSRecoveryOrder ,RecoverySize,gRecoveryBootingType);
                		gRecoveryBootingType = BOOT_OPTION_BOOTING;
                        if (!EFI_ERROR (Status))
                        BootSuccess=TRUE;
                   }
                   if(pOSRecoveryOrder) //For MemoryLeak Fix
                  	 MemFreePointer((void **) &pOSRecoveryOrder); 
                }	
            }
            //It will again try normal boot for second time when OS Recovery failed.
            if( (!OsRecoveryFlag) && (!BootSuccess) ){
            	 OsRecoveryFlag = TRUE;                
                goto _ProcessBootOptions;
            }
            
_ProcessToPlatformRecovery:
				//Platform Recovery is performed when boot order failed for second time also or OSRecovery failed
				if(!PlatformRecoveryFlag && !BootSuccess)
            {
                Status = pRS->GetVariable (L"OsIndications", &gEfiGlobalVariableGuid, &Attributes, &DataSize, (VOID *)&OsIndications);
               
                if (!EFI_ERROR (Status) && DataSize ==  sizeof (UINT64))
                {
                   OsIndications = OsIndications & (~EFI_OS_INDICATIONS_START_PLATFORM_RECOVERY);
                   Status = pRS->SetVariable (L"OsIndications", &gEfiGlobalVariableGuid, Attributes, DataSize, (VOID *)&OsIndications);
                   
                   pRecoveryOrder  = (UINT16 *)VarGetNvramName( L"PlatformRecoveryOrder", &gPlatformRecoveryOrderGuid, NULL, &RecoverySize );
                   if(pRecoveryOrder != NULL && RecoverySize)
                   {
                	   gRecoveryBootingType = PLATFORM_RECOVERY_OPTION_BOOTING;
                	   Status= ProcessRecovery(pRecoveryOrder ,RecoverySize,gRecoveryBootingType);
                	   gRecoveryBootingType = BOOT_OPTION_BOOTING;
                     if (!EFI_ERROR (Status))
                         BootSuccess=TRUE; 
                     else
                     {
                     	gPostStatus = TSE_POST_STATUS_ALL_PLATFORM_RECOVERY_OPTIONS_FAILED;
                     	Status = BootFlowManageExit();
                     	if (!EFI_ERROR (Status))
                     	{
                     		BootSuccess=TRUE;
                     	}
                     	
                     }
   
                   }
                   if(pRecoveryOrder) //For MemoryLeak Fix
                  	 MemFreePointer((void **) &pRecoveryOrder);
                }	
            }
				
            //To Avoid Executing the Os Recovery and Platfrom Recovery on Next Boot[After exiting from setup]
            PlatformRecoveryFlag = TRUE;
            OsRecoveryFlag = TRUE;
				
			//checking for Boot Flow InfiniteLoop value,if it is InfiniteLoop and all PlatformRecovery options failed,executing Normal Boot Options    
            if( IsBootFlowInfinite() && (!BootSuccess))
            {
            	goto _ProcessBootOptions;
            }
	    }
	}
#ifndef STANDALONE_APPLICATION
	SetupReentryHookHook();
    gEnterSetup = TRUE;
    goto _ShowMainMenu;
#else
	return Status;
#endif

}

/**
    This function is the protocol to display messages in
    the post screen.

    @param message : Unicode string to be displayed.

    @retval Return Status based on errors that occurred in library
        functions.

**/
extern ESA_INTERFACES_FOR_TSE   *gEsaInterfaceForTSE;
EFI_STATUS EFIAPI PostManagerDisplayPostMessage( CHAR16 *message )
{
   
    EFI_STATUS    Status = EFI_UNSUPPORTED;
    Status = gBS->LocateProtocol(&gEsaInterfacesForTseProtocolGuid, NULL,(void**) &gEsaInterfaceForTSE);
    if (!EFI_ERROR (Status))
    {
        //Get the data of gPostMsgProtocolActive from ESA to TSE
        Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (PostMessageProtocolActive,0,0,(UINTN*)&gPostMsgProtocolActive);
    }
	
	if ( ! gPostMsgProtocolActive )
		return EFI_UNSUPPORTED;

	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

	return PrintPostMessage(message, TRUE);
}

/**
    This function is the protocol to display messages with
    attributes in the post screen.

    @param message : Unicode string to be displayed.
    @param Attribute : Attribute for the message

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS EFIAPI PostManagerDisplayPostMessageEx( CHAR16 *message, UINTN Attribute )
{
    EFI_STATUS    Status = EFI_UNSUPPORTED;
    Status = gBS->LocateProtocol(&gEsaInterfacesForTseProtocolGuid, NULL,(void**) &gEsaInterfaceForTSE);
    if (!EFI_ERROR (Status))
    {
        //Get the data of gPostMsgProtocolActive from ESA to TSE
        Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (PostMessageProtocolActive,0,0,(UINTN*)&gPostMsgProtocolActive);
    }
	if ( ! gPostMsgProtocolActive )
		return EFI_UNSUPPORTED;

	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

	switch(Attribute)
    {
        case PM_EX_DONT_ADVANCE_TO_NEXT_LINE:
            return PrintPostMessage(message, FALSE);
    }

    return EFI_UNSUPPORTED;
}

/**
    This function is the protocol to switch to post screen

    @param VOID

    @retval Return Status based on errors that occurred in library
        functions.

**/
EFI_STATUS EFIAPI PostManagerSwitchToPostScreen( VOID )
{
	EFI_GUID tSwitchToPoscreenGuid = AMITSE_SWITCHING_TO_POST_SCREEN_GUID;

#ifdef STANDALONE_APPLICATION
	return EFI_UNSUPPORTED;
#else
	if ( ! gPostMsgProtocolActive )
		return EFI_UNSUPPORTED;

	//Signalling event while using SwitchingToPostScreen interface
	EfiLibNamedEventSignal (&tSwitchToPoscreenGuid);
	
    if ( gQuietBoot )
	{
		UpdateGoPUgaDraw();
      if (!gGOP)
		 return EFI_UNSUPPORTED;
	
		gQuietBoot = FALSE;
		CleanUpLogo();
		//If Post message printed in the Quiteboot by using Set cur position
		// It is better to flash all the screen.
		//FlushLines( 0, gMaxRows - 1 );
		//DoRealFlushLines(); 
		InitPostScreen();
      return EFI_SUCCESS;
	}

    return EFI_UNSUPPORTED;
#endif
}
/**
    Initializes the PostManagerProgressBar

    @param x Column or Left screen position of ProgressBar in pixels
    @param y Row or Top screen position of ProgressBar in pixels
    @param w Width of ProgressBar in pixels
    @param h Height of ProgressBar in pixels
    @param iterate Total number of iterations or Calls
        to advanced the progressbar to 100% of the given width.
        The delta or changed value will be computed.								 

    @retval EFI_STATUS status - If the function runs correctly, returns
        EFI_SUCCESS, else other EFI defined error values.

**/

EFI_STATUS EFIAPI PostManagerInitProgressBar(
	UINTN x,
	UINTN y,
	UINTN w,
	UINTN h,
	UINTN iterate)
{
	EFI_STATUS Status = 0;
	EFI_UGA_PIXEL	*BGColor;
	EFI_UGA_PIXEL	*BDRColor;
	EFI_UGA_PIXEL	*FillColor;
#if !TSE_USE_EDK_LIBRARY
    EFI_CONSOLE_CONTROL_SCREEN_MODE ConsoleMode = 0;
#endif

	BGColor = EfiLibAllocateZeroPool(sizeof (EFI_UGA_PIXEL));
	BDRColor = EfiLibAllocateZeroPool(sizeof (EFI_UGA_PIXEL));
	FillColor = EfiLibAllocateZeroPool(sizeof (EFI_UGA_PIXEL));
	
	GetProgressColor(BGColor,BDRColor,FillColor);
	
	gProgress->delta = w/iterate;
	gProgress->w = gProgress->delta*iterate;
	gProgress->h = h;
	gProgress->x = x;
	gProgress->y = y;
	gProgress->backgroundColor = BGColor;
	gProgress->borderColor = BDRColor;
	gProgress->fillColor = FillColor;

#if !TSE_USE_EDK_LIBRARY	
	if (gConsoleControl)
	        gConsoleControl->GetMode(gConsoleControl, &ConsoleMode, NULL, NULL);
	    
	if((EfiConsoleControlScreenGraphics == ConsoleMode)&&( gPostStatus <= TSE_POST_STATUS_IN_BOOT_TIME_OUT))
	    gProgress->quiteBootActive = TRUE ;
	else
#endif	
	    gProgress->quiteBootActive = FALSE ;
	
	gProgress->active = TRUE ;         // progressbar has been initialized
	gProgress->completed =0;
	return Status;
}

/**
    Increments the PostManagerProgressBar

    @param VOID
				 

    @retval EFI_STATUS status - If the function runs correctly, returns
        EFI_SUCCESS, else other EFI defined error values.

**/

EFI_STATUS EFIAPI PostManagerSetProgressBarPosition()
{
	EFI_STATUS Status = EFI_SUCCESS;
	
	if(gProgress->active)
		DrawBltProgressBar();
	else
		Status = EFI_UNSUPPORTED;
	return Status;
}

/**
    Wrapper function for SetCurPos

    @param UINTN X - Value of the column 
        UINTN Y - Number of row below the last written line

    @retval EFI_STATUS status - If the function runs correctly, returns
        EFI_SUCCESS, else other EFI defined error values.

**/

EFI_STATUS EFIAPI PostManagerSetCursorPosition(UINTN X, UINTN Y)
{
	return SetCurPos(X,Y);
}

/**
    Wrapper function for GetCurPos
    Writes cursor position into given X and Y locations.

    @param pX Pointer to storage for current column value 
    @param pY Pointer to storage for current row value

    @retval EFI_STATUS status - EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/

EFI_STATUS EFIAPI PostManagerGetCursorPosition(UINTN *pX, UINTN *pY)
{
	return GetCurPos(pX, pY);
}

/**
 

    @param MsgBoxTitle 
    @param Message 
    @param MsgBoxType 

    @retval EFI_STATUS status and UINT8 MsgBoxSel

**/
VOID SaveGraphicsScreen(VOID);
VOID RestoreGraphicsScreen(VOID);
EFI_STATUS
EFIAPI PostManagerDisplayMsgBox (
    IN CHAR16  *MsgBoxTitle,
    IN CHAR16  *Message,
    IN UINT8   MsgBoxType,
    OUT UINT8  *MsgBoxSel
	)
{

	EFI_STATUS Status = EFI_UNSUPPORTED;
	BOOLEAN ClickTimer = FALSE,KeyTimer = FALSE;
	EFI_TPL OldTpl = TPL_APPLICATION;

	if (NULL == gST->ConOut)
		return EFI_NOT_READY;
	
	if (gKeyTimer)
	{
		TimerStopTimer( &gKeyTimer );
		KeyTimer  = TRUE;
	}
	if(gClickTimer)
	{
		StopClickEvent();
		ClickTimer = TRUE;
	}
	if(GifImageFlag)
	{
	    StopGIFTimer();
		CleanUpLogo();
	}

	Status =	InitEsaTseInterfaces ();
	
	if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN)) {
		OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
		gBS->RestoreTPL (TPL_APPLICATION);
	}
	
	if (!EFI_ERROR (Status))
	{
		Status = gEsaInterfaceForTSE->DisplayMsgBox (
			    MsgBoxTitle,
			    Message,
			    MsgBoxType,
			    MsgBoxSel
			    );
	}
	else
	{
		UINTN currenttextModeCols = 0, currenttextModeRows = 0;
		UINT32 currentGOPMode = 0;
		INT32   Attribute = gST->ConOut->Mode->Attribute;
		if( gPostStatus <= TSE_POST_STATUS_ENTERING_TSE ) // Dont do save screen for quietboot screen if TSE_GOP_NOTIFICATION_SUPPORT token enabled
			SaveGraphicsScreen();
		
		Status = SaveCurrentTextGOP (&currenttextModeCols, &currenttextModeRows, &currentGOPMode);
		if (EFI_ERROR(Status))
			goto DONE;

		Status = ShowPostMsgBox(MsgBoxTitle, Message, MsgBoxType,MsgBoxSel);

		RestoreTextGOPMode (currenttextModeCols, currenttextModeRows, currentGOPMode);

		if( gPostStatus <= TSE_POST_STATUS_ENTERING_TSE )
			RestoreGraphicsScreen();
		gST->ConOut->SetAttribute( gST->ConOut, Attribute );
	}
	
DONE:
	if(KeyTimer)
	{
		InstallKeyHandlers();
	}		
	if(ClickTimer)
	{
		InstallClickHandlers();
	}	
	
	if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN) ) {
		gBS->RaiseTPL (TPL_HIGH_LEVEL);
		gBS->RestoreTPL (OldTpl);
	}
	return Status;
}

/**
    Function for the quit booting display.

    @param Message INTN CoOrdX, INTN CoOrdY,
    @param Attribute EFI_UGA_PIXEL Foreground,
    @param Background 

    @retval Status

**/
EFI_STATUS
EFIAPI PostManagerDisplayQuietBootMessage(
    CHAR16 *Message,
    INTN CoOrdX,
    INTN CoOrdY,
    CO_ORD_ATTRIBUTE Attribute,
    EFI_UGA_PIXEL Foreground,
    EFI_UGA_PIXEL Background
    )
{
#ifdef STANDALONE_APPLICATION
	return EFI_UNSUPPORTED;
#elif defined TSE_FOR_APTIO_4_50
    UINTN BltGlyphWidth=0;
    EFI_UGA_PIXEL *BltBuffer=NULL;
    UINTN BltIndex;
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN SizeOfX, SizeOfY;
    EFI_TPL OldTpl = TPL_APPLICATION;
	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

    UpdateGoPUgaDraw();
    if(EFI_SUCCESS != HiiInitializeProtocol())
        return EFI_UNSUPPORTED;

    if(
        (EFI_SUCCESS != GetScreenResolution(&SizeOfX, &SizeOfY))||
        (!gQuietBoot)
        )
        return EFI_UNSUPPORTED;

    if ((Message != NULL) && (EfiStrLen(Message) == 0)) 
		return EFI_SUCCESS;
    
    OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (TPL_APPLICATION);
    
    BltIndex = 0;

	Status = HiiString2BltBuffer(Message,Foreground,Background,&BltIndex,&BltBuffer,&BltGlyphWidth);
	gDrawQuietBootMessage = TRUE;
    if(GifImageFlag) {
    	gGifSrcX = 0; 
    	gGifSrcY = 0;
    }
	if(Status == EFI_SUCCESS){
	    if(IsTSEGopNotificationSupport())
		{
	    	SaveBltBuffer(BltBuffer, Attribute, BltIndex, HiiGetGlyphHeight(), CoOrdX, CoOrdY, BltGlyphWidth);
	    }
	    DrawBltBuffer(BltBuffer, Attribute, BltIndex, HiiGetGlyphHeight(), CoOrdX, CoOrdY, BltGlyphWidth);
	}
	gDrawQuietBootMessage = FALSE;
	
	gBS->RaiseTPL (TPL_HIGH_LEVEL);
	gBS->RestoreTPL (OldTpl);
	
    //Free BltBuffer
    MemFreePointer((VOID **)&BltBuffer);

    if (IsSetupPrintEvalMessage())
    	InvalidateStatusInBgrtWrapper();
    return Status;
#else
// For EDK Nt32 support.
//    Status = DrawHiiStringBltBuffer(Message, CoOrdX, CoOrdY, Attribute, Foreground, Background);
	return EFI_UNSUPPORTED;
#endif
}

/**
    Function returns current Post status.

    @param VOID

        One of the TSE_POST_STATUS
    @retval TSE_POST_STATUS_BEFORE_POST_SCREEN TSE Loaded and yet goto post screen
    @retval TSE_POST_STATUS_IN_POST_SCREEN In the post screen
    @retval TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN In the quite boot screen
    @retval TSE_POST_STATUS_IN_BOOT_TIME_OUT Witing for Boot timeout
    @retval TSE_POST_STATUS_ENTERING_TSE Entering TSE
    @retval TSE_POST_STATUS_IN_TSE Inside TSE
    @retval TSE_POST_STATUS_IN_BBS_POPUP Inside BBS Poupup
    @retval TSE_POST_STATUS_PROCEED_TO_BOOT Outside TSE and Booting or in Shell

**/
TSE_POST_STATUS	EFIAPI PostManagerGetPostStatus(VOID)
{
//	Status =	InitEsaTseInterfaces ();
	if ((TRUE == gLaunchOtherSetup) && (NULL != gEsaInterfaceForTSE))
	{
		return gEsaInterfaceForTSE->GetPostStatus ();
	}
	else
	{
		return gPostStatus;
	}
}


/**
    Function to display the Information box

    @param VOID

        One of the TSE_POST_STATUS
    @retval TSE_POST_STATUS_BEFORE_POST_SCREEN TSE Loaded and yet goto post screen
    @retval TSE_POST_STATUS_IN_POST_SCREEN In the post screen
    @retval TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN In the quite boot screen
    @retval TSE_POST_STATUS_IN_BOOT_TIME_OUT Witing for Boot timeout
    @retval TSE_POST_STATUS_ENTERING_TSE Entering TSE
    @retval TSE_POST_STATUS_IN_TSE Inside TSE
    @retval TSE_POST_STATUS_IN_BBS_POPUP Inside BBS Poupup
    @retval TSE_POST_STATUS_PROCEED_TO_BOOT Outside TSE and Booting or in Shell

**/
EFI_STATUS
EFIAPI PostManagerDisplayInfoBox(	IN CHAR16		*InfoBoxTitle,
							IN CHAR16		*InfoString,
							IN UINTN		Timeout,
							OUT EFI_EVENT	*Event
						 )
{
	EFI_STATUS Status = EFI_SUCCESS;
	BOOLEAN ClickTimer = FALSE,KeyTimer = FALSE;
	EFI_TPL OldTpl = TPL_APPLICATION;
	 
	if(Event == NULL)
	    return EFI_INVALID_PARAMETER;
	
	*Event = NULL;
	if (NULL == gST->ConOut)
		return EFI_NOT_READY;
	
	if (gKeyTimer)
	{
		TimerStopTimer( &gKeyTimer );
		KeyTimer  = TRUE;
	}
	if(gClickTimer)
	{
		StopClickEvent();
		ClickTimer = TRUE;
	}
	if(GifImageFlag)
	{
	    StopGIFTimer();
		CleanUpLogo();
	}
	    
	if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN))
	{
	    OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
	    gBS->RestoreTPL (TPL_APPLICATION);
	}
	Status =	InitEsaTseInterfaces ();
	
	if (!EFI_ERROR (Status))
	{
		Status = gEsaInterfaceForTSE->DisplayInfoBox (
				InfoBoxTitle,
				InfoString,
				Timeout,
				Event
				);
	}
	else
	{
		UINTN currenttextModeCols = 0, currenttextModeRows = 0;
		UINT32 currentGOPMode = 0;
		INT32   Attribute = gST->ConOut->Mode->Attribute;
		Status = SaveCurrentTextGOP (&currenttextModeCols, &currenttextModeRows, &currentGOPMode);
		
		if ( EFI_ERROR(Status) )
			goto DONE;

		Status = ShowInfoBox(InfoBoxTitle, InfoString, Timeout, Event);
		
		gST->ConOut->SetAttribute( gST->ConOut, Attribute );
		RestoreTextGOPMode (currenttextModeCols, currenttextModeRows, currentGOPMode);		
	}
	if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN))
	{
	    gBS->RaiseTPL (TPL_HIGH_LEVEL);
	    gBS->RestoreTPL (OldTpl);
	}
DONE : 
	if(KeyTimer)
	{
		InstallKeyHandlers();
	}		
	if(ClickTimer)
	{
		InstallClickHandlers();
	}
	
	return Status;
}


/**
    Function to set color

    @param Attrib Text color 
			 

    @retval EFI_STATUS status - If the function runs correctly, returns
        EFI_SUCCESS, else other EFI defined error values.

**/
EFI_STATUS EFIAPI PostManagerSetAttribute(UINT8 Attrib)
{
	gPostMgrAttribute = Attrib ;
	return EFI_SUCCESS ;
}

/**
    Function to Get color

    @param Attrib Pointer to get Text color 
			 

    @retval EFI_STATUS status - If the function runs correctly, returns
        EFI_SUCCESS, else other EFI defined error values.

**/
EFI_STATUS EFIAPI PostManagerGetAttribute(UINT8 *Attrib)
{
	*Attrib = gPostMgrAttribute ;
	return EFI_SUCCESS ;
}

BOOLEAN   FlushKeysAfterRead(VOID);
/**
    Funtion to read the key in post when ESA present. 
               

    @param pAmiKey The key pressed by user
    @param Timeout Time to wait

    @retval EFI_STATUS status - EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/
EFI_STATUS ReadKeyinPost(AMI_EFI_KEY_DATA *pAmiKey, UINT64 Timeout)
{
#if TSE_USE_AMI_EFI_KEYCODE_PROTOCOL
    EFI_STATUS Status;
    EFI_STATUS StatusFlush;
    EFI_EVENT EventList[2] = { NULL, NULL };
    UINTN Count = 1, Index = 1;
    AMI_EFI_KEY_DATA    KeyFlush;

    AMI_EFIKEYCODE_PROTOCOL *pKeyCodeProtocol;
    Status = gBS->HandleProtocol(gST->ConsoleInHandle, &gAmiEfiKeycodeProtocolGuid, (void*)&pKeyCodeProtocol);
        if (EFI_ERROR(Status)) return Status;

    if ( Timeout )
    {
        EventList[0] = pKeyCodeProtocol->WaitForKeyEx;

        Status = TimerCreateTimer( &EventList[1], NULL, NULL, TimerRelative, Timeout, EFI_TPL_CALLBACK );
        if ( EFI_ERROR( Status ) )
            return Status;

        Count++;

        gBS->WaitForEvent( Count, EventList, &Index );
    
        TimerStopTimer( &EventList[1] );

        if ( Index == 1 )
            return EFI_NOT_READY;
    }

    Status = pKeyCodeProtocol->ReadEfikey( pKeyCodeProtocol, pAmiKey );
    if (!IsPartialKeySupport())
    {
        //// If it Partial Key make the Status as Error to ignore 
        // the Partial Key.
        if((pAmiKey->KeyState.KeyToggleState & KEY_STATE_EXPOSED) ==  KEY_STATE_EXPOSED) {
            if( (!EFI_ERROR( Status )) && (pAmiKey->Key.ScanCode==0) && (pAmiKey->Key.UnicodeChar==0)) {
                Status = EFI_NOT_READY;
            }
        }
    }

    if ( ! EFI_ERROR( Status ) && FlushKeysAfterRead())//check for the FLUSH_KEYS_AFTER_READ token before flushing the keys
    {
        do
        {
            StatusFlush = pKeyCodeProtocol->ReadEfikey( pKeyCodeProtocol, &KeyFlush );
            //// If it Partial Key make the Status as Error to ignore 
            // the Partial Key.
            if((KeyFlush.KeyState.KeyToggleState & KEY_STATE_EXPOSED) ==  KEY_STATE_EXPOSED) {
                if( (!EFI_ERROR( Status )) && (KeyFlush.Key.ScanCode==0) && (KeyFlush.Key.UnicodeChar==0)) {
                    StatusFlush = EFI_NOT_READY;
                }
            }
        } while ( ! EFI_ERROR( StatusFlush ) );

    }

    return Status;
#else
    EFI_STATUS Status;
    EFI_STATUS StatusFlush;
    EFI_EVENT EventList[2] = { NULL, NULL };
    UINTN Count = 1, Index = 1;
    AMI_EFI_KEY_DATA    KeyFlush;

    if ( Timeout )
    {
        EventList[0] = gST->ConIn->WaitForKey;

        Status = TimerCreateTimer( &EventList[1], NULL, NULL, TimerRelative, Timeout, EFI_TPL_CALLBACK );
        if ( EFI_ERROR( Status ) )
            return Status;

        Count++;

        gBS->WaitForEvent( Count, EventList, &Index );
    
        TimerStopTimer( &EventList[1] );

        if ( Index == 1 )
            return EFI_NOT_READY;
    }

      Status = gST->ConIn->ReadKeyStroke( gST->ConIn, (EFI_INPUT_KEY*)pAmiKey);

    if ( ! EFI_ERROR( Status ) && FlushKeysAfterRead())//check for the FLUSH_KEYS_AFTER_READ token before flushing the keys
    {
        do
        {
            StatusFlush = gST->ConIn->ReadKeyStroke( gST->ConIn, &KeyFlush.Key );
        } while ( ! EFI_ERROR( StatusFlush ) );

    }

    return Status;
    
#endif
}

/**
    Funtion to handle DisplayMsgBoxEx without popup. 
               

    @param Message String to be displayed by the Message Box
    @param MsgBoxExCatagory The category of the message box
    @param HotKeyList The Hot key list to validate the user pressed key
    @param HotKeyListCount Number of keys available in HotKeyList
    @param OutKey Returning the key if the user pressed key matches with HotKeyList
        

    @retval EFI_STATUS status - EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/

EFI_STATUS HandleNoPopupPostMsg (
    IN CHAR16           *Message,
    IN MSGBOX_EX_CATAGORY   MsgBoxExCatagory,
    IN AMI_POST_MGR_KEY     *HotKeyList,        // NULL - AnyKeyPress closes
    IN UINT16           HotKeyListCount, 
    OUT AMI_POST_MGR_KEY    *OutKey
)
{
    AMI_EFI_KEY_DATA AmiKey;
    UINT16 i = 0;  
    
    if(Message == NULL || ((MsgBoxExCatagory == MSGBOX_EX_CATAGORY_NO_POPUP) && ((!HotKeyListCount) || !(HotKeyList))))
            return EFI_INVALID_PARAMETER;
    
    if(gPostStatus != TSE_POST_STATUS_IN_POST_SCREEN)
            return EFI_NOT_READY;

    PrintPostMessage(Message, TRUE);
    
    while (1)
    {
        MemSet(&AmiKey, sizeof(AMI_EFI_KEY_DATA), 0);   //makesure AmiKey does not have any garbage data
        if ( ReadKeyinPost(&AmiKey, TIMER_ONE_SECOND / 20))
        {
            continue;
        }

        if(HotKeyList!=NULL && HotKeyListCount>0)
        {
            for(i=0 ; i<HotKeyListCount; i++)
            {
                if( (HotKeyList[i].Key.ScanCode == AmiKey.Key.ScanCode ) &&
                    (HotKeyList[i].Key.UnicodeChar == AmiKey.Key.UnicodeChar) &&
                    (TseCheckShiftState( AmiKey, HotKeyList[i].KeyShiftState)  ) )
                {
                    // Verify passed argument is not NULL before attempting to assign to it
                    if(OutKey != NULL)
                    {
                        OutKey->Key.ScanCode = AmiKey.Key.ScanCode ; 
                        OutKey->Key.UnicodeChar = AmiKey.Key.UnicodeChar; 
                    
#if TSE_USE_AMI_EFI_KEYCODE_PROTOCOL
                        OutKey->KeyShiftState = (UINT32)AmiKey.KeyState.KeyShiftState ;
#else
                        OutKey->KeyShiftState = 0 ;
#endif
                    }
                    return EFI_SUCCESS;
                }
            }
        }
    }
}

/**
    Wrapper function for Display OEM Message box. 
               

    @param MsgBoxTitle Caption of the Message Box
    @param Message String to be displayed by the Message Box
    @param Legend Legend String to be displayed by the Message Box
    @param MsgBoxType Message Box type

    @retval EFI_STATUS status - EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/
EFI_STATUS EFIAPI PostManagerDisplayMsgBoxEx (
    IN CHAR16			*Title,
    IN CHAR16			*Message,
    IN CHAR16			*Legend,
    IN MSGBOX_EX_CATAGORY	MsgBoxExCatagory,
    IN UINT8	 		MsgBoxType,
    IN UINT16			*OptionPtrTokens,	// Valid only with MSGBOX_TYPE_CUSTOM 
    IN UINT16	 		OptionCount,		// Valid only with MSGBOX_TYPE_CUSTOM
    IN AMI_POST_MGR_KEY		*HotKeyList, 		// NULL - AnyKeyPress closes
    IN UINT16 			HotKeyListCount, 
    OUT UINT8			*MsgBoxSel,
    OUT AMI_POST_MGR_KEY	*OutKey
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	BOOLEAN ClickTimer = FALSE,KeyTimer = FALSE;
    EFI_TPL OldTpl = TPL_APPLICATION;
    
	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

    if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN) ) {
        OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
        gBS->RestoreTPL (TPL_APPLICATION);
    }
    
	if (gKeyTimer)
	{
		TimerStopTimer( &gKeyTimer );
		KeyTimer  = TRUE;
	}
	if(gClickTimer)
	{
		StopClickEvent();
		ClickTimer = TRUE;
	}
	if(GifImageFlag)
	{
	    StopGIFTimer();
		CleanUpLogo();
	}

	if (MsgBoxExCatagory == MSGBOX_EX_CATAGORY_NO_POPUP)
	{
	    Status = HandleNoPopupPostMsg(Message, MsgBoxExCatagory, HotKeyList, HotKeyListCount, OutKey);
	    goto DONE;
	}
	
	Status =	InitEsaTseInterfaces ();
	
	if (!EFI_ERROR (Status))
	{
		Status = gEsaInterfaceForTSE->DisplayMsgBoxEx (
			    Title,
			    Message,
			    Legend,
			    MsgBoxExCatagory,
			    MsgBoxType,
			    OptionPtrTokens,	// Valid only with MSGBOX_TYPE_CUSTOM 
			    OptionCount,		// Valid only with MSGBOX_TYPE_CUSTOM
			    HotKeyList, 		// NULL - AnyKeyPress closes
			    HotKeyListCount, 
			    MsgBoxSel,
			    OutKey
				);
	}
	else
	{
		UINTN currenttextModeCols = 0, currenttextModeRows = 0;
		UINT32 currentGOPMode = 0;
		INT32   Attribute = gST->ConOut->Mode->Attribute;
		Status = SaveCurrentTextGOP (&currenttextModeCols, &currenttextModeRows, &currentGOPMode);

		if ( EFI_ERROR(Status) )
			goto DONE;

		Status = ShowPostMsgBoxEx (Title, Message, Legend, MsgBoxExCatagory,
											MsgBoxType, OptionPtrTokens, OptionCount,
											HotKeyList, HotKeyListCount, MsgBoxSel, OutKey
											);
		gST->ConOut->SetAttribute( gST->ConOut, Attribute );
		RestoreTextGOPMode (currenttextModeCols, currenttextModeRows, currentGOPMode);

		
	}
DONE:
	if(KeyTimer)
	{
		InstallKeyHandlers();
	}		
	if(ClickTimer)
	{
		InstallClickHandlers();
	}
	if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN) ) {
        gBS->RaiseTPL (TPL_HIGH_LEVEL);
        gBS->RestoreTPL (OldTpl);
    }
	return Status;
}
/**
    Wrapper function for Display OEM Message box. 
               

    @param Percent Percent Complete

    @retval EFI_STATUS status - EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/
EFI_STATUS EFIAPI PostManagerDisplayProgress (
    IN UINT8	ProgressBoxState, 
    IN CHAR16	*Title,
    IN CHAR16	*Message,
    IN CHAR16	*Legend,
    IN UINTN 	Percent,	// 0 - 100
    IN OUT VOID	**Handle,	//Out HANDLE. Valid Handle for update and close
    OUT AMI_POST_MGR_KEY	*OutKey	//Out Key 
)
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    BOOLEAN ClickTimer = FALSE,KeyTimer = FALSE;

    if (NULL == gST->ConOut)
        return EFI_NOT_READY;

    if (gKeyTimer)
    {
        TimerStopTimer( &gKeyTimer );
        KeyTimer  = TRUE;
    }
    if(gClickTimer)
    {
        StopClickEvent();
        ClickTimer = TRUE;
    }

    if(GifImageFlag)
    {
         StopGIFTimer();
		CleanUpLogo();
    }

    Status =	InitEsaTseInterfaces ();

    if (!EFI_ERROR (Status))
    {
        Status = gEsaInterfaceForTSE->DisplayProgress (
                ProgressBoxState, 
                Title,
                Message,
                Legend,
                Percent,
                Handle,
                OutKey
        );
    }
    else
    {
        INT32   Attribute = gST->ConOut->Mode->Attribute;
        Status =  ShowPostProgress( ProgressBoxState, Title, Message, Legend, Percent, Handle, OutKey) ;	
        gST->ConOut->SetAttribute( gST->ConOut, Attribute );
    }
 

    if(KeyTimer)
    {
        InstallKeyHandlers();
    }		
    if(ClickTimer)
    {
        InstallClickHandlers();
    }	
    return Status ;
}
/**
    Wrapper function for Display Menu. 
               

    @param HiiHandle - Handle that contains String
    @param TitleToken - Menu Title Token
    @param LegendToken - Menu Legend Token  
    @param MenuData - Menu Items to be displayed 
    @param MenuCount - Number of Menu items
    @param pSelection Menu selection (DEFAULT)

    @retval EFI_STATUS status 		- EFI_SUCCESS if OK,
        EFI_INVALID_PARAMETER if NULL pointer

**/
EFI_STATUS EFIAPI PostManagerDisplayMenu (
    IN VOID	*HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN UINT16 	LegendToken,	  
    IN POSTMENU_TEMPLATE *MenuData,
    IN UINT16 	MenuCount,
    OUT UINT16  *pSelection
)
{
	EFI_STATUS 	Status = EFI_SUCCESS;
	UINTN SizeOfX = 0;
	UINTN SizeOfY = 0; 
	EFI_UGA_PIXEL *UgaBlt = NULL;

	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

	if(gPostStatus < TSE_POST_STATUS_IN_BOOT_TIME_OUT)
		return EFI_NOT_READY;
	
	Status =	InitEsaTseInterfaces ();
	
	if (!EFI_ERROR (Status))
	{
		//Set the tse data to esa
		/*Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (TSEHANDLE, 1, &gHiiHandle, 0);
		Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (BOOTDATA,1, &gBootData, &gBootOptionCount);
		Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (LANGDATA,1, &gLanguages, &gLangCount);
		Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (GOP, 1, &gGOP, 0);
		Status = gEsaInterfaceForTSE->EsaTseSetGetVariables (PostStatus, 1, 0, &gPostStatus);*/
//		gEsaInterfaceForTSE->InitGraphicsLibEntry (gImageHandle, gST);
		
		return gEsaInterfaceForTSE->DisplayPostMenu (
					HiiHandle, 	
					TitleToken, 
					LegendToken,	  
					MenuData,
					MenuCount,
					pSelection
				);
	}
	else
	{
		UINTN currenttextModeCols = 0, currenttextModeRows = 0;
		UINT32 currentGOPMode = 0;
		INT32   Attribute = gST->ConOut->Mode->Attribute;
		 if (TSE_POST_STATUS_IN_POST_SCREEN == gPostStatus||TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN == gPostStatus)
		 {
			 UgaBlt = SavePostScreen(&SizeOfX, &SizeOfY);
		 }
		Status = SaveCurrentTextGOP (&currenttextModeCols, &currenttextModeRows, &currentGOPMode);

		if ( EFI_ERROR(Status) )
			return Status;
		
		Status = ShowPostMenu(HiiHandle, TitleToken, LegendToken, MenuData, MenuCount, pSelection);
		gST->ConOut->SetAttribute( gST->ConOut, Attribute );
		RestoreTextGOPMode (currenttextModeCols, currenttextModeRows, currentGOPMode);
		 if (TSE_POST_STATUS_IN_POST_SCREEN == gPostStatus||TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN == gPostStatus)
		 {
			 RestorePostScreen(UgaBlt,SizeOfX,SizeOfY);
		 }

		return Status;

	}
}

/**
    Function to display text entry interface
 

    @param HiiHandle 
    @param TitleToken 
    @param InputData 
    @param ItemCount 
    @param ValidateKeyFunc 

    @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI PostManagerDisplayTextBox (
    IN VOID	    *HiiHandle, 	
    IN UINT16 	TitleToken, 
    IN TEXT_INPUT_TEMPLATE *InputData,
    IN UINT16 	ItemCount,
    IN DISPLAY_TEXT_KEY_VALIDATE ValidateKeyFunc
	)
{
	EFI_STATUS Status = EFI_UNSUPPORTED;
	EFI_TPL OldTpl = TPL_APPLICATION;
	BOOLEAN ClickTimer = FALSE,KeyTimer = FALSE;
	
	if (NULL == gST->ConOut)
		return EFI_NOT_READY;

	if (gKeyTimer)
	{
		TimerStopTimer( &gKeyTimer );
		KeyTimer  = TRUE;
	}
	if(gClickTimer)
	{
		StopClickEvent();
		ClickTimer = TRUE;
	}
	if(GifImageFlag)
	{
	    StopGIFTimer();
		CleanUpLogo();
	}

	Status =	InitEsaTseInterfaces ();
	
	
	
	if (!EFI_ERROR (Status))
	{
		Status = gEsaInterfaceForTSE->DisplayTextBox (
				HiiHandle, 	
			    TitleToken, 
			    InputData,
			    ItemCount,
			    ValidateKeyFunc
				);
		goto DONE;
	}
	else
	{
		UINTN currenttextModeCols = 0, currenttextModeRows = 0;
		UINT32 currentGOPMode = 0;
		if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN)) {
				OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
				gBS->RestoreTPL (TPL_APPLICATION);
			}
		Status = SaveCurrentTextGOP (&currenttextModeCols, &currenttextModeRows, &currentGOPMode);

		if ( EFI_ERROR(Status) )
		{
			if (gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN || gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) {
			gBS->RaiseTPL (TPL_HIGH_LEVEL);
			gBS->RestoreTPL (OldTpl);
			}
			goto DONE;
		}

		Status = ShowPostTextBox(HiiHandle, TitleToken, InputData, ItemCount, ValidateKeyFunc);

		RestoreTextGOPMode (currenttextModeCols, currenttextModeRows, currentGOPMode);
		
		if ((gPostStatus == TSE_POST_STATUS_IN_POST_SCREEN) || (gPostStatus == TSE_POST_STATUS_IN_QUITE_BOOT_SCREEN) || (gPostStatus == TSE_POST_STATUS_BEFORE_POST_SCREEN)) {
				gBS->RaiseTPL (TPL_HIGH_LEVEL);
				gBS->RestoreTPL (OldTpl);
			}
		goto DONE;

	}
	DONE : 
		if(KeyTimer)
		{
			InstallKeyHandlers();
		}		
		if(ClickTimer)
		{
			InstallClickHandlers();
		}
		
		return Status;	
	
}
/**
    Function to Stop the Click Event
               
    @param VOID

    @retval VOID

**/
VOID StopClickEvent(VOID)
{
	if(gClickTimer)
	{
		gBS->SetTimer ( gClickTimer,TimerCancel,0);
		TimerStopTimer( &gClickTimer );
	}
}


/**
    Protocol interface to invalidate BGRT status
 

    @param VOID
 

    @retval EFI_STATUS

**/
VOID InvalidateStatusInBgrtWrapper (VOID);
EFI_STATUS EFIAPI InvalidateBgrtStatusByProtocol (VOID)
{
	InvalidateStatusInBgrtWrapper();
	return EFI_SUCCESS;
}

/**
    Install Invalidate BGRT status protocol

    @param Handle 

    @retval EFI_STATUS
**/
EFI_STATUS InstallInvalBGRTStatusProtocol (EFI_HANDLE Handle)
{
  EFI_STATUS Status = EFI_SUCCESS;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gTSEInvalidateBgrtStatusProtocolGuid,
                  &gInvalidateBgrtStatus,
                  NULL
                  );

  return Status;
}

/**
        Name				:	SaveCurrentTextGOP

        Description		:	Function to Save current TextMode and GOP
               
        Input				:  UINTN currentTextModCols, UINTN currentTextModRows, UINT32 currentGOPMode

        Output			:	EFI_STATUS

**/
EFI_STATUS SaveCurrentTextGOP ( UINTN *currenttextModeCols, UINTN *currenttextModeRows, UINT32 *currentGOPMode )
{

	EFI_STATUS 	Status = EFI_SUCCESS;
	
	if ( IsTseBestTextGOPModeSupported() && TSE_POST_STATUS_IN_TSE != gPostStatus )
	{
		if (gGOP)
		{
			*currentGOPMode = gGOP->Mode->Mode;
		}

		if (TSE_BEST_HORIZONTAL_RESOLUTION != gGOP->Mode->Info->HorizontalResolution || TSE_BEST_VERTICAL_RESOLUTION != gGOP->Mode->Info->VerticalResolution)
		{
			SetScreenResolution(TSE_BEST_HORIZONTAL_RESOLUTION,TSE_BEST_VERTICAL_RESOLUTION); //If any postinterface is invoked during QuietBoot
		}

		Status = gST->ConOut->QueryMode( gST->ConOut, gST->ConOut->Mode->Mode, currenttextModeCols, currenttextModeRows);
		if (EFI_ERROR(Status))
			return EFI_NOT_FOUND;

		if ((STYLE_FULL_MAX_COLS != *currenttextModeCols)  || (STYLE_FULL_MAX_ROWS != *currenttextModeRows))
		{
			gMaxRows = STYLE_FULL_MAX_ROWS;
			gMaxCols = STYLE_FULL_MAX_COLS;
			SetDesiredTextMode ();
		}
	}
	return Status;
}

/**
        Name				:	RestoreTextGOPMode

        Description		:	Function to Save current TextMode and GOP
               
        Input				:  UINTN currentTextModCols, UINTN currentTextModRows, UINT32 currentGOPMode

        Output			:	EFI_STATUS

**/
EFI_STATUS RestoreTextGOPMode ( UINTN prevTextModeCols, UINTN prevTextModeRows, UINT32 prevGOPMode )
{
	EFI_STATUS 	Status = EFI_SUCCESS;

	if ( IsTseBestTextGOPModeSupported() && TSE_POST_STATUS_IN_TSE != gPostStatus )
	{
		if ( (gMaxRows != prevTextModeRows) || (gMaxCols != prevTextModeCols) )
		{
			gMaxRows = prevTextModeRows; 
			gMaxCols = prevTextModeCols;
			SetDesiredTextMode (); //Restoring to previous textmode if any changed
		}

		if (gGOP && prevGOPMode != gGOP->Mode->Mode)
		{
			Status = gGOP->SetMode (gGOP, prevGOPMode);	//In some case changing text mode will change the graphcis mode, so reverting here.
		}
	}
	return Status;
}
/**
        Name				:	ProcessRecovery

        Description		:	Function ProcessRecovery attempt
               
        Input				:  UINT16 *pRecoveryOrder, UINTN size, 
        Input				:  BOOLEAN - TRUE for OsRecovery, FALSE for PlatformRecovery

        Output			:	EFI_STATUS

**/
EFI_STATUS ProcessRecovery(UINT16 *pRecoveryOrder,UINTN size, UINT8 RecoveryBootingType)
{

	UINT16  u16RecoveryCount = 0,i;
	EFI_STATUS Status = EFI_UNSUPPORTED;

	u16RecoveryCount = (UINT16)(size/sizeof(UINT16));
	for ( i = 0; i < u16RecoveryCount; i++)
	{
		 BOOT_DATA *pRecoveryData;	    
		 pRecoveryData = RecoveryGetRecoveryData(pRecoveryOrder[i],RecoveryBootingType);
		 if(pRecoveryData == NULL || !(pRecoveryData->Active & LOAD_OPTION_ACTIVE))
	                continue;
		 Status = BootLaunchBootOption(pRecoveryOrder[i], pRecoveryOrder+i, u16RecoveryCount - i);
		 if (!EFI_ERROR (Status))
			 return Status;
	}
	 
	return Status;
}

/**
        Name			:	ScreenMgmtSaveTseScreen

        Description		:	Save the current post screen
               
        Output			:	EFI_STATUS

**/
EFI_STATUS EFIAPI ScreenMgmtSaveTseScreen ()
{
    EFI_STATUS Status = EFI_SUCCESS;
    if(GifImageFlag)
        StopGIFTimer();
    if (NULL == gEsaInterfaceForTSE)
    {
        ESAInitFvHook ();
        Status = gBS->LocateProtocol(&gEsaInterfacesForTseProtocolGuid, NULL,(void**) &gEsaInterfaceForTSE);
    }
    if(gEsaInterfaceForTSE && (Status == EFI_SUCCESS))
    {

        if(EFI_SUCCESS == GetScreenResolution(&gScreenWidth,&gScreenHeight))
        {
            UINTN UgaBltSize = (gScreenWidth) * (gScreenHeight) * sizeof(EFI_UGA_PIXEL);
            gScreenBuffer = EfiLibAllocatePool(UgaBltSize);
            Status = gGOP->Blt(
                    gGOP,
                    gScreenBuffer,
                    EfiBltVideoToBltBuffer,
                    0, 0,
                    0, 0,
                    gScreenWidth, gScreenHeight,
                    0);
        }
    }
    else
    {
        gScreenBuffer = (EFI_UGA_PIXEL *)SavePostScreen(&gScreenWidth, &gScreenHeight);	//Saving buffer of Quiteboot logo
        gScreenBuf = EfiLibAllocatePool( sizeof(SCREEN_BUFFER) );

        if (gScreenBuf && gActiveBuffer)
        {
            MemCopy( gScreenBuf, gActiveBuffer, sizeof(SCREEN_BUFFER) ); //Saving buffer of PostMessage, which will used to show message normal boot\quite boot
        }

    }

    return Status;
}

/**
        Name			:	ScreenMgmtRestoreTseScreen

        Description		:	Restore the saved post screen.
               
        Output			:	EFI_STATUS

**/
EFI_STATUS EFIAPI ScreenMgmtRestoreTseScreen ()
{
    EFI_STATUS Status = EFI_SUCCESS;

    if (gEsaInterfaceForTSE) 
    {
        if( (NULL != gGOP) && (gScreenWidth && gScreenHeight) && //If it has vaild gGOP and Resolution
                ( (gScreenWidth != gGOP->Mode->Info->HorizontalResolution) || (gScreenHeight != gGOP->Mode->Info->VerticalResolution) ) //If QuietBoot resolution is differ
        )
        {
            Status = GOPSetScreenResolution(&gScreenWidth, &gScreenHeight);
        }
        if (NULL != gGOP && (!EFI_ERROR( Status )))
            Status= gGOP->Blt (
                    gGOP,
                    gScreenBuffer,
                    EfiBltBufferToVideo,
                    0, 0,
                    0, 0,
                    gScreenWidth, gScreenHeight,
                    0);
    }
    else
    {
        if (gActiveBuffer && gScreenBuf)
        {
            MemSet(gActiveBuffer, sizeof(SCREEN_BUFFER), 0);
            MemCopy( gActiveBuffer, gScreenBuf, sizeof(SCREEN_BUFFER) );    //Restoring Post message in active buffer. This will be drawn when DoRealFlushLines happens
        }
        RestorePostScreen(gScreenBuffer,gScreenWidth,gScreenHeight); //Restoring Quiteboot logo

        if (gScreenBuf)
            MemFreePointer((VOID **)&gScreenBuf);   
        Status = EFI_SUCCESS;
    }
    if(GifImageFlag)
        StartGIFTimer();
    return Status;
}

/**
        Name			:	ScreenMgmtClearScreen

        Description		:	Cleans the current screen
               
        Output			:	EFI_STATUS

**/
EFI_STATUS EFIAPI ScreenMgmtClearScreen (
		IN UINT8 Attribute
		)
{
    EFI_STATUS Status = EFI_SUCCESS;
    if(GifImageFlag)
        StopGIFTimer();
    if (NULL == gEsaInterfaceForTSE)
    {
        ESAInitFvHook ();
        Status = gBS->LocateProtocol(&gEsaInterfacesForTseProtocolGuid, NULL,(void**) &gEsaInterfaceForTSE);
    }
    if((Status == EFI_SUCCESS) && gEsaInterfaceForTSE)
    {
        EFI_UGA_PIXEL UgaBlt={0,0,0,0};
        Status = gGOP->Blt (
                gGOP,
                &UgaBlt,
                EfiBltVideoFill,
                0, 0,
                0, 0,
                gScreenWidth, gScreenHeight,
                0);
    }
    else
    {

        ClearGrphxScreen ();//To clear screen while drawing logo at (0,0) co-ordinates
        ClearScreen( Attribute );
        DoRealFlushLines();
    }
/*    if(GifImageFlag)
        StartGIFTimer();*/
    return Status;

}


/**
    Install Popup Protocol

    @param Handle 

    @retval EFI_STATUS
**/
EFI_STATUS InstallHiiPopupProtocol( EFI_HANDLE Handle)
{
    EFI_STATUS      Status              = EFI_SUCCESS;
    EFI_GUID        PopupProtocolGuid   = EFI_HII_POPUP_PROTOCOL_GUID;
    
    UninstallHiiProtocolProtocol(Handle);
    Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &PopupProtocolGuid,
                  &HiiPopupProtocol,
                  NULL
                  );    
    return Status;
}

/**
    UnInstall Popup Protocol

    @param Handle 

    @retval VOID
**/
VOID UninstallHiiProtocolProtocol(EFI_HANDLE Handle)
{
    EFI_STATUS      status = EFI_SUCCESS;
    EFI_GUID        PopupProtocolGuid = EFI_HII_POPUP_PROTOCOL_GUID;
    EFI_HANDLE      handle;
    VOID            *iface;
    UINTN           size = 0;
    
    pBS = gBS;
    status = gBS->LocateHandle (
                      ByProtocol,
                      &PopupProtocolGuid,
                      NULL,
                      &size,
                      &handle
                      );
    if(status == EFI_BUFFER_TOO_SMALL)
    {
      handle = EfiLibAllocateZeroPool(size);
      if (handle == NULL)
      {
        status = EFI_OUT_OF_RESOURCES;
        goto DONE;
      }

      status = gBS->LocateHandle (
                        ByProtocol,
                        &PopupProtocolGuid,
                        NULL,
                        &size,
                        &handle
                        );
    }

    if(EFI_ERROR(status))
    {
      goto DONE;
    }
    status = gBS->LocateProtocol (
                      &PopupProtocolGuid,
                      NULL,
                      &iface
                      );
    if(EFI_ERROR(status))
    {
      goto DONE;
    }
    status = gBS->UninstallMultipleProtocolInterfaces (
                    handle,
                    &PopupProtocolGuid,
                    iface,
                    NULL
                    );

    if(EFI_ERROR(status))
    {
      goto DONE;
    }

  DONE:
    return;    
}

/**
    The function to Display the popup message box based on the popup style and type provided.
    On return,it will return the the selected options if the userSelection argument is present.

    @param 
    This        - Instance of popup protocol
    PopupStyle  - Message Box Style as per EFI_HII_POPUP_STYLE
    PopupType   - Message box type to display as per EFI_HII_POPUP_TYPE
    HiiHandle   - handle to get the String form Hii database.
    Message     - String Token ID of the message to be displayed.
    UserSelection - Returns the slected options(Optional)
    @retval EFI_STATUS 
**/
EFI_STATUS
EFIAPI
CreatePopup (
  IN  EFI_HII_POPUP_PROTOCOL    *This,
  IN  EFI_HII_POPUP_STYLE       PopupStyle,
  IN  EFI_HII_POPUP_TYPE        PopupType,
  IN  EFI_HII_HANDLE            HiiHandle,
  IN  EFI_STRING_ID             Message,
  OUT EFI_HII_POPUP_SELECTION   *UserSelection  OPTIONAL
  )
{
    CHAR16                  *MessageString = NULL,*Title = NULL;
    UINT16                  SelectionOptionToken[4] = {0};
    UINT16                  OptionCount;
    UINT8                   SelOption = 0;
    EFI_STATUS              Status = EFI_UNSUPPORTED;
    MSGBOX_EX_CATAGORY      MessageBoxCatagoty = 0;
    
    if((PopupType < EfiHiiPopupTypeOk) ||  (PopupType > EfiHiiPopupTypeYesNoCancel)) {
       return EFI_INVALID_PARAMETER; 
    }
    
    if((PopupStyle < EfiHiiPopupStyleInfo) || (PopupStyle > EfiHiiPopupStyleError))  {
        return EFI_INVALID_PARAMETER;
    }
    
    if((HiiHandle == NULL) ||(Message <= 0)) {
        return EFI_INVALID_PARAMETER;
    }
    
    MessageString = HiiGetString(HiiHandle,Message);
    if(MessageString == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    //Set the message box type and title
    if(EfiHiiPopupStyleInfo == PopupStyle) {
        Title = L"INFO";
        MessageBoxCatagoty = MSGBOX_EX_CATAGORY_NORMAL;
    }
    else if(EfiHiiPopupStyleWarning == PopupStyle) {
        Title = L"WARNING";
        MessageBoxCatagoty = MSGBOX_EX_CATAGORY_WARNING;
    }
    else  { //Error Style
        Title = L"ERROR";
        MessageBoxCatagoty = MSGBOX_EX_CATAGORY_ERROR;
    }
    
    switch(PopupType) //Create the Selected options based on PopupType
    {
    case EfiHiiPopupTypeOkCancel:
        SelectionOptionToken[0] = STR_CTRL_OK;
        SelectionOptionToken[1] = STR_CTRL_CANCEL; 
        OptionCount = 2;        
        break;
    case EfiHiiPopupTypeYesNo:
        SelectionOptionToken[0] = STR_CTRL_YES;
        SelectionOptionToken[1] = STR_CTRL_NO;   
        OptionCount = 2;
        break;
    case EfiHiiPopupTypeYesNoCancel:
        SelectionOptionToken[0] = STR_CTRL_YES;
        SelectionOptionToken[1] = STR_CTRL_NO; 
        SelectionOptionToken[2] = STR_CTRL_CANCEL; 
        OptionCount = 3;
        break;
    case EfiHiiPopupTypeOk:
    default:
        SelectionOptionToken[0] = STR_CTRL_OK; 
        OptionCount = 1;
        break;
    }
    Status = PostManagerDisplayMsgBoxEx(
                    Title,
                    MessageString,
                    NULL,
                    MessageBoxCatagoty,
                    MSGBOX_TYPE_HII_POP,
                    SelectionOptionToken,
                    OptionCount,
                    NULL,
                    0,
                    &SelOption,
                    NULL
                   );
    if(!(EFI_ERROR(Status)) && (UserSelection != NULL))
    {
        switch(PopupType) //Return the Selection value based on Type
        {
        case EfiHiiPopupTypeOkCancel:
            if(SelOption == 0)
                *UserSelection = EfiHiiPopupSelectionOk;             
            else 
                *UserSelection = EfiHiiPopupSelectionCancel;       
            break;
        case EfiHiiPopupTypeYesNo:
            if(SelOption == 0)
                *UserSelection = EfiHiiPopupSelectionYes;             
            else
                *UserSelection = EfiHiiPopupSelectionNo; 
            break;
        case EfiHiiPopupTypeYesNoCancel:
            if(SelOption == 0)
                *UserSelection = EfiHiiPopupSelectionYes;             
            else if(SelOption == 1)
                *UserSelection = EfiHiiPopupSelectionNo; 
            else
                *UserSelection = EfiHiiPopupSelectionCancel;  
            break;
        case EfiHiiPopupTypeOk:
        default:
            *UserSelection = EfiHiiPopupSelectionOk;   
            break;        
        }
    }
    if(MessageString)
        MemFreePointer((VOID **)&MessageString);
    return Status;  
}
