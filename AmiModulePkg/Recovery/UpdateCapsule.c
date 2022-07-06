//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017 American Megatrends, Inc.         **
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
  This file contains implementation of Capsule recovery device

**/

#include <AmiHobs.h>
#include <AmiPeiLib.h>
#include <Capsule.h>
#include <Library/HobLib.h>
#include <Library/AmiCapsuleUpdateLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Ppi/DeviceRecoveryModule.h>
#include <Guid/AmiRecoveryDevice.h>

EFI_STATUS VerifyFwImage(
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN OUT VOID          **pCapsule,
  IN OUT UINT32         *pCapsuleSize,
  IN OUT UINT32         *FailedVTask
);

/**
  This function searches for recovery capsule image
  
  @param Address
  @param Size

  @retval EFI_SUCCESS   Image found
  @retval EFI_NOT_FOUND Image not found
**/
EFI_STATUS FindFwCapsuleHob (
    OUT EFI_CAPSULE_HEADER **Address,
    OUT UINT32 *Size
)
{
    VOID *HobStart;
    EFI_HOB_UEFI_CAPSULE *Hob;
    EFI_CAPSULE_HEADER *Capsule;

    HobStart = GetHobList ();
    if (HobStart == NULL)
        return EFI_NOT_FOUND;

    do {
        Hob = (EFI_HOB_UEFI_CAPSULE *) GetNextHob (EFI_HOB_TYPE_UEFI_CAPSULE, HobStart);
        if (Hob != NULL) {
            Capsule = (EFI_CAPSULE_HEADER *) (VOID *) (UINTN) Hob->BaseAddress;
            if (AmiIsFwUpdateCapsule (Capsule)) {
            	*Address = Capsule;
            	*Size = (UINT32)Hob->Length;
            	return EFI_SUCCESS;
            }
            Hob = GET_NEXT_HOB (Hob);
        }
        HobStart = Hob;
    } while (HobStart != NULL);
    
    return EFI_NOT_FOUND;
}

typedef struct _AMI_MEDIA_CAPSULE_PPI AMI_MEDIA_CAPSULE_PPI;

typedef
EFI_STATUS
(EFIAPI *AMI_LOAD_MEDIA_CAPSULE)(
  IN EFI_PEI_SERVICES       **PeiServices,
  IN AMI_MEDIA_CAPSULE_PPI  *This
);

struct _AMI_MEDIA_CAPSULE_PPI {
    AMI_LOAD_MEDIA_CAPSULE LoadMediaCapsule;
};

/**
 * @brief Loads capsule from file stored on disk
 *
 * @param PeiServices Pointer to pointer to PEI services
 * @param RecoveryHob Pointer to recovery HOB to store capsule info
 * @param Address Pointer to store pointer to capsule
 * @param Size Pointer to store capsule size
 *
 * @return Status of operation
 */
EFI_STATUS LoadMediaCapsule(
    IN EFI_PEI_SERVICES **PeiServices,
    IN OUT RECOVERY_IMAGE_HOB *RecoveryHob,
    OUT EFI_CAPSULE_HEADER **Address,
    OUT UINT32 *Size
)
{
    EFI_STATUS Status;
    EFI_PEI_DEVICE_RECOVERY_MODULE_PPI *rd;
    UINTN Instance = 0;
    EFI_GUID Type;
    UINTN CapsuleSize;
    VOID *CapsuleAddress;
    BOOLEAN Loaded = FALSE;
    
    do {
        Status = PeiServicesLocatePpi (&gEfiPeiDeviceRecoveryModulePpiGuid, 
                Instance++, NULL, (VOID **)&rd);
        if (EFI_ERROR (Status))
            break;

        Status = rd->GetRecoveryCapsuleInfo (PeiServices, rd, 0, &CapsuleSize, &Type);
        if (EFI_ERROR (Status) || !CompareGuid (&Type, &gBlockDeviceCapsuleGuid))
            continue;

        CapsuleAddress = AllocateRuntimePages (EFI_SIZE_TO_PAGES (CapsuleSize));
        if (CapsuleAddress == NULL)
            return EFI_OUT_OF_RESOURCES;

        Status = rd->LoadRecoveryCapsule (PeiServices, rd, 0, CapsuleAddress);
        if (!EFI_ERROR (Status)) { //capsule loaded, capsule info stored in AmiRecoveryHob
            RecoveryHob->Address = (UINTN)CapsuleAddress;
            RecoveryHob->ImageSize = (UINT32)CapsuleSize;
            *Address = (EFI_CAPSULE_HEADER *)CapsuleAddress;
            *Size = (UINT32)CapsuleSize;
            //store loaded capsule in capsule hob for third party access
            BuildCvHob (RecoveryHob->Address, RecoveryHob->ImageSize);
            Loaded = TRUE;
        }
    } while (!Loaded);
    
    return Status;
}


/**
  This function implements upper level capsule recovery device interface
  
  @param PeiServices Pointer to pointer to EFI_PEI_SERVICES structure
  @param RecoveryHob Pointer to RECOVERY_IMAGE_HOB structure

  @retval EFI_SUCCESS   Image verified and HOB created
  @retval EFI_NOT_FOUND Image not found
**/
EFI_STATUS ProcessUpdateCapsule(
    IN EFI_PEI_SERVICES **PeiServices,
    IN RECOVERY_IMAGE_HOB *RecoveryHob
)
{
    EFI_STATUS Status;
    EFI_CAPSULE_HEADER *Capsule;
    UINT32 CapsuleSize;
    VOID *Payload;
    UINT32 PayloadSize;
    
    DEBUG((-1, "Loading new firmware image..."));

    Status = FindFwCapsuleHob(&Capsule, &CapsuleSize);
    if (EFI_ERROR(Status))    //no cap in memory, try disk
        Status = LoadMediaCapsule (PeiServices, RecoveryHob, &Capsule, &CapsuleSize);
    
    if (EFI_ERROR (Status)) {
    	PEI_ERROR_CODE(PeiServices, PEI_RECOVERY_NO_CAPSULE, EFI_ERROR_MAJOR);
    	return Status;
    }
    // we have capsule, let's preprocess and verify it
    Status = AmiPreprocessCapsule (Capsule, CapsuleSize, &Payload, &PayloadSize);
    if (!EFI_ERROR (Status))
    	Status = VerifyFwImage((CONST EFI_PEI_SERVICES **)PeiServices, &Payload, 
                &PayloadSize, &RecoveryHob->FailedStage);
    
    RecoveryHob->Address = (EFI_PHYSICAL_ADDRESS)(UINTN)Payload;
    RecoveryHob->Status = (UINT8)Status;
    RecoveryHob->ImageSize = PayloadSize;
    
    if (EFI_ERROR(Status)) {
        PEI_ERROR_CODE((CONST EFI_PEI_SERVICES **)PeiServices, 
                PEI_RECOVERY_INVALID_CAPSULE, EFI_ERROR_MAJOR);
    } else {
    	PEI_PROGRESS_CODE((CONST EFI_PEI_SERVICES **)PeiServices,
                PEI_RECOVERY_CAPSULE_LOADED);
    }

    return Status;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017 American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
