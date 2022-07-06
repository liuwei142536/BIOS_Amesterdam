/**
  This file contains a 'Sample Driver' and is licensed as such  
  under the terms of your license agreement with Intel or your  
  vendor.  This file may be modified by the user, subject to    
  the additional terms of the license agreement 
**/
/**

Copyright (c) 2011 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file 
  PchBiosWriteProtect.h

  @brief 
  Header file for the Pch Bios Write Protect Driver.

**/
#ifndef _PCH_BIOS_WRITE_PROTECT_H_
#define _PCH_BIOS_WRITE_PROTECT_H_

//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <PchSmmAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#else
#include <PchAccess.h>
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Library/PchPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciExpressLib.h>
///
/// Driver Dependency Protocols
///
#include <Protocol/PchPlatformPolicy.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmIchnDispatch2.h>
#include <Protocol/SmmIoTrapDispatch2.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiLib.h>

#endif
