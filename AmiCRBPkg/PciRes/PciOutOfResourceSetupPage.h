//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2009, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
//#**********************************************************************
//# $Header: /Alaska/BIN/Modules/Template/PciPlatformResources/PciOutOfResourceSetupPage.h 1     3/30/11 9:51a Yakovlevs $Revision: 1 $
//#
//# $Date: 3/30/11 9:51a $
//#**********************************************************************
//# Revision History
//# ----------------
//# $Log: /Alaska/BIN/Modules/Template/PciPlatformResources/PciOutOfResourceSetupPage.h $
// 
// 1     3/30/11 9:51a Yakovlevs
// Initial Checkin of the component
// 
// 1     12/23/10 11:52a Bhimanadhunik
// Support added for to display PCI resource error in the setup.
//# 
//# 
//#**********************************************************************
//#<AMI_FHDR_START>
//#
//# Name:	PciOutOfResourceSetupPage.h
//#
//# Description:	
//#
//#<AMI_FHDR_END>
//#**********************************************************************
#ifndef __REFLASH__H__
#define __REFLASH__H__
#ifdef __cplusplus
extern "C"
{
#endif
#include <PciOutOfResourceSetupPageStrDefs.h>
#include <Token.h>
#ifndef VFRCOMPILE
#include <AmiDxeLib.h>
#endif

#define PCI_OUT_OF_RESOURCE_FORM_SET_CLASS             0x79

//932d37b0-0d4a-11e0-81e0-0800200c9a66

#define PCI_OUT_OF_RESOURCE_FORM_SET_GUID { 0x932d37b0, 0x0d4a, 0x11e0, 0x81, 0xe0, 0x08, 0x00, 0x20, 0x0c, 0x9a, 0x66 }


#define PCI_OUT_OF_RESOURCE_FORM_MAIN 1

#include <Setup.h>
#ifndef VFRCOMPILE

extern EFI_HII_HANDLE PciOutOfResourceSetupPageHiiHandle;

EFI_STATUS GetHiiString(
    IN EFI_HII_HANDLE HiiHandle,
    IN STRING_REF Token,
    IN OUT UINTN *pDataSize, OUT EFI_STRING *ppData
);
EFI_STATUS UpdateProgressString(
    IN STRING_REF Token, IN UINTN Progress, IN BOOLEAN Failed
);
#endif
    /****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2009, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
