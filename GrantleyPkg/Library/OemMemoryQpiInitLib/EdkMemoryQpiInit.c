/*++
  This file contains an 'Intel Pre-EFI Module' and is licensed
  for Intel CPUs and Chipsets under the terms of your license
  agreement with Intel or your vendor.  This file may be
  modified by the user, subject to additional terms of the
  license agreement
--*/
/*++

Copyright (c) 1996 - 2015, Intel Corporation.

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

  EdkMemoryQpiInit.c

--*/

#pragma warning(disable : 4100)

#include "EdkMemoryQpiInit.h"
#include <Library/OemMemoryQpiInit.h>

#ifndef MAX_HOB_ENTRY_SIZE
#define MAX_HOB_ENTRY_SIZE  60*1024
#endif


#ifdef BDAT_SUPPORT 
#include "bdat.h"
EFI_GUID gEfiMemoryMapDataHobBdatGuid  = BDAT_MEMORY_DATA_4B_GUID;
EFI_GUID gEfiRmtDataHobBdatGuid        = RMT_SCHEMA_5_GUID;
#endif


// Aptio V Server Override - Start : Fix for OemGetPlatformVariableTagPtr corruption 
extern EFI_STATUS
OemGetPlatformVariableTagPtr(
  struct sysHost             *host,
  VOID                       **VariableTagPtr,
  SYSTEM_CONFIGURATION  *TotalSystemConfigurationPtr
  );
// // Aptio V Server Override - End : Fix for OemGetPlatformVariableTagPtr corruption
//
//  PCIE port layout
//
IIO_DMI_PCIE_INFO  mPcieInfoDxe = {
    //
    // PortInfo[NUMBER_PORTS_PER_SOCKET];
    //
    // Do not reorder these... entry #0, #1, #3, & #7 need to be the bifurcatable root ports
    //
    {
        // DMI/PCIE 0
        { PCIE_PORT_0_DEV, PCIE_PORT_0_FUNC },

        //IOU2
        { PCIE_PORT_1_DEV, PCIE_PORT_1A_FUNC },
        { PCIE_PORT_1_DEV, PCIE_PORT_1B_FUNC },

        //IOU0
        { PCIE_PORT_2_DEV, PCIE_PORT_2A_FUNC },
        { PCIE_PORT_2_DEV, PCIE_PORT_2B_FUNC },
        { PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC },
        { PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC },

        //IOU1
        { PCIE_PORT_3_DEV, PCIE_PORT_3A_FUNC },
        { PCIE_PORT_3_DEV, PCIE_PORT_3B_FUNC },
        { PCIE_PORT_3_DEV, PCIE_PORT_3C_FUNC },
        { PCIE_PORT_3_DEV, PCIE_PORT_3D_FUNC }
    }
};

VOID
PublishHobData (
  struct sysHost             *host
  )
/*++

Routine Description:

  Puts the host structure into HOB variable

Arguments:

  host - pointer to sysHost structure on stack

Returns:

 VOID

--*/
{
  EFI_STATUS                            Status;
  EFI_PEI_SERVICES                      **PeiServices;
  IIO_UDS                               IioUds;         // Module Universal Data Store!
  EFI_GUID                              UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  struct SystemMemoryMapHob             systemMemoryMap;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  UINT8                                 Ctr1,Ctr2;
  UINT16                                MemInfoMemFreq;
  UINT32                                MemInfoTotalMem;
  UINT8                                 node, socket;
  UINT8                                 haNum,i,ch,dimm,haCh,MaxNodeCount;
  UINT8                                 rank;
  PREV_BOOT_ERR_SRC_HOB                 *prevBootErrorHob;
  MCBANK_ERR_INFO                       *mcBankErrInfo;
  UINT8                                 ScratchCounter = 0;
  struct memVar                         *mem;
  UINT8                                 profile;
  UINT16                                ioLimit;
  UINT32                                mmiolLimit;
  UINT64                                mmiohLimit;
  VOID                                  *VariableTagPtr;
  // Aptio V Server Override - Start : Fix for OemGetPlatformVariableTagPtr Corruption
  SYSTEM_CONFIGURATION                  TotalSystemConfiguration;
  SYSTEM_CONFIGURATION                  *TotalSystemConfigurationPtr = &TotalSystemConfiguration;
  // Aptio V Server Override - End : Fix for OemGetPlatformVariableTagPtr Corruption
#ifdef LT_FLAG
  EFI_PLATFORM_TXT_DEVICE_MEMORY        PlatformTxtDeviceMemory;
  EFI_PHYSICAL_ADDRESS                  TxtDprAddress;
  EFI_PHYSICAL_ADDRESS                  TxtDevAddress;
  UINT32                                LtMem = 0;
  UINT32                                LtMemSize = 0;
  UINT32                                DprSize = 0;
#endif
#ifdef BDAT_SUPPORT
#ifdef SSA_FLAG
  UINT8                                 GuidIdx;
#endif //SSA_FLAG
#endif //BDAT_SUPPORT

  PeiServices      = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  node = 0;

  //
  // Update the platform info hob with system PCI resource info based on CSI output structure
  //
  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);


  // report CPU 0 resource into PlatformInfo
  PlatformInfo->PciData.PciResourceIoBase      = 0x1000;//host->var.qpi.CpuInfo[0].CpuRes.IoBase;
  PlatformInfo->PciData.PciResourceIoLimit     = host->var.qpi.CpuInfo[0].CpuRes.IoLimit;
  //
  // Get the highest IO limit in the system
  //
  for(Ctr1 = 1; Ctr1 < MAX_SOCKET; Ctr1++) {
    ioLimit = host->var.qpi.CpuInfo[Ctr1].CpuRes.IoLimit;
    if(PlatformInfo->PciData.PciResourceIoLimit < ioLimit)
      PlatformInfo->PciData.PciResourceIoLimit = ioLimit;
  }
  PlatformInfo->PciData.PciResourceMem32Base   = host->var.qpi.CpuInfo[0].CpuRes.MmiolBase;
  PlatformInfo->PciData.PciResourceMem32Limit  = host->var.qpi.CpuInfo[0].CpuRes.MmiolLimit;
  //
  // Get the highest MMIOL limit in the system
  //
  for(Ctr1 = 1; Ctr1 < MAX_SOCKET; Ctr1++) {
    mmiolLimit = host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiolLimit;
    if(PlatformInfo->PciData.PciResourceMem32Limit < mmiolLimit)
      PlatformInfo->PciData.PciResourceMem32Limit = mmiolLimit;
  }
  PlatformInfo->PciData.PciResourceMem64Base   = LShiftU64((UINT64)(host->var.qpi.CpuInfo[0].CpuRes.MmiohBase.hi), 32);
  PlatformInfo->PciData.PciResourceMem64Base   |= (UINT64)host->var.qpi.CpuInfo[0].CpuRes.MmiohBase.lo;
  PlatformInfo->PciData.PciResourceMem64Limit  = LShiftU64((UINT64)(host->var.qpi.CpuInfo[0].CpuRes.MmiohLimit.hi), 32);
  PlatformInfo->PciData.PciResourceMem64Limit  |= (UINT64)host->var.qpi.CpuInfo[0].CpuRes.MmiohLimit.lo;
  //
  // Get the highest MMIOH limit in the system
  //
  for(Ctr1 = 1; Ctr1 < MAX_SOCKET; Ctr1++) {
    mmiohLimit = LShiftU64((UINT64)(host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohLimit.hi), 32);
    mmiohLimit |= (UINT64)host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohLimit.lo;
    if(PlatformInfo->PciData.PciResourceMem64Limit < mmiohLimit) {
      PlatformInfo->PciData.PciResourceMem64Limit = mmiohLimit;
    }
  }
  PlatformInfo->PciData.PciExpressBase         = GetMmcfgAddress(host, 0);
  PlatformInfo->PciData.PciExpressSize         = 0x10000000;
  PlatformInfo->CpuData.CpuAddressWidth        = 46; //hardcode to align with the value in MemoryCallback.c bugbug fix me
  DEBUG((EFI_D_ERROR, "mmCfgBase  %x\n",host->var.common.mmCfgBase));


  PlatformInfo->MemData.MemMaxTolm             = (UINT32) PlatformInfo->PciData.PciExpressBase;
  PlatformInfo->MemData.MemTolm                = host->var.mem.lowMemSize * (64 * 1024 * 1024);
  PlatformInfo->MemData.MemMir0                = PlatformInfo->PciData.PciResourceMem64Base;
  PlatformInfo->MemData.MemMir1                = PlatformInfo->PciData.PciResourceMem64Limit + 1;

  // Initialize UDS stack variables to zero
  MemSetLocal ((UINT8 *) &IioUds, 0, sizeof(IioUds));

  VariableTagPtr = NULL;
 // Aptio V Server Override - Start : Fix for OemGetPlatformVariableTagPtr Corruption
  Status = OemGetPlatformVariableTagPtr ( host, &VariableTagPtr,TotalSystemConfigurationPtr);
 // Aptio V Server Override - End : Fix for OemGetPlatformVariableTagPtr Corruption
  // Aptio V Server Override - Start : Fix for Nvram Corruption
  //ASSERT (VariableTagPtr);
  if (EFI_ERROR (Status)) {
    DEBUG((EFI_D_INFO, "GetVariable is failed... so load default values...\n"));
    /* Aptio V Paul override
    (*PeiServices)->CopyMem (
                      &(IioUds.SetupInfo.Config),
                      &IioConfigDefault,
                      sizeof (IIO_CONFIG)
                      );
                      */

    (*PeiServices)->CopyMem (
                      &(IioUds.SetupInfo.CommonRcConfig),
                      &CommonRcConfigDefault,
                      sizeof (COMMONRC_CONFIG)
                      );

    (*PeiServices)->CopyMem (
                      &(IioUds.SetupInfo.CsiConfig),
                      &CsiConfigDefault,
                      sizeof (CSI_CONFIG)
                      );

    (*PeiServices)->CopyMem (
                      &(IioUds.SetupInfo.MemoryConfig),
                      &MemoryConfigDefault,
                      sizeof (MEMORY_CONFIG)
                      );

    (*PeiServices)->CopyMem (
                      &(IioUds.SetupInfo.MiscConfig),
                      &MiscConfigDefault,
                      sizeof (MISC_CONFIG)
                      );

    (*PeiServices)->CopyMem (
                  &(TotalSystemConfigurationPtr->VariableTag),
                  &(IioUds.SetupInfo),
                  sizeof (SETUP_INFO)
                  );

    #ifdef LT_FLAG
    TotalSystemConfigurationPtr->ProcessorSmxEnable = 1;
    TotalSystemConfigurationPtr->TpmEnable = 1;
    #endif   

    #ifdef   SERIAL_DBG_MSG
    TotalSystemConfigurationPtr->serialDebugMsgLvl = host->setup.common.serialDebugMsgLvl; // Default was already set elsewhere.
    #endif

  } else {
 // Aptio V Server Override - End : Fix for Nvram Corruption

  IioUds.PlatformData.Pci64BitResourceAllocation = PlatformInfo->PciData.Pci64BitResourceAllocation;
  }  // Aptio V Server Override : Fix for Nvram Corruption

  (*PeiServices)->CopyMem (
                  &(IioUds.SetupInfo),
                  VariableTagPtr,
                  sizeof (SETUP_INFO)
                  );

  if (host->var.common.emulation & (SOFT_SDV_FLAG | VP_FLAG)) {
    //
    // Disable for SoftSDV
    //
    IioUds.PlatformData.Pci64BitResourceAllocation = 0;
  }

  // Init UDS data here
  IioUds.PlatformData.PfSbspId                = GetSbspSktId(host);
  IioUds.PlatformData.PfGIoBase               = PlatformInfo->PciData.PciResourceIoBase;
  IioUds.PlatformData.PfGIoLimit              = PlatformInfo->PciData.PciResourceIoLimit;
  IioUds.PlatformData.PfGMmiolBase            = PlatformInfo->PciData.PciResourceMem32Base;
  IioUds.PlatformData.PfGMmiolLimit           = PlatformInfo->PciData.PciResourceMem32Limit;
  IioUds.PlatformData.PfGMmiohBase            = PlatformInfo->PciData.PciResourceMem64Base;
  IioUds.PlatformData.PfGMmiohLimit           = PlatformInfo->PciData.PciResourceMem64Limit;
  IioUds.PlatformData.MemMaxTolm              = PlatformInfo->MemData.MemMaxTolm;
  IioUds.PlatformData.MemTsegSize             = PlatformInfo->MemData.MemTsegSize;
  IioUds.PlatformData.MemIedSize              = PlatformInfo->MemData.MemIedSize;
  IioUds.PlatformData.MemMir0                 = PlatformInfo->MemData.MemMir0;
  IioUds.PlatformData.MemMir1                 = PlatformInfo->MemData.MemMir1;
  IioUds.PlatformData.PciExpressBase          = PlatformInfo->PciData.PciExpressBase;
  IioUds.PlatformData.PciExpressSize          = PlatformInfo->PciData.PciExpressSize;
  IioUds.PlatformData.numofIIO                = 0;
  IioUds.PlatformData.MaxBusNumber            = 0xFF;

  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; ++Ctr1) {
    if((host->var.common.socketPresentBitMap & (1 << Ctr1))) {
      IioUds.PlatformData.packageBspApicID[Ctr1]                   = host->var.cpu.packageBspApicID[Ctr1];  // for CpuSv <AS>
      IioUds.PlatformData.TotHa[Ctr1]                              = host->var.cpu.TotHa[Ctr1];  // for CpuSv <AS>
      IioUds.PlatformData.CpuQpiInfo[Ctr1].Valid                   = 1;
      IioUds.PlatformData.CpuQpiInfo[Ctr1].SocId                   = Ctr1;
      IioUds.PlatformData.CpuQpiInfo[Ctr1].BusNum                  = host->var.common.busUncore[Ctr1];
      for (Ctr2 = 0; Ctr2 < MAX_QPI_PORTS; ++Ctr2) {
        IioUds.PlatformData.CpuQpiInfo[Ctr1].PeerInfo[Ctr2].Valid = (UINT8)host->var.qpi.CpuInfo[Ctr1].LepInfo[Ctr2].Valid;
        IioUds.PlatformData.CpuQpiInfo[Ctr1].PeerInfo[Ctr2].PeerSocId = (UINT8)host->var.qpi.CpuInfo[Ctr1].LepInfo[Ctr2].PeerSocId;
        IioUds.PlatformData.CpuQpiInfo[Ctr1].PeerInfo[Ctr2].PeerSocType = (UINT8)host->var.qpi.CpuInfo[Ctr1].LepInfo[Ctr2].PeerSocType;
        IioUds.PlatformData.CpuQpiInfo[Ctr1].PeerInfo[Ctr2].PeerPort = (UINT8)host->var.qpi.CpuInfo[Ctr1].LepInfo[Ctr2].PeerPort;
      }
      PlatformInfo->CpuData.UncoreBusNum[Ctr1]                     = host->var.common.busUncore[Ctr1];
      DEBUG((EFI_D_ERROR, "QPI: CPU[%d] bus = %x\n",Ctr1,IioUds.PlatformData.CpuQpiInfo[Ctr1].BusNum ));
      IioUds.PlatformData.IioQpiInfo[Ctr1].Valid      = 1;
      IioUds.PlatformData.IioQpiInfo[Ctr1].SocId      = Ctr1;
      IioUds.PlatformData.IioQpiInfo[Ctr1].BusNum     = host->var.common.busIio[Ctr1];
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] bus = %x\n",Ctr1,IioUds.PlatformData.IioQpiInfo[Ctr1].BusNum ));
      IioUds.PlatformData.numofIIO++;
      IioUds.PlatformData.IIO_resource[Ctr1].Valid    = (host->var.qpi.OutIioUniphyDisable[Ctr1] == 0)? 1 : 0;  // Check IIO disable option
      IioUds.PlatformData.IIO_resource[Ctr1].SocketID = Ctr1;
      IioUds.PlatformData.IIO_resource[Ctr1].BusBase  = host->var.qpi.CpuInfo[Ctr1].CpuRes.BusBase;
      IioUds.PlatformData.IIO_resource[Ctr1].BusLimit = host->var.qpi.CpuInfo[Ctr1].CpuRes.BusLimit;
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] busbase = %x Limit=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].BusBase,
                 IioUds.PlatformData.IIO_resource[Ctr1].BusLimit));

      //
      // TODO : Hardcode this to 0x1000 if we're the legacy IIO to make sure that PCI root ports are
      // not trying to decode legacy IO ports (ACPI, GPIO, etc).
      //
