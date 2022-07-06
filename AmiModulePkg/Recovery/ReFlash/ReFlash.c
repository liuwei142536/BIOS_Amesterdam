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

/** @file
  This file contains Reflash driver entry point and Reflash Setup 
  initialization code

**/

#include <Hob.h>
#include <Flash.h>
#include <AmiCspLib.h>
#include <AmiDxeLib.h>
#include <AmiHobs.h>
#include <FlashUpd.h>
#include <ReflashDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/AmiPowerStateLib.h>
#include <Library/HobLib.h>
#include <Protocol/AmiFlash.h>

#include "ReFlash.h"
#include "ReFlashSmm.h"

#ifndef FtRecovery_SUPPORT
#define FtRecovery_SUPPORT 0
BOOLEAN IsTopSwapOn (VOID);
#endif


EFI_GUID gAmiRecoveryFormsetGuid = RECOVERY_FORM_SET_GUID;
EFI_HANDLE ThisImageHandle;
AMI_FLASH_PROTOCOL *Flash;
EFI_HII_HANDLE ReflashHiiHandle = NULL;
UINT8 *RecoveryBuffer = NULL;
UINT32 RecoveryBufferSize = 0;
EFI_STATUS RecoveryStatus = EFI_SUCCESS;
AMI_REFLASH_SMM_COMM_BUFFER *SmmCommBuffer = NULL;
BOOLEAN UpdateNvramFlag = TRUE;

/**
  This function executes OEM porting hooks before starting flash update
**/
VOID OemBeforeFlashCallback(
    VOID
)
{
    UINT32 i;
    for(i = 0; OemBeforeFlashCallbackList[i] != NULL; i++)
        OemBeforeFlashCallbackList[i]();
}

/**
  This function executes OEM porting hooks after finishing flash update
**/
VOID OemAfterFlashCallback(
    VOID
)
{
    UINT32 i;
    for(i = 0; OemAfterFlashCallbackList[i] != NULL; i++)
        OemAfterFlashCallbackList[i]();
}

/**
  This function returns String from HII database
  
  @param HiiHandle Handle of corresponding HII package list
  @param Token     ID of the string to be returned
  @param DataSize  Size of the preallocated buffer in bytes
  @param Data      Pointer where to put retrieved string

  @retval EFI_SUCCESS String returned successfully
  @retval other       error occured during execution

**/
EFI_STATUS GetHiiString(
    IN EFI_HII_HANDLE HiiHandle,
    IN STRING_REF Token,
    IN OUT UINTN *DataSize, 
    OUT EFI_STRING *Data
)
{
    EFI_STATUS Status;
    
    if (*Data == NULL) 
        *DataSize = 0;
    
    Status = HiiLibGetString(HiiHandle, Token, DataSize, *Data);
    if (!EFI_ERROR(Status)) 
        return Status;

    if (Status == EFI_BUFFER_TOO_SMALL) {
        if (*Data) 
            pBS->FreePool(*Data);
        
        Status = pBS->AllocatePool(EfiBootServicesData, *DataSize, (VOID **)Data);
        if (EFI_ERROR(Status)) 
            return Status;
        
        Status = HiiLibGetString(HiiHandle, Token, DataSize, *Data);
    }
    
    return Status;
}

