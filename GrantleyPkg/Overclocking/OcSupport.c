/** @file
  Overclocking Support implementation.

@copyright
  Copyright (c) 2012 Intel Corporation. All rights reserved
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
#include "OcSupport.h"
#pragma optimize("",off)

EFI_STATUS
EFIAPI
GetVoltageFrequencyItem (
  OUT VOLTAGE_FREQUENCY_ITEM *VfSettings,
  OUT UINT32                 *LibStatus
  )
/**
  Gets the Voltage and Frequency information for a given CPU domain

  @param[OUT] *VfSettings
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  UINT16          TempVoltageTarget;
  INT16           TempVoltageOffset;
  OC_MAILBOX_ITEM VfMsg;

  Status = EFI_SUCCESS;

  ZeroMem(&VfMsg,sizeof(VfMsg));
  ///
  /// Convert v/f command to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_GET_VOLTAGE_FREQUENCY;

  ConvertToMailboxFormat((VOID *)VfSettings, &VfMsg, CommandId);

  ///
  ///  Read From the OC Library
  ///
  Status = MailboxRead(MAILBOX_TYPE_OC, VfMsg.Interface.InterfaceData, &VfMsg.Data, LibStatus);

  ///
  ///  Copy mailbox data to VfSettings
  ///
  if ( (Status == EFI_SUCCESS) && (*LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS)) {
    VfSettings->VfSettings.MaxOcRatio = (UINT8) (VfMsg.Data & MAX_RATIO_MASK);
    VfSettings->VfSettings.VoltageTargetMode = (UINT8) ( (VfMsg.Data & VOLTAGE_MODE_MASK) >> VOLTAGE_MODE_OFFSET);

    TempVoltageTarget = (UINT16) (VfMsg.Data  & VOLTAGE_TARGET_MASK) >> VOLTAGE_TARGET_OFFSET;
    ConvertVoltageTarget(TempVoltageTarget, &VfSettings->VfSettings.VoltageTarget, CONVERT_TO_BINARY_MILLIVOLT);

    TempVoltageOffset = (INT16)((VfMsg.Data  & VOLTAGE_OFFSET_MASK) >> VOLTAGE_OFFSET_OFFSET);
    ConvertVoltageOffset(TempVoltageOffset, &VfSettings->VfSettings.VoltageOffset, CONVERT_TO_BINARY_MILLIVOLT);
  }

  return Status;
}

EFI_STATUS
EFIAPI
SetVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM VfSettings,
  OUT UINT32                *LibStatus
  )
/**
  Sets the Voltage and Frequency information for a given CPU domain

  @param[IN]  *VfSettings
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM VfMsg;

  Status = EFI_SUCCESS;

  ///
  /// Convert v/f Commands to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_SET_VOLTAGE_FREQUENCY;
  ConvertToMailboxFormat((VOID *)&VfSettings, &VfMsg, CommandId);

  ///
  /// Write the v/f Settings to the OC Mailbox
  ///
  Status = MailboxWrite(MAILBOX_TYPE_OC, VfMsg.Interface.InterfaceData, VfMsg.Data, LibStatus);

  return Status;
}

EFI_STATUS
EFIAPI
GetFivrConfig (
  OUT GLOBAL_CONFIG_ITEM *FivrConfig,
  OUT UINT32             *LibStatus
  )
/**
  Get the global FIVR Configuration information

  @param[OUT] *FivrConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM FivrMsg;

  Status = EFI_SUCCESS;
  ZeroMem(&FivrMsg, sizeof(FivrMsg));

  ///
  /// Convert FIVR message to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_GET_GLOBAL_CONFIG;
  ConvertToMailboxFormat((VOID *)FivrConfig, &FivrMsg, CommandId);

  ///
  ///  Read From the OC Library
  ///
  Status = MailboxRead(MAILBOX_TYPE_OC, FivrMsg.Interface.InterfaceData, &FivrMsg.Data, LibStatus);

  ///
  ///  Copy mailbox data to FivrConfig
  ///
  if ( (Status == EFI_SUCCESS) && (*LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS)) {
    FivrConfig->DisableFivrFaults = FivrMsg.Data & FIVR_FAULTS_MASK;
    FivrConfig->DisableFivrEfficiency = (FivrMsg.Data & FIVR_EFFICIENCY_MASK) >> FIVR_EFFICIENCY_OFFSET;
  }

  return Status;
}

EFI_STATUS
EFIAPI
SetFivrConfig (
  IN GLOBAL_CONFIG_ITEM FivrConfig,
  OUT UINT32            *LibStatus
  )
/**
  Set the Global FIVR Configuration information

  @param[IN]  FivrConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM FivrMsg;

  Status = EFI_SUCCESS;

  ///
  /// Convert FIVR Command to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_SET_GLOBAL_CONFIG;
  ConvertToMailboxFormat((VOID *)&FivrConfig, &FivrMsg, CommandId);

  ///
  /// Write the FIVR Settings to the OC Mailbox
  ///
  Status = MailboxWrite(MAILBOX_TYPE_OC, FivrMsg.Interface.InterfaceData, FivrMsg.Data, LibStatus);

  return Status;

}

EFI_STATUS
EFIAPI
GetSvidConfig (
  OUT SVID_CONFIG_ITEM *SvidConfig,
  OUT UINT32           *LibStatus
  )
/**
  Get the SVID Configuration information

  @param[OUT] *SvidConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM SvidMsg;

  Status = EFI_SUCCESS;
  ZeroMem(&SvidMsg, sizeof(SvidMsg));

  ///
  /// Convert SVID message to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_GET_SVID_CONFIG;
  ConvertToMailboxFormat((VOID *)SvidConfig, &SvidMsg, CommandId);

  ///
  ///  Read From the OC Library
  ///
  Status = MailboxRead(MAILBOX_TYPE_OC, SvidMsg.Interface.InterfaceData, &SvidMsg.Data, LibStatus);

  ///
  ///  Copy mailbox data to SvidConfig
  ///
  if ( (Status == EFI_SUCCESS) && (*LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS)) {
    SvidConfig->VoltageTarget = (UINT16) SvidMsg.Data & SVID_VOLTAGE_MASK;
    SvidConfig->SvidDisable = (UINT8) ((SvidMsg.Data & SVID_DISABLE_MASK) >> SVID_DISABLE_OFFSET);
  }

  return Status;
}

EFI_STATUS
EFIAPI
SetSvidConfig (
  IN SVID_CONFIG_ITEM SvidConfig,
  OUT UINT32          *LibStatus
  )
/**
  Set the SVID Configuration information

  @param[IN]  SvidConfig
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM SvidMsg;

  Status = EFI_SUCCESS;

  ///
  /// Convert SVID Commands to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_SET_SVID_CONFIG;
  ConvertToMailboxFormat((VOID *)&SvidConfig, &SvidMsg, CommandId);

  ///
  /// Write the Svid Settings to the OC Mailbox
  ///
  Status = MailboxWrite(MAILBOX_TYPE_OC, SvidMsg.Interface.InterfaceData, SvidMsg.Data, LibStatus);

  return Status;
}

EFI_STATUS
EFIAPI
GetOcCapabilities (
  OUT OC_CAPABILITIES_ITEM *OcCapabilities,
  OUT UINT32               *LibStatus
  )
/**
  Get the overclocking capabilities for a given CPU Domain

  @param[OUT] *OcCapabilities
  @param[OUT] *LibStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS      Status;
  UINT32          CommandId;
  OC_MAILBOX_ITEM OcCapsMsg;

  Status = EFI_SUCCESS;

  ZeroMem(&OcCapsMsg,sizeof(OC_MAILBOX_ITEM));

  ///
  /// Convert OC capabilties message to Mailbox command format
  ///
  CommandId = OC_LIB_CMD_GET_OC_CAPABILITIES;
  ConvertToMailboxFormat((VOID *)OcCapabilities, &OcCapsMsg, CommandId);

  ///
  ///  Read From the OC Library
  ///
  Status = MailboxRead(MAILBOX_TYPE_OC, OcCapsMsg.Interface.InterfaceData, &OcCapsMsg.Data, LibStatus);

  ///
  ///  Copy mailbox data to OC Capabilities structure
  ///
  if ( (Status == EFI_SUCCESS) && (*LibStatus == OC_LIB_COMPLETION_CODE_SUCCESS)) {
    OcCapabilities->MaxOcRatioLimit =
            (UINT8) OcCapsMsg.Data & OC_CAPS_MAX_RATIO_MASK;

    OcCapabilities->RatioOcSupported =
            (UINT8) ((OcCapsMsg.Data & OC_CAPS_RATIO_SUPPORT_MASK) >> OC_CAPS_RATIO_SUPPORT_OFFSET);

    OcCapabilities->VoltageOverridesSupported =
            (UINT8) ((OcCapsMsg.Data & OC_CAPS_OVERRIDE_SUPPORT_MASK) >> OC_CAPS_OVERRIDE_SUPPORT_OFFSET);

    OcCapabilities->VoltageOffsetSupported =
            (UINT8) ((OcCapsMsg.Data & OC_CAPS_OFFSET_SUPPORT_MASK) >> OC_CAPS_OFFSET_SUPPORT_OFFSET);
  }

  return Status;
}

VOID
ConvertVoltageTarget (
  IN UINT16  InputVoltageTarget,
  OUT UINT16 *OutputVoltageTarget,
  IN UINT8   ConversionType
  )
/**
  Converts the input voltage target to the fixed point U12.2.10 Volt format or
  the Binary millivolts representation based on the ConversionType

@param[IN]  InputVoltageTarget
@param[OUT] *OutputVoltageTarget
@param[IN]  ConversionType - 0:fixed point, 1:Binary millivolts
**/
{
  ///  Fixed point representation:
  ///
  ///  U12.2.10V format
  ///  | | | |
  ///  | | | v
  ///  | | v Exponent
  ///  | v Significand Size
  ///  v Size
  ///  Signed/Unsigned
  ///
  ///  Float Value = Significand x (Base ^ Exponent)
  ///  (Base ^ Exponent) = 2 ^ 10 = 1024
  ///

  if (InputVoltageTarget == 0) {
    *OutputVoltageTarget = 0;
    return;
  }

  if (ConversionType == CONVERT_TO_FIXED_POINT_VOLTS) {
    ///
    /// Input Voltage is in number of millivolts. Clip the input Voltage
    /// to the max allowed by the fixed point format
    ///
    if (InputVoltageTarget > MAX_TARGET_MV)
      InputVoltageTarget = MAX_TARGET_MV;

    ///
    /// InputTargetVoltage is the significand in mV. Need to convert to Volts
    ///
    *OutputVoltageTarget = (InputVoltageTarget * 1024)/ MILLIVOLTS_PER_VOLT;

  } else if (ConversionType == CONVERT_TO_BINARY_MILLIVOLT) {
    ///
    /// InputVoltage is specified in fixed point representation, need to
    /// convert to millivolts
    ///
    *OutputVoltageTarget = (InputVoltageTarget * MILLIVOLTS_PER_VOLT)/1024;
  }

  return;
}

