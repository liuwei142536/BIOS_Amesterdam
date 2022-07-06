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

/** @file SmmIpmiBmcInitialize.c
    SMM IPMI Transport Driver.

**/

//---------------------------------------------------------------------------

#include "DxeSmmIpmiBmc.h"
#include <Library/AmiBufferValidationLib.h>
#include <Library/SmmServicesTableLib.h>
#if (!IPMI_DEFAULT_HOOK_SUPPORT)
#include "IpmiInitHooks.h"
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

EFI_IPMI_BMC_INSTANCE_DATA  *gSmmIpmiInstance = NULL;
EFI_I2C_MASTER_PROTOCOL     *gMasterTransmit;
EFI_SMBUS_HC_PROTOCOL       *gEfiSmbusHcProtocol;
VOID                         *gRegistration;

#if (!IPMI_DEFAULT_HOOK_SUPPORT)
extern IPMI_OEM_SMM_INIT_HOOK IPMI_OEM_SMM_INIT_HOOK_LIST EndOfIpmiOemSmmInitHookListFunctions;
IPMI_OEM_SMM_INIT_HOOK* gIpmiOemSmmInitHookList[] = {IPMI_OEM_SMM_INIT_HOOK_LIST NULL};
#endif

//---------------------------------------------------------------------------

#if (!IPMI_DEFAULT_HOOK_SUPPORT)
/**
    Initiates the DXE SMM hooks of OEM.

    @param BootServices     - Pointer the set of Boot services.
    @param MmioBaseAddress  - Pointer to the MMIO base address.
    @param BaseAddressRange - Pointer to the base address range.

    @return EFI_STATUS      - Return status of the Hook.
*/

EFI_STATUS
IpmiOemSmmInitHook (
  IN CONST EFI_BOOT_SERVICES            *BootServices,
  IN       UINTN                        *MmioBaseAddress,
  IN       UINTN                        *BaseAddressRange
)
{
    EFI_STATUS  Status = EFI_DEVICE_ERROR;
    UINTN       Index;
    for (Index = 0; gIpmiOemSmmInitHookList[Index]; Index++) {
        Status = gIpmiOemSmmInitHookList[Index] (BootServices, MmioBaseAddress, BaseAddressRange);
    }
    return Status;
}
#endif

#if defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)

/**
    Software SMI callback which proxies the DXE calls to SMM IPMI protocol.

    @param DispatchHandle   The unique handle assigned to this handler by
                            SmiHandlerRegister().
    @param RegisterContext  Points to an optional handler context which was
                            specified when the handler was registered.
    @param CommBuffer       A pointer to a collection of data in memory that will
                            be conveyed from a non-SMM environment into an SMM environment.
    @param CommBufferSize   The size of the CommBuffer.

    @retval EFI_SUCCESS     Always returns EFI_SUCCESS.

**/

