/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// This file contains an 'Intel Peripheral Driver' and is
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may
// be modified by the user, subject to additional terms of the
// license agreement
//

Copyright (c) 2009 - 2012 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  OemErrorHandler.h

Abstract:
  This file will contain all definitions related to PCH Error Handler
  
------------------------------------------------------------------*/
	
#ifndef _PCH_ERROR_HANDLER_H
#define _PCH_ERROR_HANDLER_H

#define R_PCH_RCRB_AECH                           0x2080      // Advanced Error Reporting Capabilities header (rsvd)
#define R_PCH_RCRB_UES                            0x2084      // Uncorrectable Error Status
#define R_PCH_RCRB_UEM                            0x2088      // Uncorrectable Error Mask
#define R_PCH_RCRB_UEV                            0x208c      // Uncorrectable Error Severity
#define B_PCH_RCRB_UE_TE                            BIT0    // Training error
#define B_PCH_RCRB_UE_DLPE                          BIT4    // Data Link Protocol Error
#define B_PCH_RCRB_UE_PT                            BIT12   // Poisoned TLP
#define B_PCH_RCRB_UE_FCPE                          BIT13   // Flow Control Protocol Error
#define B_PCH_RCRB_UE_CT                            BIT14   // Completion Timeout
#define B_PCH_RCRB_UE_CA                            BIT15   // Completer Abort
#define B_PCH_RCRB_UE_UC                            BIT16   // Unexpected Completion
#define B_PCH_RCRB_UE_RO                            BIT17   // Receiver Overflow
#define B_PCH_RCRB_UE_MT                            BIT18   // Malformed TLP
#define B_PCH_RCRB_UE_EE                            BIT19   // ECRC Error
#define B_PCH_RCRB_UE_URE                           BIT20   // Unsupported Request Error 
#define R_PCH_RCRB_CES                            0x2090      // Correctable Error Status
#define R_PCH_RCRB_CEM                            0x2094      // Correctable Error Mask
#define B_PCH_RCRB_CE_RE                            BIT0    // Indicates a receiver error
#define B_PCH_RCRB_CE_BT                            BIT6    // Bad TLP
#define B_PCH_RCRB_CE_BD                            BIT7    // Bad DLLP
#define B_PCH_RCRB_CE_RNR                           BIT8    // Replay Number Rollover
#undef  R_PCH_RCRB_REC
#define R_PCH_RCRB_REC                            0x20AC      // Root error command (rsvd)
#define R_PCH_RCRB_RES                            0x20B0      // Root error status 
#define B_PCH_RCRB_RES_CR                           BIT0
#define B_PCH_RCRB_RES_ENR                          BIT2
#define B_PCH_PCI_PCI_BPC_PSE                       BIT6
#define R_PCH_PCI_PCI_BPC                           0x4C
#define B_PCH_PCI_PCI_BPC_RTAE                      BIT0



VOID
ClearPCHErrors(
 VOID
 );

EFI_STATUS
ProcessPCHUncorrectedError (
VOID
  );

#endif