VOID
ConvertVoltageOffset (
  IN INT16  InputVoltageOffset,
  OUT INT16 *OutputVoltageOffset,
  IN UINT8  ConversionType
  )
/**
  Converts the input votlage Offset to the fixed point S11.0.10 Volt format or
  to Binary illivolts representation based on the ConversionType.

@param[IN]  InputVoltageTarget
@param[OUT] *OutputVoltageTarget
@param[IN]  ConversionType - 0:fixed point, 1:Signed Binary millivolts
**/
{
  BOOLEAN NumIsNegative;
  ///  Fixed point representation:
  ///
  ///  S11.0.10V format
  ///  | | | |
  ///  | | | v
  ///  | | v Exponent
  ///  | v Significand Size
  ///  v Size
  ///  Signed/Unsigned
  ///
  ///  Float Value = Significand x (Base ^ Exponent)
  ///  (Base ^ Exponent) = 2 ^ 10 = 1024
  ///
  *OutputVoltageOffset = 0;
  NumIsNegative = FALSE;

  if (InputVoltageOffset == 0) {
    *OutputVoltageOffset = 0;
    return;
  }

  if (ConversionType == CONVERT_TO_FIXED_POINT_VOLTS) {
    ///
    /// Input Voltage is in INT16 representation. Check if numenr is negative
    ///
    if ( (InputVoltageOffset & INT16_SIGN_BIT_MASK) != 0) {
      NumIsNegative = TRUE;
      ///
      /// Need to 2's complement adjust to make this number positive for
      /// voltage calculation
      ///
      InputVoltageOffset = (~InputVoltageOffset+1) & (INT16_SIGN_BIT_MASK -1);
    }

    ///
    /// Clip the input Voltage Offset to 500mv
    ///
    if (InputVoltageOffset > MAX_OFFSET_MV) {
      InputVoltageOffset = MAX_OFFSET_MV;
    }

    ///
    /// Convert to fixed point representation
    ///
    *OutputVoltageOffset = (InputVoltageOffset * 1024)/ MILLIVOLTS_PER_VOLT;
    if (NumIsNegative) {
      /// 2's complement back to a negative number
      *OutputVoltageOffset = ~(*OutputVoltageOffset) + 1;
    }
  } else if (ConversionType == CONVERT_TO_BINARY_MILLIVOLT) {
    ///
    /// Input Voltage is in fixed point representation. Check if number negative
    ///
    if ( (InputVoltageOffset & FIXED_POINT_SIGN_BIT_MASK)!= 0) {
      NumIsNegative = TRUE;
      ///
      /// Need to 2's complement adjust to make this number positive for
      /// voltage calculation
      ///
      InputVoltageOffset = (~InputVoltageOffset+1) & (FIXED_POINT_SIGN_BIT_MASK -1);
    }

    ///
    ///  Convert to INT16 representation in millivolts
    ///
    *OutputVoltageOffset = (InputVoltageOffset * MILLIVOLTS_PER_VOLT)/1024;
    if (NumIsNegative) {
      /// 2's complement back to a negative number
      *OutputVoltageOffset = ~(*OutputVoltageOffset) + 1;
    }
  }

  return;
}

VOID
ConvertToMailboxFormat (
  IN VOID             *InputData,
  OUT OC_MAILBOX_ITEM *MailboxData,
  IN UINT32           CommandId
  )
