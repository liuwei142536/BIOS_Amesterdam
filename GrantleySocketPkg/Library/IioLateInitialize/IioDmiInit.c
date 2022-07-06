/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
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

  IioDmiInit.c

Abstract:

  This file initialize the DMI device on IIO

--*/
#include "IioLateInitialize.h"

VOID DmiLinkReTrain (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
  IIO_GLOBALS                                    *IioGlobalData
   )
{
  UINT8  DmiPortNegWidth;
  UINT8  DmiPortNegSpeed;
  UINT16 Var16;
#ifdef SDP_FLAG
  UINT16 DmiPortMaxWidth; // HSD 4987368 SDP: PCI-E Ports Link did not train and error
#endif
  
  UINT32 RegLocation;
  LNKSTS_IIO_PCIEDMI_STRUCT LnkSts;

  DEBUG((EFI_D_INFO, "DMI Link Retrain()\n"));

  //
  // Assumption: the DMI link speed control bits are already set to desired value, thus
  // this routine will only take care of DMI link retrain and status
  //

  do {
      Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKSTS));
      Var16 &= B_DMI_PORT_PXPLNKSTS_LINKTRAIN;
  } while (Var16);
  
  //
  // Now order the link to retrain.
  //
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKCTRL));
  Var16 |= B_DMI_PORT_PXPLNKCTRL_RETRAIN_LINK;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKCTRL), Var16);

  //
  // retrain again.
  //
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKCTRL));
  Var16 |= B_DMI_PORT_PXPLNKCTRL_RETRAIN_LINK;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKCTRL), Var16);

  //
  // Clear Error
  //
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKSTS));
  Var16 |= B_DMI_PORT_PXPLNKSTS_LABS | B_DMI_PORT_PXPLNKSTS_LBMS;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKSTS), Var16);

  do {
      Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKSTS));
      Var16 &= B_DMI_PORT_PXPLNKSTS_LINKTRAIN;
  } while (Var16);

  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_DMI_PORT_PXPLNKSTS));
  Var16 &= B_DMI_PORT_PXPLNKSTS_LNKSPD;
  if ( Var16 == 2) {
    DEBUG((EFI_D_INFO, "DMI speed is 5Gb/s (Gen2)\n"));
  } else {
    DEBUG((EFI_D_INFO, "DMI speed is 2.5Gb/s (Gen1)\n"));
  }

  //
  // Determine Negotiated link width and speed for DMI port.
  //
  RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG);
  LnkSts.Data =  PciExpressRead16(PCI_PCIE_ADDR(  PciAddress.Bus,   // (LnkSts) = get LNKSTS mode DMI register data
                                                  PciAddress.Device,
                                                  PciAddress.Function,
                                                  RegLocation));
  if (LnkSts.Bits.data_link_layer_link_active){                  // is this link active?
    DmiPortNegWidth = (UINT8)LnkSts.Bits.negotiated_link_width;  // if so, update Negotiated width value.
    DmiPortNegSpeed = (UINT8)LnkSts.Bits.current_link_speed;     // update Negotiated speed value.
#ifdef SDP_FLAG  // HSD 4987368 SDP: PCI-E Ports Link did not train and error
    DmiPortMaxWidth =   PciExpressRead16(PCI_PCIE_ADDR ( DMI_BUS_NUM, 
                                                         DMI_DEV_NUM, 
                                                         DMI_FUNC_NUM, 
                                                         ONLY_REGISTER_OFFSET(LNKCAP_IIO_PCIEDMI_REG))); 
    DmiPortMaxWidth &= B_PCIE_PORT_PXPLNKSTS_NLNKWD;
    DmiPortMaxWidth >>= 4; 
#endif                                                   
  }
  else{
    DmiPortNegWidth = 0;                                         // if not, clear negotiated width value.
    DmiPortNegSpeed = 0;                                         // Clear negotiated speed value.
#ifdef SDP_FLAG  
    DmiPortMaxWidth = 0; // HSD 4987368 SDP: PCI-E Ports Link did not train and error
#endif
  }

  IioGlobalData->IioOutData.LinkedPXPMap[0] = DmiPortNegWidth;
  IioGlobalData->IioOutData.SpeedPXPMap[0] = DmiPortNegSpeed;
