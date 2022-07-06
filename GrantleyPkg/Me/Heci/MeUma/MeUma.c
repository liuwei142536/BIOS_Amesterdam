/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2010 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  MeUma.c

@brief:

  Framework PEIM to MeUma

**/

///
/// Statements that include other files
///
#include "Guid/SetupVariable.h"
#include "Ppi/ReadOnlyVariable2.h"
#include "MeAccess.h"
#include "HeciRegs.h"
#include "IndustryStandard/Pci22.h"
#include "MeUma.h"
#include "Ppi/Stall.h"
#include "Library/DebugLib.h"
#include "Library/PciLib.h"
#include "Library/PerformanceLib.h"
#include <Protocol/MePlatformPolicyPei.h>
#include "Ppi/Wdt.h"
#include "Library/IoLib.h"
#include "Ppi/PchReset.h"
#include "Library/MeTypeLib.h"
#include <Library/PlatformHooksLib.h>
#include <MeState.h>


/*****************************************************************************
 * Local definitions.
 *****************************************************************************/
#define DEBUG_ME_UMA "ME UMA: "


/*****************************************************************************
 * Local funcion prototypes.
 *****************************************************************************/
UINT32 MeSendUmaSize(IN EFI_PEI_SERVICES **PeiServices, IN EFI_FFS_FILE_HEADER *FfsHeader);
EFI_STATUS CpuReplacementCheck(IN EFI_PEI_SERVICES    **PeiServices,
                               IN EFI_FFS_FILE_HEADER *FfsHeader,
                               UINT8                  *ForceFullTraining,
                               UINT8                  *nextStep);
EFI_STATUS MeConfigDidReg(IN EFI_PEI_SERVICES     **PeiServices,
                          IN EFI_FFS_FILE_HEADER  *FfsHeader,
                          UINT8                   InitStat,
                          UINT8                   *nextStep);
EFI_STATUS HostResetWarning(IN EFI_PEI_SERVICES **PeiServices, IN EFI_FFS_FILE_HEADER *FfsHeader);
EFI_STATUS HandleMeBiosAction(IN EFI_PEI_SERVICES **PeiServices, UINT8 BiosAction);
EFI_STATUS UmaPerformReset(IN EFI_PEI_SERVICES **PeiServices, UINT8 ResetType);
EFI_STATUS ClearDisb(VOID);
EFI_STATUS SetDISB(VOID);
BOOLEAN isMeUmaEnabled(IN CONST EFI_PEI_SERVICES **PeiServices);


/*****************************************************************************
 * Local variables.
 *****************************************************************************/
static ME_UMA_PPI         mMeUmaPpi = {
  MeSendUmaSize,
  CpuReplacementCheck,
  MeConfigDidReg,
  HandleMeBiosAction,
  isMeUmaEnabled,
  HostResetWarning
};

static EFI_PEI_PPI_DESCRIPTOR mMeUmaPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gMeUmaPpiGuid,
    &mMeUmaPpi
  }
};


/*****************************************************************************
 * Public functions.
 *****************************************************************************/
BOOLEAN
EFIAPI
isMeUmaEnabled (
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
/**

  @brief
  This function reads configurations for UMA.

  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.

**/
{
  EFI_STATUS                      Status;
  SYSTEM_CONFIGURATION            SystemConfiguration;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
  BOOLEAN                          UmaEnabled = TRUE;

  //
  // Configure UMA support
  // Disable UMA when END_OF_POST message is disabled
  //
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gEfiPeiReadOnlyVariable2PpiGuid,
                            0,
                            NULL,
                            &PeiVariable
                            );

  ASSERT_EFI_ERROR (Status);
  if (Status == EFI_SUCCESS) {
    UINTN VariableSize = sizeof (SYSTEM_CONFIGURATION);
    // AptioV Server Override: Changed variable name to IntelSetup
    Status = PeiVariable->GetVariable (
                        PeiVariable,
                        L"IntelSetup",
                        &gEfiSetupVariableGuid,
                        NULL,
                        &VariableSize,
                        &SystemConfiguration
                        );
    if (Status == EFI_SUCCESS) {
      DEBUG ((EFI_D_INFO, DEBUG_ME_UMA" UMA Enabled flag set from configuration.\n"));
      UmaEnabled = SystemConfiguration.MeDidEnabled == 0 ? FALSE : TRUE;
    } else {
      DEBUG ((EFI_D_INFO, DEBUG_ME_UMA" Cannot find Setup variable. Use module default.\n"));
    }
  } else {
    DEBUG ((EFI_D_INFO, DEBUG_ME_UMA" Cannot get system configuration. Use module default.\n"));
  }
  DEBUG ((EFI_D_INFO, DEBUG_ME_UMA" UMA Enabled flag = %d\n", UmaEnabled));

  return UmaEnabled;
}

