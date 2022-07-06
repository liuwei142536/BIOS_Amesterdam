/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement

Copyright (c) 2009-2010 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    WheaPlatform.c

Abstract:
    This is an implementation of the Whea Platform Support protocol. This is a
    runtime driver and provides Whea error logging functionality.

--------------------------------------------------------------------------------*/
#include <Library\DebugLib.h>
#include <Library\WheaErrorLib.h>
#include <Library\WheaPlatformHooksLib.h>
#include <Library\UefiBootServicesTableLib.h>
#include <Library\SmmServicesTableLib.h>
//
// Consumed protocols
//
#include <Protocol\SmmSwDispatch2.h>
#include <Protocol\SmmRtProtocol.h>

//
// Produced protocols
//
#include <Protocol\WheaPlatformSupport.h>


BOOLEAN                                 IsWheaEnableOS  = FALSE;    // OS not enabled WHEA yet
BOOLEAN                                 IsCeLogged      = FALSE;    // No CE logged yet
BOOLEAN                                 IsUeFatalLogged = FALSE;    // No UE/Fatal logged yet

EFI_SMM_BASE2_PROTOCOL                   *mSmmBase = NULL;
EFI_SMM_RUNTIME_PROTOCOL                *mSmmRT   = NULL;
static EFI_RUNTIME_SERVICES             *mRT;
EFI_SMM_SYSTEM_TABLE2          *mSmst = NULL;
//
// Driver entry point
//
EFI_STATUS
InstallWheaPlatform(
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  );

EFI_STATUS
WheaPlatformRuntimeInit (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  );

STATIC
EFI_STATUS
RestartAllWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *This
  );

STATIC
EFI_STATUS
RestartWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *This,
  IN UINT16                             Type
  );

STATIC
EFI_STATUS
EndWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *This
  );

STATIC
EFI_STATUS
PlatformElogMemory(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *This,
  IN UINT16                             ErrorType,
  IN MEMORY_DEV_INFO                    *MemInfo
  );

STATIC
EFI_STATUS
PlatformElogPcieRootDevBridge(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL *This,
  IN UINT16                             ErrorType,
  IN PCIE_PCI_DEV_INFO                  *ErrPcieDev
  );

STATIC
EFI_STATUS
PlatformElogPciDev(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN PCIE_PCI_DEV_INFO                      *ErrPcieDev
  );

STATIC
EFI_STATUS
PlatformElogIohDev(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN PCIE_PCI_DEV_INFO                      *ErrPcieDev,
  IN UINT32                                 ErrorSource
  );



