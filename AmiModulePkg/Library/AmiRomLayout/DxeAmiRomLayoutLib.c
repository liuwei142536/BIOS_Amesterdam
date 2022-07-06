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

/** @file
  DXE instance of AmiRomLayoutLib.
**/
#include <Guid/AmiRomLayout.h>
#include <Library/AmiRomLayoutLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>

extern UINT32 gAmiRomLayoutVersion;
extern UINT32 gAmiRomAreaSize;
extern UINT32 gAmiRomLayoutSize;
extern AMI_ROM_AREA *gAmiRomLayout;
extern AMI_ROM_AREA *gAmiRomLayoutEnd;


EFI_STATUS EFIAPI DxeAmiRomLayoutLibConstructor (IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    AMI_ROM_LAYOUT_HOB *AmiRomLayoutHob = NULL;
    AMI_ROM_LAYOUT_HOB_ENTRY *AmiRomLayoutHobEntry = NULL;
    AMI_ROM_AREA *RomArea = NULL;

    Status = EfiGetSystemConfigurationTable(&gAmiRomLayoutGuid, (VOID**)&AmiRomLayoutHob);
    if(EFI_ERROR(Status))
    {
        AmiRomLayoutHob = GetFirstGuidHob (&gAmiRomLayoutGuid);
    }

    if(AmiRomLayoutHob != NULL)
    {
        gAmiRomLayoutVersion = AmiRomLayoutHob->Version;
        gAmiRomAreaSize = AmiRomLayoutHob->DescriptorSize;

        // So this calculation is a little trickier. The HobLength from the HOB minus the AMI_ROM_LAYOUT_HOB size gives the total size
        //  consumed by the Hob Entries (In AMI_ROM_LAYOUT_HOB_ENTRY sizes). Divide that number by the size of a AMI_ROM_LAYOUT_HOB_ENTRY
        //  size (as specified in the HOB) and then multipy that by the Descriptor Size (again from the header) and that gives the
        //  total size required for the RomLayouts
        gAmiRomLayoutSize = (UINT32) MultU64x32( DivU64x32( AmiRomLayoutHob->Header.Header.HobLength - sizeof(AMI_ROM_LAYOUT_HOB), AmiRomLayoutHob->HobEntrySize ), AmiRomLayoutHob->DescriptorSize);

        gAmiRomLayout = AllocatePool(gAmiRomLayoutSize);
        if(gAmiRomLayout != NULL)
        {
            gAmiRomLayoutEnd = (AMI_ROM_AREA*)((UINT8*)gAmiRomLayout + gAmiRomLayoutSize);

            // Go through the HOB and copy out the Rom Layout Information into the gAmiRomArea entries
            for(AmiRomLayoutHobEntry = (AMI_ROM_LAYOUT_HOB_ENTRY*)(AmiRomLayoutHob+1), RomArea = gAmiRomLayout;
                AmiRomLayoutHobEntry < (AMI_ROM_LAYOUT_HOB_ENTRY*)((UINT8*)(AmiRomLayoutHob+1) + AmiRomLayoutHob->Header.Header.HobLength - sizeof(AMI_ROM_LAYOUT_HOB));
                AmiRomLayoutHobEntry = (AMI_ROM_LAYOUT_HOB_ENTRY*)((UINT8*)AmiRomLayoutHobEntry + AmiRomLayoutHob->HobEntrySize), RomArea = (AMI_ROM_AREA*)((UINT8*)RomArea + gAmiRomAreaSize))
            {
                gBS->CopyMem(RomArea, &(AmiRomLayoutHobEntry->Area), gAmiRomAreaSize);
            }

            if(EFI_ERROR(Status))
            {
                AMI_ROM_LAYOUT_HOB *ConfigurationTable;
                ConfigurationTable = AllocateRuntimeCopyPool(AmiRomLayoutHob->Header.Header.HobLength, AmiRomLayoutHob);
                if (ConfigurationTable!=NULL)
                    gBS->InstallConfigurationTable(&gAmiRomLayoutGuid, ConfigurationTable);
            }
        }
    }
    return EFI_SUCCESS;
}


/**
 * @param AreaGuid
 * @return
 */
EFI_STATUS AmiPublishFvArea(IN AMI_ROM_AREA *FvArea)
{
    EFI_STATUS Status = EFI_INVALID_PARAMETER;
    EFI_HANDLE FvHandle = NULL;

    if(FvArea != NULL)
    {
        if (FvArea->Address == AMI_ROM_AREA_NOT_MEMORY_MAPPED) return EFI_NO_MAPPING;
        Status = gDS->ProcessFirmwareVolume((VOID*)(UINTN)FvArea->Address, FvArea->Size, &FvHandle);
    }
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
