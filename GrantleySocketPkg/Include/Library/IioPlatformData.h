//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioPlatformData.h

Abstract:

  This file provides required platform data structure for IOH.

--*/

#ifndef _IIO_PLATFORM_DATA_H_
#define _IIO_PLATFORM_DATA_H_

#include <Iio/IioRegs.h>
#include <Iio/PciExpress.h>
#include  "IioConfig.h"
#ifndef IA32
#ifndef RC_SIM
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/CpuCsrAccess.h>
#define IIO_CPU_CSR_ACCESS  EFI_CPU_CSR_ACCESS_PROTOCOL
#endif //RC_SIM
#endif // IA32


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define PCIEDEVFUNC2PORTINDEX(portIdx, dev, func) \
  if(dev <= 1) portIdx = (dev + func); \
  else if(dev == 2) portIdx = (dev + func + 1); \
  else if(dev == 3) portIdx = (dev + func + 4);

#define IIO_HANDLE          VOID * 
#define IIO_STATUS          UINT32

//
//Local macro definitions which would be platform specific
//
#define PCH_PCIE_ROOT_PORT_DEVICE       28
#define PCH_PCIE_ROOT_PORT_LSTS_REG     0x52

//AptioV server override: To avoid redefinition error from PlatformInfo.h
#ifndef PCH_RCBA_ADDRESS
#define PCH_RCBA_ADDRESS                0xFED1C000
#endif

#define PCH_LPC_PCI_DEVICE_NUMBER         31
#define PCH_LPC_PCI_FUNCTION_NUMBER       0
#define R_PCH_LPC_RCBA                  0xF0
#define R_PCH_DMI_LCTL2                 0x21B0
#define R_PCH_DMI_LSTS                  0x21AA
#define R_PCH_LPC_ULKMC                 0x94

#define PCH_PCIE_VIRTUAL_PORT_DEVICE    17
#define PCH_PCIE_VIRTUAL_PORT_FUNCTION  0
#define PCH_VIRTUAL_PLKCTL              0xe8

#define PCH_STEPPING_ID_B0                    4
#define PCH_STEPPING_ID_C0                    5

#define PCIE_MARGINING_BREAKPOINT   0x5AC0

#define MCU_WAIT_DELAY              50000
#define SMB_RST_DELAY               1000
#define OC_DONE_TIMEOUT                     10      //us
#define LBC_READ_DELAY                      50      //us
#define GEN3_LINK_POLL_GRANULARITY          100     //us
#define UNIPHY_RECIPE_DELAY                 180     //us
#define VPP_WAIT_DELAY                      500     //us
#define GEN3_STALL_SBR                      2000    //us
#define IIO_LINK_TRAIN_TIME_NO_G3EQ         30000   //us  (30ms)
#define IIO_LINK_TRAIN_TIME_MARGIN_NO_G3EQ  100000  //us  (100ms)
#define UNIPHY_DIS_DELAY                    100000  //us  (100ms)
#define WAIT_FOR_DOWNSTREAM_DEV             100000  //us  (100ms)
#define IIO_LINK_TRAIN_TIME_G3EQ            60000   //us  (60ms)
#define IIO_LINK_TRAIN_TIME_MARGIN_G3EQ     200000  //us  (200ms)
#define GEN3_LINK_POLL_TIMEOUT              10000   //us
#define LINK_INIT_POLL_TIMEOUT              10000   //us
#define SLTCAP_DELAY                        200000  //us
#define GEN3_LINK_POLL_TIMEOUT_MID          300000  //us
#define LINK_TRAINING_DELAY                 400000  //us
#define GEN3_LINK_POLL_TIMEOUT_RECOVERY     2000000 //us
#define SLOT_POWER_DELAY                    1000    //us  (1ms)

#define NONE_GEN3_PCIE_PORT                 0

#define GEN3_SUCCESS                        0
#define GEN3_FAILURE                        1

//
// Bifurcation control register shorthand
//
#define IIO_BIFURCATE_AUTO      0xFF
//
// Ports 1A-1B
//
#define IIO_BIFURCATE_x4x4      0
#define IIO_BIFURCATE_xxx8      1

// Ports 2A-2D, 3A-3D
//
#define IIO_BIFURCATE_x4x4x4x4  0
#define IIO_BIFURCATE_x4x4xxx8  1
#define IIO_BIFURCATE_xxx8x4x4  2
#define IIO_BIFURCATE_xxx8xxx8  3
#define IIO_BIFURCATE_xxxxxx16  4