EFI_STATUS
EFIAPI
SmmIpmiBmcHandler (
  IN     EFI_HANDLE         DispatchHandle,
  IN     CONST VOID         *RegisterContext,
  IN OUT VOID               *CommBuffer,
  IN OUT UINTN              *CommBufferSize )
{
    EFI_STATUS                                      Status = EFI_SUCCESS;
    SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA      *SmmIpmiBmcData;
    SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA        *SmmIpmiGetBmcStatusData;

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Entered Status %r gSmmIpmiInstance: %x \n", __FUNCTION__, Status, gSmmIpmiInstance));

    ASSERT (CommBuffer != NULL);

    SmmIpmiBmcData = (SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA *)CommBuffer;
    ///
    /// Validate if CommBuffer is of Non-SMRAM region before using it,
    /// in order to avoid SMRAM data corruption in SMI handlers.
    ///
    Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiBmcData, sizeof (SMM_IPMI_BMC_SEND_COMMAND_COMMUNICATE_DATA));
    if (EFI_ERROR (Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiBmcData Status: %r\n", __FUNCTION__, Status));
        return EFI_SUCCESS;
    }

    switch (SmmIpmiBmcData->FunctionNumber) {

        case SMM_IPMI_BMC_FUNCTION_SEND_COMMAND:
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "SendIpmiCommand function is called... \n"));
            if ((VOID*)SmmIpmiBmcData->CommandData != NULL) {
                ///
                /// Validate if CommandData is of Non-SMRAM region before using it,
                /// in order to avoid SMRAM data corruption in SMI handlers.
                ///
                Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiBmcData->CommandData, SmmIpmiBmcData->CommandDataSize);
                if (EFI_ERROR (Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiBmcData->CommandData Status: %r\n", __FUNCTION__, Status));
                    return EFI_SUCCESS;
                }
            }

            if ((VOID*)SmmIpmiBmcData->ResponseDataSize != NULL) {
                ///
                /// Validate if ResponseDataSize is of Non-SMRAM region before using it,
                /// in order to avoid SMRAM data corruption in SMI handlers.
                ///
                Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiBmcData->ResponseDataSize, sizeof (SmmIpmiBmcData->ResponseDataSize));
                if (EFI_ERROR (Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiBmcData->ResponseDataSize Status: %r\n", __FUNCTION__, Status));
                    return EFI_SUCCESS;
                }

                if ((VOID*)SmmIpmiBmcData->ResponseData != NULL) {
                    ///
                    /// Validate if ResponseData is of Non-SMRAM region before using it,
                    /// in order to avoid SMRAM data corruption in SMI handlers.
                    ///
                    Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiBmcData->ResponseData, (UINTN)*((UINT8 *)SmmIpmiBmcData->ResponseDataSize));
                    if (EFI_ERROR (Status)) {
                        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiBmcData->ResponseData Status: %r\n", __FUNCTION__, Status));
                        return EFI_SUCCESS;
                    }
                }
            }

            Status = gSmmIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                        &gSmmIpmiInstance->IpmiTransport,
                                                        (UINT8)SmmIpmiBmcData->NetFunction,
                                                        (UINT8)SmmIpmiBmcData->Lun,
                                                        (UINT8)SmmIpmiBmcData->Command,
                                                        (UINT8*)SmmIpmiBmcData->CommandData,
                                                        (UINT8)SmmIpmiBmcData->CommandDataSize,
                                                        (UINT8*)SmmIpmiBmcData->ResponseData,
                                                        (UINT8*)SmmIpmiBmcData->ResponseDataSize );

            SmmIpmiBmcData->CommandCompletionCode = gSmmIpmiInstance->IpmiTransport.CommandCompletionCode;
            SmmIpmiBmcData->ReturnStatus = Status;
            break;

        case SMM_IPMI_BMC_FUNCTION_GET_BMC_STATUS:
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "GetBmcStatus function is called... \n"));
            SmmIpmiGetBmcStatusData = (SMM_IPMI_BMC_GET_STATUS_COMMUNICATE_DATA *)CommBuffer;

            ///
            /// Validate if BmcStatus is of Non-SMRAM region before using it,
            /// in order to avoid SMRAM data corruption in SMI handlers.
            ///
            if ((VOID*)SmmIpmiGetBmcStatusData->BmcStatus != NULL) {
                Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiGetBmcStatusData->BmcStatus, sizeof (SmmIpmiGetBmcStatusData->BmcStatus));
                if (EFI_ERROR (Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiGetBmcStatusData->BmcStatus Status: %r\n", __FUNCTION__, Status));
                    return EFI_SUCCESS;
                }
            }

            ///
            /// Validate if ComAddress is of Non-SMRAM region before using it,
            /// in order to avoid SMRAM data corruption in SMI handlers.
            ///
            if ((VOID*)SmmIpmiGetBmcStatusData->ComAddress != NULL) {
                Status = AmiValidateMemoryBuffer ((VOID*)SmmIpmiGetBmcStatusData->ComAddress, sizeof (EFI_SM_COM_ADDRESS));
                if (EFI_ERROR (Status)) {
                    SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a AmiValidateMemoryBuffer for SmmIpmiGetBmcStatusData->ComAddress Status: %r\n", __FUNCTION__, Status));
                    return EFI_SUCCESS;
                }
            }

            Status = gSmmIpmiInstance->IpmiTransport.GetBmcStatus (
                                                        &gSmmIpmiInstance->IpmiTransport,
                                                        (EFI_BMC_STATUS*)SmmIpmiGetBmcStatusData->BmcStatus,
                                                        (EFI_SM_COM_ADDRESS*)SmmIpmiGetBmcStatusData->ComAddress );

            SmmIpmiGetBmcStatusData->ReturnStatus = Status;
            break;

        default:
            ASSERT (FALSE);
            Status = EFI_UNSUPPORTED;
    }

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Exiting... Status %r\n", __FUNCTION__, Status));

    return EFI_SUCCESS;
}

