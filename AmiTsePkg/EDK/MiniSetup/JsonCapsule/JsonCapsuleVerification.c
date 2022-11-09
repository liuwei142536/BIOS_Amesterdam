//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include "JsonCapsuleVerification.h"


static AMI_DIGITAL_SIGNATURE_PROTOCOL *gDigitalSigProtocol = NULL;
static UINT64   gTimeOfSigningLong;
static EFI_GUID gCertType;
static UINTN    gImageDigestSize;
static UINT8    gImageDigest[MAX_DIGEST_SIZE];
static UINT32   gHashAlg;
static UINT8    *gRawData;
static UINTN    gRawDataSize;

VOID  EFIAPI SetupDebugPrint(IN CONST CHAR8  *Format, ...) ;
#if SUPPRESS_PRINT
    #define SETUP_DEBUG_TSE(format,...)
#else //Else of SUPPRESS_PRINT
#if BUILD_OS == BUILD_OS_LINUX
    #define SETUP_DEBUG_TSE(format,...) SetupDebugPrint(format, ##__VA_ARGS__)
#else //Else of BUILD_OS == BUILD_OS_LINUX
    #define SETUP_DEBUG_TSE(format,...) SetupDebugPrint(format, __VA_ARGS__)
#endif //End of BUILD_OS == BUILD_OS_LINUX
#endif

/**
  This function gets the database variable from NVRAM and return the buffer if available.
  @param[in]    Name      Name of the database variable
  @param[out]   VarSize   Variable size

  @retval       VOID*     Variable buffer
**/
VOID* GetJsonCapsuleSecurityDatabaseVariableEx (
    IN CHAR16 *Name,
    IN OUT UINTN *VarSize
    )
{
    VOID *Var = NULL;
    UINT32  Attributes = 0;
    *VarSize = 0;   
    
    Var = VarGetNvramName (  Name, 
            &gEfiImageSecurityDatabaseGuid, 
            &Attributes, 
            VarSize );
    
    SETUP_DEBUG_TSE("[TSE] JsonCapsule: In GetJsonCapsuleSecurityDatabaseVariableEx Processing Certificates from '%s', VarSize %d ,Var %x \n", Name, *VarSize, Var); 
    return Var;
}