// AptioV server override start
      if (Ctr1 == 0)
        IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoBase     = 0x1000; //host->var.qpi.CpuInfo[Ctr1].CpuRes.IoBase;
      else
// AptioV server override end
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoBase     = host->var.qpi.CpuInfo[Ctr1].CpuRes.IoBase;
      if(IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoBase == 0)
         IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoBase = 0x1000; //Iobase 0 was set to 0 from RC, need to reserve legacy IO
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoLimit    = host->var.qpi.CpuInfo[Ctr1].CpuRes.IoLimit;
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] IoBase = %x IoLimit=%x\n",Ctr1,host->var.qpi.CpuInfo[Ctr1].CpuRes.IoBase,
                       host->var.qpi.CpuInfo[Ctr1].CpuRes.IoLimit));
// AptioV server override start
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] IoBase = %x IoLimit=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoBase,
    		  IioUds.PlatformData.IIO_resource[Ctr1].PciResourceIoLimit));
// AptioV server override end
      IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase            = host->var.qpi.CpuInfo[Ctr1].CpuRes.IoApicBase;
      IioUds.PlatformData.IIO_resource[Ctr1].IoApicLimit           = host->var.qpi.CpuInfo[Ctr1].CpuRes.IoApicLimit;
      if((IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase < IOAPIC_BASE) ||
         (IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase >= (IOAPIC_BASE + 0x100000))) {
         IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase = IOAPIC_BASE + Ctr1 * 0x40000;
         DEBUG((EFI_D_ERROR, "QPI: IoApicBase = 0! Change it to %x\n",IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase ));
      }
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] IoApicBase = %x IoApicLimit=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].IoApicBase,
                 IioUds.PlatformData.IIO_resource[Ctr1].IoApicLimit ));
      //IioUds.PlatformData.IIO_resource[Ctr1].IioCfgBase            = ;
      //IioUds.PlatformData.IIO_resource[Ctr1].IioCfgLimit           = ;
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Base  = host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiolBase;
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit = host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiolLimit;
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem64Base  = LShiftU64((UINT64)(host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohBase.hi), 32);
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem64Base  |= (UINT64)host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohBase.lo;
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem64Limit = LShiftU64((UINT64)(host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohLimit.hi), 32);
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem64Limit |= (UINT64)host->var.qpi.CpuInfo[Ctr1].CpuRes.MmiohLimit.lo;
      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] Mem32Base = %x Mem32Limit=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Base,
                 IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit ));
      // Reserve Vt-d BAR Address of size V_VT_D_IOH_BAR_SIZE, naturally aligned on 8KB boundary as required by the chipset
      // == Limit - 16KB
      IioUds.PlatformData.IIO_resource[Ctr1].VtdBarAddress = IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit & (~0x3FFF);

      // Reserve RBCA address of size V_DMIRCBA_SIZE naturally aligned on 4KB boundary as required by the chipset
      IioUds.PlatformData.IIO_resource[Ctr1].RcbaAddress   = IioUds.PlatformData.IIO_resource[Ctr1].VtdBarAddress + V_VT_D_IIO_BAR_SIZE;

      DEBUG((EFI_D_ERROR, "QPI: IIO[%d] VtdBarAddress = %x RcbaAddress=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].VtdBarAddress,
           IioUds.PlatformData.IIO_resource[Ctr1].RcbaAddress));
      // Now shrink Mem32 Limit to reserve the space
      IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit = (IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit & (~0x3FFF)) - 1;
      DEBUG((EFI_D_ERROR, "PCI: IIO[%d] NEW!PciResourceMem32Limit=%x\n",Ctr1,IioUds.PlatformData.IIO_resource[Ctr1].PciResourceMem32Limit));

    }  else {
      IioUds.PlatformData.CpuQpiInfo[Ctr1].Valid      = 0;
      IioUds.PlatformData.CpuQpiInfo[Ctr1].SocId      = Ctr1;
      IioUds.PlatformData.IioQpiInfo[Ctr1].Valid      = 0;
      IioUds.PlatformData.IioQpiInfo[Ctr1].SocId      = Ctr1;
      IioUds.PlatformData.IIO_resource[Ctr1].Valid    = 0;
      IioUds.PlatformData.IIO_resource[Ctr1].SocketID = Ctr1;
      DEBUG((EFI_D_ERROR, "QPI: CPU[%d] is invalid\n",Ctr1));
      PlatformInfo->SysData.SysIoApicEnable &= (~(1 << (Ctr1 + 1)));
      DEBUG((EFI_D_ERROR, "QPI: IoApic[%d] is invalid\n",Ctr1));
    }
    //
    // Set PCIE port information for later usage
    //
    (*PeiServices)->CopyMem (
                    &(IioUds.PlatformData.IIO_resource[Ctr1].PcieInfo),
                    &mPcieInfoDxe,
                    sizeof (IIO_DMI_PCIE_INFO)
                    );
  }

  IioUds.SystemStatus.numCpus                  = host->var.common.numCpus;
  DEBUG((EFI_D_ERROR, "QPI: num of Cpus = %d\n",IioUds.SystemStatus.numCpus));
  DEBUG((EFI_D_ERROR, "QPI: num of IIOs = %d\n",IioUds.PlatformData.numofIIO));
  IioUds.SystemStatus.cpuType                  = host->var.common.cpuType;
  IioUds.SystemStatus.cpuSubType               = host->var.common.cpuSubType;
  IioUds.SystemStatus.cpuFamily                = host->var.common.cpuFamily;
  IioUds.SystemStatus.skuType                  = host->var.common.skuType;
  IioUds.SystemStatus.socketPresentBitMap      = host->var.common.socketPresentBitMap;
  IioUds.SystemStatus.tolmLimit                = host->var.common.tolmLimit;
  IioUds.SystemStatus.tohmLimit                = host->var.common.tohmLimit;
  IioUds.SystemStatus.mmCfgBase                = host->var.common.mmCfgBase;

  IioUds.SystemStatus.CurrentCsiLinkSpeed      = host->var.qpi.QpiCurrentLinkSpeedMode;
  IioUds.SystemStatus.CurrentCsiLinkFrequency  = host->var.qpi.OutQpiLinkSpeed;
  IioUds.SystemStatus.RcVersion                = host->var.common.rcVersion;
  IioUds.SystemStatus.MinimumCpuStepping       = host->var.common.stepping;
  IioUds.SystemStatus.MctpEn                   = host->var.common.mctpEn;
  IioUds.SystemStatus.IsocEnable               = host->var.qpi.OutIsocEn;
  IioUds.SystemStatus.MeSegEn                  = host->var.qpi.OutMesegEn;
  IioUds.SystemStatus.DDRXOVERMODE             = host->var.mem.xoverModeVar;
  
  IioUds.SystemStatus.DE_NS_SKU_ID             = host->var.common.nsSkuId;

  // RAS
  IioUds.SystemStatus.bootMode                 = host->var.common.bootMode;
  IioUds.SystemStatus.options                  = host->var.mem.options;
  IioUds.SystemStatus.spareErrTh               = host->setup.mem.spareErrTh;
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    IioUds.PlatformData.MaxCoreToBusRatio[socket] = host->var.common.MaxCoreToBusRatio[socket];
    IioUds.PlatformData.MinCoreToBusRatio[socket] = host->var.common.MinCoreToBusRatio[socket];
    IioUds.PlatformData.MemInfo[socket].enabled = host->nvram.mem.socket[socket].enabled;

    if (IioUds.PlatformData.MemInfo[socket].enabled == 0 ) continue;
    for (ch = 0; ch < MAX_CH; ch++) {
      IioUds.PlatformData.MemInfo[socket].channelList[ch].chEnabled = host->nvram.mem.socket[socket].channelList[ch].enabled;

      if (host->nvram.mem.socket[socket].channelList[ch].enabled == 1) {
        //
        // Restore S3 state info
        //
        IioUds.PlatformData.MemInfo[socket].channelList[ch].spareInUse = host->nvram.mem.socket[socket].channelList[ch].spareInUse;
        IioUds.PlatformData.MemInfo[socket].channelList[ch].chFailed = host->nvram.mem.socket[socket].channelList[ch].chFailed;
      } else {
        IioUds.PlatformData.MemInfo[socket].channelList[ch].spareDimm = 0;
        IioUds.PlatformData.MemInfo[socket].channelList[ch].spareLogicalRank = 0;
        IioUds.PlatformData.MemInfo[socket].channelList[ch].spareInUse = 0;
        IioUds.PlatformData.MemInfo[socket].channelList[ch].chFailed = 0;
      }
    } // ch
  } // socket

  //
  // HSD 4168370 Add EVMode Support to the HSX EP BIOS
  //
  IioUds.SetupInfo.MiscConfig.EVMode           = host->var.common.EVMode;

#ifdef LT_FLAG
  if(IioUds.SetupInfo.MiscConfig.EVMode)
    IioUds.SystemStatus.LtsxEnable      = 0;
  else
    IioUds.SystemStatus.LtsxEnable      = host->setup.cpu.ltsxEnable;
  DEBUG((EFI_D_ERROR, "Var LtEnable = %d\n",IioUds.SystemStatus.LtsxEnable));
