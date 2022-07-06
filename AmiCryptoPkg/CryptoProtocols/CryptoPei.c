//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
 * CryptoPei.c
 */
#include <Token.h>
#include <AmiPeiLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include "Ppi/LoadFile.h"
#include <Protocol/Hash.h>
#include <PPI/CryptoPPI.h>
#include "AmiCertificate.h"
#include <Protocol/AmiDigitalSignature.h> // EFI_CERT_X509_SHAXXX
#include <Library/BaseMemoryLib.h>

//#include <Library/PerformanceLib.h>

//
// Global variables
//
// Although ShaXXXGuid global variables are defined in EDK's EdkProtocol Lib, but linking it adds additional 20k in debug mode.
//static EFI_GUID gEfiHashAlgorithmSha1Guid   = EFI_HASH_ALGORITHM_SHA1_GUID;
//static EFI_GUID gEfiHashAlgorithmSha256Guid = EFI_HASH_ALGORITHM_SHA256_GUID;
//static EFI_GUID gEfiHashAlgorithmSha384Guid = EFI_HASH_ALGORITHM_SHA384_GUID;
//static EFI_GUID gEfiHashAlgorithmSha512Guid = EFI_HASH_ALGORITHM_SHA512_GUID;
//static EFI_GUID gPRKeyGuid                  = PR_KEY_GUID;

//
// SDL defined Public Exponent E of RSA Key.
//
const UINT8  KeyE[] = {E_CONST}; // 0x10001
const UINTN  LenE = sizeof(KeyE);
//    PKCS_1 PSS Signature constant. Size of the Salt (random data) field in PSS signature.
const INT32  PssSaltlen = PSS_SIG_SALTLEN; // 0 - saltlen matches the size of hash

//----------------------------------------------------------------------------
// Crypto Function prototypes
//----------------------------------------------------------------------------
EFI_STATUS
EFIAPI
PeiHash (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI      *This,
  IN CONST EFI_GUID               *HashAlgorithm,
  IN UINTN                        num_elem,
  IN CONST UINT8                  *addr[],
  IN CONST UINTN                  *len,
  OUT UINT8                       *Hash
  );

EFI_STATUS
EFIAPI
PeiPkcs7Verify (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST UINT8 *P7Data,
  IN UINTN        P7Size,
  IN CONST UINT8 *TrustedCert,
  IN UINTN        CertSize,
  IN OUT UINT8  **Data,
  IN OUT UINTN   *DataSize
  );

EFI_STATUS
EFIAPI
PeiPkcs7GetDigestAlgorithm (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST UINT8 *P7Data,
  IN UINTN        P7Size,
  IN OUT HASH_ALG *DigestAlgorithm
  );

EFI_STATUS
EFIAPI
PeiVerifySig 
(
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN PEI_CRYPT_HANDLE          *PublicKey,
  IN PEI_CRYPT_HANDLE          *Hash,
  IN VOID                      *Signature,
  IN UINTN                     SignatureSize,
  IN UINT32                    Flags
);

EFI_STATUS
EFIAPI
PeiGetKey 
(
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST EFI_GUID            *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN PEI_CRYPT_HANDLE          *PublicKey
  );
  
EFI_STATUS
EFIAPI
PeiVerifyKey 
(
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST EFI_GUID            *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN PEI_CRYPT_HANDLE          *PublicKey
  );

//----------------------------------------------------------------------------
// Crypto Protocol Identifiers
//----------------------------------------------------------------------------
AMI_CRYPT_DIGITAL_SIGNATURE_PPI  mSigPeiInitPpi = {
  PeiHash,
  PeiVerifyKey,
  PeiVerifySig,
  PeiGetKey,
  PeiPkcs7Verify,
  PeiPkcs7GetDigestAlgorithm
};

//--------------------------------------------------------------------------------
// Event callbacks and PPIs to be installed
//--------------------------------------------------------------------------------
EFI_PEI_PPI_DESCRIPTOR mPpiSigListVariable = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmiDigitalSignaturePPIGuid,
  &mSigPeiInitPpi
};

