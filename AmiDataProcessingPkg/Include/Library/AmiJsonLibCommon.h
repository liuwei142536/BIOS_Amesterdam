//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
    AMI JSON Common library functions definition

**/

#ifndef __AMI_JSON_LIBRARY_COMMON_H__
#define __AMI_JSON_LIBRARY_COMMON_H__

AMI_JSON_OBJECT
RfCheckAndCreateObject(
        AMI_JSON_OBJECT       jsonParent,
        CHAR8                 *ObjectName);


AMI_JSON_OBJECT
RfCheckAndCreateArray(
        AMI_JSON_OBJECT       jsonParent,
        CHAR8                 *ArrayName);

AMI_JSON_OBJECT
RfCheckAndCreateItemInArray(
        AMI_JSON_OBJECT     jsonArray,
        UINTN               ItemIndex);

CHAR16*
RfGetODataValueInObjectByName(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchName);

CHAR8*
RfGetAsciiODataValueInObjectByName(
        AMI_JSON_OBJECT             JsonObject,
        CHAR8                       *SearchName);

#endif

