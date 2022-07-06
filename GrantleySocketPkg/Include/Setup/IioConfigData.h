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
  IioConfigData.h
    
Abstract:
  Data format for Universal Data Structure

--*/
#include <Token.h> // AptioV server override

#if (MAX_CPU_SOCKET > 4)
#error "ERROR: The IIO_CONFIG currently supports maximum of 4 CPU/IIO. Need to port for >4 IIOs."
#endif
#define MAX_TOTAL_PORTS   44
#define TOTAL_CB3_DEVICES 32
#define IIO_UPLINK_PORT_INDEX 10   //Port 3D is the uplink port
#define MAX_TOTAL_CORE_HIDE 16  //(MAX_CPU_SOCKET * VARIABLE_FUNC3_ELEMENTS)

#define  VARIABLE_GROUP(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh0f4;x##Ioh0f5;x##Ioh0f6;x##Ioh0f7;x##Ioh0f8;x##Ioh0f9;x##Ioh0f10;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3;x##Ioh1f4;x##Ioh1f5;x##Ioh1f6;x##Ioh1f7;x##Ioh1f8;x##Ioh1f9;x##Ioh1f10
#define  VARIABLE_GROUP_ELEMENTS    11
#define  VARGROUP_IOHPP(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh0f4;x##Ioh0f5;x##Ioh0f6;x##Ioh0f7;x##Ioh0f8;x##Ioh0f9;x##Ioh0f10;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3;x##Ioh1f4;x##Ioh1f5;x##Ioh1f6;x##Ioh1f7;x##Ioh1f8;x##Ioh1f9;x##Ioh1f10
#define  VARGROUP_CBDMA(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh0f4;x##Ioh0f5;x##Ioh0f6;x##Ioh0f7;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3;x##Ioh1f4;x##Ioh1f5;x##Ioh1f6;x##Ioh1f7
#define  VARGROUP_FUNC1(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh1f0;x##Ioh1f1
#define  VARIABLE_FUNC1_ELEMENTS    2
#define  VARGROUP_FUNC3(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3
#define  VARIABLE_FUNC3_ELEMENTS    4
#define  VARGROUP_FUNC4(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh0f4;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3;x##Ioh1f4
#define  VARIABLE_FUNC4_ELEMENTS    5
#define  VARGROUP_FUNC7(x)  x##Ioh0f0;x##Ioh0f1;x##Ioh0f2;x##Ioh0f3;x##Ioh0f4;x##Ioh0f5;x##Ioh0f6;x##Ioh0f7;x##Ioh1f0;x##Ioh1f1;x##Ioh1f2;x##Ioh1f3;x##Ioh1f4;x##Ioh1f5;x##Ioh1f6;x##Ioh1f7
#define  VARIABLE_FUNC7_ELEMENTS    8
#define  VARGROUP_PERIOH(x) x##Ioh0;x##Ioh1


#define PCIEDEVFUNC2PORTINDEX(portIdx, dev, func) \
  if(dev <= 1) portIdx = (dev + func); \
  else if(dev == 2) portIdx = (dev + func + 1); \
  else if(dev == 3) portIdx = (dev + func + 4);

//typedef struct {

/*++
==================================================================================================
==================================      VTd Setup Options       ==================================
==================================================================================================
--*/
    
    UINT8                   EnableAzaliaVCpOptimizations;
    UINT8                   VTdSupport;
    UINT8                   InterruptRemap;
    UINT8                   DisableAddressDraining;
    UINT8                   CoherencySupport;
    UINT8                   IsochCoherencySupport;
    UINT8                   ATS;
    UINT8                   PassThroughDma;
/*++
==================================================================================================
==================================    TXT DPR Setup Options     ==================================
==================================================================================================
--*/
    UINT8                   VTDBarLock;
    UINT8                   DPRSize;

