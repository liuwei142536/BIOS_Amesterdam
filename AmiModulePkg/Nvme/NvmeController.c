//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file NvmeController.c
    Provides Access to Nvme Controller

**/

//---------------------------------------------------------------------------

#include "NvmeIncludes.h"
#include "NvmeBus.h"

//---------------------------------------------------------------------------


/**
    Creates Submission and Completion Queue

        
    @param NVMeController 
    @param NvmeCmdWrapper 
    @param QueueNumber 
    @param QueueSize 

    @retval EFI_STATUS

    @note  Can be called recursively  

**/

EFI_STATUS
CreateAdditionalSubmissionCompletionQueue ( 
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper,
    IN  UINT16                          QueueNumber,
    IN  UINT32                          QueueSize
)
{
    EFI_STATUS              Status=EFI_SUCCESS;
    COMPLETION_QUEUE_ENTRY  CompletionData;
    UINTN                   AllocatePageSize;
    UINT64                  QueueAddress = 0;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    
    // Clear memory
    SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
    
    // Allocate memory only if the pointer is NULL
    if (NvmeController->Queue1CompletionUnAligned == 0) {
        AllocatePageSize = NvmeController->MemoryPageSize + QueueSize * sizeof(COMPLETION_QUEUE_ENTRY);
    
        Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                                 AllocateAnyPages,
                                                 EfiRuntimeServicesData,
                                                 EFI_SIZE_TO_PAGES(AllocatePageSize),
                                                 (VOID **)&(QueueAddress),
                                                 EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                                 );
            
        if (EFI_ERROR(Status)) {
            return Status;
        }
    
        NvmeController->Queue1CompletionUnAligned = QueueAddress;
        NvmeController->Queue1CompletionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
        NvmeController->Queue1CompletionQueue = (QueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                                NvmeController->MemoryPageSize;
    
        NvmeController->Queue1CompletionQueueSize = QueueSize;
        
        Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                      EfiPciIoOperationBusMasterCommonBuffer,
                                      (VOID *)NvmeController->Queue1CompletionQueue,
                                      &AllocatePageSize,
                                      &NvmeController->Queue1CompletionQueueMappedAddr,
                                      &NvmeController->Queue1CompletionQueueUnMap
                                      );
        
        if (EFI_ERROR(Status)) {
            return Status;
        }
    }

    SetMem((VOID *)NvmeController->Queue1CompletionQueue, \
                        NvmeController->Queue1CompletionQueueSize * sizeof (COMPLETION_QUEUE_ENTRY), 0);

    // If FATAL error occurred for CREATE_IO_COMPLETION_QUEUE nvme command, Return from here.
    if( (NvmeController->DeviceState == Fatal_Recovery_state) && \
        NvmeController->CreateIoCompletionQueueInProgress ) {
        goto CreateAdditionalSubmissionCompletionQueue_Exit;
    }

    NvmeController->CreateIoCompletionQueueInProgress = TRUE;

    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = CREATE_IO_COMPLETION_QUEUE;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = 0;
            
    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64)NvmeController->Queue1CompletionQueueMappedAddr;
    NvmeCmdWrapper->NvmCmd.PRP2 = 0;
    NvmeCmdWrapper->NvmCmd.CDW10 = ((QueueSize - 1) << 16 )+ QueueNumber;
    NvmeCmdWrapper->NvmCmd.CDW11 = 1;           // Contiguous
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;           // Cmd issued in Queue0
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);

    NvmeController->CreateIoCompletionQueueInProgress = FALSE;

    if (EFI_ERROR(Status)) {
        goto CreateAdditionalSubmissionCompletionQueue_Exit;
    }

    // Allocate memory only if the pointer is NULL
    if (NvmeController->Queue1SubmissionUnAligned == 0) { 
        
        AllocatePageSize = NvmeController->MemoryPageSize + 
                            QueueSize * sizeof(NVME_ADMIN_COMMAND);  
        
        Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                             AllocateAnyPages,
                                             EfiRuntimeServicesData,
                                             EFI_SIZE_TO_PAGES(AllocatePageSize),
                                             (VOID **)&(QueueAddress),
                                             EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                             );
                    
        if (EFI_ERROR(Status)) {
            return Status;
        }
    
        NvmeController->Queue1SubmissionUnAligned = QueueAddress;
        NvmeController->Queue1SubmissionUnAlignedSize = EFI_SIZE_TO_PAGES(AllocatePageSize);
        NvmeController->Queue1SubmissionQueue = (QueueAddress & ~((UINT64)NvmeController->MemoryPageSize - 1)) + 
                                                NvmeController->MemoryPageSize;
    
        NvmeController->Queue1SubmissionQueueSize = QueueSize;
    
        Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                         EfiPciIoOperationBusMasterCommonBuffer,
                                         (VOID *)NvmeController->Queue1SubmissionQueue,
                                         &AllocatePageSize,
                                         &NvmeController->Queue1SubmissionQueueMappedAddr,
                                         &NvmeController->Queue1SubmissionQueueUnMap
                                         );
                
        if (EFI_ERROR(Status)) {
            return Status;
        }
                
    }

    SetMem((VOID *)NvmeController->Queue1SubmissionQueue, \
                            NvmeController->Queue1SubmissionQueueSize * sizeof(NVME_ADMIN_COMMAND), 0);

    // If Nvme FATAL error occurred for CREATE_IO_SUBMISSION_QUEUE nvme command, Return from here.
    if( (NvmeController->DeviceState == Fatal_Recovery_state) && \
        NvmeController->CreateIoSubmissionQueueInProgress ) {
        goto CreateAdditionalSubmissionCompletionQueue_Exit;
    }

    NvmeController->CreateIoSubmissionQueueInProgress = TRUE;

    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = CREATE_IO_SUBMISSION_QUEUE;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = 0;
            
    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64)NvmeController->Queue1SubmissionQueueMappedAddr;
    NvmeCmdWrapper->NvmCmd.PRP2 = 0;
    NvmeCmdWrapper->NvmCmd.CDW10 = ((QueueSize - 1) << 16 )+ QueueNumber;
    NvmeCmdWrapper->NvmCmd.CDW11 = (QueueNumber << 16) + 1;           // Contiguous
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;           // Cmd issued to admin queue
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
    
    NvmeController->CreateIoSubmissionQueueInProgress = FALSE;

    CreateAdditionalSubmissionCompletionQueue_Exit:

    NvmeDxeSmmDataArea->Queue1PhaseTag = FALSE;
    
    NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr = 0;
    
    NvmeDxeSmmDataArea->Queue1CompletionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->Queue1CompletionQueueHeadPtr = 0;
    
    return Status;
   
}

/**@internal
    Routine to get the Active NameSpace

    @param NvmeController 
    @param NamespaceId 

    @retval Returns ACTIVE_NAMESPACE_DATA pointer for the NamespaceId
	@endinternal
**/

