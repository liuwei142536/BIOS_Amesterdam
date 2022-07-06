//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file AmiNwsShutdownNotify.h
    This file defines AMI defined Protocol interface to
    notify the user on shutting down the Snp
**/

#ifndef _AMI_NWS_SHUTDOWN_NOTIFY_
#define _AMI_NWS_SHUTDOWN_NOTIFY_


#define AMI_NWS_SHUTDOWN_NOTIFY_PROTOCOL_GUID  \
    { 0xc667a390, 0xbcca, 0x4261, {0x90, 0x4, 0xa0, 0xdb, 0x9, 0x3b, 0x5e, 0x93} }

typedef struct _AMI_NWS_SHUTDOWN_NOTIFY_PROTOCOL   AMI_NWS_SHUTDOWN_NOTIFY_PROTOCOL;


/**
    Function need to be passed an argument for registering or unregistering
    
    @param[in] Handle   SNP handle
  
**/

typedef
void
(EFIAPI *NWS_SHUTDOWN_NOTIFICATION_FUNCTION) (
    IN EFI_HANDLE  Handle
);

/**
    Register the function which need to be notified on snp shutdown, or unregistered the registered function.
    
    @param[in] ShutdownNotifyFunction       Notification function need to be called on snp shutdown
    
    @retval EFI_SUCCESS                     Function registered or unregistered successfully.
    @retval EFI_INVALID_PARAMETER           Function registration or unregistration went wrong.
**/

typedef
EFI_STATUS
(EFIAPI *NWS_SHUTDOWN_REGISTER_NOTIFICATION) (
    IN EFI_HANDLE                           Handle,     
    IN NWS_SHUTDOWN_NOTIFICATION_FUNCTION   ShutdownNotifyFunction
);


// AMI Network Shutdown Notify Protocol 

struct _AMI_NWS_SHUTDOWN_NOTIFY_PROTOCOL {
    NWS_SHUTDOWN_REGISTER_NOTIFICATION      RegisterNotification;
    NWS_SHUTDOWN_REGISTER_NOTIFICATION      UnRegisterNotification;
 };

extern EFI_GUID gAmiNwsShutdownNotifyProtocolGuid;

#endif  //_AMI_NWS_SHUTDOWN_NOTIFY_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
