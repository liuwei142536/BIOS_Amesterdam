/** @file
  Initializes PCH USB Controllers.

@copyright
  Copyright (c) 2009 - 2012 Intel Corporation. All rights reserved
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
#include "PchInitPeim.h"

/**
  The function performing USB init in PEI phase. This could be used by USB recovery 
  or debug features that need USB initialization during PEI phase.
  Note: Before executing this function, please be sure that PCH_INIT_PPI.Initialize 
  has been done and PchUsbPolicyPpi has been installed.

  @param[in] PeiServices    General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval Others            All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
PchUsbInit (
  IN CONST EFI_PEI_SERVICES             **PeiServices
  ){
  EFI_STATUS          Status;
  PCH_USB_POLICY_PPI  *PchUsbPolicyPpi;

  DEBUG ((DEBUG_INFO, "PchUsbInit() - Start\n"));

  ///
  /// Get PchUsbPolicy PPI for PCH_USB_CONFIG
  ///
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPchUsbPolicyPpiGuid,
                            0,
                            NULL,
                            (VOID **)&PchUsbPolicyPpi
                            );

  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "\n------------------------ PchUsbPolicyPpi Dump Begin -----------------\n"));
    DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", PchUsbPolicyPpi->Revision));
    DEBUG ((DEBUG_INFO, "Mode : 0x%x\n", PchUsbPolicyPpi->Mode));
    DEBUG ((DEBUG_INFO, "EhciMemBaseAddr : 0x%x\n", PchUsbPolicyPpi->EhciMemBaseAddr));
    DEBUG ((DEBUG_INFO, "EhciMemLength : 0x%x\n", PchUsbPolicyPpi->EhciMemLength));
    DEBUG ((DEBUG_INFO, "XhciMemBaseAddr : 0x%x\n", PchUsbPolicyPpi->XhciMemBaseAddr));
    Status = PchStartUsbInit (
              PchUsbPolicyPpi->UsbConfig,
              (UINT32) PchUsbPolicyPpi->EhciMemBaseAddr,
              (UINT32) PchUsbPolicyPpi->XhciMemBaseAddr,
              PchUsbPolicyPpi->Revision
              );
    DEBUG ((DEBUG_INFO, "\n"));
    DEBUG ((DEBUG_INFO, "\n------------------------ PchUsbPolicyPpi Dump End -----------------\n"));
  }
  DEBUG ((EFI_D_INFO, "PchUsbInit() - End\n"));
  return Status;
}

/**
  The function performing USB init in PEI phase. This could be
  used by USB recovery ,debug features or usb precondition
  enabled case that need USB initialization during PEI phase.
  Please be sure the function should not be executed in if the
  boot mode is S3 resume.

  @param[in] UsbConfig            Pointer to a PCH_USB_CONFIG that provides the platform setting
  @param[in] EhciMemBaseAddr      Predefined Ehci memory base address for Ehci hc configuration
  @param[in] XhciMemBaseAddr      Predefined Xhci memory base address for Xhci hc configuration
  @param[in] Revision             Revision of PCH_USB_CONFIG

  @retval EFI_SUCCESS             The function completed successfully
  @retval Others                  All other error conditions encountered result in an ASSERT
**/
EFI_STATUS
PchStartUsbInit (
  IN PCH_USB_CONFIG               *UsbConfig,
  IN UINT32                       EhciMemBaseAddr,
  IN UINT32                       XhciMemBaseAddr,
  IN UINT8                        Revision
  )
{
  EFI_STATUS          Status;
  UINT32              RootComplexBar;
  UINT32              FuncDisableReg;

  DEBUG ((EFI_D_INFO, "PchStartUsbInit() - Start\n"));
  Status = EFI_INVALID_PARAMETER;
  if (UsbConfig != NULL) {
    RootComplexBar  = PCH_RCRB_BASE;
    FuncDisableReg  = MmioRead32 (RootComplexBar + R_PCH_RCRB_FUNC_DIS);

    Status = CommonUsbInit (
              UsbConfig,
              (UINT32) EhciMemBaseAddr,
              (UINT32) XhciMemBaseAddr,
              0,
              RootComplexBar,
              &FuncDisableReg,
              Revision
              );

    ASSERT_EFI_ERROR (Status);

    MmioWrite32 ((UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS), (UINT32) (FuncDisableReg));
    //
    // Reads back for posted write to take effect
    //
    MmioRead32 ((UINTN) (RootComplexBar + R_PCH_RCRB_FUNC_DIS));
  }

  DEBUG ((DEBUG_INFO, "PchUsbInit() - End\n"));

  return Status;

}
