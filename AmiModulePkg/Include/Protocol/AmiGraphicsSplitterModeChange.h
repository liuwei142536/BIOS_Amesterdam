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
  @brief AMI Graphics Splitter Mode Change Protocol definition.

  The protocol has NULL interface. It is installed by Graphics Splitter whenever output mode changes.
**/

#ifndef __AMI_GRAPHICS_SPLITTER_MODE_CHANGE_PROTOCOL__H__
#define __AMI_GRAPHICS_SPLITTER_MODE_CHANGE_PROTOCOL__H__

/// Protocol GUID.
/// The protocol has NULL interface. It is installed by Graphics Splitter whenever output mode changes.
#define AMI_GRAPHICS_SPLITTER_MODE_CHANGE_PROTOCOL_GUID \
    { 0xc9a2603a, 0xd5d6, 0x4b64, { 0x83, 0x2b, 0x1a, 0xc, 0xb2, 0x41, 0x96, 0x69 } }


/// see ::AMI_GRAPHICS_SPLITTER_MODE_CHANGE_PROTOCOL_GUID
extern EFI_GUID gAmiGraphicsSplitterModeChangeProtocolGuid;

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
