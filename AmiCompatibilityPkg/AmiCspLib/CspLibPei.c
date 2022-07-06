//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//*****************************************************************************
//<AMI_FHDR_START>
//----------------------------------------------------------------------------
//
// Name:		CspLibPei.c
//
// Description:	
//  This file contains code for Generic CSP Library PEI functions.  The 
//  functions include PCI table update etc.
//
//----------------------------------------------------------------------------
//<AMI_FHDR_END>


//============================================================================
// Module specific Includes
//#include <AmiLib.h>
#include "Efi.h"
//#include "Pei.h"
#include <AmiPeiLib.h>
#include "StatusCodes.h"

#include "Token.h"
#include "Ppi/CspLibPpi.h"
//#include "Ppi\PCICfg.h"



//============================================================================
// GUID Definitions
//	EFI_GUID	gPeiPCITableInitPCIGUID = AMI_PEI_PCI_TABLE_INIT_PPI_GUID;
//============================================================================
// Produced PPIs
// PPI Member definitions

#ifndef PI_SPECIFICATION_VERSION //old Core

EFI_STATUS PciCfgModify(
    IN CONST EFI_PEI_SERVICES 	**PeiServices,
    IN EFI_PEI_PCI_CFG_PPI_WIDTH	Width,
    IN UINT64					Address,
    IN UINTN					SetBits,
    IN UINTN					ClearBits)
{
    if((*PeiServices)->PciCfg==NULL)
        return EFI_NOT_AVAILABLE_YET;

    return (*PeiServices)->PciCfg->Modify(
                    (EFI_PEI_SERVICES**)PeiServices,
                    (*PeiServices)->PciCfg,
                    Width, Address,
                    SetBits, ClearBits);
}
#endif


EFI_STATUS PciTableInit (
  IN CONST	EFI_PEI_SERVICES      			**PeiServices,
  IN  		AMI_PEI_PCI_TABLE_INIT_PPI		*This,
  IN		EFI_PEI_PCI_CFG2_PPI				*PciCfg,
  IN  		UINT64                			Address,
  IN		AMI_PEI_PCI_INIT_TABLE_STRUCT	*PCIInitTable,
  IN		UINT16							wSize
);
	
EFI_STATUS PciTableInit2 (
	IN			EFI_PEI_SERVICES                **PeiServices,
	IN			AMI_PEI_PCI_TABLE_INIT_PPI      *This,
    IN		    EFI_PEI_PCI_CFG2_PPI				*PciCfg,
	IN			UINT64							CfgAddress,
	IN			AMI_PEI_PCI_INIT_TABLE_STRUCT2	*PciInitTable,
	IN			UINT16							TableEntries,
    IN          EFI_PEI_PCI_CFG_PPI_WIDTH       AccessWidth
);


// PPI interface definition
AMI_PEI_PCI_TABLE_INIT_PPI  mPciTableInitPpi =
{
	PciTableInit,
    PciTableInit2//,
};