/**
  Converts the input data to valid mailbox command format based on CommandID

@param[IN]  InputData
@param[OUT] *MailboxData
@param[IN]  CommandId
**/
{
  VOLTAGE_FREQUENCY_ITEM  *VfItem;
  SVID_CONFIG_ITEM        *SvidItem;
  OC_CAPABILITIES_ITEM    *OcCapItem;
  CORE_RATIO_LIMITS_ITEM  *CoreRatioItem;
  GLOBAL_CONFIG_ITEM      *GlobalConfigItem;
  VF_MAILBOX_COMMAND_DATA VfMailboxCommandData;
  UINT16                  TempVoltage;
  UINT32				  MsrValue;
  UINT8					  MaxOcRatio;
 

  ///
  ///  Initialize local varaibles and mailbox data
  ///
  ZeroMem ((UINT32 *)MailboxData, sizeof(OC_MAILBOX_ITEM));

  ///
  /// Then make a decision based on CommandId how to format
  ///
  //DEBUG((EFI_D_INFO, "\nCOMMAND ID :%x\n", CommandId));
  switch (CommandId) {
    case OC_LIB_CMD_GET_OC_CAPABILITIES:
      OcCapItem = (OC_CAPABILITIES_ITEM *) InputData;
      ///
      /// OC Capabilities are returned on a per domain basis
      ///
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_GET_OC_CAPABILITIES;
      MailboxData->Interface.Fields.Param1 = OcCapItem->DomainId;
      break;

    case OC_LIB_CMD_GET_PER_CORE_RATIO_LIMIT:
      CoreRatioItem = (CORE_RATIO_LIMITS_ITEM *) InputData;
      ///
      /// Core Ratio Limits are only valid in the IA Core domain
      ///
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_GET_PER_CORE_RATIO_LIMIT;
      MailboxData->Interface.Fields.Param1 = OC_LIB_DOMAIN_ID_IA_CORE;
      MailboxData->Interface.Fields.Param2 = CoreRatioItem->Index;
      break;

    case OC_LIB_CMD_GET_VOLTAGE_FREQUENCY:
      VfItem = (VOLTAGE_FREQUENCY_ITEM *) InputData;
      ///
      /// Voltage Frequency Settings are on a per domain basis
      ///
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_GET_VOLTAGE_FREQUENCY;
      MailboxData->Interface.Fields.Param1 = VfItem->DomainId;
      break;

    case OC_LIB_CMD_SET_VOLTAGE_FREQUENCY:
      VfItem = (VOLTAGE_FREQUENCY_ITEM *) InputData;
      ///
      /// Voltages are stored in a fixed point format
      ///
      VfMailboxCommandData.MaxOcRatio = 0x00;
      MaxOcRatio = VfItem->VfSettings.MaxOcRatio;

	  // Gan 
	  // copy the MaxOCRatio into MSR 0x620
	  	  
	  //DEBUG((EFI_D_INFO, "readingMSR 0x620\n"));
	  //DEBUG((EFI_D_INFO, "INPUT VALUE :%x\n", MaxOcRatio));
      //DEBUG((EFI_D_INFO, "INPUT VALUE vfI :%x\n",VfItem->VfSettings.MaxOcRatio));
      MsrValue = (UINT32)AsmReadMsr64 (PCU_CR_RATIO_MSR);
	  DEBUG((EFI_D_INFO, "read value MSR 0x620:%x\n", MsrValue));
      VfMailboxCommandData.MaxOcRatio = MaxOcRatio;

	  //DEBUG((EFI_D_INFO, "write value MSR 0x620:%x\n", MsrValue));
	  //write to PCU_CR_RATIO_MSR only for the RING/CLR
	  if(VfItem->DomainId == OC_LIB_DOMAIN_ID_CLR)
	  {
	  MsrValue = ((MsrValue & RATIO_LIMIT_MASK)  | MaxOcRatio);
	  AsmWriteMsr64(PCU_CR_RATIO_MSR,MsrValue);
	  VfMailboxCommandData.MaxOcRatio = (MsrValue & MAX_CLR_MASK);
	  }

	  if((VfItem->DomainId == OC_LIB_DOMAIN_ID_UNCORE)|| (VfItem->DomainId == OC_LIB_DOMAIN_ID_VCCU))
	  VfMailboxCommandData.MaxOcRatio = 0x00;

      TempVoltage = 0;
      ConvertVoltageTarget(VfItem->VfSettings.VoltageTarget, &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
      VfMailboxCommandData.VoltageTargetU12 = TempVoltage;

      VfMailboxCommandData.TargetMode = VfItem->VfSettings.VoltageTargetMode;

      TempVoltage = 0;
      ConvertVoltageOffset(VfItem->VfSettings.VoltageOffset, (INT16 *) &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
      VfMailboxCommandData.VoltageOffsetS11 = TempVoltage;

      CopyMem(&MailboxData->Data, &VfMailboxCommandData, sizeof(VfMailboxCommandData));
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_SET_VOLTAGE_FREQUENCY;
      MailboxData->Interface.Fields.Param1 = VfItem->DomainId;
      break;

    case OC_LIB_CMD_GET_SVID_CONFIG:
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_GET_SVID_CONFIG;
      MailboxData->Interface.Fields.Param1 = 0;
      break;

    case OC_LIB_CMD_SET_SVID_CONFIG:
      SvidItem = (SVID_CONFIG_ITEM *) InputData;

	  if(SvidItem->SvidDisable)
	  TempVoltage = SVID_DEFAULT_VCC;
	  else
      ConvertVoltageTarget(SvidItem->VoltageTarget, &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);

      MailboxData->Data = TempVoltage | (SvidItem->SvidDisable << SVID_DISABLE_OFFSET);
	  //DEBUG ((EFI_D_ERROR, "(OC MAILBOX) SET SVID Data :%x\n",MailboxData->Data ));
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_SET_SVID_CONFIG;
      MailboxData->Interface.Fields.Param1 = 0;
      break;

    case OC_LIB_CMD_GET_GLOBAL_CONFIG:
      MailboxData->Data = 0;
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_GET_GLOBAL_CONFIG;
      MailboxData->Interface.Fields.Param1 = 0;

    case OC_LIB_CMD_SET_GLOBAL_CONFIG:
      GlobalConfigItem = (GLOBAL_CONFIG_ITEM *) InputData;
      MailboxData->Data =
              (GlobalConfigItem->DisableFivrFaults & BIT0_MASK) |
              ((GlobalConfigItem->DisableFivrEfficiency & BIT0_MASK) << FIVR_EFFICIENCY_OFFSET);
      MailboxData->Interface.Fields.CommandCompletion = OC_LIB_CMD_SET_GLOBAL_CONFIG;
      MailboxData->Interface.Fields.Param1 = 0;
      break;

    default:
     // DEBUG ((EFI_D_ERROR, "(OC MAILBOX) Unkown Command ID\n"));

      break;

  }

}

EFI_STATUS
EFIAPI
MailboxWrite (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  IN UINT32  MailboxData,
  OUT UINT32 *MailboxStatus
  )
/**
  Generic Mailbox function for mailbox write commands. This function will
  poll the mailbox interface for control, issue the write request, poll
  for completion, and verify the write was succussful.

  @param[IN]  MailboxType,
  @param[IN]  MailboxCommand,
  @param[IN]  MailboxData,
  @param[OUT] *MailboxStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS         Status;
  UINT64             MsrData;
  UINT32             MchBar;
  OC_MAILBOX_FULL    OcMailboxFull;
  OC_MAILBOX_FULL    OcMailboxFullVerify;
  PCODE_MAILBOX_FULL PcodeMailboxFull;
  PCODE_MAILBOX_FULL PcodeMailboxFullVerify;

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = PollMailboxReady(MailboxType);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //DEBUG ((EFI_D_INFO, "(MAILBOX) Mailbox Write Command = %2X\n", (UINT8)MailboxCommand));

  switch (MailboxType)
  {
    case MAILBOX_TYPE_PCODE:
      ///
      /// Copy in Mailbox data and write the PCODE mailbox DATA field
      ///
      PcodeMailboxFull.Interface.InterfaceData = MailboxCommand;
      PcodeMailboxFull.Data = MailboxData;
      MchBar = (MmioRead32 (MmPciAddress (0, 0, 0, 0, MCHBAR_OFFSET)) &~BIT0);

      MmioWrite32 ( (MchBar + PCODE_MAILBOX_DATA_OFFSET), PcodeMailboxFull.Data);

      ///
      /// Set the Run/Busy bit to signal mailbox data is ready to process
      ///
      PcodeMailboxFull.Interface.Fields.RunBusy = 1;
      MmioWrite32 ( (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET), PcodeMailboxFull.Interface.InterfaceData);

      ///
      /// Poll run/busy to indicate the completion of write request
      ///
      PollMailboxReady(MailboxType);

      ///
      /// Read the BIOS PCODE mailbox to verify write completion success.
      /// Mailbox protocol requires software to read back the interface twice
      /// to ensure the read results are consistent.
      ///
      PcodeMailboxFull.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
      PcodeMailboxFull.Data = MmioRead32 (MchBar + PCODE_MAILBOX_DATA_OFFSET);

      PchPmTimerStall(MAILBOX_READ_TIMEOUT);

      ///
      /// Read twice to verify data is consitent
      ///
      PcodeMailboxFullVerify.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
      PcodeMailboxFullVerify.Data = MmioRead32 (MchBar + PCODE_MAILBOX_DATA_OFFSET);

      ///
      /// If the data is inconsistent, we cannot trust the results
      ///
      if (PcodeMailboxFull.Interface.InterfaceData != PcodeMailboxFullVerify.Interface.InterfaceData) {
        if (PcodeMailboxFull.Data  != PcodeMailboxFullVerify.Data) {
          //DEBUG ((EFI_D_ERROR, "(MAILBOX) Mailbox read data is corrupted.\n"));
          return EFI_INVALID_PARAMETER;
        }
      }

      ///
      ///  Copy PCODE mailbox completion code
      ///
      *MailboxStatus = (UINT32) PcodeMailboxFull.Interface.Fields.Command;
      break;

    case MAILBOX_TYPE_OC:
      ///
      /// Set the Run/Busy bit to signal mailbox data is ready to process
      ///
      OcMailboxFull.Interface.InterfaceData = MailboxCommand;
      OcMailboxFull.Data = MailboxData;
      OcMailboxFull.Interface.Fields.RunBusy = 1;
      CopyMem (&MsrData, &OcMailboxFull, sizeof(MsrData));

      ///
      /// Write mailbox command to OC mailbox
      ///
      AsmWriteMsr64 (OC_MAILBOX_MSR, MsrData);

      ///
      /// Poll run/busy to indicate the completion of write request
      ///
      PollMailboxReady(MailboxType);

      ///
      /// Read the mailbox command from OC mailbox. Read twice to ensure data.
      ///
      MsrData = AsmReadMsr64 (OC_MAILBOX_MSR);
      CopyMem (&OcMailboxFull, &MsrData, sizeof(OcMailboxFull));

      PchPmTimerStall(MAILBOX_READ_TIMEOUT);

      MsrData = AsmReadMsr64 (OC_MAILBOX_MSR);
      CopyMem (&OcMailboxFullVerify, &MsrData, sizeof(OcMailboxFullVerify));

      ///
      /// If the data is inconsistent, we cannot trust the results
      ///
      if (OcMailboxFull.Interface.InterfaceData != OcMailboxFullVerify.Interface.InterfaceData) {
        if (OcMailboxFull.Data != OcMailboxFullVerify.Data) {
          //DEBUG ((EFI_D_ERROR, "(MAILBOX) Mailbox read data is corrupted.\n"));
          return EFI_INVALID_PARAMETER;
        }
      }

      ///
      ///  Copy Overclocking mailbox completion code and read results
      ///
      *MailboxStatus = OcMailboxFull.Interface.Fields.CommandCompletion;
      break;

    default:
      //DEBUG ((EFI_D_ERROR, "(MAILBOX) Unrecognized Mailbox Type.\n"));
      Status = EFI_UNSUPPORTED;
      break;
  }

  //DEBUG ((EFI_D_INFO, "(MAILBOX) Mailbox Status = %2X\n", *MailboxStatus));
  return Status;
}

EFI_STATUS
EFIAPI
MailboxRead (
  IN UINT32  MailboxType,
  IN UINT32  MailboxCommand,
  OUT UINT32 *MailboxDataPtr,
  OUT UINT32 *MailboxStatus
  )
/**
  Generic Mailbox function for mailbox read commands. This function will write
  the read request, and populate the read results in the output data.

  @param[IN]  MailboxType,
  @param[IN]  MailboxCommand,
  @param[OUT] *MailboxDataPtr,
  @param[OUT] *MailboxStatus

  @retval EFI_STATUS
**/
{
  EFI_STATUS         Status;
  UINT64             MsrData;
  UINT32             MchBar;
  PCODE_MAILBOX_FULL PcodeMailboxFull;
  PCODE_MAILBOX_FULL PcodeMailboxFullVerify;
  OC_MAILBOX_FULL    OcMailboxFull;
  OC_MAILBOX_FULL    OcMailboxFullVerify;

  ///
  ///  Poll the run/busy to ensure the interface is available
  ///
  Status = PollMailboxReady(MailboxType);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  //DEBUG ((EFI_D_INFO, "(MAILBOX) Mailbox Read Command = %2X\n", (UINT8)MailboxCommand));

  switch (MailboxType)
  {
    case MAILBOX_TYPE_PCODE:
      ///
      /// Write the PCODE mailbox read request.
      /// Read requests only require a write to the PCODE interface mailbox.
      /// The read results will be updated in the data mailbox.
      ///
      PcodeMailboxFull.Interface.InterfaceData = MailboxCommand;
      PcodeMailboxFull.Interface.Fields.RunBusy = 1;
      MchBar = (MmioRead32 (MmPciAddress (0, 0, 0, 0, MCHBAR_OFFSET)) &~BIT0);

      MmioWrite32 ( (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET), PcodeMailboxFull.Interface.InterfaceData);

      ///
      /// Poll run/busy to indicate the completion of read request
      ///
      PollMailboxReady(MailboxType);

      ///
      /// Read the BIOS PCODE mailbox to verify read completion success.
      /// Mailbox protocol requires software to read back the interface twice
      /// to ensure the read results are consistent.
      ///
      PcodeMailboxFull.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
      PcodeMailboxFull.Data = MmioRead32 (MchBar + PCODE_MAILBOX_DATA_OFFSET);

      PchPmTimerStall(MAILBOX_READ_TIMEOUT);

      ///
      /// Read twice to verify data is consitent
      ///
      PcodeMailboxFullVerify.Interface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
      PcodeMailboxFullVerify.Data = MmioRead32 (MchBar + PCODE_MAILBOX_DATA_OFFSET);

      ///
      /// If the data is inconsistent, we cannot trust the results
      ///
      if (PcodeMailboxFull.Interface.InterfaceData != PcodeMailboxFullVerify.Interface.InterfaceData) {
        if (PcodeMailboxFull.Data  != PcodeMailboxFullVerify.Data) {
         // DEBUG ((EFI_D_ERROR, "(MAILBOX) Mailbox read data is corrupted.\n"));
          return EFI_INVALID_PARAMETER;
        }
      }

      ///
      ///  Copy PCODE mailbox completion code and read results
      ///
      *MailboxStatus = (UINT32) PcodeMailboxFull.Interface.Fields.Command;
      CopyMem(MailboxDataPtr, &PcodeMailboxFull.Interface.InterfaceData, sizeof(UINT32));
      break;

    case MAILBOX_TYPE_OC:
      ///
      /// Set the Run/Busy bit to signal mailbox data is ready to process
      ///
      OcMailboxFull.Interface.InterfaceData = MailboxCommand;
      OcMailboxFull.Data = *MailboxDataPtr;
      OcMailboxFull.Interface.Fields.RunBusy = 1;
      CopyMem (&MsrData, &OcMailboxFull, sizeof(MsrData));

      ///
      /// Write mailbox command to OC mailbox
      ///
      AsmWriteMsr64 (OC_MAILBOX_MSR, MsrData);

      ///
      /// Poll run/busy to indicate the completion of write request
      ///
      PollMailboxReady(MailboxType);

      ///
      /// Read the OC mailbox to verify read completion success.
      /// Mailbox protocol requires software to read back the interface twice
      /// to ensure the read results are consistent.
      ///
      MsrData = AsmReadMsr64 (OC_MAILBOX_MSR);
      CopyMem (&OcMailboxFull, &MsrData, sizeof(OcMailboxFull));

      PchPmTimerStall(MAILBOX_READ_TIMEOUT);

      MsrData = AsmReadMsr64 (OC_MAILBOX_MSR);
      CopyMem (&OcMailboxFullVerify, &MsrData, sizeof(OcMailboxFullVerify));

      ///
      /// If the data is inconsistent, we cannot trust the results
      ///
      if (OcMailboxFull.Interface.InterfaceData != OcMailboxFullVerify.Interface.InterfaceData) {
        if (OcMailboxFull.Data != OcMailboxFullVerify.Data) {
          //DEBUG ((EFI_D_ERROR, "(MAILBOX) Mailbox read data is corrupted.\n"));
          return EFI_INVALID_PARAMETER;
        }
      }

      ///
      ///  Copy Overclocking mailbox completion code and read results
      ///
      *MailboxStatus = OcMailboxFull.Interface.Fields.CommandCompletion;
      CopyMem(MailboxDataPtr, &OcMailboxFull.Data, sizeof(UINT32));
      break;

    default:
      //DEBUG ((EFI_D_ERROR, "(MAILBOX) Unrecognized Mailbox Type.\n"));
      Status = EFI_UNSUPPORTED;
      break;
  }

  //DEBUG ((EFI_D_INFO, "(MAILBOX) Mailbox Status = %2X\n", *MailboxStatus));

  return Status;
}

/**
  Poll the run/busy bit of the mailbox until available or timeout expires.

  @param[IN]  MailboxType,

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
PollMailboxReady (
  IN UINT32 MailboxType
  )
{
  EFI_STATUS              Status;
  UINT16                  StallCount;
  UINT8                   RunBusyBit;
  UINT64                  MsrData;
  UINT32                  MchBar;
  OC_MAILBOX_FULL         OcMailboxFull;
  PCODE_MAILBOX_INTERFACE PcodeMailboxInterface;

  Status = EFI_SUCCESS;
  StallCount = 0;
  RunBusyBit = 1;

  do {
    switch (MailboxType)
    {
      case MAILBOX_TYPE_PCODE:
        ///
        /// Read the MMIO run/busy state
        ///
        MchBar = (MmioRead32 (MmPciAddress (0, 0, 0, 0, MCHBAR_OFFSET)) &~BIT0);
        PcodeMailboxInterface.InterfaceData = MmioRead32 (MchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
        RunBusyBit = (UINT8) PcodeMailboxInterface.Fields.RunBusy;
        break;

      case MAILBOX_TYPE_OC:
        ///
        /// Read the OC mailbox run/busy state
        ///
        MsrData = AsmReadMsr64(OC_MAILBOX_MSR);
        CopyMem(&OcMailboxFull.Data, &MsrData, sizeof(OcMailboxFull));
        RunBusyBit = OcMailboxFull.Interface.Fields.RunBusy;
        break;
    }
    //
    // Wait for 1us
    //
    PchPmTimerStall(MAILBOX_WAIT_STALL);
    StallCount++;
  }
  while ((RunBusyBit == 1) && (StallCount < MAILBOX_WAIT_TIMEOUT));

  if ((RunBusyBit == 1) && (StallCount == MAILBOX_WAIT_TIMEOUT)) {
    //DEBUG ((EFI_D_ERROR, "(MAILBOX) Mailbox interface timed out.\n"));
    Status = EFI_TIMEOUT;
  }
  return Status;
}

/**
  Based on ResetType, perform warm or cold reset using PCH Reset PPI

  @param[in] PeiServices       - Indirect reference to the PEI Services Table.
  @param[in] ResetType         - CPU_RESET_TYPE to indicate which reset shoudl be performed.

  @exception EFI_UNSUPPORTED - Reset type unsupported
  @retval EFI_SUCCESS     - function successfully (system should already reset)
**/
EFI_STATUS
PerformWarmORColdReset (
  IN EFI_PEI_SERVICES CONST **PeiServices,
  IN CPU_RESET_TYPE         ResetType
  )
{
  PCH_RESET_PPI *PchResetPpi;
  EFI_STATUS    Status;

  ///
  /// Loccate Reset PPI
  ///
  Status = (*PeiServices)->LocatePpi (
                  PeiServices,
                  &gPchResetPpiGuid,
                  0,
                  NULL,
                  (VOID **) &PchResetPpi
                  );

  ASSERT_EFI_ERROR (Status);

  ///
  /// Perfrom the requested reset using PCH reset PPI
  ///
  Status = EFI_SUCCESS;
  switch (ResetType) {
    case WARM_RESET:
      PchResetPpi->Reset (PchResetPpi, RESET_PPI_WARM_RESET);
      break;

    case COLDRESET:
      PchResetPpi->Reset (PchResetPpi, RESET_PPI_COLD_RESET);
      break;

    default:
      //DEBUG ((EFI_D_ERROR, "CpuInitPeim: PerformWarmORColdReset - ResetType %d not supported: \n", ResetType));
      Status = EFI_UNSUPPORTED;
      ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

VOID
VccioOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_SMBUS2_PPI                    *Smbus;

  EFI_SMBUS_DEVICE_ADDRESS              SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND              Command;
  EFI_PEI_STALL_PPI                     *StallPpi;
  UINTN                                 SmbusLength;
  UINT8                                 Value = 0x00;

  //
  // Locate SMBUS function.
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiSmbus2PpiGuid,
                                      0,
                                      NULL,
                                      &Smbus);

  //
  // Locate Stall function.
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiStallPpiGuid,
                                      0,
                                      NULL,
                                      &StallPpi);

  //
  // Program VccIO VR
  //
  SlaveAddress.SmbusDeviceAddress = EFI_VCCIO_SMBUS_ADDRESS;
  Command = EFI_VCCIO_SMBUS_REG; 
  SmbusLength = sizeof(Value);
  Value = SystemConfiguration->VccIoVoltage;

  Status = Smbus->Execute (Smbus,
                           SlaveAddress,
                           Command,
                           EfiSmbusWriteWord,
                           FALSE,
                           &SmbusLength,
                           &Value);

  //
  // Give some delay (may not be needed)
  //
  StallPpi->Stall (PeiServices, StallPpi, 5000);
}

VOID
VsmOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_SMBUS2_PPI                    *Smbus;

  EFI_SMBUS_DEVICE_ADDRESS              SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND              Command;
  EFI_PEI_STALL_PPI                     *StallPpi;
  UINTN                                 SmbusLength;
  UINT16                                Value = 0x0000;


  //
  // Second locate SMBUS access function.
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiSmbus2PpiGuid,   // GUID
                                      0,                   // INSTANCE
                                      NULL,                // EFI_PEI_PPI_DESCRIPTOR
                                      &Smbus               // PPI
                                     );

  //
  // Prepare fixed delay service
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiStallPpiGuid,
                                      0,
                                      NULL,
                                      &StallPpi
                                     );

  //
  // To override Vsm Voltage(s), write max Vout for the controller and then send Gamer Mode
  // appropriately, else ensure Gamer Mode is Disabled
  //
  if ( SystemConfiguration->C01MemoryVoltage ) {

	//
	// Now program Gamer Mode based upon user selection
	//
	Value = 0;
	Value |= EFI_VR_GAMER_MODE_EN_MASK;

	// 
	// Since we know C01MemoryVoltage is non-default, write in user selection.
	//
	Value |= (SystemConfiguration->C01MemoryVoltage << 8);

	if (SystemConfiguration->C01MemoryVoltage & BIT8) {
	  Value |= BIT0;
	}

	//
	// Spec states this Reserved Bit must be set.
	//
	Value |= EFI_VR_RESERVED;

	//
	// Now write the collected 'Value' to the VR Controller
	//
	SlaveAddress.SmbusDeviceAddress = EFI_C01VSM_SMBUS_ADDRESS;
	Command = EFI_VR_GAMER_CMD;
	SmbusLength = sizeof(Value);

	//
	// Send a word to VR Controller
	//

	Status = Smbus->Execute (
							Smbus,
							SlaveAddress,
							Command,
							EfiSmbusWriteWord,
							FALSE,
							&SmbusLength,
							&Value
							);

  }

  if ( SystemConfiguration->C23MemoryVoltage ) {

	//
	// Now program Gamer Mode based upon user selection
	//
	Value = 0;
	Value |= EFI_VR_GAMER_MODE_EN_MASK;

	// 
	// Since we know C23MemoryVoltage is non-default, write in user selection.
	//
	Value |= (SystemConfiguration->C23MemoryVoltage << 8);

	if (SystemConfiguration->C23MemoryVoltage & BIT8) {
	  Value |= BIT0;
	}

	//
	// Spec states this Reserved Bit must be set.
	//
	Value |= EFI_VR_RESERVED;

	//
	// Now write the collected 'Value' to the VR Controller
	//
	SlaveAddress.SmbusDeviceAddress = EFI_C23VSM_SMBUS_ADDRESS;
	Command = EFI_VR_GAMER_CMD;
	SmbusLength = sizeof(Value);

	//
	// Send a word to VR Controller
	//

	Status = Smbus->Execute (
							Smbus,
							SlaveAddress,
							Command,
							EfiSmbusWriteWord,
							FALSE,
							&SmbusLength,
							&Value
							);

  }
}

