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

  IioIoat.c

Abstract:

  Houses all code related to Uncore specific CPU IIO initialization 

Revision History:

--*/
#include <Token.h> // AptioV server override
#include "IioLateInitialize.h"

//
// CB BAR Memory mapped regisers
//
#define R_CB_BAR_CSI_DMACAPABILITY                0x10
#define   B_CB_BAR_CSI_DMACAP_DCA_SUP             BIT4
#define R_CB_BAR_CSI_CAP_ENABLE                   0x10C
#define   B_CB_BAR_CSI_CAP_ENABLE_PREFETCH_HINT   BIT0
#define R_CB_BAR_PCIE_CAP_ENABLE                  0x10E
#define   B_CB_BAR_PCIE_CAP_ENABLE_MEM_WR         BIT0
#define R_CB_BAR_APICID_TAG_MAP                   0x110
#define R_CB_BAR_CBVER                            0x08
#define   V_CB32_VERSION                          0x32
#define   V_CB33_VERSION                          0x33

#define IIO_CBDMA_MMIO_SIZE 0x10000 //64kB for one CBDMA function
#define IIO_CBDMA_MMIO_ALLIGNMENT 14 //2^14 - 16kB

extern EFI_HANDLE mDriverImageHandle;

UINT64
GetActualCbBAR(
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  UINTN   Addr;
  UINT64  CbBar = 0;

  //
  // Determine the CB version from the CB DMA MMIO config space
  //
  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
          ONLY_REGISTER_OFFSET(CB_BAR_0_IIOCB_FUNC0_REG));

  CbBar = (UINT64)PciExpressRead32(Addr + 4) << 32;
  CbBar |= (UINT64)(PciExpressRead32(Addr) & ~ 0x0F);

  return CbBar;
}

UINT8
Get_CB_version(
    PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress,
    UINT64                                          CbBar
)
{
  UINTN                       Addr;
  PCICMD_IIOCB_FUNC0_STRUCT   OrgPciCmd, PciCmd;
  UINT8                       CbVer;

  //
  // Retrieve the CB version from the device
  //
  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
            ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG));
  //(1) - save the CB PCI CMD register
  PciCmd.Data = PciExpressRead16(Addr);
  OrgPciCmd.Data = PciCmd.Data;

  //(2) - enable the CB configuration MMIO space
  PciCmd.Bits.mse = 1;
  PciExpressWrite16(Addr, PciCmd.Data);

  //{3} - read CBVER register from the CB MMIO space
  CbVer = *(volatile UINT8 *)((UINTN)(CbBar + R_CB_BAR_CBVER));

  //(4) - restore the CB device state
  PciExpressWrite16(Addr, OrgPciCmd.Data);

  return CbVer;
}

BOOLEAN
DmaFunctionExist(
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  if(PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
                            ONLY_REGISTER_OFFSET(VID_IIOCB_FUNC0_REG))) != 0x8086)
    return FALSE;
  else
    return TRUE;
}

VOID
SaveCbDmaDeviceBaseAddress(
    IIO_GLOBALS                                    *IioGlobalData,
    UINT8                                          Iio,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  UINT8   TotalCbDmaDevicesPerPkg = IioGlobalData->IioVData.DmaDevice[Iio].TotalDmaControllers;

  if(TotalCbDmaDevicesPerPkg >= MAX_CBDMA_CONTROLLERS){
    DEBUG((EFI_D_INFO, "WARNING!!! The CBDMA device (%d,%d,%d) cannot be stored internally.\n",
        PciAddress.Bus, PciAddress.Device, PciAddress.Function));
    return;
  }

  IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[TotalCbDmaDevicesPerPkg].PciBusNo = PciAddress.Bus;
  IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[TotalCbDmaDevicesPerPkg].PciDevNo = PciAddress.Device;
  IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[TotalCbDmaDevicesPerPkg].PciFunNo = PciAddress.Function;
  IioGlobalData->IioVData.DmaDevice[Iio].TotalDmaControllers++;
}

VOID
DetermineCbDmaType(
    IIO_GLOBALS                                    *IioGlobalData,
    UINT8                                          Iio,
    UINT8                                          CbDevNo,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
/*
 * The CBDMA controller type can be determine by reading the channel 0
 * configuration space, thus. this routine should be called post PCI enumeration
 * where the CBDMA PCI resources are already assigned.
 * Assumption: The PCI address input parameter contains valid CBDMA device address
 */
{
    UINT64  CbBar;
    BOOLEAN FormulateCBversion = FALSE;

    CbBar = GetActualCbBAR(PciAddress);
    DEBUG((EFI_D_INFO, "BDF=%d,%d,%d - CB_BAR=0x%lx\n",
            PciAddress.Bus, PciAddress.Device, PciAddress.Function, CbBar));
    if(CbBar){
        // read CB version
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER =
            Get_CB_version( PciAddress, CbBar);
        if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == 0  ||
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == 0xFF){
          DEBUG((EFI_D_ERROR,"CBDMA ERROR! not able to read the CB version\n"));
          FormulateCBversion = TRUE;
        }
    }
    else{
        DEBUG((EFI_D_ERROR,"CB BAR not initialized!\n"));
        FormulateCBversion = TRUE;
    }

    if(FormulateCBversion == TRUE){
      if(OemIdentifyLegacyCbDmaDevice( IioGlobalData, PciAddress.Bus,PciAddress.Device, PciAddress.Function) == FALSE){
        //
        // since the CB BAR is not yet initialized now simply go with assumption based on CPU type & SKU
        //
        if(IioGlobalData->IioVData.CpuType == CPU_BDX){
          if(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE)
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER = V_CB33_VERSION;
          else
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER = V_CB32_VERSION;
        }
        else
          IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER = V_CB32_VERSION;
      }
      else
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER = V_CB32_VERSION;
    }
    DEBUG((EFI_D_INFO,"CBDMA[%d].version=0x%x\n", CbDevNo, IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER));
    //
    // determine the PCI address and number of channels
    //
    IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].BusNo = PciAddress.Bus;
    IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevNo = PciAddress.Device;
    IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].FunNo = PciAddress.Function;
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB32_VERSION){
        if(IioGlobalData->IioVData.CpuType == CPU_IVT ||
           IioGlobalData->IioVData.CpuType == CPU_HSX ||
           IioGlobalData->IioVData.CpuType == CPU_BDX){
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].MaxNoChannels = IOAT_TOTAL_FUNCS;
        }
        else
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].MaxNoChannels = 0;  //unknown
    }
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
        if(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE)
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].MaxNoChannels = IOAT_TOTAL_FUNCS / 2;
        else
            IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].MaxNoChannels = 0;  //unknown
    }

    //
    // determine the CBDMA capability features
    //
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB32_VERSION){
      if(IioGlobalData->IioVData.CpuType == CPU_IVT ||
          IioGlobalData->IioVData.CpuType == CPU_HSX ||
          IioGlobalData->IioVData.CpuType == CPU_BDX){
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.DcaSupported = TRUE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.NoSnoopSupported = TRUE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.RelaxOrderSupported = TRUE;
      }
    }
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
      if(IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE){
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.DcaSupported = FALSE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.NoSnoopSupported = TRUE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.RelaxOrderSupported = FALSE;
      }
      else{
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.DcaSupported = FALSE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.NoSnoopSupported = FALSE;
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.RelaxOrderSupported = FALSE;
      }
    }

    //
    // determine the Device configuration register
    //
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB32_VERSION){
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevCfgRegister = ONLY_REGISTER_OFFSET(DEVCFG_IIOCB_FUNC0_REG);
    }
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevCfgRegister = R_IIO_CB33_DEVCFG;
    }

    //
    // determine the Device control register
    //
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB32_VERSION){
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevConRegister = ONLY_REGISTER_OFFSET(DEVCON_IIOCB_FUNC0_REG);
    }
    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevConRegister = R_IIO_CB33_DEVCON;
    }

    IioGlobalData->IioOutData.DMAhost[Iio].TotalCbDmaDevicePerPackage++;
}

