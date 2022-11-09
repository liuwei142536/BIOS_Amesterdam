//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file password.h
    Header file for code to handle password operations

**/

#ifndef _PASSWORD_H_
#define	_PASSWORD_H_

BOOLEAN _PasswordProtocolInit( VOID );
BOOLEAN PasswordBootMode( VOID );
UINT32 PasswordCheckInstalledVarIndex( UINT32 VarIndex );
UINT32 PasswordAuthenticate( CHAR16 *Password );
BOOLEAN PasswordAuthenticateVarIndex( UINT32 VarIndex, CHAR16 *Password );
CHAR16 *PasswordUpdate( CHAR16 *Password, UINTN Size );
VOID PasswordCommitChanges( BOOLEAN SaveChanges );

#endif /* _PASSWORD_H_ */