//------------------------------------------------------------------------
// Local prototypes
//------------------------------------------------------------------------
#if 0
EFI_STATUS NotifyAtPeiEnd (
    IN EFI_PEI_SERVICES             **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
    IN VOID                         *Ppi
);
//------------------------------------------------------------------------
// Event callbacks and PPI notifications to be installed
//------------------------------------------------------------------------
static EFI_PEI_NOTIFY_DESCRIPTOR mInstallCryptoPpiNotify[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
        &gAmiInstallCryptoPpiGuid,
        NotifyAtPeiEnd
    },
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK,
        &gEfiPeiBootInRecoveryModePpiGuid,
        NotifyAtPeiEnd
    },
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gFlashUpdBootModePpiGuid,//gEfiPeiMemoryDiscoveredPpiGuid,
        NotifyAtPeiEnd
    },
};
#endif
//--------------------------------------------------------------------------------
/**
  The function locates a FwKey Hob pointing to a
  Platform Signing Key (PK) inside FV_BB ffs

  @param[out]  Pointer to pFwKeyHob

  @retval     Status
    EFI_SUCCESS               The Key Hob was found
    EFI_NOT_FOUND             The Key Hob not found
**/
EFI_STATUS
CryptoGetFwKeyHob (
    IN OUT FW_KEY_HOB **pFwKeyHob
){
    EFI_STATUS       Status = EFI_NOT_FOUND;
    CONST EFI_PEI_SERVICES **PeiServices;
    VOID            *FirstHob;

    PeiServices = GetPeiServicesTablePointer ();
    if (*PeiServices == NULL) ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    (*PeiServices)->GetHobList(PeiServices, &FirstHob);
    if (!FirstHob) ASSERT_EFI_ERROR (EFI_NOT_FOUND);

    Status = FindNextHobByGuid (&gPRKeyGuid, &FirstHob);
    if (!EFI_ERROR(Status)) 
        *pFwKeyHob = (FW_KEY_HOB *)FirstHob;

    return Status;
}

//------------------------------------------------------
// PPI
//------------------------------------------------------
/**
  Allows creating a hash of an arbitrary message digest using one or more hash algorithms

  @param[in]  This          Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]  HashAlgorithm Points to the EFI_GUID which identifies the algorithm to use.
  @param[in]  num_elem      Number of blocks to be passed via next argument:addr[]
  @param[in]  addr[]        Pointer to array of UINT8* addresses of data blocks to be hashed
  @param[in]  len           Pointer to array of integers containing length of each block listed by addr[]
  @param[in]  Hash          Holds the resulting hash computed from the message.
  
  @retval     Status
        EFI_SUCCESS           Hash returned successfully.
        EFI_INVALID_PARAMETER Message or Hash is NULL
        EFI_UNSUPPORTED       The algorithm specified by HashAlgorithm is not supported by this driver.
**/
EFI_STATUS
EFIAPI
PeiHash (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI      *This,
  IN CONST EFI_GUID               *HashAlgorithm,
  IN UINTN                        num_elem,
  IN CONST UINT8                  *addr[],
  IN CONST UINTN                  *len,
  OUT UINT8                       *Hash
  )
{
// Support SHA hashes
    if(!guidcmp((EFI_GUID*)HashAlgorithm, &gEfiHashAlgorithmSha256Guid))
    {
       sha256_vector(num_elem, (const UINT8 **)addr, (const size_t *)len, Hash);
    }
#if CONFIG_PEI_PKCS7 == 1
    else if(!guidcmp((EFI_GUID*)HashAlgorithm, &gEfiHashAlgorithmSha1Guid)) 
        {
           sha1_vector(num_elem, (const UINT8 **)addr, (const size_t *)len, Hash);
        }
    else if(!guidcmp((EFI_GUID*)HashAlgorithm, &gEfiHashAlgorithmSha384Guid))
        {
           sha384_vector(num_elem, (const UINT8 **)addr, (const size_t *)len, Hash);
        }
    else if(!guidcmp((EFI_GUID*)HashAlgorithm, &gEfiHashAlgorithmSha512Guid))
        {
           sha512_vector(num_elem, (const UINT8 **)addr, (const size_t *)len, Hash);
        }
#endif
    else
        return EFI_UNSUPPORTED;

    return  EFI_SUCCESS;
}

