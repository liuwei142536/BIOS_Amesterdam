/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2006 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  MeLib.c

@brief:

  Implementation file for Me functionality

**/

#include <Library/PciLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Framework/DxeCis.h>
#include <Library/DebugLib.h>
#include <Uefi/UefiSpec.h>
#include <MeAccess.h>
#include <HeciRegs.h>
#include <MeState.h>

#include "MeLib.h"


/*****************************************************************************
 * Local definitions.
 *****************************************************************************/
//
// HECIMBAR register definition
//
#define H_CB_WW   0x00
#define H_CSR     0x04
#define ME_CB_RW  0x08
#define ME_CSR_HA 0x0C

//
// ME_CSR_HA - ME Control Status Host Access
//
/*
typedef union {
  UINT32  ul;
  struct {
    UINT32  ME_IE_HRA : 1;    // 0 - ME Interrupt Enable (Host Read Access)
    UINT32  ME_IS_HRA : 1;    // 1 - ME Interrupt Status (Host Read Access)
    UINT32  ME_IG_HRA : 1;    // 2 - ME Interrupt Generate (Host Read Access)
    UINT32  ME_RDY_HRA : 1;   // 3 - ME Ready (Host Read Access)
    UINT32  ME_RST_HRA : 1;   // 4 - ME Reset (Host Read Access)
    UINT32  Reserved : 3;     // 7:5
    UINT32  ME_CBRP_HRA : 8;  // 15:8 - ME CB Read Pointer (Host Read Access)
    UINT32  ME_CBWP_HRA : 8;  // 23:16 - ME CB Write Pointer (Host Read Access)
    UINT32  ME_CBD_HRA : 8;   // 31:24 - ME Circular Buffer Depth (Host Read Access)
  } r;
} HECI_ME_CONTROL_REGISTER;
*/

//
// H_CSR - Host Control Status
//
/*
typedef union {
  UINT32  ul;
  struct {
    UINT32  H_IE : 1;     // 0 - Host Interrupt Enable ME
    UINT32  H_IS : 1;     // 1 - Host Interrupt Status ME
    UINT32  H_IG : 1;     // 2 - Host Interrupt Generate
    UINT32  H_RDY : 1;    // 3 - Host Ready
    UINT32  H_RST : 1;    // 4 - Host Reset
    UINT32  Reserved : 3; // 7:5
    UINT32  H_CBRP : 8;   // 15:8 - Host CB Read Pointer
    UINT32  H_CBWP : 8;   // 23:16 - Host CB Write Pointer
    UINT32  H_CBD : 8;    // 31:24 - Host Circular Buffer Depth
  } r;
} HECI_HOST_CONTROL_REGISTER;
*/

/*****************************************************************************
 * Public functions.
 *****************************************************************************/
EFI_STATUS
MeLibInit(VOID)
/**

  @brief

  Check if Me is enabled

  @param[in] None.


**/
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  return Status;
}

EFI_STATUS
HeciGetFwCapsSku (
  MEFWCAPS_SKU       *FwCapsSku
  )
