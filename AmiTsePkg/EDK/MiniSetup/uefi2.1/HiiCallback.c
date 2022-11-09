//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

//*****************************************************************//
/** @file HiiCallback.c
    Contains Hii related Functions

**/
//*************************************************************************

//----------------------------------------------------------------------------
#include "minisetup.h"
#include "FormBrowser2.h"
#include "TseUefiHii.h"
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
static UINT8 *gPrevControlValue = NULL;
static CHAR16 *gCurPswdString = NULL;
extern BOOLEAN gEnableDrvNotification; //TRUE if allow notification function to process action, FALSE to ignore the notification
extern BOOLEAN gBrowserCallbackEnabled; //Allow external drivers to change cache only if it's TRUE; ignore browser callback otherwise
extern EFI_BROWSER_ACTION gBrowserCallbackAction ; //Contains the BrowserCallback action when a callback is in progress.
extern UINT32 PageListOffset, FirstPageOffset, PageInfoOffset;
EFI_STATUS ProcessBrowserActionRequestHook(EFI_BROWSER_ACTION_REQUEST actionReq);//Modified  ProcessBrowserActionRequest as board module hook
VOID CallBrowserActionsForNewHandles();
VOID ClearNewHandleList();
extern EFI_HANDLE *gNewHandleList;
extern UINTN gNewHandleCount;
extern BOOLEAN gPackUpdateHappened;
extern BOOLEAN IsCallbackForPageAllowed(UINTN Action, PAGE_INFO* PageInfo);
extern UINTN gSkipCallbackListCount;
//----------------------------------------------------------------------------

UINT32 GetUefiSpecVersion (VOID);
VOID UefiGetValidOptionSize(CONTROL_INFO *CtrlInfo, UINT32 *SizeOfData);
extern BOOLEAN IsControlWithDisableIfOpcode(CONTROL_INFO *controlInfo);
extern UINT16 gDisableFlag;
#if TSE_DEBUG_MESSAGES
extern EFI_STATUS DebugShowControlInfo(UINT32 formID, VOID *passedCtrlInfo) ;
#endif
/**

    @param Handle EFI_QUESTION_ID QuestionID,
    @param Type EFI_IFR_TYPE_VALUE *Value

    @retval EFI_STATUS
**/
EFI_STATUS FormCallBack(EFI_HII_HANDLE Handle, EFI_QUESTION_ID QuestionID, UINT8 Type, EFI_IFR_TYPE_VALUE *Value, EFI_BROWSER_ACTION action)
{
  EFI_HANDLE DriverHandle;
  EFI_GUID EfiHiiConfigAccessProtocolGuid = EFI_HII_CONFIG_ACCESS_PROTOCOL_GUID;
  EFI_HII_CONFIG_ACCESS_PROTOCOL  *ConfigAccess;
  EFI_BROWSER_ACTION_REQUEST actionReq = EFI_BROWSER_ACTION_REQUEST_NONE;
  //EFI_BROWSER_ACTION action = EFI_BROWSER_ACTION_CHANGED; //EFI_BROWSER_ACTION_CHANGING;
  EFI_STATUS status = EFI_SUCCESS;
  BOOLEAN PreservegEnableDrvNotification = FALSE;
	

  //
  //  Get Driver Handle that installed this Hii Form
  //  
  status = gHiiDatabase->GetPackageListHandle(gHiiDatabase, Handle, &DriverHandle); 
  if(EFI_ERROR(status))
  {
    SETUP_DEBUG_UEFI_CALLBACK ("[TSE] In FormCallBack(), Getting Driver Handle returned, status = %r\n", status );  
    goto DONE;
  }

  //
  //  Get ConfigAccess protocol handle
  //
  status = gBS->HandleProtocol( DriverHandle, &EfiHiiConfigAccessProtocolGuid , (VOID**)&ConfigAccess );
     
  if(EFI_ERROR(status))
  {
    SETUP_DEBUG_UEFI_CALLBACK ("[TSE] In FormCallBack(), Getting ConfigAccess protocol handle returned,  status = %r\n" , status ); 
    goto DONE;
  }

  //
  //  Invoke Config Access Protocol
  //
  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] In FormCallBack(), calling ConfigAccess->Callback()\n" );

  PreservegEnableDrvNotification = gEnableDrvNotification;
  gEnableDrvNotification = TRUE; //Set to enable notification processing
  gBrowserCallbackEnabled = TRUE;
  gBrowserCallbackAction = action ; //Save the current BrowserCallback action
  status = ConfigAccess->Callback(ConfigAccess, action, QuestionID, Type, Value, &actionReq);
  gBrowserCallbackAction = 0 ;		//Clear the current BrowserCallback action
  gBrowserCallbackEnabled = FALSE;
  if (!PreservegEnableDrvNotification) // If gEnableDrvNotification is already True Don't touch it
	  gEnableDrvNotification = FALSE; //Reset to disable notification processing

  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] In FormCallBack(), ConfigAccess->Callback returned, %r\n" , status );   
  if(EFI_ERROR(status))
  {
    goto DONE;
  }

  // Clean up PAGE_INFO struct with Handle set to 0xFFFF
  status = FixSetupData();
  if(EFI_ERROR(status))
  {
  }

  if(actionReq)
  {
    status = ProcessBrowserActionRequestHook(actionReq);//Modified  ProcessBrowserActionRequest as board module hook
    SETUP_DEBUG_UEFI_CALLBACK ("[TSE] In FormCallBack(),ProcessBrowserActionRequestHook with actionReq: %d returned,status = %r\n\n" ,actionReq,status );
  }

DONE:
  return status;
}

