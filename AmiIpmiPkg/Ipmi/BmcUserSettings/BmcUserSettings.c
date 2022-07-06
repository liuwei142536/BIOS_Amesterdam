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

/** @file BmcUserSettings.c
    Provides functionality to Add/Delete/Change BMC user
    settings.

**/

//----------------------------------------------------------------------

#include "BmcUserSettings.h"

//----------------------------------------------------------------------

//----------------------------------------------------------------------

//
// Global variables
//
EFI_IPMI_TRANSPORT          *gIpmiTransport;
BMC_USER_DETAILS_LIST       gBmcUserDetailsList[MAX_BMC_USER_COUNT];
AMI_POST_MANAGER_PROTOCOL   *gAmiPostMgr = NULL;
UINT8                       gAllowableUser = 0;
UINT8                       *gPcdLanChannelBufferPtr;
UINT8                       gPcdSize;


BMC_USER_DETAILS            gBmcAddUserDetails;
BMC_USER_DETAILS            gBmcDeleteUserDetails;
BMC_USER_DETAILS            gBmcChangeUserDetails;

//----------------------------------------------------------------------

/**
    Converts Unicode to ASCII Characters.

    @param UnicodeStr The Unicode string to be written to. The buffer must
                      be large enough.
    @param AsciiStr The ASCII string to be converted.

    @retval VOID
 
**/

VOID
IpmiUnicode2Ascii (
  IN      CHAR16         *UnicodeStr,
  IN OUT  CHAR8          *AsciiStr
   )
{

    while (*UnicodeStr != '\0') {
      *(AsciiStr++) = (CHAR8) *(UnicodeStr++);
    }

    *AsciiStr = '\0';

    return;
}

/**
    This function collects number of users allowed BMC.

    @param VOID

    @retval VOID
**/

VOID
GetMaxUserAllowable(
 VOID )
{

    EFI_STATUS                        Status;
    EFI_GET_USER_ACCESS_COMMAND       GetUserAccessCommand = {0};
    EFI_GET_USER_ACCESS_RESPONSE      GetUserAccessResponse = {0};
    UINT8                             GetUserAccessResponseSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    if (gPcdLanChannelBufferPtr[0] != 0) {
        GetUserAccessCommand.ChannelNo = gPcdLanChannelBufferPtr[0];
        GetUserAccessCommand.UserId = 1;
        GetUserAccessResponseSize = sizeof (GetUserAccessResponse);
        Status = gIpmiTransport->SendIpmiCommand(
                                    gIpmiTransport,
                                    IPMI_NETFN_APP,
                                    BMC_LUN,
                                    IPMI_APP_GET_USER_ACCESS,
                                    (UINT8 *)&GetUserAccessCommand,
                                    sizeof (GetUserAccessCommand),
                                    (UINT8*)&GetUserAccessResponse,
                                    &GetUserAccessResponseSize );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of Get User Access Command: %r \n", Status));
        if (!EFI_ERROR(Status)) {
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Maximum User Id allowable: %04x \n", GetUserAccessResponse.MaxUserId));
            gAllowableUser = GetUserAccessResponse.MaxUserId;
            return;
        }
    }
}
/**
    This function collects all user names from BMC.

    @param VOID

    @retval VOID
**/

VOID
CollectAllBmcUserDetails (
  VOID )
{
    UINT8                       i;
    EFI_STATUS                  Status;
    EFI_GET_USER_NAME_COMMAND   GetUserNameCommandData = {0};
    EFI_GET_USER_NAME_RESPONSE  GetUserNameResponseData = {0};
    UINT8                       GetUserNameResponseDataSize;
    CHAR8                       *InvalidUserName = "Invalid";
    CHAR8                       *Reserved = "Reserved";

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    //
    // Send Get User Name Command to BMC for all possible User IDs
    //
    GetUserNameResponseDataSize = sizeof (GetUserNameResponseData);

    for(i = 0; i <= gAllowableUser; i++) {
        gBmcUserDetailsList[i].UserId = i;
        if ((i == 0) || (i == 1)) {// User Id 0 & 1 are reserved as per IPMI specification
            CopyMem (gBmcUserDetailsList[i].UserName, Reserved, AsciiStrLen (Reserved));
            continue;
        }

        CopyMem ( gBmcUserDetailsList[i].UserName, InvalidUserName, AsciiStrLen (InvalidUserName));

        GetUserNameCommandData.UserId = i;
        GetUserNameResponseDataSize = sizeof (GetUserNameResponseData);
        ZeroMem (GetUserNameResponseData.UserName, MAX_BMC_USER_NAME_LENGTH );

        Status = gIpmiTransport->SendIpmiCommand(
                                       gIpmiTransport,
                                       IPMI_NETFN_APP,
                                       BMC_LUN,
                                       IPMI_APP_GET_USER_NAME,
                                       (UINT8 *)&GetUserNameCommandData,
                                       sizeof (GetUserNameCommandData),
                                       GetUserNameResponseData.UserName,
                                       &GetUserNameResponseDataSize );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of Get User Name Command: %r \n", Status));
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));

        if (!EFI_ERROR (Status)) {
            //
            // Copy the user name in User Details Global List
            //
            if (GetUserNameResponseData.UserName[0] != 0x00) {
                CopyMem ( gBmcUserDetailsList[i].UserName, GetUserNameResponseData.UserName, sizeof(GetUserNameResponseData));
            } 
        }
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Id: %x \n", gBmcUserDetailsList[i].UserId));
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Name: %s \n", gBmcUserDetailsList[i].UserName));
    }

    return;
}

/**
    Checks the given user name already available in global user list

    @param UserDetails Contains user name

    @retval TRUE User already available.
    @retval FALSE User not available.
**/

