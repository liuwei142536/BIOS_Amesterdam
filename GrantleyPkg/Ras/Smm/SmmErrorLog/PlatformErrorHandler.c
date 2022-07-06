/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  This file contains an 'Intel Peripheral Driver' and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement

Copyright (c) 2009-2010 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  PlatformErrorLog.c

Abstract:

  Platform based component hooks for event logging.

-------------------------------------------------------------------*/

#include "CommonErrorHandlerIncludes.h"


EFI_GUID gBootFlowGuid = BOOT_FLOW_VARIABLE_GUID;
EFI_SEL_RECORD_DATA           mSelRecord;
UINT8   mMemErrCount=0;
UINT8   mPciErrCount=0;
EFI_SMM_GPI_DISPATCH2_PROTOCOL *mGpiDispatch;



EFI_STATUS
ProcessPlatformUncorrectedError (
  VOID
)
{
  return EFI_SUCCESS;
}

EFI_STATUS
FillErrorSection
(
UINT8 McBank,
UINT8 *skt,
UINT32 Sev,
EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
GENERIC_ERROR_STATUS *ErrSts
)
{
  UINT16 SecType;
  GENERIC_ERROR_DATA_ENTRY *DataEntry;
  VOID  *ErrData;

  if(ErrSts == NULL || McErrSig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SecType = GetMcBankSecType(McBank);
  DataEntry = (GENERIC_ERROR_DATA_ENTRY *)((UINTN)ErrSts + sizeof(GENERIC_ERROR_STATUS));
  SetMem(DataEntry, sizeof(GENERIC_ERROR_DATA_ENTRY), 0);

  DataEntry->Severity = Sev;
  DataEntry->Revision = GENERIC_ERROR_SECTION_REVISION_UEFI231;
  DataEntry->SecValidMask.FruIdValid = 0; //FruId and FruText are not valid
  DataEntry->SecValidMask.FruStringValid = 0;
  DataEntry->SectionFlags.Primary = 1; //primary error 

  ErrData = (VOID *)((UINT64)DataEntry + sizeof(GENERIC_ERROR_DATA_ENTRY));

  switch(SecType) {
    case EMCA_SECTYPE_PLATMEM:
      DataEntry->DataSize = sizeof(PLATFORM_MEMORY_ERROR_DATA_UEFI_231);
      CopyMem(&DataEntry->SectionType,&gEfiWheaPlatformMemoryErrorSectionGuid,sizeof(GUID));
      //Need to add FruString and FruId here. 
      FillPlatformMemorySection(McBank, skt, &Sev, McErrSig, ErrData);
      break;
      
    case EMCA_SECTYPE_NONE:
    default:
      return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
ConstructErrorRecord
(
  UINT32 Sev,
  GENERIC_ERROR_STATUS *ErrSts
)
{
  GENERIC_ERROR_DATA_ENTRY *DataEntry;

  if(ErrSts == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetMem(ErrSts, sizeof(GENERIC_ERROR_STATUS), 0);
  DataEntry = (GENERIC_ERROR_DATA_ENTRY *) ((UINTN)ErrSts + sizeof(GENERIC_ERROR_STATUS));

  ErrSts->Severity = Sev; 
  ErrSts->ErrDataSize = sizeof(GENERIC_ERROR_DATA_ENTRY) + DataEntry->DataSize;
  ErrSts->BlockStatus.NumErrorDataEntry++;
  
  if(Sev == EFI_ACPI_5_0_ERROR_SEVERITY_FATAL || Sev == EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE) {
    ErrSts->BlockStatus.UeValid = 1;
  } else {
    ErrSts->BlockStatus.CeValid = 1;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
LogWheaRecord
(
  UINT8 McBank,
  EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
  GENERIC_ERROR_STATUS *ErrSts
)
{
  MEMORY_DEV_INFO MemInfo;
  UINT16        ErrorType;
  GENERIC_ERROR_DATA_ENTRY *DataEntry;
  PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *MemSection;
  EFI_STATUS Status;

  if(McErrSig == NULL && ErrSts == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;

  DataEntry = (GENERIC_ERROR_DATA_ENTRY *) ((UINTN)ErrSts + sizeof(GENERIC_ERROR_STATUS));

  if(CompareGuid ((GUID *)&DataEntry->SectionType, &gEfiWheaPlatformMemoryErrorSectionGuid)) {
    MemSection = (PLATFORM_MEMORY_ERROR_DATA_UEFI_231 *)((UINTN)DataEntry + sizeof(GENERIC_ERROR_DATA_ENTRY));
    switch(ErrSts->Severity) {
      case EFI_ACPI_5_0_ERROR_SEVERITY_FATAL:
        ErrorType = GEN_ERR_SEV_PLATFORM_MEMORY_FATAL;
        break;
      case EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTABLE:
        ErrorType = GEN_ERR_SEV_PLATFORM_MEMORY_RECOVERABLE;
        break;
      case EFI_ACPI_5_0_ERROR_SEVERITY_CORRECTED:
        ErrorType = GEN_ERR_SEV_PLATFORM_MEMORY_CORRECTED;
        break;
      default:
        ErrorType = GEN_ERR_SEV_PLATFORM_MEMORY_INFORMATION;
        break;
    }
    DEBUG ((DEBUG_ERROR, "LogWheaRecord : Node = %d, ValidBits = %x, Uefi 2_3 = %x, ErrorType = %x, McBank = %d, \n",
            MemSection->Node, 
            MemSection->ValidFields, 
            mErrLogSetupData->WheaErrorRecordFormat,  
            ErrorType, 
            McBank));
    if( ( ErrorType == GEN_ERR_SEV_PLATFORM_MEMORY_CORRECTED ) ||
        ( ErrorType == GEN_ERR_SEV_PLATFORM_MEMORY_RECOVERABLE ) || 
        (ErrorType == GEN_ERR_SEV_PLATFORM_MEMORY_FATAL) ) {
      

      MemInfo.Bank = MemSection->Bank;
      MemInfo.Branch = 0;
      MemInfo.Channel = MemSection->Card;
      MemInfo.Column = MemSection->Column;
      MemInfo.Device = MemSection->Device;
      MemInfo.Dimm = MemSection->ModuleRank;
      MemInfo.ErrorType = MemSection->ErrorType;   
      MemInfo.Node = MemSection->Node;
      MemInfo.PhyAddr = MemSection->PhysicalAddress;
      MemInfo.Rank = MemSection->RankNumber;
      MemInfo.Row = MemSection->Row;
      MemInfo.SmBiosCardHandle = MemSection->SmBiosCardHandle;
      MemInfo.SmBiosModuleHandle = MemSection->SmBiosModuleHandle;
      MemInfo.UefiErrorRecordRevision = mErrLogSetupData->WheaErrorRecordFormat;
      MemInfo.ValidBits = MemSection->ValidFields;

      
      DEBUG ((DEBUG_ERROR, "    Addr = 0x%lx, ddrCh = %d, rank = %d, dimm = %d, Device = %d,  \n",
              MemInfo.PhyAddr, 
              MemInfo.Channel, 
              MemInfo.Rank,  
              MemInfo.Dimm, 
              MemInfo.Device ));
      
      DEBUG ((DEBUG_ERROR, "    Bank = %d, Column = %d, Row = %d, SmBiosCardHandle = %d, SmBiosModuleHandle = %d,  \n",
               MemInfo.Bank, 
               MemInfo.Column, 
               MemInfo.Row,  
               MemInfo.SmBiosCardHandle, 
               MemInfo.SmBiosModuleHandle ));
      DEBUG ((DEBUG_ERROR, "    ErrorType = %d \n",MemInfo.ErrorType ));
      

      if( ( MemInfo.Node >= MAX_SOCKET) || (( MemInfo.PhyAddr >> 46) != 0) || ((MemInfo.ValidBits >> 14) != 0) ) {
        DEBUG ((DEBUG_ERROR, "LogWheaRecord : Wrong MemSection data \n" ));
        MemInfo.Bank    = 00;
        MemInfo.Branch  = 0;
        MemInfo.Channel = 00;
        MemInfo.ValidBits = PLATFORM_MEM_NODE_VALID;
      }

      if(mWheaLogStarted == FALSE)  {
        Status = mWheaPlatformSupport->WheaPlatformRestartAllElog (mWheaPlatformSupport);
        if (Status == EFI_SUCCESS) {
          mWheaLogStarted = TRUE;
        }
      }  

      if (ErrorType == GEN_ERR_SEV_PLATFORM_MEMORY_RECOVERABLE) {
        // recovery action is taken care by OS. OS is expecting McBanks status for recovery action.  
        // Bios has no way to provide with the existing structures.Bios side we ned to specify as None or corrected.
        ErrorType = GEN_ERR_SEV_PLATFORM_MEMORY_CORRECTED;
      }
      
      Status = mWheaPlatformSupport->WheaPlatformElogMemory (
        mWheaPlatformSupport,
        ErrorType,
        &MemInfo
      );
      MemInfo.ValidBits = 0;
      Status = mWheaPlatformSupport->WheaPlatformEndElog (mWheaPlatformSupport );
    }
  }

  return Status;
}

EFI_STATUS
LogErrorRecord
(
UINT8 McBank,
EMCA_MC_SIGNATURE_ERR_RECORD *McErrSig,
GENERIC_ERROR_STATUS *ErrSts
)
{
  EFI_STATUS Status = EFI_SUCCESS;

  if(mErrLogSetupData->EMcaLogEn == 1 && 
    (mErrLogSetupData->EMcaForceOptin == 1 || isEmcaOptInFlagSet()) &&
    isEMCALogSupported(McBank)) {
    AcquireSpinLock(&mSmmErrorMpSyncData.LoggingLock);
    Status = LogEmcaRecord(McBank, McErrSig, ErrSts);
    ReleaseSpinLock(&mSmmErrorMpSyncData.LoggingLock);
  }
  /* Aptio V Server Override Start : AMI logging code will log to WHEA listener
  else if(mWheaPlatformSupportAvailable == TRUE) {
    AcquireSpinLock (&mSmmErrorMpSyncData.LoggingLock);
    Status = LogWheaRecord(McBank, McErrSig, ErrSts);
    ReleaseSpinLock (&mSmmErrorMpSyncData.LoggingLock);
  }
 Aptio V Server Override End : AMI logging code will log to WHEA listener  */
  return Status;
}


VOID
ClearPlatformErrors(VOID)
{
  return;
}


VOID
ClearGpiSmiStatus (
  VOID
  )
//
// We need to clear any pending GPI SMI status to avoid a EOS
//
{
  UINT16  Data16;
  UINT16  PmBase;
  //
  // Get the PM Base Address
  //
  PmBase = PciRead16 (PCI_LIB_ADDRESS (
            DEFAULT_PCI_BUS_NUMBER_PCH, // Bus
            PCI_DEVICE_NUMBER_PCH_LPC,
            PCI_FUNCTION_NUMBER_PCH_LPC, 
            R_PCH_LPC_ACPI_BASE // Register
            ));

  //
  // Base Address should be in bits 15:7 of PMBASE register 9.1.1 page 280, but 0x400 is bigger
  // so they use the whole 16 bit
  //
  PmBase = (UINT16) (PmBase & 0xFF80);
  //
  // caterr# connected to GPI 8 
  //
  Data16 = IoRead16 ((PmBase + R_ACPI_ALT_GP_SMI_STS));
  Data16 |= BIT8;
  IoWrite16 ((PmBase + R_ACPI_ALT_GP_SMI_STS), Data16);
}

EFI_STATUS
ElogProcessorCatastrophicErrorHandler (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *GpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  // Return success
  return EFI_SUCCESS;
}





static
UINT8
RtcRead (
  IN UINT8 Address
  )
/*++

Routine Description:
    This function reads the CMOS location

Arguments:
    i/p : cmos address to read 

Returns:
    o/p : cmos address value

--*/
{
  UINT8 data8;

  //
  // Port 0x74 is alias of port 0x70 which is write only
  //
  data8 = IoRead8 (0x74);
  data8 &= 0x80;
  data8 |= Address;
  IoWrite8 (0x70, data8);

  //
  // Data is at port 0x71
  //
  data8= IoRead8 (0x71);
  return data8;
}


EFI_STATUS
EfiSmmGetTime (
  IN OUT TIME_STAMP *Time
  )
  /*++

Routine Description:
    This function provides the time stamp information.

Arguments:
    time :  

Returns:
    time : 

--*/
{
  EFI_STATUS  status;
  UINT8       data8;

  status        = EFI_SUCCESS;

  data8         = RtcRead (RTC_ADDRESS_YEAR);
  Time->Year    = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_MONTH);
  Time->Month   = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_DAY_OF_THE_MONTH);
  Time->Day     = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_HOURS);
  Time->Hour    = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_MINUTES);
  Time->Minute  = BCD2DEC (data8);
  data8         = RtcRead (RTC_ADDRESS_SECONDS);
  Time->Second  = BCD2DEC (data8);

  return status;
}


UINT32
EfiSmmGetNoofHoursPast (
  IN OUT TIME_STAMP *PrevTime
  )
{
  TIME_STAMP  CurTime;
  UINT32      CurHours;
  UINT32      PrevHours;
  UINT32      HoursPast = 0;

  EfiSmmGetTime (&CurTime);

  CurHours = (CurTime.Year * 365 * 24) + (CurTime.Month * 30 * 24) + (CurTime.Day * 24) + (CurTime.Hour);
  PrevHours = (PrevTime->Year * 365 * 24) + (PrevTime->Month * 30 * 24) + (PrevTime->Day * 24) + (PrevTime->Hour);
  if (CurHours > PrevHours)
    HoursPast = (CurHours - PrevHours);

  return HoursPast;
}


EFI_STATUS
GpiChildCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *mGpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{   
  EFI_SMM_GPI_REGISTER_CONTEXT *Context = (EFI_SMM_GPI_REGISTER_CONTEXT *)mGpiContext;
  //
  // caterr# connected to GPI 8 
  //
  if(Context->GpiNum == ICH_GPI_SMI_CAT_ERROR_PIN_CV)   {
    ElogProcessorCatastrophicErrorHandler (DispatchHandle, mGpiContext, CommBuffer, CommBufferSize);
  }   
  return EFI_SUCCESS;
}


EFI_STATUS
EnableGpiChildHandler (
  UINT8     GpiNumber
  )
{
  EFI_SMM_GPI_REGISTER_CONTEXT  mGpiContext;
  EFI_HANDLE                    GpiHandle;
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;
  //
  // Register our SMI handlers through a child protocol
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmGpiDispatch2ProtocolGuid, NULL, &mGpiDispatch);
  ASSERT_EFI_ERROR (Status);

  //
  // Register the enable handler
  //
  mGpiContext.GpiNum = GpiNumber;
  Status = mGpiDispatch->Register (
                          mGpiDispatch,
                          GpiChildCallback,
                          &mGpiContext,
                          &GpiHandle
                          );
  ASSERT_EFI_ERROR (Status);

  return Status;
}




