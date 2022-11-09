//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file variable.c
    This file contains code to handle variable operations

**/

#include "minisetup.h"

extern UINTN gSetupCount;
extern UINTN gSetupStatus;  

//---------------------------------------------------------------------------
// Variables
//---------------------------------------------------------------------------
NVRAM_VARIABLE *gCurrNvramVarList = (NVRAM_VARIABLE *)NULL;
UINTN gCurrNvramVarCount = 0;

CHAR8 *gPrevLanguage;
UINTN gPrevLangSize = 0;
BOOLEAN gSetupUpdated = FALSE;
BOOLEAN gTseCacheUpdated = FALSE;
extern CHAR16  gCachePlatformLang[20];
#if SETUP_OEM_SPECIAL_CONTROL_SUPPORT
EFI_STATUS OEMSpecialUpdateOneOf(UINT16 newOption,UINT32 *offset,UINTN *size, VOID OUT **buffer);
#endif // SETUP_OEM_SPECIAL_CONTROL_SUPPORT
BOOLEAN   IsTseCachePlatformLang (VOID);
UINT8   IsBootOptionsGroupingEnabled (void);
BOOLEAN IsBBSOptionsMatch(BBS_ORDER_TABLE *DefaultLegDevOrder,BBS_ORDER_TABLE *LegacyDevOrder) ;
#if TSE_DEBUG_MESSAGES
extern VOID DebugShowPrintVariableDetails(UINT16 Start, UINT16 End);
extern EFI_STATUS DebugShowPrintOptimalDefaults(UINT32 varIndex);
#endif
UINT32  EFIAPI BitFieldRead32(
    IN      UINT32                    Operand,
    IN      UINTN                     StartBit,
    IN      UINTN                     EndBit
);

#define EDKII_IFR_NUMERIC_SIZE_BIT       0x3F

/**
    Gets all system variables

    @param RetNvramVarList Returns pointer to the var list

    @retval EFI_STATUS status - EFI_SUCCESS if successful, else EFI_ERROR
**/
EFI_STATUS GetNvramVariableList(NVRAM_VARIABLE **RetNvramVarList)
{
    EFI_STATUS status = EFI_SUCCESS;
    NVRAM_VARIABLE *nvVarlist = (NVRAM_VARIABLE *)NULL;
    UINT32 index = 0;

    if ( (gCurrNvramVarList == NULL) || (gVariables->VariableCount != gCurrNvramVarCount) || (gPostStatus <= TSE_POST_STATUS_ENTERING_TSE))
	{
        gCurrNvramVarList = EfiLibAllocateZeroPool(gVariables->VariableCount * sizeof(NVRAM_VARIABLE));
        if(gCurrNvramVarList == NULL)
		{
            status = EFI_OUT_OF_RESOURCES;
            goto DONE;
        }

        // Get the Current Values from the NVRAM
        nvVarlist = gCurrNvramVarList;
        for(index = 0; index < gVariables->VariableCount; index++, nvVarlist++){
            nvVarlist->Buffer = VarGetNvram(index, &nvVarlist->Size);
            if ((0 != nvVarlist->Size) && (NULL == nvVarlist->Buffer))
            {
        	    nvVarlist->Buffer = EfiLibAllocateZeroPool (nvVarlist->Size);
            }
        }
        gCurrNvramVarCount = gVariables->VariableCount;
    }
    status = CopyNvramVariableList(gCurrNvramVarList, RetNvramVarList);
    
DONE:
    return status;
}

/**
    Clean up the current retrive variable list

    @param VOID

    @retval VOID
**/
VOID CleanTempNvramVariableList()
{
    NVRAM_VARIABLE *nvVarlist = (NVRAM_VARIABLE *)NULL;
    UINT32 index = 0;

    if(gCurrNvramVarList != NULL){
        nvVarlist = gCurrNvramVarList;
        for(index = 0; index < gCurrNvramVarCount; index++, nvVarlist++){
            MemFreePointer((VOID**)&nvVarlist->Buffer);
        }        
    }
    gCurrNvramVarCount = 0;
    MemFreePointer((void**)&gCurrNvramVarList);
    gCurrNvramVarList = (NVRAM_VARIABLE *)NULL;
}

/**
    Clean up the current retrive variable list

    @param VOID

    @retval VOID
**/ //For memleak
VOID CleanNvramVariableList(NVRAM_VARIABLE **InvVarlist)
{
    UINT32 index = 0;
    NVRAM_VARIABLE *nvVarlist = *InvVarlist;
    
    if(nvVarlist != NULL){
        for(index = 0; index < gVariables->VariableCount; index++, nvVarlist++){
            MemFreePointer((VOID**)&nvVarlist->Buffer);
        }        
    }
    MemFreePointer((void**)InvVarlist);
}

/**
    Copies variable buffer from one var list to another var list

    @param SrcVarList Source variable list
    @param DestVarList Destination variable list

    @retval EFI_STATUS status - EFI_SUCCESS if successful, else EFI_ERROR
**/
EFI_STATUS CopyNvramVariableList(NVRAM_VARIABLE *SrcVarList, NVRAM_VARIABLE **DestVarList)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINT32 index = 0;
    NVRAM_VARIABLE *varPtr = (NVRAM_VARIABLE *)NULL;
    NVRAM_VARIABLE *currVarPtr = (NVRAM_VARIABLE *)NULL;

    *DestVarList = (NVRAM_VARIABLE *)EfiLibAllocateZeroPool(gVariables->VariableCount * sizeof(NVRAM_VARIABLE));
    if(*DestVarList == NULL){
        status = EFI_OUT_OF_RESOURCES;
        goto DONE;
    }

    varPtr = *DestVarList;
    currVarPtr = SrcVarList;
    if(currVarPtr == NULL)
    {
        status = EFI_INVALID_PARAMETER;
        goto DONE;  
    }
    for(index = 0; index < gVariables->VariableCount; index++, varPtr++, currVarPtr++)
    {
        if((varPtr->Buffer == NULL) && (currVarPtr->Size != 0))
        {
            varPtr->Buffer = EfiLibAllocatePool(currVarPtr->Size);
            if(varPtr->Buffer == NULL){
                status = EFI_OUT_OF_RESOURCES;
                goto DONE;
            } else{
        	    if (currVarPtr->Buffer)
        	    {
        	            //Update the buffered value for DynamicPageCount from the cache.
        	            if(index == VARIABLE_ID_DYNAMIC_PAGE_COUNT)
        	            {
        	            	if(varPtr->Buffer) //For MemoryLeak Fix[Memory will reallocated inside the VarGetValue for the dynamic page count] 
        	            		MemFreePointer((void **)&varPtr->Buffer);
        	            	VarGetValue( VARIABLE_ID_DYNAMIC_PAGE_COUNT, 0, sizeof(UINT16), currVarPtr->Buffer );    
        	            }
        	            if (varPtr->Buffer)
        	            {
        	        	    MemCopy( varPtr->Buffer, currVarPtr->Buffer, currVarPtr->Size );
        	            }
        	    }
                varPtr->Size = currVarPtr->Size;
            }
        }
    }
DONE:
    return status;
}

/**
    function to load the variables

    @param list NVRAM_VARIABLE *defaultList

    @retval status

**/
EFI_STATUS VarLoadVariables( VOID **list, NVRAM_VARIABLE *defaultList )
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINT32 index;
#if !TSE_NVRAM_DEFAULTS_SUPPORT
	UINTN size;
	VARIABLE_INFO *varInfo = NULL;
