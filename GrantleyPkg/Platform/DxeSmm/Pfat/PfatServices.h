/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

@copyright
  Copyright (c) 2011 - 2014 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

@file
  PfatServices.h

@brief
  Header file for PFAT Driver.
**/

#ifndef _PFAT_SERVICES_H_
#define _PFAT_SERVICES_H_

#include "PfatDefinitions.h"
//AptioV server override start: dynamic MMCFG base change support.
#ifdef DYNAMIC_MMCFG_BASE_FLAG 
#include <PchSmmAccess.h> //AptioV server override: Support for Dynamic MMCFG. Do not USe PCIe PCD in SMM since PCD SMM is unavailable.
#else
#include <PchAccess.h>
#endif
//AptioV server override end: dynamic MMCFG base change support.
#include <Library/PchPlatformLib.h>

///
/// Driver Dependency Protocols
///
#include <Protocol\PfatFlashProtocol.h>
#include <Protocol\SmmBase2.h>
#include <Protocol\FirmwareVolume2.h>
#include <Protocol\SmmIoTrapDispatch2.h>
#include <Protocol\GlobalNvsArea.h>
#include <Protocol\AcpiSupport.h>
#include <Protocol\AcpiTable.h>
#include <Protocol\SmmVariable.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library\PchAslUpdateLib.h>
#include <Cpu\CpuRegs.h>
#include <Guid/SetupVariable.h>
#include <Library/DxeServicesTableLib.h> //AptioV server override: Pfat function implement for Grantley workstation.

#define EFI_SIGNATURE_16(A, B)        ((A) | (B << 8))
#define EFI_SIGNATURE_32(A, B, C, D)  (EFI_SIGNATURE_16 (A, B) | (EFI_SIGNATURE_16 (C, D) << 16))

UINT8                mPfatUpdateData[PUP_BUFFER_SIZE];
static UINT16        mPfatUpdateCounter;
PUP                  *mPfatUpdatePackagePtr;
EFI_PHYSICAL_ADDRESS mPupCertificate;
PFAT_LOG             *mPfatLogPtr;
PFAT_LOG             mPfatLogTemp;
PUP_HEADER           mPfatPupHeaderTemp;
EFI_PHYSICAL_ADDRESS mPfatMemAddress;
UINT32               mPfatMemSize;
UINT64               mPfatFullStatus;

extern EFI_GUID   gPfatModuleGuid;

///
/// Private data structure definitions for the driver
///
#define PFAT_SIGNATURE  EFI_SIGNATURE_32 ('P', 'F', 'A', 'T')

#define PFAT_DIRECTORY_MAX_SIZE               6
#define PFAT_DIRECTORY_PFAT_MODULE_ENTRY      0x00
#define PFAT_DIRECTORY_PPDT_ENTRY             0x01
#define PFAT_DIRECTORY_PUP_ENTRY              0x02
#define PFAT_DIRECTORY_PUP_CERTIFICATE_ENTRY  0x03
#define PFAT_DIRECTORY_PFAT_LOG_ENTRY         0x04
#define PFAT_DIRECTORY_UNDEFINED_ENTRY        0xFE
#define PFAT_DIRECTORY_END_MARKER             0xFF

#define AML_OPREGION_OP       0x80

typedef enum {
  EnumPfatModule        = 0,
  EnumPpdt,
  EnumPup,
  EnumPupCertificate,
  EnumPfatLog,
  EnumPfatDirectoryEnd
} PFAT_DIRECTORY;

typedef struct {
  UINTN                Signature;
  EFI_HANDLE           Handle;
  PFAT_PROTOCOL        PfatProtocol;
  EFI_PHYSICAL_ADDRESS PfatDirectory[PFAT_DIRECTORY_MAX_SIZE];
  UINT32               AddrMask;
  UINT64               MsrValue;
} PFAT_INSTANCE;

#define PFAT_INSTANCE_FROM_PFATPROTOCOL(a)  CR (a, PFAT_INSTANCE, PfatProtocol, PFAT_SIGNATURE)

///
/// Stall period in microseconds
///
#define PFAT_WAIT_PERIOD          0
#define PFAT_AP_SAFE_RETRY_LIMIT  1