ACTIVE_NAMESPACE_DATA*
GetActiveNameSpace (
    AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    UINT32                         NamespaceId
)
{
    LIST_ENTRY            *LinkData;
    ACTIVE_NAMESPACE_DATA *ActiveNameSpace = NULL;
    
    if(!NvmeController) {
        return NULL;
    }

    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {

        ActiveNameSpace = BASE_CR(LinkData, ACTIVE_NAMESPACE_DATA, Link);
        
        // If NamespaceId is 0xFFFFFFFF, then return the first valid ActiveNameSpace
        if( NamespaceId == 0xFFFFFFFF ) {
            return ActiveNameSpace;
        }

        if(ActiveNameSpace->ActiveNameSpaceID == NamespaceId) {
            return ActiveNameSpace;
        }
    }
    return NULL;
}
/**
    Programs PRP2 with the list of page address for data transfer

    @param This 

    @retval EFI_STATUS

**/

EFI_STATUS
ProgramPRP2List (
    IN  UINT64  *PRP2List, 
    IN  UINT32  PageSize, 
    IN  UINTN   BufferAddress,
    IN  UINTN   BufferSize,
    IN  UINTN   *PRP2TransferSize
)
{
    UINTN          TotalNumberOfEntries = PageSize / 8;        // Each entry 64 bytes long
            
    *PRP2TransferSize = 0;
    do {       
        *PRP2List++ = BufferAddress;
        if (BufferSize >= PageSize) {
            *PRP2TransferSize += PageSize;
            BufferAddress += PageSize;
            BufferSize -= PageSize;
        } else {
              *PRP2TransferSize = *PRP2TransferSize + (UINT32)BufferSize;
              BufferAddress += BufferSize;
              BufferSize = 0;
        }
    } while (--TotalNumberOfEntries && (BufferSize > 0));
    return  EFI_SUCCESS;
    
}
/**@internal
    For the corresponding controller version, check whether the command supports PRP2 list 
	or not.

    @param ControllerVersion    Specifies the Nvme spec version
    @param NvmeCmdWrapper       Pointer to hold NvmeCommandWrapper

    @retval TRUE                PRP2 can be a pointer.(i.e PRP2List can be programmed)
    @retval FALSE               PRP2 should not be a pointer

    @endinternal
**/
BOOLEAN
IsPrp2ListSupported (
  IN  UINT32                        ControllerVersion,      
  IN  NVME_COMMAND_WRAPPER          *NvmeCmdWrapper
) {
    UINT8    Prp2ListUnSupportedAdminCommands_1_1[] = { PRP2LIST_UNSUPPORTED_ADMIN_CMDS_1_1,0xFF };
    UINT8    Prp2ListUnSupportedNvmCommands_1_1[] = { PRP2LIST_UNSUPPORTED_NVM_CMDS_1_1,0xFF };
    UINT8    Prp2ListUnSupportedAdminCommands[] = { PRP2LIST_UNSUPPORTED_ADMIN_CMDS, 0xFF };
    UINT8    Index;
    
	//PRP2 should not be a pointer for Admin commands like Identity, Set feature,NameSpace attachment 
    if( NvmeCmdWrapper->AdminOrNVMCmdSet ) {      
        for (Index = 0; Prp2ListUnSupportedAdminCommands [Index] != 0xFF; Index++ ) {
            if ( NvmeCmdWrapper->NvmCmd.CMD0.Opcode == Prp2ListUnSupportedAdminCommands[Index] ) {
                return FALSE;
            }
        }
    }
    
	//Device which supports controller version 1.1 and less has limitation on PRP2list usage for some additional commands.  
    if ( ControllerVersion <= NVME_SPEC_VS_1_1 ) {
        if( NvmeCmdWrapper->AdminOrNVMCmdSet ) {
            for ( Index = 0; Prp2ListUnSupportedAdminCommands_1_1 [Index] != 0xFF; Index++ ) {
                if ( NvmeCmdWrapper->NvmCmd.CMD0.Opcode == Prp2ListUnSupportedAdminCommands_1_1[Index] ) {
                    return FALSE;
                }
            }
        } else {
            for ( Index = 0; Prp2ListUnSupportedNvmCommands_1_1 [Index] != 0xFF; Index++ ) {
                if ( NvmeCmdWrapper->NvmCmd.CMD0.Opcode == Prp2ListUnSupportedNvmCommands_1_1[Index] ) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}
/**@internal
    Program PRP1 and PRP2 entry in the NvmeCommandWrapper input.If ActiveNameSpace is found, then 
    PRP2List address will be obtained from ActiveNameSpaceList. If no ActiveNameSpace is found then 
    PRP2List address will be allocated and mapped inside this function.

    @param NvmeController    Pointer which holds AMI_NVME_CONTROLLER_PROTOCOL instance
    @param NvmeCmdWrapper    Pointer which holds NVME_COMMAND_WRAPPER instance
	@param TransferLength    Size of transfer buffer, which is transferred 
	                         to or from the NVM Express controller or namespace
    @param TransferBuffer    Pointer to hold the data buffer transferred.
    @param PRP2List          Holds the PRP2List address which is either obtained from ActiveNameSpaceList
                             or the newly allocated one.
    @param PRP2ListUnMap     If PRP2List address is newly allocated one, then it hold 
    @param BufferSize        Holds the sum of PRP1 and PRP2 transfer size

    @retval EFI_STATUS
	
	Note: Only when ActiveNameSpace is NULL, PRP2List and PRP2ListUnMap output parameters 
	      should be used by the caller function to unmap and free the allocated memory.
    @endinternal
**/
EFI_STATUS 
ProgramPRPEntry (
    IN  AMI_NVME_CONTROLLER_PROTOCOL         *NvmeController,
    IN  NVME_COMMAND_WRAPPER                 *NvmeCmdWrapper,
    IN  UINTN                                TransferLength,
    IN  UINTN                                *TransferBuffer,
    IN  ACTIVE_NAMESPACE_DATA                *ActiveNameSpace,
    OUT UINT64                               **PRP2List,
    OUT VOID                                 **PRP2ListUnMap,
    OUT UINTN                                *BufferSize
) {
	EFI_STATUS                    Status;
    UINTN                         PRP1TransferSize = 0;
    UINTN                         PRP2TransferSize = 0;
    EFI_PHYSICAL_ADDRESS          PRP2ListMappedAddr;             
    UINTN                         AllocatePageSize;

    NvmeCmdWrapper->NvmCmd.PRP1 = (UINT64)TransferBuffer; 
    PRP1TransferSize = NvmeController->MemoryPageSize - 
                         ((UINTN)(TransferBuffer) & ((UINTN)(NvmeController->MemoryPageSize) - 1));
    
    // If all data can be transferred using only PRP1 then do that.
    if (PRP1TransferSize >= TransferLength) {                    
        PRP1TransferSize = TransferLength;
    }
    
    // Do we need PRP2
    if (TransferLength - PRP1TransferSize) {                       	
        // Do we need either a PRP2 pointer or a List
        if ((TransferLength - PRP1TransferSize) <= NvmeController->MemoryPageSize) {                    	        	
            NvmeCmdWrapper->NvmCmd.PRP2 = NvmeCmdWrapper->NvmCmd.PRP1 + PRP1TransferSize;
            PRP2TransferSize = TransferLength - PRP1TransferSize;       	
        } else {
            if(!IsPrp2ListSupported(NvmeController->ControllerVersion,NvmeCmdWrapper)) {
                *BufferSize = PRP1TransferSize + NvmeController->MemoryPageSize;
                return EFI_BAD_BUFFER_SIZE;
            }
              if( !ActiveNameSpace ) {     
                  AllocatePageSize = NvmeController->MemoryPageSize;
                  Status = NvmeController->PciIO->AllocateBuffer (NvmeController->PciIO,
                                                       AllocateAnyPages,
                                                       EfiRuntimeServicesData,
                                                       EFI_SIZE_TO_PAGES(AllocatePageSize),
                                                       (VOID**)PRP2List,
                                                       EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED | EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE
                                                       );
                  if (EFI_ERROR(Status)) {
                	  return Status;
                  }
                  
                  Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                                  EfiPciIoOperationBusMasterCommonBuffer,
                                                  (VOID *)*PRP2List,
                                                  &AllocatePageSize,
                                                  &PRP2ListMappedAddr,
                                                  PRP2ListUnMap
                                                  );
                  if (EFI_ERROR(Status)) {
                	  return Status;
                  }
				  
              } else {
                  *PRP2List = ActiveNameSpace->PRP2List;
                  PRP2ListMappedAddr = ActiveNameSpace->PRP2ListMappedAddr;
              }
            
              // We need PRP2 List
              Status = ProgramPRP2List (*PRP2List, NvmeController->MemoryPageSize, \
                                    (UINTN)(TransferBuffer) + PRP1TransferSize, \
                                    TransferLength -  PRP1TransferSize, &PRP2TransferSize
                                    );
              if (EFI_ERROR(Status)) {
                  return Status;
              }
              NvmeCmdWrapper->NvmCmd.PRP2 = (UINT64) PRP2ListMappedAddr;
        }
    }      
    *BufferSize = PRP1TransferSize + PRP2TransferSize;       
    return  EFI_SUCCESS;
}
/**
    Read/Write data from the given LBA address

        
    @param This 
    @param MediaId 
    @param LBA
    @param BufferSize 
    @param Buffer 
    @param ReadWriteOpCode 

    @retval EFI_STATUS

**/

EFI_STATUS
NvmeReadWriteBlocks (
    IN  ACTIVE_NAMESPACE_DATA   *ActiveNameSpace,
    IN  UINT32                  MediaId,
    IN  EFI_LBA                 LBA,
    IN  UINTN                   BufferSize,
    OUT VOID                    *Buffer,
    OUT COMPLETION_QUEUE_ENTRY  *NvmeCompletionData,
    IN  UINT8                   ReadWriteOpCode
)
{  
    EFI_STATUS                    Status = EFI_SUCCESS;
    AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController = ActiveNameSpace->NvmeController;
    NVME_DXE_SMM_DATA_AREA        *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    EFI_BLOCK_IO_MEDIA            *BlkMedia = ActiveNameSpace->NvmeBlockIO.Media;
    NVME_COMMAND_WRAPPER          *NvmeCmdWrapper = NvmeController->NvmeCmdWrapper;
    COMPLETION_QUEUE_ENTRY        CompletionData;
    COMPLETION_QUEUE_ENTRY        *pCompletionData = &CompletionData;
    UINTN                         DataN;
    UINT32                        LBACountInOneTransfer;
    UINTN                         MappedBufferSize = BufferSize;
    UINTN                         PRPTransferSize;   
    EFI_PHYSICAL_ADDRESS          MappedBuffer;
    VOID                          *BufferUnMap;
    EFI_PHYSICAL_ADDRESS          UserMappedBufferPtr;
    UINTN                         Count;
    UINTN                         TotalMetaDataSize = 0;
    UINT16                        MetaDataSize = 0; 
    
    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO,"LBA : %lx BufferSize : %lx Buffer : %lx Opcode : %x", LBA, BufferSize, Buffer, ReadWriteOpCode));
    }

    // Check if Media ID matches
    if (BlkMedia->MediaId != MediaId) {
        return EFI_MEDIA_CHANGED;
    }

    if (BufferSize == 0) {
        return EFI_SUCCESS;
    }
    
    //
    // If IoAlign values is 0 or 1, means that the buffer can be placed 
    // anywhere in memory or else IoAlign value should be power of 2. To be
    // properly aligned the buffer address should be divisible by IoAlign  
    // with no remainder. 
    // 
    if((BlkMedia->IoAlign > 1 ) && ((UINTN)Buffer % BlkMedia->IoAlign)) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Check whether the block size is multiple of BlkMedia->BlockSize
    DataN = BufferSize % BlkMedia->BlockSize;
    if (DataN){
	    return EFI_BAD_BUFFER_SIZE;
    }

    // Check for Valid start LBA #
    if (LBA > BlkMedia->LastBlock) {
        return EFI_INVALID_PARAMETER;
    }

    // Check for Valid End LBA #
    DataN = BufferSize / BlkMedia->BlockSize;
    if (LBA + DataN > BlkMedia->LastBlock + 1) {
        return EFI_INVALID_PARAMETER;
    }
    MetaDataSize = ActiveNameSpace->IdentifyNamespaceData.LBAF[ActiveNameSpace->IdentifyNamespaceData.FLBAS & 0xF].MS;
    // Map pre-allocated buffer for MetaData
    if (!NvmeController->NvmeInSmm ) {
    
        if (ActiveNameSpace->MetaDataBlock) {
            
            ActiveNameSpace->MappedMetaDataBufferSize = ActiveNameSpace->MetaDataBufferSize;
            
            Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                     EfiPciIoOperationBusMasterCommonBuffer,
                                     (VOID *)ActiveNameSpace->MetaDataBlock,
                                     (UINTN *)&ActiveNameSpace->MappedMetaDataBufferSize,
                                     &ActiveNameSpace->MetaDataBlockMappedAddr,
                                     (VOID**)&ActiveNameSpace->MetaDataBlockUnMap
                                     );
            
            if (EFI_ERROR(Status)) {
                return Status;
            }
            
        }
        
        if (ActiveNameSpace->ExtendedLBA) {
            
            ActiveNameSpace->MappedExtendedLBABufferSize = ActiveNameSpace->ExtendedLBABufferSize;
            
            Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                             EfiPciIoOperationBusMasterCommonBuffer,
                                             (VOID *)ActiveNameSpace->ExtendedLBA,
                                             (UINTN *)&ActiveNameSpace->MappedExtendedLBABufferSize,
                                             &ActiveNameSpace->ExtendedLBAMappedAddr,
                                             (VOID**)&ActiveNameSpace->ExtendedLBAUnMap
                                             );
                    
            if (EFI_ERROR(Status)) {
                return Status;;
            }
           
        }
        
    }
    
    do {        
        MappedBuffer = (EFI_PHYSICAL_ADDRESS) Buffer;     
        // Limit the transfer size to MDTS value
        if ( (NvmeController->IdentifyControllerData->MDTS) && \
                (MappedBufferSize > ((UINTN)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) * NvmeController->MemoryPageSizeMin))){
            MappedBufferSize = (UINTN)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) * NvmeController->MemoryPageSizeMin;
        }
       
        // Reduce MappedBuffer size if local buffer available is less when MetaData is enabled
        if (ActiveNameSpace->ExtendedLBA) {
            
            TotalMetaDataSize = (MappedBufferSize / BlkMedia->BlockSize) * MetaDataSize;

            // Make sure MappedExtendedLBABufferSize can hold LBA Data + MetaData
            MappedBufferSize += TotalMetaDataSize;
            MappedBufferSize = (UINTN)(MappedBufferSize > (ActiveNameSpace->MappedExtendedLBABufferSize) ? \
                               (ActiveNameSpace->MappedExtendedLBABufferSize) : MappedBufferSize);            
            UserMappedBufferPtr = MappedBuffer;
            
            // Use the local temp. buffer instead of user provided buffer
            MappedBuffer = ActiveNameSpace->ExtendedLBAMappedAddr;
            
        } else if  (ActiveNameSpace->MetaDataBlock) {
            // MetaData is a separate buffer. Data will be transferred based on the size of meta data buffer allocated locally
            MappedBufferSize = (UINTN)(MappedBufferSize > (ActiveNameSpace->MappedMetaDataBufferSize) ? \
                               (ActiveNameSpace->MappedMetaDataBufferSize) : MappedBufferSize);
        }
        
        // After MAP, Mapped BufferSize may not be same as input. 
        // So need to complete the transfer in multiple loops.
        // When the call originates from SMM, don't call Map function. 
        // Also don't need to map it if ExtLBA is used. The buffer is already mapped.
        if (!NvmeController->NvmeInSmm && !ActiveNameSpace->ExtendedLBA) {

            Status = NvmeController->PciIO->Map ( NvmeController->PciIO,
                                          (ReadWriteOpCode == NVME_READ) ? \
                                                  EfiPciIoOperationBusMasterWrite : EfiPciIoOperationBusMasterRead,
                                          Buffer,
                                          &MappedBufferSize,
                                          &MappedBuffer,
                                          &BufferUnMap
                                          );
          
            if (EFI_ERROR(Status)) {
                return Status;
            }
        }
    
        SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0); 
        LBACountInOneTransfer = 0;
        NvmeCmdWrapper->NvmCmd.CMD0.Opcode = ReadWriteOpCode;
        NvmeCmdWrapper->AdminOrNVMCmdSet = FALSE;
        Status = ProgramPRPEntry(NvmeController,
        		                NvmeCmdWrapper,
        		                MappedBufferSize, 
        		                (VOID*)MappedBuffer,
        		                ActiveNameSpace,
        		                &(ActiveNameSpace->PRP2List),
        		                NULL,
        		                &PRPTransferSize);
        
        if (EFI_ERROR(Status)) {
            break;
        }    
        LBACountInOneTransfer = (UINT32)((PRPTransferSize - TotalMetaDataSize) / BlkMedia->BlockSize);
        // Is metaData enabled AND is it a WRITE? 
        if ((ReadWriteOpCode == NVME_WRITE) || (ReadWriteOpCode == NVME_COMPARE)) {
            
            // For Write operations, Data need to be moved from user buffer to local buffer.
            if (ActiveNameSpace->ExtendedLBA) {
                
                // Transfer data from user buffer to local buffer
                for (Count = 0; Count < LBACountInOneTransfer; Count++) { 
                    
                                
                    CopyMem ((VOID *)(ActiveNameSpace->ExtendedLBAMappedAddr + ((BlkMedia->BlockSize + MetaDataSize)* Count)), \
                            (VOID *)(UserMappedBufferPtr + (BlkMedia->BlockSize * Count)), BlkMedia->BlockSize);

                    //MetaData part of extLBA. So clear memory at the end of LBA size
                    SetMem((VOID *)(ActiveNameSpace->ExtendedLBAMappedAddr + ((BlkMedia->BlockSize + MetaDataSize) * Count ) + BlkMedia->BlockSize), \
                            MetaDataSize, 0);
                }

            } else if (ActiveNameSpace->MetaDataBlock) {
                
                // Clear out the metadata alone
                for (Count = 0; Count < LBACountInOneTransfer; Count++) { 
                    SetMem((VOID *)(ActiveNameSpace->MetaDataBlockMappedAddr  + (MetaDataSize * Count)), \
                            MetaDataSize, 0);
                }
            }
        }
        
        // Build NVME command
        NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = 0;
        NvmeCmdWrapper->NvmCmd.CMD0.PSDT = 0;
        NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierQueue1;
        NvmeCmdWrapper->NvmCmd.NSID = ActiveNameSpace->ActiveNameSpaceID;
        
        NvmeCmdWrapper->NvmCmd.MPTR = ActiveNameSpace->MetaDataBlock ? ActiveNameSpace->MetaDataBlockMappedAddr : 0;
        NvmeCmdWrapper->NvmCmd.CDW10 = (UINT32)LBA;       
        NvmeCmdWrapper->NvmCmd.CDW11 = (UINT32) RShiftU64 (LBA, 32);           
        NvmeCmdWrapper->NvmCmd.CDW12 = 0x80000000 + (LBACountInOneTransfer - 1);           
        NvmeCmdWrapper->NvmCmd.CDW13 = 0;           
        NvmeCmdWrapper->NvmCmd.CDW14 = 0;           
            
        NvmeCmdWrapper->SQIdentifier = NvmeController->NVMQueueNumber;
        NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
        
        // If caller has passed buffer to return controller status, use it.
        if (NvmeCompletionData) {
            pCompletionData = NvmeCompletionData;
        }
        
        // Is Protection enabled?
        if (ActiveNameSpace->IdentifyNamespaceData.DPS & 0x07) {
            // Protection is stripped/inserted for Read/write by the controller. Set PRACT
            NvmeCmdWrapper->NvmCmd.CDW12 |= 0x20000000;    
        }
        
        Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, pCompletionData);
        
        if (EFI_ERROR(Status)) {
            if (!NvmeController->NvmeInSmm  && !ActiveNameSpace->ExtendedLBA) {
                NvmeController->PciIO->Unmap(NvmeController->PciIO, BufferUnMap);
            }
            break;
        }
        
        // Reduce the LBA and Buffer size to reflect only the DATA and exclude MetaData
        if (ActiveNameSpace->ExtendedLBA) {
            
            MappedBufferSize = MappedBufferSize -  TotalMetaDataSize;
                
            if (ReadWriteOpCode == NVME_READ) {
                // Copy data from local buffer to user buffer and skip the meta data
                for (Count = 0; Count < LBACountInOneTransfer; Count++) { 
                    CopyMem ((VOID *)(UserMappedBufferPtr + (BlkMedia->BlockSize * Count)), \
                            (VOID *)(ActiveNameSpace->ExtendedLBAMappedAddr + ((BlkMedia->BlockSize + MetaDataSize ) * Count)),  BlkMedia->BlockSize);
                }
            }        
        }
        
        
        // Remaining Bytes to be transferred
        MappedBufferSize -= (LBACountInOneTransfer * BlkMedia->BlockSize);
        
        // Update LBA # for next transfer if needed        
        LBA += LBACountInOneTransfer;
        
       // Adjust the Buffer address
       Buffer =(VOID*) ((UINTN) Buffer + (LBACountInOneTransfer * BlkMedia->BlockSize));
       BufferSize -= (LBACountInOneTransfer * BlkMedia->BlockSize);

       // When PciIO->Map is called, it might not map the complete buffer.
       // After the complete MappedBufferSize is transferred, if there is a left over do that transfer also
       if (MappedBufferSize == 0) {
           MappedBufferSize = BufferSize;
       }
       
       if (!NvmeController->NvmeInSmm && !ActiveNameSpace->ExtendedLBA) {
           NvmeController->PciIO->Unmap(NvmeController->PciIO, BufferUnMap);
       }
       
    } while (MappedBufferSize);
    
    if (!NvmeController->NvmeInSmm) {
        
        if (ActiveNameSpace->MetaDataBlock) {
            NvmeController->PciIO->Unmap(NvmeController->PciIO, ActiveNameSpace->MetaDataBlockUnMap);    
        }
        
        if (ActiveNameSpace->ExtendedLBA) {
            NvmeController->PciIO->Unmap(NvmeController->PciIO, ActiveNameSpace->ExtendedLBAUnMap);    
        }
        DEBUG((DEBUG_BLKIO,"NvmeReadWriteBlocks : %r \n", Status));
    }
    return Status;
    
}

