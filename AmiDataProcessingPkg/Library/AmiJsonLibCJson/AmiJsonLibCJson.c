//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


//**********************************************************************
/** @file 
 Implementation of the abstract AMI JSON library based on cJSON library.

 **/
//**********************************************************************
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiJsonLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/AmiCJsonLib.h>

EFI_STATUS
JsonAddStringToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, char *AttributeValue)
{
    cJSON_AddStringToObject(JsonObject, AttributeName, AttributeValue);
    return EFI_SUCCESS;
}

EFI_STATUS
JsonAddNumAsStringToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, char *AttributeValue)
{
    cJSON_AddNumAsStringToObject(JsonObject, AttributeName, AttributeValue);
    return EFI_SUCCESS;
}


EFI_STATUS
JsonAddNumberIntToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, INT64 AttributeValue)
{
    cJSON_AddNumberIntToObject(JsonObject, AttributeName, AttributeValue);
    return EFI_SUCCESS;
}


EFI_STATUS
JsonAddBoolToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, BOOLEAN AttributeValue)
{
    cJSON_AddBoolToObject(JsonObject, AttributeName, AttributeValue);
    return EFI_SUCCESS;
}

EFI_STATUS
JsonAddItemToArray(AMI_JSON_OBJECT JsonArray, AMI_JSON_OBJECT JsonItem)
{
    cJSON_AddItemToArray(JsonArray, JsonItem);
    return EFI_SUCCESS;
}

EFI_STATUS
JsonAddItemToObject(AMI_JSON_OBJECT JsonObject, char *ItemName, AMI_JSON_OBJECT JsonItem)
{
    cJSON_AddItemToObject(JsonObject, ItemName, JsonItem);
    return EFI_SUCCESS;
}

AMI_JSON_OBJECT
JsonCreateArray(void)
{
    return cJSON_CreateArray();
}

AMI_JSON_OBJECT
JsonCreateObject(void)
{
    return cJSON_CreateObject();
}

void
JsonDelete(AMI_JSON_OBJECT JsonObject)
{
    cJSON_Delete(JsonObject);
    return;
}

char*
JsonPrint(AMI_JSON_OBJECT JsonObject)
{
    return cJSON_Print(JsonObject);
}

/* Free buffer allocated by Json_Print. */
void 
JsonFree(void *ptr)
{
    FreePool(ptr);
    return;
}


AMI_JSON_OBJECT
JsonParse(const char *text){
    return cJSON_Parse((CHAR8*)text);
}

AMI_JSON_OBJECT 
JsonGetObjectItem(AMI_JSON_OBJECT JsonObject,const char *ItemName)
{
    return cJSON_GetObjectItem(JsonObject, (CHAR8*)ItemName);
}

UINTN
JsonGetArraySize(AMI_JSON_OBJECT JsonArray)
{
    return cJSON_GetArraySize(JsonArray);
}

BOOLEAN
JsonValueIsObject(AMI_JSON_OBJECT JsonValue)
{
    return ((cJSON*)JsonValue)->type == cJSON_Object;
}

BOOLEAN
JsonValueIsArray(AMI_JSON_OBJECT JsonValue)
{
    return ((cJSON*)JsonValue)->type == cJSON_Array;
}

BOOLEAN
JsonValueIsString(AMI_JSON_OBJECT JsonValue)
{
    return ((cJSON*)JsonValue)->type == cJSON_String;
}

BOOLEAN
JsonValueIsNumber(AMI_JSON_OBJECT JsonValue)
{
    return ((cJSON*)JsonValue)->type == cJSON_Number;
}

BOOLEAN
JsonValueIsBoolean(AMI_JSON_OBJECT JsonValue)
{
    return ((((cJSON*)JsonValue)->type == cJSON_True) || (((cJSON*)JsonValue)->type == cJSON_False));
}

void 
JsonInitSeed(UINT32 SeedForJsonLib)
{
    return;
}

AMI_JSON_OBJECT
JsonCreateString(CHAR8    *value)
{
    return cJSON_CreateString(value);
}

AMI_JSON_OBJECT
JsonCreateNumberInt(INT64 value)
{
    return cJSON_CreateNumberInt(value);
}


