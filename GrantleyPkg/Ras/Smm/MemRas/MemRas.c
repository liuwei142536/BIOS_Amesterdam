/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Copyright (c) 2008 - 2015 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    MemRas.c
   
Abstract:
    Mem Ras functions and platform data, which needs to be 
    ported.

-----------------------------------------------------------------------------*/

//
// Includes
//
#include "MemRas.h"
#include "PatrolScrub.h"
#include "RankSparing.h"
#include "DramDeviceCorrection.h"
#include "HardErrorDetection.h"
#include "Platform.h"
#pragma optimize("",off)

BIOS_ACPI_PARAM       		  *mAcpiParameter;
EFI_MEM_RAS_DRIVER_PRIVATE  *mMemRasPrivate;
struct SystemMemoryMapHob   *mSystemMemoryMap = NULL;
SMMELOG_HOST                mMemrasS3Param;
EFI_CPU_CSR_ACCESS_PROTOCOL *mCpuCsrAccess;
EFI_GUID              mSystemConfigurationGuid = SYSTEM_CONFIGURATION_GUID;
SYSTEM_CONFIGURATION  mSystemConfiguration;
UINT8 OppSelfRefresh = 0;

#define EFI_MEM_RAS_SIGNATURE  SIGNATURE_32 ('R', 'M', 'E', 'M')

UINT8    *mMrcPrivateStack;
UINT8    *mCurrHostStack;
UINT8    *mCurrGuestStack;
UINT8    *mGuestStackTop;
VOID     *mScheduleFunc;
struct   sysHost mHost;
UINT64   mRasFlags;
UINT8    mMaxBusNumber;

#ifndef GRANGEVILLE_FLAG
EFI_SMM_BASE2_PROTOCOL *mSmmBase;
EFI_SMM_SYSTEM_TABLE2         *mSmst=NULL;
#endif // GRANGEVILLE_FLAG
//
// Driver entry point
//

#ifndef EFI_DEADLOOP
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif


EFI_STATUS
InitializeMemRas (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_SYSTEM_TABLE     *SystemTable
    )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EFI_IIO_UDS_PROTOCOL  *mIioUds;
  UINT8 Node, Socket;
  UINTN StackSize;
  EFI_HOB_GUID_TYPE         *GuidHob;
  UINTN   VarSize;
  CAPID3_PCU_FUN3_BDX_STRUCT capId3;
  MCMTR2_MC_MAIN_STRUCT  imcMcmtr2;
  UINT8  mMpstEnabled = FALSE;

  UINT8 SocketId = 00;
  UINT8 MemoryControllerId = 00;
  UINT8 ChannelId = 00;
  UINT8 DimmSlot = 00;
  UINT8 DimmRank = 00;
  UINT16 i=0;
  UINT32 Row = 00;
  UINT32 Col = 00;
  UINT8  Bank = 00;
  UINT8  BankGroup=00;
  UINT64 SysAddress=0;
  UINT64 TestAddr=0;
  UINT8  ChipSelect = 0;

  //
  // Allocate the protocol instance and install the protocol
  //
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_MEM_RAS_DRIVER_PRIVATE), &mMemRasPrivate);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mMemRasPrivate, sizeof (EFI_MEM_RAS_DRIVER_PRIVATE));
  
  //
  // Allocate the protocol instance and install the protocol
  //
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (EFI_MEM_RAS_PROTOCOL), &mMemRas);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mMemRas, sizeof (EFI_MEM_RAS_PROTOCOL));

  mMemRasPrivate->mMemRas = mMemRas;

  //
  // Allocate the MRC statck for 32K
  //
  StackSize = 0x8000;
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, StackSize, &mMrcPrivateStack);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mMrcPrivateStack, StackSize);

  mGuestStackTop = mMrcPrivateStack + StackSize - sizeof(UINTN);
  mCurrHostStack = NULL; 
  mCurrHostStack = NULL;

  //
  // Allocate the protocol instance and install the protocol
  // 
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (struct SystemMemoryMapHob), &mMemRas->SystemMemInfo);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mMemRas->SystemMemInfo, sizeof (struct SystemMemoryMapHob ));

  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &mAcpiParameter);
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof(SYSTEM_CONFIGURATION);
  // AptioV Server Override: Changed variable name to IntelSetup
  Status = SystemTable->RuntimeServices->GetVariable(L"IntelSetup", &mSystemConfigurationGuid, NULL, &VarSize, &mSystemConfiguration);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize our protocol
  //
  mMemRasPrivate->Signature = 0x12345678;
  Status = InitializeMemRasData( mMemRas );
  ASSERT_EFI_ERROR (Status);

  mMemRasPrivate->Signature = EFI_MEM_RAS_SIGNATURE;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmPeriodicTimerDispatch2ProtocolGuid, NULL, (VOID **)&mSmmPeriodicTimerDispatch);
  ASSERT_EFI_ERROR (Status);
