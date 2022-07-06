//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**        5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093         **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//*************************************************************************
// $Header: /Alaska/SOURCE/Modules/TcgNext/Common/AmiTcgPlatform/AmiTcgPlatformPeiAfterMem.c 1     10/08/13 12:04p Fredericko $
//
// $Revision: 1 $
//
// $Date: 10/08/13 12:04p $
//*************************************************************************
// Revision History
// ----------------
// $Log: /Alaska/SOURCE/Modules/TcgNext/Common/AmiTcgPlatform/AmiTcgPlatformPeiAfterMem.c $
//*************************************************************************
//<AMI_FHDR_START>
//
// Name:	AmiTcgPlatformPeiAfterMem.c
//
// Description:	Function file for AmiTcgPlatformPeiAfterMem
//
//<AMI_FHDR_END>
//*************************************************************************
#include <Efi.h>
#include <Pei.h>
#include <AmiTcg/TcgCommon12.h>
#include <AmiTcg/TCGMisc.h>
#include <Ppi/TcgTcmService.h>
#include <Ppi/TcgService.h>
#include <Ppi/TpmDevice.h>
#include "Ppi/CpuIo.h"
#include "Ppi/LoadFile.h"
#include <Ppi/ReadOnlyVariable.h>
#include <AmiTcg/AmiTcgPlatformPei.h>
#include <Ppi/AmiTcgPlatformPpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
#include <Library/AmiRomLayoutLib.h>
#include <RomLayout.h>
#endif
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeCoffLib.h>
// NIST 800-155
#include <Guid/HobList.h>
#include <AmiHobs.h>
#include <Token.h>
#include <AmiTcg/AmiTpmStatusCodes.h>
#include <Guid/AmiTcgGuidIncludes.h>

#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
#include <Ppi/AmiFvDigest.h>
#endif
// NIST 800-155

//**********************************************************************
//                  Links
//**********************************************************************
extern MEASURE_CRTM_VERSION_PEI_FUNC_PTR  MEASURE_CRTM_VERSION_PEI_FUNCTION;
MEASURE_CRTM_VERSION_PEI_FUNC_PTR *MeasureCRTMVersionFuncPtr = MEASURE_CRTM_VERSION_PEI_FUNCTION;

extern EFI_GUID gTcgPeiPolicyGuid;
extern EFI_GUID gAmiTcgPlatformPpiAfterMem;
extern EFI_GUID PeiMpFileGuid;
extern EFI_GUID gMpTcmFileGuid;
extern TCM_PC_REGISTERS_PTR      TcmReg;

static EFI_PEI_PPI_DESCRIPTOR TcmInitPpi[] = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmiTcmSignalguid,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR LegacyTpmInitPpi[] = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAmiLegacyTpmguid,
  NULL
};

EFI_STATUS FindAndMeasureDxeCore(EFI_PEI_SERVICES **PeiServices);



#if TCG_LEGACY == 1
    EFI_STATUS Configure_Tpm_Chip( );
#endif
    
//NIST-155

// ->>>>>>>>>>>>>>>> NIST 800-155
void printbuffer(UINT8 *Buffer, UINTN BufferSize)
{
    UINTN i=0; UINTN j=0;
                
    for(i=0; i<BufferSize; i++){

        if(i%16 == 0){
            DEBUG((DEBUG_INFO,"\n"));
            DEBUG((DEBUG_INFO,"%04x :", j));
            j+=1;
        }
        DEBUG((DEBUG_INFO,"%02x ", Buffer[i]));
    }
    DEBUG((DEBUG_INFO,"\n"));
    
}


#pragma pack(push,1)

typedef struct{

    UINT64   Base;
    UINTN   Size;
}HASHDATA;
#pragma pack(pop)


