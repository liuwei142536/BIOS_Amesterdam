//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IioUniveralData.h
    
Abstract:
  Data format for Universal Data Structure

--*/

#ifndef _IIO_UNIVERSAL_DATA_
#define _IIO_UNIVERSAL_DATA_

#define IIO_UNIVERSAL_DATA_GUID { 0x7FF396A1, 0xEE7D, 0x431E, 0xBA, 0x53, 0x8F, 0xCA, 0x12, 0x7C, 0x44, 0xC0 }
#include <Token.h> // AptioV server override
#include <Iio/IioRegs.h>
#include "UncoreCommonIncludes.h"
#include "SysHost.h"
#include "IioConfig.h"

#define MAX_CPU_PORTS  4
#define MAX_IIO_PORTS  4


//--------------------------------------------------------------------------------------//
// Structure definitions for Universal Data Store (UDS)
//--------------------------------------------------------------------------------------//
#define UINT64  unsigned long long

#pragma pack(1)
typedef struct {
    #include    "IioCommonRcConfigData.h"
} COMMONRC_CONFIG;

typedef struct {
    #include    "IioCsiConfigData.h"
} CSI_CONFIG;

typedef struct {
    #include    "IioMemoryConfigData.h"
} MEMORY_CONFIG;

typedef struct {
    #include    "IioMiscConfigData.h"
} MISC_CONFIG;

typedef struct {
  UINT8                     Valid;         // TRUE, if the link is valid (i.e reached normal operation)
  UINT8                     PeerSocId;     // Socket ID
  UINT8                     PeerSocType;   // Socket Type (0 - CPU; 1 - IIO)
  UINT8                     PeerPort;      // Port of the peer socket
}QPI_PEER_DATA;

typedef struct {
  UINT8                     Valid;
  UINT8                     BusNum;
  UINT32                    SocId;
  QPI_PEER_DATA             PeerInfo[MAX_CPU_PORTS];    // QPI LEP info
} QPI_CPU_DATA;

typedef struct {
  UINT8                     Valid;
  UINT8                     BusNum;
  UINT8                     SocId;
  QPI_PEER_DATA             PeerInfo[MAX_IIO_PORTS];    // QPI LEP info
} QPI_IIO_DATA;

typedef struct {
    IIO_PORT_INFO           PortInfo[NUMBER_PORTS_PER_SOCKET];
} IIO_DMI_PCIE_INFO;

typedef struct _MEMORY_CHANNELLIST {
  UINT8 chEnabled;
  UINT8 chFailed;
  UINT8 spareDimm;
  UINT8 spareLogicalRank;
  UINT8 spareInUse;
  UINT8 oldLogicalRank;
  UINT8 devTag[8];
  UINT32 leakyBucket2ndCounter;
  UINT32 rankErrCountInfo[4];
} MEMORY_CHANNELLIST;

typedef struct _MEM_INFO {
  UINT8 enabled;
  MEMORY_CHANNELLIST channelList[MAX_CH];
} MEM_INFO;

typedef struct{
    UINT8                   Valid; 
    UINT8                   SocketID;            // Socket ID of the IIO (0..3)
    UINT8                   BusBase;
    UINT8                   BusLimit;
    UINT16                  PciResourceIoBase;
    UINT16                  PciResourceIoLimit;
    UINT32                  IioCfgBase;
    UINT32                  IioCfgLimit;
    UINT32                  IoApicBase;
    UINT32                  IoApicLimit;
    UINT32                  PciResourceMem32Base;
    UINT32                  PciResourceMem32Limit;
    UINT64                  PciResourceMem64Base;
    UINT64                  PciResourceMem64Limit;
    UINT32                  RcbaAddress;
    UINT32                  VtdBarAddress;
    IIO_DMI_PCIE_INFO       PcieInfo;
    UINT8                   DmaDeviceCount;
    DMA_DEVICE_INFO         DmaDeviceInfo[MAX_CBDMA_CONTROLLERS];
} IIO_RESOURCE_INSTANCE;

typedef struct {
    UINT8                   PfSbspId;  // Socket ID of SBSP
    UINT8                   PfGBusBase;  // Global IIO Bus base
    UINT8                   PfGBusLimit;  // Global IIO Bus limit
    UINT16                  PfGIoBase;  // Global IIO IO base
    UINT16                  PfGIoLimit;  // Global IIO IO limit
    UINT32                  PfGMmiolBase;  // Global Mmiol base
    UINT32                  PfGMmiolLimit;  // Global Mmiol limit
    UINT64                  PfGMmiohBase;  // Global Mmioh Base [43:0]
    UINT64                  PfGMmiohLimit;  // Global Mmioh Limit [43:0]
    QPI_CPU_DATA            CpuQpiInfo[MAX_SOCKET]; // QPI related info per CPU
    QPI_IIO_DATA            IioQpiInfo[MAX_SOCKET]; // QPI related info per IIO
    UINT32                  MemMaxTolm;
    UINT32                  MemTsegSize;
    UINT32                  MemIedSize;
    UINT64                  MemMir0;
    UINT64                  MemMir1;
    UINT32                  Eye1;
    UINT64                  PciExpressBase;
    UINT32                  PciExpressSize;
    UINT32                  Eye2; 
    UINT8                   Pci64BitResourceAllocation;
    IIO_RESOURCE_INSTANCE   IIO_resource[MAX_SOCKET];
    UINT8                   numofIIO;
    UINT8                   MaxBusNumber;
    UINT32                  packageBspApicID[MAX_SOCKET]; // This data array is valid only for SBSP, not for non-SBSP CPUs. <AS> for CpuSv
    UINT8                   TotHa[MAX_SOCKET];             // Total HAs; All CPUs must have same number of HA
    MEM_INFO                MemInfo[MAX_MEM_NODE];
    UINT8                   MaxCoreToBusRatio[MAX_SOCKET]; // Package Max Non-turbo Ratio (per socket).
    UINT8                   MinCoreToBusRatio[MAX_SOCKET]; // Package Maximum Efficiency Ratio (per socket).
    
} PLATFORM_DATA;

