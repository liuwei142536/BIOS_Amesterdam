/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2014   Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  IioEarlyInitialize.c

Abstract:

  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

--*/
#include "IioEarlyInitialize.h"

//
// Global Data Items
//
#pragma pack (push, 1)

typedef struct {
    UINT8   PortValue[2];
} PCIE_IOU2PORT_STATUS;

PCIE_IOU2PORT_STATUS PcieIou2Status[] = { // int  IOU2_array [2][2]
    {0x04, 0x04},                  //   00 = x4  | x4 | -- | --    (IOU1 Only)
    {0x08, 0x00}                   //   01 = x8  | -- | -- | --    (IOU1 Only)
};

typedef struct {
    UINT8   PortValue[4];
} PCIE_IOU0OR1PORT_STATUS;

PCIE_IOU0OR1PORT_STATUS PcieIou0Or1Status [] = { // int  IOUX_array [5][4]
    {0x04, 0x04, 0x04, 0x04},     //   00 = x4  | x4 | x4 | x4    (IOU 2 & 3)
    {0x08, 0x00, 0x04, 0x04},     //   01 = x8  | -- | x4 | x4    (IOU 2 & 3)
    {0x04, 0x04, 0x08, 0x00},     //   02 = x4  | x4 | x8 | --    (IOU 2 & 3)
    {0x08, 0x00, 0x08, 0x00},     //   03 = x8  | -- | x8 | --    (IOU 2 & 3)
    {0x10, 0x00, 0x00, 0x00}      //   04 = x16 | -- | -- | --    (IOU 2 & 3)
};

#pragma pack (pop)


VOID
IioOtherVariableInit (
  IIO_GLOBALS           *IioGlobalData,
  UINT8                 Iio
  )
{
  UINT8                 IioUplinkPortIndex;

  UINT8                 Bus;
  UINT8                 Dev;
  UINT8                 Func;

  //
  // Init any platform port specific settings here
  // for ex, Uplink port
  //
  OemIioUplinkPortDetails(Iio, &IioUplinkPortIndex, &Bus, &Dev, &Func);

  if( IioUplinkPortIndex < NUMBER_PORTS_PER_SOCKET ) {
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  Found uplink port @ skt %d port %d\n", Iio, IioUplinkPortIndex);

    IioGlobalData->PreLinkData.UplinkInfo[Iio].Valid = 1;
    IioGlobalData->PreLinkData.UplinkInfo[Iio].IioUplinkPortIndex = IioUplinkPortIndex;
    IioGlobalData->PreLinkData.UplinkInfo[Iio].UplinkPortInfo.Device = Dev;
    IioGlobalData->PreLinkData.UplinkInfo[Iio].UplinkPortInfo.Function = Func;
  } else
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "  Socket %d does not support uplink port!\n", Iio);

  //
  // Init the Gen3 Prelink adaptation configuration variables based on its
  // primary port; since user sees the Setup page only for the primary
  // port as per its IOU bifurcation, than other non-primary port has to be aligned since 
  // pre-link settings are done to every Gen3 individual port prior to its link training
  //
  AlignGen3PreLinkConfigurationSettings(IioGlobalData, Iio);

  //
  // Update our PCIEXpress configuration in our status information
  // loop through 10 PCIE ports that correspond to the three IOUs
  //
  IioUpdatePcieConfigurationData(IioGlobalData, Iio);

}

VOID
IioUpdatePcieConfigurationData(
  IIO_GLOBALS *IioGlobalData,
  UINT8       Iio
)
{
  UINT8   i,j;
  UINT8   *Array=NULL;

  IioGlobalData->IioOutData.CurrentPXPMap[(Iio*NUMBER_PORTS_PER_SOCKET)] = 0x04;  // DMI port is active or else we wouldnt be booting
  Array = &(IioGlobalData->IioOutData.CurrentPXPMap[0]);   // setup array to update status of PCIe ports

  for (i = 0; i < sizeof(PcieIou2Status)/sizeof(PCIE_IOU2PORT_STATUS); i++) {
    if (IioGlobalData->SetupData.ConfigIOU2[Iio] == i) {
      for (j = 0; j < sizeof(PCIE_IOU2PORT_STATUS) ; j++) {
        Array[(Iio*NUMBER_PORTS_PER_SOCKET)+j+1] = PcieIou2Status[i].PortValue[j];
      }
    }
  }

  for (i = 0; i < sizeof(PcieIou0Or1Status)/sizeof(PCIE_IOU0OR1PORT_STATUS); i++) {
    if (IioGlobalData->SetupData.ConfigIOU0[Iio] == i) {
      for (j = 0; j < sizeof(PCIE_IOU0OR1PORT_STATUS) ; j++) {
        Array[(Iio*NUMBER_PORTS_PER_SOCKET)+j+3] = PcieIou0Or1Status[i].PortValue[j];
      }
    }

    if (IioGlobalData->SetupData.ConfigIOU1[Iio] == i) {
      for (j = 0; j < sizeof(PCIE_IOU0OR1PORT_STATUS) ; j++) {
         Array[(Iio*NUMBER_PORTS_PER_SOCKET)+j+7] = PcieIou0Or1Status[i].PortValue[j];
      }
    }
  }
}

