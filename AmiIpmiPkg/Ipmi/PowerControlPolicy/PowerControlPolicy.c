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

/** @file PowerControlPolicy.c
    Reads the Power Control Policy from BMC and updates in Setup
    and Power Control Policy to BMC according to the Setup Option selected using Setup Callback.
 
**/
//----------------------------------------------------------------------

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/AMIPostMgr.h>
#include <Uefi/UefiSpec.h>
#include <Include/IpmiNetFnChassisDefinitions.h>
#include <ServerMgmtSetup.h>
#include <Setup.h>
#include <Library/HiiLib.h>

//----------------------------------------------------------------------

#define RESERVED                0x00
#define MASK_VALUE              0x60
#define STRING_BUFFER_LENGTH    50

AMI_POST_MANAGER_PROTOCOL       *gPcpPostMgr = NULL;
static EFI_IPMI_TRANSPORT       *gIpmiTransport = NULL;

//----------------------------------------------------------------------

/**
    Displays error message pop up in setup.

    @param HiiHandle - Handle to HII database.
    @param Message - Error message token value.

    @retval VOID
**/

VOID
IpmiProtocolErrorMessagePopUp (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Message 
  )
{
    UINT8      MsgBoxSel;
    EFI_STRING CharPtrString;
    EFI_STRING MessagePtrString;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered \n", __FUNCTION__));

    CharPtrString = HiiGetString ( HiiHandle, STRING_TOKEN(STR_PWR_CONTROL_POLICY_INFO), NULL);
    MessagePtrString = HiiGetString ( HiiHandle, Message, NULL);
    if ((CharPtrString != NULL) && (MessagePtrString != NULL)) {
        gPcpPostMgr->DisplayMsgBox (
                        CharPtrString,
                        MessagePtrString,
                        MSGBOX_TYPE_OK,
                        &MsgBoxSel
                        );
        gBS->FreePool (CharPtrString);
        gBS->FreePool (MessagePtrString);
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exited \n", __FUNCTION__));
    return;
}

/**
    This function changes the PowerControl Policy option in Setup
    and Sends command to BMC with selected option in Setup.

    @param HiiHandle   A handle that was previously registered in the
                       HII Database.
    @param Class       Formset Class of the Form Callback Protocol passed in
    @param SubClass    Formset sub Class of the Form Callback Protocol passed in
    @param Key Formset Key of the Form Callback Protocol passed in

    @return EFI_STATUS Return Status

**/
EFI_STATUS
PowerControlPolicyCallbackFunction (
  IN  EFI_HII_HANDLE     HiiHandle,
  IN  UINT16             Class,
  IN  UINT16             SubClass,
  IN  UINT16             Key
  )
{
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration ;
    BOOLEAN                             BrowserDataStatus;
    EFI_STATUS                          Status;
    UINTN                               SelectionBufferSize = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    UINTN                               Size = 0;
    SET_POWER_RESTORE_REQUEST           SetPowerRestore = {0};
    SET_POWER_RESTORE_RESPONSE          SetPowerRestoreResponse = {0};

    CALLBACK_PARAMETERS *CallbackParameters = GetCallbackParameters();


    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered with Key: %x \n", __FUNCTION__, Key));

    //
    // Check for the key and Return if Key value does not match
    //
    if ((Key != PWR_CONTROL_POLICY_KEY)) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR,"\nIPMI: Callback function is called with Wrong Key Value. \
                                              Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }
    //
    // Get the call back parameters and verify the action
    //
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so load the defaults \n"));
        return  EFI_UNSUPPORTED;
    }
    
    if (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    if (gIpmiTransport == NULL) {   // IPMI transport protocol not found.
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "\nIPMI: IPMI transport protocol not found\n"));
        
        //
        // Locate AmiPostMgrProtocol to display the message boxes in setup 
        //
        Status = gBS->LocateProtocol (
                        &gAmiPostManagerProtocolGuid,
                        NULL,
                        (VOID **)&gPcpPostMgr
                        );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI: gAmiPostManagerProtocolGuid locateProtocol Status: %r\n", Status));

        if (EFI_ERROR (Status)) {
            return Status;
        }
        IpmiProtocolErrorMessagePopUp (HiiHandle, STRING_TOKEN(STR_IPMI_PROTOCOL_ERROR));
        return EFI_NOT_FOUND;
    }

    //
    // Get Browser DATA
    //
    BrowserDataStatus = HiiGetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nIPMI: Status of HiiGetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
         return EFI_UNSUPPORTED;
    }

    SetPowerRestore.PowerRestorePolicy = ServerMgmtConfiguration.PowerControlPolicy; // 3: No Change(Unknown) 2: Restore Power, 1: Last Power State, 0: Power OFF
    SetPowerRestore.Reserved = RESERVED;

    //
    // SET Chassis Power policy according to the Setup option.
    //
    Size = sizeof(SET_POWER_RESTORE_RESPONSE);
    Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_CHASSIS,
                                    BMC_LUN,
                                    IPMI_CHASSIS_SET_POWER_RESTORE_POLICY,
                                    (UINT8 *) &SetPowerRestore,
                                    sizeof(SET_POWER_RESTORE_REQUEST),
                                    (UINT8 *) &SetPowerRestoreResponse,
                                    (UINT8 *) &Size
                                    );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Set Power Restore Policy status :: %r ", Status ));
    if (EFI_ERROR (Status)) {
        IpmiProtocolErrorMessagePopUp (HiiHandle, STRING_TOKEN(STR_PWR_CONTROL_POLICY_ERROR));
        return Status;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Power Restore Support = %x, Size = %x\n", SetPowerRestoreResponse.Support, Size));
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "BMC Power Restore Policy Set's to :: %d\n",SetPowerRestore.PowerRestorePolicy));

    //
    // Set Browser DATA
    //
    BrowserDataStatus = HiiSetBrowserData (
                           &gEfiServerMgmtSetupVariableGuid,
                           L"ServerSetup",
                           SelectionBufferSize,
                           (UINT8*)&ServerMgmtConfiguration,
                           NULL);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "\nIPMI: Status of HiiSetBrowserData() = %d\n",BrowserDataStatus));
    if (!BrowserDataStatus) {
        return EFI_UNSUPPORTED;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function initializes the Power Control Policy information 
    related setup option values.

    @param HiiHandle Handle to HII database
    @param Class     Indicates the setup class

    @retval VOID

**/

VOID
InitPowerControlPolicyStrings (
    IN EFI_HII_HANDLE     HiiHandle,
    IN UINT16             Class
  )
{
    EFI_STATUS                          Status;
    UINTN                               Size = 0 ;
    GET_CHASSIS_RESPONSE                GetChassisStatusResponse ={0};
    UINT32                              Attributes;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;

    //
    // Continue only for Server Mgmt setup page
    //
    if (Class != SERVER_MGMT_CLASS_ID) {
        return;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport
                    );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status :: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }

    //
    // Get Server Mgmt Setup Variable
    //
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                         L"ServerSetup",
                         &gEfiServerMgmtSetupVariableGuid,
                         &Attributes,   // Attributes
                         &Size,
                         &ServerMgmtConfiguration
                         );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gRT->GetVariable Status :: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gServerMgmtConfiguration.PowerControlPolicy (SetUp Option Value):: %d \n",ServerMgmtConfiguration.PowerControlPolicy));

    //Get the Value from BMC
    Size = sizeof(GET_CHASSIS_RESPONSE);
    Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_CHASSIS,
                                    BMC_LUN,
                                    IPMI_CHASSIS_GET_STATUS,
                                    NULL,
                                    0,
                                    (UINT8 *) &GetChassisStatusResponse,
                                    (UINT8 *)&Size
                                    );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IPMI_CHASSIS_GET_STATUS->SendIpmiCommand Status :: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Power Restore Policy (BMC Data) :: %x \n", ((GetChassisStatusResponse.CurrentPowerState & MASK_VALUE) >> 5) ));
    
    //If the SetUp Value and BMC value both are same, no need to Update in Setup.
    if (((GetChassisStatusResponse.CurrentPowerState & MASK_VALUE) >> 5) == ServerMgmtConfiguration.PowerControlPolicy) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "Not Changed in Setup, because Setup Option and BMC values both are same\n"));
        return;
    }
    
    //If the BMC Value is not same as to Setup option value, means somebody changed BMC power control policy without using setup.(DOS or Console)
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    ServerMgmtConfiguration.PowerControlPolicy = ((GetChassisStatusResponse.CurrentPowerState & MASK_VALUE) >> 5); // 2: Restore Power, 1: Last Power State, 0: Power OFF
    Status = gRT->SetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        Attributes,   // Attributes,
                        Size,
                        &ServerMgmtConfiguration
                        );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gRT->SetVariable Status :: %r \n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gServerMgmtConfiguration.PowerControlPolicy (Data Modified in SetUp):: %d \n",ServerMgmtConfiguration.PowerControlPolicy));

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return;
}

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
