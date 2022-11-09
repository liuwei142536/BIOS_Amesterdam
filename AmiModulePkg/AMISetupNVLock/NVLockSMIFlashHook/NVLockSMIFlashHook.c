//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file NVLockSMIFlashHook.c
    This file contains code that defines SmiFlash Hook Functions

**/

#include "AmiDxeLib.h"
#include <Library/BaseMemoryLib.h>

#define BUFFER_SIZE 256

//Support to allow Variable update through AFU
typedef EFI_STATUS (*TOGGLE_SMI_FLASH_FLAG)(BOOLEAN Start);
typedef struct {
    TOGGLE_SMI_FLASH_FLAG ToggleSmiFlashUpdateFlag;
} AMI_SETUPNV_CONTROL_PROTOCOL;

static AMI_SETUPNV_CONTROL_PROTOCOL *gNvramLockControl;

/**
    This Procedure locates SetupNV Control Protocol.

    @param NONE

    @retval Pointer to the configuration table

**/
AMI_SETUPNV_CONTROL_PROTOCOL 
*LocateSetupNvControlProtocol(VOID)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINT32      Attributes = 0;
    UINTN       VarSize = BUFFER_SIZE;
    EFI_GUID    gAmiSetupNvControlProtocolGuid = \
                    { 0x1bd378d6, 0xd2d6, 0x417f, { 0x87, 0xf6, 0x9e, 0x52, 0xf2, 0x8d, 0xcb, 0x2f } };

    if (NULL == (gNvramLockControl = GetSmstConfigurationTable(&gAmiSetupNvControlProtocolGuid)) ) {
        //If there isn't any SetVariable call from SMM, then the protocol will not be installed.
        //And calling GetSmstConfigurationTable will fail.
        //So Below code is to install the NvControl Protocol if the protocol is not installed before.
        UINT16 Language[BUFFER_SIZE];
        SetMem(Language, BUFFER_SIZE,0 );
        Status = pRS->GetVariable(L"PlatformLang", 
                                  &gEfiGlobalVariableGuid, 
                                  &Attributes, 
                                  &VarSize, 
                                  Language );
        if (Status == EFI_SUCCESS) {
            Status = pRS->SetVariable( L"PlatformLang", 
                                       &gEfiGlobalVariableGuid, 
                                       Attributes , 
                                       VarSize, 
                                       Language );
            DEBUG((DEBUG_ERROR, "\nAmiSetupNvLock: NvramLockControl installed by forcing SetVariable Status=:%r!!", Status));
            gNvramLockControl = GetSmstConfigurationTable(&gAmiSetupNvControlProtocolGuid);
        }
    }
    return gNvramLockControl;
}

/**
    Hook function gets control when update through SmiFlash is started.

    @param NONE

    @retval NONE

**/
VOID 
SmiUpdateStart(VOID)
{
    // Set SmiFlashUpdate flag TRUE in SetupNVLock.
    gNvramLockControl = LocateSetupNvControlProtocol();
    if (gNvramLockControl) {
        DEBUG((DEBUG_INFO, "\nAmiSetupNvLock: Unlock the Variable Access!!"));
        gNvramLockControl->ToggleSmiFlashUpdateFlag(FALSE);
    }
    return;
}

/**
    Hook function gets control when update through SmiFlash is completed.

    @param NONE

    @retval NONE

**/
VOID 
SmiUpdateEnd(VOID)
{
    // Set SmiFlashUpdate flag FALSE in SetupNVLock.
    gNvramLockControl = LocateSetupNvControlProtocol();
    if (gNvramLockControl) {
        DEBUG((DEBUG_INFO, "\nAmiSetupNvLock: Lock the Variable Access !!"));
        gNvramLockControl->ToggleSmiFlashUpdateFlag(TRUE);
    }
    return;
}
