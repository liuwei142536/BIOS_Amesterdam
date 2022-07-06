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

/** @file AmiIpmiLib.h
    AmiIpmiLib definitions

**/

#ifndef _AMI_IPMI_LIB_H_
#define _AMI_IPMI_LIB_H_

#include <Protocol/IpmiTransportProtocol.h>

/**
    Gets the list of channel number for all available BMC LAN channels. 
    Lan channels will be updated to PCD PcdLanChannelPtr.
     
    @param   IpmiTransport        Pointer to Ipmi transport protocol
    @param   ChannelBuffer        Buffer to copy available channel number
    @param   ChannelCount         Size ChannelBuffer on input and channel count on output

    @return  Status : EFI_NOT_FOUND
                      EFI_SUCCESS
**/
EFI_STATUS
GetLanChannelNumber (
  IN     EFI_IPMI_TRANSPORT  *IpmiTransport,
  IN OUT UINT8               *ChannelBuffer,
  IN OUT UINT8               *ChannelCount 
);

#endif

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
