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

    OemIioInit.c

Abstract:

    Oem Hooks file for IioInit Module

--*/
#include <Library/OemIioInit.h>
#include "OemIioInitCommon.h"

#ifndef IA32
#endif // IA32
#include "PchRegs.h"
#include "Platform.h"

#define EFI_INTEL_OEM_MISC_CMD      0xFF

//
// Gen3 CTLe Table
//
IIO_CTLEP  Gen3CTLEpInfo[] = {
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0xFF,   PCIE_PORT_0_DFX_DEV,     PCIE_PORT_0_DFX_FUNC,    0xFFFFFC00,        0x0,            0x3de,             0x0,             0xFF},         //for port 0
  { 0xFF,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0xf7bde,           0x0,             0xFF},         //for port 1A, 1B
  { 0xFF,   PCIE_PORT_2A_DFX_DEV,    PCIE_PORT_2A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,        0x3de,           0xFF},         //for port 2A, 2B, 2C , 2D
  { 0xFF,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,        0x3de,           0xFF}          //for port 3A, 3B, 3C , 3D
};

//
// Gen3 CTLe Table for B0 stepping or beyond
//
IIO_CTLEP  Gen3CTLEpInfo_BX[] = {
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0xFF,   PCIE_PORT_0_DFX_DEV,     PCIE_PORT_0_DFX_FUNC,    0xFFFFFC00,        0x0,            0x18c,             0x0,             0xFF},         //for port 0
  { 0xFF,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0x6318c,           0x0,             0xFF},         //for port 1A, 1B
  { 0xFF,   PCIE_PORT_2A_DFX_DEV,    PCIE_PORT_2A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x18c6318c,        0x18c,           0xFF},         //for port 2A, 2B, 2C , 2D
  { 0xFF,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x18c6318c,        0x18c,           0xFF}          //for port 3A, 3B, 3C , 3D
 };

//
// Gen2 CTLe Table
//
IIO_CTLEP  Gen2CTLEpInfo[] = {
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0xFF,   PCIE_PORT_0_DFX_DEV,     PCIE_PORT_0_DFX_FUNC,    0xFFFFFC00,        0x0,            0x3de,             0x0,             0xFF},         //for port 0
  { 0xFF,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0xf7bde,           0x0,             0xFF},         //for port 1A, 1B
  { 0xFF,   PCIE_PORT_2A_DFX_DEV,    PCIE_PORT_2A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,        0x3de,           0xFF},         //for port 2A, 2B, 2C , 2D
  { 0xFF,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,        0x3de,           0xFF}          //for port 3A, 3B, 3C , 3D
};

//
// Gen1 CTLe Table
//
IIO_CTLEP  Gen1CTLEpInfo[] = {
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0xFF,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0x0,               0x0,             0xFF},         //for S0 port 1A, 1B
  { 0xFF,   PCIE_PORT_2A_DFX_DEV,    PCIE_PORT_2A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x0,               0x0,             0xFF},         //for S0 port 2A, 2B, 2C , 2D
  { 0xFF,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x0,               0x0,             0xFF}          //for S0 port 3A, 3B, 3C , 3D
};

VOID
IioStall (
  IIO_GLOBALS *IioGlobalData,
  UINT32   UsDelay
  )
/*++

Routine Description:

  Delay time in 1 us units

Arguments:

  usDelay - number of us units to delay

Returns:

  VOID

--*/
{
  UINT32  StartCount, StopCount, Delay;


  StartCount = IioIoRead32(IioGlobalData, (UINT16)(IioGlobalData->IioVData.PmBase + 0x8)) & 0xFFFFFF;

  while(1) {
    StopCount = IioIoRead32(IioGlobalData, (UINT16)(IioGlobalData->IioVData.PmBase + 0x8)) & 0xFFFFFF;
    if (StartCount > StopCount) {
      StartCount |= 0xFF000000;
    }
    Delay = (StopCount - StartCount) * 1000U / 3579U;
    if (Delay > UsDelay) {
      break;
    }
  }
}


VOID
DeducePEXPHideFromIouBif (
  IIO_GLOBALS     *IioGlobalData,
  UINT32          IOUx,
  UINT32          IIOn,
  UINT8           CurrentIOUConfigValue,
  UINT8           *PXPHide,
  UINT8           *HidePEXPMenu
  );

VOID
OemGetIioPlatformInfo(
  IIO_GLOBALS *IioGlobalData
)
{
  EFI_PLATFORM_INFO             *mPlatformInfo;
  EFI_HOB_GUID_TYPE             *GuidHob;

  GuidHob    = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  mPlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (mPlatformInfo);

  //
  // Update Other variables required for IIO Init
  //
  IioGlobalData->IioVData.PlatformType   = mPlatformInfo->BoardId;
  IioGlobalData->IioVData.PcieRiser1Type = mPlatformInfo->PcieRiser1Type;
  IioGlobalData->IioVData.PcieRiser2Type = mPlatformInfo->PcieRiser2Type;
  IioGlobalData->SetupData.DmiVc1        = mPlatformInfo->DmiVc1;
  IioGlobalData->SetupData.DmiVcp        = mPlatformInfo->DmiVcp;
  // TODO: Check logic for DmiVcm
  IioGlobalData->SetupData.DmiVcm        = mPlatformInfo->DmiVcm;

}

VOID
OemIioPreLinkDataInitPerSocket(
  IIO_GLOBALS  *IioGlobalData,
  UINT8         Iio
  )
{
  UINT8   j;
  UINT8   BusBase;
  UINT8   Device = 0;
  UINT8   Function = 0;

  IioCopyMem(&IioGlobalData->PreLinkData.PcieInfo, &PcieInfoDefaults, sizeof(INTEL_DMI_PCIE_INFO));

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    //
    // Always Configure all ports
    //
    IioGlobalData->IioOutData.PciePortConfig[(Iio*NUMBER_PORTS_PER_SOCKET)+j] = 0x01;

    Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

    //
    // If the Port is not present (e.g. Vendor ID is not Intel, then the port doesnt exist and should not be configured)
    //  This condition could exist on some SKUs where certain IOUs are not present or if some external entity has disabled a port
    //

    if (IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, ONLY_REGISTER_OFFSET(VID_IIO_PCIEDMI_REG))) != 0x8086)
      IioGlobalData->IioOutData.PciePortConfig[(Iio*NUMBER_PORTS_PER_SOCKET)+j] = 0x00;
    else
      IioGlobalData->IioOutData.PciePortPresent[(Iio*NUMBER_PORTS_PER_SOCKET)+j] = 0x01;
  }
}

/*++

Routine Description:

  Initialize the Global Data Structure for this module with 
  Refresh specific members.

Arguments:
  IioGlobalData - Ptr to the global data.
  SetupData     - Ptr to the data from the setup variable.


Returns:

  EFI_SUCCESS               All other error conditions encountered result in an ASSERT.
                            IioGlobalData BDX members are initialized upon return.
--*/

/*++

Routine Description:

  Initialize the Global Data Structure for this module with 
  Refresh specific members.

Arguments:
  IioGlobalData - Ptr to the global data.
  SetupData     - Ptr to the data from the setup variable.


Returns:

  EFI_SUCCESS               All other error conditions encountered result in an ASSERT.
                            IioGlobalData BDX members are initialized upon return.
--*/


STATIC
EFI_STATUS
OemVariableGetCurrentBdx (
  IN OUT IIO_GLOBALS  *IioGlobalData,
  IN SYSTEM_CONFIGURATION *SetupData
  )
{
  UINT8 i;

  //
  // Check params for sanity
  //

  if ((IioGlobalData == NULL) || (SetupData == NULL)) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  }

  for (i = 0; i < MAX_TOTAL_PORTS; i++) {

    IioGlobalData->BdxSetupData.Ecrc[i] = SetupData->Ecrc[i];
    IioGlobalData->BdxSetupData.PciePortLinkMaxWidth[i] = SetupData->PciePortLinkMaxWidth[i];
  }

  for (i = 0; i < MAX_CPU_SOCKET; i++) {
    IioGlobalData->BdxSetupData.Iou0Np_Pf[i] = SetupData->Iou0Np_Pf[i];
    IioGlobalData->BdxSetupData.Iou1Np_Pf[i] = SetupData->Iou1Np_Pf[i];
    IioGlobalData->BdxSetupData.Iou2Np_Pf[i] = SetupData->Iou2Np_Pf[i];

    
    IioGlobalData->BdxSetupData.VPP_INVERT[i] = SetupData->VPP_INVERT[i];

    IioGlobalData->BdxSetupData.NtbPpdAutoConfig[i] = SetupData->NtbPpdAutoConfig[i];
    IioGlobalData->BdxSetupData.NtbXlinkCtlAutoConfig[i] = SetupData->NtbXlinkCtlAutoConfig[i];
  }

#ifdef DE_SKU

  IioGlobalData->BdxSetupData.Disable_SC_Cb3Dma = SetupData->Disable_SC_Cb3Dma;
  IioGlobalData->BdxSetupData.Disable_SC_GbE = SetupData->Disable_SC_GbE;
  IioGlobalData->BdxSetupData.SC_GbE_PF0 = SetupData->SC_GbE_PF0;
  IioGlobalData->BdxSetupData.SC_GbE_PF1 = SetupData->SC_GbE_PF1;

  IioGlobalData->BdxSetupData.Disable_SC_CPM = SetupData->Disable_SC_CPM;
  IioGlobalData->BdxSetupData.Disable_SC_GbE1 = SetupData->Disable_SC_GbE1;
  IioGlobalData->BdxSetupData.SC_GbE1_PF0 = SetupData->SC_GbE1_PF0;
  IioGlobalData->BdxSetupData.SC_GbE1_PF1 = SetupData->SC_GbE1_PF1;
#endif // DE_SKU

  IioGlobalData->BdxSetupData.PCIe_Enable_SSD_SUPPORT = SetupData->PCIe_Enable_SSD_SUPPORT;
  IioGlobalData->BdxSetupData.IioLinkTrainPhase = SetupData->IioLinkTrainPhase;
  IioGlobalData->BdxSetupData.Apply_s328_cbdma_WA = SetupData->Apply_s328_cbdma_WA;

  return EFI_SUCCESS;

}


VOID
OemVariableGetCurrent (
  IIO_GLOBALS  *IioGlobalData
  )
