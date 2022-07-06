//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file

  Implement image verification services for secure boot service

  Caution: This file requires additional review when modified.
  This library will have external input - PE/COFF image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  DxeImageVerificationLibImageRead() function will make sure the PE/COFF image content
  read is within the image buffer.

  DxeImageVerificationHandler(), HashPeImageByType(), HashPeImage() function will accept
  untrusted PE/COFF image and validate its data structure within this image buffer before use.

Copyright (c) 2009 - 2018, Intel Corporation. All rights reserved.<BR>
(C) Copyright 2016 Hewlett Packard Enterprise Development LP<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#pragma warning (disable : 4090)

#include <Token.h>
#include <AmiDxeLib.h>
#include <Protocol/Security.h>
#include <Library/AmiImageTypeLib.h>
#include <Library/PeCoffLib.h>

#include <Setup.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextOut.h>

//DBG
#include <Library/DebugLib.h>
//#include <Library/PerformanceLib.h>

#include <Protocol/AmiDigitalSignature.h>
#include <AmiSecureBootLib.h>
#include <SecureBoot.h>
#include "DxeImageVerificationLib.h"
// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
#include <Library/SafeIntLib.h>
// APTIOV_OVERRIDE_END

//********tmp declaration from EdkIICommon.h
#define MEDIA_RELATIVE_OFFSET_RANGE_DP 0x08

extern VOID ZeroMem (
    OUT VOID    *Buffer,
    IN  UINTN   Size 
);
VOID* CopyMem (
    OUT VOID  *DestinationBuffer,
    IN  VOID  *SourceBuffer,
    IN UINTN  Length
);
// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
#if (defined(ENABLE_IMAGE_EXEC_POLICY_OVERRIDE) && ENABLE_IMAGE_EXEC_POLICY_OVERRIDE == 1)
static EFI_GUID gSecureSetupGuid = SECURITY_FORM_SET_GUID;
#endif
// APTIOV_OVERRIDE_END

static AMI_DIGITAL_SIGNATURE_PROTOCOL *mDigitalSigProtocol = NULL;
//
// Caution: This is used by a function which may receive untrusted input.
// These global variables hold PE/COFF image data, and they should be validated before use.
//
EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION mNtHeader;
static UINT8                               *mImageBase       = NULL;
static UINTN                               mImageSize;
static EFI_GUID                            mCertType;
static UINT32                              mHashAlg;
static UINT32                              mPeCoffHeaderOffset; 
static UINT8                               mImageDigest[MAX_DIGEST_SIZE];
static UINTN                               mImageDigestSize;
static SECURE_BOOT_SETUP_VAR               mSecureBootSetup;
static UINT8                               mSecureBootEnable = SECURE_BOOT_MODE_ENABLE;
static UINT8                               mAuditMode = AUDIT_MODE_DISABLE;

//
// Notify string for authorization UI.
//
CHAR16  mNotifyString1[MAX_NOTIFY_STRING_LEN] = L"\r\nImage verification failed!\r\n";
CHAR16  mNotifyString2[MAX_NOTIFY_STRING_LEN] = L"\r\nLaunch this image anyway? (Y/N)";
CHAR16  mNotifyString3[MAX_NOTIFY_STRING_LEN] = L"Image Certificate not found in Authorized database(db)!";
CHAR16  mNotifyString4[MAX_NOTIFY_STRING_LEN] = L"Image Certificate is found in Forbidden database(dbx)!";
CHAR16  mNotifyString5[MAX_NOTIFY_STRING_LEN] = L"Image is unsigned or Certificate is invalid!";

//6683D10C-CF6E-4914-B5B4-AB8ED7370ED7
EFI_GUID gBootImageCertTblGuid  = AMI_VALID_BOOT_IMAGE_CERT_TBL_GUID;
static UINT32   mTrustSigDbOffs = 0;
static UINT32   mTrustSigDbSize = 0;
//---------------------------------------
// NEW TIME STAMP definitions ECR#1009
//---------------------------------------
static os_time_t   mTimeOfSigningLong;

typedef enum {
    CertUndefined,
    CertSha256,
    CertSha384,
    CertSha512,
    CertX509,
    CertX509Sha256,
    CertX509Sha384,
    CertX509Sha512,
    CertTypeMax
} nCertType;
//---------------------------------------
// NEW TIME STAMP definitions ECR#1009
//---------------------------------------

//************TEMP UTILITY FUNCTIONS. Use EDKII common wrapper lib instead****************
#if defined(CORE_COMBINED_VERSION) && CORE_COMBINED_VERSION >=0x4028b
#else
UINTN StrSize (
    IN CHAR16 *String)
{
  UINTN Size;

  for (Size = 2; *String != L'\0'; String++, Size += 2);

  return Size;
}
#endif

VOID* GetEfiGlobalVariableEx (
    IN CHAR16 *Name,
    IN OUT UINTN *VarSize
    )
{
    EFI_STATUS Status;
    VOID *Var=NULL;

    Status = GetEfiVariable(Name, &gEfiGlobalVariableGuid, NULL, VarSize, &Var);
    return (EFI_ERROR(Status)) ? NULL : Var;
}
EFI_STATUS GetEfiImageSecurityDatabaseVariableEx (
    IN CHAR16 *Name,
    IN OUT VOID **Data,
    IN OUT UINTN *VarSize
    )
{
    EFI_STATUS Status;
    VOID *Var = NULL;

    Status = GetEfiVariable(Name, &gEfiImageSecurityDatabaseGuid, NULL, VarSize, &Var);
    DEBUG ((SecureBoot_DEBUG_LEVEL,"\n===>Download Certificates(%d bytes) from '%s' %r\n", *VarSize, Name, Status));
    *Data = Var;
    return Status;
}

/**
  Reads contents of a PE/COFF image in memory buffer.

  Caution: This function may receive untrusted input.
  PE/COFF image is external input, so this function will make sure the PE/COFF image content
  read is within the image buffer.

  @param  FileHandle      Pointer to the file handle to read the PE/COFF image.
  @param  FileOffset      Offset into the PE/COFF image to begin the read operation.
  @param  ReadSize        On input, the size in bytes of the requested read operation.  
                          On output, the number of bytes actually read.
  @param  Buffer          Output buffer that contains the data read from the PE/COFF image.
  
  @retval EFI_SUCCESS     The specified portion of the PE/COFF image was read and the size 
**/
EFI_STATUS
EFIAPI
DxeImageVerificationLibImageRead (
  IN     VOID    *FileHandle,
  IN     UINTN   FileOffset,
  IN OUT UINTN   *ReadSize,
  OUT    VOID    *Buffer
  )
{

  if (FileHandle == NULL || ReadSize == NULL || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;    
  }

  if ((EFI_MAX_ADDRESS - FileOffset) < *ReadSize) {
    return EFI_INVALID_PARAMETER;
  }

  if (((UINT64)FileOffset + *ReadSize) > mImageSize) {
//    *ReadSize = (UINT32)(mImageSize - FileOffset);
    return EFI_INVALID_PARAMETER;
  }

  if (FileOffset >= mImageSize) {
//    *ReadSize = 0;
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (Buffer, (UINT8 *)((UINTN) FileHandle + FileOffset), *ReadSize);

  return EFI_SUCCESS;
}

/**
  Calculate hash of Pe/Coff image based on the authenticode image hashing in
  PE/COFF Specification 8.0 Appendix A

  Caution: This function may receive untrusted input.
  PE/COFF image is external input, so this function will validate its data structure
  within this image buffer before use.

  @param[in]    HashAlg   Hash algorithm type.

  @retval TRUE            Successfully hash image.
  @retval FALSE           Fail in hash image.

**/
BOOLEAN
HashPeImage (
  IN UINT32              HashAlg
  )
{
  EFI_STATUS                EfiStatus;
  BOOLEAN                   Status;
  UINT16                    Magic;
  EFI_IMAGE_SECTION_HEADER  *Section;
  UINT8                     *HashBase;
  UINTN                     HashSize;
  UINTN                     SumOfBytesHashed;
  EFI_IMAGE_SECTION_HEADER  *SectionHeader;
  UINTN                     Index;
  UINTN                     Pos;

  const UINT8             *addr[MAX_ELEM_NUM];
  UINTN                    num_elem, len[MAX_ELEM_NUM];
  EFI_GUID                *EfiHashAlgorithmGuid;

  UINT32                    CertSize;
  UINT32                    NumberOfRvaAndSizes;

  SectionHeader = NULL;
  Status        = FALSE;
  EfiStatus     = EFI_SECURITY_VIOLATION;
  num_elem      = 0;
  //
  // Initialize context of hash.
  //
  ZeroMem (mImageDigest, MAX_DIGEST_SIZE);
  switch(HashAlg) {
      case HASHALG_SHA1:
          mImageDigestSize  = SHA1_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha1Guid;
          CopyMem (&mCertType, &gEfiCertSha1Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA256:
          mImageDigestSize  = SHA256_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha256Guid;
          CopyMem (&mCertType, &gEfiCertSha256Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA384:
          mImageDigestSize  = SHA384_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha384Guid;
          CopyMem (&mCertType, &gEfiCertSha384Guid, sizeof (EFI_GUID));
          break;
      case HASHALG_SHA512:
          mImageDigestSize  = SHA512_DIGEST_SIZE;
          EfiHashAlgorithmGuid = &gEfiHashAlgorithmSha512Guid;
          CopyMem (&mCertType, &gEfiCertSha512Guid, sizeof (EFI_GUID));
          break;
      default:
        return FALSE;
  }
  mHashAlg = HashAlg;
  // 1.  Load the image header into memory.

  //
  // Measuring PE/COFF Image Header;
  // But CheckSum field and SECURITY data directory (certificate) are excluded
  //
  if (mNtHeader.Pe32->FileHeader.Machine == EFI_IMAGE_MACHINE_IA64 && mNtHeader.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // NOTE: Some versions of Linux ELILO for Itanium have an incorrect magic value
    //       in the PE/COFF Header. If the MachineType is Itanium(IA64) and the
    //       Magic value in the OptionalHeader is EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC
    //       then override the magic value to EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC
    //
    Magic = EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
  } else {
    //
    // Get the magic value from the PE/COFF Optional Header
    //
    Magic =  mNtHeader.Pe32->OptionalHeader.Magic;
  }

  //
  // 3.  Calculate the distance from the base of the image header to the image checksum address.
  // 4.  Hash the image header from its base to beginning of the image checksum.
  //
  HashBase = mImageBase;
  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // Use PE32 offset.
    //
    HashSize = (UINTN) (&mNtHeader.Pe32->OptionalHeader.CheckSum) - (UINTN) HashBase;
    NumberOfRvaAndSizes = mNtHeader.Pe32->OptionalHeader.NumberOfRvaAndSizes;
  } else if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
    //
    // Use PE32+ offset.
    //
    HashSize = (UINTN) (&mNtHeader.Pe32Plus->OptionalHeader.CheckSum) - (UINTN) HashBase;
    NumberOfRvaAndSizes = mNtHeader.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
  } else {
    //
    // Invalid header magic number.
    //
//    DEBUG ((SecureBoot_DEBUG_LEVEL,"Invalid header magic number.\n"));
    goto Done;
  }

    if (HashSize != 0) 
    {
           addr[num_elem] = HashBase;
           len[num_elem++] =  HashSize;
    } else {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"Hash1.%x\n", HashSize));
        goto Done;}
  //
  // 5.  Skip over the image checksum (it occupies a single ULONG).
  //
  if (NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_SECURITY) {
    //
    // 6.  Since there is no Cert Directory in optional header, hash everything
    //     from the end of the checksum to the end of image header.
    //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset.
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32->OptionalHeader.CheckSum + sizeof (UINT32);
      HashSize = mNtHeader.Pe32->OptionalHeader.SizeOfHeaders - ((UINTN) HashBase - (UINTN) mImageBase);
    } else {
      //
      // Use PE32+ offset.
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32Plus->OptionalHeader.CheckSum + sizeof (UINT32);
      HashSize = mNtHeader.Pe32Plus->OptionalHeader.SizeOfHeaders - ((UINTN) HashBase - (UINTN) mImageBase);
    }

    if (HashSize != 0) 
    {
           addr[num_elem] = HashBase;
           len[num_elem++] =  HashSize;
    } else {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"Hash2.%x\n", HashSize));
        goto Done;}
  } else {
    //
    // 7.  Hash everything from the end of the checksum to the start of the Cert Directory.
    //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset.
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32->OptionalHeader.CheckSum + sizeof (UINT32);
      HashSize = (UINTN) (&mNtHeader.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY]) - (UINTN) HashBase;
    } else {
      //
      // Use PE32+ offset.
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32Plus->OptionalHeader.CheckSum + sizeof (UINT32);
      HashSize = (UINTN) (&mNtHeader.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY]) - (UINTN) HashBase;
    }
    if (HashSize != 0) 
    {
           addr[num_elem] = HashBase;
           len[num_elem++] =  HashSize;
    } else {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"Hash3.%x\n", HashSize));
        goto Done;}

    //
    // 8.  Skip over the Cert Directory. (It is sizeof(IMAGE_DATA_DIRECTORY) bytes.)
    // 9.  Hash everything from the end of the Cert Directory to the end of image header.
    //
    if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
      //
      // Use PE32 offset
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1];
      HashSize = mNtHeader.Pe32->OptionalHeader.SizeOfHeaders - ((UINTN) HashBase - (UINTN) mImageBase);
    } else {
      //
      // Use PE32+ offset.
      //
      HashBase = (UINT8 *) &mNtHeader.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1];
      HashSize = mNtHeader.Pe32Plus->OptionalHeader.SizeOfHeaders - ((UINTN) HashBase - (UINTN) mImageBase);
    }
    if (HashSize != 0) 
    {
           addr[num_elem] = HashBase;
           len[num_elem++] =  HashSize;
    } else {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"Hash4.%x\n", HashSize));
        goto Done;}
  }

  //
  // 10. Set the SUM_OF_BYTES_HASHED to the size of the header.
  //
  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // Use PE32 offset.
    //
    SumOfBytesHashed = mNtHeader.Pe32->OptionalHeader.SizeOfHeaders;
  } else {
    //
    // Use PE32+ offset
    //
    SumOfBytesHashed = mNtHeader.Pe32Plus->OptionalHeader.SizeOfHeaders;
  }


  Section = (EFI_IMAGE_SECTION_HEADER *) (
               mImageBase +
               mPeCoffHeaderOffset +
               sizeof (UINT32) +
               sizeof (EFI_IMAGE_FILE_HEADER) +
               mNtHeader.Pe32->FileHeader.SizeOfOptionalHeader
               );
  //
  // 11. Build a temporary table of pointers to all the IMAGE_SECTION_HEADER
  //     structures in the image. The 'NumberOfSections' field of the image
  //     header indicates how big the table should be. Do not include any
  //     IMAGE_SECTION_HEADERs in the table whose 'SizeOfRawData' field is zero.
  //
