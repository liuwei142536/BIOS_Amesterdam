/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2012 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    eMCA.c

Abstract:
    This is an implementation of the eMCA driver 

-----------------------------------------------------------------------------*/

#include <Library/emcalib.h>
#include <Library/emcaplatformhookslib.h>
#include <Library/SynchronizationLib.h>

VOID   *mL1DirBkp = NULL;
VOID   *mElogDirBkp = NULL;

//EFI_SMM_BASE2_PROTOCOL    *mSmmBase;
//EFI_SMM_SYSTEM_TABLE2     *mSmst;

/**
  Get memory size in bytes required to store the L1 Directory.

  L1 Dir Size = Number of L1 Dir entries * 8 + 0x40

  Number of L1 Dir Entries = SOCKET_COUNT * ( 2 ** APICID_BITS_PER_SOCKET) * MCBANKS_PER_LP where: 
  
  SOCKET_COUNT is  the maximum number  of  processor  sockets in  the platform,
  APICID_BITS_PER_SOCKET is the number of APIC ID bits used up by each socket  
  MCBANKS_PER_LP = MCBANK_CORE + MCBANKS_UNCORE = MCG_CAP.Count
  
  @return required sized in bytes 

**/
STATIC
UINT64
GetL1DirSize (
  VOID
  )

{
  return (GetNumL1DirEntries() * L1_DIR_PTR_LEN) + sizeof(EMCA_L1_HEADER); 
}

/**
  Get memory size in bytes required to store the Elog Directory(L2).

  NumElogEntries * 0x400 
  
  @return required sized in bytes 

**/
STATIC
UINT64
GetElogDirSize (
  VOID
  )

{
  return (GetNumElogEntries() * ELOG_ENTRY_LEN);  
}

STATIC
EFI_STATUS
InitL1Dir(
  IN VOID                         *L1Dir
  )
{
  EMCA_L1_HEADER *L1DirHdr;
  UINT32 i;
  UINT16  McScope;
  UINT8 *NxtElog;
  EFI_STATUS Status = EFI_SUCCESS;
  
  L1DirHdr = (EMCA_L1_HEADER *)L1Dir;
  NxtElog = (UINT8 *) L1DirHdr->ElogDirBase;

  for(i = 0; i < L1DirHdr->NumL1EntryLp; i++) {
    if(isEMCALogSupported(i)) {
      McScope = GetMcBankScope(i);
      switch(McScope) {
        
        case MC_SCOPE_TD:
          Status = InitL1PtrForThreads(L1DirHdr, i, &NxtElog);
          if(Status != EFI_SUCCESS)
            return Status;
          break;
        case MC_SCOPE_CORE:
          Status = InitL1PtrForCores(L1DirHdr, i, &NxtElog);
          if(Status != EFI_SUCCESS)
            return Status;
          break;
        case MC_SCOPE_PKG:
          Status = InitL1PtrForSockets(L1DirHdr, i, &NxtElog);
          if(Status != EFI_SUCCESS)
            return Status;
          break;
        default:
          ASSERT(FALSE); //should not enter here
          break;
      }         
    }
  }
  return Status;
}

STATIC
EFI_STATUS
CreateL1DirHdr(
  IN OUT EMCA_L1_HEADER *L1DirHdr,
  IN VOID *ElogDirBase,
  IN UINT64 L1DirSize,
  IN UINT64 ElogSize
  )
{  
  if(L1DirHdr == NULL || ElogDirBase == NULL) {
    return EFI_INVALID_PARAMETER;
  }  
  
  L1DirHdr->Version = EMCA_L1_VERSION;
  L1DirHdr->HdrLen = sizeof(EMCA_L1_HEADER);
  L1DirHdr->L1DirLen = L1DirSize;
  L1DirHdr->ElogDirBase = (UINT64)ElogDirBase;
  L1DirHdr->ElogDirLen = ElogSize; 
  L1DirHdr->NumL1EntryLp = (UINT32)GetNumMcBanksPerLp();

  return EFI_SUCCESS;  
}

