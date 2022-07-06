//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file
  Common file for the SMM driver. Contains the main code for FlashSmiSmm module.
  
  FlashSmi module will hook FLASH_PROTOCOL functions to genrate SMI to use
  functions in SMM.
  
**/

//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include <PiSmm.h>
#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SmmServicesTableLib.h>

#include <Library/AmiBufferValidationLib.h>

#include <Protocol/SmmBase.h>

#include <Protocol/AmiFlash.h>

#include "FlashSmi.h"

EFI_GUID                gFlashSmiGuid = FLASH_SMI_GUID;

AMI_FLASH_PROTOCOL      *gFlashSmm = NULL;

AMI_FLASH_ERASE         gSavedFlashEraseSmm;
AMI_FLASH_WRITE         gSavedFlashWriteSmm;
AMI_FLASH_UPDATE        gSavedFlashUpdateSmm;
AMI_FLASH_WRITE_ENABLE  gSavedWriteEnableSmm;
AMI_FLASH_WRITE_DISABLE gSavedWriteDisableSmm;

//----------------------------------------------------------------------------
// Function Definitions
/**
    Main SMI handler function.

    @param DispatchHandle The unique handle assigned to this handler by SmiHandlerRegister()
    @param Context Points to an optional handler context which was specified when the handler was registered
    @param CommBuffer A pointer to memory will be conveyed from a non-SMM environment into an SMM environment
    @param CommBufferSize The size of the CommBuffer

    @return EFI_STATUS

**/
EFI_STATUS
EFIAPI
FlashSmiHandler(
    IN      EFI_HANDLE  DispatchHandle,
    IN      CONST VOID  *Context        OPTIONAL,
    IN  OUT VOID        *CommBuffer     OPTIONAL,
    IN  OUT UINTN       *CommBufferSize OPTIONAL
)
{
    FLASH_SMI_UPDATE    *SmmFlashUpdate;
    EFI_STATUS          Status;

    if( CommBuffer == NULL )
        return EFI_SUCCESS;

    SmmFlashUpdate = (FLASH_SMI_UPDATE*)CommBuffer;

    Status = AmiValidateMemoryBuffer(
                (VOID*)SmmFlashUpdate,
                sizeof(FLASH_SMI_UPDATE) );
    if( EFI_ERROR(Status) )
        return EFI_SUCCESS;

    if( SmmFlashUpdate->Signature != FSMI_SIGNATURE )
        return EFI_SUCCESS;

    switch( SmmFlashUpdate->Subfunction )
    {
        case 'Un':
        {
            Status = gSmst->SmiHandlerUnRegister( DispatchHandle );
            SmmFlashUpdate->Status = Status;
            return EFI_SUCCESS;
        }
        break;

        case 'Fu':
        case 'Fw':
        {	
			if(EFI_ERROR(AmiValidateMemoryBuffer((VOID*)(SmmFlashUpdate->DataBuffer), SmmFlashUpdate->Size)))
			{
				SmmFlashUpdate->Status = EFI_ACCESS_DENIED;
                return EFI_SUCCESS;
			}
        }
        case 'Fe':
        {
            Status = AmiValidateMmioBuffer(
                        (VOID*)(SmmFlashUpdate->FlashAddress),
                        SmmFlashUpdate->Size );
            if( EFI_ERROR(Status) )
            {
                SmmFlashUpdate->Status = Status;
                return EFI_SUCCESS;
            }
        }
        break;
    }

    if( gFlashSmm == NULL )
        return EFI_SUCCESS;

    switch( SmmFlashUpdate->Subfunction )
    {
        // FlashUpdate call
        case 'Fu':
        {
            Status = gSavedFlashUpdateSmm(
                        (VOID*)(SmmFlashUpdate->FlashAddress),
                        SmmFlashUpdate->Size,
                        (VOID*)(SmmFlashUpdate->DataBuffer) );
        }
        break;

        // FlashErase call
        case 'Fe':
        {
            Status = gSavedFlashEraseSmm(
                        (VOID*)(SmmFlashUpdate->FlashAddress),
                        SmmFlashUpdate->Size );
        }
        break;

        // FlashWrite call
        case 'Fw':
        {
            Status = gSavedFlashWriteSmm(
                        (VOID*)(SmmFlashUpdate->FlashAddress),
                        SmmFlashUpdate->Size,
                        (VOID*)(SmmFlashUpdate->DataBuffer));
        }
        break;

        // WriteEnable call
        case 'We':
        {
            Status = gSavedWriteEnableSmm();
        }
        break;

        // WriteDisable call
        case 'Wd':
        {
            Status = gSavedWriteDisableSmm();
        }
        break;
        
        default:
        {
            SmmFlashUpdate->Status = EFI_UNSUPPORTED;
            return EFI_SUCCESS;
        }
    }

    SmmFlashUpdate->Status = Status;
    SmmFlashUpdate->Subfunction = 0;
    // Invalidate Flash SMI Buffer.
    SmmFlashUpdate->Signature = ~FSMI_SIGNATURE;

    return EFI_SUCCESS;
}
/**
    Main function in SMM.

    @param ImageHandle image handle
    @param SystemTable pointer to EFI_SYSTEM_TABLE

    @return VOID

**/
EFI_STATUS
FlashSmiSmmFunction(
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS  Status;
    EFI_HANDLE  Handle = NULL;

    // Locate SmmFlashProtocol.
    Status = gSmst->SmmLocateProtocol( &gAmiSmmFlashProtocolGuid, NULL, &gFlashSmm );
    if( EFI_ERROR(Status) )
        gFlashSmm = NULL;

    if( gFlashSmm != NULL )
    {
        gSavedFlashEraseSmm = gFlashSmm->Erase;
        gSavedFlashWriteSmm = gFlashSmm->Write;
        gSavedFlashUpdateSmm = gFlashSmm->Update;
        gSavedWriteEnableSmm = gFlashSmm->DeviceWriteEnable;
        gSavedWriteDisableSmm = gFlashSmm->DeviceWriteDisable;
    }

    // Register Flash Software SMI.
    Status = gSmst->SmiHandlerRegister(
                        FlashSmiHandler,
                        &gFlashSmiGuid,
                        &Handle );

    return Status;
}
/**
    Module entry point.

    @param ImageHandle image handle
    @param SystemTable pointer to EFI_SYSTEM_TABLE

    @return VOID

**/
EFI_STATUS
EFIAPI
FlashSmiSmmEntry(
    IN  EFI_HANDLE          ImageHandle,
    IN  EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_BOOT_MODE   Bootmode;

    Bootmode = GetBootModeHob();
    if( (Bootmode == BOOT_ON_FLASH_UPDATE) ||
        (Bootmode == BOOT_IN_RECOVERY_MODE) )
    {
        return FlashSmiSmmFunction( ImageHandle, SystemTable );
    }

    return EFI_UNSUPPORTED;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************