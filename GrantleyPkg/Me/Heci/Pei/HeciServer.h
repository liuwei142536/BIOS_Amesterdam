/*++
 This file contains an 'Intel Peripheral Driver' and is        
 licensed for Intel CPUs and chipsets under the terms of your  
 license agreement with Intel or your vendor.  This file may   
 be modified by the user, subject to additional terms of the   
 license agreement                                             
--*/

/*++

Copyright (c) 2008-2013 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

File Name:

  HeciServer.h

Abstract:

  Heci2 driver core (Server).
  For PEI Phase, determines the HECI2 device and initializes it.

--*/

#ifndef _HECI_SERVER_H
#define _HECI_SERVER_H

EFI_STATUS
EFIAPI
Heci2GetMeStatus (
  IN CONST EFI_PEI_SERVICES **PeiServices,
  IN UINT32                 *MeStatus
  );

EFI_STATUS
EFIAPI
Heci2Initialize(
  IN CONST EFI_PEI_SERVICES **PeiServices,
  IN PEI_HECI2_PPI          *This,
  IN OUT  UINT32            *HeciMemBar
  );

#endif // _HECI_SERVER_H

