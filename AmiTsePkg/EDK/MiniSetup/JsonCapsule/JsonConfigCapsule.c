//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file JsonConfigCapsule.c
    Process the Json capsule and updates the configuration data.
**/


#include "JsonConfigCapsule.h"
#include "Library/AmiJsonLib.h"
#include "minisetup.h"  // To access the SETUP_TSE_DEBUG message

//
// Function Declarations
//
extern EFI_STATUS ValidateImage(VOID* Image, UINTN ImageSize);
VOID ConstructResultJsonNode( cJSON *jsonCtrlList, EFI_STATUS Status);
extern EFI_GUID gAmiTseProcessCapsuleGuid;
#define SETUP_FORM_SET_GUID    { 0x7B59104A, 0xC00D, 0x4158, {0x87, 0xFF, 0xF0, 0x4D, 0x63, 0x96, 0xA9, 0x15 }}


//
// Global Variables
//
CONFIG_LOOK_UP_TABLE     *gConfigLookUpTable;
UINTN                    gLookUpTableItemCount;
VAR_LIST                 *gVarTable;
UINTN                    gVariableCount;
UINTN                    gControlIndex;
UINTN                    gCtrlIndex;
NVRAM_VARIABLE           *gConfigVarList; // Holds all variables NVRAM values in an array
cJSON                    *gJsonRootNode; // Holds Root node of JSON data parsed from FFS file
cJSON                    *gResultJsonRootNode; // Holds the Root node for Capsule Result
cJSON                    *gConfigurationData;
BOOLEAN                  gIsCapsuleUpdate = FALSE;

AMI_PROCESS_CAPSULE_PROTOCOL gCapsuleUpdate = {
    ProcessJsonConfigCapsule
};

/**
    Install JsonProcessCapsule Protocol

    @param Handle 

    @retval EFI_STATUS
**/
EFI_STATUS 
InstallJsonProcessCapsuleProtocol(
    VOID
    )
{
    EFI_GUID CapsuleGuid            = EFI_JSON_CAPSULE_ID_GUID;
    EFI_STATUS Status               = EFI_SUCCESS;
    VOID                *Registration = NULL;
    EFI_EVENT           ProcessJsonConfigCapsuleEvent = NULL;
    static EFI_HANDLE   ProtocolHandle = NULL;
    UINT32              OsIndicationsSupportAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    UINTN               DataSize = sizeof (UINT64);
    UINT64              OsIndicationsSupported = EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH;

    Status = gBS->InstallMultipleProtocolInterfaces (
            &ProtocolHandle,
            &CapsuleGuid,
            &gCapsuleUpdate,
            NULL
    );
    
    Status = gBS->CreateEvent(EFI_EVENT_NOTIFY_SIGNAL, EFI_TPL_NOTIFY, PublishFFSJsonPayLoadToOS, NULL, &ProcessJsonConfigCapsuleEvent);
     if (!EFI_ERROR (Status))
     {
         Status=  gBS->RegisterProtocolNotify(
                 &gAmiTseProcessCapsuleGuid,
                 ProcessJsonConfigCapsuleEvent,
                 &Registration
         );
         if (EFI_ERROR (Status))
             return Status;
     }
    Status = pRS->GetVariable (L"OsIndicationsSupported",&gEfiGlobalVariableGuid,&(OsIndicationsSupportAttributes),&DataSize,(VOID *)&OsIndicationsSupported);
    
    if(!EFI_ERROR (Status) && DataSize ==  sizeof (UINT64))
    {
        OsIndicationsSupported = OsIndicationsSupported|EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH;   
        Status = pRS->SetVariable (L"OsIndicationsSupported", &gEfiGlobalVariableGuid, OsIndicationsSupportAttributes, DataSize, (VOID *)&OsIndicationsSupported); 
    }
    return Status;
}

/**
    This function is to process the capsule update

    @param This      : Pointer of AMI_PROCESS_CAPSULE_PROTOCOL.
    @param Image     : Pointer of Capsule Image
    @param ImageSize : Size of Capsule Image

    @retval Return Status 

**/
EFI_STATUS 
ProcessJsonConfigCapsule(
    AMI_PROCESS_CAPSULE_PROTOCOL *This,
    void *Image,
    UINTN ImageSize
    )
{
    EFI_JSON_CAPSULE_HEADER  *JsonCapsuleHeader = (EFI_JSON_CAPSULE_HEADER*)NULL; //Holds the Capsule Header
    EFI_STATUS Status = EFI_NOT_FOUND;
    EFI_GUID CapsuleGuid = EFI_JSON_CAPSULE_ID_GUID;
    UINTN Size = 7* sizeof(UINT8);// See the structure of SecureBootSetup
    EFI_GUID SetupFormset_Guid = SETUP_FORM_SET_GUID;

    UINT8 SecBoot[7] ={0};
    
    SETUP_DEBUG_TSE("[TSE] JsonCapsule: In ProcessJsonConfigCapsule\n"); 

    if( CompareGuid( &((EFI_CAPSULE_HEADER*)Image)->CapsuleGuid, &CapsuleGuid))
    {
        JsonCapsuleHeader = (EFI_JSON_CAPSULE_HEADER *)((UINT8*)Image + ((EFI_CAPSULE_HEADER*)Image)->HeaderSize);

        Status = gRT->GetVariable (L"SecureBootSetup", &SetupFormset_Guid,NULL,&Size,&SecBoot);
        
        if( !EFI_ERROR( Status ) && (SecBoot[0] == TRUE))
        {
            Status = ValidateImage(Image, ((EFI_CAPSULE_HEADER*)Image)->CapsuleImageSize);
            SETUP_DEBUG_TSE("[TSE] JsonCapsule: ValidateImage status %r\n",Status); 

            if(EFI_ERROR(Status) && Status != EFI_UNSUPPORTED)
            {
                ConstructResultJsonNode(NULL, EFI_SECURITY_VIOLATION);
                Status = WriteJsonCapsuleResult(EFI_SECURITY_VIOLATION,JsonCapsuleHeader->CapsuleId,((EFI_CAPSULE_HEADER*)Image)->Flags);
                FreePool((void*)Image);
                JsonCapsuleHeader = NULL;
                SETUP_DEBUG_TSE("[TSE] JsonCapsule: Returning from ProcessJsonConfigCapsule with status %r\n",Status); 
                return Status;
            }
        }
        Status = ProcessCapsuleFileBuffer(Image, ImageSize);
        SETUP_DEBUG_TSE("[TSE] JsonCapsule: ProcessCapsuleFileBuffer status %r\n",Status); 
        return Status;
    }

    return EFI_UNSUPPORTED;
}

/**
    This function reset EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH in OSIndications

    @param VOID
    
    @retval VOID
**/
VOID
ClearRefreshBit(
    VOID        
)
{
    UINTN   DataSize = sizeof (UINT64);
    UINTN   OsIndications = 0;
    UINT32      Attributes = 0;
    EFI_STATUS  Status = EFI_UNSUPPORTED;

    Status = pRS->GetVariable (L"OsIndications", &gEfiGlobalVariableGuid, &Attributes, &DataSize, (VOID *)&OsIndications);
   
    if(!EFI_ERROR (Status) && DataSize ==  sizeof (UINT64))
    {
        OsIndications = OsIndications & (~EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH);
        pRS->SetVariable (L"OsIndications", &gEfiGlobalVariableGuid, Attributes, DataSize, (VOID *)&OsIndications);
    }
}

/**
    This function passes the JsonPayload to OS

    @param   cJSON *jsonCtrlList
    
    @retval Return Status 

**/
EFI_STATUS
PublishJsonPayLoadToOS(
    char    *JsonBuffer,
    UINTN    JsonBufferSize
    )
{
    UINT32 TableSize = 0; 
    EFI_GUID OsGuid = EFI_JSON_CONFIG_DATA_TABLE_GUID;
    EFI_STATUS Status = EFI_NOT_FOUND;
    EFI_JSON_CAPSULE_CONFIG_DATA *Table;
    EFI_JSON_CONFIG_DATA_ITEM *ConfigDataItem;

    if(NULL == JsonBuffer)
        return Status;

    Table = AllocateRuntimePool(sizeof(EFI_JSON_CAPSULE_CONFIG_DATA) + sizeof(EFI_JSON_CONFIG_DATA_ITEM) + JsonBufferSize);
    if(NULL == Table)
    {
        Status = EFI_OUT_OF_RESOURCES;
        return Status;
    }

    //Filling the configuration table
    TableSize = (UINT32)sizeof(EFI_JSON_CAPSULE_CONFIG_DATA) + sizeof(EFI_JSON_CONFIG_DATA_ITEM) + (UINT32)JsonBufferSize;

    Table->Version = 0x01;
    Table->TotalLength = TableSize;

    ConfigDataItem =  (EFI_JSON_CONFIG_DATA_ITEM*)((UINT8*)Table + sizeof(EFI_JSON_CAPSULE_CONFIG_DATA));

    ConfigDataItem->ConfigDataLength = (UINT32)JsonBufferSize;
    CopyMem(&ConfigDataItem->ConfigData, JsonBuffer, JsonBufferSize);

    //Installing the configurations table with EFI_JSON_CONFIG_DATA_TABLE_GUID
    Status = gBS->InstallConfigurationTable(&OsGuid, (VOID*)Table);   
    SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: InstallConfigurationTable: %r\n",Status); 
    
    if(!EFI_ERROR (Status) && (!gIsCapsuleUpdate))
        ClearRefreshBit();
    
    return Status;
}

/**
    This function initialize the Lookup table for capsule update

    @param This      : VOID
    
    @retval Return Status 

**/
EFI_STATUS
InitJsonConfigData(
    VOID
    )
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    UINT8   *FileBuffer = (UINT8*)NULL;
    UINTN    BufferSize = 0;
    UINTN    JsonBufferSize = 0;      
    char    *JsonBuffer = (char*)NULL;  

    //Getting the Json Buffer from FFS
    Status = GetConfigFileBufferFromFfs(&FileBuffer,&BufferSize);

    if(EFI_ERROR(Status))
    {
        SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: FFS not found\n"); 
        return Status;
    }

    //Initializing the lookup table
    Status = InitConfigLookUpTable(FileBuffer);
    if(EFI_ERROR(Status))
    {
        SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: Initializing lookup table failed %r.\n",Status);
        goto END;
    }

    if(IsOsIndicationSupported())
    {
        Status = UpdateJsonDataWithValues(); //Updating the Json data with values

        if(EFI_ERROR(Status))
            SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: Failed to update values into JSON data\n");           


        JsonBuffer = cJSON_Print(gJsonRootNode);
        if(NULL != JsonBuffer)
        {
            JsonBufferSize = AsciiStrLen(JsonBuffer) + 1;
            Status = PublishJsonPayLoadToOS(JsonBuffer, JsonBufferSize); //Publishing the Current Configuration
        }

    }

    END:
    if(EFI_ERROR(Status) && gConfigLookUpTable)
        CleanUpLookUpTable();

    if(JsonBuffer)
        FreePool((void *)JsonBuffer);
    
    if(FileBuffer)
    {
        FreePool((void*)FileBuffer);
        FileBuffer = NULL;
    }

    return Status;
}

