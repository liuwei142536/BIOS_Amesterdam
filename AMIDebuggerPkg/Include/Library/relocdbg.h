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
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/relocdbg.h 2     2/20/14 5:01p Sudhirv $
//
// $Revision: 2 $
//
// $Date: 2/20/14 5:01p $
//**********************************************************************
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/relocdbg.h $
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
//**********************************************************************

#ifndef _PEI_DBGSUPPORT_RELOCATE_H
#define _PEI_DBGSUPPORT_RELOCATE_H

#include <PPI/LoadFile.h>
#include <AmiLib.h>

VOID	PublishDebugPortHob(
		IN EFI_PEI_SERVICES **PeiServices,
		IN EFI_PHYSICAL_ADDRESS		DataBaseAddress);

#endif

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
