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
  @brief Ami Debug Print Error Level Service Protocol header file

  The file contains Ami Debug Print Error Level Service Protocol definitions.
**/

#ifndef __AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL__H__
#define __AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL__H__

/// AMI Debug Print Error Level Service Protocol GUID definition
#define AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL_GUID \
    { 0x1103ce13, 0x8f69, 0x4cf2, { 0x8d, 0x41, 0xb8, 0x9, 0x28, 0x6c, 0xe5, 0x14 } }

typedef struct _AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL;

/**
  Get configured Debug Print Error Level for a module

  @param  This          Pointer to Ami Debug Print Error Level Service Protocol
  @param  ModuleGuid    Pointer to Module File Guid
  @param  ErrorLevel    Pointer to return current Debug Print Error Level mask for the current module

  @retval EFI_STATUS    Returns Success if module is found in Database, else Not Found
**/
typedef
EFI_STATUS
(EFIAPI *AMI_GET_DEBUG_PRINT_ERROR_LEVEL)(
  IN      AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL  *This,
  IN      EFI_GUID                                      *ModuleGuid,
  OUT     UINT32                                        *ErrorLevel
  );

/**
  Set/Configure Debug Print Error Level for a module

  @param  This          Pointer to Ami Debug Print Error Level Service Protocol
  @param  ModuleGuid    Pointer to Module File Guid
  @param  ErrorLevel    Debug Print Error Level mask to be Set/Configured for the current module

  @retval EFI_STATUS    Returns Success if module is found in Database, else Not Found
**/
typedef
EFI_STATUS
(EFIAPI *AMI_SET_DEBUG_PRINT_ERROR_LEVEL)(
  IN      AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL  *This,
  IN      EFI_GUID                                      *ModuleGuid,
  IN      UINT32                                        ErrorLevel
  );

struct _AMI_DEBUG_PRINT_ERROR_LEVEL_SERVICE_PROTOCOL{
    AMI_GET_DEBUG_PRINT_ERROR_LEVEL     GetPrintErrorLevel;
    AMI_SET_DEBUG_PRINT_ERROR_LEVEL     SetPrintErrorLevel;
};

extern EFI_GUID gAmiDebugPrintErrorLevelServiceProtocolGuid;

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
