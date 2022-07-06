//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file ServerMgmtSetup.c
    ServerMgmtSetup driver to initialize server management
    setup screen

**/

//----------------------------------------------------------------------
#include "ServerMgmtSetup.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

//
// Macro Definition
//
#define NUMBER_OF_FORMSETS    (sizeof(gSetupCallBack)/sizeof(CALLBACK_INFO))
#define STRING_BUFFER_LENGTH  20
//
// Global variables
//
EFI_HANDLE                          gThisImageHandle = NULL;
SERVER_MGMT_CONFIGURATION_DATA      gServerMgmtConfiguration;
static EFI_HII_STRING_PROTOCOL      *gHiiString = NULL;
static EFI_HII_DATABASE_PROTOCOL    *gHiiDatabase = NULL;
//
// Declare list of string initialization functions
//
typedef VOID (STRING_INIT_FUNC)(
    EFI_HII_HANDLE HiiHandle, UINT16 Class
);

extern STRING_INIT_FUNC SERSVER_MGMT_STRING_INIT_LIST EndOfFunctionList;
STRING_INIT_FUNC *gStringInitFunc[] = { SERSVER_MGMT_STRING_INIT_LIST NULL };

/**

    These Data Structure define a structure used to match a specific
    Callback Protocol to an HII Form through the use of Class and SubClass
    values

**/
typedef struct {
    EFI_HII_CONFIG_ACCESS_PROTOCOL Callback;
    UINT16 Class, SubClass;
} SETUP_CALLBACK;

EFI_STATUS Callback(
    IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
    IN EFI_BROWSER_ACTION Action,
    IN EFI_QUESTION_ID KeyValue,
    IN UINT8 Type,
    IN EFI_IFR_TYPE_VALUE *Value,
    OUT EFI_BROWSER_ACTION_REQUEST *ActionRequest
);

/**
    These Variable definitions define the different formsets and what Callback
    protocol should be used for each one

**/
SETUP_CALLBACK gServerMgmtCallbackProtocol = {{NULL, NULL, Callback}, SERVER_MGMT_CLASS_ID, 0};

/**

    This array contains the different HII packages that are used in the system

  @note
  The HiiHandle is updated in the LoadResources function when the HII Packages
  are loaded
**/
CALLBACK_INFO gSetupCallBack[] = {
    // Last field in every structure will be filled by the Setup
    { &gEfiServerMgmtSetupVariableGuid, &gServerMgmtCallbackProtocol.Callback, SERVER_MGMT_CLASS_ID, 0, 0}
};
#if    CORE_COMBINED_VERSION < 0x50007
/**

    This Data Structure is used by the setup infrastructure to define 
    callback functions that should be used for interacting with setup forms
    or individual questions.

**/
typedef struct{
    UINT16 Class, SubClass, Key;
    SETUP_ITEM_CALLBACK_HANDLER *UpdateItem;
} SETUP_ITEM_CALLBACK;
#endif

//
// Brings the definitions of the SDL token defined list of callbacks into this
//  file as a list of functions that can be called
#define ITEM_CALLBACK(Class,Subclass,Key,Callback) Callback
extern SETUP_ITEM_CALLBACK_HANDLER SERVER_MGMT_SETUP_ITEM_CALLBACK_LIST EndOfList;
#undef ITEM_CALLBACK

//
// This creates an array of callbacks to be used
//
#define ITEM_CALLBACK(Class,Subclass,Key,Callback) {Class,Subclass,Key,&Callback}
SETUP_ITEM_CALLBACK ServerMgmtSetupItemCallback[] = { SERVER_MGMT_SETUP_ITEM_CALLBACK_LIST {0, 0, 0, NULL} };

CALLBACK_PARAMETERS *CallbackParametersPtr = NULL;
CALLBACK_PARAMETERS* GetCallbackParameters(){
    return CallbackParametersPtr;
}

/**
    This function is used to identify the function to call when an interactive 
    item has been triggered in the setup browser based on the information in
    the Callback protocol and the SetupCallBack Array

    @param This Pointer to the instance of the callback
                protocol
    @param KeyValue Unique value that defines the type of data to expect
                    in the Data parameter
    @param Data Data defined by KeyValue Parameter
    @param Packet Data passed from the Callback back to
               the setup Browser

    @retval EFI_SUCCESS

**/

