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

/** @file PeiIpmiBmcInitialize.c
    PEI IPMI Transport Driver.

**/

//---------------------------------------------------------------------------

#include "Token.h"
#include "IpmiBmc.h"
#include "IpmiInitHooks.h"
#include <Include/ServerMgmtSetupVariable.h>
#include <MdeModulePkg/Core/Pei/PeiMain.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern INIT_LPC_BMC INIT_LPC_BMC_HOOK_LIST EndOfInitLpcBmcHookListFunctions;
INIT_LPC_BMC* gInitLpcBmcHookList[] = {INIT_LPC_BMC_HOOK_LIST NULL};

#if BMC_INIT_DELAY
extern BMC_INIT_DELAY_HOOK BMC_INIT_DELAY_HOOK_LIST EndOfBmcInitDelayHookListFunctions;
BMC_INIT_DELAY_HOOK* gBmcInitDelayHookList[] = {BMC_INIT_DELAY_HOOK_LIST NULL};
#endif

#if (!IPMI_DEFAULT_HOOK_SUPPORT)
extern IPMI_OEM_PEI_INIT_HOOK IPMI_OEM_PEI_INIT_HOOK_LIST EndOfIpmiOemPeiInitHookListFunctions;
IPMI_OEM_PEI_INIT_HOOK* gIpmiOemPeiInitHookList[] = {IPMI_OEM_PEI_INIT_HOOK_LIST NULL};
#endif

//---------------------------------------------------------------------------

/**
    This routine calls the InitLpcBmc function or platform hooked function.

    @param PeiServices Pointer to PEI Services.

    @return EFI_STATUS Return Status

**/
EFI_STATUS
InitLpcBmcHook (
  IN CONST EFI_PEI_SERVICES          **PeiServices )
{
    EFI_STATUS Status;
    UINTN i;

    for(i = 0; gInitLpcBmcHookList[i]; i++) {
        Status = gInitLpcBmcHookList[i](PeiServices);
    }

    return Status;
}

#if BMC_INIT_DELAY
/**
    This routine calls the BmcInitDelay function or platform hooked function.

    @param PeiServices Pointer to PEI Services.

    @return BOOLEAN

**/
BOOLEAN
BmcInitDelayHook (
  IN CONST EFI_PEI_SERVICES          **PeiServices )
{
    BOOLEAN IsPowerFail;
    UINTN   i;

    for(i = 0; gBmcInitDelayHookList[i]; i++) {
        IsPowerFail = gBmcInitDelayHookList[i](PeiServices);
    }

    return IsPowerFail;
}
#endif

#if (!IPMI_DEFAULT_HOOK_SUPPORT)
/**
    Initiates the PEI hooks of OEM.

    @param PeiServices      - Pointer the set of PEI services.
    @param MmioBaseAddress  - Pointer to the MMIO base address.
    @param BaseAddressRange - Pointer to the base address range.

    @return EFI_STATUS      - Return status of the Hook.
*/

EFI_STATUS
IpmiOemPeiInitHook (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN       UINTN                        *MmioBaseAddress,
  IN       UINTN                        *BaseAddressRange
)
{
    EFI_STATUS  Status = EFI_DEVICE_ERROR;
    UINTN       Index;
    for (Index = 0; gIpmiOemPeiInitHookList[Index]; Index++) {
        Status = gIpmiOemPeiInitHookList[Index] (PeiServices, MmioBaseAddress, BaseAddressRange);
    }
    return Status;
}
#endif