EFI_STATUS
TcgPeiSimpleExtend(
   PEI_TCG_PPI   *TcgPpi,  
   EFI_PEI_SERVICES **ps,
   IN TPM_PCRINDEX PCRIndex,
   IN TCG_DIGEST   *Digest,
   OUT TCG_DIGEST  *NewPCRValue )
{
    UINT8   *cmd;
    UINT8   *BufferPtr=NULL;
    UINT32   BufferSize=0;
    EFI_STATUS Status;
    UINT8   *ret;

    BufferSize = sizeof (TPM_1_2_CMD_HEADER) + sizeof (PCRIndex) + TPM_SHA1_160_HASH_LEN;
    Status = (*ps)->AllocatePool((CONST EFI_PEI_SERVICES **)ps, BufferSize, (void **)&cmd);
    if(EFI_ERROR(Status))return Status;
    
    Status = (*ps)->AllocatePool((CONST EFI_PEI_SERVICES **)ps, BufferSize, (void **)&ret);
    if(EFI_ERROR(Status))return Status;
    
    (*ps)->SetMem(cmd, BufferSize, 0);
        
    BufferPtr = cmd;
    ((TPM_1_2_CMD_HEADER *)cmd)->Tag       = TPM_H2NS( TPM_TAG_RQU_COMMAND );
    ((TPM_1_2_CMD_HEADER *)cmd)->ParamSize = TPM_H2NL(BufferSize);
    ((TPM_1_2_CMD_HEADER *)cmd)->Ordinal   = TPM_H2NL( TPM_ORD_Extend );
    
    BufferPtr += sizeof(TPM_1_2_CMD_HEADER);
    
    PCRIndex = TPM_H2NL(PCRIndex);
    
    (*ps)->CopyMem(BufferPtr, &PCRIndex, sizeof(TPM_PCRINDEX));
    
    BufferPtr+= sizeof(TPM_PCRINDEX);
    
    (*ps)->CopyMem(BufferPtr, Digest->digest, TPM_SHA1_160_HASH_LEN);
    
    //printbuffer(cmd, BufferSize);
    
    Status = TcgPpi->TCGPassThroughToTpm(TcgPpi,
                                ps,
                                BufferSize,
                                cmd,
                                BufferSize,
                                ret);
    
    DEBUG ((DEBUG_INFO, "Status = %r\n", Status));
    //printbuffer(ret, BufferSize);
    
    return Status;
}
    


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureLogDxeFwVol
//
// Description: Hashes FVMain Volume
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
MeasureLogDxeFwVol(
    IN  CONST EFI_PEI_SERVICES  **PeiServices,
    IN  FwVolHob                *TpmFwVolHob,
    IN  PEI_TPM_PPI             *TpmPpi,
    IN  PEI_TCG_PPI             *TcgPpi
)
{
    EFI_STATUS                        Status = EFI_SUCCESS;
    SHA1_CTX                          Sha1Ctx;
    PEI_EFI_POST_CODE                 *TcgEvent = NULL;
    UINT32                            EventNumber;
    UINT32                            i = 0;
    UINT32                            FwVolHobCount;
    TCG_DIGEST                        Sha1Digest;

    DEBUG(( DEBUG_INFO, "[%d] Enter MeasureLogDxeFwVol\n", __LINE__));

    Status = (*PeiServices)->AllocatePool( PeiServices,
                                           sizeof(PEI_EFI_POST_CODE),
                                           (void **)&TcgEvent);
    if (EFI_ERROR (Status))
    {
        return Status;
    }

    FwVolHobCount = TpmFwVolHob->Count;

    TcgEvent->Header.PCRIndex      = AMI_PCRi_CRTM_AND_POST_BIOS;
    TcgEvent->Header.EventType     = EV_POST_CODE;
    TcgEvent->Header.EventDataSize = sizeof (EFI_TCG_EV_POST_CODE);
    TcgEvent->Event.PostCodeAddress = TpmFwVolHob[0].baseAddress;
    TcgEvent->Event.PostCodeLength = (UINT64)TpmFwVolHob[0].Size;

    SHA1Init( &Sha1Ctx );

    DEBUG ((DEBUG_INFO, "FwVolHobCount = %x \n", FwVolHobCount));
    for(i=0; i< FwVolHobCount; i++)
    {
        DEBUG ((DEBUG_INFO, "TpmFwVolHob[i].Size = %x \n", TpmFwVolHob[i].Size));
        DEBUG ((DEBUG_INFO, "TpmFwVolHob[i].baseAddress = %lx \n", TpmFwVolHob[i].baseAddress));
        SHA1Update( &Sha1Ctx,  (unsigned char *)(UINTN)TpmFwVolHob[i].baseAddress, (u32)TpmFwVolHob[i].Size );
    }

    SHA1Final(Sha1Digest.digest, &Sha1Ctx);

    //printbuffer(Sha1Digest.digest, 20);

    (*PeiServices)->CopyMem(TcgEvent->Header.Digest.digest, Sha1Digest.digest, TPM_SHA1_160_HASH_LEN);

    TpmPpi->Init(TpmPpi, (EFI_PEI_SERVICES **)PeiServices);

    Status = TcgPeiSimpleExtend(
                  TcgPpi,
                  (EFI_PEI_SERVICES **)PeiServices,
                  TcgEvent->Header.PCRIndex,
                  &TcgEvent->Header.Digest,
                  &Sha1Digest );

    TpmPpi->Close(TpmPpi, (EFI_PEI_SERVICES **)PeiServices);

    if (EFI_ERROR (Status))
    {
        return Status;
    }

    Status = TcgPpi->TCGLogEvent( TcgPpi,
                                  (EFI_PEI_SERVICES **)PeiServices,
                                  (TCG_PCR_EVENT *)TcgEvent,
                                  &EventNumber);

    DEBUG(( DEBUG_INFO, "MeasureLogDxeFwVol - %r\n", Status));

    TpmPeiReportStatusCodeEx(EFI_PROGRESS_CODE, AMI_SPECIFIC_BIOS_FWVOL_MEASURED | EFI_SOFTWARE_PEI_MODULE,
                             0, NULL, &ExtendedDataGuid, &TcgEvent->Event, sizeof (EFI_TCG_EV_POST_CODE));
    TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_BIOS_FWVOL_MEASURED | EFI_SOFTWARE_PEI_MODULE);

    return Status;
}


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureTeImage
//
// Description: Measure Te Images (if DxeCore is a TE image)
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
MeasureTeImage (
        IN  EFI_IMAGE_LOAD_EVENT     *ImageLoad,
        IN  UINT8                    *Digest)
{
    SHA1_CTX                          Sha1Ctx;
    EFI_TE_IMAGE_HEADER               *ptrToTEHdr;
    EFI_IMAGE_SECTION_HEADER          *Section;
    UINT8                             *HashBase;
    UINT64                            HashSize;
    UINT64                            SumOfBytesHashed;
    UINTN                             SectionHeaderOffset;
    UINTN                             numOfSectionHeaders;
    UINTN                             Index;


    // 2. Initialize a SHA hash context.
    SHA1Init(&Sha1Ctx);
                             
    DEBUG ((DEBUG_INFO, "Printing TE Image Buffer in Memory Image Location = %x, Image size = %x\n",
    (UINTN)ImageLoad->ImageLocationInMemory, ImageLoad->ImageLengthInMemory));
           
    //printbuffer((UINT8 *)(UINTN)ImageLoad->ImageLocationInMemory, 1024);
           
    ptrToTEHdr = (EFI_TE_IMAGE_HEADER *)((UINT8 *)(UINTN)ImageLoad->ImageLocationInMemory);
    //Hash TE Image header and section headers
           
    HashBase = (UINT8 *)(UINTN)ptrToTEHdr;
    HashSize = sizeof(EFI_TE_IMAGE_HEADER) + ( EFI_IMAGE_SIZEOF_SECTION_HEADER * ptrToTEHdr->NumberOfSections);
           
    DEBUG ((DEBUG_INFO, "Printing Hashed TE Hdr and Section Header\n"));
    DEBUG ((DEBUG_INFO, "Base = %x Len = %x \n", HashBase, HashSize));
    
    //printbuffer(HashBase, HashSize);
    SHA1Update(&Sha1Ctx,
                HashBase,
                (u32)HashSize);
                                       
    SectionHeaderOffset = sizeof(EFI_TE_IMAGE_HEADER) + (UINTN)ptrToTEHdr;
    numOfSectionHeaders = ptrToTEHdr->NumberOfSections;
           
    SumOfBytesHashed = HashSize;
           
    //check for alignment
    //hash Alignment buffer
    HashSize = (((EFI_IMAGE_SECTION_HEADER *)((UINT8 *)SectionHeaderOffset))->PointerToRawData 
                - ptrToTEHdr->StrippedSize + sizeof(EFI_TE_IMAGE_HEADER)+ (UINT64)ImageLoad->ImageLocationInMemory 
                - ((UINT64)ImageLoad->ImageLocationInMemory + HashSize));
    
    if(HashSize !=0){
    
        HashBase += sizeof(EFI_TE_IMAGE_HEADER) + ( EFI_IMAGE_SIZEOF_SECTION_HEADER * ptrToTEHdr->NumberOfSections);               

        DEBUG ((DEBUG_INFO, "Printing Hashed TE Alignment Buffer\n"));
        DEBUG ((DEBUG_INFO, "Base = %x Len = %x \n", HashBase, HashSize));
            
        SHA1Update(&Sha1Ctx,
                        HashBase,
                        (u32)HashSize);
                                             
        SumOfBytesHashed+=HashSize;
        
    }

    //
    // TE Images do not have a CertDirector
    //
    //process hash section by section
    //hash in order sections. We do not sort TE sections
    for (Index = 0; Index < numOfSectionHeaders; Index++) {
        Section = (EFI_IMAGE_SECTION_HEADER *)((UINT8 *)SectionHeaderOffset);
        
        if (Section->SizeOfRawData == 0) {
            continue;
        }
                      
        //Hash raw data
        HashBase = (UINT8 *)(((UINTN)ImageLoad->ImageLocationInMemory) 
                            + (UINTN)Section->PointerToRawData - ptrToTEHdr->StrippedSize + sizeof(EFI_TE_IMAGE_HEADER));
               
        HashSize = (UINTN) Section->SizeOfRawData;
                      
        DEBUG ((DEBUG_INFO, "Section Base = %x Section Len = %x \n", HashBase, HashSize));
        //printbuffer(HashBase, 1024);

        SHA1Update(&Sha1Ctx,
                   HashBase,
                   (u32)HashSize);

        SumOfBytesHashed += HashSize;              
        SectionHeaderOffset += EFI_IMAGE_SIZEOF_SECTION_HEADER;
    }  
    
    DEBUG ((DEBUG_INFO, "SumOfBytesHashed = %x \n", SumOfBytesHashed));
                 
    //verify size
    if ( (UINT32)ImageLoad->ImageLengthInMemory > SumOfBytesHashed) {
        DEBUG ((DEBUG_INFO, "Hash rest of Data if true \n"));
        
        HashBase = (UINT8 *)(UINTN)ImageLoad->ImageLocationInMemory + SumOfBytesHashed;
        HashSize = (UINTN)(ImageLoad->ImageLengthInMemory - SumOfBytesHashed);
        
        DEBUG ((DEBUG_INFO, "Base = %x Len = %x \n", HashBase, HashSize));
        //printbuffer(HashBase, 106);

        SHA1Update(&Sha1Ctx,
                HashBase,
                (u32)HashSize);

    }
                  
    //
    // Finalize the SHA hash.
    //
    SHA1Final(Digest, &Sha1Ctx);

    return EFI_SUCCESS;
}

              
//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MeasureDxeCorePEImage
//
// Description: Measure Dxe Core as a PE Image
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI
MeasureDxeCorePEImage (
   IN  CONST EFI_PEI_SERVICES **PeiServices,
   IN  EFI_PHYSICAL_ADDRESS      ImageAddress,
   IN  UINT32                     ImageSize,
   IN  PEI_TPM_PPI              *TpmPpi,
   IN  PEI_TCG_PPI              *TcgPpi,
   IN  TCG_CONFIGURATION        *ConfigFlags
)
{
   EFI_STATUS                        Status;
   TCG_PCR_EVENT                     *TcgEvent;
   TCG_PCR_EVENT                     *TcgEventlog = NULL;
   EFI_IMAGE_LOAD_EVENT              *ImageLoad;
    EFI_DEVICE_PATH_PROTOCOL          *FullPath;
    UINT32                            FullPathSize;
   SHA1_CTX                          Sha1Ctx;
   EFI_IMAGE_DOS_HEADER              *DosHdr;
   UINT32                            PeCoffHeaderOffset;
    EFI_IMAGE_NT_HEADERS64            *Hdr;
   EFI_IMAGE_SECTION_HEADER          *Section;
   UINT8                             *HashBase;
   UINTN                             HashSize;
   UINTN                             SumOfBytesHashed;
   EFI_IMAGE_SECTION_HEADER          *SectionHeader;
    UINTN                             Index, iPos;
   UINT32                            EventSize;
   UINT32                            EventNumber;
   TCG_DIGEST                        Sha1Digest;
    UINT8                             *EventData = NULL;

                   
   ImageLoad     = NULL;
    FullPath      = NULL;
   SectionHeader = NULL;
    FullPathSize  = 0;

   DEBUG ((DEBUG_INFO, "PeiMeasurePeImage Entry\n"));

    Status = (*PeiServices)->AllocatePool(PeiServices, ((sizeof (*ImageLoad)
                                - sizeof (ImageLoad->DevicePath)) + FullPathSize), (void **)&EventData);
    if(EFI_ERROR(Status))return Status;
   //
   // Determine destination PCR by BootPolicy
   //   
   EventSize  = sizeof (*ImageLoad) - sizeof (ImageLoad->DevicePath);
   EventSize += FullPathSize;

    Status = (*PeiServices)->AllocatePool(PeiServices,EventSize, (void **)&ImageLoad);
   
   if (ImageLoad == NULL || EFI_ERROR(Status)) {
        return EFI_OUT_OF_RESOURCES;
   }
   
   Status = (*PeiServices)->AllocatePool(PeiServices, sizeof(TCG_PCR_EVENT_HDR)+EventSize, (void **)&TcgEvent);
      if(EFI_ERROR(Status))return Status;

   TcgEvent->EventSize = EventSize;
   
   ImageLoad->ImageLocationInMemory = ImageAddress;
   ImageLoad->ImageLengthInMemory   = ImageSize;
   ImageLoad->ImageLinkTimeAddress  = 0;
   ImageLoad->LengthOfDevicePath    = 0;
   
   DEBUG ((DEBUG_INFO, "ImageLoad->ImageLocationInMemory = %lx,  ImageLoad->ImageLengthInMemory = %lx \n", 
           ImageLoad->ImageLocationInMemory, (UINT32)ImageLoad->ImageLengthInMemory));
     
   //
   // Check PE/COFF image
   //
   DosHdr = (EFI_IMAGE_DOS_HEADER *)(UINTN)ImageAddress;
   PeCoffHeaderOffset = 0;
   if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
        PeCoffHeaderOffset = DosHdr->e_lfanew;
   }
   
   if (((EFI_TE_IMAGE_HEADER *)((UINT8 *)(UINTN)ImageAddress + PeCoffHeaderOffset))->Signature 
          == EFI_TE_IMAGE_HEADER_SIGNATURE) {
            
        //Measure TE Image
        MeasureTeImage(ImageLoad, Sha1Digest.digest);
        (*PeiServices)->CopyMem(&TcgEvent->Digest.digest, Sha1Digest.digest, SHA1_DIGEST_SIZE);
        goto TEImageDone;
   }
   
   DEBUG ((DEBUG_INFO, "Image is a PE image\n"));

   //
   // PE/COFF Image Measurement
   //
   //    NOTE: The following codes/steps are based upon the authenticode image hashing in 
   //      PE/COFF Specification 8.0 Appendix A.
   //      
   //

   // 1. Load the image header into memory.
      
   // 2. Initialize a SHA hash context.
   // 1.   Load the image header into memory.
    
    // 2. Initialize a SHA hash context.
    SHA1Init( &Sha1Ctx);

    //
    // Measuring PE/COFF Image Header; 
    // But CheckSum field and SECURITY data directory (certificate) are excluded
    //
    Hdr   = (EFI_IMAGE_NT_HEADERS64 *)((UINT8 *)(UINTN)ImageAddress + PeCoffHeaderOffset);

    //
     // 3.  Calculate the distance from the base of the image header to the image checksum address.
     // 4.  Hash the image header from its base to beginning of the image checksum.
     //
     HashBase = (UINT8 *) (UINTN) ImageAddress;
     HashSize = (UINTN) ((UINT8 *)(&Hdr->OptionalHeader.CheckSum) - HashBase);
  
     SHA1Update( &Sha1Ctx,
                HashBase,
               (u32) HashSize
                );
    
    //
    // 5. Skip over the image checksum (it occupies a single ULONG).
    // 6. Get the address of the beginning of the Cert Directory.
    // 7. Hash everything from the end of the checksum to the start of the Cert Directory.
    //

    HashBase = (UINT8 *) &Hdr->OptionalHeader.CheckSum + sizeof (UINT32);
    HashSize = (UINTN) ((UINT8 *)(&Hdr->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY]) - HashBase);

    
    SHA1Update( &Sha1Ctx,
                HashBase,
                (u32)HashSize
                );  
    //
    // 8. Skip over the Cert Directory. (It is sizeof(IMAGE_DATA_DIRECTORY) bytes.)
    // 9. Hash everything from the end of the Cert Directory to the end of image header.
    //
    HashBase = (UINT8 *) &Hdr->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1];
    HashSize = Hdr->OptionalHeader.SizeOfHeaders - 
                 (UINTN) ((UINT8 *)(&Hdr->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY + 1]) - (UINT8 *)(UINTN)ImageAddress);

    
    SHA1Update( &Sha1Ctx,
                HashBase,
                (u32)HashSize
                );
    
    //
    // 10. Set the SUM_OF_BYTES_HASHED to the size of the header 
    //
    SumOfBytesHashed = Hdr->OptionalHeader.SizeOfHeaders;

    
    //
    // 11. Build a temporary table of pointers to all the IMAGE_SECTION_HEADER 
    //     structures in the image. The 'NumberOfSections' field of the image 
    //     header indicates how big the table should be. Do not include any 
    //     IMAGE_SECTION_HEADERs in the table whose 'SizeOfRawData' field is zero.   
    //
    Status = (*PeiServices)->AllocatePool(PeiServices,sizeof (EFI_IMAGE_SECTION_HEADER) * Hdr->FileHeader.NumberOfSections, (void **)&SectionHeader);

    if(SectionHeader==NULL || EFI_ERROR(Status))return EFI_OUT_OF_RESOURCES;
    (*PeiServices)->SetMem(SectionHeader, (sizeof (EFI_IMAGE_SECTION_HEADER) * Hdr->FileHeader.NumberOfSections), 0);
    
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
                      Hdr->FileHeader.SizeOfOptionalHeader
                    );
    for (Index = 0; Index < Hdr->FileHeader.NumberOfSections; Index++) {
           iPos = Index;
           while ((iPos > 0) && (Section->PointerToRawData < SectionHeader[iPos - 1].PointerToRawData)) {
               (*PeiServices)->CopyMem (&SectionHeader[iPos], &SectionHeader[iPos - 1], sizeof(EFI_IMAGE_SECTION_HEADER));
             iPos--;
           }
           
           (*PeiServices)->CopyMem( &SectionHeader[iPos], Section,
                           sizeof(EFI_IMAGE_SECTION_HEADER));
           Section += 1;    
    }
    
    
    //
    // 13.    Walk through the sorted table, bring the corresponding section 
    //      into memory, and hash the entire section (using the 'SizeOfRawData' 
    //      field in the section header to determine the amount of data to hash).
    // 14.    Add the section's 'SizeOfRawData' to SUM_OF_BYTES_HASHED .
    // 15.    Repeat steps 13 and 14 for all the sections in the sorted table.
    //
    for (Index = 0; Index < Hdr->FileHeader.NumberOfSections; Index++) {
           Section  = (EFI_IMAGE_SECTION_HEADER *) &SectionHeader[Index];
           if (Section->SizeOfRawData == 0) {
             continue;
           }
           HashBase = (UINT8 *)(UINTN)ImageAddress + Section->PointerToRawData;
           HashSize = (UINTN) Section->SizeOfRawData;
    
           SHA1Update(&Sha1Ctx,
                                 HashBase,
                                  (u32)HashSize
                                );
    
           SumOfBytesHashed += HashSize;
       }
    
    
    //
    // 16.    If the file size is greater than SUM_OF_BYTES_HASHED, there is extra
    //      data in the file that needs to be added to the hash. This data begins 
    //      at file offset SUM_OF_BYTES_HASHED and its length is:
    //             FileSize  -  (CertDirectory->Size)
    //
    if (ImageSize > SumOfBytesHashed) {
           HashBase = (UINT8 *)(UINTN)ImageAddress + SumOfBytesHashed;
           HashSize = (UINTN)(ImageSize -
                      Hdr->OptionalHeader.DataDirectory[EFI_IMAGE_DIRECTORY_ENTRY_SECURITY].Size -
                      SumOfBytesHashed);
           

        SHA1Update(&Sha1Ctx,
                     HashBase,
                     (u32)HashSize);
    }

    //
    // 17.    Finalize the SHA hash.
    //
    SHA1Final(Sha1Digest.digest, &Sha1Ctx);

