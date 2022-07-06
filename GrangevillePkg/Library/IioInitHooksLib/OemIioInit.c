/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
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
#include <Token.h> // AptioV Server override
#include <Library\OemIioInit.h>
#include "OemIioInitCommon.h"
#include "PchRegs.h"
#include "Platform.h"
#include "IoAccess.h"


#ifdef  DE_SKU
#define IS_BDX_DE_IOSF_DEVICE_FUNCTION( Device , Function ) \
  ( \
    (Device == PCIE_PORT_2_DEV)  && \
    ( (Function == PCIE_PORT_2A_FUNC)  ||    /*D2:F0*/ \
      (Function == PCIE_PORT_2B_FUNC)  ||    /*D2:F1*/ \
      (Function == PCIE_PORT_2C_FUNC)  ||    /*D2:F2*/ \
      (Function == PCIE_PORT_2D_FUNC) )      /*D2:F3*/ \
  ) 
#endif  //DE_SKU

IIO_CTLEP  Gen3CTLEpInfo[] = {
  //
  // Gen3 CTLe Table for Durango DVP platform
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0,   PCIE_PORT_0_DFX_DEV,     PCIE_PORT_0_DFX_FUNC,    0xFFFFFC00,        0x0,            0x3de,            0x0,              0xFF},      //for port 0
  { 0,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0x000f7bde,       0x0,              0xFF},      //for S0 port 1A, 1B
  { 0,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,       0x000003de,       0xFF},      //for S0 port 3A, 3B, 3C , 3D
  { 0,   0xFF,                    0xFF,                    0xFFFFFFFF,        0xFFFFFFFF,     0x00000000,       0x00000000,       0xFF}       //Dummpy entry 
};

//
// Gen2 CTLe Table
//
IIO_CTLEP  Gen2CTLEpInfo[] = {
  //
  // Soc       DfxDev                      DfxFunc        AndValueCtlep_0    AndValueCtlep_1  OrValueCtlep_0    OrValueCtlep_1     PlatformType
  //
  { 0,   PCIE_PORT_0_DFX_DEV,     PCIE_PORT_0_DFX_FUNC,    0xFFFFFC00,        0x0,            0x3de,             0x0,             0xFF},         //for port 0
  { 0,   PCIE_PORT_1A_DFX_DEV,    PCIE_PORT_1A_DFX_FUNC,   0xFFF00000,        0x0,            0xf7bde,           0x0,             0xFF},         //for port 1A, 1B
  { 0,   PCIE_PORT_3A_DFX_DEV,    PCIE_PORT_3A_DFX_FUNC,   0xC0000000,        0xFFFFFC00,     0x3def7bde,        0x3de,           0xFF}          //for port 3A, 3B, 3C , 3D
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

  StartCount = IioIoRead32(IioGlobalData, (UINT16)(IioGlobalData->IioVData.PmBase + 0x8) ) & 0xFFFFFF;

  while(1) {
    StopCount = IioIoRead32 ( IioGlobalData, (UINT16)(IioGlobalData->IioVData.PmBase + 0x8)) & 0xFFFFFF;
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
  IioGlobalData->IioVData.PlatformType = mPlatformInfo->BoardId;
  IioGlobalData->IioVData.PcieRiser1Type = mPlatformInfo->PcieRiser1Type;
  IioGlobalData->IioVData.PcieRiser2Type = mPlatformInfo->PcieRiser2Type;
  IioGlobalData->SetupData.DmiVc1 = mPlatformInfo->DmiVc1;
  IioGlobalData->SetupData.DmiVcp = mPlatformInfo->DmiVcp;
  // TODO: Check logic for DmiVcm
  IioGlobalData->SetupData.DmiVcm = mPlatformInfo->DmiVcm;

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

#endif // DE_SKU

  IioGlobalData->BdxSetupData.PCIe_Enable_SSD_SUPPORT = SetupData->PCIe_Enable_SSD_SUPPORT;
  IioGlobalData->BdxSetupData.IioLinkTrainPhase = SetupData->IioLinkTrainPhase;

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
  Status = GetSystemVariable(&SetupData);

  //
  // Update the global data structure with the system configuration data
  //

  IioGlobalData->IioVData.PchIoApicBase = PCH_IOAPIC_ADDRESS;
  IioGlobalData->IioVData.PmBase = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(0, PCH_LPC_PCI_DEVICE_NUMBER, PCH_LPC_PCI_FUNCTION_NUMBER, 0x40)) & ~(1 << 0);
  IioGlobalData->IioVData.PchRcbaAddress = OemGetPchRootComplexBaseAddress(IioGlobalData);
  IioGlobalData->IioVData.GpioBase = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(0, PCH_LPC_PCI_DEVICE_NUMBER, PCH_LPC_PCI_FUNCTION_NUMBER, GPIO_BASE_REG)) & 0xFFF0;
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


Returns:

  VOID              All other error conditions encountered result in an ASSERT
                    IioGlobalData is completely initialized for use by module code!
