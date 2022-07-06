/**
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your   
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c) 2011 - 2019 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  PfatServices.c

@brief:

  PFAT Driver implements the PFAT Host Controller Compatibility Interface.

**/

#include "PfatServices.h"
#include <Guid/PlatformInfo.h>

///
/// Global variables
///
EFI_SMM_SYSTEM_TABLE2               *mSmst;
PFAT_PROTOCOL                       *mPfatProtocol;
PFAT_HOB                            *mPfatHobPtr;
EFI_SMM_IO_TRAP_DISPATCH2_PROTOCOL  *mPchIoTrap;
static EFI_ACPI_SUPPORT_PROTOCOL    *mAcpiSupport = NULL;
static EFI_ACPI_TABLE_PROTOCOL      *mAcpiTable   = NULL;


EFI_STATUS
PfatUpdateBios (
  IN  EFI_HANDLE        DispatchHandle,
  IN CONST VOID         *Context         OPTIONAL,
  IN OUT VOID           *CommBuffer      OPTIONAL,
  IN OUT UINTN          *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:

  This function is triggered by the BIOS update tool with an IO trap. It executres
  Pfat protocol execute with the true flag indicating that there is an update package
  in the Pfat reserved region of memory.

Arguments:

  DispatchHandle      - Not used
  Context             - Not used
  CommBuffer          - Not used
  CommBufferSize      - Not used
  
Returns: 

  None.

--*/
{
  //
  // Invoke PFAT Services for updating BIOS
  //
  mPfatProtocol->Execute(mPfatProtocol, TRUE);

  ///
  /// Based on the interface defined for Tools implementation, the status from the IO Trap needs to be 
  /// placed at the address of the PUP, essentially overwriting the header. 
  /// The PUP Header, along with the Script, Data and PUP Certificate are written by the tools into DPR memory, 
  /// therefore, we can safely overwrite the information in that address after execution as the next time tools does an 
  /// update operation, it will pass the complete package providing the proper PUP Header
  ///
  CopyMem (mPfatUpdatePackagePtr, &mPfatFullStatus, sizeof (UINT64));

  return EFI_SUCCESS;
}

EFI_STATUS
InitializeAslUpdateLib (
  VOID
  )
/*++

Routine Description:

  Initialize the ASL update library state.
  This must be called prior to invoking other library functions. 

Arguments:

  None
  
Returns: 

  EFI_SUCCESS             The function completed successfully.

--*/
{
  EFI_STATUS  Status;

  //
  // Locate ACPI tables
  //
  Status = gBS->LocateProtocol (&gEfiAcpiSupportProtocolGuid, NULL, (VOID **) &mAcpiSupport);
  ASSERT_EFI_ERROR (Status);
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);
  return EFI_SUCCESS;
}

EFI_STATUS
PfatUpdateAslCode (
  IN     UINT32                   AslSignature,
  IN     UINT16                   BaseAddress,
  IN     UINT8                    Length
  )
