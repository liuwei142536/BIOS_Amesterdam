/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++
  
  Copyright (c) 2012-2013 Intel Corporation.  All rights reserved.

  This software and associated documentation (if any) is furnished under 
  a license and may only be used or copied in accordance with the terms 
  of the license.  Except as permitted by such license, no part of this 
  software or documentation may be reproduced, stored in a retrieval system, 
  or transmitted in any form or by any means without the express written 
  consent of Intel Corporation.
  
  Module Name:
  
    SpsNvs.asl
  
  Abstract:

    This file defines ASL code for SPS ME firware support.
  
--*/

DefinitionBlock("SpsNvs.aml", "SSDT", 2, "INTEL", "SpsNvs  ", 2)
{
  Scope(\)
  {
    Name(PURA, 0)
    Name(PURB, 0)
    Name(PURE, 0)
    
    //
    // The address below is just stub to be updated in UpdateAslCode().
    //
    OperationRegion(MEVS, SystemMemory, 0x80000000, 0x4)
    Field(MEVS, AnyAcc, Lock, Preserve)
    {
      THNU, 8,
      H1UI, 8,
      H2UI, 8, 
      OSLD, 8,
    }
  } // Scope (\)
}