--*/
{
  UINT8                         PortIndex;
  UINT8                         Direction = 0;
  
  for (PortIndex = 0; PortIndex < NUMBER_PORTS_PER_SOCKET; PortIndex++) {
    IioGlobalData->SetupData.PEXPHIDE[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] = 0;
    IioGlobalData->SetupData.HidePEXPMenu[IioIndex * NUMBER_PORTS_PER_SOCKET + PortIndex] = 0;
  }

/*
  Platform list for reference

    TypeBeverlyCoveCRB = 1,
    TypeBeverlyCoveSODimmCRB,

*/
//@tbd-grt: Many changes as compared to Brklnd.Confirm for GRT.
	switch (IioGlobalData->IioVData.PlatformType)  {


  case TypeTrabucoCanyon:
    // Source: TrabucoCanyon HW spec
    //
    // Socket 0
    //
    if (IioIndex == 0){

/*
      IIO port# * B:D:F * BIOS port# * slot#
      Port Mappings
      DMI - Port 0
      IOU2 - Ports 1A and 1B
      IOU0 - Ports 2A, 2B, 2C, 2D
      IOU1 - Ports 3A, 3B, 3C, 3D
      CPU0:
      IOU0: -- x8 -- x8  IOSF Bridge (for Integrated MAC and CBDMA)
      IOU2: -- x8        x8 Slot on the mother board
      IOU1: -- x8 -- x8  Ports 3A, 3B NTB Ports and Ports 3C and 3D SAS controller

      IIO port# * B:D:F * BIOS port# * slot#
      IOU2: -- x8        1B * 0:1:1 * P2  * SLOT2_7:4  | 1A * 0:1:0 * P1 * SLOT2_3:0
*/

      if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxx8xxx8;
      }

      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                               IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      // Add slot implemented details...
      // Slot 1
      //
      IioGlobalData->SetupData.SLOTIMP[1] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[1] = 2;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[1] = 0;   // Electromechanical Interlock Present

     // NTB Auto Config
     if (IioGlobalData->SetupData.NtbPpd[0] == NTB_PORT_DEF_NTB_AUTO) // If auto config is set, configure the port as NTB to NTB.
       IioGlobalData->BdxSetupData.NtbPpdAutoConfig[0] = NTB_PORT_DEF_NTB_NTB; //NTB_PORT_DEF_NTB_NTB


      // Setup NTBXlinkAutoConfig based on which canister we are running on.
     if ((IioGlobalData->SetupData.NtbPpd[0] == NTB_PORT_DEF_NTB_NTB || IioGlobalData->SetupData.NtbPpd[0] == NTB_PORT_DEF_NTB_AUTO)) {
       if (IioGlobalData->SetupData.NtbXlinkCtlOverride[0] == NTB_XLINK_CTL_AUTO) { // Auto Config
         if (IoRead32(IioGlobalData->IioVData.GpioBase + 0xC) & (1 << 24)) {//if GPIO24 is 1, then it is canister2, else canister 1
           DEBUG((EFI_D_INFO, "Canister 2. DSD/USP\n"));
           IioGlobalData->BdxSetupData.NtbXlinkCtlAutoConfig[0] = NTB_XLINK_CTL_DSD_USP; // DSD/USP
           // Set the upstream/downstream bit for the IDT retimer.
           Direction = Downstream;
         }
         else {
           DEBUG((EFI_D_INFO, "Canister 1. USD/DSP\n"));
           IioGlobalData->BdxSetupData.NtbXlinkCtlAutoConfig[0] = NTB_XLINK_CTL_USD_DSP; // USD/DSP
           Direction = Upstream;
         }
       }
       else if (IioGlobalData->SetupData.NtbXlinkCtlOverride[0] == NTB_XLINK_CTL_DSD_USP) {
         // DSD/USP
         Direction = Downstream;
       }
       else if (IioGlobalData->SetupData.NtbXlinkCtlOverride[0] == NTB_XLINK_CTL_USD_DSP) {
         // USD/DSP
         Direction = Upstream;
       }
     }
     else if (IioGlobalData->SetupData.NtbPpd[0] == NTB_PORT_DEF_NTB_RP) { // For NTB - RP mode the hardware directly sets it to DSD/USP. So make sure we set the same on Retimer also.
       Direction = Downstream;
     }
     else { // It is a transparent bridge, set it as Upstream device
       Direction = Upstream;
     }

     // Set the direction bit correctly in the IDT retimer.
     if (Direction) {
       DEBUG((EFI_D_INFO, "Setting IDT retimer as %s\n", (Direction == Upstream) ? "Upstream" : "Downstream"));
       SetIDTRetimerDirection(Direction);
     }
    }
  break;
  case TypeCamelbackMountain:
    // Source: TypeCamelbackMountain HW spec
    //
    // Socket 0
    //
    if (IioIndex == 0){

/*
      IIO port# * B:D:F * BIOS port# * slot#
      Port Mappings
      DMI - Port 0
      IOU2 - Ports 1A and 1B
      IOU0 - Ports 2A, 2B, 2C, 2D
      IOU1 - Ports 3A, 3B, 3C, 3D
      CPU0:
      IOU0: -- x8 -- x8  IOSF Bridge (for Integrated MAC and CBDMA)
      IOU2: -- x8        x8 Slot on the mother board
      IOU1: -- -- -- x16  x16 Slot on the mother board

      IIO port# * B:D:F * BIOS port# * slot#

      IOU2: -- x8        1B * 0:1:1 * P2  * SLOT2_7:4  | 1A * 0:1:0 * P1 * SLOT2_3:0
      IOU1: -- -- -- x16 3D * 0:3:3 * P10 * SLOT1_15:12 | 3C * 0:3:2 * P9 * SLOT1_11:8 | 3B * 0:3:1 * P8 * SLOT1_7:4 | 3A * 0:3:0 * P7 * SLOT1_3:0

*/

      if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
      }

      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                               IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      // Add slot implemented details...
      // Slot 2
      //
      IioGlobalData->SetupData.SLOTIMP[1] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[1] = 2;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[1] = 0;   // Electromechanical Interlock Present

      // Slot 1 , Default
      //
      IioGlobalData->SetupData.SLOTIMP[7] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[7] = 1;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[7] = 0;   // Electromechanical Interlock Present

    }
  break;
  case TypeEchoCanyon:
    // Source: TypeCamelbackMountain HW spec
    //
    // Socket 0
    //
    if (IioIndex == 0){

/*
      IIO port# * B:D:F * BIOS port# * slot#
      Port Mappings
      DMI - Port 0
      IOU2 - Ports 1A and 1B
      IOU0 - Ports 2A, 2B, 2C, 2D
      IOU1 - Ports 3A, 3B, 3C, 3D
      CPU0:
      IOU0: -- x8 -- x8  IOSF Bridge (for Integrated MAC and CBDMA)
      IOU2: -- x8        x8 Slot on the mother board
      IOU1: -- -- -- x16  x16 Slot on the mother board

      IIO port# * B:D:F * BIOS port# * slot#

      IOU2: -- x8        1B * 0:1:1 * P2  * SLOT2_7:4  | 1A * 0:1:0 * P1 * SLOT2_3:0
      IOU1: -- -- -- x16 3D * 0:3:3 * P10 * SLOT1_15:12 | 3C * 0:3:2 * P9 * SLOT1_11:8 | 3B * 0:3:1 * P8 * SLOT1_7:4 | 3A * 0:3:0 * P7 * SLOT1_3:0

*/

      if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_x4x4xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
      }

      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                               IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      // Add slot implemented details...
      // Slot 2
      //
      IioGlobalData->SetupData.SLOTIMP[1] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[1] = 2;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[1] = 0;   // Electromechanical Interlock Present

      // Slot 1 , Default
      //
      IioGlobalData->SetupData.SLOTIMP[7] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[7] = 1;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[7] = 0;   // Electromechanical Interlock Present

    }
  break;
  case TypeTlaquepaque:
    // Source: TypeTlaquepaque HW spec
    //
    // Socket 0
    //
    if (IioIndex == 0){

/*
      IIO port# * B:D:F * BIOS port# * slot#
      Port Mappings
      DMI - Port 0
      IOU2 - Ports 1A and 1B
      IOU0 - Ports 2A, 2B, 2C, 2D
      IOU1 - Ports 3A, 3B, 3C, 3D
      CPU0:
      IOU0: -- x8 -- x8  IOSF Bridge (for Integrated MAC and CBDMA)
      IOU2: -- x8        x8 Slot on the mother board
      IOU1: -- -- -- x16 Ports 3A, 3B,3C and 3D are NTB Ports

      IIO port# * B:D : F * BIOS port# * slot#

      IOU2 : --x8        1B * 0 : 1 : 1 * P2  * SLOT2_7 : 4 | 1A * 0 : 1 : 0 * P1 * SLOT2_3 : 0
      IOU1 : -- -- --x16 3D * 0 : 3 : 3 * P10 * SLOT1_15 : 12 | 3C * 0 : 3 : 2 * P9 * SLOT1_11 : 8 | 3B * 0 : 3 : 1 * P8 * SLOT1_7 : 4 | 3A * 0 : 3 : 0 * P7 * SLOT1_3 : 0
*/


      if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
      }

      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                               IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      // Add slot implemented details...
      // Slot 2
      //
      IioGlobalData->SetupData.SLOTIMP[1] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[1] = 2;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[1] = 0;   // Electromechanical Interlock Present

      IioGlobalData->SetupData.SLOTIMP[7] = 1;   // Slot Implemented
      IioGlobalData->SetupData.SLOTPSP[7] = 1;   // Physical Slot #
      IioGlobalData->SetupData.SLOTEIP[7] = 0;   // Electromechanical Interlock Present

    }
  break;
  
 case TypePiestewaPeak:
   if (IioIndex == 0){

/*
      IIO port# * B:D:F * BIOS port# * slot#

      CPU0:
      IOU0: -- x8 -- x8  PCIe Video Connector
      IOU2: -- x8  Not Connected or IOU2: --x4--x4 for PCIe SSD Cache 
      IOU1: -- -- -- x16  Edge Connector
*/

      if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;
      }

      if (IioGlobalData->BdxSetupData.PCIe_Enable_SSD_SUPPORT == 1) {
        IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_x4x4;
      }else{
      	IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
      }

      if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
        IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
      }

      //
      // Hide the root ports whose lanes are assigned preceding ports
      //
      DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                               IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

      DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);
    
    }
    break;

    
  case TypeBeverlyCoveCRB:
  case TypeBeverlyCoveSODimmCRB:
  default:
      // Source: Beverly-Cove HW spec
      //
      // Socket 0
      //
      if (IioIndex == 0){

/*
        IIO port# * B:D:F * BIOS port# * slot#

        CPU0:
        IOU0: -- x8 -- x8  IOSF Bridge
        IOU2: -- x8        Not Connected
        IOU1: -- -- -- x16 Edge Connector
*/

        if (IioGlobalData->SetupData.ConfigIOU0[0] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU0[0] = IIO_BIFURCATE_xxx8xxx8;
        }

        if (IioGlobalData->SetupData.ConfigIOU2[0] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU2[0] = IIO_BIFURCATE_xxx8;
        }
       
        if (IioGlobalData->SetupData.ConfigIOU1[0] == IIO_BIFURCATE_AUTO) {
          IioGlobalData->SetupData.ConfigIOU1[0] = IIO_BIFURCATE_xxxxxx16;
        }

        //
        // Hide the root ports whose lanes are assigned preceding ports
        //
        DeducePEXPHideFromIouBif( IioGlobalData, 2, 0, IioGlobalData->SetupData.ConfigIOU2[0],
                                   IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

        DeducePEXPHideFromIouBif( IioGlobalData, 0, 0, IioGlobalData->SetupData.ConfigIOU0[0],
                                 IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

        DeducePEXPHideFromIouBif( IioGlobalData, 1, 0, IioGlobalData->SetupData.ConfigIOU1[0],
                                   IioGlobalData->SetupData.PEXPHIDE, IioGlobalData->SetupData.HidePEXPMenu);

        //
        // Also hide the ports which are not used
        // 
        IioGlobalData->SetupData.PEXPHIDE[1]     = 1;    // Ports 1A is not routed
        IioGlobalData->SetupData.HidePEXPMenu[1] = 1;

        //
        // Add slot implemented details
        //
        IioGlobalData->SetupData.SLOTIMP[7] = 1;   // Slot Implemented
        IioGlobalData->SetupData.SLOTPSP[7] = 1;   // Physical Slot #
        IioGlobalData->SetupData.SLOTEIP[7] = 0;   // Electromechanical Interlock Present

        //PCISIG requires greater than 25W power on PCIe slot
        IioGlobalData->SetupData.SLOTSPLS[7] = 0;    //PCISIG requires greater than 25W power on PCIe slot
        IioGlobalData->SetupData.SLOTSPLV[7] = 0x1A; //PCISIG requires greater than 25W power on PCIe slot    
      }
    }

  
  // end of Switch for platform type
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
          SecBus = IioPciExpressRead8 (IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
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
                DevCtrl2 = IioPciExpressRead16 (IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, Func, 0xC8));
                DevCtrl2 |= 0x05;  // Completion Timeout Value of 16ms-55ms
                PciExpressWrite16 (PCI_PCIE_ADDR(SecBus, 0, Func, 0xC8), DevCtrl2);
                IioDebugPrintInfo (IioGlobalData, IIO_DEBUG_INFO, "pwu3:bus :%d device 0 function %d set\n",SecBus, Func);
              }
            } else {
              //
              // Also check for 82576 (Kawela) & 82598 (Oplin) Add-in cards.
              // Same clarification for Completion Timeout detailed in 82598 Spec Update v2.7
              //
              DidVid = IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 0, PCI_VENDOR_ID_OFFSET));
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
                  DevCtrl2 = IioPciExpressRead16 (IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 1, 0xC8));
                  DevCtrl2 |= 0x05;  // Completion Timeout Value of 16ms-55ms
                  IioPciExpressWrite16 ( IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 1, 0xC8), DevCtrl2);

                case NIC_82598EB_XFSR_SNGL_DIDVID:
                case NIC_82598EB_AT_SNGL_DIDVID:
                case NIC_82598EB_XFLR_SNGL_DIDVID:
                  //
                  // Single Port NICs just need to check the single function.
                  // Update Completion Timeout Value for function 0.
                  //
                  DevCtrl2 = IioPciExpressRead16 (IioGlobalData, PCI_PCIE_ADDR(SecBus, 0, 0, 0xC8));
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

