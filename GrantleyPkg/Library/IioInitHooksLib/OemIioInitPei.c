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

    OemIioInit.c

Abstract:

    Oem Hooks file for IioInit Module

--*/

#include "OemIioInit.h"
#include "OemIioInitCommon.h"
#include <Ppi/ReadOnlyVariable2.h>
#include <PiPei.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/PchDmiTcVcMap.h>
#include <Ppi/PchInit.h>

EFI_STATUS
GetSystemVariable(
    SYSTEM_CONFIGURATION  *SetupData
)
{
  UINT32                               VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI      *PeiVariable;
  EFI_PEI_SERVICES                     **PeiServices;
  EFI_STATUS                           Status;


  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();

  //
  // Locate Variable PPI
  //
  Status = (*PeiServices)->LocatePpi (
                                       PeiServices,
                                       &gEfiPeiReadOnlyVariable2PpiGuid,
                                       0,
                                       NULL,
                                       &PeiVariable
                                     );
  ASSERT_EFI_ERROR (Status);

  //
  // Obtain the system configuration data
  //
  VarSize = sizeof(SYSTEM_CONFIGURATION);
  return PeiVariable->GetVariable (
                                    PeiVariable,
                                    L"IntelSetup", //AptioV Grangeville Override
                                    &gEfiSetupVariableGuid,
                                    NULL,
                                    &VarSize,
                                    SetupData
                                  );
}


EFI_STATUS
SetSystemVariable(
    SYSTEM_CONFIGURATION  *SetupData,
    UINTN                 VarSize
)
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
LocateSmBus (
  VOID                     **SmBus
  )
{
  CONST EFI_PEI_SERVICES    **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  //
  // Locate SmBus Ppi
  //
  return (**PeiServices).LocatePpi (
                            PeiServices,
                            &gEfiPeiSmbus2PpiGuid,
                            0,
                            NULL,
                            SmBus
                            );
  }


VOID
OemPchDmiTcVcMapInit(
 IIO_GLOBALS               *IioGlobalData
)
{
  EFI_PEI_SERVICES             **PeiServices;
  EFI_STATUS                   Status;
  PCH_DMI_TC_VC_PPI            *PchDmiTcVcMapPpi;
  UINT8                        i;

  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchDmiTcVcMapPpiGuid, 0, NULL, &PchDmiTcVcMapPpi);
  ASSERT_EFI_ERROR (Status);
  
  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable = IioGlobalData->IioVData.DmiVc[DmiVcTypeVc1];
  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable = IioGlobalData->IioVData.DmiVc[DmiVcTypeVcp];
  PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable = IioGlobalData->IioVData.DmiVc[DmiVcTypeVcm];
  PchDmiTcVcMapPpi->WA_4167453 = 0;

  for (i = 0; i < MaxDmiTcType; i++) {
    if (((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVc1) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVc1].Enable == FALSE)) ||
        ((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVcp) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcp].Enable == FALSE)) ||
        ((PchDmiTcVcMapPpi->DmiTc[i].Vc == DmiVcTypeVcm) && (PchDmiTcVcMapPpi->DmiVc[DmiVcTypeVcm].Enable == FALSE)))
    {
      PchDmiTcVcMapPpi->DmiTc[i].Vc = DmiVcTypeVc0;
      IioGlobalData->IioVData.DmiTc[i] = DmiVcTypeVc0;
    } else {
      IioGlobalData->IioVData.DmiTc[i] = PchDmiTcVcMapPpi->DmiTc[i].Vc;
    }
  }
}

VOID
OemDmiTcVcProg(
  IIO_GLOBALS *IioGlobalData
){
  EFI_PEI_SERVICES       **PeiServices;  
  EFI_STATUS             Status;
  PCH_INIT_PPI          *PchInitPpi;
  
  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  //
  // Locate PchInitPpi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  ASSERT_EFI_ERROR (Status);
  
  Status = PchInitPpi->DmiTcVcProg (PeiServices);
  ASSERT_EFI_ERROR (Status);
}

VOID
OemDmiTcVcEnablePoll(
  IIO_GLOBALS *IioGlobalData
)
{
  EFI_PEI_SERVICES       **PeiServices;  
  EFI_STATUS             Status;
  PCH_INIT_PPI           *PchInitPpi;
  
  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  //
  // Locate PchInitPpi
  //
  Status = (UINT32)(*PeiServices)->LocatePpi (PeiServices, &gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  ASSERT_EFI_ERROR (Status);
  
  Status = PchInitPpi->DmiTcVcEnablePoll (PeiServices);
  ASSERT_EFI_ERROR (Status);
}

VOID
OemDmiGen2Prog(
 IIO_GLOBALS *IioGlobalData
)
{
  EFI_PEI_SERVICES       **PeiServices;  
  EFI_STATUS             Status;
  PCH_INIT_PPI           *PchInitPpi;
  
  PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
  //
  // Locate PchInitPpi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gPchInitPpiGuid, 0, NULL, &PchInitPpi);
  ASSERT_EFI_ERROR (Status);
  
  PchInitPpi->DmiGen2Prog(PeiServices);
}
