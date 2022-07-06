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
  Reset Request Protocol

  The protocol provides API to register system reset request.
  The protocol can be used to reduce number of system resets
  by combining multiple reset request into a single system reset.
**/

#ifndef __AMI_RESET_REQUEST_PROTOCOL__H__
#define __AMI_RESET_REQUEST_PROTOCOL__H__

#define AMI_RESET_REQUEST_PROTOCOL_GUID\
    { 0x83ca92cc, 0xe4c7, 0x4b2f, { 0xb6, 0x45, 0xf3, 0x70, 0xd7, 0xe5, 0x27, 0x41 } }

#define AMI_RESET_REQUEST_PROTOCOL_REVISION 1

typedef struct _AMI_RESET_REQUEST_PROTOCOL AMI_RESET_REQUEST_PROTOCOL;

/**
  Schedules system reset.

  The function registers system reset request.
  The actual system reset is performed at the end of the boot process
  before launching OS.

  @param[in]  This      A pointer to the AMI_RESET_REQUEST_PROTOCOL instance.

  @retval EFI_SUCCESS   Reset request has been registered.
**/
typedef EFI_STATUS (EFIAPI *AMI_RESET_REQUEST)(
    IN AMI_RESET_REQUEST_PROTOCOL *This
);

struct _AMI_RESET_REQUEST_PROTOCOL {
    UINT64 Revision;
    AMI_RESET_REQUEST  RequestSystemReset;
};

extern EFI_GUID gAmiResetRequestProtocolGuid;

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
