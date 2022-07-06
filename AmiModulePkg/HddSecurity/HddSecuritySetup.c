//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file HddSecuritySetup.c
    HDD Security Setup Routines
    Use this hook to dynamically change HDD Security Driver Setup screen Strings.

**/

//---------------------------------------------------------------------------
#include <Token.h>
#include <AmiDxeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Setup.h>
#include <Protocol/PciIo.h>
#include <Protocol/BlockIo.h>
#include <HddSecurityCommon.h>
#include <Protocol/HiiString.h>
#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0
#include <HddSecuritySetupVar.h>
// {2FF61C92-DB91-431d-94F9-8992C3615F48}
EFI_GUID  gHddSecurityLoadSetupDefaultsGuid = AMI_HDD_SECURITY_LOAD_SETUP_DEFAULTS_GUID;
EFI_EVENT gHddSecurityLoadSetupDefaultsEvent = NULL;
#endif

CHAR16*
Wcscpy(
        CHAR16 *,
        CHAR16 *
);

int
Wcscmp(
        CHAR16 *string1,
        CHAR16 *string2
);

VOID
RefreshHddSecuritySetupData(
        VOID
);

VOID 
EFIAPI
AmiTseHddNotifyInitString(
        IN EFI_EVENT Event, 
        IN VOID *Context
);

HDD_SECURITY_INTERNAL_DATA     *HddSecurityInternalData = NULL;
UINT16                         gHddSecurityCount = 0;
UINT16                         gValidHddSecurityCount;
static VOID                    *gHddSecEscNotifyRegistration;
static EFI_EVENT               gHddSecEscNotifyEvent;
static BOOLEAN                 gHddSecIsEscPressed = FALSE;
#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0
HDD_SECURITY_CONFIG            *gHddSecurityConfig = NULL;
#endif
static EFI_HII_STRING_PROTOCOL *HiiString      = NULL;

static EFI_GUID   EfiVariableGuid      = EFI_GLOBAL_VARIABLE;
EFI_GUID   gAmiHddSecurityProtocolGuid = AMI_HDD_SECURITY_PROTOCOL_GUID;
#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0
EFI_GUID   gHddSecurityConfigGuid      = HDD_SECURITY_CONFIG_GUID;
#endif
static EFI_GUID gHddSecEscOnPwdPromptGuid = AMITSE_ESC_ON_PWD_PROMPT_GUID;

EFI_HII_HANDLE HddSecurityHiiHandle = 0; 

// Store the names of HDDs present in the system. Dynamically updated.
STRING_REF gHddSecurityName[6] =
{
    STRING_TOKEN(STR_SECURITY_HDD_ONE), STRING_TOKEN(STR_SECURITY_HDD_TWO),
    STRING_TOKEN(STR_SECURITY_HDD_THREE), STRING_TOKEN(STR_SECURITY_HDD_FOUR),
    STRING_TOKEN(STR_SECURITY_HDD_FIVE), STRING_TOKEN(STR_SECURITY_HDD_SIX)
};

#if !(defined (MDE_CPU_IA32) || defined (MDE_CPU_X64))
/**
  Local function defined for the Build except X64, IA32 Architecture
  i.e. GetRandomNumber64() function defined in "RngLib" will be used for X64 and IA32 supported projects.

  if Rand is NULL, then ASSERT().

  @param[out] Rand     Buffer pointer to store the 64-bit random value.

  @retval TRUE         Random number generated successfully.
  @retval FALSE        Failed to generate the random number.

**/
static
BOOLEAN
EFIAPI
HddSecGetRandomNumber64 (
  OUT     UINT64                    *Rand
)
{
    if(!Rand) {
       ASSERT(FALSE);
       return FALSE;
    }
    
    *Rand = 0;
    return TRUE;
}
#define GET_RANDOM_NUMBER_64(Arg) HddSecGetRandomNumber64(Arg)
#else
BOOLEAN
EFIAPI
GetRandomNumber64 (
  OUT     UINT64    *Rand
);
#define GET_RANDOM_NUMBER_64(Arg) GetRandomNumber64(Arg)
#endif

// Function Definitions

#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0

#if defined(SETUP_PASSWORD_NON_CASE_SENSITIVE) && SETUP_PASSWORD_NON_CASE_SENSITIVE
/** @internal
    Function to update the given password to non case sensitive

    @param *Password     Pointer to Passsword for data security.
    @param PwdLength     Length of the password.

    @return VOID

    @endinternal
**/
static
VOID
UpdatePasswordToNonCaseSensitive(
        CHAR16 *Password,
        UINTN PwdLength
)
{
    UINTN Idx;
    for ( Idx = 0; Idx < PwdLength; Idx++ ) {
        Password[Idx] = ((Password[Idx]>=L'a')&&(Password[Idx]<=L'z'))?(Password[Idx]+L'A'-L'a'):Password[Idx];
    }
}
#endif

/** 
    Notification function for "Load Default" action in BIOS setup. This function
    will re-initialize the HDD security setup data.

    @param Event       Event to signal
    @param Context     Event specific context

    @return VOID

**/
VOID
EFIAPI
HddSecurityLoadSetupDefaults (
        IN EFI_EVENT Event,
        IN VOID *Context
)
{
    RefreshHddSecuritySetupData();
}