UINT32
MeSendUmaSize (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN EFI_FFS_FILE_HEADER  *FfsHeader
  )
/**

  @brief
  This procedure will read and return the amount of ME UMA requested
  by ME ROM from the HECI device.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] FfsHeader            Pointer to the FFS file header

  @retval UINT32                  Return ME UMA Size
  @retval 0                       Do not check for ME UMA

**/
{
  UINT32                  Timeout;
  HECI_MISC_SHDW_REGISTER MeMiscShdw;
  HECI_FWS_REGISTER       MeHfs;
  EFI_STATUS              Status;
  EFI_PEI_STALL_PPI       *StallPpi;

  if (isMeUmaEnabled(PeiServices) == 0) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME UMA feature disabled (MeSendUmaSize()).\n"));
    return 0;
  }

  MeMiscShdw.ul = HeciPciRead32 (R_ME_MISC_SHDW);
  MeHfs.ul      = HeciPciRead32 (R_ME_HFS);
  Timeout       = 0x0;

  if (MeHfs.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME debug mode, do not check for ME UMA. \n"));
    return 0;
  }

  if (MeHfs.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME error, do not check for ME UMA. \n"));
    return 0;
  }


  ///
  /// Poll on MUSZV until it indicates a valid size is present or 5s timeout expires.
  ///
  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiStallPpiGuid, 0, NULL, &StallPpi);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" Cannot find gEfiPeiStallPpiGuid. \n"));
    return 0;
  }

  PERF_START (FfsHeader, "MUSZV", NULL, AsmReadTsc ());
  while ((MeMiscShdw.r.MUSZV == 0) && (Timeout < MUSZV_TIMEOUT_MULTIPLIER)) {
    StallPpi->Stall (PeiServices, StallPpi, STALL_1_MILLISECOND);
    MeMiscShdw.ul = HeciPciRead32 (R_ME_MISC_SHDW);
    Timeout++;
  }

  PERF_END (FfsHeader, "MUSZV", NULL, AsmReadTsc ());

  if (Timeout >= MUSZV_TIMEOUT_MULTIPLIER) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" Timeout occurred waiting for MUSZV. \n"));
    if (IsSoftSdvPlatform() == TRUE) {
      DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" For SOFTSDV set 32MB for ME UMA size. \n"));
      return 32;
    }
    else {
      return 0;
    }
  }

  ///
  /// Return MeUmaSize value
  ///
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME UMA Size Requested: %x\n", MeMiscShdw.r.MUSZ));

  return MeMiscShdw.r.MUSZ;
}

VOID
DumpMePlatformPolicyPei (
  IN PEI_ME_PLATFORM_POLICY_PPI  *PeiMePolicyPpi
  )
/**

@brief

  Internal function performing Heci platform ME Debug Dump in PEI phase

  @param[in] PeiMePolicyPpi - Policy Ppi

**/
{
  DEBUG ((DEBUG_INFO, "\n"DEBUG_ME_UMA" ------------- MePlatformPolicyPpi Dump Begin -------------\n"));
  DEBUG ((DEBUG_INFO, " Revision    : 0x%x\n", PeiMePolicyPpi->Revision));
  DEBUG ((DEBUG_INFO, " DidEnabled  : 0x%x\n", PeiMePolicyPpi->DidEnabled));
  DEBUG ((DEBUG_INFO, " DidTimeout  : 0x%x\n", PeiMePolicyPpi->DidTimeout));
  DEBUG ((DEBUG_INFO, " DidInitStat : 0x%x\n", PeiMePolicyPpi->DidInitStat));
  DEBUG ((DEBUG_INFO,     DEBUG_ME_UMA" ------------- MePlatformPolicyPpi Dump End ----------------\n"));
}

