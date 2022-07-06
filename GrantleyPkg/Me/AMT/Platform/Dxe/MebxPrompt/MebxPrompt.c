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
// $Header: /Alaska/SOURCE/Modules/SharkBayRefCodes/ME/MeWrapper/MeSetup/MebxPrompt.c 2     4/23/12 11:09p Klzhan $
//
// $Revision: 2 $
//
// $Date: 4/23/12 11:09p $
//**********************************************************************

//<AMI_FHDR_START>
//---------------------------------------------------------------------------
// Name:        MebxPrompt.c
//
// Description:	AMT TSE Functions.
//
//---------------------------------------------------------------------------
//<AMI_FHDR_END>
#include <Setup.h>
#include <variable.h>
#include <AmiDxeLib.h>
#include <Protocol\ConsoleControl.h>
#include <Protocol\SimpleTextInEx.h>
#include <Protocol\AmtPlatformPolicy.h>
#include <Guid\MeBiosExtensionSetup.h>
#include <Protocol\MeBiosPayloadData.h>
#include <Protocol\HeciProtocol.h>
#include <MeState.h>
#include <Protocol\AmtWrapperProtocol.h>

extern EFI_STATUS PostManagerDisplayPostMessage( CHAR16 *message );

extern EFI_BOOT_SERVICES      *gBS;
extern EFI_SYSTEM_TABLE       *gST;
extern EFI_RUNTIME_SERVICES   *gRT;
ME_BIOS_EXTENSION_SETUP       MeBiosExtensionSetupData;

EFI_HANDLE                    MebxKeyHandle[2];
EFI_HANDLE                    CIRAKeyHandle[2];
DXE_AMT_POLICY_PROTOCOL       *mDxePlatformAmtPolicy = NULL;
EFI_GUID                      gDxePlatformAmtPolicyGuid = DXE_PLATFORM_AMT_POLICY_GUID;
#define EFI_EVENT_ME_PLATFORM_READY_TO_BOOT \
  { \
    0x3fdf171, 0x1d67, 0x4ace, 0xa9, 0x4, 0x3e, 0x36, 0xd3, 0x38, 0xfa, 0x74 \
  }
EFI_GUID      gEfiSimpleTextInExProtocolGuid = EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL_GUID;

EFI_GUID      gMePlatformReadyToBootGuid = EFI_EVENT_ME_PLATFORM_READY_TO_BOOT;
//<AMI_PHDR_START>
//*******************************************************************************
// Procedure:	SetConsoleControlModeBack
//
// Description:	Set to GraphicMode
//
// Input:	Event
//              Context
//
// Output:      VOID
//
//*******************************************************************************
//<AMI_PHDR_END>
EFI_STATUS
SetConsoleControlModeBack(
  EFI_EVENT   Event,
  VOID        *ParentImageHandle
)
{
  EFI_STATUS                        Status;
  EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;

  Status = gBS->LocateProtocol( &gEfiConsoleControlProtocolGuid, NULL, &ConsoleControl);

 //Set back to Graphics
  Status = ConsoleControl->SetMode( ConsoleControl, EfiConsoleControlScreenGraphics );

  gBS->CloseEvent(Event);
  return Status;
}

//<AMI_PHDR_START>
//*******************************************************************************
// Procedure:	CIRACheckForKey
//
// Description:	
//
// Input:	Event: Timer event.
//              Context: Event context; always NULL
//
// Output:		VOID
//
//*******************************************************************************
//<AMI_PHDR_END>
EFI_STATUS CIRACheckForKey
(
  IN EFI_KEY_DATA *Key
)
{
#if CIRA_SUPPORT
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEX;
  EFI_STATUS                        Status;

  if(mDxePlatformAmtPolicy == NULL)
  {    
    Status = gBS->LocateProtocol (&gDxePlatformAmtPolicyGuid, NULL, &mDxePlatformAmtPolicy);
    if (EFI_ERROR(Status)) return Status;
  }

  Status = gBS->HandleProtocol(gST->ConsoleInHandle, 
                              &gEfiSimpleTextInExProtocolGuid, 
                              (void*)&SimpleTextInEX);

  if (EFI_ERROR(Status)) return Status;

  Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, CIRAKeyHandle[0]);
  if((CIRA_UNICODE>0x40 && CIRA_UNICODE<0x5b) || (CIRA_UNICODE>0x60 && CIRA_UNICODE<0x7b)){
    Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, CIRAKeyHandle[1]);
  }

  mDxePlatformAmtPolicy->AmtConfig.CiraRequest = 1;
  PostManagerDisplayPostMessage(L"Requesting CIRA ......");
