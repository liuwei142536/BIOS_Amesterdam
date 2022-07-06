//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

//**********************************************************************
// $Header:  $
//
// $Revision:  $
//
// $Date:  $
//**********************************************************************
// Revision History
// ----------------
// $Log:  $
// 
// 
// 
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:  RtErrorLogGpnv.h
//
// Description: Runtime error log GPNV includes and declarations
//
//<AMI_FHDR_END>
//**********************************************************************

#ifndef _RT_ERROR_LOG_GPNV_H
#define _RT_ERROR_LOG_GPNV_H

//
// Include files
//
#include "token.h"
#include <Protocol/SmbiosElogSupport.h>
#include <Protocol/GenericElogProtocol.h>
#include <Include/PCIE.h>
#include <Include/PCI.h>
#include <Library/SmmServicesTableLib.h>
#include <Library\DebugLib.h>
#include <Protocol\GenericElogProtocol.h>
#include <Library\IoLib.h>
#include "RtErrorLogBoard.h"

//
// Function Declaration
//
EFI_STATUS
LogMemErrorToGpnv(
  IN MEMORY_DEV_INFO     *MemInfo
 );

EFI_STATUS
LogPciErrorToGpnv (
  IN UINT8    ErrorSource,
  IN UINT8    ErrType,
  IN UINT8    Segment,
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Func,
  IN BOOLEAN  IsBridge,
  IN UINT16   SecondaryStatus,
  IN UINT8    PcieCapOffset,
  IN UINT16   PcieDeviceStatus,
  IN UINT16   AerOffset,
  IN UINT32   UncorrErrStatus,
  IN UINT32   CorrErrStatus,
  IN UINT32   RootErrStatus
 );

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