/**
    Execute Admin and Nvme cmds

    @param NvmeController 
    @param NvmeCmdWrapper 
    @param CmdCompletionData 

    @retval EFI_STATUS

    @note  If the cmd needs to be retried due to a failure, caller can 
    initialize the RetryCount.Can be called recursively.


**/

EFI_STATUS
EFIAPI
ExecuteNvmeCmd (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper,
    OUT COMPLETION_QUEUE_ENTRY          *CmdCompletionData
)

{
    EFI_STATUS              Status;
    EFI_STATUS              RecreateQueueStatus;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    UINT8    CmdRetryCount  = MAX_RETRY_COUNT_FOR_FATAL_ERROR;

    //controller in FATAL error condition state. Can't be recovered
    if (NvmeController->DeviceState == Fatal_UnRecovered ) {
        return EFI_DEVICE_ERROR;
    }
    
    do {

        if (!NvmeController->NvmeInSmm) {
            PrintNvmeCmdWrapper (NvmeCmdWrapper);
        }
        
        RecreateQueueStatus = EFI_DEVICE_ERROR;

        // Because of recursive nature and retry mechanism, cmd identifier needs to be updated just before giving the call.
        NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeDxeSmmDataArea->CommandIdentifierAdmin : \
                                                         NvmeDxeSmmDataArea->CommandIdentifierQueue1;
        
        NvmeCmdWrapper->AdminOrNVMCmdSet ? (Status = AddToAdminSubmissionQueue(NvmeController, NvmeCmdWrapper)) : \
                (Status = AddToQueue1SubmissionQueue(NvmeController, NvmeCmdWrapper));
    
        if (EFI_ERROR(Status)) {
            continue;
        }
    
        Status = UpdateDoorBellRegister(NvmeController, 
                                        NvmeCmdWrapper->SQIdentifier,
                                        NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr :
                                        NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr 
                                        );

        if (EFI_ERROR(Status)) {
            continue;
        }

        //Wait for cmd to complete
        if (NvmeCmdWrapper->NvmCmd.CMD0.Opcode != ASYNC_EVENT_REQUEST) {
            Status = WaitForCompletionQueueUpdate(NvmeController, NvmeCmdWrapper, CmdCompletionData);
        }
        
        // Follow Fatal Error Mechanism if WaitForCompletionQueueUpdate returns EFI_ABORTED/EFI_TIMEOUT Status
        if ((Status == EFI_ABORTED || Status == EFI_TIMEOUT) && (NvmeController->DeviceState==Functional)) {

            RecreateQueueStatus = RecreateAllQueues(NvmeController );

            if ( EFI_ERROR(RecreateQueueStatus) ){
                NvmeController->DeviceState = Fatal_UnRecovered;
            } else {
                NvmeController->DeviceState = Functional;
            }
        }
    
    } while (EFI_ERROR(Status) && (!EFI_ERROR(RecreateQueueStatus) && CmdRetryCount--) );
    
    return  Status;
    
}

