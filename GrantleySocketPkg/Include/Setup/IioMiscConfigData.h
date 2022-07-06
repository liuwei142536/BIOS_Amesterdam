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
  IioMiscConfigData.h
    
Abstract:
  Data format for IIO Misc Data Structure

--*/


//typedef struct {
    UINT8                   ValidationIohMiscSsWa;
    UINT8                   Poison;
    UINT8                   Viral;
    //Do not change order of IoApic BYTES or insert items between them...
    UINT8                   DevPresIoApicIio0;
    UINT8                   DevPresIoApicIio1;
    UINT8                   DevPresIoApicIio2;
    UINT8                   DevPresIoApicIio3;
    UINT8                   Ck410bConfigSpreadSpectrumEnable; 
    UINT8                   EVMode;
//} MISC_CONFIG;