/**

  @brief
  Send Get Firmware SKU Request to ME

  @param[in] FwCapsSku            Return Data from Get Firmware Capabilities MKHI Request

  @retval EFI_SUCCESS             The function completed successfully.

**/
{
  EFI_STATUS              Status;
  GEN_GET_FW_CAPSKU       MsgGenGetFwCapsSku;
  GEN_GET_FW_CAPS_SKU_ACK MsgGenGetFwCapsSkuAck;

  Status = HeciGetFwCapsSkuMsg (&MsgGenGetFwCapsSku, &MsgGenGetFwCapsSkuAck);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (((MsgGenGetFwCapsSkuAck.MKHIHeader.Fields.Command) == FWCAPS_GET_RULE_CMD) &&
      ((MsgGenGetFwCapsSkuAck.MKHIHeader.Fields.IsResponse) == 1) &&
      (MsgGenGetFwCapsSkuAck.MKHIHeader.Fields.Result == 0)
      ) {
    *FwCapsSku = MsgGenGetFwCapsSkuAck.Data.FWCapSku;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
HeciGetPlatformType (
  OUT PLATFORM_TYPE_RULE_DATA   *RuleData
  )
/**

  @brief
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to get Ibex Peak platform type.
  One of usages is to utilize this command to determine if the platform runs in
  4M or 8M size firmware.

  @param[in] RuleData             PlatformBrand,
                                  IntelMeFwImageType,
                                  SuperSku,
                                  PlatformTargetMarketType,
                                  PlatformTargetUsageType

  @retval EFI_SUCCESS             The function completed successfully.

**/
{
  EFI_STATUS  Status;

  Status = HeciGetPlatformTypeMsg (RuleData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

VOID
MbpCleared (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
/**
  @brief
  Routine checks whether MBP buffer has been cleared form HECI buffer or not.
  BIOS must check this before executing any 3rd paty code or Oprom

  @param[in] Event                The event that triggered this notification function
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
 
**/
{
  EFI_STATUS              Status;
  HECI_GS_SHDW_REGISTER   MeFwsts2;
  DXE_ME_POLICY_PROTOCOL  *DxePlatformMePolicy;
  DXE_MBP_DATA_PROTOCOL   *MbpData;
  HECI_FWS_REGISTER       MeFirmwareStatus;

  ///Aptiov server override : ME disable function
  UINT8                   TimeOut;
  UINT64                              HECI_BASE_ADDRESS = 0;
  HECI_HOST_CONTROL_REGISTER          HeciRegHCsr;
  volatile HECI_HOST_CONTROL_REGISTER *HeciRegHCsrPtr;
  
  HECI_BASE_ADDRESS = (UINT64)PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_HECIMBAR));
  HECI_BASE_ADDRESS &= 0xFFFFFFFFFFFFFFF0;
  HeciRegHCsrPtr    = (VOID *) (UINTN) (HECI_BASE_ADDRESS + H_CSR);
  ///Aptiov server override : ME disable function
  
  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
  if (!EFI_ERROR (Status)) {
    ///
    /// UnInstall the MBP protocol
    ///
    Status = gBS->UninstallMultipleProtocolInterfaces (
                    MbpData->Handle,
                    &gMeBiosPayloadDataProtocolGuid,
                    MbpData,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Unable to Uninstall Mbp protocol and Status is %r\n", Status));
    }
  }
  ///
  /// Get the ME platform policy.
  ///
  Status = gBS->LocateProtocol (&gDxePlatformMePolicyGuid, NULL, &DxePlatformMePolicy);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "No ME Platform Policy Protocol available\n"));
    return;
  }

  if (DxePlatformMePolicy->MeConfig.MbpSecurity == 0) {
    DEBUG ((EFI_D_INFO, "MBP security guarantees to be disabled\n"));
    return;
  }
  ///
  /// Check for MEI1 PCI device availability
  ///
  if (HeciPciRead32 (R_VENDORID) == 0xFFFFFFFF) {
    DEBUG ((EFI_D_ERROR, "MEI1 PCI device does not exist\n"));
    return;
  }
  ///
  /// Read ME FWS2
  ///
  MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);

 ///Aptiov server override : ME disable function
  TimeOut     = 200;
  while (!(MeFwsts2.r.Reserved2 & 0x2) && TimeOut) {
    DEBUG ((EFI_D_ERROR, "MbpClear = %x, HECI buffer is still not clear\n", MeFwsts2.r.Reserved2 & 0x2));
    ///
    /// BIOS polls on FWSTS.MbpClear until it is set
    ///
    gBS->Stall (FIVE_MS_TIMEOUT);
    MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
    TimeOut--;
  }
  ///
  /// MBP Give up if Time out
  ///
  if (!(MeFwsts2.r.Reserved2 & 0x2)) {
    HeciPciOr32 (0x70, 1);
    
    HeciRegHCsr.ul      = HeciRegHCsrPtr->ul;
    HeciRegHCsr.r.H_RST = 1;
    HeciRegHCsr.r.H_IG  = 1;
    HeciRegHCsrPtr->ul  = HeciRegHCsr.ul;
  }
  ///Aptiov server override : ME disable function
	
  MeFirmwareStatus.ul = HeciPciRead32 (R_FWSTATE);
  if (MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_UNKNOWN || MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_IMAGE_FAILURE) {
    DisableAllMEDevices ();
  }
  Status = EFI_SUCCESS;
  return;
}

