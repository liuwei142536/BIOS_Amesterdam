//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file callback.c
    This file contains code to handle callbacks

**/

#include "minisetup.h"

ACTION_DATA *gMsgBoxAction;

UINTN _CallbackGetValue( MSGBOX_DATA *msgbox );
VOID _CallbackMsgbox( VOID *container, VOID *object, VOID *cookie );
VOID _LoadDefaults( NVRAM_VARIABLE *defaults, UINTN data );
BOOLEAN _SaveValues( UINTN data );
BOOLEAN LoadDefaultsFromDefaultBuffer( VOID );
EFI_STATUS DoReConnectPageHandles(void);
extern UINTN gPreviousMsgBox;
extern UINTN gFailsafeMsgBox;
extern UINTN gOptimalMsgBox;
extern UINTN gSaveMsgBox;
extern UINTN gSaveExitMsgBox;
extern UINTN gExitMsgBox;
extern UINTN gSaveResetMsgBox;
extern UINTN gResetMsgBox;
extern UINTN gHelpMsgBox;
extern UINTN gSaveUserMsgBox;
extern UINTN gLoadUserMsgBox;
extern BOOLEAN gBrowserCallbackEnabled;
extern APPLICATION_DATA	*gApp;
extern BOOLEAN gTseCacheUpdated;
extern BOOLEAN IsTseUefi26FeaturesSupport(VOID);
#if TSE_DEBUG_MESSAGES
extern VOID DebugShowPrintPageDetails(UINTN Start, UINTN End);
#endif
VOID HandleReadOpOnRetrieve();
/**
    Callback function for LoadPreviousValue.

    @param Bool ShowMsgBox.

    @retval VOID

**/
VOID LoadPreviousValues( BOOLEAN showMsgBox )
{
	UINT8 result = MSGBOX_YES;
	UINT16 CurrentBootOption=0;

//- Fix for Screen corruption while editing Date/edit controls.
	FRAME_DATA *MainFrame=NULL;
//-

	if ( showMsgBox )
		result = CallbackShowMessageBox( (UINTN)gPreviousMsgBox, MSGBOX_TYPE_YESNO );

	if ( result == MSGBOX_YES )
	{
		if(gCurrLegacyBootData)
		{
			UINT16 CurLegBootDataValid = 0;
			UINT32 i;

			// Check is gCurrLegacyBootData valid one
		    for (i = 0; i < gBootOptionCount; i++ )
		    {
		        if ( &gBootData[i] == gCurrLegacyBootData )
		            CurLegBootDataValid = 1;
		    }
			// If valid
			if(CurLegBootDataValid)
				CurrentBootOption = gCurrLegacyBootData->Option;
			else
				gCurrLegacyBootData = NULL;
		}

		CleanTempNvramVariableList(); // Reset to read the variable from NVRAM
		VarLoadVariables( (VOID **)&gVariableList, NULL );

		// special provisions needed for language
		//VarSetValue( VARIABLE_ID_LANGUAGE, 0, 3, gPrevLanguage );
		VarSetNvram ( VARIABLE_ID_LANGUAGE, gPrevLanguage,gPrevLangSize );

		///Hook to Load previous saved values 
		LoadedPreviousValuesHook();

		PasswordCommitChanges( FALSE );

		TseDiscardAddDelBootOptions();
		TseDiscardAddDelDriverOptions();
		// reinit the gCurrLegacyBootData
		//if(gCurrLegacyBootData)
		//{
			gCurrLegacyBootData = BootGetBootData(CurrentBootOption);
			if(gCurrLegacyBootData) //Avoid crashing when press F3 to load previous values
				VarSetValue(VARIABLE_ID_LEGACY_GROUP_INFO, 0, sizeof(UINT16), &(gCurrLegacyBootData->LegacyDevCount));
		//}

		if ( ! gVariableChanged )
			gResetRequired = FALSE;

		UEFICallSetupFormCallBack(AMI_CALLBACK_RETRIEVE);//Calling the Wraper function with action AMI_CALLBACK_RETRIEVE
		HandleReadOpOnRetrieve();				//Evaluating Read opcode after AMI_CALLBACK_RETRIEVE callback done while load previous values

		if(gApp!=NULL)
		{
	        //Recreate the special controls to reflect changes
	        gApp->CompleteRedraw = TRUE;

	//- Fix for Screen corruption while editing Date/edit controls.
	        MainFrame = gApp->PageList[gApp->CurrentPage]->FrameList[StyleFrameIndexOf(MAIN_FRAME)];
	        if(MainFrame->ControlCount)
	        if(MainFrame->CurrentControl != FRAME_NONE_FOCUSED)
	            if(MainFrame->ControlList[MainFrame->CurrentControl]->ControlActive == TRUE)
	                MainFrame->ControlList[MainFrame->CurrentControl]->ControlActive = FALSE;
	// -    
		}
		gTseCacheUpdated = FALSE;
	}
}
/**
    Loads the optimal defaults.

    @param VOID

    @retval VOID

**/
VOID LoadOptimalDefaults( VOID )
{
	///Load optimal defaults modified as a hook
	LoadSetupDefaultsHook( gOptimalDefaults, (UINTN)gOptimalMsgBox );
}
/**
    Loads the failsafe defaults.

    @param VOID

    @retval VOID

**/
VOID LoadFailsafeDefaults( VOID )
{
	///Load Failsafe defaults modified as a hook
	LoadSetupDefaultsHook( gFailsafeDefaults, (UINTN)gFailsafeMsgBox );
}