#endif
	NVRAM_VARIABLE *varPtr;
	NVRAM_VARIABLE *defaultVar = defaultList;
    UINTN BackUpSetupStatus = gSetupStatus;
    gSetupStatus  = TSE_SETUP_STATUS_LOAD_VARIABLES;
    
	if(*list != NULL)// If the list is null then clearing the cache values and updaitng the cache with new list //For memleak
	{
		CleanNvramVariableList( (NVRAM_VARIABLE **)list );
		CleanTempNvramVariableList();
	}

	if( ( ! gVariables ) || ( gVariables->VariableCount == 0 ) )	// fatal error
		while(1);

	Status = GetNvramVariableList((NVRAM_VARIABLE **)list);
    if(EFI_ERROR(Status)){
        goto DONE;//Performance Improving of variable data load and usage
    }

	varPtr = *list;
	for ( index = 0; index < gVariables->VariableCount; index++, varPtr++, defaultVar++ )
	{
		if ( ( varPtr->Buffer == NULL ) && ( defaultList != NULL ) && ( defaultVar->Size != 0 ) )
		{
#if !TSE_NVRAM_DEFAULTS_SUPPORT
            varInfo = (VARIABLE_INFO *)((UINT8 *)gVariables + gVariables->VariableList[index]);
		    if(varInfo && varInfo->VariableSize){
		        varPtr->Buffer = EfiLibAllocateZeroPool( varInfo->VariableSize );
                size = varInfo->VariableSize;
		    }
		    else{
		        varPtr->Buffer = EfiLibAllocateZeroPool( defaultVar->Size );
                size = defaultVar->Size;
		    }
#else	    
            varPtr->Buffer = EfiLibAllocatePool( defaultVar->Size );
#endif
		    if ( varPtr->Buffer == NULL )
				Status = EFI_OUT_OF_RESOURCES;
			else
			{
				MemCopy( varPtr->Buffer, defaultVar->Buffer, defaultVar->Size );
#if !TSE_NVRAM_DEFAULTS_SUPPORT
				varPtr->Size = size;
#else
                varPtr->Size =defaultVar->Size;
#endif			
			}
		}

		if ( varPtr->Buffer == NULL ) // If varPtr->Buffer is still NULL Skip the variable.
			continue;		

        //Introduce DISABLED_BOOT_OPTION in place of disabled options
        //for mem copy of BootOrder and BBSOrder
        if(gBootData && (VARIABLE_ID_BOOT_ORDER == index))
        {
            UINT16 *BootOrder = (UINT16 *)varPtr->Buffer;
            BOOT_DATA *pBootData;
            UINTN i;

            for(i=0; i<(varPtr->Size / sizeof(UINT16)); i++)
            {
                pBootData = BootGetBootData(BootOrder[i]); //Should yield a valid option
				if(NULL != pBootData)			//Suppress the warnings from static code analyzer
				{
					if ( (!(pBootData->Active & LOAD_OPTION_ACTIVE)) && (!(pBootData->Active & LOAD_OPTION_HIDDEN)) )
                    	BootOrder[i] = DISABLED_BOOT_OPTION;
				}
            }
        }
        //Introduce DISABLED_DRIVER_OPTION in place of disabled options
        //Change the DriverOrder cache
        else if (gDriverData && (VARIABLE_ID_DRIVER_ORDER == index))		// Support for driver order
        {
            UINT16 *DriverOrder = (UINT16 *)varPtr->Buffer;
            BOOT_DATA *pDriverData;
            UINTN i;

            for (i = 0; i < (varPtr->Size / sizeof (UINT16)); i++)
            {
				pDriverData = DriverGetDriverData (DriverOrder[i]); //Should yield a valid option
				if (pDriverData)
				{
					if ( (!(pDriverData->Active & LOAD_OPTION_ACTIVE)) && (!(pDriverData->Active & LOAD_OPTION_HIDDEN)) )
						DriverOrder [i] = DISABLED_DRIVER_OPTION;
				}
            }
        }
        else if(VARIABLE_ID_BBS_ORDER == index)
        {
            BBS_ORDER_TABLE *TypeEntry, *TSEDevOrder = (BBS_ORDER_TABLE *) varPtr->Buffer;
            UINTN i,j,count;

            TypeEntry = TSEDevOrder;
            for(i=0; i < varPtr->Size;)
            {
					if ( ( TypeEntry->Length >= varPtr->Size) || (0 == TypeEntry->Length) ) 
					{
               	break;
					}

					count = TypeEntry->Length / sizeof(UINT16) - 1;

                for(j=0; j < count; j++)
                {
                    if(TypeEntry->Data[j] & BBS_ORDER_DISABLE_MASK)
                        TypeEntry->Data[j] = DISABLED_BOOT_OPTION;
                }
        
                count = sizeof(UINT32) + TypeEntry->Length;
        		TypeEntry = (BBS_ORDER_TABLE *)((UINTN)TypeEntry + count);
        		i += count;
            }
        }
	}
#if TSE_DEBUG_MESSAGES
	if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
	{
	    SETUP_DEBUG_TSE("\n\n[TSE] Loaded NVRAM/Previous values into cache\nVariables List: \n");
	    DebugShowPrintVariableDetails((UINT16)0,(UINT16)gVariables->VariableCount);
	}
#endif
DONE:
    gSetupStatus = BackUpSetupStatus;
	return Status;
}

/**
    function to build the default variables

    @param VOID

    @retval status

**/
EFI_STATUS VarBuildDefaults( VOID )
{
	EFI_STATUS Status;

	UINT8 *newBuffer = NULL;
	UINT8 *DefDrvBuffer = NULL;
	UINTN size = 0;

	Status = VarBuildAMIDefaults();

	if(Status == EFI_UNSUPPORTED)
	{
		Status = HiiLoadDefaults( (VOID **)&gFailsafeDefaults,(UINT32) HiiGetManufactuingMask() ); // Allocates Space and filles
		if ( EFI_ERROR( Status ) )
			return Status;
	
		Status = HiiLoadDefaults( (VOID **)&gOptimalDefaults, (UINT32) HiiGetDefaultMask() ); // Allocates Space and filles
		if ( EFI_ERROR( Status ) )
			return Status;

	}

	// reset default values of BBS order variable.
	if ( gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer != NULL ) {
		MemFreePointer( (VOID **) &gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer );
	}
	if ( gOptimalDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer != NULL ) {				//  Support for driver order
		MemFreePointer( (VOID **) &gOptimalDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer );
	}
	if ( gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer != NULL ) { // Memory leak Fix- Free it before overwritting with NULL
		MemFreePointer( (VOID **) &gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer );
	}
	gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer = gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer = gOptimalDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer = NULL;
	gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Size = gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Size = gOptimalDefaults[ VARIABLE_ID_DRIVER_ORDER ].Size = 0;

	if ( gFailsafeDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer != NULL ) {
		MemFreePointer( (VOID **) &gFailsafeDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer );
	}
	if ( gFailsafeDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer != NULL ) {
		MemFreePointer( (VOID **) &gFailsafeDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer );
	}
	if ( gFailsafeDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer != NULL ) { // Memory leak Fix- Free it before overwritting with NULL
		MemFreePointer( (VOID **) &gFailsafeDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer );
	}

	gFailsafeDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer = gFailsafeDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer = gFailsafeDefaults[ VARIABLE_ID_DRIVER_ORDER ].Buffer = NULL;
	gFailsafeDefaults[ VARIABLE_ID_BBS_ORDER ].Size = gFailsafeDefaults[ VARIABLE_ID_BOOT_ORDER ].Size = gFailsafeDefaults[ VARIABLE_ID_DRIVER_ORDER ].Size = 0;

	// special provision for language
	//VarGetValue( VARIABLE_ID_LANGUAGE, 0, 3, gPrevLanguage );
	gPrevLanguage = VarGetNvram ( VARIABLE_ID_LANGUAGE, &gPrevLangSize );

    //Special provision for "Boot order" and "Legacy dev order"
	size = 0;
	newBuffer = VarGetNvramName ( L"DefaultBootOrder", &EfiDefaultBootOrderGuid, NULL, &size );
	if(newBuffer)
	{
		MemFreePointer( (VOID **) &gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer );
		gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Buffer = (UINT8 *)newBuffer;
		gOptimalDefaults[ VARIABLE_ID_BOOT_ORDER ].Size = size;
	}
	size = 0;				//Default driver order support
	DefDrvBuffer = VarGetNvramName (L"DefaultDriverOrder", &EfiDefaultDriverOrderGuid, NULL, &size);
	if (DefDrvBuffer)
	{
		MemFreePointer ((VOID **) &gOptimalDefaults [VARIABLE_ID_DRIVER_ORDER].Buffer);
		gOptimalDefaults [VARIABLE_ID_DRIVER_ORDER].Buffer = (UINT8 *)DefDrvBuffer;
		gOptimalDefaults [VARIABLE_ID_DRIVER_ORDER].Size = size;
	}	
	CsmLoadDefaultLegDevOrder();
	LoadedBuildDefaultsHook();
#if TSE_DEBUG_MESSAGES
	if((gDbgPrint & PRINT_UEFI_VAR) == PRINT_UEFI_VAR)
	{
	    UINT32 Index;
	    for(Index = 0 ; Index < gVariables->VariableCount; Index++)
	        DebugShowPrintOptimalDefaults(Index);
	}
#endif	
	
	
	return Status;
}
/**
    Function to load default legacy boot order

    @param VOID

    @retval VOID

**/
VOID LoadDefaultLegDevOrder(VOID)
{
	UINT8 *newBuffer = NULL;
	UINTN size = 0;

	newBuffer = VarGetNvramName( L"DefaultLegacyDevOrder", &EfiDefaultLegacyDevOrderGuid, NULL, &size );
	if(newBuffer)
	{
        MemFreePointer( (VOID **)&gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer );
		gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer = BBSReOrderDefultLegacyDevOrder((BBS_ORDER_TABLE *)newBuffer,size);
		//Ignore the DefaultLegacyDevOrder if is size is not matching with LegacyDevOder.
		if(gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Buffer == NULL)
			size = 0;
		gOptimalDefaults[ VARIABLE_ID_BBS_ORDER ].Size = size;
		MemFreePointer((VOID**)&newBuffer);
	}
}