/*++
==================================================================================================
==================================      PCIE Setup Options       ==================================
==================================================================================================
--*/


    // Platform data needs to update these PCI Configuration settings
    UINT8    SLOTIMP[MAX_TOTAL_PORTS];          // Slot Implemented - PCIE Capabilities (D0-10 / F0 / R0x92 / B8)
    UINT16   SLOTPSP[MAX_TOTAL_PORTS];          // Physical slot Number - Slot Capabilities (D0-10 / F0 / R0xA4 / B31:19). Change to use 13 bits instead of 8 only per HSD 4168942
    UINT8    SLOTEIP[MAX_TOTAL_PORTS];          // Electromechanical Interlock Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B17)
    UINT8    SLOTSPLS[MAX_TOTAL_PORTS];         // Slot Power Limit Scale - Slot Capabilities (D0-10 / F0 / R0xA4 / B16:15)
    UINT8    SLOTSPLV[MAX_TOTAL_PORTS];         // Slot Power Limit Value - Slot Capabilities (D0-10 / F0 / R0xA4 / B14:7)
    UINT8    SLOTHPCAP[MAX_TOTAL_PORTS];        // Slot Hot Plug capable - Slot Capabilities (D0-10 / F0 / R0xA4 / B6)
    UINT8    SLOTHPSUP[MAX_TOTAL_PORTS];        // Hot Plug surprise supported - Slot Capabilities (D0-10 / F0 / R0xA4 / B5)
    UINT8    SLOTPIP[MAX_TOTAL_PORTS];          // Power Indicator Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B4)
    UINT8    SLOTAIP[MAX_TOTAL_PORTS];          // Attention Indactor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B3)
    UINT8    SLOTMRLSP[MAX_TOTAL_PORTS];        // MRL Sensor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B2)
    UINT8    SLOTPCP[MAX_TOTAL_PORTS];          // Power Controller Present - Slot Capabilities (D0-10 / F0 / R0xA4 /B1)
    UINT8    SLOTABP[MAX_TOTAL_PORTS];          // Attention Button Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B0)

    // General PCIE Configuration

    // General PCIE Configuration

    VARGROUP_PERIOH(UINT8   PcieIouCfg);        // 00-Hardware Initialized, 01-Software Note:this setup option may not be needed
                                                // becuse currently board must be strapped for software control.
                                                // BIOS will always check strap first and take user values if system
                                                // strapped to "Wait4BIOS".
    UINT8   ConfigIOU0[MAX_CPU_SOCKET];         // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P3p4p5p6)
    UINT8   ConfigIOU1[MAX_CPU_SOCKET];         // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P7p8p9p10)
    UINT8   ConfigIOU2[MAX_CPU_SOCKET];         // 00-x4x4, 01-x8 (p1,p2)
    UINT8   CompletionTimeoutGlobal;            // HSD 4165823
    UINT8   CompletionTimeoutGlobalValue;
    UINT8   CompletionTimeout[MAX_CPU_SOCKET];  // On Setup
    UINT8   CompletionTimeoutValue[MAX_CPU_SOCKET]; // On Setup
    UINT8   CoherentReadPart;
    UINT8   CoherentReadFull;
    UINT8   PcieGlobalAspm;                    // HSD 4165822
    UINT8   StopAndScream;                     // HSD 4166867
    UINT8   SnoopResponseHoldOff;              // On Setup HSD 4986531 
    UINT8   B0PciSettings;      // Apply B0 TBG Pcie settings

    // mixc PCIE configuration
    UINT8    PcieLinkDis[MAX_TOTAL_PORTS];      // On Setup
    UINT8    PcieAspm[MAX_TOTAL_PORTS];         // On Setup
    UINT8    PcieCommonClock[MAX_TOTAL_PORTS];  // On Setup
    UINT8    PcieSlotClockSource[MAX_TOTAL_PORTS];  // On Setup
    UINT8    PcieMaxPayload[MAX_TOTAL_PORTS];   // On Setup PRD
    UINT8    PcieDState[MAX_TOTAL_PORTS];       // On Setup
    UINT8    PcieL0sLatency[MAX_TOTAL_PORTS];   //On Setup
    UINT8    PcieL1Latency[MAX_TOTAL_PORTS];    //On Setup
    UINT8    MsiEn[MAX_TOTAL_PORTS];            // On Setup
    UINT8    ExtendedSync[MAX_TOTAL_PORTS];     // On Setup
    UINT8    InbandPresenceDetect[MAX_TOTAL_PORTS];   // Not implemented in code
    UINT8    PciePortDisable[MAX_TOTAL_PORTS];        // Not implemented in code
    UINT8    PciePmeIntEn[MAX_TOTAL_PORTS];           // Not implemented in code

    //
    // VPP Control
    //
    UINT8    VppEnable[MAX_TOTAL_PORTS];        // 00 -- Disable, 01 -- Enable  //no setup option defined- aj
    UINT8    VppPort[MAX_TOTAL_PORTS];          // 00 -- Port 0, 01 -- Port 1   //no setup option defined- aj
    UINT8    VppAddress[MAX_TOTAL_PORTS];       // 01-07 for SMBUS address of Vpp   //no setup option defined- aj

    //
    // PCIE setup options for Link Control2
    //
    UINT8    PciePortLinkSpeed[MAX_TOTAL_PORTS];   //on Setup
    UINT8    ComplianceMode[MAX_TOTAL_PORTS];   // On Setup  PRD
    UINT8    DeEmphasis[MAX_TOTAL_PORTS];       // On Setup

    //
    // PCIE setup options for MISCCTRLSTS
    //
    UINT8    EOI[MAX_TOTAL_PORTS];              // On Setup
    UINT8    MSIFATEN[MAX_TOTAL_PORTS];         //On Setup. 
    UINT8    MSINFATEN[MAX_TOTAL_PORTS];        //On Setup. 
    UINT8    MSICOREN[MAX_TOTAL_PORTS];         //On Setup. 
    UINT8    ACPIPMEn[MAX_TOTAL_PORTS];         //On Setup
    UINT8    DISL0STx[MAX_TOTAL_PORTS];         //On Setup
    VARIABLE_GROUP(UINT8    INBOUNDIOEn);       //On Setup
    VARIABLE_GROUP(UINT8    SIOLnkBWSts);       //On Setup
    UINT8    P2PWrtDis[MAX_TOTAL_PORTS];        //On Setup Peer 2 Peer
    UINT8    P2PRdDis[MAX_TOTAL_PORTS];         //On Setup Peer 2 peer
    VARIABLE_GROUP(UINT8    PHoldDis);          //Not On Setup
    UINT8    PMEAckTOCtl[MAX_TOTAL_PORTS];      //Not On Setup
    UINT8    DisPMETOAck[MAX_TOTAL_PORTS];      //On Setup
    VARIABLE_GROUP(UINT8    SYSErrAER);         // not On Setup NOTE: same a Malformed TLP
    UINT8    ACPIHP[MAX_TOTAL_PORTS];           //On Setup
    UINT8    ACPIPM[MAX_TOTAL_PORTS];           //On Setup
    UINT8    INBCFGEn[MAX_TOTAL_PORTS];         //not yet On Setup
    VARIABLE_GROUP(UINT8    HOSTBRDGECC);       //NOT On Setup

    //
    // PCIE PERFCTRLSTS
    //
    UINT8    PERFCSNSO;                         // bit3,2 ==> 0=snoop, 1=no snoop

    //
    // PCIE RAS (Errors)
    //

    // CCT_TODO - Verify if these are been used or update accordingly

    VARIABLE_GROUP(UINT8    MsiRasEn);          // not On Setup, commented


    //
    // PCIE  Link Training Ctrl
    //
    VARIABLE_GROUP(UINT8    UpCfgAdvtEn);       // Not defined On Setup

