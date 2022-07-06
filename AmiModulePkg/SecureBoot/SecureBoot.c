//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <AmiDxeLib.h>
#include <Protocol/AmiDigitalSignature.h>
#include <Library/PrintLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DebugLib.h>
#include <Setup.h>
#include <SecureBoot.h>
#if (defined(SECUREBOOT_DEVICE_GUARD_READY_SETUP) && SECUREBOOT_DEVICE_GUARD_READY_SETUP == 1)
#include <AmiDeviceGuardApi.h>
#endif
#include <AmiSecureBootLib.h>

#include <AutoId.h>
#include <Protocol/AMIPostMgr.h> 
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/HiiString.h>
#include <Protocol/DevicePath.h>

static EFI_GUID guidSecurity = SECURITY_FORM_SET_GUID;

typedef enum { Absent=0, Custom, Vendor, Mixed, AmiTest} SECURE_VAR_PRESENT_TYPE;
static UINT8 bKey[SecureBootPolicyVariableCount] = {Absent, Absent, Absent, Absent, Absent, Absent};

#if (defined(SECUREBOOT_DEVICE_GUARD_READY_SETUP) && SECUREBOOT_DEVICE_GUARD_READY_SETUP == 1)
static AMI_DEVICEGUARD_SECBOOT_PROTOCOL *mAmiDeviceGuardApi = NULL;
#endif
static AMI_DIGITAL_SIGNATURE_PROTOCOL    *mAmiSig = NULL;

static UINT8 mDigest[SHA256_DIGEST_SIZE]={0};
#define MAX_ELEM_NUM        28 // ~28 max number of PE hdr elements to be hashed in one pass

#define StrMaxSize 200
static CHAR16 StrTitle[StrMaxSize]; 
static CHAR16 StrMessage[StrMaxSize];
static CHAR16 StrTemp[StrMaxSize];
//
#ifndef TSE_FOR_APTIO_4_50
//
static AMI_POST_MANAGER_PROTOCOL *mPostMgr = NULL;

typedef struct
{
    UINT64 Type;
    UINTN Size;
    CHAR16 *Name;
    STRING_REF Token;
} FILE_TYPE;

static EFI_HII_STRING_PROTOCOL *HiiString = NULL;
static EFI_HII_HANDLE gHiiHandle;

static STRING_REF SecureVariableStringTitle[] = {
    STRING_TOKEN(STR_DBX_TITLE),
    STRING_TOKEN(STR_DBT_TITLE),
    STRING_TOKEN(STR_DBR_TITLE),
    STRING_TOKEN(STR_DB_TITLE),
    STRING_TOKEN(STR_KEK_TITLE),
    STRING_TOKEN(STR_PK_TITLE),
    0
};
static STRING_REF SecureVariableStringRef[] = {
    STRING_TOKEN(STR_DBX_TITLE_TEXT),
    STRING_TOKEN(STR_DBT_TITLE_TEXT),
    STRING_TOKEN(STR_DBR_TITLE_TEXT),
    STRING_TOKEN(STR_DB_TITLE_TEXT),
    STRING_TOKEN(STR_KEK_TITLE_TEXT),
    STRING_TOKEN(STR_PK_TITLE_TEXT),
    0
};
EFI_STATUS DevicePathToStr(EFI_DEVICE_PATH_PROTOCOL *Path,CHAR8    **Str);
EFI_STATUS FileBrowserLaunchFileSystem(BOOLEAN bSelectFile, OUT EFI_HANDLE **outFsHandle, OUT CHAR16 **outFilePath, OUT UINT8 **outFileBuf,OUT UINTN *size );
VOID GetHiiString(IN EFI_HII_HANDLE HiiHandle, IN STRING_REF Token, UINTN DataSize, CHAR16  * pData);
VOID SetAppendSecureBootDBCallback(EFI_HII_HANDLE HiiHandle, UINT8 VarSetMode, UINT8 index);
VOID DeleteSecureBootDBCallback(EFI_HII_HANDLE HiiHandle, UINT8 index);
VOID GetSecureBootVarCertificateInfo(EFI_HII_HANDLE HiiHandle, UINT8 index, UINT16 TitleToken);
EFI_STATUS GetCertificateListInfoFromVar(IN UINT8 Index, IN UINT16 TitleToken,IN OUT UINT8 **VarBuf,IN OUT UINTN *VarSize);
EFI_STATUS SignatureListInfo (IN UINT16 TitleToken,IN VOID *bufIn,IN OUT UINTN *sizeIn);

BOOLEAN gBrowserCallbackEnabled = FALSE;
#else
//ifdef TSE_FOR_APTIO_4_50
extern BOOLEAN gBrowserCallbackEnabled;
#endif //#ifndef TSE_FOR_APTIO_4_50