/**
    function to get the NvRam varible names

    @param variable UINTN *size

    @retval void

**/
VOID *VarGetNvram( UINT32 variable, UINTN *size )
{
	VOID *buffer = NULL;
	VARIABLE_INFO *varInfo;
	EFI_STATUS status = EFI_SUCCESS;

	*size = 0;

	varInfo = VarGetVariableInfoIndex( variable );
	if ( varInfo == NULL )
		return buffer;

	if((varInfo->ExtendedAttibutes & AMI_SPECIAL_VARIABLE_NO_GET) == AMI_SPECIAL_VARIABLE_NO_GET)
	{
		// Don't read from the NVRAM
		// But in the Init we may need to take care return empty buffer
		NVRAM_VARIABLE *list = gVariableList;
		NVRAM_VARIABLE *nvVar;
		if(gVariableList != NULL)
		{
			nvVar = &list[ variable ];
			if((nvVar!=NULL)&&(nvVar->Size!=0))
			{
				*size = nvVar->Size;
				//This function Suppose allocate and give the buffer
				buffer = EfiLibAllocateZeroPool( *size );
				if(nvVar->Buffer != NULL)
					MemCopy( buffer, nvVar->Buffer, *size );
				return buffer;
			}
		}
		else if(varInfo->VariableSize)// Create memory for Fake variables since it won't present in NVRAM
		{
		    *size = varInfo->VariableSize;
		    buffer = EfiLibAllocateZeroPool( *size );
		    return buffer;
		}
		
		// If Local catch is not found Just return null.
		*size = 0;
		return buffer;
	}

	if(UefiIsEfiVariable(variable,varInfo))
	{
		*size = 0;
    	if(varInfo->VariableAttributes == 0)
      		varInfo->VariableAttributes = 0x07;
       	if( (variable == VARIABLE_ID_LANGUAGE) && IsTseCachePlatformLang())
        	{
       		if(*gCachePlatformLang)
       		{
       			*size=(EfiStrLen(gCachePlatformLang)*sizeof(CHAR16));
       			buffer = EfiLibAllocateZeroPool(*size+sizeof(CHAR16));
       			EfiCopyMem (buffer, gCachePlatformLang, *size);
       		}
       		else
       		{
       			CHAR16  *Language =NULL;
       			Language=VarGetNvramName( varInfo->VariableName, &varInfo->VariableGuid, &varInfo->VariableAttributes, size );
       			if(NULL != Language)
       			{
       				EfiZeroMem (gCachePlatformLang, sizeof (gCachePlatformLang));	
       				EfiCopyMem (gCachePlatformLang, Language, *size);
       				MemFreePointer ((VOID **)&Language);
       			}
       		}
        	}
        	else
			buffer = VarGetNvramName( varInfo->VariableName, &varInfo->VariableGuid, &varInfo->VariableAttributes, size );
	}
	else
	{
		{
			if((varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_VARSTORE) == VARIABLE_ATTRIBUTE_VARSTORE)
				*size = varInfo->VariableSize;
			status = UefiVarGetNvram(varInfo, &buffer, VAR_ZERO_OFFSET, *size);
			if(EFI_ERROR(status))
			{
				*size = 0;
			}
		}
	}
	return buffer;
}

/**
    Gets the question value buffer from a specific offset of a
    variable buffer

    @param variable 
    @param offset 
    @param size 

    @retval VOID *buffer - Question value buffer
**/
VOID *VarGetNvramQuestionValue(UINT32 variable, UINT32 Offset, UINTN Size)
{
    VOID *buffer = (VOID *)NULL;
    VARIABLE_INFO *varInfo = (VARIABLE_INFO *)NULL;
    EFI_STATUS status = EFI_SUCCESS;

    varInfo = VarGetVariableInfoIndex( variable );
    if ( varInfo == NULL || Offset == 0xFFFF || ((varInfo->ExtendedAttibutes & AMI_SPECIAL_VARIABLE_NO_GET ) == AMI_SPECIAL_VARIABLE_NO_GET ))
    	return buffer;
// Minisetup: Memory leaks in text browser
    if(UefiIsEfiVariable(variable,varInfo))
    {
        UINTN size = 0;
        UINT8 *tempBuffer = NULL;
   
		buffer = EfiLibAllocateZeroPool(Size+1);
   		if(buffer == NULL)
    	{
     		status = EFI_OUT_OF_RESOURCES;
      		goto DONE;
    	}

        if(varInfo->VariableAttributes == 0){
            varInfo->VariableAttributes = 0x07;
        }
        tempBuffer = VarGetNvramName( varInfo->VariableName, &varInfo->VariableGuid, &varInfo->VariableAttributes, &size );
        if(tempBuffer != NULL && size != 0)
        {
            if((Offset + Size) > size)
            {
                status = EFI_INVALID_PARAMETER;
                goto DONE;
            }
            MemCopy(buffer, &tempBuffer[Offset], Size);
            MemFreePointer((VOID**)&tempBuffer);
        }
    }
    else
    {        
        //VARIABLE_ATTRIBUTE_NAMEVALUE
        if((varInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE) == VARIABLE_ATTRIBUTE_NAMEVALUE)
        {
            if(Size > varInfo->VariableSize)
            {
                status = EFI_INVALID_PARAMETER;
                goto DONE;
            }
            status = UefiVarGetNvram(varInfo, &buffer, 0, Size);  // Offset is always 0 for namevalue variables
        }else{
            if((Offset + Size) > varInfo->VariableSize)
            {
                status = EFI_INVALID_PARAMETER;
                goto DONE;
            }
            status = UefiVarGetNvram(varInfo, &buffer, Offset, Size);
        }


    }
DONE:
if ( ! EFI_ERROR( status ) )  
{
    return buffer;
}
else
	return NULL; 
}

