//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file 
    Detect pending Capsule/Recovery flash update and update BootMode accordingly
**/
#include "Efi.h"
#include "Pei.h"
#include "Token.h"
#include <AmiPeiLib.h>
//#include <Hob.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <FlashUpd.h>
#include <Capsule.h>
#include <Guid/CapsuleVendor.h>
#include <Library/DebugLib.h>

//------------------------------------------------------------------------
// Definitions
//------------------------------------------------------------------------
#if FWCAPSULE_RECOVERY_SUPPORT == 1 && CSLIB_WARM_RESET_SUPPORTED == 0
// {64C96700-6B4C-480C-A3E1-B8BDE8F602B2}
#define AMI_PEI_AFTER_MRC_GUID \
 {0x64c96700, 0x6b4c, 0x480c, 0xa3, 0xe1, 0xb8, 0xbd, 0xe8, 0xf6, 0x2, 0xb2}
 EFI_GUID gAmiPeiAfterMrcGuid      = AMI_PEI_AFTER_MRC_GUID;
#endif
static EFI_GUID gFlashUpdGuid            = FLASH_UPDATE_GUID;

//------------------------------------------------------------------------
// Local prototypes
//------------------------------------------------------------------------
EFI_STATUS ChangeBootMode (
    IN EFI_PEI_SERVICES         **PeiServices
);

EFI_STATUS EFIAPI ChangeBootModeAfterEndofMrc (
    IN EFI_PEI_SERVICES         **PeiServices,
    IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
    IN VOID                     *InvokePpi
);

//------------------------------------------------------------------------
// PPIs notifications to be  installed
//------------------------------------------------------------------------
//static EFI_PEI_PPI_DESCRIPTOR mFlashUpdateModePpi[] =
//  {
//   (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
//   &gFlashUpdBootModePpiGuid,
//    NULL
//};
static EFI_PEI_PPI_DESCRIPTOR mRecoveryModePpi[] =
{
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiBootInRecoveryModePpiGuid,
    NULL
};

//------------------------------------------------------------------------
// PPI event to  hook up to
//------------------------------------------------------------------------
#if FWCAPSULE_RECOVERY_SUPPORT == 1 && CSLIB_WARM_RESET_SUPPORTED == 0
static EFI_PEI_NOTIFY_DESCRIPTOR EndOfMrcNotifyList[] = {
    { EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK |  EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
      &gAmiPeiAfterMrcGuid,
      ChangeBootModeAfterEndofMrc
    },
};
#endif
//--------------------------------------------------------------------------
/**
  This function checks if recovery flow is needed due to pending Flash Update

  @param[in] PeiServices Pointer to the PEI services table
  @param[in] FlashUpdDesc
  @param[in] CounterHi

  @retval BOLEAN  TRUE if variable is present
**/
BOOLEAN GetFlashUpdateVar (
    EFI_PEI_SERVICES    **PeiServices,
    AMI_FLASH_UPDATE_BLOCK *FlashUpdDesc,
    UINT32                  *CounterHi
){
    EFI_STATUS      Status;
    UINTN           Size;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadOnlyVariable;

// Check FlashOp variable for Recovery Flash operation 
// Detect if we are in Flash Update mode and set some recovery global variables
// Read "FlashOp" Variable to update global RecoveryFileName, Size
    Status = (*PeiServices)->LocatePpi( PeiServices,
                                &gEfiPeiReadOnlyVariable2PpiGuid,//gPeiReadOnlyVariablePpiGuid,
                                0,
                                NULL,
                                &ReadOnlyVariable );
    if(EFI_ERROR(Status)) 
        return FALSE;

    Size = sizeof(AMI_FLASH_UPDATE_BLOCK);
    Status = ReadOnlyVariable->GetVariable( ReadOnlyVariable,
                                    FLASH_UPDATE_VAR,
                                    &gFlashUpdGuid,
                                    NULL,
                                    &Size,
                                    FlashUpdDesc );
    if (!EFI_ERROR(Status)) 
    {
        Size = sizeof(UINT32);
        Status = ReadOnlyVariable->GetVariable(ReadOnlyVariable,
                                        L"MonotonicCounter", 
                                        &gAmiGlobalVariableGuid,
                                        NULL, 
                                        &Size,
                                        CounterHi);
        if (EFI_ERROR(Status) || FlashUpdDesc->MonotonicCounter == 0xffffffff)
            *CounterHi = 0xffffffff;
        
        return TRUE;
    }

    return FALSE;
}

