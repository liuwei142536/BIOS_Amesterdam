//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file ComponentName.c
    AMI USB Component Name definitions

**/

#include <AmiDef.h>
#include "ComponentName.h"

CHAR16*
AmiUsbDriverGetControllerName(
    EFI_HANDLE  Controller,
    EFI_HANDLE  Child
);

CHAR16*
UsbBusGetControllerName(
    EFI_HANDLE  Controller,
    EFI_HANDLE  Child
);                                  

/// Driver name for the Usb driver binding protocol
static CHAR16 *gDriverNameUsb=L"AMI USB Driver";
static CHAR16 *gControllerNameUsb=L"USB Host Controller";

/// Driver name for the UsbBus driver binding protocol
static CHAR16 *gDriverNameUsbBus=L"AMI USB Bus Driver";
static CHAR16 *gControllerNameUsbBus=L"USB Host Controller (USBBUS)";

/// Driver name for the UsbMass driver binding protocol
static CHAR16 *gDriverNameUsbMass=L"AMI USB Mass Storage Driver";
static CHAR16 *gControllerNameUsbMass=L"USB Mass Storage Device";

/// Driver name for the UsbHid driver binding protocol
static CHAR16 *gDriverNameUsbHid=L"AMI USB Hid Driver";
static CHAR16 *gControllerNameUsbHid=L"USB Hid Device";

/// Driver name for the UsbCcid driver binding protocol
static CHAR16 *gDriverNameUsbCcid=L"AMI USB CCID Driver";
static CHAR16 *gControllerNameUsbCcid=L"USB CCID Device";

/// Component name protocol for the Usb driver binding protocol
EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsb = {
    AmiUsbComponentNameGetDriverName,
    AmiUsbComponentNameGetControllerName,
    LANGUAGE_CODE_ENGLISH_2
};

/// Component name protocol for the Usb Bus binding protocol
EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbBus = {
    AmiUsbComponentNameGetDriverName,
    AmiUsbComponentNameGetControllerName,
    LANGUAGE_CODE_ENGLISH_2
};

/// Component name protocol for the Usb Mass binding protocol
EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbMass = {
    AmiUsbComponentNameGetDriverName,
    AmiUsbComponentNameGetControllerName,
    LANGUAGE_CODE_ENGLISH_2
};

/// Component name protocol for the Usb Hid binding protocol
EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbHid = {
    AmiUsbComponentNameGetDriverName,
    AmiUsbComponentNameGetControllerName,
    LANGUAGE_CODE_ENGLISH_2
};

/// Component name protocol for the Usb Hid binding protocol
EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbCcid = {
    AmiUsbComponentNameGetDriverName,
    AmiUsbComponentNameGetControllerName,
    LANGUAGE_CODE_ENGLISH_2
};

/**
    Retrieves a Unicode string that is the user readable name of
    the EFI Driver.

    @param This       A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
    @param Language   A pointer to a three character ISO 639-2 language identifier.
                      This is the language of the driver name that that the caller
                      is requesting, and it must match one of the languages specified
                      in SupportedLanguages.  The number of languages supported by a
                      driver is up to the driver writer.
    @param DriverName A pointer to the Unicode string to return.  This Unicode string
                      is the name of the driver specified by This in the language
                      specified by Language.

         
    @retval EFI_SUCCES            The Unicode string for the Driver specified by This
                                  and the language specified by Language was returned
                                  in DriverName.
    @retval EFI_INVALID_PARAMETER Language is NULL.
    @retval EFI_INVALID_PARAMETER DriverName is NULL.
    @retval EFI_UNSUPPORTED       The driver specified by This does not support the
                                  language specified by Language.

**/