/**
    function to set the NvRam varible names

    @param variable VOID *buffer, UINTN *size

    @retval status
**/
EFI_STATUS VarSetNvram( UINT32 variable, VOID *buffer, UINTN size )
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    VARIABLE_INFO *varInfo = (VARIABLE_INFO *)NULL;

    if(buffer == NULL)
    {
        goto DONE;
    }

    varInfo = VarGetVariableInfoIndex(variable);
    if(varInfo == NULL)
    {
        goto DONE;
    }

	if((varInfo->ExtendedAttibutes & AMI_SPECIAL_VARIABLE_NO_SET) == AMI_SPECIAL_VARIABLE_NO_SET)
	{
		// Don't save the variable
		Status = EFI_SUCCESS;
	}
	else
	{
		if(UefiIsEfiVariable(variable,varInfo))
		{
			Status = VarSetNvramName( varInfo->VariableName, &varInfo->VariableGuid, varInfo->VariableAttributes, buffer, size );
			if( ! EFI_ERROR( Status ) && (variable == VARIABLE_ID_LANGUAGE) && IsTseCachePlatformLang())
			{
				if(*gCachePlatformLang)
				{
					EfiZeroMem (gCachePlatformLang,sizeof (gCachePlatformLang));
					EfiCopyMem (gCachePlatformLang, buffer, size);
				}
			}
		}
		else
		{
			Status = UefiVarSetNvram(varInfo, buffer, VAR_ZERO_OFFSET, size);
		}
	}
	if ( ! EFI_ERROR( Status ) )
    {
        VarUpdateVariable(variable);
    }

DONE:
    return Status;
}

/**
    function to get the real value of the variable

    @param variable UINTN offset, UINTN size, VOID *buffer

    @retval status

**/
EFI_STATUS _VarGetRealValue( UINT32 variable, UINTN offset, UINTN size, VOID *buffer )
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN i;

	switch ( variable )
	{
		case VARIABLE_ID_LANGUAGE:
			{
                UINTN varsize=0;
        		VOID * LangBuf = VarGetNvram( variable, &varsize ); // Read the Variable from Variable Service
				for ( i = 0; i < gLangCount; i++ )
				{
					if ( ! EfiCompareMem( LangBuf, gLanguages[i].LangCode, varsize ) )
					{
						MemSet( buffer, size, 0 );
						*(UINT16 *)buffer = (UINT16)i;
						break;
					}
				}
        		MemFreePointer(&LangBuf);
        	}
			break;

        default:
			break;
	}

	return Status;
}

/**
    function to set the real value of the variable

    @param variable UINT32 *offset, UINTN *size,
    @param buffer VOID **realBuffer

    @retval status

**/
EFI_STATUS _VarSetRealValue( UINT32 variable, UINT32 *offset, UINTN *size, VOID *buffer, VOID **realBuffer )
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINT16 index = *(UINT16 *)buffer;

	if ( variable == VARIABLE_ID_SETUP )
	{
		Status = EFI_UNSUPPORTED;
		return Status;
	}
	if (VARIABLE_ID_BOOT_ORDER == variable)
		*size = gBootOptionCount * sizeof(UINT16);
	
	if (VARIABLE_ID_DRIVER_ORDER == variable)				// Support for driver order
		*size = gDriverOptionCount * sizeof(UINT16);
	
	*realBuffer = EfiLibAllocateZeroPool( *size );
	if ( *realBuffer == NULL )
	{
		Status = EFI_OUT_OF_RESOURCES;
		return Status;
	}

	switch ( variable )
	{
		case VARIABLE_ID_LANGUAGE:
			{
				if ( index > gLangCount )
					Status = EFI_INVALID_PARAMETER;
				else
				{
					///Removed the Token SETUP_SUPPORT_PLATFORM_LANG_VAR dependency from TSE sources
					if( PlatformLangVerSupport() ) {	
				        	MemFreePointer( (VOID **)realBuffer );
				        	*size = StrLen8(gLanguages[index].LangCode) +1;
				        	*realBuffer = EfiLibAllocateZeroPool( *size );
					}
					else {
				        	*size = 3;
					}

					MemCopy( *realBuffer, gLanguages[index].LangCode, *size );
				}
			}
			break;
		case VARIABLE_ID_BOOT_ORDER:
   			MemFreePointer ((VOID **)realBuffer);
			BootUpdateOrder (buffer, offset, size, realBuffer);
			break;
		case VARIABLE_ID_DRIVER_ORDER:								// Support for driver order
			MemFreePointer ((VOID **)realBuffer);
			DriverUpdateOrder (buffer, offset, size, realBuffer);
			break;
        case VARIABLE_ID_BBS_ORDER:
   			MemFreePointer( (VOID **)realBuffer );
			//BBSUpdateOrder( /*index*/buffer, offset, size, realBuffer );
			Status = CsmBBSUpdateOrder(buffer, offset, size, realBuffer);
            break;

#if SETUP_OEM_SPECIAL_CONTROL_SUPPORT
        case VARIABLE_ID_OEM_TSE_VAR:
   			MemFreePointer( (VOID **)realBuffer );
			Status = OEMSpecialUpdateOneOf( index, offset, size, realBuffer );
            break;
#endif

		default:
			MemFreePointer( (VOID **)realBuffer );
			Status = EFI_INVALID_PARAMETER;
			break;
	}

	return Status;
}
/**
    Function to get var value

    @param variable UINT32 offset, UINTN size, VOID *buffer

    @retval STATUS

**/
EFI_STATUS VarGetValue( UINT32 variable, UINT32 offset, UINTN size, VOID *buffer )
{
	return _VarGetData( variable, offset, size, buffer, FALSE );
}
/**
    Function to get var defaults

    @param variable UINT32 offset, UINTN size, VOID *buffer

    @retval STATUS
**/
EFI_STATUS VarGetDefaults( UINT32 variable, UINT32 offset, UINTN size, VOID *buffer )
{
	return _VarGetData( variable, offset, size, buffer, TRUE );
}

/**
    function to get variable data

    @param variable UINT32 offset, UINTN size,
    @param buffer BOOLEAN useDefaults

    @retval status
**/
EFI_STATUS _VarGetData( UINT32 variable, UINT32 offset, UINTN size, VOID *buffer, BOOLEAN useDefaults )
{
	UINT32 TempOffset =offset;
    EFI_STATUS Status = EFI_SUCCESS;

	NVRAM_VARIABLE *list = gVariableList;

	if ( ( useDefaults ) || ( list == NULL ) )
		list = gOptimalDefaults;

	Status = _VarGetSetValue( VAR_COMMAND_GET_VALUE, list, variable, TempOffset, size, buffer );

	if ( ! EFI_ERROR( Status ) )
		Status = _VarGetRealValue( variable, offset,size, buffer );

	return Status;
}

