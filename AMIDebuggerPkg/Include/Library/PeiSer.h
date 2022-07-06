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
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/PeiSer.h 1     11/02/12 10:07a Sudhirv $
//
// $Revision: 1 $
//
// $Date: 11/02/12 10:07a $
//*****************************************************************
//*****************************************************************
//
//	PeiSer.h
//
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/PeiSer.h $
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



#define CONFIG_PORT0                0x2E
#define INDEX_PORT0                 0x2E
#define DATA_PORT0                  0x2F

extern void PEIIoWrite8 (UINT16, UINT8);
extern UINT8 PEIIoRead8 (UINT16);
void InitSIO();
void InitMain();

#if USB_DEBUG_TRANSPORT
void	Program8254Timer0(UINT16 Count);
void	Init_USB_EHCI(UINTN PeiDbgData_BaseAddr,UINTN MMIO_BaseAddr);
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

