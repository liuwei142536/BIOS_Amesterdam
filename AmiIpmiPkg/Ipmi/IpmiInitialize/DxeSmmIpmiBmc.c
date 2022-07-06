//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @file DxeSmmIpmiBmc.c
    Establishes communication to use SMM services for DXE case

**/

//----------------------------------------------------------------------

#include <Library/UefiLib.h>
#include "DxeSmmIpmiBmc.h"

//----------------------------------------------------------------------

#if defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)

#define MAXIMUM_DATA_SIZE 0xFF
#define SMM_IPMI_RUNTIME_BUFFER_SIZE 0x2FF
//
// Extern Variables
//
extern EFI_IPMI_BMC_INSTANCE_DATA   *gIpmiInstance;

//
// Global variables
//
EFI_SMM_COMMUNICATION_PROTOCOL  *gSmmCommunication = NULL;
UINT8                           *gCommunicateBuffer = NULL;
UINTN                            gCommunicateBufferSize;

UINT8                           *gCommandData;
UINT8                           *gResponseDataSize;
UINT8                           *gResponseData;
UINT8                           *gBmcStatus;
UINT8                           *gComAddress;
UINT8                           *TotalBlockSize;
/**
    IPMI send command SMM service for Non SMM case

    @param This             Pointer to IPMI Protocol/PPI
    @param NetFunction      Net function of the command
    @param LUN              Logical Unit number of the command
    @param Command          Command to be sent to BMC
    @param CommandData      Command data to be sent along with Command
    @param CommandDataSize  Command Data size
    @param ResponseData     Response Data from BMC
    @param ResponseDataSize Size of the Response data
    @param CompletionCode   Completion code returned by Bmc
    
    @return EFI_STATUS      Status returned from BMC while executing the command.

**/

EFI_STATUS
SmmIpmiSendCommand (
  IN EFI_IPMI_TRANSPORT    *This,
  IN UINT8                 NetFunction,
  IN UINT8                 Lun,
  IN UINT8                 Command,
  IN UINT8                 *CommandData,
  IN UINT8                 CommandDataSize,
  OUT UINT8                *ResponseData,
  OUT UINT8                *ResponseDataSize,
  OUT UINT8                *CompletionCode
  )
{
    EFI_STATUS                                    Status;
    EFI_SMM_COMMUNICATE_HEADER                   *SmmCommunicateHeader;
    SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA   *SmmIpmiBmcData;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered...  IpmiInstance: %x \n", __FUNCTION__, gIpmiInstance));

    //
    // Copy SMM Communicate Header Here
    //
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gCommunicateBuffer;
    CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gEfiSmmIpmiTransportProtocolGuid);
    SmmCommunicateHeader->MessageLength = SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA_SIZE;

    //
    // Copy SMM IPMI BMC Communicate Data Here
    //
    SmmIpmiBmcData = (SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA *)SmmCommunicateHeader->Data;
    SmmIpmiBmcData->FunctionNumber = SMM_IPMI_BMC_FUNCTION_SEND_COMMAND;
    SmmIpmiBmcData->NetFunction = NetFunction;
    SmmIpmiBmcData->Lun = Lun;
    SmmIpmiBmcData->Command = Command;
    if ((VOID *) CommandData == NULL || CommandDataSize == 0) {
        SmmIpmiBmcData->CommandData = 0;
    } else {
        SmmIpmiBmcData->CommandData = ((UINTN)(UINTN *) gCommandData);
        CopyMem ((VOID *)(UINTN *)SmmIpmiBmcData->CommandData, CommandData, (UINTN)CommandDataSize);
    }
    SmmIpmiBmcData->CommandDataSize = CommandDataSize;
    if ((VOID *) ResponseDataSize == NULL) {
        SmmIpmiBmcData->ResponseDataSize = 0;
    } else {
        SmmIpmiBmcData->ResponseDataSize = ((UINTN)(UINTN *) gResponseDataSize);
        CopyMem ((VOID *)(UINTN *)SmmIpmiBmcData->ResponseDataSize, ResponseDataSize,sizeof (UINT8));
    }
    if ((VOID *) ResponseData == NULL || *ResponseDataSize == 0) {
        SmmIpmiBmcData->ResponseData = 0;
    } else {
        SmmIpmiBmcData->ResponseData = ((UINTN)(UINTN *) gResponseData);
        CopyMem ((VOID *)(UINTN *)SmmIpmiBmcData->ResponseData, ResponseData,*ResponseDataSize);
    }

    //
    // Specify some initial value to data's return status
    //
    SmmIpmiBmcData->ReturnStatus = EFI_PROTOCOL_ERROR;

    //
    // Send data to SMM.
    //
    gCommunicateBufferSize = SMM_COMMUNICATE_HEADER_SIZE + SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA_SIZE;
    Status = gSmmCommunication->Communicate (gSmmCommunication, gCommunicateBuffer, &gCommunicateBufferSize);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gSmmCommunication->Communicate  Status: %r \n", Status));
    ASSERT_EFI_ERROR (Status);

    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) gCommunicateBuffer;
    SmmIpmiBmcData = (SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA *)SmmCommunicateHeader->Data;
    CopyMem (ResponseDataSize, (VOID *)(UINTN *) SmmIpmiBmcData->ResponseDataSize, sizeof(UINT8));
    CopyMem (ResponseData, (VOID *)(UINTN *) SmmIpmiBmcData->ResponseData, *ResponseDataSize);

    if (SmmIpmiBmcData->ReturnStatus == EFI_PROTOCOL_ERROR) {
        SmmIpmiBmcData->ReturnStatus = EFI_ABORTED;
    }

    //
    //  If Completion Code is assigned with a proper address
    //
    if ((CompletionCode != NULL)) {
        *CompletionCode = SmmIpmiBmcData->CommandCompletionCode;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmmIpmiBmcData->ReturnStatus : %r \n", SmmIpmiBmcData->ReturnStatus));

    This->CommandCompletionCode = SmmIpmiBmcData->CommandCompletionCode;
    //
    // Status returned by the SMM based routine.
    //
    return SmmIpmiBmcData->ReturnStatus;
}