/**
    Function to set variable value

    @param variable UINT32 offset, UINTN size, VOID *buffer
					

    @retval status
**/
EFI_STATUS VarSetValue( UINT32 variable, UINT32 offset, UINTN size, VOID *buffer )
{
	EFI_STATUS Status = EFI_SUCCESS;
	VOID *realBuffer = (VOID *)NULL;
	VARIABLE_INFO *VarInfo = VarGetVariableInfoIndex( variable ); 

	Status = _VarSetRealValue( variable, &offset, &size, buffer, &realBuffer );
	if ( EFI_ERROR( Status ) )
		realBuffer = buffer;

	Status = _VarGetSetValue( VAR_COMMAND_SET_VALUE, gVariableList, variable, offset, size, realBuffer );

	if ( ( variable == VARIABLE_ID_LANGUAGE ) && ( ! EFI_ERROR( Status ) ) )
	{
		if (NULL != gApp)
		{
        gApp->CompleteRedraw = TRUE;
		}
		Status = VarSetNvram( variable, realBuffer, size );
	}

    if ( realBuffer != buffer )
		MemFreePointer( (VOID **)&realBuffer );
	
    //Prevent calls to SetupConfigModifiedHook for variables that don't affect setup changes. 
    if(!(
    		//variable == VARIABLE_ID_SETUP			    	||
    		//variable == VARIABLE_ID_LANGUAGE		    	||
    		//variable == VARIABLE_ID_BOOT_TIMEOUT	   		 ||
    		//variable == VARIABLE_ID_USER_DEFAULTS	   		 ||
    		variable == VARIABLE_ID_ERROR_MANAGER	   		 ||
    		//variable == VARIABLE_ID_AMITSESETUP      		 ||
    		//variable == VARIABLE_ID_IDE_SECURITY      		 ||
    		//variable == VARIABLE_ID_BOOT_ORDER       		 ||
    		//variable == VARIABLE_ID_BBS_ORDER           	 ||
    		variable == VARIABLE_ID_DEL_BOOT_OPTION     	 ||
    		variable == VARIABLE_ID_ADD_BOOT_OPTION     	 ||
    		variable == VARIABLE_ID_BOOT_MANAGER        	 ||
    		variable == VARIABLE_ID_BOOT_NOW            	 ||
    		variable == VARIABLE_ID_LEGACY_DEV_INFO     	 ||
    		variable == VARIABLE_ID_AMI_CALLBACK        	 ||
    		variable == VARIABLE_ID_LEGACY_GROUP_INFO   	 ||
    		variable == VARIABLE_ID_OEM_TSE_VAR		    	 ||
    		variable == VARIABLE_ID_DYNAMIC_PAGE_COUNT		 ||
    		variable == VARIABLE_ID_DRV_HLTH_ENB			 ||
    		variable == VARIABLE_ID_DRV_HLTH_COUNT			 ||
    		variable == VARIABLE_ID_DRIVER_MANAGER			 ||
    		//variable == VARIABLE_ID_DRIVER_ORDER			 ||
    		variable == VARIABLE_ID_ADD_DRIVER_OPTION   	 ||
    		variable == VARIABLE_ID_DEL_DRIVER_OPTION   	 ||
    		variable == VARIABLE_ID_PORT_OEM1				 ||
    		variable == VARIABLE_ID_PORT_OEM2				 ||
    		variable == VARIABLE_ID_PORT_OEM3				 ||
    		variable == VARIABLE_ID_PORT_OEM4				 ||
    		variable == VARIABLE_ID_PORT_OEM5		||
    		variable == VARIABLE_ID_PORT_OEM6 ||
            variable == VARIABLE_ID_PORT_OEM7 ||
            variable == VARIABLE_ID_PORT_OEM8 ||
            variable == VARIABLE_ID_PORT_OEM9 ||
            variable == VARIABLE_ID_PORT_OEM10 ||
            variable == VARIABLE_ID_PORT_OEM11 ||
            variable == VARIABLE_ID_PORT_OEM12 ||
            variable == VARIABLE_ID_PORT_OEM13 ||
            variable == VARIABLE_ID_PORT_OEM14 ||
            variable == VARIABLE_ID_PORT_OEM15 ||
            variable == VARIABLE_ID_PORT_OEM16 || 
            variable == VARIABLE_ID_PORT_OEM17 ||
            variable == VARIABLE_ID_PORT_OEM18 ||
            variable == VARIABLE_ID_PORT_OEM19 ||
            variable == VARIABLE_ID_PORT_OEM20 ||
            variable == VARIABLE_ID_PORT_OEM21 ||
            variable == VARIABLE_ID_PORT_OEM22 ||
            variable == VARIABLE_ID_PORT_OEM23 ||
            variable == VARIABLE_ID_PORT_OEM24 ||
            variable == VARIABLE_ID_PORT_OEM25 ||
            variable == VARIABLE_ID_PORT_OEM26 ||
            variable == VARIABLE_ID_PORT_OEM27 || 
            variable == VARIABLE_ID_PORT_OEM28 ||
            variable == VARIABLE_ID_PORT_OEM29 ||
    		variable == VARIABLE_ID_DYNAMIC_PAGE_GROUP		||
    		variable == VARIABLE_ID_DYNAMIC_PAGE_DEVICE		||
    		variable == VARIABLE_ID_DYNAMIC_PAGE_GROUP_CLASS ||
    		variable == SETUP_SAVE_STATE_INFO_KEY_ID ||
    		(VarInfo && ((VarInfo->ExtendedAttibutes & AMI_SPECIAL_VARIABLE_NO_SET ) == AMI_SPECIAL_VARIABLE_NO_SET )) )) 
    	{
    		/// Hook can be used after a control is modified...
    		SetupConfigModifiedHook();
         gSetupUpdated = TRUE;//Flag is Set when any of the setup variable is set
         gTseCacheUpdated = TRUE;
    	}

	

	return Status;
}

/**
    intermediate function for the set and get operations

    @param command NVRAM_VARIABLE *list, UINT32 variable,
    @param offset UINTN size, VOID *buffer

    @retval status
**/
EFI_STATUS _VarGetSetValue( UINTN command, NVRAM_VARIABLE *list, UINT32 variable, UINT32 offset, UINTN size, VOID *buffer )
{
	EFI_STATUS Status = EFI_INVALID_PARAMETER;
	NVRAM_VARIABLE *varInfo;
    VARIABLE_INFO *VariableInfo = (VARIABLE_INFO *)NULL;
    
	if ( ( variable >= gVariables->VariableCount ) || ( buffer == NULL ) || ( size == 0 ) )
		return Status;

	varInfo = &list[ variable ];
	VariableInfo = VarGetVariableInfoIndex( variable );
	if (!VariableInfo)
	{
		return Status;
	}
	
	if(offset == 0xFFFF) // Update offset to 0 if Storage control have no VarId in Vfr, by default Vfr compiler gives 0xFFFF for this case. 
	{					//Match using FakeVariable since fake varialbes will be created only for Storage control with no VarId
	    EFI_GUID FakeVariableGuid = TSE_FAKE_VARIABLE_GUID;
	    if(EfiCompareGuid(&VariableInfo->VariableGuid,&FakeVariableGuid))
	    {
	        offset  = 0;
	    }
	    else
	    {
	        return Status;
	    }
	}

	if (VARIABLE_ATTRIBUTE_NAMEVALUE == (VariableInfo->ExtendedAttibutes & VARIABLE_ATTRIBUTE_NAMEVALUE))
    {
        offset  = 0;
    }
    
	if ( ( command == VAR_COMMAND_SET_VALUE ) && ( varInfo->Buffer == NULL ) )
	{
		varInfo->Buffer = EfiLibAllocateZeroPool( offset + size );
		if ( varInfo->Buffer != NULL )
			varInfo->Size = offset + size;
		else
		    return Status;
	}

	if ( varInfo->Size == 0 )
	{
		varInfo->Buffer = VarGetNvram( variable, &varInfo->Size );
		if ( varInfo->Buffer == NULL )
			return Status;
	}

	if ( offset + size > varInfo->Size )
	{
		if ( command == VAR_COMMAND_SET_VALUE )
		{
			UINT8 *newBuffer;

			newBuffer = EfiLibAllocateZeroPool( offset + size );
			if ( newBuffer == NULL )
				return Status;
			MemCopy( newBuffer, varInfo->Buffer, varInfo->Size );
			MemFreePointer( (VOID **)&varInfo->Buffer );
			varInfo->Buffer = newBuffer;
			varInfo->Size = offset + size;
		}
	}

	if ( command == VAR_COMMAND_GET_VALUE )
    {
       	MemSet( buffer, size, 0 );
    	if ( offset + size > varInfo->Size )
            size = varInfo->Size - offset;
    	if ( varInfo->Size > offset )
    		MemCopy( buffer, &varInfo->Buffer[offset], size );
    }
	else
	{	   
	        MemCopy( &varInfo->Buffer[offset], buffer, size );
	}

	Status = EFI_SUCCESS;

	return Status;

}