/**
    This function Check whether the OSIndications is supported or not

    @param VOID
    
    @retval Return  TRUE        : OSIndications is supported
                    FALSE       : OSIndications is not supported

**/
BOOLEAN
IsOsIndicationSupported(
    VOID      
    )
{
    EFI_STATUS Status = EFI_UNSUPPORTED; 
    UINTN   DataSize = sizeof (UINT64);
    UINT64   OsIndications = 0;
    BOOLEAN RetVal = FALSE;

    Status = pRS->GetVariable (L"OsIndications", &gEfiGlobalVariableGuid, NULL, &DataSize, (VOID *)&OsIndications);
   
    if(!EFI_ERROR(Status) && ((OsIndications & EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH) == EFI_OS_INDICATIONS_JSON_CONFIG_DATA_REFRESH) && (DataSize ==  sizeof (UINT64)))
        RetVal =TRUE;

    return RetVal;
}

/**
    This function Validates the Control Values from Capsule buffer

    @param LookUpTableItem      : controls pointer to lookup table 
    @param jsonCtrlList         : controls pointer to json data from capsule buffer
    
    @retval Return  TRUE        : Control is having valid value
                    FALSE       : Control is having Invalid value

**/
BOOLEAN
IsValidControlValue(
    CONFIG_LOOK_UP_TABLE *LookUpTableItem,
    cJSON *jsonCtrlList        
    )
{
    cJSON *jsonChild = (cJSON*)NULL;
    UINTN OptionIndex = 0;
    UINTN DataIndex = 0;
    UINTN OptionCount = 0;
    UINTN Year = 0;
    UINTN Day = 0;
    UINTN Month = 0;
    BOOLEAN RetVal = TRUE;

    if((NULL == LookUpTableItem->ValidateData) 
            && (LookUpTableItem->ControlType != CONTROL_TYPE_CHECKBOX) 
            && (LookUpTableItem->ControlType != CONTROL_TYPE_DATE) 
            && (LookUpTableItem->ControlType != CONTROL_TYPE_TIME))
    {
        return RetVal;
    }

    switch(LookUpTableItem->ControlType)
    {
    case  CONTROL_TYPE_POPUP_STRING:
    {    

        if((jsonCtrlList->type != cJSON_String) || ((AsciiStrLen(jsonCtrlList->valuestring) < LookUpTableItem->ValidateData->MinMax.Min) 
                || (AsciiStrLen(jsonCtrlList->valuestring) > LookUpTableItem->ValidateData->MinMax.Max)))
        {
            RetVal = FALSE;
        }
    }
    break;

    case CONTROL_TYPE_NUMERIC:
    {
        if(jsonCtrlList->type != cJSON_Number)
        {
            RetVal = FALSE;
            break;
        }

        switch(LookUpTableItem->Width)
        {
        case 1:
        {
            if((LookUpTableItem->ValidateData->MinMax.IsSigned
                    &&(jsonCtrlList->valueint < (INT8)LookUpTableItem->ValidateData->MinMax.Min
                            || jsonCtrlList->valueint > (INT8)LookUpTableItem->ValidateData->MinMax.Max))
                    ||(!LookUpTableItem->ValidateData->MinMax.IsSigned
                            &&(jsonCtrlList->valueint < (UINT8)LookUpTableItem->ValidateData->MinMax.Min
                                    || jsonCtrlList->valueint > (UINT8)LookUpTableItem->ValidateData->MinMax.Max))) 
            {
                RetVal = FALSE;
            }
        }
        break;

        case 2:
        {
            if((LookUpTableItem->ValidateData->MinMax.IsSigned
                    &&(jsonCtrlList->valueint < (INT16)LookUpTableItem->ValidateData->MinMax.Min
                            || jsonCtrlList->valueint > (INT16)LookUpTableItem->ValidateData->MinMax.Max))
                    ||(!LookUpTableItem->ValidateData->MinMax.IsSigned
                            &&(jsonCtrlList->valueint < (UINT16)LookUpTableItem->ValidateData->MinMax.Min
                                    || jsonCtrlList->valueint > (UINT16)LookUpTableItem->ValidateData->MinMax.Max))) 
            {
                RetVal = FALSE;
            }
        }
        break;

        case 4:
        {
            if((LookUpTableItem->ValidateData->MinMax.IsSigned
                    &&(jsonCtrlList->valueint < (INT32)LookUpTableItem->ValidateData->MinMax.Min
                            || jsonCtrlList->valueint > (INT32)LookUpTableItem->ValidateData->MinMax.Max))
                    ||(!LookUpTableItem->ValidateData->MinMax.IsSigned
                            &&(jsonCtrlList->valueint < (UINT32)LookUpTableItem->ValidateData->MinMax.Min
                                    || jsonCtrlList->valueint > (UINT32)LookUpTableItem->ValidateData->MinMax.Max))) 
            {
                RetVal = FALSE;
            }

        }
        break;

        case 8:
        {
            if((LookUpTableItem->ValidateData->MinMax.IsSigned
                    &&(jsonCtrlList->valueint < (INT64)LookUpTableItem->ValidateData->MinMax.Min
                            || jsonCtrlList->valueint > (INT64)LookUpTableItem->ValidateData->MinMax.Max))
                    ||(!LookUpTableItem->ValidateData->MinMax.IsSigned
                            &&((UINT64)jsonCtrlList->valueint < LookUpTableItem->ValidateData->MinMax.Min
                                    || (UINT64)jsonCtrlList->valueint > LookUpTableItem->ValidateData->MinMax.Max))) 
            {
                RetVal = FALSE;
            }
        }
        break;
        default:
        break;
        }
    }
    break;

    case CONTROL_TYPE_CHECKBOX:
    {

        if((jsonCtrlList->type != cJSON_Number) || ((jsonCtrlList->valueint != 0) && (jsonCtrlList->valueint != 1)))
        {
            RetVal = FALSE;
        }
    }
    break;

    case CONTROL_TYPE_POPUPSEL:
    {
        if(jsonCtrlList->type != cJSON_Number)
        {
            RetVal = FALSE;
            break;
        }
        for(OptionIndex = 0; OptionIndex < LookUpTableItem->ValidateData->Options.OptionCount; OptionIndex++)
        {
            if((LookUpTableItem->Width == 1) 
                    && ((UINT8)jsonCtrlList->valueint == (UINT8)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
            {
                break;
            }
            else if((LookUpTableItem->Width == 2) 
                    && ((UINT16)jsonCtrlList->valueint == (UINT16)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
            {
                break;
            }
            else if((LookUpTableItem->Width == 4) 
                    && ((UINT32)jsonCtrlList->valueint == (UINT32)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
            {
                break;
            }
            else if((LookUpTableItem->Width == 8) 
                    && ((UINT64)jsonCtrlList->valueint == (UINT64)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
            {
                break;
            }
        }

        if(OptionIndex == LookUpTableItem->ValidateData->Options.OptionCount)
        {
            RetVal = FALSE;
        }
    }
    break;
    case CONTROL_TYPE_ORDERED_LIST:
    {
        // Check all values in the input list is in option list.
        OptionCount = cJSON_GetArraySize(jsonCtrlList);
        if((jsonCtrlList->type != cJSON_Array) || (OptionCount != LookUpTableItem->ValidateData->Options.OptionCount))
        {
            RetVal = FALSE;
            break;
        }

        for(OptionIndex = 0; RetVal && OptionIndex < LookUpTableItem->ValidateData->Options.OptionCount; OptionIndex++)
        {
            UINTN Count = 0;
            jsonChild = jsonCtrlList->child;
            for(DataIndex = 0; DataIndex < LookUpTableItem->ValidateData->Options.OptionCount; DataIndex++, jsonChild = jsonChild->next)
            {
                if(jsonChild->type != cJSON_Number)
                {
                    RetVal = FALSE;
                    break;
                }

                if((LookUpTableItem->ValidateData->Options.OptionWidth == 1) 
                        && ((UINT8)jsonChild->valueint == 
                                (UINT8)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
                {
                    Count++;
                }
                else if((LookUpTableItem->ValidateData->Options.OptionWidth == 2) 
                        && ((UINT16)jsonChild->valueint == 
                                (UINT16)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
                {
                    Count++;
                }
                else if((LookUpTableItem->ValidateData->Options.OptionWidth == 4) 
                        && ((UINT32)jsonChild->valueint == 
                                (UINT32)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
                {
                    Count++;
                }
                else if((LookUpTableItem->ValidateData->Options.OptionWidth == 8) 
                        && ((UINT64)jsonChild->valueint == 
                                (UINT64)LookUpTableItem->ValidateData->Options.OptionList[OptionIndex]))
                {
                    Count++;
                }
            }

            if(Count != 1)
            {
                // If Count != 1, either the option is not present in the list 
                // or an option is present twice in the input
                RetVal = FALSE;
                break;
            }
        }
    }
    break;

    case CONTROL_TYPE_DATE:
    {
        OptionCount = cJSON_GetArraySize(jsonCtrlList);
        if((jsonCtrlList->type != cJSON_Array) || (OptionCount != 3))
        {
            RetVal = FALSE;
            break;
        }

        jsonChild = jsonCtrlList->child;

        for(DataIndex = 0; RetVal && jsonChild; jsonChild = jsonChild->next,DataIndex++)
        {
            if(jsonChild->type != cJSON_Number)
            {
                RetVal = FALSE;
                break;
            }
            if(DataIndex == 0)
                Day = (UINTN)jsonChild->valueint;

            if(DataIndex == 1)
                Month = (UINTN)jsonChild->valueint;

            if(DataIndex == 2)
                Year = (UINTN)jsonChild->valueint;
        }

        if( (Month == 0) || (Month > 12) || (Day == 0) || (Day > 31) || (Year == 0) ||  (Year > 9999))
        {
            RetVal = FALSE;
        }
        else // Validate Day
        {
            if(((Month == 0) 
                    || (Month == 4) 
                    || (Month == 6) 
                    || (Month == 9) 
                    || (Month == 11)) 
                    && (Day > 30))
            {
                RetVal = FALSE;
            }
            else if(((Year % 4000) == 0) 
                    && (Month == 2) 
                    && (Day > 28))
            { 
                //The year can be divided by 4000 is not leap year
                RetVal = FALSE;
            }
            else if((((Year % 400) == 0) 
                    && (Month == 2) 
                    && (Day > 29)) 
                    && ((Year % 4000) != 0))
            {
                RetVal = FALSE;
            }
            else if((((Year % 100) == 0) 
                    && (Month == 2) 
                    && (Day > 28)) 
                    && ((Year % 4000) != 0) 
                    && ((Year % 400) != 0))
            {
                RetVal = FALSE;
            }
            else if((((Year % 4) == 0) 
                    && (Month == 2) 
                    && (Day > 29)) 
                    && ((Year % 4000) != 0) 
                    && ((Year % 400) != 0) 
                    && ((Year % 100) != 0))
            {
                RetVal = FALSE;
            }
            else if(((Year % 4) != 0) 
                    && (Month == 2) 
                    && (Day > 28))
            {
                RetVal = FALSE;
            }
        }

    }
    break;

    case CONTROL_TYPE_TIME:
    {
        OptionCount = cJSON_GetArraySize(jsonCtrlList);
        if((jsonCtrlList->type != cJSON_Array) || (OptionCount != 3))
        {
            RetVal = FALSE;
            break;
        }

        jsonChild = jsonCtrlList->child;
        for(DataIndex = 0; DataIndex < 3; DataIndex++)
        {
            if((jsonChild->type != cJSON_Number)|| (((DataIndex == 0) && ((UINT8)jsonChild->valueint > 23)) 
                    || ((DataIndex != 0) && ((UINT8)jsonChild->valueint > 59))))
            {
                RetVal = FALSE;
                break;
            }

            jsonChild = jsonChild->next;
        }
    }
    break;

    default:
        break;

    }
    return RetVal;
}

/**
    This function Gets the Data Buffer from the Json Data of each control

    @param jsonCtrlList         : controls pointer to json data from capsule buffer
    @param Data                 : Pointer to data buffer to be filled
    @param LookUpTableItem      : controls pointer to lookup table 
   
    
    @retval Return  Status  EFI_SUCCESS: Data Buffer is filled Successfully
                            EFI_NOT_FOUND: Data Buffer is not filled                               
**/
EFI_STATUS
GetControlValueFromJsonData(
    cJSON *jsonCtrlList,
    VOID *Data,
    CONFIG_LOOK_UP_TABLE *LookUpTableItem
    )
{
    EFI_STATUS Status = EFI_SUCCESS;
    CHAR16 *StringSet = (CHAR16*)NULL;
    UINTN DataIndex = 0;
    cJSON *jsonChild = (cJSON*)NULL;

    if(NULL == jsonCtrlList)
        return EFI_NOT_FOUND;

    switch(LookUpTableItem->ControlType)
    {
    case CONTROL_TYPE_NUMERIC:
    {
        switch(LookUpTableItem->Width)
        {
        case 1:
        {
            if(LookUpTableItem->ValidateData->MinMax.IsSigned)
            {
                (*(INT8*)Data) = (INT8)jsonCtrlList->valueint;
            }
            else
            {
                (*(UINT8*)Data) = (UINT8)jsonCtrlList->valueint;
            }
        }
        break;

        case 2:
        {
            if(LookUpTableItem->ValidateData->MinMax.IsSigned)
            {
                (*(INT16*)Data) = (INT16)jsonCtrlList->valueint;
            }
            else
            {
                (*(UINT16*)Data) = (UINT16)jsonCtrlList->valueint;
            }
        }
        break;

        case 4:
        {
            if(LookUpTableItem->ValidateData->MinMax.IsSigned)
            {
                (*(INT32*)Data) = (INT32)jsonCtrlList->valueint;
            }
            else
            {
                (*(UINT32*)Data) = (UINT32)jsonCtrlList->valueint;
            }
        }
        break;

        case 8:
        {
            if(LookUpTableItem->ValidateData->MinMax.IsSigned)
            {
                (*(INT64*)Data) = (INT64)jsonCtrlList->valueint;
            }
            else
            {
                (*(UINT64*)Data) = (UINT64)jsonCtrlList->valueint;
            }
        }
        break;
        default:
        break;    
        }
    }
    break;

    case CONTROL_TYPE_CHECKBOX:
    {
        (*(UINT8*)Data) = (UINT8)jsonCtrlList->valueint;   
    }
    break;

    case CONTROL_TYPE_POPUP_STRING:
    {
        StringSet = AllocateZeroPool(sizeof(CHAR16) * (AsciiStrLen(jsonCtrlList->valuestring) + 1));
        AsciiStrToUnicodeStrS(jsonCtrlList->valuestring, StringSet, AsciiStrLen(jsonCtrlList->valuestring) + 1);
        
        

         StrCpyS((CHAR16*)Data, LookUpTableItem->Width / sizeof(CHAR16), StringSet);
        
        
    }
    break;

    case CONTROL_TYPE_ORDERED_LIST:
    {
        jsonChild = jsonCtrlList->child;
        for(DataIndex = 0; jsonChild; DataIndex++, jsonChild = jsonChild->next)
        {
            if((LookUpTableItem->ValidateData->Options.OptionWidth == 1))
            {
                ((UINT8*)Data)[DataIndex] = (UINT8)jsonChild->valueint;
            }
            else if((LookUpTableItem->ValidateData->Options.OptionWidth == 2))
            {
                ((UINT16*)Data)[DataIndex] = (UINT16)jsonChild->valueint;
            }
            else if((LookUpTableItem->ValidateData->Options.OptionWidth == 4))
            {
                ((UINT32*)Data)[DataIndex] = (UINT32)jsonChild->valueint;
            }
            else if((LookUpTableItem->ValidateData->Options.OptionWidth == 8))
            {
                ((UINT64*)Data)[DataIndex] = (UINT64)jsonChild->valueint;
            }
        }
    }
    break;

    case CONTROL_TYPE_POPUPSEL:
    {
        switch(LookUpTableItem->Width)
        {
        case 1:
        {
            (*(UINT8*)Data) = (UINT8)jsonCtrlList->valueint;
        }
        break;
        case 2:
        {
            (*(UINT16*)Data) = (UINT16)jsonCtrlList->valueint;
        }
        break;
        case 4:
        {
            (*(UINT32*)Data) = (UINT32)jsonCtrlList->valueint;
        }
        break;
        case 8:
        {
            (*(UINT64*)Data) = (UINT64)jsonCtrlList->valueint;
        }
        break;
        default:
        break;
        }
    }
    break;

    case CONTROL_TYPE_TIME:
    {
        jsonChild = jsonCtrlList->child;
        for(DataIndex = 0; jsonChild; DataIndex++, jsonChild = jsonChild->next)
        {
            if(DataIndex == 0)
                ((EFI_HII_TIME*)Data)->Hour = (UINT8)jsonChild->valueint;

            if(DataIndex == 1)
                ((EFI_HII_TIME*)Data)->Minute = (UINT8)jsonChild->valueint;

            if(DataIndex == 2)
                ((EFI_HII_TIME*)Data)->Second = (UINT8)jsonChild->valueint;
        }
    }
    break;

    case CONTROL_TYPE_DATE:
    {
        jsonChild = jsonCtrlList->child;
        for(DataIndex = 0; jsonChild; DataIndex++, jsonChild = jsonChild->next)
        {
            if(DataIndex == 0)
                ((EFI_HII_DATE*)Data)->Day = (UINT8)jsonChild->valueint;

            if(DataIndex == 1)
                ((EFI_HII_DATE*)Data)->Month = (UINT8)jsonChild->valueint;

            if(DataIndex == 2)
                ((EFI_HII_DATE*)Data)->Year = (UINT16)jsonChild->valueint;
        }
    }
    break;

    default:
    {
        Status = EFI_NOT_FOUND;
    }
    break; 

    }
    return Status;
}

/**
  This function return the EFI status in string format 

  @param[in] EFI_STATUS Status  : The EFI Status to be converted to string

  @retval CHAR8*                : Converted EFI status in string

**/
CHAR8*
GetStatusInString(
    EFI_STATUS Status
    )
{   
    CHAR8 *StatusStr = NULL;

    StatusStr = AllocateZeroPool(MAX_ERROR_BUFFER_LENGTH);
    
    AsciiSPrint(StatusStr,MAX_ERROR_BUFFER_LENGTH,"%r",Status);
    
    return StatusStr;
}

/**
  This function creates the Configuration result table..

  @param[in] EFI_STATUS Status       Processed Capsule Status
  @param[in] UINT32 CapsuleId        The capsule Id
  @param[in] UINT32 Flags            Flags from  EFI_CAPSULE_HEADER

  @retval EFI_SUCCESS                The capsule status variable is recorded.
  @retval EFI_OUT_OF_RESOURCES       No resource to record the capsule status variable.
**/
EFI_STATUS
WriteJsonCapsuleResult(
    EFI_STATUS Status,
    UINT32 CapsuleId,
    UINT32 Flags
    )
{
    EFI_CAPSULE_RESULT_VARIABLE_HEADER *CapsuleResultVariableHeader = (EFI_CAPSULE_RESULT_VARIABLE_HEADER*)NULL;
    EFI_CAPSULE_RESULT_VARIABLE_JSON   *CapsuleResultVariableJson = (EFI_CAPSULE_RESULT_VARIABLE_JSON *)NULL;
    EFI_GUID CapsuleHeaderGuid = EFI_JSON_CAPSULE_ID_GUID;
    UINTN  CapsuleResultVariableSize = 0;
    UINT8 *CapsuleResultVariable = NULL;
    char *ResultBuffer = (char*)NULL;
    UINTN ResultBufferSize = 0;

    if(NULL == gResultJsonRootNode)
        return EFI_NOT_FOUND;

    ResultBuffer = cJSON_Print(gResultJsonRootNode);
    if(NULL == ResultBuffer)
        return EFI_NOT_FOUND;

    ResultBufferSize = AsciiStrLen(ResultBuffer) + 1;

    //Calculating the total memory required
    CapsuleResultVariableSize = sizeof(EFI_CAPSULE_RESULT_VARIABLE_HEADER) + sizeof(EFI_CAPSULE_RESULT_VARIABLE_JSON) + ResultBufferSize;

    //Allocating  the total memory required
    CapsuleResultVariable = AllocateZeroPool (CapsuleResultVariableSize);
    if (CapsuleResultVariable == NULL) 
    {
        return EFI_OUT_OF_RESOURCES;
    }

    //Assigning the start address to first struct & filling it
    CapsuleResultVariableHeader = (VOID *)CapsuleResultVariable;

    CapsuleResultVariableHeader->VariableTotalSize = (UINT32)CapsuleResultVariableSize;
    CapsuleResultVariableHeader->Reserved = 0;    
    CopyGuid(&CapsuleResultVariableHeader->CapsuleGuid, &CapsuleHeaderGuid);
    ZeroMem(&CapsuleResultVariableHeader->CapsuleProcessed, sizeof(CapsuleResultVariableHeader->CapsuleProcessed));
    gRT->GetTime(&CapsuleResultVariableHeader->CapsuleProcessed, NULL);
    CapsuleResultVariableHeader->CapsuleStatus = Status;

    //Assigning the start address to Second struct & filling it
    CapsuleResultVariableJson = (VOID *)((UINT8*)CapsuleResultVariable + sizeof(EFI_CAPSULE_RESULT_VARIABLE_HEADER));

    CapsuleResultVariableJson->Version = 1;
    CapsuleResultVariableJson->CapsuleId = CapsuleId;
    CapsuleResultVariableJson->RespLength = (UINT32)ResultBufferSize;
    CopyMem((UINT8*)CapsuleResultVariableJson + sizeof(EFI_CAPSULE_RESULT_VARIABLE_JSON), ResultBuffer, ResultBufferSize);

    // if ((Flags & CAPSULE_FLAGS_PERSIST_ACROSS_RESET) != 0)
        Status = WriteNewCapsuleResultVariable(CapsuleResultVariable, CapsuleResultVariableSize);

    FreePool((void*)CapsuleResultVariable);

    return Status;
}

/**
  Write a new capsule status variable.

  @param[in] CapsuleResult      The capsule status variable
  @param[in] CapsuleResultSize  The size of the capsule stauts variable in bytes

  @retval EFI_SUCCESS          The capsule status variable is recorded.
  @retval EFI_OUT_OF_RESOURCES No resource to record the capsule status variable.
**/
EFI_STATUS
WriteNewCapsuleResultVariable (
    IN VOID    *CapsuleResult,
    IN UINTN   CapsuleResultSize
    )
{
    EFI_GUID   EfiCapsuleReportGuid = EFI_JSON_CAPSULE_ID_GUID;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    Status = gRT->SetVariable(
            L"AmiCapsuleResult",
            &EfiCapsuleReportGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
            CapsuleResultSize,
            CapsuleResult
    );
    return Status;
}

/**
    This function Construts the result Json variable node

    @param cJSON *jsonCtrlList : Json Root node of capsule data
    @param  EFI_STATUS Status  : Status to update
        
    @retval Return  VOID
**/
VOID
ConstructResultJsonNode(
    cJSON *jsonCtrlList,
    EFI_STATUS Status     
    )
{
    cJSON *ConfigurationResult = (cJSON*)NULL;
    CHAR8* StatusStr = NULL; 

    gResultJsonRootNode = cJSON_CreateObject();
    if (gResultJsonRootNode == NULL)
    {
        return;
    }
    ConfigurationResult = cJSON_CreateObject();
    if (ConfigurationResult == NULL)
    {
        return;
    }
    cJSON_AddItemToObject(gResultJsonRootNode, "ConfigurationResult", ConfigurationResult);

    if(jsonCtrlList == NULL || !cJSON_GetArraySize(jsonCtrlList))
    {
        return;
    }

    jsonCtrlList = jsonCtrlList->child;
    while(jsonCtrlList)
    {
        StatusStr = GetStatusInString(Status);
        cJSON_AddStringToObject(ConfigurationResult, jsonCtrlList->string, StatusStr);
        jsonCtrlList = jsonCtrlList->next;  
        FreePool((void*)StatusStr);
    }  
}

/**
    This function Initialize global pointers and fixing the Hii data  

    @param VOID
        
    @retval Return  EFI_STATUS  EFI_SUCCESS - If global pointers initializes and Hii data is fixed up
                    EFI_ERROR   Failure
**/
EFI_STATUS
InitHiiAndVariables(
        VOID
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    if(gApplicationData == NULL || gVariables == NULL)
    {     
        InitAmiLib(gImageHandle,gST);
        Status = InitApplicationData(gImageHandle); // To initialize gApplication
        if(EFI_ERROR(Status))
            return Status;
        
        InitGlobalPointers();       // To initialize global Variables
    }

    Status = VarLoadVariables ((VOID **)&gVariableList, NULL);
    if(EFI_ERROR(Status))
        return Status;
   
    Status = HiiFixupData();
    if(EFI_ERROR(Status))
        return Status;
  
    Status = VarBuildDefaults();
    if(EFI_ERROR(Status))
        return Status;
    
    return Status;
}

/**
    This function Validates the whole capsule buffer and updated the valid controls values in NVRAM

    @param FileBuffer      : Capsule buffer
        
    @retval Return  Status
**/
EFI_STATUS
ProcessCapsuleFileBuffer(
        void *Image,
        UINTN ImageSize
    )
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    cJSON *jsonRootNode = (cJSON*)NULL;
    cJSON *jsonCtrlList = (cJSON*)NULL;
    CHAR16 *MappingId = (CHAR16*)NULL;
    UINT8 *FileBuffer = (UINT8*)NULL;   
    VOID *Data = NULL;
    UINTN TableIndex = 0;
    EFI_TPL OldTpl;
    EFI_JSON_CAPSULE_HEADER *JsonCapsuleHeader = (EFI_JSON_CAPSULE_HEADER*)NULL;    
    UINT8   *FfsFileBuffer = (UINT8*)NULL;
    UINTN    FfsBufferSize = 0;
  
    SETUP_DEBUG_TSE("[TSE] JsonCapsule: In ProcessCapsuleFileBuffer\n"); 
    gIsCapsuleUpdate = TRUE;
    
    OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL); //Changing callback TPL to application
    gBS->RestoreTPL (TPL_APPLICATION);
    
    if(!IsOsIndicationSupported())
    {
        PERF_START (0, AMITSE_TEXT("TseCapsuleFfsPublish"), NULL, 0);
        Status = GetConfigFileBufferFromFfs(&FfsFileBuffer,&FfsBufferSize);
        
        Status = PublishJsonPayLoadToOS((char*)FfsFileBuffer, FfsBufferSize + 1); //Publishing the Cache Configuration
        PERF_END (0, AMITSE_TEXT("TseCapsuleFfsPublish"), NULL, 0);
        
        if(FfsFileBuffer)
        {
            FreePool((void*)FfsFileBuffer);
            FfsFileBuffer = NULL;
        }
        
        if(!gIsCapsuleUpdate)
            goto ERROR;
    }
 
    Status = InitHiiAndVariables();
    if(EFI_ERROR(Status)) 
    {
        Status = EFI_INVALID_PARAMETER;
        goto ERROR;
    }
    
    PERF_START (0, AMITSE_TEXT("TseCapsuleConfigurationData"), NULL, 0);
    Status = InitJsonConfigData();
    PERF_END (0, AMITSE_TEXT("TseCapsuleConfigurationData"), NULL, 0);
    
    if(EFI_ERROR(Status))
    {
        SETUP_DEBUG_TSE("[TSE] JsonCapsule: Initializing lookup table failed %r.\n",Status);
        Status = EFI_INVALID_PARAMETER;
        goto ERROR;
    } 
      
    if(!gIsCapsuleUpdate)
    {
        Status = EFI_INVALID_PARAMETER;
        goto ERROR;
    }
        
    JsonCapsuleHeader = (EFI_JSON_CAPSULE_HEADER *)((UINT8*)Image + ((EFI_CAPSULE_HEADER*)Image)->HeaderSize);
    
    FileBuffer = (UINT8*)JsonCapsuleHeader + sizeof(EFI_JSON_CAPSULE_HEADER);
   
    jsonRootNode = cJSON_Parse((const char *)FileBuffer);
    if(jsonRootNode) 
        jsonCtrlList = cJSON_GetObjectItem(jsonRootNode,"ConfigurationData");

    if(NULL == jsonCtrlList || !cJSON_GetArraySize(jsonCtrlList)) 
    {
        ConstructResultJsonNode(NULL, EFI_INVALID_PARAMETER);
        Status = WriteJsonCapsuleResult(EFI_INVALID_PARAMETER,JsonCapsuleHeader->CapsuleId,((EFI_CAPSULE_HEADER*)Image)->Flags);
        Status = EFI_INVALID_PARAMETER;     //Json Capsule data is incorrect or it doesn't have any members item
        goto ERROR;
    }

    if((NULL == gConfigLookUpTable) || !gLookUpTableItemCount)
    {
        SETUP_DEBUG_TSE ("[TSE] JsonCapsule:LookUpTable not initialized yet\n");
        ConstructResultJsonNode(jsonRootNode, EFI_UNSUPPORTED);
        Status = WriteJsonCapsuleResult(EFI_UNSUPPORTED,JsonCapsuleHeader->CapsuleId,((EFI_CAPSULE_HEADER*)Image)->Flags);
        Status = EFI_UNSUPPORTED;     // Lookup Table not initialized 
        goto ERROR;
    }

    ConstructResultJsonNode(jsonCtrlList, EFI_SUCCESS);

    //Creating a NVRAM table for all the available controls
    Status = GetVariableBufferFromNvram();

    jsonCtrlList = jsonCtrlList->child; 
    while(jsonCtrlList)
    {
        MappingId = AllocateZeroPool(sizeof(CHAR16) * (AsciiStrLen(jsonCtrlList->string) + 1));
        AsciiStrToUnicodeStrS(jsonCtrlList->string, MappingId, AsciiStrLen(jsonCtrlList->string) + 1);

        Status = GetLookUpTableIndex(MappingId,&TableIndex);
        if(EFI_ERROR(Status))
        {
            SETUP_DEBUG_TSE("[TSE] JsonCapsule: MappingID - %s not found in LookupTable %r\n", MappingId, Status);
            UpadteResultVariable(-1,jsonCtrlList->string,Status);
            goto NEXT_CTRL;
        }

        if(IsValidControlValue(&gConfigLookUpTable[TableIndex],jsonCtrlList))
        {             
            Data = AllocateZeroPool(gConfigLookUpTable[TableIndex].Width);
            if(NULL == Data)
            {
                goto NEXT_CTRL;
            }

            //Getting the data buffer from json data
            Status = GetControlValueFromJsonData(jsonCtrlList,Data,&gConfigLookUpTable[TableIndex]); 
            if(EFI_ERROR(Status))
            {
                SETUP_DEBUG_TSE("[TSE] JsonCapsule: Failed to get the data buffer from JSON data for Control: %s \n", MappingId);
                UpadteResultVariable(-1,jsonCtrlList->string,EFI_NOT_FOUND);
                goto NEXT_CTRL;
            }

            //Setting the controls current value into cache buffer    
            Status = SetControlCurrentValue(TableIndex,Data);
            if(EFI_ERROR(Status))
            {
                SETUP_DEBUG_TSE("[TSE] JsonCapsule: Failed to Update Current Value of Control: %s \n", MappingId);
                UpadteResultVariable(-1,jsonCtrlList->string,EFI_INVALID_PARAMETER);
                goto NEXT_CTRL;
            }

        }
        else
            UpadteResultVariable(-1,jsonCtrlList->string,EFI_INVALID_PARAMETER);

NEXT_CTRL:
        if(MappingId)
        {
            FreePool((void*)MappingId);
            MappingId = NULL;
        }

        if(Data)
        {
            FreePool((void*)Data);
            Data = NULL;
        }

        jsonCtrlList = jsonCtrlList->next;

    }


    EfiLibNamedEventSignal ( &gAmiTseSetupEnterGuid );//Signal to unlock secure variables


    //Updating all the variable from cache buffer to NVRAM
    Status = SetVariableBufferInNvram();

    //Updating The Json Result to NVRAM variable
    Status = WriteJsonCapsuleResult(EFI_SUCCESS,JsonCapsuleHeader->CapsuleId,((EFI_CAPSULE_HEADER*)Image)->Flags);
    if(EFI_ERROR(Status))
    {
        SETUP_DEBUG_TSE("[TSE] JsonCapsule: Failed to Update the Capsule Result\n");
    }
    if(gConfigVarList)
        CleanUpVariableTable();

    cJSON_Delete(jsonRootNode);

    cJSON_Delete(gResultJsonRootNode);
    
    if(gJsonRootNode)
    {
        cJSON_Delete(gJsonRootNode);
        gJsonRootNode = NULL;
    }
//    SETUP_DEBUG_TSE("[TSE] JsonCapsule: Capsule Processed Successfully Reseting the System.............\n");
//    gRT->ResetSystem(EfiResetCold,EFI_SUCCESS,0,NULL);

ERROR:
    if(gJsonRootNode)
    {
        cJSON_Delete(gJsonRootNode);
        gJsonRootNode = NULL;
    }
    gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (OldTpl);
    return Status;
}

VOID EFIAPI
PublishFFSJsonPayLoadToOS(
    IN EFI_EVENT    Event, 
    IN VOID         *Context   
    )
{
   
    EFI_TPL OldTpl;
  
    UINT8   *FfsFileBuffer = (UINT8*)NULL;
    UINTN    FfsBufferSize = 0;
  
   	OldTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL); 
    gBS->RestoreTPL (TPL_APPLICATION);
 //   DebugPrint(-1,"PublishFFSJsonPayLoadToOS calling IsOsIndicationSupported() \n");
    if(!IsOsIndicationSupported())
    {
        PERF_START (0, AMITSE_TEXT("TseCapsuleFfsPublish"), NULL, 0);
        GetConfigFileBufferFromFfs(&FfsFileBuffer,&FfsBufferSize);
        
        PublishJsonPayLoadToOS((char*)FfsFileBuffer, FfsBufferSize + 1); //Publishing the Cache Configuration
        PERF_END (0, AMITSE_TEXT("TseCapsuleFfsPublish"), NULL, 0);
        
        if(FfsFileBuffer)
        {
            FreePool((void*)FfsFileBuffer);
            FfsFileBuffer = NULL;
        }
        
    }
 
    gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (OldTpl);
}


/**
    This function is to get the varInfo of the variables present in Json file from gVariables

    @param name       : Variable name from Json file
    @param Data       : Variable guid from Json file
        
    @retval Return  Status
**/
VARIABLE_INFO*
GetVarInfo(
    CHAR16 *name, 
    EFI_GUID *guid
    )
{
    UINT32 index;
    VARIABLE_INFO *varInfo = NULL;

    if(gVariables)
    {
        for (index = 0; index < gVariables->VariableCount; index++)
        {
            varInfo = NULL;

            //Traversing each variable from gVariables
            varInfo = (VARIABLE_INFO *)((UINT8 *)gVariables + gVariables->VariableList[index]); 

            //Checking the variable from Json to gVariables variable
            if(!StrCmp(name, varInfo->VariableName) && (CompareGuid(guid, &varInfo->VariableGuid)))
                return varInfo;   
        }
    }
    return varInfo;
}
/**
    This function is Creating a NVRAM table for all the available controls

    @param VOID     
        
    @retval Return  Status
**/
EFI_STATUS
GetVariableBufferFromNvram(
    VOID           
    )
{
    EFI_STATUS Status = EFI_SUCCESS; 
    UINTN TableIndex = 0;
    UINTN Size = 0;
    UINT32 Attributes = 0;
    VARIABLE_INFO *varInfo = NULL;
    VOID* Buffer = NULL;

    gConfigVarList = (NVRAM_VARIABLE *)AllocateZeroPool(gVariableCount * sizeof(NVRAM_VARIABLE));
    if(NULL == gConfigVarList)
    {
        Status = EFI_OUT_OF_RESOURCES;
        return Status;
    }

    if(gVariables)
    {
        for(TableIndex = 0;(TableIndex < gVariableCount) && (TableIndex < gVariables->VariableCount); TableIndex++)
        {
            Size = 0;
            Attributes = 0;

            //Getting the variable Info from gVariables
            varInfo = GetVarInfo(gVarTable[TableIndex].Varname,&gVarTable[TableIndex].VarGuid);

            if(varInfo)
            {
                //If the variable is Varstore or namespace variable then do Extract config to get variable buffer
                if((varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_VARSTORE) || (varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_NAMEVALUE))
                {
                    Status = UefiVarGetNvram(varInfo,&Buffer,0, varInfo->VariableSize);
                    if(!EFI_ERROR(Status) && Buffer)
                    {
                        gConfigVarList[TableIndex].Buffer = Buffer;
                        gConfigVarList[TableIndex].Size = varInfo->VariableSize;
                        gVarTable[TableIndex].Attributes = varInfo->VariableAttributes;
                    }
                }

                //If the variable is EfiVarstore then get the variable buffer directly from NVRAM 
                else if(varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_EFI_VARSTORE)
                {
                    gConfigVarList[TableIndex].Buffer = VarGetNvramName(gVarTable[TableIndex].Varname,&gVarTable[TableIndex].VarGuid,&Attributes, &Size);
                    gConfigVarList[TableIndex].Size = Size;
                    gVarTable[TableIndex].Attributes = Attributes;
                }      
            }
        }
    }
    else 
        return EFI_NOT_FOUND;

    return Status;
}

/**
    This function Sets each control values from Json Buffer to the Cache Buffer

    @param TableIndex : controls TableIndex in LookupTable Table
    @param Data       : Void pointer to the data to be set
    
    @retval Return  Status  EFI_SUCCESS: Data Buffer is set Successfully
                            EFI_INVALID_PARAMETER: Data Buffer is not Set                               
**/
EFI_STATUS
SetControlCurrentValue(
    UINTN TableIndex,
    VOID *Data
    )
{

    if (gConfigVarList[gConfigLookUpTable[TableIndex].VarId].Buffer && Data)
    {
        CopyMem(gConfigVarList[gConfigLookUpTable[TableIndex].VarId].Buffer + gConfigLookUpTable[TableIndex].Offset, Data, gConfigLookUpTable[TableIndex].Width);
        return EFI_SUCCESS;
    }
    return EFI_INVALID_PARAMETER;
}

/**
    This function Sets The cache buffer into NVRAM

    @param VOID
    
    @retval Return  Status                          
**/
EFI_STATUS 
SetVariableBufferInNvram(
    VOID
   )
{
    UINTN Index = 0;
    EFI_STATUS Status = EFI_UNSUPPORTED;
    VARIABLE_INFO *varInfo = NULL;

    if(gVariables)
    {
        for(Index = 0;(Index < gVariableCount) && (Index < gVariables->VariableCount); Index++)
        {
            //Getting the variable Info from gVariables
            varInfo = GetVarInfo(gVarTable[Index].Varname,&gVarTable[Index].VarGuid);

            if(varInfo)
            {
                //If the variable is Varstore or namespace variable then do Route config to Set variable buffer
                if((varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_VARSTORE) || (varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_NAMEVALUE))
                {
                    Status = UefiVarSetNvram(varInfo, (VOID*)gConfigVarList[Index].Buffer, 0, varInfo->VariableSize);

                    if(EFI_ERROR(Status))
                        UpadteResultVariable((INTN)Index,NULL,Status); //Update the result variable in failure case
                }

                //If the variable is EfiVarstore then gSet the variable buffer directly to NVRAM 
                else if(varInfo->ExtendedAttibutes == VARIABLE_ATTRIBUTE_EFI_VARSTORE)
                {
                    if(gConfigVarList[Index].Size != 0)
                    {
                        Status = gRT->SetVariable(
                                gVarTable[Index].Varname,
                                &gVarTable[Index].VarGuid,
                                gVarTable[Index].Attributes,
                                gConfigVarList[Index].Size,
                                (VOID*)gConfigVarList[Index].Buffer
                        );

                        if(EFI_ERROR(Status))
                            UpadteResultVariable((INTN)Index,NULL,Status); //Update the result variable in failure case
                    }
                }
            }
        }
    }
    else
        return EFI_NOT_FOUND;
    
    return Status;
}

/**
    This function Updates the Capsule result Json node

    @param UINTN VariableIndex    : Variable TableIndex
    @param char* InMappingId      : MappingId of control
    @param EFI_STATUS FailStatus  : Failure Status
    
    @retval Return  Status                          
**/
EFI_STATUS
UpadteResultVariable(
    INTN VariableIndex,
    char* ControlMappingId,
    EFI_STATUS FailStatus
    )
{
    cJSON *jsonResultList = (cJSON*)NULL; 
    UINTN   TableIndex = 0;
    CHAR16 *MappingId = (CHAR16*)NULL;
    EFI_STATUS Status = EFI_SUCCESS;

    if(((VariableIndex == -1) && (NULL == ControlMappingId)) || (NULL == gResultJsonRootNode))
        return EFI_INVALID_PARAMETER;   
    
    jsonResultList = cJSON_GetObjectItem(gResultJsonRootNode,(char *)"ConfigurationResult");
    if(NULL == jsonResultList)
        return EFI_NOT_FOUND;

    jsonResultList = jsonResultList->child;
    if(NULL != ControlMappingId)
    {
        while(jsonResultList)
        {
            if(AsciiStrCmp(ControlMappingId,jsonResultList->string) == 0)
            {
                FreePool((void*)jsonResultList->valuestring);
                jsonResultList->valuestring = GetStatusInString(FailStatus);
                return Status;
            }
            jsonResultList = jsonResultList->next;
        }
    }
    else if(NULL == ControlMappingId)
    {
        while(jsonResultList)
        {     
            MappingId = AllocateZeroPool(sizeof(CHAR16) * (AsciiStrLen(jsonResultList->string) + 1));
            AsciiStrToUnicodeStrS(jsonResultList->string, MappingId, AsciiStrLen(jsonResultList->string) + 1);

                       
            Status = GetLookUpTableIndex(MappingId,&TableIndex);
            if(!EFI_ERROR(Status) && ((StrCmp((CHAR16*)gVarTable[VariableIndex].Varname,(CHAR16*)gVarTable[gConfigLookUpTable[TableIndex].VarId].Varname) == 0) && (CompareGuid(&gVarTable[VariableIndex].VarGuid,&gVarTable[gConfigLookUpTable[TableIndex].VarId].VarGuid))))
            {
                FreePool((void*)jsonResultList->valuestring);
                jsonResultList->valuestring = GetStatusInString(FailStatus);
            }
            if(MappingId)
            {
                FreePool((void*)MappingId);
                MappingId = NULL;
            }
            jsonResultList = jsonResultList->next;
        }
    }

    return Status;
}

/**
    This function finds the Index in which the input MappingID's entry present in the Look up table.
     
    @param  CHAR16 *MappingID, UINTN *TableIndex

    @retval EFI_SUCCESS : MappingID's entry found in the look up table
            EFI_INVALID_PARAMETER : Either MappingID or TableIndex is NULL.
            EFI_NOT_FOUND : MappingID's entry not found in the table
**/
EFI_STATUS 
GetLookUpTableIndex(
    CHAR16* MappingID,
    UINTN *TableIndex
    )
{
    UINTN Index = 0;

    if((NULL == MappingID) || (NULL == TableIndex))
        return EFI_INVALID_PARAMETER;

    for(Index = 0; Index < gLookUpTableItemCount; Index++)
    {
        if(!StrCmp(MappingID, gConfigLookUpTable[Index].MappingID))
        {
            *TableIndex = Index;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;
}

/**
    This function is to Update each control current value into the Json node 

    @param: jsonCtrlList: json node
    
    @retval Return Status 

**/
EFI_STATUS
UpdateCurrentValues(
    cJSON *jsonCtrlList
    )
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    UINT8 Index = 0;
    UINT64 CurVal = 0;
    UINTN Length = 0;
    CHAR8 *SignedString = (CHAR8*)NULL;
    CHAR8 *StringBuff =(CHAR8*)NULL;
    CHAR8 *MappingId = (CHAR8*)NULL;
    UINTN MappingIdLength = 0;
    VOID *Buffer =NULL;

    if(NULL == jsonCtrlList)
        return Status;

    //Parsing All the available controls one by one and updating the current values
    jsonCtrlList = jsonCtrlList->child;

    for(; jsonCtrlList && (gCtrlIndex < gLookUpTableItemCount); gCtrlIndex++,jsonCtrlList = jsonCtrlList->next)
    {
        Buffer = AllocateZeroPool(gConfigLookUpTable[gCtrlIndex].Width);
        if(NULL == Buffer)
        {
            Status = EFI_OUT_OF_RESOURCES;
            goto END;
        }

        Status = GetControlsCurrentValue(gCtrlIndex,Buffer);

        if(EFI_ERROR(Status))
        {
            if(Buffer)
			{
                FreePool((void*)Buffer);
				Buffer = NULL;	
			}

            continue;
        }
        
        MappingIdLength = StrLen (gConfigLookUpTable[gCtrlIndex].MappingID) + 1;
        
        MappingId = AllocateZeroPool(MappingIdLength);
        Status = UnicodeStrToAsciiStrS(gConfigLookUpTable[gCtrlIndex].MappingID,MappingId,MappingIdLength);
            
        switch(gConfigLookUpTable[gCtrlIndex].ControlType)
        {
        case CONTROL_TYPE_NUMERIC:
        case CONTROL_TYPE_CHECKBOX:  
        case CONTROL_TYPE_POPUPSEL:    
        {
            if((gConfigLookUpTable[gCtrlIndex].ControlType == CONTROL_TYPE_NUMERIC) && (gConfigLookUpTable[gCtrlIndex].ValidateData->MinMax.IsSigned) && IsNegative(Buffer,gConfigLookUpTable[gCtrlIndex].Width))
            {     
                SignedString = AllocateZeroPool(2048);
                if(NULL == SignedString)
                {
                    Status = EFI_OUT_OF_RESOURCES;
                    goto END;
                }
                
                SignedString = ConvVfrNumToString(gConfigLookUpTable[gCtrlIndex].Width,Buffer);
               
                cJSON_AddNumAsStringToObject(gConfigurationData,MappingId,SignedString);

                if(SignedString)
                    FreePool((void*)SignedString);
            }

            else
            {                            
                switch(gConfigLookUpTable[gCtrlIndex].Width)
                {
                case 1:
                    CurVal = *(UINT8*)Buffer; 
                    break;

                case 2:
                    CurVal = *(UINT16*)Buffer;
                    break;

                case 4:
                    CurVal = *(UINT32*)Buffer;
                    break;

                case 8:
                    CurVal = *(UINT64*)Buffer;
                    break;
				default:
					break;
                }
                cJSON_AddNumberIntToObject(gConfigurationData,MappingId,CurVal);
                
            }
        }
        break;

        case CONTROL_TYPE_POPUP_STRING:
        case CONTROL_TYPE_PASSWORD:    
        {
            Length = StrLen ((CHAR16*)Buffer) + 1;
            StringBuff = AllocateZeroPool(Length);
            if(NULL == StringBuff)
            {
                Status = EFI_OUT_OF_RESOURCES;
                goto END;
            }

            Status = UnicodeStrToAsciiStrS((CHAR16 *)Buffer,StringBuff,Length);
            cJSON_AddStringToObject(gConfigurationData,MappingId, StringBuff);

            if(StringBuff)
                FreePool((void*)StringBuff);
        }                 
        break;

        case CONTROL_TYPE_ORDERED_LIST:
        {
            cJSON *OptionList = cJSON_CreateArray();
            if (OptionList == NULL)
                break;
            
            cJSON_AddItemToObject(gConfigurationData,MappingId, OptionList);

            switch(gConfigLookUpTable[gCtrlIndex].ValidateData->Options.OptionWidth)
            {
            case 1:
            {
                UINT8 OrdCurVal = 0; 

                for(Index=0; Index < gConfigLookUpTable[gCtrlIndex].ValidateData->Options.OptionCount; Index++)
                {
                    OrdCurVal = ((UINT8*)Buffer)[Index];
                    cJSON_AddNumberIntToObject(OptionList, "",OrdCurVal);

                }
            }
            break;
            case 2:
            {
                UINT16 OrdCurVal = 0;
                for(Index = 0; Index < gConfigLookUpTable[gCtrlIndex].ValidateData->Options.OptionCount; Index++)
                {
                    OrdCurVal = ((UINT16*)Buffer)[Index];
                    cJSON_AddNumberIntToObject(OptionList,"",OrdCurVal);

                }

            }
            break;
            case 4:
            {
                UINT32 OrdCurVal = 0;
                for(Index = 0; Index < gConfigLookUpTable[gCtrlIndex].ValidateData->Options.OptionCount; Index++)
                {
                    OrdCurVal = ((UINT32*)Buffer)[Index];
                    cJSON_AddNumberIntToObject(OptionList,"",OrdCurVal);

                }
            }
            break;
            case 8:
            {
                UINT64 OrdCurVal = 0;
                for(Index = 0; Index < gConfigLookUpTable[gCtrlIndex].ValidateData->Options.OptionCount; Index++)
                {
                    OrdCurVal = ((UINT64*)Buffer)[Index];
                    cJSON_AddNumberIntToObject(OptionList,"",OrdCurVal);

                }
            }
            break;
			default:
			break;
            }
        }
        break;

        case CONTROL_TYPE_TIME:
        {
            cJSON *OptionList = cJSON_CreateArray();
            if (OptionList == NULL)
                break;
                        
            cJSON_AddItemToObject(gConfigurationData,MappingId, OptionList);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_TIME*)Buffer)->Hour);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_TIME*)Buffer)->Minute);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_TIME*)Buffer)->Second);
        }
        break;

        case CONTROL_TYPE_DATE:
        {
            cJSON *OptionList = cJSON_CreateArray();
            if (OptionList == NULL)
                break;

            cJSON_AddItemToObject(gConfigurationData,MappingId, OptionList);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_DATE*)Buffer)->Day);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_DATE*)Buffer)->Month);
            cJSON_AddNumberIntToObject(OptionList, "", ((EFI_HII_DATE*)Buffer)->Year);
        }
        break;

		default:
		break;
        }
        
        if(Buffer)
		{
            FreePool((void*)Buffer);
			Buffer = NULL;
		}
        
        if(MappingId)
            FreePool((void*)MappingId);
    }    
    END:
	    if(Buffer)
		{
            FreePool((void*)Buffer);
			Buffer = NULL;
		}
    return Status;
}