VOID
IioEarlyInit (
  IIO_GLOBALS  *IioGlobalData
  )
{
  UINT8       Iio;

  //
  // Other platform specific IIO_resource init
  //
  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (IioGlobalData->IioVData.SocketPresent[Iio] == 1)
      IioOtherVariableInit(IioGlobalData, Iio);
  }

  //
  // If LinkTraining was done early in PEI then skip this during DXE phase
  //
  if (IioGlobalData->IioVData.IioLinkTrainPhase == IN_DXE_PHASE) {
    IioInitDriverInstalled(IioGlobalData);
  }
}

#ifndef IA32
VOID
InitializeIioGlobalData (
  IIO_GLOBALS  *IioGlobalData
  )
/*++

Routine Description:

  Initialize IIO Global Data

Arguments:


Returns:

--*/
{
  UINT8       Iio;

  OemInitializeIioInputData(IioGlobalData);

  //
  // Other platform specific IIO_resource init
  //
  for (Iio = 0; Iio < MaxIIO; Iio++) {
    if (IioGlobalData->IioVData.SocketPresent[Iio] == 0) continue;

    OemIioPreLinkDataInitPerSocket(IioGlobalData, Iio);

    if (Iio == 0)
      OemVariableGetCurrent(IioGlobalData);

    OemVariableInitPerSocket(IioGlobalData, Iio);
  }

}

EFI_STATUS
IioEarlyInitializeEntry (
  IIO_GLOBALS                *IioGlobalData
  )
/*++

Routine Description:

  Install the IIO Initialization Protocol

Arguments:

  IIO Global Data
  
Returns:

  Status
    EFI_SUCCESS           - Installed successfully
    Others                - Failed to install this protocol

--*/
{
  UINT8                         Iio;

  if (IioGlobalData->IioVData.RasOperation) {
    //
    // Update SocketPresent for RAS Operation
    //
    for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ) {
      if (Iio == IioGlobalData->IioVData.SocketUnderOnline)
        IioGlobalData->IioVData.SocketPresent[Iio] = 1;
      else 
        IioGlobalData->IioVData.SocketPresent[Iio] = 0;
    }
  }
  IioEarlyInit(IioGlobalData);

  return EFI_SUCCESS;
}
#endif // IA32

VOID 
WaitTrainingFinished (
    IIO_GLOBALS                         *IioGlobalData,
    UINT8                           BusNumber,
    UINT8                           PciePortDeviceNumber,
    UINT8                           Port
  )
{
  BOOLEAN        PcieLinkTrained; 
  UINT32         Loop;
  UINT16         Var16;
   
  //
  // We need to wait 50ms according to SA BS to let training completed
  // We used not to care about this, but now we do the port init immediately
  // after Bifurcutation, hence have  to wait enough time and also poll on
  // LNKSTS.DLLA bit as per the IIO PCIe HAS
  //
  PcieLinkTrained = FALSE;
  Loop            = 0;    
  do {
     Var16 = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(BusNumber, PciePortDeviceNumber, Port, R_PCIE_PORT_PXPLNKSTS));
    if ( Var16 & B_PCIE_PORT_PXPLNKSTS_DLLA ) {   //wait on DLLA bit would never set for unpopulated slots
      //
      // if completed training
      //
      PcieLinkTrained = TRUE;
    } else {
      //
      // Wait for 10ms
      // TODO: Add stall S3 script here
      IioStall (IioGlobalData, GEN3_LINK_POLL_TIMEOUT);
      Loop++;
    }    
  } while (!PcieLinkTrained && Loop < 10);  // sending bifurcation command in parallel, here delay can be shorter to 100ms from 500ms

}

