/*++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
--*/
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

  Ntb.c

Abstract:

  Implement north bridge PCI express port NTB-specific initialization

--*/
#include "IioLateInitialize.h"

#define NTB_PORT_DEF_TRANSPARENT  0
#define NTB_PORT_DEF_NTB_NTB      1
#define NTB_PORT_DEF_NTB_RP       2
#define NTB_PORT_DEF_NTB_AUTO     3

#define NTB_PB01_POW2_ALIGNMENT   16
#define NTB_PB01_SIZE             0x10000

#define R_PB01BASE_SB01BASE       0x510
#define   B_SB01BASE_PREFETCHABLE BIT3
#define R_PB01BASE_SB23BASE       0x518
#define R_PB01BASE_SB45BASE       0x520
#define R_NTBCNTL                 0x58
#define   B_SECLNK_DIS            BIT1
#define   B_SECCFG_LOCK           BIT0
#define ONLY_REGISTER_OFFSET(x) (x & 0xFFF)


VOID
Set_NTB_MPL(
    IIO_GLOBALS   *IioGlobalData,
    UINT8         Iio,
    UINT8         Device,
    UINT8         Function
)
/*++

Routine Description:
 Sets the given port Max. Payload size as per the user defined system configuration variable.
 This routine translates the PCI Port address to map correctly to port specific setup option.

Arguments:
  IioGlobalData  - Pointer to IIO_GLOBALS
  Iio     - index to CPU/IIO
  Device  - PCI device # of port which is going into NTB mode
  Function- PCI function # of port which is going into NTB mode

Returns:
  None

--*/
{
  UINT8   PortIndex = 0;
  UINT8   Bus;
  UINT16  Var16;

  Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  PCIEDEVFUNC2PORTINDEX(PortIndex, Device, Function);
  if(IioGlobalData->SetupData.PcieMaxPayload[(Iio * VARIABLE_GROUP_ELEMENTS) + PortIndex] != 2) //if not auto
  {
    Var16 = PciExpressRead16 (PCI_PCIE_ADDR(Bus, Device, Function, R_PCIE_PORT_PXPDEVCTRL));
    Var16 &= ~(B_PCIE_PORT_PXPDEVCTRL_MPS);
    Var16 |= (IioGlobalData->SetupData.PcieMaxPayload[(Iio * VARIABLE_GROUP_ELEMENTS) + PortIndex] << 5);
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(Bus, Device, Function, R_PCIE_PORT_PXPDEVCTRL), Var16);
  }
}

VOID
PreNtbInitPortConfiguration(
    IIO_GLOBALS   *IioGlobalData,
    UINT8         Iio,
    UINT8         Device,
    UINT8         Function
)
/*++

Routine Description:
 Performs any port configuration that is required before switching to NTB mode.

Arguments:
  IioGlobalData  - Pointer to IIO_GLOBALS
  Iio     - index to CPU/IIO
  Device  - PCI device # of port which is going into NTB mode
  Function- PCI function # of port which is going into NTB mode

Returns:
  None

--*/
{
  //
  //5002495:Clone from BDX Sighting:[BDX-DE/EP] Max Payload Size = 256B does not set bit correctly
  //
  Set_NTB_MPL(IioGlobalData, (UINT8)Iio, Device, Function);
}

