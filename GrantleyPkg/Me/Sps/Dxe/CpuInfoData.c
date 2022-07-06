/*++

Copyright (c) 2010-2013, Intel Corporation.

This source code and any documentation accompanying it ("Material") is furnished
under license and may only be used or copied in accordance with the terms of that
license.  No license, express or implied, by estoppel or otherwise, to any
intellectual property rights is granted to you by disclosure or delivery of these
Materials.  The Materials are subject to change without notice and should not be
construed as a commitment by Intel Corporation to market, license, sell or support
any product or technology.  Unless otherwise provided for in the license under which
this Material is provided, the Material is provided AS IS, with no warranties of
any kind, express or implied, including without limitation the implied warranties
of fitness, merchantability, or non-infringement.  Except as expressly permitted by
the license for the Material, neither Intel Corporation nor its suppliers assumes
any responsibility for any errors or inaccuracies that may appear herein.  Except
as expressly permitted by the license for the Material, no part of the Material
may be reproduced, stored in a retrieval system, transmitted in any form, or
distributed by any means without the express written consent of Intel Corporation.


Module Name:

  CpuInfoData.c

Abstract:

   This implements filling out the HECI Message responsible of passing 
   CPU Info data. 

--*/
#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>

#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>

#include <Guid/SetupVariable.h>

#include <Protocol/MpService.h>
#include <Library/CpuConfigLib.h>
#include <CpuPpmIncludes.h>

#include <Protocol/HeciProtocol.h>
#include <Sps.h>
#include <Guid/SpsInfoHobGuid.h>


/*****************************************************************************
 * Local definitions
 *****************************************************************************/
#define CPU_TSTATES_NUM_STD    8
#define CPU_TSTATES_NUM_FG    15  // fine grained T-states


