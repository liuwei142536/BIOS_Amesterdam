/** @file
  This driver is responsible for the registration of child drivers
  and the abstraction of the PCH SMI sources.

@copyright
  Copyright (c) 1999 - 2017 Intel Corporation. All rights reserved
  This software and associated documentation (if any) is furnished
  under a license and may only be used or copied in accordance
  with the terms of the license. Except as permitted by such
  license, no part of this software or documentation may be
  reproduced, stored in a retrieval system, or transmitted in any
  form or by any means without the express written consent of
  Intel Corporation.

  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

**/
#include "PchSmm.h"
#include "PchSmmHelpers.h"
#include <Protocol/SmmCpu.h>
#include <Protocol/SmmBase2.h>
#include "Protocol/SmmPowerButtonDispatch.h"



///
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MODULE / GLOBAL DATA
///
/// Module variables used by the both the main dispatcher and the source dispatchers
/// Declared in PchSmmSources.h
///

UINT32                mAcpiBaseAddr;
UINT32                mGpioBaseAddr;
PRIVATE_DATA          mPrivateData = {  ///< for the structure
  {
    NULL
  },                                    ///< CallbackDataBase linked list head
  NULL,                                 ///< Handler returned whan calling SmiHandlerRegister
  NULL,                                 ///< EFI handle returned when calling InstallMultipleProtocolInterfaces
  {                                     ///< protocol arrays
    /// elements within the array
    ///
    {
      PROTOCOL_SIGNATURE,
      UsbType,
      &gEfiSmmUsbDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      SxType,
      &gEfiSmmSxDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      SwType,
      &gEfiSmmSwDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) MAXIMUM_SWI_VALUE
      }
    },
    {
      PROTOCOL_SIGNATURE,
      GpiType,
      &gEfiSmmGpiDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) NUM_SUPPORTED_GPIS
      }
    },
    {
      PROTOCOL_SIGNATURE,
      IchnType,
      &gEfiSmmIchnDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      IchnExType,
      &gEfiSmmIchnDispatch2ExProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      PowerButtonType,
      &gEfiSmmPowerButtonDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister
      }
    },
    {
      PROTOCOL_SIGNATURE,
      PeriodicTimerType,
      &gEfiSmmPeriodicTimerDispatch2ProtocolGuid,
      {
        (PCH_SMM_GENERIC_REGISTER) PchSmmCoreRegister,
        (PCH_SMM_GENERIC_UNREGISTER) PchSmmCoreUnRegister,
        (UINTN) PchSmmPeriodicTimerDispatchGetNextShorterInterval
      }
    },
  }
};

CONTEXT_FUNCTIONS     mContextFunctions[PchSmmProtocolTypeMax] = {
  {
    NULL,
    NULL,
    NULL
  },
  {
    SxGetContext,
    SxCmpContext,
    NULL
  },
  {
    SwGetContext,
    SwCmpContext,
    SwGetBuffer
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    NULL,
    NULL,
    NULL
  },
  {
    PowerButtonGetContext,
    PowerButtonCmpContext,
    NULL
  },
  {
    PeriodicTimerGetContext,
    PeriodicTimerCmpContext,
    PeriodicTimerGetBuffer
  },
};

///
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PROTOTYPES
///
/// Functions use only in this file
///
EFI_STATUS
PchSmmCoreDispatcher (
  IN     EFI_HANDLE               DispatchHandle,
  IN     CONST VOID               *PchSmmCore,     OPTIONAL
  IN OUT VOID                     *CommunicationBuffer,     OPTIONAL
  IN OUT UINTN                    *BufferSize  OPTIONAL
  );

//AptioV server override: Commented out to use AMI power button handler
/*EFI_STATUS
PowerButtonCallback (
  IN  EFI_HANDLE                  DispatchHandle,
  IN CONST VOID                   *Context         OPTIONAL,
  IN OUT VOID                     *CommBuffer      OPTIONAL,
  IN OUT UINTN                    *CommBufferSize  OPTIONAL
);

EFI_STATUS
InstallPowerButtonCallback (
  void
);
*/
///
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FUNCTIONS
///
/// Driver entry point
///