VOID
NtbReadyToBootEvent (
    EFI_EVENT        Event,
    VOID             *Context
    )
{
  EFI_STATUS Status;
  UINT64 Addr;
  UINTN  Iio;
  UINT32 Data32;
  UINTN  Bus;
  UINT8  Ppd;
  PCICMD_IIO_PCIENTB_STRUCT   PciCmd, PciCmdSave;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR(Status))
    return;

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if(!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

    Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    //if NTB is configured as NTB-RP...
    Ppd = IioGlobalData->SetupData.NtbPpd[Iio];
    if(Ppd == NTB_PORT_DEF_NTB_AUTO) // If auto, then decide if it is a TB or NTB port based on NtbPpdAutoConfig.
      Ppd = IioGlobalData->BdxSetupData.NtbPpdAutoConfig[Iio]; 
    if (Ppd != NTB_PORT_DEF_TRANSPARENT) {
      Addr = PciExpressRead32(PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB01BASE_0_IIO_PCIENTB_REG)));
      Addr &= ~0x1fff;
      Addr |= (UINTN)PciExpressRead32(PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB01BASE_1_IIO_PCIENTB_REG))) << 32;
      Addr += R_PB01BASE_SB01BASE;


      //we now set prefetch in the register based on the setup question the user has selected
      //1st check if access enable?
      PciCmdSave.Data = PciExpressRead16(PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PCICMD_IIO_PCIENTB_REG)));
      if(PciCmdSave.Bits.mse == 0) {
        PciCmd.Data = PciCmdSave.Data;
        PciCmd.Bits.mse = 1;
        S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PCICMD_IIO_PCIENTB_REG)), PciCmd.Data);
      }

      //Bit 3 is the prefetchable bit, with 1 = enabled
      Data32 = *(volatile UINT32 *) Addr;  

      Data32 &= ~B_SB01BASE_PREFETCHABLE;
      if (IioGlobalData->SetupData.NtbSBar01Prefetch[Iio])
      {
        Data32 |= B_SB01BASE_PREFETCHABLE;
      }

      *(volatile UINT32 *) Addr =  Data32;
      S3BootScriptSaveMemWrite ( S3BootScriptWidthUint32, IioGlobalData->IioVData.PciExpressBase + Addr, 1, &Data32);

      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PCICMD_IIO_PCIENTB_REG)), PciCmdSave.Data);
    }
  } // per-socket loop.

  gBS->CloseEvent (Event);

  return;
}
///////////////////////////////////////////////////////////////////////////////////////
//
// Routine: NtbInit
//
// Description: Initializes the non-transparent bridge for dev 3 / fun 0
//              
// We configured the NTB_PPD setup option as follows:
//
//		NTB_PPD = 0		Transparent Bridge configuration
//		NTB_PPD = 1		Non Transparent Bridge to Non Transparent Bridge configuration
//  	NTB_PPD = 2		Non Transparent Brigde to Root Port configuration
//
//	Note that in the PPD register (d3/f0/off 0D4h), bits 1:0 are used to configure the
//	NTB port as follows: 
//		00 = TB
//		01 = NTB-NTB
//		02=  NTB-RP
//
// Input:
//
//
// Output:       NTB Root port has been initialized for dev 3 / fun 0
//
// Return value: none
//
/////////////////////////////////////////////////////////////////////////////////////
VOID
NtbInit(
    IIO_GLOBALS  *IioGlobalData,
    EFI_HANDLE   DriverImageHandle
  )
{
  UINT8  PPD; 	//the current value of this setup option
  UINT64 Addr;
  UINTN  Bus;
  UINTN  Iio;
  PPD_IIO_PCIENTB_HSX_BDX_STRUCT            PpdReg;
  PBAR23SZ_IIO_PCIENTB_STRUCT           Pbar23Sz;
  PB23BASE_0_IIO_PCIENTB_STRUCT         Pb23BaseN0;
  PB23BASE_1_IIO_PCIENTB_STRUCT         Pb23BaseN1;
  PB45BASE_0_IIO_PCIENTB_STRUCT         Pb45BaseN0;
  PB45BASE_1_IIO_PCIENTB_STRUCT         Pb45BaseN1;
  PBAR45SZ_IIO_PCIENTB_STRUCT           Pbar45Sz;
  SBAR23SZ_IIO_PCIENTB_STRUCT           Sbar23Sz;
  SBAR45SZ_IIO_PCIENTB_STRUCT           Sbar45Sz;
  MISCCTRLSTS_0_IIO_PCIE_STRUCT         IioMiscCtrl;
  MISCCTRLSTS_1_IIO_PCIE_BDX_STRUCT     MiscCtrlStsN1;
#ifdef DE_SKU
  MAX_FC0_IIO_DFX_GLOBAL_HSX_BDX_STRUCT MaxFc0;
#endif
  VOID                                  *Registration;
    

  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

    Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    DEBUG ((EFI_D_INFO, "Initializing NTB for SKT%d\n", Iio));

    //retrieve NTB PPD setup question to determine if port should be configured as Transparent Bridge, NTB to NTB, or
    //NTB to Root Port

    PPD = IioGlobalData->SetupData.NtbPpd[Iio]; //need to use setup option gDxePlatformSaPolicy->PcieConfig->NtbPpd[Socket];
    if(PPD == NTB_PORT_DEF_NTB_AUTO) // If auto, then decide if it is a TB or NTB port based on NtbPpdAutoConfig.
      PPD = IioGlobalData->BdxSetupData.NtbPpdAutoConfig[Iio];

    DEBUG ((EFI_D_INFO, "setup PPD %d\n", PPD));
    
    if(PPD != NTB_PORT_DEF_TRANSPARENT) { //if NTB is not configured as "TB"
      //
      // Perform any port configuration that are required before switching to NTB mode
      //
      PreNtbInitPortConfiguration(IioGlobalData, (UINT8)Iio, NTB_DEV, NTB_FUNC);
    }

    //Read in PPD register
    Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PPD_IIO_PCIENTB_REG));
    PpdReg.Data = PciExpressRead8(Addr); 
    PpdReg.Bits.port_definition = PPD;
    if (IioGlobalData->SetupData.NtbSplitBar[Iio] && (PPD != NTB_PORT_DEF_TRANSPARENT))
      PpdReg.Data |= BIT6;
    S3_PciExpressWrite8(IioGlobalData, Addr, PpdReg.Data);

    //now if the NTB is acting as just a TB, we don't need to do anything further in terms of the BARs...
    //else, let's now configure the sizes of the prefetchable BARs

    if(PPD != NTB_PORT_DEF_TRANSPARENT) {	//if NTB is not configured as "TB"

      //djm_xxx: we wish to only set the NTB BAR Size registers if the setup option for BAR register enable indicates so.  The reason
      //is that in some usage models, we leave it up to the drivers to write these registers.  If BIOS writes these registers, this may
      //cause driver implementations issues as these registers are write once.

      if(IioGlobalData->SetupData.NtbBarSizeOverride[Iio] == 1) {
        // HSD 3616783: IVT- EP: PCIe NTB: pb23base and pb45base not set correctly
        //              set the prefetchable BAR size registers
        // Applies to HSX
        //

        //
        // Set PBAR23.
        //
        Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PBAR23SZ_IIO_PCIENTB_REG));
        Pbar23Sz.Bits.primary_bar_2_3_size = IioGlobalData->SetupData.NtbBarSizePBar23[Iio]; 
        S3_PciExpressWrite8(IioGlobalData, Addr,  Pbar23Sz.Data);

        Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB23BASE_0_IIO_PCIENTB_REG));
        Pb23BaseN0.Data = 0;
        Pb23BaseN0.Bits.primary_bar_2_3_base = 0;
        S3_PciExpressWrite32(IioGlobalData, Addr, Pb23BaseN0.Data);

        Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB23BASE_1_IIO_PCIENTB_REG));
        Pb23BaseN1.Bits.primary_bar_2_3_base = 0xff00;
        S3_PciExpressWrite32(IioGlobalData, Addr, Pb23BaseN1.Data);

        if (!IioGlobalData->SetupData.NtbSplitBar[Iio]) {
          //
          // Set PBAR45.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PBAR45SZ_IIO_PCIENTB_REG));
          Pbar45Sz.Bits.primary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizePBar45[Iio]; 
          S3_PciExpressWrite8(IioGlobalData, Addr, Pbar45Sz.Data);

          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB45BASE_0_IIO_PCIENTB_REG));
          Pb45BaseN0.Data = 0;
          Pb45BaseN0.Bits.primary_bar_4_5_base = 0;
          S3_PciExpressWrite32(IioGlobalData, Addr, Pb45BaseN0.Data);

          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PB45BASE_1_IIO_PCIENTB_REG));
          Pb45BaseN1.Data = 0;
          Pb45BaseN1.Bits.primary_bar_4_5_base = 0xff00;
          S3_PciExpressWrite32(IioGlobalData, Addr, Pb45BaseN1.Data);
        } else {
          //
          // Set PBAR4.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PBAR45SZ_IIO_PCIENTB_REG));
          Pbar45Sz.Bits.primary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizePBar4[Iio]; 
          S3_PciExpressWrite8 (IioGlobalData,Addr, Pbar45Sz.Data);
          //
          // Set PBAR5.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, 0xD6);
          Pbar45Sz.Bits.primary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizePBar5[Iio]; 
          S3_PciExpressWrite8 (IioGlobalData, Addr, Pbar45Sz.Data);

        }
        //
        // Set SBAR23.
        //
        Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(SBAR23SZ_IIO_PCIENTB_REG));
        Sbar23Sz.Bits.secondary_bar_2_3_size = IioGlobalData->SetupData.NtbBarSizeSBar23[Iio]; 
        S3_PciExpressWrite8(IioGlobalData, Addr, Sbar23Sz.Data);

        if (!IioGlobalData->SetupData.NtbSplitBar[Iio]) {
          //
          // Set SBAR45.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(SBAR45SZ_IIO_PCIENTB_REG));
          Sbar45Sz.Bits.secondary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizeSBar45[Iio]; 
          S3_PciExpressWrite8(IioGlobalData, Addr, Sbar45Sz.Data);
        } else {
          //
          // Set SBAR4.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(SBAR45SZ_IIO_PCIENTB_REG));
          Sbar45Sz.Bits.secondary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizeSBar4[Iio]; 
          S3_PciExpressWrite8 (IioGlobalData, Addr, Sbar45Sz.Data);
          
          //
          // Set SBAR5.
          //
          Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, 0xD5);
          Sbar45Sz.Bits.secondary_bar_4_5_size = IioGlobalData->SetupData.NtbBarSizeSBar5[Iio]; 
          S3_PciExpressWrite8 (IioGlobalData, Addr, Sbar45Sz.Data);
        
        }
      }
    }

    //if NTB is configured as NTB-NTB...
    if(PPD == NTB_PORT_DEF_NTB_NTB) {
      //only in the NTB-NTB configuration does BIOS need to configure the crosslink control override field of the
      //PCIE Port Definition register.  According to EAS, one platform BIOS should configure the register field as
      // "USD/DSP" while the other platform BIOS should configure the register field as "DSD/USP".

      //a setup option has been created for specifying this value (see IIO.sd).  we will read the value of that
      //setup option into XLINK below.  The value of the setup option matches the value that should be written
      //into bits 2:3 of the PPD register.

      //Port bifurcation is always "wait-on-BIOS".  However a potential issue is that the crosslink control
      //option is programmed after link training is initiated.  Also has no Crosslink-control HW strap like JSP does,
      //so this is always prorammed from setup.

      //now read in the PPD register so that we can set bits 2:3
      Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(PPD_IIO_PCIENTB_REG));
      PpdReg.Data = PciExpressRead8(Addr); 
      if (IioGlobalData->SetupData.NtbXlinkCtlOverride[Iio] == 1) // Autoconfig
        PpdReg.Bits.crosslink_control = IioGlobalData->BdxSetupData.NtbXlinkCtlAutoConfig[Iio];
      else
        PpdReg.Bits.crosslink_control = IioGlobalData->SetupData.NtbXlinkCtlOverride[Iio]; 
      S3_PciExpressWrite8(IioGlobalData, Addr, PpdReg.Data);
    }


    // SERVER_BIOS_FLAG note: skip power management programming, should be taken care of standard port config.
    // SERVER_BIOS_FLAG note: skip gen1/gen2/gen3 programming, should be taken care of in standard port config.
    // SERVER_BIOS_FLAG note: skip no_forw_prog programming, appears to not be available in this architcture.