VOID UefiGetQuestionWidthForBitFieldVariable(VOID *IfrPtr, UINTN *StartBit, UINTN *EndBit, UINTN *Width)
{
    EFI_IFR_OP_HEADER *OpHeader =(EFI_IFR_OP_HEADER *)IfrPtr;
    UINT8     Flags; 
    UINT16    BitOffset;
    UINT16    BitWidth;
    UINT16    TotalBits;

    if (NULL == OpHeader) 
        return ;

    switch(OpHeader->OpCode)
    {
    case EFI_IFR_CHECKBOX_OP:
        BitOffset = ((EFI_IFR_CHECKBOX*)OpHeader)->Question.VarStoreInfo.VarOffset;
        BitWidth = 1; //checkbox can support one bit        
        TotalBits = BitOffset % 8 + BitWidth;
        *Width = (TotalBits % 8 == 0 ? TotalBits / 8: TotalBits / 8 + 1);
        *StartBit = (BitOffset % 8);
        *EndBit = (*StartBit + BitWidth - 1);
        break;
    case EFI_IFR_ONE_OF_OP:
        BitOffset = ((EFI_IFR_ONE_OF*)OpHeader)->Question.VarStoreInfo.VarOffset;
        Flags = ((EFI_IFR_ONE_OF*)OpHeader)->Flags;
        BitWidth = Flags & EDKII_IFR_NUMERIC_SIZE_BIT; 
        TotalBits = BitOffset % 8 + BitWidth;
        *Width = (TotalBits % 8 == 0 ? TotalBits / 8: TotalBits / 8 + 1);
        *StartBit = (BitOffset % 8);
        *EndBit = (*StartBit + BitWidth - 1);
        break;
    case EFI_IFR_NUMERIC_OP:
        BitOffset = ((EFI_IFR_NUMERIC*)OpHeader)->Question.VarStoreInfo.VarOffset;
        Flags = ((EFI_IFR_NUMERIC*)OpHeader)->Flags;
        BitWidth = Flags & EDKII_IFR_NUMERIC_SIZE_BIT; 
        TotalBits = BitOffset % 8 + BitWidth;
        *Width = (TotalBits % 8 == 0 ? TotalBits / 8: TotalBits / 8 + 1);
        *StartBit = (BitOffset % 8);
        *EndBit = (*StartBit + BitWidth - 1);
        break;
      break;
    default:
        break;
    }
}

/**
    intermediate function for the set and get operations

    @param command NVRAM_VARIABLE *list, UINT32 variable,
    @param offset UINTN size, VOID *buffer

    @retval status
**/
EFI_STATUS _VarGetSetValueForBitField( UINTN command, NVRAM_VARIABLE *list, UINT32 variable, UINT32 offset, UINTN size, VOID *buffer,VOID * ControlPtr )
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    NVRAM_VARIABLE *varInfo;
    UINT32 VarValue =0, NewVal =0, ReturnValue = 0;
    UINTN Startbit = 0;
    UINTN Endbit = 0;
    UINTN StorageWidth = 0;

    UINTN RealOffset = offset / 8; //getting real offset from bit offset

    if ( ( variable >= gVariables->VariableCount ) || ( buffer == NULL ) || ( size == 0 ) )
        return Status;
    
    UefiGetQuestionWidthForBitFieldVariable(ControlPtr, &Startbit, &Endbit, &StorageWidth);
    
    varInfo = &list[ variable ];

    if ( varInfo->Size == 0 )
    {
        varInfo->Buffer = VarGetNvram( variable, &varInfo->Size );
        if ( varInfo->Buffer == NULL )
            return Status;
    }
    
    if ( command == VAR_COMMAND_GET_VALUE )
    {
        MemCopy ( (UINT8 *)&VarValue, (UINT8 *)&varInfo->Buffer[RealOffset], StorageWidth);
        ReturnValue = BitFieldRead32 (VarValue, Startbit, Endbit);
        MemCopy((UINT8 *)buffer, (UINT8 *)&ReturnValue, StorageWidth);

        SETUP_DEBUG_VAR("[TSE] BitField GET Var[%d] off: %d, RealOff: %d, start: %d, end: %d, Width: %d, Val: %x \n", 
            variable, offset, RealOffset, Startbit, Endbit, StorageWidth, ReturnValue); 
    }
    else    
    {
        if(size == sizeof(UINT8))
            NewVal = *(UINT8*)buffer;
        else if(size == sizeof(UINT16))
            NewVal = *(UINT16*)buffer;
        else if(size == sizeof(UINT32))
            NewVal = *(UINT32*)buffer;

        MemCopy ( (UINT8 *)&VarValue, (UINT8 *)&varInfo->Buffer[RealOffset], StorageWidth);
        ReturnValue = BitFieldWrite32 (VarValue, Startbit, Endbit, NewVal);
        MemCopy((UINT8 *)&varInfo->Buffer[RealOffset], (UINT8 *)&ReturnValue, StorageWidth);

        SETUP_DEBUG_VAR("[TSE] BitField SET Var[%d] off:%d, RealOff: %d, start: %d, end: %d, Width: %d, New: %x Var: %x, Ret: %x \n", 
            variable, offset, RealOffset, Startbit, Endbit, StorageWidth, NewVal, VarValue, ReturnValue); 
    }

    Status = EFI_SUCCESS;

    return Status;

}

/**

    @param UINT32 variable, UINT32 offset, UINTN size, VOID * ControlPtr

    @retval VOID *buffer
**/
VOID* _VarGetValueForBitFieldNvram(UINT32 variable, UINT32 offset, UINTN size, VOID * ControlPtr )
{
    UINTN tempSize = 0;
    UINT8 *tempBuffer = NULL;
    UINT8 *buffer = NULL;
   
    UINT32 VarValue =0, ReturnValue = 0;
    UINTN Startbit = 0;
    UINTN Endbit = 0;
    UINTN StorageWidth = 0;
    UINTN RealOffset = offset / 8; //getting real offset from bit offset

    if ( ( variable >= gVariables->VariableCount ) || ( size == 0 ) )
        return buffer;

    tempBuffer = VarGetNvram( variable, &tempSize );
    if ( ( tempBuffer == NULL ) || (tempSize < RealOffset) )
        return buffer;

    buffer = EfiLibAllocateZeroPool(size);
    if(buffer == NULL)
    {
        return buffer;
    }
    UefiGetQuestionWidthForBitFieldVariable(ControlPtr, &Startbit, &Endbit, &StorageWidth);
    
    MemCopy ( (UINT8 *)&VarValue, (UINT8 *)&tempBuffer[RealOffset], StorageWidth);
    ReturnValue = BitFieldRead32 (VarValue, Startbit, Endbit);
    MemCopy((UINT8 *)buffer, (UINT8 *)&ReturnValue, StorageWidth);

   SETUP_DEBUG_VAR("[TSE] BitFieldNvram Var[%d] off = %d, RealOff: %d, start: %d, end: %d, Width: %d, Val: %x \n", 
           variable, offset, RealOffset, Startbit, Endbit, StorageWidth, ReturnValue); 

    return buffer;
}