/*++

Routine Description:

  This procedure will update the operating region base address and length for IO Trap. 

Arguments:

  AslSignature         - The signature of Operation Region that we want to update.
  BaseAddress          - Base address of IO trap.
  Length               - Length of IO address.
  
Returns: 

  EFI_SUCCESS             The function completed successfully.

--*/
{
  EFI_STATUS                  Status;
  EFI_ACPI_DESCRIPTION_HEADER *Table;
  EFI_ACPI_TABLE_VERSION      Version;
  UINT8                       *CurrPtr;
  UINT8                       *Operation;
  UINT32                      *Signature;
  UINT8                       *DsdtPointer;
  INTN                        Index;
  UINTN                       Handle;
  UINT16                      AslLength;

  ///
  /// Locate table with matching ID
  ///
  Index     = 0;
  AslLength = 0;
  do {
    Status = mAcpiSupport->GetAcpiTable (mAcpiSupport, Index, (VOID **) &Table, &Version, &Handle);
    if (Status == EFI_NOT_FOUND) {
      break;
    }

    ASSERT_EFI_ERROR (Status);
    Index++;
  } while (Table->Signature != EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE);

  ///
  /// Fix up the following ASL Code in DSDT:
  /// (1) OperationRegion's IO Base Address and Length.
  /// (2) Resource Consumption in LPC Device.
  ///
  CurrPtr = (UINT8 *) Table;

  ///
  /// Loop through the ASL looking for values that we must fix up.
  ///
  for (DsdtPointer = CurrPtr; DsdtPointer < (CurrPtr + ((EFI_ACPI_COMMON_HEADER *) CurrPtr)->Length); DsdtPointer++) {
    ///
    /// Get a pointer to compare for signature
    ///
    Signature = (UINT32 *) DsdtPointer;

    ///
    /// Check if this is the signature we are looking for
    ///
    if ((*Signature) == AslSignature) {
      ///
      /// Conditional match.  For Region Objects, the Operator will always be the
      /// byte immediately before the specific name.  Therefore, subtract 1 to check
      /// the Operator.
      ///
      Operation = DsdtPointer - 1;

      ///
      /// If we have an operation region, update the base address and length
      ///
      if (*Operation == AML_OPREGION_OP) {
        ///
        /// Fixup the Base Address in OperationRegion.
        ///
        *(UINT16 *) (DsdtPointer + 6) = BaseAddress;

        ///
        /// Fixup the Length in OperationRegion.
        ///
        *(DsdtPointer + 9) = Length;
      }

    }
  }
  ///
  /// Update the modified ACPI table
  ///
  Status = mAcpiTable->InstallAcpiTable (
                                  mAcpiTable,
                                  Table,
                                  Table->Length,
                                  &Handle
                                  );
  FreePool (Table);

  return EFI_SUCCESS;
}

EFI_STATUS
PfatToolsInterfaceInit (
  IN  EFI_HANDLE          DispatchHandle,
  IN CONST VOID           *Context         OPTIONAL,
  IN OUT VOID             *CommBuffer      OPTIONAL,
  IN OUT UINTN            *CommBufferSize  OPTIONAL
  )
/*++

Routine Description:

  This method registers and sets up the IOTRAP and NVS area for the PFAT tools interface 

Arguments:

  DispatchHandle      - Not used
  Context             - Not used
  CommBuffer          - Not used
  CommBufferSize      - Not used
  
Returns: 

  None

--*/
{
  EFI_STATUS                                 Status;
  EFI_GLOBAL_NVS_AREA_PROTOCOL               *GlobalNvsAreaProtocol;
  BIOS_ACPI_PARAM                            *GlobalNvsArea;
  EFI_HANDLE                                 PchIoTrapHandle;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT           PchIoTrapContext;

  ///
  /// Locate Global NVS and update PFAT DPR size & Memory address for ACPI tables
  ///
  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, &GlobalNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  GlobalNvsArea                     = GlobalNvsAreaProtocol->Area;
  GlobalNvsArea->PfatMemAddress     = 0;
  GlobalNvsArea->PfatMemSize        = 0;
  GlobalNvsArea->PfatIoTrapAddress  = 0;
  ///
  /// Locate PFAT SMM Protocol
  ///
  //AptioV server override start: Pfat function implement for Grantley workstation.
  //-Status = mSmst->SmmLocateProtocol (&gSmmPfatProtocolGuid, NULL, &mPfatProtocol);
  Status = gBS->LocateProtocol (&gSmmPfatProtocolGuid, NULL, (VOID **) &mPfatProtocol);
  //AptioV server override end: Pfat function implement for Grantley workstation.
  ASSERT_EFI_ERROR (Status);
  if(mPfatProtocol != NULL)
  {
    ///
    /// Register PFAT IO TRAP handler
    ///
    PchIoTrapHandle               = NULL;
    PchIoTrapContext.Type         = ReadWriteTrap;
    PchIoTrapContext.Length       = 4;
    PchIoTrapContext.Address      = 0;
    Status = mPchIoTrap->Register (
                          mPchIoTrap,
                          PfatUpdateBios,
                          &PchIoTrapContext,
                          &PchIoTrapHandle
                          );
    ASSERT_EFI_ERROR (Status);
    ///
    /// Initialize ASL manipulation library
    ///
    InitializeAslUpdateLib ();
    Status = PfatUpdateAslCode (
               (EFI_SIGNATURE_32 ('I', 'O', '_', 'P')),
               PchIoTrapContext.Address,
               (UINT8) PchIoTrapContext.Length
             );
    ASSERT_EFI_ERROR (Status);
    GlobalNvsArea->PfatMemAddress     = mPfatMemAddress;
    GlobalNvsArea->PfatMemSize        = (UINT8) RShiftU64(mPfatMemSize, 20);
    GlobalNvsArea->PfatIoTrapAddress  = PchIoTrapContext.Address;
  }
  
  return Status;
}

