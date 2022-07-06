//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
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
// $Header: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/PeiDbgPort.c 10    6/15/15 3:53a Jekekumarg $
//
// $Revision: 10 $
//
// $Date: 6/15/15 3:53a $
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/PeiDbgPort.c $
// 
// 10    6/15/15 3:53a Jekekumarg
// [TAG]			EIP222976
// [Description]	[Debugger] Apply Internal Label 22.2 (INT)
// [Files]			PeiDbgPort.c
// PeiDbgPortUSB.c
// DbgrHdr.c
// AMIDebugRx.chm
// XportHdr.c
// UsbCableSelect.c
// 
// 9     6/10/15 10:50a Sruthindn
// [TAG]			EIP220379
// [Description]	Convert CHM of Debugger to Doxygen Format
// [Files]			CommonDebugAgent.c
// PeiDbgPort.c
// PeiDbgPortUSB.c
// AmiUsbDbgPortHob.c
// CommonDebug.c
// XportHdr.c
// UsbCableSelect.c
// DebugPortUsb.c
// 
// 8     6/09/15 1:58a Jekekumarg
// [TAG]			EIP217777
// [Description]	AMI Debug for UEFI - Generate Binary, Test Report and
// Document Update for INT Label 22 
// [Files]			PeiAMIDebugAgent\PeiDbgMain.c
// AMIDebugAgentCommon\DebugAgent.c
// DxeAMIDebugAgent\DxeDbgMain.c
// SmmAMIDebugAgent\SmmDbgMain.c
// CommonDebugAgent.c
// DbgRxPortLib.sdl
// PeiDbgPort.c
// PeiDbgPortUSB.c
// MinDbgr.c
// AmiUsbDbgPortHob.c
// DbgHostStatusLib.sdl
// SmmDbgHostStatusLib.c
// CommonDebug.c
// relocdbg.c
// DebugPortUsb.c
// 
// 7     6/26/14 3:00p Sudhirv
// Updated after 3.01.0010 INT label
// 
// 6     6/24/14 6:00p Sudhirv
// Updated for AMI Debug for UEFI 3.01.0010 Label
// 
// 5     9/21/13 4:23a Sudhirv
// Updated for AMI Debug for UEFI 3.01.0006 Label.
// 
// 4     1/21/13 10:04a Sudhirv
// [TAG]	EIP97162
// [Category]	Improvement
// [Description]	Debugger 3.0: Make changes to recover\re-init the
// Debugport at runtime if required.
// [Files]	PeiDbgPort.c
// 
// 3     11/19/12 7:44a Sudhirv
// [TAG]     EIP106836
// [Category] Function Request
// [Severity] Normal
// [Symptom] Make changes to AMIDebugRx\Debugger for chipset porting
// through ELink
// [Solution] Issue is fixed, Now chipset porting is done by ELink.
// 
// 6     8/10/12 1:34p Sudhirv
// Updated
//
// 5     8/09/12 10:15p Sudhirv
// Updated
//
// 4     8/09/12 12:16p Sudhirv
// After making SBDebugPorting Modulepart
//
// 3     8/03/12 6:32p Sudhirv
// Updated before making binaries.
//
// 1     7/15/12 8:12p Sudhirv
// AMIDebugRx Module for Aptio 5
//
//*****************************************************************
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:		PeiDbgPort.c
//
// Description:This file contains code to intialize com port in pei
//
//<AMI_FHDR_END>
//**********************************************************************

#include <Library/PciLib.h>
#include <Token.h>

#ifndef EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif

#include <AmiDebugPort.h>
#include "PEIDbgRxPortELink.h"
//AMI SB Porting
#define	APIC_SPURIOUS_VECTOR_REGISTER					0xf0
#define	APIC_LVT_LINT0_REGISTER							0x350
#define	APIC_LVT_LINT1_REGISTER							0x360

UINT8  FindCapPtrDbg( IN UINT8 Bus, IN UINT8 DevFunc, IN UINT8 CapId);

#define DBG_PRT_CAP_ID     			0xA

#ifndef EFI_PCI_COMMAND_MEMORY_SPACE
#define EFI_PCI_COMMAND_MEMORY_SPACE                    0x02   ///< 0x0002
#endif

#ifndef PCI_COMMAND_OFFSET
#define PCI_COMMAND_OFFSET 0x04
#endif

#ifndef EFI_PCI_COMMAND_BUS_MASTER
#define EFI_PCI_COMMAND_BUS_MASTER 0x04 ///< 0x0004
#endif

