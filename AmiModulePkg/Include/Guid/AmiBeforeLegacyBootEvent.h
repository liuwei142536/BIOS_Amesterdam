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
  @brief Before-legacy-boot event group GUID definition.
**/

#ifndef __AMI_BEFORE_LEGACY_BOOT_EVENT_GUID__H__
#define __AMI_BEFORE_LEGACY_BOOT_EVENT_GUID__H__

/// Event group GUID.
/// This event is signaled by CSM prior to signaling PI legacy boot event.
/// This event presents the last opportunity to use services that are 
/// going to be shut down by the legacy boot callback.
#define AMI_BEFORE_LEGACY_BOOT_EVENT_GUID \
    { 0xacbd2563, 0x725d, 0x4bd8, { 0xb4, 0xb6, 0x4e, 0x55, 0x71, 0xa3, 0x6b, 0x50 } }

/// see ::AMI_BEFORE_LEGACY_BOOT_EVENT_GUID
extern EFI_GUID gAmiBeforeLegacyBootEventGuid;

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