/** @internal
    Return the Security Status Information

    @param *HddSecurityProtocol    Pointer to AMI_HDD_SECURITY_PROTOCOL
    @param *ReqStatus              Pointer to Mask security status 
    @param Mask                    Flags for security.
    
    @retval TRUE        Security Status returned successfully.
    @retval FALSE       Unable to get security status

    @endinternal
**/
BOOLEAN
CheckSecurityStatus (
    IN  AMI_HDD_SECURITY_PROTOCOL   *HddSecurityProtocol,
    IN  OUT BOOLEAN                 *ReqStatus,
    IN  UINT16                      Mask
)
{
    UINT16      SecurityStatus = 0;
    EFI_STATUS  Status;

    //
    //get the security status of the device
    //
    Status = HddSecurityProtocol->ReturnSecurityStatus( HddSecurityProtocol, &SecurityStatus );

    if ( EFI_ERROR( Status ))
        return FALSE; // Unable to get security status
    // Mask security status based on supplied mask data
    *ReqStatus = (BOOLEAN)((SecurityStatus & Mask) ? TRUE : FALSE );
    return TRUE;
}
#endif

#if (!defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0) || \
              (defined(SECURITY_SETUP_ON_SAME_PAGE) && SECURITY_SETUP_ON_SAME_PAGE == 1)
/** @internal
    Return the String based on StringId

    @param HiiHandle       Handle to HII database
    @param StringId        The string's id, which is unique within PackageList.
                                 

    @return EFI_STRING      String is returned based on StringID.
    @return NULL            No String is returned.

    @endinternal
**/
EFI_STRING
HddHiiGetString (
    IN  EFI_HII_HANDLE  HiiHandle,
    IN  EFI_STRING_ID   StringId
)
{
    EFI_STRING String = NULL;
    UINTN StringSize = 0;
    EFI_STATUS Status;
    CHAR8* PlatformLang = NULL;
    UINTN Size = 0;

    if(HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status))
            return NULL;
    }

    //language not provided - try platform language
    Status = GetEfiVariable(L"PlatformLang", &EfiVariableGuid, NULL, &Size, (VOID**)&PlatformLang);
    if(!EFI_ERROR(Status)) {
        Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, StringSize, (VOID**)&String);
            if(!EFI_ERROR(Status))
                Status = HiiString->GetString(HiiString, PlatformLang, HiiHandle, StringId, String, &StringSize, NULL);
        }
        pBS->FreePool(PlatformLang);
    }
    return (EFI_ERROR(Status)) ? NULL : String;
}

/** @internal
    Locate the Security Protocols and return the information

    @param VOID

    @return UINT16       Count of Valid HDD Security 

    @endinternal
**/
UINT16
InitHddSecurityInternalDataPtr(
    VOID
)
{
    EFI_STATUS                      Status;
    AMI_HDD_SECURITY_INTERFACE      *Security = NULL;
    UINTN                           i;
    VOID                            *TempPtr;

    Status = pBS->LocateProtocol(
                    &gAmiHddSecurityProtocolGuid,
                    NULL,
                    (VOID**) &Security);

    if ( !EFI_ERROR( Status )) {
        gHddSecurityCount = *((UINT16*)Security->PostHddCount);
        HddSecurityInternalData = *((HDD_SECURITY_INTERNAL_DATA**)Security->PostHddData);

        gValidHddSecurityCount = gHddSecurityCount;

        for(i=0;i<gHddSecurityCount;i++) {

            if( (HddSecurityInternalData+i) != NULL ) {
                Status = pBS->HandleProtocol(
                                HddSecurityInternalData[i].DeviceHandle,
                                &gAmiHddSecurityProtocolGuid,
                                &TempPtr
                                );

                if( EFI_ERROR(Status) ) {
                    TempPtr = 0;
                    gValidHddSecurityCount--;
                }
            }
        }
		
        return gValidHddSecurityCount;
    }

    return 0;
}

/** @internal
    Get the HDD security internal Data pointer

    @param Index     Index of HDD

    @return VOID

    @endinternal
**/
VOID*
GetHddSecurityInternalDataPtr (
    UINTN   Index
)
{
    HDD_SECURITY_INTERNAL_DATA *DataPtr;

    if ( gValidHddSecurityCount==0 || gHddSecurityCount == 0 || HddSecurityInternalData == NULL ) {
        //
        //try to initialize, if not initialized
        //
        gValidHddSecurityCount = InitHddSecurityInternalDataPtr( );
        DEBUG((DEBUG_VERBOSE,"HddSecurity: Validated Hdd Count = %d", gValidHddSecurityCount));
    }
    if ( gValidHddSecurityCount == 0 || HddSecurityInternalData == NULL || Index >=
         gHddSecurityCount ) {
        return NULL;
    }
    DataPtr = (HDD_SECURITY_INTERNAL_DATA*)HddSecurityInternalData;

    return (VOID*)&DataPtr[Index];
}
#endif

/** @internal
    This function initializes the HddSecurity setup option values or registers
    an event that will later initialize security strings.

    @param  HiiHandle   Handle to HII database
    @param  Class       Indicates the setup class

    @retval VOID

    @endinternal
**/

