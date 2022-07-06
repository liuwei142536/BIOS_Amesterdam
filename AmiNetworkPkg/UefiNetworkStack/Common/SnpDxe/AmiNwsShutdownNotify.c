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
/** @file AmiNwsShutdownNotify.c
    This file contains implementation for snp shutdown notifications.
**/
#include <Token.h>
#include <AmiDxeLib.h>
#include <Uefi.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AmiNwsShutdownNotify.h>

extern  EFI_BOOT_SERVICES    *gBS;
extern  EFI_RUNTIME_SERVICES *gRT;


typedef struct {
    LIST_ENTRY                          Link;
    EFI_HANDLE                          Handle;
    NWS_SHUTDOWN_NOTIFICATION_FUNCTION  NotifyFunction;
} NWS_SHUTDOWN_FUNCTIONS_LIST;

LIST_ENTRY  gNwsShutdownFunctionsList;
BOOLEAN     gInitHead = FALSE;

/**
    Registers the callback function for snp shutdown notification

    @param[in]  Handle            Snp Handle to which notification function need to be registered
    @param[in]  NotifyFunction    Notification function to be notified on snp shutdown for the input handle
  
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER If NotifyFunction or Handle parameter is NULL
    @retval EFI_UNSUPPORTED       If the input parameter is already registered
    @retval EFI_NOT_FOUND         Resource not avilable for the New entry 
    @retval EFI_SUCCESS           The function is registered successfully.
**/

EFI_STATUS
ShutdownSnpRegisterCallbackHandler (
    IN EFI_HANDLE                           Handle,
    IN NWS_SHUTDOWN_NOTIFICATION_FUNCTION   NotifyFunction
    )
{
    NWS_SHUTDOWN_FUNCTIONS_LIST *NewEntry = NULL;
    NWS_SHUTDOWN_FUNCTIONS_LIST *NextLink = NULL;

    if(NotifyFunction == NULL || Handle == NULL ) {
        return EFI_INVALID_PARAMETER; 
    }
    
    // Verify if the Entry already Present
    for(NextLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetFirstNode(&gNwsShutdownFunctionsList);
            !IsNull(&gNwsShutdownFunctionsList, &NextLink->Link); 
            NextLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetNextNode(&gNwsShutdownFunctionsList, &NextLink->Link)
        ){
        if ((NextLink->Handle == Handle) && (NextLink->NotifyFunction == NotifyFunction)) {
            // Entry Already present.
            return EFI_UNSUPPORTED;
        }
    }

    // Allocate the Buffer and Add the New Entry
    NewEntry = AllocateZeroPool (sizeof (NWS_SHUTDOWN_FUNCTIONS_LIST));
    
    if (NULL == NewEntry) {
        return EFI_NOT_FOUND;
    }
    
    NewEntry->Handle = Handle;
    NewEntry->NotifyFunction = NotifyFunction;
    InsertTailList((LIST_ENTRY*)&gNwsShutdownFunctionsList, (LIST_ENTRY*)&(NewEntry->Link));
    
    return EFI_SUCCESS;
}

/**
    Unregistered the callback function from snp shutdown notification

    @param[in]  Handle            Snp Handle to which notification function is registered
    @param[in]  NotifyFunction    Notification function which is registered already
  
    @return EFI_STATUS
    @retval EFI_INVALID_PARAMETER If NotifyFunction or Handle parameter is NULL
    @retval EFI_NOT_FOUND         The input parameter is not registered already
    @retval EFI_SUCCESS           The function is unregistered successfully.
**/

EFI_STATUS
ShutdownSnpUnRegisterCallbackHandler (
   IN EFI_HANDLE                           Handle,
   IN NWS_SHUTDOWN_NOTIFICATION_FUNCTION   NotifyFunction
   )
{
    NWS_SHUTDOWN_FUNCTIONS_LIST *NextLink = NULL;
    
    if(NotifyFunction == NULL || Handle == NULL ) {
        return EFI_INVALID_PARAMETER; 
    }    

    for(NextLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetFirstNode(&gNwsShutdownFunctionsList);
            !IsNull(&gNwsShutdownFunctionsList, &NextLink->Link); 
            NextLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetNextNode(&gNwsShutdownFunctionsList, &NextLink->Link)
        ){
        if ((NextLink->Handle == Handle) && (NextLink->NotifyFunction == NotifyFunction)) {
            // Entry Found and Remove from the Linked List
            RemoveEntryList((LIST_ENTRY*)&(NextLink->Link));
            gBS->FreePool(NextLink);
            return EFI_SUCCESS;
        }
   }
    // Unable to find the entry in the Linked List for the given input data
    return EFI_NOT_FOUND;
}

/**
    Invokes the functions registered for the snp shutdown handle.

    @param[in]  snp handle which is going to shutdown

**/

VOID
CallTheRegisteredShutdownFunctions (
    IN EFI_HANDLE Handle
    )
{
    NWS_SHUTDOWN_FUNCTIONS_LIST *NextLink = NULL, *TmpLink = NULL;
    
    for(NextLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetFirstNode(&gNwsShutdownFunctionsList);
        !IsNull(&gNwsShutdownFunctionsList, &NextLink->Link); 
        NextLink = TmpLink
    ){
        TmpLink = (NWS_SHUTDOWN_FUNCTIONS_LIST*)GetNextNode(&gNwsShutdownFunctionsList, &NextLink->Link);
        if((NextLink->Handle == Handle)) {
            NextLink->NotifyFunction (Handle);
        }
    }
    
    return;
}

// Protocol Initilization
AMI_NWS_SHUTDOWN_NOTIFY_PROTOCOL SnpShutdownNotifyProtocol = {
    ShutdownSnpRegisterCallbackHandler,
    ShutdownSnpUnRegisterCallbackHandler
 };

/**
    Initilize Linked List and Install the AMI Network Shutdown Notify Protocol 

    @param[in]  Snp Controller Handle 

**/

EFI_STATUS
InstallAmiNwsProtocol (
    IN EFI_HANDLE ControllerHandle
    )
{
    
    EFI_STATUS      Status;

    // Initilize the Linked List only one time. 
    if (FALSE == gInitHead) {
        InitializeListHead(&gNwsShutdownFunctionsList);
        gInitHead = TRUE;
    }
    
    Status = gBS->InstallProtocolInterface (
                    &ControllerHandle,
                    &gAmiNwsShutdownNotifyProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &SnpShutdownNotifyProtocol
                    );    
    
    return Status;
}

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
