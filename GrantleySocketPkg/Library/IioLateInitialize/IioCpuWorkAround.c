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

  IioCpuWorkAround.c

Abstract:

  Houses all code related to CPU workarounds 

Revision History:

--*/

#include "IioLateInitialize.h"
#include <Library\PciExpressLib.h>

#define TEMP_BUSBASE_OFFSET       1
#define GEN3_CAPID_PCIE           0x10
#define MAX_LINK_DEGRADE_RETRIES  3
typedef UINT32 GEN3_STATUS;
static UINTN BifurcationMap[5][4] = {{ 4, 4, 4, 4},
                                     { 8, 0, 4, 4},
                                     { 4, 4, 8, 0},
                                     { 8, 0, 8, 0},
                                     {16, 0, 0, 0} };

VOID
IioCpuWorkAround (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData,
    IIO_INIT_ENUMERATION                                InitPhase,
    VOID                                                *Buffer
  )
/*++

Routine Description:

  Do Cpu specific work arounds for future fix items as well as known Errata

Arguments:

  PciAddress        - The Iio controller PCI address
  Phase             - Defines the phase of PCI initialization that the PCI driver is executing. 
                      Either: 
                        EfiPciEnumerationDeviceScanning
                        EfiPciEnumerationBusNumberAssigned
                        EfiPciEnumerationResourceAssigned
  IioGlobalData            - Pointer to IIO Internal Data Store
  InitPhase                - Additional selective code execution decision marker
  Buffer                   - Scratch buffer for data passing if necessary based on InitPhase

Returns:

  EFI_SUCCESS  -  Early platform initialization completed successfully.
  All other error conditions encountered result in an ASSERT.

--*/
{

  IRP_MISC_DFX0_IIO_VTD_HSX_BDX_STRUCT  IrpMiscDfx0;
  //
  //4167453 SB traffic on VCp does not generate critical priority on ring
  //
  if ( IioGlobalData->SetupData.WA_4167453){
    IrpMiscDfx0.Data =  PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(IRP_MISC_DFX0_IIO_VTD_REG)));
    IrpMiscDfx0.Bits.force_no_snp_on_vc1_vcm=0x0;    
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_VTD_DEV, IIO_VTD_FUN, ONLY_REGISTER_OFFSET(IRP_MISC_DFX0_IIO_VTD_REG)), IrpMiscDfx0.Data);
  }
}

/*
 * Generic PCIe helper routines are defined below
 */

VOID
GetLinkStatusLate(
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress,
    LNKSTS_IIO_PCIE_STRUCT   *Lnksts
    )
{
  UINT8 bus = PciAddress.Bus;
  UINT8 device = PciAddress.Device;
  UINT8 func = PciAddress.Function;

  Lnksts->Data = (UINT16)PciExpressRead16(PCI_PCIE_ADDR(bus, device, func,
                    ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG)));
}

VOID
EnableDisablePCIePortLate(
    IIO_GLOBALS                                     *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS     PciAddress,
    BOOLEAN                                         PortEn
    )
{
  LNKCON_IIO_PCIE_STRUCT   LnkCon;
  UINT32  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  UINT8   bus = PciAddress.Bus;
  UINT8   device = PciAddress.Device;
  UINT8   func = PciAddress.Function;

  LnkCon.Data = PciExpressRead16(PCI_PCIE_ADDR(bus, device, func, RegLocation));
  if(PortEn == TRUE)
    LnkCon.Bits.link_disable = 0;   //enable link
  else
    LnkCon.Bits.link_disable = 1;   //disable link
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation), LnkCon.Data);
  IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
}

VOID
IssuePortRetrainLinkLate(
    IIO_GLOBALS                                     *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
    )
{
  LNKCON_IIO_PCIE_STRUCT   LnkCon;
  UINT32  RegLocation = ONLY_REGISTER_OFFSET(LNKCON_IIO_PCIE_REG);
  UINT8   bus = PciAddress.Bus;
  UINT8   device = PciAddress.Device;
  UINT8   func = PciAddress.Function;

  LnkCon.Data = PciExpressRead16(PCI_PCIE_ADDR(bus, device, func, RegLocation));
  LnkCon.Bits.retrain_link = 1;
  S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(bus, device, func, RegLocation), LnkCon.Data);
  IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);
}

BOOLEAN
ForcePortRetrainLate(
    IIO_GLOBALS                                    *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress
)
{
  LNKSTS_IIO_PCIE_STRUCT   LnkSts;
  UINTN                     ExitLoopCnt = 1000;

  IssuePortRetrainLinkLate(IioGlobalData, PciAddress);
  GetLinkStatusLate(PciAddress, &LnkSts);

  while(LnkSts.Bits.link_training && ExitLoopCnt){
    IioStall (IioGlobalData, GEN3_LINK_POLL_TIMEOUT);                //wait for 10ms
    GetLinkStatusLate(PciAddress, &LnkSts);
    ExitLoopCnt--;
  }
  if(!ExitLoopCnt && LnkSts.Bits.link_training){    //if the port is still training after 1s, disable the port
    EnableDisablePCIePortLate(IioGlobalData, PciAddress, FALSE);       //DISABLE THE PORT
    return FALSE;
  }
  return TRUE;
}

