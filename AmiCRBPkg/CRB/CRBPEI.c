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
// Revision History
// ----------------
// $Log: $
// 
//
//*****************************************************************************
//<AMI_FHDR_START>
//
// Name:        CRBPEI.c
//
// Description: This file contains code for Chipset Reference Board Template
//              initialization in the PEI stage
//
//<AMI_FHDR_END>
//*****************************************************************************

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------

#include <Efi.h>
#include <Pei.h>
#include <Token.h>
#include <AmiLib.h>
#include <AmiPeiLib.h>
#include <Hob.h>
#include <Setup.h>
#include <AmiCspLib.h>
#include <AmiCrbInc.h>//Build directory
//AptioV
#include <CrbPeiInitElink.h>
#if defined(PI_SPECIFICATION_VERSION)&&(PI_SPECIFICATION_VERSION>=0x00010000)
#include <PPI\smbus2.h>
#else
#include <PPI\smbus.h>
#endif

//----------------------------------------------------------------------------
// Constant, Macro and Type Definition(s)
//----------------------------------------------------------------------------
// Constant Definition(s)

// Macro Definition(s)

// Type Definition(s)

// Function Prototype(s)

//----------------------------------------------------------------------------
// Variable and External Declaration(s)
//----------------------------------------------------------------------------
// Variable Declaration(s)

// GUID Definition(s)
#if defined(PI_SPECIFICATION_VERSION)&&(PI_SPECIFICATION_VERSION>=0x00010000)
EFI_GUID gEfiPeiSmbus2PpiGuid = EFI_PEI_SMBUS2_PPI_GUID;
#else
EFI_GUID gPeiSmBusPpiGuid = EFI_PEI_SMBUS_PPI_GUID;
#endif

// PPI Definition(s)

// Function Definition(s)
EFI_STATUS InitCK505ClockGen(
	IN EFI_PEI_SERVICES             **PeiServices,
	IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
	IN VOID                         *NullPpi
);

// PPI that are installed

// PPI that are notified
static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList[] = 
{
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
#if defined(PI_SPECIFICATION_VERSION)&&(PI_SPECIFICATION_VERSION>=0x00010000)
        &gEfiPeiSmbus2PpiGuid,
#else
        &gPeiSmBusPpiGuid,
#endif
		InitCK505ClockGen
	}
};

// External Declaration(s)

//----------------------------------------------------------------------------

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   CRBPEI_Init
//
// Description: This function is the entry point for this PEI. This function
//              initializes the chipset CRB
//
// Parameters:  FfsHeader   Pointer to the FFS file header
//              PeiServices Pointer to the PEI services table
//
// Returns:     Return Status based on errors that occurred while waiting for
//              time to expire.
//
// Notes:       This function could initialize CRB for anything.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS EFIAPI CRBPEI_Init (
    IN       EFI_PEI_FILE_HANDLE   FileHandle,
    IN CONST EFI_PEI_SERVICES     **PeiServices )
{
    EFI_STATUS Status = EFI_SUCCESS;
#if PI_SPECIFICATION_VERSION < 0x00010000
    EFI_PEI_PCI_CFG_PPI *PciCfg;   
#else
    EFI_PEI_PCI_CFG2_PPI *PciCfg;
#endif
    EFI_PEI_CPU_IO_PPI          *CpuIo;

    // Get pointer to the PCI config PPI
    PciCfg = (*PeiServices)->PciCfg;
    CpuIo = (*PeiServices)->CpuIo;

    // Set the CRB Notify PPI
    Status = (*PeiServices)->NotifyPpi(PeiServices, mNotifyList);
    ASSERT_PEI_ERROR (PeiServices, Status);

    return EFI_SUCCESS;
}


EFI_STATUS InitCK505ClockGen(
    IN EFI_PEI_SERVICES **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID *NullPpi
)
{
	EFI_STATUS		Status;
	EFI_PEI_SMBUS2_PPI	*SmBus;
	EFI_SMBUS_DEVICE_ADDRESS	Address;
	UINTN		Len = 12;
	UINT8		CK505TBL[] = { 0x95, 0x83, 0xff, 0xff, 0xff, 0x00, 0x00, 0x95, 0x13, 0x65, 0x7d, 0x56 };

	PEI_TRACE((TRACE_ALWAYS, PeiServices, "Program CK505 start.\n"));
        return EFI_SUCCESS; // Richard debug
	Address.SmbusDeviceAddress = 0x69;

	Status = (*PeiServices)->LocatePpi( PeiServices,
				&gEfiPeiSmbus2PpiGuid,
				0,
				NULL,
				&SmBus );
	ASSERT_PEI_ERROR( PeiServices, Status);

	SmBus->Execute( SmBus,
			Address,
			0,
			EfiSmbusWriteBlock,
			0,
			&Len,
			CK505TBL
			);

	PEI_TRACE((TRACE_ALWAYS, PeiServices, "Program CK505 end.\n"));


	return EFI_SUCCESS;
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