/*++

Routine Description:

  Initialize the Global Data Structure for this module if any additional 
  changes required in phase. 

Arguments:


Returns:

  VOID              All other error conditions encountered result in an ASSERT
                    IioGlobalData is completely initialized for use by module code!
--*/
{
  EFI_STATUS            Status;
  SYSTEM_CONFIGURATION  SetupData;
  SETUP_INFO            SetupInfo;
  //
  // Obtain the system configuration data
  //
  Status  = GetSystemVariable(&SetupData );

  //
  // Update the global data structure with the system configuration data
  //

  IioGlobalData->IioVData.PchIoApicBase = PCH_IOAPIC_ADDRESS;
  IioGlobalData->IioVData.PmBase = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(0, PCH_LPC_PCI_DEVICE_NUMBER, PCH_LPC_PCI_FUNCTION_NUMBER, 0x40)) & ~(1 << 0);
  IioGlobalData->IioVData.PchRcbaAddress = OemGetPchRootComplexBaseAddress(IioGlobalData);
  if (EFI_ERROR (Status)) {

  IioCopyMem (&(IioGlobalData->SetupData), &IioConfigDefault, sizeof (IIO_CONFIG));

  //
  // Setup the BDX defaults.
  //

  IioCopyMem (&(IioGlobalData->BdxSetupData), &BdxIioConfigDefault, sizeof (BDX_IIO_CONFIG));


    
    IioGlobalData->IioVData.Serr = 0;
    IioGlobalData->IioVData.Perr = 0;
    IioGlobalData->IioVData.LockChipset = 1;
    IioGlobalData->IioVData.LegacyVgaSoc = 0;
    IioGlobalData->IioVData.DevPresIoApicIio0 = 1;
    IioGlobalData->IioVData.DevPresIoApicIio1 = 1;
    IioGlobalData->IioVData.DevPresIoApicIio2 = 1;
    IioGlobalData->IioVData.DevPresIoApicIio3 = 1;
    IioGlobalData->IioVData.WheaSupport       = 0;
    IioGlobalData->IioVData.IioErrorEn        = 0;
  } else {
    // get a copy of IIO_UDS.SETUP_INFO data structure from the system configuration data
    CopyMem (
      &SetupInfo,
      &(SetupData.VariableTag),
      sizeof (SETUP_INFO)
      );
    //
    // refresh the IIO_GLOBALS.SETUP_INFO.IIO_CONFIG global data strucutre with the configuration data
    //
    CopyMem (
      &(IioGlobalData->SetupData),
      &SetupInfo.Config,
      sizeof (IIO_CONFIG)
      );

    //
    // Insert the BDX additions into IioGlobalData.
    //

    Status = OemVariableGetCurrentBdx (IioGlobalData, &SetupData);
    ASSERT_EFI_ERROR (Status);

    IioGlobalData->IioVData.Serr = SetupData.PropagateSerr;
    IioGlobalData->IioVData.Perr = SetupData.PropagatePerr;
    IioGlobalData->IioVData.LockChipset = SetupData.LockChipset;
    IioGlobalData->IioVData.LegacyVgaSoc = SetupData.LegacyVgaSoc;
    IioGlobalData->IioVData.DevPresIoApicIio0 = SetupData.DevPresIoApicIio0;
    IioGlobalData->IioVData.DevPresIoApicIio1 = SetupData.DevPresIoApicIio1;
    IioGlobalData->IioVData.DevPresIoApicIio2 = SetupData.DevPresIoApicIio2;
    IioGlobalData->IioVData.DevPresIoApicIio3 = SetupData.DevPresIoApicIio3;
    IioGlobalData->IioVData.WheaSupport       = SetupData.WheaSupport;
    IioGlobalData->IioVData.IioErrorEn        = SetupData.IohErrorEn;
  }
}


VOID
OemVariableInitPerSocket (
  IIO_GLOBALS     *IioGlobalData,
    UINT8        IioIndex
  )

/*++

Routine Description:

  Initialize the Global Data Structure for this module if any additional 
  changes required in phase. 

Arguments: 
          IioGlobalData  Pointer to IIO_GLOBALS
          Iio            Iio Index
 
Returns:

  VOID              All other error conditions encountered result in an ASSERT
                    IioGlobalData is completely initialized for use by module code!
--*/
{
  UINT8                         PortIndex;
  UINT32                        Data32;
  UINT16 GpioBase=0;  
  UINT32 Data=0;     
  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //

  for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
    IioGlobalData->SetupData.PEXPHIDE[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] = 0;
    IioGlobalData->SetupData.HidePEXPMenu[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] = 0;
  }

  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping > B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)){
    IioGlobalData->SetupData.Hide_WA_4167453 = 1; // Hide WA_4167453 if HSX C0 or beyond                                                 
  } else{
    IioGlobalData->SetupData.Hide_WA_4167453 = 0;  
  }