VOID
Gen3WaitTillLinkRetrainsLate(
    IIO_GLOBALS *IioGlobalData, 
    UINT8 BusBase,
    UINT8 Device,
    UINT8 Function
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS    PciAddress;
  PciAddress.Bus = BusBase;
  PciAddress.Device = Device;
  PciAddress.Function = Function;

  if(ForcePortRetrainLate(IioGlobalData, PciAddress) == TRUE)
    return;
  else{   //not able to retrain and port is disabled, hence enable again
    EnableDisablePCIePortLate(IioGlobalData, PciAddress, TRUE);
  }
}

GEN3_STATUS
Gen3GetEndpointInfoLate(
    IIO_GLOBALS                      *IioGlobalData,
    UINT8                            Socket,
    UINT8                            Port,
    UINT16                           *Vid, 
    UINT16                           *Did,
    UINT32                           *MaxSpeed,
    UINT32                           *MaxWidth
    )
{
  UINT8                                BusBase;
  UINT32                               RegLocation;
  UINT32                               RegLocation2;
  LNKCAP_IIO_PCIE_STRUCT               LnkCap;
  LNKCAP2_IIO_PCIE_STRUCT              LnkCap2; 
  UINT16                               CapId;
  PBUS_IIO_PCIE_STRUCT                 Pbus;
  SECBUS_IIO_PCIE_STRUCT               Secbus;
  SUBBUS_IIO_PCIE_STRUCT               Subbus;
  GEN3_STATUS                          status;
  UINTN                                FailSafeCapListParse = 0;

  DEBUG((EFI_D_INFO, "Gen3: Gen3GetEndpointInfoLate(SKT=%d, PORT=%s(%d)).\n", Socket, IIO_PORT_LABEL(Port), Port ));

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];

  Pbus.Bits.pbn = BusBase;
  Secbus.Bits.sbn = BusBase + TEMP_BUSBASE_OFFSET;
  Subbus.Bits.subordinate_bus_number = BusBase + TEMP_BUSBASE_OFFSET;
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(PBUS_IIO_PCIE_REG)), Pbus.Bits.pbn);
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SECBUS_IIO_PCIE_REG)), Secbus.Bits.sbn);
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SUBBUS_IIO_PCIE_REG)), Subbus.Bits.subordinate_bus_number);

  if(PciExpressRead16(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, 0)) != 0xFFFF) {
    LnkCap.Data = 0;
    // Compare endpoint max and negotiated link widths.
    CapId = 0;
    RegLocation =  ONLY_REGISTER_OFFSET(CAPPTR_IIO_PCIEDMI_REG);
    RegLocation =  PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
    while (RegLocation != 0) {
      if( (RegLocation & 1) == 0 )    //thus making sure that WORD read is not on odd byte boundary
        CapId =  PciExpressRead16(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
      else
        CapId =  (UINT16) ((PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+1)) << 8) | \
            PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation)) );
      if ((CapId & 0xFF) == GEN3_CAPID_PCIE) {
        break;
      } else {
        RegLocation = (CapId & 0xFF00) >> 8;
      }
      //track how many capid nodes it is parsing if >255 than pci space corrupted
      FailSafeCapListParse++;
      if(RegLocation == 0xFF || FailSafeCapListParse >= 0xFF){
        DEBUG((EFI_D_ERROR, "  Invalid capability list pointer!!!\n"));
        break;
      }
    }
  }
  else  {
    CapId = 0;
    RegLocation = 0;
  }

  if ((CapId & 0xFF) != GEN3_CAPID_PCIE) {
    DEBUG((EFI_D_ERROR, "Gen3: Can't get CAPID.\n"));
    status = GEN3_FAILURE;
//    goto errexit;
  }
  else  {
    RegLocation2 = RegLocation + ONLY_REGISTER_OFFSET(LNKCAP2_IIO_PCIE_REG) - ONLY_REGISTER_OFFSET(PXPCAPID_IIO_PCIE_REG);
    if( (RegLocation2 & 3) == 0)   //making sure it is on DWORD boundary
      LnkCap2.Data =  PciExpressRead32(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2));
    else{
      LnkCap2.Data =  PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2));
      LnkCap2.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+1)) << 8;
      LnkCap2.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+2)) << 16;
      LnkCap2.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation2+3)) << 24;
    }


    RegLocation = RegLocation + ONLY_REGISTER_OFFSET(LNKCAP_IIO_PCIE_REG) - ONLY_REGISTER_OFFSET(PXPCAPID_IIO_PCIE_REG);
    if( (RegLocation & 3) == 0)   //making sure it is on DWORD boundary
      LnkCap.Data =  PciExpressRead32(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
    else{
      LnkCap.Data =  PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation));
      LnkCap.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+1)) << 8;
      LnkCap.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+2)) << 16;
      LnkCap.Data |= PciExpressRead8(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, RegLocation+3)) << 24;
    }

    DEBUG((EFI_D_INFO, "Gen3: "));
    if(Vid != NULL) {
      *Vid = PciExpressRead16(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, ONLY_REGISTER_OFFSET(VID_IIO_PCIE_REG)));
      DEBUG((EFI_D_INFO, " VID=%04X", *Vid));
    }
    if(Did != NULL) {
      *Did = PciExpressRead16(PCI_PCIE_ADDR(Secbus.Bits.sbn, SECBUS_TEMP_DEV_NUM, SECBUS_TEMP_FUNC_NUM, ONLY_REGISTER_OFFSET(DID_IIO_PCIE_REG)));
      DEBUG((EFI_D_INFO, " DID=%04X", *Did));
    }
    if(MaxSpeed != NULL)  {
      // Check if the  max lnkspd is coherent with the vector. 
      if (( (1 << (LnkCap.Bits.maxlnkspd-1)) & (LnkCap2.Bits.lnkspdvec)) != 0)
      {
        *MaxSpeed = LnkCap.Bits.maxlnkspd;
      } else {
        *MaxSpeed = 1;
      }      
      DEBUG((EFI_D_INFO, " MaxLinkSpeed=%d", *MaxSpeed));
    }
    if(MaxWidth != NULL)  {
      *MaxWidth = LnkCap.Bits.maximum_link_width;
      DEBUG((EFI_D_INFO, " MaxLinkWidth=%d\n", *MaxWidth));
    }
    status = GEN3_SUCCESS;
  }

  //clean up before exit
  Pbus.Bits.pbn = 0;
  Secbus.Bits.sbn = 0;
  Subbus.Bits.subordinate_bus_number = 0;
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(PBUS_IIO_PCIE_REG)), Pbus.Bits.pbn);
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SECBUS_IIO_PCIE_REG)), Secbus.Bits.sbn);
  S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, ONLY_REGISTER_OFFSET(SUBBUS_IIO_PCIE_REG)), Subbus.Bits.subordinate_bus_number);

  return  status;