VOID
IioIoatWorkarounds(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           CbDevNo,
    PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress,
    IIO_INIT_PHASE                                  Phase
)
{
  if(Phase == IIOInitPhase3){

    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB32_VERSION){
      //TODO: if any
    }

    if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
#ifdef DE_SKU
      UINT32  Value32;
      UINTN   Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 0);
      if(DmaFunctionExist(PciAddress) == TRUE){
        //
        // bwd_cb33_eco 3563327: CBDMA Advisory ERRCORMSK.ANFEM register field has incorrect default value
        //  [promoted to 4986904]
        Addr = (Addr & 0xFFFFF000) | R_BWD_CB33_ERRCORMSK;
        Value32 = PciExpressRead32(Addr);
        Value32 |= B_BWD_CB33_ANFEM;
        S3_PciExpressWrite32(IioGlobalData, Addr, Value32);

        //
        // as per the 4988026, the following BDX-DE bugeco fixes the Briarwood errata 3563416...
        // bdx_bugeco 268669: CLONE from broadwell_server: Fix: BWD Clock Gating bug (clkreq goes low
        // before MSI is sent out)
        //
        // bwd_cb33_eco 3563416: Pending interrupts may be delayed when interrupt
        // coalescing timer is running and Intel Quickdata technology allows its clock to be gated
        //  [promoted to 4986902] - clear IOSFDEVCLKGCTL.ICGE & SBDEVCLKGCTL.ICGE

      }
#endif  //DE_SKU
    } // CB ver 3.3

  } //phase 3
}

VOID
Get_UDS(
    IIO_UDS                             **IioUds
)
{
  EFI_IIO_UDS_PROTOCOL                *IioUdsProtocol;
  EFI_STATUS                          Status;
  //
  // Locate the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &IioUdsProtocol);
  if ( EFI_ERROR(Status) )
  {
    DEBUG((EFI_D_ERROR, "IIO IOAT Init: Cannot locate gEfiIioUdsProtocolGuid protocol!!\n"));
    ASSERT_EFI_ERROR(Status);
  }

  *IioUds = IioUdsProtocol->IioUdsPtr;
}

VOID
StoreDmaDeviceInfo(
  IIO_GLOBALS                                    *IioGlobalData,
  UINT8                                          Iio,
  UINT8                                          CbDevNo,
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  IIO_UDS                             *IioUds;

  Get_UDS(&IioUds);

  if(IioUds == NULL){
    DEBUG((EFI_D_ERROR,"IIO IOAT Init:Unable to store DMA device info!\n"));
    return;
  }

  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceInfo[CbDevNo].DmaControllerPresent = 1;
  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceCount = \
      IioGlobalData->IioOutData.DMAhost[Iio].TotalCbDmaDevicePerPackage;
  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceInfo[CbDevNo].MaxDmaChannels = \
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].MaxNoChannels;
  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceInfo[CbDevNo].DmaStartAddress.PciBusNo = \
      IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].BusNo;
  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceInfo[CbDevNo].DmaStartAddress.PciDevNo = \
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevNo;
  IioUds->PlatformData.IIO_resource[Iio].DmaDeviceInfo[CbDevNo].DmaStartAddress.PciFunNo = \
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].FunNo;
}

BOOLEAN
DetectCpuIdDcaCapable (
    )
{  
    UINT32  RegEax, RegEbx, RegEcx, RegEdx;
      
    RegEcx = 0;
    AsmCpuid(0x9, &RegEax, &RegEbx, &RegEcx, &RegEdx);
    if((RegEax & 1))
      DEBUG ((EFI_D_INFO, "Core DCA Cap is enabled !\n"));
    else
      DEBUG ((EFI_D_INFO, "Core DCA Cap is disabled !\n"));
    return  (RegEax & 1) ? TRUE : FALSE;
}

BOOLEAN
DetectCpuDcaCapable (
    )
{  

  return DetectCpuIdDcaCapable();

}