/*
  Platform list for reference
  
  TypeMayanCityCRB = 0x00,
  TypeMayanCitySRP,
  TypeMayanCitySRPDDR3,
  TypeYakimaCityCRB = 0x03, 
  TypeYakimaCitySRP, 
  TypeYakimaCitySTHI,
  TypeMayanCityCRBDDR3, 
  TypeAztecCityCRBSRP = 0x07,
  TypeAztecCitySTHI,
  TypeIncaCityCRB = 0x0A,
  TypeIncaCitySRP,
  TypeHalfWidthCitySRP = 0x0C,
  TypeKahunaDVP2SPbg = 0x0F,
  TypeKahunaDVP2S,
  TypeArandasVVP2S = 0x11, 
  TypeArandasVVP4S,
  TypeHedtSKU1CRB = 0x13,
  TypeHedtEv,
  TypeHedtDDR3,
  TypeSaxtonRock = 0x18,
  TypeEchoRock,
  TypeBalanceRock,
  TypeEmeraldPoint,
  TypeWildcatPass = 0x32
*/
//@tbd-grt: Many changes as compared to Brklnd.Confirm for GRT.

  switch(IioGlobalData->IioVData.PlatformType)  {
    case TypeEmeraldPoint: 
    // Source: Emerald Point schematics version dated Oct 2011 page 18

       if (IioIndex == 0) {
/*
                         IIO port# * B:D:F * BIOS port# * slot#
      CPU0:
      IOU2: --  x8        1B * 0:1:1 * P2  * SE_7:4  | 1A * 0:1:0 * P1  * SE_3:0
      IOU0: --  -- -- x16 2D * 0:2:3 * P6  * MidPlane_15:12   | 2C * 0:2:2 * P5  * MidPlane_11:8   | 2B * 0:2:1 * P4  * MidPlane_7:4 | 2A * 0:2:0 * P3  * MidPlane_3:0
      IOU1: --  -- -- x8 3D * 0:3:3 * P10 * NTB_15:12 | 3C * 0:3:2 * P9  * NTB_11:8  | 3B * 0:3:1 * P8  * NTB_7:4  | 3A * 0:3:0 * P7  * NTB_3:0
*/
    //
    // Set bifurcation control regs
    //
    DEBUG((EFI_D_ERROR, "Setting the bifurcation for Socket0\n"));
    if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO){     // If no user override
       DEBUG((EFI_D_ERROR, "IOU2 --> IIO_BIFURCATE_xxx8\n"));              // 
       IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above
    }

    if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO){
      DEBUG((EFI_D_ERROR, "IOU1 --> IIO_BIFURCATE_xxxxxx16\n"));
      IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
    }

    // In Emerald Point on canister 1 port2a is x16 and canister 2 port 2a is (x8, x4, x4)
    // GPIO34 High = Canister 1 and Low = Canister 2
    GpioBase = (UINT16)PciExpressRead16(PCI_PCIE_ADDR(0, 0x1F, 0, 0x48)) & 0xFF00;
    DEBUG((EFI_D_ERROR, "GpioBase = 0x%x\n",GpioBase));
    Data = IoRead32(GpioBase+0x38);
    DEBUG((EFI_D_ERROR, "GPIO34 Data = 0x%x\n",Data));
    if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
      if(Data & (1 << 2)) {
        DEBUG((EFI_D_ERROR, "Canister 1 -->IOU0 --> IIO_BIFURCATE_xxx16\n"));
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxxxxx16;
        IioGlobalData->SetupData.SLOTIMP[3]   = 1;
        IioGlobalData->SetupData.SLOTPSP[3]   = 6;   
       //IioGlobalData->SetupData.Canister = 0;
       // IioGlobalData->SetupData.Canister = 1;
      } else{
        DEBUG((EFI_D_ERROR, "Canister 2 -->IOU1 --> IIO_BIFURCATE_xxx8x4x4\n"));
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_x4x4xxx8;

        IioGlobalData->SetupData.SLOTIMP[3]   = 1;
        IioGlobalData->SetupData.SLOTPSP[3]   = 6;      

        IioGlobalData->SetupData.SLOTIMP[5]   = 1;
        IioGlobalData->SetupData.SLOTPSP[5]   = 7;      

        IioGlobalData->SetupData.SLOTIMP[6]   = 1;
        IioGlobalData->SetupData.SLOTPSP[6]   = 8;      
        //IioGlobalData->SetupData.Canister = 1;
        //IioGlobalData->SetupData.Canister = 0;
      }
    }

    // Slot E
    IioGlobalData->SetupData.SLOTIMP[1]   = 1;
    IioGlobalData->SetupData.SLOTPSP[1]   = 5;      

    }
     if (IioIndex == 1) {
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU1:
        IOU2: --  x8        1B * 80:1:1 * P2  * SD_7:4   | 1A * 80:1:0 * P1 * SD_3:0
        IOU0: --  -- -- x16 2D * 80:2:3 * P6  * SB_15:12 | 2C * 80:2:2 * P5  * SB_11:8  | 2B * 80:2:1 * P4  * SC_7:4  | 2A * 80:2:0 * P3  * SC_3:0
        IOU1: --  -- -- x16 3D * 80:3:3 * P10 * SA_15:12 | 3C * 80:3:2 * P9  * SA_11:8  | 3B * 80:3:1 * P8  * SA_7:4  | 3A * 80:3:0 * P7  * SA_3:0
*/
    if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO){     // If no user override
         DEBUG((EFI_D_ERROR, "IOU2 --> IIO_BIFURCATE_xxx8\n"));              // 
         IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;        // Use value from chart above
    }
    if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO){
        DEBUG((EFI_D_ERROR, "IOU1 --> IIO_BIFURCATE_xxxxxx16\n"));
        IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxxxxx16;
    }
    if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO){
        DEBUG((EFI_D_ERROR, "IOU0 --> IIO_BIFURCATE_xxx8xxx8\n"));
        IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxx8xxx8;
    }


    // Slot D
    IioGlobalData->SetupData.SLOTIMP[11 + 1]   = 1;
    IioGlobalData->SetupData.SLOTPSP[11 + 1]   = 4;      

    // Slot B
    IioGlobalData->SetupData.SLOTIMP[11 + 3]   = 1;
    IioGlobalData->SetupData.SLOTPSP[11 + 3]   = 2;      

    // Slot C
    IioGlobalData->SetupData.SLOTIMP[11 + 5]   = 1;
    IioGlobalData->SetupData.SLOTPSP[11 + 5]   = 3;      

    // Slot A
    IioGlobalData->SetupData.SLOTIMP[11 + 7]   = 1;
    IioGlobalData->SetupData.SLOTPSP[11 + 7]   = 1;      
    //
    // Hide the root ports whose lanes are assigned preceding ports
    //

    }

    //
    // Hide the root ports whose lanes are assigned preceding ports
    //
    DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
      IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

    DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
      IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

    DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
      IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

    break;


    case TypeMayanCityCRB:
    case TypeMayanCityCRBDDR3:
    // Source: Mayan City Architecture Block Diagram (CRB) Rev 0.62
      if (IioIndex == 0) {

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: --  x8        1B * 0:1:1 * P2  * S11_7:4  | 1A * 0:1:0 * P1  * S11_3:0
        IOU0: --  x8 -- x8  2D * 0:2:3 * P6  * TW_7:4   | 2C * 0:2:2 * P5  * TW_3:0   | 2B * 0:2:1 * P4  * S10_7:4 | 2A * 0:2:0 * P3  * S10_3:0
        IOU1: --  -- -- x16 3D * 0:3:3 * P10 * S1_15:12 | 3C * 0:3:2 * P9  * S1_11:8  | 3B * 0:3:1 * P8  * S1_7:4  | 3A * 0:3:0 * P7  * S1_3:0
*/

        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
         IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

        // 
        // VPP invert
        //
        IioGlobalData->BdxSetupData.VPP_INVERT[0] = 1;   // socket 0     

        //
        // Slot Mappings
        //
        // Slot 1
        IioGlobalData->SetupData.SLOTIMP[7]   = 1;
        IioGlobalData->SetupData.SLOTPSP[7]   = 1;      

        // Slot 11
        IioGlobalData->SetupData.SLOTIMP[1]   = 1;
        IioGlobalData->SetupData.SLOTPSP[1]   = 11;  

        IioGlobalData->SetupData.VppEnable[1]  = 1;
        IioGlobalData->SetupData.VppPort[1]    = 0;   //VPP Port reflects the PCA95x IO Port
        IioGlobalData->SetupData.VppAddress[1] = 0;   //Refer to Sch 0v610 sheet 109

        IioGlobalData->SetupData.SLOTHPCAP[1]  = 1;
        IioGlobalData->SetupData.SLOTPIP[1]    = 1;
        IioGlobalData->SetupData.SLOTAIP[1]    = 1;
        IioGlobalData->SetupData.SLOTMRLSP[1]  = 1; //MRL Present on Slot 0xA. J172
        IioGlobalData->SetupData.SLOTPCP[1]    = 1;
        IioGlobalData->SetupData.SLOTABP[1]    = 1;

        // Twinville
        IioGlobalData->SetupData.SLOTIMP[5]   = 1;
        IioGlobalData->SetupData.SLOTPSP[5]   = 15;

     }

     if (IioIndex == 1){ 
  /*
                             IIO port# * B:D:F * BIOS port# * slot#
          CPU1:
          IOU2: x4  x4        1B * 80:1:1 * P2  * S3_7:4   | 1A * 80:1:0 * P1 * S3_3:0
          IOU0: --  -- -- x16 2D * 80:2:3 * P6  * S4_15:12 | 2C * 80:2:2 * P5  * S4_11:8  | 2B * 80:2:1 * P4  * S4_7:4  | 2A * 80:2:0 * P3  * S4_3:0
          IOU1: --  x8 -- --  3D * 80:3:3 * P10 * S6_7:4   | 3C * 80:3:2 * P9  * S6_3:0   | 3B * 80:3:1 * P8            | 3A * 80:3:0 * P7
  */
        if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxx8xxx8;    // can also be IIO_BIFURCATE_xxx8x4x4 since 3A-3B not used

        // 
        // VPP invert
        //
        IioGlobalData->BdxSetupData.VPP_INVERT[1] = 1;   // socket 1 

        //
        // Slot Mappings
        //
        // Slot 3
        IioGlobalData->SetupData.SLOTIMP[11 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 1] = 3;

        // Slot 4
        IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 3] = 4;

        // Slot 6
        IioGlobalData->SetupData.SLOTIMP[11 + 9] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 9] = 6;

      }
      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      //
      // Also hide the ports which are not used
      //
      if (IioIndex == 1) {
        IioGlobalData->SetupData.PEXPHIDE[11 + 7] =     1;
        IioGlobalData->SetupData.PEXPHIDE[11 + 8] =     1;

        IioGlobalData->SetupData.HidePEXPMenu[11 + 7] = 1;
        IioGlobalData->SetupData.HidePEXPMenu[11 + 8] = 1;
      }
      
      break;

    case TypeMayanCitySRP:
    case TypeMayanCitySRPDDR3:
    // Source: Mayan City Architecture Block Diagram (SRP-EV) Rev 0.61

      if (IioIndex == 0){

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: --  x8        1B * 0:1:1 * P2  * S11_7:4  | 1A * 0:1:0 * P1  * S11_3:0
        IOU0: --  x8 -- x8  2D * 0:2:3 * P6  * TW_7:4   | 2C * 0:2:2 * P5  * TW_3:0   | 2B * 0:2:1 * P4  * S10_7:4 | 2A * 0:2:0 * P3  * S10_3:0
        IOU1: --  -- -- x16 3D * 0:3:3 * P10 * S1_15:12 | 3C * 0:3:2 * P9  * S1_11:8  | 3B * 0:3:1 * P8  * S1_7:4  | 3A * 0:3:0 * P7  * S1_3:0
        */
        
        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

        //
        // Set bifurcation if riser card is inserted in Socket 0
        //
        if (IioGlobalData->IioVData.PcieRiser1Type) {
          DEBUG((EFI_D_INFO, "Riser Card inserted on Socket 0!  RiserType = %x\n", IioGlobalData->IioVData.PcieRiser1Type));
          if (IioGlobalData->IioVData.PcieRiser1Type == 0x03)
            IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
          else
            IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxx8xxx8;
        }

        // 
        // VPP invert
        //
        IioGlobalData->BdxSetupData.VPP_INVERT[0] = 1;   // socket 0  
          
        //
        // Slot Mappings
        //
        // Slot 1
        IioGlobalData->SetupData.SLOTIMP[7]   = 1;
        IioGlobalData->SetupData.SLOTPSP[7]   = 1;

        // Slot 11

        IioGlobalData->SetupData.SLOTIMP[1]   = 1;
        IioGlobalData->SetupData.SLOTPSP[1]   = 11;  

        IioGlobalData->SetupData.VppEnable[1]  = 1;
        IioGlobalData->SetupData.VppPort[1]    = 0;   //VPP Port reflects the PCA95x IO Port
        IioGlobalData->SetupData.VppAddress[1] = 0;   //Refer to Sch 0v610 sheet 109

        IioGlobalData->SetupData.SLOTHPCAP[1]  = 1;
        IioGlobalData->SetupData.SLOTPIP[1]    = 1;
        IioGlobalData->SetupData.SLOTAIP[1]    = 1;
        IioGlobalData->SetupData.SLOTMRLSP[1]  = 1; //MRL Present on Slot 0xA. J172
        IioGlobalData->SetupData.SLOTPCP[1]    = 1;
        IioGlobalData->SetupData.SLOTABP[1]    = 1;

        // Twinville
        IioGlobalData->SetupData.SLOTIMP[5]   = 1;
        IioGlobalData->SetupData.SLOTPSP[5]   = 15;

     }

     if (IioIndex == 1){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU1:
        IOU2: x4  x4        1B * 80:1:1 * P2  * S3_7:4   | 1A * 80:1:0 * P1 * S3_3:0
        IOU0: --  -- -- x16 2D * 80:2:3 * P6  * S4_15:12 | 2C * 80:2:2 * P5  * S4_11:8  | 2B * 80:2:1 * P4  * S4_7:4  | 2A * 80:2:0 * P3 * S4_3:0
        IOU1: x4  x4 -- --  3D * 80:3:3 * P10 * S6_7:4   | 3C * 80:3:2 * P9  * S6_3:0   | 3B * 80:3:1 * P8  * S14_7:4 | 3A * 80:3:0 * P7 * S14_3:0
*/
        if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxx8xxx8;

        //
        // Set bifurcation if riser card is inserted on Socket 0
        //
        if (IioGlobalData->IioVData.PcieRiser1Type) {
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;
        }
        
        //
        // Set bifurcation if riser card is inserted on Socket 1
        //
        if (IioGlobalData->IioVData.PcieRiser2Type) {
          DEBUG((EFI_D_INFO, "Riser Card inserted on Socket 1!  RiserType = %x\n", IioGlobalData->IioVData.PcieRiser2Type));
          if (IioGlobalData->IioVData.PcieRiser2Type == 0x03)
            IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;
          else
            IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxx8xxx8;
            
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxx8xxx8;
          
        }

        // 
        // VPP invert
        //
        IioGlobalData->BdxSetupData.VPP_INVERT[1] = 1;   // socket 1        
          
        //
        // Slot Mappings
        //
        // Slot 3
        IioGlobalData->SetupData.SLOTIMP[11 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 1] = 3;

        // Slot 4
        IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 3] = 4;

        // Slot 6
        IioGlobalData->SetupData.SLOTIMP[11 + 9] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 9] = 6;

        // Slot 14
        IioGlobalData->SetupData.SLOTIMP[11 + 7] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 7] = 14;

      }
      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      break;

    case TypeWildcatPass:
{
  UINT32                            Data32;
  UINT8                             Id0 = 0;
  UINT8                             Id1 = 0;
  UINT8                             Id2 = 0;
  UINT8                             Id3 = 0;
#ifndef IA32
#endif // IA32
  BOOLEAN                           IsPcieSsdExist = FALSE;

  //RiserID0  RiserID1  RiserID2  RiserID3  RiserID4  CPU0IOU0  CPU0IOU1  CPU0IOU2  CPU1IOU0      CPU1IOU1  CPU1IOU2
  //   1          1        NA        NA         NA              PE3x16                                      PE1A+B 2x4
  //   0          NA                                            PE3AB+CD 2X8                                PE1A+B 2x4
  //   NA         0                                             PE3AB+CD 2X8                                PE1A+B 2x4
  //   NA         NA       1         1          NA                                  PE2 X16       PE3C+D 2X4
  //   NA         NA       0         NA         NA                                  PE2AB+CD 2X8  PE3C+D 2X4
  //   NA         NA       NA        0          NA                                  PE2AB+CD 2X8  PE3C+D 2X4

  // RISER3                                                                                       PE3AB x8
  // SAS                                                                  PE1AB x8
  // IOM                                             PE2AB x8

  DEBUG((EFI_D_WARN, "UpdateIioConfig-WildcatPass\n"));


     if (IioIndex == 0){ 

     //
     // Set bifurcation control regs
     //
     if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
       IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above

     if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
       IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;

     if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
       IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

    //
    // Slot Mappings
    //
    //
    // FM_LINK_WIDTH_ID0_LVC3 GPIO12 RISER ID0
    //
    Data32 = IoRead32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL));
    if (Data32 & BIT12) {
      Id0 = 1;      
    } 

    //
    // RISER ID1, GPIO65
    //
    Data32 = IoRead32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3));
    if (Data32 & BIT1) {
      Id1 = 1;
    }

    if ( (Id0 == 1) && (Id1 == 1)) {
      IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

      // Slot 31
      IioGlobalData->SetupData.SLOTIMP[7]   = 1;
      IioGlobalData->SetupData.SLOTPSP[7]   = 0x311; 
    } else {
      IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxx8xxx8;

      //Slot 31
      IioGlobalData->SetupData.SLOTIMP[7]   = 1;
      IioGlobalData->SetupData.SLOTPSP[7]   = 0x311;  

      //Slot 32
      IioGlobalData->SetupData.SLOTIMP[9]   = 1;
      IioGlobalData->SetupData.SLOTPSP[9]   = 0x312;  
    }
    
     // Slot 21
     IioGlobalData->SetupData.SLOTIMP[3]   = 1;
     IioGlobalData->SetupData.SLOTPSP[3]   = 0x200;   // IO

     // Slot 22
     IioGlobalData->SetupData.SLOTIMP[5]   = 1;
     //IioGlobalData->SetupData.SLOTPSP[5]   = 22;    // Gbe

     // Slot 11
     IioGlobalData->SetupData.SLOTIMP[1]   = 1;
     IioGlobalData->SetupData.SLOTPSP[1]   = 0x210;   // SAS

    if (IsPcieSsdExist) {
      //
      // Done something for bifurcation
      //

//       IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_x4x4x4x4;
// 
//       // Slot 1
//       IioGlobalData->SetupData.SLOTIMP[3]   = 1;
//       IioGlobalData->SetupData.SLOTPSP[3]   = 10;  
// 
//       IioGlobalData->SetupData.SLOTIMP[4]   = 1;
//       IioGlobalData->SetupData.SLOTPSP[4]   = 11;
// 
//       // Slot 2
//       IioGlobalData->SetupData.SLOTIMP[5]   = 1;
//       IioGlobalData->SetupData.SLOTPSP[5]   = 12;
// 
//       IioGlobalData->SetupData.SLOTIMP[6]   = 1;
//       IioGlobalData->SetupData.SLOTPSP[6]   = 13;
    }


