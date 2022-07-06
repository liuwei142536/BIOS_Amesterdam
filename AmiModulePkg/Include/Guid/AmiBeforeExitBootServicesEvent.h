//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
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
  @brief Before-exit-boot-services event group GUID definition.
**/

#ifndef __AMI_BEFORE_EXIT_BOOT_SERVICES_EVENT_GUID__H__
#define __AMI_BEFORE_EXIT_BOOT_SERVICES_EVENT_GUID__H__

/// Event group GUID.
/// This event is signaled by the ExitBootServices() function prior to signaling
/// exit-boot-services event and prior to disabling system timer.
/// This event presents the last opportunity to use services that are 
/// going to be shut down by the exit-boot-services callback.
#define AMI_BEFORE_EXIT_BOOT_SERVICES_EVENT_GUID \
    { 0x8be0e274, 0x3970, 0x4b44, { 0x80, 0xc5, 0x1a, 0xb9, 0x50, 0x2f, 0x3b, 0xfc } }

/// see ::AMI_BEFORE_EXIT_BOOT_SERVICES_EVENT_GUID
extern EFI_GUID gAmiBeforeExitBootServicesEventGuid;

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