VOID
ApicIdTagMapping (
    UINT64  CbBar
)
{
    UINT32  Value;
    UINT32  *Addr;

    
    Value = 0x01878685;
    Addr =  (UINT32 *)(UINTN)(CbBar + R_CB_BAR_APICID_TAG_MAP);
    *(volatile UINT32 *)Addr = Value;
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, (UINTN)(CbBar + R_CB_BAR_APICID_TAG_MAP), 1, &Value);

    Value = 0x01;
    Addr = (UINT32 *)(UINTN)(CbBar + R_CB_BAR_APICID_TAG_MAP+4);
    *(volatile UINT32 *)Addr = Value;
    S3BootScriptSaveMemWrite (S3BootScriptWidthUint32, (UINTN)(CbBar + R_CB_BAR_APICID_TAG_MAP+4), 1, &Value);
}


VOID
RwoRegisterProgramming (
    IIO_GLOBALS                                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  UINTN                       Addr;
  INTPIN_IIOCB_FUNC0_STRUCT   IntPinReg;

  // write back read value to lock RW-O INTPIN
  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ONLY_REGISTER_OFFSET(INTPIN_IIOCB_FUNC0_REG));
  IntPinReg.Data = PciExpressRead8(Addr);
  #ifdef    DE_SKU
  // We need to configure the CBDMA3.3 IRQ  --> Fun 0  = INT A , Fun 1 = INTB Fun 2 = INTC Fun 3 = INTD
  if( OemIdentifyLegacyCbDmaDevice(IioGlobalData,PciAddress.Bus, PciAddress.Device, PciAddress.Function)== FALSE)
    IntPinReg.Data= ((PciAddress.Function)% 4 )+1;
  #endif
  S3_PciExpressWrite8(IioGlobalData, Addr, IntPinReg.Data );
}



VOID
EarlyIoatInit ( 
    PCI_ROOT_BRIDGE_PCI_ADDRESS                    PciAddress,
    IIO_INIT_PHASE                                 Phase,
    IIO_GLOBALS                                    *IioGlobalData
)
{
  UINT8                               i;
  UINT8                               UncoreBus;
  UINT32                              Value32;
  UINT8                               channel;
#ifdef  DE_SKU
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT      PcuF3CAP1;
#endif

  for (i = 0 ; i < MaxIIO ; i++) {
    if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[i]) && IioGlobalData->IioVData.SocketPresent[i]) {
      break;
    }
  }

  if (i >= MaxIIO) return;

  if(Phase == IIOInitPhase1){
    if(OemIdentifyLegacyCbDmaDevice( IioGlobalData, PciAddress.Bus,PciAddress.Device, PciAddress.Function) == TRUE){
      UncoreBus = IioGlobalData->IioVData.SocketUncoreBusNumber[i];
      channel = PciAddress.Function;
#ifdef DE_SKU
      if (IioGlobalData->IioVData.CpuType == CPU_BDX) { // For BDX_DE family
        PcuF3CAP1.Data =  PciExpressRead32 ( PCI_LIB_ADDRESS (IioGlobalData->IioVData.SocketUncoreBusNumber[0],
                                                      PCU22_DEV_NUM,
                                                      PCU22_FUNC_NUM,
                                                      ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));
        //
        //NS Comms SKU could have CB3.3 fuse disabled in which case the legacy
        //CB3.2 DMA should not be hidden. Only DE (or future NS storage) SKUs where
        //CB3.3 DMA is always present, in which case the legacy CB3.2 DMA
        //should be hidden
        if (IioGlobalData->IioVData.SkuType == SKU_DE ||
            PcuF3CAP1.Bits.cbdma_disabled == 0) {

            // Mask the device as hide in UBOX device 16 function 7
            Value32 = PciExpressRead32 (PCI_PCIE_ADDR(UncoreBus, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
                (R_IIO_DEVHIDE_FUN0 + (channel * 4))));
            Value32 |= (1 << PciAddress.Device);
            PciExpressWrite32 (PCI_PCIE_ADDR(UncoreBus, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
                (R_IIO_DEVHIDE_FUN0 + (channel * 4))),Value32);

            // Mask the device  as hide in PCU device 30 function 3
            Value32 = PciExpressRead32 (PCI_PCIE_ADDR(UncoreBus, PCU22_DEV_NUM, PCU22_FUNC_NUM, \
                (R_IIO_PCU_DEVHIDE_FUN0 + (channel * 4))));
            Value32 |= (1 << PciAddress.Device);
            PciExpressWrite32 (PCI_PCIE_ADDR(UncoreBus, PCU22_DEV_NUM, PCU22_FUNC_NUM, \
                (R_IIO_PCU_DEVHIDE_FUN0 + (channel * 4))),Value32);
        }
      } // CPU_BDX
#else
      //
      // Hide those CB DMA channels which are forced as disabled through user setup option
      //
      //if ( !IioGlobalData->SetupData.Cb3DmaEn[(i * IOAT_TOTAL_FUNCS) + channel] ) {
       if (!IioGlobalData->SetupData.EnableIOAT){  // AptioV server override: Disable IOAT devices based on one setup control 
        // Mask the device as hide in UBOX device 11 function 3 for IVT and device 16 function 7 for HSX
        Value32 = PciExpressRead32 (PCI_PCIE_ADDR(UncoreBus, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
            (R_IIO_DEVHIDE_FUN0 + (channel * 4))));
        Value32 |= (1 << PciAddress.Device);
        S3_PciExpressWrite32 (IioGlobalData, PCI_PCIE_ADDR(UncoreBus, UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, \
            (R_IIO_DEVHIDE_FUN0 + (channel * 4))),Value32);
      } else if(PciAddress.Function == 0) {
          //save the CBDMA device base address internally
          SaveCbDmaDeviceBaseAddress(IioGlobalData, i, PciAddress);
      }
#endif //DE_SKU
    } // legacy CBDMA device
  } //phase 1
}