#endif

  // Fill in COD Enabled bit
  IioUds.SystemStatus.OutClusterOnDieEn = host->var.qpi.OutClusterOnDieEn;

  // Notify if Link Training was done in PEI so we can skip it in DXE
  IioUds.SystemStatus.IioLinkTrainPhase = host->var.iio.IioVData.IioLinkTrainPhase;

  // Build HOB data to forward to DXE phase now that all initialization is complete!
  BuildGuidDataHob (
      &UniversalDataGuid,
      (VOID *) &IioUds,
      sizeof(IIO_UDS)
      );

  MemSetLocal ((UINT8 *) &systemMemoryMap, 0, sizeof(struct SystemMemoryMapHob));

  MemInfoMemFreq = 0;

  for(socket = 0; socket < MAX_SOCKET; socket++) {
    if ((host->var.common.socketPresentBitMap & (BIT0 << socket)) == 0) continue;
    if ((host->nvram.mem.socket[socket].enabled == 0) || (host->nvram.mem.socket[socket].maxDimmPop == 0)) continue;

    MemInfoMemFreq = host->nvram.mem.socket[socket].ddrFreqMHz;

    break;

  } // node loop

  MemInfoTotalMem = (host->var.mem.memSize << 6) ;  // Memory size in 1MB units

  // Init Memory map structure here for NUMA
  mem = &(host->var.mem);
  // For each SAD entry in system
  for(i = 0; i < SAD_RULES; i++) {
    // If SADTable entry is active, populate information into HOB
    if((mem->SAD[i].Enable) && (mem->SAD[i].IotEnabled != 1)) { //To ensure that IOT is not reported to systemMemoryMap/SRAT, other tables
      // If mode for SAD entry is non-Interleaved (Mode == 0)

      // Verify that the SAD entry is mapped completely to a single socket
      systemMemoryMap.Element[ScratchCounter].NodeId = mem->SADintList[i][0]; // Get Node ID
      if(i) {
        systemMemoryMap.Element[ScratchCounter].BaseAddress = mem->SAD[i-1].Limit; //AptioV server override: SRAT table reports incorrect system memory map when 4TB of memory or more installed.
        systemMemoryMap.Element[ScratchCounter].ElementSize = (mem->SAD[i].Limit - mem->SAD[i-1].Limit); // Get size of current element
      } else {
        //
        // If its the First SAD, then base is always zero and limit is the size for the range.
        //
        systemMemoryMap.Element[ScratchCounter].BaseAddress = 0;
        systemMemoryMap.Element[ScratchCounter].ElementSize = mem->SAD[i].Limit; // Get size of current element
      }
      DEBUG((EFI_D_ERROR, "Node:%d BaseAddress:%08x ElementSize:%08x\n",
             systemMemoryMap.Element[ScratchCounter].NodeId,
             systemMemoryMap.Element[ScratchCounter].BaseAddress,
             systemMemoryMap.Element[ScratchCounter].ElementSize));

      ScratchCounter++;    // Adding a valid entry
    }
  }

  systemMemoryMap.version = 0x01;
  systemMemoryMap.numberEntries = ScratchCounter;
  systemMemoryMap.EccEn = host->nvram.mem.eccEn;

  //
  // Copy SAD information to HOB
  //
  (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.SADintList), (VOID *)(&(host->var.mem.SADintList)), sizeof(host->var.mem.SADintList));
  (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.SAD), (VOID *)(&(host->var.mem.SAD)), sizeof(host->var.mem.SAD));
  //
  // Copy BDAT information to HOB
  //
  systemMemoryMap.SADNum       = host->var.mem.SADNum;
  systemMemoryMap.lowMemBase   = host->var.mem.lowMemBase;
  systemMemoryMap.lowMemSize   = host->var.mem.lowMemSize;
  systemMemoryMap.highMemBase  = host->var.mem.highMemBase;
  systemMemoryMap.highMemSize  = host->var.mem.highMemSize;

  systemMemoryMap.memSize      = host->var.mem.memSize;

  systemMemoryMap.memFreq = MemInfoMemFreq;
  systemMemoryMap.memMode = 0;
  //
  // AptioV Server Override Start : Added to get the number of HA per socket for the Smbios Type17 structure.
  //APTIO V serveroverride - Grangeville
//  systemMemoryMap.NoHAPerSocket = host->var.mem.numHAPerSocket;
  //
  // AptioV Server Override End : Added to get the number of HA per socket for the Smbios Type17 structure.
  //
  systemMemoryMap.IsColdBoot = host->var.mem.IsColdBoot; // AptioV Server Override: Added to skip LastBoot Error Handler for cold boot.
  //MaxNodeCount  = MAX_SOCKET;
  //if(host->var.mem.numChPerHA == 2) // 2 HA case.
    MaxNodeCount  = MAX_SOCKET * MAX_HA;

  systemMemoryMap.XMPProfilesSup = host->nvram.mem.XMPProfilesSup;

  systemMemoryMap.cpuType = host->var.common.cpuType;

//Aptio V Server Override for XTU - START 
#if defined PERF_TUNE_SUPPORT && PERF_TUNE_SUPPORT == 1
{
  UINT8                     Ch;
  UINT8                     DramEn = 0;  
  UINT32                    CsrReg = 0;  
  const UINT8   BCLKs[MAX_SUP_FREQ] = {133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133, 100, 133};  

  for (Ch = 0; Ch < 4; Ch++) { // Channel loop
    DramEn = host->nvram.mem.socket[0].channelList[Ch].enabled;
    if (DramEn == 1)
      break;
  }  
  
  //BCLK
  systemMemoryMap.Bclk = BCLKs[host->nvram.mem.ratioIndex];

  //CsrReg
  CsrReg = ReadCpuPciCfgEx (host, socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);  
  if (systemMemoryMap.Bclk == 100) {
	  CsrReg = CsrReg - 256;
  }
  systemMemoryMap.CsrReg = (UINT8)CsrReg;
  
  //CMD rate
  systemMemoryMap.currentMemTime.nCMDRate = host->nvram.mem.socket[0].channelList[Ch].timingMode;
  systemMemoryMap.currentMemTime.nCL = host->nvram.mem.socket[0].channelList[Ch].common.nCL;
  systemMemoryMap.currentMemTime.nRCD = host->nvram.mem.socket[0].channelList[Ch].common.nRCD;  
  systemMemoryMap.currentMemTime.nRP = host->nvram.mem.socket[0].channelList[Ch].common.nRP;  
  systemMemoryMap.currentMemTime.nRAS = host->nvram.mem.socket[0].channelList[Ch].common.nRAS;   
  systemMemoryMap.currentMemTime.nWR = host->nvram.mem.socket[0].channelList[Ch].common.nWR;     
  systemMemoryMap.currentMemTime.nRFC = host->nvram.mem.socket[0].channelList[Ch].common.nRFC;       
  systemMemoryMap.currentMemTime.nRRD = host->nvram.mem.socket[0].channelList[Ch].common.nRRD;         
  systemMemoryMap.currentMemTime.nRTP = host->nvram.mem.socket[0].channelList[Ch].common.nRTP;           
  systemMemoryMap.currentMemTime.nRC = host->nvram.mem.socket[0].channelList[Ch].common.nRC;             
  systemMemoryMap.currentMemTime.nWTR = host->nvram.mem.socket[0].channelList[Ch].common.nWTR;           
  systemMemoryMap.currentMemTime.nFAW = host->nvram.mem.socket[0].channelList[Ch].common.nFAW;             

  
  //Default Standard    
  systemMemoryMap.defaultMemTime.nCL = host->nvram.mem.socket[0].channelList[Ch].standard.nCL;
  systemMemoryMap.defaultMemTime.nRCD = host->nvram.mem.socket[0].channelList[Ch].standard.nRCD;  
  systemMemoryMap.defaultMemTime.nRP = host->nvram.mem.socket[0].channelList[Ch].standard.nRP;  
  systemMemoryMap.defaultMemTime.nRAS = host->nvram.mem.socket[0].channelList[Ch].standard.nRAS;   
  systemMemoryMap.defaultMemTime.nWR = host->nvram.mem.socket[0].channelList[Ch].standard.nWR;     
  systemMemoryMap.defaultMemTime.nRFC = host->nvram.mem.socket[0].channelList[Ch].standard.nRFC;       
  systemMemoryMap.defaultMemTime.nRRD = host->nvram.mem.socket[0].channelList[Ch].standard.nRRD;         
  systemMemoryMap.defaultMemTime.nRTP = host->nvram.mem.socket[0].channelList[Ch].standard.nRTP;           
  systemMemoryMap.defaultMemTime.nRC = host->nvram.mem.socket[0].channelList[Ch].standard.nRC;             
  systemMemoryMap.defaultMemTime.nWTR = host->nvram.mem.socket[0].channelList[Ch].standard.nWTR;           
  systemMemoryMap.defaultMemTime.nFAW = host->nvram.mem.socket[0].channelList[Ch].standard.nFAW;
      
  systemMemoryMap.defaultMemTime.nCWL = host->nvram.mem.socket[0].channelList[Ch].standard.nCWL;        
  systemMemoryMap.defaultMemTime.tREFI = host->nvram.mem.socket[0].channelList[Ch].standard.tREFI;       

}  
#endif  //PERF_TUNE_SUPPORT
//Aptio V Server Override for XTU - END
  for (profile = 0; profile < 2; profile++) {
    if (host->nvram.mem.XMPProfilesSup & (1 << profile)) {
      systemMemoryMap.profileMemTime[profile] = host->nvram.mem.profileMemTime[profile];
    }
  }

  // Now fill Channel/DIMM info/TAD info
  for(node = 0; node < MaxNodeCount; node++) {
	socket = NODE_TO_SKT(node);
	haNum = NODE_TO_MC(node);

    if((host->var.mem.numChPerHA == 4) && (haNum ==1)) {
      systemMemoryMap.NodeInfo[node].Enabled = 00;
      continue;
    }

    systemMemoryMap.NodeInfo[node].Enabled        = host->nvram.mem.socket[socket].haList[haNum].enabled;
    systemMemoryMap.NodeInfo[node].MemFreq        = host->nvram.mem.socket[socket].ddrFreq;
    systemMemoryMap.NodeInfo[node].MemSize        = host->var.mem.socket[socket].haList[haNum].memSize;
    // systemMemoryMap.NodeInfo[node].RequiredMemoryBase  = host->var.mem.imc[socket].RequiredMemoryBase;  // not initilized
    systemMemoryMap.NodeInfo[node].DimmType       = host->nvram.mem.dimmTypePresent;
    systemMemoryMap.NodeInfo[node].DramType       = host->nvram.mem.dramType;
    systemMemoryMap.NodeInfo[node].DdrVoltage     = host->nvram.mem.socket[socket].ddrVoltage;
    systemMemoryMap.NodeInfo[node].LrDimmPresent  = host->nvram.mem.socket[socket].lrDimmPresent;
    systemMemoryMap.NodeInfo[node].NumChPerHA     = host->var.mem.numChPerHA;
    systemMemoryMap.NodeInfo[node].HaNodeId       = host->nvram.mem.socket[socket].haList[haNum].haNodeId;
    systemMemoryMap.NodeInfo[node].RasModesEnabled= host->nvram.mem.RASmode;
    systemMemoryMap.NodeInfo[node].ExRasModesEnabled = host->nvram.mem.RASmodeEx;

#ifdef BDAT_SUPPORT
#ifdef SSA_FLAG
    systemMemoryMap.bssaNumberDistinctGuids   = host->var.mem.bssaNumberDistinctGuids;
    DEBUG((EFI_D_ERROR, "BSSA: Disctinct result GUIDs: %d\n",systemMemoryMap.bssaNumberDistinctGuids));
    systemMemoryMap.bssaBdatSize = host->var.mem.bssaBdatSize;
    DEBUG((EFI_D_ERROR, "BSSA: Total BDAT size: %d\n",systemMemoryMap.bssaBdatSize));
    for (GuidIdx = 0; GuidIdx < MAX_NUMBER_SSA_BIOS_RESULTS; GuidIdx++) {
      systemMemoryMap.bssaBdatGuid[GuidIdx]   = host->var.mem.bssaBdatGuid[GuidIdx];
      systemMemoryMap.bssaNumberHobs[GuidIdx]   = host->var.mem.bssaNumberHobs[GuidIdx];
      DEBUG((EFI_D_ERROR, "BSSA: GuidIdx %d has %d HOBs\n", GuidIdx, systemMemoryMap.bssaNumberHobs[GuidIdx]));
    }
#endif //SSA_FLAG
#endif //BDAT_SUPPORT

//AptioV server override: SMBIOS type 17 data is independent of flag in AMI BIOS
//#ifdef SDP_FLAG
    systemMemoryMap.NodeInfo[node].commonTck = host->nvram.mem.socket[socket].minTCK; //max speed for SMBIOS17
//#endif

    for (ch = 0; ch < MAX_CH; ch++) {

      haCh  = ch;
      if(host->var.mem.numChPerHA == 2) { // 2 HA case.
        if( ((haNum == 0) && (ch >= 2)) || ((haNum == 1) && (ch <  2)) )  continue;   // skip ha0ch2,3 and ha1ch 0,1
        // 2ha : ch0,1,2,3 -> ha0ch0,1 and Ha1ch0,1
        if(haNum == 1)   haCh = ch -2;
      }
      systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].Enabled  = host->nvram.mem.socket[socket].channelList[ch].enabled;
      systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].Features = host->nvram.mem.socket[socket].channelList[ch].features;
      systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].MaxDimm  = host->nvram.mem.socket[socket].channelList[ch].maxDimm;
      systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].NumRanks = host->nvram.mem.socket[socket].channelList[ch].numRanks;
      for(rank=0;rank < MAX_RANK_CH/2; rank ++){
         systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].SpareLogicalRank[rank] = host->nvram.mem.socket[socket].channelList[ch].spareLogicalRank[rank];
      }

      systemMemoryMap.NodeInfo[node].ChannelInfo[ch].EnabledLogicalRanks = 0;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].Present           = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].dimmPresent;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].NumRanks          = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].numRanks;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].X4Present         = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].x4Present;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDMMfgId         = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDMMfgId;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSize          = host->var.mem.socket[socket].channelList[ch].dimmList[dimm].memSize;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDSDRAMBanks     = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDSDRAMBanks;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].actSPDModuleOrg   = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].actSPDModuleOrg;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].keyByte           = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].keyByte;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].actKeyByte2       = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].actKeyByte2;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].actSPDSDRAMBanks  = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].actSPDSDRAMBanks;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDMemBusWidth    = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDMemBusWidth;
        systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDSecondaryDeviceType = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDSecondaryDeviceType;
        //AptioV Server Override Start : Update the speed of DIMM in the System Memory Map Hob.
        if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].dimmPresent) {
            if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK == 0) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 0;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_3200_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 3200;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_3000_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 3000;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2933_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2933;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2800_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2800;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2667_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2667;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2600_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2600;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2400_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2400;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2200_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2200;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2133_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2133;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_2000_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 2000;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1867_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1867;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1800_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1800;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1600_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1600;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1400_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1400;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1333_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1333;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1200_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1200;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1067_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1067;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_1000_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 1000;
            } else if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].minTCK <= DDR_800_TCK_MIN) {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 800;
            } else {
                systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 0;
            }
        } else {
            systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].DimmSpeed = 0;
        }
        //AptioV Server Override End : Update the speed of DIMM in the System Memory Map Hob.