/**

    @param ControlData UINT16 Key , UINT8 Flags, UINTN Action

    @retval EFI_STATUS
**/
BOOLEAN BrowserActionDiscard = FALSE;
EFI_STATUS CallFormCallBack(CONTROL_INFO * ControlData, UINT16 Key ,UINT8 Flags, UINTN Action)
{
  EFI_STATUS status = EFI_SUCCESS;
  EFI_IFR_TYPE_VALUE value;
  EFI_IFR_TYPE_VALUE Temp;
  EFI_IFR_TYPE_VALUE *orderlistvalue = (EFI_IFR_TYPE_VALUE *)NULL;
  EFI_IFR_TYPE_VALUE *orderlistTemp = (EFI_IFR_TYPE_VALUE *) NULL;
  EFI_IFR_OP_HEADER *OpHeader = (EFI_IFR_OP_HEADER *)ControlData->ControlPtr;
  EFI_BROWSER_ACTION action ;
  EFI_HII_HANDLE hiiHandle = NULL;
  UINT16 ControlType = 0;
  UINT8	type = 0;
  UINT16 tok = 0;
  UINT32 size = 0;
  UINT32 offset = 0;
  VOID *ctrlValue = NULL, *FullCtrlValue = NULL;
  UINT32 UefiSpecVer = 0;
  UINT32 tempVariable = 0; 
  CONTROL_DATA *Control;
  UINT8 OpCode = OpHeader->OpCode;
  VARIABLE_INFO *varInfo = (VARIABLE_INFO *)NULL ;

      
  gPackUpdateHappened = FALSE;

  MemSet( &value, sizeof(value), 0 );
  MemSet( &Temp, sizeof(Temp), 0 );
  
  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Entering CallFormCallBack()\n" ); 

  UefiSpecVer = GetUefiSpecVersion ();
  if (UefiSpecVer <= 0x2000A)     //If spec version is 2.0 suppressing other actions
  {
    if (AMI_CALLBACK_CONTROL_UPDATE != Action)
    {
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Exiting CallFormCallBack() since action (%d) is not supported for the UEFI spec version 2.0\n", Action );
        status = EFI_UNSUPPORTED;
		goto DONE;
    }
  }
  else if (UefiSpecVer >= 0x2001E)   //If spec version is greater than 2.30 then suppressing other actions
  {
    if (
        (AMI_CALLBACK_CONTROL_UPDATE != Action) &&  
        (AMI_CALLBACK_RETRIEVE != Action) &&
        (AMI_CALLBACK_FORM_OPEN != Action) &&
        (AMI_CALLBACK_FORM_CLOSE != Action) &&
        (AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING != Action)&&
  		  (AMI_CALLBACK_FORM_DEFAULT_STANDARD != Action) &&
  		  (AMI_CALLBACK_SUBMITTED != Action)
  		  )//Making the specify setup items departing from F2/F3 effect.
    {
       switch(Action)
	    {
	        case AMI_CALLBACK_FORM_OPEN:
                SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_FORM_OPEN Unsupported\n" );         
                break;
            case AMI_CALLBACK_FORM_CLOSE:
                SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_FORM_CLOSE Unsupported\n" );
                break;
            case AMI_CALLBACK_RETRIEVE:
                SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_RETRIEVE Unsupported\n" );
                break;
            case AMI_CALLBACK_CONTROL_UPDATE:            
                SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_CHANGING Unsupported\n" );
                break;
				case AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING : 
					SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING Unsupported\n" );
               break;
				case AMI_CALLBACK_FORM_DEFAULT_STANDARD : 
					SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] AMI_CALLBACK_FORM_DEFAULT_STANDARD Unsupported\n" );// Making the specify setup items departing from F2/F3 effect.
               break;
    	    default:
    	        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Callback Action Unsupported\n" ); 
    	    break;
        }
        
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Exiting CallFormCallBack()\n" );
        status = EFI_UNSUPPORTED;
		goto DONE;
    }
  }        
  action = UefiGetActionWapper(Action);
    
  if(ControlData->ControlHandle == INVALID_HANDLE)
  {  
	SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Exiting CallFormCallBack() due to invalid control handle \n" ); 
   	goto DONE;
  }  
  
  hiiHandle = ControlData->ControlHandle;
  ControlType = ControlData->ControlType;
  tempVariable = ControlData->ControlVariable;

  if(AMI_CALLBACK_FORM_OPEN != Action)
  {
	  if(OpHeader->OpCode != EFI_IFR_ACTION_OP && OpHeader->OpCode != EFI_IFR_REF_OP)
	  {
	    offset = UefiGetQuestionOffset(ControlData->ControlPtr);
	    varInfo = VarGetVariableInfoIndex( ControlData->ControlVariable );
	    if ((VARIABLE_ATTRIBUTE_NAMEVALUE == (varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE)))
	    {
	        offset = 0;  //offset is always 0 for name/value variables
	    }
	    
	    //If the control is Ordered list then we are calculating the actual size
		if(ControlData->ControlType == CONTROL_TYPE_ORDERED_LIST)
		{
		    UINTN Type=0;
		    UefiGetValidOptionType( ControlData, &Type, &size);
		    size = ((EFI_IFR_ORDERED_LIST *)(ControlData->ControlPtr))->MaxContainers * size;
		    if(size)
		    {
		        orderlistvalue = (EFI_IFR_TYPE_VALUE *)EfiLibAllocateZeroPool (size);
		        orderlistTemp = (EFI_IFR_TYPE_VALUE *) EfiLibAllocateZeroPool (size);  
		    }
		}
		else
			size = UefiGetWidth(ControlData->ControlPtr);

		if ((EFI_IFR_PASSWORD_OP == OpHeader->OpCode) || (EFI_IFR_STRING_OP == OpHeader->OpCode))
		{
			ctrlValue = EfiLibAllocateZeroPool (size + sizeof (CHAR16));		//If max character reached then the string should have NULL at last
		}
		else
		{
			ctrlValue = EfiLibAllocateZeroPool (size);
		}
	    if(ctrlValue == NULL)
	    {
	      status = EFI_OUT_OF_RESOURCES;
	      goto DONE;
	    }
	
		if((EFI_IFR_PASSWORD_OP == OpHeader->OpCode)&&(NULL != gCurPswdString) )
		{
			MemCopy( ctrlValue, gCurPswdString, size);
		}
		else
	   	{
			if (ControlData->ControlFlags.BitFieldVarstore)
			{
			    status = _VarGetSetValueForBitField( VAR_COMMAND_GET_VALUE, gVariableList, ControlData->ControlVariable, 
					offset, size, ctrlValue, ControlData->ControlPtr);
			}
			else 
			{
			    status = VarGetValue(ControlData->ControlVariable, offset, size, ctrlValue );
			}
	  	}
	    
	    if(EFI_ERROR(status))
	    {
	      goto DONE;
	    }
		//Checking the Previous value presence and updateing the cache with the previous value
		if(NULL != gPrevControlValue)
        {
            if (ControlData->ControlFlags.BitFieldVarstore)
            {
                status = _VarGetSetValueForBitField( VAR_COMMAND_SET_VALUE, gVariableList, ControlData->ControlVariable, 
                    offset,  size,  gPrevControlValue, ControlData->ControlPtr);
            }
            else
            {
                status = VarSetValue(ControlData->ControlVariable, offset, size, gPrevControlValue );
            }
        }
		if(EFI_ERROR(status))
	    {
	      goto DONE;
	    }
	  }
	
	  switch(OpHeader->OpCode)
	  {
	  case EFI_IFR_NUMERIC_OP:
	    type = ((EFI_IFR_NUMERIC *)(ControlData->ControlPtr))->Flags & EFI_IFR_NUMERIC_SIZE;
	    MemCopy(&(value), ctrlValue, size);
	    break;
	  case EFI_IFR_REF_OP:
        //type = EFI_IFR_TYPE_UNDEFINED; //REF_OP uses type EFI_IFR_TYPE_UNDEFINED
		 
        if ((GetUefiSpecVersion ()) >= 0x2001E)			//If spec version is greater than or equals 2.3 then type will be buffer
        {
            type = EFI_IFR_TYPE_REF;
        }
        else
        {
            type = EFI_IFR_TYPE_UNDEFINED;
        }	
	    break;
	  case EFI_IFR_ONE_OF_OP:
	    type = ((EFI_IFR_ONE_OF *)(ControlData->ControlPtr))->Flags & EFI_IFR_NUMERIC_SIZE;
	    MemCopy(&(value), ctrlValue, size);
	    break;
	  case EFI_IFR_ORDERED_LIST_OP:
		 if ((GetUefiSpecVersion ()) >= 0x2001E)			//If spec version is greater than or equals 2.3 then type will be buffer
		 {
	    	type = EFI_IFR_TYPE_BUFFER;
		 }
		 else
		 {
			type = EFI_IFR_TYPE_OTHER;
		 }		
	    MemCopy((orderlistvalue), ctrlValue, size);
	    break;
	  case EFI_IFR_CHECKBOX_OP:
	    type = EFI_IFR_TYPE_BOOLEAN;
	    MemCopy(&(value), ctrlValue, size);
	    break;
	  case EFI_IFR_ACTION_OP:
	    type = EFI_IFR_TYPE_ACTION;
	    tok = ((EFI_IFR_ACTION*)ControlData->ControlPtr)->QuestionConfig;
	    MemCopy(&(value), &tok, sizeof(EFI_STRING_ID));
	    break;
	  case EFI_IFR_STRING_OP:
	  case EFI_IFR_PASSWORD_OP:
	    type = EFI_IFR_TYPE_STRING;
	    tok = HiiAddString(hiiHandle, ctrlValue);
	    MemCopy(&(value), &tok, sizeof(EFI_STRING_ID));
	    break;
	  case EFI_IFR_DATE_OP:
		type = EFI_IFR_TYPE_DATE;
		MemCopy(&(value), ctrlValue, size);
		break;
  	  case EFI_IFR_TIME_OP:
		type = EFI_IFR_TYPE_TIME;
		MemCopy(&(value), ctrlValue, size);
		break;

	  default:
	    goto DONE;
	    break;
	  }
  }
  else
  {
  	type = EFI_IFR_TYPE_UNDEFINED;
  }

 SetCallBackControlInfo(hiiHandle, tempVariable);

  switch(Action)
  {
    case AMI_CALLBACK_FORM_OPEN:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_FORM_OPEN \n" );         
        break;
    case AMI_CALLBACK_FORM_CLOSE:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_FORM_CLOSE \n" );
        break;
    case AMI_CALLBACK_RETRIEVE:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_RETRIEVE \n" );
        break;
    case AMI_CALLBACK_CONTROL_UPDATE:            
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_CHANGING \n" );
        break;
		case AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING : 
			SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING \n" );
         break;
		case AMI_CALLBACK_FORM_DEFAULT_STANDARD : 
			SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_DEFAULT_STANDARD \n" );// Making the specify setup items departing from F2/F3 effect.
         break;
	    case AMI_CALLBACK_SUBMITTED:
	        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_SUBMITTED \n" );
	        break;
    default:
       
       break;
  }
  