/**
    Submits the cmd to the Admin Submission queue


    @param NvmeController 
    @param NvmeCmdWrapper 

    @retval EFI_STATUS

**/

EFI_STATUS
AddToAdminSubmissionQueue (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
)
{
    EFI_STATUS              Status;
    UINT64                  DestinationAddress;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    
    // Is Admin Queue full?
    if (NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr) {  // Non-zero value
        if ((NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr - 1) == NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr){
            
            // In this design, queue should never get filled up. 
            // If it does something is wrong. Delete and start all over again.
            
            Status = RecreateAllQueues (NvmeController);
            if (EFI_ERROR(Status)) {            
                return EFI_OUT_OF_RESOURCES;
            }
          
        }
    } else {
        // If Head is at the start and Tail is at the end, then queue is full
          if (NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr == (NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr + 
                                                            NvmeController->AdminSubmissionQueueSize - 1)) {
            
            // In this design, queue should never get filled up. 
            // If it does something is wrong. Delete and start all over again.
              Status = RecreateAllQueues (NvmeController);
              if (EFI_ERROR(Status)) {            
                  return EFI_OUT_OF_RESOURCES;
              }
          }
    }
    
    // Copy cmd to Admin Queue
    DestinationAddress = NvmeController->AdminSubmissionQueue + (
                          NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr * sizeof(NVME_ADMIN_COMMAND));

    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO, "Destination Address for Cmd               : %016lx\n", DestinationAddress));
    }

    
    CopyMem ((VOID *)DestinationAddress, &(NvmeCmdWrapper->NvmCmd), sizeof(NVME_ADMIN_COMMAND));
    
    NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr++;
    
    // Check if there is a roller over
    if (NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr >= (NvmeController->AdminSubmissionQueueSize)) {
        NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr = 0;
    }
    
    return EFI_SUCCESS;
    
}