// AptioV Server Override Begin: Commenting the function as it is not required. We are handling it in OemEarlyUncoreInit() function.
/*
VOID
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
}
*/
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
  // 4986450 ESCALATE from hexaii_hsx_mock:C0:  Virtual channels not disabled when Isoc disabled via secondary means
  // QPI could have changed the IsocEn and MeSegEn Save the possible changes into CommonRcConfig. 
  CopyMem ( 
             &SetupInfo.CommonRcConfig.IsocEn, 
             &IioGlobalData->IioVData.IsocEnable, 
             sizeof(SetupInfo.CommonRcConfig.IsocEn)
           );
  CopyMem ( 
             &SetupInfo.CommonRcConfig.MeSegEn, 
             &IioGlobalData->IioVData.MeSegEn, 
             sizeof(SetupInfo.CommonRcConfig.MeSegEn)
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
    IIO_GLOBALS     *IioGlobalData,
    UINT32     IOUx,
    UINT32     IIOn,
    UINT8     CurrentIOUConfigValue,
    UINT8     *PXPHide,
    UINT8     *HidePEXPMenu
    )
{

#ifdef IA32
  if( CurrentIOUConfigValue == 0xFF){
#ifdef SERIAL_DBG_MSG //AptioV Grangeville Override 	  
    rcPrintf ((IioGlobalData->IioOutData.host, "ERROR: Invalid IOUx Bifurcation =%x\n", CurrentIOUConfigValue));
#endif
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
    case TypeBeverlyCoveCRB:
    case TypeBeverlyCoveSODimmCRB:
      if (IioGlobalData->IioVData.SocketPresent[Iio]) {
        switch(Iio){
          case  0:  //IIO 0
            PortDisableBitMap = BIT1 | BIT2;	//Port 1A, 1B (D1F0, D1F1) not routed for TR IIO0
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
  // Get the current DMiVcx values based on Isoc/MeSeg knobs chaged on PlatformInfo.c
  //
  
  if (IioGlobalData->IioVData.IsocEnable == 0) {
	DEBUG((EFI_D_INFO, "IsocEn changed because QPI config. IsocEn =0\n")); 
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
  if( CheckSoftSimulationPlatform() && (Bus == 0 && Device == 0 && Function == 0) ) 
    return TRUE;              // CCR register is not available on simics for now , so directly comparing the BDF
  else 
  if(GetPortClassCode(IioGlobalData, Bus, Device, Function) == V_PCIE_CCR_PCI_HOST_BRIDGE)
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
  UINT8  j, Iio;
  
  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }
  if(Iio >= MaxIIO)
    return FALSE;

  if( DfxArguments == TRUE ) {
    for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      if( (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice == Device) &&
          (IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction == Function) ){
          break;
      } // if
    } // for
    if( j >= NUMBER_PORTS_PER_SOCKET)
      return FALSE;
    else {  // map the DfxDevice , DfxFunction
      Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    } // if       
  } // DfxArguments

#ifdef DE_SKU   // For PPO , since HSX cpu will not have IOSF return False
  if( ((IioGlobalData->IioVData.CpuFamily) == CPU_FAMILY_BDX_DE) && (IS_BDX_DE_IOSF_DEVICE_FUNCTION(Device, Function)) )
    return TRUE;
  else
#endif   
    return FALSE;   
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
LocateDeviceTypeInPciBus(
        IIO_GLOBALS *IioGlobalData,
	UINT8	MinBus,
	UINT8	MaxBus,
	UINT32	DeviceTypeClass,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    *DevPciAddress
)
/*++

Routine Description:
  Helper routine which scans the given PCI bus range for the Device Class code type, and returns it PCI address

Arguments:
  MinBus, MaxBus - the low and high bus range to scan
  DeviceTypeClass - PCI Spec compliant device class code value
  DevPciAddress - outputs PCI address in BDF format if found
Returns:
  TRUE if given device class code found in the bus range along wiht the PCI address, else FALSE with no PCI address

--*/
{
  UINT8	    Bus, Dev, Func;
  UINT32    Data32;

  for(Bus = MinBus; Bus <= MaxBus; Bus++){
	for(Dev = 0; Dev < TOTAL_PCI_DEVICE_PER_BUS; Dev++){
	  for(Func = 0; Func < TOTAL_PCI_FUNCTION_PER_DEVICE; Func++){
		Data32 = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(Bus,Dev,Func,PCIE_VID_REGISTER));
		if(Data32 == V_PCIE_DEVICE_DO_NOT_EXIST || !Data32){
		  if(!Func)
			break;	//skip to next device
		  else
			continue;	//skip to next fucntion
		}

		Data32 = IioPciExpressRead32(IioGlobalData, PCI_PCIE_ADDR(Bus,Dev,Func,PCIE_RID_REGISTER));

		if((Data32 >> 8) == DeviceTypeClass){
		  DevPciAddress->Bus = Bus;
		  DevPciAddress->Device = Dev;
		  DevPciAddress->Function = Func;
		  return TRUE;
		}
		else{
		  continue;		//scan the next PCI function
		}
	  }  // for pci function
	}  //for pci device

	if (Bus == 0xFF) {
      break;
	}
  }  //for pci bus
  return FALSE;
}

#ifdef  DE_SKU
BOOLEAN
OemIdentifyVendorSpecificPort_CBDMA_CPM(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is Intel specific bridge device (IOSF) port for CBDMA (DE & NS Storage Sku) or CPM (NS Comms Sku)

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if known IIO port, else FALSE

--*/
{
  UINT8  Iio;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }
  if(Iio >= MaxIIO)
    return FALSE;


  if( ((IioGlobalData->IioVData.CpuFamily) == CPU_FAMILY_BDX_DE) && (IS_BDX_DE_IOSF_CBDMA_CPM_DEVICE_FUNCTION(Device, Function)) )
    return TRUE;
  else
    return FALSE;
}

BOOLEAN
OemIdentifyVendorSpecificPort_GbE(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is Intel specific bridge device (IOSF) port for GbE

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if known IIO port, else FALSE

--*/
{
  UINT8  Iio;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }
  if(Iio >= MaxIIO)
    return FALSE;


  if( ((IioGlobalData->IioVData.CpuFamily) == CPU_FAMILY_BDX_DE) && (IS_BDX_DE_IOSF_GBE_DEVICE_FUNCTION(Device, Function)) )
    return TRUE;
  else
    return FALSE;
}

BOOLEAN
OemIdentifyVendorSpecificPort_CPM_GbE1(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Bus,
  UINT8         Device,
  UINT8         Function
)
/*++

Routine Description:
  OEM Hook to check whether the BDF given is Intel specific bridge device (IOSF) port for CPM (NS Storage Sku) /GbE1 (NS Comms Sku)

Arguments:
  IioGlobalData - IIO Global data structure
  Bus, Device, Function - PCIE port BDF

Returns:
  TRUE if known IIO port, else FALSE

--*/
{
  UINT8  Iio;

  for(Iio = 0; Iio < MaxIIO; Iio++){
    if(IioGlobalData->IioVData.SocketBaseBusNumber[Iio] == Bus)
      break;
  }
  if(Iio >= MaxIIO)
    return FALSE;
 
  if( ((IioGlobalData->IioVData.CpuFamily) == CPU_FAMILY_BDX_DE) && (IioGlobalData->IioVData.SkuType == SKU_NS) && (IS_BDX_DE_IOSF_CPM_GBE1_DEVICE_FUNCTION(Device, Function)) )
    return TRUE;
  else
    return FALSE;
}

BOOLEAN
OemRetrieve_IOSF_CBDMA_CPM_PciAddress(
	IIO_GLOBALS                                    *IioGlobalData,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    *CbdmaPciAddress
)
/*++

Routine Description:
  OEM hooks which scans the given PCIe bridge port, and returns the PCI address of the CBDMA device (DE & NS Storage Sku) or CPM device (NS Comms Sku)

Arguments:
  PciAddress - the root port to search for the CBDMA endpoint device
  CbdmaPciAddress - Outputs CBDMA PCI address if found
Returns:
  TRUE if CBDMA device found in RP along with its PCI address, else FALSE with no PCI address

--*/
{
  UINT8	SecBusNo, SubBusNo;
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT  PcuF3CAP1; 

  // make sure the RP is valid IOSF bridge for CBDMA
  if(OemIdentifyVendorSpecificPort_CBDMA_CPM(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
	return FALSE;

  SecBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SECONDARY_BUS_REGISTER));
  SubBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SUBORDINATE_BUS_REGISTER));

  
  PcuF3CAP1.Data =  IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR (IioGlobalData->IioVData.SocketUncoreBusNumber[0],
													PCU22_DEV_NUM,
													PCU22_FUNC_NUM,
													ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));

  if ((IioGlobalData->IioVData.SkuType == SKU_NS) && (PcuF3CAP1.Bits.storage_seg_sel == 0)) {
  	// NS Comms Sku
    return LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_CPM_COPROCESSOR, CbdmaPciAddress);
  } else {
    // DE / NS Storage Sku
    return LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_OTHER_SYS_PERIPHERAL, CbdmaPciAddress);
  }
}

