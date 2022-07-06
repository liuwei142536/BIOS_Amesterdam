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

/** @file NvmeIncludes.h
    Common header file for the Nvme Driver

**/

#ifndef _NVME_INCLUDES_H_
#define _NVME_INCLUDES_H_

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/BlockIo.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/ComponentName2.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/SmmBase.h>
#include <Protocol/SmmControl2.h>
#include <Protocol/SmmCommunication.h>
#include "NvmeInt13/NvmeInt13.h"
#include <Library/TimerLib.h>
#include <Protocol/StorageSecurityCommand.h>
#include <Protocol/AmiBlockIoWriteProtection.h>
#include <Protocol/NvmExpressPassthru.h>

//---------------------------------------------------------------------------
#define LANGUAGE_CODE_ENGLISH           "en-US"
#define MSG_NVME_DP                     23
#define MAX_NVME_READ_FAIL_RETRY_COUNT  0x3


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