#ifdef DE_SKU
    //HSD 5003397 Whenever IOU1 is bifurcated and has peer-to-peer (P2P) access to the NTB link 
    //(IOU1 function 0), a deadlock may result from an internal resource arbitration bug.
    if (PPD == NTB_PORT_DEF_NTB_NTB) {
      Addr = PCI_PCIE_ADDR(Bus,PCIE_PORT_GLOBAL_DFX_DEV ,PCIE_PORT_GLOBAL_DFX_FUNC, ONLY_REGISTER_OFFSET(MAX_FC0_IIO_DFX_GLOBAL_REG));
      MaxFc0.Data = PciExpressRead32(Addr);
      MaxFc0.Bits.no_forw_prog = 1;
      S3_PciExpressWrite32(IioGlobalData, Addr, MaxFc0.Data);
    }
#endif
    if (PPD != NTB_PORT_DEF_TRANSPARENT) {
      //
      // To avoid potential deadlock issues arising from two Processors connected together which create certain
      // dependencies, disable remote p2p in the system, disable ACS on the NTB port, and mark the NTB port
      // as problematic.  Reference IIO EAS $6.4 "In/Out Dependency".
      // 
      //

      // disable remote peer2peer in the system.
      Addr = PCI_PCIE_ADDR(Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, ONLY_REGISTER_OFFSET(IIOMISCCTRL_N0_IIO_VTD_REG));
      IioMiscCtrl.Data = PciExpressRead32(Addr); 
      //IioMiscCtrl.Bits.disable_remote_p2p_reads = 1; // A step and B step use same bit locations
      //IioMiscCtrl.Bits.disable_remote_p2p_memory_writes = 1; // A step and B step use same bit locations
      S3_PciExpressWrite32(IioGlobalData, Addr, IioMiscCtrl.Data);

      // 
      // HSD 4166662: HSX Clone: Supercollider: 3-strike timeout (Ubox Lock FSM hang)
      //                         Only set B_PROBLEMATIC_PORT_FOR_LOCK_FLOW in IVT
      // HSD 4986522: Apply the IVT fix for BDX and HSX C0. 
      //
      if ((IioGlobalData->IioVData.CpuType == CPU_IVT) || (IioGlobalData->IioVData.CpuType == CPU_BDX) ||
          ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_HSX))) {
        Addr = PCI_PCIE_ADDR(Bus, NTB_DEV, NTB_FUNC, ONLY_REGISTER_OFFSET(MISCCTRLSTS_1_IIO_PCIENTB_REG));
        MiscCtrlStsN1.Data = PciExpressRead32(Addr); 
        MiscCtrlStsN1.Bits.problematic_port_for_lock_flows = 1;
        S3_PciExpressWrite32(IioGlobalData, Addr, MiscCtrlStsN1.Data);
      }

    } // if PPD != NTP_PORT_DEF_TRANSPARENT
  } // per-socket loop.

//  Status = CreateReadyToBootEvent (TPL_CALLBACK, NtbReadyToBootEvent, NULL, &ReadyToBootEvent);

   EfiCreateProtocolNotifyEvent (
            &gExitPmAuthProtocolGuid,
            TPL_CALLBACK,
            NtbReadyToBootEvent,
            NULL,
            &Registration
            );

} //end NtbInit
