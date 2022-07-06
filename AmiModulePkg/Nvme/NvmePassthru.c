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

/** @file NvmePassthru.c
    Contain Nvme Express passthru protocol APIs definition

**/

//---------------------------------------------------------------------------
#include "NvmeIncludes.h"
#include "NvmeController.h"
#include "NvmeBus.h"
#include "Protocol/AmiNvmeController.h"
#include "NvmePassthru.h"
//---------------------------------------------------------------------------
/**
 * Verifies Get Log command's buffer length
 * 
 * @param Packet        A pointer to the EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET instance
 * 
 * @retval TRUE         Valid buffer length passed
 * @retval FALSE        Invalid buffer length passed
 */
BOOLEAN
ValidateGetLogBuffer (
    IN EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  *Packet
)
{
    UINTN   Numdl = 0;
    UINTN   NumOfBytes = 0;
    
    if(!Packet->TransferBuffer) return FALSE;
    
    // Calculating the number of Dwords
    if((Packet->NvmeCmd->Flags & CDW11_VALID) != CDW11_VALID){
        NumOfBytes = (((Packet->NvmeCmd->Cdw10 >>16) + 1) << 2);
        Numdl = Packet->NvmeCmd->Cdw10>>16;
    } else {
        // NUMDU
        Numdl = Packet->NvmeCmd->Cdw11;
        Numdl = Numdl << 16;
        //NUMDL
        Numdl = (Numdl | (Packet->NvmeCmd->Cdw10 >>16));
        NumOfBytes = (Numdl + 1) << 2;
    }
    
    if(Packet->TransferLength < NumOfBytes ){
        return FALSE;
    }
    return TRUE;
}