//----------------------------------------------------------------------------
// Function forward declaration
//----------------------------------------------------------------------------
VOID UpdateSecureVariableBrowserInfo();
VOID UpdateSecureBootBrowserInfo ();
VOID UpdateSecureBootSetupVariable(UINT8);
//----------------------------------------------------------------------------
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   UpdateSecureBootBrowserInfo
//
// Description: Update Secure Boot flags status
//
// Input:       none
//
// Output:      none
//
// Modified:
//
// Referrals:
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UpdateSecureBootBrowserInfo ()
{
    UINT8       Byte;
    UINTN       Size;
    UINT8       i;
    BOOLEAN     tmpBrowserCallbackEnabled = gBrowserCallbackEnabled;

    gBrowserCallbackEnabled = TRUE;
    Size = sizeof(bKey);
    pRS->GetVariable(AMI_SECURE_VAR_PRESENT_VAR, &guidSecurity, NULL, &Size, bKey);
    DEBUG((SecureBoot_DEBUG_LEVEL,"\n%s:",AMI_SECURE_VAR_PRESENT_VAR));
    for(i=0;i<Size;i++) DEBUG((SecureBoot_DEBUG_LEVEL," %x:",bKey[i]));
    DEBUG((SecureBoot_DEBUG_LEVEL,"\n"));
    HiiLibSetBrowserData( sizeof(bKey), &bKey, &guidSecurity, AMI_SECURE_VAR_PRESENT_VAR);
    Size = 1;
    pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s =%x\n",EFI_SETUP_MODE_NAME,  Byte));
    HiiLibSetBrowserData( Size, &Byte, &gEfiGlobalVariableGuid, EFI_SETUP_MODE_NAME);
    Size = 1;
    pRS->GetVariable(EFI_SECURE_BOOT_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n", EFI_SECURE_BOOT_MODE_NAME, Byte));
    HiiLibSetBrowserData(Size, &Byte, &gEfiGlobalVariableGuid, EFI_SECURE_BOOT_MODE_NAME);
    Size = 1;
    pRS->GetVariable(EFI_VENDOR_KEYS_VARIABLE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n",EFI_VENDOR_KEYS_VARIABLE_NAME,  Byte));
    HiiLibSetBrowserData(Size, &Byte, &gEfiGlobalVariableGuid, EFI_VENDOR_KEYS_VARIABLE_NAME);
    Size = 1;
    pRS->GetVariable(EFI_VENDOR_KEYS_NV_VARIABLE_NAME, &gAmiVendorKeysNvGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n",EFI_VENDOR_KEYS_NV_VARIABLE_NAME,  Byte));

    // Update Audit/Deployed Vars.
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT
    Size = 1;
    pRS->GetVariable(EFI_AUDIT_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n",EFI_AUDIT_MODE_NAME,  Byte));
    HiiLibSetBrowserData(Size, &Byte, &gEfiGlobalVariableGuid, EFI_AUDIT_MODE_NAME);
    Size = 1;
    pRS->GetVariable(EFI_DEPLOYED_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n",EFI_DEPLOYED_MODE_NAME,  Byte));
    HiiLibSetBrowserData(Size, &Byte, &gEfiGlobalVariableGuid, EFI_DEPLOYED_MODE_NAME);
    // dbg
    Size = 1;
    pRS->GetVariable(AMI_DEPLOYMENT_MODE_VARIABLE_NAME, &gAmiDeploymentModeNvGuid, NULL, &Size, &Byte);
    DEBUG((SecureBoot_DEBUG_LEVEL,"%s=%x\n",AMI_DEPLOYMENT_MODE_VARIABLE_NAME,  Byte));
#endif

    gBrowserCallbackEnabled = tmpBrowserCallbackEnabled;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   UpdateSecureVariableBrowserInfo
//
// Description: Detect EFI Variables: PK, KEK, db, dbt, dbx, dbr
//
// Input:       none
//
// Output:      none
//
// Modified:
//
// Referrals:
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UpdateSecureVariableBrowserInfo()
{
    EFI_STATUS  Status;
    UINT8       Index;
    EFI_GUID    *EfiVarGuid;
    UINTN       SigCount;
    UINTN       Size, Size2, Size3;
    UINT8       *Buf1, *Buf2;

// Ami Test PK Key, n-modulus RSA2048
static UINT8 AmiPkKeyHash1[SHA256_DIGEST_SIZE] = {
      0x6a, 0x3f, 0x23, 0x0c, 0x2b, 0xd5, 0xea, 0x36, 0xcb, 0xdc, 0x25, 0x75, 0x12, 0xd8, 0x80, 0xb2,
      0x08, 0x53, 0x02, 0x8b, 0x09, 0x09, 0x8f, 0x7e, 0x71, 0x76, 0x15, 0x2a, 0xf1, 0xec, 0x0f, 0x2f
//      0xa9, 0x51, 0x19, 0x2f, 0x10, 0xe3, 0x90, 0x51, 0xf8, 0x0a, 0xfb, 0xc3, 0x83, 0x0e, 0x9f, 0xc9,
//      0x10, 0xe7, 0xa5, 0x49, 0x20, 0xbe, 0x50, 0xf9, 0xb8, 0x04, 0xd6, 0x7e, 0x88, 0x15, 0x96, 0xd9
    };
// Ami Test PK Key, x509 Certificate
static UINT8 AmiPkKeyHash2[SHA256_DIGEST_SIZE]={
      0x01, 0xad, 0xaf, 0x2c, 0x33, 0x4e, 0x76, 0xa6, 0x47, 0x95, 0x16, 0xda, 0xf6, 0x18, 0x38, 0x18,
      0x98, 0x32, 0x3d, 0x8f, 0x5e, 0x7a, 0x57, 0xbe, 0xd1, 0x2b, 0xad, 0x7a, 0xb2, 0x29, 0x20, 0x9d
//      0xcc, 0xa4, 0xe3, 0xf3, 0x17, 0x02, 0x30, 0x03, 0x0d, 0xc3, 0xe3, 0x3d, 0x1e, 0x3f, 0xa7, 0xd1,
//      0x38, 0x3d, 0xe8, 0xb3, 0x36, 0x74, 0x30, 0x89, 0x2e, 0x93, 0xcb, 0xcc, 0xde, 0x03, 0x4c, 0xe0
    };
// AMI Test KEK (TBD)

    UINT8  *CertStart;
    UINTN   CertLen;
    EFI_SIGNATURE_LIST *SigList;
    EFI_SIGNATURE_DATA *SigData;
    INTN err;

    Index = 0;
    while(SecureVariableFileName[Index] != NULL)
    {
        if(Index < SecureVariableSigDatabaseCount)
            EfiVarGuid = &gEfiImageSecurityDatabaseGuid;
        else
            EfiVarGuid = &gEfiGlobalVariableGuid;

        Size = 0;
        Size2 = 0;
        SigCount = 0;
        bKey[Index] = Absent;
        //get var size
        Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &Size, NULL);
        DEBUG((SecureBoot_DEBUG_LEVEL,"%s(%Xh),  status=%r\n",  SecureVariableFileName[Index], Size, Status));
        if(Status == EFI_BUFFER_TOO_SMALL) {
            bKey[Index] = Custom;
//
// enhanced logic for vendor/custom key detection
// 
            Buf1 = NULL;
            Status = pBS->AllocatePool(EfiBootServicesData, Size, (void**)&Buf1);
            ASSERT_EFI_ERROR (Status);
//read Key var to a buffer
            Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &Size, Buf1);
            if(!EFI_ERROR(Status)) {
                Status = AmiValidateSignatureList (Buf1, Size, &SigCount, NULL);
                if(!EFI_ERROR(Status)) {
                    //get Default Key from Volatile key var (use Key size)
                    Buf2 = NULL;
                    Status = pBS->AllocatePool(EfiBootServicesData, Size, (void**)&Buf2);
                    ASSERT_EFI_ERROR (Status);
                    Size2 = Size;
                    Status = pRS->GetVariable( SecureVariableFileNameDefault[Index], &gEfiGlobalVariableGuid, NULL, &Size2, Buf2);
                    DEBUG((SecureBoot_DEBUG_LEVEL,"%s(%Xh),  status=%r\n",  SecureVariableFileNameDefault[Index], Size2, Status));
        // search for matching patterns of buf2, sz 2 inside of buf1
                    if(!EFI_ERROR(Status)) {
                        SigList = (EFI_SIGNATURE_LIST *)Buf1;
                        Size3 = 0;
                        while((Size3 + Size2) <= Size) {
                            if(!MemCmp(SigList, Buf2, Size2) ) {
                                bKey[Index] = Vendor;      //vendor
                                if(Size > Size2)
                                   bKey[Index] = Mixed;   // vendor+extra keys
                                break;
                            }
                            if(SigList->SignatureListSize < 0x2D || // Min size for a dummy sig.list with 1 byte Sig.Data
                               SigList->SignatureListSize > NVRAM_SIZE)
                                break; 
                            Size3+=SigList->SignatureListSize;
                            SigList =(EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
                        }
                    }
                    if(Buf2) pBS->FreePool(Buf2);
//
// Check for AMI Test key in Secure Boot vars
//
    //            if(Wcscmp(SecureVariableFileName[Index], EFI_PLATFORM_KEY_NAME) == 0) {
// Ami Test Rsa2048 key
                    // Verify the PK SignatureType GUID
                    SigList = (EFI_SIGNATURE_LIST *)Buf1;
                    Size3 = 0;
                    while(Size3 < Size) {
                        DEBUG((SecureBoot_DEBUG_LEVEL,"SigList->SignatureType %g\n",  &(SigList->SignatureType)));
                        if (!guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertRsa2048Guid) 
                            ||
                            !guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertX509Guid)
                        ){
                            SigData   = (EFI_SIGNATURE_DATA *) ((UINT8 *) SigList + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize);
                            CertStart = (UINT8*)SigData->SignatureData;
                            CertLen = (UINTN)SigList->SignatureSize-sizeof(EFI_GUID);
                            DEBUG((SecureBoot_DEBUG_LEVEL, "CertStartOffs %x, CertLen %x\n", (UINT32)(CertStart-Buf1), CertLen));
                            if(mAmiSig == NULL)    {
                                Status = pBS->LocateProtocol(&gAmiDigitalSignatureProtocolGuid, NULL, &mAmiSig); 
                            }
                            if(mAmiSig) {
                                Status = mAmiSig->Hash(mAmiSig, &gEfiHashAlgorithmSha256Guid, 
                                        1, &CertStart, (const UINTN*)&CertLen, mDigest);
                                err = MemCmp(AmiPkKeyHash1, mDigest, SHA256_DIGEST_SIZE);
                                DEBUG((SecureBoot_DEBUG_LEVEL, "Compare Key[%d]_1 == FW Key : %a\n", 1,(!err)?"Yes":"No"));
                                if(err) {
                                    err = MemCmp(AmiPkKeyHash2, mDigest, SHA256_DIGEST_SIZE);
                                    DEBUG((SecureBoot_DEBUG_LEVEL, "Compare Key[%d]_2 == FW Key : %a\n", 1,(!err)?"Yes":"No"));
                                }
                                if(!err) {
                                   bKey[Index] = AmiTest;   // AMI Test PK
                                   break;
                                }
                            }
                        }
                        if(SigList->SignatureListSize < 0x4C || // Min size for SHA2 Hash Certificate sig list
                           SigList->SignatureListSize > NVRAM_SIZE)
                            break; 
                        Size3+=SigList->SignatureListSize;
                        SigList =(EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
                    }
    //            }
                }
            }
            if(Buf1) pBS->FreePool(Buf1);
        }
#ifndef TSE_FOR_APTIO_4_50
        switch(bKey[Index]) 
        {
            case Custom: GetHiiString(gHiiHandle, STRING_TOKEN(STR_CUSTOM_KEY), sizeof(StrMessage), StrMessage);
                break;
            case Vendor: GetHiiString(gHiiHandle, STRING_TOKEN(STR_DEFAULT_KEY), sizeof(StrMessage), StrMessage);
                break;
            case Mixed: GetHiiString(gHiiHandle, STRING_TOKEN(STR_MIXED_KEY), sizeof(StrMessage), StrMessage);
                break;
            case AmiTest: GetHiiString(gHiiHandle, STRING_TOKEN(STR_AMI_TEST_KEY), sizeof(StrMessage), StrMessage);
                break;
            case Absent:
            default: GetHiiString(gHiiHandle, STRING_TOKEN(STR_ABSENT_KEY), sizeof(StrMessage), StrMessage);
                break;
        }
        MemSet(StrTemp, sizeof(StrTemp), 0);
        GetHiiString(gHiiHandle, SecureVariableStringRef[Index], sizeof(StrTemp), StrTemp);
        Swprintf_s(StrTitle, sizeof(StrTitle), L"%s|%5d|%5d| %s", StrTemp, Size, SigCount, StrMessage);
//        DEBUG((SecureBoot_DEBUG_LEVEL,"StrTitle '%s'\n", StrTitle));
        HiiLibSetString(gHiiHandle, SecureVariableStringTitle[Index], StrTitle);
#endif
        Index++;
    }
    pRS->SetVariable(AMI_SECURE_VAR_PRESENT_VAR, &guidSecurity, EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof(bKey), &bKey);

    UpdateSecureBootBrowserInfo();
}

#ifndef TSE_FOR_APTIO_4_50
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   UpdateSecureBootSetupVariable
//
// Description: Update Browser Info with Vendor Key status change
//
// Input:       UINT8 CustomMode change
//
// Output:      none
//
// Modified:
//
// Referrals:
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UpdateSecureBootSetupVariable( 
    IN UINT8 CustomMode
    )
{
    EFI_STATUS  Status;
    UINTN       Size;
    UINT32      Attributes;
    BOOLEAN     tmpBrowserCallbackEnabled = gBrowserCallbackEnabled;
    SECURE_BOOT_SETUP_VAR SecureBootSetup, SecureBootSetup1;

    Size = sizeof(SECURE_BOOT_SETUP_VAR);
    gBrowserCallbackEnabled = TRUE;
    Status = HiiLibGetBrowserData( &Size, &SecureBootSetup, &guidSecurity, AMI_SECURE_BOOT_SETUP_VAR);
    gBrowserCallbackEnabled = tmpBrowserCallbackEnabled;
    if(!EFI_ERROR (Status)) {
        pRS->GetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity, &Attributes, &Size, &SecureBootSetup1);
        SecureBootSetup.SecureBootMode = CustomMode;
        if(MemCmp(&SecureBootSetup, &SecureBootSetup1, Size) != 0) {
            Status = pRS->SetVariable (AMI_SECURE_BOOT_SETUP_VAR, &guidSecurity, Attributes, Size, &SecureBootSetup);
            DEBUG((SecureBoot_DEBUG_LEVEL,"Update %s Var %r\n", AMI_SECURE_BOOT_SETUP_VAR, Status));
        }
    }
}

/**
  Calculate hash of Pe/Coff image based on the Authenticode image hashing in
  PE/COFF Specification 8.0 Appendix A

  Caution: This function may receive untrusted input.
  PE/COFF image is external input, so this function will validate its data structure
  within this image buffer before use.
  Caller is responsible to free newly created Digest buffer.

  @param[in]    ImageAddress
  @param[in]    ImageSize
  @param[in,out]    SHA256 Digest

  @retval TRUE      Successfully hashed image.
  @retval FALSE     Fail in hashing of image.

**/
BOOLEAN
HashPeImage (
  IN const UINT8 *ImageAddress,
  IN UINTN        ImageSize,
  IN OUT UINT8   *Digest
  )
{
    EFI_STATUS                        Status;
    EFI_IMAGE_DOS_HEADER              *DosHdr;
    UINT32                            PeCoffHeaderOffset;
    EFI_IMAGE_SECTION_HEADER          *Section;
    UINT8                             *HashBase;
    UINTN                             HashSize;
    UINTN                             SumOfBytesHashed;
    EFI_IMAGE_SECTION_HEADER          *SectionHeader;
    UINTN                             Index, Pos;
    UINT16                            Magic;
    BOOLEAN                           res;
    const UINT8             *addr[MAX_ELEM_NUM];
    UINTN                    num_elem, len[MAX_ELEM_NUM];

    EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;

    SectionHeader = NULL;
    res = FALSE;
    Status = EFI_ABORTED;
    PeCoffHeaderOffset = 0;
    num_elem = 0;

    //
    // Check PE/COFF image
    //
    DosHdr = (EFI_IMAGE_DOS_HEADER *) (UINTN) ImageAddress;
    if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
        PeCoffHeaderOffset = DosHdr->e_lfanew;
      } else {
        goto Finish;
    }

    if ( ((EFI_TE_IMAGE_HEADER *)((UINT8 *) (UINTN) ImageAddress + PeCoffHeaderOffset))->Signature
        == EFI_TE_IMAGE_HEADER_SIGNATURE) {
        goto Finish;
    }

  //
  // PE/COFF Image Measurement
  //
  //    NOTE: The following codes/steps are based upon the authenticode image hashing in 
  //      PE/COFF Specification 8.0 Appendix A.
  //      
  //

  // 1.    Load the image header into memory.
  
  // 2.    Initialize a SHA hash context.
    if(mAmiSig == NULL) {
        Status = pBS->LocateProtocol(&gAmiDigitalSignatureProtocolGuid, NULL, &mAmiSig); 
        if(EFI_ERROR(Status) || mAmiSig == NULL)
            goto Finish;
    }
  //
  // Measuring PE/COFF Image Header; 
  // But CheckSum field and SECURITY data directory (certificate) are excluded
  //
    Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINT8 *) (UINTN) ImageAddress + PeCoffHeaderOffset);
    Magic    = Hdr.Pe32->OptionalHeader.Magic;

    //
    // 3.  Calculate the distance from the base of the image header to the image checksum address.
    // 4.  Hash the image header from its base to beginning of the image checksum.
    //
    HashBase = (UINT8 *) (UINTN) ImageAddress;
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      HashSize = (UINTN) ((UINT8 *)(&Hdr.Pe32->OptionalHeader.CheckSum) - HashBase);
    } else {
      //
      // Use PE32+ offset
      //
      HashSize = (UINTN) ((UINT8 *)(&Hdr.Pe32Plus->OptionalHeader.CheckSum) - HashBase);
    }

    addr[num_elem] = HashBase;
    len[num_elem++] =  HashSize;

  //
  // 5.    Skip over the image checksum (it occupies a single ULONG).
  // 6.    Get the address of the beginning of the Cert Directory.
  // 7.    Hash everything from the end of the checksum to the start of the Cert Directory.
  //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
        HashBase = (UINT8 *) &Hdr.Pe32->OptionalHeader.CheckSum + sizeof (UINT32);
        HashSize = (UINTN) ((UINT8 *)(&Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY]) - HashBase);
    } else {
      //
      // Use PE32+ offset
      //
        HashBase = (UINT8 *) &Hdr.Pe32Plus->OptionalHeader.CheckSum + sizeof (UINT32);
        HashSize = (UINTN) ((UINT8 *)(&Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY]) - HashBase);
    }

    addr[num_elem] = HashBase;
    len[num_elem++] =  HashSize;

  //
  // 8.    Skip over the Cert Directory. (It is sizeof(IMAGE_DATA_DIRECTORY) bytes.)
  // 9.    Hash everything from the end of the Cert Directory to the end of image header.
  //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
     //
     // Use PE32 offset
     //
        HashBase = (UINT8 *) &Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1];
         HashSize = Hdr.Pe32->OptionalHeader.SizeOfHeaders -
              (UINTN) ((UINT8 *)(&Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1]) - (UINT8 *) (UINTN) ImageAddress);
    } else {
     //
     // Use PE32+ offset
     //
        HashBase = (UINT8 *) &Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1];
        HashSize = Hdr.Pe32Plus->OptionalHeader.SizeOfHeaders -
              (UINTN) ((UINT8 *)(&Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1]) - (UINT8 *) (UINTN) ImageAddress);
    }

    addr[num_elem] = HashBase;
    len[num_elem++] =  HashSize;

  //
  // 10. Set the SUM_OF_BYTES_HASHED to the size of the header 
  //
   if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
        SumOfBytesHashed = Hdr.Pe32->OptionalHeader.SizeOfHeaders;
    } else {
      //
      // Use PE32+ offset
      //
        SumOfBytesHashed = Hdr.Pe32Plus->OptionalHeader.SizeOfHeaders;
    }

  //
  // 11. Build a temporary table of pointers to all the IMAGE_SECTION_HEADER 
  //     structures in the image. The 'NumberOfSections' field of the image 
  //     header indicates how big the table should be. Do not include any 
  //     IMAGE_SECTION_HEADERs in the table whose 'SizeOfRawData' field is zero.   
  //

    Status = pBS->AllocatePool(EfiBootServicesData, sizeof (EFI_IMAGE_SECTION_HEADER) * Hdr.Pe32->FileHeader.NumberOfSections, (void**)&SectionHeader);  
    if (SectionHeader == NULL) {
        goto Finish;
    }
    MemSet( SectionHeader, sizeof (EFI_IMAGE_SECTION_HEADER) * Hdr.Pe32->FileHeader.NumberOfSections, 0 );

  //
  // 12.    Using the 'PointerToRawData' in the referenced section headers as 
  //      a key, arrange the elements in the table in ascending order. In other 
  //      words, sort the section headers according to the disk-file offset of 
  //      the section.
  //
    Section = (EFI_IMAGE_SECTION_HEADER *) (
                   (UINT8 *) (UINTN) ImageAddress +
                   PeCoffHeaderOffset +
                   sizeof(UINT32) +
                   sizeof(EFI_IMAGE_FILE_HEADER) +
                   Hdr.Pe32->FileHeader.SizeOfOptionalHeader
                   );
    for (Index = 0; Index < Hdr.Pe32->FileHeader.NumberOfSections; Index++) {
        Pos = Index;
        while ((Pos > 0) && (Section->PointerToRawData < SectionHeader[Pos - 1].PointerToRawData)) {
          MemCpy (&SectionHeader[Pos], &SectionHeader[Pos - 1], sizeof(EFI_IMAGE_SECTION_HEADER));
          Pos--;
        }
        MemCpy (&SectionHeader[Pos], Section, sizeof(EFI_IMAGE_SECTION_HEADER));
        Section += 1;
    }

  //
  // 13.    Walk through the sorted table, bring the corresponding section 
  //      into memory, and hash the entire section (using the 'SizeOfRawData' 
  //      field in the section header to determine the amount of data to hash).
  // 14.    Add the section's 'SizeOfRawData' to SUM_OF_BYTES_HASHED .
  // 15.    Repeat steps 13 and 14 for all the sections in the sorted table.
  //
     for (Index = 0; Index < Hdr.Pe32->FileHeader.NumberOfSections; Index++) {
         Section  = (EFI_IMAGE_SECTION_HEADER *) &SectionHeader[Index];
         if (Section->SizeOfRawData == 0) {
             continue;
         }
         HashBase = (UINT8 *) (UINTN) ImageAddress + Section->PointerToRawData;
         HashSize = (UINTN) Section->SizeOfRawData;

         addr[num_elem] = HashBase;
         len[num_elem++] =  HashSize;
         if(num_elem >= MAX_ELEM_NUM)
             goto Finish;

         SumOfBytesHashed += HashSize;
    }


  //
  // 16.    If the file size is greater than SUM_OF_BYTES_HASHED, there is extra
  //      data in the file that needs to be added to the hash. This data begins 
  //      at file offset SUM_OF_BYTES_HASHED and its length is:
  //             FileSize  -  (CertDirectory->Size)
  //
    if (ImageSize > SumOfBytesHashed) {
        HashBase = (UINT8 *) (UINTN) ImageAddress + SumOfBytesHashed;
        if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        //
        // Use PE32 offset
        //
            HashSize = (UINTN)(ImageSize -
                       Hdr.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY].Size -
                       SumOfBytesHashed);
        } else {
        //
        // Use PE32+ offset
        //
            HashSize = (UINTN)(ImageSize -
                       Hdr.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY].Size -
                       SumOfBytesHashed);
        }

        addr[num_elem] = HashBase;
        len[num_elem++] =  HashSize;
        if(num_elem >= MAX_ELEM_NUM)
            goto Finish;
    }
  //
  // 17.    Finalize the SHA hash.
  //
    Status = mAmiSig->Hash(mAmiSig, &gEfiHashAlgorithmSha256Guid, num_elem, addr, len, Digest);

