//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
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
//
// $Header: $
//
// $Revision: $
//
// $Date: $
//
//**********************************************************************
// Revision History
// ----------------
// $Log: $
// 
//
//**********************************************************************
//<AMI_FHDR_START>
//---------------------------------------------------------------------------
// Name:		AMTTseFunc.c
//
// Description:	AMT TSE Functions.
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>
#include <Setup.h>
#include <TseCommon.h>
#include <variable.h>
#include <Protocol\AmtWrapperProtocol.h>
#include <Protocol\AlertStandardFormat.h>

#define SETUP_GUID \
        { 0xEC87D643, 0xEBA4, 0x4BB5, 0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9 }

#define	BOOT_MANAGER_GUID \
	{ 0xB4909CF3, 0x7B93, 0x4751, 0x9B, 0xD8, 0x5B, 0xA8, 0x22, 0x0B, 0x9B, 0xB2 }

#define EFI_BOOT_SCRIPT_SAVE_PROTOCOL_GUID \
  { \
    0x470e1529, 0xb79e, 0x4e32, 0xa0, 0xfe, 0x6a, 0x15, 0x6d, 0x29, 0xf9, 0xb2 \
  }

#define AMI_EFI_SOL_POST_MESSAGE_GUID \
 { 0xf42f3752, 0x12e, 0x4812, 0x99, 0xe6, 0x49, 0xf9, 0x43, 0x4, 0x84, 0x54 }

EFI_GUID gEfiAmtWrapperProtocolGuidTse = EFI_AMT_WRAPPER_PROTOCOL_GUID;

extern BOOLEAN gEnterSetup;

VOID AMTBypassPasswordRestore(VOID);

extern EFI_BOOT_SERVICES *gBS;
extern EFI_SYSTEM_TABLE *gST;
extern EFI_RUNTIME_SERVICES *gRT;

//*******************************************************************************
//<AMI_PHDR_START>
// Procedure:	AMTProcessProceedToBootHook
//
// Description:	
//
// Input:		Event: Timer event.
//              Context: Event context; always NULL
//
// Output:		VOID
//
//<AMI_PHDR_END>
//*******************************************************************************
VOID AMTProcessProceedToBootHook ( EFI_EVENT Event, VOID *Context)
{
    AMT_WRAPPER_PROTOCOL *pAmtWrapper = NULL;
    EFI_STATUS            Status;
    EFI_ALERT_STANDARD_FORMAT_PROTOCOL  *AsfCheck;
    EFI_ASF_BOOT_OPTIONS  *mInternalAsfBootOptions;

    if (pAmtWrapper == NULL) {
        Status = gBS->LocateProtocol(&gEfiAmtWrapperProtocolGuidTse, NULL, &pAmtWrapper);
    }

    //case IDER
    if (pAmtWrapper != NULL) {
        if (pAmtWrapper->ActiveManagementEnableIdeR()||pAmtWrapper->ActiveManagementEnableSol()){
        Status = pAmtWrapper->BdsBootViaAsf();
        return;
        }
    }

    //case ASF
    //Get the ASF options
    //if set then we have to do and Asfboot
    Status = gBS->LocateProtocol (
                    &gEfiAlertStandardFormatProtocolGuid,
                    NULL,
                    &AsfCheck
                    );
	
    if (EFI_ERROR (Status)) {
        return;
    }

    Status = AsfCheck->GetBootOptions (AsfCheck, &mInternalAsfBootOptions);
	  
    if (mInternalAsfBootOptions->SubCommand != ASF_BOOT_OPTIONS_PRESENT) {
        return;
    }else{
        Status = pAmtWrapper->BdsBootViaAsf();
        return;
    }
}


