/*++
  This file contains an 'Intel Peripheral Driver' and is        
  licensed for Intel CPUs and chipsets under the terms of your  
  license agreement with Intel or your vendor.  This file may   
  be modified by the user, subject to additional terms of the   
  license agreement                                             
--*/
/*++

Copyright (c)  2005 - 2015 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:
  SecurityCallback.c

Abstract:
  Updates the IFR with runtime information.

Revision History:

--*/

#include "SetupPlatform.h"
//#include "EfiTpm.h"

STATIC EFI_PASSWORD_DATA *SecurityData = NULL;
STATIC UINT8             State;

EFI_PASSWORD_DATA mAdminPassword = {
  {
    0
  },
  {
    0
  },
  &mSystemConfiguration.AdminPassword,
  EfiAdminPassword
};

EFI_PASSWORD_DATA mUserPassword = {
  {
    0
  },
  {
    0
  },
  &mSystemConfiguration.UserPassword,
  EfiUserPassword
};

STATIC
EFI_STATUS
EnableTpmDevice (
  IN     BOOLEAN                   TpmEnable
  );

STATIC
EFI_STATUS
ActivateTpmDevice (
  IN     BOOLEAN                   TpmActive
  );

STATIC
TPM_RESULT
TpmCommandNoReturnData (
  IN     EFI_TCG_PROTOCOL          *TcgProtocol,
  IN     TPM_COMMAND_CODE          Ordinal,
  IN     UINTN                     AdditionalParameterSize,
  IN     VOID                      *AdditionalParameters
  );

#define TPM_PP_USER_ABORT           ((TPM_RESULT)(-0x10))
#define TPM_PP_BIOS_FAILURE         ((TPM_RESULT)(-0x0f))

#define H2NS(x) ((((x) << 8) | ((x) >> 8)) & 0xffff)
#define H2NL(x) (H2NS ((x) >> 16) | (H2NS ((x) & 0xffff) << 16))

STATIC
EFI_STATUS
SimpleTpmCommand (
  IN     TPM_COMMAND_CODE          Ordinal,
  IN     UINTN                     AdditionalParameterSize,
  IN     VOID                      *AdditionalParameters
  )
{
  EFI_STATUS                       Status;
  TPM_RQU_COMMAND_HDR              *TpmRqu;
  TPM_RSP_COMMAND_HDR              TpmRsp;
  UINT32                           Size;
  EFI_TCG_PROTOCOL                 *TcgProtocol;
  
  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, &TcgProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TpmRqu = (TPM_RQU_COMMAND_HDR*)AllocateZeroPool (
                                   sizeof (*TpmRqu) + AdditionalParameterSize
                                   );
  ASSERT (TpmRqu != NULL);

  TpmRqu->tag       = H2NS (TPM_TAG_RQU_COMMAND);
  Size              = (UINT32)(sizeof (*TpmRqu) + AdditionalParameterSize);
  TpmRqu->paramSize = H2NL (Size);
  TpmRqu->ordinal   = H2NL (Ordinal);
  gBS->CopyMem (TpmRqu + 1, AdditionalParameters, AdditionalParameterSize);

  Status = TcgProtocol->PassThroughToTpm (
                          TcgProtocol,
                          Size,
                          (UINT8*)TpmRqu,
                          (UINT32)sizeof (TpmRsp),
                          (UINT8*)&TpmRsp
                          );
  gBS->FreePool (TpmRqu);
  ASSERT_EFI_ERROR (Status);
  ASSERT (TpmRsp.tag == H2NS (TPM_TAG_RSP_COMMAND));

  if (H2NL (TpmRsp.returnCode) != 0) {
    return EFI_INVALID_PARAMETER;
  }
  return Status;
}


EFI_STATUS
GetTpmState (
     OUT BOOLEAN                   *TpmEnable, OPTIONAL
     OUT BOOLEAN                   *TpmActivated, OPTIONAL
     OUT BOOLEAN                   *PhysicalPresenceLock, OPTIONAL
     OUT BOOLEAN                   *LifetimeLock, OPTIONAL
     OUT BOOLEAN                   *CmdEnable OPTIONAL
  )
{
  EFI_STATUS                       Status;
  TPM_RSP_COMMAND_HDR              *TpmRsp;
  UINT32                           TpmSendSize;
  TPM_PERMANENT_FLAGS              *TpmPermanentFlags;
  TPM_STCLEAR_FLAGS                *VFlags;
  UINT8                            CmdBuf[64];
  EFI_TCG_PROTOCOL                 *TcgProtocol;
    
  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, &TcgProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get TPM Permanent flags (TpmEnable, TpmActivated, LifetimeLock, CmdEnable)
  //
  if ((TpmEnable != NULL) || (TpmActivated != NULL) || (LifetimeLock != NULL) || (CmdEnable != NULL)) {
    TpmSendSize           = sizeof (TPM_RQU_COMMAND_HDR) + sizeof (UINT32) * 3;
    *(UINT16*)&CmdBuf[0]  = H2NS (TPM_TAG_RQU_COMMAND);
    *(UINT32*)&CmdBuf[2]  = H2NL (TpmSendSize);
    *(UINT32*)&CmdBuf[6]  = H2NL (TPM_ORD_GetCapability);
  
    *(UINT32*)&CmdBuf[10] = H2NL (TPM_CAP_FLAG);
    *(UINT32*)&CmdBuf[14] = H2NL (sizeof (TPM_CAP_FLAG_PERMANENT));
    *(UINT32*)&CmdBuf[18] = H2NL (TPM_CAP_FLAG_PERMANENT);

    Status = TcgProtocol->PassThroughToTpm (
                            TcgProtocol,
                            TpmSendSize,
                            CmdBuf,
                            sizeof (CmdBuf),
                            CmdBuf
                            ); 
    TpmRsp = (TPM_RSP_COMMAND_HDR*)&CmdBuf[0];
    if (EFI_ERROR (Status) || (TpmRsp->tag != H2NS (TPM_TAG_RSP_COMMAND)) || (TpmRsp->returnCode != 0)) {
      return EFI_DEVICE_ERROR;
    }
  
    TpmPermanentFlags = (TPM_PERMANENT_FLAGS *)&CmdBuf[sizeof (TPM_RSP_COMMAND_HDR) + sizeof (UINT32)];

    if (TpmEnable != NULL) {
      *TpmEnable = !TpmPermanentFlags->disable;
    }

    if (TpmActivated != NULL) {
      *TpmActivated = !TpmPermanentFlags->deactivated;
    }
    if (LifetimeLock != NULL) {
      *LifetimeLock = TpmPermanentFlags->physicalPresenceLifetimeLock;
    }

    if (CmdEnable != NULL) {
      *CmdEnable = TpmPermanentFlags->physicalPresenceCMDEnable;
    }
  }
  
  //
  // Get TPM Volatile flags (PhysicalPresenceLock)
  //
  if (PhysicalPresenceLock != NULL) {
    TpmSendSize           = sizeof (TPM_RQU_COMMAND_HDR) + sizeof (UINT32) * 3;
    *(UINT16*)&CmdBuf[0]  = H2NS (TPM_TAG_RQU_COMMAND);
    *(UINT32*)&CmdBuf[2]  = H2NL (TpmSendSize);
    *(UINT32*)&CmdBuf[6]  = H2NL (TPM_ORD_GetCapability);
  
    *(UINT32*)&CmdBuf[10] = H2NL (TPM_CAP_FLAG);
    *(UINT32*)&CmdBuf[14] = H2NL (sizeof (TPM_CAP_FLAG_VOLATILE));
    *(UINT32*)&CmdBuf[18] = H2NL (TPM_CAP_FLAG_VOLATILE);

    Status = TcgProtocol->PassThroughToTpm (
                            TcgProtocol,
                            TpmSendSize,
                            CmdBuf,
                            sizeof (CmdBuf),
                            CmdBuf
                            ); 
    TpmRsp = (TPM_RSP_COMMAND_HDR*)&CmdBuf[0];
    if (EFI_ERROR (Status) || (TpmRsp->tag != H2NS (TPM_TAG_RSP_COMMAND)) || (TpmRsp->returnCode != 0)) {
      return EFI_DEVICE_ERROR;
    }
  
    VFlags = (TPM_STCLEAR_FLAGS *)&CmdBuf[sizeof (TPM_RSP_COMMAND_HDR) + sizeof (UINT32)];

    if (PhysicalPresenceLock != NULL) {
      *PhysicalPresenceLock = VFlags->physicalPresenceLock;
    }   
  }
 
  return EFI_SUCCESS;  
}