VOID
InitHddSecurityStrings (
    EFI_HII_HANDLE  HiiHandle,
    UINT16          Class
)
{
    EFI_STATUS            Status;
    UINTN                 BufferSize = sizeof(UINTN);
    VOID                  *NotifyRegistration;
    EFI_EVENT             NotifyEvent;
    EFI_HANDLE            TseHddNotify;
    EFI_GUID              AmiTseHddNotifyGuid = AMI_TSE_HDD_NOTIFY_GUID;
#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0
    HDD_SECURITY_CONFIG   HddSecurityConfig;
    
    // Initialize HDDSecConfig variable
    pBS->SetMem(&HddSecurityConfig, sizeof(HDD_SECURITY_CONFIG), 0);
    Status = pRS->SetVariable(L"HDDSecConfig",
                              &gHddSecurityConfigGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS,
                              sizeof(HDD_SECURITY_CONFIG),
                              &HddSecurityConfig);
    ASSERT_EFI_ERROR(Status);
#endif
    
    HddSecurityHiiHandle = HiiHandle; // Store HII handle needed for SETUP items
    
    Status = pBS->LocateHandle ( ByProtocol,
                                 &AmiTseHddNotifyGuid,
                                 NULL,
                                 &BufferSize,
                                 &TseHddNotify );
    
    if (Status == EFI_SUCCESS) {
        //If HDD post data already initialized then Init Setup Strings here.
        AmiTseHddNotifyInitString (NULL, NULL);        
    } else if (Status == EFI_NOT_FOUND) {
        //Else register notify for AmiTseHddNotifyGuid
        Status = pBS->CreateEvent(  EFI_EVENT_NOTIFY_SIGNAL,
                                    TPL_CALLBACK,
                                    AmiTseHddNotifyInitString,
                                    &NotifyRegistration,
                                    &NotifyEvent );
        if(EFI_ERROR(Status)) {
           return;
        }
        
        Status = pBS->RegisterProtocolNotify( &AmiTseHddNotifyGuid,
                                              NotifyEvent,
                                              &NotifyRegistration );
        ASSERT_EFI_ERROR(Status);
    }
}

/**
    This Event notifies String initialization for HddSecurity for  can be processed
    
    @param  Event                 Event whose notification function is being invoked.
    @param  Context               The pointer to the notification function's context,
                                  which is implementation-dependent.

    @retval VOID

**/
VOID 
EFIAPI
AmiTseHddNotifyInitString(
    IN EFI_EVENT Event, 
    IN VOID *Context
) 
{
#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0
    EFI_STATUS                 Status;
    UINTN                      i;
    CHAR16                     *Name;
    HDD_SECURITY_INTERNAL_DATA *DataPtr;
    UINT16                     HddSecurityCount;
    
    // Initialize HDD security internal data structure and gHddSecurityCount
    GetHddSecurityInternalDataPtr(0);
    // Allocate memory for setup data
    if(NULL == gHddSecurityConfig) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(HDD_SECURITY_CONFIG),
                                    (VOID**)&gHddSecurityConfig);
        if(EFI_ERROR(Status) || NULL == gHddSecurityConfig) {
            return;
        }
    }

    // Update HDD counts in setup data so that setup data for only connected HDDs will be visible
    gHddSecurityConfig->Count = gValidHddSecurityCount;
    HddSecurityCount = gValidHddSecurityCount;

    // Set the new setup configuration data
    Status = pRS->SetVariable(L"HDDSecConfig",
                                &gHddSecurityConfigGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                sizeof(HDD_SECURITY_CONFIG),
                                gHddSecurityConfig);
    ASSERT_EFI_ERROR(Status);

    // Update HDD Names
    for(i = 0; (i<gHddSecurityCount) && HddSecurityCount ; i++) {
        DataPtr = GetHddSecurityInternalDataPtr(i);
        if (DataPtr == NULL) continue;
        Name = HddHiiGetString( DataPtr->PostHiiHandle, DataPtr->PromptToken);
        InitString(HddSecurityHiiHandle, gHddSecurityName[i], L"%s", Name);
        HddSecurityCount--;
    }

    if(gHddSecurityLoadSetupDefaultsEvent == NULL) {
      Status = pBS->CreateEventEx(
                     EVT_NOTIFY_SIGNAL,
                     TPL_CALLBACK,
                     HddSecurityLoadSetupDefaults,
                     NULL,
                     &gHddSecurityLoadSetupDefaultsGuid,
                     &gHddSecurityLoadSetupDefaultsEvent);
      ASSERT_EFI_ERROR(Status);
    }
#else
#if defined(SECURITY_SETUP_ON_SAME_PAGE) && SECURITY_SETUP_ON_SAME_PAGE
    UINTN                      i;
    CHAR16                     *Name;
    HDD_SECURITY_INTERNAL_DATA *DataPtr;
    UINT16                     HddSecurityCount = gValidHddSecurityCount;

    // Initialize HDD security internal data structure and gHddSecurityCount
    GetHddSecurityInternalDataPtr(0);

    // Update HDD Names
    for(i = 0; (i<gHddSecurityCount) && HddSecurityCount; i++) {
        DataPtr = GetHddSecurityInternalDataPtr(i);
        if (DataPtr == NULL) continue;
        Name = HddHiiGetString( DataPtr->PostHiiHandle, DataPtr->PromptToken);
        InitString(HddSecurityHiiHandle, gHddSecurityName[i], L"%s", Name);
        HddSecurityCount--;
    }
#endif
#endif
    return;
}

#if !defined(SETUP_IDE_SECURITY_SUPPORT) || SETUP_IDE_SECURITY_SUPPORT == 0

