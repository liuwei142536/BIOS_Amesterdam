/** @file
  This is the PEIM that performs the S3 resume tasks.

@copyright
  Copyright (c) 2008 - 2017 Intel Corporation. All rights reserved
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
#ifndef _PCH_S3_PEIM_H_
#define _PCH_S3_PEIM_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include "PchAccess.h"
#include "Library/PchPlatformLib.h"
//#include "PchPlatformPolicy.h"
#include "Library/PchPciExpressHelpersLib.h"
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h> 
#include <Library/DebugLib.h>
#include "Protocol/PchS3Support.h"
#include <Ppi/ReadOnlyVariable.h>
#include <Ppi/ReadOnlyVariable2.h>


#define EFI_PCH_S3_STALL_INTERVAL 50  ///< us

/**
  This routine is used to search SMRAM and get SmramCpuData point.

  @param[in] PeiServices  - PEI services global pointer
  @param[in] SmmAccessPpi - SmmAccess PPI instance

  @retval SmramCpuData - The pointer of CPU information in SMRAM.
**/
EFI_STATUS
CreateS3DataHob (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  );

/**
  Send Codec command on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_DEVICE_ERROR        Device status error, operation failed
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
PchS3SendCodecCommand (
  EFI_PCH_S3_PARAMETER_SEND_CODEC_COMMAND      *Parameter
  );

/**
  Perform Init Root Port Downstream devices on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3InitPcieRootPortDownstream (
  EFI_PCH_S3_PARAMETER_INIT_PCIE_ROOT_PORT_DOWNSTREAM     *Parameter
  );

/**
  Perform Root Port Downstream devices PCIE ASPM and LTR override on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3PcieSetPm (
  EFI_PCH_S3_PARAMETER_PCIE_SET_PM     *Parameter
  );

/**
  Perform PCH IOBP programming on S3 resume

  @param[in] Parameter            Parameters passed in from DXE

  @retval EFI_STATUS
**/
EFI_STATUS
PchS3ProgramIobp (
  EFI_PCH_S3_PARAMETER_PROG_IOBP     *Parameter
  );

#endif
