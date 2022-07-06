/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
Copyright (c)2009-2010 Intel Corporation.  All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license.  Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

Module Name:
    IsPlatformSupportWhea.h

Abstract:
    IsPlatformSupportWhea Protocol

-----------------------------------------------------------------------------*/


/*

	This module will be consumed by the Whea core module. The core Whea module will locate IsPlatformSupportWhea protocol.
	if successfully located, the core Whea module knows that this platform supports whea. And it can use 
	this protocol to enable or disable platform's  specific Whea capability
*/
//
// EPSD_IP_START
//

#ifndef _IS_PLATFORM_SUPPORT_WHEA_H_
#define _IS_PLATFORM_SUPPORT_WHEA_H_

typedef enum
{
	WheaNotSupported    = 0x00,
	WheaHestSupport     = 0x01,
	WheaBertSupport     = 0x02,
	WheaEinjSupport     = 0x04,
	WheaErstSupport     = 0x08,
	WheaSupported       = 0x0F  // WheaHestSupport|WheaBertFeature|WheaEinjFeature|WheaErstFeature
} WHEA_SUPPORT_LEVEL; 


typedef struct _EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL;

typedef
BOOLEAN
(EFIAPI *IS_PLATFORM_SUPPORT_WHEA)  (
IN EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL          *This
  );
/*++


Whea core calls this function to see if the specific platform support WHEA

--*/

typedef struct _EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL {
  WHEA_SUPPORT_LEVEL            WheaSupport;
  UINT8                         WheaAcpi5Support;
  IS_PLATFORM_SUPPORT_WHEA      IsPlatformSupportWhea;
} EFI_IS_PLATFORM_SUPPORT_WHEA_PROTOCOL;

extern EFI_GUID     gEfiIsPlatformSupportWheaProtocolGuid;

#endif /* _IS_PLATFORM_SUPPORT_WHEA_H_ */
//
// EPSD_IP_END
//

