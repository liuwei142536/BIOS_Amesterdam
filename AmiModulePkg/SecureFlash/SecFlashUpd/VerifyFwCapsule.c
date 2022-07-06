//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  PEI secured recovery services
  File contains VerifyFwCapsule hook to evaluate Fw Capsule
**/

//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include "Efi.h"
#include "Pei.h"
#include "Token.h"
#include <AmiPeiLib.h>
#include <Hob.h>
#include <Ffs.h>
#include <FlashUpd.h>
#include <PPI/CryptoPPI.h>
#include <Ppi/FwVersion.h> 
#include "AmiCertificate.h"
#include <Protocol/AmiDigitalSignature.h> // EFI_CERT_X509_SHAXXX
#include <Library/DebugLib.h>

//----------------------------------------------------------------------------
// Function Externs

//----------------------------------------------------------------------------
// Local prototypes

//----------------------------------------------------------------------------
typedef struct {
    EFI_FFS_FILE_HEADER FfsHdr;
    EFI_COMMON_SECTION_HEADER SecHdr;
    EFI_GUID            SectionGuid;
    UINT8               FwCapHdr[0];
} AMI_FFS_COMMON_SECTION_HEADER;

typedef struct _AMI_FID_SECTION {
    EFI_GUID   Guid;
    FW_VERSION FwVersion;
} AMI_FID_SECTION;

//----------------------------------------------------------------------------
// Local Variables
static EFI_GUID gFwCapFfsGuid     = AMI_FW_CAPSULE_FFS_GUID;
static EFI_GUID gFwCapSectionGuid  = AMI_FW_CAPSULE_SECTION_GUID;

static EFI_GUID FidSectionGuid   = \
    { 0x2EBE0275, 0x6458, 0x4AF9, 0x91, 0xed, 0xD3, 0xF4, 0xED, 0xB1, 0x00, 0xAA };

const UINT8 *FwBadKey = (const UINT8 *)"$BAD";
const UINT8 *FidSignature = (const UINT8 *)"$FID";

CONST EFI_PEI_SERVICES  **gPeiServices;
static AMI_CRYPT_DIGITAL_SIGNATURE_PPI *gpAmiSigPPI=NULL;

static UINT8 gHashDB[SHA512_DIGEST_SIZE];
// Allocate Hash Descr table
static UINTN *gAddrList=NULL;
static UINTN *gLenList=NULL;
static UINTN gHashNumElem=0;

//----------------------------------------------------------------------------
// Function Definitions