BOOLEAN
CheckUserNameAvailability (
  IN OUT BMC_USER_DETAILS   *UserDetails )
{
    UINT8       i;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: UserName: %s", UserDetails->UserName));

    for (i = 0; i <= gAllowableUser; i++) {

        if (AsciiStrCmp (gBmcUserDetailsList[i].UserName, UserDetails->UserName) == 0x00) {
            UserDetails->UserId = gBmcUserDetailsList[i].UserId;
            DEBUG ((DEBUG_INFO, "IPMI: Available UserId: %x", UserDetails->UserId));
            DEBUG ((DEBUG_INFO, "IPMI: Returning TRUE from %a\n", __FUNCTION__));
            return TRUE;
        }
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning FALSE from %a\n", __FUNCTION__));
    return FALSE;
}

/**
    Checks add new user is possible or not.

    @param VOID

    @retval TRUE Add new user is possible.
    @retval FALSE Add new user is not possible.
**/

BOOLEAN
CheckAddNewUserPossibility (
  VOID )
{
    UINT8           i;
    CHAR8           *InvalidUserName = "Invalid";

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    for (i = 0; i <= gAllowableUser; i++){
        if (AsciiStrCmp (gBmcUserDetailsList[i].UserName, InvalidUserName) == 0x00) {
            DEBUG ((DEBUG_INFO, "IPMI: Returning TRUE from %a\n", __FUNCTION__));
            return TRUE;
        }
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning FALSE from %a\n", __FUNCTION__));
    return FALSE;
}

/**
    Send Set User Name command to BMC.

    @param UserId User Id.
    @param UserName User Name.

    @return EFI_STATUS
**/

EFI_STATUS
SendSetUserNameCommandToBmc(
  IN UINT8      UserId,
  IN CHAR8      *UserName)
{
    EFI_STATUS                      Status;
    EFI_SET_USER_NAME_COMMAND       SetUserName = {0};
    UINT8                           ResponseDataSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Id: %x\n", UserId));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Name: %s\n", UserName));

    SetUserName.UserId = UserId;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: AsciiStrLen(UserName): %x\n", AsciiStrLen(UserName)));
    CopyMem (&SetUserName.UserName, UserName, AsciiStrLen (UserName));
    ResponseDataSize = 0;

    Status = gIpmiTransport->SendIpmiCommand(
                gIpmiTransport,
                IPMI_NETFN_APP,
                BMC_LUN,
                IPMI_APP_SET_USER_NAME,
                (UINT8 *)&SetUserName,
                sizeof (SetUserName),
                NULL,
                &ResponseDataSize );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Status of Set User Name Command: %r \n", Status));

    return Status; 
}

/**
    Delete user details from global list.

    @param UserName User Name.

    @retval EFI_SUCCESS User details successfully deleted from list.
    @retval EFI_NOT_FOUND User is not available in list.
**/

EFI_STATUS
DeleteUserEntryFromList(
  IN CHAR8      *UserName )
{
    UINT8           i;
    CHAR8           *InvalidUserName = "Invalid";

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    for (i = 0; i <= gAllowableUser; i++) {

        if (AsciiStrCmp (gBmcUserDetailsList[i].UserName, UserName) == 0x00) {
            AsciiStrCpy (gBmcUserDetailsList[i].UserName, InvalidUserName);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[%x].UserName: %s\n", i, gBmcUserDetailsList[i].UserName));
            return EFI_SUCCESS;
        }

    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_NOT_FOUND;
}

/**
    Add user details to global list.

    @param UserDetails User details.

    @retval VOID
**/

VOID
AddUserEntryToList(
  IN BMC_USER_DETAILS      *UserDetails )
{
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    gBmcUserDetailsList[UserDetails->UserId].UserId = UserDetails->UserId;
    CopyMem ( gBmcUserDetailsList[UserDetails->UserId].UserName, UserDetails->UserName, sizeof(UserDetails->UserName));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[UserDetails->UserId].UserId:%x\n", gBmcUserDetailsList[UserDetails->UserId].UserId));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gBmcUserDetailsList[UserDetails->UserId].UserName:%s\n", gBmcUserDetailsList[UserDetails->UserId].UserName));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));

}

/**
    Parse the global list and get the available used id for new user.

    @param UserId User Id.

    @retval EFI_SUCCESS ID is available.
    @retval EFI_NOT_FOUND No more id is available.
**/

EFI_STATUS
GetAvailableUserId(
  IN OUT UINT8      *UserId)
{
    UINT8       i;
    CHAR8       *InvalidUserName = "Invalid";

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    for (i = 0; i <= gAllowableUser; i++){
        if (AsciiStrCmp (gBmcUserDetailsList[i].UserName, InvalidUserName) == 0x00) {
            *UserId = gBmcUserDetailsList[i].UserId;
            DEBUG ((DEBUG_INFO,"IPMI: User Id: %x", *UserId));
            DEBUG ((DEBUG_INFO,"IPMI: Returning EFI_SUCCESS from %a\n", __FUNCTION__));
            return EFI_SUCCESS;
        }
    }
    SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Returning EFI_NOT_FOUND from %a\n", __FUNCTION__));
    return  EFI_NOT_FOUND;
}

/**
    Displays error message pop up in setup.

    @param HiiHandle - Handle to HII database.
    @param Message - Error message token value.

    @retval VOID

**/
VOID
DisplayErrorMessagePopUp (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Message )
{
    UINT8       MsgBoxSel;
    EFI_STRING  CharPtrString;
    EFI_STRING  MessagePtrString;

    CharPtrString = HiiGetString ( HiiHandle, STRING_TOKEN(STR_BMC_USER_SETTINGS_INFO), NULL);
    MessagePtrString = HiiGetString ( HiiHandle, Message, NULL);
    if ((CharPtrString != NULL) && (MessagePtrString != NULL)) {
        gAmiPostMgr->DisplayMsgBox (
                         CharPtrString,
                         MessagePtrString,
                         MSGBOX_TYPE_OK,
                         &MsgBoxSel );
        gBS->FreePool(MessagePtrString);
        gBS->FreePool(CharPtrString);
    }
}

/**
    Send Set user password command as per input data.

    @param UserId User Id.
    @param PasswordSize Password size(16 or 20 byte).
    @param Operation Tells what operation to perform.
    @param UserPassword Password.

    @return EFI_STATUS
**/

EFI_STATUS
PerformGivenUserPasswordAction(
  IN UINT8      UserId,
  IN UINT8      PasswordSize,
  IN UINT8      Operation,
  IN CHAR8      *UserPassword )
{
    EFI_SET_USER_PASSWORD_COMMAND_OF_SIZE_TWENTY        SetUserPassword = {0};
    EFI_STATUS                                          Status;
    UINT8                                               DataSize;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    ZeroMem (SetUserPassword.PasswordData, sizeof (SetUserPassword.PasswordData));
    CopyMem (SetUserPassword.PasswordData, UserPassword, AsciiStrLen(UserPassword));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: UserId: %x\n", UserId));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: IsPasswordSize20: %x\n", PasswordSize));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Operation: %x\n", Operation));

    SetUserPassword.UserId = UserId;
    SetUserPassword.PasswordSize = PasswordSize;
    SetUserPassword.Operation = Operation;
    DataSize = 0;

    Status = gIpmiTransport->SendIpmiCommand(
                gIpmiTransport,
                IPMI_NETFN_APP,
                BMC_LUN,
                IPMI_APP_SET_USER_PASSWORD,
                (UINT8 *)&SetUserPassword,
                sizeof (SetUserPassword),
                NULL,
                &DataSize );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Set User Password Command Status: %r\n", Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));

    if((!EFI_ERROR(Status)) && (gIpmiTransport->CommandCompletionCode == 0x80)) {
        return EFI_INVALID_PARAMETER;
    }
    return Status;
}