VOID
WaitTrainingFinishedEx (
    IIO_GLOBALS                         *IioGlobalData,
    UINT8                           BusNumber,
    UINT8                           PciePortDeviceNumber,
    UINT8                           iou_bif
  )
{
  if(PciePortDeviceNumber == 0){   //PCIe port 0 always x4
    WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_0_FUNC);
  }

  if(PciePortDeviceNumber == 1){   //IOU2
    switch(iou_bif){
    case 0:   //x4x4
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_1A_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_1B_FUNC);
      break;
    case 1:   //x8
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_1A_FUNC);
      break;
    }
  }

  if(PciePortDeviceNumber == 2 || PciePortDeviceNumber == 3){   //IOU0 or IOU1
    switch(iou_bif){
    case 0:   //x4x4x4x4
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2A_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2B_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2C_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2D_FUNC);
      break;
    case 1:   //x4x4x8
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2A_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2C_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2D_FUNC);
      break;
    case 2:   //x8x4x4
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2A_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2B_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2C_FUNC);
      break;
    case 3:   //x8x8
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2A_FUNC);
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2C_FUNC);
      break;
    case 4:   //x16
      WaitTrainingFinished(IioGlobalData, BusNumber, PciePortDeviceNumber, PCIE_PORT_2A_FUNC);
      break;
    }
  }
}

VOID
PcieLinkTrainingInit (
    IIO_GLOBALS                         *IioGlobalData,
    UINT8                               Iio
  )
{
  /*++
  The purpose of this routine is to do the necessary work required to enable the PCIE POrts.
  The implementaiton is slightly different from Seaburg so we will check the straps and make the necessary
  adjustments before the links begin training.  In addition this routine could and should be
  be used to do any othe initialization required before PCI Express training.
  --*/

  UINT8                     Iou0 = 0, Iou1 = 0, Iou2 = 0;
  UINT16                    BifCtrlData = 0;
  UINT8                     *Array=NULL;
  UINT32                    ScratchReg;
  UINT8                     BusNumber = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "PcieLinkTrainingInit at device scanning...\n");

  if (Iio >= MaxIIO) return;    //fix for s4031021
  //
  // Halt before bifurcation of the PCIe Margining breakpoint CSR is set
  // 4166306: PCIE: Need mechanism to tell BIOS to stop and wait prior link bifurcation
  //
  ScratchReg = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0], UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD6_UBOX_MISC_REG)));
  if ((ScratchReg >> 16) == PCIE_MARGINING_BREAKPOINT) {
    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0], UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD7_UBOX_MISC_REG)), (UINT32)(PCIE_MARGINING_BREAKPOINT << 16 ));
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Halting for PCIe Margining breakpoint!!\n");
    IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Clear UBOXSCRATCHPAD6[31:16] to make forward progress!!\n");
    while ((ScratchReg >> 16) == PCIE_MARGINING_BREAKPOINT) {
      ScratchReg = IioPciExpressRead32( IioGlobalData, PCI_PCIE_ADDR(IioGlobalData->IioVData.SocketUncoreBusNumber[0], UBOX_MISC_DEV_NUM, UBOX_MISC_FUNC_NUM, ONLY_REGISTER_OFFSET(BIOSSCRATCHPAD6_UBOX_MISC_REG)));
    }
  }


  //get IOU bifurcation info
  Array = &(IioGlobalData->SetupData.ConfigIOU2[0]);
  Iou2 = Array[Iio];
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "IIO=%d, IOU2=%d.\n", Iio, Iou2);
  Array = &(IioGlobalData->SetupData.ConfigIOU0[0]);
  Iou0 = Array[Iio];
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "IIO=%d, IOU0=%d.\n", Iio, Iou0);
  Array = &(IioGlobalData->SetupData.ConfigIOU1[0]);
  Iou1 = Array[Iio];
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "IIO=%d, IOU1=%d.\n", Iio, Iou1);
        

  LimitPcieLinkWidth(IioGlobalData, Iio);   

  //PCIE0 always x4
  if(BusNumber != 0) {
    BifCtrlData = 0;
    BifCtrlData &= (~B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG);
    BifCtrlData |= B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF;    // or in bifurcation control and set start bifurcation control bit
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].Device,IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL) ,BifCtrlData);
  }

  BifCtrlData = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].Device,IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL));
  BifCtrlData &= (~B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG);
  BifCtrlData |= (Iou2 | B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF);    // or in bifurcation control and set start bifurcation control bit
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].Device,IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL) ,BifCtrlData);

  if(IioGlobalData->IioVData.CpuFamily != CPU_FAMILY_BDX_DE) {
    //
    // bdx_eco: 282659 : CLONE from broadwell_server: BDX_DE: behaviour of IOU_BIF_CTRL_0_2_0.iou_start_bifurcation is not matching to the spec
    // [cloned to s4987895 ] - Permanent
    //
    BifCtrlData = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL));
    BifCtrlData &= (~B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG);
    BifCtrlData |= (Iou0 | B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF);    // or in bifurcation control and set start bifurcation control bit
    IioPciExpressWrite16 (IioGlobalData,PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].Device,IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL) ,BifCtrlData);
  } // CpuFamily != CPU_FAMILY_BDX_DE

  BifCtrlData = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].Device, IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL));
  BifCtrlData &= (~B_PCIE_PORT_PCIEIOUxBIFCTRL_LNKCFG);
  BifCtrlData |= (Iou1 | B_PCIE_PORT_PCIEIOUxBIFCTRL_SBIF);    // or in bifurcation control and set start bifurcation control bit
  IioPciExpressWrite16 (IioGlobalData,PCI_PCIE_ADDR(BusNumber, IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].Device,IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].Function, R_PCIE_PORT_PCIEIOUxBIFCTRL) ,BifCtrlData);
}