//AptioV server override Start: Remove flag dependency to report all data in type 17
//#ifdef SDP_FLAG		
        for (i = 0; i < 4; i++) {
          systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDModSN[i] = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDModSN[i];
        }
        if (host->nvram.mem.dramType == SPD_TYPE_DDR3){
          for (i = 0; i < SPD_MODULE_PART; i++) {
            systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDModPart[i] = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDModPart[i];
          }
        } else {
          for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
            systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].DimmInfo[dimm].SPDModPart[i] = host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].SPDModPartDDR4[i];
          }
        }
//#endif //SDP_FLAG 
//AptioV server override End: Remove flag dependency to report all data in type 17
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].enabled == 0) continue;
          if (host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank]) continue;
          systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].EnabledLogicalRanks |=
              1 <<  host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].logicalRank;

        } // rank loop
      } // dimm loop

      //
      // Copy RIR information
      //
      (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].rirIntList),
          (VOID *)(&(host->var.mem.socket[socket].channelList[ch].rirIntList)), sizeof(host->var.mem.socket[socket].channelList[ch].rirIntList));
      (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].rirLimit),
          (VOID *)(&(host->var.mem.socket[socket].channelList[ch].rirLimit)), sizeof(host->var.mem.socket[socket].channelList[ch].rirLimit));
      (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].rirWay),
          (VOID *)(&(host->var.mem.socket[socket].channelList[ch].rirWay)), sizeof(host->var.mem.socket[socket].channelList[ch].rirWay));
      (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[socket].ChannelInfo[haCh].rirValid),
          (VOID *)(&(host->var.mem.socket[socket].channelList[ch].rirValid)), sizeof(host->var.mem.socket[socket].channelList[ch].rirValid));
      (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].ChannelInfo[haCh].rirOffset),
          (VOID *)(&(host->var.mem.socket[socket].channelList[ch].rirOffset)), sizeof(host->var.mem.socket[socket].channelList[ch].rirOffset));

    } // ch loop

    //
    // Copy TAD information
    //
    (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].TAD), (VOID *)(&(host->var.mem.socket[socket].haList[haNum].TAD)),
        sizeof(host->var.mem.socket[socket].haList[haNum].TAD));
    (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].TADChnIndex), (VOID *)(&(host->var.mem.socket[socket].haList[haNum].TADChnIndex)),
        sizeof(host->var.mem.socket[socket].haList[haNum].TADChnIndex));
    (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].TADOffset), (VOID *)(&(host->var.mem.socket[socket].haList[haNum].TADOffset)),
        sizeof(host->var.mem.socket[socket].haList[haNum].TADOffset));
    (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.NodeInfo[node].TADintList), (VOID *)(&(host->var.mem.socket[socket].haList[haNum].TADintList)),
        sizeof(host->var.mem.socket[socket].haList[haNum].TADintList));

  } // socket loop
  MemCopy ((UINT8 *)&(systemMemoryMap.MemSetup), (UINT8 *)&host->setup.mem, sizeof (struct memSetup));
  (*PeiServices)->CopyMem((VOID *)(&systemMemoryMap.pprStatus), (VOID *)(&(host->var.mem.pprStatus)), sizeof(host->var.mem.pprStatus));

#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "PrevBootErrLog - Num Valid Entries: %d\n", host->prevBootErr.validEntries));
#endif // SERIAL_DBG_MSG
  prevBootErrorHob = NULL;
  Status = (*PeiServices)->AllocatePool (
                PeiServices,
                sizeof(PREV_BOOT_ERR_SRC_HOB),
                &prevBootErrorHob
            );
  ASSERT (prevBootErrorHob);
  prevBootErrorHob->Length = (UINT16) host->prevBootErr.validEntries * sizeof(MCBANK_ERR_INFO) + 2;  // 2 is for the length field in the hob
  mcBankErrInfo = (MCBANK_ERR_INFO*) ((UINTN)prevBootErrorHob + 2);

  for (i = 0; i < host->prevBootErr.validEntries; i++) {

    mcBankErrInfo->Type	        = McBankType;
    mcBankErrInfo->Socket       = host->prevBootErr.mcRegs[i].socketId;
    mcBankErrInfo->McBankNum    = host->prevBootErr.mcRegs[i].mcBankNum;
    *(UINT64_STRUCT*)(UINTN)&mcBankErrInfo->McBankStatus = host->prevBootErr.mcRegs[i].mcStatus;
    *(UINT64_STRUCT*)(UINTN)&mcBankErrInfo->McbankAddr   = host->prevBootErr.mcRegs[i].mcAddress;
    *(UINT64_STRUCT*)(UINTN)&mcBankErrInfo->McBankMisc   = host->prevBootErr.mcRegs[i].mcMisc;

    //
    // Display data
    //

#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "PrevBootErrLog - skt: %d, MCBANK: %d, Status = %08x%08x, Addr = %08x%08x, Misc = %08x%08x\n",
        mcBankErrInfo->Socket, mcBankErrInfo->McBankNum,
        *(UINT32*)((UINTN)&mcBankErrInfo->McBankStatus + 4), *(UINT32*)((UINTN)&mcBankErrInfo->McBankStatus),
        *(UINT32*)((UINTN)&mcBankErrInfo->McbankAddr + 4), *(UINT32*)((UINTN)&mcBankErrInfo->McbankAddr),
        *(UINT32*)((UINTN)&mcBankErrInfo->McBankMisc + 4), *(UINT32*)((UINTN)&mcBankErrInfo->McBankMisc)
        ));
#endif // SERIAL_DBG_MSG

	mcBankErrInfo += sizeof(MCBANK_ERR_INFO);	// Move to next entry
  }
  //
  // Build HOB for prev boot errors
  //
  BuildGuidDataHob (
        &gPrevBootErrSrcHobGuid,
        prevBootErrorHob,
        sizeof(PREV_BOOT_ERR_SRC_HOB)
      );
  // Build HOB for system memory map
  //
  BuildGuidDataHob (
               &gEfiMemoryMapGuid,
               &systemMemoryMap,
               sizeof (struct SystemMemoryMapHob)
           );

#ifdef LT_FLAG

  if (IioUds.SystemStatus.LtsxEnable || PlatformInfo->PfatMemSize != 0) {
    DEBUG((EFI_D_ERROR, "Reserve LT Mem!Should equal (MemTolm - MemTsegSize - LtMemSize) = 0x%08x\n", PlatformInfo->MemData.MemTolm - PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemLtMemSize));
    DprSize = PlatformInfo->MemData.MemDprMemSize;
//    LtMem = PlatformInfo->MemData.MemTolm - PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemLtMemSize
    LtMem = PlatformInfo->MemData.MemLtMemAddress;
    DEBUG((EFI_D_ERROR, "LtMem: 0x%x\n",    LtMem));
    LtMem &= (~(LT_DEV_MEM_ALIGNMENT-1));
    LtMemSize = PlatformInfo->MemData.MemLtMemSize;
    TxtDprAddress = LtMem + LtMemSize - DprSize;
    TxtDevAddress = LtMem;
    PlatformTxtDeviceMemory.NoDMATableAddress                = TxtDevAddress;
    PlatformTxtDeviceMemory.NoDMATableSize                   = LtMemSize - DprSize;
    PlatformTxtDeviceMemory.SINITMemoryAddress               = TxtDprAddress + DprSize - LT_HEAP_SIZE - SINIT_MAX_SIZE;
    PlatformTxtDeviceMemory.SINITMemorySize                  = SINIT_MAX_SIZE;
    PlatformTxtDeviceMemory.TXTHeapMemoryAddress             = TxtDprAddress + DprSize - LT_HEAP_SIZE;
    PlatformTxtDeviceMemory.TXTHeapMemorySize                = LT_HEAP_SIZE;
    // Aptio V Server Override start - Pfat is not supported in Mayancity
    if (PlatformInfo->PfatMemSize != 0) 
    {
	    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionAddress = LtMem - (UINT32)LShiftU64 (PlatformInfo->PfatMemSize, 20); //TxtDprAddress;
	    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionSize    = (UINT32)LShiftU64 (PlatformInfo->PfatMemSize, 20) + LtMemSize;

    }
    
    else
    {
	    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionAddress = TxtDprAddress;
	    PlatformTxtDeviceMemory.DMAProtectionMemoryRegionSize    = DprSize;
    }
    
    // Aptio V Server Override End - Pfat is not supported in Mayancity
    DEBUG((EFI_D_ERROR, "========== LT Memory space ========== \n"));
    DEBUG((EFI_D_ERROR, "NoDMATableAddress: 0x%x\n",    PlatformTxtDeviceMemory.NoDMATableAddress));
    DEBUG((EFI_D_ERROR, "NoDMATableSize: 0x%x\n",       PlatformTxtDeviceMemory.NoDMATableSize));
    DEBUG((EFI_D_ERROR, "SINITMemoryAddress: 0x%x\n",   PlatformTxtDeviceMemory.SINITMemoryAddress));
    DEBUG((EFI_D_ERROR, "SINITMemorySize: 0x%x\n",      PlatformTxtDeviceMemory.SINITMemorySize));
    DEBUG((EFI_D_ERROR, "TXTHeapMemoryAddress: 0x%x\n", PlatformTxtDeviceMemory.TXTHeapMemoryAddress));
    DEBUG((EFI_D_ERROR, "TXTHeapMemorySize: 0x%x\n",    PlatformTxtDeviceMemory.TXTHeapMemorySize));
    DEBUG((EFI_D_ERROR, "DMAProtectionMemoryRegionAddress: 0x%x\n", PlatformTxtDeviceMemory.DMAProtectionMemoryRegionAddress));
    DEBUG((EFI_D_ERROR, "DMAProtectionMemoryRegionSize: 0x%x\n\n",  PlatformTxtDeviceMemory.DMAProtectionMemoryRegionSize));

    BuildGuidDataHob (&gEfiPlatformTxtDeviceMemoryGuid, &PlatformTxtDeviceMemory, sizeof(PlatformTxtDeviceMemory));

  }
#endif //LT_FLAG

#ifdef BDAT_SUPPORT
  if ((host->var.common.bootMode != S3Resume) && (host->setup.common.bdatEn)) SaveBdatDataToHob(host);
#endif // BDAT_SUPPORT

}

//
// Function Implementations
//
EFI_STATUS
InstallEfiMemory (
  struct sysHost             *host
  )
/*++

Routine Description:

  This function installs memory.

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  0 - success
  Other -  failure

--*/

