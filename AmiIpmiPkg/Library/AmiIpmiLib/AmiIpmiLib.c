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

/** @file AmiIpmiLib.c
    Library function definitions for AmiIpmiLib

**/

//---------------------------------------------------------------------------
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Include/IpmiNetFnAppDefinitions.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/AmiIpmiLib.h>

// Channel Medium type of 4 denotes 802.3 LAN 
#define LAN_CHANNEL_MEDIUM_TYPE    0x04
#define MAX_CHANNEL_NUM            0x0B  //Maximum channel number (implementation specific) that can be assigned to LAN 

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
  )
{
    EFI_STATUS                       Status;
    IPMI_GET_CHANNEL_INFO_RESPONSE   GetChannelInfoResponse;
    UINT8                            ResponseDataSize;
    UINT8                            *PcdLanChannelBufferPtr;
    UINT8                            PcdSize;
    UINT8                            ChannelNumber = 0 ;
    UINT8                            Index = 0;
    
    if (!IpmiTransport)
        return EFI_INVALID_PARAMETER;
    
    if (!ChannelBuffer || !ChannelCount || !*ChannelCount)
        return EFI_INVALID_PARAMETER;
                
    //Get PcdLanChannelPtr and size
    PcdLanChannelBufferPtr = PcdGetPtr(PcdLanChannelPtr);
    PcdSize = (UINT8)PcdGetSize(PcdLanChannelPtr);
    
    if (!PcdLanChannelBufferPtr || !PcdSize){
        return EFI_NOT_FOUND;
    }
    
    //Update Pcd if available channel number is not updated for this platform.
    //Pcd buffer should have at least one valid entry. Channel number 0 for Primary IPMB
    if (!PcdLanChannelBufferPtr[0]) {
    
        // Sends the get channel info command for all available BMC channel numbers
        for (ChannelNumber = 1; ChannelNumber <= MAX_CHANNEL_NUM; ChannelNumber++) {
            
            ResponseDataSize = sizeof (GetChannelInfoResponse);
            
            Status = IpmiTransport->SendIpmiCommand (IpmiTransport,
                                                      IPMI_NETFN_APP,
                                                      BMC_LUN,
                                                      IPMI_APP_GET_CHANNEL_INFO,
                                                      &ChannelNumber,
                                                      sizeof (ChannelNumber),
                                                      (UINT8 *)&GetChannelInfoResponse,
                                                      &ResponseDataSize);
            
            DEBUG ((DEBUG_INFO, " %a:%r ChannelNo: %d, CompletionCode: %x\n", __FUNCTION__, Status, GetChannelInfoResponse.ChannelNo, IpmiTransport->CommandCompletionCode));
            
            if (EFI_ERROR(Status))
                continue;
            
            // check for completion code and channel medium type is 802.3 LAN Ethernet.
            if (!IpmiTransport->CommandCompletionCode && (GetChannelInfoResponse.ChannelMediumType == LAN_CHANNEL_MEDIUM_TYPE)) {
                
                DEBUG ((DEBUG_INFO, "%a: Channel Medium Type: %x\n", __FUNCTION__, GetChannelInfoResponse.ChannelMediumType));

                //If maximum LAN count is not reached, store the LAN channel number into buffer
                if (Index < PcdSize) {
                    PcdLanChannelBufferPtr[Index] = ChannelNumber;
                    Index ++;
                }
            }
        }
    }
    
    if (!Index && (ChannelNumber > MAX_CHANNEL_NUM)){
        return EFI_NOT_FOUND;
    }
    //If request buffer size is greater than Pcd Buffer size, update with actual buffer size.
    //If requested buffer size is lesser than Pcd Buffer size, copy only requested buffer size.
    if (*ChannelCount > PcdSize)
        *ChannelCount = PcdSize;
    
    CopyMem (ChannelBuffer, PcdLanChannelBufferPtr, *ChannelCount);
    return EFI_SUCCESS;
}

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