/**
  Calculate hash of JsonCapsule image based on the Authenticode image hashing

  Caution: This function may receive untrusted input.
  JsonCapsule image is external input, so this function will validate its data structure
  within this image buffer before use.

  @param[in]    HashAlg   Hash algorithm type.

  @retval TRUE            Successfully hash image.
  @retval FALSE           Fail in hash image.

**/
BOOLEAN
HashCapsuleImage (
  IN UINT32              HashAlg
  )
{
    EFI_GUID                *EfiHashAlgorithmGuid;
    EFI_STATUS                EfiStatus;
    BOOLEAN                   Status;
    const UINT8             *addr[2];
    UINTN                    num_elem, len[2];


  Status        = FALSE;
  EfiStatus     = EFI_SECURITY_VIOLATION;

  //
  // Initialize context of hash.
  //
  ZeroMem (gImageDigest, MAX_DIGEST_SIZE);
  switch(HashAlg) {
      case HASHALG_SHA1:
          gImageDigestSize  = SHA1_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha1Guid;
          CopyMem (&gCertType, &gEfiCertSha1Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA256:
          gImageDigestSize  = SHA256_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha256Guid;
          CopyMem (&gCertType, &gEfiCertSha256Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA384:
          gImageDigestSize  = SHA384_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha384Guid;
          CopyMem (&gCertType, &gEfiCertSha384Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA512:
          gImageDigestSize  = SHA512_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha512Guid;
          CopyMem (&gCertType, &gEfiCertSha512Guid, sizeof (EFI_GUID));
          break;
      default:
        return FALSE;
  }
 
  
  gHashAlg = HashAlg;

  addr[0] = gRawData;
  len[0] = gRawDataSize;
  num_elem = 1;
  
  EfiStatus = gDigitalSigProtocol->Hash(
              gDigitalSigProtocol, 
              EfiHashAlgorithmGuid, num_elem, addr, len, (UINT8*)&gImageDigest); 

  if(!EFI_ERROR(EfiStatus))
    Status = TRUE;

  return Status;
}

/**
  Recognize the Hash algorithm in JsonCapsule Authenticode and re-calculate hash of
  JsonCapsule.

  Caution: This function may receive untrusted input.
  JsonCapsule image is external input, so this function will validate its data structure
  within this image buffer before use.

  @param[in]  AuthData            Pointer to the Authenticode Signature retrieved from signed image.
  @param[in]  AuthDataSize        Size of the Authenticode Signature in bytes.
  
  @retval EFI_UNSUPPORTED             Hash algorithm is not supported.
  @retval EFI_SUCCESS                 Hash successfully.

**/
EFI_STATUS
HashImageByType (
  IN UINT8              *AuthData,
  IN UINTN              AuthDataSize
  )
{
  EFI_STATUS                Status;
  UINT8                     DigestAlgo;
  UINT8                    *pDigestAlgo;
  UINTN                     Size;

// get Digest Algorithm
  Size         = 0;  
  pDigestAlgo  = (UINT8*)&DigestAlgo;
  Status = gDigitalSigProtocol->Pkcs7Verify (
         gDigitalSigProtocol,
         AuthData,
         AuthDataSize,
         NULL,
         0,
         &pDigestAlgo,        // returns DER Ptr to Sign Cert
         &Size,
         Pkcs7GetDigestAlgorithm,
         LOCK
         );
  if (EFI_ERROR(Status)) 
    return Status;

  switch(DigestAlgo) {
    case HASHALG_SHA256:
          if (!HashCapsuleImage (HASHALG_SHA256)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    case HASHALG_SHA384:
          if (!HashCapsuleImage (HASHALG_SHA384)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    case HASHALG_SHA512:
          if (!HashCapsuleImage (HASHALG_SHA512)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    default:
        Status = EFI_UNSUPPORTED;
        break;
  }

  return Status;
}

/**
  Verify PKCS#7 SignedData using certificate found in Variable which formatted
  as EFI_SIGNATURE_LIST. The Variable may be PK, KEK, DB or DBX.

  @param[in]  AuthData             Pointer to the Authenticode signature retrieved from the signed image.
  @param[in]  AuthDataSize         Size of the Authenticode signature in bytes.
  @param[in]  VariableName         Name of Variable to search for Certificate.

  @retval TRUE         Image pass verification.
  @retval FALSE        Image fail verification.

**/
BOOLEAN
IsPkcs7SignedDataVerifiedBySignatureList (
  IN UINT8          *AuthData,
  IN UINTN           AuthDataSize,
  IN CHAR16         *VariableName,
  IN UINT8           Operation
  )
{
    EFI_STATUS                Status;
    EFI_SIGNATURE_LIST        *CertList;
    EFI_SIGNATURE_DATA        *Cert;
    UINT32                    CertCount;
    UINT32                    Index;
    UINT8                     *Data;
    UINTN                     DataSize;
    UINT8                     *RootCert;
    UINTN                     RootCertSize;
    UINT8                     *SigCert;
    UINTN                     SigCertSize;
    BOOLEAN                   IsVerified;
    EFI_TIME                 *TimeStamp;
    UINT64                    TimeOfRevocationLong;
    UINT8                     ValidCertType;
    BOOLEAN                   bVerifyTimeStampStatus;
    BOOLEAN                   bProcessVerifyTimeStampStatus;
    
    CertList              = NULL;
    Cert                  = NULL;
    RootCert              = NULL;
    TimeStamp             = NULL;
    RootCertSize          = 0;
    IsVerified            = FALSE;
    ValidCertType         = CertUndefined;
    bVerifyTimeStampStatus= FALSE;
    bProcessVerifyTimeStampStatus = FALSE;
    
    gTimeOfSigningLong    = 0;
    TimeOfRevocationLong = 0;
    
    // Verify the certificate's header
    //
    // Find certificate in store and verify authenticode struct.
    //
    Data = GetJsonCapsuleSecurityDatabaseVariableEx (VariableName, &DataSize);

    if (!Data) 
        return IsVerified;

    CertList = (EFI_SIGNATURE_LIST *)Data; 

    //
    // TO DO Optimization. 
    //  1. Find Root CA Cert Ptr or Signer Cert if Root is not found
    //  2. loop thru Trust Cert list and compare each one against Root CA cert
    //     (verify SignCert signature with Trust Cert)
    //  3. If found, break the 'while' loop and Pkcs7Verify with Trust Cert
    //  
    //
    // Find Cert Enrolled in database to verify the signature in pkcs7 signed data.
    // 
    // here should be a loop thru Cert list till CertCount
    while ((DataSize > 0) && (DataSize >= CertList->SignatureListSize)) {

        ValidCertType = CertUndefined;

        if(CompareGuid(&CertList->SignatureType, &gEfiCertX509Guid))
            ValidCertType = CertX509;
        else
            if(CompareGuid(&CertList->SignatureType, &gCertType) &&
                gImageDigestSize  >= SHA256_DIGEST_SIZE ) // one of SHA256,SHA384 or SHA512
                ValidCertType = CertSha256;
            else
                if(CompareGuid(&CertList->SignatureType, &gEfiCertX509Sha256Guid))
                    ValidCertType = CertX509Sha256;
                else
                    if(CompareGuid(&CertList->SignatureType, &gEfiCertX509Sha384Guid))
                        ValidCertType = CertX509Sha384;
                    else
                        if(CompareGuid(&CertList->SignatureType, &gEfiCertX509Sha512Guid))
                            ValidCertType = CertX509Sha512;

        if (ValidCertType != CertUndefined) {
            Cert       = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList + sizeof (EFI_SIGNATURE_LIST) + CertList->SignatureHeaderSize);
            CertCount  = (CertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - CertList->SignatureHeaderSize) / CertList->SignatureSize;
            RootCertSize  = CertList->SignatureSize-sizeof(EFI_GUID); // sig data structure starts with SigOwner Guid
            SETUP_DEBUG_TSE ("Cert Type %g (enum type %d)\nCertSize = 0x%X\n", &CertList->SignatureType, ValidCertType, RootCertSize);

            for (Index = 0; Index < CertCount; Index++) {
              //
              // Iterate each Signature Data Node within this CertList for verify.
              //
                RootCert = Cert->SignatureData;
                // is Sha256/Sha384/Sha512
                switch(ValidCertType) {
                case CertSha256:
                    if(RootCertSize == gImageDigestSize && 
                        MemCmp(RootCert, gImageDigest, gImageDigestSize) == 0) {
                      //
                      // Found the signature in database.
                      //
                        IsVerified = TRUE;
                        goto Exit;
                    }
                    SETUP_DEBUG_TSE ("Got SHAxxx Cert(%d): match=%d, size=%d, [%04X],..)\n", gHashAlg, IsVerified, gImageDigestSize, *(UINT32*)RootCert);
                    break;
                // x509, x509_ShaXXX
                /*
                Revocation is true:
                - return success && Time = 0
                - return success && Time != 0 && VerifyTimeStampStatus
                - success 
                if success but mVerifyTimeStampStatus = ACCESS_DENIED = return FAIL
                timestamp non-0 Year > 0
                */
                case CertX509Sha256:
                case CertX509Sha384:
                case CertX509Sha512:                    
                case CertX509:                  
                        SETUP_DEBUG_TSE ("Got X509_SHAxxx Cert ...");
                    
                    SETUP_DEBUG_TSE ("X.509, Proceed to Pkcs7Verify...\n");
                    //
                    //
                    // Verify Authenticode struct for image's current certificate.
                    //
                    // using protocol
                    SigCert       = gImageDigest;
                    SigCertSize   = gImageDigestSize;
                    Status = gDigitalSigProtocol->Pkcs7Verify (
                            gDigitalSigProtocol,
                         AuthData,
                         AuthDataSize,
                         RootCert,
                         RootCertSize,
                        // mDigest, DigestLen
                         &SigCert,
                         &SigCertSize,
                         Operation,             // Operation; Pkcs7CertValidate OR Pkcs7CertGetMatchInCertChain
                         KEEP                   // Flags 
                         );
                    SETUP_DEBUG_TSE ("====> %r\n", Status);
                    if (!EFI_ERROR(Status)) {
                        IsVerified = TRUE; // found matching certificate in the image
                        if(Operation == Pkcs7CertValidate) {
                             break;
                        }
                        if(Operation == Pkcs7TimeStampCertValidateGet) {
                            gTimeOfSigningLong = SigCertSize;
                            break;
                        }
                      
                        if(ValidCertType == CertX509Sha256)
                            TimeStamp = &((EFI_CERT_X509_SHA256*)RootCert)->TimeOfRevocation;
                        else
                            if(ValidCertType == CertX509Sha384)
                                TimeStamp = &((EFI_CERT_X509_SHA384*)RootCert)->TimeOfRevocation;
                            else
                                if(ValidCertType == CertX509Sha512)
                                    TimeStamp = &((EFI_CERT_X509_SHA512*)RootCert)->TimeOfRevocation;
                                else
                                    break;

                        if(EFI_ERROR(AmiTimeToSec(TimeStamp, &TimeOfRevocationLong)))
                            TimeOfRevocationLong = 0;

                         // skip dbt check if Time of Revocation = 0
                        if(TimeOfRevocationLong != 0) {
                         // Check the timestamp cert validity the valid certificate in allowed database (dbt).
                            if(!bProcessVerifyTimeStampStatus) {
                                 bVerifyTimeStampStatus = IsPkcs7SignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE2, Pkcs7TimeStampCertValidateGet); 
                                 bProcessVerifyTimeStampStatus = TRUE;
                            }
                            SETUP_DEBUG_TSE ("\nTimeStamp cert validate %a\nTimeOfSigning %X\nTimeOfRevocation %X\n",  
                                                 (!bVerifyTimeStampStatus?"FAIL":"PASS"), gTimeOfSigningLong, TimeOfRevocationLong);
                             // revoke Image if cert match in dbx but failed dbt!
                            if(!bVerifyTimeStampStatus) {
                                //IsVerified = TRUE;  
                                break;
                            }
                             // keep looking for cert with sign time passed revocation
                            if(gTimeOfSigningLong< TimeOfRevocationLong) 
                                IsVerified = FALSE;
                        }
                    }
                    break;
                } // switch
                if (IsVerified) {
                    goto Exit;
                }
                Cert = (EFI_SIGNATURE_DATA *) ((UINT8 *) Cert + CertList->SignatureSize);
            }  // end for
        } // end If 

        DataSize -= CertList->SignatureListSize;
        CertList = (EFI_SIGNATURE_LIST *) ((UINT8 *) CertList + CertList->SignatureListSize);
    } // end while
Exit:
    FreePool (Data);
    SETUP_DEBUG_TSE ("\n<===%a cert match in '%s'\n",(IsVerified?"Got":"No"), VariableName);
    return IsVerified;
}

/**
  Advanced check in Signature Database
  Check whether signature is located in target database

  @param[in]  Signature           Pointer to signature that is searched for.
  @param[in]  SignatureSize       Size of Signature.
  @param[in]  SignatureType       Type of the Certificate, Guid

  @return TRUE - found in IsSigForbidden            
  @return FALSE            

**/
BOOLEAN
IsSignatureFoundInCertDatabase (
  IN CHAR16          *Name,
  EFI_GUID           *SignatureType,
  IN UINT8           *Signature, 
  IN UINTN            SignatureSize
  )
{
  EFI_SIGNATURE_LIST  *CertList;
  EFI_SIGNATURE_DATA  *Cert;
  UINT8               *Data;
  UINTN               DataSize;
  UINTN               Index;
  UINTN               CertCount;
  BOOLEAN             IsSigFound;

  //
  // Read signature database variable.
  //
  IsSigFound   = FALSE;

  //
  // Get Signature Database variable.
  //

  Data = GetJsonCapsuleSecurityDatabaseVariableEx (Name, &DataSize);

  if (Data != NULL) {
// not an error if no "dbx" defined
      //
      // Enumerate all signature data in SigDB to check if executable's signature exists.
      //
      CertList = (EFI_SIGNATURE_LIST *) Data;
      while (!IsSigFound && (DataSize > 0) && (DataSize >= CertList->SignatureListSize)) {
        if (CompareGuid(&CertList->SignatureType, SignatureType)) { // Cert types do match 
            CertCount = (CertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - CertList->SignatureHeaderSize) / CertList->SignatureSize;
            Cert      = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList + sizeof (EFI_SIGNATURE_LIST) + CertList->SignatureHeaderSize);
            if (CertList->SignatureSize == (sizeof(EFI_SIGNATURE_DATA) - 1 + SignatureSize)) {
              for (Index = 0; Index < CertCount; Index++) {
                if (MemCmp(Cert->SignatureData, Signature, SignatureSize) == 0) {
                  //
                  // Find the signature in database.
                  //
                  IsSigFound = TRUE;
                  break;
                }
                Cert = (EFI_SIGNATURE_DATA *) ((UINT8 *) Cert + CertList->SignatureSize);
              }
            }
        } // next CertList
        DataSize -= CertList->SignatureListSize;
        CertList = (EFI_SIGNATURE_LIST *) ((UINT8 *) CertList + CertList->SignatureListSize);
      }
    FreePool (Data);
  }

  return IsSigFound;
}


/**
  This function gets the signature from the input capsule buffer and validate the signature 
  from the SecureBoot database. User has to register their certificate, using which they signed
  capsule file, into SecureBoot database.

  @param[in]  Image           Capsule buffer
  @param[in]  ImageSize       Capsule size

  @return EFI_SUCCESS       - Verification success
  @return EFI_UNSUPPORTED   - DigitalSignature Protocol not available to validate.            
  @return EFI_SUCCESS       - Verification failed
**/
EFI_STATUS 
ValidateImage(
    VOID* Image, 
    UINTN ImageSize
    )
{
    AMI_FFS_COMMON_SECTION_HEADER *FwCapsuleFfs = (AMI_FFS_COMMON_SECTION_HEADER *)NULL;
    EFI_JSON_CAPSULE_HEADER *JsonCapsuleHdr = (EFI_JSON_CAPSULE_HEADER *)NULL;
    APTIO_FW_CAPSULE_HEADER *FwCapsuleHdr = (APTIO_FW_CAPSULE_HEADER *)NULL;
    EFI_CAPSULE_HEADER *CapsuleHdr = (EFI_CAPSULE_HEADER *)Image;
    UINT8 *AuthData = (UINT8*)NULL;
    UINTN AuthDataSize = 0;
    EFI_IMAGE_EXECUTION_ACTION Action = 0;
    EFI_STATUS VerifyStatus = EFI_ACCESS_DENIED;
    EFI_STATUS Status;
    UINT32 HashAlg;
    
    Status = gBS->LocateProtocol( &gAmiDigitalSignatureProtocolGuid, NULL, (VOID**)&gDigitalSigProtocol );
    if(EFI_ERROR(Status))
        return EFI_UNSUPPORTED;

    JsonCapsuleHdr = (EFI_JSON_CAPSULE_HEADER *)((UINT8*)CapsuleHdr + sizeof(EFI_CAPSULE_HEADER));
    
    if(CapsuleHdr->CapsuleImageSize <= (sizeof(EFI_CAPSULE_HEADER) + sizeof(EFI_JSON_CAPSULE_HEADER) + JsonCapsuleHdr->PayloadLength))
        return VerifyStatus; // Capsule doesn't have AmiFwCapsuleFfs which contains the certificate
    
    
    FwCapsuleFfs = (AMI_FFS_COMMON_SECTION_HEADER *) ((UINT8*)JsonCapsuleHdr + sizeof(EFI_JSON_CAPSULE_HEADER) + JsonCapsuleHdr->PayloadLength);
    FwCapsuleHdr = (APTIO_FW_CAPSULE_HEADER *)FwCapsuleFfs->FwCapHdr;
    
    //The ceritifcation should be of type PKCS#7
    if((FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.wCertificateType == WIN_CERT_TYPE_PKCS_SIGNED_DATA) ||
            ((FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.wCertificateType == WIN_CERT_TYPE_EFI_GUID) && CompareGuid(&FwCapsuleHdr->FWCert.SignCert.Hdr.CertType, &gEfiCertPkcs7Guid)))
    {
        AuthData = (UINT8*)&FwCapsuleHdr->FWCert.SignCert.CertData;
        AuthDataSize = FwCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength - sizeof(FwCapsuleHdr->FWCert.SignCert.Hdr);
    }
    else
    {
        return VerifyStatus;
    }
    
    
    // Update the details of actual data in the capsule for which hashing has to be verified.
    gRawData = (UINT8*)CapsuleHdr + sizeof(EFI_CAPSULE_HEADER);
    gRawDataSize = sizeof(EFI_JSON_CAPSULE_HEADER) + JsonCapsuleHdr->PayloadLength;
    
    
    //
    //Starting Signature verification
    //
    
    
    //
    // Verify Pkcs signed data type.
    //
    // At least 1 Cert from Pe Image should be in DB
    // None of Certs should be found in DBX 
    //
    // get Digest Algorithm, set gCertType
    // lock mutex, preserve Pkcs7 context
    Status = HashImageByType ((UINT8*)AuthData, AuthDataSize);
    if (EFI_ERROR (Status)) {
        return VerifyStatus;
    }
    
    //
    // Check the digital signature against the revoked certificate in forbidden database (dbx).
    //
    if (IsPkcs7SignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE1, Pkcs7CertGetMatchInCertChain)) {

        Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND;
        SETUP_DEBUG_TSE ("Image is signed but it's revoked by DBX.\n");
        VerifyStatus = EFI_ACCESS_DENIED;
        return VerifyStatus;
    } else {
        //
        // Check the digital signature against the valid certificate in allowed database (db).
        //
        if (EFI_ERROR (VerifyStatus)) {
            if(IsPkcs7SignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE, Pkcs7CertValidate)) {
                VerifyStatus = EFI_SUCCESS;
            } else {
                SETUP_DEBUG_TSE ("Image is signed but signature is not allowed by DB.\n");
            }
        }
    }

    // clear context
    Status = gDigitalSigProtocol->Pkcs7Verify (
            gDigitalSigProtocol,
         AuthData, AuthDataSize,
         NULL, 0,
         NULL, NULL,
         Pkcs7Arg0,             // Dummy op
         RESET                  // Flags 
         );


    // Image not Signed or 
    // Image is Signed, its Cert verified in db but not found in DBX
    if( Action != EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND) {
        //
        // This image is not signed or signed but found only in db or failed previous checks.
        // Expected:
        // The SHA256 hash value of the image must match a record in the security database "db",
        // and not be reflected in the security data base "dbx".
        //
        SETUP_DEBUG_TSE ("IMAGE not signed or\n\tsigned but x509 Cert not found in DBX\n\tTest image's PE Hash certificate...\n");
        // loop through all supported digest algorithms
        for (HashAlg = HASHALG_SHA256; HashAlg < HASHALG_MAX; HashAlg++) {

            if(gHashAlg != HashAlg) {        // skip if gImageDigest matching HashAlg already calculated
                if(!HashCapsuleImage (HashAlg))   // init gCertType and calculate PE hash if not done already
                    break;
            }

            if(IsSignatureFoundInCertDatabase (EFI_IMAGE_SECURITY_DATABASE1, &gCertType, gImageDigest, gImageDigestSize)) {
                //
                // Image Hash in forbidden database (DBX)
                //
                VerifyStatus = EFI_ACCESS_DENIED;
                Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND;
                break;
            } else {
                // no - try in allowed db if not there already
                if(EFI_ERROR (VerifyStatus) && 
                        IsSignatureFoundInCertDatabase (EFI_IMAGE_SECURITY_DATABASE, &gCertType, gImageDigest, gImageDigestSize)) {
                    //
                    // Image Hash is in allowed database (DB).
                    //
                    VerifyStatus = EFI_SUCCESS;
                }
            }
        }
    }
    
    return VerifyStatus;
}

