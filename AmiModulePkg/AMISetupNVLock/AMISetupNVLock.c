//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AMISetupNVLock.c
    This file contains the algorithm to protect the BIOS Configuration
    update during runtime. The variables Setup & AMITSESetup write is
    blocked until the system password provided to Unlock.

**/

#include "AMISetupNVLock.h"
#include <NvLockElink.h>

//This function returns the value of Runtime local variable.
extern BOOLEAN IsNvramRuntime();

static BOOLEAN  gNvWriteProtect = FALSE;
static BOOLEAN  gEventsRegistered = FALSE;
static BOOLEAN  gSmiFlashUpdate = FALSE;
static UINT16   gPasswordRetryCount = 0;
static BOOLEAN  gLockedInRuntime = FALSE;
static BOOLEAN  gSetupNvProtocolValid = FALSE;
static UINT8    NvLockSetupControl=0xFF;

extern HOOK_LOCK_UNLOCK_NV VALIDATE_AND_LOCK_NV_LIST EndOfLockUnlockHook;


SETUPNVLOCK_MAILBOX NvLockMailbox ; //communication between DXE and SMM
EFI_GUID NVLockMailboxVariableGuid = SETUPNVLOCK_MAILBOX_ADDRESS_VARIABLE_GUID;
EFI_GUID gAmiSetupNvControlProtocolGuid = SETUP_NV_CONTROL_PROTOCOL_GUID;

//"ValidateAndLockNvHookList" Has all hook to valiadte
HOOK_LOCK_UNLOCK_NV* ValidateAndLockNvHookList[] = {VALIDATE_AND_LOCK_NV_LIST NULL};
AMI_SETUPNV_CONTROL_PROTOCOL SetupNvControl = {ToggleSmiFlashUpdateFlag};

/**
    This Function Registers the event call back

    @param  *pProtocol         The numeric ID of the protocol for 
                               which the event is to be registered.
    @param  NotifyFunction     The pointer to the event's notification
                               function.
    @param  *pNotifyContext    The pointer to the notification function's 
                               context.
    @param  *pEvent            The pointer to the newly created event.
    @param  **ppRegistration   A pointer to a memory location to receive 
                               the registration value.

    @retval EFI_STATUS         return the status

**/

EFI_STATUS 
RegisterEventCallback (
    EFI_GUID            *pProtocol, 
    EFI_EVENT_NOTIFY	NotifyFunction,
    VOID                *pNotifyContext, 
    EFI_EVENT           *pEvent,
    VOID                **ppRegistration )
{
	EFI_STATUS Status;

    Status = pBS->CreateEvent(EVT_NOTIFY_SIGNAL, 
                              TPL_CALLBACK,
                              NotifyFunction, 
                              pNotifyContext, 
                              pEvent );

    if (EFI_ERROR(Status)) {
        return Status;
    }
     
    return pBS->RegisterProtocolNotify(pProtocol, *pEvent, ppRegistration);
}

/**
    This Function notifies the call back for EndOfDxe and BeforeBoot event 

    @param  Event       
    @param  *Context

    @retval VOID
**/

VOID 
EFIAPI 
AmiSetupNvEndOfDxeAndBeforeBootNotify (
    EFI_EVENT Event, 
    VOID *Context )
{
    EFI_STATUS  Status;
    
    gNvWriteProtect = TRUE;
	
    DEBUG_SETUPNVLOCK((DEBUG_ERROR, "\n[Yogn]AmiSetupNvLock: EndOfDxe or Entering OS. Lock the Variable Access"));
    
    //Set NvLockMailbox variable value with 1
    NvLockMailbox.NvLockState = 1; 
    Status = pRS->SetVariable( L"NvLockMailbox", 
                                  &NVLockMailboxVariableGuid,
                                  EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                  sizeof(NvLockMailbox), 
                                  &NvLockMailbox );
    
    DEBUG_SETUPNVLOCK((DEBUG_ERROR, "\n[Yogn]AmiSetupNvLock: NvLockMailbox Set Status: %r", Status));
    return;
}

/**
    This Function gets called while entering into Setup 

    @param Event
    @param *Context

    @retval VOID
**/

