/*++
  This file contains an 'Intel Peripheral Driver' and uniquely  
  identified as "Intel Reference Module" and is                 
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/

/*++

Copyright (c)  1999 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:

  SaDmiPeim.h

Abstract:

  Header file for the SA Dmi Init library.

--*/

#ifndef _IIO_DMI_PEIM_H_
#define _IIO_DMI_PEIM_H_

#include "Token.h" // Aptio V server override

#include <Iio\IioAccess.h>
#include <Setup\IioUniversalData.h>
#include <PPI\PchInit.h>
#include <PPI\PchDmiTcVcMap.h>
#include <PPI\PchPlatformPolicy.h>
#include <PPI\ReadOnlyVariable2.h>
#include <PPI\MemoryDiscovered.h>
#include <Guid\PlatformInfo.h>
#include <Library\DebugLib.h>
#include <Library\HobLib.h>
#include <Library\BaseMemoryLib.h>
#include <Library\IoLib.h>
#include <Library\PciLib.h>
#include <Library\PciExpressLib.h>
#define _EFI_PCI_CALLBACK_H		//for not including the <Protocol/PciCallback.h>
#include <Library\IioPlatformData.h>
#include "CpuPciAccess.h"
#include <Library/PlatformHooksLib.h>
#include "SysFunc.h"

#define TEMP_DMI_BAR 0xCFFFF000//0xFED1B000
//
// DMI input structure
//
typedef struct {
  UINT8 IsocEn;       // Isoc Enabled
  UINT8 MeSegEn;      // MeSeg Enabled
  UINT8 DmiVc1;       // 0 - Disable, 1 - Enable  
  UINT8 DmiVcp;       // 0 - Disable, 1 - Enable  
  UINT8 DmiVcm;       // 0 - Disable, 1 - Enable  
} VC_SETUP_INPUT;

//
// Functions
//
EFI_STATUS
EFIAPI
DmiTcVcSetup (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
;


EFI_STATUS
SaSetDmiTcVcMapping (
  IN    PCH_DMI_TC_VC_PPI  *PchDmiTcVcMapPpi,
  IN    UINT64             DmiBar
  )
/*++

Routine Description:

  Map SA DMI TCs to VC

Arguments:

  PchDmiTcVcMapPpi     - Instance of PCH_DMI_TC_VC_PPI
  DmiBar               - DMIBAR address

Returns:

  EFI_SUCCESS

--*/
;

EFI_STATUS
SaPollDmiVcStatus (
  IN    PCH_DMI_TC_VC_PPI  *PchDmiTcVcMapPpi,
  IN    UINT64             DmiBar
  )
/*++

Routine Description:

  Poll SA DMI negotiation completion

Arguments:

  PchDmiTcVcMapPpi     - Instance of PCH_DMI_TC_VC_PPI
  DmiBar               - DMIBAR address

Returns:

  EFI_SUCCESS

--*/
;

VOID
DmiLinkTrain (
  IN  UINT64 DmiBar
  )
/*++

Routine Description:

  DMI link training
  
Arguments:

  DmiBar - DMIBAR address

Returns:

  None

--*/
;

VOID
InitDmiSpecificSiWorkaorunds  (
    IN CONST EFI_PEI_SERVICES    **PeiServices,
    IN  IIO_UDS                   *IioUds
);

VOID
IioMiscPeiInit  (
	IN  CONST EFI_PEI_SERVICES    **PeiServices
);

UINT32 
Bios2PcodeMailboxCommand(
  IN UINT8  UncoreBusNo,
  IN UINT32 MailboxCmd,
  IN UINT32 MailboxData
);

#ifdef DE_SKU
EFI_STATUS
EFIAPI
IioSouthComplexPeiInit  (
  IN  CONST EFI_PEI_SERVICES      **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
);

VOID
ProcessSC_Gbe_PF( 
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  IN  IIO_UDS                   *IioUds
);

VOID
IioSouthComplexUartInit(
  BOOLEAN                         EnableSCUart
);

VOID
GetPlatformSouthComplexPolicy(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN OUT   SC_SETUP_INPUT       *ScInput

);

BOOLEAN
Override_SC_LANPorts(
  IN	UINT8	UncoreBusNo,
  IN    UINT32  RegOffset,
  IN	UINT8	EnableLanPort0,
  IN	UINT8	EnableLanPort1
);
#endif // DE_SKU

VOID 
IioIsocConfiguration(
  CONST EFI_PEI_SERVICES    **PeiServices,
  IIO_UDS                   *IioUds,
  PCH_DMI_TC_VC_PPI         *PchDmiTcVcMapPpi
);


#endif // _IIO_DMI_PEIM_H_
