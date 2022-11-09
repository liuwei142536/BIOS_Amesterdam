//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file CtrlCond.c

**/


//---------------------------------------------------------------------------
#include "minisetup.h"
#include "CtrlCond.h"
#include "TseUefiHii.h"
//---------------------------------------------------------------------------

extern VOID EvaluateReadWrite(UINT8 *buf,CONTROL_INFO *ControlInfo,EFI_HII_VALUE *);
extern BOOLEAN IsControlWithDisableIfOpcode(CONTROL_INFO *controlInfo);
BOOLEAN IsControlUnderCurrentOpcodeScope(CONTROL_INFO *ControlInfo, VOID* OpHdr);
BOOLEAN IsParentControlDisabled(PAGE_INFO *PageInfo, UINTN ControlIndex);
UINTN _SkipExpression(UINT8 *ControlCondPtr);
extern AMI_POST_MANAGER_PROTOCOL 	*mPostMgr;
extern UINT16 gDisableFlag;
extern EFI_GUID gEdkiiIfrBitVarstoreGuid;
/**
    check control flags

    @param ControlAccess 

    @retval UINT8
**/
UINT8 CheckControlAccess(UINT32 ControlAccess)
{
  UINT8 Cond = COND_NONE;

  if(gPasswordType  == AMI_PASSWORD_USER)
  {
    switch(ControlAccess)
    {
    case CONTROL_ACCESS_ADMIN:  // suppress
      Cond = COND_SUPPRESS;
      break;
    case CONTROL_ACCESS_USER: // grayout (read-only)
      Cond = COND_GRAYOUT;
      break;
    default:
      break;
    }
  }

  return Cond;
}