VOID
IioIoatInit( 
    IIO_GLOBALS                                    *IioGlobalData,
    UINT8                                          Iio,
    UINT8                                          CbDevNo,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{

  BOOLEAN                             EnableDCA;
  UINT8                               FunctionNum, DeviceNum;
  UINT8                               Bus;
  UINT16                              Value16;
  UINT32                              Value32;
  UINT64                              CbBar;
  UINT32                              CbBarTemp = 0;
  UINT8                               Socket;
  UINTN                               Addr;
  PCICMD_IIOCB_FUNC0_STRUCT           CmdReg;
  PCICMD_IIOCB_FUNC0_STRUCT           CmdRegOrg;
  DEVCFG_IIOCB_FUNC0_STRUCT           DevCfg;
  DEVCON_IIOCB_FUNC0_STRUCT           DevCon;
  UINT64                              BaseAddress = 0;
  EFI_STATUS                          Status;

  //DEBUG ((EFI_D_INFO, "IIO IOAT Init .... !\n"));

  Bus         = PciAddress.Bus;
  FunctionNum = PciAddress.Function;
  DeviceNum   = PciAddress.Device;
  Socket      = Iio;
  //
  // Verify that the function is present (e.g. not hidden)
  //
  if(DmaFunctionExist(PciAddress) == FALSE)
    return;

  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 0);
  CbBar = GetActualCbBAR(PciAddress);

  //DEBUG ((EFI_D_INFO, "IIO IOAT Init After Resource Assigned.... !\n"));

  //
  // Program DMA requirements based on setup questions
  //
  Addr = (Addr & 0xFFFFF000) | ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG);
  CmdReg.Data = PciExpressRead16(Addr);
  CmdRegOrg.Data = CmdReg.Data;

   //AptioV server override start: Program IOAT based on one setup control EnableIOAT
    //if (IioGlobalData->SetupData.Cb3DmaEn[(i * IOAT_TOTAL_FUNCS) + FunctionNum]) CmdReg.Bits.bme = 0x01;
    if (IioGlobalData->SetupData.EnableIOAT)CmdReg.Bits.bme = 0x01;
    else CmdReg.Bits.bme = 0x00;
    //AptioV server override end: Program IOAT based on one setup control EnableIOAT

  S3_PciExpressWrite16(IioGlobalData, Addr, CmdReg.Data);

  if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.NoSnoopSupported == TRUE){

    //
    // This bit enable is shared for all DMA engines and resides in Func 0 only
    //
    if (FunctionNum == 0x00 &&
        OemIdentifyLegacyCbDmaDevice(IioGlobalData, Bus, DeviceNum, FunctionNum) == TRUE) {
      Addr = (Addr & 0xFFFFF000) |
          IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevCfgRegister;

      DevCfg.Data = PciExpressRead16(Addr);
      //AptioV server override start: Program IOAT based on one setup control EnableIOAT
//      if (IioGlobalData->SetupData.Cb3DmaEn[(i * IOAT_TOTAL_FUNCS) + FunctionNum]) DevCfg.Bits.enable_no_snoop = 0x01; // A and B step are same bit
      if (IioGlobalData->SetupData.EnableIOAT) DevCfg.Bits.enable_no_snoop = 0x01; // A and B step are same bit      
      else DevCfg.Bits.enable_no_snoop = 0x00;
      //AptioV server override end: Program IOAT based on one setup control EnableIOAT

      S3_PciExpressWrite16(IioGlobalData, Addr, DevCfg.Data);
    }

    //
    // Setup DMA no-snoop hased on setup questions
    //
    Addr = (Addr & 0xFFFFF000) |
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevConRegister;
    DevCon.Data = PciExpressRead16(Addr);

    //AptioV server override start: Program DMA no snoop based on one setup control EnableNoSnoop
//    if (IioGlobalData->SetupData.Cb3NoSnoopEn[(i * IOAT_TOTAL_FUNCS) + FunctionNum]) {
    if (IioGlobalData->SetupData.EnableNoSnoop){
      DevCon.Bits.enable_no_snoop = 0x01;
    } else {
      DevCon.Bits.enable_no_snoop = 0x00;
    }
    S3_PciExpressWrite16(IioGlobalData, Addr, DevCon.Data);
  }

  if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.RelaxOrderSupported == TRUE){

    Addr = (Addr & 0xFFFFF000) |
        IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].DevConRegister;
    DevCon.Data = PciExpressRead16(Addr);
    //
    // HSD 3612867: defeature CBDMA relaxed ordering
    // HSD 3612733: Getting completion for a strongly ordered txn which does not exist!
    //
    if (IioGlobalData->SetupData.RelaxedOrdering) {
      DevCon.Bits.enable_relaxed_ordering = 1;
    } else {
      DevCon.Bits.enable_relaxed_ordering = 0;
    }
    S3_PciExpressWrite16(IioGlobalData, Addr, DevCon.Data);
  }
  
  //Program bits in MMIO using temporary 32 bit MMIO space and capture them all in boot script

  if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CBConfigCap.DcaSupported == TRUE){

    if(CbBar){  // Enable DCA only if original CbBar was non-zero

      //
      // 4987622 : Systems loop during S3 using BDX BIOS in HSX systems
      // 5001720 : CBDMA Bars not enumerated correctly
      //
      Status = OemAllocate32BitMmio(IioGlobalData, Socket, mDriverImageHandle, IIO_CBDMA_MMIO_ALLIGNMENT, IIO_CBDMA_MMIO_SIZE , &BaseAddress);

      if( EFI_ERROR(Status) ) //Need 32 bit MMIO for DCA enabling during S3
      {
        DEBUG((EFI_D_ERROR, "IIO IOAT Init: Cannot allocate temporary 32bit MMIO Socket: %d!!\n", Socket));
        ASSERT_EFI_ERROR(Status);
      }

      // clear upper 32bit of bar
      CbBarTemp = 0;
      Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum, ONLY_REGISTER_OFFSET(CB_BAR_1_IIOCB_FUNC0_REG));
      S3_PciExpressWrite32(IioGlobalData, Addr, CbBarTemp);

      // Set 32bit temporary Bar field and capture for the S3 boot script
      CbBarTemp = (UINT32)BaseAddress;
      Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum, ONLY_REGISTER_OFFSET(CB_BAR_0_IIOCB_FUNC0_REG));
      S3_PciExpressWrite32(IioGlobalData, Addr, CbBarTemp);
      //
      // Read command register
      //
      Addr = (Addr & 0xFFFFF000) | ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG);
      CmdReg.Data = PciExpressRead16(Addr);

      CmdRegOrg.Data = CmdReg.Data;
      CmdReg.Bits.mse = 0x01; // Enable Memory Space

      S3_PciExpressWrite16(IioGlobalData, Addr, CmdReg.Data);

      DEBUG ((EFI_D_INFO, "SKT=%d: CB BAR Temp =0x%lx, Value=0x%x.\n", Iio, CbBarTemp, *((UINT32*)(UINT64)CbBarTemp)));
      DEBUG ((EFI_D_INFO, "SKT=%d: CB BAR Actual =0x%lx.\n", Iio, CbBar));
      //
      // (DCA) Check if DCA is supported and enabled by CPU
      //
      EnableDCA = FALSE;

      //AptioV server override: DCA capability detected from Processor question DcaEnable