TEImageDone:        

    //printbuffer(Sha1Digest.digest,  sizeof (TcgEvent->Digest.digest));
    (*PeiServices)->CopyMem (&TcgEvent->Digest.digest, Sha1Digest.digest, sizeof (TcgEvent->Digest.digest));
        
      
     TpmPpi->Init(TpmPpi, (EFI_PEI_SERVICES **)PeiServices);

     
     Status = TcgPeiSimpleExtend (
                  TcgPpi,
                  (EFI_PEI_SERVICES **)PeiServices,
                  AMI_PCRi_CRTM_AND_POST_BIOS,
                  &TcgEvent->Digest,
                  &Sha1Digest
                 );
    

     TpmPpi->Close(TpmPpi, (EFI_PEI_SERVICES **)PeiServices);
     
     if (!EFI_ERROR (Status)) {  
    
          Status = (*PeiServices)->AllocatePool(PeiServices,EventSize + sizeof (TCG_PCR_EVENT), (void **)&TcgEventlog);
          if(EFI_ERROR(Status) || TcgEventlog == NULL )return Status;
                 
          //Now log the event
          TcgEventlog->PCRIndex  = AMI_PCRi_CRTM_AND_POST_BIOS;
          TcgEventlog->EventType = EV_POST_CODE;
          TcgEventlog->EventSize = TcgEvent->EventSize;
          DEBUG ((DEBUG_INFO, "TcgEvent->EventSize = %x \n", TcgEvent->EventSize));

          (*PeiServices)->CopyMem (&TcgEventlog->Digest, &TcgEvent->Digest, sizeof(TCG_DIGEST));
          (*PeiServices)->CopyMem (&TcgEventlog->Event, ImageLoad, TcgEvent->EventSize);
          //printbuffer((UINT8 *)TcgEventlog, 50);
          Status = TcgPpi->TCGLogEvent(TcgPpi,(EFI_PEI_SERVICES **)PeiServices, TcgEventlog, &EventNumber);
          TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_CRTM_VERSION_MEASURED | EFI_SOFTWARE_PEI_MODULE);
     }
     