#ifdef  DE_SKU
#define MAX_CBDMA_CONTROLLERS 2   //max CBDMA devices per socket supported
#else
#define MAX_CBDMA_CONTROLLERS 1   //max CBDMA devices per socket supported
#endif

#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)

typedef struct {
  UINT8   Register;
  UINT8   Function;
  UINT8   Device;
  UINT8   Bus;
  UINT32  ExtendedRegister;
} PCI_ROOT_BRIDGE_PCI_ADDRESS;

typedef enum {
  IIOInitPhase1 = 1,
  IIOInitPhase2 = 2,
  IIOInitPhase3 = 4,
} IIO_INIT_PHASE;

typedef enum {  
  IioBeforeBifurcation,            // Point before IOU Bi-fucuation and link training, no generic inbound access at this point
  IioAfterBifurcation,             // Point immediately after IOU bifurcation and link training but before any PCIe root port initialization
  IioPortEnumeration,              // Point before Port initialization, no generic inbound access at this point
  IioPortEnumProgramMISCCTRL,      // Inside IioPortInit.PcieSlotInit
  IioEnumEnd,
  IioVtDPreEn,
  IioVtDInit,
  IioVtDEn,                        // At this point it has been decided to enable VtD through setup IioVtdInit.VtdInitialization
  IioPostInitEnd                   // this is the last stage of IIO PCIe port init
} IIO_INIT_ENUMERATION;

typedef struct {
  UINT32 Device;
  UINT32 Function;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} PCI_OP_STRUCT;

typedef struct {
  UINT32 Instance;
  UINT8  RegSize;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} CSR_ACCESS_OP_STRUCT;

//
// 4986112:ESCALATE from hexaii_hsx_mock:C0 BIOS issues for isoc/ME
//
typedef struct {
  UINT8 Isoc;
  UINT8 Meseg;
  UINT8 Vc1;
  UINT8 Vcp;
  UINT8 Vcm;
  UINT8 Vc1_pri_en;
  UINT8 Vcp_pri_en;
  UINT8 Isoc_Enable;
} ISOC_VC_TABLE_STRUCT;

#ifdef DE_SKU
//
// SC input structure
//
typedef struct {
  UINT8  Disable_SC_Cb3Dma;
  UINT8  Disable_SC_GbE;
  UINT8  SC_GbE_PF0;
  UINT8  SC_GbE_PF1;
  UINT8  DisableSCUart;
  UINT8  SC_Gbe_Status;
  UINT8  Disable_SC_CPM;
  UINT8  Disable_SC_GbE1;
  UINT8  SC_GbE1_PF0;
  UINT8  SC_GbE1_PF1;
  UINT8  DE_NS_SocketSkuType;
  UINT8  SC_Gbe_Status1;
}SC_SETUP_INPUT;
#endif  //DE_SKU

static const CHAR16* IioPortLabel[NUMBER_PORTS_PER_SOCKET + 1] = { L"0", L"1a", L"1b", L"2a", L"2b", L"2c", L"2d", L"3a", L"3b", L"3c", L"3d", L"N/A"};
#define IIO_PORT_LABEL(x)   ( ((x) < NUMBER_PORTS_PER_SOCKET) ? (IioPortLabel[(x)]) : IioPortLabel[NUMBER_PORTS_PER_SOCKET] )

#pragma pack(1)

/*
 * Following are the data structure defined to support multiple CBDMA types on a system
 */
typedef struct{
  UINT8 PciBusNo;       //PCI bus number
  UINT8 PciDevNo;       //PCI device number
  UINT8 PciFunNo;       //PCI function number
}DMA_ADDRESS;

typedef struct{
  UINT8         DmaControllerPresent;   //indicates whether the DMA node controller exist
  UINT8         MaxDmaChannels;         //indicates max number of channels for this DMA node controller
  DMA_ADDRESS   DmaStartAddress;        //indicates the start PCI address of this controller
}DMA_DEVICE_INFO;

typedef struct{
  UINT32 DcaSupported : 1;
  UINT32 NoSnoopSupported : 1;
  UINT32 RelaxOrderSupported : 1;
}CB_CONFIG_CAPABILITY;

typedef struct{
  UINT8                   CB_VER;
  UINT8                   BusNo;
  UINT8                   DevNo;
  UINT8                   FunNo;
  UINT8                   MaxNoChannels;
  CB_CONFIG_CAPABILITY    CBConfigCap;
  UINT8                   DevCfgRegister;
  UINT8                   DevConRegister;
}CBDMA_CONTROLLER;