BOOLEAN
OemRetrieve_IOSF_GbE_PciAddress(
	IIO_GLOBALS                                    *IioGlobalData,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    *GbePciAddress
)
/*++

Routine Description:
  OEM hooks which scans the given PCIe bridge port, and returns the PCI address of the GbE device

Arguments:
  PciAddress - the root port to search for the GbE endpoint device
  GbePciAddress - Outputs GbE PCI address if found
Returns:
  TRUE if GbE device found in RP along with its PCI address, else FALSE with no PCI address

--*/
{
  UINT8	SecBusNo, SubBusNo;

  // make sure the RP is valid IOSF bridge for GbE
  if(OemIdentifyVendorSpecificPort_GbE(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
  	return FALSE;

  SecBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SECONDARY_BUS_REGISTER));
  SubBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SUBORDINATE_BUS_REGISTER));

  if(LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_ETHERNET_CONTROLLER, GbePciAddress) == TRUE)
	return TRUE;
  else
	return LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_OEM_DUMMY_FUNCTION, GbePciAddress);
}

BOOLEAN
OemRetrieve_IOSF_CPM_GbE1_PciAddress(
	IIO_GLOBALS                                    *IioGlobalData,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
	PCI_ROOT_BRIDGE_PCI_ADDRESS    *CpmPciAddress
)
/*++

Routine Description:
  OEM hooks which scans the given PCIe bridge port, and returns the PCI address of the CPM (NS Storage Sku) /GbE1 (NS Comms Sku)

Arguments:
  PciAddress - the root port to search for the CPM endpoint device
  CpmPciAddress - Outputs CPM/GbE1 PCI address if found, CPM for Storage Sku; GbE1 for Comms Sku.
Returns:
  TRUE if GbE device found in RP along with its PCI address, else FALSE with no PCI address

--*/
{
  UINT8	SecBusNo, SubBusNo;
  CAPID1_PCU_FUN3_HSX_BDX_STRUCT  PcuF3CAP1; 
 
  // make sure the RP is valid IOSF bridge for CPM
  if(OemIdentifyVendorSpecificPort_CPM_GbE1(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
  	return FALSE;

  PcuF3CAP1.Data =  IioPciExpressRead32 (IioGlobalData, PCI_PCIE_ADDR (IioGlobalData->IioVData.SocketUncoreBusNumber[0],
                                                  PCU22_DEV_NUM,
                                                  PCU22_FUNC_NUM,
                                                  ONLY_REGISTER_OFFSET(CAPID1_PCU_FUN3_REG)));

  SecBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SECONDARY_BUS_REGISTER));
  SubBusNo = IioPciExpressRead8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus,PciAddress.Device,PciAddress.Function,PCIE_SUBORDINATE_BUS_REGISTER));

  if (!PcuF3CAP1.Bits.storage_seg_sel) {
  	// NS Comms Sku
    if(LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_ETHERNET_CONTROLLER, CpmPciAddress) == TRUE)
	  return TRUE;
    else
	  return LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_OEM_DUMMY_FUNCTION, CpmPciAddress);
  } else {
    // NS Storage Sku 
    return LocateDeviceTypeInPciBus(IioGlobalData, SecBusNo, SubBusNo, V_PCIE_CCR_CPM_COPROCESSOR, CpmPciAddress);
  }
}