/**
    Calls the IPMI send command SMM service for Non SMM case

    @param This             Pointer to IPMI Protocol/PPI
    @param NetFunction      Net function of the command
    @param LUN              Logical Unit number of the command
    @param Command          Command to be sent to BMC
    @param CommandData      Command data to be sent along with Command
    @param CommandDataSize  Command Data size
    @param ResponseData     Response Data from BMC
    @param ResponseDataSize Size of the Response data

    @return EFI_STATUS      Status returned from BMC while executing the command.

**/
EFI_STATUS
SmmEfiIpmiSendCommand (
  IN EFI_IPMI_TRANSPORT    *This,
  IN UINT8                 NetFunction,
  IN UINT8                 Lun,
  IN UINT8                 Command,
  IN UINT8                 *CommandData,
  IN UINT8                 CommandDataSize,
  OUT UINT8                *ResponseData,
  OUT UINT8                *ResponseDataSize
  )
{
    return SmmIpmiSendCommand (
            This,
            NetFunction,
            Lun,
            Command,
            CommandData,
            CommandDataSize,
            ResponseData,
            ResponseDataSize,
            NULL
            );
}

/**
    Calls the IPMI send command Extension SMM service for Non SMM case

    @param This             Pointer to IPMI Protocol/PPI
    @param NetFunction      Net function of the command
    @param LUN              Logical Unit number of the command
    @param Command          Command to be sent to BMC
    @param CommandData      Command data to be sent along with Command
    @param CommandDataSize  Command Data size
    @param ResponseData     Response Data from BMC
    @param ResponseDataSize Size of the Response data
    @param CompletionCode   Completion code returned by Bmc

    @return EFI_STATUS      Status returned from BMC while executing the command.

**/
EFI_STATUS
SmmEfiIpmiSendCommandEx (
  IN EFI_IPMI_TRANSPORT    *This,
  IN UINT8                 NetFunction,
  IN UINT8                 Lun,
  IN UINT8                 Command,
  IN UINT8                 *CommandData,
  IN UINT8                 CommandDataSize,
  OUT UINT8                *ResponseData,
  OUT UINT8                *ResponseDataSize,
  OUT UINT8                *CompletionCode
  )
{
    return SmmIpmiSendCommand (
            This,
            NetFunction,
            Lun,
            Command,
            CommandData,
            CommandDataSize,
            ResponseData,
            ResponseDataSize,
            CompletionCode
            );
}

/**
    Calls the IPMI BMC status SMM service for Non SMM case

    @param This Pointer to IPMI Protocol/PPI
    @param BmcStatus State of the BMC
    @param ComAddress BMC Communication address

    @retval EFI_SUCCESS Status returned

**/