//      if (IioGlobalData->SetupData.Cb3DcaEn[i]){
        //
        // Enable CB DCA based on setup question if CPU is capable - Note that CPU will only enable DCA if all sockets support DCA
        //
        if(DetectCpuDcaCapable()) EnableDCA = TRUE;

        if (EnableDCA) {
          //
          // (DCA) Set "Enable Prefetch Hint on QPI"
          //
          Addr = (UINTN) (CbBarTemp + R_CB_BAR_CSI_CAP_ENABLE);
          Value16 = *(volatile UINT16 *)Addr;
          Value16 |= B_CB_BAR_CSI_CAP_ENABLE_PREFETCH_HINT;
          *(volatile UINT16 *)Addr = Value16;
          S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, Addr, 1, &Value16);

          //
          // (DCA) Set "Enable MemWR on PCIE"
          //
          Addr = (UINTN) (CbBarTemp + R_CB_BAR_PCIE_CAP_ENABLE);
          Value16 = *(volatile UINT16 *)Addr;
          Value16 |= B_CB_BAR_PCIE_CAP_ENABLE_MEM_WR;
          *(volatile UINT16 *)Addr = Value16;
          S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, Addr, 1, &Value16);
        }
        //AptioV server override: DCA capability detected from Processor question DcaEnable
//      } //end if Cb3DcaEn

      if (EnableDCA
          //
          // ESS_IP_START
          //
          //
          // ESS_IP_END
          //
      ) {
        //
        // Setup ApicId Tag Mapping
        //
        ApicIdTagMapping(CbBarTemp);

        //
        //  Enable port DCA
        //
        Addr =  (UINTN) (CbBarTemp + R_CB_BAR_CSI_DMACAPABILITY);
        Value32 = *(volatile UINT32 *)Addr;
        Value32 |= B_CB_BAR_CSI_DMACAP_DCA_SUP;
        *(volatile UINT32 *)Addr = Value32;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, Addr, 1, &Value32);

      } else {
        //
        //  Disable port DCA
        //
        Addr =  (UINTN) (CbBarTemp + R_CB_BAR_CSI_DMACAPABILITY);
        Value32 = *(volatile UINT32 *)Addr;
        Value32 &= ~B_CB_BAR_CSI_DMACAP_DCA_SUP;
        *(volatile UINT32 *)Addr = Value32;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, Addr, 1, &Value32);

      } //end if EnableDCA


      //
      // Restore Command input command register setting
      //
      Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum,
          ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG));
      S3_PciExpressWrite16(IioGlobalData, Addr, CmdRegOrg.Data);
      
      // Restore BAR 
      CbBarTemp = (UINT32)(CbBar >> 32) ;
      Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum, ONLY_REGISTER_OFFSET(CB_BAR_1_IIOCB_FUNC0_REG));
      S3_PciExpressWrite32(IioGlobalData, Addr, CbBarTemp);

      CbBarTemp = (UINT32)CbBar;  
      Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum, ONLY_REGISTER_OFFSET(CB_BAR_0_IIOCB_FUNC0_REG));
      S3_PciExpressWrite32(IioGlobalData, Addr, CbBarTemp);
  
    }
    else
      DEBUG((EFI_D_ERROR,"ERROR! CB BAR not programmed for %d,%d,%d\n", Bus, DeviceNum, FunctionNum));
  }

  // Finally free temporary memory space
  //
  // 5001720 : CBDMA Bars not enumerated correctly
  //
  //  OemFreeMemorySpace(BaseAddress, IIO_CBDMA_MMIO_SIZE);
  
  //
  // Lock Down RWO registers
  //
  RwoRegisterProgramming(IioGlobalData, PciAddress);

  //
  //perform any CB3.3 specific init if any
  //
  if(IioGlobalData->IioOutData.DMAhost[Iio].CbDmaDevice[CbDevNo].CB_VER == V_CB33_VERSION){
#ifdef  DE_SKU
    //  [4986906]
    // as per BDW CB3.3 CSpec, the following are the de-feature bits which
    // needs to be set before the lock......
    //  - NTB mode enable (NTBMDEN), MCast enable (MCEN), DIF enable (DIFEN),
    //    Crystal Beach Enable (CBEN),
    // and as per 4988027 - Raid 6 Enable (RAID6EN)
    // and as per 4988535 - Need to disable RAID in CBCTL register for BDX-DE CBDMA 3.3
    Addr = PCI_PCIE_ADDR(Bus, DeviceNum, FunctionNum, R_IIO_CB33_DEVCFG);
    Value32 = PciExpressRead32(Addr);
    Value32 |= (B_BWD_CB33_NTBMDEN + B_BWD_CB33_MCEN + B_BWD_CB33_DIFEN + B_BWD_CB33_CBEN);
    S3_PciExpressWrite32(IioGlobalData, Addr, Value32);
#endif
  }
}

