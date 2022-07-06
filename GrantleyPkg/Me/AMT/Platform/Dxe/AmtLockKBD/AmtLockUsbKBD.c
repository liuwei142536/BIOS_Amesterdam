//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmtLockUsbKBD.c
    AMT Lock USB KeyBoard Functions.

**/
#include <AmiDxeLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/AmiUsbController.h>
#include <Protocol/AlertStandardFormat.h>
//============================================================================
// Type definitions
//======================================================================
//Function Prototypes
//======================================================================
// Global and extern variables
EFI_SMM_SYSTEM_TABLE2   *gSmst2 = NULL;
AMI_USB_SMM_PROTOCOL    *gAmiUsbSmmProtocol = NULL;

/**
    Use UsbRtKbcAccessControl() to lock keyboard.

          
    @param DispatchHandle 
    @param Context 
    @param CommBuffer 
    @param CommBufferSize 

    @retval EFI_STATUS Status


**/
EFI_STATUS
EFIAPI
AMTLockUsbKBDSmi (
  IN     EFI_HANDLE     DispatchHandle,
  IN     CONST VOID     *Context,
  IN OUT VOID           *CommBuffer,
  IN OUT UINTN          *CommBufferSize
  )
{
    URP_STRUC   UrpStruc;
    API_FUNC    UsbRtKbcAccessControl;
    
    if (gAmiUsbSmmProtocol == NULL) {
        return EFI_SUCCESS;
    }
    
    // USBAPI_KbcAccessControl is the 17th API_FUNC in UsbApiTable.    
    UrpStruc.ApiData.KbcControlCode = 1;    
    UsbRtKbcAccessControl = (API_FUNC)gAmiUsbSmmProtocol->UsbApiTable[17];
    if (UsbRtKbcAccessControl != NULL) {
        UsbRtKbcAccessControl(&UrpStruc);
    }
    return EFI_SUCCESS;
}
/**
    While USB_RUNTIME_DRIVER_IN_SMM is on, the
    UsbRtKbcAccessControl() will be ready after the
    gAmiUsbSmmProtocolGuid is installed.
    Register the AMTLockUsbKBDSmi after gAmiUsbSmmProtocolGuid is installed.

    @param Protocol 
    @param Interface 
    @param Handle 

    @retval EFI_STATUS Status


**/
EFI_STATUS
EFIAPI
AmiUsbSmmProtocolCallback (
    CONST EFI_GUID  *Protocol,
    VOID            *Interface,
    EFI_HANDLE      Handle
)
{
    EFI_STATUS      Status;
    EFI_HANDLE      SmiHandle = NULL;
    
    Status = gSmst2->SmmLocateProtocol(&gAmiUsbSmmProtocolGuid , NULL,
                                        &gAmiUsbSmmProtocol);
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    Status = gSmst2->SmiHandlerRegister( &AMTLockUsbKBDSmi,
                                         &gEfiEventLegacyBootGuid, &SmiHandle );
    if (EFI_ERROR(Status)) {
        return Status;
    }
    
	return Status;
}
/**
    If the BootOption is set with LockKeyboard, register the
    event with gEfiEventLegacyBootGuid or register SMI directly.
    The PiSmmIpl will trigger it to lock keyboard.

    @param ImageHandle 
    @param SystemTable 

    @retval EFI_STATUS Status


**/
EFI_STATUS
InSmmFunction(
    IN EFI_HANDLE          ImageHandle,
    IN EFI_SYSTEM_TABLE    *SystemTable
)
{
    EFI_STATUS Status;
    EFI_SMM_BASE2_PROTOCOL          *pEfiSmmBase2Protocol = NULL;
    EFI_ALERT_STANDARD_FORMAT_PROTOCOL  *pAlertStandardFormatProtocol = NULL;
    VOID	                        *AmiUsbSmmProtocolReg = NULL;
    EFI_ASF_BOOT_OPTIONS                *pAsfBootOptions = NULL;
    EFI_HANDLE                      SmiHandle = NULL;
    
    Status = pBS->LocateProtocol( &gEfiSmmBase2ProtocolGuid, NULL,
                                  &pEfiSmmBase2Protocol );
    if( !EFI_ERROR(Status) )
    {
        Status = pEfiSmmBase2Protocol->GetSmstLocation(
                    pEfiSmmBase2Protocol, &gSmst2 );
        if( EFI_ERROR(Status) ) return Status;
    }

    Status = pBS->LocateProtocol( &gEfiAlertStandardFormatProtocolGuid, NULL,
                                  &pAlertStandardFormatProtocol );
    if( !EFI_ERROR(Status) )
    {
        Status = pAlertStandardFormatProtocol->GetBootOptions (
                        pAlertStandardFormatProtocol, &pAsfBootOptions) ;
        if( EFI_ERROR(Status) ) return Status;
    }
    
    if( pAsfBootOptions->BootOptions & LOCK_KEYBOARD )
    {
#if USB_RUNTIME_DRIVER_IN_SMM
        Status = gSmst2->SmmLocateProtocol(&gAmiUsbSmmProtocolGuid , NULL,
                                            &gAmiUsbSmmProtocol);
        if(!EFI_ERROR(Status))
        {
            Status = gSmst2->SmiHandlerRegister( &AMTLockUsbKBDSmi,
                                                 &gEfiEventLegacyBootGuid,
                                                 &SmiHandle );
        }
        else
        {
            Status = gSmst2->SmmRegisterProtocolNotify (
                                &gAmiUsbSmmProtocolGuid,
                                AmiUsbSmmProtocolCallback,
                                &AmiUsbSmmProtocolReg );
        }
#endif
    }
    return Status;
}
/**
    Lock USB Keyboard Entrypoint.

    @param ImageHandle 
    @param SystemTable 

    @retval EFI_STATUS Status

**/
EFI_STATUS
EFIAPI
AMTLockUsbKBDEntryPoint(
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    InitAmiLib(ImageHandle, SystemTable);
    return InitSmmHandlerEx (ImageHandle, SystemTable, InSmmFunction, NULL);
}