VOID
EFIAPI 
AmiSetupNvEnterSetupNotify (
    EFI_EVENT Event, 
    VOID *Context )
{
    EFI_STATUS RetStatus;

    gNvWriteProtect = FALSE;
    gLockedInRuntime = FALSE;
    
    DEBUG_SETUPNVLOCK((DEBUG_ERROR, "\n[Yogn]AmiSetupNvLock: Entering Setup"));
    
    //Set NvLockMailbox variable value with 0 to unlock the variable Access
    NvLockMailbox.NvLockState = 0; 
    RetStatus = pRS->SetVariable( L"NvLockMailbox", 
                                  &NVLockMailboxVariableGuid,
                                  EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                  sizeof(NvLockMailbox), 
                                  &NvLockMailbox );
    
    DEBUG_SETUPNVLOCK((DEBUG_ERROR, "\nAmiSetupNvLock: Set Status: %r", RetStatus));
    return;
}

/**
    This Function Registers the event for different Boot phases to lock/unlock
    the variable access.

    @param VOID

    @retval EFI_STATUS

**/
EFI_STATUS
RegisterEventsForLockUnlock()
{
    EFI_EVENT           Event;
    EFI_STATUS          Status;
    static EFI_EVENT    EnterSetupEvent = NULL;
    VOID                *SetupRegistration = NULL;
    VOID                *SetupEnterNotifyReg = NULL;
    VOID                *BeforeBootRegistration = (VOID*) NULL;
    
    //Install Callback for Before Boot Guid (Before Booting to any boot option). 
    Status = RegisterEventCallback( &gAmiTseEventBeforeBootGuid, 
                                    AmiSetupNvEndOfDxeAndBeforeBootNotify, 
                                    NULL, 
                                    &Event, 
                                    &BeforeBootRegistration );
    
#if (LOCK_SETVAR_AT_ENDOFDXE == 1)
    {
        VOID *Registration = (VOID*) NULL;
        // Create Callback for End of DXE event.
        Status = pBS->CreateEventEx(EVT_NOTIFY_SIGNAL,
                                    TPL_CALLBACK,
                                    AmiSetupNvEndOfDxeAndBeforeBootNotify,
                                    NULL,
                                    &gEfiDxeSmmReadyToLockProtocolGuid,
                                    &Event );
        Status = pBS->RegisterProtocolNotify(&gEfiDxeSmmReadyToLockProtocolGuid, 
                                             Event, 
                                             &Registration );
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: EndOfDxe CreateEventEx Status: %r", Status));
    }
#endif
    
    //Install Callback for After Boot Guid ( Coming back to BIOS after boot failed). 
    Status = RegisterEventCallback( &gAmiTseEventAfterBootGuid, 
                                       AmiSetupNvEnterSetupNotify, 
                                       NULL, 
                                       &EnterSetupEvent, 
                                       &SetupRegistration );
        
    //Install callback on entering into Setup to Unlock the Variable Access
    Status = RegisterProtocolCallback( &gAmiTseSetupEnterGuid,
                                           AmiSetupNvEnterSetupNotify,
                                           NULL,
                                           &Event,
                                           &SetupEnterNotifyReg);
    return Status;
}

/**
    This Function Interface to lock NVRAM Access 

    @param VOID

    @retval EFI_SUCCESS

**/
EFI_STATUS 
LockNv()
{
    EFI_STATUS RetStatus = EFI_UNSUPPORTED;

    gNvWriteProtect = TRUE;

    // Lock the Variable Access. 
    NvLockMailbox.NvLockState = 1; 
    RetStatus = DxeSetVariable( L"NvLockMailbox", 
                                &NVLockMailboxVariableGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                sizeof(NvLockMailbox), 
                                &NvLockMailbox );
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Access to Nvram Variables Locked Status: %r", RetStatus));
    
    return EFI_SUCCESS;
}

/**
    This Function Interface to unlock NVRAM Variable Access

    @param VOID

    @retval EFI_SUCCESS

**/
EFI_STATUS 
UnlockNv()
{
    EFI_STATUS RetStatus;

    gNvWriteProtect = FALSE;

    // UNlock the Variables Access
    NvLockMailbox.NvLockState = 0; 
    RetStatus = DxeSetVariable( L"NvLockMailbox", 
                                &NVLockMailboxVariableGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                sizeof(NvLockMailbox), 
                                &NvLockMailbox );
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Unlock the Variable Access : Status: %r", RetStatus));
    
    return EFI_SUCCESS;
}

/**
    This Function to call all the hook functions in ValidateAndLockNvHookList

    @param    *VariableName      NV Variable Name
    @param    *VendorGUID        Variable GUID
    @param    Attributes         BS / RT / NV
    @param    DataSize           Size of the Data
    @param    *Data              Variable Data

    @retval EFI_STATUS
**/

