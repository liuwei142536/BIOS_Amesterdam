//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************


/** @file 
    Interface to a subset of EFI Framework protocols using 
    legacy interfaces that will allow external software to access EFI 
    protocols in a legacy environment.

**/


//---------------------------------------------------------------------------

#include <AmiDxeLib.h>
#include <AmiSmm.h>
#include <Token.h>
#include <AmiCspLib.h>
#include "SmiVariable.h"
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmCpu.h>
#include <Library/AmiBufferValidationLib.h>
#include <SmiVarElink.h>

#include <Protocol/SmmAccess2.h>
#include <Library/MemoryAllocationLib.h>

typedef EFI_STATUS (HOOK_SMI_VARIABLE)(
    IN REG_BLOCK* RegBlkPtr
);

extern HOOK_SMI_VARIABLE SMI_VAR_LIST EndOfSmiVariableHook;

HOOK_SMI_VARIABLE* SmiVariableHookList[]=
    {SMI_VAR_LIST NULL};

EXPORT_HIIDB_BLOCK ExportHiiDbBlock = {0,0};

typedef struct {
    EFI_PHYSICAL_ADDRESS Address;
    UINT64 Size;
} SMM_AMI_BUFFER_VALIDATION_LIB_ADDRESS_RANGE;

SMM_AMI_BUFFER_VALIDATION_LIB_ADDRESS_RANGE *gValidationSmramRanges = NULL;
UINTN gValidationNumberOfSmramRanges = 0;

/**
    Returns size of validated string, which can not be bigger than size of NVRAM.

         
    @param String Pointer to the beginning of string.
	@param Size Pointer to where size will be returned.
          
    @retval Status Based on Verification Result

**/
EFI_STATUS GetVarNameSize(IN CHAR16 *String, UINTN *Size)
{
	CHAR16 *Str, *EndOfStr;
	UINTN MaxSize = NVRAM_SIZE;
	EFI_STATUS Status = EFI_SUCCESS;
			
	ASSERT(String != NULL);
	if (Size == NULL)
		return EFI_INVALID_PARAMETER;
	
	if (String == NULL) 
	{	
		*Size = 0;
		return Status;
	}
	EndOfStr = (CHAR16*)((UINT8*)String + MaxSize);
	if (EndOfStr < String)
	{
		EndOfStr = (CHAR16*)MAX_UINTN;
	}
	for(Str = String; Str < EndOfStr; Str++)
	{	
		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)Str, sizeof(CHAR16));
		if (EFI_ERROR(Status))
		{
			*Size = 0;
		    return Status;
		}
		if (!*Str) break;
	}
	*Size = (Str - String + 1)*sizeof(CHAR16);
	return Status;
}

/**
    Executes hooks, registered on SmiVariableHook parent eLink.

         
    @param RegBlkPtr Structure, which stores values of registers.

          
    @retval EFI_UNSUPPORTED Should continue to execute handler
        Everything else     -  execution of handler will be terminated and this status will be returned to the caller!

**/
EFI_STATUS SmiVariableHook(
      IN REG_BLOCK* RegBlkPtr
){
    UINTN i;
    EFI_STATUS Result = EFI_UNSUPPORTED;
    for(i=0; SmiVariableHookList[i] && (Result == EFI_UNSUPPORTED); i++) 
        Result = SmiVariableHookList[i](RegBlkPtr);
    return Result;
}

/**
    Check destination address range if it's cleared by zero.

    @param Address - starting address
    @param Range   - length of the area

          
    @retval EFI_SUCCESS Access granted
    @retval EFI_ACCESS_DENIED Access denied!

**/

EFI_STATUS CheckDestinationMemoryRange( IN UINT8 *Address, IN UINTN Range )
{
    while (Range--)
        if (*Address++)
            return EFI_ACCESS_DENIED;
    return EFI_SUCCESS;
}