/**
  This function updates flash parameteres based on user selection
  or Setup values
  
  @param Interactive If TRUE get values from user input, otherwise get values
                     from Setup variable

**/
VOID ApplyUserSelection(
    IN BOOLEAN Interactive
)
{
    EFI_STATUS Status;
    AUTOFLASH FlashUpdateControl;
    UINTN Size = sizeof(FlashUpdateControl);
    UINT32 i;

    if(Interactive) {
    /* get values from Setup Browser */
        Status = HiiLibGetBrowserData(&Size, &FlashUpdateControl, &gAmiRecoveryFormsetGuid, L"Setup");
    } else {
    /* get values from NVRAM */
        Status = pRS->GetVariable(L"Setup", &gAmiRecoveryFormsetGuid, NULL, &Size, &FlashUpdateControl);
    }
    
    if(EFI_ERROR(Status) || FlashUpdateControl.PartialUpdateAllowed == 0)
    /* no user selection, flash everything */
        return;

    for(i = 0; BlocksToUpdate[i].Type != FvTypeMax; i++) {
        switch(BlocksToUpdate[i].Type) {
            case FvTypeBootBlock:
                BlocksToUpdate[i].Update = FlashUpdateControl.UpdateBb;
                if(FtRecovery_SUPPORT && IsTopSwapOn())   //if we're here BB update failed we use backup copy - force BB update again
                    BlocksToUpdate[i].Update = TRUE;
                break;
            case FvTypeNvRam:
                BlocksToUpdate[i].Update = FlashUpdateControl.UpdateNv;
                break;
            default:
                break;
        }
    }
    UpdateNvramFlag = FlashUpdateControl.UpdateNv;
}

/**
  This function updates status strings in setup window, based
  on execution results
  
  @param Handle     Handle of corresponding HII package list
  @param AutoFlash  Pointer to flash parameters variable

**/
VOID UpdateSetupStrings(
    IN EFI_HII_HANDLE Handle,
    IN AUTOFLASH *AutoFlash
)
{
    UINTN Size;
    EFI_STRING Template = NULL;
    EFI_STRING Template2 = NULL;
    CHAR16 ReportString[100];

//prepare STR_SUBTITLE1 String
    GetHiiString(Handle, STRING_TOKEN(STR_SUBTITLE1_TEMPLATE), &Size, &Template);

    switch(AutoFlash->VerificationError) {
        case InvalidHeader:
            GetHiiString(Handle, STRING_TOKEN(STR_ERR), &Size, &Template2);
            break;
        case InvalidSignature:
            GetHiiString(Handle, STRING_TOKEN(STR_ERR1), &Size, &Template2);
            break;
        case IvalidPlatformKey:
            GetHiiString(Handle, STRING_TOKEN(STR_ERR2), &Size, &Template2);
            break;
        case InvalidFwVersion:
            GetHiiString(Handle, STRING_TOKEN(STR_ERR3), &Size, &Template2);
            break;
        default:
            GetHiiString(Handle, STRING_TOKEN(STR_ERR4), &Size, &Template2);
            break;
    }

    if((Template != NULL) && (Template2 != NULL)) {
        Swprintf(ReportString, Template, Template2);
        HiiLibSetString(Handle, STRING_TOKEN(STR_SUBTITLE1), ReportString);
        pBS->FreePool(Template);
        pBS->FreePool(Template2);
        Template = NULL;
    }

//prepare STR_SUBTITLE2 String
    if(AutoFlash->VerificationStatus == 0) {
        GetHiiString(Handle, STRING_TOKEN(STR_SUBTITLE2_WARNING), &Size, &Template);
    } else {
        GetHiiString(Handle, STRING_TOKEN(STR_SUBTITLE2_ERROR), &Size, &Template);
    }

    if(Template != NULL) {
        HiiLibSetString(Handle, STRING_TOKEN(STR_SUBTITLE2), Template);
        pBS->FreePool(Template);
    }
}

/**
 * @brief Publish firmware update setup page
 */
VOID InitHiiHandle (VOID)
{
    static EFI_HII_CONFIG_ACCESS_PROTOCOL CallBack = { NULL,NULL,FlashProgressEx };
    static CALLBACK_INFO SetupCallBack[] =	{
        // Last field in every structure will be filled by the Setup
        { &gAmiRecoveryFormsetGuid, &CallBack, RECOVERY_FORM_SET_CLASS, 0, 0},
    };
	
    LoadResources(ThisImageHandle, sizeof(SetupCallBack) / sizeof(CALLBACK_INFO), SetupCallBack, NULL);
    ReflashHiiHandle = SetupCallBack[0].HiiHandle;
}