/**
  Initializes the PCH SMM Dispatcher

  @param[in] ImageHandle          Pointer to the loaded image protocol for this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             PchSmmDispatcher Initialization completed.
**/
EFI_STATUS
EFIAPI
InitializePchSmmDispatcher (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  ///
  /// Access ACPI Base Addresses Register
  ///
  mAcpiBaseAddr = (UINT32) (MmioRead16 (
                              MmPciAddress (0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_LPC,
                              PCI_FUNCTION_NUMBER_PCH_LPC,
                              R_PCH_LPC_ACPI_BASE)
                              ) & B_PCH_LPC_ACPI_BASE_BAR);
  ASSERT (mAcpiBaseAddr != 0);

  ///
  /// Access GPIO Base Addresses Register
  ///
  mGpioBaseAddr = (UINT32) (MmioRead16 (
                              MmPciAddress (0,
                              DEFAULT_PCI_BUS_NUMBER_PCH,
                              PCI_DEVICE_NUMBER_PCH_LPC,
                              PCI_FUNCTION_NUMBER_PCH_LPC,
                              R_PCH_LPC_GPIO_BASE)
                              ) & B_PCH_LPC_GPIO_BASE_BAR);
  ASSERT (mGpioBaseAddr != 0);


  ///
  /// Initialize Callback DataBase
  /// Move here to avoid OEM callback failure
  ///
  InitializeListHead (&mPrivateData.CallbackDataBase);

  ///
  /// Fix the ICHN_SOURCE_DESCS base, in order to obtain independence with platform.
  ///
  PchSmmIchnFixSourceBase ();

  PchSmmPublishDispatchProtocols ();

  ///
  /// Register a callback function to handle subsequent SMIs.  This callback
  /// will be called by SmmCoreDispatcher.
  ///
  Status = gSmst->SmiHandlerRegister (PchSmmCoreDispatcher, NULL, &mPrivateData.SmiHandle);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Enable SMIs on the PCH now that we have a callback
  ///
  PchSmmInitHardware ();

  ///
  /// Install callback for power button
  ///
  //InstallPowerButtonCallback (); //AptioV server override: Commented out to use AMI power button handler

  return EFI_SUCCESS;
}

//AptioV server override: Commented out to use AMI power button handler
/*EFI_STATUS
PowerButtonCallback (
  IN  EFI_HANDLE                  DispatchHandle,
  IN CONST VOID                   *Context         OPTIONAL,
  IN OUT VOID                     *CommBuffer      OPTIONAL,
  IN OUT UINTN                    *CommBufferSize  OPTIONAL
)*/
/*++

Routine Description:

  When a power button event happens, it shuts off the machine

Arguments:

  DispatchHandle  - Handle of this dispatch function
  DispatchContext - Pointer to the dispatch function's context

Returns:

  None.

--*/
/*{
  UINT32      Buffer;
  EFI_STATUS  Status;
  UINT16      Data16;

  //
  // Clear Power Button Status
  //
  Status = gSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT16,
                                 mAcpiBaseAddr + R_PCH_ACPI_PM1_STS,
                                 1,
                                 &Data16);
  Data16 |=  B_PCH_SMI_EN_ON_SLP_EN;
  Status |= gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT16,
                                 mAcpiBaseAddr + R_PCH_ACPI_PM1_STS,
                                 1,
                                 &Data16);

  //
  // Shut it off now - As SMI_EN_ON_SLP_EN is Set, This won't shut down
  //
  Status |= gSmst->SmmIo.Io.Read (&gSmst->SmmIo, SMM_IO_UINT32,
                                 mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT,
                                 1,
                                 &Buffer);
  Buffer &= ~(B_PCH_ACPI_PM1_CNT_SLP_EN | B_PCH_ACPI_PM1_CNT_SLP_TYP);
  Buffer |= V_PCH_ACPI_PM1_CNT_S5;
  Status |= gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32,
                                 mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT,
                                 1,
                                 &Buffer);
  Buffer |= B_PCH_ACPI_PM1_CNT_SLP_EN;
  Status |= gSmst->SmmIo.Io.Write (&gSmst->SmmIo, SMM_IO_UINT32,
                                 mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT,
                                 1,
                                 &Buffer);

  return Status;
}

EFI_STATUS
InstallPowerButtonCallback (void)
{
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL    *PowerButtonDispatch;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT      PowerButtonContext;
  EFI_HANDLE                                 PowerButtonHandle = NULL;
  EFI_STATUS                                 Status;

  //
  // Locate the ICH SMM Power Button dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                                     &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                                     NULL,
                                     &PowerButtonDispatch
                                    );
  if (!EFI_ERROR (Status)) {
    //
    // Register the power button SMM event
    //
    PowerButtonContext.Phase = EfiPowerButtonEntry;
    Status = PowerButtonDispatch->Register (
                                            PowerButtonDispatch,
                                            PowerButtonCallback,
                                            (void *) &PowerButtonContext,
                                            &PowerButtonHandle
                                           );
  }
  DEBUG((DEBUG_INFO,"Power Button callback registration status: %r\n", Status ));

  return Status;
}*/