UINT8
FilledSlots2 (
  IN  UINT32 ReadPointer,
  IN  UINT32 WritePointer
  )
/**

  @brief
  Calculate if the circular buffer has overflowed.
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in] ReadPointer          Location of the read pointer.
  @param[in] WritePointer         Location of the write pointer.

  @retval UINT8                   Number of filled slots.

**/
{
  UINT8 FilledSlots;

  ///
  /// Calculation documented in HECI HPS 0.68 section 4.2.1
  ///
  FilledSlots = (((INT8) WritePointer) - ((INT8) ReadPointer));

  return FilledSlots;
}

EFI_STATUS
PrepareMeBiosPayload (
  IN     ME_BIOS_PAYLOAD *MbpPtr,
  IN OUT EFI_EVENT       *MpbClearEvent
  )
/**
  @brief
  This routine returns ME-BIOS Payload information.

  @param[in] MbpPtr               ME_INFO_HOB, ME-BIOS Payload information.
  @param[in out] MpbClearEvent    event to create on gExitPmAuthProtocolGuid
                                  to check MBP FWSTS bit.
                                 
  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        The circular buffer is empty
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.

**/
{
  EFI_STATUS                          Status;
  UINT32                              *Ptr;
  UINT32                              MbpItemId;
  UINT32                              MbpItemSize;
  UINT32                              i;
  UINT32                              Index;
  MBP_HEADER                          MbpHeader;
  MBP_ITEM_HEADER                     MbpItemHeader;
  VOID                                *Registration;
  HECI_GS_SHDW_REGISTER               MeFwsts2;
  UINT64                              HECI_BASE_ADDRESS = 0;
  HECI_ME_CONTROL_REGISTER            HeciRegMeCsrHa;
  HECI_HOST_CONTROL_REGISTER          HeciRegHCsr;
  volatile HECI_HOST_CONTROL_REGISTER *HeciRegHCsrPtr;
  volatile HECI_ME_CONTROL_REGISTER   *HeciRegMeCsrHaPtr;
  volatile UINT32                     *HeciRegMeCbrwPtr;
  VOID                                *DestPtr;
  UINTN                               DestSize;
  UINT8                               TimeOut;

  Ptr     = NULL;
  Status  = EFI_UNSUPPORTED;

  ///
  /// Read ME FWS2
  ///
  MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);

  DEBUG ((EFI_D_INFO, "MbpPresent = %x\n", MeFwsts2.r.MbpRdy));

  if (MeFwsts2.r.MbpRdy) {
    DEBUG ((EFI_D_INFO, "MBP is present.\n"));
    ///
    /// Initialize memory mapped register pointers
    ///
    HECI_BASE_ADDRESS = (UINT64)PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_HECIMBAR));
    /*if (HECI_BASE_ADDRESS & 0x4) { AptioV server override
      HECI_BASE_ADDRESS |= ((UINT64)PciRead32 (PCI_LIB_ADDRESS (ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_HECIMBAR + 4))) << 32;
    }*/
    HECI_BASE_ADDRESS &= 0xFFFFFFFFFFFFFFF0;
    HeciRegHCsrPtr    = (VOID *) (UINTN) (HECI_BASE_ADDRESS + H_CSR);
    HeciRegMeCsrHaPtr = (VOID *) (UINTN) (HECI_BASE_ADDRESS + ME_CSR_HA);
    HeciRegMeCbrwPtr  = (VOID *) (UINTN) (HECI_BASE_ADDRESS + ME_CB_RW);

    HeciRegMeCsrHa.ul = HeciRegMeCsrHaPtr->ul;
    if (FilledSlots2 (HeciRegMeCsrHa.r.ME_CBRP_HRA, HeciRegMeCsrHa.r.ME_CBWP_HRA) == 0) {
      ///
      /// Exit if the circular buffer is empty
      ///
      DEBUG ((EFI_D_ERROR, "MBP is present but circular buffer is empty, exit.\n"));
      return Status;
    } else {

      MbpHeader.Data = *HeciRegMeCbrwPtr;

      DEBUG ((EFI_D_INFO, "MBP header: %x\n", MbpHeader.Data));
      DEBUG ((EFI_D_INFO, "MbpSize: %x\n", MbpHeader.Fields.MbpSize));
      DEBUG ((EFI_D_INFO, "No. of Mbp Entries: %x\n", MbpHeader.Fields.NumEntries));

      MbpPtr->FwCapsSku.Available               = FALSE;
      MbpPtr->FwPlatType.Available              = FALSE;
      MbpPtr->HwaRequest.Available              = FALSE;
      MbpPtr->HwaRequest.Data.Fields.MediaTablePush = FALSE;

      for (i = 0; i < MbpHeader.Fields.NumEntries; i++) {
        ///
        /// If MBP Data Item is available but, RD and WR pointer are same, it indicates an error
        /// ME FW indication of number of MBP entries and size are wrong
        ///
        HeciRegMeCsrHa.ul = HeciRegMeCsrHaPtr->ul;
        if (FilledSlots2 (HeciRegMeCsrHa.r.ME_CBRP_HRA, HeciRegMeCsrHa.r.ME_CBWP_HRA) == 0) {
          ///
          /// Exit if the circular buffer is empty
          ///
          DEBUG ((EFI_D_ERROR, "MBP Item is present but circular buffer is empty, exit.\n"));
          return EFI_DEVICE_ERROR;
        }
        ///
        /// Consume MBP Item Header
        ///
        MbpItemHeader.Data = *HeciRegMeCbrwPtr;
        DEBUG ((EFI_D_INFO, "MBP Item %x header: %x\n", i + 1, MbpItemHeader.Data));

        MbpItemId = (MbpItemHeader.Fields.AppId << 4) | MbpItemHeader.Fields.ItemId;
        ///
        /// Copy data from HECI buffer per size of each MBP item
        ///
        MbpItemSize = MbpItemHeader.Fields.Length - 1;
        Ptr         = AllocateZeroPool ((MbpItemSize) * 4);
        if (Ptr == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }

        for (Index = 0; Index < MbpItemSize; Index++) {
          Ptr[Index] = *HeciRegMeCbrwPtr;
          DEBUG ((EFI_D_INFO, "MBP Item Data: %x\n", Ptr[Index]));
        }

        DEBUG ((EFI_D_INFO, "Parse MBP Item ID %x\n", MbpItemId));
        switch (MbpItemId) {
        case 0x11:
          ///
          /// FW Version Name
          ///
          DestSize = sizeof (MbpPtr->FwVersionName);
          DestPtr = &MbpPtr->FwVersionName;
          break;

        case 0x12:
          ///
          /// FW Capabilities
          ///
          DestSize = sizeof (MbpPtr->FwCapsSku.FwCapabilities);
          DestPtr = &MbpPtr->FwCapsSku.FwCapabilities;
          MbpPtr->FwCapsSku.Available = TRUE;
          break;

        case 0x13:
          ///
          /// ROM BIST Data
          ///
          DestSize = sizeof (MbpPtr->RomBistData);
          DestPtr = &MbpPtr->RomBistData;
          break;

        case 0x14:
          ///
          /// Platform Key
          ///
          DestSize = sizeof (MbpPtr->PlatformKey);
          DestPtr = &MbpPtr->PlatformKey;
          break;

        case 0x15:
          ///
          /// ME Platform TYpe
          ///
          DestSize = sizeof (MbpPtr->FwPlatType.RuleData);
          DestPtr = &MbpPtr->FwPlatType.RuleData;
          MbpPtr->FwPlatType.Available = TRUE;
          break;

        case 0x51:
          ///
          /// ICC Profile
          ///
          DestSize = sizeof (MbpPtr->IccProfile);
          DestPtr = &MbpPtr->IccProfile;
          break;

        case 0x31:
          ///
          /// AT State
          ///
          DestSize = sizeof (MbpPtr->AtState);
          DestPtr = &MbpPtr->AtState;
          break;

        case 0x16:
          ///
          /// ME MFS status
          ///
          DestSize = sizeof (MbpPtr->MFSIntegrity);
          DestPtr = &MbpPtr->MFSIntegrity;
          break;

        case 0x41:
          ///
          /// HWA Request
          ///
          DestSize = sizeof (MbpPtr->HwaRequest.Data);
          DestPtr = &MbpPtr->HwaRequest.Data;
          MbpPtr->HwaRequest.Available = TRUE;
          break;

        default:
          ///
          /// default case
          ///
          DestSize = 0;
          DestPtr = NULL;
          break;
        }
        if (DestPtr != NULL) {
          if ((MbpItemSize * 4) <= DestSize) {
            CopyMem (DestPtr, Ptr, (MbpItemSize * 4));
          } else {
            DEBUG ((EFI_D_INFO, "Data size is larger than destination buffer. This item is not copied.\n"));
          }
          ///
          /// Clear buffer
          ///
          ZeroMem (Ptr, MbpItemSize * 4);
          FreePool (Ptr);
        }
      }
      /// Aptiov server override : ME disable function
      /// BIOS checks two things for R/W pointer mismatch after MBP consumption.
      ///
      HeciRegMeCsrHa.ul = HeciRegMeCsrHaPtr->ul;
      if (FilledSlots2 (HeciRegMeCsrHa.r.ME_CBRP_HRA, HeciRegMeCsrHa.r.ME_CBWP_HRA) == 0) {
      ///Aptiov server override : ME disable function

      /// Interrupt ME FW so FW can clear HECI buffer after MBP is consumed
      ///
      HeciRegHCsr.ul      = HeciRegHCsrPtr->ul;
      HeciRegHCsr.r.H_IG  = 1;
      HeciRegHCsrPtr->ul  = HeciRegHCsr.ul;

      ///
      /// Create a callback event to check MBP FWSTS bit
      ///
      Status = gBS->CreateEvent (
                      EVT_NOTIFY_SIGNAL,
                      TPL_CALLBACK,
                      MbpCleared,
                      NULL,
                      MpbClearEvent
                      );
      ASSERT_EFI_ERROR (Status);
      ///
      /// Register EXIT_PM_AUTH_PROTOCOL notify function
      ///
      Status = gBS->RegisterProtocolNotify (
                      &gExitPmAuthProtocolGuid,
                      MpbClearEvent,
                      &Registration
                      );
      ASSERT_EFI_ERROR (Status);
      }   ///Aptiov server override : ME disable function
      else {
        ///
        /// Interrupt ME FW
        ///
        HeciRegHCsr.ul      = HeciRegHCsrPtr->ul;
        HeciRegHCsr.r.H_IG  = 1;
        HeciRegHCsrPtr->ul  = HeciRegHCsr.ul;
        
        ///
        /// Read ME FWS2
        ///
        MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
        TimeOut     = 200;
        while (MeFwsts2.r.MbpRdy && TimeOut) {
          DEBUG ((EFI_D_ERROR, "MbpPresent = %x, HECI buffer is still not clear\n", MeFwsts2.r.MbpRdy));
          ///
          /// BIOS polls on FWSTS.MbpReady until it is clear
          ///
          gBS->Stall (FIVE_MS_TIMEOUT);
          MeFwsts2.ul = HeciPciRead32 (R_ME_GS_SHDW);
          TimeOut--;
        }
        ///
        /// MBP Give up if Time out
        ///
        if (MeFwsts2.r.MbpRdy) {
          HeciPciOr32 (0x70, 1);
          
          HeciRegHCsr.ul      = HeciRegHCsrPtr->ul;
          HeciRegHCsr.r.H_RST = 1;
          HeciRegHCsr.r.H_IG  = 1;
          HeciRegHCsrPtr->ul  = HeciRegHCsr.ul;
        }
      }    ///Aptiov server override : ME disable function
    }
  }

  return Status;
}