#endif // defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)

/**
    Execute the Get System Interface Capability command and update the RwSupport
    and PecSupport of Ipmi Instance.

    @param  VOID

    @return  VOID

**/
VOID
GetSystemInterfaceCapability (
  IN VOID
)
{
    EFI_STATUS                                     Status;
    EFI_GET_SYSTEM_INTERFACE_CAPABILITY_COMMAND    GetSystemInterfaceCapabilityCmd;
    EFI_IPMI_SSIF_INTERFACE_CAPABILITY             GetSsifInterfaceCapability;
    UINT8                                          DataSize = sizeof (GetSsifInterfaceCapability);

    GetSystemInterfaceCapabilityCmd.SystemInterfaceType = 0x0; // SSIF
    GetSystemInterfaceCapabilityCmd.Reserved = 0x0;

    Status = gSmmIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                &gSmmIpmiInstance->IpmiTransport,
                                                IPMI_NETFN_APP,
                                                BMC_LUN,
                                                IPMI_APP_GET_SYSTEM_INTERFACE_CAPABILITIES,
                                                (UINT8*) &GetSystemInterfaceCapabilityCmd,
                                                sizeof (GetSystemInterfaceCapabilityCmd),
                                                (UINT8*) &GetSsifInterfaceCapability,
                                                &DataSize);

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Status %r\n", __FUNCTION__, Status));

    if (!EFI_ERROR (Status)) {
        gSmmIpmiInstance->Interface.SSIF.RwSupport = GetSsifInterfaceCapability.TransactionSupport;
        gSmmIpmiInstance->Interface.SSIF.PecSupport = GetSsifInterfaceCapability.PecSupport;
    }
}

/**
    Execute the Get Global Enable command to get receive message queue interrupt.

    @return VOID
**/
VOID
GetGlobalEnables ()
{
    EFI_STATUS                      Status;
    EFI_BMC_GLOBAL_ENABLES          BmcGlobalEnables;
    UINT32                          ResponseDataSize = sizeof (BmcGlobalEnables);

    //
    // Get Global Enables Information.
    //
    Status = gSmmIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                &gSmmIpmiInstance->IpmiTransport,
                                                IPMI_NETFN_APP,
                                                BMC_LUN,
                                                IPMI_APP_GET_BMC_GLOBAL_ENABLES,
                                                NULL,
                                                0,
                                                (UINT8 *) (&BmcGlobalEnables),
                                                (UINT8 *) &ResponseDataSize
                                                );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Get BMC Global Enables status %r\n", Status));

    if (!EFI_ERROR(Status)) {
        //
        // Set Smb alert pin based on ReceiveMsgQueueInterrupt bit
        //
        gSmmIpmiInstance->Interface.SSIF.SmbAlertSupport = BmcGlobalEnables.ReceiveMsgQueueInterrupt;
    }
}

