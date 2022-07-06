//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file
    AmiHiiLib library instance
**/
#include <Protocol/LoadedImage.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiConfigRouting.h>
#include <Protocol/HiiPackageList.h>
#include <Protocol/HiiString.h>

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/AmiHiiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>


typedef struct _NAME_PATTERN
{
    UINTN Length;
    CHAR16 *Tag;
} NAME_PATTERN;

const NAME_PATTERN gGuidPattern   = { 10, L"GUID=" };
const NAME_PATTERN gNamePattern   = { 10, L"NAME=" };
const NAME_PATTERN gPathPattern   = { 10, L"PATH=" };
const NAME_PATTERN gOffsetPattern = { 14, L"OFFSET=" };

// *************************** Service functions (not for public use) **********************

/**
    Reads an EFI variable with the name Name and GUID Guid.
    If variable is found, variable attributes, size, and data are returned
    using Attributes, DataSize, and Data parameters and EFI_SUCCESS is
    returned. If variable is not found, EFI_NOT_FOUND is returned. The output
    buffer is specified by a double indirection of the Data parameter. The
    input value of *Data is used to determine if the output buffer is caller
    allocated or is dynamically allocated by this functions. If the input
    value of  *Data is not NULL, it indicates that the output buffer is caller
    allocated. In this case, the input value of *DataSize indicates the size
    of the caller-allocated output buffer. If the output buffer is not large
    enough to contain the variable data, it is freed and new   buffer of
    sufficient size is allocated from boot services pool memory, which will be
    returned in *Data.  If the input value of *Data not NULL, it indicates
    that the output buffer is allocated by LocalGetEfiVar routine. In this
    case, LocalGetEfiVar will allocate an appropriately sized buffer from boot
    services pool memory, which will be returned in *Data.

    @param sName -  Pointer to the null-terminated Unicode string with name of the EFI
                    Variable to read.
    @param pGuid - Pointer to GUID of the EFI Variable to read.
    @param pAttributes - OPTIONAL Pointer to the Attributes of the EFI Variable.
    @param pDataSize - Pointer to the variable data size.
    @param ppData - Pointer to the variable data.

    @retval - Status, based on result.
**/

EFI_STATUS LocalGetEfiVar(
    IN CHAR16 *sName, IN EFI_GUID *pGuid,
    OUT UINT32 *pAttributes OPTIONAL,
    IN OUT UINTN *pDataSize, OUT VOID **ppData
)
{
    EFI_STATUS Status;
    if (!*ppData) *pDataSize=0;
    Status = gRT->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    if (!EFI_ERROR(Status)) return Status;
    if (Status==EFI_BUFFER_TOO_SMALL)
    {
        if (*ppData) FreePool(*ppData);
        if (!(*ppData=AllocateZeroPool(*pDataSize))) return EFI_OUT_OF_RESOURCES;
        Status = gRT->GetVariable(sName, pGuid, pAttributes, pDataSize, *ppData);
    }
    return Status;
}

/**
    Checks whether passed Unicode Char represents Hex number

    @param Char - Unicode Char

    @retval - TRUE if passed Char is a Hex number, FALSE other vice

**/

BOOLEAN LocalCheckIfHexDigit(CHAR16 Char)
{
    if(Char < 0x30 || Char > 0x66)
        return FALSE;

    if(Char > 0x39 && Char < 0x41)
        return FALSE;

    if(Char > 0x46 && Char < 0x61)
        return FALSE;

    return TRUE;
}

/**
    Returns Hex value of passed Character

    @param Char - CHAR8 Character

    @retval - Hex value

**/

UINT8 LocalHexValue(CHAR8 Char)
{
    if(Char >= 0x30 && Char <= 0x39)        //symbols 0...9
        return Char - 0x30;

    if(Char >= 0x41 && Char <= 0x46)        //symbols A...F
        return Char - 0x37;

    return Char - 0x57;                     //symbols a...f
}

/**
    Checks passed String against the Patern

    @param String - The input string to check
    @param Pattern - Pattern to look for

    @retval - TRUE if pattern exist, FALSE other vice

**/

static BOOLEAN CheckForPattern(CHAR16 *String, const NAME_PATTERN *Pattern)
{
    if(CompareMem ((VOID*)String, (VOID*)Pattern->Tag, Pattern->Length))
        return FALSE;
    else
        return TRUE;
}

