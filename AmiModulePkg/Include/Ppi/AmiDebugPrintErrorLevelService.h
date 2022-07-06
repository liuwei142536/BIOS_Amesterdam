//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
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
  @brief Ami Debug Print Error Level Service Ppi header file

  The file contains Ami Debug Print Error Level Service Ppi definitions.
**/

#ifndef __AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI__H__
#define __AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI__H__

/// AMI Debug Print Error Level Service PPI GUID definition
#define AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI_GUID \
    { 0x7cb7131b, 0x9b51, 0x45c8, { 0xb1, 0x5, 0x48, 0x1e, 0x98, 0x31, 0x48, 0x32 } }

typedef struct _AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI;

/**
  Get configured Debug Print Error Level for a module

  @param  This          Pointer to Ami Debug Print Error Level Service Ppi
  @param  ModuleGuid    Pointer to Module File Guid
  @param  ErrorLevel    Pointer to return current Debug Print Error Level mask for the current module

  @retval EFI_STATUS    Returns Success if module is found in Database, else Not Found
**/
typedef
EFI_STATUS
(EFIAPI *AMI_GET_DEBUG_PRINT_ERROR_LEVEL)(
  IN      AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI       *This,
  IN      EFI_GUID                                      *ModuleGuid,
  OUT     UINT32                                        *ErrorLevel
  );

/**
  Set/Configure Debug Print Error Level for a module

  @param  This          Pointer to Ami Debug Print Error Level Service Ppi
  @param  ModuleGuid    Pointer to Module File Guid
  @param  ErrorLevel    Debug Print Error Level mask to be Set/Configured for the current module

  @retval EFI_STATUS    Returns Success if module is found in Database, else Not Found
**/
typedef
EFI_STATUS
(EFIAPI *AMI_SET_DEBUG_PRINT_ERROR_LEVEL)(
  IN  AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI       *This,
  IN  EFI_GUID                                      *ModuleGuid,
  IN  UINT32                                        ErrorLevel
  );

struct _AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PPI {
    AMI_GET_DEBUG_PRINT_ERROR_LEVEL     GetPrintErrorLevel;
    AMI_SET_DEBUG_PRINT_ERROR_LEVEL     SetPrintErrorLevel;
};

extern EFI_GUID gAmiDebugPrintErrorLevelServicePpiGuid;

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
