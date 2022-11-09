//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file Password.c
    This file contains the algorithm to handle the Input Password

**/

#include "AMISetupNVLock.h"

/** 
    This function deletes the SETUPPASSWD Variable
    
    @retval VOID
**/

VOID
DeletePasswordVariable()
{
    UINT32          Attributes = 0;
    UINTN           DataSize = 0; 
    VOID            *Data = NULL;
    EFI_STATUS      RetStatus = EFI_UNSUPPORTED;
    EFI_GUID        SetupPwdGuid = SETUPPASSWD_GUID;
    
    RetStatus = DxeGetVariable(L"$SETUPPASSWD",
                                &SetupPwdGuid,
                                &Attributes,
                                &DataSize,
                                Data);
         
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: SETUPPASSWD GetVariable Status: %r", RetStatus));
    if(EFI_NOT_FOUND != RetStatus){
        RetStatus = DxeSetVariable( L"$SETUPPASSWD", 
                                      &SetupPwdGuid,
                                      0 , 
                                      0, 
                                      Data );
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: SETUPPASSWD Variable Delete Status: %r", RetStatus));
    }
    return;
}


/**
    This function compares the variable is SETUPPASSWD

    @param *VariableName    Pointer to Variable name
    @param *VendorGuid      Pointer to VendorGuid

    @retval TRUE            if Variable is SETUPPASSWD
    @retval FALSE           Otherwise 

**/

BOOLEAN 
IsSetupPasswordVariable (
    IN CHAR16       *VariableName,
    IN EFI_GUID     *VendorGuid )
{
    EFI_GUID SetupPwdGuid = SETUPPASSWD_GUID;

    if ((guidcmp(VendorGuid, &SetupPwdGuid) == FALSE) && \
         (Wcscmp((CHAR16 *)VariableName, (CHAR16 *)SETUPPASS_VAR_NAME) == FALSE)) {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Password Variable Found"));
        return TRUE;
    }
    return FALSE;
}


/**
    This Function to validate the Password

    @param  Password        Password String
    @param  Passwordsize    Size of Password 

    @retval AMI_PASSWORD_NONE     The Password not validated
    @retval AMI_INVALID_PASSWORD  If the password is invalid
    @retval AMI_PASSWORD_ADMIN    Returns appropriate value as per validation.

**/

UINT32 
AuthenticatePassword ( 
    CHAR16  *Password, 
    UINTN   Passwordsize )
{
    UINT32 PasswordType = AMI_PASSWORD_NONE;
    UINTN  TsePasswordLength = TSE_PASSWORD_LENGTH;
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID TseSetupGuid = AMITSESETUP_GUID;
    AMITSESETUP *mSysConf = NULL;
    UINT16 OutBuffer[sizeof(AMITSESETUP)];
    UINT16 EncPass[TSE_PASSWORD_LENGTH+1];
    UINT16 EmptyPass[TSE_PASSWORD_LENGTH+1];
    UINT32 SetupDataAttributes = 0;
    UINTN SetupDataSize = sizeof(AMITSESETUP);
    
    Status = DxeGetVariable(TSE_SETUP_VAR_NAME, 
                            &TseSetupGuid, 
                            &SetupDataAttributes, 
                            &SetupDataSize, 
                            &OutBuffer );

    if (EFI_SUCCESS != Status) {
        return PasswordType;
    }
    
    mSysConf = (AMITSESETUP *)OutBuffer;
    MemSet( EmptyPass, (TsePasswordLength + 1)*sizeof(CHAR16), 0 );
    if (!MemCmp(EmptyPass, mSysConf->AdminPassword, TsePasswordLength * sizeof(CHAR16))) {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: Input Password is Empty"));
        return PasswordType;    // No Admin password Set.
    }
    
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: AuthenticatePassword Received Password Length: %d", Passwordsize));
#if (SETUP_STORE_KEYCODE_PASSWORD == 0)
    //For Unicode Password,exclude the size of NULL for PasswordSize  
    if ( (Passwordsize >= 2) && ( L'\0' == Password[(Passwordsize/2) -1] ) ) {
        Passwordsize = Passwordsize - 2;
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: AuthenticatePassword Modified Password Length: %d", Passwordsize));
    }
#endif    
    
    //Password length should be minimum PASSWORD_MIN_SIZE chars and maximum is TSE_PASSWORD_LENGTH
    if ((Passwordsize < (PASSWORD_MIN_SIZE * sizeof(CHAR16)) ) || \
         (Passwordsize > (TSE_PASSWORD_LENGTH * sizeof(CHAR16)) )) {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: Invalid Password length: %d", Passwordsize));
        return (UINT32)AMI_INVALID_PASSWORD;
    }
    
    MemSet(EncPass, (TsePasswordLength + 1)*sizeof(CHAR16), 0);
    MemCpy(EncPass, Password, Passwordsize);
    PasswordEncode(EncPass, TsePasswordLength * sizeof(CHAR16));
    
    if ( ( ! MemCmp( Password, mSysConf->AdminPassword, TsePasswordLength * sizeof(CHAR16) ) ) || \
            ( ! MemCmp( EncPass, mSysConf->AdminPassword, TsePasswordLength * sizeof(CHAR16) ) ) ) {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: Password match"));
        PasswordType = AMI_PASSWORD_ADMIN;
    } else {
        DEBUG_SETUPNVLOCK((DEBUG_INFO, "\n AmiSetupNvLock: Password did not match"));
        PasswordType = AMI_INVALID_PASSWORD;
    }
    
    return PasswordType;
}