Finish:

    res = (!EFI_ERROR(Status)) ? TRUE : FALSE;

    if(SectionHeader!=NULL)
    {
        pBS->FreePool(SectionHeader);
    }

    return res;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:    IsCsmEnabled
//
// Description:  This function checks if CSM is enabled
//
//  Input:
//     None
//
//  Output:
//  0 - CSM is disabled
//  1 - CSM is enabled
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
#if (defined(CSM_SUPPORT) && (CSM_SUPPORT != 0))
BOOLEAN
IsCsmEnabled(VOID)
{
    EFI_STATUS Status;
    UINTN Size = sizeof(EFI_HANDLE);
    EFI_HANDLE Handle;

    Status = pBS->LocateHandle(ByProtocol, &gEfiLegacyBiosProtocolGuid, NULL, &Size, &Handle);
    return (EFI_ERROR(Status)) ? 0 : 1;
}
#endif
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   ForceSetupModeCallback
//
// Description: 
//
// Input:       none
//
// Output:      VOID
//
// Modified:
//
// Referrals:
//
// Notes:
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
ForceSetupModeCallback(EFI_HII_HANDLE HiiHandle, UINT16 Class, UINT16 SubClass, UINT16 Key)
{
    EFI_STATUS Status;
    UINT8       Sel = 0;
    UINTN       Size;
    UINT8       SetupMode;
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT
    UINT8       AuditMode = 0, DeployedMode = 0;
#endif

    CALLBACK_PARAMETERS *Callback;
    EFI_BROWSER_ACTION_REQUEST *rq;

    Callback = GetCallbackParameters();
    if(!Callback) {
        return EFI_SUCCESS;
    }
//    DEBUG((SecureBoot_DEBUG_LEVEL,"\n====ForceSetupModeCallback==== Key=%d, Action=%0X, Value=%X\n",  Key, Callback->Action, Callback->Value->u8 ));
    if( Callback->Action != EFI_BROWSER_ACTION_CHANGING) {
//        if(Callback->Action==EFI_BROWSER_ACTION_RETRIEVE)
//            UpdateSecureVariableBrowserInfo();
        return EFI_UNSUPPORTED;
    }
    if(mPostMgr == NULL)
    {
        Status = pBS->LocateProtocol(&gAmiPostManagerProtocolGuid, NULL, &mPostMgr);
        if(EFI_ERROR(Status) || !mPostMgr) {
            return EFI_SUCCESS;
        }
    }
    Status = EFI_SUCCESS;
    gHiiHandle = HiiHandle;
    switch(Key) {
        case  SECURE_BOOT_MENU_REFRESH:
//            UpdateSecureBootBrowserInfo();
//            break;
        case  KEY_MANAGEMENT_MENU_REFRESH:
            UpdateSecureVariableBrowserInfo();
            break;
        case SECURE_BOOT_SUPPORT_CHANGE_KEY:
            Size = 1;
            pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &SetupMode);
            if( Callback->Value->u8 == SECURE_BOOT_ENABLE) // trying to switch Secure Boot from Disable to Enable
            {
                rq = Callback->ActionRequest;
                *rq = EFI_BROWSER_ACTION_REQUEST_NONE;
                if(SetupMode == SETUP_MODE) {
                    GetHiiString(HiiHandle, STRING_TOKEN(STR_ENABLE_ERROR_MODE_TITLE), sizeof(StrTitle), StrTitle);
                    GetHiiString(HiiHandle, STRING_TOKEN(STR_ENABLE_ERROR_MODE), sizeof(StrMessage),StrMessage);
                    mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK,NULL);
#if DEFAULT_SECURE_BOOT_ENABLE == 0
                    Status = EFI_DEVICE_ERROR;
#endif
                } else {
#if defined(CSM_SUPPORT) && CSM_SUPPORT
                    if (IsCsmEnabled()) {
                        GetHiiString(HiiHandle, STRING_TOKEN(STR_CSM_LOAD_TITLE), sizeof(StrTitle),StrTitle);
                        GetHiiString(HiiHandle, STRING_TOKEN(STR_CSM_LOAD), sizeof(StrMessage),StrMessage);
                        mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK,NULL);
#if DEFAULT_SECURE_BOOT_ENABLE == 0
                        Status = EFI_DEVICE_ERROR;
#endif
                    }
#endif // CSM
                }
            }
            break;
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT && CUSTOMIZED_SECURE_BOOT_DEPLOYMENT_SETUP
        case FORCE_AUDIT_KEY:
        case FORCE_DEPLOYED_KEY:
        case CLEAR_DEPLOYED_KEY:
             if (Key == FORCE_AUDIT_KEY) {
                  GetHiiString(HiiHandle, STRING_TOKEN(STR_FORCE_AUDIT_MODE), sizeof(StrTitle),StrTitle);
                  Size = 1;
                  pRS->GetVariable(EFI_SETUP_MODE_NAME, &gEfiGlobalVariableGuid, NULL, &Size, &SetupMode);
                  if(SetupMode == USER_MODE) {
                      GetHiiString(HiiHandle, STRING_TOKEN(STR_PK_DEL_WARNING), sizeof(StrMessage),StrMessage);
                      mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
                      if (Sel == 1)
                          break;
                  }
                  AuditMode = 1;
                  Status = pRS->SetVariable(EFI_AUDIT_MODE_NAME,&gEfiGlobalVariableGuid, 
                              EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, 1, &AuditMode);
                  DEBUG((SecureBoot_DEBUG_LEVEL,"Force '%s' %r (%X)\n",EFI_AUDIT_MODE_NAME,  Status, Status));
              }
              if (Key == FORCE_DEPLOYED_KEY) {
                  DeployedMode = 1;
                  GetHiiString(HiiHandle, STRING_TOKEN(STR_FORCE_DEPLOYED_MODE),sizeof(StrTitle), StrTitle);
                  Status = pRS->SetVariable(EFI_DEPLOYED_MODE_NAME,&gEfiGlobalVariableGuid, 
                              EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, 1, &DeployedMode);
                  DEBUG((SecureBoot_DEBUG_LEVEL,"Force '%s' %r\n",EFI_DEPLOYED_MODE_NAME,  Status));
              }
              if (Key == CLEAR_DEPLOYED_KEY) {
                  DeployedMode = 0;
                  GetHiiString(HiiHandle, STRING_TOKEN(STR_FORCE_DEPLOYED_MODE),sizeof(StrTitle), StrTitle);
                  Status = pRS->SetVariable(EFI_DEPLOYED_MODE_NAME,&gEfiGlobalVariableGuid, 
                              EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, 1, &DeployedMode);
                  DEBUG((SecureBoot_DEBUG_LEVEL,"Clear '%s' %r\n",EFI_DEPLOYED_MODE_NAME,  Status));
              }
              UnicodeSPrint(StrMessage, sizeof(StrMessage), L"%r", Status);
              mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );

              if(!EFI_ERROR(Status)){
                  UpdateSecureVariableBrowserInfo();
              }
           break;
#endif
        case SECURE_BOOT_MODE_CHANGE_KEY:
            if( Callback->Value->u8 == 0)    // trying to switch from Custom to Standard
                ForceSetupModeCallback(HiiHandle, Class, SubClass, FORCE_DEFAULT_KEY);
            UpdateSecureBootSetupVariable(Callback->Value->u8); // upd sec Boot Setup variable
            break;

        case KEY_PROVISION_CHANGE_KEY:
            if(Callback->Value->u8 == 0) // trying to switch from Enable to Disable
                break;
        case FORCE_DEFAULT_KEY:
        case FORCE_DEFAULT_KEY_2:
            GetHiiString(HiiHandle, STRING_TOKEN(STR_LOAD_DEFAULT_VARS_TITLE), sizeof(StrTitle),StrTitle);
            GetHiiString(HiiHandle, STRING_TOKEN(STR_SECURE_BOOT_PROCEED_CANCEL), sizeof(StrMessage),StrMessage);
            mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
            if (Sel == 0) {
                AmiInstallSecureBootDefaults(RESET_NV_KEYS); // erase all secure boot NV variables before installing selected defaults
                Status = AmiInstallSecureBootDefaults(SET_NV_DEFAULT_KEYS);    // erase+set
#if CUSTOMIZED_SECURE_BOOT_DEPLOYMENT && DEFAULT_SECURE_BOOT_DEPLOYED_MODE
                if (!EFI_ERROR(Status)) {
                    DeployedMode = 1;
                    Status = pRS->SetVariable(EFI_DEPLOYED_MODE_NAME,&gEfiGlobalVariableGuid, 
                                EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_BOOTSERVICE_ACCESS, 1, &DeployedMode);
                    DEBUG((SecureBoot_DEBUG_LEVEL,"Force '%s' %r\n",EFI_DEPLOYED_MODE_NAME,  Status));
                }
#endif
            }
        case FORCE_SETUP_KEY:
        case FORCE_SETUP_KEY_2:
            if (Key == FORCE_SETUP_KEY || Key == FORCE_SETUP_KEY_2)
            {
                GetHiiString(HiiHandle, STRING_TOKEN(STR_FORCE_SETUP_MODE), sizeof(StrTitle),StrTitle);
                GetHiiString(HiiHandle, STRING_TOKEN(STR_RESET_TO_SETUP), sizeof(StrMessage),StrMessage);
                mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
                if (Sel == 0)
                   Status = AmiInstallSecureBootDefaults(RESET_NV_KEYS);    // erase
            }
            if (Sel == 0) {
//                if (Key == FORCE_DEFAULT_KEY_2 || Key == FORCE_SETUP_KEY_2)
//                    UpdateSecureBootBrowserInfo();
//                else 
                    UpdateSecureVariableBrowserInfo();
            }
            if (EFI_ERROR(Status) && Status != EFI_NOT_FOUND)
            {
                GetHiiString(HiiHandle, STRING_TOKEN(STR_VAR_UPDATE_LOCKED_TITLE), sizeof(StrTitle),StrTitle);
                GetHiiString(HiiHandle, STRING_TOKEN(STR_VAR_UPDATE_LOCKED), sizeof(StrMessage),StrMessage);
                mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK,NULL);
            }
            Status = EFI_SUCCESS;
            break;