#if TSE_DEBUG_MESSAGES
  if((gDbgPrint & PRINT_UEFI_CALLBACK) == PRINT_UEFI_CALLBACK)
  {
	    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Control details: \n" );  
  		DebugShowControlInfo(ControlData->ControlPageID, ControlData);
		SETUP_DEBUG_UEFI_CALLBACK (  "Value type : %d\n\n", type);  
  }
#endif  
  BrowserActionDiscard = FALSE;	
  if(ControlData->ControlType != CONTROL_TYPE_ORDERED_LIST)
  {
      Temp=value;
      status = FormCallBack(hiiHandle, Key, type, &value, action);
  }
  else
  {
      MemCopy((orderlistTemp), orderlistvalue, size);
      status = FormCallBack(hiiHandle, Key, type, orderlistvalue, action);
  }
  
  switch(Action)
  {
    case AMI_CALLBACK_FORM_OPEN:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_FORM_OPEN returned,  status = %r \n" , status );          
        break;
    case AMI_CALLBACK_FORM_CLOSE:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_FORM_CLOSE returned,  status = %r \n" , status ); 
        break;
    case AMI_CALLBACK_RETRIEVE:
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_RETRIEVE returned,  status = %r \n" , status ); 
        break;
    case AMI_CALLBACK_CONTROL_UPDATE:            
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_CHANGING returned,  status = %r \n" , status ); 
        break;
		case AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING : 
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING returned,  status = %r \n" , status ); 
        break; 
		case AMI_CALLBACK_FORM_DEFAULT_STANDARD : 
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] AMI_CALLBACK_FORM_DEFAULT_STANDARD returned,  status = %r \n" , status ); // Making the specify setup items departing from F2/F3 effect.
        break;
	    case AMI_CALLBACK_SUBMITTED:
	    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_SUBMITTED returned,  status = %r \n" , status ); 
	    break;
   default:
       
       break;
  }     
    
  SetCallBackControlInfo(NULL, 0);
  
  if(CONTROL_TYPE_SUBMENU == ControlType) { //Skip rest of the step for REF / Submenu control
    goto DONE;
  }
  //As per Spec 2.5: Driver returns EFI_NOT_AVAILABLE_YET or EFI_UNSUPPORTED to terminate password processing and other error for pre existing password.
  if ((GetUefiSpecVersion()) >= 0x20032)
  {
	  if( (OpCode == EFI_IFR_PASSWORD_OP) && ( (EFI_UNSUPPORTED == status) || (EFI_NOT_AVAILABLE_YET == status) ) )
	  {
		  goto DONE;
	  }
  }
  if((status == EFI_UNSUPPORTED)&& (action==EFI_BROWSER_ACTION_CHANGING))
  {
      if(ControlData->ControlType != CONTROL_TYPE_ORDERED_LIST)
          value=Temp;
      else
          MemCopy((orderlistvalue), orderlistTemp, size);
  }
	//as per spec, update the value in cache only if Status is EFI_SUCCESS or EFI_UNSUPPORTED
  	//Update the Value in cache if action other than EFI_BROWSER_ACTION_FORM_OPEN and EFI_BROWSER_ACTION_FORM_CLOSE
	//For the action EFI_BROWSER_ACTION_CHANGING update the cache and invoke callback with EFI_BROWSER_ACTION_CHANGED
    if((!EFI_ERROR(status) ||(status == EFI_UNSUPPORTED))&&(OpCode != EFI_IFR_ACTION_OP && OpCode != EFI_IFR_REF_OP) && (action!=EFI_BROWSER_ACTION_FORM_OPEN && action!=EFI_BROWSER_ACTION_FORM_CLOSE))
	{
		if (FALSE == BrowserActionDiscard)
	 	{
			if((OpCode == EFI_IFR_STRING_OP)||(OpCode == EFI_IFR_PASSWORD_OP))
			{
				if ( NULL != ctrlValue )
				{
				    MemSet( ctrlValue, size + sizeof(CHAR16), 0 );
					MemFreePointer( (VOID **)&ctrlValue );
				}
				ctrlValue = HiiGetString( hiiHandle, value.string );
				if(ctrlValue == NULL)
				{
					status = EFI_NOT_FOUND;
					goto DONE;
				}
				else
				{
				    FullCtrlValue = EfiLibAllocateZeroPool (size + sizeof(CHAR16));
				    if (NULL == FullCtrlValue)
				    {
				        status = EFI_OUT_OF_RESOURCES;
				        goto DONE;
				    }
				    StrCpyS (FullCtrlValue, (size/sizeof(CHAR16)) + 1, ctrlValue);  //Using StrCpyS instead of memcpy to avoid exceeding boundary
				    VarSetValue (tempVariable, offset, size, FullCtrlValue);// Making the specify setup items departing from F2/F3 effect.
				    MemSet( FullCtrlValue, size, 0 );
				    MemFreePointer ((VOID **)&FullCtrlValue);
				}
			}
			else
			{
			    if(ControlData->ControlType != CONTROL_TYPE_ORDERED_LIST)
			    {
			        if (ControlData->ControlFlags.BitFieldVarstore)
			        {
			            _VarGetSetValueForBitField( VAR_COMMAND_SET_VALUE, gVariableList, tempVariable, offset, size,  &value, ControlData->ControlPtr);
			        }
			        else
			        {
			            VarSetValue(tempVariable, offset, size, &value );// Making the specify setup items departing from F2/F3 effect.
			        }
			    }
			    else 
			    {
			        VarSetValue(tempVariable, offset, size, orderlistvalue );
			    }
			}
		}
	}
	BrowserActionDiscard = FALSE;
  
	if( (action == EFI_BROWSER_ACTION_CHANGING ) && ((status == EFI_UNSUPPORTED) || (status == EFI_SUCCESS)) )
	{
	    if (gPackUpdateHappened)         //If there is no packupdate done then no need of getting updated control
	    {
	        Control = GetUpdatedControlData((CONTROL_DATA*)NULL, ControlType, hiiHandle, Key);
	        if (Control)
	        {
	            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Aft GetUpdatedControlData Variable- %x, Handle - %x, OpCode - %x\n", Control->ControlData.ControlVariable, Control->ControlData.ControlHandle, ((EFI_IFR_OP_HEADER *)(Control->ControlData.ControlPtr))->OpCode);
	            if (Control->ControlData.ControlVariable == tempVariable && ((EFI_IFR_OP_HEADER *)(Control->ControlData.ControlPtr))->OpCode == OpCode)
	            {
	                OpHeader = Control->ControlData.ControlPtr;
	                if (MemCmp((VOID *)ControlData, (VOID *)&(Control->ControlData), sizeof(CONTROL_INFO)))
	                {
	                    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL DATA MISMATCHING -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	                    ControlData = &(Control->ControlData);
	                }
	            }
	            else
	            {
	                SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL NOT FOUND -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	                goto DONE;
	            }
	        }
	        else
	        {
	            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL IS DELETED -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	            goto DONE;
	        }
            gPackUpdateHappened = FALSE;
	    }
	    switch(OpHeader->OpCode)                    //Need to use the updated Opcode if packupdate done in the changing callback.
	    {
	    case EFI_IFR_NUMERIC_OP:
	    case EFI_IFR_ONE_OF_OP:
	    case EFI_IFR_ORDERED_LIST_OP:
	    case EFI_IFR_CHECKBOX_OP:
	    case EFI_IFR_ACTION_OP:
	    case EFI_IFR_STRING_OP:
	    case EFI_IFR_PASSWORD_OP:
	    case EFI_IFR_DATE_OP:
	    case EFI_IFR_TIME_OP:
	        if(hiiHandle != NULL)
	        {
	            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_CHANGING Success\n" ); 

	            action = EFI_BROWSER_ACTION_CHANGED;
	            SetCallBackControlInfo(hiiHandle, tempVariable);

	            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Sending EFI_BROWSER_ACTION_CHANGED \n" ); 
	            //errors returned from EFI_BROWSER_ACTION_CHANGED are ignored.
             if(ControlData->ControlType != CONTROL_TYPE_ORDERED_LIST)
	            FormCallBack(hiiHandle, Key, type, &value,action);
             else 
                FormCallBack(hiiHandle, Key, type, orderlistvalue,action);
             
	            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] EFI_BROWSER_ACTION_CHANGED returned,  status = %r \n" , status ); 

	            SetCallBackControlInfo(NULL, 0);

	            if (gPackUpdateHappened)     //If there is no packupdate done then no need of getting updated control
	            {
	                Control = GetUpdatedControlData((CONTROL_DATA*)NULL, ControlType, hiiHandle, Key);
	                if (Control)
	                {
	                    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Aft GetUpdatedControlData Variable- %x, Handle - %x, OpCode - %x\n", Control->ControlData.ControlVariable, Control->ControlData.ControlHandle, ((EFI_IFR_OP_HEADER *)(Control->ControlData.ControlPtr))->OpCode);
	                    if (Control->ControlData.ControlVariable == tempVariable && ((EFI_IFR_OP_HEADER *)(Control->ControlData.ControlPtr))->OpCode == OpCode)
	                    {
	                        OpHeader = Control->ControlData.ControlPtr;
	                        if (MemCmp((VOID *)ControlData, (VOID *)&(Control->ControlData), sizeof(CONTROL_INFO)))
	                        {
	                            SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL DATA MISMATCHING -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	                            ControlData = &(Control->ControlData);
	                        }
	                    }
	                    else
	                    {
	                        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL NOT FOUND -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	                        goto DONE;
	                    }
	                }
	                else
	                {
	                    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] CONTROL IS DELETED -> ContorlHandle - %x, ControlType - %x, Key - %x, Opcode - %x\n" ,hiiHandle, ControlType, Key, OpCode);
	                    goto DONE;
	                }
	            }
				SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Checking Read/Write for current control\n");
	            HandleWriteOpOnChanged(ControlData);      //Evaluating Write opcode for a current control if the action is AMI_CALLBACK_CONTROL_UPDATE
	            HandleReadOpOnChanged(ControlData->ControlPageID);    //Evaluating Read opcode for all the controls in current page to update to the latest edited value
				SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Read/Write verification completed\n");
	        }
	        status = EFI_SUCCESS;
	        break;
	    default:
	        break;
	    }
	}