/**
    This function is to Update the values of all the control

    @param: VOID
    
    @retval Return Status 

**/
EFI_STATUS
UpdateJsonDataWithValues(
    VOID
    )
{
    cJSON *jsonCtrlList = (cJSON*)NULL;
    EFI_STATUS Status;
       
    gConfigurationData = cJSON_CreateObject();
 
    //Creating a NVRAM table for all the available controls
    Status = GetVariableBufferFromNvram();
    SETUP_DEBUG_TSE("[TSE] JsonCapsule: GetVariableBufferFromNvram %r.\n",Status);

    jsonCtrlList = cJSON_GetObjectItem(gJsonRootNode,"BS Questions"); //calling the functions to update the controls current values which are under BS Questions
    if(jsonCtrlList && cJSON_GetArraySize(jsonCtrlList))
    {
        Status = UpdateCurrentValues(jsonCtrlList);
        SETUP_DEBUG_TSE("[TSE] JsonCapsule: BS Questions Update Status: %r.\n",Status);
    }
    
    jsonCtrlList = cJSON_GetObjectItem(gJsonRootNode,"RT Questions"); //calling the functions to update the controls current values which are under RT Questions
    if(jsonCtrlList && cJSON_GetArraySize(jsonCtrlList))
    {
        Status = UpdateCurrentValues(jsonCtrlList);
        SETUP_DEBUG_TSE("[TSE] JsonCapsule: RT Questions Update Status: %r.\n",Status);
    }   
    
    cJSON_AddItemToObject(gJsonRootNode, "ConfigurationData", gConfigurationData);
    
    if(gConfigVarList)
        CleanUpVariableTable();

    return Status;
}

