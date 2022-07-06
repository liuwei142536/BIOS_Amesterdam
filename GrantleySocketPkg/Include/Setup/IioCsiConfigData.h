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
  IioCsiConfigData.h
    
Abstract:
  Data format for Universal Data Structure

--*/

//typedef struct {
    UINT8   QpiSetupNvVariableStartTag;  // This must be the very first one of the whole QPI Setup NV variable!

    //
    //  The following items are inherited from SSG's EDKII tree, They remain part of the BIOS setup here because they are still used in the current code.
    //  All items are actually not updated in QpiRC but used in PciHostBridge.c for resource adjustment on as needed basis that meets the requirements of io/mem aligment/length.
    //
    UINT8   IntLogicalMode;
    UINT8   ClusterCheckSampling;
    UINT8   DebugMsgLevel;

    UINT8   PortPriorityEn;
    UINT8   NonDataDeratingEn;
    UINT8   CrcMode;

    UINT8   LinkSpeed;
    UINT8   FreqSelect;
    UINT8   CsiScrambling;

    UINT8   ErrorReporting;
    UINT8   IohPowerGoodResetEn;

    UINT8   LinkL0sEn;
    UINT8   LinkL0pEn;
    UINT8   LinkL1En;

    UINT8   CsiSocketResourceAutoAdjust;
    UINT8   BusAlignment[MAX_CPU_SOCKET];
    UINT64  BusLength[MAX_CPU_SOCKET];
    UINT8   IoAlignment[MAX_CPU_SOCKET];
    UINT64  IoLength[MAX_CPU_SOCKET];
    UINT8   MmiolAlignment[MAX_CPU_SOCKET];
    UINT64  MmiolLength[MAX_CPU_SOCKET];
    //
    //  QPI_HOST_IN
    // 

    UINT8               BusRatio[MAX_CPU_SOCKET];
    UINT8               IoRatio[MAX_CPU_SOCKET];
    UINT8               MmiolRatio[MAX_CPU_SOCKET];

    UINT8               LegacyVgaSoc;       // Socket that claims the legacy VGA range; valid values are 0-3; 0 is default.
    UINT8               MmioP2pDis;         // 1 - Disable; 0 - Enable
    UINT8               IsocAzaliaVc1En;    // 1 - Enable; 0 - Disable (i.e use VCp if Azalia is enabled); 2 - Auto.
    UINT8               DebugPrintLevel;    // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
    UINT8               AltRtid2S;          // 1 - Enable; 0 - Disable;  Normally disabled, but if enabled can boost performance in some 2S environments
                                            // only affects 2S configurations (EP only)
    UINT8               ClusterOnDieEn;     // 1 - Enable; 0 - Disable (Only available in 1/2S EP)
    UINT8               MctpEn;             // 1 - Enable; 0 - Disable 
    UINT8               E2EParityEn;        // 1 - Enable; 0 - Disable 
    UINT8               EarlySnoopEn;       // 1 - Enable; 0 - Disable (Only available in 1/2S EP 2Q)
    UINT8               DegradePrecedence;  // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
    UINT8               IBPECIEn;           // 1 - Enable; 0 - Disable
    //
    // Phy/Link Layer Options
    //
    UINT8               QpiLinkSpeedMode;   // Link speed mode selection; 0 - Slow Speed; 1- Full Speed
    UINT8               QpiLinkSpeed;       // Use QPI_LINK_SPEED definition; MAX_QPI_LINK_SPEED - Auto (i.e BIOS choosen speed)
    UINT8               QpiLinkL0pEn;       // 0 - Disable (default), 1 - Enable
    UINT8               QpiLinkL1En;        // 0 - Disable, 1 - Enable (default)
    UINT8               QpiLinkL0rEn;       // 0 - Disable; 1 - Enable (default); 
    UINT8               QpiLbEn;            // 0 - Disable(default), 1 - Enable
    UINT8               IioUniphyDisable[MAX_CPU_SOCKET]; // 0 - No (default), 1 - Yes, without Memory Hot Add,  2 - Yes, with Memory Hot Add
    UINT8               QpiLinkCreditReduce;   //  0 - Use max credit (default); 1 - Use reduced credit; 2 - use per link option
    UINT8               QpiConfigTxWci;        //  One of WIDTH_Q3Q2Q1Q0 (default), WIDTH_Q1Q0, WIDTH_Q3Q2, MAX_QPI_LINK_WIDTH (to use per link option)
    UINT8               QpiAdVNACreditArbThreshold;   // 1 - default, 4 bits value, R3QPI credit merging configuration field

    UINT8               QpiCrcMode;              // 0 - 8 bit CRC 1 - 16 bit CRC Mode

    UINT8               QpiAdaptationEn;         // setup input 0: no;     1: yes
    UINT8               QpiAdaptationInParallel; // setup input 0: Serial, 1: Parallel
    UINT8               QpiAdaptationSpeed;      // Use QPI_LINK_SPEED definition; MAX_QPI_LINK_SPEED - Auto (i.e BIOS choosen speed)
    UINT8               QpiCpuSktHotPlugEn;      // 0 - Disable (default), 1 - Enable  
    UINT8               QpiCpuSktHotPlugTopology;     // 0 - 4S Topology (default), 1 - 8S Topology
    UINT8               QpiSkuMismatchCheck;          // 0 - No, 1 - Yes (default)


    #define  CSICPUPRTVARIABLE(x)       x##QpiPortDisable;x##QpiLinkCreditReduce;\
                                        x##QpiLinkSpeed;x##QpiProbeType;x##QpiConfigTxWci;

    UINT8 QpiCpuPerPortStartTag;
    CSICPUPRTVARIABLE(UINT8 Cpu0P0)
    CSICPUPRTVARIABLE(UINT8 Cpu0P1)