AMI_JSON_OBJECT
JsonGetArrayItem(AMI_JSON_OBJECT JsonArray, UINTN Index)
{
    if(JsonValueIsArray(JsonArray) && JsonGetArraySize(JsonArray) > Index){
        return cJSON_GetArrayItem(JsonArray, (int)Index);
    }
    else{
        return NULL;
    }
}

INT64
JsonGetNumber(AMI_JSON_OBJECT JsonValue)
{
    if(JsonValueIsNumber(JsonValue)){
        return ((cJSON*)JsonValue)->valueint;
    }
    else{
        return 0;
    }
}

CHAR8*
JsonGetAsciiString(AMI_JSON_OBJECT JsonValue)
{
    if(JsonValueIsString(JsonValue)){
        return ((cJSON*)JsonValue)->valuestring;
    }
    else{
        return NULL;
    }
}

CHAR16*
JsonGetUnicodeString(AMI_JSON_OBJECT JsonValue)
{
    CONST CHAR8    *Utf8Str;
    CHAR16         *Ucs2Str;
    UINTN           StrLen;

    Utf8Str = JsonGetAsciiString(JsonValue);
    if(Utf8Str == NULL){
        return NULL;
    }
    
    StrLen = (AsciiStrLen(Utf8Str) + 1) * sizeof(CHAR16);
    Ucs2Str = AllocatePool(StrLen);
    if(Ucs2Str != NULL){
        UnicodeSPrint(Ucs2Str, StrLen, L"%a", Utf8Str);
    }
    return Ucs2Str;
}

BOOLEAN
JsonGetBoolean(AMI_JSON_OBJECT JsonValue)
{
    if(JsonValueIsBoolean(JsonValue)){
        return ((cJSON*)JsonValue)->type == cJSON_True;
    }
    else{
        return FALSE;
    }
}


AMI_JSON_OBJECT
JsonDuplicate(AMI_JSON_OBJECT JsonValue)
{
    return cJSON_Duplicate(JsonValue, 1);
}

AMI_JSON_OBJECT
JsonSearchForObject(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchName)
{
    UINT32                  LinksSize = 0, i;
    cJSON                   *jsonSelf = NULL, *JsonReturn = NULL;
    
    if(JsonObject == NULL){
        return NULL;
    }
   
    LinksSize = cJSON_GetArraySize((cJSON*)JsonObject);
    
    for(i = 0; i < LinksSize; i++){
        jsonSelf = cJSON_GetArrayItem((cJSON*)JsonObject, i);
        if(jsonSelf == NULL) continue;
        if(AsciiStrCmp(jsonSelf->string, SearchName) == 0){
            return jsonSelf;
        }
        if(jsonSelf->type == cJSON_Object){
            JsonReturn = JsonSearchForObject(jsonSelf, SearchName);
        }
        if(JsonReturn != NULL){
            break;
        }
    }
    return JsonReturn;
}

VOID
JsonReplaceItemInObject(
        AMI_JSON_OBJECT JsonObject,
        CHAR8           *ItemName,
        AMI_JSON_OBJECT JsonNewItem)
{
    cJSON_ReplaceItemInObject(JsonObject, ItemName, JsonNewItem);
}


/**
 * The function is used to enumerate all items in a JSON object
 * 
 * Caller should be responsible to free the returned item array reference.
 * But contained items are read only and must not be modified or freed.
 * 
 * @param[in]  JsonObject - The provided JSON object for enumeration
 * @param[out] KeyCount - The count of items in this JSON object
 * @return Return an array of the enumerated items in this JSON object or NULL
 */

CHAR8**
JsonGetObjectKeys(
        AMI_JSON_OBJECT JsonObject,
        UINTN           *KeyCount)
{
    UINTN   Index;
    CHAR8   **KeyArray = NULL;
    cJSON   *jsonItem;
    if(((cJSON*)JsonObject)->type != cJSON_Object){
        *KeyCount = 0;
        return NULL;
    }
    *KeyCount = cJSON_GetArraySize(JsonObject);
    if(*KeyCount != 0){
        KeyArray = (CHAR8**)AllocateZeroPool(*KeyCount * sizeof(CHAR8*));
        if(KeyArray != NULL){
            for(Index = 0; Index < *KeyCount; Index++){
                jsonItem = cJSON_GetArrayItem((cJSON*)JsonObject, (int)Index);
                KeyArray[Index] = jsonItem->string;
            }
        }
    }
    return KeyArray;
}

