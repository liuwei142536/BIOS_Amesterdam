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
  @brief AMI BDS ConOut Started Protocol definition.

  The protocol has NULL interface. It is installed by BDS when all known to BDS
  console output devices have been connected.
**/

#ifndef __AMI_CON_OUT_STARTED_PROTOCOL__H__
#define __AMI_CON_OUT_STARTED_PROTOCOL__H__

/// Protocol GUID.
/// The protocol has NULL interface. It is installed by BDS when all known to BDS
/// console output devices have been connected.
#define AMI_CON_OUT_STARTED_PROTOCOL_GUID \
    { 0xef9a3971, 0xc1a0, 0x4a93, { 0xbd, 0x40, 0x5a, 0xa1, 0x65, 0xf2, 0xdc, 0x3a } }


/// see ::AMI_CON_OUT_STARTED_GUID
extern EFI_GUID gAmiConOutStartedProtocolGuid;

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
