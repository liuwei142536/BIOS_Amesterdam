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
// $Header: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/PeiDbgPortUSB.c 6     6/15/15 3:53a Jekekumarg $
//
// $Revision: 6 $
//
// $Date: 6/15/15 3:53a $
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRx/DbgRxPortLib/PeiDbgPortUSB.c $
// 
// 6     6/15/15 3:53a Jekekumarg
// [TAG]			EIP222976
// [Description]	[Debugger] Apply Internal Label 22.2 (INT)
// [Files]			PeiDbgPort.c
// PeiDbgPortUSB.c
// DbgrHdr.c
// AMIDebugRx.chm
// XportHdr.c
// UsbCableSelect.c
// 
// 5     6/10/15 10:50a Sruthindn
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
// 4     6/09/15 1:58a Jekekumarg
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
// 3     6/26/14 3:00p Sudhirv
// Updated after 3.01.0010 INT label
// 
// 2     6/24/14 6:00p Sudhirv
// Updated for AMI Debug for UEFI 3.01.0010 Label
// 
// 1     11/02/12 10:04a Sudhirv
// AMIDebugRx eModule for AMI Debug for UEFI 3.0
// 
// 2     8/03/12 6:32p Sudhirv
// Updated before making binaries.
//
// 1     7/15/12 8:12p Sudhirv
// AMIDebugRx Module for Aptio 5
//
//*****************************************************************
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:		PeiDbgPortUSB.c
//
// Description:This file contains code to intialize PeiDebugPortInterface
//
//<AMI_FHDR_END>
//**********************************************************************

#ifndef EFIx64
#include <Library/AMIpeidebug.h>
#else
#include <Library/AMIpeidebugX64.h>
#endif
#include <AmiDebugPort.h>

void DBGWriteChkPort(UINT8	DbgChkPoint);
//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	Initialise
//
// Description:	Internal Helper function.
//
// Input:		PEI_DBG_PORT_INFO *
//
// Output:		void
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
void Initialise(PEI_DBG_PORT_INFO *DebugPort)
{
	DebugPort->UsbDebugPort.USBBaseAddr = 0;
	DebugPort->UsbDebugPort.USBDebugPortStartAddr = 0;
	DebugPort->UsbDebugPort.MemoryMappedIoSpaceSize = 0;
	DebugPort->UsbDebugPort.PciBusNumber = 0;
	DebugPort->UsbDebugPort.PciDeviceNumber = 0;
	DebugPort->UsbDebugPort.PciFunctionNumber = 0;
	DebugPort->UsbDebugPort.PciBAROffset = 0;
}

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	InitPeiDebugPortInterface
//
// Description:	Internal Helper function.
//
// Input:		PEI_DBG_PORT_INFO *,
//				UINTN ,
//				UINTN 
//
// Output:		void
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>
void InitPeiDebugPortInterface(PEI_DBG_PORT_INFO *DebugPort,UINTN SetupFlag, UINTN IDTBaseAddress)
{
#ifndef EFIx64
	PEIDebugData_T		*pData = (PEIDebugData_T *)IDTBaseAddress;
	SerXPortGblData_T	*PeiXportData = (SerXPortGblData_T *)pData->XportLocalVar;
	DbgGblData_T		*DbgData = (DbgGblData_T *)(pData->DbgrLocalVar);
#else
	PEIDebugData_Tx64		*pData = (PEIDebugData_Tx64 *)IDTBaseAddress;
	SerXPortGblData_Tx64	*PeiXportData = (SerXPortGblData_Tx64 *)pData->XportLocalVar;
	DbgGblData_Tx64		*DbgData = (DbgGblData_Tx64 *)(pData->DbgrLocalVar);
#endif

#if USB_DEBUG_TRANSPORT
	DbgData->m_FlagToDebugAfterMemory = (UINT8)SetupFlag;
#ifndef EFIx64
	// PORTING for ALASKA: For PciBus DXE driver HOB interface
	((SerXPortGblVar_T*)pData->XportGlobalVar)->PeiDbgSIO_Init_USB_EHCI =
							DebugPort->UsbDebugPort.InitUSBEHCI;
#else
	((SerXPortGblVar_Tx64*)pData->XportGlobalVar)->PeiDbgSIO_Init_USB_EHCI =
								DebugPort->UsbDebugPort.InitUSBEHCI;
#endif

	PeiXportData->USBBASE = DebugPort->UsbDebugPort.USBBaseAddr;
	PeiXportData->USB2_DEBUG_PORT_REGISTER_INTERFACE = DebugPort->UsbDebugPort.USBDebugPortStartAddr;
	PeiXportData->m_EHCI_MEMORY_SIZE = DebugPort->UsbDebugPort.MemoryMappedIoSpaceSize;
	PeiXportData->m_PCI_EHCI_BUS_NUMBER = DebugPort->UsbDebugPort.PciBusNumber;
	PeiXportData->m_PCI_EHCI_DEVICE_NUMBER = DebugPort->UsbDebugPort.PciDeviceNumber;
	PeiXportData->m_PCI_EHCI_FUNCTION_NUMBER = DebugPort->UsbDebugPort.PciFunctionNumber;
	PeiXportData->m_PCI_EHCI_BAR_OFFSET = DebugPort->UsbDebugPort.PciBAROffset;

	switch(PeiXportData->USBBASE){
		case 0:
			DBGWriteChkPort(0xBF);	//err code for invalid USB base address register
			while(1);
		case 0xffffffff:
			DBGWriteChkPort(0xD1);	//err code for invalid CAR
			while(1);
		default:
			break;
	}
	if(PeiXportData->USBBASE == PeiXportData->USB2_DEBUG_PORT_REGISTER_INTERFACE){
		DBGWriteChkPort(0xBE);	//err code for invalid USB Debug Port base address
		while(1);
	}
#endif
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
