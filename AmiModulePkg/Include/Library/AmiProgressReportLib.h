//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file
  Definition of the AmiProgressReportLib library class.
*/
#ifndef __AMI_PROGRESS_REPORT_LIB__H__
#define __AMI_PROGRESS_REPORT_LIB__H__

#include <Uefi.h>

/// The library class is used by the ReFlash module to show flash process progress

/**
  Shows progress of the operation

  @param PercentComplete Operation completeness percentage (0 - 100)

  @retval  EFI_SUCCESS  Progress displayed
  @retval  Other        Error occurred during operation
*/
EFI_STATUS AmiShowProgress (
    IN UINTN PercentComplete    //(0% - 100%)
);

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