/**
    Execute the Get Device ID command to determine whether or not the BMC is in
    Force Update Mode. If it is, then report it to the error manager.

    @param IpmiInstance Data structure describing BMC
               variables and used for sending commands

    @return EFI_STATUS  Return Status of the Sent Command
    @retval EFI_SUCCESS Command executed Successfully

**/
EFI_STATUS
GetDeviceId (
  IN  OUT  EFI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance )
{
    EFI_STATUS                            Status;
    UINT8                                 DataSize;
    IPMI_GET_DEVICE_ID_RESPONSE           GetDeviceIdResponse;

    DataSize = sizeof (GetDeviceIdResponse);

    //
    // Get the device ID information for the BMC.
    //
    Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &IpmiInstance->IpmiTransport,
                                            IPMI_NETFN_APP,
                                            BMC_LUN,
                                            IPMI_APP_GET_DEVICE_ID,
                                            NULL,
                                            0,
                                            (UINT8*)&GetDeviceIdResponse,
                                            &DataSize );

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Get Device ID Command: Status %r\n", Status));
    //
    // If there is no error then proceed to check the data returned by the BMC
    //
    if (!EFI_ERROR(Status)) {
        //
        // If the controller is in Update Mode then set the BMC Status to indicate
        // the BMC is in force update mode.
        //
        if (GetDeviceIdResponse.UpdateMode != 0) {
            IpmiInstance->BmcStatus = EFI_BMC_UPDATE_IN_PROGRESS;
        }
    }
    return Status;
}

/**
    Execute the Get System Interface Capability command and update the RwSupport
    and PecSupport of Ipmi Instance.
    
    @ IpmiInstance Pointer to Data structure describing BMC
                   variables and used for sending commands

    @retval  VOID

**/
VOID
GetSystemInterfaceCapability (
  IN OUT  EFI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance
)
{
    EFI_STATUS                                     Status;
    EFI_GET_SYSTEM_INTERFACE_CAPABILITY_COMMAND    GetSystemInterfaceCapabilityCmd;
    EFI_IPMI_SSIF_INTERFACE_CAPABILITY             GetSsifInterfaceCapability;
    UINT8                                          DataSize = sizeof (GetSsifInterfaceCapability);

    GetSystemInterfaceCapabilityCmd.SystemInterfaceType = 0x0; // SSIF
    GetSystemInterfaceCapabilityCmd.Reserved = 0x0;

    Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &IpmiInstance->IpmiTransport,
                                            IPMI_NETFN_APP,
                                            BMC_LUN,
                                            IPMI_APP_GET_SYSTEM_INTERFACE_CAPABILITIES,
                                            (UINT8*) &GetSystemInterfaceCapabilityCmd,
                                            sizeof (GetSystemInterfaceCapabilityCmd),
                                            (UINT8*) &GetSsifInterfaceCapability,
                                            &DataSize);

    SERVER_IPMI_DEBUG ((DEBUG_INFO, "Get System Interface Capability status %r\n", Status));

    if (!EFI_ERROR (Status)) {
        IpmiInstance->Interface.SSIF.RwSupport = GetSsifInterfaceCapability.TransactionSupport;
        IpmiInstance->Interface.SSIF.PecSupport = GetSsifInterfaceCapability.PecSupport;
    }
}

/**
    Execute the Get Global Enable command to get receive message queue interrupt.

    @param IpmiInstance Data structure describing BMC
                        variables and used for sending commands

    @return VOID
**/
VOID
GetGlobalEnables (
  IN OUT  EFI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance
)
{
    EFI_STATUS                      Status;
    EFI_BMC_GLOBAL_ENABLES          BmcGlobalEnables;
    UINT32                          ResponseDataSize = sizeof (BmcGlobalEnables);

    //
    // Get Global Enable Information.
    //
    Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                            &IpmiInstance->IpmiTransport,
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
        // Set SmbAlertSupport based on ReceiveMsgQueueInterrupt bit
        //
        IpmiInstance->Interface.SSIF.SmbAlertSupport = BmcGlobalEnables.ReceiveMsgQueueInterrupt;
    }
}