DONE:	
	if((OpCode == EFI_IFR_STRING_OP || OpCode == EFI_IFR_PASSWORD_OP)&&(tok != 0))
	{
	    HiiRemoveString(hiiHandle, tok);
	}
	gPackUpdateHappened = FALSE;
    if(gPrevControlValue)
        MemFreePointer( (VOID **)&gPrevControlValue );
    if(gCurPswdString)
        StringZeroFreeMemory( (VOID **)&gCurPswdString );
    if(ControlType == CONTROL_TYPE_ORDERED_LIST)
    {
        if(orderlistvalue)
            MemFreePointer( (VOID **)&orderlistvalue );
        if(orderlistTemp)
            MemFreePointer( (VOID **)&orderlistTemp );
    }
    if(ctrlValue)
    {
        if (OpCode == EFI_IFR_PASSWORD_OP)
        {
            UINTN ctrlValueSize = StrSize(ctrlValue);
            MemSet( ctrlValue, ctrlValueSize, 0 );
        }    
        MemFreePointer( (VOID **)&ctrlValue );
    }
    SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Exiting CallFormCallBack() Status = %r\n\n", status ); 
    return status;
}
/**

    @param ControlData UINT16 Key

    @retval EFI_STATUS
**/
EFI_STATUS SpecialActionCallBack(CONTROL_INFO * ControlData, UINT16 Key)
{
  EFI_STATUS status = EFI_SUCCESS;
  EFI_IFR_TYPE_VALUE value;

  UINT16 tok = 0;
  UINT8	type = 0;


  MemSet( &value, sizeof(value), 0 );
  type = EFI_IFR_TYPE_ACTION;
  tok = ((EFI_IFR_ACTION*)ControlData->ControlPtr)->Question.Header.Prompt;
  MemCopy(&(value), &tok, sizeof(EFI_STRING_ID));

  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] In SpecialActionCallBack(), Sending EFI_BROWSER_ACTION_CHANGING \n" ); 
