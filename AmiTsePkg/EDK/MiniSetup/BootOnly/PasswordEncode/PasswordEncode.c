//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Archive: /Alaska/BIN/Modules/AMITSE2_0/AMITSE/PasswordEncode/PasswordEncode.c $
//
/** @file PasswordEncode.c
    This file contains code to handle password encoding feature

**/
//


#include "PasswordEncode.h"
#include <Library/BaseMemoryLib.h>

//**********************************************************************
/**
    Allows creating a hash of an arbitrary message digest using one or more hash algorithms

    @param 
        This          Pointer to the AMI_DIGITAL_SIGNATURE_PROTOCOL instance.
        HashAlgorithm Points to the EFI_GUID which identifies the algorithm to use.
        num_elem      Number of blocks to be passed via next argument:addr[]
        addr[]        Pointer to array of UINT8* addresses of data blocks to be hashed
        len           Pointer to array of integers containing length of each block listed by addr[]
        Hash          Holds the resulting hash computed from the message.

    @retval 
        EFI_SUCCESS           Hash returned successfully.
        EFI_INVALID_PARAMETER Message or Hash is NULL
        EFI_UNSUPPORTED       The algorithm specified by HashAlgorithm is not supported by this
        driver. Or extend is TRUE and the algorithm doesn't support extending the hash.

**/
#if TSE_HASH_PASSWORD
#if TSE_PWD_ENCRYPT_USING_SHA512
#define ShaInit Sha512Init
#define ShaUpdate Sha512Update
#define ShaFinal Sha512Final

#elif TSE_PWD_ENCRYPT_USING_SHA384
#define ShaInit Sha384Init
#define ShaUpdate Sha384Update
#define ShaFinal Sha384Final

#elif TSE_PWD_ENCRYPT_USING_SHA256
#define ShaInit Sha256Init
#define ShaUpdate Sha256Update
#define ShaFinal Sha256Final

#else
#define ShaInit Sha1Init
#define ShaUpdate Sha1Update
#define ShaFinal Sha1Final
#endif
#endif
#if TSE_HASH_PASSWORD
EFI_STATUS Hash(
	IN CONST EFI_GUID               *HashAlgorithm,
	IN UINTN                        num_elem,
	UINT8                  *stringToHash,
	IN CONST UINTN                  *HashSize,
	OUT UINT8                       *HashOutput
	)
{
	BOOLEAN Result = FALSE;
	UINT8 orgContext [216];
    Result = ShaInit(orgContext);
    if (!Result)
        return EFI_UNSUPPORTED;
         
    Result = ShaUpdate (orgContext, stringToHash, *HashSize);
    if (!Result)
        return EFI_UNSUPPORTED;
       
    Result = ShaFinal (orgContext, HashOutput);

	if(!Result) {
		return EFI_UNSUPPORTED;
	}
	return  EFI_SUCCESS;
}
#endif

/**
    Encodes the input string

    @param Password : Password array to be encrypted. Encryped
        password is returned in the same array.
        MaxSize : Max size of Password

    @retval VOID

**/
VOID TsePasswordEncodeLocal( CHAR16 *Password, UINTN MaxSize);
VOID PasswordEncode( CHAR16 *Password, UINTN MaxSize)
{
#if TSE_HASH_PASSWORD
    UINTN   ii;
    EFI_STATUS Status;

#if TSE_PWD_ENCRYPT_USING_SHA512
    UINT8 HashOutput [64];
    UINTN HashSize = SHA512_DIGEST_SIZE;
    EFI_GUID EfiHashAlgorithmShaGuid = EFI_HASH_ALGORITHM_SHA512_GUID;

#elif TSE_PWD_ENCRYPT_USING_SHA384
    UINT8 HashOutput [48];
    UINTN HashSize = SHA384_DIGEST_SIZE;
    EFI_GUID EfiHashAlgorithmShaGuid = EFI_HASH_ALGORITHM_SHA384_GUID;
    
#elif TSE_PWD_ENCRYPT_USING_SHA256
    UINT8 HashOutput [32];
    UINTN HashSize = SHA256_DIGEST_SIZE;
    EFI_GUID EfiHashAlgorithmShaGuid = EFI_HASH_ALGORITHM_SHA256_GUID;

#if (SETUP_PASSWORD_LENGTH < 16) && (EXTENDED_TSE_PWD_ENCRYPT_SUPPORT == 0)
#error SETUP_PASSWORD_LENGTH cannot be less than 16 when SHA256 encryption is used
#endif  

#else
    UINT8 HashOutput[20];
    UINTN HashSize = SHA1_DIGEST_SIZE;
    EFI_GUID EfiHashAlgorithmShaGuid = EFI_HASH_ALGORITHM_SHA1_GUID;

#if (SETUP_PASSWORD_LENGTH < 10) && (EXTENDED_TSE_PWD_ENCRYPT_SUPPORT == 0)
#error SETUP_PASSWORD_LENGTH cannot be less than 10 when SHA1 encryption is used
#endif  

#endif    
    if (MaxSize < HashSize)
    {
         MemSet (Password, MaxSize, 0);
         return;
    }
    if (IsPasswordSupportNonCaseSensitive ())
    {
        for ( ii = 0; ii < MaxSize/2; ii++ )
            Password[ii] = ((Password[ii]>=L'a')&&(Password[ii]<=L'z'))?(Password[ii]+L'A'-L'a'):Password[ii];
    }
    Status = Hash(&EfiHashAlgorithmShaGuid, TRUE, (UINT8*)Password, (CONST UINTN*)&MaxSize, (UINT8*)&HashOutput);
    if (!EFI_ERROR (Status))
    {
        MemSet (Password, MaxSize, 0);
        CopyMem ((UINT8*)Password, (UINT8*)HashOutput, HashSize);
    }
#else
   TsePasswordEncodeLocal (Password, MaxSize);
#endif
}

/**
    Returns SETUP_PASSWORD_NON_CASE_SENSITIVE token value

    @param void

    @retval BOOLEAN

**/
BOOLEAN IsPasswordSupportNonCaseSensitive()
{
#if SETUP_PASSWORD_NON_CASE_SENSITIVE
	return TRUE;
#else
	return FALSE;
#endif
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