//Finish:   
     return Status;
}



#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
// <AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Name: GetRomLayout
//
// Description: 
//
// Input:
//  OUT UINT8 **pRomLayout
//  OUT UINTN *Size
//
// Output:      EFI_STATUS
//
//--------------------------------------------------------------------------
// <AMI_PHDR_END>
EFI_STATUS GetRomArea(
  IN OUT AMI_ROM_AREA **pRomLayout,
  IN OUT UINTN *Size,
  IN  AMI_ROM_AREA *First
)
{             
  if(pRomLayout == NULL)return EFI_INVALID_PARAMETER;
  if(First == NULL){
      *pRomLayout = AmiGetFirstRomArea();
      if(*pRomLayout == NULL)return EFI_NOT_FOUND;
  }else{
      *pRomLayout = AmiGetNextRomArea(First);
      if(*pRomLayout == NULL)return EFI_NOT_FOUND;
  }
  
  *Size = (*pRomLayout)->Size;
  return EFI_SUCCESS;
}
#endif
  
#ifndef ROM_AREA_TCG_MEASURED
// Platform Firmware Volume that either contains or 
// measures the EFI Boot Services and EFI Run Time Services; (code measured into TPM PCR[0])
#define ROM_AREA_TCG_MEASURED     0x000008000  
#endif

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   InternalPeiBuildHobGuid
//
// Description: Internal abstracted function to create a Hob
//
// Input:       IN  EFI_PEI_SERVICES  **PeiServices,
//              IN  EFI_GUID          *Guid,
//              IN  UINTN             DataLength,
//              OUT VOID              **Hob
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS InternalPeiBuildHobGuid(
    IN EFI_PEI_SERVICES **PeiServices,
    IN EFI_GUID         *Guid,
    IN UINTN            DataLength,
    OUT VOID            **Hob )
{
    EFI_STATUS Status;

    Status = (*PeiServices)->CreateHob(
        (CONST EFI_PEI_SERVICES **)PeiServices,
        EFI_HOB_TYPE_GUID_EXTENSION,
        (UINT16) ( sizeof (EFI_HOB_GUID_TYPE) + DataLength ),
        Hob
        );

    if ( EFI_ERROR( Status ))
    {
        DEBUG((DEBUG_INFO, "Failed to create TCG/TPM Hob Status = %r \n", Status));
        return Status;
    }
    
    DEBUG((DEBUG_INFO, "Hob created \n")); 
    ((EFI_HOB_GUID_TYPE*)(*Hob))->Name = *Guid;

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ImageRead (
  IN     VOID    *FileHandle,
  IN     UINTN   FileOffset,
  IN OUT UINTN   *ReadSize,
  OUT    VOID    *Buffer
  )
{
  CopyMem (Buffer, (UINT8 *)((UINTN) FileHandle + FileOffset), *ReadSize);
  return EFI_SUCCESS;
}


#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
EFI_STATUS Tpm12UpdateLogWithVerifiedHash (CONST EFI_PEI_SERVICES  **PeiServices,
                                      AMI_FV_DIGEST_PPI *VerifiedHashDigestPpi,
                                      PEI_TCG_PPI             *TcgPpi)
{
    EFI_STATUS Status = EFI_NOT_FOUND;
    PEI_EFI_POST_CODE               TcgEvent;
    UINT32                          EventNum;
    UINTN                           i = 0;
    
    
    DEBUG(( DEBUG_INFO, "UpdateLogWithVerifiedHash \n"));
    
    if((PeiServices == NULL) || (VerifiedHashDigestPpi == NULL)
            || TcgPpi == NULL ){
        return EFI_INVALID_PARAMETER;
    }
    
    TcgEvent.Header.PCRIndex      = AMI_PCRi_CRTM_AND_POST_BIOS;
    TcgEvent.Header.EventType     = EV_POST_CODE;
    TcgEvent.Header.EventDataSize = sizeof (EFI_TCG_EV_POST_CODE);
       
    DEBUG(( DEBUG_INFO, "VerifiedHashDigestPpi->AddrList[0] = %x \n", VerifiedHashDigestPpi->AddrList[0]));
    TcgEvent.Event.PostCodeAddress = (EFI_PHYSICAL_ADDRESS)VerifiedHashDigestPpi->AddrList[0];
    TcgEvent.Event.PostCodeLength = 0;
    for(i=0; i<VerifiedHashDigestPpi->HashElemCnt; i++){
        DEBUG(( DEBUG_INFO, "VerifiedHashDigestPpi->LenList = %x \n", VerifiedHashDigestPpi->LenList[i]));
        TcgEvent.Event.PostCodeLength += (UINT64)VerifiedHashDigestPpi->LenList[i];
    }
    
    DEBUG(( DEBUG_INFO, "EventData.BlobLength = %x \n", TcgEvent.Event.PostCodeLength));
    printbuffer(VerifiedHashDigestPpi->FvHash,VerifiedHashDigestPpi->HashSize);
    
    Status = TcgPpi->TCGHashLogExtendEvent(TcgPpi, (EFI_PEI_SERVICES  **)PeiServices, VerifiedHashDigestPpi->FvHash, 
                                        VerifiedHashDigestPpi->HashSize, (TCG_PCR_EVENT *)&TcgEvent, &EventNum );

    printbuffer(VerifiedHashDigestPpi->FvHash,VerifiedHashDigestPpi->HashSize);
    DEBUG(( DEBUG_INFO, "UpdateLogWithVerifiedHash - %r\n", Status));
    
    return Status;
}

EFI_STATUS
EFIAPI
Tpm12EndOfPeiSignalPpiNotifyCallback (
        IN EFI_PEI_SERVICES           **PeiServices,
        IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
        IN VOID                       *Ppi
)
{
    AMI_FV_DIGEST_PPI               *VerifiedHashDigestPpi = NULL;
    EFI_GUID     gAmiFvDigestPpiguid = AMI_FV_DIGEST_PPI_GUID;
    PEI_TPM_PPI             *TpmPpi         = NULL;         
    PEI_TCG_PPI             *TcgPpi         = NULL;
    EFI_STATUS              Status= EFI_NOT_FOUND;
    
    Status = LocateTcgPpi((EFI_PEI_SERVICES **)PeiServices, &TpmPpi, &TcgPpi);
    if(EFI_ERROR(Status)){
      DEBUG((DEBUG_ERROR, "\n LocateTcgPpi is failed \n"));
      return EFI_NOT_FOUND;
    }
    
    Status = (*PeiServices)->LocatePpi(PeiServices,
                                    &gAmiFvDigestPpiguid,
                                    0, NULL,
                                    (void **)&VerifiedHashDigestPpi);
    
    if(!EFI_ERROR(Status) && (VerifiedHashDigestPpi != NULL)){
        Status = Tpm12UpdateLogWithVerifiedHash(PeiServices, VerifiedHashDigestPpi,
                                                TcgPpi);
        return Status;
    }else{
        
        Status = FindAndMeasureDxeCore((EFI_PEI_SERVICES   **)PeiServices);
        if ( EFI_ERROR( Status ))
        {
            DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
        }
    }

    return Status;
}


EFI_PEI_NOTIFY_DESCRIPTOR mTpm12EndOfPeiSignalPpiNotifyList[] = {
    {
           (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
            &gEfiEndOfPeiSignalPpiGuid,
            Tpm12EndOfPeiSignalPpiNotifyCallback
    }
};

#endif
   
//*******************************************************************************
//<AMI_PHDR_START>
//
// Procedure:   FindAndMeasureDxeCore
//
// Description: 
//
// Input:      
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//******************************************************************************
EFI_STATUS FindAndMeasureDxeCore(EFI_PEI_SERVICES **PeiServices)
{

   EFI_STATUS              Status;
   PEI_TPM_PPI             *TpmPpi         = NULL;         
   PEI_TCG_PPI             *TcgPpi         = NULL;
   TCG_PLATFORM_SETUP_INTERFACE    *TcgPeiPolicy=NULL;
   TCG_CONFIGURATION               ConfigFlags;

#if defined(MeasureDxeCorePe) && (MeasureDxeCorePe == 1)
   EFI_HOB_MEMORY_ALLOCATION_MODULE    *DxeCoreHob;
   EFI_PEI_HOB_POINTERS                  Hob;
   BOOLEAN                         Found=FALSE;
#endif
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
   AMI_ROM_AREA                    *RomArea = NULL;
   UINTN                           RomArea_size;
#endif
   FwVolHob                        *TpmFwVolHob = NULL;
   FwVolHob                        FwVolHobArray[10];
   UINTN                           i=0;
   UINTN                            Count=0;
   EFI_HOB_GUID_TYPE               *VolHob = NULL;

   
   //locate the PeiTree Protocol
    Status = LocateTcgPpi((EFI_PEI_SERVICES **)PeiServices, &TpmPpi, &TcgPpi);
    if(EFI_ERROR(Status)){
      DEBUG((DEBUG_ERROR, "\n LocateTcgPpi is failed \n"));
      return EFI_NOT_FOUND;
    }
   
    //create RomAreaOHob for Measurement of FV in Dxemain
#if defined (TCGRomLayout_SUPPORT) && (TCGRomLayout_SUPPORT!=0)
    Status = GetRomArea(&RomArea,&RomArea_size, NULL);
    if( (!EFI_ERROR(Status))&&(RomArea != NULL) )
    {
        for(i=0; RomArea!=NULL; i++)
        {
            
            if((RomArea->Attributes & ROM_AREA_FV_SIGNED) && (RomArea->Attributes & ROM_AREA_TCG_MEASURED) ||
                ((RomArea->Attributes & (ROM_AREA_FV_PEI+ROM_AREA_FV_DXE)) &&
                !(RomArea->Attributes & ROM_AREA_VITAL) ) )  // Attributes)
            {
                
                FwVolHobArray[Count].baseAddress = RomArea->Address;  
                FwVolHobArray[Count].Size        = RomArea->Size;  
               DEBUG ((DEBUG_INFO, "RomArea->Address = %x \n", RomArea->Address));
               DEBUG ((DEBUG_INFO, "RomArea->Size = %x \n",RomArea->Size));
                Count +=1;
            }
            
            GetRomArea(&RomArea,&RomArea_size, RomArea);
        }
    }
    else
    {
        FwVolHobArray[Count].baseAddress = FV_MAIN_BASE;
        FwVolHobArray[Count].Size = FV_MAIN_SIZE;
        Count+=1;
    }
#else
    FwVolHobArray[Count].baseAddress = FV_MAIN_BASE;
    FwVolHobArray[Count].Size = FV_MAIN_SIZE;
    Count+=1;
#endif
    
    Status = InternalPeiBuildHobGuid(PeiServices, &amiFVhoblistguid,
                              (sizeof(FwVolHob)*Count),  (void **)&VolHob);
    if (EFI_ERROR (Status))
    {
        return Status;
    }
    
    Status = (*PeiServices)->LocatePpi(
                    (CONST EFI_PEI_SERVICES **)PeiServices,
                     &gTcgPeiPolicyGuid,
                     0, NULL,
                     (void **)&TcgPeiPolicy);
    
    if(EFI_ERROR(Status)){
        return EFI_NOT_FOUND;  
    }
           
    Status = TcgPeiPolicy->getTcgPeiPolicy(PeiServices, &ConfigFlags);
    if(EFI_ERROR(Status))return Status;
    
    TpmFwVolHob = (FwVolHob*)(VolHob + 1);
    for(i=0; i<Count; i++){
       
       if(i==0){
           TpmFwVolHob->Size = FwVolHobArray[i].Size;
           TpmFwVolHob->baseAddress = FwVolHobArray[i].baseAddress;
           TpmFwVolHob->Count = Count;
           // For TPM 1.2, the Tcg2SpecVersion should be 0
           TpmFwVolHob->Tcg2SpecVersion = 0;
       }else{
           TpmFwVolHob->Size = FwVolHobArray[i].Size;
           TpmFwVolHob->baseAddress = FwVolHobArray[i].baseAddress;
           TpmFwVolHob->Count = 0;
           TpmFwVolHob->Tcg2SpecVersion = 0;
       }
       
       DEBUG ((DEBUG_INFO, "TpmFwVolHob->Size = %x \n", TpmFwVolHob->Size));
       DEBUG ((DEBUG_INFO, "TpmFwVolHob->baseAddress = %x \n",TpmFwVolHob->baseAddress));
       DEBUG ((DEBUG_INFO, "TpmFwVolHob->Tcg2SpecVersion = %x \n",TpmFwVolHob->Tcg2SpecVersion));
       DEBUG ((DEBUG_INFO, "TpmFwVolHob address = %x \n", TpmFwVolHob));
       TpmFwVolHob+=1;
    }
                         
      
#if defined(SAVE_ENTIRE_FV_IN_MEM) && (SAVE_ENTIRE_FV_IN_MEM == 0)
    MeasureLogDxeFwVol(
            (CONST EFI_PEI_SERVICES **)PeiServices,
            ((FwVolHob*)(VolHob + 1)),
            TpmPpi,
            TcgPpi
            );
#endif
    //locate DxeCore Hob
    //treat it like a PE image
    // Hash and extend it
#if defined(MeasureDxeCorePe) && (MeasureDxeCorePe == 1)
    PeiServicesGetHobList(&Hob.Raw);
    while (!TCGEND_OF_HOB_LIST (Hob)) {
    //          DEBUG ((DEBUG_INFO, "Hob.Raw = %x \n", Hob.Raw));
    //          DEBUG ((DEBUG_INFO, "Hob.Header->HobType = %x \n",Hob.Header->HobType));
         if(Hob.Header->HobType == EFI_HOB_TYPE_MEMORY_ALLOCATION){
             DxeCoreHob = (EFI_HOB_MEMORY_ALLOCATION_MODULE *) (Hob.Raw);
         }
         
         if (Hob.Header->HobType == EFI_HOB_TYPE_MEMORY_ALLOCATION && (CompareMem ((UINT8 *)(&DxeCoreHob->MemoryAllocationHeader.Name), &gEfiHobMemoryAllocModuleGuid, sizeof(EFI_GUID))==0)) {                
             Found = TRUE;
             break;
         }
         Hob.Raw = TCGGET_NEXT_HOB (Hob);
    }
    
    DEBUG ((DEBUG_INFO, "Found = %x \n", Found));
    
    if(!Found)return EFI_NOT_FOUND;
    
    DEBUG ((DEBUG_INFO, "DxeCoreHob->MemoryAllocationHeader.MemoryBaseAddress = %lx,  DxeCoreHob->MemoryAllocationHeader.MemoryLength = %lx \n", 
           DxeCoreHob->MemoryAllocationHeader.MemoryBaseAddress, DxeCoreHob->MemoryAllocationHeader.MemoryLength));
    
    //printbuffer((UINT8 *)DxeCoreHob->MemoryAllocationHeader.MemoryBaseAddress, (UINTN)DxeCoreHob->MemoryAllocationHeader.MemoryLength);
    //printbuffer((UINT8 *)(ImageContext.ImageAddress + DxeCoreHob->MemoryAllocationHeader.MemoryLength - 100), DxeCoreHob->MemoryAllocationHeader.MemoryLength);
    
    //call PE hashlog Extend EFI Image
    MeasureDxeCorePEImage(PeiServices,DxeCoreHob->MemoryAllocationHeader.MemoryBaseAddress,
               (UINT32)DxeCoreHob->MemoryAllocationHeader.MemoryLength, TpmPpi, TcgPpi, &ConfigFlags);
    
#endif    
    return Status;
}



//NIST-155    
    
    
    
    

EFI_STATUS MeasureNISTManifest(
        IN EFI_PEI_SERVICES **ps, 
        IN PEI_TCG_PPI      *tcg );


//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   VerifyTcgVariables
//
// Description: Function to check whether we need to reset TCG variables
//
//
// Input:       EFI_PEI_SERVICES **PeiServices
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS VerifyTcgVariables(
    EFI_PEI_SERVICES **PeiServices )
{
    AMI_TCG_PEI_FUNCTION_OVERRIDE_PPI       *VerifyVarOverride;
    EFI_STATUS						Status;

    
    Status = (*PeiServices)->LocatePpi(
                   (CONST EFI_PEI_SERVICES    **)PeiServices,
                    &AmiVerifyTcgVariablesGuid,
                    0, NULL,
                    (void **)&VerifyVarOverride);

    if(!EFI_ERROR(Status)){
        return (VerifyVarOverride->Function(PeiServices));
    }

    return EFI_SUCCESS;
}




//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   Set_TPMPhysicalPresence
//
// Description: Sets TPM physical Presence
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS Set_TPMPhysicalPresence(
    IN EFI_PEI_SERVICES **PeiServices )
{
    EFI_STATUS                      Status;
    TPM_RSP_COMMAND_HDR             RspHdr;
    TPM_Capabilities_PermanentFlag  Cap;

    AMI_TCG_PEI_FUNCTION_OVERRIDE_PPI   *SetPhysicalOverride;
    struct
    {
        TPM_RQU_COMMAND_HDR CmdHdr;
        UINT8               Data[0x4];
    } cmd;

    TPM_Capabilities_PermanentFlag INTTCGPEI_GETCAP( EFI_PEI_SERVICES **PeiServices );

    UINT16        physical_CMD_on = TPM_H2NS(TPM_PHYSICAL_PRESENCE_CMD_ENABLE );
    UINT16        physical_on     = TPM_H2NS(TPM_PHYSICAL_PRESENCE_PRESENT );
    PEI_TPM_PPI   *TpmPpi         = NULL;         
    PEI_TCG_PPI   *TcgPpi         = NULL;


    Status = (*PeiServices)->LocatePpi(
                    (CONST EFI_PEI_SERVICES    **)PeiServices,
                    &AmiSetPhysicalPresenceGuid,
                    0, NULL,
                    (void **)&SetPhysicalOverride);

    if(!EFI_ERROR(Status)){
        return (SetPhysicalOverride->Function(PeiServices));
    }
    
    if(IsTcmSupportType()){
    	return EFI_SUCCESS;
    }


    cmd.CmdHdr.tag =     TPM_H2NS( TPM_TAG_RQU_COMMAND );
    cmd.CmdHdr.paramSize = TPM_H2NL((UINT32)( sizeof (cmd.CmdHdr)
                           + sizeof(TPM_PHYSICAL_PRESENCE)));

    cmd.CmdHdr.ordinal = TPM_H2NL(TSC_ORD_PhysicalPresence );

    if ( Lock_TPMPhysicalPresence( PeiServices ))
    {
        physical_on = TPM_H2NS( TPM_PHYSICAL_PRESENCE_LOCK );
    }else{
		// When TCM plug into the board, we nee to skip the TPM selftest command.
        if( !IsTcmSupportType() )
        {
            Status = ContinueTPMSelfTest( PeiServices );
            if(EFI_ERROR(Status)){
                DEBUG((DEBUG_INFO, "\n ContinueTpmSelftest Status = %r \n", Status));
            }
        }
    }

    Status = LocateTcgPpi(PeiServices, &TpmPpi, &TcgPpi);
    if(EFI_ERROR(Status)){
        DEBUG((DEBUG_INFO, "\n ContinueTpmSelftest Status = %r \n", Status));
    }
    
    if(EFI_ERROR(Status))return EFI_NOT_FOUND;
    Cap = INTTCGPEI_GETCAP(PeiServices);
    if( 1 == Cap.physicalPresenceLifetimeLock )
    {
        Status = TpmPpi->Init( TpmPpi, PeiServices );
        if ( EFI_ERROR( Status ))
        {
            return Status;
        }

        CopyMem( cmd.Data, &physical_on, sizeof(TPM_PHYSICAL_PRESENCE));

        Status =TcgPpi->TCGPassThroughToTpm(
                    TcgPpi,
                    PeiServices,
                    (sizeof (cmd.CmdHdr) + sizeof(TPM_PHYSICAL_PRESENCE)),
                    (UINT8*)&cmd,
                    sizeof (RspHdr),
                    (UINT8*)&RspHdr );

        Status = TpmPpi->Close( TpmPpi, PeiServices );
        if ( EFI_ERROR( Status ))
        {
            return Status;
        }
        return EFI_SUCCESS;
    }

    Status = TpmPpi->Init( TpmPpi, PeiServices );
    if ( EFI_ERROR( Status ))
    {
        return Status;
    }

    CopyMem( cmd.Data, &physical_CMD_on, sizeof(TPM_PHYSICAL_PRESENCE));

    Status =TcgPpi->TCGPassThroughToTpm(
        TcgPpi,
        PeiServices,
        (sizeof (cmd.CmdHdr) + sizeof(TPM_PHYSICAL_PRESENCE)),
        (UINT8*)&cmd,
        sizeof (RspHdr),
        (UINT8*)&RspHdr );

    CopyMem( cmd.Data, &physical_on, sizeof(TPM_PHYSICAL_PRESENCE));

    Status = TcgPpi->TCGPassThroughToTpm(
        TcgPpi,
        PeiServices,
        (sizeof (cmd.CmdHdr) + sizeof(TPM_PHYSICAL_PRESENCE)),
        (UINT8*)&cmd,
        sizeof (RspHdr),
        (UINT8*)&RspHdr );

    Status = TpmPpi->Close( TpmPpi, PeiServices );
    if ( EFI_ERROR( Status ))
    {
        return Status;
    }

    if ( RspHdr.returnCode != 0 )
    {
        TpmPeiReportStatusCode(EFI_ERROR_CODE|EFI_ERROR_MAJOR, AMI_SPECIFIC_TPM_1_2_PP_LOCK_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);   
        return EFI_DEVICE_ERROR;
    }
    
    if( physical_on == TPM_H2NS( TPM_PHYSICAL_PRESENCE_LOCK )){
        TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_TPM_1_2_PP_LOCK_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);
    }
    
    return EFI_SUCCESS;
}





//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   Set_TCMPhysicalPresence
//
// Description: Sets TCM physical Presence
//
//
// Input:       IN      EFI_PEI_SERVICES          **PeiServices,
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS Set_TCMPhysicalPresence(
    IN EFI_PEI_SERVICES **PeiServices )
{
    EFI_STATUS               Status;
    TPM_RSP_COMMAND_HDR      RspHdr;
    UINT16  physical_CMD_on = TPM_H2NS(TPM_PHYSICAL_PRESENCE_CMD_ENABLE );
    UINT16  physical_on     = TPM_H2NS(TPM_PHYSICAL_PRESENCE_PRESENT );
    PEI_TCM_PPI     *TcmPpi = NULL;
    struct
    {
        TPM_RQU_COMMAND_HDR CmdHdr;
        UINT8               Data[0x4];
    } cmd;



    cmd.CmdHdr.tag =     TPM_H2NS( TPM_TAG_RQU_COMMAND );
    cmd.CmdHdr.paramSize = TPM_H2NL((UINT32)( sizeof (cmd.CmdHdr)
                           + sizeof(TPM_PHYSICAL_PRESENCE)));

    cmd.CmdHdr.ordinal = TPM_H2NL(TCM_TSC_ORD_PhysicalPresence );
    

    if ( Lock_TPMPhysicalPresence( PeiServices ))
    {
        physical_on = TPM_H2NS( TPM_PHYSICAL_PRESENCE_LOCK );
    }

    Status = LocateTcmPpi(PeiServices, &TcmPpi);
    ASSERT_EFI_ERROR(  Status );

    CopyMem( cmd.Data, &physical_CMD_on, sizeof(TPM_PHYSICAL_PRESENCE));

    Status = TcmPpi->TcmLibPassThrough(
                        TcmReg,
                        (UINT8*)&cmd,
                        (sizeof (cmd.CmdHdr) + sizeof(TPM_PHYSICAL_PRESENCE)),
                        (UINT8*)&RspHdr,
                        sizeof (RspHdr));

    CopyMem( cmd.Data, &physical_on, sizeof(TPM_PHYSICAL_PRESENCE));

    Status = TcmPpi->TcmLibPassThrough(
            TcmReg,
            (UINT8*)&cmd,
            (sizeof (cmd.CmdHdr) + sizeof(TPM_PHYSICAL_PRESENCE)),
            (UINT8*)&RspHdr,
            sizeof (RspHdr));

    if ( RspHdr.returnCode != 0 )
    {
        TpmPeiReportStatusCode(EFI_ERROR_CODE|EFI_ERROR_MAJOR, AMI_SPECIFIC_TPM_1_2_PP_LOCK_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);   
        return EFI_DEVICE_ERROR;
    }
    
    if( physical_on == TPM_H2NS( TPM_PHYSICAL_PRESENCE_LOCK )){
        TpmPeiReportStatusCode(EFI_PROGRESS_CODE, AMI_SPECIFIC_TPM_1_2_PP_LOCK_CMD_SENT | EFI_SOFTWARE_PEI_MODULE);
    }
    
    return Status;
}



//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:  TcgPeiGetRawImage
//
// Description:    Loads binary from RAW section of main firwmare volume
//
// Input:       IN EFI_PEI_SERVICES **PeiServices
//              IN OUT VOID         **Buffer
//              IN OUT UINT16       *size
//              IN     EFI_GUID     guid
//
// Output:        EFI_STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI TcgPeiGetRawImage(
    IN EFI_PEI_SERVICES **PeiServices,
    IN OUT VOID         **Buffer,
    IN OUT UINT16       *size,
    EFI_GUID            guid )
{
    EFI_STATUS                 Status;
    EFI_FIRMWARE_VOLUME_HEADER *pFV;
    UINTN                      FvNum   = 0;
    EFI_FFS_FILE_HEADER        *ppFile = NULL;
    MPDRIVER_LEGHEADER         *Temp;
    BOOLEAN                    Found = FALSE;

    while ( TRUE )
    {
        Status = (*PeiServices)->FfsFindNextVolume( (CONST EFI_PEI_SERVICES **)PeiServices, FvNum, (void **)&pFV );

        if ( EFI_ERROR( Status ))
        {
            return Status;
        }

        ppFile = NULL;

        while ( TRUE )
        {
            Status = (*PeiServices)->FfsFindNextFile( (CONST EFI_PEI_SERVICES **)PeiServices,
            										  EFI_FV_FILETYPE_ALL,
                                                      pFV,
                                                     (void **) &ppFile );

            if ( Status == EFI_NOT_FOUND )
            {
                break;
            }

            if ( CompareMem( &ppFile->Name, &guid, sizeof(EFI_GUID) ) == 0 )
            {
                Found = TRUE;
                break;
            }
        }

        if ( Found )
        {
            break;
        }
        else {
            FvNum++;
        }
    }

    (*PeiServices)->FfsFindSectionData( (CONST EFI_PEI_SERVICES **)PeiServices,
                                        EFI_SECTION_RAW,
                                        ppFile,
                                        Buffer );

    if ( Buffer == NULL )
    {
        return EFI_NOT_FOUND;
    }

    Temp  = ((MPDRIVER_LEGHEADER*)(((UINT8* )ppFile )+sizeof(EFI_FFS_FILE_HEADER)));
    *size = Temp->Size;
    
    Status = (*PeiServices)->AllocatePool((CONST EFI_PEI_SERVICES **)PeiServices, *size,  (void **)Buffer );
    if(EFI_ERROR(Status)) return Status;
    
    (*PeiServices)->CopyMem(*Buffer,( ((UINT8* )ppFile )+sizeof(EFI_FFS_FILE_HEADER) ), *size) ;

    return Status;
}

