//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2021, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AMILogo.c
    This file contains code to handle logo.

**/

#ifdef TSE_FOR_APTIO_4_50
#include "Token.h"  
#else
#include "tokens.h"
#endif

#if !TSE_USE_EDK_LIBRARY
#include "Efi.h"  
#endif

#include "AMILogo.h"

EFI_GUID gAMIEfiLogoGuid = AMI_EFI_LOGO_GUID;