STATIC
EFI_STATUS
TpmPhysicalPresence (
  IN      TPM_PHYSICAL_PRESENCE     PhysicalPresence
  )
{
  EFI_STATUS                        Status;
  EFI_TCG_PROTOCOL                  *TcgProtocol;
  TPM_RQU_COMMAND_HDR               *TpmRqu;
  TPM_PHYSICAL_PRESENCE             *TpmPp;
  TPM_RSP_COMMAND_HDR               TpmRsp;
  UINT8                             Buffer[sizeof (*TpmRqu) + sizeof (*TpmPp)];

  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, &TcgProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  TpmRqu = (TPM_RQU_COMMAND_HDR*)Buffer;
  TpmPp = (TPM_PHYSICAL_PRESENCE*)(TpmRqu + 1);

  TpmRqu->tag = H2NS (TPM_TAG_RQU_COMMAND);
  TpmRqu->paramSize = H2NL (sizeof (Buffer));
  TpmRqu->ordinal = H2NL (TSC_ORD_PhysicalPresence);
  *TpmPp = H2NS (PhysicalPresence);

  Status = TcgProtocol->PassThroughToTpm (
                          TcgProtocol,
                          sizeof (Buffer),
                          (UINT8*)TpmRqu,
                          sizeof (TpmRsp),
                          (UINT8*)&TpmRsp
                          );
  ASSERT_EFI_ERROR (Status);
  ASSERT (TpmRsp.tag == H2NS (TPM_TAG_RSP_COMMAND));
  if (TpmRsp.returnCode != 0) {
    //
    // If it fails, some requirements may be needed for this command.
    //
    return EFI_SECURITY_VIOLATION;
  }
  return Status;
}


STATIC
EFI_STATUS
SetOppPresent (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           LifetimeLock;
  BOOLEAN                           CmdEnable;
  
  //
  // Make sure TPM_PHYSICAL_PRESENCE_CMD_ENABLE is TRUE.
  //  
  Status = GetTpmState (NULL, NULL, NULL, &LifetimeLock, &CmdEnable);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] Get GetTpmState error Status = %r\n", Status));
    return Status;
  }

  if (!CmdEnable) {
    if (LifetimeLock) {
      //
      // physicalPresenceCMDEnable is locked, can't change.
      //
      return EFI_ABORTED;
    }

    Status = TpmPhysicalPresence (TPM_PHYSICAL_PRESENCE_CMD_ENABLE);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Set operator physical presence flags
  // 
  Status = TpmPhysicalPresence (TPM_PHYSICAL_PRESENCE_PRESENT);
  return Status;
}



STATIC
EFI_STATUS
TpmForceClear (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINTN                            VarSize;
  UINT32						   Attributes;
  
  DEBUG ((EFI_D_ERROR, "[TPM] Into ForceClear!\n"));  
  Status = SetOppPresent();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // TPM Force Clear
  //
  DEBUG ((EFI_D_ERROR, "[TPM] SimpleTpmCommand TPM_ORD_ForceClear!\n"));  
  Status = SimpleTpmCommand (TPM_ORD_ForceClear, 0, NULL);        
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] Fail to force clear TPM device!\n"));
    return Status;
  }

  //
  // Get current TPM configuration
  //
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable(mVariableName, &mSystemConfigurationGuid, &Attributes, &VarSize, &mSystemConfiguration);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] TPM NV Variable not found!(0x%x) \n", Status));
    return Status;
  }

  if (mSystemConfiguration.TpmEnable || mSystemConfiguration.TpmState) {
    //
    // TPM variable need update
    //
    mSystemConfiguration.TpmEnable = FALSE;
    mSystemConfiguration.TpmState  = FALSE;
    Status = gRT->SetVariable (
                    mVariableName,
                    &mSystemConfigurationGuid,
                    Attributes,
                    sizeof(SYSTEM_CONFIGURATION),
                    &mSystemConfiguration
                    );
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Fail to update TPM NV Variable!(0x%x)\n", Status));
      return Status;
    }

    //
    // Pass changed uncommitted data back to Form Browser
    // 
    //SetBrowserData (NULL, NULL, sizeof (mSystemConfiguration), (UINT8 *) &mSystemConfiguration, NULL);
  }

  return EFI_SUCCESS;
}


