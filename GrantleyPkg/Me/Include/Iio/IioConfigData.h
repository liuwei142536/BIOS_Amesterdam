//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
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
//typedef struct {

/*++
==================================================================================================
==================================      VTd Setup Options       ==================================
==================================================================================================
--*/
    
    UINT8                   VTdSupport;
    UINT8                   InterruptRemap;
    UINT8                   DisableAddressDraining;
    UINT8                   CoherencySupport;
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
    VARIABLE_GROUP(UINT8    SLOTIMP);           // Slot Implemented - PCIE Capabilities (D0-10 / F0 / R0x92 / B8)
    VARIABLE_GROUP(UINT16   SLOTPSP);           // Physical slot Number - Slot Capabilities (D0-10 / F0 / R0xA4 / B31:19). . Change to use 13 bits instead of 8 only per HSD 4168942
    VARIABLE_GROUP(UINT8    SLOTEIP);           // Electromechanical Interlock Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B17)
    VARIABLE_GROUP(UINT8    SLOTSPLS);          // Slot Power Limit Scale - Slot Capabilities (D0-10 / F0 / R0xA4 / B16:15)
    VARIABLE_GROUP(UINT8    SLOTSPLV);          // Slot Power Limit Value - Slot Capabilities (D0-10 / F0 / R0xA4 / B14:7)
    VARIABLE_GROUP(UINT8    SLOTHPCAP);         // Slot Hot Plug capable - Slot Capabilities (D0-10 / F0 / R0xA4 / B6)
    VARIABLE_GROUP(UINT8    SLOTHPSUP);         // Hot Plug surprise supported - Slot Capabilities (D0-10 / F0 / R0xA4 / B5)
    VARIABLE_GROUP(UINT8    SLOTPIP);           // Power Indicator Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B4)
    VARIABLE_GROUP(UINT8    SLOTAIP);           // Attention Indactor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B3)
    VARIABLE_GROUP(UINT8    SLOTMRLSP);         // MRL Sensor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B2)
    VARIABLE_GROUP(UINT8    SLOTPCP);           // Power Controller Present - Slot Capabilities (D0-10 / F0 / R0xA4 /B1)
    VARIABLE_GROUP(UINT8    SLOTABP);           // Attention Button Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B0)

    // General PCIE Configuration 

    // General PCIE Configuration 

    VARGROUP_PERIOH(UINT8   PcieIouCfg);        // 00-Hardware Initialized, 01-Software Note:this setup option may not be needed
                                                // becuse currently board must be strapped for software control.
                                                  // BIOS will always check strap first and take user values if system
                                                // strapped to "Wait4BIOS".                                              
    VARGROUP_PERIOH(UINT8   ConfigIOU0);        // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P3p4p5p6)
    VARGROUP_PERIOH(UINT8   ConfigIOU1);        // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P7p8p9p10)
    VARGROUP_PERIOH(UINT8   ConfigIOU2);        // 00-x2x2, 01-x4 (p1,p2)
    VARGROUP_PERIOH(UINT8    CompletionTimeout);	 // On Setup
    VARGROUP_PERIOH(UINT8    CompletionTimeoutValue); // On Setup    

	  UINT8                   B0PciSettings;      // Apply B0 TBG Pcie settings

    // misc PCIE configuration
    VARIABLE_GROUP(UINT8    PcieLinkDis);		// On Setup
    UINT8                   PcieGlobalAspm;	    // On Setup
    VARIABLE_GROUP(UINT8    PcieAspm);			// On Setup
    VARIABLE_GROUP(UINT8    PcieCommonClock);
    VARIABLE_GROUP(UINT8    PcieMaxPayload);	// On Setup PRD
    VARIABLE_GROUP(UINT8    MsiEn);				// On Setup
    VARIABLE_GROUP(UINT8    ExtendedSync);		// On Setup
    VARIABLE_GROUP(UINT8    InbandPresenceDetect);  // CCT_TODO Not currently implemented in code. On Setup
    VARIABLE_GROUP(UINT8    PciePortDis);           // On Setup. CCT_TODO Not currently implemented in code, also modifier of PcieLinkDis question
    VARIABLE_GROUP(UINT8    PciePmeIntEn);          // CCT_TODO Not currently implemented in code

	//
    // VPP Control
	//
    VARIABLE_GROUP(UINT8    VppEnable);            // 00 -- Disable, 01 -- Enable
    VARIABLE_GROUP(UINT8    VppPort);              // 00 -- Port 0, 01 -- Port 1
    VARIABLE_GROUP(UINT8    VppAddress);           // 01-07 for SMBUS address of Vpp

    //
    // PCIE setup options for Link Control2
    //
    VARIABLE_GROUP(UINT8    PcieGen3);			  // On Setup PRD
    VARIABLE_GROUP(UINT8    PcieGen2);			  // On Setup PRD
    VARIABLE_GROUP(UINT8    ComplianceMode);	  // On Setup  PRD
    VARIABLE_GROUP(UINT8    DeEmphasis);		  // On Setup

    //
    // PCIE setup options for MISCCTRLSTS
    //
    VARIABLE_GROUP(UINT8    EOI);	    // On Setup
    VARIABLE_GROUP(UINT8    MSIFATEN);  //On Setup. CCT_TODO - Change name of option, this is split into 3 overrides & repelaces LNKSTS_IDO in Seaburg
    VARIABLE_GROUP(UINT8    MSINFATEN); //On Setup. CCT_TODO - Change name of option, this is split into 3 overrides & repelaces LNKSTS_IDO in Seaburg
    VARIABLE_GROUP(UINT8    MSICOREN);  //On Setup. CCT_TODO - Change name of option, this is split into 3 overrides & repelaces LNKSTS_IDO in Seaburg
    VARIABLE_GROUP(UINT8    ACPIPMEn);	//On Setup
    VARIABLE_GROUP(UINT8    DISL0STx);	//On Setup
    VARIABLE_GROUP(UINT8    INBOUNDIOEn); //On Setup
    VARIABLE_GROUP(UINT8    SIOLnkBWSts); //On Setup
    VARIABLE_GROUP(UINT8    P2PWrtDis);	  //On Setup Peer 2 Peer
    VARIABLE_GROUP(UINT8    P2PRdDis);	  //On Setup Peer 2 peer
    VARIABLE_GROUP(UINT8    PHoldDis);	  //On Setup
    VARIABLE_GROUP(UINT8    PMEAckTOCtl); //On Setup
    VARIABLE_GROUP(UINT8    DisPMETOAck); //On Setup
    VARIABLE_GROUP(UINT8    SYSErrAER);   // On Setup NOTE: same a Malformed TLP
    VARIABLE_GROUP(UINT8    ACPIHP);	  //On Setup
    VARIABLE_GROUP(UINT8    ACPIPM);	  //On Setup
    VARIABLE_GROUP(UINT8    INBCFGEn);	  //On Setup
    VARIABLE_GROUP(UINT8    HOSTBRDGECC); //On Setup

    //
    // PCIE PERFCTRLSTS 
    //
    UINT8    PERFCSNSO;                     // bit3,2 ==> 0=snoop, 1=no snoop

    //
    // PCIE RAS (Errors)
    //
    VARIABLE_GROUP(UINT8    MsiCorr);		//On Setup
    VARIABLE_GROUP(UINT8    MsiNonFat);		//On Setup
    VARIABLE_GROUP(UINT8    PciSerrEn);		// On Setup
    VARIABLE_GROUP(UINT8    PciPerrEn);		// On Setup


    // CCT_TODO - Verify if these are been used or update accordingly 

    VARIABLE_GROUP(UINT8    MsiRasEn);		// On Setup

    //
	// PCIE  Link Training Ctrl
    //
    VARIABLE_GROUP(UINT8    UpCfgAdvtEn);  // On Setup