/**
  Allocates L1 memory and the pointer is stored in the parameter.
  
  @param  Buffer                A pointer to a pointer to the allocated buffer if the call succeeds;
                                undefined otherwise.

  @retval EFI_SUCCESS           The requested number of bytes was allocated.
  @retval EFI_OUT_OF_RESOURCES  The pool requested could not be allocated.
  @retval EFI_INVALID_PARAMETER PoolType was invalid.

**/
EFI_STATUS
AllocateL1Dir(
  OUT VOID                         **Buffer,
  OUT UINT64                        *L1Size
  )
{
  //UseAllocatepages, below 4GB, Memory reserved 
  UINTN Pages = 0;
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS MaxAddress;

  if(Buffer == NULL || L1Size == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  *Buffer = NULL;
  *L1Size = GetL1DirSize(); 
  Pages = EFI_SIZE_TO_PAGES(*L1Size);
  MaxAddress = 0xffffffff; //less or equal address for the space allocated. It must be under 4GB

  Status = gBS->AllocatePages(
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  Pages,
                  &MaxAddress          
               );

  if(Status == EFI_SUCCESS) {
    *Buffer = (VOID *)MaxAddress;
    gBS->SetMem(*Buffer,EFI_PAGES_TO_SIZE(Pages), 0);
  } 

  return Status;
}

/**
  Allocates Elog(L2) memory and the pointer is stored in the parameter.
  
  @param  Buffer                A pointer to a pointer to the allocated buffer if the call succeeds;
                                undefined otherwise.

  @retval EFI_SUCCESS           The buffer was allocated.
  @retval EFI_OUT_OF_RESOURCES  The buffer could not be allocated.
  @retval EFI_INVALID_PARAMETER

**/
STATIC
EFI_STATUS 
AllocateElogDir(
  OUT VOID                         **Buffer,
  OUT UINT64                            *ElogSize
  )
{
  UINTN Pages = 0;
  EFI_STATUS Status;
  EFI_PHYSICAL_ADDRESS MaxAddress;

  if(Buffer == NULL || ElogSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  
  *Buffer = NULL;
  *ElogSize = GetElogDirSize();

  Pages = EFI_SIZE_TO_PAGES(*ElogSize);
  MaxAddress = 0xffffffff;

  Status = gBS->AllocatePages(
                  AllocateMaxAddress,
                  EfiReservedMemoryType,
                  Pages,
                  &MaxAddress
                );

  if(Status == EFI_SUCCESS) {
    *Buffer = (VOID *) MaxAddress;
    gBS->SetMem(*Buffer,EFI_PAGES_TO_SIZE(Pages), 0); 
  }

  return Status;
}  


/**
  Initialize directories

  a.  Allocate and zero out memory
  b.  Setup L1 directory  header  and clear all the L1 directory entries
  c.  Setup valid L1 entries 
  d.  Bring all the threads to SMM and update Enhanced_MCA_LOG(63)
  
  @param  L1Dir                 Pointer to Pointer to save the L1 Directory Memory Location 
  @param  ElogDir               Pointer to Pointer to save Elog(L2) Directory Memory Location ;
  undefined otherwise.

  @retval EFI_SUCCESS          
  @retval EFI_INVALID_PARAMETER 

**/
STATIC
EFI_STATUS
InitLogDirs(
  IN OUT VOID                         **L1Dir,
  IN OUT VOID                         **ElogDir
  )
{
  EFI_STATUS Status = EFI_SUCCESS; 
  UINT64 L1DirSize;
  UINT64 ElogSize;  

  Status = AllocateL1Dir(L1Dir,&L1DirSize);
  if(Status != EFI_SUCCESS) {
    return Status;
  }
  Status = AllocateElogDir(ElogDir,&ElogSize);
  if(Status != EFI_SUCCESS) {
    return Status;
  }
  Status = CreateL1DirHdr((EMCA_L1_HEADER *)*L1Dir,*ElogDir,L1DirSize,ElogSize);
  if(Status != EFI_SUCCESS) {
    return Status;
  }
  Status = InitL1Dir(*L1Dir);
  if(Status != EFI_SUCCESS) {
    return Status;
  }

  mL1DirBkp = *L1Dir;
  mElogDirBkp = *ElogDir;
  
  return Status;
}


EFI_STATUS
InitEMCA( 
IN OUT VOID                         **L1Dir,
IN OUT VOID                         **ElogDir
)
{
  BOOLEAN EmcaLiteCap;
  EFI_STATUS Status = EFI_SUCCESS;
  
  Status = GetEMCALogProcCap(&EmcaLiteCap);
  ASSERT_EFI_ERROR(Status);
  
  if(EmcaLiteCap) {
    Status = InitLogDirs(L1Dir,ElogDir);
    ASSERT_EFI_ERROR(Status);
    //Status = RegisterEmcaOSOptinSmi();
    //ASSERT_EFI_ERROR(Status);
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return Status;  
}

EFI_STATUS
GetL1DirHdr(
OUT VOID **Hdr
)
{
  if(Hdr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Hdr = mL1DirBkp;
  return EFI_SUCCESS;   
}

BOOLEAN
isEmcaOptInFlagSet(
VOID
) 
{
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN    optin;
  EMCA_L1_HEADER *L1Hdr = NULL;

  Status = GetL1DirHdr(&L1Hdr);
  if(Status != EFI_SUCCESS) {
    optin = FALSE;
  } else {
    optin = ((L1Hdr->Flags & OS_VMM_OPT_IN_FLAG) != 0);
  }

  return optin;  
}


EFI_STATUS
LogEmcaRecord(
  IN UINT32                                 McBankNum,
  IN CONST EMCA_MC_SIGNATURE_ERR_RECORD     *McSig,
  IN CONST GENERIC_ERROR_STATUS             *Log
  )  
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    GENERIC_ERROR_STATUS       *ErrStsBlk = NULL;
    EMCA_L1_HEADER             *L1Hdr = NULL;
    UINT8                      *L1Base = NULL;
    UINT8                      *RawData = NULL;
    UINT32                     *BlockStatus = NULL;
    UINT64                       L1Offset;
    UINT32                        BlkLen;
    UINT32 CompareValue;
    UINT32 ExchangeValue;

    if(Log == NULL || McSig == NULL) {
      return EFI_INVALID_PARAMETER;
    }

    Status = GetL1DirHdr(&L1Hdr);
    ASSERT_EFI_ERROR(Status);

	if(L1Hdr == NULL) 
      return EFI_INVALID_PARAMETER;

    if(McBankNum >= L1Hdr->NumL1EntryLp) {
      return EFI_INVALID_PARAMETER;
    }

    L1Base = EMCA_GET_L1_BASE(L1Hdr);
    L1Offset = EMCA_GET_L1_OFFSET(L1Hdr, McSig->ApicId, McBankNum);
    ErrStsBlk = EMCA_GET_ELOG_ENTRY(L1Base, L1Offset);

    //Check for Valid bit in pointer and mask 63:52 bits which are not part of the address. 
    if(((UINT64)ErrStsBlk & BIT63) != 0) { 
      ErrStsBlk = (GENERIC_ERROR_STATUS *)((UINT64)ErrStsBlk & 0x000FFFFFFFFFFFFF);
    } else {
      return EFI_OUT_OF_RESOURCES;
    }

    BlkLen = sizeof(GENERIC_ERROR_STATUS) + Log->ErrDataSize;
    BlockStatus = (UINT32*) &(ErrStsBlk->BlockStatus);

    CompareValue = 0;
    ExchangeValue = *((UINT32*) &(Log->BlockStatus));
    
    if(InterlockedCompareExchange32(BlockStatus, CompareValue, ExchangeValue) == 0) {
      //If blockstatus is 0 then the record is either invalid or the OS has consumed it, we can just write the record to memory 
      CopyMem(ErrStsBlk, Log, BlkLen);
      ErrStsBlk->RawDataOffset = BlkLen;
      ErrStsBlk->RawDataSize = sizeof(EMCA_MC_SIGNATURE_ERR_RECORD);
      RawData = ((UINT8 *)ErrStsBlk) + ErrStsBlk->RawDataOffset;
      CopyMem(RawData, McSig, ErrStsBlk->RawDataSize);
        
    } else {
      //There is already a log in memory
      //Use MCA Overwrite Rules to either overwrite the existing record or leave it alone. 
      if( (ErrStsBlk->Severity == GENERIC_ERROR_CORRECTED) || 
          (ErrStsBlk->Severity == GENERIC_ERROR_RECOVERABLE && Log->Severity == GENERIC_ERROR_FATAL) 
         ) {
         //Overwrite elog
        CopyMem(ErrStsBlk, Log, BlkLen);
        ErrStsBlk->RawDataOffset = BlkLen;
        ErrStsBlk->RawDataSize = sizeof(EMCA_MC_SIGNATURE_ERR_RECORD);
        RawData = ((UINT8 *)ErrStsBlk) + ErrStsBlk->RawDataOffset;
        CopyMem(RawData, McSig, ErrStsBlk->RawDataSize);
      } else {
        if(McSig->Signature.McSts & BIT62) {
            //Over bit is set, update MC Signature
            RawData = ((UINT8 *)ErrStsBlk) + ErrStsBlk->RawDataOffset;
            CopyMem(RawData, McSig, ErrStsBlk->RawDataSize);
        }
      }
    }
    
  return Status; 
}