EFI_STATUS
HeciGetFwVersion (
  IN OUT GEN_GET_FW_VER_ACK_DATA      *MsgGenGetFwVersionAckData
  )
/**

  @brief
  Send Get Firmware Version Request to ME

  @param[in] MsgGenGetFwVersionAckData  Return themessage of FW version

  @retval EFI_SUCCESS             The function completed successfully.

**/
{
  EFI_STATUS          Status;
  GEN_GET_FW_VER_ACK  MsgGenGetFwVersionAck;

  Status = HeciGetFwVersionMsg (&MsgGenGetFwVersionAck);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((MsgGenGetFwVersionAck.MKHIHeader.Fields.Command == GEN_GET_FW_VERSION_CMD) &&
      (MsgGenGetFwVersionAck.MKHIHeader.Fields.IsResponse == 1) &&
      (MsgGenGetFwVersionAck.MKHIHeader.Fields.Result == 0)
      ) {
    *MsgGenGetFwVersionAckData = MsgGenGetFwVersionAck.Data;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
HeciGetMeFwInfo (
  IN OUT ME_CAP *MECapability
  )
/**

  @brief
  Host client gets Firmware update info from ME client

  @param[in] MECapability         Structure of FirmwareUpdateInfo

  @exception EFI_UNSUPPORTED      No MBP Data Protocol available

**/
{
  EFI_STATUS            Status;
  DXE_MBP_DATA_PROTOCOL *MbpData;
  MEFWCAPS_SKU          FwCapsSku;

  ///
  /// Get the MBP Data.
  ///
  Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "HeciGetMeFwInfo: No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  MECapability->MeEnabled = 1;

  FwCapsSku.Data          = MbpData->MeBiosPayload.FwCapsSku.FwCapabilities.Data;
  if (FwCapsSku.Fields.IntelAT) {
    MECapability->AtSupported = 1;
  }

  if (FwCapsSku.Fields.KVM) {
    MECapability->IntelKVM = 1;
  }

  switch (MbpData->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand) {
  case INTEL_AMT_BRAND:
    MECapability->IntelAmtFw        = 1;
    MECapability->LocalWakeupTimer  = 1;
    break;

  case INTEL_STAND_MANAGEABILITY_BRAND:
    MECapability->IntelAmtFwStandard = 1;
    break;

  case INTEL_SMALL_BUSINESS_TECHNOLOGY_BRAND:
    MECapability->IntelSmallBusiness = 1;
    break;
  }

  MECapability->MeMajorVer  = MbpData->MeBiosPayload.FwVersionName.MajorVersion;
  MECapability->MeMinorVer  = MbpData->MeBiosPayload.FwVersionName.MinorVersion;
  MECapability->MeBuildNo   = MbpData->MeBiosPayload.FwVersionName.BuildVersion;
  MECapability->MeHotFixNo  = MbpData->MeBiosPayload.FwVersionName.HotfixVersion;

  return Status;
}

VOID
MeEmptyEventFunction (
  IN  EFI_EVENT                   Event,
  IN  void                        *ParentImageHandle
  )
/**

  @brief
  Dummy return for Me signal event use

  @param[in] Event                The event that triggered this notification function
  @param[in] ParentImageHandle    Pointer to the notification functions context

  @retval EFI_SUCCESS             Always return EFI_SUCCESS

**/
{
  return;
}

EFI_STATUS
GetAtStateInfo (
  AT_STATE_INFO *AtStateInfo
  )
/**

  @brief
  Get AT State Information From Stored ME platform policy

  @param[in] AtState              Pointer to AT State Information
  @param[in] AtLastTheftTrigger   Pointer to Variable holding the cause of last AT Stolen Stae
  @param[in] AtLockState          Pointer to variable indicating whether AT is locked or not
  @param[in] AtAmPref             Pointer to variable indicating whether ATAM or PBA should be used

  @retval EFI_SUCCESS             The function completed successfully
  @exception EFI_UNSUPPORTED      No MBP Data Protocol available

**/
{
  EFI_STATUS            Status;
  DXE_MBP_DATA_PROTOCOL *MbpData;

  ///
  /// Get the ME platform policy.
  ///
  Status = gBS->LocateProtocol (&gMeBiosPayloadDataProtocolGuid, NULL, &MbpData);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "GetAtStateInfo: No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }
  AtStateInfo->State                         = MbpData->MeBiosPayload.AtState.State;
  AtStateInfo->LastTheftTrigger              = MbpData->MeBiosPayload.AtState.LastTheftTrigger;
  AtStateInfo->flags.LockState               = MbpData->MeBiosPayload.AtState.flags.LockState;
  AtStateInfo->flags.AuthenticateModule      = MbpData->MeBiosPayload.AtState.flags.AuthenticateModule;
  AtStateInfo->flags.S3Authentication        = MbpData->MeBiosPayload.AtState.flags.S3Authentication;
  AtStateInfo->flags.FlashVariableSecurity   = MbpData->MeBiosPayload.AtState.flags.FlashVariableSecurity;
  AtStateInfo->flags.FlashWearOut            = MbpData->MeBiosPayload.AtState.flags.FlashWearOut;

  return EFI_SUCCESS;
}