/**
    Sends an NVM Express Command Packet to an NVM Express controller or namespace

    @param This         A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance
    @param NamespaceId  A 32 bit namespace ID as defined in the NVMe specification 
                        to which the NVM Express Command Packet will be sent.  
    @param Packet       A pointer to the NVM Express Command Packet to send to
                        the NVMe namespace specified by NamespaceId. 
    @param Event        Event to be signaled when the NVM Express Command Packet completes
                        and non-blocking I/O is supported

    @retval EFI_STATUS

**/
EFI_STATUS 
EFIAPI
NvmePassThru ( 
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL            *This,
    IN UINT32                                        NamespaceId,
    IN OUT EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  *Packet,
    IN EFI_EVENT                                     Event
)
{
    EFI_STATUS                    Status;
    ACTIVE_NAMESPACE_DATA         *ActiveNameSpace = NULL;
    NVME_COMMAND_WRAPPER          *NvmeCmdWrapper = NULL;
    NVME_DXE_SMM_DATA_AREA        *NvmeDxeSmmDataArea = NULL;
    AMI_NVME_CONTROLLER_PROTOCOL  *NvmeController;
    UINT32                        AlignmentBoundry;
    COMPLETION_QUEUE_ENTRY        *NvmeCompletionData = NULL;
    UINT8                         NvmeOpcode;
    UINT64                        LBA = 0;
	UINT64                        *PRP2List = NULL; 
    UINTN                         BufferSize = 0;
    VOID                          *PRP2ListUnMap = NULL;
    EFI_PCI_IO_PROTOCOL_OPERATION IoOperation = EfiPciIoOperationBusMasterRead;
    EFI_PHYSICAL_ADDRESS          TransferBufferMappedAddr;
    VOID                          *TransferBufferUnMap = NULL;
    VOID                          *OrgTransferBuffer = NULL;
    UINTN                          OrgTransferLength;
    UINT8                         SanitizeAction;
    
    if( !This || !Packet || (Packet->TransferLength && !Packet->TransferBuffer) || !Packet->NvmeCmd || \
        (Packet->QueueType != NVME_ADMIN_SUBMISSION_QUEUE && Packet->QueueType != NVME_IO_SUBMISSION_QUEUE) || \
        (!Packet->NvmeCompletion)) {
        return EFI_INVALID_PARAMETER;
    }

    // Check Transfer buffer is NULL, if not,Check its memory is properly alligned or not
    if( Packet->TransferBuffer && ( This->Mode->IoAlign > 1 ) ) {

        //
        // IoAlign values of 0 and 1 mean that the buffer can be
        // placed anywhere in memory. Otherwise, IoAlign must be a
        // power of 2, and the requirement is that the start address of a
        // buffer must be evenly divisible by IoAlign with no remainder.
        //
        AlignmentBoundry = This->Mode->IoAlign;
        //
        //Get what is the number in Mode->IoAlign 
        //       
        if((0 !=  ((UINTN)((UINTN)(Packet->TransferBuffer) % AlignmentBoundry) ) ||
           (0 !=  ((UINTN)( Packet->TransferLength % AlignmentBoundry))) )) {
            return EFI_INVALID_PARAMETER;
        }
    }

    NvmeController = NVME_CONTROLLER_PROTOCOL_FROM_THIS(This);

    NvmeDxeSmmDataArea = NvmeController->NvmeDxeSmmDataArea;

    NvmeCompletionData = (COMPLETION_QUEUE_ENTRY*)Packet->NvmeCompletion;

    NvmeOpcode = (UINT8)Packet->NvmeCmd->Cdw0.Opcode;
    
    Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof(NVME_COMMAND_WRAPPER),
                  (VOID**)&NvmeCmdWrapper
                  );

    if (EFI_ERROR(Status)) {
        return Status;
    }
    // Clear memory
    SetMem(NvmeCmdWrapper, sizeof(NVME_COMMAND_WRAPPER), 0);
    
    // Fill Common values of command
    NvmeCmdWrapper->NvmCmd.CMD0.Opcode = NvmeOpcode;
    NvmeCmdWrapper->NvmCmd.CMD0.FusedOperation = Packet->NvmeCmd->Cdw0.FusedOperation;
    NvmeCmdWrapper->NvmCmd.CMD0.CommandIdentifier = \
            Packet->QueueType?  NvmeDxeSmmDataArea->CommandIdentifierQueue1 : NvmeDxeSmmDataArea->CommandIdentifierAdmin;
    NvmeCmdWrapper->NvmCmd.NSID = Packet->NvmeCmd->Nsid;
    NvmeCmdWrapper->NvmCmd.MPTR = 0;
    NvmeCmdWrapper->AdminOrNVMCmdSet = Packet->QueueType ? FALSE : TRUE;
    NvmeCmdWrapper->SQIdentifier = Packet->QueueType;           // Queue 0 for Admin cmds,1 for I/O command
    if (Packet->CommandTimeout) {
	    // If timeout is less than 1 millisecond, Assign Timeout value to 1 millisecond
        NvmeCmdWrapper->CmdTimeOut = (UINT32)(Packet->CommandTimeout < 10000 ? 1 : DivU64x32 (Packet->CommandTimeout, 10000));
    }
    if(Packet->NvmeCmd->Flags & CDW10_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW10 = Packet->NvmeCmd->Cdw10;
    }
    if(Packet->NvmeCmd->Flags & CDW11_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW11 = Packet->NvmeCmd->Cdw11;
    }
    if(Packet->NvmeCmd->Flags & CDW12_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW12 = Packet->NvmeCmd->Cdw12;
    }
    if(Packet->NvmeCmd->Flags & CDW13_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW13 = Packet->NvmeCmd->Cdw13;
    }
    if(Packet->NvmeCmd->Flags & CDW14_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW14 = Packet->NvmeCmd->Cdw14;
    }
    if(Packet->NvmeCmd->Flags & CDW15_VALID) {
        NvmeCmdWrapper->NvmCmd.CDW15 = Packet->NvmeCmd->Cdw15;
    }

    Status = EFI_INVALID_PARAMETER;
    

    if (Packet->TransferBuffer && Packet->TransferLength) { 
        // For read/write/compare map/unmap is called inside NvmeReadWriteBlocks()
        if (!((Packet->QueueType == NVME_IO_SUBMISSION_QUEUE) && (NvmeOpcode == NVME_READ || NvmeOpcode == NVME_WRITE || NvmeOpcode == NVME_COMPARE))) {   
            switch (NvmeOpcode & 3) {
                case 1:
                    IoOperation = EfiPciIoOperationBusMasterRead;
                    break;
                case 2:
                    IoOperation = EfiPciIoOperationBusMasterWrite;
                    break;
                case 3:
                    IoOperation = EfiPciIoOperationBusMasterCommonBuffer;
                    break;
            }    
            
            OrgTransferBuffer = Packet->TransferBuffer;
            OrgTransferLength = Packet->TransferLength;
           
            Status = NvmeController->PciIO->Map ( NvmeController->PciIO, 
                                        IoOperation,
                                        Packet->TransferBuffer,
                                        &OrgTransferLength,
                                        &TransferBufferMappedAddr,
                                        &TransferBufferUnMap
                                        );
            
            // If the entire TransferLength is not mapped give an error
            if (EFI_ERROR(Status)){
                return Status;
            }
            
            Packet->TransferBuffer = (void *)TransferBufferMappedAddr;
            Packet->TransferLength = (UINT32) OrgTransferLength;
        }
    }
    
    
	// if QueueType is 0, Admin Command
    // if QueueType is 1, I/O Command
    if ( Packet->QueueType == NVME_ADMIN_SUBMISSION_QUEUE ) 
    {
	    if( Packet->TransferBuffer ) {   
        
	        // Check the transfer size to MDTS value
	        if (NvmeController->IdentifyControllerData->MDTS && (Packet->TransferLength > \
	            ((UINTN)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) * NvmeController->MemoryPageSizeMin))) {
	            Packet->TransferLength = (UINT32)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) * NvmeController->MemoryPageSizeMin;
	            Status = EFI_BAD_BUFFER_SIZE;
			    goto PassThru_ErrorPath;
	        }  
        
	        // Program PRP1 and PRP2 entry
	        ActiveNameSpace = GetActiveNameSpace( NvmeController, -1 );          
	        Status = ProgramPRPEntry(NvmeController,
	                                NvmeCmdWrapper,
	                                Packet->TransferLength, 
	                                Packet->TransferBuffer,
	                                ActiveNameSpace,
	                                &PRP2List,
	                                &PRP2ListUnMap,
	                                &BufferSize); 
	        if( EFI_ERROR(Status) ) {
	            if( Status == EFI_BAD_BUFFER_SIZE ) 
	            	Packet->TransferLength = (UINT32)BufferSize;
	            goto PassThru_ErrorPath;
	        }
	    }
    

        switch(NvmeOpcode)
        {
            case DELETE_IO_SUBMISSION_QUEUE:
            case CREATE_IO_SUBMISSION_QUEUE:
            case DELETE_IO_COMPLETION_QUEUE:
            case CREATE_IO_COMPLETION_QUEUE:
            case ABORT:
            case ASYNC_EVENT_REQUEST:
            case DOORBELL_BUFFER_CONFIG:
                Status = EFI_UNSUPPORTED;
                break;
            case KEEP_ALIVE: 
                // Keep alive command is supported only from Nvme 1.2.1 spec
                if (NvmeController->ControllerVersion < NVME_SPEC_VS_1_2_1) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }                
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;                                            
            case DEVICE_SELF_TEST:
                // Device Self-test command is supported only if BIT4 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT4)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) { 
                    break;
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
                
            case NVMEMI_RECEIVE:  
                // NVMEMI_RECEIVE command is supported only if BIT6 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT6)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) { 
                    break;
                }
                if((Packet->TransferBuffer) && (Packet->TransferLength == 0)) {
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case FORMAT_NVM:
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) break;
                
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case SECURITY_SEND:
            case SECURITY_RECEIVE:
                // SECURITY_SEND and SECURITY_RECEIVE commands is supported only if BIT0 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT0)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }
                
                if(Packet->TransferLength!=Packet->NvmeCmd->Cdw11) {
                  Status = EFI_BAD_BUFFER_SIZE;
                  break;
                }                
                
                NvmeCmdWrapper->NvmCmd.CDW11 = (UINT32)BufferSize;  // BufferSize                
  
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;

            case DIRECTIVE_SEND:
            case DIRECTIVE_RECEIVE:
                // DIRECTIVE_SEND and DIRECTIVE_RECEIVE commands is supported only if BIT 5 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT5)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }
                if(Packet->TransferLength < ((Packet->NvmeCmd->Cdw10 + 1) << 2) ){
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case VIRTUALIZATION_MANAGEMENT:
                // VIRTUALIZATION_MANAGEMENT command is supported only if BIT 7 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT7)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }
                
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case NVMEMI_SEND:         
                // NVMEMI_SEND Command is supported only if BIT6 is set
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT6)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }
                if(Packet->TransferLength < ((Packet->NvmeCmd->Cdw11 + 1) << 2) ){
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case GET_FEATURES:               
            case SET_FEATURES:
                
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }                
                
                if((Packet->TransferBuffer) && (Packet->TransferLength ==0)) {
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);               
                break;
            case GET_LOG_PAGE:
                if(NvmeController->ControllerVersion < NVME_SPEC_VS_1_2_1){
                    if((Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID|CDW12_VALID|CDW13_VALID)) != CDW10_VALID) {
                        break;
                    }
                } else if((Packet->NvmeCmd->Flags & CDW10_VALID) != CDW10_VALID) {
                    break;
                }   
                if(ValidateGetLogBuffer(Packet)== TRUE) {
                    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                } else {
                    Status = EFI_BAD_BUFFER_SIZE;
                }
                break;
            case SANITIZE_CMD:
                Status = EFI_UNSUPPORTED;
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }   
                // sanitize action to perform(Block erase, Crypto erase, Overwrite, Exit Failure Mode)
                SanitizeAction = ((Packet->NvmeCmd->Cdw10) & (BIT0 | BIT1 | BIT2));
                
                // Check IdentifyControllerData (Sanitize Capabilities offset 331:328) to verify Sanitize command
                // is supported by the controller or not
                if((SanitizeAction == CRYPTO_ERASE_SANITIZE) && !((NvmeController->IdentifyControllerData->SANICAP) & BIT0)){
                    break;
                } else if((SanitizeAction == BLOCK_ERASE_SANITIZE) && !((NvmeController->IdentifyControllerData->SANICAP) & BIT1)){
                    break;
                } else if((SanitizeAction == OVERWRITE_SANITIZE) && !((NvmeController->IdentifyControllerData->SANICAP) & BIT2)){
                    break;
                } else if((SanitizeAction == EXIT_FAILURE_MODE) && !(NvmeController->IdentifyControllerData->SANICAP)){
                    break;
                } 
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case IDENTIFY:
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) break;
                
                if( Packet->TransferLength < sizeof(IDENTIFY_CONTROLLER_DATA) ) {
                    Packet->TransferLength = sizeof(IDENTIFY_CONTROLLER_DATA);
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }                
                
                if(Packet->TransferBuffer) {
                  Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                }
                break;
            case FIRMWARE_ACTIVATE:
                // Firmware activate command is supported only if BIT2 is set.
                if (!(NvmeController->IdentifyControllerData->OACS & BIT2)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) break;
                
                if(Packet->NvmeCmd->Nsid) break;

                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;              
            case FIRMWARE_IMAGE_DOWNLOAD:
                // Firmware image download command is supported only if BIT2 is set.
                if (!(NvmeController->IdentifyControllerData->OACS & BIT2)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                    (CDW10_VALID|CDW11_VALID) ) {
                        break;
                }
                
                if(Packet->NvmeCmd->Nsid) break;
                
                if(Packet->TransferBuffer) {
                    NvmeCmdWrapper->NvmCmd.CDW10 = (UINT32)(BufferSize>>2)-1;  // BufferSize in Dwords        
                    Status = ExecuteNvmeCmd (NvmeController, 
					                        NvmeCmdWrapper, 
                    		                (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);                                  
                }
                break;
				
            case NAMESPACE_MANAGEMENT:
                // NameSpace management command is supported only if BIT3 is set.
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT3)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) break;
                
                if( Packet->TransferLength != sizeof(IDENTIFY_NAMESPACE_DATA) ) {
                    Packet->TransferLength = sizeof(IDENTIFY_NAMESPACE_DATA);
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                                
                if( (NvmeCmdWrapper->NvmCmd.CDW10 == CREATE_NAMESPACE) || (NvmeCmdWrapper->NvmCmd.CDW10 == DELETE_NAMESPACE) ) 
                {                    
                    // if Command issued is for new NameSpace Creation,NSID value should be zero
                    if(NvmeCmdWrapper->NvmCmd.CDW10 == CREATE_NAMESPACE) {
                        if( NvmeCmdWrapper->NvmCmd.NSID || !Packet->TransferBuffer)  {
                            Status = EFI_INVALID_PARAMETER;
                            break;
                        }
                    }    

                    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                }
                break;
				
            case NAMESPACE_ATTACHMENT:
                // NameSpace attachment command is supported only BIT3 is set.
                if (!((NvmeController->IdentifyControllerData->OACS) & BIT3)) {
                    Status = EFI_UNSUPPORTED;
                    break;
                }
                if( !(Packet->NvmeCmd->Flags & CDW10_VALID) ) break;
                
                if( Packet->TransferLength != sizeof(NVME_CONTROLLER_LIST) ) {
                    Packet->TransferLength = sizeof(NVME_CONTROLLER_LIST);
                    Status = EFI_BAD_BUFFER_SIZE;
                    break;
                }
                
                if( (NvmeCmdWrapper->NvmCmd.CDW10 == CONTROLLER_ATTACH) || (NvmeCmdWrapper->NvmCmd.CDW10 == CONTROLLER_DETACH) )
                {
                    // NSID value should be an Active NameSpace ID value for Controller
                    // Detach Operation 
                    if(NvmeCmdWrapper->NvmCmd.CDW10 == CONTROLLER_DETACH) {
                        ActiveNameSpace = GetActiveNameSpace( NvmeController, Packet->NvmeCmd->Nsid );
                        if( !ActiveNameSpace ) break;
                    }
                    
                    if(Packet->TransferBuffer) {
                      Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                    }
                }                    			
                break;
				
            default:
                // Vendor Specific 
                if((NvmeOpcode >= 0xC0) && (NvmeOpcode <= 0xFF)) {                    
                    
                    //Check if Admin Vendor specific/NVMe vendor specific cmoond configuration
                    if((NvmeController->IdentifyControllerData->AVSCC == 1)&&(Packet->TransferLength!=0)){
					    if(Packet->TransferLength < (((Packet->NvmeCmd->Cdw10 >>16) + 1) << 2) ){
                            Status = EFI_BAD_BUFFER_SIZE;
                            break;
                        }
                    }                    
                    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                    break;
                }
        }

    } else {  // Send NVM command to Valid Namespace ID

        switch(NvmeOpcode)
        {
            case NVME_READ:
            case NVME_WRITE:
                if( (Packet->NvmeCmd->Flags & CDW13_VALID) != CDW13_VALID ) {
                        break;
                }
            case NVME_COMPARE:
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID|CDW12_VALID|CDW14_VALID|CDW15_VALID)) != 
                    (CDW10_VALID|CDW11_VALID|CDW12_VALID|CDW14_VALID|CDW15_VALID) ) {
                        break;
                }
				
                LBA = Packet->NvmeCmd->Cdw11;
                LBA = LShiftU64 (LBA, 32) | ( (UINT64)Packet->NvmeCmd->Cdw10 & 0x0FFFFFFFF );

                ActiveNameSpace = GetActiveNameSpace( NvmeController, Packet->NvmeCmd->Nsid );

                if( !ActiveNameSpace ) break;

                Status = NvmeReadWriteBlocks ( ActiveNameSpace,
                                               0,                                // MediaId = 0
                                               LBA,
                                               (UINT64)Packet->TransferLength,   // BufferSize,
                                               (VOID*)(Packet->TransferBuffer),  // Buffer,
                                               (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData,
                                                NvmeOpcode                       // ReadWriteOpCode
                                              );
                 break;
            case NVME_FLUSH:
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case NVME_DATASET_MANAGEMENT:
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID)) != 
                         (CDW10_VALID|CDW11_VALID) ) {
                    break;
                }
                if( Packet->TransferBuffer ) {                              
                   // Check the transfer size to MDTS value
                    if (NvmeController->IdentifyControllerData->MDTS && (Packet->TransferLength > \
                       ((UINTN)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) * NvmeController->MemoryPageSizeMin))) {
                       
                        Packet->TransferLength = (UINT32)LShiftU64(1, NvmeController->IdentifyControllerData->MDTS) \
                                                * (NvmeController->MemoryPageSizeMin);
                        Status = EFI_BAD_BUFFER_SIZE;
                        break;
                    }  
                           
                   // Program PRP1 and PRP2 entry
                   ActiveNameSpace = GetActiveNameSpace( NvmeController,Packet->NvmeCmd->Nsid);          
                   Status = ProgramPRPEntry(NvmeController,
                                           NvmeCmdWrapper,
                                           Packet->TransferLength, 
                                           Packet->TransferBuffer,
                                           ActiveNameSpace,
                                           &PRP2List,
                                           &PRP2ListUnMap,
                                           &BufferSize ); 
                   if( EFI_ERROR(Status)) {
                       if(Status == EFI_BAD_BUFFER_SIZE)  Packet->TransferLength = (UINT32)BufferSize;                          
                       break;
                   }
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;                
            case NVME_WRITE_UNCORRECTABLE:
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID|CDW12_VALID)) != 
                         (CDW10_VALID|CDW11_VALID|CDW12_VALID) ) {
                    break;
                }
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            case NVME_WRITE_ZEROES:                 
                if( (Packet->NvmeCmd->Flags & (CDW10_VALID|CDW11_VALID|CDW12_VALID|CDW14_VALID|CDW15_VALID)) != 
                    (CDW10_VALID|CDW11_VALID|CDW12_VALID|CDW14_VALID|CDW15_VALID) ) {
                        break;
                }                
                Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);
                break;
            default:
                
                if((NvmeOpcode >= 0x80) && (NvmeOpcode <= 0xFF)) {
                    
                    //Check if Admin Vendor specific/NVMe vendor specific cmoond configuration
                    if((NvmeController->IdentifyControllerData->AVSCC == 1)&&(Packet->TransferLength!=0)){
					    if(Packet->TransferLength < (((Packet->NvmeCmd->Cdw10 >>16) + 1) << 2) ){
                            Status = EFI_BAD_BUFFER_SIZE;
                            break;
                        }
                    }                                        
                    Status = ExecuteNvmeCmd (NvmeController, NvmeCmdWrapper, (COMPLETION_QUEUE_ENTRY*)NvmeCompletionData);

                }
                break;
         }
    }
    
