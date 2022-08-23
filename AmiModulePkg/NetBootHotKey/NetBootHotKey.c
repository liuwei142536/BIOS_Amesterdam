/** @file 
    This module allows invokes PXE Network Boot.
    
    
        (C)Copyright 1985-2017, American Megatrends, Inc. 
                       All Rights Reserved.               

**/


#include <EFI.h>
#include <Protocol/LegacyBios.h>
#include <Pci.h>
#include <AmiDxeLib.h>
#include <Library/DebugLib.h>


extern EFI_SYSTEM_TABLE *gST; /*!< external global pointer to EFI system table */

typedef EFI_STATUS (*PXE_BOOT_CHECK_DEVICE) (EFI_DEVICE_PATH_PROTOCOL* DevicePath); /*! < check what kind of PXE device */

/**
    This TSE function used to launch UEFI boot option. 
    @param  u16Option       boot option to launch
    @param  pOrder          optional pointer to option's order
    @param  u16OrderCount   boot order count
    @retval EFI_STATUS      Status code
**/
EFI_STATUS BootLaunchBootOption(UINT16 u16Option, UINT16 *pOrder, UINTN u16OrderCount);

/**
    This TSE function used to launch Legacy PXE boot option. 
    @param  DevicePath      pointer to PXE boot option
    @param  Options         pointer to options
    @param  OptionSize      size of the boot option
    @param  ValidBootOption boolean option to check the boot option
    @retval EFI_STATUS      Status code
**/
EFI_STATUS _BootLaunchDevicePath( EFI_DEVICE_PATH_PROTOCOL *DevicePath, VOID *Options, UINTN OptionSize, BOOLEAN ValidBootOption );


/** 
    This function checks if device path belong to PXE boot option. 
    @param  DevicePath           pointer to device path.
    @retval EFI_SUCCESS          Belong to uefi network boot option
    @retval EFI_UNSUPPORTED      Do not belong to
**/ 
EFI_STATUS IsNetworkDevice( IN EFI_DEVICE_PATH_PROTOCOL *DevicePath )
{
    EFI_DEVICE_PATH_PROTOCOL *Dp;

    for ( Dp=DevicePath; !(isEndNode(Dp)); Dp=NEXT_NODE(Dp) ) 
        if ( Dp->Type == MESSAGING_DEVICE_PATH && Dp->SubType == MSG_MAC_ADDR_DP )
            return EFI_SUCCESS;

    return EFI_UNSUPPORTED;    
}

/** 
    This function checks if device path belong to legacy network boot option. 
    @param  DevicePath           pointer to device path.
    @retval EFI_SUCCESS          Belong to legacy network boot option
    @retval EFI_UNSUPPORTED      Do not belong to
**/ 
EFI_STATUS IsLegacyNetworkDevice( IN EFI_DEVICE_PATH_PROTOCOL *DevicePath )
{
    EFI_DEVICE_PATH_PROTOCOL *Dp;

    for ( Dp=DevicePath; !(isEndNode(Dp)); Dp=NEXT_NODE(Dp) ) 
        if ( Dp->Type == BBS_DEVICE_PATH && Dp->SubType == BBS_BBS_DP)
        {
            BBS_BBS_DEVICE_PATH* BbsDp = (BBS_BBS_DEVICE_PATH*)Dp;
            if(BbsDp->DeviceType == BBS_EMBED_NETWORK
                    || BbsDp->DeviceType == BBS_BEV_DEVICE)
            {
                return EFI_SUCCESS;
            }
        }

    return EFI_UNSUPPORTED;    
}

