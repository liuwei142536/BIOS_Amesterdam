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

  IioVtdInit.c

Abstract:

  This file initialize the Vtd registers and report DMAR table

--*/
#include "IioLateInitialize.h"

EFI_STATUS
VtdInitialization (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData
    ) 
{
  UINT64            VtdMmioCap;
  UINT64            VtdMmioExtCap;
  UINT64            Vtd1MmioCap;
  UINT64            Vtd1MmioExtCap;
  UINTN             VtdMemoryMap;
  UINT32            VtdBase;
  UINT16            VtGenCtrl;
  UINT16            VtdIsoCtrl;
  UINT8             CORE05_BUSNUM=0;
  UINT8             RootBridgeLoop;


  if (Phase == IIOInitPhase1) {
    IioWorkAround(PciAddress, Phase, IioGlobalData, IioVtDPreEn, NULL);
  }

  if (Phase == IIOInitPhase3) {

    if (!IioGlobalData->SetupData.VTdSupport) {
      VtGenCtrl  = PciExpressRead16 (PCI_PCIE_ADDR(CORE05_BUSNUM,
                              CORE05_DEV_NUM,
                              CORE05_FUNC_NUM,
                              R_IIO_VTGENCTRL));

      VtGenCtrl  |= (V_IIO_VTCTRL_LOCK_VTD_SET);   //Bit[15]  VT_D BAR lock bit

      S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(CORE05_BUSNUM,
                    CORE05_DEV_NUM,
                    CORE05_FUNC_NUM,
                    R_IIO_VTGENCTRL), VtGenCtrl);

      DEBUG ((EFI_D_INFO, "Vt-D support is disabled. \n"));
      return EFI_UNSUPPORTED;
    }


    if (PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[0]){
      //
      // 1st(legacy) IIO
      //
      if (IioGlobalData->SetupData.COREHIDE[0]) return EFI_UNSUPPORTED;

      RootBridgeLoop  = 0;

    } else if (PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[1]){ 
      //
      // 2nd IIO
      //
      if (IioGlobalData->SetupData.COREHIDE[1*VARIABLE_FUNC3_ELEMENTS]) return EFI_UNSUPPORTED;

      RootBridgeLoop  = 1;

    }
    else if (PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[2]){
      //
      // 3rd IIO
      //
      if (IioGlobalData->SetupData.COREHIDE[2*VARIABLE_FUNC3_ELEMENTS]) return EFI_UNSUPPORTED;

      RootBridgeLoop  = 2;

    }
    else if (PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[3]){
      //
      // 4th IIO
      //
      if (IioGlobalData->SetupData.COREHIDE[3*VARIABLE_FUNC3_ELEMENTS]) return EFI_UNSUPPORTED;

      RootBridgeLoop  = 3;

    }
    else return EFI_UNSUPPORTED;

    //VtdSetupIntRemapUpdate( IioGlobalData);

    DEBUG ((EFI_D_INFO, "VT-d Chipset Initialization for IIO%d ...\n", RootBridgeLoop));

    VtdBase = (UINT32)IioGlobalData->IioVData.VtdBarAddress[RootBridgeLoop];

    CORE05_BUSNUM = IioGlobalData->IioVData.SocketBaseBusNumber[RootBridgeLoop];

    IioWorkAround ( PciAddress, Phase, IioGlobalData, IioVtDEn, NULL);

    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(CORE05_BUSNUM, CORE05_DEV_NUM, CORE05_FUNC_NUM,
                            R_IIO_VTBAR), (VtdBase | B_IIO_VT_BAR_EN));

    VtGenCtrl  = PciExpressRead16 (PCI_PCIE_ADDR(CORE05_BUSNUM,
                            CORE05_DEV_NUM,
                            CORE05_FUNC_NUM,
                            R_IIO_VTGENCTRL));

    VtGenCtrl  &= V_IIO_VTCTRL_LIMITS_CLEAR;     // Clear Bits[0-10]corresponding to Isoch and non isoch engine limit values

    VtGenCtrl  |= (V_IIO_VTCTRL_LOCK_VTD_SET);   //Bit[15]  lockvtd bit
    
    VtdIsoCtrl  = PciExpressRead16 (PCI_PCIE_ADDR(CORE05_BUSNUM,
                            CORE05_DEV_NUM,
                            CORE05_FUNC_NUM,
                            R_IIO_VTISOCHCTRL));
    VtdIsoCtrl  &= ~(BIT1 + BIT2 + BIT3 + BIT4);  // Clear the bits before changes

    if (IioGlobalData->SetupData.EnableAzaliaVCpOptimizations) {
      VtdIsoCtrl  |= (B_IIO_VTISOCHCTRL_AZALIA_ON_VCP);
    } else {
      VtdIsoCtrl  &= ~(B_IIO_VTISOCHCTRL_AZALIA_ON_VCP);
    }    

    //
    //  NonIsoc GPAand HPA limits
    // 
    VtGenCtrl  |= V_IIO_VTCTRL_VTHPAL_46BIT << 04;

    VtGenCtrl  |= V_IIO_VTCTRL_VTNGPAL_48BIT;

    //
    // Isoc L1 Entry(BIT0=1), Azalia Descriptor entry(BIT1=0), Steer to non-isac VT-d engine programm(BITS[2-4]=0)
    //
    
    if ((RootBridgeLoop == 0) && ((IioGlobalData->IioVData.IsocEnable) || (IioGlobalData->IioVData.MeSegEn))) {
       // VtIsochCtrl.Bits.azaliausenonisochvtd = 0;
       VtdIsoCtrl  &= (UINT16)(~BIT0); //AzaliaUseNonIsochVTd
      } else {
       // VtIsochCtrl.Bits.azaliausenonisochvtd = 1;
       VtdIsoCtrl  |= BIT0;
      }


    
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(CORE05_BUSNUM,
              CORE05_DEV_NUM,
              CORE05_FUNC_NUM,
              R_IIO_VTGENCTRL), VtGenCtrl);

    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(CORE05_BUSNUM,
              CORE05_DEV_NUM,
              CORE05_FUNC_NUM,
              R_IIO_VTISOCHCTRL), VtdIsoCtrl);

    DEBUG ((EFI_D_INFO, "Vt-D base address         :  0x%08lx\n", VtdBase));
    DEBUG ((EFI_D_INFO, "VtDGenCtrlReg             :  0x%08x\n", VtGenCtrl));
    DEBUG ((EFI_D_INFO, "VtDIsoCtrlReg             :  0x%08x\n", VtdIsoCtrl));

    //
    // Non Isoc engine Capabilities and Ext capabilities
    //

    VtdMemoryMap      = VtdBase;
    VtdMmioCap        = *(volatile UINT64 *)(UINTN) (VtdMemoryMap + R_VTD_CAP_LOW);

    //
    // BIT23=0 for other than Isoch VT-d MMMIO.
    //
    VtdMmioCap &= ~(UINT64)BIT23;

    //
    // ZLR support enable BIT22 = 1
    //
    VtdMmioCap |= BIT22;

    // comment the following because the register is Read-Only
    //*(volatile UINT64 *)(UINTN) (VtdMemoryMap + R_VTD_CAP_LOW)  = VtdMmioCap;

    VtdMmioExtCap     = *(volatile UINT64 *)(UINTN) (VtdMemoryMap + R_VTD_EXT_CAP_LOW);

    //
    // Set Coherency for Non-Isoch Vtd
    // 
    VtdMmioExtCap &= ~(BIT0);
    if (IioGlobalData->SetupData.CoherencySupport) {
        VtdMmioExtCap   |= BIT0;
    }


    //
    // 4986555 ATS should always be 1. ATS is bit 2 on ecap.
    // (Removed setup option)
    //
    VtdMmioExtCap   |= BIT2;
    VtdMmioExtCap   &= ~(BIT3);
    if (IioGlobalData->SetupData.InterruptRemap) {
      VtdMmioExtCap   |= BIT3;
    }

    *(volatile UINT64 *)(UINTN) (VtdMemoryMap + R_VTD_EXT_CAP_LOW)  = VtdMmioExtCap;

    //
    // Save for S3 resume
    //
    S3BootScriptSaveMemWrite(S3BootScriptWidthUint64, VtdMemoryMap + R_VTD_EXT_CAP_LOW, 1, &VtdMmioExtCap);

    DEBUG ((EFI_D_INFO, "Non-Iso Engine CapReg     :  0x%016lx\n", VtdMmioCap));
    DEBUG ((EFI_D_INFO, "Non-Iso Engine ExtCapReg  :  0x%016lx\n", VtdMmioExtCap));

    //
    // Program Isoch Capability and ExtCapabilities
    // 
    
    Vtd1MmioCap     = *(volatile UINT64 *)(UINTN) (VtdBase + R_VTD1_CAP_LOW);
    Vtd1MmioExtCap  = *(volatile UINT64 *)(UINTN) (VtdBase + R_VTD1_EXT_CAP_LOW);

    //
    // Set Coherency for Isoch Vtd
    // 
    Vtd1MmioExtCap &= ~(BIT0);
    if ( (IioGlobalData->SetupData.IsochCoherencySupport) && (Vtd1MmioCap & BIT23) ) {
        Vtd1MmioExtCap   |= BIT0;
    } 

    //
    // Write back updated Vtd1MmioExtCap register
    // 
    *(volatile UINT64 *)(UINTN) (VtdBase + R_VTD1_EXT_CAP_LOW) = Vtd1MmioExtCap;
    //
    // Save for S3 resume
    //
    S3BootScriptSaveMemWrite(S3BootScriptWidthUint64, VtdBase + R_VTD1_EXT_CAP_LOW, 1, &Vtd1MmioExtCap);
    
  }
  return EFI_SUCCESS;
}