/**
    Convert the string to a null terminated Unicode string buffer

    @param String - The input string to convert
    @param Size - The number of characters to convert
    @param Block - The buffer to store the output

    @retval - VOID

**/

VOID LocalStringToChar16(
    IN CHAR16 *String,
    IN OUT UINTN  *Size,
    OUT CHAR16 *Block
)
{
    UINTN i, j;

    i = 0;
    j = 0;

    while( LocalCheckIfHexDigit(String[i]) && LocalCheckIfHexDigit(String[i + 1]) &&
           LocalCheckIfHexDigit(String[i + 2]) && LocalCheckIfHexDigit(String[i + 3]) &&
           j < *Size - 1 )
    {
        Block[j]  = LocalHexValue((UINT8)String[i]) & 0x000f;
        Block[j] <<= 4;
        Block[j] |= LocalHexValue((UINT8)String[i + 1]);
        Block[j] <<= 4;
        Block[j] |= LocalHexValue((UINT8)String[i + 2]);
        Block[j] <<= 4;
        Block[j] |= LocalHexValue((UINT8)String[i + 3]);

        i += 4;
        j++;
    }
    *Size = j;      //number of words processed
    Block[j] = 0;   //put null-terminator
}

/**
    Convert a length of the passed Unicode string buffer into a buffer of hexadecimal values

    @param String  - The string buffer
    @param StringSize - The number of string buffer characters to convert
    @param Block - The output buffer to store the converted characters

    @retval - VOID

**/

VOID LocalStringToBlock(
    IN CHAR16 *String,
    IN UINTN StringSize,
    OUT UINT8 *Block)
{
    UINT8 LowNibble, HiNibble;
    UINTN i, j;

    j = 0;

    for(i = 0; i < StringSize; i += 2)
    {
        if(i == StringSize - 1) {   //odd number need to fill with zeroes
            HiNibble = 0;
            LowNibble = LocalHexValue((UINT8)String[i]);
        } else {
            HiNibble = LocalHexValue((UINT8)String[i]);
            LowNibble = LocalHexValue((UINT8)String[i + 1]);
        }
        Block[j] = (HiNibble << 4) | LowNibble;
        j++;
    }
}

/**
    Extracts DevicePath from the passed string

    @param ConfigString - Pointer to a string to extract Device Path from
    @param DevicePath - On return will contain DevicePath
    @param DpLength - On return will contain length of the extracted DevicePath

    @retval - Status, based on result

**/

static EFI_STATUS ExtractDevicePath(CHAR16 *ConfigString, UINT8 **DevicePath, UINTN *DpLength)
{
    UINTN Length = 0;
    UINTN i = 5;        //set pointer to skip first four symbols "PATH="

    while(LocalCheckIfHexDigit(ConfigString[i]))      //determine device path length in characters
    {
        Length++;
        i++;
    }

    *DevicePath = AllocatePool(Length/2);
    if (*DevicePath==NULL)
        return EFI_OUT_OF_RESOURCES;

    LocalStringToBlock(&ConfigString[5], Length, *DevicePath);
    *DpLength = Length / 2;

    return EFI_SUCCESS;
}

/**
    Validates DevicePath

    @param DevicePath - Pointer to the DevicePath to Validate
    @param DpLength - Length of DevicePath

    @retval - TRUE if valid device path, FALSE other vice

**/

static BOOLEAN CheckDevicePath(
    IN UINT8 *DevicePath,
    IN UINTN DpLength)
{
    EFI_DEVICE_PATH_PROTOCOL* TempDp;

    TempDp = (EFI_DEVICE_PATH_PROTOCOL*)DevicePath;

    if (DevicePath == NULL)
        return FALSE;

    // Valid device path must have at least one node
    if (DpLength < 4)
        return FALSE;

    // Valid device path protocol cannot be longer than binary data
    if (*(UINT16*)TempDp->Length > DpLength)
        return FALSE;

    // Valide device path types
    if (TempDp->Type != HARDWARE_DEVICE_PATH    &&
        TempDp->Type != ACPI_DEVICE_PATH        &&
        TempDp->Type != MESSAGING_DEVICE_PATH   &&
        TempDp->Type != MEDIA_DEVICE_PATH       &&
        TempDp->Type != BBS_DEVICE_PATH         &&
        TempDp->Type != END_DEVICE_PATH_TYPE)
    {
        return FALSE;
    }

    return TRUE;
}