EFI_STATUS Callback(
    IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN        EFI_BROWSER_ACTION               Action,
    IN        EFI_QUESTION_ID                  KeyValue,
    IN        UINT8                            Type,
    IN        EFI_IFR_TYPE_VALUE               *Value,
    OUT       EFI_BROWSER_ACTION_REQUEST       *ActionRequest )
{
    SETUP_CALLBACK *pCallback = (SETUP_CALLBACK*)This;
    CALLBACK_PARAMETERS CallbackParameters;
    EFI_STATUS Status;
    SETUP_ITEM_CALLBACK *pItemCallback = ServerMgmtSetupItemCallback;

    CallbackParametersPtr = &CallbackParameters;

    CallbackParameters.This = (VOID*)This;
    CallbackParameters.Action = Action;
    CallbackParameters.KeyValue = KeyValue;
    CallbackParameters.Type = Type;
    CallbackParameters.Value = Value;
    CallbackParameters.ActionRequest = ActionRequest;
    if (ActionRequest) *ActionRequest=EFI_BROWSER_ACTION_REQUEST_NONE;
    Status = EFI_UNSUPPORTED;

    while(pItemCallback->UpdateItem) {

        if ( pItemCallback->Class == pCallback->Class
            &&  pItemCallback->SubClass == pCallback->SubClass
            &&  pItemCallback->Key == KeyValue ) {

            Status = pItemCallback->UpdateItem(
                        gSetupCallBack[0].HiiHandle,
                        pItemCallback->Class, pItemCallback->SubClass,
                        KeyValue );
            if (Status != EFI_UNSUPPORTED) break;
        }
        pItemCallback++;
    }
    CallbackParametersPtr = NULL;
    return Status;
}
/**
    This function is called for each Formset and initializes strings based on
    the porting provided and then updates the HII database

    @param HiiHandle - handle that indicates which HII Package
                       that is being used
    @param pCallBackFound - pointer to an instance of CALLBACK_INFO
                       that works with HiiHandle

    @retval VOID

**/

VOID
InitFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN CALLBACK_INFO      *pCallBackFound )
{
    UINT16  i;

    if (!pCallBackFound || !pCallBackFound->HiiHandle) {
        return;
    }

    for (i = 0; gStringInitFunc[i] != NULL; i++) {
        gStringInitFunc[i] (HiiHandle, pCallBackFound->Class);
    }

    return;
}
/**
    This function publishes all HII resources and initializes the HII databases
    There is a token ALWAYS_PUBLISH_HII_RESOURCES that would call this function
    on every boot not just when the user tries to enter Setup

    @param Event Event that was triggered
    @param Context data pointer to information that is defined when the 
        event is registered

    @retval VOID

**/

VOID
EFIAPI
SetupCallback (
  IN EFI_EVENT      Event,
  IN VOID           *Context )
{
    static BOOLEAN ResourcesLoaded = FALSE;

    //
    // Locate the HII based protocols
    //
    if ( !gHiiString
        && EFI_ERROR(gBS->LocateProtocol(
                            &gEfiHiiStringProtocolGuid,
                            NULL,
                           (VOID **)&gHiiString)) ) {
        return ;
    }
    if ( !gHiiDatabase
        && EFI_ERROR(gBS->LocateProtocol(
                            &gEfiHiiDatabaseProtocolGuid,
                            NULL,
                            (VOID **)&gHiiDatabase)) ) {
        return ;
    }
    if (Event) {
        gBS->CloseEvent(Event);
    }
    if (ResourcesLoaded) {
        return;
    }
    ResourcesLoaded = TRUE;
    LoadResources(
        gThisImageHandle,
        NUMBER_OF_FORMSETS,
        gSetupCallBack,
        InitFunction);
    return;
}

/**
    Entry point of the Server Management screen setup driver.

    
    @param  ImageHandle A handle for the image that is
                  initializing this driver
    @param SystemTable A pointer to the EFI system table
            event is registered

    @retval EFI_STATUS Driver initialized successfully

**/

EFI_STATUS
InstallServerMgmtSetupScreen (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    UINTN       Size;
    EFI_STATUS  Status;
    UINT32      Attributes;

    InitAmiLib(ImageHandle,SystemTable);

    gThisImageHandle = ImageHandle;
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &gServerMgmtConfiguration );
    if (Status == EFI_NOT_FOUND) {
        SetupCallback(NULL,NULL);
        gRT->SetVariable(
               L"ServerSetup",
               &gEfiServerMgmtSetupVariableGuid,
               Attributes,
               sizeof(SERVER_MGMT_CONFIGURATION_DATA),
               &gServerMgmtConfiguration );
    } else {
#if ALWAYS_PUBLISH_HII_RESOURCES
    {
        VOID            *BdsEventBeforeSetupRegistration;
        EFI_EVENT       BdsEventBeforeSetup;
        EFI_GUID        guidBdsBeforeSetup = EFI_BDS_EVENT_BEFORE_SETUP_GUID;
        //
        // Register call back notification on "BDS event before setup GUID".
        //
        RegisterProtocolCallback(
            &guidBdsBeforeSetup,
            SetupCallback,
            NULL,
            &BdsEventBeforeSetup,
            &BdsEventBeforeSetupRegistration );

    }
#else 
    {
        VOID            *pSetupRegistration;
        EFI_EVENT       SetupEnterEvent;
        EFI_GUID        guidSetupEnter = AMITSE_SETUP_ENTER_GUID;
        //
        // Register callback notification on "Setup Enter GUID".
        //
        RegisterProtocolCallback(
            &guidSetupEnter,
            SetupCallback,
            NULL,
            &SetupEnterEvent,
            &pSetupRegistration );
    }
#endif
    }
    return EFI_SUCCESS;
}