#if (defined(SECUREBOOT_DEVICE_GUARD_READY_SETUP) && SECUREBOOT_DEVICE_GUARD_READY_SETUP == 1)
        case REMOVE_UEFI_CA:
        case RESTORE_DB_DEFAULTS:
            GetHiiString(HiiHandle, STRING_TOKEN(STR_SECURE_BOOT_PROCEED_CANCEL), sizeof(StrMessage),StrMessage);
            if(mAmiDeviceGuardApi == NULL)
                pBS->LocateProtocol(&gAmiDeviceGuardSecBootApiGuid, NULL, &mAmiDeviceGuardApi);
            if(mAmiDeviceGuardApi == NULL)
                break;
            if(Key == REMOVE_UEFI_CA) {
                // ask to remove UEFI CA
                GetHiiString(HiiHandle, STRING_TOKEN(STR_REMOVE_UEFI_CA_FROM_DB), sizeof(StrTitle),StrTitle);
                mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
                if (Sel == 0) {
                    /*tst mAmiDeviceGuardApi->IsUefiCaInDb(mAmiDeviceGuardApi); */
                    Status = mAmiDeviceGuardApi->DeleteUefiCaFromDb(mAmiDeviceGuardApi);
                }
            }
            if(Key == RESTORE_DB_DEFAULTS) {
                // ask to load DB defaults
                GetHiiString(HiiHandle, STRING_TOKEN(STR_RESTORE_DB_DEFAULTS), sizeof(StrTitle),StrTitle);
                mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
                if (Sel == 0) {
                    Status = mAmiDeviceGuardApi->ResetDbToDefault(mAmiDeviceGuardApi);
                }
            }
            if (Sel == 0) {
                UnicodeSPrint(StrMessage, sizeof(StrMessage), L"%r", Status);
                mPostMgr->DisplayMsgBox( StrTitle,  StrMessage, MSGBOX_TYPE_OK,NULL);
                UpdateSecureVariableBrowserInfo();
            }
            Status = EFI_SUCCESS;
            break;
#endif

        default:
            break;
    }
    return Status;//EFI_SUCCESS;
}

// ----------------------------------------------------------------------------
// Auxiliary functions for Key Management Callbacks
// ----------------------------------------------------------------------------
EFI_STATUS SetSecureVariable(UINT8 Index, UINT16 InstallVars, UINT8 *pVarBuffer, UINTN VarSize )
{
    EFI_GUID    *EfiVarGuid;

    if(Index < SecureVariableSigDatabaseCount)
        EfiVarGuid = &gEfiImageSecurityDatabaseGuid;
    else
        EfiVarGuid = &gEfiGlobalVariableGuid;

    return AmiSetSecureBootVariable(InstallVars, SecureVariableFileName[Index], NULL, EfiVarGuid, pVarBuffer, VarSize );
}

EFI_STATUS
SaveToFileSecureBootVar(EFI_HII_HANDLE HiiHandle, UINT16 Key)
{
    EFI_STATUS Status;
    EFI_HANDLE *FsHandle;
    UINT8 *FileBuf=NULL;
    UINTN size;
    CHAR16 *FilePath=NULL;
    UINT16 Index, nVars;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *pSFP;
    EFI_FILE_PROTOCOL *pRoot,*FileHandle;
    EFI_GUID    *EfiVarGuid;
    UINT8       *Data=NULL;
    BOOLEAN     bFound = TRUE;

    if(Key > SecureBootPolicyVariableCount)
        return EFI_SUCCESS;

    gHiiHandle = HiiHandle;
    size = 0;
    Status = FileBrowserLaunchFileSystem(FALSE, &FsHandle, &FilePath, &FileBuf, &size);
    if(EFI_ERROR(Status))
       goto Done;

    Index = (Key == SecureBootPolicyVariableCount)?0:Key;
//    Index = 0;
    nVars = 0;
    MemSet(StrTemp, sizeof(StrTemp), 0);
    while(bFound && SecureVariableFileName[Index] != NULL)
    {
        if(Index < SecureVariableSigDatabaseCount)
            EfiVarGuid = &gEfiImageSecurityDatabaseGuid;
        else
            EfiVarGuid = &gEfiGlobalVariableGuid;

        size = 0;  
        Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &size, NULL);
        if(Status == EFI_BUFFER_TOO_SMALL)
        {
            // Append AuthHdr to Var data.
            //Allocate Size
            Status = pBS->AllocatePool(EfiBootServicesData, size, (void**)&Data);
            ASSERT_EFI_ERROR (Status);

            // Read the Variable
            Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, &size, Data);
            if (!EFI_ERROR(Status)) 
            {
                Status = pBS->HandleProtocol( FsHandle, &gEfiSimpleFileSystemProtocolGuid, &pSFP );
                if (!EFI_ERROR(Status)) 
                {
                    Status = pSFP->OpenVolume(pSFP,&pRoot);
                    if (!EFI_ERROR(Status)) 
                    {
                        Status = pRoot->Open(pRoot,&FileHandle,SecureVariableFileName[Index],
                            EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,0);
                        if(!EFI_ERROR(Status))
                        {
                            FileHandle->Delete(FileHandle);
                            Status = pRoot->Open(pRoot,&FileHandle,SecureVariableFileName[Index],
                                EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,0);
                            if(!EFI_ERROR(Status))
                            {
                                // Write
                                Status = FileHandle->Write(FileHandle,&size, Data);
                                FileHandle->Close(FileHandle);
                                nVars++;
                            }
                        }
                    }
                }
               if (EFI_ERROR(Status)) {
                    bFound = FALSE;
                    GetHiiString(HiiHandle, STRING_TOKEN(STR_WRITE_ERROR_TITLE), sizeof(StrTitle),StrTitle);
                    mPostMgr->DisplayMsgBox( StrTitle, SecureVariableFileName[Index], MSGBOX_TYPE_OK, NULL );
                }
            }
            if(Data!=NULL)
                pBS->FreePool(Data);
        }
        Index++;
        if(Key != SecureBootPolicyVariableCount) { Index = SecureBootPolicyVariableCount; break; }
    }
    if (Index==SecureBootPolicyVariableCount && bFound) {
        GetHiiString(HiiHandle, STRING_TOKEN(STR_SAVE_SEC_KEY),sizeof(StrMessage), StrMessage);
        Swprintf_s(StrTemp, sizeof(StrTemp), StrMessage, nVars);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_SAVE_SEC_KEY_TITLE),sizeof(StrTitle), StrTitle);
        mPostMgr->DisplayMsgBox(StrTitle , StrTemp, MSGBOX_TYPE_OK, NULL );
    }

Done:

    if(FileBuf)
        pBS->FreePool(FileBuf);

    if(FilePath)
        pBS->FreePool(FilePath);

    return EFI_SUCCESS;
}

/*
display MsgBox with drop down list of Certificates included in the signature database db(x,r,t,)
*/
EFI_STATUS GetCertificateListInfoFromVar(
    IN UINT8       Index,
    IN UINT16      TitleToken,
    IN OUT UINT8 **VarBuf,
    IN OUT UINTN *VarSize
)
{
    EFI_STATUS  Status;
    EFI_GUID    *EfiVarGuid;

    if(VarSize == NULL)
        return EFI_BUFFER_TOO_SMALL;
    if(Index >= SecureBootPolicyVariableCount)
        return EFI_ACCESS_DENIED;

    *VarSize = 0;
    if(Index < SecureVariableSigDatabaseCount)
        EfiVarGuid = &gEfiImageSecurityDatabaseGuid;
    else
        EfiVarGuid = &gEfiGlobalVariableGuid;
    Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, VarSize, NULL);
    if(Status != EFI_BUFFER_TOO_SMALL)
        return EFI_ACCESS_DENIED;

    Status = pBS->AllocatePool(EfiBootServicesData, *VarSize, (void**)VarBuf);
    ASSERT_EFI_ERROR (Status);
    Status = pRS->GetVariable( SecureVariableFileName[Index], EfiVarGuid, NULL, VarSize, *VarBuf);
    if (!EFI_ERROR(Status))
        Status = SignatureListInfo(TitleToken, *VarBuf, VarSize);

    return Status;
}

VOID 
GetSecureBootVarCertificateInfo(
    EFI_HII_HANDLE HiiHandle,
    UINT8          Index,
    UINT16         TitleToken
)
{
    UINT8 *VarBuf;
    UINTN VarSize;

    VarBuf = NULL;
    GetCertificateListInfoFromVar(Index, TitleToken, &VarBuf, &VarSize);
    if(VarBuf!=NULL)
        pBS->FreePool(VarBuf);
}

//--------------------------------------------------------------
// Key Management Callbacks
//---------------------------------------------------------------
EFI_STATUS
ManageSecureBootKeysCallback(EFI_HII_HANDLE HiiHandle, UINT16 Class, UINT16 SubClass, UINT16 Key)
{
    EFI_STATUS Status;
    UINT8 index;
    UINT16 CertSel = 0;
    UINT8 MenuSize, DelItem, SetItem, AppendItem;
    UINT8 KeyInfo, SaveItem;
    POSTMENU_TEMPLATE MenuList[5]; // Info/Save/Set/Append/Delete
    CALLBACK_PARAMETERS *Callback;

    Callback = GetCallbackParameters();
    if(!Callback || Callback->Action != EFI_BROWSER_ACTION_CHANGING)
        return EFI_UNSUPPORTED;

    switch(Key)
    {
    case MANAGE_PK_KEY:
                    index = PkVar;
                    break;
    case MANAGE_KEK_KEY:
                    index = KekVar;
                    break;
    case MANAGE_DB_KEY:
                    index = DbVar;
                    break;
    case MANAGE_DBR_KEY:
                    index = DbrVar;
                    break;
    case MANAGE_DBT_KEY:
                    index = DbtVar;
                    break;
    case MANAGE_DBX_KEY:
                    index = DbxVar;
                    break;
    default:
        return EFI_SUCCESS;
    }

    if(mPostMgr == NULL)
        return EFI_SUCCESS;

    gHiiHandle = HiiHandle;

    MenuSize = 5;

    KeyInfo  = 0;
    SaveItem = 1;
    SetItem  = 2;
    AppendItem = 3;
    DelItem = 4;

    // Clear the memory allocated 
    MemSet(MenuList, sizeof(MenuList), 0);

    if(bKey[index]==Absent) { // no Key - no Del
        MenuSize = 2; 
        SetItem  = 0;
        AppendItem = 1;
        // hidden
        KeyInfo  = 4;
        SaveItem  = 4;
        DelItem = 4;
    }
    if(Key == MANAGE_PK_KEY) { // PK - no Append
        MenuSize--;
        DelItem--; // move Del up
        AppendItem = 4;
    }

    MenuList[SetItem].ItemToken = STRING_TOKEN(STR_SECURE_SET);
    MenuList[AppendItem].ItemToken = STRING_TOKEN(STR_SECURE_APPEND);
    MenuList[DelItem].ItemToken = STRING_TOKEN(STR_SECURE_DELETE);
    MenuList[SaveItem].ItemToken = STRING_TOKEN(STR_SAVE_VAR_DATA);
    MenuList[KeyInfo].ItemToken = STRING_TOKEN(STR_SECURE_KEY_INFO);

    // Call post manager to display the menu
    Status = mPostMgr->DisplayPostMenu(HiiHandle,
                        SecureVariableStringRef[index],
                        0,//STRING_TOKEN(STR_KEY_MANAGEMENT_TITLE),
                        MenuList,
                        MenuSize,
                        &CertSel);

    if(!EFI_ERROR(Status))
    {
        if(CertSel == DelItem) {
            if(bKey[index] != Absent)
                DeleteSecureBootDBCallback( HiiHandle,  index);
        }
        if(CertSel == SetItem) {
            SetAppendSecureBootDBCallback( HiiHandle, SET_SECURE_VARIABLE_SET | SET_SECURE_VARIABLE_DEL, index);
        }
        if(CertSel == AppendItem) {
            if(Key != MANAGE_PK_KEY)
                SetAppendSecureBootDBCallback( HiiHandle,  SET_SECURE_VARIABLE_SET | SET_SECURE_VARIABLE_APPEND,  index);
        }
        if(CertSel == SaveItem) {
            SaveToFileSecureBootVar(HiiHandle, index);
        }
        if(CertSel == KeyInfo) {
            GetSecureBootVarCertificateInfo( HiiHandle,  index, SecureVariableStringRef[index]);
        }
    }
    return EFI_SUCCESS;
}

