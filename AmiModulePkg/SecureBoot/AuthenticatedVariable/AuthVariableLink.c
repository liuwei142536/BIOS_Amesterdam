//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  AuthVariableInternal.c: Provides a hook PhysicalUserPresent() with default implementation 
  for Physical User Presence detection. Confirmation the Administrative User has signed in;
**/

#include <Setup.h>

//
// Global defines and external variables
// 
EFI_STATUS DxeGetVariable(
    IN CHAR16 *VariableName, IN EFI_GUID *VendorGuid,
    OUT UINT32 *Attributes OPTIONAL,
    IN OUT UINTN *DataSize, OUT VOID *Data
    );

//--------------------------------------------------------------------------------------------------------------

/**
  Default implementation for Physical User Presence detection.
  Confirmation the Administrative User has signed in;

  @param[in]   none
  
  @retval BOOLEAN TRUE is physical user is detected

**/

BOOLEAN PhysicalUserPresent ( VOID  )
{
    EFI_STATUS    Status;
static EFI_GUID  SystemAccessGuid  = SYSTEM_ACCESS_GUID;
    SYSTEM_ACCESS SystemAccess = {SYSTEM_PASSWORD_USER};
    UINTN         Size;
    UINT32        Attributes;

    Size = sizeof(SYSTEM_ACCESS);
    Status = DxeGetVariable(L"SystemAccess", &SystemAccessGuid, &Attributes, &Size, &SystemAccess);
//AVAR_TRACE((SecureBoot_DEBUG_LEVEL,"Admin=%x, %r, sz=%x, attr=%x\n", (SystemAccess.Access)?0:1, Status, Size, Attributes));
    if (!EFI_ERROR(Status) && (Attributes == EFI_VARIABLE_BOOTSERVICE_ACCESS)
         // Confirm the hook is called from inside of TSE Setup: Access==SYSTEM_PASSWORD_ADMIN
        && SystemAccess.Access==SYSTEM_PASSWORD_ADMIN)
    {
       return TRUE;
    }
    return FALSE;
}