/**
    Check source address range if it's not out of the HiiDb space.

    @param Address - starting address
    @param Range   - length of the area

          
    @retval EFI_SUCCESS Access granted
    @retval EFI_ACCESS_DENIED Access denied!

    @note  Remove this function after COPY_MEMORY_SUBFUNCTION will be removed.

**/

EFI_STATUS CheckAddressRangeHiiDb( IN UINT8 *Address, IN UINTN Range )
{
    EFI_STATUS Status;
    EFI_GUID ExportDatabaseGuid = EFI_HII_EXPORT_DATABASE_GUID;
    UINTN  DataSize = sizeof(EXPORT_HIIDB_BLOCK);
    UINT32 Attributes;

    if (ExportHiiDbBlock.Data == 0 || ExportHiiDbBlock.DataSize == 0)
    {// Get HII DB variable
    	Status = pRS->GetVariable(
    			L"HiiDB",
    			&ExportDatabaseGuid,
    			&Attributes,
    			&DataSize,
    			&ExportHiiDbBlock
    	);
    	if (EFI_ERROR(Status))
    		return EFI_ACCESS_DENIED;
    }
    // Check the size and range
    if ( (ExportHiiDbBlock.Data == 0) || (ExportHiiDbBlock.DataSize == 0) ||
	     (Range > ExportHiiDbBlock.DataSize) ||
    	 ((UINT32)Address + Range < (UINT32)Address) || 
         ((UINT32)Address < ExportHiiDbBlock.Data) || 
         ( ((UINT32)Address + Range) > (ExportHiiDbBlock.Data + ExportHiiDbBlock.DataSize)) )
        return EFI_ACCESS_DENIED;

    if (gValidationNumberOfSmramRanges == 0 || gValidationSmramRanges == NULL)
        return EFI_ACCESS_DENIED;
    
    {
    	UINTN  i;
    	UINTN BufferAddress;
    	
    		
    	BufferAddress = (UINTN)Address;

    	for (i = 0; i < gValidationNumberOfSmramRanges; i ++) {
    	        if (    BufferAddress >= gValidationSmramRanges[i].Address
    	             && BufferAddress < gValidationSmramRanges[i].Address + gValidationSmramRanges[i].Size
    	        ) return EFI_ACCESS_DENIED; // Buffer starts in SMRAM
    	        if (    BufferAddress < gValidationSmramRanges[i].Address
    	             && BufferAddress+Range > gValidationSmramRanges[i].Address
    	        ) return EFI_ACCESS_DENIED; // Buffer overlaps with SMRAM
    	}
    	
    }
    
    return EFI_SUCCESS;
}


/**
    Calling the SMI Interface. The caller will write AL (the value 0xef) to 
    the SMI Command Port as defined in the ACPI FADT. The SMI handler will 
    update the callers' buffer(s) and return.

    @param DispatchHandle
    @param Context
    @param CommBuffer
    @param CommBufferSize

    @retval VOID

    @note  
 The function will clear the carry bit if it is successful (CF = 0). 
 If the function is unsuccessful, it will set the carry bit and set the 
 error code in the AH register as indicated by the error table below.
 The function returns the following data in the provided parameter block. 

**/