BOOLEAN TSEDonotLoadPasswordOnDefaults(VOID);

/**
    Loads the defaults with optimal or Failsafe data.

    @param Bool ShowMsgBox.

    @retval VOID

**/
VOID CsmRearrangeBBSOrder (VOID *nvPtr, VOID *DefaultBuffer);
UINT16 EvaluateControlDefault (CONTROL_INFO *CtrlInfo, UINT64 *Defaults);
extern UINTN gSetupStatus;  

VOID _LoadDefaults( NVRAM_VARIABLE *defaults, UINTN data )
{
	UINT32 i;
	UINT16 CurrentBootOption=0;
	EFI_STATUS Status;
	FRAME_DATA *MainFrame;
	EFI_IFR_OP_HEADER *OpHeader = NULL;
    UINTN BackUpSetupStatus = gSetupStatus;
    VARIABLE_INFO *varInfo = NULL;

#if TSE_DEBUG_MESSAGES
	CHAR16 *String = (CHAR16 *)NULL;
	if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
	    SETUP_DEBUG_VAR("\n[TSE] Entering LoadDefaults() \n");
#endif
	
    gSetupStatus = TSE_SETUP_STATUS_BLOCK_PACKUPDATEPROCESS;
	if ( data == (UINTN)NULL ?1:(CallbackShowMessageBox( data, MSGBOX_TYPE_YESNO ) == MSGBOX_YES) ) //To avoid draw MessageBox when data is passed as NULL to load Defaults silently
	{
	    UINT32	page, control;
	    gBrowserCallbackEnabled = TRUE;
	    if(gCurrLegacyBootData)
	    {
	        UINT16 CurLegBootDataValid = 0;

	        // Check is gCurrLegacyBootData valid one
	        for ( i = 0; i < gBootOptionCount; i++ )
	        {
	            if ( &gBootData[i] == gCurrLegacyBootData )
	                CurLegBootDataValid = 1;
	        }
	        // If valid
	        if(CurLegBootDataValid)
	            CurrentBootOption = gCurrLegacyBootData->Option;
	        else
	            gCurrLegacyBootData = NULL;
	    }

	

		for ( page = 0; page < gPages->PageCount; page++ )
		{
			PAGE_INFO *pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[page]);
			UINTN Action;
		
			if (0 == pageInfo->PageHandle)
				continue;
		
#if TSE_DEBUG_MESSAGES
			if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
			    DebugShowPrintPageDetails(page,page+1);
#endif

			for ( control= 0; control < pageInfo->PageControls.ControlCount; control++ )
			{
				CONTROL_INFO *controlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + pageInfo->PageControls.ControlList[control]);
				
				if( NULL == controlInfo || NULL == controlInfo->ControlPtr || !controlInfo->ControlFlags.ControlVisible)		//Suppressing inconsistent and nosubmit controls so added check for ControlPtr
					continue;	

#if TSE_DEBUG_MESSAGES
				if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
				{
				    String = HiiGetString(controlInfo->ControlHandle, controlInfo->QuestionId);
				    if (String)
				    {
				        SETUP_DEBUG_VAR("[TSE] Control Name: %s ControlHandle: 0x%x Control type: 0x%x\n",String,controlInfo->ControlHandle,controlInfo->ControlType);
				        MemFreePointer((VOID**)&String); 
				    }
				}
#endif	 
				
				Status = EFI_SUCCESS;
				if (UefiIsInteractive (controlInfo))
				{
					Action = UefiGetActionWapper (AMI_CALLBACK_FORM_DEFAULT_STANDARD);
					Status = CallFormCallBack (controlInfo, UefiGetControlKey (controlInfo), FALSE, Action);
#if TSE_DEBUG_MESSAGES
					if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
					{
					    if(Status == EFI_SUCCESS)
					        SETUP_DEBUG_VAR("[TSE] Default value loaded from driver callback\n\n");
					}
#endif
				}
				if (
						(!UefiIsInteractive (controlInfo) || (EFI_UNSUPPORTED == Status)) &&
						(!controlInfo->ControlFlags.ControlEvaluateDefault) &&
						(defaults[controlInfo->ControlVariable].Size > 0)
						)
				{
					UINT32 offset = 0, size = 0;
					
					if ((!LoadDefaultsFromDefaultBuffer ()) && (controlInfo->ControlDataWidth ==  0))		//If defaults not present for a control but if the offset present
					{																						//in the variable which has atleast default then
						continue;																			//try to load it if TSE_LOAD_DEFAULTS_FROM_DEFAULTS_BUFFER
					}																						//is enabled.	
					if (CONTROL_TYPE_ORDERED_LIST != controlInfo->ControlType)
					{
						size = UefiGetWidth(controlInfo->ControlPtr);
					}
					else
					{
					    OpHeader = (EFI_IFR_OP_HEADER*)controlInfo->ControlPtr;
                        while(OpHeader->OpCode != EFI_IFR_ONE_OF_OPTION_OP && OpHeader->OpCode != EFI_IFR_END_OP)
                        {
                            OpHeader = (EFI_IFR_OP_HEADER*)((UINT8*)OpHeader + OpHeader->Length);
                        }

                        if (OpHeader->OpCode == EFI_IFR_ONE_OF_OPTION_OP)
                            size = ((EFI_IFR_ORDERED_LIST *)(controlInfo->ControlPtr))->MaxContainers * ((UINT8) (1 << (((EFI_IFR_ONE_OF_OPTION *)OpHeader)->Type & EFI_IFR_NUMERIC_SIZE)));
					}
					if (0 == size)			//For text and submenu skip loading defaults
					{
						continue;
					}
					offset = UefiGetQuestionOffset(controlInfo->ControlPtr);
					varInfo = VarGetVariableInfoIndex( controlInfo->ControlVariable );
					if ((VARIABLE_ATTRIBUTE_NAMEVALUE == (varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE)))
					{
					    offset = 0;  //offset is always 0 for name/value variables
					}
                    if (0xFFFF == offset) //QF_DATE_STORAGE_TIME and QF_TIME_STORAGE_TIME offsets will be 0xFFFF so we need to skip it
                    {
                        continue;
                    }
                    if (  !(controlInfo->ControlFlags.BitFieldVarstore) && ((offset + size) > defaults [controlInfo->ControlVariable].Size) )
                    {
                        continue;
                    }          					
					if(defaults[controlInfo->ControlVariable].Buffer != NULL && gVariableList [controlInfo->ControlVariable].Buffer == NULL)
					{
						// On loading previous value, if the variable not present in NVRAM gVariableList buffer of that variable will become NULL.
						// So before loading default for that variables, allocate memory before accessing it..  
						gVariableList [controlInfo->ControlVariable].Buffer = (UINT8*)EfiLibAllocateZeroPool(defaults[controlInfo->ControlVariable].Size);
						gVariableList [controlInfo->ControlVariable].Size = defaults[controlInfo->ControlVariable].Size;
#if TSE_DEBUG_MESSAGES
						if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
						{
						    if(varInfo)
						        SETUP_DEBUG_VAR("[TSE] Variable %s is not present in the NVRAM gVariableList buffer. So allocated memory with default buffer size(%d) before accessing it\n",varInfo->VariableName,defaults[controlInfo->ControlVariable].Size);
						}
#endif
					}
					
					if ( ( controlInfo->ControlVariable == VARIABLE_ID_LANGUAGE) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_DEL_BOOT_OPTION) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_ADD_BOOT_OPTION) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_USER_DEFAULTS) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_IDE_SECURITY) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_LEGACY_DEV_INFO) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_LEGACY_GROUP_INFO) ||
					 ( controlInfo->ControlVariable == VARIABLE_ID_ADD_DRIVER_OPTION) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DEL_DRIVER_OPTION) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DYNAMIC_PAGE_COUNT) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DYNAMIC_PAGE_GROUP) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DYNAMIC_PAGE_DEVICE) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DYNAMIC_PAGE_GROUP_CLASS) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DRV_HLTH_ENB) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DRV_HLTH_COUNT) ||
					 (controlInfo->ControlVariable == VARIABLE_ID_DRV_HLTH_CTRL_COUNT) ||
					( (TSEDonotLoadPasswordOnDefaults()) && (controlInfo->ControlVariable == VARIABLE_ID_AMITSESETUP) && (controlInfo->ControlType == CONTROL_TYPE_PASSWORD)) ) // Save pwd to nvram and not loading empty pwd on loading defaults.
					{
#if TSE_DEBUG_MESSAGES
					    if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
					        SETUP_DEBUG_VAR("[TSE] Skipped loading default for this control\n\n");
#endif						
						continue;
					}
					else if(	((defaults[controlInfo->ControlVariable].Buffer != NULL) && (defaults[controlInfo->ControlVariable].Size))
								&&((VARIABLE_ID_BOOT_ORDER == controlInfo->ControlVariable) || (VARIABLE_ID_DRIVER_ORDER == controlInfo->ControlVariable))
							)
					{
						if(defaults[controlInfo->ControlVariable].Size > gVariableList[controlInfo->ControlVariable].Size) // Size may not be sufficient if boot/driver options deleted.
						{
							MemFreePointer((VOID **) &gVariableList[controlInfo->ControlVariable].Buffer);
							gVariableList[controlInfo->ControlVariable].Buffer = EfiLibAllocateZeroPool(defaults[controlInfo->ControlVariable].Size);
						}
						
						MemCopy (gVariableList[controlInfo->ControlVariable].Buffer, defaults[controlInfo->ControlVariable].Buffer, defaults[controlInfo->ControlVariable].Size);
						gVariableList[controlInfo->ControlVariable].Size = defaults[controlInfo->ControlVariable].Size;
					}
					else if (VARIABLE_ID_BBS_ORDER == controlInfo->ControlVariable)      // Since boot order loads first legacydevorder corrupted so restructuring with gbootdata 
					{
						if((defaults[controlInfo->ControlVariable].Buffer != NULL) && (defaults[controlInfo->ControlVariable].Size))
							CsmRearrangeBBSOrder (&(gVariableList [controlInfo->ControlVariable]), &(defaults[controlInfo->ControlVariable]));
					}
					else
					{
					    if((defaults[controlInfo->ControlVariable].Buffer != NULL) && (defaults[controlInfo->ControlVariable].Size))
					    {
					        if(defaults[controlInfo->ControlVariable].Size > gVariableList[controlInfo->ControlVariable].Size) //if default buffer size is more than the gVariableList buffer size then we are allocating sufficient memory and copying default data into gVariableList.
					        {
#if TSE_DEBUG_MESSAGES
					            if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
					            {
					                varInfo = VarGetVariableInfoIndex( controlInfo->ControlVariable );
					                if(varInfo)
					                    SETUP_DEBUG_VAR("[TSE] For Variable %s, default buffer size(%d) is greater then gVariableList buf size(%d). Reallocating\n",
					                            varInfo->VariableName,defaults[controlInfo->ControlVariable].Size,gVariableList[controlInfo->ControlVariable].Size);
					            }
#endif
					            MemFreePointer((VOID **) &gVariableList[controlInfo->ControlVariable].Buffer);
					            gVariableList[controlInfo->ControlVariable].Buffer = EfiLibAllocateZeroPool(defaults[controlInfo->ControlVariable].Size);
					            gVariableList[controlInfo->ControlVariable].Size = defaults[controlInfo->ControlVariable].Size;

					        }
					        if (controlInfo->ControlFlags.BitFieldVarstore) // For bitfield variables copy complete buffer
					        {
					            VOID *ctrlValue = EfiLibAllocateZeroPool (size);
					            _VarGetSetValueForBitField( VAR_COMMAND_GET_VALUE, defaults, controlInfo->ControlVariable, offset, size, ctrlValue, controlInfo->ControlPtr);
					            _VarGetSetValueForBitField( VAR_COMMAND_SET_VALUE, gVariableList, controlInfo->ControlVariable, offset, size, ctrlValue, controlInfo->ControlPtr);
					            SETUP_DEBUG_VAR("[TSE] For variable %s, offset %d, size %d loading default value %d \n",varInfo->VariableName, offset, size, &ctrlValue);
					            MemFreePointer( (VOID **)&ctrlValue );

					        }   
					        else 
					        {
					            MemCopy (gVariableList [controlInfo->ControlVariable].Buffer + offset, defaults[controlInfo->ControlVariable].Buffer + offset, size);
					        }					        
					    }
					}
