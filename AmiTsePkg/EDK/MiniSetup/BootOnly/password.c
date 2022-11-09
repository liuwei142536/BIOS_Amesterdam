//***********************************************************************//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/SOURCE/Modules/AMITSE2_0/AMITSE/BootOnly/password.c $
//
// $Author: Rajashakerg $
//
// $Revision: 2 $
//
// $Date: 12/08/11 12:46p $
//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//

/** @file password.c
    This file contains code to handle password operations

**/

#include "minisetup.h"

extern UINTN GetAMITSESETUPSize();
extern UINTN GetUserPasswordOffset();
extern UINTN GetAdminPasswordOffset();

/**
    function to check the password type

    @param void

    @retval void

**/
UINT32 PasswordCheckInstalledLocal(VOID)
{
	UINT32	Installed = AMI_PASSWORD_NONE;
	UINT8 *setup = NULL;
	AMITSESETUP *mSysConf = NULL;
	UINTN VarSize = 0;
	CHAR16 *TestPassword;
	
	TestPassword = EfiLibAllocateZeroPool((TsePasswordVariableLength + 1)*sizeof(CHAR16));
	
	GetAMITSEVariable(&mSysConf,&setup,&VarSize);

    
	if ( ( mSysConf != NULL ) && ( VarSize == GetAMITSESETUPSize() ) )
	{
		MemSet( TestPassword, (TsePasswordVariableLength + 1)*sizeof(CHAR16), 0 );
		if ( ! EfiCompareMem( TestPassword, ((UINT8*)(mSysConf))+ GetUserPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) )
			Installed |= AMI_PASSWORD_USER;
		if ( ! EfiCompareMem( TestPassword, ((UINT8*)(mSysConf))+ GetAdminPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) )
			Installed |= AMI_PASSWORD_ADMIN;
		Installed ^= AMI_PASSWORD_ANY;
	}

	if(setup)
		MemFreePointer( (VOID **)&setup );
	else
	    MemFreePointer( (VOID **)&mSysConf );

	StringZeroFreeMemory( (VOID **)&TestPassword);

	return Installed;
}

/**
    function to check the authentication of the password

    @param Password 

    @retval password type

**/
BOOLEAN IsPasswordSupportNonCaseSensitive ();
UINT32 PasswordAuthenticateLocal( CHAR16 *Password )
{
	UINT32 PasswordType = AMI_PASSWORD_NONE;
	UINT8 *setup = NULL;
	AMITSESETUP *mSysConf = NULL;
	UINTN VarSize = 0;
	CHAR16 *EncPass;
	UINTN	ii;

	GetAMITSEVariable(&mSysConf,&setup,&VarSize);
	
	if ( ( mSysConf == NULL ) || ( VarSize != GetAMITSESETUPSize() ) )
		return PasswordType;

	EncPass = EfiLibAllocateZeroPool((TsePasswordVariableLength + 1)*sizeof(CHAR16));

	MemSet( EncPass, (TsePasswordVariableLength + 1)*sizeof(CHAR16), 0 );
	StrCpyS( EncPass, TsePasswordVariableLength + 1, Password );
	
	if (IsPasswordSupportNonCaseSensitive ()) //If admin/user passwords are not encoded then case pbm will arose so avoiding that
	{
		for (ii = 0; ii < TsePasswordVariableLength; ii++)
			Password [ii] = ((Password [ii]>=L'a')&&(Password [ii]<=L'z'))?(Password [ii]+L'A'-L'a'):Password [ii];
	}

	PasswordEncodeHook( EncPass, TsePasswordVariableLength*sizeof(CHAR16));

	if ( ( ! EfiCompareMem( Password, ((UINT8*)(mSysConf))+ GetUserPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) ) ||
		 ( ! EfiCompareMem( EncPass, ((UINT8*)(mSysConf))+ GetUserPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) ) )
	{
		PasswordType = AMI_PASSWORD_USER;
	}

	if ( ( ! EfiCompareMem( Password, ((UINT8*)(mSysConf))+ GetAdminPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) ) ||
		 ( ! EfiCompareMem( EncPass, ((UINT8*)(mSysConf))+ GetAdminPasswordOffset(), TsePasswordVariableLength * sizeof(CHAR16) ) ) )
	{
		PasswordType = AMI_PASSWORD_ADMIN;
	}

	if(setup)
		MemFreePointer( (VOID **)&setup );
	else
	    MemFreePointer( (VOID **)&mSysConf );

	StringZeroFreeMemory( (VOID **)&EncPass );

	return PasswordType;
}

/**
    function to ubdate the password

    @param Password , UINTN Size - size of the buffer

    @retval updated password

**/
CHAR16 *PasswordUpdateLocal( CHAR16 *Password, UINTN Size )
{
	CHAR16 *Buffer = NULL;
	UINTN PwdSize = EfiStrLen(Password);

    if ( Password[0] == L'\0' )
        return Password;

    if ((PwdSize*sizeof(CHAR16)) > Size) // can't do anything, return buffer is not updated
        return Buffer;
    
    Buffer = EfiLibAllocateZeroPool( Size + sizeof(CHAR16) );
    
    if ( Buffer == NULL )
		return Buffer;

	MemCopy( Buffer, Password, PwdSize * sizeof (CHAR16));
	PasswordEncodeHook( Buffer, Size );

	return Buffer;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2015, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