typedef struct {
    UINT8                   ResetType;          // Pending Reset Type required by system
    UINT8                   CurrentCsiLinkSpeed;// Current programmed CSI Link speed (Slow/Full speed mode)
    UINT8                   CurrentCsiLinkFrequency;// Current requested CSI Link frequency (in GT)
    UINT8                   MinCsiFreqSupported;// Minimum Csi frequency supported by the set of CSI nodes within the system
    UINT8                   MaxCsiFreqSupported;// Maximum Csi frequency supported by the set of CSI nodes within the system
    UINT8                   ForceExitValidation;// "Get out of jail card" wildcard for hard hang validation functions 
    UINT8                   IsocEnable;
    UINT8                   MeSegEn;
    UINT8                   MinimumCpuStepping;
    UINT8                   MinimumIioStepping;
    UINT8                   LtEnable;
    UINT8                   LtsxEnable;
    UINT8                   MctpEn;
    UINT8                   cpuType;
    UINT8                   cpuSubType;
    UINT32                  cpuFamily;              // Bit[19:4] of cpuid(1)_eax.  CPU_FAMILY_HSX, CPU_FAMILY_BDX, CPU_FAMILY_IVT
    UINT8                   skuType;
    UINT8                   numCpus;                // 1,..4. Total number of CPU packages installed and detected (1..4)by QPI RC
    UINT32                  socketPresentBitMap;    // bitmap of sockets with CPUs present detected by QPI RC
    UINT8                   nodePresentBitMap;      // bitmap of sockets with CPUs present detected by QPI RC
    UINT16                  tolmLimit;
    UINT32                  tohmLimit;
    UINT32                  mmCfgBase;
    UINT32                  RcVersion;
    UINT8                   DDRXOVERMODE;           // DDR 2.2 Mode

    // For RAS
    UINT8                   bootMode;
    UINT8                   RASmode[MAX_MEM_NODE];
    UINT8                   RASmodeEx[MAX_MEM_NODE];
    UINT32                  options;    
    UINT16                  spareErrTh;    
    
    UINT8                   CpuOnlineStatus; // BitN == 1 means CpuN is present in system and not offlined by jumper.
    UINT8                   CpuSpareMask;    // BitN == 1 means CpuN is present in system but offlined by a jumper.
    UINT8                   IioOnlineStatus; // BitN == 0 means Iio(N+1) is offlined by jumper.
    UINT16                  MemBoardPresent; // BitN == 1 means MemboardN is present in system.
    
    UINT8                   OutClusterOnDieEn; // Whether RC enabled COD support
    UINT8                   IioLinkTrainPhase;   // Indicates that IIO Link trainig has been Done in PEI
    
    UINT8                   DE_NS_SKU_ID;      // NS SKU Identification. 0: DE, 1: NS Storage Sku, 2: NS Comms Sku  

} SYSTEM_STATUS;
 
//
/*  Due to the constraints of VFRCOMPILE that will not allow for structures within the setup data, the layout
    of the IIO_SETUP_INFO structure must be a byte-4-byte match to the IioSetupInfoData.h file!
*/
typedef struct {
    UINT8                   VariableTag;         // do not move this tag!!
    IIO_CONFIG              Config;
    COMMONRC_CONFIG         CommonRcConfig;
    CSI_CONFIG              CsiConfig;
    MEMORY_CONFIG           MemoryConfig;
    MISC_CONFIG             MiscConfig;
} SETUP_INFO;

typedef struct {
    SETUP_INFO              SetupInfo;
    PLATFORM_DATA           PlatformData;
    SYSTEM_STATUS           SystemStatus;
    UINT32                  OemValue;
} IIO_UDS;
#pragma pack()

//
// Value that should reside in Revision ID register for CPU for each stepping
//
#define A0_CPU_STEPPING         0x00
#define B0_CPU_STEPPING         0x10        
#define INVALID_CPU_STEPPING    0xFF

//
// Value that should reside in Revision ID register for IIO for each stepping
//
#define A0_IOH_STEPPING         0x00
#define B0_IOH_STEPPING         0x10
#define B1_IOH_STEPPING         0x02
#define B2_IOH_STEPPING         0x12
#define B3_IOH_STEPPING         0x13
#define C0_IOH_STEPPING         0x20
#define C1_IOH_STEPPING         0x21
#define C2_IOH_STEPPING         0x22
#define INVALID_IOH_STEPPING    0xFF


// VALUE definitions for IIO_UDS.SystemStatus.CurrentCsiLinkSpeed
#define UDS_FREQ_5_600_GT		0
#define UDS_FREQ_6_400_GT       1
#define UDS_FREQ_7_200_GT       2
#define UDS_FREQ_8_000_GT       3
#define UDS_FREQ_8_800_GT		4
#define UDS_FREQ_9_600_GT		5

#define V_DO_NOTHING                        0x00
#define V_CSI0_TX_RX                        0x01
#define V_CSI1_TX_RX                        0x02
#define V_CSI0_TX_CSI1_RX                   0x03
#define V_CSI0_RX_CSI1_TX                   0x04

#endif
