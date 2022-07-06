/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/** @file
  This module provides an implementation of the SMM Control PPI for use with
  the PCH.

Copyright (c) 2011, Intel Corporation. All rights reserved.<BR>
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

**/

#include <PiPei.h>

#include <Ppi/SmmControl.h>

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>

#include <PchAccess.h>

/**
  Generates an SMI using the parameters passed in.

  @param  PeiServices         Describes the list of possible PEI Services.
  @param  This                A pointer to an instance of
                              EFI_SMM_CONTROL_PPI
  @param  ArgumentBuffer      The argument buffer
  @param  ArgumentBufferSize  The size of the argument buffer
  @param  Periodic            TRUE to indicate a periodical SMI
  @param  ActivationInterval  Interval of the periodical SMI

  @retval EFI_INVALID_PARAMETER  Periodic is TRUE or ArgumentBufferSize > 1
  @retval EFI_SUCCESS            SMI generated

**/
EFI_STATUS
EFIAPI
PeiActivate (
  IN      EFI_PEI_SERVICES           **PeiServices,
  IN      PEI_SMM_CONTROL_PPI        *This,
  IN OUT  INT8                       *ArgumentBuffer OPTIONAL,
  IN OUT  UINTN                      *ArgumentBufferSize OPTIONAL,
  IN      BOOLEAN                    Periodic OPTIONAL,
  IN      UINTN                      ActivationInterval OPTIONAL
  );

/**
  Clears an SMI.

  @param  PeiServices         Describes the list of possible PEI Services.
  @param  This                Pointer to an instance of EFI_SMM_CONTROL_PPI
  @param  Periodic            TRUE to indicate a periodical SMI

  @return Return value from SmmClear()

**/
EFI_STATUS
EFIAPI
PeiDeactivate (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  PEI_SMM_CONTROL_PPI         *This,
  IN  BOOLEAN                     Periodic OPTIONAL
  );

PEI_SMM_CONTROL_PPI      mSmmControlPpi = {
  PeiActivate,
  PeiDeactivate
};

EFI_PEI_PPI_DESCRIPTOR   mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiSmmControlPpiGuid,
  &mSmmControlPpi
};

