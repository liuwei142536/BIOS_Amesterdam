
#include <Token.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>

#include <Library\UefiLib.h>

#include <Guid/SetupVariable.h>

#include <Protocol/SmmControl2.h> //AptioV Server override: Trigger Smi to enable Pci Errors

EFI_STATUS
TriggerSmiForPciErrorsEnable (
  EFI_EVENT   Event,
  VOID        *Context )
{
    EFI_STATUS                  Status;
    EFI_SMM_CONTROL2_PROTOCOL   *SmmControl2 = NULL;
    EFI_GUID                    gEfiSmmControl2ProtocolGuid = EFI_SMM_CONTROL2_PROTOCOL_GUID;
    UINT8                       DataSize = 1;
    UINT8                       SwSmiValue = PCIE_ELOG_SWSMI;
    
    DEBUG((DEBUG_ERROR, "Pcielog: Triggering PCIe Error Logging Callback...\n"));
    
    //
    // Locate Smm Control2 protocol
    //
    Status = gBS->LocateProtocol (&gEfiSmmControl2ProtocolGuid, NULL, (VOID **)&SmmControl2);
    if(EFI_ERROR(Status)){
        DEBUG((DEBUG_ERROR, "Error in locating Smm Control2 protocol Status: %r ...\n", Status));
        ASSERT_EFI_ERROR(Status);
        return Status;
    }     
    //
    // Trigger SMI
    //
    SmmControl2->Trigger(SmmControl2, &SwSmiValue, &DataSize, 0, 0);

    //
    // Close Event
    //
    gBS->CloseEvent(Event);

    return EFI_SUCCESS;
}
//AptioV Server override End: Trigger Smi to enable Pci Errors

EFI_STATUS
InitializeAmiErrorLog (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{

    EFI_STATUS              Status = EFI_SUCCESS;
    UINTN                   VarSize = sizeof(SYSTEM_CONFIGURATION);
    SYSTEM_CONFIGURATION    SetupData;
    EFI_EVENT               ReadyToBootEvent;
    
    Status = gRT->GetVariable( 
                L"IntelSetup",
                &gEfiSetupVariableGuid,
                NULL,
                &VarSize,
                &SetupData 
                );
    if( EFI_ERROR(Status) ) {
        DEBUG ((DEBUG_ERROR, "AmiErrorLog: Failed to Get IntelSetup Variable! \n"));
        return EFI_SUCCESS;
    }
    
    DEBUG ((DEBUG_ERROR, "AmiErrorLog: SetupData.SystemErrorEn = %x \n", SetupData.SystemErrorEn ));

    //
    // Return if SystemErrorEn is not enabled. 
    //
    if ( SetupData.SystemErrorEn != 1 ) {
        return EFI_SUCCESS;
    }
    
    Status = EfiCreateEventReadyToBootEx (
                TPL_CALLBACK,
                (EFI_EVENT_NOTIFY)TriggerSmiForPciErrorsEnable,
                NULL,
                &ReadyToBootEvent );
    DEBUG ((DEBUG_ERROR, "AmiErrorLog: EfiCreateEventReadyToBootEx Status: %r \n", Status ));
    
    return EFI_SUCCESS;
}