{
  EFI_STATUS                            Status;
  UINT8                                 Index;
  UINT8                                 NumRanges;
  UINT8                                 SmramIndex;
  UINT8                                 SmramRanges;
  UINT64                                PeiMemoryLength;
  UINTN                                 BufferSize;
  UINTN                                 PeiMemoryIndex;
  UINTN                                 RequiredMemSize;
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_PHYSICAL_ADDRESS                  PeiMemoryBaseAddress;
  EFI_PHYSICAL_ADDRESS                  TopUseableMemAddr;
  EFI_PHYSICAL_ADDRESS                  TopUseableMemSize;
  PEI_MEMORY_TEST_OP                    MemoryTestOp;
  PEI_BASE_MEMORY_TEST_PPI              *BaseMemoryTestPpi;
  PEI_PLATFORM_MEMORY_SIZE_PPI          *MemSize;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE MemoryMap[MAX_RANGES];
  EFI_PHYSICAL_ADDRESS                  BadMemoryAddress;
  EFI_BOOT_MODE                         BootMode;
  EFI_PEI_SERVICES                      **gPeiServices;
  EFI_SMRAM_DESCRIPTOR                  DescriptorAcpiVariable;
#ifdef NVMEM_FEATURE_EN
  UINT32                                normal0size64 = 0;
  UINT32                                nv0size64 = 0;
  UINT32                                normal1size64 = 0;
  UINT32                                nv1size64 = 0;
  EFI_PHYSICAL_ADDRESS                  normal0size=0;
  EFI_PHYSICAL_ADDRESS                  nv0size = 0;
  EFI_PHYSICAL_ADDRESS                  normal1size = 0;
  EFI_PHYSICAL_ADDRESS                  nv1size=0;
#endif

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  DEBUG ((EFI_D_ERROR, "InstallEfiMemory()\n"));

  if(host->var.mem.subBootMode == AdrResume) {
    // Change Boot Mode to skip the memory init.
    BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;
  } else {
  //
  // Determine boot mode
  //
  Status = (*gPeiServices)->GetBootMode (gPeiServices, &BootMode);
  ASSERT_EFI_ERROR (Status);
  }

  //
  // Get the Memory Map
  //
  NumRanges = MAX_RANGES;
  MemSetLocal ((UINT8 *) MemoryMap, 0, (sizeof(PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE) * NumRanges));
  Status = GetMemoryMap ((PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE *) MemoryMap, &NumRanges, host);
  ASSERT_EFI_ERROR (Status);

  //
  // Find the highest memory range in processor native address space to give to
  // PEI. Then take the top.
  // If this algorithm changes, then we need to fix the capsule memory
  // selection algorithm below.
  //
  PeiMemoryBaseAddress = 0;

  //
  // Query the platform for the minimum memory size
  //
  Status = (*gPeiServices)->LocatePpi (gPeiServices, &gPeiPlatformMemorySizePpiGuid, 0, NULL, &MemSize);
  ASSERT_EFI_ERROR (Status);

  Status = MemSize->GetPlatformMemorySize (gPeiServices, MemSize, &PeiMemoryLength);
  ASSERT_EFI_ERROR (Status);

  //
  // Get required memory size for ACPI use. This helps to put ACPI memory on the top
  //
  RequiredMemSize = 0;
  // Aptio V Server override: WA.to disallow platform code to subtract the BS memory from platform memory.
  //RetrieveRequiredMemorySize (host, &RequiredMemSize);
  DEBUG ((EFI_D_ERROR, " RequiredMemSize = 0x%X bytes\n", RequiredMemSize));

  PeiMemoryIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    DEBUG ((EFI_D_ERROR, "Found 0x%016lX bytes at ", MemoryMap[Index].RangeLength));
    DEBUG ((EFI_D_ERROR, "0x%016lX\n", MemoryMap[Index].PhysicalAddress));
    if ((MemoryMap[Index].Type == DualChannelDdrMainMemory) &&
        (MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength < MAX_ADDRESS) &&
        (MemoryMap[Index].PhysicalAddress >= PeiMemoryBaseAddress) &&
        (MemoryMap[Index].RangeLength >= PeiMemoryLength)
        ) {
      PeiMemoryBaseAddress  = MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength - PeiMemoryLength;
      PeiMemoryIndex        = Index;
    }
  }
  //
  // Test only the PEI memory if necessary. Some platforms do not require the
  // Base Memory PEIM to be present
  //
  Status = (*gPeiServices)->LocatePpi (gPeiServices, &gPeiBaseMemoryTestPpiGuid, 0, NULL, &BaseMemoryTestPpi);

  switch (BootMode) {

  case BOOT_WITH_FULL_CONFIGURATION:
    MemoryTestOp = Quick;
    break;

  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
    MemoryTestOp = Extensive;
    break;

  default:
    MemoryTestOp = Ignore;
    break;
  }


  Status = BaseMemoryTestPpi->BaseMemoryTest (
                                gPeiServices,
                                BaseMemoryTestPpi,
                                PeiMemoryBaseAddress,
                                PeiMemoryLength,
                                MemoryTestOp,
                                &BadMemoryAddress
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "Memory test failure at 0x%lX.\n", BadMemoryAddress));
  }

  ASSERT_EFI_ERROR (Status);

  //
  // Carve out the top memory reserved for ACPI
  //
  Status = (*gPeiServices)->InstallPeiMemory (gPeiServices, PeiMemoryBaseAddress, PeiMemoryLength - RequiredMemSize);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((EFI_D_ERROR, "Building RESOURCE_SYSTEM_MEMORY Hob:\n"));
  DEBUG ((
    EFI_D_ERROR,
    " PeiMemoryBaseAddress = 0x%lX, PeiMemoryLength = 0x%lX\n",
    PeiMemoryBaseAddress,
    PeiMemoryLength)
    );

  BuildResourceDescriptorHob (
            EFI_RESOURCE_SYSTEM_MEMORY,
            (
            EFI_RESOURCE_ATTRIBUTE_PRESENT |
            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
            EFI_RESOURCE_ATTRIBUTE_TESTED |
            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
            ),
            PeiMemoryBaseAddress,
            PeiMemoryLength
            );

  //
  // Install physical memory descriptor hobs for each memory range.
  //
  SmramRanges = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if (MemoryMap[Index].Type == DualChannelDdrMainMemory) {
      if (Index == PeiMemoryIndex) {
        //
        // This is a partially tested Main Memory range, give it to EFI
        //
          BuildResourceDescriptorHob (
                  EFI_RESOURCE_SYSTEM_MEMORY,
                  (
                  EFI_RESOURCE_ATTRIBUTE_PRESENT |
                  EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                  EFI_RESOURCE_ATTRIBUTE_TESTED |
                  EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                  EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                  EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                  EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                  ),
                  MemoryMap[Index].PhysicalAddress,
                  MemoryMap[Index].RangeLength - PeiMemoryLength
                  );
        DEBUG ((EFI_D_ERROR, "Partially tested Main Memory Base:0x%lX Length:0x%lX\n",MemoryMap[Index].PhysicalAddress,MemoryMap[Index].RangeLength - PeiMemoryLength));

        //
        // Test this memory range
        //
        Status = BaseMemoryTestPpi->BaseMemoryTest (
                                      gPeiServices,
                                      BaseMemoryTestPpi,
                                      MemoryMap[Index].PhysicalAddress,
                                      MemoryMap[Index].RangeLength - PeiMemoryLength,
                                      MemoryTestOp,
                                      &BadMemoryAddress
                                      );
        ASSERT_EFI_ERROR (Status);
      } else {
        //
        // This is an untested Main Memory range, give it to EFI
        //

	//AptioV Server Override start: memory range 0-9FFFF is not available in the GetMemoryMap() service
	      if((MemoryMap[Index].PhysicalAddress==0)&&(MemoryMap[Index].RangeLength==0xa0000))
	      {
               BuildResourceDescriptorHob (
                                   EFI_RESOURCE_SYSTEM_MEMORY,
                                  (
                                  EFI_RESOURCE_ATTRIBUTE_PRESENT |
                                  EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                                  EFI_RESOURCE_ATTRIBUTE_TESTED |
                                  EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                                  EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                                  EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                                  EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                                   ),
                                  MemoryMap[Index].PhysicalAddress, // MemoryBegin
                                  MemoryMap[Index].RangeLength      // MemoryLength
                                   );
               }
	      else
	      {
	       BuildResourceDescriptorHob (
		                     EFI_RESOURCE_SYSTEM_MEMORY,
		                     (
		                     EFI_RESOURCE_ATTRIBUTE_PRESENT |
		                     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
		                     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
		                     EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
		                     EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
		                     EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
		                     ),
		                     MemoryMap[Index].PhysicalAddress, // MemoryBegin
		                     MemoryMap[Index].RangeLength      // MemoryLength
		                     );
	      DEBUG ((EFI_D_ERROR, "Untested Main Memory range Base:0x%lX Length:0x%lX\n",MemoryMap[Index].PhysicalAddress,MemoryMap[Index].RangeLength));
		      
	      }
      
        //AptioV Server Override end: memory range 0-9FFFF is not available in the GetMemoryMap() service
        //
        // Test this memory range
        //
        Status = BaseMemoryTestPpi->BaseMemoryTest (
                                      gPeiServices,
                                      BaseMemoryTestPpi,
                                      MemoryMap[Index].PhysicalAddress,
                                      MemoryMap[Index].RangeLength,
                                      MemoryTestOp,
                                      &BadMemoryAddress
                                      );
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
          (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
          ) {
        SmramRanges++;
      }
      //
      // Make sure non-system memory is marked as reserved
      //
      BuildResourceDescriptorHob (
                EFI_RESOURCE_MEMORY_RESERVED,     // MemoryType,
                0,                                // MemoryAttribute
                MemoryMap[Index].PhysicalAddress, // MemoryBegin
                MemoryMap[Index].RangeLength      // MemoryLength
                );
    }
  }

  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK);
  if (SmramRanges > 0) {
    BufferSize += ((SmramRanges - 1) * sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  Hob.Raw = BuildGuidHob (
              &gEfiSmmPeiSmramMemoryReserveGuid,
              BufferSize
              );
  ASSERT (Hob.Raw);

  SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Raw);
  SmramHobDescriptorBlock->NumberOfSmmReservedRegions = SmramRanges;

  SmramIndex = 0;

  for (Index = 0; Index < NumRanges; Index++) {

    if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
        (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
        ) {
      //
      // This is an SMRAM range, create an SMRAM descriptor
      //
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalStart = MemoryMap[Index].PhysicalAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].CpuStart      = MemoryMap[Index].CpuAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalSize  = MemoryMap[Index].RangeLength;

      if (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
      } else {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED;
      }
      SmramIndex++;
    }
  }

  //
  // Build a HOB with the location of the reserved memory range.
  //
  CopyMem(&DescriptorAcpiVariable, &SmramHobDescriptorBlock->Descriptor[SmramRanges-1], sizeof(EFI_SMRAM_DESCRIPTOR));
  DescriptorAcpiVariable.CpuStart += RESERVED_CPU_S3_SAVE_OFFSET;
  BuildGuidDataHob (
    &gEfiAcpiVariableGuid,
    &DescriptorAcpiVariable,
    sizeof (EFI_SMRAM_DESCRIPTOR)
    );

  //
  // Get the current "Top of Upper Usable Memory" address from TOUUD.
  // If TOUUD > 4G, it means memory re-mapped.
  //
  TopUseableMemSize = ((EFI_PHYSICAL_ADDRESS)host->var.mem.highMemSize) * (64 * 1024 * 1024);
  TopUseableMemAddr = ((EFI_PHYSICAL_ADDRESS)host->var.mem.highMemBase) * (64 * 1024 * 1024);

  DEBUG ((EFI_D_ERROR, "TOHM:0x%016lX\n ", TopUseableMemSize));
