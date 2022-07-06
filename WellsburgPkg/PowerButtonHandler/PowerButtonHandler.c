//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>


EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *gSmmPowerbuttonDispatch2;

EFI_STATUS PowerButtonHandler(
		  IN EFI_HANDLE  DispatchHandle,
		  IN CONST VOID  *Context         OPTIONAL,
		  IN OUT VOID    *CommBuffer      OPTIONAL,
		  IN OUT UINTN   *CommBufferSize  OPTIONAL
)
{
	//Empty handler. SMM Child Dispatcher will power off board.
	return EFI_SUCCESS;
}

EFI_STATUS PowerButtonHandlerEntry(
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	EFI_HANDLE DispatchHandle;
	
	EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT Context = {EfiPowerButtonEntry};
	
	Status = gSmst->SmmLocateProtocol(
				&gEfiSmmPowerButtonDispatch2ProtocolGuid,
				NULL,
				&gSmmPowerbuttonDispatch2
	);
	ASSERT_EFI_ERROR(Status);

	Status = gSmmPowerbuttonDispatch2->Register(
				gSmmPowerbuttonDispatch2,
				PowerButtonHandler,
				&Context,
				&DispatchHandle
	);
	ASSERT_EFI_ERROR(Status);

	return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2013, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