STATIC
EFI_STATUS
EnableMor (
  IN     BOOLEAN                   MorActive
  )
{
  EFI_STATUS                       Status;
  UINTN                            VarSize;
  UINT32						   Attributes;

  // Get current Mor configuration
  VarSize = sizeof (SYSTEM_CONFIGURATION);  
  Status = gRT->GetVariable(mVariableName, &mSystemConfigurationGuid, &Attributes, &VarSize, &mSystemConfiguration);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[Mor] MOR NV Variable not found!(0x%x) \n", Status));
    return Status;
  }

  //
  // Update MOR variable
  //
  mSystemConfiguration.MorState = MorActive;
  Status = gRT->SetVariable (
                  mVariableName,
                  &mSystemConfigurationGuid,
                  Attributes,
                  sizeof(SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[Mor] Fail to update MOR NV Variable!(0x%x)\n", Status));
    return Status;
  }
  return EFI_SUCCESS;
}

/* TBD_TBD_TBD
EFI_STATUS
SecurityRunPasswordStateMachine (
  IN  UINT8                           State,
  IN  EFI_PASSWORD_DATA               *SecurityData,
  IN  EFI_IFR_DATA_ARRAY              *Data,
  OUT EFI_HII_CALLBACK_PACKET         **Packet
  )
*/
/*++

Routine Description:
  Single-steps the Password Update state machine.
  The Setup Browser manages the state of the machine.

Arguments:

  State         -  current state.
  SecurityData  -  Local data that holds info on the accounts and their states.
  Data          -  The data structure as passed to the callback from the browser.
  Packet        -  Packet as returned to the browser from the callback.

Returns:

  EFI_SUCCESS  -  If step is sucessful.

--*/
/* TBD_TBD_TBD
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;
  //
  // Single-step the state machine.
  //
  switch (State) {

  case 0:
    //
    // This state tells the browser to either ask for the current password or a new password.
    //
    if (((CHAR16 *) SecurityData->PasswordLocation)[0] != 0) {
      //
      // Prompt for current password - error other than EFI_NOT_READY.
      //
      Status = EFI_NOT_AVAILABLE_YET;
    } else {
      //
      // No current password, because there is no account. Prompt for new password.
      //
      Status = EFI_SUCCESS;
    }
    break;

  case 1:
    EfiZeroMem ((CHAR16 *) SecurityData->EncodedPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiStrCpy ((CHAR16 *) SecurityData->EncodedPassword, (CHAR16 *) Data->Data->Data);
    EncodePassword ((CHAR16 *) SecurityData->EncodedPassword);
    //
    // This state authenticates the current password.
    //
    Status = (EfiCompareMem (
                (CHAR16 *) SecurityData->PasswordLocation,
                (CHAR16 *) SecurityData->EncodedPassword,
                PASSWORD_MAX_SIZE * sizeof (CHAR16)
                ) != 0) ? EFI_NOT_READY : EFI_SUCCESS;

    if (EFI_ERROR (Status)) {
      //
      // Dislay wrong password
      //
      GetStringFromToken (
        &mSystemConfigurationGuid,
        STRING_TOKEN (STR_SECURITY_PASSWORD_INVALID),
        (CHAR16 **) Packet
        );
    }
    break;

  case 2:
    //
    // This state is the opportunity to ask to prompt for new password.
    // Prompt for new password.
    //
    Status = EFI_SUCCESS;
    break;

  case 3:
    //
    // This state handles the new password. Check for valid length and set a new one
    //
    if ((
        (EfiStrLen ((CHAR16 *) Data->Data->Data) < 6) ||
        (EfiStrLen ((CHAR16 *) Data->Data->Data) > PASSWORD_MAX_SIZE)
          ) &&
        ((((CHAR16 *) Data->Data->Data)[0] != 0))
          ) {
      //
      // Dislay invalid length
      //
      GetStringFromToken (
        &mSystemConfigurationGuid,
        STRING_TOKEN (STR_SECURITY_PASSWORD_LENGTH),
        (CHAR16 **) Packet
        );
      Status = EFI_NOT_READY;
    } else {
      //
      // Copy the password. Need it for confirmation comparing.
      //
      EfiStrCpy (SecurityData->TempPassword, (CHAR16 *) Data->Data->Data);
      Status = EFI_SUCCESS;
    }
    break;

  case 4:
    //
    // This state handles the confirmation of the new password.
    // If the password is confirmed it is applied.
    //
    Status = (EfiStrCmp (SecurityData->TempPassword, (CHAR16 *) Data->Data->Data) != 0) ? EFI_NOT_READY : EFI_SUCCESS;

    if (EFI_ERROR (Status)) {
      //
      // The password confirmation failed.
      //
      *Packet = NULL;
      Status  = EFI_NOT_READY;
    } else {
      //
      // If here, attempt to apply the new password.
      //
      EfiZeroMem ((CHAR16 *) SecurityData->PasswordLocation, PASSWORD_MAX_SIZE * sizeof (CHAR16));
      EfiStrCpy ((CHAR16 *) SecurityData->PasswordLocation, (CHAR16 *) Data->Data->Data);

      if (((CHAR16 *) SecurityData->PasswordLocation)[0] != 0) {
        EncodePassword ((CHAR16 *) SecurityData->PasswordLocation);
      }

      Status = gRT->SetVariable (
                      mVariableName,
                      &mSystemConfigurationGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      sizeof (SYSTEM_CONFIGURATION),
                      &mSystemConfiguration
                      );

      //
      // Update the fake NV map.
      //
      EfiCopyMem (Data->NvRamMap, &mSystemConfiguration, sizeof (SYSTEM_CONFIGURATION));

    }

    break;
  };

  return Status;
}
*/