/**
    Function to verify BIOS Tag and Project revision number

    @param[in]   FwVersionData  pointer to a data buffer to read from

    @retval      FailedVTask Task status bitmap
    @retval      Status
**/
EFI_STATUS VerifyProjectId (
    IN FW_VERSION      *FwVersionData,
    IN OUT UINT32      *FailedVTask
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

    *FailedVTask = 0;
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

    @retval      FailedVTask Task status bitmap
    @retval      Status      EFI_SUCCESS if FW revision test passes
**/
EFI_STATUS
VerifyFwRevision (
    IN APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr,
    IN UINT8           *RomData,
    IN OUT UINT32      *FailedVTask
)
{
    ROM_AREA               *Area;
    UINT8                  *FvAddress;
    FW_VERSION             *FwVersionData;

    *FailedVTask = Ver;

    for (Area = (ROM_AREA *)((UINTN)FWCapsuleHdr+FWCapsuleHdr->RomLayoutOffset);
            Area->Size != 0; Area++) {
        if (!(Area->Attributes & ROM_AREA_FV_SIGNED))
            continue;
        //  $FID can be in FV with either PEI or DXE
        if (!(Area->Attributes & (ROM_AREA_FV_PEI+ROM_AREA_FV_DXE)))
            continue;

        FvAddress = (UINT8*)((UINTN)RomData + Area->Offset);
        if (GetFidData(FvAddress, Area->Size, &FwVersionData)) {
            return VerifyProjectId(FwVersionData, FailedVTask);
        }
    }
// At least one FW block must be signed OR no $FID structure found in the new FW image
    return EFI_SECURITY_VIOLATION;
}

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
    DEBUG ((SecureMod_DEBUG_LEVEL, "FwCapHdr: %a\n", Result?"InValid":"Valid"));
    return !Result;
}
/**
    Function to read FW Cap Sig data from Ffs

    @param[in]   pCapsule   pointer to data buffer to read from
    @param[in]   Size       Capsule size

    @retval      pFwCapHdr  pointer to start of FwCap Hdr inside a FFs file section
    @retval      Status     EFI_SUCCESS if Capsule Hdr with Signature is retrieved
**/
EFI_STATUS FindCapHdrFFS(
    IN  VOID    *pCapsule,
    IN  UINT32   Size,
    OUT APTIO_FW_CAPSULE_HEADER  **pFwCapHdr
)
{
    UINT32 Signature;
    UINT32 *SearchPointer;
    AMI_FFS_COMMON_SECTION_HEADER *FileSection;
    APTIO_FW_CAPSULE_HEADER *FwCapHdr;

    Signature = gFwCapFfsGuid.Data1;
    for(SearchPointer = (UINT32 *)pCapsule;
        SearchPointer < (UINT32 *)((UINTN)pCapsule + Size - sizeof(AMI_FFS_COMMON_SECTION_HEADER));
        SearchPointer++) {

        if(*SearchPointer == Signature) {
            FileSection = (AMI_FFS_COMMON_SECTION_HEADER *)SearchPointer;
            if(!guidcmp(&gFwCapFfsGuid, &(FileSection->FfsHdr.Name)) &&
               !guidcmp(&gFwCapSectionGuid, &(FileSection->SectionGuid))
            ){
                FwCapHdr = (APTIO_FW_CAPSULE_HEADER*)(FileSection->FwCapHdr);
                // just a sanity check - Cap Size must match the Section size
                if(((*(UINT32 *)FileSection->FfsHdr.Size) & 0xffffff) >=
                        FwCapHdr->CapHdr.HeaderSize + sizeof(AMI_FFS_COMMON_SECTION_HEADER) &&
                    !guidcmp((EFI_GUID*)&FwCapHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid)
                ){
                    *pFwCapHdr = FwCapHdr;
                    return EFI_SUCCESS;
                }
            }
        }
    }

    return EFI_NOT_FOUND;
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

    UINTN    i, RomMap_size, max_num_elem, num_elem;

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
    max_num_elem=i;
    num_elem+=2; // add 2 extra entries
    if(!gAddrList || !gHashNumElem || gHashNumElem<num_elem) {
        gHashNumElem = num_elem;
        i = num_elem*sizeof(UINTN);
        Status = (*gPeiServices)->AllocatePool(gPeiServices, i*2, (void**)&gAddrList);
        ASSERT_PEI_ERROR (gPeiServices, Status);
        if(EFI_ERROR(Status)) return Status;
        gLenList = (UINTN*)((UINTN)gAddrList + i);
    }
    num_elem = 0;
    for(i=0; i < max_num_elem && num_elem < gHashNumElem && RomAreaTbl[i].Size != 0; i++)
    {
        if(!(RomAreaTbl[i].Attributes & ROM_AREA_FV_SIGNED)) 
            continue;
    // sanity check for buffer overruns
        if(RomAreaTbl[i].Offset > RomSize ||
           ((UINT64)RomAreaTbl[i].Offset + RomAreaTbl[i].Size) > RomSize )
            return EFI_SECURITY_VIOLATION;
    // RomArea only holds offsets within a payload
        gAddrList[num_elem] = (UINTN)Payload + RomAreaTbl[i].Offset;
        gLenList[num_elem] = RomAreaTbl[i].Size;

        num_elem++;
    }
    if(num_elem >= gHashNumElem) return EFI_SECURITY_VIOLATION;
//
//  Hash of Capsule Hdr + FW Certificate Hdr
//
    if(FWCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_CAPHDR_IN_SIGNCERT) {
        gAddrList[num_elem] = (UINTN) FWCapsuleHdr;
        gLenList[num_elem] = (UINTN)&FWCapsuleHdr->FWCert.SignCert.CertData - (UINTN)FWCapsuleHdr;
        num_elem++;
        if(num_elem >= gHashNumElem) return EFI_SECURITY_VIOLATION;
    }
//
//  Hash of the ROM_MAP table
//
    gAddrList[num_elem] = (UINTN)RomAreaTbl;
    gLenList[num_elem] = (i+1)*sizeof(ROM_AREA);
    num_elem++;

    Status = gpAmiSigPPI->Hash(gpAmiSigPPI, HashAlgorithm,
                 num_elem, (const UINT8**)gAddrList,  (const UINTN*)gLenList, gHashDB );

    DEBUG ((SecureMod_DEBUG_LEVEL, "FW Hash: %r\nSHA Algorithm: %g\n", Status, HashAlgorithm));

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
    IN UINTN                     RomSize,
    IN PEI_CRYPT_HANDLE         *PubKeyHndl,
    IN OUT UINT32               *FailedVTask
){
    EFI_STATUS              Status;
    UINT8                  *Pkcs7Cert, *pDigest, *TrustCert;
    UINTN                   Pkcs7Cert_len, DigestLen, CertSize;
    EFI_GUID               *HashAlgorithm;
    HASH_ALG                DigestAlgo=SHA256;
    EFI_CERT_X509_SHA256    HashCert;
//
// 1. Validate Root Key
//
    if( FailedVTask==NULL )
        return EFI_SECURITY_VIOLATION;

    *FailedVTask = Key;

    if( PubKeyHndl==NULL || PubKeyHndl->Blob==NULL)
        return EFI_SECURITY_VIOLATION; 

    // FwKey is potentially a TimeStamped Hash of x509 tbs certificate data
    if(!guidcmp(&PubKeyHndl->AlgGuid, &gEfiCertSha256Guid)) {
        MemCpy(HashCert.ToBeSignedHash, PubKeyHndl->Blob, SHA256_DIGEST_SIZE);
        TrustCert = (UINT8*)&HashCert;
        CertSize = sizeof(EFI_CERT_X509_SHA256);
    } else
        if(!guidcmp(&PubKeyHndl->AlgGuid, &gEfiCertX509Guid)) {
            TrustCert = PubKeyHndl->Blob;
            CertSize  = PubKeyHndl->BlobSize;
        } else
            return EFI_UNSUPPORTED;

    pDigest = &gHashDB[0];
    Pkcs7Cert = (UINT8*)&FWCapsuleHdr->FWCert.SignCert.CertData; 
    Pkcs7Cert_len = FWCapsuleHdr->FWCert.SignCert.Hdr.Hdr.dwLength-sizeof(WIN_CERTIFICATE_UEFI_GUID_1);
    // get Digest Algorithm
    Status = gpAmiSigPPI->Pkcs7GetDigestAlgorithm( gpAmiSigPPI,
                         Pkcs7Cert, Pkcs7Cert_len, &DigestAlgo);
    DEBUG ((SecureMod_DEBUG_LEVEL, "Get Hash Algorithm %r, %d\n", Status, DigestAlgo));
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
    *FailedVTask = Sig;

    Status = HashFwRomMapImage(FWCapsuleHdr, Payload, RomSize, HashAlgorithm, gHashDB);
    if (EFI_ERROR(Status)) return Status;

// 2.2 Verify Fw Certificate
    return gpAmiSigPPI->VerifyPkcs7Sig( gpAmiSigPPI,
                     Pkcs7Cert, Pkcs7Cert_len,           // Pkcs7Cert
                     TrustCert, CertSize,
                     &pDigest, &DigestLen               // In/OutData
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
    IN UINTN                     RomSize,
    IN OUT UINT32               *FailedVTask
){
    EFI_STATUS              Status;
    PEI_CRYPT_HANDLE        HashHndl;
    PEI_CRYPT_HANDLE        PubKeyHndl;
    UINT8                   *pSig;
    UINT32                  Flags;
    UINT8                   *Addr;
    UINTN                   Size;
    EFI_CERT_BLOCK_RSA_2048_SHA256* pRootCert;

    Flags = (FWCapsuleHdr->CapHdr.Flags & CAPSULE_FLAGS_RSA_PSS_PADDING_SCHEME)?EFI_CRYPT_RSASSA_PSS:EFI_CRYPT_RSASSA_PKCS1V15;

    HashHndl.AlgGuid  = gEfiHashAlgorithmSha256Guid;
    HashHndl.BlobSize = SHA256_DIGEST_SIZE;
    HashHndl.Blob = (UINT8*)&gHashDB;
//
// 1. Compare Capsule's Sign Cert key with Platform Root Key
//
    if( FailedVTask==NULL )
        return EFI_SECURITY_VIOLATION;

    *FailedVTask = Key;

    PubKeyHndl.BlobSize =  DEFAULT_RSA_KEY_MODULUS_LEN;
    PubKeyHndl.AlgGuid = gEfiCertRsa2048Guid;
    PubKeyHndl.Blob = (UINT8*)FWCapsuleHdr->FWCert.SignCert.CertData.PublicKey;
    Status = gpAmiSigPPI->VerifyKey(gpAmiSigPPI, &gPRKeyGuid, &PubKeyHndl); 
    DEBUG ((SecureMod_DEBUG_LEVEL, "Compare Platform and SignCert Keys : %r\n", Status));
    //  Skip the RootCert key checking if SignCert Key and PR Key are matching
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
            Status = gpAmiSigPPI->VerifyKey(gpAmiSigPPI, &gPRKeyGuid, &PubKeyHndl); 
            DEBUG ((SecureMod_DEBUG_LEVEL, "Compare Platform and RootCert Keys : %r\n", Status));
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

            Status = gpAmiSigPPI->Hash(gpAmiSigPPI,&gEfiHashAlgorithmSha256Guid, 1,(const UINT8**)&Addr,(const UINTN*)&Size, gHashDB); 
            if (EFI_ERROR(Status)) break;
        
            pSig = (void*)pRootCert->Signature; 
            Status = gpAmiSigPPI->VerifySig(gpAmiSigPPI, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags ); 
            DEBUG ((SecureMod_DEBUG_LEVEL, "Verify Root Cert : %r\n", Status));
            break;
        }
    }
    if (EFI_ERROR(Status)) return EFI_SECURITY_VIOLATION;