//  return GEN3_SUCCESS;

//errexit:
//  return GEN3_FAILURE;
}

VOID
FixLinkDegrades (
  IIO_GLOBALS                       *IioGlobalData,
  UINT8                            Socket,
  UINT8                            Port
  )
{
  UINT16 Bifurcation;
  UINT32 RegLocation;
  UINT8  BusBase;
  UINT32 MaxWidth, MaxCardWidth=0;
  UINT16 CurrentLinkWidth;
  UINTN  RetryCount;
  LNKSTS_IIO_PCIE_STRUCT Lnksts;
  BCTRL_IIO_PCIE_STRUCT Bctrl;
  SPARE_IIO_DFX_STRUCT Spare;
  GEN3_STATUS status;
  UINT16  vid=0, did=0;
  UINT32  MaxCardSpeed = 0;
  UINTN   ClusterPortIndex = 0;
  UINT8   IioDfxDev;
  UINT8   IioDfxFunc;
  UINT8   IioSuperClusterPort;

  IioSuperClusterPort = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].SuperClusterPort;
  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Socket];
  Bifurcation = PciExpressRead16(PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[IioSuperClusterPort].Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIEDMI_REG))) & 0x7;
  ASSERT(Bifurcation <= 4);

  ClusterPortIndex = Port - IioSuperClusterPort;    //fix for s4031021
  if(ClusterPortIndex > 4)
    return;

  RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG);
  Lnksts.Data = PciExpressRead16(PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));

  if (!Lnksts.Bits.data_link_layer_link_active) {
    DEBUG((EFI_D_ERROR,"IIO=%d, Port=%s(%d) - Link down!.\n", Socket, IIO_PORT_LABEL(Port), Port));
    goto errexit;
  }
  CurrentLinkWidth = Lnksts.Bits.negotiated_link_width;

  MaxWidth = (UINT32)BifurcationMap[Bifurcation][ClusterPortIndex];
  if( !MaxWidth ) //width 0 means lanes assigned to other port
    return;
  DEBUG((EFI_D_INFO,"IIO=%d, Port=%s(%d), Max width assigned=%d, current link width=%d.\n", Socket, IIO_PORT_LABEL(Port), Port, MaxWidth, CurrentLinkWidth ));
  status = Gen3GetEndpointInfoLate(IioGlobalData, Socket, Port, &vid, &did, &MaxCardSpeed, &MaxCardWidth);
  if(status == GEN3_FAILURE)  {
    DEBUG((EFI_D_ERROR,"No PCIe Card found on IIO=%d, Port=%s(%d).\n", Socket, IIO_PORT_LABEL(Port), Port));
    return;
  }

  if (MaxCardWidth < MaxWidth) {
    MaxWidth = MaxCardWidth;
  }

  RetryCount = 0;
  while ((MaxWidth != CurrentLinkWidth) && (RetryCount < MAX_LINK_DEGRADE_RETRIES)) {
    DEBUG((EFI_D_ERROR,"Link width recovery #%d on IIO=%d, Port=%s(%d).\n", RetryCount, Socket, IIO_PORT_LABEL(Port), Port ));
    RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
    Bctrl.Data = PciExpressRead16(PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
    Bctrl.Bits.sbr = 1;
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);

    IioStall (IioGlobalData, GEN3_STALL_SBR);

    RegLocation = ONLY_REGISTER_OFFSET(BCTRL_IIO_PCIE_REG);
    Bctrl.Data = PciExpressRead16(PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
    Bctrl.Bits.sbr = 0;
    S3_PciExpressWrite16(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation), Bctrl.Data);

    IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);

    // apply additional retrain from the root port side
    Gen3WaitTillLinkRetrainsLate(IioGlobalData, BusBase,
        IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device,
        IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function);

    RegLocation = ONLY_REGISTER_OFFSET(LNKSTS_IIO_PCIE_REG);
    Lnksts.Data = PciExpressRead16(PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].Function, RegLocation));
    CurrentLinkWidth = Lnksts.Bits.negotiated_link_width;

    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[Port].DfxFunction;
    RegLocation = ONLY_REGISTER_OFFSET(SPARE_IIO_DFX_REG);
    Spare.Data = PciExpressRead32(PCI_PCIE_ADDR(BusBase, IioDfxDev, IioDfxFunc, RegLocation));
    Spare.Bits.txn_port_spare0 |= (1 << RetryCount);
    if ((MaxWidth != CurrentLinkWidth) && (RetryCount >= (MAX_LINK_DEGRADE_RETRIES - 1))) {
      Spare.Bits.txn_port_spare0 |= (1 << (RetryCount + 1));
    }
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( BusBase, IioDfxDev, IioDfxFunc, RegLocation), Spare.Data);

    RetryCount++;
  }

  return;