#endif  //DE_SKU

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
  VppInvert = IioGlobalData->BdxSetupData.VPP_INVERT[Iio];

  return VppInvert;
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

  if (IioGlobalData->IioVData.CpuFamily == CPU_FAMILY_BDX_DE) {
    switch( LinkSpeed )
    {
      case 1:
        return 0xFF;  // No Gen1 Table BDX-DE
      case 2:
        CtlepTable     = Gen2CTLEpInfo;
        CtlepTableSize = sizeof(Gen2CTLEpInfo);
        break;
      case 3:
        CtlepTable     = Gen3CTLEpInfo;
        CtlepTableSize = sizeof(Gen3CTLEpInfo);
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

  } // CpuFamily == CPU_FAMILY_BDX_DE

  // If no condition is met
  return 0xFF; // EFI_UNSUPPORTED
}  

#ifdef IA32
EFI_STATUS IDTSmBusRead(EFI_PEI_SMBUS2_PPI    *Smbus, UINT16 Address, UINTN SmbusLength, UINT8* Buffer)
#else
EFI_STATUS IDTSmBusRead(EFI_SMBUS_HC_PROTOCOL    *Smbus, UINT16 Address, UINTN SmbusLength, UINT8* Buffer)
#endif
{
  EFI_STATUS Status = 0;
  EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;

  SmbusLength = 3;
  SmbusCommand = 0x43; // CCODE
  Buffer[0] = 0x1f; //CMD Read
  Buffer[1] = (UINT8)(Address & 0xFF); //ADDRL
  Buffer[2] = (UINT8)((Address >> 8) & 0xFF); ///ADDRH

  SlaveAddress.SmbusDeviceAddress = (IDT89HT0816AP_SMBUS_ADDR >> 1);
  

  Status = Smbus->Execute(Smbus,
    SlaveAddress,
    SmbusCommand,
    EfiSmbusWriteBlock,
    FALSE,
    &SmbusLength,
    Buffer);

  SmbusLength = 10;
  Status = Smbus->Execute(Smbus,
    SlaveAddress,
    SmbusCommand,
    EfiSmbusReadBlock,
    FALSE,
    &SmbusLength,
    Buffer);

  return Status;
}


