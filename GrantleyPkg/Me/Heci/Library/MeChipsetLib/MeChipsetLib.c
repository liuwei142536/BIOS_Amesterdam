/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2004 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:
  MeChipsetLib.c

@brief:
  Me Chipset Lib implementation.

**/
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/PciExpressLib.h>
#include "Library/PchPlatformLib.h"
#include "MeAccess.h"
#include "PchAccess.h"

VOID
MeDeviceControl (
  IN  ME_DEVICE             WhichDevice,
  IN  ME_DEVICE_FUNC_CTRL   DeviceFuncCtrl
  )
/**

  @brief
  Enable/Disable Me devices

  @param[in] WhichDevice          Select of Me device
  @param[in] DeviceFuncCtrl       Function control


**/
{
  switch (WhichDevice) {
    case HECI1:
    case HECI2:
    case IDER:
    case SOL:
      if (DeviceFuncCtrl) {
        Mmio16And (PCH_RCRB_BASE, R_PCH_RCRB_FD2, ~(BIT0 << WhichDevice));
      } else {
        Mmio16Or (PCH_RCRB_BASE, R_PCH_RCRB_FD2, BIT0 << WhichDevice);
      }
      Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FD2);
      break;

    case USBR1:
      if (DeviceFuncCtrl) {
        MmioOr16 (
          MmPciAddress (0,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PCI_DEVICE_NUMBER_PCH_USB,
          PCI_FUNCTION_NUMBER_PCH_EHCI,
          0x7A),
          (UINT16) (0x100)
          );
      } else {
        MmioAnd16 (
          MmPciAddress (0,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PCI_DEVICE_NUMBER_PCH_USB,
          PCI_FUNCTION_NUMBER_PCH_EHCI,
          0x7A),
          (UINT16) (~0x100)
          );
      }
      break;

    case USBR2:
      if (GetPchSeries() == PchH) {
        if (DeviceFuncCtrl) {
          MmioOr16 (
            MmPciAddress (0,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_USB_EXT,
            PCI_FUNCTION_NUMBER_PCH_EHCI2,
            0x7A),
            (UINT16) (0x100)
            );
        } else {
          MmioAnd16 (
            MmPciAddress (0,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_USB_EXT,
            PCI_FUNCTION_NUMBER_PCH_EHCI2,
            0x7A),
            (UINT16) (~0x100)
            );
        }
      }
      break;

    ///
    /// Function Disable SUS well lockdown
    ///
    case FDSWL:
      if (DeviceFuncCtrl) {
        Mmio16Or (PCH_RCRB_BASE, R_PCH_RCRB_FDSW, (UINT16) B_PCH_RCRB_FDSW_FDSWL);
      } else {
        Mmio16And (PCH_RCRB_BASE, R_PCH_RCRB_FDSW, (UINT16)~(B_PCH_RCRB_FDSW_FDSWL));
      }
      Mmio16 (PCH_RCRB_BASE, R_PCH_RCRB_FDSW);
      break;
  }
}