EFI_STATUS PrepareStatusVariable (
    IN RECOVERY_IMAGE_HOB *RecoveryHob
    )
{
    AMI_FW_UPDATE_STATUS FwStatus;
    EFI_STATUS Status;
/* set version we're upgrading to */
    FwStatus.Version = GetFirmwareVersion(RecoveryBuffer);
    FwStatus.Status = RecoveryHob->FailedStage;
    if (FwStatus.Status == 0) {
        BOOLEAN AcConnected;
        UINT16 BatteryCharge;
        Status = AmiGetPowerState (&AcConnected, &BatteryCharge);
        if (!EFI_ERROR (Status) && !AcConnected) {	//no ac
            if (DISABLE_FLASH_UPDATE_WITH_NO_AC != 0)
                FwStatus.Status = 5;	//disable update with ac not connected
            else if (BatteryCharge < FLASH_UPDATE_BATTERY_THRESHOLD)
            	FwStatus.Status = 6;	//disable update with no ac and low battery
        }
    }
    pRS->SetVariable(FW_VERSION_VARIABLE, &gAmiGlobalVariableGuid,
                              EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                              sizeof(AMI_FW_UPDATE_STATUS), &FwStatus);
    if (FwStatus.Status == 0)
        return EFI_SUCCESS;         //everything is OK
    else if (FwStatus.Status >= 5)
        return EFI_ACCESS_DENIED;   //battery or AC problem do not flash

    //if we're here Failed stage is between 1 and 4, so we have to check for
    //possible user override
    Status = RecoveryHob->Status;
    if (Status)
        Status |= EFI_ERROR_BIT;
    return Status;
}

