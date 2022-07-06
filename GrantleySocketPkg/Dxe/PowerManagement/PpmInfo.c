//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Code File for CPU Power Management

  Copyright (c) 2006 - 2019, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  PpmInfo.c

**/

#include "PpmInitialize.h"
#include <Library/CpuConfigLib.h>


PPM_FROM_PPMINFO_HOB *mPpmInfo;

EFI_MP_SERVICES_PROTOCOL        *mMpService = NULL;
EFI_GUID gEfiMpServiceProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;



VOID InitializeCpuInfoStruct( VOID ){

  EFI_PLATFORM_INFO           *PlatformInfo;
  struct SystemMemoryMapHob   *SystemMemoryMap;
  EFI_CPU_CSR_ACCESS_PROTOCOL *CpuCsrAccess;
  IIO_UDS                     *UdsHobPtr;
  UINT8                       socket;
  EFI_STATUS                  Status;
  EFI_HOB_GUID_TYPE           *GuidHob;
  VOID                        *HobList;
  EFI_GUID                    UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  MSR_REGISTER                CoreThreadCount;
  UINT64                      RegAddr = 0;
  UINTN                       NumberOfCPUs = 0;
  UINTN                       numberOfEnabledCPUs = 0;
  UINT32                      CapId4 = 0;
  UINT32                      CapId5 = 0;
  UINT32                      NumberOfActiveCores   = 0;
  UINT32                      NumberOfActiveThreads = 0;

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &CpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  UdsHobPtr = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (UdsHobPtr);

  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  GuidHob = GetFirstGuidHob (&gEfiMemoryMapGuid);
  if (GuidHob != NULL){
    SystemMemoryMap = (struct SystemMemoryMapHob*) GET_GUID_HOB_DATA (GuidHob);
    ASSERT (SystemMemoryMap);
  } else {
    SystemMemoryMap = NULL;
    Status = EFI_DEVICE_ERROR;
    ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (PPM_FROM_PPMINFO_HOB), (VOID **) &mPpmInfo );
  ASSERT_EFI_ERROR (Status);
  ZeroMem(mPpmInfo, sizeof(PPM_FROM_PPMINFO_HOB));

  for(socket = 0; socket < MAX_SOCKET; socket++){
    mPpmInfo->IioBusNumber[socket] = UdsHobPtr->PlatformData.IioQpiInfo[socket].BusNum;
    mPpmInfo->UncoreBusNumber[socket] = UdsHobPtr->PlatformData.CpuQpiInfo[socket].BusNum ;
    mPpmInfo->TotHa[socket] = UdsHobPtr->PlatformData.TotHa[socket] ;
    if ( SystemMemoryMap && SystemMemoryMap->NodeInfo[socket*MAX_HA].Enabled) {
      mPpmInfo->dimmTypePresent[socket] = SystemMemoryMap->NodeInfo[socket*MAX_HA].DimmType;
      mPpmInfo->ddrVoltage[socket] = SystemMemoryMap->NodeInfo[socket*MAX_HA].DdrVoltage;
    } else if (SystemMemoryMap && SystemMemoryMap->NodeInfo[socket*MAX_HA + 1].Enabled) {
      mPpmInfo->dimmTypePresent[socket] = SystemMemoryMap->NodeInfo[socket*MAX_HA + 1].DimmType;
      mPpmInfo->ddrVoltage[socket] = SystemMemoryMap->NodeInfo[socket*MAX_HA +  1].DdrVoltage;
    }
    DEBUG ((EFI_D_INFO, "Socket =%x dimmTypePresent = %x \n",socket,mPpmInfo->dimmTypePresent[socket]));
  }
  mPpmInfo->mmCfgBase=UdsHobPtr->SystemStatus.mmCfgBase; 
  mPpmInfo->SocketPresentBitMap = UdsHobPtr->SystemStatus.socketPresentBitMap; 
  mPpmInfo->DDRXOVERMODE = UdsHobPtr->SystemStatus.DDRXOVERMODE; 
  mPpmInfo->BoardId = PlatformInfo->Type; 
  mPpmInfo->CpuType = PlatformInfo->CpuType; 
  mPpmInfo->CpuStepping = UdsHobPtr->SystemStatus.MinimumCpuStepping;
  mPpmInfo->NumberOfProcessors = NumberOfCPUs;

  CoreThreadCount.Qword = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
  PlatformInfo->CpuData.CpuPCPSInfo = (UINT32) (CoreThreadCount.Dwords.Low & THREAD_COUNT_MASK);
  NumberOfActiveCores   = (UINT32) ((CoreThreadCount.Qword >> 16) & 0xFFFF);
  NumberOfActiveThreads = (UINT32) (CoreThreadCount.Qword & 0xFFFF);

  DEBUG ((EFI_D_INFO, "Number of Active Cores / Threads = %d / %d\n", NumberOfActiveCores, NumberOfActiveThreads));
 
 
   
  if (CoreThreadCount.Dwords.High != CoreThreadCount.Dwords.Low) {
    PlatformInfo->CpuData.CpuPCPSInfo |= B_PCPS_HT_ENABLE;
  }

  CapId4 = CpuCsrAccess->ReadCpuCsr(0, 0, CAPID4_PCU_FUN3_REG);
  CapId5 = CpuCsrAccess->ReadCpuCsr(0, 0, CAPID5_PCU_FUN3_REG);
  PlatformInfo->CpuData.SkuSlices = CapId5 & LLC_CLICE_EN_MASK;
  DEBUG((EFI_D_ERROR, ":::: CapId5 = %x,   SkuSlices = %x\n", CapId5, PlatformInfo->CpuData.SkuSlices));

  if ((CapId4 & B_PCPS_DISABLE) == 0) {
    PlatformInfo->CpuData.CpuPCPSInfo |= B_PCPS_SPD_ENABLE;
  }
  DEBUG((EFI_D_ERROR, ":::: CapId4 = %x,   CpuPCPSInfo = %x\n", CapId4, PlatformInfo->CpuData.CpuPCPSInfo));

  //DEBUG
  DEBUG((EFI_D_INFO, "Socket Present BitMap =%x, mmCfgBase =%x,  BoardId =%x, CpuType =%x \n", mPpmInfo->SocketPresentBitMap, mPpmInfo->mmCfgBase,  mPpmInfo->BoardId, mPpmInfo->CpuType));

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &mMpService
                  );
  ASSERT (mMpService);
  //
  // Determine the number of processors
  //
  mMpService->GetNumberOfProcessors (
              mMpService,
              &NumberOfCPUs,
              &numberOfEnabledCPUs
              );

  RegAddr = PcdGet64(PcdCpuPmStructAddr);
  if(RegAddr == 0) {
    DEBUG ( (EFI_D_ERROR, " \n\n :: !!!NULL pointer on CpuPM struct PCD!!!. \n\n") );
    ASSERT(RegAddr != 0);
  } else {
    DEBUG((EFI_D_INFO, " CpuPM struct PCD addr = %x\n", RegAddr));
  }

  mPpmLib.Info  = mPpmInfo;
  mPpmLib.Setup = (EFI_PPM_STRUCT *)(RegAddr);
  mPpmLib.CpuCsrAccess = CpuCsrAccess;
  mPpmLib.Info->EistCap = ((PcdGet32 (PcdCpuProcessorFeatureCapability) & PCD_CPU_TURBO_MODE_BIT) ? TRUE : FALSE);
  mPpmLib.Info->CapId4 = CapId4;
  mPpmLib.Info->CapId5 = CapId5;

  GetMsrTemperatureTarget((EFI_CPU_PM_STRUCT *)&mPpmLib);

  return;
}
