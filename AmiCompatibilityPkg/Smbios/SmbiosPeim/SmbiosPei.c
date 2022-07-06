//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2022, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file SmbiosPei.c
    This file provides function to update "Wake-up Type"
    data field in Smbios Type 1 structure

**/

#include <AmiPeiLib.h>
#include <Protocol/AmiSmbios.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/DebugLib.h>
//APTIOV_OVERRIDE_START: Address SA50090 smbios vulnerability
// EIP#674389
#include <Ppi/SmmCommunication.h>
#include <Library/PeiServicesLib.h>
#include <Library/LockBoxLib.h>
// APTIOV_OVERRIDE_END

//extern VOID		OemRuntimeShadowRamWrite(IN BOOLEAN Enable);
extern UINT8	getWakeupTypeForSmbios(VOID);
extern VOID     NbRuntimeShadowRamWrite(IN BOOLEAN Enable);

//APTIOV_OVERRIDE_START: Address SA50090 smbios vulnerability
// EIP#674389
EFI_STATUS
EFIAPI
NotifyOnSmmCommunication (
    IN  EFI_PEI_SERVICES                **PeiServices,
    IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
    IN  VOID                            *InvokePpi
);

static
EFI_PEI_NOTIFY_DESCRIPTOR NotifyList[] = {
    {
        EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,    
        &gEfiPeiSmmCommunicationPpiGuid,
        NotifyOnSmmCommunication
    }
};

EFI_GUID    gSmbiosWakeupTypeUpdateGuid = SMBIOS_WAKEUPTYPE_GUID;
// APTIOV_OVERRIDE_END

/**
    Driver entry point for SmbiosPei
    
    Detect and update SMBIOS Type 1 structure "Wake-up Type"
    data field

    @param PeiServices

    @retval VOID Updated SMBIOS Type 1 "Wake-up Type"
    @param FfsHeader
    @param PeiServices

    @retval EFI_STATUS Status

**/
EFI_STATUS
EFIAPI
SmbiosPeiEntryPoint(
    IN       EFI_PEI_FILE_HANDLE    FileHandle,
  	IN CONST EFI_PEI_SERVICES       **PeiServices
)
{
    EFI_STATUS      Status;
    EFI_BOOT_MODE   BootMode;

    // Determine boot mode
    Status = (*PeiServices)->GetBootMode(PeiServices, &BootMode);
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "Failed to get BootMode\n"));
        //APTIOV_OVERRIDE_START: Address SA50090 smbios vulnerability
        // EIP#674389
        return Status;
    }

    if (BootMode == BOOT_ON_S3_RESUME) {         
        Status = (**PeiServices).NotifyPpi(
                                    (const EFI_PEI_SERVICES **)PeiServices, 
                                    (const EFI_PEI_NOTIFY_DESCRIPTOR *)NotifyList);
        if (EFI_ERROR(Status)) {
            return Status;
        }
        // APTIOV_OVERRIDE_END
    }

	return EFI_SUCCESS;
}

//APTIOV_OVERRIDE_START: Address SA50090 smbios vulnerability
// EIP#674389
/**
    Returns the length of the structure pointed by BufferStart
    in bytes

    @param BufferStart  pointer to buffer

    @return Structure size

**/
UINT16
GetStructureLength(
    IN UINT8    *BufferStart
)
{
    UINT8       *BufferEnd = BufferStart;

    BufferEnd += ((SMBIOS_STRUCTURE_HEADER *)BufferStart)->Length;

    while (ReadUnaligned16((UINT16 *)BufferEnd) != 0) {
        BufferEnd++;
    }

    return (UINT16)((BufferEnd + 2) - BufferStart);   // +2 for double zero terminator
}

/**
    Find structure type1 starting from memory location pointed by
    Buffer

    @param Buffer
    @param StructureFoundPtr


    @retval TRUE Structure found
    @retval FALSE Structure not found

    @remark
        @li If Type1 is found:\n
            UINT8   **StructureFoundPtr - Points to the structure that was found\n

        @li If Type1 is not found:\n
            UINT8   **StructureFoundPtr = NULL

**/
BOOLEAN
FindStructureType1(
    IN     UINT8    *Buffer,
    IN OUT UINT8    **StructureFoundPtr
)
{
    UINT8       *BufferPtr = Buffer;
    BOOLEAN     FindStatus = FALSE;

    *StructureFoundPtr = NULL;
    while (((SMBIOS_STRUCTURE_HEADER *)BufferPtr)->Type != SMBIOS_TYPE_END_OF_TABLE) {
        if (((SMBIOS_STRUCTURE_HEADER *)BufferPtr)->Type == SMBIOS_TYPE_SYSTEM_INFORMATION) {
            // set the find status flag and update the Buffer pointer
            FindStatus = TRUE;
            *StructureFoundPtr = BufferPtr;
            break;
        }
        BufferPtr += GetStructureLength(BufferPtr);
    }
    return FindStatus;
}

/**
   @internal
   
    This routine is called when gEfiPeiSmmCommunicationPpiGuid
    is available.

    @param PeiServices      Pointer to PEI Services
    @param NotifyDesc       Pointer to notify descriptor
    @param InvokePpi        Pointer to Invoke PPI.

    @retval EFI_SUCCESS
    
   @endinternal
**/

EFI_STATUS
EFIAPI
NotifyOnSmmCommunication (
    IN  EFI_PEI_SERVICES                **PeiServices,
    IN  EFI_PEI_NOTIFY_DESCRIPTOR       *NotifyDesc,
    IN  VOID                            *InvokePpi
)
{
    EFI_STATUS      Status;
    UINT8           *WakeupTypePtr;
    UINT64          SmbiosTablePtr;
    UINTN           SmbiosTablePtrSize;
    
    // First get LockBox data size
    SmbiosTablePtrSize = sizeof (SmbiosTablePtr);
	//Get smbios table start address using Restore Lockbox
    Status = RestoreLockBox (
                        &gSmbiosWakeupTypeUpdateGuid, 
                        &SmbiosTablePtr, 
                        &SmbiosTablePtrSize); 
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "**SmbiosPei**RestoreLockBox:%r\n",Status));
        return EFI_SUCCESS;
    }
    
    //Find smbios Type1 to update WakeUpType pointer
    if (FindStructureType1 ((UINT8 *)&SmbiosTablePtr, &WakeupTypePtr)) {
        WakeupTypePtr += OFFSET_OF (SMBIOS_TABLE_TYPE1, WakeUpType);
    } else {
        DEBUG((DEBUG_INFO, "**SmbiosPei : FindStructureType1Failed***\n"));
        return EFI_NOT_FOUND;
    }
    
    if ((UINTN)WakeupTypePtr > 0xF0000) {
        *WakeupTypePtr = getWakeupTypeForSmbios();
    } else {
        //OemRuntimeShadowRamWrite(TRUE);
        NbRuntimeShadowRamWrite(TRUE);
        *WakeupTypePtr = getWakeupTypeForSmbios();
        //OemRuntimeShadowRamWrite(FALSE);
        NbRuntimeShadowRamWrite(FALSE);
    }

    return EFI_SUCCESS;
}
// APTIOV_OVERRIDE_END