/**
  Check the Fed SwSmiInputValue to see if there is a duplicated one in the database

  @param[in] FedSwSmiInputValue   Fed SwSmiInputValue

  @retval EFI_SUCCESS             There is no duplicated SwSmiInputValue
  @retval EFI_INVALID_PARAMETER   There is a duplicated SwSmiInputValue
**/
EFI_STATUS
SmiInputValueDuplicateCheck (
  UINTN           FedSwSmiInputValue
  )
{

  DATABASE_RECORD *RecordInDb;
  LIST_ENTRY      *LinkInDb;

  LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);
  while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
    RecordInDb = DATABASE_RECORD_FROM_LINK (LinkInDb);

    if (RecordInDb->ProtocolType == SwType) {
      if (RecordInDb->ChildContext.Sw.SwSmiInputValue == FedSwSmiInputValue) {
        return EFI_INVALID_PARAMETER;
      }
    }

    LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
  }

  return EFI_SUCCESS;
}

/**
  Register a child SMI dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for this SMI source.
  @param[in] RegisterContext      Pointer to the dispatch function's context.
  @param[out] DispatchHandle       Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.

  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database record
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid
  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
**/
EFI_STATUS
PchSmmCoreRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                          *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2                      DispatchFunction,
  IN  PCH_SMM_CONTEXT                                   *RegisterContext,
  OUT EFI_HANDLE                                        *DispatchHandle
  )
{
  EFI_STATUS                  Status;
  DATABASE_RECORD             *Record;
  PCH_SMM_QUALIFIED_PROTOCOL  *Qualified;
  PCH_SMM_SOURCE_DESC         NullSourceDesc = NULL_SOURCE_DESC_INITIALIZER;
  PCH_SERIES                  PchSeries; 
  UINTN                       Index;

  PchSeries = GetPchSeries();
  Index     = 0;
  ///
  /// Create database record and add to database
  ///
  if (gSmst == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (DATABASE_RECORD), &Record);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Gather information about the registration request
  ///
  Record->Callback          = DispatchFunction;
  Record->CallbackContext   = RegisterContext; 
  Record->ChildContext      = *RegisterContext;

  Qualified                 = QUALIFIED_PROTOCOL_FROM_GENERIC (This);

  Record->ProtocolType      = Qualified->Type;

  Record->ContextFunctions  = mContextFunctions[Qualified->Type];
  Record->BufferSize        = 0;
  ///
  /// Perform linked list housekeeping
  ///
  Record->Signature = DATABASE_RECORD_SIGNATURE;

  switch (Qualified->Type) {
  ///
  /// By the end of this switch statement, we'll know the
  /// source description the child is registering for
  ///
  case UsbType:
    ///
    /// Check the validity of Context Type
    ///
    if ((Record->ChildContext.Usb.Type < UsbLegacy) || (Record->ChildContext.Usb.Type > UsbWake)) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    MapUsbToSrcDesc (RegisterContext, &(Record->SrcDesc));
    Record->ClearSource = NULL;
    ///
    /// use default clear source function
    ///
    break;

  case SxType:
    ///
    /// Check the validity of Context Type and Phase
    ///
    if ((Record->ChildContext.Sx.Type < SxS0) ||
        (Record->ChildContext.Sx.Type >= EfiMaximumSleepType) ||
        (Record->ChildContext.Sx.Phase < SxEntry) ||
        (Record->ChildContext.Sx.Phase >= EfiMaximumPhase)
        ) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) (&SX_SOURCE_DESC), sizeof (PCH_SMM_SOURCE_DESC));
    Record->ClearSource = NULL;
    ///
    /// use default clear source function
    ///
    break;

  case SwType:
    if (RegisterContext->Sw.SwSmiInputValue == (UINTN)-1) {
      ///
      /// If SwSmiInputValue is set to (UINTN) -1 then a unique value will be assigned and returned in the structure.
      ///
      Status = EFI_NOT_FOUND;
      for (Index = 1; Index < MAXIMUM_SWI_VALUE; Index++) {
        Status = SmiInputValueDuplicateCheck (Index);
        if (!EFI_ERROR (Status)) {
          RegisterContext->Sw.SwSmiInputValue = Index;
          break;
        }
      }
      if (RegisterContext->Sw.SwSmiInputValue == (UINTN)-1) {
        Status = gSmst->SmmFreePool (Record);
        return EFI_OUT_OF_RESOURCES;
      }
      ///
      /// Update ChildContext again as SwSmiInputValue has been changed
      ///
      Record->ChildContext = *RegisterContext;
    }
    

    ///
    /// Check the validity of Context Value
    ///
    if (Record->ChildContext.Sw.SwSmiInputValue > MAXIMUM_SWI_VALUE) {
      goto Error;
    }

    if (EFI_ERROR (SmiInputValueDuplicateCheck (Record->ChildContext.Sw.SwSmiInputValue))) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) (&SW_SOURCE_DESC), sizeof (PCH_SMM_SOURCE_DESC));
    Record->ClearSource = NULL;