/*++
==================================================================================================
==================================    Crystal Beach 3 Setup Options    ===========================
==================================================================================================
--*/
      UINT8                 EnableIOAT; //AptioV server override: One Setup control added to enable/disable Crystal Beach devices
      UINT8                 EnableNoSnoop; // AptioV server override: Program DMA no snoop based on one setup control EnableNoSnoop
      UINT8                 Cb3DcaEn[MAX_CPU_SOCKET];           // on setup
      UINT8                 Cb3DmaEn[TOTAL_CB3_DEVICES];        // on setup
      UINT8                 Cb3NoSnoopEn[TOTAL_CB3_DEVICES];    // on setup
      UINT8                 SetApicIdTagMap;                    // on setup
      UINT8                 DisableTPH;
      UINT8                 RelaxedOrdering;
#ifdef DE_SKU
#endif // DE_SKU

/*++
==================================================================================================
==================================    IOH Thermal Throttling Setup Options =======================
==================================================================================================
--*/

    // ioh Thermal Throttling control options
    UINT8                   IOHTTEN;              // On Setup  IOH THERMAL THROTTLING ENABLE
    UINT8                   PWROPTEN;             // On Setup  IOH THERMAL THROTTLING ENABLE
/*++
==================================================================================================
// The following options are place holders in case we want to add setup options later

    UINT8                   TSMAXINIT;            // TSMAXINIT
    UINT8                   TSTHRCATA;            // TSTHRCATA
    UINT8                   TSTHRCATA;            // TSTHRCATA
    UINT8                   TSTHRRCSI;            // TSTHRRCSI
    UINT8                   TSTHRRPEX;            // TSTHRRPEX
    UINT8                   TSTHRLO;              // TSTHRLO
    UINT8                   TSTHRHI;              // TSTHRHI
    UINT8                   TSTHRN0BMC;           // TSTHRN0BMC
    UINT8                   CTHINT;               // CTHINT
    UINT8                   CTCTRL;               // CTHINT
    UINT8                   TSTIMER;              // TSTIMER
==================================================================================================
--*/


/*++
==================================================================================================
==================================    Performance Monitor Setup Options    =======================
==================================================================================================
--*/