/*++

Routine Description:

  Initialize PFAT protocol instance.

Arguments:

  PfatInstance      - Pointer to PfatInstance to initialize
  
Returns: 

  EFI_SUCCESS       - The protocol instance was properly initialized
  EFI_NOT_FOUND     - PFAT Binary module was not found.

--*/
EFI_STATUS
PfatProtocolConstructor (
  PFAT_INSTANCE *PfatInstance
  );

/*++

Routine Description:

  Set MSR 0x115 with PFAT DIRECTORY Address.
  Trigger MSR 0x116 to invoke PFAT Binary.
  Read MSR 0x115 to get PFAT Binary Status.

Arguments:

  PfatInstance        - Pointer to PfatInstance to initialize
  
Returns: 

  None

--*/
VOID
EFIAPI
PfatModuleExecute (
  IN VOID          *PfatInstance
  );

/*++

Routine Description:

  This service will write PFAT_DIRECTORY MSR and invoke the PFAT Module by writing to PLAT_FRMW_PROT_TRIGGER MSR for writing/erasing to flash.
  BIOS should invoke PFAT_PROTOCOL.Write() or PFAT_PROTOCOL.Erase() function prior to calling PFAT_PROTOCOL.Execute() for flash writes/erases (except for BiosUpdate).
  Write()/Erase() function will render PFAT script during execution.
  Execute() function will implement the following steps:
  1. Update PFAT directory with address of PUP.
  2. All the AP's except the master thread are put to sleep.
  3. PFAT module is invoked from BSP to execute desired operation.
  If BiosUpdate flag is set to true, PUP (PUP Header + PFAT Script + Update data) is part of data that is passed to SMI Handler. SMI Handler invokes PFAT module to process the update.
  This function would be called by runtime driver, please do not use any MMIO macro here.

Arguments:

  This           - Pointer to the PFAT_PROTOCOL instance.
  Address        - This value specifies the offset from the start of the SPI Flash component where BIOS Image is located.
  DataByteCount  - Number of bytes in the data portion.
  Buffer         - Pointer to caller-allocated buffer containing the dada sent.
  
Returns: 

  EFI_SUCCESS            - Successfully completed flash operation.
  EFI_INVALID_PARAMETER  - The parameters specified are not valid.
  EFI_UNSUPPORTED        - The CPU or SPI memory is not supported.
  EFI_DEVICE_ERROR       - Device error, command aborts abnormally.

--*/
EFI_STATUS
EFIAPI
PfatProtocolExecute (
  IN PFAT_PROTOCOL *This,
  IN BOOLEAN       BiosUpdate
  );

/*++

Routine Description:

  This service fills PFAT script buffer for flash writes.
  BIOS should invoke this function prior to calling PFAT_PROTOCOL.Execute() with all the relevant data required for flash write.
  This function will not invoke PFAT Module, only create script required for writing to flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

Arguments:

  This           - Pointer to the PFAT_PROTOCOL instance.
  Address        - This value specifies the offset from the start of the SPI Flash component where BIOS Image is located.
  DataByteCount  - Number of bytes in the data portion.
  Buffer         - Pointer to caller-allocated buffer containing the dada sent.
  
Returns: 

  None

--*/
VOID
EFIAPI
PfatProtocolWrite (
  IN PFAT_PROTOCOL *This,
  IN UINTN         Address,
  IN UINT32        DataByteCount,
  IN OUT UINT8     *Buffer
  );

/*++

Routine Description:

  This service fills PFAT script buffer for erasing blocks in flash.
  BIOS should invoke this function prior to calling PFAT_PROTOCOL.Execute() with all the relevant data required for flash erase.
  This function will not invoke PFAT module, only create script required for erasing each block in the flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

Arguments:

  This       - Pointer to the PFAT_PROTOCOL instance.
  Address    - This value specifies the offset from the start of the SPI Flash component where BIOS Image is located.
  
Returns: 

  None

--*/
VOID
EFIAPI
PfatProtocolBlockErase (
  IN PFAT_PROTOCOL *This,
  IN UINTN         Address
  );

#endif