//      //
//      // Also hide the ports which are not used
//      //
//      IioGlobalData->SetupData.PEXPHIDE[5] =     1;
//      IioGlobalData->SetupData.PEXPHIDE[6] =     1;
// 
//      IioGlobalData->SetupData.HidePEXPMenu[5] = 1;
//      IioGlobalData->SetupData.HidePEXPMenu[6] = 1;
  }

     if (IioIndex == 1){ 

     if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
       IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;        // Use value from chart above

     if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
       IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

     if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
       IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxx8xxx8;    

    //
    // FM_LINK_WIDTH_ID2_LVC3 GPIO66 RISER ID2
    //
    Data32 = IoRead32((PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3));
    if (Data32 & BIT2) {
      Id2 = 1;
    }

    //
    // GPIO64, RISER ID3
    //
    Data32 = IoRead32(PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_LVL3);
    if (Data32 & BIT0) {
      Id3 = 1;
    }

    if ( (Id2 == 1) && (Id3 == 1)) {
      IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

      // Slot 41
      IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
      IioGlobalData->SetupData.SLOTPSP[11 + 3] = 0x324;
    } else {
      IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxx8xxx8;

      // Slot 41
      IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
      IioGlobalData->SetupData.SLOTPSP[11 + 3] = 0x324;   // Riser2 #4

      // Slot 42
      IioGlobalData->SetupData.SLOTIMP[11 + 5] = 1;
      IioGlobalData->SetupData.SLOTPSP[11 + 5] = 0x325;   // Riser2 #5
    }
    
     //
     // Slot Mappings
     //
     // Slot 51
     IioGlobalData->SetupData.SLOTIMP[11 + 1] = 1;
     IioGlobalData->SetupData.SLOTPSP[11 + 1] = 0x313;    // Riser1 #3

     // Slot 61
     IioGlobalData->SetupData.SLOTIMP[11 + 7] = 1;
     IioGlobalData->SetupData.SLOTPSP[11 + 7] = 0x337;    // Riser3 #7

     // Slot 62
     IioGlobalData->SetupData.SLOTIMP[11 + 9] = 1;
     IioGlobalData->SetupData.SLOTPSP[11 + 9] = 0x326;    // Riser2 #6

   }
   //
   // Hide the root ports whose lanes are assigned preceding ports
   //
   DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
     IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

   DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
     IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

   DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
     IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

  break;
}
	  break;  // TypeWildcatPass

    case TypeYakimaCityCRB:
    case TypeYakimaCitySRP:
    case TypeYakimaCitySTHI:
    // Source: Yakima City System Block Diagram (SRP-EV) Rev 0.3

     if (IioIndex == 0){ 

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: --  x8        1B * 0:1:1 * P2  * S3_7:4   | 1A * 0:1:0 * P1  * S3_3:0
        IOU0: --  -- -- --  2D * 0:2:3 * P6             | 2C * 0:2:2 * P5           | 2B * 0:2:1 * P4          | 2A * 0:2:0 * P3
        IOU1: --  -- -- x16 3D * 0:3:3 * P10 * S6_15:12 | 3C * 0:3:2 * P9 * S6_11:8 | 3B * 0:3:1 * P8 * S6_7:4 | 3A * 0:3:0 * P7 * S6_3:0
*/

        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)     // not used, EN package
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

        //
        // Slot Mappings
        //
        // Slot 3
        IioGlobalData->SetupData.SLOTIMP[1]   = 1;
        IioGlobalData->SetupData.SLOTPSP[1]   = 3;

        // Slot 6
        IioGlobalData->SetupData.SLOTIMP[7]   = 1;
        IioGlobalData->SetupData.SLOTPSP[7]   = 6;

     }

     if (IioIndex == 1){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU1:
        IOU2: x4  --        1B * 80:1:1 * P2           | 1A * 80:1:0 * P1 * S2_3:0  
        IOU0: --  -- -- --  2D * 80:2:3 * P6           | 2C * 80:2:2 * P5           | 2B * 80:2:1 * P4           | 2A * 80:2:0 * P3
        IOU1: --  x8 -- x8  3D * 80:3:3 * P10 * S4_7:4 | 3C * 80:3:2 * P9  * S4_3:0 | 3B * 80:3:1 * P8  * S5_7:4 | 3A * 80:3:0 * P7 * S5_3:0
*/
        if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_x4x4;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;    // not used, EN package

        if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxx8xxx8;

        //
        // Slot Mappings
        //
        // Slot 2
        IioGlobalData->SetupData.SLOTIMP[11 + 2] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 2] = 2;

        // Slot 4
        IioGlobalData->SetupData.SLOTIMP[11 + 9] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 9] = 4;

        // Slot 5
        IioGlobalData->SetupData.SLOTIMP[11 + 7] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 7] = 5;

      }
     //
     // Hide the root ports whose lanes are assigned preceding ports
     //
     DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     //
     // Also hide the ports which are not used
     //
     if (IioIndex == 0) {
       IioGlobalData->SetupData.PEXPHIDE[3]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[4]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[5]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[6]     = 1;

       IioGlobalData->SetupData.HidePEXPMenu[3] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[4] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[5] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[6] = 1;
     }
     if (IioIndex == 1) {
       IioGlobalData->SetupData.PEXPHIDE[11 + 3]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 4]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 5]     = 1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 6]     = 1;

       IioGlobalData->SetupData.HidePEXPMenu[11 + 3] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 4] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 5] = 1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 6] = 1;
     }
      break;

    case TypeAztecCityCRBSRP:
    case TypeAztecCitySTHI:
    // Source: Schematic, Aztec City CRB Rev 0V315

     if (IioIndex == 0){ 

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: --  x8        1B * 0:1:1 * P2  * S1_7:4   | 1A * 0:1:0 * P1  * S1_3:0
        IOU0: --  -- -- x16 2D * 0:2:3 * P6  * S6_15:12 | 2C * 0:2:2 * P5 * S6_11:8 | 2B * 0:2:1 * P4 * S6_7:4 | 2A * 0:2:0 * P3 * S6_3:0
        IOU1: --  -- -- x16 3D * 0:3:3 * P10 * S4_15:12 | 3C * 0:3:2 * P9 * S4_11:8 | 3B * 0:3:1 * P8 * S4_7:4 | 3A * 0:3:0 * P7 * S4_3:0
*/

        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
          
        //
        // Slot Mappings
        //
        // Slot 1
        IioGlobalData->SetupData.SLOTIMP[1]   = 1;
        IioGlobalData->SetupData.SLOTPSP[1]   = 1;
        IioGlobalData->SetupData.SLOTSPLS[1]  = 0;
        IioGlobalData->SetupData.SLOTSPLV[1]  = 0x1A;

        // Slot 6
        IioGlobalData->SetupData.SLOTIMP[3]   = 1;
        IioGlobalData->SetupData.SLOTPSP[3]   = 6;
        IioGlobalData->SetupData.SLOTSPLS[3]  = 0;
        IioGlobalData->SetupData.SLOTSPLV[3]  = 0x1A;

        // Slot 4
        IioGlobalData->SetupData.SLOTIMP[7]   = 1;
        IioGlobalData->SetupData.SLOTPSP[7]   = 4;
        IioGlobalData->SetupData.SLOTSPLS[7]  = 0;
        IioGlobalData->SetupData.SLOTSPLV[7]  = 0x1A;

     }

     if (IioIndex == 1){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU1:
        IOU2: -- x8        1B * 80:1:1 * P2  * S5_7:4   | 1A * 80:1:0 * P1 * S5_3:0
        IOU0: -- -- -- x16 2D * 80:2:3 * P6  * S2_15:12 | 2C * 80:2:2 * P5  * S2_11:8 | 2B * 80:2:1 * P4  * S2_7:4 | 2A * 80:2:0 * P3  * S2_3:0
        IOU1: -- -- -- --- 3D * 80:3:3 * P10            | 3C * 80:3:2 * P9  *         | 3B * 80:3:1 * P8           | 3A * 80:3:0 * P7
*/
        if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_x4x4;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_xxxxxx16;    // not used, so really don't care

        //
        // Slot Mappings
        //
        // Slot 2
        IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 3] = 2;
        IioGlobalData->SetupData.SLOTSPLS[11 + 3]  = 0;
        IioGlobalData->SetupData.SLOTSPLV[11 + 3]  = 0x1A;

        // Slot 5
        IioGlobalData->SetupData.SLOTIMP[11 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 1] = 5;
        IioGlobalData->SetupData.SLOTSPLS[11 + 1]  = 0;
        IioGlobalData->SetupData.SLOTSPLV[11 + 1]  = 0x1A;

      }
     //
     // Hide the root ports whose lanes are assigned preceding ports
     //
     DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     //
     // Also hide the ports which are not used
     //
     if (IioIndex == 1) {
       IioGlobalData->SetupData.PEXPHIDE[11 + 7] =      1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 8] =      1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 9] =      1;
       IioGlobalData->SetupData.PEXPHIDE[11 + 10] =     1;

       IioGlobalData->SetupData.HidePEXPMenu[11 + 7] =  1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 8] =  1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 9] =  1;
       IioGlobalData->SetupData.HidePEXPMenu[11 + 10] = 1;
     }
      break;

    case TypeIncaCityCRB:
    case TypeIncaCitySRP:

    // Source: Schematic, Inca City Baseboard SRP Fab 1 Version 051612
    // Also PCIeRiser 05912.xlsx

     if (IioIndex == 0){ 

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: x4  --        1B * 0:1:1 * P2  * X4IPASS_3:0 | 1A * 0:1:0 * P1
        IOU0: --  -- -- x16 2D * 0:2:3 * P6  * S2_15:12    | 2C * 0:2:2 * P5 * S2_11:8  | 2B * 0:2:1 * P4 * S2_7:4 | 2A * 0:2:0 * P3 * S2_3:0
        IOU1: --  x8 -- x8  3D * 0:3:3 * P10 * IOM2_7:4    | 3C * 0:3:2 * P9 * IOM2_3:0 | 3B * 0:3:1 * P8 * TW_7:4 | 3A * 0:3:0 * P7 * TW_3:0

        * Slot numbers assume Inca City riser installed
*/

        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_x4x4;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxx8xxx8;
          
        //
        // Slot Mappings
        //
        //
        // If riser card 1 inserted
        //
        if (IioGlobalData->IioVData.PcieRiser1Type != 0xFF) {

          switch (IioGlobalData->IioVData.PcieRiser1Type) {

            case 1:
            case 2:
              // Slot 6 - x16 - IOU0
              // Thunder Ridge risers
              //
              IioGlobalData->SetupData.SLOTIMP[3] = 1;
              IioGlobalData->SetupData.SLOTPSP[3] = 6;
              break;

            case 0:
            case 3:
              // Slot 2 - x16 - IOU0
              // Inca City risers
              //
              IioGlobalData->SetupData.SLOTIMP[3] = 1;
              IioGlobalData->SetupData.SLOTPSP[3] = 2;
              break;
          }
        }               

        // Slot 15 - PCIe x4 IPASS connector
        //
        IioGlobalData->SetupData.SLOTIMP[2] = 1;
        IioGlobalData->SetupData.SLOTPSP[2] = 15;

        // Slot 14 - Twinville
        //
        IioGlobalData->SetupData.SLOTIMP[7] = 1;
        IioGlobalData->SetupData.SLOTPSP[7] = 14;      

        // Slot 13 - IOM2
        //
        IioGlobalData->SetupData.SLOTIMP[9] = 1;
        IioGlobalData->SetupData.SLOTPSP[9] = 13;  

     }

     if (IioIndex == 1){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU1:
        IOU2: -- x8        1B * 40:1:1 * P2  * IOM1_7:4       | 1A * 40:1:0 * P1 * IOM1_3:0
        IOU0: -- -- -- x16 2D * 40:2:3 * P6  * S5_15:12       | 2C * 40:2:2 * P5 * S5_11:8       | 2B * 40:2:1 * P4 * S5_7:4       | 2A * 40:2:0 * P3 * S2_3:0
        IOU1: -- -- -- x16 3D * 40:3:3 * P10 * X16IPASS_15:12 | 3C * 40:3:2 * P9 * X16IPASS_11:8 | 3B * 40:3:1 * P8 * X16IPASS_7:4 | 3A * 40:3:0 * P7 * X16IPASS_3:0

        * Slot numbers above assume Inca City riser installed
*/
        if (IioGlobalData->SetupData.ConfigIOU2[1] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[1] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[1] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[1] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[1] = IIO_BIFURCATE_x4x4x4x4;

        //
        // Slot Mappings
        //
        // If riser card 2 inserted
        //
        if (IioGlobalData->IioVData.PcieRiser2Type != 0xFF) {

          switch (IioGlobalData->IioVData.PcieRiser2Type) {

            case 1:
            case 2:
              // Slot 9 - x16 - IOU0
              // Thunder Ridge risers
              //
              IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[11 + 3] = 9;
              break;

            case 0:
            case 3:
              // Slot 5 - x16 - IOU0
              // Inca City risers
              //
              IioGlobalData->SetupData.SLOTIMP[11 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[11 + 3] = 5;
              break;
          }
        }               

        // Slot 12 - IOM1
        //
        IioGlobalData->SetupData.SLOTIMP[11 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 1] = 13;

        // Slot 14-17 - PCIe x4 IPASS connector(s)
        //
        IioGlobalData->SetupData.SLOTIMP[11 + 7] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 7] = 14;

        IioGlobalData->SetupData.SLOTIMP[11 + 8] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 8] = 15;

        IioGlobalData->SetupData.SLOTIMP[11 + 9] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 9] = 16;

        IioGlobalData->SetupData.SLOTIMP[11 + 10] = 1;
        IioGlobalData->SetupData.SLOTPSP[11 + 10] = 17;

      }
      
      if (IioIndex == 2){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU2:
        IOU2: -- x8        1B * 80:1:1 * P2  * x16IPASS_7:4 | 1A * 80:1:0 * P1 * X16IPASS_3:0
        IOU0: -- -- -- x16 2D * 80:2:3 * P6  * S6_15:12     | 2C * 80:2:2 * P5 * S6_11:8      | 2B * 80:2:1 * P4 * S6_7:4 | 2A * 80:2:0 * P3 * S6_3:0
        IOU1: -- -- -- x16 3D * 80:3:3 * P10 * S4_15:12     | 3C * 80:3:2 * P9 * S4_11:8      | 3B * 80:3:1 * P8 * S4_7:4 | 3A * 80:3:0 * P7 * S4_3:0

        * Slot numbers assume Inca City riser installed
*/
        if (IioGlobalData->SetupData.ConfigIOU2[2] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[2] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[2] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[2] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[2] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[2] = IIO_BIFURCATE_xxxxxx16;

        //
        // Slot Mappings
        //
        // If riser card 2 inserted
        //
        if (IioGlobalData->IioVData.PcieRiser2Type != 0xFF) {
          
          switch (IioGlobalData->IioVData.PcieRiser2Type) {

            case 1:
            case 2:
              // Thunder Ridge risers
              //
              // Slot 11 - x16 - IOU0
              //
              IioGlobalData->SetupData.SLOTIMP[22 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[22 + 3] = 11;

              // Set IOU1 to x8x8
              //
                IioGlobalData->SetupData.ConfigIOU1[2] = IIO_BIFURCATE_xxx8xxx8;

              // Slot 10 - x8 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[22 + 7] = 1;
              IioGlobalData->SetupData.SLOTPSP[22 + 7] = 10;

              // Slot 12 - x8 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[22 + 9] = 1;
              IioGlobalData->SetupData.SLOTPSP[22 + 9] = 12;

              break;

            case 0:
            case 3:
              // Inca risers
              //
              // Slot 6 - x16 - IOU0
              //
              IioGlobalData->SetupData.SLOTIMP[22 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[22 + 3] = 6;

              // Slot 4 - x16 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[22 + 7] = 1;
              IioGlobalData->SetupData.SLOTPSP[22 + 7] = 4;

              break;
          }
        }               

        // Slot 18 - x16 iPASS connector
        //
        IioGlobalData->SetupData.SLOTIMP[22 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[22 + 1] = 18;

      }

     if (IioIndex == 3){ 
/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU2:
        IOU2: -- x8        1B * C0:1:1 * P2  * X16IPASS_7:4 | 1A * C0:1:0 * P1 * X16IPASS_3:0
        IOU0: -- -- -- x16 2D * C0:2:3 * P6  * S3_15:12     | 2C * C0:2:2 * P5 * S3_11:8      | 2B * C0:2:1 * P4 * S3_7:4 | 2A * C0:2:0 * P3 * S3_3:0
        IOU1: -- -- -- x16 3D * C0:3:3 * P10 * S1_15:12     | 3C * C0:3:2 * P9 * S1_11:8      | 3B * C0:3:1 * P8 * S1_7:4 | 3A * C0:3:0 * P7 * S1_3:0

        * Slot numbers assume Inca City riser installed
*/
        if (IioGlobalData->SetupData.ConfigIOU2[3] == IIO_BIFURCATE_AUTO)     // If no user override
          IioGlobalData->SetupData.ConfigIOU2[3] = IIO_BIFURCATE_xxx8;        // Use value from chart above

        if (IioGlobalData->SetupData.ConfigIOU0[3] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[3] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[3] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[3] = IIO_BIFURCATE_xxxxxx16;

        //
        // Slot Mappings
        //
        // If riser card 1 inserted
        //
        if (IioGlobalData->IioVData.PcieRiser1Type != 0xFF) {

          switch (IioGlobalData->IioVData.PcieRiser1Type) {
            
            case 1:
            case 2:
              // Thunder Ridge risers
              //
              // Slot 8 - x16 - IOU0
              //
              IioGlobalData->SetupData.SLOTIMP[33 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[33 + 3] = 8;

              // Set IOU1 to x8x8
              //
                IioGlobalData->SetupData.ConfigIOU1[3] = IIO_BIFURCATE_xxx8xxx8;

              // Slot 5 - x8 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[33 + 7] = 1;
              IioGlobalData->SetupData.SLOTPSP[33 + 7] = 5;

              // Slot 7 - x8 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[33 + 9] = 1;
              IioGlobalData->SetupData.SLOTPSP[33 + 9] = 7;
              break;

            case 0:
            case 3:
              // Inca risers
              //
              // Slot 3 - x16 - IOU0
              //
              IioGlobalData->SetupData.SLOTIMP[33 + 3] = 1;
              IioGlobalData->SetupData.SLOTPSP[33 + 3] = 3;

              // Slot 1 - x16 - IOU1
              //
              IioGlobalData->SetupData.SLOTIMP[33 + 7] = 1;
              IioGlobalData->SetupData.SLOTPSP[33 + 7] = 1;
              break;
          }
        }               

        // Slot 19 - x16 iPASS connector
        IioGlobalData->SetupData.SLOTIMP[33 + 1] = 1;
        IioGlobalData->SetupData.SLOTPSP[33 + 1] = 9;

      }
     //
     // Hide the root ports whose lanes are assigned preceding ports
     //
     DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      break;


    case TypeHedtSKU1CRB:
    case TypeHedtEv:
    case TypeHedtDDR3:

      // Source: Odin ERB Fab1 schematic odin_ev_rev1.076.pdf
      //
     if (IioIndex == 0){ 

/*
                           IIO port# * B:D:F * BIOS port# * slot#
        CPU0:
        IOU2: --  x8        1B * 0:1:1 * P2  * S2_7:4   | 1A * 0:1:0 * P1  * S2_3:0
        IOU0: --  -- -- x16 2D * 0:2:3 * P6  * S6_15:12 | 2C * 0:2:2 * P5  * S6_11:8 | 2B * 0:2:1 * P4  * S6_7:4 | 2A * 0:2:0 * P3  * S6_3:0
        IOU1: --  -- -- x16 3D * 0:3:3 * P10 * S4_15:12 | 3C * 0:3:2 * P9  * S4_11:8 | 3B * 0:3:1 * P8  * S4_7:4 | 3A * 0:3:0 * P7  * S4_3:0
*/

        //
        // Set bifurcation control regs
        //
        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {     // If no user override
          //
          // 4168945: PCIE:  L0 20-lane reduced part hides Device 1 function 0
          // 4986974: HEDT (Lituya Bay) PCIe Bifurcation Set Incorrectly
          // 4986968: HEDT SKU w/PCIe 28-lanes, algorithm to allow IOU2 be detected as x4 does not work for 40-lanes
          //
          // D1:F0 was being hidden on HSW SKUs with reduced PCIe lanes (used only on HEDT),
          //  this W/A takes care of it. But, make sure we implement the W/A *only* on
          //  those parts, otherwise port bifurcation can get mangled.
          //
          // Read CAPID2 fuses in B1:D30:F3 to see if this IIO has reduced PCIe lanes
          // If PCIE_DISXPDEV != 0, lanes are reduced
          //
          Data32 = PciExpressRead32 (PCI_PCIE_ADDR (IioGlobalData->IioVData.SocketUncoreBusNumber[0], PCIE_PORT_CPU_CAPIDS_DEV, PCIE_PORT_CPU_CAPIDS_FUNC, R_CPU_CAPID2));
          Data32 = (Data32 >> 3) & 0xFFF;
          if (Data32) {
             IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_x4x4;        // Force x4x4 to avoid Hide D1F0
          } else {
             IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;        // Use value from chart above
          }
        }
           

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxxxxx16;

        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO)
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;

        //
        // Slot Mappings
        //
        // Slot 2
        IioGlobalData->SetupData.SLOTIMP[1]   = 1;
        IioGlobalData->SetupData.SLOTPSP[1]   = 2;

        // Slot 6
        IioGlobalData->SetupData.SLOTIMP[3]   = 1;
        IioGlobalData->SetupData.SLOTPSP[3]   = 6;

        // Slot 4
        IioGlobalData->SetupData.SLOTIMP[7]   = 1;
        IioGlobalData->SetupData.SLOTPSP[7]   = 4;

      }

     //
     // Hide the root ports whose lanes are assigned preceding ports
     //
     DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
       IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

     //
     // Also hide the ports which are not used
     //

      break;

    default :
      // Unidentified platform
      //
      // In this case, program the HW default IOU bif value - x16 for IOU0,1, & x8 for IOU2
      //
      // If user not overriding, set default bifurcation
      //
      if (IioGlobalData->SetupData.ConfigIOU2[IioIndex] == IIO_BIFURCATE_AUTO) {
         IioGlobalData->SetupData.ConfigIOU2[IioIndex] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU0[IioIndex] == IIO_BIFURCATE_AUTO) {
         IioGlobalData->SetupData.ConfigIOU0[IioIndex] = IIO_BIFURCATE_xxxxxx16;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[IioIndex] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU1[IioIndex] = IIO_BIFURCATE_xxxxxx16;
      }
      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif(IioGlobalData, 2, IioIndex, IioGlobalData->SetupData.ConfigIOU2[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 0, IioIndex, IioGlobalData->SetupData.ConfigIOU0[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif(IioGlobalData, 1, IioIndex, IioGlobalData->SetupData.ConfigIOU1[IioIndex],
        IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      break;
    }  // end of Switch for platform type
}

#ifndef IA32
VOID
OverwriteSelectiveIioConfigSettings (
    IIO_CONFIG  *dest,
    IIO_CONFIG  *src
)
{
  //
  //  Update the flags for each of the PCIe port display
  //
  CopyMem (
          dest->HidePEXPMenu,
          src->HidePEXPMenu,
          MAX_TOTAL_IIO_PORTS
      );
}

VOID
OemIioInitDriverInstalled (
  IIO_GLOBALS  *IioGlobalData
  )
/*++

 Routine Description:

  OEM Hook after all initialization at IioInit driver entry point invocation. 
  (e.g. last call before returning to dispatcher from IioInit driver entry point)

Arguments:


Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
  SETUP_INFO              SetupInfo;
  SYSTEM_CONFIGURATION    SetupData;
  UINT32                  VariableSize;
  EFI_STATUS              Status;

  //
  //  Overwrite the NVRAM Setup options of IIO with the defaults as deduce dynamically based on the
  //  platform type - ajaveed
  //
  VariableSize = sizeof(SYSTEM_CONFIGURATION);
  Status = GetSystemVariable(&SetupData);
  ASSERT_EFI_ERROR(Status);

  // only overwrite the IIO_CONFIG fields since some of the IIO setup question depends on other members
  // which have different settings as per the board settings
  CopyMem (
      &SetupInfo,
      &(SetupData.VariableTag),
      sizeof (SETUP_INFO)
  );

  CopyMem(
          &SetupInfo.Config.Hide_WA_4167453,
          &IioGlobalData->SetupData.Hide_WA_4167453, 
          sizeof(SetupInfo.Config.Hide_WA_4167453)
           );
  //just overwrite the selected IIO_CONFIG members on NVRAM since their would be dependency to other IIO_CONFIG options
  OverwriteSelectiveIioConfigSettings(
      &SetupInfo.Config,
      &IioGlobalData->SetupData
           );

  CopyMem (
      &(SetupData.VariableTag),
      &SetupInfo,
      sizeof (SETUP_INFO)
           );

  Status = SetSystemVariable(&SetupData, VariableSize);
  ASSERT_EFI_ERROR(Status);
}
#endif //IA32

VOID
OemDmiPreInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
    IIO_GLOBALS   *IioGlobalData
  )
/*++

Routine Description:

  OEM Hook before bus number is assigned to DMI (earliest PCI enumeration point) but after chipset DMI 
  Initialization occurs (chipset DMI initialization before resources are assigned). 

Arguments:

   IioGlobalData   Pointer to the IIO Global for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
}

VOID
OemDmiDeviceInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
    IIO_GLOBALS   *IioGlobalData
  )
/*++

Routine Description:

  OEM Hook before resources (Bus number is assigned, other resources aren't) are assigned to DMI but after 
  chipset DMI Initialization occurs. 

Arguments:

   IioGlobalData   Pointer to the IIO Global for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
}

VOID
OemDmiResourceAssignedInit (
    PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
    IIO_GLOBALS   *IioGlobalData
  )
/*++

Routine Description:

  OEM Hook after all Dmi resources are assigned and all DMI Chipset init is complete.
  
Arguments:

   IioGlobalData   Pointer to the IIO Global for this driver

Returns:

  VOID              All other error conditions encountered result in an ASSERT

--*/
{
}

UINT8
CheckForAndLogCsiErrors(
  VALIDATION_CSI_ERROR_STRUCT        *CsiErrorPtr
  )
{
  if (CsiErrorPtr == NULL) {
    return (UINT8)-1;
  }
  return (UINT8)0;   // No Errors present
}

UINT8
CheckForAndLogPcieErrors(
  VALIDATION_PCIE_ERROR_STRUCT       *PcieErrorPtr
  )
{
  if (PcieErrorPtr == NULL) {
    return (UINT8)-1;
  }
  return (UINT8)0;   // No Errors present
}

#ifndef IA32
VOID
OemIioInitCallbackEndPoint (
  IN  PCI_ROOT_BRIDGE_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                   Phase,
  IN  IIO_GLOBALS                                 *IioGlobalData
  )
/*++

Routine Description: Oem Callback at end of IioInitCallback.

Arguments:
 
Returns:
  None
--*/
{  
    UINT8                SecBus;
    UINT8                Func;
    UINT16               DevCtrl2;
    UINT32               DidVid;

    if (Phase == EfiPciEnumerationResourceAssigned) {
          //
          // Find NIC behind the IIO PCIE Root Ports
          //
          SecBus = PciExpressRead8 (PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
          if ((SecBus != 0xFF) && (SecBus != 0x00)) {
            //
            // HSD BIOS Sighting #3427621 - Set Kawela Completion Timeout value to at least 10ms
            // Match the clarification in 82576EB (Kawela) Spec Update v2.3
            // Clarification 3: PCIe Completion Timeout value must be properly set.
            // Check for 82576 (Kawela) onboard NIC on the I/O riser.
            //
            if (PciAddress.Bus == PCIE_PORT_BUS_NUM &&
                (PciAddress.Device == SR_NIC0_RP_DEV_NUM || PciAddress.Device == SR_NIC1_RP_DEV_NUM)) {
              for (Func = 0; Func <= 1; Func++) {
                //
                // Need to update Completion Timeout Value (bit 3:0) in Device Control 2 Register (0xC8)
                // per each function on both NICs.
                //
                DevCtrl2 = PciExpressRead16 (PCI_PCIE_ADDR(SecBus, 0, Func, 0xC8));
                DevCtrl2 |= 0x05;  // Completion Timeout Value of 16ms-55ms
                PciExpressWrite16 (PCI_PCIE_ADDR(SecBus, 0, Func, 0xC8), DevCtrl2);
                IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "pwu3:bus :%d device 0 function %d set\n",SecBus, Func);
              }
            } else {
              //
              // Also check for 82576 (Kawela) & 82598 (Oplin) Add-in cards.
              // Same clarification for Completion Timeout detailed in 82598 Spec Update v2.7
              //
              DidVid = PciExpressRead32 (PCI_PCIE_ADDR(SecBus, 0, 0, PCI_VENDOR_ID_OFFSET));
              switch (DidVid) {
                case NIC_82598EB_XFSR_DUAL_DIDVID:
                case NIC_82598EB_CX4_DUAL_DIDVID:
                case NIC_82598EB_AFDA_DUAL_DIDVID:
                case NIC_82576_ET_DUAL_DIDVID:
                case NIC_82576_EF_DUAL_DIDVID:
                  //
                  // Dual Port NICs need to check the function for each port.
                  // Update Completion Timeout Value for function 1 then drop through for function 0.
                  //
                  DevCtrl2 = PciExpressRead16 (PCI_PCIE_ADDR(SecBus, 0, 1, 0xC8));
                  DevCtrl2 |= 0x05;  // Completion Timeout Value of 16ms-55ms
                  IioPciExpressWrite16 ( IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 1, 0xC8), DevCtrl2);

                case NIC_82598EB_XFSR_SNGL_DIDVID:
                case NIC_82598EB_AT_SNGL_DIDVID:
                case NIC_82598EB_XFLR_SNGL_DIDVID:
                  //
                  // Single Port NICs just need to check the single function.
                  // Update Completion Timeout Value for function 0.
                  //
                  DevCtrl2 = PciExpressRead16 (PCI_PCIE_ADDR(SecBus, 0, 0, 0xC8));
                  DevCtrl2 |= 0x05;  // Completion Timeout Value of 16ms-55ms
                  IioPciExpressWrite16 ( IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 0, 0xC8), DevCtrl2);
                  break;

                default:
                  break;
              }
            }
          }
    }

}


#endif //IA32

/*---------------------------------------------------------------------------*/
/*                      OEM hooks for PciHostBridge driver                   */
/*---------------------------------------------------------------------------*/

UINT8
CheckSoftSimulationPlatform(
  VOID
  )
{
  return (UINT8)IsSoftSdvPlatform();
}

EFI_STATUS
OemCheckMmio64Enabled(
   BOOLEAN    *Is64bitEnabled
)
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  SYSTEM_CONFIGURATION                  SystemConfiguration;

  Size = sizeof(SYSTEM_CONFIGURATION);
  ZeroMem(&SystemConfiguration, Size);    //clean local buffer
  Status = GetSystemVariable(&SystemConfiguration);
  if (EFI_SUCCESS == Status) {
    *Is64bitEnabled = SystemConfiguration.Pci64BitResourceAllocation ? TRUE: FALSE;
  } else {
    Status = EFI_NOT_READY;
  }

  return Status;
}

EFI_STATUS
OemEnableMmio64bitAllocation(
    VOID
)
{
  EFI_STATUS                            Status;
  UINTN                                 Size;
  SYSTEM_CONFIGURATION                  SystemConfiguration;

  Size = sizeof(SYSTEM_CONFIGURATION);
  ZeroMem(&SystemConfiguration, Size);    //clean local buffer
  Status = GetSystemVariable( &SystemConfiguration);

  if (EFI_SUCCESS == Status) {
    SystemConfiguration.Pci64BitResourceAllocation = 1;
    Status = SetSystemVariable( &SystemConfiguration, sizeof(SYSTEM_CONFIGURATION));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
// AptioV Server Override Start: Commenting the function as it is not required. We are handling it in OemEarlyUncoreInit() function.
/*VOID
OemUpdateUdsSetupToSystemConfig(
    SETUP_INFO                            *SetupInfo
)
{
  UINT32                VarSize;
  EFI_STATUS            Status;
  SYSTEM_CONFIGURATION  SetupData;
  UINT8                 Socket;

  //
  // Obtaine the system configuration data
  //

  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status  = GetSystemVariable(&SetupData);
  ASSERT_EFI_ERROR(Status);

  //
  // Update the system configuration with the SETUP_INFO data
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    SetupData.IoAlignment[Socket] = SetupInfo->CsiConfig.IoAlignment[Socket];
    SetupData.IoLength[Socket] = SetupInfo->CsiConfig.IoLength[Socket];
    SetupData.MmiolAlignment[Socket] = SetupInfo->CsiConfig.MmiolAlignment[Socket];
    SetupData.MmiolLength[Socket] = SetupInfo->CsiConfig.MmiolLength[Socket];
  }

  Status = SetSystemVariable(&SetupData, VarSize);
  ASSERT_EFI_ERROR(Status);
}*/
// AptioV Server Override End: Commenting the function as it is not required. We are handling it in OemEarlyUncoreInit() function.

VOID
OemUpdateDmiVcxSettings(
  IIO_GLOBALS   *IioGlobalData
)
/*++

Routine Description: Update DmiVcx setup values in the IIO Configuration Menu

Arguments:
 
Returns:
  None
--*/
{
  UINT32                VarSize;
  EFI_STATUS            Status;
  SYSTEM_CONFIGURATION  SetupData;
  SETUP_INFO              SetupInfo;

  //
  // Obtaine the system configuration data
  //
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status  = GetSystemVariable(&SetupData);
  ASSERT_EFI_ERROR(Status);
  CopyMem (
    &SetupInfo,
    &(SetupData.VariableTag),
    sizeof (SETUP_INFO)
  );
  // 4986285: ESCALATE from hexaii_hsx_mock:C0 BIOS/Isoc issues with 18d15  
  // Override DmiVcx settings if changed based on Isoc/Meseg values on PlatformInfo.c
  // and show the current value on the setup menu.
  CopyMem (
              &SetupInfo.Config.DmiVcp,
              &IioGlobalData->SetupData.DmiVcp,
              sizeof(SetupInfo.Config.DmiVcp)
            );
  CopyMem (
              &SetupInfo.Config.DmiVc1,             
              &IioGlobalData->SetupData.DmiVc1,
              sizeof(SetupInfo.Config.DmiVc1)
            );    
  CopyMem (
              &SetupInfo.Config.DmiVcm,
              &IioGlobalData->SetupData.DmiVcm,
              sizeof(SetupInfo.Config.DmiVcm)
            );    
  
  CopyMem (
      &(SetupData.VariableTag),
      &SetupInfo,
      sizeof (SETUP_INFO)
  );

  Status = SetSystemVariable( &SetupData, VarSize );
  ASSERT_EFI_ERROR(Status);

}
/*---------------------------------------------------------------------------*/

/*****************************
 * Auto determine which PCIe Root port to be hidden if its lanes are assigned
 * to its preceding root port...use the Setup option variable of ConfigIOU to
 * determine which ports are to be hidden on each IOU for corresponding IIO
 *****************************/
VOID  DeducePEXPHideFromIouBif  (
    IIO_GLOBALS *IioGlobalData,
    UINT32     IOUx,
    UINT32     IIOn,
    UINT8     CurrentIOUConfigValue,
    UINT8     *PXPHide,
    UINT8     *HidePEXPMenu
    )
{

#ifdef IA32
  if( CurrentIOUConfigValue == 0xFF){
    IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_ERROR, "ERROR: Invalid IOUx Bifurcation =%x\n", CurrentIOUConfigValue);
    return;
  }
#else  
      ASSERT(CurrentIOUConfigValue != 0xFF);
#endif //IA32

  switch (IOUx) {
  case  2:   // IOU2 has 2 PCIe root ports 1A & 1B
    if(CurrentIOUConfigValue == 1)  {          // port 1A is x8 ..
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) + 2] = 1;        // .. hide 1B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) + 2] = 1;   // hide the Setup menu for 1B
  }
    else
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) + 2] = 0;
    break;

  case  0:   // IOU0 has the 4 PCIe root ports 2A, 2B, 2C, 2D
    switch(CurrentIOUConfigValue)
    {
    case 1:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;        // hide 2B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;   // hide the Setup menu for 2B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 0;   // make sure the port 2C is not hidden
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 0;   // make sure the port 2D is not hidden
      break;
    case 2: 
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;        // hide port 2D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;   // hide the Setup menu for 2D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 0;   // make sure the port 2B is not hidden
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 0;   // make sure the port 2C is not hidden
      break;
    case 3:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;        // hide 2B
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;        // hide 2D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;   // hide the Setup menu for 2B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;   // hide the Setup menu for 2D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 0;   // make sure the port 2C is not hidden
      break;
    case 4:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;        // hide 2B
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 1;        // hide 2C
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;        // hide 2D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 1;   // hide the Setup menu for 2B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 1;   // hide the Setup menu for 2C
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 1;   // hide the Setup menu for 2D
      break;
    default:
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +4] = 0;
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +5] = 0;
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +6] = 0;
      break;
}
    break;

  case  1:   // IOU1 has the 4 PCIe root ports 3A, 3B, 3C, 3D
    switch(CurrentIOUConfigValue)
{
    case 1:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;        // hide port 3B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;   // hide the Setup menu for 3B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 0;   // make sure the port 3C is not hidden
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 0;  // make sure the port 3D is not hidden
      break;
    case 2:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;       // hide port 3D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;  // hide the Setup menu for 3D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 0;   // make sure port 3B menu is not hidden
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 0;   // make sure port 3C menu is not hidden
      break;
    case 3:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;        // hide 3B
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;       // hide 3D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;   // hide the Setup menu for 3B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;  // hide the Setup menu for 3D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 0;   // make sure port 3C menu is not hidden
      break;
    case 4:
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;        // hide 3B
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 1;        // hide 3C
      PXPHide[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;       // hide 3D
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 1;   // hide the Setup menu for 3B
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 1;   // hide the Setup menu for 3C
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 1;  // hide the Setup menu for 3D
      break;
    default:
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +8] = 0;
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +9] = 0;
      HidePEXPMenu[(IIOn * NUMBER_PORTS_PER_SOCKET) +10] = 0;
      break;
    }
    break;
  }
  return;
}

