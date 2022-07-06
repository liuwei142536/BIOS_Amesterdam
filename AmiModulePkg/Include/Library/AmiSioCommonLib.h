//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file AmiSioCommonlib.h

    This contains the functions to Access the Super IO.
**/
#ifndef  _AMISIOCOMMONLIB_H_
#define  _AMISIOCOMMONLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <Token.h>

/**
    Function Definitions
**/
#if (NUMBER_OF_SIO > 1)
VOID
EFIAPI SioEnterConfigMode(IN UINT8);

VOID
EFIAPI SioExitConfigMode(IN UINT8);

VOID
EFIAPI SioSelectLdn(IN UINT8, IN UINT8);
#else
VOID
EFIAPI SioEnterConfigMode(VOID);

VOID
EFIAPI SioExitConfigMode(VOID);

VOID
EFIAPI SioSelectLdn(IN UINT8);
#endif
/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