EFI_STATUS
CpuReplacementCheck (
  IN EFI_PEI_SERVICES    **PeiServices,
  IN EFI_FFS_FILE_HEADER *FfsHeader,
  UINT8                  *ForceFullTraining,
  UINT8                  *nextStep
  )
/**

  @brief
  This procedure will determine whether or not the CPU was replaced
  during system power loss or via dynamic fusing.
  Calling this procedure could result in a warm reset (if ME FW is requesting one).

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] FfsHeader            Not used.
  @param[in] ForceFullTraining    When set = 0x1, MRC will be forced to perform
                                  a full memory training cycle.
  @param[in] nextStep             returns information about required next action

  @retval EFI_SUCCESS             The function completed successfully.

**/
{
  HECI_GS_SHDW_REGISTER       MeFwsts2;
  EFI_STATUS                  Status;
  UINT32                      Timeout;
  EFI_PEI_STALL_PPI           *StallPpi;
  HECI_FWS_REGISTER           MeHfs;
  PEI_ME_PLATFORM_POLICY_PPI  *PeiMePolicyPpi;

  *nextStep = CBM_DIR_DO_NOTHING;

  if (isMeUmaEnabled(PeiServices) == 0) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME UMA feature disabled (CpuReplacementCheck()).\n"));
    return EFI_SUCCESS;
  }

  MeHfs.ul = HeciPciRead32 (R_ME_HFS);
  ///
  ///
  if (MeHfs.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME debug mode, do not check CPU Replaced Status. \n"));
    return EFI_SUCCESS;
  }


  ///
  /// Get platform policy settings through the MePlatformPolicy PPI
  ///
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPeiMePlatformPolicyPpiGuid,
                            0,
                            NULL,
                            &PeiMePolicyPpi
                            );
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS)
    return Status;

  Timeout = 0x0;

  ///
  /// Read ME FWS2
  ///
  MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" MeFwsts2 = %x.\n", MeFwsts2.r));

  ///
  /// SoftSDV emulation returns 0xffffffff.
  /// This gives false CPU swap indication and circular reset execution
  /// This check can be removed when hardware device will be used
  ///
  if (MeFwsts2.ul == 0xffffffff) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" MeFwsts2 0xffffffff detected, do not check CPU Replaced Status.\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Locate Stall Ppi
  ///
  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiStallPpiGuid, 0, NULL, &StallPpi);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS)
    return Status;

  PERF_START (FfsHeader, "CPURLOP", NULL, AsmReadTsc ());
  ///
  /// Poll 50 ms on CPU Replaced Valid bit
  ///
  while ((((MeFwsts2.r.CpuReplacedValid == 0) && Timeout < CPURV_TIMEOUT_MULTIPLIER))
          && (PeiMePolicyPpi->DidTimeout == 1)
  ) {
    StallPpi->Stall (PeiServices, StallPpi, STALL_1_MILLISECOND);
    MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
    Timeout++;
  }
  PERF_END (FfsHeader, "CPURLOP", NULL, AsmReadTsc ());

  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" CpuReplacedValid = %x, ", MeFwsts2.r.CpuReplacedValid));
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" CpuReplacedStatus = %x, ", MeFwsts2.r.CpuReplacedSts));
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" WarmRstReqForDF = %x.\n", MeFwsts2.r.WarmRstReqForDF));

  if (Timeout >= CPURV_TIMEOUT_MULTIPLIER || MeFwsts2.r.CpuReplacedValid == 0x0) {
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" Timeout occurred, the CPU Replacement Valid Bit is not set.\n"));
    *ForceFullTraining = 0x1;
  } else {
    if (MeFwsts2.r.CpuReplacedValid == 0x1) {
      if (MeFwsts2.r.WarmRstReqForDF == 0x1) {
        *nextStep = CBM_DIR_NON_PCR_CLEARDISB;
      }

      if ((MeFwsts2.r.CpuReplacedSts == 0x1 && MeFwsts2.r.WarmRstReqForDF == 0x0)) {
        *ForceFullTraining = 0x1;
      }
    }
  }

  return Status;
}