EFI_STATUS MeasureTCGPcClientSpecID(
    IN EFI_PEI_SERVICES **ps, 
    IN PEI_TCG_PPI      *tcg );

//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   MemoryPresentEntry
//
// Description: This function performs TPM MP initialization
//
//
// Input:       IN     EFI_PEI_SERVICES  **PeiServices,
//
// Output:      EFI STATUS
//
// Modified:
//
// Referrals:
//
// Notes:
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI MemoryPresentEntry(
    IN EFI_PEI_SERVICES **PeiServices )
{
    EFI_STATUS                     Status;
    EFI_HOB_GUID_TYPE              *Hob;
    TCG_LOG_HOB                    *TcgLog;
    PEI_TPM_PPI                    *TpmPpi = NULL;         
    PEI_TCG_PPI                    *TcgPpi = NULL;
    EFI_HOB_GUID_TYPE              *ptrBootMode;
    EFI_BOOT_MODE                  BootMode;
    void                           *TcgMPBuffer = NULL;
    UINT16                         Pages      = 0;
    UINT32                         Offset     = 0;
    UINT8                          FuncID     = 1;
    void                           *ParamIN   = NULL;
    void                           *ParamOut  = NULL;
    UINT32                         RetVal     = 0;
    EFI_PEI_FILE_HANDLE            *FfsHeader = NULL;
    EFI_PEI_CPU_IO_PPI             *CpuIo      = (*PeiServices)->CpuIo;
    UINT8                          MPFILEERROR = 0xFA;
    BOOLEAN                        ResetMor = FALSE;
    BOOLEAN			   			   TpmLegBin = FALSE;

    AMI_TCG_PEI_FUNCTION_OVERRIDE_PPI       *MpOverride;

    Status = (*PeiServices)->LocatePpi(
                                (CONST EFI_PEI_SERVICES    **)PeiServices,
                                &AmiMemoryPresentFunctionOverrideGuid,
                                0, NULL,
                                (void **)&MpOverride);

    if(!EFI_ERROR(Status)){
        return (MpOverride->Function(PeiServices));
    }

#if TCG_LEGACY == 1
    TpmLegBin = TRUE;
    MpFileGuid  = gEfiTcgMPDriverGuid;
    Status = Configure_Tpm_Chip( );
    if ( EFI_ERROR( Status ))
    {
         DEBUG((DEBUG_INFO, 
           "Device not configured for legacy IO aborting TPM initialization\n"));
        return Status;
    }
#endif

    Status = (*PeiServices)->GetBootMode( (CONST EFI_PEI_SERVICES **)PeiServices, &BootMode );
    ASSERT_EFI_ERROR(  Status );
    
 #if (StartupCmd_SelfTest_State == 0)
    Status = SendStartupandSelftest(PeiServices,BootMode);
    if(EFI_ERROR(Status))return Status;  //if startup or selftest fails, treat it as a fatal error; return
 #endif

    if((BootMode == BOOT_ON_S3_RESUME) || (BootMode == BOOT_IN_RECOVERY_MODE)){
        return EFI_SUCCESS;
    }

    Status = TcgPeiBuildHobGuid(
        (CONST EFI_PEI_SERVICES    **) PeiServices,
        &gEfiTcgWakeEventDataHobGuid,
        sizeof (BootMode),
        (void **)&ptrBootMode );

    if(EFI_ERROR(Status)){
        return Status;
    }

    ptrBootMode++;
    (*PeiServices)->CopyMem( ptrBootMode, &BootMode, sizeof (BootMode));

    //even if TPM is deactivated still build hob but
    //don't populate it.
    Status = TcgPeiBuildHobGuid(
         (CONST EFI_PEI_SERVICES **)PeiServices,
        &gEfiPeiTcgLogHobGuid,
        sizeof (*TcgLog) + TCG_LOG_MAX_TABLE_SIZE,
        (void **)&Hob );

    if(EFI_ERROR(Status)){
        return Status;
    }

    TcgLog = (TCG_LOG_HOB*)(Hob + 1);
    (*PeiServices)->SetMem( TcgLog, sizeof (*TcgLog), 0 );
    TcgLog->TableMaxSize = TCG_LOG_MAX_TABLE_SIZE;
    
    Status = LocateTcgPpi(PeiServices, &TpmPpi, &TcgPpi);
    if(EFI_ERROR(Status))return EFI_NOT_FOUND;

    if(!IsTcmSupportType())
    {
#if defined(TcgNistSp800_155_SUPPORT) && (TcgNistSp800_155_SUPPORT == 1)
        Status = MeasureTCGPcClientSpecID(PeiServices, TcgPpi);
        ASSERT_EFI_ERROR( Status );
        Status = MeasureNISTManifest(PeiServices, TcgPpi);
        ASSERT_EFI_ERROR( Status );
#endif
        Status = MeasureCRTMVersionFuncPtr( PeiServices );
        ASSERT_EFI_ERROR(  Status );
        if ( EFI_ERROR( Status ))
        {
            DEBUG((DEBUG_ERROR, "Error: Failure %d %a Status = %r\n", __LINE__, __FUNCTION__, Status));
        }
    }

#if defined (AmiTrustedFv_Support) && (AmiTrustedFv_Support == 1)
    Status = PeiServicesNotifyPpi (&mTpm12EndOfPeiSignalPpiNotifyList[0]);
#else
    Status = FindAndMeasureDxeCore(PeiServices);
#endif

    return Status;
}