VOID
CpuVccInOverride (
  IN EFI_PEI_SERVICES CONST  **PeiServices,
  IN SYSTEM_CONFIGURATION    *SystemConfiguration
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_SMBUS2_PPI                     *Smbus;

  EFI_SMBUS_DEVICE_ADDRESS              SlaveAddress;
  EFI_SMBUS_DEVICE_COMMAND              Command;
  EFI_PEI_STALL_PPI                     *StallPpi;
  UINTN                                 SmbusLength;
  UINT16                                Value = 0x0000;


  //
  // Second locate SMBUS access function.
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiSmbus2PpiGuid,   // GUID
                                      0,                   // INSTANCE
                                      NULL,                // EFI_PEI_PPI_DESCRIPTOR
                                      &Smbus               // PPI
                                     );

  //
  // Prepare fixed delay service
  //
  Status = (**PeiServices).LocatePpi (PeiServices,
                                      &gEfiPeiStallPpiGuid,
                                      0,
                                      NULL,
                                      &StallPpi
                                     );

  //
  // To override VCCin Voltage(s), write max Vout for the controller and then send Gamer Mode
  // appropriately, else ensure Gamer Mode is Disabled
  //



  if ( SystemConfiguration->CpuVccInVoltage && (!SystemConfiguration->SvidEnable)) {

	//
	// Now program Gamer Mode based upon user selection
	//
	Value = 0;
	Value |= EFI_VR_GAMER_MODE_EN_MASK;

	// 
	// Since we know CpuVccInVoltage is non-default, write in user selection.
	//
	Value |= (SystemConfiguration->CpuVccInVoltage << 8);

	if (SystemConfiguration->CpuVccInVoltage & BIT8) {
	  Value |= BIT0;
	}

	//
	// Spec states this Reserved Bit must be set.
	//
	Value |= EFI_VR_RESERVED;

	//
	// Now write the collected 'Value' to the VR Controller
	//
	SlaveAddress.SmbusDeviceAddress = EFI_VCCIN_SMBUS_ADDRESS;
	Command = EFI_VR_GAMER_CMD;
	SmbusLength = sizeof(Value);

	//
	// Send a word to VR Controller
	//

	Status = Smbus->Execute (
							Smbus,
							SlaveAddress,
							Command,
							EfiSmbusWriteWord,
							FALSE,
							&SmbusLength,
							&Value
							);

  }
}