/**
  This function returns proper BootMode if Flash Update mode is pending

  @param[in]  PeiServices Pointer to the PEI services table
  @param[out] updated BootMode value if TRUE

  @retval BOLEAN
      TRUE - recovery is requested
      FALSE - recovery is not requested
**/
BOOLEAN IsFlashUpdate(
    EFI_PEI_SERVICES    **PeiServices,
    EFI_BOOT_MODE       *BootMode
){
    UINT32                  CounterHi;
    AMI_FLASH_UPDATE_BLOCK FlashUpdDesc;

    CounterHi = 0;
    FlashUpdDesc.MonotonicCounter = CounterHi;
    FlashUpdDesc.FlashOpType = FlDisabled;

 //SetMode should have set FlashUpd var even if no MC var detected.
// MC check should fail Recovery
    if( GetFlashUpdateVar(PeiServices, &FlashUpdDesc, &CounterHi) &&
        CounterHi==(UINT32)FlashUpdDesc.MonotonicCounter)
    {
        if(FlashUpdDesc.FlashOpType == FlRecovery)
        {
            *BootMode = BOOT_IN_RECOVERY_MODE;
            return TRUE;
        }
        if(FlashUpdDesc.FlashOpType == FlCapsule)
        {
            *BootMode = BOOT_ON_FLASH_UPDATE;
            return TRUE;  
        }
    }
    return FALSE;
}

/**
  The function updates a BootMode to BOOT_IN_RECOVERY_MODE
  if Flash Update variable is installed and the flash mode is set to Recovery

  @param[in]  PeiServices Pointer to the PEI services table
  @param[out]  BootMode

  @retval     Status
**/
EFI_STATUS ChangeBootMode (
    IN EFI_PEI_SERVICES **PeiServices
    )
{
    EFI_BOOT_MODE        BootMode;

    (*PeiServices)->GetBootMode( PeiServices, &BootMode );
    if ((BootMode != BOOT_IN_RECOVERY_MODE) && (BootMode != BOOT_ON_S3_RESUME)) {
        // Change the Boot Mode to Recovery from regular boot mode
        if(IsFlashUpdate(PeiServices, &BootMode) && BootMode == BOOT_IN_RECOVERY_MODE) {
                (*PeiServices)->SetBootMode(PeiServices, BootMode);
                (*PeiServices)->InstallPpi(PeiServices, mRecoveryModePpi);
         }
    }

    return EFI_SUCCESS;
}

/**
  Change BootMode to BOOT_ON_FLASH_UPDATE if Capsule update is pending
  Install FlashUpdatePpi to initiate Recovery Capsule validation

  @param[in]  PeiServices Pointer to the PEI services table
  @param[out]  BootMode

  @retval     Status EFI_SUCCESS               
**/
#if FWCAPSULE_RECOVERY_SUPPORT == 1 && CSLIB_WARM_RESET_SUPPORTED == 0
EFI_STATUS EFIAPI ChangeBootModeAfterEndofMrc (
        IN EFI_PEI_SERVICES **PeiServices,
        IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
        IN VOID *InvokePpi)
{
    EFI_BOOT_MODE        BootMode;

    (*PeiServices)->GetBootMode( PeiServices, &BootMode );
//    DEBUG ((SecureMod_DEBUG_LEVEL, "FWCapsule AfterMrc BootMode %x\n", BootMode));
    // BOOT_ON_FLASH_UPDATE will be set in Capsule PPI for FwCapsule
    if(BootMode == BOOT_ON_S3_RESUME) {
        // Change the Boot Mode to BOOT_ON_FLASH_UPDATE from S3_RESUME
       if (IsFlashUpdate(PeiServices, &BootMode) && (BootMode == BOOT_ON_FLASH_UPDATE)) {
           (*PeiServices)->SetBootMode(PeiServices, BootMode);
//           (*PeiServices)->InstallPpi( PeiServices, mFlashUpdateModePpi);
       }
    }
//    DEBUG ((SecureMod_DEBUG_LEVEL, "New BootMode %x\n", BootMode));

    return EFI_SUCCESS;
}
#endif

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI

**/
EFI_STATUS
IsSecRecoveryPeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
#if FWCAPSULE_RECOVERY_SUPPORT == 1 && CSLIB_WARM_RESET_SUPPORTED == 0
    (*PeiServices)->NotifyPpi( PeiServices, EndOfMrcNotifyList);
#endif
    return ChangeBootMode(PeiServices);
}