#if TSE_DEBUG_MESSAGES
  if((gDbgPrint & PRINT_UEFI_CALLBACK) == PRINT_UEFI_CALLBACK)
  {
        SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] Control details: \n" );  
        DebugShowControlInfo(ControlData->ControlPageID, ControlData);
        SETUP_DEBUG_UEFI_CALLBACK (  "Value type : %d\n\n", type);  
  }
#endif  
  SetCallBackControlInfo(ControlData->ControlHandle, ControlData->ControlVariable);
  status = FormCallBack(ControlData->ControlHandle, Key, type, &value,EFI_BROWSER_ACTION_CHANGING);
  SetCallBackControlInfo(NULL, 0);

  SETUP_DEBUG_UEFI_CALLBACK (  "[TSE] In SpecialActionCallBack(), EFI_BROWSER_ACTION_CHANGING returned,  status = %r \n\n" , status );     

  return status;
}

/**
    store the Control value before updating and calling the callback.

    @param ControlData 

    @retval EFI_STATUS
**/

VOID UefiPreControlUpdate (CONTROL_INFO *ControlData)
{

	if(gPrevControlValue)
		MemFreePointer( (VOID **)&gPrevControlValue );

	if (NULL == ControlData)			//UefiPreControlUpdate will be called with NULL for clearing gPrevControlValue. So returning from here if it is NULL.
	{
		return;
	}	
	if(UefiIsInteractive(ControlData))
	{
		UINT32 size = 0;
		UINT32 offset = 0;
		offset = UefiGetQuestionOffset(ControlData->ControlPtr);
		if(ControlData->ControlType == CONTROL_TYPE_ORDERED_LIST)
		{
			UINTN Type=0;
			UefiGetValidOptionType( ControlData, &Type, &size);
			size = ((EFI_IFR_ORDERED_LIST *)(ControlData->ControlPtr))->MaxContainers * size;
		}
		else
			size = UefiGetWidth(ControlData->ControlPtr);
		gPrevControlValue = EfiLibAllocateZeroPool(size);
		if(gPrevControlValue == NULL)
		{
			//EFI_OUT_OF_RESOURCES;
			return;
		}

        if (ControlData->ControlFlags.BitFieldVarstore)
        {
            _VarGetSetValueForBitField( VAR_COMMAND_GET_VALUE, gVariableList, ControlData->ControlVariable, offset, size, 
                    gPrevControlValue, ControlData->ControlPtr);
        }
        else
        {
            VarGetValue(ControlData->ControlVariable, offset, size, gPrevControlValue );
        }
	}
}

