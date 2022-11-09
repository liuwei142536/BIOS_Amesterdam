//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/**@file NVLockLegacyBoot.c
    This file contains Lock and Unlock the Protected Variables while
    Legacy Boot From Setup and Legacy Boot To Setup. Locking the Protected
	Variables Before Legacy Boot and UnLocking the protected Variables
	After Legacy Boot.
    
 */
#include "AMISetupNVLock.h"
#include <NvLockElink.h>

SETUPNVLOCK_MAILBOX NvLockMailbox ;
EFI_GUID NVLockMailboxVariableGuid = SETUPNVLOCK_MAILBOX_ADDRESS_VARIABLE_GUID;
/**
     Lock the Protected Variables Before Legacy Boot from Setup.
     
     @param VOID
     
     @retval VOID
**/
VOID BeforeLegacyBoot()
{
    EFI_STATUS RetStatus;
    NvLockMailbox.NvLockState = 1; 
    RetStatus = pRS->SetVariable( L"NvLockMailbox", 
                                &NVLockMailboxVariableGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                sizeof(NvLockMailbox), 
                                &NvLockMailbox );
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nNVLockLegacyBoot: Access to Nvram Variables Locked Status: %r", RetStatus));
    return;
}

/**
     Unlock the Protected Variables After Legacy Boot to Setup.
     
     @param VOID
     
     @retval VOID
**/
VOID AfterLegacyBoot()
{
    EFI_STATUS RetStatus;
    NvLockMailbox.NvLockState = 0; 
    RetStatus = pRS->SetVariable( L"NvLockMailbox", 
                                &NVLockMailboxVariableGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS , 
                                sizeof(NvLockMailbox), 
                                &NvLockMailbox );
    DEBUG_SETUPNVLOCK((DEBUG_INFO, "\nNVLockLegacyBoot: Access to Nvram Variables UnLocked Status: %r", RetStatus));
}
