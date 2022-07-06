//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1987-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file CpuSpSmi.c
Provides CPU MSR writes for S3 Resume.
Detailed description: This file description is OK.  File name is provided.
*/

#include <Library/SmmServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Token.h>

#define RESTORE_MSR_CALLBACK_GUID \
    {0x2b6487ee, 0x6d58, 0x4d83, 0x90, 0xef, 0x79, 0x50, 0xec, 0xb7, 0xd6, 0x8e}


EFI_GUID RestoreMsrCallbackGuid =  RESTORE_MSR_CALLBACK_GUID;

/**
  Writes MSR value(s) on logical processor.
  
  This function provides a service to send and receive messages from a registered UEFI service.

  @param[in] Buffer     Pointer not used.

  @retval VOID
**/

VOID RestoreMsrOnCpu(IN VOID *Buffer)
{
    //Currently no MSRs.
}   

/**
    This function restores the CPU MSRs during S3 resume.
    @param  DispatchHandle      Not Used.
    @param  Context             Not Used.
    @param  CommBuffer          Not Used.
    @param  CommBufferSize      Not Used.

    @retval EFI_SUCCESS         Always
**/

EFI_STATUS RestoreCpuMsr (
    IN  EFI_HANDLE                  DispatchHandle,
	IN CONST VOID                   *Context OPTIONAL,
	IN OUT VOID                     *CommBuffer OPTIONAL,
	IN OUT UINTN                    *CommBufferSize OPTIONAL
)
{
//Currently no MSRs
#if 0
    UINT32  i;

    DEBUG((-1,"Write MSR Lock Bit.\n"));
    
    for (i = 0; i < gSmst->NumberOfCpus; ++i) {
        gSmst->SmmStartupThisAp(RestoreMsrOnCpu, i, NULL);
    }
	
    RestoreMsrOnCpu(NULL);
#endif    
    return EFI_SUCCESS;
}


/**
    Initializes CPU specific SMM Drivers.

    @param  ImageHandle     Not Used.
    @param  SystemTable     Not Used.

    @retval EFI_SUCCESS     Always
**/

EFI_STATUS CpuSpSmiInit(
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_STATUS  Status;
    EFI_HANDLE  DispatchHandle;

    Status = gSmst->SmiHandlerRegister(
        RestoreCpuMsr,
        &RestoreMsrCallbackGuid,
        &DispatchHandle
    );
    ASSERT_EFI_ERROR(Status);
    
    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1987-2014, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Pkwy, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