/**
    Submits the Nvme cmd to the Queue1 Submission queue

    @param  NvmeController 
    @param  NvmeCmdWrapper 

    @retval EFI_STATUS

**/

EFI_STATUS
AddToQueue1SubmissionQueue (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
)
{
    EFI_STATUS              Status;
    UINT64                  DestinationAddress;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    
    // Is Queue full? If Tail is one less than the Head queue is full.
    if (NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr) {  // Non-zero value
        if ((NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr - 1) == NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr){
            
            // In this design, queue should never get filled up. 
            // If it does something is wrong. Delete and start all over again.
            Status = RecreateAllQueues (NvmeController);
            if (EFI_ERROR(Status)) {
                return EFI_OUT_OF_RESOURCES;
            }
        }
    } else { 
          // If Head is at the start and Tail is at the end, then queue is full
          if (NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr == (NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr + 
                                                        NvmeController->Queue1SubmissionQueueSize - 1)) {
              
              // In this design, queue should never get filled up. 
              // If it does something is wrong. Delete and start all over again.
              Status = RecreateAllQueues (NvmeController);
              if (EFI_ERROR(Status)) {		    
                  return EFI_OUT_OF_RESOURCES;
              }
          }
    }
    
    // Copy cmd to Admin Queue
    DestinationAddress = NvmeController->Queue1SubmissionQueue + 
                        (NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr * sizeof(NVME_ADMIN_COMMAND));

    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO, "Destination Address for Cmd               : %016lx\n", DestinationAddress));
    }

    
    CopyMem ((VOID *)DestinationAddress, &(NvmeCmdWrapper->NvmCmd), sizeof(NVME_ADMIN_COMMAND));
    
    NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr++; 
    
    // Check if there is a roller over
    if (NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr >= NvmeController->Queue1SubmissionQueueSize) {
        NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr = 0;
    }
    
    return EFI_SUCCESS;
    
}