/**
    Wait for BMC for 30 seconds if setup option "WaitForBmc" is enabled and PWR_FLR bit is set.

    @param PeiServices   Pointer to the PEI Core data Structure

    @return VOID

**/
#if BMC_INIT_DELAY
VOID
WaitForBmcDelay (
  IN  CONST EFI_PEI_SERVICES        **PeiServices,
  IN        UINTN                   WaitForBmc
)
{
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "ServerMgmtConfiguration.WaitForBmc: %x \n", WaitForBmc));

    //
    // Add 30 Sec delay before sending Self Test command during First AC Power Cycle On.
    // BMC starts simultaneously with BIOS so BMC takes 30 Sec to respond.
    // Give a delay of 30 Sec if the Power Failure (PWR_FLR) bit is set and WaitForBmc Setup
    // option is enabled.
    //
    if (WaitForBmc && BmcInitDelayHook (PeiServices)) {
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "Giving DELAY_OF_THIRTY_SECOND Sec delay.. start \n"));
        MicroSecondDelay (DELAY_OF_THIRTY_SECOND);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "Giving DELAY_OF_THIRTY_SECOND Sec delay.. end \n"));
    }
}
#endif

/**
    After memory is discovered, update the IPMI Instance and SmbusPpi pointers in HOB.
        
    @param PeiServices       Pointer to the PEI Core data Structure
    @param NotifyDescriptor  Pointer to the instance of the EFI_PEI_NOTIFY_DESCRIPTOR
    @param Ppi               Pointer to the instance of the interface

    @return EFI_STATUS       Return Status

**/

EFI_STATUS
EFIAPI
UpdateIpmiHobPtrs (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi )
{
    PEI_CORE_INSTANCE             *PrivateData;
    EFI_IPMI_BMC_INSTANCE_DATA    *IpmiInstance;
    EFI_HOB_GUID_TYPE             *GuidHob;
    AMI_PEI_IPMI_DATA_HOB         *PeiIpmiDataHob;
    EFI_PEI_SMBUS2_PPI            *EfiPeiSmbus2Ppi = NULL;
    
    GuidHob = GetFirstGuidHob (&gAmiPeiIpmiDataHobGuid);

    ASSERT (GuidHob != NULL);
    if (GuidHob == NULL) {
        return EFI_NOT_FOUND;
    }

    PeiIpmiDataHob = (AMI_PEI_IPMI_DATA_HOB *)GET_GUID_HOB_DATA(GuidHob);
    
    IpmiInstance = (EFI_IPMI_BMC_INSTANCE_DATA *) PeiIpmiDataHob->IpmiInstance;
    EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *) PeiIpmiDataHob->EfiPeiSmbus2Ppi;
    
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IpmiInstance Signature: %x\n", IpmiInstance->Signature));
    
    // Get PEI Core private data
    PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS (PeiServices);
    
    if (PrivateData->HeapOffsetPositive) {
        IpmiInstance   = (EFI_IPMI_BMC_INSTANCE_DATA *) ((UINT8 *)IpmiInstance + PrivateData->HeapOffset);
        EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)((UINT8 *)EfiPeiSmbus2Ppi + PrivateData->HeapOffset);
    } else {
        IpmiInstance   = (EFI_IPMI_BMC_INSTANCE_DATA *)((UINT8 *)IpmiInstance - PrivateData->HeapOffset);
        EfiPeiSmbus2Ppi = (EFI_PEI_SMBUS2_PPI *)((UINT8 *)EfiPeiSmbus2Ppi - PrivateData->HeapOffset);
    }
    
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "IpmiInstance Signature after Memory discovered: %x\n", IpmiInstance->Signature));
    
    PeiIpmiDataHob->IpmiInstance = (UINTN)IpmiInstance;
    PeiIpmiDataHob->EfiPeiSmbus2Ppi = (UINTN)EfiPeiSmbus2Ppi;

    return EFI_SUCCESS;
}

