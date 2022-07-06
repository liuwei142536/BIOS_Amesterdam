//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
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
  @brief AMI BDS All Drivers Connected Protocol definition.

  The protocol has NULL interface. It is installed by BDS when all the drivers have been connected.
**/

#ifndef __AMI_BDS_ALL_DRIVERS_CONNECTED_PROTOCOL__H__
#define __AMI_BDS_ALL_DRIVERS_CONNECTED_PROTOCOL__H__

/// Protocol GUID.
/// The protocol has NULL interface. It is installed by BDS when all the drivers have been connected.
#define AMI_BDS_ALL_DRIVERS_CONNECTED_PROTOCOL_GUID \
    { 0xdbc9fd21, 0xfad8, 0x45b0, { 0x9e, 0x78, 0x27, 0x15, 0x88, 0x67, 0xcc, 0x93 } }

/// see ::AMI_BDS_ALL_DRIVERS_CONNECTED_PROTOCOL_GUID
extern EFI_GUID gAmiBdsAllDriversConnectedProtocolGuid;

#endif
//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
