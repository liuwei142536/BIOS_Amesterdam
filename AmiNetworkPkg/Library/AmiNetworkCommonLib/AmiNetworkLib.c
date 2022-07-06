//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiNetworkLib.c
    This file contains code for Common function that can be used for Network and ISCSI drivers 
**/
#include <Token.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/AmiNetworkLib.h>
#include <Protocol/UsbIo.h> 
#include <Protocol/DevicePath.h>
#include <NetworkStackSetup.h>
#include <Library/UefiRuntimeServicesTableLib.h>



extern  EFI_BOOT_SERVICES    *gBS;

/**
    Returns whether network device on the USB Controller to be supported for 
    different UEFI Network Feature 
    
    For each features like PXE, ISCSI etc the Blocked device list also passed to the function. 
    Based on the Input Network Contoller, the vendor Id and device Id checked for the blocked 
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
  )
{
    EFI_STATUS                          Status;
    EFI_USB_DEVICE_DESCRIPTOR           DevDesc;
    EFI_USB_IO_PROTOCOL                 *UsbIo = NULL;
    UINTN                               Index=0;
    EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
    
    if(BlockedDeviceList == NULL || ControllerHandle == NULL ) {
        return FALSE;
    }
    
    // Get the DevicePath of the Controller Handle
    Status = gBS->HandleProtocol(ControllerHandle, 
                          &gEfiDevicePathProtocolGuid,
                          (VOID*)&DevicePath);
    
    if(EFI_ERROR(Status)) {
        return FALSE;
    }
    
    Status= gBS->LocateDevicePath(&gEfiUsbIoProtocolGuid, 
                            &DevicePath, 
                            &ControllerHandle
                        );

    if(EFI_ERROR(Status)) {
        return FALSE;
    }
    
    // Get the UsbIo Protocol
    Status = gBS->HandleProtocol(ControllerHandle,
                                   &gEfiUsbIoProtocolGuid,
                                   (VOID**)&UsbIo
                                    );
    
    if(EFI_ERROR(Status)) {
        return FALSE;
    }
    
    // Get the Device
    Status = UsbIo->UsbGetDeviceDescriptor(UsbIo, &DevDesc);
    
    if(EFI_ERROR(Status)) {
        return FALSE;
    }
    
    // Search the device until end of the device Found
    while ((BlockedDeviceList[Index].VendorId != 0xFFFF) && (BlockedDeviceList[Index].ProductId != 0xFFFF)) {
        
        if (BlockedDeviceList[Index].VendorId == DevDesc.IdVendor &&
                BlockedDeviceList[Index].ProductId == DevDesc.IdProduct) {
            // Device Found in the Blocked device list and return TRUE
            return TRUE;
        }
        Index++;
    }

    // Device Not found in the Blocked network device List.
    return FALSE;
    
}

/**
    Return the UefiNetwork Stack Driver start status based on the Setup option. 

    @param  NULL 

    @return 
    TRUE - Start the Network Stack drivers 
    FALSE- Don't start Uefi Network Drivers 

**/
BOOLEAN 
CheckNetworkDriverLoadStatus (
  )
{
    
#if (LOAD_DRIVER_ONLY_ON_NWSTACK_ENABLE == 1)
    NETWORK_STACK       NetworkStackSetupData;
    UINTN               VarSize;
    EFI_STATUS          Status; 

    VarSize = sizeof (NETWORK_STACK);

    Status= gRT->GetVariable(L"NetworkStackVar", 
                   &gEfiNetworkStackSetupGuid, 
                   NULL, 
                   &VarSize, 
                   &NetworkStackSetupData);
       if (Status != EFI_SUCCESS) {
        //Return the default SDL token value when the variable not found 
        return NWSTACK_DEFAULT_VALUE; 
    }

    if (NetworkStackSetupData.Enable == 0){
        // Based on the Setup option, Drivers are disabled. 
        return FALSE;
    }
#endif  

    // Load all the Network Stack drivers. 
    return TRUE;
  
}

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
  )
{
    NETWORK_STACK       NetworkStackSetupData;
    UINTN               VarSize;
    EFI_STATUS          Status; 

    VarSize = sizeof (NETWORK_STACK);

    Status= gRT->GetVariable(L"NetworkStackVar", 
                   &gEfiNetworkStackSetupGuid, 
                   NULL, 
                   &VarSize, 
                   &NetworkStackSetupData);
    if (Status == EFI_SUCCESS) {
        *Ipv4PxeBoot=NetworkStackSetupData.Ipv4Pxe;
        *Ipv6PxeBoot=NetworkStackSetupData.Ipv6Pxe;
        *PxeBootWaitTime=NetworkStackSetupData.PxeBootWaitTime;
        *MediaDetectCount=NetworkStackSetupData.MediaDetectCount;
    } else {
        //
        // Variable not found. Load the defaults from the SDL token 
        //
        *Ipv4PxeBoot=NETWORKSTACK_IPV4_PXE_SUPPORT;
        *Ipv6PxeBoot=NETWORKSTACK_IPV6_PXE_SUPPORT;
        *PxeBootWaitTime=0;
        *MediaDetectCount=MIN_MEDIA_DETECT_COUNT;
    }
    return EFI_SUCCESS;
}

/**
    Get the Http Boot Setup option. If the Setup option variable not found
    Load the default value from the SDL token.

    @param  *Ipv4HttpBoot    Setup option value for Ipv4 Http Boot
    @param  *Ipv6HttpBoot    Setup option value for Ipv6 Http Boot
    @param  *MediaDetectCount   Retry count for the Media Detect

    @return 
    EFI_SUCCESS 

**/
EFI_STATUS 
GetHttpBootSetupOptions (
    UINT8   *Ipv4HttpBoot,
    UINT8   *Ipv6HttpBoot,
    UINT8   *MediaDetectCount
  )
{
    NETWORK_STACK       NetworkStackSetupData;
    UINTN               VarSize;
    EFI_STATUS          Status; 

    VarSize = sizeof (NETWORK_STACK);
    Status= gRT->GetVariable(L"NetworkStackVar", 
                   &gEfiNetworkStackSetupGuid, 
                   NULL, 
                   &VarSize, 
                   &NetworkStackSetupData);
       if (Status == EFI_SUCCESS) {
        *Ipv4HttpBoot=NetworkStackSetupData.Ipv4Http;
        *Ipv6HttpBoot=NetworkStackSetupData.Ipv6Http;
        *MediaDetectCount=NetworkStackSetupData.MediaDetectCount;
      } else {
        // Setup variable not found. return the default value based on the SDL token.
        *Ipv4HttpBoot=NETWORKSTACK_IPV4_HTTP_SUPPORT;
        *Ipv6HttpBoot=NETWORKSTACK_IPV6_HTTP_SUPPORT;
        *MediaDetectCount=MIN_MEDIA_DETECT_COUNT;
    }
    return EFI_SUCCESS;
}