/**
    Gets the self test results and update the BmcStatus accordingly in
    IpmiInstance structure. Installs the SmmIpmiTransportProtocol if
    status is not Hard fail and proxy the DXE calls to SMM based on a token.

    @param VOID

    @return EFI_STATUS            Status of the function.
    @retval EFI_SUCCESS           Self test command is success and DxeIpmiTransportProtocol
                                  is installed successfully.
    @retval EFI_OUT_OF_RESOURCES  Failed to allocate requested memory.
    @retval EFI_UNSUPPORTED       Bmc is in hard fail state.
    @retval Others                Return status from other functions used.

**/
EFI_STATUS
CheckSelfTestAndInstallSmmIpmiProtocol (VOID)
{
    UINT8                           Index;
    UINT8                           ResponseDataSize;
    EFI_STATUS                      BmcStatus;
    IPMI_SELF_TEST_RESULT_RESPONSE  BstStatus;
    EFI_STATUS                      Status = EFI_UNSUPPORTED;
    EFI_HANDLE                      NewHandle = NULL;


    //
    // Get the BMC SELF TEST Results.
    //
    for (Index = 0; Index < IPMI_SELF_TEST_COMMAND_RETRY_COUNT; Index++) {
        ResponseDataSize = sizeof (BstStatus);
        BmcStatus = gSmmIpmiInstance->IpmiTransport.SendIpmiCommand (
                                                        &gSmmIpmiInstance->IpmiTransport,
                                                        IPMI_NETFN_APP,
                                                        BMC_LUN,
                                                        IPMI_APP_GET_SELFTEST_RESULTS,
                                                        NULL,
                                                        0,
                                                        (UINT8*)&BstStatus,
                                                        &ResponseDataSize );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a SMM: Get Self test results status %r\n",__FUNCTION__, BmcStatus));
        if (!EFI_ERROR(BmcStatus)) {
            break;
        }
    }
    //
    // If Status indicates a Device error, then the BMC is not responding, so send an error.
    //
    if (EFI_ERROR (BmcStatus)) {
        gSmmIpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
        return Status;
    } else {
        //
        // Check the self test results.  Cases 55h - 58h are IPMI defined test results.
        // Additional Cases are device specific test results.
        //
        switch (BstStatus.Result) {
            case IPMI_APP_SELFTEST_NO_ERROR: // 0x55
            case IPMI_APP_SELFTEST_NOT_IMPLEMENTED: // 0x56
            case IPMI_APP_SELFTEST_RESERVED: // 0xFF
                gSmmIpmiInstance->BmcStatus = EFI_BMC_OK;
                break;

            case IPMI_APP_SELFTEST_ERROR: // 0x57
                gSmmIpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
                break;

            default: // 0x58 and Other Device Specific Hardware Error
                gSmmIpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
                break;
        }
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a SMM BMC status %x\n", __FUNCTION__, gSmmIpmiInstance->BmcStatus));
    
    //
    // Now install the Protocol if the BMC is not in a Hard Fail State
    //
    if (gSmmIpmiInstance->BmcStatus != EFI_BMC_HARDFAIL) {
        Status = gSmst->SmmInstallProtocolInterface (
                            &NewHandle,
                            &gEfiSmmIpmiTransportProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &gSmmIpmiInstance->IpmiTransport);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a SMM Install gEfiSmmIpmiTransportProtocolGuid protocol status %r\n",__FUNCTION__, Status));
        if (EFI_ERROR (Status)) {
            //
            // Error in installing the protocol. So free the allocated memory.
            //
            FreePool (gSmmIpmiInstance);
            return Status;
        }

#if defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)
        //
        // Same Interface used for both SMM and DXE phases so proxy the DXE calls to SMM
        // Register SMM IPMI BMC SMI handler
        //
        NewHandle = NULL;
        Status = gSmst->SmiHandlerRegister (
                            SmmIpmiBmcHandler,
                            &gEfiSmmIpmiTransportProtocolGuid,
                            &NewHandle);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "gSmst->SmiHandlerRegister  status %r\n", Status));
        ASSERT_EFI_ERROR (Status);

        if (!EFI_ERROR(Status)) {
            //
            // Notify the DxeIpmiBmc driver that SmmIpmiBmcTransport protocol is ready
            //
            NewHandle = NULL;
            Status = gBS->InstallProtocolInterface (
                            &NewHandle,
                            &gEfiSmmIpmiTransportProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            NULL );
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "gBS->Install: gEfiSmmIpmiTransportProtocolGuid protocol  status %r\n", Status));
            ASSERT_EFI_ERROR (Status);
        }