/** @internal
    Initializes the structure HDD_SECURITY_CONFIG for the current
    HDD if the data pointer to the structure HDD_SECURITY_INTERNAL_DATA is
    initialized already.

    @param *TempHddSecurityConfig    Pointer to HDD_SECURITY_CONFIG
    @param  value                    Number of HDD connected.

    @return VOID

    @endinternal
**/
VOID
UpdateSetupDataByHddIndex(
    VOID    *TempHddSecurityConfig,
    UINTN   value
)
{
    HDD_SECURITY_INTERNAL_DATA    *DataPtr             = NULL;
    AMI_HDD_SECURITY_PROTOCOL     *HddSecurityProtocol = NULL;
    BOOLEAN                       Status;
    UINT32                        IdePasswordFlags = 0;
    EFI_STATUS                    ReturnStatus;
    HDD_SECURITY_CONFIG           *HddSecurityConfig
        = (HDD_SECURITY_CONFIG*)TempHddSecurityConfig;


    //
    //Set current HDD security page
    //
    DataPtr = (HDD_SECURITY_INTERNAL_DATA*)GetHddSecurityInternalDataPtr( value );
    
    if (DataPtr == NULL) return;

    if ( DataPtr ) {
        HddSecurityProtocol = DataPtr->HddSecurityProtocol;

        CheckSecurityStatus(
            HddSecurityProtocol,
            &Status,
            SecuritySupportedMask );
        HddSecurityConfig->Supported[value] = Status ? 1 : 0;
        CheckSecurityStatus(
            HddSecurityProtocol,
            &Status,
            SecurityEnabledMask );
        HddSecurityConfig->Enabled[value] = Status ? 1 : 0;
        CheckSecurityStatus(
            HddSecurityProtocol,
            &Status,
            SecurityLockedMask );
        HddSecurityConfig->Locked[value] = Status ? 1 : 0;
        CheckSecurityStatus(
            HddSecurityProtocol,
            &Status,
            SecurityFrozenMask );
        HddSecurityConfig->Frozen[value] = Status ? 1 : 0;
        ReturnStatus         = HddSecurityProtocol->ReturnIdePasswordFlags(
            HddSecurityProtocol,
            &IdePasswordFlags );

        if ( EFI_ERROR( ReturnStatus )) {
            return;
        }

        HddSecurityConfig->UserPasswordStatus[value]
            = (IdePasswordFlags & 0x00020000) ? 1 : 0;
        HddSecurityConfig->MasterPasswordStatus[value]
            = (IdePasswordFlags & 0x00010000) ? 1 : 0;

        HddSecurityConfig->ShowMaster[value] = 0x0000;

        if ( HddSecurityConfig->Locked[value] && HddSecurityConfig->MasterPasswordStatus[value] ) {
            HddSecurityConfig->ShowMaster[value] = 0x0001;
        } else if ( (DataPtr->LoggedInAsMaster)) {
            HddSecurityConfig->ShowMaster[value] = 0x0001;
        } else if ( !(HddSecurityConfig->UserPasswordStatus[value])) {
            HddSecurityConfig->ShowMaster[value] = 0x0001;
        }
        
    }// end if
    return;
}

/** @internal
    Initializes the structure HDD_SECURITY_CONFIG for all the
    HDDs present if the data pointer to the structure
    HDD_SECURITY_INTERNAL_DATA is initialized already.

    @param *HddSecurityConfig     Pointer to HDD_SECURITY_CONFIG
    @param value                  Number of HDD connected.

    @return VOID
    
    @endinternal
**/
VOID
UpdateSetupDataForAllHdd(
    VOID    *HddSecurityConfig,
    UINTN   value
)
{
    UINTN i;

    for ( i = 0; i < gHddSecurityCount; i++ ) {
        UpdateSetupDataByHddIndex( HddSecurityConfig, i);
    }
    return;
}

/** @internal
    Hook function to Initialize the structure HDD_SECURITY_CONFIG
    for the HDDs based on the token ALL_HDD_SAME_PW.

    @param *HddSecurityConfig    Pointer to HDD_SECURITY_CONFIG
    @param  value                Number of HDD connected.

    @return VOID

    @endinternal
**/
VOID
UpdateHddSecuritySetupData(
    VOID    *HddSecurityConfig,
    UINTN   value
)
{
    #if ALL_HDD_SAME_PW
     UpdateSetupDataForAllHdd( HddSecurityConfig, value);
    #else
     UpdateSetupDataByHddIndex( HddSecurityConfig, value);
    #endif

}

/** @internal
    Function to update the setup configuration page after HDD password update

    @param VOID

    @return VOID
    
    @note
        1.If the password is set, the security enabled option will be enabled in setup.
        1.If the password is cleared, the security enabled option will be disabled in setup.
    @endinternal
**/
VOID
RefreshHddSecuritySetupData (
        VOID
)
{
    EFI_STATUS          Status;
    UINT8               HardDiskNumber = 0xFF;

    // Allocate memory for setup configuration data
    if(NULL == gHddSecurityConfig) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(HDD_SECURITY_CONFIG),
                                    (VOID**)&gHddSecurityConfig);
        if(EFI_ERROR(Status) || NULL == gHddSecurityConfig) {
            return;
        }
    }

    gHddSecurityConfig->Count =  InitHddSecurityInternalDataPtr( );

    // Update setup data for all HDDs
    for( HardDiskNumber = 0 ; HardDiskNumber < gHddSecurityCount ; HardDiskNumber++ )
    {
        UpdateHddSecuritySetupData( (VOID*)gHddSecurityConfig, HardDiskNumber );
    }

    // Set the new setup configuration data
    Status = pRS->SetVariable(L"HDDSecConfig",
                    &gHddSecurityConfigGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof(HDD_SECURITY_CONFIG),
                    gHddSecurityConfig);
    
    ASSERT_EFI_ERROR(Status);
    

    // Refresh browser data
    HiiLibSetBrowserData( sizeof(HDD_SECURITY_CONFIG),
                        gHddSecurityConfig,
                        &gHddSecurityConfigGuid,
                        L"HDDSecConfig");

}