EFI_STATUS 
ValidateAndLockNvHook(
    IN CHAR16   *VariableName, 
    IN EFI_GUID *VendorGuid,
    IN UINT32   Attributes, 
    IN UINTN    DataSize, 
    IN VOID     *Data )
{
    UINTN i;
    EFI_STATUS Result = EFI_UNSUPPORTED;

    for (i = 0; ValidateAndLockNvHookList[i] && (Result == EFI_UNSUPPORTED); i++) {
        Result = ValidateAndLockNvHookList[i](VariableName, VendorGuid, Attributes, DataSize, Data);
    }

    return Result;
}

/**
    This Function to check whether the Variable is $SETUPPASSWD or not.
    If $SETUPPASSWD, Validates the password in Variable data and Locks/Unlocks NV

    @param   *VariableName  NV Variable Name
    @param   *VendorGUID    Variable GUID
    @param   Attributes     BS / RT / NV
    @param   DataSize       Size of the Data
    @param   *Data          Variable Data

    @retval  EFI_UNSUPPORTED        If Variable is not $SETUPPASSWD
    @retval  EFI_SUCCESS            If Variable is $SETUPPASSWD and data matched with admin password
    @retval  EFI_ACCESS_DENIED      Password Retry count Expired
    @retval  EFI_NOT_READY          No admin Password Set
    @retval  EFI_SECURITY_VIOLATION If Variable is $SETUPPASSWD and data not matched with admin password
**/

EFI_STATUS 
ValidateAndLockNv (
    CHAR16      *VariableName, 
    EFI_GUID    *VendorGuid, 
    UINT32      Attributes, 
    UINTN       DataSize, 
    VOID        *Data )
{
    UINT32 PasswordState = AMI_PASSWORD_NONE;
    CHAR16 *PassWord = (VOID*)Data;
    EFI_STATUS Status;
    
    if (IsSetupPasswordVariable(VariableName, VendorGuid) != TRUE){
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Not SetupPasswd Variable."));
        return EFI_UNSUPPORTED;
    }
    
    if (gPasswordRetryCount == 3){
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Password Retry count Expired: Variable Access Denied"));
        return EFI_ACCESS_DENIED;
    }
    
    //Invoke the to authentication method PasswordAuthenticate
    PasswordState = AuthenticatePassword(PassWord, DataSize);

    // If the status is AMI_PASSWORD_NONE, the password is not valid ignore the write
    // If the status is AMI_INVALID_PASSWORD, Password verified failed. Lock the variable Access
    // If not, Password verification is success. Allow the Variable Access
    if (PasswordState == AMI_PASSWORD_NONE){
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: No Password Set. Lock the Variable Access"));
        LockNv();
        // If no Password Set in the System 
        Status = EFI_NOT_READY;	
    }
    else if (PasswordState == AMI_INVALID_PASSWORD){
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Password Verification failed."));
        if(gNvWriteProtect == TRUE){
            // Increment the Password retry Count
            gPasswordRetryCount++;
        } else {
            gPasswordRetryCount = 0;
        }
        LockNv();
        //When Invalid password is provided for validation
        Status = EFI_SECURITY_VIOLATION;
    } else {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Password Verification passed: Unlock the Variable Accesss in Nv"));
        //If the status is ADMIN, then update the gNvWriteProtect with FALSE
        UnlockNv();
        Status = EFI_SUCCESS;
        gPasswordRetryCount = 0;
    }

    return Status;
}