/**
    Deletes user name from BMC.

    @param UserId User Id.

    @return EFI_STATUS
**/

EFI_STATUS
DeleteUserNameFromBmc(
  IN UINT8      UserId )
{
    EFI_STATUS                  Status;
    CHAR8                       DeleteUserName[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Id: %x\n", UserId));

    SetMem (DeleteUserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER, IPMI_BMC_DELETE_USER_DATA);
    Status = SendSetUserNameCommandToBmc ( UserId, DeleteUserName);

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Set User Name Command Status: %r\n", Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return Status;

}

/**
    Displays text box in setup to get user password.

    @param HiiHandle HII Handle.
    @param TitleToken Display title token.
    @param UserPassword User password.

    @retval VOID
**/

VOID
DisplayTextBox(
  IN  VOID        *HiiHandle,
  IN  UINT16      TitleToken,
  IN  CHAR16      **UserPassword )
{
    EFI_STATUS              Status;
    TEXT_INPUT_TEMPLATE     BmcUserPasswordPrompt = {0};

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Inside %a\n", __FUNCTION__));

    //
    // Fill the Text input template
    //
    BmcUserPasswordPrompt.ItemToken = STR_BMC_USER_PASSWORD;
    BmcUserPasswordPrompt.MaxSize = BMC_USER_PASSWORD_MAX_SIZE;
    BmcUserPasswordPrompt.FilterKey = TSE_FILTER_KEY_NULL;
    BmcUserPasswordPrompt.Hidden = TRUE;
    BmcUserPasswordPrompt.Value = *UserPassword;

    //
    // Call DisplayTextBox() function to display the text box in setup and
    // get the Secret string.
    //
    Status = gAmiPostMgr->DisplayTextBox(
                HiiHandle,
                TitleToken,
                &BmcUserPasswordPrompt,
                1,
                NULL );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of DisplayTextBox: %r\n", Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));

    return;

}

/**
    This function Set User Access with ChannelNo,UserId&UserPrivilegeLimit.

    @param ChannelNo - A channel number to giver user access.
    @param UserId - Id of the user to provide user access.
    @param UserPrivilegeLimit - Privilege to be set for the user.

    @retval EFI_STATUS
**/
EFI_STATUS
SetUserAccessCommand(
   IN  UINT8 ChannelNo,
   IN  UINT8 UserId,
   IN  UINT8 UserPrivilegeLimit )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    EFI_SET_USER_ACCESS_COMMAND         SetUserAccess = {0};
    UINT8                               ResponseDataSize;

    SetUserAccess.ChannelNo = ChannelNo;
    SetUserAccess.EnableIpmiMessaging = 1;
    SetUserAccess.EnableUserLinkAuthetication = 1;
    SetUserAccess.UserRestrictedToCallback = 0;
    SetUserAccess.EnableChangeBitsInByte1 = 1;
    SetUserAccess.UserID = UserId;
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: SetUserAccess.UserID = %x\n",SetUserAccess.UserID));
    SetUserAccess.UserPrivilegeLimit = UserPrivilegeLimit;
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: SetUserAccess.UserPrivilegeLimit = %x\n",SetUserAccess.UserPrivilegeLimit));
    SetUserAccess.UserSimultaneousSessionLimit = 0;

    ResponseDataSize = 0;
    Status = gIpmiTransport->SendIpmiCommand (
                                 gIpmiTransport,
                                 IPMI_NETFN_APP,
                                 BMC_LUN,
                                 IPMI_APP_SET_USER_ACCESS,
                                 (UINT8 *)&SetUserAccess,
                                 sizeof(SetUserAccess),
                                 NULL,
                                 &ResponseDataSize );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_SET_USER_ACCESS Command: %r \n", Status));

    return Status;
}
/**
    This function collects all BMC user details from BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcUserSettingsCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    CALLBACK_PARAMETERS         *CallbackParameters;
    STATIC BOOLEAN              IsFirstTimeCallBack = TRUE;
    UINT8                       ChannelNumberBuffer[BMC_LAN_COUNT];
    UINT8                       ChannelCount = BMC_LAN_COUNT;
    DEBUG ((DEBUG_INFO, "%a Entry... Key: %x\n", __FUNCTION__, Key));

    //
    // Continue only for BMC_USER SETTING_KEY
    //
    if ( Key != BMC_USER_SETTINGS_KEY ) {
        DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action: %x\n", CallbackParameters->Action));
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: IsFirstTimeCallBack: %x\n", IsFirstTimeCallBack));
#if COLLECT_BMC_USER_DETAILS_FOR_EVERY_BMC_USER_SETTING_CALLBACK == 0
    if (IsFirstTimeCallBack == FALSE) {
        return EFI_SUCCESS;
    }
#endif

    //
    // Locate AmiPostMgrProtocol to display the message boxes in setup 
    //
    Status = gBS->LocateProtocol (
                &gAmiPostManagerProtocolGuid,
                NULL,
                (VOID **)&gAmiPostMgr);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gAmiPostManagerProtocolGuid locateProtocol Status: %r\n", Status));

    if(EFI_ERROR (Status)) {
        return  EFI_UNSUPPORTED;
    }

    //
    // Locate IpmiTransport protocol
    //
    Status = gBS->LocateProtocol (
                &gEfiDxeIpmiTransportProtocolGuid,
                NULL,
                (VOID **)&gIpmiTransport );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, " IPMI: gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

    //
    // If locate protocol fails, display error message and return
    //
    if (EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_IPMI_PROTOCOL_ERROR) );
        return EFI_UNSUPPORTED;
    }
    //
    // Collect BMC LAN Channels
    //
    gPcdLanChannelBufferPtr = PcdGetPtr(PcdLanChannelPtr);
    gPcdSize = (UINT8)PcdGetSize(PcdLanChannelPtr);
    if (gPcdLanChannelBufferPtr[0] == 0) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: PcdLanChannelPtr is NULL\n"));
        Status = GetLanChannelNumber (gIpmiTransport, ChannelNumberBuffer, &ChannelCount);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: GetLanChannelNumber Status:%r \n",Status));
        if ((EFI_ERROR (Status)) || (gPcdLanChannelBufferPtr[0] == 0)) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_VALID_BMC_LAN_CHANNEL_NOT_FOUND));
            return  EFI_UNSUPPORTED;
        }
    }
    //
    // Collect maximum user allowable 
    //
    GetMaxUserAllowable ();

    if(IsFirstTimeCallBack) {
        CollectAllBmcUserDetails ();
    } else {
#if COLLECT_BMC_USER_DETAILS_FOR_EVERY_BMC_USER_SETTING_CALLBACK
        CollectAllBmcUserDetails ();
#endif
    }

    IsFirstTimeCallBack = FALSE;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return  EFI_SUCCESS;
}

/**
    This function validates whether add new user is possible or not.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcUserSettingsAddUserCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    EFI_STATUS                         Status = EFI_SUCCESS;
    BOOLEAN                            BrowserDataStatus;
    CALLBACK_PARAMETERS                *CallbackParameters;
    BOOLEAN                            IsAddNewUserPossible;
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x\n", __FUNCTION__, Key));

    //
    // Continue only for BMC_ADD USER KEY
    //
    if (Key != ADD_BMC_USER_KEY) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action: %x\n", CallbackParameters->Action));
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    IsAddNewUserPossible = CheckAddNewUserPossibility ();

    if (IsAddNewUserPossible == FALSE) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Add New User Not Possible!!!\n"));
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_NEW_USER_ADD_ERROR) );
        *(CallbackParameters->ActionRequest) = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
        return  EFI_SUCCESS;
    }
    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    ZeroMem (ServerMgmtConfiguration.AddBmcUserName, MAX_BMC_USER_NAME_LENGTH);
    ServerMgmtConfiguration.AddBmcUserPrivilegeLimit  = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcUserCredentials = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcUser = 0;
    ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 0;
    ServerMgmtConfiguration.AddBmcChannelNo = 0;
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid, 
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return  EFI_SUCCESS;
}

/**
    This function send set user name to BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcAddUserNameCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if (Key != ADD_BMC_USER_NAME_KEY) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG ((DEBUG_ERROR,"IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        DEBUG ((DEBUG_INFO,"IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    //
    // Convert User name format from UNICODE to ASCII
    //
    ZeroMem (gBmcAddUserDetails.UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER);
    IpmiUnicode2Ascii (ServerMgmtConfiguration.AddBmcUserName, gBmcAddUserDetails.UserName);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: AddUserName: %s\n", gBmcAddUserDetails.UserName));

    //
    // Check the user name is already available in User Details list
    //
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcAddUserDetails);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if(UserNameAlreadyAvailable) {
        //
        // Display User Name already available pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_NAME_EXISTS));
        ZeroMem (ServerMgmtConfiguration.AddBmcUserName, MAX_BMC_USER_NAME_LENGTH);
        ZeroMem (&gBmcAddUserDetails, sizeof (gBmcAddUserDetails.UserName));
        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit  = 0; 
    } else {
        Status = GetAvailableUserId (&(gBmcAddUserDetails.UserId));
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of GetAvailableUserId Function = %r\n", Status));
        if (EFI_ERROR (Status)) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_ADD_NOT_POSSIBLE));
        } else {
            //
            // Send Set User Name Command
            //
            Status = SendSetUserNameCommandToBmc (gBmcAddUserDetails.UserId, gBmcAddUserDetails.UserName);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Set User Name Command Status: %r\n", Status));
            if (EFI_ERROR (Status)){
                DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_NAME_CMD_FAIL) );
            } else {
                ServerMgmtConfiguration.AddBmcUserValidBmcUser = 1;
            }
        }
        if (EFI_ERROR (Status)) {
            ZeroMem (ServerMgmtConfiguration.AddBmcUserName, MAX_BMC_USER_NAME_LENGTH);
            ZeroMem (&gBmcAddUserDetails, sizeof (BMC_USER_DETAILS));
            ServerMgmtConfiguration.AddBmcUserPrivilegeLimit = 0;
        }
    }

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
	                       &gEfiServerMgmtSetupVariableGuid,
	                       L"ServerSetup",
	                       SelectionBufferSize,
	                       (UINT8*)&ServerMgmtConfiguration,
	                       NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    This function adds user password to BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS

**/
EFI_STATUS
BmcAddUserPasswordCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != ADD_BMC_USER_PASSWORD_KEY ) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    //
    // Return EFI_SUCCESS if the password is NULL.
    //
    if (ServerMgmtConfiguration.AddBmcUserPassword[0] == '\0') {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Add Bmc User Password is NULL\n"));
        return EFI_SUCCESS;
    }

    //
    // Convert User Password format from UNICODE to ASCII
    //
    ZeroMem (gBmcAddUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER);
    IpmiUnicode2Ascii (ServerMgmtConfiguration.AddBmcUserPassword, gBmcAddUserDetails.UserPassword);

    Status = PerformGivenUserPasswordAction (gBmcAddUserDetails.UserId, TwentyByte, SetPassword, (CHAR8 *)gBmcAddUserDetails.UserPassword);
    if( EFI_ERROR (Status) ) {

        //
        // Display Set User password Failed pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_PASSWORD_FAIL) );
        ZeroMem (ServerMgmtConfiguration.AddBmcUserPassword, MAX_BMC_USER_PASSWORD_LENGTH);
        ZeroMem (&gBmcAddUserDetails.UserPassword, sizeof (gBmcAddUserDetails.UserPassword));
    } else {
        AddUserEntryToList (&gBmcAddUserDetails);
        ZeroMem (ServerMgmtConfiguration.AddBmcUserPassword, MAX_BMC_USER_PASSWORD_LENGTH); //Clearing the password from browser data
        ServerMgmtConfiguration.AddBmcUserValidBmcUserCredentials = 1;
        // Setting user with default channel and Access
        Status = PerformGivenUserPasswordAction (gBmcAddUserDetails.UserId, TwentyByte, DisableUser, gBmcAddUserDetails.UserPassword);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: PerformGivenUserPasswordAction disable Access Status= %r\n",Status ));
        if (gPcdLanChannelBufferPtr[0] != 0) {
            Status = SetUserAccessCommand((UINT8)gPcdLanChannelBufferPtr[0],(UINT8)gBmcAddUserDetails.UserId,USER_PRIVILEGE_NO_ACCESS);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: SetUserAccessCommand Status= %r\n",Status ));
            if (EFI_ERROR (Status)) {
                DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
                return EFI_UNSUPPORTED;
            }
        }
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_ADD_USERNAME_OPERATION_PASSED) );
    }
    ServerMgmtConfiguration.AddBmcChannelNo = 0;
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function sends get channel access command to BMC for given input.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcAddUserChannelCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand = {0};
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse = {0};
    UINT8                               GetChannelAccessResponseDataSize;
    UINT8                               Index;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));
    //
    // Check for the key and Return if Key value does not match
    //
    if (Key != ADD_BMC_USER_CHANNEL_NO_KEY) {
        DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO,"IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User entered channel no = %x\n", ServerMgmtConfiguration.AddBmcChannelNo));
    if (gPcdLanChannelBufferPtr[0] == 0) {
        return EFI_UNSUPPORTED; 
    }
    for (Index=0;Index<gPcdSize;Index++) {
         if ((gPcdLanChannelBufferPtr[Index] == ServerMgmtConfiguration.AddBmcChannelNo)) {
             GetChannelAccessCommand.ChannelNo = ServerMgmtConfiguration.AddBmcChannelNo;
             GetChannelAccessCommand.MemoryType = NonVolatile;// Non Volatile channel Access
             GetChannelAccessResponseDataSize = sizeof (GetChannelAccessResponse);
             Status = gIpmiTransport->SendIpmiCommand (
                                          gIpmiTransport,
                                          IPMI_NETFN_APP,
                                          BMC_LUN,
                                          IPMI_APP_GET_CHANNEL_ACCESS,
                                          (UINT8 *)&GetChannelAccessCommand,
                                          sizeof (GetChannelAccessCommand),
                                          (UINT8 *)&GetChannelAccessResponse,
                                          &GetChannelAccessResponseDataSize);
             SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
             SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));
             SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.ChannelPriviledgeLimit));
             ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.ChannelPriviledgeLimit;
             break;
         }
    }
    if ((EFI_ERROR (Status)) || (gIpmiTransport->CommandCompletionCode != COMPLETION_CODE_SUCCESS) || (Index == gPcdSize)) {
        DisplayErrorMessagePopUp (HiiHandle, STRING_TOKEN(STR_GET_CHANNEL_ACCESS_CMD_FAIL));
        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit  = 0;
        ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel =0;
        ServerMgmtConfiguration.AddBmcChannelNo = 0;
        ServerMgmtConfiguration.AddBmcUserAccess = 0;
        ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 0;
    } else  {
        ServerMgmtConfiguration.AddBmcUserValidBmcLanChannel = 1;
        Status = SetUserAccessCommand((UINT8)ServerMgmtConfiguration.AddBmcChannelNo,(UINT8)gBmcAddUserDetails.UserId,USER_PRIVILEGE_NO_ACCESS);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: SetUserAccessCommand Status= %r\n",Status ));
        if (EFI_ERROR (Status)) {
           DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
           return EFI_UNSUPPORTED;
        }
    }

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiLibSetBrowserData() = %d\n", BrowserDataStatus));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: AddBmcUserMaximumPossibleUserPrivilegeLevel(After Set Browser data) = %x\n", ServerMgmtConfiguration.AddBmcUserMaximumPossibleUserPrivilegeLevel));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    return EFI_SUCCESS;
}