#ifdef NVMEM_FEATURE_EN
  // Calculate the Non volatile memory behind each socket
  if ((host->setup.mem.options & NUMA_AWARE) && (host->var.mem.socket[0].memSize) && (host->var.mem.socket[1].memSize)) {
    normal0size64 = host->var.mem.socket[0].memSize - host->var.mem.socket[0].NVmemSize;
    nv0size64 = host->var.mem.socket[0].NVmemSize;
    if (normal0size64 != 0)
      normal0size64 -= host->var.mem.lowMemSize;
    else
      nv0size64 -= host->var.mem.lowMemSize;

    normal1size64 = host->var.mem.socket[1].memSize - host->var.mem.socket[1].NVmemSize;
    nv1size64 = host->var.mem.socket[1].NVmemSize;
  } else {
    normal0size64 = host->var.mem.memSize - host->var.mem.NVmemSize;
    nv0size64 = host->var.mem.NVmemSize;
    if (normal0size64 != 0)
      normal0size64 -= host->var.mem.lowMemSize;
    else
      nv0size64 -= host->var.mem.lowMemSize;

    normal1size64 = 0;
    nv1size64 = 0;
  }
  DEBUG((EFI_D_ERROR, "host->var.mem.socket[0].memSize = 0x%x\n host->var.mem.socket[0].NVmemSize = 0x%x\n", host->var.mem.socket[0].memSize, host->var.mem.socket[0].NVmemSize));
  DEBUG((EFI_D_ERROR, "host->var.mem.socket[1].memSize = 0x%x\n host->var.mem.socket[1].NVmemSize = 0x%x\n", host->var.mem.socket[1].memSize, host->var.mem.socket[1].NVmemSize));
  DEBUG((EFI_D_ERROR, "host->var.mem.memSize = 0x%x\n host->var.mem.socket[0].NVmemSize = 0x%x\n", host->var.mem.memSize, host->var.mem.NVmemSize));
  DEBUG((EFI_D_ERROR, "host->var.mem.lowMemSize = 0x%x\n", host->var.mem.lowMemSize));
  DEBUG((EFI_D_ERROR, "normal0size64 = 0x%x\n nv0size64 = 0x%x\n normal1size64 = 0x%x\n nv1size64 = 0x%X\n ", normal0size64, nv0size64, normal1size64, nv1size64));

  normal0size = ((EFI_PHYSICAL_ADDRESS)normal0size64) * (64 * 1024 * 1024);
  nv0size = ((EFI_PHYSICAL_ADDRESS)nv0size64) * (64 * 1024 * 1024);
  normal1size = ((EFI_PHYSICAL_ADDRESS)normal1size64) * (64 * 1024 * 1024);
  nv1size = ((EFI_PHYSICAL_ADDRESS)nv1size64) * (64 * 1024 * 1024);

  DEBUG((EFI_D_ERROR, "normal0size = 0x%016lX\n nv0size = 0x%016lX\n normal1size = 0x%016lX\n nv1size = 0xY%016lX\n ", normal0size, nv0size, normal1size, nv1size));

  TopUseableMemSize = normal0size;

  while (TopUseableMemSize > MEM_EQU_4GB) {
    BuildResourceDescriptorHob (
                EFI_RESOURCE_SYSTEM_MEMORY,
                (
                EFI_RESOURCE_ATTRIBUTE_PRESENT |
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                ),
                TopUseableMemAddr,
                MEM_EQU_4GB
                );
    DEBUG((EFI_D_INFO, "Just added 4GB of System memory at =0x%lx\n", TopUseableMemAddr));
    TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
    TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
  }
  //
  // Create hob for remain memory which above 4G memory.
  //
  if(TopUseableMemSize) {
    BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
          TopUseableMemAddr,
          TopUseableMemSize
          );
    TopUseableMemAddr = TopUseableMemAddr + TopUseableMemSize;
    DEBUG((EFI_D_INFO, "Just added 0x%lx of System memory at =0x%lx\n", TopUseableMemSize,TopUseableMemAddr));
  }

  TopUseableMemSize = nv0size;

  while (TopUseableMemSize > MEM_EQU_4GB) {
    BuildResourceDescriptorHob(
      EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE,
      TopUseableMemAddr,
      MEM_EQU_4GB
      );
    DEBUG((EFI_D_INFO, "Just added 4G of Persistent (ADR) memory at =0x%lx\n",TopUseableMemAddr));
    TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
    TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
  }
  //
  // Create hob for remaining NV0Memory
  //
  if(TopUseableMemSize) {
    BuildResourceDescriptorHob(
      EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE,
      TopUseableMemAddr,
      TopUseableMemSize
      );
    TopUseableMemAddr = TopUseableMemAddr + TopUseableMemSize;
    DEBUG((EFI_D_INFO, "Just added 0x%lx of Persistent (ADR) memory at =0x%lx\n", TopUseableMemSize,TopUseableMemAddr));

  }

  TopUseableMemSize = normal1size;

  while (TopUseableMemSize > MEM_EQU_4GB) {
    BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
          TopUseableMemAddr,
          MEM_EQU_4GB
          );
    DEBUG((EFI_D_INFO, "Just added 4GB of System memory at = 0x%lx\n", TopUseableMemAddr));
    TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
    TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
  }
  //
  // Create hob for remain memory which above 4G memory.
  //
  if(TopUseableMemSize) {
    BuildResourceDescriptorHob (
          EFI_RESOURCE_SYSTEM_MEMORY,
          (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
          TopUseableMemAddr,
          TopUseableMemSize
          );
    TopUseableMemAddr = TopUseableMemAddr + TopUseableMemSize;
    DEBUG((EFI_D_INFO, "Just added 0x%lx of System memory at = 0x%lx\n", TopUseableMemSize,TopUseableMemAddr));
  }

  TopUseableMemSize = nv1size;

  while (TopUseableMemSize > MEM_EQU_4GB) {
    BuildResourceDescriptorHob(
      EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE,
      TopUseableMemAddr,
      MEM_EQU_4GB
      );
    DEBUG((EFI_D_INFO, "Just added 4G of Persistent (ADR) memory at =0x%lx\n",TopUseableMemAddr));
    TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
    TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
  }
  //
  // Create hob for remaining NV0Memory
  //
  if(TopUseableMemSize) {
    BuildResourceDescriptorHob (
      EFI_RESOURCE_SYSTEM_MEMORY,
      EFI_RESOURCE_ATTRIBUTE_PERSISTENT | EFI_RESOURCE_ATTRIBUTE_PERSISTABLE,
      TopUseableMemAddr,
      TopUseableMemSize
      );
    TopUseableMemAddr = TopUseableMemAddr + TopUseableMemSize;
    DEBUG((EFI_D_INFO, "Just added 0x%lx of Persistent (ADR) memory at =0x%lx\n", TopUseableMemSize,TopUseableMemAddr));
  }
#else
  if (TopUseableMemSize > 0) {
    //
    // This is a above 4G memory, give it to EFI
    // if memory hob length is above 4G length,cut and separate it.
    //
    while (TopUseableMemSize > MEM_EQU_4GB) {
      BuildResourceDescriptorHob (
                EFI_RESOURCE_SYSTEM_MEMORY,
                (
                EFI_RESOURCE_ATTRIBUTE_PRESENT |
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                ),
                TopUseableMemAddr,
                MEM_EQU_4GB
                );

      TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
      TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
    }
    //
    // Create hob for remain memory which above 4G memory.
    //
    BuildResourceDescriptorHob (
              EFI_RESOURCE_SYSTEM_MEMORY,
              (
              EFI_RESOURCE_ATTRIBUTE_PRESENT |
              EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
              EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
              ),
              TopUseableMemAddr,
              TopUseableMemSize
              );
    }
#endif
  return EFI_SUCCESS;
}

EFI_STATUS
InstallS3Memory (
  struct sysHost             *host
  )
/*++

Routine Description:

  Find memory that is reserved so PEI has some to use.

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  0 - Success
  Other - failure

--*/
{
  EFI_STATUS                  Status;
  UINTN                       BufferSize;
  UINT8                       Index;
  UINT8                       NumRanges;
  UINT8                       SmramIndex;
  UINT8                       SmramRanges;
  EFI_PHYSICAL_ADDRESS        TopUseableMemAddr;
  EFI_PHYSICAL_ADDRESS        TopUseableMemSize;
  UINTN                       S3MemoryBase;
  UINTN                       S3MemorySize;
  EFI_PEI_HOB_POINTERS        Hob;
  RESERVED_ACPI_S3_RANGE      *S3MemoryRangeData;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE MemoryMap[MAX_RANGES];
  EFI_PEI_SERVICES            **gPeiServices;
  EFI_SMRAM_DESCRIPTOR        DescriptorAcpiVariable;
  UINT8                       Socket;

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  DEBUG ((EFI_D_ERROR, "InstallS3Memory()\n"));

  //
  // Get the Memory Map and calculate TSEG place
  //
  NumRanges = MAX_RANGES;
  ZeroMem (MemoryMap, sizeof (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE) * NumRanges);
  Status = GetMemoryMap ((PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE *) MemoryMap, &NumRanges, host);
  ASSERT_EFI_ERROR (Status);

  //
  // Get how manage SMRAM desc
  //
  SmramRanges = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if  ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
          (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
          ) {
      SmramRanges++;
    }
  }
  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK);

  if (SmramRanges > 0) {
    BufferSize += ((SmramRanges - 1) * sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  Hob.Raw = BuildGuidHob (
              &gEfiSmmPeiSmramMemoryReserveGuid,
              BufferSize
                              );
  ASSERT (Hob.Raw);

  SmramHobDescriptorBlock = (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK *) (Hob.Raw);
  SmramHobDescriptorBlock->NumberOfSmmReservedRegions = SmramRanges;

  SmramIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if ((MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
        (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
        ) {
      //
      // This is an SMRAM range, create an SMRAM descriptor
      //
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalStart = MemoryMap[Index].PhysicalAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].CpuStart      = MemoryMap[Index].CpuAddress;
      SmramHobDescriptorBlock->Descriptor[SmramIndex].PhysicalSize  = MemoryMap[Index].RangeLength;
      if (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
      } else {
        SmramHobDescriptorBlock->Descriptor[SmramIndex].RegionState = EFI_SMRAM_CLOSED;
      }

      SmramIndex++;
    }
  }

  //
  // Build a HOB with the location of the reserved memory range.
  //
  CopyMem(&DescriptorAcpiVariable, &SmramHobDescriptorBlock->Descriptor[SmramRanges-1], sizeof(EFI_SMRAM_DESCRIPTOR));
  DescriptorAcpiVariable.CpuStart += RESERVED_CPU_S3_SAVE_OFFSET;

  BuildGuidDataHob (
            &gEfiAcpiVariableGuid,
            &DescriptorAcpiVariable,
            sizeof (EFI_SMRAM_DESCRIPTOR)
            );

  DEBUG((DEBUG_INFO, "Installs3Memory: After Hob build.\n"));

  //
  // Get the location and size of the S3 memory range in the reserved page and
  // install it as PEI Memory.
  //

  DEBUG ((EFI_D_ERROR, "TSEG Base = 0x%08x\n", SmramHobDescriptorBlock->Descriptor[SmramRanges-1].PhysicalStart));
  S3MemoryRangeData = (RESERVED_ACPI_S3_RANGE*)(UINTN)
    (SmramHobDescriptorBlock->Descriptor[SmramRanges-1].PhysicalStart + RESERVED_ACPI_S3_RANGE_OFFSET);

  S3MemoryBase  = (UINTN) (S3MemoryRangeData->AcpiReservedMemoryBase);
  DEBUG ((EFI_D_ERROR, "S3MemoryBase = 0x%08x\n", S3MemoryBase));
  S3MemorySize  = (UINTN) (S3MemoryRangeData->AcpiReservedMemorySize);
  DEBUG ((EFI_D_ERROR, "S3MemorySise = 0x%08x\n", S3MemorySize));

  DEBUG((DEBUG_INFO, "Installs3Memory: S3MemoryBase: %x, S3MemorySize: %x.\n", S3MemoryBase, S3MemorySize));

  //
  // Setup SMRAM in IIO
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Continue if this socket is not present
    //
    if ((host->var.common.socketPresentBitMap & (BIT0 << Socket)) == 0) continue;

    //
    // Program TSEG Base
    //
    WriteCpuPciCfgEx (host, Socket, 0, TSEG_N0_IIO_VTD_REG, host->nvram.common.TsegBase.Data);
    //
    // Program TSEG Limit
    //
    WriteCpuPciCfgEx (host, Socket, 0, TSEG_N1_IIO_VTD_REG, host->nvram.common.TsegLimit.Data);
  }

#if ME_SUPPORT_FLAG
  //
  // MESEG size requested and MESEG enabled in QPI before programming ME registers
  if ((host->var.qpi.OutMesegEn || host->var.qpi.OutIsocEn) && host->var.mem.meStolenSize) {
    //
    // Setup ME SEG
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if ((host->var.common.socketPresentBitMap & (BIT0 << Socket)) == 0) continue;

      //
      // Program ME SEG Base
      //
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N0_CBOBC_MEMSAD_REG, host->nvram.common.MeMemLowBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N1_CBOBC_MEMSAD_REG, host->nvram.common.MeMemHighBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_0_IIO_VTD_REG, host->nvram.common.MeNcMemLowBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_1_IIO_VTD_REG, host->nvram.common.MeNcMemHighBaseAddr.Data);
      //
      // Program ME SEG Limit
      //
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N0_CBOBC_MEMSAD_REG, host->nvram.common.MeMemLowLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N1_CBOBC_MEMSAD_REG, host->nvram.common.MeMemHighLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_0_IIO_VTD_REG, host->nvram.common.MeNcMemLowLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_1_IIO_VTD_REG, host->nvram.common.MeNcMemHighLimit.Data);
    }
  }
#endif // ME_SUPPORT_FLAG

  Status        = (*gPeiServices)->InstallPeiMemory (gPeiServices, S3MemoryBase, S3MemorySize);

  DEBUG((DEBUG_INFO, "Installs3Memory: After InstallPeiMemory. \n"));

  ASSERT_EFI_ERROR (Status);

  //
  // Retrieve the system memory length and build memory hob for the system
  // memory above 1MB. So Memory Callback can set cache for the system memory
  // correctly on S3 boot path, just like it does on Normal boot path.
  //
  ASSERT_EFI_ERROR ((S3MemoryRangeData->SystemMemoryLength - 0x100000) > 0);
  BuildResourceDescriptorHob (
            EFI_RESOURCE_SYSTEM_MEMORY,
            (
            EFI_RESOURCE_ATTRIBUTE_PRESENT |
            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
            ),
            0x100000,
            S3MemoryRangeData->SystemMemoryLength - 0x100000
            );

  //
  // Get the current "Top of Upper Usable Memory" address from TOUUD.
  // If TOUUD > 4G, it means memory re-mapped.
  //
  TopUseableMemSize = ((EFI_PHYSICAL_ADDRESS)host->var.mem.highMemSize) * (64 * 1024 * 1024);
  TopUseableMemAddr = ((EFI_PHYSICAL_ADDRESS)host->var.mem.highMemBase) * (64 * 1024 * 1024);

  if (TopUseableMemSize > MEM_EQU_4GB) {
    //
    // This is a above 4G memory, give it to EFI
    // if memory hob length is above 4G length,cut and separate it.
    //
    while (TopUseableMemSize > MEM_EQU_4GB) {
      BuildResourceDescriptorHob (
                EFI_RESOURCE_SYSTEM_MEMORY,
                (
                EFI_RESOURCE_ATTRIBUTE_PRESENT |
                EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
                ),
                TopUseableMemAddr,
                MEM_EQU_4GB
                );

      TopUseableMemSize = TopUseableMemSize - MEM_EQU_4GB;
      TopUseableMemAddr = TopUseableMemAddr + MEM_EQU_4GB;
    }
    //
    // Create hob for remain memory which above 4G memory.
    //
    BuildResourceDescriptorHob (
              EFI_RESOURCE_SYSTEM_MEMORY,
              (
              EFI_RESOURCE_ATTRIBUTE_PRESENT |
              EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
              EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
              EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
              ),
              TopUseableMemAddr,
              TopUseableMemSize
              );
  }

  return EFI_SUCCESS;
}

VOID
RetrieveRequiredMemorySize (
  PSYSHOST host,
  UINTN    *Size
  )
