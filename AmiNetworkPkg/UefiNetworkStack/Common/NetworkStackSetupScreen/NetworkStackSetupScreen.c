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

//**********************************************************************
/** @file NetworkStackSetupScreen.c
    NetworkStackSetupScreen related functions

**/
//**********************************************************************
#include <AmiDxeLib.h>
#include <NetworkStackSetup.h>
#include <Token.h>

NETWORK_STACK  	mNetworkStackData;


/**
    Entry point of the  network stack setup driver. This entry point is 
    necessary to initialize the NetworkStack setup driver.

    @param 
        ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
        SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table  
 
    @retval 
  EFI_SUCCESS:              Driver initialized successfully
**/

EFI_STATUS EFIAPI InitNetworkStackVar (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{

  UINTN                Size;
  EFI_STATUS           Status;

  InitAmiLib(ImageHandle,SystemTable);

  Size = sizeof(NETWORK_STACK);
  Status = pRS->GetVariable(L"NetworkStackVar",&gEfiNetworkStackSetupGuid, NULL, &Size, &mNetworkStackData);

  if (Status == EFI_NOT_FOUND) {

    pBS->SetMem(&mNetworkStackData, 
                    sizeof(NETWORK_STACK), 
                    0);
    //
    // Sync Default values for Network configuration across the component
    //
    mNetworkStackData.Enable   = NWSTACK_DEFAULT_VALUE;
    mNetworkStackData.Ipv4Http = NETWORKSTACK_IPV4_HTTP_SUPPORT;
    mNetworkStackData.Ipv4Pxe  = NETWORKSTACK_IPV4_PXE_SUPPORT;
    mNetworkStackData.Ipv6Http = NETWORKSTACK_IPV6_HTTP_SUPPORT;
    mNetworkStackData.Ipv6Pxe  = NETWORKSTACK_IPV6_PXE_SUPPORT;
    mNetworkStackData.MediaDetectCount = DEFAULT_MEDIA_DETECT_COUNT;
    
#if (IpSecDxe_SUPPORT == 1)
    mNetworkStackData.IpsecCertificate = 1;
#else   
    mNetworkStackData.IpsecCertificate = 0;
#endif    	
    
   pRS->SetVariable(
         L"NetworkStackVar",&gEfiNetworkStackSetupGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         sizeof(NETWORK_STACK), &mNetworkStackData
     );

  } 

  return EFI_SUCCESS;

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