extern EFI_STATUS SBPEIDBG_Initialize(PEI_DBG_PORT_INFO *DebugPort);
extern EFI_STATUS SBPEIDBG_InitUsbEhci(PEI_DBG_PORT_INFO *DebugPort);
extern EFI_STATUS CPUPEIDBG_Initialize(PEI_DBG_PORT_INFO *DebugPort);
extern EFI_STATUS SIOPEIDBG_Initialize(PEI_DBG_PORT_INFO *DebugPort);

void InitPeiDebugPortInterface(PEI_DBG_PORT_INFO *,UINTN, UINTN);
void Initialise(PEI_DBG_PORT_INFO *);

extern PEI_INIT_FUNCTION PEI_DBG_INIT_LIST EndOfInitList;
PEI_INIT_FUNCTION* PeiDebuggerInitList [] = {PEI_DBG_INIT_LIST NULL};

void DBGWriteChkPort(UINT8	DbgChkPoint);
//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	PeiDbgIsS3Hook
//
// Description:	Dummy Function to return FALSE.
//
// Input:		void
//
// Output:		UINT8
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
UINT8 PeiDbgIsS3Hook()
{
	return FALSE;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	ExecutePeiDebuggerInitList()
//
// Description:	Internal Helper function to invoke ELINKS.
//
// Input:		IN OUT PEI_DBG_PORT_INFO	*
//
// Output:		void
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
VOID
ExecutePeiDebuggerInitList(
	IN OUT	PEI_DBG_PORT_INFO	*DebugPort
)
{
	UINTN i;

	if(!PeiDebuggerInitList){
		while(1);
	}
	for(i=0; PeiDebuggerInitList[i]; i++)
		PeiDebuggerInitList[i](DebugPort);
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	InitPeiDbgPort
//
// Description:	Entrypoint for the PeiDbgPort, the module init the specific
//				chipset.
//
// Input:		UINTN	
//
// Output:		void
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
void InitPeiDbgPort(UINTN	DebugDataBaseAddr)
{
	PEI_DBG_PORT_INFO	DebugPort;
	UINTN				PeiDbgSetupFlag;

#ifndef EFIx64
	PEIDebugData_T		*pData = (PEIDebugData_T *)DebugDataBaseAddr;
	SerXPortGblData_T	*SerData = (SerXPortGblData_T *)pData->XportLocalVar;
#else
	PEIDebugData_Tx64		*pData = (PEIDebugData_Tx64 *)DebugDataBaseAddr;
	SerXPortGblData_Tx64	*SerData = (SerXPortGblData_Tx64 *)pData->XportLocalVar;

#endif

#if	PEI_DEBUG_SUPPORT_AFTER_MEMORY
	PeiDbgSetupFlag = 1;
#else
	PeiDbgSetupFlag = 0;
#endif

	SerData->m_IsHostConnected = 0;
	Initialise(&DebugPort);

	//Pei Dbg Porting
	/*SBPEIDBG_Initialize(&DebugPort);
	SBPEIDBG_InitUsbEhci(&DebugPort);*/
	ExecutePeiDebuggerInitList(&DebugPort);

	InitPeiDebugPortInterface(&DebugPort,PeiDbgSetupFlag,DebugDataBaseAddr);
	SerData->m_IsPeiDbgIsNotS3 = 1;
}

#define USB_DEBUG_PORT_IN_USE     BIT10
#define USB_DEBUG_PORT_ENABLE     BIT28
#define USB_DEBUG_PORT_OWNER      BIT30

#if USB_DEBUG_TRANSPORT
//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	NeedReinitializeHardwareDbg
//
// Description:	Procedure to determine if Re-Initialization of the USB
//				Debug port is required or not.
//
// Input:		IN UINTN 
//
// Output:		BOOLEAN	(True = Re-Init Required)
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN NeedReinitializeHardwareDbg(IN UINTN Context)
{
#ifndef EFIx64
	SerXPortGblData_T*	pUsbData = (SerXPortGblData_T *)Context;
#else
	SerXPortGblData_Tx64*	pUsbData = (SerXPortGblData_Tx64 *)Context;
#endif
	UINT8    	DbgPrtCapReg;
	UINT16   	DbgPrtBaseOffset;
	UINT32   	EhciBaseAddress = 0;
	BOOLEAN		Status = FALSE;
	UINT32		PciAddr = 0;
	UINT32 		*pDbgCtrlReg;
	DEBUG_PORT_CONTROL_REGISTER DbgCtrlReg;

	//AMIDebugRx\Debugger recovery Support	
	if(!pUsbData->m_IsHostConnected)
		return FALSE;

	PciAddr = (UINT32)(((pUsbData->m_PCI_EHCI_BUS_NUMBER << 8) | (pUsbData->m_PCI_EHCI_DEVICE_NUMBER << 3) | pUsbData->m_PCI_EHCI_FUNCTION_NUMBER)<<12);

	//Get the EHCI Base Address from the PCI Device
	EhciBaseAddress = PciRead32(PciAddr+pUsbData->m_PCI_EHCI_BAR_OFFSET);

	//Compare the Base Address with the current Base Address from DebugData
	if (EhciBaseAddress != pUsbData->USBBASE) {
		//pUsbData->USBBASE = EhciBaseAddress;
		Status = TRUE;
	}

	// Locate the Debug port register Interface location
	 DbgPrtCapReg = FindCapPtrDbg(
				   pUsbData->m_PCI_EHCI_BUS_NUMBER,
				   (UINT8)((UINT8)(pUsbData->m_PCI_EHCI_DEVICE_NUMBER << 3) | pUsbData->m_PCI_EHCI_FUNCTION_NUMBER),
				   DBG_PRT_CAP_ID
				   );

	 DbgPrtBaseOffset = PciRead16(PciAddr + DbgPrtCapReg + 2);

	 DbgPrtBaseOffset &= 0x1fff;
	 //Check if the EHCI start Address matches
	 if(pUsbData->USB2_DEBUG_PORT_REGISTER_INTERFACE != (EhciBaseAddress + DbgPrtBaseOffset)) {
		 //pUsbData->USB2_DEBUG_PORT_REGISTER_INTERFACE = EhciBaseAddress + DbgPrtBaseOffset;
		 Status = TRUE;
	 }

	//
	// Check if the debug port is enabled and owned by myself.
	//
	pDbgCtrlReg = (UINT32 *)(EhciBaseAddress + DbgPrtBaseOffset);
	DbgCtrlReg = *(DEBUG_PORT_CONTROL_REGISTER *)pDbgCtrlReg;
	if((DbgCtrlReg.Owner != 1)||(DbgCtrlReg.Enabled != 1)) {
		Status = TRUE;
	}

	return Status;
}

#endif

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	UpdateDebugXportData
//
// Description:	Update the Debug data structures with re-initialised
//				values
//
// Input:		PEI_DBG_PORT_INFO *, UINTN 
//
// Output:		VOID
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
VOID UpdateDebugXportData(PEI_DBG_PORT_INFO *DebugPort, UINTN Context)
{

#ifndef EFIx64
	SerXPortGblData_T	*XportData = (SerXPortGblData_T *)Context;
#else
	SerXPortGblData_Tx64	*XportData = (SerXPortGblData_Tx64 *)Context;
#endif

	XportData->USBBASE = DebugPort->UsbDebugPort.USBBaseAddr;
	XportData->USB2_DEBUG_PORT_REGISTER_INTERFACE = DebugPort->UsbDebugPort.USBDebugPortStartAddr;
	XportData->m_EHCI_MEMORY_SIZE = DebugPort->UsbDebugPort.MemoryMappedIoSpaceSize;
	XportData->m_PCI_EHCI_BUS_NUMBER = DebugPort->UsbDebugPort.PciBusNumber;
	XportData->m_PCI_EHCI_DEVICE_NUMBER = DebugPort->UsbDebugPort.PciDeviceNumber;
	XportData->m_PCI_EHCI_FUNCTION_NUMBER = DebugPort->UsbDebugPort.PciFunctionNumber;
	XportData->m_PCI_EHCI_BAR_OFFSET = DebugPort->UsbDebugPort.PciBAROffset;

	switch(XportData->USBBASE){
		case 0:
			DBGWriteChkPort(0xBF);	//err code for invalid USB base address register
			while(1);
		case 0xffffffff:
			DBGWriteChkPort(0xD1);	//err code for invalid Address
			while(1);
		default:
			break;
	}
	if(XportData->USBBASE == XportData->USB2_DEBUG_PORT_REGISTER_INTERFACE){
		DBGWriteChkPort(0xBE);	//err code for invalid USB Debug Port base address
		while(1);
	}
}

//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2014, American Megatrends, Inc.        **//
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