//
// 3. Verify Signing Cert
//
    *FailedVTask = Sig;
//
// 3.1 The Rom image hash is calculated based on info from the Rom Area map
//
    Status = HashFwRomMapImage(FWCapsuleHdr, Payload, RomSize, &gEfiHashAlgorithmSha256Guid, gHashDB);
    if (EFI_ERROR(Status)) return Status;

    pSig = (void*)FWCapsuleHdr->FWCert.SignCert.CertData.Signature; 
    PubKeyHndl.Blob = (UINT8*)FWCapsuleHdr->FWCert.SignCert.CertData.PublicKey;

    return gpAmiSigPPI->VerifySig(gpAmiSigPPI, &PubKeyHndl, &HashHndl, pSig, DEFAULT_RSA_SIG_LEN, Flags); 
}

/**
    This code verifies FW Capsule is genuine, 
    and performs following checks on the image:
    1. FWCapsule Header format
    2. Signer Certificate key is chained to root Platform key
    3. Integrity check. Image Signature verification
    4. Re-Play protection. Verifies that new FW image version is newer then the current one
    5. Update pPwCapsuleSize = FwPayload and set pCapsule to point to FW Payload

    @param[in]   pFwCapsule      Points to the start of the Aptio FW Capsule.
    @param[in]   pFwCapsuleSize  The size of buffer, in bytes.

    @retval       pFwCapsule      ptr to a start of Capsule's Payload - ROM image
    @retval       pFwCapsuleSize  return ROM image size
    @retval       pFWCapsuleHdr   return ptr to FwCap Hdr within a Capsule'a image
    @retval       FailedVTask     Identify specific task the Verification has failed for
    @retval       Status 
                    EFI_SUCCESS               The signature is successfully verified.
                    EFI_SECURITY_VIOLATION    The signature does not match the given message.
                    EFI_ACCESS_DENIED         The key could not be used in signature operation.
                    EFI_INVALID_PARAMETER     The size of input message or signature does not meet the criteria 
                                              of the underlying signature algorithm.
**/
EFI_STATUS 
VerifyFwImage(
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN OUT VOID          **pCapsule,
  IN OUT UINT32         *pCapsuleSize,
  IN OUT UINT32         *FailedVTask
){
    EFI_STATUS              Status;
    APTIO_FW_CAPSULE_HEADER  *FWCapsuleHdr;
    UINT8                   *Payload;
    UINTN                   RomSize;
    PEI_CRYPT_HANDLE        PubKeyHndl;
    EFI_BOOT_MODE           BootMode;

    gPeiServices = PeiServices; 

    (*PeiServices)->GetBootMode(PeiServices, &BootMode);
    DEBUG ((SecureMod_DEBUG_LEVEL, "\nValidating FW Capsule (size %X) BootMode %X...\n", (pCapsuleSize?*pCapsuleSize:0), BootMode));

    // FW Capsule presence check
    if(pCapsule == NULL || *pCapsule == NULL ||
       pCapsuleSize == NULL ||
       FailedVTask == NULL )
        return EFI_SECURITY_VIOLATION;

    // Predefined bit mask of checks to perform on Aptio FW Capsule
    *FailedVTask = Cap;

    Status = (*PeiServices)->LocatePpi(PeiServices, &gAmiDigitalSignaturePPIGuid, 0, NULL, &gpAmiSigPPI);
    if(EFI_ERROR(Status)) return Status;


    FWCapsuleHdr = *pCapsule;
    Payload = (UINT8*)*pCapsule;

// verify Capsule Mailbox points to FW_CAPSULE hdr
    if(!guidcmp((EFI_GUID*)&FWCapsuleHdr->CapHdr.CapsuleGuid, &gFWCapsuleGuid)) 
    {
        // Update Payload to point to beginning of Bios ROM
        Payload = (UINT8*)((UINTN)FWCapsuleHdr + FWCapsuleHdr->RomImageOffset);
        if ((UINTN)Payload < (UINTN)FWCapsuleHdr)
          return EFI_SECURITY_VIOLATION;
    } else {
        if(EFI_ERROR(FindCapHdrFFS(Payload, *pCapsuleSize, &FWCapsuleHdr)))
            return EFI_SECURITY_VIOLATION;
    }

    // More Hdr fields sanity checks for buffer overruns
    if(!IsFwCapHdrValid(FWCapsuleHdr))
        return EFI_SECURITY_VIOLATION;

    RomSize = FWCapsuleHdr->CapHdr.CapsuleImageSize - FWCapsuleHdr->RomImageOffset;
    DEBUG ((SecureMod_DEBUG_LEVEL, "FW Capsule Hdr offs 0x%X\nGUID: %g\nPayload Size: 0x%X\n",
               ((UINTN)FWCapsuleHdr-(UINTN)*pCapsule), &(FWCapsuleHdr->CapHdr.CapsuleGuid), RomSize));

    if(RomSize > *pCapsuleSize)
        return EFI_SECURITY_VIOLATION;

    *pCapsule = (UINT32*)Payload;
    *pCapsuleSize = (UINT32)RomSize;

    *FailedVTask = Key;
    PubKeyHndl.BlobSize =  0;
    PubKeyHndl.Blob = NULL;
    Status = gpAmiSigPPI->GetKey(gpAmiSigPPI, &gPRKeyGuid, &PubKeyHndl);
    DEBUG ((SecureMod_DEBUG_LEVEL, "Get FW Key: %r\n", Status));
    // Skip further Capsule Fw revision test if there is NO or a dummy FWkey is detected or if FWkey store empty
    if(EFI_ERROR(Status) || *((UINT32*)(PubKeyHndl.Blob)) == *(UINT32*)FwBadKey) {
        DEBUG ((SecureMod_DEBUG_LEVEL, "FW Key store is empty...Skip further image validation.\n"));
        return EFI_SUCCESS;
    }
// Begin Authentication
    if(!guidcmp((EFI_GUID*)&FWCapsuleHdr->FWCert.SignCert.Hdr.CertType, &gEfiCertPkcs7Guid))
        Status = VerifyFwCertPkcs7(FWCapsuleHdr, Payload, RomSize, &PubKeyHndl, FailedVTask);
    else
        Status = VerifyFwCertRsa2048Sha256(FWCapsuleHdr, Payload, RomSize, FailedVTask);

    DEBUG ((SecureMod_DEBUG_LEVEL, "Signature verify: %r\n", Status));
    if (EFI_ERROR(Status)) return Status;

    Status = VerifyFwRevision(FWCapsuleHdr, Payload, FailedVTask);
    DEBUG ((SecureMod_DEBUG_LEVEL, "FW Revision test: %r (FailedVTask = %x)\n", Status, *FailedVTask));

    // Allow Flash Update with FW version roll-back error only if
    // 1. Interactive Flash enabled and,
    // 2. Boot on Flash Update and DXE is set to load from Flash(exclude Recovery because it always boots DXE from new image)
#if (defined(REFLASH_INTERACTIVE) && REFLASH_INTERACTIVE==1)
    if(EFI_ERROR(Status) && Status == EFI_INCOMPATIBLE_VERSION) {
        if (BootMode == BOOT_ON_FLASH_UPDATE && !PcdGetBool(PcdUseNewImageOnFlashUpdate) ) {
            Status = EFI_SUCCESS;
        }
    }
#endif

    return Status;
}