errexit:
  return;
}

BOOLEAN
IsPchPciePortsPopulated(
    UINT8                                               BusBase
)
/*++

Routine Description:
  Dtermines if the PCH PCIe slots have devices connected or not.

Arguments:
  BusBase     - base bus # of the PCH

Returns:
  TRUE  - if one or more PCIe slots are populated
  FALSE - if all the PCIe slots for that IIO are open / empty

--*/
{
  UINT8   PchPxp;
  UINT16  wdata;

  for(PchPxp = 0; PchPxp <=7; PchPxp++){
    wdata = PciExpressRead16(PCI_PCIE_ADDR(BusBase, PCH_PCIE_ROOT_PORT_DEVICE,
                                  PchPxp, 0));
    if(wdata != 0xFFFF){
      wdata = PciExpressRead16(PCI_PCIE_ADDR(BusBase, PCH_PCIE_ROOT_PORT_DEVICE,
          PchPxp, PCH_PCIE_ROOT_PORT_LSTS_REG));
      if( (wdata & B_PCIE_PORT_PXPLNKSTS_DLLA) )    //data link layer active
        break;                                      //exit on 1st PCIe port found active
    }
    else
      continue;
  }
  if(PchPxp >= 8)   //no endpoint devices found behind PCH root ports
    return FALSE;
  else
    return TRUE;
}

BOOLEAN
IsIioPciePortsPopulated(
    IIO_GLOBALS                                             *IioGlobalData,
    UINT8                                               Iio
)
/*++

Routine Description:
  Dtermines if the PCIe slots have devices connected or not.

Arguments:
  IioGlobalData  - Pointer to UDS
  Iio     - index to CPU/IIO

Returns:
  TRUE  - if one or more PCIe slots are populated
  FALSE - if all the PCIe slots for that IIO are open / empty

--*/
{
  UINTN                   j;
  BOOLEAN                 IioPortsActive = FALSE;
  PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress;

  PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if( IioVData_ConfigurePciePortLate(IioGlobalData, Iio, (UINT8)j) == FALSE)  //check whether the PCIe port can be configured
      continue;
    if(!Iio && !j)  continue;   //DMI port

    PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    if(IsPciePortDLActive(IioGlobalData, PciAddress) == TRUE)    //data link layer active
      IioPortsActive = TRUE;
  }

  if(IioPortsActive == FALSE && j == NUMBER_PORTS_PER_SOCKET){
    DEBUG ((EFI_D_WARN, "  No PCIe devices connected to IIO%d!\n",Iio));

    if(!Iio){    //legacy socket
      IioPortsActive = IsPchPciePortsPopulated(PciAddress.Bus);

      if(IioPortsActive == TRUE)
        DEBUG ((EFI_D_WARN, "  But PCH has devices connected..\n"));
    }
  }
  return IioPortsActive;
}

VOID
IioLinkNotActiveWorkarounds(
    IIO_GLOBALS                                             *IioGlobalData,
    UINT8                                               Iio
)
{
  UINT32  data;
  UINT8   Bus = 0;

  //
  // ivt_eco: be4622299:  CLONE from ivytown: PCIe Squelch Break stuck High on
  //                    sockets w/ no PCIe allocated
  //  [cloned to s4030710] - for IVT B0 & beyond
  //
  // As per HSX 4166237, keep only for HSX A0
  if((IioGlobalData->IioVData.CpuType == CPU_IVT && IioGlobalData->IioVData.CpuStepping >= B0_REV) ||
     (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)
      ){
    if(IsIioPciePortsPopulated(IioGlobalData, Iio) == FALSE){
      Bus = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

      switch(IioGlobalData->SetupData.WA_NoPciePortActive[Iio]){
        case 2:   //option# 2: Reset the SQ FLOP by CSR option
          data = PciExpressRead32(PCI_PCIE_ADDR(Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC,
                  IIO_DFX_L1_CLK_GATING_REG_IIO_DFX_GLOBAL_REG));
          data |= 1;    //set blkl1_exit_en = 1
          S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(Bus, PCIE_PORT_GLOBAL_DFX_DEV, PCIE_PORT_GLOBAL_DFX_FUNC,
              IIO_DFX_L1_CLK_GATING_REG_IIO_DFX_GLOBAL_REG), data);
          break;

        case 1:   //option# 1: PCU Squelch exit ignore option
        default:
          data = IioGlobalData->IioOutData.CpuCsrAccess->ReadCpuCsr(Iio, 0, SAPMCTL_PCU_FUN1_REG);
          data &= ~BIT25;    //clear qpi2_l0s_pll_sen_enable
          IioGlobalData->IioOutData.CpuCsrAccess->WriteCpuCsr(Iio, 0, SAPMCTL_PCU_FUN1_REG, data);
          break;
      }
    }
  }
}