// Record->BufferSize = sizeof (EFI_SMM_SW_REGISTER_CONTEXT); //AptioV Override: Use EFI_SMM_SW_CONTEXT to fix AFU Fail Issue 
    Record->BufferSize = sizeof (EFI_SMM_SW_CONTEXT); //AptioV Override: Use EFI_SMM_SW_CONTEXT to fix AFU Fail Issue
    ///
    /// use default clear source function
    ///
    break;

  case GpiType:
    Index = Record->ChildContext.Gpi.GpiNum;
    if (PchSeries == PchH) {
      Index -= V_PCH_LPTH_ALT_GP_SMI_GPIBASE;
      if (Index >= S_PCH_LPTH_ALT_GP_SMI_GPISIZE) {
        goto Error;
      }
    } else if (PchSeries == PchLp) {
      Index -= V_PCH_LPTLP_ALT_GP_SMI_GPIBASE;
      if (Index >= S_PCH_LPTLP_ALT_GP_SMI_GPISIZE) {
        goto Error;
      }
    } else {
      goto Error;
    }
    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    if (PchSeries == PchH) {
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(LPTH_GPI_SOURCE_DESC[Index]),
        sizeof (PCH_SMM_SOURCE_DESC)
      );
    } else if (PchSeries == PchLp) {
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(LPTLP_GPI_SOURCE_DESC[Index]),
        sizeof (PCH_SMM_SOURCE_DESC)
      );
    }
    Record->ClearSource = NULL;
    ///
    /// use default clear source function
    ///
    break;

  case IchnType:
    ///
    /// Check the validity of Context Type
    ///
    if (Record->ChildContext.Ichn.Type >= NUM_ICHN_TYPES) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    if (PchSeries == PchLp) {
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(ICHN_LP_SOURCE_DESCS[Record->ChildContext.Ichn.Type]),
        sizeof (PCH_SMM_SOURCE_DESC)
        );
    } else if (PchSeries == PchH) {
      CopyMem (
        (VOID *) &(Record->SrcDesc),
        (VOID *) &(ICHN_H_SOURCE_DESCS[Record->ChildContext.Ichn.Type]),
        sizeof (PCH_SMM_SOURCE_DESC)
        );
    }
    Record->ClearSource = PchSmmIchnClearSource;
    break;

  case IchnExType:
    ///
    /// Check the validity of Context Type
    ///
    if ((Record->ChildContext.IchnEx.Type < IchnExPciExpress) || (Record->ChildContext.IchnEx.Type >= IchnExTypeMAX)) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    CopyMem (
      (VOID *) &(Record->SrcDesc),
      (VOID *) &(ICHN_EX_SOURCE_DESCS[Record->ChildContext.IchnEx.Type - IchnExPciExpress]),
      sizeof (PCH_SMM_SOURCE_DESC)
      );
    Record->ClearSource = NULL;
    break;

  case PowerButtonType:
    ///
    /// Check the validity of Context Phase
    ///
    if ((Record->ChildContext.PowerButton.Phase < EfiPowerButtonEntry) ||
        (Record->ChildContext.PowerButton.Phase > EfiPowerButtonExit)
        ) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    CopyMem ((VOID *) &(Record->SrcDesc), (VOID *) &POWER_BUTTON_SOURCE_DESC, sizeof (PCH_SMM_SOURCE_DESC));
    Record->ClearSource = NULL;
    ///
    /// use default clear source function
    ///
    break;

  case PeriodicTimerType:
    ///
    /// Check the validity of timer value
    ///
    if (RegisterContext->PeriodicTimer.SmiTickInterval <= 0) {
      goto Error;
    }

    InsertTailList (&mPrivateData.CallbackDataBase, &Record->Link);
    MapPeriodicTimerToSrcDesc (RegisterContext, &(Record->SrcDesc));
    Record->BufferSize = sizeof (EFI_SMM_PERIODIC_TIMER_CONTEXT);
    Record->ClearSource = PchSmmPeriodicTimerClearSource;
    break;

  default:
    return EFI_INVALID_PARAMETER;
    break;
  }

  if (CompareSources (&Record->SrcDesc, &NullSourceDesc)) {
    goto Error;
  }

  if (Record->ClearSource == NULL) {
    ///
    /// Clear the SMI associated w/ the source using the default function
    ///
    PchSmmClearSource (&Record->SrcDesc);
  } else {
    ///
    /// This source requires special handling to clear
    ///
    Record->ClearSource (&Record->SrcDesc);
  }

  PchSmmEnableSource (&Record->SrcDesc);

  ///
  /// Child's handle will be the address linked list link in the record
  ///
  *DispatchHandle = (EFI_HANDLE) (&Record->Link);

  return EFI_SUCCESS;

