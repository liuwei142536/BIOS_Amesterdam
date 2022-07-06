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
  IioUniveralData.h
    
Abstract:
  Data format for Universal Data Structure

--*/


/*  Due to the constraints of VFRCOMPILE that will not allow for structures within the setup data, the layout
    of this file must be a byte-4-byte match to the Ioh_SETUP_INFO structure defined in the IohUniversalDataFile!
*/

//typedef struct {
    UINT8       VariableTag;         // do not move this tag!!
//    IIO_CONFIG          IohConfig;
    #include    "IioConfigData.h"
//    COMMONRC_CONFIG          IohCsiConfig;
    #include    "IioCommonRcConfigData.h"
//    CSI_CONFIG          IohCsiConfig;
    #include    "IioCsiConfigData.h"
//    MEMORY_CONFIG       IohMemoryConfig;
    #include    "IioMemoryConfigData.h"
//    MISC_CONFIG         IohMiscConfig;
    #include    "IioMiscConfigData.h"
//} SETUP_INFO;