EFI_STATUS
EFIAPI
InstallPfatProtocol (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
/*++

Routine Description:

  Entry point for the PFAT protocol driver.

Arguments:

  ImageHandle     - Image handle of this driver.
  SystemTable     - Global system service table.
  
Returns: 

  EFI_SUCCESS           - Initialization complete.
  EFI_OUT_OF_RESOURCES  - Do not have enough resources to initialize the driver.
  EFI_ERROR             - Driver exits abnormally.

--*/
{
  EFI_STATUS                                 Status;
  EFI_SMM_BASE2_PROTOCOL                     *SmmBase;
  PFAT_INSTANCE                              *PfatInstance;
  VOID                                       *PfatProtocolAddr;
  EFI_HANDLE                                 PchIoTrapHandle;
  EFI_SMM_IO_TRAP_REGISTER_CONTEXT           PchIoTrapContext;
  UINT64                                     BaseAddress; //AptioV server override: Pfat function implement for Grantley workstation.
  ///
  /// Locate SMM Base Protocol
  ///
  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &SmmBase);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize our module variables
  ///
  Status = SmmBase->GetSmstLocation (SmmBase, &mSmst);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Allocate pool for PFAT protocol instance
  ///
  Status = mSmst->SmmAllocatePool (
                  EfiRuntimeServicesData,
                  sizeof (PFAT_INSTANCE),
                  (VOID **) &PfatInstance
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (PfatInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((VOID *) PfatInstance, sizeof (PFAT_INSTANCE));
  PfatInstance->Handle = NULL;
  PfatProtocolAddr     = NULL;

  if ((AsmReadMsr64 (EFI_PLATFORM_INFORMATION)) & B_MSR_PLATFORM_INFO_PFAT_AVAIL) {
    if ((AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL)) & B_MSR_PLAT_FRMW_PROT_CTRL_EN) {
      ///
      /// Initialize the PFAT protocol instance
      ///
      Status = PfatProtocolConstructor (PfatInstance);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      PfatProtocolAddr = &(PfatInstance->PfatProtocol);
      Status = mSmst->SmmLocateProtocol (&gEfiSmmIoTrapDispatch2ProtocolGuid, NULL, &mPchIoTrap);
      ASSERT_EFI_ERROR (Status);
//AptioV server override start: Pfat function implement for Grantley workstation.
      BaseAddress = 0x1050;
      Status = gDS->AllocateIoSpace (
                      EfiGcdAllocateMaxAddressSearchBottomUp,
                      EfiGcdIoTypeIo,
                      1,
                      0x10,
                      &BaseAddress,
                      ImageHandle,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);
//AptioV server override end: Pfat function implement for Grantley workstation.
      PchIoTrapHandle               = NULL;
      PchIoTrapContext.Type         = ReadTrap;
      PchIoTrapContext.Length       = 4;
	  //AptioV server override start: Pfat function implement for Grantley workstation.
      //-PchIoTrapContext.Address      = 0;
	  PchIoTrapContext.Address      = (UINT16)BaseAddress;
	  //AptioV server override end: Pfat function implement for Grantley workstation.
      Status = mPchIoTrap->Register (
                            mPchIoTrap,
                            PfatToolsInterfaceInit,
                            &PchIoTrapContext,
                            &PchIoTrapHandle
                            );
      ASSERT_EFI_ERROR (Status);

      mPfatHobPtr->PfatToolsIntIoTrapAdd = (UINT64) PchIoTrapContext.Address;
    } else {
      DEBUG ((EFI_D_INFO, "PFAT Feature supported but disabled\n"));
    }
  } else {
    DEBUG ((EFI_D_WARN, "PFAT Feature is not supported\n"));
  }
  
  //AptioV server override start: Pfat function implement for Grantley workstation.
  ///
  /// Install the SMM PFAT_PROTOCOL interface
  ///   
  //-Status = mSmst->SmmInstallProtocolInterface (
  //-                &(PfatInstance->Handle),
  //-                &gSmmPfatProtocolGuid,
  //-                EFI_NATIVE_INTERFACE,
  //-                PfatProtocolAddr
  //-                );
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(PfatInstance->Handle),
                  &gSmmPfatProtocolGuid,
                  PfatProtocolAddr,
                  NULL
                  );
  //AptioV server override end: Pfat function implement for Grantley workstation.				  
  if (EFI_ERROR (Status)) {
    mSmst->SmmFreePool (PfatInstance);
  }

  return Status;
}

