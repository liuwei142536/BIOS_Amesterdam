//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
/** @file AmiNvmeLib.c
    Ami Nvme Library module for Nvme device access

**/

#include "AmiNvmeLibPrivate.h"

/**
@Internal
    Returns Locking Feature and BlockSid Feature data of NVMe device

    @param [in]  Level0BufferData     Points to the TCG_OPAL_LEVEL0_DISCOVERY_DATA.
    @param [out] OpalFeatureList      Points to the TCG_OPAL_FEATURE_LIST which contains features list.

    @retval EFI_SUCCESS                 Locking and BlockSid feature returned successfully.
    @retval EFI_NOT_FOUND               Locking and BlockSid feature not supported.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
@endInternal

**/

EFI_STATUS
AmiNvmeGetSecurityStatus (
    IN  TCG_OPAL_LEVEL0_DISCOVERY_DATA   *Level0BufferData,
    OUT TCG_OPAL_FEATURE_LIST            *OpalFeatureList
)
{
    EFI_STATUS                        Status = EFI_NOT_FOUND;
    TCG_OPAL_FEATURE_DESCRIPTOR       *FeatureDescriptor = NULL;
    UINTN                             TotalLength = 0;
    UINT16                            FeatureCode = 0;
    TCG_FEATURE_DESCRIPTOR_BLOCK_SID  *BlockSid = NULL;

    if((Level0BufferData == NULL) || (OpalFeatureList == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->SetMem(OpalFeatureList, sizeof(TCG_OPAL_FEATURE_LIST), 0);

    // Convert to big endian format and store
    TotalLength =  ToBigLittleEndianDword(Level0BufferData->Level0DiscoveryHeader.Length) + 4;
    FeatureDescriptor = (TCG_OPAL_FEATURE_DESCRIPTOR *)((UINT8 *)Level0BufferData + sizeof(TCG_OPAL_LEVEL0_DISCOVERY_HEADER));
    do {
        FeatureCode = ((FeatureDescriptor->FeatureCode >> 8) & 0xFF) + (FeatureDescriptor->FeatureCode << 8);
        // Check for the Block SID feature and Locking feature in the supported feature Code
        if( FeatureCode == BLOCK_SID_AUTHENTIFICATION_FEATURE ) {
            Status = EFI_SUCCESS;
            BlockSid = (TCG_FEATURE_DESCRIPTOR_BLOCK_SID *) FeatureDescriptor;
            OpalFeatureList->BlockSid.FeatureCode = FeatureCode;
            OpalFeatureList->BlockSid.Version = BlockSid->Version;
            OpalFeatureList->BlockSid.Length = BlockSid->Length;
            OpalFeatureList->BlockSid.HWReset = BlockSid->HWReset;
            OpalFeatureList->BlockSid.SIDBlockedState = BlockSid->SIDBlockedState;
            OpalFeatureList->BlockSid.SIDStateValue = BlockSid->SIDStateValue;
        }

        if( FeatureCode == LOCKING_FEATURE ) {
            Status = EFI_SUCCESS;
            OpalFeatureList->LockingFeature.FeatureCode = FeatureCode;
            OpalFeatureList->LockingFeature.Version = FeatureDescriptor->Version;
            OpalFeatureList->LockingFeature.Length = FeatureDescriptor->Length;
            OpalFeatureList->LockingFeature.Data[0] = FeatureDescriptor->Data[0];
        }

        // Get the Next Feature Descriptor
        FeatureDescriptor = (TCG_OPAL_FEATURE_DESCRIPTOR *)((UINT8 *)FeatureDescriptor + FeatureDescriptor->Length + 4);

    } while ((UINT8 *)FeatureDescriptor < (UINT8 *)((UINT8 *)Level0BufferData + TotalLength));

    return Status;
}

/**
@Internal
    Gets Level 0 discovery data of NVMe device.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [in]  BufferSize             Size of the Buffer.
    @param [out] Buffer                 Points to Buffer which gets Level 0 discovery data.

    @retval EFI_SUCCESS                 Level 0 discovery data retrieved successfully.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
@endInternal

**/

EFI_STATUS
AmiNvmeGetLevel0DiscoveryData (
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    IN  UINT32                                BufferSize,
    OUT VOID                                  *Buffer
)
{
    EFI_STATUS                                 Status;
    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET   NvmePacket;
    EFI_NVM_EXPRESS_COMMAND                    NvmeCommand;
    EFI_NVM_EXPRESS_COMPLETION                 NvmeCompletion;

    if((Buffer == NULL) || (EfiNvmExpressPassThru == NULL) || (BufferSize == 0)) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->SetMem(Buffer, BufferSize, 0);
    gBS->SetMem(&NvmePacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0);
    gBS->SetMem(&NvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);
    gBS->SetMem(&NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    NvmeCommand.Cdw0.Opcode = NVME_ADMIN_SECURITY_RECEIVE_CMD;
    NvmeCommand.Cdw0.FusedOperation = 0;
    NvmeCommand.Cdw10 = (SECURITY_PROTOCOL_1 << 24) + ((LEVEL0_DISCOVERY_COMID & 0xFF) << 8);
    NvmeCommand.Cdw11 = BufferSize;
    NvmeCommand.Nsid = 0;
    NvmeCommand.Flags = ( CDW10_VALID | CDW11_VALID );

    NvmePacket.CommandTimeout = 10000000; // 1sec in 100 nano sec units
    NvmePacket.QueueType = 0;
    NvmePacket.NvmeCompletion = &NvmeCompletion;
    NvmePacket.NvmeCmd = &NvmeCommand;
    NvmePacket.TransferLength = BufferSize;
    NvmePacket.TransferBuffer = Buffer;

    // Send Command through Passthru API
    Status = EfiNvmExpressPassThru->PassThru ( EfiNvmExpressPassThru,
                                               0,
                                               &NvmePacket,
                                               NULL
                                             );
    return Status;
}

/**
@Internal
    Gets supported security protocol list of NVMe device.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] Buffer                 Points to Buffer which gets Supported Protocol list.

    @retval EFI_SUCCESS                 Supported Security Protocol List retrieved successfully.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
@endInternal

**/

EFI_STATUS
AmiNvmeGetSupportedProtocolList (
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT VOID                                  *Buffer
)
{
    EFI_STATUS                                 Status;
    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET   NvmePacket;
    EFI_NVM_EXPRESS_COMMAND                    NvmeCommand;
    EFI_NVM_EXPRESS_COMPLETION                 NvmeCompletion;
    UINT32                                     BufferSize = sizeof(TCG_OPAL_SP0_TRUSTED_RECEIVE_PARAMETER_DATA);

    if((Buffer == NULL) || (EfiNvmExpressPassThru == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->SetMem(Buffer, BufferSize, 0);
    gBS->SetMem( &NvmePacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0 );
    gBS->SetMem( &NvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);
    gBS->SetMem( &NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    NvmeCommand.Cdw0.Opcode = NVME_ADMIN_SECURITY_RECEIVE_CMD;
    NvmeCommand.Cdw0.FusedOperation = 0;
    NvmeCommand.Cdw10 = SECURITY_PROTOCOL_INFORMATION;
    NvmeCommand.Cdw11 = BufferSize;
    NvmeCommand.Nsid = 0;
    NvmeCommand.Flags = ( CDW10_VALID | CDW11_VALID );

    NvmePacket.CommandTimeout = 10000000; // 1sec in 100 nano sec units
    NvmePacket.QueueType = 0;
    NvmePacket.NvmeCompletion = &NvmeCompletion;
    NvmePacket.NvmeCmd = &NvmeCommand;
    NvmePacket.TransferLength = BufferSize;
    NvmePacket.TransferBuffer = Buffer;

    // Send Command through Passthru API to get security protocol information
    Status = EfiNvmExpressPassThru->PassThru ( EfiNvmExpressPassThru,
                                               0,
                                               &NvmePacket,
                                               NULL
                                              );
    return Status;
}

/**
@Internal
    Gets Identify Controller Data of NVMe device.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] Buffer                 Points to Buffer which gets Identify Controller Data.

    @retval EFI_SUCCESS                 Identify Controller Data retrieved successfully.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
@endInternal

**/

EFI_STATUS
AmiNvmeGetIdentifyControllerData (
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT VOID                                  *Buffer
)
{
    EFI_STATUS                                 Status;
    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET   NvmePacket;
    EFI_NVM_EXPRESS_COMMAND                    NvmeCommand;
    EFI_NVM_EXPRESS_COMPLETION                 NvmeCompletion;
    UINT32                                     BufferSize = sizeof(NVME_ADMIN_CONTROLLER_DATA);

    if((Buffer == NULL) || (EfiNvmExpressPassThru == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->SetMem(Buffer, BufferSize, 0);
    gBS->SetMem( &NvmePacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0 );
    gBS->SetMem( &NvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);
    gBS->SetMem( &NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    NvmeCommand.Cdw0.Opcode = NVME_ADMIN_IDENTIFY_CMD;
    NvmeCommand.Cdw0.FusedOperation = 0;
    NvmeCommand.Cdw10 = IdentifyControllerCns;  // For Identify controller data
    NvmeCommand.Nsid = 0;
    NvmeCommand.Flags = CDW10_VALID;

    NvmePacket.CommandTimeout = 10000000;       // 1sec in 100 nano sec units
    NvmePacket.QueueType = 0;
    NvmePacket.NvmeCompletion = &NvmeCompletion;
    NvmePacket.NvmeCmd = &NvmeCommand;
    NvmePacket.TransferBuffer = Buffer;
    NvmePacket.TransferLength = BufferSize;

    // Send Command through Passthru API
    Status = EfiNvmExpressPassThru->PassThru ( EfiNvmExpressPassThru,
                                               0,
                                               &NvmePacket,
                                               NULL
                                              );
    return Status;
}

/**
@Internal
    Returns whether the NVMe device supports TCG security protocol or not.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.

    @retval TRUE        TCG Security protocol supported by the NVMe device.
    @retval FALSE       TCG Security protocol not supported by the NVMe device.
@endInternal

**/

BOOLEAN
AmiIsSecurityProtocolSupported (
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru
)
{
    EFI_STATUS                                      Status;
    NVME_ADMIN_CONTROLLER_DATA                      *IdentifyControllerData = NULL;
    UINT16                                          ListLength = 0;
    UINT16                                          SpByte = 0;
    TCG_OPAL_SP0_TRUSTED_RECEIVE_PARAMETER_DATA     SecurityProtocolList;

    if(EfiNvmExpressPassThru == NULL) {
        return FALSE;
    }

    IdentifyControllerData = AllocatePool(sizeof(NVME_ADMIN_CONTROLLER_DATA));
    if (IdentifyControllerData==NULL) {
        return FALSE;
    }

    // Get Identify Controller Data to check Security Send and Receive command support
    Status = AmiNvmeGetIdentifyControllerData(EfiNvmExpressPassThru, IdentifyControllerData);
    if(EFI_ERROR(Status)) {
        FreePool(IdentifyControllerData);
        return FALSE;
    }

    // Check OACS field for Security Send and Receive command support
    if(!(IdentifyControllerData->Oacs & SECURITY_SEND_RECEIVE_SUPPORTED)) {
        FreePool(IdentifyControllerData);
        return FALSE;
    }
    FreePool(IdentifyControllerData);

    // Get Supported Protocol List
    Status = AmiNvmeGetSupportedProtocolList(EfiNvmExpressPassThru, &SecurityProtocolList);
    if (EFI_ERROR(Status)) {
        return FALSE;
    }

    // The List Length field indicates the total length, in bytes, of the supported security protocol list.
    ListLength = (SecurityProtocolList.ListLengthHigh << 8) | SecurityProtocolList.ListLengthLow;
    if (ListLength == 0) {
        return FALSE;
    }

    for (SpByte = 0; SpByte < ListLength; SpByte++) {
        if (SecurityProtocolList.SupportedSPList[SpByte] == SECURITY_PROTOCOL_1) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
    Returns whether the NVMe device freeze locked or not.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] FreezeLocked           Flag for returning the device freeze lock status.

    @retval EFI_SUCCESS                 Device freeze lock state obtained successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol/BlockSid feature is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeFrozen(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *FreezeLocked
)
{
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    UINT8                       Buffer[0x200];
    UINT32                      BufferSize = sizeof(Buffer);
    TCG_OPAL_FEATURE_LIST       OpalFeatureList;

    if((EfiNvmExpressPassThru == NULL) || (FreezeLocked == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    *FreezeLocked = FALSE;

    // Check if TCG protocol supported or not
    if(AmiIsSecurityProtocolSupported(EfiNvmExpressPassThru)) {

        // Get Level 0 Discovery data
        Status = AmiNvmeGetLevel0DiscoveryData(EfiNvmExpressPassThru, BufferSize, Buffer);
        if (!EFI_ERROR(Status)) {
            // Get Security Status from Level 0 discovery data
            Status = AmiNvmeGetSecurityStatus((TCG_OPAL_LEVEL0_DISCOVERY_DATA*)Buffer, &OpalFeatureList);
            if (!EFI_ERROR(Status)) {
                Status = EFI_UNSUPPORTED;
                // Check BlockSid feature available or not
                if(OpalFeatureList.BlockSid.FeatureCode == BLOCK_SID_AUTHENTIFICATION_FEATURE) {
                    Status = EFI_SUCCESS;   // BlockSid feature available
                    if(OpalFeatureList.BlockSid.SIDBlockedState) {
                        *FreezeLocked = TRUE;
                    }
                }
            }
        }
    }

    return Status;
}

/**
    Returns whether the NVMe device locked or not.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] DeviceLocked           Flag for returning the device lock status.

    @retval EFI_SUCCESS                 Device lock state obtained successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeLocked(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *DeviceLocked
)
{
    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    UINT8                       Buffer[0x200];
    UINT32                      BufferSize = sizeof(Buffer);
    TCG_OPAL_FEATURE_LIST       OpalFeatureList;

    if((EfiNvmExpressPassThru == NULL) || (DeviceLocked == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    *DeviceLocked = FALSE;

    // Check if TCG protocol supported or not
    if(AmiIsSecurityProtocolSupported(EfiNvmExpressPassThru)) {
        // Get Level 0 Discovery data
        Status = AmiNvmeGetLevel0DiscoveryData(EfiNvmExpressPassThru, BufferSize, Buffer);
        if (!EFI_ERROR(Status)) {
            // Get Security Status from Level 0 discovery data
            Status = AmiNvmeGetSecurityStatus((TCG_OPAL_LEVEL0_DISCOVERY_DATA*)Buffer, &OpalFeatureList);
            if (!EFI_ERROR(Status)) {
                Status = EFI_UNSUPPORTED;
                // Check Locking Feature available or not
                if(OpalFeatureList.LockingFeature.FeatureCode == LOCKING_FEATURE) {
                    Status = EFI_SUCCESS;     // Locking Feature data obtained
                    if(OpalFeatureList.LockingFeature.Data[0] & TCG_DEVICE_SECURITY_LOCK_MASK) {
                        *DeviceLocked = TRUE;
                    }
                }
            }
        }
    }

    return Status;
}

/**
    Returns whether BlockSID command supported by the NVMe device.

    @param [in]  EfiNvmExpressPassThru  Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.
    @param [out] BlockSIDFeatureSupport Flag for returning the status of Block Sid support.

    @retval EFI_SUCCESS                 Device supports Block SID command.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             TCG Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiIsNvmeBlockSidSupported(
    IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru,
    OUT BOOLEAN                               *BlockSidFeatureSupport
)
{

    EFI_STATUS                  Status = EFI_UNSUPPORTED;
    UINT8                       Buffer[0x200];
    UINT32                      BufferSize = sizeof(Buffer);
    TCG_OPAL_FEATURE_LIST       OpalFeatureList;

    if((EfiNvmExpressPassThru == NULL) || (BlockSidFeatureSupport == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    *BlockSidFeatureSupport = FALSE;

    // Check if TCG protocol supported or not
    // If supported, then check for BlockSid support
    if(AmiIsSecurityProtocolSupported(EfiNvmExpressPassThru)) {
        // Get Level 0 Discovery data
        Status = AmiNvmeGetLevel0DiscoveryData(EfiNvmExpressPassThru, BufferSize, Buffer);
        if (!EFI_ERROR(Status)) {
            // Get Security Status from Level 0 discovery data
            Status = AmiNvmeGetSecurityStatus((TCG_OPAL_LEVEL0_DISCOVERY_DATA*)Buffer, &OpalFeatureList);
            if (!EFI_ERROR(Status)) {
                // Check BlockSid feature available or not
                if(OpalFeatureList.BlockSid.FeatureCode == BLOCK_SID_AUTHENTIFICATION_FEATURE) {
                    *BlockSidFeatureSupport = TRUE;
                }
            }
        }
    }

    return Status;
}

/**
    Issues the Enable BlockSID command for the NVMe device.

    @param [in] EfiNvmExpressPassThru   Points to the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL.

    @retval EFI_SUCCESS                 Block SID command is sent successfully.
    @retval EFI_OUT_OF_RESOURCES        Memory could not be allocated for the buffer.
    @retval EFI_UNSUPPORTED             Security protocol is not supported by the device.
    @retval EFI_INVALID_PARAMETER       One or more input parameters are invalid.
    @retval EFI_DEVICE_ERROR            Device error occurred while attempting to send the command.
    @retval EFI_TIMEOUT                 Timeout occurred while waiting for the NVM Express Command Packet to execute.

**/

EFI_STATUS
AmiNvmeIssueBlockSid(
    IN EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *EfiNvmExpressPassThru
)
{
    EFI_STATUS                                Status;
    UINT8                                     Buffer[0x200];
    UINT32                                    BufferSize = sizeof(Buffer);
    EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET  NvmePacket;
    EFI_NVM_EXPRESS_COMMAND                   NvmeCommand;
    EFI_NVM_EXPRESS_COMPLETION                NvmeCompletion;

    if(EfiNvmExpressPassThru == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    gBS->SetMem( &NvmePacket, sizeof(EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET), 0 );
    gBS->SetMem( &NvmeCommand, sizeof(EFI_NVM_EXPRESS_COMMAND), 0);
    gBS->SetMem( &NvmeCompletion, sizeof(EFI_NVM_EXPRESS_COMPLETION), 0);

    gBS->SetMem(Buffer, BufferSize, 0);
    ((UINT8 *)Buffer)[0] = 1;

    NvmeCommand.Cdw0.Opcode = NVME_ADMIN_SECURITY_SEND_CMD;
    NvmeCommand.Cdw0.FusedOperation = 0;
    NvmeCommand.Cdw10 = (SECURITY_PROTOCOL_2 << 24) + ((BLOCK_SID_COMID & 0xFF) << 8);
    NvmeCommand.Cdw11 = BufferSize;
    NvmeCommand.Nsid = 0;
    NvmeCommand.Flags = ( CDW10_VALID | CDW11_VALID );

    NvmePacket.CommandTimeout = 10000000; // 1sec in 100 nano sec units
    NvmePacket.QueueType = 0;
    NvmePacket.NvmeCompletion = &NvmeCompletion;
    NvmePacket.NvmeCmd = &NvmeCommand;
    NvmePacket.TransferLength = BufferSize;
    NvmePacket.TransferBuffer = Buffer;

    // Send Command through Passthru API
    Status = EfiNvmExpressPassThru->PassThru ( EfiNvmExpressPassThru,
                                               0,
                                               &NvmePacket,
                                               NULL
                                             );
    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
