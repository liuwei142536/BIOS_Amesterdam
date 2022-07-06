/** @file
  Header file for the PCH Init PEIM

@copyright
  Copyright (c) 2004 - 2013 Intel Corporation. All rights reserved
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
#ifndef _PCH_INIT_PEIM_H_
#define _PCH_INIT_PEIM_H_

#include <Token.h> // AptioV server override

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include "PchAccess.h"
#include "Library/PchPlatformLib.h"
#include "Ppi/PchPlatformPolicy.h"
#include "Ppi/PchInit.h"
#include "Ppi/PchReset.h"
#include "Ppi/PchDmiTcVcMap.h"
#include "Ppi/PchPeiInitDone.h"
#include "Ppi/ReadOnlyVariable2.h"
#include "PchUsbCommon.h"
#include "IobpDefinitions.h"
#include "PchRegs/PchRegsLpc.h"
#include "PchHsio.h"
#include "PchInitVar.h"
#include "IioRegs.h"

#define MSR_CPU_BUS_NUMBER				0x300
#define UBOX_SCRATCH_DEVICE             0x10
#define UBOX_SCRATCH_FUNC7              0x7
#define UBOX_BIOS_SCRATCHPAD1           0x44
#define UBOX_BIOS_SCRATCHPAD0           0x40
#define PCH_ACPI_BASE_ADDRESS           0x0400
/**
  Internal function performing SATA init needed in PEI phase

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             No platform reset action is taken. System can continue boot flow.
  @retval Others                  Won't return if platform reset action is taken
**/
EFI_STATUS
EFIAPI
PchSataInit (
  IN  CONST EFI_PEI_SERVICES            **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  );

#ifdef PCH_SERVER_BIOS_FLAG
#define HSIO_REVISION              35
/**

   @brief 
   Internal function perdorming EVA's xSATA init needed in PEI phase. 
   
   @param [in] PeiServices		General purpose services available to every PEIM. 
   @param [in] PchPlatformPolicyPpi  Platform policy PPI pointer. 
   
   @retval None
  
**/
VOID
PchsSataInit (
  IN  CONST EFI_PEI_SERVICES            **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi  
  );

/**

  @brief 
  Internal function performing miscellaneous init needed in early PEI phase

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.

**/
EFI_STATUS
PchEvaMiscInit (
  IN  CONST EFI_PEI_SERVICES                    **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  );
#endif //PCH_SERVER_BIOS_FLAG

/**
  This function may trigger platform reset depending on the current GbE status,
  the intended GbE enabling, and current ME status. (When ME is enabled, this function
  may trigger a Global reset.)
  This function may not return if it triggers an platform reset and the BIOS boot flow
  restarts.
  If this function returns EFI_SUCCESS it indicates there is no need for platform
  reset in this boot, and boot flow continues.
  If this function returns EFI_DEVICE_ERROR, something error happens.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             No platform reset action is taken. System can continue boot flow.
  @retval Others                  Won't return if platform reset action is taken
**/
EFI_STATUS
EFIAPI
PchGbeMandatedReset (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  );

/**
  Perform Thermal Management Support initialization

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
RETURN_STATUS
PchThermalInit (
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  );

/**
  Initialize IOAPIC according to IoApicConfig policy of the PCH
  Platform Policy PPI

  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
PchIoApicInit (
  IN  PCH_PLATFORM_POLICY_PPI     *PchPlatformPolicyPpi
  );

/**
  This function performs basic initialization for PCH in PEI phase.
  If any of the base address arguments is zero, this function will disable the corresponding
  decoding, otherwise this function will enable the decoding.
  This function locks down the PMBase.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             Succeeds.
  @retval EFI_DEVICE_ERROR        Device error, aborts abnormally.
**/
EFI_STATUS
EFIAPI
PchInitialize (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
  );

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
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completed successfully
  @retval Others                  All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
PchDmiTcVcProgPoll (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

//
//  ESS: OverrideBegin
//
/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

  @param[in] PeiServices             General purpose services available to every PEIM

  @retval    EFI_SUCCESS             The function completed successfully
             EFI_ERROR               All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
PchDmiTcVcProg (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

  @param[in] PeiServices             General purpose services available to every PEIM

  @retval    EFI_SUCCESS             The function completed successfully
             EFI_ERROR               All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
PchDmiTcVcEnablePoll (
  IN  EFI_PEI_SERVICES            **PeiServices
  );
//
//  ESS: OverrideEnd 
//

/**
  The function set the Target Link Speed in PCH to DMI GEN 2.

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval None
**/
VOID
EFIAPI
PchDmiGen2Prog (
  IN CONST EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function program DMI miscellaneous registers.

  @param[in] PeiServices          General purpose services available to every PEIM
  @param[in] PchPlatformPolicyPpi The PCH Platform Policy PPI instance

  @retval EFI_SUCCESS             The DMI required settings programmed correctly
**/
EFI_STATUS
EFIAPI
PchDmiMiscProg (
  IN  CONST EFI_PEI_SERVICES                   **PeiServices,
  IN  PCH_PLATFORM_POLICY_PPI           *PchPlatformPolicyPpi
  );

/**
  The function is used while doing CPU Only Reset, where PCH may be required
  to initialize strap data before soft reset.

  @param[in] PeiServices          General purpose services available to every PEIM
  @param[in] Operation            Get/Set Cpu Strap Set Data
  @param[in, out] CpuStrapSet     Cpu Strap Set Data

  @retval EFI_SUCCESS             The function completed successfully.
  @exception EFI_UNSUPPORTED      The function is not supported.
**/
EFI_STATUS
EFIAPI
PchCpuStrapSet (
  IN CONST EFI_PEI_SERVICES            **PeiServices,
  IN      CPU_STRAP_OPERATION         Operation,
  IN OUT  UINT16                      *CpuStrapSet
  );

/**
  The function performing USB init in PEI phase. This could be
  used by USB recovery ,debug features or usb precondition
  enabled case that need USB initialization during PEI phase.
  Please be sure the function should not be executed in if the
  boot mode is S3 resume.

  @param[in] PeiServices    General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval Others            All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
PchStartUsbInit (
  IN PCH_USB_CONFIG               *UsbConfig,
  IN UINT32                       EhciMemBaseAddr,
  IN UINT32                       XhciMemBaseAddr,
  IN UINT8                        Revision
  );

#endif