EFI_STATUS
PfatProtocolConstructor (
  PFAT_INSTANCE          *PfatInstance
  )
/*++

Routine Description:

  Initialize PFAT protocol instance.

Arguments:

  PfatInstance      - Pointer to PfatInstance to initialize
  
Returns: 

  EFI_SUCCESS       - The protocol instance was properly initialized
  EFI_NOT_FOUND     - PFAT Binary module was not found.

--*/
{
  EFI_STATUS                    Status;
  PPDT                          *Ppdt;
  UINTN                         i;
  UINTN                         NumHandles;
  EFI_HANDLE                    *Buffer;
  UINTN                         Size;
  UINT32                        FvStatus;
  EFI_FV_FILETYPE               FileType;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  UINTN                         NumPages;
  EFI_PHYSICAL_ADDRESS          Addr;
  EFI_PHYSICAL_ADDRESS          PfatModule;
  EFI_HOB_GUID_TYPE             *GuidHob;
  EFI_PLATFORM_INFO             *PlatformInfoHobPtr;
  EFI_GUID                      PfatModuleGuid  = PFAT_MODULE_GUID;
  EFI_GUID                      PfatHobGuid     = PFAT_HOB_GUID;

  FwVol           = NULL;
  FvStatus        = 0;
  NumPages        = 0;
  Addr            = 0;
  Size = 0;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumHandles,
                  &Buffer
                  );
  ASSERT_EFI_ERROR (Status);

  for (i = 0; i < NumHandles; i++) {
    Status = gBS->HandleProtocol (
                    Buffer[i],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);

    ///
    /// Locate PFAT Binary.
    ///
    Status = FwVol->ReadFile (
                    FwVol,
                    &PfatModuleGuid,
                    NULL,
                    &Size,
                    &FileType,
                    &Attributes,
                    &FvStatus
                    );
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  FreePool (Buffer);
  ASSERT (Size);
  if ((FwVol == NULL) || (Size == 0)) {
    return EFI_NOT_FOUND;
  }
  NumPages = PFAT_MEMORY_PAGES + ALIGNMENT_IN_PAGES;

  ///
  /// Allocate memory buffer for PFAT Module
  ///
  Status = (mSmst->SmmAllocatePages)(AllocateAnyPages, EfiRuntimeServicesData, NumPages, &Addr);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Align address to 256K.
  ///
  PfatModule  = Addr &~(ALIGN_256KB - 1);
  PfatModule  = PfatModule < Addr ? (PfatModule + ALIGN_256KB) : PfatModule;

  ///
  /// Read PFAT Module into prepared buffer.
  ///
  Status = FwVol->ReadFile (
                  FwVol,
                  &PfatModuleGuid,
                  ((VOID **) &PfatModule),
                  &Size,
                  &FileType,
                  &Attributes,
                  &FvStatus
                  );
  ASSERT (Size);
  if ((FwVol == NULL) || (Size == 0)) {
    return EFI_NOT_FOUND;
  }

  mPfatHobPtr = GetFirstGuidHob (&PfatHobGuid);
  if (mPfatHobPtr == NULL) {
    DEBUG ((EFI_D_ERROR, "PFAT HOB not available\n"));
    return EFI_NOT_FOUND;
  }

  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfoHobPtr = GET_GUID_HOB_DATA (GuidHob);
  if (PlatformInfoHobPtr == NULL) {
    DEBUG ((EFI_D_ERROR, "Platform Info HOB not available\n"));
    return EFI_NOT_FOUND;
  }  
  

  ///
  /// Allocate pool for PPDT Data
  ///
  Status = mSmst->SmmAllocatePool (
                  EfiRuntimeServicesData,
                  mPfatHobPtr->Ppdt.PpdtSize,
                  (VOID **) &Ppdt
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyMem (Ppdt, &mPfatHobPtr->Ppdt, mPfatHobPtr->Ppdt.PpdtSize);

  ///
  /// Allocate PFAT variables in DPR
  ///

  ///
  /// First, initialize the PFAT memory address in the pre-allocated space within DPR
  ///
  mPfatMemAddress        = PlatformInfoHobPtr->PfatPhysBase;
  mPfatMemSize           = (UINT32) LShiftU64(PlatformInfoHobPtr->PfatMemSize, 20);

  ///
  /// The PUP uses the majority of the space within the allocated region and uses the initial block of memory
  ///
  mPfatUpdatePackagePtr  = (PUP *) mPfatMemAddress;

  ///
  /// A subset of the memory is allotted for the space required for the PFAT certificate after the PUP
  ///
  mPupCertificate        = (EFI_PHYSICAL_ADDRESS) (mPfatMemAddress + mPfatMemSize - PUPC_MEMORY_OFFSET);

  ///
  /// A final allocation is made for the PFAT Log at the end of the buffer after the certificate
  ///
  mPfatLogPtr            = (PFAT_LOG *) (mPfatMemAddress + mPfatMemSize - PFAT_LOG_MEMORY_OFFSET);

  ///
  /// Save the Log & PUP headers into a temporary location so it can be used for re-initialization of the log between PfatProtocolExecute calls
  ///
  CopyMem (&mPfatLogTemp, &mPfatHobPtr->PfatLog, sizeof (PFAT_LOG));
  CopyMem (&mPfatPupHeaderTemp, &mPfatHobPtr->PupHeader, sizeof (PUP_HEADER));

  ///
  /// Initialize the PFAT Update Package with the package header and zero out the rest of the buffer
  ///
  ZeroMem (mPfatUpdatePackagePtr->PupBuffer, PUP_BUFFER_SIZE);
  CopyMem (&mPfatUpdatePackagePtr->PupHeader, &mPfatHobPtr->PupHeader, sizeof (PUP_HEADER));
  mPfatUpdatePackagePtr->PupHeader.ScriptSectionSize  = 0;
  mPfatUpdatePackagePtr->PupHeader.DataSectionSize    = 0;
  mPfatUpdateCounter = 0;

  ///
  /// Set Begin command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = PFAT_COMMAND_BEGIN;

  ///
  /// Initialize the PFAT protocol instance
  ///
  PfatInstance->Signature             = PFAT_SIGNATURE;
  PfatInstance->PfatProtocol.Write    = PfatProtocolWrite;
  PfatInstance->PfatProtocol.Erase    = PfatProtocolBlockErase;
  PfatInstance->PfatProtocol.Execute  = PfatProtocolExecute;
  PfatInstance->AddrMask              = 0;
  for (i = 0; i < mPfatHobPtr->NumSpiComponents; i++) {
    PfatInstance->AddrMask += (SPI_SIZE_BASE_512KB << mPfatHobPtr->ComponentSize[i]);
  }
  PfatInstance->AddrMask -= 1;

  PfatInstance->PfatDirectory[EnumPfatModule]        = PfatModule;
  PfatInstance->PfatDirectory[EnumPfatModule]       |= LShiftU64 (PFAT_DIRECTORY_PFAT_MODULE_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPpdt]              = (EFI_PHYSICAL_ADDRESS) Ppdt;
  PfatInstance->PfatDirectory[EnumPpdt]             |= LShiftU64 (PFAT_DIRECTORY_PPDT_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPup]               = (EFI_PHYSICAL_ADDRESS) mPfatUpdatePackagePtr;
  PfatInstance->PfatDirectory[EnumPup]              |= LShiftU64(PFAT_DIRECTORY_PUP_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPupCertificate]    = 0;
  PfatInstance->PfatDirectory[EnumPupCertificate]   |= LShiftU64(PFAT_DIRECTORY_UNDEFINED_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPfatLog]           = 0;
  PfatInstance->PfatDirectory[EnumPfatLog]          |= LShiftU64(PFAT_DIRECTORY_UNDEFINED_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPfatDirectoryEnd]  = 0;
  PfatInstance->PfatDirectory[EnumPfatDirectoryEnd] |= LShiftU64 (PFAT_DIRECTORY_END_MARKER, 56);

  return EFI_SUCCESS;
}

VOID
EFIAPI
PfatModuleExecute (
  IN VOID          *PfatInstancePtr
  )
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
{
  PFAT_INSTANCE *PfatInstance;
  PfatInstance = PfatInstancePtr;

  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM, (UINT64) PfatInstance->PfatDirectory);
  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIGGER, 0);
  //AptioV server override: Pfat function implement for Grantley workstation.
  //
  // Read MSR_PLAT_FRMW_PROT_TRIG_PARAM to get PFAT Binary status
  //
  PfatInstance->MsrValue = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM);  

  return ;
}