/* TBD_TBD_TBD
EFI_STATUS
PromptPasswordStateMachine (
  IN  UINT8                           State,
  IN  EFI_PASSWORD_DATA               *SecurityData,
  IN  EFI_IFR_DATA_ARRAY              *Data,
  OUT EFI_HII_CALLBACK_PACKET         **Packet
  )
*/
/*++

Routine Description:
  Single-steps the Password Update state machine.
  The Setup Browser manages the state of the machine.

Arguments:

  State         -  current state.
  SecurityData  -  Local data that holds info on the accounts and their states.
  Data          -  The data structure as passed to the callback from the browser.
  Packet        -  Packet as returned to the browser from the callback.

Returns:

  EFI_SUCCESS  -  If step is sucessful.

--*/
/* TBD_TBD_TBD
{
  EFI_STATUS  Status;
  UINT16      AdminState;

  Status = EFI_SUCCESS;
  //
  // Single-step the state machine.
  //
  switch (State) {

  case 0:
    //
    // This state tells the browser to either ask for the current password
    //
    if (((CHAR16 *) SecurityData->PasswordLocation)[0] != 0) {
      //
      // Prompt for current password - error other than EFI_NOT_READY.
      //
      Status = EFI_NOT_AVAILABLE_YET;
    }
    break;

  case 1:
    EfiZeroMem ((CHAR16 *) SecurityData->EncodedPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiStrCpy ((CHAR16 *) SecurityData->EncodedPassword, (CHAR16 *) Data->Data->Data);
    EncodePassword ((CHAR16 *) SecurityData->EncodedPassword);
    //
    // This state authenticates the current password.
    //
    Status = (EfiCompareMem (
                (CHAR16 *) SecurityData->PasswordLocation,
                (CHAR16 *) SecurityData->EncodedPassword,
                PASSWORD_MAX_SIZE * sizeof (CHAR16)
                ) != 0) ? EFI_NOT_READY : EFI_SUCCESS;

    if (EFI_ERROR (Status)) {
      //
      // Dislay wrong password
      //
      GetStringFromToken (
        &mSystemConfigurationGuid,
        STRING_TOKEN (STR_SECURITY_PASSWORD_INVALID),
        (CHAR16 **) Packet
        );

    } else {
      //
      // Display that the password has been cleared
      //
      GetStringFromToken (
        &mSystemConfigurationGuid,
        STRING_TOKEN (STR_SECURITY_PASSWORD_VALID),
        (CHAR16 **) Packet
        );
      //
      // Clear the password indicator
      //
      AdminState = 0;
      gRT->SetVariable (
            VAR_EQ_ADMIN_DECIMAL_NAME,
            &mSystemConfigurationGuid,
            EFI_VARIABLE_BOOTSERVICE_ACCESS,
            2,
            (VOID *) &AdminState
            );

    }
    //
    // Bail out of this
    //
    Status = EFI_NOT_READY;
    break;

  };

  return Status;
}
*/

/* TBD_TBD_TBD
EFI_STATUS
EFIAPI
SecurityDriverCallback (
  IN  EFI_FORM_CALLBACK_PROTOCOL       *This,
  IN  UINT16                           KeyValue,
  IN  EFI_IFR_DATA_ARRAY               *Data,
  OUT EFI_HII_CALLBACK_PACKET          **Packet
  )
*/
/*++

Routine Description:

  This is the function that is called to provide results data to the driver.  This data
  consists of a unique key which is used to identify what data is either being passed back
  or being asked for. 

Arguments:

  This      -  Pointer to instance of Efi Form Callback protocol interface.
  KeyValue  -  A unique value which is sent to the original exporting driver so that it
               can identify the type of data to expect.  The format of the data tends to
               vary based on the op-code that geerated the callback.
  Data      -  A pointer to the data being sent to the original exporting driver.
  Packet    -  Returned data packets.
  
Returns: 

  EFI_SUCCESS  -  Function has completed successfully.
  Other        -  Error occurred during execution.
      
--*/
/* TBD_TBD_TBD
{
  EFI_STATUS        Status;
  STATIC UINT8      State;
  EFI_PASSWORD_DATA *SecurityData;
  UINT8             TpmState;
//  EFI_TCG_PROTOCOL  *TcgProtocol;

  Status        = EFI_SUCCESS;
  SecurityData  = NULL;

  DEBUG ((EFI_D_ERROR, "[TPM] SecurityDriverCallback! \n"));

  switch (KeyValue) {
  case 0x1000:
    SecurityData = &mAdminPassword;
    //
    // Test if this call is about announcing next state. It would carry the state byte.
    //
    if (Data->Data->Length == 3) {
      //
      // Save the next state. This handler will be called again with this new state context.
      //
      State = (UINT8) (UINTN) Data->Data->Data;
      //
      // Exception to the above rule - for some states, is already the state call (not just an anouncement).
      //
      if ((State == 0) || (State == 2)) {
        Status = SecurityRunPasswordStateMachine (State, SecurityData, Data, Packet);
      } else {
        Status = EFI_SUCCESS;
      }
    } else {
      //
      // If here, this is a state call.
      //
      Status = SecurityRunPasswordStateMachine (State, SecurityData, Data, Packet);
    }
    break;

  case 0x1001:
    SecurityData = &mUserPassword;
    //
    // Test if this call is about announcing next state. It would carry the state byte.
    //
    if (Data->Data->Length == 3) {
      //
      // Save the next state. This handler will be called again with this new state context.
      //
      State = (UINT8) (UINTN) Data->Data->Data;
      //
      // Exception to the above rule - for some states, is already the state call (not just an anouncement).
      //
      if ((State == 0) || (State == 2)) {
        Status = SecurityRunPasswordStateMachine (State, SecurityData, Data, Packet);
      } else {
        Status = EFI_SUCCESS;
      }
    } else {
      //
      // If here, this is a state call.
      //
      Status = SecurityRunPasswordStateMachine (State, SecurityData, Data, Packet);
    }
    break;

  case 0x1003:

    SecurityData = &mAdminPassword;

    //
    // Test if this call is about announcing next state. It would carry the state byte.
    //
    if (Data->Data->Length == 3) {
      //
      // Save the next state. This handler will be called again with this new state context.
      //
      State = (UINT8) (UINTN) Data->Data->Data;

      //
      // Exception to the above rule - for some states, is already the state call (not just an anouncement).
      //
      if ((State == 0) || (State == 2)) {
        Status = PromptPasswordStateMachine (State, SecurityData, Data, Packet);
      } else {
        Status = EFI_SUCCESS;
      }
    } else {
      //
      // If here, this is a state call.
      //
      Status = PromptPasswordStateMachine (State, SecurityData, Data, Packet);
    }
    break;

  case 0x1002:
    EfiZeroMem (&mSystemConfiguration.AdminPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiZeroMem (&mSystemConfiguration.UserPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiZeroMem (&mAdminPassword.EncodedPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiZeroMem (&mAdminPassword.TempPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiZeroMem (&mUserPassword.EncodedPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    EfiZeroMem (&mUserPassword.TempPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    Status = gRT->SetVariable (
                    mVariableName,
                    &mSystemConfigurationGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    sizeof (SYSTEM_CONFIGURATION),
                    &mSystemConfiguration
                    );
    //
    // Update the fake NV map.
    //
    EfiCopyMem (Data->NvRamMap, &gEfiSetupVariableGuid, sizeof (SYSTEM_CONFIGURATION));

    Status = EFI_SUCCESS;
    break;

  case 0x1004:    
    //
    // Enable TPM device
    //
    TpmState = (UINT8) (UINTN) Data->Data->Data;

    Status = EnableTpmDevice (TRUE);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when enable TPM Device!(0x%x) \n", Status));
      return Status;
    }    
    break;

  case 0x1005:    
    //
    // Disable TPM device
    //
    TpmState = (UINT8) (UINTN) Data->Data->Data;

    Status = EnableTpmDevice (FALSE);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when disable TPM Device!(0x%x) \n", Status));
      return Status;
    }    
    break;
    
  case 0x1006:    
    Status = TpmForceClear ();
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when force clear TPM!(0x%x) \n", Status));
      return Status;
    }
    break;

  case 0x1007:
    //
    // Activate TPM device
    //
    TpmState = (UINT8) (UINTN) Data->Data->Data;

    Status = ActivateTpmDevice (TpmState);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when activate TPM Device!(0x%x) \n", Status));
      return Status;
    }    
    break;

  case 0x1008:
    //
    // Deactivate TPM device
    //
    TpmState = (UINT8) (UINTN) Data->Data->Data;

    Status = ActivateTpmDevice (TpmState);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when deactivate TPM Device!(0x%x) \n", Status));
      return Status;
    }    
    break;

  case 0x1009:
    //
    // Enable MOR
    //
    Status = EnableMor (TRUE);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[MOR] Error occurs when enable Mor!(0x%x) \n", Status));
      return Status;
    }    
    break;

  case 0x1010:
    //
    // Disable MOR
    //
    Status = EnableMor (FALSE);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[MOR] Error occurs when disable Mor!(0x%x) \n", Status));
      return Status;
    }    
    break;

  default:
    Status = EFI_SUCCESS;
    break;
  };

  return Status;
}
*/
EFI_STATUS
SetPasswordState (
  IN  EFI_PASSWORD_DATA               *SecurityData
  )