typedef struct{
  UINT8      TotalCbDmaDevicePerPackage;
  CBDMA_CONTROLLER  CbDmaDevice[MAX_CBDMA_CONTROLLERS];
}DMA_HOST;

typedef struct{
  UINT8         TotalDmaControllers;
  DMA_ADDRESS   DmaAddress[MAX_CBDMA_CONTROLLERS];
}DMANODE;
// <<<< end of CBDMA data structures >>>>

typedef struct {
    UINT8       CpuType;
    UINT8       CpuStepping;
    UINT32      CpuFamily;
    UINT8       CpuSubType;	
    UINT8       SkuType;	
    UINT8       IsocEnable;
    UINT8       EVMode;
    UINT8       MeSegEn;
    UINT8       DmiVc[MaxDmiVcType]; 
    UINT8       DmiVcId[MaxDmiVcType]; 
    UINT8       DmiTc[MaxDmiTcType]; 
    UINT8       LegacyVgaSoc;
    UINT8       Serr;
    UINT8       Perr;
    UINT8       LockChipset;
    UINT8       DevPresIoApicIio0;
    UINT8       DevPresIoApicIio1;
    UINT8       DevPresIoApicIio2;
    UINT8       DevPresIoApicIio3;
    UINT8       PlatformType;
    UINT8       IOxAPICCallbackBootEvent;
    UINT8       RasOperation;
    UINT8       SocketUnderOnline;
    UINT8       CompletedReadyToBootEventServices;
    UINT8       SocketPresent[MaxIIO];
    UINT8       SocketBaseBusNumber[MaxIIO];
    UINT8       SocketUncoreBusNumber[MaxIIO];
    UINT32      PchIoApicBase;
    UINT32      PciResourceMem32Limit[MaxIIO];
    UINT32      PciResourceMem32Base[MaxIIO];
    UINT32      VtdBarAddress[MaxIIO];
    UINT32      RcbaAddress[MaxIIO];
    UINT32      IoApicBase[MaxIIO];
    UINT64      PciExpressBase;
    UINT32      PmBase;
    UINT32      PchRcbaAddress;
    UINT8       PcieRiser1Type;
    UINT8       PcieRiser2Type;
    UINT8       MctpEn;
    UINT8       WheaSupport;
    UINT8       UnlockMsr ; // Aptio V Server Overrride : MSR 0x2e7 needs to be cleared for the RAS tool (Windows/Linux version)
    DMANODE     DmaDevice[MaxIIO];
    UINT8       IioErrorEn;
    UINT16      GpioBase;
    UINT8       IioLinkTrainPhase;
    UINT8       CompletePostPciBusNumberAssignServices;
    UINT8       CompletePostPciResourceAllocationServices;
} IIO_V_DATA;

typedef struct {
    UINT8       Device;
    UINT8       Function;
} IIO_PORT_INFO;

typedef struct {
    UINT8                     Valid;
    UINT8                     IioUplinkPortIndex;   //defines platform specific uplink port index (if any else FF)
    IIO_PORT_INFO             UplinkPortInfo;
}IIO_UPLINK_PORT_INFO;

typedef struct _INTEL_IIO_PORT_INFO {
    UINT8   Device;
    UINT8   Function;
    UINT8   DfxDevice;
    UINT8   DfxFunction;
    UINT8   DfxClusterDevice;
    UINT8   DfxClusterFunction;
    UINT8   DfxReutLinkSel;
    UINT8   SuperClusterPort;
} INTEL_IIO_PORT_INFO;

typedef struct _INTEL_DMI_PCIE_INFO {
    INTEL_IIO_PORT_INFO         PortInfo[NUMBER_PORTS_PER_SOCKET];
} INTEL_DMI_PCIE_INFO;

typedef struct _INTEL_IIO_PRELINK_DATA {
    INTEL_DMI_PCIE_INFO         PcieInfo;
    IIO_UPLINK_PORT_INFO        UplinkInfo[MaxIIO]; 
} INTEL_IIO_PRELINK_DATA;

typedef struct {
  UINT8       PciePortPresent[MaxIIO*NUMBER_PORTS_PER_SOCKET];    
  UINT8       PciePortConfig[MaxIIO*NUMBER_PORTS_PER_SOCKET];     
  UINT8       CurrentPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET]; 
  UINT8       MaxPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET]; 
  UINT8       LinkedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET]; 
  UINT8       SpeedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET]; 
  UINT8       LaneReversedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET]; 
  UINT8       PciePortMaxWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegSpeed[MaxIIO*NUMBER_PORTS_PER_SOCKET];