#ifdef IA32
EFI_STATUS IDTSmBusWrite(EFI_PEI_SMBUS2_PPI    *Smbus, UINT16 Address, UINTN SmbusLength, UINT8* Buffer)
#else
EFI_STATUS IDTSmBusWrite(EFI_SMBUS_HC_PROTOCOL    *Smbus, UINT16 Address, UINTN SmbusLength, UINT8* Buffer)
#endif
{
  EFI_STATUS Status = 0;
  EFI_SMBUS_DEVICE_ADDRESS  SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;

  SmbusLength = 7;
  SmbusCommand = 0x43; // CCODE
  Buffer[0] = 0xf; //CMD Write
  Buffer[1] = (UINT8)(Address & 0xFF); //ADDRL
  Buffer[2] = (UINT8)((Address >> 8) & 0xFF); ///ADDRH

  SlaveAddress.SmbusDeviceAddress = (IDT89HT0816AP_SMBUS_ADDR >> 1);
  
  Status = Smbus->Execute(Smbus,
    SlaveAddress,
    SmbusCommand,
    EfiSmbusWriteBlock,
    FALSE,
    &SmbusLength,
    Buffer);

  SmbusLength = 10;
  Status = Smbus->Execute(Smbus,
    SlaveAddress,
    SmbusCommand,
    EfiSmbusReadBlock,
    FALSE,
    &SmbusLength,
    Buffer);

  return Status;
}