/**
    Update door bell register for the controller to start executing the cmd

    @param  NvmeController 
    @param  QueueNumber 
    @param  Value 

    @retval EFI_STATUS

**/

EFI_STATUS
UpdateDoorBellRegister (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  UINT16                          QueueNumber,
    IN  UINT32                          Value
)
{
    
    UINT32      Offset;
    
    // Update Door Bell Register
    Offset = QUEUE_DOORBELL_OFFSET(QueueNumber, 0, NvmeController->DoorBellStride);
    
    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO, "\nDoorBell Offset %016lx Value %08X\n", NvmeController->NvmeBarOffset + Offset, Value));
    }
    
    CONTROLLER_WRITE_REG32(NvmeController, Offset, Value);
    return EFI_SUCCESS;
    
}

/**
    Checks for the completion queue for the correct PhaseTag, 
    Queue Identified and Cmd Identifier.

    @param  NvmeController 
    @param  NvmeCmdWrapper 
    @param  CmdCompletionData 

    @retval EFI_STATUS

**/

EFI_STATUS
WaitForCompletionQueueUpdate (
    IN  AMI_NVME_CONTROLLER_PROTOCOL        *NvmeController,
    IN  NVME_COMMAND_WRAPPER                *NvmeCmdWrapper,
    OUT COMPLETION_QUEUE_ENTRY              *CmdCompletionData 
)
{
    EFI_STATUS              Status;
    UINT64                  TimeOut = MultU64x32((UINT64)NvmeCmdWrapper->CmdTimeOut,100); // Convert the delay unit from 1ms to 10us
    UINT32                  Offset;
    COMPLETION_QUEUE_ENTRY  *pCmdCompletionData;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;

    UINT16                  CommandIdentifier = NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeDxeSmmDataArea->CommandIdentifierAdmin : \
                                                NvmeDxeSmmDataArea->CommandIdentifierQueue1; 
    
    UINT64                  CompletionQueueStart = NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeController->AdminCompletionQueue : \
                                                NvmeController->Queue1CompletionQueue; 
    
    UINT16                  CompletionQueueHeadPtr = NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr :\
                                                NvmeDxeSmmDataArea->Queue1CompletionQueueHeadPtr;
    
    // Toggle expected phase tag on every roll over
    if (CompletionQueueHeadPtr == 0) {
            NvmeCmdWrapper->AdminOrNVMCmdSet ? (NvmeDxeSmmDataArea->AdminPhaseTag = ~NvmeDxeSmmDataArea->AdminPhaseTag) : \
                        (NvmeDxeSmmDataArea->Queue1PhaseTag = ~NvmeDxeSmmDataArea->Queue1PhaseTag);
    }
    
    // Get the offset to the Command Completion Queue Head Pointer
    pCmdCompletionData = (COMPLETION_QUEUE_ENTRY *)(CompletionQueueStart + CompletionQueueHeadPtr * sizeof(COMPLETION_QUEUE_ENTRY));
    
    do {
        // Check whether Command Identifier, SQ ID matches and Phase Tag matches with the cmd issued.
        if ((pCmdCompletionData->CommandIdentifier == CommandIdentifier) && \
            (pCmdCompletionData->SQIdentifier == NvmeCmdWrapper->SQIdentifier) && \
            (pCmdCompletionData->PhaseTag == (NvmeCmdWrapper->AdminOrNVMCmdSet ? \
                    NvmeDxeSmmDataArea->AdminPhaseTag : NvmeDxeSmmDataArea->Queue1PhaseTag)))
        {
            break;
        }
        
        // check if there are any fatal errors
        if (CONTROLLER_REG32(NvmeController, Offset_CSTS) & CSTS_CFS){
            if (!NvmeController->NvmeInSmm) {
                DEBUG((DEBUG_ERROR, "Nvme Fatal Error\n"));
            }
            return EFI_ABORTED;
        }
        
        MicroSecondDelay (10);  // 10us delay

    } while (--TimeOut);
    
    if (!NvmeController->NvmeInSmm) { 
        PrintCommandCompletionData (pCmdCompletionData);
    }
    
    if (!TimeOut) {
        if (!NvmeController->NvmeInSmm) {
            DEBUG((DEBUG_ERROR, "Nvme TimeOut\n"));
        }

        return EFI_TIMEOUT;
    }
    
    // Update HeadPtr from Completion Queue. 
    // Check what Queue was cmd posted to and then update the corresponding Head/Tail pointer
    if (NvmeCmdWrapper->AdminOrNVMCmdSet) {
        NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr = pCmdCompletionData->SQHeadPointer;
        NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr = NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr;
        Offset = QUEUE_DOORBELL_OFFSET(NvmeCmdWrapper->SQIdentifier, 1, NvmeController->DoorBellStride);
        CONTROLLER_WRITE_REG32(NvmeController, Offset, NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr);
    } else { 
        NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr = pCmdCompletionData->SQHeadPointer;
        NvmeDxeSmmDataArea->Queue1CompletionQueueHeadPtr = NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr;
        Offset = QUEUE_DOORBELL_OFFSET(NvmeCmdWrapper->SQIdentifier, 1, NvmeController->DoorBellStride);
        CONTROLLER_WRITE_REG32(NvmeController, Offset, NvmeDxeSmmDataArea->Queue1CompletionQueueHeadPtr);   
    }
    
    // Check whether all cmds submitted has been completed. CompletionQueue Head pointer should give a clue on
    // how many cmds where executed.
    if (pCmdCompletionData->StatusCode || pCmdCompletionData->StatusCodeType) {
        Status = EFI_DEVICE_ERROR;
    } else {
        Status = EFI_SUCCESS;
    }

#if  NVME_VERBOSE_PRINT
    if (!NvmeController->NvmeInSmm) {
        if (NvmeCmdWrapper->AdminOrNVMCmdSet) {
            DEBUG((DEBUG_BLKIO, "AdminSubmissionQueueHeadPtr %08X\n", NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr));
            DEBUG((DEBUG_BLKIO, "AdminSubmissionQueueTailPtr %08X\n", NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr));
            DEBUG((DEBUG_BLKIO, "AdminCompletionQueueHeadPtr %08X\n", NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr));
            DEBUG((DEBUG_BLKIO, "AdminCompletionQueueTailPtr %08X\n", NvmeDxeSmmDataArea->AdminCompletionQueueTailPtr));
        } else {
            DEBUG((DEBUG_BLKIO, "Queue1SubmissionQueueHeadPtr %08X\n", NvmeDxeSmmDataArea->Queue1SubmissionQueueHeadPtr));
            DEBUG((DEBUG_BLKIO, "Queue1SubmissionQueueTailPtr %08X\n", NvmeDxeSmmDataArea->Queue1SubmissionQueueTailPtr));
            DEBUG((DEBUG_BLKIO, "Queue1CompletionQueueHeadPtr %08X\n", NvmeDxeSmmDataArea->Queue1CompletionQueueHeadPtr));
            DEBUG((DEBUG_BLKIO, "Queue1CompletionQueueTailPtr %08X\n", NvmeDxeSmmDataArea->Queue1CompletionQueueTailPtr));
        }
    }
#endif
    
    // Update o/p buffer
    CopyMem ((VOID *)CmdCompletionData, pCmdCompletionData, sizeof(COMPLETION_QUEUE_ENTRY));
    NvmeCmdWrapper->AdminOrNVMCmdSet ? NvmeDxeSmmDataArea->CommandIdentifierAdmin++ : NvmeDxeSmmDataArea->CommandIdentifierQueue1++;
    
    return Status;
    
}