EFI_STATUS EFIAPI VariableInterface( 
    IN EFI_HANDLE DispatchHandle,
    IN CONST VOID  *Context OPTIONAL,
    IN OUT   VOID  *CommBuffer OPTIONAL,
    IN OUT   UINTN *CommBufferSize OPTIONAL )
{
    EFI_STATUS Status;
    REG_BLOCK* RegBlockPtr = NULL;
    VARIABLE_BLOCK *VariableBlock = NULL;
    GET_NEXT_VARIABLE_BLOCK *GetNextVariableBlock = NULL;
    UINTN  DataSize;
    EFI_SMM_SW_CONTEXT *SmmSwContext = ((EFI_SMM_SW_CONTEXT*)CommBuffer);
    EFI_GUID EfiSmmCpuProtocolGuid = EFI_SMM_CPU_PROTOCOL_GUID;
    EFI_SMM_CPU_PROTOCOL *SmmCpuProtocol = NULL;
    EFI_PHYSICAL_ADDRESS Register = 0;

    Status = pSmst->SmmLocateProtocol(
        &EfiSmmCpuProtocolGuid,
        NULL, 
        (VOID**)&SmmCpuProtocol 
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    // Read the RSI register
    Status = SmmCpuProtocol->ReadSaveState( 
        SmmCpuProtocol,
        sizeof(UINT32), 
        EFI_SMM_SAVE_STATE_REGISTER_RSI,
        SmmSwContext->SwSmiCpuIndex,
        &Register
    );
    if (EFI_ERROR(Status))
    	return EFI_SUCCESS;
    if (EFI_ERROR(AmiValidateMemoryBuffer((VOID*)Register, sizeof(REG_BLOCK))))
    	return EFI_SUCCESS;

    RegBlockPtr = (REG_BLOCK*)Register;
    
    Status = SmiVariableHook (RegBlockPtr);
    if (Status == EFI_UNSUPPORTED){        
    
    	switch((RegBlockPtr->EAX & 0xff00) >> 8) {

        	case GET_VARIABLE_SUBFUNCTION: 
        		// Address of the VARIABLE_BLOCK parameter block
        		VariableBlock = (VARIABLE_BLOCK*)(UINTN)RegBlockPtr->EBX;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)VariableBlock, sizeof(VARIABLE_BLOCK));
        		if (EFI_ERROR(Status)) break;
        		
        		//Get the size of VariableName and check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = GetVarNameSize((CHAR16*)(UINTN)VariableBlock->VariableName, &DataSize);
        		
        		if (EFI_ERROR(Status)) break;
        		
        		DataSize = VariableBlock->DataSize;
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)VariableBlock->Data, DataSize);
        		if (!EFI_ERROR(Status)) {
        			// check destination address range if it's cleared by zero
        			Status = CheckDestinationMemoryRange((UINT8*)(UINTN)VariableBlock->Data, DataSize);
        			if (!EFI_ERROR(Status)) {
        				Status = pRS->GetVariable( 
        						(CHAR16*)(UINTN)VariableBlock->VariableName, 
        						&VariableBlock->VendorGuid, 
        						&VariableBlock->Attributes, 
        						&DataSize, 
        						(VOID*)(UINTN)VariableBlock->Data 
        				);
        				VariableBlock->DataSize = (UINT32)DataSize;
        			}
        		}
        	break;

        	case GET_NEXT_VARIABLE_SUBFUNCTION:
        		// Address of the GET_NEXT_VARIABLE_BLOCK parameter block
        		GetNextVariableBlock = (GET_NEXT_VARIABLE_BLOCK*)(UINTN)RegBlockPtr->EBX;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)GetNextVariableBlock, sizeof(GET_NEXT_VARIABLE_BLOCK));
        		if (EFI_ERROR(Status)) break;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)GetNextVariableBlock->VariableName, GetNextVariableBlock->VariableNameSize);
        		if (EFI_ERROR(Status)) break;
        		
        		Status = GetVarNameSize((CHAR16*)(UINTN)GetNextVariableBlock->VariableName, &DataSize);
        				
        		if (DataSize > GetNextVariableBlock->VariableNameSize) 
        			Status = EFI_INVALID_PARAMETER;
     
        		if (!EFI_ERROR(Status)) {
        			Status = pRS->GetNextVariableName( 
        					&DataSize,
        					(CHAR16*)(UINTN)GetNextVariableBlock->VariableName,
        					&GetNextVariableBlock->VendorGuid 
        			);
        			GetNextVariableBlock->VariableNameSize = (UINT32)DataSize;
        		}
        	break;

        	case SET_VARIABLE_SUBFUNCTION:
        		// Address of the VARIABLE_BLOCK parameter block
        		VariableBlock = (VARIABLE_BLOCK*)(UINTN)RegBlockPtr->EBX;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)VariableBlock, sizeof(VARIABLE_BLOCK));
        		if (EFI_ERROR(Status)) break;
        		
        		//Get the size of VariableName
        		Status = GetVarNameSize((CHAR16*)(UINTN)VariableBlock->VariableName, &DataSize);
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)

        		if (EFI_ERROR(Status)) break;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
                if (VariableBlock->DataSize != 0)//we can't validate zero size buffer
                {
                    Status = AmiValidateMemoryBuffer((VOID*)(UINTN)VariableBlock->Data, VariableBlock->DataSize);
                    if (!EFI_ERROR(Status))
                        Status = pRS->SetVariable( 
                                (CHAR16*)(UINTN)VariableBlock->VariableName, 
                                &VariableBlock->VendorGuid, 
                                VariableBlock->Attributes, 
                                VariableBlock->DataSize, 
                                (VOID*)(UINTN)VariableBlock->Data 
                        );
                }
                else
                {
                    Status = pRS->SetVariable( 
                             (CHAR16*)(UINTN)VariableBlock->VariableName, 
                              &VariableBlock->VendorGuid, 
                              VariableBlock->Attributes, 
                              VariableBlock->DataSize, 
                              NULL 
                    );
                }
        	break;

        	// Remove this sub-function after Utility will be able do not use it for Export HII DB!!!
        	case COPY_MEMORY_SUBFUNCTION:
        	{
        		// Address of the COPY_MEMORY_BLOCK parameter block
        		COPY_MEMORY_BLOCK *CopyMemBlock = (COPY_MEMORY_BLOCK*)(UINTN)RegBlockPtr->EBX;
        		
        		// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        		Status = AmiValidateMemoryBuffer((VOID*)(UINTN)CopyMemBlock, sizeof(COPY_MEMORY_BLOCK));
        		if (EFI_ERROR(Status)) break;
        		
        		if ( CopyMemBlock->Size == 0 )
                Status = EFI_INVALID_PARAMETER;
        		else {

        			// Check source address range if it's not out of the HiiDb space
        			Status = CheckAddressRangeHiiDb((UINT8*)(UINTN)CopyMemBlock->SrcAddr, CopyMemBlock->Size);
        			if (!EFI_ERROR(Status)) {
        				
        				// check address range to avoid TSEG area (return EFI_ACCESS_DENIED)
        				Status = AmiValidateMemoryBuffer((VOID*)(UINTN)CopyMemBlock->DestAddr, CopyMemBlock->Size);
        				if (!EFI_ERROR(Status)) {

        					// check destination address range if it's cleared by zero
        					Status = CheckDestinationMemoryRange((UINT8*)(UINTN)CopyMemBlock->DestAddr, CopyMemBlock->Size);
        					if (!EFI_ERROR(Status)) {
        						MemCpy( 
        								(UINT8*)(UINTN)CopyMemBlock->DestAddr, 
        								(UINT8*)(UINTN)CopyMemBlock->SrcAddr, 
        								CopyMemBlock->Size
        						);
        						Status = EFI_SUCCESS;
        						
        					}
        				}
        			}
        		}
        	}
        	break;

        	default: 
        		Status = EFI_INVALID_PARAMETER;
        	break;
    	}
    }

    RegBlockPtr->EAX &= 0xffff0000; 

    if (EFI_ERROR(Status)) {

        RegBlockPtr->EFLAGS |= CARRY_FLAG;  // set CF

        // Convertation of the EFI Status to standard INT 0x15 error codes.
        RegBlockPtr->EAX |= (Status | 0x80) << 8; 

    } else
        RegBlockPtr->EFLAGS &= ~CARRY_FLAG; // clear CF

    return EFI_SUCCESS;
}