VOID
IioIoatInitBootEvent(
  IIO_GLOBALS                                   *IioGlobalData,
  UINT8                                         Iio,
  IIO_INIT_PHASE                                Phase
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress;
  UINT8                                          CbDevNo, DmaFn;

  if(!IioGlobalData->IioVData.DmaDevice[Iio].TotalDmaControllers){
    DEBUG((EFI_D_INFO,"No IOAT devices present in socket %d\n", Iio));
    return;
  }
  for(CbDevNo = 0; CbDevNo < MAX_CBDMA_CONTROLLERS; CbDevNo++){
    PciAddress.Bus      = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciBusNo;
    PciAddress.Device   = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciDevNo;
    PciAddress.Function = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciFunNo;

    if(OemIdentifyCbDmaDevice( IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
      //
      // process each valid IOAT component
      //
      for(DmaFn = PciAddress.Function; DmaFn < IOAT_TOTAL_FUNCS; DmaFn++){
        PciAddress.Function = DmaFn;
        //
        // apply any CBDMA specific WA here
        //
        IioIoatWorkarounds(IioGlobalData, Iio, CbDevNo, PciAddress, Phase);

        if(Phase == IIOInitPhase1){
          if(DmaFn == IOAT_FUNC_START)
            DetermineCbDmaType(IioGlobalData, Iio, CbDevNo, PciAddress);
        }

        if(Phase == IIOInitPhase2){
          IioIoatInit(IioGlobalData, Iio, CbDevNo, PciAddress);
        }

        if(Phase == IIOInitPhase3){
          if(DmaFn == IOAT_FUNC_START)
            StoreDmaDeviceInfo(IioGlobalData, Iio, CbDevNo, PciAddress);
        }
      } //for each IOAT function
    }
  } //for each DMA device per socket

}

#ifndef DE_SKU
/*
 * BDX CBDMA WA for s328 : Lock TOR timeout with CBDMA and CPU traffic
 *  [cloned to 5001827]
 */
#define CHANCTRL    0x80  //16b
#define CHANCMD     0x84  //8b
#define DMACOUNT    0x86  //16b
#define CHANSTS_0   0x88  //32b
#define CHANSTS_1   0x8C  //32b
#define CHAINADDR_0 0x90  //32b
#define CHAINADDR_1 0x94  //32b
#define CHANERR     0xA8  //32b

#define CACHE_LINE  64
#define CACHE_ALIGN __declspec(align(CACHE_LINE))

#pragma pack(1)
UINT8 PACK[32];
volatile UINT8                   _ABA[MaxIIO][CACHE_LINE*IOAT_TOTAL_FUNCS];
volatile UINT8                   _BAB[MaxIIO][CACHE_LINE*IOAT_TOTAL_FUNCS];
UINT8 PACKZ[192*2];
#pragma pack()

UINT64                              *Descriptor[MaxIIO];

PCICMD_IIOCB_FUNC0_STRUCT           CmdRegOrg[IOAT_TOTAL_FUNCS];
volatile UINT8                   *pABA;
volatile UINT8                   *pBAB;


#pragma optimize( "", off )
#define RDTSC __rdtsc
#pragma intrinsic(RDTSC)
#define inline_cpu_instr  RDTSC()
#pragma warning( disable : 4189 )
VOID
IioIoatInitiateDummyTxn_epilogue(
    IIO_GLOBALS                                   *IioGlobalData,
    UINT8                                         DmaFn,
    UINT64                                        CbBar,
    UINT64                                        Descriptor
)
{
  volatile  UINT16                    *DmaCount;
  UINTN     serialize =  inline_cpu_instr;  //serializing

  DmaCount  = (UINT16*) (CbBar + DMACOUNT);

  *DmaCount = 1;
  //wait while channel is armed
  //  while ( (*Chnsts & 7) == 4);

 /* IioStall (IioGlobalData, 1000); //1ms delay
  if((*Chnsts & 7) == 4){
    DEBUG ((EFI_D_INFO, "\nERR issuing warm reset..."));
    IioWarmReset(IioGlobalData);
  }*/
}

VOID
IioIoatInitiateDummyTxn_prologue(
    IIO_GLOBALS                                   *IioGlobalData,
    UINT8                                         DmaFn,
    UINT64                                        CbBar,
    UINT64                                        Descriptor
)
{
  volatile UINT32                     *Chnsts;
  volatile UINT64                     *ChainAddr;
  UINTN     serialize =  inline_cpu_instr;  //serializing

  Chnsts    = (UINT32*) (CbBar + CHANSTS_0);
  ChainAddr = (UINT64*) (CbBar + CHAINADDR_0);

  *ChainAddr = Descriptor;
  //wait until channel gets armed
  while ( (*Chnsts & 7) != 4);
}

VOID
IioIoatInitiateDummyTxn_preset(
    IIO_GLOBALS                                   *IioGlobalData,
    UINT8                                         DmaFn,
    UINT64                                        CbBar,
    UINT64                                        Descriptor
)
{
  volatile  UINT32                    *Chnsts;
  volatile  UINT32                    *ChnErr;


  Chnsts    = (UINT32*) (CbBar + CHANSTS_0);
  ChnErr    = (UINT32*) (CbBar+ CHANERR);

  DEBUG ((EFI_D_INFO, "\tchannel %d Err status register (before clearing) = 0x%x\n", DmaFn, *ChnErr));
  *ChnErr = *ChnErr & 0xFFFFFFFF;
  //wait while channel is armed
  if( (*Chnsts & 7) == 4){
    DEBUG ((EFI_D_INFO, "\twait while channel %d is armed...\n", DmaFn));
    while ( (*Chnsts & 7) == 4);
  }
}

VOID
IioIoatInitiateDummyTxn_addendum(
    IIO_GLOBALS                                   *IioGlobalData,
    UINT8                                         DmaFn,
    UINT64                                        CbBar,
    UINT64                                        Descriptor
)
{
  volatile  UINT32                    *Chnsts;
  volatile  UINT32                    *ChnErr;
  volatile UINT64                     *ChainAddr;
  UINTN     serialize =  inline_cpu_instr;  //serializing

  ChainAddr = (UINT64*) (CbBar + CHAINADDR_0);
  Chnsts    = (UINT32*) (CbBar + CHANSTS_0);
  ChnErr    = (UINT32*) (CbBar+ CHANERR);
  DEBUG ((EFI_D_INFO, "\tCBDMA FN:%d - CHNSTS=0x%x, CHAINADDR=0x%lx, CHANERR=0x%x\n", DmaFn, *Chnsts, *ChainAddr, *ChnErr ));
  //DEBUG ((EFI_D_INFO, "\tchannel %d Err status register (post operation)= 0x%x\n", DmaFn, *ChnErr));
}
#pragma warning( default : 4189 )

VOID
EnableCbDmaChannel(
    IIO_GLOBALS                         *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress
)
{
  PCICMD_IIOCB_FUNC0_STRUCT           CmdReg;
  UINTN                               Addr;

  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 0);

  Addr = (Addr & 0xFFFFF000) | ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG);
  CmdReg.Data = PciExpressRead16(Addr);
  CmdRegOrg[PciAddress.Function].Data = CmdReg.Data;
  CmdReg.Bits.mse = 1;
  CmdReg.Bits.bme = 0x01;
  PciExpressWrite16(Addr, CmdReg.Data);
}

