///
/// This file contains 'Framework Code' and is licensed as such
/// under the terms of your license agreement with Intel or your
/// vendor.  This file may not be modified, except as allowed by
/// additional terms of your license agreement.
///

/**

Copyright (c)  1999 - 2011 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


  @file 
  UsbController.h

  @brief 
  Usb Controller PPI as defined in EFI 2.0

  This code abstracts the PEI core to provide Usb Controller Info from Chipset.

**/
#ifndef _PEI_USB_CONTROLLER_PPI_H_
#define _PEI_USB_CONTROLLER_PPI_H_

#define PEI_EHCI_CONTROLLER 0x03

typedef struct _PEI_USB_CONTROLLER_PPI PEI_USB_CONTROLLER_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_GET_USB_CONTROLLER) (
  IN  EFI_PEI_SERVICES               **PeiServices,
  IN  PEI_USB_CONTROLLER_PPI         *This,
  IN  UINT8                          UsbControllerId,
  OUT UINTN                          *ControllerType,
  OUT UINTN                          *BaseAddress
  );

typedef struct _PEI_USB_CONTROLLER_PPI {
  PEI_GET_USB_CONTROLLER  GetUsbController;
} PEI_USB_CONTROLLER_PPI;

extern EFI_GUID gPeiUsbControllerPpiGuid;

#endif