/**
    This Function is to set the Setup Password User or Admin based on User input

    @param VariableName   Decides the Admin/User password to be set 
    @param Password       Password String
    @param Passwordsize   Size of Password 

    @retval EFI_STATUS    return the status of Set variable

**/

EFI_STATUS 
SetPassword (
    CHAR16  *VariableName, 
    VOID    *Password, 
    UINTN   Passwordsize )
{
    UINTN  TsePasswordLength = TSE_PASSWORD_LENGTH;
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GUID TSESetupGuid = AMITSESETUP_GUID;
    UINT32 SetupDataAttributes = 0;
    UINTN SetupDataSize = sizeof(AMITSESETUP);
    AMITSESETUP *mSysConf = NULL;
    UINT16 OutBuffer[sizeof(AMITSESETUP)];
    UINT16 EncPass[TSE_PASSWORD_LENGTH+1];
    
    //Get the Current Variable data
    Status = DxeGetVariable(TSE_SETUP_VAR_NAME, 
                            &TSESetupGuid, 
                            &SetupDataAttributes, 
                            &SetupDataSize, 
                            &OutBuffer );

    if (EFI_SUCCESS != Status) {
        return Status;
    }

    mSysConf = (AMITSESETUP *)OutBuffer;

    MemSet( EncPass, (TsePasswordLength + 1)*sizeof(CHAR16), 0 );
    MemCpy(EncPass, Password, Passwordsize);
    PasswordEncode(EncPass, TsePasswordLength*sizeof(CHAR16));

    //Update the password based on variable name
    if(!Wcscmp((CHAR16 *)VariableName, (CHAR16 *)SET_ADMINPASS_VAR_NAME)) {
        MemSet( mSysConf->AdminPassword, (TsePasswordLength )*sizeof(CHAR16), 0 );
        MemCpy(mSysConf->AdminPassword,EncPass,(TsePasswordLength )*sizeof(CHAR16));
    } else {
        MemSet( mSysConf->UserPassword, (TsePasswordLength )*sizeof(CHAR16), 0 );
        MemCpy(mSysConf->UserPassword,EncPass,(TsePasswordLength )*sizeof(CHAR16));
    }
    //Set the Variable data
    Status = DxeSetVariable(TSE_SETUP_VAR_NAME, 
                            &TSESetupGuid, 
                            SetupDataAttributes, 
                            SetupDataSize, 
                            mSysConf );
    
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nAmiSetupNvLock: SetPassword status : %r", Status));
    return Status;
}

#if (TSE_MINOR <= 0x16)
/**
    This Function returns SETUP_PASSWORD_NON_CASE_SENSITIVE token value

    @param void

    @retval TRUE    if SETUP_PASSWORD_NON_CASE_SENSITIVE is enabled
    @retval FALSE   if SETUP_PASSWORD_NON_CASE_SENSITIVE is disabled

**/
BOOLEAN 
IsPasswordSupportIsNonCaseSensitive() 
{
#if SETUP_PASSWORD_NON_CASE_SENSITIVE
    return TRUE;
#endif
    return FALSE;
}

/**
    This Function encodes the input string

    @param Password     Password array to be encrypted. Encrypted password is returned in the same array.
    @param MaxSize      Max size of Password

    @retval VOID
**/
VOID 
PasswordEncode ( 
    CHAR16 *Password, 
    UINTN MaxSize )
{
    UINTN	ii;
    unsigned int key = 0x935b;

    if (IsPasswordSupportIsNonCaseSensitive ()) {
        for ( ii = 0; ii < MaxSize; ii++ ) {
            Password[ii] = ((Password[ii] >= L'a') && (Password[ii] <= L'z')) ? (Password[ii] + L'A' - L'a') : Password[ii];
        }
    }
    // Encode the password..
    for ( ii = 1; ii <= MaxSize / 2; ii++ ) {
        Password[ii - 1] = (CHAR16)(Password[ii - 1] ^ (key * ii));
    }
}
#endif      //(TSE_MINOR <= 0x16)
