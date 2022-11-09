//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


//**********************************************************************
/** @file 
 Implementation of the AMI JSON Common library.

 **/
//**********************************************************************
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiJsonLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>


AMI_JSON_OBJECT
RfCheckAndCreateObject(
        AMI_JSON_OBJECT       jsonParent,
        CHAR8                 *ObjectName)
{
    EFI_STATUS             Status = EFI_SUCCESS;
    AMI_JSON_OBJECT        jsonObject = NULL;
    
    
    jsonObject = JsonGetObjectItem(jsonParent, ObjectName); 
    
    if(jsonObject == NULL){
        jsonObject = JsonCreateObject();
        if(jsonObject != NULL){
            Status = JsonAddItemToObject(jsonParent, ObjectName, jsonObject);
        }
    }
    if(EFI_ERROR(Status)){
        if(jsonObject != NULL){
            JsonDelete(jsonObject);
            jsonObject = NULL;
        }
    }
    return jsonObject;
}


AMI_JSON_OBJECT
RfCheckAndCreateArray(
        AMI_JSON_OBJECT       jsonParent,
        CHAR8                 *ArrayName)
{
    EFI_STATUS             Status = EFI_SUCCESS;
    AMI_JSON_OBJECT        jsonArray = NULL;
    
    
    jsonArray = JsonGetObjectItem(jsonParent, ArrayName); 
    
    if(jsonArray == NULL){
        jsonArray = JsonCreateArray();
        if(jsonArray != NULL){
            Status = JsonAddItemToObject(jsonParent, ArrayName, jsonArray);
        }
    }
    if(EFI_ERROR(Status)){
        if(jsonArray != NULL){
            JsonDelete(jsonArray);
            jsonArray = NULL;
        }
    }
    return jsonArray;
}

AMI_JSON_OBJECT
RfCheckAndCreateItemInArray(
        AMI_JSON_OBJECT     jsonArray,
        UINTN               ItemIndex)
{
    EFI_STATUS          Status;
    AMI_JSON_OBJECT     jsonItem = NULL;
    
    if(JsonGetArraySize(jsonArray) <= ItemIndex){
        jsonItem = JsonCreateObject();
        if(jsonItem == NULL){
            return NULL;
        }
        Status = JsonAddItemToArray(jsonArray, jsonItem);
        if(EFI_ERROR(Status)){
            if(jsonItem != NULL){
                JsonDelete(jsonItem);
                jsonItem = NULL;
            }
        }
    }
    else{
        jsonItem = JsonGetArrayItem(jsonArray, ItemIndex);
    }
    return jsonItem;
}

CHAR16*
RfGetODataValueInObjectByName(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchName)
{
    CHAR16              *ODataValue = NULL;
    AMI_JSON_OBJECT     JsonSearchObject, JsonOData;
    
    JsonSearchObject = JsonSearchForObject(JsonObject, SearchName);
    if(JsonSearchObject != NULL && JsonValueIsObject(JsonSearchObject)){
        JsonOData = JsonGetObjectItem(JsonSearchObject, "@odata.id");
        if(JsonOData != NULL && JsonValueIsString(JsonOData)){
            ODataValue = JsonGetUnicodeString(JsonOData);
        }
    }
    return ODataValue;
}

CHAR8*
RfGetAsciiODataValueInObjectByName(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchName)
{
    CHAR8               *ODataValue = NULL;
    AMI_JSON_OBJECT     JsonSearchObject, JsonOData;
    
    JsonSearchObject = JsonSearchForObject(JsonObject, SearchName);
    if(JsonSearchObject != NULL && JsonValueIsObject(JsonSearchObject)){
        JsonOData = JsonGetObjectItem(JsonSearchObject, "@odata.id");
        if(JsonOData != NULL && JsonValueIsString(JsonOData)){
            ODataValue = JsonGetAsciiString(JsonOData);
        }
    }
    return ODataValue;
}