static EFI_PEI_PPI_DESCRIPTOR mPpiList[] =  { 
(EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
					    &gAmiPeiPciTableInitPpiGuid,//&gPeiPciTableInitPpiGuid,
					    &mPciTableInitPpi};

//============================================================================
// Portable Constants

//============================================================================
// Function Prototypes

//============================================================================
// Function Definitions


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	PciTableInit
//
// Description:	
//  This function programs the PCI device with the values provided 
//  in the init table provided
//
// Input:
//  EFI_PEI_SERVICES **PeiServices - Pointer to the PEI Core data Structure
//  AMI_PEI_PCI_TABLE_INIT_PPI *This - Pointer to an instance of the AMI PEI PCI TABLE INIT PPI
//  EFI_PEI_PCI_CFG_PPI *PciCfg - Optional pointer to an instance of the PciCfg PPI
//  UINT64 Address - PCI address of the register to write to (Bus/Dev/Func/Reg)
//  AMI_PEI_PCI_INIT_TABLE_STRUCT *PCIInitTable - Table with register number, set and clear bits
//  UINT16 wSize - Table length (multiples of structure)
//
// Output:		
//  Always returns EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
PciTableInit (
  IN CONST	EFI_PEI_SERVICES      			**PeiServices,
  IN  		AMI_PEI_PCI_TABLE_INIT_PPI		*This,
  IN		EFI_PEI_PCI_CFG2_PPI			*PciCfg,			// OPTIONAL
  IN  		UINT64                			Address,
  IN		AMI_PEI_PCI_INIT_TABLE_STRUCT	PCIInitTable[],
  IN		UINT16							wSize
  )
{
	EFI_STATUS		Status = EFI_SUCCESS;
	UINTN			Index;
	UINT64			LocalAddr;


	// If PciCfg is NULL then locate PciCfg PPI
//		if (PciCfg == NULL) {
//			PciCfg = (*PeiServices)->PciCfg;
//		}

	if ((! wSize) || (!PCIInitTable))
		return Status;

    // Take data defined in the PCIInitTable and program the PCI devices
    // as ported
	for (Index = 0; Index < wSize; Index ++) {
		LocalAddr = Address + PCIInitTable[Index].bRegIndex;

        PciCfgModify(PeiServices,
                    EfiPeiPciCfgWidthUint8, LocalAddr,
                    PCIInitTable[Index].bORMask, ~PCIInitTable[Index].bANDMask);
//			PciCfg->Modify( PeiServices, PciCfg, EfiPeiPciCfgWidthUint8,
//							LocalAddr, 
//							PCIInitTable[Index].bORMask, 
//							~PCIInitTable[Index].bANDMask);
        
	}

	return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	PciTableInit2
//
// Description:	
//  This function is identical to PciTableInit with the addition of an extra 
//  parameter to specify PCI access width.
//
// Input:
//  EFI_PEI_SERVICES **PeiServices - Pointer to the PEI Core data Structure
//  AMI_PEI_PCI_TABLE_INIT_PPI *This - Pointer to an instance of the AMI PEI PCI TABLE INIT PPI
//  EFI_PEI_PCI_CFG_PPI *PciCfg - Optional pointer to an instance of the PciCfg PPI
//  UINT64 CfgAddress - PCI address of the register to write to (Bus/Dev/Func/Reg)
//  AMI_PEI_PCI_INIT_TABLE_STRUCT2 *PCIInitTable - Table with register number, set and clear bits
//  UINT16 TableEntries - Table length (multiples of structure)
//  EFI_PEI_PCI_CFG_PPI_WIDTH AccessWidth - Register-level access width
//
// Output:		
//  Always returns EFI_SUCCESS
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS                            
PciTableInit2 (
	IN			EFI_PEI_SERVICES                **PeiServices,
	IN			AMI_PEI_PCI_TABLE_INIT_PPI      *This,
    IN			EFI_PEI_PCI_CFG2_PPI			*PciCfg,		// OPTIONAL
	IN			UINT64							CfgAddress,
	IN			AMI_PEI_PCI_INIT_TABLE_STRUCT2	*PciInitTable,
	IN			UINT16							TableEntries,
    IN          EFI_PEI_PCI_CFG_PPI_WIDTH       AccessWidth
  )
{
	EFI_STATUS		Status = EFI_SUCCESS;
	UINTN			Index;
	UINT64			LocalAddr;

	// If PciCfg is NULL then locate PciCfg PPI
//		if (PciCfg == NULL) {
//			PciCfg = (*PeiServices)->PciCfg;
//		}

	if ((! TableEntries) || (!PciInitTable))
		return Status;

    // Take data defined in the PCIInitTable and program the PCI devices
    // as ported
	for (Index = 0; 
         Index < TableEntries && !EFI_ERROR(Status); 
         Index ++) 
    {
		LocalAddr = CfgAddress + PciInitTable[Index].bRegIndex;

                    Status = PciCfgModify(PeiServices,
                                AccessWidth, LocalAddr,
                                PciInitTable[Index].bORMask, PciInitTable[Index].bANDMask);

//			Status = PciCfg->Modify( PeiServices,
//	                        PciCfg,
//	                        AccessWidth,
//							LocalAddr,
//							PciInitTable[Index].bORMask,
//							PciInitTable[Index].bANDMask);
        
    }

	return EFI_SUCCESS;
}


//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	CspLibPei_Init
//
// Description:	
//  This function is the entry point for this PEI. This function initializes 
//  installs the CSP PPI
//
// Input:		
//  FfsHeader   Pointer to the FFS file header
//  PeiServices Pointer to the PEI services table
//
// Output:		
//  Return Status based on errors that are returned by the InstallPpi function
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS
EFIAPI
CspLibPei_Init (
  IN EFI_PEI_FILE_HANDLE       FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{

	EFI_STATUS					Status;

	// Install the NB Init Policy PPI
	Status = (*PeiServices)->InstallPpi(PeiServices, &mPpiList[0]);
#ifdef EFI_DEBUG
	if (Status) {
		PEI_TRACE((TRACE_PEI_CHIPSET, PeiServices, "CspLibPei_Init Return Code : %r\n", Status));
	}
#endif
	return Status;

}

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

