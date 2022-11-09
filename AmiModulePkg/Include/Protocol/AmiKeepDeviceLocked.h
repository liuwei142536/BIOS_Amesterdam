//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file
  AMI KeepDeviceLocked protocol definition.
  
  The protocol can be published on the device handle to disable device unlocking.
**/
#ifndef __AMI_KEEP_DEVICE_LOCKED_PROTOCOL_H__
#define __AMI_KEEP_DEVICE_LOCKED_PROTOCOL_H__

#define AMI_KEEP_DEVICE_LOCKED_PROTOCOL_GUID \
    { 0x73b1d339, 0xe4e8, 0x44e6, { 0x81, 0x51, 0xc2, 0xae, 0xcc, 0x9a, 0x15, 0x3b } }

extern EFI_GUID gAmiKeepDeviceLockedProtocolGuid;

#endif