/**
    This function is to Get the Variable Buffer

    @param TableIndex   : Lookup table Index
    @param Buffer       : Variable Buffer
    
    @retval Return Status 

**/
EFI_STATUS
GetControlsCurrentValue(
    UINTN TableIndex,
    VOID *Buffer
    )
{ 
    if (gConfigVarList[gConfigLookUpTable[TableIndex].VarId].Buffer && Buffer)
    {
        MemCopy(Buffer, gConfigVarList[gConfigLookUpTable[TableIndex].VarId].Buffer + gConfigLookUpTable[TableIndex].Offset, gConfigLookUpTable[TableIndex].Width);
        return EFI_SUCCESS;
    }
    return EFI_INVALID_PARAMETER;    
}

/**
    This Function read the file from FFS and pass the buffer and its size through its arguments.

    @param   UINT8** FileBuffer - Output file buffer read from FFS
             UINTN *BufferSize  - Output buffer size.

    @retval  EFI_SUCCESS    - Json File found in FFS
             EFI_NOT_FOUND  - Json File not found in FFS
             EFI_INVALID_PARAMETER  - Either FileBuffer or BufferSize is NULL.
**/
EFI_STATUS
GetConfigFileBufferFromFfs(
    OUT UINT8 **FileBuffer, 
    OUT UINTN *BufferSize
)
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    EFI_FIRMWARE_VOLUME_PROTOCOL *FV = (EFI_FIRMWARE_VOLUME_PROTOCOL*)NULL;
    EFI_GUID FFSGuid = JSON_CONFIG_FFS_GUID;
    EFI_HANDLE *HandleBuffer = (EFI_HANDLE *)NULL;
    UINT8 *LookUpTableFFS = (UINT8 *)NULL;
    UINTN Index, HandleCount = 0, FfsSize = 0;
    UINT32 AuthenticationStatus = 0;

    if((NULL == FileBuffer) || (NULL == BufferSize))
        return EFI_INVALID_PARAMETER;

    Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiFirmwareVolume2ProtocolGuid, NULL, &HandleCount, &HandleBuffer);
    if(EFI_ERROR(Status))
        return Status;

    //Search FFS in available FVs
    for(Index = 0; Index < HandleCount; Index++)
    {
        Status = gBS->HandleProtocol(HandleBuffer[Index],&gEfiFirmwareVolume2ProtocolGuid, (VOID**)&FV );
        if(EFI_ERROR(Status))
            continue;

        Status = FV->ReadSection(FV,&FFSGuid,EFI_SECTION_RAW,0,(VOID**)&LookUpTableFFS, &FfsSize, &AuthenticationStatus);
        if(EFI_ERROR(Status))
            continue;

        *FileBuffer = LookUpTableFFS;
        *BufferSize = FfsSize;
        
        FreePool((void*)HandleBuffer);

        return Status;
    }
    return EFI_NOT_FOUND;
}