/*++

Routine Description:

  Set password state.

Arguments:

  SecurityData  -  Proposed password state to set to.

Returns:

  EFI_INVALID_PARAMETER  -  Invalid parameter passed in.
  EFI_SUCCESS            -  State has been set successfully.
  Others                 -  Error occurred during state setting.

--*/
{
  UINT8  State;

  switch (SecurityData->PasswordType) {
  case EfiAdminPassword:
    State = (UINT8) ((((CHAR16 *) SecurityData->PasswordLocation)[0] == 0) ?  0 : (((CHAR16 *) SecurityData->PasswordLocation)[0]!= 0x0053) ? 1 : 0);
    return gRT->SetVariable (
                  VAR_EQ_ADMIN_DECIMAL_NAME,
                  &mSystemConfigurationGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  1,
                  (VOID *) &State
                  );
    break;

   case EfiUserPassword:
    State = (UINT8) ((((CHAR16 *) SecurityData->PasswordLocation)[0] == 0) ?  0 :(((CHAR16 *) SecurityData->PasswordLocation)[0]!= 0x0053) ? 1 : 0);
// If Q - This is used for encoding Password

    return gRT->SetVariable (
                  VAR_EQ_USER_DECIMAL_NAME,
                  &mSystemConfigurationGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  1,
                  (VOID *) &State
                  );
    break;

  default:
    break;
  };

  return EFI_INVALID_PARAMETER;
}

VOID
EncodePassword (
  IN  CHAR16                      *Password
  )
/*++

Routine Description:

  Encode password passed in.

Arguments:

  Password  -  The password to encode.

Returns:

  None.

--*/
{
  UINTN   Index;
  UINTN   Loop;
  CHAR16  *Buffer;
  CHAR16  *Key;

  Key     = L"AHGBEZ1U3Q";

  Buffer  = AllocateZeroPool (PASSWORD_MAX_SIZE * sizeof (CHAR16));

  ASSERT (Buffer);

  Index = StrLen(Key) - 1; 
  for (Loop = 0; Loop < PASSWORD_MAX_SIZE; Loop++) {
    Buffer[Loop] = (CHAR16) ((Password[Loop] ^ Key[Index])+2); // 2 added to, set password starting with 'Q' and only char 'Q is used for encoding
  }

  CopyMem (Password, Buffer, PASSWORD_MAX_SIZE * sizeof (CHAR16));

  gBS->FreePool (Buffer);
  return ;
}

#define TPM_PP_USER_ABORT           ((TPM_RESULT)(-0x10))
#define TPM_PP_BIOS_FAILURE         ((TPM_RESULT)(-0x0f))

#define H2NS(x) ((((x) << 8) | ((x) >> 8)) & 0xffff)
#define H2NL(x) (H2NS ((x) >> 16) | (H2NS ((x) & 0xffff) << 16))