/*++
==================================================================================================
==================================    DFX Setup Options                 ==========================
==================================================================================================
--*/

    UINT8     PowerdownUnusedPorts;

/*++
==================================================================================================
==================================    System Address Setup Options      ==========================
==================================================================================================
--*/


/*++
==================================================================================================
==================================    Dual IOH Setup Options            ==========================
==================================================================================================
--*/


/*++
==================================================================================================
==================================    MISC IOH Setup Options            ==========================
==================================================================================================
--*/

    // The following are for hiding each indivisual device and function
    UINT8    PEXPHIDE[MAX_TOTAL_PORTS];           // Hide any of the DMI or PCIE devices - SKT 0,1,2,3; Device 0-10 PRD
//    VARGROUP_FUNC7(UINT8    DFX1HIDE);          // Hide DFX1 Device 13, Functions 0-7
//    VARGROUP_FUNC4(UINT8    DFX2HIDE);          // Hide DFX2 Device 14, Function 0-4
    UINT8   PMONHIDE[MAX_CPU_SOCKET];             // Hide PerfMon Device 15, Function 0
//    VARGROUP_FUNC1(UINT8    CSI0HIDE);          // Hide CSI Port 0 Device 16, Function 1:0
//    VARGROUP_FUNC1(UINT8    CSI1HIDE);          // Hide CSI Port 1 Device 17, Function 1:0
//    VARGROUP_FUNC3(UINT8    HECIHIDE);          // Hide HECI Device 18, Function 0-3
    UINT8   IOAPICHIDE[MAX_CPU_SOCKET];           // Hide IOAPIC Device 19, Function 0
    UINT8   COREHIDE[MAX_TOTAL_CORE_HIDE];        // Hide Core Device 20, Function 3:0
//    VARGROUP_PERIOH(UINT8   LTHIDE);            // Hide LT Device 21, Function 0
    UINT8   CB3HIDE[TOTAL_CB3_DEVICES];           // Hide Device CB3 - Device 22, Finction 7:0
    UINT8   PCUF3Hide;                            //Hide Device PCU Device 30, Function 3
    UINT8   EN1K;                                 // Enable/Disable 1K granularity of IO for P2P bridges 0:20:0:98 bit 2
    UINT8   DualCvIoFlow;                         // Dual CV IO Flow
//    UINT8   iTPM;
    UINT8   PcieBiosTrainEnable;                  // Used as a work around for A0 PCIe
    UINT8   MultiCastEnable;                      // MultiCastEnable test enable
    UINT8   McastBaseAddrRegion;                  // McastBaseAddrRegion
    UINT8   McastIndexPosition;                   // McastIndexPosition
    UINT8   McastNumGroup;                        // McastNumGroup

//
// HSD 4168370 Add EVMode Support to the HSX EP BIOS
//
    UINT8   DFXEnable;

    //
    // To enable/disable a root port resource padding: Bus, MEM32, PMEM64, I/O
    //
    UINT8    PcieResourcePadding[MAX_TOTAL_PORTS];  //not yet on setup

    UINT8   HidePEXPMenu[MAX_TOTAL_PORTS];          // to suppress /display the PCIe port menu

/*++
==================================================================================================
==================================    NTB Related Setup Options ==========================
==================================================================================================
--*/
  UINT8   NtbPpd[MAX_CPU_SOCKET];                   //on setup option
  UINT8   NtbBarSizeOverride[MAX_CPU_SOCKET];       //on setup option
  UINT8   NtbSplitBar[MAX_CPU_SOCKET];       	    //on setup option  
  UINT8   NtbBarSizePBar23[MAX_CPU_SOCKET];         //on setup option
  UINT8   NtbBarSizePBar45[MAX_CPU_SOCKET];         //on setup option
  UINT8   NtbBarSizePBar4[MAX_CPU_SOCKET];          //on setup option
  UINT8   NtbBarSizePBar5[MAX_CPU_SOCKET];          //on setup option  
  UINT8   NtbBarSizeSBar23[MAX_CPU_SOCKET];         //on setup option
  UINT8   NtbBarSizeSBar45[MAX_CPU_SOCKET];         //on setup option
  UINT8   NtbBarSizeSBar4[MAX_CPU_SOCKET];          //on setup option
  UINT8   NtbBarSizeSBar5[MAX_CPU_SOCKET];          //on setup option  
  UINT8   NtbSBar01Prefetch[MAX_CPU_SOCKET];        //on setup option
  UINT8   NtbXlinkCtlOverride[MAX_CPU_SOCKET];      //on setup option