VOID
IioLinkRecoveryWorkarounds(
    IIO_GLOBALS                                         *IioGlobalData,
    UINT8                                               Iio
)
{
  UINTN                   j;

  if((IioGlobalData->IioVData.CpuType == CPU_IVT || IioGlobalData->IioVData.CpuType == CPU_HSX) &&
     (IioGlobalData->IioVData.CpuStepping == A0_REV))  {
    for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      if ((IioGlobalData->IioOutData.PciePortPresent[(Iio*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) ||
          (IioGlobalData->IioOutData.PciePortConfig[(Iio*NUMBER_PORTS_PER_SOCKET)+j] == 0 ) )  continue;

      // skip the DMI port link width check here, (taken care in DmiInit routine)
      if(Iio == 0 && j == 0)  continue;   //fix for s4030517

      //
      // ivt_eco: s4537569: DUT does not look for 2TS consecutive TS
      //                    order set before deciding on the polarity inversion.
      //  [cloned to s4030163]
      //  (rml -3615828: PCIE Matrix: Barton Hills NIC failed Gen1 SBR test on Taxco with link degrade)
      FixLinkDegrades(IioGlobalData, Iio, (UINT8)j);
    }
  }
}

VOID 
ClearXPUncErrStsBit6Late(
    IIO_GLOBALS              *IioGlobalData,
    UINT8                Iio
)
{
    UINT8   BusBase;
    UINT8   Device;
    UINT8   Function;
    UINT8   j;
    UINT32  Data32;

    BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    //P0 DMI or PCIE0
    Data32 = 0;
    //this bit is R/W1CS = write 1 to clear 
    Data32 |= RECEIVED_PCIE_COMPELTION_WITH_UR_STATUS;
    for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, R_XPUNCERRSTS), Data32);
    }
    return;
}
VOID
IioVtDPreEnWorkAround  (
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
{
  UINT8                               j;
  UINT32                              RegLocation;
  UINT8                               IioDfxDev;
  UINT8                               IioDfxFunc;
  VTDPRIVC0_IIO_DFX_VTD_STRUCT        VTdPrivC0;
  XPDFXSPAREREG_IIO_DFX_HSX_BDX_STRUCT    XpDfxSpareReg;
  VTDDEBUGREG0_IIO_DFX_VTD_HSX_BDX_STRUCT VtdDebugReg0;
  if ((IioGlobalData->IioVData.CpuType == CPU_IVT) && (IioGlobalData->IioVData.CpuStepping == A0_REV)) {
    //
    //VT-d related ECO for A0 stepping
    //
    //
    // ivt_eco: s4620614: With the VT-d denial of service fix enabled
    //                    the PCIE TXN Layer does not return posted data credits
    //  [cloned to s4031086] - remove the WA for B0 defined in s4030322/s4030421
    //s4030322: Possible VTd denial of service issues
    RegLocation = ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG);
    VTdPrivC0.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation));
    VTdPrivC0.Bits.ud |= BIT3;    //set bit 30 to 1
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation), VTdPrivC0.Data);

    // ivt_eco: s4571071: VT-d Context Cache Invalidation with Function Mask set incorrectly checks Parity
    // ivt_eco: s4571072: IOTLB invalidation affected by parity bits when AM (addr mask) is between 10 to 18
    RegLocation = ONLY_REGISTER_OFFSET(VTDDEBUGREG0_IIO_DFX_VTD_REG);
    VtdDebugReg0.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation));
    VtdDebugReg0.Bits.vtinvd |= BIT0;         // ivt_eco: 4571071
    VtdDebugReg0.Bits.vtinvd |= BIT1;         // ivt_eco: 4571072
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation), VtdDebugReg0.Data);
  }

  //
  // All HSX Workarounds related to VTd are below
  // ____________________________________________
  //
  if ((IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX)){
    //
    // HSD 4166366: [HSX_A0_PO] CLONE from HSX: PUSH from ivytown: With the VT-d denial of 
    //              service fix enabled the PCIE TXN Layer does not return posted data credits
    //
    RegLocation = ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG);
    VTdPrivC0.Data =  PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation)) ;
    if (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)
      VTdPrivC0.Bits.ud |= BIT3;              // Chicken bit controls error logging on misc error bit
    else
      VTdPrivC0.Bits.ud &= ~(BIT3 | BIT4);
      
    //  
    //4167999: VT-d ATS faults (ITE+ICE) when protected memory regions enabled
    //4986774: HSX Clone: VT-d ATS faults (ITE+ICE) when protected memory regions enabled
    //         Timeout increased from  256us (0x01) <set in 4167999> to 1s (0x02)
    //
    VTdPrivC0.Bits.tlbcmpto = 0x02;
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation), VTdPrivC0.Data);


    //
    // Port based HSX workarounds related to VTd below
    //
    for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
      if(j==0 || j==3 || j==7) {

        IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
        IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;
        //
        // HSD 4166366 (continued): [HSX_A0_PO] CLONE from HSX: PUSH from ivytown: With the VT-d denial of 
        //              service fix enabled the PCIE TXN Layer does not return posted data credits
        //
        RegLocation = ONLY_REGISTER_OFFSET(XPDFXSPAREREG_IIO_DFX_REG);
        XpDfxSpareReg.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxFunc, RegLocation));
        if (IioGlobalData->IioVData.CpuType == CPU_HSX && IioGlobalData->IioVData.CpuStepping < B0_REV)
          XpDfxSpareReg.Bits.disable_msi_gt_1dw = 0;
        else
          XpDfxSpareReg.Bits.disable_msi_gt_1dw = 1;
        S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IioDfxDev, IioDfxFunc, RegLocation), XpDfxSpareReg.Data);
      }
    } // j
  }
}