#ifndef IA32
#ifndef RC_SIM
  IIO_CPU_CSR_ACCESS    *CpuCsrAccess;
#endif //RC_SIM
#else
  VOID        *host;
#endif // IA32
  UINT64      McastRsvdMemory;   
  DMA_HOST    DMAhost[MaxIIO];
} IIO_OUT_DATA;

//
// Additional structures for use with Grangeville / GrantleyRefresh
// to preserve offsets in the main setup variable.
//

//
// We cannot include IioConfigData.h where this
// is defined because of the non-standard layout of 
// that file, so redefine MAX_TOTAL_PORTS here.
//

#define MAX_TOTAL_PORTS   44

typedef struct _BDX_IIO_CONFIG {

  UINT8 Ecrc[MAX_TOTAL_PORTS];
  UINT8 PciePortLinkMaxWidth[MAX_TOTAL_PORTS]; // On Setup
  UINT8 Iou0Np_Pf[MAX_CPU_SOCKET];             // On setup WA  4988093
  UINT8 Iou1Np_Pf[MAX_CPU_SOCKET];             // On setup 
  UINT8 Iou2Np_Pf[MAX_CPU_SOCKET];             // On setup

#ifdef DE_SKU

  UINT8 Disable_SC_Cb3Dma;                   // Disable the complete SC CB3 DMA device
  UINT8 Disable_SC_GbE;                      // Disable the complete SC GbE device
  UINT8 SC_GbE_PF0;                          // Force Enable / Disable SC GbE physical function 0
  UINT8 SC_GbE_PF1;                          // Force Enable / Disable SC GbE physical function 1
#endif // DE_SKU

  UINT8 PCIe_Enable_SSD_SUPPORT;
  UINT8 VPP_INVERT[MAX_CPU_SOCKET];
  UINT8 IioLinkTrainPhase;
  UINT8 NtbPpdAutoConfig[MAX_CPU_SOCKET];
  UINT8 NtbXlinkCtlAutoConfig[MAX_CPU_SOCKET];
#ifndef DE_SKU
  UINT8 Apply_s328_cbdma_WA;
#else  //DE_SKU
  UINT8 Disable_SC_CPM;                       // Disable the complete SC CPM device
  UINT8 Disable_SC_GbE1;                      // Disable the complete SC GbE1 device
  UINT8 SC_GbE1_PF0;                          // Force Enable / Disable SC GbE1 physical function 0
  UINT8 SC_GbE1_PF1;                          // Force Enable / Disable SC GbE1 physical function 1
#endif 
} BDX_IIO_CONFIG;

typedef struct _BDX_MEMORY_CONFIG {
  UINT8                   cmdVrefCenter;
  UINT8                   trainrxctle;
  UINT8                   lrdimmbacksideVref;

#ifdef SSA_FLAG
  UINT8                   EnableBiosSsaLoader;
#endif

  UINT8                   C2FMode;
  UINT8                   MemDownConfig;

#if !defined(DE_SKU) || defined(SV_HOOKS)
  UINT8                   WrCRC;
#endif

  UINT8                   ceccWaChMask;

  UINT8                   EnCECCWA;
  UINT8                   CapErrFlow;
  UINT8                   WrCrcErrFlow;
} BDX_MEMORY_CONFIG;

typedef struct {
  IIO_CONFIG             SetupData;
  BDX_IIO_CONFIG         BdxSetupData;
  IIO_V_DATA             IioVData;
  INTEL_IIO_PRELINK_DATA PreLinkData;
#ifndef RC_SIM
  IIO_OUT_DATA           IioOutData;
#endif //RC_SIM
} IIO_GLOBALS;

typedef struct _IIO_CTLEP {
    UINT8   Iio;
    UINT8   DfxDevice;
    UINT8   DfxFunction;
    UINT32  AndValueCtle_0;
    UINT32  AndValueCtle_1;
    UINT32  OrValueCtle_0;
    UINT32  OrValueCtle_1;
    UINT8   PlatformType;
} IIO_CTLEP;

#pragma pack()

