//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IioCommonRcConfigData.h
    
Abstract:
  Data format for Universal Data Structure

--*/

    //
    //  Common Section of RC
    //
    UINT32   MmcfgBase;
    UINT32   MmcfgSize;
    UINT32   MmiolBase;
    UINT32   MmiolSize;
    UINT32   MmiohBase;
    UINT32   MmiohSize;
    UINT8   IsocEn;
    UINT8   MeSegEn;   
    UINT8   NumaEn;