/**

    @param UINT32 variable, UINT32 offset, UINTN size, VOID *buffer, VOID * ControlPtr

    @retval EFI_STATUS
**/
EFI_STATUS _VarSetValueForBitFieldNvram(UINT32 variable, UINT32 offset, UINTN size, VOID *buffer, VOID * ControlPtr )
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    UINTN tempSize = 0;
    UINT8 *tempBuffer = NULL;
    UINT32 VarValue =0, NewVal =0, ReturnValue = 0;
    UINTN Startbit = 0;
    UINTN Endbit = 0;
    UINTN StorageWidth = 0;
    UINTN RealOffset = offset / 8; //getting real offset from bit offset
    VARIABLE_INFO   *varInfo = (VARIABLE_INFO *)NULL;

    if ( ( variable >= gVariables->VariableCount ) || ( size == 0 ) )
        return Status;

    varInfo = VarGetVariableInfoIndex (variable);
    if(varInfo == NULL)
    {
        return Status;
    }

    tempBuffer = VarGetNvram( variable, &tempSize );
    if ( ( tempBuffer == NULL ) || (tempSize < RealOffset) )
        return Status;
    
    
    UefiGetQuestionWidthForBitFieldVariable(ControlPtr, &Startbit, &Endbit, &StorageWidth);
    
    if(size == sizeof(UINT8))
        NewVal = *(UINT8*)buffer;
    else if(size == sizeof(UINT16))
        NewVal = *(UINT16*)buffer;
    else if(size == sizeof(UINT32))
        NewVal = *(UINT32*)buffer;

    MemCopy ( (UINT8 *)&VarValue, (UINT8 *)&tempBuffer[RealOffset], StorageWidth);
    ReturnValue = BitFieldWrite32 (VarValue, Startbit, Endbit, NewVal);
    MemCopy((UINT8 *)&tempBuffer[RealOffset], (UINT8 *)&ReturnValue, StorageWidth);

    if(UefiIsEfiVariable(variable, varInfo))
    {
        VarSetNvramName( varInfo->VariableName, &varInfo->VariableGuid, varInfo->VariableAttributes, tempBuffer, tempSize );
    }
    else
    {
        UefiVarSetNvram(varInfo, tempBuffer, VAR_ZERO_OFFSET, tempSize);
    }
   SETUP_DEBUG_VAR("[TSE] BitFieldNvram SET Var[%d] off: %d, RealOff: %d, start: %d, end: %d, Width: %d, New: %x Var: %x, Val = %x \n", 
            variable, offset, RealOffset, Startbit, Endbit, StorageWidth, NewVal, VarValue, ReturnValue); 

    Status = EFI_SUCCESS;
    return Status;
}

/**
    Function to update variable

    @param nvramVar UINT32 variable
					

    @retval VOID
**/
VOID _VarUpdate(NVRAM_VARIABLE *nvramVar, UINT32 variable)
{
	UINT8 *newBuffer;
	UINTN nvSize;
	
	newBuffer = (UINT8 *)VarGetNvram( variable, &nvSize );
	if (NULL == newBuffer)				//No need to update the buffer if it is NULL
	{
		return;
	}
	MemFreePointer( (VOID **)&nvramVar->Buffer );
	nvramVar->Buffer = newBuffer;
	nvramVar->Size = nvSize;
}
/**
    Function to update variable

    @param variable 
					

    @retval VOID
**/
VOID VarUpdateVariable(UINT32 variable)
{
	NVRAM_VARIABLE 	*nvramVar;
    EFI_STATUS 		Status = EFI_SUCCESS;

    if (NULL == gVariableList) 
    {
        Status = VarLoadVariables ((VOID **)&gVariableList, NULL);
    }
    if (EFI_ERROR (Status))
    {    
        return;
    }
	nvramVar = &gVariableList[ variable ];
	_VarUpdate(nvramVar, variable);
}
/**
    Function to update variable defaults

    @param variable 
					

    @retval VOID
**/
VOID VarUpdateDefaults(UINT32 variable)
{
	NVRAM_VARIABLE *nvramVar;
	nvramVar = &gFailsafeDefaults[ variable ];
	_VarUpdate(nvramVar, variable);
	nvramVar = &gOptimalDefaults[ variable ];
	_VarUpdate(nvramVar, variable);
}
/**
    Function to get variable 

    @param variable UINTN *size 
					

    @retval VOID*
**/
VOID *VarGetVariable( UINT32 variable, UINTN *size )
{
	VOID *buffer = NULL;
	NVRAM_VARIABLE *nvramVar;

	if ( variable >= gVariables->VariableCount )
	{
	    *size = 0;
		return buffer;
	}
	
	if(gVariableList == NULL)
	{
	    *size = 0;
		return buffer;
	}

	nvramVar = &gVariableList[ variable ];

	if ( ( *size == 0 ) || ( *size > nvramVar->Size ) )
		*size = nvramVar->Size;

	if(*size == 0)
		return buffer;

	buffer = EfiLibAllocateZeroPool( *size );
	if ( ( buffer != NULL ) && ( nvramVar->Buffer != NULL ) )
		MemCopy( buffer, nvramVar->Buffer, *size );
	else
		*size = 0;

	return buffer;
}
/**
    Function to get variable info index

    @param index 
					

    @retval VARIABLE_INFO *
**/
VARIABLE_INFO *VarGetVariableInfoIndex( UINT32 index )
{
	VARIABLE_INFO *varInfo = NULL;

	if ( index < gVariables->VariableCount )
		varInfo = (VARIABLE_INFO *)((UINT8 *)gVariables + gVariables->VariableList[index]);

	return varInfo;
}

/**
    function to get the ID of the variable information

    @param command NVRAM_VARIABLE *list, UINT32 variable,
    @param offset UINTN size, VOID *buffer

    @retval Variable Info
**/
VARIABLE_INFO *VarGetVariableInfoId( UINT32 varId, UINT32 *index )
{
	VARIABLE_INFO *varInfo = NULL;
	UINT32 i;

	for ( i = 0; i < gVariables->VariableCount; i++ )
	{
		varInfo = VarGetVariableInfoIndex( i );
		if ( NULL != varInfo && varInfo->VariableID == varId)// Suppress the warnings from static code analyzer
		{
			if ( index != NULL )
				*index = i;
			return varInfo;
		}
	}

	varInfo = NULL;
	return varInfo;
}

