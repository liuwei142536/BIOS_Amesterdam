//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2018, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
// $Header: /AptioV/SRC/SerialDebugger/DbgSerInitLib/DbgSerInitLib.c 4     6/10/15 11:20a Sruthindn $
//
// $Revision: 4 $
//
// $Date: 6/10/15 11:20a $
//
//**********************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/SerialDebugger/DbgSerInitLib/DbgSerInitLib.c $
// 
// 4     6/10/15 11:20a Sruthindn
// [TAG]			EIP220379
// [Description]	Convert CHM of Debugger to Doxygen Format
// [Files]			XportHdr.c
// PeCoffHelp.c
// DbgModuleHelp.c
// AmiUsbDbgPortHob.c
// PeiDbgPort.c
// PeiDbgPortCOM.c
// DbgrHdr.c
// CommonDebug.c
// CommonDebugAgent.c
// DbgSerInitLib.c
// DebugPortUsb.c
// 
// 3     12/22/14 3:53p Sudhirv
// System waits at 0xD0 for a long time in case of no cable connected
// 
// 2     12/04/14 4:46p Sudhirv
// Updated for SerialDebugger 3.01.0015.1
// 
// 1     10/24/14 8:02p Sudhirv
// [TAG]     EIP189668
// [Category] Action Item
// [Description] Serial Debugger to support debugging by applying two
// wires, RX and TX
// 
//**********************************************************************
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:		DbgSerInitLib.c
//
// Description:This file contains code to halt debugger at pei entry point
//
//<AMI_FHDR_END>
//**********************************************************************

#include <Pei.h>
#include <Efi.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PeiServicesLib.h>
#ifndef EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif

#pragma optimize( "", off )

#define DEBUG_MAILBOX_VECTOR            33

VOID SerialPeiInitialBreakPoint();

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	GetPeiDebugDataBaseaddress
//
// Description:	Function returns the DebugData pointer from  IDT.
//
// Input:		void
//
// Output:		UINTN
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
UINTN
GetPeiDebugDataBaseaddress (
  VOID
  )
{
  IA32_IDT_GATE_DESCRIPTOR   *IdtEntry;
  IA32_DESCRIPTOR            IdtDescriptor;
  UINTN                      Mailbox;

  AsmReadIdtr (&IdtDescriptor);
  IdtEntry = (IA32_IDT_GATE_DESCRIPTOR *) IdtDescriptor.Base;

  Mailbox = IdtEntry[DEBUG_MAILBOX_VECTOR].Bits.OffsetLow + (IdtEntry[DEBUG_MAILBOX_VECTOR].Bits.OffsetHigh << 16);
  return Mailbox;
}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//  Procedure:   DbgSerInitConstructor
//
//  Description:
//  Library constructor - 
//
//  Input:	
//	VOID
//
//  Output: 
//	EFI_STATUS, based on result
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS 
EFIAPI 
DbgSerInitConstructor (
		VOID)
{
	PEIDebugData_T		*pData;
	SerXPortGblVar_T	*SerVar;
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN PeiDebugDataBaseAddress = 0;
	CONST EFI_PEI_SERVICES  **PeiServices;
	VOID                    *Dummy;

	PeiServices = GetPeiServicesTablePointer ();
	
    Status = PeiServicesLocatePpi(
               &gEfiPeiMemoryDiscoveredPpiGuid,
               0,
               NULL,
               (VOID **)&Dummy
               );
    
    if (!(EFI_ERROR(Status))) {
        return Status;
    }
	
	// Get the PeiDebug Data Base Address
	PeiDebugDataBaseAddress = GetPeiDebugDataBaseaddress ();	

	pData		= (PEIDebugData_T *)PeiDebugDataBaseAddress;
	SerVar		= (SerXPortGblVar_T *)(pData->XportGlobalVar);

	//Initialize the PEIDbg Libraries
	Status = SerVar->gTransportProtocol->ReInitialize();
    
	if(!EFI_ERROR(Status)) {
		//SerialDebugger : This acts as the Pei Initial Breakpoint
		__debugbreak();
	}
	
	//Always return Success to avoid ASSERT
	return EFI_SUCCESS;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	SerialPeiInitialBreakPoint
//
// Description:	Function sets Breakpoint
//
// Input:		VOID
//
// Output:		VOID
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
VOID SerialPeiInitialBreakPoint ()
{
	__debugbreak();
	
	return;
}
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2018, American Megatrends, Inc.        **//
//**                                                             **//
//**                     All Rights Reserved.                    **//
//**                                                             **//
//**   5555 Oakbrook Pkwy, Building 200,Norcross, Georgia 30093  **//
//**                                                             **//
//**                     Phone (770)-246-8600                    **//
//**                                                             **//
//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