VOID
IioInitDriverInstalled (
  IIO_GLOBALS       *IioGlobalData
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
  UINT8   Iio;
  UINT8   Phase;

  for (Phase = 1; Phase <= IIOInitPhase3; Phase = Phase << 1) {

    // early link init in phases
    for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
      if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

      if(Phase == IIOInitPhase1){   //early pre-link training phase
        ProgramIioClockConfiguration (IioGlobalData, Iio);
        IioEarlyWorkAround(IioGlobalData, Iio, IioBeforeBifurcation);
        DfxPcieInit(IioGlobalData, Iio);
        IioPreLinkInit(IioGlobalData, Iio);
      }

      if(Phase == IIOInitPhase2){   //early link training phase
        ProgramGen3Equalization(IioGlobalData, Iio);
        CheckGen3PreLinkOverride(IioGlobalData, Iio); 
        PcieLinkTrainingInit(IioGlobalData, Iio);
      }

      if(Phase == IIOInitPhase3){   //early post link training phase
        CheckGen3Override(IioGlobalData, Iio);
      }

    } //for each IIO

    if(Phase == IIOInitPhase2){
      // add any link training delay which would be common for all (out of spec)
      IioStall (IioGlobalData, LINK_TRAINING_DELAY);   //400ms delay
      DumpIioPciePortPcieLinkStatus(IioGlobalData);    //dump link status
    }

    if(Phase == IIOInitPhase3){
      // add any link training delay which would be common for all (out of spec)
      IioStall (IioGlobalData, IIO_LINK_TRAIN_TIME_MARGIN_G3EQ);   //200ms delay

      for ( Iio = 0 ; Iio < MaxIIO ; Iio++ ){
        if (!IioGlobalData->IioVData.SocketPresent[Iio]) continue;

        IioEarlyWorkAround(IioGlobalData, Iio, IioAfterBifurcation);
      }   //for each IIO
      DumpIioPciePortPcieLinkStatus(IioGlobalData);    //dump link status
    }

  }   //link init in phases
}