/**
    fucntion to save the password string.

    @param ControlData CHAR16 *String

    @retval VOID
**/

VOID SavePswdString (CONTROL_INFO *ControlData, CHAR16 *String)
{

	if(gCurPswdString)
	    StringZeroFreeMemory ( (VOID **)&gCurPswdString );

	if (NULL == ControlData)			//UefiPreControlUpdate will be called with NULL for clearing gPrevControlValue. So returning from here if it is NULL.
	{
		return;
	}	
	if(UefiIsInteractive(ControlData))
	{
		UINT32 size = 0;
		size = UefiGetWidth(ControlData->ControlPtr);
		gCurPswdString = EfiLibAllocateZeroPool(size + sizeof (CHAR16));
		if ( NULL == gCurPswdString )
        	return ;
		if(String == NULL){
			*gCurPswdString = L'\0';//Set password to null string
			return;
		}
		MemCopy(gCurPswdString, String, size);
	}
}

/**
    If the Formcallback returns status is EFI_UNSUPPORTED or EFI_SUCCESS.
    Change page is allowed. Otherwise changeing of page should not happen
    For UEFI 2.0 Just return EFI_SUCCESS;

    @param EFI_STATUS

    @retval EFI_STATUS
**/
EFI_STATUS UefiIsProceedWithPageChange(EFI_STATUS Status)
{
	// For Submenu if the status is EFI_UNSUPPORTED or EFI_SUCCESS.
	// Change page is allowed.
	if((Status == EFI_UNSUPPORTED) || (Status == EFI_SUCCESS))
	{
		return EFI_SUCCESS;
	}
	else
		return Status;
}

/**
  	Processes the callbacks for the specific PAGE_INFO with the assumption that a package update may happen during the callback.
  	Aborts the execution, returning the last processed ControlID
  	The caller is supposed to pass the last ControlID to resume the processing.
  	Any controls inserted in front the processed control will be skipped.
  
    @param PAGE_INFO*, Action, ControlId

    @retval EFI_STATUS
**/

EFI_STATUS UEFICallSetupFormCallBackFormExt(PAGE_INFO* PageInfo, UINTN Action, UINT32* ControlID )
{
	UINT32 control, ControlStart = 0, CurrentControlID;
	UINT32 ControlCount = PageInfo->PageControls.ControlCount;
	CONTROL_INFO *controlInfo;
	UINT8* CurrentAppData = gApplicationData;
	
	if (*ControlID != 0)// the page has been processed before, find the next control after ControlID
	{
		for ( control = 0; control < ControlCount; control++ )
		{
			controlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + PageInfo->PageControls.ControlList[control]);
			if (UefiGetControlKey(controlInfo)== *ControlID)
				break;
		}
		if (control < ControlCount) // if break happened, need to update ControlStart
		{	
			ControlStart = ++control;
		}
		//if control is not found start over
	}
	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackFormExt ControlStart %d, ControlCount %d\n", ControlStart, ControlCount );
	for ( control = ControlStart; control < ControlCount; control++ )
	{
		controlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + PageInfo->PageControls.ControlList[control]);

		if (UefiIsInteractive(controlInfo))
		{
		    //skip callback for the controls inside DisableIf opcode
		    if((gDisableFlag == SkipDisableOp)&&(Action == AMI_CALLBACK_FORM_OPEN || Action == AMI_CALLBACK_RETRIEVE) && 
		            (IsControlWithDisableIfOpcode(controlInfo)) )
		        continue;
		    // skip callback for all the controls but the controls evaluated to enable during disable evaluation
		    if((gDisableFlag == EvaluateDisableOpControls)&&(Action == AMI_CALLBACK_FORM_OPEN || Action == AMI_CALLBACK_RETRIEVE) && 
		            (((IsControlWithDisableIfOpcode(controlInfo)) && !(controlInfo->ControlFlags.ControlVisible)) || 
		            !(IsControlWithDisableIfOpcode(controlInfo))) )
                continue;
		    
		    CurrentControlID = UefiGetControlKey(controlInfo);
			UefiPreControlUpdate(NULL);	// Updating the gPrevControlValue buffer to NULLL before invoking CallFormCallBack 
            CallFormCallBack(controlInfo, UefiGetControlKey(controlInfo), FALSE,Action);
            // After the callback things may change - if that's the case report back
            if (gApplicationDataRelocated || (CurrentAppData != gApplicationData))
            {
                gApplicationDataRelocated = FALSE;
                *ControlID = CurrentControlID;
				SETUP_DEBUG_UEFI_CALLBACK ( "[TSE]: UEFICallSetupFormCallBackFormExt return EFI_NOT_READY, control: %d  CurrentControlID: %d\n", control, CurrentControlID );
				return EFI_NOT_READY;
			}
		}
	} // end for control
	SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] UEFICallSetupFormCallBackFormExt return EFI_SUCCESS\n" );
	return EFI_SUCCESS;
}

/**
  	Finds the pointer to the actual Page_info structure, based on Page ID and a handle
  
    @param PAGE_INFO*

    @retval VOID
**/