/**
    This function sends set user access command to BMC for given input.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcAddUserChannelPrivilegeLimitCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if (Key != ADD_BMC_USER_PRIVILEGE_LIMIT_KEY) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: AddBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.AddBmcUserPrivilegeLimit));
    Status = SetUserAccessCommand((UINT8)ServerMgmtConfiguration.AddBmcChannelNo,(UINT8)gBmcAddUserDetails.UserId,(UINT8)ServerMgmtConfiguration.AddBmcUserPrivilegeLimit);
    if(EFI_ERROR (Status)) {
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
        ServerMgmtConfiguration.AddBmcUserPrivilegeLimit = 0;

        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        if (!BrowserDataStatus) {
             return EFI_UNSUPPORTED;
        }
        return EFI_SUCCESS;
    }
    return EFI_SUCCESS;
}
/**
    This function enables/disables a user in BMC as per given input.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/
EFI_STATUS
BmcAddUserAccessCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != ADD_BMC_USER_ACCESS_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    } else if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }
    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiLibGetBrowserData() = %r\n", Status));
    ASSERT_EFI_ERROR(Status);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n ServerMgmtConfiguration.ChangeUserSettingsBmcUser: %x\n", ServerMgmtConfiguration.AddBmcUserAccess));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI:gBmcChangeUserDetails.UserId %x\n", gBmcAddUserDetails.UserId));
    Status = PerformGivenUserPasswordAction( gBmcAddUserDetails.UserId, TwentyByte, ServerMgmtConfiguration.AddBmcUserAccess, gBmcAddUserDetails.UserPassword);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Enable/Disable Status: %r\n", Status));
    if (EFI_ERROR (Status)) {
        if (ServerMgmtConfiguration.AddBmcUserAccess) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_ENABLE_OPERATION_FAIL) );
        } else {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_DISABLE_OPERATION_FAIL) );
        }
        ServerMgmtConfiguration.AddBmcUserAccess = 0;
        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                                 &gEfiServerMgmtSetupVariableGuid,
                                 L"ServerSetup",
                                 SelectionBufferSize,
                                 (UINT8*)&ServerMgmtConfiguration,
                                  NULL);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        if (!BrowserDataStatus) {
            return EFI_UNSUPPORTED;
        }
    } else {
        if (ServerMgmtConfiguration.AddBmcUserAccess) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_ENABLE_OPERATION_PASS) );
        } else {
           DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_DISABLE_OPERATION_PASS) );
        }
    }
    return EFI_SUCCESS;
}
/**
    This function Clears previous NVRAM value before performing delete user action.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcDeleteUserCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    EFI_STATUS                         Status = EFI_SUCCESS;
    BOOLEAN                            BrowserDataStatus;
    CALLBACK_PARAMETERS                *CallbackParameters;
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key Value: %x\n", __FUNCTION__, Key));

    //
    // Continue only for BMC_ADD USER KEY
    //
    if ( Key != DELETE_BMC_USER_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action: %x\n", CallbackParameters->Action));
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    ZeroMem (ServerMgmtConfiguration.DeleteBmcUserName, MAX_BMC_USER_NAME_LENGTH); 
    ServerMgmtConfiguration.DeleteValidBmcUser = 0;
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return  EFI_SUCCESS;
}

/**
    This function gets the user name for deleting a user from BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcDeleteUserNameCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != DELETE_BMC_USER_NAME_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if ( CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
   BrowserDataStatus = HiiGetBrowserData (
                          &gEfiServerMgmtSetupVariableGuid,
                          L"ServerSetup",
                          SelectionBufferSize,
                          (UINT8*)&ServerMgmtConfiguration);
   SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    //
    // Convert User name format from UNICODE to ASCII
    //
    ZeroMem (gBmcDeleteUserDetails.UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER);
    IpmiUnicode2Ascii (ServerMgmtConfiguration.DeleteBmcUserName, gBmcDeleteUserDetails.UserName);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gBmcDeleteUserDetails.UserName = %s\n", gBmcDeleteUserDetails.UserName));

    //
    // Check the user name is already available in User Details list
    //
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcDeleteUserDetails);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if( !UserNameAlreadyAvailable ) {

        //
        // Display User Name not available pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_NAME_NOT_AVAILABLE) );
        ZeroMem (ServerMgmtConfiguration.DeleteBmcUserName, MAX_BMC_USER_NAME_LENGTH);
        ZeroMem (&gBmcDeleteUserDetails, sizeof (gBmcDeleteUserDetails));
    } else {
        ServerMgmtConfiguration.DeleteValidBmcUser = 1;
    }
        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        if (!BrowserDataStatus) {
            return EFI_UNSUPPORTED;
        }

    return EFI_SUCCESS;
}

/**
    This function gets the user password for deleting a user from BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
BmcDeleteUserPasswordCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    TEXT_INPUT_TEMPLATE                 BmcDeleteUserPasswordPrompt = {0};
    CHAR16                              *UserPassword = NULL;
    CHAR16                              Password[MAX_BMC_USER_PASSWORD_LENGTH];

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != DELETE_BMC_USER_PASSWORD_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }

    //
    // Call DisplayTextBox function to get user password
    //
    UserPassword = Password;
    ZeroMem (UserPassword, MAX_BMC_USER_PASSWORD_LENGTH);
    DisplayTextBox (HiiHandle, STR_BMC_USER_PASSWORD, (CHAR16 **)&UserPassword );
    IpmiUnicode2Ascii ((CHAR16 *)UserPassword, gBmcDeleteUserDetails.UserPassword);

    if ( ( (AsciiStrLen (gBmcDeleteUserDetails.UserPassword) < BMC_USER_PASSWORD_MIN_SIZE) || \
           (AsciiStrLen (gBmcDeleteUserDetails.UserPassword) > BMC_USER_PASSWORD_MAX_SIZE) ) ) {
        //
        // Display User Name not available pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_PASSWORD_INVALID_SIZE) );
        ZeroMem (gBmcDeleteUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER);
        return EFI_INVALID_PARAMETER;
    }

    Status = PerformGivenUserPasswordAction ( gBmcDeleteUserDetails.UserId, TwentyByte, TestPassword, gBmcDeleteUserDetails.UserPassword);
    if (EFI_ERROR (Status)){
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_PASSWORD_MISMATCH) );
        return EFI_UNSUPPORTED;
    }

    Status = DeleteUserNameFromBmc (gBmcDeleteUserDetails.UserId);
    if ( EFI_ERROR(Status)){
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_NAME_DELETE_FAIL) );
        return EFI_UNSUPPORTED;
    }

    Status = DeleteUserEntryFromList (gBmcDeleteUserDetails.UserName);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nStatus of DeleteUserEntryFromList = %r\n", Status));
    if (EFI_ERROR (Status)){
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_NAME_DELETE_FAIL) );
        return EFI_UNSUPPORTED;
    }
    ServerMgmtConfiguration.DeleteValidBmcUser = 0;
    DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_DELETED) );

    ZeroMem (&gBmcDeleteUserDetails, sizeof (gBmcDeleteUserDetails));
    ZeroMem (ServerMgmtConfiguration.DeleteBmcUserName, MAX_BMC_USER_NAME_LENGTH);

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    This function load default values to controls.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != CHANGE_BMC_USER_SETTINGS_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

        ZeroMem (ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName, MAX_BMC_USER_NAME_LENGTH);
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", Status));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    This function gets the current user name for changing a user details from BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsCurrentBmcUserNameCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    BOOLEAN                             UserNameAlreadyAvailable;
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != CHANGE_USER_SETTINGS_BMC_CURRENT_USER_NAME_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if ( CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    //
    // Convert User name format from UNICODE to ASCII
    //
    ZeroMem (gBmcChangeUserDetails.UserName, MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER);
    IpmiUnicode2Ascii (ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName, gBmcChangeUserDetails.UserName);

    //
    // Check the user name is already available in User Details list
    //
    UserNameAlreadyAvailable = CheckUserNameAvailability (&gBmcChangeUserDetails);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: UserNameAlreadyAvailable:%x\n", UserNameAlreadyAvailable));

    if( !UserNameAlreadyAvailable ) {
        //
        // Display User Name not available pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_NAME_NOT_FOUND) );
        ZeroMem (ServerMgmtConfiguration.ChangeUserSettingsBmcCurrentUserName, MAX_BMC_USER_NAME_LENGTH);
        ZeroMem (gBmcChangeUserDetails.UserName, sizeof (gBmcChangeUserDetails.UserName));
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 0;
     
    } else {
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUser = 1;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 0;
        ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 0;
    }

    ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;
    ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;
    ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    This function gets the current user password for changing a user details from BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsCurrentBmcUserPasswordCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    
    EFI_GET_USER_ACCESS_COMMAND         GetUserAccesscommond={0};
    EFI_GET_USER_ACCESS_RESPONSE        GetUserAccessResponse={0};
    UINT8                               GetUserAccessResponseDataSize;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand = {0};
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse = {0};
    UINT8                               GetChannelAccessResponseDataSize;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    TEXT_INPUT_TEMPLATE                 ChangeUserSettingsCurrentBmcUserPasswordPrompt = {0};
    CHAR16                              *UserPassword;
    CHAR16                              Password[MAX_BMC_USER_PASSWORD_LENGTH];
    UINT8                               Index;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != CHANGE_USER_SETTINGS_BMC_USER_CURRENT_PASSWORD_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    //
    // Call DisplayTextBox function to get user password
    //
    UserPassword = Password;
    ZeroMem (UserPassword, MAX_BMC_USER_PASSWORD_LENGTH);
    DisplayTextBox (HiiHandle, STR_BMC_USER_PASSWORD, (CHAR16 **)&UserPassword );
    IpmiUnicode2Ascii ((CHAR16 *)UserPassword, gBmcChangeUserDetails.UserPassword);

    if ( ( (AsciiStrLen (gBmcChangeUserDetails.UserPassword) < BMC_USER_PASSWORD_MIN_SIZE) || \
           (AsciiStrLen (gBmcChangeUserDetails.UserPassword) > BMC_USER_PASSWORD_MAX_SIZE) ) ) {
        //
        // Display User Name not available pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_INVALID_USER_PASSWORD) );
        ZeroMem (gBmcChangeUserDetails.UserPassword, sizeof (gBmcChangeUserDetails.UserPassword));
        return EFI_INVALID_PARAMETER;
    }

    Status = PerformGivenUserPasswordAction ( gBmcChangeUserDetails.UserId, TwentyByte, TestPassword, gBmcChangeUserDetails.UserPassword);
    if (EFI_ERROR (Status)){
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_PASSWORD_MISMATCH) );
        ZeroMem (gBmcChangeUserDetails.UserPassword, sizeof (gBmcChangeUserDetails.UserPassword));
        return EFI_UNSUPPORTED;
    } else {
        ServerMgmtConfiguration.ChangeUserSettingsValidPassword = 1;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcUserCredentials = 1;
        for(Index=0;Index<gPcdSize;Index++){
            GetChannelAccessCommand.ChannelNo = gPcdLanChannelBufferPtr[Index];
            GetChannelAccessCommand.MemoryType = NonVolatile;// Non Volatile channel Access
            GetChannelAccessResponseDataSize = sizeof (GetChannelAccessResponse);
            Status = gIpmiTransport->SendIpmiCommand (
                                          gIpmiTransport,
                                          IPMI_NETFN_APP,
                                          BMC_LUN,
                                          IPMI_APP_GET_CHANNEL_ACCESS,
                                          (UINT8 *)&GetChannelAccessCommand,
                                          sizeof (GetChannelAccessCommand),
                                          (UINT8 *)&GetChannelAccessResponse,
                                           &GetChannelAccessResponseDataSize);
           SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
           SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));
           SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.ChannelPriviledgeLimit));
           ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.ChannelPriviledgeLimit;    
           if (!EFI_ERROR (Status)) {
               GetUserAccessResponseDataSize= sizeof(GetUserAccessResponse);
               GetUserAccesscommond.UserId =gBmcChangeUserDetails.UserId;
               GetUserAccesscommond.ChannelNo=gPcdLanChannelBufferPtr[Index];
               Status = gIpmiTransport->SendIpmiCommand(
                                            gIpmiTransport,
                                            IPMI_NETFN_APP,
                                            BMC_LUN,
                                            IPMI_APP_GET_USER_ACCESS,
                                            (UINT8 *)&GetUserAccesscommond,
                                            sizeof (GetUserAccesscommond),
                                            (UINT8*)&GetUserAccessResponse,
                                            &GetUserAccessResponseDataSize);
               SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: IPMI_APP_GET_USER_ACCESS = %r\n", Status));
               if ((!EFI_ERROR (Status)) && (gIpmiTransport->CommandCompletionCode == COMPLETION_CODE_SUCCESS)) {
                   ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = GetUserAccessResponse.UserIdEnable;
                   if (GetUserAccessResponse.UserPrivilegeLimit != USER_PRIVILEGE_NO_ACCESS) {
                       ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = gPcdLanChannelBufferPtr[Index];
                       ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = GetUserAccessResponse.UserPrivilegeLimit;
                       ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 1;
                       break;
                   }
               }
            }
        }
    }
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nStatus of HiiSetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function gets new user password for changing a user details in BMC.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsBmcUserPasswordCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != CHANGE_USER_SETTINGS_BMC_USER_PASSWORD_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    //
    // Return EFI_SUCCESS if the password is NULL.
    //
    if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword[0] == '\0') {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Change Bmc User Password is NULL\n"));
        return EFI_SUCCESS;
    }
    //
    // Convert User Password format from UNICODE to ASCII
    //
    ZeroMem (gBmcChangeUserDetails.UserPassword, MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER);
    IpmiUnicode2Ascii (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword, gBmcChangeUserDetails.UserPassword);

    Status = PerformGivenUserPasswordAction (gBmcChangeUserDetails.UserId, TwentyByte, SetPassword, (CHAR8 *)gBmcChangeUserDetails.UserPassword);
    if (EFI_ERROR (Status)) {
        //
        // Display Set User password Failed pop up message
        //
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_PASSWORD_FAIL) );
        ZeroMem (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword, MAX_BMC_USER_PASSWORD_LENGTH);
        ZeroMem (&gBmcChangeUserDetails.UserPassword, sizeof(gBmcChangeUserDetails.UserPassword));
    } else {
        ZeroMem (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPassword, MAX_BMC_USER_PASSWORD_LENGTH); //Clearing the password from browser data
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_CHANGED_USERPASSWORD_OPERATION_PASSED) );
    }

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                            &gEfiServerMgmtSetupVariableGuid,
                            L"ServerSetup",
                            SelectionBufferSize,
                            (UINT8*)&ServerMgmtConfiguration,
                            NULL );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Returning from %a\n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function enables/disables a user in BMC as per given input.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsEnableOrDisableBmcUserCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != CHANGE_USER_SETTINGS_BMC_USER_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters();
    if ( CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n ServerMgmtConfiguration.ChangeUserSettingsBmcUser: %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI:gBmcChangeUserDetails.UserId %x\n", gBmcChangeUserDetails.UserId));
    Status = PerformGivenUserPasswordAction( gBmcChangeUserDetails.UserId, TwentyByte, ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess, gBmcChangeUserDetails.UserPassword);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User Enable/Disable Status: %r\n", Status));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
    if (EFI_ERROR (Status)) {
        if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_ENABLE_OPERATION_FAIL) );
        } else {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_DISABLE_OPERATION_FAIL) );
        }
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = 0;

        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL);        
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nStatus of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        if (!BrowserDataStatus) {
             return EFI_UNSUPPORTED;
        }
    } else {
        if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess) {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_ENABLE_OPERATION_PASS) );
        } else {
            DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_USER_DISABLE_OPERATION_PASS) );
        }
    }

    return EFI_SUCCESS;
}

/**
    This function sends get channel access command for the given channel and
    determines the maximum possible privilege level for the channel.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsBmcUserChannelCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    EFI_GET_USER_ACCESS_COMMAND         GetUserAccesscommond={0};
    EFI_GET_USER_ACCESS_RESPONSE        GetUserAccessResponse={0};
    UINT8                               GetUserAccessResponseDataSize;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     GetChannelAccessCommand = {0};
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    GetChannelAccessResponse = {0};
    UINT8                               GetChannelAccessResponseDataSize;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;
    UINT8                               Index;
    

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_CHANNEL_NO_KEY) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser Data
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: User entered channel no = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo));
    for (Index=0;Index<gPcdSize;Index++) {
        if ((gPcdLanChannelBufferPtr[Index] == ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo)) {
            GetChannelAccessCommand.ChannelNo = ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo;
            GetChannelAccessCommand.MemoryType = NonVolatile;// Non Volatile channel Access
            GetChannelAccessResponseDataSize = sizeof (GetChannelAccessResponse);
            Status = gIpmiTransport->SendIpmiCommand (
                                         gIpmiTransport,
                                         IPMI_NETFN_APP,
                                         BMC_LUN,
                                         IPMI_APP_GET_CHANNEL_ACCESS,
                                         (UINT8 *)&GetChannelAccessCommand,
                                         sizeof (GetChannelAccessCommand),
                                         (UINT8 *)&GetChannelAccessResponse,
                                          &GetChannelAccessResponseDataSize);
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CommandCompletionCode: %x \n", gIpmiTransport->CommandCompletionCode));
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of IPMI_APP_GET_CHANNEL_ACCESS Command: %r \n", Status));
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: GetChannelAccessResponse.ChannelPriviledgeLimit: %x \n", GetChannelAccessResponse.ChannelPriviledgeLimit));
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = GetChannelAccessResponse.ChannelPriviledgeLimit;
            break;
        }
    }
    if ((EFI_ERROR (Status)) || (gIpmiTransport->CommandCompletionCode != COMPLETION_CODE_SUCCESS) || (Index == gPcdSize)) {
        DisplayErrorMessagePopUp (HiiHandle, STRING_TOKEN(STR_GET_CHANNEL_ACCESS_CMD_FAIL));
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit  = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel =0;
        ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 0;
        ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo = 0;
    } else {
        GetUserAccessResponseDataSize= sizeof(GetUserAccessResponse);
        GetUserAccesscommond.UserId = gBmcChangeUserDetails.UserId;
        GetUserAccesscommond.ChannelNo = ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo;
        Status = gIpmiTransport->SendIpmiCommand(
                                     gIpmiTransport,
                                     IPMI_NETFN_APP,
                                     BMC_LUN,
                                     IPMI_APP_GET_USER_ACCESS,
                                     (UINT8 *)&GetUserAccesscommond,
                                     sizeof (GetUserAccesscommond),
                                     (UINT8*)&GetUserAccessResponse,
                                     &GetUserAccessResponseDataSize);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: IPMI_APP_GET_USER_ACCESS = %r\n", Status));
        if ((!EFI_ERROR (Status)) && (gIpmiTransport->CommandCompletionCode == COMPLETION_CODE_SUCCESS)) {
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserAccess = GetUserAccessResponse.UserIdEnable;
            ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = GetUserAccessResponse.UserPrivilegeLimit;
            ServerMgmtConfiguration.ChangeUserSettingsValidBmcLanChannel = 1;
            if (ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit > ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel) {
                SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI:BmcUserPrivilegeLimit is higher than Channel maximum privilegeLimit\n"));
                ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel = ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit;
            }
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: ChangeUserSettingsBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit));
        } else {
            return EFI_UNSUPPORTED;
            }
    }
        
    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: ChangeUserMaximumPossibleUserPrivilegeLevel(After Set Browser data) = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserMaximumPossibleUserPrivilegeLevel));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;
}

/**
    This function sets the privilege level for a user as per the given input.

    @param HiiHandle - A handle that was previously registered in the
                               HII Database.
    @param Class - Class of the Form Callback Protocol passed in
    @param SubClass - Sub Class of the Form Callback Protocol passed in
    @param Key - Key of the Form Callback Protocol passed in

    @return EFI_STATUS
**/