/**
    To reoder the DefaultLegacyDevOrder Variable as TSE reorders 
    the LegacyDevOrder based on Boot Order

    @param DefaultLegDevOrder and DefaultLegDevOrderSize

    @retval NewDefultLegacyDevOrder
        NULL = if DefaultLegDevOrderSize is not Equal with LegacuDevOrderSize
**/
VOID *BBSReOrderDefultLegacyDevOrder(BBS_ORDER_TABLE *DefaultLegDevOrder,UINTN DefaultLegDevOrderSize)
{
	BBS_ORDER_TABLE * LegacyDevOrder,*NewDefultLegacyDevOrder,*TempDevOrder,*NewDevEntry, *VarLegacyDevOrder;
	UINTN LegacyOrderSize=0;
	UINTN count=0;
	UINTN i,j;
	
    VarLegacyDevOrder = VarGetNvramName( L"LegacyDevOrder", &gLegacyDevGuid, NULL, &LegacyOrderSize );
    LegacyDevOrder = VarLegacyDevOrder;
    

	// If the DefaultLegacyDevOrderSize is not Matching with LegacyDevOrderSize 
	if(LegacyOrderSize != DefaultLegDevOrderSize)
		return NULL;

	if(DefaultLegDevOrderSize && LegacyDevOrder)
		NewDefultLegacyDevOrder = EfiLibAllocateZeroPool( DefaultLegDevOrderSize);
	else 
		return NULL;

	NewDevEntry = NewDefultLegacyDevOrder;
	for(j=0; j<LegacyOrderSize; )
 	{
		TempDevOrder = DefaultLegDevOrder;
		for(i=0;i<DefaultLegDevOrderSize;)
		{
			if ( (0 == TempDevOrder->Length) || (TempDevOrder->Length >= DefaultLegDevOrderSize) ) 
			{
				return NULL;
			}
			
			count = sizeof(UINT32) + TempDevOrder->Length;
			
			//If GROUP_BOOT_OPTIONS_BY_TAG is enable
			if ( (LegacyDevOrder->Type == TempDevOrder->Type) && (LegacyDevOrder->Length == TempDevOrder->Length) && IsBootOptionsGroupingEnabled() ) //To Restore BootOption default issue
			{
			    if(IsBBSOptionsMatch(TempDevOrder,LegacyDevOrder))
			    {
                    MemCopy(NewDevEntry,TempDevOrder,count); //Copying default legacyDevOrder
                    NewDevEntry = (BBS_ORDER_TABLE *)((UINTN)NewDevEntry + count);							
                    break;
			    }
			}
			//If GROUP_BOOT_OPTIONS_BY_TAG is disable
			else
			{
				if ( (LegacyDevOrder->Type == TempDevOrder->Type) && (LegacyDevOrder->Data[0] == TempDevOrder->Data[0]) )
				{
					MemCopy(NewDevEntry,TempDevOrder,count); //Copying default legacyDevOrder
					NewDevEntry = (BBS_ORDER_TABLE *)((UINTN)NewDevEntry + count);							
					break;
				}
			}

			TempDevOrder = (BBS_ORDER_TABLE *)((UINTN)TempDevOrder + count);
            i += count;
		}

		//Go to next entry
  		count = sizeof(UINT32) + LegacyDevOrder->Length;
    	LegacyDevOrder = (BBS_ORDER_TABLE *)((UINTN)LegacyDevOrder + count);
     	j += count;
	}
	
	if(((UINTN)NewDevEntry - (UINTN)NewDefultLegacyDevOrder) != LegacyOrderSize)
		// Something wrong in the Variables. Lets just use LegacyDevOrder
		MemCopy(NewDefultLegacyDevOrder,VarLegacyDevOrder,LegacyOrderSize);

	return NewDefultLegacyDevOrder;
}

/**
    Function to compare the Default Legacy order with legacy order,if the order same it will return TRUE else FLASE

    BBS_ORDER_TABLE *DefaultLegDevOrder,BBS_ORDER_TABLE *LegacyDevOrder                    

    @retval BOOLEAN
**/
BOOLEAN IsBBSOptionsMatch(BBS_ORDER_TABLE *DefaultLegDevOrder,BBS_ORDER_TABLE *LegacyDevOrder) 
{
    UINTN i,j;
    BOOLEAN BootOptionFound = FALSE;
    if((DefaultLegDevOrder == NULL) || (LegacyDevOrder == NULL) ||
            (DefaultLegDevOrder->Length == 0)||(LegacyDevOrder->Length == 0)||(DefaultLegDevOrder->Length != LegacyDevOrder->Length))
        return FALSE;
    
    for(i=0;i<((DefaultLegDevOrder->Length/sizeof(UINT16)) -1);i++)
    {
        BootOptionFound = FALSE;
        for(j=0;j<((LegacyDevOrder->Length/sizeof(UINT16)) -1);j++)
        {
             if((UINT8)(DefaultLegDevOrder->Data[i]) == (UINT8)(LegacyDevOrder->Data[j]))
            {
                BootOptionFound = TRUE;
                break;
            }            
        }
        if(BootOptionFound == FALSE)
        {
            return FALSE;
        }
    }    
    return TRUE;   
}
/**
    Function helper to get variable

    @param variable CHAR16 *name, EFI_GUID *guid, UINT32 *attributes, UINTN *size
					

    @retval VOID *
**/
VOID * HelperGetVariable( UINT32 variable, CHAR16 *name, EFI_GUID *guid, UINT32 *attributes, UINTN *size )
{
	VOID * Buffer;
	Buffer = VarGetVariable( variable, size );
	if(Buffer == NULL)
		Buffer = VarGetNvram( variable, size );
	return Buffer;
}

/**
    function to get TSE Cache variable Index 

    @param EFI_HANDLE VariableHandle,CHAR16 *VariableName, EFI_GUID *VariableGuid,UINT32  *varIndex
    @param buffer UINTN *size

    @retval status

**/
EFI_STATUS VarGetVariableIndex(EFI_HANDLE VariableHandle,CHAR16 *VariableName, EFI_GUID *VariableGuid,UINT32  *varIndex)
{
    UINT32  index;
    BOOLEAN found = FALSE;
    VARIABLE_INFO *varInfo = NULL;
    EFI_STATUS Status = EFI_NOT_FOUND;
   
    //Compare Name, Guid and Handle
    for(index = 0; index < gVariables->VariableCount; index++)
    {
        varInfo = (VARIABLE_INFO *)VarGetVariableInfoIndex(index);
        if(NULL == varInfo){
            continue;
        }
        //Compare GUID and Name
        if(EfiCompareGuid(&varInfo->VariableGuid, VariableGuid) && (EfiStrCmp (varInfo->VariableName, VariableName) == 0))
        {
            if(found && (VariableHandle == NULL))
                return EFI_INVALID_PARAMETER;

            if(VariableHandle && VariableHandle == varInfo->VariableHandle)
            {
                found = TRUE;
                *varIndex = index;
                Status = EFI_SUCCESS;
                break;
            }
            else if(VariableHandle == NULL)
            {
                found = TRUE;
                *varIndex = index;
                Status = EFI_SUCCESS;
            }
        }
    }
    
   return Status; 
}

/**
    This function is used to set the value in the NVRAM

    @param EFI_HANDLE , CHAR16*, EFI_GUID*, UINT32, UINTN, VOID*   

    @retval EFI_STATUS
    Return EFI_INVALID_PARAMETER, if the variable name, guid matched and handle not matched for
    the variable with same name and guid present twice.
    Return EFI_NOT_FOUND,  if the variable name and guid not matched.
    Return EFI_NOT_FOUND,  if the variable name and guid not matched.
**/
EFI_STATUS TSESetValue( EFI_HANDLE VariableHandle,CHAR16 *VariableName, EFI_GUID *VariableGuid, UINT32 offset, UINTN size, VOID *buffer )
{
    EFI_STATUS Status;
    UINT32 varIndex = 0;
    
    if((buffer == NULL )||( VariableName == NULL) || (VariableGuid == NULL))
        return EFI_INVALID_PARAMETER;
    
    Status = VarGetVariableIndex(VariableHandle,VariableName,VariableGuid, &varIndex);

    if(!EFI_ERROR(Status))
        Status = VarSetValue( varIndex, offset, size, buffer );
    
    return Status;   
 }

/**
    This function is used to get the value from the NVRAM

    @param EFI_HANDLE , CHAR16*, EFI_GUID*, UINT32, UINTN, VOID*   

    @retval EFI_STATUS
                        Return EFI_INVALID_PARAMETER, if either VariableName or VariableGuid is NULL or 
                            the requested variable is present more than once the VariableHandle is NULL.
                        Return EFI_NOT_FOUND,  if the variable name and guid not matched.
                        Return EFI_SUCCESS, if VariableIndex is found for the variable requested. And VarGetValue is Success.
                        
   .
**/
EFI_STATUS TSEGetValue( EFI_HANDLE VariableHandle,CHAR16 *VariableName, EFI_GUID *VariableGuid, UINT32 offset, UINTN size, VOID *buffer )
{
    EFI_STATUS Status;
    UINT32 varIndex = 0;
   
    if((buffer == NULL )||( VariableName == NULL) || (VariableGuid == NULL))
        return EFI_INVALID_PARAMETER;
    
    Status = VarGetVariableIndex(VariableHandle,VariableName,VariableGuid, &varIndex);

    if(!EFI_ERROR(Status))
        Status = VarGetValue( varIndex, offset, size, buffer );
    
    return Status;   
 }