/** 
    This function attempts for an PXE boot operation. 
    @param  Check     Check whether target boot device
**/ 
VOID PxeBoot(PXE_BOOT_CHECK_DEVICE Check)
{
    EFI_STATUS Status;
    EFI_GUID EfiVarGuid = EFI_GLOBAL_VARIABLE;
    UINT16 *BootOrder = NULL;
    UINTN  BootOrderSize = 0;
    UINT16 Index;
    UINTN  Size;
    EFI_LOAD_OPTION *BootOption = NULL;
    CHAR16 Name[10] = {0};
    CHAR16 *DescriptionStr = NULL;
    EFI_DEVICE_PATH_PROTOCOL *DevicePathNode = NULL;

    Status = GetEfiVariable( L"BootOrder", &EfiVarGuid, NULL, &BootOrderSize, &BootOrder );
    if (EFI_ERROR(Status))
        return;

    for (Index = 0; Index < BootOrderSize / sizeof(UINT16); Index++) {

        // Get next boot item variable
        Swprintf( Name, L"Boot%04X", BootOrder[Index] );
        Size = 0;
        if (BootOption != NULL) {
            pBS->FreePool(BootOption);
            BootOption = NULL;
        }
        Status = GetEfiVariable( Name, &EfiVarGuid, NULL, &Size, &BootOption );
        if (EFI_ERROR(Status))
            continue;

        if ( !(BootOption->Attributes & LOAD_OPTION_ACTIVE) ) // skip disabled item
            continue;

        // Get device path pointer
        DescriptionStr = (CHAR16*)(BootOption + 1);
        DevicePathNode = (EFI_DEVICE_PATH_PROTOCOL*)( DescriptionStr + Wcslen(DescriptionStr) + 1 );

        // Check device path
        Status = Check(DevicePathNode);
        if (!EFI_ERROR(Status)) {

            // Call TSE function to launch the EFI PXE boot option 
            DEBUG((DEBUG_INFO,"Launch the PXE boot option %S (%S)...\n",Name,DescriptionStr));
            Status = BootLaunchBootOption( BootOrder[Index], NULL, 0 );
            DEBUG((DEBUG_INFO,"...%S Status=%r\n",Name,Status));
        }
    }
    pBS->FreePool(BootOption);
    pBS->FreePool(BootOrder);
}


/** 
    This function attempts for an EFI PXE boot operation. 
**/ 
VOID EfiPxeBoot(VOID)
{
    EFI_STATUS Status;
    EFI_GUID EfiVarGuid = EFI_GLOBAL_VARIABLE;
    UINT16 *BootOrder = NULL;
    UINTN  BootOrderSize = 0;
    UINT16 Index;
    UINTN  Size;
    EFI_LOAD_OPTION *BootOption = NULL;
    CHAR16 Name[10] = {0};
    CHAR16 *DescriptionStr = NULL;
    EFI_DEVICE_PATH_PROTOCOL *DevicePathNode = NULL;

    Status = GetEfiVariable( L"BootOrder", &EfiVarGuid, NULL, &BootOrderSize, &BootOrder );
    if (EFI_ERROR(Status))
        return;

    for (Index = 0; Index < BootOrderSize / sizeof(UINT16); Index++) {

        // Get next boot item variable
        Swprintf( Name, L"Boot%04X", BootOrder[Index] );
        Size = 0;
        if (BootOption != NULL) {
            pBS->FreePool(BootOption);
            BootOption = NULL;
        }
        Status = GetEfiVariable( Name, &EfiVarGuid, NULL, &Size, &BootOption );
        if (EFI_ERROR(Status))
            continue;

        if ( !(BootOption->Attributes & LOAD_OPTION_ACTIVE) ) // skip disabled item
            continue;

        // Get device path pointer
        DescriptionStr = (CHAR16*)(BootOption + 1);
        DevicePathNode = (EFI_DEVICE_PATH_PROTOCOL*)( DescriptionStr + Wcslen(DescriptionStr) + 1 );

        // Check device path
        Status = IsNetworkDevice(DevicePathNode);
        if (!EFI_ERROR(Status)) {

            // Call TSE function to launch the EFI PXE boot option 
            DEBUG((DEBUG_INFO,"Launch the EFI PXE boot option %S (%S)...\n",Name,DescriptionStr));
            Status = BootLaunchBootOption( BootOrder[Index], NULL, 0 );
            DEBUG((DEBUG_INFO,"...%S Status=%r\n",Name,Status));
        }
    }
    pBS->FreePool(BootOption);
    pBS->FreePool(BootOrder);
}