EFI_STATUS
EFIAPI
PfatProtocolExecute (
  IN     PFAT_PROTOCOL  *This,
  IN     BOOLEAN        IoTrapBasedRequest
  )
/*++

Routine Description:

  Set PFAT DIRECTORY Address and triggers MSR to pass control to PFAT Binary module to execute command script.
  This function would be called by runtime driver, please do not use any MMIO macro here

Arguments:

  This                - Pointer to the PFAT_PROTOCOL instance.
  BiosUpdate          - Flag to indicate flash update is requested by the Tool
  
Returns: 

  EFI_SUCCESS             - Command succeed.
  EFI_INVALID_PARAMETER   - The parameters specified are not valid.
  EFI_UNSUPPORTED         - The CPU or SPI memory is not supported.
  EFI_DEVICE_ERROR        - Device error, command aborts abnormally.

--*/
{
  EFI_STATUS            Status;
  PFAT_INSTANCE         *PfatInstance;
  UINT16                PfatStatus;
  UINT16                PfatAdditionalData;
  UINT16                PfatTerminalLine;
  UINT8                 PfatSE;
  UINTN                 Index;
  UINT8                 RetryIteration;
  BOOLEAN               DisallowedUpdate;

  ///
  /// Initialize local variables
  ///
  PfatStatus         = ERR_OK;
  PfatAdditionalData = ERR_OK;
  PfatTerminalLine   = ERR_OK;
  PfatSE             = ERR_OK;
  DisallowedUpdate   = FALSE;
  Status             = EFI_SUCCESS;

  //DEBUG ((EFI_D_INFO, "PfatProtocolExecute\n")); // Removing - only needed for debug purposes

  PfatInstance = PFAT_INSTANCE_FROM_PFATPROTOCOL (This);

  ///
  /// Prior to execution of the PFAT module, reinitialize the PFAT Log area & PFAT Binary Return Status
  ///
  if (mPfatLogPtr != NULL) {
    ZeroMem(mPfatLogPtr, PFAT_LOG_MEMORY_SIZE);
    CopyMem(mPfatLogPtr, &mPfatLogTemp, sizeof(PFAT_LOG));
  }
  PfatInstance->MsrValue = ERR_LAUNCH_FAIL;
  
  if (IoTrapBasedRequest == FALSE) {
    ///
    /// If Update Package has been created by the BIOS during POST then complete the script
    /// and create update Pkg
    ///

    ///
    /// First, finalize the script by adding the "End" command
    ///
    mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++]  = PFAT_COMMAND_END;
    mPfatUpdatePackagePtr->PupHeader.ScriptSectionSize      = (mPfatUpdateCounter * 8);

    ///
    /// Copy the PFAT Update Data member variable into the PUP buffer directly after the "End" command
    ///
    CopyMem (
            &mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter],
            &mPfatUpdateData,
            (mPfatUpdatePackagePtr->PupHeader.DataSectionSize)
            );
  } else {
    ///
    /// If the Update Package was retrieved from the OS via Global NVS and IO Trap then require it to be signed
    /// Update Pup Certificate pointer to dynamically point to certificate based on image size
    ///
    mPupCertificate = (EFI_PHYSICAL_ADDRESS)(mPfatMemAddress + sizeof(PUP_HEADER) + mPfatUpdatePackagePtr->PupHeader.ScriptSectionSize + mPfatUpdatePackagePtr->PupHeader.DataSectionSize);
    if (mPfatUpdatePackagePtr->PupHeader.PkgAttributes) {
      PfatInstance->PfatDirectory[EnumPupCertificate]   = mPupCertificate;
      PfatInstance->PfatDirectory[EnumPupCertificate]  |= LShiftU64(PFAT_DIRECTORY_PUP_CERTIFICATE_ENTRY, 56);
      PfatInstance->PfatDirectory[EnumPfatLog]          = (EFI_PHYSICAL_ADDRESS) mPfatLogPtr;
      PfatInstance->PfatDirectory[EnumPfatLog]         |= LShiftU64(PFAT_DIRECTORY_PFAT_LOG_ENTRY, 56);
    } else {
      /// BIOS Updates will not be allowed to be passed through when there is no certificate required
      DisallowedUpdate = TRUE;
    }
  }

  if (!DisallowedUpdate) {
    ///
    /// Prior to launching the PFAT Binary on the BSP, each of the APs must launch the PFAT binary so that
    /// they can be accounted for and placed in sleep by the PFAT binary.
    ///
    for (Index = mSmst->NumberOfCpus - 1; Index > 0 ; Index--) {
      Status = EFI_NOT_READY;
      for (RetryIteration = 0; (RetryIteration < PFAT_AP_SAFE_RETRY_LIMIT) && (Status != EFI_SUCCESS); RetryIteration++) {
        Status = mSmst->SmmStartupThisAp (PfatModuleExecute, Index, (VOID *) PfatInstance);
        if (Status != EFI_SUCCESS) {
          ///
          /// SmmStartupThisAp might return failure if AP is busy executing some other code. Let's wait for sometime and try again.
          ///
          PchPmTimerStall (PFAT_WAIT_PERIOD);
        }
      }
      if (Status != EFI_SUCCESS) {
        break;
      }
    }

    if (Status == EFI_SUCCESS) {
      PfatModuleExecute ((VOID *) PfatInstance);
    }

    //
    // Read MSR_PLAT_FRMW_PROT_TRIG_PARAM to get PFAT Binary status
    //
    PfatInstance->MsrValue = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM);  

    PfatStatus = (UINT16) RShiftU64 (
      (PfatInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_MASK,
                                           N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET)),
      N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET
      );

    switch (PfatStatus) {
      case ERR_OK:
        Status = EFI_SUCCESS;
        break;

      case ERR_RANGE_VIOLATION:
      case ERR_SFAM_VIOLATION:
      case ERR_EXEC_LIMIT:
      case ERR_INTERNAL_ERROR:
        Status = EFI_DEVICE_ERROR;
        break;

      case ERR_UNSUPPORTED_CPU:
      case ERR_UNDEFINED_FLASH_OBJECT:
      case ERR_LAUNCH_FAIL:
        Status = EFI_UNSUPPORTED;
        break;

      default:

      case ERR_BAD_DIRECTORY:
      case ERR_BAD_PPDT:
      case ERR_BAD_PUP:
      case ERR_SCRIPT_SYNTAX:
      case ERR_INVALID_LINE:
      case ERR_BAD_PUPC:
      case ERR_BAD_SVN:
      case ERR_UNEXPECTED_OPCODE:
      case ERR_OVERFLOW:
        Status = EFI_INVALID_PARAMETER;
        break;
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  if (EFI_ERROR (Status)) {
    PfatAdditionalData = (UINT16) RShiftU64 (
                    (PfatInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_MASK,
                                                         N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET)),
                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET
                    );
    PfatTerminalLine = (UINT16) RShiftU64 (
                    (PfatInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_MASK,
                                                         N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET)),
                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET
                    );
    PfatSE = (UINT8) RShiftU64 (
                    (PfatInstance->MsrValue & B_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE),
                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE_OFFSET
                    );
    DEBUG ((EFI_D_ERROR, "PFAT Status          = 0x%X\n", PfatStatus));
    DEBUG ((EFI_D_ERROR, "PFAT Additional Data = 0x%X\n", PfatAdditionalData));
    DEBUG ((EFI_D_ERROR, "PFAT Terminal Line   = 0x%X\n", PfatTerminalLine));
    DEBUG ((EFI_D_ERROR, "PFAT SE              = 0x%X\n", PfatSE));
    DEBUG ((EFI_D_ERROR, "PFAT Disallowed Upd  = %s\n", (DisallowedUpdate)?L"TRUE":L"FALSE"));
  }

  mPfatFullStatus = PfatInstance->MsrValue;

  PfatInstance->PfatDirectory[EnumPupCertificate]     = 0;
  PfatInstance->PfatDirectory[EnumPupCertificate]     |= LShiftU64(PFAT_DIRECTORY_UNDEFINED_ENTRY, 56);
  PfatInstance->PfatDirectory[EnumPfatLog]            = 0;
  PfatInstance->PfatDirectory[EnumPfatLog] |= LShiftU64(PFAT_DIRECTORY_UNDEFINED_ENTRY, 56);
  ZeroMem(mPfatUpdatePackagePtr, mPfatMemSize);
  CopyMem (&mPfatUpdatePackagePtr->PupHeader, &mPfatPupHeaderTemp, sizeof (PUP_HEADER));
  mPfatUpdatePackagePtr->PupHeader.ScriptSectionSize  = 0;
  mPfatUpdatePackagePtr->PupHeader.DataSectionSize    = 0;
  mPfatUpdateCounter                                  = 0;

  ///
  /// Prep for the next script execution by adding the "Begin" command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = PFAT_COMMAND_BEGIN;

  
  return Status;
}