STATIC
TPM_RESULT
TpmCommandNoReturnData (
  IN     EFI_TCG_PROTOCOL          *TcgProtocol,
  IN     TPM_COMMAND_CODE          Ordinal,
  IN     UINTN                     AdditionalParameterSize,
  IN     VOID                      *AdditionalParameters
  )
{
  EFI_STATUS                       Status;
  TPM_RQU_COMMAND_HDR              *TpmRqu;
  TPM_RSP_COMMAND_HDR              TpmRsp;
  UINT32                           Size;

  TpmRqu = (TPM_RQU_COMMAND_HDR*)AllocatePool (
                                   sizeof (*TpmRqu) + AdditionalParameterSize
                                   );
  ASSERT (TpmRqu != NULL);
  if (TpmRqu == NULL) {
    return TPM_PP_BIOS_FAILURE;
  }

  TpmRqu->tag       = H2NS (TPM_TAG_RQU_COMMAND);
  Size              = (UINT32)(sizeof (*TpmRqu) + AdditionalParameterSize);
  TpmRqu->paramSize = H2NL (Size);
  TpmRqu->ordinal   = H2NL (Ordinal);
  gBS->CopyMem (TpmRqu + 1, AdditionalParameters, AdditionalParameterSize);

  Status = TcgProtocol->PassThroughToTpm (
                          TcgProtocol,
                          Size,
                          (UINT8*)TpmRqu,
                          (UINT32)sizeof (TpmRsp),
                          (UINT8*)&TpmRsp
                          );
  ASSERT_EFI_ERROR (Status);
  ASSERT (TpmRsp.tag == H2NS (TPM_TAG_RSP_COMMAND));
  return H2NL (TpmRsp.returnCode);
}

STATIC
EFI_STATUS
EnableTpmDevice (
  IN     BOOLEAN                   TpmEnable
  )
{
  EFI_STATUS                       Status;
  EFI_TCG_PROTOCOL                 *TcgProtocol;
  UINTN                            VarSize;
  UINT32						   Attributes;

  // Load TCG protocol
  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, &TcgProtocol);
  ASSERT_EFI_ERROR (Status);

  // Get current TPM configuration
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable(mVariableName, &mSystemConfigurationGuid, &Attributes, &VarSize, &mSystemConfiguration);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] TPM NV Variable not found!(0x%x) \n", Status));
    return Status;
  }    

  mSystemConfiguration.TpmEnable= TpmEnable;

  //
  // Enable/Disable TPM
  //
  if (TpmEnable == TRUE) {
    Status = TpmCommandNoReturnData (
               TcgProtocol, 
               TPM_ORD_PhysicalEnable,
               0,
               NULL
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Fail to enable TPM device!\n"));
      return Status;
    }
  } else {
    Status = TpmCommandNoReturnData (
               TcgProtocol, 
               TPM_ORD_PhysicalDisable,
               0,
               NULL
               );
    
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Fail to disable TPM device!\n"));
      return Status;
    }  
  }
    
  // Update TPM variable
  Status = gRT->SetVariable (
                  mVariableName,
                  &mSystemConfigurationGuid,
                  Attributes,
                  sizeof(SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] Fail to update TPM NV Variable!(0x%x)\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
ActivateTpmDevice (
  IN     BOOLEAN                   TpmActive
  )
{
  EFI_STATUS                       Status;
  EFI_TCG_PROTOCOL                 *TcgProtocol;
  UINTN                            VarSize;
  BOOLEAN                          BoolVal;
  UINT32						   Attributes;

  // Load TCG protocol
  Status = gBS->LocateProtocol (&gEfiTcgProtocolGuid, NULL, &TcgProtocol);
  ASSERT_EFI_ERROR (Status);

  // Get current TPM configuration
  VarSize = sizeof (SYSTEM_CONFIGURATION);
  Status = gRT->GetVariable(mVariableName, &mSystemConfigurationGuid, &Attributes, &VarSize, &mSystemConfiguration);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] TPM NV Variable not found!(0x%x) \n", Status));
    return Status;
  }    

  mSystemConfiguration.TpmState = TpmActive;
  //
  // Activate/Deactivate TPM
  //
  if (TpmActive == TRUE) {
    BoolVal = FALSE;
    Status = TpmCommandNoReturnData (
               TcgProtocol, 
               TPM_ORD_PhysicalSetDeactivated,
               sizeof (BoolVal),
               &BoolVal
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Fail to activate TPM device!\n"));
      return Status;
    }
  } else {
    BoolVal = TRUE;
    Status = TpmCommandNoReturnData (
               TcgProtocol, 
               TPM_ORD_PhysicalSetDeactivated,
               sizeof (BoolVal),
               &BoolVal
               );
    
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Fail to deactivate TPM device!\n"));
      return Status;
    }  
  }
    
  // Update TPM variable
  Status = gRT->SetVariable (
                  mVariableName,
                  &mSystemConfigurationGuid,
                  Attributes,
                  sizeof(SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_ERROR, "[TPM] Fail to update TPM NV Variable!(0x%x)\n", Status));
    return Status;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
ValidatePassword (
  EFI_STRING_ID                   StringId
  )
{
  EFI_STATUS                      Status;
  CHAR16                          Password[PASSWORD_MAX_SIZE];
  EFI_STRING                      TempPassword;

  if (((CHAR16 *) SecurityData->PasswordLocation)[0] == 0) {
    //
    // Old password not exist
    //
    return EFI_SUCCESS;
  }

  //
  // Get user input password
  //
  TempPassword = HiiGetString (mSecurityHiiHandle, StringId, NULL);
  if (TempPassword == NULL) {
    return EFI_NOT_READY;
  }
  if (StrLen (TempPassword) > PASSWORD_MAX_SIZE) {
    FreePool (TempPassword);
    return EFI_NOT_READY;
  }

  StrnCpy (Password, TempPassword, PASSWORD_MAX_SIZE);
  FreePool (TempPassword);

  //
  // Validate old password
  //
  EncodePassword (Password);
  if (CompareMem (Password, SecurityData->PasswordLocation, PASSWORD_MAX_SIZE * sizeof (CHAR16)) != 0) {
    //
    // Old password mismatch, return EFI_NOT_READY to prompt for error message
    //
    Status = EFI_NOT_READY;
  } else {
    Status = EFI_SUCCESS;
  }

  return Status;
}