static AMI_TCG_PLATFORM_PPI_AFTER_MEM  mAmiTcgPlatformPPI = {
    Set_TPMPhysicalPresence,
    MemoryPresentEntry,
    VerifyTcgVariables
};


static EFI_PEI_PPI_DESCRIPTOR mAmiTcgPlatformPPIListAfterMem[] = {
    {
        EFI_PEI_PPI_DESCRIPTOR_PPI
        | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gAmiTcgPlatformPpiAfterMem,
        &mAmiTcgPlatformPPI
    }
};




//**********************************************************************
//<AMI_PHDR_START>
//
// Procedure:   AmiTcgPlatformPEI_EntryAfterMem
//
// Description: Installs AMIplatform PPI for initialization in PEI after 
//              memory is installed
//
// Input:        IN EFI_FFS_FILE_HEADER *FfsHeader,
//               IN EFI_PEI_SERVICES    **PeiServices
//
// Output:      EFI_STATUS
//
// Modified:
//
// Referrals:   
//
// Notes:       
//<AMI_PHDR_END>
//**********************************************************************
EFI_STATUS
EFIAPI AmiTcgPlatformPEI_EntryAfterMem(
    IN EFI_PEI_FILE_HANDLE  FileHandle,
    IN CONST EFI_PEI_SERVICES    **PeiServices 
){
    EFI_STATUS Status;

    Status = (*PeiServices)->InstallPpi( PeiServices, mAmiTcgPlatformPPIListAfterMem );
    return Status;
}


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**     5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093            **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
