/** @file
  PCH RESET Runtime Driver

@copyright
  Copyright (c) 1999 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

**/
#include <Library\PchSmmResetLib.h>

/**
  Execute Pch Reset from the host controller.

  @param[in] This                 Pointer to the PCH_RESET_PROTOCOL instance.
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
EFI_STATUS
EFIAPI
PchResetSmm (
  IN     PCH_RESET_TYPE           PchResetType
  )
{
  UINT16              PmBase;
  UINT16              GpioBase;
  UINT8               OutputData;
  UINT32              Data32;
  UINT16              Data16;
  PCH_SERIES          PchSeries;

  PchSeries         = GetPchSeries();

  PmBase = PciRead16 (
            PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_LPC,
            PCI_FUNCTION_NUMBER_PCH_LPC,
            R_PCH_LPC_ACPI_BASE)
            ) & B_PCH_LPC_ACPI_BASE_BAR;

  GpioBase = PciRead16 (
              PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC,
              R_PCH_LPC_GPIO_BASE)
              ) & B_PCH_LPC_GPIO_BASE_BAR;

  switch (PchResetType) {
// Grangeville AptioV override Start
  case WarmReset:
    IoWrite8 ((UINTN) R_PCH_RST_CNT, (UINT8) V_PCH_RST_CNT_HARDSTARTSTATE);
    OutputData = V_PCH_RST_CNT_HARDRESET;
    break;

  case ColdReset:
    IoWrite8 ((UINTN) R_PCH_RST_CNT, (UINT8) V_PCH_RST_CNT_HARDSTARTSTATE);
    OutputData = V_PCH_RST_CNT_FULLRESET;
    break;
// Grangeville AptioV override End
  case ShutdownReset:
    ///
    /// Firstly, ACPI decode must be enabled
    ///
    PciOr8 (
      PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
      PCI_DEVICE_NUMBER_PCH_LPC,
      PCI_FUNCTION_NUMBER_PCH_LPC,
      R_PCH_LPC_ACPI_CNT),
      (UINT8) (B_PCH_LPC_ACPI_CNT_ACPI_EN)
      );

    ///
    /// Then, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
    ///
    if (PchSeries == PchLp) {
      IoWrite32 ((UINTN) (PmBase + R_PCH_ACPI_GPE0_EN_127_96), 0);
    } else if (PchSeries == PchH) {
      IoWrite16 ((UINTN) (PmBase + R_PCH_ACPI_GPE0a_EN), 0);
      IoWrite16 ((UINTN) (PmBase + R_PCH_ACPI_GPE0b_EN), 0);
    }

    ///
    /// Secondly, PwrSts register must be cleared
    ///
    /// Write a "1" to bit[8] of power button status register at
    /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
    ///
    Data16 = B_PCH_SMI_STS_PM1_STS_REG;
    IoWrite16 ((UINTN) (PmBase + R_PCH_SMI_STS), Data16);

    ///
    /// Finally, transform system into S5 sleep state
    ///
    Data32  = IoRead32 ((UINTN) (PmBase + R_PCH_ACPI_PM1_CNT));

    Data32  = (UINT32) ((Data32 &~(B_PCH_ACPI_PM1_CNT_SLP_TYP + B_PCH_ACPI_PM1_CNT_SLP_EN)) | V_PCH_ACPI_PM1_CNT_S5);

    IoWrite32 ((UINTN) (PmBase + R_PCH_ACPI_PM1_CNT), Data32);

    Data32 = Data32 | B_PCH_ACPI_PM1_CNT_SLP_EN;

    IoWrite32 ((UINTN) (PmBase + R_PCH_ACPI_PM1_CNT), Data32);

    return EFI_SUCCESS;

  case PowerCycleReset:
  case GlobalReset:
  case GlobalResetWithEc:
    ///
    /// PCH BIOS Spec Rev 0.5.0, Section 4.6 GPIO Reset Requirement
    /// System BIOS is recommended to clear "GPIO Reset Select" registers [GP_RST_SEL1 (GPIOBASE + offset 60h),
    /// GP_RST_SEL2 (GPIOBASE + offset 64h) and GP_RST_SEL3 (GPIOBASE + offset 68h)] before issuing a hard or
    /// global reset unless specially requested by the platform designer.
    ///
    IoWrite32 ((UINTN) (GpioBase + R_PCH_GP_RST_SEL), 0);
    IoWrite32 ((UINTN) (GpioBase + R_PCH_GP_RST_SEL2), 0);
    IoWrite32 ((UINTN) (GpioBase + R_PCH_GP_RST_SEL3), 0);

    if ((PchResetType == GlobalReset) || (PchResetType == GlobalResetWithEc)) {
      PciOr32 (
        PCI_LIB_ADDRESS (DEFAULT_PCI_BUS_NUMBER_PCH,
        PCI_DEVICE_NUMBER_PCH_LPC,
        PCI_FUNCTION_NUMBER_PCH_LPC,
        R_PCH_LPC_PMIR),
        (UINT32) (B_PCH_LPC_PMIR_CF9GR)
        );
    }

    OutputData = V_PCH_RST_CNT_FULLRESET;
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  IoWrite8 ((UINTN) R_PCH_RST_CNT, OutputData);
  ///
  /// Waiting for system reset
  ///
  CpuDeadLoop();
  
  return EFI_SUCCESS;

}
