//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file PreserveCACert.c
    This file contains code to extract the certificate from BIOS and preserve in a nvram variable.
**/
#include <AmiDxeLib.h>
#include <Guid/TlsAuthentication.h>
#include <Guid/ImageAuthentication.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Token.h>
#include "HttpsCertsElink.h"

extern  EFI_BOOT_SERVICES    *gBS;
extern  EFI_RUNTIME_SERVICES *gRT;
extern  EFI_GUID gEfiFirmwareVolume2ProtocolGuid;
extern  EFI_GUID gEfiTlsCaCertificateGuid;

#define HTTPS_NULL_GUID { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
#define TLS_AUTH_CONFIG_VAR_BASE_ATTR  (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS)

typedef struct
{
    EFI_GUID CertificateFileGuid;
} HTTPSCERTIFICATE_LIST;

/**
  Preserve certificate in Nvram variable.

  @param[in]  VariableName        The variable name of the database.
  @param[in]  CertGuid            A unique identifier for the certificate.
  @param[in]  X509DataSize        Total size of certificate.
  @param[in]  X509Data            Pointer to certificate buffer.

  @retval EFI_SUCCESS             The certificate is set to nvram successfully.
  @retval EFI_OUT_OF_RESOURCES    Required system resources could not be allocated.
**/
EFI_STATUS
EnrollCertToVariable (
  IN CHAR16                         *VariableName,
  IN EFI_GUID                       CertGuid,
  IN UINTN                          X509DataSize,
  IN VOID                           *X509Data
  )
{
  EFI_STATUS                        Status;
  EFI_SIGNATURE_LIST                *CACert;
  EFI_SIGNATURE_DATA                *CACertData;
  VOID                              *Data;
  UINTN                             DataSize;
  UINTN                             SigDataSize;
  UINT32                            Attr;

  SigDataSize   = 0;
  DataSize      = 0;
  CACert        = NULL;
  CACertData    = NULL;
  Data          = NULL;

  SigDataSize = sizeof(EFI_SIGNATURE_LIST) + sizeof(EFI_SIGNATURE_DATA) - 1 + X509DataSize;

  Data = AllocateZeroPool (SigDataSize);
  if (Data == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Fill Certificate Database parameters.
  //
  CACert = (EFI_SIGNATURE_LIST*) Data;
  CACert->SignatureListSize   = (UINT32) SigDataSize;
  CACert->SignatureHeaderSize = 0;
  CACert->SignatureSize = (UINT32) (sizeof(EFI_SIGNATURE_DATA) - 1 + X509DataSize);
  CopyGuid (&CACert->SignatureType, &gEfiCertX509Guid);

  CACertData = (EFI_SIGNATURE_DATA*) ((UINT8* ) CACert + sizeof (EFI_SIGNATURE_LIST));
  CopyGuid (&CACertData->SignatureOwner, &CertGuid);
  CopyMem ((UINT8* ) (CACertData->SignatureData), X509Data, X509DataSize);

  //
  // Check if signature database entry has been already existed.
  // If true, use EFI_VARIABLE_APPEND_WRITE attribute to append the
  // new signature data to original variable
  //
  Attr = TLS_AUTH_CONFIG_VAR_BASE_ATTR;

  Status = gRT->GetVariable(
                  VariableName,
                  &gEfiTlsCaCertificateGuid,
                  NULL,
                  &DataSize,
                  NULL
                  );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    Attr |= EFI_VARIABLE_APPEND_WRITE;
  }

  Status = gRT->SetVariable(
                  VariableName,
                  &gEfiTlsCaCertificateGuid,
                  Attr,
                  SigDataSize,
                  Data
                  );

  if (Data != NULL) {
    FreePool (Data);
  }

  return Status;
}