//
// 
// ----------------------------------------------------------------------------
// Procedure: PerformBclkOcFlow
//
// Description: This function will perform the BCLK overclocking flow which 
//              handles FilterPll, PciePll, and DMI/PEG ratio programming.
//
// Input:
//      IN  UINT32                   **PeiServices
//      IN  UINT32                   *SetupData
// Output:
//       EFI_SUCCESS on success
//       Appropiate failure code on error.
//
// ----------------------------------------------------------------------------
// 
//
EFI_STATUS
PerformBclkOcFlow (
  IN  EFI_PEI_SERVICES CONST  **PeiServices,
  IN  SYSTEM_CONFIGURATION    *SystemConfiguration
  )
/*++

Routine Description:


Arguments:


Returns:


--*/
{
  UINT64            MsrData;
  UINT32            FwStatus3;
  UINT32            GenPMCon;
  UINT8             PrevDmiPegRatio;
  UINT8             FwDmiRatio;
  UINT8             PciePll;
  UINT8             S3Scratchpad;
  //PCH_RESET_PPI     *PchResetPpi;
  //PCH_RESET_TYPE    PchResetType;
  EFI_STATUS        Status;
  UINT32            MePresent;
  UINT32            FwStatus;
  UINT32            BootMode;

  Status = EFI_SUCCESS;
  MePresent = 0;
  S3Scratchpad = 0;

  //
  // Start with the Filter Pll programming because if this changes, the system
  // will need to power cylce before we can proceed.
  //
  
  //
  // Filter PLL programming selects between the Hi-Speed (default) and 
  // Lo-Speed filter, intended for use when overclocking BCLK.
  //
//Not needed if part defaults properly.  ProgramFilterPll(PeiServices, SystemConfiguration);

  //
  // Check the Pcie Pll selection is up to date
  //

  MsrData = AsmReadMsr64 (MSR_PCIE_PLLGEN3);
  //DEBUG ((EFI_D_INFO, "(ICC) MSR 0x61E:%X\n", MsrData));

  PciePll = (UINT8) RShiftU64((MsrData & PCIE_PLL_MASK), PCIE_PLL_OFFSET);
  //DEBUG ((EFI_D_INFO, "(ICC) PCIe PLL from MSR 0x61E:%X\n", PciePll));

  //
  // Get the DMI Ratio stored in FWStatus3 bits 2:0
  //
  MePresent = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x0));
  //DEBUG ((EFI_D_INFO, "(ICC) Read B/D/F:%X/%X/%X Register offset 0x0 to get ME Presence:%X\n", ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, MePresent));

  //
  // Dump FW Status registers for debug
  //
  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x40));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS1: AUX 0x10005, PCI 0:22:0 offset 0x40:%X\n", FwStatus));

  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x48));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS2: AUX 0x10011, PCI 0:22:0 offset 0x48:%X\n", FwStatus));

  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x60));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS3: AUX 0x10013, PCI 0:22:0 offset 0x60:%X\n", FwStatus));

  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x64));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS4: AUX 0x10014, PCI 0:22:0 offset 0x64:%X\n", FwStatus));

  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x68));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS5: AUX 0x10015, PCI 0:22:0 offset 0x68:%X\n", FwStatus));

  FwStatus = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x6C));
  //DEBUG ((EFI_D_INFO, "(ICC) FWSTS6: AUX 0x10016, PCI 0:22:0 offset 0x6C:%X\n", FwStatus));


  if(MePresent != 0xFFFFFFFF) {
    FwStatus3 = PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, FWSTATUS3_OFFSET));
    FwDmiRatio = (UINT8)(FwStatus3 & FW_DMI_RATIO_MASK);
    PrevDmiPegRatio = (UINT8)(MsrData & PEG_DMI_RATIO_MASK);
    DEBUG ((EFI_D_INFO, "(ICC) CPU DMI ratio:%X, PCH DMI ratio:%X\n",  PrevDmiPegRatio, FwDmiRatio));

    //
    // Check the S3 scratch pad bit indicating an S3 exit occured.  If set then
    // adjust the boot mode. This is required for correct MRC execution path.
    //

	/* Disable S3 flow for now
    ReadCmos (CMOS_OC_S3_SCRATCHPAD, &S3Scratchpad);
    DEBUG ((EFI_D_INFO, "(ICC) S3Scratchpad Read:%X\n",  S3Scratchpad));
    if (S3Scratchpad) {
      DEBUG((EFI_D_INFO, "(ICC) Forcing the S3 Resume Path after HSW OC Warm Reset\n"));
      Status = (*PeiServices)->SetBootMode(PeiServices, BOOT_ON_S3_RESUME);
      ASSERT_EFI_ERROR(Status);
      // Now clear the flag.  At this point platform should now be overclocking.
      ASSERT_EFI_ERROR(FwDmiRatio == PrevDmiPegRatio);
      S3Scratchpad = 0;
      DEBUG ((EFI_D_INFO, "(ICC) S3Scratchpad Clear\n"));
      WriteCmos (CMOS_OC_S3_SCRATCHPAD, &S3Scratchpad);
    }
	*/


    //
    // Detect DMI/PEG Ratio change
    //
    if (FwDmiRatio != PrevDmiPegRatio) {
      UINT32 Msg; // Variable for reading/writing the Host to ME and ME to Host messaging.
      UINT32 MsgTimeout; // Counter used as a timeout value while waiting for ME to acknowledge the ramp flag.
  	  EFI_PEI_STALL_PPI     *StallPpi;

      //
      // Program new DMI/Peg Ratio through MSR 0x61E
      //
      MsrData |= (UINT64)FwDmiRatio;

      //
      // Set the Long Reset Bit in MSR 0x61E
      //
      MsrData |= LONG_RESET_BIT;
	  
      AsmWriteMsr64 (MSR_PCIE_PLLGEN3, MsrData);

      GenPMCon = PciRead32(PCI_LIB_ADDRESS(0x0, 0x1F, 0x0, 0xA0));

      //DEBUG((EFI_D_INFO, "(ICC) GEN_PMCON_12=0x%04X SR=%d DISB=%d\n", GenPMCon,
      //                   ((GenPMCon & GEN_PMCON_12_SR_MASK)   ? 1:0),
       //                  ((GenPMCon & GEN_PMCON_12_DISB_MASK) ? 1:0)));

      // If memory is not in self refresh, then clear the DRAMInit'd flag, because SR flag sets automatically
      // on Platform Warm Reset and causes MRC to restore memory as though it was in self refresh.
      if ((GenPMCon & GEN_PMCON_12_SR_MASK) == 0) {
        PciWrite32 (PCI_LIB_ADDRESS(0x0, 0x1F, 0x0, 0xA0), GenPMCon & (~GEN_PMCON_12_DISB_MASK));
        //DEBUG ((EFI_D_INFO, "(ICC) Cleared DRAM Initialized Flag 0x%08X->0x%08X\n", GenPMCon,
        //PciRead32(PCI_LIB_ADDRESS(0x0, 0x1F, 0x0, 0xA0))));
      }

      Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
      ASSERT_EFI_ERROR (Status);

      //
      // If performing an S3 exit, set a flag that can be used to force the proper boot flow after the HSW OC Warm reset below.
      //
      if (BootMode == BOOT_ON_S3_RESUME) {
        //DEBUG((EFI_D_INFO, "(ICC) Setting the OC S3 bit to force S3 after warm reset\n"));

        //
        // Set Flag in OC WDT scratch pad that won't change on warm reset.
        //
        S3Scratchpad = 1;
        //DEBUG ((EFI_D_INFO, "(ICC) S3Scratchpad Write:%X\n",  S3Scratchpad));
        WriteCmos (CMOS_OC_S3_SCRATCHPAD, &S3Scratchpad);
      }

      Status = PeiServicesLocatePpi(&gEfiPeiStallPpiGuid,0,NULL,(VOID **) &StallPpi);
      ASSERT_EFI_ERROR (Status);

      // Set the Host To ME flag indicating a ICC clock ramp should be performed on next warm reset. 
      Msg = H2M_ICC_MESSAGE | H2M_BCLK_RAMP_EN_BIT;
      PciWrite32(PCI_LIB_ADDRESS(ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, H2M_MSG1_OFFSET), Msg);
      // Wait for the acknowledge from the FW, otherwise FW may not perform the ramp.
      MsgTimeout = 100000;  // Wait max of 100ms for FW to acknowledge. 
      do {
        StallPpi->Stall(PeiServices, StallPpi, 1); // Delay 1us.  Need to give some time for ME to respond.
        Msg = PciRead32(PCI_LIB_ADDRESS(ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, FWSTATUS1_OFFSET));
        MsgTimeout--;
        if (MsgTimeout == 0) {
          //DEBUG ((EFI_D_INFO, "(ICC) ME FW failed to acknowledge the BCLK Ramp message.\n"));
          Status = RETURN_TIMEOUT;
          ASSERT_EFI_ERROR(Status);
          return Status;
        }
      } while ((Msg & M2H_MSG_ACK_MASK) != M2H_ICC_MSG_ACK);

      //DEBUG((EFI_D_INFO, "(ICC) ME FW acknowledged the BCLK Ramp message (%d).\n", MsgTimeout));
      //
      // Start the Watchdog Timer
      //

	  /* Disable Reset
      StartTimer (PeiServices, WDT_TIMEOUT);

      //
      // Cold reset is required for DMI ratio changes. When the long reset bit
      // is set during a cold reset, this triggers the BCLK ramping algorithm 
      // in FW/PMC. We do not want a power cycle for responsiveness reasons.
      //
	  
      DEBUG ((EFI_D_INFO, "(ICC) Performing Cold Reset for DMI/PEG Ratio change.\n"));
      Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gPchResetPpiGuid,
                                0,
                                NULL,
                                &PchResetPpi
                                );
      
      ASSERT_EFI_ERROR (Status);
      
      PchResetType = ColdReset;

	  DEBUG ((EFI_D_INFO, "(ICC) Did DMI/PEG Ratio programming, now to reset to lock it in.\n"));

      PchResetPpi->Reset (PchResetPpi, PchResetType);
      */


	  

      //
      // We should never get here
      //
    }
  } else {
	//DEBUG ((EFI_D_INFO, "(ICC) Skipped DMI/PEG Ratio programming.\n"));
  }

  return Status;
}

