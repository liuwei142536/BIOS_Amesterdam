//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
/** @file
  PCH SMM Extended protocol.

  Copyright (c) 2008 - 2011, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

**/
#ifndef _EFI_SMM_ICHN_DISPATCH2_EX_H_
#define _EFI_SMM_ICHN_DISPATCH2_EX_H_

//
// GUID for the SmmIchnDispatch Extended Protocol
//
#define EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL_GUID \
  {0x3920405b, 0xc897, 0x44da, {0x88, 0xf3, 0x4c, 0x49, 0x8a, 0x6f, 0xf7, 0x36}}

//
// Extern the GUID for protocol users.
//
extern EFI_GUID gEfiSmmIchnDispatch2ExProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL;

//
// Related Definitions
//
//
// Ichn Dispatch Extended Types
//
typedef enum {
    IchnExPciExpress = NUM_ICHN_TYPES + 1,
    IchnExMonitor,
    IchnExSpi,
    IchnExQRT,
    IchnExGpioUnlock,
    IchnExTmrOverflow,
    IchnExPcieReserved1,
    IchnExPcieReserved2,
    IchnExPcie0Hotplug,
    IchnExPcie1Hotplug,
    IchnExPcie2Hotplug,
    IchnExPcie3Hotplug,
    IchnExPcie4Hotplug,
    IchnExPcie5Hotplug,
    IchnExPcie6Hotplug,
    IchnExPcie7Hotplug,
    IchnExPcie0LinkActive,
    IchnExPcie1LinkActive,
    IchnExPcie2LinkActive,
    IchnExPcie3LinkActive,
    IchnExPcie4LinkActive,
    IchnExPcie5LinkActive,
    IchnExPcie6LinkActive,
    IchnExPcie7LinkActive,
    //
    // INSERT NEW ITEMS JUST BEFORE THIS LINE
    //
    IchnExTypeMAX // the maximum number of items in this enumeration

} EFI_SMM_ICHN_EX_SMI_TYPE;

typedef struct {
  EFI_SMM_ICHN_EX_SMI_TYPE  Type;
} EFI_SMM_ICHN_EX_REGISTER_CONTEXT;


typedef
EFI_STATUS
(EFIAPI *EFI_SMM_ICHN_EX_DISPATCH2_REGISTER) (
  IN CONST EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2       DispatchFunction,
  IN OUT   EFI_SMM_ICHN_EX_REGISTER_CONTEXT   *RegisterContext,
     OUT   EFI_HANDLE                         *DispatchHandle
  );

/*++

  Routine Description:
    Register a child SMI source dispatch function with a parent SMM driver

  Arguments:
    This                  - Protocol instance pointer.
    DispatchFunction      - Pointer to dispatch function to be invoked for
                            this SMI source
    RegisterContext       - Pointer to the dispatch function's context.
                            The caller fills this context in before calling
                            the register function to indicate to the register
                            function the ICHN SMI source for which the dispatch
                            function should be invoked.
    DispatchHandle        - Handle of dispatch function, for when interfacing
                            with the parent SMM driver.

  Returns:
    EFI_SUCCESS           - The dispatch function has been successfully
                            registered and the SMI source has been enabled.
    EFI_DEVICE_ERROR      - The driver was unable to enable the SMI source.
    EFI_OUT_OF_RESOURCES  - Not enough memory (system or SMM) to manage this
                            child.
    EFI_INVALID_PARAMETER - DispatchContext is invalid. The ICHN input value
                            is not within valid range.

--*/
typedef
EFI_STATUS
(EFIAPI *EFI_SMM_ICHN_EX_DISPATCH2_UNREGISTER) (
  IN  EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL  *This,
  IN  EFI_HANDLE                          DispatchHandle
  );

/*++

  Routine Description:
    Unregister a child SMI source dispatch function with a parent SMM driver

  Arguments:
    This                  - Protocol instance pointer.
    DispatchHandle        - Handle of dispatch function to deregister.

  Returns:
    EFI_SUCCESS           - The dispatch function has been successfully
                            unregistered and the SMI source has been disabled
                            if there are no other registered child dispatch
                            functions for this SMI source.
    EFI_INVALID_PARAMETER - Handle is invalid.
    other                 - TBD

--*/

//
// Interface structure for the SMM Ich n specific SMI Dispatch Protocol
//
struct _EFI_SMM_ICHN_DISPATCH2_EX_PROTOCOL {
  EFI_SMM_ICHN_EX_DISPATCH2_REGISTER    Register;
  EFI_SMM_ICHN_EX_DISPATCH2_UNREGISTER  UnRegister;
};

#endif