EFI_SMM_RUNTIME_PROTOCOL *
GetSmmRt (
  IN  VOID
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:


Returns:

  GC_TODO: add return values

--*/
{
  if (mSmmRT == NULL) {
    if (EFI_ERROR (gBS->LocateProtocol (&gEfiSmmRuntimeProtocolGuid, NULL, &mSmmRT))) {
      mSmmRT = NULL;
    } else {
      if (mSmmRT->SmmRuntime.Hdr.Signature == EFI_RUNTIME_SERVICES_SIGNATURE) {
        mRT = &(mSmmRT->SmmRuntime);
      }
    }
  }

  return mSmmRT;
}

// Functions & Data structures
//
EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL \
                            mPlatformWheaInst = \
  {
    &RestartAllWheaElog,
    &RestartWheaElog,
    &EndWheaElog,
    &PlatformElogMemory,
    &PlatformElogPcieRootDevBridge,
    &PlatformElogPciDev,
    &PlatformElogIohDev
  };

///////////////////////////////////////////////////////////////////////////////////////
//
// Driver code/functions that installs all Error Sources (Common) in the platform & chipset
//
///////////////////////////////////////////////////////////////////////////////////////

//
// Driver functions
//

EFI_STATUS
InstallWheaPlatform(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
/*++

Routine Description: 

  Entry point of the Whea support driver. 

Arguments:  

  ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

Returns:  

  EFI_SUCCESS:              Driver initialized successfully
  EFI_LOAD_ERROR:           Failed to Initialize or has been loaded 
  EFI_OUT_OF_RESOURCES:     Could not allocate needed resources

--*/
{
  EFI_STATUS                    Status;
  BOOLEAN                       InSmm;
  EFI_WHEA_SUPPORT_PROTOCOL     *WheaDrv;
  UINTN                         i;
  
   InitWheaPlatformHookLib();

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &mSmmBase
                  );
 

  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
  }

  if (InSmm) {

    //
    // runtime Whea Platform support init
    //
    Status = WheaPlatformRuntimeInit(ImageHandle, SystemTable);
    //
    // DXE/PST initialization - create all error sources
    //
    Status = gBS->LocateProtocol (&gEfiWheaSupportProtocolGuid, NULL, &WheaDrv);
    if (EFI_ERROR(Status))return Status;

    //
    // Install all nativly handled error sources
    //
    for (i=0; i < NumNativeErrorSources; i++) {
      //
      // Report all WHEA error sources; SourceId and Error Status block address 
      // will be assigned by WHEA protocol driver and returned.
      //
      Status = WheaDrv->AddErrorSource(
                WheaDrv,
                SysNativeErrSources[i].Type,
                SysNativeErrSources[i].Flags,
                TRUE,
                &SysNativeErrSources[i].SourceId,
                SysNativeErrSources[i].NumOfRecords,
                SysNativeErrSources[i].MaxSecPerRecord,
                SysNativeErrSources[i].SourceData
             );
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Install all FW first model error source, i.e. all Generic HW errors 
    //
    for (i=0; i < NumSysGenericErrorSources; i++) {
      //
      // Report all WHEA error sources; SourceId and Error Status block address 
      // will be assigned by WHEA protocol driver and returned.
      //
      Status = WheaDrv->AddErrorSource(
                WheaDrv,
                GenericHw,
                SysGenErrSources[i].Flags,
                TRUE,
                &SysGenErrSources[i].SourceId,
                SysGenErrSources[i].NumOfRecords, // No of error records
                SysGenErrSources[i].MaxSecPerRecord, // No of error sections in the record
                SysGenErrSources[i].SourceData
             );
      ASSERT_EFI_ERROR (Status);
    }

    return Status;
  }
  return EFI_SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////////////
//
// Runtime Whea Error data log functions by the platform common 
//
///////////////////////////////////////////////////////////////////////////////////////


STATIC
EFI_STATUS
FindSysGenErrorSource(
  UINT16                        Type,
  SYSTEM_GENERIC_ERROR_SOURCE   **ErrSrc
  )
{
  UINTN i;

//
// Search System Generic error HW source structures
//
  *ErrSrc = NULL;
  for (i=0; i < NumSysGenericErrorSources; i++) {
    if (Type == SysGenErrSources[i].Type) {
      *ErrSrc = &SysGenErrSources[i];
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
} 

//
// This functions will intialize Error Status address block for all errors and ready
// for logging errors.
//
STATIC
EFI_STATUS
RestartAllWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This
  )
{
  GENERIC_ERROR_STATUS          *ErrStsBlk;
  GENERIC_HW_ERROR_SOURCE       *ErrSrc;
  UINTN                         i;
  UINTN                         MaxErrorStatusSize;

  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;
  IsCeLogged            = FALSE;
  IsUeFatalLogged       = FALSE;

  //
  // Clear all error data in all Error status blocks.
  //
  for (i=0; i < NumSysGenericErrorSources; i++) {
    ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysGenErrSources[i].SourceData;
    ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);
    ASSERT (ErrStsBlk != NULL);

    //
    // Find maximum Error status block size used
    //
    MaxErrorStatusSize = ErrSrc->MaxErrorDataSize;    // Raw data size
    if (ErrSrc->ErrorStatusSize > MaxErrorStatusSize) 
    {
      MaxErrorStatusSize = ErrSrc->ErrorStatusSize;    // Error Status size
    }
    ZeroMem((VOID *)ErrStsBlk, MaxErrorStatusSize);
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
RestartWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType
  )
{
  EFI_STATUS                        Status;
  GENERIC_ERROR_STATUS              *ErrStsBlk;
  GENERIC_HW_ERROR_SOURCE           *ErrSrc;
  SYSTEM_GENERIC_ERROR_SOURCE       *SysErrSrc;
  UINTN                             MaxErrorStatusSize;

  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;
  IsCeLogged            = FALSE;
  IsUeFatalLogged       = FALSE;

  if ((ErrorType & 0x03) == 0x01) {
    ErrorType = GENERIC_ERROR_FATAL;
  } else {
    ErrorType = GENERIC_ERROR_CORRECTED;
  }

  //
  // Clear all error data in all Error status blocks.
  //
  // Get the Error source structure
  //
  Status = FindSysGenErrorSource(ErrorType, &SysErrSrc);
  if (EFI_ERROR(Status) || SysErrSrc == NULL) {
    return EFI_NOT_FOUND;
  }

  ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysErrSrc->SourceData;
  ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);
  ASSERT (ErrStsBlk != NULL);

  //
  // Find maximum Error status block size used
  //
  MaxErrorStatusSize = ErrSrc->MaxErrorDataSize;    // Raw data size
  if (ErrSrc->ErrorStatusSize > MaxErrorStatusSize) 
  {
    MaxErrorStatusSize = ErrSrc->ErrorStatusSize;    // Error Status size
  }
  ZeroMem((VOID *)ErrStsBlk, MaxErrorStatusSize);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EndWheaElog( 
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This
  )
{
  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;

  if (IsCeLogged) {
    TriggerWheaSci(mSmmBase, mSmst);
  }
  
  if (IsUeFatalLogged) {
    //
    // Not require since our platform will trigger NMI on UE/Fatal error.
    //
    //TriggerWheaNmi();
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
PlatformElogMemory(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN MEMORY_DEV_INFO                        *MemInfo
  )  
{
    EFI_STATUS                  Status;
    SYSTEM_GENERIC_ERROR_SOURCE *SysErrSrc;
    GENERIC_ERROR_STATUS        *ErrStsBlk;
    GENERIC_HW_ERROR_SOURCE     *ErrSrc;


  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;
  if (ErrorType == GEN_ERR_SEV_PLATFORM_MEMORY_FATAL) {
    IsUeFatalLogged       = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_FATAL, &SysErrSrc);
  } else {
    IsCeLogged            = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_CORRECTED, &SysErrSrc);
  }

  //
  // Get the Error source structure
  //
  if (EFI_ERROR(Status) || SysErrSrc == NULL) {
    return EFI_NOT_FOUND;
  }

  ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysErrSrc->SourceData;
  ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);

  if (ErrStsBlk == NULL) {
    return EFI_NOT_FOUND;
  }

  return WheaGenElogMemory (ErrorType, MemInfo, ErrStsBlk);
}

STATIC
EFI_STATUS
PlatformElogPcieRootDevBridge(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN PCIE_PCI_DEV_INFO                      *ErrPcieDev
  )
{
    EFI_STATUS                  Status;
    SYSTEM_GENERIC_ERROR_SOURCE *SysErrSrc;
    GENERIC_ERROR_STATUS        *ErrStsBlk;
    GENERIC_HW_ERROR_SOURCE     *ErrSrc;

  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;
  if( ErrorType == GEN_ERR_SEV_PCIE_RECOVERABLE || ErrorType == GEN_ERR_SEV_PCIE_CORRECTED) {
    IsCeLogged            = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_CORRECTED, &SysErrSrc);
  } else {
    IsUeFatalLogged       = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_FATAL, &SysErrSrc);
  }

  //
  // Get the Error status block address
  //
  if (EFI_ERROR(Status) || SysErrSrc == NULL) {
    return EFI_NOT_FOUND;
  }

  ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysErrSrc->SourceData;
  ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);

  if (ErrStsBlk == NULL) {
    return EFI_NOT_FOUND;
  }

  return WheaGenElogPcieRootDevBridge (ErrorType, ErrPcieDev, ErrStsBlk);
}

