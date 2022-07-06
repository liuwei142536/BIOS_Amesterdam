//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
//*****************************************************************************
/** @file OA3_SMM.c
    This part of code allows to update ACPI in the OS Runtime through 
    the SW SMI for OEM Activation without rebooting the system.

**/
//*****************************************************************************


#include "OA3.h"
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmCpu.h>
#include <Protocol/SmmVariable.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/AmiBufferValidationLib.h>
#if OEM_ACTIVATION_TABLE_LOCATION == 2
#include <OA3SmmTableAddressElink.h>
#endif

#if OEM_ACTIVATION_TABLE_LOCATION == 2
// Type Definition(s)
typedef VOID (OA3_SMM_TABLE_ADDRESS_FUNC) (  
    OUT EFI_PHYSICAL_ADDRESS *OA3TableAddress);

// External Declaration(s)
extern OA3_SMM_TABLE_ADDRESS_FUNC OA3_SMM_TABLE_ADDRESS_ELINK EndOfList;

OA3_SMM_TABLE_ADDRESS_FUNC* OA3SmmTableAddressElink[] = \
                                            {OA3_SMM_TABLE_ADDRESS_ELINK NULL};
#endif


/**
    This function calculates a checksum of table starting at 
    TblStart of length BytesCount and returns the checksum value.

    @param 
        *TblStart   - Starting address of the memory area to checksum.
        BytesCount  - Length in bytes of the memory area to checksum.

    @retval 
        Checksum value starting from TblStart and ending at TblStart + BytesCount.

**/

UINT8 ChsumTbl( IN UINT8* TblStart, IN UINT32 BytesCount )
{
    UINT8  Result = *TblStart;
    UINTN  i;
    
    for ( i = 1; i < BytesCount; i++ )  
        Result += TblStart[i];
    return 0 - Result;
}


/**
    This function will update ACPI table for OA3.

    @param ProductKey - pointer to the Product Key

    @retval EFI_STATUS

**/

EFI_STATUS Oa3UpdateAcpiTable( IN UINT8 *ProductKey )
{
    EFI_STATUS Status;
    EFI_OA3_MSDM_STRUCTURE MsdmVariable = {0};
    EFI_GUID AmiGlobalVariableGuid = AMI_GLOBAL_VARIABLE_GUID;
    UINTN  Size = sizeof(EFI_OA3_MSDM_STRUCTURE);
    EFI_ACPI_MSDM_TABLE *MsdmTable;
    EFI_ACPI_DESCRIPTION_HEADER *AcpiHdr;
    UINT8 *Ptr;
    EFI_PHYSICAL_ADDRESS *AcpiPtr;
#if OEM_ACTIVATION_TABLE_LOCATION == 2
    EFI_PHYSICAL_ADDRESS OA3TableAddress = 0;
	UINTN  i;
#endif
    UINT8 TblChksum = 0;
    EFI_SMM_VARIABLE_PROTOCOL *SmmVariable;

    if (ProductKey == NULL)
        return EFI_INVALID_PARAMETER;

// AptioV Server Override Start: EIP#484584 - SA50072 Security Fix
    Status = AmiValidateMemoryBuffer((VOID*)ProductKey, (sizeof(EFI_ACPI_MSDM_TABLE) - sizeof(EFI_ACPI_DESCRIPTION_HEADER)));
    if (EFI_ERROR(Status))
        return Status;

    Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**) &SmmVariable);
    ASSERT_EFI_ERROR (Status);
    Status = SmmVariable->SmmGetVariable (
                                EFI_OA3_MSDM_VARIABLE,
                                &AmiGlobalVariableGuid,
                                NULL,
                                &Size,
                                &MsdmVariable
                               );
    DEBUG ((DEBUG_INFO, "Oa3UpdateAcpiTable: XsdtAddr=%X, MsdmAddr=%X\n",MsdmVariable.XsdtAddress,MsdmVariable.MsdmAddress));
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    if ( MsdmVariable.XsdtAddress == 0 || MsdmVariable.MsdmAddress == 0 )
        return EFI_INVALID_PARAMETER;

    //Check ACPI header range is in legal memory.
    Status = AmiValidateMemoryBuffer((VOID*)MsdmVariable.XsdtAddress, sizeof(EFI_ACPI_DESCRIPTION_HEADER));
    if (EFI_ERROR(Status)) 
        return Status;

    //Check MSDM table range is in legal memory.
    Status = AmiValidateMemoryBuffer((VOID*)MsdmVariable.MsdmAddress, sizeof(EFI_ACPI_MSDM_TABLE));
    if (EFI_ERROR(Status)) 
        return Status;

    // Validate MSDM table signature 
    MsdmTable = (EFI_ACPI_MSDM_TABLE*)(UINTN)MsdmVariable.MsdmAddress;
    if (MsdmTable->Header.Signature != MSDM_SIG)
        return EFI_INVALID_PARAMETER;
    
    // Validate XSDT table signature 
    AcpiHdr = (EFI_ACPI_DESCRIPTION_HEADER*)(UINTN)MsdmVariable.XsdtAddress;
    if (AcpiHdr->Signature != XSDT_SIG)
        return EFI_INVALID_PARAMETER;
    
    // XSDT table length should larger than standard ACPI description header size.
    if (AcpiHdr->Length < sizeof(EFI_ACPI_DESCRIPTION_HEADER))
        return EFI_INVALID_PARAMETER;
    
    //Check XSDT table and all other description table entry is in legal memory.
    Status = AmiValidateMemoryBuffer((VOID*)AcpiHdr, (UINTN)AcpiHdr->Length);
    if (EFI_ERROR(Status)) 
        return Status;