/**
    Gets the self test results and update the BmcStatus accordingly in
    IpmiInstance structure. And Install the Ppi if status is not Hard fail.

    @param IpmiInstance    Data structure describing BMC
                           variables and used for sending commands

    @return EFI_STATUS            Status of the function.
    @retval EFI_SUCCESS           Self test command is success and PeiIpmiTransportPpi
                                  is installed successfully.
    @retval EFI_OUT_OF_RESOURCES  Failed to allocate requested memory.
    @retval EFI_UNSUPPORTED       Bmc is in hard fail state.
    @retval Others                Return status from SendIpmiCommand or InstallPpi.

**/
EFI_STATUS CheckSelfTestAndInstallPeiIpmiPpi (
  IN CONST EFI_PEI_SERVICES         **PeiServices,
  IN EFI_IPMI_BMC_INSTANCE_DATA     *IpmiInstance
)
{
    UINT8                           Index;
    IPMI_SELF_TEST_RESULT_RESPONSE  BstStatus;
    UINT8                           DataSize;
    EFI_PEI_PPI_DESCRIPTOR          *PeiIpmiPpiDesc = NULL;
    EFI_STATUS                      Status;


    IpmiInstance->BmcStatus     = EFI_BMC_OK;
    //
    // Get the BMC SELF TEST Results.
    //
    for (Index = 0; Index < IPMI_SELF_TEST_COMMAND_RETRY_COUNT; Index++) {
        DataSize = sizeof (BstStatus);
        Status = IpmiInstance->IpmiTransport.SendIpmiCommand (
                                                &IpmiInstance->IpmiTransport,
                                                IPMI_NETFN_APP,
                                                BMC_LUN,
                                                IPMI_APP_GET_SELFTEST_RESULTS,
                                                NULL,
                                                0,
                                               (UINT8*) &BstStatus,
                                                &DataSize );
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "Get Self test results Index: %x Status %r\n", Index, Status));
        if (!EFI_ERROR(Status)) {
            break;
        }
    }

    DEBUG ((DEBUG_INFO, "Self test result Status: %r\n", Status));
    //
    // If Status indicates a Device error, then the BMC is not responding, so send an error.
    //
    if (EFI_ERROR (Status)) {
        IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
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
                IpmiInstance->BmcStatus = EFI_BMC_OK;
                break;

            case IPMI_APP_SELFTEST_ERROR: // 0x57
                IpmiInstance->BmcStatus = EFI_BMC_SOFTFAIL;
                break;

            default: // 0x58 and Other Device Specific Hardware Error
                IpmiInstance->BmcStatus = EFI_BMC_HARDFAIL;
                break;
        }

        //
        // The Device ID information is only retrieved when the self test has passed or generated a SOFTFAIL.
        // The BMC Status may be overwritten to show the BMC is in Force Update mode.
        // But the BMC Status will always be preserved if a hard fail has occurred since this code won't execute.
        //
        if ((IpmiInstance->BmcStatus == EFI_BMC_OK) || (IpmiInstance->BmcStatus == EFI_BMC_SOFTFAIL)) {
            //
            // Send Get Device ID command
            //
            GetDeviceId (IpmiInstance);
        }
    }
    DEBUG ((DEBUG_INFO, "BMC Status %x\n", IpmiInstance->BmcStatus));
    if (IpmiInstance->BmcStatus != EFI_BMC_HARDFAIL) {
        //
        // Allocate memory for PPI Descriptor
        //
        PeiIpmiPpiDesc = (EFI_PEI_PPI_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
        if (PeiIpmiPpiDesc == NULL) {
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "Failed to allocate memory for PeiIpmiPpiDesc! \n"));
            //
            // AllocateZeroPool has failed and return EFI_OUT_OF_RESOURCES.
            //
            return EFI_OUT_OF_RESOURCES;
        }
    
        PeiIpmiPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
        PeiIpmiPpiDesc->Guid = &gEfiPeiIpmiTransportPpiGuid;
        PeiIpmiPpiDesc->Ppi = &IpmiInstance->IpmiTransport;
    
        Status = (*PeiServices)->InstallPpi (
                    PeiServices,
                    PeiIpmiPpiDesc);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "Install gEfiPeiIpmiTransportPpiGuid PPI  Status %r\n", Status));
        if (EFI_ERROR(Status)) {
            //
            // Error in installing the PPI. So free the allocated memory.
            //
            FreePool (PeiIpmiPpiDesc);
        }
    } else {
        Status = EFI_UNSUPPORTED;
    }
    return Status;
}