STATIC
EFI_STATUS
PlatformElogPciDev(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN PCIE_PCI_DEV_INFO                      *ErrPcieDev
  )
{
    EFI_STATUS                  Status;
    SYSTEM_GENERIC_ERROR_SOURCE *SysErrSrc;
    GENERIC_ERROR_STATUS        *ErrStsBlk;
    GENERIC_HW_ERROR_SOURCE     *ErrSrc;

  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) return EFI_NOT_READY;
  if( ErrorType == GEN_ERR_SEV_PCI_DEV_RECOVERABLE || ErrorType == GEN_ERR_SEV_PCI_DEV_CORRECTED) {
    IsCeLogged            = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_CORRECTED, &SysErrSrc);
  } else {
    IsUeFatalLogged       = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_FATAL, &SysErrSrc);
  }

  //
  // Get the Error status block address
  //
  if (EFI_ERROR(Status) || SysErrSrc == NULL) {
    return EFI_NOT_FOUND;
  }

  ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysErrSrc->SourceData;
  ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);

  if (ErrStsBlk == NULL) {
    return EFI_NOT_FOUND;
  }

  return WheaGenElogPciDev (ErrorType, ErrPcieDev, ErrStsBlk);
}

STATIC
EFI_STATUS
PlatformElogIohDev(
  IN EFI_WHEA_PLATFORM_SUPPORT_PROTOCOL     *This,
  IN UINT16                                 ErrorType,
  IN PCIE_PCI_DEV_INFO                      *ErrIohDev,
  IN UINT32                                 ErrorSource
  )
{
  EFI_STATUS                    Status;
  SYSTEM_GENERIC_ERROR_SOURCE   *SysErrSrc;
  GENERIC_ERROR_STATUS          *ErrStsBlk;
  GENERIC_HW_ERROR_SOURCE       *ErrSrc;

  //
  // Check if Whea OS active
  //
  if (!IsWheaEnableOS) {
    return EFI_NOT_READY;
  }

  if ( ErrorType == GEN_ERR_SEV_IOH_RECOVERABLE || ErrorType == GEN_ERR_SEV_IOH_CORRECTED) {
    IsCeLogged            = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_CORRECTED, &SysErrSrc);
  } else {
    IsUeFatalLogged       = TRUE;
    Status = FindSysGenErrorSource(GENERIC_ERROR_FATAL, &SysErrSrc);
  }

  if (EFI_ERROR(Status) || SysErrSrc == NULL) {
    return EFI_NOT_FOUND;
  }

  ErrSrc = (GENERIC_HW_ERROR_SOURCE *)SysErrSrc->SourceData;
  ErrStsBlk = (GENERIC_ERROR_STATUS *)(*(UINT64 *)ErrSrc->ErrorStatusAddress.Address);

  if (ErrStsBlk == NULL) {
    return EFI_NOT_FOUND;
  }

  return WheaGenElogIohDev (ErrorType, ErrIohDev, ErrStsBlk, ErrorSource);
}

