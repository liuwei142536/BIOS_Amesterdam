//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  Verify Aptio FW capsule integrity and performs other security checks
**/

#include <AmiDxeLib.h>
#include <Protocol/SecSmiFlash.h>
#include <Library/AmiBufferValidationLib.h>
#include <Ppi/FwVersion.h>
#include <Ffs.h>
#include "AmiCertificate.h"
#include <Library/DebugLib.h>

//----------------------------------------------------------------------------
// Function Externs
extern AMI_DIGITAL_SIGNATURE_PROTOCOL *gAmiSig;
extern UINTN     gFwCapMaxSize; // add 4k for capsule's header
// Now defined in AmiCryptoPkg (lbl 36+)
//extern EFI_GUID  gFWCapsuleGuid;
//extern EFI_GUID  gPRKeyGuid;

extern EFI_GUID  gFwCapFfsGuid;
extern EFI_GUID  gFwCapSectionGuid;

extern EFI_SHA256_HASH  *gHashTbl;
extern CRYPT_HANDLE  gpPubKeyHndl;
extern UINT8     gHashDB[SHA512_DIGEST_SIZE];

//----------------------------------------------------------------------------
// Local Variables

const UINT8 *FwBadKey = (const UINT8 *)"$BAD";

//----------------------------------------------------------------------------
// Local Defines

#ifndef AMI_FFS_COMMON_SECTION_HEADER
typedef struct _AMI_FFS_COMMON_SECTION_HEADER {
    EFI_FFS_FILE_HEADER FfsHdr;                         //< The FFS file header
    EFI_FREEFORM_SUBTYPE_GUID_SECTION GuidedSection;    //< The Guided Section of the FFS file
    APTIO_FW_CAPSULE_HEADER   pFvSigHeader[0];
} AMI_FFS_COMMON_SECTION_HEADER;
#endif

//----------------------------------------------------------------------------
// Local prototypes
static EFI_STATUS VerifyFwRevision (
    IN APTIO_FW_CAPSULE_HEADER      *FWCapsuleHdr,
    IN UINT8                        *RomData
);
EFI_STATUS CapsuleValidate (
    IN OUT UINT8                    **pFwCapsule,
    IN OUT VOID                     **pFWCapsuleHdr
);
EFI_STATUS FindCapHdrFFS(
    IN  VOID                        *pCapsule,
    IN  UINT32                      Size,
    OUT APTIO_FW_CAPSULE_HEADER     **pFwCapHdr
);

//----------------------------------------------------------------------------
#if IGNORE_RUNTIME_UPDATE_IMAGE_REVISION_CHECK == 0
//----------------------------------------------------------------------------
typedef struct _AMI_FID_SECTION {
    EFI_GUID   Guid;
    FW_VERSION FwVersion;
} AMI_FID_SECTION;

static EFI_GUID FidSectionGuid = \
    { 0x2EBE0275, 0x6458, 0x4AF9, 0x91, 0xed, 0xD3, 0xF4, 0xED, 0xB1, 0x00, 0xAA };

const UINT8 *FidSignature = (const UINT8 *)"$FID";
//----------------------------------------------------------------------------
// Function Definitions