EFI_STATUS
SetPassword (
  EFI_STRING_ID                   StringId
  )
{
  EFI_STATUS                      Status;
  EFI_STRING                      Password;

  //
  // Get user input password
  //
  if (StringId == 0) {
    return EFI_NOT_READY;
  }
  Password = HiiGetString (mSecurityHiiHandle, StringId, NULL);
  if (Password == NULL) {
    return EFI_NOT_READY;
  }
  if (StrLen (Password) > PASSWORD_MAX_SIZE) {
    FreePool (Password);
    return EFI_NOT_READY;
  }
  StrnCpy (SecurityData->PasswordLocation, Password, PASSWORD_MAX_SIZE);
  FreePool (Password);

  //
  // Encode and set password
  //
  EncodePassword (SecurityData->PasswordLocation);
  Status = gRT->SetVariable(
                  mVariableName,
                  &mSystemConfigurationGuid,
				  SETUP_VARIABLE_ATTRIBUTES,
                  sizeof (SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );

  return Status;
}
VOID
ClearPassword(
)
{
  EFI_STATUS                      Status;
  EFI_STRING                      Password;

  // Null password

  Password  = AllocateZeroPool (PASSWORD_MAX_SIZE * sizeof (CHAR16));
  //
  // Dont Encode and set password
  //
  StrnCpy (SecurityData->PasswordLocation, Password, PASSWORD_MAX_SIZE);

  Status = gRT->SetVariable(
                  mVariableName,
                  &mSystemConfigurationGuid,
				  SETUP_VARIABLE_ATTRIBUTES,
                  sizeof (SYSTEM_CONFIGURATION),
                  &mSystemConfiguration
                  );

}

STATIC
VOID
SetAdminIndicatorAndSavePassword (
  IN UINT8                      State
  )
{
  EFI_STATUS                    Status;

  Status = gRT->SetVariable(
                  VAR_EQ_ADMIN_DECIMAL_NAME,
                  &mSystemConfigurationGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  1,
                  &State
                  );
  ASSERT_EFI_ERROR (Status);
  
  // 
  // Update setup buffer with changed value
  //
  HiiSetBrowserData (&mSystemConfigurationGuid, VAR_EQ_ADMIN_DECIMAL_NAME, 1, &State, NULL);
}

STATIC
VOID
SetUserIndicatorAndSavePassword (
  IN UINT8                      State 
  )
{
  EFI_STATUS                    Status;

  Status = gRT->SetVariable(
                  VAR_EQ_USER_DECIMAL_NAME,
                  &mSystemConfigurationGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  1,
                  &State
                  );
  ASSERT_EFI_ERROR (Status);
  
  // 
  // Update setup buffer with changed value
  //
  HiiSetBrowserData (&mSystemConfigurationGuid, VAR_EQ_USER_DECIMAL_NAME, 1, &State, NULL);  
}

EFI_STATUS
EFIAPI
SecurityDriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_STATUS                                 Status;
  EFI_STRING                                 PasswordString = NULL;
  
  
  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }

  if ((Value == NULL) || (ActionRequest == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;

  switch (QuestionId) {
  case 0x1000:
    SecurityData = &mAdminPassword;
    switch (State) {
    case BROWSER_STATE_VALIDATE_PASSWORD:
      Status = ValidatePassword (Value->string);
      if (Status == EFI_SUCCESS) {
        State = BROWSER_STATE_SET_PASSWORD;
      } else {
        return EFI_NOT_READY;
      }
      break;

    case BROWSER_STATE_SET_PASSWORD:

      // User had Pressed only the Null Character , So update the statue as Admin password not set
      if(Value->string != 0){
        PasswordString = HiiGetString (mSecurityHiiHandle, Value->string, NULL);
        if (*PasswordString == 00)
          {
           //Clear User P/W also
           SecurityData = &mUserPassword;
           ClearPassword();
           SetUserIndicatorAndSavePassword (0x00);

           SecurityData = &mAdminPassword;
           ClearPassword();
           SetAdminIndicatorAndSavePassword (0x00);
           State = BROWSER_STATE_SET_PASSWORD;
           break;
          }
      Status = SetPassword (Value->string);
      SetAdminIndicatorAndSavePassword (0x01);
      }
      State = BROWSER_STATE_VALIDATE_PASSWORD;
      break;

    default:
      break;
    }
    break;

  case 0x1001:
    SecurityData = &mUserPassword;
    switch (State) {
    case BROWSER_STATE_VALIDATE_PASSWORD:
      Status = ValidatePassword (Value->string);
      if (Status == EFI_SUCCESS) {
        State = BROWSER_STATE_SET_PASSWORD;
      } else {
        return EFI_NOT_READY;
      }
      break;

    case BROWSER_STATE_SET_PASSWORD:

      // User has Pressed only the Null Character , So update the statue as password not set
      if(Value->string != 0){
        PasswordString = HiiGetString (mSecurityHiiHandle, Value->string, NULL);
        if (*PasswordString == 00)
          {
           ClearPassword(); // When USER pressed the enter key Clear the password
           SetUserIndicatorAndSavePassword (0x00);
           State = BROWSER_STATE_SET_PASSWORD;
           break;
          }
      Status = SetPassword (Value->string);

        SetUserIndicatorAndSavePassword (0x01); 
       }
      State = BROWSER_STATE_VALIDATE_PASSWORD;
      break;

    default:
      break;
    }
    break;

  case 0x1002:
    //
    // Clear all the password
    //
    ZeroMem (&mSystemConfiguration.AdminPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    ZeroMem (&mSystemConfiguration.UserPassword, PASSWORD_MAX_SIZE * sizeof (CHAR16));
    Status = gRT->SetVariable(
                    mVariableName,
                    &mSystemConfigurationGuid,
					SETUP_VARIABLE_ATTRIBUTES,
                    sizeof (SYSTEM_CONFIGURATION),
                    &mSystemConfiguration
                    );
    return EFI_NOT_AVAILABLE_YET;

  case 0x1003:
    SecurityData = &mAdminPassword;
    Status = ValidatePassword (Value->string);
    if (Status == EFI_SUCCESS) {
      //
      // Do clear password action
      //
      ZeroMem (SecurityData->PasswordLocation, PASSWORD_MAX_SIZE * sizeof(UINT16));
      gRT->SetVariable(
             mVariableName,
             &mSystemConfigurationGuid,
			 SETUP_VARIABLE_ATTRIBUTES,       
			 sizeof (SYSTEM_CONFIGURATION),
             &mSystemConfiguration
             );
      SetAdminIndicatorAndSavePassword (0x0);
      return EFI_NOT_AVAILABLE_YET;
    } else {
      //
      // Not match the password
      //
      return EFI_NOT_READY;
    }
    break;

  default:
    break;
  }

  return Status;
}


EFI_STATUS
InstallSecurityCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  ASSERT (ConfigAccess != NULL);

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = SecurityDriverCallback;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  if (((CHAR16 *)mAdminPassword.PasswordLocation)[0] != 0) {
    SetAdminIndicatorAndSavePassword (0x01);
  } else if (((CHAR16 *)mUserPassword.PasswordLocation)[0] != 0) {
    SetUserIndicatorAndSavePassword (0x01);
  } else {
    SetAdminIndicatorAndSavePassword (0x00);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
TpmDriverCallback (
  IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
  IN  EFI_BROWSER_ACTION                     Action,
  IN  EFI_QUESTION_ID                        QuestionId,
  IN  UINT8                                  Type,
  IN  EFI_IFR_TYPE_VALUE                     *Value,
  OUT EFI_BROWSER_ACTION_REQUEST             *ActionRequest  OPTIONAL
  )
{
  EFI_STATUS                                 Status;
  UINTN                                      VarSize;
  UINT32									 Attributes;
  BOOLEAN                                    TpmEnable;
  BOOLEAN                                    TpmActivated;
//  BOOLEAN                                    PhysicalPresenceLock;
//  EFI_STRING                                 String;
    
  Status = EFI_SUCCESS;

  if (Action == EFI_BROWSER_ACTION_FORM_OPEN) {
    //
    // Upate TPM state when TPM Form is opened.
    // This will be done only in FORM_OPEN CallBack of question with ID 0x1000 from TPM Form.
    //
    if (QuestionId != 0x1000) {
      return EFI_SUCCESS;
    }

    //
    // Get the TPM state
    //
    Status = GetTpmState (&TpmEnable, &TpmActivated, NULL, NULL, NULL);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Get GetTpmState error Status = %r\n", Status));
      return EFI_SUCCESS;
    }

    //
    // Update TPM variable if it differs from current TPM hardware state
    //
    VarSize = sizeof (SYSTEM_CONFIGURATION);
    Status  = gRT->GetVariable (mVariableName, &mSystemConfigurationGuid, &Attributes, &VarSize, &mSystemConfiguration);  
    if (EFI_ERROR (Status) || 
        (mSystemConfiguration.TpmEnable != TpmEnable) || 
        (mSystemConfiguration.TpmState  != TpmActivated)/* ||
        (mSystemConfiguration.PhysicalPresenceLock != PhysicalPresenceLock)*/) {

      mSystemConfiguration.TpmEnable            = TpmEnable; 
      mSystemConfiguration.TpmState             = TpmActivated;
//      mSystemConfiguration.PhysicalPresenceLock = PhysicalPresenceLock;
      Status = gRT->SetVariable (
                      mVariableName,
                      &mSystemConfigurationGuid,
                      Attributes,
                      sizeof(SYSTEM_CONFIGURATION),
                      &mSystemConfiguration
                      );
      if (EFI_ERROR(Status)) {
        return Status;
      }
    }

    /*
    //
    // Update TPM physical presence lock state information in UI
    // 
    if (mSystemConfiguration.PhysicalPresenceLock) {
      String = HiiGetString (mTpmHiiHandle, STRING_TOKEN (STR_TPM_PHYSICAL_PRESENCE_LOCKED), NULL);
      if (String != NULL) {
        HiiSetString (mTpmHiiHandle, STRING_TOKEN (STR_TPM_PHYSICAL_PRESENCE_UNLOCKED), String, NULL); 
        gBS->FreePool (String);
      }
    }; 
    */
    return EFI_SUCCESS;
  }

  if (Action == EFI_BROWSER_ACTION_FORM_CLOSE) {
    //
    // Do nothing for UEFI CLOSE Action
    //
    return EFI_SUCCESS;
  }

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    //
    // Do nothing for other UEFI Action. Only do call back when data is changed.
    //
    return EFI_UNSUPPORTED;
  }

  switch (QuestionId) {

  case 0x1000:
    //
    // Enable/disable TPM device
    //
    Status = EnableTpmDevice (Value->u8);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when enable/disable TPM!(0x%x) \n", Status));
      return Status;
    }
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_SUBMIT;
    break;

  case 0x1001:
    //
    // Deactivate TPM device
    //
    Status = ActivateTpmDevice (Value->u8);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when activate/deactivate TPM!(0x%x) \n", Status));
      return Status;
    }
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_SUBMIT;
    break;
  

  case 0x1002:
    //
    // TPM Force Clear
    //
    Status = TpmForceClear ();
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[TPM] Error occurs when force clear TPM!(0x%x) \n", Status));
      return Status;
    }
    *ActionRequest = EFI_BROWSER_ACTION_REQUEST_SUBMIT;
    break;
    
  case 0x1003:
    //
    // Enable MOR
    //
    Status = EnableMor (Value->u8);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_ERROR, "[MOR] Error occurs when enable/disable Mor!(0x%x) \n", Status));
      return Status;
    }    
    break;

  default:
    Status = EFI_SUCCESS;
    break;
  }

  return Status;
}


EFI_STATUS
InstallTpmCallbackRoutine (
  IN EFI_HANDLE                     DriverHandle,
  IN EFI_HII_CONFIG_ACCESS_PROTOCOL *ConfigAccess
  )
{
  EFI_STATUS          Status;

  ASSERT (ConfigAccess != NULL);

  ConfigAccess->ExtractConfig = DefaultExtractConfig;
  ConfigAccess->RouteConfig   = DefaultRouteConfig;
  ConfigAccess->Callback      = TpmDriverCallback;

  Status = gBS->InstallProtocolInterface (
                  &DriverHandle,
                  &gEfiHiiConfigAccessProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  ConfigAccess
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