/**
    Locate the Smbus2 Ppi with 0 to N instances for SSIF. For each instance,
    Gets the self test results and install the PeiIpmiTransportPpi and
    return the Status accordingly.

    @param PeiServices   Pointer to the PEI Core data Structure.

    @return EFI_STATUS      Status of the function.
    @retval EFI_NOT_FOUND   The AmiPeiIpmiDataHob is not found.
    @retval EFI_SUCCESS     Smbus2Ppi instance of BMC is found and Ppi is
                            installed successfully.
    @retval Others          Return status of other functions used.

**/
EFI_STATUS
LocateAndFindSmbusInstance (
  IN  CONST EFI_PEI_SERVICES    **PeiServices
)
{
    UINTN                               Instance;
    EFI_STATUS                          Status;
    EFI_HOB_GUID_TYPE                   *GuidHob;
    AMI_PEI_IPMI_DATA_HOB               *AmiPeiIpmiDataHob;
    EFI_PEI_SMBUS2_PPI                  *EfiPeiSmbus2Ppi = NULL;
    EFI_IPMI_BMC_INSTANCE_DATA          *IpmiInstance;

    GuidHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gAmiPeiIpmiDataHobGuid);
    if (GuidHob == NULL) {
        ASSERT (GuidHob != NULL);
        return EFI_NOT_FOUND;
    }

    AmiPeiIpmiDataHob = (AMI_PEI_IPMI_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
    IpmiInstance = (EFI_IPMI_BMC_INSTANCE_DATA *)AmiPeiIpmiDataHob->IpmiInstance;

    //
    // Traverse all Smbus2 PPI instances and find the right instance for SSIF.
    //
    for (Instance = 0; ; Instance++) {
        //
        // Locate the SMBUS PPI for Communication and returns Status.
        //
        Status = (*PeiServices)->LocatePpi (
                                   PeiServices,
                                   &gEfiPeiSmbus2PpiGuid,
                                   Instance,
                                   NULL,
                                   (VOID **)&EfiPeiSmbus2Ppi);
        if (EFI_ERROR (Status)) {
            break;
        }
        AmiPeiIpmiDataHob->EfiPeiSmbus2Ppi = (UINTN)(UINTN *)EfiPeiSmbus2Ppi;
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "%a Instance: %d LocatePpi Status %r\n", __FUNCTION__, Instance, Status));

        //
        // Check SSIF interface capability and Global Enables state.
        //
        GetSystemInterfaceCapability (IpmiInstance);
        GetGlobalEnables (IpmiInstance);
        //
        // Get the Self Test Results and install PeiIpmiTrasportPpi if BMC instance is found.
        //
        Status = CheckSelfTestAndInstallPeiIpmiPpi (PeiServices, IpmiInstance);
        if (!EFI_ERROR(Status)) {
            break;
        }
    }
    return Status;
}

/**
    Notify function for EfiPeiSmbus2Ppi and calls LocateAndFindSmbusInstance
    for checking the SSIF instance and install the protocol.
    
    @param PeiServices      Pointer to the PEI Core data Structure.
    @param NotifyDescriptor Notify descriptor of function.
    @param Ppi              Pointer to the Ppi Installed.
    
    @return EFI_STATUS      Status of the function.
    @retval EFI_NOT_FOUND   The AmiPeiIpmiDataHob is not found.
    @retval EFI_SUCCESS     Smbus2Ppi instance of BMC is found and Ppi is
                            installed successfully.
    @retval Others          Return status of other functions used.
**/
EFI_STATUS NotifySmbusInstance (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi
)
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_HOB_GUID_TYPE               *GuidHob;
    AMI_PEI_IPMI_DATA_HOB           *AmiPeiIpmiDataHob;
    EFI_IPMI_BMC_INSTANCE_DATA      *IpmiInstance;

    GuidHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gAmiPeiIpmiDataHobGuid);
    if (GuidHob == NULL) {
        ASSERT (GuidHob != NULL);
        return EFI_NOT_FOUND;
    }

    AmiPeiIpmiDataHob = (AMI_PEI_IPMI_DATA_HOB *)GET_GUID_HOB_DATA (GuidHob);
    IpmiInstance = (EFI_IPMI_BMC_INSTANCE_DATA *)AmiPeiIpmiDataHob->IpmiInstance;
    if ((IpmiInstance->BmcStatus == EFI_BMC_OK) || (IpmiInstance->BmcStatus == EFI_BMC_SOFTFAIL)) {
        return Status;
    }
    AmiPeiIpmiDataHob->EfiPeiSmbus2Ppi = (UINTN)(UINTN *)Ppi;
    //
    // Check SSIF interface capability and Global Enables state.
    //
    GetSystemInterfaceCapability (IpmiInstance);
    GetGlobalEnables (IpmiInstance);
    //
    // Get the Self Test Results and install PeiIpmiTrasportPpi if BMC instance is found.
    //
    Status = CheckSelfTestAndInstallPeiIpmiPpi (PeiServices, IpmiInstance);
    if (EFI_ERROR(Status)) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "%a CheckSelfTestAndInstallPeiIpmiPpi Status: %r\n", __FUNCTION__, Status));
    }
    return Status;
}

