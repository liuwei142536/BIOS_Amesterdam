//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  SmiFlash DXE Driver File.
**/

//----------------------------------------------------------------------
// header includes
#include <AmiDxeLib.h>
#include <Token.h>
#include <SMIFlashELinks.h>
#if (AMIUSB_SUPPORT == 1) && (USB_DRIVER_MAJOR_VER < 10)
#include <Guid/EventGroup.h>
#include <Protocol/AmiUsbController.h>
#endif

#define AFU_RUNTIME_BUFFER_LENGTH 64
//----------------------------------------------------------------------
// component MACROs

//----------------------------------------------------------------------
// Module defined global variables

//----------------------------------------------------------------------
// Module specific global variable
// oem flash write enable/disable list creation code must be in this order
typedef VOID (SMIFLASH_INIT) (VOID);
extern SMIFLASH_INIT SMIFLASH_NOT_IN_SMM_LIST EndOfNotInSmmList;
SMIFLASH_INIT* SMIFlashNotInSmm[] = {SMIFLASH_NOT_IN_SMM_LIST NULL};

//----------------------------------------------------------------------
// externally defined variables

//----------------------------------------------------------------------
// Function definitions
#if (AMIUSB_SUPPORT == 1) && (USB_DRIVER_MAJOR_VER < 10)
extern EFI_GUID gBdsAllDriversConnectedProtocolGuid;
/**
 * Callback function executed when the gEfiEventExitBootServicesGuid is installed.
 * Callback code will delete the USBProtocol pointer..
 *
 * @param Event Pointer to the EFI_EVENT
 * @param Context Pointer to the context for this event
 */
VOID
SmiFlashExitBootServicesNotify (
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
    EFI_STATUS  Status;
    EFI_GUID    AmiGlobalVariableGuid = AMI_GLOBAL_VARIABLE_GUID;
    UINT32      VarAttr = 0;
    UINTN       VarSize = 0;
    UINTN       gSMIAmiUsb = 0;

    pBS->CloseEvent (Event);

    VarSize = sizeof(UINTN);
    Status = pRS->GetVariable (L"USB_POINT", \
                &AmiGlobalVariableGuid, &VarAttr, &VarSize, &gSMIAmiUsb);
    if (!EFI_ERROR(Status)) {
        VarSize = 0;
        gSMIAmiUsb = 0;
        pRS->SetVariable(L"USB_POINT", \
            &AmiGlobalVariableGuid, VarAttr, VarSize, &gSMIAmiUsb);
    }
}
/**
 * Callback function executed when the EFI_USB protocol is installed by the USB Driver.
 * Callback code will save the USBProtocol pointer to Volatile NVRAM.
 *
 * @param Event Pointer to the EFI_EVENT
 * @param Context Pointer to the context for this event
 */
VOID
GetUsbProtocolPoint(
    IN EFI_EVENT   Event,
    IN VOID        *Context
)
{
    EFI_GUID            AmiGlobalVariableGuid = AMI_GLOBAL_VARIABLE_GUID;
    EFI_USB_PROTOCOL    *AmiUsb = NULL;
    EFI_STATUS          Status;

    Status = pBS->LocateProtocol(&gEfiUsbProtocolGuid, NULL, &AmiUsb);
    if (EFI_ERROR(Status)) return;
    // Directly savieUsbRtKbcAccessControl function to avoid SmiFlash calls outside SMM.
    Status = pRS->SetVariable (  L"USB_POINT",
                        &AmiGlobalVariableGuid,
                        EFI_VARIABLE_RUNTIME_ACCESS |
                        EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof(VOID*),
                        (VOID*)&AmiUsb->UsbRtKbcAccessControl);
    return;
}
#endif //AMIUSB_SUPPORT
/**
 * Entry pointer to the SmiFlashDxeDriver. 
 *
 * @param ImageHandle Pointer to the handle that corresponds to this image
 * @param SystemTable Pointer to the EFI_SYSTEM_TABLE
 *
 * @retval EFI_SUCCESS This driver's entry point executed correctly
 */
EFI_STATUS 
EFIAPI
SmiFlashDxeEntry (
    IN EFI_HANDLE       ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
)
{
    UINTN       i;

    InitAmiLib(ImageHandle, SystemTable);
#if (AMIUSB_SUPPORT == 1) && (USB_DRIVER_MAJOR_VER < 10)
    {
        EFI_EVENT   EvtProtocolEvt = NULL;
        VOID        *RgnUsbProtocol = NULL;
        EFI_EVENT   ExitBootEvt = NULL;
        // Because of the UsbRtKbcAccessControl function is updated in USB InSmmFunction,
        // Register gBdsAllDriversConnectedProtocol callback for saving USB Protocol 
        // address for runtime used.
        RegisterProtocolCallback (&gBdsAllDriversConnectedProtocolGuid, \
                GetUsbProtocolPoint, NULL, &EvtProtocolEvt, &RgnUsbProtocol);
    
        pBS->CreateEventEx (EVT_NOTIFY_SIGNAL, \
                            TPL_NOTIFY, \
                            SmiFlashExitBootServicesNotify, \
                            NULL, \
                            &gEfiEventExitBootServicesGuid, \
                            &ExitBootEvt );
    }                
#endif //AMIUSB_SUPPORT

#if defined RECOVERY_PRESERVE_VARS_IN_SMM && RECOVERY_PRESERVE_VARS_IN_SMM == 1
    {
        EFI_STATUS      Status;
        EFI_BOOT_MODE   Bootmode;
        VOID            *Buffer;
        EFI_GUID        AmiGlobalVariableGuid = AMI_GLOBAL_VARIABLE_GUID;

        Bootmode = GetBootMode();
        if((Bootmode == BOOT_ON_FLASH_UPDATE) || (Bootmode == BOOT_IN_RECOVERY_MODE))
        {
            Buffer = NULL;
            Status = pBS->AllocatePool(
                        EfiRuntimeServicesData,
                        AFU_RUNTIME_BUFFER_LENGTH,
                        &Buffer );

            if(!EFI_ERROR(Status))
            {
                MemSet( Buffer, AFU_RUNTIME_BUFFER_LENGTH, 0 );
                Status = pRS->SetVariable (L"SFPFREC",
                        &AmiGlobalVariableGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof(VOID*),
                        (VOID*)(&Buffer));
            }
        } 
    }
#endif

    for (i = 0; SMIFlashNotInSmm[i] != NULL; SMIFlashNotInSmm[i++]());
    return EFI_SUCCESS;
}