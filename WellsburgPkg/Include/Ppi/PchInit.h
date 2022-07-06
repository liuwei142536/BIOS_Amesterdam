/** @file
  This file defines the PCH Init PPI

@copyright
  Copyright (c) 2006 - 2012 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains a 'Sample Driver' and is licensed as such
  under the terms of your license agreement with Intel or your
  vendor.  This file may be modified by the user, subject to
  the additional terms of the license agreement
**/
#ifndef _PCH_INIT_H_
#define _PCH_INIT_H_

extern EFI_GUID               gPchInitPpiGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_INIT_PPI  PCH_INIT_PPI;

//
// Data structure definitions
//
typedef enum _CPU_STRAP_OPERATION
{
  GetCpuStrapSetData,
  SetCpuStrapSetData,
  LockCpuStrapSetData
} CPU_STRAP_OPERATION;

/**
  The function performing USB init in PEI phase. This could be used by USB recovery
  or debug features that need USB initialization during PEI phase.
  Note: Before executing this function, please be sure that PCH_INIT_PPI.Initialize
  has been done and PchUsbPolicyPpi has been installed.

  @param[in] PeiServices    General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval Others            All other error conditions encountered result in an ASSERT.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_USB_INIT) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  until negotiation completion

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             The function completed successfully
  @retval Others                  All other error conditions encountered result in an ASSERT.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_DMI_TCVC_PROGPOLL) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

  @param[in] PeiServices       General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval All other error conditions encountered result in an ASSERT.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_DMI_TCVC_PROG) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function performing TC/VC mapping program, and poll all PCH Virtual Channel 
  until negotiation completion

  @param[in] PeiServices       General purpose services available to every PEIM

  @retval EFI_SUCCESS       The function completed successfully
  @retval All other error conditions encountered result in an ASSERT.
**/

typedef
EFI_STATUS
(EFIAPI *PCH_DMI_TCVC_ENABLEPOLL) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function set the Target Link Speed in PCH to DMI GEN 2.

  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval None
**/
typedef
VOID
(EFIAPI *PCH_DMI_GEN2_PROG) (
  IN  EFI_PEI_SERVICES            **PeiServices
  );

/**
  The function is used while doing CPU Only Reset, where PCH may be required
  to initialize strap data before soft reset.

  @param[in] PeiServices          General purpose services available to every PEIM
  @param[in] Operation            Get/Set Cpu Strap Set Data
  @param[in] CpuStrapSet          Cpu Strap Set Data

  @retval EFI_SUCCESS             The function completed successfully.
  @exception EFI_UNSUPPORTED      The function is not supported.
**/
typedef
EFI_STATUS
(EFIAPI *PCH_CPU_STRAP_SET) (
  IN      EFI_PEI_SERVICES            **PeiServices,
  IN      CPU_STRAP_OPERATION         Operation,
  IN OUT  UINT16                      *CpuStrapSet
  );

///
/// PCH_INIT_PPI Structure Definition
///
struct _PCH_INIT_PPI {
  ///
  /// The function performs USB init in PEI phase. This could be used by USB recovery
  /// or debug function that USB initialization needs to be done in PEI phase.
  /// Note: Before executing this function, please be sure that PCH_PLATFORM_POLICY_PPI
  /// and PCH_USB_POLICY_PPI have been installed.
  ///
  PCH_USB_INIT          UsbInit;
  ///
  /// The function performing TC/VC mapping program, and poll all PCH Virtual Channel
  /// until negotiation completion.
  ///
  PCH_DMI_TCVC_PROGPOLL DmiTcVcProgPoll;
  ///
  ///  The function changes the PCH target link speed to DMI Gen 2
  ///
  PCH_DMI_TCVC_PROG     DmiTcVcProg;
  PCH_DMI_TCVC_ENABLEPOLL DmiTcVcEnablePoll;
  PCH_DMI_GEN2_PROG     DmiGen2Prog;
  ///
  /// The function provides a way to initialize PCH strap data before soft reset
  /// while doing CPU Only Reset
  ///
  PCH_CPU_STRAP_SET     CpuStrapSet;
};

#endif