/**
  Clear SMI related chipset status and re-enable SMI by setting the EOS bit.

  @retval EFI_SUCCESS       The requested operation has been carried out successfully
  @retval EFI_DEVICE_ERROR  The EOS bit could not be set.

**/
EFI_STATUS
SmmClear (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT32      OutputData;
  UINT32      OutputPort;
  UINT32      mPmBase;

  Status = EFI_SUCCESS;
  //
  // Get the Power Management I/O space base address.  We assume that
  // this base address has already been programmed if this driver is
  // being run.
  //
  mPmBase = MmioRead32 (
              PchPciDeviceMmBase (DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC) + R_PCH_LPC_ACPI_BASE
              ) & B_PCH_LPC_ACPI_BASE_BAR;

  //
  // Clear the Power Button Override Status Bit, it gates EOS from being set.
  //
  OutputPort  = mPmBase + R_PCH_ACPI_PM1_STS;
  OutputData  = B_PCH_ACPI_PM1_STS_PRBTNOR;
  DEBUG (
    (DEBUG_INFO,
    "The PM1 Status Port at address %x will be written to %x.\n",
    OutputPort,
    OutputData)
    );
  IoWrite16 (
    (UINTN) OutputPort,
    (UINT16) (OutputData)
    );

  //
  // Clear the APM SMI Status Bit
  //
  OutputPort  = mPmBase + R_PCH_SMI_STS;
  OutputData  = B_PCH_SMI_STS_APM;
  DEBUG (
    (DEBUG_INFO,
    "The SMI Status Port at address %x will be written to %x.\n",
    OutputPort,
    OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  //
  // Set the EOS Bit
  //
  OutputPort  = mPmBase + R_PCH_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= B_PCH_SMI_EN_EOS;
  DEBUG (
    (DEBUG_INFO,
    "The SMI Control Port at address %x will be written to %x.\n",
    OutputPort,
    OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  //
  // There is no need to read EOS back and check if it is set.
  // This can lead to a reading of zero if an SMI occurs right after the SMI_EN port read
  // but before the data is returned to the CPU.
  // SMM Dispatcher should make sure that EOS is set after all SMI sources are processed.
  //

  return Status;
}


EFI_STATUS
EFIAPI
SmmTrigger (
  IN UINT8   Data
  )
/*++

Routine Description:

  Trigger the software SMI

Arguments:

  Data                          The value to be set on the software SMI data port

Returns:

  EFI_SUCCESS                   Function completes successfully

--*/
{
  UINT32  OutputData;
  UINT32  OutputPort;
  UINT32      mPmBase;

  //
  // Get the Power Management I/O space base address.  We assume that
  // this base address has already been programmed if this driver is
  // being run.
  //
  mPmBase = MmioRead32 (
              PchPciDeviceMmBase (DEFAULT_PCI_BUS_NUMBER_PCH,
              PCI_DEVICE_NUMBER_PCH_LPC,
              PCI_FUNCTION_NUMBER_PCH_LPC) + R_PCH_LPC_ACPI_BASE
              ) & B_PCH_LPC_ACPI_BASE_BAR;


  //
  // Enable the APMC SMI
  //
  OutputPort  = mPmBase + R_PCH_SMI_EN;
  OutputData  = IoRead32 ((UINTN) OutputPort);
  OutputData |= (B_PCH_SMI_EN_APMC | B_PCH_SMI_EN_GBL_SMI);
  DEBUG (
    (DEBUG_INFO,
    "The SMI Control Port at address %x will be written to %x.\n",
    OutputPort,
    OutputData)
    );
  IoWrite32 (
    (UINTN) OutputPort,
    (UINT32) (OutputData)
    );

  OutputPort  = R_PCH_APM_CNT;
  OutputData  = Data;

  //
  // Generate the APMC SMI
  //
  IoWrite8 (
    (UINTN) OutputPort,
    (UINT8) (OutputData)
    );

  return EFI_SUCCESS;
}

/**
  Generates an SMI using the parameters passed in.

  @param  PeiServices         Describes the list of possible PEI Services.
  @param  This                A pointer to an instance of
                              EFI_SMM_CONTROL_PPI
  @param  ArgumentBuffer      The argument buffer
  @param  ArgumentBufferSize  The size of the argument buffer
  @param  Periodic            TRUE to indicate a periodical SMI
  @param  ActivationInterval  Interval of the periodical SMI

  @retval EFI_INVALID_PARAMETER  Periodic is TRUE or ArgumentBufferSize > 1
  @retval EFI_SUCCESS            SMI generated

**/
EFI_STATUS
EFIAPI
PeiActivate (
  IN      EFI_PEI_SERVICES           **PeiServices,
  IN      PEI_SMM_CONTROL_PPI        *This,
  IN OUT  INT8                       *ArgumentBuffer OPTIONAL,
  IN OUT  UINTN                      *ArgumentBufferSize OPTIONAL,
  IN      BOOLEAN                    Periodic OPTIONAL,
  IN      UINTN                      ActivationInterval OPTIONAL
  )
{
  INT8       Data;
  EFI_STATUS Status;
  //
  // Periodic SMI not supported.
  //
  if (Periodic) {
    DEBUG ((DEBUG_WARN, "Invalid parameter\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (ArgumentBuffer == NULL) {
    Data = 0xFF;
  } else {
    if (ArgumentBufferSize == NULL || *ArgumentBufferSize != 1) {
      return EFI_INVALID_PARAMETER;
    }

    Data = *ArgumentBuffer;
  }
  //
  // Clear any pending the APM SMI
  //
  Status = SmmClear ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return SmmTrigger (Data);
}

/**
  Clears an SMI.

  @param  PeiServices         Describes the list of possible PEI Services.
  @param  This                Pointer to an instance of EFI_SMM_CONTROL_PPI
  @param  Periodic            TRUE to indicate a periodical SMI

  @return Return value from SmmClear()

**/
EFI_STATUS
EFIAPI
PeiDeactivate (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  PEI_SMM_CONTROL_PPI         *This,
  IN  BOOLEAN                     Periodic OPTIONAL
  )
{
  if (Periodic) {
    return EFI_INVALID_PARAMETER;
  }
  return SmmClear ();
}

/**
  This is the constructor for the SMM Control Ppi.

  This function installs EFI_SMM_CONTROL_PPI.

  @param   FileHandle       Handle of the file being invoked.
  @param   PeiServices      Describes the list of possible PEI Services.

  @retval EFI_UNSUPPORTED There's no Intel ICH on this platform
  @return The status returned from InstallProtocolInterface().

--*/
EFI_STATUS
EFIAPI
SmmControlPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  Status      = (**PeiServices).InstallPpi (PeiServices, &mPpiList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