VOID
RestoreCbDmaChannel(
    IIO_GLOBALS                         *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress
)
{
  UINTN                               Addr;

  Addr = PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, 0);

  Addr = (Addr & 0xFFFFF000) | ONLY_REGISTER_OFFSET(PCICMD_IIOCB_FUNC0_REG);
  PciExpressWrite16(Addr, CmdRegOrg[PciAddress.Function].Data);
}

VOID
SetupCbDmaDescriptor(
  UINT8                       Iio,
  UINT8                       DmaFn,
  volatile UINT64             **Descriptor
)
{
  //Descriptor = (UINT64*)&pABA[Iio][DmaFn * CACHE_LINE];
  *Descriptor = (UINT64*) (pABA + (Iio * IOAT_TOTAL_FUNCS * CACHE_LINE) + (DmaFn * CACHE_LINE));
  DEBUG ((EFI_D_INFO, "\tCBDMA[%d] Descriptor address = 0x%lx\n", Iio, *Descriptor));

  (*Descriptor)[0] = 1;
  DEBUG ((EFI_D_INFO, "\t  A      = 0x%lx\n", (*Descriptor)[0]));

//  Descriptor[1] = (UINTN)&pBAB[Iio][DmaFn * CACHE_LINE];
  (*Descriptor)[1] = (UINTN) (pBAB + (Iio * IOAT_TOTAL_FUNCS * CACHE_LINE) + (DmaFn * CACHE_LINE));
  DEBUG ((EFI_D_INFO, "\t  A + 8  = 0x%lx\n", (*Descriptor)[1]));

//  Descriptor[2] = (UINTN)&pBAB[Iio][DmaFn * CACHE_LINE];
  (*Descriptor)[2] = (UINTN) (pBAB + (Iio * IOAT_TOTAL_FUNCS * CACHE_LINE) + (DmaFn * CACHE_LINE));
  DEBUG ((EFI_D_INFO, "\t  A + 16 = 0x%lx\n", (*Descriptor)[2]));

//  Descriptor[3] = (UINTN)&pABA[Iio][DmaFn * CACHE_LINE];
  (*Descriptor)[3] = (UINTN) (pABA + (Iio * IOAT_TOTAL_FUNCS * CACHE_LINE) + (DmaFn * CACHE_LINE));
  DEBUG ((EFI_D_INFO, "\t  A + 24 = 0x%lx\n", (*Descriptor)[3]));
}

VOID
AllocSysMem(
    IIO_GLOBALS                                   *IioGlobalData
)
{
  UINT8         *BaseAddress=0;
  EFI_STATUS    Status;

  Status = OemAllocateSystemMemory(
              IioGlobalData,
              mDriverImageHandle,
              6,                                         //Alignment: 2^6=64
              (CACHE_LINE*IOAT_TOTAL_FUNCS*MaxIIO) * 2,  //Length: twice the length of what was allocated for each channel for all sockets
              (EFI_PHYSICAL_ADDRESS)0x80000000,          // base : below 2G
              &BaseAddress);

  if(EFI_ERROR(Status)){
    DEBUG ((EFI_D_INFO, "\tUnable to dynamically allocate system memory\n"));
    pABA = (UINT8*) &_ABA[0][0];
    pBAB = (UINT8*) &_BAB[0][0];
    //ALIGN ON 64B BOUNDARY
    pABA += (UINTN)pABA % 64;
    pBAB += (UINTN)pBAB % 64;
  }
  else{
    pABA = (UINT8*)(BaseAddress);
    pBAB = (UINT8*)(BaseAddress + (CACHE_LINE*IOAT_TOTAL_FUNCS*MaxIIO));
  }
}

