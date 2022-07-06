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

#ifndef __AMI_PAUSE_PROTOCOL__H__
#define __AMI_PAUSE_PROTOCOL__H__

#define AMI_PAUSE_PROTOCOL_GUID\
    { 0xc7a047a8, 0x82b1, 0x48db, { 0x8e, 0x6a, 0x7b, 0x27, 0xcc, 0x7c, 0x56, 0x1f } }

/**
  This function determines if the criteria has been met to pause the system and then
  it pauses the system and waits for the proper conditions to unpause the system.
**/
typedef VOID (EFIAPI *AMI_PAUSE_PROTOCOL_PAUSE) (VOID);

/**
  This protocol is called by BDS and TSE during their execution to allow the system to
  pause if required. This functionality is not available in shell, unless someone adds
  it to the Shell source.
**/
typedef struct _AMI_PAUSE_PROTOCOL {
    AMI_PAUSE_PROTOCOL_PAUSE Pause;
} AMI_PAUSE_PROTOCOL;

extern EFI_GUID gAmiPauseProtocolGuid;

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
