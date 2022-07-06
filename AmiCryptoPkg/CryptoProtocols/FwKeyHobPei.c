//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
 * FwKeyHobPei.c
 */
#include <Token.h>
#include <AmiPeiLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include "Ppi/LoadFile.h"
#include "AmiCertificate.h" // FWKEY_HOB
#include <Library/BaseMemoryLib.h>

//
// Global variables
//

static EFI_GUID *gKeyFileGuid [] = {
    &gPKeyFileSha256Guid,
    &gPKeyFileX509Guid,
    &gPKeyFileRsa2048Guid,
    NULL
};
static EFI_GUID *gKeyTypeGuid [] = {
    &gEfiCertSha256Guid,
    &gEfiCertX509Guid,
    &gEfiCertRsa2048Guid,
    NULL
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
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gAmiInstallCryptoPpiGuid,//gEfiPeiMemoryDiscoveredPpiGuid,
        NotifyAtPeiEnd
    }
};
// Redundant callback if FvVerify service is installed....
static EFI_PEI_NOTIFY_DESCRIPTOR mCryptoNotifyDescs[] = {
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gEfiEndOfPeiSignalPpiGuid,
        NotifyAtPeiEnd
    }
};
#endif
//--------------------------------------------------------------------------------
/**
  Loads a binary from RAW section of FV_BB

  @param[out] Buffer - returns a pointer to allocated memory. Caller must free it when done
  @param[out] Size  - returns the size of the binary loaded into the buffer.
  
  @retval     Status
**/
EFI_STATUS
CryptoGetRawImage (
   IN EFI_GUID *FileGuid, 
   IN OUT VOID **Buffer, 
   IN OUT UINTN *Size)
{
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME_HEADER    *pFV;
  UINTN                         FvNum=0;
  EFI_FFS_FILE_HEADER           *ppFile=NULL;
  static EFI_GUID               pFV_BB_Guid = FWKEY_FV_HEADER_GUID;
  BOOLEAN    bFound = FALSE;

  if (!Buffer || !Size) 
    return EFI_INVALID_PARAMETER;

  while(TRUE)
  {
      Status = PeiServicesFfsFindNextVolume(FvNum++, (VOID**)&pFV);
//      DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "FvNum %d : %g %r\n", FvNum-1, (EFI_GUID*)((UINTN)pFV+pFV->ExtHeaderOffset),Status));
      if(EFI_ERROR(Status)) return Status;
      //
      // FwKey location is in FV_BB only
      //
      if(guidcmp((EFI_GUID*)((UINTN)pFV+pFV->ExtHeaderOffset), &pFV_BB_Guid) != 0)
        continue;

      ppFile = NULL;

      while(TRUE)
      {
          Status = PeiServicesFfsFindNextFile (EFI_FV_FILETYPE_FREEFORM/*EFI_FV_FILETYPE_ALL*/, pFV, (VOID**)&ppFile);
//          DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "%r ppFile type %d = %g\n", Status, ppFile->Type,(EFI_GUID*)&ppFile->Name));
          if(Status == EFI_NOT_FOUND)
          {
              break;
          }
          if(guidcmp(&ppFile->Name, FileGuid)==0) 
          {
//              DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "==>Match!\n"));
              bFound = TRUE; 
              break;
          }
      }
      if(bFound) break;
  }
    // hopefully we found the file...now try to read raw data
    // !!! Keys are uncompressed. There is no much reason to run compression on prime numbers anyway
  Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, ppFile, Buffer);
  if(!EFI_ERROR(Status)) {
    // Size may need to subtract Section hdr size = 28 bytes sizeof(EFI_FFS_FILE_HEADER + EFI_COMMON_SECTION_HEADER)
    *Size = FVFILE_SIZE(ppFile)-sizeof(EFI_FFS_FILE_HEADER)-sizeof(EFI_COMMON_SECTION_HEADER);
  }

  DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "Find Key Ffs %g, %r\n(addr %X, len 0x%x) [%X...]\n", FileGuid, Status, (UINT32)*Buffer, *Size, *((UINT32*)*Buffer)));
  return Status;

}