EFI_STATUS
MeConfigDidReg (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN EFI_FFS_FILE_HEADER  *FfsHeader,
  UINT8                   InitStat,
  UINT8                   *nextStep
  )
/**

  @brief
  This procedure will configure the ME Host General Status register,
  indicating that DRAM Initialization is complete and ME FW may
  begin using the allocated ME UMA space.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] InitStat             H_GS[27:24] Status
  @param[in] nextStep             returns information about required next action

  @retval EFI_SUCCESS

**/
{
  UINT32                      MeUmaBase;
  UINT32                      MeUmaBaseExt;
  UINT32                      MeHgs;
  UINT32                      Timeout;
  HECI_FWS_REGISTER           MeHfs;
  EFI_STATUS                  Status;
  EFI_PEI_STALL_PPI           *StallPpi;
  PEI_ME_PLATFORM_POLICY_PPI  *PeiMePolicyPpi;

  *nextStep = CBM_DIR_DO_NOTHING;

  if (isMeUmaEnabled(PeiServices) == 0) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME UMA feature disabled (MeConfigDidReg()).\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Get platform policy settings through the MePlatformPolicy PPI
  ///
  Status = (**PeiServices).LocatePpi (
                            PeiServices,
                            &gPeiMePlatformPolicyPpiGuid,
                            0,
                            NULL,
                            &PeiMePolicyPpi
                            );
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS)
    return Status;

  DumpMePlatformPolicyPei (PeiMePolicyPpi);

  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiStallPpiGuid, 0, NULL, &StallPpi);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS)
    return Status;

  MeHgs     = 0x0;
  Timeout   = 0x0;
  MeHfs.ul  = HeciPciRead32 (R_ME_HFS);


  if (MeHfs.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME debug mode, do not check for ME UMA. \n"));
    return EFI_SUCCESS;
  }

  if (MeHfs.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME error, do not check for ME UMA. \n"));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" Entered ME DRAM Init Done procedure.\n"));

  ///
  /// Read MESEGBASE value
  ///
  MeUmaBase     = PciRead32 (PCI_LIB_ADDRESS (0, 5, 0, R_MESEG_BASE));
  MeUmaBaseExt  = PciRead32 (PCI_LIB_ADDRESS (0, 5, 0, R_MESEG_BASE + 0x04));
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" MeUmaBase read: %x\n", MeUmaBase));

  ///
  /// Write DRAM Init Done (DID) data to the ME H_GS[23:0].
  /// H_GS[23:16] = extended UMA base address (reserved)
  /// H_GS[15:0] = 1M aligned UMA base address
  /// ME FW will 0 extend these values to determine MeUmaBase
  ///
  MeUmaBase = ((MeUmaBaseExt << 28) + (MeUmaBase >> 4)) >> 16;
  MeHgs     = MeUmaBase;

  ///
  /// Set H_GS[31:28] = 0x1 indicating DRAM Init Done
  ///
  MeHgs |= B_ME_DID_TYPE_MASK;

  ///
  /// RapidStart
  ///
  if (InitStat & 0x80) {
    MeHgs |= B_ME_DID_RAPID_START_BIT;
  }

  InitStat &= 0x7F;

  ///
  /// Set H_GS[27:24] = Status
  ///   0x0 = Success
  ///   0x1 = No Memory in channels
  ///   0x2 = Memory Init Error
  ///   0x3 = Memory not preserved across reset
  ///   0x4-0xF = Reserved
  ///
  ///
  /// If DidInitStat is not set to "Auto" (0), provide the DidInitStat
  /// value specified in the setup option to ME FW.
  ///
  if (PeiMePolicyPpi->DidInitStat > 0) {
    InitStat = PeiMePolicyPpi->DidInitStat - 1;
  }
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" InitStat: %x\n", InitStat));
  MeHgs |= (InitStat << 24);

  PERF_START (FfsHeader, "DID", NULL, AsmReadTsc ());
  HeciPciAndThenOr32 (R_ME_H_GS, 0, MeHgs);
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME H_GS written: %x\n", MeHgs));

  ///
  /// ME FW typically responds with the DID ACK w/in 1ms
  /// Adding short delay to avoid wasting time in the timeout loop
  ///
  StallPpi->Stall (PeiServices, StallPpi, STALL_1_MILLISECOND + STALL_100_MICROSECONDS);

  ///
  /// Read the ME H_FS Register to look for DID ACK.
  ///
  MeHfs.ul = HeciPciRead32 (R_ME_HFS);
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" HFS read before DID ACK: 0x%08X\n", MeHfs.ul));
  if (
      PeiMePolicyPpi->DidTimeout &&
      MeSendUmaSize(PeiServices, FfsHeader) != 0)
  {
    ///
    /// ~5 second Timeout for DID ACK
    ///
    Timeout = DID_TIMEOUT_MULTIPLIER;
    while (MeHfs.r.BiosMessageAck == 0)
    {
      if (!Timeout--)
      {
         DEBUG((EFI_D_ERROR, " "DEBUG_ME_UMA" ERROR: Timeout waiting for DRAM Init Done ACK"
                             "(MEFS1: 0x%08X)\n", MeHfs.ul));
         return EFI_TIMEOUT;
      }
      StallPpi->Stall(PeiServices, StallPpi, STALL_1_MILLISECOND);
      MeHfs.ul = HeciPciRead32(R_ME_HFS);
    }
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME DRAM Init Done ACK received.\n"));
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" HFS read after DID ACK: 0x%08X\n", MeHfs.ul));
    *nextStep = (UINT8)MeHfs.r.AckData;
  }
  PERF_END (FfsHeader, "DID", NULL, AsmReadTsc ());

  DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" BiosAction = %x\n", *nextStep));

  return EFI_SUCCESS;
}