// DEBUG ((SecureBoot_DEBUG_LEVEL,"Num Sections = %x\n",  mNtHeader.Pe32->FileHeader.NumberOfSections));

// Security review [305408]: HashPeImage does not validate NumberOfSections field
  Pos = sizeof (EFI_IMAGE_SECTION_HEADER) * mNtHeader.Pe32->FileHeader.NumberOfSections;
  if(Pos > mImageSize)
    goto Done;

  SectionHeader = (EFI_IMAGE_SECTION_HEADER *) AllocateZeroPool (Pos);
  if (SectionHeader == NULL) {
  //DEBUG ((SecureBoot_DEBUG_LEVEL,"Hash4.\n"));
    Status = FALSE;
    goto Done;
  }
  //
  // 12.  Using the 'PointerToRawData' in the referenced section headers as
  //      a key, arrange the elements in the table in ascending order. In other
  //      words, sort the section headers according to the disk-file offset of
  //      the section.
  //
  for (Index = 0; Index < mNtHeader.Pe32->FileHeader.NumberOfSections; Index++) {
    Pos = Index;

    // Security review EIP[104046]:6. HashPeImage does not validate NumberOfSections field
    if(((UINT64)(UINTN)Section + sizeof (EFI_IMAGE_SECTION_HEADER))> ((UINT64)(UINTN)mImageBase+mImageSize) )
        goto Done;

    while ((Pos > 0) && (Section->PointerToRawData < SectionHeader[Pos - 1].PointerToRawData)) {
      CopyMem (&SectionHeader[Pos], &SectionHeader[Pos - 1], sizeof (EFI_IMAGE_SECTION_HEADER));
      Pos--;
    }
    CopyMem (&SectionHeader[Pos], Section, sizeof (EFI_IMAGE_SECTION_HEADER));
    Section += 1;
  }

  //
  // 13.  Walk through the sorted table, bring the corresponding section
  //      into memory, and hash the entire section (using the 'SizeOfRawData'
  //      field in the section header to determine the amount of data to hash).
  // 14.  Add the section's 'SizeOfRawData' to SUM_OF_BYTES_HASHED .
  // 15.  Repeat steps 13 and 14 for all the sections in the sorted table.
  //
  for (Index = 0; Index < mNtHeader.Pe32->FileHeader.NumberOfSections; Index++) {
    Section = &SectionHeader[Index];
    if (Section->SizeOfRawData == 0) {
      continue;
    }

//DEBUG ((SecureBoot_DEBUG_LEVEL,"Section->PointerToRawData = %x\nSection->SizeOfRawData = %x\nSumOfBytesHashed = %x\n",Section->PointerToRawData, Section->SizeOfRawData, SumOfBytesHashed+Section->SizeOfRawData));
    // Security review EIP[104046]: 5.HashPeImage does not validate PointerToRawData and SizeOfRawData fields
    if( ((UINT64)Section->PointerToRawData + Section->SizeOfRawData) > mImageSize )
        goto Done;

    HashBase  = mImageBase + Section->PointerToRawData;
    HashSize  = (UINTN) Section->SizeOfRawData;

    addr[num_elem] = HashBase;
    len[num_elem++] =  HashSize;
    if(num_elem >= MAX_ELEM_NUM)
//        goto Done;
         {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"MAX_ELEM_NUM.1 = %d\n", num_elem));
        goto Done;}
    // Security review EIP[125931]: HashPeImage does not validate certificate table offset and size
    if (((UINT64)SumOfBytesHashed+HashSize) > mImageSize) 
        goto Done;

    SumOfBytesHashed += HashSize;
  }

  //
  // 16.  If the file size is greater than SUM_OF_BYTES_HASHED, there is extra
  //      data in the file that needs to be added to the hash. This data begins
  //      at file offset SUM_OF_BYTES_HASHED and its length is:
  //             FileSize  -  (CertDirectory->Size)
  //
//DEBUG ((SecureBoot_DEBUG_LEVEL,"mImageSize > SumOfBytesHashed.\n%d > %d\n", mImageSize, SumOfBytesHashed));
  if (mImageSize > SumOfBytesHashed) {

    HashBase = mImageBase + SumOfBytesHashed;

    if (NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY_SECURITY) {
      CertSize = 0;
    } else {
      if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        //
        // Use PE32 offset.
        //
        CertSize = mNtHeader.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY].Size;
      } else {
        //
        // Use PE32+ offset.
        //
        CertSize = mNtHeader.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY].Size;
      }
    }

    if (mImageSize > ((UINT64)CertSize + SumOfBytesHashed)) {
        HashSize = (UINTN) (mImageSize - CertSize - SumOfBytesHashed);

//DEBUG ((SecureBoot_DEBUG_LEVEL,"\nData beyond nSumOfBytesHashed\nBase = %d, Size = %d\n",SumOfBytesHashed, HashSize));

        addr[num_elem] = HashBase;
        len[num_elem++] =  HashSize;
        if(num_elem >= MAX_ELEM_NUM)
    //         goto Done;
        {
    //DEBUG ((SecureBoot_DEBUG_LEVEL,"MAX_ELEM_NUM.2=%d\n", num_elem));
            goto Done;
        }
    } else if (mImageSize < ((UINT64)CertSize + SumOfBytesHashed)) {
//DEBUG ((SecureBoot_DEBUG_LEVEL,"mImageSize < CertSize + SumOfBytesHashed.\n%d < %d\n", mImageSize, CertSize + SumOfBytesHashed));
        goto Done;
    }
 // ???     HashBase += CertSize;
//DEBUG ((SecureBoot_DEBUG_LEVEL,"\nData beyond CertDir\nBase = %d, Size = %d\n",SumOfBytesHashed+HashSize+CertSize, mImageSize-(SumOfBytesHashed+HashSize+CertSize)));
  }
  EfiStatus = mDigitalSigProtocol->Hash(
              mDigitalSigProtocol, 
              EfiHashAlgorithmGuid, num_elem, addr, len, (UINT8*)&mImageDigest); 