VOID
IioVtdEnWorkAround  (
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
{
  UINT32  RegLocation;
  VTGENCTRL2_IIO_VTD_HSX_BDX_STRUCT   VTdGenCtrl2;
  VTGENCTRL3_IIO_VTD_STRUCT           VTdGenCtrl3;
  VTDPRIVC0_IIO_DFX_VTD_STRUCT        VTdPrivC0;
  UINT8   Index,NumSockets; 
  
  //// Figure out if we are in UP/1S configuration:Get the number of sockets present. 
  for (NumSockets=0, Index=0; Index <MaxIIO; Index ++) {
    if (IioGlobalData->IioVData.SocketPresent[Index])
      NumSockets ++;
  }
  
	
  if (((IioGlobalData->IioVData.CpuType == CPU_IVT) && (IioGlobalData->IioVData.CpuStepping == A0_REV)) ||
      /// s4985923 ECO needed for all steppings of CPU_HSX
      (IioGlobalData->IioVData.CpuType == CPU_HSX) || (IioGlobalData->IioVData.CpuType == CPU_BDX) )  {  
    //VT-d related ECO for A0 stepping
    // ivt_eco: s4572807: Hang in VTd due to busy bit not getting set in retry response
    //  [cloned to 4030375]
    RegLocation = ONLY_REGISTER_OFFSET(VTGENCTRL2_IIO_VTD_REG);
    VTdGenCtrl2.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation)); 
    // 
    // HSD 4986149 HSX Clone: PCIE: Rocket CATERR with enable_extended_vtstf=1
    //
    // bdx_eco: s280642 :  CLONE from broadwell_server: Reserved TLB entry stays allocated after lock flow completion causing a future lock to hang
    // [cloned to s4986880 ] - Permanent for BDX
    //
    if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping > B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      VTdGenCtrl2.Bits.en_lock_rsvd_ent = 0; // Disables standard method of reserving entries in the TLB
    }

    if (((IioGlobalData->IioVData.CpuType == CPU_HSX)  && ((IioGlobalData->IioVData.CpuStepping <=B0_REV_HSX) && (NumSockets !=1)))) 
    {
      //
      // HSD 4166665: HSX Clone: ESCALATE from hexaii_hsx_mock:VT-d LRUctrl set incorrectly (flexconVTd cbVTdJKT3876632wa check)
      //
      VTdGenCtrl2.Bits.lructrl = 1; 
    } else {
      /// IVT A0, HSX B0 on UP, and HSX C0, and BDX
      VTdGenCtrl2.Bits.lructrl = 0;
    }
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), VTdGenCtrl2.Data);

    //to force periodic switch retry
    RegLocation = ONLY_REGISTER_OFFSET(VTDPRIVC0_IIO_DFX_VTD_REG);
    VTdPrivC0.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation));

    if (IioGlobalData->IioVData.CpuType == CPU_IVT || 
    //
    // 4986149/4985923  Set this bit for UP on all steppings and for all configurations on > HSX_C0 & BDX steppings   
    //
    ((IioGlobalData->IioVData.CpuType == CPU_HSX) && ((IioGlobalData->IioVData.CpuStepping>B0_REV) || (NumSockets ==1))) || (IioGlobalData->IioVData.CpuType == CPU_BDX) ) {
      VTdPrivC0.Bits.fpsr = 1;
    }
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IIO_DFX_VTD_DEV, IIO_DFX_VTD_FUNC, RegLocation), VTdPrivC0.Data);

  }
    
  // 
  // HSD 4986149 HSX Clone: PCIE: Rocket CATERR with enable_extended_vtstf=1 
  //
  // bdx_eco: s280642 :  CLONE from broadwell_server: Reserved TLB entry stays allocated after lock flow completion causing a future lock to hang
  // [cloned to s4986880 ] - Permanent for BDX
  //
  if ( ((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping > B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
    RegLocation = ONLY_REGISTER_OFFSET(VTGENCTRL3_IIO_VTD_REG);
    VTdGenCtrl3.Data = PciExpressRead32(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation));
    VTdGenCtrl3.Bits.en_agt_rsvd_p_f_ent_tlb0 = 0x3f; //These two reserve posted entries in TLB for each port. 
    VTdGenCtrl3.Bits.en_agt_rsvd_p_f_ent_tlb1 = 0x1ff; 
    S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, RegLocation), VTdGenCtrl3.Data);
  }

}

