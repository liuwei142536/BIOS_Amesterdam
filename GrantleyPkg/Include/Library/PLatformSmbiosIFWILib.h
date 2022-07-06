/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformSmbiosIFWILib.h

Abstract:
  This file will contain all definitions related to SmbiosIFWIDxe. 

------------------------------------------------------------------*/
	
#ifndef _PLATFORM_SMBIOS_IFWI_LIB_H
#define _PLATFORM_SMBIOS_IFWI_LIB_H

#define DFT_ID "SmbiosIFWI"
#define DEBUG_FUN_ENTRY DEBUG((EFI_D_INFO, "["DFT_ID"] "__FUNCTION__" Entry\n"))
#define DEBUG_FUN_EXIT DEBUG((EFI_D_INFO, "["DFT_ID"] "__FUNCTION__" Exit\n"))

EFI_STATUS
Plat_GetRCVer(
char *VersionString
);
VOID
Plat_GetMEVer(
char *VersionString
);
#endif