PassThru_ErrorPath:

    gBS->FreePool ( NvmeCmdWrapper );

    // If no ActiveNameSpace is found and PRP2List is programmed, then free the memory
    if( !ActiveNameSpace && PRP2List) {
        // Free up all the memory 
        NvmeController->PciIO->Unmap( NvmeController->PciIO, 
                                      PRP2ListUnMap );

        NvmeController->PciIO->FreeBuffer(NvmeController->PciIO,
                                     EFI_SIZE_TO_PAGES(NvmeController->MemoryPageSize),\
                                     PRP2List );
    }
    
    //IOMMU_Support
    if (TransferBufferUnMap){
        NvmeController->PciIO->Unmap( NvmeController->PciIO, 
                        TransferBufferUnMap );    
    }
    if (OrgTransferBuffer) {
        Packet->TransferBuffer = OrgTransferBuffer;
    }
    
    return Status;
}

/**
    API to get next valid NameSpace ID of the Nvme Device

    @param  This         A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance
    @param  NamespaceId  A pointer to a valid Namespace ID on this NVM Express controller 

    @retval EFI_STATUS

**/

EFI_STATUS
EFIAPI
GetNextNamespace(
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *This,
    IN OUT UINT32                            *NamespaceId
)
{
    AMI_NVME_CONTROLLER_PROTOCOL *NvmeController;
    ACTIVE_NAMESPACE_DATA        *ActiveNameSpaceData;
    LIST_ENTRY                   *LinkData;
    BOOLEAN                      ReturnNextNameSpaceId = FALSE;

    if(!NamespaceId || !This) {
        return EFI_INVALID_PARAMETER;
    }
    if( 0xFFFFFFFF == *NamespaceId) {
        ReturnNextNameSpaceId = TRUE;
    }

    NvmeController = NVME_CONTROLLER_PROTOCOL_FROM_THIS(This);

    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {
        
        ActiveNameSpaceData = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);

        if( TRUE == ReturnNextNameSpaceId) {
            *NamespaceId = ActiveNameSpaceData->ActiveNameSpaceID;
            return EFI_SUCCESS;
        }
        
        if(ActiveNameSpaceData->ActiveNameSpaceID == *NamespaceId) {
            ReturnNextNameSpaceId = TRUE;
        }
    }
    if( ReturnNextNameSpaceId ) {
        return EFI_NOT_FOUND;
    }

    return EFI_INVALID_PARAMETER;

}