VOID
EFIAPI
PfatProtocolWrite (
  IN     PFAT_PROTOCOL  *This,
  IN     UINTN          Address,
  IN     UINT32         DataByteCount,
  IN OUT UINT8          *Buffer
  )
/*++

Routine Description:

  Fill up Write script data into the PFAT Script buffer.
  This function would be called by runtime driver, please do not use any MMIO macro here

Arguments:

  This                - Pointer to the PFAT_PROTOCOL instance.
  Address             - This value specifies the offset from the start of the SPI Flash component where
                        BIOS Image is located.
  DataByteCount       - Number of bytes in the data portion.
  Buffer              - Pointer to caller-allocated buffer containing the data to be sent.
  
Returns: 

  None

--*/
{
  PFAT_INSTANCE *PfatInstance;

  PfatInstance = PFAT_INSTANCE_FROM_PFATPROTOCOL (This);

  ///
  /// Set Buffer Offset Index immediate command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] =
          (LShiftU64 ((UINTN) mPfatUpdatePackagePtr->PupHeader.DataSectionSize, 32)) |
          (LShiftU64 (PFAT_B0_INDEX, 16)) |
          PFAT_COMMAND_SET_BUFFER_INDEX;
  ///
  /// Set Flash Index immediate command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = (LShiftU64 ((Address & PfatInstance->AddrMask), 32)) | (LShiftU64 (PFAT_F0_INDEX, 16)) | PFAT_COMMAND_SET_FLASH_INDEX;
  ///
  /// Write to Flash Index from Buffer Offset Index with specific Size command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = (LShiftU64 (DataByteCount, 32)) | (LShiftU64 (PFAT_B0_INDEX, 24)) | (LShiftU64 (PFAT_F0_INDEX, 16)) | PFAT_COMMAND_WRITE_IMM;

  ///
  /// Place the data from the caller into the global PFAT Update data
  ///
  CopyMem (&mPfatUpdateData[mPfatUpdatePackagePtr->PupHeader.DataSectionSize], Buffer, DataByteCount);

  ///
  /// Update the size of the data section to match the input data size
  ///
  mPfatUpdatePackagePtr->PupHeader.DataSectionSize += DataByteCount;

  return;
}

