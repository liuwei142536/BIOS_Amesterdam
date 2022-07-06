//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
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

    OemCpuInit.h

Abstract:

    Oem Hooks file for Cpu Init Module

--*/
#ifndef _CPU_OEMINITIALIZE_H_
#define _CPU_OEMINITIALIZE_H_

/**
  This is an OEM hook to initialize CPU thermal levels as per other external chip.
  
  @param  pCpuPmLib   Pointer to CPU PM struct
 **/
VOID
OemCpuOtherChipThermalInit (EFI_CPU_PM_STRUCT *pCpuPmLib);

#endif