#endif // defined(IPMI_SEPARATE_DXE_SMM_INTERFACES) && (IPMI_SEPARATE_DXE_SMM_INTERFACES == 0)
    }
    return Status;
}

/**
    Locate the HandleBuffer of SmbusHcProtocol and for each handle, try to send
    Get Self Test Results command. If the command is success, then SSIF BMC instance
    is found and install the SmmIpmiTransportProtocol and return the Status accordingly.

    @param VOID

    @return EFI_STATUS      Status of the function.
    @retval EFI_NOT_FOUND   The SmbusHcProtocol instance for BMC is not found.
    @retval EFI_SUCCESS     SmbusHcProtocol instance of BMC is found and Protocol is
                            installed successfully.
    @retval Others          Return status of other functions used.

**/
EFI_STATUS
LocateAndFindSmbusInstance (VOID)
{
    EFI_STATUS      Status;
    UINTN           HandleCount = sizeof (EFI_HANDLE);
    EFI_HANDLE      *HandleBuffer = NULL;
    UINTN           Index;

    Status = gSmst->SmmLocateHandle (
                        ByProtocol,
                        &gEfiSmbusHcProtocolGuid,
                        NULL,
                        &HandleCount,
                        HandleBuffer
                    );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a HandleCount: %d LocateHandleBuffer Status %r\n", __FUNCTION__, HandleCount, Status));
    if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
        return EFI_NOT_FOUND;
    }

    //
    // Allocate memory for Handle buffer
    //
    HandleBuffer = AllocateZeroPool (HandleCount);
    if (HandleBuffer == NULL) {
        return EFI_NOT_FOUND;
    }

    Status = gSmst->SmmLocateHandle (
                        ByProtocol,
                        &gEfiSmbusHcProtocolGuid,
                        NULL,
                        &HandleCount,
                        HandleBuffer
                    );
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a HandleCount: %d LocateHandleBuffer Status %r\n", __FUNCTION__, HandleCount, Status));
    if (EFI_ERROR (Status)) {
        return EFI_NOT_FOUND;
    }
    
    for (Index = 0; Index < HandleCount; Index++) {
        Status = gSmst->SmmHandleProtocol (
                            HandleBuffer[Index],
                            &gEfiSmbusHcProtocolGuid,
                            (VOID **)&gEfiSmbusHcProtocol
                        );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Index: %d SmmLocateProtocol Status %r\n", __FUNCTION__, Index, Status));
        if (!EFI_ERROR (Status)) {
            //
            // Check SSIF interface capability and Global Enables information
            //
            GetSystemInterfaceCapability ();
            GetGlobalEnables ();
            //
            // Get self test results and installs SmmIpmiProtocol
            //
            Status = CheckSelfTestAndInstallSmmIpmiProtocol ();
            if (!EFI_ERROR (Status)) {
                return Status;
            }
        }
    }
    return EFI_NOT_FOUND;
}

/**
  Notify function for SmbusHcProtocol protocol.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Returns EFI_SUCCESS.

**/