VOID
EFIAPI
PfatProtocolBlockErase (
  IN     PFAT_PROTOCOL  *This,
  IN     UINTN          Address
  )
/*++

Routine Description:

  Fill up Erase script data into the PFAT Script buffer.
  This function would be called by runtime driver, please do not use any MMIO macro here

Arguments:
  This                - Pointer to the PFAT_PROTOCOL instance.
  Address             - This value specifies the offset from the start of the SPI Flash component where
                        BIOS Image is located.

Returns: 
  None

--*/
{
  PFAT_INSTANCE *PfatInstance;

  PfatInstance = PFAT_INSTANCE_FROM_PFATPROTOCOL (This);

  ///
  /// Set Flash Index immediate command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = (LShiftU64 ((Address & PfatInstance->AddrMask), 32)) | (LShiftU64 (PFAT_F0_INDEX, 16)) | PFAT_COMMAND_SET_FLASH_INDEX;
  ///
  /// Erase Flash Index command
  ///
  mPfatUpdatePackagePtr->PupBuffer[mPfatUpdateCounter++] = (LShiftU64 (PFAT_F0_INDEX, 16)) | PFAT_COMMAND_ERASE_BLK;

  mPfatUpdatePackagePtr->PupHeader.DataSectionSize += 0;

  return ;
}