/**
    Function locates a Platform Signing Key (PK) Ffs inside Recovery FV (FV_BB or similar)
    and installs a FwKey Hob with a ptr to a Key store

  @param[in]   PeiServices Pointer to the PEI services table
  @param[out]  Pointer to pFwKeyHob
  
  @retval     Status
    EFI_SUCCESS               The Key is successfully returned.
    EFI_NOT_FOUND             The Key was found and FwKey Hob created
**/
EFI_STATUS
CryptoInstallFwKeyHob (
    IN CONST EFI_PEI_SERVICES     **PeiServices,
    IN OUT FW_KEY_HOB **pFwKeyHob
)
{
    EFI_STATUS              Status;
    UINTN                   Size;
    UINT8                   Index;
    UINT8                   *pBuf;
//    UINTN                   i;
//    UINT8                   Byte;
//    const UINT8 *FwBadKey = "$BAD";
//    UINT8                 *pRamBuf;

    
    Status = EFI_SUCCESS;

    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "Install pFwKeyHob...\n"));

    // The KeyHob already installed
    if(pFwKeyHob == NULL)
        return EFI_INVALID_PARAMETER;

    if(*pFwKeyHob)
        return EFI_SUCCESS;

////////////////////////////////////////////////////////////////////////////////////
//
// Create FwKey Hob
//
////////////////////////////////////////////////////////////////////////////////////
    Index = 0;
    while(gKeyFileGuid[Index] != NULL)
    {
        // Available Key Cert GUIDs: RSA2048, SHA256 and x509
        Status = CryptoGetRawImage(gKeyFileGuid[Index], (VOID**)&pBuf, (UINTN*)&Size);
        if(!EFI_ERROR(Status)) {
/*
            //  make sure the Key buffer is mapped to FV_BB address space 
            if(!((UINT32)pBuf > FV_BB_BASE && 
                 (UINT64)((UINT32)pBuf+Size) < 
                 (UINT64)(FV_BB_BASE+(UINT64)FV_BB_BLOCKS*FLASH_BLOCK_SIZE)))
                     break;
            Status = (*PeiServices)->AllocatePages(PeiServices, EfiBootServicesCode,EFI_SIZE_TO_PAGES(Size), (EFI_PHYSICAL_ADDRESS*)&pRamBuf);
            if (!EFI_ERROR(Status)) {
                MemCpy((VOID*)pRamBuf, pBuf, Size);
                pBuf = pRamBuf;
            }
            // don't report a Key if dummy FWkey  is found
            if(*((UINT32*)(pBuf)) == *(UINT32*)FwBadKey) {
                DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "FFS with dummy FW Key detected...\n"));
                break;
            }
*/
            Status = (*PeiServices)->CreateHob( PeiServices,
                EFI_HOB_TYPE_GUID_EXTENSION, 
                sizeof(FW_KEY_HOB), 
                (void**)pFwKeyHob);

            if (!EFI_ERROR(Status) && *pFwKeyHob) {
                CopyGuid (&(((FW_KEY_HOB*)*pFwKeyHob)->Header.Name), &gPRKeyGuid);
                CopyGuid (&(((FW_KEY_HOB*)*pFwKeyHob)->KeyGuid), gKeyTypeGuid[Index]);
                ((FW_KEY_HOB*)*pFwKeyHob)->KeyAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)pBuf;
                ((FW_KEY_HOB*)*pFwKeyHob)->KeySize = (UINT32)Size;
            }
            DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "KeyHob %X [%X]...\n", pFwKeyHob, *pFwKeyHob));
            //TBD: install HOBs for all FwKeys that available
            break;
        }
        Index++;
    }

    return Status;
}