Error:
  Status = gSmst->SmmFreePool (Record);
  ///
  /// DEBUG((DEBUG_ERROR,"Free pool status %d\n", Status ));
  ///
  return EFI_INVALID_PARAMETER;
}

/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the  EFI_SMM_IO_TRAP_DISPATCH_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
PchSmmCoreUnRegister (
  IN PCH_SMM_GENERIC_PROTOCOL                           *This,
  IN EFI_HANDLE                                         *DispatchHandle
  )
{
  BOOLEAN         SafeToDisable;
  DATABASE_RECORD *RecordToDelete;
  DATABASE_RECORD *RecordInDb;
  LIST_ENTRY      *LinkInDb;

  if (DispatchHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RecordToDelete = DATABASE_RECORD_FROM_LINK (DispatchHandle);

  ///
  /// Take the entry out of the linked list
  ///
  if (RecordToDelete->Link.ForwardLink == (LIST_ENTRY *) EFI_BAD_POINTER) {
    return EFI_INVALID_PARAMETER;
  }

  RemoveEntryList (&RecordToDelete->Link);

  //See if we can disable the source
  //If there is no another record in DB with the same enable bits, we can disable source of SMI
  SafeToDisable = TRUE;
  LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);
  while(!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
    RecordInDb = DATABASE_RECORD_FROM_LINK (LinkInDb);
    if (CompareEnables (&RecordToDelete->SrcDesc, &RecordInDb->SrcDesc)) {
      SafeToDisable = FALSE;
      break;
    }
    LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
  }
  if (SafeToDisable) {
    PchSmmDisableSource( &RecordToDelete->SrcDesc );
  }

  return EFI_SUCCESS;
}

// AptioV Server Override Start: Fix for System hang when setting BIOS LOCK Enable BIT and SMM WriteProtect Bit in PCH register
BOOLEAN
IsTcoEvent(
  VOID
  )
