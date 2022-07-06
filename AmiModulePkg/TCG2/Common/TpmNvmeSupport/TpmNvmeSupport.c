//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file TpmSmbiosDxe.c
    Install Tpm Device type in Smbios table

**/

//---------------------------------------------------------------------------
#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Token.h>
#include "MdePkg/Include/Protocol/NvmExpressPassthru.h"
#include <Setup.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Guid/AmiTcgGuidIncludes.h>
#include <AmiTcg/TCGMisc.h>
#if defined (CORE_BUILD_NUMBER) && (CORE_BUILD_NUMBER > 0xA) && NVRAM_VERSION > 6
#include <Protocol/VariableLock.h>
#endif
    

void         *Callbackregistration = NULL;
EFI_EVENT     EventRegistration = NULL;

#define TCPA_PPIOP_ENABLE_BLOCK_SID_FUNC     96
#define TCPA_PPIOP_DISABLE_BLOCK_SID_FUNC    97

EFI_STATUS
EFIAPI
CheckNvmeDevicePresence()
{
    EFI_STATUS           Status;
    EFI_HANDLE           *NvmePassThruHandles;
    EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL   *NvmeDevice;
    UINTN                NumOfHandles=0;
    UINTN                Index;
    BOOLEAN              Found = FALSE;
    UINT8                SetupDataBuffer;
    UINTN                SetupVariableSize = sizeof(UINT8);
    UINT32               SetupVariableAttributes;
    BOOLEAN              SidSupport=0;
#if defined (CORE_BUILD_NUMBER) && (CORE_BUILD_NUMBER > 0xA) && NVRAM_VERSION > 6
    EDKII_VARIABLE_LOCK_PROTOCOL    *LockProtocol;
#endif
    
    Status = gBS->LocateHandleBuffer (
                     ByProtocol,
                     &gEfiNvmExpressPassThruProtocolGuid,
                     NULL,
                     &NumOfHandles,
                     &NvmePassThruHandles
                   );
    
    if(EFI_ERROR(Status)){
        if(NumOfHandles){
            gBS->FreePool(NvmePassThruHandles);
        }
        
        Status = gRT->GetVariable (L"TcgNvmeVar",
                                 &gSetupVariableGuid,
                                 &SetupVariableAttributes,
                                 &SetupVariableSize,
                                 &SetupDataBuffer);
        
        DEBUG ((DEBUG_INFO, "GetVariable TcgNvmeVar results = %r\n", Status));
        if(Status == EFI_NOT_FOUND || SetupDataBuffer != 0){
            
            SetupDataBuffer = 0;
            Status = gRT->SetVariable (
                       L"TcgNvmeVar",
                       &gSetupVariableGuid,
                       EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                       SetupVariableSize,
                       &SetupDataBuffer);
            DEBUG ((DEBUG_INFO, "Set TcgNvmeVar results = %r\n", Status));
        }
        
        return EFI_NOT_FOUND;
    }
    
    DEBUG ((DEBUG_INFO, "Locate NvmePassThruhandles results = %r\n", Status));
    
    for (Index = 0; Index < NumOfHandles; Index++) {

       Status = gBS->HandleProtocol (
               NvmePassThruHandles[Index],
               &gEfiNvmExpressPassThruProtocolGuid,
               (VOID **) &NvmeDevice);
               
       if (EFI_ERROR (Status)) {
         DEBUG ((DEBUG_INFO, "gBS->HandleProtocol: Status=%r\n", Status));
         continue;
       }
        
       if (!EFI_ERROR(Status)) {
           Found = TRUE;
           break;
       }
   }
    
   DEBUG((DEBUG_INFO," \n CheckNvmeDevicePresence Found = %x  \n ", Found));
    
   Status = gRT->GetVariable (L"TcgNvmeVar",
                            &gSetupVariableGuid,
                            &SetupVariableAttributes,
                            &SetupVariableSize,
                            &SetupDataBuffer);

   DEBUG ((DEBUG_INFO, "GetVariable2 TcgNvmeVar results = %r\n", Status));
   if(EFI_ERROR(Status)){
       if(NumOfHandles){
           gBS->FreePool(NvmePassThruHandles);
       }
       return Status;
   }
   
   SetupDataBuffer = Found;
   
   Status = gRT->SetVariable (
                L"TcgNvmeVar",
                &gSetupVariableGuid,
                SetupVariableAttributes,
                SetupVariableSize,
                &SetupDataBuffer);
   
   DEBUG ((DEBUG_INFO, "Set NvmeDeviceFound Status = %r\n", Status));
   
#if defined (CORE_BUILD_NUMBER) && (CORE_BUILD_NUMBER > 0xA) && NVRAM_VERSION > 6
   Status =  gBS->LocateProtocol(&gEdkiiVariableLockProtocolGuid, NULL, (void **)&LockProtocol);
   if(!EFI_ERROR(Status))
   {
       LockProtocol->RequestToLock(LockProtocol, L"TcgNvmeVar", &gSetupVariableGuid);
   }
#endif

   if(NumOfHandles){
       gBS->FreePool(NvmePassThruHandles);
   }
   return Status;
}