PAGE_INFO* FindNewPageInfo (PAGE_INFO* WorkingPageInfo)
{
	UINTN i;
	for (i = 0; i < (UINTN)gPages->PageCount; i++ ) 
	{
		PAGE_INFO* info = (PAGE_INFO*)((UINTN)gApplicationData + gPages->PageList[i]);
		if( (info->PageFormID == WorkingPageInfo->PageFormID) && (info->PageHandle == WorkingPageInfo->PageHandle)
			&& 0 != info->PageHandle && info->PageHandle != (VOID*)(UINTN)0xffff ) 
		{
			SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] New Page Info found: index %d\n", i );
			return info; 
		}
	}
	SETUP_DEBUG_UEFI_CALLBACK ( "[TSE] New Page Info not found\n");
	return (PAGE_INFO*) NULL;			
}
/**
    The Wrapper function function which calls the CallFormCallBack() if the control is Interactive
    This function allows for package update during its execution
    It will process all the pages that exist at the moment of initialization. The pages that are added using
    New package update are not covered.
    
    @param Action 

    @retval VOID
**/
VOID UEFICallSetupFormCallBack(UINTN Action )
{
//Implementation of FormBrowser with actions support 
	UINT32	page;
	UINT32	PageCount = 0;
	UINT32  CurrentControlID = 0;

	PAGE_LIST *WorkingPageList = NULL;  // Copy of gPages on function entry
	PAGE_INFO *WorkingAppData = NULL;   // copy gApplicationData on function entry
	UINT8 *CurrentAppData = gApplicationData; // back up of gpages ptr -> can be updated during pack update.

	if(gPages)
	    PageCount = gPages->PageCount;
	ClearNewHandleList(); // To have new handles Formset's handle only in Open or Retrieve Action's callback. 
	
	WorkingAppData = (PAGE_INFO *)EfiLibAllocateZeroPool(PageListOffset+FirstPageOffset+PageInfoOffset);
	MemCopy(WorkingAppData, gApplicationData, PageListOffset+FirstPageOffset+PageInfoOffset);
	WorkingPageList = (PAGE_LIST *) WorkingAppData;
		
	gBrowserCallbackEnabled = TRUE;
	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBack Entering : Action = %d", Action );
	if (NULL != gApplicationData)
	{
		// go through the previously saved WorkingPageInfo 
		for ( page = 0; page < PageCount; page++ )
		{
			PAGE_INFO *pageInfo;
			PAGE_INFO *WorkingPageInfo = (PAGE_INFO *)((UINTN)WorkingAppData + WorkingPageList->PageList[page]);
			SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBack page %d of %d", page, PageCount );
			
			if(NULL == WorkingPageInfo || 0 == WorkingPageInfo->PageHandle || WorkingPageInfo->PageHandle == (VOID*)(UINTN)0xffff)
				continue;  // non recoverable, go to next
			
			if((gSkipCallbackListCount>1) && !IsCallbackForPageAllowed(Action,WorkingPageInfo))
			    continue;// Don't allow callback with current Action for this Page.
			
			if (CurrentAppData == gApplicationData) // the page order is the same, so can use gpages directly
			{	
				pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[page]);
			}		
			else // gPages was updated so the page has to be found again
			{
				SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBack: Page Info was updated find new page %d", page );
				pageInfo = FindNewPageInfo(WorkingPageInfo);			
			}
			if (NULL == pageInfo || 0 == pageInfo->PageHandle || pageInfo->PageHandle == (VOID*)(UINTN)0xffff) 
				continue; // if the page is still bad, continue
			//try calling the callback
			if (EFI_NOT_READY == UEFICallSetupFormCallBackFormExt(pageInfo, Action, &CurrentControlID))
			{
				SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBack returned: page %d, CurrentControlID %d", page, CurrentControlID );
				page--; // process the page once again
			}
			else //everything is fine, reset controlID
			{	
				CurrentControlID = 0;
			}
		} // end for page loop
	} 
	
	if(Action == EFI_BROWSER_ACTION_FORM_OPEN || Action == EFI_BROWSER_ACTION_RETRIEVE)
		CallBrowserActionsForNewHandles();// Call Browser Action EFI_BROWSER_ACTION_FORM_OPEN and EFI_BROWSER_ACTION_RETRIEVE for newly added Formset 
	
	gBrowserCallbackEnabled = FALSE;
	MemFreePointer ((VOID **)&WorkingAppData);
	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] Exiting UEFICallSetupFormCallBack()\n");
}