/*++

Routine Description:
  
  Dummy Handler to handle writes to BIOS when the BIOSWE bit(BIOS_CNTL.Bit0) goes from 0 to 1
  while the BLE bit (BIOS_CNTL.Bit1 register) is already/also set.
  
  When this occurs, the PCH TCO1_STS:BIOSWR_STS (Bit8) gets set, as well as the SMI_STS.TCO_STS (Bit13)
  
  This handler will clear those two status bits.

  OPTIONAL: Add additional verification routine to determine whether the attempt to write to BIOS is valid or not,
            and then handle accordingly.
--*/
{
  UINT16        Data16;
  BOOLEAN       Status;
  
  Status = FALSE;
  //DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: All SMI sources checked with none confirmed. Will now check if TCO1.BIOSWR Status Bit was set.\n"));
  
  Data16 = IoRead16((mAcpiBaseAddr + PCH_TCO_BASE + R_PCH_TCO1_STS));
  if (Data16 & B_PCH_TCO1_STS_BIOSWR) {
    //DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: Confirmed that TC01.BIOSWR Status Bit was set. Will clear it, along with the SMI_STS.TCO_STS bit.\n"));
    Data16 = B_PCH_TCO1_STS_BIOSWR;
    IoWrite16 ((mAcpiBaseAddr + PCH_TCO_BASE+ R_PCH_TCO1_STS), Data16); //Clear the bit by writing back to it (RW1C)
    
    Data16 = B_PCH_SMI_STS_TCO;
    IoWrite16 ((mAcpiBaseAddr + R_PCH_SMI_STS), Data16); //Clear the bit by writing back to it (RW1C)

    //AptioV Server override: Clear BIOSWE bit in BIOS CNTRL register if it is set.
    Data16 = MmioRead16 (
				PchPciDeviceMmBase (DEFAULT_PCI_BUS_NUMBER_PCH,
				PCI_DEVICE_NUMBER_PCH_LPC,
				PCI_FUNCTION_NUMBER_PCH_LPC) + R_PCH_LPC_BIOS_CNTL
				);	 
    //DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: BIOS_CNTL: %x \n", Data16));                                  
    if ( Data16 & B_PCH_LPC_BIOS_CNTL_BIOSWE ) {
        //DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: BIOSWE bit in BIOS_CNTL is set. \n"));
        MmioWrite16 (
			PchPciDeviceMmBase (DEFAULT_PCI_BUS_NUMBER_PCH,
			PCI_DEVICE_NUMBER_PCH_LPC,
			PCI_FUNCTION_NUMBER_PCH_LPC) + R_PCH_LPC_BIOS_CNTL,
			Data16 & ((UINT16)~B_PCH_LPC_BIOS_CNTL_BIOSWE)
			);	 
    }
    //AptioV Server override: Clear BIOSWE bit if it is set.

    Status = TRUE;
  } //else DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: Confirmed that TC01.BIOSWR Status Bit was NOT set (0x%x). No action will be taken.\n", Data16));

  //DEBUG((EFI_D_ERROR | EFI_D_INFO,"IsTcoEvent: Status = %d\n", Status));
  return Status;
}
// AptioV Server Override End: Fix for System hang when setting BIOS LOCK Enable BIT and SMM WriteProtect Bit in PCH register
/**
  The callback function to handle subsequent SMIs.  This callback will be called by SmmCoreDispatcher.

  @param[in] DispatchHandle         Not used
  @param[in] PchSmmCoreContext      Not used
  @param[in] CommunicationBuffer    Not used
  @param[in] BufferSize             Not used

  @retval EFI_SUCCESS                   Function successfully completed
**/
EFI_STATUS
EFIAPI
PchSmmCoreDispatcher (
  IN     EFI_HANDLE               DispatchHandle,
  IN     CONST VOID               *PchSmmCoreContext,        OPTIONAL
  IN OUT VOID                     *CommunicationBuffer,     OPTIONAL
  IN OUT UINTN                    *BufferSize  OPTIONAL
  )
{
  ///
  /// Used to prevent infinite loops
  ///
  UINTN               EscapeCount;
  BOOLEAN             ContextsMatch;
  BOOLEAN             EosSet;
  BOOLEAN             SxChildWasDispatched;
  BOOLEAN             PbtnChildWasDispatched = FALSE;         //AptioV server override: Multiple Power button handler support

  DATABASE_RECORD     *RecordInDb;
  LIST_ENTRY          *LinkInDb;
  DATABASE_RECORD     *RecordToExhaust;
  LIST_ENTRY          *LinkToExhaust;

  PCH_SMM_CONTEXT     Context;

  EFI_STATUS          Status;

  PCH_SMM_SOURCE_DESC ActiveSource = NULL_SOURCE_DESC_INITIALIZER;

  EscapeCount           = 100;
  ContextsMatch         = FALSE;
  EosSet                = FALSE;
  SxChildWasDispatched  = FALSE;
  Status                = EFI_SUCCESS;

  if (!IsListEmpty (&mPrivateData.CallbackDataBase)) {
    ///
    /// We have children registered w/ us -- continue
    ///
    while ((!EosSet) && (EscapeCount > 0)) {
      EscapeCount--;

      LinkInDb = GetFirstNode (&mPrivateData.CallbackDataBase);

      while (!IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
        RecordInDb = DATABASE_RECORD_FROM_LINK (LinkInDb);

        ///
        /// look for the first active source
        ///
        if (!SourceIsActive (&RecordInDb->SrcDesc)) {
          ///
          /// Didn't find the source yet, keep looking
          ///
          LinkInDb = GetNextNode (&mPrivateData.CallbackDataBase, &RecordInDb->Link);
// AptioV Server Override Start: Fix for System hang when setting BIOS LOCK Enable BIT and SMM WriteProtect Bit in PCH register
          //TODO: Print out the RecordInDb contents to see if TCO events are different from others.
          //      If so, then we can make this solution more elegant and handle the event in the 
          //      else block in line 754, below. the key is to have SourceIsActive trigger the else block.
          //
          // if it's the last one, check to see if it's a TCO event then try to clear EOS
          //
          if (IsNull (&mPrivateData.CallbackDataBase, LinkInDb)) {
            if (!IsTcoEvent()) {
              EosSet = PchSmmSetAndCheckEos ();
            }
// AptioV Server Override End: Fix for System hang when setting BIOS LOCK Enable BIT and SMM WriteProtect Bit in PCH register
          }
        } else {
          ///
          /// We found a source. If this is a sleep type, we have to go to
          /// appropriate sleep state anyway.No matter there is sleep child or not
          ///
          if (RecordInDb->ProtocolType == SxType) {
            SxChildWasDispatched = TRUE;
          }
   //AptioV server override start: Multiple Power button handler support
          if (RecordInDb->ProtocolType == PowerButtonType) {
            PbtnChildWasDispatched = TRUE;
          }
   //AptioV server override end: Multiple Power button handler support
          ///
          /// "cache" the source description and don't query I/O anymore
          ///
          CopyMem ((VOID *) &ActiveSource, (VOID *) &(RecordInDb->SrcDesc), sizeof (PCH_SMM_SOURCE_DESC));
          LinkToExhaust = LinkInDb;

          ///
          /// exhaust the rest of the queue looking for the same source
          ///
          while (!IsNull (&mPrivateData.CallbackDataBase, LinkToExhaust)) {
            RecordToExhaust = DATABASE_RECORD_FROM_LINK (LinkToExhaust);
            ///
            /// RecordToExhaust->Link might be removed (unregistered) by Callback function, and then the
            /// system will hang in ASSERT() while calling GetNextNode().
            /// To prevent the issue, we need to get next record in DB here (before Callback function).
            ///
            LinkToExhaust = GetNextNode (&mPrivateData.CallbackDataBase, &RecordToExhaust->Link);

            if (CompareSources (&RecordToExhaust->SrcDesc, &ActiveSource)) {
              ///
              /// These source descriptions are equal, so this callback should be
              /// dispatched.
              ///
              if (RecordToExhaust->ContextFunctions.GetContext != NULL) {
                ///
                /// This child requires that we get a calling context from
                /// hardware and compare that context to the one supplied
                /// by the child.
                ///
                ASSERT (RecordToExhaust->ContextFunctions.CmpContext != NULL);

                ///
                /// Make sure contexts match before dispatching event to child
                ///
                RecordToExhaust->ContextFunctions.GetContext (RecordToExhaust, &Context);
                ContextsMatch = RecordToExhaust->ContextFunctions.CmpContext (&Context, &RecordToExhaust->ChildContext);

              } else {
                ///
                /// This child doesn't require any more calling context beyond what
                /// it supplied in registration.  Simply pass back what it gave us.
                ///
                ASSERT (RecordToExhaust->Callback != NULL);
                Context       = RecordToExhaust->ChildContext;
                ContextsMatch = TRUE;
              }

              if (ContextsMatch) {
                if (RecordToExhaust->BufferSize != 0) {
                  ASSERT (RecordToExhaust->ContextFunctions.GetBuffer != NULL);

                  RecordToExhaust->ContextFunctions.GetBuffer (RecordToExhaust);

                  CommunicationBuffer = &RecordToExhaust->CommBuffer;
                  //AptioV server override start: Should fill BufferSize address and value is not required as it is always sizeof (EFI_SMM_SW_CONTEXT).
                 // *BufferSize = RecordToExhaust->BufferSize;
                  BufferSize = &RecordToExhaust->BufferSize;
                  //AptioV server override end: Should fill BufferSize address and value is not required as it is always sizeof (EFI_SMM_SW_CONTEXT).
                } else {
                  CommunicationBuffer = NULL;
                  //AptioV server override start: Should fill BufferSize address and not value at address.
                 // *BufferSize = 0;
                  BufferSize = NULL;
                  //AptioV server override end: Should fill BufferSize address and not value at address.
                }

                ASSERT (RecordToExhaust->Callback != NULL);

                 RecordToExhaust->Callback ((EFI_HANDLE) & RecordToExhaust->Link, &Context,CommunicationBuffer,BufferSize);

                if (RecordToExhaust->ProtocolType == SxType) {
                  SxChildWasDispatched = TRUE;
                }

                  //AptioV server override start: Multiple Power button handler support
                if (RecordToExhaust->ProtocolType == PowerButtonType) {
                  PbtnChildWasDispatched = TRUE;
                }
                  //AptioV server override end: Multiple Power button handler support
              }
            }
          }

          if (RecordInDb->ClearSource == NULL) {
            ///
            /// Clear the SMI associated w/ the source using the default function
            ///
            PchSmmClearSource (&ActiveSource);
          } else {
            ///
            /// This source requires special handling to clear
            ///
            RecordInDb->ClearSource (&ActiveSource);
          }
          ///
          /// Also, try to clear EOS
          ///
          EosSet = PchSmmSetAndCheckEos ();
          ///
          /// Queue is empty, reset the search
          ///
          break;
        }
      }
    }
  }
  ///
  /// If you arrive here, there are two possible reasons:
  /// (1) you've got problems with clearing the SMI status bits in the
  /// ACPI table.  If you don't properly clear the SMI bits, then you won't be able to set the
  /// EOS bit.  If this happens too many times, the loop exits.
  /// (2) there was a SMM communicate for callback messages that was received prior
  /// to this driver.
  /// If there is an asynchronous SMI that occurs while processing the Callback, let
  /// all of the drivers (including this one) have an opportunity to scan for the SMI
  /// and handle it.
  /// If not, we don't want to exit and have the foreground app. clear EOS without letting
  /// these other sources get serviced.
  ///
  /// This assert is not valid with CSM legacy solution because it generates software SMI
  /// to test for legacy USB support presence.
  /// This may not be illegal, so we cannot assert at this time.
  ///
  ///  ASSERT (EscapeCount > 0);
  ///
   //AptioV server override start: Multiple Power button handler support
  if (PbtnChildWasDispatched) {
    IoOr32(mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, V_PCH_ACPI_PM1_CNT_S5);
    IoOr32(mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT, B_PCH_ACPI_PM1_CNT_SLP_EN);
  }
  //AptioV server override end: Multiple Power button handler support
  if (SxChildWasDispatched) {
    ///
    /// A child of the SmmSxDispatch protocol was dispatched during this call;
    /// put the system to sleep.
    ///
    PchSmmSxGoToSleep ();
  }

  return Status;

}