///////////////////////////////////////////////////////////////////////////////////////
//
// Runtime driver code/functions that enables Error path/signal for WHEA
//
///////////////////////////////////////////////////////////////////////////////////////

EFI_STATUS
WheaPlatformEnable (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )
{
  IsWheaEnableOS = TRUE; 
  //
  // Call platform hook for SKU or board specific programming
  //
  //ProgramWheaSignalsHook (IsWheaEnableOS, mSmmBase, mSmst); // AptioV Server Override: Commented the function as Polling is used instead of SCI for Error Notification.

  return EFI_SUCCESS;
}

EFI_STATUS
WheaPlatformDisable (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  )
{
  IsWheaEnableOS = FALSE;
  //
  // Call platform hook for SKU or board specific programming
  //
  //ProgramWheaSignalsHook (IsWheaEnableOS, mSmmBase, mSmst); // AptioV Server Override: Commented the function as Polling is used instead of SCI for Error Notification.

  return EFI_SUCCESS;
}

EFI_STATUS
WheaPlatformRuntimeInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                        Status;
  EFI_HANDLE                        SwHandle;
  BOOLEAN                             InSmm;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL      *SwDispatch  = 0;

   Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &mSmmBase
                  );
 

  if (mSmmBase == NULL) {
    InSmm = FALSE;
  } else {
    mSmmBase->InSmm (mSmmBase, &InSmm);
  }

  if (InSmm) {
    //
    // We're now in SMM
    // Initialize module global variables
    //
    Status  = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &mSmmBase);
    mSmmBase->GetSmstLocation (mSmmBase, &mSmst);

    //
    // Register our SMI handlers
    //
    Status =   mSmst->SmmLocateProtocol ( &gEfiSmmSwDispatch2ProtocolGuid, NULL, &SwDispatch);
    ASSERT_EFI_ERROR (Status);

    //
    // Install SW SMI handler to enable WHEA
    //
    SwContext.SwSmiInputValue = EFI_WHEA_ENABLE_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, WheaPlatformEnable, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status);

    //
    // Install SW SMI handler to disable WHEA
    //
    SwContext.SwSmiInputValue = EFI_WHEA_DISABLE_SWSMI;
    Status                    = SwDispatch->Register (SwDispatch, WheaPlatformDisable, &SwContext, &SwHandle);
    ASSERT_EFI_ERROR (Status); 

    //
    // Install runtime protocol for WHEA error log functions
    //
    SwHandle = NULL;
	
	gBS->InstallProtocolInterface (
                                &SwHandle,
                                &gEfiWheaPlatformSupportProtocolGuid,
                                EFI_NATIVE_INTERFACE,
                                &mPlatformWheaInst
                            );

    ASSERT_EFI_ERROR (Status);
  }
  return EFI_SUCCESS;

}