VOID
IioIoatFlushAll(
  IIO_GLOBALS                                   *IioGlobalData,
  UINT8                                         Iio,
  UINTN                                         Phase
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress;
  UINT8                               CbDevNo, DmaFn;
  UINT64                              CbBar;
  BOOLEAN                             Skip = FALSE;

  Skip = IioIoat_s328_WA_skip_GT1(IioGlobalData); //check biosSR5 to skip this for other channels

  for(CbDevNo = 0; CbDevNo < MAX_CBDMA_CONTROLLERS; CbDevNo++){
    PciAddress.Bus      = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciBusNo;
    PciAddress.Device   = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciDevNo;
    PciAddress.Function = IioGlobalData->IioVData.DmaDevice[Iio].DmaAddress[CbDevNo].PciFunNo;

    if(OemIdentifyCbDmaDevice( IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE){
      //
      // process each valid IOAT component
      //
      for(DmaFn = PciAddress.Function; DmaFn < IOAT_TOTAL_FUNCS; DmaFn++){
        PciAddress.Function = DmaFn;
        //
        // Verify that the function is present (e.g. not hidden)
        //
        if(DmaFunctionExist(PciAddress) == FALSE)
          return;

        if(Skip == TRUE && DmaFn)
          return;

        CbBar = GetActualCbBAR(PciAddress);
        if( CbBar ){

          if(Phase == 1){
            DEBUG ((EFI_D_INFO, "IIO IOAT s328 WA for CB[%d:%d:%d]\n", PciAddress.Bus, PciAddress.Device, PciAddress.Function));
            DEBUG ((EFI_D_INFO, "CB BAR %d : 0x%lx\n", DmaFn, CbBar));

            EnableCbDmaChannel(IioGlobalData, PciAddress);

            SetupCbDmaDescriptor( Iio, DmaFn, &Descriptor[Iio]);

            //initiate dummy transaction
            //DEBUG ((EFI_D_INFO, "\t initiate txn..."));
            IioIoatInitiateDummyTxn_preset(IioGlobalData, DmaFn, CbBar, (UINTN)Descriptor[Iio]);
          }
          if(Phase == 2){
            IioIoatInitiateDummyTxn_prologue(IioGlobalData, DmaFn, CbBar, (UINTN)Descriptor[Iio]);
          }

          if(Phase == 3){
            IioIoatInitiateDummyTxn_epilogue(IioGlobalData, DmaFn, CbBar, (UINTN)Descriptor[Iio]);
          }
          if(Phase == 4){
            IioIoatInitiateDummyTxn_addendum(IioGlobalData, DmaFn, CbBar, (UINTN)Descriptor[Iio]);
            //DEBUG ((EFI_D_INFO, "done\n"));
            //restore the channel
            RestoreCbDmaChannel(IioGlobalData, PciAddress);
          }
        }
      }
    }
  }
}
#pragma optimize( "", on )

VOID
IioIoatReset(
    IIO_GLOBALS   *IioGlobalData
)
{
  UINTN     serialize =  inline_cpu_instr;  //serializing
  UINT32    ScratchReg;
  UINTN     ADDR5 = PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0],
                        UBOX_MISC_DEV_NUM,
                        UBOX_MISC_FUNC_NUM,
                        ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD5_UBOX_MISC_REG));
  ScratchReg = PciExpressRead32(ADDR5);
  if(ScratchReg & 0x80000000)   //IOAT reset already performed, exit
    return;
  DEBUG ((EFI_D_INFO, "Issuing warm reset to reset IOAT...\n"));
  ScratchReg |= 0x80000000;
  PciExpressWrite32(ADDR5, ScratchReg);

  serialize =  inline_cpu_instr;  //serializing
  IoWrite8(0xCF9, 6);
  while(ScratchReg |= 0x80000000);
//  IioWarmReset(IioGlobalData);
}

BOOLEAN
Apply_s328_WA(
    IIO_GLOBALS   *IioGlobalData
)
{
  UINT32    ScratchReg;
  UINTN     ADDR5 = PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0],
      UBOX_MISC_DEV_NUM,
      UBOX_MISC_FUNC_NUM,
      ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD5_UBOX_MISC_REG));
  ScratchReg = PciExpressRead32(ADDR5);
  if(ScratchReg & 0x80000000)   //IOAT reset already performed, exit
    return FALSE;
  else
    return TRUE;
}

BOOLEAN
IioIoat_s328_WA_skip_GT1(
    IIO_GLOBALS   *IioGlobalData
)
{
#if 0
  UINT32    ScratchReg;
  UINTN     ADDR5 = PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0],
      UBOX_MISC_DEV_NUM,
      UBOX_MISC_FUNC_NUM,
      ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD5_UBOX_MISC_REG));
  ScratchReg = PciExpressRead32(ADDR5);
  if(ScratchReg & 0x40000000)   //IOAT reset already performed, exit
    return TRUE;
  else
    return FALSE;
#else
  return TRUE;
#endif
}

VOID
DisableSystemInterrupts()
{
  PciExpressWrite16(PCI_PCIE_ADDR(0, 22, 0, 4), 0x400);  //disable HECI#1
  PciExpressWrite16(PCI_PCIE_ADDR(0, 22, 1, 4), 0x400);  //disable HECI#2
  PciExpressWrite16(PCI_PCIE_ADDR(0, 22, 2, 4), 0x400);  //disable IDE-Redirection (IDE-R)
  PciExpressWrite16(PCI_PCIE_ADDR(0, 22, 3, 4), 0x400);  //disable Keyboard and Text (KT) Redirection
  PciExpressWrite16(PCI_PCIE_ADDR(0, 20, 0, 4), 0x400);  //disable USB 3.0 xHCI Controller
  PciExpressWrite16(PCI_PCIE_ADDR(0, 20, 1, 4), 0x400);  //disable USB xHCI virtual HC #1
  PciExpressWrite16(PCI_PCIE_ADDR(0, 26, 0, 4), 0x400);  //disable EHCI#1
  PciExpressWrite16(PCI_PCIE_ADDR(0, 26, 4, 4), 0x400);  //disable EHCI#1
  PciExpressWrite16(PCI_PCIE_ADDR(0, 29, 0, 4), 0x400);  //disable EHCI#2
  PciExpressWrite16(PCI_PCIE_ADDR(0, 29, 4, 4), 0x400);  //disable EHCI#2
  PciExpressWrite32(PCI_PCIE_ADDR(0, 31, 0, 0x94), 0);   //disable SMI generation due to USB KBD/MSE
  gBS->RaiseTPL(TPL_HIGH_LEVEL);
}

VOID
IioIoat_s328_WA(
    IIO_GLOBALS   *IioGlobalData
)
{
  UINT8 Iio, Phase;

  if(Apply_s328_WA(IioGlobalData) == TRUE){
    AllocSysMem(IioGlobalData);
    DisableSystemInterrupts();
    for (Phase = 1; Phase <= 4; Phase++) { // AptioV Grangeville Override: Fix for CPP Check Error EIP#277300 
      for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
        if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

        if(!IioGlobalData->IioVData.DmaDevice[Iio].TotalDmaControllers){
          DEBUG((EFI_D_INFO,"No IOAT devices present in socket %d\n", Iio));
          continue;
        }

        IioIoatFlushAll(IioGlobalData, Iio, Phase);
      } //loop for all sockets
    } //for all phases
    IioIoatReset(IioGlobalData);
  }

}
#endif  //DE_SKU