/**
    Hook for the Variable Write Method SetVariableHook().
    This method should not allow the Write Calls for Setup / 
    AMITSESetup until the password provided in SETUPPASSWD variable.

    @param  *VariableName   NV Variable Name
    @param  *VendorGUID     Variable GUID
    @param  Attributes      BS / RT / NV
    @param  DataSize        Size of the Data
    @param  *Data           Variable Data

    @retval EFI_SECURITY_VIOLATION   NV Locked, The variable could not be updated
    @retval EFI_UNSUPPORTED          Variable could be updated
**/
EFI_STATUS 	
AmiSetupNvWriteHook(
    CHAR16      *VariableName, 
    EFI_GUID    *VendorGuid, 
    UINT32      Attributes, 
    UINTN       DataSize, 
    VOID        *Data )
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    EFI_STATUS RetStatus = EFI_UNSUPPORTED;
    EFI_GUID SetupPwdGuid = SETUPPASSWD_GUID;
    UINTN VarSize = sizeof(NvLockMailbox);
    
    // Get the variable and check the Nvram Variable Access Protocol Support. 
    // NvLockSetupControl =0xFF, it's getting called 1st time to initialize the variable. 
    if(NvLockSetupControl == 0xFF) {
        SETUP_DATA NvLockSetupVariable = {0};
        UINTN SetupVarSize = sizeof(NvLockSetupVariable);
    
        Status =  DxeGetVariable(L"Setup", 
                             &gAmiSetupVariableGuid, 
                             NULL, 
                             &SetupVarSize, 
                             &NvLockSetupVariable );
        if(!EFI_ERROR(Status)){
            NvLockSetupControl=NvLockSetupVariable.NvLockSetupControl;
        }
    }

    // Setup NvLock is disabled. Allow the Access to all the variables. 
    if(NvLockSetupControl == 0) {
        return EFI_UNSUPPORTED;
    }
    
    // Protect the NvLockMailBox variable in Runtime as this variable is used by this driver alone
    // Not allowed to use it from any other driver  
    if (!Wcscmp(VariableName, L"NvLockMailbox") && 
           (guidcmp(VendorGuid, &NVLockMailboxVariableGuid) == FALSE)){
        //If Variable has EFI_VARIABLE_RUNTIME_ACCESS,Block the access
        if ((Attributes & EFI_VARIABLE_RUNTIME_ACCESS) == EFI_VARIABLE_RUNTIME_ACCESS){
            DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: NvLockMailBox Access Blocked"));
            return EFI_ACCESS_DENIED;
        } else{
            return EFI_UNSUPPORTED ;
        }
    }
    
    // During Dxe phase, initilize the events if it's not yet registered. This is expected to 
    // get called only once.
    if(pSmst == NULL && !gEventsRegistered) {
        DeletePasswordVariable();
        RegisterEventsForLockUnlock();
        gEventsRegistered = TRUE;
    }
    
    // Install NvControlProtocol once if in SMM 
    // Lock the Variable Access if it's Runtime
    if(pSmst != NULL){
        //When entering Runtime, SetVariable for BOOTSERVICE_ACCESS variable will fail. 
        //So use this to identify Runtime entry and enable Lock for Runtime.
        if(FALSE == gLockedInRuntime) {
            if(IsNvramRuntime()== TRUE) {
                DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: RunTime Lock Applied"));
                LockNv();
                gLockedInRuntime = TRUE;
            }   
        }
        
        // Protocol installation called only once. 
        if(FALSE == gSetupNvProtocolValid) {
            RetStatus = InstallSetupNvControlProtocol();
            if(RetStatus == EFI_SUCCESS)
                gSetupNvProtocolValid = TRUE;
        }
    }
    
    // Sync the NvWriteProtect Between SMM and Non SMM Nv variable access code. 
    if((gSmiFlashUpdate == FALSE) && (FALSE == gLockedInRuntime)) {
        // If Variable is present, Control comes in SMM phase first time after ReadyToBoot.
        RetStatus =  DxeGetVariable(L"NvLockMailbox", 
                                    &NVLockMailboxVariableGuid, 
                                    NULL, 
                                    &VarSize, 
                                    &NvLockMailbox );

        if(RetStatus == EFI_SUCCESS ) {
            //Based on Lock state, Lock/Unlock NV
            if(NvLockMailbox.NvLockState == 1) {
                DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Write Protect Enabled"));
                gNvWriteProtect = TRUE;                
            } else {
                DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Write Protect Disabled"));
                gNvWriteProtect = FALSE;
            }
        } else {
            DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: NvLockMailBox Variable Not Found !!: %r", RetStatus));
        }
    }
    
    // Validate the Admin and user password 
    Status = ValidateAndLockNvHook(VariableName, VendorGuid, Attributes, DataSize, Data);
    
    // The input Variable is not Password variable. Handle the Variable Function.
    // 1. Password can be modified in runtime
    // 2. Verify the Protected variable list. Based on the Nv Write Protect Status, return the status to caller
    if (EFI_UNSUPPORTED == Status){
        int Index = 0;
        SETUP_VARIABLE_DATA ProtectedVarList[] = {PROTECTED_VARIABLE_LIST {"", NULL_GUID}};
        int Varcount = sizeof(ProtectedVarList)/sizeof(SETUP_VARIABLE_DATA);
        CHAR8 VarNameinChar8[256] = {0};	
        
        //Set the Setup Password Admin or User Using $SETADMINPASSWD and $SETUSERPASSWD Variables
        if( ((guidcmp(VendorGuid, &SetupPwdGuid) == FALSE) && (Wcscmp((CHAR16 *)VariableName, (CHAR16 *)SET_ADMINPASS_VAR_NAME) == FALSE) )|| \
                ((guidcmp(VendorGuid, &SetupPwdGuid) == FALSE) && (Wcscmp((CHAR16 *)VariableName, (CHAR16 *)SET_USERPASS_VAR_NAME) == FALSE)) ) {
              Status = ValidateAndSetSetupPwd(VariableName, VendorGuid, Attributes, DataSize, Data);
              DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Set the Admin/User Password Status: %r\n", Status));
              return Status;
        }
        
        UnicodeStrToAsciiStr(VariableName,VarNameinChar8);
        

        // Check if the variable is present in the protected List. If the variable is present Protected List
        // Based on the NvWriteProtect, allow the access to the variable. For other variables, access it allowed. 
        for(Index = 0; Index < Varcount; Index++) {
            
            if((Strcmp(VarNameinChar8, (ProtectedVarList[Index].VariableName)) == 0) && \
                    (guidcmp(VendorGuid, &(ProtectedVarList[Index].VariableGuid)) == 0)) {
                
                DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: AmiSetupNvWriteHook: Security Violation Check for: Var: %a, %x, %x", 
                        VarNameinChar8, gNvWriteProtect, gSmiFlashUpdate));
                
                //If the Write is Protected enabled for the NV report EFI_SECURITY_VIOLATION
                if ((gNvWriteProtect == TRUE) && (gSmiFlashUpdate == FALSE)) {
                    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: AmiSetupNvWriteHook: Security Violation for the Variable"));
                    Status = EFI_SECURITY_VIOLATION;
                }
                break;
            }
        }
    }
    if(Status == EFI_UNSUPPORTED)
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Variable Access allowed. \n"));
    return Status;
}