/**
    Delete Admin and other Completion/Submission queue and 
    create it back again.

    @param  NvmeController 

    @retval EFI_STATUS

    @note  This procedure will be called if the queue gets filled up. 
    This situation shouldn't generally happen as each cmd is completed in 
    our case. So both Head and Tail should point to the same location 
    before and after cmd is executed.One possibility for calling this 
    routine will be when the cmd doesn't get completed and Completion 
    Queue doesn't get updated by the controller. Also any Set Feature 
    cmd that was issued during initialization should be re-issued here.

**/

EFI_STATUS
RecreateAllQueues (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController
)
{
    
    EFI_STATUS              Status = EFI_SUCCESS;
    UINT32                  ProgramCC = 0;
    UINT32                  Delay;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    NVME_COMMAND_WRAPPER    LocalNvmeCmdWrapper;
    NVME_COMMAND_WRAPPER    *NvmeCmdWrapper = &LocalNvmeCmdWrapper;
    
    // In SMM, this function is not supported
    if (NvmeController->NvmeInSmm) {
        return EFI_DEVICE_ERROR;
    }

    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO, "Recreate Admin/IO Queue : "));
    }
 
    // Device is already in Re-Configuring State, Exit with Error
    if ( NvmeController->DeviceState == Fatal_Recovery_state ) {
        return EFI_DEVICE_ERROR;
    }
    
    NvmeController->DeviceState = Fatal_Recovery_state;
    
    ProgramCC = CONTROLLER_REG32(NvmeController, Offset_CC);
    
    // Check if the controller is already running. If yes stop it.
    Delay = NvmeController->TimeOut * 500;
    if (CONTROLLER_REG32(NvmeController, Offset_CC) & 0x1) {
        
        //  Clear Control register
        CONTROLLER_WRITE_REG32 (NvmeController, Offset_CC, 0);
        do {
            if (!(CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0x1)) {
                break;
            }
            MicroSecondDelay (1000);
        } while (--Delay);
        
    }
    
    if (!Delay) {
        goto  RecreateAllQueues_Error;  
    }
    
    // Program Admin Queue Size and Base Address
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Aqa ,  
             ((NvmeController->AdminCompletionQueueSize - 1) << 16) + (NvmeController->AdminSubmissionQueueSize - 1) );
     
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Asq,  
             (UINT32) NvmeController->AdminSubmissionQueue);
     
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Asq + 4,   
             (UINT32) RShiftU64(NvmeController->AdminSubmissionQueue, 32) );
     
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Acq,   
             (UINT32)NvmeController->AdminCompletionQueue);
     
    CONTROLLER_WRITE_REG32(NvmeController, Offset_Acq + 4, 
             (UINT32) RShiftU64(NvmeController->AdminCompletionQueue, 32) );

    // Enable Controller
    CONTROLLER_WRITE_REG32(NvmeController, Offset_CC, (ProgramCC | 1) );
     
    // Wait for the controller to get ready
    // Check if the controller is already running. If yes stop it.
    Delay = NvmeController->TimeOut * 500;
    do {
        if ((CONTROLLER_REG32(NvmeController, Offset_CSTS) & 0x1)) {
            break;
        }
        
        MicroSecondDelay (1000);
    } while (--Delay);
         
RecreateAllQueues_Error:     
    if (!Delay) {
        Status = EFI_DEVICE_ERROR;
        if (!NvmeController->NvmeInSmm) {
            DEBUG((DEBUG_ERROR, "Status  %r\n", Status));
        }
        return Status;
    }
    
    NvmeDxeSmmDataArea->AdminSubmissionQueueHeadPtr = 0;
    NvmeDxeSmmDataArea->AdminSubmissionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->AdminCompletionQueueHeadPtr = 0;
    NvmeDxeSmmDataArea->AdminCompletionQueueTailPtr = 0;
    NvmeDxeSmmDataArea->AdminPhaseTag = FALSE;
    
    Status = SetNumberOfQueues ( NvmeController,
                                 NvmeCmdWrapper );

    if (!EFI_ERROR(Status) && (NvmeController->NVMQueueNumber == 1) ) {
        
        // Create Submission and Completion Queue1
        Status  = CreateAdditionalSubmissionCompletionQueue(
                                    NvmeController, 
                                    NvmeCmdWrapper, 
                                    NvmeController->NVMQueueNumber, 
                                    NvmeController->Queue1SubmissionQueueSize
                                    );
    }
    
    if (!NvmeController->NvmeInSmm) {
        DEBUG((DEBUG_BLKIO, "Recreate Admin/IO Queues Status %r\n", Status));
    }
    
    return Status;
}

/**
    Issue Set feature command to allocate I/O queues

    @param  NvmeController 
    @param  NvmeCmdWrapper

    @retval EFI_STATUS

**/

EFI_STATUS
SetNumberOfQueues (
    IN  AMI_NVME_CONTROLLER_PROTOCOL    *NvmeController,
    IN  NVME_COMMAND_WRAPPER            *NvmeCmdWrapper
)
{
    EFI_STATUS              Status;
    NVME_DXE_SMM_DATA_AREA  *NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;
    COMPLETION_QUEUE_ENTRY  CompletionData;

    // If FATAL error occurred for SET_FEATURES nvme command, Return from here.
    if( (NvmeController->DeviceState == Fatal_Recovery_state) && \
        NvmeController->SetNumberOfQueuesInProgress ) {
        return EFI_SUCCESS;
    }

    NvmeController->SetNumberOfQueuesInProgress = TRUE;

    // Issue Set Feature cmd to initialize # of queues to 1
    gBS->SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
    
    // Build NVME command
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = SET_FEATURES;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = NvmeDxeSmmDataArea->CommandIdentifierAdmin;
            
    NvmeCmdWrapper->NvmCmd.CDW10 = 0x7;
    NvmeCmdWrapper->NvmCmd.CDW11 = 0x10001;
    
    NvmeCmdWrapper->AdminOrNVMCmdSet = TRUE;
    NvmeCmdWrapper->SQIdentifier = 0;           // Queue 0 for Admin cmds
    NvmeCmdWrapper->CmdTimeOut = NVME_COMMAND_TIMEOUT * 1000;
    
    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, &CompletionData);
	
	NvmeController->SetNumberOfQueuesInProgress = FALSE;
    
    return Status;
    
}

