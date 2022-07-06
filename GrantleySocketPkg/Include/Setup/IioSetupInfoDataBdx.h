//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/*++

Copyright (c)  1999 - 2016 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  IioSetupInfoDataBdx.h
    
Abstract:
  Contains new additions added for Refresh programme

--*/

/* NOTE :-
    It is developer's responsibility to ensure that,
    [1] Ordering is NOT changed - we might have QPIRC options interleaved with MRC- options. That's okay
    [2] If you need to remove a knob, add a reserved field in place of older knob variable
    [3] Add a tag at end of variable declaration to allow easy isolation of whose the knob is. 
*/


#ifdef SDP_FLAG
  UINT8    SvSnoopMode;    // QPIRC-
  UINT8    SvHitMeEn;           // QPIRC-
  UINT8    SvIoDcEn;             // QPIRC-
#endif // SDP_FLAG
  UINT8	   HomeDirWOSBEn; 	  // 1 - Enable; 0 - Disable  - QPIRC

#ifdef DE_SKU
  UINT8   ADROnReset; // Enable ADR On Reset used by SoftADR driver to enable ADR flow on reset.
#endif
  //VCCU
  UINT8    VCCUMaxOcRatio;							//CPU
  UINT8    VCCUVoltageMode;                         //CPU
  UINT16   VCCUVoltageOverride;						//CPU
  UINT16   VCCUVoltageOffset;						//CPU
  UINT8    VCCUVoltageOffsetPrefix;					//CPU
  UINT16   VCCUExtraTurboVoltage;					//CPU
  UINT8	   AVX2Offset;
  UINT8	   SfrTrim;
  UINT8    TrainingResultOffsetFunctionEnable;	    //MRC
  UINT16   OffsetTxDq; //Dq Write						//MRC
  UINT16   OffsetRxDq; //Dq Read						//MRC
  UINT16   OffsetTxVref;							//MRC
  UINT16   OffsetRxVref;							//MRC
  UINT16   OffsetCmdAll;							//MRC
  UINT16   OffsetCmdVref;							//MRC
  UINT16   OffsetCtlAll;							//MRC
  UINT8    ProcessorIOTGEnable;						//CPU
  UINT8	   ReservedDoNotDelete;					    // For migration purpose. 

  UINT8    DDR4Platform;  //MRC
#ifdef DE_SKU
  UINT8    DE_NS_SKU_ID;                             //IIO
  UINT8    Disable_SC_CPM;                           // IIO - Disable the complete SC CPM device
  UINT8    Disable_SC_GbE1;                          // IIO - Disable the complete SC GbE1 device
  UINT8    SC_GbE1_PF0;                              // Force Enable / Disable SC GbE1 physical function 0
  UINT8    SC_GbE1_PF1;                              // Force Enable / Disable SC GbE1 physical function 1  
#endif  
#ifndef DE_SKU
  UINT8    ReservedDoNotDelete2;                    // For migration purpose. 
#endif