/**
  Function locates a Platform Signing Key (PK) Hob

  @param[in]   PeiServices Pointer to the PEI services table
  @param[out]  Pointer to pFwKeyHob
  
  @retval     Status
    EFI_SUCCESS               The Key is successfully returned.
    EFI_NOT_FOUND             The Key was found and FwKey Hob created
**/
/*
EFI_STATUS
CryptoGetFwKeyHob (
    IN CONST EFI_PEI_SERVICES     **PeiServices,
    IN OUT FW_KEY_HOB **pFwKeyHob
)
{
    EFI_STATUS       Status = EFI_NOT_FOUND;
    VOID            *FirstHob;

    (*PeiServices)->GetHobList(PeiServices, &FirstHob);
    if (!FirstHob) ASSERT_EFI_ERROR (EFI_NOT_FOUND);

    while ( !EFI_ERROR(Status = FindNextHobByType(EFI_HOB_TYPE_GUID_EXTENSION, &FirstHob)) ) {
     if ( guidcmp(&((EFI_HOB_GUID_TYPE*)FirstHob)->Name, &gPRKeyGuid) == 0 ) {
         break;
     }
    }
    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "CryptoGetFwKeyHob %X...%r\n", FirstHob, Status));
    if (!EFI_ERROR(Status)) 
        *pFwKeyHob = (FW_KEY_HOB *)FirstHob;

    return Status;
}
//-----------------------------------------------------------------------------------------------------------------------
// Callback on MemInstalled
//-----------------------------------------------------------------------------------------------------------------------
/**
 * The function will be invoked after memory initialized event
 *
 * @param PeiServices Pointer to the Pointer to the Pei Services Table.
 * @param NotifyDescriptor The notification descriptor for this notification event.
 * @param NullPpi Pointer to a NULL ppi.
 *
 * @retval EFI_SUCCESS
 */
/*
EFI_STATUS NotifyAtPeiEnd (
    IN EFI_PEI_SERVICES             **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
    IN VOID                         *Ppi
)
{
    EFI_STATUS              Status;
    FW_KEY_HOB              *pFwKeyHob;
    EFI_BOOT_MODE           BootMode;

    // WARNING. This module must always be launched from RAM after the boot mode type is finalized.
//    Status = (*PeiServices)->GetBootMode( PeiServices, &BootMode );
//    if(EFI_ERROR(Status) || BootMode == BOOT_ON_S3_RESUME) {
//        return EFI_SUCCESS; // skip on S3 resume
//    }
    // Check if the KeyHob already installed
    pFwKeyHob = NULL;
    Status = CryptoGetFwKeyHob(PeiServices, &pFwKeyHob);
    if (EFI_ERROR(Status)) {
        //First time acquire FwKey
        CryptoInstallFwKeyHob(PeiServices, &pFwKeyHob);
    }
    return EFI_SUCCESS;
}
*/
/**
  This function is the entry point for this PEI.
  Function locates a FwKey ffs and creates FwKey Hob,
 
  @param  FileHandle    The firmware allocated handle for the EFI image.
  @param  PeiServices   A pointer to the PEI Services.
  
  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
FwKeyHobPei_Init (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
//    EFI_STATUS  Status;
//    EFI_BOOT_MODE           BootMode;
    FW_KEY_HOB              *pFwKeyHob;

    // WARNING. This module must always be launched from RAM after the boot mode type is finalized.
//    Status = (*PeiServices)->GetBootMode( PeiServices, &BootMode );
//    if(EFI_ERROR(Status) || BootMode == BOOT_ON_S3_RESUME) {
//        return EFI_SUCCESS; // skip on S3 resume
//    }
    pFwKeyHob = NULL;
    CryptoInstallFwKeyHob(PeiServices, &pFwKeyHob);
////////////////////////////////////////////////////////////////////////////////////
//
//  Call back to locate FwKey ffs when Cpu cache is already setup at the End of PEI.
//
////////////////////////////////////////////////////////////////////////////////////
//    (*PeiServices)->NotifyPpi(PeiServices, mInstallCryptoPpiNotify);
//    (*PeiServices)->NotifyPpi(PeiServices, mCryptoNotifyDescs);

    return EFI_SUCCESS;
}