/**
    Parse string to find Device Path in it and returns Handle associated with it

    @param ConfigString - Pointer to the String to parse
    @param Handle - returned Handle

    @retval - Status, based on result

**/

static EFI_STATUS FindDeviceHandle(
    IN CHAR16 *ConfigString,
    OUT EFI_HANDLE *Handle)
{
    UINTN i = 0;
    UINTN DpLength;
    UINT8 *DevicePath = NULL;
    EFI_DEVICE_PATH_PROTOCOL* TempDp;
    EFI_STATUS Status;

//scan config string to find PATH pattern
    do
    {
        while((ConfigString[i] != L'&') && (ConfigString[i] != 0))
            i++;

        if(ConfigString[i] == 0)
            return EFI_NOT_FOUND;

        i++;            //shift pointer to next symbol
        if(CheckForPattern(&ConfigString[i], &gPathPattern))
            break;
    } while(1);

//if we get here, we have pointer set on PATH pattern
    Status = ExtractDevicePath(&ConfigString[i], &DevicePath, &DpLength);
    if(EFI_ERROR(Status))
        return Status;

    if (CheckDevicePath(DevicePath, DpLength)) {
        TempDp = (EFI_DEVICE_PATH_PROTOCOL*)DevicePath;
        Status = gBS->LocateDevicePath (
                          &gEfiDevicePathProtocolGuid,
                          (EFI_DEVICE_PATH_PROTOCOL**)&TempDp,
                          Handle
                          );
    } else {
        Status = EFI_NOT_FOUND;
    }

    FreePool(DevicePath);

    return Status;
}