/**
    The Wrapper function function which calls the CallFormCallBack() if the control is Interactive    	

    @param Action 

    @retval VOID
**/
VOID UEFICallSetupFormCallBackForm(UINT16 PageID, UINTN Action )
{
//Implementation of FormBrowser with actions support 
	UINT32	page,CurrentControlID = 0;
	PAGE_INFO *WorkingPageInfo = NULL;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    
	gBrowserCallbackEnabled = TRUE;

	if (NULL != gApplicationData)
	{
		SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] In UEFICallSetupFormCallBackForm for page %d with Action %d", PageID, Action );
		
		for ( page = 0; ((page < gPages->PageCount) && (Status != EFI_SUCCESS)); page++ ) // Rotate the loop till the page is.
		{
			PAGE_INFO *pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[page]);

			if (NULL == pageInfo || 0 == pageInfo->PageHandle || pageInfo->PageHandle == (VOID*)(UINTN)0xffff)
				continue;

			if(pageInfo->PageID != PageID) // keep on going until the page is matching
				continue;

			WorkingPageInfo = (PAGE_INFO *)EfiLibAllocateZeroPool(sizeof(PAGE_INFO));
			MemCopy(WorkingPageInfo, pageInfo, sizeof(PAGE_INFO));// Copy of PageInfo before callback

			do{
				Status = UEFICallSetupFormCallBackFormExt(pageInfo,Action,&CurrentControlID);
				if(Status == EFI_NOT_READY)// process the page once again
				{
					SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE]In UEFICallSetupFormCallBackForm(), Page Info was updated find new page %d", WorkingPageInfo->PageID );
					pageInfo = FindNewPageInfo(WorkingPageInfo);
				}
				else //everything is fine, reset controlID
					CurrentControlID = 0;
			}while(Status == EFI_NOT_READY);

			MemFreePointer ((VOID **)&WorkingPageInfo);
	    }
	}

	gBrowserCallbackEnabled = FALSE;
	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackForm Exiting()\n");
}
/**
    The function which calls the UEFICallSetupFormCallBackForm() for the pages which are added newly one by one.     

    @param NewFormsetHandle, Action 

    @retval VOID
**/
VOID UEFICallSetupFormCallBackFormForNewFormset(EFI_HANDLE NewFormsetHandle,UINTN Action)
{
	UINT32	page;
	UINT32	PageCount = gPages->PageCount;
	UINT32  CurrentControlID = 0;

	PAGE_LIST *WorkingPageList = NULL;  // Copy of gPages on function entry
	PAGE_INFO *WorkingAppData = NULL;   // copy gApplicationData on function entry
	UINT8 *CurrentAppData = gApplicationData; // back up of gpages ptr -> can be updated during pack update.

	WorkingAppData = (PAGE_INFO *)EfiLibAllocateZeroPool(PageListOffset+FirstPageOffset+PageInfoOffset);
	MemCopy(WorkingAppData, gApplicationData, PageListOffset+FirstPageOffset+PageInfoOffset);
	WorkingPageList = (PAGE_LIST *) WorkingAppData;
	
	gBrowserCallbackEnabled = TRUE;

	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackFormForNewFormset() Entering: \n");
	if (NULL != gApplicationData)
	{
		// go through the previously saved WorkingPageInfo 
		for ( page = 0; page < PageCount; page++ )
		{
			PAGE_INFO *pageInfo;
			PAGE_INFO *WorkingPageInfo = (PAGE_INFO *)((UINTN)WorkingAppData + WorkingPageList->PageList[page]);
			SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackExt page %d of %d", page, PageCount );
			
			if(NULL == WorkingPageInfo || 0 == WorkingPageInfo->PageHandle || WorkingPageInfo->PageHandle == (VOID*)(UINTN)0xffff)
				continue;  // non recoverable, go to next

			if(WorkingPageInfo->PageHandle != NewFormsetHandle) // Only for new formset pages
				continue;
			
			if((gSkipCallbackListCount>1) && !IsCallbackForPageAllowed(Action,WorkingPageInfo))
			    continue;// Don't allow callback with current Action for this Page

			if (CurrentAppData == gApplicationData) // the page order is the same, so can use gpages directly
			{	
				pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[page]);
			}		
			else // gPages was updated so the page has to be found again
			{
				SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] Page Info was updated find new page %d", page );
				pageInfo = FindNewPageInfo(WorkingPageInfo);			
			}
			if (NULL == pageInfo || 0 == pageInfo->PageHandle || pageInfo->PageHandle == (VOID*)(UINTN)0xffff) 
				continue; // if the page is still bad, continue
			//try calling the callback
			if (EFI_NOT_READY == UEFICallSetupFormCallBackFormExt(pageInfo, Action, &CurrentControlID))
			{
				SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackFormExt returned: page %d, CurrentControlID %d", page, CurrentControlID );
				page--; // process the page once again
			}
			else //everything is fine, reset controlID
			{	
				CurrentControlID = 0;
			}
		} // end for page loop
	} 
	gBrowserCallbackEnabled = FALSE;
	MemFreePointer ((VOID **)&WorkingAppData);
	SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] UEFICallSetupFormCallBackFormForNewFormset() Exiting: \n");
}

/**
    The Wrapper function to get the actual action for the driver   	

    @param Action 

    @retval EFI_BROWSER_ACTION 
**/
EFI_BROWSER_ACTION UefiGetActionWapper(UINTN Action)
{
	if(Action == AMI_CALLBACK_CONTROL_UPDATE)
	  return EFI_BROWSER_ACTION_CHANGING;
	  
	if(Action == AMI_CALLBACK_RETRIEVE)
	  return EFI_BROWSER_ACTION_RETRIEVE;
	  
	if(Action == AMI_CALLBACK_FORM_OPEN)
	  return EFI_BROWSER_ACTION_FORM_OPEN;
	  
	if(Action == AMI_CALLBACK_SUBMITTED)
	  return EFI_BROWSER_ACTION_SUBMITTED;

	if(Action == AMI_CALLBACK_FORM_CLOSE)
	  return EFI_BROWSER_ACTION_FORM_CLOSE;

	if(Action == AMI_CALLBACK_FORM_DEFAULT_MANUFACTURING)
	  return  EFI_BROWSER_ACTION_DEFAULT_MANUFACTURING;// Making the specify setup items departing from F2/F3 effect.

	if(Action == AMI_CALLBACK_FORM_DEFAULT_STANDARD)
	  return  EFI_BROWSER_ACTION_DEFAULT_STANDARD;
	  
	return Action;

}
/**
    This function calls Action EFI_BROWSER_ACTION_FORM_OPEN and EFI_BROWSER_ACTION_RETRIEVE for the pages related to New Formset   	

    @param VOID 

    @retval VOID 
**/
VOID CallBrowserActionsForNewHandles()
{
	UINTN i;
	
	if(!gNewHandleCount)
		return; // return if no new Formset added.
	
	for(i = 0; i < gNewHandleCount; i++)
	{
		SETUP_DEBUG_UEFI_CALLBACK ( "\n[TSE] Giving callback with action EFI_BROWSER_ACTION_FORM_OPEN and EFI_BROWSER_ACTION_RETRIEVE for new formset : 0x%x \n",gNewHandleList[i]);
		UEFICallSetupFormCallBackFormForNewFormset(gNewHandleList[i], EFI_BROWSER_ACTION_FORM_OPEN);
		UEFICallSetupFormCallBackFormForNewFormset(gNewHandleList[i], EFI_BROWSER_ACTION_RETRIEVE);
	}
	ClearNewHandleList();
}
/**
    The function to clear gNewHandleList.

    @param Action 

    @retval EFI_BROWSER_ACTION 
**/
VOID ClearNewHandleList()
{
	if(gNewHandleList)
	{
		MemFreePointer((VOID**)&gNewHandleList);
		gNewHandleList = NULL;
	}
	gNewHandleCount = 0;	
}