EFI_STATUS
SetIDTRetimerDirection(
IDT_RETIMER_DIR dir
)
{
  EFI_STATUS                     Status;
  UINT8                          Buffer[10] = { 0 };
#ifdef IA32
  EFI_PEI_SMBUS2_PPI             *Smbus = NULL;
#else
  EFI_SMBUS_HC_PROTOCOL          *Smbus = NULL;
#endif // IA32

  
  Status = LocateSmBus((VOID **)&Smbus);

  if (Status != EFI_SUCCESS || Smbus == NULL) {
    DEBUG((EFI_D_ERROR, "!!!!Get SMBus protocol error for IDT Retimer %x\n", Status));
    return Status;
  }
  // Read DeviceID and Vendor Id to make sure it is IDT device
  Status = IDTSmBusRead(Smbus, 0, 4, Buffer);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "Error Reading IDT device ID\n"));
    return Status;
  }

  if (Buffer[3] != (UINT8)(IDT89HT0816AP_VID & 0xFF) || Buffer[4] != (UINT8)(IDT89HT0816AP_VID >> 8 & 0xFF)) {
    DEBUG((EFI_D_ERROR, "Could not locate IDT device with device id 0x%x\n", IDT89HT0816AP_VID));
    return Status;
  }

  Status = IDTSmBusRead(Smbus, 2, 4, Buffer);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "Error Reading IDT device control register\n"));
    return Status;
  }
  DEBUG((EFI_D_INFO, "DCR: Before - 0x%x%x%x%x\n", Buffer[6], Buffer[5], Buffer[4], Buffer[3]));
  // REGUNLOCK Bit 1, WEN Bit14
  Buffer[3] |= (UINT8)BIT1;
  Buffer[4] |= (UINT8)(BIT14 >> 8);
  // It is expected that IDT EEPROM will hold the retimer in Reset untill BIOS comes and resets.
  Status = IDTSmBusWrite(Smbus, 2, 4, Buffer);

  // Set DIR bit (bit 12) , Enable Rx Termination(Bit 13), and Clear WEN(Bit 14)
  if (dir == Upstream)
    Buffer[4] |= (UINT8)(BIT12 >> 8);
  else if (dir == Downstream)
    Buffer[4] &= ~(UINT8)(BIT12 >> 8);
 // Buffer[4] |= (UINT8)(BIT13 >> 8);
  Buffer[4] &= ~(UINT8)(BIT14 >> 8);
  //Status = IDTSmBusWrite(Smbus, 2, 4, Buffer);

  DEBUG((EFI_D_INFO, "Clear RSTHLT bit \n"));
  //Clear RSTHLT bit Reg Unlock bit
  Buffer[3] &= ~(UINT8)BIT2;
  Buffer[3] &= ~(UINT8)BIT1;
  Status = IDTSmBusWrite(Smbus, 2, 4, Buffer);

  return Status;

}