#if TSE_DEBUG_MESSAGES
					if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
					    SETUP_DEBUG_VAR("[TSE] Default value loaded from optimal buffer\n\n");
#endif
				}
				if(controlInfo->ControlFlags.ControlEvaluateDefault)
				{
				    if(IsDefaultConditionalExpression())
				    {                            
                        UINT64 defaultval = 0 ;
                        UINTN size = 0;
                        UINT32 offset = 0;
                        VOID *defaultStr = NULL;
                        
                        offset = UefiGetQuestionOffset(controlInfo->ControlPtr);
    
                        //Evaluating the Control Default
                        size = EvaluateControlDefault(controlInfo,&defaultval);
                        defaultStr = (VOID*)&defaultval;
                        
                        if ((controlInfo->ControlType == CONTROL_TYPE_POPUP_STRING))
                        {//For String controls Defaults will be string token, so need to get the string for this.
                            defaultStr = (VOID*)HiiGetString( controlInfo->ControlHandle, (UINT16)defaultval);
                            if (NULL == defaultStr)
                                continue;
                            size = (EfiStrLen(defaultStr) + 1)*sizeof(CHAR16);
                        }
    
                        //Updating gVariableList buffer.
                        VarSetValue(controlInfo->ControlVariable, offset, size, defaultStr );
#if TSE_DEBUG_MESSAGES
                        if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
                            SETUP_DEBUG_VAR("[TSE] Default value loaded based on conditional expression\n\n");
#endif						
                        //Updating Manufacturing/FailSafe Defaults buffer.
                        //_VarGetSetValue( VAR_COMMAND_SET_VALUE, gFailsafeDefaults, controlInfo->ControlVariable, ControlVarOffset, size, defaultStr );
    
                        //Updating Optimal Defaults buffer.
                        _VarGetSetValue(VAR_COMMAND_SET_VALUE, gOptimalDefaults, controlInfo->ControlVariable, offset, size, defaultStr);
    
                        if(CONTROL_TYPE_POPUP_STRING == controlInfo->ControlType)
                            MemFreePointer( (VOID **)&defaultStr );
				    }
				    else if(LoadDefaultsFromDefaultBuffer ())
				    {
                        UINT32 offset = 0, size = 0;

                        if (CONTROL_TYPE_ORDERED_LIST != controlInfo->ControlType)
                        {
                            size = UefiGetWidth(controlInfo->ControlPtr);
                        }
                        else
                        {
                            OpHeader = (EFI_IFR_OP_HEADER*)controlInfo->ControlPtr;
                            while(OpHeader->OpCode != EFI_IFR_ONE_OF_OPTION_OP && OpHeader->OpCode != EFI_IFR_END_OP)
                            {
                                OpHeader = (EFI_IFR_OP_HEADER*)((UINT8*)OpHeader + OpHeader->Length);
                            }

                            if (OpHeader->OpCode == EFI_IFR_ONE_OF_OPTION_OP)
                                size = ((EFI_IFR_ORDERED_LIST *)(controlInfo->ControlPtr))->MaxContainers * ((UINT8) (1 << (((EFI_IFR_ONE_OF_OPTION *)OpHeader)->Type & EFI_IFR_NUMERIC_SIZE)));
                        }
                        if (0 == size)          //For text and submenu skip loading defaults
                        {
                            continue;
                        }
                        offset = UefiGetQuestionOffset(controlInfo->ControlPtr);
                        if ((0xFFFF == offset) || ((offset + size) > defaults [controlInfo->ControlVariable].Size))     //QF_DATE_STORAGE_TIME and QF_TIME_STORAGE_TIME offsets will be 0xFFFF so we need to skip it
                        {
                            continue;
                        }                               
                        if((defaults[controlInfo->ControlVariable].Buffer != NULL) && (defaults[controlInfo->ControlVariable].Size))
                            MemCopy (gVariableList [controlInfo->ControlVariable].Buffer + offset, defaults[controlInfo->ControlVariable].Buffer + offset, size);
#if TSE_DEBUG_MESSAGES
                        if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
                            SETUP_DEBUG_VAR("[TSE] Default value loaded from optimal buffer based on LoadDefaultsFromDefaultBuffer()\n\n");
#endif				   
				    }
				}
				if(controlInfo->ControlFlags.ControlRWEvaluate)			
                { 
#if TSE_DEBUG_MESSAGES
				    if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
				    {
				        String = HiiGetString(controlInfo->ControlHandle, controlInfo->QuestionId);
				        if(String)
				        {
				            SETUP_DEBUG_VAR("[TSE] This control has Read/Wirte opcode. So handling read write for this control.\n\n",String);
				            MemFreePointer((VOID**)&String); 
				        }
				    }
#endif
					HandleWriteOpOnChanged(controlInfo);			//Evaluating Write opcode for a current control if it is having Write opcode
				    HandleReadOpOnChanged(controlInfo->ControlPageID);		//Evaluating Read opcode for all the controls in current page
                }
			}
		}
		
		gBrowserCallbackEnabled = FALSE;
		PasswordCommitChanges( FALSE );

		TseDiscardAddDelBootOptions();
		TseDiscardAddDelDriverOptions();							

		// reinit the gCurrLegacyBootData
		//if(gCurrLegacyBootData)                 // If defaults is loaded, boot order defaults is not loading after a restart. So commented.
		//{
			gCurrLegacyBootData = BootGetBootData(CurrentBootOption);
			if(gCurrLegacyBootData) //Avoid crashing when press F3 to load defaults
				VarSetValue(VARIABLE_ID_LEGACY_GROUP_INFO, 0, sizeof(UINT16), &(gCurrLegacyBootData->LegacyDevCount));
		//}

		gResetRequired = TRUE;

        LoadedConfigDefaultsHook();
        
        if(gApp!=NULL)
        {
            //Recreate the special controls to reflect changes
              gApp->CompleteRedraw = TRUE;

      //- Fix for Screen corruption while editing Date/edit controls.
              MainFrame = gApp->PageList[gApp->CurrentPage]->FrameList[StyleFrameIndexOf(MAIN_FRAME)];

          //Load Optimized Defaults(F3)hangs in a empty page || Fix: Check whether the frame has any controls before doing any operation on a control. 
          if(MainFrame->ControlCount)
              if(MainFrame->CurrentControl != FRAME_NONE_FOCUSED)
                  if(MainFrame->ControlList[MainFrame->CurrentControl]->ControlActive == TRUE)
                      MainFrame->ControlList[MainFrame->CurrentControl]->ControlActive = FALSE;
        }
	}
