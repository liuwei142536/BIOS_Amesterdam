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
  @brief AMI BDS ConIn Started Protocol definition.

  The protocol has NULL interface. It is installed by BDS when all known to BDS
  console input devices have been connected.
**/

#ifndef __AMI_CON_IN_STARTED_PROTOCOL__H__
#define __AMI_CON_IN_STARTED_PROTOCOL__H__

/// Protocol GUID.
/// The protocol has NULL interface. It is installed by BDS when all known to BDS
/// console input devices have been connected.
#define AMI_CON_IN_STARTED_PROTOCOL_GUID \
    { 0x2df1e051, 0x906d, 0x4eff, { 0x86, 0x9d, 0x24, 0xe6, 0x53, 0x78, 0xfb, 0x9e } }


/// see ::AMI_CON_IN_STARTED_GUID
extern EFI_GUID gAmiConInStartedProtocolGuid;

#endif
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