//
// 
// ----------------------------------------------------------------------------
// Procedure: ProgramFilterPll
//
// Description: This function will program the Filter Pll if needed.
//
// Input:
//      IN  UINT32                   **PeiServices
//      IN  UINT32                   *SystemConfiguration
// Output:
//       EFI_SUCCESS on success
//       Appropiate failure code on error.
//
// ----------------------------------------------------------------------------
// 
//
//kap EFI_STATUS
//kap ProgramFilterPll (
//kap   IN  EFI_PEI_SERVICES CONST  **PeiServices,
//kap   IN  SYSTEM_CONFIGURATION    *SystemConfiguration
//kap   )
/*++

Routine Description:


Arguments:


Returns:


--*/
//kap {
//kap   EFI_STATUS      Status;
//kap   UINTN           PciD31F0RegBase;
//kap   UINT32          RootComplexBar;
//kap   UINT8           PmCfgData;
//kap   UINT8           FilterPll;
//kap   UINT32          GpioUseSel;
//kap   UINT32          GpioRstSel;  
//kap   PCH_RESET_PPI   *PchResetPpi;
//kap   PCH_RESET_TYPE  PchResetType;
//kap 
//kap   Status = EFI_SUCCESS;
//kap 
//kap   //
//kap   // Get current Filter PLL value from hardware. PMCFG 0/31/0 offset 0x3318 bit5
//kap   //
//kap   PciD31F0RegBase = MmPciAddress (
//kap                       0,
//kap                       0,
//kap                       PCI_DEVICE_NUMBER_PCH_LPC,
//kap                       PCI_FUNCTION_NUMBER_PCH_LPC,
//kap                       0
//kap                       );
//kap   RootComplexBar  = MmioRead32 (PciD31F0RegBase + R_PCH_LPC_RCBA) & B_PCH_LPC_RCBA_BAR;
//kap   PmCfgData = MmioRead8 ((UINTN) (RootComplexBar + R_PCH_RCRB_PM_CFG));
//kap   FilterPll = (PmCfgData & CPU_FILTER_PLL_MASK)>>CPU_FILTER_PLL_OFFSET;
//kap 
//kap   if (FilterPll != SystemConfiguration->FilterPll ) {
//kap     //
//kap     // We need to program the Filter PLL through GPIO8_OCS
//kap     //  1. Set the GPIO[8] to native mode '0'
//kap     //  2. Set GPIO[8] Reset select to '1'
//kap     //  3. Write new Filter PLL value in PM_CFG
//kap     //  4. Start PCH/OC WDT
//kap     //  5. Perform power cycle reset
//kap     //
//kap 
//kap     //
//kap     // Step 1 - Set GPIO[8] to native mode
//kap     //
//kap     GpioUseSel = IoRead32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_USE_SEL);
//kap     GpioUseSel &= ~(CPU_OCS_GPIO_MASK);
//kap     IoWrite32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GPIO_USE_SEL, GpioUseSel);
//kap 
//kap     //
//kap     // Step 2 - Set GPIO[8] reset select
//kap     //
//kap     GpioRstSel = IoRead32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GP_RST_SEL);
//kap     GpioRstSel |= CPU_OCS_GPIO_MASK;
//kap     IoWrite32 (PCH_GPIO_BASE_ADDRESS + R_PCH_GP_RST_SEL, GpioUseSel);
//kap 
//kap     //
//kap     // Step 3 - Write new Filter Pll value in PM_CFG
//kap     //
//kap     DEBUG ((EFI_D_INFO, "(OC) Changing FilterPll = %X\n", SystemConfiguration->FilterPll));
//kap     //Clear the CPUOCS bit
//kap     PmCfgData &= ~CPU_FILTER_PLL_MASK;
//kap     // Set with the current Filter Pll setting
//kap     PmCfgData |= (SystemConfiguration->FilterPll << CPU_FILTER_PLL_OFFSET);
//kap     MmioWrite8 ((UINTN) (RootComplexBar + R_PCH_RCRB_PM_CFG),PmCfgData);
//kap 
//kap     //
//kap     // Step 4 - Start PCH/OC WDT
//kap     //
//kap     Status = StartTimer (PeiServices, WDT_TIMEOUT);
//kap     ASSERT_EFI_ERROR (Status);
//kap 
//kap     //
//kap     // Step 5 - Perform power cycle reset
//kap     //
//kap     Status = PeiServicesLocatePpi (
//kap                &gPchResetPpiGuid,
//kap                0,
//kap                NULL,
//kap                &PchResetPpi
//kap                );
//kap     ASSERT_EFI_ERROR (Status);
//kap 
//kap     DEBUG ((EFI_D_INFO, "(OC) Filter Pll changed - power cycle reset via CF9...\n"));
//kap     PchResetType = PowerCycleReset;
//kap     PchResetPpi->Reset (PchResetPpi, PchResetType);
//kap 
//kap     //
//kap     // Shouldn't reach here
//kap     //
//kap   }
//kap   
//kap   return Status;
//kap }


