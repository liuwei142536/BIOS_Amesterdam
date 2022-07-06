//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
#include "includes.h"
#include <Library/PeiServicesLib.h>
#include <CryptLib.h>

//------------------------------------------------------------------------
// Callback PPI
//------------------------------------------------------------------------
EFI_STATUS EFIAPI AmiCryptLibAfterMem(
    IN CONST EFI_PEI_SERVICES **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *NullPpi);

static EFI_PEI_NOTIFY_DESCRIPTOR MemoryAvailableNotify[] =
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
        &gEfiPeiMemoryDiscoveredPpiGuid,
        AmiCryptLibAfterMem
    }
};
//------------------------------------------------------------------------

/**
  Return pointer to CRMm Descriptor table

  @param[in]  none
  
  @retval     pointer to CRMm Descriptor table
**/
VOID * GetCRmmPtr()
{
    return (VOID*)(UINTN)PcdGet32(AmiPcdCpHeapAddr);
}

/**
  This function allocates memory for PEI Crypto Memory manager,
 
  @param  PeiServices   A pointer to the PEI Services.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS 
EFIAPI
AmiCryptLibAfterMem(
    IN CONST EFI_PEI_SERVICES    **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID                      *NotifyPpi
) {
    EFI_STATUS  Status;

    EFI_PHYSICAL_ADDRESS HeapAddress = 0;
    UINTN HeapSize = 0;

    DEBUG((AmiCryptoPkg_DEBUG_LEVEL,"Ami PeiCryptLib Alloc Heap\n"));

    ///////////////////////////////////////////////////////////////////////////////
    //
    // Init Aux Memory Manager
    // Pre-allocates runtime space for possible cryptographic operations
    //
    ///////////////////////////////////////////////////////////////////////////////
    HeapSize = PcdGet32(AmiPcdCpMaxHeapSize);
    Status = (*PeiServices)->AllocatePages(PeiServices, EfiBootServicesData, EFI_SIZE_TO_PAGES(HeapSize), &HeapAddress);

    DEBUG ((AmiCryptoPkg_DEBUG_LEVEL, "PEI Heap alloc %r (addr=%X, size=%x)\n", Status, (UINT32)HeapAddress, HeapSize));
    if(!EFI_ERROR(Status) && HeapAddress != 0 ) {
        PcdSet32(AmiPcdCpHeapAddr, (UINT32)HeapAddress);
        InitCRmm( (void*)(UINTN)HeapAddress, HeapSize);
    }

    return EFI_SUCCESS;
}

/**
  This constructor function allocates memory for PEI Crypto Memory manager
 
  @param  FileHandle    The firmware allocated handle for the EFI image.
  @param  PeiServices   A pointer to the PEI Services.
  
  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
AmiPeiCryptLibConstructor (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
    VOID  *Ppi;
    DEBUG((AmiCryptoPkg_DEBUG_LEVEL,"Ami PeiCryptLib Constructor\n"));
    // re=alloc Heap after System RAM is initialized
    if(EFI_ERROR((*PeiServices)->LocatePpi(PeiServices, &gEfiPeiMemoryDiscoveredPpiGuid, 0, NULL, &Ppi)))
        (*PeiServices)->NotifyPpi(PeiServices, MemoryAvailableNotify);

    return AmiCryptLibAfterMem(PeiServices, NULL, NULL);
}