EFI_STATUS
EFIAPI
AmiUsbComponentNameGetDriverName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  CHAR8                         *Language,
    OUT CHAR16                        **DriverName
)
{
    UINT8    LanMatch = FALSE;
    
     //Supports only English
    if(!Language || !DriverName)
        return EFI_INVALID_PARAMETER;

    if (AsciiStrCmp( Language, LANGUAGE_CODE_ENGLISH_2)==0)
        LanMatch = TRUE;

    if (LanMatch==FALSE)
        return EFI_UNSUPPORTED;

    if(This == &gComponentNameUsb)
        *DriverName = gDriverNameUsb;
    else if(This == &gComponentNameUsbBus)
        *DriverName = gDriverNameUsbBus;
    else if(This == &gComponentNameUsbMass)
        *DriverName = gDriverNameUsbMass;
    else if(This == &gComponentNameUsbHid)
        *DriverName = gDriverNameUsbHid;
    else if(This == &gComponentNameUsbCcid)
        *DriverName = gDriverNameUsbCcid;

    return  EFI_SUCCESS;
}


/**
    Retrieves a Unicode string that is the user readable name of
    the controller that is being managed by an EFI Driver.
 
    @param This             A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
    @param ControllerHandle The handle of a controller that the driver specified by
                            This is managing.  This handle specifies the controller
                            whose name is to be returned.
    @param ChildHandle      The handle of the child controller to retrieve the name
                            of.  This is an optional parameter that may be NULL.  It
                            will be NULL for device drivers.  It will also be NULL
                            for a bus drivers that wish to retrieve the name of the
                            bus controller.  It will not be NULL for a bus driver
                            that wishes to retrieve the name of a child controller.
                            Language         - A pointer to a three character ISO 639-2 language
                            identifier.  This is the language of the controller name
                            that that the caller is requesting, and it must match one
                            of the languages specified in SupportedLanguages.  The
                            number of languages supported by a driver is up to the
                            driver writer.
    @param ControllerName   A pointer to the Unicode string to return.  This Unicode
                            string is the name of the controller specified by
                            ControllerHandle and ChildHandle in the language
                            specified by Language from the point of view of the
                            driver specified by This.

         
    @retval EFI_SUCCESS     The Unicode string for the user readable name in the
                            language specified by Language for the driver
                            specified by This was returned in DriverName.
    @retval EFI_INVALID_PARAMETER  ControllerHandle is not a valid EFI_HANDLE.
    @retval EFI_INVALID_PARAMETER  ChildHandle is not NULL and it is not a valid
                                   EFI_HANDLE.
    @retval EFI_INVALID_PARAMETER  Language is NULL.
    @retval EFI_INVALID_PARAMETER  ControllerName is NULL.
    @retval EFI_UNSUPPORTED        The driver specified by This is not currently
                                   managing the controller specified by
                                   ControllerHandle and ChildHandle.
    @retval EFI_UNSUPPORTED        The driver specified by This does not support the
                                   language specified by Language.

**/

EFI_STATUS
EFIAPI
AmiUsbComponentNameGetControllerName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  EFI_HANDLE                    ControllerHandle,
    IN  EFI_HANDLE                    ChildHandle        OPTIONAL,
    IN  CHAR8                         *Language,
    OUT CHAR16                        **ControllerName
  )
{
    UINT8    LanMatch = FALSE;

    //Supports only English
    if(!Language || !ControllerName || !ControllerHandle)
        return EFI_INVALID_PARAMETER;
        
    if (AsciiStrCmp( Language, LANGUAGE_CODE_ENGLISH_2)==0)
        LanMatch = TRUE;

    if (LanMatch==FALSE)
        return EFI_UNSUPPORTED;

    if(This == &gComponentNameUsb){
        *ControllerName = AmiUsbDriverGetControllerName(ControllerHandle,ChildHandle);
    }
    else if(This == &gComponentNameUsbBus){
    	*ControllerName = UsbBusGetControllerName(ControllerHandle,ChildHandle);
    }
    else if(This == &gComponentNameUsbMass){
        *ControllerName = UsbBusGetControllerName(ControllerHandle,ChildHandle);
    }
    else if(This == &gComponentNameUsbHid){
        *ControllerName = UsbBusGetControllerName(ControllerHandle,ChildHandle);
    }
    else if(This == &gComponentNameUsbCcid){
        *ControllerName = UsbBusGetControllerName(ControllerHandle,ChildHandle);
    }
    
    if (*ControllerName==0) {
        return EFI_UNSUPPORTED;
    }

    return EFI_SUCCESS;

}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