/**
   Function verifies that the specified signature matches the specified hash. 
                 Verifies the RSA-SSA signature with EMSA-PKCS1-v1_5 encoding scheme defined in
                 RSA PKCS#1.
                 This function decrypts the signature with the provided key and then compares 
                 the decrypted value to the specified hash value

  @param[in]  This          Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]  PublicKey     Handle to a key used for verifying signatures. This handle must be identifying a public key.
  @param[in]  Hash          Handle of the hash object to verify.
  @param[in]  Signature     Pointer to the signature data to be verified.
  @param[in]  SignatureSize The size, in bytes, of the signature data.
  @param[in]  Flags        Specifies additional flags to further customize the signing/verifying behavior.

  @retval     Status
    EFI_SUCCESS               The signature is successfully verified.
    EFI_SECURITY_VIOLATION    The signature does not match the given message.
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
EFIAPI
PeiVerifySig
(
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI      *This,
  IN PEI_CRYPT_HANDLE            *PublicKey,
  IN PEI_CRYPT_HANDLE            *Hash,
  IN VOID                        *Signature,
  IN UINTN                       SignatureSize,
  IN UINT32                      Flags
  )
{
    EFI_STATUS      Status;
    INTN            err;
    struct          crypto_rsa_key *key = NULL;
    size_t         *sig_len=(size_t*)&SignatureSize;
    INT32           pss_modulus_bitlen;
    INT32           pss_saltlen;
    UINT32          HashLen;
    UINT8           DecriptedSig[DEFAULT_RSA_SIG_LEN];

// Only supporting RSASSA_PKCS1V15 signature types
    if(!((Flags & EFI_CRYPT_RSASSA_PKCS1V15) || 
         (Flags & EFI_CRYPT_RSASSA_PSS)) )
            return EFI_INVALID_PARAMETER;

    ResetCRmm();

// check Key handle if requested PubKey is a Platform FW Key
// In this case use the FW key from ffs image
// validity of the Key must be verified prior to calling VerifySig
// Guid must match one of the valid keys we can use in PEI Sig verification.
// For now Public Key is supported in 2 formats: RAW 256 bytes and ASN.1 Integer
// shall be extended to gPBkey_x509_Guid
    if(!guidcmp(&PublicKey->AlgGuid, &gEfiCertRsa2048Guid))
        key = crypto_import_rsa2048_public_key(PublicKey->Blob, PublicKey->BlobSize, (UINT8*)&KeyE, LenE);
    else
        return EFI_INVALID_PARAMETER;

    if(!guidcmp(&Hash->AlgGuid, &gEfiHashAlgorithmSha256Guid))
        HashLen = SHA256_DIGEST_SIZE;
    else if(!guidcmp(&Hash->AlgGuid, &gEfiHashAlgorithmSha1Guid))
            HashLen = SHA1_DIGEST_SIZE;
        else
            HashLen = SHA256_DIGEST_SIZE;

    if(key == NULL )
        err = -1;
    else
        err = crypto_rsa_exptmod((const UINT8*)Signature, (size_t)SignatureSize, (UINT8*)&DecriptedSig, sig_len, key, 0);
// locate Hash inside the decrypted signature body and compare it with given Hash;
// Should be extended to handle sha1, sha256 hashes. use Hash->AlgGuid to determine the Hash type
    if(!err) 
    {
        if(Flags & EFI_CRYPT_RSASSA_PKCS1V15)
        {
            // Validate PKCS#1v1.5 Padding
            err = pkcs_1_v1_5_decode((const UINT8 *)Hash->Blob, HashLen, (const UINT8 *)&DecriptedSig, (unsigned long)*sig_len);
            // get location of the hash in the Pkcs#1v1.5 padded sig
//            err = MemCmp(Hash->Blob, (void*)((UINTN)DecriptedSig + (UINTN)(*sig_len - HashLen)), HashLen);
        } else { //(Flags & EFI_CRYPT_RSASSA_PSS))
            // Validate PKCS#1 PSS Signature: padding & hash
            pss_saltlen = (PssSaltlen!=0)?(INT32)PssSaltlen:HashLen;
            pss_modulus_bitlen = DEFAULT_RSA_SIG_LEN << 3;
            err = pkcs_1_pss_decode(
                    Hash->Blob, HashLen, 
                    (const unsigned char *)&DecriptedSig, (unsigned long)*sig_len, 
                    pss_saltlen,  pss_modulus_bitlen);
        }
    }

    Status = !err ? EFI_SUCCESS:EFI_SECURITY_VIOLATION;

// Security concern, memory heap is being cleared on exit 
    ResetCRmm();

    return Status;
}

/**
 Verifies the validity of a PKCS#7 signed data as described in 
 "PKCS #7: Cryptographic Message Syntax Standard".
 Function verifies validity of the signature contained inside the Certificate
 This function decrypts the signature with the Public key from the Signer certificate
 and then compares the decrypted value to the input Data

  @param[in]  This         Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]  P7Data       Pointer to the PKCS#7 DER encoded message to verify.
  @param[in]  P7Size       Size of the PKCS#7 message in bytes.
  @param[in]  TrustedCert  Pointer to a trusted/root X509 certificate encoded in DER, which
                           is used for certificate chain verification.
  @param[in]  CertSize     Size of the trusted certificate in bytes.
  @param[in]  Data         Pointer to the content to be verified/returned at
  @param[in]  DataSize     Size of Data in bytes

  @retval     Status
    EFI_SUCCESS              The specified PKCS#7 signed data is valid
    EFI_SECURITY_VIOLATION   Invalid PKCS#7 signed data.
    EFI_ACCESS_DENIED        The Trusted certificate does not have a match in SignedData.certificate store.
    EFI_INVALID_PARAMETER    The size of input message or signature does not meet the criteria 
                             of the underlying signature algorithm.
**/
EFI_STATUS
EFIAPI
PeiPkcs7Verify (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST UINT8 *P7Data,
  IN UINTN        P7Size,
  IN CONST UINT8 *TrustedCert,
  IN UINTN        CertSize,
  IN OUT UINT8  **Data,
  IN OUT UINTN   *DataSize
  )
{
#if CONFIG_PEI_PKCS7 == 1

    INTN                       err, reason;
    struct pkcs7_signed_data_st* PKCS7cert;
    struct x509_certificate *x509TrustCert;
    struct x509_certificate *x509SignCert;
    struct pkcs7_cert_revoke_info revokeInfo;


    if((*Data == NULL || DataSize == NULL) ||
       (P7Data == NULL || P7Size== 0) || 
       (TrustedCert == NULL || CertSize== 0)
    ) 
        return EFI_INVALID_PARAMETER;

    ResetCRmm();

    err     = -1;
    reason  = 0;
    x509SignCert = NULL;

    PKCS7cert = Pkcs7_parse_Authenticode_certificate(P7Data, P7Size);
    if (PKCS7cert) {
        // verify Pkcs7 Signing Cert chain up to the TrustCert...
        x509TrustCert = x509_certificate_parse(TrustedCert, CertSize);
        if(x509TrustCert) {
            err = Pkcs7_x509_certificate_chain_validate(PKCS7cert, x509TrustCert, (int*)&reason);
        } else {
        // potentially a TimeStamped Hash of x509 tbs certificate data
            if(CertSize == sizeof(EFI_CERT_X509_SHA256)) {
                revokeInfo.ToBeSignedHashLen = 32;
                revokeInfo.ToBeSignedHash = (UINT8*)TrustedCert;
                err = Pkcs7_x509_certificate_chain_validate_with_timestamp(PKCS7cert, FALSE, NULL, &revokeInfo, (int*)&reason);
            }
            else // unsupported Hash struct
                err = -1;
        }
        if(!err)
            err = Pkcs7_certificate_validate_digest(PKCS7cert, x509SignCert, *Data, *DataSize);
    } 

    // Security concern, memory heap is being cleared on exit 
    ResetCRmm();

    // failed to process
    return ((!err) ? EFI_SUCCESS:EFI_SECURITY_VIOLATION);
#else
    return EFI_UNSUPPORTED;
#endif    
}