//
// ENROLL_EFI_IMAGE
//
EFI_STATUS
EnrollCertificateToDbFromEfiImageCallback(EFI_HII_HANDLE HiiHandle, UINT16 Class, UINT16 SubClass, UINT16 Key)
{
    EFI_STATUS Status;
    EFI_HANDLE *FsHandle;
    UINT8 *FileBuf=NULL;
    UINT8 *Data=NULL;
    UINTN size, VarSize;
    CHAR16 *FilePath=NULL;
    UINT16 AddSize;
    UINT8 Sel;

    CALLBACK_PARAMETERS *Callback;

    Callback = GetCallbackParameters();
    if(!Callback || Callback->Action != EFI_BROWSER_ACTION_CHANGING)
        return EFI_UNSUPPORTED;

    if(mPostMgr == NULL)
        return EFI_SUCCESS;

    size = 0;
    AddSize = 0;
    Status = FileBrowserLaunchFileSystem(TRUE, &FsHandle, &FilePath, &FileBuf, &size);
    if(!EFI_ERROR(Status) && FileBuf)
    {
        Status = EFI_LOAD_ERROR;
        if(FALSE == HashPeImage ((const UINT8*)FileBuf, size, mDigest)) {
            goto exit;
        }
        size = SHA256_DIGEST_SIZE;
        //
        // form an AuthVar Hdr on top of Var
        //
        //Allocate new Size
        AddSize = sizeof(EFI_VARIABLE_SIG_HDR_2)-1; // decrement by 1 byte as SIG_DATA adds 1 dummy byte
        VarSize = size+AddSize;
        if(EFI_ERROR(pBS->AllocatePool(EfiBootServicesData, VarSize, (void**)&Data)))
            goto exit;
        MemSet(Data, VarSize, 0);
        // Append AuthHdr to Var data.
        if(EFI_ERROR(AmiFillAuthenticatedVariableHdr(Data, mDigest, size, SET_SECURE_VARIABLE_APPEND)))
            goto exit;
        MemCpy ((VOID*)((UINTN)Data+AddSize), mDigest, size);

        MemSet(StrTemp, sizeof(StrTemp), 0);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_UPDATE_FROM_FILE), sizeof(StrMessage), StrMessage);
        Swprintf_s(StrTemp, sizeof(StrTemp),StrMessage, SecureVariableFileName[DbVar],FilePath);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_ENROLL_EFI_IMAGE),sizeof(StrTitle), StrTitle);
        mPostMgr->DisplayMsgBox( StrTitle, StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
        if(Sel == 0)
        {
            Status = SetSecureVariable(DbVar, SET_SECURE_VARIABLE_SET | SET_SECURE_VARIABLE_APPEND, Data, VarSize);
        } else
            Status = EFI_ABORTED;
    }
exit:
    if(!EFI_ERROR(Status)){
        UpdateSecureVariableBrowserInfo(); // changes StrTitle and StrMessage
    }
    UnicodeSPrint(StrMessage, sizeof(StrMessage), L"%r", Status);
    GetHiiString(HiiHandle, STRING_TOKEN(STR_ENROLL_EFI_IMAGE),sizeof(StrTitle), StrTitle);
    mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );

    if(Data)
        pBS->FreePool(Data);

    if(FileBuf)
        pBS->FreePool(FileBuf);

    if(FilePath)
        pBS->FreePool(FilePath);

    return EFI_SUCCESS;
}

VOID
SetAppendSecureBootDBCallback(EFI_HII_HANDLE HiiHandle, UINT8 VarSetMode, UINT8 index)
{
    EFI_STATUS Status;
    EFI_HANDLE *FsHandle;
    UINT8 *FileBuf=NULL;
    UINT8 *Data=NULL;
    UINTN size, VarSize;
    CHAR16 *FilePath=NULL;
    UINT8 Sel;
    UINT16 CertSel, AddSize;
    UINT8 MenuSize;
    POSTMENU_TEMPLATE MenuList[3]; // SigList_Var_PE/COFF

    MemSet(StrTemp, sizeof(StrTemp), 0);
    Sel = 1; // No
    CertSel = 0;
    
    if(VarSetMode & SET_SECURE_VARIABLE_APPEND)
        GetHiiString(HiiHandle, STRING_TOKEN(STR_SECURE_APPEND), sizeof(StrTitle), StrTitle);
    else
        GetHiiString(HiiHandle, STRING_TOKEN(STR_SECURE_SET), sizeof(StrTitle), StrTitle);

    GetHiiString(HiiHandle, STRING_TOKEN(STR_UPDATE_FROM_DEFAULTS),sizeof(StrMessage), StrMessage);
    Swprintf_s(StrTemp, sizeof(StrTemp), StrMessage ,  SecureVariableFileName[index]);
    Status = mPostMgr->DisplayMsgBox( StrTitle, StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
    if(Sel == 0)
    {
        size = 0 ; 
        Status = GetSectionFromAnyFv (SecureVariableFileGuid[index], EFI_SECTION_RAW, 0, &FileBuf, (UINTN*)&size);
    } else
    {
        size = 0;
        AddSize = 0;
        MenuSize = (index < SecureVariableSigDatabaseCount)?3:2; // all db(x) can accept certs from PE/COFF
        Status = FileBrowserLaunchFileSystem(TRUE, &FsHandle, &FilePath, &FileBuf, &size);
        if(!EFI_ERROR(Status) && FileBuf)
        {
            // Clear the memory allocated 
            MemSet(MenuList, sizeof(MenuList), 0);
            MenuList[0].ItemToken = STRING_TOKEN(STR_SECURE_CER);
            MenuList[1].ItemToken = STRING_TOKEN(STR_SECURE_VAR);
            MenuList[2].ItemToken = STRING_TOKEN(STR_SECURE_EFI);
            // Call post manager to display the menu
            Status = mPostMgr->DisplayPostMenu(gHiiHandle,
                                                STRING_TOKEN(STR_SECURE_FILE_TITLE), // Change this
                                                0,
                                                MenuList,
                                                MenuSize,
                                                &CertSel);
            if(!EFI_ERROR(Status))
            {
                Status = EFI_LOAD_ERROR;
                if(CertSel==2) { // Efi executable
                    if(FALSE == HashPeImage ((const UINT8*)FileBuf, size, mDigest)) {
                        goto exit;
                    }
                    pBS->FreePool(FileBuf);
                    FileBuf = &mDigest[0];
                    size = SHA256_DIGEST_SIZE;
                    CertSel = 0; // treat as a Certificate 
                }
                if(CertSel==0) {
                    AddSize = sizeof(EFI_VARIABLE_SIG_HDR_2)-1; // decrement by 1 byte as SIG_DATA adds 1 dummy byte
                }
                // Validate Signature List integrity 
                if(!EFI_ERROR(AmiValidateSignatureList (FileBuf, size, NULL, NULL))) {
                    CertSel=MenuSize;
                    AddSize = sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2);
                }
                //
                // form an AuthVar Hdr on top of Var
                //
                //Allocate new Size
                VarSize = size+AddSize;
                if(EFI_ERROR(pBS->AllocatePool(EfiBootServicesData, VarSize, (void**)&Data)))
                    goto exit;
                MemSet(Data, VarSize, 0);
                // Append AuthHdr to Var data.
                if(CertSel==0) {
                    
            /*
             * Enter new OwnerGuid or use a default one-> initialized with Ami Guid
             */
#define SBOOT_CERT_OWNER_GUID_SIZE                 36
#define SBOOT_CERT_OWNER_GUID_STORAGE_SIZE         37
                    EFI_GUID        OwnerGuid;
            static CHAR16          *gGuidEntered = NULL;
                    UINT16          SetOwnerGuidTitle = 0;
                    TEXT_INPUT_TEMPLATE     SetOwnerGuidPrompt[1] = {
                            {STRING_TOKEN(STR_SBOOT_CERT_OWNER_GUID), SBOOT_CERT_OWNER_GUID_SIZE, TSE_FILTER_KEY_NULL, FALSE, NULL}};

                    if(gGuidEntered == NULL) {
                        if(EFI_ERROR(pBS->AllocatePool(EfiBootServicesData, SBOOT_CERT_OWNER_GUID_STORAGE_SIZE, (void**)&gGuidEntered))) {
                            goto exit;
                        }
                        // Set guid str buffer to a default OwnerGuid
                        Swprintf_s(gGuidEntered, SBOOT_CERT_OWNER_GUID_STORAGE_SIZE, L"%G", &gAmiSigOwherGuid);
                    }
                    SetOwnerGuidTitle = STRING_TOKEN(STR_SBOOT_CERT_OWNER_GUID_TITLE);
                    SetOwnerGuidPrompt[0].ItemToken = STRING_TOKEN(STR_SBOOT_CERT_OWNER_GUID);
                    SetOwnerGuidPrompt[0].Value = gGuidEntered;
                    //DEBUG((SecureBoot_DEBUG_LEVEL, "Guid Before '%s'\n", gGuidEntered));
                    //Get Owner Guid
                    if (EFI_ERROR(mPostMgr->DisplayTextBox (gHiiHandle, SetOwnerGuidTitle, SetOwnerGuidPrompt, 1, NULL)) 
                    ) {
                        goto exit;
                    }
                    //DEBUG((SecureBoot_DEBUG_LEVEL, "Guid after '%s'\n",  gGuidEntered));
                    if ( *(CHAR16*)(SetOwnerGuidPrompt[0].Value) ) {
                        if (RETURN_ERROR (StrToGuid (gGuidEntered, &OwnerGuid)) || (gGuidEntered[GUID_STRING_LENGTH] != L'\0')) {
                            goto exit;
                        }
                        //DEBUG((SecureBoot_DEBUG_LEVEL, "OwnerGuid '%g'\n",  OwnerGuid));
                        if(EFI_ERROR(AmiFillAuthenticatedVariableHdr2(Data, FileBuf, size, &OwnerGuid, VarSetMode))) {
                            goto exit;
                        }
                    }
                }
                else 
                    if(CertSel==MenuSize) // unsupported - append from SigList
                        AmiFillAuthenticatedHdr2(Data, VarSetMode);

                MemCpy ((VOID*)((UINTN)Data+AddSize), FileBuf, size);
                if(FileBuf!=&mDigest[0])
                    pBS->FreePool(FileBuf);
                FileBuf = Data;
                size = VarSize;
                Status = EFI_SUCCESS;
            }
        }
    }
    if(!EFI_ERROR(Status) && FileBuf) {

        if(Sel == 1) {
            GetHiiString(HiiHandle, STRING_TOKEN(STR_UPDATE_FROM_FILE), sizeof(StrMessage), StrMessage);
            Swprintf_s(StrTemp, sizeof(StrTemp),StrMessage, SecureVariableFileName[index],FilePath);
            mPostMgr->DisplayMsgBox( StrTitle, StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
        }
        if(Sel == 0)
        {
            Status = SetSecureVariable(index, VarSetMode, FileBuf, size);
        } else 
            Status = EFI_ABORTED;
    }
exit:
    UnicodeSPrint(StrMessage, sizeof(StrMessage), L"%r", Status);
    mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );
    UpdateSecureVariableBrowserInfo(); // changes StrTitle and StrMessage

    if(Data != FileBuf)
        pBS->FreePool(Data);

    if(FileBuf)
        pBS->FreePool(FileBuf);

    if(FilePath)
        pBS->FreePool(FilePath);
}