VOID  
DumpIioPciePortPcieLinkStatus (
  IIO_GLOBALS         *IioGlobalData
  )
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress;
  UINT8 i, j;
  UINT16  wdata;

  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "\tDumpIioPcieLinkStatus().....\n");
  for (i = 0 ; i < MaxIIO ; i++) {
    if (!IioGlobalData->IioVData.SocketPresent[i])
           continue;

    for (j = 0 ; j < NUMBER_PORTS_PER_SOCKET ; j++) {
      PciAddress.Bus = IioGlobalData->IioVData.SocketBaseBusNumber[i];
      PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
      if( IioVData_ConfigurePciePort(IioGlobalData, i, j) == FALSE)  //check whether the PCIe port can be configured
        continue;
      if( j < 1)  continue;

      wdata = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_PXPLNKSTS));
      if(((wdata >> 4)& 0x3F) == 0) {
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, " Skt[%d], D[%d]:F[%d] ", i, PciAddress.Device, PciAddress.Function);
        IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, " Link Down! \n");
      } else {
        if((((wdata >> 4) & 0x3F) == 1) ||
           (((wdata >> 4) & 0x3F) == 2) ||
           (((wdata >> 4) & 0x3F) == 4) ||
           (((wdata >> 4) & 0x3F) == 8) ||
           (((wdata >> 4) & 0x3F) == 16)) {
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, " Skt[%d], D[%d]:F[%d] ", i, PciAddress.Device, PciAddress.Function);
          IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR, "\t\tLink up as x%02d Gen%d! \n", ((wdata >> 4) & 0x3F), (wdata & 0xF));
        } 
      }
    }
  }
}