#if TSE_DEBUG_MESSAGES
	if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
	    SETUP_DEBUG_VAR("[TSE] Exiting LoadDefaults()\n\n");
#endif
    gSetupStatus = BackUpSetupStatus;
}
/**
    Function to save and exit

    @param VOID

    @retval BOOLEAN

**/
BOOLEAN SaveAndExit( VOID )
{
	if ( _SaveValues( (UINTN)gSaveExitMsgBox ) )
	{
		gVariableChanged = TRUE;
		return TRUE;
	}

	return FALSE;
}
/**
    Function to save without exit

    @param VOID

    @retval VOID

**/
VOID SaveWithoutExit( VOID )
{
	if ( _SaveValues( (UINTN)gSaveMsgBox ) )
		gVariableChanged = TRUE;
}
/**
    Function to save and reset

    @param VOID

    @retval BOOLEAN

**/
BOOLEAN SaveAndReset( VOID )
{
	if ( _SaveValues( (UINTN)gSaveResetMsgBox ) )
	{
		gVariableChanged = TRUE;
        gResetRequired = TRUE;
		return TRUE;
	}

	return FALSE;
}
/**
    Function to reset

    @param VOID

    @retval BOOLEAN

**/
BOOLEAN ResetSys( VOID )
{
	if ( CallbackShowMessageBox( (UINTN)gResetMsgBox, MSGBOX_TYPE_YESNO ) == MSGBOX_YES )
	{
        	gResetRequired = TRUE; // force a reset 
		gVariableChanged = FALSE;
		return TRUE;
	}

	return FALSE;
}

