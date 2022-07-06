/**
  This file contains an 'Intel Peripheral Driver' and uniquely
  identified as "Intel Reference Module" and is
  licensed for Intel CPUs and chipsets under the terms of your
  license agreement with Intel or your vendor.  This file may
  be modified by the user, subject to additional terms of the
  license agreement
**/
/**

Copyright (c)  2010 - 2014 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.

@file:

  AlertStandardFormatPei.c

@brief:

  Processes ASF messages

**/
#include <Library\BaseMemoryLib.h>
#include <Library\HobLib.h>
#include "AlertStandardFormatPei.h"
#include "HeciMsgLibPei.h"

#define ASF_PEI
#include "AlertStandardFormatCommon.c"
#include "MeBiosExtensionSetup.h"
#include "AmtPolicyLibPei.h"
#include "AmtForcePushPetPolicy.h"
#include "Library/MeTypeLib.h"

EFI_STATUS
SaveForcePushErrorEvent (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  EFI_FRAMEWORK_MESSAGE_TYPE MessageType
  );

EFI_STATUS
QueuePetMessage (
  IN  EFI_PEI_SERVICES        **PeiServices,
  IN  EFI_STATUS_CODE_TYPE    Type,
  IN  EFI_STATUS_CODE_VALUE   Value
  );

EFI_STATUS
SendPETMessageInQueue (
  IN  EFI_PEI_SERVICES        **PeiServices
  );

static PEI_AMT_STATUS_CODE_PPI  mPeiAmtStatusCodePpi = { PeiAmtReportStatusCode };

static EFI_PEI_PPI_DESCRIPTOR   mInstallPeiAmtStatusCodePpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gPeiAmtStatusCodePpiGuid,
  &mPeiAmtStatusCodePpi
};

