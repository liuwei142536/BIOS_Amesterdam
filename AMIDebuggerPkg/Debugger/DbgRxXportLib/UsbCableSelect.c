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
// $Header: /AptioV/SRC/AMIDebugRx/DbgRxXportLib/UsbCableSelect.c 3     6/15/15 3:53a Jekekumarg $
//
// $Revision: 3 $
//
// $Date: 6/15/15 3:53a $
//*****************************************************************
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRx/DbgRxXportLib/UsbCableSelect.c $
// 
// 3     6/15/15 3:53a Jekekumarg
// [TAG]			EIP222976
// [Description]	[Debugger] Apply Internal Label 22.2 (INT)
// [Files]			PeiDbgPort.c
// PeiDbgPortUSB.c
// DbgrHdr.c
// AMIDebugRx.chm
// XportHdr.c
// UsbCableSelect.c
// 
// 2     6/10/15 10:50a Sruthindn
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
// 1     11/21/12 10:24a Sudhirv
// Updated afor Porting purposes
// 
//
//*****************************************************************

//<AMI_FHDR_START>
//--------------------------------------------------------------------
//
// Name:	UsbCableSelect.c
//
// Description:	DbgrUsbCableSelectLib library source file.
//				
//--------------------------------------------------------------------
//<AMI_FHDR_END>

//--------------------------------------------------------------------
#include <Efi.h>
#include "Token.h"
//--------------------------------------------------------------------
//
// The following functions are from the USB Transport module, to INIT USB
// Cable Device.
//
EFI_STATUS	InitUsb2DebugCableDevice();
EFI_STATUS	SetFeature(UINT16 FeatureSelector);
EFI_STATUS	SetAddress(UINT8 Addr);
EFI_STATUS	SetConfiguration(UINT16 ConfigValue);
void		UpdateBulkTransferEndpoints(UINT8	InEndpoint, UINT8	OutEndpoint);
//--------------------------------------------------------------------

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	Init_ALi5632Ctrl_Usb2Cable
//
// Description:	Sets ALi 5632 Controller USB Cable device address
//				and issues configuration commands.
//				This is left as an example for porting engineer.
//
// Input:		UINT8	
//
// Output:		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

#if	USE_ALI_5632_CTRL_USB2_CABLE

EFI_STATUS	Init_ALi5632Ctrl_Usb2Cable(UINT8	Addr)
{
	EFI_STATUS	Status = EFI_NOT_FOUND;
	Status = SetAddress(Addr);
	if(Status == EFI_SUCCESS){

		Status = SetConfiguration(1);

		//update the globals for bulk transfer endpoints
		UpdateBulkTransferEndpoints(1,2);
	}
	
	return Status;
}

#endif

//<AMI_PHDR_START>
//--------------------------------------------------------------------
// Procedure:	InitUsbCableDevice()
//
// Description:	To select between USB 2.0 Debug Cable device or any
//				other OEM specific USB 2.0 USB cable like ALi 5632 Usb2.0 Cable.
//
// Input:		void
//
// Output:		EFI_STATUS
//
//--------------------------------------------------------------------
//<AMI_PHDR_END>

EFI_STATUS InitUsbCableDevice()
{
#if	USE_ALI_5632_CTRL_USB2_CABLE
	//
	//This is just left as an example to porting Engineer
	//
	return Init_ALi5632Ctrl_Usb2Cable(MULTI_LINQ_USB2_DEVICE_ADDRESS);
#else
	return InitUsb2DebugCableDevice();
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