/**
    Function to save the given values

    @param data 

    @retval TRUE/FALSE

**/
BOOLEAN _SaveValues( UINTN data )
{
	UINT32 i;
	NVRAM_VARIABLE *nvPtr;
	EFI_GUID AmitseNvramUpdateGuid = AMITSE_NVRAM_UPDATE_GUID;
	EFI_GUID AmitseBootOrderChangeGuid = AMITSE_BOOT_ORDER_CHANGE_GUID;

	if (data == (UINTN)NULL ? 1 : (CallbackShowMessageBox (data, MSGBOX_TYPE_YESNO) == MSGBOX_YES) ) //To avoid draw MessageBox when data is passed as NULL to save data silently 
	{
		//Check for NO_SUBMIT_IF before saving data in every page
		if(CheckNoSubmitIf())
		{
			return FALSE; //cant save Invalid Submit value
		}
		if(gApp!=NULL)
		{
	        //Check for inconsistency before saving data
	        if(CheckInconsistence((PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[gApp->CurrentPage])))
	        {
	            return FALSE; //cant save inconsistent value
	        }
		}

		if(gResetRequired)
		{	//Let's update the SaveState Variable 
			UINT32 SaveStateVarNum;
			UINT16 SaveState=SETUP_SAVED_AND_RESET_REQUIRED;
			if(VarGetVariableInfoId( SETUP_SAVE_STATE_INFO_KEY_ID, &SaveStateVarNum ))
				VarSetValue(SaveStateVarNum,0,sizeof(UINT16),&SaveState);
		}

		nvPtr = gVariableList;
		for ( i = 0; i < gVariables->VariableCount; i++, nvPtr++ )
		{
			if(VARIABLE_ID_BOOT_ORDER == i)
			{
                UINTN j,k=0,n=0, CurrSize = 0;
                UINT16 *BootOrder = NULL, *CurrOrder = NULL;
                BOOT_DATA *pBootData = NULL;

				TseSaveAddDelBootOptions();

                BootOrder = EfiLibAllocateZeroPool(nvPtr->Size);
                MemCopy(BootOrder,nvPtr->Buffer,nvPtr->Size);

                //Disable all options
                for(j=0;j<gBootOptionCount;j++)
				{
					if (!(gBootData[j].Active & LOAD_OPTION_HIDDEN))		//TSE will not do the option anything if it is hidden
					{	
	                    gBootData[j].Active &= ~(LOAD_OPTION_ACTIVE);
					}
				}
                //Enable options in BootOrder
                for(j=0;j<gBootOptionCount;j++)
                {
                    pBootData = BootGetBootData(BootOrder[j]);
                    if(pBootData)
					{
						if (!(pBootData->Active & LOAD_OPTION_HIDDEN))		//TSE will not do the option anything if it is hidden	
						{
	                        pBootData->Active |= LOAD_OPTION_ACTIVE;
						}
					}
                }

                //Save boot options
                for(j=0;j<gBootOptionCount;j++)
                {
                    CHAR16 varName[] = L"BootXXXX";
                    BOOT_OPTION *pOption;
                    UINTN OptionSize;

                    OptionSize = 0; pOption = NULL;
                    SPrint( varName, sizeof(CHAR16) * (EfiStrLen( varName )+1),gBootFormarSpecifier, gBootData[j].Option );
                    pOption = (BOOT_OPTION *)VarGetNvramName( varName, &gEfiGlobalVariableGuid, NULL, &OptionSize );
                    if (pOption)
                    {    
                        pOption->Active = gBootData[j].Active;
    
                        VarSetNvramName(varName,
                            &gEfiGlobalVariableGuid,
                            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                            pOption,
                            OptionSize);
                        
                        MemFreePointer((VOID **)&pOption);
                    }
                }
				n=0;
                //Enabled options first
                for(k=0;k<gBootOptionCount;k++)
                {
                    if(DISABLED_BOOT_OPTION == BootOrder[k])
				    {	
			//add Disabled options
	                	for(j=n; j < gBootOptionCount; j++ )
	                	{
	                    		if ( (!(gBootData[j].Active & LOAD_OPTION_ACTIVE)) && (!(gBootData[j].Active & LOAD_OPTION_HIDDEN)) )
	                    		{
									BootOrder[k] = gBootData[j].Option;
									n=j;
									n++;
									break;
	                       		}
	                	}
				    }		
                        
                }

                ASSERT(k==gBootOptionCount);

                //Signal TPM if Boot order has changed
				CurrOrder = VarGetNvramName(L"BootOrder", &gEfiGlobalVariableGuid, NULL, &CurrSize);

                if( CurrSize != (k*sizeof(UINT16)) )
                    EfiLibNamedEventSignal (&AmitseBootOrderChangeGuid);
                else if(CurrOrder)
                {
                    if (MemCmp((UINT8 *)CurrOrder, (UINT8 *)BootOrder, CurrSize))
						EfiLibNamedEventSignal (&AmitseBootOrderChangeGuid);
                }

                MemFreePointer((VOID **)&CurrOrder);

                //Save boot order
                VarSetNvramName(L"BootOrder",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    BootOrder,
                    k * sizeof(UINT16));

                MemFreePointer((VOID **) &BootOrder);

                //Continue to next variable
                continue;
			}
			if(VARIABLE_ID_DRIVER_ORDER == i)
			{
                UINTN 	j, k=0, n=0;
                UINT16 	*DriverOrder = NULL;
                BOOT_DATA *pDriverData = NULL;

				TseSaveAddDelDriverOptions ();

                DriverOrder = EfiLibAllocateZeroPool (nvPtr->Size);
                MemCopy (DriverOrder,nvPtr->Buffer,nvPtr->Size);

                //Disable all options
                for (j = 0; j < gDriverOptionCount; j ++)
				{
					if (!(gDriverData [j].Active & LOAD_OPTION_HIDDEN))		//TSE will not do the option anything if it is hidden
					{
                    	gDriverData [j].Active &= ~(LOAD_OPTION_ACTIVE);
					}
				}
                //Enable options in DriverOrder
                for (j = 0; j < gDriverOptionCount; j ++)
                {
                    pDriverData = DriverGetDriverData (DriverOrder [j]);
                    if(pDriverData)
					{
						if (!(pDriverData->Active & LOAD_OPTION_HIDDEN))		//TSE will not do the option anything if it is hidden
						{	
	                        pDriverData->Active |= LOAD_OPTION_ACTIVE;
						}
					}
                }
                //Save driver options
                for (j  = 0; j < gDriverOptionCount; j ++)
                {
                    CHAR16 varName[] = L"DriverXXXX";
                    BOOT_OPTION *pOption;
                    UINTN OptionSize;

                    OptionSize = 0; pOption = NULL;
                    SPrint (varName, sizeof (CHAR16) * (EfiStrLen (varName) + 1), gDriverFormarSpecifier, gDriverData [j].Option );
                    pOption = (BOOT_OPTION *)VarGetNvramName (varName, &gEfiGlobalVariableGuid, NULL, &OptionSize );
                    
                    if(pOption)
                    {
                        pOption->Active = gDriverData [j].Active;
    
                        VarSetNvramName (varName,
                            &gEfiGlobalVariableGuid,
                            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                            pOption,
                            OptionSize);
                        
                        MemFreePointer((VOID **)&pOption);
                    }
                }
				n=0;
                //Enabled options first
                for (k = 0; k < gDriverOptionCount; k ++)
                {
                    if(DISABLED_BOOT_OPTION == DriverOrder [k])
				    {	
			//add Disabled options
	                	for (j = n; j < gDriverOptionCount; j++ )
	                	{
                    		if ( (!(gDriverData [j].Active & LOAD_OPTION_ACTIVE)) && (!(gDriverData [j].Active & LOAD_OPTION_HIDDEN)) )
                    		{
								DriverOrder [k] = gDriverData [j].Option;
								n = j;
								n ++;
								break;
                       		}
	                	}
				    }		
                        
                }

                ASSERT (k == gDriverOptionCount);

                //Signal TPM if Boot order has changed			//No need of signalling the event for driver order change
/*				CurrOrder = VarGetNvramName (L"DriverOrder", &gEfiGlobalVariableGuid, NULL, &CurrSize);

                if (CurrSize != (k*sizeof(UINT16)))
                    EfiLibNamedEventSignal (&AmitseBootOrderChangeGuid);
                else if (CurrOrder)
                {
                    if (MemCmp ((UINT8 *)CurrOrder, (UINT8 *)DriverOrder, CurrSize) )
						EfiLibNamedEventSignal (&AmitseBootOrderChangeGuid);
                }

                MemFreePointer((VOID **)&CurrOrder);*/

                //Save boot order
                VarSetNvramName(L"DriverOrder",
                    &gEfiGlobalVariableGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    DriverOrder,
                    k * sizeof(UINT16));

                MemFreePointer((VOID **) &DriverOrder);

                //Continue to next variable
                continue;
			}

			if(VARIABLE_ID_BBS_ORDER == i)
			{
				CsmSaveBBSOrder(nvPtr);
                //Continue to Next variable
                continue;
			}

			VarSetNvram( i, nvPtr->Buffer, nvPtr->Size );
		}

		PasswordCommitChanges( TRUE );

		TSESetHDDPassword();

        SavedConfigChangesHook();

        //Disable AMI_CALLBACK_SUBMITTED if TSE_UEFI_26_FEATURES_SUPPORT is off
        if(IsTseUefi26FeaturesSupport())
        {
         	UEFICallSetupFormCallBack(AMI_CALLBACK_SUBMITTED);	//this helps EfiVarstore variables to get notified once their values are submitted(Saved into NvRam).
        }
		EfiLibNamedEventSignal (&AmitseNvramUpdateGuid);
		gTseCacheUpdated = FALSE;
		DoReConnectPageHandles();
		return TRUE;
	}
	return FALSE;
}