/******************************************************************************
 * Routine name: OemIioPortDisable
 * Input : index for system IIO or CPU socket #
 * Output : List of Ports which needs to be disable or power down
 * Description: Routine returns bitwise list of the ports which needs to be
 * power down. The bit 0 indicates Port 0, bit 1 for port 1, and so on; the
 * value of 1 indicates affirmation of the port to be disabled.
 ******************************************************************************/
VOID
OemIioPortDisable (
  UINT8          Iio,
  IIO_GLOBALS    *IioGlobalData,
  UINT32         *IioPortDisableBitMap
)
{
  UINT32  PortDisableBitMap = 0;

  switch(IioGlobalData->IioVData.PlatformType)  {
      //case TypeBricklandSRP:
    case TypeMayanCityCRB:
    case TypeMayanCitySRP:
    case TypeMayanCityCRBDDR3:
    case TypeMayanCitySRPDDR3:
    case TypeWildcatPass:
    
        if (IioGlobalData->IioVData.SocketPresent[Iio]) {
          switch(Iio){
          case  0:  //IIO 0
            PortDisableBitMap = BIT1 | BIT2;  //Port 1A, 1B (D1F0, D1F1) not routed for TR IIO0
            break;
          case  1:  //IIO 1
            PortDisableBitMap = BIT0 | BIT1 | BIT2;  //Port 0, Port 1A, 1B (D0F0, D1F0, D1F1) not routed for TR IIO1
            break;
          case  2:  //IIO 2
            PortDisableBitMap = BIT0 | BIT1 | BIT2;  //Port 0, Port 1A, 1B (D0F0, D1F0, D1F1) not routed for TR IIO2
            break;
          case  3:  //IIO 3
            PortDisableBitMap = BIT0 | BIT1 | BIT2;  //Port 0, Port 1A, 1B (D0F0, D1F0, D1F1) not routed for TR IIO3
            break;
          }
        }
        break;

    default:
    break;
  }


  *IioPortDisableBitMap = PortDisableBitMap;
  return;
}