VOID
LimitPcieLinkWidth(
    IIO_GLOBALS                           *IioGlobalData,
    UINT8                                 Iio
)
{
  UINT8        BusBase = 0;
  UINT8        j;
  UINT8        DefaultMaxLinkWidth;
  UINT8        OverrideMaxLinkWidthOptVal;
  UINT8        OverrideMaxLinkWidth[NUMBER_PORTS_PER_SOCKET] = {0};
  UINT16       LaneMaskVal[4] = {0};
  UINT8        LaneMaskValIndex;
  UINT8        *ArrayMaxLinkWidth;
  UINT8        *ArrayReverseLink;;
  
  //
  // Collect data about overridden max link widths
  //
  for ( j = 0; j < NUMBER_PORTS_PER_SOCKET ;j++) {
    BusBase      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    if( IioVData_ConfigurePciePort(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
      continue;

    // Store default max link width
    DefaultMaxLinkWidth = IioGlobalData->IioOutData.CurrentPXPMap[(Iio * NUMBER_PORTS_PER_SOCKET) + j];

    // Check setup option and choose desired width
    OverrideMaxLinkWidthOptVal = IioGlobalData->BdxSetupData.PciePortLinkMaxWidth[(Iio * NUMBER_PORTS_PER_SOCKET) + j];
    if(OverrideMaxLinkWidthOptVal == 0) {
      OverrideMaxLinkWidth[j] = DefaultMaxLinkWidth;
      continue;
    }
    
    //
    // Find option setting
    //
    switch(OverrideMaxLinkWidthOptVal) {
    case 1:
      // x1
      OverrideMaxLinkWidth[j] = 1;
      break;
    case 2:
      // x2
      OverrideMaxLinkWidth[j] = 2;
      break;
    case 3:
      // x4
      OverrideMaxLinkWidth[j] = 4;
      break;
    case 4:
      // x8
      OverrideMaxLinkWidth[j] = 8;
      break;
    case 5:
      // x16
      OverrideMaxLinkWidth[j] = 16;
      break;
    default:
      // Shouldn't happen, set to default bifurcated link width
      OverrideMaxLinkWidth[j] = DefaultMaxLinkWidth;
      break;
     }

    // Check to make sure Override Max doesn't exceed the max possible by bifurcation
    if(OverrideMaxLinkWidth[j] > DefaultMaxLinkWidth) {
      OverrideMaxLinkWidth[j] = DefaultMaxLinkWidth;
    }

    if(DefaultMaxLinkWidth != OverrideMaxLinkWidth[j]) {
      // Document change of link width, if there is any change
      IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_INFO, "Link Width (IIO%d, port%s(%d)) forced from default x%d to override x%d\n", Iio, IIO_PORT_LABEL(j), j, DefaultMaxLinkWidth, OverrideMaxLinkWidth[j]);
    }
  }
  
  //
  // Fetch setup data
  //
  ArrayMaxLinkWidth = &(IioGlobalData->IioOutData.MaxPXPMap[0]);
  ArrayReverseLink = &(IioGlobalData->IioOutData.LaneReversedPXPMap[0]);
  
  //
  // Calculate port masks for slots where max link width has been overridden
  //
  for ( j = 0; j < NUMBER_PORTS_PER_SOCKET ;j++) {
    BusBase      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

    if( IioVData_ConfigurePciePort(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
      continue;

    DefaultMaxLinkWidth = IioGlobalData->IioOutData.CurrentPXPMap[(Iio * NUMBER_PORTS_PER_SOCKET) + j];
    OverrideMaxLinkWidthOptVal = IioGlobalData->BdxSetupData.PciePortLinkMaxWidth[(Iio * NUMBER_PORTS_PER_SOCKET) + j];

    if((OverrideMaxLinkWidthOptVal == 0) || (OverrideMaxLinkWidth[j] == DefaultMaxLinkWidth)) 
      continue;

    ArrayMaxLinkWidth[(Iio * NUMBER_PORTS_PER_SOCKET) + j] = OverrideMaxLinkWidth[j];
      
    //
    // Select index into LaneMask array based on which port (j) is being configured
    //
    switch(j) { 
    case 0:
      LaneMaskValIndex = 0;
      break;
    case 1:
    case 2:
      LaneMaskValIndex = 1;
      break;
    case 3:
    case 4:
    case 5:
    case 6:
      LaneMaskValIndex = 2;
      break;
    case 7:
    case 8:
    case 9:
    case 10:
      LaneMaskValIndex = 3;
      break;
    default:
      // Invalid value
      LaneMaskValIndex = 0;
      break;
    }

    switch(DefaultMaxLinkWidth) {
    case 16:
      // if port is bifurcated to x16, force all 16 lanes to the mask specified by the override link width
      switch(OverrideMaxLinkWidth[j]) {
      case 8:
        // Force x16 to x8
        if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
          LaneMaskVal[LaneMaskValIndex] = 0x00FF;
        } else {
          LaneMaskVal[LaneMaskValIndex] = 0xFF00;
        }
        break;
      case 4:
        // Force x16 to x4
        if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
          LaneMaskVal[LaneMaskValIndex] = 0x0FFF;
        } else {
          LaneMaskVal[LaneMaskValIndex] = 0xFFF0;
        }
        break;
      case 2:
        // Force x16 to x2
        if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
          LaneMaskVal[LaneMaskValIndex] = 0x3FFF;
        } else {
          LaneMaskVal[LaneMaskValIndex] = 0xFFFC;
        }
        break;
      case 1:
        // Force x16 to x1
        if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
          LaneMaskVal[LaneMaskValIndex] = 0x7FFF;
        } else {
          LaneMaskVal[LaneMaskValIndex] = 0xFFFE;
        }
        break;
      default:
        LaneMaskVal[LaneMaskValIndex] = 0x0000;
        break;
      }
      break;
    case 8:
      // if port is bifurcated to x8, set lane mask according to birfurcation and which port it is.
      switch(OverrideMaxLinkWidth[j]) {
      case 4:
        // Force x8 to x4
        switch(j) {
        case 1:
        case 3:
        case 7:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x000F;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x00F0;
          }
          break;
        case 5:
        case 9:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0F00;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0xF000;
          }
          break;
        default:
          LaneMaskVal[LaneMaskValIndex] |= 0x0000;
          break;
        }
        break;
      case 2:
        // Force x8 to x2
        switch(j) {
        case 1:
        case 3:
        case 7:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x003F;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x00FC;
          }
          break;
        case 5:
        case 9:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x3F00;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0xFC00;
          }
          break;
        default:
          LaneMaskVal[LaneMaskValIndex] |= 0x0000;
          break;
        }
        break;
      case 1:
        // Force x8 to x1
        switch(j) {
        case 1:
        case 3:
        case 7:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x007F;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x00FE;
          }
          break;
        case 5:
        case 9:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x7F00;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0xFE00;
          }
          break;
        default:
          LaneMaskVal[LaneMaskValIndex] |= 0x0000;
          break;
        }
        break;
      }
      break;
    case 4:
      switch(OverrideMaxLinkWidth[j]) {
      case 2:
        // Force x4 to x2
        switch(j) {
        case 0:
        case 1:
        case 3:
        case 7:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0003;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x000C;
          }
          break;
        case 2:
        case 4:
        case 8:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0030;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x00C0;
          }
          break;
        case 5:
        case 9:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0300;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x0C00;
          }
          break;
        case 6:
        case 10:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x3000;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0xC000;
          }
          break;
        default:
          LaneMaskVal[LaneMaskValIndex] |= 0x0000;
          break;
        }
        break;
      case 1:
        // Force x4 to x1
        switch(j) {
        case 0:
        case 1:
        case 3:
        case 7:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0007;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x000E;
          }
          break;
        case 2:
        case 4:
        case 8:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0070;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x00E0;
          }
          break;
        case 5:
        case 9:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x0700;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0x0E00;
          }
          break;
        case 6:
        case 10:
          if(ArrayReverseLink[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) {
            LaneMaskVal[LaneMaskValIndex] |= 0x7000;
          } else {
            LaneMaskVal[LaneMaskValIndex] |= 0xE000;
          }
          break;
        default:
          LaneMaskVal[LaneMaskValIndex] |= 0x0000;
          break;
        }
        break;
      }
      break;
    }    
  }
   
  //
  // Write lane-disable values calcuated by this function
  //
  IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].DfxDevice, IioGlobalData->PreLinkData.PcieInfo.PortInfo[0].DfxClusterFunction,
      ONLY_REGISTER_OFFSET(PCIE_LANE_MASK_IIO_DFX_REG)), LaneMaskVal[0]);    
  IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].DfxDevice, IioGlobalData->PreLinkData.PcieInfo.PortInfo[1].DfxClusterFunction,
      ONLY_REGISTER_OFFSET(PCIE_LANE_MASK_IIO_DFX_REG)), LaneMaskVal[1]);    
  IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].DfxDevice, IioGlobalData->PreLinkData.PcieInfo.PortInfo[3].DfxClusterFunction,
      ONLY_REGISTER_OFFSET(PCIE_LANE_MASK_IIO_DFX_REG)), LaneMaskVal[2]);    
  IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(BusBase, IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].DfxDevice, IioGlobalData->PreLinkData.PcieInfo.PortInfo[7].DfxClusterFunction,
      ONLY_REGISTER_OFFSET(PCIE_LANE_MASK_IIO_DFX_REG)), LaneMaskVal[3]);    
}