EFI_STATUS
EFIAPI
AlertStandardFormatDriverPeiEntryPoint (
  IN EFI_FFS_FILE_HEADER       *FfsHeader,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
/**

  @brief

  Perform AMT PET message sending

  @param[in] FfsHeader            FFS file header pointer of this driver.
  @param[in] PeiServices          General purpose services available to every PEIM.

  @retval EFI_SUCCESS             if the AMT StatusCode PPI is successfully installed.
  @exception EFI_UNSUPPORTED      ASF is not enabled or ManageabilityMode is zero.

**/
{
  EFI_STATUS  Status;

#if defined(SPS_SUPPORT) && SPS_SUPPORT
  if (!MeTypeIsAmt()) {
    return EFI_UNSUPPORTED;
  }
#endif // SPS_SUPPORT

  ///
  /// First check if ASF support is enabled in Setup.
  ///
  if (!PeiAsfSupported ((EFI_PEI_SERVICES **)PeiServices)) {
    return EFI_UNSUPPORTED;
  }
  ///
  /// Sending ASF Messaging if ManageabilityMode is not zero
  ///
  if (ManageabilityModeSetting ((EFI_PEI_SERVICES **)PeiServices) == MNT_OFF) {
    return EFI_UNSUPPORTED;
  }
  ///
  /// Install AMT report status code PPI
  ///
  Status = (**PeiServices).InstallPpi (PeiServices, &mInstallPeiAmtStatusCodePpi);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Try to send PET message
  ///
  SendPETMessageInQueue ((EFI_PEI_SERVICES **)PeiServices);

  return EFI_SUCCESS;
}

EFI_STATUS
SendAsfMessage (
  IN  EFI_PEI_SERVICES                     **PeiServices,
  IN  EFI_ASF_MESSAGE                      *AsfMessage
  )
/**

  @brief
    Send ASF Message.

  @param[in] PeiServices        General purpose services available to every PEIM.
  @param[in] AsfMessage         Pointer to ASF message

  @retval EFI_SUCCESS           Boot options copied
  @retval EFI_INVALID_PARAMETER Invalid pointer
  @retval EFI_NOT_READY         No controller
  @retval EFI_DEVICE_ERROR      The function should not be completed due to a device error.

**/
{
  EFI_STATUS                  Status;
  PEI_HECI_PPI                *Heci;
  UINT32                      HeciMemBar;
  UINT32                      HeciLength;
  HECI_ASF_PUSH_PROGRESS_CODE HeciAsfPushProgressCode;
  UINT32                      MeStatus;

  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gPeiHeciPpiGuid, /// GUID
                            0,                /// INSTANCE
                            NULL,             /// EFI_PEI_PPI_DESCRIPTOR
                            &Heci             /// PPI
                            );
  ASSERT_EFI_ERROR (Status);

  Status = Heci->InitializeHeci (Heci, &HeciMemBar);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  Status = Heci->GetMeStatus (&MeStatus);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Only send ASF Push Progress code when ME is ready.  Ignore FW Init Status.
  ///
  if (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY) {
    return EFI_NOT_READY;
  }

  ZeroMem ((VOID *) &HeciAsfPushProgressCode, sizeof (HECI_ASF_PUSH_PROGRESS_CODE));
  HeciAsfPushProgressCode.Command   = EFI_ASF_MESSAGE_COMMAND_MESSAGE;
  HeciAsfPushProgressCode.ByteCount = 0x10;
  HeciLength                        = HECI_ASF_PUSH_PROGRESS_CODE_LENGTH;
  CopyMem ((VOID *) &(HeciAsfPushProgressCode.AsfMessage), (VOID *) AsfMessage, sizeof (EFI_ASF_MESSAGE));

  Status = Heci->SendMsg (
                  Heci,
                  (UINT32 *) &HeciAsfPushProgressCode,
                  HeciMemBar,
                  HeciLength,
                  BIOS_ASF_HOST_ADDR,
                  HECI_ASF_MESSAGE_ADDR
                  );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

BOOLEAN
IsForcePushErrorEvent (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_FRAMEWORK_MESSAGE_TYPE  MessageType
  )
/**
  @brief

  This routine checks whethre current message is ForcePush message.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] MessageType          AMT PET Message Type.

  @retval TRUE                    It is ForcePush message.
  @retval FALSE                   It is not ForcePush message.

**/
{
  AMT_FORCE_PUSH_PET_POLICY_HOB *AmtForcePushPETPolicyHob;
  UINTN                         Index;
  UINTN                         Number;
  EFI_STATUS                    Status;

  Status = (*PeiServices)->GetHobList (PeiServices, (VOID **) &AmtForcePushPETPolicyHob);
  ASSERT_EFI_ERROR (Status);

  AmtForcePushPETPolicyHob = GetNextGuidHob (&gAmtForcePushPetPolicyGuid, AmtForcePushPETPolicyHob);
  if (AmtForcePushPETPolicyHob == NULL) {
    return FALSE;
  }

  Number = (AmtForcePushPETPolicyHob->EfiHobGuidType.Header.HobLength - sizeof (EFI_HOB_GUID_TYPE)) /
  sizeof (EFI_FRAMEWORK_MESSAGE_TYPE);
  for (Index = 0; Index < Number; Index++) {
    if (AmtForcePushPETPolicyHob->MessageType[Index] == MessageType) {
      return TRUE;
    }
  }

  return FALSE;
}

EFI_STATUS
EFIAPI
PeiAmtReportStatusCode (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  PEI_AMT_STATUS_CODE_PPI     * This,
  IN  EFI_STATUS_CODE_TYPE        Type,
  IN  EFI_STATUS_CODE_VALUE       Value,
  IN  UINT32                      Instance,
  IN  EFI_GUID                    * CallerId OPTIONAL,
  IN  EFI_STATUS_CODE_DATA        * Data OPTIONAL
  )
/**
  @brief

  Provides an interface that a software module can call to report an ASF PEI status code.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] This                 This interface.
  @param[in] Type                 Indicates the type of status code being reported.
  @param[in] Value                Describes the current status of a hardware or software entity.
                                  This included information about the class and subclass that is
                                  used to classify the entity as well as an operation.
  @param[in] Instance             The enumeration of a hardware or software entity within
                                  the system. Valid instance numbers start with 1.
  @param[in] CallerId             This optional parameter may be used to identify the caller.
                                  This parameter allows the status code driver to apply different
                                  rules to different callers.
  @param[in] Data                 This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        The function should not be completed due to a device error.

**/
{
  UINTN       Index;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (PeiFwProgressSupport (PeiServices)) {
    if ((Type & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE) {
      for (Index = 0; Index < sizeof (mAsfProgressDataHubMap) / sizeof (EFI_ASF_DATA_HUB_MAP); Index++) {
        if (mAsfProgressDataHubMap[Index].StatusCodeValue == Value) {
          ///
          /// Queue Progress Code and send PET after checking Boot Options
          ///
          QueuePetMessage (PeiServices, Type, Value);
        }
      }
    }
  }

  if ((Type & EFI_STATUS_CODE_TYPE_MASK) == EFI_ERROR_CODE) {
    for (Index = 0; Index < sizeof (mAsfErrorDataHubMap) / sizeof (EFI_ASF_DATA_HUB_MAP); Index++) {
      if (mAsfErrorDataHubMap[Index].StatusCodeValue == Value) {
        Status = SendPostPacket (PeiServices, mAsfErrorDataHubMap[Index].MessageType);
        if ((Status == EFI_DEVICE_ERROR) && IsForcePushErrorEvent (PeiServices, mAsfErrorDataHubMap[Index].MessageType)) {
          SaveForcePushErrorEvent (PeiServices, mAsfErrorDataHubMap[Index].MessageType);
        }

        if (Status == EFI_NOT_READY) {
          QueuePetMessage (PeiServices, Type, Value);
        }
      }
    }
  }

  return Status;
}

EFI_STATUS
SendPostPacket (
  IN  EFI_PEI_SERVICES                **PeiServices,
  IN  EFI_FRAMEWORK_MESSAGE_TYPE      MessageType
  )
/**

  @brief

  Sends a POST packet across ASF

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] MessageType          POST Status Code

  @retval EFI_SUCCESS             The function completed successfully

**/
{
  UINTN Index;

  ///
  /// Find the message to send across the wire
  ///
  for (Index = 0; Index < sizeof (mAsfFrameworkMessage) / sizeof (EFI_ASF_FRAMEWORK_MESSAGE); Index++) {
    if (mAsfFrameworkMessage[Index].MessageType == MessageType) {
      return SendAsfMessage (PeiServices, &mAsfFrameworkMessage[Index].Message);
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
SaveForcePushErrorEvent (
  IN  EFI_PEI_SERVICES           **PeiServices,
  IN  EFI_FRAMEWORK_MESSAGE_TYPE MessageType
  )
/**
  @brief

  This routine saves current ForcePush ErrorEvent to Hob, which will be sent again.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] MessageType          ASF PET message type.

  @retval EFI_SUCCESS             The function completed successfully

**/
{
  AMT_FORCE_PUSH_PET_HOB  *ForcePushPETHob;
  EFI_STATUS              Status;

  ///
  /// Create PET queue hob
  ///
  Status = (**PeiServices).CreateHob (
                            PeiServices,
                            EFI_HOB_TYPE_GUID_EXTENSION,
                            sizeof (AMT_FORCE_PUSH_PET_HOB),
                            &ForcePushPETHob
                            );
  ASSERT_EFI_ERROR (Status);

  ForcePushPETHob->EfiHobGuidType.Name  = gAmtForcePushPetHobGuid;
  ForcePushPETHob->MessageType          = MessageType;

  return EFI_SUCCESS;
}

EFI_STATUS
QueuePetMessage (
  IN  EFI_PEI_SERVICES        **PeiServices,
  IN  EFI_STATUS_CODE_TYPE    Type,
  IN  EFI_STATUS_CODE_VALUE   Value
  )
/**
  @brief

  This routine puts PET message to MessageQueue, which will be sent later.

  @param[in] PeiServices          PeiServices pointer.
  @param[in] Type                 StatusCode message type.
  @param[in] Value                StatusCode message value.

  @retval EFI_SUCCESS             The function completed successfully

**/
{
  AMT_PET_QUEUE_HOB *PETQueueHob;
  EFI_STATUS        Status;

  ///
  /// Create PET queue hob
  ///
  Status = (**PeiServices).CreateHob (
                            PeiServices,
                            EFI_HOB_TYPE_GUID_EXTENSION,
                            sizeof (AMT_PET_QUEUE_HOB),
                            &PETQueueHob
                            );
  ASSERT_EFI_ERROR (Status);
  PETQueueHob->EfiHobGuidType.Name  = gAmtPetQueueHobGuid;
  PETQueueHob->Value                = Value;

  return EFI_SUCCESS;
}

EFI_STATUS
SendPETMessageInQueue (
  IN  EFI_PEI_SERVICES        **PeiServices
  )
/**
  @brief

  This routine sends PET message in MessageQueue.

  @param[in] PeiServices          PeiServices pointer.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_READY           No controller

**/
{
  EFI_STATUS            Status;
  AMT_PET_QUEUE_HOB     *PETQueueHob;
  EFI_PEI_HOB_POINTERS  Hob;

  PEI_HECI_PPI          *Heci;
  UINT32                HeciMemBar;
  UINT32                MeStatus;

  ///
  /// Try HECI state
  ///
  Status = (*PeiServices)->LocatePpi (
                            PeiServices,
                            &gPeiHeciPpiGuid, /// GUID
                            0,                /// INSTANCE
                            NULL,             /// EFI_PEI_PPI_DESCRIPTOR
                            &Heci             /// PPI
                            );
  ASSERT_EFI_ERROR (Status);

  Status = Heci->InitializeHeci (Heci, &HeciMemBar);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_READY;
  }

  Status = Heci->GetMeStatus (&MeStatus);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Only send ASF Push Progress code when ME is ready.  Ignore FW Init Status.
  ///
  if (ME_STATUS_ME_STATE_ONLY (MeStatus) != ME_READY) {
    return EFI_NOT_READY;
  }
  ///
  /// Get PETQueueHob
  ///
  Status = (*PeiServices)->GetHobList (PeiServices, (VOID **) &PETQueueHob);
  ASSERT_EFI_ERROR (Status);

  while (TRUE) {
    PETQueueHob = GetNextGuidHob (&gAmtPetQueueHobGuid, PETQueueHob);
    if (PETQueueHob == NULL) {
      break;
    }
    ///
    /// Send message
    ///
    PeiAmtReportStatusCode (PeiServices, NULL, PETQueueHob->Type, PETQueueHob->Value, 0, NULL, NULL);

    ///
    /// Mark it as sent
    ///
    PETQueueHob->Type = (UINT32) -1;

    ///
    /// Need find next one
    ///
    Hob.Raw     = (VOID *) PETQueueHob;
    PETQueueHob = (AMT_PET_QUEUE_HOB *) GET_NEXT_HOB (Hob);
  }

  return EFI_SUCCESS;
}
