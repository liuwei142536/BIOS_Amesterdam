/**

Copyright (c) 2009-2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

  @file Cpgc.h

  Memory Initialization Module.

**/

#ifndef _CPGCCHIP_H
#define _CPGCCHIP_H

//
// CPGC Init modes
//
#define Idle_Mode                           0
#define CPGC_Testing_Mode                   1
#define MRS_Mode                            2
#define NOP_Mode                            3


#define CPGC_ERR_MONITOR_ALL_CL        0xFF // monitor errors on all cachelines
#define CPGC_ERR_MONITOR_NO_CL         0x0  // do not monitor errors on any cacheline
#define CPGC_ERR_MONITOR_ALL_CHUNK     0xFF // monitor errors on all chunks
#define CPGC_ERR_MONITOR_NO_CHUNK      0x0  // do not monitor errors on any chunk
#define CPGC_ERR_NEVER_STOP_ON_ERR     0x0  // Prevent any err from causing a test to stop
#define CPGC_ERR_STOP_ON_ALL_ERR       0x3  // If all data lanes see an err (Data_ Err_Status and ECC_Err_Status) then a test is stopped.
//
// Pattern Buffer Mux Data
//
#define DDR_CPGC_PATBUF_MUX0 0xAAAAAA
#define DDR_CPGC_PATBUF_MUX1 0xCCCCCC
#define DDR_CPGC_PATBUF_MUX2 0xF0F0F0
#define DDR_CPGC_PATBUF_MUX3 0xFFFFFF

void
CpgcSetupWdb (
  PSYSHOST                                      host,
  UINT8                                         socket,
  UINT8                                         ch,
  UINT8                                         wdbIncrRate,
  UINT8                                         wdbIncrScale,
  UINT8                                         wdbStartPntr,
  UINT8                                         wdbEndPntr,
  UINT8                                         setupPbwr,
  UINT8                                         muxCtl,
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT         seqBaincCtl00,
  CPGC_SEQ_BAINCCTL10_MC_MAINEXT_HSX_BDX_STRUCT     seqBaincCtl10, 
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT  seqBaseAddrStartLo0,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT  seqBaseAddrStartHi0,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT   seqBaseAddrWrapLo0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT   seqBaseAddrWrapHi0,
  CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT            cpgcSeqBaoCic0
  );

VOID
CpgcSetupAddrGen (
  PSYSHOST                                      host,
  UINT8                                         socket,
  UINT8                                         ch,
  CPGC_SEQ_BAINCCTL00_MC_MAINEXT_STRUCT         seqBaincCtl00,
  CPGC_SEQ_BAINCCTL10_MC_MAINEXT_HSX_BDX_STRUCT     seqBaincCtl10, 
  CPGC_SEQ_BASEADDRSTART_LO0_MC_MAINEXT_STRUCT  seqBaseAddrStartLo0,
  CPGC_SEQ_BASEADDRSTART_HI0_MC_MAINEXT_STRUCT  seqBaseAddrStartHi0,
  CPGC_SEQ_BASEADDRWRAP_LO0_MC_MAINEXT_STRUCT   seqBaseAddrWrapLo0,
  CPGC_SEQ_BASEADDRWRAP_HI0_MC_MAINEXT_STRUCT   seqBaseAddrWrapHi0,
  CPGC_SEQ_BAOCIC0_MC_MAINEXT_STRUCT            cpgcSeqBaoCic0
  );


#endif