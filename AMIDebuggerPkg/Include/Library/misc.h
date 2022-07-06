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
// $Header: /AptioV/SRC/AMIDebugRxPkg/Include/Library/misc.h 1     11/02/12 10:07a Sudhirv $
//
// $Revision: 1 $
//
// $Date: 11/02/12 10:07a $
//*****************************************************************
//*****************************************************************
//
//	misc.h
//
//
// Revision History
// ----------------
// $Log: /AptioV/SRC/AMIDebugRxPkg/Include/Library/misc.h $
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
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:		misc.h
//
// Description:	Generic Misc header file.
//
//<AMI_FHDR_END>
//**********************************************************************

#ifdef ALASKA_SUPPORT

#include <Efi.h>

#else	/* Support for APTIO projects */

#define UINT64  	unsigned __int64
#define UINT32  	unsigned long
#define UINT16  	unsigned short
#define UINT8  		unsigned char

#define INT32  		signed long
#define INT8  		signed char
#define INT16  		signed short

#define CHAR8		char
#define CHAR16		unsigned short
#define BOOLEAN		unsigned char

#define	UINTN	UINT32
#define	INTN	INT32

#define	VOID	void

#ifndef _EFI_TYPES_H_

#define TRUE	1

#define FALSE	0

#define NULL    0

#define IN
#define OUT

//EFI error types

#define	EFI_STATUS		UINTN

#define EFI_ERROR(a)					(((INTN) a) < 0)

#define EFIERR(a)						(0x80000000 | a)

#define EFI_SUCCESS                             0
#define EFI_LOAD_ERROR                  EFIERR(1)
#define EFI_INVALID_PARAMETER           EFIERR(2)
#define EFI_UNSUPPORTED                 EFIERR(3)
#define EFI_BAD_BUFFER_SIZE             EFIERR(4)
#define EFI_BUFFER_TOO_SMALL            EFIERR(5)
#define EFI_NOT_READY                   EFIERR(6)
#define EFI_DEVICE_ERROR                EFIERR(7)
#define EFI_WRITE_PROTECTED             EFIERR(8)
#define EFI_OUT_OF_RESOURCES            EFIERR(9)
#define EFI_VOLUME_CORRUPTED            EFIERR(10)
#define EFI_VOLUME_FULL                 EFIERR(11)
#define EFI_NO_MEDIA                    EFIERR(12)
#define EFI_MEDIA_CHANGED               EFIERR(13)
#define EFI_NOT_FOUND                   EFIERR(14)
#define EFI_ACCESS_DENIED               EFIERR(15)
#define EFI_NO_RESPONSE                 EFIERR(16)
#define EFI_NO_MAPPING                  EFIERR(17)
#define EFI_TIMEOUT                     EFIERR(18)
#define EFI_NOT_STARTED                 EFIERR(19)
#define EFI_ALREADY_STARTED             EFIERR(20)
#define EFI_ABORTED                     EFIERR(21)
#define EFI_ICMP_ERROR                  EFIERR(22)
#define EFI_TFTP_ERROR                  EFIERR(23)
#define EFI_PROTOCOL_ERROR              EFIERR(24)

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