VOID
DeleteSecureBootDBCallback(EFI_HII_HANDLE HiiHandle, UINT8 index)
{
    EFI_STATUS Status;
    UINT8      Sel = 0;
    UINTN      size, VarSize;
    UINT8      *Data=NULL, *VarBuf=NULL;

    if(index >= SecureBootPolicyVariableCount)
        return;

    GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_SEC_KEY_TITLE), sizeof(StrTitle),StrTitle);
    if(index == PkVar) {
        GetHiiString(HiiHandle, STRING_TOKEN(STR_PK_DEL_WARNING), sizeof(StrTemp),StrTemp);
        Status = mPostMgr->DisplayMsgBox( StrTitle,  StrTemp, (UINT8)MSGBOX_TYPE_OK, &Sel);
    } else {
        MemSet(StrTemp, sizeof(StrTemp), 0);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_KEY_OPTION), sizeof(StrMessage), StrMessage);
        Swprintf_s(StrTemp, sizeof(StrTemp), StrMessage, SecureVariableFileName[index]);
        Status = mPostMgr->DisplayMsgBox( StrTitle,  StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
    }
    if(Sel == 0)
    {
        GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_SEC_KEY), sizeof(StrMessage), StrMessage);
        MemSet(StrTemp, sizeof(StrTemp), 0);
        Swprintf_s(StrTemp, sizeof(StrTemp), StrMessage, SecureVariableFileName[index]);
        mPostMgr->DisplayMsgBox( StrTitle,  StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
        if(Sel == 0)
        {
            Status = SetSecureVariable(index, SET_SECURE_VARIABLE_DEL, NULL, 0);
        }
    } else {
        Status = GetCertificateListInfoFromVar(index, STRING_TOKEN(STR_CERT_DEL_INFO_TITLE), &VarBuf, &VarSize);
        if (EFI_ERROR(Status))
            goto exit;
        GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_SEC_KEY_TITLE), sizeof(StrTitle),StrTitle);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_CERT_FROM_DB), sizeof(StrMessage), StrMessage);
        MemSet(StrTemp, sizeof(StrTemp), 0);
        Swprintf_s(StrTemp, sizeof(StrTemp), StrMessage, SecureVariableFileName[index]);
        mPostMgr->DisplayMsgBox( StrTitle,  StrTemp, (UINT8)MSGBOX_TYPE_YESNO, &Sel);
        if(Sel == 0)
        {
            size = VarSize+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2);
            if(EFI_ERROR(pBS->AllocatePool(EfiBootServicesData, size, (void**)&Data)))
                goto exit;
            MemSet(Data, size, 0);
            AmiFillAuthenticatedHdr2(Data, SET_SECURE_VARIABLE_SET);
            MemCpy ((VOID*)((UINTN)Data+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2)), VarBuf, VarSize);

            Status = SetSecureVariable(index, SET_SECURE_VARIABLE_SET | SET_SECURE_VARIABLE_DEL, Data, size);
        }
    }
    if(Sel == 0)
    {
        if(!EFI_ERROR(Status)){
            UpdateSecureVariableBrowserInfo(); // changes StrTitle and StrMessage
        }
        UnicodeSPrint(StrMessage, sizeof(StrMessage), L"%r", Status);
        GetHiiString(HiiHandle, STRING_TOKEN(STR_DELETE_SEC_KEY_TITLE),sizeof(StrTitle), StrTitle);
        mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );
    }
exit:
    if(VarBuf!=NULL)
        pBS->FreePool(VarBuf);
    if(Data!=NULL)
        pBS->FreePool(Data);
}

EFI_STATUS
GetSecureBootDBCallback(EFI_HII_HANDLE HiiHandle, UINT16 Class, UINT16 SubClass, UINT16 Key)
{
    CALLBACK_PARAMETERS *Callback;

    Callback = GetCallbackParameters();
    if(!Callback || Callback->Action != EFI_BROWSER_ACTION_CHANGING)
       return EFI_UNSUPPORTED;

    gHiiHandle = HiiHandle;

    SaveToFileSecureBootVar(HiiHandle, SecureBootPolicyVariableCount);

    return EFI_SUCCESS;
}
// ----------------------------------------------------------------------------
// Auxiliary functions to display File Browser menu
// ----------------------------------------------------------------------------
static VOID EfiStrCat (
    IN CHAR16   *Destination,
    IN CHAR16   *Source
    )
{   
    Wcscpy (Destination + Wcslen (Destination), Source);
}

static CHAR16 *StrDup8to16( CHAR8 *string )
{
    CHAR16  *text;
    UINTN   i;

    if ( string == NULL )
        return NULL;

    pBS->AllocatePool(EfiBootServicesData, (1 + Strlen( string )) * sizeof(CHAR16), (void**)&text);

    if ( text != NULL )
    {
        i=0;
        while(text[i] = (CHAR16)string[i])
            i++;
    }

    return text;
}
 
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:   GetHiiString
//
// Description: This function Reads a String from HII
//
// Input:       IN EFI_HII_HANDLE   HiiHandle - Efi Hii Handle
//              IN STRING_REF       Token     - String Token
//              IN OUT UINTN        *pDataSize - Length of the StringBuffer
//              OUT EFI_STRING      *ppData - The buffer to receive the characters in the string.
//
// Output:      EFI_STATUS - Depending on result
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID GetHiiString(
    IN EFI_HII_HANDLE HiiHandle,
    IN STRING_REF Token,
    IN  UINTN DataSize,
    IN OUT CHAR16  *pData
    )
{
    EFI_STATUS Status;

    if (!pData) return;

    Status = HiiLibGetString(HiiHandle, Token, &DataSize, (EFI_STRING)pData);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status))  Wcscpy(pData,(CHAR16*)L"??? ");

//DEBUG((SecureBoot_DEBUG_LEVEL,"%r, StrRef '%s', Size %Xh, Token=%d\n",Status, ppData, DataSize, Token));
}

// Add the string to the HiiDataBase
STRING_REF HiiAddString(IN EFI_HII_HANDLE HiiHandle,IN CHAR16 *String)
{
    EFI_STATUS Status;
    CHAR8* Languages = NULL;
    UINTN LangSize = 0;
    CHAR8* CurrentLanguage;
    BOOLEAN LastLanguage = FALSE;
    EFI_STRING_ID  StringId = 0;
static CHAR8  *SupportedLanguages=NULL;

    if(HiiString == NULL) {
        Status = pBS->LocateProtocol(&gEfiHiiStringProtocolGuid, NULL, (VOID **) &HiiString);
        if(EFI_ERROR(Status))
            return 0;
    }

    if(SupportedLanguages == NULL) {
        Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &LangSize);
        if(Status == EFI_BUFFER_TOO_SMALL) {
            Status = pBS->AllocatePool(EfiBootServicesData, LangSize, (void**)&SupportedLanguages);
            if(EFI_ERROR(Status))
                return 0;        //not enough resources to allocate string
            Status = HiiString->GetLanguages(HiiString, HiiHandle, SupportedLanguages, &LangSize);
        }
        if(EFI_ERROR(Status) || SupportedLanguages == NULL)
            return 0;
    }

    Languages = SupportedLanguages;

    while(!LastLanguage) {
        CurrentLanguage = Languages;        //point CurrentLanguage to start of new language
        while(*Languages != ';' && *Languages != 0)
            Languages++;

        if(*Languages == 0) {       //last language in language list
            LastLanguage = TRUE;
            if(StringId == 0)
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            else
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            if(EFI_ERROR(Status))
                return 0;
        } else {
            *Languages = 0;         //put null-terminator
            if(StringId == 0)
                Status = HiiString->NewString(HiiString, HiiHandle, &StringId, CurrentLanguage, NULL, String, NULL);
            else
                Status = HiiString->SetString(HiiString, HiiHandle, StringId, CurrentLanguage, String, NULL);
            *Languages = ';';       //restore original character
            Languages++;
            if(EFI_ERROR(Status))
                return 0;
        }
    }

    return StringId;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:    EfiLibAllocateCopyPool