EFI_STATUS
SmmNotifySmbusInstance (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
    EFI_STATUS      Status = EFI_SUCCESS;
    if ((gSmmIpmiInstance->BmcStatus == EFI_BMC_OK) || (gSmmIpmiInstance->BmcStatus == EFI_BMC_SOFTFAIL)) {
        return Status;
    }
    gEfiSmbusHcProtocol = (EFI_SMBUS_HC_PROTOCOL *)Interface;

    //
    // Check the SSIF interface capability and Global Enables information
    //
    GetSystemInterfaceCapability();
    GetGlobalEnables();
    //
    // Get self test results and installs SmmIpmiProtocol
    //
    Status = CheckSelfTestAndInstallSmmIpmiProtocol ();
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a CheckSelfTestAndInstallSmmIpmiProtocol Status %r\n", __FUNCTION__, Status));
    return EFI_SUCCESS;
}

/**
    Setup and initialize the BMC for the SMM phase.
    1. In order to verify the BMC is functioning as expected, the BMC Self-test
    is performed. Updates the BMC status in Private data
    2. Installs SMM IPMI transport protocol.
    3. Registers Software SMI callback to proxy the DXE calls to SMM IPMI protocol.
    This applicable only when both DXE and SMM uses same KCS interface.
    4. Notify the DxeIpmiBmc driver that SmmIpmiBmcTransport protocol is ready.

    @param ImageHandle Handle of this driver image
    @param SystemTable Table containing standard EFI services

    @return Status                Status of Driver execution.
    @retval EFI_SUCCESS           Successful driver initialization.
    @retval EFI_UNSUPPORTED       ServerSetup variable is found and BmcSupport is
                                  disabled or Self test has failed or error in
                                  installing SmmIpmiTransportProtocol.
    @retval EFI_OUT_OF_RESOURCES  Failed to allocate requested memory.
    @retval Others                Return status from other functions used.

**/