/**
    Setup and initialize the BMC for the PEI phase. In order to
    verify the BMC is functioning as expected, the BMC Self-test is performed.
    The results are then checked and any errors are reported to the error manager.
    Errors are collected throughout this routine and reported just prior to 
    installing the driver.  If there are more errors than BST_LOG_SIZE, then they
    will be ignored.

    @param FileHandle    Pointer to image file handle.
    @param PeiServices   Pointer to the PEI Core data Structure

    @return EFI_STATUS            Status of Driver execution
    @retval EFI_SUCCESS           Successful driver initialization
    @retval EFI_UNSUPPORTED       ServerSetup variable is found and BmcSupport is
                                  disabled or Self test has failed or error in
                                  installing ppi.
    @retval EFI_OUT_OF_RESOURCES  Failed to allocate requested memory.
    @retval Others                Return status from SendIpmiCommand or InstallPpi.

**/
EFI_STATUS
InitializeIpmiPhysicalLayer (
  IN        EFI_PEI_FILE_HANDLE     FileHandle,
  IN  CONST EFI_PEI_SERVICES        **PeiServices )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    EFI_IPMI_BMC_INSTANCE_DATA          *IpmiInstance = NULL;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI     *PeiVariable;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    UINTN                               Size;
    EFI_PEI_I2C_MASTER_PPI              *MasterTransmit = NULL;
    EFI_PEI_SMBUS2_PPI                  *EfiPeiSmbus2Ppi = NULL;
    AMI_PEI_IPMI_DATA_HOB               *AmiPeiIpmiDataHob;
    EFI_PEI_NOTIFY_DESCRIPTOR           SmbusPpiNotifyDesc[] = {
                                                                EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
                                                                &gEfiPeiSmbus2PpiGuid,
                                                                NotifySmbusInstance
                                                               };
    EFI_PEI_NOTIFY_DESCRIPTOR           NotifyList[] = {
                                                        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
                                                        &gEfiPeiMemoryDiscoveredPpiGuid,
                                                        UpdateIpmiHobPtrs
                                                       };

    //
    // Locate the Setup configuration value.
    //
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiReadOnlyVariable2PpiGuid,
                                0,
                                NULL,
                                (VOID **)&PeiVariable);
    SERVER_IPMI_DEBUG ((DEBUG_INFO, "LocatePpi for gEfiPeiReadOnlyVariable2PpiGuid Status %r\n", Status));
    ASSERT_EFI_ERROR (Status);

    if (!EFI_ERROR(Status)) {
        Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
        Status = PeiVariable->GetVariable (
                                PeiVariable,
                                L"ServerSetup",
                                &gEfiServerMgmtSetupVariableGuid,
                                NULL,
                                &Size,
                                &ServerMgmtConfiguration);
        SERVER_IPMI_DEBUG ((DEBUG_INFO, "PeiVariable->GetVariable for ServerSetup Status %r ServerMgmtConfiguration.BmcSupport: %x \n", Status, ServerMgmtConfiguration.BmcSupport));

        if (!EFI_ERROR(Status) && !ServerMgmtConfiguration.BmcSupport) {
            return EFI_UNSUPPORTED;
        }
    }

