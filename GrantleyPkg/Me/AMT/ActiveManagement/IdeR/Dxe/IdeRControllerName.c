/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  IdeRControllerName.c

@brief:

  This portion is to register the IDE Redirect Controller Driver name

**/
#include <Library/UefiLib.h>
#include "IdeRController.h"

///
/// Forward reference declaration
///
EFI_STATUS
IdeRControllerGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

EFI_STATUS
IdeRControllerGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle OPTIONAL,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  );

///
/// EFI Component Name Protocol
/// This portion declares a gloabl variable of EFI_COMPONENT_NAME_PROTOCOL type.
/// It initializes the followings:
///      .GetDriverName()     to PlatformIdeGetDriverName() and
///      SupportedLanguages   to "eng" (3 char ISO639-2 language indetifier)
///
EFI_COMPONENT_NAME_PROTOCOL     mIdeRControllerName = {
  IdeRControllerGetDriverName,
  IdeRControllerGetControllerName,
  "eng" /// English
};

//
// EFI Component Name 2 Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_COMPONENT_NAME2_PROTOCOL mIdeRControllerName2 = {
  (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) IdeRControllerGetDriverName,
  (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) IdeRControllerGetControllerName,
  "en"
};

///
///  Define the Driver's unicode name string
///  IDE controller Driver name string and IDE Controller Name string
///
static EFI_UNICODE_STRING_TABLE mIdeRControllerDriverNameTable[] = {
  {
    "eng",
    L"IDER Controller Init Driver"
  },
  {
    NULL,
    NULL
  }
};

static EFI_UNICODE_STRING_TABLE mIdeRControllerControllerNameTable[] = {
  {
    "eng",
    L"IDER Controller"
  },
  {
    NULL,
    NULL
  }
};

///
/// /////////////////////////////////////////////////////////////////////////////////
///
EFI_STATUS
IdeRControllerGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  )
/**

  @brief
  This is a function definition of EFI_COMPONENT_NAME_PROTOCOL.GetDriverName(). This function
  is to provide the user readable name of IDE Driver, defined in mPlaformIdeNameTable
  This function is called by the platform management utilities to display the name of component.

  @param[in] This                 The address of protocol
  @param[in] Language             If the caller specificed Language matches SupportedLanguage, a pointer
                                  to the Driver name is returned in the DriverName.
  @param[in] DriverName           If the caller specificed Language matches SupportedLanguage, a pointer
                                  to the Driver name is returned in the DriverName.

  @retval EFI_SUCCESS             If the caller specificed Language matches SupportedLanguage.
                                  i.e. Language == gPlatformIdeName.SupportedLanguages
  @exception EFI_UNSUPPORTED      If the caller specificed Language doesn't match SupportedLanguage.

**/
{

  return LookupUnicodeString (
          Language,
          mIdeRControllerName.SupportedLanguages,
          mIdeRControllerDriverNameTable,
          DriverName
          );

}

EFI_STATUS
IdeRControllerGetControllerName (
  IN  EFI_COMPONENT_NAME_PROTOCOL   *This,
  IN  EFI_HANDLE                    ControllerHandle,
  IN  EFI_HANDLE                    ChildHandle OPTIONAL,
  IN  CHAR8                         *Language,
  OUT CHAR16                        **ControllerName
  )
/**

  @brief
    Retrieves a Unicode string that is the user readable name of the controller
    that is being managed by an EFI Driver.

  @param[in] This                 A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in] ControllerHandle     The handle of a controller that the driver specified by
                                  This is managing.  This handle specifies the controller
                                  whose name is to be returned.
  @param[in] ChildHandle          The handle of the child controller to retrieve the name
                                  of. This is an optional parameter that may be NULL.  It
                                  will be NULL for device drivers.  It will also be NULL
                                  for a bus drivers that wish to retrieve the name of the
                                  bus controller.  It will not be NULL for a bus driver
                                  that wishes to retrieve the name of a child controller.
  @param[in] Language             A pointer to a three character ISO 639-2 language
                                  identifier.  This is the language of the controller name
                                  that that the caller is requesting, and it must match one
                                  of the languages specified in SupportedLanguages.  The
                                  number of languages supported by a driver is up to the
                                  driver writer.
  @param[in] ControllerName       A pointer to the Unicode string to return. This Unicode
                                  string is the name of the controller specified by
                                  ControllerHandle and ChildHandle in the language
                                  specified by Language from the point of view of the
                                  driver specified by This.

  @retval EFI_SUCCESS             The Unicode string for the user readable name in the
                                  language specified by Language for the driver
                                  specified by This was returned in DriverName.
  @retval EFI_INVALID_PARAMETER   ControllerHandle is not a valid EFI_HANDLE. Language
                                  or ControllerName is NULL.
  @exception EFI_UNSUPPORTED      The driver specified by this is not currently
                                  managing the controller specified by
                                  ControllerHandle and ChildHandle. Or the driver
                                  specified by This does not support the language specified
                                  by Language.

**/
{
  return LookupUnicodeString (
          Language,
          mIdeRControllerName.SupportedLanguages,
          mIdeRControllerControllerNameTable,
          ControllerName
          );
}