/*****************************************************************************
 @brief
  Gather and send to ME host configuration data needed by Node Manager

  @param[in] pSpsHob   Info from SPS PEI driver
  @param[in] pSysCfg  BIOS setup configuration

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS 
GatherCPUInfoData(
  EFI_HOB_TYPE_SPS_INFO *pSpsHob, 
  SYSTEM_CONFIGURATION  *pSysCfg)
{
  EFI_STATUS Status;
  UINT32     HeciMsgLen;
  UINT32     Reg;
  UINT32     Index;
  UINT32     ProcMask;
  UINT32     tCoreMask[MAX_CPU_SOCKET];
  MKHI_MSG_NM_HOST_CFG_REQ HeciMsg;
  EFI_HECI_PROTOCOL        *pHeciProtocol;
  CPU_COLLECTED_DATA       *pCpuData;
  
  Status = gBS->LocateProtocol(&gEfiHeciProtocolGuid, NULL, &pHeciProtocol);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
    return Status;
  }
  SetMem(&HeciMsg, sizeof(HeciMsg), 0);
  
  HeciMsg.Mkhi.Data = (NM_CMD_HOSTCFG << 8) | MKHI_GRP_NM;
  
  HeciMsg.Capabilities.Bits.TurboEn = pSysCfg->TurboMode;
  HeciMsg.Capabilities.Bits.PowerMsmt = pSysCfg->NmPowerMsmtSupport;
  HeciMsg.Capabilities.Bits.HwChange = pSysCfg->NmHwChangeStatus;
  //
  // ME wants TURBO_RATIO_LIMIT and PLATFORM_INFO MSRs sent raw.
  //
  HeciMsg.TurboRatioLimit = AsmReadMsr64(MSR_TURBO_RATIO_LIMIT);
  HeciMsg.PlatformInfo = AsmReadMsr64(MSR_PLATFORM_INFO);
  HeciMsg.Altitude = (INT32)pSysCfg->SpsAltitude;
  
  ProcMask = 0;
  SetMem(&tCoreMask[0], sizeof(tCoreMask), 0);
  for (pCpuData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0];
       pCpuData < &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[mCpuConfigLibConfigContextBuffer->NumberOfProcessors];
       pCpuData++)
  {
    ASSERT(pCpuData->ProcessorLocation.Package < sizeof(tCoreMask)/sizeof(tCoreMask[0]));
    if (!(ProcMask & 1 << pCpuData->ProcessorLocation.Package))
    {
      ProcMask |= 1 << pCpuData->ProcessorLocation.Package;
      HeciMsg.ProcNumber++;
    }
    if (!(tCoreMask[pCpuData->ProcessorLocation.Package] & (1 << pCpuData->ProcessorLocation.Core)))
    {
      tCoreMask[pCpuData->ProcessorLocation.Package] |= 1 << pCpuData->ProcessorLocation.Core;
      HeciMsg.ProcCoresEnabled++;
      HeciMsg.ProcThreadsEnabled++;
    }
    else
    { //
      // For some reason hyper-threads are listed in this library regardless whether
      // HT is enabled or not. Let's filter them using configuration data.
      //
      if (!pSysCfg->ProcessorHyperThreadingDisable)
      {
        HeciMsg.ProcThreadsEnabled++;
      }
    }
    if (pCpuData->PackageBsp)
    {
      HeciMsg.PStatesNumber = (UINT8)pCpuData->CpuMiscData.NumberOfPStates;
      ASSERT(pCpuData->CpuMiscData.NumberOfPStates <= sizeof(HeciMsg.PStatesRatio)/sizeof(HeciMsg.PStatesRatio[0]));
      if (pCpuData->CpuMiscData.FvidTable != NULL)
      {
        for (Index = 0; Index < pCpuData->CpuMiscData.NumberOfPStates; Index++)
        {
          HeciMsg.PStatesRatio[Index] = (UINT8)pCpuData->CpuMiscData.FvidTable[Index].Ratio;
        }
      }
      if (pSysCfg->TStateEnable)
      {
        HeciMsg.TStatesNumber = CPU_TSTATES_NUM_STD;
        if (pCpuData->CpuidData.CpuIdLeaf != NULL &&
            pCpuData->CpuidData.NumberOfBasicCpuidLeafs > EFI_CPUID_POWER_MANAGEMENT_PARAMS &&
            (pCpuData->CpuidData.CpuIdLeaf[EFI_CPUID_POWER_MANAGEMENT_PARAMS].RegEax & EFI_FINE_GRAINED_CLOCK_MODULATION))
        {
          HeciMsg.TStatesNumber = CPU_TSTATES_NUM_FG;
        }
      }
    }
  } // for (pProcData...)
  //
  // Number of cores implemented can be found in CPU ID function 11 - core topology.
  // Core topology subfunction 1 gives the number of possible logical threads, subfunction 0
  // the number of threads per core. To calculate cores divide threads by threads per core.
  //
  AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 1, NULL, &Reg, NULL, NULL);
  HeciMsg.ProcCoresNumber = (UINT8)Reg;
  AsmCpuidEx(EFI_CPUID_CORE_TOPOLOGY, 0, NULL, &Reg, NULL, NULL);
  HeciMsg.ProcCoresNumber /= (UINT8)Reg;
  HeciMsg.ProcCoresNumber *= HeciMsg.ProcNumber;
  
  DEBUG((EFI_D_INFO, "[SPS] Sending host configuration to NM\n"));
  DEBUG((EFI_D_INFO, "[SPS]   Capabilities : 0x%04x\n", HeciMsg.Capabilities.Uint16));
  DEBUG((EFI_D_INFO, "[SPS]   P/T-states:    %d/%d\n",
                     HeciMsg.PStatesNumber, HeciMsg.TStatesNumber));
  DEBUG((EFI_D_INFO, "[SPS]   Min/max power: %d/%d\n",
                     HeciMsg.MinPower, HeciMsg.MaxPower));
  DEBUG((EFI_D_INFO, "[SPS]   Processor packages: %d\n", HeciMsg.ProcNumber));
  DEBUG((EFI_D_INFO, "[SPS]   Processor cores:    %d\n", HeciMsg.ProcCoresNumber));
  DEBUG((EFI_D_INFO, "[SPS]   Processor cores enabled:   %d\n", HeciMsg.ProcCoresEnabled));
  DEBUG((EFI_D_INFO, "[SPS]   processor threads enabled: %d\n", HeciMsg.ProcThreadsEnabled));
  DEBUG((EFI_D_INFO, "[SPS]   Turbo ratio limit: 0x%08X%08X\n",
                     (UINT32)(HeciMsg.TurboRatioLimit >> 32), (UINT32)HeciMsg.TurboRatioLimit));
  DEBUG((EFI_D_INFO, "[SPS]   Platform info:     0x%08X%08X\n",
                     (UINT32)(HeciMsg.PlatformInfo >> 32), (UINT32)HeciMsg.PlatformInfo));
  DEBUG((EFI_D_INFO, "[SPS]   Altitude: %d\n", HeciMsg.Altitude));
  
  if ((HeciMsg.PStatesRatio[0] != 0))
  {
    DEBUG((EFI_D_INFO, "[SPS]   PStatesRatio[%d]: ", HeciMsg.PStatesNumber));
    for (Index = 0; Index < sizeof(HeciMsg.PStatesRatio)/sizeof(HeciMsg.PStatesRatio[0]); Index++)
    {
      DEBUG((EFI_D_INFO, " %02X", HeciMsg.PStatesRatio[Index]));
    }
    DEBUG((EFI_D_INFO, "\n"));
    HeciMsgLen = sizeof(HeciMsg);
  }
  else
  {
    HeciMsgLen = sizeof(HeciMsg) - sizeof(HeciMsg.PStatesRatio);
  }
  Status = pHeciProtocol->SendMsg((UINT32*)&HeciMsg, HeciMsgLen,
                                      SPS_CLIENTID_BIOS, SPS_CLIENTID_ME_MKHI);
  if (EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[SPS] ERROR: Cannot send host configration info to NM (status: %r)\n", Status));
  }
  return Status;
}