// AptioV Server Override End: EIP#484584 - SA50072 Security Fix

    
	// Validate XSDT table checksum
	TblChksum = AcpiHdr->Checksum;
	AcpiHdr->Checksum = 0;
	AcpiHdr->Checksum = ChsumTbl((UINT8*)AcpiHdr, AcpiHdr->Length);
	
	// Do not update XSDT table if checksum invalid
	if (AcpiHdr->Checksum != TblChksum){
		AcpiHdr->Checksum = TblChksum;
		return EFI_INVALID_PARAMETER;
	}
	
#if OEM_ACTIVATION_TABLE_LOCATION == 2
    if (OA3SmmTableAddressElink[0] == NULL) return EFI_INVALID_PARAMETER;
    
    for (i = 0; OA3SmmTableAddressElink[i] != NULL; i++) {
        OA3SmmTableAddressElink[i](&OA3TableAddress);
    }
#endif

    // Copy a new Product Key to MSDM Data array
    CopyMem( 
        (UINT8*)&MsdmTable->Version,
#if OEM_ACTIVATION_TABLE_LOCATION == 0
        ProductKey,
#elif OEM_ACTIVATION_TABLE_LOCATION == 1
        (UINT8*)OEM_ACTIVATION_TABLE_ADDRESS,   // Copy Key from the NCB
#elif OEM_ACTIVATION_TABLE_LOCATION == 2
        (UINT8*)OA3TableAddress,
#endif
        sizeof(EFI_ACPI_MSDM_TABLE) - sizeof(EFI_ACPI_DESCRIPTION_HEADER) );

    // Updates MSDM table checksum
    MsdmTable->Header.Checksum = 0;
    MsdmTable->Header.Checksum = ChsumTbl((UINT8*)MsdmTable, sizeof(EFI_ACPI_MSDM_TABLE));

    // Check if pointer to MSDM ACPI table has been set already
    Ptr = (UINT8*)AcpiHdr + AcpiHdr->Length - sizeof(EFI_PHYSICAL_ADDRESS);
    AcpiPtr = (EFI_PHYSICAL_ADDRESS*)Ptr;
    if ( *AcpiPtr != (EFI_PHYSICAL_ADDRESS)MsdmTable ) {

// AptioV Server Override Start: EIP#484584 - SA50072 Security Fix

        //Check next entry is in legal memory.
        Ptr = (UINT8*)AcpiHdr + AcpiHdr->Length;
        Status = AmiValidateMemoryBuffer((VOID*)Ptr, sizeof(EFI_PHYSICAL_ADDRESS));
		if (EFI_ERROR(Status)) 
// AptioV Server Override End: EIP#484584 - SA50072 Security Fix
            return Status;
        
        // Check destination if it's cleared by zero 
        AcpiPtr = (EFI_PHYSICAL_ADDRESS*)Ptr;
        if (*AcpiPtr != 0)
            return EFI_SECURITY_VIOLATION;
        
        // Add address of MSDM table as the last pointer in XSDT
        *AcpiPtr = (EFI_PHYSICAL_ADDRESS)MsdmTable;

        // Updates Length filed of the XSDT table (increments by 8)
        AcpiHdr->Length += sizeof(EFI_PHYSICAL_ADDRESS);

        // Updates XSDT checksum
        AcpiHdr->Checksum = 0; 
        AcpiHdr->Checksum = ChsumTbl((UINT8*)AcpiHdr, AcpiHdr->Length);
    }

    return EFI_SUCCESS;
}