#if BMC_INIT_DELAY
    if (EFI_ERROR(Status)) {
         ServerMgmtConfiguration.WaitForBmc = 0; //Load default value.
    }
#endif

    //
    // Allocate memory for IPMI Instance
    //
    IpmiInstance = (EFI_IPMI_BMC_INSTANCE_DATA*)AllocateZeroPool (sizeof (EFI_IPMI_BMC_INSTANCE_DATA));
    if (IpmiInstance == NULL) {
        SERVER_IPMI_DEBUG ((DEBUG_ERROR, "Failed to allocate memory for IpmiInstance! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    //
    //Platform Hook to initialize LPC for BMC and Host interface.
    //
    Status = InitLpcBmcHook (PeiServices);
    if (EFI_ERROR(Status)) {
        //
        // InitLpcBmcHook has failed, so free the allocated memory and return Status.
        //
        FreePool (IpmiInstance);
        return Status;
    }

    if (IPMI_DEFAULT_HOOK_SUPPORT) {
        //
        // Initialize IPMI Instance Data
        //
        if (IPMI_SYSTEM_INTERFACE == KCS_INTERFACE) { // KCS Interface
            IpmiInstance->Interface.KCS.DataPort        = IPMI_KCS_DATA_PORT;       // KCS Data Port
            IpmiInstance->Interface.KCS.CommandPort     = IPMI_KCS_COMMAND_PORT;    // KCS Command Port
        } else if (IPMI_SYSTEM_INTERFACE == BT_INTERFACE) { // BT_Interface
            IpmiInstance->Interface.BT.CtrlPort    = IPMI_BT_CTRL_PORT;     // BT Control Port
            IpmiInstance->Interface.BT.ComBuffer   = IPMI_BT_BUFFER_PORT; // BT Buffer Port
            IpmiInstance->Interface.BT.IntMaskPort = IPMI_BT_INTERRUPT_MASK_PORT; // BT IntMask Port
        }
        IpmiInstance->MmioBaseAddress = 0;
        IpmiInstance->AccessType = IPMI_IO_ACCESS;
        IpmiInstance->BaseAddressRange = 0;
    } else {
#if (!IPMI_DEFAULT_HOOK_SUPPORT)
        IpmiInstance->AccessType = IPMI_MMIO_ACCESS;
        Status = IpmiOemPeiInitHook (PeiServices, &IpmiInstance->MmioBaseAddress, &IpmiInstance->BaseAddressRange);
        if (EFI_ERROR (Status)) {
            //
            // IpmiOemPeiInitHook has failed, so free the allocated memory and return Status.
            //
            FreePool (IpmiInstance);
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "IpmiOemPeiInitHook is failed and Status is %r.\n", Status));
            return Status;
        }
#endif
    }
    IpmiInstance->Signature     = SM_IPMI_BMC_SIGNATURE;
    IpmiInstance->SlaveAddress  = IPMI_BMC_SLAVE_ADDRESS;
    IpmiInstance->IpmiTransport.GetBmcStatus = EfiIpmiBmcStatus;

    if (IPMI_SYSTEM_INTERFACE == KCS_INTERFACE) { // KCS Interface
        IpmiInstance->Interface.KCS.KcsRetryCounter = IPMI_KCS_COMMAND_PORT_READ_RETRY_COUNTER;
        IpmiInstance->IpmiTransport.SendIpmiCommand = EfiIpmiSendCommand;       // KCS SendCommand
        IpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiIpmiSendCommandEx; // KCS SendCommand Extension
    } else if (IPMI_SYSTEM_INTERFACE == BT_INTERFACE) { // BT_Interface
        IpmiInstance->Interface.BT.BtDelay     = BT_DELAY; // BT Delay
        IpmiInstance->IpmiTransport.SendIpmiCommand = EfiBtIpmiSendCommand; // BT SendCommand
        IpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiBtIpmiSendCommandEx; // BT SendCommand Extension
    } else if (IPMI_SYSTEM_INTERFACE == SSIF_INTERFACE) { // SSIF Interface
        IpmiInstance->IpmiTransport.SendIpmiCommand = EfiSsifIpmiSendCommand;   // SSIF SendCommand
        IpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiSsifIpmiSendCommandEx; // SSIF SendCommand Extension
        IpmiInstance->Interface.SSIF.SsifRetryCounter = IPMI_SSIF_COMMAND_REQUEST_RETRY_COUNTER; // SSIF retry counter
        IpmiInstance->Interface.SSIF.PecSupport = FALSE; // SSIF PEC support
        IpmiInstance->Interface.SSIF.RwSupport = 0x0; // SSIF multi-part reads/writes support
        IpmiInstance->Interface.SSIF.SmbAlertSupport = FALSE; // Smb alert pin support

        AmiPeiIpmiDataHob = BuildGuidHob (&gAmiPeiIpmiDataHobGuid, sizeof(AMI_PEI_IPMI_DATA_HOB));
        AmiPeiIpmiDataHob->IpmiInstance = (UINTN)IpmiInstance;
        AmiPeiIpmiDataHob->EfiPeiSmbus2Ppi = 0;
        // Register Memory discovered PPI call back to update HOB with new IpmiInstance pointer
        Status = (*PeiServices)->NotifyPpi (PeiServices, &NotifyList[0]);
        if (EFI_ERROR(Status)) {
            // Failed to Register Memory discovered PPI call back
            SERVER_IPMI_DEBUG ((DEBUG_ERROR, "Failed to Register Memory discovered PPI call back. %r\n", Status));  
            FreePool (IpmiInstance);
            return Status;
        }
        WaitForBmcDelay (PeiServices, ServerMgmtConfiguration.WaitForBmc);
        Status = LocateAndFindSmbusInstance (PeiServices);
        if (EFI_ERROR(Status)) {
            ///
            /// LocateAndFindSmbusInstance has failed, so register NotifyPpi and return Status.
            ///
            Status = (*PeiServices)->NotifyPpi (PeiServices, SmbusPpiNotifyDesc);
            if (EFI_ERROR(Status)) {
                SERVER_IPMI_DEBUG ((DEBUG_ERROR, "NotifyPpi for EfiPeiSmbus2Ppi is failed and Status is %r.\n", Status));
                return EFI_UNSUPPORTED;
            }
        }
        return EFI_SUCCESS;
    } else {
        //
        // Locate the I2C Ppi for communication.
        // Return if Ppi is not available.
        //
        Status = (*PeiServices)->LocatePpi (
                                    PeiServices,
                                    &gEfiPeiI2cMasterPpiGuid,
                                    0,
                                    NULL,
                                    (VOID **)&MasterTransmit);
        if (EFI_ERROR(Status)) {
            //
            // LocatePpi has failed, so freeing the allocated memory and return Status.
            //
            FreePool (IpmiInstance);
            return Status;
        }
        IpmiInstance->IpmiTransport.SendIpmiCommand = EfiIpmbIpmiSendCommand; // IPMB SendCommand
        IpmiInstance->IpmiTransport.SendIpmiCommandEx = EfiIpmbIpmiSendCommandEx; // IPMB SendCommand Extension
    }

    WaitForBmcDelay (PeiServices, ServerMgmtConfiguration.WaitForBmc);

    //
    // Now install the PPI if the BMC is not in a Hard Fail State
    //
    Status = CheckSelfTestAndInstallPeiIpmiPpi (PeiServices, IpmiInstance);
    if (EFI_ERROR(Status)) {
        //
        // Self test has failed or Error in installing the PPI. So free the allocated memory and return EFI_UNSUPPORTED to unload driver from memory.
        //
        FreePool (IpmiInstance);
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