void
InitPortClockEarly (
    IIO_GLOBALS                                   *IioGlobalData,
    PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress,
    UINT8                                         PcieSlotClockSource,
    UINT8                                         PciePortClockConfiguration
)
{
  UINT32         ClkCtrlReg, ClkStsReg;
  UINT16         Var16;

  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    ClkCtrlReg = R_PCIE_PORT_PXPLNKCTRL;
  else
    ClkCtrlReg = R_DMI_PORT_PXPLNKCTRL;
  //
  // Program PCIe port clock source
  //
  if(PcieSlotClockSource != 0xFF) {   //skip programming the slot clock source configuration if FFh
    if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
      ClkStsReg = R_PCIE_PORT_PXPLNKSTS;
    else
      ClkStsReg = R_DMI_PORT_PXPLNKSTS;

    Var16 = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
              ClkStsReg));
    Var16 &= ~(B_PCIE_PORT_PXPLNKSTS_SCCON);
    Var16 |= (PcieSlotClockSource) ? B_PCIE_PORT_PXPLNKSTS_SCCON : 0;
    IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
        ClkStsReg), Var16);
  }

  //
  // Program PCIe port clocking
  //
  if (OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == FALSE)
    ClkCtrlReg = R_PCIE_PORT_PXPLNKCTRL;
  else
    ClkCtrlReg = R_DMI_PORT_PXPLNKCTRL;

  Var16 = IioPciExpressRead16( IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function, ClkCtrlReg));
  Var16 &= ~(B_PCIE_PORT_PXPLNKCTRL_CCCON + B_PCIE_PORT_PXPLNKCTRL_LNKDIS);
  Var16 |= (PciePortClockConfiguration) ? B_PCIE_PORT_PXPLNKCTRL_CCCON : 0;
  IioPciExpressWrite16 (IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,
      ClkCtrlReg), Var16);
  IioStall (IioGlobalData, GEN3_STALL_SBR); //2ms delay = SBR flip interval
}

