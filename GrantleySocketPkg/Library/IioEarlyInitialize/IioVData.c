
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

  IioVData.c

Abstract:

  IioVData initialization.

--*/

#include "IioEarlyInitialize.h"


VOID
IioVData_DisablePciePortConfigure(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    )
{
  IioGlobalData->IioOutData.PciePortConfig[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex] = 0;
  return;
}

BOOLEAN
IioVData_ConfigurePciePort(
    IIO_GLOBALS                     *IioGlobalData,
    UINT8                           Iio,
    UINT8                           PortIndex
    )
{
  return (IioGlobalData->IioOutData.PciePortConfig[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex] == 0)  \
          ? FALSE  : TRUE;
}

VOID
IioVDataPostLinkInit(
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                           Iio
  )
{
  UINT8 j, BusBase, Device, Function;
  UINT16 BifurcationData;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];
  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {
    if (j == 0 || j == 1 || j == 3 || j == 7) {
      Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
      Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
      //
      // Get PCIE IOU Bifurcation register
      //
      BifurcationData = IioPciExpressRead16(IioGlobalData, PCI_PCIE_ADDR(BusBase, Device, Function, ONLY_REGISTER_OFFSET(PCIE_IOU_BIF_CTRL_IIO_PCIE_REG)));
      IioDebugPrintInfo(IioGlobalData,IIO_DEBUG_INFO, "IIO=%d, Port index = %s(%d), BIFCTRL=0x%x.\n", Iio, IIO_PORT_LABEL(j), j, (BifurcationData & 0x7));

    }
  }
}


VOID
UpdateVDataAfterApplyWorkAround(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         Iio
  )
{
  UINT8   j;
  UINT8   BusBase;
  UINT8   Device = 0;
  UINT8   Function = 0;
  UINT8   IioDfxDev = 0;
  UINT8   IioDfxFunc = 0;

  BusBase = IioGlobalData->IioVData.SocketBaseBusNumber[Iio];

  for(j = 0; j < NUMBER_PORTS_PER_SOCKET; j++) {

    //
    // Do not continue buffer setup if device is set for dont configure 
    //
    if (!IioGlobalData->IioOutData.PciePortConfig[(Iio * NUMBER_PORTS_PER_SOCKET) + j]) continue;

    Device = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Device;
    Function = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].Function;
    IioDfxDev = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxDevice;
    IioDfxFunc = IioGlobalData->PreLinkData.PcieInfo.PortInfo[j].DfxFunction;
  }
}

BOOLEAN
IsThisUplinkPort(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  if(Iio < MaxIIO){
    if(IioGlobalData->PreLinkData.UplinkInfo[Iio].Valid &&
       IioGlobalData->PreLinkData.UplinkInfo[Iio].IioUplinkPortIndex == PortIndex)
      return TRUE;
    else
      return FALSE;
  }
  else
    return FALSE;
}

BOOLEAN
CheckVarPortEnable(
    IIO_GLOBALS                                         *IioGlobalData,
    UINT8                                               Iio,
    UINT8                                               PortIndex
)
{
  UINT8 PciePortDisable;

  PciePortDisable = \
      IioGlobalData->SetupData.PciePortDisable[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(PciePortDisable == 0){   //auto
    // If PciePortDisable is set to "AUTO", than take care of port specific
    // special requirements, otherwise set the ports to enable by default
    // for example, as the PO safe config the requirement was to keep the
    // uplink port (if any) disable by default....
    if (IsThisUplinkPort(IioGlobalData, Iio, PortIndex) == TRUE)
      return FALSE;             //to indicate port is to be disabled
    else
      return TRUE;              //to indicate port is to be enabled

  }else if(PciePortDisable == 1)   //enable
    return TRUE;                //to indicate port is to be enabled
  else if(PciePortDisable == 2)   //disable
    return FALSE;               //to indicate port is to be disabled
  else
    return TRUE;                //to indicate port is to be enabled
}

UINT8
CheckVarPortLinkSpeed(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  UINT8   PcieLinkSpeed;

  PcieLinkSpeed = \
      IioGlobalData->SetupData.PciePortLinkSpeed[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(PcieLinkSpeed == PCIE_LINK_SPEED_AUTO){
    //
    // HSD 3616041: change S0 port3D (uplink port) default to Gen1.
    if (IsThisUplinkPort(IioGlobalData, Iio, PortIndex) == TRUE){
      return PCIE_LINK_SPEED_GEN1;       //Gen1 speed
    }
    else if (PortIndex == NONE_GEN3_PCIE_PORT)  //port 0 cannot go to Gen3
      return PCIE_LINK_SPEED_GEN2;              //expected HW default for PCIe port 0
    else
      return PCIE_LINK_SPEED_GEN3;              //expected HW default for all other ports
  }
  else
    return PcieLinkSpeed;
}

BOOLEAN
ChangeVarPortLinkSpeed(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  //
  // HSD 3616041: change S0 port3D (uplink port) default to Gen1.
  //
  if (IsThisUplinkPort(IioGlobalData, Iio, PortIndex) == TRUE)
    return TRUE;            //speed change required in case of AUTO
  else
    return FALSE;           // no speed change required in case of other ports
}

UINT8
CheckVarPortASPM(
    IIO_GLOBALS                                     *IioGlobalData,
    UINT8                                           Iio,
    UINT8                                           PortIndex
)
{
  UINT8       Aspm;

  Aspm = IioGlobalData->SetupData.PcieAspm[(Iio * NUMBER_PORTS_PER_SOCKET) + PortIndex];

  if(Aspm == PCIE_ASPM_AUTO){
    //as per ss4030806 - if uplink enabled than ASPM = L1
    if (IsThisUplinkPort(IioGlobalData, Iio, (UINT8)PortIndex) == TRUE)
      Aspm = PCIE_ASPM_L1_ONLY;     // ASPM = L1
    else
      Aspm = PCIE_ASPM_DISABLE;     // ASPM = disable
  }

  return Aspm;
}
/*---------------------------------------------------------------------------*/
