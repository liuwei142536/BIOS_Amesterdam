/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  1999 - 2009 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  
    IioInitPeim.c
   
Abstract:

    Functions for IOH PEI initilization
    
--*/

#include "PlatformEarlyInit.h"

VOID
IioDmiInit (
  VOID
  )
{

}

VOID 
PeiIioInit(
  VOID
  )
{
      
  IioDmiInit();
  
}