/*++
==================================================================================================
==================================    Gen3 Related Setup Options ==========================
==================================================================================================
--*/
  UINT8   Gen3EqMode[MAX_TOTAL_PORTS];
  UINT8   Gen3SpecMode[MAX_TOTAL_PORTS];
  UINT8   Gen3LinkupFlow[MAX_TOTAL_PORTS];
  UINT8   Gen3Phase2EqMode[MAX_TOTAL_PORTS];
  UINT8   Gen3Phase3EqMode[MAX_TOTAL_PORTS];
  UINT8   Phase2PrivTxEqOvrPreCursor[MAX_TOTAL_PORTS];
  UINT8   Phase2PrivTxEqOvrCursor[MAX_TOTAL_PORTS];
  UINT8   Phase2PrivTxEqOvrPostCursor[MAX_TOTAL_PORTS];
  UINT8   Phase3PrivTxEqOvrPreCursor[MAX_TOTAL_PORTS];
  UINT8   Phase3PrivTxEqOvrCursor[MAX_TOTAL_PORTS];
  UINT8   Phase3PrivTxEqOvrPostCursor[MAX_TOTAL_PORTS];
  UINT8   Gen3RetrainType[MAX_TOTAL_PORTS];
  UINT8   Gen3DnTxPreset[MAX_TOTAL_PORTS];
  UINT8   Gen3DnRxPreset[MAX_TOTAL_PORTS];
  UINT8   Gen3UpTxPreset[MAX_TOTAL_PORTS];

  //PCIE Global Option
  UINT8   DmiVc1;                                   //on Setup
  UINT8   DmiVcp;                                   //on Setup
  UINT8   DmiVcm;                                   //on Setup
  UINT8   WA_4167453;                                //on Setup
  UINT8   TxEq_LookupTable_WA;                      // on Setup                          
  UINT8   NoSnoopVc0Cfg;                            //on Setup
  UINT8   MaxReadCompCombSize;                      //on Setup
  UINT8   ProblematicPort;                          //on Setup
  UINT8   DmiAllocatingFlow;                        //on Setup
  UINT8   PcieAllocatingFlow;                       //on Setup
  UINT8   Gen3Phase3LoopCount;                      //on Setup
  UINT8   PcieHotPlugEnable;                        //on Setup
  UINT8   PcieAcpiHotPlugEnable;                    //on Setup 
  UINT8   PciePortToSlotMapping;                    //on Setup
  UINT8   HaltOnDmiDegraded;                        //on Setup
  UINT8   SldWaEn;                                  //on Setup
  UINT8   RxClockWA;
  UINT8   CdrHangUpDetector;                        //on Setup
  UINT8   CtocPreCalibration;                       //on Setup
  UINT8   CtocFplaPatch;                            //on Setup
  UINT8   GlobalPme2AckTOCtrl;                      //on Setup
  UINT8   ForceUnusedPortsToL1;                     //on Setup
  UINT8   ASCSelect;                                //on Setup
  UINT8   PcieSlotOprom1;                           //On Setup
  UINT8   PcieSlotOprom2;                           //On Setup
  UINT8   PcieSlotOprom3;                           //On Setup
  UINT8   PcieSlotOprom4;                           //On Setup
  UINT8   PcieSlotOprom5;                           //On Setup
  UINT8   PcieSlotOprom6;                           //On Setup
  UINT8   PcieSlotOprom7;                           //On Setup
  UINT8   PcieSlotOprom8;                           //On Setup
  UINT8   PcieSlotItemCtrl;                         //On Setup
/*++
==================================================================================================
==================================    Work Around Related Setup Options ==========================
==================================================================================================
--*/
  UINT8   WA_NoPciePortActive[MAX_CPU_SOCKET];
  UINT8   Hide_WA_4167453;                         //to supress WA_4167453 On Setup
  
//}IIO_CONFIG;


/*++
==================================================================================================
================= Equates common for Setup options (.vfr/.hfr) and source files (.c/.h) ==========
==================================================================================================
--*/

#define PCIE_ASPM_AUTO                     7
#define PCIE_ASPM_DISABLE                  0
#define PCIE_ASPM_L0S_ONLY                 1
#define PCIE_ASPM_L1_ONLY                  2
#define PCIE_ASPM_L0S_L1_BOTH              3

#define PCIE_LINK_SPEED_AUTO               0
#define PCIE_LINK_SPEED_GEN1               1
#define PCIE_LINK_SPEED_GEN2               2
#define PCIE_LINK_SPEED_GEN3               3

#ifdef DE_SKU
#define	SC_LAN_PF_AUTO						2
#define SC_LAN_PF_EN						1
#define SC_LAN_PF_DIS						0
#endif // DE_SKU
