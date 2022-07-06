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

  IioEarlyInit.c

Abstract:

  Houses all code related to Initialize IIO before Port Initialization

Revision History:

--*/

#include <Library/OemIioInit.h>
#include <Library/IioEarlyInitializeLib.h>
#include "IioEarlyInit.h"
#include "PciAccess.h"



VOID
InitializeIioGlobalDataPerSocket (
  struct sysHost       *host,
  UINT8                Iio
  )
/*++

Routine Description:

  Initialize IIO Global Data

Arguments:
  *host     - Pointer to syshost struture
  Iio       - Index to Socket

Returns:
  None
--*/
{
  IIO_GLOBALS *IioGlobalData;
  IioGlobalData = &host->var.iio;
  //
  // Let IioInit Driver that link training has been done already in PEI
  //
  IioGlobalData->IioVData.IioLinkTrainPhase = IN_PEI_PHASE;

  IioGlobalData->IioVData.PciExpressBase = host->var.common.mmCfgBase;

  OemGetIioPlatformInfo(IioGlobalData);
  //
  // Update Other variables required for IIO Init
  //
  IioGlobalData->IioVData.CpuType     = host->var.common.cpuType;
  IioGlobalData->IioVData.CpuFamily   = host->var.common.cpuFamily;
  IioGlobalData->IioVData.SkuType     = host->var.common.skuType;

  IioGlobalData->IioVData.CpuStepping = host->var.common.stepping;
  IioGlobalData->IioVData.CpuSubType  = host->var.common.cpuSubType;
  IioGlobalData->IioVData.MctpEn      = host->var.common.mctpEn;
  IioGlobalData->IioVData.IsocEnable  = host->var.qpi.OutIsocEn;
  IioGlobalData->IioVData.MeSegEn     = host->var.qpi.OutMesegEn;
  IioGlobalData->IioVData.IOxAPICCallbackBootEvent = FALSE;

  IioGlobalData->IioVData.SocketPresent[Iio]         = (host->var.common.socketPresentBitMap & (1 << Iio)) ? 1: 0;
#ifdef SERIAL_DBG_MSG
  rcPrintf ((host, "[%x]SocketPresent %x\n", Iio, IioGlobalData->IioVData.SocketPresent[Iio]));
#endif //SERIAL_DBG_MSG
  IioGlobalData->IioVData.SocketBaseBusNumber[Iio]   = host->var.qpi.CpuInfo[Iio].CpuRes.BusBase;
  IioGlobalData->IioVData.SocketUncoreBusNumber[Iio] = host->var.qpi.CpuInfo[Iio].CpuRes.BusLimit;

  IioGlobalData->IioVData.RcbaAddress[Iio]           = (host->var.qpi.CpuInfo[Iio].CpuRes.MmiolLimit & (~0x3FFF))+ V_VT_D_IIO_BAR_SIZE;
  IioGlobalData->IioVData.IoApicBase[Iio]            = host->var.qpi.CpuInfo[Iio].CpuRes.IoApicBase;
  IioGlobalData->IioVData.PciResourceMem32Base[Iio]  = host->var.qpi.CpuInfo[Iio].CpuRes.MmiolBase;
  IioGlobalData->IioVData.PciResourceMem32Limit[Iio] = host->var.qpi.CpuInfo[Iio].CpuRes.MmiolLimit;

  OemIioPreLinkDataInitPerSocket(IioGlobalData, Iio);

  OemVariableGetCurrent(IioGlobalData);

  OemVariableInitPerSocket(IioGlobalData, Iio);
}


/*++

Routine Description:

  Execute the Phase 1 and 2 of IIO Ports initialization

Arguments:
  *host     - Pointer to syshost structure

Returns:
  None
--*/
VOID
IioEarlyLinkInit(
  struct sysHost             *host
)
{
  UINT8         Iio;
  IIO_GLOBALS   *IioGlobalData;

  IioGlobalData = &host->var.iio;

  if (IioGlobalData->IioVData.IioLinkTrainPhase == IN_PEI_PHASE) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "IIO Early Link Training Starting...\n"));
#endif
    //
    // Saves the pointer to the host structure
    //
    IioGlobalData->IioOutData.host = (VOID *)host;

    for (Iio = 0; Iio < MaxIIO; Iio++) {
#ifdef SERIAL_DBG_MSG
      rcPrintf((host, "Socket[%x] is socketValid = %x\n", Iio,  host->var.common.socketPresentBitMap & (1 << Iio)));
#endif
      if(!(host->var.common.socketPresentBitMap & (1 << Iio)))
        continue;
      InitializeIioGlobalDataPerSocket(host, Iio);
      IioOtherVariableInit(IioGlobalData, Iio);
      IioEarlyPreLinkTrainingPhase(IioGlobalData, Iio);
      IioEarlyPcieLinkTrainingPhase(IioGlobalData,Iio);
    }
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "IIO Early Link Training Completed!\n"));
#endif // SERIAL_DBG_MSG
  }
}

/*++

Routine Description:

  Execute the Phase 3 of IIO Ports initialization

Arguments: 
  *host     - Pointer to syshost structure
 
Returns:
  None
--*/
VOID
IioEarlyPostLinkInit(
  struct sysHost             *host
)
{
  UINT8 Iio;
  IIO_GLOBALS *IioGlobalData;

  IioGlobalData = &host->var.iio;

  if (IioGlobalData->IioVData.IioLinkTrainPhase == IN_PEI_PHASE) {
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "IIO Early Post Link Training Starting...\n"));
#endif // SERIAL_DBG_MSG
    for (Iio = 0; Iio < MaxIIO; Iio++) {
      if (!(host->var.common.socketPresentBitMap & (1 << Iio)))
        continue;
#ifdef SERIAL_DBG_MSG
      rcPrintf ((host, "Socket = %x\n", Iio));
#endif // SERIAL_DBG_MSG
      IioEarlyPostLinkTrainingPhase(IioGlobalData, Iio);
    }
    //
    //dump link status
    //
    DumpIioPciePortPcieLinkStatus(IioGlobalData);
#ifdef SERIAL_DBG_MSG
    rcPrintf ((host, "IIO Early Post Link Training Completed!\n"));
#endif // SERIAL_DBG_MSG
  }
}

VOID
IioEarlyPreDataLinkInit(
  struct sysHost             *host
)
/*++

Routine Description:

  Prior to execution of the Phase 1 and 2 of IIO Ports initialization (if any / optional)

Arguments:
  *host     - Pointer to syshost structure

Returns:
  None
--*/
{
  IIO_GLOBALS   *IioGlobalData;
  UINT32        MmCfgBase, MmCfgLimit;

  IioGlobalData = &host->var.iio;

  MmCfgBase = GetMmcfgAddress(host, 0);
  MmCfgLimit = GetMmcfgAddress(host, 1);
  if(MmCfgBase > MmCfgLimit)  //MMCFG base not init, quit
    return;

  IioGlobalData->IioVData.PciExpressBase = MmCfgBase;

}

VOID
InitializeIioDefaultData(
  struct sysHost             *host
)
/*++

  Routine Description:

    Initialize default data for IIO from reference code perspective

  Arguments:
   *host     - Pointer to syshost structure

  Returns:
    None
  --*/
{
}