/**

    @param FinalCond UINT8 Cond

    @retval UINT8
**/
UINT8 UpdateFinalCondition(UINT8 FinalCond, UINT8 Cond)
{
  switch(FinalCond)
  {
  case COND_NONE:
    return(Cond);
    break;

  case COND_NOSUBMIT:    
	return(Cond);
	break;
  case COND_SUPPRESS:
  case COND_DISABLE:
  case COND_HIDDEN:
      if(Cond == COND_DISABLE)
          return (Cond);
      else
          return(FinalCond);
    break;

  case COND_GRAYOUT:
    if(Cond != COND_NONE)
      return(Cond);
    break;

  case COND_INCONSISTENT:
  case COND_WARN_IF:
    return(Cond);
    break;

  }
  return(FinalCond);
}
extern EFI_STATUS _SetValueFromQuestionId(UINT16 QuestionId, UINT16 PageId, EFI_HII_VALUE *Value);
/**

    @param controlInfo 

    @retval UINT8
**/
UINT8 CheckControlCondition( CONTROL_INFO *controlInfo )
{
  EFI_STATUS status = EFI_SUCCESS;
  CONTROL_INFO *tempControlInfo = NULL;
  UINT32 CtrlAccess;
  UINT8 *CondPtr;
  UINT8 *CtrlPtr;
  UINT8 Cond = COND_NONE, FinalCond = COND_NONE;
  UINTN controlInfoLength = 0;
  UINTN offset = 0;

  UINT8 condStack[50];
  UINT8 opcodeStack[50];

  INTN stackIndex = 0;
  INTN inScope = 0;
  EFI_HII_VALUE Value;
  EFI_HII_VALUE Value1;

  BOOLEAN Inside = TRUE;
  BOOLEAN IsControlUnderDisableIfScope = FALSE;
  
  CondPtr    = (UINT8 *)controlInfo->ControlConditionalPtr;
  CtrlAccess = controlInfo->ControlFlags.ControlAccess;

  CtrlPtr = (UINT8 *)controlInfo->ControlPtr;
  EfiZeroMem (&Value1, sizeof (EFI_HII_VALUE));
//  EfiZeroMem (&Value, sizeof (EFI_HII_VALUE));
//Read and Write evaluation code is moved to separate functions. 
//Since there is no need of evaluating read and write in check control condition.

  if ( CondPtr == NULL )
  {  
    FinalCond = CheckControlAccess(CtrlAccess) ;
    if(FinalCond != COND_NONE)
        return FinalCond ;
    goto DONE ;
  }
  else if(CtrlPtr && CondPtr >= CtrlPtr)// to avoid ending up in dead loop
      goto DONE;

  MemSet(&Value, sizeof(EFI_HII_VALUE),0);
  MemSet(&condStack, 50, 0);
  MemSet(&opcodeStack, 50, 0);

  controlInfoLength = sizeof(CONTROL_INFO);
  tempControlInfo = (CONTROL_INFO*)EfiLibAllocateZeroPool(controlInfoLength);
  if (NULL == tempControlInfo) 
    return FinalCond;

  MemCopy(tempControlInfo, controlInfo, controlInfoLength);

  //skipping statements that were already evaluated for disabledif
  if( (((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_DISABLE_IF_OP)&&(gDisableFlag != EvaluateDisableOpControls) )
  {   
      offset = _SkipExpression(tempControlInfo->ControlConditionalPtr);
      FinalCond = COND_NONE;
      Value.Value.b = FALSE;
  }
  else       
  {
	  status = EvaluateExpression(tempControlInfo, &Value, &offset);
	  if(EFI_ERROR(status))
	  {
	      offset = _SkipExpression(tempControlInfo->ControlConditionalPtr);
	      FinalCond = COND_NONE;
	      Value.Value.b = FALSE;
	  }
  }
  if(Value.Value.b)
  {
    if((((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_SUPPRESS_IF_OP) && !((gDisableFlag == EvaluateDisableOpControls) && IsControlWithDisableIfOpcode(controlInfo)))
    {
        FinalCond = COND_SUPPRESS;
        Inside = FALSE;
    }
    if(((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_DISABLE_IF_OP)
    {
        if(gDisableFlag == EvaluateDisableOpControls)
            FinalCond = COND_DISABLE;
        else
            FinalCond = COND_NONE;
        Inside = FALSE;
    }
    if(((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_INCONSISTENT_IF_OP)
    {
      FinalCond = COND_INCONSISTENT;
      Inside = FALSE;
    }
		if(((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_NO_SUBMIT_IF_OP)
		{
			FinalCond = COND_NOSUBMIT;
			Inside = FALSE;
		}
		if(((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == TSE_EFI_IFR_WARNING_IF_OP)
		{
			FinalCond = COND_WARN_IF;
			Inside = FALSE;
		}
		
  }

  while( Inside )
  {
    switch(*(CondPtr))
    {
    case EFI_IFR_DISABLE_IF_OP:
      if(gDisableFlag == EvaluateDisableOpControls)
      {
          Cond = COND_DISABLE;
          IsControlUnderDisableIfScope = IsControlUnderCurrentOpcodeScope(controlInfo, CondPtr);
      }
      else
          Cond = COND_NONE;
      break;
    case EFI_IFR_SUPPRESS_IF_OP:
      Cond = COND_SUPPRESS;
      break;
    case EFI_IFR_GRAY_OUT_IF_OP:
      Cond = COND_GRAYOUT;
      break;

    case EFI_IFR_INCONSISTENT_IF_OP:
      Cond = COND_INCONSISTENT;
	  Inside = FALSE;
      break;
	case EFI_IFR_NO_SUBMIT_IF_OP:
	  Cond = COND_NOSUBMIT;
	  Inside = FALSE;
      break;
	case TSE_EFI_IFR_WARNING_IF_OP:
	  Cond = COND_WARN_IF;
	  Inside = FALSE;
	  break;
    case EFI_IFR_END_OP:
      if(stackIndex)
      {
        condStack[--stackIndex] = 0;
        opcodeStack[stackIndex] = 0;
      }
      offset = ((EFI_IFR_OP_HEADER*)CondPtr)->Length;
      break;
    case EFI_IFR_FORM_OP:
    case EFI_IFR_FORM_MAP_OP:
        offset += ((EFI_IFR_OP_HEADER*)CondPtr)->Length;
        break;
    case EFI_IFR_GUID_OP:
            if (EfiCompareGuid ((EFI_GUID *)((UINT8*)CondPtr + sizeof (EFI_IFR_OP_HEADER)), &gEdkiiIfrBitVarstoreGuid)) {
               controlInfo->ControlFlags.BitFieldVarstore = TRUE;
            }  
      offset += ((EFI_IFR_OP_HEADER*)CondPtr)->Length;
	  break;
    default:
      if(CondPtr == (UINT8*)controlInfo->ControlPtr)
      {
        Inside = FALSE;
      }else if(((EFI_IFR_OP_HEADER*)CondPtr)->OpCode != EFI_IFR_END_OP)
      {
        //Skip this Control
        inScope = 0;
        do
        {
          if(((EFI_IFR_OP_HEADER*)((UINT8*)CondPtr + offset))->Scope)
            inScope++;
          offset += ((EFI_IFR_OP_HEADER*)((UINT8*)CondPtr + offset))->Length;
          if(((EFI_IFR_OP_HEADER*)((UINT8*)CondPtr + offset))->OpCode == EFI_IFR_END_OP)
            inScope--;
        }while(inScope > 0);
      }
      break;
    }
    if(((EFI_IFR_OP_HEADER*)CondPtr)->Scope)
    {
      if(Value.Value.b)
      {
        condStack[stackIndex] = Cond;
      }else
      {
        condStack[stackIndex] = COND_NONE;
      }
      opcodeStack[stackIndex++] = ((EFI_IFR_OP_HEADER*)CondPtr)->OpCode;
      
      if(IsControlUnderDisableIfScope && Value.Value.b) // If DisableIf condition over the control results TRUE, stop processing nested statement or expression under the scope of Disableif.
      {
          Inside = FALSE;
          continue;
      }
    }

    tempControlInfo->ControlConditionalPtr = (VOID *)((UINT8 *)tempControlInfo->ControlConditionalPtr + offset);
    CondPtr = (UINT8*)tempControlInfo->ControlConditionalPtr;
    offset = 0;
    Value.Value.b = 0;
    IsControlUnderDisableIfScope = FALSE;
    if((((EFI_IFR_OP_HEADER*)CondPtr)->OpCode == EFI_IFR_DISABLE_IF_OP)&&(gDisableFlag != EvaluateDisableOpControls))
    {
        offset = _SkipExpression(tempControlInfo->ControlConditionalPtr);
        FinalCond = COND_NONE;
        Value.Value.b = FALSE;
    }
    else       
    {
        status = EvaluateExpression(tempControlInfo, &Value, &offset);
        if(EFI_ERROR(status))
        {
            offset = _SkipExpression(tempControlInfo->ControlConditionalPtr);
            FinalCond = COND_NONE;
            Value.Value.b = FALSE;
        }
     }
  }

  while(stackIndex--)
  {
    Cond = condStack[stackIndex];
    FinalCond = UpdateFinalCondition(FinalCond,Cond);
  }

  // if CtrlAccess is CONTROL_ACCESS_DEFAULT then do not override VFR conditions
  if(CONTROL_ACCESS_DEFAULT != CtrlAccess)
  {
    FinalCond = CheckControlAccess(CtrlAccess);
  }

  if(tempControlInfo)
  {
    MemFreePointer((void**)&tempControlInfo);
  }

DONE:

  if( (FinalCond == COND_NONE) && 
      (controlInfo->ControlFlags.ControlReadOnly == 1) && 
      IsReadOnlyGrayout() )
    {
        FinalCond = COND_GRAYOUT ;
    } 
	
	FinalCond = OEMCheckControlCondition(FinalCond,controlInfo);// provide the Support to override the control Condition checking() generically. And provide customization to supress the controls with Security GUIDed opcode.
	return FinalCond;
}

/**
    checks Inconsistence

    @param PAGE_INFO

    @retval BOOLEAN
**/
BOOLEAN CheckInconsistence( PAGE_INFO *pPageInfo )
{
	CONTROL_INFO *pControlInfo;
	UINTN i;
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_EVENT Event = NULL;
	CHAR16 *Title = NULL,*Message = NULL;

	Status = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL,(void**) &mPostMgr);
	if((Status == EFI_SUCCESS) && (mPostMgr != NULL))
	{
		//Find out if there is inconsistent value in any of the controls
		for(i=0; i < pPageInfo->PageControls.ControlCount; i++)
		{
			pControlInfo = (CONTROL_INFO*)((UINT8 *)(gControlInfo) + pPageInfo->PageControls.ControlList[i]);
			//Check if there is a CONTROL_TYPE_MSGBOX in this page
			if(pControlInfo->ControlType == CONTROL_TYPE_MSGBOX)
			{
			    //Check the previous Control is Disabled
			    if(IsParentControlDisabled(pPageInfo,i))
			        continue;

			    //check for warning condition
				if((CheckControlCondition(pControlInfo) == COND_WARN_IF) && (pControlInfo->ControlFlags.ControlEvaluateDefault == 0))
				{
					pControlInfo->ControlFlags.ControlEvaluateDefault = 1;
	          		Title = HiiGetString( 	gHiiHandle, STRING_TOKEN(STR_WARNING));
	          		Message = HiiGetString( pControlInfo->ControlHandle, ((TSE_EFI_IFR_WARNING_IF*)pControlInfo->ControlConditionalPtr)->Warning);
					Status = mPostMgr->DisplayInfoBox(
							Title,
							Message,
							((TSE_EFI_IFR_WARNING_IF*)pControlInfo->ControlConditionalPtr)->TimeOut, 
							&Event
							);
	          		MemFreePointer( (VOID **)&Title );
	          		MemFreePointer( (VOID **)&Message );
				}
				else if( (CheckControlCondition(pControlInfo) != COND_WARN_IF) && (pControlInfo->ControlFlags.ControlEvaluateDefault == 1))
					pControlInfo->ControlFlags.ControlEvaluateDefault = 0;
				if(CheckControlCondition(pControlInfo) == COND_INCONSISTENT)
				{
	          		Title = HiiGetString( gHiiHandle, STRING_TOKEN(STR_INCONSISTENT_MSG_TITLE));
	          		Message = HiiGetString( pControlInfo->ControlHandle, ((EFI_IFR_INCONSISTENT_IF*)pControlInfo->ControlConditionalPtr)->Error);
	          		mPostMgr->DisplayMsgBox( 
	          				Title, 
	          				Message , 
	          				MSGBOX_TYPE_OK,
	          				NULL
	          			);
	          		MemFreePointer( (VOID **)&Title );
	          		MemFreePointer( (VOID **)&Message );
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
/**
    checks NoSubmitIf

    @param VOID

    @retval BOOLEAN
**/
BOOLEAN CheckNoSubmitIf( VOID)
{
	UINT32 index = 0;
    EFI_STATUS Status = EFI_SUCCESS;
	// Get the Forms
	for ( index = 0; index < gPages->PageCount; index++ )
	{
		PAGE_INFO *pageInfo = (PAGE_INFO*)((UINT8 *)gPages + gPages->PageList[index]);
		UINTN control = 0;

		if(pageInfo->PageHandle == NULL)
		{
			continue;
		}	
			// Get the Controls in each Page
		for(control = 0; control < pageInfo->PageControls.ControlCount; control++)
		{
			CONTROL_INFO *pControlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + pageInfo->PageControls.ControlList[control]);			
		
			if(pControlInfo->ControlType == NO_SUBMIT_IF)			
			{
			    if(IsParentControlDisabled(pageInfo,control))
			        continue;
			    
			    if(CheckControlCondition(pControlInfo) == COND_NOSUBMIT){
                    //return DrawMessageBox(pControlInfo, STRING_TOKEN(STR_NOSUBMITIF_MSG_TITLE), ((EFI_IFR_NO_SUBMIT_IF*)pControlInfo->ControlConditionalPtr)->Error);							
                    Status = gBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL,(void**) &mPostMgr);
                    if((Status == EFI_SUCCESS) && (mPostMgr != NULL))
                    {
                   		CHAR16 *Title = NULL,*Message = NULL;
                   		Title = HiiGetString( gHiiHandle, STRING_TOKEN(STR_NOSUBMITIF_MSG_TITLE));
                   		Message = HiiGetString( pControlInfo->ControlHandle, ((EFI_IFR_NO_SUBMIT_IF*)pControlInfo->ControlConditionalPtr)->Error);
                   		mPostMgr->DisplayMsgBox( 
                   				Title, 
                   				Message , 
                   				MSGBOX_TYPE_OK,
                   				NULL
                   			);
                   		MemFreePointer( (VOID **)&Title );
                   		MemFreePointer( (VOID **)&Message );                   	
                        return TRUE;
                    }
                }
			}
		}		
	}
	return FALSE;
}

/**

    @param ParentCtrlInfo UINT8* CtrlCondPtr

    @retval UINT8
**/
UINT8 CheckOneOfOptionSuppressIf(CONTROL_INFO *ParentCtrlInfo, UINT8* CtrlCondPtr, UINT8* CtrlPtr)
{
  CONTROL_INFO *newCtrlInfo = NULL;
  UINT8 condition = COND_NONE;

  newCtrlInfo = (CONTROL_INFO*)EfiLibAllocateZeroPool(sizeof(CONTROL_INFO));
  if(newCtrlInfo == NULL)
  {
    goto DONE;
  }

  newCtrlInfo->ControlHandle = ParentCtrlInfo->ControlHandle;
  newCtrlInfo->ControlPageID = ParentCtrlInfo->ControlPageID;
  newCtrlInfo->ControlKey = ParentCtrlInfo->ControlKey;
  newCtrlInfo->ControlConditionalPtr = (VOID *)CtrlCondPtr;
  newCtrlInfo->ControlPtr = (VOID *)CtrlPtr;
  newCtrlInfo->ControlVariable = ParentCtrlInfo->ControlVariable;
  condition = CheckControlCondition(newCtrlInfo);
  MemFreePointer((void**)&newCtrlInfo);

DONE:

  return condition;
}

/**

    @param CtrlInfo 

    @retval UINT64
**/
UINT16 EvaluateControlDefault(CONTROL_INFO *CtrlInfo, UINT64 *Defaults)
{
    CONTROL_INFO *newCtrlInfo = NULL;
    EFI_IFR_OP_HEADER *header = (EFI_IFR_OP_HEADER*)CtrlInfo->ControlPtr;
    EFI_IFR_OP_HEADER *OpHeader = NULL;
    EFI_HII_VALUE value;
    UINT16 size = 0;
    UINT32 scope = 0;
    UINTN offset = 0;
    EFI_STATUS Status = EFI_SUCCESS;
    EfiZeroMem (&value, sizeof (EFI_HII_VALUE));		//If Undefined value returned from EvaluateExpression Junk value is updated, hence filling 0s
    
    newCtrlInfo = (CONTROL_INFO*)EfiLibAllocateZeroPool(sizeof(CONTROL_INFO));
    if(newCtrlInfo == NULL)
        return size;
    
    MemCopy(newCtrlInfo, CtrlInfo, sizeof(CONTROL_INFO));

        
    while(header->OpCode != EFI_IFR_DEFAULT_OP)
    {
        switch(header->OpCode)
        {
            case EFI_IFR_END_OP:
                if ( scope)
                {
                    --scope;
                }
                else
                {
                    scope = 0;
                }
                break;
            default:
                break;
        }
        
        if ( header->Scope)  
            ++scope;
        
        header = (EFI_IFR_OP_HEADER*)((UINT8*)header + header->Length);
        
        if (!scope)
            goto DONE;
    }
    
    newCtrlInfo->ControlConditionalPtr = (VOID *)((UINT8 *)header + header->Length);
    // Set End of Default Scope as ControlPtr
    do
    {
        switch(header->OpCode)
        {
            case EFI_IFR_END_OP:
            //      scope = scope? --scope : 0;
                if ( scope)
                {
                    --scope;
                }
                else
                {
                    scope = 0;
                }
            
                break;
            default:
                break;
        }
    //    scope = header->Scope? ++scope : scope;
        if ( header->Scope)  ++scope ;
        header = (EFI_IFR_OP_HEADER*)((UINT8*)header + header->Length);
    }while(scope > 0);

    newCtrlInfo->ControlPtr = (VOID *)header;
     
    
    Status = EvaluateExpression(newCtrlInfo, &value, &offset);
    if(EFI_ERROR(Status))
         goto DONE;
    if (CtrlInfo->ControlType == CONTROL_TYPE_ORDERED_LIST) 
    {
        OpHeader = (EFI_IFR_OP_HEADER*)CtrlInfo->ControlPtr;
        while(OpHeader->OpCode != EFI_IFR_ONE_OF_OPTION_OP && OpHeader->OpCode != EFI_IFR_END_OP)
        {
            OpHeader = (EFI_IFR_OP_HEADER*)((UINT8*)OpHeader + OpHeader->Length);
        }

        if (OpHeader->OpCode == EFI_IFR_ONE_OF_OPTION_OP)
            size = (UINT8) (1 << (((EFI_IFR_ONE_OF_OPTION *)OpHeader)->Type & EFI_IFR_NUMERIC_SIZE));
    }
    else 
    {
        size = (UINT16)GetControlDataLength(CtrlInfo);
    }
  
    //For String controls size will be length of string, so that should not be set to default value
    if(CONTROL_TYPE_POPUP_STRING == CtrlInfo->ControlType || CONTROL_TYPE_PASSWORD == CtrlInfo->ControlType)
        MemCpy(Defaults, &(value.Value), sizeof(UINT16));
    else
        MemCpy(Defaults, &(value.Value), size);
    
  
DONE:
    MemFreePointer((void**)&newCtrlInfo);
    return size;
}

extern VOID EvaluateReadWrite(UINT8 *buf,CONTROL_INFO *ControlInfo,EFI_HII_VALUE *);
extern EFI_STATUS _SetValueFromQuestionId(UINT16 QuestionId, UINT16 PageId, EFI_HII_VALUE *Value);
VOID HandleWriteOpOnChanged(CONTROL_INFO *ControlInfo)
{
    EFI_HII_VALUE Value1;
    UINT8 *CtrlPtr;
    UINTN ScopeCount = 0;
    UINTN   i = 0;
    EFI_IFR_OP_HEADER *opHeader= (EFI_IFR_OP_HEADER *) NULL;

    CtrlPtr = (UINT8 *)ControlInfo->ControlPtr;
    EfiZeroMem (&Value1, sizeof (EFI_HII_VALUE));

	//Evaluating Write opcode for a current control when the control value is edited.
    if(ControlInfo->ControlFlags.ControlRWEvaluate)
    {
        do
        {
            opHeader = (EFI_IFR_OP_HEADER*)(CtrlPtr + i);
            switch(opHeader->OpCode)
            {
            case EFI_IFR_READ_OP:
                break;
            case EFI_IFR_WRITE_OP:
                EvaluateReadWrite((CtrlPtr + i + opHeader->Length),ControlInfo,&Value1);
                break;
            case EFI_IFR_END_OP:
                if(ScopeCount)
                {
                    ScopeCount--;
                }
                break;
            }
            if(opHeader->Scope)
            {
                ScopeCount++;
            }
            i += opHeader->Length ;
        }while(ScopeCount);
    }
}
VOID HandleReadOpOnRetrieve()
{
    UINT32  page = 0, control = 0;
    
    EFI_HII_VALUE Value1;
    UINT8 *CtrlPtr;
    UINTN ScopeCount = 0;
    UINTN   i = 0;
    EFI_IFR_OP_HEADER *opHeader= (EFI_IFR_OP_HEADER *) NULL;
    
	//Evaluating Read opcode for all the control in all the pages after AMI_CALLBACK_RETRIEVE callback done while load previous values
    for ( page = 0; page < gPages->PageCount; page++ )
    {
        PAGE_INFO *pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[page]);

        if(pageInfo->PageHandle ==0)
            continue;

        for ( control = 0; control < pageInfo->PageControls.ControlCount; control++ )
        {
            CONTROL_INFO *controlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + pageInfo->PageControls.ControlList[control]);
            if ( NULL == controlInfo->ControlPtr )
                continue;
            
            CtrlPtr = (UINT8 *)controlInfo->ControlPtr;
            EfiZeroMem (&Value1, sizeof (EFI_HII_VALUE));
            
            if(controlInfo->ControlFlags.ControlRWEvaluate)
            {
                i = 0;
                opHeader = (EFI_IFR_OP_HEADER *) NULL;
                do
                {
                 opHeader = (EFI_IFR_OP_HEADER*)(CtrlPtr + i);
                  switch(opHeader->OpCode)
                  {
                      case EFI_IFR_READ_OP:
                            EvaluateReadWrite((CtrlPtr + i + opHeader->Length),controlInfo,&Value1);
                            if(Value1.Type != EFI_IFR_TYPE_UNDEFINED){
                               _SetValueFromQuestionId(controlInfo->ControlKey,controlInfo->ControlPageID,&Value1);
                            }
                            break;
                      case EFI_IFR_WRITE_OP:
                          break;
                      case EFI_IFR_END_OP:
                          if(ScopeCount)
                          {
                              ScopeCount--;
                          }
                          break;
                  }
                  if(opHeader->Scope)
                  {
                      ScopeCount++;
                  }
                  i += opHeader->Length ;
                }while(ScopeCount);
            }
        }
    }
}

VOID HandleReadOpOnChanged(UINT32 CurrentPage)
{
    UINT32  control = 0;
    
    EFI_HII_VALUE Value1;
    UINT8 *CtrlPtr;
    UINTN ScopeCount = 0;
    UINTN   i = 0;
    EFI_IFR_OP_HEADER *opHeader= (EFI_IFR_OP_HEADER *) NULL;
    
	//Evaluating Read opcode for all the controls in current page to update to the latest edited value.
    PAGE_INFO *pageInfo = (PAGE_INFO *)((UINTN)gApplicationData + gPages->PageList[CurrentPage]);

    if(pageInfo->PageHandle ==0)
        return;

    for ( control = 0; control < pageInfo->PageControls.ControlCount; control++ )
    {
        CONTROL_INFO *controlInfo = (CONTROL_INFO *)((UINTN)gControlInfo + pageInfo->PageControls.ControlList[control]);
        if ( NULL == controlInfo->ControlPtr )
            continue;

        CtrlPtr = (UINT8 *)controlInfo->ControlPtr;
        EfiZeroMem (&Value1, sizeof (EFI_HII_VALUE));

        if(controlInfo->ControlFlags.ControlRWEvaluate)
        {
            i = 0;
            opHeader = (EFI_IFR_OP_HEADER *) NULL;
            do
            {
                opHeader = (EFI_IFR_OP_HEADER*)(CtrlPtr + i);
                switch(opHeader->OpCode)
                {
                case EFI_IFR_READ_OP:
                    EvaluateReadWrite((CtrlPtr + i + opHeader->Length),controlInfo,&Value1);
                    if(Value1.Type != EFI_IFR_TYPE_UNDEFINED){
                        _SetValueFromQuestionId(controlInfo->ControlKey,controlInfo->ControlPageID,&Value1);
                    }
                    break;
                case EFI_IFR_WRITE_OP:
                    break;
                case EFI_IFR_END_OP:
                    if(ScopeCount)
                    {
                        ScopeCount--;
                    }
                    break;
                }
                if(opHeader->Scope)
                {
                    ScopeCount++;
                }
                i += opHeader->Length ;
            }while(ScopeCount);
        }
    }
}

BOOLEAN IsControlUnderCurrentOpcodeScope(CONTROL_INFO *ControlInfo, VOID* OpHdr)
{
    UINTN ScopeCount = 0;
    EFI_IFR_OP_HEADER *OpHeader = (EFI_IFR_OP_HEADER *)OpHdr;
    
    if(!OpHdr || !ControlInfo->ControlPtr || OpHdr >= ControlInfo->ControlPtr)
        return FALSE;
        
    do
    {
        if(OpHeader->Scope)
            ScopeCount++;
        
        if((void*)OpHeader == ControlInfo->ControlPtr) // Control inside OpHdr's scope.
            return TRUE;
        
        if(OpHeader->OpCode == EFI_IFR_END_OP)
            ScopeCount--;
        
        OpHeader = (EFI_IFR_OP_HEADER *)((UINT8*)OpHeader + OpHeader->Length);
    }while((ScopeCount != 0) && OpHeader->Length);
        
    return FALSE;
}

BOOLEAN IsParentControlDisabled(PAGE_INFO *PageInfo, UINTN ControlIndex)
{
    CONTROL_INFO *ControlInfo = (CONTROL_INFO *)NULL;
    INTN Index;
    BOOLEAN IsParentDisabled = FALSE;
    
    if(!PageInfo || !ControlIndex || ControlIndex > PageInfo->PageControls.ControlCount)
        return FALSE;
    
    for(Index = (INTN)ControlIndex - 1; Index >= 0 ; Index--)
    {
        ControlInfo = (CONTROL_INFO*)((UINT8 *)(gControlInfo) + PageInfo->PageControls.ControlList[Index]);
        
        if(ControlInfo->ControlType != CONTROL_TYPE_MSGBOX && ControlInfo->ControlType != NO_SUBMIT_IF)
        {
            if(!ControlInfo->ControlFlags.ControlVisible)
                IsParentDisabled = TRUE;
            break;
        }
    }
    
    return IsParentDisabled;
}