/** @internal
    Validates the password for the current HDD alone.

    @param *Password      Pointer to Password for security.
    @param *Ptr           Pointer to HDD_SECURITY_INTERNAL_DATA
    @param bCheckUser     Check for user Password/Master Password: 0/1

    @retval EFI_SUCCESS         Password for current HDD is validated successfully.
    @retval EFI_ACCESS_DENIED   Access for Password validation is denied.

    @endinternal
**/
EFI_STATUS
HddPasswordAuthenticateHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    UINT16            Control = 0;
    EFI_STATUS        Status;
    UINT8             Buffer[IDE_PASSWORD_LENGTH + 1];

    #if !SETUP_SAME_SYS_HDD_PW
    UINTN             i;
    #endif

    HDD_SECURITY_INTERNAL_DATA * DataPtr = (HDD_SECURITY_INTERNAL_DATA*)Ptr;

#if defined(SETUP_PASSWORD_NON_CASE_SENSITIVE) && SETUP_PASSWORD_NON_CASE_SENSITIVE
    UpdatePasswordToNonCaseSensitive(Password, Wcslen(Password));
#endif

    MemSet( &Buffer, IDE_PASSWORD_LENGTH + 1, 0 );

    #if !SETUP_SAME_SYS_HDD_PW

    for ( i = 0; i < IDE_PASSWORD_LENGTH + 1; i++ ) {
        Buffer[i] = (UINT8)Password[i];

        if ( Password[i] == L'\0' ) {
            break;
        }
    }
    #else
    pBS->CopyMem( Buffer, Password, IDE_PASSWORD_LENGTH + 1 );
    #endif

    Control = bCheckUser ? 0 : 1;

    Status = (DataPtr->HddSecurityProtocol)->SecurityUnlockPassword(
        DataPtr->HddSecurityProtocol,
        Control,
        Buffer );

    if ( EFI_ERROR( Status )) {
        return EFI_ACCESS_DENIED;
    }

    //
    //save password in case we need to disable it during the setup
    //
    pBS->CopyMem( &(DataPtr->PWD), &Buffer, IDE_PASSWORD_LENGTH + 1 );
//    DataPtr->Locked = FALSE;

    if ( !bCheckUser ) {
        DataPtr->LoggedInAsMaster = TRUE;
    }

    return EFI_SUCCESS;
}

/** @internal
    Validates the password for all the HDDs Present.

    @param *Password        Pointer to Password for security
    @param *Ptr             Index of HDD
    @param bCheckUser       Check for user/Master Password: 0/1

    @return EFI_SUCCESS      Password validation is success.
    @return EFI_NOT_FOUND    Security internal data is not available.

    @endinternal
**/
EFI_STATUS
HddPasswordAuthenticateAllHdd (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    HDD_SECURITY_INTERNAL_DATA *DataPtr;
    UINTN i;
    EFI_STATUS        Status=EFI_NOT_FOUND;

        //
        //For all drives
        //
        DataPtr = HddSecurityInternalData;

        if(DataPtr == NULL) {
            return EFI_NOT_FOUND;
        }

        for ( i = 0; i < gHddSecurityCount; i++ ) {

            Status = HddPasswordAuthenticateHdd( Password,
                                              DataPtr,
                                              bCheckUser );
            if ( EFI_SUCCESS != Status ) {
                //
                // Unlock failed.
                //
                LibReportStatusCode( EFI_ERROR_CODE | EFI_ERROR_MAJOR,
                                        DXE_INVALID_IDE_PASSWORD,
                                        0,
                                        NULL,
                                        NULL );
            }
            DataPtr++;
        }
    return Status;
}

/** @internal
    Hook function to validate Password for the HDDs based on
    the token ALL_HDD_SAME_PW

    @param *Password        Pointer to Password for security.
    @param *Ptr             Index of HDD
    @param bCheckUser       Check for user Password/Master Password: 0/1

    @return EFI_SUCCESS         Password for current HDD is validated successfully.
    @return EFI_ACCESS_DENIED   Access for Password validation is denied.

    @endinternal
**/
EFI_STATUS
HddPasswordAuthenticate (
    CHAR16  *Password,
    VOID    * Ptr,
    BOOLEAN bCheckUser
)
{
    #if ALL_HDD_SAME_PW
     return HddPasswordAuthenticateAllHdd( Password, Ptr, bCheckUser);
    #else
     return HddPasswordAuthenticateHdd( Password, Ptr, bCheckUser);
    #endif
}

