//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file 
CleanErrorLog.h

Header file for the CleanErrorLog

*/

#ifndef _EFI_CLEANERRORLOG_H_
#define _EFI_CLEANERRORLOG_H_
#ifdef __cplusplus
extern "C" {
#endif

VOID CleanErrorLogEntry (IN VOID *Buffer, IN OUT UINT8 *pOFBDDataHandled);
VOID CleanErrorLogInSmm (VOID);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif


