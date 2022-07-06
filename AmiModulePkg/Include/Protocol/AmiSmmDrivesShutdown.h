//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file
  Header file that defines the AmiSmmDrivesShutdown protocol.
**/
#ifndef __AMI_SMM_DRIVES_SHUTDOWN_PROTOCOL__H__
#define __AMI_SMM_DRIVES_SHUTDOWN_PROTOCOL__H__

#define AMI_SMM_DRIVES_SHUTDOWN_PROTOCOL_GUID \
	{ 0xbfcda60a, 0xe8c7, 0x4160, { 0x80, 0x41, 0xf2, 0xce, 0x3d, 0x38, 0xad, 0x7d } }

/**
	Function to shutdown all the supported mass storage devices.

    @param    None

    @retval   EFI_STATUS
**/
typedef EFI_STATUS (EFIAPI *AMI_SMM_SHUTDOWN_ALL_DRIVES)();

/// AMI SMM Drives Shutdown Protocol.
typedef struct {
    AMI_SMM_SHUTDOWN_ALL_DRIVES ShutdownAllDrives;
} AMI_SMM_DRIVES_SHUTDOWN_PROTOCOL;

extern EFI_GUID gAmiSmmDrivesShutdownProtocolGuid;

#endif