/**
    Routine Searches device path by specific Type and SubType

    @param DevicePath A pointer to the device path protocol
    @param Type       Device path Node Type
    @param SubType    Device path node subtype

    @retval EFI_DEVICE_PATH_PROTOCOL*

**/

EFI_DEVICE_PATH_PROTOCOL*
SearchDevicePathNode (
  IN    OUT     EFI_DEVICE_PATH_PROTOCOL *DevicePath,
  IN    UINT8   Type,
  IN    UINT8   SubType
)
{
    if (DevicePath == NULL) {
        return NULL;
    }

    while (!IsDevicePathEnd (DevicePath)) {
        if ((DevicePathType (DevicePath) == Type) && (DevicePathSubType (DevicePath) == SubType)) {
            return DevicePath;
        }
        DevicePath = NextDevicePathNode (DevicePath);
    }

    return NULL;
}

/**
    Used to allocate and build a device path node for an NVM Express namespace 
    on an NVM Express controller

    @param This         A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance
    @param NamespaceId  The NVM Express namespace ID for which a device path node is
                        to be allocated and built
    @param DevicePath   A pointer to a single device path node that describes the 
                        NVM Express namespace specified by NamespaceId. 

    @retval EFI_STATUS
**/
EFI_STATUS 
EFIAPI
BuildDevicePath (
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *This,
    IN UINT32                                NamespaceId,
    IN OUT EFI_DEVICE_PATH_PROTOCOL          **DevicePath
)
{
    EFI_STATUS                   Status;
    AMI_NVME_CONTROLLER_PROTOCOL *NvmeController;
    LIST_ENTRY                   *LinkData;
    ACTIVE_NAMESPACE_DATA        *ActiveNameSpaceData;
    EFI_DEVICE_PATH_PROTOCOL     *DevicePathProtocol;
    NVME_DEVICE_PATH              *NvmeDevicePathNode = NULL;
    
    if( !DevicePath || !This ) {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->AllocatePool( EfiBootServicesData, 
                                sizeof(NVME_DEVICE_PATH),
                                (void**)DevicePath );
    if(EFI_ERROR(Status)) {
        return EFI_OUT_OF_RESOURCES;
    }       
    
    NvmeController = NVME_CONTROLLER_PROTOCOL_FROM_THIS(This);
    
    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {
        
        ActiveNameSpaceData = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
        
        if(ActiveNameSpaceData->ActiveNameSpaceID == NamespaceId) {

            Status = gBS->HandleProtocol (
                        ActiveNameSpaceData->NvmeDeviceHandle,
                        &gEfiDevicePathProtocolGuid,
                        (VOID**)&DevicePathProtocol );

            if (EFI_ERROR(Status)) {
                return EFI_NOT_FOUND;
            }

             NvmeDevicePathNode = (NVME_DEVICE_PATH*)SearchDevicePathNode ( DevicePathProtocol, MESSAGING_DEVICE_PATH, MSG_NVME_DP);
            gBS->CopyMem( (UINT8*)*DevicePath, (UINT8*)NvmeDevicePathNode, sizeof(NVME_DEVICE_PATH) );
 
            return EFI_SUCCESS;
        }
    }
    
    return EFI_NOT_FOUND;
}

/**
    Used to translate a device path node to a Namespace ID

    @param This         A pointer to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL instance
    @param DevicePath   A pointer to the device path node that describes an NVM 
                        Express namespace on the NVM Express controller. 
    @param NamespaceId  The NVM Express namespace ID contained in the device path node

    @retval EFI_STATUS

**/
EFI_STATUS 
EFIAPI
GetNamespace (
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *This,
    IN EFI_DEVICE_PATH_PROTOCOL              *DevicePath,
    OUT UINT32                               *NamespaceId
)
{
    UINT16                       DevicePathLength = 0;
    AMI_NVME_CONTROLLER_PROTOCOL *NvmeController;
    LIST_ENTRY                   *LinkData;
    ACTIVE_NAMESPACE_DATA        *ActiveNameSpaceData;
    
    if(!DevicePath || !This || !NamespaceId) {
        return EFI_INVALID_PARAMETER;
    }

    DevicePathLength = (DevicePath)->Length[0] | (DevicePath)->Length[1] << 8;

    if ((DevicePath->Type != MESSAGING_DEVICE_PATH) || 
        (DevicePath->SubType != MSG_NVME_DP) ||
        (DevicePathLength != sizeof(NVME_DEVICE_PATH))) {
        return EFI_UNSUPPORTED;
    }
    
    NvmeController = NVME_CONTROLLER_PROTOCOL_FROM_THIS(This);
        
    for (LinkData = NvmeController->ActiveNameSpaceList.ForwardLink; 
            LinkData != &NvmeController->ActiveNameSpaceList; 
            LinkData = LinkData->ForwardLink) {
        
        ActiveNameSpaceData = BASE_CR(LinkData ,ACTIVE_NAMESPACE_DATA, Link);
        
        if(ActiveNameSpaceData->ActiveNameSpaceID == ((NVME_DEVICE_PATH* )DevicePath)->Nsid) {
            *NamespaceId = ((NVME_DEVICE_PATH* )DevicePath)->Nsid;
            return EFI_SUCCESS;
        }
    }

    return EFI_NOT_FOUND;
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