/**
    Function to save the bbs device order

    @param nvPtr 

    @retval VOID

**/
VOID SaveBBSOrder(NVRAM_VARIABLE *nvPtr)
{
    UINT16 *BootOrder;
    UINTN BootOrderSize,p;
    BBS_ORDER_TABLE *NewLegacyOrder, *NewDevEntry, *BufferDevEntry;
    UINT8 *NewBuffer, *NewBuffEntry;
    BOOT_DATA *pBootData;
    BBS_ORDER_LIST *BBSList = NULL;
    UINT16 count,r,s;
    UINT8 *CurrOrder = NULL;
    UINTN CurrSize = 0;
    UINTN DPSize = 0;                
    VOID *DisDPs = NULL;
    EFI_GUID BootManGuid = BOOT_MANAGER_GUID;
    EFI_GUID AmitseBootOrderChangeGuid = AMITSE_BOOT_ORDER_CHANGE_GUID;
    UINT32 LegDevOrderAttribute = 7;

    BootOrderSize = 0;
    BootOrder = VarGetNvramName(L"BootOrder", &gEfiGlobalVariableGuid, NULL, &BootOrderSize);

    if ((0 == BootOrderSize) || (NULL == nvPtr))
        return; //continue;

    NewLegacyOrder = EfiLibAllocateZeroPool(nvPtr->Size);
    NewBuffer = EfiLibAllocateZeroPool(nvPtr->Size);

    //NOTE: Taking advantage of the fact that
    //VARIABLE_ID_BBS_ORDER is after
    //VARIABLE_ID_BOOT_ORDER
    NewDevEntry = NewLegacyOrder;
    NewBuffEntry = NewBuffer;
    for(p=0; p<(BootOrderSize/sizeof(UINT16)); p++)
    {
        pBootData = BootGetBootData(BootOrder[p]);
			//Error Handling in case of pBootData is NULL. If its NULL the setupgets a CRASH
			if(pBootData == NULL)
				goto DONE;

        if(BBSValidDevicePath(pBootData->DevicePath))
        {
            if((nvPtr->Size < pBootData->LegacyEntryOffset)||((nvPtr->Size - (pBootData->LegacyEntryOffset))< sizeof(BBS_ORDER_TABLE)))
            {
                goto DONE;
            }
            
            BufferDevEntry = (BBS_ORDER_TABLE *)(nvPtr->Buffer + pBootData->LegacyEntryOffset);

				if ( (BufferDevEntry->Length >= nvPtr->Size) || (0 == BufferDevEntry->Length) ) 
				{
					goto DONE;
				}

            BBSList = pBootData->OrderList;

            count = pBootData->LegacyDevCount;

            MemCopy(NewDevEntry, BufferDevEntry, sizeof(UINT32)+(count+1)*sizeof(UINT16));
            MemCopy(NewBuffEntry, BufferDevEntry, sizeof(UINT32)+(count+1)*sizeof(UINT16));
        
	        //----------------------------------------------------------------------------------//       
	        // This block considers the case of legacy devices being positioned	//
	        // anywhere in the LegacyDevOrder (whether enabled or disabled)			//
	        //----------------------------------------------------------------------------------// 
	        if(count>0) //Disabled boot options exist
            {
                for(r=0;r<count;r++)
                {
      		        //See if a given index in BBSList exist in TypeEntry              
		            for(s=0;s<count;s++)
                    {
                        if(BBSList[r].Index == NewDevEntry->Data[s])
			            {
    				        break;
			            }
                    }

		            if(s>=count) //Index Not present
                    {
			            //Find the first disabled slot
			            for(s=0;s<count;s++)
              		    {
                      	    if(NewDevEntry->Data[s] == DISABLED_BOOT_OPTION)
			                {
				                //Add device path to Nvram variable
                                if(gIsSaveDisabledBBSDevicePath)
                                {
                  			        CsmBBSSetDisabled(BBSList[r].Index, &DisDPs, &DPSize);
                                }
				                NewDevEntry->Data[s] = BBSList[r].Index;
               			        NewDevEntry->Data[s] |= BBS_ORDER_DISABLE_MASK;
				                break;
			                }
               		    }
                    }
                }
            }

            pBootData->LegacyEntryOffset = (UINTN)NewDevEntry - (UINTN)NewLegacyOrder;

            NewDevEntry = (BBS_ORDER_TABLE *)((UINTN)NewDevEntry + sizeof(UINT32)+(count+1)*sizeof(UINT16));
            NewBuffEntry = NewBuffEntry + sizeof(UINT32)+(count+1)*sizeof(UINT16);
        }
    }

DONE:
    //Update the nv buffer with the re ordered list
    MemFreePointer((VOID **) &(nvPtr->Buffer));
    nvPtr->Buffer = NewBuffer;

    //Signal TPM if BBS order has changed
	CurrOrder = VarGetNvramName( L"LegacyDevOrder", &gLegacyDevGuid, &LegDevOrderAttribute, &CurrSize );

	if(CurrOrder)
	{
		if(MemCmp((UINT8 *)CurrOrder, (UINT8 *)NewLegacyOrder, CurrSize))
			EfiLibNamedEventSignal (&AmitseBootOrderChangeGuid);

		MemFreePointer((VOID **) &CurrOrder);
	}

    //Save BBS order
    VarSetNvramName(L"LegacyDevOrder",
        &gLegacyDevGuid,
        LegDevOrderAttribute,
        NewLegacyOrder,
        nvPtr->Size);

    if(gIsSaveDisabledBBSDevicePath)
    {
        //Save disabled device paths
        VarSetNvramName(L"DisabledDevs",
            &BootManGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
            DisDPs,
            DPSize);
    
        MemFreePointer((VOID **) &DisDPs);
        MemFreePointer((VOID **) &BootOrder);
    }
    MemFreePointer((VOID **) &NewLegacyOrder);
}

/**
    Function to exit form the setup application

    @param VOID

    @retval TRUE/FALSE

**/
BOOLEAN ExitApplication( VOID )
{
	if ( CallbackShowMessageBox( (UINTN)gExitMsgBox, MSGBOX_TYPE_YESNO ) == MSGBOX_YES )
	{
		if ( ! gVariableChanged )
			gResetRequired = FALSE;

		LoadPreviousValues( FALSE );
		gVariableChanged = FALSE;
		return TRUE;
	}

	return FALSE;
}
