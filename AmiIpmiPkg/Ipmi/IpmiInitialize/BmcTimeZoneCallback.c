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

/** @file BmcTimeZoneCallback.c
    1. Using setup callback function, validates Time Zone entered in Setup
    2. Using Hook function, loads optimal defaults when F3 is pressed

**/

//----------------------------------------------------------------------

#include <Token.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/RealTimeClock.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <Include/IpmiNetFnStorageDefinitions.h>
#include <ServerMgmtSetupStrDefs.h>
#include <ServerMgmtSetup.h>
#include <Library/HiiLib.h>

//----------------------------------------------------------------------

//----------------------------------------------------------------------

#define MAX_UTC_OFFSET      1440
#define MIN_UTC_OFFSET      0
#define STRING_BUFFER_LENGTH  100

//----------------------------------------------------------------------

/**
    Sets default Time Zone value to 0x7FFFF

    @param TimeZone [] - Pointer to TimeZone in BIOS setup variable

    @return VOID

**/

VOID
SetDefaultTimeZone (
  IN OUT  CHAR16  TimeZone[] )
{
    TimeZone[0] ='0';
    TimeZone[1] ='x';
    TimeZone[2] ='7';
    TimeZone[3] ='F';
    TimeZone[4] ='F';
    TimeZone[5] ='F';
}
/**
    Validates Time zone entered in Setup and returns Status.
    1. Checks for special condition 0x7FFF. Return EFI_SUCCESS if time zone 
    entered in setup is 0x7FFF
    2. Checks below conditions also
    a. Time should be in (+/-)hh:mm format
    b. TimeZone[0] should be +/- character
    c. TimeZone[1] should be between 0 and 2
    d. TimeZone[2] should be between 0 and 4
    e. TimeZone[3] should be : character
    f. TimeZone[4] should be between 0 and 5
    g. TimeZone[5] should be between 0 and 9

    @param TimeZone [] - Pointer to TimeZone in BIOS setup variable

    @return EFI_STATUS
    @retval EFI_SUCCESS - for valid Time zone
    @retval EFI_INVALID_PARAMETER - for in valid Time zone

**/