/**
    Calling the SMI Interface
    The caller will write AL (the value 0xee) to the SMI Command Port as 
    defined in the ACPI FADT.
    The SMI handler will update the callers' buffer(s) and return.

    @param 
        DispatchHandle
        DispatchContext

    @retval VOID

    @note  
 The function will clear the carry bit if it is successful (CF = 0). 
 If the function is unsuccessful, it will set the carry bit and set the 
 error code in the AH register as indicated by the error table below.
 The function returns the following data in the provided parameter block. 

**/

EFI_STATUS SwSmiOa3Function( 
    IN EFI_HANDLE DispatchHandle,
    IN CONST VOID  *Context OPTIONAL,
    IN OUT   VOID  *CommBuffer OPTIONAL,
    IN OUT   UINTN *CommBufferSize OPTIONAL )
{
    EFI_STATUS Status;
    EFI_SMM_SW_CONTEXT *SmmSwContext = ((EFI_SMM_SW_CONTEXT*)CommBuffer); 
    EFI_GUID EfiSmmCpuProtocolGuid = EFI_SMM_CPU_PROTOCOL_GUID;
    EFI_SMM_CPU_PROTOCOL *SmmCpuProtocol = NULL;
    EFI_PHYSICAL_ADDRESS Register = 0;

    Status = gSmst->SmmLocateProtocol(
        &EfiSmmCpuProtocolGuid,
        NULL, 
        &SmmCpuProtocol 
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    // Read the RBX register
    Status = SmmCpuProtocol->ReadSaveState( 
        SmmCpuProtocol,
        sizeof(UINT32), 
        EFI_SMM_SAVE_STATE_REGISTER_RBX,
        SmmSwContext->SwSmiCpuIndex,
        &Register
    );
    if (!EFI_ERROR(Status))
        Oa3UpdateAcpiTable((UINT8*)Register); // Update the ACPI table

    return Status;
}


/**
  The module Entry Point of the OA3 SMM driver.

  @param  ImageHandle    The firmware allocated handle for the EFI image.
  @param  SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS    The entry point is executed successfully.
  @retval Other          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
OA3_SMM_EntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
    EFI_STATUS Status;
    EFI_HANDLE Handle = 0;
    EFI_GUID EfiSmmSwDispatchProtocolGuid = EFI_SMM_SW_DISPATCH2_PROTOCOL_GUID;
    EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT   SwContext;

    Status = gSmst->SmmLocateProtocol(
        &EfiSmmSwDispatchProtocolGuid, 
        NULL, 
        &SwDispatch 
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status))
        return Status;

    SwContext.SwSmiInputValue = SW_SMI_OA3_FUNCTION_NUMBER;
    Status = SwDispatch->Register( 
        SwDispatch, 
        SwSmiOa3Function,
        &SwContext, 
        &Handle 
    );
    ASSERT_EFI_ERROR(Status);
    return Status;
}