/**
    This function Locate DxeIpmi and IpmiUsb Protocol and update the BMC
    Interfaces Exists list in setup.

    @param  HiiHandle - Handle that indicates which HII Package
                        that is being used
    @param  Class     - Indicates the setup class

    @retval VOID

**/
VOID
UpdateIpmiInterfacesList (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class )
{
    EFI_STATUS          Status;
    CHAR16              UpdateStr[STRING_BUFFER_LENGTH];
    VOID                *Registration;
    IPMI_USB_TRANSPORT  *IpmiUsbTransport = NULL;
    EFI_IPMI_TRANSPORT  *IpmiTransport = NULL;
    ZeroMem (UpdateStr, sizeof (UpdateStr));
    //
    // Locate IpmiTransport protocol
    //
    Status = gBS->LocateProtocol (
                     &gEfiDxeIpmiTransportProtocolGuid,
                     NULL,
                     (VOID**)&IpmiTransport );

    if (!EFI_ERROR (Status)) {
        if (IPMI_SYSTEM_INTERFACE == KCS_INTERFACE) {        // KCS 
            UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"KCS");
        } else if (IPMI_SYSTEM_INTERFACE == BT_INTERFACE){   // BT
            UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"BT");
        } else if (IPMI_SYSTEM_INTERFACE == SSIF_INTERFACE){ // SSIF 
            UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"SSIF");
        } else {                                             // IPMB
            UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"IPMB");
        }
        HiiSetString (
              HiiHandle,
              STRING_TOKEN(STR_BMC_INTERFACES_LIST),
              UpdateStr,
              NULL );
    }
    //
    // Locate IpmiUsbTransport protocol and check for USB interface.
    //
    Status = gBS->LocateProtocol (
                     &gEfiDxeIpmiUsbTransportProtocolGuid,
                     NULL,
                     (VOID **)&IpmiUsbTransport );
    if (!EFI_ERROR (Status)) {
        if (StrCmp (UpdateStr, L"") == 0x00){
            UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"USB");
        } else {
            UnicodeSPrint(
               UpdateStr + StrLen(UpdateStr),
               sizeof (UpdateStr),
               L", USB" );
        }
        HiiSetString (
              HiiHandle,
              STRING_TOKEN(STR_BMC_INTERFACES_LIST),
              UpdateStr,
              NULL );
     } else {
        EfiCreateProtocolNotifyEvent (
               &gEfiDxeIpmiUsbTransportProtocolGuid,
               TPL_CALLBACK,
               UpdateUsbInIpmiInterfacesListCallback,
               NULL,
               &Registration );
    }
}

/**
  This is callback function,
  If IpmiUsb Protocol is installed update a string in Setup.

  @param  Event   - The Event that is being processed, not used.
  @param  Context - Event Context, not used.
  @retval VOID

**/
VOID
EFIAPI
UpdateUsbInIpmiInterfacesListCallback(
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS          Status;
    EFI_STRING          String=NULL;
    CHAR16              UpdateStr[STRING_BUFFER_LENGTH];
    EFI_STRING          CheckStr = L"Unknown";
    IPMI_USB_TRANSPORT  *IpmiUsbTransport = NULL;
    ZeroMem (UpdateStr, sizeof (UpdateStr));
    //
    // Locate IpmiUsbTransport protocol and Check for USB interface.
    //
    Status = gBS->LocateProtocol (
                     &gEfiDxeIpmiUsbTransportProtocolGuid,
                     NULL,
                     (VOID **)&IpmiUsbTransport );

    if (!EFI_ERROR (Status)) {
         String = HiiGetString (
                        gSetupCallBack[0].HiiHandle,
                        STRING_TOKEN(STR_BMC_INTERFACES_LIST),
                        NULL );
         if (StrCmp(String , CheckStr) == 0) {
             UnicodeSPrint (UpdateStr, sizeof (UpdateStr), L"USB");
         } else {
             gBS->CopyMem(UpdateStr,String,sizeof(String));
             UnicodeSPrint(
                UpdateStr + StrLen(String),
                STRING_BUFFER_LENGTH,
                L", USB" );
         }
         HiiSetString (
               gSetupCallBack[0].HiiHandle,
               STRING_TOKEN(STR_BMC_INTERFACES_LIST),
               UpdateStr,
               NULL );

         if(String != NULL) {
             gBS->FreePool(String);
         }
     }
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