VOID
IioMiscCtrlWorkAroundPh1(
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
/*++

Routine Description:
  This routine is primarily to address the settings related to the RWO bit
  features of the IIOMISCCTRL global register for the socket.

Arguments:
  PciAddress        - The IIO controller PCI address
  IioGlobalData            - Pointer to IIO Internal Data Store

Returns:

++*/
{
  UINT8     j, Iio;
  UINT32    Data32;
  BOOLEAN   IioUniphyDisable = TRUE;
  UINTN     Recheck = 0;

  //
  // This routine is primarily to address the settings related to the RWO bit
  // features of the IIOMISCCTRL global register for the socket.
  //
  for ( Iio = 0; Iio < MaxIIO; Iio++ ) {
    if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[Iio]) && IioGlobalData->IioVData.SocketPresent[Iio]) {
      break;
    }
  }
  if(Iio == MaxIIO)
    return;
  //
  // ivt_eco: s4676871: PCIe Squelch Break stuck High on sockets w/ no PCIe allocated
  //  [cloned to s4030607]
  // only for IVT A0/1, and not required for legacy socket
  if((IioGlobalData->IioVData.CpuType == CPU_IVT && IioGlobalData->IioVData.CpuStepping == A0_REV) &&
      Iio != 0) {
    //
    // determine whether any card connected to any of the PCIe slots for this IIO
    //
    do{
      for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
        if( IioVData_ConfigurePciePortLate(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
          continue;
        PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
        PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
        if ((PciExpressRead16(PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPLNKSTS)) & B_PCIE_PORT_PXPLNKSTS_DLLA) == TRUE) //data link layer active
          IioUniphyDisable = FALSE;                   //no need to disable the Uniphy
      }
      Recheck++;
      if(IioUniphyDisable == TRUE && j == NUMBER_PORTS_PER_SOCKET && Recheck < 10){
        DEBUG ((EFI_D_WARN, "  No PCIe devices connected to IIO%d!....retry...\n",Iio));
        IioStall (IioGlobalData, UNIPHY_DIS_DELAY); //100ms delay
      }
    }while(Recheck < 10 && IioUniphyDisable == TRUE);
    // disable the uniphy for that IIO where there are no endpoints connected to the pcie slots
    if(IioUniphyDisable == TRUE && j == NUMBER_PORTS_PER_SOCKET){
      DEBUG ((EFI_D_WARN, "  No PCIe devices connected to IIO%d!\n",Iio));
      //
      // since UNIPHY of IIO is getting disabled, call the IIO as not valid
      //
      Data32 = PciExpressRead32(PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));
      Data32 |= B_IIO_DIS_UNUSE_PORT;
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL), Data32);

      Data32 = PciExpressRead32(PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));
      DEBUG ((EFI_D_INFO, "FORCING THE UNIPHY DISABLE FOR SOCKET %d (0x%x)\n",Iio, Data32));
    }
  }
}

VOID
IioMiscCtrlWorkAroundPh2(
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
/*++

Routine Description:
  This routine is to address the post RWO settings of the IIOMISCCTRL global register

Arguments:
  PciAddress        - The IIO controller PCI address
  IioGlobalData            - Pointer to IIO Internal Data Store

Returns:

++*/
{
  UINT8                   j, Iio;
  UINT32                  Data32, RegLocation;
  IOBAS_IIO_PCIE_STRUCT   IoBas;

  for ( Iio = 0; Iio < MaxIIO; Iio++ ) {
    if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[Iio]) && IioGlobalData->IioVData.SocketPresent[Iio]) {
      break;
    }
  }
  if(Iio == MaxIIO)
    return;

  Data32 = PciExpressRead32(PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));

  // jkt_eco # 3615392: Relaxed Order disable bits in iiomiscctrl need to be set to 1
  //
  // ivt_eco: s4538144: PCIE Ordering: Strongly ordered posted issued prior to
  //                    the completion of previous relaxed ordered posted in some cases
  //  [cloned to s4029926]
  // 4985884: Enabled RO=1 for Improved Remote P2P Performance
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= B0_REV_HSX)) || (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      Data32 &= ~(BIT14 | BIT18 | BIT22);  //pipelines_ns_writes_on_csi, disable_inbound_ro_for_vc0_vcp_writes , disable_ro_on_writes_from_cb_dma
      Data32 |= (BIT17 | BIT16);           // dmi_vc1_write_ordering
  } else {
     Data32 |= BIT18;  //disable_inbound_ro_for_vc0_vcp_writes
     Data32 |= BIT22;  //disable_ro_on_writes_from_cb_dma
  }
  //
  //s3615786 CLONE from jaketown: ESCALATE from hexa_jaketown_c0:CATERR with a stuck Lock when azalia is using VCp channel
  //
  Data32 &= ~BIT9;  //disable azalia_on_vcp
  //
  // 4986166: ESCALATE from hexaii_hsx_mock:Updated C0 register settings for isoc
  //
  if (((IioGlobalData->IioVData.CpuType == CPU_HSX) && (IioGlobalData->IioVData.CpuStepping >= C0_REV_HSX)) ||
      (IioGlobalData->IioVData.CpuType == CPU_BDX)) {
      if((IioGlobalData->SetupData.DmiVc1 == 0) && OemIsDmiVcpEnable(IioGlobalData)) {
        Data32 |= BIT9; // Enable azaliza_on_vcp 
      }
  }

  S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL), Data32);

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if( IioVData_ConfigurePciePortLate(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
      continue;
    //
    //Invalid 'I/O addressing Capability' implemented in II-O chipset (3614969, 3876211, 3247349)
    //
    if (!(Iio == 0 && j==0)) {
      //setting EN1K bit
      Data32 = PciExpressRead32(PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL));
      Data32 |= B_IIO_MISC_EN_1K;
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL), Data32);
      RegLocation = ONLY_REGISTER_OFFSET(IOBAS_IIO_PCIE_REG);
      IoBas.Data = PciExpressRead8(PCI_PCIE_ADDR(PciAddress.Bus, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, \
                                                 IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, RegLocation));
      IoBas.Bits.more_i_o_base_address = 0;
      S3_PciExpressWrite8(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device, \
                                     IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function, RegLocation), IoBas.Data);
      //clear EN1K bit
      Data32 &= ~B_IIO_MISC_EN_1K;
      S3_PciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR( PciAddress.Bus, CORE05_DEV_NUM, CORE05_FUNC_NUM, R_IIO_IIOMISCCTRL), Data32);
    }
  }

}