/*
 * Routine:     OemIioUplinkPortDetails
 * Input:       None
 * Output:      CPU index & its port index
 * Description: Returns the Uplink port details from the IIO side, as per the
 *              platform board schematics, if no uplink port than return 0xFF
 *              for both IIO and port index.
 */
VOID
OemIioUplinkPortDetails(
  UINT8   Iio,
  UINT8   *PortIndex,
  UINT8   *Bus,
  UINT8   *Dev,
  UINT8   *Func
)
{
  //
  //init the input variables
  //
  *PortIndex  = 0xFF;
  *Bus        = 0xFF;
  *Dev        = 0xFF;
  *Func       = 0xFF;

  //
  // override with platform specific data
  //
  return;
}


VOID
OemInitializeIioInputData  (
  IN IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8                         Iio;
  IIO_UDS                       *IioUds;
  EFI_HOB_GUID_TYPE             *GuidHob;
  EFI_GUID                      UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;

  GuidHob       = GetFirstGuidHob (&UniversalDataGuid);
  IioUds  = GET_GUID_HOB_DATA (GuidHob);  
  ASSERT (IioUds);

  OemGetIioPlatformInfo(IioGlobalData);
  IioGlobalData->IioVData.CpuType = IioUds->SystemStatus.cpuType;
  IioGlobalData->IioVData.CpuFamily = IioUds->SystemStatus.cpuFamily;
  IioGlobalData->IioVData.SkuType = IioUds->SystemStatus.skuType;
  IioGlobalData->IioVData.PciExpressBase = IioUds->PlatformData.PciExpressBase;
  IioGlobalData->IioVData.CpuStepping = IioUds->SystemStatus.MinimumCpuStepping;
  IioGlobalData->IioVData.CpuSubType = IioUds->SystemStatus.cpuSubType;
  IioGlobalData->IioVData.MctpEn = IioUds->SystemStatus.MctpEn;
  IioGlobalData->IioVData.IsocEnable = IioUds->SystemStatus.IsocEnable;
  IioGlobalData->IioVData.MeSegEn = IioUds->SystemStatus.MeSegEn;
  IioGlobalData->IioVData.IOxAPICCallbackBootEvent = FALSE;
  IioGlobalData->IioVData.EVMode = IioUds->SetupInfo.MiscConfig.EVMode;
  IioGlobalData->IioVData.IioLinkTrainPhase = IioUds->SystemStatus.IioLinkTrainPhase;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if (!IioUds->PlatformData.IIO_resource[Iio].Valid) continue;
    IioGlobalData->IioVData.SocketPresent[Iio] = IioUds->PlatformData.IIO_resource[Iio].Valid;
    IioGlobalData->IioVData.SocketBaseBusNumber[Iio] = IioUds->PlatformData.IIO_resource[Iio].BusBase;
    IioGlobalData->IioVData.SocketUncoreBusNumber[Iio] = IioUds->PlatformData.IIO_resource[Iio].BusLimit;
    IioGlobalData->IioVData.PciResourceMem32Limit[Iio] = IioUds->PlatformData.IIO_resource[Iio].PciResourceMem32Limit;
    IioGlobalData->IioVData.PciResourceMem32Base[Iio] = IioUds->PlatformData.IIO_resource[Iio].PciResourceMem32Base;
    IioGlobalData->IioVData.VtdBarAddress[Iio] = IioUds->PlatformData.IIO_resource[Iio].VtdBarAddress;
    IioGlobalData->IioVData.RcbaAddress[Iio] = IioUds->PlatformData.IIO_resource[Iio].RcbaAddress;
    IioGlobalData->IioVData.IoApicBase[Iio] = IioUds->PlatformData.IIO_resource[Iio].IoApicBase;
  }
  // 4986285: ESCALATE from hexaii_hsx_mock:C0 BIOS/Isoc issues with 18d15
  // Get the current DMiVcx values based on what QPIRC configured for Isoc
  // and MeSeg.
  //
  if (IioGlobalData->IioVData.IsocEnable == 0) {
    //
    // 4986450 ESCALATE from hexaii_hsx_mock:C0:  Virtual channels not disabled when Isoc disabled via secondary 
    // The isoc enable value might have changed when QPI was configured. mPlatformData might contain invalid 
    // information since the system could have detected that isoc is not supported on the current configuration. 
    // So, if QPI changed IsocEnabled value to 0, set al the VC channels to unsupported. 
    IioGlobalData->SetupData.DmiVc1 = 0;
    IioGlobalData->SetupData.DmiVcp = 0;
    if (IioGlobalData->IioVData.MeSegEn == 0 ) {
      // Assuming that MeSegEnable also changed, we replicate the condition on PlatformInfo.c, 
      IioGlobalData->SetupData.DmiVcm = 0; 
    }
  }
  OemUpdateDmiVcxSettings(IioGlobalData);

}