/** @internal
    Updates the password for the current HDD alone.

    @param Index            Number of Hard Disk connected.
    @param *Password        Pointer to Password for security.
    @param bCheckUser       Check for user Password/Master Password: 0/1

    @retval TRUE       Password update is success.
    @retval FALSE      Password not updated.

    @endinternal
**/
BOOLEAN
HddPasswordUpdateHdd (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    AMI_HDD_SECURITY_PROTOCOL   *HddSecurityProtocol = NULL;
    UINT16                      Control              = bCheckUser ? 0 : 1;
    UINT8                       RevisionCode         = 0;
    BOOLEAN                     Locked, Enabled;
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    UINT8                       Buffer[IDE_PASSWORD_LENGTH + 1];
    #if !SETUP_SAME_SYS_HDD_PW
    UINTN                       ii;
    #endif
    HDD_SECURITY_INTERNAL_DATA  *DataPtr;

#if defined(SETUP_PASSWORD_NON_CASE_SENSITIVE) && SETUP_PASSWORD_NON_CASE_SENSITIVE
    UpdatePasswordToNonCaseSensitive(Password, Wcslen(Password));
#endif

    DataPtr = (HDD_SECURITY_INTERNAL_DATA*)GetHddSecurityInternalDataPtr( Index );

    if ( DataPtr == NULL ) {
        return FALSE;
    }
    HddSecurityProtocol = DataPtr->HddSecurityProtocol;

    //
    //get the status of the device
    //
    if ( !(
                CheckSecurityStatus(
                                     HddSecurityProtocol, &Locked,
                                     SecurityLockedMask )
             && CheckSecurityStatus( HddSecurityProtocol, &Enabled,
                                     SecurityEnabledMask ))) {
        return FALSE;
    }

    if ( !Locked ) {
        if ( Password[0] == 0 ) {
            //
            //empty string is entered -> disable password
            //
            Status = HddSecurityProtocol->SecurityDisablePassword(
                HddSecurityProtocol,
                Control,
                HddSecurityInternalData[Index].PWD );
        } else {
            //
            //set new password
            //
            MemSet( &Buffer, IDE_PASSWORD_LENGTH + 1, 0 );
            #if !SETUP_SAME_SYS_HDD_PW

            for ( ii = 0; ii < IDE_PASSWORD_LENGTH + 1; ii++ ) {
                Buffer[ii] = (UINT8)Password[ii];

                if ( Password[ii] == L'\0' ) {
                    break;
                }
            }// end of for
            #else
            pBS->CopyMem( Buffer, Password, IDE_PASSWORD_LENGTH + 1 );
            #endif

            Status = HddSecurityProtocol->SecuritySetPassword(
                HddSecurityProtocol,
                Control,
                Buffer,
                RevisionCode );
        }
    }// end if(!Locked)

    if ( EFI_ERROR( Status )) {
        return FALSE;
    }

    return TRUE;
}

/** @internal
    Updates the HDD password for all the HDDs present.

    @param Index            Number of Hard Disk connected.
    @param *Password        Pointer to Password for security.
    @param bCheckUser       Check for user Password/Master Password: 0/1

    @return TRUE        Password update is success.
    @return FALSE       Password not updated.

    @endinternal
**/
BOOLEAN
HddPasswordUpdateAllHdd (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    UINTN       i;
    BOOLEAN     Status = FALSE;
        for ( i = 0; i < gHddSecurityCount; i++ ) {
            Status = HddPasswordUpdateHdd( (UINT32)i, Password, bCheckUser);
        }
    return Status;
}

/** @internal
    Hook function to update the password for the HDDs based
    on the token ALL_HDD_SAME_PW.

    @param Index           Number of Hard Disk connected.
    @param *Password       Pointer to Password for security.
    @param bCheckUser      Check for user Password/Master Password: 0/1

    @return TRUE        Password update is success.
    @return FALSE       Password not updated.
    
    @endinternal
**/
BOOLEAN
HddPasswordUpdate (
    UINT32      Index,
    CHAR16      *Password,
    BOOLEAN     bCheckUser
)
{
    #if ALL_HDD_SAME_PW
     return HddPasswordUpdateAllHdd( Index, Password, bCheckUser);
    #else
     return HddPasswordUpdateHdd( Index, Password, bCheckUser);
    #endif
}

/**
    Notification function for "Esc pressed" action in BIOS setup.

    @param EFI_EVENT Event - Event to signal
    @param VOID* Context - Event specific context

    @retval VOID

**/
STATIC
VOID 
EFIAPI
NotifyEscPressed(
    IN EFI_EVENT Event, 
    IN VOID *Context
)
{
    gHddSecIsEscPressed = TRUE;
    return;
}

/** @internal
    Fills the password buffer with the random number
    generated using GetRandomNumber64()

    @param  Buffer       Pointer to Password buffer.
    @param  size         Size of the buffer.

    @return VOID
    
    @endinternal
**/
STATIC
VOID
FillRandNoInPwdBuffer(
    UINT64 *Buffer, 
    UINT8  size
)
{
    UINT64     RandNo;
    UINT8      i;
    UINT8      *TempBuffer;
    UINT8      Remainder = size % 8;

    if(Buffer == NULL || size == 0)
        return;
    // Fill the buffer in multiples of eight as
    // we are getting eight byte random number
    for(i= 0; i < (size/8); i++) {
        GET_RANDOM_NUMBER_64(&RandNo);
        *Buffer = RandNo;
        Buffer++;
    }
    // Fill the remaining buffer byte by byte
    if(Remainder) {
        TempBuffer = (UINT8*)Buffer;
        GET_RANDOM_NUMBER_64(&RandNo);
        
        while(Remainder) {
          *TempBuffer = (UINT8)RandNo;
          TempBuffer++;
          RandNo = RandNo >> 8;
          Remainder--;
        }
    }
    return;
}