//*******************************************************************************
//<AMI_PHDR_START>
// Procedure:	AMTProcessEnterSetupHook
//
// Description:	
//
// Input:		Event: Timer event.
//              Context: Event context; always NULL
//
// Output:		VOID
//
//<AMI_PHDR_END>
//*******************************************************************************
VOID AMTProcessEnterSetupHook  ( EFI_EVENT Event, VOID *Context)
{
    EFI_STATUS Status;
    UINT16	   count = 0;
    EFI_GUID BootManGuid = BOOT_MANAGER_GUID;
    AMT_WRAPPER_PROTOCOL *pAmtWrapper = NULL;

    if (pAmtWrapper == NULL) 
        Status = gBS->LocateProtocol(&gEfiAmtWrapperProtocolGuidTse, NULL, &pAmtWrapper);
    if (pAmtWrapper != NULL) {		  
        if (pAmtWrapper->ActiveManagementEnableIdeR()) {
            count = 0xFFFF;
        }
    }
    if (count == 0xFFFF){ 
        gRT->SetVariable( L"BootManager", &BootManGuid, EFI_VARIABLE_BOOTSERVICE_ACCESS,sizeof(count),&count );}
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	AMTBypassPasswordRestore
//
// Description:	This function is a hook called when TSE determines
//              that SETUP utility has to be displayed. This function
//              is available as ELINK. In the generic implementation
//              setup password is prompted in this function.
//
// Input:		VOID
//
// Output:		VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN AMTProcessConInAvailabilityHook  (VOID)
{
    AMT_WRAPPER_PROTOCOL *pAmtWrapper = NULL;
    EFI_HANDLE	SolPostMessageHandle = NULL;
    EFI_STATUS Status;
    EFI_GUID gAmiEfiSolPostMessageGuid = AMI_EFI_SOL_POST_MESSAGE_GUID;

    // Handle EnterSetup Flag here !!
    if (pAmtWrapper == NULL){
        Status = gBS->LocateProtocol(&gEfiAmtWrapperProtocolGuidTse, NULL, &pAmtWrapper);
    }
    if(EFI_ERROR(Status))
        return FALSE;
    if(pAmtWrapper->ActiveManagementEnterSetup())
        gEnterSetup = TRUE;

    // Install Protocol here, Trig Callback event to show SOL message.
    Status = gBS->InstallMultipleProtocolInterfaces(
                 &SolPostMessageHandle,
                 &gAmiEfiSolPostMessageGuid,
                 NULL,
                 NULL);


    return FALSE;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	AMTBypassPasswordRestore
//
// Description:	This function is a hook called when TSE determines
//              that SETUP utility has to be displayed. This function
//              is available as ELINK. In the generic implementation
//              setup password is prompted in this function.
//
// Input:		VOID
//
// Output:		VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID AMTBypassPasswordRestore(VOID)
{

    EFI_STATUS Status;
    UINTN VariableSize;
    EFI_GUID gAmiTseSetupGuid = AMITSESETUP_GUID;
    AMITSESETUP AmiTseData;
#if defined(TSE_ADVANCED_BIN_SUPPORT)
    AMITSESETUP *pSetupVariable;
#endif
    VariableSize = sizeof(AMITSESETUP);
    Status = gRT->GetVariable ( L"AMITSESetupBackup", \
                                &gAmiTseSetupGuid, \
                                NULL, \
                                &VariableSize, \
                                &AmiTseData );

    // If we get this variable, restore it back
    if (!EFI_ERROR (Status)) {

        VariableSize = sizeof(AMITSESETUP);
        Status = gRT->SetVariable ( L"AMITSESetup", \
                                    &gAmiTseSetupGuid, \
                                    EFI_VARIABLE_NON_VOLATILE | \
                                    EFI_VARIABLE_RUNTIME_ACCESS | \
                                    EFI_VARIABLE_BOOTSERVICE_ACCESS, \
                                    VariableSize, \
                                    &AmiTseData );

        VariableSize = 0;
        Status = gRT->SetVariable ( L"AMITSESetupBackup", \
                                    &gAmiTseSetupGuid, \
                                    EFI_VARIABLE_NON_VOLATILE | \
                                    EFI_VARIABLE_RUNTIME_ACCESS | \
                                    EFI_VARIABLE_BOOTSERVICE_ACCESS, \
                                    VariableSize, \
                                    &AmiTseData );
#if defined(TSE_ADVANCED_BIN_SUPPORT)
        VariableSize = sizeof(AMITSESETUP);
        pSetupVariable = VarGetVariable( VARIABLE_ID_AMITSESETUP, &VariableSize );
        if ( ( pSetupVariable == NULL ) || ( VariableSize != sizeof(AMITSESETUP) ) )
            pSetupVariable = VarGetNvram( VARIABLE_ID_AMITSESETUP, &VariableSize );
        MemCopy(pSetupVariable->UserPassword, AmiTseData.UserPassword,
            sizeof(pSetupVariable->UserPassword));
        MemCopy(pSetupVariable->AdminPassword, AmiTseData.AdminPassword,
            sizeof(pSetupVariable->AdminPassword));
        VarSetNvram( VARIABLE_ID_AMITSESETUP, \
            pSetupVariable, sizeof(AMITSESETUP));
#endif
    }

}

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	AMTBypassPasswordCheck
//
// Description:	ELink to ProcessConInAvailabilityHook.
//              If AMT Bypass user password, Delete AMITSESetup.
//
// Input:       VOID
//
// Output:      BOOLEAN. Should return TRUE if the screen was used to
//              ask password; FALSE if the screen was not used to ask
//              password.
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
BOOLEAN AMTBypassPasswordCheck(VOID)
{
    EFI_STATUS Status;
    UINTN VariableSize;
    EFI_GUID gAmiTseSetupGuid = AMITSESETUP_GUID;
    AMITSESETUP AmiTseData;
#if defined(TSE_ADVANCED_BIN_SUPPORT)
    AMITSESETUP *pSetupVariable;
#endif
    UINTN VarSize=0;
    EFI_ALERT_STANDARD_FORMAT_PROTOCOL  *Asf;
    EFI_ASF_BOOT_OPTIONS                *mAsfBootOptions;
    EFI_GUID  gSetupGuid = SETUP_GUID;
    mAsfBootOptions = NULL;
    //
    // Get Protocol for ASF
    //
    Status = gBS->LocateProtocol (
                    &gEfiAlertStandardFormatProtocolGuid,
                    NULL,
                    &Asf
                    );
    if (EFI_ERROR (Status)) {
//      DEBUG ((EFI_D_ERROR, "Info : Error gettings ASF protocol -> %r\n", Status));
      return FALSE;
    }

    Status = Asf->GetBootOptions (Asf, &mAsfBootOptions);
    // Configuration data resert.
    if(mAsfBootOptions->BootOptions & 0x8000)
    {
        // Delete Variable.
        VariableSize = 0;
        gRT->SetVariable(
                L"Setup",
                &gSetupGuid,
                EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                VariableSize,
                &VarSize);
    }
    // ByPass Password
    if(mAsfBootOptions->BootOptions & 0x0800)
    {
        VariableSize = sizeof(AMITSESETUP);
        Status = gRT->GetVariable ( L"AMITSESetup", \
                                    &gAmiTseSetupGuid, \
                                    NULL, \
                                    &VariableSize, \
                                    &AmiTseData );

      // No Password installed just return
      if (Status == EFI_NOT_FOUND) {
        return 0;
      }
      // If exist , store a backup then delete AMITSESetup

        VariableSize = sizeof(AMITSESETUP);
        Status = gRT->SetVariable ( L"AMITSESetupBackup", \
                                    &gAmiTseSetupGuid, \
                                    EFI_VARIABLE_NON_VOLATILE | \
                                    EFI_VARIABLE_RUNTIME_ACCESS | \
                                    EFI_VARIABLE_BOOTSERVICE_ACCESS, \
                                    VariableSize, \
                                    &AmiTseData );
        Status = gRT->SetVariable ( L"AMITSESetup", \
                                    &gAmiTseSetupGuid, \
                                    EFI_VARIABLE_NON_VOLATILE | \
                                    EFI_VARIABLE_RUNTIME_ACCESS | \
                                    EFI_VARIABLE_BOOTSERVICE_ACCESS, \
                                    0, \
                                    &AmiTseData );
#if defined(TSE_ADVANCED_BIN_SUPPORT)
        pSetupVariable = VarGetVariable( VARIABLE_ID_AMITSESETUP, &VarSize );
        if ( ( pSetupVariable == NULL ) || ( VarSize != sizeof(AMITSESETUP) ) )
            pSetupVariable = VarGetNvram( VARIABLE_ID_AMITSESETUP, &VarSize );
        MemSet(pSetupVariable->UserPassword, \
            sizeof(pSetupVariable->UserPassword), 0);
        MemSet(pSetupVariable->AdminPassword, \
            sizeof(pSetupVariable->AdminPassword), 0);
        VarSetNvram( VARIABLE_ID_AMITSESETUP, \
            pSetupVariable, sizeof(AMITSESETUP));
#endif
    }else
    {
        // If not bypass password , check AMITSESetupBackup vairblae
        AMTBypassPasswordRestore();
    }

    return 0;
}
//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2010, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