/**
  The user Entry Point for Reflash driver. The user code starts with this function.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS EFIAPI ReFlashEntry (
    IN EFI_HANDLE ImageHandle, 
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
    static EFI_GUID guidBootFlow = BOOT_FLOW_VARIABLE_GUID;
    UINT32 BootFlow = BOOT_FLOW_CONDITION_RECOVERY;
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_BOOT_MODE BootMode;
    RECOVERY_IMAGE_HOB *RecoveryHob;
    UINTN Size;
    UINT32 Attributes;
    UINT32 FailedStage;
    UINT32 RomLayoutState;
    EFI_STRING Template = NULL;

    AUTOFLASH AutoFlash = {
        0, 
        0, 
        0, 
        REFLASH_UPDATE_NVRAM, 
        REFLASH_UPDATE_BOOT_BLOCK, 
        REFLASH_UPDATE_MAIN_BLOCK,
        0
    };
    
    AUTOFLASH2 AutoFlash2 = {
    	0
    };

    ThisImageHandle = ImageHandle;
    InitAmiLib(ImageHandle,SystemTable);
    
    //Get Boot Mode
    BootMode = GetBootModeHob ();
    //allocate RT buffer for SMM communication
    pBS->AllocatePool (EfiRuntimeServicesData, 
            sizeof (AMI_REFLASH_SMM_COMM_BUFFER), (VOID **)&SmmCommBuffer);
    
    //check if Recovery HOB is present
    RecoveryHob = (RECOVERY_IMAGE_HOB *) GetFirstGuidHob (&gAmiRecoveryImageHobGuid);
    if (RecoveryHob != NULL) {
        RecoveryBuffer = (UINT8 *)(UINTN)RecoveryHob->Address;
        RecoveryBufferSize = RecoveryHob->ImageSize;
        FailedStage = RecoveryHob->FailedStage;
        RecoveryStatus = PrepareStatusVariable (RecoveryHob);
    }

    if (BootMode != BOOT_IN_RECOVERY_MODE && BootMode != BOOT_ON_FLASH_UPDATE) {
    	SetBdsCapsuleLoadCallback ();
    	InstallEsrtTable();
    	Status = CheckSetupFlashUpdate ();
    	if (EFI_ERROR (Status)) {
    	    pBS->FreePool (SmmCommBuffer);
    	    return EFI_SUCCESS;
    	} else {
            return SetupFlashUpdateFlow ();
    	}
    }

	// If we are on the flash upadte boot path, apply AFU update settings
    if(BootMode == BOOT_ON_FLASH_UPDATE) {
        static EFI_GUID FlashUpdGuid = FLASH_UPDATE_GUID;
        AMI_FLASH_UPDATE_BLOCK  FlashUpdDesc;	
    	// Prep the FlashOp variable
        Size = sizeof(AMI_FLASH_UPDATE_BLOCK);
        Status = pRS->GetVariable( FLASH_UPDATE_VAR, &FlashUpdGuid, NULL, &Size, &FlashUpdDesc);
        if(!EFI_ERROR(Status))
        {
            AutoFlash.UpdateNv   = (FlashUpdDesc.ROMSection & (1 << FV_NV))   ? 1 : 0;
            AutoFlash.UpdateBb   = (FlashUpdDesc.ROMSection & (1 << FV_BB))   ? 1 : 0;
            AutoFlash.UpdateMain = (FlashUpdDesc.ROMSection & (1 << FV_MAIN)) ? 1 : 0;
        }
    }

    Status =pBS->LocateProtocol(&gAmiFlashProtocolGuid, NULL, (VOID **)&Flash);
    if (EFI_ERROR(Status))
        return EFI_UNLOAD_IMAGE;
    
    AutoFlash.VerificationStatus = (UINT8)RecoveryStatus;
    AutoFlash.VerificationError = (UINT8)FailedStage;
    AutoFlash.UserOverride = (AutoFlash.VerificationError == 0 && AutoFlash.VerificationStatus == 0) ? 0 : 1;
    if (!EFI_ERROR (RecoveryStatus))
        AutoFlash.PartialUpdateAllowed = CheckRomLayout (&RomLayoutState);
    else
        AutoFlash.PartialUpdateAllowed = 0;


//Update Reflash parameters
    Size = sizeof(AUTOFLASH);
    Attributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;
    pRS->SetVariable(L"Setup", &gAmiRecoveryFormsetGuid, Attributes, Size, &AutoFlash);
        
    Size = sizeof(AUTOFLASH2);
    pRS->SetVariable(L"Setup2", &gAmiRecoveryFormsetGuid, Attributes, Size, &AutoFlash2);

    if (EFI_ERROR (RecoveryStatus))
        pBS->FreePool (SmmCommBuffer);
    
//Verify if we're on OS firmware update path
    if (IsWin8Update(RecoveryStatus))
        return EFI_SUCCESS;

//Load setup page and create error message if necessary
    InitHiiHandle ();
    pRS->SetVariable(L"BootFlow", &guidBootFlow, EFI_VARIABLE_BOOTSERVICE_ACCESS, sizeof(BootFlow), &BootFlow);

    if(AutoFlash.UserOverride == 1)
        UpdateSetupStrings(ReflashHiiHandle, &AutoFlash);
    
    if (RomLayoutState != 0) {
        if (RomLayoutState == 1)	//system boot from new image
            GetHiiString(ReflashHiiHandle, STRING_TOKEN(STR_PARTIAL_UPDATE1), &Size, &Template);
        else						//actual rom layout mismatch
            GetHiiString(ReflashHiiHandle, STRING_TOKEN(STR_PARTIAL_UPDATE2), &Size, &Template);
		
    	if(Template != NULL) {
            HiiLibSetString(ReflashHiiHandle, STRING_TOKEN(STR_PARTIAL_UPDATE), Template);
            pBS->FreePool (Template);
    	}
    }
    
    return EFI_SUCCESS;
}

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