VOID
IioPostInitWorkaround(
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
/*++

Routine Description:
  This routine is to any IIO workarounds which would be done post IIO init

Arguments:
  PciAddress        - The IIO controller PCI address
  IioGlobalData            - Pointer to IIO Internal Data Store

Returns:

++*/
{
  UINT8   Iio;

  for ( Iio = 0; Iio < MaxIIO; Iio++ ) {
    if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[Iio]) && IioGlobalData->IioVData.SocketPresent[Iio]) {
      break;
    }
  }
  if(Iio == MaxIIO)
    return;

  //
  // apply any post IIO init link recovery workaround
  //
  IioLinkRecoveryWorkarounds(IioGlobalData, Iio);    //WA for link recovery
  IioLinkNotActiveWorkarounds(IioGlobalData, Iio);   //WA if links not active
}

VOID
IioBeforeBootWorkaround(
    PCI_ROOT_BRIDGE_PCI_ADDRESS         PciAddress,
    IIO_GLOBALS                                             *IioGlobalData
)
/*++

Routine Description:
  This routine is to any IIO workarounds which would be done post IIO init

Arguments:
  PciAddress        - The IIO controller PCI address
  IioGlobalData            - Pointer to IIO Internal Data Store

Returns:

++*/
{
  UINT8   Iio;

  for ( Iio = 0; Iio < MaxIIO; Iio++ ) {
    if((PciAddress.Bus == IioGlobalData->IioVData.SocketBaseBusNumber[Iio]) && IioGlobalData->IioVData.SocketPresent[Iio]) {
      break;
    }
  }
  if(Iio == MaxIIO)
    return;

  //
  // apply any IIO workarounds before the OS boot
  //
  IioLinkNotActiveWorkarounds(IioGlobalData, Iio);   //WA if links not active
}

VOID
IioWorkAround (
    PCI_ROOT_BRIDGE_PCI_ADDRESS                         PciAddress,
    IIO_INIT_PHASE                                      Phase,
    IIO_GLOBALS                                         *IioGlobalData,
    IIO_INIT_ENUMERATION                                InitPhase,
    VOID                                                *Buffer
  )
/*++

Routine Description:

  do IIO specific work arounds for future fix items as well as known Errata

Arguments:

  PciAddress        - The IIO controller PCI address
  Phase             - Defines the phase of PCI initialization that the PCI driver is executing.
                      Either: 
                        EfiPciEnumerationDeviceScanning
                        EfiPciEnumerationBusNumberAssigned
                        EfiPciEnumerationResourceAssigned
  IioGlobalData     - Pointer to IIO Internal Data Store
  InitPhase         - Additional selective code execution decision marker
  Buffer            - Scratch buffer for data passing if necessary based on InitPhase


Returns:

  EFI_SUCCESS  -  Early platform initialization completed successfully.
  All other error conditions encountered result in an ASSERT.

--*/
{
  switch(Phase)  {
  case IIOInitPhase1:
    if(InitPhase == IioVtDPreEn)
      IioVtDPreEnWorkAround(PciAddress, IioGlobalData);
    if(InitPhase == IioEnumEnd)
      IioPostInitWorkaround(PciAddress, IioGlobalData);
    break;
  case IIOInitPhase2:
    if(InitPhase == IioPortEnumProgramMISCCTRL){
      IioMiscCtrlWorkAroundPh1(PciAddress, IioGlobalData);
      IioMiscCtrlWorkAroundPh2(PciAddress, IioGlobalData);
    }
    break;
  case IIOInitPhase3:
    if(InitPhase == IioVtDEn)
      IioVtdEnWorkAround(PciAddress, IioGlobalData);
    if(InitPhase == IioPostInitEnd)
      IioBeforeBootWorkaround(PciAddress, IioGlobalData);
    break;
  }
}  // End of IioWorkAround