EFI_STATUS
ChangeUserSettingsBmcUserChannelPrivilegeLimitCallbackFunction(
  IN  EFI_HII_HANDLE    HiiHandle,
  IN  UINT16            Class,
  IN  UINT16            SubClass,
  IN  UINT16            Key )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status = EFI_SUCCESS;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS                 *CallbackParameters;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if (Key != CHANGE_USER_SETTINGS_BMC_USER_PRIVILEGE_LIMIT_KEY) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters ();
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so return EFI_UNSUPPORTED\n"));
        return  EFI_UNSUPPORTED;
    }
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiGetBrowserData() = %d\n", BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: ChangeUserSettingsBmcUserPrivilegeLimit = %x\n", ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit));
    Status = SetUserAccessCommand((UINT8)ServerMgmtConfiguration.ChangeUserSettingsBmcChannelNo,(UINT8)gBmcChangeUserDetails.UserId,(UINT8)ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit);
    if (EFI_ERROR(Status)) {
        DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_ACCESS_CMD_FAIL) );
        ServerMgmtConfiguration.ChangeUserSettingsBmcUserPrivilegeLimit = 0;
        //
        // Set Browser DATA
        //
        BrowserDataStatus = HiiSetBrowserData (
                               &gEfiServerMgmtSetupVariableGuid,
                               L"ServerSetup",
                               SelectionBufferSize,
                               (UINT8*)&ServerMgmtConfiguration,
                               NULL);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: Status of HiiSetBrowserData() = %d\n", BrowserDataStatus));
        if (!BrowserDataStatus) {
             return EFI_UNSUPPORTED;
        }
        return EFI_SUCCESS;
    }
    DisplayErrorMessagePopUp ( HiiHandle, STRING_TOKEN(STR_SET_USER_ACCESS_CMD_PASS) );
    return EFI_SUCCESS;
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