/*++
==================================================================================================
==================================    Crystal Beach 3 Setup Options    ===========================
==================================================================================================
--*/

//    VARGROUP_CBDMA(UINT8    Cb3DMAEn);	 // Not on Setup but referenced
//    VARGROUP_CBDMA(UINT8    Cb3DCAEn);     // on Setup Up and referenced
//    VARGROUP_CBDMA(UINT8    Cb3NOSNOOPEN); // Not on Setup but referenced 
    
    
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
    VARIABLE_GROUP(UINT8    PEXPHIDE);             // Hide any of the DMI or PCIE devices - Device 0-10 PRD
    VARGROUP_FUNC7(UINT8    DFX1HIDE);             // Hide DFX1 Device 13, Functions 0-7
    VARGROUP_FUNC4(UINT8    DFX2HIDE);             // Hide DFX2 Device 14, Function 0-4
    VARGROUP_PERIOH(UINT8   PMONHIDE);             // Hide PerfMon Device 15, Function 0
    VARGROUP_FUNC1(UINT8    CSI0HIDE);             // Hide CSI Port 0 Device 16, Function 1:0
    VARGROUP_FUNC1(UINT8    CSI1HIDE);             // Hide CSI Port 1 Device 17, Function 1:0
    VARGROUP_FUNC3(UINT8    HECIHIDE);			   // Hide HECI Device 18, Function 0-3
    VARGROUP_PERIOH(UINT8   IOAPICHIDE);           // Hide IOAPIC Device 19, Function 0
    VARGROUP_FUNC3(UINT8    COREHIDE);             // Hide Core Device 20, Function 3:0
    VARGROUP_PERIOH(UINT8   LTHIDE);               // Hide LT Device 21, Function 0
    VARGROUP_CBDMA(UINT8    CB3HIDE);              // Hide Device CB3 - Device 22, Finction 7:0
	UINT8                   EN1K;                  // Enable/Disable 1K granularity of IO for P2P bridges 0:20:0:98 bit 2
	UINT8                   iTPM;
    UINT8                   PcieBiosTrainEnable;   // Used as a work around for A0 PCIe 

    //
    // To enable/disalbe a root port resource padding: Bus, MEM32, PMEM64, I/O
    //
    VARIABLE_GROUP(UINT8    PcieResourcePadding);

/*++
==================================================================================================
==================================    Work Around Related Setup Options ==========================
==================================================================================================
--*/

//}IIO_CONFIG;