EFI_STATUS
ValidateTimeZone (
  IN  CHAR16  TimeZone[] )
{
    INT16       UtcOffset = 0;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... TimeZone: %S \n", __FUNCTION__, TimeZone));

    //
    // Check for special condition 0x7FFF. Return success if condition met
    // When 0x7FFF is entered in setup, BIOS time is considered as local time
    //
    if ( ( TimeZone[0] == '0') && ( (TimeZone[1] == 'x') ||\
            (TimeZone[1] == 'X') ) && (TimeZone[2] == '7') && \
            ( (TimeZone[3] == 'f') || (TimeZone[3] == 'F') ) &&\
            ( (TimeZone[4] == 'f') || (TimeZone[4] == 'F') ) &&\
            ( (TimeZone[5] == 'f') || (TimeZone[5] == 'F') ) ) {
        return EFI_SUCCESS;
    }

    //
    // Check for special characters
    //
    if ( (TimeZone[0] != '+') && (TimeZone[0] != '-') ) {
        goto ErrorExit;
    } else if ( TimeZone[3] != ':' ) {
        goto ErrorExit;
    }

    //
    // Validate TimeZone[5] range. Range: 0 - 9
    // Converting char to number - 1's position
    //
    if ( (TimeZone[5] >= '0') && (TimeZone[5] <= '9') ) {
        UtcOffset += (TimeZone[5] - 0x30);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, " After Converting char to number - 1's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    //
    // Validate TimeZone[4] range. Range: 0 - 5
    // Converting char to number - 10's position
    //
    if ( (TimeZone[4] >= '0') && (TimeZone[4] <= '5') ) {
        UtcOffset += ( (TimeZone[4] - 0x30) * 10);
        SERVER_IPMI_DEBUG ((DEBUG_INFO," After Converting char to number - 10's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    //
    // Validate TimeZone[2] range. Range: 0 - 9
    // Converting Hour to minute - 1's position
    //
    if ( (TimeZone[2] >= '0') && (TimeZone[2] <= '9') ) {
        UtcOffset += ( (TimeZone[2] - 0x30) * 60);
        SERVER_IPMI_DEBUG ((DEBUG_INFO," After Converting Hour to minute - 1's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    //
    // Validate TimeZone[1] range. Range: 0 - 2
    // Converting Hour to minute - 10's position
    //
    if ( (TimeZone[1] >= '0') && (TimeZone[1] <= '2') ) {
        UtcOffset += ( (TimeZone[1] - 0x30) * 60 * 10);
        SERVER_IPMI_DEBUG ((DEBUG_INFO," After Converting Hour to minute - 10's position UtcOffset: %d \n", UtcOffset));
    } else {
        goto ErrorExit;
    }

    //
    // UtcOffset should be in the range 0 to 1440
    // check for the range.
    //
    if ( (UtcOffset <= MAX_UTC_OFFSET) && \
         (UtcOffset >= MIN_UTC_OFFSET) ) {
        return EFI_SUCCESS;
    }

ErrorExit:
    return EFI_INVALID_PARAMETER;
}

/**
    This function validate the UTC offset given in setup and display error
    messages if given input is invalid data

    @param HiiHandle A handle that was previously registered in the
                     HII Database.
    @param Class    Formset Class of the Form Callback Protocol passed in
    @param SubClass Formset sub Class of the Form Callback Protocol passed in
    @param Key Formset Key of the Form Callback Protocol passed in

    @return EFI_STATUS Return Status

**/

EFI_STATUS
BmcTimeZoneCallback (
  IN  EFI_HII_HANDLE     HiiHandle,
  IN  UINT16             Class,
  IN  UINT16             SubClass,
  IN  UINT16             Key )
{

    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration ;
    BOOLEAN                            BrowserDataStatus;
    EFI_STATUS                         Status = EFI_SUCCESS;
    EFI_STRING                         OutputString;
    EFI_STRING                         CharPtrString;
    UINTN                              SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    CALLBACK_PARAMETERS         *CallbackParameters;
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nEntered BmcTimeZoneCallback Key: %x BMC_TIME_ZONE_KEY: %x \n", Key, BMC_TIME_ZONE_KEY));

    //
    // Check for the key and Return if Key value does not match
    //
    if ( Key != BMC_TIME_ZONE_KEY ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"\nCallback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }
    //
    // Get the call back parameters and verify the action
    //
    CallbackParameters = GetCallbackParameters();
    if ( CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD ) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so load the defaults \n"));
        return  EFI_UNSUPPORTED;
    } else if ( CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED ) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "\n CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n",Status));
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
    SERVER_IPMI_DEBUG ((DEBUG_INFO,"\nStatus of HiiGetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }
    //
    // Validate UTC TimeZone offset and display error message
    //
    Status = ValidateTimeZone ( ServerMgmtConfiguration.TimeZone );
    if ( EFI_ERROR (Status) ) {

        AMI_POST_MANAGER_PROTOCOL           *AmiPostMgr = NULL;
        UINT8                               MsgBoxSel;
        //
        // Set Default time zone for error case
        //
        SetDefaultTimeZone (ServerMgmtConfiguration.TimeZone);
        
        //
        // Locate AmiPostMgr protocol to print the Error message
        //
        if ( !EFI_ERROR(gBS->LocateProtocol
            (&gAmiPostManagerProtocolGuid, NULL, (VOID **)&AmiPostMgr)) ) {
            OutputString = HiiGetString ( HiiHandle, STRING_TOKEN(STR_INVALID_TIME_ZONE), NULL);
            CharPtrString = HiiGetString ( HiiHandle, STRING_TOKEN(STR_TIME_ZONE_ERROR_INFO), NULL);
            if ((OutputString != NULL) && (CharPtrString != NULL)) {
                AmiPostMgr->DisplayMsgBox (
                                CharPtrString,
                                OutputString,
                                MSGBOX_TYPE_OK,
                                &MsgBoxSel );
                gBS->FreePool(OutputString);
                gBS->FreePool(CharPtrString);
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
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "\nStatus of HiiSetBrowserData() = %d\n",BrowserDataStatus));
        if (!BrowserDataStatus) {
            return EFI_UNSUPPORTED;
        }
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return EFI_SUCCESS;
}

/**
    This function initializes the current BMC Time Zone value display

    @param HiiHandle Handle to HII database
    @param Class Indicates the setup class

    @return VOID

**/

VOID
InitBmcTimeZoneDisplay(
  IN EFI_HII_HANDLE     HiiHandle,
  IN UINT16             Class )
{
    EFI_IPMI_TRANSPORT    *IpmiTransport;
    EFI_STATUS            Status;
    UINT8                 DataSize;
    INT16                 BmcUtcOffset = 0x00;
    CHAR16                CurrTimeZone[7] = {0};

    //
    // Locate IpmiTransport protocol
    //
    Status = gBS->LocateProtocol (
                        &gEfiDxeIpmiTransportProtocolGuid,
                        NULL,
                        (VOID**)&IpmiTransport );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entry... gEfiDxeIpmiTransportProtocolGuid Status: %r \n", __FUNCTION__, Status));

    if(!EFI_ERROR (Status)) {

        //
        // Get SEL Time UTC Offset Command
        //
        DataSize = sizeof (BmcUtcOffset);
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    BMC_LUN,
                                    EFI_STORAGE_GET_SEL_TIME_UTC_OFFSET,
                                    NULL,
                                    0,
                                    (UINT8*)&(BmcUtcOffset),
                                    &DataSize );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, " In InitBmcTimeZoneDisplay : EFI_STORAGE_GET_SEL_TIME_UTC_OFFSET Status: %r\n", Status));
        if (!EFI_ERROR (Status)) {

            if (BmcUtcOffset != EFI_UNSPECIFIED_TIMEZONE) {
                CurrTimeZone[0] = '+';
                if (BmcUtcOffset < 0) {
                    CurrTimeZone[0] = '-';
                    BmcUtcOffset *= -1;
                }
                CurrTimeZone[3] = ':';
                CurrTimeZone[1] = ((BmcUtcOffset / 60 ) / 10) + 0x30;
                CurrTimeZone[2] = ((BmcUtcOffset / 60 ) % 10) + 0x30;
                CurrTimeZone[4] = ((BmcUtcOffset % 60 ) / 10) + 0x30;
                CurrTimeZone[5] = ((BmcUtcOffset % 60 ) % 10) + 0x30;
                HiiSetString (
                    HiiHandle,
                    STRING_TOKEN(STR_BMC_CURR_TIME_ZONE_VALUE),
                    CurrTimeZone,
                    NULL);
                return;
            }
        }
    }
    return;
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