EFI_STATUS
SmmEfiIpmiBmcStatus (
  IN EFI_IPMI_TRANSPORT     *This,
  OUT EFI_BMC_STATUS        *BmcStatus,
  OUT EFI_SM_COM_ADDRESS    *ComAddress )
{

    EFI_STATUS                                   Status;
    EFI_SMM_COMMUNICATE_HEADER                   *SmmCommunicateHeader;
    SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA     *SmmIpmiGetBmcStatusData;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered...  IpmiInstance: %x \n", __FUNCTION__, gIpmiInstance));

    //
    // Copy SMM Communicate Header Here
    //
    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *)gCommunicateBuffer;
    CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gEfiSmmIpmiTransportProtocolGuid);
    SmmCommunicateHeader->MessageLength = SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA_SIZE;

    //
    // Copy SMM IPMI Get BMC Communicate Data Here
    //
    SmmIpmiGetBmcStatusData = (SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA *)SmmCommunicateHeader->Data;
    SmmIpmiGetBmcStatusData->FunctionNumber = SMM_IPMI_BMC_FUNCTION_GET_BMC_STATUS;
    SmmIpmiGetBmcStatusData->BmcStatus = (UINTN) gBmcStatus;
    SmmIpmiGetBmcStatusData->ComAddress = (UINTN) gComAddress;

    //
    // Specify some initial value to data's return status
    //
    SmmIpmiGetBmcStatusData->ReturnStatus = EFI_PROTOCOL_ERROR;

    //
    // Send data to SMM.
    //
    gCommunicateBufferSize = SMM_COMMUNICATE_HEADER_SIZE + SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA_SIZE;
    Status = gSmmCommunication->Communicate (gSmmCommunication, gCommunicateBuffer, &gCommunicateBufferSize);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gSmmCommunication->Communicate  Status: %r \n", Status));
    ASSERT_EFI_ERROR (Status);

    SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER *) gCommunicateBuffer;
    SmmIpmiGetBmcStatusData = (SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA *)SmmCommunicateHeader->Data;

    CopyMem(BmcStatus, (VOID *) SmmIpmiGetBmcStatusData->BmcStatus, sizeof(EFI_BMC_STATUS) );
    CopyMem(ComAddress, (VOID *) SmmIpmiGetBmcStatusData->ComAddress, sizeof(EFI_SM_COM_ADDRESS) );
    if (SmmIpmiGetBmcStatusData->ReturnStatus == EFI_PROTOCOL_ERROR) {
        SmmIpmiGetBmcStatusData->ReturnStatus = EFI_ABORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "SmmIpmiGetBmcStatusData->ReturnStatus : %r \n", SmmIpmiGetBmcStatusData->ReturnStatus));

    //
    // Status returned by the SMM based routine.
    //
    return  SmmIpmiGetBmcStatusData->ReturnStatus;
}

/**
    Notification function for SmmIpmi Transport protocol.
    Performs necessary steps to proxy the DXE calls to SMM

    @param Event Event which caused this handler
    @param Context Context passed during Event Handler registration

    @retval VOID

**/

VOID
EFIAPI
SmmIpmiBmcTransportProtocolReady (
  IN  EFI_EVENT         Event,
  IN  VOID              *Context )
{
    EFI_STATUS                    Status;
    EFI_IPMI_TRANSPORT            *DummySmmmIpmiTransport = NULL;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered... IpmiInstance: %x \n", __FUNCTION__, gIpmiInstance));
    Status = gBS->LocateProtocol (&gEfiSmmIpmiTransportProtocolGuid, NULL, (VOID **)&DummySmmmIpmiTransport);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gBS->LocateProtocol gEfiSmmIpmiTransportProtocolGuid protocol  status %r\n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }

    Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID **) &gSmmCommunication);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gBS->LocateProtocol gEfiSmmCommunicationProtocolGuid protocol  status %r\n", Status));
    if (EFI_ERROR (Status)) {
        return;
    }

    //
    // Allocate memory for variable store.
    //
    gCommunicateBufferSize  = SMM_COMMUNICATE_HEADER_SIZE + SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA_SIZE;
    gCommunicateBuffer      = AllocateRuntimeZeroPool (gCommunicateBufferSize);
    ASSERT (gCommunicateBuffer != NULL);

    TotalBlockSize = AllocateRuntimeZeroPool (SMM_IPMI_RUNTIME_BUFFER_SIZE);
    ASSERT (TotalBlockSize != NULL);
    gCommandData = (UINT8 *) (TotalBlockSize);
    gResponseDataSize = gCommandData + MAXIMUM_DATA_SIZE;
    gResponseData = (UINT8 *) (gResponseDataSize + sizeof(UINT8));
    gBmcStatus = gResponseData + MAXIMUM_DATA_SIZE;
    gComAddress = gBmcStatus + sizeof(EFI_BMC_STATUS);
    //
    // Override SendIpmiCommand and GetBmcStatus entries in IpmiTransport protocol
    //
    gIpmiInstance->IpmiTransport.SendIpmiCommand = SmmEfiIpmiSendCommand;
    gIpmiInstance->IpmiTransport.SendIpmiCommandEx = SmmEfiIpmiSendCommandEx;
    gIpmiInstance->IpmiTransport.GetBmcStatus = SmmEfiIpmiBmcStatus;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting...  IpmiInstance: %x \n", __FUNCTION__, gIpmiInstance));

    return;
}

/**
    Hook to register Notification function for SmmIpmi Transport protocol.

    @param VOID

    @return EFI_STATUS
    @retval EFI_SUCCESS

**/

EFI_STATUS
InitializeIpmiPhysicalLayerHook (
  VOID
  )
{
    VOID         *SmmIpmiBmcRegistration = NULL;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered...  gIpmiInstance: %x \n", __FUNCTION__, gIpmiInstance));

    //
    // Create Notification event for SmmDxeIpmiBmc GUID
    //
    EfiNamedEventListen (
        &gEfiSmmIpmiTransportProtocolGuid,
        TPL_CALLBACK,
        SmmIpmiBmcTransportProtocolReady,
        NULL,
        SmmIpmiBmcRegistration
        );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
    return EFI_SUCCESS;
}

#endif // defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
