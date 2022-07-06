//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

//**********************************************************************
// $Header: $
//
// $Revision: $
//
// $Date: $
//**********************************************************************
// Revision History
// ----------------
// $Log: $
// 
//
//**********************************************************************
//<AMI_FHDR_START>
//
// Name:
//
// Description:
//
//<AMI_FHDR_END>
//**********************************************************************
//----------------------------------------------------------------------------
// Includes
// Statements that include other files
#include <AmiDxeLib.h>
#include <Protocol\ActiveManagementProtocol.h>

//----------------------------------------------------------------------------
// Function Externs
EFI_ACTIVE_MANAGEMENT_PROTOCOL  *mActiveManagement = NULL;
EFI_INSTALL_PROTOCOL_INTERFACE    gSavedInstallProtocol;

VOID InstallConInStartedProtocol(VOID);
extern EFI_GUID ConInStartedProtocolGuid;

//----------------------------------------------------------------------------
// Local prototypes
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   
//
// Description: 
//
// Input:       
//
// Output:      
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
EFI_STATUS
EFIAPI
PrivateInstallProtocolInterface (
  IN OUT EFI_HANDLE     *UserHandle,
  IN EFI_GUID           *Protocol,
  IN EFI_INTERFACE_TYPE InterfaceType,
  IN VOID               *Interface
  )
{
    if (!guidcmp(Protocol, &ConInStartedProtocolGuid)) return EFI_SUCCESS;
    return gSavedInstallProtocol(UserHandle, Protocol, InterfaceType, Interface);
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   
//
// Description: 
//
// Input:       
//
// Output:      
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN IsSolEnabled (VOID) {
    EFI_STATUS          Status;    
    static BOOLEAN      CurrentState = 0xEE;

    if (mActiveManagement == NULL) {
        Status = pBS->LocateProtocol (&gEfiActiveManagementProtocolGuid, \
                                        NULL, (VOID **) &mActiveManagement);
        if (EFI_ERROR(Status)) return FALSE;                                
    }                                 
    if (CurrentState == 0xEE) 
        mActiveManagement->GetSolState (mActiveManagement, &CurrentState);
    return CurrentState;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   
//
// Description: 
//
// Input:       
//
// Output:      
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID AmtSolBeforeConInProtocolHook() {
    if (IsSolEnabled() == FALSE) return;
    // if USER Password enabled with SOL enabled, the Password is checked if 
    // ConOut,ConIn and ConsoleControll Protocols are instatlled, but 
    // when the Terminal and PciSerial are not connected yet, so, here we 
    // hooks the pBS->InstallProtocolInterface to postpone the 
    // ConInStartedProtocol to be installed after "ConnectEverything" hook.
    gSavedInstallProtocol = pBS->InstallProtocolInterface;
    pBS->InstallProtocolInterface = PrivateInstallProtocolInterface;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   
//
// Description: 
//
// Input:       
//
// Output:      
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID AmtSolAfterDriverConnectedHook() {
    if (IsSolEnabled() == FALSE) return;
    pBS->InstallProtocolInterface = gSavedInstallProtocol;
    InstallConInStartedProtocol();
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2011, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