VOID
ProgramIioClockConfiguration (
    IIO_GLOBALS                           *IioGlobalData,
    UINT8                                 Iio
)
{
  PCI_ROOT_BRIDGE_PCI_ADDRESS PciAddress;
  UINT8                                       j, PortSerialNumber;

  for ( j = 0; j < NUMBER_PORTS_PER_SOCKET ;j++) {
    PciAddress.Bus      = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    PciAddress.Device   = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

    if( IioVData_ConfigurePciePort(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
      continue;
    //
    // make the root port to operate on common clock mode, don't care about
    // receiver side clock configuration during pre-link configuration
    PortSerialNumber = (Iio * NUMBER_PORTS_PER_SOCKET) + j;
    InitPortClockEarly(IioGlobalData,
                  PciAddress,
                  0xFF,                //skip slot's clock source config here
                  IioGlobalData->SetupData.PcieCommonClock[PortSerialNumber]);
  }

}

VOID
IioPreLinkInit(
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   Iio
  )
{
  //
  // Program PCIE ECRC
  //
  ProgramEcrc(IioGlobalData,Iio);
}

VOID
ProgramEcrc(
  IIO_GLOBALS  *IioGlobalData,
  UINT8  Iio
  )
{  
  PCI_ROOT_BRIDGE_PCI_ADDRESS                 PciAddress;
  UINT8                                       j;
  ERRCAP_IIO_PCIE_STRUCT                      Errcap;
  UINT32                                       PortIndex=0;
  UINT8                                       *Array=NULL;


  for ( j = 0; j < NUMBER_PORTS_PER_SOCKET ;j++) {
    PciAddress.Bus		= IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
    PciAddress.Device	= IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    PciAddress.Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;

    if (IioVData_ConfigurePciePort(IioGlobalData, Iio, j) == FALSE)  //check whether the PCIe port can be configured
      continue;

    if ( OemIdentifyDmiPort(IioGlobalData, PciAddress.Bus, PciAddress.Device, PciAddress.Function) == TRUE)
      continue;

    if(!IioGlobalData->IioOutData.CurrentPXPMap[(Iio*NUMBER_PORTS_PER_SOCKET)+j]) continue;

    PCIEDEVFUNC2PORTINDEX(PortIndex, PciAddress.Device, PciAddress.Function);
    if (((Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex) >= MAX_TOTAL_IIO_PORTS) return;

    Array = &(IioGlobalData->BdxSetupData.Ecrc[0]);

    Errcap.Data = IioPciExpressRead32 (IioGlobalData,PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,ERRCAP_IIO_PCIE_REG ));

    //enable ecrc check 
    if (Errcap.Bits.ecrc_check_capable == 1) {
      if(Array[(Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 2) {// if user input is auto, skip programming
      }else {
         Errcap.Bits.ecrc_check_enable = (Array[(Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex] != 0)? 1 : 0;
      }
    }//eod if(Errcap
      
     //enable ecrc generation 
    if (Errcap.Bits.ecrc_generation_capable == 1) {
      if(Array[(Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex] == 2) {// if user input is auto, skip programming
      }else {
         Errcap.Bits.ecrc_generation_enable = (Array[(Iio*NUMBER_PORTS_PER_SOCKET)+PortIndex] != 0)? 1 : 0;
      }
    }//eod if(Errcap

    IioPciExpressWrite32(IioGlobalData, PCI_PCIE_ADDR(PciAddress.Bus, PciAddress.Device, PciAddress.Function,ONLY_REGISTER_OFFSET(ERRCAP_IIO_PCIE_REG)),Errcap.Data);

  }

}

VOID
IioEarlyPreLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
  )
{
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,"IioEarlyPreLinkTrainingPhase End \n");
  ProgramIioClockConfiguration (IioGlobalData, Iio);
  IioEarlyWorkAround(IioGlobalData, Iio, IioBeforeBifurcation);
  DfxPcieInit(IioGlobalData, Iio);
  IioPreLinkInit(IioGlobalData, Iio);
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,"IioEarlyPreLinkTrainingPhase End \n");
}

VOID
IioEarlyPcieLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
  )
{
  ProgramGen3Equalization(IioGlobalData, Iio);
  CheckGen3PreLinkOverride(IioGlobalData, Iio);
  PcieLinkTrainingInit(IioGlobalData, Iio);
}

VOID
IioEarlyPostLinkTrainingPhase(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               Iio
  )
{
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,"IioEarlyPreLinkTrainingPhase %x \n",IioGlobalData);
  IioDebugPrintInfo(IioGlobalData, IIO_DEBUG_ERROR,"IioGlobalData->IioVData.PmBase =%x\n",IioGlobalData->IioVData.PmBase);
  //early post link training phase
  //Note: make sure before this routine is call there is a big delay for PCie link
  // training for that same IIO is covered before this routine is invoked.
  CheckGen3Override(IioGlobalData, Iio);
  IioEarlyWorkAround(IioGlobalData, Iio, IioAfterBifurcation);
}
