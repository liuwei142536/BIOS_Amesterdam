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
    
Abstract:

--*/
#include <Token.h> // AptioV server override

static IIO_CONFIG IioConfigDefault;

static IIO_CONFIG IioConfigDefault = {
/*++
==================================================================================================
==================================    Defualt Options Start             ==========================
==================================================================================================
===========   Option here must match order of options defined in IohConfigData.h  ================
==================================================================================================
--*/


/*++
==================================================================================================
==================================      VTd Setup Options       ==================================
==================================================================================================
--*/

    0,     //     EnableAzaliaVCpOptimizations;
    1,     //     VTdSupport;
    0,     //     InterruptRemap;
    0,     //     DisableAddressDraining
    0,     //     CoherencySupport;
    0,     //     IsochCoherencySupport;
    1,     //     ATS;
    0,     //     PassThroughDma;

/*++
==================================================================================================
==================================    TXT DPR Setup Options     ==================================
==================================================================================================
--*/
    1,     // VTDBarLock
    3,     // TXT DPR memory setting    

/*++
==================================================================================================
==================================      PCIE & DMI Setup Options    ==============================
==================================================================================================
--*/

    // Platform Data that must be accounted for 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTIMP.p0->SLOTIMP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTIMP.p22->SLOTIMP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPSP.p0->SLOTPSP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPSP.p22->SLOTPSP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTEIP.p0->SLOTEIP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTEIP.p22->SLOTEIP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTSPLS.p0->SLOTSPLS.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTSPLS.p22->SLOTSPLS.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTSPLV.p0->SLOTSPLV.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTSPLV.p22->SLOTSPLV.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTHPCAP.p0->SLOTHPCAP.P21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTHPCAP.p22->SLOTHPCAP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTHPSUP.p0->SLOTHPSUP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTHPSUP.p22->SLOTHPSUP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPIP.p0->SLOTPIP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPIP.p22->SLOTPIP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTAIP.p0->SLOTAIP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTAIP.p22->SLOTAIP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTMRLSP.p0->SLOTMRLSP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTMRLSP.p22->SLOTMRLSP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPCP.p0->SLOTPCP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTPCP.p22->SLOTPCP.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTABP.p0->SLOTABP.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     SLOTABP.p22->SLOTABP.p43

    // Misc. PCIE Configuration
    //
    0, 0,
    0xff, 0xff, 0xff, 0xff,                                             //     ConfigIOU0
    0xff, 0xff, 0xff, 0xff,                                             //     ConfigIOU1
    0xff, 0xff, 0xff, 0xff,                                             //     ConfigIOU2
    1,                                                                  //     Completion Timeout Global
    9,                                                                  //     Completion Timeout Global Value
    0, 0, 0, 0,                                                         //     CompletionTimeout.cpu0->CompletionTimeout.cpu3
    9, 9, 9, 9,                                                         //     CompletionTimeoutValue.cpu0->CompletionTimeoutValue.cpu3
    0,                                                                  //     CoherentReadPart
    0,                                                                  //     CoherentReadFull
#ifdef  GRANGEVILLE_PO_FLAG
    0,                                                                  //     PcieGlobalAspm
#else
    2,                                                                  //     PcieGlobalAspm
#endif
    0,                                                                  //     StopAndScream
    6,                                                                  //     SnoopResponseHoldOff 
    1,                                     //     B0PciSettings
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieLinkDis.P0->PcieLinkDis.P21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieLinkDis.P22->PcieLinkDis.P43
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,   //     PcieAspm.P0->PcieAspm.P21
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,   //     PcieAspm.P22->PcieAspm.P43
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     PcieCommonClock.p0->PcieCommonClock.p21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     PcieCommonClock.p22->PcieCommonClock.p43
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     PcieSlotClockSource.p0->PcieSlotClockSource.p21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     PcieSlotClockSource.p22->PcieSlotClockSource.p43
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   //     PcieMaxPayload.P0->PcieMaxPayload.P21
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   //     PcieMaxPayload.P22->PcieMaxPayload.P43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieDState.P0->PcieDState.P21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieDState.P22->PcieDState.P43
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,   //     PcieL0SLatency.P0->PcieL0SLatency.P21
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,   //     PcieL0SLatency.P22->PcieL0SLatency.P43
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   //     PcieL1Latency.P0->PcieL1Latency.P21
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,   //     PcieL1Latency.P22->PcieL1Latency.P43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MsiEn.P0->MsiEn.P21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MsiEn.P22->MsiEn.P43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ExtendedSync.P0->ExtendedSync.P21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ExtendedSync.P22->ExtendedSync.P43
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     InbandPresenceDetect.p0->InbandPresenceDetect.p21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     InbandPresenceDetect.p22->InbandPresenceDetect.p43

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //      PciePortDisable.p0->PciePortDisable.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  //      PciePortDisable.p22->PciePortDisable.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePmeIntEn.p0->PciePmeIntEn.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePmeIntEn.p22->PciePmeIntEn.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     Ecrc.p0-> Ecrc.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     Ecrc.p22-> Ecrc.p43

    // VPP Control 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     VppEnable.p0->VppEnable.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     VppEnable.p22->VppEnable.p43
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,   //     VppPort.p0->VppPort.p21
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,   //     VppPort.p22->VppPort.p43
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6,   //     VppAddress.p0->VppAddress.p21
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6,   //     VppAddress.p22->VppAddress.p43

    // PCIE setup options for Link Control2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePortLinkSpeed.p0->PciePortLinkSpeed.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePortLinkSpeed.p22->PciePortLinkSpeed.p43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePortLinkMaxWidth.Port0->PciePortLinkMaxWidth.Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciePortLinkMaxWidth.Port22->PciePortLinkMaxWidth.Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ComplianceMode.Port0->ComplianceMode.Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ComplianceMode.Port22->ComplianceMode.Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     DeEmphasis.p0->DeEmphasis.p21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     DeEmphasis.p22->DeEmphasis1.p43

    // PCIE setup options for MISCCTRLSTS
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     FormFact     Port0->Port10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     EOI     Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     EOI     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSIFATEN     Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSIFATEN     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSINFATEN    Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSINFATEN    Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSICOREN     Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MSICOREN     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIPMEn     Port0->Port10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIPMEn     Port0->Port10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     DISL0STx     Port0->Port21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     DISL0STx     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     INBOUNDIOEn  Port0->Port10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     SIOLnkBWSts  Port0->Port10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     P2PWrtDis    Port0->Port21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     P2PWrtDis    Port22->Port43
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     P2PRdDis     Port0->Port21
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     P2PRdDis     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PHoldDis     Port0->Port10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PMEAckTOCtl  Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PMEAckTOCtl  Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     DisPMETOAck  Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     DisPMETOAck  Port22->Port43
    //1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     PME2ACKT
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     SYSErrAER    Port0->Port10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIHP       Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIHP       Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIPM       Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     ACPIPM       Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     INBCFGEn     Port0->Port21
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     INBCFGEn     Port22->Port43
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     HOSTBRDGECC  Port0->Port10

    // PCIE PERFCTRLSTS 
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PERFCSSIS0->PERFCSSIS10
    1,                                                                   //     PERFCSNSO
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PERFCSSsP0->PERFCSSSP10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PERFCSSSCTL0->PERFCSSSCTL10

     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     MsiRasEn0->MsiRasEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciBcsErrEn0->PciBcsErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PciPrspEn0->PciPrspEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     DevUnsupReqErrEn0->DevUnsupReqErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     DevFatErrEn0->DevFatErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     DevNonFatErrEn0->DevNonFatErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     DevCorrErrEn0->DevCorrErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     RootFatErrEn0->RootFatErrEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     RootNonFatEn0->RootNonFatEn10
    //0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    //     RootCorrErrEn0->RootCorrErrEn10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //     UpCfgAdvtEn0->UpCfgAdvtEn10

/*++
==================================================================================================
==================================    Crystal Beach 3 Setup Options    ===========================
==================================================================================================
--*/

    0,                                                              //AptioV server override: One Setup control added to enable/disable Crystal Beach devices
    0,                                                              // AptioV server override: Program DMA no snoop based on one setup control EnableNoSnoop
    0, 0, 0, 0, 												//     Cb3DcaEn skt 0 - 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             //     Cb3DmaEn skt 0-3, & Channel0-7 per skt
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             //     Cb3DmaEn skt 0-3, & Channel0-7 per skt
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             //     Cb3NoSnoopEn skt 0-3, & Channel0-7 per skt
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,             //     Cb3NoSnoopEn skt 0-3, & Channel0-7 per skt
    1,                                                          //     SetApicIdTagMap
    0,                                                          //     DisableTPH
    0,                                                          //     RelaxedOrdering
#ifdef DE_SKU 
#endif // DE_SKU
/*++
==================================================================================================
==================================    IOH Thermal Throttling Setup Options =======================
==================================================================================================
--*/

/*++
==================================================================================================
==================================    IOH Thermal Throttling Setup Options =======================
==================================================================================================
--*/

    // ioh Thermal Throttling control options
    0,                                  // IOH THERMAL THROTTLING ENABLE
    0,                                  // IOH POWER OPTION ENABLE
/*++
==================================================================================================
// The following options are place holders in case we want to add setup options later

                         0,             // TSMAXINIT
                         0,             // TSTHRCATA
                         0,             // TSTHRCATA
                         0,             // TSTHRRCSI
                         0,             // TSTHRRPEX
                         0,             // TSTHRLO
                         0,             // TSTHRHI
                         0,             // TSTHRN0BMC
                         0,             // CTHINT
                         0,             // CTHINT
                         0,             // TSTIMER
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

      2,                                         // PowerdownUnusedPorts
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

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    // PEXPHIDE    DMI & PXP  Hide  - Device 0 - 10, Function 0
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    // PEXPHIDE    DMI & PXP  Hide  - Device 0 - 10, Function 0
//                 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //DFX1HIDE     DFX1 Hide  - Device 13, Function 7:0
//                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   //DFX2HIDE     DFX2 Hide  - Device 14, Function 7:4
                                      1, 1, 1, 1,      //PMONHIDE     PerfMonHide  - Device 15, Function 0
//                                   0, 0, 0, 0,   //CSI0HIDE     CSI Port0 Hide  - Device 16, Function 1:0
//                                   0, 0, 0, 0,   //CSI1HIDE     CSI Port1 Hide  - Device 17, Function 1:0
//                             0, 0, 0, 0, 0, 0, 0, 0,   //HECIHIDE     HECI Hide  - Device 18, Function 3:0
                                      0, 0, 0, 0,   //IOAPICHIDE     IOAPIC Hide  - Device 19, Function 0
                             0, 0, 0, 0, 0, 0, 0, 0,   //COREHIDE     Core Hide  - Device 20, Function 3:0 of skt 0,1
                             0, 0, 0, 0, 0, 0, 0, 0,   //COREHIDE     Core Hide  - Device 20, Function 3:0 of skt 2,3
//                                      0, 0,   //LTHIDE     LT Hide  - Device 21, Function 0
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // CB3HIDE    Crystal Beach 3 Hide  - Device 22, Function 7:0 of skt 0,1
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // CB3HIDE    Crystal Beach 3 Hide  - Device 22, Function 7:0 of skt 2,3
                                      0,    //    PCUF3Hide
                                      0,    //    EN1K
                                      1,    //    DualCvIoFlow
//                                    0,    // iTPM
                                      1,    //    PcieBiosTrainEnable to determine BIOS should initiate IOH PCIe training
                                      0,    // MultiCastEnable test enable
                                      0,    // McastBaseAddrRegion
                                      0xC,  // McastIndexPosition
                                      0,    // McastNumGroup
                                            // 

//
// HSD 4168370 Add EVMode Support to the HSX EP BIOS
//
                                      0,    // DFXEnable;

 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieResourcePadding.p0->PcieResourcePadding.p21
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   //     PcieResourcePadding.p22->PcieResourcePadding.p43
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    // HidePEXPMenu    Suppress the PCIe Port Setup menu for skt 0 & 1
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    // HidePEXPMenu    Suppress the PCIe Port Setup menu for skt 2 & 3

/*++
==================================================================================================
==================================    NTB Related Setup Options ==========================
==================================================================================================
--*/
        0,0,0,0,              //NtbPpd
	0,0,0,0,	      //NtbSplitBar
        0,0,0,0,              //NtbBarSizeOverride
        0xC,0xC,0xC,0xC,      //NtbBarSizePBar23
        0xC,0xC,0xC,0xC,      //NtbBarSizePBar45
        0xC,0xC,0xC,0xC,      //NtbBarSizeSBar23
        0xC,0xC,0xC,0xC,      //NtbBarSizeSBar45
        0,0,0,0,              //NtbSBar01Prefetch
        2,2,2,2,              //NtbXlinkCtlOverride


/*++
==================================================================================================
==================================    Gen3 Related Setup Options ==========================
==================================================================================================
--*/
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3EqMode.P0->Gen3EqMode.P21
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3EqMode.P22->Gen3EqMode.P43
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3SpecMode.P0->Gen3SpecMode.P21
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3SpecMode.P22->Gen3SpecMode.P43
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3LinkupFlow.p0->Gen3LinkupFlow.p21
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3LinkupFlow.p22->Gen3LinkupFlow.p43
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3Phase2EqMode.P0->Gen3Phase2EqMode.P10 (GEN3_EQ_MODE_HW_ADAPTIVE)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     //     Gen3Phase2EqMode.P22->Gen3Phase2EqMode.P43 (GEN3_EQ_MODE_HW_ADAPTIVE)
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,     //     Gen3Phase3EqMode.p0->Gen3Phase3EqMode.p21 (GEN3_EQ_MODE_MAXBOOST_HW)
        11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,     //     Gen3Phase3EqMode.p22->Gen3Phase3EqMode.p43 (GEN3_EQ_MODE_MAXBOOST_HW)
        0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA,   //     Phase2PrivTxEqOvrPreCursor.p0->Phase2PrivTxEqOvrPreCursor.p21 (IIO_OPTION_P2_PRECURSOR_DEFAULT)
        0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA,   //     Phase2PrivTxEqOvrPreCursor.p22->Phase2PrivTxEqOvrPreCursor.p43 (IIO_OPTION_P2_PRECURSOR_DEFAULT)
        0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,     //     Phase2PrivTxEqOvrCursor0->Phase2PrivTxEqOvrCursor.p21 (IIO_OPTION_P2_CURSOR_DEFAULT)
        0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,     //     Phase2PrivTxEqOvrCursor.p22->Phase2PrivTxEqOvrCursor.p43 (IIO_OPTION_P2_CURSOR_DEFAULT)
        0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,   //     Phase2PrivTxEqOvrPostCursor0->Phase2PrivTxEqOvrPostCursor.p21 (IIO_OPTION_P2_POSTCURSOR_DEFAULT)
        0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,   //     Phase2PrivTxEqOvrPostCursor.p22->Phase2PrivTxEqOvrPostCursor.p43 (IIO_OPTION_P2_POSTCURSOR_DEFAULT)
        0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA,  //     Phase3PrivTxEqOvrPreCursor0->Phase3PrivTxEqOvrPreCursor.p21 (IIO_OPTION_P3_PRECURSOR_DEFAULT)
        0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0x0, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA,  //     Phase3PrivTxEqOvrPreCursor0->Phase3PrivTxEqOvrPreCursor.p43 (IIO_OPTION_P3_PRECURSOR_DEFAULT)
        0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,     //     Phase3PrivTxEqOvrCursor.p0->Phase3PrivTxEqOvrCursor.p21 (IIO_OPTION_P3_CURSOR_DEFAULT)
        0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x00, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,     //     Phase3PrivTxEqOvrCursor.p22->Phase3PrivTxEqOvrCursor.p43 (IIO_OPTION_P3_CURSOR_DEFAULT)
        0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,  //     Phase3PrivTxEqOvrPostCursor.p0->Phase3PrivTxEqOvrPostCursor.p21 (IIO_OPTION_P3_POSTCURSOR_DEFAULT)
        0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0x0, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB,  //     Phase3PrivTxEqOvrPostCursor.p21->Phase3PrivTxEqOvrPostCursor.p43 (IIO_OPTION_P3_POSTCURSOR_DEFAULT)
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,     //     Gen3RetrainType.p0->Gen3RetrainType.p21 (GEN3_RETRAIN_TYPE_SBR)
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,     //     Gen3RetrainType.p22->Gen3RetrainType.p43 (GEN3_RETRAIN_TYPE_SBR)
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3DnTxPreset.p0->Gen3DnTxPreset.p21
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3DnTxPreset.p22->Gen3DnTxPreset.p43
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3DnRxPreset.p0->Gen3DnRxPreset.p21
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3DnRxPreset.p22->Gen3DnRxPreset.p43
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3UpTxPreset0->Gen3UpTxPreset10
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,     //     Gen3UpTxPreset0->Gen3UpTxPreset10
                
    //PCIE Global Option
   1,                     // DmiVc1 Setup
   1,                     //  DmiVcp on Setup
   1,                     //  DmiVcm on Setup
   0,					  //  WA_4167453 on setup
   1,                     //  TxEq_LookupTable_WA on setup
   0,                     //  NoSnoopVc0Cfg on Setup
   0,                     //  MaxReadCompCombSize
   0,                     //  ProblematicPort setup                      
   1,                     //  DmiAllocatingFlow on Setup
   1,                     //  PcieAllocatingFlow on Setup
   2,                     //  Gen3Phase3LoopCount Setup
   1,                     //  PcieHotPlugEnable on Setup
   0,                     //  PcieAcpiHotPlugEnable on Setup 
   0,                     //  PciePortToSlotMapping on Setup
   0,                     //  HaltOnDmiDegraded on Setup
   0,                     //  SldWaEn on Setup
   0,                     //  HSX HSD 4166557: RxClockWA
   1,                     //  CdrHangUpDetector on Setup
   1,                     //  CtocPreCalibration on Setup
   1,                     //  CtocFplaPatch on Setup
   0,                     //  GlobalPme2AckTOCtrl on Setup
   0,                     //  ForceUnusedPortsToL1 on Setup
   0,                     //  ASCSelect on Setup
   0,                    // PcieSlotOprom1
   0,                    // PcieSlotOprom2
   0,                    // PcieSlotOprom3
   0,                    // PcieSlotOprom4
   0,                    // PcieSlotOprom5
   0,                    // PcieSlotOprom6
   0,                    // PcieSlotOprom7
   0,                    // PcieSlotOprom8
   0,                    // PcieSlotItemCtrl
/*++
==================================================================================================
==================================    Work Around Related Setup Options ==========================
==================================================================================================
--*/
  1, 1, 1, 1,             // WA_NoPciePortActive[4]  on setup
  0,                      // Hide_WA_4167453 on Setup
#ifdef DE_SKU
/*++
==================================================================================================
==================================    uServer IP specific Setup Options    ===========================
==================================================================================================
--*/
    0,                    //    Disable_SC_Cb3Dma
    0,                    //    Disable_SC_GbE
    2,                    //    SC_GbE_PF0
    2,                     //    SC_GbE_PF1
#endif // DE_SKU
    0,                     //    PCIe_Enable_SSD_SUPPORT
/*++
==================================================================================================
==================================    Platform specific   ========================================
==================================================================================================
--*/ 
   0, 0, 0, 0              //   VPP_INVERT[MAX_CPU_SOCKET] 
/*++
==================================================================================================
==================================    Defualt Options End              ===========================
==================================================================================================
=========================== There should be no more options past this point  =====================
==================================================================================================
--*/
};