EFI_STATUS
HostResetWarning(
  IN EFI_PEI_SERVICES     **PeiServices,
  IN EFI_FFS_FILE_HEADER  *FfsHeader
  )
/**

  @brief
  This procedure will configure the ME Host General Status register,
  indicating that host system is about to perform a warm reset, but after
  the reset it is going to continue current flow, so ME should ignore reset
  and sustain previous power flow.
  
  NOTE: This function should be called on S3/S4 resume flow only.
        On other flow resets can be done without notification.
  
  @param[in] PeiServices  General purpose services available to every PEIM
  @param[in] FfsHeader    Pointer to the FFS file header

  @retval EFI_SUCCESS

**/
{
  UINT32             Timeout;
  HECI_FWS_REGISTER  MeFs1;
  EFI_BOOT_MODE      BootMode;
  EFI_PEI_STALL_PPI  *pStallPpi;

  if ((!EFI_ERROR((*PeiServices)->GetBootMode(PeiServices, &BootMode)) &&
       BootMode != BOOT_ON_S3_RESUME && BootMode != BOOT_ON_S4_RESUME)
#if defined(AMT_SUPPORT) && AMT_SUPPORT
      // For AMT firmware HostResetWarning has to be sent for all Sx paths
      && (MeTypeIsAmt() == FALSE)
#endif defined(AMT_SUPPORT) && AMT_SUPPORT
     )
  {
    DEBUG((DEBUG_WARN, " "DEBUG_ME_UMA" WARNING: HostResetWarning called on non S3/4 resume flow (%d)"
           " - ignored\n", BootMode));
    return EFI_SUCCESS;
  }
  ///
  /// Write Reset Warning data to the HECI-1 H_GS register at offset 4Ch.
  ///
  /// D22:F0:R4Ch Register Based Host Reset Warning Message Format
  /// Bits              Action
  /// H_GS 31:28  0x8 = Preserve previous host state across warm reset
  /// H_GS 27:24  (0x00 = Success, 0x01 -0x0F = Reserved)
  /// H_GS 23:16  Reserved
  /// H_GS 15:0    Must be 0x0
  HeciPciWrite32(R_ME_H_GS, B_HOST_RESET_WARN_TYPE_MASK);
  DEBUG((DEBUG_INFO, " "DEBUG_ME_UMA" Host Reset Warning written\n"));
  ///
  /// The Reset Warning notification requires acknowledge, will need stall service.
  ///
  if (EFI_ERROR((*PeiServices)->LocatePpi(PeiServices, &gEfiPeiStallPpiGuid, 0, NULL, &pStallPpi)))
  {
    DEBUG((DEBUG_ERROR, " "DEBUG_ME_UMA" ERROR: Cannot find stall service\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Read the ME Firmware Status #1 in HECI-1 H_FS register to look for Host Reset Warning ACK.
  /// But wait only if ME is in a valid state.
  /// Ignore errors for ME9x FW and sent notification when ME FW reports errors also.
  ///
  MeFs1.ul = HeciPciRead32(R_ME_HFS);
#if TrabucoCanyon_SUPPORT
  MeFs1.ul &= 0x00FFFFFF; // Aptio V override - force offboard video to display
  HeciPciWrite32(R_ME_HFS, MeFs1.ul);
#endif
  DEBUG((DEBUG_INFO, " "DEBUG_ME_UMA" MEFS1: 0x%08X\n", MeFs1.ul));
  if ( MeFs1.r.CurrentState == ME_STATE_RECOVERY ||
       MeFs1.r.CurrentState > ME_STATE_TRANSITION ||
     ((MeFs1.r.ErrorCode != 0) && (MeTypeIsAmt() == FALSE)))
  {
    DEBUG((DEBUG_INFO, " "DEBUG_ME_UMA" No ACK expected for current ME state (0x%X)\n",
           MeFs1.r.CurrentState));
    return EFI_TIMEOUT;
  }
  if (MeFs1.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME debug mode, do not send HostResetWarning. \n"));
    return EFI_TIMEOUT;
  }

  Timeout = DID_TIMEOUT_MULTIPLIER; // ~5 seconds timeout for Host Reset Warning ACK
  while (MeFs1.r.BiosMessageAck != 8)
  {
    if (!Timeout--)
    {
      DEBUG((EFI_D_ERROR, " "DEBUG_ME_UMA" ERROR: Timeout waiting for Host Reset Warning ACK"
                          "(MEFS1: 0x%08X)\n", MeFs1.ul));
      return EFI_TIMEOUT;
    }
    pStallPpi->Stall(PeiServices, pStallPpi, STALL_1_MILLISECOND);
    MeFs1.ul = HeciPciRead32(R_ME_HFS);
  }
  DEBUG((DEBUG_INFO, " "DEBUG_ME_UMA" Host Reset Warning ACK received (MEFS1: 0x%08X)\n",
         MeFs1.ul));
  return EFI_SUCCESS;
} // HostResetWarning()


EFI_STATUS
HandleMeBiosAction (
  IN EFI_PEI_SERVICES **PeiServices,
  UINT8               BiosAction
  )
/**

  @brief
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] BiosAction           Me requests BIOS to act

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
{
  EFI_STATUS            Status;
  HECI_GS_SHDW_REGISTER MeFwsts2;
  HECI_FWS_REGISTER     MeFs1;

  if (isMeUmaEnabled(PeiServices) == 0) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME UMA feature disabled (HandleMeBiosAction()).\n"));
    return EFI_SUCCESS;
  }

  MeFs1.ul = HeciPciRead32(R_ME_HFS);
  if (MeFs1.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) {
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME debug mode, do not HandleMeBiosAction. \n"));
    return EFI_SUCCESS;
  }

  ///
  /// Read ME FWS2
  ///
  MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
  DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" MeFwsts2 = %x.\n", MeFwsts2.r));

  switch (BiosAction) {
  case CBM_DIR_DO_NOTHING:
    ///
    /// DID ACK was not received
    ///
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" DID Ack was not received, no BIOS Action to process.\n"));
    break;
  case CBM_DIR_NON_PCR_CLEARDISB:
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW DID ACK has requested a Clear DISB.\n"));
    ///
    /// Clear DISB and Issue a Non-Power Cycle Reset
    ///
    Status  = ClearDisb ();
    ASSERT_EFI_ERROR (Status);
    ///
    /// This type of reset is handled directly by MRC
    ///
    return Status;
  case CBM_DIR_NON_PCR:
    ///
    /// Perform Non-Power Cycle Reset
    ///
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW has requested a Non-PCR.\n"));
    Status = UmaPerformReset (PeiServices, CBM_DIR_NON_PCR);
    break;

  case CBM_DIR_PCR:
    ///
    /// Perform Power Cycle Reset
    ///
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW has requested a PCR.\n"));
    Status = UmaPerformReset (PeiServices, CBM_DIR_PCR);
    break;

  case 3:
    ///
    /// Go To S3
    ///
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME FW DID ACK has requested entry to S3.  Not defined, continuing to POST.\n"));
    break;

  case 4:
    ///
    /// Go To S4
    ///
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME FW DID ACK has requested entry to S4.  Not defined, continuing to POST.\n"));
    break;

  case 5:
    ///
    /// Go To S5
    ///
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME FW DID ACK has requested entry to S5.  Not defined, continuing to POST.\n"));
    break;

  case CBM_DIR_GLOBAL_RESET:
    ///
    /// Perform Global Reset
    ///
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW has requested a Global Reset.\n"));
    Status = UmaPerformReset (PeiServices, CBM_DIR_GLOBAL_RESET);
    break;

  case CBM_DIR_CONTINUE_POST:
    ///
    /// Continue to POST
    ///
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME FW DID Ack requested to continue to POST.\n"));
    break;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
UmaPerformReset (
  IN EFI_PEI_SERVICES **PeiServices,
  UINT8               ResetType
  )
/**

@brief
  This procedure will issue a Non-Power Cycle, Power Cycle, or Global Rest.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] ResetType            Type of reset to be issued.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
{
  PCH_RESET_PROTOCOL  *mPchReset;
  PCH_RESET_TYPE      PchResetType;
  EFI_STATUS          Status;

  Status = (*PeiServices)->LocatePpi(PeiServices, &gPchResetPpiGuid, 0, NULL, &mPchReset);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS)
    return Status;

  switch (ResetType) {
  case CBM_DIR_NON_PCR_CLEARDISB:
  case CBM_DIR_NON_PCR:
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW DID ACK has requested a Non Power Cycle Reset.\n"));
    PchResetType = ColdReset;
    break;

  case CBM_DIR_PCR:
    ///
    /// Power Cycle Reset requested
    ///
    DEBUG ((DEBUG_INFO, DEBUG_ME_UMA" ME FW DID ACK has requested a Power Cycle Reset.\n"));
    PchResetType = PowerCycleReset;
    break;

  case CBM_DIR_GLOBAL_RESET:
    ///
    /// Global Reset
    ///
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW DID Ack requested a global reset.\n"));
    PchResetType = GlobalReset;
    break;
  default:
    DEBUG ((EFI_D_ERROR, DEBUG_ME_UMA" ME FW No reset.\n"));

    return EFI_SUCCESS;
  }

  mPchReset->Reset (mPchReset, PchResetType);

  //
  // We should never get here
  //
  ASSERT (FALSE);

  return EFI_SUCCESS;
}

EFI_STATUS
ClearDisb (
  VOID
  )
/**

  @brief
  This procedure will clear the DISB.

  @param[in] None

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
{
  UINT16  Data16;

  Data16  = PciRead16 (PCI_LIB_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_2));
  Data16  = Data16 &~B_PCH_LPC_GEN_PMCON_DRAM_INIT;
  PciWrite16 (PCI_LIB_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_2), Data16);

  return EFI_SUCCESS;
}

EFI_STATUS
SetDISB (
  VOID
  )
/**

  @brief
  This procedure will clear the DISB.

  @param[in] None

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
{
  UINT16  Data16;

  Data16  = PciRead16 (PCI_LIB_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_2));
  Data16  = Data16 & B_PCH_LPC_GEN_PMCON_DRAM_INIT;
  PciWrite16 (PCI_LIB_ADDRESS (0, PCI_DEVICE_NUMBER_PCH_LPC, 0, R_PCH_LPC_GEN_PMCON_2), Data16);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MeUmaEntry (
  IN EFI_FFS_FILE_HEADER      *FfsHeader,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  )
/**

  @brief
  This function is the entry point for this PEI.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] PeiServices          Pointer to the PEI services table

  @retval Return Status based on errors that occurred while waiting for time to expire.

**/
{
  EFI_STATUS  Status = EFI_SUCCESS;

  Status  = (*PeiServices)->InstallPpi (PeiServices, mMeUmaPpiList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_INFO, DEBUG_ME_UMA" ME UMA PPI Installed\n"));

  return Status;
}
