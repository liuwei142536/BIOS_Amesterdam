//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    Ami JSON library functions definition

**/

#ifndef _AMI_REDFISH_JSON_LIBRARY_H_
#define _AMI_REDFISH_JSON_LIBRARY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/AmiCJsonLib.h>

typedef void* AMI_JSON_OBJECT;

EFI_STATUS
JsonAddStringToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, char *AttributeValue);

EFI_STATUS
JsonAddNumAsStringToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, char *AttributeValue);

EFI_STATUS
JsonAddNumberIntToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, INT64 AttributeValue);

EFI_STATUS
JsonAddBoolToObject(AMI_JSON_OBJECT JsonObject, char *AttributeName, BOOLEAN AttributeValue);

EFI_STATUS
JsonAddItemToArray(AMI_JSON_OBJECT JsonArray, AMI_JSON_OBJECT JsonItem);

EFI_STATUS
JsonAddItemToObject(AMI_JSON_OBJECT JsonObject, char *ItemName, AMI_JSON_OBJECT JsonItem);

AMI_JSON_OBJECT
JsonCreateArray(void);

AMI_JSON_OBJECT
JsonCreateObject(void);

void
JsonDelete(AMI_JSON_OBJECT JsonObject);

char*
JsonPrint(AMI_JSON_OBJECT JsonObject);

/* Free buffer allocated by JsonPrint. */
void 
JsonFree(void *ptr);

AMI_JSON_OBJECT
JsonParse(const char *text);

AMI_JSON_OBJECT 
JsonGetObjectItem(AMI_JSON_OBJECT JsonObject,const char *ItemName);

UINTN
JsonGetArraySize(AMI_JSON_OBJECT JsonArray);

AMI_JSON_OBJECT
JsonGetArrayItem(AMI_JSON_OBJECT JsonArray, UINTN Index);

AMI_JSON_OBJECT
JsonCreateString(CHAR8 *value);

AMI_JSON_OBJECT
JsonCreateNumberInt(INT64 value);

BOOLEAN
JsonValueIsObject(AMI_JSON_OBJECT JsonValue);

BOOLEAN
JsonValueIsArray(AMI_JSON_OBJECT JsonValue);

BOOLEAN
JsonValueIsString(AMI_JSON_OBJECT JsonValue);

BOOLEAN
JsonValueIsBoolean(AMI_JSON_OBJECT JsonValue);

BOOLEAN
JsonValueIsNumber(AMI_JSON_OBJECT JsonValue);

INT64
JsonGetNumber(AMI_JSON_OBJECT JsonValue);

CHAR8*
JsonGetAsciiString(AMI_JSON_OBJECT JsonValue);

CHAR16*
JsonGetUnicodeString(AMI_JSON_OBJECT JsonValue);

BOOLEAN
JsonGetBoolean(AMI_JSON_OBJECT JsonValue);

void 
JsonInitSeed(UINT32 SeedForJsonLib);

AMI_JSON_OBJECT
JsonDuplicate(AMI_JSON_OBJECT JsonValue);

AMI_JSON_OBJECT
JsonSearchForObject(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchObjectName);

VOID
JsonReplaceItemInObject(
        AMI_JSON_OBJECT JsonObject,
        CHAR8           *ItemName,
        AMI_JSON_OBJECT JsonNewItem);

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
        UINTN           *KeyCount);


#include <Library/AmiJsonLibCommon.h>

#endif