UINT32
GetPortClassCode(
   IIO_GLOBALS   *IioGlobalData,
   UINT8         Bus,
   UINT8         Device,
   UINT8         Function
)
/*++

Routine Description:
  Helper routine which returns the PCI header class code value for the given BDF

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  Class code value from PCI header offset 9,Ah, & Bh

--*/
{
  UINT32  ClassCode = 0;

  ClassCode = IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR( Bus,
                                              Device,
                                              Function,
                                              ONLY_REGISTER_OFFSET(RID_IIO_PCIE_REG)));
  return (ClassCode >> 8);
}

BOOLEAN
OemIdentifyIioPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is valid port of the IIO

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  UINT8   j, Iio;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }

  if(Iio >= MaxIIO)
    return FALSE;

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if( (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device == Device) &&
        (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function == Function) ){
      return TRUE;
    }
  }

  if(j >= NUMBER_PORTS_PER_SOCKET)
    return FALSE;

  return FALSE;
}


BOOLEAN
OemIdentifyDmiPort(
   IIO_GLOBALS   *IioGlobalData,
   UINT8         Bus,
   UINT8         Device,
   UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is DMI port

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  if(OemIdentifyIioPort(IioGlobalData, Bus, Device, Function) == FALSE)
    return FALSE;
  //
  // if Class code matches the PCI Host Bridge's base class (06), subclass (0),
  // Reg. Prog interface(0), than it is connected to the PCH on the downlink
  // which eventually determins the DMI port
  //
  if (GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_PCI_HOST_BRIDGE)
    return TRUE;              //return TRUE to indicate that the PCI address belongs to DMI port
  else
    return FALSE;             //return FALSE as it does not belong to DMI port
}

