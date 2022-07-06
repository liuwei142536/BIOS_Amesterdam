//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/SmmIchnDispatch2.h>
#include <AmiDxeLib.h>

EFI_RUNTIME_SERVICES  *gSmmRs = NULL;
EFI_SMM_ICHN_DISPATCH2_PROTOCOL *gIchnDispatch2;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID SmmRsTableGuid = EFI_SMM_RUNTIME_SERVICES_TABLE_GUID; 

/**
  Handler to write the proper century value to RTC RAM when the century 
  Rollover occurs

  @param[in] DispatchHandle       Not used
  @param[in] CallbackContext      Information about the IO trap that occurred

  @retval EFI_SUCCESS  RTC RAM is updated with proper century value
**/
EFI_STATUS
TcoSmiHandler(
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL )
{
    EFI_STATUS          Status;
    EFI_TIME            Time;
    UINTN               Index;

    //
    // Check for SmmRsTableGuid in configuration table
    //
    for (Index = 0; Index < gSmst->NumberOfTableEntries; Index++) {
      if (CompareGuid (&gSmst->SmmConfigurationTable[Index].VendorGuid, &SmmRsTableGuid)) {
        //
        // Found Smm Rs table
        //
        gSmmRs = gSmst->SmmConfigurationTable[Index].VendorTable;
      }
    }

    if ( gSmmRs == NULL ) {
        DEBUG ((EFI_D_LOAD, "Couldn't find SMM Runtime Services\n"));
        return EFI_NOT_FOUND;
    }

    //
    // Get the Current time and date information
    // 
    Status = gSmmRs->GetTime (&Time, NULL);
    ASSERT_EFI_ERROR (Status);

    //
    // Set the proper Year value
    //
    Time.Year = Time.Year + 100;

    //
    // Set the time and date information
    //
    Status = gSmmRs->SetTime (&Time);
    ASSERT_EFI_ERROR (Status);

    return EFI_SUCCESS;
}

/**
  Entry point for TCO SMI Handler driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
TcoSmiHandlerEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                      Status;
    EFI_HANDLE                      DispatchHandle;
    EFI_SMM_ICHN_REGISTER_CONTEXT   IchnContext;

    //
    // Get the ICHn protocol
    //
    Status = gSmst->SmmLocateProtocol(
                &gEfiSmmIchnDispatch2ProtocolGuid,
                NULL,
                &gIchnDispatch2
    );
    ASSERT_EFI_ERROR(Status);

    //
    // Register SMI Handler for IchnY2KRollover
    //

    IchnContext.Type = IchnY2KRollover;
    Status = gIchnDispatch2->Register(
                gIchnDispatch2,
                TcoSmiHandler,
                &IchnContext,
                &DispatchHandle
    );
    ASSERT_EFI_ERROR(Status);

    return EFI_SUCCESS;
}

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