/**
    Function to update the global flag to check for flash update

    @param  Start   TRUE or FALSE

    @retval EFI_STATUS
**/

EFI_STATUS 
ToggleSmiFlashUpdateFlag (
    BOOLEAN Start)
{
    gSmiFlashUpdate = !Start;
    DEBUG_SETUPNVLOCK((DEBUG_ERROR, "\n[Yogn]AmiSetupNvLock: Setting gSmiFlashUpdate to %d", gSmiFlashUpdate));
    return EFI_SUCCESS;
}

/**
    Function to install the NVControl protocol

    @param  VOID

    @retval EFI_STATUS
**/
EFI_STATUS 
InstallSetupNvControlProtocol()
{
    EFI_STATUS status = EFI_NOT_READY;

    if (pSmst != NULL) {
        status = pSmst->SmmInstallConfigurationTable( pSmst, 
                                                      &gAmiSetupNvControlProtocolGuid, 
                                                      &SetupNvControl, 
                                                      sizeof(SetupNvControl) );
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Install Setup Nv Protocol Status: %r", status));
    }
    return status;
}

/**
    Function to set the Setup Password (User or Admin)

    @param  *VariableName   NV Variable Name
    @param  *VendorGUID     Variable GUID
    @param  Attributes      BS / RT / NV
    @param  DataSize        Size of the Data
    @param  *Data           Variable Data


    @retval EFI_STATUS
**/

EFI_STATUS 
ValidateAndSetSetupPwd (
    CHAR16      *VariableName, 
    EFI_GUID    *VendorGuid, 
    UINT32      Attributes, 
    UINTN       DataSize, 
    VOID        *Data )
{
    EFI_STATUS Status = EFI_UNSUPPORTED;
    CHAR16 *PassWord = (VOID*)Data;

    if (FALSE == gNvWriteProtect){
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: ValidateAndSetSetupPwd Received Password Length: %d", DataSize));
        
#if (SETUP_STORE_KEYCODE_PASSWORD == 0)
        //For Unicode Password,exclude the size of NULL for PasswordSize
        if ( ( DataSize >= 2 ) && ( L'\0' == PassWord[(DataSize/2) -1] ) ) {
            DataSize = DataSize - 2;
            DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: ValidateAndSetSetupPwd Modified Password Length: %d", DataSize));
        }
#endif
        
        //Password length should be minimum PASSWORD_MIN_SIZE chars and 
        //maximum is TSE_PASSWORD_LENGTH
        if ((DataSize < (PASSWORD_MIN_SIZE * sizeof(CHAR16)) ) || \
             (DataSize > (TSE_PASSWORD_LENGTH * sizeof(CHAR16)) )) { 
            DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: ValidateAndSetSetupPwd Invalid Length: %d", DataSize));
            Status = EFI_INVALID_PARAMETER;
        } else {
            Status = SetPassword(VariableName, Data, DataSize);
            DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: ValidateAndSetSetupPwd: SetPassword Status: %r",Status));
        }
    } else {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: ValidateAndSetSetupPwd SetPassword: Nv write protected"));
        Status = EFI_SECURITY_VIOLATION;
    }

    return Status;
}