BOOLEAN
OemIdentifyDmiDfxPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         DfxDevice,
  UINT8         DfxFunction
)
{
  UINT8   j;

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if( (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice == DfxDevice) &&
        (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction == DfxFunction) ){
       return OemIdentifyDmiPort(IioGlobalData,
                                 Bus,
                                 IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device,
                                 IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function);
    }
  }
  return FALSE;
}
BOOLEAN
OemIdentifyPCIeRootPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is regular PCIe root port

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  if(OemIdentifyIioPort(IioGlobalData, Bus, Device, Function) == FALSE)
    return FALSE;
  //
  // if Class code matches the PCI to PCI Bridge's base class (06), subclass (04),
  // Reg. Prog interface(0), than it is the regular PCIe Root port
  //
  if(GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_PCI_TO_PCI_BRIDGE)
    return TRUE;              //return TRUE to indicate that the PCI address belongs to RP
  else
    return FALSE;             //return FALSE as it does not belong to RP
}

BOOLEAN
OemIdentifyPCIeNTBPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is PCIe NTB port

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  if(OemIdentifyIioPort(IioGlobalData, Bus, Device, Function) == FALSE)
    return FALSE;

  //
  // if Class code matches the Other Bridge device's base class (06), subclass (80),
  // Reg. Prog interface(0), than it is the PCIe NTB port
  //
  if(GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_PCI_OTHER_BRIDGE)
    return TRUE;              //return TRUE to indicate that the PCI address belongs to NTB port
  else
    return FALSE;             //return FALSE as it does not belong to NTB port
}

BOOLEAN
OemIdentifyVendorSpecificPort(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function,
  BOOLEAN       DfxArguments
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is Intel specific bridge device (IOSF)

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF
  DfxArguments - Boolean varaible TURE when BDF is passed as DFX else FALSE

Returns:
  TRUE if known IIO port, else FALSE

--*/
{
  return FALSE;   //no special vendor specific PCIe port present in EP/EX IIO
}

UINT32
OemGetPchRootComplexBaseAddress (
  IIO_GLOBALS *IioGlobalData
  )
{
  UINT32  RCBA = 0;
  RCBA = IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketBaseBusNumber[0], 0x31, 0, 0xF0));
  if(RCBA & 1)    //RCBA is valid (bit 0)
    return RCBA & 0xFFFFFFFE;
  else
    return 0;
}

BOOLEAN
OemSetPchDmiLinkSpeed (
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress,
  IIO_GLOBALS                                   *IioGlobalData,
  UINT8                                         LinkSpeed
)
{
  UINT32  RootComplexBase;
  UINT8   PchDmiLinkSpeed;

  if(OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    return FALSE;
  RootComplexBase = IioGlobalData->IioVData.PchRcbaAddress;
  if(!RootComplexBase)
    return FALSE;

  PchDmiLinkSpeed = (UINT8) (*(UINT8*)((UINTN)RootComplexBase + R_PCH_DMI_LCTL2) & (BIT3 | BIT2 | BIT1 | BIT0));
  IioDebugPrintInfo( IioGlobalData, IIO_DEBUG_INFO, "RootComplexBase = 0x%x, PCH DMI target link speed = %d\n", RootComplexBase, PchDmiLinkSpeed);

  if(PchDmiLinkSpeed == LinkSpeed)
    return TRUE;
  else{
    //
    // Set RCBA + Offset 21B0h[3:0] with target link speed as DMI Gen1/Gen2
    //
    *(UINT8*) ((UINTN)RootComplexBase + R_PCH_DMI_LCTL2) &= (UINT8)~(BIT3 | BIT2 | BIT1 | BIT0);
    *(UINT8*) ((UINTN)RootComplexBase + R_PCH_DMI_LCTL2) |= LinkSpeed;
    return TRUE;    //to indicate speed change operation done on PCH DMI side
  }
}

BOOLEAN
OemIsDmiVcpEnable(
  IIO_GLOBALS *IioGlobalData
)
/*++

Routine Description:
  OEM Hook to check if DmiVcp is enable or not following this rule:
  Even if DmiVcp is Enable, if IsocEn is Disable then DmiVcp should be considered Disable
  since DmiVcp cannot be enabled if isoc is not enabled

Arguments:
  IioGlobalData - IIO Global data structure

Returns:
  TRUE if REALLY enable, else FALSE to disable 

--*/
{
  if (IioGlobalData->IioVData.IsocEnable == 0) {
    return FALSE;
  } else {
    if(IioGlobalData->SetupData.DmiVcp == 0) {
      return FALSE;
    }
  }

  return TRUE;
}

BOOLEAN
OemIdentifyLegacyCbDmaDevice(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given belongs to Intel's Xeon server legacy
  CBDMA device address

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - CBDMA device BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  UINT8  Iio;

  if(Device == 0xFF && Function == 0xFF)
    return FALSE;
  if( !Bus && !Device && !Function )  //it cannot be the first device in root complex
      return FALSE;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }
  if(Iio >= MaxIIO)
    return FALSE;

  //
  // leagcy CBDMA have fixed PCI address, the bus# equal IIO bus# & device#
  // is legacy fixed value of 4
  //
  if(Device == IOAT_DEVICE_NUM){
    //
    // if Class code matches the Other system peripheral device  base class (08), subclass (80h),
    // Reg. Prog interface(0), than it is the regular CBDMA device
    //
    if(GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_OTHER_SYS_PERIPHERAL)
      return TRUE;              //return TRUE to indicate that the PCI address belongs to CBDMA device
    else
      return FALSE;             //return FALSE as it does not belong to CBDMA
  }
  else
    return FALSE;
}

BOOLEAN
OemIdentifyCbDmaDevice(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given belongs to Intel's CBDMA device

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - CBDMA device BDF

Returns:
  TRUE if valid, else FALSE

--*/
{
  UINT8  Iio;

  if(Device == 0xFF && Function == 0xFF)
    return FALSE;
  if( !Bus && !Device && !Function )  //it cannot be the first device in root complex
      return FALSE;
  //
  // determine whether it is in legacy CPU IIO/uncore bus#
  //
  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus ||
       IioGlobalData->IioVData.SocketUncoreBusNumber[Iio] == Bus )
      return OemIdentifyLegacyCbDmaDevice( IioGlobalData, Bus, Device, Function);
  }
  //
  // if Class code matches the Other system peripheral device  base class (08), subclass (80h),
  // Reg. Prog interface(0), than it is the regular CBDMA device
  //
  if(GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_OTHER_SYS_PERIPHERAL)
    return TRUE;              //return TRUE to indicate that the PCI address belongs to CBDMA device
  else
    return FALSE;             //return FALSE as it does not belong to CBDMA
}

UINT8
OemGetVppInvertRegValue(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Iio
)
/*++

Routine Description:
  OEM Hook to get the VPP Invert value for a board

Arguments:
  IioGlobalData - IIO Global data structure
  Iio           - Socket number


Returns:
  VPP Invert register value

--*/
{
  UINT8 VppInvert = 0;
  //
  // VPP_INVERT variable is initialzed based on hot-plug capable platform
  //
  if (IioGlobalData->IioVData.CpuType == CPU_BDX)
    VppInvert = IioGlobalData->BdxSetupData.VPP_INVERT[Iio];

  
  return VppInvert;
  
}

BOOLEAN
SelectLatestGen3CTLEpTable(
    IIO_GLOBALS  *IioGlobalData,
    UINT8         Iio,
    UINT8         Dev,
    UINT8         Func
)
{
  UINT8         PortIndex = 0;

  PCIEDEVFUNC2PORTINDEX(PortIndex, Dev, Func);

  //select the latest default table for Gen3 CTLEp for BDX-EP B0 or greater if Gen3 EQ mode is not equal to option GEN3_EQ_MODE_ALT_SHORT_CHANNEL
  if ((IioGlobalData->IioVData.CpuStepping >= B0_REV_BDX) &&
      (IioGlobalData->SetupData.Gen3EqMode[Iio*NUMBER_PORTS_PER_SOCKET + PortIndex] != 0x000F ))
    return TRUE;
  else
    return FALSE;
}

IIO_STATUS
OemGetCTLEInfo (
  IIO_GLOBALS  *IioGlobalData,
  UINT8         Iio,
  UINT8         Dev,
  UINT8         Func,
  UINT32        *OrValueCtle_0,
  UINT32        *OrValueCtle_1,
  UINT32        *AndValueCtle_0,
  UINT32        *AndValueCtle_1,
  UINT8         LinkSpeed
  )
{
  UINT8         i;
  IIO_CTLEP	    *CtlepTable;
  UINT32        CtlepTableSize;

  if (IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX) {
    switch( LinkSpeed )
    {
      case 1:
        CtlepTable     = Gen1CTLEpInfo;
        CtlepTableSize = sizeof(Gen1CTLEpInfo);
        break;
      case 2:
        CtlepTable     = Gen2CTLEpInfo;
        CtlepTableSize = sizeof(Gen2CTLEpInfo);
        break;
      case 3:
        if(SelectLatestGen3CTLEpTable(IioGlobalData, Iio, Dev, Func) == TRUE){
          CtlepTable     = Gen3CTLEpInfo_BX;
          CtlepTableSize = sizeof(Gen3CTLEpInfo_BX);
        }
        else{
          CtlepTable     = Gen3CTLEpInfo;
          CtlepTableSize = sizeof(Gen3CTLEpInfo);
        }
        break;
      default:
        IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "Link Speed Not Supported \n");
        return 0xFF;  // EFI_UNSUPPORTED
    }

    for (i = 0; i < (CtlepTableSize/sizeof(IIO_CTLEP)); i++)
    {
      if ( (CtlepTable[i].Iio == Iio || CtlepTable[i].Iio == 0xFF) && (CtlepTable[i].DfxDevice == Dev) && (CtlepTable[i].DfxFunction == Func)
    		  && (CtlepTable[i].PlatformType == IioGlobalData->IioVData.PlatformType || CtlepTable[i].PlatformType == 0xFF))
      {
        *OrValueCtle_0   = CtlepTable[i].OrValueCtle_0;
        *OrValueCtle_1   = CtlepTable[i].OrValueCtle_1;
        *AndValueCtle_0  = CtlepTable[i].AndValueCtle_0;
        *AndValueCtle_1  = CtlepTable[i].AndValueCtle_1;

        return IIO_SUCCESS;
      }
      else
        continue;  // CTLE value not applicable to this port
    }  // for

  } // CpuFamily == CPU_FAMILY_BDX

  // If no condition is met
  return 0xFF; // EFI_UNSUPPORTED
}

BOOLEAN
OemOverrideMiscCtlep(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         BusNnmber,
  UINT8         Device,
  UINT8         Function
)
{
  return FALSE; 
}