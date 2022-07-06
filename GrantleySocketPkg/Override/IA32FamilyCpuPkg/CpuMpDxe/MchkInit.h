//
// This file contains an 'Intel Peripheral Driver' and is      
// licensed for Intel CPUs and chipsets under the terms of your
// license agreement with Intel or your vendor.  This file may 
// be modified by the user, subject to additional terms of the 
// license agreement                                           
//
/** @file

  Include file for machine check initialization

  Copyright (c) 2006 - 2014, Intel Corporation. All rights reserved.<BR>
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  Module Name:  MchkInit.h

**/

#ifndef _EFI_MCHK_INIT_H_
#define _EFI_MCHK_INIT_H_

#include <Token.h> // Aptio V Server Override
#include "Cpu.h"
#include "MpCommon.h"
#include <SysHost.h>

typedef struct {
  BOOLEAN   Mca;
  BOOLEAN   ControlMsrPresent;
  UINTN     NumberOfErrorReportingBank;
  struct mcBankStruct McReg[4];
} MACHINE_CHECK_ATTRIBUTE;

#define MCI_REG_BASE    0x400

#define IFU_CTL      (MCI_REG_BASE + 0x00)
#define DCU_CTL      (MCI_REG_BASE + 0x04)
#define DTLB_CTL     (MCI_REG_BASE + 0x08)
#define MLC_CTL      (MCI_REG_BASE + 0x0C)

MACHINE_CHECK_ATTRIBUTE MachineCheckAttribute[FixedPcdGet32(PcdCpuMaxLogicalProcessorNumber)];

/**
  Detect capability of machine check for specified processor.
  
  This function detects capability of machine check for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
MachineCheckDetect (
  UINTN   ProcessorNumber
  );

/**
  Configures Processor Feature Lists for machine check for all processors.
  
  This function configures Processor Feature Lists for machine check for all processors.

**/
VOID
MachineCheckConfigFeatureList (
  VOID
  );

/**
  Produces entry of machine check in Register Table for specified processor.
  
  This function produces entry of machine check in Register Table for specified processor.

  @param  ProcessorNumber   The handle number of specified processor.
  @param  Attribute         Machine check Attribute.

**/
VOID
MachineCheckReg (
  UINTN      ProcessorNumber,
  VOID       *Attribute
  );

#endif