static INTEL_DMI_PCIE_INFO PcieInfoDefaults = {

    // PortInfo[NUMBER_PORTS_PER_SOCKET];
    //
    // Do not reorder these... entry #0, #1, #3, & #7 need to be the bifurcatable root ports
    //
    {
        // DMI/PCIE 0PcieInfoDefaults
        { PCIE_PORT_0_DEV, PCIE_PORT_0_FUNC, PCIE_PORT_0_DFX_DEV, PCIE_PORT_0_DFX_FUNC, PCIE_PORT_0_DFX_DEV, PCIE_PORT_0_DFX_FUNC, PCIE_PORT_0_LINK_SEL, PCIE_PORT_0_SUPER_CLUSTER_PORT },

        //IOU2
        { PCIE_PORT_1_DEV, PCIE_PORT_1A_FUNC, PCIE_PORT_1A_DFX_DEV, PCIE_PORT_1A_DFX_FUNC, PCIE_PORT_1A_DFX_DEV, PCIE_PORT_1A_DFX_FUNC, PCIE_PORT_1A_LINK_SEL, PCIE_PORT_1A_SUPER_CLUSTER_PORT },
        { PCIE_PORT_1_DEV, PCIE_PORT_1B_FUNC, PCIE_PORT_1B_DFX_DEV, PCIE_PORT_1B_DFX_FUNC, PCIE_PORT_1A_DFX_DEV, PCIE_PORT_1A_DFX_FUNC, PCIE_PORT_1B_LINK_SEL, PCIE_PORT_1B_SUPER_CLUSTER_PORT },

        //IOU0
        { PCIE_PORT_2_DEV, PCIE_PORT_2A_FUNC, PCIE_PORT_2A_DFX_DEV, PCIE_PORT_2A_DFX_FUNC, PCIE_PORT_2A_DFX_DEV, PCIE_PORT_2A_DFX_FUNC, PCIE_PORT_2A_LINK_SEL, PCIE_PORT_2A_SUPER_CLUSTER_PORT },
        { PCIE_PORT_2_DEV, PCIE_PORT_2B_FUNC, PCIE_PORT_2B_DFX_DEV, PCIE_PORT_2B_DFX_FUNC, PCIE_PORT_2A_DFX_DEV, PCIE_PORT_2A_DFX_FUNC, PCIE_PORT_2B_LINK_SEL, PCIE_PORT_2B_SUPER_CLUSTER_PORT },
        { PCIE_PORT_2_DEV, PCIE_PORT_2C_FUNC, PCIE_PORT_2C_DFX_DEV, PCIE_PORT_2C_DFX_FUNC, PCIE_PORT_2A_DFX_DEV, PCIE_PORT_2A_DFX_FUNC, PCIE_PORT_2C_LINK_SEL, PCIE_PORT_2C_SUPER_CLUSTER_PORT },
        { PCIE_PORT_2_DEV, PCIE_PORT_2D_FUNC, PCIE_PORT_2D_DFX_DEV, PCIE_PORT_2D_DFX_FUNC, PCIE_PORT_2A_DFX_DEV, PCIE_PORT_2A_DFX_FUNC, PCIE_PORT_2D_LINK_SEL, PCIE_PORT_2D_SUPER_CLUSTER_PORT },

        //IOU1
        { PCIE_PORT_3_DEV, PCIE_PORT_3A_FUNC, PCIE_PORT_3A_DFX_DEV, PCIE_PORT_3A_DFX_FUNC, PCIE_PORT_3A_DFX_DEV, PCIE_PORT_3A_DFX_FUNC, PCIE_PORT_3A_LINK_SEL, PCIE_PORT_3A_SUPER_CLUSTER_PORT },
        { PCIE_PORT_3_DEV, PCIE_PORT_3B_FUNC, PCIE_PORT_3B_DFX_DEV, PCIE_PORT_3B_DFX_FUNC, PCIE_PORT_3A_DFX_DEV, PCIE_PORT_3A_DFX_FUNC, PCIE_PORT_3B_LINK_SEL, PCIE_PORT_3B_SUPER_CLUSTER_PORT },
        { PCIE_PORT_3_DEV, PCIE_PORT_3C_FUNC, PCIE_PORT_3C_DFX_DEV, PCIE_PORT_3C_DFX_FUNC, PCIE_PORT_3A_DFX_DEV, PCIE_PORT_3A_DFX_FUNC, PCIE_PORT_3C_LINK_SEL, PCIE_PORT_3C_SUPER_CLUSTER_PORT },
        { PCIE_PORT_3_DEV, PCIE_PORT_3D_FUNC, PCIE_PORT_3D_DFX_DEV, PCIE_PORT_3D_DFX_FUNC, PCIE_PORT_3A_DFX_DEV, PCIE_PORT_3A_DFX_FUNC, PCIE_PORT_3D_LINK_SEL, PCIE_PORT_3D_SUPER_CLUSTER_PORT }
    }
};


/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif //__cplusplus

#endif