//
// Description:    Allocate BootServicesData pool and use a buffer provided by 
//                    caller to fill it.
//
// Input:    AllocationSize  - The size to allocate
//                    Buffer          - Buffer that will be filled into the buffer allocated
//
// Output:    Pointer of the buffer allocated.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID *
EfiLibAllocateCopyPool (
  IN  UINTN   AllocationSize,
  IN  VOID    *Buffer
  )
{
    VOID  *Memory;

    Memory = NULL;
    pBS->AllocatePool (EfiBootServicesData, AllocationSize, &Memory);
    if (Memory != NULL && Buffer != NULL) {
        pBS->CopyMem (Memory, Buffer, AllocationSize);
    }

    return Memory;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CleanFileTypes
//
// Description: Frees all allocated memory associated with the FILE_TYPE structure
//      and resets the InternalString current strings next available string token
//      to be the first dynamically added string
//
// Input:   FILE_TYPE **FileList - The array of FILE_TYPE structures found in 
//              a directory
//          UINTN *FileCount - pointer to the number of entries in the FileList
//
// Output:
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID CleanFileTypes(FILE_TYPE **FileList, UINTN *FileCount)
{
    UINTN i;
    for(i = 0; i<*FileCount; i++) pBS->FreePool((*FileList)[i].Name);
    if(FileList!=NULL && (*FileList!=NULL) && (*FileCount>0)) pBS->FreePool(*FileList);
    if(FileList!=NULL) *FileList = NULL;
    *FileCount = 0;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CheckDirectoryType
//
// Description: Checks if the EFI_FILE_INFO is a directory (and not the current directory)
//
// Input:   EFI_FILE_INFO *File
//
// Output:  
//
// Returns: BOOLEAN - TRUE - item is a directory, and not the current directory
//                    FALSE - item is not a directory, or is the current directory
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
/*
BOOLEAN CheckDirectoryType(EFI_FILE_INFO *File)
{
    BOOLEAN Status = FALSE;

    if((File->Attribute & EFI_FILE_DIRECTORY) && (Wcscmp(File->FileName, L".") != 0)) {

        Status = TRUE;
    }

    return Status;
}
*/
//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CheckExtension
//
// Description: Check is the EFI_FILE_INFO has the same extension as the 
//      extension passed in the second parameter
//
// Input:   EFI_FILE_INFO *File - The file entry whose extension should be checked
//          CHAR16 *ExtensionEfi - the extension
//
// Output:
//
// Returns: BOOLEAN - TRUE - The extension matches
//                    FALSE - the extension does not match
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN CheckExtension(EFI_FILE_INFO *File, CHAR16 *ExtensionEfi)
{
    BOOLEAN Status = FALSE;
    UINTN Length = Wcslen(File->FileName);

    if((File->Attribute & EFI_FILE_DIRECTORY) != EFI_FILE_DIRECTORY && Length > 3)
        if((((File->FileName[Length-1])&0xdf) == ((ExtensionEfi[2])&0xdf)) &&
           (((File->FileName[Length-2])&0xdf) == ((ExtensionEfi[1])&0xdf)) &&
           (((File->FileName[Length-3])&0xdf) == ((ExtensionEfi[0])&0xdf)))
            Status = TRUE;
    return Status;    
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   FindInsertionIndex
//
// Description: Finds the index where directories items turn into file items
//
// Input:   FILE_TYPE *List - the current array of File Type structures
//          UINTN FileCount - the count of File Type structures in the array
//
// Output:
//
// Returns: the index to insert a new item
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
UINTN FindInsertionIndex(FILE_TYPE *List, UINTN FileCount)
{
    UINTN i = 0;
    
    if(FileCount <= 1) return 0;

    for(i = 1; i < (FileCount-1); i++)
    {
        if(List[i-1].Type != List[i].Type)
        break;
    }

    return i;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   AddFileTypeEntry
//
// Description: Creates a new entry in the FILE_TYPE array and adds the current File into
//      the array.
//
// Input:   FILE_TYPE **List - Array of FILE_TYPE structures already found
//          UINTN *FileCount - number of entries in the FILE_TYPE array
//          EFI_FILE_INFO *FileInfo - file info of the file that should be added
//
// Output:
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
static VOID AddFileTypeEntry(FILE_TYPE **List, UINTN *FileCount, EFI_FILE_INFO *FileInfo)
{
    FILE_TYPE *NewList=NULL;
    UINTN Length;
    UINTN Index = 0;

    Length = (Wcslen(FileInfo->FileName)+3)*sizeof(CHAR16);

    // Allocate space for a new list entry plus all the previous list items
    NewList = EfiLibAllocateCopyPool(sizeof(FILE_TYPE)*(++(*FileCount)), NewList);
    if (NewList != NULL) 
    {
        // Clear the memory of the entire list
        MemSet(NewList, sizeof(FILE_TYPE)*(*FileCount), 0);
    
        // Copy the old entries (if there are any old entries to copy)
        if(*List != NULL) 
        {
            Index = FindInsertionIndex(*List, *FileCount);

            pBS->CopyMem(NewList, *List, sizeof(FILE_TYPE)*(Index));
            pBS->CopyMem(&(NewList[Index+1]), &((*List)[Index]), sizeof(FILE_TYPE)*((*FileCount)-Index-1));

            pBS->FreePool(*List);
        }

        // Store the type of this FILE_TYPE entry (non-zero is directory)
        NewList[Index].Type = ((FileInfo->Attribute) & EFI_FILE_DIRECTORY);

        // Store the size of the file
        NewList[Index].Size = (UINTN)FileInfo->FileSize;

        // Allocate space for the string
        NewList[Index].Name = EfiLibAllocateCopyPool(Length, NewList[Index].Name);
        if((NewList[Index].Name) != NULL )
        {
            // Clear the allocated memory
            MemSet(NewList[Index].Name, Length, 0);

            // Create either a Dir string or a File string for addition to the HiiDataBase
            if(NewList[Index].Type == EFI_FILE_DIRECTORY)
                 Swprintf_s(NewList[Index].Name, Length, L"<%s>", FileInfo->FileName);
            else
                 Swprintf_s(NewList[Index].Name, Length, L"%s", FileInfo->FileName);

            // Add the string to the HiiDataBase
            ///NewList[Index].Token = AddStringToHii(FileInfo->FileName, &gInternalStrings);    ///Just by trying using the following line
            NewList[Index].Token =     HiiAddString(gHiiHandle, NewList[Index].Name );

            // Clear the memory and create the string for the File Structure
            MemSet(NewList[Index].Name, Length, 0);
             Swprintf_s(NewList[Index].Name, Length, L"%s", FileInfo->FileName);
        }
        *List = NewList;
    }
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   CreateFileList
//
// Description: Parse all the files in the current directory and add valid files to the
//      FILE_TYPE list and update the filecount
//
// Input:   EFI_FILE_PROTOCOL *FileProtocol - the current directory to parse
//
// Output:  FILE_TYPE **FileList - pointer in which to return the array of FileType items
//          UINTN *FileCount - the count of filetype items discovered
//
// Returns: EFI_STATUS
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
static EFI_STATUS CreateFileList(EFI_FILE_PROTOCOL *FileProtocol, FILE_TYPE **FileList, UINTN *FileCount)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINTN   BufferSize = 1;
    EFI_FILE_INFO *File = NULL;

//    CHAR16 ExtensionEfi[] = L"EFI";

    // Continue parsing the directory until we no longer read valid files
    while(BufferSize != 0 && !EFI_ERROR(Status))
    {
        BufferSize = 0;
        Status = FileProtocol->Read(FileProtocol, &BufferSize, NULL);

        if(!EFI_ERROR(Status)) break;

        if(Status == EFI_BUFFER_TOO_SMALL)
        {
            File = EfiLibAllocateCopyPool(BufferSize, File);
            if(File != NULL) {
                    MemSet(File, BufferSize, 0);
                }
        }

        Status = FileProtocol->Read(FileProtocol, &BufferSize, File);

        // Check if a valid file was read
        if(!EFI_ERROR(Status) && BufferSize != 0)
        {
            // check if the file read was a directory or a ".efi" extension
//            if(CheckDirectoryType(File) ||  CheckExtension(File, ExtensionEfi))
//            {
                // the file was valid, add it to the file list
                AddFileTypeEntry(FileList, FileCount, File);
//            }
        }

        // free the space allocated for reading the file info structure
        pBS->FreePool(File);

        // set the pointer to null to prevent the chance of memory corruption
        File = NULL;
    }

    if(*FileCount == 0) Status = EFI_NOT_FOUND;

    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   DisplayFileListMenu
//
// Description: Display a menu of the FILE_TYPE items and return the selected item
//              in the Selection
//
// Input:   FILE_TYPE *FileList - List of FILE_TYPE items to display in the menu
//          UINTN FileCount - the number of FILE_TYPE items in the list
//
// Output:  UINT16 *Selection - The index of the selected FILE_TYPE item
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
static EFI_STATUS DisplayFileListMenu(FILE_TYPE *FileList, UINTN FileCount, UINT16 *Selection)
{
    EFI_STATUS Status = EFI_SUCCESS;

    UINT16 i = 0;

    POSTMENU_TEMPLATE *List = NULL;

    // Check there are any files to display
    if(FileCount != 0 && FileList != NULL)
    {
        // allocate space for the POSTMENU_TEMPLATE items
        List = EfiLibAllocateCopyPool(sizeof(POSTMENU_TEMPLATE)*FileCount, List);
        if(List != NULL)
        {
            // Clear the memory allocated 
            MemSet(List, sizeof(POSTMENU_TEMPLATE)*FileCount, 0);

            // Add the STRING_REF tokens to the POSTMENU_TEMPLATE structures
            for(i = 0; i < FileCount; i++)
                List[i].ItemToken = FileList[i].Token;
        }

        // Call post manager to display the menu
        Status = mPostMgr->DisplayPostMenu(gHiiHandle,
                                            STRING_TOKEN(STR_FILE_SELECT), // Change this
                                            0,
                                            List,
                                            (UINT16)FileCount,
                                            Selection);
    }
    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   UpdateFilePathString
//
// Description: To create the File Path string based on the file selected.
//
// Input:   CHAR16 *FilePath  - Buffer to fill with the file path
//          CHAR16 * CurFile  - current file selected
//          UINT16 idx        - Index of the file in the current directory
//
// Output:  CHAR16 *FilePath - Updated File Path
//
// Returns:
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UpdateFilePathString(CHAR16 *FilePath, CHAR16 * CurFile, UINT16 idx)
{
    UINTN Length=0;

    if(Wcslen(FilePath))
    {
        if( idx==0 ) {
            if(Wcscmp(CurFile,L".."))  {
                EfiStrCat(FilePath,L"\\");
                EfiStrCat(FilePath,CurFile);
            }
            else {
                
                for ( Length = Wcslen(FilePath); ( Length!= 0 ) && (FilePath[Length-1] != L'\\') ; Length -- ); 
                    if ( Length )
                        FilePath[Length-1] = L'\0';
                    else
                        FilePath[Length] = L'\0'; 
            }
        }
        else {
            EfiStrCat(FilePath,L"\\");
            EfiStrCat(FilePath,CurFile);
        }
    }
    else {
        Wcscpy(FilePath,CurFile);
    }
}

EFI_STATUS FileBrowserLaunchFilePath(IN EFI_HANDLE *FileHandle, IN BOOLEAN ValidOption, OUT CHAR16 **outFilePath, OUT UINT8 **outFileBuf,OUT UINTN *size );

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   FileBrowserLaunchFileSystem
//
// Description: To select the File System for the new boot option with the help of file browser.
//
// Input:   BOOLEAN bSelectFile - TRUE  - Select FSHandle and File path
//                                FALSE - Select only FSHandle
//
// Output:  Selected File System Index
//
// Returns: EFI_STATUS
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS FileBrowserLaunchFileSystem(BOOLEAN bSelectFile, OUT EFI_HANDLE **outFsHandle,OUT CHAR16 **outFilePath, OUT UINT8 **outFileBuf,OUT UINTN *size )
{
    EFI_STATUS Status;
    UINTN Count = 0;
    UINT16 i = 0;
    EFI_HANDLE *gSmplFileSysHndlBuf = NULL;    
    UINT16 gSelIdx=0;
    BOOLEAN ValidOption = FALSE;
    
    EFI_DEVICE_PATH_PROTOCOL *Dp = NULL;

    POSTMENU_TEMPLATE *PossibleFileSystems = NULL;

    if(mPostMgr == NULL)
        return EFI_UNSUPPORTED;
    
    // To launch the files from the selected file system
    if(!size)
        return EFI_INVALID_PARAMETER;

    // Locate all the simple file system devices in the system
    Status = pBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &Count, &gSmplFileSysHndlBuf);
    if(!EFI_ERROR(Status))
    {
        // allocate space to display all the simple file system devices
        PossibleFileSystems = EfiLibAllocateCopyPool(sizeof(POSTMENU_TEMPLATE)*Count,PossibleFileSystems);
        if(PossibleFileSystems != NULL)
        {
            // clear the allocated space
            MemSet(PossibleFileSystems, sizeof(POSTMENU_TEMPLATE)*Count, 0);
            for(i = 0; i < Count; i++)
            {
                // get the device path for each handle with a simple file system
                Status = pBS->HandleProtocol(gSmplFileSysHndlBuf[i], &gEfiDevicePathProtocolGuid, &Dp);
                if(!EFI_ERROR(Status))
                {
                    CHAR16 *Name;
                    CHAR8  *Name8;
                    // Get the name of the driver installed on the handle
                    // GetControllerName(gHandleBuffer[i],&Name);

                    Name8 = NULL;
                    Status  = DevicePathToStr(Dp, &Name8 );
                    Name = StrDup8to16(Name8);

                    // Add the name to the Hii Database
                    //PossibleFileSystems[i].ItemToken = AddStringToHii(Name);
                    PossibleFileSystems[i].ItemToken = HiiAddString(gHiiHandle, Name ); 

                    PossibleFileSystems[i].Attribute = AMI_POSTMENU_ATTRIB_FOCUS;
                    pBS->FreePool(Name);
                    pBS->FreePool(Name8);
                }
                else
                {
                    PossibleFileSystems[i].ItemToken = 0;
                    PossibleFileSystems[i].Attribute = AMI_POSTMENU_ATTRIB_HIDDEN;
                }
            }
            // Reset the item selected to be the first item
            gSelIdx = 0;

            // Display the post menu and wait for user input
            Status = mPostMgr->DisplayPostMenu(gHiiHandle,
                                                STRING_TOKEN(STR_FILE_SYSTEM),
                                                0,
                                                PossibleFileSystems,
                                                (UINT16)Count,
                                                &gSelIdx);

            // A valid item was selected by the user
            if(!EFI_ERROR(Status))
            {
                ValidOption = TRUE;
            }
        }
        // Free the allocated menu list space
        if(PossibleFileSystems != NULL) 
            pBS->FreePool(PossibleFileSystems);

         *outFsHandle = gSmplFileSysHndlBuf[gSelIdx];

         *size = 0; 
         if(bSelectFile)
             Status = FileBrowserLaunchFilePath(*outFsHandle, ValidOption, outFilePath, outFileBuf,size );//EIP:41615 Returning the status of Filebrowselaunchfilepath
    }
    else {
        GetHiiString(gHiiHandle, STRING_TOKEN(STR_NO_VALID_FS_TITLE),  sizeof(StrTitle) ,StrTitle);
        GetHiiString(gHiiHandle, STRING_TOKEN(STR_NO_VALID_FS), sizeof(StrMessage),StrMessage);
        mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );//EIP:41615  To display Warning message when there is no file system connected.
    }
    
     if(gSmplFileSysHndlBuf != NULL) 
        pBS->FreePool(gSmplFileSysHndlBuf);

    return Status;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------
// Procedure:   FileBrowserLaunchFilePath
//
// Description: To select the Boot file for the new boot option with the help of file browser.
//
// Input:   VOID
//
// Output:  File Path string
//
// Returns: EFI_STATUS
//
//----------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS FileBrowserLaunchFilePath(IN EFI_HANDLE *FileHandle,IN BOOLEAN ValidOption, OUT CHAR16 **outFilePath, OUT UINT8 **outFileBuf,OUT UINTN *size )
{
    EFI_STATUS Status;

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFs = NULL;
    EFI_FILE_PROTOCOL *NewFs = NULL;
    FILE_TYPE *FileList = NULL;
    UINTN FileCount = 0;
    UINT16 i = 0;
    CHAR16 FilePath[120];
    EFI_FILE_PROTOCOL *gFs = NULL;

    // Attempt to locate the post manager protocol
    if(mPostMgr == NULL)
        return EFI_UNSUPPORTED;

    if( ValidOption == TRUE ) 
    {

        // Get the simple file system protocol 
        Status = pBS->HandleProtocol(FileHandle, &gEfiSimpleFileSystemProtocolGuid, &SimpleFs);
        if(!EFI_ERROR(Status))
        {
            // And open it and return the efi file protocol
            Status = SimpleFs->OpenVolume(SimpleFs, &gFs);
        }
    }
    else {
            return EFI_ABORTED;
    }

    // clean up the file list and strings used in getting the file system
    CleanFileTypes(&FileList, &FileCount);

    while(!EFI_ERROR(Status) && gFs != NULL)
    {
        i = 0;
        MemSet(FilePath, sizeof(FilePath), 0);

        // Create a list of the files in the current directory
        Status = CreateFileList(gFs, &FileList, &FileCount);
        if(!EFI_ERROR(Status))
        {
            // Display the list in a menu and allow the user to select
            Status = DisplayFileListMenu(FileList, FileCount, &i);
            if(!EFI_ERROR(Status))
            {
                // The user selected something, attempt to open it
                Status = gFs->Open(  gFs,
                                    &NewFs,
                                    FileList[i].Name,
                                    EFI_FILE_MODE_READ,
                                    0);

                // close the old file system protocol and set it to null
                gFs->Close(gFs);
                gFs = NULL;

                // Create the File Path based on the file selected
                UpdateFilePathString(FilePath, FileList[i].Name, i);

                // the newly selected item was opened correctly
                if(!EFI_ERROR(Status))
                {
                    // check what type was opened
                    if(FileList[i].Type != EFI_FILE_DIRECTORY)
                    {

                        Status = pBS->AllocatePool(EfiBootServicesData,FileList[i].Size, (VOID**)outFileBuf);
                        if(!EFI_ERROR(Status))
                        {
                            *size = FileList[i].Size;
                            // The user selected something, attempt to open it
                            Status = NewFs->Read( NewFs, size, *outFileBuf); }

                        // the file was read, close the file system protocol and set it to null
                        NewFs->Close(NewFs);
                        NewFs = NULL;
                        //Swprintf_s (FileName, 50, L"%s", FileList[i].Name);
                        //ShowPostMsgBox( L"Selected Boot File Name", FileName, MSGBOX_TYPE_OK, &SelOpt );
                    }
                    gFs = NewFs;
                }
            }
        }

        if(FileCount <= 0) {
            GetHiiString(gHiiHandle, STRING_TOKEN(STR_NO_VALID_FILE_TITLE),sizeof(StrTitle), StrTitle);
            GetHiiString(gHiiHandle, STRING_TOKEN(STR_NO_VALID_FILE),sizeof(StrMessage),StrMessage);
            mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );//EIP:41615 Warning message to show unavailability of the selected file
        }

        // clean the strings that were used and free allocated space
        CleanFileTypes(&FileList, &FileCount);

        if(Status == EFI_ABORTED) {
            return Status;//EIP:41615 Returning the status if its aborted.
        }
    }
    // Set the File path for the new boot option added.
    Status = pBS->AllocatePool(EfiBootServicesData, ((Wcslen(FilePath)+1)*sizeof(CHAR16)), (void**)outFilePath);
    Wcscpy (*outFilePath, FilePath);

    return Status;
}

/*
 1. display MsgBox with drop down list 

  Index: Count : Cert Type : Size : Owner GUID    : Key Subject Name
      1    1       X.509     740     XXXXXX..     : MS UEFI CA
      2   13      SHA256    1370     XXXXXX..     : N/A

 Display Subject Name of a x509 Cert and prompt to delete the Sig List
 For Hash certificates display index and first n bytes of hash value...

 scan through SignatureLists
 record ptr to each cert
 Fill in browser info: 
 get Cert Name from Pkcs7Verify
 
 2. To delete the cert from list -> update the Var's SigList and Set Var back to db/dbx/...
*/
EFI_STATUS SignatureListInfo(
    IN UINT16       TitleToken,
    IN VOID         *bufIn,
    IN OUT UINTN    *sizeIn
    )
{
    EFI_STATUS Status;
    UINTN Count, i;
    UINT16 gSelIdx;
    EFI_SIGNATURE_DATA *SigData;
    EFI_SIGNATURE_LIST *SigList;
    UINT8* pCert;
    UINTN  CopySize, SigSize, CertSize, CertCount;
    CHAR8  *Subject = NULL;
    CHAR16 *CertName;
    UINTN  str_len;
    EFI_GUID *OwnerGuid;

    UINTN             *SignatureListPtrList;
    POSTMENU_TEMPLATE *SignatureListMenu;
    EFI_EVENT          Event = NULL;

    if(mPostMgr == NULL)
        return EFI_UNSUPPORTED;

    // To launch the files from the selected file system
    if(sizeIn == NULL || *sizeIn == 0 || bufIn == NULL)
        return EFI_INVALID_PARAMETER; // Sig not found

    SignatureListPtrList = NULL;
    SignatureListMenu = NULL;

    Status = AmiValidateSignatureList (bufIn, *sizeIn, NULL, &Count);
    if(EFI_ERROR(Status))
        goto exit;

    // allocate space to display collection of signature lists including the table header line
    Status = pBS->AllocatePool (EfiBootServicesData, sizeof(POSTMENU_TEMPLATE)*(Count+1), (void**)&SignatureListMenu);
    if(EFI_ERROR(Status) || SignatureListMenu == NULL)
        goto exit;
    MemSet(SignatureListMenu, sizeof(POSTMENU_TEMPLATE)*(Count+1), 0);

    Status = pBS->AllocatePool (EfiBootServicesData, sizeof(UINTN)*Count, (void**)&SignatureListPtrList);
    if(EFI_ERROR(Status) || SignatureListPtrList == NULL)
        goto exit;
    // clear the allocated space
    MemSet(SignatureListPtrList, sizeof(UINTN)*Count, 0);

    if(mAmiSig == NULL)
         Status = pBS->LocateProtocol(&gAmiDigitalSignatureProtocolGuid, NULL, &mAmiSig);
    if(EFI_ERROR(Status) || mAmiSig == NULL)
         goto exit;
    str_len = 256;    // max Subject name string length
    Status = pBS->AllocatePool(EfiBootServicesData, str_len, (void**)&Subject);
    if(EFI_ERROR(Status) || Subject == NULL)
         goto exit;

    SigList  = (EFI_SIGNATURE_LIST *)bufIn;
    for(i = 0; i < Count; i++) {
        SignatureListPtrList[i] = (UINTN)SigList;
        SigData   = (EFI_SIGNATURE_DATA *) ((UINT8 *) SigList + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize);
        pCert = (UINT8*)SigData->SignatureData;
        CertSize = (UINTN)SigList->SignatureSize-sizeof(EFI_GUID);
        CertCount = (SigList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - SigList->SignatureHeaderSize) / SigList->SignatureSize;
        OwnerGuid = (EFI_GUID*)&SigData->SignatureOwner;
        MemSet(StrMessage, sizeof(StrMessage), 0);
        CertName = NULL;
        // cmp Guid -> if x509 -> // Get the Cert Subject name
        // else it's a Hash certificate
        if(!guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertX509Guid)) {
            str_len = 256;
            MemSet(Subject, str_len, 0);
            Status = mAmiSig->Pkcs7Verify (
                 mAmiSig,
                 pCert, CertSize,
                 NULL, 0,
                 &Subject, &str_len,
                 x509GetSubjectsCommonNameStr,
                 RESET // Flags 
            );
            if(EFI_ERROR(Status)){
                AsciiSPrint(Subject, str_len, "!!! %r", Status);
            }
            DEBUG((SecureBoot_DEBUG_LEVEL, "Cert#%d-%d parse %r, Len %Xh\nSubject Name: '%a'\n", i+1, Count, Status, CertSize, Subject));
            CertName = StrDup8to16(Subject);
            GetHiiString(gHiiHandle, STRING_TOKEN(STR_X509_CERT_TYPE), sizeof(StrMessage), StrMessage);
        } else {
// SHA256/x509_RSA2048 Type guid
            if(!guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertSha256Guid)) {
                GetHiiString(gHiiHandle, STRING_TOKEN(STR_SHA256_CERT_TYPE), sizeof(StrMessage), StrMessage);
            } else if(!guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertX509Sha256Guid) ||
                      !guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertX509Sha384Guid) ||
                      !guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertX509Sha512Guid) ) {
                        GetHiiString(gHiiHandle, STRING_TOKEN(STR_X509_SHA_CERT_TYPE), sizeof(StrMessage), StrMessage);
                    } else if(!guidcmp ((EFI_GUID*) &(SigList->SignatureType), &gEfiCertRsa2048Guid) ) {
                                GetHiiString(gHiiHandle, STRING_TOKEN(STR_RSA2048_CERT_TYPE), sizeof(StrMessage), StrMessage);
                            } else {
                                Swprintf_s(StrMessage, 10, L"%G", &(SigList->SignatureType));
                            }
              pBS->AllocatePool(EfiBootServicesData, 1+(14 * sizeof(CHAR16)), (void**)&CertName);
              if(CertName)
                  Swprintf_s(CertName, (14 * sizeof(CHAR16)), L"%X ...", *(UINT64*)pCert);
        }
        MemSet(StrTemp, sizeof(StrTemp), 0);
        Swprintf_s(StrTemp, 10, L"%G", OwnerGuid);
        Swprintf_s(StrTitle, sizeof(StrTitle), L"%4d|%s|%5d|%5d| %s...| %s", i+1, StrMessage, CertCount, CertSize, StrTemp, CertName);
        if(CertName)
            pBS->FreePool(CertName);
        // Add the name to the Hii Database
        SignatureListMenu[i+1].ItemToken = HiiAddString(gHiiHandle, StrTitle );
        SignatureListMenu[i+1].Attribute = AMI_POSTMENU_ATTRIB_FOCUS;

        SigList = (EFI_SIGNATURE_LIST *) ((UINT8 *) SigList + SigList->SignatureListSize);
    } // for

    GetHiiString(gHiiHandle, STRING_TOKEN(STR_CERT_INFO_TITLE), sizeof(StrTitle), StrTitle);
    SignatureListMenu[0].ItemToken = HiiAddString(gHiiHandle, StrTitle );
    SignatureListMenu[0].Attribute = AMI_POSTMENU_ATTRIB_NON_FOCUS;

    // Reset the item selected to be the first item
    gSelIdx = 1;
    while (1) 
    {
        // Display the post menu and wait for user input
        Status = mPostMgr->DisplayPostMenu(gHiiHandle,
                            TitleToken,
                            0,
                            SignatureListMenu,
                            (UINT16)Count+1,
                            &gSelIdx);
//    DEBUG((SecureBoot_DEBUG_LEVEL, "%r Indx=%d\n",  Status, gSelIdx));

        // A valid item was selected by the user
        if(!EFI_ERROR(Status)) {
            SigList = (EFI_SIGNATURE_LIST*)SignatureListPtrList[gSelIdx-1];
            if( TitleToken == STRING_TOKEN(STR_CERT_DEL_INFO_TITLE)) {
             // delete selected SignatureList
             // move cert list up at the address. Upd size etc...
                SigSize = SigList->SignatureListSize;
                CopySize = ((UINTN)bufIn+*sizeIn) - ((UINTN)SigList+SigSize);
                // replace this SigData block with data following it
                MemCpy (SigList, (void*)((UINTN)SigList+SigSize), CopySize);
                *sizeIn = *sizeIn - SigSize;
                break;
            } else {
                SigData   = (EFI_SIGNATURE_DATA *) ((UINT8 *) SigList + sizeof (EFI_SIGNATURE_LIST) + SigList->SignatureHeaderSize);
                pCert = (UINT8*)SigData->SignatureData;
                OwnerGuid = (EFI_GUID*)&SigData->SignatureOwner;
                MemSet(StrTemp, sizeof(StrTemp), 0);
                GetHiiString(gHiiHandle, SignatureListMenu[gSelIdx].ItemToken,sizeof(StrTemp), StrTemp);
                // Note: Offsets 41(Cert Name) and 28(Owner Guid) are calculated within a fixed unicode string: "%4d|%s|%5d|%5d| %s...| %s"
                //        that is mapped to STR_CERT_INFO_TITLE : "List| Sig.Type|Count| Size| Owner GUID  | Certificate Legend"
                Swprintf_s(StrMessage, sizeof(StrMessage), L"%G | %s", OwnerGuid, &StrTemp[41]);
//                mPostMgr->DisplayMsgBox(&StrTitle[28], StrMessage, MSGBOX_TYPE_OK, NULL );
                mPostMgr->DisplayInfoBox(&StrTitle[28], StrMessage, 10, &Event);
            }
        } else { break; }
    } // end while LOOP
exit:
    if (EFI_ERROR(Status) && Status != EFI_ABORTED) {
        GetHiiString(gHiiHandle, STRING_TOKEN(STR_FAILED),sizeof(StrMessage),StrMessage);
        GetHiiString(gHiiHandle, STRING_TOKEN(TitleToken),sizeof(StrTitle), StrTitle);
        mPostMgr->DisplayMsgBox( StrTitle, StrMessage, MSGBOX_TYPE_OK, NULL );
    }
    // Free the allocated menu list space
    if(SignatureListMenu != NULL) 
        pBS->FreePool(SignatureListMenu);
    if(SignatureListPtrList != NULL) 
        pBS->FreePool(SignatureListPtrList);
    if(Subject != NULL) 
        pBS->FreePool(Subject);

    return Status;
}

#endif // #ifndef TSE_FOR_APTIO_4_50