/**
    This Function Fills the look up table for local use (used during UpdateCapsule) 
    from JSON file buffer(for both BS controls as Well as RT controls). It uses Json library to parse the file buffer.

    @param   cJSON *jsonCtrlList - JSON Pointer

    @retval  EFI_SUCCESS    - Look up table has been filled successfully
             EFI_NOT_FOUND  - Variables list or Controls list is/are not found.
 **/
EFI_STATUS
FillConfigLookupTable(
        cJSON *jsonCtrlList
)
{
    cJSON *jsonChild = (cJSON*)NULL;
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN VarIndex = 0;


    if(NULL == jsonCtrlList)
        return EFI_INVALID_PARAMETER;

    jsonCtrlList = jsonCtrlList->child;

    while(jsonCtrlList)
    {
        jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"QuestionID");
        if(jsonChild && jsonChild->type == cJSON_String)
        {
            gConfigLookUpTable[gControlIndex].MappingID = AllocateZeroPool((AsciiStrLen(jsonChild->valuestring)+1) * sizeof(CHAR16));
            if(NULL == gConfigLookUpTable[gControlIndex].MappingID)
            {
                Status = EFI_OUT_OF_RESOURCES;
                goto END;
            }

             AsciiStrToUnicodeStrS( 
	                 (CHAR8*)jsonChild->valuestring, 
					(CHAR16*)gConfigLookUpTable[gControlIndex].MappingID,
                     AsciiStrLen(jsonChild->valuestring)+1); 
        }

        jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"VarOffset");
        if(jsonChild && jsonChild->type == cJSON_Number)
            gConfigLookUpTable[gControlIndex].Offset = (UINTN)jsonChild->valueint; 

        jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"VarWidth");
        if(jsonChild && jsonChild->type == cJSON_Number)
            gConfigLookUpTable[gControlIndex].Width = (UINTN)jsonChild->valueint; 

        jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"VarID");
        if(jsonChild && (jsonChild->type == cJSON_Number))
        {
            EFI_STATUS VarStatus = GetVairableIndex((UINTN)jsonChild->valueint, &VarIndex);

            if(VarStatus == EFI_SUCCESS)   
                gConfigLookUpTable[gControlIndex].VarId = VarIndex;

        }

        //Validation Related details
        jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"QuestionType");
        if(jsonChild && jsonChild->type == cJSON_String)
        {
            if(!AsciiStrCmp(jsonChild->valuestring, "Numeric"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_NUMERIC;
            else if(!AsciiStrCmp(jsonChild->valuestring, "CheckBox"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_CHECKBOX;
            else if(!AsciiStrCmp(jsonChild->valuestring, "OneOf"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_POPUPSEL;
            else if(!AsciiStrCmp(jsonChild->valuestring, "String"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_POPUP_STRING;
            else if(!AsciiStrCmp(jsonChild->valuestring, "Password"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_PASSWORD;
            else if(!AsciiStrCmp(jsonChild->valuestring, "OrderedList"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_ORDERED_LIST;
            else if(!AsciiStrCmp(jsonChild->valuestring, "Date"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_DATE;
            else if(!AsciiStrCmp(jsonChild->valuestring, "Time"))
                gConfigLookUpTable[gControlIndex].ControlType = CONTROL_TYPE_TIME;
        }

        switch(gConfigLookUpTable[gControlIndex].ControlType)
        {
        case CONTROL_TYPE_NUMERIC:
        {         
            gConfigLookUpTable[gControlIndex].ValidateData = AllocateZeroPool(sizeof(VALIDATE_DATA));
            if(NULL == gConfigLookUpTable[gControlIndex].ValidateData)
            {
                Status = EFI_OUT_OF_RESOURCES;
                goto END;
            }

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"MinValue");
            if(jsonChild && jsonChild->type == cJSON_Number)        
                gConfigLookUpTable[gControlIndex].ValidateData->MinMax.Min = jsonChild->valueint;

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"MaxValue");
            if(jsonChild && jsonChild->type == cJSON_Number)        
                gConfigLookUpTable[gControlIndex].ValidateData->MinMax.Max = jsonChild->valueint;

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"Base");
            if(jsonChild && jsonChild->type == cJSON_String && !AsciiStrCmp(jsonChild->valuestring, "Signed"))        
                gConfigLookUpTable[gControlIndex].ValidateData->MinMax.IsSigned = TRUE;

        }
        break;
        case CONTROL_TYPE_POPUP_STRING:
        case CONTROL_TYPE_PASSWORD:
        {            
            gConfigLookUpTable[gControlIndex].ValidateData = AllocateZeroPool(sizeof(VALIDATE_DATA));
            if(NULL == gConfigLookUpTable[gControlIndex].ValidateData)
            {
                Status = EFI_OUT_OF_RESOURCES;
                goto END;
            }

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"MinLength");
            if(jsonChild && jsonChild->type == cJSON_Number)        
                gConfigLookUpTable[gControlIndex].ValidateData->MinMax.Min = jsonChild->valueint;

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"MaxLength");
            if(jsonChild && jsonChild->type == cJSON_Number)        
                gConfigLookUpTable[gControlIndex].ValidateData->MinMax.Max = jsonChild->valueint;

        }
        break;
        case CONTROL_TYPE_POPUPSEL:
        case CONTROL_TYPE_ORDERED_LIST:
        {
            cJSON *JsonObj = (cJSON *)NULL;
            UINTN OptionCount = 0;
            UINT8 OptionWidth = 0;

            if(gConfigLookUpTable[gControlIndex].ControlType == CONTROL_TYPE_ORDERED_LIST)
            {
                jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"OptionWidth");
                if(jsonChild && (jsonChild->type == cJSON_Number))
                    OptionWidth = (UINT8)jsonChild->valueint;
            }

            jsonChild = cJSON_GetObjectItem(jsonCtrlList,(char *)"Options");

            if(jsonChild)
            {    
                OptionCount = cJSON_GetArraySize(jsonChild);

                gConfigLookUpTable[gControlIndex].ValidateData = AllocateZeroPool(sizeof(VALIDATE_DATA) + ((OptionCount -1) * sizeof(UINTN)));
                if(NULL == gConfigLookUpTable[gControlIndex].ValidateData)
                {
                    Status = EFI_OUT_OF_RESOURCES;
                    goto END;
                }

                gConfigLookUpTable[gControlIndex].ValidateData->Options.OptionCount = OptionCount;
                gConfigLookUpTable[gControlIndex].ValidateData->Options.OptionWidth = OptionWidth;

                OptionCount = 0;
                jsonChild = jsonChild->child;
                while(jsonChild)
                {
                    JsonObj = cJSON_GetObjectItem(jsonChild,(char *)"OptionValue");
                    if(JsonObj && JsonObj->type == cJSON_Number)        
                        gConfigLookUpTable[gControlIndex].ValidateData->Options.OptionList[OptionCount++] 
                                                                                           = (UINT64)JsonObj->valueint;
                    jsonChild = jsonChild->next;
                }
            }
        }
        break;
        default:
            break;
        } 

        jsonCtrlList = jsonCtrlList->next;
        gControlIndex++;
    }

    END:      
    return Status;
}

/**
    This Function creates the look up table for local use (used during UpdateCapsule) 
    from JSON file buffer. It uses Json library to parse the file buffer.

    @param   UINT8* FileBuffer - JSON file buffer

    @retval  EFI_SUCCESS    - Json File found in FFS
             EFI_NOT_FOUND  - Variables list or Controls list is/are not found.
             EFI_INVALID_PARAMETER  - FileBuffer is NULL or can't parse the JSON buffer.
**/
EFI_STATUS
InitConfigLookUpTable(
    UINT8* FileBuffer
    )
{
    cJSON *jsonChild = (cJSON*)NULL;
    cJSON *jsonVarList = (cJSON*)NULL;
    cJSON *jsonBsCtrlList = (cJSON*)NULL;
    cJSON *jsonRtCtrlList = (cJSON*)NULL;
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN Index = 0;


    if(NULL == FileBuffer)
        return EFI_INVALID_PARAMETER;

    //Parse the JSON buffer
    gJsonRootNode = cJSON_Parse((const char *)FileBuffer);
    if(NULL == gJsonRootNode)
        return EFI_INVALID_PARAMETER;

    //Get Variables list.
    jsonVarList = cJSON_GetObjectItem(gJsonRootNode,(char *)"Variables");
    if(jsonVarList)
        gVariableCount = cJSON_GetArraySize(jsonVarList);

    if((NULL == jsonVarList) || !gVariableCount) 
    {
        // The file should have variables list. 
        // Otherwise, finding variable details related to mapping ID is not possible.
        Status = EFI_NOT_FOUND;
        goto END;
    }

    gVarTable = (VAR_LIST *)AllocateZeroPool(gVariableCount * sizeof(VAR_LIST));
    if(NULL == gVarTable)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto END;
    }

    //Initialize Variable table which will be used during look up table initialization.
    jsonVarList = jsonVarList->child;
    for(Index = 0; jsonVarList && Index < gVariableCount; Index++, jsonVarList = jsonVarList->next)
    {
        jsonChild = cJSON_GetObjectItem(jsonVarList,(char *)"VarName");
        if(jsonChild && (jsonChild->type == cJSON_String))
            AsciiStrToUnicodeStrS(jsonChild->valuestring, gVarTable[Index].Varname, sizeof(gVarTable[Index].Varname) / sizeof(CHAR16));

        


        jsonChild = cJSON_GetObjectItem(jsonVarList,(char *)"VarGuid");
        if(jsonChild && (jsonChild->type == cJSON_String))
            AsciiStrToGuid(jsonChild->valuestring,&gVarTable[Index].VarGuid);

        jsonChild = cJSON_GetObjectItem(jsonVarList,(char *)"VarID");
        if(jsonChild && (jsonChild->type == cJSON_Number))
            gVarTable[Index].VarID = (UINTN)jsonChild->valueint;

    }

    //Get BS Questions list.
    jsonBsCtrlList = cJSON_GetObjectItem(gJsonRootNode,"BS Questions");
    if(jsonBsCtrlList)
        gLookUpTableItemCount = cJSON_GetArraySize(jsonBsCtrlList);

    //Get RT Questions list.
    jsonRtCtrlList = cJSON_GetObjectItem(gJsonRootNode,"RT Questions");
    if(jsonRtCtrlList)
        gLookUpTableItemCount += cJSON_GetArraySize(jsonRtCtrlList);

    if(((NULL == jsonBsCtrlList) && (NULL == jsonRtCtrlList)) || !gLookUpTableItemCount)
    {
        Status = EFI_NOT_FOUND;
        goto END;
    }

    //Allocate memory for look up table    
    gConfigLookUpTable = AllocateZeroPool(gLookUpTableItemCount * sizeof(CONFIG_LOOK_UP_TABLE));
    if(NULL == gConfigLookUpTable)
    {
        Status = EFI_OUT_OF_RESOURCES;
        goto END;
    }

    //Initialize look up table from JSON data.

    //Fill look up table with BS control from JSON data 
    Status = FillConfigLookupTable(jsonBsCtrlList);
    if(EFI_ERROR(Status))
        goto END;

    //Fill look up table with RT control from JSON data
    Status = FillConfigLookupTable(jsonRtCtrlList);

    END:  
    if(!EFI_ERROR(Status))
        SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: created look up table successfully.\n");
    else
        SETUP_DEBUG_TSE("\n[TSE] JsonCapsule: Look up table creation failed %r.\n", Status);

    return Status; 

}

/**
    This Function finds the variable index based on VarID.

    @param  UINTN VarID     - VarID of a variable
            UINTN *VarIndex - Variable array index which has VarID as input VarID.

    @retval  EFI_SUCCESS    - Json File found in FFS
             EFI_NOT_FOUND  - Variables list or Controls list is/are not found.
             EFI_INVALID_PARAMETER  - FileBuffer is NULL or can't parse the JSON buffer.
**/
EFI_STATUS 
GetVairableIndex(
    IN UINTN VarID,
    OUT UINTN *VarIndex
    )
{
    UINTN Index;

    if(NULL == VarIndex)
        return EFI_INVALID_PARAMETER;

    if((NULL == gVarTable) || !gVariableCount)
        return EFI_UNSUPPORTED;

    for(Index = 0; Index < gVariableCount; Index++)
    {
        if(gVarTable[Index].VarID == VarID)
        {
            *VarIndex = Index;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;        
}

/**
    This function cleans up the Config look up table.

    @param   VOID

    @retval  VOID
**/
VOID
CleanUpLookUpTable( 
    VOID 
    )
{
    UINTN Index;
    
    if(NULL == gConfigLookUpTable)
        return;
    
    for(Index = 0; Index < gLookUpTableItemCount; Index++)
        if(gConfigLookUpTable[Index].MappingID)
            FreePool((void*)gConfigLookUpTable[Index].MappingID);
    
    FreePool((void*)gConfigLookUpTable);

    gConfigLookUpTable = NULL;
    gLookUpTableItemCount = 0;
}

/**
    This function cleans up the Config look up table.

    @param   VOID

    @retval  VOID
**/
VOID
CleanUpVariableTable( 
    VOID 
    )
{
    UINTN Index;

    if(NULL == gConfigVarList)
        return;

    for(Index = 0; Index < gVariableCount; Index++)
        if(gConfigVarList[Index].Buffer)
            FreePool((void*)gConfigVarList[Index].Buffer);
    
    FreePool((void*)gConfigVarList);

    gConfigVarList = NULL;
}

/**
    This function is to check the value of signed numeric control that it is negative or not.

    @param InputVal     : Input Control buffer 
    @param Size         : Size of the control
    
    @retval Return UINT8 : Return 0 if not negative, Return 1 if negative

**/
UINT8 
IsNegative(
        VOID *Buffer,
        UINTN Size
)
{
    UINT64 Num = 0;
    UINT8 RetVal = 0;

    switch(Size)
    {
    case 1:
        Num = *(INT8*)Buffer;
        if((INT8)Num < 0)
            RetVal = 1;
        break;
        
    case 2:
        Num = *(INT16*)Buffer;
        if((INT16)Num < 0)
            RetVal = 1;
        break;
        
    case 4:
        Num = *(INT32*)Buffer;
        if((INT32)Num < 0)
            RetVal = 1;
        break;
        
    case 8:
        Num = *(INT64*)Buffer;
        if((INT64)Num < 0)
            RetVal = 1;
        break;
        
    default:
        Num = *(INT8*)Buffer;
        if((INT8)Num < 0)
            RetVal = 1;
        break;
    }
    return RetVal;
}

/**
    This function Returns the converted string from the given number

    @param ControlDataWidth : Width of the control
    @param Buffer           : Control Buffer
    
    @retval Return CHAR8*   : Converted String 

**/
CHAR8* 
ConvVfrNumToString(
    UINTN ControlDataWidth,
    VOID *Buffer
    )
{
    CHAR8 *Text =(CHAR8*)NULL;
    UINT64 Num;

    Text = AllocateZeroPool(MAX_BUFFER_SIZE);
    if(NULL == Text)
        return NULL;

    switch(ControlDataWidth)
    {
    case 1:
        Num = *(INT8*)Buffer;
        Num = (UINT8)(-(INT8)Num);
        AsciiSPrint (Text, MAX_BUFFER_SIZE, "-%u", Num);
        break;
        
    case 2:
        Num = *(INT16*)Buffer;
        Num = (UINT16)(-(INT16)Num);
        AsciiSPrint(Text, MAX_BUFFER_SIZE, "-%u", Num);
        break;
        
    case 4:
        Num = *(INT32*)Buffer;
        Num = (UINT32)(-(INT32)Num);
        AsciiSPrint(Text, MAX_BUFFER_SIZE, "-%u", Num);
        break;
        
    case 8:
        Num = *(INT64*)Buffer;
        Num = (UINT64)(-(INT64)Num);
        AsciiSPrint(Text, MAX_BUFFER_SIZE, "-%lu", Num);
        break;
    default:
        Num = *(INT8*)Buffer;
        Num = (UINT8)(-(INT8)Num);
        AsciiSPrint(Text, MAX_BUFFER_SIZE, "-%u", Num);
        break;
    }

    return Text;
}