/**
  Local implementation of LocalHiiAccessExtractConfig
  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Any and all alternative
  configuration strings shall also be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param Request    A null-terminated Unicode string in
                    <ConfigRequest> format. Note that this
                    includes the routing information as well as
                    the configurable name / value pairs. It is
                    invalid for this string to be in
                    <MultiConfigRequest> format.
                    If a NULL is passed in for the Request field,
                    all of the settings being abstracted by this function
                    will be returned in the Results field.  In addition,
                    if a ConfigHdr is passed in with no request elements,
                    all of the settings being abstracted for that particular
                    ConfigHdr reference will be returned in the Results Field.

  @param Progress   On return, points to a character in the
                    Request string. Points to the string's null
                    terminator if request was successful. Points
                    to the most recent "&" before the first
                    failing name / value pair (or the beginning
                    of the string if the failure is in the first
                    name / value pair) if the request was not
                    successful.

  @param Results    A null-terminated Unicode string in
                    <MultiConfigAltResp> format which has all values
                    filled in for the names in the Request string.
                    String to be allocated by the called function.


  @retval Status, based on result

**/
static EFI_STATUS EFIAPI LocalHiiAccessExtractConfig(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN CONST EFI_STRING Request,
    OUT EFI_STRING *Progress,
    OUT EFI_STRING *Results
)
{
    EFI_STATUS  Status;
    UINTN       Length;
    EFI_GUID    Guid;
    CHAR16      Name[80];
    CHAR16      *Pointer = Request;
    UINT8       *Var = NULL;
    EFI_HII_CONFIG_ROUTING_PROTOCOL *ConfigRouting;
    CHAR16      *ErrorMarker;

    if(Request == NULL) {
        *Progress = NULL;
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateProtocol(&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&ConfigRouting);
    if(EFI_ERROR(Status))
        return Status;

//check syntax for guid
    if(!CheckForPattern(Request, &gGuidPattern)) {
        *Progress = Request;
        return EFI_INVALID_PARAMETER;
    }
//extract guid
    ErrorMarker = Pointer;
    Pointer += 5;       //skip "GUID=" pattern
    Length = 32;        //length of guid in symbols
    LocalStringToBlock(Pointer, Length, (UINT8 *)&Guid);
    Pointer += Length;
    if(*Pointer != L'&') {
        DEBUG ((EFI_D_ERROR, "HII Extract Config: Guid extraction failed\n"));
        *Progress = ErrorMarker;
        return EFI_INVALID_PARAMETER;
    }

//check syntax for name
    if(!CheckForPattern(Pointer + 1, &gNamePattern)) {
        *Progress = Pointer;
        return EFI_INVALID_PARAMETER;
    }
//extract name
    ErrorMarker = Pointer;
    Pointer += 6;       //skip '&NAME=' pattern
    Length = 80;
    LocalStringToChar16(Pointer, &Length, Name);
    Pointer += (Length * 4);
    if(*Pointer != L'&') {
        DEBUG ((EFI_D_ERROR, "HII Extract Config: Name extraction failed\n"));
        *Progress = ErrorMarker;
        return EFI_INVALID_PARAMETER;
    }

//get variable by name and guid from nvram

    Length = 0;
    Status = LocalGetEfiVar(Name, &Guid, NULL, &Length, (VOID**)&Var);
    if(EFI_ERROR(Status)) {
        *Progress = Request;
        return Status;
    }

//find start of OFFSET/WIDTH/VALUE block
    Pointer++;      //skip '&'
    while((*Pointer != L'&') && (*Pointer != 0))    //skip PATH
            Pointer++;

    if(*Pointer == 0) {                     //OFFSET/WIDTH blocks are missed
        UINTN TempSize;
        CHAR16 *TempString;
        CHAR16 *TempString2;

        TempSize = (StrLen(L"&OFFSET=0&WIDTH=") + sizeof(Length) + 1) * sizeof(CHAR16);
        TempString = AllocateZeroPool(TempSize);
        if(TempString == NULL)
            return EFI_OUT_OF_RESOURCES;

        UnicodeSPrint(TempString, TempSize, L"&OFFSET=0&WIDTH=%x", Length);

        TempSize += StrSize(Request);
        TempString2 = AllocateZeroPool(TempSize);
        if(TempString2 == NULL)
            return EFI_OUT_OF_RESOURCES;

        StrCat(TempString2, Request);
        StrCat(TempString2, TempString);

        Status = ConfigRouting->BlockToConfig(ConfigRouting, TempString2, Var, Length, Results, Progress);
        *Progress = Request + StrLen(Request); //set Progress to end of ConfigHdr
        FreePool(TempString);
        FreePool(TempString2);
    } else {                                        //OFFSET/WIDTH block found, let's check if it's correct
        if(!CheckForPattern(Pointer + 1, &gOffsetPattern)) {
            *Progress = Pointer;
            return EFI_INVALID_PARAMETER;
        }
        Status = ConfigRouting->BlockToConfig(ConfigRouting, Request, Var, Length, Results, Progress);
    }

    FreePool(Var);

    return Status;
}

/**
    Local implementation of HiiAccessRouteConfig

    Convert the Configuration string into a appropriate information and then store the data
    contained in it to the correct medium

    @param This Pointer to the EFI_HII_CONFIG_ACCESS_PROTOCOL
    @param Configuration Pointer to the Null terminated string that represents the configuration request
    @param Progress A pointer to the progress of parsing the Configuration request

    @retval Status, based on result.
 */

static EFI_STATUS EFIAPI LocalHiiAccessRouteConfig(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN CONST EFI_STRING Configuration,
    OUT EFI_STRING *Progress
)
{
    EFI_STATUS  Status;
    UINTN       Length;
    UINTN       VarSize;
    EFI_GUID    Guid;
    CHAR16      Name[80];
    CHAR16      *Pointer = Configuration;
    UINT8       *Var = NULL;
    EFI_HII_CONFIG_ROUTING_PROTOCOL *ConfigRouting;
    EFI_HANDLE  Handle;
    UINT32      Attributes;

    if(Configuration == NULL)
    {
        *Progress = NULL;
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateProtocol(&gEfiHiiConfigRoutingProtocolGuid, NULL, (VOID**)&ConfigRouting);
    if(EFI_ERROR(Status))
        return Status;

//check syntax for guid
    if(!CheckForPattern(Configuration, &gGuidPattern))
    {
        *Progress = Configuration;
        return EFI_INVALID_PARAMETER;
    }

//check proper device path
    Status = FindDeviceHandle(Configuration, &Handle);
    if(EFI_ERROR(Status))
        return EFI_NOT_FOUND;

//extract guid
    Pointer += 5;       //skip "GUID=" pattern
    Length = 32;        //length of guid in bytes
    LocalStringToBlock(Pointer, Length, (UINT8 *)&Guid);
    Pointer += Length;
    if(*Pointer != L'&')
    {
        DEBUG ((EFI_D_ERROR, "HII Route Config: Guid extraction failed\n"));
        *Progress = Pointer;
        return EFI_INVALID_PARAMETER;
    }

//check syntax for name
    if(!CheckForPattern(Pointer + 1, &gNamePattern))
    {
        *Progress = Pointer;
        return EFI_INVALID_PARAMETER;
    }
//extract name
    Pointer += 6;       //skip '&NAME=' pattern
    Length = 80;
    LocalStringToChar16(Pointer, &Length, Name);
    Pointer += (Length * 4);
    if(*Pointer != L'&')
    {
        DEBUG ((EFI_D_ERROR, "HII Route Config: Name extraction failed\n"));
        *Progress = Pointer;
        return EFI_INVALID_PARAMETER;
    }

//get variable by name and guid from nvram

    Length = 0;
    Status = LocalGetEfiVar(Name, &Guid, &Attributes, &Length, (VOID**)&Var);
    if(EFI_ERROR(Status))   //requested variable not found
    {
        if(Var != NULL)
            FreePool(Var);

        Var = NULL;
        Length = 0;
#if (defined(RT_ACCESS_SUPPORT_IN_HPKTOOL) && (RT_ACCESS_SUPPORT_IN_HPKTOOL == 1))
        {
            Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
        }
#else
        {
            Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS;
        }
#endif
    }
    VarSize = Length;

    Status = ConfigRouting->ConfigToBlock(ConfigRouting, Configuration, Var, &Length, Progress);
    while(Status == EFI_DEVICE_ERROR || (Status == EFI_INVALID_PARAMETER && Var == NULL)) //block not large enough
    {
        if(Var != NULL)
            FreePool(Var);

        Var = AllocatePool(Length);
        if(Var==NULL)
            return EFI_OUT_OF_RESOURCES;

        VarSize = Length;
        Status = ConfigRouting->ConfigToBlock(ConfigRouting, Configuration, Var, &Length, Progress);
    }

    if(EFI_ERROR(Status))
        return Status;

    gRT->SetVariable(Name, &Guid, Attributes, VarSize, Var);

    FreePool(Var);

    return EFI_SUCCESS;
}

/**
    Dummy local implementation of LocalHiiAccessFormCallback

    @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
    @param  Action                 Specifies the type of action taken by the browser.
    @param  QuestionId             A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
    @param  Type                   The type of value for the question.
    @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
    @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.

**/

static EFI_STATUS EFIAPI LocalHiiAccessFormCallback(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN EFI_BROWSER_ACTION Action,
    IN EFI_QUESTION_ID QuestionId,
    IN UINT8 Type,
    IN EFI_IFR_TYPE_VALUE *Value,
    OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest
)
{
    return EFI_UNSUPPORTED;
}

/**
    Installs HiiConfigAccessProtocol for callback to happened.

    @param Image - Loaded image protocol
    @param FormSet - Form Set
    @param HiiConfigAccess - Hii Config Access protocol
    @param CallbackHandle - Callback handle

    @retval - Status, based on result

**/

EFI_STATUS SetHiiCallback(
    EFI_LOADED_IMAGE_PROTOCOL *Image,
    EFI_IFR_FORM_SET *FormSet,
    EFI_HII_CONFIG_ACCESS_PROTOCOL *HiiConfigAccess,
    EFI_HANDLE *CallbackHandle
)
{

    EFI_STATUS Status;
    EFI_HANDLE Handle=NULL;
    EFI_DEVICE_PATH_PROTOCOL *pPath, *pPath2;
    EFI_HII_CONFIG_ACCESS_PROTOCOL *LocalHiiConfigAccess = HiiConfigAccess;

    static EFI_HII_CONFIG_ACCESS_PROTOCOL DefaultConfigAccess = {
        LocalHiiAccessExtractConfig, LocalHiiAccessRouteConfig, LocalHiiAccessFormCallback
    };

    static VENDOR_DEVICE_PATH FormSetDevicePathNode = {
        {
            MEDIA_DEVICE_PATH, MEDIA_VENDOR_DP,
            {sizeof(VENDOR_DEVICE_PATH), 0}
        },
        {0}
    };

    if (HiiConfigAccess == NULL)
        LocalHiiConfigAccess = &DefaultConfigAccess;
    else
    {
        if (LocalHiiConfigAccess->ExtractConfig==NULL)
            LocalHiiConfigAccess->ExtractConfig = LocalHiiAccessExtractConfig;
        if (LocalHiiConfigAccess->RouteConfig==NULL)
            LocalHiiConfigAccess->RouteConfig = LocalHiiAccessRouteConfig;
        if (LocalHiiConfigAccess->Callback==NULL)
            LocalHiiConfigAccess->Callback = LocalHiiAccessFormCallback;
    }

    Status=gBS->HandleProtocol(Image->DeviceHandle, &gEfiDevicePathProtocolGuid, (VOID**)&pPath);
    if (EFI_ERROR(Status)) pPath=NULL;
    pPath = AppendDevicePathNode(pPath, Image->FilePath);
    FormSetDevicePathNode.Guid = FormSet->Guid;
    pPath2 = AppendDevicePathNode(pPath, &FormSetDevicePathNode.Header);
    FreePool(pPath);
    Status = gBS->InstallMultipleProtocolInterfaces(
                &Handle,
                &gEfiDevicePathProtocolGuid, pPath2,
                &gEfiHiiConfigAccessProtocolGuid, LocalHiiConfigAccess,
                NULL
            );

    if (EFI_ERROR(Status)) return Status;

    if (CallbackHandle) *CallbackHandle=Handle;

    return EFI_SUCCESS;
}



/**
    Loads HII packages associated with
    the specified image and publishes them to the HII database

    @param HiiConfigAccess - Pointer to HII Config Access Protocol
    @param InitFunction - initialization function to be launched once resources are published.
    @param HiiHandle - Pointer to Handle to return


    @retval - Status, based on result

**/

EFI_STATUS AmiPublishHiiResources(
        IN EFI_HII_CONFIG_ACCESS_PROTOCOL *HiiConfigAccess OPTIONAL,
        IN AMI_INIT_HII_PACK InitFunction OPTIONAL,
        OUT EFI_HII_HANDLE *HiiHandle
)
{
    EFI_STATUS                      Status;
    EFI_HII_PACKAGE_LIST_HEADER     *PackageList;
    EFI_HII_DATABASE_PROTOCOL       *HiiDatabase;
    EFI_LOADED_IMAGE_PROTOCOL       *Image;
    EFI_HII_HANDLE                  LocalHiiHandle;
    EFI_HANDLE                      CallbackHandle = NULL;
    EFI_HII_PACKAGE_HEADER          *IfrPackagePtr;



    if (EFI_ERROR(Status=gBS->HandleProtocol(gImageHandle, &gEfiLoadedImageProtocolGuid, (VOID**)&Image))) return Status;

    //
    // Retrieve HII package list from ImageHandle
    //
    Status = gBS->OpenProtocol (
        gImageHandle,
        &gEfiHiiPackageListProtocolGuid,
        (VOID **) &PackageList,
        gImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    Status = gBS->LocateProtocol (
        &gEfiHiiDatabaseProtocolGuid,
        NULL,
        (VOID**)&HiiDatabase
    );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    for (IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)(PackageList+1);
         IfrPackagePtr < (EFI_HII_PACKAGE_HEADER*)((UINT8*)PackageList+PackageList->PackageLength);
         IfrPackagePtr = (EFI_HII_PACKAGE_HEADER*)((UINT8*)IfrPackagePtr+IfrPackagePtr->Length))
    {
        if (IfrPackagePtr->Type == EFI_HII_PACKAGE_FORMS) {
            Status = SetHiiCallback(
                    Image, (EFI_IFR_FORM_SET*)(IfrPackagePtr+1),
                    HiiConfigAccess,&CallbackHandle
            );
            DEBUG ((EFI_D_INFO, "SetHiiCallback: %r\n", Status));
            break;
        }
    }

    // Publish HII package list to HII Database.
    Status = HiiDatabase->NewPackageList (
        HiiDatabase,
        PackageList,
        CallbackHandle,
        &LocalHiiHandle
    );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    if (InitFunction) InitFunction(LocalHiiHandle);

    if (HiiHandle) *HiiHandle = LocalHiiHandle;


    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