/** 
    This function attempts for a Legacy PXE boot operation. 
    @retval EFI_NOT_FOUND      Find no Legacy PXE device
    @retval EFI_STATUS         Return from
                                   1. Locate LegacyBiosProtocol
                                   2. GetBbsInfo method of LegacyBiosProtocol
                                   3. _BootLaunchDevicePath call
**/ 
EFI_STATUS LegacyPxeBoot(VOID)
{
    EFI_STATUS Status;
    EFI_GUID EfiLegacyBiosProtocolGuid = EFI_LEGACY_BIOS_PROTOCOL_GUID;
    EFI_LEGACY_BIOS_PROTOCOL *LegacyBios = NULL;
    HDD_INFO *HddInfo = NULL;
    BBS_TABLE *BbsTable = NULL;
    UINT16 HddCount = 0;
    UINT16 BbsCount = 0;
    UINT16 NetBootPriority = 0;
    static struct {
        BBS_BBS_DEVICE_PATH bbs;
        EFI_DEVICE_PATH_PROTOCOL end;
    } LegacyDp = {
        { {BBS_DEVICE_PATH, BBS_BBS_DP, sizeof(BBS_BBS_DEVICE_PATH)}, BBS_EMBED_NETWORK, 0, 0 },
        {END_DEVICE_PATH, END_ENTIRE_SUBTYPE, sizeof(EFI_DEVICE_PATH_PROTOCOL)} 
    };
    UINT16 Index;

    // Locate legacy BIOS protocol
    Status = gST->BootServices->LocateProtocol( 
        &EfiLegacyBiosProtocolGuid, 
        NULL, 
        &LegacyBios 
    );
    if (EFI_ERROR(Status))
        return Status;

    // Get BBS information and structures
    Status = LegacyBios->GetBbsInfo( 
        LegacyBios, 
        &HddCount, 
        &HddInfo, 
        &BbsCount, 
        &BbsTable 
    );
    if (EFI_ERROR(Status))
        return Status;

    // Check is there any Network device in the BBS table

    for ( Index = 0; Index < BbsCount; Index++ ) {

        // skip items 
        if ( BbsTable[Index].BootPriority == BBS_IGNORE_ENTRY || 
             BbsTable[Index].BootPriority == BBS_DO_NOT_BOOT_FROM ) 
            continue;
    
        // check if it's search for a Network device
        if (BbsTable[Index].Class == PCI_CL_NETWORK)
        	BbsTable[Index].BootPriority = NetBootPriority++;
    }
    if (NetBootPriority == 0)
        return EFI_NOT_FOUND;
    for ( Index = 0; Index < BbsCount; Index++ ) {

        // skip items 
        if ( BbsTable[Index].BootPriority == BBS_IGNORE_ENTRY || 
             BbsTable[Index].BootPriority == BBS_DO_NOT_BOOT_FROM || 
             BbsTable[Index].Class == PCI_CL_NETWORK) 
            continue;
        BbsTable[Index].BootPriority = NetBootPriority++;
    }

    // Call TSE function to launch the Net Boot operation 
    DEBUG((DEBUG_INFO,"Launch the Legacy PXE Boot...\n"));
    Status = _BootLaunchDevicePath(
        (EFI_DEVICE_PATH_PROTOCOL*)&LegacyDp,
        NULL,
        0,
        TRUE
    );

    return Status;
}

PXE_BOOT_CHECK_DEVICE gPxeBootCheckDevice[] = {IsNetworkDevice, IsLegacyNetworkDevice, NULL, NULL}; /*!< check what kind of PXE device */
/**
    This function is a hook called when user activates
    configurable post hot key 1. This function is
    available as ELINK. Generic implementation is empty.
    OEMs may choose to use different logic here.
    @retval EFI_SUCCESS      No error
**/
EFI_STATUS NetBootHotKey(VOID)
{
    UINT8 i;

    DEBUG((DEBUG_INFO,"NetBootHotKey hook has been activated\n"));

    for(i = 0; gPxeBootCheckDevice[i]; i++)
        PxeBoot(gPxeBootCheckDevice[i]);
		
    return EFI_SUCCESS;
}


//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