VOID
EFIAPI
OnNvmExpressPassThruProtocolEvent(
    IN EFI_EVENT ev,
    IN VOID      *ctx )
{
    EFI_STATUS    Status;
    
    DEBUG((DEBUG_INFO," \n TpmNvmeSupportDxeEntry  \n "));
    Status = CheckNvmeDevicePresence();
    gBS->CloseEvent(ev);
}


/**
  Entry point for the driver.

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
TpmNvmeSupportDxeEntry (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS        Status;
  AMI_PPI_NV_VAR    Ppivar;
  UINT32 Attribs2 = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS 
                    | EFI_VARIABLE_RUNTIME_ACCESS;
  UINTN PpivarSize = sizeof (AMI_PPI_NV_VAR);
  SETUP_DATA          SetupDataBuffer;
  UINTN               SetupVariableSize = sizeof(SETUP_DATA);
  UINT32              SetupVariableAttributes;
  
  DEBUG((DEBUG_INFO," \n TpmNvmeSupportDxeEntry  \n "));
  
  Status = gRT->GetVariable ( L"Setup",
                              &gSetupVariableGuid,
                              &SetupVariableAttributes,
                              &SetupVariableSize,
                              &SetupDataBuffer);
  
  if(!EFI_ERROR(Status)){
      
      if(SetupDataBuffer.BlockSidSetup){
          SetupDataBuffer.BlockSidSetup = 0;
               
          Status = gRT->SetVariable (
                        L"Setup",
                        &gSetupVariableGuid,
                        SetupVariableAttributes,
                        SetupVariableSize,
                        &SetupDataBuffer);
           
          Ppivar.RQST    = TCPA_PPIOP_DISABLE_BLOCK_SID_FUNC;
          Ppivar.RCNT    = 0;
          Ppivar.ERROR   = 0;
          Ppivar.Flag    = 0;
          Ppivar.AmiMisc = 0;
           
          Status = gRT->SetVariable(L"AMITCGPPIVAR", &AmitcgefiOsVariableGuid,
                                                                Attribs2, PpivarSize, &Ppivar);
      }
     
  }
  
  Status = CheckNvmeDevicePresence();
  
  if(EFI_ERROR(Status)){
      //assume protocol not yet published so set callback and 
      //return
      Status = gBS->CreateEvent( EVT_NOTIFY_SIGNAL,
                                 TPL_CALLBACK, OnNvmExpressPassThruProtocolEvent, NULL, &Callbackregistration);
      
      if(!EFI_ERROR(Status)){
         ASSERT( !EFI_ERROR( Status ));
         Status = gBS->RegisterProtocolNotify(&gEfiNvmExpressPassThruProtocolGuid, Callbackregistration, &EventRegistration);
      }
  }
  
  DEBUG((DEBUG_INFO," \n CheckNvmeDevicePresence results = %r  \n ", Status));
  return Status;
}
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