EFI_STATUS
InitializeIpmiSmmPhysicalLayer (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
    EFI_STATUS                            Status = EFI_SUCCESS;

    //
    // Allocate memory for IPMI Instance
    //
    gSmmIpmiInstance = AllocateRuntimeZeroPool (sizeof (EFI_IPMI_BMC_INSTANCE_DATA));
    if (gSmmIpmiInstance == NULL) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gSmmIpmiInstance! \n"));
        return EFI_OUT_OF_RESOURCES;
    }
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "gSmmIpmiInstance: %x \n", gSmmIpmiInstance));

    //
    // Initialize IPMI Instance Data
    //
    if (IPMI_DEFAULT_HOOK_SUPPORT) {
#if IPMI_SYSTEM_INTERFACE == KCS_INTERFACE
        gSmmIpmiInstance->Interface.KCS.DataPort = IPMI_SMM_KCS_DATA_PORT;          // KCS Data Port
        gSmmIpmiInstance->Interface.KCS.CommandPort = IPMI_SMM_KCS_COMMAND_PORT;    // KCS Command Port
#elif IPMI_SYSTEM_INTERFACE == BT_INTERFACE
        gSmmIpmiInstance->Interface.BT.CtrlPort = IPMI_BT_CTRL_PORT;     // BT Control Port
        gSmmIpmiInstance->Interface.BT.ComBuffer = IPMI_BT_BUFFER_PORT;  // BT Buffer Port
        gSmmIpmiInstance->Interface.BT.IntMaskPort = IPMI_BT_INTERRUPT_MASK_PORT; // BT IntMask Port
#endif
        gSmmIpmiInstance->MmioBaseAddress = 0;
        gSmmIpmiInstance->AccessType = IPMI_IO_ACCESS;
        gSmmIpmiInstance->BaseAddressRange = 0;
    } else {
#if (!IPMI_DEFAULT_HOOK_SUPPORT)
        gSmmIpmiInstance->AccessType = IPMI_MMIO_ACCESS;
        Status = IpmiOemSmmInitHook (gBS, &gSmmIpmiInstance->MmioBaseAddress, &gSmmIpmiInstance->BaseAddressRange);
        if (EFI_ERROR (Status)) {
            //
            // IpmiOemSmmInitHook has failed, so free the allocated memory and return Status.
            //
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IpmiOemSmmInitHook is failed and Status is %r.\n", Status));
            FreePool (gSmmIpmiInstance);
            return Status;
        }
#endif
    }

    gSmmIpmiInstance->Signature     = SM_IPMI_BMC_SIGNATURE;
    gSmmIpmiInstance->SlaveAddress  = IPMI_BMC_SLAVE_ADDRESS;
    gSmmIpmiInstance->BmcStatus     = EFI_BMC_OK;
    gSmmIpmiInstance->IpmiTransport.GetBmcStatus = EfiIpmiBmcStatus;

    if (IPMI_SYSTEM_INTERFACE == KCS_INTERFACE) {
        gSmmIpmiInstance->Interface.KCS.KcsRetryCounter = IPMI_SMM_KCS_COMMAND_PORT_READ_RETRY_COUNTER;
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommand = EfiIpmiSendCommand;       // KCS SendCommand
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiIpmiSendCommandEx; // KCS SendCommand Extension
    } else if (IPMI_SYSTEM_INTERFACE == BT_INTERFACE) {
        gSmmIpmiInstance->Interface.BT.BtDelay = BT_DELAY;       // BT Delay
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommand = EfiBtIpmiSendCommand;   // BT SendCommand
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiBtIpmiSendCommandEx; // BT SendCommand Extension
    } else if (IPMI_SYSTEM_INTERFACE == SSIF_INTERFACE) {
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommand = EfiSsifIpmiSendCommand;   // SSIF SendCommand
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiSsifIpmiSendCommandEx; // SSIF SendCommand Extension
        gSmmIpmiInstance->Interface.SSIF.SsifRetryCounter = IPMI_SSIF_COMMAND_REQUEST_RETRY_COUNTER; // SSIF retry counter
        gSmmIpmiInstance->Interface.SSIF.PecSupport = FALSE; // SSIF PEC support
        gSmmIpmiInstance->Interface.SSIF.RwSupport = 0x0; // SSIF multi-part reads/writes support
        gSmmIpmiInstance->Interface.SSIF.SmbAlertSupport = FALSE; // Smb alert pin support
    
        Status = LocateAndFindSmbusInstance ();
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a After LocateAndFindSmbusInstance Status: %r gEfiSmbusHcProtocol: %X\n", __FUNCTION__, Status, gEfiSmbusHcProtocol));
        if (EFI_ERROR (Status)) {
            Status = gSmst->SmmRegisterProtocolNotify (
                                &gEfiSmbusHcProtocolGuid,
                                SmmNotifySmbusInstance,
                                gRegistration
                                );
            SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a SmmRegisterProtocolNotify Status: %r\n", __FUNCTION__, Status));
            if (EFI_ERROR (Status)) {
                FreePool (gSmmIpmiInstance);
                return EFI_UNSUPPORTED;
            }
        }
        return EFI_SUCCESS;
    } else {
        Status = gSmst->SmmLocateProtocol(
                            &gEfiI2cMasterProtocolGuid,
                            NULL,
                            (VOID **)&gMasterTransmit);
        if(EFI_ERROR(Status)) {
            FreePool (gSmmIpmiInstance);
            return Status;
        }
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommand = EfiIpmbIpmiSendCommand;   // IPMB SendCommand
        gSmmIpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiIpmbIpmiSendCommandEx; // IPMB SendCommand Extension
    }

    Status = CheckSelfTestAndInstallSmmIpmiProtocol();
    if (EFI_ERROR(Status)) {
        //
        // Self test has failed or error while installing protocol, so free the memory and return EFI_UNSUPPORTED to unload driver from memory.
        //
        gSmst->SmmFreePool (gSmmIpmiInstance);
        Status = EFI_UNSUPPORTED;
    }

    return Status;
}

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
