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
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/ser_acc.h 3     1/05/16 7:38p Sudhirv $
//
// $Revision: 3 $
//
// $Date: 1/05/16 7:38p $
//*****************************************************************
//*****************************************************************
//
//	ser_acc.h
//
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/ser_acc.h $
// 
// 3     1/05/16 7:38p Sudhirv
// [TAG]			EIP249624
// [Category]   	Bug Fix
// [Severity]		Normal
// [Symptom]  	Setting Breakpoint in SMM Code from DXE phase when SMM
// Entry\Exit is ON causes the Host-Target to lose sync
// [RootCause]	Setting Breakpoint in SMM Code from DXE phase when SMM
// Entry\Exit is ON causes the Host-Target to lose sync
// [Solution]		handle the memory access case
// [Files]			AMIDebugRxPkg.sdl
// AMIDebugRxPkg.dec
// AMIDebugRxPkg.cif
// AMIpeidebug.h
// AMIpeidebugX64.h
// ser_acc.h
// ser_xp.h
// 
// 2     2/20/14 5:01p Sudhirv
// Updated for AMI Debug for UEFI 3.01.0008 INT Label
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

UINT8 IsDataInIp();
UINT8 ReadByte();
UINTN WriteByte(UINT8);
void DisableDTRRTS();
void EnableDTRRTS();
void DoEnableDTRRTS();

void TempEnableHwFlowCtrl();
void TempDisableHwFlowCtrl();
UINT8 RetrieveCacheData();

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