/**
  Function to check whether certificate is already preserved in nvram.

  @param[in]  SearchGuid          GUID to be searched in the Nvram buffer

  @retval TRUE              The certificate is preserved in Nvram.
  @retval FALSE             The certificate is not preserved in Nvram.
**/
BOOLEAN IsCertSavedInNvRam(EFI_GUID *SearchGuid)
{
    EFI_STATUS     Status = EFI_SUCCESS;
    VOID           *VarBuffer = NULL;
    UINTN          VarBufSize = 0;
    EFI_SIGNATURE_LIST     *CACert;
    EFI_SIGNATURE_DATA     *CACertData;
    UINT32           Offset;
    
	/* Obtain the Variable and parse the variable buffer for the presence of GUID of the certificate 
	 * and return the status*/
    Status = gRT->GetVariable( EFI_TLS_CA_CERTIFICATE_VARIABLE, &gEfiTlsCaCertificateGuid, NULL, &VarBufSize, VarBuffer );       
    if (EFI_NOT_FOUND == Status)
    {
        return FALSE;
    }
    else if (EFI_BUFFER_TOO_SMALL == Status) 
    {
        VarBuffer = AllocateZeroPool(VarBufSize);
        gRT->GetVariable( EFI_TLS_CA_CERTIFICATE_VARIABLE, &gEfiTlsCaCertificateGuid, NULL, &VarBufSize, VarBuffer );
    }

    for(Offset = 0; Offset < VarBufSize; )
    {
        CACert = (EFI_SIGNATURE_LIST*) ( (UINT8*)VarBuffer + Offset);
        CACertData = (EFI_SIGNATURE_DATA*) ((UINT8* ) CACert + sizeof (EFI_SIGNATURE_LIST));        
        if(guidcmp(SearchGuid, &CACertData->SignatureOwner) == 0)
        {
            if(VarBuffer)
                FreePool(VarBuffer);                
            return TRUE;
        }
        
        Offset += sizeof(EFI_SIGNATURE_LIST) + CACert->SignatureSize;
    }
    
    if(VarBuffer)
    {
        FreePool(VarBuffer);
    }    
    
    return FALSE;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS PreserveCACertEntry(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{        
     EFI_STATUS     Status = EFI_SUCCESS;
     UINTN          NoOfHandles = 0;
     EFI_HANDLE     *Handles = (EFI_HANDLE  *)NULL;
     UINT32         Index = 0;
     UINT32         Authentication = 0;
     VOID           *CertBuffer = NULL;
     int            Count = 0;
     EFI_GUID       NullGuid = HTTPS_NULL_GUID;
     EFI_FIRMWARE_VOLUME2_PROTOCOL *FV2P;
     HTTPSCERTIFICATE_LIST CacertGuidList[] = {HTTPS_ROOT_CERTIFICATE_LIST {HTTPS_NULL_GUID}};
     int            Certificatecount = sizeof(CacertGuidList)/sizeof(HTTPSCERTIFICATE_LIST);
     UINTN          CertSize = 0;
     UINT32         Attr = TLS_AUTH_CONFIG_VAR_BASE_ATTR;
     UINT8          Data;
          
     // Skip the process from second boot onwards
     Status  = gRT->GetVariable ( L"TlsCertSaved", &gEfiTlsCaCertificateGuid, NULL, &CertSize, NULL );
     if (EFI_NOT_FOUND != Status)
     {
         return EFI_SUCCESS;
     }
    
     Status = gBS->LocateHandleBuffer ( ByProtocol, &gEfiFirmwareVolume2ProtocolGuid, NULL, &NoOfHandles, &Handles );
     DEBUG((DEBUG_ERROR, "\nLocateHandleBuffer Status = %r, NoOfHandles = %d", Status, NoOfHandles));
     if ( EFI_ERROR(Status) )
     {
         return Status;
     }

     for ( Index = 0; Index  < NoOfHandles; Index ++ )
     {
         Status = gBS->HandleProtocol ( Handles[ Index ], & gEfiFirmwareVolume2ProtocolGuid, ( VOID * )&FV2P );
         DEBUG((DEBUG_ERROR, "\n HandleProtocol Status = %r ", Status));
         if ( EFI_ERROR(Status) )
         {
             continue;
         }
         
         for(Count = 0, CertSize = 0; Count < Certificatecount; Count++)
         {
             if(guidcmp(&(CacertGuidList[Count].CertificateFileGuid), &NullGuid) == 0)
             {
                 break;
             }
             
             //Check if the certificate is already saved in Nvram. If present, donot save again
             if(IsCertSavedInNvRam(&CacertGuidList[Count].CertificateFileGuid))
             {
			 	 Data = 1;
                 gRT->SetVariable( L"TlsCertSaved", &gEfiTlsCaCertificateGuid, Attr, sizeof(UINT8), &Data );
                 continue;
             }
             
             // Read the cerificate from ROM and save to variable
             Status = FV2P->ReadSection ( FV2P, &(CacertGuidList[Count].CertificateFileGuid), EFI_SECTION_RAW, 0, ( VOID ** )&CertBuffer, &CertSize, &Authentication );
             DEBUG((DEBUG_ERROR, "\n ReadSection Status = %r ", Status));                 
             if ( EFI_ERROR(Status) )
             {
                 continue;
             }
             else
             {
                 if (CertBuffer != NULL) 
                 {
                     Status = EnrollCertToVariable ( EFI_TLS_CA_CERTIFICATE_VARIABLE, CacertGuidList[Count].CertificateFileGuid, CertSize, CertBuffer );                        
                     if ( EFI_ERROR(Status) )
                     {
						 FreePool (CertBuffer);
                         continue;
                     }
                     CertSize = 0;
                     Data = 1;
                     gRT->SetVariable( L"TlsCertSaved", &gEfiTlsCaCertificateGuid, Attr, sizeof(UINT8), &Data );
                     FreePool (CertBuffer);
                 }                                            
             }
         }
     }
	if(Handles != NULL)
	{
		FreePool(Handles);
	}
	
     return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