/** @internal
    This function handles password operations (check/validate/change/clear)
    for all HDD password fields

    @param HiiHandle           HII handle of formset
    @param Class               class of formset
    @param SubClass            subclass of formset
    @param Key                 Id of setup control

    @retval EFI_UNSUPPORTED     Browser actions not supported.

    @note
    Case1: Check password status
        EFI_SUCCESS      No password
        EFI_NOT_FOUND    Password exists
    Case2: Validate password
        EFI_SUCCESS      Password is correct
        EFI_NOT_READY    Password is wrong
    Case3: Set/Clear password
        EFI_SUCCESS      Operation successful
        EFI_NOT_READY    Operation failed
        
    @endinternal
**/
EFI_STATUS
HddPasswordCallback(
        EFI_HII_HANDLE HiiHandle,
        UINT16         Class,
        UINT16         SubClass,
        UINT16         Key
)
{
    EFI_STATUS          Status;
    CALLBACK_PARAMETERS *CallbackParameters;
    CHAR16              PasswordBuffer[IDE_PASSWORD_LENGTH+1];
    static CHAR16       OldPasswordBuffer[IDE_PASSWORD_LENGTH+1];
    BOOLEAN             bCheckUser = FALSE;
    BOOLEAN             IsSecurityEnabled = FALSE;
    static BOOLEAN      IsPasswordValidated = FALSE;
    static BOOLEAN      NoPasswordExistedOnFirstCall = FALSE;
    static UINT8        LastHardDiskNumber = 0xFF;
    UINT8               CurrentHardDiskNumber;
    UINTN               size;
    VOID*               DataPtr;

    // Get actual parameter passed by browser
    CallbackParameters = GetCallbackParameters();

    // Check for supported browser actions
    if(!CallbackParameters || ((CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) && \
            (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) && (CallbackParameters->Action != EFI_BROWSER_ACTION_DEFAULT_STANDARD))) {
        return EFI_UNSUPPORTED;
    }

    // Always return success here
    if((CallbackParameters->Action == EFI_BROWSER_ACTION_CHANGED) || (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD )) {
        return EFI_SUCCESS;
    }

    // Check which password field was selected
    switch(Key)
    {
    case HDD_SECURITY_GET_USERPASSWORD1_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 0;
        break;
#if defined(ALL_HDD_SAME_PW) && ALL_HDD_SAME_PW == 0
    case HDD_SECURITY_GET_USERPASSWORD2_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 1;
        break;
    case HDD_SECURITY_GET_USERPASSWORD3_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 2;
        break;
    case HDD_SECURITY_GET_USERPASSWORD4_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 3;
        break;
    case HDD_SECURITY_GET_USERPASSWORD5_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 4;
        break;
    case HDD_SECURITY_GET_USERPASSWORD6_KEY:
        bCheckUser = TRUE;
        CurrentHardDiskNumber = 5;
        break;
#endif //ALL_HDD_SAME_PW
#if defined(DISPLAY_MASTER_PASSWORD) && DISPLAY_MASTER_PASSWORD
    case HDD_SECURITY_GET_MASTERPASSWORD1_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 0;
        break;
#if defined(ALL_HDD_SAME_PW) && ALL_HDD_SAME_PW == 0
    case HDD_SECURITY_GET_MASTERPASSWORD2_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 1;
        break;
    case HDD_SECURITY_GET_MASTERPASSWORD3_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 2;
        break;
    case HDD_SECURITY_GET_MASTERPASSWORD4_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 3;
        break;
    case HDD_SECURITY_GET_MASTERPASSWORD5_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 4;
        break;
    case HDD_SECURITY_GET_MASTERPASSWORD6_KEY:
        bCheckUser = FALSE;
        CurrentHardDiskNumber = 5;
        break;
#endif //ALL_HDD_SAME_PW
#endif //DISPLAY_MASTER_PASSWORD
    default:
        CurrentHardDiskNumber = 0xFF;
        LastHardDiskNumber = 0xFF;
        break;
    }

    // Not a valid HDD number
    if( CurrentHardDiskNumber == 0xFF ) {
        return EFI_NOT_FOUND;
    }

    // Just to make sure that two subsequent calls are for same HDD
    if(LastHardDiskNumber != 0xFF && LastHardDiskNumber != CurrentHardDiskNumber) {
        IsPasswordValidated = FALSE;
        LastHardDiskNumber = 0xFF;
        NoPasswordExistedOnFirstCall = FALSE;
        ZeroMem((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
    }

    // Allocate memory for setup data
    if(NULL == gHddSecurityConfig) {
        Status = pBS->AllocatePool( EfiBootServicesData,
                                    sizeof(HDD_SECURITY_CONFIG),
                                    (VOID**)&gHddSecurityConfig);
        if(EFI_ERROR(Status) || NULL == gHddSecurityConfig) {
            return EFI_NOT_FOUND;
        }
    }

    // Get setup data
    size = sizeof(HDD_SECURITY_CONFIG);
    Status = pRS->GetVariable(  L"HDDSecConfig",
                                &gHddSecurityConfigGuid,
                                NULL,
                                &size,
                                gHddSecurityConfig);
    if(EFI_ERROR(Status)) {
        return EFI_NOT_FOUND;
    }
    
    if(gHddSecEscNotifyEvent == NULL) {
        //register notify for gHddSecEscOnPwdPromptGuid
        Status = pBS->CreateEvent( EFI_EVENT_NOTIFY_SIGNAL,
                                   TPL_CALLBACK,
                                   NotifyEscPressed,
                                   &gHddSecEscNotifyRegistration,
                                   &gHddSecEscNotifyEvent );
        if(EFI_ERROR(Status)) {
            return Status;
        }
          
        pBS->RegisterProtocolNotify( &gHddSecEscOnPwdPromptGuid,
                                     gHddSecEscNotifyEvent,
                                     &gHddSecEscNotifyRegistration );
    }

    IsSecurityEnabled = gHddSecurityConfig->Enabled[CurrentHardDiskNumber] ? TRUE : FALSE;

    // Get string from HII database using input string token
    if(CallbackParameters->Value->string != 0) {
        size = sizeof(PasswordBuffer);
        Status = HiiLibGetString(   HiiHandle,
                                    CallbackParameters->Value->string,
                                    &size,
                                    (EFI_STRING)PasswordBuffer);
        if(EFI_ERROR(Status)) {
            return EFI_NOT_FOUND;
        }
    }
    else {
        return EFI_NOT_FOUND;
    }
    // Return password status
    if (PasswordBuffer[0] == L'\0' && (IsPasswordValidated == FALSE) && (NoPasswordExistedOnFirstCall == FALSE)) {
        LastHardDiskNumber = CurrentHardDiskNumber;
        // Initializing the OldPasswordBuffer to zeros for handling the callback 
        // when Esc is pressed
        ZeroMem((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
        // If Esc is pressed reset the flag
        gHddSecIsEscPressed = FALSE;
        if(IsSecurityEnabled == TRUE) {
            NoPasswordExistedOnFirstCall = FALSE;
            return EFI_NOT_FOUND;   // Pre-existing password
        }
        else {
            NoPasswordExistedOnFirstCall = TRUE;
            return EFI_SUCCESS;    // No pre-existing password.
        }
    }
    else if(IsSecurityEnabled == TRUE && IsPasswordValidated == FALSE) { // Validate password
        DataPtr = GetHddSecurityInternalDataPtr( CurrentHardDiskNumber );
        Status = HddPasswordAuthenticate( PasswordBuffer, DataPtr, bCheckUser ); //Authenticate it
        if(EFI_ERROR( Status )) {
            // To make sure OldPasswordBuffer is filled with random number before exiting the function.
            FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
            return EFI_NOT_READY;   // Password is wrong.
        }
        else {
            IsPasswordValidated = TRUE; // Set it so that password could be changed/cleared on next call
            Wcscpy(OldPasswordBuffer,PasswordBuffer); // Save existing HDD password
            return EFI_SUCCESS;    // Password is correct.
        }
    }
    else { // Set new password or clear old one
        // Reset static variables
        IsPasswordValidated = FALSE;
        NoPasswordExistedOnFirstCall = FALSE;
        DataPtr = GetHddSecurityInternalDataPtr( CurrentHardDiskNumber );

        // If existing password is equals to new password then do nothing
        if((Wcscmp(OldPasswordBuffer,PasswordBuffer)==0) && gHddSecIsEscPressed) {
            // Make sure Buffer is filled with random number before exiting the function.
            FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));
            FillRandNoInPwdBuffer((VOID*)((HDD_SECURITY_INTERNAL_DATA*)DataPtr)->PWD, sizeof(((HDD_SECURITY_INTERNAL_DATA*)DataPtr)->PWD));
            // If Esc is pressed reset the flag
            gHddSecIsEscPressed = FALSE;
            return EFI_SUCCESS;
        }

        FillRandNoInPwdBuffer((VOID *)OldPasswordBuffer,sizeof(OldPasswordBuffer));

        // Set/Clear HDD password
        if(!HddPasswordUpdate( (UINT32)CurrentHardDiskNumber, PasswordBuffer, bCheckUser )) {
        	if(bCheckUser) {
        		return EFI_NOT_READY; // Not updated
        	}
        }
        // After setting pwd fill the buffer with random number
        FillRandNoInPwdBuffer((VOID*)((HDD_SECURITY_INTERNAL_DATA*)DataPtr)->PWD, sizeof(((HDD_SECURITY_INTERNAL_DATA*)DataPtr)->PWD));
        //Update NVRAM data and browser data
        RefreshHddSecuritySetupData();
    }
    return EFI_SUCCESS;
}

/** @internal
    This function will update security information for all HDDs

    @param HiiHandle              HII handle of formset
    @param Class                  class of formset
    @param UINT16 SubClass        subclass of formset
    @param UINT16 Key             Id of setup control

    @retval EFI_SUCCESS         Updated security information
    @retval EFI_UNSUPPORTED     Operation not supported on current event

    @endinternal
**/
EFI_STATUS
HddSecurityUpdateFormCallback(
        EFI_HII_HANDLE HiiHandle,
        UINT16         Class,
        UINT16         SubClass,
        UINT16         Key
)
{
    CALLBACK_PARAMETERS *CallbackParameters;

    // Get actual parameter passed by browser
    CallbackParameters = GetCallbackParameters();

    // Check for supported browser actions
    if(!CallbackParameters || ((CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGING) && \
              (CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED)) ) {
        return EFI_UNSUPPORTED;
    }

    // Update security information (NVRAM data and browser data) for all HDDs
    RefreshHddSecuritySetupData();

    return EFI_SUCCESS;
}
#endif