/**
    Prints the cmd completion status

    @param pCmdCompletionData 

    @retval VOID

    @note  The amount of data that will get printed can be controlled 
    using DEBUG_ERROR_LEVEL_MASK SDL token.
    Make sure PcdDebugPrintErrorLevel is properly cloned and set to 
    PcdsPatchableInModule in the project.

**/

VOID
PrintCommandCompletionData (
    IN  COMPLETION_QUEUE_ENTRY  *pCmdCompletionData
)
{
    
#if  NVME_VERBOSE_PRINT
    // Fig 25 NVM Express 1.1 spec
    // Print Completion Cmd Data
    DEBUG((DEBUG_BLKIO, "Completion Queue DW2                      : %08X\n", *((UINT32 *)pCmdCompletionData + 2)));
    DEBUG((DEBUG_BLKIO, "Completion Queue DW3                      : %08X\n", *((UINT32 *)pCmdCompletionData + 3)));
    DEBUG((DEBUG_BLKIO, "Completion Queue DW0                      : %08X\n", pCmdCompletionData->DW0));
    DEBUG((DEBUG_BLKIO, "Completion Queue Reserved                 : %08X\n", pCmdCompletionData->DW1));
#endif
    
}

/**
    Prints Nvme cmd parameters

    @param NvmeCmdWrapper 

    @retval VOID

    @note  The amount of data that will get printed can be controlled 
    using DEBUG_ERROR_LEVEL_MASK SDL token.
    Make sure PcdDebugPrintErrorLevel is properly cloned and set to 
    PcdsPatchableInModule in the project.

**/

VOID
PrintNvmeCmdWrapper (
    NVME_COMMAND_WRAPPER    *NvmeCmdWrapper
)
{
    
#if  NVME_VERBOSE_PRINT    
    DEBUG((DEBUG_BLKIO,"\nCMD DW0                                  : %08X\n", *(UINT32 *)&(NvmeCmdWrapper->NvmCmd)));
    DEBUG((DEBUG_BLKIO, "NSID                                      : %08X\n", NvmeCmdWrapper->NvmCmd.NSID));
    DEBUG((DEBUG_BLKIO, "MPTR                                      : %016lX\n", NvmeCmdWrapper->NvmCmd.MPTR));
    DEBUG((DEBUG_BLKIO, "PRP1                                      : %016lX\n", NvmeCmdWrapper->NvmCmd.PRP1));
    DEBUG((DEBUG_BLKIO, "PRP2                                      : %016lX\n", NvmeCmdWrapper->NvmCmd.PRP2));
    DEBUG((DEBUG_BLKIO, "CDW10                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW10));
    DEBUG((DEBUG_BLKIO, "CDW11                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW11));
    DEBUG((DEBUG_BLKIO, "CDW12                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW12));
    DEBUG((DEBUG_BLKIO, "CDW13                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW13));
    DEBUG((DEBUG_BLKIO, "CDW14                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW14));
    DEBUG((DEBUG_BLKIO, "CDW15                                     : %08X\n",  NvmeCmdWrapper->NvmCmd.CDW15));
    DEBUG((DEBUG_BLKIO, "Cmd sent to Queue                         : %08X\n",  NvmeCmdWrapper->SQIdentifier));
#endif   
    
}

/**
    Read the Dword Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Index       - Index/Offset address to read           

    @retval Value Read

**/
UINT32
ReadDataDword(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset
)
{
    UINT32      Data = 0;
    
    if( NvmeController == NULL ) {
        return Data;
    }
    
    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Read (
                     NvmeController->PciIO,
                     EfiPciIoWidthUint32,
                     NVME_BAR0_INDEX,
                     Offset,
                     1,
                     &Data);
    } else {
        Data = (*NvmeMm32Ptr ((NvmeController->NvmeBarOffset), (Offset)));
    }
    return Data;
}
/**
    Write Read the Dword Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Offset       - Index/Offset address to Write
    @param    Data        - Data to be written        

    @retval    Nothing

**/
VOID
WriteDataDword(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset, 
    IN  UINTN   Data
)
{
    
    if( NvmeController == NULL ) {
        return;
    }
    
    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Write (
                             NvmeController->PciIO,
                             EfiPciIoWidthUint32,
                             NVME_BAR0_INDEX,
                             Offset,
                             1,
                             &Data);
    } else {
        (*NvmeMm32Ptr ((NvmeController->NvmeBarOffset), (Offset))) = ((UINT32) (Data));
    }

}

/**
    Read the Word Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Offset       - Index/Offset address to read           

    @retval Value Read

**/
UINT16
ReadDataWord(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset
)
{
    UINT16      Data = 0;
    
    if( NvmeController == NULL ) {
        return Data;
    }
    
    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Read (
                             NvmeController->PciIO,
                             EfiPciIoWidthUint16,
                             NVME_BAR0_INDEX,
                             Offset,
                             1,
                             &Data);
    } else {
        Data = (*NvmeMm16Ptr ((NvmeController->NvmeBarOffset), (Offset)));
    }
    
    return Data;
}

/**
    WriteRead the word Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Offset       - Index/Offset address to Write
    @param    Data        - Data to be written        

    @retval    Nothing

**/
VOID
WriteDataWord(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset, 
    IN  UINTN   Data
)
{
    
    if( NvmeController == NULL ) {
        return;
    }
    
    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Write (
                             NvmeController->PciIO,
                             EfiPciIoWidthUint16,
                             NVME_BAR0_INDEX,
                             Offset,
                             1,
                             &Data);
    } else {
        (*NvmeMm16Ptr ((NvmeController->NvmeBarOffset), (Offset))) = ((UINT16) (Data));
    }

}
/**
    Read the Byte Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Offset       - Index/Offset address to read           

    @retval    Value Read

**/
UINT8
ReadDataByte(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset
)
{
    UINT8       Data = 0;
    
    if( NvmeController == NULL ) {
        return Data;
    }

    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Read (
                             NvmeController->PciIO,
                             EfiPciIoWidthUint8,
                             NVME_BAR0_INDEX,
                             Offset,
                             1,
                             &Data);
    } else {
        Data = (*NvmeMm8Ptr ((NvmeController->NvmeBarOffset), (Offset)));
    }
    
    return Data;
}
/**
    WriteRead the Byte Data

    @param    NvmeController - Pointer to AMI_AHCI_BUS_PROTOCOL of AHCI Controller
    @param    Offset       - Index/Offset address to Write
    @param    Data        - Data to be written        

    @retval Nothing

**/
VOID
WriteDataByte(
    IN AMI_NVME_CONTROLLER_PROTOCOL   *NvmeController,
    IN  UINTN   Offset,
    IN  UINTN   Data
)
{
    
    if( NvmeController == NULL ) {
        return;
    }
    
    if( !NvmeController->NvmeInSmm && NvmeController->PciIO != NULL) {
        NvmeController->PciIO->Mem.Write (
                     NvmeController->PciIO,
                     EfiPciIoWidthUint8,
                     NVME_BAR0_INDEX,
                     Offset,
                     1,
                     &Data);
    } else {
        (*NvmeMm8Ptr ((NvmeController->NvmeBarOffset), (Offset))) = ((UINT8) (Data));
    }
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