/*++

Routine Description:

  Determine the memory size desired.  Based on HOB memory information.

Arguments:

  host - pointer to sysHost structure on stack
  size - pointer for reporting memory size required

Returns:

  None

--*/
{
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT8                       Index;
  UINTN                       TempPageNum;
  EFI_PEI_SERVICES            **gPeiServices;
  EFI_HOB_GUID_TYPE           *GuidHob;

  gPeiServices  = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  *Size         = 0;
  MemoryData    = NULL;

  GuidHob     = GetFirstGuidHob (&gEfiMemoryTypeInformationGuid);
  MemoryData  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (MemoryData);

  //
  // Platform PEIM should supply such a information. Generic PEIM doesn't assume any default value
  //
  if (MemoryData == NULL) {
    return ;
  }

  TempPageNum = 0;
  for (Index = 0; MemoryData[Index].Type != EfiMaxMemoryType; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  if (TempPageNum == 0) {
    return ;
  }
  //
  // Two more pages are used by DXE memory manager
  //
  *Size = (TempPageNum + 4) * EFI_PAGE_SIZE;

  return ;
}

EFI_STATUS
GetMemoryMap (
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE                   *MemoryMap,
  UINT8                                                   *NumRanges,
  struct sysHost                                          *host
  )
/*++

Routine Description:

  This function returns the memory ranges to be enabled, along with information
  describing how the range should be used.

Arguments:

  MemoryMap: Buffer to record details of the memory ranges to be enabled.

  NumRanges: On input, this contains the maximum number of memory ranges that
             can be described in the MemoryMap buffer.

  host: pointer to sysHost structure on stack


Returns:

  The MemoryMap buffer will be filled in and NumRanges will contain the actual
  number of memory ranges that are to be anabled.

  0 - completed successfully.

--*/
{
  BOOLEAN                           EnableSmram;
  EFI_PHYSICAL_ADDRESS              MemorySize;
  EFI_PHYSICAL_ADDRESS              RowLength;
  EFI_PHYSICAL_ADDRESS              AlignedTsegBase = 0;
  EFI_PHYSICAL_ADDRESS              AlignedIedBase = 0;
  PEI_MEMORY_RANGE_SMRAM            SmramMask;
  PEI_MEMORY_RANGE_SMRAM            TsegMask;
  UINT32                            BlockNum;
  UINT8                             ExtendedMemoryIndex;
  UINT8                             Index;
  UINT8                             Socket;
  TSEG_N0_IIO_VTD_STRUCT            TsegBase;
  TSEG_N1_IIO_VTD_STRUCT            TsegLimit;
  EFI_PEI_SERVICES                  **gPeiServices;
  EFI_PLATFORM_INFO                 *PlatformInfo;
  EFI_HOB_GUID_TYPE                 *GuidHob;
  UINT32                            PfatMemSizeMB;
#if ME_SUPPORT_FLAG
  UINT32                            SMRRSize;
  UINT32                            GapForSMRRAlign;
  EFI_PHYSICAL_ADDRESS              MemorySize_gap;
  EFI_PHYSICAL_ADDRESS              AlignedTsegBase_gap;
  UINT8                             MesegIndex = 0;
  EFI_PHYSICAL_ADDRESS              AlignedMesegBase;
  MESEG_BASE_N0_CBOBC_MEMSAD_STRUCT MeMemLowBaseAddr;
  MESEG_BASE_N1_CBOBC_MEMSAD_STRUCT MeMemHighBaseAddr;
  MESEG_LIMIT_N0_CBOBC_MEMSAD_HSX_BDX_STRUCT  MeMemLowLimit; 
  MESEG_LIMIT_N1_CBOBC_MEMSAD_STRUCT  MeMemHighLimit;
  MENCMEM_BASE_0_IIO_VTD_STRUCT     MeNcMemLowBaseAddr;
  MENCMEM_BASE_1_IIO_VTD_STRUCT     MeNcMemHighBaseAddr;
  MENCMEM_LIMIT_0_IIO_VTD_STRUCT    MeNcMemLowLimit;
  MENCMEM_LIMIT_1_IIO_VTD_STRUCT    MeNcMemHighLimit;
#endif  // ME_SUPPORT_FLAG

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);


  if ((*NumRanges) < MAX_RANGES) {
    return EFI_BUFFER_TOO_SMALL;
  }

  *NumRanges = 0;

  //
  // Find out which memory ranges to reserve on this platform
  //
  SmramMask = (PlatformInfo->MemData.MemTsegSize >> 17) | PEI_MR_SMRAM_CACHEABLE_MASK | PEI_MR_SMRAM_TSEG_MASK;

  //
  //  DEBUG ((EFI_D_ERROR, " SmramMask          = 0x%X\n", SmramMask));
  //
  // Generate memory ranges for the memory map.
  //
  EnableSmram = FALSE;
  MemorySize  = 0;
  Index       = 0;

  //
  // Get the current "max usable memory" address from TOLUD because we will not
  // support any memory above 4Gig.  Will ignore the memory between 4G and TOUUD.
  //
  RowLength = (host->var.mem.lowMemBase + host->var.mem.lowMemSize) * (64 * 1024 * 1024);
  //
  // Debug code because the MRC isnt filling this data in just yet!
  //
  if (!RowLength) {
    RowLength = 512 * 1024 * 1024;
    //
    // hardcode the $%^&**$%^ to 512MB!
    //
  }
  //
  // System is very unlikely to work with less than 32MB
  //
  ASSERT_EFI_ERROR (RowLength >= (32 * 1024 * 1024));

  DEBUG((EFI_D_ERROR, "MRC: lowMemBase:%x lowMemSize:%x\n",host->var.mem.lowMemBase,host->var.mem.lowMemSize ));
  DEBUG((EFI_D_ERROR, "MRC: highMemBase:%x highMemSize:%x\n",host->var.mem.highMemBase,host->var.mem.highMemSize));


  //
  // Add memory below 640KB to the memory map. Make sure memory between
  // 640KB and 1MB are reserved, even if not used for SMRAM
  //
  MemoryMap[*NumRanges].RowNumber       = Index;
  MemoryMap[*NumRanges].PhysicalAddress = MemorySize;
  MemoryMap[*NumRanges].CpuAddress      = MemorySize;
  MemoryMap[*NumRanges].RangeLength     = 0xA0000;
  MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
  (*NumRanges)++;

  //
  // Reserve ABSEG or HSEG SMRAM if needed
  //
  if (SmramMask & (PEI_MR_SMRAM_ABSEG_MASK | PEI_MR_SMRAM_HSEG_MASK)) {
    EnableSmram = TRUE;
    MemoryMap[*NumRanges].PhysicalAddress = MC_ABSEG_HSEG_PHYSICAL_START;
    MemoryMap[*NumRanges].RangeLength     = MC_ABSEG_HSEG_LENGTH;

    if (SmramMask & PEI_MR_SMRAM_ABSEG_MASK) {
      MemoryMap[*NumRanges].CpuAddress = MC_ABSEG_CPU_START;
    } else {
      MemoryMap[*NumRanges].CpuAddress = MC_HSEG_CPU_START;
    }
    //
    // Chipset only supports cacheable SMRAM
    //
    MemoryMap[*NumRanges].Type = DualChannelDdrSmramCacheable;
  } else {
    //
    // Just mark this range reserved
    //
    MemoryMap[*NumRanges].PhysicalAddress = MC_ABSEG_HSEG_PHYSICAL_START;
    MemoryMap[*NumRanges].CpuAddress      = MC_ABSEG_CPU_START;
    MemoryMap[*NumRanges].RangeLength     = 0x60000;
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
  }

  MemoryMap[*NumRanges].RowNumber = Index;
  (*NumRanges)++;

  RowLength -= 0x100000;
  MemorySize = 0x100000;

  //
  // Add remaining memory to the memory map
  //
  if (RowLength > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = MemorySize;
    MemoryMap[*NumRanges].CpuAddress      = MemorySize;
    MemoryMap[*NumRanges].RangeLength     = RowLength;
    MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
    (*NumRanges)++;
    MemorySize += RowLength;
  }

  ExtendedMemoryIndex = (UINT8) (*NumRanges - 1);

#if ME_SUPPORT_FLAG
  //
  // See if we need to trim MESEG out of the highest memory range below 4GB
  //
  PlatformInfo->MemData.MemMESEGSize = 0;
  PlatformInfo->MemData.MemMESEGBase = 0;


  if ((host->var.qpi.OutMesegEn || host->var.qpi.OutIsocEn) && host->var.mem.meStolenSize) {
    DEBUG((EFI_D_ERROR, "MRC: meStolenSize in MB: %x\n",host->var.mem.meStolenSize));

    //
    // Remove MESEG DRAM range from the available memory map
    //
    MemoryMap[*NumRanges].RangeLength = (host->var.mem.meStolenSize * 1024 * 1024);
    MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize -= MemoryMap[*NumRanges].RangeLength;
    //
    // MESEG base requires 1MB alignment.
    // Need to adjust memory map accordingly.
    //
    AlignedMesegBase = MemorySize &~(0xFFFFF);
    MemoryMap[*NumRanges].RangeLength += (MemorySize - AlignedMesegBase);

    MemoryMap[*NumRanges].PhysicalAddress = AlignedMesegBase;
    MemoryMap[*NumRanges].CpuAddress      = AlignedMesegBase;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

    MesegIndex = (UINT8) (*NumRanges);
    PlatformInfo->MemData.MemMESEGBase = (UINT64)MemoryMap[MesegIndex].PhysicalAddress;
    PlatformInfo->MemData.MemMESEGSize = (UINT64)MemoryMap[MesegIndex].RangeLength;
    DEBUG((EFI_D_ERROR, "MRC: PhysicalAddress:%x; RangeLength:%x\n",(UINT32)MemoryMap[MesegIndex].PhysicalAddress,MemoryMap[MesegIndex].RangeLength));

    (*NumRanges)++;


  }
#endif //ME_SUPPORT_FLAG

  //
  // See if we need to trim TSEG out of the highest memory range
  //
  if (SmramMask & PEI_MR_SMRAM_TSEG_MASK) {
    //
    // Create the new range for TSEG and remove that range from the previous SdrDdrMainMemory range
    //
    TsegMask  = (SmramMask & PEI_MR_SMRAM_SIZE_MASK);

    BlockNum  = 1;
    while (TsegMask) {
      TsegMask >>= 1;
      BlockNum <<= 1;
    }

    BlockNum >>= 1;

    switch (BlockNum) {
    case PEI_MR_SMRAM_SIZE_1024K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_2048K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_8192K_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_16M_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_32M_MASK:
      break;

    case PEI_MR_SMRAM_SIZE_64M_MASK:
      break;

    default:
      //
      // Non supported size. Set to 0.
      //
      BlockNum = 0;
      break;
    }
    DEBUG((EFI_D_ERROR, "MRC: TSEG BlockNum:%d\n",BlockNum));

    if (BlockNum) {
      EnableSmram                       = TRUE;

#if ME_SUPPORT_FLAG
  //
  // See if we need to build a hob for memory gap caused by SMRR alignment
  //
  SMRRSize = (BlockNum * 128 * 1024);
  DEBUG((EFI_D_ERROR, "MRC: SMRR Size: %x\n", SMRRSize));
  MemorySize_gap = MemorySize - SMRRSize;
  AlignedTsegBase_gap = MemorySize_gap &~(SMRRSize-1);
  GapForSMRRAlign = (UINT32)(MemorySize_gap - AlignedTsegBase_gap);         // If MESeg is at top, then we possibly have SMRR alignment issue
  if( GapForSMRRAlign )
  {
    //
    // Remove SMRR alignment gap reserved range from the available memory map
    //
    MemoryMap[*NumRanges].RangeLength = GapForSMRRAlign;
    MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].PhysicalAddress = AlignedTsegBase_gap + SMRRSize;
    MemoryMap[*NumRanges].CpuAddress      = AlignedTsegBase_gap + SMRRSize;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

    DEBUG((EFI_D_ERROR, "GetMemoryMap: SMRR gap reserved base  : %08X\n", (UINT32)MemoryMap[*NumRanges].PhysicalAddress));
    DEBUG((EFI_D_ERROR, "GetMemoryMap: SMRR gap reserved length: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));
    (*NumRanges)++;
  } //if( GapForSMRRAlign )

#endif //ME_SUPPORT_FLAG

      MemoryMap[*NumRanges].RangeLength = (BlockNum * 128 * 1024);
      MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize -= MemoryMap[*NumRanges].RangeLength;

      //
      // TSEG base requires TSEG-Size alignment, done at last step.
      //
//    AlignedTsegBase = MemorySize &~(0xFFFFF);
      AlignedTsegBase = MemorySize &~(MemoryMap[*NumRanges].RangeLength-1);
      MemoryMap[*NumRanges].RangeLength += (MemorySize - AlignedTsegBase);
      MemoryMap[*NumRanges].PhysicalAddress = AlignedTsegBase;
      MemoryMap[*NumRanges].CpuAddress      = AlignedTsegBase;
      //
      // Need to reserve the IED from TSEG
      //
      MemoryMap[*NumRanges].RangeLength -= PlatformInfo->MemData.MemIedSize;
      MemorySize += PlatformInfo->MemData.MemIedSize;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
      AlignedIedBase = AlignedTsegBase + MemoryMap[*NumRanges].RangeLength;

    } // if (BlockNum)

    //
    // Chipset only supports cacheable SMRAM
    //
    MemoryMap[*NumRanges].Type = DualChannelDdrSmramCacheable;

    DEBUG((EFI_D_ERROR, "GetMemoryMap: TsegBase: %08X\n", (UINT32)AlignedTsegBase));
    DEBUG((EFI_D_ERROR, "GetMemoryMap: TsegRange: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));

    //
    // Setup SMRAM in IIO
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if ((host->var.common.socketPresentBitMap & (BIT0 << Socket)) == 0) continue;

      //
      // Program TSEG Base
      //
      TsegBase.Data = ReadCpuPciCfgEx (host, Socket, 0, TSEG_N0_IIO_VTD_REG);
      TsegBase.Bits.base = (UINT32)(MemoryMap[*NumRanges].PhysicalAddress) >> 20; // In MegaBytes
      WriteCpuPciCfgEx (host, Socket, 0, TSEG_N0_IIO_VTD_REG, TsegBase.Data);
      //
      // Program TSEG Limit
      //
      TsegLimit.Data = ReadCpuPciCfgEx (host, Socket, 0, TSEG_N1_IIO_VTD_REG);
      TsegLimit.Bits.limit = (UINT32)((MemoryMap[*NumRanges].PhysicalAddress + MemoryMap[*NumRanges].RangeLength + PlatformInfo->MemData.MemIedSize - 1)) >> 20;
      WriteCpuPciCfgEx (host, Socket, 0, TSEG_N1_IIO_VTD_REG, TsegLimit.Data);
    }
    host->nvram.common.TsegBase.Data = ReadCpuPciCfgEx (host, 0, 0, TSEG_N0_IIO_VTD_REG);
    host->nvram.common.TsegLimit.Data = ReadCpuPciCfgEx (host, 0, 0, TSEG_N1_IIO_VTD_REG);

    (*NumRanges)++;

    //
    // And make an entry just for IED RAM, if used
    //
    if (PlatformInfo->MemData.MemIedSize) {
      MemoryMap[*NumRanges].RangeLength = PlatformInfo->MemData.MemIedSize;
      MemoryMap[*NumRanges].RowNumber   = MemoryMap[ExtendedMemoryIndex].RowNumber;
      MemorySize -= MemoryMap[*NumRanges].RangeLength;

      MemoryMap[*NumRanges].PhysicalAddress = AlignedIedBase;
      MemoryMap[*NumRanges].CpuAddress      = AlignedIedBase;
      MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;

      MemoryMap[*NumRanges].Type = DualChannelDdrReservedMemory;

      DEBUG((EFI_D_ERROR, "GetMemoryMap: IedRamBase: %08X\n", (UINT32)AlignedIedBase));
      DEBUG((EFI_D_ERROR, "GetMemoryMap: IedRamRange: %08X\n", (UINT32)MemoryMap[*NumRanges].RangeLength));

      (*NumRanges)++;
    }
  }


#if ME_SUPPORT_FLAG
  //
  // MESEG size requested and MESEG enabled in QPI before programming ME registers
  if ((host->var.qpi.OutMesegEn || host->var.qpi.OutIsocEn) && host->var.mem.meStolenSize) {
    //
    // Setup ME SEG
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      //
      // Continue if this socket is not present
      //
      if ((host->var.common.socketPresentBitMap & (BIT0 << Socket)) == 0) continue;


      MeMemLowBaseAddr.Data = ReadCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N0_CBOBC_MEMSAD_REG);
      MeMemHighBaseAddr.Data = ReadCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N1_CBOBC_MEMSAD_REG);
      MeMemLowLimit.Data = ReadCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N0_CBOBC_MEMSAD_REG);
      MeMemHighLimit.Data = ReadCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N1_CBOBC_MEMSAD_REG);

      MeNcMemLowBaseAddr.Data = ReadCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_0_IIO_VTD_REG);
      MeNcMemHighBaseAddr.Data = ReadCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_1_IIO_VTD_REG);
      MeNcMemLowLimit.Data = ReadCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_0_IIO_VTD_REG);
      MeNcMemHighLimit.Data = ReadCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_1_IIO_VTD_REG);

      MeMemHighBaseAddr.Bits.mebase = 0;
      MeNcMemHighBaseAddr.Bits.addr = 0;
      MeMemHighLimit.Bits.melimit = 0;
      MeNcMemHighLimit.Bits.addr = 0;
      //
      // get ME base address from previous calculations
      //
      MeMemLowBaseAddr.Bits.mebase = (UINT32)(MemoryMap[MesegIndex].PhysicalAddress) >> 19;
      MeNcMemLowBaseAddr.Bits.addr = (UINT32)(MemoryMap[MesegIndex].PhysicalAddress) >> 19;
      //
      // get ME length from previous calculations
      //
      MeMemLowLimit.Bits.melimit = (UINT32)((MemoryMap[MesegIndex].PhysicalAddress + MemoryMap[MesegIndex].RangeLength - 1)) >> 19;
      MeMemLowLimit.Bits.en = 1;
      MeNcMemLowLimit.Bits.addr = (UINT32)((MemoryMap[MesegIndex].PhysicalAddress + MemoryMap[MesegIndex].RangeLength - 1)) >> 19;
      //
      // Program ME SEG Base
      //
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N0_CBOBC_MEMSAD_REG, MeMemLowBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_BASE_N1_CBOBC_MEMSAD_REG, MeMemHighBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_0_IIO_VTD_REG, MeNcMemLowBaseAddr.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_BASE_1_IIO_VTD_REG, MeNcMemHighBaseAddr.Data);
      //
      // Program ME SEG Limit
      //
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N0_CBOBC_MEMSAD_REG, MeMemLowLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MESEG_LIMIT_N1_CBOBC_MEMSAD_REG, MeMemHighLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_0_IIO_VTD_REG, MeNcMemLowLimit.Data);
      WriteCpuPciCfgEx (host, Socket, 0, MENCMEM_LIMIT_1_IIO_VTD_REG, MeNcMemHighLimit.Data);
    }
    host->nvram.common.MeMemLowBaseAddr.Data = ReadCpuPciCfgEx (host, 0, 0, MESEG_BASE_N0_CBOBC_MEMSAD_REG);
    host->nvram.common.MeMemHighBaseAddr.Data = ReadCpuPciCfgEx (host, 0, 0, MESEG_BASE_N1_CBOBC_MEMSAD_REG);
    host->nvram.common.MeMemLowLimit.Data = ReadCpuPciCfgEx (host, 0, 0, MESEG_LIMIT_N0_CBOBC_MEMSAD_REG);
    host->nvram.common.MeMemHighLimit.Data = ReadCpuPciCfgEx (host, 0, 0, MESEG_LIMIT_N1_CBOBC_MEMSAD_REG);

    host->nvram.common.MeNcMemLowBaseAddr.Data = ReadCpuPciCfgEx (host, 0, 0, MENCMEM_BASE_0_IIO_VTD_REG);
    host->nvram.common.MeNcMemHighBaseAddr.Data = ReadCpuPciCfgEx (host, 0, 0, MENCMEM_BASE_1_IIO_VTD_REG);
    host->nvram.common.MeNcMemLowLimit.Data = ReadCpuPciCfgEx (host, 0, 0, MENCMEM_LIMIT_0_IIO_VTD_REG);
    host->nvram.common.MeNcMemHighLimit.Data = ReadCpuPciCfgEx (host, 0, 0, MENCMEM_LIMIT_1_IIO_VTD_REG);

  }