/**
    Function to verify BIOS Tag and Project revision number

    @param[in]   FwVersionData  pointer to a data buffer to read from

    @retval      Status
**/
EFI_STATUS VerifyProjectId (
    IN FW_VERSION      *FwVersionData
)
{
/*
    CHAR8       BiosTag[9];     //BIOS Tag
    EFI_GUID    FirmwareGuid;       //Firmware GUID
    CHAR8       CoreMajorVersion[3];    
    CHAR8       CoreMinorVersion[3];
    CHAR8       ProjectMajorVersion[3];
    CHAR8       ProjectMinorVersion[3];
*/
// Project ID, Major, Minor rev
    char  *strOrgProjectId = CONVERT_TO_STRING(PROJECT_TAG);
    UINTN  Size = sizeof(CONVERT_TO_STRING(PROJECT_TAG));
    EFI_GUID FirmwareGuid = FW_VERSION_GUID;
DEBUG_CODE (
    char   strProjectId[sizeof(CONVERT_TO_STRING(PROJECT_TAG))];

    MemSet(&strProjectId, Size, 0);
    MemCpy( strProjectId, FwVersionData->BiosTag, Size-1);
    DEBUG ((SecureMod_DEBUG_LEVEL, "Org/New BiosTag: %a=%a\nOrg/New ProjVer: %d=%d\nOrg/New FwGuid :\n%g\n%g\n",
        strOrgProjectId, strProjectId,(PROJECT_MAJOR_VERSION*100+PROJECT_MINOR_VERSION),
        (Atoi(FwVersionData->ProjectMajorVersion)*100+Atoi(FwVersionData->ProjectMinorVersion)),
        &FirmwareGuid, &(FwVersionData->FirmwareGuid)
    ));
);
    if (!Size || MemCmp (strOrgProjectId, FwVersionData->BiosTag, Size-1)) return EFI_SECURITY_VIOLATION;
#if IGNORE_FID_FW_VERSION_GUID_CHECK == 0
    if(guidcmp(&FirmwareGuid, &(FwVersionData->FirmwareGuid)) ) return EFI_SECURITY_VIOLATION;
#endif

#if IGNORE_IMAGE_ROLLBACK == 0
    if((Atoi(FwVersionData->ProjectMajorVersion)*100+Atoi(FwVersionData->ProjectMinorVersion)) <
       (PROJECT_MAJOR_VERSION*100+PROJECT_MINOR_VERSION) 
     ){
        // Physically present user may override the roll-back protection from the Setup screen
        return EFI_INCOMPATIBLE_VERSION;
    }
#endif

    return EFI_SUCCESS;
}