/*
To verify whether board is TBC Beta Board or not based on presence of IDT Device ID
*/

BOOLEAN
IsTBCBetaBoard(
  IIO_GLOBALS *IioGlobalData,
  UINT8       BusNnmber,
  UINT8       Device,
  UINT8       Function
)
{
  EFI_STATUS                     Status;
  UINT8                          Buffer[10] = { 0 };
#ifdef IA32
  EFI_PEI_SMBUS2_PPI             *Smbus = NULL;
#else
  EFI_SMBUS_HC_PROTOCOL          *Smbus = NULL;
#endif // IA32

  if (IioGlobalData->IioVData.PlatformType != TypeTrabucoCanyon)
    return FALSE;

  if (Device != PCIE_PORT_3_DEV || Function != PCIE_PORT_3A_FUNC)
    return FALSE;

  DEBUG((EFI_D_ERROR, "NTB port present on D%x , F%x \n",Device,Function));    
  
  Status = LocateSmBus((VOID **)&Smbus);

  if (Status != EFI_SUCCESS || Smbus == NULL) {
    DEBUG((EFI_D_ERROR, "!!!!Get SMBus protocol error for IDT Retimer %x\n", Status));
    return FALSE;;
  }
  // Failure to read IDT DeviceID and Vendor Id indicates TBC Beta board
  Status = IDTSmBusRead(Smbus, 0, 4, Buffer);
  if (Status != EFI_SUCCESS) {
    DEBUG((EFI_D_ERROR, "TBC Beta Board since IDT device not present \n"));
    return TRUE;
  }
  return FALSE; // Device ID present indicates NOT a TBC beta board
}

BOOLEAN
OemOverrideMiscCtlep(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         BusNnmber,
  UINT8         Device,
  UINT8         Function
)
{
  return IsTBCBetaBoard(IioGlobalData, BusNnmber, Device, Function); 
}