// Grantley platforms support only 2 QPI ports.
#if (MAX_QPI_PORTS > 2)
    CSICPUPRTVARIABLE(UINT8 Cpu0P2)
#endif
    CSICPUPRTVARIABLE(UINT8 Cpu1P0)
    CSICPUPRTVARIABLE(UINT8 Cpu1P1)
#if (MAX_QPI_PORTS > 2)
    CSICPUPRTVARIABLE(UINT8 Cpu1P2)
#endif
    CSICPUPRTVARIABLE(UINT8 Cpu2P0)
    CSICPUPRTVARIABLE(UINT8 Cpu2P1)
#if (MAX_QPI_PORTS > 2)
    CSICPUPRTVARIABLE(UINT8 Cpu2P2)
#endif
    CSICPUPRTVARIABLE(UINT8 Cpu3P0)
    CSICPUPRTVARIABLE(UINT8 Cpu3P1)
#if (MAX_QPI_PORTS > 2)
    CSICPUPRTVARIABLE(UINT8 Cpu3P2)
#endif
#if (MAX_CPU_SOCKET > 4)
    CSICPUPRTVARIABLE(UINT8 Cpu4P0)
    CSICPUPRTVARIABLE(UINT8 Cpu4P1)
    CSICPUPRTVARIABLE(UINT8 Cpu4P2)
    CSICPUPRTVARIABLE(UINT8 Cpu5P0)
    CSICPUPRTVARIABLE(UINT8 Cpu5P1)
    CSICPUPRTVARIABLE(UINT8 Cpu5P2)
    CSICPUPRTVARIABLE(UINT8 Cpu6P0)
    CSICPUPRTVARIABLE(UINT8 Cpu6P1)
    CSICPUPRTVARIABLE(UINT8 Cpu6P2)
    CSICPUPRTVARIABLE(UINT8 Cpu7P0)
    CSICPUPRTVARIABLE(UINT8 Cpu7P1)
    CSICPUPRTVARIABLE(UINT8 Cpu7P2)
#endif

    UINT8   QpiSetupNvVariableEndTag;  // This must be the last one of the whole QPI Setup NV variable!

//} CSI_CONFIG;