#ifdef SDP_FLAG
  IioGlobalData->IioOutData.MaxPXPMap [0]  = (UINT8) DmiPortMaxWidth; // HSD 4987368 SDP: PCI-E Ports Link did not train and error
#endif
  //
  // Degraded DMI link width is not POR. So error and halt if DMI negotiated width is reduced!
  //
  if (DmiPortNegWidth != 0x04) {
    DEBUG((EFI_D_ERROR, "DMI is in a degraded state!  Should be 0x4 width, but is currently running at 0x%x!", DmiPortNegWidth));
    //fgarciac HSD 3614405: BIOS PBG CPV requires a setup option to avoid BIOS hanging on DMI not training to x4 Gen2
    if(IioGlobalData->SetupData.HaltOnDmiDegraded) {  // Is "halt on DMI degraded" setup option  enabled?
      ASSERT(FALSE);                              // if so, halt.
    }
  }

  //
  // Degraded DMI link speed is not POR. So error and halt if DMI link speed is reduced!
  //
  if (IioGlobalData->SetupData.PciePortLinkSpeed[0] != PCIE_LINK_SPEED_AUTO){ //ignore if it is auto
    if (DmiPortNegSpeed != IioGlobalData->SetupData.PciePortLinkSpeed[0]) {
      DEBUG((EFI_D_ERROR, "DMI is in a degraded state!  Link Status current_link_speed field should %x, but is currently %x!", IioGlobalData->SetupData.PciePortLinkSpeed[0], DmiPortNegSpeed));
      //fgarciac HSD 3614405: BIOS PBG CPV requires a setup option to avoid BIOS hanging on DMI not training to x4 Gen2
      if(IioGlobalData->SetupData.HaltOnDmiDegraded) {  // Is "halt on DMI degraded" setup option  enabled?
        ASSERT(FALSE);                              // if so, halt.
      }
    }
  }
}

