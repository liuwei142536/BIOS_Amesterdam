//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2011 Intel Corporation. All rights reservedb
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    PpmProcessorSupport.h
  
Abstract:

  This protocol provide the callback for the PM feature programming in the CPUMPDXE driver

Revision History

--*/

#ifndef _EFI_PPM_PROCESSOR_SUPPORT_H
#define _EFI_PPM_PROCESSOR_SUPPORT_H



//
// Global Id for PM call back
//
#define EFI_PPM_PROCESSOR_SUPPORT_PROTOCOL_GUID \
  { \
	0x627f0077, 0x5e97, 0x4b34, 0xac, 0xf1, 0xcc, 0x47, 0x37, 0x1, 0xd1, 0x38 \
  }  

extern EFI_GUID gEfiPpmProcessorSupportProtocolGuid;

#endif