/**
  Verifies the validity of a PKCS#7 signed data as described in 
  "PKCS #7: Cryptographic Message Syntax Standard" 
  and returns the Digest Algorithm identifier

  @param[in]  This         Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]  P7Data       Pointer to the PKCS#7 DER encoded message to verify.
  @param[in]  P7Size       Size of the PKCS#7 message in bytes.

  @retval     Status
    EFI_SUCCESS              The specified PKCS#7 signed data is valid
    EFI_SECURITY_VIOLATION   Invalid PKCS#7 signed data.
    EFI_ACCESS_DENIED        The Trusted certificate does not have a match in SignedData.certificate store.
    EFI_INVALID_PARAMETER    The size of input message or signature does not meet the criteria 
                             of the underlying signature algorithm.
**/
EFI_STATUS
EFIAPI
PeiPkcs7GetDigestAlgorithm (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI *This,
  IN CONST UINT8 *P7Data,
  IN UINTN        P7Size,
  IN OUT HASH_ALG *DigestAlgorithm
  )
{
#if CONFIG_PEI_PKCS7 == 1

    INTN        err;
    struct pkcs7_signed_data_st* PKCS7cert;

    if(P7Data == NULL || P7Size== 0 || DigestAlgorithm == NULL)
        return EFI_INVALID_PARAMETER;

    ResetCRmm();

    err = -1;

    PKCS7cert = Pkcs7_parse_Authenticode_certificate(P7Data, P7Size);
    if (PKCS7cert) {
        err = Pkcs7_return_digestAlgorithm(PKCS7cert, (UINT8*)DigestAlgorithm);
//    err = Pkcs7_return_digestType(P7Data, P7Size, (UINT8*)DigestAlgorithm);
    }
    // Security concern, memory heap is being cleared on exit 
    ResetCRmm();

    // failed to process
    return ((!err) ? EFI_SUCCESS:EFI_SECURITY_VIOLATION);
#else
    return EFI_UNSUPPORTED;
#endif
}

