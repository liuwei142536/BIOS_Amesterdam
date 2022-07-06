//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/** @AmiNetworkLib.h
  Header file that defines AmiNetwork library class, which provides Common LIB 
  Functions for the Network devices.
**/

#ifndef __AMI_NETWORK_COMMON_LIB_H__
#define __AMI_NETWORK_COMMON_LIB_H__

/**
    Blocked Network device details that won't be used for both PXE and ISCSI features 
**/
typedef struct {
    UINT16      VendorId;
    UINT16      ProductId;
} AMI_NETWORK_BLOCKED;


/**
    Returns whether network device on the USB Controller to be supported for 
    different UEFI Network Feature 
    
    For each features like PXE, ISCSI etc the Blocked device list also passed to the function. 
    Based on the Input Network Controller, the vendor Id and device Id checked for the blocked 
    list. If it's present in the blocked list it's returns TRUE so that UEFI feature BUS driver
    can ignore the network device to be used for the feature. 
    
    @param [in]  ControllerHandle       Network Controller Handle 
    @param [In]  VOID                   Pointer to the Blocked device list structure 
    
    @retval TRUE                        Device is Blocked 
    @retval FALSE                       Device can be used for the UEFI network features 
**/
BOOLEAN
IsAmiNetworkDeviceBlocked (
    IN EFI_HANDLE                   ControllerHandle,
    IN AMI_NETWORK_BLOCKED          *BlockedDeviceList
);

/**
    Return the UefiNetwork Stack Driver start status based on the Setup option. 

    @param  NULL 

    @return 
    TRUE - Start the Network Stack drivers 
    FALSE- Don't start Uefi Network Drivers 

**/
BOOLEAN 
CheckNetworkDriverLoadStatus (
);

/**
    Get the Pxe Boot Setup option. If the Setup option variable not found
    Load the default value from the SDL token.

    @param  *Ipv4PxeBoot    Setup option value for Ipv4 Pxe Boot
    @param  *Ipv6PxeBoot    Setup option value for Ipv6 Pxe Boot
    @param  *PxeBootWaitTime    Timeout for the PxeBoot
    @param  *MediaDetectCount   Retry count for the Media Detect

    @return 
    EFI_SUCCESS 

**/
EFI_STATUS 
GetPxeBootSetupOptions (
    UINT8   *Ipv4PxeBoot,
    UINT8   *Ipv6PxeBoot,
    UINT8   *PxeBootWaitTime,
    UINT8   *MediaDetectCount
    
);

/**
    Get the Http Boot Setup option. If the Setup option variable not found
    Load the default value from the SDL token.

    @param  *Ipv4HttpBoot    Setup option value for Ipv4 Http Boot
    @param  *Ipv6HttpBoot    Setup option value for Ipv6 Http Boot

    @return 
    EFI_SUCCESS 

**/
EFI_STATUS 
GetHttpBootSetupOptions (
    UINT8   *Ipv4HttpBoot,
    UINT8   *Ipv6HttpBoot,
    UINT8   *MediaDetectCount
);

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