DEBUG ((SecureBoot_DEBUG_LEVEL,"SHA%d, Hashed Elements %d, Hash: %02X,%02X,..\n", (HashAlg==HASHALG_SHA512)?5:HashAlg+1, num_elem, mImageDigest[0], mImageDigest[1]));

  if(!EFI_ERROR(EfiStatus))
    Status = TRUE;

Done:  

  if (SectionHeader != NULL) {
    FreePool (SectionHeader);
  }
  // Security review EIP[104046]: 4.HashPeImage returns TRUE even in error case
  // Status = FALSE unless updated to TRUE in the final Hash step before "Done" label
  return Status;
}

/**
  Recognize the Hash algorithm in PE/COFF Authenticode and calculate hash of
  Pe/Coff image based on the authenticode image hashing in PE/COFF Specification
  8.0 Appendix A

  Caution: This function may receive untrusted input.
  PE/COFF image is external input, so this function will validate its data structure
  within this image buffer before use.

  @param[in]  AuthData            Pointer to the Authenticode Signature retrieved from signed image.
  @param[in]  AuthDataSize        Size of the Authenticode Signature in bytes.
  
  @retval EFI_UNSUPPORTED             Hash algorithm is not supported.
  @retval EFI_SUCCESS                 Hash successfully.

**/
EFI_STATUS
HashPeImageByType (
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
  Status = mDigitalSigProtocol->Pkcs7Verify (
         mDigitalSigProtocol,
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
    case HASHALG_SHA1:
          if (!HashPeImage (HASHALG_SHA1)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    case HASHALG_SHA256:
          if (!HashPeImage (HASHALG_SHA256)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    case HASHALG_SHA384:
          if (!HashPeImage (HASHALG_SHA384)) {
              Status = EFI_SECURITY_VIOLATION;
          }
        break;
    case HASHALG_SHA512:
          if (!HashPeImage (HASHALG_SHA512)) {
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
  Returns the size of a given image execution info table in bytes.

  This function returns the size, in bytes, of the image execution info table specified by
  ImageExeInfoTable. If ImageExeInfoTable is NULL, then 0 is returned.

  @param  ImageExeInfoTable          A pointer to a image execution info table structure.
  
  @retval 0       If ImageExeInfoTable is NULL.
  @retval Others  The size of a image execution info table in bytes.

**/
UINTN
GetImageExeInfoTableSize (
  EFI_IMAGE_EXECUTION_INFO_TABLE        *ImageExeInfoTable
  )
{
  UINTN                     Index;
  EFI_IMAGE_EXECUTION_INFO  *ImageExeInfoItem;
  UINTN                     TotalSize;

  if (ImageExeInfoTable == NULL) {
    return 0;
  }

  ImageExeInfoItem  = (EFI_IMAGE_EXECUTION_INFO *) ((UINT8 *) ImageExeInfoTable + sizeof (EFI_IMAGE_EXECUTION_INFO_TABLE));
  TotalSize         = sizeof (EFI_IMAGE_EXECUTION_INFO_TABLE);
  for (Index = 0; Index < ImageExeInfoTable->NumberOfImages; Index++) {
    TotalSize += ReadUnaligned32 ((UINT32 *) &ImageExeInfoItem->InfoSize);
    ImageExeInfoItem = (EFI_IMAGE_EXECUTION_INFO *) ((UINT8 *) ImageExeInfoItem + ReadUnaligned32 ((UINT32 *) &ImageExeInfoItem->InfoSize));
  }

  return TotalSize;
}

/**
  Create signature list based on input signature data and certificate type GUID.
  Caller is responsible to free newly created SignatureList.

  @param[in]   SignatureData           Signature data in SignatureList.
  @param[in]   SignatureDataSize       Signature data size.
  @param[in]   CertType                Certificate Type.
  @param[out]  SignatureList           Created SignatureList.
  @param[out]  SignatureListSize       Created SignatureListSize.

  @return EFI_OUT_OF_RESOURCES         The operation is failed due to lack of resources.
  @retval EFI_SUCCESS          Successfully create signature list.

**/
EFI_STATUS
CreateSignatureList(
  IN UINT8                *SignatureData,
  IN UINTN                SignatureDataSize,
  IN EFI_GUID             *CertType,
  OUT EFI_SIGNATURE_LIST  **SignatureList,
  OUT UINTN               *SignatureListSize
  )
{
  EFI_SIGNATURE_LIST   *SignList;
  UINTN                SignListSize;
  EFI_SIGNATURE_DATA   *Signature;

  *SignatureList = NULL;

  SignListSize = sizeof (EFI_SIGNATURE_LIST) + sizeof (EFI_SIGNATURE_DATA) - 1 + SignatureDataSize;
  SignList     = (EFI_SIGNATURE_LIST *) AllocateZeroPool (SignListSize);
  if (SignList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SignList->SignatureHeaderSize = 0;
  SignList->SignatureListSize   = (UINT32) SignListSize;
  SignList->SignatureSize       = (UINT32) SignatureDataSize + sizeof (EFI_SIGNATURE_DATA) - 1;
  CopyMem (&SignList->SignatureType, CertType, sizeof (EFI_GUID));

  Signature = (EFI_SIGNATURE_DATA *) ((UINT8 *) SignList + sizeof (EFI_SIGNATURE_LIST));
  CopyMem (Signature->SignatureData, SignatureData, SignatureDataSize);

  *SignatureList     = SignList;
  *SignatureListSize = SignListSize;

  return EFI_SUCCESS;

}
/**
  Create an Image Execution Information Table entry and add it to system configuration table.

  @param[in]  Action          Describes the action taken by the firmware regarding this image.
  @param[in]  Name            Input a null-terminated, user-friendly name.
  @param[in]  DevicePath      Input device path pointer.
  @param[in]  Signature       Input signature info in EFI_SIGNATURE_LIST data structure.
  @param[in]  SignatureSize   Size of signature.

**/
VOID
AddImageExeInfo (
  IN       EFI_IMAGE_EXECUTION_ACTION       Action,
  IN       CHAR16                           *Name OPTIONAL,
  IN CONST EFI_DEVICE_PATH_PROTOCOL         *DevicePath,
  IN       EFI_SIGNATURE_LIST               *Signature OPTIONAL,
  IN       UINTN                            SignatureSize
  )
{
  EFI_IMAGE_EXECUTION_INFO_TABLE  *ImageExeInfoTable;
  EFI_IMAGE_EXECUTION_INFO_TABLE  *NewImageExeInfoTable;
  EFI_IMAGE_EXECUTION_INFO        *ImageExeInfoEntry;
  UINTN                           ImageExeInfoTableSize;
  UINTN                           NewImageExeInfoEntrySize;
  UINTN                           NameStringLen;
  UINTN                           DevicePathSize;
  CHAR16                          *NameStr;

  EFI_STATUS                      Status;
  UINTN                           Index;
  EFI_SIGNATURE_LIST              *SignatureOrg;
  
  ImageExeInfoTable     = NULL;
  NewImageExeInfoTable  = NULL;
  ImageExeInfoEntry     = NULL;
  NameStringLen         = 0;
  NameStr               = NULL;

  if (DevicePath == NULL) {
    return ;
  }

  if (Name != NULL) {
    NameStringLen = StrSize (Name);
  } else {
    NameStringLen = sizeof (CHAR16);
  }

  DevicePathSize = DPLength((EFI_DEVICE_PATH_PROTOCOL*)DevicePath);

//  EfiGetSystemConfigurationTable (&gEfiImageSecurityDatabaseGuid, (VOID **) &ImageExeInfoTable);
  ImageExeInfoTable = GetEfiConfigurationTable(pST, &gEfiImageSecurityDatabaseGuid);
  if (ImageExeInfoTable != NULL) {
    //
    // The table has been found!
    // We must enlarge the table to accommodate the new exe info entry.
    //
    ImageExeInfoTableSize = GetImageExeInfoTableSize (ImageExeInfoTable);
//    DEBUG ((SecureBoot_DEBUG_LEVEL,"ImageExeTbl size = %X\n", ImageExeInfoTableSize ));
    
// loop through existing tbl for all entries to match the Name, exit if match is found
    ImageExeInfoEntry = (EFI_IMAGE_EXECUTION_INFO *) ((UINT8 *) ImageExeInfoTable + sizeof (EFI_IMAGE_EXECUTION_INFO_TABLE));
    for (Index = 0; Index < ImageExeInfoTable->NumberOfImages; Index++) {
//        DEBUG ((SecureBoot_DEBUG_LEVEL,"Image = %d-%d: CurTblInfo size = %X\n", Index, ImageExeInfoTable->NumberOfImages, ImageExeInfoEntry->InfoSize, ImageExeInfoTableSize ));
//        DEBUG ((SecureBoot_DEBUG_LEVEL,"NewDevicePathSize = %X, NewNameStringLen = %X\nCur Image Name %s\nNew Image Name %s\n", 
//                DevicePathSize, NameStringLen, (CHAR16*)((UINT8 *)&ImageExeInfoEntry->InfoSize + sizeof (UINT32)), Name));
        if((Name != NULL) && (!MemCmp((UINT8 *) &ImageExeInfoEntry->InfoSize + sizeof (UINT32), Name, NameStringLen))) {
          if(Signature != NULL) {
              NameStr = (CHAR16 *)(ImageExeInfoEntry + 1);
              SignatureOrg = (EFI_SIGNATURE_LIST*)((UINT8 *) NameStr + NameStringLen + DevicePathSize);
//              DEBUG ((SecureBoot_DEBUG_LEVEL,"Cur Sig Type %g, Cur Sig Size 0x%04X\nNew Sig Type %g, New Sig Size 0x%04X\n", 
//                      &SignatureOrg->SignatureType, SignatureOrg->SignatureSize, &Signature->SignatureType, Signature->SignatureSize));
              if(!guidcmp(&SignatureOrg->SignatureType, &Signature->SignatureType) &&
                 (SignatureOrg->SignatureSize == Signature->SignatureSize) &&
                   (!MemCmp((UINT8 *)((UINT8 *) SignatureOrg + sizeof (EFI_SIGNATURE_LIST)), (UINT8 *) Signature + sizeof (EFI_SIGNATURE_LIST), Signature->SignatureSize))
              ) { // Cert types do match
                  return ;
              }
          }
        }
        ImageExeInfoEntry = (EFI_IMAGE_EXECUTION_INFO *) ((UINT8 *) ImageExeInfoEntry + ImageExeInfoEntry->InfoSize);
    }
  } else {
    //
    // Not Found!
    // We should create a new table to append to the configuration table.
    //
    ImageExeInfoTableSize = sizeof (EFI_IMAGE_EXECUTION_INFO_TABLE);
  }

//  DevicePathSize            = GetDevicePathSize (DevicePath);

  //
  // Signature size can be odd. Pad after signature to ensure next EXECUTION_INFO entry align
  //
  NewImageExeInfoEntrySize = sizeof (EFI_IMAGE_EXECUTION_INFO) + NameStringLen + DevicePathSize + SignatureSize;

  NewImageExeInfoTable      = (EFI_IMAGE_EXECUTION_INFO_TABLE *) AllocateRuntimePool (ImageExeInfoTableSize + NewImageExeInfoEntrySize);
  if (NewImageExeInfoTable == NULL) {
    return ;
  }

  if (ImageExeInfoTable != NULL) {
    CopyMem (NewImageExeInfoTable, ImageExeInfoTable, ImageExeInfoTableSize);
  } else {
    NewImageExeInfoTable->NumberOfImages = 0;
  }
  NewImageExeInfoTable->NumberOfImages++;
  ImageExeInfoEntry = (EFI_IMAGE_EXECUTION_INFO *) ((UINT8 *) NewImageExeInfoTable + ImageExeInfoTableSize);
  //
  // Update new item's information.
  //
  WriteUnaligned32 ((UINT32 *) ImageExeInfoEntry, Action);
  WriteUnaligned32 ((UINT32 *) ((UINT8 *) ImageExeInfoEntry + sizeof (EFI_IMAGE_EXECUTION_ACTION)), (UINT32) NewImageExeInfoEntrySize);

  NameStr = (CHAR16 *)(ImageExeInfoEntry + 1);
  if (Name != NULL) {
    CopyMem ((UINT8 *) NameStr, Name, NameStringLen);
  } else {
    ZeroMem ((UINT8 *) NameStr, sizeof (CHAR16));
  }

  CopyMem (
    (UINT8 *) NameStr + NameStringLen,
    (void*)DevicePath,
    DevicePathSize
    );
  if (Signature != NULL) {
    CopyMem (
      (UINT8 *) NameStr + NameStringLen + DevicePathSize,
      Signature,
      SignatureSize
      );
  }
  //
  // Update/replace the image execution table.
  //
  Status = pBS->InstallConfigurationTable (&gEfiImageSecurityDatabaseGuid, (VOID *) NewImageExeInfoTable);
  ASSERT_EFI_ERROR (Status);
  if (Signature != NULL) {
      DEBUG ((SecureBoot_DEBUG_LEVEL,"New Record Added to Image Execution Table %r\n(Addr=%X, Size=0x%X, Type %g, Action=%d)\n", Status, NewImageExeInfoTable,ImageExeInfoTableSize + NewImageExeInfoEntrySize,&Signature->SignatureType, Action));
  }

  //
  // Free Old table data!
  //
  if (ImageExeInfoTable != NULL) {
    FreePool (ImageExeInfoTable);
  }
}

/**
  Record multiple certificate list & verification state of a verified image to 
  IMAGE_EXECUTION_TABLE.

  @param[in]  CertBuf              Certificate list buffer.
  @param[in]  CertBufLength        Certificate list buffer.
  @param[in]  Action               Certificate list action to be record.
  @param[in]  ImageName            Image name.
  @param[in]  ImageDevicePath      Image device path.

**/
VOID 
RecordCertListToImageExecutionTable(
  IN UINT8                          *CertBuf,
  IN UINTN                           CertBufLength,
  IN EFI_IMAGE_EXECUTION_ACTION      Action,
  IN CHAR16                         *ImageName OPTIONAL,
  IN CONST EFI_DEVICE_PATH_PROTOCOL *ImageDevicePath OPTIONAL
  )
{
  UINT8               CertNumber;
  UINT8               *CertPtr;
  UINTN               Index;
  UINT8               *Cert;
  UINTN               CertSize;
  EFI_STATUS          Status;
  EFI_SIGNATURE_LIST  *SignatureList;
  UINTN               SignatureListSize;
  //      UINT8  CertNumber;
  //      UINT32 Cert1Length;
  //      UINTN  Cert1Ptr;
  //      UINT32 Cert2Length;
  //      UINTN  Cert2Ptr;
  //      ...
  //      UINT32 CertnLength;
  //      UINTN  CertnPtr;
  //
  CertNumber = (UINT8) (*CertBuf);
  CertPtr    = CertBuf + 1;
  for (Index = 0; Index < CertNumber && CertPtr < (CertBuf + CertBufLength); Index++) {
    CertSize = (UINTN) ReadUnaligned32 ((UINT32 *)CertPtr);
    Cert     = (UINT8 *)CertPtr + sizeof (UINT32);
    //
    // Record all cert in cert chain to be passed
    //
    Status = CreateSignatureList((UINT8*)*(UINTN*)Cert, CertSize, &gEfiCertX509Guid, &SignatureList, &SignatureListSize);
    if (!EFI_ERROR(Status)) {
      AddImageExeInfo (Action, ImageName, ImageDevicePath, SignatureList, SignatureListSize);
      FreePool (SignatureList);
    }
    CertPtr = Cert + sizeof(UINTN);
  }
}

#if (defined(SECUREBOOT_AUDIT_MODE_CUSTOMIZATION) && SECUREBOOT_AUDIT_MODE_CUSTOMIZATION == 1)
/**
  Append a list of signer-chained certificates to a Signature Database(db) in AuditMode.
  Helps to debug Efi applications signed with un-registered key certificates

  @param[in]  CertBuf              Certificate list buffer.
  @param[in]  CertBufLength        Buffer length.

**/
VOID 
RecordCertListToDb(
  IN UINT8                          *CertBuf,
  IN UINTN                           CertBufLength
  )
{
  UINT8               CertNumber;
  UINT8               *CertPtr;
  UINTN               Index;
  UINT8               *Cert;
  UINTN               CertSize;
  EFI_STATUS          Status;
  EFI_SIGNATURE_LIST  *SignatureList;
  UINTN               SignatureListSize;
  UINT32              Attributes;
  UINTN     size;
  UINT8    *Data;

  Attributes = (EFI_VARIABLE_RUNTIME_ACCESS | 
                EFI_VARIABLE_BOOTSERVICE_ACCESS |
                EFI_VARIABLE_NON_VOLATILE | 
                EFI_VARIABLE_APPEND_WRITE |
                EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS);

  CertNumber = (UINT8) (*CertBuf);
  CertPtr    = CertBuf + 1;
  for (Index = 0; Index < CertNumber && CertPtr < (CertBuf + CertBufLength); Index++) {
    CertSize = (UINTN) ReadUnaligned32 ((UINT32 *)CertPtr);
    Cert     = (UINT8 *)CertPtr + sizeof (UINT32);
    //
    // Record all cert in cert chain to be passed
    //
    Status = CreateSignatureList((UINT8*)*(UINTN*)Cert, CertSize, &gEfiCertX509Guid, &SignatureList, &SignatureListSize);
    if (!EFI_ERROR(Status)) {
        size = SignatureListSize+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2);
        Data = AllocatePool(size);
        Status = EFI_OUT_OF_RESOURCES;
        if(Data != NULL) {
            AmiFillAuthenticatedHdr2(Data, SET_SECURE_VARIABLE_SET | EFI_VARIABLE_APPEND_WRITE);
            MemCpy ((VOID*)((UINTN)Data+sizeof(AMI_EFI_VARIABLE_AUTHENTICATION_2)), SignatureList, SignatureListSize);
//            Status = AmiSetSecureBootVariable((SET_SECURE_VARIABLE_SET | SET_SECURE_VARIABLE_APPEND), EFI_IMAGE_SECURITY_DATABASE, NULL, &gEfiImageSecurityDatabaseGuid, Data, size );
            Status = pRS->SetVariable(EFI_IMAGE_SECURITY_DATABASE, &gEfiImageSecurityDatabaseGuid, Attributes, size, Data);
            FreePool (Data);
        }
        FreePool (SignatureList);
    }
    CertPtr = Cert + sizeof(UINTN);
  }
}
#endif
/**
  Create a Boot Image Certificate table entry and add it to system configuration table.

  @param[in]  Name            Input a null-terminated, user-friendly name.
  @param[in]  DevicePath      Input device path pointer.
  @param[in]  Signature       Input signature info in EFI_SIGNATURE_LIST data structure.
  @param[in]  SignatureSize   Size of signature.
  
**/
VOID
AddBootImageCertInfo (
  IN       CHAR16                           *Name OPTIONAL, 
  IN       UINT32                           SignatureOffs,
  IN       UINT32                           SignatureSize
  )
{
    EFI_STATUS                 Status;
    AMI_VALID_CERT_IN_SIG_DB  *BootImageCertInfoTable;
    UINTN                      BootImageCertInfoTableSize;

    if(SignatureOffs == 0 || SignatureSize == 0)
    return;
    
    BootImageCertInfoTable = GetEfiConfigurationTable(pST, &gBootImageCertTblGuid);
    if (BootImageCertInfoTable == NULL) {
        //
        // Not Found!
        // We should create a new table.
        //
        BootImageCertInfoTableSize = sizeof (AMI_VALID_CERT_IN_SIG_DB);
        BootImageCertInfoTable = AllocatePool(BootImageCertInfoTableSize);
        if(BootImageCertInfoTable == NULL) return;
        //
        // Update/replace the image execution table.
        //
        Status = pBS->InstallConfigurationTable (&gBootImageCertTblGuid, (VOID *) BootImageCertInfoTable);
        ASSERT_EFI_ERROR (Status);
        DEBUG ((SecureBoot_DEBUG_LEVEL,"Install BootImageCert Table ...%r\n", Status));
        if (EFI_ERROR (Status)) return;
    }
    //
    // Update Table's infomation.
    //
    BootImageCertInfoTable->SigOffset = SignatureOffs;
    BootImageCertInfoTable->SigLength = SignatureSize;

DEBUG ((SecureBoot_DEBUG_LEVEL,"BootImage Cert in db\nOffset=%X\nLength=%X\n", BootImageCertInfoTable->SigOffset, BootImageCertInfoTable->SigLength));
}

/**
  Discover if the UEFI image is authorized by user's policy setting.

  @param[in]    Policy            Specify platform's policy setting. 
  @param[in]    Action            Image Validation status. 

  @retval EFI_ACCESS_DENIED       Image is not allowed to run.
  @retval EFI_SECURITY_VIOLATION  Image is deferred.
  @retval EFI_SUCCESS             Image is authorized to run.

**/
EFI_STATUS
ImageAuthorization (
  IN UINT32     Policy,
  IN UINT32     Action
  )
{
    EFI_STATUS    Status;
#if (defined(ENABLE_IMAGE_EXEC_POLICY_OVERRIDE) && ENABLE_IMAGE_EXEC_POLICY_OVERRIDE == 1)
    EFI_INPUT_KEY Key = {0,0};
    UINTN         Index;
    BOOLEAN       bQuery;
#endif

    Status = EFI_ACCESS_DENIED;

    switch (Policy) {

#if (defined(ENABLE_IMAGE_EXEC_POLICY_OVERRIDE) && ENABLE_IMAGE_EXEC_POLICY_OVERRIDE == 1)
    case QUERY_USER_ON_SECURITY_VIOLATION:
        // Conditions to allow overwrite of Execution Policy:
        // 1. Output Console available
        if((pST->ConIn != NULL && pST->ConOut != NULL))
        {
            pST->ConOut->OutputString(pST->ConOut, mNotifyString1);
            bQuery = TRUE;
            switch(Action) {
                case EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND: // not found in db
                    pST->ConOut->OutputString(pST->ConOut, mNotifyString3);
                    break;
                case EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND:    // found in dbx
                    pST->ConOut->OutputString(pST->ConOut, mNotifyString4);
                    break;
                case EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED:
                case EFI_IMAGE_EXECUTION_AUTH_UNTESTED:
                    pST->ConOut->OutputString(pST->ConOut, mNotifyString5);
                    break;
                default:
                    bQuery = FALSE;
                    break;
            }
            if(!bQuery) break;
            //Wait for key
            pST->ConOut->OutputString(pST->ConOut, mNotifyString2);
Repeat:
            pBS->WaitForEvent( 1, &pST->ConIn->WaitForKey, &Index );
            Status = pST->ConIn->ReadKeyStroke( pST->ConIn, &Key );
            if (!EFI_ERROR (Status)) {
              if ((Key.UnicodeChar == L'y') || (Key.UnicodeChar == L'Y')) { //yes
                Status = EFI_SUCCESS;
              } else if ((Key.UnicodeChar == L'n') || (Key.UnicodeChar == L'N')) { //no
                Status = EFI_ACCESS_DENIED;
              } else {
                goto Repeat;
              }
            }
            pST->ConOut->OutputString(pST->ConOut, L"\r\n");
        } 
        break;
#endif
    case DEFER_EXECUTE_ON_SECURITY_VIOLATION:
        Status = EFI_SECURITY_VIOLATION;
        break;

    //  case DENY_EXECUTE_ON_SECURITY_VIOLATION:
    default:
        Status = EFI_ACCESS_DENIED;
        break;
    }

DEBUG ((SecureBoot_DEBUG_LEVEL,"Image Authorization policy...%r\n", Status));

    return Status;
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
IsSignatureFoundInDatabase (
  IN CHAR16          *VariableName,
  EFI_GUID           *SignatureType,
  IN UINT8           *Signature, 
  IN UINTN            SignatureSize
  )
{
  EFI_SIGNATURE_LIST  *CertList;
  EFI_SIGNATURE_DATA  *Cert;
  VOID               *Data;
  UINTN               DataSize;
  UINTN               Index;
  UINTN               CertCount;
  BOOLEAN             IsSigFound;
  EFI_STATUS          Status;
  //
  // Read signature database variable.
  ///
  IsSigFound   = FALSE;

// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
  //
  // Get Signature Database variable.
  //
  Status = GetEfiImageSecurityDatabaseVariableEx (VariableName, (VOID**)&Data, &DataSize);
  // APTIOV_OVERRIDE_END
//  DEBUG((SecureBoot_DEBUG_LEVEL,"Var %X(sz %X)\n", Data, DataSize));
  //
  // Failure to read from "dbx" is considered a failure to validate Image
  //
  if (EFI_ERROR(Status) && Status != EFI_NOT_FOUND &&
      StrCmp(VariableName, EFI_IMAGE_SECURITY_DATABASE1) == 0) {
      return TRUE;
  }
  // not an error if no "dbx" defined
  if (Data != NULL) {
      //
      // Enumerate all signature data in SigDB to check if signature exists for executable.
      //
      CertList = (EFI_SIGNATURE_LIST *) Data;
      while (!IsSigFound && (DataSize > 0) && (DataSize >= CertList->SignatureListSize)) {
//DEBUG((SecureBoot_DEBUG_LEVEL," SigType %g == %g\n\n", CertList->SignatureType, SignatureType));
        if (!guidcmp(&CertList->SignatureType, SignatureType)) { // Cert types do match 
            CertCount = (CertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - CertList->SignatureHeaderSize) / CertList->SignatureSize;
            Cert      = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList + sizeof (EFI_SIGNATURE_LIST) + CertList->SignatureHeaderSize);
//DEBUG((SecureBoot_DEBUG_LEVEL," SigSize %X == %X(raw %X)\n", CertList->SignatureSize, (sizeof(EFI_SIGNATURE_DATA) - 1 + SignatureSize) , SignatureSize));
            if (CertList->SignatureSize == (sizeof(EFI_SIGNATURE_DATA) - 1 + SignatureSize)) {
              for (Index = 0; Index < CertCount; Index++) {
//DEBUG((SecureBoot_DEBUG_LEVEL," SigData %X.. == %X.., SigSize %X\n", *(UINT32*)Cert->SignatureData, *(UINT32*)Signature, SignatureSize));
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
//DEBUG((SecureBoot_DEBUG_LEVEL," SigFound %d\n", IsSigFound));
  return IsSigFound;
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
IsPkcsSignedDataVerifiedBySignatureList (
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
    VOID                      *Data;
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
    IsVerified            = FALSE;
    bVerifyTimeStampStatus= FALSE;
    bProcessVerifyTimeStampStatus = FALSE;

    mTimeOfSigningLong    = 0;
    TimeOfRevocationLong = 0;

// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
    // Verify the certificate's header
    //
    // Find certificate in store and verify authenticode struct.
    //
    Status = GetEfiImageSecurityDatabaseVariableEx (VariableName, (VOID**)&Data, &DataSize);
    //
    // Failure to read from "dbx" is considered a failure to validate Image
    //
    // APTIOV_OVERRIDE_END
    if (EFI_ERROR(Status) && Status != EFI_NOT_FOUND &&
        StrCmp(VariableName, EFI_IMAGE_SECURITY_DATABASE1) == 0) {
        return TRUE;
    }
    if (!Data) 
        return FALSE;

    CertList = (EFI_SIGNATURE_LIST *)Data; 

    //
    // TO DO Optimization. 
    //  1. Find Root CA Cert Ptr or Signer Cert if Root is not found
    //  2. loop thru Trust Cert list and compare each one against Root CA cert(verify SignCert signature with Trust Cert)
    //  3. If found, break the 'while' loop and Pkcs7Verify with Trust Cert
    //  
    //
    // Find Cert Enrolled in database to verify the signature in pkcs7 signed data.
    // 
    // here should be a loop thru Cert list till CertCount
    while ((DataSize > 0) && (DataSize >= CertList->SignatureListSize)) {

        ValidCertType = CertUndefined;

        if(!guidcmp(&CertList->SignatureType, &gEfiCertX509Guid))
            ValidCertType = CertX509;
        else
            if(!guidcmp(&CertList->SignatureType, &mCertType) &&
                mImageDigestSize  >= SHA256_DIGEST_SIZE ) // one of SHA256,SHA384 or SHA512
                ValidCertType = CertSha256;
            else
                if(!guidcmp(&CertList->SignatureType, &gEfiCertX509Sha256Guid))
                    ValidCertType = CertX509Sha256;
                else
                    if(!guidcmp(&CertList->SignatureType, &gEfiCertX509Sha384Guid))
                        ValidCertType = CertX509Sha384;
                    else
                        if(!guidcmp(&CertList->SignatureType, &gEfiCertX509Sha512Guid))
                            ValidCertType = CertX509Sha512;

        if (ValidCertType != CertUndefined) {
            Cert       = (EFI_SIGNATURE_DATA *) ((UINT8 *) CertList + sizeof (EFI_SIGNATURE_LIST) + CertList->SignatureHeaderSize);
            CertCount  = (CertList->SignatureListSize - sizeof (EFI_SIGNATURE_LIST) - CertList->SignatureHeaderSize) / CertList->SignatureSize;
            RootCertSize  = CertList->SignatureSize-sizeof(EFI_GUID); // sig data structure starts with SigOwner Guid
DEBUG ((SecureBoot_DEBUG_LEVEL,"Cert Type %g (enum type %d)\nCertSize = 0x%X\n", &CertList->SignatureType, ValidCertType, RootCertSize));

            for (Index = 0; Index < CertCount; Index++) {
              //
              // Iterate each Signature Data Node within this CertList for verify.
              //
                RootCert = Cert->SignatureData;
                switch(ValidCertType) {
// is Sha256/Sha384/Sha512
                case CertSha256:
                    if(RootCertSize == mImageDigestSize && 
                        MemCmp(RootCert, mImageDigest, mImageDigestSize) == 0) {
                      //
                      // Found the signature in database.
                      //
                        IsVerified = TRUE;
                        goto Exit;
                    }
DEBUG ((SecureBoot_DEBUG_LEVEL,"Got SHAxxx Cert(%d): match=%d, size=%d, [%04X],..)\n", mHashAlg, IsVerified, mImageDigestSize, *(UINT32*)RootCert));
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
DEBUG ((SecureBoot_DEBUG_LEVEL,"Got X509_SHAxxx Cert ..."));
                case CertX509:
DEBUG ((SecureBoot_DEBUG_LEVEL,"X.509, Proceed to Pkcs7Verify...\n"));
                    //
                    //
                    // Verify Authenticode struct for image's current certificate.
                    //
                    // using protocol
                    SigCert       = mImageDigest;
                    SigCertSize   = mImageDigestSize;
                    Status = mDigitalSigProtocol->Pkcs7Verify (
                         mDigitalSigProtocol,
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
DEBUG ((SecureBoot_DEBUG_LEVEL,"====> %r\n", Status));
                    if (!EFI_ERROR(Status)) {
                        IsVerified = TRUE; // found matching certificate in the image
                        if(Operation == Pkcs7CertValidate) {
                         // only x509 certs from db are measured in PCR[7]
                             mTrustSigDbOffs = (UINT32)((UINTN)Cert-(UINTN)Data);
                             mTrustSigDbSize = CertList->SignatureSize;
                             break;
                        }
                        if(Operation == Pkcs7TimeStampCertValidateGet) {
                            mTimeOfSigningLong = (os_time_t)SigCertSize;
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
                                 bVerifyTimeStampStatus = IsPkcsSignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE2, Pkcs7TimeStampCertValidateGet); 
                                 bProcessVerifyTimeStampStatus = TRUE;
                            }
                            DEBUG ((SecureBoot_DEBUG_LEVEL,"\nTimeStamp cert validate %a\nTimeOfSigning %X\nTimeOfRevocation %X\n",  
                                                 (!bVerifyTimeStampStatus?"FAIL":"PASS"), mTimeOfSigningLong, TimeOfRevocationLong));
                             // revoke Image if cert match in dbx but failed dbt!
                            if(!bVerifyTimeStampStatus) {
                                //IsVerified = TRUE;  
                                break;
                            }
                             // keep looking for cert with sign time passed revocation
                            if(mTimeOfSigningLong< TimeOfRevocationLong) 
                                IsVerified = FALSE;
                        }
                    }
                    break;
                default :
DEBUG ((SecureBoot_DEBUG_LEVEL,"Unknown cert type...\n"));
                       goto Exit;
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
DEBUG ((SecureBoot_DEBUG_LEVEL,"\n<===%a cert match in '%s'\n",(IsVerified?"Got":"No"), VariableName));
    return IsVerified;
}

/**
        Handle for Security Architectural Protocol
        Provide verification service for signed images, which include both signature validation
        and platform policy control. For signature types, both UEFI WIN_CERTIFICATE_UEFI_GUID and
        MSFT Authenticode type signatures are supported.

        In this implementation, only verify external executables when in USER MODE.
        Executables from FV is bypass, so pass in AuthenticationStatus is ignored.

        The image verification policy is:
          If the image is signed,
            At least one valid signature or at least one hash value of the image must match a record
            in the security database "db", and no valid signature nor any hash value of the image may
            be reflected in the security database "dbx".
          Otherwise, the image is not signed,
            The SHA256 hash value of the image must match a record in the security database "db", and
            not be reflected in the security data base "dbx".

        Caution: This function may receive untrusted input.
          PE/COFF image is external input, so this function will validate its data structure
          within this image buffer before use.

  @param[in]    AuthenticationStatus 
                           The authentication status returned from the security
                           measurement services for the input file.
  @param[in]    File       The pointer to the device path of the file that is
                           being dispatched. This will optionally be used for logging.
  @param[in]    FileBuffer The file buffer matches the input file device path.
  @param[in]    FileSize   The size of File buffer matches the input file device path.
  @param[in]    BootPolicy A boot policy that was used to call LoadImage() UEFI service.

  @return       EFI_STATUS

      EFI_SUCCESS            The file specified by File did authenticate, and the
                             platform policy dictates that the DXE Core may use File.
      EFI_INVALID_PARAMETER  File is NULL.
      EFI_SECURITY_VIOLATION The file specified by File did not authenticate, and
                             the platform policy dictates that File should be placed
                             in the untrusted state. A file may be promoted from
                             the untrusted to the trusted state at a future time
                             with a call to the Trust() DXE Service.
      EFI_ACCESS_DENIED      The file specified by File did not authenticate, and
                             the platform policy dictates that File should not be
                             used for any purpose.

**/
EFI_STATUS
EFIAPI
DxeImageVerificationHandler (
  IN  UINT32                           AuthenticationStatus,
  IN  CONST EFI_DEVICE_PATH_PROTOCOL   *File,
  IN  VOID                             *FileBuffer,
  IN  UINTN                            FileSize,
  IN BOOLEAN                           BootPolicy
  )
{
  EFI_STATUS                           Status;
  UINT16                               Magic;
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  EFI_STATUS                           VerifyStatus;
  EFI_SIGNATURE_LIST                   *SignatureList;
  UINTN                                SignatureListSize;
  EFI_IMAGE_EXECUTION_ACTION           Action;
  WIN_CERTIFICATE                      *WinCertificate;
  UINT32                               Policy;
  UINT8                                *VarData;
  UINTN                                DataSize;

  PE_COFF_LOADER_IMAGE_CONTEXT         ImageContext;
  UINT32                               NumberOfRvaAndSizes;
  WIN_CERTIFICATE_EFI_PKCS             *PkcsCertData;
  WIN_CERTIFICATE_UEFI_GUID            *WinCertUefiGuid;
  UINT8                                *AuthData;
  UINTN                                 AuthDataSize;
  EFI_IMAGE_DATA_DIRECTORY             *SecDataDir;
  UINT32                                OffSet;
  CHAR16                               *FilePathStr;
  UINT8                                *SignerCert;
  UINTN                                 SignerCertLength;
  UINT8                                *UnchainCert;
  UINTN                                 UnchainCertLength;
  UINT32                                HashAlg;

  SignatureList     = NULL;
  SignatureListSize = 0;
  WinCertificate    = NULL;
  SecDataDir        = NULL;
  PkcsCertData      = NULL;
  FilePathStr       = NULL;
  Action            = EFI_IMAGE_EXECUTION_AUTH_UNTESTED;
  VerifyStatus      = EFI_ACCESS_DENIED;
  Status            = EFI_ACCESS_DENIED;

  mImageDigestSize  = 0;
  mHashAlg          = HASHALG_MAX;
  //
  // Check the image type and get policy setting.
  //
  Policy = AmiGetImageType (File, FileBuffer, FileSize, BootPolicy);
  DEBUG ((SecureBoot_DEBUG_LEVEL,"DxeImageVerificationLib: Image type: %x\n", Policy));
  switch(Policy) {

  case IMAGE_FROM_FV:
    Policy = ALWAYS_EXECUTE;
    break;

  case IMAGE_FROM_OPTION_ROM:
    Policy = mSecureBootSetup.Load_from_OROM;
    break;

  case IMAGE_FROM_REMOVABLE_MEDIA:
    Policy = mSecureBootSetup.Load_from_REMOVABLE_MEDIA;
    break;

  case IMAGE_FROM_FIXED_MEDIA:
    Policy = mSecureBootSetup.Load_from_FIXED_MEDIA;
    break;

  default:
    DEBUG ((SecureBoot_DEBUG_LEVEL,"Unknown Image Path\n"));
    Policy = DENY_EXECUTE_ON_SECURITY_VIOLATION;
    break;
  }

  if (Policy == ALWAYS_EXECUTE) {
    return EFI_SUCCESS;
  }

// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
  VarData = GetEfiGlobalVariableEx (EFI_AUDIT_MODE_NAME, &DataSize);
  if(VarData) {
      mAuditMode = *VarData;
      FreePool(VarData);
  } else {
      // Assume AuditMode is Disabled on failure to get the variable
      mAuditMode = AUDIT_MODE_DISABLE;
  }
// APTIOV_OVERRIDE_END
  //
  // SecureBoot & AuditMode variables are installed by NVRAM driver
  //
  // Skip verification if SecureBoot is disabled and not in AuditMode
  //
  if (mSecureBootEnable == SECURE_BOOT_MODE_DISABLE && 
      mAuditMode == AUDIT_MODE_DISABLE)
     return  EFI_SUCCESS;

  //
  // Get Image Device Path Str
  //
  FilePathStr = ConvertDevicePathToText (File, FALSE, TRUE);
  DEBUG ((SecureBoot_DEBUG_LEVEL,"DxeImageVerificationLib: Image Path: '%s'\nPolicy %d, DigitalSigProtocol %x\n", FilePathStr, Policy, mDigitalSigProtocol));
  //
  // Authentication failed because of (unspecified) firmware security policy or failure to locate mDigitalSigProtocol
  //
  if (Policy == NEVER_EXECUTE || 
      mDigitalSigProtocol == NULL
      ) {
    //
    // No signature, record FilePath/FilePathStr only
    //
        AddImageExeInfo (EFI_IMAGE_EXECUTION_POLICY_FAILED | EFI_IMAGE_EXECUTION_INITIALIZED, FilePathStr, File, NULL, 0);
        return EFI_ACCESS_DENIED;
  }
  //
  // The policy QUERY_USER_ON_SECURITY_VIOLATION and ALLOW_EXECUTE_ON_SECURITY_VIOLATION
  // violates the UEFI spec and has been removed.
  //
#if !defined(ENABLE_IMAGE_EXEC_POLICY_OVERRIDE) || !ENABLE_IMAGE_EXEC_POLICY_OVERRIDE 
  ASSERT (Policy != QUERY_USER_ON_SECURITY_VIOLATION && Policy != ALLOW_EXECUTE_ON_SECURITY_VIOLATION);
  if (Policy == QUERY_USER_ON_SECURITY_VIOLATION || Policy == ALLOW_EXECUTE_ON_SECURITY_VIOLATION) {
    CpuDeadLoop ();
  }
#endif
  //
  // Read the Dos header.
  //
  if (FileBuffer == NULL) {
// APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
    return EFI_ACCESS_DENIED;
// APTIOV_OVERRIDE_END
  }

//  PERF_START(NULL, "FileAuthenticate", NULL,0);

  mImageBase  = (UINT8 *) FileBuffer;
  mImageSize  = FileSize;

  ZeroMem (&ImageContext, sizeof (ImageContext));
  ImageContext.Handle    = (VOID *) FileBuffer;
  ImageContext.ImageRead = (PE_COFF_LOADER_READ_FILE) DxeImageVerificationLibImageRead;

  //
  // Get information about the image being loaded
  //
  if (EFI_ERROR (PeCoffLoaderGetImageInfo (&ImageContext)) ||
      ( mImageSize< sizeof(EFI_IMAGE_DOS_HEADER)) ) {
    //
    // The information can't be gotten from the invalid PeImage
    //
    DEBUG ((SecureBoot_DEBUG_LEVEL, "PeImage invalid. Cannot retrieve image information.\n"));
    goto Done;
  }

  DosHdr = (EFI_IMAGE_DOS_HEADER *) mImageBase;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    //
    // DOS image header is present,
    // so read the PE header after the DOS image header.
    //
    mPeCoffHeaderOffset = DosHdr->e_lfanew;
  } else {
    mPeCoffHeaderOffset = 0;
  }
  // Security review EIP[104046]: 3.HashPeImage does not validate minimum image size
  // Validate mPeCoffHeaderOffset is within the Image size range
  if(((UINT64)mPeCoffHeaderOffset+sizeof(EFI_IMAGE_NT_HEADERS32)) > mImageSize)  {
    DEBUG ((SecureBoot_DEBUG_LEVEL, "PE/COFF image header error.\n"));
    goto Done;
  }

  //
  // Check PE/COFF image.
  //
  mNtHeader.Pe32 = (EFI_IMAGE_NT_HEADERS32 *) (mImageBase + mPeCoffHeaderOffset);
  if (mNtHeader.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    //
    // It is not a valid Pe/Coff file.
    //
    DEBUG ((SecureBoot_DEBUG_LEVEL, "Not a valid PE/COFF image.\n"));
    goto Done;
  }

  if (mNtHeader.Pe32->FileHeader.Machine == EFI_IMAGE_MACHINE_IA64 && mNtHeader.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // NOTE: Some versions of Linux ELILO for Itanium have an incorrect magic value 
    //       in the PE/COFF Header. If the MachineType is Itanium(IA64) and the 
    //       Magic value in the OptionalHeader is EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC
    //       then override the magic value to EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC
    //
    Magic = EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
  } else {
    //
    // Get the magic value from the PE/COFF Optional Header
    //
    Magic = mNtHeader.Pe32->OptionalHeader.Magic;
  }
  
  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // Use PE32 offset.
    //
    NumberOfRvaAndSizes = mNtHeader.Pe32->OptionalHeader.NumberOfRvaAndSizes;
    if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_SECURITY) {
      SecDataDir = (EFI_IMAGE_DATA_DIRECTORY *) &mNtHeader.Pe32->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY];
    }        
  } else {
    //
    // Use PE32+ offset.
    //
    NumberOfRvaAndSizes = mNtHeader.Pe32Plus->OptionalHeader.NumberOfRvaAndSizes;
    if (NumberOfRvaAndSizes > EFI_IMAGE_DIRECTORY_ENTRY_SECURITY) {
      SecDataDir = (EFI_IMAGE_DATA_DIRECTORY *) &mNtHeader.Pe32Plus->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY];
    }
  }

  DEBUG ((SecureBoot_DEBUG_LEVEL,"mImageSize %X\n", mImageSize));

  //
  // Start Image Validation.
  //
  if (SecDataDir && SecDataDir->Size) {
  //  
  // Image is possibly signed
  //
    DEBUG ((SecureBoot_DEBUG_LEVEL,"SecDataDir->VirtualAddress %x\nSecDataDir->Size = %X\nIMAGE is Signed\n",
      SecDataDir->VirtualAddress, SecDataDir->Size));
  // Security review EIP[104046]: 2.Multiple integer underflows calculating size of certificate data in PE file
    if(((UINT64)SecDataDir->VirtualAddress+SecDataDir->Size) > mImageSize)
        goto Done;

  //
  // Verify the signature of the image, multiple signatures are allowed as per PE/COFF Section 4.7 
  // "Attribute Certificate Table".
  // The first certificate starts at offset (SecDataDir->VirtualAddress) from the start of the file.
  //
    Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED;
    // APTIOV_OVERRIDE_START : Address Security Advisory SA50088 
    Status = EFI_SUCCESS;
    // APTIOV_OVERRIDE_END
    for (OffSet = SecDataDir->VirtualAddress;
         // APTIOV_OVERRIDE_RC_START : Address Security Advisory SA50088 
         !EFI_ERROR(Status) &&
         (OffSet >= SecDataDir->VirtualAddress) && (OffSet < (SecDataDir->VirtualAddress + SecDataDir->Size));
         Status = SafeUint32Add (OffSet, (WinCertificate->dwLength + ALIGN_SIZE (WinCertificate->dwLength)), &OffSet)
         ) {
         // APTIOV_OVERRIDE_END
        WinCertificate = (WIN_CERTIFICATE *) (mImageBase + OffSet);
        DEBUG ((SecureBoot_DEBUG_LEVEL,"\nOffSet = %X, WinCertificate->dwLength %X\n\n", OffSet, WinCertificate->dwLength));
        if ((SecDataDir->VirtualAddress + SecDataDir->Size - OffSet) <= sizeof (WIN_CERTIFICATE) ||
            (SecDataDir->VirtualAddress + SecDataDir->Size - OffSet) < WinCertificate->dwLength) {
          break;
        }
        //
        // Verify the image's Authenticode signature, only DER-encoded PKCS#7 signed data is supported.
        //
        //
        // Verify signature of executables.
        //

        if (WinCertificate->wCertificateType == WIN_CERT_TYPE_PKCS_SIGNED_DATA) {
          //
          // The certificate is formatted as WIN_CERTIFICATE_EFI_PKCS which is described in the 
          // Authenticode specification.
          //
          PkcsCertData = (WIN_CERTIFICATE_EFI_PKCS *) WinCertificate;
          if (PkcsCertData->Hdr.dwLength <= sizeof (PkcsCertData->Hdr)) {
            break;
          }
          AuthData   = PkcsCertData->CertData;
          AuthDataSize = PkcsCertData->Hdr.dwLength - sizeof(PkcsCertData->Hdr);

        } else if (WinCertificate->wCertificateType == WIN_CERT_TYPE_EFI_GUID) {
          //
          // The certificate is formatted as WIN_CERTIFICATE_UEFI_GUID which is described in UEFI Spec.
          //
          WinCertUefiGuid = (WIN_CERTIFICATE_UEFI_GUID *) WinCertificate;
          if (WinCertUefiGuid->Hdr.dwLength <= OFFSET_OF(WIN_CERTIFICATE_UEFI_GUID, CertData)) {
            break;
          }
          if (guidcmp (&WinCertUefiGuid->CertType, &gEfiCertPkcs7Guid)) {
            continue;
          }
          AuthData = WinCertUefiGuid->CertData;
          AuthDataSize = WinCertUefiGuid->Hdr.dwLength - OFFSET_OF(WIN_CERTIFICATE_UEFI_GUID, CertData);

        } else {
          if (WinCertificate->dwLength < sizeof (WIN_CERTIFICATE)) {
            break;
          }
          continue;
        }
        //
        // Verify Pkcs signed data type.
        //
        // At least 1 Cert from Pe Image should be in DB
        // None of Certs should be found in DBX 
        //
        // get Digest Algorithm, set mCertType
        // lock mutex, preserve Pkcs7 context
        Status = HashPeImageByType (AuthData, AuthDataSize);
        if (EFI_ERROR (Status)) {
            continue;
        }

        //
        // Check the digital signature against the revoked certificate in forbidden database (dbx).
        //
        if (IsPkcsSignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE1, Pkcs7CertGetMatchInCertChain)) {

            Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND;
            DEBUG ((SecureBoot_DEBUG_LEVEL, "Image is signed but its certificate is revoked by %s.\n",EFI_IMAGE_SECURITY_DATABASE1));
            VerifyStatus = EFI_ACCESS_DENIED;
            if (mAuditMode == AUDIT_MODE_DISABLE)
                break;
        } else {
            //
            // Check the digital signature against the valid certificate in allowed database (db).
            //
            if (EFI_ERROR (VerifyStatus)) {
                if(IsPkcsSignedDataVerifiedBySignatureList (AuthData, AuthDataSize, EFI_IMAGE_SECURITY_DATABASE, Pkcs7CertValidate)) {
                    VerifyStatus = EFI_SUCCESS;
                } else {
                  DEBUG ((SecureBoot_DEBUG_LEVEL, "Image is signed but its certificate not cleared by %s.\n",EFI_IMAGE_SECURITY_DATABASE));
                }
            }
        }
        //
        // Is mAuditMode = Record all chained and unchained certificates lists to be failed
        //
        if (mAuditMode == AUDIT_MODE_ENABLE) {
            UnchainCert       = NULL;
            UnchainCertLength = 0;
            Status = mDigitalSigProtocol->Pkcs7Verify (
                    mDigitalSigProtocol,
                    AuthData, AuthDataSize,
                    NULL, 0,
                    &UnchainCert, &UnchainCertLength,
                    Pkcs7GetUnChainedCertificates,
                    KEEP                  // Flags 
                );
            DEBUG ((SecureBoot_DEBUG_LEVEL,"AuditMode: Un-chained cert list %r, size %x\n", Status, UnchainCertLength));
            if ((UnchainCertLength != 0) && (UnchainCert != NULL)) {
                RecordCertListToImageExecutionTable(
                    UnchainCert,
                    UnchainCertLength,
                    EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED | EFI_IMAGE_EXECUTION_INITIALIZED,
                    FilePathStr,
                    File
                );
                FreePool (UnchainCert);
            }
            SignerCert        = NULL;
            SignerCertLength  = 0;
            Status = mDigitalSigProtocol->Pkcs7Verify (
                 mDigitalSigProtocol,
                 AuthData, AuthDataSize,
                 NULL, 0,
                 &SignerCert, &SignerCertLength,
                 Pkcs7GetChainedCertificates,
                 KEEP                  // Flags 
                 );
            DEBUG ((SecureBoot_DEBUG_LEVEL,"AuditMode: Chain cert list %r, size %x\n", Status, SignerCertLength));
            if ((SignerCertLength != 0) && (SignerCert != NULL)) {
                if (!EFI_ERROR (VerifyStatus)) {
                    RecordCertListToImageExecutionTable(
                        SignerCert,
                        SignerCertLength,
                        EFI_IMAGE_EXECUTION_AUTH_SIG_PASSED | EFI_IMAGE_EXECUTION_INITIALIZED,
                        FilePathStr,
                        File
                    );
                } else {
                    RecordCertListToImageExecutionTable(
                        SignerCert,
                        SignerCertLength,
                        EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED | EFI_IMAGE_EXECUTION_INITIALIZED,
                        FilePathStr,
                        File
                    );
#if (defined(SECUREBOOT_AUDIT_MODE_CUSTOMIZATION) && SECUREBOOT_AUDIT_MODE_CUSTOMIZATION == 1)
                    // Append a list of signer-chained certificates to a db.
                    // Helps to debug Efi applications signed with un-registered cert key
                    // 
                    if(mSecureBootSetup.SecureBootMode == SECURE_BOOT_MODE_CUSTOM)
                        RecordCertListToDb(SignerCert, SignerCertLength);
#endif
                }
                FreePool (SignerCert);
            }
        }
        // clear context
        Status = mDigitalSigProtocol->Pkcs7Verify (
             mDigitalSigProtocol,
             AuthData, AuthDataSize,
             NULL, 0,
             NULL, NULL,
             Pkcs7Arg0,             // Dummy op
             RESET                  // Flags 
             );

    } // end multi-sig for

//    if (OffSet != (SecDataDir->VirtualAddress + SecDataDir->Size + ALIGN_SIZE(SecDataDir->Size))) {
    if(Action != EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND) {
        if (OffSet != (SecDataDir->VirtualAddress + SecDataDir->Size)) {
            DEBUG ((SecureBoot_DEBUG_LEVEL,"Offset outside of CertDir boundary %X != %x\n", OffSet, SecDataDir->VirtualAddress + SecDataDir->Size));
            //
            // The Size in Certificate Table or the attribute certificate table is corrupted.
            //
            VerifyStatus = EFI_ACCESS_DENIED;
        } else 
            Action = EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND;
    }

  } // end Signed cert branch

  // Image not Signed or 
  // Image is Signed, its Cert verified in db but not found in DBX
  if( Action != EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND || mAuditMode == AUDIT_MODE_ENABLE ) {
    //
    // This image is not signed or signed but found only in db or failed previous checks.
    // Expected:
    // The SHA256 hash value of the image must match a record in the security database "db",
    // and not be reflected in the security data base "dbx".
    //
    DEBUG ((SecureBoot_DEBUG_LEVEL,"IMAGE is either not signed or its X509 Certificate not found in DBX\nCheck image's PE Hash certificate in DB and DBX...\n"));
    // loop through all supported digest algorithms
    for (HashAlg = HASHALG_SHA256; HashAlg < HASHALG_MAX; HashAlg++) {

        if(mHashAlg != HashAlg) {        // skip if mImageDigest matching HashAlg already calculated
            if(!HashPeImage (HashAlg))   // init mCertType and calculate PE hash if not done already
              goto Done;
        }

        if(IsSignatureFoundInDatabase (EFI_IMAGE_SECURITY_DATABASE1, &mCertType, mImageDigest, mImageDigestSize)) {
            //
            // Image Hash in forbidden database (DBX)
            //
            DEBUG ((SecureBoot_DEBUG_LEVEL, "Image's hash%d is revoked by %s.\n",HashAlg+1,EFI_IMAGE_SECURITY_DATABASE1));
            VerifyStatus = EFI_ACCESS_DENIED;
            Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND;
            if (mAuditMode == AUDIT_MODE_DISABLE)
                break;
        } else {
            // no - try in allowed db if not there already
            if(EFI_ERROR (VerifyStatus) && 
                IsSignatureFoundInDatabase (EFI_IMAGE_SECURITY_DATABASE, &mCertType, mImageDigest, mImageDigestSize)) {
                //
                // Image Hash is in allowed database (DB).
                //
                DEBUG ((SecureBoot_DEBUG_LEVEL, "Image's hash%d is allowed by %s.\n",HashAlg+1, EFI_IMAGE_SECURITY_DATABASE));
                VerifyStatus = EFI_SUCCESS;
                Action = EFI_IMAGE_EXECUTION_AUTH_SIG_PASSED;
            } else 
                Action = EFI_IMAGE_EXECUTION_AUTH_SIG_FAILED;
        }
        //
        // Add HASH digest for image with signature
        //
        // Is mAuditMode = Record all chained and unchained certificates lists to be failed
        //
        if(mAuditMode == AUDIT_MODE_ENABLE) {
            Status = CreateSignatureList(mImageDigest, mImageDigestSize, &mCertType, &SignatureList, &SignatureListSize);
            if (!EFI_ERROR(Status)) {
                Action = Action | EFI_IMAGE_EXECUTION_INITIALIZED;
                AddImageExeInfo (Action, FilePathStr, File, SignatureList, SignatureListSize);
                FreePool (SignatureList);
            }
        }
    }
  }
  //
  // Signature database check after verification.
  //
  if(EFI_ERROR (VerifyStatus)) {
    //
    // Executable signature not found in authorized signature database.
    //
    if((Action & EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND) == EFI_IMAGE_EXECUTION_AUTH_SIG_FOUND) {
    //
    // Executable signature is found in forbidden signature database.
    //
        DEBUG ((SecureBoot_DEBUG_LEVEL,"Certificate IS FOUND in forbidden database!\n"));
    } else
    //
    // Verification failure.
    //
    if((Action & EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND) == EFI_IMAGE_EXECUTION_AUTH_SIG_NOT_FOUND) {
    //
    // Executable signature cannot be found in authorized signature database.
    // Use platform policy to determine the action.
    //
      DEBUG ((SecureBoot_DEBUG_LEVEL,"Certificate NOT FOUND in authorized database!\n"));
    } else 
      DEBUG ((SecureBoot_DEBUG_LEVEL,"Image is Forbidden!\n"));

    if(mImageDigestSize) {
        Status = CreateSignatureList(mImageDigest, mImageDigestSize, &mCertType, &SignatureList, &SignatureListSize);
        if (!EFI_ERROR(Status)) {
            AddImageExeInfo (Action, FilePathStr, File, SignatureList, SignatureListSize);
            FreePool (SignatureList);
        }
    }
  }
  
Done:

  Status = VerifyStatus;

  if (!EFI_ERROR(Status)) {
  // Install Tbl only on the Boot Loader launch
//    if ( BootPolicy)
    AddBootImageCertInfo(NULL, mTrustSigDbOffs, mTrustSigDbSize);
    mTrustSigDbOffs = 0;
    mTrustSigDbSize = 0;
  } else {
    // Treat unsecured images according to Image Authorization policy
    Status = ImageAuthorization (Policy, Action);
  }

  if (FilePathStr != NULL) {
    FreePool(FilePathStr);
  }

DEBUG ((SecureBoot_DEBUG_LEVEL,"Image Verification ...%r\nAuditMode %d\n", Status, mAuditMode));
  if (mAuditMode == AUDIT_MODE_ENABLE)
      Status = EFI_SUCCESS;

//PERF_END(NULL, "FileAuthenticate", NULL,0);

  return Status;
}

/**
  On BdsConnectDriversProtocol Event notification handler.

  This function initialize mDigitalSigProtocol ptr

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context

**/
VOID
EFIAPI
InitDigitalSignatureProtocolCallback (
    IN EFI_EVENT Event, 
    IN VOID *Context
  ){
    UINTN      DataSize;
    UINT8      *VarData;
    //
    // Look up for image authorization policy in "SetupData" variable
    //
#if (defined(ENABLE_IMAGE_EXEC_POLICY_OVERRIDE) && ENABLE_IMAGE_EXEC_POLICY_OVERRIDE == 1)
    DataSize = sizeof(mSecureBootSetup);
    if(EFI_ERROR(pRS->GetVariable (AMI_SECURE_BOOT_SETUP_VAR,&gSecureSetupGuid,NULL,&DataSize,&mSecureBootSetup)) ||
       // Standard boot mode policy->apply defaults
       mSecureBootSetup.SecureBootMode == SECURE_BOOT_MODE_STANDARD || 
       // Policy check against fixed settings
       mSecureBootSetup.Load_from_OROM < LOAD_FROM_OROM ||
       mSecureBootSetup.Load_from_REMOVABLE_MEDIA < LOAD_FROM_REMOVABLE_MEDIA ||
       mSecureBootSetup.Load_from_FIXED_MEDIA < LOAD_FROM_FIXED_MEDIA
    )
#endif
    { 
        // NVRAM service getting installed from CoreDxe image which is being validated by this policy
        // until we have NVRAM, use hardwired policy
        mSecureBootSetup.Load_from_OROM = LOAD_FROM_OROM;
        mSecureBootSetup.Load_from_REMOVABLE_MEDIA = LOAD_FROM_REMOVABLE_MEDIA;
        mSecureBootSetup.Load_from_FIXED_MEDIA = LOAD_FROM_FIXED_MEDIA;
    }
    //
    // SecureBoot variable is installed at NVRAM driver init and not to be updated until re-start
    //
    VarData = GetEfiGlobalVariableEx (EFI_SECURE_BOOT_NAME, &DataSize);
    if(VarData) {
        mSecureBootEnable = *VarData;
        FreePool(VarData);
    } else {
    //
    // Assume SecureBoot is Enabled on failure to get the variable
    //
        mSecureBootEnable = SECURE_BOOT_MODE_ENABLE;
    }

    pBS->LocateProtocol( &gAmiDigitalSignatureProtocolGuid, NULL, (void**)&mDigitalSigProtocol );

    RegisterSecurity2Handler ( DxeImageVerificationHandler,
            EFI_AUTH_OPERATION_VERIFY_IMAGE | EFI_AUTH_OPERATION_IMAGE_REQUIRED
            );

    return;
}
/**
  Register security measurement handler.

  @param  ImageHandle   ImageHandle of the loaded driver.
  @param  SystemTable   Pointer to the EFI System Table.

  @retval EFI_SUCCESS   The handlers were registered successfully.
**/
EFI_STATUS
EFIAPI
DxeImageVerificationLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_EVENT  Event;
    VOID       *p;
    EFI_GUID   BdsConnectDriversProtocolGuid = AMI_BDS_CONNECT_DRIVERS_PROTOCOL_GUID;

    InitAmiLib(ImageHandle, SystemTable);

    // Enable Security verification at beginning of BDS connect controller phase,
    // We assume all drivers before the event were launched from internal FVs
    return RegisterProtocolCallback(
        &BdsConnectDriversProtocolGuid,
        InitDigitalSignatureProtocolCallback,
        NULL,
        &Event,
        &p
    );
//    return RegisterSecurity2Handler (
//            DxeImageVerificationHandler,
//            EFI_AUTH_OPERATION_VERIFY_IMAGE | EFI_AUTH_OPERATION_IMAGE_REQUIRED
//            );
}

#pragma warning (default : 4090)