// Procedure: StartTimer
//
// Description: This function will start the Watchdog Timer.
//
// Input:
//      IN  UINT32                   TimeoutValue
// Output:
//       EFI_SUCCESS on success
//       Appropiate failure code on error.
//
// ----------------------------------------------------------------------------
// 
//
EFI_STATUS
StartTimer (
  IN  EFI_PEI_SERVICES CONST  **PeiServices,
  IN  UINT32                  TimeoutValue
  )
/*++

Routine Description:


Arguments:


Returns:


--*/
{
  EFI_STATUS  Status;
  WDT_PPI     *gWdtPei;

  //
  // Locate WDT_PPI (ICC WDT PPI)
  //
  Status = PeiServicesLocatePpi (
             &gWdtPpiGuid,
             0,
             NULL,
             &gWdtPei
             );
  ASSERT_EFI_ERROR (Status);

  Status = gWdtPei->ReloadAndStart (TimeoutValue);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

EFI_STATUS RingMinRatio(UINT8 Ratio)
{
UINT32 MailBoxCommand;
UINT32 MsrValue = 0x0000;
UINT32 UboxBase = 0x00;
UboxBase = (UINT32)AsmReadMsr64 (0x300);
UboxBase = (UboxBase >> 8) & 0xFF;

if(PciRead32 (PCI_LIB_ADDRESS (UboxBase,0x10,0x07,0x5C )) & BIT5)
{
DEBUG ((EFI_D_ERROR, "(OC) Raise CLR MIN.\n"));

 MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
 while(MailBoxCommand & BIT31)
 {
	MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
 }

 MailBoxCommand = ((Ratio << 8) |0x800000A4);
 MailBoxCommand = ((Ratio << 16) |MailBoxCommand);
 PciWrite32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILDATA),0x00);
 PciWrite32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM),MailBoxCommand);

