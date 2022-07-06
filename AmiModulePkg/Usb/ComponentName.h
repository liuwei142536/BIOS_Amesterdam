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

/** @file ComponentName.h
    AMI USB driver component name header file

**/

#ifndef _AMIUSB_COMPONENT_NAME_H__
#define _AMIUSB_COMPONENT_NAME_H__

#include <Protocol/ComponentName2.h>
#include <Library/BaseLib.h>

#ifndef LANGUAGE_CODE_ENGLISH_2
#define LANGUAGE_CODE_ENGLISH_2 "en"
#endif

extern EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsb;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbBus;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbMass;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbHid;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentNameUsbCcid;
//
// EFI Component Name Functions
//
EFI_STATUS
EFIAPI
AmiUsbComponentNameGetDriverName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  CHAR8                         *Language,
    OUT CHAR16                        **DriverName
);

EFI_STATUS
EFIAPI
AmiUsbComponentNameGetControllerName (
    IN  EFI_COMPONENT_NAME2_PROTOCOL  *This,
    IN  EFI_HANDLE                    ControllerHandle,
    IN  EFI_HANDLE                    ChildHandle        OPTIONAL,
    IN  CHAR8                         *Language,
    OUT CHAR16                        **ControllerName
  );
                                        //<(EIP59272)
#endif //_AMIUSB_COMPONENT_NAME_H__

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