#endif
  return EFI_SUCCESS;
}
//<AMI_PHDR_START>
//*******************************************************************************
// Procedure:	MebxCheckForKey
//
// Description:	
//
// Input:	Event: Timer event.
//              Context: Event context; always NULL
//
// Output:		VOID
//
//*******************************************************************************
//<AMI_PHDR_END>
EFI_STATUS MebxCheckForKey
(
  IN EFI_KEY_DATA *Key
)
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEX = NULL;
  EFI_STATUS                        Status;
  EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;
  EFI_EVENT                         SetConsoleControlModeBackEvent;
  EFI_CONSOLE_CONTROL_SCREEN_MODE   ScreenMode;

  if(mDxePlatformAmtPolicy == NULL)
  {    
    Status = gBS->LocateProtocol (&gDxePlatformAmtPolicyGuid, NULL, &mDxePlatformAmtPolicy);
    if (EFI_ERROR(Status)) return Status;
  }

  if(SimpleTextInEX == NULL) {
  Status = gBS->HandleProtocol(gST->ConsoleInHandle, 
                               &gEfiSimpleTextInExProtocolGuid, 
                               (void*)&SimpleTextInEX);

  if (EFI_ERROR(Status)) return Status;

  //Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, MebxKeyHandle[0]);
  if((MEBX_UNICODE>0x40 && MEBX_UNICODE<0x5b) || (MEBX_UNICODE>0x60 && MEBX_UNICODE<0x7b)){
    Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, MebxKeyHandle[1]);
  }
}
  if (mDxePlatformAmtPolicy->AmtConfig.iAmtbxHotkeyPressed == 1) return EFI_SUCCESS;
  mDxePlatformAmtPolicy->AmtConfig.iAmtbxHotkeyPressed = 1;
  PostManagerDisplayPostMessage(L"Entering MEBX setup menu ......");

  // Fix Mebx Setup screen not full screen.
  Status = gBS->LocateProtocol( &gEfiConsoleControlProtocolGuid, NULL, &ConsoleControl);

  ConsoleControl->GetMode(ConsoleControl, &ScreenMode, NULL, NULL);

  if (ScreenMode == EfiConsoleControlScreenGraphics) 
  {
    ConsoleControl->SetMode( ConsoleControl, EfiConsoleControlScreenText );
    Status = gBS->CreateEventEx (
                    EFI_EVENT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SetConsoleControlModeBack,
                    NULL,
                    &gMePlatformReadyToBootGuid,
                    &SetConsoleControlModeBackEvent
                    );
  }

  return EFI_SUCCESS;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	MebxPromptConInAvailabilityHook
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
BOOLEAN MebxPromptConInAvailabilityHook  (VOID)
{
    EFI_STATUS        Status;
//    EFI_GUID          gSetupGuid = SETUP_GUID;
//    UINTN             VarSize;
//    SETUP_DATA        SetupData;
#if MEBX_UNICODE>0x40 && MEBX_UNICODE<0x5b == 1
                      // ScanCode, UnicodeChar, KeyShiftState, KeyToggleState
    EFI_KEY_DATA      Key[] = {{MEBX_SCANCODE, MEBX_UNICODE, MEBX_KeyShiftState, MEBX_KeyToggleState},
                               {MEBX_SCANCODE, MEBX_UNICODE+0x20, MEBX_KeyShiftState, MEBX_KeyToggleState}};
#else
#if MEBX_UNICODE>0x60 && MEBX_UNICODE<0x7b == 1
    EFI_KEY_DATA      Key[] = {{MEBX_SCANCODE, MEBX_UNICODE-0x20, MEBX_KeyShiftState, MEBX_KeyToggleState},
                               {MEBX_SCANCODE, MEBX_UNICODE, MEBX_KeyShiftState, MEBX_KeyToggleState}};
#else
    EFI_KEY_DATA      Key[] = {{MEBX_SCANCODE, MEBX_UNICODE, MEBX_KeyShiftState, MEBX_KeyToggleState}};
#endif
#endif
    EFI_HECI_PROTOCOL *Heci;
    UINT32            MeMode = ME_MODE_FAILED;
    EFI_GUID          gEfiHeciProtocolGuid = HECI_PROTOCOL_GUID;
    EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEX;
    EFI_GUID          gMeBiosPayloadDataProtocolGuid = 
                      ME_BIOS_PAYLOAD_DATA_PROTOCOL_GUID;
//Aptiov server override. Remove the iAMT_SUPPORT.					  
#if defined AMT_SUPPORT && AMT_SUPPORT == 1
    EFI_GUID      gEfiAmtWrapperGuid = EFI_AMT_WRAPPER_PROTOCOL_GUID;
    AMT_WRAPPER_PROTOCOL              *AmtWrapperProtocol;

    Status = gBS->LocateProtocol (&gEfiAmtWrapperGuid, NULL, &AmtWrapperProtocol);
    if (EFI_ERROR(Status)) return FALSE;
    if ((AmtWrapperProtocol->ActiveManagementEnableKvm()) ||
        (AmtWrapperProtocol->ActiveManagementEnableSol()))
        return FALSE;
#endif
/*    VarSize = sizeof(SETUP_DATA);
    Status = gRT->GetVariable (
                    L"IntelSetup", //AptioV server override
                    &gSetupGuid,
                    NULL,
                    &VarSize,
                    &SetupData
                    );
    if (EFI_ERROR(Status)) return FALSE;
    if(SetupData.MeImageType == 3)
      return FALSE;*/
    Status = pBS->LocateProtocol (
                    &gEfiHeciProtocolGuid,
                    NULL,
                    &Heci
                    );

    if (EFI_ERROR(Status)) return EFI_SUCCESS; 

    Status = Heci->GetMeMode (&MeMode);

    // If not normal mode
    if(MeMode != 0)
      return FALSE;

    if(mDxePlatformAmtPolicy == NULL)
    {    
        Status = gBS->LocateProtocol (&gDxePlatformAmtPolicyGuid, NULL, &mDxePlatformAmtPolicy);

        if (EFI_ERROR(Status)) 
            return FALSE;
    }

    if(mDxePlatformAmtPolicy->AmtConfig.iAmtbxHotkeyPressed == 1)
    {
        EFI_EVENT        SetConsoleControlModeBackEvent;
        EFI_CONSOLE_CONTROL_PROTOCOL      *ConsoleControl;
        EFI_CONSOLE_CONTROL_SCREEN_MODE   ScreenMode;

        // Fix Mebx Setup screen not full screen.
        Status = gBS->LocateProtocol( 
                 &gEfiConsoleControlProtocolGuid, NULL, &ConsoleControl);



        ConsoleControl->GetMode(ConsoleControl, &ScreenMode, NULL, NULL);
        if (ScreenMode == EfiConsoleControlScreenGraphics) {
          ConsoleControl->SetMode( ConsoleControl, EfiConsoleControlScreenText );

          Status = gBS->CreateEventEx (
                          EFI_EVENT_NOTIFY_SIGNAL,
                          TPL_CALLBACK,
                          SetConsoleControlModeBack,
                          NULL,
                          &gMePlatformReadyToBootGuid,
                          &SetConsoleControlModeBackEvent
                          );
        }
        return FALSE;
    }

    PostManagerDisplayPostMessage(L"Press <CTRL + P> to Enter MEBX setup menu ");

    Status = gBS->HandleProtocol(gST->ConsoleInHandle, &gEfiSimpleTextInExProtocolGuid, (void*)&SimpleTextInEX);
    if (EFI_ERROR(Status)) return FALSE;


    Status = SimpleTextInEX->RegisterKeyNotify (SimpleTextInEX, &Key[0], &MebxCheckForKey, &MebxKeyHandle[0]);
    if((MEBX_UNICODE>0x40 && MEBX_UNICODE<0x5b) || (MEBX_UNICODE>0x60 && MEBX_UNICODE<0x7b)){
        Status = SimpleTextInEX->RegisterKeyNotify (SimpleTextInEX, &Key[1], &MebxCheckForKey, &MebxKeyHandle[1]);
    }

    return FALSE;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	RemoteAssistConInAvailabilityHook
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
BOOLEAN RemoteAssistConInAvailabilityHook  (VOID)
{
#if CIRA_SUPPORT
    EFI_STATUS    Status;
    UINTN         VariableSize = sizeof (ME_BIOS_EXTENSION_SETUP);
    EFI_GUID      gEfiMeBiosExtensionSetupGuid = EFI_ME_BIOS_EXTENSION_SETUP_GUID;
    CHAR16        gEfiMeBiosExtensionSetupName[] = EFI_ME_BIOS_EXTENSION_SETUP_VARIABLE_NAME;
    EFI_GUID      gEfiAmtWrapperGuid = EFI_AMT_WRAPPER_PROTOCOL_GUID;
    AMT_WRAPPER_PROTOCOL              *AmtWrapperProtocol;

    Status = gBS->LocateProtocol (&gDxePlatformAmtPolicyGuid, NULL, &mDxePlatformAmtPolicy);
    if (EFI_ERROR(Status)) return FALSE;

    Status = gBS->LocateProtocol (&gEfiAmtWrapperGuid, NULL, &AmtWrapperProtocol);
    if (EFI_ERROR(Status)) return FALSE;

    Status = gRT->GetVariable (
             gEfiMeBiosExtensionSetupName,
             &gEfiMeBiosExtensionSetupGuid,
             NULL,
             &VariableSize,
             &MeBiosExtensionSetupData);

    if(EFI_ERROR(Status))
        return FALSE;

    if((MeBiosExtensionSetupData.RemoteAssistanceTriggerAvailablilty) &&
       (mDxePlatformAmtPolicy->AmtConfig.iAmtEnabled) &&
       (!AmtWrapperProtocol->ActiveManagementEnableKvm()) &&
       (!AmtWrapperProtocol->ActiveManagementEnableSol()))
    {
#if CIRA_UNICODE>0x40 && CIRA_UNICODE<0x5b == 1
                      // ScanCode, UnicodeChar, KeyShiftState, KeyToggleState
    EFI_KEY_DATA      Key[] = {{CIRA_SCANCODE, CIRA_UNICODE, CIRA_KeyShiftState, CIRA_KeyToggleState},
                               {CIRA_SCANCODE, CIRA_UNICODE+0x20, CIRA_KeyShiftState, CIRA_KeyToggleState}};

#else
#if CIRA_UNICODE>0x60 && CIRA_UNICODE<0x7b == 1
                      // ScanCode, UnicodeChar, KeyShiftState, KeyToggleState
    EFI_KEY_DATA      Key[] = {{CIRA_SCANCODE, CIRA_UNICODE-0x20, CIRA_KeyShiftState, CIRA_KeyToggleState},
                               {CIRA_SCANCODE, CIRA_UNICODE, CIRA_KeyShiftState, CIRA_KeyToggleState}};
#else
    EFI_KEY_DATA      Key[] = {{CIRA_SCANCODE, CIRA_UNICODE, CIRA_KeyShiftState, CIRA_KeyToggleState}};
#endif
#endif

        EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEX;


        Status = gBS->HandleProtocol(gST->ConsoleInHandle,
                      &gEfiSimpleTextInExProtocolGuid, (void*)&SimpleTextInEX);

        if (EFI_ERROR(Status)) return FALSE;
        PostManagerDisplayPostMessage(L"Press <CTRL + ALT + F1> Remote Assistance");
        Status = SimpleTextInEX->RegisterKeyNotify (SimpleTextInEX, &Key[0], 
                                                    &CIRACheckForKey, &CIRAKeyHandle[0]);
        if((CIRA_UNICODE>0x40 && CIRA_UNICODE<0x5b) || (CIRA_UNICODE>0x60 && CIRA_UNICODE<0x7b)){
            Status = SimpleTextInEX->RegisterKeyNotify (SimpleTextInEX, &Key[1], 
                                                    &CIRACheckForKey, &CIRAKeyHandle[1]);
        }

    }
#endif //CIRA_SUPPORT
    return FALSE;
}
//<AMI_PHDR_START>
//----------------------------------------------------------------------------
// Procedure:	MEProcessEnterSetup
//
// Description:	This function is a hook called when TSE determines
//              that it has to load the boot options in the boot
//              order. This function is available as ELINK.
//
// Input:               VOID
//
// Output:              VOID
//
//----------------------------------------------------------------------------
//<AMI_PHDR_END>
VOID MEProcessEnterSetup(VOID)
{
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *SimpleTextInEX;
  EFI_STATUS                        Status;

  PostManagerDisplayPostMessage(L"ME ProcessEnterSetup...");
  
  Status = gBS->HandleProtocol(gST->ConsoleInHandle, 
                              &gEfiSimpleTextInExProtocolGuid, 
                              (void*)&SimpleTextInEX);
  if(EFI_ERROR(Status))
      return;

#if CIRA_SUPPORT
  Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, CIRAKeyHandle[0]);
  if((CIRA_UNICODE>0x40 && CIRA_UNICODE<0x5b) || (CIRA_UNICODE>0x60 && CIRA_UNICODE<0x7b)){
    Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, CIRAKeyHandle[1]);
  }
#endif

  Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, MebxKeyHandle[0]);
  if((MEBX_UNICODE>0x40 && MEBX_UNICODE<0x5b) || (MEBX_UNICODE>0x60 && MEBX_UNICODE<0x7b)){
    Status = SimpleTextInEX->UnregisterKeyNotify (SimpleTextInEX, MebxKeyHandle[1]);
  }
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