MsrValue = (UINT32)AsmReadMsr64 (PCU_CR_RATIO_MSR);
MsrValue = ((MsrValue & 0x80FF)  | (Ratio << 8 ));
AsmWriteMsr64(PCU_CR_RATIO_MSR,MsrValue);

//MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
// while(MailBoxCommand & BIT31)
// {
//	MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
 //}
 

}

return EFI_SUCCESS;

}

UINT32 WriteVCUMailbox(UINT32 Command, UINT32 Data, UINT32 Indx)
{
UINT32 VcuInterface;
UINT32 UboxBase; 
//volatile UINT8 Indxx = 1;

//while(Indxx);

UboxBase = (UINT32)AsmReadMsr64 (0x300);
UboxBase = (UboxBase >> 8) & 0xFF;

VcuInterface = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40));
while(VcuInterface & BIT31)
{
	VcuInterface = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40));
}

// Write Data register first
PciWrite32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x44),Data);

// Write Interface register 
PciWrite32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40),(BIT31 | Command)| (Indx << 16));

VcuInterface = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40));
while(VcuInterface & BIT31)
{
	VcuInterface = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40));
}

VcuInterface = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x40));
Data = PciRead32 (PCI_LIB_ADDRESS (UboxBase, 0x1F, 0x00, 0x44));
if((VcuInterface & 0xFF) == 0x40)
{
DEBUG ((EFI_D_ERROR, "(OC) Raise PLL command Success. :%x \n ",Command));
}
else
{
DEBUG ((EFI_D_ERROR, "(OC) Raise PLL command FAILURE Interface: %X  Data :%X\n",VcuInterface,Data));
}

return VcuInterface;
}


EFI_STATUS WriteTjMax (UINT8 TjMaxoffset )
{
 UINT32 MailBoxCommand;
UINT32 UboxBase = 0x00;
//volatile UINT8 Indx = 1;

//while(Indx);

UboxBase = (UINT32)AsmReadMsr64 (0x300);
UboxBase = (UboxBase >> 8) & 0xFF;

 MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
 while(MailBoxCommand & BIT31)
 {
	MailBoxCommand = PciRead32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM));
 }

 MailBoxCommand = 0x800001A5;
 PciWrite32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILDATA),TjMaxoffset);
 PciWrite32 (PCI_LIB_ADDRESS (UboxBase, PCUDEV, PCUFUN, MAILCOMM),MailBoxCommand);

return EFI_SUCCESS;
}


EFI_STATUS AVXOffset (UINT8 AVX2Offset )
{

OC_MAILBOX_FULL OCMailbox;
AVX_DATA		AvxData;
EFI_STATUS		LibStatus = 0x00,Status = 0x00;

ZeroMem(&AvxData,sizeof(AVX_DATA));
ZeroMem(&OCMailbox,sizeof(OC_MAILBOX_FULL));

if(AVX2Offset != 0x00)
{
	AvxData.DataFields.Avx2 = AVX2Offset;
	OCMailbox.Data = AvxData.Data;
	OCMailbox.Interface.Fields.CommandCompletion = AVX_NEG_WRITE;
	Status = MailboxWrite (MAILBOX_TYPE_OC,OCMailbox.Interface.InterfaceData,OCMailbox.Data,&LibStatus);
}

return Status;

}