/**
    Function to read FFS FID data structure from the given data buffer

    @param[in]   pFV  pointer to a data buffer to read from
    @param[in]   Size Capsule size

    @retval      FwVersionData   pointer to output buffer
    @retval      Status     EFI_SUCCESS if  if FID data is retrieved
**/
BOOLEAN GetFidData(
    IN  UINT8          *pFV,
    IN  UINT32          Size,
    OUT FW_VERSION    **FwVersionData
)
{
    UINT32 Signature;
    UINT8 *SearchPointer;
    AMI_FID_SECTION *Section;

    Signature = FidSectionGuid.Data1;
    for(SearchPointer = pFV; 
        SearchPointer < (UINT8*)((UINTN)pFV+Size);
        SearchPointer++) {
        Section = (AMI_FID_SECTION *)SearchPointer;
        if(Section->Guid.Data1 == Signature) {
            if(!guidcmp(&FidSectionGuid, &(Section->Guid)) && 
               (*((UINT32*)(&Section->FwVersion.FirmwareID[0])) == *(UINT32*)FidSignature)){ 
                *FwVersionData = &Section->FwVersion;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
    Verify Fw revision compatibility
    NewVer > OldVer, newProjectTAGid = oldProjectTAGid
    Local PEI $FID is linked with CspLib. extern FW_VERSION FwVersionData;
    Find $FID in new Fw FVs. Any found should do for us. Use RomMap from Capsule's Hdr
    compare local BB and Main $Fid BIOS Major/Minor revs with New one.
    
    @param[in]   FWCapsuleHdr   pointer to FvCapsule Header
    @param[in]   RomData        search data buffer

    @retval      Status     EFI_SUCCESS if FW revision test passes
**/
EFI_STATUS
VerifyFwRevision (
    IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr,
    IN UINT8 *RomData
)    
{
    ROM_AREA               *Area;
    UINT8                  *FvAddress;
    FW_VERSION             *FwVersionData;

    for (Area = (ROM_AREA *)((UINTN)FWCapsuleHdr+FWCapsuleHdr->RomLayoutOffset); 
            Area->Size != 0; Area++) {
        if (!(Area->Attributes & ROM_AREA_FV_SIGNED))
            continue;
        //  $FID can be in FV with either PEI or DXE
        if (!(Area->Attributes & (ROM_AREA_FV_PEI+ROM_AREA_FV_DXE)))
            continue;

        FvAddress = (UINT8*)((UINTN)RomData + Area->Offset);
        if (GetFidData(FvAddress, Area->Size, &FwVersionData)) {
            return VerifyProjectId(FwVersionData);
        }
    }
// At least one FW block must be signed OR no $FID structure found in the new FW image
    return EFI_INCOMPATIBLE_VERSION;
}

#endif // #if IGNORE_RUNTIME_UPDATE_IMAGE_REVISION_CHECK == 0

/**
    Verify Fw Capsule Hdr structure

    @param[in]   FWCapsuleHdr   pointer to FvCapsule Header
    @retval      BOOLEAN
**/
BOOLEAN IsFwCapHdrValid(
        IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr
)
{
    BOOLEAN Result;
    Result = (FWCapsuleHdr->FWCert.SignCert.Hdr.Hdr.wCertificateType != WIN_CERT_TYPE_EFI_GUID ||
            FWCapsuleHdr->CapHdr.CapsuleImageSize > FWCAPSULE_IMAGE_SIZE ||
            FWCapsuleHdr->CapHdr.HeaderSize > (FWCAPSULE_IMAGE_SIZE-FWCAPSULE_MAX_PAYLOAD_SIZE) || // 16k is a MAX possible FwCap Hdr size
            FWCapsuleHdr->CapHdr.HeaderSize > FWCapsuleHdr->CapHdr.CapsuleImageSize ||
            FWCapsuleHdr->CapHdr.HeaderSize != FWCapsuleHdr->RomImageOffset ||
            FWCapsuleHdr->RomImageOffset < (UINT64)FWCapsuleHdr->RomLayoutOffset + 2*sizeof(ROM_AREA) ||
            FWCapsuleHdr->RomLayoutOffset< (UINT64)FWCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength + offsetof(APTIO_FW_CAPSULE_HEADER, FWCert)
        );
    DEBUG ((SecureMod_DEBUG_LEVEL, "FwCap Hdr(%8lX): %a\n", FWCapsuleHdr, Result?"InValid":"Valid"));
    return !Result;
}
/**
    Locates FwCapsule Hdr inside GUIDed section of ffs

    @param[in]   pCapsule   Pointer to data buffer to read from
    @param[in]   Size       Capsule size
    @param[out]  pFwCapHdr  Pointer to start of FwCap Hdr inside a FFs file section

    @retval      Status     EFI_SUCCESS if Capsule Hdr with Signature is retrieved
**/
EFI_STATUS FindCapHdrFFS(
    IN  VOID    *pCapsule,
    IN  UINT32   Size,
    OUT APTIO_FW_CAPSULE_HEADER  **pFwCapHdr
)
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    UINT32 Signature;
    UINT32 *SearchPointer;
    AMI_FFS_COMMON_SECTION_HEADER *FileSection;
    APTIO_FW_CAPSULE_HEADER *FwCapHdr = NULL;

    Signature = gFwCapFfsGuid.Data1;
    for(SearchPointer = (UINT32 *)pCapsule;
        SearchPointer < (UINT32 *)((UINTN)pCapsule + Size - sizeof(AMI_FFS_COMMON_SECTION_HEADER));
        SearchPointer++) {

        if(*SearchPointer == Signature) {
            FileSection = (AMI_FFS_COMMON_SECTION_HEADER *)SearchPointer;
            if(FileSection->FfsHdr.Type == EFI_FV_FILETYPE_FREEFORM &&
               !guidcmp(&gFwCapFfsGuid, &(FileSection->FfsHdr.Name)) &&
               !guidcmp(&gFwCapSectionGuid, &(FileSection->GuidedSection.SubTypeGuid))
            ){
                FwCapHdr = (APTIO_FW_CAPSULE_HEADER*)(FileSection->pFvSigHeader);
                // just a sanity check - Cap Size must match the Section size
                if(((*(UINT32 *)FileSection->FfsHdr.Size) & 0xffffff) >=
                        FwCapHdr->CapHdr.HeaderSize + sizeof(AMI_FFS_COMMON_SECTION_HEADER) &&
                    !guidcmp((EFI_GUID*)&FwCapHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid)
                ){
                    *pFwCapHdr = FwCapHdr;
                    Status = EFI_SUCCESS;
                    break;
                }
            }
        }
    }
    DEBUG ((SecureMod_DEBUG_LEVEL, "Find FwCap Ffs: %r (%8lX)\n", Status, FwCapHdr));
    return Status;
}

/**
    The Rom image hash is calculated based on info from the Rom Area map

    @param[in]   FwCapsuleHdr - pointer to a FW Capsule Hdr
    @param[in]   Payload - pointer to a FW Image
    @param[in]   RomSize - Size of Rom Image
    @param[in]   HashAlgorithm
    @param[in]   gHashDB

    @retval      Status 
                 EFI_SUCCESS
                 EFI_SECURITY_VIOLATION
**/
EFI_STATUS HashFwRomMapImage (
    IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize,
    IN EFI_GUID                 *HashAlgorithm,
    IN OUT UINT8                *gHashDB
){
    EFI_STATUS  Status = EFI_SUCCESS;
    ROM_AREA    *RomAreaTbl;
    UINTN       *Addr;
    UINTN       *Len;
    UINTN        i, RomMap_size, max_num_elem, num_elem, max_hash_elem;

    RomAreaTbl = (ROM_AREA *)((UINTN)FWCapsuleHdr+FWCapsuleHdr->RomLayoutOffset);

    RomMap_size = FWCapsuleHdr->RomImageOffset-FWCapsuleHdr->RomLayoutOffset;
    max_num_elem = RomMap_size/sizeof(ROM_AREA);
// assume max size of RomMap array = RomMap_size/sizeof(ROM_AREA);
// or better yet ...calculate exact number
    num_elem = 0;
    for (i=0; i < max_num_elem && RomAreaTbl[i].Size != 0; i++ )
    {
        if (RomAreaTbl[i].Attributes & ROM_AREA_FV_SIGNED)
            num_elem++;
    }
    // at least one signed range must be included
    if(num_elem == 0) return EFI_SECURITY_VIOLATION;
    max_num_elem  = i;
    max_hash_elem = num_elem+2; // add 2 extra entries
//    DEBUG ((SecureMod_DEBUG_LEVEL, "Max Num elem %d\nNum Hash elem %d\n", max_num_elem, max_hash_elem));
    Addr = (UINTN*)gHashTbl;
    Len = (UINTN*)((UINTN)gHashTbl + max_hash_elem*sizeof(UINTN));
    num_elem = 0;
    for(i=0; i < max_num_elem && num_elem < max_hash_elem && RomAreaTbl[i].Size != 0; i++)
    {
        if(!(RomAreaTbl[i].Attributes & ROM_AREA_FV_SIGNED)) 
            continue;
    // sanity check for buffer overruns
        if(RomAreaTbl[i].Offset > RomSize ||
           ((UINT64)RomAreaTbl[i].Offset + RomAreaTbl[i].Size) > RomSize )
            return EFI_SECURITY_VIOLATION;
    // RomArea only holds offsets within a payload
        Addr[num_elem] = (UINTN)Payload + RomAreaTbl[i].Offset;
        Len[num_elem] = RomAreaTbl[i].Size;

        num_elem++;
    }
    if(num_elem >= max_hash_elem) return EFI_SECURITY_VIOLATION;
//
//  Hash of Capsule Hdr + FW Certificate Hdr
//
    if(FWCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_CAPHDR_IN_SIGNCERT) {
        Addr[num_elem] = (UINTN) FWCapsuleHdr;
        Len[num_elem] = (UINTN)&FWCapsuleHdr->FWCert.SignCert.CertData - (UINTN)FWCapsuleHdr;
        num_elem++;
        if(num_elem >= max_hash_elem) return EFI_SECURITY_VIOLATION;
    }
//
//  Hash of the ROM_MAP table
//
    Addr[num_elem] = (UINTN)RomAreaTbl;
    Len[num_elem] = (i+1)*sizeof(ROM_AREA);
    num_elem++;

    Status = gAmiSig->Hash(gAmiSig, HashAlgorithm/*&gEfiHashAlgorithmSha256Guid*/, 
                num_elem, (const UINT8**)Addr,  (const UINTN*)Len, gHashDB );

DEBUG_CODE (
    for (i=0; i<num_elem; i++) DEBUG ((SecureMod_DEBUG_LEVEL, "Hash Range %2d: %08lX:%08lX (%4X)\n", i+1, 
            ((UINTN)Addr[i]>=(UINTN)Payload)?((UINTN)Addr[i]-(UINTN)Payload):((UINTN)Addr[i]-(UINTN)FWCapsuleHdr), 
            ((UINTN)Addr[i]>=(UINTN)Payload)?((UINTN)Addr[i]+Len[i]-(UINTN)Payload):((UINTN)Addr[i]+Len[i]-(UINTN)FWCapsuleHdr),
            (UINT32)*(UINT32*)Addr[i]));
    for (i=0; i<16; i++) DEBUG ((SecureMod_DEBUG_LEVEL," %02X", (gHashDB[i]) ));
    DEBUG ((SecureMod_DEBUG_LEVEL, "\nFW Hash: %r\nSHA Algorithm: %g\n", Status, HashAlgorithm));
);
    return Status;
}

/**
    This code verifies FW Capsule is genuine,
     and performs following checks on the image:
      1. Signing certificate is signed with trusted Root Platform key
      2. Integrity check. Image Signature verification

    @param[in]   FwCapsuleHdr - pointer to a FW Capsule Hdr
    @param[in]   Payload pointer to a FW Image
    @param[in]   RomSize Size of Rom Image
    @param[in]   PubKeyHndl

    @retval      Status 
                 EFI_SUCCESS               The signature is successfully verified.
                 EFI_SECURITY_VIOLATION    The signature does not match the given message.
**/
EFI_STATUS VerifyFwCertPkcs7 (
    IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize
){
    EFI_STATUS              Status;
    UINT8                  *Pkcs7Cert, *pDigest, *TrustCert;
    UINTN                   Pkcs7Cert_len, DigestLen, CertSize;
    EFI_GUID               *HashAlgorithm;
    UINT8                   DigestAlgo;
    UINT8                  *pDigestAlgo;
    UINTN                   Size;

    EFI_CERT_X509_SHA256    HashCert;

    DEBUG ((SecureMod_DEBUG_LEVEL, "Verify Fw Pkcs7 Cert\n"));
//
// 1. Verify Platform Key algo matches x509 cert or Sha256 (possibly a hash of x509 cert)
//
    // FwKey is potentially a TimeStamped Hash of x509 tbs certificate data
    if(!guidcmp(&gpPubKeyHndl.AlgGuid, &gEfiCertSha256Guid)) {
        MemSet((UINT8*)&HashCert, sizeof(HashCert), 0);
        MemCpy(HashCert.ToBeSignedHash, gpPubKeyHndl.Blob, SHA256_DIGEST_SIZE);
        TrustCert = (UINT8*)&HashCert;
        CertSize = sizeof(EFI_CERT_X509_SHA256);
    } else 
        if(!guidcmp(&gpPubKeyHndl.AlgGuid, &gEfiCertX509Guid)) { 
            TrustCert = gpPubKeyHndl.Blob;
            CertSize  = gpPubKeyHndl.BlobSize;
        } else
            return EFI_UNSUPPORTED;

  pDigest = &gHashDB[0];
  Pkcs7Cert = (UINT8*)&FWCapsuleHdr->FWCert.SignCert.CertData; 
  Pkcs7Cert_len = FWCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength-sizeof(WIN_CERTIFICATE_UEFI_GUID_1);

  // get Digest Algorithm
  Size         = 0;  
  pDigestAlgo  = (UINT8*)&DigestAlgo;
  Status = gAmiSig->Pkcs7Verify( gAmiSig,
                      Pkcs7Cert, Pkcs7Cert_len,
                      NULL,
                      0,
                      &pDigestAlgo,
                      &Size,
                      Pkcs7GetDigestAlgorithm,
                      LOCK
                          );
  if (EFI_ERROR(Status)) 
    return Status;

  switch(DigestAlgo) {
      case SHA256: 
          HashAlgorithm = &gEfiHashAlgorithmSha256Guid;
          DigestLen = SHA256_DIGEST_SIZE;
      break;
      case SHA384: 
          HashAlgorithm = &gEfiHashAlgorithmSha384Guid;
          DigestLen = SHA384_DIGEST_SIZE;
      break;
      case SHA512: 
          HashAlgorithm = &gEfiHashAlgorithmSha512Guid;
          DigestLen = SHA512_DIGEST_SIZE;
      break;
      default: 
          DEBUG ((SecureMod_DEBUG_LEVEL, "Unsupported Hash Algorithm(1-3) %d\n", DigestAlgo));
          return EFI_UNSUPPORTED;
  }

// 2. Verify Signing Cert Signature
//
// 2.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(FWCapsuleHdr, Payload, RomSize, HashAlgorithm, gHashDB);
    if (EFI_ERROR(Status)) return Status;

// 2.2 Verify Fw Certificate
    return gAmiSig->Pkcs7Verify( gAmiSig,
                     Pkcs7Cert, Pkcs7Cert_len,           // Pkcs7Cert
                     TrustCert, CertSize,
                     &pDigest, &DigestLen,               // In/OutData
                     Pkcs7CertValidate,
                     RELEASE                             // Flags, mutex
                     );
}

/**
    This code verifies FW Capsule is genuine, 
     and performs following checks on the image:
      1. Signing certificate is signed with trusted Root Platform key
      2. Integrity check. Image Signature verification
    Validate  Root Certificate 
       -Compare PubKey in Root Certificate Hdr with local FW Platform Key
       -Hash SignCert.PubKey 
       -VerifySig for RootCert.Signature and compare with SignCert.Key hash
     Validate  Sign Certificate 
       -Hash the ROM image inside the FW Capsule
       -VerifySig for SignCert.Signature and compare with ROM hash

    @param[in]   FwCapsuleHdr - pointer to a FW Capsule Hdr
    @param[in]   Payload - pointer to a FW Image
    @param[in]   RomSize - Size of Rom Image
    @param[in]   FailedVTask   Specifies additional flags to further customize the signing/verifying behavior.

    @retval      Status 
                 EFI_SUCCESS               The signature is successfully verified.
                 EFI_SECURITY_VIOLATION    The signature does not match the given message.
**/
EFI_STATUS VerifyFwCertRsa2048Sha256 (
    IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr,
    IN UINT8                    *Payload,
    IN UINTN                     RomSize
){
    EFI_STATUS              Status;
    CRYPT_HANDLE            HashHndl;
    CRYPT_HANDLE            PubKeyHndl;
    UINT8                   *pSig;
    UINT32                  Flags;
    UINT8                   *Addr;
    UINTN                   Size;
    EFI_CERT_BLOCK_RSA_2048_SHA256* pRootCert;

    DEBUG ((SecureMod_DEBUG_LEVEL, "Verify Fw RSA2048_SHA256 Cert\n"));

    Flags = (FWCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_RSA_PSS_PADDING_SCHEME)?EFI_CRYPT_RSASSA_PSS:EFI_CRYPT_RSASSA_PKCS1V15;

    HashHndl.AlgGuid  = gEfiHashAlgorithmSha256Guid;
    HashHndl.BlobSize = SHA256_DIGEST_SIZE;
    HashHndl.Blob = gHashDB;

    PubKeyHndl.AlgGuid = gEfiCertRsa2048Guid;
    PubKeyHndl.BlobSize =  DEFAULT_RSA_KEY_MODULUS_LEN;
//
// 1. Compare Capsule's Sign Cert key with Platform Root Key
//
    PubKeyHndl.Blob = (UINT8*)FWCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
    Status = gAmiSig->VerifyKey(gAmiSig, &gPRKeyGuid, &PubKeyHndl);
    //  Skip the RootCert key checking if SignCert Key and PR Key are matching
    DEBUG ((SecureMod_DEBUG_LEVEL, "Compare SignCert Key == FW Key: %r\n", Status));
    if(EFI_ERROR(Status)) {
//
// 1.1 Compare Platform Root with Capsule's Key from a Root Key store
//
        for (pRootCert = &FWCapsuleHdr->FWCert.RootCert; 
             (UINTN)pRootCert < ((UINT64)(UINTN)&FWCapsuleHdr->FWCert+FWCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength) &&
                 pRootCert->PublicKey[0]!=0;
             pRootCert++) 
        {
            PubKeyHndl.Blob = (UINT8*)pRootCert->PublicKey;
            Status = gAmiSig->VerifyKey(gAmiSig, &gPRKeyGuid, &PubKeyHndl);
DEBUG ((SecureMod_DEBUG_LEVEL, "Compare RootCert Key == FW Key: %r (0x%4X, ..)\n", Status, (UINT32)*PubKeyHndl.Blob));
            if (EFI_ERROR(Status)) continue;
//
// 2. Verify RootCert.Signature
//
// 2.1  Compute FWCert.SignCert.PublicKey Hash
// 
            if(FWCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_SIGNKEY_IN_ROOTCERT)
            {
                Addr = (UINT8*)FWCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
                Size = DEFAULT_RSA_KEY_MODULUS_LEN;
            } else
// 2.2  Compute FWCert.SignCert Hash
            {
                Addr = (UINT8*)&FWCapsuleHdr->FWCert.SignCert;
                Size = sizeof(AMI_CERTIFICATE_RSA2048_SHA256);
            }

            Status = gAmiSig->Hash(gAmiSig, &gEfiHashAlgorithmSha256Guid, 1, (const UINT8**)&Addr,(const UINTN*)&Size, gHashDB); 
            if (EFI_ERROR(Status)) break;
        
            pSig = (void*)pRootCert->Signature; 
            Status = gAmiSig->Pkcs1Verify(gAmiSig, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags); 
DEBUG ((SecureMod_DEBUG_LEVEL, "Verify Root Cert: %r\n", Status));
            break;
        }
    }
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
//
// 3. Verify Signing Cert
//
// 3.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(FWCapsuleHdr, Payload, RomSize, &gEfiHashAlgorithmSha256Guid, gHashDB);
    if (EFI_ERROR(Status)) return Status;

    pSig = (void*)FWCapsuleHdr->FWCert.SignCert.CertData.Signature; 
    PubKeyHndl.Blob = (UINT8*)FWCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
    return gAmiSig->Pkcs1Verify(gAmiSig, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags); 
}

/**
    This code verifies FW Capsule is genuine, 
    and performs following checks on the image:
    1. FWCapsule Header format
    2. Signer Certificate key is chained to root Platform key
    3. Integrity check. Image Signature verification
    4. Re-Play protection. Verifies that new FW image version is newer then the current one

    @param[in]   pFwCapsule      Points to the start of the Aptio FW Capsule.

    @retval      pFwCapsule      ptr to a start of Capsule's Payload - ROM image
    @retval      pFWCapsuleHdr   return ptr to FwCap Hdr within a Capsule'a image
    @retval      Status 
                    EFI_SUCCESS               The signature is successfully verified.
                    EFI_SECURITY_VIOLATION    The signature does not match the given message.
                    EFI_ACCESS_DENIED         The key could not be used in signature operation.
                    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                                              of the underlying signature algorithm.
**/
EFI_STATUS CapsuleValidate (
    IN OUT UINT8     **pFwCapsule,
    IN OUT VOID      **pFWCapsuleHdr
){
    EFI_STATUS              Status;
    APTIO_FW_CAPSULE_HEADER *FWCapsuleHdr;
    UINTN                   RomSize;
    UINT8                   *Payload;

/*
 - CapsuleValidate 
     - Look up Capsule GUID
         - Found - 
            update pFwCapsule ptr to beginning of BIOS ROM data
            continue with Image Verify
         - Not found at offs 0 - assume Cap Hdr in FFS
            Call GetSigFFS
            locate FFS by Hole GUID, Sec GUID
             if found, update FWCapsuleHdr,
            continue with Image Verify
*/
// proper FW Capsule presence check
    if(pFwCapsule == NULL || *pFwCapsule == NULL)
        return EFI_SECURITY_VIOLATION;

    FWCapsuleHdr = (APTIO_FW_CAPSULE_HEADER*)*pFwCapsule;
    Payload = *pFwCapsule;

// verify Capsule Mailbox points to FW_CAPSULE hdr
    if(!guidcmp((EFI_GUID*)&FWCapsuleHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid))
    {
// Update Payload to point to beginning of Bios ROM
        Payload = (UINT8*)((UINTN)FWCapsuleHdr + FWCapsuleHdr->RomImageOffset);
        if ((UINTN)Payload < (UINTN)FWCapsuleHdr)
            return EFI_SECURITY_VIOLATION;

        *pFwCapsule = Payload;
    } else {
        DEBUG ((SecureMod_DEBUG_LEVEL, "Looking for embedded Signature...\n"));
        if(EFI_ERROR(FindCapHdrFFS(Payload, FWCAPSULE_MAX_PAYLOAD_SIZE, &FWCapsuleHdr)))
            return EFI_SECURITY_VIOLATION;
    }

    // More Hdr fields sanity checks for buffer overruns
    if(!IsFwCapHdrValid(FWCapsuleHdr))
        return EFI_SECURITY_VIOLATION;

    RomSize = FWCapsuleHdr->CapHdr.CapsuleImageSize - FWCapsuleHdr->RomImageOffset;
    DEBUG ((SecureMod_DEBUG_LEVEL, "Fw Capsule GUID %g\nPayload size 0x%X\n", &(FWCapsuleHdr->CapHdr.CapsuleGuid), RomSize));

    // More Hdr fields sanity checks for buffer overruns
    if(RomSize > gFwCapMaxSize)
        return EFI_SECURITY_VIOLATION;

    // update return argument with a ptr to FwCapHdr
    if(pFWCapsuleHdr)
        *pFWCapsuleHdr = (VOID*)FWCapsuleHdr;

    // Key buffer inside SMRAM
    if(EFI_ERROR(AmiValidateSmramBuffer(gpPubKeyHndl.Blob, gpPubKeyHndl.BlobSize))
    )
        return EFI_SECURITY_VIOLATION;

    // Skip integrity check if the dummy FWkey is detected
    // Only Capsule's Hdr structure in verified in this phase
    if(*((UINT32*)(gpPubKeyHndl.Blob)) == *(UINT32*)FwBadKey) {
        DEBUG ((SecureMod_DEBUG_LEVEL, "WARNING! FW Key store is empty. Skip further image validation\n"));
        return EFI_INCOMPATIBLE_VERSION;
    }
// Begin Authentication
    if(!guidcmp((EFI_GUID*)&FWCapsuleHdr->FWCert.SignCert.Hdr.CertType, &gEfiCertPkcs7Guid))
        Status = VerifyFwCertPkcs7(FWCapsuleHdr, Payload, RomSize);
    else
        Status = VerifyFwCertRsa2048Sha256(FWCapsuleHdr, Payload, RomSize);

    DEBUG ((SecureMod_DEBUG_LEVEL, "Signature verify: %r\n", Status));
    if (EFI_ERROR(Status)) return Status;

#if IGNORE_RUNTIME_UPDATE_IMAGE_REVISION_CHECK == 0
    Status = VerifyFwRevision(FWCapsuleHdr, Payload);
    DEBUG ((SecureMod_DEBUG_LEVEL, "FW Revision test: %r\n", Status));
#endif

    return Status;
}