#ifndef GRANGEVILLE_FLAG
  // Find the SMM base protocol
  //
  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &mSmmBase);
  ASSERT_EFI_ERROR (Status);
  mSmmBase->GetSmstLocation (mSmmBase, &mSmst);
#endif  // GRANGEVILLE_FLAG

  mMemRas->MemReadPciCfg                 = MemReadPciCfg;
  mMemRas->MemWritePciCfg                = MemWritePciCfg;
  mMemRas->PciReadCfg                    = PciReadCfg;
  mMemRas->PciWriteCfg                   = PciWriteCfg;
  mMemRas->GetCpuCfgPciAddress           = GetCpuCfgPciAddress;
  mMemRas->IsMemNodeEnabled              = IsMemNodeEnabled;

  mMemRas->EnableErrorSignallingtoBIOS   = EnableErrorSignallingtoBIOS;
  mMemRas->ReEnablePatrolScrubEngine     = ReEnablePatrolScrubEngine;
  mMemRas->DisablePatrolScrubEngine      = DisablePatrolScrubEngine;
  mMemRas->GetCurrentMemoryNodeState     = GetCurrentMemoryNodeState;
#ifndef GRANGEVILLE_FLAG      
  mMemRas->InitializeHardErrorDetection          = InitializeHardErrorDetection;
  mMemRas->CheckAndHandleHardErrorDetection      = CheckAndHandleHardErrorDetection;
  mMemRas->SuppressCMCI = SuppressCMCI;

  mMemRas->InitializeRankSparing         = InitializeRankSparing;
  mMemRas->CheckAndHandleRankSparing     = CheckAndHandleRankSparing;
#endif  // GRANGEVILLE_FLAG
  mMemRas->InitializeDevTagging          = InitializeDevTagging;
  mMemRas->CheckAndHandleDevTagging      = CheckAndHandleDevTagging;
  mMemRas->ClearUnhandledCorrError       = ClearUnhandledCorrError;
  mMemRas->GetLastErrInfo                = GetLastErrInfo;
  mMemRas->OpenPamRegion                 = OpenPamRegion;
  mMemRas->RestorePamRegion              = RestorePamRegion;
  mMemRas->SystemAddressToDimmAddress    = SystemAddressToDimmAddress;
  mMemRas->DimmAddressToSystemAddress    = DimmAddressToSystemAddress;
  mMemRas->GetBitMapOfNodeWithErrors          = GetBitmapOfNodeWithErrors;
  mMemRas->GetBitMapOfNodeWithEventInProgress = GetBitMapOfNodeWithEventInProgress;

  // Locate the IIO Protocol Interface
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);

  Status = gSmst->SmmLocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess); 
  ASSERT_EFI_ERROR (Status);

  mMaxBusNumber = mIioUds->IioUdsPtr->PlatformData.MaxBusNumber;

  mMemRas->MmCfgBaseAddress = (UINT32)mIioUds->IioUdsPtr->PlatformData.PciExpressBase;

  //
  // Search for the Memory Map GUID HOB.  If it is not present, then there's nothing we can do. It may not exist on the update path.
  // Dont forget to update this structure after mem hotAdd/hotRemove/Migration otherwise PatrolScrub and MemCopy engine would be screwed
  //
  mSystemMemoryMap = NULL;
  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if (GuidHob == NULL) {
    ASSERT (FALSE);
  }

  mSystemMemoryMap = (struct SystemMemoryMapHob *) GET_GUID_HOB_DATA (GuidHob);

  CopyMem(mMemRas->SystemMemInfo, mSystemMemoryMap, sizeof(struct SystemMemoryMapHob));

  //
  // mSystemMemoryMap is used by runtime RAS modules.  It is pointing to the HOB now.
  // Reassign mSystemMemoryMap to mMemRas->SystemMemInfo so it can be used at runtime
  //
  mSystemMemoryMap = mMemRas->SystemMemInfo;

  //
  // Update Host structure for using for Mem RAS driver
  // Need to bring struct sysHost to DXE SMM module!
  //
  ZeroMem (&mHost, sizeof (struct sysHost));
  mHost.var.common.cpuType = mIioUds->IioUdsPtr->SystemStatus.cpuType;
  mHost.var.common.cpuSubType = mIioUds->IioUdsPtr->SystemStatus.cpuSubType;
  mHost.var.common.stepping = mIioUds->IioUdsPtr->SystemStatus.MinimumCpuStepping;
  mHost.var.common.socketPresentBitMap = mIioUds->IioUdsPtr->SystemStatus.socketPresentBitMap;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    mHost.var.common.busIio[Socket] = mIioUds->IioUdsPtr->PlatformData.IioQpiInfo[Socket].BusNum;
    mHost.var.common.busUncore[Socket] = mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[Socket].BusNum;
  }

  mHost.var.common.mmCfgBase = (UINT32)mIioUds->IioUdsPtr->PlatformData.PciExpressBase;

  mMemRasPrivate->Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
      &mMemRasPrivate->Handle,
      &gEfiMemRasProtocolGuid,
      EFI_NATIVE_INTERFACE,
      mMemRasPrivate->mMemRas
  );
  ASSERT_EFI_ERROR (Status);

  if (mSystemConfiguration.Mpst) {
    for (Node = 0; Node < MAX_MEM_NODE; Node++) {
      if (mSystemMemoryMap->NodeInfo[Node].Enabled == 0) continue;
      capId3.Data = MemReadPciCfg(NodeToSkt(Node), 0, CAPID3_PCU_FUN3_REG);
      if (!(capId3.Bits.disable_monroe)) {
        //
        // Enable MPST
        //
        imcMcmtr2.Data = MemReadPciCfg(Node, PARAM_UNUSED, MCMTR2_MC_MAIN_REG);
        imcMcmtr2.Bits.monroe_chn_force_sr = 0xF;
        MemWritePciCfg (Node, PARAM_UNUSED, MCMTR2_MC_MAIN_REG, imcMcmtr2.Data);
        mMpstEnabled = TRUE;
      }
    }
    OppSelfRefresh = mSystemConfiguration.oppSR;
    if (mMpstEnabled == TRUE) {
      Status = InitializeMpst();
      ASSERT_EFI_ERROR (Status);
    }
  }

  IniMemRasS3();

  //
  // Initialize Device Tagging
  //
  InitializeDevTagging();