#endif // ME_SUPPORT_FLAG

#ifdef LT_FLAG
  if (PlatformInfo->MemData.MemLtMemSize > 0) {	//Aptio V Server Override - Add if state ment
    MemorySize -= PlatformInfo->MemData.MemLtMemSize;
    MemoryMap[*NumRanges].RowNumber       = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemoryMap[*NumRanges].PhysicalAddress = MemorySize &~(0xFFFFF);
    MemoryMap[*NumRanges].CpuAddress      = MemorySize &~(0xFFFFF);
    MemoryMap[*NumRanges].RangeLength     = PlatformInfo->MemData.MemLtMemSize;
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    PlatformInfo->MemData.MemLtMemAddress = (UINT32)(MemoryMap[*NumRanges].PhysicalAddress);
    DEBUG((EFI_D_ERROR, "Reserve LT Mem[0x%08x] Size = 0x%08x\n", MemoryMap[*NumRanges].PhysicalAddress, MemoryMap[*NumRanges].RangeLength));
    (*NumRanges)++;
  }
#endif

  //
  // Reserve PFAT Memory based on the size requested if enabled
  //
  PfatMemSizeMB = PlatformInfo->PfatMemSize;

  if (PfatMemSizeMB != 0) {

    MemoryMap[*NumRanges].RangeLength           = (UINT64) LShiftU64 (PfatMemSizeMB, 20);
    MemoryMap[*NumRanges].RowNumber             = MemoryMap[ExtendedMemoryIndex].RowNumber;
    MemorySize                                 -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].PhysicalAddress       = MemorySize;
    MemoryMap[*NumRanges].CpuAddress            = MemorySize;
    MemoryMap[ExtendedMemoryIndex].RangeLength -= MemoryMap[*NumRanges].RangeLength;
    MemoryMap[*NumRanges].Type                  = DualChannelDdrReservedMemory;

    PlatformInfo->PfatPhysBase = (UINT32)MemorySize;
    DEBUG((EFI_D_ERROR, "Reserve PFAT Mem[0x%08x] Size = %x\n", MemoryMap[*NumRanges].PhysicalAddress, LShiftU64 (PfatMemSizeMB, 20)));
    (*NumRanges)++;
  }
  ASSERT_EFI_ERROR ((*NumRanges) <= MAX_RANGES);
  return EFI_SUCCESS;
}

#ifdef BDAT_SUPPORT
EFI_STATUS
SaveBdatDataToHob (
  struct sysHost      *host
  )
/*++

Routine Description:

  This function saves the Bdat Data from sysHost to the HOB for later use when it will be copied over to the ACPI table in memory.
  It also saves the BDAT Data Blob entered via the BSSA function - BiosSaveToBdat()

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  0 - Successful
  Other - Failure

--*/
{
  VOID                  *CurrentHobLocation;
  UINT32                csrReg = 0;
  UINTN                 RemainingHobSizeMemSchema, RemainingHobSizeRmtSchema;
  UINTN                 CurrentHobSize;

  RemainingHobSizeMemSchema = sizeof(BDAT_MEMORY_DATA_STRUCTURE);
  RemainingHobSizeRmtSchema = sizeof(BDAT_RMT_STRUCTURE);

  //
  // Save BDAT to Scratchpad
  //
  csrReg = (UINT32)&host->bdat;
  WriteCpuPciCfgEx (host, 0, 0, SR_BDAT_STRUCT_PTR_CSR, csrReg);

  CurrentHobLocation = &host->bdat;
  //Putting the Memory Schema into a HOB/HOBs
  while (RemainingHobSizeMemSchema) {
    if (RemainingHobSizeMemSchema >= MAX_HOB_ENTRY_SIZE) {
      CurrentHobSize = MAX_HOB_ENTRY_SIZE;
    } else {
      CurrentHobSize = RemainingHobSizeMemSchema;
    }
    //An instance of the BDAT Data HOB getting created in each loop iteration with the GUID
    BuildGuidDataHob (&gEfiMemoryMapDataHobBdatGuid, CurrentHobLocation, CurrentHobSize);

    RemainingHobSizeMemSchema -= CurrentHobSize;
    CurrentHobLocation = (VOID *)((UINT32) CurrentHobLocation + CurrentHobSize);
  }

  CurrentHobLocation = &host->bdatRmt;
  //Putting the RMT Schema into a HOB
  while (RemainingHobSizeRmtSchema) {
    if (RemainingHobSizeRmtSchema >= MAX_HOB_ENTRY_SIZE) {
      CurrentHobSize = MAX_HOB_ENTRY_SIZE;
    } else {
      CurrentHobSize = RemainingHobSizeRmtSchema;
    }
    //An instance of the BDAT Data HOB getting created in each loop iteration with the GUID
    BuildGuidDataHob (&gEfiRmtDataHobBdatGuid, CurrentHobLocation, CurrentHobSize);

    RemainingHobSizeRmtSchema -= CurrentHobSize;
    CurrentHobLocation = (VOID *)((UINT32) CurrentHobLocation + CurrentHobSize);
  }


  return EFI_SUCCESS;
}
#endif //BDAT_SUPPORT

EFI_STATUS
MrcSaveS3RestoreDataToHob (
  struct sysHost      *host
  )
/*++

Routine Description:

  This function saves the S3 Restore Data to Hob for later use.

Arguments:

  host - pointer to sysHost structure on stack

Returns:

  0 - Successful
  Other - Failure

--*/
{
  VOID                  *CurrentHob;
  UINTN                 RemainingHobSize;
  UINTN                 CurrentHobSize;
  EFI_PEI_SERVICES      **gPeiServices;

  gPeiServices = (EFI_PEI_SERVICES **) host->var.common.oemVariable;
  DEBUG((EFI_D_ERROR, "In MrcSaveS3RestoreDataToHob \n"));

  RemainingHobSize = sizeof(struct sysNvram);
  CurrentHob = &(host->nvram);

  while (RemainingHobSize) {

    if (RemainingHobSize >= MAX_HOB_ENTRY_SIZE){
      CurrentHobSize = MAX_HOB_ENTRY_SIZE;
    } else {
      CurrentHobSize = RemainingHobSize;
    }

    BuildGuidDataHob (&gEfiMemoryConfigDataHobGuid, CurrentHob, CurrentHobSize);

    RemainingHobSize -= CurrentHobSize;
    CurrentHob = (VOID *) ((UINT32) CurrentHob + CurrentHobSize);
  }

  return EFI_SUCCESS;
}