/**
    Function returns Ptr to a Platform Signing Key (PK) Ffs inside Recovery FV (FV_BB or similar)

  @param[in]   This         Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]   KeyAlgorithm Points to the EFI_GUID which identifies the PKpub algorithm to use.
  @param[out]  PublicKey    Handle to a key used to return a ptr to a Key. This handle must be identifying a public key.

  @retval     Status
    EFI_SUCCESS               The Key is successfully returned.
    EFI_NOT_FOUND             The Key not found
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
EFIAPI
PeiGetKey (
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI      *This,
  IN CONST EFI_GUID          *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN OUT PEI_CRYPT_HANDLE        *PublicKey
  )
{
    FW_KEY_HOB *pFwKeyHob;

    if(PublicKey == NULL || KeyAlgorithm == NULL)
        return EFI_INVALID_PARAMETER;

    // now only supporting PKpub key comparison
    if(guidcmp((EFI_GUID*)KeyAlgorithm, &gPRKeyGuid))
        return EFI_INVALID_PARAMETER;

    // Acquire FwKey hob
    pFwKeyHob = NULL;
    CryptoGetFwKeyHob(&pFwKeyHob);
    if(pFwKeyHob) {
        PublicKey->AlgGuid = pFwKeyHob->KeyGuid;
        PublicKey->Blob = (UINT8*)(UINTN)(pFwKeyHob->KeyAddress);
        PublicKey->BlobSize = pFwKeyHob->KeySize;
        return EFI_SUCCESS;
    }

    return EFI_NOT_FOUND;
}

/**
  Function compares the input PublicKey against Platform Signing Key (PK) image in the flash.

  @param[in]   This         Pointer to the AMI_CRYPT_DIGITAL_SIGNATURE_PPI instance.
  @param[in]   KeyAlgorithm    Points to the EFI_GUID which identifies the PKpub algorithm to use.
  @param[in]   PublicKey       Handle to a key used for verifying signatures.  This handle must be identifying a public key.

  @retval     Status
    EFI_SUCCESS               The Key is successfully verified.
    EFI_SECURITY_VIOLATION    The Key does not match current FW key.
    EFI_ACCESS_DENIED         The key could not be used in signature operation.
    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                              of the underlying signature algorithm.
**/
EFI_STATUS
EFIAPI
PeiVerifyKey
(
  IN CONST AMI_CRYPT_DIGITAL_SIGNATURE_PPI      *This,
  IN CONST EFI_GUID          *KeyAlgorithm, // reserved to PKPUB_KEY_GUID
  IN PEI_CRYPT_HANDLE        *PublicKey
  )
{
    EFI_STATUS    Status;
    int           err;
    PEI_CRYPT_HANDLE KeyHndl;
    UINT8         Hash[SHA256_DIGEST_SIZE] = {0};
    UINT8         *KeyBuf, *PKpubBuffer=NULL;
    size_t        KeyLen;

    if(PublicKey == NULL || KeyAlgorithm == NULL || PublicKey->Blob == NULL)
        return EFI_INVALID_PARAMETER;

// check Key handle if requested PubKey is a Platform Key PKpub.
// In this case use PKpub key from ffs image
    if(guidcmp((EFI_GUID*)KeyAlgorithm, &gPRKeyGuid))
        return EFI_INVALID_PARAMETER;
    
    Status = PeiGetKey(This, KeyAlgorithm, &KeyHndl);
    if(EFI_ERROR(Status))
        return Status;
//
//==========================    
// FWKey : hash 
//    pubkey : Rsa2048 - hash it
//    pubkey : x509 - N/A 
//
// FWKey : x509
//    pubkey : Rsa2048 (Pkcs1) - xtract x509 pub key from FWkey
// 
// FWKey : Rsa2048
//    pubkey : x509 (Pkcs7) - xtract nModulus from x509 pubkey
//==========================    
//
    PKpubBuffer = KeyHndl.Blob;
    KeyLen = (size_t)KeyHndl.BlobSize; // always 256
    KeyBuf = PublicKey->Blob;
    err = TRUE;
//  FWKey : hash
    if(!guidcmp(&KeyHndl.AlgGuid, &gEfiCertSha256Guid) ){
        KeyBuf = Hash;
        //    pubkey : Rsa2048 - hash it
        if(!guidcmp(&PublicKey->AlgGuid, &gEfiCertRsa2048Guid) ){
        // SHA256 Hash of RSA Key
            KeyLen = (size_t)PublicKey->BlobSize;
            sha256_vector(1, (const UINT8**)&PublicKey->Blob, (const size_t*)&KeyLen, KeyBuf);
        } 
        KeyLen = SHA256_DIGEST_SIZE;
    } else 
// FWKey : x509        
    if(!guidcmp(&KeyHndl.AlgGuid, &gEfiCertX509Guid) ){
    //    pubkey : Rsa2048 (Pkcs1) - xtract x509 pub key
        if(!guidcmp(&PublicKey->AlgGuid, &gEfiCertRsa2048Guid) ){
#if CONFIG_PEI_PKCS7 == 1
            KeyLen = DEFAULT_RSA_KEY_MODULUS_LEN;
            KeyBuf = PublicKey->Blob;
            ResetCRmm();
            err = Pkcs7_x509_return_Cert_pubKey((UINT8*)KeyHndl.Blob, KeyHndl.BlobSize, &PKpubBuffer, &KeyLen);
            ResetCRmm();
#endif
            if(err) return EFI_SECURITY_VIOLATION;
        }
    } else
// FWKey : Rsa2048            
    if(!guidcmp(&KeyHndl.AlgGuid, &gEfiCertRsa2048Guid) ){
    //    pubkey : x509 (Pkcs7) - xtract nModulus    
        if(!guidcmp(&PublicKey->AlgGuid, &gEfiCertX509Guid) ){
#if CONFIG_PEI_PKCS7 == 1
            PKpubBuffer = &Hash[0];
            KeyLen = DEFAULT_RSA_KEY_MODULUS_LEN;
            KeyBuf = PublicKey->Blob;
            ResetCRmm();
            err = Pkcs7_x509_return_Cert_pubKey((UINT8*)PublicKey->Blob, PublicKey->BlobSize, &PKpubBuffer, &KeyLen);
            ResetCRmm();
#endif
            if(err) return EFI_SECURITY_VIOLATION;
        }
    } else {
        if(err) return EFI_SECURITY_VIOLATION;
    }

    err = MemCmp(PKpubBuffer, KeyBuf, KeyLen);
    
    return (!err ? EFI_SUCCESS:EFI_SECURITY_VIOLATION);
}
//-----------------------------------------------------------------------------------------------------------------------
// End PPI 
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
// Callback on MemInstalled
//-----------------------------------------------------------------------------------------------------------------------
#if 0
/**
 * The function will be invoked after memory initialized event
 *
 * @param PeiServices Pointer to the Pointer to the Pei Services Table.
 * @param NotifyDescriptor The notification descriptor for this notification event.
 * @param NullPpi Pointer to a NULL ppi.
 *
 * @retval EFI_SUCCESS
 */