/**
    Registration of the SMI function

    @param ImageHandle - Image handle
    @param SystemTable - Pointer to the system table

    @retval EFI_STATUS

**/

EFI_STATUS InSmmFunction(
    IN EFI_HANDLE ImageHandle, 
    IN EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status;
    EFI_HANDLE Handle = 0;
    EFI_GUID EfiSmmSwDispatchProtocolGuid = EFI_SMM_SW_DISPATCH2_PROTOCOL_GUID;
    EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
    EFI_SMM_ACCESS2_PROTOCOL *SmmAccess;
    EFI_SMRAM_DESCRIPTOR *SmramRanges;
    UINTN Size;
    UINTN i;
          
    Status = pSmst->SmmLocateProtocol(
        &EfiSmmSwDispatchProtocolGuid, 
        NULL, 
        (VOID**)&SwDispatch 
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    SwContext.SwSmiInputValue = FUNCTION_NUMBER;
    Status = SwDispatch->Register( 
        SwDispatch, 
        VariableInterface, 
        &SwContext, 
        &Handle 
    );
    ASSERT_EFI_ERROR(Status);
    
    if (EFI_ERROR(Status))
    	return Status;

    // Get SMRAM information
        Status = pBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **)&SmmAccess);
        if (EFI_ERROR(Status)) return EFI_SUCCESS;
        
        Size = 0;
        Status = SmmAccess->GetCapabilities (SmmAccess, &Size, NULL);
        ASSERT (Status == EFI_BUFFER_TOO_SMALL);
        if (Status != EFI_BUFFER_TOO_SMALL) return Status;
        Status = pBS->AllocatePool (EfiBootServicesData, Size, (VOID **)&SmramRanges);
        ASSERT_EFI_ERROR(Status);
        if (EFI_ERROR(Status)) return EFI_SUCCESS;

        Status = SmmAccess->GetCapabilities (SmmAccess, &Size, SmramRanges);
        ASSERT_EFI_ERROR (Status);
        if (EFI_ERROR(Status)){
        	pBS->FreePool(SmramRanges);
        	return EFI_SUCCESS;
        }
        gValidationNumberOfSmramRanges = Size / sizeof (EFI_SMRAM_DESCRIPTOR);
        gValidationSmramRanges = AllocatePool (
                gValidationNumberOfSmramRanges * sizeof(SMM_AMI_BUFFER_VALIDATION_LIB_ADDRESS_RANGE)
        );
        ASSERT (gValidationSmramRanges != NULL);
        if (gValidationSmramRanges == NULL){
            gValidationNumberOfSmramRanges = 0;
            pBS->FreePool(SmramRanges);
            return EFI_SUCCESS;
        }
        for(i=0; i<gValidationNumberOfSmramRanges; i++){
            gValidationSmramRanges[i].Address = SmramRanges[i].CpuStart;
            gValidationSmramRanges[i].Size = SmramRanges[i].PhysicalSize;
        }
        pBS->FreePool(SmramRanges);

    return Status;
}


/**
    Entry point of the SMI variable interface
 
    @param ImageHandle - Image handle
    @param SystemTable - Pointer to the system table

    @retval EFI_STATUS

**/

EFI_STATUS SmiVariableEntryPoint(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status;

    InitAmiLib( ImageHandle, SystemTable );

    // Initialize the SMM handler 
    Status = InitSmmHandler( 
        ImageHandle, 
        SystemTable, 
        InSmmFunction, 
        NULL
    );
    ASSERT_EFI_ERROR(Status);

    return Status;
}

//****************************************************************************
//****************************************************************************
//**                                                                        **
//**             (C)Copyright 1985-2019, American Megatrends, Inc.          **
//**                                                                        **
//**                          All Rights Reserved.                          **
//**                                                                        **
//**             5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093          **
//**                                                                        **
//**                          Phone (770)-246-8600                          **
//**                                                                        **
//****************************************************************************
//****************************************************************************
