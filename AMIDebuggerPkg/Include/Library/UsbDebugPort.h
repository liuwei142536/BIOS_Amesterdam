//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2012, American Megatrends, Inc.        **//
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
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/UsbDebugPort.h 2     6/24/14 6:05p Sudhirv $
//
// $Revision: 2 $
//
// $Date: 6/24/14 6:05p $
//*****************************************************************
//*****************************************************************
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/UsbDebugPort.h $
// 
// 2     6/24/14 6:05p Sudhirv
// Updated for AMI Debug for UEFI 3.01.0010 Label
// 
// 1     11/02/12 10:07a Sudhirv
// AMIDebugRxPkg modulepart for AMI Debug for UEFI 3.0
// 
// 2     8/03/12 6:32p Sudhirv
// Updated before making binaries.
//
// 1     7/15/12 8:12p Sudhirv
// AMIDebugRx Module for Aptio 5
//
//*****************************************************************

#ifndef	__USBDEBUGPORT_H__
#define __USBDEBUGPORT_H__

#include "Token.h"

#if USB_DEBUG_TRANSPORT
#pragma pack(1)

typedef struct _RAW_PACKET{
	UINT8	D0;
	UINT8	D1;
	UINT8	D2;
	UINT8	D3;
	UINT8	D4;
	UINT8	D5;
	UINT8	D6;
	UINT8	D7;
}RAW_PACKET;

typedef struct _QWORD_PACKET{
	UINT16	w1;
	UINT16	w2;
	UINT16	w3;
	UINT16	w4;
}QWORD_PACKET;

typedef union _PACKET{
	UINT64			Packet;

	RAW_PACKET		RawPacket;

	DATA_BUFFER		BfrPacket;	//bifurcated packet into 2 dwords

	QWORD_PACKET	QWord;		//4 word packet

}PACKET;

#pragma pack()

void		AddDelay();
void 		AddExtDelay();
void 		AddSpecialDelay();
void		DBGWriteChkPort(UINT8	DbgChkPoint);

EFI_STATUS	LocateDebugPortFromPCI();

EFI_STATUS	InitDebugPort();
EFI_STATUS	InitDebugDevice();
void		CloseDebugPort();
void		RestoreHC();
UINT32*		LocatePORTSC();
EFI_STATUS	ResetDebugPort(UINT32	*pPortSC);
EFI_STATUS	DisableDebugPORTSC();


/////////////similar APIs to serial transport//////////////////////
UINT16		DbgPortReady( UINTN );
UINT16		ReadDbgPortByte( VOID );
UINTN		WriteDbgPortByte( UINT8 data );

#endif
#endif


//*****************************************************************//
//*****************************************************************//
//*****************************************************************//
//**                                                             **//
//**         (C)Copyright 2012, American Megatrends, Inc.        **//
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

