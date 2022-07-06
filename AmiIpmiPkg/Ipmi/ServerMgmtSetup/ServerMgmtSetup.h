//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file ServerMgmtSetup.h
    Server Mgmt setup header file

**/

#ifndef __SERVER_MGMT_SETUP_PAGE_H__
#define __SERVER_MGMT_SETUP_PAGE_H__
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------

#include "ServerMgmtSetupVariable.h"
#include <Setup.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <AmiDxeLib.h>
#include <UefiHii.h>
#include <Protocol/HiiString.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiUtilities.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/IpmiUsbTransportProtocol.h>
#include <Library/BaseMemoryLib.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>

//-----------------------------------------------------------------------

//-----------------------------------------------------------------------

#define EFI_BDS_EVENT_BEFORE_SETUP_GUID  \
  { 0xdbc9fd21, 0xfad8, 0x45b0, { 0x9e, 0x78, 0x27, 0x15, 0x88, 0x67, 0xcc, 0x93 } };

//-----------------------------------------------------------------------

#ifndef VFRCOMPILE
VOID UpdateUsbInIpmiInterfacesListCallback(
  IN EFI_EVENT  Event,
  IN VOID       *Context  
  );
#endif//VFRCOMPILE
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
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
