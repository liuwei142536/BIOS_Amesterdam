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

  QuiesceSupport.h

Abstract:

  Quiesce driver.

Revision History:

--*/

#ifndef _QUIESCE_SUPPORT_H
#define _QUIESCE_SUPPORT_H

//
// Include files
//

#define QUIESCE_CODE_SIZE (64 * 1024)
#define QUIESCE_DATA_SIZE (64 * 1024)
#define QUIESCE_STACK_SIZE (2 * 1024) 


#define EFI_MSR_IA32_MTRR_CAP                  0xFE
#define  IA32_MTRR_SMRR_SUPPORT_BIT            BIT11


typedef struct {
  EFI_PHYSICAL_ADDRESS  CodeBaseAddress; 
  UINTN                 CodeSize;
  EFI_PHYSICAL_ADDRESS  DataBaseAddress; 
  UINTN                 DataSize;
  EFI_PHYSICAL_ADDRESS  PageTableBaseAddr; 
  UINTN                 PageTableSize;
  EFI_PHYSICAL_ADDRESS  BuildDataBaseAddress; 
  UINTN                 BuildDataSize;
  UINT64                MMCfgBase;
  UINT64                MiscFeatureMsrVal;
  UINT32                AvailableBufferLegth;
  UINT32                MonarchSocketId;
  UINT32                MonarchApicId;
  volatile UINT32       QuiesceStatus4APs;
  BOOLEAN               CodeCoppied;
  BOOLEAN               SMTEnabled;     //HT Enabled
  BOOLEAN               MonarchApCheckInStatus;

} QUIESCE_COMMUNICATION_DATA_STRUCT;

 

EFI_STATUS
GetQuiesceCodeMemoryInfo (
  IN OUT UINTN *QuiesceCodeBaseAddress,
  IN OUT UINTN *QuiesceCodeSize
)
/*++

Routine Description:

  This routine returns pointer to base address and size of .
  Quiesce Code memory.

Arguments:

  QuiesceCodeBaseAddress  - Base address of Quiesce Code memory
  QuiesceCodeSize         - Quiesce Code memory size

Returns:

  EFI_STATUS       
  
--*/
;


EFI_STATUS
GetQuiesceDataMemoryInfo (
  IN OUT UINTN *QuiesceDataBaseAddress,
  IN OUT UINTN *QuiesceDataSize
)
/*++

Routine Description:

  This routine returns pointer to base address and size of .
  Quiesce Data memory.

Arguments:

  QuiesceDataBaseAddress  - Base address of Quiesce Data memory
  QuiesceDataSize         - Quiesce Data memory size

Returns:

  EFI_STATUS       
  
--*/
;


EFI_STATUS
EFIAPI
QuiesceApProc(
  VOID)
/*++

Routine Description:

  This routine is API for AP to enter Quiesce AP handler.
  
Arguments:

  None
 
Returns:

  EFI_STATUS       
  
--*/
;

EFI_STATUS
EFIAPI
AquireDataBuffer(
  IN OUT EFI_PHYSICAL_ADDRESS   *BufferAddress 
  )
/*++

Routine Description:

  This routine returns pointer to next avaialble address in
  Quiesce Data memory.
  
Arguments:

  BufferAddress       -  pointer to next avialble quiesce memory
 
Returns:

  EFI_STATUS       
  
--*/
;


EFI_STATUS
EFIAPI
ReleaseDataBuffer(
  IN EFI_PHYSICAL_ADDRESS  BaseAddress
  )
/*++

Routine Description:

  This routine release the Quiesce Data memory from the address 
  passed in.
  
Arguments:

  BaseAddress       -  Start address to be freed
 
Returns:

  EFI_STATUS       
   
--*/
;

EFI_STATUS
EFIAPI
SystemQuiesceUnQuiesce(
  BOOLEAN QuiesceFlag
  )
/*++

Routine Description:

  This routine disable MSI/VLW from IOH PCIE/Internal devices, 
  and set quiesce command.
  
Arguments:

  None
 
Returns:

  EFI_STATUS   

--*/
;

VOID
DisableIioPcieMsg(
    UINT8     ProgAll
);

VOID
RestoreIioPcieMsg(
    UINT8     ProgAll
);

EFI_STATUS
QuiesceMain(
  VOID
)
/*++

Routine Description:

  This routine is main entry point for Monarch to do system Quiesce.
  It contains main flow for Monarch to Quiesce/UnQuiesce system.

Arguments:

  None
 
Returns:

  EFI_STATUS       
  
--*/
;

EFI_STATUS
EFIAPI
MonarchAPCheckIn (
  IN BOOLEAN  CheckIn  
  );


EFI_STATUS 
EFIAPI 
QuiesceApPollingProc (
  VOID
);
/*++

Routine Description:

  This routine is API for AP to enter Quiesce AP handler and wait for BSP completion.
  
Arguments:

  None
 
Returns:

  EFI_STATUS       
  
--*/

#endif
