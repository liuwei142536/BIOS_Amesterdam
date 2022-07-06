//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file Names.c
    Component name producer for PS/2 Controller DXE driver

**/

//----------------------------------------------------------------------

#include "Efi.h"
#include <Protocol/ComponentName.h>
#include "Ps2Ctl.h"

//----------------------------------------------------------------------

#ifndef EFI_COMPONENT_NAME2_PROTOCOL_GUID //old Core
#ifndef LANGUAGE_CODE_ENGLISH
#define LANGUAGE_CODE_ENGLISH "eng"
#endif
static BOOLEAN LanguageCodesEqual(
    CONST CHAR8* LangCode1, CONST CHAR8* LangCode2
)
{
    return    LangCode1[0]==LangCode2[0] 
           && LangCode1[1]==LangCode2[1]
           && LangCode1[2]==LangCode2[2];
}
#endif
//----------------------------------------------------------------------

EFI_STATUS 
EFIAPI 
PS2GetDriverName (
    IN EFI_COMPONENT_NAME_PROTOCOL  *This,
    IN CHAR8                        *Language,
    OUT CHAR16                      **DriverName
);

EFI_STATUS
EFIAPI 
PS2GetControllerName (
    IN EFI_COMPONENT_NAME_PROTOCOL  *This,
    IN EFI_HANDLE                   ControllerHandle,
    IN EFI_HANDLE                   ChildHandle        OPTIONAL,
    IN CHAR8                        *Language,
    OUT CHAR16                      **ControllerName
);

extern EFI_DRIVER_BINDING_PROTOCOL gPS2CtlDriverBinding;
extern PS2DEV_TABLE supportedDevices[];

//----------------------------------------------------------------------

CHAR16 *gPS2DriverName = L"AMI PS/2 Driver";

//==================================================================================
// Driver component name instance for PS2Ctl Driver
//==================================================================================
EFI_COMPONENT_NAME_PROTOCOL gPS2CtlDriverName = {
    PS2GetDriverName,
    PS2GetControllerName,
    LANGUAGE_CODE_ENGLISH
};

//----------------------------------------------------------------------


/**
    Retrieves a Unicode string that is the user readable name of 
    the EFI Driver.


    @param    This       - A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
              Language   - A pointer to a three character ISO 639-2 language identifier.
                           This is the language of the driver name that that the caller
                           is requesting, and it must match one of the languages specified
                           in SupportedLanguages.  The number of languages supported by a
                           driver is up to the driver writer.

              DriverName - A pointer to the Unicode string to return.  This Unicode string
                           is the name of the driver specified by This in the language
                           specified by Language.

         
    @retval    EFI_SUCCES The Unicode string for the Driver specified by This
               and the language specified by Language was returned
               in DriverName.
    @retval    EFI_INVALID_PARAMETER Language is NULL.
    @retval    EFI_INVALID_PARAMETER DriverName is NULL.
    @retval    EFI_UNSUPPORTED The driver specified by This does not support the
               language specified by Language.

**/

EFI_STATUS 
EFIAPI 
PS2GetDriverName (
    IN EFI_COMPONENT_NAME_PROTOCOL  *This,
    IN CHAR8                        *Language,
    OUT CHAR16                      **DriverName
)
{
    if(!Language || !DriverName) return EFI_INVALID_PARAMETER;
    if (!LanguageCodesEqual( Language, LANGUAGE_CODE_ENGLISH))
        return EFI_UNSUPPORTED;
    *DriverName = gPS2DriverName;
    return EFI_SUCCESS;
}


/**
    Retrieves a Unicode string that is the user readable name of
    the controller that is being managed by an EFI Driver.

    @param    This             - A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
              ControllerHandle - The handle of a controller that the driver specified by
                                 This is managing.  This handle specifies the controller
                                 whose name is to be returned.
              ChildHandle      - The handle of the child controller to retrieve the name
                                 of.  This is an optional parameter that may be NULL.  It
                                 will be NULL for device drivers.  It will also be NULL
                                 for a bus drivers that wish to retrieve the name of the
                                 bus controller.  It will not be NULL for a bus driver
                                 that wishes to retrieve the name of a child controller.
              *Language        - A pointer to a three character ISO 639-2 language
                                 identifier.  This is the language of the controller name
                                 that the caller is requesting, and it must match one
                                 of the languages specified in SupportedLanguages.  The
                                 number of languages supported by a driver is up to the
                                 driver writer.
              **ControllerName - A pointer to the Unicode string to return.  This Unicode
                                 string is the name of the controller specified by
                                 ControllerHandle and ChildHandle in the language
                                 specified by Language from the point of view of the
                                 driver specified by This.

         
    @retval    EFI_SUCCESS The Unicode string for the user readable name in the
                           language specified by Language for the driver
                           specified by This was returned in DriverName.
    @retval    EFI_INVALID_PARAMETER ControllerHandle is not a valid EFI_HANDLE.
    @retval    EFI_INVALID_PARAMETER ChildHandle is not NULL and it is not a valid
               EFI_HANDLE.
    @retval    EFI_INVALID_PARAMETER Language is NULL.
    @retval    EFI_INVALID_PARAMETER ControllerName is NULL.
    @retval    EFI_UNSUPPORTED The driver specified by This is not currently
                               managing the controller specified by
                               ControllerHandle and ChildHandle.
    @retval    EFI_UNSUPPORTED The driver specified by This does not support the
               language specified by Language.

**/

EFI_STATUS 
EFIAPI 
PS2GetControllerName (
    IN EFI_COMPONENT_NAME_PROTOCOL  *This,
    IN EFI_HANDLE                   Controller,
    IN EFI_HANDLE                   ChildHandle        OPTIONAL,
    IN CHAR8                        *Language,
    OUT CHAR16                      **ControllerName
)
{
    ACPI_HID_DEVICE_PATH    *acpiDP;
    PS2DEV_TABLE            *ps2dev = 0;
    
    if(!Language || !ControllerName || !Controller) return EFI_INVALID_PARAMETER;
    if (!LanguageCodesEqual( Language, LANGUAGE_CODE_ENGLISH))
        return EFI_UNSUPPORTED;

    // Find the last device node in the device path and return "Supported" 
    // for mouse and/or keyboard depending on the SDL switches.
    if(!EFI_ERROR(GetPS2_DP(&gPS2CtlDriverBinding, Controller, &acpiDP, EFI_OPEN_PROTOCOL_GET_PROTOCOL)) &&
            LookupPs2Hid(supportedDevices, acpiDP->HID, acpiDP->UID, &ps2dev)){
        *ControllerName = ps2dev->name;
        return EFI_SUCCESS;
    } 
    return EFI_UNSUPPORTED;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2016, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