VOID
DmiLinkInit (
    IN PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    IN IIO_GLOBALS                                    *IioGlobalData
)
/*++

Routine Description:

  DMI link specific initialization

Arguments:

   IioGlobalData    Pointer to the Internal Data Storage for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
  LNKCAP_IIO_PCIEDMI_STRUCT   DmiLnkCap;
  LNKCON2_IIO_PCIEDMI_STRUCT  DmiLnkCon2;
  LNKCON_IIO_PCIEDMI_STRUCT   DmiLnkCon;
  UINT16                      LnkSts;
  UINT8                       PortWidth;
  UINT8                       PortSpeed;

  if(OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    return;

  //
  // Setup the DMI port Link capability features (which have RW-O attribute)
  //
  DmiLnkCap.Data = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,
                                     PciAddress.Function,ONLY_REGISTER_OFFSET(LNKCAP_IIO_PCIEDMI_REG)));
  //
  // L1 exit Latecy (17:15)
  //
  DmiLnkCap.Bits.l1_exit_latency = IioGlobalData->SetupData.PcieL1Latency[0];
  //
  // L0s exit Latecy (14:12)
  //
#ifdef BDX_HOST
  if ((IioGlobalData->IioVData.CpuType != CPU_HSX) && (IioGlobalData->IioVData.CpuType != CPU_BDX)) {
#else
  if ((IioGlobalData->IioVData.CpuType != CPU_HSX)) {
#endif
    DmiLnkCap.Bits.l0s_exit_latency = IioGlobalData->SetupData.PcieL0sLatency[0];
  }
  //
  // ASPM(11:10): 00=disabled, 10=L1 only, 01 = L0s entry support, 11=Both L1 & L0s support
  //
  // 4168647: BIOS to enable ASPM L1 on DMI and PCIe ports by default
  // 4987909: Incorrect register settings in IioDmiInit.c
  //
  DmiLnkCap.Data = DmiLnkCap.Data & ~(B_PCIE_PORT_PXPLNKCAP_ASPMCTRL);                     // clear ASPM bits 11:10

//AptioV Server override - Start: Set ASPM capabilities based on setup option
// DmiLnkCap.Data |= B_PCIE_PORT_PXPLNKCAP_ASPMCTRL_L1; // Set ASPM L1 cap by default.

  if (IioGlobalData->SetupData.PcieGlobalAspm) {
    DmiLnkCap.Bits.active_state_link_pm_support = IioGlobalData->SetupData.PcieGlobalAspm;
  } else {
    if(IioGlobalData->SetupData.PcieAspm[0] != PCIE_ASPM_DISABLE)  //if auto then PCIE_ASPM_L1_ONLY
      DmiLnkCap.Bits.active_state_link_pm_support = PCIE_ASPM_L1_ONLY;
  } 
//AptioV Server override - End: Set ASPM capabilities based on setup option

  //
  // MaxLnkWidth (9:4)
  //
  // If Override Max Link Width was not specified in MaxPXPMap, then copy CurrentPXPMap (bifurcated max width) to MaxPXPMap (override max width)
  if(IioGlobalData->IioOutData.MaxPXPMap[0] == 0)
    IioGlobalData->IioOutData.MaxPXPMap[0] = IioGlobalData->IioOutData.CurrentPXPMap[0];

  DmiLnkCap.Bits.maximum_link_width = IioGlobalData->IioOutData.MaxPXPMap[0];
  //
  // ivt_eco s4677507: PCIE - Mismatch in advertising link capability between
  //                   lnkcap and lnkcap2 can cause legacy hardware(prior to 3.0
  //                   base PCIE spec) to train only to Gen1.
  //  [cloned to s4031727] - applicable to IVX, HSX, BDX

  DmiLnkCap.Bits.maxlnkspd = V_PCIE_PORT_PXPLNKCAP_LS_GEN2;

  S3_PciExpressWrite32 (IioGlobalData,
		  	  	  	  	PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,ONLY_REGISTER_OFFSET(LNKCAP_IIO_PCIEDMI_REG)),
		  	  	  	  	DmiLnkCap.Data);

  //
  // Program Link Control 2
  //
  DmiLnkCon2.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,
                                                    ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIEDMI_REG)));

  //
  // Set Compliance Mode (bit 4)
  //
  DmiLnkCon2.Bits.enter_compliance = IioGlobalData->SetupData.ComplianceMode[0];
  //
  // Program De-Emphasis select (bit 6)
  //
  DmiLnkCon2.Bits.selectable_de_emphasis = IioGlobalData->SetupData.DeEmphasis[0];

  //
  // Target link speed
  //
  // Assumption: PCH DMI is always program to Gen2 by PCH driver, therefore set the DMI to Gen2 as default,
  // if user knob indicates Gen1, than first program the PCH DMI to Gen1 than CPU root complex to Gen1
  //
  // HSD 4166587: PCIE: DMI - BIOS not setting DMI to train to GEN2
  //              Set LNKCON2 to support DMI GEN2
  //
  DmiLnkCon2.Bits.target_link_speed = V_DMI_PORT_PXPLNKCTRL2_LS_GEN2;
  if(IioGlobalData->SetupData.PciePortLinkSpeed[0] == V_DMI_PORT_PXPLNKCTRL2_LS_GEN1){
    if(OemSetPchDmiLinkSpeed(PciAddress, IioGlobalData, V_DMI_PORT_PXPLNKCTRL2_LS_GEN1) == TRUE)
      DmiLnkCon2.Bits.target_link_speed = V_DMI_PORT_PXPLNKCTRL2_LS_GEN1;
  }

  S3_PciExpressWrite16 (IioGlobalData,
		  	  	  	  	PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,ONLY_REGISTER_OFFSET(LNKCON2_IIO_PCIEDMI_REG)),
		  	  	  	  	DmiLnkCon2.Data);

  //
  // Retrain the DMI link to 2.0
  //
  DmiLinkReTrain(PciAddress, IioGlobalData);

  LnkSts = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
		  	  	  	  	  	  ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG)));

  // Gather and store data about linked ports.
  PortSpeed = (UINT8)(LnkSts & B_DMI_PORT_PXPLNKSTS_LNKSPD);
  PortWidth = (UINT8)((LnkSts & B_DMI_PORT_PXPLNKSTS_NLNKWD) >> 4) ;

  // Copy the port width and the port speed to LinkedPXPmap and SpeedPXPMap
  IioGlobalData->IioOutData.LinkedPXPMap[0] = PortWidth;
  IioGlobalData->IioOutData.SpeedPXPMap[0]  = PortSpeed;

  //
  // Program DMI other link control features
  //
  DmiLnkCon.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,
                                      ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIEDMI_REG)));
  //
  // ivt_eco: s4570709: PUSH from jaketown: Disable PCIe L0s with unCore patch
  //  [cloned to s4030134] - WA is to remove the L0s from the knob defn., L1 as capability will be indicated clearly in B0 spec
  //
  // HSD 4165822:PCIE: Global PCIE ASPM option needed 
  //             Code added to check a global setup option first to see if the user just wants all the ports
  //             configured for L1.  If this option is set, then all ports get the L1 setting, but if the option is
  //             DISABLED, then the individual port setting uses the older setup option.
  //

  if (IioGlobalData->SetupData.PcieGlobalAspm) {
    DmiLnkCon.Bits.active_state_link_pm_control = IioGlobalData->SetupData.PcieGlobalAspm;
  } else {
    DmiLnkCon.Bits.active_state_link_pm_control = CheckVarPortASPMLate(IioGlobalData, 0, 0);
  }

  //
  // Program Extended Sync
  //
  DmiLnkCon.Bits.extended_synch = IioGlobalData->SetupData.ExtendedSync[0];

  S3_PciExpressWrite16 (IioGlobalData,
		  	  	  	  	PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIEDMI_REG)),
		  	  	  	  	DmiLnkCon.Data);

}

VOID
DmiPreInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
  IIO_GLOBALS                                    *IioGlobalData
)
/*++

Routine Description:

  DMI initialization before Bus number is/are assigned

Arguments:

   IioGlobalData           Pointer to the Internal Data Storage (UDS) for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
  UINT8 					Var8;
  UINT16 					Var16;
  INTPIN_IIO_PCIE_STRUCT    IntPinReg;
  UINT64					ExtCtrl;
  ERRCAP_IIO_PCIE_STRUCT    Errcap;

  //
  // SA BSU 0.8.11 
  // #3614765, #3247235: devcap2.ari_en default is "1" for DMI port and it
  //                     should be set to 0 by BIOS
  // This is a permanent workaround
  //
  Var8 = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_PCIE_PORT_PXPDEVCAP2));
  Var8 &= ~B_PCIE_PORT_ARI_EN;
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_PCIE_PORT_PXPDEVCAP2), Var8);

  //
  // Disable interrupts on legacy host bridge (Legacy DMI).
  // 
  Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_PCIE_PORT_PCICMD));
  Var16 |= B_PCIE_PORT_PCICMD_INTDIS;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,R_PCIE_PORT_PCICMD), Var16);

  // write back read value to lock RW-O INTPIN for legacy socket DMI
  IntPinReg.Data = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(INTPIN_IIO_PCIE_REG)));
  S3_PciExpressWrite8(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(INTPIN_IIO_PCIE_REG)),IntPinReg.Data);
          

  //
  // Program DMI Clock configuration (if it is not yet done earlier)
  //
  InitPortClock(IioGlobalData, PciAddress,
                IioGlobalData->SetupData.PcieSlotClockSource[0],
                IioGlobalData->SetupData.PcieCommonClock[0]);


  // Program MISCCTRL
  // This was moved here because there are RWO registers that must be programmed.
  //
  ExtCtrl = ((UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS+4)) << 0x20);
  ExtCtrl |= (UINT64)PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS));

  ExtCtrl &= ~(B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI | B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN | B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN |
		  	   B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN | B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN |
		  	   B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx | B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis);
  if(IioGlobalData->SetupData.EOI[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_EOI;

  // Override System Error on PCIE Fatal Errors  - Bit 35 of MISCCTRLSTS
  if(IioGlobalData->SetupData.MSIFATEN[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSIFATEN;

  // Override System Error on PCIE Non-Fatal Errors  - Bit 34 of MISCCTRLSTS
  if(IioGlobalData->SetupData.MSINFATEN[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSINFATEN;

  // Override System Error on PCIE Correctable  Errors  - Bit 33 of MISCCTRLSTS
  if(IioGlobalData->SetupData.MSICOREN[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_MSICOREN;

  // ACPI PME Interrupt Enable  - Bit 32 of MISCCTRLSTS
  if(IioGlobalData->SetupData.ACPIPMEn[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_ACPI_PMEEN;

  //
  // 4166584: L0s Feature Is Still Partially Present in the HSX BIOS
  //
  if (IioGlobalData->IioVData.CpuType == CPU_IVT) {
	  // Disable L0s on transmitter  - Bit 31 of MISCCTRLSTS
	  if(IioGlobalData->SetupData.DISL0STx[0])
		  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx;
#ifndef	BDX_HOST
  }else if(IioGlobalData->IioVData.CpuType == CPU_HSX){
#else
  }else if(IioGlobalData->IioVData.CpuType == CPU_HSX || IioGlobalData->IioVData.CpuType == CPU_BDX)  {
#endif
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_L0sTx;
  }

  // P2P Memory Read Disable  - Bit 24 of MISCCTRLSTS
  if(IioGlobalData->SetupData.P2PRdDis[0])
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_DIS_INB_P2PRdDis;

  // Disable Timeout 4 Rcv PME TO ACK  - Bit 6 of MISCCTRLSTS
  if (!IioGlobalData->SetupData.DisPMETOAck[0]) {
	  ExtCtrl |= B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT;
	  //
	  // if receiving of PME to ACK TO is enabled than use global
	  // option to set the timeout value in control bits 8:7
	  ExtCtrl |= ((IioGlobalData->SetupData.GlobalPme2AckTOCtrl & 3) << V_PCIE_PORT_PCIEMISCCTRLSTS_PME_AckTOCtl);
  }
  else{
	  ExtCtrl &= ~B_PCIE_PORT_PCIEMISCCTRLSTS_PME_TO_ACK_TIMEOUT;
  }

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS), (UINT32)ExtCtrl);
  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function, R_PCIE_PORT_PCIEMISCCTRLSTS+4), (UINT32)(ExtCtrl >> 0x20));

  //
  // bdx_eco: b226567 : Transactions are sent from IIO with wrong data and incorrect LCRC if the ECRC generation is enabled when the transaction is in progress
  // [cloned to s4988041] -[PCIe]: ECRC needs to be disabled on DMI port
  //
  Errcap.Data = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,ERRCAP_IIO_PCIE_REG ));
  if( Errcap.Bits.ecrc_check_capable == 1)
    Errcap.Bits.ecrc_check_capable = 0;
  if( Errcap.Bits.ecrc_generation_capable == 1)
    Errcap.Bits.ecrc_generation_capable = 0;
  S3_PciExpressWrite32(IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,ONLY_REGISTER_OFFSET(ERRCAP_IIO_PCIE_REG)),Errcap.Data);

}

VOID
DmiDeviceInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS            PciAddress,
  IIO_GLOBALS                                     *IioGlobalData
  )
/*++

Routine Description:

  DMI initialization after Bus number is/are assigned but other DMI resources are 
  not assigned.

Arguments:

   IioGlobalData           Pointer to the Universal Data Storage (UDS) for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
  UINT32                      RegLocation;
  DEVCTRL_IIO_PCIEDMI_STRUCT  DmiDevCtrl;
  PMCSR_IIO_PCIEDMI_STRUCT    DmiPmCsr;
  DEVCTRL2_IIO_PCIEDMI_HSX_BDX_STRUCT    DevCtrl2;
	//
	// Program the Device Control features
	//
	DmiDevCtrl.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
			PciAddress.Function,ONLY_REGISTER_OFFSET(DEVCTRL_IIO_PCIEDMI_REG)));
	S3_PciExpressWrite16 (IioGlobalData,
			 	 	 	  PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function,ONLY_REGISTER_OFFSET(DEVCTRL_IIO_PCIEDMI_REG)),
			 	 	 	  DmiDevCtrl.Data);
	//
	// Set D0 or D3Hot state based on setup option
	//
	DmiPmCsr.Data = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,
			PciAddress.Function,ONLY_REGISTER_OFFSET(PMCSR_IIO_PCIEDMI_REG)));
	if(IioGlobalData->SetupData.PcieDState[0] == 1)
		DmiPmCsr.Bits.power_state = 3;    // bit1:0 = 11: D3_hot
	else
		DmiPmCsr.Bits.power_state = 0;    // bit1:0 = 00: D0
	S3_PciExpressWrite32 (IioGlobalData,
						  PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device,PciAddress.Function,ONLY_REGISTER_OFFSET(PMCSR_IIO_PCIEDMI_REG)),
						  DmiPmCsr.Data);

	//
	// Program the DEVCTRL2 Completion Timeout features
	//
	//
    // Set Completion Time Out and value
    // HSD 4165823: PCIE: Global Completion Timeout Options needed
    //
  RegLocation = ONLY_REGISTER_OFFSET(DEVCTRL2_IIO_PCIEDMI_REG);

  DevCtrl2.Data = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation));

  if (IioGlobalData->SetupData.CompletionTimeoutGlobal) {
    DevCtrl2.Bits.compltodis = 0;                                       // enable the completion TO counter
    DevCtrl2.Bits.compltoval = IioGlobalData->SetupData.CompletionTimeoutGlobalValue;
  } else {
    if (IioGlobalData->SetupData.CompletionTimeout[0]) {
      DevCtrl2.Bits.compltodis = 0;                                       // enable the completion TO counter
      DevCtrl2.Bits.compltoval = IioGlobalData->SetupData.CompletionTimeoutValue[0];
    } else {
      DevCtrl2.Bits.compltodis = 1;                                       // disable the completion TO counter
      DevCtrl2.Bits.compltoval = 0;
    }
  }

  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), DevCtrl2.Data);

}

VOID
DmiMiscInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
  IIO_GLOBALS                                    *IioGlobalData
  )
/*++

Routine Description:

  DMI initialization all miscellaneous CSRs are initialized
  
Arguments:

   IioGlobalData           Pointer to the Universal Data Storage (UDS) for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
  PcieMiscInit (PciAddress, IioGlobalData);
}
      

VOID
DmiResourceAssignedInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
  IIO_GLOBALS                                    *IioGlobalData
  )
/*++

Routine Description:

  DMI initialization all DMI resources (bus and other resources) are assigned.
  
Arguments:

   IioGlobalData           Pointer to the Internal Data Storage (UDS) for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
    UINT32  RcbaAddress;
    UINT32  Var32;
    UINT16  Var16;
    UINT8   Var8;
    UINT8   i;
  
    for ( i = 0 ; i < MaxIIO ; i++ ) {
        if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) {
            break;
        }
    }
    if (i >= MaxIIO) return;
      
    //Program the DMIRCBAR
    if(IioGlobalData->IioVData.RcbaAddress[i]) {
	  Var32 = PciExpressRead32(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                   PciAddress.Device, PciAddress.Function, R_PCIE_PORT_DMIRCBAR));
      Var32 = IioGlobalData->IioVData.RcbaAddress[i] | BIT0;
	  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
            PciAddress.Device, PciAddress.Function, R_PCIE_PORT_DMIRCBAR), Var32);
    }

    // Handle programming of RWO and RWOG registers for IIO Port functions
    // Lock Revision ID
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                 PciAddress.Device, PciAddress.Function, PCI_REVISION_ID_OFFSET));
	S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
          PciAddress.Device, PciAddress.Function, PCI_REVISION_ID_OFFSET), Var8);
    
    if (FeaturePcdGet (PcdLockCsrSsidSvidRegister)) {
      // Lock Subsystem Vendor ID
      Var16 = PciExpressRead16(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                   PciAddress.Device, PciAddress.Function, PCI_SVID_OFFSET));
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
           PciAddress.Device, PciAddress.Function, PCI_SVID_OFFSET), Var16);

      // Lock Subsystem Device ID
      Var16 = PciExpressRead16(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                   PciAddress.Device, PciAddress.Function, PCI_SID_OFFSET));
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
           PciAddress.Device, PciAddress.Function, PCI_SID_OFFSET), Var16);

      // Lock Other Subsystem Device ID
      Var16 = PciExpressRead16(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                   PciAddress.Device, PciAddress.Function, R_PCIE_PORT_SSID));
      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
           PciAddress.Device, PciAddress.Function, R_PCIE_PORT_SSID), Var16);
    }

    // Lock Capabilities pointer
    // remove because this register access type =  RO_V 
    //Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
    //             PciAddress.Device, PciAddress.Function, PCI_CAPBILITY_POINTER_OFFSET));
    //S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
    //      PciAddress.Device, PciAddress.Function, PCI_CAPBILITY_POINTER_OFFSET), Var8);

    // Lock MSINXTPTR register
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                 PciAddress.Device, PciAddress.Function, R_PCIE_PORT_MSINXTPTR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
          PciAddress.Device, PciAddress.Function, R_PCIE_PORT_MSINXTPTR), Var8);

    // Lock PXPNXTPTR register
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                 PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPNXTPTR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
          PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPNXTPTR), Var8);

    // Lock PXPNXTPTR register
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                 PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPNXTPTR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
          PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPNXTPTR), Var8);

    
    // Lock PMCSR register
    Var8 = PciExpressRead8(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
                 PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPPMCSR));
    S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], 
          PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPPMCSR), Var8);

    
    RcbaAddress = IioGlobalData->IioVData.RcbaAddress[i];
    if (RcbaAddress) {
    }

    //
    // DMI masks should not be set, so clear them out
    //
    Var32 = PciExpressRead32(PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_UNCEDMASK));
    Var32 = BIT12; //to mask poisoned_tlp_detect_mask so the IIO port will neither log nor signal an
                   //           error when it intercepts data with ‘Poison TLP Receive’ error 

    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_UNCEDMASK), Var32);
    Var32 = 0x00000000;

    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_COREDMASK), Var32);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_RPEDMASK), Var32);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_XPUNCEDMASK), Var32);
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[i], PciAddress.Device,
                                     PciAddress.Function, R_PCIE_PORT_XPCOREDMASK), Var32);
    //
    // Clear Expected PCIe Errors (3612591, 3875328)
    //  [permanent programming consideration for all steppings]
    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIEDMI_REG)));
    Var32 |= BIT20;   //clear unsupported requests error
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(UNCERRSTS_IIO_PCIEDMI_REG)), Var32);

    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG)));
    Var16 |= (BIT14 + BIT15);   //clear Link Bandwidth Management Status & Link Autonomous Bandwidth Status
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIEDMI_REG)), Var16);

    Var32 = PciExpressRead32 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIEDMI_REG)));
    Var32 |= BIT0;   //clear PCILink Bandwidth Changed Status
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ONLY_REGISTER_OFFSET(XPCORERRSTS_IIO_PCIEDMI_REG)), Var32);

}

VOID
DmiInit (
  PCI_ROOT_BRIDGE_PCI_ADDRESS                 PciAddress,
  IIO_INIT_PHASE                              Phase,
  IIO_GLOBALS                                 *IioGlobalData
  )
/*++

Routine Description:

    Initialize DMI IIO function

Arguments:

  DmiPciAddress     - PCI address information of the device being enumerated/initialized
  Phase             - Defines the phase of PCI initialization that the PCI driver is executing. 
                      Either: 
                        EfiPciEnumerationDeviceScanning
                        EfiPciEnumerationBusNumberAssigned
                        EfiPciEnumerationResourceAssigned

Returns:

  Status            -
  EFI_SUCCESS       - Installed successfully
  Others            - Failed to install this protocol

--*/
{
  UINT8   i;
  
  for ( i = 0 ; i < MaxIIO ; i++ ) {
    if(PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) {
      break;
    }
  }
  if (i >= MaxIIO) return;
  // Just do the Legacy DMI
  if(OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE) return;

  switch(Phase) {
    case IIOInitPhase1:
        DEBUG ((EFI_D_INFO, "DMI IIOInitPhase1...\n"));
        DmiPreInit (PciAddress, IioGlobalData);
        OemDmiPreInit (PciAddress, IioGlobalData);
        break;

    case IIOInitPhase2:
        DEBUG ((EFI_D_INFO, "DMI IIOInitPhase2...\n"));
        DmiDeviceInit (PciAddress, IioGlobalData);
        OemDmiDeviceInit (PciAddress, IioGlobalData);
        break;

    case IIOInitPhase3:
        DEBUG ((EFI_D_INFO, "DMI IIOInitPhase3...\n"));
        DmiLinkInit (PciAddress, IioGlobalData);
        DmiMiscInit (PciAddress, IioGlobalData);
        DmiResourceAssignedInit (PciAddress, IioGlobalData);
        OemDmiResourceAssignedInit (PciAddress, IioGlobalData);
        break;
  }
}


VOID
DmiPostInit (
    IIO_GLOBALS                                     *IioGlobalData
  )
/*++

Routine Description:

  DMI post initialization settings after the other DMI resources are assigned.

Arguments:

   IioGlobalData           Pointer to the Internal Data Storage (UDS) for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{


}