#ifndef GRANGEVILLE_FLAG
  //
  if(mIioUds->IioUdsPtr->SystemStatus.cpuType == CPU_BDX) 
    InitializeHardErrorDetection(); 
  //initialize spare rank
  //
  InitializeRankSparing();
#endif  // GRANGEVILLE_FLAG
  //
  //initialize patrol scrub
  //
  InstallPatrolSmiHandler();

  if(0) {

    TestAddr = 0x100330000;
    while(1) {
      DEBUG ((DEBUG_ERROR, "\n Address translation test : \n"));  
      for(i=0; i<0x600; i=i+0x40) {
        SysAddress = TestAddr+i;
        mMemRas->SystemAddressToDimmAddress (SysAddress,&SocketId,&MemoryControllerId,&ChannelId,&DimmSlot,&DimmRank,&Row,&Col,&Bank,&BankGroup,&ChipSelect);
        DEBUG ((DEBUG_ERROR, "\t Forward translation System address  = %lx \n", SysAddress));
        DEBUG ((DEBUG_ERROR, "\t\t SocketId = %x \n", SocketId));
        DEBUG ((DEBUG_ERROR, "\t\t MemoryControllerId = %x \n", MemoryControllerId));
        DEBUG ((DEBUG_ERROR, "\t\t ChannelId = %x \n", ChannelId));
        DEBUG ((DEBUG_ERROR, "\t\t DimmSlot = %x \n", DimmSlot));
        DEBUG ((DEBUG_ERROR, "\t\t DimmRank= %x \n", DimmRank));
        DEBUG ((DEBUG_ERROR, "\t\t Row = %x \n", Row));
        DEBUG ((DEBUG_ERROR, "\t\t Col = %x \n", Col));
        DEBUG ((DEBUG_ERROR, "\t\t Bank = %x \n", Bank));
        DEBUG ((DEBUG_ERROR, "\t\t BankGroup = %x \n", BankGroup));

        mMemRas->DimmAddressToSystemAddress(SocketId,MemoryControllerId,ChannelId,DimmSlot,DimmRank,Row,Col,Bank,BankGroup,ChipSelect, &SysAddress);
        DEBUG ((DEBUG_ERROR, "\t SysAddress after reverse trnaslation :  %lx \n", SysAddress));
    	  if( (TestAddr +i) != SysAddress) {
          DEBUG ((DEBUG_ERROR, "Address mismatch happened \n\n", SysAddress));
          EFI_DEADLOOP();
        }
      }
      TestAddr = TestAddr + 0x50000;
    }
  }
  //
  //Asymmetric DIMM workaround for Patrol Scrub
  //
  InstallAsymmetricDIMMPatrolScrubHandler();
  return Status;
}
