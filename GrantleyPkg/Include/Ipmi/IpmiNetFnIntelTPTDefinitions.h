//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//
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
  IpmiNetFnIntelTPTDefinitions.h
  
Abstract:
  The file has all the definitions required for Intel TCP-IP Pass through commands
--*/

#ifndef _IPMINETFNINTELTPTDEFINITIONS_H_
#define _IPMINETFNINTELTPTDEFINITIONS_H_

//
// Net function definition for Intel TCP-IP Pass through command
//
#define EFI_SM_NETFN_INTEL_TPT  0x34

/*----------------------------------------------------------------------------------------
    Definitions for Get TPT Status command  
----------------------------------------------------------------------------------------*/
#define EFI_TPT_GET_TPT_STATUS  0x10

//
//  Constants and Structure definitions for "Get TPT Status" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Get TPT Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TPT_GET_TPT_CONFIGURATION_PARAMETERS  0x12

//
//  Constants and Structure definitions for "Get TPT Configuration Parameters" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Set TPT Configuration Parameters command    
----------------------------------------------------------------------------------------*/
#define EFI_TPT_SET_TPT_CONFIGURATION_PARAMETERS  0x13

//
//  Constants and Structure definitions for "Set TPT Configuration Parameters" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Forwarded Event Message Configuration command   
----------------------------------------------------------------------------------------*/
#define EFI_TPT_EVENT_MSG_CONFIGURATION 0x1A

//
//  Constants and Structure definitions for "Forwarded Event Message Configuration" command to follow here
//

/*----------------------------------------------------------------------------------------
    Definitions for Enable FML Network Interface command    
----------------------------------------------------------------------------------------*/
#define EFI_TPT_ENABLE_FML_NETWORK_INTERFACE  0x1B

//
//  Constants and Structure definitions for "Enable FML Network Interface" command to follow here
//

#endif