EFI_STATUS NotifyAtPeiEnd (
    IN EFI_PEI_SERVICES             **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
    IN VOID                         *Ppi
)
{
    EFI_STATUS              Status;
    EFI_BOOT_MODE           BootMode;
    AMI_CRYPT_DIGITAL_SIGNATURE_PPI *AmiSigPpi;

    // WARNING. This module must always be launched from RAM after the boot mode type is finalized.
    Status = (*PeiServices)->GetBootMode( PeiServices, &BootMode );
    if(EFI_ERROR(Status) || BootMode == BOOT_ON_S3_RESUME) {
        return EFI_SUCCESS; // skip on S3 resume
    }
    // This may be called for multiple events
    Status = (*PeiServices)->LocatePpi(PeiServices, 
                                &gAmiDigitalSignaturePPIGuid, 
                                0, NULL, 
                                &AmiSigPpi);
    DEBUG ((DEBUG_INFO, "Locate AmiSigPPI (%r)\n", Status));
    if(EFI_ERROR(Status) || AmiSigPpi == NULL)
        (*PeiServices)->InstallPpi (PeiServices, &mPpiSigListVariable);
    return EFI_SUCCESS;
//    return (*PeiServices)->InstallPpi (PeiServices, &mPpiSigListVariable);
//    return Status;
}
#endif
/**
  This function is the entry point for this PEI.
  This module must always be launched from RAM after the boot mode type is finalized.

  @param[in]   FileHandle  Pointer to the file handle
  @param[in]   PeiServices Pointer to the PEI services table

  @retval     Status
**/

//dbg
//#include <Library/BaseCryptLib.h>

EFI_STATUS
EFIAPI
CryptoPei_Init (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
//    EFI_STATUS              Status;
//    EFI_BOOT_MODE           BootMode;

//DBG
//    VOID *Rsa = RsaNew();
//DBG

//    Status = (*PeiServices)->GetBootMode( PeiServices, &BootMode );
//    if(EFI_ERROR(Status) || BootMode == BOOT_ON_S3_RESUME) {
//        return Status; // skip Crypto PPI install on S3 resume
//    }
////////////////////////////////////////////////////////////////////////////////////
//
// Install AmiCryptDigitalSignature PPI
//
////////////////////////////////////////////////////////////////////////////////////
    return (*PeiServices)->InstallPpi (PeiServices, &mPpiSigListVariable);
//    return (*PeiServices)->NotifyPpi(PeiServices, mInstallCryptoPpiNotify);
}
